Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
============
- Keep J2 and J19 unconnected to avoid unintentional reset by IF_RST signal in full deep power down mode.
- For low power purpose, the demo requires pad voltage supply to be fixed as follows:
  VDDIO_0: 1.8V (connecting JP20 1-2, and PMIC SW2_OUT using default setting 1.8V)
  VDDIO_1: 1.8V (connecting JP12 1-2, supply from PMIC SW2_OUT))
  VDDIO_2: 3.3V (connecting JP27 1-2, and PMIC LDO2_OUT using default setting 3.3V)
  NOTE: By default, the demo allows wide voltage supply on VDDIO_1. To further reduce the leakage,
        Vdde1Range can be set to narrow range kPadVol_171_198. Special care should be taken that
        supplying wrong voltage on the pads is harmful to the device.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power Manager Demo.
The "user key" is: SW1
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
        4. Full deep power down mode
/* Type in '1' into UART terminal */
Entering Sleep [Press the user key to wakeup] ...
/* Press the user key on board */
Pin event occurs
Wakeup.
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
        4. Full deep power down mode
/* Type in '2' into UART terminal */
Entering Deep Sleep [Press the user key to wakeup] ...
/* Press the user key on board */
Pin event occurs
Wakeup.
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
        4. Full deep power down mode
/* Type in '3' into UART terminal */
Entering Deep Powerdown [Reset to wakeup] ...
Press any key to confirm to enter the deep sleep mode and wakeup the device by reset.
/* Type in any key into UART terminal */

/* Press the reset key on board. */
Board wake up from deep or full deep power down mode.
Power Manager Demo.
The "user key" is: SW1
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
        4. Full deep power down mode
/* Type in '4' into UART terminal */
Entering Full Deep Powerdown [Reset to wakeup] ...
Press any key to confirm to enter the deep sleep mode and wakeup the device by reset.
/* Type in any key into UART terminal */

/* Press the reset key on board. */
Board wake up from deep or full deep power down mode.
Power Manager Demo.
The "user key" is: SW1
Select an option
        1. Sleep mode
        2. Deep Sleep mode
        3. Deep power down mode
        4. Full deep power down mode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
