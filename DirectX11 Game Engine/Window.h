#pragma once

#include "Win.h"
#include "macros.h"
#include <string>

class Window
{
private:
	// Singleton windows class to manage registration of window class
	class WindowClass
	{
	public:
		static HINSTANCE GetHInstance() noexcept;
		static std::wstring GetName() noexcept;
	private:
		WindowClass();
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
	private:
		static WindowClass wndClass;
		HINSTANCE hInst;
		static constexpr const wchar_t* ClassName = L"Direct3D Windows Class";
	};
public:
	Window(UINT Width, UINT Height, const std::wstring& Title);
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

private:
	static LRESULT WINAPI SetupMessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	static LRESULT WINAPI RedirectMessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	LRESULT				  MessageProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	UINT width;
	UINT height;
	HWND hWnd;
};

