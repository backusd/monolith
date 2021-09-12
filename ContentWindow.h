#pragma once
#include "pch.h"
#include "WindowBase.h"
#include "DeviceResources.h"
#include "Layout.h"
#include "Theme.h"
#include "ThemeManager.h"
#include "MeshManager.h"
#include "MouseState.h"
#include "OnMessageResult.h"
#include "StepTimer.h"
#include "SimulationManager.h"
#include "Text.h"
#include "TextInput.h"
#include "ComboBox.h"
#include "Slider.h"

#include <memory>
#include <map>
#include <string>


class ContentWindow : public WindowBase
{
public:
	ContentWindow(int width, int height, const char* name);
	~ContentWindow();

	float Height();
	float Width();

	void InitializeSimulation() { SimulationManager::CreateSimulation(m_deviceResources); }
	void InitializeThemes() { ThemeManager::Initialize(m_deviceResources); }
	void InitializeMeshes() { MeshManager::CreateMeshes(m_deviceResources); }

	std::shared_ptr<Layout> GetLayout() { return m_layout; }
	std::shared_ptr<DeviceResources> GetDeviceResources() { return m_deviceResources; }
	
	void Update() override;
	bool Render() override;
	void Present() override;


	// Click methods
	void NewSimulationButtonClick() override;
	
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

	LRESULT OnResize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	
	LRESULT OnMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnMouseLeave(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	LRESULT OnGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

	LRESULT OnChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	LRESULT OnKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT OnKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

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


	StepTimer m_timer;
};