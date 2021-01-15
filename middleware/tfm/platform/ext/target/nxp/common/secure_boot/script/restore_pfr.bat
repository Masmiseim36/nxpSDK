@echo off
rem /*
rem * Copyright 2020 NXP
rem * All rights reserved.
rem *
rem * SPDX-License-Identifier: BSD-3-Clause
rem */
rem Restoring of PFR
setlocal
set CMD=%~1
if "%CMD%" == "" (
  echo [ERR] Input parameters are missed!
  goto usage
)

set COM=
set DEVICE=
set CFPA_ADDRESS=
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
) else if "%PARAMETER%" == "-a1" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set CFPA_ADDRESS=%ARGUMENT%
      shift
    ) else (
      goto cfpa_address_missed
    )
  ) else (
      goto cfpa_address_missed
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
if "%CFPA_ADDRESS%" == "" (
  :cfpa_address_missed
  echo [ERR] ^<cfpa address^> is missed
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
echo ^<cfpa address^> = %CFPA_ADDRESS%
echo ^<cmpa address^> = %CMPA_ADDRESS%

cd ..\..\..\..\..\middleware\tfm\platform\ext\target\nxp\common\secure_boot\tools

rem ping
..\tools\blhost -V -p %COM% -- get-property 1

if %errorlevel% NEQ 0 (
    echo.
    echo [ERROR] not able to connect to the device via ISP!
    exit /b %errorlevel%
)

if not exist "_tmp" mkdir "_tmp"

rem Prepare new CFPA table for restoring

..\tools\blhost -V -p %COM% -- read-memory %CFPA_ADDRESS% 512 "_tmp\cfpa.bin"

rem Convert binary file to hex file as batch file cannot work with null values in binary file
certutil -f -encodeHex _tmp\cfpa.bin _tmp\cfpa.hex >nul

rem store data from hex file to the variable
set /A DATA
set /A "c=0"
set /A ARRAY
set /A FINAL

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

rem conversion
FOR /L %%a in (6,1,9) do (
   set /A ARRAY[%%a]=0x!ARRAY[%%a]!
   rem echo !ARRAY[%%a]!
)

rem increment version number
FOR /L %%a in (6,1,9) do (
    set /A ARRAY[%%a]=!ARRAY[%%a]! + 1
	rem echo !ARRAY[%%a]!

    if !ARRAY[%%a]! == 256 (
        set /A ARRAY[%%a]=0
    ) else (
        goto continueScript
    )
)

rem format data
:continueScript
FOR /L %%a in (6,1,9) do (
	call :dec2hex ARRAY[%%a] !ARRAY[%%a]!
	rem echo !ARRAY[%%a]!
)

rem store first row of cfpa.bin
set /A "c=0"
FOR %%b in (!DATA[1]!) do (
	set /A c=c+1
	set FINAL[!c!]=%%b
)

rem upgrate version number 
FOR /L %%a in (6,1,9) do (
	set FINAL[%%a]=!ARRAY[%%a]!
	set FINAL[%%a]=!ARRAY[%%a]!
)

(echo !FINAL[2]! !FINAL[3]! !FINAL[4]! !FINAL[5]! !FINAL[6]! !FINAL[7]! !FINAL[8]! !FINAL[9]!^
	!FINAL[10]! !FINAL[11]! !FINAL[12]! !FINAL[13]! !FINAL[14]! !FINAL[15]! !FINAL[16]! !FINAL[17]!)>"_tmp\cfpaUpdate.hex"

rem Prepare new CFPA table for cleaning
FOR /L %%a in (2,1,3) do (
    set /A "c=0"
    FOR %%b in (!DATA[%%a]!) do (
	    set /A c=c+1
		set FINAL[!c!]=%%b
	    rem echo %%b, !c!
	)
	if %%a==3 (
	    rem Check if debug authentication was not set
	    if not !FINAL[5]!==00 (
		    goto endScript
		)
		rem Set to 0x00000000 in CFPA scratch page (CMPA page programming on going)
	    set FINAL[14]=00
		set FINAL[15]=00
		set FINAL[16]=00
		set FINAL[17]=00
	)
	(echo !FINAL[2]! !FINAL[3]! !FINAL[4]! !FINAL[5]! !FINAL[6]! !FINAL[7]! !FINAL[8]! !FINAL[9]!^
	!FINAL[10]! !FINAL[11]! !FINAL[12]! !FINAL[13]! !FINAL[14]! !FINAL[15]! !FINAL[16]! !FINAL[17]!)>>"_tmp\cfpaUpdate.hex"
)
FOR /L %%a in (4,1,32) do (
    (echo 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00)>>"_tmp\cfpaUpdate.hex"
)
rem Convert hex file to bin file
certutil -f -decodehex "_tmp\cfpaUpdate.hex" "_tmp\cfpaUpdate.bin" >nul

rem Prepare new CMPA table for restoring
FOR /L %%a in (1,1,32) do (
    (echo 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00)>>"_tmp\cmpaClean.hex"
)
rem Convert hex file to bin file
certutil -f -decodehex "_tmp\cmpaClean.hex" "_tmp\cmpaClean.bin" >nul

echo.
echo Clean CFPA table

..\tools\blhost -V -p %COM% -- write-memory %CFPA_ADDRESS% "_tmp\cfpaUpdate.bin"

echo.
echo Clean CMPA table

..\tools\blhost -V -p %COM% -- write-memory %CMPA_ADDRESS% "_tmp\cmpaClean.bin"

echo.
echo Device reset

..\tools\blhost -V -p %COM% -- reset

endlocal

rmdir /q /s _tmp

cd %CURRENT_DIR%
exit /b 1 

:usage
echo Usage: restore_device.bat -p COMx -d ^<device^> -a1 ^<cfpa address^> -a2 ^<cmpa address^>
exit /b 1  

:endScript
echo WARNING: The PFR cannot be restored due to rollback protection, which will cause the device lock.
rmdir /q /s _tmp
exit /b 1

rem format data
:dec2hex
set /A a = %2 / 16
set /A b = %2 - 16 * %a%
set converstion=0123456789ABCDEF
set b=!converstion:~%b%,1!
set a=!converstion:~%a%,1!
set %1=%a%%b%
goto:eof
