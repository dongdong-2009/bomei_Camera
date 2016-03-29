/**************************************************************************
*
* Copyrights (C) 2002, ANYKA software Inc
* All rights reserced.
*
* File name: AKThread.h
* Function: 
* Author: 
* Date:  
* Version: 1.0
*
***************************************************************************/
#ifndef __AKTHREAD_H__
#define __AKTHREAD_H__

#include "akdefine.h"
#include "AKComponent.h"
#include "AKInterface.h"
#include "anyka_types.h"
#include "fwl_vme.h"
#include "Fwl_sysevent.h"
#include "eng_debug.h"
#include "AKApp_Def.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* ===================================>        Macro define begin         <==================================*/
#define THREAD_PROP_BASE        0x0000
#define AKAPP_PROP_OFFSET       0x0100
#define SUBTHREAD_PROP_OFFSET   0x0200


/*===========================================================================================================*/
//*************************************************************************************************************
#define AK_VTFUNC_CHECK(i,p,fn,err) \
    while (AK_NULL != p)  \
    { \
        if ((AK_NULL != (((i *)p)->pvt)) &&  \
           (AK_NULL != (((i *)p)->pvt->fn)))\
        { \
          break; \
        } \
        p = (i *)*((i **)p->pData);\
    } \
    if (AK_NULL == p) \
    { \
         return err; \
    }


/* ===================================>        Macro define end          <===================================*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*�߳��������ö�ٶ���*/
typedef enum
{
  THREAD_PROP_NONE = THREAD_PROP_BASE,
  THREAD_PROP_EVENT_MSK,               /*�߳��¼�����*/
  THREAD_PROP_INTER_TASK,              /*�߳��ڲ�����*/
  THREAD_PROP_INTER_QUEUE,             /*�߳��ڲ�����*/ 
  THREAD_PROP_TASK_NAME,               /*�̵߳�ǰ��������*/
  THREAD_PROP_TASK_STATE,              /*�̵߳�ǰ����״̬*/
  THREAD_PROP_BACKGROUND,              /*�߳�������Ϣ*/
  THREAD_PROP_ATTACH_SUBTHREAD,        /*�߳�������߳�*/
  THREAD_PROP_DETACH_SUBTHREAD         /*�߳�ɾ�����߳�*/
  
}T_eTHREAD_PROPERTY;

/*�̳߳�ʼ����Ϣ����*/
typedef struct 
{
  T_CHR *pcszName;            /*�̵߳�����*/	
  T_U8  byPriority;		      /*�߳����ȼ�*/		
  T_U32 ulTimeSlice;		  /*�߳�ʱ��Ƭ*/
  T_U32 ulStackSize;          /*�̶߳�ջ��С*/
  T_U32 ulQueueSize;		  /*�̶߳�����ռ�ռ��С������Ϣ����*/
}T_THREAD_INITPARAM;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*==================================>      Define a call back  interface        <============================*/
typedef struct ICBThread ICBThread;

AK_INTERFACE(ICBThread)
{
    T_S32 (*Prepare)(ICBThread *pICBThread);
    T_S32 (*Handle)(ICBThread *pICBThread, T_SYS_EVTID eEvent, T_SYS_PARAM *pEvtParam);
    T_S32 (*Free)(ICBThread *pICBThread);
};

struct ICBThread
{
    AK_DEFINEVT(ICBThread);
    T_VOID *pData;
};
/*###########################################################################################################*/
/**
===============================================================================================================
##                                       Ӧ�ó���ص��ӿ�˵��
##  Ӧ�ó���ص��ӿ���Ϊ�˽����дӦ�ó���Ҫ�̳�����IThread�Ľӿں�������Ƴ�����һ���򻯽ӿڡ�Ӧ�ó���ֻҪ
##ʵ���������ӿں����Ϳ����ˡ����нӿں���Prepare���Բ���ʵ�֣�ֱ��������︳AK_NULL�Ϳ����ˡ���Ӧ�ó����
##��ʼ����ʱ�����Prepare()������Ȼ��ͽ������������Ϣѭ���ȴ���Ϣ��������Ϣ��ʱ���ͻ����Handle()����Ϣ��
##��Ӧ�ó���ʵ�ֵ�Handle����������Ӧ���˳�ʱ�����ͷ�֮ǰ�����Free()��ΪӦ�ó����ṩһ���ͷ���Դ�Ļ��ᡣ
===============================================================================================================
===============================================================================================================
Function:     T_S32 ICBThread_Prepare(ICBThread *pICBThread);
Description:  Ӧ�ó�������ǰ׼����
Parameters:
   pICBThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    Ӧ�ó��������ʱ���øýӿں�����
===============================================================================================================
===============================================================================================================
Function:     T_S32 ICBThread_Handle(ICBThread *pICBThread, T_SYS_EVTID eEvent, T_SYS_PARAM *pEvtParam);
Description:  Ӧ�ó�����Ϣ��������
Parameters:
   pICBThread   [in] ָ������ʵ���ӿڵ�ָ�롣
   eEvent       [in] �¼����
   pEvtParam    [in] �¼�������
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   Ӧ�ó����յ���ϢҪ����ʱ���ô˺����� 
===============================================================================================================
===============================================================================================================
Function:     T_S32 ICBThread_Free(ICBThread *pICBThread);
Description:  Ӧ�ó����ͷź�����
Parameters:
   pICBThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    Ӧ�ó����˳���ʵ�����ͷ�ǰ���ô˺�����
===============================================================================================================
  */
/*###########################################################################################################*/
/*========================================>    Define End     <==============================================*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct IThread IThread;

/*************************************************************************************************************/
/**
                       <---- DESCRIPTION ---->
#define AK_INHERIT_ITHREAD(name) \ 
        AK_INHERIT_IUNKNOWN(name); \
        //Add your function pointer list
        //of virtual table here...
If your extention doest inherit from IUnknown, you can use the Inherit 
macro AK_INHERIT_IYyy instead of AK_INHERIT_IUNKNOWN. For ezample:

#define AK_INHERIT_ICAT(name) \
        AK_INHERIT_IANIMAL(name); \
        ...    ...

*/
/*************************************************************************************************************/
#define AK_INHERIT_ITHREAD(name) \
      AK_INHERIT_IUNKNOWN(name); \
      T_S32  (*Run)(name* p##name);  \
      T_S32  (*Suspend)(name* p##name); \
      T_S32  (*Terminate)(name* p##name); \
      T_S32  (*Exit)(name* p##name); \
      T_S32  (*Register)(name* p##name, ICBThread *pIListener); \
      T_S32  (*SetProperty)(name* p##name, T_U16 wPropID, T_VOID*  pPropData); \
      T_S32  (*GetProperty)(name* p##name, T_U16 wPropID, T_VOID*  pPropData)




AK_INTERFACE(IThread)
{
    AK_INHERIT_ITHREAD(IThread);
};

struct IThread
{
    AK_DEFINEVT(IThread);
    T_VOID *pData;
};

/*************************************************************************************************************/
/**
###############################################################################################################
##                                     �������
##   �����ΪӦ���߳�������Ƕ�ʵ��������̳���IUnknown�ӿڡ���Ҫ��������Ĵ��������ٺ���Ϣ���еĴ�������Ϣ
## ѭ�������١������Ҳ�ṩ��Ӧ�ó�����������á�
###############################################################################################################
===============================================================================================================
Function:      T_U32 IThread_AddRef(IThread *pIThread);
Description:   ���ü�����1��
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_U32 IThread_Release(IThread *pIThread);
Description:   ���ü�����1��������ü���Ϊ0�����ͷŸ����ʵ����
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_Run(IThread *pIThread);
Description:   ����ָ���̣߳����ָ���̴߳���Terminate״̬�����Ȼ������̣߳�Ȼ���������С�
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_Suspend(IThread *pIThread);
Description:   ����ָ���̡߳�
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_Terminate(IThread *pIThread);
Description:   ��ָֹ���̡߳�
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls    [in] Ӧ�ó���ID��
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    �����ڵ�ǰ�߳�����ֹ�Լ���
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_Exit(IThread *pIThread);
Description:   �˳�ָ���̣߳�������ֹ���̡߳�ɾ�����̣߳�Ȼ���ͷŸ�Ӧ�ó���ʵ����
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    �����ڵ�ǰ�߳����˳��Լ���
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_Register(IThread *pIThread, ICBThread *pIListener);
Description:   ��ָ���߳�ע��һ��Ӧ�ó���ص��ӿڡ�
Parameters:
   pIThread    [in] ָ������ʵ���ӿڵ�ָ�롣
   pIListener  [in] ָ��Ӧ�ó���ص��ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   ���Ӧ�ó���ִ�й��캯��ʧ�ܣ�����Ҫȡ��ע�ᣬ��ע��һ����ָ��AK_NULL��
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_SetProperty(IThread *pIThread, T_U16 wPropID, T_VOID*  pPropData);
Description:   ����ָ���߳����ԡ�
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
   wPropID    [in] �������ID��
   pPropData  [in] ָ�������ڴ���ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   T_S32 IThread_SetEvtMsk(IThread *pIThread, T_U32 ulEvtMsk);
   DES:����ָ���̵߳��¼����롣
===============================================================================================================
===============================================================================================================
Function:      T_S32 IThread_GetProperty(IThread *pIThread, T_U16 wPropID, T_VOID*  pPropData);
Description:   ��ȡָ���߳����ԡ�
Parameters:
   pIThread   [in] ָ������ʵ���ӿڵ�ָ�롣
   wPropID    [in] �������ID��
   pPropData  [in/out] ָ�������ڴ���ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   T_BOOL IThread_IsBackground(IThread *pIThread);
   DES:�ж�ָ���߳�����Ӧ��Ӧ���Ƿ��Ǻ�̨Ӧ�á�
   T_U32 IThread_GetEvtMsk(IThread *pIThread);
   DES:��ȡָ���̵߳��¼����롣
   T_hTask IThread_GetTask(IThread *pIThread);
   DES:��ȡָ���̵߳��ڲ�����
   T_hQueue IThread_GetQueue(IThread *pIThread);
   DES:��ȡָ���̵߳��ڲ����С�
   T_S32 IThread_GetState(IThread *pIThread);
   DES:��ȡָ���̵߳ĵ�ǰ״̬��
===============================================================================================================
*/
/*************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
T_S32 CThread_New(IThread **ppi, T_THREAD_INITPARAM *pParam);

//===========================================================================================================//
__inline T_U32 IThread_AddRef(IThread *pIThread)
{
    AK_VTFUNC_CHECK(IThread, pIThread, AddRef, 0);
    return AK_GETVT(IThread, pIThread)->AddRef(pIThread); 
}

__inline T_U32 IThread_Release(IThread *pIThread)
{
    AK_VTFUNC_CHECK(IThread, pIThread, Release, 0);
    return AK_GETVT(IThread, pIThread)->Release(pIThread); 
}

__inline T_S32 IThread_Run(IThread *pIThread)
{
    AK_VTFUNC_CHECK(IThread, pIThread, Run, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->Run(pIThread); 
}

__inline T_S32 IThread_Suspend(IThread *pIThread)
{
    AK_VTFUNC_CHECK(IThread, pIThread, Suspend, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->Suspend(pIThread); 
}

__inline T_S32 IThread_Terminate(IThread *pIThread)
{
    AK_VTFUNC_CHECK(IThread, pIThread, Terminate, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->Terminate(pIThread);
}

__inline T_S32 IThread_Exit(IThread *pIThread)
{
    AK_VTFUNC_CHECK(IThread, pIThread, Exit, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->Exit(pIThread);
}

__inline T_S32 IThread_Register(IThread *pIThread, ICBThread *pIListener)
{
    AK_VTFUNC_CHECK(IThread, pIThread, Register, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->Register(pIThread, pIListener);
}

__inline T_S32 IThread_SetProperty(IThread *pIThread, T_U16 wPropID, T_VOID*  pPropData)
{
    AK_VTFUNC_CHECK(IThread, pIThread, SetProperty, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->SetProperty(pIThread, wPropID, pPropData);
}

__inline T_S32 IThread_GetProperty(IThread *pIThread, T_U16 wPropID, T_VOID*  pPropData)
{
    AK_VTFUNC_CHECK(IThread, pIThread, GetProperty, AK_EUNSUPPORT);
    return AK_GETVT(IThread, pIThread)->GetProperty(pIThread, wPropID, pPropData);
}


__inline T_BOOL IThread_IsBackground(IThread *pIThread)
{
    T_BOOL bIsBackground = AK_FALSE;
    
    IThread_GetProperty(pIThread, 
                       THREAD_PROP_BACKGROUND,
                       (T_VOID*)&bIsBackground);
    
    return bIsBackground;
}

__inline T_S32 IThread_SetEvtMsk(IThread *pIThread, T_U32 ulEvtMsk)
{
    return IThread_SetProperty(pIThread, 
                             THREAD_PROP_EVENT_MSK, 
                             (T_VOID*)&ulEvtMsk);
}

__inline T_U32 IThread_GetEvtMsk(IThread *pIThread)
{
    T_U32 ulEvtMsk = 0;
    
    IThread_GetProperty(pIThread, 
                       THREAD_PROP_EVENT_MSK, 
                       (T_VOID*)&ulEvtMsk);
    
    return ulEvtMsk;
}

__inline T_hTask IThread_GetTask(IThread *pIThread)
{
    T_hTask hTask = 0;
    
    IThread_GetProperty(pIThread, 
                       THREAD_PROP_INTER_TASK, 
                       (T_VOID*)&hTask);
    
    return hTask;
}

__inline T_hQueue IThread_GetQueue(IThread *pIThread)
{
    T_hQueue hQueue = AK_INVALID_QUEUE;
    
    IThread_GetProperty(pIThread, 
                       THREAD_PROP_INTER_QUEUE, 
                       (T_VOID*)&hQueue);
    return hQueue;
}

__inline T_pCSTR IThread_GetName(IThread *pIThread)
{
    T_CHR *pszName = AK_NULL;
    
    IThread_GetProperty(pIThread, 
                       THREAD_PROP_TASK_NAME, 
                       (T_VOID*)&pszName);
    
    return (T_pCSTR)pszName;
}

__inline T_S32 IThread_GetState(IThread *pIThread)
{
    T_S32 lState = 0;
    
    IThread_GetProperty(pIThread, 
                       THREAD_PROP_TASK_STATE, 
                       (T_VOID*)&lState);
    return lState;
}

__inline T_S32 IThread_AttachSubThread(IThread *pIThread, IThread *pISubThread)
{
    return IThread_SetProperty(pIThread, 
                               THREAD_PROP_ATTACH_SUBTHREAD, 
                               (T_VOID*)pISubThread);
}

__inline T_S32 IThread_DetachSubThread(IThread *pIThread, IThread *pISubThread)
{
    return IThread_SetProperty(pIThread, 
                               THREAD_PROP_DETACH_SUBTHREAD, 
                               (T_VOID*)pISubThread);
}

#endif //__AKTHREAD_H__

