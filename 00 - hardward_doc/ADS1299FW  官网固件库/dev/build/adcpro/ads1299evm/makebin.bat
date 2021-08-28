@echo off
echo Hex conversion utility %~f1\tools\compiler\c5500\bin\hex55.exe


REM @if "%1." == "." (
@if "%~f1." == "." (
        echo MUST HAVE PATH TO CCS TOOLS DIRECTORY
        GOTO:EOF
        )

@if "%3." == "." (
        echo Generating default DEBUG bin
        GOTO:DEBUG_GEN
        )

echo Version is "%3"
pushd .
cd "%~f2"

if "%3." == "RELEASE." (
    if exist "Release\ads1299evm-mmb0.out" (
        echo Generating Release binary file
        "%~f1\tools\compiler\c5500\bin\hex55.exe" mmb0-r.hexcmd
        GOTO:finishUp
    )

    echo Release .out does not exist
    GOTO:finishUp
)

:DEBUG_GEN
REM Drop through generates debug .bin
if exist "Debug\ads1299evm-mmb0.out" (
    echo Generating Debug binary file
    "%~f1\tools\compiler\c5500\bin\hex55.exe" mmb0.hexcmd
    GOTO:finishUp
)

echo Debug .out does not exist

:finishUp
popd
