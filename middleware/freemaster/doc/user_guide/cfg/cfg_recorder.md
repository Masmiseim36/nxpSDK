# Recorder options

## FMSTR_USE_RECORDER

```c
#define FMSTR_USE_RECORDER [number]
```

##### Value Type

Integer number.

##### Description

Number of Recorder instances to be supported. Set to 0 to disable the Recorder feature.  
Default value is 0.

## FMSTR_REC_BUFF_SIZE

```c
#define FMSTR_REC_BUFF_SIZE [number]
```

##### Value Type

Integer number larger than 2.

##### Description

Defines the size of the memory buffer used by the Recorder instance \#0.  
Default: not defined, user shall call `FMSTR_RecorderCreate()`` API function to specify this parameter in run time.

## FMSTR_REC_TIMEBASE

```c
#define FMSTR_REC_TIMEBASE [time specification]
```

##### Value Type

Number (nanoseconds time).

##### Description

Defines the base sampling rate in nanoseconds (sampling speed) Recorder instance \#0.

Use one of the following macros:
 - `FMSTR_REC_BASE_SECONDS(x)`
 - `FMSTR_REC_BASE_MILLISEC(x)`
 - `FMSTR_REC_BASE_MICROSEC(x)`
 - `FMSTR_REC_BASE_NANOSEC(x)`

Default: not defined, user shall call `FMSTR_RecorderCreate()`` API function to specify this parameter in run time.


## FMSTR_REC_FLOAT_TRIG

```c
#define FMSTR_REC_FLOAT_TRIG [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Define as non-zero to implement the floating-point triggering. Be aware that floating-point triggering may grow the code size by linking the floating-point standard library.

Default value is 0 (false).
