/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* \brief       Connectivity Framework Module Information
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

#include "ModuleInfo.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/* FWK build version  */
/*! \cond DOXY_SKIP_TAG */
#define gFwkVerMajor_c   5
#define gFwkVerMinor_c   3
#define gFwkVerPatch_c   6
#define gFwkBuildNo_c    0
#define gFwkModuleId_c   0xA3
/*! \endcond */

#define gFwkVerString_c "CONNECTIVITY FRAMEWORK v" \
                        QUH(gFwkVerMajor_c) "." \
                        QUH(gFwkVerMinor_c) "." \
                        QUH(gFwkVerPatch_c)

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
/*!   
 * \brief The version string of the Connectivity Framework
 */
static char* mFwkVersionString = gFwkVerString_c;

/*!   
 * \brief Registers the Connectivity Framework module info
 */
RegisterModuleInfo(FWK, /* DO NOT MODIFY */
                   mFwkVersionString, /* DO NOT MODIFY */
                   gFwkModuleId_c, /* DO NOT MODIFY */
                   gFwkVerMajor_c, /* DO NOT MODIFY */
                   gFwkVerMinor_c, /* DO NOT MODIFY */
                   gFwkVerPatch_c, /* DO NOT MODIFY */
                   gFwkBuildNo_c); /* DO NOT MODIFY */
