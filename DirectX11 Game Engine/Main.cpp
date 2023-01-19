#include "Window.h"
#include "BaseException.h"

int WINAPI wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR         pCmdLine,
	_In_ int           nCmdShow )
{
	try
	{
		Window window( 800, 600, L"Direct3D Window." );

		// Message loop
		MSG msg;
		BOOL gResult;
		while ( ( gResult = GetMessage( &msg, nullptr, 0u, 0u ) ) > 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// Error code
		if ( gResult == -1 )
		{
			return -1;
		}
		// Code given to WM_QUIT message
		return (int)msg.wParam;
	}
	catch ( const BaseException& e )
	{
		MessageBoxA( nullptr, e.what(), e.GetType(), 0u );
	}
	catch ( const std::exception& e )
	{
		MessageBoxA( nullptr, e.what(), "STL exception", 0u);
	}
	catch ( ... )
	{
		MessageBoxA( nullptr, "Unknown exception thrown", "unknown", 0u);
	}
	// If we get passed exception
	return -1;
}
