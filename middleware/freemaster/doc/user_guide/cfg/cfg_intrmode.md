# Interrupt modes

```c
#define FMSTR_LONG_INTR   [0|1]
#define FMSTR_SHORT_INTR  [0|1]
#define FMSTR_POLL_DRIVEN [0|1]
```

##### Value Type

boolean (0 or 1)

##### Description

Exactly one of the three macros must be defined to non-zero. The others must be defined to zero or left undefined. The non-zero-defined constant selects the interrupt mode of the driver. See [Driver interrupt modes](driver-interrupt-modes).

 - `FMSTR_LONG_INTR` — long interrupt mode
 - `FMSTR_SHORT_INTR` — short interrupt mode
 - `FMSTR_POLL_DRIVEN` — poll-driven mode

 **Note:** Some options may not be supported by all communication interfaces. For example, the FMSTR_SHORT_INTR option is not supported by the USB_CDC interface.

