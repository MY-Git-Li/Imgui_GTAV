#include "Keys.h"
#include "includes.h"

extern HMODULE g_hDll;
void Keys::CheckKeys()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) && 0x8000)
		{

			Setting::mainBeginSate = !Setting::mainBeginSate;

			
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));; //–›√ﬂ100∫¡√Î
			Sleep(100);
		}

		if (GetAsyncKeyState(VK_END) && 0x8000)
		{
			FreeLibraryAndExitThread(g_hDll, 0);
		}

		Aimbot::AimbotThread();
	}
	
}
