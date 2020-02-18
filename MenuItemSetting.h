#pragma once

#include "stdafx.h"
#include <string>

class MenuItemSetting {
public:
    // (Šù’è)
    std::wstring name;
    // Command
    std::wstring command;
    // Icon
    std::wstring icon;
    // Runas
    bool runas;

    MenuItemSetting() { }
    MenuItemSetting(std::wstring name, std::wstring command, std::wstring icon, bool runas) 
        : name(name), command(command), icon(icon), runas(runas) { }
};
