// ----------------------------------------------------------------------------
// Copyright 2018 ARM Ltd.
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

#ifndef __CERTIFICATE_RENEWAL_DATA_H__
#define __CERTIFICATE_RENEWAL_DATA_H__

#include "certificate_enrollment.h"
#include "est_defs.h"

/*
 * This file declares the CertificateRenewalDataBase base class and its derived class.
 * An instantiated object holds all the data necessary for a certificate renewal process.
 * The derived classes implement different virtual methods of the base class.
 */

namespace CertificateEnrollmentClient {

    // Abstract base class for data for the renewal process of a single certificate
    /*
     * Abstract base class for data for the renewal process of a single certificate
     * Keeps data required for the process.
     * Derived class must implement the pure virtual functions of this class.
     */
    class CertificateRenewalDataBase {

    public:
        CertificateRenewalDataBase(const uint8_t *raw_data, size_t raw_data_size);
        virtual ~CertificateRenewalDataBase();

        /*
         * Gets a TLV (Type-Length-Value) buffer to parse, each element in the TLV buffer is being treated and executed
         * according to the given ce_tlv_type_e that is defined in ce_tlv.h file.
         * Each element Type defined as uint16_t primitive and signifies two things:
         * (1) the type of operation
         * (2) is element 'required' or 'optional'.
         *
         * We distinguish if an element is required or optional by toggling the type's field MSB (Most Significant Bit).
         * If the type's field MSB is set to '0' - this element marked as 'required'
         * If the type's field MSB is set to '1' - this element marked as 'optional'
         *
         * The function iterates through the TLV buffer and enforces the following rules for each element:
         * (1) if element's type is unsupported and the type is marked as 'optional' - element is being skipped
         * (2) if element's type is unsupported and the type is marked as 'required' - CE_STATUS_BAD_INPUT_FROM_SERVER error will be returned
         * (3) if element's type is supported and the type is marked as 'optional' / 'required' - element is parsed and executed
         *
         * Currently the only supported type is CE_TLV_TYPE_CERT_NAME as presents in ce_tlv.h, it means that the certificate name
         * will be pointed by 'cert_name' which must be persist until this object is destroyed.
         *
         * The TLV buffer MUST be coherent in memory.
         * The TLV buffer is not forced to be word aligned.
         *
         * @return CE_STATUS_SUCCESS if parsing succeeded or one of the faulty errors in ce_status.h
         */
        virtual ce_status_e parse() = 0;

        /*
        * This function is called after the certificate renewal operation has completed (success or error).
        * Important: When this function is called, the application assumes that the operation had already finished and new connections are allowed to be made.
        *
        * \param status The end status of the certificate renewal.
        */
        virtual void finish(ce_status_e status) = 0;

        // Certificate name - NULL terminated. Should not be freed, should point to the name inside _raw_data 
        const char *cert_name;

        cs_renewal_names_s renewal_items_names;

        // The certificate chain received from the EST service. Released in the destructor.
        cert_chain_context_s *est_data;

        // Key handle that should be initialized and then used when generating a CSR and later when storing the certificate. Released in destructor.
        cs_key_handle_t key_handle;

        // Pointer to the generated CSR. Freed in destructor.
        uint8_t *csr;

        // Size of the CSR
        size_t csr_size;

    protected:
        // Pointer to raw data containing the certificate name. Free in destructor
        uint8_t *_raw_data;

        // Size of _raw_data
        size_t _raw_data_size;

    };

    // From device API data is not a TLV but a string
    class CertificateRenewalDataFromDevice : public CertificateRenewalDataBase {
    public:
        CertificateRenewalDataFromDevice(const char *raw_data);
        virtual ~CertificateRenewalDataFromDevice();

        /*
         * Set cert_name to point to the raw_data from the user which is null terminated.
         * Note that the constructor already allocated and copied the string provided by the user so cert_name will just point to that.
         */
        virtual ce_status_e parse();

        /*
         * Call the user callback with status. The initiator is CE_INITIATOR_DEVICE.
         *
         * \param status The status that will be specified when the user callback is called.
         */
        virtual void finish(ce_status_e status);
    };

    // Class used when the request was initiated by the server. raw_data is TLV
    class CertificateRenewalDataFromServer : public CertificateRenewalDataBase {
    public:
        CertificateRenewalDataFromServer(const uint8_t *raw_data, size_t raw_data_size);
        virtual ~CertificateRenewalDataFromServer();

        /*
         *	Parse the certificate name from _raw_data which contains the TLV received from the server.
         */
        virtual ce_status_e parse();

        /*
        * Call the user callback with status. The initiator is CE_INITIATOR_DEVICE.
        * Then set the resource to the status value and set a delayed response to the server.
        *
        * \param status The status that will be specified when the user callback is called, and sent to the server.
        */
        virtual void finish(ce_status_e status);
    };


}

#endif // __CERTIFICATE_RENEWAL_DATA_H__

