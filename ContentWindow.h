#pragma once
#include "pch.h"
#include "WindowBase.h"
#include "DeviceResources.h"
#include "Layout.h"
#include "Theme.h"
#include "ThemeManager.h"
#include "MouseState.h"
#include "OnMessageResult.h"

#include <memory>
#include <map>
#include <string>


class ContentWindow : public WindowBase
{
public:
	ContentWindow(int width, int height, const char* name) noexcept;
	~ContentWindow();

	float Height();
	float Width();

	std::shared_ptr<Layout> GetLayout() { return m_layout; }
	std::shared_ptr<DeviceResources> GetDeviceResources() { return m_deviceResources; }
	// Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> D2DRenderTarget();
	
protected:
	LRESULT OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	LRESULT OnLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnLButtonDoubleClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	LRESULT OnPaint(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	LRESULT OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

private:
	void DiscardGraphicsResources();

	// Keep track of device resources for rendering to this window
	std::shared_ptr<DeviceResources> m_deviceResources;

	// Keep a single instance of drawing state info that will be set for each draw call
	Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;

	// Keep a single layout for structuring the window content
	std::shared_ptr<Layout> m_layout;

	// Keep track off mouse details to pass to layout/controls
	std::shared_ptr<MouseState> m_mouseState;




	//HCURSOR m_cursor;
};