# tflm_cifar10

## Overview

This example demonstrates a **Convolutional Neural Network (CNN)** for image classification on MCUs using TensorFlow Lite Micro. It showcases the implementation of convolution, ReLU activation, pooling, and fully-connected layers. 

In this example, a static test images ("ship") is evaluated for classification into one of 10 CIFAR-10 classes.

### Features

- **Model Architecture**: CifarNet CNN with 3 convolution layers
- **Input**: 32×32 pixel color image
- **Output**: Classification into 10 classes
- **Model Size**: 91 KB
- **Detection Threshold**: 60%

### Network Structure

The neural network consists of:
1. 3 convolutional layers
2. ReLU activation functions (after each convolution)
3. Max pooling layers (interspersed)
4. Fully-connected output layer

### Model Origin

- **Training**: Based on scripts from [TensorFlow Models](https://github.com/tensorflow/models/tree/master/research/slim)
- **Configuration**: Modified to match CMSIS-NN CIFAR-10 example structure
- **Source Code**: Adapted from [TensorFlow Lite Label Image Example](https://github.com/tensorflow/tensorflow/tree/r2.3/tensorflow/lite/examples/label_image)

---

## Project Files

| File | Description |
|------|-------------|
| `main.cpp` | Example main function |
| `ship.bmp` | Static test image (source: [Wikipedia](https://en.wikipedia.org/wiki/File:Christian_Radich_aft_foto_Ulrich_Grun.jpg)) |
| `image_data.h` | Image converted to C array (RGB values) |
| `timer.c` | Timer source code |
| `image/*` | Image capture and pre-processing code |
| `get_top_n.cpp` | Top-N results retrieval |
| `model_data.h` | Model data converted from `.tflite` to C array |
| `model.cpp` | Model initialization and inference |
| `model_cifarnet_ops.cpp` | Model operations registration |
| `output_postproc.cpp` | Output post-processing |
| `video/*` | Camera and display handling |


### Project Structure in MCU SDK

Only primary files are listed here: 

```bash
Path_to_MCUSDK/mcuxsdk/examples/
├── eiq_examples/
│   ├── common/                # Common utilities for eIQ examples
│   │   ├── image/*            # Image capture and pre-processing module
│   │   └── video/*            # Camera and display handling module
│   └── tflm_cifar10/          
|       ├── CMakeLists.txt
|       ├── image_data.h
|       ├── labels.h
|       ├── main.cpp
|       ├── readme.md
|       └── ship.bmp
└── _boards/
    └── <board_name>/
        └── eiq_examples/
            └── tflm_cifar10/
                ├── example_board_readme.md
                └── pcq_npu    # NPU version of the model files
                    ├── cifarnet_quant_int8_npu.tflite
                    ├── model_cifarnet_ops_npu.cpp
                    └── model_data.h
```

(For the boards without NPU hardware, the CPU version of the model files is located in the `pcq/` folder.)

### Project Structure in IDE

**(1) Project Structure in *MCUXpresso IDE***

Only primary files are listed here: 

```bash
.
├── eiq/                     # eIQ library files
│   ├── neutron/             # Neutron NPU support
│   └── tensorflow-lite/     
├── source/                  # Example source files
│   ├── main.cpp             # Main application entry
│   ├── get_top_n.cpp/h      # Top-N results retrieval
│   ├── model.cpp/h          
│   ├── model_data.h         # Model data (from .tflite)
│   ├── model_cifarnet_ops_npu.cpp  # operations registration
│   ├── output_postproc.cpp/h # Output processing
│   ├── image_data.h         # Static image array
│   ├── ship.bmp             # Static test image
│   ├── labels.h             
│   ├── timer.c/h            # Timer utilities
│   └── image/*              # Image processing module
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
├── CMakeLists.txt           # CMake build configuration
├── main.cpp                 # Main application entry
├── labels.h                 
├── image_data.h             # Static image array
├── ship.bmp                 # Static test image
├── cm33                     # Neutron-Software related
│   ├── libNeutronDriver.a
│   └── libNeutronFirmware.a
├── driver_include           # Neutron-Software related
│   └── NeutronDriver.h
├── include                  # Neutron-Software related
│   └── NeutronErrors.h
├── common/                 
│   └── timer.c/h
├── image/*                  # Image processing module
├── model/                   
│   ├── get_top_n.cpp/h      # Top-N results retrieval
│   ├── output_postproc.cpp/h 
│   └── model.h              
├── tflm/                    
│   └── model.cpp            
└── pcq_npu/                 # NPU version of the model files
    ├── cifarnet_quant_int8_npu.tflite  
    ├── model_cifarnet_ops_npu.cpp      
    └── model_data.h         
```

The library files and header files in the `cm33/`, `driver_include/`, and `include/` folders are Neutron-Software related files.

Users can update the Neutron-Software version in this project by replacing these four files: `NeutronDriver.h`, `NeutronErrors.h`, `libNeutronDriver.a`, and `libNeutronFirmware.a`.

(For the boards without NPU hardware, the CPU version of the model file is located in the `pcq/` folder.)


## Replace the model file

If you need to replace the model file with your own model, please follow two main steps:
- Step 1: Modify the `model_data.h` file.
- Step 2: Modify the `model_cifarnet_ops_npu.cpp` file.

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

### Step 2: Modify the `model_cifarnet_ops_npu.cpp` file.

In step 2, you need to refer to the `.h` file generated by the **Neutron Converter** in step 1 and modify the model operators in the `model_cifarnet_ops_npu.cpp` file.

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

Copy the operator registration code from the `.h` file generated by the Neutron Converter into the `model_cifarnet_ops_npu.cpp` file.

### Image Conversion Script

The `image_data.h` file is generated using Python with OpenCV and NumPy:

```python
import cv2
import numpy as np

img = cv2.imread('ship.bmp')
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

with open('image_data.h', 'w') as fout:
    print('#define STATIC_IMAGE_NAME "ship"', file=fout)
    print('static const uint8_t image_data[] = {', file=fout)
    img.tofile(fout, ', ', '0x%02X')
    print('};\n', file=fout)
```


---

## Running the Demo

Run result on MIMXRT700-EVK board with ARM GCC toolchain (NPU version):

```
CIFAR-10 example using a TensorFlow Lite Micro model.
Detection threshold: 60%
Model: cifarnet_quant_int8_npu
Core/NPU Frequency: 324 MHz
TensorArena Addr: 0x20000000 - 0x20040000
TensorArena Size: Total 0x40000 (262144 B); Used 0xaee0 (44768 B)
Model Addr: 0x20400000 - 0x204183e0
Model Size: 0x183e0 (99296 B)
Total Size Used: 144064 B (Model (99296 B) + TensorArena (44768 B))

Static data processing:
----------------------------------------
     Inference time: 1027 us
     Detected: ship (99%)
----------------------------------------
```

---

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
