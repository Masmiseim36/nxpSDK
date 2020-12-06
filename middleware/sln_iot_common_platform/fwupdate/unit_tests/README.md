# Firmware Update Test Client Example

FW Update Test Client Example is a python script that tests FWUPDATE-API communication to the SLN-ALEXA-IOT or SLN-VIZN-IOT kit

**NOTE:** This script is intended as an example, and not inteded to be run 'as-is' in a production environment.

## Software Requirements

Python 3.6 or greater
PySerial
libscrc

### Linux

```
user@host:~$ pip3 install pyserial
user@host:~$ pip3 install libscrc
```

### Windows

```
C:\> pip install pyserial
C:\> pip install libscrc
```

## OTA

This gets status of the board as well as executes an OTA image that has been created by the Ivaldi OTA Signing

## OTW

This executes OTW image update over an UART port

### Hardware Configuration

Program the Bootloader and Main Application firmware with the Comms Handler installation.

### Usage

Open fwupdate_client.py and edit the COM_PORT to match the target com port address. See line 70 below for example.

```
65  if FWUPDATE_METHOD == "OTA":
66      IP_ADDRESS='192.168.1.190'
67      PORT=8889
68  elif FWUPDATE_METHOD == "OTW":
69      # If working on Windows, change this to the appropriate COM
70      COM_PORT='/dev/ttyUSB0'
```

#### Linux

```
user@host:~$ python3 fwupdate_client.py sln_local_iot OTW A bundle.BankA_RVDISP.bin BankA_RVDISP.bin.sha256.txt
```

#### Windows

```
C:\> python fwupdate_client.py sln_local_iot OTW A bundle.BankA_RVDISP.bin BankA_RVDISP.bin.sha256.txt
```
