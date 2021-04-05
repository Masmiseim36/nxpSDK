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
- MIMX8MN6-EVK  board
- J-Link Debug Probe
- 12V power supply
- Personal Computer
- Headphone

Board settings
==============
No special settings are required.

#### Note! ####
1.  This case does not support ddr target because of the mass of the music data decoded by A core will be placed in DDR. 

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
1.  The 16/24/32bits for PCM Music stream are supported.
2.  Since the music files are typically large, users could create a new large size patition in the SD card to place the music files.
3.  After M core running, please boot the linux kernel to create the rpmsg channel between A core and M core.
4.  Please make sure there exists xxx.wav file in the SD card.
    If the music file is placed at the Windows FAT32 paritions, after the linux kernel boots up and logs on as root,
    using the "mount /dev/mmcblk1p1 /mnt" and then go to "/mnt" folder to playabck the music using the playback command.
    If the music file is placed at the Linux paritions, eg "/home", could playback the music directly using the playback command. 

******************
Playback command
******************
Note:
1. Please use the command "cat /proc/asound/cards" to check the wm8524 sound card number.
E.g: Type command:
        ~# cat /proc/asound/cards
     The available sound cards can be shown:
     0 [wm8524audio    ]: wm8524-audio - wm8524-audio
                          wm8524-audio
     1 [imxspdif       ]: imx-spdif - imx-spdif
                          imx-spdif
     2 [imxaudiomicfil ]: imx-audio-micfi - imx-audio-micfil
                          imx-audio-micfil

Then the wm8524 sound number is 0.

When playback the .wav file:
1.  If want to playabck with pause/resume command, could use command: 
      "aplay -Dhw:0 -i xxx.wav -N";
    press space key on the keyboard to pause, and press the space key again to resume.
2.  If want to playback with low power mode and specified period-size, could use command:
      "aplay -Dhw:0 --buffer-size=xxx --period-size=xxx xxx.wav -N &" or
      "aplay -Dhw:0 --buffer-time=xxx --period-time=xxx xxx.wav -N &".
    E.g: aplay -Dhw:0 --period-time=500000 --buffer-time=10000000 xxx.wav -N &
    Now please use "echo mem > /sys/power/state" command to make A core enter suspend mode and the playabck work normally.

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




