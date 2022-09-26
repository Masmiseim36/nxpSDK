
/**
 *  \file appl_dbase_dyn.c
 *
 *  Source File for SDP dynamic Database creation reference
 */

/*
 *  Copyright (C) 2012. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_sdp.h"

#ifdef SDP_SERVER
#ifdef SDP_DYNAMIC_DB
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
DECL_STATIC UINT8 bt_current_server_channel = 0x01U;

/* ----------------------------------------- Functions */
/* Add example records */
UINT8 bt_dbase_get_server_channel(void)
{
    UINT8 server_channel;

    server_channel = bt_current_server_channel;

    bt_current_server_channel++;

    /* TODO: Enhance logic to return free server channel */
    return server_channel;
}

void db_add_sap_database_record(void)
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x112D, 0x1204 };
    UINT8 service_name[] = { 0x53, 0x49, 0x4D, 0x20, 0x41, 0x63, 0x63, 0x65, 0x73, 0x73, 0x00 };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_SAP, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = bt_dbase_get_server_channel();

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems)/sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x112D,
            0x0101
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );
            
        BT_dbase_add_service_name
        (
            record_handle,
            sizeof(service_name),
            service_name
        );

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_spp_database_record(UINT8 server_ch, UINT8 *service_name, UINT16 service_name_length)
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x1101 };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_SPP, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = server_ch;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems)/sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x1101,
            0x0102
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );
            
        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add SPP record. Retval: 0x%04X\n", retval);
    }
}

void db_add_spp_vs_database_record(UINT8 server_ch, UINT8 * vs_uuid, UINT8 * service_name, UINT16 service_name_length)
{
    UINT32 record_handle;
    API_RESULT retval;
    DB_SERVICE_CLASS_UUID_ELEM service_uuids[2U];
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_SPP_VS, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = server_ch;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x1101,
            0x0102
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        {
            service_uuids[0U].uuid_len = 16U;
            BT_mem_copy(&service_uuids[0U].uuid_128[0], vs_uuid, 16U);

            service_uuids[1U].uuid_len = 2U;
            service_uuids[1U].uuid_16 = 0x1101;

            BT_dbase_add_service_class_id_list_ex
            (
                record_handle,
                sizeof(service_uuids) / sizeof(DB_SERVICE_CLASS_UUID_ELEM),
                service_uuids
            );
        }

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add SPP VS record. Retval: 0x%04X\n", retval);
    }
}

void db_add_hfu_database_record(UINT8 server_ch, UINT8 *service_name, UINT16 service_name_length)
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x111E, 0x1203 };
    UINT16 browse_group_uuids[] = { 0x1002 };
    UINT16 supported_features = 0x00DE;

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_HFU, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = server_ch;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x111E,
            0x0106
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_supported_features
        (
            record_handle,
            supported_features
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_hfag_database_record(UINT8 server_ch, UINT8 *service_name, UINT16 service_name_length)
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x111F, 0x1203 };
    UINT16 browse_group_uuids[] = { 0x1002 };
    UINT16 supported_features = 0x00DE;

    DB_PROTOCOL_ELEM elems[2U];
    UINT8 value_network;

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_HFAG, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = server_ch;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x111E,
            0x0106
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_supported_features
        (
            record_handle,
            supported_features
        );

        value_network = 0x01U;
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0301,
            1,
            &value_network
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_a2dp_snk_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 * provider_name,
         UINT16 provider_name_length
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x110B };
    UINT16 browse_group_uuids[] = { 0x1002 };
    UINT16 supported_features = 0x0001U;

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_A2DP_SINK, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
   
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0001;
        elems[0U].num_sub_params = 0x0000;
        elems[0U].params[0] = 0x0019; /* PSM AVDTP */

        elems[1U].protocol_uuid = 0x0019U; /* AVDTP */
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = 0x0102;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x110D,
            0x0102
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_provider_name
        (
            record_handle,
            provider_name_length,
            provider_name
        );

        BT_dbase_add_supported_features
        (
            record_handle,
            supported_features
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_a2dp_src_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 * provider_name,
         UINT16 provider_name_length
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x110A };
    UINT16 browse_group_uuids[] = { 0x1002 };
    UINT16 supported_features = 0x0001U;

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_A2DP_SOURCE, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0001;
        elems[0U].num_sub_params = 0x0000;
        elems[0U].params[0] = 0x0019; /* PSM AVDTP */

        elems[1U].protocol_uuid = 0x0019U; /* AVDTP */
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = 0x0102;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x110D,
            0x0102
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_provider_name
        (
            record_handle,
            provider_name_length,
            provider_name
        );

        BT_dbase_add_supported_features
        (
            record_handle,
            supported_features
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_panu_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 * service_desc,
         UINT16 service_desc_length,
         UINT8 * value_security_desc,
         UINT16  num_value_security_desc,
         UINT8 value_service_availability
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x1115 };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_PANU, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0001;
        elems[0U].num_sub_params = 0x0000;
        elems[0U].params[0] = 0x000F; /* PSM BNEP */

        elems[1U].protocol_uuid = 0x000FU; /* BNEP */
        elems[1U].num_params = 0x0005;
        elems[1U].num_sub_params = 0x0004;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = 0x0100;
        elems[1U].params[1] = 0x0800;
        elems[1U].params[2] = 0x0806;
        elems[1U].params[3] = 0x86DD;
        elems[1U].params[4] = 0x880B;

        /* TODO: Packet1, 2, ... */
        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x1115,
            0x0100
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_service_description
        (
            record_handle,
            service_desc_length,
            service_desc
        );

        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x030A,
            num_value_security_desc,
            value_security_desc
        );

        /* ServiceAvailability	Load Factor	UINT8	0xFF */
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0008,
            1,
            &value_service_availability
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_avrcp_ct_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 * provider_name,
         UINT16 provider_name_length,
         UINT16 supported_features
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x110E, 0x110F };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[2U];
    DB_PROTO_LIST_ELEM apd_elems[1U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_AVRCP_CONTROLLER, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0001;
        elems[0U].num_sub_params = 0x0000;
        elems[0U].params[0] = 0x0017; /* PSM AVCTP */

        elems[1U].protocol_uuid = 0x0017U; /* AVCTP */
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = 0x0104;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x110E,
            0x0106
        );

        /* Add Additional Protocol Descriptor List */
        apd_elems[0U].num_elems = 0x0002U;

        apd_elems[0U].elem[0U].protocol_uuid = 0x0100U;
        apd_elems[0U].elem[0U].num_params = 0x0001U;
        apd_elems[0U].elem[0U].num_sub_params = 0x0000;
        apd_elems[0U].elem[0U].params[0] = 0x001BU;

        apd_elems[0U].elem[1U].protocol_uuid = 0x0017U;
        apd_elems[0U].elem[1U].num_params = 0x0001U;
        apd_elems[0U].elem[1U].num_sub_params = 0x0000;
        apd_elems[0U].elem[1U].params[0] = 0x0104U;

        BT_dbase_add_additional_proto_desc_list
        (
            record_handle,
            sizeof(apd_elems) / sizeof(DB_PROTO_LIST_ELEM),
            apd_elems
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_provider_name
        (
            record_handle,
            provider_name_length,
            provider_name
        );

        BT_dbase_add_supported_features
        (
            record_handle,
            supported_features
        );

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_avrcp_tg_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 * provider_name,
         UINT16 provider_name_length,
         UINT16 supported_features
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x110C };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[2U];
    DB_PROTO_LIST_ELEM apd_elems[2U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_AVRCP_TARGET, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0001;
        elems[0U].num_sub_params = 0x0000;
        elems[0U].params[0] = 0x0017; /* PSM AVCTP */

        elems[1U].protocol_uuid = 0x0017U; /* AVCTP */
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = 0x0104;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x110E,
            0x0106
        );

        /* Add Additional Protocol Descriptor List */
        apd_elems[0U].num_elems = 0x0002U;

        apd_elems[0U].elem[0U].protocol_uuid = 0x0100U;
        apd_elems[0U].elem[0U].num_params = 0x0001U;
        apd_elems[0U].elem[0U].num_sub_params = 0x0000;
        apd_elems[0U].elem[0U].params[0] = 0x001BU;

        apd_elems[0U].elem[1U].protocol_uuid = 0x0017U;
        apd_elems[0U].elem[1U].num_params = 0x0001U;
        apd_elems[0U].elem[1U].num_sub_params = 0x0000;
        apd_elems[0U].elem[1U].params[0] = 0x0104U;

        apd_elems[1U].num_elems = 0x0002U;

        apd_elems[1U].elem[0U].protocol_uuid = 0x0100U;
        apd_elems[1U].elem[0U].num_params = 0x0001U;
        apd_elems[1U].elem[0U].num_sub_params = 0x0000;
        apd_elems[1U].elem[0U].params[0] = 0x8401U;

        apd_elems[1U].elem[1U].protocol_uuid = 0x0008U;
        apd_elems[1U].elem[1U].num_params = 0x0000U;
        apd_elems[1U].elem[1U].num_sub_params = 0x0000;

        BT_dbase_add_additional_proto_desc_list
        (
            record_handle,
            sizeof(apd_elems) / sizeof(DB_PROTO_LIST_ELEM),
            apd_elems
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_provider_name
        (
            record_handle,
            provider_name_length,
            provider_name
        );

        BT_dbase_add_supported_features
        (
            record_handle,
            supported_features
        );

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_pce_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x112E };
    UINT16 browse_group_uuids[] = { 0x1002 };

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_PBAP_PCE, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x1130,
            0x0102
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_pse_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 value_supported_repo
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x112F };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[3U];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_PBAP_PSE, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = bt_dbase_get_server_channel();

        elems[2U].protocol_uuid = 0x0008U; /* OBEX */
        elems[2U].num_params = 0x0000;
        elems[2U].num_sub_params = 0x0000;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x1130,
            0x0101
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        /* TODO: Supported Repositories */
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0314,
            1,
            &value_supported_repo
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_did_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT8 * cli_exe_url,
         UINT16  cli_exe_url_length,
         UINT8 * doc_url,
         UINT16  doc_url_length,
         UINT16 spec_id,
         UINT16 vendor_id,
         UINT16 prod_id,
         UINT16 ver_attr,
         UINT16 vendor_src,
         UINT8 primary_record
      )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x1200 };
    UINT16 browse_group_uuids[] = { 0x1002 };

    UINT8 spec_id_buf[2];
    UINT8 vendor_id_buf[2];
    UINT8 prod_id_buf[2];
    UINT8 ver_attr_buf[2];
    UINT8 vendor_src_buf[2];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_DID, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_description
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_PACK_BE_2_BYTE_VAL(&spec_id_buf[0], spec_id);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0200,
            sizeof(spec_id_buf),
            spec_id_buf
        );

        BT_PACK_BE_2_BYTE_VAL(&vendor_id_buf[0], vendor_id);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0201,
            sizeof(vendor_id_buf),
            vendor_id_buf
        );

        BT_PACK_BE_2_BYTE_VAL(&prod_id_buf[0], prod_id);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0202,
            sizeof(prod_id_buf),
            prod_id_buf
        );

        BT_PACK_BE_2_BYTE_VAL(&ver_attr_buf[0], ver_attr);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0203,
            sizeof(ver_attr_buf),
            ver_attr_buf
        );

        BT_PACK_BE_2_BYTE_VAL(&vendor_src_buf[0], vendor_src);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0205,
            sizeof(vendor_src_buf),
            vendor_src_buf
        );

        BT_dbase_add_attribute_type_boolean
        (
            record_handle,
            0x0204,
            primary_record
        );

        BT_dbase_add_attribute_type_url
        (
            record_handle,
            0x000A,
            doc_url_length,
            doc_url
        );

        BT_dbase_add_attribute_type_url
        (
            record_handle,
            0x000B,
            cli_exe_url_length,
            cli_exe_url
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_mce_database_record
     (
         UINT8 * service_name,
         UINT16 service_name_length,
         UINT16 obex_psm,
         UINT32 supported_features
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x1133 };
    UINT16 browse_group_uuids[] = { 0x1002 };

    DB_PROTOCOL_ELEM elems[3U];
    UINT8 value_map_supported_features[4];
    UINT8 value_goep_l2cap_psm[2];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_MAP_MCE, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        elems[0U].protocol_uuid = 0x0100U;
        elems[0U].num_params = 0x0000;
        elems[0U].num_sub_params = 0x0000;

        elems[1U].protocol_uuid = 0x003U;
        elems[1U].num_params = 0x0001;
        elems[1U].num_sub_params = 0x0000;

        /* Todo: dynamic server channel. user bit array to find which is free */
        elems[1U].params[0] = bt_dbase_get_server_channel();

        elems[2U].protocol_uuid = 0x0008U; /* OBEX */
        elems[2U].num_params = 0x0000;
        elems[2U].num_sub_params = 0x0000;

        BT_dbase_add_proto_desc_list
        (
            record_handle,
            sizeof(elems) / sizeof(DB_PROTOCOL_ELEM),
            elems
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x1134,
            0x0103
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_service_name
        (
            record_handle,
            service_name_length,
            service_name
        );

        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        /* TODO: Supported Repositories */
        /* MapSupportedFeatures	MAP Supported Features	UINT32	0x0077FFFF	M */
        BT_PACK_BE_4_BYTE_VAL(&value_map_supported_features[0], supported_features);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0317,
            sizeof(value_map_supported_features),
            value_map_supported_features
        );

        /* GoepL2CapPsm	L2CAP PSM	UINT16	0x8603	M */
        BT_PACK_BE_2_BYTE_VAL(&value_goep_l2cap_psm[0], obex_psm);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0200,
            sizeof(value_goep_l2cap_psm),
            value_goep_l2cap_psm
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_mps_database_record
     (
         UINT8 *supported_scenario_mpsd,
         UINT8 *supported_scenario_mpmd,
         UINT16 supported_proto_dependencies
     )
{
    UINT32 record_handle;
    API_RESULT retval;
    UINT16 service_uuids[] = { 0x113B };
    UINT16 browse_group_uuids[] = { 0x1002 };

    UINT8 value_supported_proto_deps[2];

    /* Create Record */
    retval = BT_dbase_create_record(DB_RECORD_MPS, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        BT_dbase_add_service_class_id_list
        (
            record_handle,
            sizeof(service_uuids) / sizeof(UINT16),
            service_uuids
        );

        BT_dbase_add_profile_descriptor_list
        (
            record_handle,
            0x113A,
            0x0100
        );

        BT_dbase_add_languagebase_attr_id_list
        (
            record_handle,
            0x656E,
            0x006A,
            0x0100
        );

        BT_dbase_add_browse_group_list
        (
            record_handle,
            sizeof(browse_group_uuids) / sizeof(UINT16),
            browse_group_uuids
        );

        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0200,
            0x08U,
            supported_scenario_mpsd
        );

        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0201,
            0x08U,
            supported_scenario_mpmd
        );

        BT_PACK_BE_2_BYTE_VAL(&value_supported_proto_deps[0], supported_proto_dependencies);
        BT_dbase_add_attribute_type_uint
        (
            record_handle,
            0x0202,
            sizeof(value_supported_proto_deps),
            value_supported_proto_deps
        );

        /* Activate the record */
        BT_dbase_activate_record(record_handle);
    }
    else
    {
        printf("Failed to add record. Retval: 0x%04X\n", retval);
    }
}

void db_add_record(void)
{
    UINT8 CustomUUID_0[16U] = { 0x59, 0x84, 0x55, 0x25, 0xF6, 0x12, 0x4F, 0xDE, 0x83, 0xD9, 0x1C, 0x6C, 0x91, 0x4C, 0x42, 0x72 };
    UINT8 CustomUUID_1[16U] = { 0x00, 0x00, 0x00, 0x00, 0xDE, 0xCA, 0xFA, 0xDE, 0xDE, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xCA, 0xFF };
    UINT8 CustomUUID_2[16U] = { 0x00, 0x00, 0x00, 0x00, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE };
    UINT8 CustomUUID_3[16U] = { 0x58, 0xE1, 0xF7, 0x90, 0xAA, 0x26, 0x11, 0xE3, 0xA5, 0xE2, 0x08, 0x00, 0x20, 0x0C, 0x9A, 0x67 };
    UINT8 CustomUUID_4[16U] = { 0xDE, 0xAB, 0x91, 0xE4, 0x67, 0x0F, 0x11, 0xE1, 0x9A, 0xCE, 0x30, 0xBC, 0x48, 0x24, 0x01, 0x9B };
    UINT8 CustomUUID_5[16U] = { 0x1C, 0x1E, 0xDF, 0xA7, 0x06, 0x57, 0x45, 0x1F, 0xBA, 0xE0, 0x18, 0x7B, 0xFB, 0xB9, 0x69, 0xB6 };

    UINT8 server_channel;

    /* SAP Record */
    db_add_sap_database_record();

    /* SPP Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_database_record(server_channel, (UINT8 *)"COM5", 5);

    /* SPP Vendor Specific Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_vs_database_record(server_channel, CustomUUID_0, (UINT8 *)"COM6", 5);

    /* SPP Vendor Specific Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_vs_database_record(server_channel, CustomUUID_1, (UINT8 *)"COM7", 5);

    /* SPP Vendor Specific Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_vs_database_record(server_channel, CustomUUID_2, (UINT8 *)"COM8", 5);

    /* SPP Vendor Specific Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_vs_database_record(server_channel, CustomUUID_3, (UINT8 *)"COM9", 5);

    /* SPP Vendor Specific Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_vs_database_record(server_channel, CustomUUID_4, (UINT8 *)"COM10", 6);

    /* SPP Vendor Specific Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_spp_vs_database_record(server_channel, CustomUUID_5, (UINT8 *)"COM11", 6);

    /* HFU Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_hfu_database_record(server_channel, (UINT8 *)"HFU", 4);

    /* HFAG Record */
    server_channel = bt_dbase_get_server_channel();
    db_add_hfag_database_record(server_channel, (UINT8 *)"Voice gateway", 14);

    /* A2DP Sink Record */
    db_add_a2dp_snk_database_record((UINT8 *)"A2DPSink", 9, (UINT8 *)"Mindtree", 9);

    /* A2DP Source Record */
    db_add_a2dp_src_database_record((UINT8 *)"A2DPSource", 11, (UINT8 *)"Mindtree", 9);

    /* PANU Record */
    {
        UINT8 panu_value_security_desc[] = { 0x00, 0x00 };
        UINT8 panu_value_service_availability;

        panu_value_service_availability = 0xFFU;
        db_add_panu_database_record
        (
            (UINT8 *)"Personal Ad-hoc User Service", 29,
            (UINT8 *)"Personal Ad-hoc User Service", 29,
            panu_value_security_desc, sizeof(panu_value_security_desc),
            panu_value_service_availability
        );
    }

    /* AVRCP Controller Record */
    {
        UINT16 avrcp_ct_supported_features = 0x03CFU;
        db_add_avrcp_ct_database_record((UINT8 *)"AVRCP Controller", 17, (UINT8 *)"Mindtree", 9, avrcp_ct_supported_features);
    }

    /* AVRCP Target Record */
    {
        UINT16 avrcp_tg_supported_features = 0x01FFU;
        db_add_avrcp_tg_database_record((UINT8 *)"AVRCP Target", 13, (UINT8 *)"Mindtree", 9, avrcp_tg_supported_features);
    }

    /* PBAP_PCE Record */
    db_add_pce_database_record((UINT8 *)"Phonebook Access PCE", 21);
    /* PBAP_PSE Record */
    {
        UINT8 pse_value_supported_repo = 0x03U;

        db_add_pse_database_record((UINT8 *)"Phonebook Access PSE", 21, pse_value_supported_repo);
    }
    /* DID Record */
    {
        UINT16 spec_id = 0x0102;
        UINT16 vendor_id = 0xFFFF;
        UINT16 prod_id = 0xFFFF;
        UINT16 ver_attr = 0x0620;
        UINT16 vendor_src = 0x0001;
        UINT8  primary_record = 0x01;

        db_add_did_database_record
        (
            (UINT8 *)"DeviceID", 9,
            (UINT8 *)"url executable", 15,
            (UINT8 *)"url documentation", 18,
            spec_id,
            vendor_id,
            prod_id,
            ver_attr,
            vendor_src,
            primary_record
        );
    }

    /* MAP_MCE Record */
    {
        UINT32 mce_supported_features = 0x0077FFFF;
        UINT16 mce_obex_psm = 0x8603;

        db_add_mce_database_record
        (
            (UINT8 *)"MAP Notification Service", 25,
            mce_obex_psm,
            mce_supported_features
        );
    }
    /* MPS Record */
    {
        UINT8 supported_scenarios_mpsd[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        UINT8 supported_scenarios_mpmd[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        UINT16 supported_proto_deps = 0x0000;

        db_add_mps_database_record
        (
            supported_scenarios_mpsd,
            supported_scenarios_mpmd,
            supported_proto_deps
        );
    }
}
#endif /* SDP_DYNAMIC_DB */
#endif /* SDP_SERVER */
