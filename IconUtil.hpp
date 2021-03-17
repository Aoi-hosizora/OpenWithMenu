#pragma once

#include "stdafx.h"
#include <string>

class IconUtil {
public:
    static HICON GetSmallIcon(const std::wstring &path) {
        HICON largeIcon, smallIcon;
        UINT num = ExtractIconEx(path.c_str(), -1, nullptr, nullptr, 0);
        if (num <= 0 || num == UINT_MAX) {
            return nullptr;
        }
        num = ExtractIconEx(path.c_str(), 0, &largeIcon, &smallIcon, 1);
        if (num == UINT_MAX) {
            return nullptr;
        }
        return smallIcon;
    }

    static HBITMAP CreateBitmapARGB(int nWidth, int nHeight) {
        LPVOID lpBits;
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = nWidth;
        bmi.bmiHeader.biHeight = nHeight;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        return CreateDIBSection(nullptr, (BITMAPINFO *) &bmi, DIB_RGB_COLORS, &lpBits, nullptr, 0);
    }

    static HBITMAP GetBitmapFromIcon(HICON hIcon) {
        int uWidth = GetSystemMetrics(SM_CXSMICON);
        int uHeight = GetSystemMetrics(SM_CYSMICON);

        HBITMAP hBmp = CreateBitmapARGB(uWidth, uHeight);
        HDC hdcMem = CreateCompatibleDC(nullptr);
        auto hbmpPrev = (HBITMAP) SelectObject(hdcMem, hBmp);
        DrawIconEx(hdcMem, 0, 0, hIcon, uWidth, uHeight, 0, nullptr, DI_NORMAL); // may panic
        SelectObject(hdcMem, hbmpPrev);
        DeleteDC(hdcMem);
        return hBmp;
    }
};
