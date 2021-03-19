#pragma once

#include "resource.h"
#include "OpenWithMenu_i.h"
#include "MenuItemConfig.h"
#include "Utils.hpp"

#include <shlobj.h>
#include <vector>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

class ATL_NO_VTABLE COpenWithMenuImpl :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<COpenWithMenuImpl, &CLSID_OpenWithMenuImpl>,
    public IDispatchImpl<IOpenWithMenuImpl, &IID_IOpenWithMenuImpl, &LIBID_OpenWithMenuLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IShellExtInit,
    public IContextMenu {

public:

    COpenWithMenuImpl() {}

    DECLARE_REGISTRY_RESOURCEID(IDR_OPENWITHMENUIMPL)

    BEGIN_COM_MAP(COpenWithMenuImpl)
        COM_INTERFACE_ENTRY(IOpenWithMenuImpl)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct() {
        return S_OK;
    }

    void FinalRelease() { }

private:

    /**
     * @brief Current folder.
     */
    LPCITEMIDLIST curr_folder = nullptr;

    /**
     * @brief Menu item configs.
     */
    std::vector<MenuItemConfig> configs = {};

public:

    virtual HRESULT STDMETHODCALLTYPE Initialize(
        /* [annotation][unique][in] */
        __in_opt PCIDLIST_ABSOLUTE pidlFolder,
        /* [annotation][unique][in] */
        __in_opt IDataObject *pdtobj,
        /* [annotation][unique][in] */
        __in_opt HKEY hkeyProgID
    );

    virtual HRESULT STDMETHODCALLTYPE QueryContextMenu(
        /* [annotation][in] */
        __in HMENU hmenu,
        /* [annotation][in] */
        __in UINT indexMenu,
        /* [annotation][in] */
        __in UINT idCmdFirst,
        /* [annotation][in] */
        __in UINT idCmdLast,
        /* [annotation][in] */
        __in UINT uFlags
    );

    virtual HRESULT STDMETHODCALLTYPE InvokeCommand(
        /* [annotation][in] */
        __in CMINVOKECOMMANDINFO *pici
    );

    virtual HRESULT STDMETHODCALLTYPE GetCommandString(
        /* [annotation][in] */
        __in UINT_PTR idCmd,
        /* [annotation][in] */
        __in UINT uType,
        /* [annotation][in] */
        __reserved UINT *pReserved,
        /* [annotation][out] */
        __out_awcount(!(uType &GCS_UNICODE), cchMax) LPSTR pszName,
        /* [annotation][in] */
        __in UINT cchMax
    ) {
        return S_OK;
    }

};

OBJECT_ENTRY_AUTO(__uuidof(OpenWithMenuImpl), COpenWithMenuImpl)
