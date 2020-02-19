#pragma once

#include "stdafx.h"
#include "define.h"
#include <string>
#include <vector>

class RegUtil {
public:
    static LONG openRegKey(std::wstring subKey, PHKEY phkResult) {
        // RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
        return RegOpenKeyEx(HKEY_MY_ROOT, subKey.c_str(), 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_WOW64_64KEY, phkResult);
    }

    static std::vector<std::wstring> getSubKeys(PHKEY hKey) {
        std::vector<std::wstring> keyNames;
        int index = 0;
        TCHAR keyName[REG_KEY_MAX];
        DWORD keyNameSize = sizeof(keyName) / sizeof(keyName[0]);
        // RegEnumKeyExW(
        //     HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved,
        //     LPWSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime);
        while (RegEnumKeyEx(*hKey, index, keyName, &keyNameSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS) {
            keyNameSize = sizeof(keyName) / sizeof(keyName[0]);
            index++;
            keyNames.push_back(keyName);
        }
        return keyNames;
    }

    static std::pair<std::wstring, LONG> readRegSz(HKEY hKey, const std::wstring &val, const std::wstring &def) {
        TCHAR buf[REG_SZ_MAX];
        DWORD bufSize = sizeof(buf) / sizeof(buf[0]);
        // RegQueryValueExW(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
        ULONG nError = RegQueryValueEx(hKey, val.c_str(), nullptr, nullptr, (LPBYTE) buf, &bufSize);
        return std::make_pair<std::wstring, LONG>(ERROR_SUCCESS == nError ? buf : def, nError);
    }
};