#include "Draw.h"
#include "cmath"

extern ImFont* font;
extern HMODULE g_hDll;

ImVec4 Create(float x, float y, float width, float height)
{
	return ImVec4(x, y, x + width, y + height);
}

void Draw::DrawWin()
{

	static bool esp_flag = true;

	static bool esp_2D_flag = true;
	static bool esp_2D_ShowBlood_flag = true;
	static bool esp_2D_Show2DLine_flag = true;

	static bool esp_2D_ShowNPC_flag = false;
	static bool esp_2D_ShowbloodNum_flag = true;
	static bool esp_2D_ShowName_flag = false;
	static bool esp_2D_ShowPlayer_flag = false;
	static bool esp_2D_ShowBone_flag = true;

	/*float gview_width = DataManger::windowData.Width / 2;
	float gview_height = DataManger::windowData.Height / 2;*/


	long long m_ped_factory = Memory::ReadMem<long long>(DataManger::hProcess,Globals::WorldPTR);
	long long m_local_ped = Memory::ReadMem<long long>(DataManger::hProcess,m_ped_factory + 0x08);

	// �Լ�����
	long long pCNavigation = Memory::ReadMem<long long>(DataManger::hProcess, m_local_ped + 0x30);
	ImVec3 myPosV3 = Memory::ReadMem<ImVec3>(DataManger::hProcess, pCNavigation + 0x50);

	/////////////////////////////////////////////////////////////////////

	// ����б�
	long long pPlayerListPTR = Memory::ReadMem<long long>(DataManger::hProcess, Globals::NetworkPlayerMgrPTR);
	int playerCount = Memory::ReadMem<int>(DataManger::hProcess, pPlayerListPTR + 0x178);

	// Ped����
	long long m_replay = Memory::ReadMem<long long>(DataManger::hProcess, Globals::ReplayInterfacePTR);
	long long m_ped_interface = Memory::ReadMem<long long>(DataManger::hProcess, m_replay + 0x18);
	int m_max_peds = Memory::ReadMem<int>(DataManger::hProcess, m_ped_interface + 0x108);
	int m_cur_peds = Memory::ReadMem<int>(DataManger::hProcess, m_ped_interface + 0x110);

	if (Setting::mainBeginSate)
	{
		ShowCursor(true);

		ImGui::Begin(u8"GTAV͸��  ֧�����°汾����      By:Mr.Li");
		ImGui::Checkbox(u8"������-Ӱ����Ϸ����     ", &Setting::checkBoxFalgs_1); ImGui::SameLine();
		CString tmpste = (Setting::checkBoxFalgs_1) ? "ON" : "OFF";
		ImGui::Text(u8"[ %S ]", tmpste);
		ImGui::Checkbox(u8"����ESP", &esp_flag);

		if (esp_flag)
		{
			ImGui::Checkbox(u8"��ʾ2D����", &esp_2D_flag);
			ImGui::Checkbox(u8"��ʾ����", &esp_2D_Show2DLine_flag);
			ImGui::Checkbox(u8"��ʾ����", &esp_2D_ShowBone_flag);
			ImGui::Checkbox(u8"��ʾѪ��", &esp_2D_ShowBlood_flag);
			ImGui::Checkbox(u8"��ʾ����Ѫ��", &esp_2D_ShowbloodNum_flag);
			ImGui::Checkbox(u8"��ʾ����", &esp_2D_ShowName_flag);
			ImGui::Checkbox(u8"��ʾ���", &esp_2D_ShowPlayer_flag);
			ImGui::Checkbox(u8"��ʾNPC", &esp_2D_ShowNPC_flag);

			ImGui::Checkbox(u8"����AimBot", &Setting::Aimbot_Falgs);
			ImGui::SliderFloat(u8"AimBot��Χ", &Setting::AimBot_Fov,0.1,1080);
			ImGui::Checkbox(u8"��ʾ��Χ", &Setting::Aimbot_ShowFov);
			ImGui::Checkbox(u8"���NPC", &Setting::AimBot_ShootNPC);
			ImGui::Checkbox(u8"������", &Setting::AimBot_ShootPlay);

			static float a[3] = { 0 };
			ImGui::ColorEdit3(u8"color ��ɫ", a);

		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
		
	
	char TT[256] = { 0 };
	sprintf_s(TT, u8"�������%d \nPed����:%d\nX:%.2f\nY:%.2f\nZ:%.2f", playerCount, m_cur_peds, myPosV3.X, myPosV3.Y, myPosV3.Z);
	ImGui::GetForegroundDrawList()->AddText(font, font->FontSize,ImVec2(10,100), IMCOLOR_���ɫ, TT);


	if (Setting::Aimbot_ShowFov)
	{
		ImGui::GetForegroundDrawList()->AddCircle(ImVec2(gview_width, gview_height), Setting::AimBot_Fov, IMCOLOR_WHITE,100);
		
	}


	///////////////////////////////////////////////////////

	for (int i = 0; i < m_max_peds; i++)
	{
		long long m_ped_list = Memory::ReadMem<long long>(DataManger::hProcess,m_ped_interface + 0x100);
		m_ped_list = Memory::ReadMem<long long>(DataManger::hProcess,m_ped_list + i * 0x10);
		if (!Helpers::IsValid(m_ped_list))
			continue;

		// ������Լ�������
		if (m_local_ped == m_ped_list)
			continue;

		// ���ped����������
		float ped_Health = Memory::ReadMem<float>(DataManger::hProcess, m_ped_list + 0x280);
		if (ped_Health <= 0)
			continue;

		float ped_MaxHealth = Memory::ReadMem<float>(DataManger::hProcess,m_ped_list + 0x2A0);
		float ped_HPPercentage = ped_Health / ped_MaxHealth;

		long long m_player_info = Memory::ReadMem<long long>(DataManger::hProcess, m_ped_list + 0x10C8);
			
		char* pedNameC = Memory::ReadMem<char>(DataManger::hProcess, m_player_info + 0xA4, 20);
		
		char pedName[22]={0};
		if (m_player_info == 0)
		{
			strcpy_s(pedName, strlen("") + 1, "");
		}
		else
		{
			strcpy_s(pedName, strlen(pedNameC) + 1, pedNameC);
		}
		delete[] pedNameC;
		// �������
		if (!esp_2D_ShowPlayer_flag)
		{
			if (strcmp(pedName, "")!=0)
			{
				continue;
			}
		}

		// ����Ped
		if (!esp_2D_ShowNPC_flag)
		{
			if (strcmp(pedName, "") == 0)
			{
				continue;
			}
		}

		long long m_navigation = Memory::ReadMem<long long>(DataManger::hProcess, m_ped_list + 0x30);
		if (!Helpers::IsValid(m_navigation))
			continue;

		// ped����
		ImVec3 pedPosV3 = Memory::ReadMem<ImVec3>(DataManger::hProcess,m_navigation + 0x50);

		// Ped���Լ��ľ���
		float myToPedDistance = (float)sqrt(
			pow(myPosV3.X - pedPosV3.X, 2) +
			pow(myPosV3.Y - pedPosV3.Y, 2) +
			pow(myPosV3.Z - pedPosV3.Z, 2));

		// m_heading    0x20
		// m_heading2   0x24
		ImVec2 v2PedSinCos =ImVec2
		(
			Memory::ReadMem<float>(DataManger::hProcess, m_navigation + 0x20),
			Memory::ReadMem<float>(DataManger::hProcess, m_navigation + 0x30)
		);

		ImVec2 pedPosV2 = Helpers::WorldToScreen(pedPosV3);
		ImVec2 pedBoxV2 = Helpers::GetBoxWH(pedPosV3);

		if (esp_2D_ShowBone_flag)
		{
			// ����
			DrawBone(m_ped_list, 0, 7, IMCOLOR_WHITE);
			DrawBone(m_ped_list, 7, 8, IMCOLOR_WHITE);
			DrawBone(m_ped_list, 8, 3, IMCOLOR_WHITE);
			DrawBone(m_ped_list, 8, 4, IMCOLOR_WHITE);
			DrawBone(m_ped_list, 7, 5, IMCOLOR_WHITE);
			DrawBone(m_ped_list, 7, 6, IMCOLOR_WHITE);
		}

		if (!Helpers::IsNullVector2(pedPosV2))
		{
			if (esp_2D_flag)
			{
				Draw2DBox(pedPosV2, pedBoxV2, IMCOLOR_����ɫ);
			}

			if (esp_2D_Show2DLine_flag)
			{
				Draw2DLine(pedPosV2, pedBoxV2, IMCOLOR_WHITE);
			}

			if (esp_2D_ShowBlood_flag)
			{
				Draw2DHealthBar(pedPosV2, pedBoxV2, IMCOLOR_����ɫ, IMCOLOR_���ɫ, ped_HPPercentage);
			}

			if (esp_2D_ShowbloodNum_flag)
			{
				Draw2DHealthText(pedPosV2, pedBoxV2, IMCOLOR_ǳ��ɫ, ped_Health, ped_MaxHealth, i);

			}

			if (esp_2D_ShowName_flag)
			{
				Draw2DNameText(pedPosV2, pedBoxV2, IMCOLOR_ǳ��ɫ, pedName, myToPedDistance);
			}

		}
		
	}

}


void Draw::Draw2DBox(const ImVec2& screenV2, const ImVec2& boxV2, ImColor col)
{
	auto rect = Create(screenV2.x - boxV2.x / 2,
		screenV2.y - boxV2.y / 2,
		boxV2.x,
		boxV2.y);

	ImGui::GetForegroundDrawList()->AddRect(ImVec2(rect.x,rect.y), ImVec2(rect.z, rect.w), col);
}

void Draw::Draw2DHealthBar(const ImVec2& screenV2, const ImVec2& boxV2, ImColor RectCol, ImColor fillCol, float pedHPPercentage)
{

	auto A = Create(screenV2.x - boxV2.x / 2 - boxV2.x / 8,
		screenV2.y + boxV2.y / 2,
		boxV2.x / 10,
		boxV2.y * -1.0f);

	auto B = Create(screenV2.x - boxV2.x / 2 - boxV2.x / 8,
		screenV2.y + boxV2.y / 2,
		boxV2.x / 10,
		boxV2.y * pedHPPercentage * -1.0f);

	ImGui::GetForegroundDrawList()->AddRect(ImVec2(A.x, A.y),
		ImVec2(A.z,A.w), RectCol);

	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(B.x, B.y),
		ImVec2(B.z, B.w), fillCol);
}

void Draw::Draw2DLine(const ImVec2& screenV2, const ImVec2& boxV2, ImColor col)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(WindowData_Width / 2, 0), ImVec2(screenV2.x, screenV2.y - boxV2.y / 2),
		col);
}

void Draw::Draw2DNameText(const ImVec2& screenV2, const ImVec2& boxV2, ImColor col,const char* name, float distance)
{
	
	char TT[256]  = { 0 };
	sprintf_s(TT, u8"\n[%.1fm] ID:%s", distance, name);
	ImGui::GetForegroundDrawList()->AddText(font, font->FontSize,ImVec2(screenV2.x + boxV2.x / 2 + boxV2.x / 8 - boxV2.x / 10, screenV2.y - boxV2.y / 2),
		col,TT);

}

void Draw::DrawBone(long long offset, int bone0, int bone1, ImColor col)
{
	ImVec2 v2Bone0 = Helpers::WorldToScreen(Helpers::GetBonePosition(offset, bone0));
	ImVec2 v2Bone1 = Helpers::WorldToScreen(Helpers::GetBonePosition(offset, bone1));

	if (!Helpers::IsNullVector2(v2Bone0) && !Helpers::IsNullVector2(v2Bone1))
	{
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(v2Bone0.x, v2Bone0.y), ImVec2(v2Bone1.x, v2Bone1.y), col);
		
	}
}

void Draw::Draw2DHealthText(const ImVec2& screenV2, const ImVec2& boxV2, ImColor col,float health, float maxHealth, int index)
{
	char TT[50] = { 0 };
	sprintf_s(TT, u8"[%d] HP:%.0f/%.0f", index,health, maxHealth);
	ImGui::GetForegroundDrawList()->AddText(font, font->FontSize, ImVec2(screenV2.x + boxV2.x / 2 + boxV2.x / 8 - boxV2.x / 10, screenV2.y - boxV2.y / 2),
		col, TT);
}

