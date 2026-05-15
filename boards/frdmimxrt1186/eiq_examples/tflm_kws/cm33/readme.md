# tflm_kws

## Overview

This is a keyword spotting example based on the *Keyword spotting for Microcontrollers* ([ARM-software/ML-KWS-for-MCU](https://github.com/ARM-software/ML-KWS-for-MCU)) project. It demonstrates how to run a TensorFlow Lite Micro model on NXP MCUs for audio keyword detection.

In this example, static audio samples ("off", "right") are evaluated for classification into one of 12 keyword classes.

### Input Data Preprocessing

Raw audio data is pre-processed by calculating a spectrogram:

- A 40 ms window slides over a one-second audio sample with a 20 ms stride
- For each window, audio frequency strengths are computed using FFT
- The results are transformed into Mel-Frequency Cepstral Coefficients (MFCC)
- Only the first 10 coefficients are taken into account
- The window slides over a sample 49 times, creating a matrix with 49 rows and 10 columns

### Classification

The spectrogram is fed into a neural network for classification:

- The neural network is a depthwise separable convolutional neural network based on *MobileNet* ([MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications](https://arxiv.org/abs/1704.04861))
- The model produces a probability vector for the following 12 classes:
  - "Silence"
  - "Unknown"
  - "yes", "no", "up", "down", "left", "right"
  - "on", "off"
  - "stop", "go"

### Quantization

The neural network model is quantized to optimize performance on MCUs:

- The model takes quantized input and produces quantized output
- Input spectrogram scaling: from range `[-247, 30]` to range `[0, 255]`, rounded to integers
  - Values lower than zero are set to zero
  - Values exceeding 255 are set to 255
- Output: a vector with components in the interval `(0, 255)` that add up to 255



### Model Source

The pre-trained model is sourced from:
- https://github.com/ARM-software/ML-KWS-for-MCU/blob/master/Pretrained_models/DS_CNN/DS_CNN_S.pb

For details on model quantization and conversion, refer to the *eIQ TensorFlow Lite User's Guide* included in the MCUXpresso SDK package.

### Audio Data Source

The static audio samples are from the Speech Commands Dataset:
- https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz
- `speech_commands_test_set_v0.02/off/0ba018fc_nohash_2.wav`
- `speech_commands_test_set_v0.02/right/0a2b400e_nohash_1.wav`

### Audio Data Conversion

Waveform files can be converted to C arrays using Python with Scipy:

```python
from scipy.io import wavfile

rate, data = wavfile.read('yes.wav')
with open('wav_data.h', 'w') as fout:
    print('#define WAVE_DATA {', file=fout)
    data.tofile(fout, ',', '%d')
    print('}\n', file=fout)
```


## Project Files

| File/Directory | Description |
|----------------|-------------|
| `main.cpp` | Example main function |
| `off.wav` | Waveform audio file for the word "off" |
| `right.wav` | Waveform audio file for the word "right" |
| `audio_data.h` | Waveform audio files converted to C language array |
| `train.py` | Model training script based on TensorFlow audio tutorial |
| `timer.c` | Timer source code |
| `audio/*` | Audio capture and pre-processing code |
| `audio/mfcc.cpp` | MFCC feature extraction matching TensorFlow MFCC operation |
| `audio/kws_mfcc.cpp` | Audio buffer handling for MFCC feature extraction |
| `get_top_n.cpp` | Top results retrieval |
| `model_data.h` | Model data converted to C language array |
| `ds_cnn_s_npu.tflite` | NPU TensorFlow Lite model |
| `model.cpp` | Model initialization and inference code |
| `model_ds_cnn_ops.cpp` | Model operations registration |
| `output_postproc.cpp` | Model output processing |





### Project Structure in MCU SDK

Only primary files are listed here: 

```bash
Path_to_MCUSDK/mcuxsdk/examples/
├── eiq_examples/
│   ├── common/                
│   │   └── audio/*            
│   └── tflm_kws/          
│       ├── CMakeLists.txt
│       ├── audio_data.h
│       ├── demo_config.h
│       ├── dsp
│       │   └── main.c
│       ├── labels.h
│       ├── main.cpp
│       ├── off.wav
│       ├── output_postproc.cpp
│       ├── readme.md
│       └── train.py
└── _boards/
    └── <board_name>/
        └── eiq_examples/
            └── tflm_kws/
                ├── example_board_readme.md
                ├── npu               # NPU version of the model files
                │   ├── ds_cnn_s_npu.tflite
                │   ├── model_data.h
                │   └── model_ds_cnn_ops_npu.cpp
                └── reconfig.cmake

```

(For the boards without NPU hardware, the CPU version of the model files is located in the `cpu/` folder.)

### Project Structure in IDE

**(1) Project Structure in *MCUXpresso IDE***

Only primary files are listed here: 

```bash
.
├── eiq/                     
│   ├── neutron/             # Neutron NPU support
│   └── tensorflow-lite/     
├── source/                  # Example source files
│   ├── audio.h
│   ├── audio_data.h
│   ├── audio_load.cpp
│   ├── get_top_n.cpp
│   ├── get_top_n.h
│   ├── kws_mfcc.cpp
│   ├── kws_mfcc.hpp
│   ├── labels.h
│   ├── main.cpp
│   ├── mfcc.cpp
│   ├── model.cpp
│   ├── model.h
│   ├── model_data.h
│   ├── model_ds_cnn_ops_npu.cpp
│   ├── off.wav
│   ├── output_postproc.cpp
│   ├── output_postproc.h
│   ├── timer.c
│   ├── timer.h
│   └── train.py
└── doc/                     # Documentation
    └── readme.md
```

The library files and header files in the `eiq/neutron/` folder are Neutron-Software related files.

Users can update the Neutron-Software version in this project by replacing these four files in the `eiq/neutron/` folder: `NeutronDriver.h`, `NeutronErrors.h`, `libNeutronDriver.a`, and `libNeutronFirmware.a`.


**(2) Project Structure in *MCUXpresso for VScode***

Please select the following two options in the ***MCUXpresso for VS Code*** extension settings:

- [✓] Mcuxpresso > Experimental: Enable Add Files To Project
- [✓] Mcuxpresso > Experimental: Enable Freestanding Copy Board

After importing a **Freestanding** project from the MCU SDK, the project structure is as follows (only primary files are listed here):

```bash
.
├── CMakeLists.txt
├── audio/*
├── audio_data.h
├── common
│   ├── timer.c
│   ├── timer.h
│   └── timer_xtensa.h
├── cm33                   # Neutron-Software related
│   ├── libNeutronDriver.a
│   └── libNeutronFirmware.a
├── driver_include         # Neutron-Software related
│   └── NeutronDriver.h
├── include                # Neutron-Software related
│   └── NeutronErrors.h
├── labels.h
├── main.cpp
├── model
│   ├── get_top_n.cpp
│   ├── get_top_n.h
│   ├── model.h
│   └── output_postproc.h
├── npu                     # NPU version of the model files
│   ├── ds_cnn_s_npu.tflite
│   ├── model_data.h
│   └── model_ds_cnn_ops_npu.cpp
├── off.wav
├── output_postproc.cpp
└── train.py
```

The library files and header files in the `cm33/`, `driver_include/`, and `include/` folders are Neutron-Software related files.

Users can update the Neutron-Software version in this project by replacing these four files: `NeutronDriver.h`, `NeutronErrors.h`, `libNeutronDriver.a`, and `libNeutronFirmware.a`.

(For the boards without NPU hardware, the CPU version of the model file is located in the `cpu/` folder.)


## Replace the model file

If you need to replace the model file with your own model, please follow two main steps:
- Step 1: Modify the `model_data.h` file.
- Step 2: Modify the `model_ds_cnn_ops_npu.cpp` file.

### Step 1: Modify the `model_data.h` file.

**(1) Use *NPU* model**

For NPU boards, you can use the **Neutron Converter** tool to convert your `.tflite` model into an NPU-optimized version, which simultaneously generates an NPU-specific `.tflite` file and a corresponding `.h` header file.
* You can obtain the Neutron Converter tool from the [eIQ Neutron SDK](https://www.nxp.com/design/design-center/software/eiq-ai-development-environment/eiq-toolkit-for-end-to-end-model-development-and-deployment:EIQ-TOOLKIT). 
* After downloading and extracting the Neutron SDK Zip package, you can find the Neutron Converter tool at the following path: `/eiq-neutron-sdk-linux-x.x.x/bin/neutron-converter`.

```bash
# Set environment variables
export NEUTRON_SDK_PATH="/path/to/eiq-neutron-sdk-linux-x.x.x"
export LD_LIBRARY_PATH="${NEUTRON_SDK_PATH}/lib:${LD_LIBRARY_PATH}"
export PATH="${NEUTRON_SDK_PATH}/bin:${PATH}"

# Convert for mcxn94x target
neutron-converter --input model_name.tflite --output model_name_mcxn94x.tflite --target mcxn94x --dump-header-file-output

# Convert for imxrt700 target
neutron-converter --input model_name.tflite --output model_name_rt700.tflite --target imxrt700 --dump-header-file-output
```

Neutron Converter Target Platforms:

| Target | Description | Boards |
|--------|-------------|--------|
| `mcxn94x` | MCXN94x series | frdmmcxn947, mcxn5xxevk, mcxn9xxevk |
| `imxrt700` | i.MX RT700 series | mimxrt700evk |


Modify the `model_data.h` file in the project:
- Replace the original model data with `model_data` array and `model_data_len` from your model's `.h` file.
- Refer to the "Total data" value for graph "main" shown in the terminal output of neutron converter, and adjust the `kTensorArenaSize` value in the `model_data.h` file accordingly. Typically, `kTensorArenaSize` should be set to 105% or more of the "Total data" value.

```bash
# Example terminal output from neutron converter:
# The "Total data" value for graph "main" is 16,512 (bytes), so the kTensorArenaSize should be set to about 17,338 (bytes) (16,512 * 1.05)

Overall statistics for graph "main":
  Operators:
    ...<operator details>...
  Memory:
    Total data    = 16,512 (bytes) (Inputs + Outputs + Intermediate Variable Tensors)
    Total weights = 30,400 (bytes) (Weights)
    Total size    = 46,912 (bytes) (All)
```

```cpp
// Modify the model_data.h file:

constexpr int kTensorArenaSize = <Total data * 1.05>;  // TensorArenaSize in (bytes)

static const uint8_t model_data[] __ALIGNED(16) __PLACEMENT = {
...<your model array>...
}

static const unsigned int model_data_len = <your model length>;
```

If the following error occurs while running the example, it is caused by the `kTensorArenaSize` being too small. Please increase the `kTensorArenaSize` accordingly.

```bash
Failed to resize buffer. Requested:16544, available 15928, missing:616
AllocateTensors() failed
```


**(2) Use *CPU* model**

You can use the following workflow to run a CPU-version model in this example.

The `xxd` tool can convert your `.tflite` model file into a `.h` header file. The command is as follows:

```bash
xxd -i model_name.tflite > model_name.h
```

Or you can also use Neutron Converter (Recommend) to generate the `.h` file by using the `--dump-header-file-input` option. This will convert your input CPU-version `.tflite` model into a CPU-version `.h` file.

```bash
neutron-converter --input model_name.tflite  --output model_name_imxrt700.tflite --target imxrt700  --dump-header-file-input
```

Modify the model_data.h file in the project: replace the original model data with `model_data` array and `model_data_len` from your model’s `.h` file.

```cpp
# Modify the model_data.h file:

static const uint8_t model_data[] __ALIGNED(16) __PLACEMENT = {
...<your model array>...
}

static const unsigned int model_data_len = <your model length>;
```

### Step 2: Modify the `model_ds_cnn_ops_npu.cpp` file.

In step 2, you need to refer to the `.h` file generated by the **Neutron Converter** in step 1 and modify the model operators in the `model_ds_cnn_ops_npu.cpp` file.

Typically, the `.h` file output by the Neutron Converter contains content similar to the following:

```cpp
/*
// Register operators for TFLite Micro.
static tflite::MicroMutableOpResolver<4> s_microOpResolver;
s_microOpResolver.AddQuantize();
s_microOpResolver.AddSoftmax();
s_microOpResolver.AddDequantize();
s_microOpResolver.AddCustom(tflite::GetString_NEUTRON_GRAPH(), tflite::Register_NEUTRON_GRAPH());
*/
```

Copy the operator registration code from the `.h` file generated by the Neutron Converter into the `model_ds_cnn_ops_npu.cpp` file.


## Running the Demo

Run result on MIMXRT700-EVK board with ARM GCC toolchain (NPU version):

```
Keyword spotting example using a TensorFlow Lite Micro model.
Detection threshold: 95%
Model: ds_cnn_s_npu
Core/NPU Frequency: 324 MHz
TensorArena Addr: 0x20000000 - 0x20020000
TensorArena Size: Total 0x20000 (131072 B); Used 0x4390 (17296 B)
Model Addr: 0x20400000 - 0x20407d50
Model Size: 0x7d50 (32080 B)
Total Size Used: 49376 B (Model (32080 B) + TensorArena (17296 B))

Static data processing:
----------------------------------------
     Inference time: 595 us
     Detected: off (99%)
----------------------------------------

----------------------------------------
     Inference time: 576 us
     Detected: right (99%)
----------------------------------------
```


## Supported Boards with NPU
- [FRDM-MCXN947](../../_boards/frdmmcxn947/eiq_examples/tflm_cifar10/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/eiq_examples/tflm_cifar10/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/eiq_examples/tflm_cifar10/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/eiq_examples/tflm_cifar10/example_board_readme.md)

## Supported Boards with CPU Only
- MIMXRT1040-EVK
- EVKB-IMXRT1050
- MIMXRT1060-EVKB
- MIMXRT1060-EVKC
- EVK-MIMXRT1064
- MIMXRT1160-EVK
- MIMXRT1170-EVKB
- MIMXRT1180-EVK
- FRDM-IMXRT1186
- EVK-MIMXRT595
- EVK-MIMXRT685
- MIMXRT685-AUD-EVK