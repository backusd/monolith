#include "pch.h"
//#include <Windows.h>
#include <sstream>

#include "WindowsMessageMap.h"
#include "WindowManager.h"
#include "ContentWindow.h"
#include "LayoutConfig.h"
#include "ThemeManager.h"

#include <memory>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, wParam, lParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_LBUTTONDOWN:
		const POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << ", " << pt.y << ")";
		SetWindowText(hWnd, oss.str().c_str());
	}



	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// All other hard coded pixel values will be in DIPS. However, because we don't have access to the D2DFactory
	// at the time of window creation (that could probably be changed), these values are physical pixels for the window size
	std::shared_ptr<ContentWindow> main = std::make_shared<ContentWindow>(800, 300, "Main Window");
	LayoutConfiguration::ConfigureMainWindow(main);

	WindowManager::AddWindow(main);

	ThemeManager::Initialize(main->GetDeviceResources());

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// check if GetMessage call itself borked
	if (gResult == -1)
	{
		return -1;
	}

	// Destroy Theme resources
	ThemeManager::Destroy();

	// wParam here is the value passed to PostQuitMessage
	return static_cast<int>(msg.wParam);
}