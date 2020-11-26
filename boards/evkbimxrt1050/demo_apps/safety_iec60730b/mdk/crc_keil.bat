REM 1. crop tested range and compute crc. This step reverse bytes.
REM 2. cut crc.
REM 3. merge crc into original hex file.
.\..\srec_cat .\debug\safety_iec60730b.hex -intel -crop 0x60002410 0x60005000 -fill 0xFF 0x60002410 0x60005000 -Bit_Reverse -CRC32LE 0x60002400 -Bit_Reverse -XOR 0xff -o .\debug\tmp_cropped.hex -intel 
.\..\srec_cat .\debug\tmp_cropped.hex -intel -crop 0x60002400 0x60002410 -byte-swap 4 -o .\debug\tmp_crc_cut.hex -intel
.\..\srec_cat .\debug\safety_iec60730b.hex -intel .\debug\tmp_crc_cut.hex -intel -o .\debug\safety_iec60730b_crc.hex -intel 

REM remove artifacts
IF EXIST .\debug\tmp_cropped.hex DEL /F .\debug\tmp_cropped.hex
IF EXIST .\debug\tmp_crc_cut.hex DEL /F .\debug\tmp_crc_cut.hex
