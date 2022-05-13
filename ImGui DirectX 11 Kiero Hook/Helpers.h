#pragma once
#include "includes.h"
class Helpers
{
public:

	// 窗口信息获取类
	static BOOL		CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);
	static HWND		GetCurrentWindowHandle();
	static DWORD	GetCurrentProcessID();
	static ImVec2 WorldToScreen(ImVec3 posV3);
	static ImVec3 GetBonePosition(long long offset, int BoneID);
	static ImVec2 GetBoxWH(ImVec3 posV3);
	static ImVec3 GetCCameraViewAngles(ImVec3 cameraV3, ImVec3 targetV3);
	static bool IsNullVector2(ImVec2 vector);
	static bool IsValid(long long Address);

};

