#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include "stdafx.h"
#include "OpenWithMenuImpl.h"
#include "MenuItemSetting.h"

#include "define.h"
#include <string>
#include <vector>
#include <Windows.h>
#include "Util.hpp"
#include "IconUtil.hpp"
#include "RegUtil.hpp"

#define TITLE L"Open with"

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt  IDataObject *pdtobj,
    __in_opt  HKEY hkeyProgID
) {
    this->pidlFolder = pidlFolder;
    readRegInfo();
    return S_OK;
}

void COpenWithMenuImpl::readRegInfo() {
    this->settings.clear();
    HKEY hKey;
    std::wstring regRoot = L"Software\\AoiHosizora\\OpenFolderWith";
    if (RegUtil::openRegKey(regRoot, &hKey) != ERROR_SUCCESS) {
        return;
    }
    std::vector<std::wstring> subKeys = RegUtil::getSubKeys(&hKey);
    RegCloseKey(hKey);
    for (auto iter = subKeys.begin(); iter != subKeys.end(); iter++) {
        std::wstring regSubPath = regRoot + L"\\" + *iter;
        if (RegUtil::openRegKey(regSubPath, &hKey) != ERROR_SUCCESS) {
            continue;
        }

        std::wstring name = RegUtil::readRegSz(hKey, L"", L"").first;
        std::wstring command = RegUtil::readRegSz(hKey, L"Command", L"").first;
        std::wstring icon = RegUtil::readRegSz(hKey, L"Icon", L"").first;
        std::wstring runas = RegUtil::readRegSz(hKey, L"Runas", L"0").first;
        if (name.empty()) {
            continue;
        }
        icon = Util::trimPath(icon);

        MenuItemSetting setting;
        setting.name = name;
        setting.command = command;
        setting.icon = icon;
        setting.runas = runas.empty(); // "" for runas

        this->settings.push_back(setting);
        RegCloseKey(hKey);
    }
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::QueryContextMenu(
    __in  HMENU hmenu,
    __in  UINT indexMenu,
    __in  UINT idCmdFirst,
    __in  UINT idCmdLast,
    __in  UINT uFlags
) {
    if (uFlags & CMF_DEFAULTONLY) {
        return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 0);
    }

    HMENU subMenu = CreateMenu();
    MENUITEMINFO outMenu;
    outMenu.cbSize = sizeof(MENUITEMINFO);
    outMenu.fMask = MIIM_STRING | MIIM_SUBMENU;
    outMenu.dwTypeData = L"Open folder with application";
    outMenu.hSubMenu = subMenu;
    InsertMenuItem(hmenu, indexMenu, true, &outMenu);

    MENUITEMINFO settingMenuItem;
    settingMenuItem.cbSize = sizeof(MENUITEMINFO);
    settingMenuItem.fMask = MIIM_STRING | MIIM_ID;
    settingMenuItem.dwTypeData = L"Setting...";
    settingMenuItem.wID = idCmdFirst + MENU_ID_SETTING;

    MENUITEMINFO splitMenuItem;
    splitMenuItem.cbSize = sizeof(MENUITEMINFO);
    splitMenuItem.fMask = MIIM_FTYPE;
    splitMenuItem.fType = MFT_SEPARATOR;

    InsertMenuItem(subMenu, 0, true, &settingMenuItem);
    InsertMenuItem(subMenu, 1, true, &splitMenuItem);

    for (int i = 0; i < this->settings.size(); i++) {
        auto setting = &this->settings.at(i);

        MENUITEMINFO menuItem;
        menuItem.cbSize = sizeof(MENUITEMINFO);
        menuItem.fMask = MIIM_STRING | MIIM_ID;
        menuItem.wID = idCmdFirst + MENU_ID_BEGIN + i;

        // title
        std::wstring id = (i < 10 ? L"&" : L"") + Util::i2ws(i) + L": ";
        std::wstring name = setting->name.empty() ? L"(Not set)" : setting->name;
        menuItem.dwTypeData = (LPWSTR) (id + name).c_str(); // &5: xx

        // icon
        try {
            if (Util::exist(setting->icon)) {
                HICON icon = IconUtil::GetSmallIcon(setting->icon);
                HBITMAP bitmap = IconUtil::GetBitmapFromIcon(icon);
                if (bitmap != nullptr) {
                    menuItem.fMask = MIIM_STRING | MIIM_ID | MIIM_BITMAP;
                    menuItem.hbmpItem = bitmap;
                }
            }
        } catch (...) { }

        InsertMenuItem(subMenu, i + 2, true, &menuItem); // 0 -> 1 -> xx
    }

    return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 2 + this->settings.size());
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in  CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }

    if (idCmd == MENU_ID_SETTING) {
        ShellExecute(nullptr, L"runas", L"cmd.exe", L"/C regjump HKEY_CURRENT_USER\\Software\\AoiHosizora\\OpenFolderWith", nullptr, SW_HIDE);
        // ShellExecute(nullptr, L"runas", L"regjump.exe", L"HKEY_CURRENT_USER\Software\AoiHosizora\OpenFolderWith", nullptr, SW_SHOWNORMAL);
    } else {
        auto setting = &this->settings.at(idCmd - 1);
        auto currentPath = new WCHAR[2048];
        if (!SHGetPathFromIDListW(this->pidlFolder, currentPath)) {
            MessageBox(nullptr, L"Failed to get folder information.", TITLE, MB_OK);
            return S_FALSE;
        }
        if (setting->command.empty()) {
            // MessageBox(nullptr, L"Command empty.", TITLE, MB_OK);
            return S_FALSE;
        }
        std::wstring op = setting->runas ? L"runas" : L"open";
        std::wstring command = L"/C " + Util::replaceAll(setting->command, L"%V", currentPath);
        ShellExecute(nullptr, op.c_str(), L"cmd.exe", command.c_str(), currentPath, SW_HIDE);
        return S_OK;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::GetCommandString(
    __in  UINT_PTR idCmd,
    __in  UINT uType,
    __reserved  UINT *pReserved,
    __out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
    __in  UINT cchMax
) {
    return S_OK;
}
