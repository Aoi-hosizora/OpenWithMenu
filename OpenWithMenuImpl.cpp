#include "stdafx.h"
#include "OpenWithMenuImpl.h"

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include <string>
#include "Util.hpp"
#include "IconUtil.hpp"
#include "MenuItemSetting.h"
#include <Windows.h>

#define TITLE L"Open with"

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt  IDataObject *pdtobj,
    __in_opt  HKEY hkeyProgID
) {
    this->pdtobj = pdtobj;
    this->pidlFolder = pidlFolder;
    readRegInfo();
    return S_OK;
}

void COpenWithMenuImpl::readRegInfo() {
    HKEY hKey;
    for (int i = MENU_ID_BEGIN; i <= MENU_ID_END; i++) { // 0 .. 9
        std::wstring name, path, icon, script;
        std::wstring regRoot = Util::s2ws("Software\\AoiHosizora\\OpenFolderWith\\" + Util::i2s(i));
        if (RegOpenKeyEx(HKEY_CURRENT_USER, regRoot.c_str(), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS) {
            Util::readRegSz(hKey, L"", name, L"");
            Util::readRegSz(hKey, L"Path", path, L"");
            Util::readRegSz(hKey, L"Icon", icon, L"");
            Util::readRegSz(hKey, L"Script", script, L"");
        }
        path = Util::trimPath(path);
        icon = Util::trimPath(icon);
        if (name == L"" && path != L"") {
            name = Util::getFilename(path); // replace name with path
        }
        this->menuItemSettings[i].name = name;
        this->menuItemSettings[i].path = path;
        this->menuItemSettings[i].icon = icon;
        this->menuItemSettings[i].script = script;
    }
    RegCloseKey(hKey);
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

    // BOOL InsertMenuItem(HMENU hMenu, UINT uItem, BOOL fByPosition, LPMENUITEMINFO lpmii);

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

    for (int i = MENU_ID_BEGIN; i <= MENU_ID_END; i++) { // 0 .. 9
        MENUITEMINFO menuItem;
        menuItem.cbSize = sizeof(MENUITEMINFO);
        menuItem.fMask = MIIM_STRING | MIIM_ID;
        menuItem.wID = idCmdFirst + i;

        // title
        std::wstring name = this->menuItemSettings[i].name;
        name = L"&" + Util::i2ws(i) + L": " + (name == L"" ? L"(Not set)" : name);
        menuItem.dwTypeData = (LPWSTR) name.c_str();

        try {
            // icon
            std::wstring iconPath = this->menuItemSettings[i].icon;
            if (Util::exist(iconPath)) {
                menuItem.fMask = MIIM_STRING | MIIM_ID | MIIM_BITMAP;
                HICON icon = IconUtil::GetSmallIcon(iconPath);
                HBITMAP bitmap = IconUtil::GetBitmapFromIcon(icon);
                if (bitmap != nullptr) {
                    menuItem.hbmpItem = bitmap;
                } else {
                    menuItem.fMask = MIIM_STRING | MIIM_ID;
                }
            }
        } catch (...) {
            menuItem.fMask = MIIM_STRING | MIIM_ID;
        }

        InsertMenuItem(subMenu, i + 2, true, &menuItem); // 0 -> 1 -> xx
    }

    return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 2 + MENU_ID_COUNT);
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in  CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }

    if (idCmd == MENU_ID_SETTING) {
        MessageBox(nullptr, L"Setting", TITLE, MB_OK);
    } else {
        std::wstring path = this->menuItemSettings[idCmd].path;
        std::wstring script = this->menuItemSettings[idCmd].script;

        if (script != L"") { // Run script
            _wsystem(script.c_str()); 
        } else if (path != L"") { // Open with
            auto currentPath = new WCHAR[2048];
            if (!SHGetPathFromIDListW(this->pidlFolder, currentPath)) {
                MessageBox(nullptr, L"Failed to open!", TITLE, MB_OK);
                return S_OK;
            }
            std::wstring cmd = path + L" \"" + currentPath + L"\"";
            // MessageBox(nullptr, cmd.c_str(), TITLE, MB_OK);
            _wsystem(cmd.c_str()); 
        } else {
            MessageBox(nullptr, L"Application path not set!", TITLE, MB_OK);
        }
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
