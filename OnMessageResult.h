#pragma once
#include "pch.h"

enum class OnMessageResult
{
	NONE,
	CAPTURE_MOUSE,
	MESSAGE_HANDLED,
	CAPTURE_MOUSE_AND_MESSAGE_HANDLED
};