Overview
========
Keyword spotting example based on source code [1] from a study [2] using
Mel-frequency cepstral coefficients (MFCC) to convert speech signal into
a set of frequency-domain spectral coefficients. The extracted speech feature
matrix is fed into a classifier module, which generates the probabilities
for the output classes - "yes", "no", "up", "down", "left", "right", "on",
"off", "stop", "go", along with "silence" (i.e. no word spoken) and "unknown"
word. In this example, depthwise separable convolutional neural network
(DS-CNN) is used for classification. The expected input is a 1 second long
audio sample of 16-bit values with 16 kHz rate.

[1] https://github.com/ARM-software/ML-KWS-for-MCU
[2] Hello edge: Keyword spotting on microcontrollers
    Y. Zhang, N. Suda, L. Lai, V. Chandra - arXiv preprint arXiv:1711.07128, 2017 - arxiv.org

Files:
  main.cpp - example source code
  yes.wav - waveform audio file of the word to recognize
    (source: Speech Commands Dataset available at
    https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz,
    file speech_commands_test_set_v0.02/yes/f2e59fea_nohash_1.wav)
  wav_data.h - waveform audio file converted into a C language array of
    audio signal values using Python with the Scipy package:
    from scipy.io import wavfile
    rate, data = wavfile.read('yes.wav')
    with open('wav_data.h', 'w') as fout:
      print('#define WAVE_DATA {', file=fout)
      data.tofile(fout, ',', '%d')
      print('}\n', file=fout)


Toolchain supported
===================
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.0
- GCC ARM Embedded  8.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 or EVK-MIMXRT1060 board
- Personal computer

Board settings
==============
No special settings are required.

Prepare the demo
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.

Prepare the Demo
================

Running the demo
================
The log below shows the output of the demo in the terminal window:

Keyword spotting example using convolutional neural network
Elapsed time: 21 ms
Detected: yes (99% confidence)

Customization options
=====================

