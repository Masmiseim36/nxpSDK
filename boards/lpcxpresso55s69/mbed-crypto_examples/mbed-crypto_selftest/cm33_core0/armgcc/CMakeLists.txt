INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})


SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_FREESCALE_HASHCRYPT_SHA1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_FREESCALE_HASHCRYPT_SHA256")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"mbedcrypto_mcux_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MMD")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MP")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-common")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mapcs")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_FREESCALE_HASHCRYPT_SHA1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_FREESCALE_HASHCRYPT_SHA256")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"mbedcrypto_mcux_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mthumb")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MMD")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MP")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-common")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mapcs")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mthumb")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MMD")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MP")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-common")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mapcs")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-rtti")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCPU_LPC55S69JBD100_cm33_core0")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DSERIAL_PORT_TYPE_UART=1")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mthumb")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MMD")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MP")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-common")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mapcs")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-rtti")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -z")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__stack_size__=0x3000")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --defsym=__heap_size__=0x8000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m33")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=hard")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfpu=fpv5-sp-d16")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-common")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mthumb")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mapcs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -z")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} muldefs")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Map=output.map")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__stack_size__=0x3000")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Xlinker")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --defsym=__heap_size__=0x8000")

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../..)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/drivers)

include_directories(${ProjDirPath}/../../../../../../middleware/mbed-crypto/include)

include_directories(${ProjDirPath}/../../../../../../middleware/mbed-crypto/port)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console)

include_directories(${ProjDirPath}/../../../../../../components/uart)

include_directories(${ProjDirPath}/../../../../../../components/serial_manager)

include_directories(${ProjDirPath}/../../../../../../components/lists)

include_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/utilities)

add_executable(mbed-crypto_selftest.elf 
"${ProjDirPath}/../selftest.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_casper.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_casper.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_hashcrypt.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_rng.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_rng.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_accel_driver.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_compat.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_driver_common.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_entropy_driver.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_extra.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_platform.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_se_driver.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_sizes.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_struct.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_types.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbed_psa/crypto_values.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/aria.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/chacha20.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/chachapoly.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/check_config.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/cipher.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/cipher_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/cmac.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/compat-1.3.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/config.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ctr_drbg.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/des.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/dhm.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ecdh.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ecdsa.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ecjpake.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ecp.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ecp_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/entropy.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/entropy_poll.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/error.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/gcm.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/havege.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/hkdf.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/nist_kw.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/platform_time.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/platform_util.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/poly1305.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/psa_util.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/rsa_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/sha1.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/sha256.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/sha512.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/threading.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/timing.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/version.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/include/mbedtls/xtea.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/aes.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/aesni.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/arc4.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/asn1parse.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/asn1write.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/base64.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/bignum.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/blowfish.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/camellia.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ccm.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/chacha20.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/chachapoly.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/cipher.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/cipher_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/cmac.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ctr_drbg.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/des.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/dhm.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ecdh.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ecdsa.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ecjpake.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ecp.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ecp_curves.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/entropy.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/entropy_poll.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/error.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/gcm.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/havege.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/hkdf.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/md.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/md2.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/md4.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/md5.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/nist_kw.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/oid.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/padlock.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pem.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pk.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pk_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pkcs12.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pkcs5.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pkparse.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/pkwrite.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/platform.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/platform_util.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/poly1305.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_core.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_invasive.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_its.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_se.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_se.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_service_integration.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_slot_management.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_slot_management.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_storage.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_crypto_storage.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/psa_its_file.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/ripemd160.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/rsa.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/rsa_internal.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/sha1.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/sha256.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/sha512.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/threading.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/timing.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/version.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/version_features.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/library/xtea.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/aes_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_casper.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_config.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_entropy.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_hashcrypt.c"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/mbedcrypto_mcux_hw_config.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/sha1_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbed-crypto/port/sha256_alt.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_iocon.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_power.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/LPC55S69_cm33_core0_features.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/fsl_device_registers.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/system_LPC55S69_cm33_core0.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/startup_LPC55S69_cm33_core0.S"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_usart.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_flexcomm.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_assert.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/debug_console/fsl_debug_console_conf.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/str/fsl_str.h"
"${ProjDirPath}/../../../../../../components/uart/uart.h"
"${ProjDirPath}/../../../../../../components/uart/usart_adapter.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_manager.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_internal.h"
"${ProjDirPath}/../../../../../../components/lists/generic_list.c"
"${ProjDirPath}/../../../../../../components/lists/generic_list.h"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.c"
"${ProjDirPath}/../../../../../../components/serial_manager/serial_port_uart.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/core_cm33.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/mpu_armv8.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/tz_context.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_armclang_ltm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_compiler.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_gcc.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_iccarm.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/cmsis_version.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_common_tables.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_const_structs.h"
"${ProjDirPath}/../../../../../../CMSIS/Include/arm_math.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.c"
"${ProjDirPath}/../../../../../../devices/LPC55S69/drivers/fsl_reset.h"
"${ProjDirPath}/../../../../../../devices/LPC55S69/utilities/fsl_sbrk.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/LPC55S69_cm33_core0_flash.ld -static")

TARGET_LINK_LIBRARIES(mbed-crypto_selftest.elf -Wl,--start-group)
target_link_libraries(mbed-crypto_selftest.elf debug m)

target_link_libraries(mbed-crypto_selftest.elf debug c)

target_link_libraries(mbed-crypto_selftest.elf debug gcc)

target_link_libraries(mbed-crypto_selftest.elf debug nosys)

target_link_libraries(mbed-crypto_selftest.elf optimized m)

target_link_libraries(mbed-crypto_selftest.elf optimized c)

target_link_libraries(mbed-crypto_selftest.elf optimized gcc)

target_link_libraries(mbed-crypto_selftest.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(mbed-crypto_selftest.elf debug ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

link_directories(${ProjDirPath}/../../../../../../devices/LPC55S69/gcc)

target_link_libraries(mbed-crypto_selftest.elf optimized ${ProjDirPath}/../../../../../../devices/LPC55S69/gcc/libpower_hardabi.a)

TARGET_LINK_LIBRARIES(mbed-crypto_selftest.elf -Wl,--end-group)


