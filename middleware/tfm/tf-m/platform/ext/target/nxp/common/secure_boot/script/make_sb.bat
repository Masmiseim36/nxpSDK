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
set BIN_SIGN=
set OUTPUT_FILE=
set BIN_CFPA=
set BD=

set SWITCH=%~1

if "%SWITCH%" == "-secure_boot" (
   goto parse_args_sec_boot
) else if "%SWITCH%" == "-cfpa" (
   goto parse_args_cfpa
) else (
   goto usage
)

rem shift
:parse_args_sec_boot
set PARAMETER=%~2
set ARGUMENT=%~3
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
) else if "%PARAMETER%" == "-b" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set BIN_SIGN=%ARGUMENT%
      shift
    ) else (
      goto path_for_input_sign_file_missed
    )
  ) else (
      goto path_for_input_sign_file_missed
  )
) else if "%PARAMETER%" == "-o" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set OUTPUT_FILE=%ARGUMENT%
      shift
    ) else (
      goto output_file_is_not_present
    )
  ) else (
      goto output_file_is_not_present
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args_sec_boot
) else (
  goto usage
)
goto parse_args_sec_boot
:end_parse_args_sec_boot

rem Find json file for correct device
cd ../../..

for /R %%P in (*%DEVICE%_secure_boot*.bd) do (
    Set BD=%%~dpP%%~nxP
)

cd %CURRENT_DIR%

if "%DEVICE%" == "" (
  :device_not_specified
  echo [ERR] device must be specified
  echo:
  goto usage
)
if "%BIN_SIGN%" == "" (
  :path_for_input_sign_file_missed
  echo [ERR] path for output file must be specified
  echo:
  goto usage
)
if "%OUTPUT_FILE%" == "" (
  :output_file_is_not_present
  echo [ERR] path for output file must be specified
  echo:
  goto usage
)
if "%BD%" == "" (
  echo [ERR] path for command file must be specified
  echo:
  goto usage
)

echo Concatenating...
echo ^<device^> = %DEVICE%
echo ^<path for input signed file^> = %BIN_SIGN%

rem Get output path from INPUT_BINARY_FILE
for %%P in ("%BIN_SIGN%") do Set OUTPUT_FOLDER=%%~dpP

@echo on
call ..\tools\elftosb.exe -f %DEVICE% -k "..\template\SBkek.txt" -c "%BD%" -o "%OUTPUT_FILE%" -s ..\template\rotk1.pem -S ..\template\certificate_rotk1.der.crt -R ..\template\certificate_rotk1.der.crt -R ..\template\certificate_rotk2.der.crt -R ..\template\certificate_rotk3.der.crt -R ..\template\certificate_rotk4.der.crt -h "%OUTPUT_FOLDER%RHKT_SECURE_BOOT.bin" "%BIN_SIGN%"
@echo off

goto end_make

:parse_args_cfpa
set PARAMETER=%~2
set ARGUMENT=%~3
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
) else if "%PARAMETER%" == "-b" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set BIN_SIGN=%ARGUMENT%
      shift
    ) else (
      goto path_for_input_sign_file_missed
    )
  ) else (
      goto path_for_input_sign_file_missed
  )
) else if "%PARAMETER%" == "-c" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set BIN_CFPA=%ARGUMENT%
      shift
    ) else (
      echo CFPA binary file is not present
    )
  ) else (
      echo CFPA binary file is not present
  )
) else if "%PARAMETER%" == "-o" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set OUTPUT_FILE=%ARGUMENT%
      shift
    ) else (
      goto output_file_is_not_present
    )
  ) else (
      goto output_file_is_not_present
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args_cfpa
) else (
  goto usage
)
goto parse_args_cfpa
:end_parse_args_cfpa

rem Find json file for correct device
cd ../../..

for /R %%P in (*%DEVICE%_cfpa*.bd) do (
    Set BD=%%~dpP%%~nxP
)

cd %CURRENT_DIR%

if "%DEVICE%" == "" (
  :device_not_specified
  echo [ERR] device must be specified
  echo:
  goto usage
)
if "%BIN_SIGN%" == "" (
  :path_for_input_sign_file_missed
  echo [ERR] path for output file must be specified
  echo:
  goto usage
)
if "%OUTPUT_FILE%" == "" (
  :output_file_is_not_present
  echo [ERR] path for output file must be specified
  echo:
  goto usage
)
if "%BD%" == "" (
  echo [ERR] path for command file must be specified
  echo:
  goto usage
)

echo Concatenating...
echo ^<device^> = %DEVICE%
echo ^<path for input signed file^> = %BIN_SIGN%
echo ^<path for cfpa binary file^> = %BIN_CFPA%

rem Get output path from INPUT_BINARY_FILE
for %%P in ("%BIN_SIGN%") do Set OUTPUT_FOLDER=%%~dpP

@echo on
call ..\tools\elftosb.exe -f %DEVICE% -k "..\template\SBkek.txt" -c "%BD%" -o "%OUTPUT_FILE%" -s ..\template\rotk1.pem -S ..\template\certificate_rotk1.der.crt -R ..\template\certificate_rotk1.der.crt -R ..\template\certificate_rotk2.der.crt -R ..\template\certificate_rotk3.der.crt -R ..\template\certificate_rotk4.der.crt -h "%OUTPUT_FOLDER%RHKT_CFPA.bin" "%BIN_SIGN%"
@echo off

goto end_make

:usage
echo Usage: make_sb.bat -secure_boot -d ^<device^> -b ^<path for input signed binary file^> -o <^path for output sb file^>
echo        or
echo        make_sb.bat -cfpa -d ^<device^> -b ^<path for input signed binary file^> -c ^<path for cfpa binary file^> -o <^path for output sb file^>
exit /b 1

:end_make
exit /b 1