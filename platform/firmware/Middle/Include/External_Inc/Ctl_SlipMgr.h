/**
 * @file Ctl_SlipMgr.h
 * @brief This header file is for title definition and function prototype
 * @author: songmengxing
 */

#ifndef __CTL_SLIPMGR_H__
#define __CTL_SLIPMGR_H__


#include "akdefine.h"
#include "Gbl_macrodef.h"
#include "Fwl_vme.h"

#define SLIP_ITEM_HEIGHT		(27)

#define SLIP_ITEM_ICON_WIDTH	(SLIP_ITEM_HEIGHT - 2 * X_INTERVAL)
#define SLIP_ITEM_ICON_HEIGHT	(SLIP_ITEM_HEIGHT - 2 * Y_INTERVAL)


#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	ITEM_TYPE_LIST,		// �б�ʽ��list
	ITEM_TYPE_IMAGE,	// ͼƬ���ģ������ͼר��
	ITEM_TYPE_NUM
	
}E_ITEMTYPE;

//iconMode
typedef enum{
	ICON_NONE,				// ��ͼƬ
	ICON_LEFT_ONLY,			// ֻ����ͼƬ
	ICON_RIGHT_ONLY,		// ֻ����ͼƬ
	ICON_ALL,				// ����ͼƬ���

}SLIP_ITEM_ICON_MODE;


//textMode
typedef enum{
	TEXT_NONE, 				// ������
	TEXT_MAIN_ONLY,			// ֻ��������
	TEXT_MAIN_AND_DOWNLINE,	// �����ֺ���������
	TEXT_MAIN_AND_RIGHT,	// �����ֺ��Ҳ�����
	TEXT_ALL,				// �����֡��������ֺ��Ҳ����ֶ���
	
}SLIP_ITEM_TEXT_MODE;

typedef enum{
	SLIPMSG_STA_STOP,		// ֹͣ״̬���û����ն�û���κνӴ�
	SLIPMSG_STA_DOWN,		// ����״̬���û��ֽӴ��ն�ʱ����Ӧdown�¼�
	SLIPMSG_STA_FOCUS,		// �۽�״̬���û��ֽӴ��նˣ�����Ӧ�˵���Ϊ����
	SLIPMSG_STA_MOVE,		// �ƶ�״̬���û��ֽӴ��ն��ƶ�
	SLIPMSG_STA_SLIP		// ����״̬���û���û�нӴ��նˣ��˵����Ի���
	
}E_SLIPMSG_STA;

typedef enum{
	SLIPMSG_NONE,
	SLIPMSG_UP,				// �����ſ���Ϣ
	SLIPMSG_DOWN,			// ����������Ϣ
	SLIPMSG_MOVE,			// �����ƶ���Ϣ
	SLIPMSG_DOWNTIMER,		// down��ʱ����Ϣ
	SLIPMSG_REFRESHTIMER,	// ˢ�¼�ʱ����Ϣ
	SLIPMSG_SLIPOK			// �ƶ������Ϣ
	
}E_SLIPMSG_TYPE;

typedef enum{
	SLIPMSG_UNFOCUS_ITEM,	// unfocused item
	SLIPMSG_FOCUS_ITEM,		// focus item
	SLIPMSG_NOT_ITEM,		// not in item rect
	
}E_FOCUS_FLAG;

typedef  enum {
	SLIPCALC_MOVE_UP,		// �����ƶ�
	SLIPCALC_MOVE_DOWN,		// �����ƶ�
	SLIPCALC_MOVE_LEFT,		// �����ƶ�
	SLIPCALC_MOVE_RIGHT		// �����ƶ�
	
} E_MOVE_DIRECTION;


typedef struct _T_SLIP_TEXT{
	T_U16	*pText;			// ��������
	T_RECT	rect;			// ��������
	T_U32	alignMode;		// ���ֵĶ��뷽ʽ
	T_S32	scrlOffset;		// ����ƫ���ַ���
	T_U32	scrlOkCnt;		// ������������
	
}T_SLIP_TEXT;


typedef struct _T_ICON{
	T_pDATA	pIcon;			// ͼƬ����
	T_RECT	rect;			// ͼƬ����
	T_U32	alignMode;		// ͼƬ�Ķ��뷽ʽ
	
}T_SLIP_ICON;

typedef struct _T_SLIP_ITEM{
	T_S32				id_of_item;		// ��item��Ӧʵ�������е�item�ı�ʶ����Ӧ�þ�����ʲô������
	T_RECT				rect;			// ��item����������ҳbuffer�������
	T_U8				*pbuf;			// ��item��buffer
	E_ITEMTYPE			type;			// item���ͣ����б�ʽ����ͼƬ�������ͼ
	
	//bg
	T_COLOR				bgColor;		// ��item�ǽ���ʱ������ɫ
	T_COLOR				focusBgColor;	// ��item����ʱ������ɫ
	T_pDATA				pBgImg;			// ��item�ǽ���ʱ����ͼƬ
	T_pDATA				pFocusBgImg;	// ��item����ʱ����ͼƬ
	
	//text
	SLIP_ITEM_TEXT_MODE textMode;		// ��item����ģʽ
	T_SLIP_TEXT			*pTextMain;		// ��item�����ֽṹָ��
	T_SLIP_TEXT			*pTextDownLine;	// ��item�������ֽṹָ��
	T_SLIP_TEXT			*pTextRight;	// ��item�Ҳ����ֽṹָ��
	
	//icon
	SLIP_ITEM_ICON_MODE	iconMode;		// ��itemͼƬģʽ
	T_SLIP_ICON			*pIconLeft;		// ��item��ͼƬ�ṹָ��
	T_SLIP_ICON			*pIconRight;	// ��item��ͼƬ�ṹָ��

	struct _T_SLIP_ITEM	*pPrevious;		// ��itemǰһ���ڵ�
    struct _T_SLIP_ITEM	*pNext;			// ��item��һ���ڵ�
    
} T_SLIP_ITEM;

typedef struct _T_ITEM_MGR
{   
	T_U8			*pShow;			// Display Buffer
	T_U32			width;			// Buffer Width
	T_U32			height;			// Buffer Height
	T_U32			itemW;			// Item Width
	T_U32			itemH;			// Item Height
	
	E_ITEMTYPE		itemType;		// item���ͣ����б�ʽ����ͼƬ�������ͼ
	T_U32			itemNumPerRow;	// ÿ�е�item����(����ͼ��)
	T_U32			itemNumPerCol;	// ÿ�е�item����(����ͼ��)
	
	T_pDATA			pBgImg;			// ����ͼƬ
	T_COLOR			bgColor;		// ������ɫ	

	T_U32			itemNum; 		// �ּ���item	
	T_U32			curItem;		// ��ǰ��ʾ�ĵ�һ��item�����itemNum��˵
	
	E_MOVETYPE		moveType; 		// �������ͣ������»��������һ���
	T_U32			loadItemNum;	// ��ǰitem���嵽�ĸ���
	T_U32			totalItemNum;	// ʵ�������item�ܸ���
	T_SLIP_ITEM 	**ppItemhead;	// item������ͷ˫�ص�ָ��
	
	T_S32			overLen;		// Խ���߽�ĳ���
	T_U32			nextRemainLen;	// ��ʣ�೤��
	T_U32			preRemainLen;	// ǰ��ʣ�೤��
	
}T_ITEM_MGR;


/*
 * Slipping Message Management
 */
typedef struct _T_SLIP_MSG
{   
	E_SLIPMSG_STA	lastSta;	// �ϴε�״̬
	E_SLIPMSG_STA	curSta;		// ��ǰ��״̬
	T_TIMER			timer;		// ��Ϣʱ���ʱ��
	E_MOVETYPE		moveType; 	// �������ͣ������»��������һ���
	T_POINT			downPoint;	// down����
	E_FOCUS_FLAG	focusFlag;	// �Ѿ���focus��item
	
}T_SLIP_MSG;


/*
 * Scrollbar Struct
 */
typedef struct _T_SLIP_SCRB
{
	T_RECT		rect;		// ������������
	T_BOOL		visible;	// �������Ƿ�ɼ�
	E_MOVETYPE	moveType;	// ���»����������һ���

	T_U32		maxSize;	// ��󳤶�
	T_U32		size;		// �����С
	T_POS		pos;		// ����λ��
}T_SLIP_SCRB;


/*
 * Slipping Speed Calculating
 */
typedef struct _T_SLIP_CALC
{   
	E_MOVETYPE			moveType;			// �������ͣ������»��������һ���
	E_MOVE_DIRECTION	moveDirection;		// ��������
	
	T_POINT				downPoint;			// touch down������
	T_POINT				movePointOld;		// touch move�ɵ�����
	T_POINT				movePointNew;		// touch move�µ�����
	T_POINT				upPoint;			// touch up������
	
	T_U32				time;				// ʱ��
	float				V0;					// ���ٶ�
	float				Vt;					// ��ǰ�ٶ�
	float				a;					// ���ٶ�
	T_S32				S; 					// ��ǰλ��
	
	T_S32				totalLen;			// �����ص����ܻ���·��
	T_S32				actualLen;			// ��㵽�յ��ʵ��λ��
	T_BOOL				bReboundFlag;		// �ص����
	T_BOOL				bStillFlag;			// ��ֹ���
	
}T_SLIP_CALC;


typedef struct _T_SLIPMGR
{
	T_ITEM_MGR	*pItemMgr;		// �����ģ����
	T_SLIP_MSG	*pMsg;			// ��Ϣ����ģ����
	T_SLIP_CALC	*pCalc;			// �����㷨ģ����
	T_SLIP_SCRB	*pScrb;			// ������ģ����

	T_RECT		rect;			// ��ʾ����
	T_TIMER		refreshTimer;	// ˢ��timer
	E_ITEMTYPE	itemType;		// item���ͣ����б�ʽ����ͼƬ�������ͼ
	E_MOVETYPE	moveType; 		// �������ͣ������»��������һ���
}T_SLIPMGR;

extern T_BOOL	gb_UserkeyValid;

/**
* @brief Creat a slip manager control
*
* @author Songmengxing
* @date 2011-8-23
* @param in E_ITEMTYPE itemtype:ITEM_TYPE_LIST or ITEM_TYPE_IMAGE
* @param in T_RECT rect: rect
* @param in T_U32 itemW: item width
* @param in T_U32 itemH: item height
* @param in T_U32 totalItemNum: total item num
* @param in E_MOVETYPE movetype:MOVETYPE_X or MOVETYPE_Y
* @return T_SLIPMGR * the SlipMgr handle
* @retval
*/
T_SLIPMGR *SlipMgr_Creat(E_ITEMTYPE itemtype, T_RECT rect, T_U32 itemW, T_U32 itemH, T_U32 totalItemNum, E_MOVETYPE movetype);

/**
* @brief Destroy a slip manager control
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @return AK_NULL
* @retval
*/
T_VOID *SlipMgr_Destroy(T_SLIPMGR *pSlipMgr);

/**
* @brief get Item index by id
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_S32 id:the Item id
* @return T_S32
* @retval >=0 : index; <0 : error
*/
T_S32 SlipMgr_GetIndexById(T_SLIPMGR *pSlipMgr, T_S32 id);

/**
* @brief get item num
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @return T_U32
* @retval 
*/
T_U32 SlipMgr_GetItemNum(T_SLIPMGR *pSlipMgr);

/**
* @brief Set Item id, icon and text by index
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_U32 index:the Item index in mgr
* @param in T_S32 id_of_item:the Item id
* @param in const T_pDATA pIconLeft:the left icon data
* @param in const T_pDATA pIconRight:the right icon data
* @param in const T_U16* pTextMain:the main text data
* @param in const T_U16* pTextDown:the down line text data
* @param in const T_U16* pTextRight:the right text data
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_SetItem(T_SLIPMGR *pSlipMgr, T_U32 index, T_S32 id_of_item, 
							T_pCDATA pIconLeft, T_pCDATA pIconRight,
							const T_U16* pTextMain, const T_U16* pTextDown, const T_U16* pTextRight);

/**
* @brief Change Item right text  by id
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_S32 id:the Item id
* @param in const T_U16* pTextRight:the new right text data
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_ChangeTextRightById(T_SLIPMGR *pSlipMgr, T_S32 id, const T_U16* pTextRight);

/**
* @brief show Item by id
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_S32 id:the Item id
* @param in T_BOOL bFocus:it is focus or not
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_ShowItemById(T_SLIPMGR *pSlipMgr, T_S32 id, T_BOOL bFocus);

/**
* @brief scroll show Item by id
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_S32 id:the Item id
* @param in T_BOOL bFocus:it is focus or not
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_ScrollShowItemById(T_SLIPMGR *pSlipMgr, T_S32 id, T_BOOL bFocus);


/**
* @brief add loaded item num
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_S32 count: add count
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_AddLoadItemNum(T_SLIPMGR *pSlipMgr, T_S32 count);


/**
* @brief set total item num
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_U32 num: total num
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_SetTotalItemNum(T_SLIPMGR *pSlipMgr, T_U32 totalnum);

/**
* @brief refresh
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_Refresh(T_SLIPMGR *pSlipMgr);

/**
* @brief handle function
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_EVT_CODE event: event
* @param in T_EVT_PARAM *pEventParm: pEventParm
* @param out  T_S32 *count: count of item need to load
* @param out T_U32 *loadItemNum: loaded item num
* @param in/out T_U32 *focusId: focus item id
* @param in T_U32 emptyNum: empty item num
* @return T_BOOL
* @retval 
*/
T_eBACK_STATE SlipMgr_Handle(T_SLIPMGR *pSlipMgr, T_EVT_CODE event, T_EVT_PARAM *pEventParm, 
									T_S32 *count, T_U32 *loadItemNum, T_U32 *focusId, T_U32 emptyNum);

/**
* @brief check focus item is in show rect or not
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_U32 focusId: focus item id
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_CheckFocusItem(T_SLIPMGR *pSlipMgr, T_U32 focusId);

/**
* @brief prepare to show
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param out  T_S32 *count: count of item need to load
* @param out T_U32 *loadItemNum: loaded item num
* @param in T_S32 offset: offset
* @param in T_U32 emptyNum: empty item num
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_PrepareToShow(T_SLIPMGR *pSlipMgr, T_S32 *count, T_U32 *loadItemNum, T_S32 offset, T_U32 emptyNum);

/**
* @brief clean offset
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_U32 num: ��Ӧʵ��item�����Ѿ������ϵ�item����
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_ClearOffset(T_SLIPMGR *pSlipMgr, T_U32 num);

/**
* @brief set loaded item num
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_U32 num: num
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_SetLoadItemNum(T_SLIPMGR *pSlipMgr, T_U32 num);

/**
* @brief Get Cur Status
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @return E_SLIPMSG_STA
* @retval
*/
E_SLIPMSG_STA SlipMgr_GetCurStatus(T_SLIPMGR *pSlipMgr);

/**
* @brief Set Item id by index
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_U32 index:the Item index in mgr
* @param in T_S32 id_of_item:the Item id
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_SetItemId(T_SLIPMGR *pSlipMgr, T_U32 index, T_S32 id_of_item);

/**
* @brief get item buf by index
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param inT_U32 index: item index
* @param out T_U8 **pbuf: buf
* @param out T_U32 *width: width of buf
* @param out T_U32 *height: height of buf
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_GetItemBufByIndex(T_SLIPMGR *pSlipMgr, T_U32 index, T_U8 **pbuf, T_U32 *width, T_U32 *height);

/**
* @brief set background color
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param in T_COLOR color: color
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_SetBgColor(T_SLIPMGR *pSlipMgr, T_COLOR color);

/**
* @brief get display buf
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr: the SlipMgr handle
* @param out T_U8 **pbuf: buf
* @param out T_U32 *width: width of buf
* @param out T_U32 *height: height of buf
* @return T_BOOL
* @retval 
*/
T_BOOL SlipMgr_GetDisBuf(T_SLIPMGR *pSlipMgr, T_U8 **pbuf, T_U32 *width, T_U32 *height);



/**
* @brief Get Item backgroud img
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr:the SlipMgr handle
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_LoadItemBgImg(T_SLIPMGR *pSlipMgr);

/**
* @brief clean Item focus
*
* @author Songmengxing
* @date 2011-8-23
* @param in T_SLIPMGR *pSlipMgr:the SlipMgr handle
* @return T_BOOL
* @retval
*/
T_BOOL SlipMgr_CleanItemFocus(T_SLIPMGR *pSlipMgr);


#ifdef __cplusplus
}
#endif

#endif
