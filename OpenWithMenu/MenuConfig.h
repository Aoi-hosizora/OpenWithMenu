#pragma once

#include <string>

/**
 * @brief Menu config, which is set in registry.
 */
class MenuConfig {
public:
    std::wstring name = L"";
    std::wstring icon = L"";

    MenuConfig() {}
    MenuConfig(std::wstring name, std::wstring icon)
        : name(name), icon(icon) {}
};
