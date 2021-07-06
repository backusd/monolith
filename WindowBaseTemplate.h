#pragma once
#include "pch.h"


template<typename T>
class WindowBaseTemplate
{
public:
	WindowBaseTemplate(int width, int height, const char* name) noexcept;
	~WindowBaseTemplate();

	virtual LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}


protected:
	static LRESULT CALLBACK HandleMsgSetupBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	// Window Class Data
	static constexpr const char* wndBaseClassName = "Engine Window";
	HINSTANCE hInst;


	// Window Data
	int m_width;
	int m_height;
	HWND m_hWnd;


};

template<typename T>
WindowBaseTemplate<T>::WindowBaseTemplate(int width, int height, const char* name) noexcept :
	m_height(height),
	m_width(width),
	hInst(GetModuleHandle(nullptr))
{
	// Register the window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = HandleMsgSetupBase;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wndBaseClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	auto WS_options = WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | WS_MAXIMIZEBOX | WS_SIZEBOX;

	AdjustWindowRect(&wr, WS_options, FALSE);
	// create window & get hWnd
	m_hWnd = CreateWindow(
		wndBaseClassName, name,
		WS_options,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, hInst, this
	);
	// show window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
};

template<typename T>
WindowBaseTemplate<T>::~WindowBaseTemplate()
{
	UnregisterClass(wndBaseClassName, hInst);
	DestroyWindow(m_hWnd);
};

template<typename T>
LRESULT CALLBACK WindowBaseTemplate<T>::HandleMsgSetupBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		
		WindowBaseTemplate<T>* const pWnd = static_cast<WindowBaseTemplate<T>*>(pCreate->lpCreateParams);

		//T* const pWnd = static_cast<T*>(pCreate->lpCreateParams);
		//Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowBaseTemplate<T>::HandleMsgBase));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

template<typename T>
LRESULT CALLBACK WindowBaseTemplate<T>::HandleMsgBase(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	WindowBaseTemplate<T>* const pWnd = reinterpret_cast<WindowBaseTemplate<T>*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
