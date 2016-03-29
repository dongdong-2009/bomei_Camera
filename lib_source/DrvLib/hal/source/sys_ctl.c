/**
 * @FILENAME: sys_ctl.c
 * @BRIEF sys_ctl driver file
 * Copyright (C) 2007 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @AUTHOR 
 * @DATE
 * @VERSION 1.0
 * @REF
 */

#include "interrupt.h" 
#include "drv_api.h"
#include "gpio.h"
#include "arch_pmu.h"
#include "sysctl.h"
#include "arch_sysctl.h"



/*******************************************************************************
 * @brief   Soft_Reset
 * @author  kejianping
 * @date    2014-09-4
 * @param   T_VOID
 * @return  T_VOID
*******************************************************************************/

T_VOID soft_reset(T_VOID)
{

    void (*F)(void);
       
    set_pll_value(280 >> 1);//xuyr ddr set_pll_value(124);
    store_all_int();
    gpio_int_disableall();
    vtimer_free();//timer_reset_reg	
	pmu_set_ldo12(LDO12_120V);//reset ������ldoģʽ�½���

	REG32(RESET_CTRL_REG) = 0xFFFFF9FF; //��λ����RAM��ROM������Ƭ�����������
	REG32(RESET_CTRL_REG) = 0;	
	REG32(INT_SYS_MODULE_REG) = 0;//�رն����жϡ�

    MMU_DisableDCache();
    MMU_DisableICache();
    MMU_InvalidateICache();
    MMU_DisableMMU();
    MMU_InvalidateTLB();


    (void*)F = (void*)0; 
    F();
    __asm
    {
        nop
    }
    
}


