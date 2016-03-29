
/**@file fm_probe.h
 * @brief fm moudle, fm probe
 *
 * Copyright (C) 2007 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @date 2008.4.17
 * @version 1.0
 */

#ifndef __KEYPAD_TYPE_H__
#define __KEYPAD_TYPE_H__


typedef enum {
    KEYPAD_MATRIX_NORMAL = 0,   // normal matrix keypad
    KEYPAD_MATRIX_DIODE,        // diode matrix keypad
    KEYPAD_MATRIX_MIXED,        // mixed matrix keypad
    KEYPAD_KEY_PER_GPIO,         //one gpio = one key keypad
    KEYPAD_TYPE_NUM
} T_KEYPAD_TYPE;



/* �˽ṹ�岻�����޸�, �˽ṹ���������ɿͻ�ʵ�֣���ͨ���ӿ�keypad_get_platform_parm���ظ����� */
/* �����ڵ���keypad_initʱ�����keypad_get_platform_parm�ӿڣ��õ��û�ƽ̨�ļ�����Ϣ */
typedef struct _PLATFORM_KEYPAD_PARM_{
    T_U32		row_qty;        			///< row gpio ���� 
    T_U32		column_qty;     			///< column gpio ���� 
    T_U8 		*RowGpio;       			///< ָ��row gpio �����ָ�� 
    T_U8 		*ColumnGpio;    			///< ָ��Column gpio �����ָ�� 
    T_U32		*keypad_matrix; 			///< ָ����������߼������ָ�� 
    T_S8 		*updown_matrix; 			///< ָ��updown�߼������ָ�� 
    T_U32		active_level;   			///< ������Ч��ƽֵ, 1��0 
    T_U32		switch_key_id;  			///< ��Դ����gpio idֵ 
    T_U32       switch_key_value;			///< ��Դ���ļ�ֵ 
    T_U32		switch_key_active_level; 	///< ��Դ������Ч��ƽֵ��1��0 
} T_PLATFORM_KEYPAD_PARM;



/**
 * @brief �û�ʵ�ֵĽӿڣ�ȡƽ̨�ļ��̵�������Ϣ���˽ӿڻᱻkeypad_init����
 *
 * @author Miaobaoli
 * @date 2004-09-21
 * @return T_PLATFORM_KEYPAD_PARM *��ƽ̨�ļ��̵�������Ϣ�Ľṹ��ָ��
 * @retval the pointer of platform keypad information 
 */
const T_PLATFORM_KEYPAD_PARM *keypad_get_platform_parm(T_VOID);

/**
 * @ȡƽ̨����ɨ������
 * @author Dengjian
 * @date 2008-05-21
 * @param[in] T_VOID
 * @return T_KEYPAD_TYPE
 */
const T_KEYPAD_TYPE keypad_get_platform_type(T_VOID);
//**********************************************************************************************




#endif

