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

	static DWORD GetProcessIdByHwnd(DWORD hWnd); //ͨ�����ھ���õ�����pid

	static HANDLE GetProcessHandle(DWORD pid); //ͨ��pid �õ����̾��

	static HMODULE GetProcessModuleHandle(DWORD pid, CONST TCHAR* moduleName);//ȡָ������PID��ģ����

	static DWORD GetProcessIdByName(LPCTSTR  name); //ͨ����������ȡ����ID

	static DWORD GetProcessModuleSize(DWORD pid, CONST TCHAR* moduleName);

	template<typename T1>
	static T1 ReadMem(HANDLE processHandle, DWORD64 pAddr)   //��ȡ����: ������ �������� ������ �ֽ���
	{
		T1 result;
		ReadProcessMemory(processHandle, (LPCVOID)(pAddr), &result, sizeof(T1), NULL);
		return result;
	}
	template<typename T1>
	static	T1* ReadMem(HANDLE processHandle, DWORD64 pAddr, DWORD length)   //��ȡ����: �Զ����С�ڴ�
	{
		T1* result = new T1[length];
		ReadProcessMemory(processHandle, (LPCVOID)(pAddr), result, length * sizeof(T1), NULL);
		return result;
	}

	template<typename T1, typename T2>
	static	T1 ReadMem(HANDLE processHandle, T2 address, int* Offset, int size)   //��ȡ����: ָ��ƫ�Ƶ��ڴ�
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
	static void WriteMem(HANDLE processHandle, DWORD64 pAddr, T1 vaule)   //д����: ������ �������� ������ �ֽ���
	{
		WriteProcessMemory(processHandle, (LPVOID)(pAddr), &vaule, sizeof(T1), NULL);
	}
	template<typename T1>
	static void WriteMem(HANDLE processHandle, DWORD64 pAddr, T1 vaule, DWORD length)   //д����: �ֽڼ�
	{
		WriteProcessMemory(processHandle, (LPVOID)(pAddr), vaule, length, NULL);
	}

	template<typename T1, typename T2>
	static	void WriteMem(HANDLE processHandle, T2 address, int* Offset, int size,T1 vaule)   //д����: ָ��ƫ�Ƶ��ڴ棬sizeƫ�ƴ�С
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
	* Ѱ��������
	* 1)���̾��
	* 2)�����룬��8b 4e 5c 2b c8 8b 46 ?? 23 
	* 3)������ʼ��ַ
	* 4)����������ַ
	* 5)���ؽ���Ĵ�С
	* 6)���ؽ��������
	*/
	static int FindPattern(HANDLE hProcess,const char* Tzm, unsigned __int64 StartAddress, unsigned __int64 EndAddress, int InitSize, vector<unsigned __int64>& ResultArray);

private:

};


