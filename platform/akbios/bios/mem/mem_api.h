/************************************************************************************
* Copyright(c) 2006 Anyka.com
* All rights reserved.
*
* File	:	mem_api.h
* Brief :	heap memory allocator
*           
* 
* Version : 1.0
* Author  : ZhangMuJun
* Modify  : 
* Data    : 2006-06-24
*************************************************************************************/
#ifndef __MEM_API_H__
#define __MEM_API_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "anyka_types.h"




/* ********************************* Common ******************************************* */

#define MEMLIB_VERSION	("2.0.1")


typedef T_S32 	(*Ram_PrintfTraceFun)(T_pCSTR format, ...); 
	

/* ********************************* ȫ���ڴ�ѷ����� ********************************* */

/**
 * @brief  Global heap memory allocator handler
 */
typedef T_pVOID		T_GLOBALMEMHandle;


/**
 * @brief  Initialize Global heap memory allocator
 * @brief  ���ڴ�����ʵ����
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_U8  *ptr :	global heap memory start adress
 * @param	T_U32 size :	global heap memory length
 * @param	T_U16 llenpad : ǰ�߽����ó��� (���ڼ���ڴ�ǰԽ��) (Ĭ��Ϊ0)
 * @param	T_U16 rlenpad : ��߽����ó��� (���ڼ���ڴ��Խ��) (Ĭ��Ϊ0)
 * @param	T_U8  lvar :	ǰ�߽�Ԥ���ֵ (���ڼ���ڴ�ǰԽ��)	(Ĭ��Ϊ0x27)
 * @param	T_U8  rvar :	��߽�Ԥ���ֵ (���ڼ���ڴ��Խ��)	(Ĭ��Ϊ0x75)
 * @param	T_U8 align :	�ڴ�������   (8X ==�鵥λ��С)	(Ĭ��Ϊ16)
 * @param	T_U32 lenfree : �ͷ����еĳ��� (32X)				(Ĭ��Ϊ6400) [SIZE=6400*16/1024=100k : SIZE��Χ�ڴ����ҷ���, SIZE��Χ����ҵ������]
 * @param	T_U8  sos :		�����ڴ���ϵĿ�����ռ���ڴ����  (Ĭ��Ϊ50) [��������Ӧ���������ٶȺ���Ƭ���ö� : ��ǿ������]
 * @param   T_U8 split :    ���ݲ������ѵ���С����			(Ĭ��Ϊ4) [���ڼ���С��Ƭ����Ŀ]
 * @param   Ram_PrintfTraceFun lpPrintf : �Ƿ��ӡ��Ϣ			(Ĭ��ΪAK_NULL, ����ӡ)
 *
 * @return  ram handle for success;else for AK_NULL
 *
 */
T_GLOBALMEMHandle	Ram_Initial(T_U8 *ptr, T_U32 size);

T_GLOBALMEMHandle	Ram_InitialEx(T_U8 *ptr, T_U32 size, T_U16 llenpad, T_U16 rlenpad, T_U8 lvar, T_U8 rvar, T_U8 align, T_U32 lenfree, T_U8 sos, T_U8 split, Ram_PrintfTraceFun lpPrintf);


/**
 * @brief  Destroy Global heap memory allocator
 * @brief  ���ڴ�����ʵ����
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
}T_MEMORY_TRACE, *T_lpMEMORY_TRACE;

/**
 * @brief  Debug memory relation info
 * @
 * @memory map callback func define 
 */
typedef T_VOID	(*Ram_EnumMemTraceFun)(T_GLOBALMEMHandle hMemory, const T_MEMORY_TRACE *map, T_pVOID attatch);



/**
 * @brief  Enumerate memory statck status info
 * @ֻ�����ڲ�ѯ�ڴ�״̬�ͼ����ʽ�ڴ�й©���
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
	T_U32		ratio;		//location handler ratio of all (��Handler����������ڴ�/�����Ѿ���������ڴ�)*100
}T_MEMORY_STATUS, *T_lpMEMORY_STATUS;

/**
 * @brief  Debug memory heat info
 * @
 * @memory map callback func define 
 */
typedef T_VOID	(*Ram_EnumMemStatusFun)(T_GLOBALMEMHandle hMemory, const T_MEMORY_STATUS *map, T_pVOID attatch);

/**
 * @brief  Enumerate memory statck status info
 * @ֻ�����ڲ�ѯ�ڴ��ȵ�״̬�ͼ����ʽ�ڴ�й©���
 * @����Ϊ��һ���Ż��ṩ����
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
 * @brief  Debug memory beyond info
 * @
 * @memory block map
 */
typedef enum tagT_BEYOND_TYPE
{
	BEYOND_OK		= 0x000,	//ptr��Խ��
	BEYOND_PREV		= 0x101,	//ptr������ǰ��Խ��
	BEYOND_BACK		= 0x110,	//ptr����������Խ��
	BEYOND_BOTH		= 0x111,	//ptr������ȫԽ��
	BEYOND_SUFFER	= 0x201,	//ptr��ǰ���ڵ�Խ�� 
	BEYOND_INVADE	= 0x202,	//ptrԽ�絽�����ڵ�
	BEYOND_ERR					//UnKnwon error
}T_BEYOND_TYPE;

typedef struct tagT_MEMORY_BEYOND_TRACE
{
	T_pVOID		addr;		//memory ponter
	T_S32		size;		//memory size
	T_U32		line;		//memory alloc location line
	T_S8		*filename;	//memory alloc location file
	T_U8		old;		//used or not
	T_BEYOND_TYPE type;		//beyond type
}T_MEMORY_BEYOND_TRACE, *T_lpMEMORY_BEYOND_TRACE;

/**
 * @brief  Enumerate memory statck status info
 * @ֻ�����ڼ���ڴ�Խ�����
 * @�Ӷ��ڴ�ͷ��ʼ���,��⵽��һ��Խ�缴�˳�
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
 * @ֻ������ֱ�Ӽ��ָ���ڴ�Խ�����
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
 * @brief  Debug memory wild info
 * @
 * @memory block map
 */
typedef enum tagT_WILD_TYPE
{
	WILD_OK			= 0,	//ptr����Чָ��
	WILD_WILD		= 1,	//ptr��Ұָ��
	WILD_OUTRANGE	= 2,	//ptr�����˶��ڴ��ַ��Χ
	WILD_NOTSTART	= 3,	//ptr�����ڴ�ͷָ��
	WILD_ERR				//UnKnwon error
}T_WILD_TYPE;

/**
 * @brief  Enumerate memory statck status info
 * @ֻ������ֱ�Ӽ��ָ���ڴ�Ϸ����
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	T_WILD_TYPE *wildInfo : wild info loader
 *
 * @return  AK_TRUE for no wild ;else for AK_FALSE
 *
 */
T_BOOL Ram_CheckPtr(T_GLOBALMEMHandle hMemory, T_VOID *var, T_WILD_TYPE *wildType); 


/**
 * @brief  �������Զ����ڴ�й©�����
 * @
 * @memory auto callback func define 
 */
typedef T_VOID	(*Ram_EnterStateMachine)(T_GLOBALMEMHandle hMemory);
typedef T_VOID	(*Ram_LeaveStateMachine)(T_GLOBALMEMHandle hMemory);

/**
 * @brief  Ҫ������ε��ڴ�й©״�� : ��ʼλ��
 * @�뱣�����ʹ��
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 */
T_VOID  RAM_ENTER_LEAKCHK_SECTION(T_GLOBALMEMHandle hMemory);


/**
 * @brief  Ҫ������ε��ڴ�й©״�� : ����λ��
 * @�뱣�����ʹ��
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 */
T_VOID	RAM_LEAVE_LEAKCHK_SECTION(T_GLOBALMEMHandle hMemory);



/**
 * @brief  enable memory leak auto trace mechnics
 * @ֻ������״̬�����ڴ�й©�Զ���� 
 * @��2000�ڴ�������,��20��״̬��ջ�������
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_GLOBALMEMHandle hMemory : memory allocator handler
 * @param	Ram_EnumMemTraceFun enumFun : callback printf func
 * @param   Ram_EnterStateMachine *enterSection : GET HOOK for enter critical leakchk callback function
 * @param   Ram_LeaveStateMachine *leaveSection : GET HOOK for leave critical leakchk callback function
 *
 * @return  T_VOID
 *
 * @return  AK_TRUE for auto ok else return false
 *
 */
T_BOOL Ram_EnableAutoLeakTrace(T_GLOBALMEMHandle hMemory, Ram_EnumMemTraceFun enumFun, Ram_EnterStateMachine *enterSection, Ram_LeaveStateMachine *leaveSection);

/**
 * @brief  disable memory leak auto trace mechnics
 * @ֻ������״̬�����ڴ�й©�Զ����
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
 * @brief  Memory Information
 * @
 * @memory 
 */
typedef struct tagGLOBALMEMInfo
{
	T_U16	align;			//�ڴ�������
	T_U32	szTotal;		//ȫ�ֶ��ڴ���ܴ�С
	T_U32	szBlocks;		//ȫ�ֶ��ڴ���ܿ����
	T_U32   szUsed;			//��ǰʹ�õ��ڴ���ܴ�С
	T_U32	blkUsed;		//��ǰʹ�õ��ڴ���ܿ���
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
 *
 */
T_BOOL Ram_GetPtrInfo(T_GLOBALMEMHandle hMemory, T_pVOID var, T_MEMORY_TRACE *map);



/* ********************************* �ֲ��ڴ�ѷ����� ********************************* */

/**
 * @brief  Local heap memory allocator handler
 */
typedef T_pVOID		T_LOCALMEMHandle;

/**
 * @brief  Initialize Local heap memory allocator
 * @brief  ���ڴ�����ʵ����
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_pVOID membody : external largebuf start address 
 * @param	T_U32 maxmemsize: external largebuf size (if membody is AK_NULL will drive malloc largebuf from global heap memory)
 * @param	T_U16 llenpad : ǰ�߽����ó��� (���ڼ���ڴ�ǰԽ��) (Ĭ��Ϊ0)
 * @param	T_U16 rlenpad : ��߽����ó��� (���ڼ���ڴ��Խ��) (Ĭ��Ϊ0)
 * @param	T_U8  lvar :	ǰ�߽�Ԥ���ֵ (���ڼ���ڴ�ǰԽ��) (Ĭ��Ϊ0x27)
 * @param	T_U8  rvar :	��߽�Ԥ���ֵ (���ڼ���ڴ��Խ��) (Ĭ��Ϊ0x75)
 * @param	T_U8 align :	�ڴ�������   (16X)				(Ĭ��Ϊ16)
 * @param	T_U32 lenfree : �ͷ����еĳ��� 						(Ĭ��Ϊ512) [SIZE=512*16/1024=8k : SIZE��Χ�ڴ����ҷ���, SIZE��Χ����ҵ������]
 *
 * @return  T_LOCALMEMHandle for success else for AK_NULL
 *
 */
T_LOCALMEMHandle CreateBlockMem(T_pVOID membody, T_U32 maxmemsize);

T_LOCALMEMHandle CreateBlockMemEx(T_pVOID membody, T_U32 maxmemsize, T_U16 llenpad, T_U16 rlenpad, T_U8 lvar, T_U8 rvar, T_U8 align, T_U32 lenfree);


/**
 * @brief  Destroy local heap memory allocator
 * @
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param	T_LOCALMEMHandle hBlockMem : assigned local memory allocator handler
 *
 * @return  AK_NULL
 *
 */
T_LOCALMEMHandle ReleaseBlockMem(T_LOCALMEMHandle hBlockMem);


/**
 * @brief  Malloc one memory block from local heap memory
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	T_U32 size : want memory size 
 * @param   T_S8 *filename : alloc hander location filename
 * @param   T_S8 *fileline : alloc hander location fileline
 *
 * @return  T_pVOID : memory address for success, AK_NULL for failure 
 *
 */
T_pVOID  Blk_Malloc(T_LOCALMEMHandle hBlockMem, T_U32 size, T_S8 *filename, T_U32 fileline);


/**
 * @brief  Free one memory block from local heap memory
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	T_pVOID ptr : memory start address alloced before
 * @param   T_S8 *filename : alloc hander location filename
 * @param   T_S8 *fileline : alloc hander location fileline
 *
 * @return  T_pVOID : memory address for success, AK_NULL for failure 
 *
 */
T_pVOID  Blk_Free(T_LOCALMEMHandle hBlockMem, T_pVOID ptr, T_pSTR filename, T_U16 line);

/**
 * @brief  Enumerate memory statck status info
 * @ֻ�����ڲ�ѯ�ڴ�״̬�ͼ����ʽ�ڴ�й©���
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	Ram_EnumMemTraceFun enumFun : user define memory map callback query and prontf function
 * @param   T_pVOID attach : user self data needed by callback function
 *
 * @return  T_pVOID : AK_NULL
 *
 */
T_VOID Blk_EnumMemTraceInfo(T_LOCALMEMHandle hBlockMem, Ram_EnumMemTraceFun enumFun, T_pVOID attach);

/**
 * @brief  Enumerate memory statck status info
 * @ֻ�����ڲ�ѯ�ڴ��ȵ�״̬�ͼ����ʽ�ڴ�й©���
 * @����Ϊ��һ���Ż��ṩ����
 *
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	Ram_EnumMemStatusFun enumFun : user define memory map callback query and prontf function
 * @param   T_pVOID attach : user self data needed by callback function
 *
 * @return  T_VOID
 *
 */
T_VOID Blk_EnumMemStatusInfo(T_LOCALMEMHandle hBlockMem, Ram_EnumMemStatusFun enumFun, T_pVOID attach);

/**
 * @brief  Enumerate memory statck status info
 * @ֻ�����ڼ���ڴ�Խ�����
 * @�Ӷ��ڴ�ͷ��ʼ���,��⵽��һ��Խ�缴�˳�
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	T_MEMORY_BEYOND_TRACE *beyondInfo : beyond info loader
 *
 * @return  AK_TRUE for no beyond ;else for AK_FALSE
 *
 */
T_BOOL Blk_CheckBeyond(T_LOCALMEMHandle hBlockMem, T_MEMORY_BEYOND_TRACE *beyondInfo);   

/**
 * @brief  Enumerate memory statck status info
 * @ֻ������ֱ�Ӽ��ָ���ڴ�Խ�����
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	T_MEMORY_BEYOND_TRACE *beyondInfo : beyond info loader
 *
 * @return  AK_TRUE for no beyond ;else for AK_FALSE
 *
 */
T_BOOL Blk_CheckBeyondPtr(T_LOCALMEMHandle hBlockMem, T_VOID *ptr, T_BEYOND_TYPE *beyondType);

/**
 * @brief  Enumerate memory statck status info
 * @ֻ������ֱ�Ӽ��ָ���ڴ�Ϸ����
 * @author  ZMJ
 * @date	6/24/2006 
 *
 * @param   T_LOCALMEMHandle hBlockMem : assigned local allocator handler
 * @param	T_WILD_TYPE *wildInfo : wild info loader
 *
 * @return  AK_TRUE for no wild ;else for AK_FALSE
 *
 */
T_BOOL Blk_CheckPtr(T_LOCALMEMHandle hBlockMem, T_VOID *ptr, T_WILD_TYPE *wildType); 









#ifdef __cplusplus
}
#endif


#endif






