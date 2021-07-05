#include "MouseState.h"

MouseState::MouseState() :
	m_x(0),
	m_y(0),
	m_leftButtonDown(false),
	m_middleButtonDown(false),
	m_rightButtonDown(false),
	m_mouseTrackingEnabled(false)
{
}

void MouseState::EnableMouseTracking(HWND hWnd)
{
	if (!m_mouseTrackingEnabled)
	{
		m_mouseTrackingEnabled = true;

		TRACKMOUSEEVENT eventDetails;
		eventDetails.cbSize = sizeof(TRACKMOUSEEVENT);
		eventDetails.dwFlags = TME_LEAVE;
		eventDetails.dwHoverTime = 0;
		eventDetails.hwndTrack = hWnd;

		// Will be true on success
		m_mouseTrackingEnabled = TrackMouseEvent(&eventDetails);
	}
}

void MouseState::DisableMouseTracking()
{
	m_mouseTrackingEnabled = false;
}