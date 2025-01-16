Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT700-EVK board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
1. Set the hardware jumpers (Tower system/base module) to default settings.

Prepare the Demo
================
NOTE: To be able to build the DSP project, please see the document
'Getting Started with Xplorer for MIMXRT700-EVK.pdf'.

1.  Connect headphones to Audio HP connector.
2.  Connect a micro USB cable between the PC host and the debug USB port on the board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program for CM33 core to the target board.
5.  Launch the debugger in your IDE to begin running the demo.
6.  If building debug configuration, start the xt-ocd daemon and download the program for DSP core to the target board.
7.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to begin running the demo.

NOTE: DSP image can only be debugged using J-Link debugger. See again
'Getting Started with Xplorer for MIMXRT700-EVK.pdf' for more information.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display similar output:

    ******************************
    DSP audio framework demo start
    ******************************

    [CM33 Main] Configure codec

    [DSP_Main] Cadence Xtensa Audio Framework
    [DSP_Main] Library Name    : Audio Framework (Hostless)
    [DSP_Main] Library Version : 3.5
    [DSP_Main] API Version     : 3.2

    [DSP_Main] start
    [DSP_Main] established RPMsg link
    [CM33 Main] DSP image copied to DSP TCM
    [CM33 Main][APP_DSP_IPC_Task] start
    [CM33 Main][APP_Shell_Task] start

    Copyright  2024  NXP

    >>

Demo commands:

    "help": List all the registered commands

    "exit": Exit program

    "version": Query DSP for component versions

    "record_dmic": Record DMIC audio , perform voice recognition (VIT) and playback on codec
    USAGE: record_dmic [language]
    For voice recognition say supported WakeWord and in 3s frame supported command.
    If selected model contains strings, then WakeWord and list of commands will be printed in console.
    NOTE: this command does not return to the shell


After running the "record_dmic en" command, similar output will be printed

    [CM33 CMD] Setting VIT language to en
    [DSP_Main] Number of channels 1, sampling rate 16000, PCM width 32
    [CM33 CMD] [APP_DSP_IPC_Task] response from DSP, cmd: 13, error: 0
    [DSP Record] Audio Device Ready
    [CM33 CMD] DSP DMIC Recording started
    [CM33 CMD] To see VIT functionality say wakeword and command
    [DSP VIT] VIT Model info
    [DSP VIT]   VIT Model Release = 0x40a00
    [DSP VIT]   Language supported : English
    [DSP VIT]   Number of WakeWords supported : 2
    [DSP VIT]   Number of Commands supported : 12
    [DSP VIT]   VIT_Model integrating WakeWord and Voice Commands strings : YES
    [DSP VIT]   WakeWords supported :
    [DSP VIT]    'HEY NXP'
    [DSP VIT]    'HEY TV'
    [DSP VIT]   Voice commands supported :
    [DSP VIT]    'MUTE'
    [DSP VIT]    'NEXT'
    [DSP VIT]    'SKIP'
    [DSP VIT]    'PAIR DEVICE'
    [DSP VIT]    'PAUSE'
    [DSP VIT]    'STOP'
    [DSP VIT]    'POWER OFF'
    [DSP VIT]    'POWER ON'
    [DSP VIT]    'PLAY MUSIC'
    [DSP VIT]    'PLAY GAME'
    [DSP VIT]    'WATCH CARTOON'
    [DSP VIT]    'WATCH MOVIE'
    [DSP Record] connected CAPTURER -> GAIN_0
    [DSP Record] connected XA_GAIN_0 -> XA_VIT_PRE_PROC_0
    [DSP Record] connected XA_VIT_PRE_PROC_0 -> XA_RENDERER_0
    [DSP VIT]  - WakeWord detected 1 HEY NXP
    [DSP VIT]  - Voice Command detected 6 STOP
