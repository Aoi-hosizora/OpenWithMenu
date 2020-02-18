Dim objHTA
Dim cClipBoard
Dim WshShell

set objHTA = CreateObject("htmlfile")
cClipBoard = objHTA.ParentWindow.ClipBoardData.GetData("text")

Set WshShell = WScript.CreateObject("WScript.Shell")
WshShell.RegWrite "HKEY_CURRENT_USER\Software\AoiHosizora\OpenFolderWith", cClipBoard, "REG_SZ"
WshShell.Run "regedit.exe -m"

Set objHTA = nothing
Set WshShell = nothing
