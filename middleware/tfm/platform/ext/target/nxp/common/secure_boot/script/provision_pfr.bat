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
set CFPA_INIT=
set CMPA=
set CFPA_INIT_ADDRESS=
set CMPA_ADDRESS=

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
) else if "%PARAMETER%" == "-i1" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set CFPA_INIT=%ARGUMENT%
      shift
    ) else (
      goto cfpa_init_file_missed
    )
  ) else (
      goto cfpa_init_file_missed
  )
) else if "%PARAMETER%" == "-i2" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set CMPA=%ARGUMENT%
      shift
    ) else (
      goto cmpa_file_missed
    )
  ) else (
      goto cmpa_file_missed
  )
) else if "%PARAMETER%" == "-a1" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set CFPA_INIT_ADDRESS=%ARGUMENT%
      shift
    ) else (
      goto cfpa_init_address_missed
    )
  ) else (
      goto cfpa_init_address_missed
  )
) else if "%PARAMETER%" == "-a2" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set CMPA_ADDRESS=%ARGUMENT%
      shift
    ) else (
      goto cmpa_address_missed
    )
  ) else (
      goto cmpa_address_missed
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
if "%CFPA_INIT%" == "" (
  :cfpa_init_file_missed
  echo [ERR] ^<cfpaInit.bin^> is missed
  echo:
  goto usage
)
if "%CMPA%" == "" (
  :cmpa_file_missed
  echo [ERR] ^<cmpa.bin^> is missed
  echo:
  goto usage
)
if "%CFPA_INIT_ADDRESS%" == "" (
  :cfpa_init_address_missed
  echo [ERR] ^<cfpaInit address^> is missed
  echo:
  goto usage
)
if "%CMPA_ADDRESS%" == "" (
  :cmpa_address_missed
  echo [ERR] ^<cmpa address^> is missed
  echo:
  goto usage
)

echo Concatenating...
echo ^<COM^> = %COM%
echo ^<Device^> = %DEVICE%
echo ^<cfpaInit.bin^> = %CFPA_INIT%
echo ^<cmpa.bin^> = %CMPA%
echo ^<cfpaInit address^> = %CFPA_INIT_ADDRESS%
echo ^<cmpa address^> = %CMPA_ADDRESS%

cd ..\..\..\..\..\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools

rem ping
..\tools\blhost -V -p %COM% -- get-property 1

if %errorlevel% NEQ 0 (
    echo.
    echo [ERROR] not able to connect to the device via ISP!
    exit /b %errorlevel%
)

rem erase all flash
..\tools\blhost -V -p %COM% -- flash-erase-all 0

echo Key store initialization

rem Generate Activation Code
..\tools\blhost -V -p %COM% -- key-provisioning enroll
rem Generate Key Code for SBKEK according to SBkek.bin key
..\tools\blhost -V -p %COM% -- key-provisioning set_user_key 3 "..\template\SBkek.bin"
rem Generate random Key Code for PRINCE region 0
..\tools\blhost -V -p %COM% -- key-provisioning set_key 7 16
rem save the key store into PFR page of Flash memory
..\tools\blhost -V -p %COM% -- key-provisioning write_key_nonvolatile 0

echo.
echo Load CFPA table

if not exist "_tmp" mkdir "_tmp"
rem load old cfpa page
..\tools\blhost -V -p COM5 -- read-memory 0x3de00 512 "_tmp\cfpa.bin"
rem Convert binary file to hex file as batch file cannot work with null values in binary file
certutil -f -encodeHex _tmp\cfpa.bin _tmp\cfpa.hex >nul

setlocal ENABLEDELAYEDEXPANSION

FOR /F "eol=; tokens=1,2* delims=, " %%i in (_tmp\cfpa.hex) do (
   set /a c=c+1
   set DATA[!c!]=%%i %%j %%k
)

set /a "c=0"
 
rem store data from first raw to the array (values are split by blank space)
FOR %%a in (%DATA[1]%) do (
	set /a c=c+1
    set ARRAY[!c!]=%%a
	rem echo %%a, !c!
)

set /a "counter=0"

rem increment version number
FOR /L %%a in (6,1,9) do (
	rem echo !ARRAY[%%a]!

    if !ARRAY[%%a]!==00 (
        set /a counter=counter+1
    )
    if !counter! == 4  (
        rem save the cfpa page to the PFR page of Flash memory
        echo CFPA table is loaded to the Device
        ..\tools\blhost -V -p %COM% -- write-memory %CFPA_INIT_ADDRESS% %CFPA_INIT%
    )
)
rmdir /q /s _tmp

endlocal

echo.
echo Load CMPA table

rem save the cmpa page to the PFR page of Flash memory
..\tools\blhost -V -p %COM% -- write-memory %CMPA_ADDRESS% %CMPA%

echo.
echo PRINCE region configuration

rem Region selection
..\tools\blhost -V -p %COM% -- fill-memory 0x20008000 4 0x50000000
rem Start address of encrypted area
..\tools\blhost -V -p %COM% -- fill-memory 0x20008004 4 0x0
rem Length of encrypted area
..\tools\blhost -V -p %COM% -- fill-memory 0x20008008 4 0x38000
rem Call configure-memory with prepared structure in RAM
..\tools\blhost -V -p %COM% -- configure-memory 0 0x20008000

echo.
echo Device reset

..\tools\blhost -V -p %COM% -- reset

cd %CURRENT_DIR%
goto :eof

:usage
echo Usage: provision_pfr.bat -p COMx -d ^<device^> -i1 ^<path for cfpaInit file^> -i2 ^<path for cmpa file^> -a1 ^<cfpaInit address^> -a2 ^<cmpa address^>
exit /b 1
