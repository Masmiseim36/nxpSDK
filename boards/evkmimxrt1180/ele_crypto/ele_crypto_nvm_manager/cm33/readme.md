# ele_crypto_nvm_manager

## Overview
The ELE Crypto NVM Example project is a demonstration program that uses the MCUX SDK
software to perform non volatile memory (flash) operations with EdgeLock Enclave (ELE)
and usage of its services with direct use of Messaging Unit driver.

## Running the demo
Example output on terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Card inserted.

Create directory......
EdgeLock Enclave secure Sub-System Driver Example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Start RNG ******************************
EdgeLock RNG Start success.
EdgeLock RNG ready to use.

****************** Initialize ELE services ****************
ELE services initialized successfully.

****************** Load EdgeLock NVM Mgr ***********************
EdgeLock NVM manager registered.

****************** Open EdgeLock session ******************
Open session successfully. Session ID: 0xf8fb18e0

****************** Open NVM Storage service ****************
Open NVM Storage service successfully. Handle ID: 0xf8fb18b8

****************** Create Key Store ***********************
Key Store Opened successfully. Key Handle ID: 0xf8fb1630

****************** Key Management Open ********************
Open Key management service successfully. Key Handle ID: 0xf8fb16f8

****************** Key Generate (ECC NIST P256) ***********
Key generated successfully. Key Pair ID: 0x1

****************** Key Generate () ************************
Key for AES-ECB generated successfully. Key ID: 0x2

****************** Open cipher service *********************
Open Cipher service successfully. Handle ID: 0xf8fb2d60

****************** Encrypt Cipher AES-ECB ******************
AES-ECB encryption success, Output size returned: 0x20
****************** Open Sign service **********************
Sign service open successfully. Signature generation handle ID: 0xf8fb2d30

****************** ECC NIST P256 sign *********************
Signature generated successfully.

****************** Open Storage service ********************
Open Storage service successfully. Handle ID: 0xf8fb2dc0

****************** Data Storage Store **********************
Data Storage successfully. Exported Chunk at address 0x20484018

****************** Data Storage Retrieve *******************
Data Storage Retrieve successfully. Exported Chunk at address 0x204c0000
Success: Retrieved data at addr 0x204c0000 are same as stored plain data at 0x2001fee8
****************** Key Delete (ECC NIST P256) *************
Key deleted successfully. Key Pair ID: 0x1

Close data storage session successfully.

****************** Close cipher service **********************
Cipher service closed successfully.

****************** Close Sign service ************************
Sign service close successfully.

****************** Close Key Management Service ***********
Close Key Management Service successfully.

****************** Close Key Store ************************
Close Key Store successfully.

Close NVM storage session successfully.

****************** Close EdgeLock session *****************
Close session successfully.

****************** Open EdgeLock session ******************
Open session successfully. Session ID: 0xf8fb18e0

****************** Open NVM Storage service ****************
Open NVM Storage service successfully. Handle ID: 0xf8fb18b8

****************** Import Master storage chunk ************
Import Master storage chunk successfully.

****************** Open Key Store ****************
Open service and create Key Store successfully. Key Store ID: 0xf8fb1630

****************** Key Management Open ********************
Open Key management service successfully. Key Handle ID: 0xf8fb16f8

****************** Open cipher service *********************
Open Cipher service successfully. Handle ID: 0xf8fb1688

****************** Decrypt Cipher AES-ECB ******************
AES-ECB decrypted data match the original plain text - success.

****************** Key Delete (AES) *************
Key deleted successfully. Key ID: 0x2

****************** Close cipher service **********************
Cipher service closed successfully.

****************** Close Key Management Service ***********
Close Key Management Service successfully.

****************** Close Key Store ************************
Close Key Store successfully.

Close NVM storage session successfully.

****************** Close EdgeLock session *****************
Close session successfully.

****************** Unregister NVM Manager *****************
NVM manager unregistered successfully.

End of Example with SUCCESS!!

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ele_crypto/ele_crypto_nvm_manager/example_board_readme.md)
