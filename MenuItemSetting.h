#pragma once

#include "stdafx.h"
#include <string>

class MenuItemSetting {
public:
    std::wstring name;
    std::wstring path;
    std::wstring icon;
    std::wstring script;

    MenuItemSetting() { }
    MenuItemSetting(std::wstring name, std::wstring path, std::wstring icon, std::wstring script) 
        : name(name), path(path), icon(icon), script(script) { }
};
