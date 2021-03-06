# OpenWithMenu

+ A Windows extension that is used to open applications quickly through context menu.
+ Environment: `Windows 10 20H2 x64` `Visual Studio 2019` `msvc2019`.

### Register

+ Open cmd as administrator, and you may need to restart explorer.exe.
+ Note that you need to set `regjump.exe` to your PATH for jump to setting, please visit https://docs.microsoft.com/en-us/sysinternals/downloads/regjump for details.

```bash
Register.bat
# or execute the following commands

regsvr32 x64\Release\OpenWithMenu.dll
```

+ Setup registry for background's context menu.
+ Note that CLSID is set in [OpenWithMenu.idl](./OpenWithMenu/OpenWithMenu.idl) and [OpenWithMenuImpl.rgs](./OpenWithMenu/OpenWithMenuImpl.rgs).

```reg
; Use Register.reg

[HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers\OpenWithMenu]
@="{33f3c901-789f-4e96-82d5-32b1cb204b40}"
```

### Unregister

+ Open cmd as administrator.

```bash
Unregister.bat
# or execute the following commands

regsvr32 /u x64\Release\OpenWithMenu.dll
```

+ Remove context menu from registry.

```reg
; Use Unregister.reg

[-HKEY_CLASSES_ROOT\Directory\Background\shellex\ContextMenuHandlers\OpenWithMenu]

; Also remove menu configs
; [-HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu]
```

### Configure

+ Configs are saved in `HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu` registry.

```reg
; Use DemoMenu.reg to add demo menu

[HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu]
"Name"="Open folder with"
"SettingName"="Setting"

[HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu\01 cmd]
@="Command prompt"
"File"="cmd.exe"
"Icon"="cmd.exe"

[HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu\02 cmd admin]
@="Command prompt (Admin)"
"File"="cmd.exe"
"Icon"="cmd.exe"
"Operation"="runas"
"Parameter"="/s /k pushd \"%V\""
```

#### Root key

+ `Name` - Menu title, defaults to `Open folder with`
+ `Icon` - Menu icon, supports both exe and ico path
+ `SettingName` - Setting menu title, defaults to `Setting`

#### Sub key

> + Note that only those menu items which has non empty `@` and `File` will be shown.
> + Some of theme arguments have the same meaning of `ShellExecuteW`, see [ShellExecuteW function (shellapi.h)](https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutew).

+ `key name` - Menu ID, **required**, items will be sorted by this value
+ `@` - Menu title, support CJK characters
+ `Icon` - Menu icon, supports both exe and ico path
+ `Operation` - Execute operation, defaults to `open`, this can be set to `runas` to execute as administrator
+ `File` - Executed file path, **required**, note that `%V` means the current directory (may need to be wrapped by `""`)
    + Execute with `open` operation can detect the current path, but `runas` cannot, need to use `Parameter: %V`
    + Command prompt example: `File: cmd.exe; Operation: runas; Parameter: /s /k pushd "%V"`
    + Powershell example: `File: powershell.exe; Operation: runas; Parameter: -noexit -command Set-Location -literalPath '%V'`
+ `Parameter` - Execute parameter, optional
    + Can also execute command line, example: `File: cmd.exe; Parameter: /c ping www.google.co.jp`
+ `Directory` - Current directory to execute, defaults to `%V`
+ `Style` - Window syle to execute file, defaults to `SW_NORMAL`, this can be set to `SW_HIDE` or `0` to hide window

### Screenshots

|![screenshot1](./assets/screenshot1.png)|![screenshot2](./assets/screenshot2.png)|
|---|---|

### References

+ [メニューにアイコンを付ける](http://home.a00.itscom.net/hatada/windows/tips/iconmenu.html)
+ [Icon Extractor Shell Extension](https://www.codeproject.com/Articles/1139/Icon-Extractor-Shell-Extension)
+ [Visual C++ 文字列 まとめ](https://qiita.com/tadnakam/items/99d7c40670fb3649875d)
+ [Windows レジストリ 解剖記](https://www.pg-fl.jp/program/winreg/classes.htm)
+ [ShellExecuteW function (shellapi.h)](https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutew)
