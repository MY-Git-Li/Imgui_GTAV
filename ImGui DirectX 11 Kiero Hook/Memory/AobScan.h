#include <windows.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <Psapi.h>

#define BLOCKMAXSIZE 409600//每次读取内存的最大大小

int SearchMemory(HANDLE hProcess,const char* Tzm, unsigned __int64 StartAddress, unsigned __int64 EndAddress, int InitSize, std::vector<unsigned __int64>& ResultArray);
