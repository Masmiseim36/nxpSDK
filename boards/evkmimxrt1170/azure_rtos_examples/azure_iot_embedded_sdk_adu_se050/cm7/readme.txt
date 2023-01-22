Overview
========
This example showcases the Azure device update (ADU) feature. It connects to Azure IoT Hub and starts interacting with the service,
Device Update for IoT Hubs. When the example is running, it will report the device status, and fetch the device update information.

In this example, the device credential will be stored in a secure element chip, SE050. 

Prerequisites

SE050 Provisioning

  1. Download EdgeLock SE05x Plug & Trust Middleware (04.01.01) at:
     https://www.nxp.com/products/security-and-authentication/authentication/edgelock-se050-development-kits:OM-SE050X
     Unzip the zip file, and enter the simw-top directory. 
  
  2. Please refer to Chapter 9 CLI TOOL in AN13030_Plug_Trust_MW_Documentation_1.7.pdf to install the ssscli tool.
  
  3. Install the SE050 board on the Arduino interface of the development board (e.g. evkmimxrt1060).
  
  4. In binaries/MCU/se05x/, find the vcom binary image and write it into the develop board. For evkmimxrt1060, the binary image
     is se05x_vcom-T1oI2C-evkmimxrt1060.bin. This program makes the development board as a USB device. Thus, we can use the ssscli tool
     to write device credential on SE050.
  
  5. Connect the USB OTG port on the the development board to PC. Then, generate CA, device private key and certificate on PC:
  
     > cd Provisioning/
     > python GenerateAZURECredentials.py COM5
  
     This will generate files, like:
        rootCA_key.pem
        rootCA_certificate.cer
        377825058786114158113369_device_key.pem
        377825058786114158113369_device_certificate.cer
  
     Write the device private key and certificate into the SE050 chip:
  
     > python ResetAndUpdate_AZURE.py COM5
  
     These are the corresponding key IDs in the SE050 chip.
  
      Injecting ECC key pair at key ID: 0x223344
      Injecting certificate at key ID: 0x223345
  
  6 We need the device certificate thumbprint to create a device in Azure IoT hub. This command can get it:
  
    > openssl x509 -in 377825058786114158113369_device_certificate.cer -noout -fingerprint | tr -d ':'
    57BCA176DEC6E0C8E7F7354620F86E0FB1D593B0
  
    The result will be used in the following step, "Create a new device identity". Put it in the place of {MyCertThumbprint}.


Azure IoT Hub Setup

  Before running the example, need to set up a device in Azure IoT Hub and set up the Device Update for IoT Hubs service.
  
  Here, we demonstrate how to setup a device in Azure IoT Hub. If you are not familiar with Azure CLI, please refer to
  the document for the details. (https://learn.microsoft.com/en-us/azure/iot-hub/)
  
  Note that these steps assume you use the Azure IoT Hub for the first time. The file directory is relative to the root
  directory of MCU SDK package.

    1. Register an Azure account.
    
    2. Install Azure CLI locally, or use Azure Cloud Shell.
    
    3. Before using any CLI commands locally, you need to sign in:
         > az login
    
    4. Add the Microsoft Azure IoT Extension for Azure CLI.
         > az extension add --name azure-iot
    
    5. Create a new resource group which is a logical container into which Azure IoT Hub are deployed and managed.
       {MyResourceGroupName}: Name of the new resource group
       {MyResourceLocation}: Location, for example, westus. Select a location from: az account list-locations -o table.
         > az group create --name {MyResourceGroupName} --location {MyResourceLocation}
    
    6. Create a new IoT Hub in the resource group.
       {MyResourceGroupName}: The name of the resource group you just created.
       {MyIoTHubName}: Name of the new IoT Hub. This name must be globally unique. If failed, please try another name.
         > az iot hub create --resource-group {MyResourceGroupName} --name {MyIoTHubName}
    
    7. Create a new device identity in the Hub IoT.
       {MyIoTHubName}: Name of the IoT Hub just created
       {MyDeviceId}: Name of the new device
       {MyCertThumbprint}: the thumbprint of the device certificate generated in the SE050 provisioning
         > az iot hub device-identity create --hub-name {MyIoTHubName} --device-id {MyDeviceId} --am x509_thumbprint --ptp {MyCertThumbprint}
    
    8. Add the tag ADUGroup in the Device twin page. For example, if the name of your device group is "adu_group", add it like:
    
        "tags": {
                "ADUGroup": "adu_group"
            },
    
       Note that do not forget to save. Please refer to
         https://learn.microsoft.com/en-us/azure/iot-hub-device-update/create-update-group

    9. In the web page of Azure portal, create a new service, Device Update for IoT Hubs.
       Please refer to https://learn.microsoft.com/en-us/azure/iot-hub-device-update/create-device-update-account
    

Local Board Setup

    1. Install the bootloader, mcuboot_opensource, which is in the SDK directory, e.g. boards/evkmimxrt1060/mcuboot_opensource/.
       This bootloader will work with the azure_iot_embedded_sdk_adu_se050 example to implement the Azure Device Update feature. 
       Build the bootloader image and write it into flash.
    
    2. Install a python program, imgtool, which is used for processing the raw image in the following steps:
    
        > pip3 install imgtool==1.9.0
    
    3. Build an initial image. Enter the ADU example directory, boards/evkmimxrt1060/azure_rtos_examples/azure_iot_embedded_sdk_adu_se050/.
       Change the value of these macros in sample_config.h on demand:

        HOST_NAME  "<put {MyIoTHubName} here>"
        DEVICE_ID  "<put {MyDeviceId} here>"
    
        SAMPLE_DEVICE_MANUFACTURER
        SAMPLE_DEVICE_MODEL
        SAMPLE_DEVICE_FIRMWARE_VERSION
    
       Note that the content of SAMPLE_UPDATE_ID_PROVIDER and SAMPLE_UPDATE_ID_NAME must only contain these ASCII characters:
         a-z, A-Z, 0-9, dot(.), dash(-). The length is less than or equal to 64.
    
       Build the example and get a binary image named azure_iot_embedded_sdk_adu_se050.bin. 
    
    4. Sign the image and write it into the flash. Use the script sign_image.py in the scripts directory to sign the image, for example:
    
        > cd scripts
        > python sign_image.py -k ../keys/sign-rsa2048-priv.pem -v "1.0.0" -i ../armgcc/flexspi_nor_release/azure_iot_embedded_sdk_adu_se050.bin -f
    
       Note the -f parameter, it means generating an image for the first boot. It will generate an flash image file, like
       azure_iot_embedded_sdk_adu_se050.1.0.0.signed.1667447086.boot.bin. Then, write the signed image into the flash at the address of
       the first slot. The flash address depends on a specific board. It is defined in the macro BOOT_FLASH_ACT_APP in
       mcuboot/flash_partitioning.h. On evkmimxrt1060, it's 0x60040000.
    
       Note that this key, ./keys/sign-rsa2048-priv.pem, for signing images is the same as the one in the mcuboot_opensource project.
       It's just for test. 
    
    5. After write the initial image into flash, boot up the device. See if it can connect to Azure IoT hub successfully. If failed, please
       check if NTP is successful, if the Azure IoT hub server can be accessed in your network.
    

Device Update Process

    1. Build an update image which has a greater version than the initial image. First, increase the version number
       in the macro SAMPLE_DEVICE_FIRMWARE_VERSION in sample_config.h, for example, changing from 1.0.0 to 1.1.0. Then,
       build the azure_iot_embedded_sdk_adu_se050 example again.
    
    2. To create an update image that can be imported in the Azure IoT hub, need to sign the update image and generate
       a manifest file for it. To do this, first execute the script sign_image.py.
    
        > cd scripts
        > python sign_image.py -k ../keys/sign-rsa2048-priv.pem -v "1.1.0" -i ../armgcc/flexspi_nor_release/azure_iot_embedded_sdk_adu_se050.bin
    
       It will generate a signed image, like azure_iot_embedded_sdk_adu_se050.1.1.0.signed.1667547086.bin.
    
    3. Generate a manifest file using Azure CLI. If the macros in sample_config.h have been changed, please change the constants
       in the header of the script gen_manifest.py accordinlgy. Then, input this command:
    
        > python gen_manifest.py -v "1.1.0" -i azure_iot_embedded_sdk_adu.1.1.0.signed.1667547086.bin
    
    4. Import the update files including a signed image and a manifest json file. In the left panel of Azure IoT Hub, find "Updates"
       in "Device management" and click it. In the Updates tab page, click the button "Import a new update". Then, select a storage container,
       upload these files, one image file and one json file. This creates a new update entry in the Updates tab page.
       Please refer to https://docs.microsoft.com/en-us/azure/iot-hub-device-update/import-update
    
    5. Deploy an update image. Enter the "Groups and Deployments" tab page and click the group name which is the same as the tag value of ADUGroup.
       In the Group details page, enter the Current updates tab. On the right side of the page, you will see a deploy/redeploy link beside the version
       you just imported. Click it to start deployment. Please refer to https://docs.microsoft.com/en-us/azure/iot-hub-device-update/deploy-update.
       Wait for a moment, the device will receive the update message, and print, for example:
    
         Received new update: Provider: NXP; Name: MIMXRT1060, Version: 1.1.0
    
       The update process will start automatically. After writing the update image into flash, we can reset the device to start up
       the new updated image.


Time Sync Problem

   If an error message as below appears in the serial output, it's because the board cannot connect to the NTP server.
   If the time on the device is not up to date, it causes device authentication failure.
   
       SNTP Time Sync failed.
   
   To resolve it, one method is to replace SAMPLE_SNTP_SERVER_NAME in the main.c file with an available NTP server.

   Another method just for test is to update the default Epoch time in the SAMPLE_SYSTEM_TIME macro in the main.c file.
   On Linux, use the command to get the current Epoch time, for example:
   
       $ date +%s
       1610343882
   
   On Windows 10, use the following command in PowerShell to get the current Epoch time, for example:
   
       PS C:\Users> (New-TimeSpan -Start (Get-Date 01/01/1970) -End (Get-Date)).TotalSeconds
       1610343882.02736
   
   Update the value of SAMPLE_SYSTEM_TIME to the current Epoch time, for example, 1610343882. Then, rebuild the project.



Toolchain supported
===================
- IAR embedded Workbench  9.32.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1170-EVK board
- OM-SE050ARD-E: EdgeLock SE050 Arduino Compatible Development Kits
- Personal Computer

Board settings
==============
Install the OM-SE050ARD-E on the Arduino interface of the board MIMXRT1170-EVK.
use the default jumper setting.

On MIMXRT1170-EVK REVC board, GPIO_AD_32 uses as ENET_MDC in this example which is muxed with the SD1_CD_B,
please check the R1926 and R136 connected to SD1_CD_B. If they are populated with resistor and SD card is
inserted, this time enet can't access PHY.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  This example uses 1G port by default. If using the 100M ENET port, set the macro
    BOARD_NETWORK_USE_100M_ENET_PORT in board.h to 1.
4.  Compile the demo.
5.  Download the program to the target board.
6.  Insert an Ethernet cable to the default Ethernet RJ45 port labelled "1G ENET" and connect it to
    an Ethernet switch. Note that if set BOARD_NETWORK_USE_100M_ENET_PORT, the RJ45 port should be
    the one labelled "100M ENET".
7.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the device update is running, the serial port will output, for example:


hello sbl.
Disabling flash remapping function
Bootloader Version 1.9.0
Primary   slot: version=1.0.0+0
Secondary slot: Image not found
Found a candidate in slot 1
writing copy_done; fa_id=1 off=0x1fffe0 (0x43ffe0)
Image 0 loaded from the primary slot
Bootloader chainload address offset: 0x40000
Reset_Handler address offset: 0x40400
Jumping to the image


Booting the primary slot - flash remapping is disabled
Start the azure_iot_embedded_sdk_adu_se050 example
ex_sss_boot_open()...
sss   :INFO :atr (Len=35)
		01 A0 00 00 	03 96 04 03 	E8 00 FE 02 	0B 03 E8 00 
		01 00 00 00 	00 64 13 88 	0A 00 65 53 	45 30 35 31 
		00 00 00 
sss   :WARN :Communication channel is Plain.
sss   :WARN :!!!Not recommended for production use.!!!
call ex_sss_key_store_and_object_init()...
call se05x_GetCertificate()...
DHCP In Progress...
IP address: 10.193.20.181
Mask: 255.255.255.0
Gateway: 10.193.20.254
DNS Server address: 165.114.89.4
SNTP Time Sync...0.pool.ntp.org
SNTP Time Sync successfully.
[INFO] Azure IoT Security Module has been enabled, status=0
IoTHub Host Name: adu.azure-devices.net; Device ID: adu_test2.
ex_sss_boot_open()...
sss   :INFO :atr (Len=35)
		01 A0 00 00 	03 96 04 03 	E8 00 FE 02 	0B 03 E8 00 
		01 00 00 00 	00 64 13 88 	0A 00 65 53 	45 30 35 31 
		00 00 00 
sss   :WARN :Communication channel is Plain.
sss   :WARN :!!!Not recommended for production use.!!!
call ex_sss_key_store_and_object_init()...
Connected to IoTHub.
[INFO] ADU agent started successfully!
Device Properties: manufacturer: NXP, model: MIMXRT1xxx, installed criteria: 1.0.0.
Sent properties request.
Telemetry message send: {"temperature":22}.
[INFO] Cancel Command received
Received all properties
[INFO] Azure IoT Security Module message is empty
Telemetry message send: {"temperature":22}.
Telemetry message send: {"temperature":22}.
Telemetry message send: {"temperature":22}.
Received new update: Provider: NXP; Name: MIMXRT1xxx, Version: 1.1.0
Received writable property
[INFO] Updating firmware...
[INFO] Manufacturer: NXP
[INFO] Model: MIMXRT1xxx
[INFO] Firmware downloading...
firmware size: 0x3e318
Erase the update partition (start: 0x240000, size: 0x200000)
Telemetry message send: {"temperature":22}.
[INFO] Getting download data... 539
[INFO] Getting download data... 1899
[INFO] Getting download data... 3259
[INFO] Getting download data... 4619
[INFO] Getting download data... 5979
[INFO] Getting download data... 7339
[INFO] Getting download data... 8699
[INFO] Getting download data... 10059
[INFO] Getting download data... 11419
[INFO] Getting download data... 12779
[INFO] Getting download data... 14139
[INFO] Getting download data... 15499
[INFO] Getting download data... 16859
[INFO] Getting download data... 16923
[INFO] Getting download data... 18219
[INFO] Getting download data... 19579
[INFO] Getting download data... 20939
[INFO] Getting download data... 22299
[INFO] Getting download data... 23659
[INFO] Getting download data... 25019
[INFO] Getting download data... 26379
[INFO] Getting download data... 27739
[INFO] Getting download data... 29099
[INFO] Getting download data... 30459
[INFO] Getting download data... 31819
[INFO] Getting download data... 33179
[INFO] Getting download data... 34539
[INFO] Getting download data... 35899
[INFO] Getting download data... 37259
[INFO] Getting download data... 38619
[INFO] Getting download data... 39979
[INFO] Getting download data... 41339
[INFO] Getting download data... 42699
[INFO] Getting download data... 44059
[INFO] Getting download data... 45419
[INFO] Getting download data... 46779
[INFO] Getting download data... 48139
[INFO] Getting download data... 49499
[INFO] Getting download data... 50859
[INFO] Getting download data... 52219
[INFO] Getting download data... 53579
[INFO] Getting download data... 54939
[INFO] Getting download data... 56299
[INFO] Getting download data... 57659
[INFO] Getting download data... 57723
[INFO] Getting download data... 59019
[INFO] Getting download data... 60379
[INFO] Getting download data... 61739
[INFO] Getting download data... 63099
[INFO] Getting download data... 64459
[INFO] Getting download data... 65819
[INFO] Getting download data... 67179
[INFO] Getting download data... 68539
[INFO] Getting download data... 69899
[INFO] Getting download data... 71259
[INFO] Getting download data... 72619
[INFO] Getting download data... 73979
[INFO] Getting download data... 74043
[INFO] Getting download data... 75339
[INFO] Getting download data... 76699
[INFO] Getting download data... 78059
[INFO] Getting download data... 79419
[INFO] Getting download data... 80779
[INFO] Getting download data... 82139
[INFO] Getting download data... 83499
[INFO] Getting download data... 84859
[INFO] Getting download data... 84923
[INFO] Getting download data... 86283
[INFO] Getting download data... 87643
[INFO] Getting download data... 89003
[INFO] Getting download data... 90363
[INFO] Getting download data... 91659
[INFO] Getting download data... 93019
[INFO] Getting download data... 94379
[INFO] Getting download data... 95739
[INFO] Getting download data... 97099
[INFO] Getting download data... 98459
[INFO] Getting download data... 99819
[INFO] Getting download data... 101179
[INFO] Getting download data... 102539
[INFO] Getting download data... 103899
[INFO] Getting download data... 105259
[INFO] Getting download data... 106619
[INFO] Getting download data... 107979
[INFO] Getting download data... 109339
[INFO] Getting download data... 110699
[INFO] Getting download data... 112059
[INFO] Getting download data... 113419
[INFO] Getting download data... 114779
[INFO] Getting download data... 116139
[INFO] Getting download data... 117499
[INFO] Getting download data... 118859
[INFO] Getting download data... 120219
[INFO] Getting download data... 120283
[INFO] Getting download data... 121643
[INFO] Getting download data... 123003
[INFO] Getting download data... 124299
[INFO] Getting download data... 125659
[INFO] Getting download data... 127019
[INFO] Getting download data... 128379
[INFO] Getting download data... 129739
[INFO] Getting download data... 131099
[INFO] Getting download data... 131163
[INFO] Getting download data... 132459
[INFO] Getting download data... 133819
[INFO] Getting download data... 135179
[INFO] Getting download data... 136539
[INFO] Getting download data... 137899
[INFO] Getting download data... 139259
[INFO] Getting download data... 140619
[INFO] Getting download data... 141979
[INFO] Getting download data... 142043
[INFO] Getting download data... 143403
[INFO] Getting download data... 144763
[INFO] Getting download data... 146123
[INFO] Getting download data... 147483
[INFO] Getting download data... 148779
[INFO] Getting download data... 150139
[INFO] Getting download data... 151499
[INFO] Getting download data... 152859
[INFO] Getting download data... 154219
[INFO] Getting download data... 155579
[INFO] Getting download data... 156939
[INFO] Getting download data... 158299
[INFO] Getting download data... 159659
[INFO] Getting download data... 161019
[INFO] Getting download data... 162379
[INFO] Getting download data... 163739
[INFO] Getting download data... 163867
[INFO] Getting download data... 165099
[INFO] Getting download data... 166459
[INFO] Getting download data... 167819
[INFO] Getting download data... 169179
[INFO] Getting download data... 170539
[INFO] Getting download data... 171899
[INFO] Getting download data... 171963
[INFO] Getting download data... 173259
[INFO] Getting download data... 174619
[INFO] Getting download data... 175979
[INFO] Getting download data... 177339
[INFO] Getting download data... 178699
[INFO] Getting download data... 180059
[INFO] Getting download data... 181419
[INFO] Getting download data... 182779
[INFO] Getting download data... 184139
[INFO] Getting download data... 185499
[INFO] Getting download data... 186859
[INFO] Getting download data... 188219
[INFO] Getting download data... 189579
[INFO] Getting download data... 190939
[INFO] Getting download data... 192299
[INFO] Getting download data... 193659
[INFO] Getting download data... 195019
[INFO] Getting download data... 196379
[INFO] Getting download data... 197739
[INFO] Getting download data... 199099
[INFO] Getting download data... 200459
[INFO] Getting download data... 201819
[INFO] Getting download data... 203179
[INFO] Getting download data... 204539
[INFO] Getting download data... 205899
[INFO] Getting download data... 207259
[INFO] Getting download data... 208619
[INFO] Getting download data... 209979
[INFO] Getting download data... 211339
[INFO] Getting download data... 212699
[INFO] Getting download data... 214059
[INFO] Getting download data... 215419
[INFO] Getting download data... 216779
[INFO] Getting download data... 218139
[INFO] Getting download data... 219499
[INFO] Getting download data... 220859
[INFO] Getting download data... 220923
[INFO] Getting download data... 222219
[INFO] Getting download data... 223579
[INFO] Getting download data... 224939
[INFO] Getting download data... 226299
[INFO] Getting download data... 227659
[INFO] Getting download data... 229019
[INFO] Getting download data... 230379
[INFO] Getting download data... 231739
[INFO] Getting download data... 233099
[INFO] Getting download data... 234459
[INFO] Getting download data... 234523
[INFO] Getting download data... 235883
[INFO] Getting download data... 237243
[INFO] Getting download data... 238539
[INFO] Getting download data... 239899
[INFO] Getting download data... 241259
[INFO] Getting download data... 242619
[INFO] Getting download data... 243979
[INFO] Getting download data... 245339
[INFO] Getting download data... 245403
[INFO] Getting download data... 246699
[INFO] Getting download data... 248059
[INFO] Getting download data... 249419
[INFO] Getting download data... 250779
[INFO] Getting download data... 252139
[INFO] Getting download data... 253499
[INFO] Getting download data... 254744
[INFO] Firmware downloaded
[INFO] Firmware installing...
write magic number offset = 0x23ff00
[INFO] Firmware installed
[INFO] Applying firmware...
[INFO] Manufacturer: NXP
[INFO] Model: MIMXRT1xxx
[INFO] Firmware applying...
[INFO] Firmware applied


Telemetry message send: {"temperature":22}.
Telemetry message send: {"temperature":22}.
Telemetry message send: {"temperature":22}.

