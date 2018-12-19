Overview
========

This project connects to AWS cloud by using keys and certificates inside secure
module and subscibes to device push notifications.

THIS DEMO EXPECTS ROOT CA FOR THE DEVICE CERTIFICATE IS ALREADY UPLOADED TO THE
AWS ACCOUNT.  AND REQUIRED STEPS AS LISTED IN https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/
ARE FOLLOWED.

This project uses the intermediate CA certificate and the
device certificate pre-provisioned inside the secure module with AWS IOT
following which a secure communication channel is established.  Following this,
messages are subscribed from the AWS IOT and these messages are interpreted to
control a Tri-Color LED on the FRDM-K64F board.

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Prepare the Demo
================

1. AWS account must be pre configured with required backed scripts. (Eventually
   may be this can be done via Cloud Formation)
2. For Customer Programmable samples, follow steps in .\ecc\ReadMe.txt
3. Register the device intermediate CA and enable it for onboarding.
4. Build the demo
5. Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
6. Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO
7. a. In case of ethernet connection, connect the boards's RJ45 to network with Internet
      access.
   b. In case of wifi connection, adjust configuration macros in "wifi_config.h"
      according to your wi-fi router settings
          #define clientcredentialWIFI_SSID       "wifi_ssid"
          #define clientcredentialWIFI_PASSWORD   "password"
   c. IP address to the board is assigned by the DHCP server. Make sure the connection
      on port 8883 is not blocked.
8. Download the program to the target board.
9. Either press the reset button on your board or launch the debugger in your
   IDE to begin running the demo.

Running the demo
================
1. The BLUE LED is turned ON during boot
2. Persistent RED LED ON indicates error
3. First time during connection, the device certificate needs to be
	- Activated
	- Attached a policy that allows usage of this certificate
4. All lights off along with the following message indicates readiness to
   subscribe messages from AWS Subscribing...  -->sleep -->sleep Publish done
5. In AWS IOT shadow, the following indicates the state of the LED
   {
      "desired": {
        "COLOR": "RED",
        "MODE": "OFF"
      }
   }
   MODE can be ON or OFF and COLOR can be RED, GREEN or BLUE

If the Logger shows message "First Disconnect received while the device certificate
is registered. Is the device/CA certificate Activated"  please ensure Device or CA
Certificate are uploaded and activated in AWS Cloud.

End of project
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Board settings
==============
No special settings are required.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

