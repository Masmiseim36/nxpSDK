..
    Copyright 2019 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.


.. highlight:: bat

.. _ksdk-demos-azure:

===========================================================================
 Azure Demo for KSDK
===========================================================================

This demo demonstrates connection to Azure IoTHub using pre-provisioned 
device credentials and demonstrates publish/subscribe procedure using MQTT.

Prerequisites
===========================================================================

- Active azure account with iot hub created
- MCUXpresso  installed (for running azure demo on k64)
- Any Serial communicator
- Flash VCOM binary on the device. VCOM binary can found under :file:`<PROJECT>/binaries` folder.
- Refer to :ref:`cli-tool` for pyCLI tool setup


Using WiFi with LPC55S
=======================================================================

WiFi shield Silex-2401 is supported with LPCS55S. Mount the WiFi shield on to the 
arduino stackable headers.


Creating a device on azure IoT Hub portal
===========================================================================

1.  Navigate to the Dashboard --> <Your IoT Hub> --> "IoT Devices"
#.  Add a new device (e.g. device1), and for its authentication type
    choose X.509 CA Signed

    .. note:: Creating a new device
        For large deployment of device, creating a device on azure iot hub can be done using Azure IoT service SDK


Creating  and updating device keys and certificates to SE
===========================================================================

1) Complete :numref:`cli-doc-pre-steps` :ref:`cli-doc-pre-steps`

#) call::

    call venv\Scripts\activate.bat
    cd Provisioning

#) Check the vcom port number

#)  To create certificates on windows and provision, call::

        cd simw-top/pycli/Provisioning
        python GenerateAZURECredentials.py
        python ResetAndUpdate_AZURE.py <COM_PORT>

#) Certificates and Keys are generated at simw-top/pycli/Provisioning/azure


Uploading root certificates to IoT Hub
===========================================================================

1) On Azure IoT Hub portal, Navigate to ``Dashboard --> <Your IoT Hub> --> Certificates``. Click on Add

#) Enter a friendly name and upload the root certificate created in the previous step. Location -  ``simw-top/pycli/Provisioning/azure/RootCA.cer`` -> Save

#) Your certificate will show in the Certificate Explorer list. Click on certificate added

#) In Certificate Details, click Generate Verification Code

#) The provisioning service creates a Verification Code that you can use to validate the certificate ownership. Copy the code to your clipboard

#) Use the verification_certificate.py to generate a verify certificate (verifyCert4.cer) ::
        
        cd simw-top/pycli/Provisioning
        python verification_certificate.py <RootCA_Certificate> <RootCA_Keypair> <Verification Code>

#) On ``Azure portal -> Certificate Details``, upload the verifyCert4.cer file generated and click Verify.
   
   STATUS of your certificate should change to ``Verified`` in the Certificate Explorer list



Running the Demo
===========================================================================

1) Open frdmk64f_mbedtls_sss_azure_demo project found under :file:`<PROJECT>\projects` in MCUXPRESSO IDE
#) Update ``AZURE_IOT_HUB_NAME``  and ``AZURE_DEVICE_NAME`` in ``<PROJECT>\demo\azure_demo\azure_iot_config.h`` with your details
#) Build the project and flash the binary on FRDM-K64F board
#) Connect your board to open network
#) Open a serial terminal on PC for OpenSDA serial device with these settings
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO

#) Console Output

- Press reset button on the board
  If everything is setup correctly, the output would be as follows::

    ,----
    |
    | Connecting to network
    | Getting IP address from DHCP ...
    |
    | IPv4 Address     : 192.168.0.68
    | DHCP OK
    | MQTT attempting to connect to 'dev2'...
    |
    | Signing using key lX
    | MQTT Echo demo subscribed to devices/dev2/messages/devicebound/#
    | -->sleepEcho successfully published
    | Echo successfully published
    | -->sleepEcho successfully published
    | Echo successfully published
    |
    |   ...
    |   ...
    |
    `----

- You can use device explorer tool to control the on-board LEDs.
  Open device explorer tool and update the IoT Hub connection string to
  connect to azure IoT hub, IoT Hub connection string is found at Azure
  IoT Hub -> Shared access policies -> iothubowner -> Connection String
  Primary key

- In the "Message To Devices" tab, select the device and send the
  message as::

    ,----
    | {"green": "toggle"}, {"green": "on"}, {"red": "off"}
    `----


Appendix
========

1. for more inforation, refer to https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-mqtt-support
