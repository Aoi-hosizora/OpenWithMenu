#pragma once

#include "stdafx.h"

#define HKEY_MY_ROOT HKEY_CURRENT_USER

#define REG_KEY_MAX   255
#define REG_SZ_MAX    1024
#define REG_PATH_MAX  1024

#define LOG(_data) MessageBox(nullptr, _data, L"", MB_OK)
