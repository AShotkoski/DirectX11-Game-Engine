#pragma once
#include <log.h>
#include <string>
#include <vector>
// Please do not include this header in a header, the static functions are meant for internal linkage,
// meaning include it in source file.

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

    // Splits string str by delimiter delim into a vector of 0..* substrings
    static std::vector<std::string> SplitString( std::string str, const std::string& delim )
    {
        std::vector<std::string> split;

        size_t pos = 0;
        while ( ( pos = str.find( delim ) ) != std::string::npos )
        {
            split.push_back( str.substr( 0, pos ) );
            str.erase( 0, pos + delim.length() );
        }
        if(str.length() > 0 )
            split.push_back( str.substr( 0, str.length() ) );
        return split;
    }
}