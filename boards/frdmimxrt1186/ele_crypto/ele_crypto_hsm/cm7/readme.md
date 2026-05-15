# ele_crypto_hsm

## Overview
The ELE Crypto Example project is a demonstration program that uses the MCUX SDK
software to perform crypto operations with EdgeLock Enclave (ELE) and usage of
its services with direct use of Messaging Unit driver.

## Running the demo
Example output on terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

EdgeLock Enclave Sub-System crypto example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Start RNG ******************************
EdgeLock RNG Start success.
EdgeLock RNG ready to use.

****************** Initialize EdgeLock services ***********
EdgeLock services initialized successfully.

****************** Open EdgeLock session ******************
Open session successfully. Session ID: 0xf623c27f

****************** Create Key Store ***********************
Open service and create Key Store successfully. Key Store ID: 0xf623ccff

****************** Key Management Open ********************
Open Key management service successfully. Key Handle ID: 0xf623ccb7

****************** Key Generate (AES ECB 128) *************
Key for AES-ECB generated successfully. Key ID: 0xcafe

****************** Open cipher service ********************
Open Cipher service successfully. Handle ID: 0xf623f7cf

****************** Cipher AES ECB *************************
Output size returned by AES-ECB encryption is : 0x20
AES-ECB crypto encryption and decryption using keys in ELE success.

****************** Key Generate (AEAD) ********************
Key for AES-GCM generated successfully. Key ID: 0x6fffffff

****************** Cipher AES AEAD (GCM) ******************
Output size returned by AES-AEAD encryption is : 0x30
AES-AEAD crypto encryption and decryption using keys in ELE success.

****************** Open MAC service ***********************
Open MAC service successfully. Handle ID: 0xf623f367

****************** Key Generate (HMAC_SHA256) *************
Key for HMAC_SHA256 generated successfully. Key ID: 0x6ffffffe

****************** HMAC_SHA256 ****************************
HMAC_SHA256 has been generated successfully
HMAC_SHA256 has been verified successfully

****************** Key Generate (ECC NIST P256) ***********
Key generated successfully. Key Pair ID: 0x6ffffffd

****************** Open Sign service **********************
Sign service open successfully. Signature generation handle ID: 0xf623f317

****************** ECC NIST P256 sign *********************
Signature generated successfully.

****************** Open Verify service ********************
Verify service open successfully. Verification handle ID: 0xf623f2c7

****************** ECC NIST P256 verify *******************
ECC NIST p256 Signature verified successfully!

****************** Key Delete (ECC NIST P256) *************
Key deleted successfully. Key Pair ID: 0x6ffffffd

****************** Key Generate (ECC Brainpool) ***********
Key generated successfully. Key Pair ID: 0x6ffffffd

****************** ECC Brainpool sign *********************
Signature generated successfully.

****************** ECC Brainpool verify *******************
Brainpool Signature verified successfully!

****************** Key Delete (ECC Brainpool) *************
Key deleted successfully. Key Pair ID: 0x6ffffffd

****************** Key Generate (RSA PKCS1 V1.5) **********
Key generated successfully. Key Pair ID: 0x6ffffffd

****************** RSA PKCS1 V1.5 sign ********************
Signature generated successfully.

****************** RSA PKCS1 V1.5 verify ******************
RSA Signature verified successfully!

****************** Key Delete (RSA PKCS1 V1.5) ************
Key deleted successfully. Key Pair ID: 0x6ffffffd

****************** Key Generate (RSA PSS) *****************
Key generated successfully. Key Pair ID: 0x6ffffffd

****************** RSA PSS sign ***************************
Signature generated successfully.

****************** RSA PSS verify *************************
RSA Signature verified successfully!

****************** Close Verify service *******************
Verify service close successfully.

****************** Close Sign service *********************
Sign service close successfully.

****************** Close cipher service *******************
Cipher service closed successfully.

****************** Close MAC service **********************
MAC service closed successfully.

****************** Key Management Close *******************
Close Key management service successfully.

****************** Close Key Store ************************
Close Key Store successfully.

****************** Close EdgeLock session *****************
Close session successfully.

End of Example with SUCCESS!!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The IDs will be different on every run.

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ele_crypto/ele_crypto_hsm/example_board_readme.md)
