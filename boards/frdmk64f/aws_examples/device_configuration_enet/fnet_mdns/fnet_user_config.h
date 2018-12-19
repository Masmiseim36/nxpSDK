/**************************************************************************
*
* Copyright 2016-2018 by Andrey Butok. FNET Community.
* Copyright 2018 NXP. Not a Contribution
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

/**********************************************************************/
/*!
*
* @file fnet_user_config.h
*
* @brief FNET User configuration file.
* It should be used to change any default configuration parameter.
*
***************************************************************************/

#ifndef _FNET_USER_CONFIG_H_

#define _FNET_USER_CONFIG_H_

/*****************************************************************************
* Compiler support FNET_CFG_COMP_<name> is defined in project options.
******************************************************************************/

/*****************************************************************************
* IPv4 and/or IPv6 protocol support.
******************************************************************************/
#define FNET_CFG_IP4                (1)
#define FNET_CFG_IP6                (0) /* Not supported in this port */

/*****************************************************************************
* Multicast DNS (mDNS) "Bonjour" server/responder support.
******************************************************************************/
#define FNET_CFG_MDNS                       (1)

#define FNET_CFG_DEBUG                      (0)
#define FNET_CFG_DEBUG_MDNS                 (0)


#endif /* _FNET_USER_CONFIG_H_ */

