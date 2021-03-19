#include "pch.h"
#include "OpenWithMenuImpl.h"

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::Initialize(
    __in_opt PCIDLIST_ABSOLUTE pidlFolder,
    __in_opt IDataObject *pdtobj,
    __in_opt HKEY hkeyProgID
) {
    this->curr_folder = pidlFolder;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::QueryContextMenu(
    __in HMENU hmenu,
    __in UINT indexMenu,
    __in UINT idCmdFirst,
    __in UINT idCmdLast,
    __in UINT uFlags
) {
    if (uFlags & CMF_VERBSONLY) {
        return S_OK;
    }
    if (uFlags & CMF_DEFAULTONLY) {
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
    }
    if (this->curr_folder == nullptr) {
        return S_FALSE;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE COpenWithMenuImpl::InvokeCommand(
    __in CMINVOKECOMMANDINFO *pici
) {
    UINT idCmd = LOWORD(pici->lpVerb);
    if (HIWORD(pici->lpVerb) != 0) {
        return E_INVALIDARG;
    }
    if (this->curr_folder == nullptr) {
        return S_FALSE;
    }


    switch (idCmd) {
        // TODO
    }

    return S_OK;
}
