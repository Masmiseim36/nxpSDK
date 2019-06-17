Overview
========
Example showing how to use the eIQ TensorFlow Lite library to run a neural
network model to recognize keywords in speech. The source code is based on
the Label Image [1] and Micro Speech [2] examples from the TensorFlow Lite
examples. The model does not take in raw audio sample data. Instead it works
with spectrograms, which are two dimensional arrays that are made up of slices
of frequency information, each taken from a different time window. The example
uses spectrograms that have been pre-calculated from one-second WAV files in
the test data set. In a complete application these spectrograms would be
calculated at runtime from microphone inputs, but the code for doing that is
not yet included in this sample code. The example recognizes four classes -
"Silence", "Unknown", "yes", "no".

[1] https://github.com/tensorflow/tensorflow/tree/r1.11/tensorflow/contrib/lite/examples/label_image
[2] https://github.com/tensorflow/tensorflow/tree/r1.13/tensorflow/lite/experimental/micro/examples/micro_speech

Files:
  kws.cpp - example source code
  tiny_conv_model_data.cpp - model data converted from a .tflite file
    (source: tiny_conv_model_data.cc available at [2])
  yes.wav - waveform audio file of the word to recognize
    (source: Speech Commands Dataset available at
    https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz,
    file speech_commands_test_set_v0.02/yes/f2e59fea_nohash_1.wav)
  yes_features_data.cpp - spectrogram data of 1 second audio pre-calculated using
    wav_to_features.py from https://github.com/tensorflow/tensorflow/tree/r1.13/tensorflow/examples/speech_commands:
    wav_to_features.py \
      --sample_rate=16000 \
      --clip_duration_ms=1000 \
      --window_size_ms=30 \
      --window_stride_ms=20 \
      --feature_bin_count=40 \
      --quantize=True \
      --preprocess="average" \
      --input_wav="yes.wav" \
      --output_c_file="yes_features_data.cpp"


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

Keyword spotting example using a TensorFlow Lite model
Elapsed time: 8 ms
Detected: yes (100% confidence)

Customization options
=====================

