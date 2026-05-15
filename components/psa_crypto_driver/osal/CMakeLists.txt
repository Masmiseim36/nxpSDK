 #
 # Copyright 2023,2026 NXP
 #
 # SPDX-License-Identifier: BSD-3-Clause
 #
if(CONFIG_PSA_CRYPTO_DRIVER_DCP)
target_include_directories(
    McuxPsaCryptoDriverDcp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/.
        #baremetal
        #frtos
        zephyr/.
)

target_sources(
    McuxPsaCryptoDriverDcp
    PRIVATE
        #baremetal/osal_mutex.c
        #frtos/osal_mutex.c
        zephyr/osal_mutex.c
        zephyr/osal_mutex_platform.h
        zephyr/threading_alt.c
)
endif() #CONFIG_PSA_CRYPTO_DRIVER_DCP

if(CONFIG_PSA_CRYPTO_DRIVER_ELS_PKC)
target_include_directories(
    McuxPsaCryptoDriverElsPkc
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/.
        #baremetal
        #frtos
        zephyr/.
)

target_sources(
    McuxPsaCryptoDriverElsPkc
    PRIVATE
        #baremetal/osal_mutex.c
        #frtos/osal_mutex.c
        zephyr/osal_mutex.c
        zephyr/threading_alt.c
)
endif() # CONFIG_PSA_CRYPTO_DRIVER_ELS_PKC

if(CONFIG_PSA_CRYPTO_DRIVER_ELE_S4xx)
target_include_directories(
    McuxPsaCryptoDriverEleS4xx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/.
        #baremetal
        #frtos
        zephyr/.
)

target_sources(
    McuxPsaCryptoDriverEleS4xx
    PRIVATE
        #baremetal/osal_mutex.c
        #frtos/osal_mutex.c
        zephyr/osal_mutex.c
        zephyr/threading_alt.c
)
endif() # CONFIG_PSA_CRYPTO_DRIVER_ELE_S4xx
