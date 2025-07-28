# SecLib_RNG: Security library and random number generator
## Random number generator
### Overview
The RNG module is part of the framework used for random number generation. It uses hardware RNG peripherals as entropy sources (TRNG, Secure Subsystem, ...) to provide a true random number generator interface.
A Pseudo-Random number generator (PRNG) implementation is available. The PRNG may depend of SecLib services (thus requiring a common mutex) to perform HMAC-SHA256, SHA256, AES-CTR, or alternateively a Lehmer Linear Congruential generator. A prerequisite for the PRNG to function with desired randomness is to be seeded using a proper source of entropy.
If no hardware acceleration is present, the RNG may fallback to lesser quality ad-hoc source e.g  if present SIM_UID registers, the UIDL is used as the initial seed for the random number generator.

### Initialization
The RNG module requires an initialization via a call to RNG_Init. The RNG initialization involves a call to RNG_SetSeed.

In the case of a dual core system consisting of a Host core and an NBU core, the Secure Subsystem is owned by the Host core. The Host core then has a direct access to its TRNG embedded in its secure subssystem.
On the NBU code side, a request is emitted via RPMSG to the Host to provide a seed. On receipt of this request, the Host sets a 'reseed needed' flag (from the ISR context)
If the core running the RNG service owns the TRNG entropy hardware (if any), it can get the seed directly form this hardware synchronously.
In the case of an NBU that does not control the devices entropy source, that is owned by the Host, it request a seed from the Host processor via RPMSG exchange.
On receipt of this request the Host sets a flag notifying of this request from the RPMSG ISR context.
From the Idle thread, this flag is polled via the RNG_IsReseedNeeded API. If set the seed is regenerated and forwarded to the NBU via RPMSG.

RNG_ReInit API is to be used at wake up time in the context of LowPower.
RNG_DeInit is used for unit tests and coverage purposes but has no useful role in a real application.

### Seed handling
RNG_SetSeed: 
RNG_SetExternalSeed may be used to inject application entropy to RNG context seed using a supplied array of bytes.
RNG_IsReseedNeeded used from task in Host core to check whether seed must be sent to NBU core.


RNG_GetTrueRandomNumber is the API used to generate a Random 32 bit number from a HW source of entropy. It is essential if only to seed the pseudo random number generator.

RNG_GetPseudoRandomData is used to generate arrays of random bytes.

## Security Library

### Overview
The framework provides support for cryptography in the security module. It supports both software and hardware encryption. Depending on the device, the hardware encryption uses either the S200, MMCAU, LTC, or CAU3 module instruction set or dedicated AES and SHA hardware blocks.

Software implementation is provided in a library format.

### Support for security algorithms
|                                                             	| SW Seclib : SecLib.c                         	| EdgeLock SecLib_sss.c           	| Seclib_ecdh.c 	| Mbedtls SecLib_mbedtls.c  	| nccl (part of SecLib.c) 	| Usage example             	|
|-------------------------------------------------------------	|----------------------------------------------	|---------------------------------	|---------------	|---------------------------	|-------------------------	|---------------------------	|
| AES_128                                                     	| SecLib_aes.c                                 	| x                               	|               	| x                         	|                         	|                           	|
| AES_128_ECB                                                 	|                                              	| x                               	|               	| x                         	|                         	|                           	|
| AES_128_CBC                                                 	| x                                            	| x                               	|               	| x                         	|                         	|                           	|
| AES_128_CTR encryption                                      	| x                                            	| x                               	|               	|                           	|                         	|                           	|
| AES_128_OFB Encryption                                      	| x                                            	|                                 	|               	|                           	|                         	|                           	|
| AES_128_CMAC                                                	| x                                            	| x                               	|               	| x                         	|                         	| BLE connection, ieee 15.4 	|
| AES_128_EAX                                                 	| x                                            	|                                 	|               	|                           	|                         	|                           	|
| AES_128_CCM                                                 	| x                                            	| x                               	|               	| x                         	|                         	| BLE, ieee 15.4            	|
| SHA1                                                        	| SecLib_sha.c                                 	| x                               	|               	| x                         	|                         	|                           	|
| SHA256                                                      	| x                                            	| x                               	|               	| x                         	|                         	|                           	|
| HMAC_SHA256                                                 	| x                                            	| x                               	|               	| x                         	|                         	| PRNG, Digest for Matter   	|
| ECDH_P256 shared  secret generation                         	| x (by 15 incremental steps) -> SecLib_ecdh.c 	| x   with MACRO SecLibECDHUseSSS 	| x             	| x                         	| x                       	| BLE pairing,              	|
| EC_P256 key pair  generation                                	| x                                            	| x                               	| x             	| x                         	| x                       	|                           	|
| EC_P256 public key generation from private key              	|                                              	|                                 	| x             	| x                         	| x                       	| Matter (ECDSA)            	|
| ECDSA_P256 hash and msg signature  generation / verification 	|                                              	| only if owner of the key pair   	|               	| x                         	| x                       	| Matter                    	|
| SPAKE2+ P256 arithmetics                                    	|                                              	|                                 	|               	| x                         	| x                       	| Matter                    	|


## BLE advanced secure mode
### New elements in existing structures:
computeDhKeyParam_t::keepInternalBlob - boolean telling if the shared blob is kept in this structure(in .outpoint) after ECDH_P256_ComputeDhKey() or ECDH_P256_ComputeDhKeySeg() call.

### New arguments in existing functions:
ECDH_P256_ComputeDhKey
keepBlobDhKey - boolean telling ECDH_P256_ComputeDhKey() or ECDH_P256_ComputeDhKeySeg() to keep the shared object after computation for later use (it is required by the SecLib_GenerateBluetoothF5KeysSecure).

### New macros:
gSecLibSssUseEncryptedKeys_d - Enable or disable S200 blobs SecLib support. 0 - the Bluetooth Keys are available in plaintext, 1 - the Bluetooth Keys are not available in plaintext, but in secured blobs. Default is disabled.


### New functions:
#### LE Secure connections pairing:
##### void ECDH_P256_FreeDhKeyDataSecure

  This is a function used to free the shared object stored in computeDhKeyParam_t. When user calls ECDH_P256_ComputeDhKeySeg() with keepBlobDhKey set to 1, it should also call **ECDH_P256_FreeDhKeyDataSecure**
.

##### SecLib_GenerateBluetoothF5Keys
  This function is extracted from the Bluetooth LE Host Stack implementation. This corresponds to the legacy implementation without key blobs.

##### SecLib_GenerateBluetoothF5KeysSecure
  Similar to **SecLib_GenerateBluetoothF5Keys** this function is modified to work with key blobs, the reason is to not use SSS inside the Bluetooth LE Host Stack.
##### SecLib_DeriveBluetoothSKD
  This is a helper function used by the Bluetooth LE Host Stack in the pairing procedure, when receiving the vendor HCI command specifying that the ESK needs to be provided to LL.
##### ELKE_BLE_SM_F5_DeriveKeys
  This is a private function, helper for **SecLib_GenerateBluetoothF5KeysSecure**. It was provided by the STEC team.

#### Privacy:

##### SecLib_ObfuscateKeySecure
  This is a function used by the Bluetooth LE Host Stack to obfuscate the IRK before setting it to Bluetooth LE Controller or before saving it to NVM
##### SecLib_DeobfuscateKeySecure
  This is a function used by the Bluetooth LE Host Stack to extract the plaintext IRK key from the saved NVM blob.
##### SecLib_VerifyBluetoothAh
  This function is extracted from the legacy Bluetooth LE Host Stack implementation using plaintext keys.

##### SecLib_VerifyBluetoothAhSecure
   Similar to  **SecLib_VerifyBluetoothAh** with modification to work with S200 key blob.
##### SecLib_GenerateSymmetricKey
  This is a function used by the application to generate the local IRK and local CSRK.
###### SecLib_GenerateBluetoothEIRKBlobSecure
  This is a function used by the application to generate the EIRK needed by Bluetooth LE Controller from the IRK blob.

#### A2B feature
#### ECDH_P256_ComputeA2BKey
  This function is used to compute the EdgeLock to EdgeLock key. pInPeerPublicKey points to the peer public key, pOutE2EKey is the pointer to where the E2E key object will be stored, this will be freed by the application when it is no longer required by calling ECDH_P256_FreeE2EKeyData().
##### ECDH_P256_FreeE2EKeyData
  This function is used to free the key object given as a parameter. It is used by the application to free the E2E key when is no longer needed.
##### SecLib_ExportA2BBlobSecure
  This function is used to import an ELKE blob or plain text symmetric key in s200 and export an E2E key blob. The input type is identified by the keyType parameter.
##### SecLib_ImportA2BBlobSecure
  This function is used to import an E2E key blob in s200 and export an ELKE blob or plain text symmetric key. The output type is identified by the keyType parameter.

### LE Secure connections Pairing flow and SecLib usage:
 1. Each device needs to generate locally the public+private keypair. This is done using **ECDH_P256_GenerateKeys**.
 2. Devices exchange their public keys.
 3. Upon receiving the peer device's public key, local device is computing DH key using **ECDH_P256_ComputeDhKey**.
 4. Each device sends DHKeyCheck packet
 5. Upon receiving DhKeyCheck each device computes LTK blob using **SecLib_GenerateBluetoothF5Keys**
 6. After computing the each device sends HCI_LeStartEnc (on initiator), HCI_Le_Provide_Long_Term_Key (on responder)
 7. Bluetooth LE Controller sends back SKD report custom event
 8. Bluetooth LE Host Stack computes ESKD based on LTK blob using **SecLib_DeriveBluetoothSKD** and sends it to Bluetooth LE Controller
 9. Bluetooth LE Controller encrypts the link

### IRK flow and SecLib usage:
 1. At startup, when gInitializationComplete_c event is received:
   - the local IRK is generated using **SecLib_GenerateSymmetricKey**
   - the local EIRK is generated using **SecLib_GenerateBluetoothEIRKBlobSecure**
   - local CSRK is generated using **SecLib_GenerateSymmetricKey**
 2. During legacy pairing when receiving bonding keys, IRK is obfuscated using **SecLib_ObfuscateKeySecure** and stored
 3. When app wants to set the OOB keys using Gap_SaveKeys the IRK is obfuscated using **SecLib_ObfuscateKeySecure**
 4. When application calls API Gap_VerifyPrivateResolvableAddress IRK is obfuscated using **SecLib_ObfuscateKeySecure** and verified using **SecLib_VerifyBluetoothAhSecure**
 5. When a new connection is received in Host with RPA address not resolved by the Bluetooth LE Controller, the Host tries to resolve it by obfuscating it using 
    **SecLib_ObfuscateKeySecure** and verifying it using **SecLib_VerifyBluetoothAhSecure**
 6. When adding a peer in Bluetooth LE Controller resolving list, the peer's IRK is obfuscated using **SecLib_ObfuscateKeySecure** before setting it using **HCI_Le_Add_Device_To_Resolving_List**.
 7. When an IRK plaintext is requested by the application using Gap_LoadKeys it is obtained using **SecLib_DeobfuscateKeySecure**
 8. When legacy pairing completes and LTK needs to be send in the pairing complete event (gConnEvtPairingComplete_c) the **SecLib_DeobfuscateKey** is used to extract the plaintext.
 
### A2B flow and SecLib usage:
 1. At startup, when gInitializationComplete_c event is received, the application will call **ECDH_P256_GenerateKeys** to generate the public/private key pair required for the E2E key derivation and send the public key to the peer device.
 2. When the public key is received from the peer device, the application will call **ECDH_P256_ComputeA2BKeySecure** to generate the EdgeLock to EdgeLock key.
 3. The application will obtain an E2E IRK blob by calling **SecLib_ExportA2BBlobSecure** with key type gSecElkeBlob_c. The obtained blob is sent to the peer anchor. The peer anchor will call **SecLib_ImportA2BBlob** with keyType gSecElkeBlob_c and save the resulting ELKE blob in NVM, for Digital Key both anchors must have the same IRK.
 4. After pairing, in order to send the LTK and IRK contained in the bonding data securely, the application will call **SecLib_ExportA2BBlobSecure** with keyType gSecLtkElkeBlob_c for the LTK, and **SecLib_ExportA2BBlobSecure** with keyType gSecPlainText_c for the IRK. The E2E blobs obtained are sent along with the rest of the bonding data to the peer anchor device.
 5. After the bonding data is trasfered the E2E key is no longer needed and **ECDH_P256_FreeE2EKeyData** is called with the key object obtained at step 2 when **ECDH_P256_ComputeA2BKeySecure** was called.