#include "pch.h"
#include "OpenWithMenuImpl.h"

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt IDataObject *pdtobj,
    __in_opt HKEY hkeyProgID
) {
    this->curr_folder = pidlFolder;
    this->menu_config = Utils::GetRegistryMenuConfig();
    Utils::ReadRegistryConfigs(&configs); // ignore error
    return S_OK;
}

const UINT MENUID_SETTING = 0;
const UINT MENUID_BEGIN = 1;

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::QueryContextMenu(
    __in HMENU hmenu,
    __in UINT indexMenu,
    __in UINT idCmdFirst,
    __in UINT idCmdLast,
    __in UINT uFlags
) {
    if (uFlags & CMF_VERBSONLY) {
        return S_OK;
    }
    if (uFlags & CMF_DEFAULTONLY) {
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
    }
    if (this->curr_folder == nullptr) {
        return S_FALSE;
    }

    // sub menu
    HMENU sub_menu = CreateMenu();

    // temp mii
    MENUITEMINFO mii;
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STRING | MIIM_ID;

    // splitter mii
    MENUITEMINFO mii_splitter;
    mii_splitter.cbSize = sizeof(MENUITEMINFO);
    mii_splitter.fMask = MIIM_FTYPE;
    mii_splitter.fType = MFT_SEPARATOR;

    // command count
    UINT cmd_count = 0;

    // setting menu item
    mii.dwTypeData = _wcsdup((this->menu_config.setting_name + L"(&S)").c_str()); // setting
    mii.wID = idCmdFirst + MENUID_SETTING;
    InsertMenuItem(sub_menu, 0, true, &mii);
    InsertMenuItem(sub_menu, 1, true, &mii_splitter);
    cmd_count++;

    for (int i = 0; i < this->configs.size(); i++) {
        auto config = this->configs.at(i);
        mii.dwTypeData = _wcsdup((std::to_wstring(i + 1) + L": " + config.name).c_str()); // 0: XXX
        mii.wID = idCmdFirst + MENUID_BEGIN + i;
        HBITMAP hbmp = Utils::GetSmallBitmapIconFromPath(config.icon);
        if (hbmp != nullptr) {
            mii.fMask |= MIIM_BITMAP;
            mii.hbmpItem = hbmp;
        } else {
            mii.fMask &= ~MIIM_BITMAP;
            mii.hbmpItem = nullptr;
        }
        InsertMenuItem(sub_menu, 2 + i, true, &mii);
        cmd_count++;
    }

    // out layer menu
    MENUITEMINFO mii_out;
    mii_out.cbSize = sizeof(MENUITEMINFO);
    mii_out.fMask = MIIM_STRING | MIIM_SUBMENU;
    mii_out.dwTypeData = _wcsdup((this->menu_config.name + L"(&F)").c_str());
    mii_out.hSubMenu = sub_menu;
    HBITMAP hbmp_out = Utils::GetSmallBitmapIconFromPath(this->menu_config.icon);
    if (hbmp_out != nullptr) {
        mii_out.fMask |= MIIM_BITMAP;
        mii_out.hbmpItem = hbmp_out;
    }
    InsertMenuItem(hmenu, indexMenu, true, &mii_out);

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, cmd_count);
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }
    if (this->curr_folder == nullptr) {
        return S_FALSE;
    }

    // setting
    if (idCmd == MENUID_SETTING) {
        ShellExecute(nullptr, L"runas", L"cmd.exe", (L"/C regjump " + Utils::GetConfigRegistryPath()).c_str(), nullptr, SW_HIDE);
        return S_OK;
    }

    // each item
    auto config = this->configs.at(idCmd - MENUID_BEGIN);
    std::wstring current_path;
    if (!Utils::GetFolderNameFromItemIDList(this->curr_folder, &current_path)) {
        MessageBox(nullptr, L"Failed to get current folder's information.", config.name.c_str(), MB_OK);
        return S_FALSE;
    }

    // execute item
    std::wstring op = config.op;
    std::wstring file = Utils::ReplaceWstring(config.file, L"%V", current_path);
    std::wstring param = Utils::ReplaceWstring(config.param, L"%V", current_path);
    std::wstring dir = Utils::ReplaceWstring(config.dir, L"%V", current_path);
    int style = config.style;
    auto r = (int) ShellExecuteW(nullptr, op.c_str(), file.c_str(), param.c_str(), dir.c_str(), style);
    if (r <= 32) {
        std::wstring msg = L"Failed to execute the given command from config (errno = " + std::to_wstring(r) + L").";
        MessageBox(nullptr, msg.c_str(), config.name.c_str(), MB_OK);
    }

    return S_OK;
}
