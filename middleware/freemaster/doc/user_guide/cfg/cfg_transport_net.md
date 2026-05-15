# Network transport

This section describes configuration parameters used when Network transport is used:

```c
#define FMSTR_TRANSPORT FMSTR_NET
```

## FMSTR_NET_DRV

Select network interface implementation.

```c
#define FMSTR_NET_DRV [identifier]
```

##### Value Type

Identifiers are structure instance names defined in FreeMASTER drivers code. Specify
one of existing NET driver instances.

##### Description

When using MCUXpresso SDK, use one of the following constants (see */drivers/mcuxsdk/network implementation*):
 - **FMSTR_NET_LWIP_TCP** - TCP communication using lwIP stack
 - **FMSTR_NET_LWIP_UDP** - UDP communication using lwIP stack
 - **FMSTR_NET_SEGGER_RTT** - Communication using SEGGER J-Link RTT interface

Other SDKs or BSPs may define the custom networking interface which may be used as FMSTR_CAN_DRV.

Add another row below:


## FMSTR_NET_PORT

```c
#define FMSTR_NET_PORT [number]
```

##### Value Type

TCP or UDP port number (short integer)

##### Description

Specifies the server port number used by TCP or UDP protocols.


## FMSTR_NET_BLOCKING_TIMEOUT

```c
#define FMSTR_NET_BLOCKING_TIMEOUT [number]
```

##### Value Type

Timeout as number of milliseconds

##### Description

This value specifies a timeout in milliseconds for which the network socket operations may block the execution inside [FMSTR_Poll](fmstr-poll). This may be set high (e.g. 250) when a dedicated RTOS task is used to handle FreeMASTER protocol polling. Set to a lower value when the polling task is also responsible for other operations. Set to 0 to attempt to use non-blocking socket operations.



## FMSTR_NET_AUTODISCOVERY

```c
#define FMSTR_NET_AUTODISCOVERY [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

This option enables the FreeMASTER driver to use a separate UDP socket to broadcast auto-discovery messages to network. This helps the FreeMASTER tool to discover the target device address, port and protocol options.

