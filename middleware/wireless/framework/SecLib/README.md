# Security Library

## Overview
The framework provides support for cryptography in the security module. It supports both software and hardware encryption. Depending on the device, the hardware encryption uses either the S200, MMCAU, LTC, or CAU3 module instruction set or dedicated AES and SHA hardware blocks. 

Software implementation is provided in a library format.

## Support for security algorithms
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
| ECDSA_P256 hash and msg signature  generation / verifcation 	|                                              	| only if owner of the key pair   	|               	| x                         	| x                       	| Matter                    	|
| SPAKE2+ P256 arithmetics                                    	|                                              	|                                 	|               	| x                         	| x                       	| Matter                    	|