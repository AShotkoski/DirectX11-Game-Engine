#pragma once
#include <string>
#include "log.h"

namespace Util
{
    static std::wstring StringToWString( const std::string& str )
    {
        constexpr const size_t max_supported_strlen = 512u;
        DCHECK_LT_F( str.length(), max_supported_strlen, "string too long to convert: %s", str.c_str() );
        wchar_t wide[max_supported_strlen];
        mbstowcs_s( nullptr, wide, str.c_str(), _TRUNCATE );
        return wide;
    }

    static std::string WStringToString( const std::wstring& wstr )
    {
        constexpr const size_t max_supported_strlen = 512u;
        DCHECK_LT_F( wstr.length(), max_supported_strlen, "wstring too long to convert" );

        char narrow[max_supported_strlen];
        size_t numCharsConverted = 0;
        wcstombs_s( &numCharsConverted, narrow, wstr.c_str(), _TRUNCATE );
        // Ensure that the last character is in face a null terminating char
        narrow[numCharsConverted - 1] = '\0';
        return narrow;
    }
}