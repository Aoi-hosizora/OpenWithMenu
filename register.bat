@ECHO OFF

IF "%1" == "/r" GOTO :REG
IF "%1" == "/u" GOTO :UNREG
GOTO :HELP

:REG
regsvr32 ./x64/Release/OpenWithMenu.dll
GOTO :EOF

:UNREG
regsvr32 /u ./x64/Release/OpenWithMenu.dll
GOTO :EOF

:HELP
ECHO "%0" usage:
ECHO.
ECHO /r - Register dll
ECHO /u - Unregister dll
GOTO :EOF

:EOF
@ECHO ON
