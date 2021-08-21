#include "pch.h"

#include "App.h"

#include <sstream>




#define BUFFERSIZE 5
DWORD g_BytesTransferred = 0;

void DisplayError(LPTSTR lpszFunction);

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	std::ostringstream oss;
	oss << "Error code:\t" << dwErrorCode << "\n";
	OutputDebugString(oss.str().c_str());

	oss.clear();
	oss << "Number of bytes:\t" << dwNumberOfBytesTransfered << "\n";
	OutputDebugString(oss.str().c_str());

	g_BytesTransferred = dwNumberOfBytesTransfered;
}

void MyReadFile()
{
    HANDLE hFile;
    DWORD  dwBytesRead = 0;
    char   ReadBuffer[BUFFERSIZE] = { 0 };
    OVERLAPPED ol = { 0 };

	std::ostringstream oss;

    hFile = CreateFile("C:/Users/backu/file.txt",             // file to open
						GENERIC_READ,          // open for reading
						FILE_SHARE_READ,       // share for reading
						NULL,                  // default security
						OPEN_EXISTING,         // existing file only
						FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
						NULL);                 // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
		oss << "Error: Create File - unable to open file for read\n";
		OutputDebugString(oss.str().c_str());
        return;
    }

    // Read one character less than the buffer size to save room for
    // the terminating NULL character. 

    if (FALSE == ReadFileEx(hFile, ReadBuffer, BUFFERSIZE - 1, &ol, FileIOCompletionRoutine))
    {
		oss << "Error: Read File - Unable to read from file.\nGetLastError = " << GetLastError() << "\n";
		OutputDebugString(oss.str().c_str());

        CloseHandle(hFile);
        return;
    }
    SleepEx(5000, TRUE);
	dwBytesRead = g_BytesTransferred;
	// This is the section of code that assumes the file is ANSI text. 
	// Modify this block for other data types if needed.

	if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE - 1)
	{
		ReadBuffer[dwBytesRead] = '\0'; // NULL character

		oss << "Data read from file (" << dwBytesRead << " bytes) : \n";
		OutputDebugString(oss.str().c_str());

		oss.clear();
		oss << ReadBuffer << "\n";
		OutputDebugString(oss.str().c_str());
	}
	else if (dwBytesRead == 0)
	{
		oss << "No data read from file\n";
		OutputDebugString(oss.str().c_str());
	}
	else
	{
		oss << "\n ** Unexpected value for dwBytesRead ** \n";
		OutputDebugString(oss.str().c_str());
	}

	// It is always good practice to close the open file handles even though
	// the app will exit here and clean up open handles anyway.

	CloseHandle(hFile);
}



int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//MyReadFile();


	try
	{
		return App{}.Run();
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
	return -1;
}