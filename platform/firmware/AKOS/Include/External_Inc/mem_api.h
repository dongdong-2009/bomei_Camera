/************************************************************************************
* Copyright(c) 2006 Anyka.com
* All rights reserved.
*
* File	:	mem_api.h
* Brief :	heap memory allocator
*           
* 
* Version : 4.0.8
* Author  : ZhangMuJun
* Modify  : ZhangMuJun
* Data    : 2008-12-18
*************************************************************************************/
#ifndef __MEM_API_H__
#define __MEM_API_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "anyka_types.h"

#define  MEMLIB_VERSION		("4.0.9")


/**
 * @brief  Global heap memory allocator handler
 */
typedef T_pVOID	T_GLOBALMEMHandle;
typedef T_S32 	(*Ram_PrintfTraceFun)(const char *format, ...); 

/**
 * @brief  Thread/Reentry Control Function
 * @brief  zero will single thread ; else will multithread
 *
 * @note   Using Ram_SetLock must after  Ram_Initial(Ex).
 *         Using Ram_ClrLock must before Ram_Exit.
 *         Platform User Porting Ram_Lock/Ram_Unlock
 */
extern  T_VOID	Ram_Lock(T_U32 dwLock);
extern  T_VOID	Ram_Unlock(T_U32 dwLock);
extern  T_pVOID Fwl_MallocAndTrace(T_U32 size, T_pSTR filename, T_U32 line);
extern  T_pVOID	Fwl_ReMallocAndTrace(T_pVOID var, T_U32 size, T_pSTR filename, T_U32 line);
extern  T_pVOID Fwl_FreeAndTrace(T_pVOID var, T_pSTR filename, T_U32 line); 

#define Fwl_Malloc(size)		Fwl_MallocAndTrace((size), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))
#define Fwl_ReMalloc(var, size) Fwl_ReMallocAndTrace((var), (size), ((T_S8*)(__FILE__)), ((T_U32)__LINE__))
#define Fwl_Free(var)			Fwl_FreeAndTrace((var), ((T_S8*)(__FILE__)), ((T_U32)__LINE__)) 

T_VOID	Ram_ClrLock(T_GLOBALMEMHandle hMemory);
T_VOID	Ram_SetLock(T_GLOBALMEMHandle hMemory, T_U32 dwLock);

/* ********************************* 全局内存堆分配器 ********************************* */

/**
 * @brief  Initialize Global heap memory allocator
 * @brief  该内存库可以实例化
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_U8  *ptr :	global heap memory start adress
 * @param	T_U32 size :	global heap memory length           (必须小于 2^(26+align2) , align2是align的2幂次)
 * @param	T_U16 llenpad : 前边界设置长度 (用于检测内存前越界) (默认为0 0:不检测 其他值:检测)
 * @param   T_U8  padchk :  开启精密检测						(默认为0 0:不开启)
 * @param	T_U16 rlenpad : 后边界设置长度 (用于检测内存后越界) (默认为0 0:不检测 其他值:检测)
 * @param	T_U8  lvar :	前边界预填充值 (用于检测内存前越界)	(默认为0xFC)
 * @param	T_U8  rvar :	后边界预填充值 (用于检测内存后越界)	(默认为0xCF)
 * @param	T_U8 align :	内存对齐基数   (2^[3,4,5,6,7, 8])	(默认为16)
 * @param	T_U32 lenfree : 释放阵列的长度 (32X)				(默认为6400) [必须小于 2^14; 大内存=lenfree*align]
 * @param	T_U8  sos :		最大大内存组合的可能所占总内存比例  (默认为50) [用于自适应调节搜索速度和碎片利用度 : 非强制因素]
 * @param   T_U8 split :    泡泡产生分裂的最小块间隔			(默认为4) [用于减少小碎片的数目]
 * @param   T_U8  wildchk : 开启野操作检测						(默认为0  0:不检测 0x10:只检测内存头8个字节 0x01:只检测内存头8个字节外 0x11:全检测 )  
 * @param   T_U8  wvar :    未使用内存预填充值(用于检测野指针)  (默认为0xCC) 
 * @param   Ram_PrintfTraceFun lpPrintf : 是否打印信息			(默认为AK_NULL, 不打印)
 *
 * @return  ram handle for success;else for AK_NULL
 *
 */
T_GLOBALMEMHandle	Ram_Initial(T_U8 *ptr, T_U32 size);

T_GLOBALMEMHandle	Ram_InitialEx(T_U8  *ptr,     T_U32 size,											//堆参数
								  T_U8  padchk,   T_U16 llenpad,  T_U16 rlenpad, T_U8 lvar, T_U8 rvar,  //越界检测参数
								  T_U8  wildchk,  T_U8 wvar,											//野操作检测参数
								  T_U8  align,    T_U32 lenfree, T_U8 sos, T_U8 split,					//高级分配器优化参数
								  Ram_PrintfTraceFun lpPrintf);


/**
 * @brief  Destroy Global heap memory allocator
 * @brief  该内存库可以实例化
 * @
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 *
 * @return  AK_NULL
 *
 */
T_GLOBALMEMHandle	Ram_Exit(T_GLOBALMEMHandle hMemory);


/**
 * @brief  Malloc one memory block from global heap memory
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_U32 size : want memory size 
 * @param   T_S8 *filename : alloc hander location filename
 * @param   T_S8 *fileline : alloc hander location fileline
 *
 * @return  T_pVOID : memory address for success, AK_NULL for failure 
 *
 */
T_pVOID	Ram_Alloc(T_GLOBALMEMHandle hMemory, T_U32 size, T_S8 *filename, T_U32 fileline);

/**
 * @brief  Remalloc one memory block from global heap memory
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param   T_pVOID var : old memory address
 * @param	T_U32 size : want memory size 
 * @param   T_S8 *filename : alloc hander location filename
 * @param   T_S8 *fileline : alloc hander location fileline
 *
 * @return  T_pVOID : new memory address for success, AK_NULL for failure 
 *
 */
T_pVOID	Ram_Realloc(T_GLOBALMEMHandle hMemory, T_pVOID var, T_U32 size, T_S8 *filename, T_U32 fileline); 

/**
 * @brief  Free one memory block to global heap memory
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_pVOID var : memory start address alloced before 
 * @param   T_S8 *filename : alloc hander location filename
 * @param   T_S8 *fileline : alloc hander location fileline
 *
 * @return  T_pVOID : AK_NULL
 *
 */
T_pVOID Ram_Free(T_GLOBALMEMHandle hMemory, T_pVOID var, T_S8 *filename, T_U32 fileline);


/**
 * @brief  Debug memory relation info
 * @
 * @memory block map
 */
typedef struct tagT_MEMORY_TRACE
{
	T_pVOID		addr;		//memory ponter
	T_S32		size;		//memory size
	T_U32		line;		//memory alloc location line
	T_S8		*filename;	//memory alloc location file
	T_U8		old;		//used or not
	T_S32		reqSize;	//real request alloc size
}T_MEMORY_TRACE, *T_lpMEMORY_TRACE;

/**
 * @brief  Debug memory relation info
 * @
 * @memory map callback func define 
 */
typedef T_VOID	(*Ram_EnumMemTraceFun)(T_GLOBALMEMHandle hMemory, const T_MEMORY_TRACE *map, T_pVOID attatch);



/**
 * @brief  Enumerate memory statck status info
 * @只能用于查询内存状态和检测显式内存泄漏情况
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	Ram_EnumMemTraceFun enumFun : user define memory map callback query and prontf function
 * @param   T_pVOID attach : user self data needed by callback function
 *
 * @return  T_pVOID : AK_NULL
 *
 */
T_VOID Ram_EnumMemTraceInfo(T_GLOBALMEMHandle hMemory, Ram_EnumMemTraceFun enumFun, T_pVOID attach);


/**
 * @brief  Debug memory heat info
 * @
 * @memory block map
 */
typedef struct tagT_MEMORY_STATUS
{
	T_U32		line;		//memory alloc location line
	T_S8		*filename;	//memory alloc location file
	T_U32		hits;		//location handler alloc times : free will --
	T_U32		ratio;		//location handler ratio of all (本Handler所分配的总内存/所有已经分配的总内存)*100
}T_MEMORY_STATUS, *T_lpMEMORY_STATUS;

/**
 * @brief  Debug memory heat info
 * @
 * @memory map callback func define 
 */
typedef T_VOID	(*Ram_EnumMemStatusFun)(T_GLOBALMEMHandle hMemory, const T_MEMORY_STATUS *map, T_pVOID attatch);

/**
 * @brief  Enumerate memory statck status info
 * @只能用于查询内存热点状态和检测隐式内存泄漏情况
 * @可以为进一步优化提供依据
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	Ram_EnumMemStatusFun enumFun : user define memory map callback query and prontf function
 * @param   T_pVOID attach : user self data needed by callback function
 *
 * @return  T_pVOID : AK_NULL
 *
 */
T_VOID Ram_EnumMemStatusInfo(T_GLOBALMEMHandle hMemory, Ram_EnumMemStatusFun enumFun, T_pVOID attach);



/**
 * @brief  Debug memory wild info
 * @
 * @memory block map
 */
typedef enum tagT_WILD_TYPE
{
	WILD_OK			= 0,		//ptr是有效指针
	WILD_WILD		= 0x100,	//ptr是野指针
	WILD_WILD_PREV	= 0x101,	//ptr是野指针, 产生过非法写前8个字节动作
	WILD_WILD_BACK	= 0x110,	//ptr是野指针, 产生过非法写8个字节后数据动作
	WILD_WILD_BOTH	= 0x111,	//ptr是野指针, 产生过非法写超过8个字节数据动作
	WILD_INVALID    = 0x200,	//ptr是无效指针(属于某个有效内存块，非内存头或者内存头被毁坏)
	WILD_INVALID_UNHEAP			= 0x210,	//ptr是无效指针(不在堆内存地址范围内)
	WILD_INVALID_INHEAP			= 0x220,	//ptr是无效指针(已经回收给堆内存)
	WILD_INVALID_INHEAP_WILD	= 0x221,	//ptr是无效指针(已经回收给堆内存),产生过非法写数据动作
	WILD_ERR					//UnKnwon error
}T_WILD_TYPE;
    
typedef struct tagT_MEMORY_WILDER_TRACE
{
	T_pVOID		addr;		//memory ponter
	T_S32		size;		//memory size
	T_U32		line;		//memory alloc location line
	T_S8		*filename;	//memory alloc location file
	T_S32		reqSize;	//real request alloc size
	T_WILD_TYPE type;		//wild type
}T_MEMORY_WILDER_TRACE, *T_lpMEMORY_WILDER_TRACE;


/**
 * @brief  Enumerate memory statck status info
 * @只能用于检测野操作情况
 * @从堆内存头开始检测,检测到第一个野操作使用即退出
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_MEMORY_WILDER_TRACE *wildInfo : wild info loader
 *
 * @return  AK_TRUE for no wild ;else for AK_FALSE
 *
 */
T_BOOL Ram_CheckWilder(T_GLOBALMEMHandle hMemory, T_MEMORY_WILDER_TRACE *wildInfo);   

/**
 * @brief  Enumerate memory statck status info
 * @只能用于直接检测指定指针的野操作情况
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_WILD_TYPE *wildType : wild type
 *
 * @return  AK_TRUE for no beyond ;else for AK_FALSE
 *
 */
T_BOOL Ram_CheckWildrPtr(T_GLOBALMEMHandle hMemory, T_VOID *ptr, T_WILD_TYPE *wildType);


/**
 * @brief  Debug memory beyond info
 * @
 * @memory block map
 */
typedef enum tagT_BEYOND_TYPE
{
	BEYOND_OK		= 0x000,	//ptr无越界
	BEYOND_PREV		= 0x101,	//ptr本身发生前向越界
	BEYOND_BACK		= 0x110,	//ptr本身发生后向越界
	BEYOND_BOTH		= 0x111,	//ptr本身发生全越界
	BEYOND_INVALID	= 0x2000,	//ptr是无效指针
	BEYOND_INVALID_WILD		= BEYOND_INVALID+WILD_WILD,				//ptr是野指针,不检测
	BEYOND_INVALID_INVALID	= BEYOND_INVALID+WILD_INVALID,			//ptr是无效指针,不检测
	BEYOND_INVALID_UNHEAP	= BEYOND_INVALID+WILD_INVALID_UNHEAP,	//ptr是无效指针(不在堆内存地址范围内), 不检测
	BEYOND_INVALID_INHEAP	= BEYOND_INVALID+WILD_INVALID_INHEAP,	//ptr是无效指针(已经回收给堆内存), 不检测
	BEYOND_ERR					//UnKnwon error
}T_BEYOND_TYPE;

typedef struct tagT_MEMORY_BEYOND_TRACE
{
	T_pVOID		addr;		//memory ponter
	T_S32		size;		//memory size
	T_U32		line;		//memory alloc location line
	T_S8		*filename;	//memory alloc location file
	T_U8		old;		//used or not
	T_S32		reqSize;	//real request alloc size
	T_U16		loc;		//first beyond location <only useful for BEYOND_BACK>
	T_U16		cnt;		//beyond amount
	T_BEYOND_TYPE type;		//beyond type
}T_MEMORY_BEYOND_TRACE, *T_lpMEMORY_BEYOND_TRACE;

/**
 * @brief  Enumerate memory statck status info
 * @只能用于检测内存越界情况
 * @从堆内存头开始检测,检测到第一个越界即退出
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_MEMORY_BEYOND_TRACE *beyondInfo : beyond info loader
 *
 * @return  AK_TRUE for no beyond ;else for AK_FALSE
 *
 */
T_BOOL Ram_CheckBeyond(T_GLOBALMEMHandle hMemory, T_MEMORY_BEYOND_TRACE *beyondInfo);   

/**
 * @brief  Enumerate memory statck status info
 * @只能用于直接检测指定内存越界情况
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_BEYOND_TYPE *beyondType : beyond type
 *
 * @return  AK_TRUE for no beyond ;else for AK_FALSE
 *
 */
T_BOOL Ram_CheckBeyondPtr(T_GLOBALMEMHandle hMemory, T_VOID *ptr, T_BEYOND_TYPE *beyondType);


/**
 * @brief  过程中自动化内存泄漏检测器
 * @
 * @memory auto callback func define 
 */
typedef T_VOID	(*Ram_EnterStateMachine)(T_GLOBALMEMHandle hMemory);
typedef T_VOID	(*Ram_LeaveStateMachine)(T_GLOBALMEMHandle hMemory);

/**
 * @brief  要检测代码段的内存泄漏状况 : 开始位置
 * @请保持配对使用
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 */
T_VOID  RAM_ENTER_LEAKCHK_SECTION(T_GLOBALMEMHandle hMemory);


/**
 * @brief  要检测代码段的内存泄漏状况 : 结束位置
 * @请保持配对使用
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 */
T_VOID	RAM_LEAVE_LEAKCHK_SECTION(T_GLOBALMEMHandle hMemory);



/**
 * @brief  enable memory leak auto trace mechnics
 * @只能用于状态机间内存泄漏自动检测 
 * @有2000内存块的限制,有20个状态机栈层的限制
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	Ram_EnumMemTraceFun enumFun : callback printf func
 * @param   T_U32 logCnt : log memory count <default is 2000>
 * @param   Ram_EnterStateMachine *enterSection : GET HOOK for enter critical leakchk callback function
 * @param   Ram_LeaveStateMachine *leaveSection : GET HOOK for leave critical leakchk callback function
 *
 * @return  T_VOID
 *
 * @return  AK_TRUE for auto ok else return false
 *
 */
T_BOOL Ram_EnableAutoLeakTrace(T_GLOBALMEMHandle hMemory, Ram_EnumMemTraceFun enumFun, T_U32 logCnt, Ram_EnterStateMachine *enterSection, Ram_LeaveStateMachine *leaveSection);

/**
 * @brief  disable memory leak auto trace mechnics
 * @只能用于状态机间内存泄漏自动检测
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 *
 * @return  T_VOID
 *
 */
T_VOID Ram_DisableAutoLeakTrace(T_GLOBALMEMHandle hMemory);

/**
 * @brief  内存信息
 * @
 * @memory 
 */
typedef struct tagGLOBALMEMInfo
{
	T_U16	align;			//内存对齐基数	
	T_U32	szTotal;		//全局堆内存的总大小
	T_U32	szBlocks;		//全局堆内存的总块个数
	T_U32   szUsed;			//当前使用的内存的总大小
	T_U32	blkUsed;		//当前使用的内存的总块数
	T_U32	szSpare;		//理论总剩余未分配的内存大小<包含头，调试，空闲等空间>
	T_U32	cntBubble;		//内存泡泡总个数<包括正在使用的+已经释放但未回收的>
	T_U16   padblkBubble;	//单个内存泡泡除本身SIZE外额外添加空间占用块数
	T_U16	padszBubble;	//单个内存泡泡除本身SIZE外额外添加空间字节数
}GLOBALMEMInfo;	

/**
 * @brief  get global memory info
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	GLOBALMEMInfo *info : mem info
 *
 * @return  AK_FALSE for not initialize memory else return AK_TRUE
 *
 */
T_BOOL Ram_GetRamInfo(T_GLOBALMEMHandle hMemory, GLOBALMEMInfo *info);

/**
 * @brief  get memory info
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	GLOBALMEMInfo *info : mem info
 *
 * @return  AK_FALSE for not initialize memory else return AK_TRUE
 * &return  wildType information for AK_FALSE to query.
 *
 */
T_BOOL Ram_GetPtrInfo(T_GLOBALMEMHandle hMemory, T_pVOID var, T_MEMORY_TRACE *map, T_WILD_TYPE *wildType);

/**
 * @brief  get allocable largest memory bubble
 * @note:  该函数没有优化(速度远慢于alloc)，勿过度频繁调用 
 * @author  ZMJ
 * @date	6/12/2008 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 *
 * @return  allocable largest size
 *
 */
T_U32  Ram_GetLargestSize_Allocable(T_GLOBALMEMHandle hMemory);



#ifdef __cplusplus
}
#endif


#endif


