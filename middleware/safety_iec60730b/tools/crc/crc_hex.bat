@echo off
Pushd "%~dp0"

rem
rem Copyright 2021 NXP.
rem All rights reserved.
rem
rem SPDX-License-Identifier: BSD-3-Clause
rem

echo #     
echo #----------------------------------
echo #      CRC CALCULATOR V1.3
echo #----------------------------------

rem Get the arguments
set hex_file=%1
set hex_file=%hex_file:~1%
set hex_file_out=%2
set hex_file_out=%hex_file_out:~1%
set srec_cat_loc=%3
set srec_cat_loc=%srec_cat_loc:~1%
set crc_size=%4
set crc_size=%crc_size:~1%

rem **************************************************************************
rem Check the input hex file name and path argument
if not exist %hex_file% (
    echo # ERROR: Invalid HEX file path or name "%hex_file%"! Please fix the first argument.
    echo #        CRC NOT updated.
    GOTO end_of_file
)

rem Check the input srec_cat.exe path argument
set srec_cat_file=%srec_cat_loc%
if not exist %srec_cat_file% (
    echo # ERROR: Invalid srec_cat.exe file path "%srec_cat_file%"! Please fix the third argument.
    echo #        CRC NOT updated.
    GOTO end_of_file
)

rem **************************************************************************
echo #
echo # Getting CRC information:
echo # -------------------------------

rem Read the last 16 bytes of the HEX file
if exist temp.srec del temp.srec

rem Convert to srec to get the full CRC address
for /f %%i in ('%~dp0%srec_cat_loc% %hex_file%^
    -intel^
    -Address_Length=4^
    -Line_Length=18^
    -o temp.srec') do ( echo DONE )

rem Read the number of lines.
for /f %%i in ('findstr /R /N "^" "temp.srec" ^| find /C ":"') do @set num_of_lines=%%i

set /a lines_read=0
set /a line_id=1
set CRC_info_table=
:loop_read_table

    rem Get the next line from the end
    call :FCN_GetLine temp.srec , %num_of_lines%-line_id

    rem Parse the line
    call :FCN_ParseLine %FCN_GetLine_return%

    rem Check whether data line was encountered
    if %FCN_ParseLine_RetType%=="DATA" (
        set CRC_info_table=%FCN_ParseLine_RetContent%%CRC_info_table%
        set /a lines_read+=1
    )

rem Check whether we read enough bytes to get the CRC info table
if NOT %lines_read% GTR 7 (
    set /a line_id+=1
    rem Check whether we reached the beginning of the file
    if %line_id% GEQ %num_of_lines% (
        echo # ERROR: Could not find the CRC info table!
        echo #        CRC NOT updated.
        GOTO end_of_file
    )
    GOTO loop_read_table
)

rem Delete the temporary file if previously created
if exist temp.srec del temp.srec

rem Reverse the bytes due to endianess
call :FCN_ReverseBytes %CRC_info_table%
set CRC_info_table=%FCN_ReverseBytes_return%

rem Check whether this is the CRC info table
if NOT "%CRC_info_table:~0,4%"=="5AA5" (
    echo # ERROR: Could not find the CRC info table start!
    echo #        CRC NOT updated.
    GOTO end_of_file
)
if NOT "%CRC_info_table:~-4,4%"=="A55A" (
    echo # ERROR: Could not find the CRC info table end!
    echo #        CRC NOT updated.
    GOTO end_of_file
)

rem Parse the CRC info table
set CRC_actual=%CRC_info_table:~8,8%
set CRC_end=%CRC_info_table:~12,8%
set CRC_start=%CRC_info_table:~20,8%


rem Print the success message
echo # CRC table found at: 0x%FCN_ParseLine_RetAddressHEX%
echo # Safety FLASH start: 0x%CRC_start%
echo # Safety FLASH end:   0x%CRC_end%



rem **************************************************************************
echo #
echo # Calculating new CRC value:
echo # -------------------------------

rem Configure the srec_cat command based on required CRC length
if %crc_size%==CRC32 (
   set cmd_crc=-Bit_Reverse -CRC32LE 0xFFFFFFF0 -Bit_Reverse -XOR 0xff
   set cmd_addr=-crop 0xFFFFFFF0 0xFFFFFFF4
) else (
   set cmd_crc=-CRC16_Little_Endian 0xFFFFFFF0 -xmodem 
   set cmd_addr=-crop 0xFFFFFFF0 0xFFFFFFF2
)

rem Calculate the CRC of selected region using srec_cat.exe
for /f %%i in ('%~dp0%srec_cat_loc% %hex_file%^
    -intel -crop 0x%CRC_start% 0x%CRC_end%^
    -fill 0xFF 0x%CRC_start% 0x%CRC_end%^
    %cmd_crc%^
    %cmd_addr%') do (
        for /F "tokens=*" %%a in ("%%i") do (
            setlocal EnableDelayedExpansion
            set line=%%a
            if "!line:~0,3!"=="S30" (
                if %crc_size%==CRC32 (
                    rem Get the CRC value
                    set CRC_actual=!line:~12,8!
                    rem Print the success message
                    echo # CRC-32 value: 0x!CRC_actual!
                ) else (
                    rem Get the CRC value (reverse bytes due to little endian)
                    set CRC_actual=!line:~-4,2!!line:~-6,2!
                    rem Print the success message
                    echo # CRC-16/AUG-CCITT value: 0x!CRC_actual!
                    rem Add four upper bytes
                    set CRC_actual=0000!CRC_actual!
                )
            )
            setlocal DisableDelayedExpansion
        )
    )

rem **************************************************************************
echo #
echo # Saving new CRC value:
echo # -------------------------------

rem Get CRC location addresses for srec_cat.exe script
set /a temp_var=0x%FCN_ParseLine_RetAddressHEX%+10
call cmd /c exit /b %temp_var%
set crc_addr_start=%=exitcode%
set /a temp_var=0x%FCN_ParseLine_RetAddressHEX%+14
call cmd /c exit /b %temp_var%
set crc_addr_end=%=exitcode%
set /a temp_var=0x%FCN_ParseLine_RetAddressHEX%+16
call cmd /c exit /b %temp_var%
set crc_table_end=%=exitcode%

rem Place the CRC into HEX file
%~dp0%srec_cat_loc% %hex_file%^
    -intel -crop 0x00000000 0x%crc_addr_start%^
    -generate 0x%crc_addr_start% 0x%crc_addr_end%  -constant-l-e 0x%CRC_actual% 4^
    -generate 0x%crc_addr_end%   0x%crc_table_end% -constant-l-e 0x5AA5 2^
    -o %hex_file_out% -intel 

rem Print the success message
echo # CRC written at 0x%crc_addr_start%

rem **************************************************************************
rem Exit is below
:end_of_file
echo #
popd

EXIT /B 0


rem **************************************************************************
rem                             FUNCTIONS
rem **************************************************************************

rem Function returns the content of selected file line
rem ARG1: File name
rem ARG2: Line number
rem RETURN: FCN_GetLine_return - the requested line
:FCN_GetLine
    rem Store the line number
    set FCN_GetLine_file_name=%~1
    set /a FCN_GetLine_m=%~2

    rem Get the requested line
    set FCN_GetLine_return="FAIL"
    for /f %%i in ('more +%FCN_GetLine_m% "%FCN_GetLine_file_name%"') do (
        set FCN_GetLine_return=%%i
        rem The first line
        GOTO FCN_GetLine_done01
    )
    :FCN_GetLine_done01

EXIT /B 0

rem Function parse the s-rec file line
rem ARG1: Line text to be parsed
rem RETURN: FCN_ParseLine_RetAddressHEX - entry address in HEX
rem RETURN: FCN_ParseLine_RetType - entry type (DATA or OTHER)
rem RETURN: FCN_ParseLine_RetContent - the 2B message content
:FCN_ParseLine
    rem Store the line number
    set FCN_ParseLine_line=%~1

    set FCN_ParseLine_RetType=%FCN_ParseLine_line:~0,2%
    if "%FCN_ParseLine_RetType%"=="S3" (
        set FCN_ParseLine_RetType="DATA"
        set FCN_ParseLine_RetAddressHEX=%FCN_ParseLine_line:~4,8%
        set FCN_ParseLine_RetContent=%FCN_ParseLine_line:~-6,4%
    ) else (
        set FCN_ParseLine_RetType="OTHER"
        set FCN_ParseLine_RetAddressHEX=0x00000000
        set FCN_ParseLine_RetContent=0x0000
    )
EXIT /B 0

rem Function reverses the byte order in the entire argument
rem ARG1: Line text to be byte-reversed
rem RETURN: FCN_ReverseBytes_line - byte-reversed line
:FCN_ReverseBytes
    rem Store the line
    set FCN_ReverseBytes_line=%~1
    set FCN_ReverseBytes_line=XX%FCN_ReverseBytes_line%
    set FCN_ReverseBytes_return=

:FCN_ReverseBytes_loop01
    rem Append the bytes in reversed order
    set FCN_ReverseBytes_return=%FCN_ReverseBytes_return%%FCN_ReverseBytes_line:~-2,2%
    set FCN_ReverseBytes_line=%FCN_ReverseBytes_line:~0,-2%
    if NOT "%FCN_ReverseBytes_line%"=="XX" (
        goto FCN_ReverseBytes_loop01
    )

EXIT /B 0

