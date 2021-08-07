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

std::optional<int> WindowManager::ProcessMessages()
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return static_cast<int>(msg.wParam);
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}