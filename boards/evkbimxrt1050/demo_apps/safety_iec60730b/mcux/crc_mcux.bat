arm-none-eabi-size %1.axf
arm-none-eabi-objcopy -v -O binary %1.axf %1.bin >nul 2>&1
..\tools\srec\srec_cat %1.bin -binary -o %1.srec >nul 2>&1
..\tools\srec\srec_cat %1.srec -crop 0x2410 0x5000 -fill 0xFF 0x2410 0x5000 -Bit_Reverse -CRC32LE 0x2400 -Bit_Reverse -XOR 0xff -Output %1_TEMP.srec >nul 2>&1
..\tools\srec\srec_cat %1_TEMP.srec -crop 0x2400 0x2410 -byte-swap 4 -Output %1_TEMP.srec >nul 2>&1
..\tools\srec\srec_cat %1.srec -exclude 0x2400 0x2410 -Output %1.srec >nul 2>&1
..\tools\srec\srec_cat %1.srec %1_TEMP.srec -Output %1_CRC.srec >nul 2>&1
arm-none-eabi-objcopy --input-target=srec --output-target=binary %1_CRC.srec %1_CRC.bin >nul 2>&1