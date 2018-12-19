Overview
========
            ______________________________________________________

                  A71CH EXAMPLE : GOOGLE CLOUD PLATFORM (JWT)
                                  (V01.04.02)
            ______________________________________________________


Table of Contents
_________________

1 Purpose of the Example
2 Steps for Preparation
.. 2.1 Generating the Keys on local PC
.. 2.2 Downloading Keys on the A71CH
.. 2.3 Preparing the Cloud
..... 2.3.1 Updating cloud over command line
..... 2.3.2 Updating cloud using the Web Interface
.. 2.4 Preparing the Example
3 Running the Demo
4 Future Improvements



1 Purpose of the Example

  Google Cloud Platform (JWT) example uses A71CH to securely connect to GCP
  Device.

  This example connects to time.google.com, gets current time stamp, creates a
  JWT Token and connect to server as configured in Section-2.4

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Prepare the Demo
================

2 Steps for Preparation

2.1 Generating the Keys on local PC
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Prepare the keys on PC using OpenSSL.

  Run hostLib\mbedtls\scripts\RunOnce_CreateCertificates.bat

  This will create keys in hostLib\mbedtls\ecc folder.

  (This is only needed once per installation on the client PC.)


2.2 Downloading Keys on the A71CH
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  1) Run frdmk64f_projects\vcomA71CH Kinetis Example on FRDM K64F Board

  2) Connect the USB Port of K64F-USB to PC.  Note down the COM Port name on PC
     from device manager.

  3) Run hostLib\mbedtls\scripts\ResetAndUpdateA71CH.bat <COM PORT NAME>

     This will reset the device and insert keys and certificates from the
     hostLib\mbedtls\ecc to the A71CH.


2.3 Preparing the Cloud
~~~~~~~~~~~~~~~~~~~~~~~

2.3.1 Updating cloud over command line
--------------------------------------

  Provided the user already has ``gcooud`` command line utility setup and
  assuming the project name is ``pgh-cloud-iot`` the following commands sets up
  the code.

  Create a new events pub/sub topic::

  ,----
  | gcloud pubsub topics create a71ch-demo-events \
  |     --project=pgh-cloud-iot
  `----

  Create a registry:

  ,----
  | gcloud iot registries create a71ch-demo-reg \
  |     --project=pgh-cloud-iot \
  |     --region=us-central1 \
  |     --event-notification-config=topic=projects/pgh-cloud-iot/topics/a71ch-demo-events \
  `----

  Create a device and attach the certificate ``tls_client.cer``:


  ,----
  | gcloud iot devices create a71ch-dev-01 \
  |     --project=pgh-cloud-iot \
  |     --region=us-central1 \
  |     --registry=a71ch-demo-reg \
  |     --public-key=path=hostLib/mbedtls/ecc/tls_client.cer,type=es256-pem
  `----


2.3.2 Updating cloud using the Web Interface
--------------------------------------------

  1) Sign up for Google Cloud Platform - IoT (If you have not done that
     already)

  2) Create Registry & Device in the cloud platform.

  3) Copy For the device, add public key in ES256_X509 format.

     Copy hostLib\mbedtls\ecc\tls_client.cer and paste in the web-dialogue box.


2.4 Preparing the Example
~~~~~~~~~~~~~~~~~~~~~~~~~

  1) Update demo\gcp_jwt\gcp_iot_config.h

     GCP_PROJECT_NAME, GCP_LOCATION_NAME, GCP_REGISTRY_NAME, GCP_DEVICE_NAME

     e.g. For Device
     [https://console.cloud.google.com/iot/locations/us-central1/registries/reg-demo-01/devices/nxp-ew-gcp-demo01?project=pgh-cloud-iot],
     the values are as follows.

     - GCP_PROJECT_NAME : pgh-cloud-iot
     - GCP_LOCATION_NAME : us-central1
     - GCP_REGISTRY_NAME : a71ch-demo-reg
     - GCP_DEVICE_NAME : a71ch-dev-01

Running the demo
================

  If everything is setup correctly as mentioned in Section-2.2, Section-2.3 and
  Section-2.4, the output would be as follows::

  ,----
  | GCP JWT A71CH example
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
  `----

  You can update device config with following messages to toggle on-board keys.


  Using the below command, we can toggle LEDs::

  ,----
  | gcloud iot devices configs update \
  |     --project=pgh-cloud-iot \
  |     --region=us-central1 \
  |     --registry=a71ch-demo-reg \
  |     --device=a71ch-dev-01 \
  |     --config-data='{"red": "off"}'
  `----

  ,----
  | {"green": "toggle", "user": "test1"}
  | {"green": "on",     "user": "test1"}
  | {"red":   "off",    "user": "test1"}
  `----


4 Future Improvements

  Use the platform clock/timers to detected that re-connection is needed.

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

