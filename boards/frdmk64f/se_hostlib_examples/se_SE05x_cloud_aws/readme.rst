..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. highlight:: batch

.. _ksdk-demos-aws:

=======================================================================
 AWS Demo for KSDK
=======================================================================

This demo demonstrates connection to AWS IoT Console using pre-provisioned
device credentials and publish/subscribe procedure using MQTT.

Prerequisites
=======================================================================
- Active AWS account
- MCUXpresso  installed (for running aws demo on k64)
- Any Serial communicator
- Flash VCOM binary on the device. VCOM binary can found under :file:`<PROJECT>\binaries` folder.
- Refer to :ref:`cli-tool` for pyCLI tool setup


Using WiFi with LPC55S
=======================================================================

WiFi shield Silex-2401 is supported with LPCS55S. Mount the WiFi shield on to the
arduino stackable headers.


.. _prepare-aws-cloud:

Creating a device on AWS account
===========================================================================
TO BE UPDATED,


Creating  and updating device keys and certificates to SE
===========================================================================

1) Complete :numref:`cli-doc-pre-steps` :ref:`cli-doc-pre-steps`

#) call::

    call venv\Scripts\activate.bat
    cd Provisioning

#) Check the vcom port number

#)  To create certificates on windows and provision, call::

        cd simw-top/pycli/Provisioning
        python GenerateAWSCredentials.py
        python ResetAndUpdate_AWS.py <COM_PORT>

#) Certificates and Keys are generated at simw-top/pycli/Provisioning/aws


Running the Demo
=======================================================================
1) Open frdmk64f_mbedtls_sss_aws_demo project found under :file:`<PROJECT>\projects` in MCUXPRESSO IDE

#) Open a serial terminal on PC for OpenSDA serial device with these settings::

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO

#) Connect the boards's RJ45 to network with Internet access (IP address to the
   board is assigned by the DHCP server). Make sure the connection on port 8883
   is not blocked.

#) Download the program to the target board.

#) Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

#) The BLUE LED is turned ON during boot

#) Persistent RED LED ON indicates error

#) First time during connection, the device certificate needs to be
    - Activated
    - Attached with a policy that allows usage of this certificate

#) All lights off along with the following message indicates readiness to
   subscribe messages from AWS::

        Subscribing...
        -->sleep
        -->sleep
        Publish done

   In AWS IOT shadow, the following indicates the state of the LED::

        {
            "desired": {
            "COLOR": "RED",
            "MODE": "OFF"
            }
        }

   MODE can be ON or OFF and COLOR can be RED, GREEN or BLUE
