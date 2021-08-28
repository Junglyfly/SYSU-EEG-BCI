@echo off
echo Hex conversion utility %~f1\tools\compiler\c5500\bin\hex55.exe
echo Version is %2

@if "%2." == "." echo Generating default DEBUG bin

REM generate release .bin
@if "%2." == "RELEASE." (
    @if exist Release\ccs-dxp-evm.out (
        %~f1\tools\compiler\c5500\bin\hex55.exe dxp_mmb0-r.hexcmd
        GOTO:EOF 
    )
    echo .out does not exist
    GOTO:EOF 
) 

REM Drop through generates debug .bin
if exist Debug\ccs-dxp-evm.out (
        %~f1\tools\compiler\c5500\bin\hex55.exe dxp_mmb0.hexcmd
        GOTO:EOF 
        )

        echo .out does not exist

