#pragma once

#include "stdafx.h"
#include <atlconv.h>
#include <string>
#include <fstream>
#include <sys/stat.h>

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
        size_t l = copy.length() - 1;
        while (!str.empty() && (copy.at(l) == L' ' || copy.at(l) == L'"')) {
            copy = copy.substr(0, l);
            l = copy.length() - 1;
        }
        return copy;
    }

    static std::wstring replaceAll(const std::wstring &str, const std::wstring &src, const std::wstring &dst) {
        std::wstring cpy = str;
        int pos = 0;
        int srclen = src.size();
        int dstlen = dst.size();

        while ((pos=str.find(src, pos)) != std::string::npos) {
            cpy.replace(pos, srclen, dst);
            pos += dstlen;
        }
        return cpy;
    }

    static std::wstring getFilename(const std::wstring &path) {
        if (path.empty()) {
            return L"";
        }
        WCHAR szDrive[REG_PATH_MAX];
        WCHAR szDir[REG_PATH_MAX];
        WCHAR szFname[REG_PATH_MAX];
        WCHAR szExt[REG_PATH_MAX];
        _wsplitpath_s(path.c_str(), szDrive, szDir, szFname, szExt);
        return std::wstring(szFname) + std::wstring(szExt);
    }

    static bool exist(const std::wstring &path) {
        struct _stat64i32 buffer;
        return (_wstat(path.c_str(), &buffer) == 0); 
    }
};
