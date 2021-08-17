#pragma once
#include "pch.h"

enum class OnMessageResult
{
	NONE,
	CAPTURE_MOUSE,
	MESSAGE_HANDLED,
	CAPTURE_MOUSE_AND_MESSAGE_HANDLED
};

/*

#include <memory>

class Control;

class OnMessageResult
{
public:
	OnMessageResult();
	~OnMessageResult();
	OnMessageResult(const OnMessageResult& rhs);
	OnMessageResult& operator=(const OnMessageResult& rhs);

	void Redraw(bool redraw) { m_redraw = redraw; }
	void CaptureMouse(bool capture) { m_captureMouse = capture; }
	void MessageHandled(bool handled) { m_messageHandled = handled; }

	bool Redraw() { return m_redraw; }
	bool CaptureMouse() { return m_captureMouse; }
	bool MessageHandled() { return m_messageHandled; }
	
private:
	bool m_redraw;
	bool m_captureMouse;
	bool m_messageHandled;
};

*/