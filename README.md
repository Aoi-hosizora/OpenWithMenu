# OpenWithMenu

+ A windows extension of right click menu used to open folder with application.

### Usage

+ Note that you will need `RegJump` tool in your PATH, see https://docs.microsoft.com/en-us/sysinternals/downloads/regjump for details.

```bash
# Build x64 Register platform

cd x64/Release/
regsvr32 OpenWithMenu.dll
Register.reg

# See CLSID in HKEY_CLASSES_ROOT\CLSID, is defined in OpenWithMenuImpl.rgs and OpenWithMenu.idl
# Run install.reg to add `OpenFolderWith` key to `HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers`

# Unregister
regsvr32 /u OpenWithMenu.dll
Unregister.reg
```

### Setting

```reg
; See Register.reg for demo

[HKEY_CURRENT_USER\Software\AoiHosizora\OpenFolderWith\xxx]
@="Command here as admin"
"Command"="start cmd /s /k pushd \"%V\""
"Icon"="cmd.exe"
"Runas"=""
```

+ xxx - Subkey name (Item will be sorted by this field)
+ `@` - Menu title
+ `Command` - Menu command, `%V` for currentPath (runas don't support `.`)
    + If command is `cmd.exe` or `powershell.exe`, please use `start` to open in a new window
    + `start cmd /s /k pushd "%V"`
    + `start powershell.exe -noexit -command Set-Location -literalPath '%V'`
+ `Icon` - Menu icon, support exe and ico
+ `Runas` - Run as admin if set to `""`

### Screenshot

![screenshot](./assets/Screenshot.jpg)

![screenshot2](./assets/Screenshot2.jpg)

### References

+ [メニューにアイコンを付ける](http://home.a00.itscom.net/hatada/windows/tips/iconmenu.html)
+ [Icon Extractor Shell Extension](https://www.codeproject.com/Articles/1139/Icon-Extractor-Shell-Extension)
