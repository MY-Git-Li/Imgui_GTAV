#pragma once
#include <Windows.h>
#include <iostream> 
#include <cstring>
#include "atlstr.h"

#include <d3d11.h>
#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "Def.h"
#include "Setting.h"
#include "GameData/Datamanger.h"
#include "GameData/Globals.h"
#include "Memory/Memory.h"
#include "GameData/Offsets.h"
#include "Helpers.h"
#include "Draw.h"
#include "Keys.h"
#include "Color.h"
#include "Aimbot.h"
#include "baidu_font.hpp"
#include "Weapon.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

