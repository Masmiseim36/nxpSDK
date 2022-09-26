Overview
========
The example to demonstrate the usage of Bluetooth BT/BLE profiles.
The example provides a platform for end system designers to quickly evaluate 
EtherMind Bluetooth Software Protocol Stack and Profiles, and implement 
applications using it. The reference applications are included in the kit 
help implementers to quickly develop customized Bluetooth applications to 
typically validate on FreeRTOS.
This document provides step-by-step procedures to build and test the example, 
and also instructions for running the included sample applications.
NOTE.
To avoid the potential noise running A2DP source/sink or HFP function. 
APPL_LIMIT_LOGS might need to be enabled in the project setting.
Note: example rebuilding is necessary to make the APPL_LIMIT_LOGS take effect.
To make sure the NVM area have clean, all flash sector need be erased before 
download example code.


Toolchain supported
===================
- MCUXpresso  11.6.0
- IAR embedded Workbench  9.30.1
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
- Personal Computer
- One of the following WiFi modules:
  - AzureWave AW-AM457-uSD

Board settings
==============
Jumper settings for RT1060:
remove  J1 5-6
connect J1 1-2
connect J2 with external power(controlled by SW1)

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt1060 and AW-AM457-uSD.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
UART_RXD |  J10(pin 4)  |   J22(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_TXD |  J10(pin 2)  |   J22(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_CTS |  J10(pin 8)  |   J23(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_RTS |  J10(pin 6)  |   J23(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J25(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------

The pin connect for PCM interface as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
PCM_IN   |  J9(pin 1)   |   J22(pin 5)   |    SAI2_TXD        | GPIO_AD_B0_09
PCM_OUT  |  J9(pin 2)   |   TP11         |    SAI2_RXD        | GPIO_AD_B0_08   
PCM_SYNC |  J9(pin 3)   |   J21(pin 9)   |    SAI2_RX_SYNC    | GPIO_AD_B0_07
PCM_CLK  |  J9(pin 4)   |   J21(pin 7)   |    SAI2_RX_BCLK    | GPIO_AD_B0_06
GND      |  J9(pin 6)   |   J21(pin 20)  |    GND             | GND
------------------------------------------------------------------------------------
Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW9 or power off and on the board to run the application.

Please remove J47 on RT1060 EVK while testing HFP

Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Download the program to the target board.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

 1. When the demo starts, the following message about the demo would appear on the Terminal.
       ETHERMIND Menu Application

 2. First, the demo will load the WIFI+BT Module's firmware through the SDIO Interface.
    Once the firmware is successfully Initialized and Loaded, the demo will print the WIFI Mac address
    on the terminal as shown below
     Initialize AW-AM457-uSD Driver
AW-AM457-uSD MAC Address: 70:66:55:E5:50:79

 3. After the WIFI+BT module initialization and firmware download, the following demo application menu
    will be shown on the terminal

   0.  Exit.
   1.  Refresh this Menu.

   2.  EtherMind Init.
   3.  Bluetooth ON.
   4.  Bluetooth OFF.

   5.  Set PIN Code.
   6.  Set Device Role.

   7.  Get Free ACL Tx Queue Buffer.
   8.  Set LWM/HWM for ACL Tx Queue.

   11. HCI Operations.
   12. SDP Operations.
   13. SDDB Operations.
   14. RFCOMM Operations.
   15. MCAP Operations.
   16. BNEP Operations.
   17. AVDTP Operations.
   18. AVCTP Operations.
   19. OBEX Client Operations.
   20. OBEX Server Operations.
   21. L2CAP Operations.

   25. Write Storage event mask.

   30. Security Manager.
   31. LE SMP.
   32. LE L2CAP.
   33. ATT.

   40. SPP Operations.
   41. HFP Unit Operations.
   42. HFP AG Operations.
   43. HSP Unit Operations.
   44. HSP AG Operations.
   45. DUNP DT Operations.
   46. DUNP GW Operations.
   47. SAP Client Operations.
   48. SAP Server Operations.

   50. OPP Client Operations.
   51. OPP Server Operations.
   52. FTP Client Operations.
   53. FTP Server Operations.
   54. MAP Client Operations.
   55. MAP Server Operations.
   56. PBAP Client Operations.
   57. PBAP Server Operations.
   58. CTN Client Operations.
   59. CTN Server Operations.
   60. BIP Initiator Operations.
   61. BIP Responder Operations.
   62. SYNCP Client Operations.
   63. SYNCP Server Operations.

   65. A2DP Operations.
   66. AVRCP Operations.
   67. HDP Operations.
   68. PAN Operations.

   70. HID Device Operations.
   71. HID Host Operations.

   75. DID Client Operations.
   76. DID Server Operations.

   80. GATT Client Operations.
   81. GATT Server Operations.

   90. BPP Sender Operations.
   91. BPP Printer Operations.

Your Option ?

 4. Select the option "2(EtherMind Init)" from the above menu to Initialize the EtherMind Module for BT Operations.

 5. After this, wait for above menu to be prompted once again. Also look for the prints
    Initializing EtherMind ...
    EtherMind Stack Successfully Initialized!
    <TODO:> Add the "EtherMind Stack Successfully Initialized!" in the code.

 6. Now, select the option "3(Bluetooth ON)" from the above menu to perform the Bluetooth Initialization process
    which establishes a communication between the EtherMind Host Stack and the WIFI+BT Module.
    The following sequence of messages should be seen on the terminal

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C03 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C01 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C63 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x2001 -> HCI_LE_SET_EVENT_MASK_OPCODE
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C56 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x1009 -> UNKNOWN
        Command Status: 0x00
        Return Parameters:
-- Dumping 6 Bytes --
-------------------------------------------------------------------
78 50 E5 55 66 70                                  xP.Ufp
-------------------------------------------------------------------

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x2002 -> HCI_LE_READ_BUFFER_SIZE_OPCODE
        Command Status: 0x00
        Return Parameters:
-- Dumping 3 Bytes --
-------------------------------------------------------------------
00 01 08                                           ...
-------------------------------------------------------------------

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x1005 -> UNKNOWN
        Command Status: 0x00
        Return Parameters:
-- Dumping 7 Bytes --
-------------------------------------------------------------------
FD 03 3C 07 00 0C 00                               ..<....
-------------------------------------------------------------------

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C13 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C18 -> UNKNOWN
        Command Status: 0x00
Bluetooth ON Initialization Completed.
Stack Version - 016.000.000

 7. After the "Bluetooth ON Initialization Completed" is successfully completed. The Demo application
    automatically sets the BLE Advertisement Parameters, Data and enables Advertisements for a sample
    BLE service. The following prints should be seen on the terminal

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0C6D -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x2008 -> HCI_LE_SET_ADVERTISING_DATA_OPCODE
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x2006 -> HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x200A -> HCI_LE_SET_ADVERTISING_ENABLE_OPCODE
        Command Status: 0x00
Enabled Advertising...
EtherMind Server is now Ready

 8. Now, Initialize and Start any required profile from the corresponding menu option. At this point, the stack is ready to accept incoming
    connections from any peer device (PTS/Mobile/EtherMind application/PC Stack) for the started profile.

   8.0 Switch the peer device when have multiple connections

      When central device connected to multiple devices, the default peer device will set to newest. 
      For example, a device's HTS and PXM may implement at the same time.
      First, PXM connect to PXR device, then a Phone connect to HTS. 
      Now the default peer device is Phone, the PXM functions like write/read will not work because current ATT Handle is for Phone.
      In order to change peer device to make PXM work again, the following process must be used:

      1). Input 80 in "MAIN MENU" into "GATT CLIENT MENU". 
      2). Input 2  in "GATT CLIENT MENU" into "Register GATT Handle" options, there will be a devices list.

            ***************************************
            List of Current Connected devices:
            ***************************************

            1. Peer with - 
                 ->         APPL_HANDLE: 1
                 ->   Connection Handle: 0x0080
                 ->                ADDR: 4E 7F 38 37 60 00, TYPE: 00
                 ->         ATT Handle : DEV_ID - 0x0C ATT_ID - 0x01
            2. Peer with - 
                 ->         APPL_HANDLE: 0
                 ->   Connection Handle: 0x0081
                 ->                ADDR: 90 AE 72 68 BD 62, TYPE: 01
                 ->         ATT Handle : DEV_ID - 0x0B ATT_ID - 0x00
            Enter Device ID: 0C
            Enter ATT ID: 01

      3). Distinguish peer device by MAC address, then input target peer device's "Device ID" and "ATT ID".
          Now the peer device have changed to target.

   8.1 Running Sample SPP Application
      a) Running Serial Port Profile - Server
         1). Setup and prepare the application.
             - Enter option 2 (EtherMind Init) in Main menu.
             - Enter option 3 (Bluetooth ON) in Main menu.
         2). Setup the SPP Unit profile
             - Enter option 40 (SPP Operations) in Main menu.
             - Enter option 3 (SPP Init) in SPP menu.
             - Enter option 4 (Start SPP) in SPP menu.
             - Enter option 5 (Start SPP Vendor Service) in SPP menu (Optional)

             Notice: Now, SPP server is started and ready to accept incoming SPP connections from clients.

      b) Running Serial Port Profile - Client
         1). Setup and prepare the application.
             - Enter option 2 (EtherMind Init) in Main menu.
             - Enter option 3 (Bluetooth ON) in Main menu.
         2). Setup the SPP Unit profile
             - Enter option 40 (SPP Operations) in Main menu.
             - Enter option 3 (SPP Init) in SPP menu.
             - Enter option 4 (Start SPP) in SPP menu.
             - Enter option 2 (Set BD_ADDR of Peer SPP) in SPP menu.
               > Enter the peer BD Address (in LSB to MSB order separated by space. Eg: 0x001BDCE0384C is the peer address,enter as "4C 38 E0 DC 1B 00")
             - Enter option 10 (Create ACL Connection) in SPP menu
             - Enter option 11 (Get SPP SDP Record) in SPP menu.
             - Enter option 20 (Connect to SPP) in SPP menu.

             Notice: Now, SPP connection is created.

      c) After SPP connection is created, the following steps can be performed on server or client
         1). Send data to peer device
             - Enter option 30 (Send data over SPP) in SPP menu.
               > Enter the Sample Strings index
         2). Save data received over the connected SPP channel in a file
             - Connect U-disk to RT1060 board's J9 with OTG
             - Enter option 50 (Receive file) in SPP menu
               > Enter file name
               > Now received data will be saved in created file. 
             - Enter option 51 (Stop Receiving file) in SPP menu to stop receive data
         3). Disconnect from peer SPP
             - Enter option 21 (Disconnect from Peer SPP) in SPP menu.
   8.2 Running Sample HFP Application
      a) Running Hands Free Profile Audio Gateway
         1). Refer to main menu 1-7 steps to setup and prepare the application.
         2). Setup the HFP AG profile
             - Enter option 42 (HFP AG Operations) in Main menu.
             - Enter option 2 (Initialize) in HFP AG menu. Note for successful profile initialization
             - Enter option 3 (Start) in HFP AG menu
               > Enter Local supported features -> 16383 (All feature selection)
             - Note for successful profile startup.
         3). In order to connect to a peer HFP unit
             - Enter option 10 in HFP AG menu, register the peer HFP Unit BD Address.
             - Enter option 12 to connect to the peer Unit.
             - After Successful HCI_CONNECTION_COMPLETE_EVENT with Success (0x00) status, enter option 11 to  get the Unit server channel.
             - Enter option 4 to initiate the HFP SLC procedure. The following kind of prints should be printed on console.
         4). Notify AG indicators to Unit
             - Enter option 60 to configure and notify Inband ringtone setting
             - Enter option 62 to configure and notify of the HF indicators
         5). Gain Control
             - Enter option 63 to configure and notify of the Speaker gain
             - Enter option 64 to configure and notify of the Microphone gain
         6). Voice Recognition
             - Enter option 43 to enable voice recognition notification to HFP Unit
             - Enter option 44 to disable voice recognition notification to HFP Unit
             - Enter option 43 to notify enhanced voice recognition indications to HFP Unit
         7). Call Handling
             - Use the menu option 100 to manage the call and network settings with the simple network simulator
         8). Audio Transfer
             - Enter option 14 to transfer the voice audio when in call to the HFP Unit.
             - Enter option 15 to transfer the voice audio when in call to the HFP AG.    
      b) Running Hands Free Profile Unit
         1). Refer to main menu 1-7 steps to setup and prepare the application.
         2). Setup the HFP Unit profile
             - Enter option 41 (HFP unit Operations) in Main menu.
             - Enter option 4 (Initialize) in HFP Unit menu. Note for successful profile initialization
             - Enter option 5 (Start) in HFP Unit menu
               > Enter Local supported features; 1023
               > Enter available codecs; 1 or 1,2
               > Enter available HF indicators; 1, 2 or 1,2
             - Note for successful profile startup.
         3). Discover the device from a peer mobile phone and connect to it. Make sure should be displayed on the terminal.
         4). Enable/Disable HFP features
             - Enter options 50/51 to enable/disable Caller Line Identification Notification (CLIP)
             - Enter options 52/53 to enable/disable Call Waiting Notification (CCWA)
             - Enter options 54/55 to enable/disable Voice Recognition (BVRA)
             - Enter options 56 to disable ECNR in AG
         5). Gain Control
             - Enter option 60 to set the speaker gain level
             - Enter option 61 to set the microphone gain level
             - Enter option 62 to get the current gain level configured. 
               - Note that upon establishing a new service level connection, this option might return an incorrect value.
                 The application should set the gain using the above options after every new connection before trying to
                 fetch the current gain level.
         6). Incoming Call
             - Enter option 40 to accept an incoming call
             - Enter option 41 to reject an incoming call
         7). Outgoing Call
             - Enter option 30 to dial to a given number
             - Enter option 31 to dial to a number in a given memory location in AG
             - Enter option 32 to redial the last call on the AG
         8). Active Call
             - Enter option 42 to hangup an active call
         9). Three-Way Call 
             - Enter option 45->0 to release all held calls or Reject waiting call
             - Enter option 45->1 to release active call and accept held/waiting call
             - Enter option 45->2 to hold active call and accept held/waiting call
             - Enter option 45->3 to conference the calls
             - Enter option 45->4 to join calls and disconnect self
         10). Codec Negotiation & Trigger codec connection
             - Enter option 75->6 to update available codecs information to AG
             - Enter option 75->8 to trigger a codec connection with the AG   
             
         Notice:
            1. Headset is connected to J12 of RT1060 board.
            2. In order to initiate connection from local side to a peer HFP AG mobile:
             - Enter option 2 (Register BD Addr) the peer mobile BD Address in LSB-MSB format
             - Enter option 7 (Create ACL) to setup ACL link with registered peer device, and wait for Successful HCI Connection event as described .
             - Enter option 8 (Get AG SDP) to read the server channel of the AG service in the peer device.
             - Enter option 9 (Establish SLC) to make the HFP service level connection with the peer device. On successful connection, the following prints will be displayed on the terminal

             > Event          : HFP_UNIT_CONNECT_CNF
             > Instance       : 0x00
             > Event result   : 0x0000
             > BD_ADDR of peer F1:CE:D4:F9:A2:64
    8.3 Running Sample A2DP Application
       a) Running Advanced Audio Distribution Profile - Source
          1). Setup and prepare the application.
              - Enter option 2 (EtherMind Init) in Main menu.
              - Enter option 3 (Bluetooth ON) in Main menu.
          2). Setup the A2DP profile
              - Enter option 65 (A2DP Operations) in Main menu.
              - Enter option 2 (A2DP Initialize) in A2DP menu
              - Enter option 4 (A2DP Register Codec) in A2DP menu
                > Enter Source Endpoint; 0
                > Enter codec type SBC; 0
          3). Make the A2DP Sink in discoverable mode, search and create and ACL connection
              - Enter option 11 (HCI Operations) in Main menu
              - Enter option 10 (Create Connection (ACL));
                > Enter the peer BD Address (in LSB to MSB order separated by space. Eg: 0x001BDCE0384C is the peer address,enter as "4C 38 E0 DC 1B 00")
                > Enter Clock Offset as 0
                > Enter Role Switch as 1
          4). Create and AVDTP transport connection as below
              - Enter option 10 (AVDTP Connect) in A2DP menu
                > Enter the ACL connection index, 0
              - On successful AVDTP transport connection, the following prints should be on the terminal.

                Received AVDTP_CONNECT_CNF
                        AVDTP Handle:
                            ADDR: E2:E2:A7:B8:70:E4
                        Result = 0x0000
                      
          5). Discover the Endpoints in the peer device
              - Enter option 11 (AVDTP Discover) in A2DP menu
                > Enter the ACL connection index, 0
              - On successful discovery completion, the following prints should be on the terminal.

                Received AVDTP_DISCOVER_CNF
                        AVDTP Handle:
                            ADDR: E2:E2:A7:B8:70:E4
                            Remote SEID = 0x00
                        Result = 0x0000
                        Event Data = 2022A668, Data Length = 2
                Response Buf [0] = 0x04, [1] = 0x08
                        Remote SEP Information [0]:
                            In Use     = No
                            SEP Type   = 0x01 -> Sink
                            ACP SEID   = 0x01
                            Media Type = 0x00 -> Audio
                           
          6a). Get the capabilities of the required endpoint ID as displayed in the Discover response
               - Enter option 12 (AVDTP Get Capabilities) in A2DP menu
                 > Enter the remote Stream Endpoint Identifier (SEID), 1 (ACP SEID) in this case is the Sink endpoint as in response above
               - On successful procedure completion, the following prints should be on the terminal. This displays the codec type and
                 capabilities supported by the endpoint.

                 Received AVDTP_GET_CAPABILITIES_CNF
                         AVDTP Handle:
                             ADDR: E2:E2:A7:B8:70:E4
                             Remote SEID = 0x01
                         Result = 0x0000
                         Event Data = 2022A634, Data Length = 10
                         Remote SEP Capabilities:
                             Media Type = 0x00 -> Audio
                             Codec Type = 0x00 -> SBC
                             Codec IE   = 0xFF 0xFF 0x02 0xFA
                         Code Type: SBC
                         SBC Codec Capability IE = 0xFF 0xFF 0x02 0xFA
                             -> Sampling Frequency = 16000 32000 44100 48000
                             -> Channel Modes = Mono Dual Stereo Joint-Stereo
                             -> Block Length = 4 8 12 16
                             -> Subbands = 4 8
                             -> Allocation Method = SNR Loudness
                             -> Bitpool Range = 2 - 250
          6b). Get all the capabilities of the required endpoint ID as displayed in the Discover response
               - Enter option 13 (AVDTP Get All Capabilities) in A2DP menu
                 > Enter the remote Stream Endpoint Identifier (SEID), 1 (ACP SEID) in this case is the Sink endpoint as in response above
               - On successful procedure completion, the following prints should be on the terminal. This displays the codec type and
                 capabilities supported by the endpoint.

                 Received AVDTP_GET_ALL_CAPABILITIES_CNF
                         AVDTP Handle:
                             ADDR: 3F:E3:08:DC:1B:00
                             Remote SEID = 0x01
                         Result = 0x0000
                         Event Data = 202434B4, Data Length = 12
                         Remote SEP Capabilities:
                             Media Type = 0x00 -> Audio
                             Codec Type = 0x00 -> SBC
                             Codec IE   = 0xFF 0xFF 0x02 0xFA
                         Code Type: SBC
                         SBC Codec Capability IE = 0xFF 0xFF 0x02 0xFA
                             -> Sampling Frequency = 16000 32000 44100 48000
                             -> Channel Modes = Mono Dual Stereo Joint-Stereo
                             -> Block Length = 4 8 12 16
                             -> Subbands = 4 8
                             -> Allocation Method = SNR Loudness
                             -> Bitpool Range = 2 - 250
                         Delay Reporting Service: YES

               - Note that the aboove procedure is required in order to configure Delay reporting in the A2DP configuration in case the Sink supports.
          7). Create the A2DP connection with the required endpoint
              - Enter option 5 (A2DP Connect) in A2DP menu
                > Enter A2DP instance as source, 0
                > Enter the ACL connection index, 0
                > Default configuration parameters are set by default that correspond to the preloaded music array.
                > To enable dynamic configuration at runtime, disable the flag APPL_A2DP_DEFAULT_CONFIG in the appl_a2dp.c file.
                > Enter the remote SEID, 1 in this case as in response above
              - On successful connection, the following print will be displayed on the terminal.

                Received A2DP_CONNECT_CNF
                        Codec Instance = 0
                        Event Result   = 0x0000

          8). Start A2DP Streaming
              - Enter option 6 (A2DP Start) in A2DP menu
                > Enter A2DP instance as source, 0
              - On successful streaming start procedure, the following print will be displayed on the terminal. And the music should be heard on the
                sink device

                Received A2DP_START_CNF
                        Codec Instance = 0
                        Event Result   = 0x0000

          9). Suspend A2DP Streaming
              - Enter option 7 (A2DP Suspend) in A2DP menu
                > Enter A2DP instance as source, 0
              - On successful streaming suspend procedure, the following print will be displayed on the terminal. And the music should have stopped on the
                sink device

                Received A2DP_SUSPEND_CNF
                        Codec Instance = 0
                        Event Result   = 0x0000

          10). Disconnect A2DP Connection
               - Enter option 9 (A2DP Disconnect) in A2DP menu
                 > Enter A2DP instance as source, 0
               - On successful disconnection procedure, the following print will be displayed on the terminal.

                 Received A2DP_DISCONNECT_CNF
                         Codec Instance = 0
                         Event Result   = 0x0000

          11).Disconnect AVDTP transport
              - Enter option 14 (AVDTP Disconnect) in A2DP menu
                > Enter the ACL connection index, 0
              - On successful AVDTP transport disconnection, the following prints should be on the terminal.

                Received AVDTP_DISCONNECT_CNF
                        AVDTP Handle:
                            ADDR: E2:E2:A7:B8:70:E4
                        Result = 0x0000

          NOTE: Typically headsets connect for A2DP immediately after AVDTP transport connection. In case of such an event, this reference application requires to disconnect A2DP and then reconnect A2DP from local side for proper configuration of media information for SBC, or do a Reconfigure of the link for the default supported Sampling Rate at the Source. From the step 8), do as follow:

          8). Reconfigure the A2DP Channel
             - Enter option 8 (A2DP Reconfigure) in A2DP menu
               > Select A2DP instance, 0

          9). To disconnect the A2DP connection, select option 9 followed by the A2DP instance.
          10). To disconnect the AVDTP connection, select option 14.

          Default Streaming Configuration
          --------------------------------
          The default configuration of the A2DP Source reference application is to stream a 16KHz stereo sample. In order to enable 48KHz stereo configuration, enable the flag A2DP_PL_SOURCE_48KHz_MEDIA in a2dp_pl.h. Also place a 48KHz stereo file in WAV format in the name 'sample.wav' under 'data/a2dp' in the USB memory drive and have it plugged in to the RT1060.

          Streaming Procedure
          0). To start the A2DP streaming procedure, select option 6 followed by the A2DP instance
          1). To suspend the A2DP streaming procedure, select option 7 followed by the A2DP instance

          MTU and Channel Parameters
          ---------------------------
           0. To update the A2DP sink media MTU size, select option 20 and give the size.
              Please note that this can be done only when the A2DP is not in connected state.

          TROUBLESHOOTING
          ----------------
          1. Sometimes when initiating connection to headset, after AVDTP transport connection is estabished, the A2DP connection might return following failures:
           - 0x6614 (A2DP_INVALID_STATE): Invalid State
              - In case of this error, please check if the A2DP_CONNECT_IND is already received in the application console. This means that after the AVDTP transport connection, the headset has setup the A2DP connection already. Try an A2DP Disconnect and then Connect again.
           - 0x6615 (A2DP_NO_MATCHING_CODEC_FOUND): No Free/Matching Codec
              - In case of this error, the stack is unable to find a free codec instance for connection or a codec instance with matching capabilities at the headset.
           - 0x0129 (AVDTP_UNSUPPORTED_CONFIGURATION): Unsupported Configuration
              - This happens when the requested configuration is not supported by the headset. The current Sampling Frequency supported by the A2DP Source appliation is 16KHz Stereo. Since many of the headsets support only 44.1 and 48KHz configuration, they might return error to this 16KHz configuration. The application can be enabled to try different combinations of user given configurations by disabling the flag APPL_A2DP_DEFAULT_CONFIG in the appl_a2dp.c application file.
          2. SBC encoder error code:
           - 0x1104 (SBC_MAX_BITRATE_EXCEED): 
              - The A2dp profile limits the available maximum bit rate to 320kb/s for mono, and 512kb/s for two-channel modes. If the bit rate that is calculated based on sample rate, channel mode, block length, subband and bitpool exceeds the limitation, this error code is return.
       b) Running Advanced Audio Distribution Profile - Sink
          1). Setup and prepare the application.
              - Enter option 2 (EtherMind Init) in Main menu.
              - Enter option 3 (Bluetooth ON) in Main menu.
          2). Setup the A2DP profile
              - Enter option 65 (A2DP Operations) in Main menu.
              - Enter option 2 (A2DP Initialize) in A2DP menu
              - Enter option 4 (A2DP Register Codec) in A2DP menu
                > Enter Sink Endpoint; 1
                > Enter codec type SBC; 0
          3a). Discover the device from a peer mobile phone and connect to it. The following prints should be displayed on the terminal.
         
              Received SM Service UI Notification. Event Type 0x01
              Received UI Connection Request from SM
              ADDR: F1:CE:D4:F9:A2:64
              Replying to UI Connection Request ... OK
              Received HCI_COMMAND_STATUS_EVENT.
                      Command Status: 0x00
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x0409 (HCI_ACCEPT_CONNECTION_REQUEST_OPCODE)

              Received HCI_COMMAND_STATUS_EVENT.
                      Command Status: 0x00
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x0409 (UNKNOWN)
              Received HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT.
                      BD_ADDR:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

                      Page Scan Repetition Mode: 0x01

              Unknown Event Code 0x20 Received.
              Received HCI_CONNECTION_COMPLETE_EVENT.
                      Status: 0x00
                      Connection Handle: 0x0001
                      BD_ADDR:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

                      Link Type: 0x01 -> ACL
                      Ecnryption Mode: 0x00 -> Encryption OFF

              Unknown Event Code 0x03 Received.
              Unknown Event Code 0x38 Received.

              Unknown Event Code 0x38 Received.
              Received HCI_MAX_SLOTS_CHANGE_EVENT.
                      Connection Handle: 0x0001
                      LMP Max Slots: 0x05

              Unknown Event Code 0x1B Received.
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x042B -> UNKNOWN
                      Command Status: 0x00
                      Return Parameters:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x042B -> UNKNOWN
                      Command Status: 0x00
                      Return Parameters:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

              Received SM Service UI Notification. Event Type 0x06
              Received UI User Conf Request from SM
              ADDR: F1:CE:D4:F9:A2:64
              Numeric Value = 528999 (0x00081267)
              Replying to UI User Conf Request ... OK
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x042C -> UNKNOWN
                      Command Status: 0x00
                      Return Parameters:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x042C -> UNKNOWN
                      Command Status: 0x00
                      Return Parameters:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

              Unknown Event Code 0x36 Received.

              Unknown Event Code 0x36 Received.
              Received HCI_LINK_KEY_NOTIFICATION_EVENT.
                      BD_ADDR:
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              F1 CE D4 F9 A2 64                                  .....d
              -------------------------------------------------------------------

                      Link Key:
              -- Dumping 16 Bytes --
              -------------------------------------------------------------------
              40 2E CD 89 06 EE 04 F2 B2 76 D4 0A 9D CD 10 62    @........v.....b
              -------------------------------------------------------------------

                      Key Type: 0x05 -> ???

              Unknown Event Code 0x18 Received.
              Received HCI_ENCRYPTION_CHANGE_EVENT.
                      Status: 0x00
                      Connection Handle: 0x0001
                      Ecnryption Enable: 0x01 -> Encryption ON

              Unknown Event Code 0x08 Received.
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x1408 -> HCI_READ_ENCRYPTION_KEY_SIZE_OPCODE
                      Command Status: 0x00
                      Read Encryption Key Size Status: 0x00
                      Connection Handle: 0x0001
                      Key Size: 0x10

              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x1408 -> UNKNOWN
                      Command Status: 0x00
                      Return Parameters:
              -- Dumping 3 Bytes --
              -------------------------------------------------------------------
              01 00 10                                           ...
              -------------------------------------------------------------------

              Received AVDTP_CONNECT_IND
                      AVDTP Handle:
                          ADDR: F1:CE:D4:F9:A2:64
                      Result = 0x0000

              Received A2DP_CONFIGURE_IND
                      Codec Instance = 0
                      Event Result   = 0x0000
                      Event Data = 202421A4, Data Length = 12
              [APPL] A2DP Sink Configured
              Code Type: SBC
                      SBC Codec IE = 0x21 0x15 0x02 0x35
                      -> Sampling Frequency = 44100
                      -> Channel Mode = Joint Stereo (2)
                      -> Block Length = 16
                      -> Subbands = 8
                      -> Allocation Method = Loudness
                      -> Bitpool = 2, 53
               Delay Report Support: NO

              Received A2DP_CONNECT_IND
                      Codec Instance = 0
                      Event Result   = 0x0000

          MTU and Channel Parameters
          ---------------------------
           0. To update the A2DP sink media MTU size, select option 20 and give the size.
              Please note that this can be done only when the A2DP is not in connected state.
           1. To get the current Media Channel parameters like CID and MTU that is configured,
              select option 21. This is a supportive interface that can be used by the
              application in case of platforms where if the controller is capable to handle
              media without involving the host, it would require these information.
         3b). Initiate a connection to a peer A2DP Source
            0. Make the A2DP Source in discoverable mode, search and create and ACL connection as desribed in the 'readme_main.txt'

            1. Create and AVDTP transport connection as below
                - Enter option 10 (AVDTP Connect) in A2DP menu
                > Enter the ACL connection index, 0
                - On successful AVDTP transport connection, the following prints should be on the terminal.

                Received AVDTP_CONNECT_CNF
                    AVDTP Handle:
                        ADDR: E2:E2:A7:B8:70:E4
                    Result = 0x0000

             2. Discover the Endpoints in the peer device
                - Enter option 11 (AVDTP Discover) in A2DP menu
                  > Enter the ACL connection index, 0
                - On successful discovery completion, the following prints should be on the terminal.

                Received AVDTP_DISCOVER_CNF
                        AVDTP Handle:
                            ADDR: E2:E2:A7:B8:70:E4
                            Remote SEID = 0x00
                        Result = 0x0000
                        Event Data = 2022A668, Data Length = 2
                Response Buf [0] = 0x04, [1] = 0x08
                        Remote SEP Information [0]:
                            In Use     = No
                            SEP Type   = 0x01 -> Source
                            ACP SEID   = 0x01
                            Media Type = 0x00 -> Audio

             3. Get the capabilities of the required endpoint ID as displayed in the Discover response
                - Enter option 12 (AVDTP Get Capabilities) in A2DP menu
                  > Enter the ACL connection index, 0
                  > Enter the remote Stream Endpoint Identifier (SEID), 1 (ACP SEID) in this case is the Sink endpoint as in response above
                - On successful procedure completion, the following prints should be on the terminal. This displays the codec type and
                  capabilities supported by the endpoint.

                Received AVDTP_GET_CAPABILITIES_CNF
                        AVDTP Handle:
                            ADDR: E2:E2:A7:B8:70:E4
                            Remote SEID = 0x01
                        Result = 0x0000
                        Event Data = 2022A634, Data Length = 10
                        Remote SEP Capabilities:
                            Media Type = 0x00 -> Audio
                            Codec Type = 0x00 -> SBC
                            Codec IE   = 0xFF 0xFF 0x02 0xFA
                        Code Type: SBC
                        SBC Codec Capability IE = 0xFF 0xFF 0x02 0xFA
                            -> Sampling Frequency = 16000 32000 44100 48000
                            -> Channel Modes = Mono Dual Stereo Joint-Stereo
                            -> Block Length = 4 8 12 16
                            -> Subbands = 4 8
                            -> Allocation Method = SNR Loudness
                            -> Bitpool Range = 2 - 250

             4. Create the A2DP connection with the required endpoint
                - Enter option 5 (A2DP Connect) in A2DP menu
                  > Enter A2DP instance as source, 0
                  > Enter the ACL connection index, 0
                  > Default configuration parameters are set by default that correspond to the preloaded music array.
                  > To enable dynamic configuration at runtime, disable the flag APPL_A2DP_DEFAULT_CONFIG in the appl_a2dp.c file.
                  > Enter the remote SEID, 1 in this case as in response above
                - On successful connection, the following print will be displayed on the terminal.

                Received A2DP_CONNECT_CNF
                        Codec Instance = 0
                        Event Result   = 0x0000


            Streaming Procedure
            --------------------
             0. To start the A2DP streaming procedure, select option 6 followed by the A2DP instance
             1. To suspend the A2DP streaming procedure, select option 7 followed by the A2DP instance
            Disconnection Procedure
            -----------------------
             3. To disconnect the A2DP connection, select option 9 followed by the A2DP instance.
             4. To disconnect the AVDTP connection, select option 14.
            Delay Reporting
            ----------------
            0. To send the current delay report value to the source, select option 16 and enter the delay.
            1. To set an initial delay report value after confirming that A2DP Source offers Delay Reporting with AVDTP GetAllCapabilities procedure,
               select option 15 and enter the delay.

            TROUBLESHOOTING
            ----------------
            1. Sometimes when initiating connection to A2DP Source, after AVDTP transport connection is estabished, the A2DP connection might return following failures:
             - 0x6614 (A2DP_INVALID_STATE): Invalid State
               - In case of this error, please check if the A2DP_CONNECT_IND is already received in the application console. This means that after the AVDTP transport connection, the A2DP Source has setup the A2DP connection already. Try an A2DP Disconnect and then Connect again.
             - 0x6615 (A2DP_NO_MATCHING_CODEC_FOUND): No Free/Matching Codec
               - In case of this error, the stack is unable to find a free codec instance for connection or a codec instance with matching capabilities at the A2DP Source.
             - 0x0129 (AVDTP_UNSUPPORTED_CONFIGURATION): Unsupported Configuration
               - This happens when the requested configuration is not supported by the A2DP Source. The application can be enabled to try different combinations of user given configurations by disabling the flag APPL_A2DP_DEFAULT_CONFIG in the appl_a2dp.c application file.

            2. A2DP Streaming does not trigger a media player to start playing music on the A2DP Source device. The user should manually play the music on the media player in the A2DP source device.

            3. Sending of delay reports to certain A2DP source devices which do not support the feature or have not configured the sink for delay report indications will result in a failure with the error code 0x661E (A2DP_DELAY_REPORT_SEND_FAILED). The application can initiate an AVDTP_GET_ALL_CAPABILITIES procedure (option 13 in A2DP menu) to know if the peer supports Delay Reporting.
            
            In case the support for delay report is offered by the peer A2DP Source in the the GetAllCapabilities response, the application can disconnect and reconnect only the A2DP Connection (AVDTP need not be disconnected) after setting an initial delay report value with option 15. The following would be the procedure.
             0. A2DP Connected from Source with Delay Reporting not configured
             1. Perform AVDTP GetAllCapabilities procedure on the corresponding SEID of peer
             2. Check if DelayReporting supported. If yes, and the application wants to enable delay reporting, Set the initial delay (option 15) with the required delay value.
             3. Disconnect the A2DP (Option 9)
             4. Connect the A2DP (Option 5). The application should see a A2DP_DELAY_REPORT_CNF event as a result of sending the first delay upon configuration.
             5. Now sending delay reports with option 16 should work as expected.

            LIMITION
            ----------------
            there is a short noise at the begin audio streaming.The codec power on pop noise cannot eliminate.
            
            Notice: Headset is connected to J12 of RT1060 board.
            
    8.4 Running Sample LE HPS Application
       0). Projects and Profile/Service Feature Flag Update.
           # The project to use this Profile Service application is evkmimxrt1060_ethermind.
           # Enable the macro "HPS" in evkmimxrt1060_ethermind\ethermind\bluetooth\export\appl\reference\le\profile_appl\server\gatt_db.h
             and disable any other Profile/Services macro in project compiler preprocessor setting.
           # Make sure that the feature flag "APPL_MENU_OPS" is defined in the project compiler preprocessor setting.
           # The Ethernet port should connect to a useable network with network cable.

       1). Refer to main menu 1-7 steps to setup and prepare the application.
       
       2). This demo HPS application has GAP Peripheral roles defined by default. But, the HPS needs to play the role of GAP Central.
           On Bluetooth Initialization Procedure, following USER Prompt will appear on the Console.
                                      NOTE to USER
                                      ----------
              Default GAP Role is GAP Central for this HPS Service. But, during
              init time setting the role to Peripheral. This needs to be updated
              from the HPS Menu after the desired Peer Device Address to which this
              service needs to scan and auto initiate connection is Set.

                      ** User needs to select the role from the HPS Menu **

       3). Select GATT server Menu by selecting "Option 81". And navigate to Service Menu Operations by selecting "Option 10".
       
       4). Now, choose HPS Menu Operations by selecting "Option 24". The following menu should be seen on the console:

              0. Exit
              1. Refresh
              10. Set Network Availability
              11. Update Peer BD Address to Auto-Connect
              12. Set GAP Central Role
              Your Option ?

       5). Typically the HPS application needs to act as a GAP Central device. For this, first Set the Peer BD Address that is needed to connect through
           the option 11.

              Enter the Peer BD Address to be updated and used...
              Enter BD_ADDR : 55 44 33 22 11 00
              Enter bd_addr_type : 0

              Default Config Peer Address Updated to :- ADDR: 55:44:33:22:11:00, TYPE: 00

       6). Now, select the GAP role as "Central" from the "Option 12 - Set GAP Central Role".
       
       7). Now, the HPS device should automatically scan and connect to the desired Peer BD Address.
       
       8). After this the Peer can perform various operations such as Discovery of HPS service and its characterisitcs, perform HTTP Methods like
           GET/HEAD/POST/PUT/DELETE/CANCEL etc over HPS Control Point etc.
       
       9). This HPS application will establish connection over LWIP on each of the Control Point procedures and send HTTP Status Notifications to the remote
           HPS Collector/Client application.
       
       10). Use "Option 10" to simulate the scenario of "Network Non-Availability". When this option is selected, below menu will appear
              
              Set Network Availability State
              0.No Network
              1.Network Available

            NOTE: This specific option would be needed for some test scenarios with Bluetooth Profile Tuning Suite (PTS ver 7.6.2) etc.
       
       11). For example, When the Remote HPS Client/Collector application performs a HTTP GET Request Method from the HPS Control Point, the below
            logs should be see on the console. Here the HPS server will
            - Get the IP address from the Host name provided by the HPS Client/Collector.
            - Establish a successful TCP-IP connection to this IP address. [Default Port is 80]
            - frame the HTTP METHOD command depending on the control point opcode and send the Command
            - Send Write response to the Control point request.
            - Receive the data from the Remote HTTP Server over the Network
            - Send the HTTP Status Notification to the Remote HPS Client/Collector.

            Trying to resolve Host name: httpbin.org
            Host name: httpbin.org resolved to: 18.208.255.250
            HPS Transport Established Successfully!
            Constructed HTTP Request:

            GET http://httpbin.org/get HTTP/1.1
            Host:httpbin.org


            [APPL]: Write Response sent with result 0x0000
            Received HTTP Response is:
            HTTP/1.1 200 OK
            Date: Tue, 27 Oct 2020 21:39:14 GMT
            Content-Type: application/json
            Content-Length: 199
            Connection: keep-alive
            Server: gunicorn/19.9.0
            Access-Control-Allow-Origin: *
            Access-Control-Allow-Credentials: true

            {
              "args": {},
              "headers": {
                "Host": "httpbin.org",
                "X-Amzn-Trace-Id": "Root=1-5f989382-4bdd8d0e76b4f6684cd36be5"
              },
              "origin": "122.172.219.58",
              "url": "http://httpbin.org/get"
            }


            Received HTTP Status Line of length 15 is:
            HTTP/1.1 200 OK

            Received HTTP Response Header is:
            Date: Tue, 27 Oct 2020 21:39:14 GMT
            Content-Type: application/json
            Content-Length: 199
            Connection: keep-alive
            Server: gunicorn/19.9.0
            Access-Control-Allow-Origin: *
            Access-Control-Allow-Credentials: true


            Received HTTP Response Entity Body is:
            {
              "args": {},
              "headers": {
                "Host": "httpbin.org",
                "X-Amzn-Trace-Id": "Root=1-5f989382-4bdd8d0e76b4f6684cd36be5"
              },
              "origin": "122.172.219.58",
              "url": "http://httpbin.org/get"
            }

            [APPL]: Stopping Control Point Procedure Timer 2022F34C, result 0x0000!
            [0x02]:Sending http status code 200 On Handle 0x0028
            [ATT]:[0x0D: 0x01]: Received ATT Event 0x1C with result 0x0000

            -- Dumping 5 Bytes --
            -------------------------------------------------------------------
            28 00 C8 00 05                                     (....
            -------------------------------------------------------------------

            [ATT]:[0x0D]: Received event 0x1C with result 0x0000
            Received HVN Tx Complete (Locally generated)

            -- Dumping 5 Bytes --
            -------------------------------------------------------------------
            28 00 C8 00 05                                     (....
            -------------------------------------------------------------------
        
        NOTE:
            1). This demo application currently does not handle scenarios of HTTP Certificate verification for the URI requested by the Peer HPS Client application.
                Currently, This demo uses LwIP to realize the TCP/IP functionality.
                To realize HTTPS functionality, we need HTTP+SSL/TLS. SSL/TLS enables the encryption procedures.
                LwIP Module inherently does not expose interfaces for SSL/TLS.
                Need to explore other modules from the SDK which exposes the needed interfaces to realize HTTPS functionality.

            2). This demo use fixed MAC address 02:12:13:10:15:11 for Ethernet connection.
                If need use two HPS demo boards under one router at the same time, please change the MAC address to prevent MAC conflacts.
                (the fixed MAC address located: ethermind\port\pal\mcux\bluetooth\niface.c line 32)

    8.5 Running Sample LE Proximity Application
       a) Running Proximity Profile - GATT Server
          0). Projects and Profile/Service Feature Flag Update.
              # The project to use this Profile Service application is evkmimxrt1060_ethermind.
              # Enable the macro "PXR" in evkmimxrt1060_ethermind\ethermind\bluetooth\export\appl\reference\le\profile_appl\server\gatt_db.h
                and disable any other Profile/Services macro in project compiler preprocessor setting.
          
          1). Refer to main menu 1-7 steps to setup and prepare the application.
          
          2). This demo PXR application plays GAP Peripheral roles. Hence, by defaut this demo application will advertise once BT ON Initialization sequence is
              completed.
          
          3). After the "Bluetooth ON Initialization Completed" is successfully completed. The Demo application
              automatically sets the BLE Advertisement Parameters, Data and enables Advertisements.
              The following prints should be seen on the terminal
              
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x0C6D -> UNKNOWN
                      Command Status: 0x00
              
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x2008 -> HCI_LE_SET_ADVERTISING_DATA_OPCODE
                      Command Status: 0x00
              
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x2006 -> HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE
                      Command Status: 0x00
              
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x200A -> HCI_LE_SET_ADVERTISING_ENABLE_OPCODE
                      Command Status: 0x00
              Enabled Advertising...
              EtherMind Server is now Ready
          
          4). Now, scan for this device from a Mobile device, For example from an Android 10 device which runs nRF Connect App.
          
          5). On the app, "Mt-PXR" should be seen. From the app it should now be possible to Connect, Discover Services, Read Characterisitics
              and Enable Notifications/Indications of Characterisitics that supports it.
              This Demo application consist of the following Services:
              # Generic Access
              # Generic Attribute
              # Battery Service
              # Link Loss
              # Immediate Alert
              # Tx Power
          
          6). The Demo is configured with the IO Capability set to "Display Only". When the user initiates "Bond" from the mobile App, look for the
              below prints on the terminal where the PASSKEY to be used for the Bonding/Pairing Session can be found
              
              Event   : SMP_PASSKEY_DISPLAY_REQUEST
              BD Address : EA 5F E8 3F 56 61
              BD addr type : Random Address
              Passkey : 491152
          
          7). When the Bonding/Pairing is successfully completed, the following print should be seen on the terminal
              Recvd SMP_AUTHENTICATION_COMPLETE
              BD Address : EA 5F E8 3F 56 61
              BD addr type : Random Address
              Status : 0000
              Authentication type : With MITM
              Pairing Mode : LEGACY Pairing Mode
              Bonding type : Bonding
              Encryption Key size : 16
          
          8). When the Peer Client(Proximity monitor) i.e. the Phone app configures the Immediate Alert Level. Depending on the level of the alert, following
              should be seen in the terminal (below example is for HIGH Alert Level)
                                      User Alert
                                      ----------
              [Proximity]: HIGH Alert => IMMEDIATE ALERT
          
          9). When the Peer Client(Proximity monitor) i.e. the Phone app configures the Link Loss Alert Level. Depending on the level of the alert, following
              should be seen in the terminal once the Link Disconnection happens (below example is for HIGH Alert Level)
                                      User Alert
                                      ----------
              [Proximity]: HIGH Alert => LINK LOST
          
          10). The Peer Client(Proximity monitor) i.e. the Phone app can configure the TX Power Level for Notifications. When the Notifications are enabled,
               the Demo tries to fetch the Transmit power level from the BT Controller through the HCI command and Notifies the TX Power Level every
               "APPL_PXR_TPS_MSRMT_TIMEOUT" seconds. This value would typically be configured to 5 seconds as default.

       b) Running Proximity Profile - GATT Client
          0). Projects and Profile/Service Feature Flag Update
              # The project to use this Profile-Collector/Service application is evkmimxrt1060_ethermind.
              # Since this is a Collector application, there is no specific Feature flag needed to be enabled in either preprocessor settings or in any Header file.
                By default "PXM" flag should be enabled in the project settings.

          1). Refer to main menu 1-7 steps to setup and prepare the application.
          
          2). The application will automatically start to Advertise after Bluetooth Initialization Procedures irrespective of the Service Specific Feature flag
              defined in gatt_db.h.
          
          3). The PXM collector needs to play the role of the GAP Central Device and GATT Client Device.
          
          4). If the device is already advertising, go to "GATT Client Menu" by selecting "Option 80" from Main Menu.
          
          5). Set the desired "Peer Device Address" to which Connection has to be initiated in Selecting "Option 3".
              NOTE: BD Address is entered from LSB to MSB and separated with space. i.e. if BD address is
              0x001122334455, enter the address as 55 44 33 22 11 00. Please refer to the below prints:

              Enter the Peer BD Address to be updated and used...
              Enter BD_ADDR : 55 44 33 22 11 00
              Enter bd_addr_type : 0
              Default Config Peer Address Updated to :- ADDR: 55:44:33:22:11:00, TYPE: 00
          
          6). After this set the GAP role for the current scenario by selecting "Option 4". In this case the GAP role is CENTRAL.
              
              Enter you choice : 4
              Set Current GAP Role
              0.GAP Central
              1.GAP Peripheral
              0
              Default GAP Role Set to CENTRAL
          
          7). Once the application receives a connectable ADV report from this configured peer device address. It will automatically initiate an LE Connection.
          
          8). Once, the LE connection is established successfully, you will see the below prints
          
              Received HCI_LE_META_EVENT.
              Subevent : HCI_LE_CONNECTION_COMPLETE_SUBEVENT.
              status = 0x00
              connection_handle = 0x005D
              role = 0x00
              peer_addr_type = 0x00
              peer_addr =
              
              -- Dumping 6 Bytes --
              -------------------------------------------------------------------
              55 44 33 22 11 00                                  @.....
              -------------------------------------------------------------------
              
              conn_interval = 0x0006
              conn_latency = 0x0000
              supervision_timeout = 0x03BB
              clock_accuracy = 0x00
          
              NOTE: Please ignore any prints such as
                - ACL Connected with IPSP Node: This is because IPSP Router might also be enabled in the project.
                - Unknown Event Code 0x3E Received: This is recevied from the BREDR HCI callback which does not parse this LE META event.
          
          9). After the successful LE connection navigate to PXM Client operations menu by selecting "Option 100 - Profile Client Menu" and
              finally selct PXM Client by selecting "Option 140 - PX Profile Operation". PXM menu shows below options
              
                0 - Exit
                1 - Refresh
           
               --- Link Loss Service ---
               10 - Discover Link Loss Service
               11 - Read Link Loss Service - Alert Level
               12 - Set Link Loss Service - Alert Level - "No Alert"
               13 - Set Link Loss Service - Alert Level - "Mild Alert"
               14 - Set Link Loss Service - Alert Level - "High Alert"
           
               --- Immediate Alert Service ---
               20 - Discover Immediate Alert Service
               21 - Immediate Alert Service - Alert Level - "No Alert"
               22 - Immediate Alert Service - Alert Level - "Mild Alert"
               23 - Immediate Alert Service - Alert Level - "High Alert"
           
               --- Tx Power Service ---
               30 - Discover Tx Power Service
               31 - Read Tx Power Level
               32 - Enable/Disable TX Power Service Notification
               Your Option?
          
          10). Using different options from this menu, one can control the Link Loss Service, Immediate Alert Service and Tx Power Service on the remote Proximity
               Reporter Device.
          
          11). To Initiate Disconnection with the currently connected device. Go back to the "GATT Client Menu" and choose "Option 8"
          
          12). To Initiate Pairing with the remote device, Go back to Main Menu and navigate to "SMP Menu" by selecting "Option 31".
               - Here choose the "Option 2 : Authenticate Remote Device".
               - Use the below parameters to Initiate an SMP Pairing Procedures with the remote device with
                 # SMP LE Secure Connections
                 # MITM Enabled
                 # Bonding Set
                 # Key Size 16
           
               Enter Your Choice: 2
               0 - Legacy LE SMP Procedure
               1 - LE Secure Connections Mode
               Enter SMP Pairing Mode :1
               0 - Encryption Only/Unauthenticated (Without MITM)
               1 - Authenticated (With MITM)
               Enter Security level required : 1
               0 - non-Bonding
               1 - Bonding
               Enter Bonding type : 1
               Enter Encryption Key size required : 16

    8.6 Running Sample LE HTS Application
       0). Projects and Profile/Service Feature Flag Update.
           # The project to use this Profile Service application is evkmimxrt1060_ethermind.
           # Enable the macro "HTS" in evkmimxrt1060_ethermind\ethermind\bluetooth\export\appl\reference\le\profile_appl\server\gatt_db.h
             and disable any other Profile/Services macro in project compiler preprocessor setting.
       
       1). Refer to main menu 1-7 steps to setup and prepare the application.
       
       2). This demo HTS application plays GAP Peripheral roles. Hence, by defaut this demo application will advertise once BT ON Initialization sequence is
          completed.
       
       3). After the "Bluetooth ON Initialization Completed" is successfully completed. The Demo application
          automatically sets the BLE Advertisement Parameters, Data and enables Advertisements.
          The following prints should be seen on the terminal
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x0C6D -> UNKNOWN
                      Command Status: 0x00
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x2008 -> HCI_LE_SET_ADVERTISING_DATA_OPCODE
                      Command Status: 0x00
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x2006 -> HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE
                      Command Status: 0x00
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x200A -> HCI_LE_SET_ADVERTISING_ENABLE_OPCODE
                      Command Status: 0x00
              Enabled Advertising...
              EtherMind Server is now Ready
      
       4). Now, scan for this device from a Mobile device, For example from an Android 10 device which runs nRF Connect App.
       
       5). On the app, "Mt-HTS" should be seen. From the app it should now be possible to Connect, Discover Services, Read Characterisitics
          and Enable Notifications/Indications of Characterisitics that supports it.
          This Demo application consist of the following Services:
           # Generic Access
           # Generic Attribute
           # Battery Service
           # Device Information
           # Health Thermometer
      
       6). The Demo is configured with the IO Capability set to "Display Only". When the user initiates "Bond" from the mobile App, look for the
          below prints on the terminal where the PASSKEY to be used for the Bonding/Pairing Session can be found
      
              Event   : SMP_PASSKEY_DISPLAY_REQUEST
              BD Address : EA 5F E8 3F 56 61
              BD addr type : Random Address
              Passkey : 491152
      
       7). When the Bonding/Pairing is successfully completed, the following print should be seen on the terminal
      
              Recvd SMP_AUTHENTICATION_COMPLETE
              BD Address : EA 5F E8 3F 56 61
              BD addr type : Random Address
              Status : 0000
              Authentication type : With MITM
              Pairing Mode : LEGACY Pairing Mode
              Bonding type : Bonding
              Encryption Key size : 16
      
       8). When the Peer Client(Health Thermometer Collector) i.e. the Phone app configures the Temperature Measurement for Indications, a default template
          temperature data is sent as Indications.
          The Template temperature corresponds to "36.4 C" with some default Time Stamp [29/04/2014 06:00:01 (DD/MM/YYYY H:M:S)].
          This data is sent as per the "Mesurement Interval" Characteristic Value. The default interval is 5 Seconds.
          This Demo supports a range of "Mesurement Interval" from 1 Second to 10 Seconds.
      
       9). When the Peer Client(Health Thermometer Collector) i.e. the Phone app also configures Intermediate temperaure measurement for Notifications,
          a series of default template temperature measurements are sent. These values are "35.9 C", "36.1 C" and "36.3 C".
       
       10). All the above default template temperature measurements are assigned the Temperature Type as "Armpit".
       
    8.7 Running Sample LE IPSPN Application
       0). Projects and Profile/Service Feature Flag Update.
           # The project to use this Profile Service application is evkmimxrt1060_ethermind.
           # Enable the macro "IPSPN" in evkmimxrt1060_ethermind\ethermind\bluetooth\export\appl\reference\le\profile_appl\server\gatt_db.h
             and disable any other Profile/Services macro in project compiler preprocessor setting.

       1). Refer to main menu 1-7 steps to setup and prepare the application.
       
       2). This demo IPSPN application plays GAP Peripheral roles. Hence, by defaut this demo application will advertise once BT ON Initialization sequence is
           completed.
       
       3). During the "Bluetooth ON Initialization Sequence", the application will automatically initialize LWIP stack and DHCP. Below prints will be seen
           on the Console
      
              Initializing PHY...
      
              lwip ping netif init complete!
      
              ************************************************
              DHCP start
              ************************************************
              DHCP state       : SELECTING
              DHCP state       : REQUESTING
              DHCP state       : CHECKING
              DHCP state       : BOUND
      
              IPv4 Address     : 192.168.1.144
              IPv4 Subnet mask : 255.255.255.0
              IPv4 Gateway     : 192.168.1.251
      
      
              lwip ping DHCP complete!
              SUCCESS!!
      
       4). After the "Bluetooth ON Initialization Completed" is successfully completed. The Demo application
          automatically sets the BLE Advertisement Parameters, Data and enables Advertisements.
          The following prints should be seen on the terminal
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x0C6D -> UNKNOWN
                      Command Status: 0x00
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x2008 -> HCI_LE_SET_ADVERTISING_DATA_OPCODE
                      Command Status: 0x00
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x2006 -> HCI_LE_SET_ADVERTISING_PARAMETERS_OPCODE
                      Command Status: 0x00
      
              Received HCI_COMMAND_COMPLETE_EVENT.
                      Num Command Packets: 1 (0x01)
                      Command Opcode: 0x200A -> HCI_LE_SET_ADVERTISING_ENABLE_OPCODE
                      Command Status: 0x00
              Enabled Advertising...
              EtherMind Server is now Ready
      
       5). Now, scan for this device from a Mobile device, For example from an Android 10 device which runs nRF Connect App.
       
       6). On the app, "Mt-IPSPN" should be seen. From the app it should now be possible to Connect, Discover Services, Read Characterisitics
          and Enable Notifications/Indications of Characterisitics that supports it.
          This Demo applications consist of the following Services:
           # Generic Access
           # Generic Attribute
           # Battery Service
           # Internet Protocol Support
       
       7). The Demo is configured with the IO Capability set to "Display YesNo" and default SMP Pairing Mode set to SMP LE Secure connections.
           When the user initiates "Bond" from the mobile App, look for the below prints on the terminal where the Numeric Comparision Code to be
           used for the Bonding/Pairing Session can be found

            Event   : SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST
            BD Address : 68 00 48 DA 0C 67
            BD addr type : Random Address
            Numeric Code : 999999

       8). When the Bonding/Pairing is successfully completed, the following print should be seen on the terminal
            
            Recvd SMP_AUTHENTICATION_COMPLETE
            BD Address : 68 00 48 DA 0C 67
            BD addr type : Random Address
            Status : 0000
            Authentication type : With MITM
            Pairing Mode : LE SEC Pairing Mode
            Bonding type : Bonding
            Encryption Key size : 16

       9). This IPSPN Demo, is configured to accept LE L2CAP CoC request from remote device on LE IPSP PSM.
           The default MTU, MPS and Initial Credits value for this is 1500, 512 and 10.

       10). If IPSP_HAVE_6LO_NIFACE feature flag is defined, any value data received over this IPSP LE L2CAP CoC transport would be forwared to the Network
           stack or the 6LO Interface.

       11). This demo automatically handles the transfer of LE L2CAP CoC receive credits after each data packet reception if APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT.
           If APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM is defined then credits are sent automatically, on reaching Low Water Mark.

          Notes:
          Please follow the below procedures to verify the "ping" over IPSPN between the
          EtherMind application running on iMXRT1060 and BlueZ stack on a Ubuntu Box.

          The following steps/commands not work on new Ubuntu/BlueZ version.
          So the Ubuntu version should be "Ubuntu 14.04" and BlueZ version should be "4.101".
          
          1). Run this IPSPN Demo and perform the Bluetooth ON Intializations and wait
              for LWIP and DHCP to start and the device to Advertise.
          2). On a Ubuntu machine (the version we used was 14.04LTS) which has the BlueZ
              stack installed, perform the following commands
              - Enter Super User mode
              - Setup a bluetooth device or use the machine's inbuilt BT controller and
                check for it to show on the hciconfig interface.
              - Initial Setup related commands [in Super User Mode]
           
              hciconfig hci0 up
                 modprobe bluetooth_6lowpan
                 echo 1 > /sys/kernel/debug/bluetooth/6lowpan_enable
           
              - BLE Connection related commands. Here, the device Bluetooth Address is
                D8C0A6C0B106(D8:C0:A6:C0:B1:06). Please update the address in below
                commands depending on the bluetooth address of the local device.
           
                echo "connect D8:C0:A6:C0:B1:06 1" > /sys/kernel/debug/bluetooth/6lowpan_control
          
          3). After the above steps, the BlueZ stack will initiate LE connection to the
              device and also establish an LE L2CAP CoC channel for IPSP PSM. The below
              prints should be seen on the device's teraterm/console. Also make a note
              of the LinkLocal address.
           
                BtLE 6Lo interface added successfully
                NetIF LinkLocal Address: FE80::DAC0:A6FF:FEC0:B106
                NetIF Global Address: 2001:db8::1

          4). On the Ubuntu machine, check if any bt interface is established by issuing
              "ifconfig" command. eg: typically bt0 interface would be created
              at this point.
          
          5). Based on the bt interface creation issue the below command an check if ping
              response is received. Please change the LinkLocal address based on the
              device being used and also the bt interface number based on the one that is
              created.
           
               ping6 -s 16 FE80::DAC0:A6FF:FEC0:B106%bt0
    
    8.8 Running Sample RFCOMM Application
       a) Running RFCOMM application as server
          1). Setup and prepare the application.
              - Enter option 2 (EtherMind Init) in Main menu.
              - Enter option 3 (Bluetooth ON) in Main menu.
          2). Setup the RFCOMM Server
              - Enter option 14 (RFCOMM Operations) in Main menu.
              - Enter option 4 (RFCOMM Channel Accept) in RFCOMM menu to setup server listening on default server channel 0x01.
       b) Running RFCOMM application as client
          1). Setup and prepare the application.
              - Enter option 2 (EtherMind Init) in Main menu.
              - Enter option 3 (Bluetooth ON) in Main menu.
          2). Discover the RFCOMM Server device and establish the ACL connection. 
              - Enter option 11 (HCI Operations) in Main menu
              - Enter option 10 (Create Connection (ACL)) in HCI menu
                > Enter the peer BD Address (in LSB to MSB order separated by space. Eg: 0x001BDCE0384C is the peer address,enter as "4C 38 E0 DC 1B 00")
                > Enter Clock Offset as 0
                > Enter Role Switch as 1
          3). Setup the RFCOMM Client
              - Enter option 0 (exit) in HCI menu.
              - Enter option 14 (RFCOMM Operations) in Main menu.
              - Enter option 2 (RFCOMM Channel Open) in RFCOMM menu.
                > Enter the RFCOMM Server BD Address (in LSB to MSB order separated by space. Eg: 0x001BDCE0384C is the peer address,enter as "4C 38 E0 DC 1B 00")
                > Default server channel 0x01 will be used since APPL_RFCOMM_READ_SERVER_CHANNEL flag is not defined.
       c) Write data on the connected RFCOMM channel
          - After RFCOMM connection is created, data can be sent from either server or client.
          - Enter option 11 (RFCOMM Channel Write) in RFCOMM menu
          - Enter the number of bytes to write
          - Enter the number of times to write
          - Data can be received by peer device
          
          Notice: If number of bytes to write is bigger than APPL_RFCOMM_DUMP_CHARS_MAX_LENGTH(8 as default),
                  receiver will just dump APPL_RFCOMM_DUMP_CHARS_MAX_LENGTH bytes. 
                  You can change this length in appl_rfcomm.h.

       d) File transfer on the connected RFCOMM channel
          - After RFCOMM connection is created, file transfer can be started from either server or client.
          - For receiver
            > Connect U-disk to RT1060 board's J9 with OTG
            > Enter option 12 (RFCOMM Receive File) in RFCOMM menu
            > Enter file name           
          - For sender
            > Save the file to be transferred in the U-disk
            > Connect U-disk to RT1060 board's J9 with OTG
            > Enter option 13 (RFCOMM Send File) in RFCOMM menu
            > Enter file name that is saved
       e) Close the connected RFCOMM channel
          - RFCOMM close can be operated from either server or client.
          - Enter option 3 (RFCOMM Channel Close)in RFCOMM menu.

 9. To set a device specific friendly name to be identified in a peer device during discovery.
    - Enter option 11 (HCI Operations) in Main menu.
    - Enter option 45 (Write Local Name) in HCI menu;
      > Enter a friendly name as desired.

 10.Disable the BLE Advertisement since the mobiles display the name in the advertisements sometimes.
    - Enter option 11 (HCI Operations) in Main menu.
    - Enter option 80 (LE Operations) in HCI menu.
    - Enter option 10 (Set advrt enable) in HCI BLE menu.
      > Enter advertising enable as 0

 11.If running any audio profiles like A2DP Sink/HFP Unit, it is required to set the Class of Device in order to enable the mobile phones
     to recognize this as a headset device and display during discovery.
    - Enter option 11 (HCI Operations) in Main menu.
    - Enter option 44 (Write Class of Device) in HCI menu;
      > Enter the value 200404 or 200408.

 13.To discover any peer device, set the inquiry mode and start the inquiry procedure as below
    - Enter option 11 (HCI Operations) in Main menu
    - Enter option 50 (Write Inquiry mode) in HCI menu
      > Enter inquiry mode as 2
    - Enter option 3 (Inquiry) in HCI menu
      > Enter LAP as 9E8B33
      > Enter Inquiry length as 10
      > Enter Num Responses as 10
    - The terminal should list discovered devices and the discovery process will end with the following event

Received HCI_INQUIRY_COMPLETE_EVENT.
        Status: 0x00

 12.To initiate a profile connection from the local device, start by establishing the base ACL link connection as below
    - Enter option 11 (HCI Operations) in Main menu
    - Enter option 10 (Create Connection (ACL));
      > Enter the peer BD Address (in LSB to MSB order separated by space. Eg: 0x001BDCE0384C is the peer address,enter as "4C 38 E0 DC 1B 00")
      > Enter Clock Offset as 0
      > Enter Role Switch as 1
    - The following prints will be displayed on successful connection initiation

Received HCI_COMMAND_STATUS_EVENT.
        Command Status: 0x00
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0405 (HCI_CREATE_CONNECTION_OPCODE)

    -  On successful connection establishment, the following prints will be displayed.

Received HCI_CONNECTION_COMPLETE_EVENT.
        Status: 0x00
        Connection Handle: 0x0001
        BD_ADDR:
-- Dumping 6 Bytes --
-------------------------------------------------------------------
F1 CE D4 F9 A2 64                                  .....d
-------------------------------------------------------------------

        Link Type: 0x01 -> ACL
        Ecnryption Mode: 0x00 -> Encryption OFF

Unknown Event Code 0x03 Received.
Received HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT.
        BD_ADDR:
-- Dumping 6 Bytes --
-------------------------------------------------------------------
F1 CE D4 F9 A2 64                                  .....d
-------------------------------------------------------------------

        Page Scan Repetition Mode: 0x01

Unknown Event Code 0x20 Received.
Received HCI_MAX_SLOTS_CHANGE_EVENT.
        Connection Handle: 0x0001
        LMP Max Slots: 0x05

 13.To disconnect the ACL link with a connected peer device, do the following steps -
    - Enter option 11 (HCI Operations) in main menu
    - Enter option 20 (Get HCI Connection Details) in HCI menu. Note the connection handle of the connection.
    - Enter option 12 (Disconnection) in HCI menu;
      > Enter the connection handle as noted above
      > Enter reason as 13. 
    - The following prints will be displayed on successful disconnection initiation

Received HCI_COMMAND_STATUS_EVENT.
        Command Status: 0x00
        Num Command Packets: 1 (0x01)
        Command Opcode: 0x0406 (HCI_DISCONNECT_OPCODE)

    -  On successful disconnection, the following prints will be displayed.

Received HCI_DISCONNECTION_COMPLETE_EVENT.
        Status: 0x00
        Connection Handle: 0x0001
        Reason: 0x16

 14.During the authentication procedure that gets triggered during any profile connection, the peer device like mobile phone will ask
    the confirmation of a 6 digit numeric key. This numeric can be looked up on the terminal for the local device with the following print.

Received SM Service UI Notification. Event Type 0x06
Received UI User Conf Request from SM
ADDR: F1:CE:D4:F9:A2:64
Numeric Value = 972886 (0x000ED856)
Replying to UI User Conf Request ... OK

 12.Upon successful Authentication and link Encyption, the following prints will be seen on the terminal

Received HCI_LINK_KEY_NOTIFICATION_EVENT.
        BD_ADDR:
-- Dumping 6 Bytes --
-------------------------------------------------------------------
F1 CE D4 F9 A2 64                                  .....d
-------------------------------------------------------------------

        Link Key:
-- Dumping 16 Bytes --
-------------------------------------------------------------------
46 1C 72 D7 08 AE D6 B9 96 04 51 5D 0B 34 26 52    F.r.......Q].4&R
-------------------------------------------------------------------

        Key Type: 0x05 -> ???

Unknown Event Code 0x18 Received.
Received HCI_ENCRYPTION_CHANGE_EVENT.
        Status: 0x00
        Connection Handle: 0x0001
        Ecnryption Enable: 0x01 -> Encryption ON
Note:
Command 2(Set Security Mode) and 3(Get Security Mode) under SM option are not supported if BT_SSP flag is enabled in BT feature

    8.9 Wake on BLE and OOB wakeup over GPIO

        Host is RT1170 and Controller is Firecrest board.
        Host will initialize the RT1170 platform specific GPIOs used for Host to Controller and Controller to Host sleep and wakeup.

        Vendor specific command 0x0053 is to enable/configure the Host to Controller and Controller to Host sleep and wakeup scenarios.

        - Enter option 205 in main menu
Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0xFC53 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT in LE.
        LE Num Command Packets: 1 (0x01)
        LE Command Opcode: 0xFC53 -> UNKNOWN
        LE Command Status: 0x00

        Vendor specific command 0x0023 is to enable the Controller to make it ready for sleep and wakeup.

        - Enter option 206 in main menu
Received HCI_COMMAND_COMPLETE_EVENT.
        Num Command Packets: 1 (0x01)
        Command Opcode: 0xFC23 -> UNKNOWN
        Command Status: 0x00

Received HCI_COMMAND_COMPLETE_EVENT in LE.
        LE Num Command Packets: 1 (0x01)
        LE Command Opcode: 0xFC23 -> UNKNOWN
        LE Command Status: 0x00

        Host to controller sleep and wakeup procedure:
        ----------------------------------------------

        Host will initialize controller sleep and wakeup mechanism through one time vendor specific commands 0x0053 and 0x0023 between power cycles.
        Host has to give 0x0023 command for controller SLEEP and WAKEUP to work.
        As soon as option 206 is given and as by default GPIO12 on Controller is HIGH, Controller will go to SLEEP.

        Host will use J9 pin 12 to control the controller GPIO12 which comes on Firecrest J10 header pin number 23(on Firecrest board back side labelled as 12).

        Host has to make GPIO12 on Controller HIGH to move into DEEPSLEEP mode. As soon as Controller move into DEEPSLEEP mode, Host should not send any HCI command.
        Make sure controller sleep vendor command option 206 is given before this.
        - Enter option 207 in main menu.

        Host has to make GPIO12 on Controller LOW to move into WAKEUP mode so that Controller can accept HCI commands.
        - Enter option 208 in main menu.

        In this case it satisfies wakes up Controller externally using OOB GPIO.

        Controller to Host sleep and wakeup procedure:
        ----------------------------------------------
        Controller will use GPIO16 here to let Host know when to sleep, and when to wakeup.

        Host will use J9 pin 6 to receive interrupt from Controller GPIO16(on Firecrest board top side HD3 labelled as 16).
        Host should sleep as soon as GPIO16 goes HIGH. As soon as Host prints "Host can now go to sleep", enter option 209 in main menu.

        Host will wakeup as soon as GPIO16 goes LOW (this happens when there is any event pending on host side which controller is sending).
        
        In this case it satisfies Wake on BLE triggered internally in controller wakes up Host externally using OOB GPIO.
