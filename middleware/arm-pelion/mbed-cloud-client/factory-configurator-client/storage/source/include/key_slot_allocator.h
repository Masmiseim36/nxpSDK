// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
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

#ifdef MBED_CONF_MBED_CLOUD_CLIENT_PSA_SUPPORT

#ifndef KEY_SLOT_ALLOCATOR_H
#define KEY_SLOT_ALLOCATOR_H

#include <stdbool.h>
#include <inttypes.h>
#include "kcm_status.h"
#include "kcm_defs.h"
#include "psa/crypto.h"
#include "psa/crypto_extra.h"
#include "psa/crypto_types.h"
#include "psa_driver.h"

/* KSA items type enumeration. Each one has its own KSA table
*/
typedef enum {
    KSA_KEY_ITEM,
    KSA_CERTIFICATE_ITEM,
    KSA_CONFIG_ITEM,
    KSA_RBP_ITEM,
    KSA_LAST_ITEM
} ksa_item_type_e;

/* KSA items location type.
 * Please keep order
 */
typedef enum {
    KSA_NON_PSA_TYPE_LOCATION,
    KSA_PSA_TYPE_LOCATION,
    KSA_SECURE_ELEMENT_TYPE_LOCATION,
    KSA_MAX_TYPE_LOCATION,
} ksa_type_location_e;

//Max value of PSA id in KSA
#define KSA_MAX_PSA_ID_VALUE   0x2800

/** ksa table version current version number
*/
#define KSA_TABLE_VERSION_NUM    0x1

/** The initial individual keys allow to store in KSA
*/
#define KSA_INITIAL_TABLE_ENTRIES 10

/* KSA item name size *
 * defined as 32 bytes of name hash + 2 byte of metadata (second byte is currently reserved)
 */
#define KSA_ITEM_NAME_SIZE 34


 /*
 * Types of ksa entry ids
 */
typedef enum ksa_id_type_ {
    KSA_ACTIVE_PSA_ID_TYPE,
    KSA_FACTORY_PSA_ID_TYPE,
    KSA_CE_PSA_ID_TYPE,
    KSA_MAX_PSA_ID_TYPE
} ksa_id_type_e;

#ifdef __cplusplus
extern "C" {
#endif

    /*===========================Generatl Purpose APIs===========================================*/


        /** Loads and initializes the Key Slot Allocator table
        *
        * It loads the KSA table if it is already exists in backend store otherwise
        * it will allocates and initializes a new table in volatile memory.
        * This function also initializes the PSA crypto module.
        *
        * @returns ::KCM_STATUS_SUCCESS in case of success or one of the `::kcm_status_e` errors otherwise.
        */
    kcm_status_e ksa_init(void);


    /** Release the volatile KSA table and finalize PSA Crypto resource.
    *
    * @returns ::KCM_STATUS_SUCCESS in case of success or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_fini(void);


    /** Reset Key Slot Allocator to factory initial state by restoring all factory items
    * from backend store.
    *
    * @returns ::KCM_STATUS_SUCCESS in case of success or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_factory_reset(void);


#ifdef MBED_CONF_MBED_CLOUD_CLIENT_SECURE_ELEMENT_SUPPORT

    /** Gets the location of a certain item.
    *
    * If the given item does not exist in store, the function will return KCM_STATUS_ITEM_NOT_FOUND status.
    *
    * @item_name[IN] The complete item name (hash + metadata)
    * @kcm_item_type[IN] Item type as defined in `::kcm_item_type_e` and `storage_item_type_e`
    * @item_location_out[OUT] The item location will be set to the corresponding storage location as defined in `::kcm_item_location_e`
    *
    * @returns ::KCM_STATUS_SUCCESS if the item exist in store and the `item_location_out` parameter was set `::kcm_status_e` errors in case of an error.
    * @        ::KCM_STATUS_ITEM_NOT_FOUND if the given item name was not found in store.
    */
    kcm_status_e ksa_get_item_location(const uint8_t *item_name, uint32_t item_type, kcm_item_location_e *item_location_out);

    /** Gets private key slot number.
     *
     *
     * @prv_key_name[IN] The complete private key name (hash + metadata)
     * @se_prv_key_slot[OUT] Slot number of the key
     *
     * @returns ::KCM_STATUS_SUCCESS if the private key exists in SE(the slot number stored in `se_prv_key_slot` output parameter).
     *           `::kcm_status_e` errors in case of an error.
     */
    kcm_status_e ksa_se_private_key_get_slot(const uint8_t *prv_key_name, uint64_t *se_prv_key_slot);

    /** Registers an external items resides in secure element into a new table entry.
    *
    * If the target item already exist in table the function will verify that the item's origin is a secure element,
    * if not, KCM_STATUS_ERROR will be returned.
    *
    * @item_name[IN] The complete item name (hash + metadata)
    * @item_type[IN] KCM type of the item
    * @slot_number[IN] The SE slot number of the item
    *
    * @returns ::KCM_STATUS_SUCCESS if the key exist in store and the `ksa storage location` parameter was set `::kcm_status_e` errors in case of an error.
    * @        ::KCM_STATUS_ERROR if the given item already exist but its location is not KCM_LOCATION_SECURE_ELEMENT.
    */
    kcm_status_e ksa_register_se_item(const uint8_t *item_name, uint32_t item_type, uint64_t slot_number);
#endif // #ifdef MBED_CONF_MBED_CLOUD_CLIENT_SECURE_ELEMENT_SUPPORT

    /** Stores item in PSA
     *
     * If the given key name already exist in store, the function will return KCM_STATUS_KEY_EXIST status.
     *
     * @item_name[IN] The complete item name (hash + metadata)
     * @storage_flags[IN] Storage flags passed to PSA store
     * @item_type[IN] Item type as defined in `::kcm_item_type_e` and `storage_item_type_e`
     * @item_data[IN] The item data, if private key is passed with NULL, generate key pair with key_name
     * @item_data_size[IN] The item data size , can be 0 if key is NULL
     * @ksa_item_location[IN] The location that the item should be stored to. Can be PSA or Secure Element
     * @kcm_item_is_factory[IN] Set to "true" if this item is factory, "false" otherwise
     *
     * @returns ::KCM_STATUS_SUCCESS if the given key name occupied a new key slot or one of the `::kcm_status_e` errors in case of an error.
     * @        ::KCM_STATUS_KEY_EXIST if the given key name already exist.
     */
    kcm_status_e ksa_item_store(const uint8_t *item_name,
                                uint32_t storage_flags,
                                uint32_t item_type,
                                const uint8_t *item_data,
                                size_t item_data_size,
                                ksa_type_location_e  ksa_item_location,
                                bool kcm_item_is_factory,
                                bool is_delete_allowed);


    /** Reads data size from the PSA storage.
     *
     * If the given item name not exists in store, the function will return KCM_STATUS_ITEM_NOT_FOUND status.
     *
     * @item_name[IN] The complete item name (hash + metadata)
     * @item_name_length [IN] The complete item name size in bytes
     * @item_type[IN] Item type as defined in `::kcm_item_type_e`and `storage_item_type_e`
     * @item_data_out[IN/OUT] Item_data_out output size of items
     *
     * @returns ::KCM_STATUS_SUCCESS if the given key name occupied a new key slot or one of the `::kcm_status_e` errors in case of an error.
     * @        ::KCM_STATUS_KEY_EXIST if the given key name already exist.
     */
    kcm_status_e ksa_item_get_data_size(const uint8_t *item_name,
                                        uint32_t item_type,
                                        size_t *item_data_size_out);


    /** Reads data item from the PSA storage.
    *
    * @item_name[IN] The complete item name (hash + metadata)
    * @item_type[IN] Item type as defined in `::kcm_item_type_e`and `storage_item_type_e`
    * @param[in/out] Item_data_out item data output buffer. Can be NULL if `item_data_max_size` is 0.
    * @param[in] kcm_item_data_max_size The maximum size of the KCM item data output buffer in bytes.
    * @param[out] item_data_act_size_out Actual item data output buffer size in bytes.
    *
    *    @returns
    *       KCM_STATUS_SUCCESS in case of success otherwise one of kcm_status_e errors
    */
    kcm_status_e ksa_item_get_data(const uint8_t *item_name,
                                   uint32_t item_type,
                                   uint8_t *item_data_out,
                                   size_t item_data_max_size,
                                   size_t *item_data_act_size_out);


    /** Deletes data item from the PSA storage.
    *
    * @item_name[IN] The complete item name (hash + metadata)
    * @param[in] item_type Item type as defined in `::kcm_item_type_e` and  `storage_item_type_e`
    *
    *    @returns
    *       KCM_STATUS_SUCCESS in case of success otherwise one of kcm_status_e errors
    */
    kcm_status_e ksa_item_delete(const uint8_t *item_name,
                                 uint32_t item_type);


    /** Resets the PSA storage to an empty state.
    *
    *   @returns
    *       KCM_STATUS_SUCCESS in case of success otherwise one of kcm_status_e errors
    */
    kcm_status_e ksa_reset(void);


    /** Checks if item exists in PSA storage
    *
    * @item_name[IN] The complete item name (hash + metadata)
    * @param[in] item_type Item type as defined in `::kcm_item_type_e` and  `storage_item_type_e`
    * @returns
    *    KCM_STATUS_SUCCESS in case of success otherwise one of kcm_status_e errors
    */
    kcm_status_e ksa_item_check_existence(const uint8_t *item_name,
                                          uint32_t item_type);



    /** Returns a key handle if exists according to its name
    *
    * @key_name[IN] The complete item name (hash + metadata)
    * @key_handle_out[OUT] The key handle referred to the given key name, otherwise this out parameter value is undefined.
    *                      This out parameter is valid only if the status is KCM_STATUS_SUCCESS. In any other case this out parameter value is undefined.
    *
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_key_get_handle(const uint8_t *key_name, psa_key_handle_t *key_handle_out);

    /** Closes a key handle
    *
    * @key_handle[IN] The key handle
    *
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */

    kcm_status_e ksa_key_close_handle(psa_key_handle_t key_handle);


    /*======================Certificate Renewal related APIs===========================================*/


    /** Generates certificate enrollment keys based on existing keys according to passed key names.
    *
    * @private_key_name[IN] The name of the private key (hash + metadata)
    * @public_key_name[IN] The name of the public key (hash + metadata)
    * @psa_priv_key_handle[OUT] The output private key handle
    * @psa_pub_key_handle[OUT] The output public key handle
    *
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_generate_ce_keys(
        const uint8_t                     *private_key_name,
        const uint8_t                     *public_key_name,
        psa_key_handle_t                  *psa_priv_key_handle,
        psa_key_handle_t                   *psa_pub_key_handle);


    /** Removes certificate enrollment key according to passed key names.
    *
    * @key_name [IN] The name of the key (hash + metadata)
    * @item_type[IN] Item type as defined in `::kcm_item_type_e` and  `storage_item_type_e`
    *
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_destroy_ce_key(const uint8_t *key_name, uint32_t item_type);


    /** Creates a copy of existing ksa item using a new name.
    *
    * @item_name[IN] The name of the existing item (hash + metadata)
    * @uint32_t item_type[IN] item type to copy
    * @new_item_name[IN] The name of the new item (hash + metadata)
    *
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_copy_item(const uint8_t *existing_item_name, uint32_t item_type, const uint8_t *new_item_name);

    /** Removes the entry from ksa table without destroy PSA operation.
    *
    * @private_key_name[IN] The name of the private key (hash + metadata)
    * @item_type[IN] the item type to remove
    * @remove_active_only[IN] flag to notify if only active item should be removed from the entry
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e  ksa_remove_entry(const uint8_t *item_name, uint32_t item_type, bool remove_active_only);

    /** Activates renewal key. The API sets renewal id value to active id of the key and initializes renewal id field.
    *
    * @private_key_name[IN] The name of the private key (hash + metadata)
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_activate_ce_key(const uint8_t *key_name);

    /** Deletes old non-factory active id and removes backup entry.
    *   If the active id is factory id, the function only removes the backup entry.
    *
    * @item_name[IN] The name of the item (hash + metadata)
    * @item_type [IN] The item type
    *
    * @returns KCM_STATUS_SUCCESS if no error occured or one of the `::kcm_status_e` errors otherwise.
    */
    kcm_status_e ksa_destroy_old_active_and_remove_backup_entry(const uint8_t *item_name, uint32_t item_type);

#ifdef __cplusplus
}
#endif

#endif //KEY_SLOT_ALLOCATOR_H
#endif //MBED_CONF_MBED_CLOUD_CLIENT_PSA_SUPPORT
