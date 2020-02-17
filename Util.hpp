#pragma once

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include "stdafx.h"
#include <atlconv.h>
#include <string>

class Util {
public:
    static std::string i2s(const int num) {
        return std::to_string((long long) num);
    }

    static std::wstring s2ws(const std::string &s) {
        int strLen = (int)s.length() + 1;
        int wstrLen = MultiByteToWideChar(CP_ACP, 0, s.c_str(), strLen, nullptr, 0); 
        auto buf = new wchar_t[wstrLen];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), strLen, buf, wstrLen);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

    static LPWSTR ws2Lpwstr(const std::wstring &s) {
        return (LPWSTR) (s.c_str());
    }
};
