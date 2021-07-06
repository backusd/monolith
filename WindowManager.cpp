#include "WindowManager.h"

// Have to define static member variables in .cpp file
std::vector<std::shared_ptr<WindowBase>> WindowManager::m_windows;

void WindowManager::DestroyWindow(WindowBase* window)
{
	for (auto iii = m_windows.begin(); iii != m_windows.end(); ++iii)
	{
		if (iii->get() == window)
		{
			m_windows.erase(iii);

			break;
		}
	}

	if (m_windows.size() == 0)
		PostQuitMessage(0);
}
