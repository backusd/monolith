#include "DPIHelper.h"


DPIHelper::DPIHelper(HWND hWnd) :
	dpiScaleX(1.0f),
	dpiScaleY(1.0f)
{
	FLOAT dpi = static_cast<float>(GetDpiForWindow(hWnd));

	dpiScaleX = dpi / 96.0f;
	dpiScaleY = dpi / 96.0f;
}