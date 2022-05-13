#include "includes.h"
#include "Helpers.h"


HWND m_hWnd;

BOOL Helpers::EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
	DWORD wndPid;
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid != (DWORD)lParam) return TRUE;

	m_hWnd = hWnd;
	return FALSE;
}

// 获取鼠标所在窗口的句柄
HWND Helpers::GetCurrentWindowHandle()
{
	m_hWnd = NULL;
	EnumWindows(EnumWindowsCallback, GetCurrentProcessID());
	return m_hWnd;
}

DWORD Helpers::GetCurrentProcessID(){return GetCurrentProcessId();}
// 世界坐标转屏幕坐标
ImVec2 Helpers::WorldToScreen(ImVec3 posV3)
{
	ImVec2 screenV2;
	ImVec3 cameraV3;

	float* viewMatrix = Memory::ReadMatrix(DataManger::hProcess, Globals::ViewPortPTR + 0xC0, 16);

	cameraV3.Z = viewMatrix[2] * posV3.X + viewMatrix[6] * posV3.Y + viewMatrix[10] * posV3.Z + viewMatrix[14];
	if (cameraV3.Z < 0.001f)
	{
		delete[]viewMatrix;
		return ImVec2(0, 0);
	}


	/*cameraV3.X = DataManger::windowData.Width / 2;
	cameraV3.Y = DataManger::windowData.Height / 2;*/
	cameraV3.X = WindowData_Width / 2;
	cameraV3.Y = WindowData_Height / 2;
	cameraV3.Z = 1 / cameraV3.Z;

	screenV2.x = viewMatrix[0] * posV3.X + viewMatrix[4] * posV3.Y + viewMatrix[8] * posV3.Z + viewMatrix[12];
	screenV2.y = viewMatrix[1] * posV3.X + viewMatrix[5] * posV3.Y + viewMatrix[9] * posV3.Z + viewMatrix[13];

	screenV2.x = cameraV3.X + cameraV3.X * screenV2.x * cameraV3.Z;
	screenV2.y = cameraV3.Y - cameraV3.Y * screenV2.y * cameraV3.Z;

	delete[]viewMatrix;
	return screenV2;
}

ImVec3 Helpers::GetBonePosition(long long offset, int BoneID)
{
	float* bomematrix = Memory::ReadMatrix(DataManger::hProcess, offset + 0x60, 16);

	ImVec3 bone_offset_pos;
	bone_offset_pos.X = Memory::ReadMem<float>(DataManger::hProcess, offset + 0x430 + BoneID * 0x10);
	bone_offset_pos.Y = Memory::ReadMem<float>(DataManger::hProcess, offset + 0x430 + BoneID * 0x10 + 0x4);
	bone_offset_pos.Z = Memory::ReadMem<float>(DataManger::hProcess, offset + 0x430 + BoneID * 0x10 + 0x4 + 0x4);

	ImVec3 bone_pos;
	bone_pos.X = bomematrix[0] * bone_offset_pos.X + bomematrix[4] * bone_offset_pos.Y + bomematrix[8] * bone_offset_pos.Z + bomematrix[12];
	bone_pos.Y = bomematrix[1] * bone_offset_pos.X + bomematrix[5] * bone_offset_pos.Y + bomematrix[9] * bone_offset_pos.Z + bomematrix[13];
	bone_pos.Z = bomematrix[2] * bone_offset_pos.X + bomematrix[6] * bone_offset_pos.Y + bomematrix[10] * bone_offset_pos.Z + bomematrix[14];

	delete[]bomematrix;
	return bone_pos;
}

ImVec2 Helpers::GetBoxWH(ImVec3 posV3)
{
	ImVec2 boxV2;
	ImVec3 cameraV3;
	float* viewMatrix = Memory::ReadMatrix(DataManger::hProcess, Globals::ViewPortPTR + 0xC0, 16);
	cameraV3.Z = viewMatrix[2] * posV3.X + viewMatrix[6] * posV3.Y + viewMatrix[10] * posV3.Z + viewMatrix[14];

	if (cameraV3.Z < 0.001f)
	{
		delete[]viewMatrix;
		return ImVec2(0, 0);
	}

	/*cameraV3.Y = DataManger::windowData.Height / 2;*/
	cameraV3.Y = WindowData_Height / 2;
	cameraV3.Z = 1 / cameraV3.Z;

	boxV2.x = viewMatrix[1] * posV3.X + viewMatrix[5] * posV3.Y + viewMatrix[9] * (posV3.Z + 1.0f) + viewMatrix[13];
	boxV2.y = viewMatrix[1] * posV3.X + viewMatrix[5] * posV3.Y + viewMatrix[9] * (posV3.Z - 1.0f) + viewMatrix[13];

	boxV2.x = cameraV3.Y - cameraV3.Y * boxV2.x * cameraV3.Z;
	boxV2.y = cameraV3.Y - cameraV3.Y * boxV2.y * cameraV3.Z;
	boxV2.y = abs(boxV2.x - boxV2.y);
	boxV2.x = boxV2.y / 2;

	delete[]viewMatrix;
	return boxV2;
}

bool Helpers::IsNullVector2(ImVec2 vector)
{
	return (vector.x == 0 && vector.y == 0);
}

bool Helpers::IsValid(long long Address)
{
	return Address >= 0x10000 && Address < 0x000F000000000000;
}

// 鼠标角度
ImVec3 Helpers::GetCCameraViewAngles(ImVec3 cameraV3, ImVec3 targetV3)
{
	float distance = (float)sqrt(pow(cameraV3.X - targetV3.X, 2) +pow(cameraV3.Y - targetV3.Y, 2) + pow(cameraV3.Z - targetV3.Z, 2));

	return ImVec3((targetV3.X - cameraV3.X) / distance,
		(targetV3.Y - cameraV3.Y) / distance,
		(targetV3.Z - cameraV3.Z) / distance);
}