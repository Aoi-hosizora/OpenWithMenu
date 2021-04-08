#pragma once

#include <string>

/**
 * @brief Menu item's config, which is set in registry.
 */
class MenuItemConfig {
public:
    std::wstring name = L"";
    std::wstring command = L"";
    std::wstring icon = L"";
    bool runas = false;
    int style = SW_HIDE;

    bool use_x = false;
    std::wstring x_op = L"";
    std::wstring x_file = L"";
    std::wstring x_param = L"";

    MenuItemConfig() {}

    MenuItemConfig(std::wstring name, std::wstring command, std::wstring icon, bool runas, int style)
        : name(name), command(command), icon(icon), runas(runas), style(style), use_x(false) {}

    MenuItemConfig(std::wstring name, std::wstring command, std::wstring icon, bool runas, int style, std::wstring x_op, std::wstring x_file, std::wstring x_param)
        : name(name), command(command), icon(icon), runas(runas), style(style), use_x(true), x_op(x_op), x_file(x_file), x_param(x_param) {}
};
