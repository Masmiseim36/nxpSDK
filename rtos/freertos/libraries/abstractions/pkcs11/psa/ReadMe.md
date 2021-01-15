# What is this project

The Amazon FreeRTOS PKCS#11 to PSA shim layer provides a reference implementation of PKCS#11 API based on PSA API.

PSA is Platform Security Architecture which is initiated by Arm. Please get the details from this [link](https://www.arm.com/why-arm/architecture/platform-security-architecture).

Amazon FreeRTOS uses TLS (Transport Layer Security) to secure the connection between the devices and AWS IoT Cloud.
PKCS#11 (Public Key Cryptography Standards) provides some security functions to TLS in Amazon FreeRTOS.
For example, objective storage and signing.

The original PKCS#11 API implementation in Amazon FreeRTOS is based on [mbedTLS](https://github.com/ARMmbed/mbedtls).
This project is the PSA based PKCS#11 API implementation. This is done by adding the shim layer between these two API sets.

# License

Please check the **License.md** file in this project root folder for the detailed license information.

Unless stated otherwise, the software is provided under the [MIT License](https://spdx.org/licenses/MIT.html).

This project contains code derived from [mbedTLS](https://github.com/ARMmbed/mbedtls) local copy in [Amazon FreeRTOS project](https://github.com/aws/amazon-freertos). This code, including modifications, is provided under the [Apache-2.0](https://spdx.org/licenses/Apache-2.0.html) license.

For details:
- `iot_pkcs11_psa_input_format.h` and `iot_pkcs11_psa_input_format.c` are derivatives of
amazon-freertos\libraries\3rdparty\mbedtls\library\pkparse.c (amazon-freertos commit 74875b1d2)