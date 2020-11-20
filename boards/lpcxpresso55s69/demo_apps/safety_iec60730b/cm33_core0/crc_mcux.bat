arm-none-eabi-size %1.axf
arm-none-eabi-objcopy -v -O binary %1.axf %1.bin >nul 2>&1
..\tools\srec\srec_cat %1.bin -binary -o %1.srec >nul 2>&1
..\tools\srec\srec_cat %1.srec -crop 0x100 0x300 -fill 0xFF 0x100 0x300 -CRC16_Little_Endian 0xFFF0 -Output %1_TEMP.srec >nul 2>&1
..\tools\srec\srec_cat %1_TEMP.srec -crop 0xFFF0 0x10000 -Output %1_TEMP.srec >nul 2>&1
..\tools\srec\srec_cat %1.srec -exclude 0xFFF0 0x10000 -Output %1.srec >nul 2>&1
..\tools\srec\srec_cat %1.srec %1_TEMP.srec -Output %1_CRC.srec >nul 2>&1
arm-none-eabi-objcopy --input-target=srec --output-target=binary %1_CRC.srec %1_CRC.bin >nul 2>&1