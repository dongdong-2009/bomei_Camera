/**
 * @FILENAME: keypad_CI3771.c
 * @BRIEF keypad CI7801 driver file
 * Copyright (C) 2008 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @AUTHOR tangjianlong
 * @DATE 2008-01-14
 */

#ifdef CI37XX_PLATFORM

#include "akdefine.h"
#include "gbl_global.h"
#include "drv_api.h"
#include "gpio.h"
#include "keypad_type.h"
#include "Gpio_user_define.h"

#define KEYPAD_MAX_ROW           2    //���������
#define KEYPAD_MAX_COLUMN        3    //���������

 //use CI8802 configuration
 
const T_U8 m_ucColumnGpio[KEYPAD_MAX_COLUMN]	= {GPIO_KEYAPD_COLUMN0, GPIO_KEYAPD_COLUMN1, GPIO_KEYAPD_COLUMN2};
const T_U8 m_ucRowGpio[KEYPAD_MAX_ROW]			= {GPIO_KEYAPD_ROW0, GPIO_KEYAPD_ROW1};

const T_U32 m_keypad_matrix[KEYPAD_MAX_ROW][KEYPAD_MAX_COLUMN] = 
{
   kbOK,     kbLEFT,      kbUP,
   kbDOWN,   kbCLEAR,   kbRIGHT,
};

T_S8 keypad_updown_matrix[KEYPAD_MAX_ROW][KEYPAD_MAX_COLUMN] = 
{
    0, 0, 0,
    0, 0, 0,
};      // ==0 means key-up, >0 count key-down time(timer is 20ms)


static const T_PLATFORM_KEYPAD_PARM platform_keypad = {
    KEYPAD_MAX_ROW,         /* row gpio ���� */
    KEYPAD_MAX_COLUMN,      /* column gpio ���� */
    m_ucRowGpio,            /* row gpio������׵�ַ */
    m_ucColumnGpio,         /* Column gpio������׵�ַ */
    &m_keypad_matrix[0][0], /* ���������߼�������׵�ַ */
    &keypad_updown_matrix[0][0],    /* updown�߼�������׵�ַ */
    
    GPIO_LEVEL_HIGH,        /* ������Ч��ƽֵ, 1��0 */

	GPIO_SWITCH_KEY,		/* ��Դ����gpio��ֵ */	
    kbCLEAR,                /* ��Դ���ļ�ֵ */
    GPIO_LEVEL_HIGH,        /* ��Դ������Ч��ƽֵ��1��0 */
};

/**
 * @ȡƽ̨�������ò������˽ӿڻᱻkeypad_init�ӿڵ��ã�ÿ��ƽ̨�ֱ�ʵ��
 *
 * Function it will be call by keypad_init
 * @author Miaobaoli
 * @date 2004-09-21
 * @param[in] T_VOID
 * @return T_PLATFORM_KEYPAD_PARM *
 * @retval the pointer of platform keypad information 
 */
const T_PLATFORM_KEYPAD_PARM *keypad_get_platform_parm(T_VOID)
{
    return &platform_keypad;
}

const T_KEYPAD_TYPE keypad_get_platform_type(T_VOID)
{
    return KEYPAD_MATRIX_NORMAL;
}

#endif  //#ifdef CI7802_PLATFORM
