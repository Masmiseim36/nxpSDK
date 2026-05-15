# TSA options

## FMSTR_USE_TSA

```c
#define FMSTR_USE_TSA [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Enable the FreeMASTER TSA feature to be used. With this option enabled, the TSA tables defined
in the applications are made available to the FreeMASTER host tool.  
Default value is 0 (false).


## FMSTR_USE_TSA_SAFETY

```c
#define FMSTR_USE_TSA_SAFETY [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Enable the memory access validation in the FreeMASTER driver. With this option, the host tool is not able to access the memory which is not described by at least one TSA descriptor. Also a write access is denied for objects defined as read-only in TSA tables.  
Default value is 0 (false).


## FMSTR_USE_TSA_INROM

```c
#define FMSTR_USE_TSA_INROM [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Declare all TSA descriptors as *const*, which enables the linker to put the data into the flash memory. The actual result depends on linker settings or the linker commands used in the project.  
Default value is 0 (false).


## FMSTR_USE_TSA_DYNAMIC

```c
#define FMSTR_USE_TSA_DYNAMIC [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Enable runtime-defined TSA entries to be added to the TSA table by the `FMSTR_SetUpTsaBuff()` and `FMSTR_TsaAddVar()` functions.   
Default value is 0 (false).

