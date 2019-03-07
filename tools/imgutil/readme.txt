
   IMGUTIL User Guide
  
   ======================================
   
   1. Build SDK2.0 application
   2. Copy built binary (.bin file) to imgutil/<board> folder and rename to sdk20-app.bin
   3. In imgutil/<board> folder, run mkimage.sh in mingw32 shell to get bootable image file sdk20-app.img
       a. If the image is built with ram link file, use "mkimage.sh ram" to make the bootable image.
       b. If the image is built with flash link file, use "mkimage.sh flash" to make the bootable XIP image.
       c. If the image is built with ram link file and want to boot from MicroSD card, use "mkimage.sh sd" to make the bootable image.

   ======================================
