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

### Setting

```reg
; See install.reg for demo

[HKEY_CURRENT_USER\Software\AoiHosizora\OpenFolderWith\1]
@="Command here as admin"
"Command"="start cmd /s /k pushd \"%V\""
"Icon"="cmd.exe"
"Runas"=""
```

+ `@` - Menu title
+ `Command` - Menu command, `%V` for currentPath (runas don't support `.`)
    + if command is `cmd.exe`, please use `start cmd` to open in a new window
+ `Icon` - Menu icon, support exe and ico
+ `Runas` - Run as admin if set to `""`

### Screenshot

![screenshot](./assets/Screenshot.jpg)

![screenshot2](./assets/Screenshot2.jpg)

### References

+ [メニューにアイコンを付ける](http://home.a00.itscom.net/hatada/windows/tips/iconmenu.html)
+ [Icon Extractor Shell Extension](https://www.codeproject.com/Articles/1139/Icon-Extractor-Shell-Extension)
