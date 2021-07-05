#pragma once
#include "pch.h"


class DPIHelper
{
public:
	DPIHelper(HWND hWnd);

	template <typename T>
	float PixelsToDipsX(T pixels);

	template <typename T>
	float PixelsToDipsY(T pixels);

private:
	float dpiScaleX;
	float dpiScaleY;
};

template <typename T>
float DPIHelper::PixelsToDipsX(T pixels)
{
	return static_cast<float>(pixels) / dpiScaleX;
}

template <typename T>
float DPIHelper::PixelsToDipsY(T pixels)
{
	return static_cast<float>(pixels) / dpiScaleY;
}