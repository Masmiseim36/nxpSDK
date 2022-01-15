..
    Copyright 2019,2020 NXP


.. highlight:: bat

.. _ksdk-demos-gcp:

=======================================================================
 GCP Demo for KSDK
=======================================================================

This demo demonstrates connection to Google Cloud Platform using
pre-provisioned device credentials and publish/subscribe procedure
using MQTT.

Prerequisites
=======================================================================
- Active gcp account
- MCUXpresso  installed (for running gcp demo on K64F)
- Any Serial communicator
- Flash VCOM binary on the device. VCOM binary can found under :file:`<PROJECT>\binaries\MCU` folder.
- Refer to :ref:`cli-tool` for ssscli tool setup
- Build Plug & Trust middleware stack. (Refer :ref:`building`)


Using WiFi with LPC55S
=======================================================================

WiFi shield Silex-2401 is supported with LPCS55S. Mount the WiFi shield on to the
arduino stackable headers.


Creating and updating device keys and certificates to SE
===========================================================================

1) Complete :numref:`cli-doc-pre-steps` :ref:`cli-doc-pre-steps`

#) Check the vcom port number

#)  To create certificates on windows and provision, go to ``simw-top/pycli`` directory and call::

        call venv\Scripts\activate.bat
        cd Provisioning
        python GenerateGCPCredentials.py <COM_PORT>
        python ResetAndUpdate_GCP.py <COM_PORT>

    .. note::
        Provisioning of the keys is done with default policies.
        Refer - :numref:`cli-object-policy` to change the scripts to add required policies.

#) Certificates and Keys are generated at ``simw-top/pycli/Provisioning/gcp``


.. _prepare-gcp-cloud:

Preparing the Cloud
=======================================================================
1) Updating cloud over command line. Set up ``gcloud`` command line utility availabe at https://cloud.google.com/pubsub/docs/quickstart-cli

- Assuming the project name is ``pgh-cloud-iot`` the following commands sets up the code.

  Create a new events pub/sub topic::

    ,----
    |
    | gcloud pubsub topics create a71ch-demo-events \
    |     --project=pgh-cloud-iot
    |
    `----

- Create a registry::

    ,----
    |
    | gcloud iot registries create nxp-se-demo-reg \
    |     --project=pgh-cloud-iot \
    |     --region=us-central1 \
    |     --event-notification-config=topic=projects/pgh-cloud-iot/topics/a71ch-demo-events \
    |
    `----

- Create a device and attach the certificate ``tls_client.cer``::

    ,----
    |
    | gcloud iot devices create nxp-ecc-dev-01 \
    |     --project=pgh-cloud-iot \
    |     --region=us-central1 \
    |     --registry=nxp-se-demo-reg \
    |     --public-key=path=/simw-top/pycli/Provisioning/gcp/<UID>_device_certificate.cer,type=es256-pem
    |
    `----


2) Updating cloud using the Web Interface

   A) Sign up for Google Cloud Platform - IoT (If you have not done that already)
   #) Create Registry & Device in the cloud platform.
   #) Copy For the device, add public key in ES256_X509 format
      Copy hostLib\mbedtls\ecc\tls_client.cer and paste in the
      web-dialogue box.


Building the Demo
=======================================================================
1) Open cmake project found under :file:`<SIMW-TOP>\projects` in MCUXPRESSO IDE

#) Update cmake options::
    - ``RTOS=FreeRTOS``
    - ``mbedTLS_ALT=SSS``

#) Update the build target in make file
    - Project:``cloud_gcp``


Running the Demo
=======================================================================
1) Build the project and flash the binary on FRDM-K64F board

#) Connect your board to open network

#) Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - change Setup->Terminal->New-line->Receive->AUTO


#)  Console output - If everything is setup correctly the output would be as follows ::

      ,----
      |
      | GCP JWT NXP Secure Element example
      |
      | selectResponseDataLen: 2
      | 0x01:0x31:
      | Associating ECC key-pair '0'.
      | Connecting to network
      | Getting IP address from DHCP ...
      |
      |  IPv4 Address     : 192.168.1.55
      | DHCP OK
      | Current EPOCH = 1520599186
      | Using ECC key '0' for signing.
      | JWT TOKEN = eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJwZ2gtY2xvdWQtaW90IiwiaWF0IjoxNTIwNTk5MTg2LCJleHAiOjE1MjA2MzUxODZ9.pZK9NjzD2rMdsU9H6bLPHNTsjHE77zHTMNhxVDVR3fYo39ttM2gYrhvJBR2Ct-9a2o8FwFqWjR8YY_lDwGjYyg
      | GAE subscribe publish example
      |
      | Connecting...
      | Associating ECC key-pair '0'.
      | Using ECC key '0' to compute shared secret.
      | Subscribing...
      | -->sleep
      | -->sleep
      | Publish done
      |
      | Subscribe callback
      |
      | ...
      | ...
      |
      `----

#)  You can update device config with following messages to toggle on-board keys. Using the below command, we can toggle LEDs::

      ,----
      |
      | gcloud iot devices configs update \
      |     --project=pgh-cloud-iot \
      |     --region=us-central1 \
      |     --registry=nxp-se-demo-reg \
      |     --device=nxp-ecc-dev-01 \
      |     --config-data='{"red": "off"}'
      |
      `----

    User can toggle individual LEDs::

      ,----
      | {"green": "toggle", "user": "test1"}
      | {"green": "on",     "user": "test1"}
      | {"red":   "off",    "user": "test1"}
      `----

    For DOS Batch files, the commands can be like below (with escaping)::

      ,----
      |
      | gcloud iot devices configs update ^
      |     --project=pgh-cloud-iot ^
      |     --region=us-central1 ^
      |     --registry=nxp-se-demo-reg ^
      |     --device=nxp-ecc-dev-01 ^
      |     --config-data=^"{""red"":""on"",""blue"":""off"",""green"":""off""}^"
      |
      | gcloud iot devices configs update ^
      |     --project=pgh-cloud-iot ^
      |     --region=us-central1 ^
      |     --registry=nxp-se-demo-reg ^
      |     --device=nxp-ecc-dev-01 ^
      |     --config-data=^"{""red"":""off"",""blue"":""on"",""green"":""off""}^"
      |
      | gcloud iot devices configs update ^
      |     --project=pgh-cloud-iot ^
      |     --region=us-central1 ^
      |     --registry=nxp-se-demo-reg ^
      |     --device=nxp-ecc-dev-01 ^
      |     --config-data=^"{""red"":""off"",""blue"":""off"",""green"":""on""}^"
      |
      `----



Appendix
========

1. For more information, refer to https://github.com/GoogleCloudPlatform/cpp-docs-samples/tree/master/iot/mqtt-ciotc
