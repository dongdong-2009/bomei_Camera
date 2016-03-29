#ifdef OS_WIN32

#include "zmjDump.h"


typedef int		(*_fInitConsolePrinter)(int LLD, int bStreamFile);
typedef void	(*_fExitConsolePrinter)(int LLD);
typedef int		(*_fConsolePrint)(int LLD, const char *format, ...);
typedef int		(*_fConsolePrint_Color)(int LLD, FRONTCOLOR frontColor, BACKCOLOR bkColor, const char *format, ...);

static _fInitConsolePrinter InitConsolePrinter2 = 0;
static _fExitConsolePrinter ExitConsolePrinter2 = 0;
static _fConsolePrint       ConsolePrint2       = 0;
static _fConsolePrint_Color ConsolePrint_Color2 = 0;

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
HMODULE hDbgDumpDll = 0;

int     LoadDump()
{
	hDbgDumpDll = LoadLibrary("zmjDump.dll");
	if(hDbgDumpDll)
	{
		InitConsolePrinter2 = (_fInitConsolePrinter)GetProcAddress(hDbgDumpDll, "InitConsolePrinter");
        ExitConsolePrinter2 = (_fExitConsolePrinter)GetProcAddress(hDbgDumpDll, "ExitConsolePrinter");
		ConsolePrint2       = (_fConsolePrint)GetProcAddress(hDbgDumpDll, "ConsolePrint");
		ConsolePrint_Color2 = (_fConsolePrint_Color)GetProcAddress(hDbgDumpDll, "ConsolePrint_Color");

		if(InitConsolePrinter2 && ExitConsolePrinter2 && ConsolePrint2 && ConsolePrint_Color2)
			return 1;
	}
	return 0;
}

int     FreeDump()
{
	if(hDbgDumpDll)
		FreeLibrary(hDbgDumpDll);
	hDbgDumpDll = 0;
	return 0;
}

int		KillHistroyDump(int bKill)
{
	int  i;
	char str[128];
	HANDLE hDbgWnd;

	if(0 == bKill)
		return 0;
	
	for(i=0; i<16; i++)
	{
		sprintf(str, "���͵��Դ�ӡ���� LLD = %d", i);

		do
		{
			hDbgWnd = FindWindow(0, str); 
			if(hDbgWnd) 	
				SendMessage(hDbgWnd, WM_CLOSE, 0, 0);
		} while(hDbgWnd);
	}
	
	return 0;
}

int		InitConsolePrinter(int LLD, int bStreamFile)
{
	int ret;
	//char str[128];
	//HANDLE hDbgWnd;

	if(InitConsolePrinter2)
	{
		ret = InitConsolePrinter2(LLD, bStreamFile);

// 		sprintf(str, "���͵��Դ�ӡ���� LLD = %d", LLD);
// 		hDbgWnd = FindWindow(0, str); 
// 		if(hDbgWnd)
// 			MoveWindow(hDbgWnd, 0, 0, 80, 120, TRUE);

		if(ret >= 0)
		{
			ConsolePrint(LLD, "��Ҫ�ص㣺\n");
			ConsolePrint_Color(LLD, FRONT_RED, BACK_WHITE,  "1. ������������һ���������̣�������ʾ����\n");
			ConsolePrint_Color(LLD, FRONT_BLUE, BACK_WHITE, "2. ���������������Դ�����Ȼ�����϶��鿴\n");
			ConsolePrint_Color(LLD, FRONT_GREEN, BACK_WHITE, "3. �������п�����ʱ��ӡ������ɫ\n");
			ConsolePrint_Color(LLD, BACK_WHITE, BACK_BLACK,  "4. �������п�����ʱ�򿪹رն����ӡ����\n");
			ConsolePrint_Color(LLD, FRONT_RED, BACK_WHITE,  "5. ��ͬ���ݿ��������ӡ����һ������\n");
			ConsolePrint_Color(LLD, FRONT_RED, BACK_WHITE,  "6. �������жϵ����ʱ��Ȼ��ʱ���Բ鿴��ӡ��������\n");
			ConsolePrint(LLD, "\n\n\n");
		}

		return ret;
	}


	return -1;
}

void	ExitConsolePrinter(int LLD)
{
	if(ExitConsolePrinter2)
		ExitConsolePrinter2(LLD);
}


static int _tgprinter_nLevel=12;
void	SetConsolePrinterLevel(int level)
{
	if(_tgprinter_nLevel==(int)level)
		return;

	_tgprinter_nLevel = (int)level;
	ConsolePrint(0, "SetConsolePrinterLevel=%d\n", _tgprinter_nLevel);
}

int		ConsolePrint(int LLD, const char *format, ...)
{
	if(LLD >=_tgprinter_nLevel)//MIN_CONSOLE_PRINT_LEVEL==9
		return 0;//priority print

	if(ConsolePrint2)
	{
		int  len;
		char msg[1024];
		va_list   arg_ptr;
		va_start(arg_ptr, format );
		len = vsprintf(msg, format, arg_ptr);
		va_end( arg_ptr );	
		msg[len] = 0;

		LLD = 0; //forbit multi-window display

		return ConsolePrint2(LLD, "%s", msg);
	}	
	return -1;
}

int		ConsolePrint_Color(int LLD, FRONTCOLOR frontColor, BACKCOLOR bkColor, const char *format, ...)
{
	if(ConsolePrint_Color2)
	{
		int  len;
		char msg[1024];
		va_list   arg_ptr;
		va_start(arg_ptr, format );
		len = vsprintf(msg, format, arg_ptr);
		va_end( arg_ptr );	
		msg[len] = 0;

		return ConsolePrint_Color2(LLD, frontColor, bkColor, "%s", msg);
	}	
	return -1;
}
#endif
