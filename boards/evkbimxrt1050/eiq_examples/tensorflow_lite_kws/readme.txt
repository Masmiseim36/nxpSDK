Overview
========

The key word spotting example is based on Keyword spotting for Microcontrollers [1].

Input data preprocessing

Raw audio data is pre-processed first - a spectrogram is calculated: A 40 ms
window slides over a one-second audio sample with a 20 ms stride. For each
window, audio frequency strengths are computed using FFT and turned it into
a set of Mel-Frequency Cepstral Coefficients (MFCC). Only 10 first coefficients
are taken into account. The window slides over a sample 49 times, hence
a matrix with 49 rows and 10 columns is created. The matrix is called a spectrogram.

In the example, static audio samples (off, right) are evaluated first regardless
microphone is connected or not. Secondly, audio is processed directly from
microphone.

Classification

The spectrogram is fed into neural network. The neural network is a deepwise
separable convolutional neural network called MobileNet and described in [3].
It outputs a probability vector with probabilities of classes: 'Silence', 'Unknown',
'yes', 'no', 'up', 'down', 'left', 'right', 'on', 'off', 'stop' and 'go'.

Quantization

The NN model is quantized to run faster on MCUs and it takes in a quantized
input and outputs a quantized output. An input spectrogram needs to be scaled
from range [-247, 30] to range [0, 255] and round to integers. Values lower
than zero are set to zero and values exceeding 255 are set to 255. An output
of the softmax function is a vector with components in the interval (0, 255)
and the components will add up to 255).   

Model training and conversion

The ds_cnn model is downloaded from Github [2]. This model is trained without quantization
awareness and post-training converted to a TF Lite model with following commands:

toco --graph_def_file=/tmp/DS_CNN_S.pb --output_file=/tmp/ds_cnn_s.tflite \
     --input_shapes=1,49,10,1 --input_arrays=Reshape_1 --output_arrays=labels_softmax \
     --inference_type=QUANTIZED_UINT8 --mean_values=227 --std_dev_values=1 \
     --change_concat_input_ranges=false \
     --default_ranges_min=-6 --default_ranges_max=6
 
xxd -i /tmp/ds_cnn_s.tflite > /tmp/ds_cnn_s_model.h

HOW TO USE APPLICATION:
Play different keyword so that microphone can catch them. Voice recorded by microphone 
can be heared using headphones. 
Note Semihosting implementation causes slower or discontinuous audio experience. 
Select UART in 'Project Options' for using external debug console 
via UART (Virtual COM port).

[1] https://github.com/ARM-software/ML-KWS-for-MCU
[2] https://github.com/ARM-software/ML-KWS-for-MCU/blob/master/Pretrained_models/DS_CNN/DS_CNN_S.pb
[3] https://arxiv.org/abs/1704.04861

Files:
  kws.cpp - example source code
  off.wav - waveform audio file of the word to recognize
    (source: Speech Commands Dataset available at
    https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz,  
    file speech_commands_test_set_v0.02/off/0ba018fc_nohash_2.wav)
  right.wav - waveform audio file of the word to recognize
    (source: Speech Commands Dataset available at
    https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz,
    file speech_commands_test_set_v0.02/right/0a2b400e_nohash_1.wav)
  ds_cnn_s.h - model data converted from a .tflite file
  commands.h - waveform audio files converted into a C language array of audio signal values (OFF, RIGHT)  
      audio signal values using Python with the Scipy package:
    from scipy.io import wavfile
    rate, data = wavfile.read('yes.wav')
    with open('wav_data.h', 'w') as fout:
      print('#define WAVE_DATA {', file=fout)
      data.tofile(fout, ',', '%d')
      print('}\n', file=fout)
  get_top_n.h - get top result prototypes
  get_top_n_impl - get top result source code
  timer.c - timer counter source code
  timer.h - timer counter function prototypes
  mfcc.cpp - MFCC feature extraction to match with TensorFlow MFCC Op source code
  mfcc.h - MFCC feature extraction to match with TensorFlow MFCC Op prototypes
  kws_mfcc.cpp - Keyword spotting example code using MFCC feature extraction and neural network source code
  kws_mfcc.h - Keyword spotting example code using MFCC feature extraction and neural network prototypes  


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- MCUXpresso  11.1.0
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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

Keyword spotting example using a TensorFlow Lite model. 

Detection threshold: 30%

Static data processing:

----------------------------------------
     Inference time: 56 ms
     Detected: off (100%)
----------------------------------------

----------------------------------------
     Inference time: 56 ms
     Detected: right (98%)
----------------------------------------


Microphone data processing:

----------------------------------------
     Inference time: 56 ms
     Detected: right (43%)
----------------------------------------

----------------------------------------
     Inference time: 56 ms
     Detected: left (63%)
----------------------------------------

----------------------------------------
     Inference time: 56 ms
     Detected: up (70%)
----------------------------------------

----------------------------------------
     Inference time: 56 ms
     Detected: Silence (38%)
----------------------------------------

----------------------------------------
     Inference time: 56 ms
     Detected: up (55%)
----------------------------------------

----------------------------------------
     Inference time: 56 ms
     Detected: down (57%)
----------------------------------------
Customization options
=====================

