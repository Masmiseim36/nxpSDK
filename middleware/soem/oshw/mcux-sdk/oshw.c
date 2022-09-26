/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/* 
 * Copyright 2022 NXP
*/

#include "oshw.h"
#include <stdlib.h>

uint16 oshw_htons(uint16 host)
{
	return ((uint16)((((host) & (uint16)0x00ffU) << 8) | (((host) & (uint16)0xff00U) >> 8)));
}
uint16 oshw_ntohs(uint16 network)
{
	return ((uint16)((((network) & (uint16)0x00ffU) << 8) | (((network) & (uint16)0xff00U) >> 8)));
}

/* Create list over available network adapters.
 * @return First element in linked list of adapters
 */
ec_adaptert * oshw_find_adapters(void)
{
   ec_adaptert * ret_adapter = NULL;

   /* TODO if needed */

   return ret_adapter;
}

/** Free memory allocated memory used by adapter collection.
 * @param[in] adapter = First element in linked list of adapters
 * EC_NOFRAME.
 */
void oshw_free_adapters(ec_adaptert * adapter)
{
	   /* TODO if needed */
}
