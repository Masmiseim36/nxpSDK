# MCUboot and ota examples changelog

All notable changes to MCUboot fork in MCUXpresso SDK and related ota examples will be documented in this file.

## [26.03.0] - 03/2026

### Added
- MCUboot updated to v2.3.0
- Enabled ota_rom_basic for FRDM-MCXN236
- Added new upgrade mode SINGLE_APPLICATION_SLOT
- Enabled serial recovery feature (see [Serial Recovery doc](_doc/serial_recovery.md))

### Changed
- Update Mbed TLS to 3.0 in ota_mcuboot_client example
- Specify terminology of upgrade modes supported in ota examples with MCUBoot (see [README](README.md))
- FRDM-MCXA153 configured to use SINGLE_APPLICATION_SLOT mode due to its very limited flash size
- LPCXpresso55S69 configured to use TinyCrypt due to higher flash usage of the latest mcuboot+mbedtls. Original prj.conf is left for reference.

## [25.12.0] - 12/2025

### Added
- Added basic support for KW47-EVK and MCX-W72-EVK board
- Added SB3 processing support for MIMXRT700EVK (see [SB3 doc](_doc/sb3_common_readme.md))

## [25.09.0] - 09/2025

### Fixed
- (Encrypted XIP)(IPED) Fixed issue between IPED and PKC

### Added
- Added new doc [Kconfig and customization of OTA examples](_doc/kconfig_customization.md)

### Changed
- Update MCUboot to 2.2.0
- Update Mbed TLS to 3.0 in mcuboot_opensource example
    - PSA Crypto API (MCUBOOT_USE_PSA_CRYPTO) is used in default configurations to utilize hardware acceleration
    - Legacy API (MCUBOOT_USE_MBED_TLS) is used in configuration using Encrypted XIP due limitations in PSA Crypto API in current MCUboot version
    - EC256 made as default signature algorithm replacing obsolete RSA
        - it is still possible to manually setup RSA but only for legacy API
- Encrypted XIP: reworked the layer to use only the overwrite mode
- Changed default partition layout on FRDM-RW612, RD-RW612BGA and EVKCMIMXRT1060:
    - mcuboot_opensource: 256kB
    - application: 2MB

### Removed
- Encrypted XIP: removed the experimental three-slot mode

## [25.06.0] - 06/2025

### Added
- Added `ota_rom_basic` example demonstrating ROM bootloader processing a secure binary as an OTA image (see documentation in docs folder)
- Enabled `ota_rom_basic` example for FRDM-MCXN947, MCX-N9XX-EVK, FRDM-RW612 and RD-RW612BGA
- Enabled flash remap feature for MIMXRT700EVK
- FRDM-MCXE247 basic MCUboot support
- FRDM-MCXN947, MCX-N5XX-EVK and MCX-N9XX-EVK supports alternative build using MCUboot located in the internal flash instead of the IFR region (default) - see board specific readme for more information

### Removed
- Removed `ota_rom_server` (replaced by simpler `ota_rom_basic`)

## [25.03.0] - 03/2025

### Added

- (MCXA20) Basic support for FRDM-MCXA276 and FRDM-MCXA166
- Added SB3 processing support in `ota_mcuboot_basic` example for selected platforms
- (MCXN)(RW61x) Enabled SB3 processing support for FRDM-MCXN947, MCXN9XXEVK, FRDM-RW612 and RD-RW612BGA

### Changed

- Update MCUboot to 2.1.0

### Fixed

- (Encrypted XIP)(IPED) Fixed issues in wifi based examples using IPED encryption
- (Encrypted XIP) Fixed missing default encryption keys

### Removed

- Removed support for MDK toolchain

## [24.12.0] - 12/2024

### Added

- Initial release

### Removed

- Removed WiFi OTA examples on boards that also support Ethernet
