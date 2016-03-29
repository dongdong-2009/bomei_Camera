#ifndef __ENG_CALENDAR_H__
#define __ENG_CALENDAR_H__

#include "anyka_types.h"

#define START_YEAR  1901
#define END_YEAR    2050

extern T_S16 iLunarYear, iLunarMonth, iLunarDay;

//�ж�iYear�ǲ�������
T_BOOL IsLeapYear(T_S16 iYear);

//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
T_S16 WeekDay(T_S16 iYear, T_S16 iMonth, T_S16 iDay);

//����iYear��iMonth�µ����� 1��1�� --- 65535��12��
T_S16 MonthDays(T_S16 iYear, T_S16 iMonth);

//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
// 1901��1��---2050��12��
T_S32 LunarMonthDays(T_S16 iLunarYear, T_S16 iLunarMon);

//��������iLunarYear���������
// 1901��1��---2050��12��
T_S16 LunarYearDays(T_S16 iLunarYear);

//��������iLunarYear��������·ݣ���û�з���0
// 1901��1��---2050��12��
T_S16 GetLeapMonth(T_S16 iLunarYear);

//��iYear���ʽ������ɼ��귨��ʾ���ַ���
void FormatLunarYear(T_S16  iYear, char *pBuffer);

//��iMonth��ʽ���������ַ���
void FormatMonth(T_S16 iMonth, T_U16 *pBuffer, T_BOOL bLunar);

//��iDay��ʽ���������ַ���
void FormatLunarDay(T_S16  iDay, T_U16 *pBuffer);

//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
T_S32 CalcDateDiff(T_S16 iEndYear, T_S16 iEndMonth, T_S16 iEndDay,
                  T_S16 iStartYear, 
                  T_S16 iStartMonth, T_S16 iStartDay);

//���㹫��iYear��iMonth��iDay�ն�Ӧ����������,���ض�Ӧ���������� 0-24
//1901��1��1��---2050��12��31��
T_S16 GetLunarDate(T_S16 iYear, T_S16 iMonth, T_S16 iDay);

void  l_CalcLunarDate(T_S32 iSpanDays);

T_S16  l_GetLunarHolDay(T_S16 iYear, T_S16 iMonth, T_S16 iDay);

#endif

