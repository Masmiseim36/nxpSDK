rem Pass in full path and Release|Debug to iar project directory
cd /d %1

arm-none-eabi-objcopy -I elf32-littlearm -O binary release\flashloader.out flashloader.bin
python ..\..\..\..\..\middleware\mcu-boot\bin\create_fl_image_mdk.py release\flashloader.out flashloader.bin flashloader_image.c

arm-none-eabi-objcopy -I elf32-littlearm -O binary debug\flashloader.out flashloader.bin
python ..\..\..\..\..\middleware\mcu-boot\bin\create_fl_image_mdk.py debug\flashloader.out flashloader.bin flashloader_image.c
