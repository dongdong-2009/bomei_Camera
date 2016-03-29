/**************************************************************************
*
* Copyrights (C) 2002, ANYKA software Inc
* All rights reserced.
*
* File name: AKAppMgr.h
* Function: 
* Author: 
* Date:  
* Version: 1.0
*
***************************************************************************/
#ifndef __AKAPPMGR_H__
#define __AKAPPMGR_H__

#include "AKComponent.h"
#include "AKInterface.h"
#include "anyka_types.h"
#include "Fwl_sysevent.h"
#include "AKThread.h"
#include "AKSubThread.h"
#include "AKVector.h"
#include "AKFrameStream.h"
#include "Fwl_pfDisplay.h"
#include "Akos_api.h"

///////////////////////////////////////////////////////////////////////////
/*Ӧ�ó���IDö�ٶ��壬ֵΪ16λ������*/
enum
{
  AKAPP_CLSID_NONE    = 0x0000,
  AKAPP_CLSID_APPMGR  = 0x0001,    /*Ӧ�ù�����*/
  AKAPP_CLSID_MMI,                 /*MMIӦ��*/
  AKAPP_CLSID_MEDIA,               /*��ý������*/
  AKAPP_CLSID_DECODEIMAGE,         /*ͼƬ��������*/
  AKAPP_CLSID_TIMERHANDLE,         /* ����timer�߳� */ 
  AKAPP_CLSID_GLONAV,              /* GLONAV GPS �߳� */ 
  AKAPP_CLSID_EMAP,                /* CARELAND EMAP�߳� */
  AKAPP_CLSID_PUBLIC,              /* ������̨�߳�*/
  AKAPP_CLSID_VIDEO,               /* Video Decoder */
  AKAPP_CLSID_AUDIO,               /* Audio Decoder */
  AKAPP_CLSID_POWERON,		   	   /* PowerOn Thread */
  AKAPP_CLSID_AUDIOLIST,           /* Audio List */
  AKAPP_CLSID_VIDEOLIST,           /* Video List */
  AKAPP_CLSID_XXX,                 /*���������*/
  
  AKAPP_CLSID_ACTIVE  = 0xffff     /*��ǰ�����Ӧ��*/
};

/*Ӧ�ù������ڲ��¼�����*/
enum 
{
    APPMGR_EVT_ADDENTRY = 1,
    APPMGR_EVT_ACTIVEAPP,
    APPMGR_EVT_DEACTIVEAPP,
    APPMGR_EVT_DELETEENTRY
};

/*Ӧ�û�����Ϣ����*/
typedef struct 
{
    T_U16              wAppCls;     /*Ӧ�ó����ID*/
    T_THREAD_INITPARAM sInitparam;  /*Ӧ�ó����ʼ����Ϣ*/
}T_AppInfo;

/*Ӧ�ó����¼����*/
typedef struct tagAppEntry
{
    T_U16       wAppCls;    /*Ӧ�ó����ID*/
    IThread    *pIThread;   /*Ӧ�ó���ʵ���ӿ�ָ��*/
}T_AppEntry;

/*Ӧ�ó������Ͷ���*/
typedef enum
{
   AKAPP_FG_APP,
   AKAPP_BG_APP,
   AKAPP_ALL_APP
}T_EAppType;

///////////////////////////////////////////////////////////////////////////
typedef struct IAppMgr IAppMgr;

#define AK_INHERIT_IAPPMGR(name) \
      AK_INHERIT_ITHREAD(name); \
      T_S32    (*PostEventEx)(name* p##name, T_U16 wAppCls, T_SYS_MAILBOX *pMailBox,T_pfnEvtCmp pfnCmp,T_BOOL bIsUnique, T_BOOL bIsHead, T_BOOL bIsSuspend); \
      IThread* (*GetApp)(name* p##name, T_U16 wAppCls); \
      T_S32    (*GetAppCount)(name* p##name, T_EAppType eAppType); \
      T_S32    (*ReportAppsStatus)(name* p##name)


AK_INTERFACE(IAppMgr)
{
    AK_INHERIT_IAPPMGR(IAppMgr);
};

struct IAppMgr
{
    AK_DEFINEVT(IAppMgr);
    T_VOID *pData;
};

/*************************************************************************************************************/
/**
###############################################################################################################
##                                     �������
##   �����ΪӦ�ù������������һ����ʵ���������Ҫ����Ӧ�ó���Ĺ�������������һ��������򣬸���Ӧ�ó����
## ������С�ɾ����ǰ̨Ӧ�õļ����ȥ����Լ��������Ϣ���͡�
###############################################################################################################
===============================================================================================================
Function:      IAppMgr * AK_GetAppMgr(T_VOID);
Description:   ��ȡӦ�ù�����ʵ���ӿ�ָ�롣
Parameters:    None
Return:  ����Ӧ�ù�����ʵ���ӿ�ָ�롣
Remark:
    ע: û�����Ӹ�ʵ�������ü������������ͷ�����
===============================================================================================================
===============================================================================================================
Function:      T_AppInfo* AK_GetDefaultAppInfo(T_U16 wAppCls);
Description:   ��ȡָ��Ӧ�ó���Ĭ�ϳ�ʼ����Ϣ��
Parameters:
   wAppCls   [in] Ӧ�ó���ID��
Return:  ����Ӧ�ó���Ĭ�ϳ�ʼ����Ϣ����ָ����Ӧ�ó���IDΪ�Ƿ��ģ��򷵻�AK_NULL��
Remark:
   ���ô˽ӿ�ǰ����ȷ����Ӧ�ó���Ĭ�ϳ�ʼ����Ϣ����ӽ�g_sDefaultAppInfo�
===============================================================================================================
===============================================================================================================
Function:      T_U32 IAppMgr_AddRef(IAppMgr *pIAppMgr);
Description:   ���ü�����1��
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_U32 IAppMgr_Release(IAppMgr *pIAppMgr);
Description:   ���ü�����1��������ü���Ϊ0�����ͷŸ����ʵ����
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAppMgr_AddEntry(IAppMgr *pIAppMgr,T_AppEntry *pAppEntry, T_BOOL bIsActive);
Description:   ��Ӧ�ù��������һ��Ӧ�ü�¼���½���һ��Ӧ�ñ���Ҫ����Ӧ�ù�������Ӧ�ù�������
               �Զ����и�Ӧ�ó���
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   pAppEntry  [in] ָ��Ҫ���Ӧ�ó����¼��ָ�롣
   bIsActive  [in] �Ƿ��õ�ǰ��ӵ�Ӧ�ó�Ϊ����Ӧ�ã��˲���ֻ��ǰ̨Ӧ����Ч��
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    ���һ��Ӧ�����̼�¼��
    T_AppEntry AppEntry;
    IApp *pIApp = AK_NULL;
    T_S32 lRet  = AK_SUCCESS;

    lRet = CMMI_New(&pIApp);
    if (AK_IS_SUCCESS(lRet))
    {
        AppEntry.wAppCls  = AKAPP_CLSID_MMI;
        AppEntry.pIThread = (IThread*)pIApp;
        lRet = IAppMgr_AddEntry(pIAppMgr, &AppEntry, AK_TRUE);
    }    
===============================================================================================================
===============================================================================================================
Function:      T_S32  IAppMgr_DeleteEntry(IAppMgr *pIAppMgr,T_U16 wAppCls);
Description:   ��ָ����Ӧ�ó���������б���ɾ���������ٸ�Ӧ�ó���
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls    [in] Ӧ�ó���ID��
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   ɾ��һ��Ӧ�ó���:
   lRet = IAppMgr_DeleteEntry(pIAppMgr, AKAPP_CLSID_MMI);
===============================================================================================================
===============================================================================================================
Function:      T_S32  IAppMgr_ActiveApp(IAppMgr *pIAppMgr,T_U16 wAppCls);
Description:   ����ָ����ǰ̨Ӧ�ó������ָ������һ����̨Ӧ�ã������κβ�����
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls    [in] Ӧ�ó���ID��
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   ����Ӧ����һ�����Խ��������¼�(���̺ʹ�������)�ͻ�����ǰ̨Ӧ�á�
   ע:Ŀǰ�������ƻ�û�������ơ�
===============================================================================================================
===============================================================================================================
Function:      T_S32  IAppMgr_DeactiveApp(IAppMgr *pIAppMgr,T_U16 wAppCls);
Description:   ȥ����ָ����ǰ̨Ӧ�ó������ָ������һ����̨Ӧ�ã������κβ�����
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls    [in] Ӧ�ó���ID��
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32  IAppMgr_PostEventEx(IAppMgr *pIAppMgr,T_U16 wAppCls,
                                          T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp, 
                                          T_BOOL bIsUnique, T_BOOL bIsHead,
                                          T_BOOL bIsSuspend);
Description:   ��ָ����Ӧ�ö��з���һ����Ϣ�����bIsHeadΪAK_TRUE������Ϣ���ڶ��е�ͷ��������ڶ���
               β�����bIsSuspendΪAK_TRUE����ָ��Ӧ�ö�����ʱ��������Ϣ�����񽫱�����
Parameters:
   pIAppMgr    [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls     [in] Ӧ�ó���ID��
   pMailBox    [in] ָ����Ϣ�ṹ���ָ�롣
   pfnCmp      [in] ��Ϣ�ȽϺ�����ֻ��bIsUniqueΪAK_TRUEʱ��Ч��
   bIsUnique  [in] ��ʶ�Ƿ�Ҫ�ڶ�����Ψһ��
   bIsHead     [in] ��ʶ�Ƿ�Ҫ���ڶ���ͷ��
   bIsSuspend  [in] �Է�Ӧ�ö�����ʱ�����ͷ��Ƿ�Ҫ������
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
   �ýӿں���ֻ���ڸ߼��жϷ�������Ӧ�ó����ﱻ���ã��ҵ����߼��жϷ���������ʱ��bIsSuspend����
   ������ΪAK_FALSE�������ͽ����ɹ���
===============================================================================================================
===============================================================================================================
Function:      T_S32  IAppMgr_PostEvent(IAppMgr *pIAppMgr,T_U16 wAppCls, T_SYS_MAILBOX *pMailBox);
Description:   ��ָ����Ӧ�ö��з���һ����Ϣ��
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls    [in] Ӧ�ó���ID��
   pMailBox   [in] ָ����Ϣ�ṹ���ָ�롣
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
   �ýӿں���ֻ���ڸ߼��жϷ�������Ӧ�ó����ﱻ���ã�����Ϣ������Ӧ�ö���β��
===============================================================================================================
===============================================================================================================
Function:      IThread*  IAppMgr_GetApp(IAppMgr *pIAppMgr,T_U16 wAppCls);
Description:   ��ȡӦ�ó���IDָ����Ӧ�ó���ӿ�ָ�롣
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls    [in] Ӧ�ó���ID��
Return:  ����ָ����Ӧ�ó���ӿ�ָ�룬�����Ӧ�ò����ڻ�û�б��������򷵻�AK_NULL��
Remark:
   IThread* IAppMgr_GetActiveApp(IAppMgr *pIAppMgr);
   DES:���ص�ǰ�����Ӧ�ó���ӿ�ָ�롣
ע: IThreadΪ����Ӧ�ó���ĸ��ӿڡ�
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAppMgr_GetAppCount(IAppMgr *pIAppMgr, T_EAppType eAppType);
Description:   ��ȡָ�����͵�Ӧ�ó��������
Parameters:
   pIAppMgr   [in] ָ������ʵ���ӿڵ�ָ�롣
   eAppType    [in] Ӧ�ó������͡�
Return:  ����ָ�����͵�Ӧ�ó���������������Ĳ��������򷵻�0��
Remark:
===============================================================================================================

*/
/*************************************************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
T_S32 CAppMgr_New(IAppMgr **ppi);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
IAppMgr * AK_GetAppMgr(T_VOID);
T_AppInfo* AK_GetDefaultAppInfo(T_U16 wAppCls);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Everry ext must realize these two function...
#define IAppMgr_AddRef(p)               AK_GETVT(IAppMgr, p)->AddRef(p)
#define IAppMgr_Release(p)              AK_GETVT(IAppMgr, p)->Release(p)
#define IAppMgr_PostEvent(p,c,e)        IAppMgr_PostEventEx(p,c,e,AK_NULL,AK_FALSE,AK_FALSE,AK_FALSE)
#define IAppMgr_GetActiveApp(p)         IAppMgr_GetApp(p,AKAPP_CLSID_ACTIVE)
#define IAppMgr_PostEvt2Head(p,c,e)     IAppMgr_PostEventEx(p,c,e,AK_NULL,AK_FALSE,AK_TRUE,AK_FALSE)
#define IAppMgr_PostUniqueEvt(p,c,e)    IAppMgr_PostEventEx(p,c,e,AK_NULL,AK_TRUE,AK_FALSE,AK_FALSE)
#define IAppMgr_PostUniqueEvt2Head(p,c,e) IAppMgr_PostEventEx(p,c,e,AK_NULL,AK_TRUE,AK_TRUE,AK_FALSE)




__inline IThread* IAppMgr_GetApp(IAppMgr *pIAppMgr,T_U16 wAppCls)
{
    if (AK_NULL == pIAppMgr)
    {
        return AK_NULL;
    }

    return AK_GETVT(IAppMgr, pIAppMgr)->GetApp(pIAppMgr,wAppCls);
}

__inline T_S32 IAppMgr_GetAppCount(IAppMgr *pIAppMgr, T_EAppType eAppType)
{
    if (AK_NULL == pIAppMgr)
    {
        Fwl_Print(C1, M_AKFRAME, "--IAppMgr_GetAppCount failed, AK_NULL == pIAppMgr.");
        return 0;
    }

    return AK_GETVT(IAppMgr, pIAppMgr)->GetAppCount(pIAppMgr,eAppType);
}

__inline T_S32 IAppMgr_PostEventEx(IAppMgr *pIAppMgr,T_U16 wAppCls, 
                                          T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp, 
                                          T_BOOL bIsUnique,T_BOOL bIsHead,T_BOOL bIsSuspend)
{
    if (AK_NULL == pIAppMgr)
    {
        return AK_EBADPARAM;
    }

    return AK_GETVT(IAppMgr, pIAppMgr)->PostEventEx(pIAppMgr,wAppCls,pMailBox,
                                                    pfnCmp,bIsUnique,bIsHead,bIsSuspend);
}

__inline T_S32 IAppMgr_AddEntry(IAppMgr *pIAppMgr,T_AppEntry *pAppEntry, T_BOOL bIsActive)
{
    T_AppEntry *pEntry = AK_NULL;
    T_SYS_MAILBOX  mailbox;

    if (AK_NULL == pIAppMgr)
    {
        return AK_EBADPARAM;
    }

    /*��Ӧ���б�����Ƿ����и�Ӧ�ã�������ڣ��򷵻ش���*/
    if (AK_NULL != IAppMgr_GetApp(pIAppMgr, pAppEntry->wAppCls))
    {
        return AK_EEXISTED;
    }

    /*Ϊ��ǰӦ�ó����¼����ռ䣬��������Ϣ*/
    pEntry = (T_AppEntry*)Fwl_Malloc(sizeof(T_AppEntry));
    if (AK_NULL == pEntry)
    {
         return AK_ENOMEMORY;
    }

    pEntry->wAppCls  = pAppEntry->wAppCls;
    pEntry->pIThread = pAppEntry->pIThread; 

    mailbox.event = APPMGR_EVT_ADDENTRY;
    mailbox.param.w.Param1 = (vUINT32)pEntry;
    mailbox.param.w.Param2 = (vUINT32)bIsActive;

    return AK_Send_To_Queue(IThread_GetQueue((IThread*)pIAppMgr), &mailbox, 
                            sizeof(T_SYS_MAILBOX), AK_NO_SUSPEND);
}


__inline T_S32  IAppMgr_ActiveApp(IAppMgr *pIAppMgr,T_U16 wAppCls)
    
{
    IThread *pIThread = AK_NULL;
    T_SYS_MAILBOX  mailbox;

    if (AK_NULL == pIAppMgr)
    {
        return AK_EBADPARAM;
    }
    
    pIThread = IAppMgr_GetApp(pIAppMgr, wAppCls);
    /*�����Ӧ�ò����ڣ��򷵻ش���*/
    if (AK_NULL == pIThread)
    {
        return AK_ENOTFOUND;
    }
    /*�����Ӧ���Ǻ�̨Ӧ�û��Ѿ�����򷵻ش���*/
    if (IThread_IsBackground(pIThread) || 
        (pIThread == IAppMgr_GetActiveApp(pIAppMgr)))
    {
        return AK_EINVALIDOPT;
    }
    
    mailbox.event = APPMGR_EVT_ACTIVEAPP;
    mailbox.param.w.Param1 = (vUINT32)wAppCls;

    return AK_Send_To_Queue(IThread_GetQueue((IThread*)pIAppMgr), &mailbox, 
                            sizeof(T_SYS_MAILBOX), AK_NO_SUSPEND);
}


__inline T_S32  IAppMgr_DeactiveApp(IAppMgr *pIAppMgr,T_U16 wAppCls)
    
{
    IThread *pIThread = AK_NULL;
    T_SYS_MAILBOX  mailbox;

    if (AK_NULL == pIAppMgr)
    {
        return AK_EBADPARAM;
    }

    pIThread = IAppMgr_GetApp(pIAppMgr, wAppCls);
    /*�����Ӧ�ò����ڣ��򷵻ش���*/
    if (AK_NULL == pIThread)
    {
        return AK_ENOTFOUND;
    }
    /*�����Ӧ���Ǻ�̨Ӧ�û�Ǽ����û������ǰ̨Ӧ�ã��򷵻ش���*/
    if (IThread_IsBackground(pIThread) || 
        (pIThread != IAppMgr_GetActiveApp(pIAppMgr)) ||
        (IAppMgr_GetAppCount(pIAppMgr, AKAPP_FG_APP) <= 1))
    {
        return AK_EINVALIDOPT;
    }
    
    mailbox.event = APPMGR_EVT_DEACTIVEAPP;
    mailbox.param.w.Param1 = (vUINT32)wAppCls;

    return AK_Send_To_Queue(IThread_GetQueue((IThread*)pIAppMgr), &mailbox, 
                            sizeof(T_SYS_MAILBOX), AK_NO_SUSPEND);
}


__inline T_S32  IAppMgr_DeleteEntry(IAppMgr *pIAppMgr,T_U16 wAppCls)
    
{
    IThread *pIThread = AK_NULL;
    T_SYS_MAILBOX  mailbox;

    if (AK_NULL == pIAppMgr)
    {
        return AK_EBADPARAM;
    }

    pIThread = IAppMgr_GetApp(pIAppMgr, wAppCls);
    /*�����Ӧ�ò����ڣ��򷵻ش���*/
    if (AK_NULL == pIThread)
    {
        return AK_ENOTFOUND;
    }
    
    mailbox.event = APPMGR_EVT_DELETEENTRY;
    mailbox.param.w.Param1 = (vUINT32)wAppCls;

    return AK_Send_To_Queue(IThread_GetQueue((IThread*)pIAppMgr), &mailbox,
                            sizeof(T_SYS_MAILBOX), AK_NO_SUSPEND);
}


__inline T_S32  IAppMgr_ReportAppsStatus(IAppMgr *pIAppMgr)
{
    if (AK_NULL == pIAppMgr)
    {
        return AK_EBADPARAM;
    }

    return AK_GETVT(IAppMgr, pIAppMgr)->ReportAppsStatus(pIAppMgr);
}


#endif //__AKAPPMGR_H__

