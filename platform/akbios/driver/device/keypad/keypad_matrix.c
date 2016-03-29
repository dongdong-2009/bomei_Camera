/**
 * @FILENAME: keypad.c
 * @BRIEF keypad function file, provide keypad APIs: init, scan...
 * Copyright (C) 2007 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @VERSION 1.0
 * @REF AK3223 and AK3224 technical manual.
 */
#include "akdefine.h"
#include "drv_api.h"
#include "keypad_type.h"
#include "drv_keypad.h"
#include "drv_gpio.h"
#include "drv_timer.h"


 
#ifdef MOUNT_MATRIX_SCAN_MODE 
 
#include <string.h>
#define KEYPAD_TIMER        uiTIMER1

#define DEFAULT_UPDOWN_DETECT_LOOP		10      //��ΪĬ��ֵ��10ms loop detect
#define DEFAULT_UPDOWN_LONGKEY_DELAY	50      //��ΪĬ��ֵ��2*10ms*50 = 1s, for long press key
#define DEFAULT_PREVENT_DITHERING_DELAY 10      //��������һ�κ͵ڶ��β���ʱ����
#define DEFAULT_PREVENT_DITHERING_TIMES 2	    //�����Ĵ���
#define KEYPAD_MAX_ROW                  2		//���������
#define KEYPAD_MAX_COLUMN               3		//���������


static T_VOID keypad_updown_scan(T_TIMER timer_id, T_U32 delay);
static T_VOID keypad_gpio_callback(T_U32 column, T_U8 polarity);
static T_VOID keypad_powerkey_callback(T_U32 pin, T_U8 polarity);
static T_VOID downkey_timer(T_TIMER timer_id, T_U32 delay);
static T_VOID powerkey_down_timer(T_TIMER timer_id, T_U32 delay);
static T_VOID multiplekey_timer(T_TIMER timer_id, T_U32 delay);
static T_VOID longkey_timer(T_TIMER timer_id, T_U32 delay);
static T_VOID loopkey_timer(T_TIMER timer_id, T_U32 delay);
static T_VOID upkey_timer(T_TIMER timer_id, T_U32 delay);

typedef struct _KEYPAD_CONTROL_ {
    T_BOOL              key_init;
    T_BOOL              m_is_powerkey;
    T_U32               m_prev_key_id; // previous key ID
    T_U32               m_prevent_dithering_key_id; // prevent_dithering key ID
    T_TIMER             m_keydown_timer;
    T_TIMER             m_downkey_timer_id;
	T_TIMER             m_prevent_dithering_timer_id; //prevent_dithering timer id
    T_TIMER             m_upkey_timer_id;
    T_TIMER             m_longkey_timer_id;
    T_TIMER             m_loopkey_timer_id;
    T_U32	            m_key_row_index;
    
    T_U32               m_long_key_delay;   // long key delay timer 
    T_U32               m_powerkey_long_delay;   // long key delay timer
    T_U32               m_keydown_delay;    // key down dithering delay timer
    T_U32               m_keyup_delay;      // key up dithering delay timer
    T_U32               m_loopkey_delay;
    T_U32	            m_sent_key_num;     // key number which be has been sent
    T_U32               m_prevent_dithering_count;
	T_U32               m_multiplekey_prevent_count[KEYPAD_MAX_ROW][KEYPAD_MAX_COLUMN]; //multiple key prevent dithering times
	T_U32               m_multiplekey_prevent_key_id[KEYPAD_MAX_ROW][KEYPAD_MAX_COLUMN];//multiple key prevent dithering key id
    T_eKEY_PRESSMODE	press_mode;
} T_KEYPAD_CONTROL;

typedef struct _HARDWARE_KEYPAD_ {
    /* user difine */
    T_PLATFORM_KEYPAD_PARM  keypad_parm;
    T_KEYPAD_CONTROL        control;
    T_f_H_KEYPAD_CALLBACK   callback_func;  // global pointer to the timer callback function 
} T_HARDWARE_KEYPAD;


static T_HARDWARE_KEYPAD keypad = {0};

/**
 * @brief Initialize keypad
 * If pointer callback_func is not equal AK_NULL, the keypad interrupt will be enabled
 * Function keypad_init() must be called before call any other keypad functions
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_fKEYPAD_CALLBACK callback_func: Keypad callback function
 * @return T_VOID
 * @retval
 */
static T_VOID kpd_init(T_f_H_KEYPAD_CALLBACK callback_func, const T_PLATFORM_KEYPAD_PARM *keypad_parm)
{
    T_U32 i;

    memset(&keypad, 0, sizeof(keypad));

    if (AK_NULL == keypad_parm 
        || AK_NULL == keypad_parm->RowGpio
        || AK_NULL == keypad_parm->ColumnGpio
        || AK_NULL == keypad_parm->keypad_matrix
        || AK_NULL == keypad_parm->updown_matrix
        || AK_NULL == callback_func)
    {
        akprintf(C2, M_DRVSYS, "keypad init error\r\n");
        keypad.control.key_init = AK_FALSE;
        return;
    }
 
    for (i=0; i<keypad_parm->column_qty; i++)
    {
		gpio_set_pin_as_gpio(keypad_parm->ColumnGpio[i]);	
		gpio_set_pull_up_r(keypad_parm->ColumnGpio[i], AK_FALSE);	
		gpio_set_pull_down_r(keypad_parm->ColumnGpio[i], AK_FALSE);	
		gpio_set_pin_dir(keypad_parm->ColumnGpio[i], GPIO_DIR_OUTPUT);	
		gpio_set_pin_level(keypad_parm->ColumnGpio[i], keypad_parm->active_level);
    }

    for (i=0; i<keypad_parm->row_qty; i++)
    {
		gpio_set_pin_as_gpio(keypad_parm->RowGpio[i]);		
		gpio_set_pull_up_r(keypad_parm->RowGpio[i], AK_FALSE);		
		gpio_set_pull_down_r(keypad_parm->RowGpio[i], AK_FALSE);		
		gpio_set_pin_dir(keypad_parm->RowGpio[i], GPIO_DIR_INPUT);	
		gpio_register_int_callback(keypad_parm->RowGpio[i], keypad_parm->active_level, 
			GPIO_INTERRUPT_DISABLE, keypad_gpio_callback);  
    }
	
    gpio_set_pin_dir(keypad_parm->switch_key_id, GPIO_DIR_INPUT);
    gpio_register_int_callback(keypad_parm->switch_key_id, keypad_parm->switch_key_active_level, GPIO_INTERRUPT_DISABLE, keypad_powerkey_callback);   

    keypad.callback_func = callback_func;    
    keypad.keypad_parm = *keypad_parm;

    keypad.control.m_is_powerkey      = AK_FALSE;
    keypad.control.m_prev_key_id      = kbUnKnown;                      // previous key ID
    keypad.control.m_keydown_timer    = ERROR_TIMER;
    keypad.control.m_downkey_timer_id = ERROR_TIMER;
	keypad.control.m_prevent_dithering_timer_id = ERROR_TIMER;
    keypad.control.m_upkey_timer_id   = ERROR_TIMER;
    keypad.control.m_longkey_timer_id = ERROR_TIMER;
    keypad.control.m_loopkey_timer_id = ERROR_TIMER;
    keypad.control.m_key_row_index    = keypad_parm->row_qty;
    keypad.control.m_long_key_delay   = DEFAULT_LONG_KEY_DELAY;      // long key delay timer 
    keypad.control.m_powerkey_long_delay = DEFAULT_POWERKEY_LONG_DELAY;      // long key delay timer
    keypad.control.m_keydown_delay    = DEFAULT_KEYDOWN_DELAY;       // key down dithering delay timer
    keypad.control.m_keyup_delay      = DEFAULT_KEYUP_DELAY;         // key up dithering delay timer
    keypad.control.m_loopkey_delay    = DEFAULT_LOOP_KEY_DELAY;
    keypad.control.m_sent_key_num     = 0;                           // key number which be has been sent
    keypad.control.press_mode         = eSINGLE_PRESS;
    keypad.control.m_prevent_dithering_count = 0;
	keypad.control.key_init           = AK_TRUE;
}

/**
 * @brief keypad_updown_scan
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID keypad_updown_scan(T_TIMER timer_id, T_U32 delay)
{
    T_U32 i, j;
    T_KEYPAD key;
    T_S32 keypress = 0;
    T_S8 k;
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;
    
    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init\r\n");    
        return;
    }
	
    control->m_prevent_dithering_count++; //��������

	for (i=0; i <keypad_parm->column_qty; i++)
	{
		for (j=0; j<keypad_parm->row_qty; j++)
		{
			control->m_multiplekey_prevent_count[i][j]++;//���𰴼���������
			//key [i][j] down
			if (gpio_get_pin_level(keypad_parm->RowGpio[j]) == keypad_parm->active_level)
		//	if (1)
			{
				//set row as input
				for(k=0; k<keypad_parm->column_qty; k++)
				{
					gpio_set_pin_level(keypad_parm->ColumnGpio[k], (1 - keypad_parm->active_level));
					//gpio_set_pin_dir(keypad_parm->RowGpio[k], GPIO_DIR_INPUT);
				}
				
				//set current row as output high level
				if (gpio_get_pin_level(keypad_parm->RowGpio[j]) == keypad_parm->active_level)
                {
                    if(keypad_parm->ColumnGpio[i] != INVALID_GPIO)
                        continue;
                }

				//set current row as output high level
				//gpio_set_pin_dir(keypad_parm->RowGpio[i], GPIO_DIR_OUTPUT);
				gpio_set_pin_level(keypad_parm->ColumnGpio[i], keypad_parm->active_level);

				if (gpio_get_pin_level(keypad_parm->RowGpio[j]) == keypad_parm->active_level)
				{
					if (control->m_multiplekey_prevent_count[i][j] < DEFAULT_PREVENT_DITHERING_TIMES) //save the first time key ID
					{
						control->m_multiplekey_prevent_key_id[i][j]= keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
					}
					else //the second time save key id
					{
						if (control->m_multiplekey_prevent_key_id[i][j] == keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j])//if two time key id is scale ,the key is valid
						{
							keypress++;	//key press found	
							
							us_delay(600);
							
							//key state down
							if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] == 0)
							{
								key.keyID = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
								key.longPress = 0;
								key.status = eKEYDOWN;
								keypad.callback_func(&key);					
							}

							//long press
							if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] == DEFAULT_UPDOWN_LONGKEY_DELAY)
							{
								key.keyID = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
								key.longPress = 1;
								key.status = eKEYPRESS;
								keypad.callback_func(&key);
							}
							
							//count down time
							if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] <= DEFAULT_UPDOWN_LONGKEY_DELAY)
							{
								keypad_parm->updown_matrix[i*keypad_parm->row_qty + j]++; 
							}
						}
						else
						{
							akprintf(C2, M_DRVSYS,"key is unvalid,K1:%d;K2:%d\n",control->m_multiplekey_prevent_key_id[i][j], keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j]);
						}
					}
				}
				else
				{					
					if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] > 0)
					{
						//normal key press event
						if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] < DEFAULT_UPDOWN_LONGKEY_DELAY)
						{
							key.keyID = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
							key.longPress = 0;
							key.status = eKEYPRESS;							
							keypad.callback_func(&key);
						}

						//key from down to up state
						key.keyID = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
                        key.longPress = 0;
						key.status = eKEYUP;					
						keypad.callback_func(&key);
						
						//reset this counter
						keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] = 0;
					}	
				}

				//recover rows as output high level
				for(k = 0; k < keypad_parm->column_qty; k++)
				{
					//gpio_set_pin_dir(keypad_parm->RowGpio[k], GPIO_DIR_OUTPUT);
					gpio_set_pin_level(keypad_parm->ColumnGpio[ k ], keypad_parm->active_level);
				}
					
					
			}
			else  //key [i][j] up
			{				
				if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] > 0)
				{					
					if(gpio_get_pin_level( keypad_parm->RowGpio[j] ) == (1 - keypad_parm->active_level) )
					{
						//normal key press event
						if(keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] < DEFAULT_UPDOWN_LONGKEY_DELAY)
						{
							key.keyID = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
							key.longPress = 0;
							key.status = eKEYPRESS;							
							keypad.callback_func(&key);
						}

						//key from down to up state
						key.keyID = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
						key.longPress = 0;
						key.status = eKEYUP;					
						keypad.callback_func(&key);
						
						//reset this counter
						keypad_parm->updown_matrix[i*keypad_parm->row_qty + j] = 0;
					}
				}
			}
			if (control->m_multiplekey_prevent_count[i][j]>=DEFAULT_PREVENT_DITHERING_TIMES)
			{
				control->m_multiplekey_prevent_count[i][j] = 0;
			}
		}	
	}

	if (control->m_prevent_dithering_count >= DEFAULT_PREVENT_DITHERING_TIMES)
	{
		control->m_prevent_dithering_count = 0;
		
		if(keypress == 0)  //no key in down state, enable int
		{		
			if (ERROR_TIMER != control->m_keydown_timer)
			{
				timer_stop(control->m_keydown_timer);
				control->m_keydown_timer = ERROR_TIMER;
			}
			
	        //recover rows as output high level
	        for(k = 0; k < keypad_parm->column_qty; k++)
	        {
	            //gpio_set_pin_dir(keypad_parm->RowGpio[k], GPIO_DIR_OUTPUT);
	            gpio_set_pin_level(keypad_parm->ColumnGpio[ k ], keypad_parm->active_level);
	        }

			for(j=0; j<keypad_parm->row_qty; j++)
			{
				gpio_int_control(keypad_parm->RowGpio[j], GPIO_INTERRUPT_ENABLE);
			}		
		}
	}
}

/**
 * @BRIEF keypad gpio callback function
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @PARAM[in] column: keypad column gpio pin ID.
 * @PARAM[in] polarity: 1 means active high interrupt. 0 means active low interrupt.
 * @RETURN T_VOID
 * @RETVAL
 */
static T_VOID keypad_gpio_callback(T_U32 column, T_U8 polarity)
{   
    T_U32 i;
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
    
    keypad_disable_intr();                                                //disable all keypad gpio interrupt
    
    if (keypad_parm->active_level == polarity)                            //keypad was pressed
    {       
        control->m_is_powerkey = AK_FALSE;

        for (i = 0; i < keypad_parm->row_qty; i++)                     //get keypad column index
        {
            if (keypad_parm->RowGpio[i] == column)
            {
                control->m_key_row_index = i;
                break;
            }
        }
        
        if(control->m_keydown_delay > 0)                                  //need prevent dithering delay
        {
            if (ERROR_TIMER != control->m_downkey_timer_id)
            {
                timer_stop(control->m_downkey_timer_id);
                control->m_downkey_timer_id = ERROR_TIMER;
            }
			if (ERROR_TIMER != control->m_prevent_dithering_timer_id)
	        {
	            timer_stop(control->m_prevent_dithering_timer_id);
	            control->m_prevent_dithering_timer_id = ERROR_TIMER;
	        }
			
			if (control->press_mode != eSINGLE_PRESS)
			{
	            control->m_downkey_timer_id = timer_start(KEYPAD_TIMER, control->m_keydown_delay, AK_FALSE, multiplekey_timer);  //start timer to read multiple key
				if (ERROR_TIMER == control->m_downkey_timer_id)
	            {
			        akprintf(C2, M_DRVSYS, "Start downkey timer error, use saftware delay!\r\n");
	                mini_delay(control->m_keydown_delay);                     //start timer failed, use software delay to prevent dithering
	                //downkey_timer(0, 0);
	            }
			}
			else
			{
				control->m_prevent_dithering_count = 0; //must be assure is 0
	            control->m_downkey_timer_id = timer_start(KEYPAD_TIMER, control->m_keydown_delay, AK_FALSE, downkey_timer);  //start timer to prevent dithering
	            control->m_prevent_dithering_timer_id = timer_start(KEYPAD_TIMER, control->m_keydown_delay+DEFAULT_PREVENT_DITHERING_DELAY, AK_FALSE, downkey_timer);  //start the second timer to prevent dithering

				if (ERROR_TIMER == control->m_downkey_timer_id || ERROR_TIMER == control->m_prevent_dithering_timer_id)
	            {
			        akprintf(C2, M_DRVSYS, "Start downkey timer error, use saftware delay!\r\n");
					if (ERROR_TIMER != control->m_downkey_timer_id)
			        {
			            timer_stop(control->m_downkey_timer_id);
			            control->m_downkey_timer_id = ERROR_TIMER;
			        }
					if (ERROR_TIMER != control->m_prevent_dithering_timer_id)
			        {
			            timer_stop(control->m_prevent_dithering_timer_id);
			            control->m_prevent_dithering_timer_id = ERROR_TIMER;
			        }
					
	                mini_delay(control->m_keydown_delay+DEFAULT_PREVENT_DITHERING_DELAY);                     //start timer failed, use software delay to prevent dithering
	                //downkey_timer(0, 0);
	            }
			}
        }
        else                                                              //not need prevent dithering delay
        {
            downkey_timer(0, 0);
			downkey_timer(0, 0);
        }
    }
    else                                                                  //keypad was released
    {
        if(control->m_keyup_delay > 0)                                    //need prevent dithering delay
        {
            if (ERROR_TIMER != control->m_upkey_timer_id)
            {
                timer_stop(control->m_upkey_timer_id);
                control->m_upkey_timer_id = ERROR_TIMER;
            }
            
            control->m_upkey_timer_id = timer_start(KEYPAD_TIMER, control->m_keyup_delay, AK_FALSE, upkey_timer);        //start timer to prevent dithering
            if (ERROR_TIMER == control->m_upkey_timer_id)
            {
		        akprintf(C2, M_DRVSYS, "Start upkey timer error, use saftware delay!\r\n");
                mini_delay(control->m_keyup_delay);                       //start timer failed, use software delay to prevent dithering
                upkey_timer(0, 0);
            }
        }
        else                                                              //not need prevent dithering delay
        {
            upkey_timer(0, 0);
        }
    }
}

/**
 * @BRIEF keypad powerkey callback function
 *        Because power-key occupied one gpio itself, the process is different from other keys
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @PARAM[in] column: power key gpio pin ID.
 * @PARAM[in] polarity: 1 means active high interrupt. 0 means active low interrupt.
 * @RETURN T_VOID
 * @RETVAL
 */
static T_VOID keypad_powerkey_callback(T_U32 pin, T_U8 polarity)
{
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
    
    keypad_disable_intr();

    if (keypad_parm->switch_key_active_level == polarity)                 //keypad was pressed
    {
        control->m_is_powerkey = AK_TRUE;
        
        if(control->m_keydown_delay > 0)                                  //need prevent dithering delay
        {
            if (ERROR_TIMER != control->m_downkey_timer_id)
            {
                timer_stop(control->m_downkey_timer_id);
                control->m_downkey_timer_id = ERROR_TIMER;
            }
            
            control->m_downkey_timer_id = timer_start(KEYPAD_TIMER, control->m_keydown_delay, AK_FALSE, powerkey_down_timer);  //start timer for powerkey down
            if (ERROR_TIMER == control->m_downkey_timer_id)
            {
		        akprintf(C2, M_DRVSYS, "Start downkey timer error, use saftware delay!\r\n");
                mini_delay(control->m_keydown_delay);                     //start timer failed, use software delay to prevent dithering
                powerkey_down_timer(0, 0);
            }
        }
        else                                                              //not need prevent dithering delay
        {
            powerkey_down_timer(0, 0);
        }
    }
    else                                                                  //keypad was released
    {
        if(control->m_keyup_delay > 0)                                    //need prevent dithering delay
        {
            if (ERROR_TIMER != control->m_upkey_timer_id)
            {
                timer_stop(control->m_upkey_timer_id);
                control->m_upkey_timer_id = ERROR_TIMER;
            }
            
            control->m_upkey_timer_id = timer_start(KEYPAD_TIMER, control->m_keyup_delay, AK_FALSE, upkey_timer);        //start timer to prevent dithering
            if (ERROR_TIMER == control->m_upkey_timer_id)
            {
		        akprintf(C2, M_DRVSYS, "Start upkey timer error, use saftware delay!\r\n");
                mini_delay(control->m_keyup_delay);                       //start timer failed, use software delay to prevent dithering
                upkey_timer(0, 0);
            }
        }
        else                                                              //not need prevent dithering delay
        {
            upkey_timer(0, 0);
        }
    }
}

/**
 * @brief downkey_timer
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID downkey_timer(T_TIMER timer_id, T_U32 delay)
{
    T_U32 i;
    T_KEYPAD key;   
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
	
    control->m_prevent_dithering_count++;//prevent dithering count

	if (control->m_prevent_dithering_count < DEFAULT_PREVENT_DITHERING_TIMES) //�Ⱥ�رյ�һ���ڶ���timer
	{
	    if (ERROR_TIMER != control->m_downkey_timer_id)
	    {
	        timer_stop(control->m_downkey_timer_id);
	        control->m_downkey_timer_id = ERROR_TIMER;
	    }
	}
	else
	{
	    if (ERROR_TIMER != control->m_prevent_dithering_timer_id)
	    {
	        timer_stop(control->m_prevent_dithering_timer_id);
	        control->m_prevent_dithering_timer_id = ERROR_TIMER;
	    }
	}
	
    if (gpio_get_pin_level(keypad_parm->RowGpio[control->m_key_row_index]) == keypad_parm->active_level)    //key press down
    {		
		for (i=0; i<keypad_parm->column_qty; i++)
		{
			gpio_set_pin_level(keypad_parm->ColumnGpio[i], (1 - keypad_parm->active_level));
		}

		us_delay(10);
		
        if (gpio_get_pin_level(keypad_parm->RowGpio[control->m_key_row_index]) == keypad_parm->active_level)
        {
			if (control->m_prevent_dithering_count < DEFAULT_PREVENT_DITHERING_TIMES)
			{
				control->m_prevent_dithering_key_id = keypad_parm->keypad_matrix[(keypad_parm->column_qty - 1)*keypad_parm->row_qty + control->m_key_row_index];
			}
			else  //the second time read key id ,if two times key id is equally,the key is valid
			{
				gpio_set_int_p(keypad_parm->RowGpio[control->m_key_row_index], (1 - keypad_parm->active_level)); //��ת�жϵ�ƽֵ
				gpio_int_control(keypad_parm->RowGpio[control->m_key_row_index], GPIO_INTERRUPT_ENABLE);
				control->m_prev_key_id = keypad_parm->keypad_matrix[(keypad_parm->column_qty - 1)*keypad_parm->row_qty + control->m_key_row_index];
				if (control->m_prevent_dithering_key_id == control->m_prev_key_id)
				{
					if (ERROR_TIMER != control->m_longkey_timer_id)
		            {
		                timer_stop(control->m_longkey_timer_id);
		                control->m_longkey_timer_id = ERROR_TIMER;
		            }
		            control->m_longkey_timer_id = timer_start(KEYPAD_TIMER, control->m_long_key_delay, AK_FALSE, longkey_timer); //start longkey timer
		            control->m_sent_key_num = 0;
		            if (control->m_prev_key_id != kbUnKnown)
		            {
		                key.keyID     = control->m_prev_key_id;
		                key.longPress = 0;
		                key.status    = eKEYDOWN;
		                keypad.callback_func(&key);
		            }          
				}
				else
				{
					akprintf(C2, M_DRVSYS,"key is unvalid,K1:%d;K2:%d\n",control->m_prevent_dithering_key_id, control->m_prev_key_id);

					control->m_prev_key_id=kbUnKnown;
				}
				control->m_prevent_dithering_count = 0;
			} 
        }
        else
        {
            //check to see which pin change the column state
            for (i=0; i<keypad_parm->column_qty; i++)
            {
			//	if(INVALID_GPIO == keypad_parm->RowGpio[i])
			//		continue;
				
                gpio_set_pin_level(keypad_parm->ColumnGpio[i], keypad_parm->active_level);
               	us_delay(10);
                if (gpio_get_pin_level(keypad_parm->RowGpio[control->m_key_row_index]) == keypad_parm->active_level)
                {
					if (control->m_prevent_dithering_count < DEFAULT_PREVENT_DITHERING_TIMES)
					{
						control->m_prevent_dithering_key_id = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + control->m_key_row_index];
					}
					else //the second time read key id ,if two times key id is equally,the key is valid
					{
						gpio_set_int_p(keypad_parm->RowGpio[control->m_key_row_index], (1 - keypad_parm->active_level));
	                    gpio_int_control(keypad_parm->RowGpio[control->m_key_row_index], GPIO_INTERRUPT_ENABLE);
						control->m_prev_key_id = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + control->m_key_row_index];

						if (control->m_prevent_dithering_key_id == control->m_prev_key_id)
	                   	{
							if (ERROR_TIMER != control->m_longkey_timer_id)
		                    {
		                        timer_stop(control->m_longkey_timer_id);
		                        control->m_longkey_timer_id = ERROR_TIMER;
		                    }
		                    control->m_longkey_timer_id = timer_start(KEYPAD_TIMER, control->m_long_key_delay, AK_FALSE, longkey_timer); //start longkey timer
		                    control->m_sent_key_num = 0;

		                    if (control->m_prev_key_id != kbUnKnown)
		                    {
		                        key.keyID     = control->m_prev_key_id;
		                        key.longPress = 0;
		                        key.status    = eKEYDOWN;
		                        keypad.callback_func(&key);
		                    }           
						}
						else
						{
							akprintf(C2, M_DRVSYS,"key is unvalid,K1:%d;K2:%d\n",control->m_prevent_dithering_key_id, control->m_prev_key_id);

							control->m_prev_key_id=kbUnKnown;
						}
						control->m_prevent_dithering_count = 0;
					}
					
                    break;
                }
                gpio_set_pin_level(keypad_parm->ColumnGpio[i], (1 - keypad_parm->active_level));
            }
        
            if(keypad_parm->column_qty == i)
            {
                keypad_enable_intr();
            	//akprintf(C2, M_DRVSYS, "It is keypad key down dithering!\r\n");
            }
        }
        
       	for (i=0; i<keypad_parm->column_qty; i++)
        {
            gpio_set_pin_level(keypad_parm->ColumnGpio[i], keypad_parm->active_level);
        }
        
        // [A1_block] move into "if(keypad_parm->row_qty == i)"
        // modified by panqihe 20090609. refer to tianxin.
        //for (i=0; i<keypad_parm->row_qty; i++)
        //{
        //    gpio_set_pin_level(keypad_parm->RowGpio[i], keypad_parm->active_level);
        //}
    }
    else                                                                //no key press down, it is dithering
    {
        keypad_enable_intr();
        //akprintf(C2, M_DRVSYS, "It is keypad key down dithering!\r\n");
    }
}

/**
 * @brief powerkey_down_timer
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID powerkey_down_timer(T_TIMER timer_id, T_U32 delay)
{
    T_KEYPAD key;   
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;

	if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");      
        return;
    }
	
    if (ERROR_TIMER != control->m_downkey_timer_id)
    {
        timer_stop(control->m_downkey_timer_id);
        control->m_downkey_timer_id = ERROR_TIMER;
    }

    if (control->m_is_powerkey)         //key of pressed is powerkey
    {       
        if (gpio_get_pin_level(keypad_parm->switch_key_id) == keypad_parm->switch_key_active_level)    //key press down
        {
            if(control->press_mode != eSINGLE_PRESS)
            {
                if (ERROR_TIMER != control->m_keydown_timer)
                {
                    timer_stop(control->m_keydown_timer);
                    control->m_keydown_timer = ERROR_TIMER;
                }
            }
                        
            gpio_set_int_p(keypad_parm->switch_key_id, (1 - keypad_parm->switch_key_active_level));
			gpio_int_control(keypad_parm->switch_key_id, GPIO_INTERRUPT_ENABLE);

            control->m_prev_key_id = keypad_parm->switch_key_value;
            
            if (ERROR_TIMER != control->m_longkey_timer_id)
            {
                timer_stop(control->m_longkey_timer_id);
                control->m_longkey_timer_id = ERROR_TIMER;
            }
            control->m_longkey_timer_id = timer_start(KEYPAD_TIMER, control->m_powerkey_long_delay, AK_FALSE, longkey_timer);  //start longkey timer
            control->m_sent_key_num = 0;

            key.keyID = control->m_prev_key_id;
            key.longPress = 0;
            key.status = eKEYDOWN;
            keypad.callback_func(&key);
        }
        else                                                                //no key press down, it is dithering
        {
            control->m_is_powerkey = AK_FALSE;
            keypad_enable_intr();
            //akprintf(C2, M_DRVSYS, "It is power key down dithering!\r\n");
        }
    }
}

/**
 * @brief multiplekey_timer
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID multiplekey_timer(T_TIMER timer_id, T_U32 delay)
{
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;
	T_U32 i,j;

	if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
	
    if (ERROR_TIMER != control->m_downkey_timer_id)
    {
        timer_stop(control->m_downkey_timer_id);
        control->m_downkey_timer_id = ERROR_TIMER;
    }
	
    if (gpio_get_pin_level(keypad_parm->RowGpio[control->m_key_row_index]) == keypad_parm->active_level)    //key press down
    {
        if (ERROR_TIMER != control->m_keydown_timer)
        {
            timer_stop(control->m_keydown_timer);
            control->m_keydown_timer = ERROR_TIMER;
        }
		control->m_prevent_dithering_count = 0;
		for(i=0; i<KEYPAD_MAX_ROW; i++)
		{
			for (j=0; j<KEYPAD_MAX_COLUMN; j++)
			{
				control->m_multiplekey_prevent_count[i][j]=0;
			}
		}
		
        control->m_keydown_timer = timer_start(KEYPAD_TIMER, DEFAULT_UPDOWN_DETECT_LOOP, AK_TRUE, keypad_updown_scan);   
        if(control->m_keydown_timer == ERROR_TIMER)
        {
	        akprintf(C2, M_DRVSYS, "no timer for keypad\r\n");
        }

		gpio_int_control(keypad_parm->switch_key_id, GPIO_INTERRUPT_ENABLE);
    }
    else                                                                //no key press down, it is dithering
    {
        keypad_enable_intr();
        //akprintf(C2, M_DRVSYS, "It is keypad key down dithering!\r\n");
    }
}

/**
 * @brief longkey_timer
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID longkey_timer(T_TIMER timer_id, T_U32 delay)
{
    T_KEYPAD key; 
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
    
    if (ERROR_TIMER != control->m_longkey_timer_id)
    {
        timer_stop(control->m_longkey_timer_id);
        control->m_longkey_timer_id = ERROR_TIMER;
    }
    
    key.keyID     = control->m_prev_key_id;
    key.longPress = 1;
    key.status    = eKEYPRESS;  

    control->m_sent_key_num = 1;
    keypad.callback_func(&key);  //call keypad callback function

    if (control->m_prev_key_id != keypad_parm->switch_key_value)
    {
        if (ERROR_TIMER != control->m_loopkey_timer_id)
        {
            timer_stop(control->m_loopkey_timer_id);
            control->m_loopkey_timer_id = ERROR_TIMER;
        }
        control->m_loopkey_timer_id = timer_start(KEYPAD_TIMER, control->m_loopkey_delay, AK_TRUE, loopkey_timer);
    }
}

/**
 * @brief loopkey_timer
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID loopkey_timer(T_TIMER timer_id, T_U32 delay)
{
    T_KEYPAD key;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }

    key.keyID = keypad.control.m_prev_key_id;
    key.longPress = 0;
    key.status = eKEYPRESS;

    keypad.callback_func(&key);
}

/**
 * @brief upkey_timer
 * Function keypad_init() must be called before call this function
 * @author guoshaofeng
 * @date 2007-12-18
 * @param T_TIMER timer_id: timer ID
 * @param T_U32 delay: timer delay
 * @return T_VOID
 * @retval
 */
static T_VOID upkey_timer(T_TIMER timer_id, T_U32 delay)
{
    T_KEYPAD key;  
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;
    T_KEYPAD_CONTROL *control = &keypad.control;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
    
    if (ERROR_TIMER != control->m_upkey_timer_id)
    {
        timer_stop(control->m_upkey_timer_id);
        control->m_upkey_timer_id = ERROR_TIMER;
    }

    if (control->m_is_powerkey)                                           //key of up is powerkey
    {
        if (gpio_get_pin_level(keypad_parm->switch_key_id) == keypad_parm->switch_key_active_level)   //key is not up, it is dithering
        {
			gpio_int_control(keypad_parm->switch_key_id, GPIO_INTERRUPT_ENABLE);
        }
        else                                                              //key is up
        {
            if (ERROR_TIMER != control->m_longkey_timer_id)
            {
                timer_stop(control->m_longkey_timer_id);
                control->m_longkey_timer_id = ERROR_TIMER;
            }

            if (ERROR_TIMER != control->m_loopkey_timer_id)
            {
                timer_stop(control->m_loopkey_timer_id);
                control->m_loopkey_timer_id = ERROR_TIMER;
            }

            if (!control->m_sent_key_num)
            {           
                key.keyID     = control->m_prev_key_id;
                key.longPress = 0;
                key.status    = eKEYPRESS;

                keypad.callback_func(&key);
            }

            key.keyID     = control->m_prev_key_id;
            key.longPress = 0;
            key.status    = eKEYUP;
            keypad.callback_func(&key);
            
            control->m_is_powerkey = AK_FALSE;
            gpio_set_int_p(keypad_parm->switch_key_id, keypad_parm->switch_key_active_level);
            keypad_enable_intr();                                         //enable all keypad gpio interrupt
        }
    }
    else                                                                  //key of up is other key
    {
        if (gpio_get_pin_level(keypad_parm->RowGpio[control->m_key_row_index]) == keypad_parm->active_level)            //key is not up, it is dithering
        {
            gpio_int_control(keypad_parm->RowGpio[control->m_key_row_index], GPIO_INTERRUPT_ENABLE);
            //akprintf(C2, M_DRVSYS, "It is keypad key up dithering!\r\n");
        }
        else                                                              //key is up
        {
            if (ERROR_TIMER != control->m_longkey_timer_id)
            {
                timer_stop(control->m_longkey_timer_id);
                control->m_longkey_timer_id = ERROR_TIMER;
            }

            if (ERROR_TIMER != control->m_loopkey_timer_id)
            {
                timer_stop(control->m_loopkey_timer_id);
                control->m_loopkey_timer_id = ERROR_TIMER;
            }
			
			//������ζ���key id��һ���Ļ�����û�з�down ��Ϣ������Ҳ���÷�Up��Ϣ
			if (control->m_prevent_dithering_key_id == control->m_prev_key_id)
			{
	            if (!control->m_sent_key_num)
	            {           
	                key.keyID     = control->m_prev_key_id;
	                key.longPress = 0;
	                key.status    = eKEYPRESS;

	                keypad.callback_func(&key);
	            }

	            key.keyID     = control->m_prev_key_id;
	            key.longPress = 0;
	            key.status    = eKEYUP;
	            keypad.callback_func(&key);
			}
            // added by panqihe 20090610 for bug[Swd200000228]
#if 0			
            for (i=0; i<keypad_parm->column_qty; i++)
            {
                gpio_set_pin_level(keypad_parm->CloumnGpio[i], keypad_parm->active_level);
            }
#endif
            gpio_set_int_p(keypad_parm->RowGpio[control->m_key_row_index], keypad_parm->active_level);
            keypad_enable_intr();                                         //enable all keypad gpio interrupt
            
            
        }
    }
}

/**
 * @BRIEF enable keypad interrupt
 * Function keypad_init() must be called before call this function
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @PARAM T_VOID
 * @RETURN T_VOID
 * @RETVAL
 */
static T_VOID keypad_enable_intr_matrix(T_VOID)
{
    T_U32 i;
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
    
    for (i=0; i<keypad_parm->row_qty; i++)
    {
        gpio_int_control(keypad_parm->RowGpio[i], GPIO_INTERRUPT_ENABLE);
    }
	gpio_int_control(keypad_parm->switch_key_id, GPIO_INTERRUPT_ENABLE);
}

/**
 * @BRIEF Disable keypad interrupt
 * Function keypad_init() must be called before call this function
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @PARAM T_VOID
 * @RETURN T_VOID
 * @RETVAL
 */
static T_VOID keypad_disable_intr_matrix(T_VOID)
{
    T_U32 i;
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }
    
    for (i=0; i<keypad_parm->row_qty; i++)
    {
        gpio_int_control(keypad_parm->RowGpio[i], GPIO_INTERRUPT_DISABLE);
    }
    gpio_int_control(keypad_parm->switch_key_id, GPIO_INTERRUPT_DISABLE);
}

/**
 * @BRIEF Set keypad delay parameter
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @PARAM T_S32 keylong_delay: long key delay time (millisecond),must >0
 * @PARAM T_S32 keydown_delay: long key delay time (millisecond),must >=0
 * @PARAM T_S32 keyup_delay: long key delay time (millisecond),must >=0
 * @PARAM T_S32 loopkey_delay: loop key delay time (millisecond),must >0
 * @RETURN T_VOID
 * @RETVAL
 */
static T_VOID keypad_set_delay_matrix(T_S32 keydown_delay, T_S32 keyup_delay, T_S32 keylong_delay, T_S32 powerkey_long_delay, T_S32 loopkey_delay)
{
    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return;
    }

    if (keydown_delay >= 0)
    {
        keypad.control.m_keydown_delay = (T_U32)keydown_delay;
    }

    if (keyup_delay >= 0)
    {
        keypad.control.m_keyup_delay     = (T_U32)keyup_delay;
    }

    if (keylong_delay > 0)
    {
        keypad.control.m_long_key_delay = (T_U32)keylong_delay;
    }

    if (loopkey_delay > 0)
    {
        keypad.control.m_loopkey_delay  = (T_U32)loopkey_delay;
    } 

    if (powerkey_long_delay > 0)
    {
        keypad.control.m_powerkey_long_delay = (T_U32)powerkey_long_delay;
    }  
}

/**
 * @BRIEF Scan keypad
 * Function keypad_init() must be called before call this function
 * @AUTHOR guoshaofeng
 * @DATE 2007-12-18
 * @PARAM T_VOID
 * @RETURN T_S32: The pressed key's scan code
 * @RETVAL
 */
static T_S32 keypad_scan_matrix(T_VOID)
{
    T_U32 i, j;
    T_S32 scancode = -1;
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return -1;
    }

    for (j=0; j<keypad_parm->row_qty; j++)
    {
        if (gpio_get_pin_level(keypad_parm->RowGpio[j]) == keypad_parm->active_level)
        {
            for (i=0; i<keypad_parm->column_qty; i++)
            {
                gpio_set_pin_level(keypad_parm->ColumnGpio[i], (1 - keypad_parm->active_level));
            }
        
            if (gpio_get_pin_level(keypad_parm->RowGpio[j]) == keypad_parm->active_level)
            {
                scancode = keypad_parm->keypad_matrix[(keypad_parm->column_qty - 1)*keypad_parm->row_qty + j];
            }
            else
            {
	            for (i=0; i<keypad_parm->column_qty; i++)
	            {
	                if(keypad_parm->ColumnGpio[i] != INVALID_GPIO)
	                {
		                gpio_set_pin_level(keypad_parm->ColumnGpio[i], keypad_parm->active_level);
		                us_delay(5);
		                if (gpio_get_pin_level(keypad_parm->RowGpio[j]) == keypad_parm->active_level)
		                {
		                    scancode = keypad_parm->keypad_matrix[i*keypad_parm->row_qty + j];
		                    break;
		                }
		                gpio_set_pin_level(keypad_parm->ColumnGpio[i], (1 - keypad_parm->active_level));
	                }
	            }	
            }
            for( i=0; i<keypad_parm->column_qty; i++ )
            {
                gpio_set_pin_level( keypad_parm->ColumnGpio[ i ], keypad_parm->active_level);
            }
        }
    }
    return scancode;
}

/**
 * @BRIEF get the keyvalue
 * @AUTHOR wanliming
 * @DATE 2007-12-18
 * @PARAM T_U32 keyrow: keyapd row
 * @PARAM T_U32 keycol: keyapd column
 * @RETURN T_S32 
 * @RETVAL
 *//*
static T_S32 keypad_get_value_matrix(T_U32 keyrow, T_U32 keycol)
{                
    T_PLATFORM_KEYPAD_PARM *keypad_parm = &keypad.keypad_parm;

    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");        
        return -1;
    }
    
    if ((keyrow < keypad_parm->row_qty) && (keycol< keypad_parm->column_qty))
    {      
        return keypad_parm->keypad_matrix[keyrow*keypad_parm->column_qty + keycol]; 
    }                 
    else 
    {
        return  -1;
    }           
}*/

static T_BOOL keypad_set_pressmode_matrix(T_eKEY_PRESSMODE press_mode)
{
    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return AK_FALSE;
    }

    keypad.control.press_mode = press_mode;

    return AK_TRUE;
}

static T_eKEY_PRESSMODE keypad_get_pressmode_matrix(T_VOID)
{
    if (AK_FALSE == keypad.control.key_init)
    {
        akprintf(C2, M_DRVSYS, "keypad is not init \r\n");
        return eSINGLE_PRESS;
    }

    return keypad.control.press_mode;
}



//******************************reg my handle info***************************//
static T_KEYPAD_HANDLE matrix_handler = 
{
	kpd_init,
	keypad_scan_matrix,
	keypad_enable_intr_matrix, 
	keypad_disable_intr_matrix, 
	keypad_get_pressmode_matrix,
	keypad_set_pressmode_matrix,
	keypad_set_delay_matrix
};

static int matrix_reg(void)
{  
	keypad_reg_scanmode(KEYPAD_MATRIX_NORMAL,&matrix_handler);
	return 0;
}

#ifdef __CC_ARM
#pragma arm section rwdata = "__initcall_", zidata = "__initcall_"
#endif
module_init(matrix_reg)
#ifdef __CC_ARM
#pragma arm section
#endif

#endif

