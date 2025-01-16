Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT700-EVK board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
1. Set the hardware jumpers to default settings.

Prepare the Demo
================
NOTE: To be able to build the DSP project, please see the document
'Getting Started with Xplorer for MIMXRT700-EVK.pdf'.

1.  Connect all headphones to Audio HP connector (J29).
2.  Connect a micro USB cable between the PC host and the debug USB port (J54) on the board
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program for CM33 core to the target board.
5.  Launch the debugger in your IDE to begin running the demo.
6.  If building debug configuration, start the xt-ocd daemon and download the program for DSP core to the target board.
7.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

NOTES:
1. DSP image can only be debugged using J-Link debugger. See again 'Getting Started with Xplorer for MIMXRT700-EVK.pdf' for more information.
2. RT700 has limited RAM on Cortex-M33 core 1 which brings limitation of decoders and encoders. Only SBC decoder is enabled by default.
   In order to enable a different decoder/encoder, it is necessary to define the appropriate define on project level.
   Use one of the following define from the list of the supported decoders on the HiFi1 core:
   XA_AAC_DECODER
   XA_MP3_DECODER
   XA_SBC_DECODER
   XA_VORBIS_DECODER
   XA_OPUS_DECODER


Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:

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
  [CM33_Main] DSP image copied to DSP TCM
  [CM33_Main][APP_SDCARD_Task] start
  [CM33_Main][APP_DSP_IPC_Task] start
  [CM33_Main][APP_Shell_Task] start

  Copyright  2024  NXP

Demo commands:

  "help": List all the registered commands

  "exit": Exit program

  "version": Query DSP for component versions

  "decoder": Perform decode on DSP and play to speaker.
    USAGE: decoder [aac|]
    aac:        Decode aac data

When decoder command starts playback successfully, the terminal will display following output:

  >> decoder aac
  [DSP_Main] Input buffer addr: 0x20020000, buffer size: 94276
  [DSP Codec] Audio Device Ready
  [DSP Codec] Decoder created
  [DSP Codec] Decoder component started
  [DSP Codec] Renderer component created
  [DSP Codec] Connected XA_DECODER -> XA_RENDERER
  [DSP_ProcessThread] start
  [DSP_ProcessThread] Execution complete - exiting
  [DSP_ProcessThread] exiting
  [DSP Codec] Audio device closed

  [CM33 CMD] [APP_DSP_IPC_Task] response from DSP, cmd: 0, error: 0
  [CM33 CMD] Decode complete

