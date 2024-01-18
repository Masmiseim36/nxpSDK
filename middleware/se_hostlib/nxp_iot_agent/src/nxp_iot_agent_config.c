/* 
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <nxp_iot_agent_config.h>
#include <pb.h>

const PB_BYTES_ARRAY_T(NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES_SIZE) iot_agent_trusted_root_ca_certificates_contents =
{ NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES_SIZE, NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES };
const pb_bytes_array_t* iot_agent_trusted_root_ca_certificates = (pb_bytes_array_t*) &iot_agent_trusted_root_ca_certificates_contents;
