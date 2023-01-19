#include "Window.h"

// Setup singleton
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wnd_class = { 0 };
	wnd_class.cbSize = sizeof( wnd_class );
	wnd_class.hInstance = GetHInstance();
	wnd_class.lpszClassName = ClassName;
	wnd_class.cbClsExtra = 0;
	wnd_class.cbWndExtra = 0;
	wnd_class.hbrBackground = nullptr;
	wnd_class.hCursor = nullptr;
	wnd_class.hIcon = nullptr;
	wnd_class.hIconSm = nullptr;
	wnd_class.lpfnWndProc = Window::SetupMessageProc;
	wnd_class.lpszMenuName = 0;
	wnd_class.style = CS_OWNDC;

	RegisterClassEx( &wnd_class );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( ClassName, GetHInstance() );
}

HINSTANCE Window::WindowClass::GetHInstance() noexcept
{
	return wndClass.hInst;
}

std::wstring Window::WindowClass::GetName()  noexcept
{
	return std::wstring(ClassName);
}

Window::Window( UINT Width, UINT Height, const std::wstring& Title )
	:
	width(Width),
	height(Height)
{
	const DWORD dwCreationFlags = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	
	// Adjust creation dimensions to represent client area.
	RECT wndRect;
	wndRect.left   = 0;
	wndRect.top    = 0;
	wndRect.right  = wndRect.left + width;
	wndRect.bottom = wndRect.top + height;
	
	AdjustWindowRectEx(&wndRect, dwCreationFlags, false, 0u);
	
	// Create Window, pass in pointer to this as lParam to use for message bs.
	hWnd = CreateWindowExW(
		0,
		WindowClass::GetName().c_str(),
		Title.c_str(),
		dwCreationFlags,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		nullptr,
		nullptr,
		WindowClass::GetHInstance(),
		this );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

LRESULT WINAPI Window::SetupMessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// This man has one purpose, to associate a ptr to the window class into the windows api.
	// Once done, messages will be sent to redirectmessageproc.
	
	// NCCREATE is called before window creation
	if ( msg == WM_NCCREATE )
	{
		// lParam will have a ptr to a CreateStruct
		const CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>( lParam );
		// pCreate holds the pointer to the window class inst we passed in on creation.
		Window* const pWnd = reinterpret_cast<Window*>( pCreate->lpCreateParams );
		// Store a ptr to the window class instance in the winapi
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );
		// Change to the redirect window proc
		SetWindowLongPtr( hWnd, GWLP_WNDPROC,
						  reinterpret_cast<LONG_PTR>(Window::RedirectMessageProc) );
		// Handle the message on messageproc as well
		return pWnd->MessageProc( hWnd, msg, wParam, lParam );
	}
	
	// let windows handle any sneaky window messages that happen to be before nccreate normally
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT WINAPI Window::RedirectMessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd, GWLP_USERDATA ));
	return pWnd->MessageProc( hWnd, msg, wParam, lParam );
}

LRESULT Window::MessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_CLOSE:
			PostQuitMessage( 0 );
			break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
