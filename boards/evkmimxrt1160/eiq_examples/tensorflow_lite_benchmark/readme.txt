Overview
========
A benchmark tool for benchmarking TensorFlow Lite models based on [1]
It takes a TF Lite model, generates random inputs and then repeatedly runs
the model for specified number of runs. Aggregate latency statistics are
reported after running the benchmark.

The model to benchmark can be replaced in the model.h file. The default model
is an anomaly detection model. The anomaly detection application can be found
in the MCUXpresso SDK as well.

The application parameters can be set in the benchmark_main.cpp file in the
main function. The following parameters are supported:

   - num_threads: int (default=1)
      The number of threads to use for running TF Lite interpreter.
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

[1] https://github.com/tensorflow/tensorflow/tree/r2.3/tensorflow/lite/tools/benchmark

Files:
   benchmark_main.cpp - benchmark main function
   benchmark_model.cpp - benchmark parameters handling
   benchmark_tflite_model.cpp - input and output processing
   model.h - benchmarked model data


Toolchain supported
===================
- GCC ARM Embedded  9.3.1
- MCUXpresso  11.3.1
- IAR embedded Workbench  8.50.9
- Keil MDK  5.33

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output in the terminal window for two runs of the demo.
Once with the enable_op_profiling parameter set to false and once set to true.

STARTING!
Min num runs: [50]
Min runs duration (seconds): [1]
Max runs duration (seconds): [150]
Inter-run delay (seconds): [-1]
Num threads: [1]
Use caching: [0]
Benchmark name: []
Output prefix: []
Min warmup runs: [1]
Min warmup runs duration (seconds): [0.5]
Input layers: []
Input shapes: []
Input value ranges: []
Input layer values files: []
Allow fp16 : [0]
Require full delegation : [0]
Enable op profiling: [0]
Max profiling buffer entries: [1024]
CSV File to export profiling data to: []
Enable platform-wide tracing: [0]
Loaded model 
The input model file size (MB): 0.005912
Initialized session in 1.461ms.
Running benchmark for at least 1 iterations and at least 0.5 seconds but terminate if exceeding 150 seconds.
count=7837 first=114 curr=32 min=28 max=114 avg=31.462 std=1

Running benchmark for at least 50 iterations and at least 1 seconds but terminate if exceeding 150 seconds.
count=15523 first=51 curr=32 min=28 max=51 avg=31.4759 std=0

Inference timings in us: Init: 1461, First inference: 114, Warmup (avg): 31.462, Inference (avg): 31.4759
Inference timings in us: Init: 1461, First inference: 114, Warmup (avg): 31.462, Inference (avg): 31.4759
Benchmark has been finished


STARTING!
Min num runs: [50]
Min runs duration (seconds): [1]
Max runs duration (seconds): [150]
Inter-run delay (seconds): [-1]
Num threads: [1]
Use caching: [0]
Benchmark name: []
Output prefix: []
Min warmup runs: [1]
Min warmup runs duration (seconds): [0.5]
Input layers: []
Input shapes: []
Input value ranges: []
Input layer values files: []
Allow fp16 : [0]
Require full delegation : [0]
Enable op profiling: [1]
Max profiling buffer entries: [1024]
CSV File to export profiling data to: []
Enable platform-wide tracing: [0]
Loaded model 
The input model file size (MB): 0.005912
Initialized session in 2.45ms.
Running benchmark for at least 1 iterations and at least 0.5 seconds but terminate if exceeding 150 seconds.
count=6746 first=129 curr=43 min=34 max=1039 avg=39.1513 std=17

Running benchmark for at least 50 iterations and at least 1 seconds but terminate if exceeding 150 seconds.
count=2254 first=85 curr=60 min=51 max=85 avg=58.311 std=2

Inference timings in us: Init: 2450, First inference: 129, Warmup (avg): 39.1513, Inference (avg): 58.311
Inference timings in us: Init: 2450, First inference: 129, Warmup (avg): 39.1513, Inference (avg): 58.311
Profiling Info for Benchmark Initialization:
	             [node type]	          [start]	  [first]	 [avg ms]	     [%]	  [cdf%]	  [mem KB]	[times called]	[Name]
	         AllocateTensors	            0.000	    0.147	    0.147	100.000%	100.000%	     0.000	        1	AllocateTensors/ld

	             [node type]	          [start]	  [first]	 [avg ms]	     [%]	  [cdf%]	  [mem KB]	[times called]	[Name]
	         AllocateTensors	            0.000	    0.147	    0.147	100.000%	100.000%	     0.000	        1	AllocateTensors/ld

Number of nodes executed: 1
	             [Node type]	  [count]	  [avg ms]	    [avg %]	    [cdf %]	  [mem KB]	[times called]
	         AllocateTensors	        1	     0.147	   100.000%	   100.000%	     0.000	        1

Timings (microseconds): count=1 curr=147
Memory (bytes): count=0
1 nodes observed



Operator-wise Profiling Info for Regular Benchmark Runs:
	             [node type]	          [start]	  [first]	 [avg ms]	     [%]	  [cdf%]	  [mem KB]	[times called]	[Name]
	         FULLY_CONNECTED	            0.000	    0.030	    0.016	 34.118%	 34.118%	     0.000	        1	[dense_1/BiasAdd]:ld
	                    TANH	            0.017	    0.007	    0.004	  9.175%	 43.293%	     0.000	        1	[dense_1/Tanh]:ld
	         FULLY_CONNECTED	            0.022	    0.006	    0.005	 10.339%	 53.632%	     0.000	        0	[dense_2/BiasAdd]:ld
	                    TANH	            0.027	    0.003	    0.002	  5.381%	 59.013%	     0.000	        1	[dense_2/Tanh]:ld
	         FULLY_CONNECTED	            0.030	    0.006	    0.003	  7.606%	 66.619%	     0.000	        1	[dense_3/BiasAdd]:ld
	                    TANH	            0.034	    0.003	    0.002	  5.423%	 72.043%	     0.000	        1	[dense_3/Tanh]:ld
	         FULLY_CONNECTED	            0.037	    0.009	    0.007	 15.714%	 87.756%	     0.000	        1	[dense_4/BiasAdd]:ld
	                    TANH	            0.045	    0.006	    0.006	 12.244%	100.000%	     0.000	        1	[dense_4/Tanh]:ld

	             [node type]	          [start]	  [first]	 [avg ms]	     [%]	  [cdf%]	  [mem KB]	[times called]	[Name]
	         FULLY_CONNECTED	            0.000	    0.030	    0.016	 34.118%	 34.118%	     0.000	        1	[dense_1/BiasAdd]:ld
	         FULLY_CONNECTED	            0.037	    0.009	    0.007	 15.714%	 49.831%	     0.000	        1	[dense_4/BiasAdd]:ld
	                    TANH	            0.045	    0.006	    0.006	 12.244%	 62.075%	     0.000	        1	[dense_4/Tanh]:ld
	         FULLY_CONNECTED	            0.022	    0.006	    0.005	 10.339%	 72.414%	     0.000	        0	[dense_2/BiasAdd]:ld
	                    TANH	            0.017	    0.007	    0.004	  9.175%	 81.589%	     0.000	        1	[dense_1/Tanh]:ld
	         FULLY_CONNECTED	            0.030	    0.006	    0.003	  7.606%	 89.195%	     0.000	        1	[dense_3/BiasAdd]:ld
	                    TANH	            0.034	    0.003	    0.002	  5.423%	 94.619%	     0.000	        1	[dense_3/Tanh]:ld
	                    TANH	            0.027	    0.003	    0.002	  5.381%	100.000%	     0.000	        1	[dense_2/Tanh]:ld

Number of nodes executed: 8
	             [Node type]	  [count]	  [avg ms]	    [avg %]	    [cdf %]	  [mem KB]	[times called]
	         FULLY_CONNECTED	        4	     0.029	    69.048%	    69.048%	     0.000	        3
	                    TANH	        4	     0.013	    30.952%	   100.000%	     0.000	        4

Timings (microseconds): count=2254 first=70 curr=47 min=37 max=70 avg=45.5075 std=2
Memory (bytes): count=0
8 nodes observed



Benchmark has been finished

