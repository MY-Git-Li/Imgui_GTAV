#include "DataManger.h"

HANDLE DataManger::hProcess = 0;
int DataManger::pid = 0;
long long DataManger::baseAddress = 0;
int DataManger::moduleSize = 0;
HWND DataManger::g_hWnd = NULL;

WindowData DataManger::windowData = WindowData(0, 0, 0, 0);

const wchar_t processName[] = L"GTA5.exe";

void DataManger::DataSeting(HWND g_handle,HANDLE handle,int pid)
{
	DataManger::hProcess = handle;
	DataManger::pid = pid;
	DataManger::g_hWnd = g_handle;

	DataManger::baseAddress = (long long)Memory::GetProcessModuleHandle(pid, processName);
	DataManger::moduleSize = Memory::GetProcessModuleSize(pid, processName);


}

WindowData DataManger::GetGameWindowData()
{
	RECT windowRect;
	RECT clientRect;

	WindowData ret = WindowData(0,0,0,0);

	// 获取指定窗口句柄的窗口矩形数据和客户区矩形数据
	GetWindowRect(DataManger::g_hWnd,&windowRect);
	GetClientRect(DataManger::g_hWnd, &clientRect);

	// 计算窗口区的宽和高
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// 处理窗口最小化
	/*if (windowRect.left < 0)
	{
		ret.Left = 0;
		ret.Top= 0;
		ret.Width = 1,
		ret.Height = 1;

		return ret;

	}*/

	// 计算客户区的宽和高
	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;

	// 计算边框
	int borderWidth = (windowWidth - clientWidth) / 2;
	int borderHeight = windowHeight - clientHeight - borderWidth;

	ret.Left = windowRect.left += borderWidth;
	ret.Top = windowRect.top += borderHeight;
	ret.Width = clientWidth,
	ret.Height = clientHeight;

	if (ret.Width == 0 || ret.Height == 0)
	{
		ret.Width = 1920;
		ret.Height = 1080;
	}

	return ret;

}


long long DataManger::FindPattern(string pattern)
{
	vector<unsigned __int64> ResultArray;
	int count = Memory::FindPattern(DataManger::hProcess, pattern.c_str(), DataManger::baseAddress, DataManger::baseAddress + DataManger::moduleSize, 1, ResultArray);
	if (count>0)
	{
		return ResultArray[0];
	}
	else
	{
		return 0;
	}
	
	
}

long long Rip_37(long long address)
{
	return address + Memory::ReadMem<int>(DataManger::hProcess,address + 0x03) + 0x07;
}

long long Rip_6A(long long address)
{
	return address + Memory::ReadMem<int>(DataManger::hProcess, address + 0x06) + 0x0A;
}

long long Rip_389(long long address)
{
	return address + Memory::ReadMem<int>(DataManger::hProcess, address + 0x03) - 0x89;
}


void DataManger::DataInit()
{

	DataManger::windowData = DataManger::GetGameWindowData();

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::WorldMask);
	Globals::WorldPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::BlipMask);
	Globals::BlipPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::GlobalMask);
	Globals::GlobalPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::PlayerchatterNameMask);
	Globals::PlayerChatterNamePTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::NetworkPlayerMgrMask);
	Globals::NetworkPlayerMgrPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::ReplayInterfaceMask);
	Globals::ReplayInterfacePTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::WeatherMask);
	Globals::WeatherPTR = Rip_6A(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::UnkModelMask);
	Globals::UnkModelPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::PickupDataMask);
	Globals::PickupDataPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::ViewPortMask);
	Globals::ViewPortPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::AimingPedMask);
	Globals::AimingPedPTR = Rip_37(Globals::TempPTR);

	Globals::TempPTR = DataManger::FindPattern(Offsets::Mask::CCameraMask);
	Globals::CCameraPTR = Rip_37(Globals::TempPTR);

}


