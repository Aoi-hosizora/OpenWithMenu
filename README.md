# OpenWithMenu

+ A windows extension of right click menu used to open folder with application

### Usage

+ Open cmd with administration if throw `0x80070005` error

+ Install

```bash
# Register dll
register.bat /r

# See CLSID in HKEY_CLASSES_ROOT\CLSID 
# (Defined in OpenWithMenuImpl.rgs and OpenWithMenu.idl)

# Run install.reg to add `OpenFolderWith` key to `HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers`
```

+ Uninstall

```bash
# Run uninstall.reg to remove `OpenFolderWith` key from `HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers`

# Unregister dll
register.bat /u
```

### References

+ [メニューにアイコンを付ける](http://home.a00.itscom.net/hatada/windows/tips/iconmenu.html)
