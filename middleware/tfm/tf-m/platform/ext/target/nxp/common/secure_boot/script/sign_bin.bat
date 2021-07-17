@echo off
rem /*
rem * Copyright 2020 NXP
rem * All rights reserved.
rem *
rem * SPDX-License-Identifier: BSD-3-Clause
rem */
rem Concatenate NS and S hex files and output to binary 
setlocal
SET CURRENT_DIR=%~dp0
set CMD=%~1
if "%CMD%" == "" (
  echo [ERR] Input parameters are missed!
  goto usage
)

set DEVICE=
set INPUT_BINARY_FILE=
set CONFIG_FILE=

rem shift
:parse_args
set PARAMETER=%~1
set ARGUMENT=%~2
if "%PARAMETER%" == "-d" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set DEVICE=%ARGUMENT%
      shift
    ) else (
      goto device_not_specified
    )
  ) else (
      goto device_not_specified
  )
) else if "%PARAMETER%" == "-i" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set INPUT_BINARY_FILE=%ARGUMENT%
      shift
    ) else (
      goto input_binary_file_missed
    )
  ) else (
      goto input_binary_file_missed
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args
) else (
  goto usage
)
goto parse_args
:end_parse_args

rem Find json file for correct device
cd ../../..

for /R %%P in (*%DEVICE%*.json) do (
    Set CONFIG_FILE=%%~dpP%%~nxP
)

cd %CURRENT_DIR%

if "%CONFIG_FILE%" == "" (
    goto config_file_missed
)

if "%DEVICE%" == "" (
  :device_not_specified
  echo [ERR] device must be specified
  echo:
  goto usage
)
if "%INPUT_BINARY_FILE%" == "" (
  :input_binary_file_missed
  echo [ERR] ^<Input_Binary_File.bin^> is missed
  echo:
  goto usage
)
if "%CONFIG_FILE%" == "" (
  :config_file_missed
  echo [ERR] ^<Configuration_File.json^> is missed
  echo:
  goto usage
)

echo Concatenating...
echo ^<device^> = %DEVICE%
echo ^<Input_Binary_File.bin^> = %INPUT_BINARY_FILE%
echo ^<Configuration_File.json^> = %CONFIG_FILE%

if not exist "_tmp" mkdir "_tmp"
rem Copy input file to the tools directory.
copy %INPUT_BINARY_FILE% "_tmp\for_signing.bin"

if %errorlevel% NEQ 0 (
     echo [ERROR] not able to copy tfm_cert.bin!
     exit /b %errorlevel%
)

rem Get output path from INPUT_BINARY_FILE
for %%P in ("%INPUT_BINARY_FILE%") do Set OUTPUT_FOLDER=%%~dpP
rem Get output path from INPUT_BINARY_FILE
for %%P in ("%INPUT_BINARY_FILE%") do Set OUTPUT_FILE=%%~nxP

rem Sign
@echo on
..\tools\elftosb.exe -f %DEVICE% -J %CONFIG_FILE%
@echo off

if exist "_tmp\signed.bin" (
      copy "_tmp\signed.bin" "%OUTPUT_FOLDER%signed_%OUTPUT_FILE%"
)

rem Clean
rmdir /Q /S _tmp

goto :eof

:usage
echo Usage: sign_bin.bat -d ^<device^> - i ^<Input_Binary_File.bin^>
exit /b 1