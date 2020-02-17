#pragma once

#include "stdafx.h"
#include <string>

class MenuItemSetting {
public:
    std::wstring name;
    std::wstring path;

    MenuItemSetting() { }
    MenuItemSetting(std::wstring name, std::wstring path) : name(name), path(path) { }
};
