#pragma once
#include "pch.h"
#include "WindowBaseTemplate.h"

#include <sstream>

class SecondaryWindow : public WindowBaseTemplate<SecondaryWindow>
{
public:
	SecondaryWindow(int width, int height, const char* name) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
};
