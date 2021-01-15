@echo off
rem /*
rem * Copyright 2020 NXP
rem * All rights reserved.
rem *
rem * SPDX-License-Identifier: BSD-3-Clause
rem */
rem Concatenate NS and S hex files and output to binary 
setlocal
set CMD=%~1
if "%CMD%" == "" (
  echo [ERR] Input parameters are missed!
  goto usage
)

set COM=
set DEVICE=
set INPUT_FILE=
set CFPA=

rem shift
:parse_args
set PARAMETER=%~1
set ARGUMENT=%~2
if "%PARAMETER%" == "-p" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set COM=%ARGUMENT%
      shift
    ) else (
      goto com_setup_missed
    )
  ) else (
      goto com_setup_missed
  )
) else if "%PARAMETER%" == "-d" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set DEVICE=%ARGUMENT%
      shift
    ) else (
      goto device_missed
    )
  ) else (
      goto device_missed
  )
) else if "%PARAMETER%" == "-i" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set INPUT_FILE=%ARGUMENT%
      shift
    ) else (
      goto input_file_missed
    )
  ) else (
      goto input_file_missed
  )
) else if "%PARAMETER%" == "-i_1" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set CFPA=%ARGUMENT%
      shift
    ) else (
      goto cfpa_file_missed
    )
  ) else (
      goto cfpa_file_missed
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args
) else (
  goto usage
)
goto parse_args
:end_parse_args

if "%COM%" == "" (
  :com_setup_missed
  echo [ERR] COM PORT must be specified
  echo:
  goto usage
)
if "%DEVICE%" == "" (
  :device_missed
  echo [ERR] device must be specified
  echo:
  goto usage
)
if "%INPUT_FILE%" == "" (
  :input_file_missed
  echo [ERR] ^<input.sb2^> is missed
  echo:
  goto usage
)
if "%CFPA%" == "" (
  :cfpa_file_missed
  echo [WARN] ^<cfpa.sb2^> is missed
)

echo Concatenating...
echo ^<COM^> = %COM%
echo ^<Device^> = %DEVICE%
echo ^<input.sb2^> = %INPUT_FILE%
if NOT "%CFPA%" == "" (
    echo ^<input.sb2^> = %CFPA%
)

cd ..\..\..\..\..\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools

rem ping
..\tools\blhost -V -p %COM% -- get-property 1

if %errorlevel% NEQ 0 (
    echo.
    echo [ERROR] not able to connect to the device via ISP!
    exit /b %errorlevel%
)

echo.
echo Load SB File

if not exist "%INPUT_FILE%" (
    echo.
    echo [ERROR] Missign tfm_cert.sb2 file.
    exit /b 
)

if "%CFPA%" == "" (
    call ..\tools\blhost -V -p %COM% -- receive-sb-file "%INPUT_FILE%"
) else (
    call ..\tools\blhost -V -p %COM% -- receive-sb-file "%CFPA%"
	TIMEOUT 2
	call ..\tools\blhost -V -p %COM% -- reset
    TIMEOUT 2
	call ..\tools\blhost -V -p %COM% -- flash-erase-all 0
	call ..\tools\blhost -V -p %COM% -- receive-sb-file "%INPUT_FILE%"
)

cd %CURRENT_DIR%
goto :eof

:usage
echo Usage: update_fw.bat -p COMx -d ^<device^> -i ^<input sb file^>
exit /b 1
