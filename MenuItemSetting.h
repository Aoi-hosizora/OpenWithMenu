#pragma once

#include "stdafx.h"
#include <string>

class MenuItemSetting {
public:
    std::wstring name;
    std::wstring path;
    std::wstring icon;

    MenuItemSetting() { }
    MenuItemSetting(std::wstring name, std::wstring path, std::wstring icon) : name(name), path(path), icon(icon) { }
};
