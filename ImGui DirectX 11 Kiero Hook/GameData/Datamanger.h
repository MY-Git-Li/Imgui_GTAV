#pragma once
#include "../includes.h"
#include "../Memory/Memory.h"
#define READ_WRITE_OFFSET_PARAMETER(offset) offset,IM_ARRAYSIZE(offset)

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
	static long long FindPattern(std::string pattern);
	template<typename T1>
	static T1 Read(DWORD64 pAddr)   //读取类型: 整数型 短整数型 浮点型 字节型
	{
		return Memory::ReadMem(DataManger::hProcess, pAddr);
	}
	template<typename T1>
	static	T1* Read(DWORD64 pAddr, DWORD length)   //读取类型: 自定义大小内存
	{
		return Memory::ReadMem(DataManger::hProcess, pAddr, length);
	}

	template<typename T1, typename T2>
	static	T1 Read(T2 address, int* Offset, int size)   //读取类型: 指定偏移的内存
	{
		return Memory::ReadMem(DataManger::hProcess, address, Offset, size);
	}


	template<typename T1>
	static void Write(DWORD64 pAddr, T1 vaule)   //写类型: 整数型 短整数型 浮点型 字节型
	{
		Memory::WriteMem(DataManger::hProcess, pAddr, vaule);
	}
	template<typename T1>
	static void Write(DWORD64 pAddr, T1 vaule, DWORD length)   //写类型: 字节集
	{
		Memory::WriteMem(DataManger::hProcess, pAddr, vaule, length);
	}

	template<typename T1, typename T2>
	static	void Write(T2 address, int* Offset, int size, T1 vaule)   //写类型: 指定偏移的内存，size偏移大小
	{
		Memory::WriteMem(DataManger::hProcess, address,Offset, size, vaule);
	}

};

