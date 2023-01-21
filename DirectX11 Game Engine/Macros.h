#pragma once

#define DEFAULT_EXCEPT() (BaseException(__LINE__,__FILE__))
#define WINDOW_EXCEPT(hr) (Window::Exception(__LINE__,__FILE__,hr))
#define LAST_WND_ERR_EXCEPT() (WINDOW_EXCEPT(GetLastError()))
#define THROW_FAILED_GFX(f) if( FAILED(hr = f) ) throw Graphics::Exception(__LINE__,__FILE__,hr)
#define GFX_EXCEPT(hr) (Graphics::Exception(__LINE__,__FILE__,hr))