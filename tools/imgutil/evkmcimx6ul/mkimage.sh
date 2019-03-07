#!/bin/bash

function usage()
{
  echo "Usage: $0 target"
  echo "       target: ram -- the image will be loaded to RAM and run, the application must be built with ram link file"
  echo "       target: flash -- the image will be run on flash directly, the application must be build with flash link file"
  echo "       target: sd -- the image will be loaded from SD to RAM and run, the application must be build with ram link file"
  echo "Example: $0 ram"
}

if [ "$#" -ne 1 ]; then
  usage $0
  exit 1
fi

SYSTEM=`uname -s`
if [ $SYSTEM == "Linux" ]; then
    DCD_BUILDER=dcdgen.bin
    IMG_BUILDER=imgutil.bin
else
    DCD_BUILDER=dcdgen.exe
    IMG_BUILDER=imgutil.exe
fi

../bin/$DCD_BUILDER dcd.config dcd.bin
if [ "$1" == "ram" ]; then
    ../bin/$IMG_BUILDER --combine base_addr=0x80000000 ivt_offset=0x1000 app_offset=0x2000 dcd_file=dcd.bin app_file=sdk20-app.bin ofile=sdk20-app.img image_entry_point=0x80002000
elif [ "$1" == "flash" ]; then
    ../bin/$IMG_BUILDER --combine base_addr=0x60000000 ivt_offset=0x1000 app_offset=0x2000 dcd_file=dcd.bin app_file=sdk20-app.bin ofile=sdk20-app.img image_entry_point=0x60002000
elif [ "$1" == "sd" ]; then
    ../bin/$IMG_BUILDER --combine base_addr=0x80000000 ivt_offset=0x400 app_offset=0x2000 dcd_file=dcd.bin app_file=sdk20-app.bin ofile=sdk20-app.img image_entry_point=0x80002000
else
    echo "Unsupported target $1"
    usage $0
fi
