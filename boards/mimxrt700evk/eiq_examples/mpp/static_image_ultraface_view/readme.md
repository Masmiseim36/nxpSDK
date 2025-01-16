# static_image_ultraface_view

## Overview

This example shows how to use the library to create a use-case for
face detection using static image as source.

The machine learning framework used for this example is TensorFlow Lite Micro.
The face detection model used is quantized Ultraface-ultraslim optimized for RT700 NPU that detects multiple faces 
in an input image.

## Toolchains supported
- MCUXpresso, version 11.10.0
- GCC Arm Embedded, version 13.2.Rel1

## Hardware requirements
Refer to board.readme for hardware setup requirements.
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/eiq_examples/mpp/board_readme.md)

## Use-cases Description

HOW TO USE THE APPLICATION:

### High-level description
```
                                                                   +--------------------------------------------------------+
                                                                   |                                                        |
                                                                   |                                                        |
                                                                  \ /                                                       |
                  +-------------+      +-------------+      +-------------+      +-------------+      +-------------+       |
                  |             |      |             |      |             |      |             |      |             |       |
Pipeline 0        |    static   | -->  |  2D convert | -->  |   labeled   | -->  |  2D convert | -->  |    Display  |       |
                  |    image    |  |   |(color+flip) |      |  rectangle  |      | (rotation)  |      |             |       |
                  +-------------+  |   +-------------+      +-------------+      +-------------+      +-------------+       |
                                   |                                                                                        |
                                   |     +-------------+      +--------------+      +-------------+                         |
                                   |     |             |      |              |      |             |                         |
Pipeline 1                         +---> |  2D convert | -->  | ML Inference | -->  |  NULL sink  |                         |
                                         |             |      |              |      |             |                         |
                                         +-------------+      +--------------+      +-------------+                         |
                                                                       |                                                    |
                                                                       |                                                    |
        +-----------------+                                            |                                                    |
	|  Main app:      |                                            |                                                    |
	| ML output       |   <----- ML Inference output callback -----+                                                    |
        | post processing |                                                                                                 |
	|                 |   ------   labeled rectangle update   ----------------------------------------------------------+
	+-----------------+
```
### Detailed description

Application creates two pipelines:

- One pipeline that runs the static image preview.
- Another pipeline that runs the ML inference on the static image source.
- Pipeline 1 is split from pipeline 0
- Pipeline 0 executes the processing of each element sequentially and CANNOT be preempted by another pipeline.
- Pipeline 1 executes the processing of each element sequentially but CAN be preempted.

### Pipelines elements description

* Static image 128x128 RGB.
* Display element is configured for a specific pixel format and resolution (board dependent)
* 2D convert element on pipeline 0 is configured to perform:
  - color space conversion from RGB888 pixel format to the display pixel format
  - rotation depending on the display orientation compared to landscape mode (NB: Rotation, if needed, should be performed 
  after the labeled-rectangle to get labels in the right orientation).

* 2D convert element on pipeline 1 is configured to perform:
  - cropping to maintain image aspect ratio

* The labeled rectangle element draws a crop window from which the static image is sent to
  the ML inference element. The labeled rectangle element also displays the label "face" of the detected face.
* The ML inference element runs an inference on the image pre-processed by the 2D convert element.
* The NULL sink element closes pipeline 1 (in MPP concept, only sink elements can close a pipeline).

* At every inference, the ML inference element invokes a callback containing the inference outputs.
These outputs are post-processed by the callback client component (in this case, the main task of the application)

## Running the demo

EXPECTED OUTPUTS:
The expected outputs of the example are:
- For each detected face, a labeled rectangle should be displayed on the screen.
- Logs below should be displayed on the debug console.

Logs for static_image_ultraface_view example using TensorFlow Lite Micro model should look like this:
```
[MPP_VERSION_2.0.0]
Inference Engine: TensorFlow-Lite Micro 
inference time 326 ms 
ultraface : box 0 label face score 99(%)
ultraface : box 1 label face score 99(%)
inference time 328 ms 
ultraface : box 0 label face score 99(%)
ultraface : box 1 label face score 99(%)
inference time 327 ms 
ultraface : box 0 label face score 99(%)
ultraface : box 1 label face score 99(%)
inference time 327 ms 
ultraface : box 0 label face score 99(%)
ultraface : box 1 label face score 99(%)
inference time 327 ms 
ultraface : box 0 label face score 99(%)
ultraface : box 1 label face score 99(%)
inference time 327 ms 
```
## Important notes

TensorFLow Lite Micro is an optional engine for the ML Inference component of MPP.
This project embeds NXP's custom TensorFlow Lite Micro code by default.
TensorFLow Lite allows short-listing the "Operations" used by a specific model in order to reduce the binary image footprint.
This is done by implementing the function:

tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)

This example implements its own function MODEL_GetOpsResolver dedicated to Ultraface.
User may provide its own implementation of MODEL_GetOpsResolver when using a different model.
