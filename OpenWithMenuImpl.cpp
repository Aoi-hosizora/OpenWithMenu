#include "stdafx.h"
#include "OpenWithMenuImpl.h"

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include <string>
#include "Util.hpp"
#include "MenuItemSetting.h"

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt  IDataObject *pdtobj,
    __in_opt  HKEY hkeyProgID
) {
    this->pdtobj = pdtobj;
    readRegInfo();
    return S_OK;
}

void COpenWithMenuImpl::readRegInfo() {
    HKEY hKey;
    for (int i = MENU_ID_BEGIN; i <= MENU_ID_END; i++) { // 0 .. 9
        std::wstring name, path;
        std::wstring regRoot = Util::s2ws("Software\\AoiHosizora\\OpenFolderWith\\" + Util::i2s(i));
        if (RegOpenKeyEx(HKEY_CURRENT_USER, regRoot.c_str(), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS) {
            Util::GetStringRegKey(hKey, L"", name, L"");
            Util::GetStringRegKey(hKey, L"path", path, L"");
        }
        this->menuItemSettings[i].name = name;
        this->menuItemSettings[i].path = path;
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

        std::wstring name = this->menuItemSettings[i].name;
        name = L"&" + Util::i2ws(i) + L": " + (name == L"" ? L"(Not set)" : name);
        menuItem.dwTypeData = (LPWSTR) name.c_str();

        InsertMenuItem(subMenu, i + 2, true, &menuItem); // 0 -> 1 -> xx
    }

    return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 2 + MENU_ID_COUNT); // 2 + (10 - 1) + 1
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in  CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }

    if (idCmd == MENU_ID_SETTING) {
        MessageBox(nullptr, L"Setting", L"Open with", MB_OK);
    } else {
        std::wstring path = this->menuItemSettings[idCmd].path;
        if (path == L"") {
            MessageBox(nullptr, L"Application path not set!", L"Open with", MB_OK);
        } else {
            MessageBox(nullptr, path.c_str(), L"Open with", MB_OK);
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
