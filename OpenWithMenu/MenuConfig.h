#pragma once

#include <string>

/**
 * @brief Menu config, which is set in registry.
 */
class MenuConfig {
public:
    std::wstring name = L"";
    std::wstring icon = L"";
    std::wstring setting_name = L"";

    MenuConfig() {}
    MenuConfig(std::wstring name, std::wstring icon, std::wstring setting_name)
        : name(name), icon(icon), setting_name(setting_name) {}
};
