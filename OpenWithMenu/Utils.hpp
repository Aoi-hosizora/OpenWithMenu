#pragma once

__pragma(warning(push)) __pragma(warning(disable:4996))

#include "MenuItemConfig.h"

#include <shlobj.h>
#include <shobjidl.h>
#include <atlconv.h>

#include <vector>

class Utils {
public:

    /**
     * @breif Represent the max register key length.
     */
    static const UINT MAX_REG_KEY_LENGTH = 256;

    /**
     * @breif Represent the max register sz value length.
     */
    static const UINT MAX_REG_SZ_LENGTH = 2048;

    /**
     * @breif Represent the max path length.
     */
    static const UINT MAX_PATH_LENGTH = 2048;

    /**
     * @brief Get folder name from given folder, that is LPCITEMIDLIST.
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
     * @brief Return the config registry path.
     */
    static std::wstring GetConfigRegistryPath() {
        return L"HKEY_CURRENT_USER\\SOFTWARE\\AoiHosizora\\OpenWithMenu";
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
            key_name_size = sizeof(key_name) / sizeof(key_name[0]);
            index++;
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
            RegCloseKey(sub_key);

            // check key's value
            name = TrimWstring(name, { L' ' });
            command = TrimWstring(command, { L' ' });
            icon = TrimWstring(icon, { L' ', L'\\', L'/' });
            runas = TrimWstring(command, { L' ' });
            if (name.empty() || command.empty()) {
                continue;
            }

            out->push_back(MenuItemConfig(name, command, icon, false));
        }

        return true;
    }
};

__pragma(warning(pop))
