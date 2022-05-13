#include "AobScan.h"
using namespace std;


BYTE* MemoryData;//ÿ�ν���ȡ���ڴ��������
short Next[260];

//������ת�ֽڼ�
WORD GetTzmArray(const char* Tzm, WORD* TzmArray)
{
	int len = 0;
	WORD TzmLength = strlen(Tzm) / 3 + 1;

	for (int i = 0; i < strlen(Tzm); )//��ʮ������������תΪʮ����
	{
		char num[2];
		num[0] = Tzm[i++];
		num[1] = Tzm[i++];
		i++;
		if (num[0] != '?' && num[1] != '?')
		{
			int sum = 0;
			WORD a[2];
			for (int i = 0; i < 2; i++)
			{
				if (num[i] >= '0' && num[i] <= '9')
				{
					a[i] = num[i] - '0';
				}
				else if (num[i] >= 'a' && num[i] <= 'z')
				{
					a[i] = num[i] - 87;
				}
				else if (num[i] >= 'A' && num[i] <= 'Z')
				{
					a[i] = num[i] - 55;
				}

			}
			sum = a[0] * 16 + a[1];
			TzmArray[len++] = sum;
		}
		else
		{
			TzmArray[len++] = 256;
		}
	}
	return TzmLength;
}

//��ȡNext����
void GetNext(short* next, WORD* Tzm, WORD TzmLength)
{
	//�����루�ֽڼ�����ÿ���ֽڵķ�Χ��0-255��0-FF��֮�䣬256������ʾ�ʺţ���260��Ϊ�˷�ֹԽ��
	for (int i = 0; i < 260; i++)
		next[i] = -1;
	for (int i = 0; i < TzmLength; i++)
		next[Tzm[i]] = i;
}

//����һ���ڴ�
void SearchMemoryBlock(HANDLE hProcess, WORD* Tzm, WORD TzmLength, unsigned __int64 StartAddress, unsigned long size, vector<unsigned __int64>& ResultArray)
{
	if (!ReadProcessMemory(hProcess, (LPCVOID)StartAddress, MemoryData, size, NULL))
	{
		return;
	}

	for (int i = 0, j, k; i < size;)
	{
		j = i; k = 0;

		for (; k < TzmLength && j < size && (Tzm[k] == MemoryData[j] || Tzm[k] == 256); k++, j++);

		if (k == TzmLength)
		{
			ResultArray.push_back(StartAddress + i);
		}

		if ((i + TzmLength) >= size)
		{
			return;
		}

		int num = Next[MemoryData[i + TzmLength]];
		if (num == -1)
			i += (TzmLength - Next[256]);//������������ʺţ��ʹ��ʺŴ���ʼƥ�䣬���û�о�i+=-1
		else
			i += (TzmLength - num);
	}
}

//������������
int SearchMemory(HANDLE hProcess,const char* Tzm, unsigned __int64 StartAddress, unsigned __int64 EndAddress, int InitSize, vector<unsigned __int64>& ResultArray)
{
	//��ʼ��MemoryData��С
	MemoryData = new BYTE[BLOCKMAXSIZE];

	int i = 0;
	unsigned long BlockSize;
	MEMORY_BASIC_INFORMATION mbi;

	WORD TzmLength = strlen(Tzm) / 3 + 1;
	WORD* TzmArray = new WORD[TzmLength];

	GetTzmArray(Tzm, TzmArray);
	GetNext(Next, TzmArray, TzmLength);

	//��ʼ���������
	ResultArray.clear();
	ResultArray.reserve(InitSize);

	while (VirtualQueryEx(hProcess, (LPCVOID)StartAddress, &mbi, sizeof(mbi)) != 0)
	{
		/*
		* //�����ڴ�ռ�, �����ڴ��״̬�ͱ������Խ��й���
				//һ��ɨ�裨��д��ִ�У����ɣ��ٶȼ��죬ɨ�����Ļ��ڳ�����ӣ���д����һ����
		if (MEM_COMMIT == mbi.State &&			//�ѷ���������ڴ�
			//MEM_PRIVATE == mbi.Type ||		//˽���ڴ棬�����������̹���
			//MEM_IMAGE == mbi.Type &&
			//PAGE_READONLY == mbi.Protect ||	//ֻ��
			//PAGE_EXECUTE_READ == mbi.Protect ||	//����ִ��
			//PAGE_READWRITE == mbi.Protect ||	//��д
			PAGE_EXECUTE_READWRITE == mbi.Protect)	//��д��ִ��
		*/

		//��ȡ�ɶ���д�Ϳɶ���д��ִ�е��ڴ��
		if (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READWRITE || mbi.Protect == PAGE_EXECUTE_READ)
		{
			i = 0;
			BlockSize = mbi.RegionSize;
			//��������ڴ�
			while (BlockSize >= BLOCKMAXSIZE)
			{
				SearchMemoryBlock(hProcess, TzmArray, TzmLength, StartAddress + (BLOCKMAXSIZE * i), BLOCKMAXSIZE, ResultArray);
				BlockSize -= BLOCKMAXSIZE; i++;
			}
			SearchMemoryBlock(hProcess, TzmArray, TzmLength, StartAddress + (BLOCKMAXSIZE * i), BlockSize, ResultArray);

		}
		StartAddress += mbi.RegionSize;

		if (EndAddress != 0 && StartAddress > EndAddress)
		{
			return ResultArray.size();
		}
	}
	delete []TzmArray;
	
	//�ͷ�MemoryData��С
	delete []MemoryData;

	return ResultArray.size();

}