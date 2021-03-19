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

    MenuItemConfig() {}
    MenuItemConfig(std::wstring name, std::wstring command, std::wstring icon, bool runas)
        : name(name), command(command), icon(icon), runas(runas) {}
};
