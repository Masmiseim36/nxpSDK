set OUT=%1.out
set BIN=%1.bin

:: calculate application checksum
ielftool --fill 0xFF;0x8000A30-0x8001003 --checksum __checksum:4,crc32:i,0xFFFFFFFF;0x8000A30-0x8001003 --verbose %OUT% %OUT%

:: generate additional output: binary
ielftool.exe --bin --verbose %OUT% %BIN%