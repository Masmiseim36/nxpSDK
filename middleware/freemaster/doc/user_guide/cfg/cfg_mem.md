# Memory access

## FMSTR_USE_READMEM

```c
#define FMSTR_USE_READMEM [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Define as non-zero to implement the Memory Read command and enable FreeMASTER to 
have read access to memory and variables. The access can be further restricted
by using a TSA feature.  
Default value is 1 (true).


## FMSTR_USE_WRITEMEM

```c
#define FMSTR_USE_WRITEMEM [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Define as non-zero to implement the Memory Write command.  
The default value is 1 (true).


