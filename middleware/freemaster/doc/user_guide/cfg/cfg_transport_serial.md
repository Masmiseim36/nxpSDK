# Serial transport

This section describes configuration parameters used when serial transport is used:

```c
#define FMSTR_TRANSPORT FMSTR_SERIAL
```

## FMSTR_SERIAL_DRV

Select what low-level driver interface will be used when implementing the Serial communication.

```c
#define FMSTR_SERIAL_DRV [identifier]
```

##### Value Type

Driver identifiers are structure instance names defined in FreeMASTER drivers code. Specify
one of existing serial driver instances.

##### Description

When using MCUXpresso SDK, use one of the following constants (see */drivers/mcuxsdk/serial* implementation):

 - **FMSTR_SERIAL_MCUX_UART** - UART driver
 - **FMSTR_SERIAL_MCUX_LPUART** - LPUART driver
 - **FMSTR_SERIAL_MCUX_USART** - USART driver
 - **FMSTR_SERIAL_MCUX_MINIUSART** - miniUSART driver
 - **FMSTR_SERIAL_MCUX_QSCI** - DSC QSCI driver
 - **FMSTR_SERIAL_MCUX_USB** - USB/CDC class driver (also see code in the */support/mcuxsdk_usb* folder)
 - **FMSTR_SERIAL_56F800E_EONCE** - DSC JTAG EOnCE driver

Other SDKs or BSPs may define custom low-level driver interface structure which may be used as FMSTR_SERIAL_DRV. For example:

 - **FMSTR_SERIAL_DREG_UART** - demonstrates the low-level interface implemented without the MCUXpresso SDK and using direct access to peripheral registers.


## FMSTR_SERIAL_BASE

```c
#define FMSTR_SERIAL_BASE [address|symbol]
```

##### Value Type

Optional address value (numeric or symbolic)

##### Description

Specify the base address of the UART, LPUART, USART, or other serial peripheral module to be used for the communication.
This value is not defined by default. User application should call `FMSTR_SetSerialBaseAddress()` to select the peripheral module.


## FMSTR_COMM_BUFFER_SIZE

```c
#define FMSTR_COMM_BUFFER_SIZE [number]
```

##### Value Type

0 or a value in range 32...255

##### Description

Specify the size of the communication buffer to be allocated by the driver.  
Default value, which suits all driver features, is used when this option is defined as 0.


## FMSTR_COMM_RQUEUE_SIZE

```c
#define FMSTR_COMM_RQUEUE_SIZE [number]
```

##### Value Type

Value in range 0...255

##### Description

Specify the size of the FIFO receiver queue used to quickly receive and store characters in the FMSTR_SHORT_INTR interrupt mode.  
The default value is 32 B.

## FMSTR_SERIAL_SINGLEWIRE

```c
#define FMSTR_SERIAL_SINGLEWIRE [0|1]
```

##### Value Type

Boolean 0 or 1.

##### Description

Set to non-zero to enable the “True” single-wire mode which uses a single MCU pin to communicate. The low-level driver enables the pin direction switching when the MCU peripheral supports it.

