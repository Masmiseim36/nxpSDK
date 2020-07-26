Overview
========
A benchmark tool to benchmark tensorflow lite models on i.MX RT based on [1]
It takes a TFLite model, generates random inputs and then repeatedly runs
the model for specified number of runs. Aggregate latency statistics are
reported after running the benchmark.

[1] https://github.com/tensorflow/tensorflow/tree/r2.1/tensorflow/lite/tools/benchmark

Files:
   benchamrk_main.cpp - example source code
   benchmark_model.cpp - example source code
   benchmark_tflite_model.cpp - example source code
   model.h - benchmarked model



Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0
- IAR embedded Workbench  8.50.1
- Keil MDK  5.30

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal computer

Board settings
==============
Plug FRMD-STBC-AGM01 kit to arduino connector (J22, J23, J24, J25)

Prepare the demo
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2. Connect a camera to J35 connector. (Skip this step in the case offline version is used.)
3. Connect a LCD display to A1-A40 and B1-B6. (Skip this step in the case offline version is used.)
4. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
5. Download the program to the target board.

Prepare the Demo
================

Running the demo
================

Replace model.h with the model you want to benchmark. The current model is
anomaly detection model. The anomaly detection application can be found in
the SDK MCUXpresso as well.

You can set the application parameters in the benchamrk_main.cpp file in the
int main() function (starting line 55). Following parameters are supported:

   - num_threads: int (default=1)
      The number of threads to use for running TFLite interpreter.
   - warmup_runs: int (default=1)
      The number of warmup runs to do before starting the benchmark.
   - num_runs: int (default=50)
      The number of runs. Increase this to reduce variance.
   - warmup_min_secs: float (default=0.5)
      Minimum number of seconds to rerun for, potentially making the
        actual number of warm-up runs to be greater than warmup_runs
   - min_secs: float (default=0.5)
      Minimum number of seconds to rerun for, potentially making the
      actual number of runs to be greater than num_runs
   - max_secs: float (default=150)
      Maximum number of seconds to rerun for, potentially making the
      actual number of runs to be less than num_runs. Note if --max-secs
      is exceeded in the middle of a run, the benchmark will continue to
      the end of the run but will not start the next run
   - enable_op_profiling: bool (default=false)
      Whether to enable per-operator profiling measurement.

Running the application, you get an log similar to:

STARTING!
Min num runs: [50]
Min runs duration (seconds): [0.5]
Max runs duration (seconds): [150]
Inter-run delay (seconds): [-1]
Num threads: [1]
Benchmark name: []
Output prefix: []
Min warmup runs: [1]
Min warmup runs duration (seconds): [0.5]
Graph: []
Input layers: []
Input shapes: []
Input value ranges: []
Use gpu : [0]
Allow fp16 : [0]
Require full delegation : [0]
Enable op profiling: [0]
Max profiling buffer entries: [1024]
Loaded model 
resolved reporter
Initialized session in 1.608ms
[Init Phase] - Memory usage: max resident set size = 0 MB, total malloc-ed size = 0 MB
Running benchmark for at least 1 iterations and at least 0.5 seconds but terminate if exceeding 150 seconds.
count=2563 first=344 curr=62 min=57 max=344 avg=65.7382 std=6

Running benchmark for at least 50 iterations and at least 0.5 seconds but terminate if exceeding 150 seconds.
count=2565 first=104 curr=68 min=56 max=104 avg=65.6351 std=2

[Overall] - Memory usage: max resident set size = 0 MB, total malloc-ed size = 0 MB
Average inference timings in us: Warmup: 65.7382, Init: 1608, no stats: 65.6351


----------------------------------------------------------------------------------------------------------------------
If enable_op_profiling is set True, the log is similar to:


STARTING!
Min num runs: [50]
Min runs duration (seconds): [0.5]
Max runs duration (seconds): [150]
Inter-run delay (seconds): [-1]
Num threads: [1]
Benchmark name: []
Output prefix: []
Min warmup runs: [5]
Min warmup runs duration (seconds): [0.5]
Graph: []
Input layers: []
Input shapes: []
Input value ranges: []
Use gpu : [0]
Allow fp16 : [0]
Require full delegation : [0]
Enable op profiling: [1]
Max profiling buffer entries: [1024]
Loaded model 
resolved reporter
Initialized session in 2.491ms
[Init Phase] - Memory usage: max resident set size = 0 MB, total malloc-ed size = 0 MB
Running benchmark for at least 5 iterations and at least 0.5 seconds but terminate if exceeding 150 seconds.
count=1 curr=352

Running benchmark for at least 50 iterations and at least 0.5 seconds but terminate if exceeding 150 seconds.
count=1 curr=178

[Overall] - Memory usage: max resident set size = 0 MB, total malloc-ed size = 0 MB
Average inference timings in us: Warmup: 352, Init: 2491, no stats: 178
                [node type]             [start]     [first]    [avg ms]        [%]     [cdf%]     [mem KB]   [times called]   [Name]
           Misc Runtime Ops               0.000       0.104       0.104    55.467%    55.467%        0.000           0   AllocateTensors/0
            FULLY_CONNECTED               0.000       0.088       0.088    23.467%    78.933%        0.000           0   [dense_3/BiasAdd]:0
                   LOGISTIC               0.090       0.015       0.015     4.000%    82.933%        0.000           0   [dense_3/Sigmoid]:1
            FULLY_CONNECTED               0.106       0.048       0.048    12.800%    95.733%        0.000           0   [dense_4/BiasAdd]:2
                       TANH               0.155       0.016       0.016     4.267%   100.000%        0.000           0   [dense_4/Tanh]:3

                [node type]             [start]     [first]    [avg ms]        [%]     [cdf%]     [mem KB]   [times called]   [Name]
           Misc Runtime Ops               0.000       0.104       0.104    55.467%    55.467%        0.000           0   AllocateTensors/0
            FULLY_CONNECTED               0.000       0.088       0.088    23.467%    78.933%        0.000           0   [dense_3/BiasAdd]:0
            FULLY_CONNECTED               0.106       0.048       0.048    12.800%    91.733%        0.000           0   [dense_4/BiasAdd]:2
                       TANH               0.155       0.016       0.016     4.267%    96.000%        0.000           0   [dense_4/Tanh]:3
                   LOGISTIC               0.090       0.015       0.015     4.000%   100.000%        0.000           0   [dense_3/Sigmoid]:1

Number of nodes executed: 5
                [Node type]     [count]     [avg ms]       [avg %]       [cdf %]     [mem KB]   [times called]
           Misc Runtime Ops           1        0.069       55.645%       55.645%        0.000           0
            FULLY_CONNECTED           2        0.045       36.290%       91.935%        0.000           0
                       TANH           1        0.005        4.032%       95.968%        0.000           0
                   LOGISTIC           1        0.005        4.032%      100.000%        0.000           0

Timings (microseconds): count=3 first=104 curr=167 min=104 max=167 avg=125 std=29
Memory (bytes): count=0
5 nodes observed
Customization options
=====================

