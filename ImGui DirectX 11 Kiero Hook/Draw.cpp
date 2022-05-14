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

	static bool esp_2D_ShowNPC_flag = true;
	static bool esp_2D_ShowbloodNum_flag = true;
	static bool esp_2D_ShowName_flag = true;
	static bool esp_2D_ShowPlayer_flag = true;
	static bool esp_2D_ShowBone_flag = true;

	static float color2d_ShowName_player[3] = { 252.0f / 255.f, 157.0f / 255.f,154.0f / 255.f};
	static float color2d_ShowName_NPC[3] = { 1,1,1 };

	static float color2d_player[3] = { 131.0f / 255.f,175.0f / 255.f,155.0f / 255.f };
	static float color2d_NPC[3] = { 1,1,1 };

	static float colorLine_player[3] = { 131.0f / 255.f,175.0f / 255.f,155.0f / 255.f };
	static float colorLine_NPC[3] = { 1,1,1 };

	static float colorBone_NPC[3] = { 1,1,1 };
	static float colorBone_player[3] = { 131.0f / 255.f,175.0f / 255.f,155.0f / 255.f };


	/*float gview_width = DataManger::windowData.Width / 2;
	float gview_height = DataManger::windowData.Height / 2;*/
	static bool ShowCrosshair = false;

	long long m_ped_factory = Memory::ReadMem<long long>(DataManger::hProcess,Globals::WorldPTR);
	long long m_local_ped = Memory::ReadMem<long long>(DataManger::hProcess,m_ped_factory + 0x08);

	// 自己坐标
	long long pCNavigation = Memory::ReadMem<long long>(DataManger::hProcess, m_local_ped + 0x30);
	ImVec3 myPosV3 = Memory::ReadMem<ImVec3>(DataManger::hProcess, pCNavigation + 0x50);

	/////////////////////////////////////////////////////////////////////

	// 玩家列表
	long long pPlayerListPTR = Memory::ReadMem<long long>(DataManger::hProcess, Globals::NetworkPlayerMgrPTR);
	int playerCount = Memory::ReadMem<int>(DataManger::hProcess, pPlayerListPTR + 0x178);

	// Ped数量
	long long m_replay = Memory::ReadMem<long long>(DataManger::hProcess, Globals::ReplayInterfacePTR);
	long long m_ped_interface = Memory::ReadMem<long long>(DataManger::hProcess, m_replay + 0x18);
	int m_max_peds = Memory::ReadMem<int>(DataManger::hProcess, m_ped_interface + 0x108);
	int m_cur_peds = Memory::ReadMem<int>(DataManger::hProcess, m_ped_interface + 0x110);

	if (Setting::mainBeginSate)
	{
		ShowCursor(true);

		ImGui::Begin(u8"GTAV透视  支持最新版本线上      By:Mr.Li");

		ImGui::Checkbox(u8"鼠标操作-影响游戏界面     ", &Setting::checkBoxFalgs_1); ImGui::SameLine();
		CString tmpste = (Setting::checkBoxFalgs_1) ? "ON" : "OFF";
		ImGui::Text(u8"[ %S ]", tmpste);
		
		static bool tab_bar_flags = true;

		

		if (ImGui::BeginTabBar(u8"mytab", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(u8"ESP"))
			{
				
				ImGui::Checkbox(u8"开启ESP", &esp_flag);

				if (esp_flag)
				{

					ImGui::Checkbox(u8"显示玩家", &esp_2D_ShowPlayer_flag);

					ImGui::Checkbox(u8"显示NPC", &esp_2D_ShowNPC_flag);

					ImGui::Checkbox(u8"显示血条", &esp_2D_ShowBlood_flag);
					

					ImGui::Checkbox(u8"显示数字血量", &esp_2D_ShowbloodNum_flag);

					ImGui::Checkbox(u8"显示名称", &esp_2D_ShowName_flag);

					if (esp_2D_ShowName_flag || esp_2D_ShowbloodNum_flag)
					{
						ImGui::ColorEdit3(u8"玩家颜色", color2d_ShowName_player);
						ImGui::ColorEdit3(u8"NPC颜色", color2d_ShowName_NPC);
						ImGui::Text(u8"");
					}


					ImGui::Checkbox(u8"显示2D方框", &esp_2D_flag);
					if (esp_2D_flag)
					{
						ImGui::ColorEdit3(u8"玩家方框颜色", color2d_player);
						ImGui::ColorEdit3(u8"NPC方框颜色", color2d_NPC);
						ImGui::Text(u8"");
					}
					

					ImGui::Checkbox(u8"显示射线", &esp_2D_Show2DLine_flag);
					if (esp_2D_Show2DLine_flag)
					{
						ImGui::ColorEdit3(u8"玩家射线颜色", colorLine_player);
						ImGui::ColorEdit3(u8"NPC射线颜色", colorLine_NPC);
						ImGui::Text(u8"");
					}
					

					ImGui::Checkbox(u8"显示身体", &esp_2D_ShowBone_flag);
					if (esp_2D_ShowBone_flag)
					{
						ImGui::ColorEdit3(u8"玩家身体颜色", colorBone_player);
						ImGui::ColorEdit3(u8"NPC身体颜色", colorBone_NPC);
					}
					

					if(ImGui::Button(u8"恢复默认颜色"))
					{
						//玩家名称颜色//IMCOLOR_浅红色 ImColor(252,157,154)
						color2d_ShowName_player[0] = 252.0f / 255.f;
						color2d_ShowName_player[1] = 157.0f / 255.f;
						color2d_ShowName_player[2] = 154.0f / 255.f;

						//NPC名称颜色///白色
						color2d_ShowName_NPC[0] = 255.0f / 255.f;
						color2d_ShowName_NPC[1] = 255.0f / 255.f;
						color2d_ShowName_NPC[2] = 255.0f / 255.f;


						//身体颜色//白色
						colorBone_NPC[0] = 255.0f / 255.f;
						colorBone_NPC[1] = 255.0f / 255.f;
						colorBone_NPC[2] = 255.0f / 255.f;

						//NPC射线颜色//白色
						colorLine_NPC[0] = 255.0f / 255.f;
						colorLine_NPC[1] = 255.0f / 255.f;
						colorLine_NPC[2] = 255.0f / 255.f;

						//wanjia射线颜色//白色
						colorLine_player[0] = 131.0f / 255.f;
						colorLine_player[1] = 175.0f / 255.f;
						colorLine_player[2] = 155.0f / 255.f;

						//玩家方框颜色//IMCOLOR_淡青色 ImColor(131,175,155)
						color2d_player[0] = 131.0f / 255.f;
						color2d_player[1] = 175.0f / 255.f;
						color2d_player[2] = 155.0f / 255.f;

						//NPC方框颜色//IMCOLOR_白色 ImColor(255,255,255)
						color2d_NPC[0] = 255.0f / 255.f;
						color2d_NPC[1] = 255.0f / 255.f;
						color2d_NPC[2] = 255.0f / 255.f;

						//玩家身体颜色///IMCOLOR_淡青色 ImColor(131,175,155)
						colorBone_player[0] = 131.0f / 255.f;
						colorBone_player[1] = 175.0f / 255.f;
						colorBone_player[2] = 155.0f / 255.f;

					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"AimBot"))
			{
				
				ImGui::Checkbox(u8"开启AimBot", &Setting::Aimbot_Falgs);
				ImGui::Checkbox(u8"显示准星", &ShowCrosshair);

				

				if (Setting::Aimbot_Falgs)
				{
					const char* items[] = {u8"CTRL",u8"鼠标左键",u8"鼠标右键",u8"ALT",u8"SHIFT"};
					static int item_current_idx = 0; // Here we store our selection data as an index.
					ImGui::Combo("AimBot 按键", &item_current_idx, items, IM_ARRAYSIZE(items));
					switch (item_current_idx)
					{
					case 0:
						Setting::AimBot_Key = VK_CONTROL;
						break;
					case 1:
						Setting::AimBot_Key = VK_LBUTTON;
						break;
					case 2:
						Setting::AimBot_Key = VK_RBUTTON;
						break;
					case 3:
						Setting::AimBot_Key = VK_MENU;
						break;
					case 4:
						Setting::AimBot_Key = VK_SHIFT;
						break;
					default:
						break;
					}

					ImGui::SliderFloat(u8"AimBot范围", &Setting::AimBot_Fov, 0.1, 1300);
					ImGui::Checkbox(u8"显示范围", &Setting::Aimbot_ShowFov);
					ImGui::Checkbox(u8"射击NPC", &Setting::AimBot_ShootNPC);
					ImGui::Checkbox(u8"射击玩家", &Setting::AimBot_ShootPlay);
				}
				

				ImGui::EndTabItem();
			}







			ImGui::EndTabBar();
		}
		

		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (ShowCrosshair)
	{
		DrawCrosshair(IMCOLOR_淡青色, 12.0f);
	}
	
	char TT[256] = { 0 };
	sprintf_s(TT, u8"%.1f FPS\n玩家数量%d \nPed数量:%d\nW:%d H:%d", ImGui::GetIO().Framerate,playerCount, m_cur_peds,gview_width,gview_height);
	ImGui::GetForegroundDrawList()->AddText(font, font->FontSize,ImVec2(10,100), IMCOLOR_深红色, TT);


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

		// 如果是自己，跳过
		if (m_local_ped == m_ped_list)
			continue;

		// 如果ped死亡，跳过
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
		// 绘制玩家
		if (!esp_2D_ShowPlayer_flag)
		{
			if (strcmp(pedName, "")!=0)
			{
				continue;
			}
		}

		// 绘制Ped
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

		// ped坐标
		ImVec3 pedPosV3 = Memory::ReadMem<ImVec3>(DataManger::hProcess,m_navigation + 0x50);

		// Ped与自己的距离
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
			if (strcmp(pedName, "") != 0) //玩家
			{
				// 骨骼 0 : 头 7:脖子 8：臀部  5 ，6 手 3,4脚踝，1 2 脚指头
				DrawBone(m_ped_list, 0, 0, FLOATCOLORTOIMCOLOR(colorBone_player));
				DrawBone(m_ped_list, 0, 7, FLOATCOLORTOIMCOLOR(colorBone_player));
				DrawBone(m_ped_list, 7, 8, FLOATCOLORTOIMCOLOR(colorBone_player));
				DrawBone(m_ped_list, 8, 3, FLOATCOLORTOIMCOLOR(colorBone_player));
				DrawBone(m_ped_list, 8, 4, FLOATCOLORTOIMCOLOR(colorBone_player));
				DrawBone(m_ped_list, 7, 5, FLOATCOLORTOIMCOLOR(colorBone_player));
				DrawBone(m_ped_list, 7, 6, FLOATCOLORTOIMCOLOR(colorBone_player));
			}
			else
			{
				// 骨骼 0 : 头 7:脖子 8：臀部  5 ，6 手 3,4脚踝，1 2 脚指头
				DrawBone(m_ped_list, 0, 0, FLOATCOLORTOIMCOLOR(colorBone_NPC));
				DrawBone(m_ped_list, 0, 7, FLOATCOLORTOIMCOLOR(colorBone_NPC));
				DrawBone(m_ped_list, 7, 8, FLOATCOLORTOIMCOLOR(colorBone_NPC));
				DrawBone(m_ped_list, 8, 3, FLOATCOLORTOIMCOLOR(colorBone_NPC));
				DrawBone(m_ped_list, 8, 4, FLOATCOLORTOIMCOLOR(colorBone_NPC));
				DrawBone(m_ped_list, 7, 5, FLOATCOLORTOIMCOLOR(colorBone_NPC));
				DrawBone(m_ped_list, 7, 6, FLOATCOLORTOIMCOLOR(colorBone_NPC));
			}
			
		}

		if (!Helpers::IsNullVector2(pedPosV2))
		{
			if (esp_2D_flag)
			{
				if (strcmp(pedName, "") != 0) //玩家
					Draw2DBox(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_player));
				else//NPC
					Draw2DBox(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_NPC));
			}

			if (esp_2D_Show2DLine_flag)
			{
				if (strcmp(pedName, "") != 0) //玩家
					Draw2DLine(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(colorLine_player));
				else//NPC
					Draw2DLine(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(colorLine_NPC));
			}

			if (esp_2D_ShowBlood_flag)
			{
				Draw2DHealthBar(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_player), IMCOLOR_深红色, ped_HPPercentage);
			}

			if (esp_2D_ShowbloodNum_flag)
			{
				if (strcmp(pedName, "") != 0) //玩家
					Draw2DHealthText(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_ShowName_player), ped_Health, ped_MaxHealth, i);
				else//NPC
					Draw2DHealthText(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_ShowName_NPC), ped_Health, ped_MaxHealth, i);
			}

			if (esp_2D_ShowName_flag)
			{
				if (strcmp(pedName, "") != 0) //玩家
					Draw2DNameText(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_ShowName_player), pedName, myToPedDistance);
				else//NPC
					Draw2DNameText(pedPosV2, pedBoxV2, FLOATCOLORTOIMCOLOR(color2d_ShowName_NPC), pedName, myToPedDistance);
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
	auto V3Bone0 = Helpers::GetBonePosition(offset, bone0);
	auto V3Bone1 = Helpers::GetBonePosition(offset, bone1);
	ImVec2 v2Bone0 = Helpers::WorldToScreen(V3Bone0);
	ImVec2 v2Bone1 = Helpers::WorldToScreen(V3Bone1);

	if (!Helpers::IsNullVector2(v2Bone0) && !Helpers::IsNullVector2(v2Bone1))
	{
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(v2Bone0.x, v2Bone0.y), ImVec2(v2Bone1.x, v2Bone1.y), col);

		if (bone0 == 0 && bone1 == 0)
		{
			ImGui::GetForegroundDrawList()->AddCircle(v2Bone0, Helpers::GetBoxWH(V3Bone0).x/9, col,20);
		}
		
	}
}

void Draw::Draw2DHealthText(const ImVec2& screenV2, const ImVec2& boxV2, ImColor col,float health, float maxHealth, int index)
{
	char TT[50] = { 0 };
	sprintf_s(TT, u8"[%d] HP:%.0f/%.0f", index,health, maxHealth);
	ImGui::GetForegroundDrawList()->AddText(font, font->FontSize, ImVec2(screenV2.x + boxV2.x / 2 + boxV2.x / 8 - boxV2.x / 10, screenV2.y - boxV2.y / 2),
		col, TT);
}

void Draw::DrawCrosshair(ImColor col, float length)
{
	
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(gview_width - length, gview_height), ImVec2(gview_width + length, gview_height), col);

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(gview_width, gview_height - length), ImVec2(gview_width, gview_height + length), col);

}

