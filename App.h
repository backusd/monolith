#pragma once
#include "pch.h"

#include "ContentWindow.h"
#include "LayoutConfig.h"
#include "ThemeManager.h"
#include "WindowManager.h"



class App
{
public:
	App();

	int Run();

private:
	void DoFrame();

};