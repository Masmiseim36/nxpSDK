REM 1. crop tested range and compute crc. This step reverse bytes.
REM 2. cut crc.
REM 3. merge crc into original hex file.
.\..\..\..\..\..\tools\srec\srec_cat .\release\safety_iec60730b.hex -intel -crop 0x60002410 0x60005000 -fill 0xFF 0x60002410 0x60005000 -Bit_Reverse -CRC32LE 0x60002400 -Bit_Reverse -XOR 0xff -o .\release\tmp_cropped.hex -intel 
.\..\..\..\..\..\tools\srec\srec_cat .\release\tmp_cropped.hex -intel -crop 0x60002400 0x60002410 -byte-swap 4 -o .\release\tmp_crc_cut.hex -intel
.\..\..\..\..\..\tools\srec\srec_cat .\release\safety_iec60730b.hex -intel .\release\tmp_crc_cut.hex -intel -o .\release\safety_iec60730b_crc.hex -intel 

REM remove artifacts
IF EXIST .\release\tmp_cropped.hex DEL /F .\release\tmp_cropped.hex
IF EXIST .\release\tmp_crc_cut.hex DEL /F .\release\tmp_crc_cut.hex
