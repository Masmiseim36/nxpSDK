# caam

## Overview

The CAAM Example project is a demonstration program that uses the KSDK software to encrypt plain text
and decrypt it back using AES algorithm. CBC and GCM modes are demonstrated.
The symmetric key is generated at random, using CAAM's random number generator.

## Running the demo
The log below shows example output of the CAAM AES driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CAAM Peripheral Driver Example

*CAAM Job Ring 0* :

SHA:done successfully.

HMAC-SHA1:done successfully.

HMAC-SHA224:done successfully.

HMAC-SHA256:done successfully.

HMAC-SHA384:done successfully.

HMAC-SHA512:done successfully.

AES CBC: encrypting using 128 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 192 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 256 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC Blacken: encrypting using 128 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 192 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 256 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

*CAAM Job Ring 1* :

AES GCM: encrypt done successfully.
AES GCM: decrypt done successfully.

AES GCM Blacken: encrypt done successfully.
AES GCM Blacken: decrypt done successfully.

*CAAM Job Ring 2* :

AES CBC: encrypting using 128 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 192 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 256 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC Blacken: encrypting using 128 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 192 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 256 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

*CAAM Job Ring 3* :

AES GCM: encrypt done successfully.
AES GCM: decrypt done successfully.

AES GCM Blacken: encrypt done successfully.
AES GCM Blacken: decrypt done successfully.

AES ECB: encrypt done successfully.
AES ECB: decrypt done successfully.

AES ECB Blacken: encrypt done successfully.
AES ECB Blacken: decrypt done successfully.

AES CCM: encrypt done successfully.
AES CCM: decrypt done successfully.

AES CCM Blacken: encrypt done successfully.
AES CCM Blacken: decrypt done successfully.

AES CTR: encrypt done successfully.
AES CTR: decrypt done successfully.

AES CTR Blacken: encrypt done successfully.
AES CTR Blacken: decrypt done successfully.

RNG : Generate 128-bit random number: 0x6484D58AE8D5CC3FD203E61AC849D247
RNG : Random number generated successfully.

CAAM Red Blob Example

Generate Red Blob successfully.

Generate AES Key and blacken it.

Blacken key successfully.

CAAM Black blob Example

Generate black blob successfully.

CRC: done successfully.

Ecc (private key plain): done successfully.

Ecc (private key encrypted): done successfully.

ECC black key confimation: done successfully.

RSA (plain):
Encrypt a secret: done successfully.
Create a signature: done successfully.

RSA (plain):
Encrypt a secret: done successfully.
Create a signature: done successfully.

RSA (blacken):
Encrypt a secret: done successfully.
Create a signature: done successfully.

RSA (blacken):
Encrypt a secret: done successfully.
Create a signature: done successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/caam/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/caam/example_board_readme.md)
