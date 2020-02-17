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
    for (int i = MENU_ID_BEGIN; i <= MENU_ID_END; i++) { // 0 .. 9
        this->menuItemSettings[i].name = _T("Name ") + Util::s2ws(Util::i2s(i));
        this->menuItemSettings[i].path = _T("Path ") + Util::s2ws(Util::i2s(i));
    }
    return S_OK;
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
    outMenu.dwTypeData = _T("Open folder with application");
    outMenu.hSubMenu = subMenu;
    InsertMenuItem(hmenu, indexMenu, true, &outMenu);

    MENUITEMINFO settingMenuItem;
    settingMenuItem.cbSize = sizeof(MENUITEMINFO);
    settingMenuItem.fMask = MIIM_STRING | MIIM_ID;
    settingMenuItem.dwTypeData = _T("Setting...");
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
        std::wstring name = _T("アプリ ") + Util::s2ws(Util::i2s(i));
        menuItem.dwTypeData = Util::ws2Lpwstr(name);
        menuItem.wID = idCmdFirst + i;

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

    switch (idCmd) {
    case MENU_ID_SETTING:
        MessageBox(nullptr, _T("Setting"), _T("Title"), MB_OK);
        break;
    default:
        MessageBox(nullptr, 
            Util::ws2Lpwstr(this->menuItemSettings[idCmd].name + _T(" ") + this->menuItemSettings[idCmd].path),
            _T("Title"), MB_OK);
        break;
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
