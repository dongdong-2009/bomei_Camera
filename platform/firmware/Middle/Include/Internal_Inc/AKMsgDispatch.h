/**************************************************************************
*
* Copyrights (C) 2002, ANYKA software Inc
* All rights reserced.
*
* File name: AKMsgDispatch.h
* Function: 
* Author: 
* Date:  
* Version: 1.0
*
***************************************************************************/
#ifndef __AKMSGDISPATCH_H__
#define __AKMSGDISPATCH_H__

#include "AKComponent.h"
#include "AKInterface.h"
#include "AKThread.h"
#include "Fwl_sysevent.h"


/**************************************************************************************************************/
/*######################################################################################*/
/*Msg register type defination...*/
typedef enum
{
   MSG_REG_QUEUE,
   MSG_REG_THREAD
}T_eMsgRegType;

/*######################################################################################*/
/**************************************************************************************************************/

typedef struct IMsgDispatch IMsgDispatch;

/*************************************************************************/
/**
                       <---- DESCRIPTION ---->
#define AK_INHERIT_IMSGDISPATCH(name) \
        AK_INHERIT_IUNKNOWN(name); \
        //Add your function pointer list
        //of virtual table here...
If your extention doest inherit from IUnknown, you can use the Inherit 
macro AK_INHERIT_IYyy instead of AK_INHERIT_IUNKNOWN. For ezample:

#define AK_INHERIT_ICAT(name) \
        AK_INHERIT_IANIMAL(name); \
        ...    ...

*/
/*************************************************************************/
#define AK_INHERIT_IMSGDISPATCH(name) \
      AK_INHERIT_IUNKNOWN(name); \
	  T_S32 (*Activate)(name* p##name); \
	  T_S32 (*Register)(name* p##name, T_HANDLE hRegister, T_eMsgRegType eRegType, T_U32 dwEvtMsk); \
	  T_S32 (*UnRegister)(name* p##name, T_HANDLE hRegister)
	  

AK_INTERFACE(IMsgDispatch)
{
    AK_INHERIT_IMSGDISPATCH(IMsgDispatch);
};

struct IMsgDispatch
{
    AK_DEFINEVT(IMsgDispatch);
    T_VOID *pData;
};

/**************************************************************************************************************/
/**
###############################################################################################################
##                                     �������
##   �����Ϊ��Ϣ�ַ����������һ����ʵ���������Ҫ����ϵͳ��Ϣ�ķַ���������һ���߼��жϷ��񣬸���ַ���Ϣ��
##   ��ͬʱ��Ӧ�ó����ṩע���ȡ��ע��ϵͳ�¼��Ľӿڣ�������ʱ��ص�ע���ȡ��ע��ϵͳ�¼���
###############################################################################################################
===============================================================================================================
Function:      IMsgDispatch *AK_GetIMsgDispatch(T_VOID);
Description:   ��ȡ��Ϣ�ַ�ʵ���ӿ�ָ�롣
Parameters:
   pIMsgDispatch   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
    ����̬���Ϊ��ʵ����������۴������ٴΣ����صĶ���ͬһ���ӿ�ָ�롣
    ע: û�����Ӹ�ʵ�������ü������������ͷ�����
===============================================================================================================
===============================================================================================================
Function:      T_S32 AK_DispatchEvent(T_VOID);
Description:   ����һ����Ϣ�ַ���ִ�иú�����������Ϣ�ַ��߼��жϷ������
Parameters:    None
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_U32 IMsgDispatch_AddRef(IMsgDispatch *pIMsgDispatch);
Description:   ���ü�����1��
Parameters:
   pIMsgDispatch   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_U32 IMsgDispatch_Release(IMsgDispatch *pIMsgDispatch);
Description:   ���ü�����1��������ü���Ϊ0�����ͷŸ����ʵ����
Parameters:
   pIMsgDispatch   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ص�ǰ���ü���ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IMsgDispatch_Activate(IMsgDispatch *pIMsgDispatch);
Description:   ����߼��жϷ���������ɷ�ϵͳ�¼���Ӧ�ö��С�
Parameters:
   pIMsgDispatch   [in] ָ������ʵ���ӿڵ�ָ�롣
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IMsgDispatch_Register(IMsgDispatch *pIMsgDispatch, T_HANDLE hRegister, T_eMsgRegType eRegType, T_U32 dwEvtMsk);
Description:   ����Ҫע��Ķ�����ע�����Ϣ�ַ����У��Խ���ϵͳ��Ϣ��
               ע���Ӧ����Ҫ����Ҫ����ϵͳ�¼������룬���򽫲����յ��κ�ϵͳ�¼���
Parameters:
   pIMsgDispatch   [in] ָ������ʵ���ӿڵ�ָ�롣
   hRegister           [in] ָ��Ҫע��Ķ�������
   eRegType           [in] ָ��Ҫע��Ķ������ͣ�Ŀǰ֧���̺߳Ͷ������֡�
   dwEvtMsk           [in] ָ��Ҫע����¼����룬ֻ������Ϊ����ʱ��Ч��
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
===============================================================================================================
Function:      T_S32 IMsgDispatch_UnRegister(IMsgDispatch *pIMsgDispatch, T_HANDLE hRegister);
Description:   ����Ҫע��Ķ���������Ϣ�ַ�����ȡ��ע�ᣬ��Ӧ�ý������յ�ϵͳ�¼�������
               ���ٴ�ע�ᡣ
Parameters:
   pIMsgDispatch   [in] ָ������ʵ���ӿڵ�ָ�롣
   hRegister           [in] ָ����ע��Ķ�������
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
===============================================================================================================
*/
/**************************************************************************************************************/

/*============================================================================================================*/
IMsgDispatch *AK_GetIMsgDispatch(T_VOID);
T_S32 AK_DispatchEvent(T_VOID);

//============================================================================================================//
T_S32 CMsgDispatch_New(IMsgDispatch **pp);

#define IMsgDispatch_AddRef(p)                  AK_GETVT(IMsgDispatch, p)->AddRef(p)
#define IMsgDispatch_Release(p)                 AK_GETVT(IMsgDispatch, p)->Release(p)
#define IMsgDispatch_Activate(p)                AK_GETVT(IMsgDispatch, p)->Activate((p))
#define IMsgDispatch_Register(p, h, t, m)       AK_GETVT(IMsgDispatch, p)->Register((p), (h), (t), (m))
#define IMsgDispatch_RegisterThread(p, h)       AK_GETVT(IMsgDispatch, p)->Register((p), ((T_HANDLE)h), (MSG_REG_THREAD), (0))
#define IMsgDispatch_RegisterQueue(p, h, m)     AK_GETVT(IMsgDispatch, p)->Register((p), ((T_HANDLE)h), (MSG_REG_QUEUE), (m))
#define IMsgDispatch_UnRegister(p, h)           AK_GETVT(IMsgDispatch, p)->UnRegister((p), ((T_HANDLE)h))

#endif //__AKMSGDISPATCH_H__
