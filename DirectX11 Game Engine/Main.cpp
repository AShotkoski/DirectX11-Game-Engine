#include "Window.h"


int WINAPI wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR         pCmdLine,
	_In_ int           nCmdShow )
{
	Window window( 800, 600, L"Direct3D Window." );

	MSG msg;
	BOOL gResult;
	while ( ( gResult = GetMessage( &msg, nullptr, 0u, 0u )) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	if ( gResult == -1 )
	{
		return -1;
	}
	else
	{
		return (int)msg.wParam;
	}
}
