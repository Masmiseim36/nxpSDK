..
    Copyright 2019,2020 NXP



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
- MCUXpresso  installed (for running aws demo on K64F)
- Any Serial communicator
- Flash VCOM binary on the device. VCOM binary can found under :file:`<PROJECT>\binaries\MCU` folder.
- Refer to :ref:`cli-tool` for ssscli tool setup
- Build Plug & Trust middleware stack. (Refer :ref:`building`)


Using WiFi with LPC55S
=======================================================================

WiFi shield CMWC1ZZABR-107-EVB by muRata is supported with LPCS55S. Mount the WiFi shield on to the
mikroBUS stackable headers.


.. _prepare-aws-cloud:

Creating a device on AWS account
===========================================================================

Refer - https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs-first-thing.html


Creating  and updating device keys and certificates to SE
===========================================================================

1) Complete :numref:`cli-doc-pre-steps` :ref:`cli-doc-pre-steps`

#) Check the vcom port number

#)  To create certificates on windows and provision, go to ``simw-top/pycli`` directory and call::

        call venv\Scripts\activate.bat
        cd Provisioning
        python GenerateAWSCredentials.py <COM_PORT>
        python ResetAndUpdate_AWS.py <COM_PORT>

    .. note::
        Provisioning of the keys is done with default policies.
        Refer - :numref:`cli-object-policy` to change the scripts to add required policies.

#) Certificates and Keys are generated at ``simw-top/pycli/Provisioning/aws``

Building the Demo
=======================================================================
1) Open cmake project found under :file:`<SIMW-TOP>\projects` in MCUXPRESSO IDE

#) Update cmake options::
    - ``RTOS=FreeRTOS``
    - ``mbedTLS_ALT=SSS``

#) Update the build target in make file
    - Project:``cloud_aws``


Running the Demo
=======================================================================

1) Open a serial terminal on PC for OpenSDA serial device with these settings::

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
