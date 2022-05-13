#pragma once

#include <windows.h>
#include <time.h>
#include <iostream>
#include<TlHelp32.h>
#include "vector"
using namespace std;

class Memory
{
public:

	static DWORD GetProcessIdByHwnd(DWORD hWnd); //通过窗口句柄得到进程pid

	static HANDLE GetProcessHandle(DWORD pid); //通过pid 得到进程句柄

	static HMODULE GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);//取指定进程PID里模块句柄

	static DWORD GetProcessIdByName(LPCTSTR  name); //通过进程名获取进程ID

	static DWORD GetProcessModuleSize(DWORD pid, CONST TCHAR* moduleName);

	template<typename T1>
	static T1 ReadMem(HANDLE processHandle, DWORD64 pAddr)   //读取类型: 整数型 短整数型 浮点型 字节型
	{
		T1 result;
		ReadProcessMemory(processHandle, (LPCVOID)(pAddr), &result, sizeof(T1), NULL);
		return result;
	}
	template<typename T1>
	static	T1* ReadMem(HANDLE processHandle, DWORD64 pAddr, DWORD length)   //读取类型: 自定义大小内存
	{
		T1* result = new T1[length];
		ReadProcessMemory(processHandle, (LPCVOID)(pAddr), result, length * sizeof(T1), NULL);
		return result;
	}

	template<typename T1, typename T2>
	static	T1 ReadMem(HANDLE processHandle, T2 address, int* Offset, int size)   //读取类型: 指定偏移的内存
	{
		byte* buffer = new byte[sizeof(T2)];
		int leng = size;

		ReadProcessMemory(processHandle, (LPCVOID)address, buffer, sizeof(T2), NULL);

		for (int i = 0; i < leng; i++)
		{
			if (i != leng - 1)
			{
				ReadProcessMemory(processHandle, (LPCVOID)((*(T2*)buffer + Offset[i])), buffer, sizeof(T2), NULL);

				if ((T2)buffer == 0)
				{
					delete[] buffer;
					return 0;
				}
			}
			else
			{

				ReadProcessMemory(processHandle, (LPCVOID)((*(T2*)buffer + Offset[i])), buffer, sizeof(T1), NULL);

				T1 ans = *(T1*)buffer;
				delete[] buffer;
				return ans;
			}
		}
		return 0;
	}

	template<typename T1>
	static void WriteMem(HANDLE processHandle, DWORD64 pAddr, T1 vaule)   //写类型: 整数型 短整数型 浮点型 字节型
	{
		WriteProcessMemory(processHandle, (LPVOID)(pAddr), &vaule, sizeof(T1), NULL);
	}
	template<typename T1>
	static void WriteMem(HANDLE processHandle, DWORD64 pAddr, T1 vaule, DWORD length)   //写类型: 字节集
	{
		WriteProcessMemory(processHandle, (LPVOID)(pAddr), vaule, length, NULL);
	}

	template<typename T1, typename T2>
	static	void WriteMem(HANDLE processHandle, T2 address, int* Offset, int size,T1 vaule)   //写类型: 指定偏移的内存，size偏移大小
	{

		byte* buffer = new byte[sizeof(T2)];
		int leng = size;

		ReadProcessMemory(processHandle, (LPCVOID)address, buffer, sizeof(T2), NULL);

		for (int i = 0; i < leng; i++)
		{
			if (i != leng - 1)
			{
				ReadProcessMemory(processHandle, (LPCVOID)((*(T2*)buffer + Offset[i])), buffer, sizeof(T2), NULL);

			}
			else
			{
				WriteProcessMemory(processHandle, (LPCVOID)((*(T2*)buffer + Offset[i])), vaule, sizeof(T1), NULL);
			}
		}


	}



	static float* ReadMatrix(HANDLE processHandle, DWORD64 pAddr, DWORD length)
	{
		return ReadMem<float>(processHandle, pAddr, length);
	}

	/*
	* 寻找特征码
	* 1)进程句柄
	* 2)特征码，如8b 4e 5c 2b c8 8b 46 ?? 23 
	* 3)搜索开始地址
	* 4)搜索结束地址
	* 5)返回结果的大小
	* 6)返回结果的向量
	*/
	static int FindPattern(HANDLE hProcess,const char* Tzm, unsigned __int64 StartAddress, unsigned __int64 EndAddress, int InitSize, vector<unsigned __int64>& ResultArray);

private:

};


