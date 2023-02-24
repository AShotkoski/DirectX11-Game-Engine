#pragma once
#include <string>

namespace Util
{
    static std::wstring StringToWString( const std::string& s )
    {
        std::wstring temp;
        for ( auto& c : s )
        {
            temp.push_back( (wchar_t)c );
        }
        return temp;
    }

    static std::string WStringToString( const std::wstring& s )
    {
        std::string temp;
        for ( auto& c : s )
        {
            temp.push_back( (char)c );
        }
        return temp;
    }
}