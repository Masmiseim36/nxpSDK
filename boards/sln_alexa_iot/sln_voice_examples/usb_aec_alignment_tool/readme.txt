Overview
========


Toolchain supported
===================
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- SEGGER J-Link

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
4. Download the program to the target board.
5. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
This assumes the reader is using Audcity but other software can be used.

The purpose of this application is to obtain all the audio streams which consist of:
- Microphone one 
- Microphone two
- Microphone three
- Amplifier Reference Signal
- Output from the AFE (Noise Supression, Echo Cancellation)

For optimal barge performance, the Amplifier Reference Signal needs to be aligned to the Microphone Streams. 
When the AFE takes these streams, if they overlap, then it is able to remove the speaker audio that the device is playing itself.

To do this, visual inspection of the alignment needs to be done as the distance (speed of sound) to the microphones and speakers will be different
for different products.

1. The device will enumerate as a VCOM device. Locate the COM Port of the device
2. In the Scripts folder of the code, there is a python script called parse_audio_streams.py
3. Ensure Python 3.7 is installed
4. Ensure all python dependencies are resolved via pip (pyserial)
5. Run the script "python parse_audio_streams.py"
6. Enter the name of the test which will subsequently be the name of the audio captures
7. Enter the COM number (3/4/5) to which the device was enumerated a
8. Enter the number of microphones that have neen configured in the code (default is three)
9. The device will then start capturing audio real time and outputting the data into the files within the current directory.
10. Once captures, CTRL + C to stop.
11. Within Audacity, to import the audio File -> Import -> Raw Audio
12. Select the files individually <test name>_mic_stream1 <test name>_mic_stream2 <test name>_mic_stream3 <test name>_amp_ref_stream <test name>_clean_processed_audio
13. Ensure the Encoding is Signed 16-Bit PCM, Little Endian and Mono with the sample rate 16KHz
14. You will be able to play back the audio.

To align the streams, add into the code a tone in which when a button is pressed, will play a single tone.
By doing this, you will be able to see the point in which the reference signal was giving the start of the tone in relation to when the microphones captured it.

Within the code, change the bytes by the number of samples it's off in the audio_processing_task where the Samples are passed to the AFE.
Customization options
=====================

