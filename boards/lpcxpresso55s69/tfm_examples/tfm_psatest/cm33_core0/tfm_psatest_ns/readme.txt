Overview
========
The Trusted Firmware M (TF-M) PSA Dev API test application provides verification that secure service API behaviours are implemented correctly.
The PSA Dev API tests are the basis for getting the PSA Functional API Certification.

Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- Keil MDK  5.31
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55s69 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use secure project to download the program to target board
    For IAR environment go to project options/Debugger/Images. Open file dialog for first extra image. Select file <application_name>_ns.out and click on open button. 
    Now you have absolute path in the path box. The flash loader requires absolute path to this file.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the TFM PSA API tests in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[Sec Thread] Secure image initializing!
Booting TFM v1.0

***** PSA Architecture Test Suite - Version 1.0 *****

Running.. Crypto Suite
******************************************

TEST: 201 | DESCRIPTION: Testing psa_crypto_init API: Basic
[Info] Executing tests from non-secure
[Check 1] Test calling crypto functions before psa_crypto_init
TEST RESULT: SKIPPED (Skip Code=0x29)

******************************************

TEST: 202 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_import_key 16 Byte AES
[Check 2] Test psa_import_key 24 Byte AES
[Check 3] Test psa_import_key 32 Byte AES
[Check 4] Test psa_import_key with EC Public key
[Check 5] Test psa_import_key with key data greater than the algorithm size
[Check 6] Test psa_import_key with incorrect key data size
[Check 7] Test psa_import_key with incorrect key type
TEST RESULT: PASSED

******************************************

TEST: 203 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_export_key 16 Byte AES
[Check 2] Test psa_export_key 24 Byte AES
[Check 3] Test psa_export_key 32 Byte AES
[Check 4] Test psa_export_key with EC Public key
[Check 5] Test psa_export_key with key policy verify
[Check 6] Test psa_export_key with less buffer size
TEST RESULT: PASSED

******************************************

TEST: 204 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_export_public_key 16 Byte AES
[Check 2] Test psa_export_public_key 24 Byte AES
[Check 3] Test psa_export_public_key 32 Byte AES
[Check 4] Test psa_export_public_key with EC Public key
TEST RESULT: PASSED

******************************************

TEST: 205 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_destroy_key 16 Byte AES
[Check 2] Test psa_destroy_key 24 Byte AES
[Check 3] Test psa_destroy_key 32 Byte AES
[Check 4] Test psa_destroy_key with EC Public key
TEST RESULT: PASSED

******************************************

TEST: 208 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_setup - ECDH + HKDF-SHA-256
[Check 2] Test psa_key_derivation_setup - ECDH, unknown KDF
[Check 3] Test psa_key_derivation_setup - bad key derivation algorithm
[Check 4] Test psa_key_derivation_setup - Invalid Algorithm
TEST RESULT: PASSED

******************************************

TEST: 210 | DESCRIPTION: Testing crypto key attributes APIs
[Info] Executing tests from non-secure
[Check 1] Test set/get key attributes
TEST RESULT: PASSED

******************************************

TEST: 211 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_setup with SHA256 algorithm
[Check 2] Test psa_hash_setup with Invalid hash algorithm
[Check 3] Test psa_hash_setup with Invalid algorithm
[Check 4] Test psa_hash_setup with CTR algorithm
TEST RESULT: PASSED

******************************************

TEST: 212 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_update with SHA256 algorithm
[Check 2] Test psa_hash_update without hash setup
[Check 3] Test psa_hash_update with completed opertaion handle
TEST RESULT: PASSED

******************************************

TEST: 213 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_verify with SHA256 algorithm
[Check 2] Test psa_hash_verify with incorrect expected hash
[Check 3] Test psa_hash_verify with incorrect hash length
[Check 4] test psa_hash_verify with inactive & invalid operation handle
TEST RESULT: PASSED

******************************************

TEST: 214 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_finish with SHA256 algorithm
[Check 2] test psa_hash_finish with inactive operation handle
[Check 3] test psa_hash_finish with invalid hash buffer size
TEST RESULT: PASSED

******************************************

TEST: 215 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_abort with SHA256 algorithm
[Check 2] Test psa_hash_finish after calling psa_hash_abort
TEST RESULT: PASSED

******************************************

TEST: 216 | DESCRIPTION: Testing crypto generator functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_generate_key 16 Byte AES
[Check 2] Test psa_generate_key 24 Byte AES
[Check 3] Test psa_generate_key 32 Byte AES
[Check 4] Test psa_generate_key with invalid key type
[Check 5] Test psa_generate_key with invalid usage flags
TEST RESULT: PASSED

******************************************

TEST: 217 | DESCRIPTION: Testing crypto generation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_generate_random to get 0 Byte data
[Check 2] Test psa_generate_random to get 16 Byte data
[Check 3] Test psa_generate_random to get 24 Byte data
[Check 4] Test psa_generate_random to get 32 Byte data
[Check 5] Test psa_generate_random to get 64 Byte data
[Check 6] Test psa_generate_random to get 128 Byte data
[Check 7] Test psa_generate_random to get 256 Byte data
[Check 8] Test psa_generate_random to get 512 Byte data
[Check 9] Test psa_generate_random to get 1000 Byte data
TEST RESULT: PASSED

******************************************

TEST: 218 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_input_key 16 Byte Key
[Check 2] Test psa_key_derivation_input_key with invalid usage
[Check 3] Test psa_key_derivation_input_key with step as label
[Check 4] Test psa_key_derivation_input_key with step as info
[Check 5] Test psa_key_derivation_input_key with step as seed
[Check 6] Test psa_key_derivation_input_key with step as salt
[Check 7] Test psa_key_derivation_input_key with key type as AES(not derive)
[Check 8] Test psa_key_derivation_input_key incorrect key algorithm
[Check 9] Test psa_key_derivation_input_key with zero as step
[Check 10] Test psa_key_derivation_input_key invalid handle
TEST RESULT: PASSED

******************************************

TEST: 219 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_key_agreement - KDF instead of a key agreement algorithm
TEST RESULT: PASSED

******************************************

TEST: 222 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_abort
TEST RESULT: PASSED

******************************************

TEST: 223 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_set_get_capacity - Less than operation's capacity
[Check 2] Test psa_key_derivation_set_get_capacity - Equal to operation's capacity
[Check 3] Test psa_key_derivation_set_get_capacity - More than operation's capacity
TEST RESULT: PASSED

******************************************

TEST: 224 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_encrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 2] Test psa_aead_encrypt - AES-CCM
[Check 3] Test psa_aead_encrypt - AES-CCM 24 bytes Tag length = 4
[Check 4] Test psa_aead_encrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 5] Test psa_aead_encrypt - Unsupported Algorithm
[Check 6] Test psa_aead_encrypt - Invalid key usage
[Check 7] Test psa_aead_encrypt - Small output buffer size
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 2] Test psa_aead_decrypt - AES-CCM
[Check 3] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 4] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 5] Test psa_aead_decrypt - Unsupported Algorithm
[Check 6] Test psa_aead_decrypt - Invalid key usage
[Check 7] Test psa_aead_decrypt - Small output buffer size
[Check 8] Test psa_aead_decrypt - Invalid cipher text
[Check 9] Test psa_aead_decrypt - Invalid cipher text size
[Check 10] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 11] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 12] Test psa_aead_decrypt - AES-CCM
[Check 13] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 14] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 15] Test psa_aead_decrypt - Unsupported Algorithm
[Check 16] Test psa_aead_decrypt - Invalid key usage
[Check 17] Test psa_aead_decrypt - Small output buffer size
[Check 18] Test psa_aead_decrypt - Invalid cipher text
[Check 19] Test psa_aead_decrypt - Invalid cipher text size
[Check 20] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 21] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 22] Test psa_aead_decrypt - AES-CCM
[Check 23] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 24] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 25] Test psa_aead_decrypt - Unsupported Algorithm
[Check 26] Test psa_aead_decrypt - Invalid key usage
[Check 27] Test psa_aead_decrypt - Small output buffer size
[Check 28] Test psa_aead_decrypt - Invalid cipher text
[Check 29] Test psa_aead_decrypt - Invalid cipher text size
[Check 30] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 31] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 32] Test psa_aead_decrypt - AES-CCM
[Check 33] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 34] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 35] Test psa_aead_decrypt - Unsupported Algorithm
[Check 36] Test psa_aead_decrypt - Invalid key usage
[Check 37] Test psa_aead_decrypt - Small output buffer size
[Check 38] Test psa_aead_decrypt - Invalid cipher text
[Check 39] Test psa_aead_decrypt - Invalid cipher text size
[Check 40] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 41] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 42] Test psa_aead_decrypt - AES-CCM
[Check 43] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 44] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 45] Test psa_aead_decrypt - Unsupported Algorithm
[Check 46] Test psa_aead_decrypt - Invalid key usage
[Check 47] Test psa_aead_decrypt - Small output buffer size
[Check 48] Test psa_aead_decrypt - Invalid cipher text
[Check 49] Test psa_aead_decrypt - Invalid cipher text size
[Check 50] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 51] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 52] Test psa_aead_decrypt - AES-CCM
[Check 53] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 54] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 55] Test psa_aead_decrypt - Unsupported Algorithm
[Check 56] Test psa_aead_decrypt - Invalid key usage
[Check 57] Test psa_aead_decrypt - Small output buffer size
[Check 58] Test psa_aead_decrypt - Invalid cipher text
[Check 59] Test psa_aead_decrypt - Invalid cipher text size
[Check 60] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 61] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 62] Test psa_aead_decrypt - AES-CCM
[Check 63] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 64] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 65] Test psa_aead_decrypt - Unsupported Algorithm
[Check 66] Test psa_aead_decrypt - Invalid key usage
[Check 67] Test psa_aead_decrypt - Small output buffer size
[Check 68] Test psa_aead_decrypt - Invalid cipher text
[Check 69] Test psa_aead_decrypt - Invalid cipher text size
[Check 70] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 232 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_encrypt_setup 16 Byte AES
[Check 2] Test psa_cipher_encrypt_setup 24 Byte AES
[Check 3] Test psa_cipher_encrypt_setup 32 Byte AES
[Check 4] Test psa_cipher_encrypt_setup 16 Byte raw data
[Check 5] Test psa_cipher_encrypt_setup - not a cipher algorithm
[Check 6] Test psa_cipher_encrypt_setup - unknown cipher algorithm
[Check 7] Test psa_cipher_encrypt_setup - incorrect usage
[Check 8] Test psa_cipher_encrypt_setup - EC Public key
[Check 9] Test psa_cipher_encrypt_setup - Invalid key handle
[Check 10] Test psa_cipher_encrypt_setup - Zero as key handle
TEST RESULT: PASSED

******************************************

TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_decrypt_setup 16 Byte AES
[Check 2] Test psa_cipher_decrypt_setup 24 Byte AES
[Check 3] Test psa_cipher_decrypt_setup 32 Byte AES
[Check 4] Test psa_cipher_decrypt_setup 16 Byte raw data
[Check 5] Test psa_cipher_decrypt_setup - not a cipher algorithm
[Check 6] Test psa_cipher_decrypt_setup - unknown cipher algorithm
[Check 7] Test psa_cipher_decrypt_setup - incorrect usage
[Check 8] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 9] Test psa_cipher_decrypt_setup - Zero as key handle
TEST RESULT: PASSED

******************************************

TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 10] Test psa_cipher_decrypt_setup 16 Byte AES
[Check 11] Test psa_cipher_decrypt_setup 24 Byte AES
[Check 12] Test psa_cipher_decrypt_setup 32 Byte AES
[Check 13] Test psa_cipher_decrypt_setup 16 Byte raw data
[Check 14] Test psa_cipher_decrypt_setup - not a cipher algorithm
[Check 15] Test psa_cipher_decrypt_setup - unknown cipher algorithm
[Check 16] Test psa_cipher_decrypt_setup - incorrect usage
[Check 17] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 18] Test psa_cipher_decrypt_setup - Zero as key handle
TEST RESULT: PASSED

******************************************

TEST: 235 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_set_iv 16 Byte AES
[Check 2] Test psa_cipher_set_iv 24 Byte AES
[Check 3] Test psa_cipher_set_iv 32 Byte AES
[Check 4] Test psa_cipher_set_iv AES - small iv buffer
[Check 5] Test psa_cipher_set_iv AES - large iv buffer
TEST RESULT: PASSED

******************************************

TEST: 236 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_update - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_update - Encrypt - AES CBC_NO_PADDING (Short input)
[Check 3] Test psa_cipher_update - Encrypt - AES CTR
[Check 4] Test psa_cipher_update - small output buffer size
[Check 5] Test psa_cipher_update - Decrypt - AES CBC_NO_PADDING
[Check 6] Test psa_cipher_update - Decrypt - AES CBC_NO_PADDING (Short input)
[Check 7] Test psa_cipher_update - Decrypt - AES CTR
[Check 8] Test psa_cipher_update without cipher setup
TEST RESULT: PASSED

******************************************

TEST: 237 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_finish - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_finish - Encrypt - AES CBC_NO_PADDING (Short input)
[Check 3] Test psa_cipher_finish - Encrypt - AES CTR
[Check 4] Test psa_cipher_finish - Encrypt - AES CTR (short input)
[Check 5] Test psa_cipher_finish - Decrypt - AES CBC_NO_PADDING
[Check 6] Test psa_cipher_finish - Decrypt - AES CBC_NO_PADDING (Short input)
[Check 7] Test psa_cipher_finish - Decrypt - AES CTR
[Check 8] Test psa_cipher_finish - Decrypt - AES CTR (short input)
TEST RESULT: PASSED

******************************************

TEST: 238 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_abort - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_abort - Encrypt - AES CTR
[Check 3] Test psa_cipher_abort - Decrypt - AES CBC_NO_PADDING
[Check 4] Test psa_cipher_abort - Decrypt - AES CTR
[Check 5] Test psa_cipher_update after psa_cipher_abort should fail
TEST RESULT: PASSED

******************************************

TEST: 239 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
No test available for the selected crypto configuration
TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 240 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
No test available for the selected crypto configuration
TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 241 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_asymmetric_sign - ECDSA SECP256R1 SHA-256
TEST RESULT: PASSED

******************************************

TEST: 242 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_asymmetric_verify - ECDSA KEY_PAIR SECP256R1 SHA-256
[Check 2] Test psa_asymmetric_verify - EC public key
[Check 3] Test psa_asymmetric_verify - Wrong signature size
TEST RESULT: PASSED

******************************************

TEST: 243 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_raw_key_agreement - Not a key agreement alg
TEST RESULT: PASSED

******************************************

TEST: 244 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_copy_key 16 Byte AES
[Check 2] Test psa_copy_key without copy usage
[Check 3] Test psa_copy_key 24 Byte AES
[Check 4] Test psa_copy_key 32 Byte AES
[Check 5] Test psa_copy_key with EC Public key
[Check 6] Test psa_copy_key with Incompatible target policy
TEST RESULT: PASSED

******************************************

TEST: 245 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_clone with SHA256 algorithm
TEST RESULT: PASSED

******************************************

************ Crypto Suite Report **********
TOTAL TESTS     : 40
TOTAL PASSED    : 37
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 3
******************************************

Running.. Attestation Suite
******************************************

TEST: 801 | DESCRIPTION: Testing initial attestation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_initial_attestation_get_token with Challenge 32
[Check 2] Test psa_initial_attestation_get_token with Challenge 48
[Check 3] Test psa_initial_attestation_get_token with Challenge 64
[Check 4] Test psa_initial_attestation_get_token with zero challenge size
[Check 5] Test psa_initial_attestation_get_token with small challenge size
[Check 6] Test psa_initial_attestation_get_token with invalid challenge size
[Check 7] Test psa_initial_attestation_get_token with large challenge size
[Check 8] Test psa_initial_attestation_get_token with zero as token size
[Check 9] Test psa_initial_attestation_get_token with small token size
[Check 10] Test psa_initial_attestation_get_token_size with Challenge 32
[Check 11] Test psa_initial_attestation_get_token_size with Challenge 48
[Check 12] Test psa_initial_attestation_get_token_size with Challenge 64
[Check 13] Test psa_initial_attestation_get_token_size with zero challenge size
[Check 14] Test psa_initial_attestation_get_token_size with small challenge size
[Check 15] Test psa_initial_attestation_get_token_size with invalid challenge size
[Check 16] Test psa_initial_attestation_get_token_size with large challenge size
TEST RESULT: PASSED

******************************************

************ Attestation Suite Report **********
TOTAL TESTS     : 1
TOTAL PASSED    : 1
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 0
******************************************

Running.. Protected Storage Suite
******************************************

TEST: 401 | DESCRIPTION: UID not found check
[Info] Executing tests from non-secure
[Check 1] Call get API for UID 6 which is not set
[Check 2] Call get_info API for UID 6 which is not set
[Check 3] Call remove API for UID 6 which is not set
[Check 4] Call get API for UID 6 which is removed
[Check 5] Call get_info API for UID 6 which is removed
[Check 6] Call remove API for UID 6 which is removed
Set storage for UID 6
[Check 7] Call get API for different UID 5
[Check 8] Call get_info API for different UID 5
[Check 9] Call remove API for different UID 5
TEST RESULT: PASSED

******************************************

TEST: 402 | DESCRIPTION: Write once error check
[Info] Executing tests from non-secure
[Check 1] Update the flag of UID 1 with WRITE_ONCE flag
[Check 2] Try to remove the UID 1 having WRITE_ONCE flag
[Check 3] Create a new UID 2 with WRITE_ONCE flag
[Check 4] Try to remove the UID 2 having WRITE_ONCE flag
[Check 5] Try to change the length of write_once UID 2
[Check 6] Check UID removal still fails
[Check 7] Try to change the WRITE_ONCE flag to None for UID 2
[Check 8] Check UID removal still fails
TEST RESULT: PASSED

******************************************

TEST: 403 | DESCRIPTION: Insufficient space check
[Info] Executing tests from non-secure
[Check 1] Overload storage space
UID 7 set failed due to insufficient space
Remove all registered UIDs
[Check 2] Overload storage again to verify all previous UID removed
UID 7 set failed due to insufficient space
Remove all registered UIDs
TEST RESULT: PASSED

******************************************

TEST: 404 | DESCRIPTION: Data Consistency check
[Info] Executing tests from non-secure
[Check 1] Call set API with reduced length - TEST_BUFF_SIZE/2
[Check 2] Call get API with default length - TEST_BUFF_SIZE
TEST RESULT: PASSED

******************************************

TEST: 405 | DESCRIPTION: Success scenarios check
[Info] Executing tests from non-secure
[Check 1] Set UID with data length zero and call storage APIs
[Check 2] Resetting the length check
TEST RESULT: PASSED

******************************************

TEST: 406 | DESCRIPTION: Check for storage create flags
[Info] Executing tests from non-secure
[Check 1] Call set API with flag - PSA_STORAGE_FLAG_NONE
[Check 2] Call set API with flag - PSA_STORAGE_FLAG_NO_CONFIDENTIALITY
[Check 3] Call set API with flag - PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION
TEST RESULT: PASSED

******************************************

TEST: 407 | DESCRIPTION: Incorrect Size check
[Info] Executing tests from non-secure
Create a valid Storage - TEST_BUFF_SIZE/2
Increase the length of storage - TEST_BUFF_SIZE
[Check 1] Call get API with old length - TEST_BUFF_SIZE/2
[Check 2] Call get API with old length - TEST_BUFF_SIZE/4
Decrease the length of storage - TEST_BUFF_SIZE/4
[Check 3] Call get API with old length - TEST_BUFF_SIZE/2
[Check 4] Call get API with old length - TEST_BUFF_SIZE
[Check 5] Call get API with valid length - TEST_BUFF_SIZE/4
TEST RESULT: PASSED

******************************************

TEST: 408 | DESCRIPTION: Invalid offset check
[Info] Executing tests from non-secure
[Check 1] Try to access data with varying valid offset
[Check 2] Try to access data with varying invalid offset
TEST RESULT: PASSED

******************************************

TEST: 409 | DESCRIPTION: Invalid Arguments check
[Info] Executing tests from non-secure
[Check 1] Call set API with NULL pointer and data length 0
[Check 2] Call get API with NULL read buffer and data length 0
[Check 3] Remove the UID
[Check 4] Call get_info API to verify UID removed
[Check 5] Create UID with zero data_len and valid write buffer
[Check 8] Call get API with NULL read buffer and data length 0
[Check 9] Increase the length
TEST RESULT: PASSED

******************************************

TEST: 410 | DESCRIPTION: UID value zero check
[Info] Executing tests from non-secure
[Check 1] Creating storage with UID 0 should fail
[Check 2] Get_info for UID 0 should fail
[Check 3] Removing storage with UID 0 should fail
TEST RESULT: PASSED

******************************************

TEST: 411 | DESCRIPTION: Optional APIs: UID not found check
[Info] Executing tests from non-secure
Test Case skipped as Optional PS APIs are not supported.
TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 412 | DESCRIPTION: Optional APIs: Invalid arguments and offset invalid
[Info] Executing tests from non-secure
Test Case skipped as Optional PS APIs are not supported.
TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 413 | DESCRIPTION: Set_Extended and Create api : Success
[Info] Executing tests from non-secure
Test Case skipped as Optional PS APIs are not supported.
TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 414 | DESCRIPTION: Optional APIs not supported check
[Info] Executing tests from non-secure
Optional PS APIs are not supported.
[Check 1] Call to create API should fail as API not supported
[Check 2] Create valid storage with set API
[Check 3] Call to set_extended API call should fail
[Check 4] Verify data is unchanged
TEST RESULT: PASSED

******************************************

TEST: 415 | DESCRIPTION: Create API write_once flag value check
[Info] Executing tests from non-secure
Test Case skipped as Optional PS APIs are not supported.
TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 416 | DESCRIPTION: Storage assest capacity modification check
[Info] Executing tests from non-secure
Test Case skipped as Optional PS APIs not are supported.
TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 417 | DESCRIPTION: Storage assest capacity modification check
[Info] Executing tests from non-secure
Test Case skipped as Optional PS APIs not are supported.
TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

************ Protected Storage Suite Report **********
TOTAL TESTS     : 17
TOTAL PASSED    : 11
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 6
******************************************

Entering standby..
                             
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

