#pragma once

#include <string>

/**
 * @brief Menu item's config, which is set in registry.
 */
class MenuItemConfig {
public:
    std::wstring name = L"";
    std::wstring icon = L"";
    std::wstring op = L"";
    std::wstring file = L"";
    std::wstring param = L"";
    std::wstring dir = L"";
    int style = SW_NORMAL;

    MenuItemConfig() {}

    MenuItemConfig(std::wstring name, std::wstring icon, std::wstring op, std::wstring file, std::wstring param, std::wstring dir, int style) :
        name(name), icon(icon), op(op), file(file), param(param), dir(dir), style(style) {}
};
