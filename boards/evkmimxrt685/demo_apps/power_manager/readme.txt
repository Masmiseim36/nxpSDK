Overview
========
The Power manager demo application demonstrates the use of power modes in the KSDK. The demo use the notification mechanism
and prints the power mode menu through the debug console, where the user can set the MCU to a specific power mode. The user
can also set the wakeup source by following the debug console prompts. The purpose of this demo is to demonstrate the
implementation of a power mode manager. The callback can be registered to the framework. If a power mode transition happens,
the callback will be called and user can do something.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin cirrent would be activied unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============
- Keep J2 and J19 unconnected to avoid unintentional reset by IF_RST signal in full deep power down mode.
- For low power purpose, the demo requires pad voltage supply to be fixed as follows:
  VDDIO_0: 1.8V (connecting JP20 1-2, and PMIC SW2_OUT using default setting 1.8V)
  VDDIO_1: 1.8V (connecting JP12 1-2, supply from PMIC SW2_OUT))
  VDDIO_2: 3.3V (connecting JP27 1-2, and PMIC LDO2_OUT using default setting 3.3V)
  NOTE: Supplying wrong voltage on the pads is harmful to the device.

Prepare the Demo
================
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
Customization options
=====================

