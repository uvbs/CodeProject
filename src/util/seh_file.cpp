#include "seh_file.h"

#define	DUMP_SIZE_MAX	8000										
#define	CALL_TRACE_MAX	((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))
#define	NL				L"\n"

#ifdef _WIN32
#include <tlhelp32.h>
#include <dbghelp.h>
#include <tchar.h>

// 抛出字符串形式异常
#pragma managed(push, off)

//--------------------------------------------------------------
// 自动加载dbghelp.dll
typedef	bool (WINAPI * MINIDUMP_WRITE_DUMP)(IN HANDLE			hProcess,
												IN UINT				ProcessId,
												IN HANDLE			hFile,
												IN MINIDUMP_TYPE	DumpType,
												IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
												IN PMINIDUMP_USER_STREAM_INFORMATION		UserStreamParam, OPTIONAL
												IN PMINIDUMP_CALLBACK_INFORMATION			CallbackParam OPTIONAL
												);


struct tAUTO_LOAD_DBGHELP
{
	tAUTO_LOAD_DBGHELP();
	~tAUTO_LOAD_DBGHELP();

	HMODULE					m_hDbgHelp;
	MINIDUMP_WRITE_DUMP		m_pfnMiniDumpWriteDump;
} theDbgHelper;


tAUTO_LOAD_DBGHELP::tAUTO_LOAD_DBGHELP()
{
	//加载dbghelp.dll
	m_hDbgHelp = ::LoadLibrary(L"DBGHELP.DLL");
	if(NULL == theDbgHelper.m_hDbgHelp) 
		return;

	//得到最小的dump文件处理函数
	m_pfnMiniDumpWriteDump = (MINIDUMP_WRITE_DUMP)GetProcAddress(m_hDbgHelp, (LPCSTR)("MiniDumpWriteDump"));
	if(NULL == m_pfnMiniDumpWriteDump) 
	{
		::FreeLibrary(m_hDbgHelp);
		m_hDbgHelp = NULL;
	}
}

tAUTO_LOAD_DBGHELP::~tAUTO_LOAD_DBGHELP()
{
	if(m_hDbgHelp)
	{
		::FreeLibrary(m_hDbgHelp);
		m_hDbgHelp= NULL;
	}
}

//得到运行程序的存放路径和运行路径
bool WINAPI Get_Module_By_Ret_Addr(PBYTE Ret_Addr, wchar_t* Module_Name, PBYTE & Module_Addr)
{
	MODULEENTRY32	M = {sizeof(M)};
	HANDLE	hSnapshot;

	Module_Name[0] = 0;
	hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

	if ((hSnapshot != INVALID_HANDLE_VALUE) &&
		::Module32First(hSnapshot, &M))
	{
		do
		{
			if (uint(Ret_Addr - M.modBaseAddr) < M.modBaseSize)
			{
				lstrcpyn(Module_Name, M.szExePath, MAX_PATH);
				Module_Addr = M.modBaseAddr;
				break;
			}
		} while (::Module32Next(hSnapshot, &M));
	}

	::CloseHandle(hSnapshot);

	return !!Module_Name[0];
}

//得到调用堆栈
void Get_Call_Stack(PEXCEPTION_POINTERS pException, FILE* fp)
{
	wchar_t	Module_Name[MAX_PATH];
	PBYTE	Module_Addr = 0;
	PBYTE	Module_Addr_1;

#pragma warning(disable: 4200)
	typedef struct STACK
	{
		STACK *	Ebp;
		PBYTE	Ret_Addr;
		uint	Param[0];
	} STACK, * PSTACK;
#pragma warning(default: 4200)

	STACK	Stack = {0, 0};
	PSTACK	Ebp;

	if (pException)
	{
		//Stack.Ebp = (PSTACK)(DWORD_PTR)pException->ContextRecord->Ebp;
		Stack.Ret_Addr = (PBYTE)pException->ExceptionRecord->ExceptionAddress;
		Ebp = &Stack;
	}
	else
	{
		Ebp = (PSTACK)&pException - 1;

		if (!IsBadReadPtr(Ebp, sizeof(PSTACK)))
			Ebp = Ebp->Ebp;
	}

	for (int Ret_Addr_I = 0;
		(Ret_Addr_I < CALL_TRACE_MAX) && !IsBadReadPtr(Ebp, sizeof(PSTACK)) && !IsBadCodePtr(FARPROC(Ebp->Ret_Addr));
		Ret_Addr_I++, Ebp = Ebp->Ebp)
	{
		if (Get_Module_By_Ret_Addr(Ebp->Ret_Addr, Module_Name, Module_Addr_1))
		{
			if (Module_Addr_1 != Module_Addr)
			{
				Module_Addr = Module_Addr_1;
				_ftprintf(fp, L"%08X  %s", (LONG_PTR)Module_Addr, Module_Name);
			}

			_ftprintf(fp, L"  +%08X", Ebp->Ret_Addr - Module_Addr);

			if (pException && !Ret_Addr_I)
				_ftprintf(fp, L"  Exception Offset" );
			else if (!IsBadReadPtr(Ebp, sizeof(PSTACK) + 5 * sizeof(uint)))
			{
				_ftprintf(fp, L"  (%X, %X, %X, %X, %X)" ,
					Ebp->Param[0], Ebp->Param[1], Ebp->Param[2], Ebp->Param[3], Ebp->Param[4]);
			}
		}
		else
			_ftprintf(fp, L"%08X", (LONG_PTR)(Ebp->Ret_Addr));
	}

}

//得到windows版本
void WINAPI Get_Version_Str(FILE* fp)
{
	OSVERSIONINFOEX	V = { sizeof(OSVERSIONINFOEX) };	//NT5.0或更新

	if (!GetVersionEx((POSVERSIONINFO)&V))
	{
		ZeroMemory(&V, sizeof(V));
		V.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx((POSVERSIONINFO)&V);
	}

	if (V.dwPlatformId != VER_PLATFORM_WIN32_NT)
		V.dwBuildNumber = LOWORD(V.dwBuildNumber);		//9x版本

	wchar_t dateBuf[32];
	_tstrdate(dateBuf);
	wchar_t timeBuf[32];
	_tstrtime(timeBuf);


	_ftprintf(fp,
		L"****************************************************" 
		L"操作系统信息：" 
		L"Windows:  %d.%d.%d, SP %d.%d, Product Type %d" 
		NL
		L"Time:     %s %s",
		V.dwMajorVersion, V.dwMinorVersion, V.dwBuildNumber, V.wServicePackMajor, V.wServicePackMinor, V.wProductType, 
		dateBuf, timeBuf);
}

//创建错误描述
void CreateExceptionDesc(PEXCEPTION_POINTERS pException, FILE* fp, uint dwLastError)
{
	if (!pException || !fp) return;

	EXCEPTION_RECORD &	E = *pException->ExceptionRecord;
	CONTEXT &			C = *pException->ContextRecord;

	// 取得异常发生地
	wchar_t	szModeleInfo[MAX_PATH];
	wchar_t	Module_Name[MAX_PATH];
	PBYTE Module_Addr;
	if (Get_Module_By_Ret_Addr((PBYTE)E.ExceptionAddress, Module_Name, Module_Addr))
	{
		_sntprintf(szModeleInfo, MAX_PATH, L"%s", Module_Name);
	}
	else
	{
		_sntprintf(szModeleInfo, MAX_PATH, L"%08X", (DWORD_PTR)(E.ExceptionAddress));
	}

	switch(E.ExceptionCode)
	{
		// 转化后的c++异常
	case 0XE000C0DE:
		{
			char* szException = (char*)E.ExceptionInformation[0];

			_ftprintf(fp,
				L"C++ Exception\n"
				L"\n"
				L"Expr:      %s\n",
				szException);
		}
		break;

		// 试图对一个虚地址进行读写
	case EXCEPTION_ACCESS_VIOLATION:
		{
			// Access violation type - Write/Read.
			_ftprintf(fp,
				L"\t\tAccess violation\n"
				L"\n"
				L"@:         %s\n"
				L"Operate:   %s\n"
				L"Address:   0x%08X\n"
				L"LastError: 0x%08X\n",
				szModeleInfo,
				(E.ExceptionInformation[0]) ? "Write" : "Read", 
				E.ExceptionInformation[1], dwLastError);
		}
		break;

	default:
		{
			_ftprintf(fp,
				L"\t\tOTHER\n"
				L"\n"
				L"@:         %s\n"
				L"Code:      0x%08X\n"
				L"LastError: 0x%08X\n",
				szModeleInfo,
				E.ExceptionCode, dwLastError);
		}
		break;
	}

}

//取得错误信息
void Get_Exception_Info(PEXCEPTION_POINTERS pException, FILE* fp, uint dwLastError)
{
	wchar_t		Module_Name[MAX_PATH];
	PBYTE		Module_Addr;
	HANDLE		hFile;
	FILETIME	Last_Write_Time;
	FILETIME	Local_File_Time;
	SYSTEMTIME	T;

	Get_Version_Str(fp);

	_ftprintf(fp, L"****************************************************");
	_ftprintf(fp, L"Process:  " );

	GetModuleFileName(NULL, Module_Name, MAX_PATH);
	_ftprintf(fp, L"%s" , Module_Name);

	if (pException)
	{
		EXCEPTION_RECORD &	E = *pException->ExceptionRecord;
		CONTEXT &			C = *pException->ContextRecord;

		if (Get_Module_By_Ret_Addr((PBYTE)E.ExceptionAddress, Module_Name, Module_Addr))
		{
			_ftprintf(fp, L"Module:   %s" , Module_Name);

			if ((hFile = CreateFile(Module_Name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
			{
				if (GetFileTime(hFile, NULL, NULL, &Last_Write_Time))
				{
					FileTimeToLocalFileTime(&Last_Write_Time, &Local_File_Time);
					FileTimeToSystemTime(&Local_File_Time, &T);

					_ftprintf(fp, L"Date Modified:  %02d/%02d/%d" , 
						T.wMonth, T.wDay, T.wYear);
				}
				CloseHandle(hFile);
			}
		}
		else
		{
			_ftprintf(fp, L"Exception Addr:  %08X"  , (LONG_PTR)(E.ExceptionAddress));
		}

		_ftprintf(fp, L"****************************************************");

		// 加入具体异常解释信息
		CreateExceptionDesc(pException, fp, dwLastError);
	}

	_ftprintf(fp, L"****************************************************");
	_ftprintf(fp, L"Call Stack:");
	Get_Call_Stack(pException, fp);
}

// 生成基本描述文件
bool CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, uint dwLastError)
{
	if( !pException ) 
		return false;

	//创建基本描述文件
	wchar_t szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	wchar_t szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snwprintf(szCrashFile, MAX_PATH, L"Error%02d_%02d_%02d_%02d_%02d.sml", timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	//-------------------------------------------------------
	wchar_t szLocalFile[MAX_PATH] = {0};
	wcsncpy(szLocalFile, szCurrentDir, MAX_PATH);

	PathAppend(szLocalFile, L"CrashReport");
	if (!PathFileExists(szLocalFile))
		CreateDirectory(szLocalFile, 0);

	PathAppend(szLocalFile, szCrashFile);

	FILE* fp = _tfopen(szLocalFile, L"w");
	if( !fp )
		return false;

	CreateExceptionDesc( pException, fp, dwLastError);

	fclose(fp);
	fp = NULL;

	return true;
}

// 生成完整表述文件
bool CreateBigInfoFile(PEXCEPTION_POINTERS pException, uint dwLastError)
{
	if (!pException) return false;

	wchar_t szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	wchar_t szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snwprintf(szCrashFile, MAX_PATH, L"Error%02d_%02d_%02d_%02d_%02d.dtl", timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	//-------------------------------------------------------
	wchar_t szLocalFile[MAX_PATH] = {0};
	wcsncpy(szLocalFile, szCurrentDir, MAX_PATH);

	PathAppend(szLocalFile, L"CrashReport");
	if (!PathFileExists(szLocalFile))
		CreateDirectory(szLocalFile, 0);

	PathAppend(szLocalFile, szCrashFile);

	FILE* fp = _tfopen(szLocalFile, L"w");
	if(!fp) return false;

	Get_Exception_Info(pException, fp, dwLastError);

	fclose(fp); fp = NULL;

	return true;
}

// 创建DBGHLEP所需要的dmp信息
bool CreateDumpHelpFile( PEXCEPTION_POINTERS pException)
{
	if( !theDbgHelper.m_pfnMiniDumpWriteDump )
		return false;

	wchar_t szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	wchar_t szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snwprintf(szCrashFile, MAX_PATH, L"Error%02d_%02d_%02d_%02d_%02d.dmp", timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	//-------------------------------------------------------
	wchar_t szLocalFile[MAX_PATH] = {0};
	wcsncpy(szLocalFile, szCurrentDir, MAX_PATH);

	PathAppend(szLocalFile, L"CrashReport");
	if (!PathFileExists(szLocalFile))
		CreateDirectory(szLocalFile, 0);

	PathAppend(szLocalFile, szCrashFile);

	MINIDUMP_EXCEPTION_INFORMATION	M;

	M.ThreadId = GetCurrentThreadId();
	M.ExceptionPointers = pException;
	M.ClientPointers = 0;

	HANDLE hDump_File = CreateFile( (LPTSTR)szLocalFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	theDbgHelper.m_pfnMiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hDump_File, MiniDumpNormal, 
		(pException) ? &M : NULL, NULL, NULL );

	CloseHandle( hDump_File );

	return true;
}

void X_ThrowStringException(const wchar_t* szFmt, ...)
{
	va_list argList;
	va_start(argList, szFmt);

	uint nSize = _vscwprintf(szFmt, argList);

	wchar_t strMessage[1024];
	_vswprintf(strMessage, szFmt, argList);
	va_end(argList);

	//转化为结构化异常
	static wchar_t* strCPPException;
	strCPPException = strMessage;
	const LPVOID  pException = (const LPVOID)strCPPException;
	::RaiseException(
		0XE000C0DE, 
		EXCEPTION_NONCONTINUABLE,				// 不可继续的严重错误
		1,										// 1个参数
		(CONST ULONG_PTR *)&pException);		// 指向静态异常内存

}
#pragma managed(pop)

void X_ThrowAssertException(const wchar_t* szFileName, int nFileLine, const wchar_t* pExpr)
{
	const wchar_t *szAssertFmt = 
		L"Assert  \r\n"
		L"\r\n"
		L"File:      %s\r\n"
		L"Line:      %d\r\n"
		L"Expr:      %s\r\n";

	uint nSize = _scwprintf(szAssertFmt, szFileName, nFileLine, pExpr);

	wchar_t strMessage[1024]={0};
	_snwprintf(strMessage, 1024,szAssertFmt, szFileName, nFileLine, pExpr);

	//转化为结构化异常
	static wchar_t* strCPPException;
	strCPPException = strMessage;
	const LPVOID  pException = (const LPVOID)strCPPException;
	::RaiseException(
		0XE000C0DE, 
		EXCEPTION_NONCONTINUABLE,				// 不可继续的严重错误
		1,										// 1个参数
		(CONST ULONG_PTR *)&pException);
}

//--------------------------------------------------
// 分析SEH数据
long WINAPI X_ProcessException(PEXCEPTION_POINTERS pException)
{
	// 保存最后的错误代码
	uint	dwLastError = ::GetLastError();

	if(!pException) 
		return 0;

	// 生成基本描述文件
	if(!CreateSmallDumpInfo(pException, dwLastError))
	{
		return 0;
	}

	// 生成完整表述文件
	if(!CreateBigInfoFile(pException, dwLastError))
	{
		return 0;
	}

	// 生成dump报告
	wchar_t szDumpFile[MAX_PATH] = {0};
	CreateDumpHelpFile(pException);

	return 1;
}

#endif // _WIN32

