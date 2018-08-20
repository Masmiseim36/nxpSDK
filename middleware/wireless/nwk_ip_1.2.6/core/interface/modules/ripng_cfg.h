#ifndef _RIPNG_CFG_H
#define _RIPNG_CFG_H
/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* General Includes */
#include "EmbeddedTypes.h"
#include "app_stack_config.h"

/*==================================================================================================
Private macros
==================================================================================================*/

#ifndef RIPNG_ENABLE
   #define RIPNG_ENABLE 0
#endif

#ifndef RIPNG_ROUTE_TIMEOUT_SEC
   #define RIPNG_ROUTE_TIMEOUT_SEC  180
#endif


#ifndef RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MIN_SEC
   #define RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MIN_SEC  1
#endif

#ifndef RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MAX_SEC
   #define RIPNG_TRIGGERD_UPDATE_HOLDOFF_TIME_MAX_SEC   5
#endif

#ifndef RIPNG_ROUTING_UPDATE_TIME_MIN_SEC
   #define RIPNG_ROUTING_UPDATE_TIME_MIN_SEC 15
#endif

#ifndef RIPNG_ROUTING_UPDATE_TIME_MAX_SEC
   #define RIPNG_ROUTING_UPDATE_TIME_MAX_SEC 45
#endif

#endif  /*_RIPNG_CFG_H */
