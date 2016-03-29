/**
 * @FILENAME: freq.c
 * @BRIEF freq driver file
 * Copyright (C) 2007 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @AUTHOR liao_zhijun
 * @DATE 2010-05-24
 * @VERSION 1.0
 * @REF
 */
#include "anyka_cpu.h"
#include "anyka_types.h"
#include "l2.h"
#include "freq.h"

/*
    ASPEN CPU clock test:
    ��ִ����������ָ��Ϊ��(��ִ��100�״Σ�T ��ʾִ��ʱ��):
          ADD      r0,r0,#1
        CMP      r0,r1
        BCC      0x300064d0
    ��������ָ�������������ݾ�����ڼĴ����У������ִ��
    ʱ��ֻ���漰�����ڴ�ȡָ�Ĳ��������·�������������?
1.�ڴ�ICACHE ��DCAHE�������
    ��ʹ�����ݺ�ָ��CACHEʱ��CPU��������ָ��Ĳ���ֻ���漰
    ���Ĵ����ͻ���Ĳ������ٶ����
        CPU clock=60MHZʱ��T=8.4s
        CPU clock=100MHZʱ��T=5.0s    
    CPU �ٶȾ���Ƶ���ʱ��Ϊ:        
        CPU clock=60MHZʱ��T=4.2s
        CPU clock=100MHZʱ��T=2.5s

2.�ڹر�ICACHE ��DCACHE�������
    ��ʱCPU��Ҫ���ڴ���ȡָ���⽫�����Ľ���CPU��ִ���ٶ�
        CPU clock=60MHZ�� T = 45.3s
    ���⣬�ڹر�ICACHE����DCACHE ������£�
        CPU clock=60MHZ�� T = 44s

    �����������ݿ��Կ�����CPU��ִ������ָ��ʱ�Ĵ󲿷�ʱ��
    ��������ȡָ��

3.����
    ��Сϵͳ�У��ر�LCD������²ⶨ
    CPU clock=60MHZ�� I = 33mA
    CPU clock=200MHZ�� I = 45mA

4. ���㷽��
    �����ض���ָ�����У���ִ�����ĵ�ʱ�估ռ�õ�CPU clock��
    ���㷽��Ϊ:

    cnt:ִ��ָ��Ĵ���
    n:   ָ������
    clk: CPUʱ������
    ÿ��ARMָ��ռ������CPUʱ������

    ָ���ִ��ʱ��T = n*cnt*2/clk

    ÿ��ָ��ռ�õ�CPUʱ������:
        t = T*clk/(n * cnt)    
*/

/* define max freqency divider */

#define PLL_CLK_MIN            225

/**
 * @brief    get current asic frequency.
 *
 * @author    liaozhijun
 * @date     2010-04-06
 * @return    T_U32 the frequency of asic running
 */
T_U32 get_asic_freq(T_VOID)
{
    T_U32 ratio;
    T_U32 pll_sel;
    T_U32 asic_freq;
    T_U32 clk168_freq;
    T_U8 pre_div = 0, asic_div = 0;
    T_U8 clk168_div = 0;


    ratio = REG32(CLOCK_DIV_REG);
    clk168_div = (ratio >> 17) & 0xF;

    ratio = REG32(CLOCK_DIV_REG);
    pll_sel = (ratio & 0x3f);

    clk168_freq = ((pll_sel*5 + PLL_CLK_MIN) / (clk168_div+1))*1000000L;


    //just return clk168 if bit[31] is set
    if(REG32(CLOCK3X_CTRL_REG) & (1<<31))
    {
        return clk168_freq;
    }

    //get ASIC_DIV
    ratio = REG32(CLOCK_DIV_REG);

    //get asic div
    asic_div = (ratio >> 6) & 0x07;
    
    if(asic_div == 0) asic_div = 1;

    //get pre div
    if(ratio & (1<<21)) pre_div = (ratio >> 22) & 0xF;
    
    asic_freq = (clk168_freq >> asic_div) / (pre_div + 1);

    return asic_freq;
}

