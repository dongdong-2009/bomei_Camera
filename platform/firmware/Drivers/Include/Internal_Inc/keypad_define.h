#ifndef __KEYPAD_DEFINE_H__
#define __KEYPAD_DEFINE_H__

#include "anyka_types.h"
#include "platform_hd_config.h"

//GPIO KETPAD PARM
typedef struct{
    T_U32    row_qty;                ///< row gpio ���� 
    T_U32    column_qty;             ///< column gpio ���� 
    T_U8     *RowGpio;               ///< ָ��row gpio �����ָ�� 
    T_U8     *ColumnGpio;            ///< ָ��Column gpio �����ָ�� 
    T_U32    *keypad_matrix;         ///< ָ����������߼������ָ�� 
    T_S8     *updown_matrix;         ///< ָ��updown�߼������ָ�� 
    T_U32    active_level;           ///< ������Ч��ƽֵ, 1��0 
    T_U32    switch_key_id;          ///< ��Դ����gpio idֵ 
    T_U32    switch_key_value;        ///< ��Դ���ļ�ֵ 
    T_U32    switch_key_active_level; ///< ��Դ������Ч��ƽֵ��1��0 
} T_PLATFORM_KEYPAD_GPIO;

typedef  T_U16 T_AD_VAL;
typedef  T_U16 T_ID_VAL;

typedef struct 
{
    T_U16  Min;    //�����仯����Сֵ
    T_U16  Max;    //�����仯�����ֵ
    T_U16  KeyID;  //����ID
}KEY_DETECT_STR;

//ANALOG KETPAD PARM
typedef struct{
    KEY_DETECT_STR *key_avl_array;   //ָ��AD������ӳ���
    T_U32       key_max_num;      //����������
    T_U32       ad_avl_offset;    //һ������������ƫ����
    T_U32       AdValMin;         //��Ч��������Сֵ
    T_U32       AdValMax;         //��Ч���������ֵ
} T_PLATFORM_KEYPAD_ANALOG;

typedef union{
    T_PLATFORM_KEYPAD_GPIO      PARM_GPIO;
    T_PLATFORM_KEYPAD_ANALOG    PARM_ANALOG;
}T_PLATFORM_KEYPAD_PARM;

#endif //end of __KEYPAD_DEFINE_H__
