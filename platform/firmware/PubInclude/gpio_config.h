/**
 * @file gpio_config.h
 * @brief gpio function header file
 *
 * @FILENAME: gpio_config.h
 * @BRIEF config gpio
 * Copyright (C) 2008 Anyka (GuangZhou) Microelectronics Technology Co., Ltd.
 * @AUTHOR 
 * @DATE 2011-04-12
 * @VERSION 1.0
 * @REF
 * @NOTE:
 * 1. ����mmiϵͳ���Ѷ����˵�gpio������Ҫɾ����ش��룬ֻ�轫�䶨��ΪINVALID_GPIO
 
 * 2. �����Ҫ�õ���չio��ֻ��Ҫ��GPIO_MULTIPLE_USE�꣬�����ö�Ӧ��gpio
 *    GPIO_EXPAND_OUT1��GPIO_EXPAND_OUT2�����ֻ��һ����չio,���Խ�GPIO_EXPAND_OUT2
 *	  ��ΪINVALID_GPIO����
 */
#ifndef __GPIO_CONFIG_H__
#define __GPIO_CONFIG_H__

#ifdef OS_ANYKA

#ifndef INVALID_GPIO
#define INVALID_GPIO                    0xfe
#endif

#if (defined (CHIP_AK3771))
#include "gpio_config_CI3771.h"
#elif (defined (CHIP_AK3753))
#include "gpio_config_CI3753.h"
#elif (defined (CHIP_AK3750))
#include "gpio_config_CI3750.h"
#elif (defined (CHIP_AK3760))
#include "gpio_config_CI3760.h"
#else
#error "MUST Include GPIO Config File!!!"
#endif

#endif  // OS_ANYKA

#endif //#ifndef __GPIO_CONFIG_H__

