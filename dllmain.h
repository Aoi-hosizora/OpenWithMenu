// dllmain.h : 模块类的声明。

class COpenWithMenuModule : public ATL::CAtlDllModuleT< COpenWithMenuModule >
{
public :
	DECLARE_LIBID(LIBID_OpenWithMenuLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OPENWITHMENU, "{A851CAD7-9B9E-4122-9A57-ADD5A21F031E}")
};

extern class COpenWithMenuModule _AtlModule;
