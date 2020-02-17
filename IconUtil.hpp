#pragma once

#include "stdafx.h"
#include <string>

class IconUtil {
public:
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
        HBITMAP hbmp = CreateBitmapARGB(uWidth, uHeight);
        HDC hdcMem = CreateCompatibleDC(nullptr);
        auto hbmpPrev = (HBITMAP) SelectObject(hdcMem, hbmp);
        DrawIconEx(hdcMem, 0, 0, hicon, uWidth, uHeight, 0, nullptr, DI_NORMAL);
        SelectObject(hdcMem, hbmpPrev);
        DeleteDC(hdcMem);
        return hbmp;
    }
};
