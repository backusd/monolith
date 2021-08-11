#include "App.h"

App::App()
{
	try
	{
		// All other hard coded pixel values will be in DIPS. However, because we don't have access to the D2DFactory
		// at the time of window creation (that could probably be changed), these values are physical pixels for the window size
		std::shared_ptr<ContentWindow> main = std::make_shared<ContentWindow>(2000, 1200, "Main Window");
		LayoutConfiguration::ConfigureMainWindow(main);

		// Only the main window should create the simulation
		main->InitializeSimulation();

		WindowManager::AddWindow(main);

		ThemeManager::Initialize(main->GetDeviceResources());
	}
	catch (const MonolithException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
}

int App::Run()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = WindowManager::ProcessMessages())
		{
			// Release Theme resources
			ThemeManager::Destroy();

			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		// For each window: Update state, render, and present
		WindowManager::UpdateRenderPresent();
	}

	
}
