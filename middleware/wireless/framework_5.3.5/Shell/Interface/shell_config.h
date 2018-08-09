/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
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

#ifndef __SHELL_CONFIG_H__
#define __SHELL_CONFIG_H__

/* Enable/Disable the entire Shell module */
#ifndef SHELL_ENABLED
#define SHELL_ENABLED                (1)
#endif

/* config if help is used */
#ifndef SHELL_USE_HELP
#define SHELL_USE_HELP                (1)
#endif

/* config if logo is displayed */
#ifndef SHELL_USE_LOGO
#define SHELL_USE_LOGO                (1)
#endif

/* config if use auto complete */
#ifndef SHELL_USE_AUTO_COMPLETE
#define SHELL_USE_AUTO_COMPLETE       (1)
#endif

/* config the maximun number of entries in history table.
   if set to 0, history is disabled */
#ifndef SHELL_MAX_HIST
#define SHELL_MAX_HIST                (4)
#endif

/* enables printf functionality */
#ifndef SHELL_USE_PRINTF
#define SHELL_USE_PRINTF              (0)
#endif

/* enables echo functionality */
#ifndef SHELL_USE_ECHO
#define SHELL_USE_ECHO                (1)
#endif

/* consult buffer size */
#ifndef SHELL_CB_SIZE
#define SHELL_CB_SIZE                 (64)
#endif

/* maximum function args */
#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS                (8)
#endif

/* maximum number of user functions */
#ifndef SHELL_MAX_COMMANDS
#define SHELL_MAX_COMMANDS            (5)
#endif

/* Shell will use an alternate task for command processing */
#ifndef SHELL_USE_ALT_TASK
#define SHELL_USE_ALT_TASK            (0)
#endif

/* Interface configuration */
#ifndef SHELL_IO_TYPE
#define SHELL_IO_TYPE        (APP_SERIAL_INTERFACE_TYPE)
#endif

#ifndef SHELL_IO_NUMBER
#define SHELL_IO_NUMBER      (APP_SERIAL_INTERFACE_INSTANCE)     /* channel */
#endif

#ifndef SHELL_IO_SPEED
#define SHELL_IO_SPEED       (APP_SERIAL_INTERFACE_SPEED)
#endif

#endif //__SHELL_CONFIG_H__

/*******************************************************************************
 * EOF
 ******************************************************************************/
