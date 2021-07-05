#include "SecondaryWindow.h"


SecondaryWindow::SecondaryWindow(int width, int height, const char* name) noexcept :
	WindowBaseTemplate(width, height, name)
{
}

LRESULT SecondaryWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		// Don't post quit - Just set the destroy window flag
		//PostQuitMessage(0);
		//destroyWindow = true;
		return 0;

	case WM_LBUTTONDOWN:
		const POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << ", " << pt.y << ") - Secondary";
		SetWindowText(hWnd, oss.str().c_str());
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}