# OpenWithMenu

+ A Windows extension that is used to open applications quickly through context menu.
+ Environment: `Windows 10 20H2 x64` `Visual Studio 2019` `msvc2019`.

### Register

+ Open cmd as administrator, and you may need to restart explorer.exe.

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

### Setting

+ OpenWithMenu's config is saved in `HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu`.
+ Note that you need to set `regjump.exe` to your PATH, please visit https://docs.microsoft.com/en-us/sysinternals/downloads/regjump for details.

```reg
; Use DemoMenu.reg to add demo menu

[HKEY_CURRENT_USER\Software\AoiHosizora\OpenWithMenu]
"Name"="Open folder with"
"Icon"="xxx"

[HKEY_CURRENT_USER\Software\AoiHosizora\OpenWithMenu\1 cmd admin]
@="Command prompt (Admin)"
"Command"="start cmd /s /k pushd \"%V\""
"Icon"="cmd.exe"
"Runas"=""

[HKEY_CURRENT_USER\SOFTWARE\AoiHosizora\OpenWithMenu\4 gitbash]
@="Git bash"
"Icon"="\"D:\\Program Files\\Git\\MAINICON.ico\""
"Style"="SW_NORMAL"
"UseX"=""
"XOp"="open"
"XFile"="\"D:\\Program Files\\Git\\git-bash.exe\""
```

#### Root key

+ `Name` - Menu title, without `(&F)`
+ `Icon` - Menu icon, supports both exe and ico path

#### Sub key

> Note that only those items which has non empty title `@` and command `Command` (operation `XOp` if use extra options) will be shown.

+ `key name` - Menu id, menu will be sorted by this value
+ `@` - Menu title
+ `Command` - Menu command, this command will be executed by hided `cmd.exe`
    + Note that the default window style maybe deactive, if you want to make it in foreground, please use `XFile` with `Style`
    + If `runas` is used, please use `%V` to represent the current path, because `runas` don't support `.`
    + If command is `cmd` `powershell` or other terminal applications, please use `start` to create a new window
    + `start cmd /s /k pushd "%V"`
    + `start powershell.exe -noexit -command Set-Location -literalPath '%V'`
+ `Icon` - Menu icon, supports both exe and ico path
+ `Runas` - Run as admin if set to an empty string (`""`)
+ `Style` - Window syle of cmd that executes command, such as `""` or `"SW_HIDE"` or `"0"` means to hide
+ `UseX` - Use extra options (`Xxx`) rather than normal options (`Command` `Runas`) if set to an empty string
+ `XOp` - The argument `lpOperation` for `ShellExecuteW` (mostly is `open`, `runas` if run as admin)
+ `XFile` - The argument `lpFile` for `ShellExecuteW`, `%V` means the current path
+ `XParam` - The argument `lpParameters` for `ShellExecuteW`, `%V` means the current path

### Screenshots

|![screenshot1](./assets/screenshot1.png)|![screenshot2](./assets/screenshot2.png)|![screenshot3](./assets/screenshot3.png)|
|---|---|---|

### References

+ [メニューにアイコンを付ける](http://home.a00.itscom.net/hatada/windows/tips/iconmenu.html)
+ [Icon Extractor Shell Extension](https://www.codeproject.com/Articles/1139/Icon-Extractor-Shell-Extension)
+ [Visual C++ 文字列 まとめ](https://qiita.com/tadnakam/items/99d7c40670fb3649875d)
+ [Windows レジストリ 解剖記](https://www.pg-fl.jp/program/winreg/classes.htm)
