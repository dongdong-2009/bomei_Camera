/** @file anyka_bsp.h
 * @brief BSP(board support packet) file
 *
 * User must define the physical info of the board here. 
 * such as  FLASH/RAM/STACK etc
 * Copyright (C) 2006 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @author 
 * @date 2006-01-16
 * @version 1.0
 */
#ifndef _ANYKA_BSP_H_
#define _ANYKA_BSP_H_

/* Memory start address */
#define RAM_BASE_ADDR		0x30000000	// RAM start address


/** @{@name System Memory Size
 *	Define the size of memory device of the system.
 *	include SRAM/SDRAM/ which used by system.
 */
#ifdef SDRAM_MODE
#define  SDRAM_SIZE  (SDRAM_MODE << 20)
#if ((SDRAM_MODE > 256) && (defined (CHIP_AK9802)))
#error "CHIP_AK9802 Not support RAM larger than 256M bytes"
#endif

#if ((SDRAM_MODE > 512) && (defined (CHIP_AK9801) || defined (CHIP_AK9805)))
#error "CHIP_AK9805 or CHIP_AK9801 Not support RAM larger than 512M bytes"
#endif

#else
#error "No define SDRAM_MODE"
#endif


#define RAM_SIZE    SDRAM_SIZE

/** @} */

/* Memory distribution for MMU, TimerHISR, and each stacks 
 * --- Top of memory address----
 * |	   		mmu
 * |----------------------------
 * |	      TimerHISR(Optional)
 * |----------------------------
 * |	  Undefined mode stack
 * |----------------------------
 * |	   Abort mode stack
 * |----------------------------
 * |	    FIQ mode stack
 * |----------------------------
 * |	    IRQ mode stack
 * |----------------------------
 * |	    SVC mode stack
 * |----------------------------
 * |	   	    Heap
 * |----------------------------
 */

/* define the MMU size and start address */
#define _MMUTT_SIZE				(0x4000)
#define _MMUTT_STARTADDRESS     (RAM_BASE_ADDR + RAM_SIZE - _MMUTT_SIZE)

/* define the TimerHISR(Optional) size and start address */
#ifdef AKOS
#define TimerHISR               (0xC800)
#define Timer_Priority 		    2
#define TMC_STACK_BASE          (_MMUTT_STARTADDRESS-TimerHISR)
#else
#define TMC_STACK_BASE          (_MMUTT_STARTADDRESS)
#endif

/* each stack size and start address */
#define IRQ_MODE_STACK_SIZE     (0x10000)
#define FIQ_MODE_STACK_SIZE     (0x10)  /* if has FIQ, enlarge it */
#define ABORT_MODE_STACK_SIZE   (0x1000)
#define UNDEF_MODE_STACK_SIZE  	(0x1000)

#define SVC_MODE_STACK			(IRQ_MODE_STACK - IRQ_MODE_STACK_SIZE)	 
#define IRQ_MODE_STACK			(FIQ_MODE_STACK - FIQ_MODE_STACK_SIZE)
#define FIQ_MODE_STACK			(ABORT_MODE_STACK - ABORT_MODE_STACK_SIZE)
#define ABORT_MODE_STACK		(UNDEF_MODE_STACK - UNDEF_MODE_STACK_SIZE)	
#define UNDEF_MODE_STACK		(TMC_STACK_BASE-8)
/** @} */

/*@}*/


#endif	// #ifndef _ANYKA_BSP_H_


