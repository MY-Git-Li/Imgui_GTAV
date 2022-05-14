#pragma once
#include "includes.h"
#define FLOATCOLORTOIMCOLOR(name) ImColor(name[0] * 255.0f, name[1] * 255.0f, name[2] * 255.0f)
class Draw
{
public:
	
	static void DrawWin();
	static void Draw2DBox(const ImVec2& p_min, const ImVec2& p_max, ImColor col);
	static void Draw2DHealthBar(const ImVec2& p_min, const ImVec2& p_max, ImColor RectCol, ImColor fillCol ,float pedHPPercentage);
	static void Draw2DLine(const ImVec2& p_min, const ImVec2& p_max, ImColor col);
	static void Draw2DNameText(const ImVec2& p_min, const ImVec2& p_max, ImColor col,const char* name, float distance);
	static void DrawBone(long long offset, int bone0, int bone1, ImColor col);
	static void Draw2DHealthText(const ImVec2& screenV2, const ImVec2& boxV2, ImColor col,float health, float maxHealth, int index);
};

