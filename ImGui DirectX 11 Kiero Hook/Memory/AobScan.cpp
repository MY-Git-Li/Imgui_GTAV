#include "AobScan.h"
using namespace std;


BYTE* MemoryData;//每次将读取的内存读入这里
short Next[260];

//特征码转字节集
WORD GetTzmArray(const char* Tzm, WORD* TzmArray)
{
	int len = 0;
	WORD TzmLength = strlen(Tzm) / 3 + 1;

	for (int i = 0; i < strlen(Tzm); )//将十六进制特征码转为十进制
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

//获取Next数组
void GetNext(short* next, WORD* Tzm, WORD TzmLength)
{
	//特征码（字节集）的每个字节的范围在0-255（0-FF）之间，256用来表示问号，到260是为了防止越界
	for (int i = 0; i < 260; i++)
		next[i] = -1;
	for (int i = 0; i < TzmLength; i++)
		next[Tzm[i]] = i;
}

//搜索一块内存
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
			i += (TzmLength - Next[256]);//如果特征码有问号，就从问号处开始匹配，如果没有就i+=-1
		else
			i += (TzmLength - num);
	}
}

//搜索整个程序
int SearchMemory(HANDLE hProcess,const char* Tzm, unsigned __int64 StartAddress, unsigned __int64 EndAddress, int InitSize, vector<unsigned __int64>& ResultArray)
{
	//初始化MemoryData大小
	MemoryData = new BYTE[BLOCKMAXSIZE];

	int i = 0;
	unsigned long BlockSize;
	MEMORY_BASIC_INFORMATION mbi;

	WORD TzmLength = strlen(Tzm) / 3 + 1;
	WORD* TzmArray = new WORD[TzmLength];

	GetTzmArray(Tzm, TzmArray);
	GetNext(Next, TzmArray, TzmLength);

	//初始化结果数组
	ResultArray.clear();
	ResultArray.reserve(InitSize);

	while (VirtualQueryEx(hProcess, (LPCVOID)StartAddress, &mbi, sizeof(mbi)) != 0)
	{
		/*
		* //过滤内存空间, 根据内存的状态和保护属性进行过滤
				//一般扫描（读写及执行）即可，速度极快，扫不到的话在尝试添加（读写）这一属性
		if (MEM_COMMIT == mbi.State &&			//已分配的物理内存
			//MEM_PRIVATE == mbi.Type ||		//私有内存，不被其他进程共享
			//MEM_IMAGE == mbi.Type &&
			//PAGE_READONLY == mbi.Protect ||	//只读
			//PAGE_EXECUTE_READ == mbi.Protect ||	//读及执行
			//PAGE_READWRITE == mbi.Protect ||	//读写
			PAGE_EXECUTE_READWRITE == mbi.Protect)	//读写及执行
		*/

		//获取可读可写和可读可写可执行的内存块
		if (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READWRITE || mbi.Protect == PAGE_EXECUTE_READ)
		{
			i = 0;
			BlockSize = mbi.RegionSize;
			//搜索这块内存
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
	
	//释放MemoryData大小
	delete []MemoryData;

	return ResultArray.size();

}