#include "Aimbot.h"
#include "includes.h"

void Aimbot::AimbotThread()
{
	if (Setting::Aimbot_Falgs)
	{

		float aimBot_Min_Distance = Setting::AimBot_Fov;
		ImVec3 aimBot_ViewAngles = ImVec3(0, 0, 0);
		ImVec3 teleW_pedCoords = ImVec3(0, 0, 0);

		// ����Լ�RID
		long long myRID = Memory::ReadMem<long long>(DataManger::hProcess,Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::RID));

		// �������
		long long pCCameraPTR = Memory::ReadMem<long long>(DataManger::hProcess,Globals::CCameraPTR);
		long long pCCameraPTR_0 = Memory::ReadMem<long long>(DataManger::hProcess,pCCameraPTR + 0x00);
		pCCameraPTR_0 = Memory::ReadMem<long long>(DataManger::hProcess,pCCameraPTR_0 + 0x3C0);
		ImVec3 cameraV3Pos = Memory::ReadMem<ImVec3>(DataManger::hProcess, pCCameraPTR_0 + 0x60);

		int offset[] = { 0x30 };
		// �Ƿ��ǵ�һ�˳ƣ���Fov=0Ϊ��һ�˳ƻ��߿���״̬�������˳�50
		float isFPP = Memory::ReadMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x10, READ_WRITE_OFFSET_PARAMETER(offset));
		// ����Ƿ����ؾ��У������ڻ�״̬���ؾ�/����=0������=16��
		byte isPlayerInCar = Memory::ReadMem<byte>(DataManger::hProcess, Globals::WorldPTR, READ_WRITE_OFFSET_PARAMETER(Offsets::InVehicle));

		// Pedʵ��
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
			// �������
			if (!Setting::AimBot_ShootPlay)
			{
				if (strcmp(pedName, "") != 0)
				{
					continue;
				}
			}

			// ����Ped
			if (!Setting::AimBot_ShootNPC)
			{
				if (strcmp(pedName, "") == 0)
				{
					continue;
				}
			}

			ImVec3 pedV3Pos = Memory::ReadMem<ImVec3>(DataManger::hProcess, ped_offset_0 + 0x90);
			ImVec2 pedV2Pos = Helpers::WorldToScreen(pedV3Pos);

			// ��������
			float aimBot_Distance = (float)sqrt(pow(pedV2Pos.x - gview_width, 2) + pow(pedV2Pos.y - gview_height, 2));
			// ��ȡ����׼������ķ���
			if (aimBot_Distance < aimBot_Min_Distance)
			{
				aimBot_Min_Distance = aimBot_Distance;
				aimBot_ViewAngles = Helpers::GetCCameraViewAngles(cameraV3Pos, Helpers::GetBonePosition(ped_offset_0, Setting::AimBot_BoneIndex));
				teleW_pedCoords = pedV3Pos;
			}
		}

		// ��Ҵ����ؾ߻����ڻ�״̬�в��������飬��Ŀ��ȡ������
		if (isPlayerInCar != 0x00 && aimBot_Min_Distance != Setting::AimBot_Fov)
		{
			 //Ĭ�ϰ�סCtrl������
			if (GetAsyncKeyState(Setting::AimBot_Key) & 0x8000)
			{
				if (isFPP == 0)
				{
					// ��һ�˳Ƽ���������
					Memory::WriteMem<float>(DataManger::hProcess,pCCameraPTR_0 + 0x40, aimBot_ViewAngles.X);
					Memory::WriteMem<float>(DataManger::hProcess,pCCameraPTR_0 + 0x44, aimBot_ViewAngles.Y);
					Memory::WriteMem<float>(DataManger::hProcess,pCCameraPTR_0 + 0x48, aimBot_ViewAngles.Z);
				}
				else
				{
					// �����˳Ƽ�����
					Memory::WriteMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x3D0, aimBot_ViewAngles.X);
					Memory::WriteMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x3D4, aimBot_ViewAngles.Y);
					Memory::WriteMem<float>(DataManger::hProcess, pCCameraPTR_0 + 0x3D8, aimBot_ViewAngles.Z);
				}

			}
		}
		


	}
	
}
