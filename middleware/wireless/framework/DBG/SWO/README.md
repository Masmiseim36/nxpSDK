#  Debug features available in the framework

## SWO (Single Wire Output)
This ARM feature is supported by NXP platforms based on ARM cores. It allows to export debug information via a single wire (usually UART encoding), with very limited impact on real-time execution.

On multi-core platforms, it is usually required to configure some FUNNEL block to select which core should be routed to SWO output.

SWO natively exports interrupts timing information. Optionally, the software can implement some ITM (Instrumentation Trace Macrocell) API calls to export some trace values.

### Instrument your code with SWO primitives
Check:
- ITM_SendChar() from core_cm33.h
- ITM_EVENT8()/ITM_EVENT16()/ITM_EVENT32() from arm_itm.h


### Using SWO from a debugger
Debuggers GUI usually provide some ways to decode SWO data flow and display SWO data in several forms. IAR Workbench offers J-Link menu with several options such as "SWO Trace" or "Timeline".


