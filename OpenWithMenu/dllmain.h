// dllmain.h : Declaration of module class.

class COpenWithMenuModule : public ATL::CAtlDllModuleT< COpenWithMenuModule > {
public:
    DECLARE_LIBID(LIBID_OpenWithMenuLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OPENWITHMENU, "{fcc24c18-b7a6-45a5-bd80-9dd95ab59302}")
};

extern class COpenWithMenuModule _AtlModule;
