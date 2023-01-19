#include <Windows.h>
#include <string>

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CLOSE:
			PostQuitMessage( 0 );
			break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI wWinMain(
	_In_ HINSTANCE     hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR         pCmdLine,
	_In_ int           nCmdShow )
{
	/********Register Window***********/

	const std::wstring class_name = L"Main Window Class";

	// Create and initialize wnd class descriptor
	WNDCLASSEX wnd_class = {0};
	wnd_class.cbSize        = sizeof( wnd_class );
	wnd_class.hInstance     = hInstance;
	wnd_class.lpszClassName = class_name.c_str();
	wnd_class.cbClsExtra    = 0;
	wnd_class.cbWndExtra    = 0;
	wnd_class.hbrBackground = nullptr;
	wnd_class.hCursor       = nullptr;
	wnd_class.hIcon         = nullptr;
	wnd_class.hIconSm       = nullptr;
	wnd_class.lpfnWndProc   = WndProc;
	wnd_class.lpszMenuName  = 0;
	wnd_class.style         = CS_OWNDC;
	
	RegisterClassEx( &wnd_class );

	// Create window instance
	HWND hWnd = CreateWindowExW( 0,
								 class_name.c_str(),
								 L"Window Title d3d",
								 WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
								 CW_USEDEFAULT, CW_USEDEFAULT,
								 800, 600,
								 nullptr, nullptr, hInstance, nullptr );


	// Message loop
	MSG msg = { 0 };
	BOOL gResult;
	while ( (gResult = GetMessage( &msg, nullptr, 0u, 0u)) > 0 )
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
