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

T_pVOID Fwl_MallocAndTrace(T_U32 size, T_pSTR filename, T_U32 line);
T_pVOID	Fwl_ReMallocAndTrace(T_pVOID var, T_U32 size, T_pSTR filename, T_U32 line);
T_pVOID Fwl_FreeAndTrace(T_pVOID var, T_pSTR filename, T_U32 line); 

T_pVOID Fwl_Malloc(T_U32 size);
T_pVOID Fwl_ReMalloc(T_pVOID var, T_U32 size);
T_pVOID Fwl_Free(T_pVOID var);

T_BOOL  Fwl_CheckPtr(T_pVOID var);


/* debug function */ //should merge to one
T_U32 Fwl_GetTotalRamSize(T_VOID);
T_U32 Fwl_RamUsedBlock(T_VOID);
T_U32 Fwl_GetUsedRamSize(T_VOID);
T_U32 Fwl_RamGetBlockNum(T_VOID);
T_U32 Fwl_RamGetBlockLen(T_VOID);




/* below used to local memory share */
T_pVOID QQ_CreateAllocator(T_pVOID mempool, T_U32 size);
T_pVOID QQ_DestroyAllocator(T_pVOID hAllocator);

T_pVOID QQ_MallocAndTrace(T_pVOID hAllocator, T_U32 size, T_pSTR filename, T_U32 line);
T_pVOID	QQ_ReMallocAndTrace(T_pVOID hAllocator, T_pVOID var, T_U32 size, T_pSTR filename, T_U32 line);
T_pVOID QQ_FreeAndTrace(T_pVOID hAllocator, T_pVOID var, T_pSTR filename, T_U32 line); 

T_pVOID QQ_Malloc(T_pVOID hAllocator, T_U32 size);
T_pVOID QQ_ReMalloc(T_pVOID hAllocator, T_pVOID var, T_U32 size);
T_pVOID QQ_Free(T_pVOID hAllocator, T_pVOID var);

T_BOOL  QQ_CheckPtr(T_pVOID hAllocator, T_pVOID var);

T_VOID	QQ_RamBeyondMonitorGetbyTimer(T_pVOID hAllocator);



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

#ifndef AUTO_MEMORY_LEAK_MONITOR
	//#define AUTO_MEMORY_LEAK_MONITOR		//�ڴ�й©�Զ������
#endif

#ifndef AUTO_MEMORY_BEYOND_MONITOR
	//#define AUTO_MEMORY_BEYOND_MONITOR	//�ڴ�Խ���Զ������
#endif
*/
T_VOID	Fwl_RamBeyondMonitorGetbyTimer(T_U16 LID);	//�ڹ���Timer�е��ô˺��������ϼ���ڴ�Խ��״��
T_VOID	Fwl_RamLeakMonitorHooktoSM(T_VOID);		//ʹ��״̬�������������ڴ�й©
T_VOID	Fwl_RamLeakMonitorPointBeg(T_VOID);		//�򿪼��������µĴ������ڴ�й©
T_VOID	Fwl_RamLeakMonitorPointEnd(T_VOID);     //�رռ��������ϵĴ������ڴ�й©
T_VOID  Fwl_RamEnumerateEachSeg(T_VOID);		//ö�����е��ڴ��




#endif

