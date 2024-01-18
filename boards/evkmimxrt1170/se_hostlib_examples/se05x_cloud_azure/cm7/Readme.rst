..
    Copyright 2019,2020 NXP



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
- MCUXpresso  installed (for running azure demo on K64F)
- Any Serial communicator
- Flash VCOM binary on the device. VCOM binary can found under :file:`<PROJECT>/binaries\MCU` folder.
- Refer to :ref:`cli-tool` for ssscli tool setup
- Build Plug & Trust middleware stack. (Refer :ref:`building`)


Using WiFi with LPC55S
=======================================================================

WiFi shield CMWC1ZZABR-107-EVB by muRata is supported with LPCS55S. Mount the WiFi shield on to the
mikroBUS stackable headers.


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

#) Check the vcom port number

#)  To create certificates on windows and provision, go to ``simw-top/pycli`` directory and call::

        call venv\Scripts\activate.bat
        cd Provisioning
        python GenerateAZURECredentials.py <COM_PORT>
        python ResetAndUpdate_AZURE.py <COM_PORT>

    .. note::
        Provisioning of the keys is done with default policies.
        Refer - :numref:`cli-object-policy` to change the scripts to add required policies.

#) Certificates and Keys are generated at ``simw-top/pycli/Provisioning/azure``


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


Building the Demo
=======================================================================
1) Open cmake project found under :file:`<SIMW-TOP>\projects` in MCUXPRESSO IDE

#) Update cmake options::
    - ``RTOS=FreeRTOS``
    - ``mbedTLS_ALT=SSS``

#) Update the build target in make file
    - Project:``cloud_azure``


Running the Demo
===========================================================================

1) Update ``AZURE_IOT_HUB_NAME``  and ``AZURE_DEVICE_NAME`` in ``<PROJECT>\demo\azure_demo\azure_iot_config.h`` with your details
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
