#pragma once
#include <unordered_map>
#include "pch.h"

class WindowsMessageMap
{
public:
	WindowsMessageMap() noexcept;
	std::string operator()(DWORD msg, LPARAM lParam, WPARAM wParam) const noexcept;

private:
	std::unordered_map<DWORD, std::string> map;
};