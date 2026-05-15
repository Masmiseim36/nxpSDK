# FMSTR_Init

##### Prototype

```c
FMSTR_BOOL FMSTR_Init(void);
```

- Declaration: *freemaster.h*
- Implementation: *freemaster_protocol.c*

##### Description

This function initializes the internal variables of the FreeMASTER driver and enables the communication interface. This function does not change the configuration of the selected communication module. The hardware module must be initialized before the [FMSTR_Init](fmstr-init) function is called.

A call to this function must occur before calling any other FreeMASTER driver API functions.
