#pragma once
#define WindowData_Height DataManger::GetGameWindowData().Height
#define WindowData_Width DataManger::GetGameWindowData().Width

#define gview_width WindowData_Width / 2
#define gview_height WindowData_Height /2
struct WindowData
{
	int Left;
	int Top;
	int Width;
	int Height;

	WindowData(int l, int t, int w, int h) { Left = l; Top = t; Width = w; Height = h; }
};
struct ImVec3
{
	float     X, Y, Z;
	ImVec3() { X = Y = Z = 0.0f; }
	ImVec3(float _x, float _y, float _z) { X = _x; Y = _y; Z = _z; }
#ifdef IM_VEC3_CLASS_EXTRA
	IM_VEC3_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imconfig.h to convert back and forth between your math types and ImVec4.
#endif
};
