#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu" ]]; then
  ARCH=`uname -m`
  if [[ "$ARCH" == "i686" ]]; then
    IMGUTIL=../bin/imgutil
  else
    IMGUTIL=../bin/imgutil64
  fi
else
  IMGUTIL=../bin/imgutil.exe
fi

if [ "$#" -ne 1 ] || ! [ "$1" == "ram" -o "$1" == "flash" ] ; then
  echo "Usage: $0 target"
  echo "       target: ram -- the image will be loaded to RAM and run, the application must be built with ram link file"
  echo "       target: flash -- the image will be run on flash directly, the application must be build with flash link file"
  echo "Example: $0 ram"
  exit 1
fi

#../dcdgen dcd.config dcd.bin
if [ "$1" == "ram" ]; then
    $IMGUTIL --combine base_addr=0x1FFD0000 ivt_offset=0x1000 config_offset=0x400 config_file=qcb.bin app_offset=0x2000 app_file=sdk20-app.bin ofile=sdk20-app.img
elif [ "$1" == "flash" ]; then
    $IMGUTIL --combine base_addr=0x04000000 ivt_offset=0x1000 config_offset=0x400 config_file=qcb.bin app_offset=0x2000 app_file=sdk20-app.bin ofile=sdk20-app.img
fi
