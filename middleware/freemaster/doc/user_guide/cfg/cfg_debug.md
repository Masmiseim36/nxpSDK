# Debugging options

## FMSTR_DISABLE

```c
#define FMSTR_DISABLE [0|1]
```

##### Value Type

boolean (0 or 1)

##### Description

Define as non-zero to disable all FreeMASTER features, exclude the driver code from build, and compile all its API functions empty. This may be useful to remove FreeMASTER without modifying any application source code.
Default value is 0 (false).


## FMSTR_DEBUG_TX

```c
#define FMSTR_DEBUG_TX [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Define as non-zero to enable the driver to periodically transmit test frames out on the selected communication interface (SCI or CAN). With the debug transmission enabled, it is simpler to detect problems in the baudrate or other communication configuration settings. 

The test frames are transmitted until the first valid command frame is received from the PC Host tool. The test frame is a valid error status frame, as defined by the protocol format. On the serial line, the test frame consists of three printable characters (+©W) which are easy to capture using the serial terminal tools.

This feature requires the FMSTR_Poll() function to be called periodically.
Default value is 0 (false).

## FMSTR_APPLICATION_STR

```c
#define FMSTR_APPLICATION_STR
```

##### Value Type

String.

##### Description

Name of the application visible in FreeMASTER host application.


