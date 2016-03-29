#ifndef __FWL_SYSEVENT_H__
#define __FWL_SYSEVENT_H__

#include "anyka_types.h"

/*======================================================================================================*/
#define AK_SYS_EVT_BASE    0xffff0000

//////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef T_BOOL (*T_pfnEvtCmp)(const T_VOID *pMailBox1, const T_VOID *pMailBox2);

/*======================================================================================================*/

/*ϵͳ�¼����붨��*/
enum
{
    SYS_EVT_MSK_NONE     = 0x00000000,
    SYS_EVT_COMM_MSK     = 0x00000001,
    SYS_EVT_APP_MSK      = 0x00000002,
    SYS_EVT_INPUT_MSK    = 0x00000004, 
    SYS_EVT_VATC_MSK     = 0x00000008, 
    SYS_EVT_AUDIO_MSK    = 0x00000010, 
    SYS_EVT_MMI_MSK      = 0x00000020,    
    SYS_EVT_BT_MSK       = 0x00000040,
    SYS_EVT_XXX_MSK      = 0x00000080
};

/*ϵͳ�¼�ö��*/
typedef enum 
{
    /* ͨ��ϵͳ�¼�*/
    SYS_EVT_COMM_MSK_BEGIN   = AK_SYS_EVT_BASE,
    SYS_EVT_TIMER,
    SYS_EVT_SUBTHREAD_NOTIFY,
    SYS_EVT_SUBTHREAD_FINISH,
    SYS_EVT_COMM_MSK_END,
    //=============================================
    /* Ӧ��ϵͳ�¼�*/
    SYS_EVT_APP_MSK_BEGIN    = SYS_EVT_COMM_MSK_END,
    SYS_EVT_APP_START,
    SYS_EVT_APP_ACTIVE,
    SYS_EVT_APP_DEACTIVE,
    SYS_EVT_APP_STOP,
    SYS_EVT_APP_MSK_END,
    //=============================================
    /* ����ϵͳ�¼�*/
    SYS_EVT_INPUT_MSK_BEGIN  = SYS_EVT_APP_MSK_END,
    SYS_EVT_TSCR,
    SYS_EVT_KEYDOWN,
    SYS_EVT_KEYPRESS,
    SYS_EVT_KEYUP,        
    SYS_EVT_INPUT_MSK_END,
    //=============================================
    /* ����ϵͳ�¼�*/
    SYS_EVT_VATC_MSK_BEGIN   = SYS_EVT_INPUT_MSK_END,
    SYS_EVT_VATC_MSK_END,
    //=============================================
    /* ��Ƶϵͳ�¼�*/
    SYS_EVT_AUDIO_MSK_BEGIN  = SYS_EVT_VATC_MSK_END,
    SYS_EVT_MEDIA,
    SYS_EVT_SDCB_MESSAGE,
    SYS_EVT_AUDIO_ABPLAY,
    SYS_EVT_AUDIO_MSK_END,
        
    //=============================================
    /* MMIϵͳ�¼�*/
    SYS_EVT_MMI_MSK_BEGIN  = SYS_EVT_AUDIO_MSK_END,
    SYS_EVT_CTL_KILLFOCUS,
    SYS_EVT_Z99COM_SMS_ME,
    SYS_EVT_Z99COM_CDS_ME,
    SYS_EVT_Z98COM_SMS_SIM,
    SYS_EVT_Z04COM_SMS_CLASS0,
    SYS_EVT_Z05COM_MSG,
    SYS_EVT_PUB_TIMER,
    SYS_EVT_GPS_DATA_READY,
    SYS_EVT_PINIO,
	SYS_EVT_SD_PLUG,
	SYS_EVT_SDIO_PLUG,
	SYS_EVT_SDMMC_PLUG,
	SYS_EVT_USB_DETECT,
    SYS_EVT_USB_PLUG,
    SYS_EVT_USB_SEND_REQUEST,
    SYS_EVT_V24_DATA,
    SYS_EVT_USER_KEY,
    SYS_EVT_Z10COM_KEY_UNLOCK_HINT,
    SYS_EVT_Z99COM_STK,
    SYS_EVT_AUDIO_RESUME_PLAY,
    SYS_EVT_Z11CHARGER_GPIO,
    SYS_EVT_RTC,
    SYS_EVT_PRE_EXIT,
    SYS_EVT_PASTE_EXIT,
    SYS_EVT_MMI_MSK_END,
   
    //=============================================
    /*BTϵͳ�¼� */
    SYS_EVT_BT_MSK_BEGIN = SYS_EVT_MMI_MSK_END, 
    SYS_EVT_BLUETOOTH,
    SYS_EVT_BT_MSK_END,
    //=============================================
    /* ��չϵͳ�¼�*/  
    SYS_EVT_XXX_MSK_BEGIN = SYS_EVT_BT_MSK_END,    
    SYS_EVT_XXX_MSK_END
} T_SYS_EVENTID;
/*Ϊ�˼�����ǰ��MMI��Ŀǰ�����ϵͳ�¼����Ǻܺ����Ժ���Ҫ�Ż���*/
/*====================================================================================================*/

/*�¼���������*/
typedef union                                         
{  
    struct                                             
    {                                                
        T_U8 Param1;                                       
        T_U8 Param2;                                      
        T_U8 Param3;                                      
        T_U8 Param4;                                      
        T_U8 Param5;                                      
        T_U8 Param6;                                       
        T_U8 Param7;                                       
        T_U8 Param8;                                     
    } c;                                               
                                         
    struct                                              
    {                                                  
        T_U16 Param1;                         
        T_U16 Param2;                         
        T_U16 Param3;                         
        T_U16 Param4;                         
    } s;                                               

    struct                                              
    {                                                
        T_U32 Param1;                               
        T_U32 Param2;                               
    } w;      
    T_pVOID     lpParam;                                      
    T_U32       lParam; 
} T_SYS_PARAM;    
/*****************************************************************************************************
ע:���¼���������Ϊ�����壬Ҫע��������������Ǻ������ṹ��һ����
����һ��ռ�ģ���T_SYS_PARAM��СΪ8�ֽڡ�
******************************************************************************************************/

/*�¼���ʶ����*/
typedef T_U32  T_SYS_EVTID;

/*��Ϣ�����嶨��*/
typedef struct
{
    /*Ϊ�˼�MMI�������������ǰ��һ�µģ���Ҫ��ֵ*/
	T_SYS_EVTID event;   /*�¼���ʶ*/
	T_SYS_PARAM param;   /*�¼�����*/
    /*##############################################*/
    /*�������ڲ�ʹ�õ����ݱ������û�����Ҫ��ֵ��*/
    T_BOOL      bIsUnique;
    T_BOOL      bIsHead;
    T_pfnEvtCmp fnCmp;
    /*##############################################*/
}T_SYS_MAILBOX;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************************/
/**
===================================================================================================================
Function:      T_S32 AK_PostEventEx(T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp, 
                                    T_BOOL bIsUnique, T_BOOL bIsHead,
                                    T_BOOL bIsTerminalInput);
Description:   ����ϵͳ�¼���ϵͳ���У�������߼��жϷ���������ɷ�ϵͳ�¼���Ӧ�ö��С�
Parameters:
   pMailBox   [in] ָ��Ҫ������Ϣ��ָ�롣
   pfnCmp     [in] ��Ϣ�ȽϺ������ڷ���Ψһ��Ϣʱ���õ�������������ԡ�
   bIsUnique  [in] ��ʶ���͵���Ϣ�Ƿ��ڶ�����ҪΨһ��
   bIsHead    [in] ��ʶ���͵���Ϣ�Ƿ�Ҫ���ڶ��е�ͷ��
   bIsTerminalInput [in] ��ʶ���͵���Ϣ�������¼����Ƿ������¼���
Return:  ���ط��Ͳ����Ƿ�ɹ������ΪAK_SUCCESS�����ʾ���ͳɹ�������ֵ��ʾ����ʧ�ܣ���
         ��Ӧ�ڷ���ʧ�ܵ�ԭ��ֵ��
Remark:
   AK_PostEvent(T_SYS_MAILBOX *pMailBox);
   DES: ����һ���������¼�������β��
   AK_PostEventToHead(T_SYS_MAILBOX *pMailBox);
   DES: ����һ���������¼�������ͷ��
   AK_PostUniqueEvent(T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp);
   DES: ����һ���������¼�������β������������и��¼��������¼���
   AK_PostUniqueEventToHead(T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp);
   DES: ����һ���������¼�������ͷ������������и��¼��������¼���
   AK_PostTerminalInputEvent(T_SYS_MAILBOX *pMailBox);
   DES: ����һ�������¼�������β��
   AK_PostTerminalInputEventEx(T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp, 
                               T_BOOL bIsUnique, T_BOOL bIsHead);
   DES: ����һ�������¼������У�ͨ������bIsUnique��bIsHead�������¼��ڶ�����Ψһ�ͷ���
        ����ͷ��
====================================================================================================================
*/
/******************************************************************************************************************/

T_S32 AK_PostEventEx(T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp, 
                     T_BOOL bIsUnique, T_BOOL bIsHead,
                     T_BOOL bIsTerminalInput);

#define AK_PostEvent(m)                  AK_PostEventEx(m, AK_NULL, AK_FALSE, AK_FALSE, AK_FALSE)
#define AK_PostEventToHead(m)            AK_PostEventEx(m, AK_NULL, AK_FALSE, AK_TRUE, AK_FALSE)
#define AK_PostUniqueEvent(m,cmp)        AK_PostEventEx(m, cmp, AK_TRUE, AK_FALSE, AK_FALSE)
#define AK_PostUniqueEventToHead(m,cmp)  AK_PostEventEx(m, cmp, AK_TRUE, AK_TRUE, AK_FALSE)
/*===============================================================================*/
//T_S32 AK_PostTerminalInputEventEx(T_SYS_MAILBOX *pMailBox, T_pfnEvtCmp pfnCmp, T_BOOL bIsUnique, T_BOOL bIsHead);
#define AK_PostTerminalInputEventEx(m,cmp,u,h)   AK_PostEventEx(m, cmp, u, h, AK_TRUE)
#define AK_PostTerminalInputEvent(m)             AK_PostEventEx(m, AK_NULL, AK_FALSE, AK_FALSE, AK_TRUE)

#endif //__FWL_SYSEVENT_H__

