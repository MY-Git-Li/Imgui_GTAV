#include "includes.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

HWND g_hWnd = NULL;
HMODULE g_hDll = NULL;

ImFont* font = NULL;


HANDLE Cmain = NULL;
HANDLE KeyTH = NULL;
HANDLE AimbotTh = NULL;
void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& Io = ImGui::GetIO();
	//io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig f_cfg;
	f_cfg.FontDataOwnedByAtlas = false;
	/*const ImFont* font = Io.Fonts->AddFontFromMemoryTTF((void*)baidu_font_data, baidu_font_size, 16.0f, &f_cfg, Io.Fonts->GetGlyphRangesChineseSimplifiedCommon());*/
	font = Io.Fonts->AddFontFromMemoryTTF((void*)baidu_font_data, baidu_font_size, 16.0f, &f_cfg, Io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:    case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:    case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
		if (Setting::checkBoxFalgs_1) return true;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Draw::DrawWin();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	/*AllocConsole();
	FILE* stream1;
	freopen_s(&stream1,"CON", "w", stdout);
	SetConsoleTitleA("调试窗口");*/


	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	do
	{
		g_hWnd = Helpers::GetCurrentWindowHandle();
	} while (!g_hWnd);

	auto pid = Memory::GetProcessIdByHwnd((DWORD)g_hWnd);
	auto handle = Memory::GetProcessHandle(pid);

	DataManger::DataSeting(g_hWnd, handle, pid);
	DataManger::DataInit();


	//开启辅助线程
	KeyTH = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Keys::CheckKeys, NULL, 0, nullptr);

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hDll = (HMODULE)hMod;
		DisableThreadLibraryCalls(hMod);
		Cmain = CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		CloseHandle(KeyTH);
		CloseHandle(Cmain);
		SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
		kiero::shutdown();
		break;
	}
	return TRUE;
}