# CAN Bus transport

This section describes configuration parameters used when CAN transport is used:

```c
#define FMSTR_TRANSPORT FMSTR_CAN
```

## FMSTR_CAN_DRV

Select what low-level driver interface will be used when implementing the CAN communication.

```c
#define FMSTR_CAN_DRV [identifier]
```

##### Value Type

Driver identifiers are structure instance names defined in FreeMASTER drivers code. Specify
one of existing CAN driver instances.

##### Description

When using MCUXpresso SDK, use one of the following constants (see */drivers/mcuxsdk/can implementation*):
 - **FMSTR_CAN_MCUX_FLEXCAN** - FlexCAN driver
 - **FMSTR_CAN_MCUX_MCAN** - MCAN driver
 - **FMSTR_CAN_MCUX_MSCAN** - msCAN driver
 - **FMSTR_CAN_MCUX_DSCFLEXCAN** - DSC FlexCAN driver
 - **FMSTR_CAN_MCUX_DSCMSCAN** - DSC msCAN driver
 
Other SDKs or BSPs may define the custom low-level driver interface structure which may be used as FMSTR_CAN_DRV.


## FMSTR_CAN_BASE

```c
#define FMSTR_CAN_BASE [address|symbol]
```

##### Value Type

Optional address value (numeric or symbolic)

##### Description

Specify the base address of the FlexCAN, msCAN, or other CAN peripheral module to be used for the communication.
This value is not defined by default. User application should call `FMSTR_SetCanBaseAddress()` to select the peripheral module.


## FMSTR_CAN_CMDID

```c
#define FMSTR_CAN_CMDID [number]
```

##### Value Type

CAN identifier (11-bit or 29-bit number)

##### Description

CAN message identifier used for FreeMASTER commands (direction from PC Host tool to target application).
When declaring 29-bit identifier, combine the numeric value with FMSTR_CAN_EXTID bit.
Default value is 0x7AA.


## FMSTR_CAN_RSPID

```c
#define FMSTR_CAN_RSPID [number]
```

##### Value Type

CAN identifier (11-bit or 29-bit number)

##### Description

CAN message identifier used for responding messages (direction from target application to PC Host tool).
When declaring 29-bit identifier, combine the numeric value with FMSTR_CAN_EXTID bit. 
Note that both *CMDID* and *RSPID* values may be the same.
Default value is 0x7AA.


## FMSTR_FLEXCAN_TXMB

```c
#define FMSTR_FLEXCAN_TXMB [number]
```

##### Value Type

Number in range of 0..N where N is number of CAN message-buffers supported by HW module.

##### Description

Only used when the FlexCAN low-level driver is used. Define the FlexCAN message buffer for CAN frame transmission.
Default value is 0.


## FMSTR_FLEXCAN_RXMB

```c
#define FMSTR_FLEXCAN_RXMB [number]
```

##### Value Type

Number in range of 0..N where N is number of CAN message-buffers supported by HW module.

##### Description

Only used when the FlexCAN low-level driver is used. Define the FlexCAN message buffer for CAN frame reception.
Note that the FreeMASTER driver may also operate with a common message buffer used by both TX and RX directions.
Default value is 1.
