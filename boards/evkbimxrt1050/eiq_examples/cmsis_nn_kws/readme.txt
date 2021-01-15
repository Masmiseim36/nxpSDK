Overview
========
CMSIS-NN implementation of keyword spotting application based on
the paper Keyword spotting on microcontrollers [2] and its implementation [1].

Input data preprocessing

One second of raw audio data is processed first: audio frequency spectrograms
are computed and converted into Mel-spectrograms using Mel-frequency cepstral
coefficients method (MFCC). 

In the example, static audio samples ("off", "right") are evaluated first regardless
of microphone is connected or not. Secondly, audio is processed directly from
microphone.

Classification
   
The Mel-spectrogram stands for a 2D input that is classified by a depth-wise 
separable convolution neural network (DS-CNN). The neural network computes 
probability for the output classes - "yes", "no", "up", "down", "left", 
"right", "on", "off", "stop", "go", along with "silence" (i.e. no word spoken) 
and "unknown" word.

Quantization

The NN model is quantized to run faster on MCUs and it takes in a quantized
input and outputs a quantized output. Quantization from floating point NN model
to integers is described in the quantization guide [3]. A pre-trained and
quantized ds_cnn model [5] is converted to CMSIS-NN source code [4] and used
in the example.

HOW TO USE THE APPLICATION:
Say different keywords so that microphone can catch them. Voice recorded by
the microphone can be heard using headphones. 
Note that semihosting implementation causes slower or discontinuous audio experience. 
Select UART in 'Project Options' during project import for using external debug console 
via UART (virtual COM port).

[1] https://github.com/ARM-software/ML-KWS-for-MCU
[2] Hello edge: Keyword spotting on microcontrollers
    Y. Zhang, N. Suda, L. Lai, V. Chandra - arXiv preprint arXiv:1711.07128, 2017 - arxiv.org
[3] https://github.com/ARM-software/ML-KWS-for-MCU/blob/master/Deployment/Quant_guide.md
[4] https://github.com/ARM-software/ML-KWS-for-MCU/tree/master/Deployment/Source/NN/DS_CNN
[5] https://github.com/ARM-software/ML-KWS-for-MCU/blob/master/Pretrained_models/DS_CNN/DS_CNN_S.pb

Files:
  main.cpp - example main function
  off.wav - waveform audio file of the word to recognize
    (source: Speech Commands Dataset available at
    https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz,  
    file speech_commands_test_set_v0.02/off/0ba018fc_nohash_3.wav)
  right.wav - waveform audio file of the word to recognize
    (source: Speech Commands Dataset available at
    https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz,
    file speech_commands_test_set_v0.02/right/0a2b400e_nohash_3.wav)
  audio_data.h - waveform audio files converted into a C language array of
    audio signal values ("off", "right") using Python with the Scipy package:
    from scipy.io import wavfile
    rate, data = wavfile.read('yes.wav')
    with open('wav_data.h', 'w') as fout:
      print('#define WAVE_DATA {', file=fout)
      data.tofile(fout, ',', '%d')
      print('}\n', file=fout)
  timer.c - timer source code
  audio/* - audio capture and pre-processing code
  KWS/* - keyword spotting example application code
  KWS_NN/* - keyword spotting neural network code
  MFCC/* - MFCC feature extraction


Toolchain supported
===================
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal computer

Board settings
==============
Disconnect camera device from the J35 connector if connected (possible signal interference).

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the demo in the terminal window:

Keyword spotting example using CMSIS-NN.
Detection threshold: 25

Static data processing:
----------------------------------------
     Inference time: 11 ms
     Detected:        off (99%)
----------------------------------------

----------------------------------------
     Inference time: 11 ms
     Detected:      right (99%)
----------------------------------------


Microphone data processing:
----------------------------------------
     Inference time: 11 ms
     Detected:    Silence (99%)
----------------------------------------

Data for inference are ready
----------------------------------------
     Inference time: 11 ms
     Detected:         up (99%)
----------------------------------------

Data for inference are ready
----------------------------------------
     Inference time: 11 ms
     Detected:      right (79%)
----------------------------------------
