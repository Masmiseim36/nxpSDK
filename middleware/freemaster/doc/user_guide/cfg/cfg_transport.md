# Protocol transport

```c
#define FMSTR_TRANSPORT [identifier]
```

##### Value Type

Driver identifiers are structure instance names defined in FreeMASTER source code. Specify
one of existing instances to make use of the protocol transport.

##### Description

Use one of the pre-defined constants, as implemented by the FreeMASTER code. 
The current driver supports the following transports:

 - **FMSTR_SERIAL** - serial communication protocol
 - **FMSTR_CAN** - using CAN communication
 - **FMSTR_PDBDM** - using packet-driven BDM communication
 - **FMSTR_NET** - network communication using TCP or UDP protocol
