#pragma once
#include "pch.h"
#include "DirectXHelper.h"


class DeviceResources
{
public:
	DeviceResources(HWND hWnd);
	~DeviceResources();

	// May need to expand this method...
	void OnResize() { CreateWindowSizeDependentResources(); }

	// Direct 2D drawing components
	ID2D1Factory7* D2DFactory() const { return m_d2dFactory.Get(); }
	ID2D1Device6* D2DDevice() const { return m_d2dDevice.Get(); }
	ID2D1DeviceContext6* D2DDeviceContext() const { return m_d2dDeviceContext.Get(); }
	ID2D1Bitmap1* D2DBitmap() const { return m_d2dBitmap.Get(); }

	// const Microsoft::WRL::ComPtr<ID2D1Factory7>& D2DFactory() { return m_d2dFactory; }
	// const Microsoft::WRL::ComPtr<ID2D1Device6>& D2DDevice() { return m_d2dDevice; }
	// const Microsoft::WRL::ComPtr<ID2D1DeviceContext6>& D2DDeviceContext() { return m_d2dDeviceContext; }
	// const Microsoft::WRL::ComPtr<ID2D1Bitmap1>& D2DBitmap() { return m_d2dBitmap; }
	D2D1::Matrix3x2F OrientationTransform2D() const { return m_orientationTransform2D; }

	// Direct Write drawing components
	IDWriteFactory7* DWriteFactory() const { return m_dwriteFactory.Get(); }
	IWICImagingFactory2* WICImagingFactory() const { return m_wicImagingFactory.Get(); }

	// const Microsoft::WRL::ComPtr<IDWriteFactory7>& DWriteFactory() { return m_dwriteFactory; }
	// const Microsoft::WRL::ComPtr<IWICImagingFactory2>& WICImagingFactory() { return m_wicImagingFactory; }

	// Direct3D objects
	ID3D11Device5* D3DDevice() const { return m_d3dDevice.Get(); }
	ID3D11DeviceContext4* D3DDeviceContext() const { return m_d3dDeviceContext.Get(); }

	// const Microsoft::WRL::ComPtr<ID3D11Device5>& D3DDevice() { return m_d3dDevice; }
	// const Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& D3DDeviceContext() { return m_d3dDeviceContext; }
	D3D_FEATURE_LEVEL D3DFeatureLevel() { return m_d3dFeatureLevel; }


	// ==========================================================
	// Temporary D2D Resources
	//const Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>& D2DRenderTarget() { return m_d2dRenderTarget; }

	// ==========================================================

	D3D11_VIEWPORT GetScreenViewport() const { return m_viewport; }
	void SetViewport(CD3D11_VIEWPORT viewport);
	void ResetViewport();

	ID3D11RenderTargetView1* GetBackBufferRenderTargetView() const { return m_d3dRenderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_d3dDepthStencilView.Get(); }
	void Present();

	// DPI Scaling functions
	float DIPSToPixels(float dips) { return dips * m_dpiScale; }
	float PixelsToDIPS(float pixels) { return pixels / m_dpiScale; }

private:

	void CreateDeviceDependentResources();
	void CreateDeviceIndependentResources();
	void CreateWindowSizeDependentResources();
	void HandleDeviceLost();

	// Keep handle to window
	HWND m_hWnd;

	// DPI scale for the window
	float m_dpiScale;

	// ==========================================================
	// Temporary D2D Resources
	//Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_d2dRenderTarget;

	// ==========================================================

	// Direct 2D drawing components
	Microsoft::WRL::ComPtr<ID2D1Factory7>		m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device6>		m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext6>	m_d2dDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dBitmap;

	// Direct Write drawing components
	Microsoft::WRL::ComPtr<IDWriteFactory7>		m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicImagingFactory;

	// Direct3D objects
	Microsoft::WRL::ComPtr<ID3D11Device5>		 m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4> m_d3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain4>		 m_dxgiSwapChain;

	// Direct3D Rendering objects ------ THESE MAY END UP GETTING STORED PER WINDOW
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>	m_d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_d3dDepthStencilView;

	D3D11_VIEWPORT m_viewport;
	
	// Cached device properties
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;

	// Orientation transforms
	D2D1::Matrix3x2F	m_orientationTransform2D;
	DirectX::XMFLOAT4X4 m_orientationTransform3D;

};