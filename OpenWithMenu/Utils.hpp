#pragma once

__pragma(warning(push)) __pragma(warning(disable:4996))

#include "MenuConfig.h"
#include "MenuItemConfig.h"

#include <shlobj.h>
#include <atlconv.h>
#include <vector>

class Utils {
public:

    /**
     * @breif Represent the max path length.
     */
    static const UINT MAX_PATH_LENGTH = 2048;

    /**
     * @breif Represent the max register key length.
     */
    static const UINT MAX_REG_KEY_LENGTH = 256;

    /**
     * @breif Represent the max register string value length.
     */
    static const UINT MAX_REG_SZ_LENGTH = 2048;

    /**
     * @brief Get folder name from given LPCITEMIDLIST.
     */
    static bool GetFolderNameFromItemIDList(LPCITEMIDLIST pidlFolder, std::wstring *out) {
        auto current_path = new WCHAR[MAX_PATH_LENGTH];
        bool ok = SHGetPathFromIDListW(pidlFolder, current_path);
        if (ok) {
            *out = current_path;
        }
        return ok;
    }

    /**
     * @brief Return the config registry path.
     */
    static std::wstring GetConfigRegistryPath() {
        return L"HKEY_CURRENT_USER\\SOFTWARE\\AoiHosizora\\OpenWithMenu";
    }

    /**
     * @brief Read menu icon path from registry.
     */
    static MenuConfig GetRegistryMenuConfig() {
        std::wstring reg_root = L"SOFTWARE\\AoiHosizora\\OpenWithMenu";
        HKEY root_key;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_root.c_str(), 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &root_key) != ERROR_SUCCESS) {
            return MenuConfig(L"Open folder with", L"");
        }
        auto name = ReadRegSz(root_key, L"Name", L"Open folder with");
        auto icon = ReadRegSz(root_key, L"Icon", L"");
        name = TrimWstring(name, { L' ' });
        icon = TrimWstring(icon, { L' ', L'"' });
        return MenuConfig(name, icon);
    }

    /**
     * @brief Read configs from registry.
     */
    static bool ReadRegistryConfigs(std::vector<MenuItemConfig> *out) {
        // get root key
        std::wstring reg_root = L"SOFTWARE\\AoiHosizora\\OpenWithMenu";
        HKEY root_key;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_root.c_str(), 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &root_key) != ERROR_SUCCESS) {
            *out = {};
            return false;
        }

        // get sub keys
        std::vector<std::wstring> sub_keys;
        int index = 0;
        WCHAR key_name[MAX_REG_KEY_LENGTH];
        DWORD key_name_size = sizeof(key_name) / sizeof(key_name[0]);
        while (RegEnumKeyEx(root_key, index, key_name, &key_name_size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
            index++;
            key_name_size = sizeof(key_name) / sizeof(key_name[0]);
            sub_keys.push_back(key_name);
        }
        RegCloseKey(root_key);

        // check each sub keys
        *out = {};
        for (auto key : sub_keys) {
            std::wstring reg_sub = reg_root + L"\\" + key;
            HKEY sub_key;
            if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_sub.c_str(), 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &sub_key) != ERROR_SUCCESS) {
                continue;
            }

            // get key's values
            auto name = ReadRegSz(sub_key, L"", L"");
            auto command = ReadRegSz(sub_key, L"Command", L"");
            auto icon = ReadRegSz(sub_key, L"Icon", L"");
            auto runas = ReadRegSz(sub_key, L"Runas", L"0"); // empty for admin
            auto style = ReadRegSz(sub_key, L"Style", L"");
            auto use_x = ReadRegSz(sub_key, L"UseX", L"0"); // empty for extra options

            // check key's value
            name = TrimWstring(name, { L' ' });
            command = TrimWstring(command, { L' ' });
            icon = TrimWstring(icon, { L' ', L'"' });
            runas = TrimWstring(runas, { L' ' });
            style = TrimWstring(style, { L' ' });
            use_x = TrimWstring(use_x, { L' ' });
            bool is_runas = runas.empty();
            bool is_use_x = use_x.empty();

            // save config
            if (!is_use_x) {
                if (!name.empty() && !command.empty()) {
                    out->push_back(MenuItemConfig(name, command, icon, is_runas, ParseStyleFromString(style)));
                }
            } else {
                // extra options
                auto x_op = ReadRegSz(sub_key, L"XOp", L""); // open
                auto x_file = ReadRegSz(sub_key, L"XFile", L""); // cmd.exe
                auto x_param = ReadRegSz(sub_key, L"XParam", L""); // /C
                x_op = TrimWstring(x_op, { L' ' });
                x_file = TrimWstring(x_file, { L' ', L'"' });
                x_param = TrimWstring(x_param, { L' ' });
                if (!name.empty() && !x_op.empty()) {
                    out->push_back(MenuItemConfig(name, command, icon, is_runas, ParseStyleFromString(style), x_op, x_file, x_param));
                }
            }

            RegCloseKey(sub_key);
        }

        return true;
    }

    /**
     * @brief Parse SW_XXX value from SW_XXX string.
     */
    static int ParseStyleFromString(std::wstring s) {
        if (s == L"" || s == L"SW_HIDE" || s == L"0") {
            return SW_HIDE;
        }
        if (s == L"SW_SHOWNORMAL" || s == L"SW_NORMAL" || s == L"1") {
            return SW_SHOWNORMAL;
        }
        if (s == L"SW_SHOWMINIMIZED" || s == L"2") {
            return SW_SHOWMINIMIZED;
        }
        if (s == L"SW_SHOWMAXIMIZED" || s == L"SW_MAXIMIZE" || s == L"3") {
            return SW_SHOWMAXIMIZED;
        }
        if (s == L"SW_SHOWNOACTIVATE" || s == L"4") {
            return SW_SHOWNOACTIVATE;
        }
        if (s == L"SW_SHOW" || s == L"5") {
            return SW_SHOW;
        }
        if (s == L"SW_MINIMIZE" || s == L"6") {
            return SW_MINIMIZE;
        }
        if (s == L"SW_SHOWMINNOACTIVE" || s == L"7") {
            return SW_SHOWMINNOACTIVE;
        }
        if (s == L"SW_SHOWNA" || s == L"8") {
            return SW_SHOWNA;
        }
        if (s == L"SW_RESTORE" || s == L"9") {
            return SW_RESTORE;
        }
        if (s == L"SW_SHOWDEFAULT" || s == L"10") {
            return SW_SHOWDEFAULT;
        }
        if (s == L"SW_FORCEMINIMIZE" || s == L"SW_MAX" || s == L"11") {
            return SW_FORCEMINIMIZE;
        }
        return SW_HIDE;
    }

    /**
     * @brief Generate a HBITMAP using small icon from given file path.
     */
    static HBITMAP GetSmallBitmapIconFromPath(std::wstring path) {
        // get small icon
        UINT num = ExtractIconEx(path.c_str(), -1, nullptr, nullptr, 0);
        if (num <= 0 || num == UINT_MAX) {
            return nullptr;
        }
        HICON hicon;
        num = ExtractIconEx(path.c_str(), 0, nullptr, &hicon, 1);
        if (num <= 0 || num == UINT_MAX) {
            return nullptr;
        }

        // create bitmap
        UINT width = GetSystemMetrics(SM_CXSMICON);
        UINT height = GetSystemMetrics(SM_CYSMICON);
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32; // 32bits color
        LPVOID bits;
        HBITMAP hbmp = CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
        if (hbmp == nullptr) {
            DestroyIcon(hicon);
            return nullptr;
        }

        // draw bitmap
        HDC hdcmem = CreateCompatibleDC(nullptr);
        auto hbmp_prev = (HBITMAP) SelectObject(hdcmem, hbmp);
        DrawIconEx(hdcmem, 0, 0, hicon, width, height, 0, nullptr, DI_NORMAL); // may panic
        SelectObject(hdcmem, hbmp_prev);
        DeleteDC(hdcmem);
        DestroyIcon(hicon);

        return hbmp;
    }

    /**
     * @brief Replace given src all to dst from wstring.
     */
    static std::wstring ReplaceWstring(const std::wstring &str, const std::wstring &src, const std::wstring &dst) {
        std::wstring cpy = str;
        int pos = 0;
        size_t srclen = src.size();
        size_t dstlen = dst.size();

        while ((pos = cpy.find(src, pos)) != std::string::npos) {
            cpy.replace(pos, srclen, dst);
            pos += dstlen;
        }
        return cpy;
    }

private:

    /**
     * @brief Read a string value from given key and name with a fallback value.
     */
    static std::wstring ReadRegSz(const HKEY &key, const std::wstring &name, const std::wstring &fallback) {
        WCHAR buf[MAX_REG_SZ_LENGTH];
        DWORD buf_size = sizeof(buf) / sizeof(buf[0]);
        if (RegQueryValueEx(key, name.c_str(), nullptr, nullptr, (LPBYTE) buf, &buf_size) != ERROR_SUCCESS) {
            return fallback;
        }
        return buf;
    }

    /**
     * @brief Trim the given wstring using given chars.
     */
    static std::wstring TrimWstring(const std::wstring &str, const std::vector<wchar_t> &chars) {
        if (str.empty()) {
            return str;
        }
        std::wstring copy = str;

        auto exist_eq = [](const std::vector<wchar_t> chars, wchar_t given) -> bool {
            bool found = false;
            for (auto c : chars) {
                if (c == given) {
                    found = true;
                    break;
                }
            }
            return found;
        };

        bool need_trim_start = exist_eq(chars, copy.at(0));
        while (need_trim_start) {
            copy = copy.substr(1);
            need_trim_start = exist_eq(chars, copy.at(0));
        }
        bool need_trim_end = exist_eq(chars, copy.at(copy.length() - 1));
        while (need_trim_end) {
            copy = copy.substr(0, copy.length() - 1);
            need_trim_end = exist_eq(chars, copy.at(copy.length() - 1));
        }

        return copy;
    }
};

__pragma(warning(pop))
