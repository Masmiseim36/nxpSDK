Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT685 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
To enable the example audio using WM8904 codec, connect pins as follows:
- JP7-1        <-->        JP8-2

Prepare the Demo
================
1. Connect headphones to Audio HP / Line-Out connector (J4).
2. Connect a micro USB cable between the PC host and the debug USB port (J5) on the board
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program for CM33 core to the target board.
5. Launch the debugger in your IDE to begin running the demo.
6. If building debug configuration, start the xt-ocd daemon and download the program for
   DSP core to the target board.
7. If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

**Notes:**
- DSP image can only be debugged using J-Link debugger.  See again
'Getting Started with Xplorer for EVK-MIMXRT685.pdf' for more information.
- To be able to build the DSP project, please see the document
'Getting Started with Xplorer for EVK-MIMXRT685.pdf'.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:
```
    ******************************
    DSP audio framework demo start
    ******************************

    Configure WM8904 codec
    [APP_DSP_IPC_Task] start
    [APP_Shell_Task] start

    Copyright  2022  NXP
    >>
```

Demo commands:
```
"help": List all the registered commands

"record_dmic": Record DMIC audio, perform voice recognition (VIT) and playback on WM8904 codec
  For voice recognition pass desired supported language as an argument.
  After command starts it will provide basic information about selected language model.
  If the language model has defined names of supported commands, then supported commands will
  be printed as a list.
  To see functionality say supported WakeWord and in 3s frame spported command.
  NOTE: For more information about VIT, wakeword and supported commands see
  VIT_Integration_Guide.pdf.
```

When record_dmic command runs successfully, the terminal will display following output:
```
    [APP_DSP_IPC_Task] response from DSP, cmd: 21, error: 0
    DSP DMIC Recording started
    To see VIT functionality say wakeword and command
```

  Xtensa IDE log of successful start of command:
```
    Number of channels 2, sampling rate 16000, PCM width 16
    Audio Device Ready
    connected CAPTURER -> GAIN_0
    connected CAPTURER -> XA_VIT_PRE_PROC_0
    connected XA_VIT_PRE_PROC_0 -> XA_RENDERER_0
```

Running the demo DSP
===================
Debug configuration:
When the demo runs successfully, the terminal will display the following:
```
    Cadence Xtensa Audio Framework
      Library Name    : Audio Framework (Hostless)
      Library Version : 3.2
      API Version     : 3.0

    [DSP_Main] start
    [DSP_Main] established RPMsg link
    Number of channels 2, sampling rate 16000, PCM width 16

    Audio Device Ready
    VoiceSeekerLight lib initialized!
    ============= VoiceSeekerLight Configuration =============
      version = 0.6.0
      num mics = 2
      max num mics = 4
      mic0 = (35, 0, 0)
      mic1 = (-35, 0, 0)
      mic2 = (0, -35, 0)
      num_spks = 0
      max num spks = 2
      samplerate = 16000
      framesize_in = 32
      framesize_out = 480
      create_aec = 0
      create_doa = 0
      buffer_length_sec = 1.5
      aec_filter_length_ms = 0
    ============= VoiceSeekerLight Memory Allocation =============
      VoiceSeekerLib allocated 80592 persistent bytes
      VoiceSeekerLib allocated 3840 scratch bytes
    ==================================== VoiceSeekerLight Memory Usage =========================
    =========
      Total                 = 72400 bytes

    connected CAPTURER -> GAIN_0
    connected XA_GAIN_0 -> XA_VOICE_SEEKER_0
    connected XA_VOICE_SEEKER_0 -> XA_VIT_PRE_PROC_0
    connected XA_VIT_PRE_PROC_0 -> XA_RENDERER_0
```

