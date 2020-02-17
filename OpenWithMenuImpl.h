#pragma once

#include "resource.h"
#include "OpenWithMenu_i.h"
#include "MenuItemSetting.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

#define MENU_ID_BEGIN 0
#define MENU_ID_END 9
#define MENU_ID_COUNT (MENU_ID_END - MENU_ID_BEGIN + 1) // 10
#define MENU_ID_SETTING (MENU_ID_END + 1) // 10

using namespace ATL;

class ATL_NO_VTABLE COpenWithMenuImpl :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<COpenWithMenuImpl, &CLSID_OpenWithMenuImpl>,
    public IDispatchImpl<IOpenWithMenuImpl, &IID_IOpenWithMenuImpl, &LIBID_OpenWithMenuLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IShellExtInit,
    public IContextMenu
{
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

    IDataObject *pdtobj;
    MenuItemSetting menuItemSettings[MENU_ID_COUNT];

public:

    virtual HRESULT STDMETHODCALLTYPE Initialize( 
        /* [annotation][unique][in] */ 
        __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
        /* [annotation][unique][in] */ 
        __in_opt  IDataObject *pdtobj,
        /* [annotation][unique][in] */ 
        __in_opt  HKEY hkeyProgID
    );

    virtual HRESULT STDMETHODCALLTYPE QueryContextMenu( 
        /* [annotation][in] */ 
        __in  HMENU hmenu,
        /* [annotation][in] */ 
        __in  UINT indexMenu,
        /* [annotation][in] */ 
        __in  UINT idCmdFirst,
        /* [annotation][in] */ 
        __in  UINT idCmdLast,
        /* [annotation][in] */ 
        __in  UINT uFlags
    );

    virtual HRESULT STDMETHODCALLTYPE InvokeCommand( 
        /* [annotation][in] */ 
        __in  CMINVOKECOMMANDINFO *pici
    );

    virtual HRESULT STDMETHODCALLTYPE GetCommandString( 
        /* [annotation][in] */ 
        __in  UINT_PTR idCmd,
        /* [annotation][in] */ 
        __in  UINT uType,
        /* [annotation][in] */ 
        __reserved  UINT *pReserved,
        /* [annotation][out] */ 
        __out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
        /* [annotation][in] */ 
        __in  UINT cchMax
    );
    
};

OBJECT_ENTRY_AUTO(__uuidof(OpenWithMenuImpl), COpenWithMenuImpl)
