########################################
Trusted Firmware-M Profile Medium Design
########################################

:Authors: David Hu
:Organization: Arm Limited
:Contact: david.hu@arm.com

************
Introduction
************

Compared with Profile Small, Profile Medium aims to securely connect devices to
Cloud services with asymmetric cipher support.
Profile Medium target devices need more resources for more cipher algorithms
and higher isolation levels.

For more descriptions and background of TF-M Profile, please refer to Profile
Small design document [PROFILE-S]_.

**************
Overall design
**************

TF-M Profile Medium defines the following feature set:

    - Firmware Framework

        - Inter-Process Communication (IPC) model [PSA-FF-M]_
        - Isolation level 2 [PSA-FF-M]_

    - Internal Trusted Storage (ITS)

    - Crypto

        - Support both symmetric ciphers and asymmetric ciphers
        - Asymmetric key based cipher suite suggested in TLS/DTLS profiles for
          IoT [RFC7925]_ and CoAP [RFC7252]_, including

            - Authenticated Encryption with Associated Data (AEAD) algorithm
            - Asymmetric key algorithm based signature and verification
            - Public-key cryptography based key exchange
            - Hash function
            - HMAC for default Pseudorandom Function (PRF)

        - Asymmetric digital signature and verification for Initial Attestation
          Token (IAT)

    - Initial Attestation

        - Asymmetric key algorithm based Initial Attestation

    - Lightweight boot

        - Anti-rollback protection
        - Multiple image boot

    - Protected Storage (PS) if off-chip storage device is integrated

        - Data confidentiality
        - Data integrity
        - Rollback protection

**************
Design details
**************

More details of TF-M Profile Medium design are described in following sections.

Firmware framework
==================

Profile Medium with IPC model and isolation level 2 aims to support usage
scenarios which require more complicated secure service model and additional
protection to PSA RoT.

Level 2 isolation
-----------------

Profile Medium selects isolation level 2 by default. In addition to isolation
level 1, the PSA Root of Trust (PSA RoT) is also protected from access by the
Application Root of Trust (App RoT) in level 2 isolation.

IPC model
---------

Profile Medium enables IPC model by default. IPC model can achieve a more
flexible framework and higher levels of isolation, but may require more memory
footprint and bring in longer latency, compared to Library model.

TF-M IPC model implementation follows the PSA Firmware Framework for M
(PSA-FF-M) [PSA-FF-M]_.

Crypto service
==============

Compared to Profile Small, Profile Medium includes asymmetric cipher to support
direct connection to Cloud services via common protocols, such as TLS/DTLS 1.2.

As suggested in CoAP [RFC7252]_ and [RFC7925]_, TF-M Profile Medium by default
selects ``TLS_ECDHE_ECDSA_WITH_AES_128_CCM`` as reference, which requires:

    - ECDHE_ECDSA as key exchange algorithm.
    - AES-128-CCM (AES CCM mode with 128-bit key) as AEAD algorithm.
      Platforms can implement AES-128-CCM with truncated authentication tag to
      achieve less network bandwidth [RFC7925]_.
    - SHA256 as Hash function.
    - HMAC as Message Authentication Code algorithm.

Applications can also support TLS PSK [RFC4279]_ cipher suites, such as
``TLS_PSK_WITH_AES_128_CCM`` [RFC7925]_.

.. note ::

    **Implementation note**

    Developers can replace default algorithms with others or implement more
    algorithms according to actual usage scenarios and device capabilities.

    If a Crypto hardware accelerator is integrated, the cipher suites and
    algorithms also depend on those accelerator features.

More details of cipher suite are described below.

Digital signature and verification
----------------------------------

ECDSA is selected by default in Profile Medium.
ECDSA requires much shorter keys compared with RSA at the same security level.
Therefore, ECDSA can cost less storage area for assets and less network
bandwidth to setup a TLS connection.
ECDSA is also preferred for forward compatibility of future TLS versions.

As requested in [RFC7251]_, ECC curve ``secp256r1`` should be supported. More
ECC curves can be added based on the requirements in production.

If usage scenarios require RSA algorithm for backward compatibility and legacy
applications, platforms can add RSA support or replace ECDSA with RSA. The
cipher suite should be switched accordingly.

AEAD algorithm
--------------

If Protected Storage (PS) is implemented, it is recommended to select the same
AEAD algorithm for PS service as the one used by TLS/DTLS cipher suite.

Internal Trusted Storage
========================

The configuration of ITS is the same as those in Profile Small [PROFILE-S]_.

Lightweight boot
================

BL2 implementation can be device specific. Devices may implement diverse
boot processes with different features and configurations.
However, the boot loader must support anti-rollback protection. Boot loader must
be able to prevent unauthorized rollback, to protect devices from being
downgraded to earlier versions with known vulnerabilities.

MCUBoot in TF-M is configured as multiple image boot by default in Profile
Medium. In multiple image boot, secure and non-secure images can be signed
independently with different keys and they can be updated separately. It can
support multiple vendors scenarios, in which non-secure and secure images are
generated and updated by different vendors.
Multiple image boot may require more storage area compared with single image
boot.

Protected Storage
=================

PS service is required if an off-chip storage device is integrated and used on
the platform.

TF-M PS service relies on an AEAD algorithm to ensure data confidentiality and
integrity. It is recommended to select the same AEAD algorithm as the one used
for TLS/DTLS cipher suite.

Anti-rollback protection in PS relies on non-volatile counter(s) provided by
TF-M Platform Secure Partition (SP).

TF-M audit logging service
==========================

TF-M audit logging service allows secure services in the system to log critical
system events and information.

TF-M audit logging service is not enabled in Profile Medium since its IPC model
dedicated interface is not ready yet.

.. note ::

    **Implementation note**

    Please note that there is no dedicated PSA specification for Audit Logging
    yet.
    The design, interfaces and implementation of TF-M audit logging service may
    change.

**************
Implementation
**************

Overview
========

The basic idea is to add dedicated top-level CMake configuration files under
folder ``configs`` for TF-M Profile Medium default configuration, as the same as
Profile Small does.

The top-level Profile Medium config file collects all the necessary
configuration flags and set them to default values, to explicitly enable the
features required in Profile Medium and disable the unnecessary ones, during
TF-M build.

A platform/use case can provide a configuration extension file to overwrite
Profile Medium default setting and append other configurations.
This configuration extension file can be added via parameter
``TFM_PROFILE_CONFIG_EXT`` in build command line. The top-level config file will
include the device configuration extension file to load platform/use case
specific configurations.

The overall build flow of Profile Medium is shown as the flowchart below.

.. uml::

    @startuml

    title Overall build flow

    start

    :Profile Medium CMake file;
    note left
        Top-level CMake config file under ""configs"".
        Set configurations to default values.
    endnote

    if (Platform config\nextension specified?) then (Yes)
        :Include platform specific\nconfig extension file;
        note left
            Platform specific configuration extension file
            is provided via ""TFM_PROFILE_CONFIG_EXT"" in
            build command line.
        endnote

        :Overwrite default configurations;
    else (No)
    endif

    :CommonConfig.cmake;
    note left
        Normal building sequence
    endnote

    stop

    @enduml

The details of configurations will be covered in each module in
`Implementation details`_.

Implementation details
======================

This section discusses the details of Profile Medium implementation.

Top-level configuration files
-----------------------------

Profile Medium provides ``ConfigDefaultProfileM`` as a default top-level
configuration file without test cases.

The firmware framework configurations in ``ConfigDefaultProfileM`` are shown
below.

.. table:: Config flags in Profile Medium top-level CMake config file
   :widths: auto
   :align: center

   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | Configs                                    | Default value                       | Descriptions                        |
   +============================================+=====================================+=====================================+
   | ``TFM_LVL``                                | ``2``                               | Select level 2 isolation            |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``CORE_IPC``                               | ``True``                            | Select IPC model                    |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_INTERNAL_TRUSTED_STORAGE`` | ``ON``                              | Enable ITS SP                       |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``ITS_BUF_SIZE``                           | ``32``                              | ITS internal transient buffer size  |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_CRYPTO``                   | ``ON``                              | Enable Crypto service               |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``MBEDTLS_CONFIG_FILE``                    | ``tfm_profile_m_mbedcrypto_config`` | Default Mbed Crypto config file for |
   |                                            |                                     | Profile Medium under                |
   |                                            |                                     | ``platform/ext/common``             |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_INITIAL_ATTESTATION``      | ``ON``                              | Enable Initial Attestation service  |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_PROTECTED_STORAGE`` [1]_   | ``ON``                              | Enable PS service                   |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_PLATFORM``                 | ``ON``                              | Enable TF-M Platform SP             |
   +--------------------------------------------+-------------------------------------+-------------------------------------+
   | ``TFM_PARTITION_AUDIT_LOG``                | ``OFF``                             | Disable TF-M audit logging service  |
   +--------------------------------------------+-------------------------------------+-------------------------------------+

.. [1] PS service is enabled by default. Platforms without off-chip storage
       devices can turn off ``TFM_PARTITION_PROTECTED_STORAGE`` to disable PS
       service. See `Protected Storage Secure Partition`_ for details.

Profile Medium provides ``ConfigRegressionProfileM`` as the default top-level
configuration file with regression tests.

The table below collects ``ConfigRegressionProfileM`` major configurations
which are not covered or different from those in ``ConfigDefaultProfileM``.

.. table:: Major config flags in Profile Medium top-level CMake config file with tests
   :widths: auto
   :align: center

   +------------------+---------------+------------------------+
   | Configs          | Default value | Descriptions           |
   +==================+===============+========================+
   | ``REGRESSION``   | ``ON``        | Enable Regression test |
   +------------------+---------------+------------------------+
   | ``CORE_TEST``    | ``ON``        | Enable Core test       |
   +------------------+---------------+------------------------+
   | ``IPC_TEST``     | ``ON``        | Enable IPC test        |
   +------------------+---------------+------------------------+
   | ``PSA_API_TEST`` | ``OFF``       | Disable PSA API test   |
   +------------------+---------------+------------------------+

The test cases should be disabled in ``ConfigRegressionProfileM`` if the
corresponding feature is not selected in Profile Medium by default.

Device configuration extension
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To change default configurations and add platform specific configurations,
a platform can set the path to its own configuration extension file in parameter
``TFM_PROFILE_CONFIG_EXT`` in command line.

A platform can also add its device specific configurations into its specific
CMake file under ``platform/ext/`` folder.

Crypto service configurations
-----------------------------

Crypto Secure Partition
^^^^^^^^^^^^^^^^^^^^^^^

TF-M Profile Medium enables Crypto SP in top-level CMake config file and selects
all the Crypto modules.

Mbed Crypto configurations
^^^^^^^^^^^^^^^^^^^^^^^^^^

TF-M Profile Medium adds a dedicated Mbed Crypto config file
``tfm_profile_m_mbedcrypto_config.h`` under ``platform/ext/common``.
TF-M Profile Medium specifies ``tfm_profile_m_mbedcrypto_config.h`` as the
default Mbed Crypto config in ``MBEDTLS_CONFIG_FILE`` in top-level CMake config
file, instead of the common one ``tfm_mbedcrypto_config.h`` [CRYPTO-DESIGN]_.

Major Mbed Crypto configurations are set as listed below:

    - Enable SHA256
    - Enable generic message digest wrappers
    - Enable AES
    - Enable CCM mode for symmetric ciphers
    - Disable other modes for symmetric ciphers
    - Enable ECDH
    - Enable ECDSA
    - Select ECC curve ``secp256r1``
    - Other configurations required by selected option above

Other configurations can be selected to optimize the memory footprint of Crypto
module.

A device/use case can replace Profile Medium default Mbed Crypto config file
with its specific one to overwrite the default configurations. Alternatively, a
device can overwrite the configurations by appending a config file via
``MBEDTLS_USER_CONFIG_FILE``.

Internal Trusted Storage configurations
---------------------------------------

ITS service is enabled in top-level Profile Medium CMake config file by default.

The internal transient buffer size ``ITS_BUF_SIZE`` [ITS-INTEGRATE]_ is set to
32 bytes by default. A platform/use case can overwrite the buffer size in its
specific configuration extension according to its actual requirement of assets
and Flash attributes.

Profile Medium CMake config file won't touch the configurations of device
specific Flash hardware attributes [ITS-INTEGRATE]_.

Protected Storage Secure Partition
----------------------------------

Data confidentiality, integrity and anti-rollback protection are enabled by
default in PS.

If PS is selected, AES-CCM is used as AEAD algorithm by default. It requires to
enable PS implementation to select diverse AEAD algorithm.

If platforms don't integrate any off-chip storage device, platforms can disable
PS in platform specific configuration extension file via
``TFM_PROFILE_CONFIG_EXT``.
Profile Medium provides a configuration extension file example
``profile_m_config_ext_ps_disabled.cmake`` which disables PS service.

BL2 setting
-----------

Profile Medium enables MCUBoot provided by TF-M by default. A platform can
overwrite this configuration by disabling MCUBoot in its configuration extension
file or in its specific CMake file under ``platform/ext/`` folder.

If MCUBoot provided by TF-M is enabled, multiple image boot is selected by
default in TF-M Profile Medium top-level CMake config file.

If a device implements its own boot loader, the configurations are
implementation defined.

****************
Platform support
****************

To enable Profile Medium on a platform, the platform specific CMake file should
be added into the platform support list in top-level Profile Medium CMake config
file.

Building Profile Medium
=======================

To build Profile Medium, argument ``PROJ_CONFIG`` in build command line should
be set to following config files

    - ``ConfigDefaultProfileM.cmake``
    - ``ConfigRegressionProfileM.cmake``.

Take AN521 as an example:

The following commands build Profile Medium without test cases on **AN521** with
build type **MinSizeRel**, built by **Armclang**.

.. code-block:: bash

    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`readlink -f ../configs/ConfigDefaultProfileM.cmake` \
                             -DTARGET_PLATFORM=AN521       \
                             -DCMAKE_BUILD_TYPE=MinSizeRel \
                             -DCOMPILER=ARMCLANG ../
    cmake --build ./ -- install

The following commands build Profile Medium with regression test cases on
**AN521** with build type **MinSizeRel**, built by **Armclang**.

.. code-block:: bash

    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`readlink -f ../configs/ConfigRegressionProfileM.cmake` \
                             -DTARGET_PLATFORM=AN521       \
                             -DCMAKE_BUILD_TYPE=MinSizeRel \
                             -DCOMPILER=ARMCLANG ../
    cmake --build ./ -- install

More details of building instructions and parameters can be found TF-M build
instruction guide [TFM-BUILD]_.

The following commands include platform specific configuration extension file
via ``TFM_PROFILE_CONFIG_EXT`` in command line. ``TFM_PROFILE_CONFIG_EXT`` can
be an absolute path or a relative one to TF-M code root directory.
The configuration extension file ``profile_m_config_ext_ps_disabled.cmake`` in
the example below overwrites and disables PS service.

.. code-block:: bash

    cmake -G"Unix Makefiles" -DPROJ_CONFIG=`readlink -f ../configs/ConfigDefaultProfileM.cmake` \
                             -DTARGET_PLATFORM=AN521       \
                             -DCMAKE_BUILD_TYPE=MinSizeRel \
                             -DCOMPILER=ARMCLANG           \
                             -DTFM_PROFILE_CONFIG_EXT=configs/profile_m_config_ext_ps_disabled.cmake ../
    cmake --build ./ -- install


*********
Reference
*********

.. [PSA-FF-M] `Arm Platform Security Architecture Firmware Framework 1.0 <https://developer.arm.com/-/media/Files/pdf/PlatformSecurityArchitecture/Architect/DEN0063-PSA_Firmware_Framework-1.0.0-2.pdf?revision=2d1429fa-4b5b-461a-a60e-4ef3d8f7f4b4>`_

.. [RFC7925] `Transport Layer Security (TLS) / Datagram Transport Layer Security (DTLS) Profiles for the Internet of Things <https://tools.ietf.org/html/rfc7925>`_

.. [PROFILE-S] :doc:`Trusted Firmware-M Profile Small Design </docs/design_documents/profiles/tfm_profile_small>`

.. [RFC7252] `The Constrained Application Protocol (CoAP) <https://tools.ietf.org/html/rfc7252>`_

.. [RFC4279] `Pre-Shared Key Ciphersuites for Transport Layer Security (TLS) <https://tools.ietf.org/html/rfc4279>`_

.. [RFC7251] `AES-CCM Elliptic Curve Cryptography (ECC) Cipher Suites for TLS <https://tools.ietf.org/html/rfc7251>`_

.. [CRYPTO-DESIGN] :doc:`Crypto design </docs/design_documents/tfm_crypto_design>`

.. [ITS-INTEGRATE] :doc:`ITS integration guide </docs/reference/services/tfm_its_integration_guide>`

.. [TFM-BUILD] :doc:`TF-M build instruction </docs/getting_started/tfm_build_instruction>`

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*
