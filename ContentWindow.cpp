#include "ContentWindow.h"

using Microsoft::WRL::ComPtr;

ContentWindow::ContentWindow(int width, int height, const char* name) noexcept :
	WindowBase(width, height, name),
	m_layout(nullptr),
	m_mouseState(nullptr)
{
	// Create the device resources
	m_deviceResources = std::make_unique<DeviceResources>(m_hWnd);
	m_deviceResources->OnResize(); // Calling OnResize will create the render target, etc.

	// Create the layout to fill the entire screen with a single grid square
	m_layout = std::make_shared<Layout>(0.0f, 0.0f, Height(), Width());

	// Create the mouse state object
	m_mouseState = std::make_shared<MouseState>();

	// Try to intialize the theme manager - will only actually initialize if it was the first window
	ThemeManager::Initialize(D2DRenderTarget());

}

LRESULT ContentWindow::OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	return 0;
}

LRESULT ContentWindow::OnDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	DiscardGraphicsResources();
	return 0;
}

LRESULT ContentWindow::OnLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Call SetCapture to continue to get WM_MOUSEMOVE messages even if the mouse goes off the client area
	SetCapture(hWnd);

	const POINTS pt = MAKEPOINTS(lParam);

	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->LButtonDown(true);

	// Pass the coordinates of the mouse click to the main layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonDown(m_mouseState, true);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Call ReleaseCapture so the application does not get WM_MOUSEMOVE messages if the mouse if off screen
	ReleaseCapture();

	const POINTS pt = MAKEPOINTS(lParam);

	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->LButtonDown(false);

	// Pass the coordinates of the mouse click to the main layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonUp(m_mouseState, true);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->MButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->MButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->RButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->RButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->LButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT ContentWindow::OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	auto renderTarget = this->D2DRenderTarget();

	// If render target is nullptr, just return 0 - THIS SHOULD THROW AN EXCEPTION ====================
	if (renderTarget == nullptr)
	{
		return 0;
	}

	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	renderTarget->BeginDraw();
	
	renderTarget->Clear(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));
	//renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
	
	m_layout->OnPaint(renderTarget.Get());

	HRESULT hr = renderTarget->EndDraw();

	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
	{
		DiscardGraphicsResources();
	}
	EndPaint(hWnd, &ps);

	return 0;
}
LRESULT ContentWindow::OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Resize the device resources
	m_deviceResources->OnResize();

	// Update the Layout
	m_layout->OnResize(0.0f, 0.0f, Height(), Width());

	// Invalidate window so it redraws the whole client area
	InvalidateRect(hWnd, NULL, FALSE);

	return 0;
}

LRESULT ContentWindow::OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pts = MAKEPOINTS(lParam);
	std::ostringstream oss;
	oss << "(" << pts.x << ", " << pts.y << ") - Main";
	SetWindowText(hWnd, oss.str().c_str());

	// ====================================

	// Make sure mouse tracking is enabled
	m_mouseState->EnableMouseTracking(m_hWnd);

	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(pt.x);
	m_mouseState->Y(pt.y);
	m_mouseState->MButtonDown(true);


	// Pass the coordinates of the mouse click to the main layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseMove(m_mouseState, true);
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);


	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Mouse is no longer over client area, so disable mouse tracking
	m_mouseState->DisableMouseTracking();

	// Pass the OnMouseLeave event to the layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseLeave(true);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);


	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ContentWindow::DiscardGraphicsResources()
{
	
}

float ContentWindow::Height()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return static_cast<float>(rect.bottom);
}
float ContentWindow::Width()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	return static_cast<float>(rect.right);
}

ComPtr<ID2D1HwndRenderTarget> ContentWindow::D2DRenderTarget()
{
	//return m_deviceResources->D2DRenderTarget();
	
	ComPtr<ID2D1HwndRenderTarget> renderTarget = m_deviceResources->D2DRenderTarget();

	// If render target is nullptr, try to call OnResize to create it
	if (renderTarget == nullptr)
	{
		m_deviceResources->OnResize();

		renderTarget = m_deviceResources->D2DRenderTarget();
	}

	return renderTarget;
	
}
