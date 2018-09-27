#-------------------------------------------------------------------------------
# Copyright (c) 2012 Freescale Semiconductor, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# o Redistributions of source code must retain the above copyright notice, this list
#   of conditions and the following disclaimer.
#
# o Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
# o Neither the name of Freescale Semiconductor, Inc. nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Target and board configuration
#-------------------------------------------------------------------------------

# Chip
ifeq "$(CHIP)" "K70F12"
CPU = cortex-m4
ARCH = armv7e-m
DEFINES += -DCPU_MK70FN1M0VMJ12
else ifeq "$(CHIP)" "K64F12"
CPU = cortex-m4
ARCH = armv7e-m
DEFINES += -DCPU_MK64FN1M0VMD12
else ifeq "$(CHIP)" "K22F51212"
CPU = cortex-m4
ARCH = armv7e-m
DEFINES += -DCPU_MK22FN512VDC12
else ifeq "$(CHIP)" "KL25Z4"
CPU = cortex-m0plus
ARCH = armv6-m
DEFINES += -DCPU_MKL25Z128VLK4
else ifdef TARGET
$(error Unknown target $(TARGET))
endif

# Not debug by default
ifeq "$(build)" "debug"
DEBUG := 1
else
DEBUG := 0
endif

