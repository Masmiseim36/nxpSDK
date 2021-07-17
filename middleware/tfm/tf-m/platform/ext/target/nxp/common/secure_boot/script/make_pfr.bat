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
set PATH_TO_STORE=
set ALL_PFR=
set YAML=

set CONST=cfpa

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
) else if "%PARAMETER%" == "-p" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set PATH_TO_STORE=%ARGUMENT%
      shift
    ) else (
      goto path_for_output_file_missed
    )
  ) else (
      goto path_for_output_file_missed
  )
) else if "%PARAMETER%" == "-c" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set ALL_PFR=%ARGUMENT%
      shift
    ) else (
      goto all_pfr_not_specified
    )
  ) else (
      goto all_pfr_not_specified
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args
) else (
  goto usage
)
goto parse_args
:end_parse_args

if "%DEVICE%" == "" (
  :secure_file_missed
  echo [ERR] device must be specified
  echo:
  goto usage
)
if "%PATH_TO_STORE%" == "" (
  :path_for_output_file_missed
  echo [ERR] path for output file must be specified
  echo:
  goto usage
)
if "%ALL_PFR%" == "" (
  :all_pfr_not_specified
  echo [WARNING:] allPFR in default is set
  set ALL_PFR=allPFR
)

rem Find json file for correct device
cd ../../..

if %ALL_PFR% neq %CONST% (
	for /R %%P in (*%DEVICE%_allPFR*.yml) do (
		Set YAML=%%~dpP%%~nxP
	)
)
if %ALL_PFR% equ %CONST% (
	for /R %%P in (*%DEVICE%_cfpa*.yml) do (
		Set YAML=%%~dpP%%~nxP
	)
)

cd %CURRENT_DIR%

if "%YAML%" == "" (
  echo [ERR] ^<allPFR.yml or cfpa.yml^> is missed
  echo:
  goto usage
)

echo Concatenating...
echo ^<device^> = %DEVICE%
echo ^<path for output file^> = %PATH_TO_STORE%
echo ^<yaml^> = %YAML%

@echo on
..\tools\ymltopfr.exe %YAML%
@echo off

if %ALL_PFR% neq %CONST% (
	move "cfpaInit.bin" "%PATH_TO_STORE%"
    move "cmpa.bin" "%PATH_TO_STORE%"
)
if %ALL_PFR% equ %CONST% (
	move "cfpa.bin" "%PATH_TO_STORE%"
)
goto :eof

:usage
echo Usage: make_cfpa.bat -d ^<device^> -p ^<path for output file^> -c <"allPFR" or "cfpa"(optional)>
exit /b 1