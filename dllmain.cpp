#include "stdafx.h"
#include "resource.h"
#include "OpenWithMenu_i.h"
#include "dllmain.h"

COpenWithMenuModule _AtlModule;

extern "C"
BOOL WINAPI DllMain(
    HINSTANCE hInstance,
    DWORD dwReason,
    LPVOID lpReserved
) {
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}
