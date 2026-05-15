# Application Commands options

## FMSTR_USE_APPCMD

```c
#define FMSTR_USE_APPCMD [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Define as non-zero to implement the Application Commands feature.  
Default value is 0 (false).


## FMSTR_APPCMD_BUFF_SIZE

```c
#define FMSTR_APPCMD_BUFF_SIZE [size]
```

##### Value Type

Numeric buffer size in range 1..255

##### Description

The size of the Application Command data buffer allocated by the driver. The buffer stores the (optional) parameters of the Application Command which waits to be processed.


## FMSTR_MAX_APPCMD_CALLS

```c
#define FMSTR_MAX_APPCMD_CALLS [number]
```

##### Value Type

Number in range 0..255

##### Description

The number of different Application Commands that can be assigned a callback handler function using `FMSTR_RegisterAppCmdCall()`.
Default value is 0.
