#pragma once
#include "pch.h"

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
	void CaptureOverride(bool captureOverride) { m_captureOverride = captureOverride; }
	void MessageHandled(bool handled) { m_messageHandled = handled; }
	void CapturedControl(const std::shared_ptr<Control>& capturedControl) { m_capturedControl = capturedControl; }

	bool Redraw() { return m_redraw; }
	bool CaptureMouse() { return m_captureMouse; }
	bool CaptureOverride() { return m_captureOverride; }
	bool MessageHandled() { return m_messageHandled; }
	std::shared_ptr<Control> CapturedControl() { return m_capturedControl; }
	
private:
	bool m_redraw;
	bool m_captureMouse;
	bool m_captureOverride;
	bool m_messageHandled;
	std::shared_ptr<Control> m_capturedControl;
};