#include "stdafx.h"
#include "OpenWithMenuImpl.h"

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt  PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt  IDataObject *pdtobj,
    __in_opt  HKEY hkeyProgID
) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::QueryContextMenu(
    __in  HMENU hmenu,
    __in  UINT indexMenu,
    __in  UINT idCmdFirst,
    __in  UINT idCmdLast,
    __in  UINT uFlags
) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in  CMINVOKECOMMANDINFO *pici
) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::GetCommandString(
    __in  UINT_PTR idCmd,
    __in  UINT uType,
    __reserved  UINT *pReserved,
    __out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
    __in  UINT cchMax
) {
    return S_OK;
}
