eIQ inference with DeepView RT 2.4.11

Content
-------
1. Introduction
3. Directory structure
4. eIQ inference with DeepView RT Example
5. Documentation
6. Library configuration
7. Release notes
8. Limitations

1. Introduction
---------------
DeepView RT is an implements a neural network inference engine suitable for embedded processors,
graphics processors, and microcontrollers. The DeepView RT Model (RTM) format supports in-place
interpretation which allows it to be stored directly in flash and used as-is. This allows for 
resource constrained MCU (ex: Cortex-M) platforms as the actual model and its weights do not 
consume any RAM and instead can be kept in-place in flash memory. A small amount of memory is
required for the network evaluation graph as well as a buffer cache for storing the volatile 
input/output data at inference time, but no memory is required for the actual weights which can
remain in flash. If the cache is large enough to host the weights as well, they will be streamed
from flash on-demand as a performance optimization, and for maximum performance on parts with 
adequate memory the entire model can be stored in RAM.
DeepView RT ModelRunner is a dedicated service for hosting and evaluating RTM graphs through a
set of RPC protocols, it provides an HTTP REST API for loading an evaluating models. It can work
with eIQ toolkit to evaluating and profiling models. 
Note: DeepView RT supports only a subset of operators available in
      TensorFlow. The eIQ toolkit can convert Tensorflow 2.x model to RTM model, please refer to
      eIQ toolkit about model conversion. 

2. Directory structure
--------------------------------------
<MCUXpresso-SDK-root>
|-- boards
|   -- <board>
|      -- eiq_examples                         - Example build projects
|         -- deepviewrt                        - deepviewrt modelrunner example
|
|-- middleware
    -- eiq
       -- deepviewrt
          -- include                   - DeepviewRT library header files
          -- lib                       - DeepviewRT library binaries

3. eIQ Inference with DeepView RT example
------------------------------------
The package contains modelrunner example applications using
the DeepView RT library. The build projects can be found in
the /boards/<board>/eiq_examples/deepviewrt* folders.
The following boards are currently supported: evkbimxrt1064.
The following toolchains are currently supported:
MCUXpresso IDE

4. Documentation
----------------
4.1. The DeepView RT library documentation is available at
     http://www.tensorflow.org/lite/.
4.2. How to convert models from the TensorFlow format is described at
     https://www.tensorflow.org/lite/convert/.

5. Library configuration
------------------------
5.1. Stack memory configuration
     During the library compilation, based on the stack memory configuration,
     the EIGEN_STACK_ALLOCATION_LIMIT macro definition can be set to the maximum
     size of temporary objects that can be allocated on the stack
     (they will be dynamically allocated instead). A high number may cause stack
     overflow. A low number may decrease object allocation performance.

6. Release notes
----------------
The library is based on DeepView RT version 2.4.11.

7. Limitations
--------------
* DeepView RT library depends on NewLib and it was only supported in MCUXpresso IDE.
