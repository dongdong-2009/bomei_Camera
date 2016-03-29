#ifndef __AKOS_API_H__
#define __AKOS_API_H__

#include "akdefine.h"

#include "AKError.h"

#if 0
typedef T_S32				T_hTask;
typedef T_S32				T_hQueue;
typedef T_S32				T_hSemaphore;
typedef T_S32				T_hHisr;
typedef T_U8				T_OPTION;
typedef T_S32				T_hMailbox;
typedef T_S32				T_hEventGroup;
typedef T_S32				T_hTimer;

/* Define constants for use in service parameters.  */

#define         AK_AND                          2
#define         AK_AND_CONSUME                  3
#define         AK_DISABLE_TIMER                4
#define         AK_ENABLE_TIMER                 5
#define         AK_FIFO                         6
#define         AK_FIXED_SIZE                   7
#define         AK_NO_PREEMPT                   8
#define         AK_NO_START                     9
#define         AK_NO_SUSPEND                   0
#define         AK_OR                           0
#define         AK_OR_CONSUME                   1
#define         AK_PREEMPT                      10
#define         AK_PRIORITY                     11
#define         AK_START                        12
#define         AK_SUSPEND                      0xFFFFFFFFUL
#define         AK_VARIABLE_SIZE                13

/* Define task suspension constants.  */

#define         AK_DRIVER_SUSPEND               10
#define         AK_EVENT_SUSPEND                7
#define         AK_FINISHED                     11
#define         AK_MAILBOX_SUSPEND              3
#define         AK_MEMORY_SUSPEND               9
#define         AK_PARTITION_SUSPEND            8
#define         AK_PIPE_SUSPEND                 5
#define         AK_PURE_SUSPEND                 1
#define         AK_QUEUE_SUSPEND                4
#define         AK_READY                        0
#define         AK_SEMAPHORE_SUSPEND            6
#define         AK_SLEEP_SUSPEND                2
#define         AK_TERMINATED                   12

/* Define task status. */

#define         AK_DRIVER_SUSPEND               10
#define         AK_EVENT_SUSPEND                7
#define         AK_FINISHED                     11
#define         AK_MAILBOX_SUSPEND              3
#define         AK_MEMORY_SUSPEND               9
#define         AK_PARTITION_SUSPEND            8
#define         AK_PIPE_SUSPEND                 5
#define         AK_PURE_SUSPEND                 1
#define         AK_QUEUE_SUSPEND                4
#define         AK_READY                        0
#define         AK_SEMAPHORE_SUSPEND            6
#define         AK_SLEEP_SUSPEND                2
#define         AK_TERMINATED                   12
#endif
typedef T_BOOL (*CallbakCompare)(const T_VOID *evtParm1, const T_VOID *evtParm2);

// Task Control API


/*@brief: ������񴴽�һ��Ӧ�ó�������
  *
  *@param task entry 		[in]ָ�����������
  *@param name 				[in]�������ַ���ָ�룬�ֻ��8�ֽ�
  *@param argc				[in]T_U32�������ͣ������������ݳ�ʼ����Ϣ������
  *@param argv				[in]ָ�룬���ݳ�ʼ����Ϣ������
  *@param stack_address		[in]���������ջ������ʼ�ڴ��ַλ��
  *@param stack_size      	[in]ָ����ջ���ֽ���
  *@param priority          [in]��0��255֮��ָ�����ȼ�ֵ����ֵԽ�ͣ����񼶱�Խ�ߡ�
  *@param time_slice		[in]��ʾ��ֹ��������Ķ�ʱ���������ֵ��0ֵ��ʾ��ֹ����ʱ��Ƭ��
  *@param preempt       	[in]�����õ���Ч����ΪAK_PREEMPT��AK_NO_PREEMPT��  AK_PREEMPT��ʾ����ռ����Ч��AK_NO_PREEMPT��ʾ����ռ����Ч��  ע���������ռ����Ч��ʱ��Ƭ��ֹ
  *@param auto_start     	[in]  ��������Ч����Ϊ��AK_NO_START��AK_START��AK_START��ʾ�����񴴽����������õ�����״̬��AK_NO_START  ��ʾ�����ڴ���֮��������״̬��  ����ΪAK_NO_START�������Ժ����ָ���
  *
  *@return T_hTask: Tash handle
  *@����������ش���ֵ��С��0
  *@AK_MEMORY_CORRUPT	��ʾ�����ڴ�ռ�ʧ��
  *@AK_INVALID_ENTRY	��ʾ��ں���ָ��Ϊ��
  *@AK_INVALID_MEMORY  	��ʾstack_addressָ�����ڴ���Ϊ��
  *@AK_INVALID_SIZE 		��ʾָ���Ķ�ջ�ߴ粻����
  *@AK_INVALID_PRIORITY 	��ʾָ�������ȼ���Ч
  *@AK_INVALID_PREEMPT  	��ʾռ�Ȳ�����Ч���������������ͬ��ռ������һ��ʱ��Ƭ��ָ��
  *@AK_INVALID_START	��ʾauto_start������Ч
  */
T_hTask AK_Create_Task(T_VOID *task_entry, T_U8 *name, T_U32 argc, T_VOID *argv,
                        	T_VOID *stack_address, T_U32 stack_size, T_OPTION priority, 
                        	T_U32 time_slice, T_OPTION preempt, T_OPTION auto_start);


/*@brief: �г���ǰ����AKOS�����״̬
  *@patam T_VOID
  *@return T_S32
  *@retval T_VOID
  */
T_VOID AK_List_Task(T_VOID);

/*@brief:�˷���ɾ��һ����ǰ��������񡣲���Taskȷ����Ҫɾ������������������Ϲ��������ָ�ʱ�����ʵ��Ĵ���״̬��
  *@patam task	[in]������ƾ��
  *@return T_S32
  *@retval AK_SUCCESS  		��ʾ����ɹ�ɾ��
  *@retval AK_INVALID_TASK 	��ʾ�������Ƿ�
  *@retval AK_INVALID_DELETE  	������һ��δ��ɻ�δ��ֹ״̬
  */
T_S32 AK_Delete_Task(T_hTask task);


/*
  *@brief:�����������������taskָ��ָ����������������Ѿ����ڹ���״̬����ʹ��������¹����������ʧ���˷���ȷ��Ȼ���������ڹ���״̬��AK_Resume_Task���ڻָ����ַ�ʽ���������
  *@param  Task	  [in]  ������ƾ��
  *@retval AK_SUCCESS  	��ʾ����ɹ����
  *@retval AK_INVALID_TASK ��ʾ�������Ƿ�  
  */
T_S32 AK_Suspend_Task(T_hTask task);


/************************************************************************/
 /*@brief:�������ָ�һ����ǰͨ��AK_Suspend_Task������������		
  *@param  Task	  [in]  ������ƾ��
  *@tetval AK_SUCCESS  ��ʾ����ɹ����
  *@retval AK_INVALID_TASK ��ʾ��������Ч
  *@retval AK_INVALID_RESUME   ��ʾָ�������񲻴�������������״̬��            */          
/************************************************************************/
T_S32 AK_Resume_Task(T_hTask task);

/************************************************************************/
/*@brief:���������ֹtask����ָ��������
 *@param  Task	  [in]  ������ƾ��
 *@retval AK_SUCCESS  ��ʾ����ɹ����
 *@retval AK_INVALID_TASK ��ʾ��������Ч */                               
/************************************************************************/
T_S32 AK_Terminate_Task(T_hTask task);

/************************************************************************/
/* @brief:�������������ڵ���������ָ���Ķ�ʱ����������				*/
/* @param Tick	[in]	����Ľ�����									*/
/* @return T_VOID														*/
/************************************************************************/
T_VOID AK_Sleep(T_U32 ticks);


/************************************************************************/
/* @brief:�������CPU��λ������ͬһ���ȼ���������                   */
/************************************************************************/
T_VOID AK_Relinquish(T_VOID);

/************************************************************************/
/* @biref:�������ı�ָ����������ȼ�Ϊ���������ȼ���ֵ��				*/
/*		  ���ȼ�Ϊ��0��255��Χ����ֵ������ԽС�������ȼ�Խ�ߡ�          */
/* @param Task	[in]������											*/
/* @param new_priority	[in]�µ����ȼ�									*/
/* return:������񷵻ص��ó�����ǰ�����ȼ���							*/
/************************************************************************/
T_OPTION AK_Change_Priority(T_hTask task, T_OPTION new_priority);

/************************************************************************/
/* @biref:������񷵻ص�ǰ���еĺ���ָ�룬Ҳ���ǵ����ߵĺ���ָ�롣		*/
/* @param T_VOID														*/
/* return:������񷵻ص�ǰ�����ָ��									*/
/************************************************************************/
T_hTask AK_GetCurrent_Task(T_VOID);

/************************************************************************/
/* @brief:�������λ��ǰ����ֹ�����������							*/
/*																		*/
/* @param:task	[in]������											*/
/* @param:argc	[in]һ��T_U32�������ͣ���������������Ϣ������			*/
/* @argv		[in]һ��ָ�룬��������������Ϣ������					*/
/*																		*/
/* retval	AK_SUCCESS			��ʾ����ɹ�����						*/
/* retval	AK_INVALID_TASK		��ʾ����ָ����Ч						*/
/* retval	AK_NOT_TERMINATED	��ʾָ���������Ǵ�����ֹ�����״̬��	*/
/*								ֻ�д�����ֹ�����״̬��������Ա���λ��*/
/*																		*/
/************************************************************************/
T_S32 AK_Reset_Task(T_hTask task, T_U32 argc, T_VOID *argv);

/************************************************************************/
/* @brief:��������ѯ��ǰ�����״̬                                    */
/* @param:T_hTask	[in]����ľ��										*/
/*																		*/
/* retval	AK_INVALID_TASK		������������������ķ���ֵ������������*/
/*								״̬��������							*/
/* retval	AK_READY			���������л����״̬�������������ȼ���*/
/*								ԭ��û��ִ��							*/
/* retval	AK_FINISHED			���������״̬						*/
/* retval	AK_TERMINATED		�����ڱ���ֹ״̬						*/
/* retval	AK_TASK_SUSPEND		�����ڹ���״̬������suspend�������� */
/*								������û�����С�sleep�Թ�����������״̬��*/
/* retval	AK_TASK_WAITING		�����ڵȴ������У����С��ź������¼�����*/
/*								����������״̬��						*/
/************************************************************************/
T_S32 AK_Task_Status(T_hTask task);

/************************************************************************/
/* @brief:	��������鵱ǰ���еĸ�����Ķ�ջʹ��������������ջ�����*/
/*			�򷵻�0���������ӡ��Ϣ�����򷵻�1                          */
/* @param:	T_VOID														*/
/* retval	T_U32����brief												*/
/************************************************************************/
T_U32 AK_Check_Task_Stack(T_VOID);


// Queue Control API
/************************************************************************/
/* @brief:	�˷��񴴽�һ����Ϣ���С����д���֧�ֶ����ͱ䳤��Ϣ�Ĺ���	*/
/*			������Ϣ�ɶ��T_U8����Ԫ����ɡ�                            */
/* @param:	start_address	[in]ָ�����е���ʼ��ַ��					*/
/* @param:	queue_size		[in]ָ��������T_U8����Ԫ�ص�����			*/
/* @param:	message_type	[in]ָ�����й������Ϣ���͡�
								AK_FIXED_SIZE��ʾ���й�������Ϣ��		
								AK_VARIABLE_SIZE��ʾ���й���䳤�ߴ���Ϣ*/
/* @param:	message_size	[in]�������֧�ֶ�����Ϣ���������ָ��ÿ����
								Ϣ�ľ�ȷ���ȡ��������֧�ֱ䳤��Ϣ�����
								������ʾ�����Ϣ�ߴ硣					*/
/* @param:	suspend_type	[in]ָ�����й������͡��˲�����Ч����
								ΪAK_FIFO��AK_PRIORITY��
								�ֱ��ʾ�����ȳ������ȼ�˳�����		*/
/* retval	T_hQueue	����ɹ���ɣ����ض��о��						*/
/* ���������������С��0�Ĵ���ֵ*/
/* retval	AK_MEMORY_CORRUPT	�����ڴ�ռ�ʧ��						*/
/* retval	AK_INVALID_MESSAGE	��ʾ��Ϣ���Ͳ�����Ч					*/
/* retval	AK_INVALID_SIZE	��ʾָ������Ϣ�ߴ���ڶ��гߴ��Ϊ0			*/
/* retval	AK_INVALID_SUSPEND	��ʾ�������Ͳ�����Ч					*/
/* retval	AK_INVALID_SUSPEND	��ʾ�������Ͳ�����Ч					*/
/************************************************************************/
T_hQueue AK_Create_Queue(T_VOID *start_address, T_U32 queue_size, 
                      T_OPTION message_type, T_U32 message_size, T_OPTION suspend_type);

/************************************************************************/
/* @brief:	�˷���ɾ��һ����ǰ�������Ϣ���С�����Queueȷ����Ҫɾ����	*/
/*			��Ϣ���С�����������Ϲ��������ָ�ʱ����һ������ֵ��		*/
/*			��ɾ���ڼ��֮��Ӧ�ó�������ֹ���е�ʹ�á�              */
/* @param:	suspend_type	[in]���еľ��								*/
/* retval	AK_SUCCESS	��ʾ�ɹ�ɾ������
			AK_INVALID_QUEUE	��ʾ���зǷ�							*/
/************************************************************************/
T_S32 AK_Delete_Queue(T_hQueue queue);

/************************************************************************/
/* @brief:	������������Ϣ��ָ���Ķ��е׶ˡ�����������㹻�Ŀռ䱣����Ϣ��
			���������������
			���ݶ���֧�ֵ���Ϣ���ͣ�������Ϣ����������䳤T_U8����������*/
/* @param:	Queue	[in]���еľ��
			message	[in]������Ϣָ��
			size	[in]ָ����Ϣ��T_U8������Ŀ
						�������֧�ֱ䳤��Ϣ���������������ڻ�С�ڶ���֧�ֵ���Ϣ�ߴ硣
						�������֧�ֶ�����Ϣ���˲����������õ��ڶ���֧�ֵ���Ϣ�ߴ硣
			Suspend	[in]��������Ѿ�װ������Ϣ��ָ���Ƿ��������������й�����������Ч��
						AK_NO_SUSPEND���������Ƿ����㣬�����������ء�
						ע���������ӷ������̵߳��ã�����Ψһ��Ч�����á���������
						AK_SUSPEND	�����������ֱ�����пռ���Ч��
						ʱ����ֵ��1 �� 4294967293��
						�����������ֱ��һ��������Ϣ���ã�����ֱ��ָ��������ʱ�ӽ��ĵ�ʱ��*/


/*	retval	AK_SUCCESS			��ʾ����ɹ�
	retval	AK_INVALID_QUEUE	��ʾ���зǷ�
	retval	AK_INVALID_POINTER	��ʾ��Ϣָ��Ϊ��
	retval	AK_INVALID_SIZE		��ʾsize������ͬ�ڶ���֧�ֵ���Ϣ�ߴ硣ֻ�����ڶ��嶨���ֽڵĶ��С�
	retval	AK_INVALID_SUSPEND	��ʾ��ͼ�ӷ������̹߳���
	retval	AK_QUEUE_FULL		��ʾ����Ϊ��
	retval	AK_TIMEOUT			��ʾ�ڹ���ָ����ʱֵ֮�󣬶�����ȻΪ��
	retval	AK_QUEUE_DELETE		����������ڼ���б�ɾ��				*/
/*ע������	������жϵȷ������߳��е��ô˽ӿڣ�����������ò���AK_NO_SUSPEND�����۶����Ƿ�Ϊ�������᷵�ش��� AK_INVALID_SUSPEND*/
/************************************************************************/
T_S32 AK_Send_To_Queue(T_hQueue queue, T_VOID *message, T_U32 size, T_U32 suspend);

/************************************************************************/
/* @brief:	������������Ϣ��ָ���Ķ���ǰ�ˡ�����������㹻�Ŀռ䱣����Ϣ��
			���������������
			���ݶ���֧�ֵ���Ϣ���ͣ�������Ϣ�ɶ����򲻶����ֽ���ɡ�*/

/* @param:	Queue	[in]���еľ��
			message	[in]������Ϣָ��
			size	[in]ָ����Ϣ��T_U8������Ŀ
						�������֧�ֱ䳤��Ϣ���������������ڻ�С�ڶ���֧�ֵ���Ϣ�ߴ硣
						�������֧�ֶ�����Ϣ���˲����������õ��ڶ���֧�ֵ���Ϣ�ߴ硣
			Suspend	[in]��������Ѿ�װ������Ϣ��ָ���Ƿ��������������й�����������Ч��
						AK_NO_SUSPEND���������Ƿ����㣬�����������ء�
						ע���������ӷ������̵߳��ã�����Ψһ��Ч�����á���������
						AK_SUSPEND	�����������ֱ�����пռ���Ч��
						ʱ����ֵ��1 �� 4294967293��
						�����������ֱ��һ��������Ϣ���ã�����ֱ��ָ��������ʱ�ӽ��ĵ�ʱ��*/


/*	retval	AK_SUCCESS			��ʾ����ɹ�
	retval	AK_INVALID_QUEUE	��ʾ���зǷ�
	retval	AK_INVALID_POINTER	��ʾ��Ϣָ��Ϊ��
	retval	AK_INVALID_SIZE		��ʾsize������ͬ�ڶ���֧�ֵ���Ϣ�ߴ硣ֻ�����ڶ��嶨���ֽڵĶ��С�
	retval	AK_INVALID_SUSPEND	��ʾ��ͼ�ӷ������̹߳���
	retval	AK_QUEUE_FULL		��ʾ����Ϊ��
	retval	AK_TIMEOUT			��ʾ�ڹ���ָ����ʱֵ֮�󣬶�����ȻΪ��
	retval	AK_QUEUE_DELETE		����������ڼ���б�ɾ��				*/

/*ע������	������жϵȷ������߳��е��ô˽ӿڣ�����������ò���AK_NO_SUSPEND�����۶����Ƿ�Ϊ�������᷵�ش��� AK_INVALID_SUSPEND*/
/************************************************************************/
T_S32 AK_Send_To_Front_of_Queue(T_hQueue queue, T_VOID *message, T_U32 size, T_U32 suspend);

/************************************************************************/
/* @brief:	�������㲥һ����Ϣ����ָ���Ķ��еȴ���Ϣ����������
			���û�������ڵȴ�����Ϣֻ�Ƿŵ�����ĩ�ˡ�
			���ݶ��еĴ�����������Ϣ�ɶ����򲻶����ֽ���ɡ�
   @param:	Queue	���еľ��
			message	������Ϣָ��
			size	ָ����Ϣ��T_U8������Ŀ
					�������֧�ֱ䳤��Ϣ���������������ڻ�С�ڶ���֧�ֵ���Ϣ�ߴ硣
					�������֧�ֶ�����Ϣ���˲����������õ��ڶ���֧�ֵ���Ϣ�ߴ硣
			Suspend	��������Ѿ�������һ����Ϣ��ָ���Ƿ�����������
					AK_NO_SUSPEND
						���������Ƿ���������������ء�
					AK_SUSPEND
						�����������ֱ����Ϣ����������С�
					ʱ����ֵ��1 �� 4294967293��
						�����������ֱ����Ϣ��������л���ֱ��ָ���Ķ�ʱ�����ĵ�ʱ��

����ֵ˵��	
	AK_SUCCESS			��ʾ����ɹ�
	AK_INVALID_QUEUE	��ʾ������Ч
	AK_INVALID_POINTER	��ʾ��Ϣָ��ΪAK_NULL
	AK_INVALID_SIZE		��ʾ��Ϣ�ߴ������֧�ֵ���Ϣ�ߴ粻ƥ��
	AK_INVALID_SUSPEND	��ʾ��ͼ�ӷ������̹߳���
	AK_QUEUE_FULL		��ʾ������
	AK_TIMEOUT			��ʾ��ʹ�ڹ���ʱ֮�󣬶���״̬��ȻΪ��
	AK_QUEUE_DELETE		�����ڹ����ڼ���б�ɾ��

ע������	������жϵȷ������߳��е��ô˽ӿڣ�����������ò���AK_NO_SUSPEND��
			���۶����Ƿ�Ϊ�������᷵�ش��� AK_INVALID_SUSPEND			*/
/************************************************************************/
T_S32 AK_Broadcast_To_Queue(T_hQueue queue, T_VOID *message, T_U32 size, T_U32 suspend);

/************************************************************************/
/* @brief:	��������ָ���Ķ����н���һ����Ϣ��������а���һ��������Ϣ��
			�����Ӷ������Ƴ�ǰ�����Ϣ�ҿ�����ָ����λ�á�				*/
/* @param:	Queue	[in]���еľ��										*/
/*			message	[in]��ϢĿ��ָ�롣ע����ϢĿ������㹻��������size���ֽ�����*/
/*			size	[in]ָ����Ϣ�ڵ�T_U8������������
						���ֵ�����Ӧ���ڶ��д���ʱ�������Ϣ�ߴ硣
						ֻ�����ڶ��嶨���ֽڵĶ��У�������ԡ�			*/
/*			actual_size	[out]ָ�򱣴������Ϣʵ��T_U8�����������ı�����ָ�롣	
						ע�⣺������ָ�룬����������һ��size��������	*/
/*			Suspend	[in]�������Ϊ�գ�ָ���Ƿ�������ڵ��õ�����
						�����ǹ������͵���Чѡ�
						AK_NO_SUSPEND
						���������Ƿ����㣬�����������ء�
						ע��������񱻷������̵߳��ã�����Ψһ��Ч�����á�����������
						AK_SUSPEND
						���ڵ��õ��������ֱ��������Ϣ������
						ʱ����ֵ��1~4294967293��
						���ڵ��õķ������ֱ����Ϣ��Ч��ֱ��ָ���Ķ�ʱ����������ʱ*/

/*	retval	AK_SUCCESS			��ʾ����ɹ�
	retval	AK_INVALID_QUEUE	��ʾ���зǷ�
	retval	AK_INVALID_POINTER	��ʾ��Ϣָ��Ϊ�ջ���actual_sizeָ��Ϊ��
	retval	AK_INVALID_SIZE		��ʾsize������ͬ�ڶ���֧�ֵ���Ϣ�ߴ硣ֻ�����ڶ��嶨���ֽڵĶ��С�
	retval	AK_INVALID_SUSPEND	��ʾ��ͼ�ӷ������̹߳���
	retval	AK_QUEUE_EMPTY		��ʾ����Ϊ��
	retval	AK_TIMEOUT			��ʾ�ڹ���ָ����ʱֵ֮�󣬶�����ȻΪ��
	retval	AK_QUEUE_DELETE		����������ڼ���б�ɾ��				*/
/************************************************************************/
T_S32 AK_Receive_From_Queue(T_hQueue queue, T_VOID *message, T_U32 size,
                                T_U32 *actual_size, T_U32 suspend);

/************************************************************************/
/* @brief:	������������Ϣ��ָ���Ķ��еͶˡ����ȶԶ������Ѿ��е���Ϣ���бȽϣ�
			����Ѿ�����ͨ���ص�����Function����Ϣ�����ٷ��롣
			���û����ͬ��Ϣ���Ҷ������㹻�Ŀռ䱣����Ϣ�����������������
			�������Ŀǰֻ֧�ֶ������С�								*/
/* @param:
	Queue	[in]���еľ��
	message	[in]������Ϣָ��
	size	[in]ָ����Ϣ��T_U32������Ŀ
			�������֧�ֱ䳤��Ϣ���������������ڻ�С�ڶ���֧�ֵ���Ϣ�ߴ硣
			�������֧�ֶ�����Ϣ���˲����������õ��ڶ���֧�ֵ���Ϣ�ߴ硣
	Suspend	[in]��������Ѿ�������һ����Ϣ��ָ���Ƿ�����������
			���й�����������Ч��
			AK_NO_SUSPEND���������Ƿ����㣬�����������ء�
			ע���������ӷ������̵߳��ã�����Ψһ��Ч�����á�
			AK_SUSPEND	�����������ֱ�����пռ���Ч��
			ʱ����ֵ��1 �� 4294967293�������������ֱ��һ��������Ϣ���ã�
										����ֱ��ָ��������ʱ�ӽ��ĵ�ʱ��
	Function[in]���бȽϵĻص�������Ԥ���ȽϽ��������ȽϽ����ͬ����AK_TRUE������ȽϽ����ͬ����AK_FALSE��*/

/* @retval:
	AK_SUCCESS			�ɹ�������Ϣ������
	AK_INVALID_QUEUE	��ʾ������Ч
	AK_EXIST_MESSAGE	��ʾ�Ѿ�����ͬ����Ϣ����
	AK_INVALID_POINTER	��ʾ��Ϣָ��ΪAK_NULL
	AK_INVALID_SIZE		��ʾ��Ϣ�ߴ������֧�ֵ���Ϣ�ߴ粻ƥ��
	AK_INVALID_SUSPEND	��ʾ��ͼ�ӷ������̹߳���
	AK_QUEUE_FULL		��ʾ������
	AK_TIMEOUT			��ʾ��ʹ�ڹ���ʱ֮�󣬶���״̬��ȻΪ��
	AK_QUEUE_DELETE		�����ڹ����ڼ���б�ɾ��*/
/*ע������	������жϵȷ������߳��е��ô˽ӿڣ�����������ò���AK_NO_SUSPEND��
			���۶����Ƿ�Ϊ�������᷵�ش��� AK_INVALID_SUSPEND*/
/************************************************************************/
T_S32 AK_Send_Unique_To_Queue(T_hQueue queue, T_VOID *message, T_U32 size, 
                                    T_U32 suspend, CallbakCompare Function);

/************************************************************************/
/* @brief:	������������Ϣ��ָ���Ķ��ж��ˡ�
			���ȶԶ������Ѿ��е���Ϣ���бȽϣ�����Ѿ����˺�message��ͬ����Ϣ�����ٷ��롣
			���û����ͬ��Ϣ���Ҷ������㹻�Ŀռ䱣����Ϣ�����������������
			�������ľ��幦�ܿ��Բο�AK_Send_To_Front_of_Queue���������Ŀǰֻ֧�ֶ������С�
   @param
	Queue	[in]���еľ��
	message	[in]������Ϣָ��
	size	[in]ָ����Ϣ��T_U32������Ŀ
			��������Ǳ䳤��Ϣ���У��������������ڻ�С�ڶ���֧�ֵ���Ϣ�ߴ硣
			��������Ƕ�����Ϣ���˲����������õ��ڶ���֧�ֵ���Ϣ�ߴ硣
	Suspend	[in]��������Ѿ�������һ����Ϣ��ָ���Ƿ�����������
			���й�����������Ч��
			AK_NO_SUSPEND���������Ƿ����㣬�����������ء�
			ע���������ӷ������̵߳��ã�����Ψһ��Ч�����á�
			AK_SUSPEND	�����������ֱ�����пռ���Ч��
			ʱ����ֵ��1 �� 4294967293�������������ֱ��һ��������Ϣ���ã�
										����ֱ��ָ��������ʱ�ӽ��ĵ�ʱ��
	Function[in]���бȽϵĻص�������
				Ԥ���ȽϽ��������ȽϽ����ͬ����AK_TRUE������ȽϽ����ͬ����AK_FALSE��*/

/*����ֵ˵��	
	AK_SUCCESS			�ɹ�������Ϣ������
	AK_INVALID_QUEUE	��ʾ������Ч
	AK_EXIST_MESSAGE	��ʾ�Ѿ�����ͬ����Ϣ����
	AK_INVALID_POINTER	��ʾ��Ϣָ��ΪAK_NULL
	AK_INVALID_SIZE		��ʾ��Ϣ�ߴ������֧�ֵ���Ϣ�ߴ粻ƥ��
	AK_INVALID_SUSPEND	��ʾ��ͼ�ӷ������̹߳���
	AK_QUEUE_FULL		��ʾ������
	AK_TIMEOUT			��ʾ��ʹ�ڹ���ʱ֮�󣬶���״̬��ȻΪ��
	AK_QUEUE_DELETE		�����ڹ����ڼ���б�ɾ��*/
/*ע������	������жϵȷ������߳��е��ô˽ӿڣ�����������ò���AK_NO_SUSPEND��
			���۶����Ƿ�Ϊ�������᷵�ش��� AK_INVALID_SUSPEND*/
/************************************************************************/
T_S32 AK_Send_Unique_To_Front_of_Queue(T_hQueue queue_ptr, T_VOID *message, 
                                    T_U32 size, T_U32 suspend, CallbakCompare Function);

/************************************************************************/
/* @brief:������񷵻��Ѿ���������Ϣ��������							*/
/************************************************************************/
T_U32 AK_Established_Queues(T_VOID);

/************************************************************************/
/* @brief:�����������queueָ���Ķ��У������ڶ�����
				��������񷵻��ʵ��Ĵ���ֵ*/
/* @param:	queue	[in]���еľ��*/
/*����ֵ˵��	
	AK_SUCCESS			�ɹ�������Ϣ������
	AK_INVALID_QUEUE	��ʾ������Ч*/
/************************************************************************/
T_S32 AK_Reset_Queue(T_hQueue queue);

// Mailbox API

T_hMailbox AK_Create_Mailbox(T_OPTION suspend_type);

T_S32 AK_Delete_Mailbox(T_hMailbox mailbox);

T_S32 AK_Broadcast_To_Mailbox(T_hMailbox mailbox, T_U32 *message, T_OPTION suspend_type);

T_S32 AK_Receive_From_Mailbox(T_hMailbox mailbox, T_U32 *message, T_OPTION suspend_type);

T_S32 AK_Send_To_Mailbox(T_hMailbox mailbox, T_U32 *message, T_OPTION suspend_type);

T_U32 AK_Established_Mailboxes(T_VOID);

// Semaphore Control API

/************************************************************************/
/* @brief:	������񴴽�һ�������ź������ź���ֵ��Χ0��4294967294��		*/
/* @param:	initial_count	[in]ָ���ź����ĳ�ʼֵ
			supend_type		[in]ָ������������͡�
							�˲�����Ч����ΪAK_FIFO��AK_PRIORITY��
							�ֱ��ʾ�����ȳ������ȼ�˳�����*/

/*����ֵ˵��	
	T_hSemaphore		��ʾ�ɹ������ź��������ؾ��
	AK_INVALID_SUSPEND	��ʾ�������Ͳ�����Ч*/
/************************************************************************/
T_hSemaphore AK_Create_Semaphore(T_U32 initial_count, T_OPTION suspend_type);

/************************************************************************/
/* @brief	�˷���ɾ��һ����ǰ�������ź�����
			����Semaphoreȷ����Ҫɾ�����ź�����
			������ź����Ϲ��������ָ�ʱ�����ʵ��Ĵ���״̬��
			��ɾ���ڼ��֮��Ӧ�ó�������ֹ�ź�����ʹ�á�*/
/* @param	semaphore	ָ���ź����ľ��								*/
/*����ֵ˵��	
	AK_SUCCESS	��ʾ����ɹ����
	AK_INVALID_SEMAPHORE	��ʾ�ź�������Ƿ�*/
/************************************************************************/
T_S32 AK_Delete_Semaphore(T_hSemaphore semaphore);

/************************************************************************/
/* @brief:	���������ָ���ź�����ʵ����һ��ʵ��ͨ���ڲ�������ʵ�֣�
			���һ���ź���ת��Ϊ���ĸ��ź������ڲ���������һ��
			����ź������������������֮ǰΪ0�����������㡣			*/
/* @param	semaphore	[in]ָ���ź����ľ��
			suspend		[in]����ź������ܱ���ã���ǰΪ0����
			ָ�����ڵ��õ������Ƿ�������й���������Ч��
				AK_NO_SUSPEND	���������Ƿ����㣬�����������ء�
					ע����������һ���������̱߳����ã�����Ψһ��Ч�����á�
				AK_SUSPEND	���ڵ��õ��������ֱ���ź������Ա���á�
				ʱ����ֵ��1��4294967293�����ڵ����������
					ֱ���ź������Ա���û���ָ���Ķ�ʱ������ֵ��ʱ��*/
/* ����ֵ˵��
	AK_SUCCESS				��ʾ����ɹ����
	AK_INVALID_SEMAPHORE	��ʾ�ź�������Ƿ�
	AK_INVALID_SUSPEND		��ʾ��ͼ��һ���������̹߳���
	AK_UNAVAILABLE			��ʾ�ź������Ի��
	AK_TIMEOUT				��ʾ�����ڹ���ָ����ʱ�������ź�����Ȼ���Ի��
	AK_SEMAPHORE_DELETE		����������ڼ��ź�����ɾ��*/
/*ע������	������жϵȷ������߳��е��ô˽ӿڣ�����������ò���AK_NO_SUSPEND��
			�����ܷ����ź��������᷵�ش��� AK_INVALID_SUSPEND*/
/************************************************************************/
T_S32 AK_Obtain_Semaphore(T_hSemaphore semaphore, T_U32 suspend);

T_S32 AK_Try_Obtain_Semaphore(T_hSemaphore semaphore, T_U32 suspend);
T_U32 AK_Get_SemVal(T_hSemaphore semaphore);


/************************************************************************/
/* @brief:	��������ͷ��ɲ���semaphoreָ�����ź�����һ��ʵ����
			����кܶ�����ȴ����ͬһ���ź���������ź�
			����������create�ź���ʱ���AK_PRIORITY��AK_FIFO������
			����AK_PRIORITY�������ȼ��ߵ������ã�����FIFO
			�����ȹ���������ȵá�	���⣬���û������ȴ�
			����ź������ڲ���������һ��			*/
/* @param:	semaphore	[in]ָ���ź����ľ��							*/
/*����ֵ˵��
	AK_SUCCESS	��ʾ����ɹ����
	AK_INVALID_SEMAPHORE	��ʾ�ź�������Ƿ�							*/
/*note: ��ʹ�ɹ��ͷ��ź������ź���Ҳ������ڳ�ʼֵ*/
/************************************************************************/
T_S32 AK_Release_Semaphore(T_hSemaphore semaphore);

/************************************************************************/
/* @brief:	������������ɲ���semaphoreָ�����ź�����
			�����ڴ��ź����Ϲ�������񽫻���ʵ��ķ���ֵ				*/
/* @param:	semaphore	[in]ָ���ź����ľ��							*/
/*����ֵ˵��
	AK_SUCCESS	��ʾ����ɹ����
	AK_INVALID_SEMAPHORE	��ʾ�ź�������Ƿ�							*/
/************************************************************************/
T_S32 AK_Reset_Semaphore(T_hSemaphore semaphore, T_U32 initial_count);

//EventGroup API
T_hEventGroup AK_Create_Event_Group(T_VOID);

T_S32 AK_Delete_Event_Group(T_hEventGroup eventgroup);

T_S32 AK_Retrieve_Events(T_hEventGroup eventgroup, T_U32 requested_events, 
                        T_OPTION operation, T_U32 *retrieved_events, T_U32 suspend);

T_S32 AK_Set_Events(T_hEventGroup eventgroup, T_U32 event_flags, T_OPTION operation);

T_U32 AK_Established_Event_Groups(T_VOID);

// Timer API
T_hTimer AK_Create_Timer(T_VOID (*expiration_routine)(T_U32), T_U32 id, 
                    T_U32 initial_time, T_U32 reschedule_time, T_OPTION enable);
                    
T_S32 AK_Control_Timer(T_hTimer timer, T_OPTION enable);

T_S32 AK_Delete_Timer(T_hTimer timer);

// Interrupt API
/************************************************************************/
/* @brief:	������񴴽�һ���߼��жϷ����ӳ���HISR����
			HISRs�����������akos������ã�����ͼ��жϷ����ӳ���LISR����*/
/* @param:	
	hisr_entry	[in]ָ��HISR�ĺ�����ڵ�
	name		[in]HISR���ַ���ָ�룬�ֻ��8�ֽ�
	priority	[in]������HISR���ȼ���0��2�������ȼ�0Ϊ���
	stack_pointer	[in]HISR�Ķ�ջ��ָ�롣ÿ��HISR�����Լ��Ķ�ջ����
					ע��HISR��ջ�Ѿ��������߷������
	stack_size	[in]HISR��ջ���ֽ���									*/
/*����ֵ˵��
	AK_SUCCESS	��ʾ����ɹ����
	AK_MEMORY_CORRUPT	��ʾ�ڴ�����ʧ��
	AK_INVALID_ENTRY	��ʾHISR���ָ��Ϊ��
	AK_INVALID_PRIORITY	��ʾHISR���ȼ�Ϊ��
	AK_INVALID_MEMORY	��ʾ��ջָ��Ϊ��
	AK_INVALID_SIZE	��ʾ��ջ�ߴ�̫С									*/
/************************************************************************/
T_hHisr AK_Create_HISR(T_VOID (*hisr_entry)(T_VOID), T_U8 *name, 
                          T_OPTION priority, T_VOID *stack_address, T_U32 stack_size);
/************************************************************************/
/* @brief:	�����񼤻���hisrָ��ָ���HISR�����ָ����HISR�������У�
			��μ��������ڵ�ǰ���н���֮ǰ���ᴦ��
			��ÿ�μ�������HISR����һ�Ρ�*/
/* @param:	hisr	[in]HISR�ľ��										*/
/*����ֵ˵��
	AK_SUCCESS	��ʾ����ɹ����
	AK_INVALID_HISR	��ʾHISR����Ƿ�									*/
/************************************************************************/
T_S32 AK_Activate_HISR(T_hHisr hisr);

/************************************************************************/
/* @brief:	�˷���ɾ��һ����ǰ������HISR��
			����hisrȷ����Ҫɾ����HISR��
			��ɾ���ڼ��֮��Ӧ�ó�������ֹHISR��ʹ�á�*/
/* @param:	hisr	[in]HISR�ľ��										*/
/*����ֵ˵��
	AK_SUCCESS	��ʾ����ɹ����
	AK_INVALID_HISR	��ʾHISR����Ƿ�									*/
/************************************************************************/
T_S32 AK_Delete_HISR(T_hHisr hisr);

/************************************************************************/
/* @brief:	�������ʹ��vectorָ�����ж������ͱ�list_entry
			ָ��LISR���������������ڵ���ָ����LISR֮ǰϵͳ
			�������Զ����沢����LISR����֮��ָ�����ˣ�
			LISR����������C���Ա�д��Ȼ����LISRsֻ�������
			������AKOS����������������������̵Ľ�����
			���뼤��һ�������ȼ����жϷ����ӳ���HISR��*/
/* @param
	vector		[in]�ж�����
	list_entry	[in]�ж�����ָ��ĺ�����
	old_lisr	[out]ָ����к������ָ���ָ��							*/

/*����ֵ˵��
	AK_SUCCESS	��ʾ����ɹ����
	AK_INVALID_VECTOR	��ʾָ����������Ч
	AK_NOT_REGISTERED	��ʾ������ǰû��ע���ҷֱ�ע����lisr_entryָ��
				(Indicates the vector is not registered and de-registration was specified by lisr_entry.)
	AK_NO_MORE_LISRS	��ʾ��ע��LISRs��������Ѿ������ˡ����
	����������ͷ�ļ��и��ġ����������Ҫ�ؽ����ļ�*/

/*ע������	����:���һ��LISR�û�����Ա�д��
	��������ѭC���������ڼĴ����÷��ͷ��ػ��Ƶ�Լ����*/
/************************************************************************/
T_S32 AK_Register_LISR(T_S32 vector, T_VOID (*list_entry)(T_S32), 
							T_VOID (**old_lisr)(T_S32));

/************************************************************************/
/* @brief:	�������ʹ�õ����ߣ�����new���Զ���
			�жϷ����ӳ������vectorָ�����ж�������
			���񷵻���ǰ�ж��������ݡ�

���棺�ṩ����ӳ����ISRs�û�����Ա�д��
		����洢�ͻָ��κ�ʹ�õļĴ�����
		һЩ�˿���һЩ����Լ��ǿ������ЩISRs�ϡ�
		�뿴����ָ��Ŀ����Ϣ��ָ���������˿�Ҫ��
����˵��
	vector	[in]�ж�����
	new		[in]�µ��жϷ����ӳ���
                                                                     */
/************************************************************************/
T_VOID *AK_Setup_Vector(T_S32 vector, T_VOID *new_vector);

/************************************************************************/
/* @brief:	��������ȡ��ǰ�Ѿ������ĸ߼��ж�����*/
/************************************************************************/
T_U32 AK_Established_HISRs(T_VOID);


/******************************************************************************************************
**                              use example
**		1   call    AK_feed_watchdog( food)   //call in you thread it will start thread watchodg
**      2   close watchdog   call 	AK_feed_watchdog( 0 )   
**		  
*******************************************************************************************************/

/*************************************************************
*�����������������ӿڣ���������ϵͳ�쳣�����á�
**************************************************************/
T_VOID AK_Drv_Protect(T_VOID);
T_VOID AK_Drv_Unprotect(T_VOID);

/*****************************************************************************************************
* @author Yao Hongshi
* @date 2007-11-06
* @param unsigned int food
* @return STATUS  -- it has two status, one is zero ,ir present watchdog gets well
*                            the  other is  (-1), it present food is illegal
* @brief: watch dog handler of theard ,here it will check watchdog_counter in TCB(or HCB),
* @   wether it has been underflow , if it is right, here will awake up watchdog_HISR. or
* @  it will decrease watchdog_counter,and return.
*******************************************************************************************************/

 typedef  struct locale
 {
	 T_S32			   error_type;
	 T_S8			   tc_name[8+4];			 //task or HISR 's name
	 
	 T_VOID 		   *tc_stack_start; 		 // Stack starting address
	 T_VOID 		   *tc_stack_end;			 // Stack ending address 
	 T_VOID 		   *tc_stack_pointer;		 // HISR or Task stack pointer	 
	 T_U32				tc_stack_size;			 // HISR or Task stack's size	 
	 
	 T_U32				tc_pc_value;			 //thread current PC value
	 
	 T_U32				reg[13];
	 
	 
	 T_U32				tc_current_sp;
	 T_U32				stack_current_value[20];
	 
	 T_U32				func_caller;

	 T_pSTR				feed_file;
	 T_U32				feed_line;
#ifdef		FUNC_ENTRY_MODULE
	 T_U32				entry_function_adrress;  
#endif
	 
 } THREAD_LOCALE;
 
typedef void (*T_WD_CB)(void* pData); 

#ifdef PLATFORM_DEBUG_VER
#define AK_Feed_Watchdog(x)	AK_Feed_WatchdogEx(x, (T_pSTR)(__FILE__),(T_U32)(__LINE__))
#else
#define AK_Feed_Watchdog(x)	
#endif
T_S32 AK_Feed_WatchdogEx(T_U32  food, T_pSTR filename, T_U32 line);

T_VOID AK_Set_WD_Callback(T_WD_CB cb);

T_U32 AK_System_Init(T_VOID *task_entry, T_U8 *name);

#endif
