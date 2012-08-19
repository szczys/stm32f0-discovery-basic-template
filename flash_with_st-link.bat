@echo off & setLocal enabledelayedexpansion

echo Flashing with ST-Link ...

if exist "C:\Program Files (x86)\STMicroelectronics\STM32 ST-Link Utility\ST-Link Utility\ST-LINK_CLI.exe" (
	set PPATH="C:\Program Files (x86)\STMicroelectronics\STM32 ST-Link Utility\ST-Link Utility\"
) else (
	set PPATH="C:\Program Files\STMicroelectronics\STM32 ST-Link Utility\ST-Link Utility\"

)

%PPATH%ST-LINK_CLI.exe -c SWD -P main.bin 0x08000000 -V -Rst

REM "C:\Program Files (x86)\STMicroelectronics\STM32 ST-Link Utility\ST-Link Utility\ST-LINK_CLI.exe" -c SWD -P Release\STM32F4_WiFi.bin 0x08000000 -V -Rst

pause
