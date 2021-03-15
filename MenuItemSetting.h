#pragma once

#include "stdafx.h"
#include <string>

class MenuItemSetting {
public:
    // (Šù’è)
    std::wstring name = L"";
    // Command
    std::wstring command = L"";
    // Icon
    std::wstring icon = L"";
    // Runas
    bool runas = false;

    MenuItemSetting() { }
    MenuItemSetting(std::wstring name, std::wstring command, std::wstring icon, bool runas)
        : name(name), command(command), icon(icon), runas(runas) { }
};
