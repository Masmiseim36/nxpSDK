// ----------------------------------------------------------------------------
// Copyright 2018 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

#include "simplem2mclient.h"
#include "certificate_enrollment_user_cb.h"

#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
/*
 * Certificate Renewal user callback
 */
void certificate_renewal_cb(const char *cert_name, ce_status_e status, ce_initiator_e initiator)
{
    printf("Certificate renewal completed.\nCertificate name: %s\nStatus: 0x%x\nInitiator: %d\n", cert_name, status, initiator);
    printf("Certificate renewal status string: ");
    switch (status) {
        case CE_STATUS_SUCCESS:
            printf("Renewal Success: Operation completed successfully\n");
            break;
        case CE_STATUS_ERROR:
            printf("Renewal Error: Operation ended with an unspecified error\n");
            break;
        case CE_STATUS_INVALID_PARAMETER:
            printf("Renewal Error: A parameter provided to the function was invalid\n");
            break;
        case CE_STATUS_OUT_OF_MEMORY:
            printf("Renewal Error: An out-of-memory condition occurred\n");
            break;
        case CE_STATUS_ITEM_NOT_FOUND:
            printf("Renewal Error: Attempted to renew a certificate that does not exist in storage\n");
            break;
        case CE_STATUS_DEVICE_BUSY:
            printf("Renewal Error: Device currently processing too many certificate renewals\n");
            break;
        case CE_STATUS_BAD_INPUT_FROM_SERVER:
            printf("Renewal Error: Server sent a malformed TLV\n");
            break;
        case CE_STATUS_EST_ERROR:
            printf("Renewal Error: Error when requesting the new certificate from the EST service\n");
            break;
        case CE_STATUS_RENEWAL_ITEM_VALIDATION_ERROR:
        case CE_STATUS_BACKUP_ITEM_ERROR:
        case CE_STATUS_ORIGINAL_ITEM_ERROR:
        case CE_STATUS_RESTORE_BACKUP_ERROR:
        case CE_STATUS_RENEWAL_STATUS_ERROR:
            printf("Renewal Error: Operation failed to create/read/validate items\n");
            break;
        case CE_STATUS_FORBIDDEN_REQUEST:
            printf("Renewal Error: Server asked for forbidden request (e.g.: the server is not allowed to renew the device's bootstrap certificate)\n");
            break;
        case CE_STATUS_ITEM_IS_EMPTY:
            printf("Renewal Error: Item was found in storage but has zero length\n");
            break;
        case CE_STATUS_NOT_INITIALIZED:
            printf("Renewal Error: Called CertificateEnrollmentClient API before module initialization\n");
            break;
        default:
            printf("Renewal Error: Unknown exit status. Check ce_status.h\n");
            break;
    }

}
#endif // MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT


