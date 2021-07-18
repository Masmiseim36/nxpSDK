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

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mfloat-abi=soft")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-common")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fdata-sections")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -ffreestanding")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -fno-builtin")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mapcs")

SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -std=gnu99")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DCPU_MK60DN512VMD10")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DFREESCALE_KSDK_BM")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DMBEDTLS_CONFIG_FILE='\"ksdk_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTWR_K60D100M")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DTOWER")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DCPU_MK60DN512VMD10")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DFREESCALE_KSDK_BM")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DMBEDTLS_CONFIG_FILE='\"ksdk_mbedtls_config.h\"'")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTWR_K60D100M")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DTOWER")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fomit-frame-pointer")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfloat-abi=soft")

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

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mfloat-abi=soft")

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

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mcpu=cortex-m4")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -mfloat-abi=soft")

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

include_directories(${ProjDirPath}/../../../../../../CMSIS/Include)

include_directories(${ProjDirPath}/../../../../../../devices)

include_directories(${ProjDirPath}/..)

include_directories(${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include)

include_directories(${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/port/ksdk)

include_directories(${ProjDirPath}/../../../..)

include_directories(${ProjDirPath}/../../../../../../devices/MK60D10/drivers)

include_directories(${ProjDirPath}/../../../../../../devices/MK60D10/utilities)

include_directories(${ProjDirPath}/../../../../../../devices/MK60D10)

add_executable(mbedtls_selftest.elf 
"${ProjDirPath}/../../../../../../devices/MK60D10/gcc/startup_MK60D10.S"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_rnga.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_rnga.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_uart.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_uart.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_port.h"
"${ProjDirPath}/../selftest.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_common.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_common.c"
"${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/cau_api.h"
"${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/fsl_mmcau.h"
"${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/fsl_mmcau.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/utilities/fsl_debug_console.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/utilities/fsl_debug_console.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/system_MK60D10.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/system_MK60D10.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/aes.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/aesni.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/arc4.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/asn1.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/asn1write.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/base64.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/bignum.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/blowfish.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/bn_mul.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/camellia.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ccm.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/certs.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/check_config.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/cipher.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/cipher_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/compat-1.3.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/config.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ctr_drbg.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/debug.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/des.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/dhm.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ecdh.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ecdsa.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ecjpake.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ecp.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/entropy.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/entropy_poll.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/error.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/gcm.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/havege.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/hmac_drbg.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/md.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/md_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/md2.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/md4.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/md5.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/memory_buffer_alloc.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/net.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/oid.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/padlock.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/pem.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/pk.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/pk_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/pkcs5.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/pkcs11.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/pkcs12.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/platform.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ripemd160.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/rsa.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/sha1.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/sha256.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/sha512.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ssl.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ssl_cache.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ssl_ciphersuites.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ssl_cookie.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ssl_internal.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/ssl_ticket.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/threading.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/timing.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/version.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/x509.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/x509_crl.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/x509_crt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/x509_csr.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/include/mbedtls/xtea.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/aes.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/aesni.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/arc4.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/asn1parse.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/asn1write.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/base64.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/bignum.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/blowfish.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/camellia.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ccm.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/certs.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/cipher.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/cipher_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ctr_drbg.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/debug.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/des.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/dhm.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ecdh.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ecdsa.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ecjpake.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ecp.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ecp_curves.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/entropy.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/entropy_poll.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/error.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/gcm.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/havege.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/hmac_drbg.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/md.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/md_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/md2.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/md4.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/md5.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/memory_buffer_alloc.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/oid.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/padlock.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pem.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pk.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pk_wrap.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pkcs5.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pkcs11.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pkcs12.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pkparse.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/pkwrite.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/platform.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ripemd160.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/rsa.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/sha1.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/sha256.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/sha512.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_cache.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_ciphersuites.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_cli.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_cookie.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_srv.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_ticket.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/ssl_tls.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/threading.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/timing.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/version.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/version_features.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509_create.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509_crl.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509_crt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509_csr.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509write_crt.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/x509write_csr.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/library/xtea.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/port/ksdk/ksdk_mbedtls.c"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/port/ksdk/ksdk_mbedtls_config.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/port/ksdk/sha1_alt.h"
"${ProjDirPath}/../../../../../../middleware/mbedtls_2.3.0/port/ksdk/sha256_alt.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_gpio.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_gpio.c"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_smc.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_smc.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_flash.c"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_flash.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_clock.h"
"${ProjDirPath}/../../../../../../devices/MK60D10/drivers/fsl_clock.c"
)


set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/MK60DN512xxx10_flash.ld -static")

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/MK60DN512xxx10_flash.ld -static")

TARGET_LINK_LIBRARIES(mbedtls_selftest.elf -Wl,--start-group)
target_link_libraries(mbedtls_selftest.elf debug m)

target_link_libraries(mbedtls_selftest.elf debug c)

target_link_libraries(mbedtls_selftest.elf debug gcc)

target_link_libraries(mbedtls_selftest.elf debug nosys)

target_link_libraries(mbedtls_selftest.elf optimized m)

target_link_libraries(mbedtls_selftest.elf optimized c)

target_link_libraries(mbedtls_selftest.elf optimized gcc)

target_link_libraries(mbedtls_selftest.elf optimized nosys)

link_directories(${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/asm-cm4-cm7)

target_link_libraries(mbedtls_selftest.elf debug ${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/asm-cm4-cm7/lib_mmcau.a)

link_directories(${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/asm-cm4-cm7)

target_link_libraries(mbedtls_selftest.elf optimized ${ProjDirPath}/../../../../../../middleware/mmcau_2.0.0/asm-cm4-cm7/lib_mmcau.a)

TARGET_LINK_LIBRARIES(mbedtls_selftest.elf -Wl,--end-group)

