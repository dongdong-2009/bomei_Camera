/**************************************************************************
*
* Copyrights (C) 2002, ANYKA software Inc
* All rights reserced.
*
* File name: AKApp.h
* Function: 
* Author: 
* Date:  
* Version: 1.0
*
***************************************************************************/
#ifndef __AKAPP_H__
#define __AKAPP_H__

#include "AKComponent.h"
#include "AKInterface.h"
#include "AKThread.h"
#include "AKWnd.h"

////////////////////////////////////////////////////////////////////////////////////////////
#define AKAPP_PROP_BASE    (THREAD_PROP_BASE + AKAPP_PROP_OFFSET)

/*Ӧ������ö�ٶ���*/
typedef enum
{
  AKAPP_PROP_NONE = AKAPP_PROP_BASE,
  AKAPP_PROP_INTER_WND
  
}T_eAKAPP_PROPERTY;

/*Ӧ�ó�ʼ����Ϣ����*/
typedef struct 
{
  T_THREAD_INITPARAM  base;  /*�̳߳�ʼ����Ϣ*/
  T_WND_INITPARAM     wnd;   /*�����ʼ����Ϣ*/
}T_APP_INITPARAM;


//==============================================================================
typedef struct IApp IApp;

#define AK_INHERIT_IAPP(name) \
        AK_INHERIT_ITHREAD(name)


AK_INTERFACE(IApp)
{
    AK_INHERIT_IAPP(IApp);
};

//==============================================================================
struct IApp
{
    AK_DEFINEVT(IApp);
    T_VOID *pData;
};

/*************************************************************************************************************/
/**
###############################################################################################################
##                                     �������
##   �����ΪӦ��������Ƕ�ʵ��������̳���IThread�ӿڡ���Ҫ������Ĵ��������ٵȣ�������Ĵ��������ٺ�
## ��Ϣ���еĴ�������Ϣѭ���������Լ��������ö���ί�и����ĸ����̴߳���ġ�
###############################################################################################################
===============================================================================================================
Function:      T_U32 IAkApp_AddRef(IAkApp *pIAkApp);
Description:   ���ü�����1��
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_U32 IAkApp_Release(IAkApp *pIAkApp);
Description:   ���ü�����1��������ü���Ϊ0�����ͷŸ����ʵ����
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_Run(IAkApp *pIAkApp);
Description:   ����ָ��Ӧ�ã����ָ��Ӧ�ô���Terminate״̬�����Ȼ�����Ӧ�ã�Ȼ���������С�
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_Suspend(IAkApp *pIAkApp);
Description:   ����ָ��Ӧ�á�
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_Terminate(IAkApp *pIAkApp);
Description:   ��ָֹ��Ӧ�á�
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
   wAppCls   [in] Ӧ�ó���ID��
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    �����ڵ�ǰӦ������ֹ�Լ���
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_Exit(IAkApp *pIAkApp);
Description:   �˳�ָ��Ӧ�ã�������ֹ��Ӧ�á�ɾ����Ӧ�ã�Ȼ���ͷŸ�Ӧ�ó���ʵ����
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
    �����ڵ�ǰӦ�����˳��Լ���
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_Register(IAkApp *pIAkApp, ICBThread *pIListener);
Description:   ��ָ��Ӧ��ע��һ��Ӧ�ó���ص��ӿڡ�
Parameters:
   pIAkApp    [in] ָ������ʵ���ӿڵ�ָ�롣
   pIListener [in] ָ��Ӧ�ó���ص��ӿڵ�ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   ���Ӧ�ó���ִ�й��캯��ʧ�ܣ�����Ҫȡ��ע�ᣬ��ע��һ����ָ��AK_NULL��
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_SetProperty(IAkApp *pIAkApp, T_U16 wPropID, T_VOID*  pPropData);
Description:   ����ָ��Ӧ�����ԡ�
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
   wPropID   [in] �������ID��
   pPropData [in] ָ�������ڴ���ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   T_S32 IAkApp_SetEvtMsk(IAkApp *pIAkApp, T_U32 ulEvtMsk);
   DES:����ָ��Ӧ�õ��¼����롣
===============================================================================================================
===============================================================================================================
Function:      T_S32 IAkApp_GetProperty(IAkApp *pIAkApp, T_U16 wPropID, T_VOID*  pPropData);
Description:   ��ȡָ��Ӧ�����ԡ�
Parameters:
   pIAkApp   [in] ָ������ʵ���ӿڵ�ָ�롣
   wPropID   [in] �������ID��
   pPropData [in/out] ָ�������ڴ���ָ�롣
Return:  ���ز����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ�����ɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڲ���ʧ�ܵ�ԭ��ֵ��
Remark:
   T_U32 IAkApp_GetEvtMsk(IAkApp *pIAkApp);
   DES:��ȡָ��Ӧ�õ��¼����롣
   T_hTask IAkApp_GetTask(IAkApp *pIAkApp);
   DES:��ȡָ��Ӧ�õ��ڲ�����
   T_hQueue IAkApp_GetQueue(IAkApp *pIAkApp);
   DES:��ȡָ��Ӧ�õ��ڲ����С�
   T_S32 IAkApp_GetState(IAkApp *pIAkApp);
   DES:��ȡָ��Ӧ�õĵ�ǰ״̬��
   IWnd * IAkApp_GetIWnd(IApp *pIApp);
   DES:��ȡָ��Ӧ�õ��ڲ�����ָ�롣
===============================================================================================================
*/
/*************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

T_S32     CApp_New(IApp **ppi, T_APP_INITPARAM *param);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define IAkApp_AddRef(p)                     IThread_AddRef((IThread*)p)
#define IAkApp_Release(p)                    IThread_Release((IThread*)p)
#define IAkApp_Run(p)                        IThread_Run((IThread*)p)
#define IAkApp_Suspend(p)                    IThread_Suspend((IThread*)p)
#define IAkApp_Terminate(p)                  IThread_Terminate((IThread*)p)
#define IAkApp_Exit(p)                       IThread_Exit((IThread*)p)
#define IAkApp_Register(p,l)                 IThread_Register((IThread*)p,l)
#define IAkApp_SetProperty(p,i,v)            IThread_SetProperty((IThread*)p,i,v)
#define IAkApp_GetProperty(p,i,pp)           IThread_GetProperty((IThread*)p,i,pp)
#define IAkApp_SetEvtMsk(p,m)                IThread_SetEvtMsk((IThread*)p,m)
#define IAkApp_GetEvtMsk(p)                  IThread_GetEvtMsk((IThread*)p)
#define IAkApp_GetTask(p)                    IThread_GetTask((IThread*)p)
#define IAkApp_GetQueue(p)                   IThread_GetQueue((IThread*)p)

__inline IWnd * IAkApp_GetIWnd(IApp *pIApp)
{
    IWnd * pIWnd = AK_NULL;

    IAkApp_GetProperty(pIApp, AKAPP_PROP_INTER_WND, (T_VOID*)&pIWnd); 

    return pIWnd;
}

#endif //__AKAPP_H__

