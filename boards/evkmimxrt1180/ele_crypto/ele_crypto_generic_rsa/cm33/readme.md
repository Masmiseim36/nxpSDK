# ele_crypto_generic_rsa

## Overview
The ELE Crypto Generic Example project is a demonstration program that uses the MCUX SDK
software to perform generic crypto operations usign plaintext key with EdgeLock Enclave (ELE).

## Running the demo
Example output on terminal:

EdgeLock Enclave secure Sub-System Driver Example:

*********** Load EdgeLock FW ******************************
EdgeLock FW loaded and authenticated successfully.

****************** Start RNG ******************
EdgeLock RNG Start success.
EdgeLock RNG ready to use.

*********** Generic Crypto RSA keygen *********************
RSA Key-Pair generation.
RSA Key-pair generated successfully.

*********** Generic Crypto RSA PKCS#1.5 Signature gen *****
RSA signature with input as digest generated successfully.
RSA signature with input as message generated successfully.

*********** Generic Crypto RSA PSS Signature gen **********
RSA signature generated successfully.

*********** Generic Crypto RSA 2048 PKCS#1.5 Verify *******
RSA verify success

*********** Generic Crypto RSA 3072 PSS Verify ************
RSA verify success

*********** Generic Crypto RSA PKCS#1.5 encrypt ***********
RSA encryption success.

*********** Generic Crypto RSA PKCS#1.5 decrypt ***********
RSA decryption success, plaintext match original one.

*********** Generic Crypto RSA OAEP encrypt ***************
RSA encryption success.

*********** Generic Crypto RSA OAEP decrypt ***************
RSA decryption success, plaintext match original one.

End of Example with SUCCESS!!

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ele_crypto/ele_crypto_generic_rsa/example_board_readme.md)
