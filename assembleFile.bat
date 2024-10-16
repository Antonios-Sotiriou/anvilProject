@echo off
REM Create assembly from a given file and deletes files we are not interested about.

set /p input=Enter filename: 

if exist %input% (
    echo filenname: %input%
) else (
    echo filename %input% not found!
)

call cl /I . /Fo: "asmTempFile" /c /Fa %input% /wd4067

del .\asmTempFile.obj

REM timeout /t 5 > nul
REM pause


