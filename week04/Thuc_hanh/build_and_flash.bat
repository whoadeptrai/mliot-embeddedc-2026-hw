@echo off
cd /d "%~dp0"
echo STEP 1: CLEANING BUILD DIRECTORY
echo Deleting existing build folder...
if exist build ( rmdir /s /q build )

echo.
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
cmake -G "Ninja" -S . -B build

echo.
echo STEP 3: COMPILING FIRMWARE WITH NINJA
ninja -C build

echo.
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
STM32_Programmer_CLI -c port=SWD sn=34FF6B064248323545531157 -w build/app_firmware.bin 0x08000000 -rst

echo.
pause
