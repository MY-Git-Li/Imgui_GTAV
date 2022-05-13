#pragma once
#include "../includes.h"


class DataManger
{
public:
	static void DataInit();
	static void DataSeting(HWND g_handle,HANDLE handle ,int pid);
	static WindowData GetGameWindowData();
	static HANDLE hProcess;
	static int pid;
	static long long baseAddress;
	static int moduleSize;
	static HWND g_hWnd;
	static WindowData windowData;
};

