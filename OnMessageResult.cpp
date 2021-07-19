#include "OnMessageResult.h"


OnMessageResult::OnMessageResult() :
	m_messageHandled(false),
	m_redraw(false),
	m_captureMouse(false),
	m_captureOverride(false),
	m_capturedControl(nullptr)
{}

OnMessageResult::~OnMessageResult()
{
}

OnMessageResult::OnMessageResult(const OnMessageResult& rhs) :
	m_messageHandled(rhs.m_messageHandled),
	m_redraw(rhs.m_redraw),
	m_captureMouse(rhs.m_captureMouse),
	m_captureOverride(rhs.m_captureOverride),
	m_capturedControl(rhs.m_capturedControl)
{
}

OnMessageResult& OnMessageResult::operator=(const OnMessageResult& rhs)
{
	if (this == &rhs)
		return *this;

	m_messageHandled = rhs.m_messageHandled;
	m_redraw = rhs.m_redraw;
	m_captureMouse = rhs.m_captureMouse;
	m_captureOverride = rhs.m_captureOverride;
	m_capturedControl = rhs.m_capturedControl;

	return *this;
}