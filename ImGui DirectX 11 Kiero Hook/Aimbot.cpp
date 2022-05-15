#include "Aimbot.h"
#include "includes.h"

void Aimbot::AimbotThread()
{
	if (Setting::Aimbot_Falgs)
	{

		float aimBot_Min_Distance = Setting::AimBot_Fov;
		ImVec3 aimBot_ViewAngles = ImVec3(0, 0, 0);
		ImVec3 teleW_pedCoords = ImVec3(0, 0, 0);

		// 玩家自己RID
		long long myRID = Memory::ReadMem<long long>(DataManger::hProcess,Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::RID));

		// 相机坐标
		long long pCCameraPTR = Memory::ReadMem<long long>(DataManger::hProcess,Globals::CCameraPTR);
		long long pCCameraPTR_0 = Memory::ReadMem<long long>(DataManger::hProcess,pCCameraPTR + 0x00);
		pCCameraPTR_0 = Memory::ReadMem<long long>(DataManger::hProcess,pCCameraPTR_0 + 0x3C0);
		ImVec3 cameraV3Pos = Memory::ReadMem<ImVec3>(DataManger::hProcess, pCCameraPTR_0 + 0x60);

		int offset[] = { 0x30 };
		// 是否是第一人称，当Fov=0为第一人称或者开镜状态，第三人称50
		float isFPP = Memory::ReadMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x10, READ_WRITE_OFFSET_PARAMETER(offset));
		// 玩家是否处于载具中，或者掩护状态（载具/掩体=0，正常=16）
		byte isPlayerInCar = Memory::ReadMem<byte>(DataManger::hProcess, Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::InVehicle));

		// Ped实体
		long long pReplayInterfacePTR = Memory::ReadMem<long long>(DataManger::hProcess,Globals::ReplayInterfacePTR);
		long long my_offset_0x18 = Memory::ReadMem<long long>(DataManger::hProcess,pReplayInterfacePTR + 0x18);

		for (int i = 0; i < 128; i++)
		{
			long long ped_offset_0 = Memory::ReadMem<long long>(DataManger::hProcess,my_offset_0x18 + 0x100);
			ped_offset_0 = Memory::ReadMem<long long>(DataManger::hProcess,ped_offset_0 + i * 0x10);
			if (ped_offset_0 == 0)
			{
				continue;
			}

			float pedHealth = Memory::ReadMem<float>(DataManger::hProcess, ped_offset_0 + 0x280);
			if (pedHealth <= 0)
			{
				continue;
			}

			long long ped_offset_1 = Memory::ReadMem<long long>(DataManger::hProcess,ped_offset_0 + 0x10C8);
			long long pedRID = Memory::ReadMem<long long>(DataManger::hProcess,ped_offset_1 + 0x90);
			if (myRID == pedRID)
			{
				continue;
			}

			char* pedNameC = Memory::ReadMem<char>(DataManger::hProcess, ped_offset_1 + 0xA4, 20);

			char pedName[22] = { 0 };
			if (ped_offset_1 == 0)
			{
				strcpy_s(pedName, strlen("") + 1, "");
			}
			else
			{
				strcpy_s(pedName, strlen(pedNameC) + 1, pedNameC);
			}
			delete[] pedNameC;
			// 绘制玩家
			if (!Setting::AimBot_ShootPlay)
			{
				if (strcmp(pedName, "") != 0)
				{
					continue;
				}
			}

			// 绘制Ped
			if (!Setting::AimBot_ShootNPC)
			{
				if (strcmp(pedName, "") == 0)
				{
					continue;
				}
			}

			ImVec3 pedV3Pos = Memory::ReadMem<ImVec3>(DataManger::hProcess, ped_offset_0 + 0x90);
			ImVec2 pedV2Pos = Helpers::WorldToScreen(pedV3Pos);

			// 自瞄数据
			float aimBot_Distance = (float)sqrt(pow(pedV2Pos.x - gview_width, 2) + pow(pedV2Pos.y - gview_height, 2));
			// 获取距离准心最近的方框
			if (aimBot_Distance < aimBot_Min_Distance)
			{
				aimBot_Min_Distance = aimBot_Distance;
				aimBot_ViewAngles = Helpers::GetCCameraViewAngles(cameraV3Pos, Helpers::GetBonePosition(ped_offset_0, Setting::AimBot_BoneIndex));
				teleW_pedCoords = pedV3Pos;
			}
		}

		// 玩家处于载具或者掩护状态中不启用自瞄，无目标取消自瞄
		if (isPlayerInCar != 0x00 && aimBot_Min_Distance != Setting::AimBot_Fov)
		{
			 //默认按住Ctrl键自瞄
			if (GetAsyncKeyState(Setting::AimBot_Key) & 0x8000)
			{
				if (isFPP == 0)
				{
					// 第一人称及开镜自瞄
					Memory::WriteMem<float>(DataManger::hProcess,pCCameraPTR_0 + 0x40, aimBot_ViewAngles.X);
					Memory::WriteMem<float>(DataManger::hProcess,pCCameraPTR_0 + 0x44, aimBot_ViewAngles.Y);
					Memory::WriteMem<float>(DataManger::hProcess,pCCameraPTR_0 + 0x48, aimBot_ViewAngles.Z);
				}
				else
				{
					// 第三人称及自瞄
					Memory::WriteMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x3D0, aimBot_ViewAngles.X);
					Memory::WriteMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x3D4, aimBot_ViewAngles.Y);
					Memory::WriteMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x3D8, aimBot_ViewAngles.Z);
				}

			}
		}
		


	}
	
}
