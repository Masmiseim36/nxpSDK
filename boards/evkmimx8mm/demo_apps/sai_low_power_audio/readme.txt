Overview
========
In this demo, A core decoded music data and put it to DDR buffer and informs M core with the related information. 
Then M core will take the ownership of consuming the buffer, it will copy buffer from DDR to TCM, manipulating SDMA to transfer the data to SAI and codec for playback. 
It gives DDR and A core opportunity to do power saving for rather long time frame. M core will also take ownership of codec initialization.
SRTM(Simplified Real Time Messaging) protocol is used to communicate between A core and M core. 
The protocol provides various commands for A core and M core to communicate with each other. 
If there is no audio palyback, M core will enter the STOP mode, and the whole SOC system would enter deep sleep mode(DSM) once A core enter low power status.

Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- GCC ARM Embedded  9.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8MM6-EVK  board
- J-Link Debug Probe
- 12V power supply
- Personal Computer
- Headphone

Board settings
==============
No special settings are required.

#### Note! ####
1.  This case does not support ddr and flash target. 

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW101 to power on the board
2.  Connect a USB cable between the host PC and the J901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the demo.

******************
NOTE
******************
1.  The 16/24/32bit for PCM and DSD64/128/256/512( DSD playabck only supported by ak4497 codec) Music stream are supported
2.  The wm8524 codec on the EVK board and the ak4497 codec on the audio board are both supported,
    but please note that only one codec can be used at the same time which is determained by the macro "APP_SRTM_CODEC_WM8524_USED" and "APP_SRTM_CODEC_AK4497_USED" in app_srtm.h.
3.  Since the  DSD files are typically large, users could create a new large size patition in the SD card to place the music files.
4.  After M core running, please boot the linux kernel to create the rpmsg channel between A core and M core.
5.  Please make sure here exists xxx.wav file in the SD card.
    If the music file is placed at the Windows FAT32 paritions, after the linux kernel boot up and logged as root,
    using the "mount /dev/mmcblk1p1 /mnt" and then go to "/mnt" folder to playabck the music using the playback command.
    If the music file is placed at the Linux paritions, eg "/home", could playback the music dirctly using the playback command. 

******************
Playback command
******************
Note:
1. Please use the command "cat /proc/asound/cards" to check the ak4497 sound card number.
E.g: Type command:
        ~# cat /proc/asound/cards
    The available sound cards can be shown:
     0 [imxspdif       ]: imx-spdif - imx-spdif
                          mx-spdif
     1 [imxaudiomicfil ]: imx-audio-micfi - imx-audio-micfil
                          imx-audio-micfil
     2 [btscoaudio     ]: bt-sco-audio - bt-sco-audio
                          bt-sco-audio
     3 [ak4497audio    ]: ak4497-audio - ak4497-audio
                          ak4497-audio
Then the ak4497 sound number is 3.
2. If use the WM8524 codec, still use the ak4497 sound card number.

When playback the .wav file:
1.  If want to playabck with pause/resume command, could use command: 
      "aplay -Dhw:3 -i xxx.wav -N";
    press space key on the keyboard to pause, and press the space key again to resume.
2.  If want to playback with low power mode and specified period-size, could use command:
      "aplay -Dhw:3 --buffer-size=xxx --period-size=xxx xxx.wav -N &" or
      "aplay -Dhw:3 --buffer-time=xxx --period-time=xxx xxx.wav -N &".
    E.g: aplay -Dhw:3 --period-time=500000 --buffer-time=10000000 xxx.wav -N &
    Now please use "echo mem > /sys/power/state" command to make A core enter suspend mode and the playabck work normally.

    
When playback the .dsd/.dff file (only supported by AK4497 codec): 
1.  Enter folder where the DSD execution procedure exists, using command:
     "cd /unit_tests/ALSA_DSD"
2.  If want to test play back with specified period-size, could use command:
      "./mxc_alsa_dsd_player -Dhw:0 --buffer-size=xxx --period-size=xxx music path" or
      "./mxc_alsa_dsd_player -Dhw:0 --buffer-time=xxx --period-time=xxx music path"
    Please note that the "music path" means where the DSD file exists.
3.  Support music playabck when A core enters suspend.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
####################  LOW POWER AUDIO TASK ####################

    Build Time: Apr  8 2020--15:27:22 
********************************
 Wait the Linux kernel boot up to create the link between M core and A core.
********************************
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
M core is running now, please boot the linux kernel and use the command to playback music.




