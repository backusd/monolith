#pragma once
#include "pch.h"



class MouseState
{
public:
	MouseState();

	int X() { return m_x; }
	int Y() { return m_y; }

	bool LButtonDown() { return m_leftButtonDown; }
	bool MButtonDown() { return m_middleButtonDown; }
	bool RButtonDown() { return m_rightButtonDown; }

	void X(int x) { m_x = x; }
	void Y(int y) { m_y = y; }

	void LButtonDown(bool isDown) { m_leftButtonDown = isDown; }
	void MButtonDown(bool isDown) { m_middleButtonDown = isDown; }
	void RButtonDown(bool isDown) { m_rightButtonDown = isDown; }

	void EnableMouseTracking(HWND hWnd);
	void DisableMouseTracking();

private:
	int m_x;
	int m_y;
	bool m_leftButtonDown;
	bool m_middleButtonDown;
	bool m_rightButtonDown;
	bool m_mouseTrackingEnabled;
};