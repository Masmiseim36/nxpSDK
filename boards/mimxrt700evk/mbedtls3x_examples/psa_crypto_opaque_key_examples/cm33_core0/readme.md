# psa_crypto_opaque_key_examples

## Overview

The PSA crypto opaque key example illustrates the generation and usage of keys with different attributes using PSA API's

The PSA Key Attributes consist of :

PSA Key Identifier 
PSA Key Lifetime 
PSA Key Type 
PSA Key Policies composed of PSA Key Usage and PSA Key Algorithm

PSA Key Lifetime is composed of a location indicator and a persistence level. 
The location indicator identifies whethere the application generates the key in local storage
i.e transparent key or as an opaque key in SoC specicif NXP secure subsystem.

The location of the key is provided as a Kconfig option for the example (TEST_KEY_LOCATION)
For RT1180, default location chosen in example Kconfig is 0x1 (PSA_CRYPTO_ELE_S4XX_LOCATION).
The keys generated in the example for RT1180 are generated within ELE enclave's  key store.
For MCXN and RW612 based devices the location for opaque keys is 0xc00401 which indicates the
keys are generated within ELS50 subsystem keyslots and never leave the ELS50 in plain format.

The example also demonstrated generation of keys with different persistence level. Persistent keys
may be demonstrated if MBEDTLS_PSA_CRYPTO_STORAGE_C is enabled in the config file.

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/mbedtls3x_examples/psa_crypto_opaque_key_examples/example_board_readme.md)