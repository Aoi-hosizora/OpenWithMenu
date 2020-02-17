#pragma once

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include "stdafx.h"
#include <atlconv.h>
#include <string>
#include <fstream>
#include <sys/stat.h>

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

    static std::wstring trimPath(const std::wstring &str) {
        std::wstring copy = str;
        while (!str.empty() && (copy.at(0) == L' ' || copy.at(0) == L'"')) {
            copy = copy.substr(1);
        }
        int l = copy.length() - 1;
        while (!str.empty() && (copy.at(l) == L' ' || copy.at(l) == L'"')) {
            copy = copy.substr(0, l);
            l = copy.length() - 1;
        }
        return copy;
    }

    static std::wstring getFilename(const std::wstring &path) {
        if (path == L"") {
            return L"";
        }
        WCHAR szDrive[REG_SZ_MAX];
        WCHAR szDir[REG_SZ_MAX];
        WCHAR szFname[REG_SZ_MAX];
        WCHAR szExt[REG_SZ_MAX];
        _wsplitpath(path.c_str(), szDrive, szDir, szFname, szExt);
        return std::wstring(szFname) + std::wstring(szExt);
    }

    static LONG readRegSz(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue) {
        strValue = strDefaultValue;
        WCHAR szBuffer[REG_SZ_MAX];
        DWORD dwBufferSize = sizeof(szBuffer);
        ULONG nError = RegQueryValueExW(hKey, strValueName.c_str(), nullptr, nullptr, (LPBYTE)szBuffer, &dwBufferSize);
        if (ERROR_SUCCESS == nError) {
            strValue = szBuffer;
        }
        return nError;
    }

    static bool exist(const std::wstring &path) {
        struct _stat64i32 buffer;
        return (_wstat(path.c_str(), &buffer) == 0); 
    }

    static HICON GetSmallIcon(const std::wstring &path) {
        HICON largeIcon, smallIcon;
        int num = ExtractIconEx(path.c_str(), -1, nullptr, nullptr, 0);
        if (num <= 0) {
            return nullptr;
        }
        ExtractIconEx(path.c_str(), 0, &largeIcon, &smallIcon, num);
        return smallIcon;
        // return ExtractIcon(nullptr, path.c_str(), 0);
    }

    static HBITMAP CreateBitmapARGB(int nWidth, int nHeight) {
        LPVOID     lpBits;
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth     = nWidth;
        bmi.bmiHeader.biHeight    = nHeight;
        bmi.bmiHeader.biPlanes    = 1;
        bmi.bmiHeader.biBitCount  = 32;
        return CreateDIBSection(nullptr, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, &lpBits, nullptr, 0);
    }

    static HBITMAP GetBitmapFromIcon(HICON hicon) {
        UINT uWidth  = GetSystemMetrics(SM_CXSMICON);
        UINT uHeight = GetSystemMetrics(SM_CYSMICON);
        HBITMAP hbmp = Util::CreateBitmapARGB(uWidth, uHeight);
        HDC hdcMem = CreateCompatibleDC(nullptr);
        auto hbmpPrev = (HBITMAP) SelectObject(hdcMem, hbmp);
        DrawIconEx(hdcMem, 0, 0, hicon, uWidth, uHeight, 0, nullptr, DI_NORMAL);
        SelectObject(hdcMem, hbmpPrev);
        DeleteDC(hdcMem);
        return hbmp;
    }
};
