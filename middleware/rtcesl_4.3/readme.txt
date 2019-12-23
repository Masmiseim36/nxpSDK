RTCESL - Real Time Control Embedded Software Libraries are 
a complimentary group of algorithms, ranging from basic mathematics 
operations to advanced transformations and observers, which can be easily 
incorporated into complex real-time control applications. The complimentary
algorithms help to speed development and ease of use in applications that
require intensive math computation and control such as advanced high
efficiency motor control and power conversion.

The libraries are highly optimized, are tested on NXP hardware, and are
easy to use as they are implemented with a C-callable function interface.

The libraries are currently available for the Kinetis ARM Cortex M0+, M4 and M7
with the introduction of a universal API across all our libraries we have made 
available a tool to the help convert your API's. To quickly move from the API 
that you used in either RTCESL 3.0 or MMCLIB to the new universal API used in 
FSLESL 4.3. use API Translator. This simple tool will help migrate your code by
converting your old API to the new version. 

RTCESL consists from:

Math Library (MLIB)
Simple math functions (addition, subtraction, multiplication,
division, shifts...) with and without saturation. This aim of this
block is to have efficient and fast mathematics built on the
MCU’s instructions.

General Function Library (GFLIB)
Contains the basic building blocks of a real-time control
application. Functions for basic mathematical calculations,
trigonometric functions, simple look-up table and control
functions such as PI and PID controllers.

General Motor Control Library (GMCLIB)
The fundamental blocks of a motor control application. The
libraries include vector modulation, Park and Clarke
transformations and specific motor related functions to build
digitally controlled motor drives.

General Digital Filter Library (GDFLIB)
Includes filter functions for signal conditioning.

Advanced Control Library (ACLIB)
Functions that enable the construction of a variable speed, AC
motor drive system that implements field oriented control
techniques without position or speed sensors to provide the
lowest cost solution.

Power Control Library (PCLIB)
Contains control loop algorithms required in Power Control applications.