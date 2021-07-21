#include "ContentWindow.h"

using Microsoft::WRL::ComPtr;

ContentWindow::ContentWindow(int width, int height, const char* name) :
	WindowBase(width, height, name),
	m_stateBlock(nullptr),
	m_layout(nullptr),
	m_mouseState(nullptr)
	//
	//
	//m_cursor(LoadCursor(NULL, ))
{
	// Create the device resources
	m_deviceResources = std::make_shared<DeviceResources>(m_hWnd);
	m_deviceResources->OnResize(); // Calling OnResize will create the render target, etc.

	// Create the state block 
	ThrowIfFailed(
		m_deviceResources->D2DFactory()->CreateDrawingStateBlock(m_stateBlock.GetAddressOf())
	);

	// Create the layout to fill the entire screen with a single grid square
	// Because the layout will be rendered using DIPS, we must pass the DIPS values for height/width
	m_layout = std::make_shared<Layout>(m_deviceResources, 0.0f, 0.0f, m_deviceResources->PixelsToDIPS(Height()), m_deviceResources->PixelsToDIPS(Width()));

	// Create the mouse state object
	m_mouseState = std::make_shared<MouseState>();

	// Try to intialize the theme manager - will only actually initialize if it was the first window
	
	//ThemeManager::Initialize(D2DRenderTarget());
	ThemeManager::Initialize(m_deviceResources);
}

ContentWindow::~ContentWindow()
{
	// Each control bound to the layout has a reference to it, so it cannot simply
	// be set to nullptr and destructed.
	m_layout->ClearContents();
	m_layout = nullptr;

	m_deviceResources = nullptr;
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

	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->LButtonDown(true);

	// Pass the coordinates of the mouse click to the main layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonDown(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Call ReleaseCapture so the application does not get WM_MOUSEMOVE messages if the mouse if off screen
	ReleaseCapture();

	const POINTS pt = MAKEPOINTS(lParam);

	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->LButtonDown(false);

	// Pass the coordinates of the mouse click to the main layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonUp(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->MButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->MButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->RButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->RButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));
	m_mouseState->LButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT ContentWindow::OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	D3D11_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView* const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	FLOAT background[4] = { 45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f };
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), background);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	ID2D1DeviceContext* context2 = m_deviceResources->D2DDeviceContext();
	context2->SaveDrawingState(m_stateBlock.Get());
	context2->BeginDraw();
	context2->SetTransform(m_deviceResources->OrientationTransform2D());


	m_layout->OnPaint();


	HRESULT hr = context2->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
	{
		DiscardGraphicsResources();
	}

	context2->RestoreDrawingState(m_stateBlock.Get());

	m_deviceResources->Present();

	// Return the default window procedure otherwise an endless stream of WM_PAINT
	// messages will be generated because it thinks the window was not painted
	return DefWindowProc(hWnd, msg, wParam, lParam);

	/*
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
	*/
}
LRESULT ContentWindow::OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Resize the device resources
	m_deviceResources->OnResize();

	// Update the Layout
	m_layout->OnResize(0.0f, 0.0f, m_deviceResources->PixelsToDIPS(Height()), m_deviceResources->PixelsToDIPS(Width()));

	// Invalidate window so it redraws the whole client area
	InvalidateRect(hWnd, NULL, FALSE);

	return 0;
}

LRESULT ContentWindow::OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pts = MAKEPOINTS(lParam);
	std::ostringstream oss;
	oss << "Pixels: (" << pts.x << ", " << pts.y << ") - DIPS: (" << m_deviceResources->PixelsToDIPS(pts.x) << ", " << m_deviceResources->PixelsToDIPS(pts.y) << ") - Main";
	SetWindowText(hWnd, oss.str().c_str());

	// ====================================

	// Change cursor to arrow


	// Make sure mouse tracking is enabled
	m_mouseState->EnableMouseTracking(m_hWnd);

	// Store the mouse location in DIPS
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(m_deviceResources->PixelsToDIPS(pt.x));
	m_mouseState->Y(m_deviceResources->PixelsToDIPS(pt.y));

	// Pass the coordinates of the mouse to the main layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseMove(m_mouseState);
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);


	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Mouse is no longer over client area, so disable mouse tracking
	m_mouseState->DisableMouseTracking();

	// Pass the OnMouseLeave event to the layout
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseLeave();
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);


	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ContentWindow::DiscardGraphicsResources()
{
	m_deviceResources = nullptr;
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

LRESULT ContentWindow::OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Minimums should be set in DIPS to be uniform across devices
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMinTrackSize.x = m_deviceResources->DIPSToPixels(500);
	lpMMI->ptMinTrackSize.y = m_deviceResources->DIPSToPixels(300);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT ContentWindow::OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static std::string title;
	title.push_back((char)wParam);
	SetWindowText(hWnd, title.c_str());

	return DefWindowProc(hWnd, msg, wParam, lParam);
}