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

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt  IDataObject *pdtobj,
    __in_opt  HKEY hkeyProgID
) {
    this->pidlFolder = pidlFolder;
    readRegistryInformation();
    return S_OK;
}

void COpenWithMenuImpl::readRegistryInformation() {
    this->settings.clear();
    HKEY hKey;
    std::wstring regRoot = L"Software\\AoiHosizora\\OpenFolderWith";
    if (RegUtil::openRegKey(regRoot, &hKey) != ERROR_SUCCESS) {
        return; // error
    }

    std::vector<std::wstring> subKeys = RegUtil::getSubKeys(&hKey);
    RegCloseKey(hKey);
    for (auto iter = subKeys.begin(); iter != subKeys.end(); iter++) {
        std::wstring regSubPath = regRoot + L"\\" + *iter;
        if (RegUtil::openRegKey(regSubPath, &hKey) != ERROR_SUCCESS) {
            continue; // error
        }

        std::wstring name, command, icon, runas;
        RegUtil::readRegSz(hKey, L"", L"", name);
        RegUtil::readRegSz(hKey, L"Command", L"", command);
        RegUtil::readRegSz(hKey, L"Icon", L"", icon);
        RegUtil::readRegSz(hKey, L"Runas", L"0", runas);
        RegCloseKey(hKey);
        if (name.empty()) {
            continue;
        }
        icon = Util::trimPath(icon);

        MenuItemSetting setting;
        setting.name = name;
        setting.command = command;
        setting.icon = icon;
        setting.runas = runas.empty(); // "" for admin
        this->settings.push_back(setting);
    }
}

const UINT MENUID_SETTING = 0;
const UINT MENUID_BEGIN = 1;

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::QueryContextMenu(
    __in  HMENU hmenu,
    __in  UINT indexMenu,
    __in  UINT idCmdFirst,
    __in  UINT idCmdLast,
    __in  UINT uFlags
) {
    if ((uFlags & CMF_VERBSONLY) || (uFlags & CMF_DEFAULTONLY)) {
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
    }

    // sub menu
    HMENU submenu = CreateMenu();

    // setting
    MENUITEMINFO mii_setting;
    mii_setting.cbSize = sizeof(MENUITEMINFO);
    mii_setting.fMask = MIIM_STRING | MIIM_ID;
    mii_setting.dwTypeData = L"Setting...";
    mii_setting.wID = idCmdFirst + MENUID_SETTING;
    InsertMenuItem(submenu, 0, true, &mii_setting);

    // splitter
    MENUITEMINFO mii_splitter;
    mii_splitter.cbSize = sizeof(MENUITEMINFO);
    mii_splitter.fMask = MIIM_FTYPE;
    mii_splitter.fType = MFT_SEPARATOR;
    InsertMenuItem(submenu, 1, true, &mii_splitter);

    for (int i = 0; i < this->settings.size(); i++) {
        auto setting = this->settings.at(i);

        // item
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_STRING | MIIM_ID;
        mii.wID = idCmdFirst + MENUID_BEGIN + i;

        // title
        std::wstring id = (i < 10 ? L"&" : L"") + Util::i2ws(i) + L": ";
        std::wstring name = setting.name.empty() ? L"(Not set)" : setting.name;
        mii.dwTypeData = wcsdup((id + name).data()); // &5: xx

        // icon
        try {
            if (Util::exist(setting.icon)) {
                HICON icon = IconUtil::GetSmallIcon(setting.icon);
                HBITMAP bitmap = IconUtil::GetBitmapFromIcon(icon);
                if (bitmap != nullptr) {
                    mii.fMask = MIIM_STRING | MIIM_ID | MIIM_BITMAP;
                    mii.hbmpItem = bitmap;
                }
            }
        } catch (...) { }

        InsertMenuItem(submenu, i + 2, true, &mii); // 0 -> 1 -> xx
    }

    // out layer menu
    MENUITEMINFO mii_out;
    mii_out.cbSize = sizeof(MENUITEMINFO);
    mii_out.fMask = MIIM_STRING | MIIM_SUBMENU;
    mii_out.dwTypeData = L"Open folder with application";
    mii_out.hSubMenu = submenu;
    InsertMenuItem(hmenu, indexMenu, true, &mii_out);

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, this->settings.size() + 2);
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in  CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }

    if (idCmd == MENUID_SETTING) {
        ShellExecute(nullptr, L"runas", L"cmd.exe", L"/C regjump HKEY_CURRENT_USER\\Software\\AoiHosizora\\OpenFolderWith", nullptr, SW_HIDE);
    } else {
        auto setting = this->settings.at(idCmd - 1);
        auto currentPath = new WCHAR[2048];
        if (!SHGetPathFromIDListW(this->pidlFolder, currentPath)) {
            MessageBox(nullptr, L"Failed to get folder information.", L"Open with", MB_OK);
            return S_FALSE;
        }
        if (setting.command.empty()) {
            return S_FALSE;
        }
        std::wstring op = setting.runas ? L"runas" : L"open";
        std::wstring command = L"/C " + Util::replaceAll(setting.command, L"%V", currentPath);
        ShellExecute(nullptr, op.c_str(), L"cmd.exe", command.c_str(), currentPath, SW_HIDE);
        return S_OK;
    }

    return S_OK;
}
