/**
 * @FILENAME: keypad_CI7801.c
 * @BRIEF keypad CI7801 driver file
 * Copyright (C) 2008 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @AUTHOR tangjianlong
 * @DATE 2008-01-14
 */
#ifdef OS_ANYKA
#ifdef CI37XX_PLATFORM
#include "akdefine.h"
#include "keypad_define.h"
#include "gpio_config.h"
#include "gbl_macrodef.h"
#include "drv_gpio.h"

#if (KEYPAD_TYPE == 0)

#define KEYPAD_MAX_ROW_3           3    //���������
#define KEYPAD_MAX_COLUMN_4        4    //���������
#define KEYPAD_MAX_ROW_2           2    //���������
#define KEYPAD_MAX_COLUMN_3        3    //���������

typedef enum
{
    eTYPE_NUM_2x3 = 0,
    eTYPE_NUM_3x4,       
} T_eKEY_NUMBER_TYPE;

static T_eKEY_NUMBER_TYPE m_type_num = eTYPE_NUM_2x3;
 
const T_U8 m_ucColumnGpio[KEYPAD_MAX_COLUMN_4]    = {GPIO_KEYAPD_COLUMN0, GPIO_KEYAPD_COLUMN1, GPIO_KEYAPD_COLUMN2, GPIO_KEYAPD_COLUMN3};
const T_U8 m_ucRowGpio[KEYPAD_MAX_ROW_3]            = {GPIO_KEYAPD_ROW0, GPIO_KEYAPD_ROW1, GPIO_KEYAPD_ROW2};

const T_U32 m_keypad_matrix_3x4[KEYPAD_MAX_ROW_3][KEYPAD_MAX_COLUMN_4] = 
{
   kbMENU,        kbLEFT,       kbSWX,           kbUP,
   kbDOWN,        kbSWY,        kbCLEAR,         kbRIGHT,
   kbSWZ,         kbSWA,        kbSWB,           kbSWC,
};

T_S8 keypad_updown_matrix_3x4[KEYPAD_MAX_ROW_3][KEYPAD_MAX_COLUMN_4] = 
{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

const T_U32 m_keypad_matrix_2x3[KEYPAD_MAX_ROW_2][KEYPAD_MAX_COLUMN_3] = 
{
   kbMENU,     kbLEFT,        kbUP,      
   kbDOWN,     kbCLEAR,       kbRIGHT,
};

T_S8 keypad_updown_matrix_2x3[KEYPAD_MAX_ROW_2][KEYPAD_MAX_COLUMN_3] = 
{
    0, 0, 0,
    0, 0, 0,
};

static const T_PLATFORM_KEYPAD_GPIO platform_keypad_2x3 = {
    KEYPAD_MAX_ROW_2,       /* row gpio ���� */
    KEYPAD_MAX_COLUMN_3,    /* column gpio ���� */
    m_ucRowGpio,            /* row gpio������׵�ַ */
    m_ucColumnGpio,         /* Column gpio������׵�ַ */
    &m_keypad_matrix_2x3[0][0], /* ���������߼�������׵�ַ */
    &keypad_updown_matrix_2x3[0][0],    /* updown�߼�������׵�ַ */
    
    GPIO_LEVEL_HIGH,        /* ������Ч��ƽֵ, 1��0 */

    GPIO_SWITCH_KEY,        /* ��Դ����gpio��ֵ */    
    kbCLEAR,                /* ��Դ���ļ�ֵ */
    GPIO_LEVEL_HIGH,        /* ��Դ������Ч��ƽֵ��1��0 */
};

static const T_PLATFORM_KEYPAD_GPIO platform_keypad_3x4 = {
    KEYPAD_MAX_ROW_3,         /* row gpio ���� */
    KEYPAD_MAX_COLUMN_4,      /* column gpio ���� */
    m_ucRowGpio,            /* row gpio������׵�ַ */
    m_ucColumnGpio,         /* Column gpio������׵�ַ */
    &m_keypad_matrix_3x4[0][0], /* ���������߼�������׵�ַ */
    &keypad_updown_matrix_3x4[0][0],    /* updown�߼�������׵�ַ */
    
    GPIO_LEVEL_HIGH,        /* ������Ч��ƽֵ, 1��0 */

    GPIO_SWITCH_KEY,        /* ��Դ����gpio��ֵ */    
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
const T_VOID *keypad_get_platform_parm(T_VOID)
{
    if (eTYPE_NUM_3x4 == m_type_num)
    {
        return (T_VOID *)&platform_keypad_3x4;
    }
    else
    {
        return (T_VOID *)&platform_keypad_2x3;
    }
}

T_KEYPAD_TYPE keypad_get_platform_type(T_VOID)
{
    return KEYPAD_MATRIX_NORMAL;
}

T_BOOL keypad_set_number_type_3X4(T_VOID)
{
    T_BOOL ret = AK_FALSE;

    if (m_type_num != eTYPE_NUM_3x4)
    {
        m_type_num = eTYPE_NUM_3x4;
        ret = AK_TRUE;
    }

    return ret;
}

T_BOOL keypad_set_number_type_2X3(T_VOID)
{
    T_BOOL ret = AK_FALSE;

    if (m_type_num != eTYPE_NUM_2x3)
    {
        m_type_num = eTYPE_NUM_2x3;
        ret = AK_TRUE;
    }

    return ret;
}

#endif
#endif  //#ifdef CI7802_PLATFORM
#endif

