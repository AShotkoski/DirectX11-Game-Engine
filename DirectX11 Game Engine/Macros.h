#pragma once

#define DEFAULT_EXCEPT() (BaseException(__LINE__,__FILE__))
#define WINDOW_EXCEPT(hr) (Window::Exception(__LINE__,__FILE__,hr))
#define LAST_ERR_EXCEPT() (WINDOW_EXCEPT(GetLastError()))