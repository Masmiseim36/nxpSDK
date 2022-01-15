# Copyright 2021 NXP. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

import os

# Move to the TFM build folder
os.chdir('../../../../../../../../../boards/lpcxpresso55s69/tfm_examples/tfm_bl2_demo/cm33_core0')

target = 'LPC55S69'

# Flash with PyOCD
os.system('pyocd erase --mass -t ' + target)                                                                # erase the flash memory
os.system('pyocd flash tfm_bl2_mcuboot/armgcc/debug/tfm_bl2_mcuboot.elf -t ' + target)                      # flash the secure image into target
os.system('pyocd flash tfm_bl2_demo_s/armgcc/debug/tfm_s_signed.bin --base-address 0x8000 -t ' + target)    # flash the signed secure image into target
os.system('pyocd flash tfm_bl2_demo_ns/armgcc/debug/tfm_ns_signed.bin --base-address 0x30000 -t ' + target) # flash the signed non-secure image into target
