/************************************************************************
 * Copyright (c) 2001, Anyka Co., Ltd. 
 * All rights reserved.	
 *  
 * File Name��Fwl_osMalloc.h
 * Function��This header file is API for Memory Library
 *
 * Author��ZMJ
 * Date��
 * Version��
 *
 * Reversion: 
 * Author: 
 * Date: 
**************************************************************************/
#ifndef __FWL_OS_MALLOC_H__
#define __FWL_OS_MALLOC_H__

#include "anyka_types.h"


T_VOID Fwl_MallocInit(T_VOID);
T_VOID Fwl_MallocSupportMultithread(T_VOID);

T_pVOID Fwl_MallocAndTrace(T_U32 size, T_pSTR filename, T_U32 line);
T_pVOID	Fwl_ReMallocAndTrace(T_pVOID var, T_U32 size, T_pSTR filename, T_U32 line);
T_pVOID Fwl_FreeAndTrace(T_pVOID var, T_pSTR filename, T_U32 line); 

#define Fwl_Malloc(size)		Fwl_MallocAndTrace((size), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))
#define Fwl_ReMalloc(var, size) Fwl_ReMallocAndTrace((var), (size), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))
#define Fwl_Free(var)			Fwl_FreeAndTrace((var), ((T_S8*)(__FILE__)), ((T_U32)__LINE__)) 

#define Fwl_FreeTrace(var)	Fwl_FreeAndTrace((var), ((T_S8*)__FILE__), ((T_U32)__LINE__))

#define AK_MALLOCRECORD(type)  (type *)Fwl_Malloc(sizeof(type))



/* debug function */ //should merge to one
T_U32 Fwl_GetTotalRamSize(T_VOID);
T_U32 Fwl_RamUsedBlock(T_VOID);
T_U32 Fwl_GetUsedRamSize(T_VOID);
T_U32 Fwl_RamGetBlockNum(T_VOID);
T_U32 Fwl_RamGetBlockLen(T_VOID);
T_U32 Fwl_GetRemainRamSize(T_VOID);
T_U32 Fwl_GetLargestSize_Allocable(T_VOID);

T_BOOL Fwl_CheckPtr(T_pVOID var);
T_U32  Fwl_ReleaseMemory(T_VOID);



/* NOTE:
   every body, every library, we'll should do like below:

	#ifdef 	DEBUG_TRACE_MEMORY_LEAK
	#define Fwl_Malloc(size)		Fwl_MallocAndTrace((size), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))
	#define Fwl_ReMalloc(var, size) Fwl_ReMallocAndTrace((var), (size), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))
	#define Fwl_Free(var)			Fwl_FreeAndTrace((var), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))		
	#else
	#define Fwl_Malloc(size)		Fwl_MallocAndTrace((size), AK_NULL, 0)
	#define Fwl_ReMalloc(var, size) Fwl_ReMallocAndTrace((var), (size), AK_NULL, 0)
	#define Fwl_Free(var)			Fwl_FreeAndTrace((var), AK_NULL, 0)
	#endif   

#endif
*/

/* NOTE:
   below is define in Fwl_osMalloc.c  //for shorten complier time


#ifndef ENABLE_MEMORY_DEBUG
	//#define ENABLE_MEMORY_DEBUG	//�ڴ�Խ���Զ������
#endif
*/
T_VOID	Fwl_RamBeyondMonitorGetbyTimer(T_U32 LID);	//����ڴ�Խ��״��
T_VOID  Fwl_RamWilderMonitorGetbyTimer(T_U32 LLD);  //����ڴ�Ұָ��״��
T_VOID Fwl_RamBeyondMonitor(T_U32 LLD);
T_VOID  Fwl_RamWilderMonitor(T_U32 LLD);

T_VOID	Fwl_RamLeakMonitorHooktoSM(T_VOID);		//ʹ��״̬�������������ڴ�й©
T_VOID	Fwl_RamLeakMonitorPointBeg(T_VOID);		//�򿪼��������µĴ������ڴ�й©
T_VOID	Fwl_RamLeakMonitorPointEnd(T_VOID);     //�رռ��������ϵĴ������ڴ�й©
T_VOID  Fwl_RamEnumerateEachSeg(T_VOID);		//ö�����е��ڴ��


#endif
