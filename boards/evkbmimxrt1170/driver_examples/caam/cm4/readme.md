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

*CAAM Job Ring 1* :

AES GCM: encrypt done successfully.
AES GCM: decrypt done successfully.

*CAAM Job Ring 2* :

AES CBC: encrypting using 128 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 192 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 256 bit key done successfully.
AES CBC: decrypting back done successfully.

*CAAM Job Ring 3* :

AES GCM: encrypt done successfully.
AES GCM: decrypt done successfully.

RNG : Generate 128-bit random number: 0x18BED894D712F2895DEC91B18C61F6A4
RNG : Random number generated successfully.

CAAM Red Blob Example

Generate Red Blob successfully.

Generate AES Key and blacken it.

Blacken key successfully.

CAAM Black blob Example

Generate black blob successfully.

CRC: done successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/caam/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/caam/example_board_readme.md)
