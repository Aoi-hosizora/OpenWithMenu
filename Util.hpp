#pragma once

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include "stdafx.h"
#include <atlconv.h>
#include <string>

#define REG_SZ_MAX 1024

class Util {
public:
    static std::string i2s(const int num) {
        return std::to_string((long long) num);
    }

    static std::wstring i2ws(const int num) {
        return std::to_wstring((long long) num);
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

    static LONG GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue) {
        strValue = strDefaultValue;
        WCHAR szBuffer[REG_SZ_MAX];
        DWORD dwBufferSize = sizeof(szBuffer);
        ULONG nError = RegQueryValueExW(hKey, strValueName.c_str(), nullptr, nullptr, (LPBYTE)szBuffer, &dwBufferSize);
        if (ERROR_SUCCESS == nError) {
            strValue = szBuffer;
        }
        return nError;
    }
};
