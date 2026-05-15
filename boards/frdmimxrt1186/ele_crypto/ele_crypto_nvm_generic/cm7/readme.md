# ele_crypto_nvm_generic

## Overview
The ELE Crypto NVM Example project is a demonstration program that uses the MCUX SDK
software to perform non volatile memory (flash) operations with EdgeLock Enclave (ELE)
and usage of its services with direct use of Messaging Unit driver.

## Running the demo
Example output on terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EdgeLock Enclave secure Sub-System Driver Example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Start RNG ******************************
EdgeLock RNG Start success.
EdgeLock RNG ready to use.

****************** Initialize ELE services ****************
ELE services initialized successfully.

****************** Open EdgeLock session ******************
Open session successfully. Session ID: 0x76f7758f

****************** Create Key Store ***********************
Open service and create Key Store successfully. Key Store ID: 0x76f7750f

****************** Open NVM Storage service ****************
Open NVM Storage service successfully. Handle ID: 0x76f772d7

****************** Key Management Open ********************
Open Key management service successfully. Key Handle ID: 0x76f772a7

****************** Open Storage service ********************
Open Storage service successfully. Handle ID: 0x76f77277

****************** Data Storage Store **********************
Data Storage successfully. Exported Chunk at address 0x20484008

****************** Key Management Export Chunks ***********
Chunks exported successfully.
         Storage Master chunk at address:        0x47ffefc , size 100 Bytes
         Key Store chunk at address:     0x20484098 , size 892 Bytes
Close data storage session successfully.

****************** Close Key Management Service ***********
Close Key Management Service successfully.

****************** Close Key Store ************************
Close Key Store successfully.

Close NVM storage session successfully.

****************** Close EdgeLock session *****************
Close session successfully.

****************** Open EdgeLock session ******************
Open session successfully. Session ID: 0x76f7758f

****************** Open NVM Storage service ****************
Open NVM Storage service successfully. Handle ID: 0x76f77567

****************** Import Master storage chunk ************
Import Master storage chunk successfully.

****************** Open Key Store ****************
Open service and create Key Store successfully. Key Store ID: 0x76f772df

****************** Key Management Open ********************
Open Key management service successfully. Key Handle ID: 0x76f772a7

****************** Open Storage service ********************
Open Storage service successfully. Handle ID: 0x76f77277

****************** Data Storage Retrieve *******************
Data Storage Retrieve successfully. Exported Chunk at address 0x4000008
Success: Retrieved data at addr 0x4000008 are same as stored plain data at 0x47fff70
Close data storage session successfully.

****************** Close Key Management Service ***********
Close Key Management Service successfully.

****************** Close Key Store ************************
Close Key Store successfully.

Close NVM storage session successfully.

****************** Close EdgeLock session *****************
Close session successfully.

End of Example with SUCCESS!!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The IDs and addresses will/may be different on every run.

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ele_crypto/ele_crypto_nvm_generic/example_board_readme.md)
