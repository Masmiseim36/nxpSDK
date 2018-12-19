Overview
========

See "A71CH examples -> Hostlib API based examples"  in A71CH Host Lib Doxygen Guide.
"A71CH Host Lib Doxygen Guide" is installed to user PC when "A71CH Host Software Package 
(Windows Installer)" is installed from https://www.nxp.com/products/:A71CH 


Toolchain supported
===================
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board (Rev D or Later)
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
- Connect OM3710/A71CHARD to the board.

Prepare the Demo
================

1. Build the demo
2. Connect a USB cable between the PC host and the OpenSDA USB port on the
   target board.
3. Download the program to the target board.
4. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO
5. Either press the reset button on your board or launch the debugger in your
   IDE to begin running the demo.

Running the demo
================

On successful run of the example, output similar to below can be seen.

    a71ch HLSE example application (Rev 1.30:1.31)
    **********************************************
    Connect to A71CH-SM. Chunksize at link layer = 256.
    ATR=0xB8.04.11.01.05.04.B9.02.01.01.BA.01.01.BB.0C.41.37.31.30.78.43.48.32.34.32.52.31.BC.00.
    SCI2C_HostLib Version  : 0x0130
    Applet Version   : 0x0131
    SecureBox Version: 0x0000


    -----------
    Start exHlseCert()
    ------------

    -----------
    Start exDataObjUsage(Reset)
    ------------

    a71chInitModule(Reset)
    Reset A71CH.

    HLSE_CreateObject() - Create Data object...
    ------------

    HLSE_EnumerateObjects()...

    HLSE_GetObjectAttribute()...
            data object index = 0x0
    Verify Data object contents...
    Update Data object contents...
    Verify Data object Updated correctly...
    Show usage of direct read / Update access to Data Object...
        Verify Direct Read of Data Object...
        Verify Direct Update of Data Object...
        Test Direct Read / Write with offset that exceeds object boundaries...
        Test Direct Read / Write with length that exceeds object boundaries...

    HLSE_EraseObject()...

    Verify object was erased...

    -----------
    End exDataObjUsage(), result = OK
    ------------

    -----------
    Start exCertUsageBasic(Reset)
    ------------



End of project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
