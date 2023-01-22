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

set S_FILE=
set NS_FILE=
set FILE_APP=
set FILE_TABLE=
set OUTPUT_FILE=
set ALIGN_ADDRESS=

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
if "%PARAMETER%" == "-s" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set S_FILE=%ARGUMENT%
      shift
    ) else (
      goto secure_file_missed
    )
  ) else (
      goto secure_file_missed
  )
) else if "%PARAMETER%" == "-ns" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set NS_FILE=%ARGUMENT%
      shift
    ) else (
      goto non_secure_file_missed
    )
  ) else (
      goto non_secure_file_missed
  )
) else if "%PARAMETER%" == "-o" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set OUTPUT_FILE=%ARGUMENT%
      shift
    ) else (
      goto output_file_missed
    )
  ) else (
      goto output_file_missed
  )
) else if "%PARAMETER%" == "-a" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set ALIGN_ADDRESS=%ARGUMENT%
      shift
    ) else (
      goto align_address_missed
    )
  ) else (
      goto align_address_missed
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args_sec_boot
) else (
  goto usage
)
goto parse_args_sec_boot
:end_parse_args_sec_boot

if "%S_FILE%" == "" (
  :secure_file_missed
  echo [ERR] ^<Secure_File.hex^> is missed
  echo:
  goto usage
)
if "%NS_FILE%" == "" (
  :non_secure_file_missed
  echo [ERR] ^<Non_Secure_File.hex^> is missed
  echo:
  goto usage
)
if "%OUTPUT_FILE%" == "" (
  :output_file_missed
  echo [ERR] ^<Output_Binary_File.bin^> is missed
  echo:
  goto usage
)
REM if "%ALIGN_ADDRESS%" == "" (
  REM :align_address_missed
  REM echo [ERR] ^<0xAlign_Address^> is missed
  REM echo:
  REM goto usage
REM )

echo Concatenating...
echo ^<Secure_File.hex^> = %S_FILE%
echo ^<Non_Secure_File.hex^> = %NS_FILE%
echo ^<Output_Binary_File.bin^> = %OUTPUT_FILE%
echo ^<0xAlign_Address^> = %ALIGN_ADDRESS%

if "%ALIGN_ADDRESS%" == "" (
@echo on
..\tools\srec_cat %S_FILE% -Intel -offset - -minimum-addr %S_FILE% -Intel %NS_FILE% -Intel -o %OUTPUT_FILE% -Binary
@echo off
) else (
@echo on
..\tools\srec_cat %S_FILE% -Intel -offset - -minimum-addr %S_FILE% -Intel %NS_FILE% -Intel -fill 0xAB -maximum-addr %NS_FILE% -Intel %ALIGN_ADDRESS% -o %OUTPUT_FILE% -Binary
@echo off
)

rem -Intel -line-length=44

goto end_make

:parse_args_cfpa
set PARAMETER=%~2
set ARGUMENT=%~3
if "%PARAMETER%" == "-app" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set FILE_APP=%ARGUMENT%
      shift
    ) else (
      goto file_missed
    )
  ) else (
      goto file_missed
  )
) else if "%PARAMETER%" == "-o" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set OUTPUT_FILE=%ARGUMENT%
      shift
    ) else (
      goto output_file_missed
    )
  ) else (
      goto output_file_missed
  )
) else if "%PARAMETER%" == "-a" (
  shift
  if not "%ARGUMENT%" == "" (
    if not "%ARGUMENT:~0,1%" == "-" (
      set ALIGN_ADDRESS=%ARGUMENT%
      shift
    ) else (
      goto align_address_missed
    )
  ) else (
      goto align_address_missed
  )
) else if "%PARAMETER%" == "" (
  goto end_parse_args_cfpa
) else (
  goto usage
)
goto parse_args_cfpa
:end_parse_args_cfpa

if "%FILE_APP%" == ""  (
  :file_missed
  echo [ERR] ^<File.hex or File.bin^> is missed
  echo:
  goto usage
)
if "%OUTPUT_FILE%" == "" (
  :output_file_missed
  echo [ERR] ^<Output_Binary_File.bin^> is missed
  echo:
  goto usage
)
if "%ALIGN_ADDRESS%" == "" (
  :align_address_missed
  echo [ERR] ^<0xAlign_Address^> is missed
  echo:
  goto usage
)

echo Concatenating...
echo ^<File.hex^> = %FILE_APP%
echo ^<Table.bin^> = %FILE_TABLE%
echo ^<Output_Binary_File.bin^> = %OUTPUT_FILE%
echo ^<0xAlign_Address^> = %ALIGN_ADDRESS%

@echo on
call ..\tools\srec_cat %FILE_APP% -Intel -fill 0xAB 0x20200 %ALIGN_ADDRESS% -o %OUTPUT_FILE% -Binary 
@echo off

goto end_make

:usage
echo Usage: make_bin.bat -secure_boot -s ^<Secure_File.hex^> -ns ^<Non_Secure_File.hex^> -o ^<Output_Binary_File.bin^> -a ^<0xAlign_Address^>
echo        or
echo        make_bin.bat -tfm -app ^<File.hex^> -table ^<File.bin^> -o ^<Output_Binary_File.bin^> -a ^<0xAlign_Address^>

:end_make
exit /b 1