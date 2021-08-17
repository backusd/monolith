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

void ContentWindow::InitializeSimulation()
{
	SimulationManager::CreateSimulation(m_deviceResources);
}

void ContentWindow::Update()
{
	m_timer.Tick([&]() 
		{
			// Pass the Update call along to the layout, which will pass it along to each child control
			m_layout->Update(m_timer);

			// There should only be a single simulation which is accessible via SimulationManager
			// Because multiple controls may need to read from the simulation data, the update to the simulation
			// must come from the main window, so that multiple controls don't try updating the simulation
			SimulationManager::Update(m_timer);		
		}
	);
	
}

bool ContentWindow::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
		return false;

	// Pass the Render call along to the layout, which will pass it along to each child control
	// return m_layout->Render();

	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	//D3D11_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
	//context->RSSetViewports(1, &viewport);
	m_deviceResources->ResetViewport();

	FLOAT background[4] = { 45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f };
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), background);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RenderTargetView* const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Draw all 3D simulation controls first
	m_layout->Render3DControls();

	// Draw all 2D / Menu controls next
	m_deviceResources->ResetViewport();

	ID2D1DeviceContext* context2 = m_deviceResources->D2DDeviceContext();
	context2->SaveDrawingState(m_stateBlock.Get());
	context2->BeginDraw();
	context2->SetTransform(m_deviceResources->OrientationTransform2D());

	m_layout->Render2DControls();


	HRESULT hr = context2->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
	{
		DiscardGraphicsResources();
	}

	context2->RestoreDrawingState(m_stateBlock.Get());

	return true;
}

void ContentWindow::Present()
{
	// Present the render target to the screen
	m_deviceResources->Present();
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

	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->LButtonDown(true);

	// Pass the coordinates of the mouse click to the main layout
	m_layout->OnLButtonDown(m_mouseState);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonDown(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Call ReleaseCapture so the application does not get WM_MOUSEMOVE messages if the mouse if off screen
	ReleaseCapture();

	const POINTS pt = MAKEPOINTS(lParam);

	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->LButtonDown(false);

	// Pass the coordinates of the mouse click to the main layout
	m_layout->OnLButtonUp(m_mouseState);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonUp(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->LButtonDown(true);

	// Pass the coordinates of the mouse click to the main layout
	m_layout->OnLButtonDoubleClick(m_mouseState);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnLButtonDoubleClick(m_mouseState);
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->MButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->MButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->RButtonDown(true);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pt = MAKEPOINTS(lParam);
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));
	m_mouseState->RButtonDown(false);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


LRESULT ContentWindow::OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	/*
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
	*/

	// Return the default window procedure otherwise an endless stream of WM_PAINT
	// messages will be generated because it thinks the window was not painted
	return DefWindowProc(hWnd, msg, wParam, lParam);
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
	m_mouseState->X(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.x)));
	m_mouseState->Y(static_cast<int>(m_deviceResources->PixelsToDIPS(pt.y)));

	// Pass the coordinates of the mouse to the main layout
	m_layout->OnMouseMove(m_mouseState);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseMove(m_mouseState);
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Mouse is no longer over client area, so disable mouse tracking
	m_mouseState->DisableMouseTracking();

	// Pass the OnMouseLeave event to the layout
	m_layout->OnMouseLeave();
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseLeave();
	if (result != nullptr && result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	const POINTS pts = MAKEPOINTS(lParam);
	int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	// if the wheel Delta is 1/-1, it is from the mouse pad, so just elevate it to 120
	if (wheelDelta == 1 || wheelDelta == -1)
		wheelDelta *= 120;

	// Pass the wheel delta to main layout / controls
	m_layout->OnMouseWheel(wheelDelta);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnMouseWheel(wheelDelta);
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

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
	lpMMI->ptMinTrackSize.x = static_cast<LONG>(m_deviceResources->DIPSToPixels(500));
	lpMMI->ptMinTrackSize.y = static_cast<LONG>(m_deviceResources->DIPSToPixels(300));

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT ContentWindow::OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//static std::string title;
	//title.push_back((char)wParam);
	//SetWindowText(hWnd, title.c_str());

	// Pass the char to main layout / controls
	m_layout->OnChar((char)wParam);
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnChar((char)wParam);
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_layout->OnKeyUp(static_cast<unsigned char>(wParam));
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnKeyUp(static_cast<unsigned char>(wParam));
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT ContentWindow::OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	m_layout->OnKeyDown(static_cast<unsigned char>(wParam));
	/*
	std::shared_ptr<OnMessageResult> result = m_layout->OnKeyDown(static_cast<unsigned char>(wParam));
	if (result->Redraw())
		InvalidateRect(hWnd, NULL, FALSE);
	*/

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

