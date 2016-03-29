#include "Lib_event.h"
#include "Lib_state.h"

#include "sm_port.h"
#include "smport_cfg.h"
#include "Eng_Debug.h"

static T_U8 gb_StackBuffer[MAX_STACK_BUFFER];
// ��Ϊ���ԭʼbuffer������ת��Ϊ�¼����е�bufferʹ�õ�,������ת��ΪM_EVENTENTRY *ָ�롣
// �����¼����е�buffer�������㱻sizeof(M_EVENTENTRY)��4����������
// Ϊ�˼�ʹ����ռ�ı䣬�¼����еĳ��ȹ̶�������gb_EventQueueBufferҪ��sizeof(M_EVENTENTRY)
// ��sizeof(M_EVENTENTRY)�պ�Ϊ12,��˶���gb_EventQueueBufferʱ��Ҫ��12
static T_U8 gb_EventQueueBuffer[MAX_EVENTQUEUE_BUFFER + 12];


M_STATES SM_GetPreProcID(void)
{
	return eM_preproc;
}

M_STATES SM_GetPostProcID(void)
{
	return eM_postproc;
}

vT_EvtCode SM_GetEvtReturn(void)
{
	return M_EVT_RETURN;
}

vT_EvtCode SM_GetEvtNoNext(void)
{
	return VME_EVT_USER;
}


void *SM_GetStackBuffer(unsigned int *bufSize)
{
	if(bufSize)
	{
		(*bufSize) = MAX_STACK_BUFFER;
	}

	AK_ASSERT_VAL(MAX_STACK_BUFFER > SM_CalcStackBufByMaxDepth(MAX_STACK_DEPTH), 
		"SM_GetStackBuffer warning, add more stack buffer", 
		gb_StackBuffer);

	return gb_StackBuffer;
}

void *SM_GetEventQueueBuffer(unsigned int *bufSize)
{
	if(bufSize)
	{
		(*bufSize) = MAX_EVENTQUEUE_BUFFER;
	}
	
	AK_ASSERT_VAL(MAX_EVENTQUEUE_BUFFER > SM_CalcEventBufferByMaxEntries(MAX_EVENTQUEUE_ENTRIES),
		"SM_GetEventQueueBuffer warning, add more EventQueue buffer", 
		gb_EventQueueBuffer);

	return gb_EventQueueBuffer;
}

