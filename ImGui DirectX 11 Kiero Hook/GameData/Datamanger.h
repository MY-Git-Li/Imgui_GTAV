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
	static T1 Read(DWORD64 pAddr)   //��ȡ����: ������ �������� ������ �ֽ���
	{
		return Memory::ReadMem(DataManger::hProcess, pAddr);
	}
	template<typename T1>
	static	T1* Read(DWORD64 pAddr, DWORD length)   //��ȡ����: �Զ����С�ڴ�
	{
		return Memory::ReadMem(DataManger::hProcess, pAddr, length);
	}

	template<typename T1, typename T2>
	static	T1 Read(T2 address, int* Offset, int size)   //��ȡ����: ָ��ƫ�Ƶ��ڴ�
	{
		return Memory::ReadMem(DataManger::hProcess, address, Offset, size);
	}


	template<typename T1>
	static void Write(DWORD64 pAddr, T1 vaule)   //д����: ������ �������� ������ �ֽ���
	{
		Memory::WriteMem(DataManger::hProcess, pAddr, vaule);
	}
	template<typename T1>
	static void Write(DWORD64 pAddr, T1 vaule, DWORD length)   //д����: �ֽڼ�
	{
		Memory::WriteMem(DataManger::hProcess, pAddr, vaule, length);
	}

	template<typename T1, typename T2>
	static	void Write(T2 address, int* Offset, int size, T1 vaule)   //д����: ָ��ƫ�Ƶ��ڴ棬sizeƫ�ƴ�С
	{
		Memory::WriteMem(DataManger::hProcess, address,Offset, size, vaule);
	}

};

