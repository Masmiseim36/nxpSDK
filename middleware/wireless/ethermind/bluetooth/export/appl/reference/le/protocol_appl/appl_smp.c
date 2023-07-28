/**
 *  \file appl_smp.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */


/* --------------------------------------------- Header File Inclusion */
#include "appl_smp.h"
#include "appl_gatt_client.h"
#include "appl_gatt_server.h"
#include "appl_utils.h"
#include "appl_hci_le.h"
#include "appl_service.h"

#ifdef AES_CMAC_UTIL
#include "aes_cmac.h"
#endif /* AES_CMAC_UTIL */

#ifdef BT_SECURITY_VU_VALIDATION
#include "BT_security.h"
#endif /* BT_SECURITY_VU_VALIDATION */

#ifdef SMP

/* --------------------------------------------- External Global Variables */
/* Enable this if to validate strict key size over LE Encryption for CTKD */
/* #define APPL_SMP_VALIDATE_KEYSIZE_FOR_CTKD */

/* Application compilation switch to enable deterministic PASSKEY support */
/* #define APPL_SMP_UPDATE_PASSKEY */

/* Application compilation switch to enable auto population of Peer Address */
#define APPL_SMP_AUTO_FILL_BD_ADDR

/**
 * Application compilation switch to enable auto confirmation of Numeric
 * Comparison Value.
 * This flag is disabled by default.
 * The Numeric Comparison Confirmation Mode can be configured through menu
 * option. This can be switched dynamically between automatic or manual
 * confirmation of Numeric key value.
 */
/* #define APPL_SMP_NC_AUTO_REPLY */

/**
 * The below piece of flag is needed only for SMP LESC OOB Testcases
 * while testing against PTS v 7.6.1.
 */
/* Application compilation switch to enable */
#define APPL_SMP_SUPPORT_REVERSE_INPUT_STRING

#if ((defined HCI_LE_PRIVACY_1_2_SUPPORT) && (defined HCI_LE_SET_PRIVACY_MODE_SUPPORT))
/**
 * This below feature enables the application to automatically add a Bonded
 * device to the Resolving List and also set its Privacy Mode to Device Privacy.
 *
 * NOTE:
 * -----
 * Even if this is added, the application later needs to ensure that if
 * there is a BT OFF-ON called after this, or if the IUT is put through a power
 * cycle i.e. when ever there is  Controller Reset, The Application needs to
 * ensure that all the associated Bonded devices retrieved from persistant
 * storage are added into the resolving list etc.
 *
 * This feature is dependant on:
 *   1. HCI_LE_PRIVACY_1_2_SUPPORT -> For Addition into the Resolving list
 *
 * This feature is disabled by default.
 * It is observed in few controllers that, after a device is added to a
 * resolving list. Any ADV packet with the added Peer Identity addresss is not
 * forwarded to the Host as an ADV event even if
 *   1. the IUT is not scanning with privacy enabled and
 *   2. the Host has not enabled the address resolution.
 */
/* #define APPL_SMP_AUTO_ADD_TO_RSLV_LIST */
#endif /* defined HCI_LE_PRIVACY_1_2_SUPPORT */

/* --------------------------------------------- External Global Variables */
#ifdef APPL_SMP_AUTO_FILL_BD_ADDR
extern BT_DEVICE_ADDR g_bd_addr;
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* Application automatic SMP handling identifier */
DECL_STATIC UCHAR automatic_mode = SMP_TRUE;

/* Application automatic SMP numeric key comparison identifier */
DECL_STATIC UCHAR nc_automatic_mode = SMP_TRUE;

/* Application SMP pairable mode */
DECL_STATIC UCHAR pairable_mode = SMP_TRUE;

#ifdef SMP_LESC
/* Applcation SMP Pairing Mode [Legacy or LESC] */
DECL_STATIC UCHAR g_appl_pair_mode = SMP_LESC_MODE;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
DECL_STATIC UCHAR g_appl_xtx_mode = BT_FALSE;
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#ifdef HAVE_SMP_REPEAT_TRACKING
/* Application Repeat Attempt track timer */
#define SMP_REPEAT_ATTEMPT_TIMEOUT_MIN        8U
#define SMP_REPEAT_ATTEMPT_TIMEOUT_MAX        1024U
#define SMP_REPEAT_ATTEMPT_TIMEOUT_FRACTION   1U /* Power of 2 */

DECL_STATIC BT_timer_handle smp_repeat_timer = BT_TIMER_HANDLE_INIT_VAL;
DECL_STATIC void smp_repeat_timeout_handler (void * data_param, UINT16 datalen);
DECL_STATIC UINT32 smp_repeat_attempt_timeout = SMP_REPEAT_ATTEMPT_TIMEOUT_MIN;
#endif /* HAVE_SMP_REPEAT_TRACKING */

#ifdef SMP_DATA_SIGNING
DECL_STATIC UCHAR * sign_data = NULL;
DECL_STATIC UINT16 sign_datalen;
DECL_STATIC UINT32 sign_create_counter;
DECL_STATIC UINT32 sign_verify_counter;
DECL_STATIC UCHAR * sign_key;
DECL_STATIC UCHAR sign_mac[SMP_SIGN_MAC_SIZE];
DECL_STATIC UCHAR signature[sizeof(sign_create_counter) + SMP_SIGN_MAC_SIZE];
#endif /* SMP_DATA_SIGNING */

DECL_STATIC SMP_KEY_DIST peer_key_info;
DECL_STATIC SMP_KEY_DIST local_key_info;
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
DECL_STATIC UCHAR peer_keys;
DECL_STATIC UCHAR local_keys;
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

DECL_STATIC UCHAR        appl_user_irk[SMP_IRK_SIZE];

#ifdef APPL_SMP_UPDATE_PASSKEY
#define APPL_SMP_PASSKEY                        999999
#endif /* APPL_SMP_UPDATE_PASSKEY */

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
UCHAR appl_smp_lesc_test_f5_state = 0xFF;
UCHAR appl_smp_lesc_test_f5_T[16];
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
DECL_STATIC DECL_CONST UCHAR appl_smp_menu[] = "\n\
============== SMP MENU ================\n\
 0.  Exit.\n\
 1.  Refresh.\n\
 \n\
 2.  Authenticate Remote Device \n\
 3.  Authentication Request Reply \n\
 \n\
 5.  Encryption Request \n\
 \n\
 10. User Pass Key Request Reply \n\
 11. User Numeric Confirmation Reply \n\
 \n\
 20. Register SMP Callback \n\
 21. Set Automatic Mode \n\
 22. Set Pairable Mode \n\
 23. Set NC Automatic Mode \n\
 \n\
 30. Create Resolvable Private Address \n\
 31. Resolve Private Address \n\
 \n\
 40. Get device security information \n\
 \n\
 50. Platform Configuration \n\
 \n\
 60. Generate Signed Data \n\
 61. Verify Signed Data \n\
 \n\
 70. Test AES-CMAC \n\
 71. Test LESC F4 func \n\
 72. Test LESC F5 func \n\
 73. Test LESC F6 func \n\
 74. Test LESC g2 func \n\
 75. Test LESC h6 func \n\
 \n\
 100. Enable Cross Transport Inputs \n\
 101. Authenticate Remote Device on BREDR \n\
 \n\
 200. Initiate Disconnection \n\
 \n\
Enter Your Choice: ";
#else /* SMP_TBX_TEST_LESC_FUNCTIONS */
DECL_STATIC DECL_CONST UCHAR appl_smp_menu[] = "\n\
============== SMP MENU ================\n\
 0.  Exit.\n\
 1.  Refresh.\n\
 \n\
 2.  Authenticate Remote Device \n\
 3.  Authentication Request Reply \n\
 \n\
 5.  Encryption Request \n\
 \n\
 10. User Pass Key Request Reply \n\
 11. User Numeric Confirmation Reply \n\
 \n\
 20. Register SMP Callback \n\
 21. Set Automatic Mode \n\
 22. Set Pairable Mode \n\
 23. Set NC Automatic Mode \n\
 \n\
 30. Create Resolvable Private Address \n\
 31. Resolve Private Address \n\
 \n\
 40. Get device security information \n\
 \n\
 50. Platform Configuration \n\
 \n\
 60. Generate Signed Data \n\
 61. Verify Signed Data \n\
 \n\
 100. Enable Cross transport inputs \n\
 101. Authenticate Remote Device on BREDR \n\
 \n\
 199. Initiate Connection \n\
 200. Initiate Disconnection \n\
 \n\
Enter Your Choice: ";
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

DECL_STATIC DECL_CONST UCHAR appl_smp_pl_menu[] = "\n\
============= SMP PL MENU ==============\n\
 0.  Exit.\n\
 1.  Refresh.\n\
 \n\
 2.  Get Configuration Parameters \n\
 \n\
 3.  Set MITM policy \n\
 4.  Set LESC policy \n\
 5.  Set IO Capability \n\
 6.  Set OOB Information \n\
 7.  Set Encryption Key Size \n\
 8.  Set Key Distribution \n\
 9.  Set KeyPress Notification Support \n\
 \n\
 10. Mark Device Untrusted \n\
 \n\
 15. Generate LESC OOB Data \n\
 \n\
 16. Generate Cross Transport Keys \n\
 \n\
 20. Get Platform Keys \n\
 21. Generate Platform keys \n\
 \n\
 30. Add device to PL database \n\
 31. Remove device from PL database \n\
 \n\
 40. Set Platform Passkey Update Mode \n\
 41. Set SMP desired pairing Mode \n\
 42. Enable/Disable Cross Transport Selection \n\
 \n\
 50. Reverse Byte Stream \n\
Enter Your Choice: ";

/* Global BD Address of the SMP procedure */
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
DECL_STATIC BT_DEVICE_ADDR appl_smp_bd_addr;
#endif

/* Enable/Disable Cross transport inputs */
DECL_STATIC UCHAR appl_smp_xtxp_enable;

/* --------------------------------------------- Functions */
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
void appl_smp_test_f4 (void);
void appl_smp_test_f5 (void);
void appl_smp_test_f5_temp (void);
void appl_smp_test_f5_ltk (UCHAR * enc_key);
void appl_smp_test_f5_mackey (UCHAR * enc_key);
void appl_smp_test_f6 (void);
void appl_smp_test_g2 (void);
void appl_smp_test_h6 (void);
void appl_smp_invert_byte_stream (UCHAR *buffer, UINT16 buffer_len);
void appl_smp_lesc_test_funcs_complete
     (
         UCHAR                        * plain_text,
         UINT16                       plain_text_len,
         UCHAR                        * key,
         UCHAR                        aes_op,
         UCHAR                        * enc_out
     );
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */


#if (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT)
void appl_smp_lesc_oob_data_complete(SMP_LESC_OOB_DATA_PL * appl_lesc_oob)
{
    UINT32 i;

    APPL_TRC("\n Generated SMP OOB CONFIRM Value: ");
    APPL_TRC("\nCNF Value [LSB-MSB]: ");
    for (i = 0U; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
    {
        APPL_TRC("%02X ", appl_lesc_oob->cnf_val[i]);
    }
    APPL_TRC("\n CNF Value[MSB-LSB]:(spaced) ");
    for (i = 0U; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
    {
        APPL_TRC("%02X ", appl_lesc_oob->cnf_val[(SMP_OOB_CONFIRM_VAL_SIZE - 1U) - i]);
    }
    APPL_TRC("\n CNF Value[MSB-LSB]: ");
    for (i = 0U; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
    {
        APPL_TRC("%02X", appl_lesc_oob->cnf_val[(SMP_OOB_CONFIRM_VAL_SIZE - 1U) - i]);
    }

    APPL_TRC("\n Generated SMP OOB RAND Value: ");
    APPL_TRC("\nRAND Value [LSB-MSB]: ");
    for (i = 0U; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
    {
        APPL_TRC("%02X ", appl_lesc_oob->rand[i]);
    }
    APPL_TRC("\nRAND Value [MSB-LSB]:(spaced) ");
    for (i = 0U; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
    {
        APPL_TRC("%02X ", appl_lesc_oob->rand[(SMP_OOB_RANDOM_VAL_SIZE - 1U) - i]);
    }
    APPL_TRC("\nRAND Value [MSB-LSB]: ");
    for (i = 0U; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
    {
        APPL_TRC("%02X", appl_lesc_oob->rand[(SMP_OOB_RANDOM_VAL_SIZE - 1U) - i]);
    }

    APPL_TRC("\n\n");
}
#endif /* (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT) */

#ifdef SMP_LESC
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
void appl_smp_lesc_xtxp_ltk_complete(SMP_LESC_LK_LTK_GEN_PL * xtxp)
{
    API_RESULT retval;
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth_info;
    UCHAR lkey[BT_LINK_KEY_SIZE];
    UCHAR lkey_type;

    APPL_TRC("\n LTK of the device is ...\n");
    appl_dump_bytes(xtxp->ltk, SMP_LTK_SIZE);

    APPL_TRC("\n LK of the device is ...\n");
    appl_dump_bytes(xtxp->lk, SMP_LK_SIZE);

    /* Update the LTK. Key strength checked before generation */
    printf("Adding Device to Device SMP DB .. %02X:%02X:%02X:%02X:%02X:%02X - 0x%02X\n",
    appl_smp_bd_addr.addr[0], appl_smp_bd_addr.addr[1], appl_smp_bd_addr.addr[2],
    appl_smp_bd_addr.addr[3], appl_smp_bd_addr.addr[4], appl_smp_bd_addr.addr[5],
    appl_smp_bd_addr.type);

    retval = BT_sm_get_device_link_key_and_type(appl_smp_bd_addr.addr, lkey, &lkey_type);

    if ((API_SUCCESS == retval) &&
        ((HCI_LINK_KEY_AUTHENTICATED_P_256 == lkey_type) ||
        (HCI_LINK_KEY_UNAUTHENTICATED_P_256 == lkey_type)))
    {
        (BT_IGNORE_RETURN_VALUE)BT_smp_add_device(&appl_smp_bd_addr, &bd_handle);

        auth_info.bonding = SMP_BONDING;
        auth_info.pair_mode = SMP_LESC_MODE;
        auth_info.security = (HCI_LINK_KEY_AUTHENTICATED_P_256 == lkey_type)?
            SMP_SEC_LEVEL_2: SMP_SEC_LEVEL_1;

        /* Update the keys */
        BT_mem_copy(peer_key_info.enc_info, xtxp->ltk, 16U);
        (BT_IGNORE_RETURN_VALUE)BT_smp_update_security_info
        (
            &bd_handle,
            &auth_info,
            16U,
            local_keys,
            peer_keys,
            &peer_key_info
        );
    }
}

void appl_smp_lesc_xtxp_lk_complete(SMP_LESC_LK_LTK_GEN_PL * xtxp)
{
    API_RESULT retval;
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth;
    UCHAR type;

    APPL_TRC("\n LK of the device is ...\n");
    appl_dump_bytes(xtxp->lk, SMP_LK_SIZE);

    APPL_TRC("\n LTK of the device is ...\n");
    appl_dump_bytes(xtxp->ltk, SMP_LTK_SIZE);

    /* Update the LinkKey. Key strength checked before generation */
    printf("Adding Device to Device SM DB .. %02X:%02X:%02X:%02X:%02X:%02X\n",
    appl_smp_bd_addr.addr[0], appl_smp_bd_addr.addr[1], appl_smp_bd_addr.addr[2],
    appl_smp_bd_addr.addr[3], appl_smp_bd_addr.addr[4], appl_smp_bd_addr.addr[5]);

    /* Get the BD handle */
    (BT_IGNORE_RETURN_VALUE)BT_smp_get_bd_handle(&g_bd_addr, &bd_handle);

    /* Initialize */
    BT_mem_set(&auth, 0x00, sizeof(SMP_AUTH_INFO));

    retval = BT_smp_get_device_security_info
             (
                 &bd_handle,
                 &auth
             );
    if (API_SUCCESS == retval)
    {
        type = (SMP_SEC_LEVEL_2 == auth.security) ?
            HCI_LINK_KEY_AUTHENTICATED_P_256 : HCI_LINK_KEY_UNAUTHENTICATED_P_256;

        (BT_IGNORE_RETURN_VALUE) BT_sm_add_device(appl_smp_bd_addr.addr);
        (BT_IGNORE_RETURN_VALUE) BT_sm_set_device_link_key_and_type(appl_smp_bd_addr.addr, xtxp->lk, &type);
    }
}

void appl_smp_lesc_txp_key_gen_complete (SMP_LESC_LK_LTK_GEN_PL * appl_txp_key)
{
    APPL_TRC("\n LTK of the device is ...\n");
    appl_dump_bytes(appl_txp_key->ltk,SMP_LTK_SIZE);

    APPL_TRC("\n LK of the device is ...\n");
    appl_dump_bytes(appl_txp_key->lk,SMP_LK_SIZE);

    APPL_TRC("\n Temp output of the h6 is ...\n");
    appl_dump_bytes(appl_txp_key->temp,SMP_AES_CMAC_MAC_SIZE);
}
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#ifdef APPL_SMP_SUPPORT_REVERSE_INPUT_STRING
/* Convert string to Integer Array */
#define IS_SPACE(c) ((' ' == (c)) || ('\t' == (c)))
#define IS_DIGIT(c) (('0' <= (c)) && ('9' >= (c)))
#define IS_UPPER(c) (('A' <= (c)) && ('F' >= (c)))
#define IS_LOWER(c) (('a' <= (c)) && ('f' >= (c)))
#define IS_ALPHA(c) IS_LOWER(c) || IS_UPPER(c)
DECL_STATIC API_RESULT appl_smp_strtoarray
                       (
                           /* IN */  UCHAR  * data,
                           /* IN */  UINT16   data_length,
                           /* OUT */ UINT8  * output_array,
                           /* IN */  UINT16   output_array_len
                       )
{
    INT32  index;
    UINT8  c0, c1;
    UINT8  base;
    UINT16 output_index;

    /* HEX */
    base = 16U;
    c0 = 0U;
    c1 = 0U;

    /* Fill with Zeros */
    BT_mem_set(output_array, 0, output_array_len);

    /* Check the length */
    if (data_length > (2U * output_array_len))
    {
        return API_FAILURE;
    }

    /* Process from end */
    output_index = output_array_len - 1U;
    for (index = data_length - 1; index >= 0; index -= 2)
    {
        if (0 != index)
        {
            c1 = data[index];
            c0 = data[index - 1];
        }
        else
        {
            c1 = data[index];
            c0 = '0';
        }

        /* Check if Digit */
        if (IS_DIGIT(c0))
        {
            c0 = (c0 - '0');
        }
        else if (IS_LOWER(c0))
        {
            c0 = (c0 - 'a' + 10U);
        }
        else if (IS_UPPER(c0))
        {
            c0 = (c0 - 'A' + 10U);
        }
        else
        {
            return API_FAILURE;
        }

        /* Check if Digit */
        if (IS_DIGIT(c1))
        {
            c1 = (c1 - '0');
        }
        else if (IS_LOWER(c1))
        {
            c1 = (c1 - 'a' + 10U);
        }
        else if (IS_UPPER(c1))
        {
            c1 = (c1 - 'A' + 10U);
        }
        else
        {
            return API_FAILURE;
        }

        output_array[output_index] = c0 * base + c1;
        output_index--;
    }

    return API_SUCCESS;
}
#endif /* APPL_SMP_SUPPORT_REVERSE_INPUT_STRING */

void smp_pl_operations(void)
{
    unsigned int  choice, menu_choice;
    unsigned int  read_val;

    SMP_KEY_DIST * key_info;

    SMP_BD_ADDR   bd_addr;
    SMP_BD_HANDLE bd_handle;

    API_RESULT    retval;
    UCHAR iocap, oob, keys, ekeysize;

#ifdef SMP_HAVE_OOB_SUPPORT
    SMP_OOB_DATA oob_val = {0U};
    UCHAR         i;

    /* MISRA C-2012 Rule 9.1 */
    BT_mem_set(&oob_val, 0x0, sizeof(SMP_OOB_DATA));
#endif /* SMP_HAVE_OOB_SUPPORT */

    BT_LOOP_FOREVER()
    {
        retval = API_SUCCESS;

        APPL_TRC("%s", appl_smp_pl_menu);
        CONSOLE_IN("%u", &choice);
        menu_choice = choice;

        switch(choice)
        {
            case 0:
                break; /* return; */

            case 1:
                APPL_TRC("Refreshing...\n");
                break;

            case 2:
#ifdef SMP_HAVE_OOB_SUPPORT
                CONSOLE_OUT ("Enter peer device handle for OOB capability: ");
                CONSOLE_IN("%u", &read_val);

                bd_handle = (SMP_BD_HANDLE)read_val;
                smp_get_peer_oob_availability_pl (&bd_handle, &oob);
#else /* SMP_HAVE_OOB_SUPPORT */
                oob = 0x00U;
#endif /* SMP_HAVE_OOB_SUPPORT */

                (BT_IGNORE_RETURN_VALUE) BT_smp_get_local_capability_pl(&iocap, &keys, &ekeysize);

                APPL_TRC ("IO Capability : 0x%02X\n", iocap);
                APPL_TRC ("OOB : 0x%02X\n", oob);

#ifdef SMP_HAVE_OOB_SUPPORT
                if (0U != oob)
                {
                    (BT_IGNORE_RETURN_VALUE) smp_get_oob_tk_pl (&bd_handle, oob_val.temp_key);

                    APPL_TRC ("OOB Data : ");
                    for (i = 0U; i < SMP_OOB_TEMP_KEY_SIZE; i++)
                    {
                        APPL_TRC ("%x ", oob_val.temp_key[i]);
                    }
                    APPL_TRC ("\n");
                }
#endif /* SMP_HAVE_OOB_SUPPORT */

                APPL_TRC ("Key Exchange : 0x%02X\n", keys);
                APPL_TRC ("Max Enc Key Size : 0x%02X\n", ekeysize);
                break;

            case 3:
                APPL_TRC("Enter Platform MITM policy\n");
                APPL_TRC("(0-MITM Best Effor, 1-MITM Strict): ");
                CONSOLE_IN("%u", &read_val);

                BT_smp_set_mitm_policy_pl ((UCHAR)read_val);
                break;

            case 4:
#ifdef SMP_LESC
                APPL_TRC("Enter Platform LESC policy\n");
                APPL_TRC("(0-LESC Best Effor, 1-LESC Strict): ");
                CONSOLE_IN("%u", &read_val);

                BT_smp_set_lesc_policy_pl ((UCHAR)read_val);
#else /* SMP_LESC */
                APPL_TRC("\nSMP LE Secure Connections not enabled!!!\n");
#endif /* SMP_LESC */
                break;

            case 5:
                APPL_TRC("Enter Platform IO Capability\n");
                APPL_TRC("(0-DisplayOnly, 1-DisplayYesNo, 2-KeybdOnly, 3-NoIO, 4-KeybdDisplay, 5-Default): ");
                CONSOLE_IN("%u", &read_val);

                (BT_IGNORE_RETURN_VALUE) BT_smp_set_io_cap_pl ((UCHAR)read_val);
                break;

            case 6:
#ifdef SMP_HAVE_OOB_SUPPORT
                CONSOLE_OUT ("Enter peer BD Address: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT ("Enter peer BD Type: ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR)read_val;

                APPL_TRC("Enter OOB Flag\n");
                APPL_TRC("(1: OOB data present, 0: OOB data not present): ");
                CONSOLE_IN("%u", &read_val);

                if (0U != read_val)
                {
                    unsigned int sub_read;

                    APPL_TRC("Is TK value Present?\n");
                    APPL_TRC("(1:  TK present, 0: TK not present): ");
                    CONSOLE_IN("%u", &sub_read);

                    if (0U != sub_read)
                    {
#ifdef APPL_SMP_HAVE_CONST_OOB_TK_VAL
                        BT_mem_set(oob_val.temp_key, 0x11, SMP_OOB_TEMP_KEY_SIZE);
#else /* APPL_SMP_HAVE_CONST_OOB_TK_VAL */
                        APPL_TRC ("Enter %d octet OOB Tk (in HEX): ", SMP_OOB_TEMP_KEY_SIZE);
                        for (i = 0U; i < SMP_OOB_TEMP_KEY_SIZE; i++)
                        {
                            CONSOLE_IN ("%x", &choice);
                            oob_val.temp_key[i] = (UCHAR)choice;
                        }
#endif /* APPL_SMP_HAVE_CONST_OOB_TK_VAL */
                    }
#ifdef SMP_LESC
                    APPL_TRC("Is LESC CNF value Present?\n");
                    APPL_TRC("(1:  CNF present, 0: CNF not present): ");
                    CONSOLE_IN("%u", &sub_read);

                    if (0U != sub_read)
                    {
                        APPL_TRC ("Enter %d octet OOB CNF (in HEX): ", SMP_OOB_CONFIRM_VAL_SIZE);
                        for (i = 0U; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
                        {
                            CONSOLE_IN ("%x", &choice);
                            oob_val.lesc_cnf_val[i] = (UCHAR)choice;
                        }

                        APPL_TRC("Is LESC Rand value Present?\n");
                        APPL_TRC("(1:  Rand present, 0: Rand not present): ");
                        CONSOLE_IN("%u", &sub_read);

                        if (0U != sub_read)
                        {
                            APPL_TRC ("Enter %d octet OOB Rand (in HEX): ", SMP_OOB_RANDOM_VAL_SIZE);
                            for (i = 0U; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
                            {
                                CONSOLE_IN ("%x", &choice);
                                oob_val.lesc_rand[i] = (UCHAR)choice;
                            }
                        }
                        else
                        {
                            BT_mem_set(oob_val.lesc_rand,0x00,SMP_OOB_RANDOM_VAL_SIZE);
                        }
                    }
#endif /* SMP_LESC */
                }

                /* Remove device if already added */
                BT_smp_remove_device_pl(&bd_addr);

                retval = BT_smp_add_device_pl(&bd_addr);
                APPL_TRC("\n Adding Device to PL List returned 0x%04X.\n",retval);

                if (API_SUCCESS == retval)
                {
                    retval = BT_smp_set_oob_data_pl (&bd_addr, (UCHAR)read_val, &oob_val, NULL, NULL);
                    APPL_TRC("Peer OOB Data Setting returned 0x%04X.\n",retval);
                }

#else /* SMP_HAVE_OOB_SUPPORT */
                CONSOLE_OUT ("'SMP_HAVE_OOB_SUPPORT' flag not defined\n");
#endif /* SMP_HAVE_OOB_SUPPORT */

                break;

            case 7:
                APPL_TRC("Enter Encryption Key Size (7 < keysize < 16): \n");
                CONSOLE_IN("%u", &read_val);

                retval = BT_smp_set_max_encryption_key_size_pl ((UCHAR)read_val);

                if (API_SUCCESS != retval)
                {
                    CONSOLE_OUT ("Invalid parameter\n");
                }
                break;

            case 8:
                APPL_TRC("Enter 1 octet Key Distribution flag (in HEX): \n");
                APPL_TRC(" BIT 0-Local Encryption Info\n");
                APPL_TRC(" BIT 1-Local Identity Info\n");
                APPL_TRC(" BIT 2-Local Signing Info\n");
                APPL_TRC(" BIT 4-Remote Encryption Info\n");
                APPL_TRC(" BIT 5-Remote Identity Info\n");
                APPL_TRC(" BIT 6-Remote Signing Info\n");
                CONSOLE_IN("%x", &read_val);

                (BT_IGNORE_RETURN_VALUE) BT_smp_set_key_distribution_flag_pl ((UCHAR)read_val);
                break;

            case 9:
#ifdef SMP_LESC
                APPL_TRC("Enter Platform KeyPress Notification Support\n");
                APPL_TRC("(0-RESET, 1-SET): ");
                CONSOLE_IN("%u", &read_val);

                BT_smp_set_key_press_ntf_pl ((UCHAR)read_val);
#else /* SMP_LESC */
                APPL_TRC("\nSMP LE Secure Connections not enabled!!!\n");
#endif /* SMP_LESC */
                break;

            case 10:
                APPL_TRC("Enter BD_ADDR: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                APPL_TRC("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE) BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                retval = BT_smp_mark_device_untrusted_pl(&bd_handle);

                CONSOLE_OUT("Made Device Untrusted with result 0x%04X",retval);
                break;

            case 15:
#ifdef SMP_LESC
                APPL_TRC("\nGenerate SMP LESC OOB Data from PL...\n");
                (BT_IGNORE_RETURN_VALUE) BT_smp_generate_lesc_oob_local_data_pl(appl_smp_lesc_oob_data_complete);
#else /* SMP_LESC */
#endif /* SMP_LESC */
                break;

            case 16:
#ifdef SMP_LESC
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                APPL_TRC("Enter the key to be Generated ...\n");
                APPL_TRC("\t0. LTK\n\t1. LK\n");

                APPL_TRC("Generate SMP LESC LK ...\n");

                CONSOLE_IN("%u", &read_val);

                if (0U != read_val)
                {
                    UCHAR key[SMP_LTK_SIZE];
                    UCHAR ct2;

                    APPL_TRC ("Enter %d octet of LTK (in HEX): ", SMP_LTK_SIZE);
                    for (i = 0U; i < SMP_LTK_SIZE; i++)
                    {
                        CONSOLE_IN ("%x", &choice);
                        key[i] = (UCHAR)choice;
                    }

                    APPL_TRC("Enter the CT2: ");
                    CONSOLE_IN("%x", &choice);
                    ct2 = (UCHAR)choice;

                    APPL_TRC("Generate SMP LESC LK ...\n");
                    (BT_IGNORE_RETURN_VALUE) BT_smp_get_lk_from_ltk_pl(key,appl_smp_lesc_txp_key_gen_complete,ct2);
                }
                else
                {
                    UCHAR key[SMP_LK_SIZE];
                    UCHAR ct2;

                    APPL_TRC ("Enter %d octet of LK (in HEX): ", SMP_LK_SIZE);
                    for (i = 0U; i < SMP_LK_SIZE; i++)
                    {
                        CONSOLE_IN ("%x", &choice);
                        key[i] = (UCHAR)choice;
                    }

                    APPL_TRC("Enter the CT2: ");
                    CONSOLE_IN("%x", &choice);
                    ct2 = (UCHAR)choice;

                    APPL_TRC("Generate SMP LESC LTK ...\n");
                    (BT_IGNORE_RETURN_VALUE) BT_smp_get_ltk_from_lk_pl(key,appl_smp_lesc_txp_key_gen_complete,ct2);
                }
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
                APPL_TRC("SMP_LESC_CROSS_TXP_KEY_GEN not enabled");
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#else /* SMP_LESC */
                APPL_TRC("SMP_LESC_CROSS_TXP_KEY_GEN not enabled");
#endif /* SMP_LESC */
                break;

            case 20:
                (BT_IGNORE_RETURN_VALUE) BT_smp_get_key_exchange_info_pl (&key_info);

                APPL_TRC ("Encryption Info:\n");
                appl_dump_bytes(key_info->enc_info, sizeof (key_info->enc_info));
                APPL_TRC ("Master Identification Info:\n");
                appl_dump_bytes(key_info->mid_info, sizeof (key_info->mid_info));
                APPL_TRC ("Identity Info:\n");
                appl_dump_bytes(key_info->id_info, sizeof (key_info->id_info));
                APPL_TRC ("Identity Address Info:\n");
                appl_dump_bytes(key_info->id_addr_info, sizeof (key_info->id_addr_info));
                APPL_TRC ("Signature Info:\n");
                appl_dump_bytes(key_info->sign_info, sizeof (key_info->sign_info));

                break;

            case 21:
#ifdef SMP_PL_ENABLE_KEY_GENERATION
                APPL_TRC ("Enter Diversifier value: ");
                CONSOLE_IN("%d", &choice);
                BT_smp_generate_keys_pl((UINT16)choice);
#else /* SMP_PL_ENABLE_KEY_GENERATION */
                CONSOLE_OUT ("SMP_PL_ENABLE_KEY_GENERATION not defined\n");
#endif /* SMP_PL_ENABLE_KEY_GENERATION */
                break;

            case 30:
#ifdef SMP_HAVE_OOB_SUPPORT
                CONSOLE_OUT ("Enter peer BD Address: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                APPL_TRC("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;

                retval = BT_smp_add_device_pl (&bd_addr);
                CONSOLE_OUT ("Add device - 0x%04X\n", retval);
#else /* SMP_HAVE_OOB_SUPPORT */
                CONSOLE_OUT ("'SMP_HAVE_OOB_SUPPORT' flag not defined\n");
#endif /* SMP_HAVE_OOB_SUPPORT */
                break;

            case 31:
#ifdef SMP_HAVE_OOB_SUPPORT
                CONSOLE_OUT ("Enter peer BD Address: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                APPL_TRC("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;

                retval = BT_smp_remove_device_pl (&bd_addr);
                CONSOLE_OUT ("Remove device - 0x%04X\n", retval);
#else /* SMP_HAVE_OOB_SUPPORT */
                CONSOLE_OUT ("'SMP_HAVE_OOB_SUPPORT' flag not defined\n");
#endif /* SMP_HAVE_OOB_SUPPORT */
                break;

            case 40:
                {
                    UCHAR mode;

                    CONSOLE_OUT("\nEnter the Platform Mode for Passkey Updation : ");
                    CONSOLE_OUT("\n 1 - Use Modulo Operations");
                    CONSOLE_OUT("\n 2 - Use Masking Operations");
                    CONSOLE_OUT("\nYour Choice?");
                    CONSOLE_IN("%u", &read_val);
                    mode = (UCHAR) read_val;

                    (BT_IGNORE_RETURN_VALUE) BT_smp_set_passkey_revamp_mode_pl(mode);
                }
                break;

            case 41:
            #ifdef SMP_LESC
                {
                    CONSOLE_OUT("\nEnter the desired Pairing Mode for Platform : ");
                    CONSOLE_OUT("\n 1 - Use SMP Legacy Mode");
                    CONSOLE_OUT("\n 2 - Use SMP LE Secure Connections Mode");
                    CONSOLE_OUT("\nYour Choice?");
                    CONSOLE_IN("%u", &read_val);

                    g_appl_pair_mode = (0x01U == (UCHAR) read_val) ? SMP_LEGACY_MODE : SMP_LESC_MODE;
                }
            #else
                CONSOLE_OUT("\nSMP_LESC feature flag is not defined.\n");
            #endif /* SMP_LESC */
                break;

            case 42:
            #ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                {
                    CONSOLE_OUT("\nEnter the desired usage state for Cross Transport Mode: ");
                    CONSOLE_OUT("\n 0 - Disable Cross Transport Support");
                    CONSOLE_OUT("\n 1 - Enable Cross Transport Support");
                    CONSOLE_OUT("\nYour Choice?");
                    CONSOLE_IN("%u", &read_val);

                    g_appl_xtx_mode = (0x01U == (UCHAR) read_val) ? BT_TRUE : BT_FALSE;
                }
            #else
                CONSOLE_OUT("\nSMP_LESC_CROSS_TXP_KEY_GEN feature flag is not defined.\n");
            #endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
                break;

            case 50:
#ifdef APPL_SMP_SUPPORT_REVERSE_INPUT_STRING
                {
                    UCHAR test_byte[100U];
                    UCHAR op[16U];
                    UINT16 len;

                    BT_mem_set(test_byte, 0x0, sizeof(test_byte));

                    CONSOLE_OUT("\nEnter the octet stream to be reversed (in HEX) : \n");
                    /**
                     * Typically, this needs to be fscaf from STDIN.
                     * To remove warnings its changed to CONSOLE_IN and the
                     * entire block is protected by a flag.
                     */
                    /* fscanf(stdin, "%99[^\n]%*c", test_byte); */
                    CONSOLE_IN("%s", test_byte);

                    len = (UINT16)BT_str_len(test_byte);

                    APPL_TRC ("Total Length of Data Entered is: %d", len);

                    APPL_TRC("\nEntered value in reverse is: ");
                    appl_smp_strtoarray
                    (
                        test_byte,
                        len,
                        &op[0U],
                        16U
                    );

                    for (i = 0U; i < 16U; i++)
                    {
                        APPL_TRC("%02X ", op[(16U - 1U) - i]);
                    }

                    APPL_TRC("\n\n");
                }
#else /* APPL_SMP_SUPPORT_REVERSE_INPUT_STRING */
                APPL_TRC("APPL_SMP_SUPPORT_REVERSE_INPUT_STRING feature flag is not defined.\n");
#endif /* APPL_SMP_SUPPORT_REVERSE_INPUT_STRING */
                break;

            default:
                CONSOLE_OUT("Invalid Choice\n");
                break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

void main_smp_operations(void)
{
    int  choice, menu_choice;
    int  read_val;

    UINT32        read_passkey;
    API_RESULT    retval;

    SMP_KEY_DIST  * key_info;
    SMP_BD_ADDR   bd_addr;
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth;

    UCHAR         smp_accept, i;

#ifdef SMP_HAVE_ENCRYPT_API
    UINT16        ediv;
    UCHAR         rnd[SMP_RAND_64B_SIZE];
    UCHAR         ltk[SMP_LTK_SIZE];
#endif /* SMP_HAVE_ENCRYPT_API */

#ifdef SMP_LESC
    UCHAR ntf_val;
#endif /* SMP_LESC */

    /* MISRA C-2012 Rule 9.1 */
    read_val = 0;

    BT_LOOP_FOREVER()
    {
        retval = API_SUCCESS;

        CONSOLE_OUT("%s", appl_smp_menu);
        CONSOLE_IN("%u", &choice);
        menu_choice = choice;

        switch(choice)
        {
            case 0:
                break; /* return; */

            case 1:
                CONSOLE_OUT("Refreshing...\n");
                break;

            case 2:
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("\n0 - Public\n1 - Random\n");
                CONSOLE_OUT("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

#ifdef HAVE_SMP_REPEAT_TRACKING
                /* Check if repeat attempt tracking is on */
                if (BT_TIMER_HANDLE_INIT_VAL != smp_repeat_timer)
                {
                    BT_stop_timer (smp_repeat_timer);
                    smp_repeat_timer = BT_TIMER_HANDLE_INIT_VAL;
                }
#endif /* HAVE_SMP_REPEAT_TRACKING */

#ifdef SMP_LESC
                CONSOLE_OUT("0 - Legacy LE SMP Procedure\n");
                CONSOLE_OUT("1 - LE Secure Connections Mode\n");
                CONSOLE_OUT("Enter SMP Pairing Mode :");
                /* read value,max_strlength,min_menuoption,max_menuoption */
                retval = appl_validate_params(&read_val,1U,0U,1U);
                if (API_SUCCESS == retval)
                {
                    /* Call SMP LE Secure Connetions Mode Setting API here */
                    auth.pair_mode = (UCHAR)((read_val)?SMP_LESC_MODE:SMP_LEGACY_MODE);

                    /**
                     * Update the Default pairing mode related global to the value
                     * selected by User.
                     */
                    g_appl_pair_mode = auth.pair_mode;
                }
                else
                {
                    break;
                }

#endif /* SMP_LESC */

                CONSOLE_OUT(" 0 - Encryption Only/Unauthenticated (Without MITM)\n");
                CONSOLE_OUT(" 1 - Authenticated (With MITM)\n");
                CONSOLE_OUT("Enter Security level required : ");
                retval = appl_validate_params(&read_val,1U,0U,1U);
                if (API_SUCCESS == retval)
                {
                    auth.security = (UCHAR) ((read_val)?SMP_SEC_LEVEL_2: SMP_SEC_LEVEL_1);
                }
                else
                {
                    break;
                }

                CONSOLE_OUT("\n0 - non-Bonding\n1 - Bonding\n");
                CONSOLE_OUT("Enter Bonding type : ");
                retval = appl_validate_params(&read_val,1U,0U,1U);
                if (API_SUCCESS == retval)
                {
                    auth.bonding = (UCHAR) read_val;
                }
                else
                {
                    break;
                }

                CONSOLE_OUT("Enter Encryption Key size required : ");
                #ifndef SMP_NO_PARAM_CHECK
                    retval = appl_validate_params(&read_val,2U,SMP_MIN_ENCRYPTION_KEY_SIZE,SMP_MAX_ENCRYPTION_KEY_SIZE);
                #else
                    CONSOLE_IN("%u", &read_val);
                #endif
                if (API_SUCCESS == retval)
                {
                    auth.ekey_size = (UCHAR)read_val;
                }
                else
                {
                    break;
                }

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                if (BT_TRUE == appl_smp_xtxp_enable)
                {
                    CONSOLE_OUT("Enter transport (1-BREDR, 2-BLE): ");
                    CONSOLE_IN("%u", &read_val);
                    auth.transport = (UCHAR)read_val;

                    CONSOLE_OUT("Enter Cross Transport KeyGen (0-No, 1-Yes): ");
                    CONSOLE_IN("%u", &read_val);
                    auth.xtx_info = (read_val) ? SMP_XTX_KEYGEN_MASK : SMP_XTX_DISABLE;
                    auth.xtx_info |= SMP_XTX_H7_MASK;

#ifdef SMP_ENABLE_BLURTOOTH_VU_UPDATE
                    CONSOLE_OUT("Enter role requirement of local device (0-Master, 1-Slave): ");
                    CONSOLE_IN("%u", &read_val);
                    auth.role = (UCHAR)read_val;
#endif /* SMP_ENABLE_BLURTOOTH_VU_UPDATE */
                }
                else
                {
                    auth.transport = SMP_LINK_LE;
                    auth.xtx_info = SMP_XTX_DISABLE;
                }

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE) BT_smp_get_link_handle(&bd_addr, &bd_handle, auth.transport);
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

#ifdef BT_SECURITY_VU_VALIDATION
                if (BT_SECURITY_VU_ANSSI_REFLECT_CONFVAL == BT_security_vu_get())
                {
                    /* Update local SMP capabilities using smp_pl APIs if required here */
                    (BT_IGNORE_RETURN_VALUE)BT_smp_set_io_cap_pl(SMP_IO_CAPABILITY_DISPLAY_ONLY);
                }
                else
                {
                    /* Update local SMP capabilities using smp_pl APIs if required here */
                    (BT_IGNORE_RETURN_VALUE)BT_smp_set_io_cap_pl(SMP_IO_CAPABILITY_DISPLAY_YESNO);
                }
#endif /* BT_SECURITY_VU_VALIDATION */

                retval = BT_smp_authenticate
                         (
                             &bd_handle,
                             &auth
                         );

                CONSOLE_OUT ("BT_smp_authenticate, Retval - 0x%04X\n", retval);

                break;

            case 3:
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("\n0 - Public\n1 - Random\n");
                CONSOLE_OUT("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

#ifdef SMP_LESC
                CONSOLE_OUT("0 - Legacy LE SMP Procedure\n");
                CONSOLE_OUT("1 - LE Secure Connections Mode\n");
                CONSOLE_OUT("Enter SMP Pairing Mode :");
                CONSOLE_IN("%u", &read_val);

                /* Call SMP LE Secure Connetions Mode Setting API here */
                auth.pair_mode = (UCHAR)((read_val)?SMP_LESC_MODE:SMP_LEGACY_MODE);

                /**
                 * Update the Default pairing mode related global to the value
                 * selected by User.
                 */
                g_appl_pair_mode = auth.pair_mode;
#endif /* SMP_LESC */

                CONSOLE_OUT(" 0 - Encryption Only/Unauthenticated (Without MITM)\n");
                CONSOLE_OUT(" 1 - Authenticated (With MITM)\n");
                CONSOLE_OUT("Enter Security level required : ");
                CONSOLE_IN("%u", &read_val);
                auth.security = (UCHAR) ((read_val)?SMP_SEC_LEVEL_2: SMP_SEC_LEVEL_1);

                CONSOLE_OUT("\n0 - non-Bonding\n1 - Bonding\n");
                CONSOLE_OUT("Enter Bonding type : ");
                CONSOLE_IN("%u", &read_val);
                auth.bonding = (UCHAR) read_val;

                CONSOLE_OUT("Enter Authentication (0 - Accept/ErrorCode - Reject)  : ");
                CONSOLE_IN("%x", &read_val);
                auth.param = (UCHAR)read_val;

                CONSOLE_OUT("Enter Encryption Key size required : ");
                CONSOLE_IN("%u", &read_val);
                auth.ekey_size = (UCHAR)read_val;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                if (BT_TRUE == appl_smp_xtxp_enable)
                {
                    CONSOLE_OUT("Enter transport (1-BREDR, 2-BLE): ");
                    CONSOLE_IN("%u", &read_val);
                    auth.transport = (UCHAR)read_val;

                    CONSOLE_OUT("Enter Cross Transport KeyGen (0-No, 1-Yes): ");
                    CONSOLE_IN("%u", &read_val);
                    auth.xtx_info = (read_val) ? SMP_XTX_KEYGEN_MASK : SMP_XTX_DISABLE;
                    auth.xtx_info |= SMP_XTX_H7_MASK;

#ifdef SMP_ENABLE_BLURTOOTH_VU_UPDATE
                    CONSOLE_OUT("Enter role requirement of local device (0-Master, 1-Slave): ");
                    CONSOLE_IN("%u", &read_val);
                    auth.role = (UCHAR)read_val;
#endif /* SMP_ENABLE_BLURTOOTH_VU_UPDATE */
                }
                else
                {
                    auth.transport = SMP_LINK_LE;
                    auth.xtx_info = SMP_XTX_DISABLE;
                }

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE)BT_smp_get_link_handle(&bd_addr, &bd_handle, auth.transport);
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE)BT_smp_get_bd_handle(&bd_addr, &bd_handle);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

                retval = BT_smp_authentication_request_reply
                         (
                             &bd_handle,
                             &auth
                         );

                CONSOLE_OUT ("BT_smp_authentication_request_reply, Retval - 0x%04X\n", retval);
                break;

            case 5:
#ifdef SMP_HAVE_ENCRYPT_API
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("\n0 - Public\n1 - Random\n");
                CONSOLE_OUT("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                CONSOLE_OUT("Enter EDIV (UINT16 in HEX): ");
                CONSOLE_IN("%x", &read_val);
                ediv = read_val;

                CONSOLE_OUT("Enter RAND (8 octet in HEX): ");
                for (i = 0U; i < SMP_RAND_64B_SIZE; i++)
                {
                    CONSOLE_IN("%x", &read_val);
                    rnd[i] = read_val;
                }

                CONSOLE_OUT("Enter LTK (16 octet in HEX): ");
                for (i = 0U; i < SMP_LTK_SIZE; i++)
                {
                    CONSOLE_IN("%x", &read_val);
                    ltk[i] = read_val;
                }

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                /* Request to encrypt */
                retval = BT_smp_encrypt
                         (
                             &bd_handle,
                             rnd,
                             ediv,
                             ltk
                         );
                CONSOLE_OUT("BT_smp_encrypt retval - 0x%04X\n", retval);
#else /* SMP_HAVE_ENCRYPT_API */
                CONSOLE_OUT("SMP_HAVE_ENCRYPT_API not defined\n");
#endif /* SMP_HAVE_ENCRYPT_API */
                break;

            /* User Pass Key Request Reply */
            case 10:
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("Enter BD_ADDR type (0: Public, 1: Random): ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                CONSOLE_OUT("1 - Accept/<ERROR> - Reject)  : ");
                CONSOLE_IN("%u", &read_val);
                smp_accept = (UCHAR)read_val;

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE) BT_smp_get_bd_handle (&bd_addr, &bd_handle);

#ifdef SMP_LESC
                ntf_val = SMP_LESC_PASSKEY_ENTRY_STARTED;
                /* Send KeyPress Notification */
                (BT_IGNORE_RETURN_VALUE) BT_smp_keypress_notification(&bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */

                if (0U != smp_accept)
                {
                    CONSOLE_OUT("Enter Passkey : ");
                    CONSOLE_IN("%u", &read_passkey);
                }

#ifdef SMP_LESC
                ntf_val = SMP_LESC_PASSKEY_ENTRY_COMPLETED;
                /* Send KeyPress Notification */
                (BT_IGNORE_RETURN_VALUE) BT_smp_keypress_notification(&bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */

                retval = BT_smp_passkey_entry_request_reply
                         (
                             &bd_handle,
                             &read_passkey,
                             smp_accept
                         );
                break;

            /* Numeric Comparison Confirmation Reply */
            case 11:
#ifdef SMP_LESC
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("Enter BD_ADDR type (0: Public, 1: Random): ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                CONSOLE_OUT("1 - Accept/<ERROR> - Reject)  : ");
                CONSOLE_IN("%u", &read_val);
                smp_accept = (1U == read_val) ?
                         SMP_NUM_COMP_CNF_POSITIVE:
                         SMP_NUM_COMP_CNF_NEGATIVE;

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE) BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                retval = BT_smp_nkey_comp_cnf_reply
                         (
                             &bd_handle,
                             (void *)&smp_accept
                         );
#else /* SMP_LESC */
                CONSOLE_OUT("SMP_LESC Compilation Flag not defined!!!\n");
#endif /* SMP_LESC */
                break;

            /* Register callback */
            case 20:
                retval = BT_smp_register_user_interface(appl_smp_cb);
                break;

            case 21:
                CONSOLE_OUT("Enter mode (0: Non-automatic, 1: Automatic) : ");
                CONSOLE_IN("%d", &read_val);
                automatic_mode = (UCHAR)read_val;
                break;

            case 22:
                CONSOLE_OUT("Enter Pairable Mode (1/0): ");
                CONSOLE_IN("%d", &read_val);
                pairable_mode = (UCHAR)read_val;
                break;

            case 23:
                CONSOLE_OUT("Enter Numeric Key Comparison mode (0: Manual, 1: Automatic) : ");
                CONSOLE_IN("%d", &read_val);

                /* Set the NC Mode */
                nc_automatic_mode = (1U == (UCHAR)read_val) ? SMP_TRUE : SMP_FALSE;

                /* Check for NC and Auto Mode states */
                if ((SMP_TRUE == nc_automatic_mode) && (SMP_FALSE == automatic_mode))
                {
                    printf("Use Option 21 to set Automatic mode\n");
                }
                break;

            case 30:
                CONSOLE_OUT("Choose default IRK [1 - Yes, 0 - No]\n");
                CONSOLE_IN("%d", &choice);
                if (0U == (UCHAR)choice)
                {
                    CONSOLE_OUT("Enter IRK (16 octets): ");
                    for (i = 0U; i < SMP_IRK_SIZE; i++)
                    {
                        CONSOLE_IN("%x", &choice);
                        appl_user_irk[i] = (UCHAR)choice;
                    }
                    retval = BT_smp_create_resolvable_pvt_addr(appl_user_irk);
                }
                else
                {
                    (BT_IGNORE_RETURN_VALUE) BT_smp_get_key_exchange_info_pl(&key_info);
                    retval = BT_smp_create_resolvable_pvt_addr(key_info->id_info);
                }
                break;

            case 31:
                CONSOLE_OUT("Enter BD_ADDR: ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("Enter IRK (16 octets): ");
                for (i = 0U; i < SMP_IRK_SIZE; i++)
                {
                    CONSOLE_IN ("%x", &choice);
                    appl_user_irk[i] = (UCHAR)choice;
                }

                retval = BT_smp_verify_resolvable_pvt_addr(bd_addr.addr, appl_user_irk);
                break;

            case 40:
                CONSOLE_OUT("Enter BD_ADDR : ");
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                APPL_TRC("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR) read_val;

                /* Get the BD handle */
                (BT_IGNORE_RETURN_VALUE) BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                /* Initialize */
                BT_mem_set(&auth, 0x00, sizeof(SMP_AUTH_INFO));

                retval = BT_smp_get_device_security_info
                         (
                             &bd_handle,
                             &auth
                         );
                if (API_SUCCESS == retval)
                {
                    APPL_TRC ("Security - 0x%02X\n", auth.security);
                    APPL_TRC ("Bonding - 0x%02X\n", auth.bonding);
                    APPL_TRC ("Encryption Key Size  - 0x%02X\n", auth.ekey_size);
                }
                break;

            case 50:
                smp_pl_operations ();
                break;

            case 60:
#ifdef SMP_DATA_SIGNING
                {
                    UCHAR * data;
                    UINT16 datalen;

                    CONSOLE_OUT ("Enter data length: ");
                    CONSOLE_IN("%d", &choice);
                    datalen = (UINT16) choice;
                    data = (UCHAR *) BT_alloc_mem (datalen);

                    if (NULL != data)
                    {
                        CONSOLE_OUT ("Enter the data (In Hex): ");
                        for (i = 0U; i < datalen; i++)
                        {
                            CONSOLE_IN("%x", &choice);
                            data[i] = (UCHAR)choice;
                        }

                        CONSOLE_OUT ("Signing Counter: %d", sign_create_counter);

                        /* Call to sign the data */
                        appl_smp_generate_sign_data(data, datalen);

                        BT_free_mem (data);
                    }
            }

#else /* SMP_DATA_SIGNING */
                CONSOLE_OUT ("SMP_DATA_SIGNING not defined\n");
#endif /* SMP_DATA_SIGNING */
                break;

            case 61:
#ifdef SMP_DATA_SIGNING
                {
                    UCHAR * data;
                    UINT16 datalen;

                    CONSOLE_OUT ("Enter signed data length: ");
                    CONSOLE_IN("%d", &choice);
                    datalen = (UINT16) choice;
                    data = (UCHAR *) BT_alloc_mem (datalen);

                    if (NULL != data)
                    {
                        CONSOLE_OUT ("Enter the data (In Hex): ");
                        for (i = 0U; i < datalen; i++)
                        {
                            CONSOLE_IN("%x", &choice);
                            data[i] = (UCHAR)choice;
                        }

                        CONSOLE_OUT ("Signing Counter: %d", sign_verify_counter);

                        /* Call to sign the data */
                        appl_smp_verify_sign_data(data, datalen);

                        BT_free_mem (data);
                    }
            }

#else /* SMP_DATA_SIGNING */
                APPL_TRC ("SMP_DATA_SIGNING not defined\n");
#endif /* SMP_DATA_SIGNING */
                break;

            case 70:
#ifdef AES_CMAC_UTIL
                 /* aes_cmac_test(); */
#else /* AES_CMAC_UTIL */
                CONSOLE_OUT ("AES_CMAC_UTIL not defined\n");
#endif /* AES_CMAC_UTIL */
                break;

            case 71:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_f4();
#else
                CONSOLE_OUT ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 72:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_f5();
#else
                CONSOLE_OUT ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 73:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_f6();
#else
                CONSOLE_OUT ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 74:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_g2();
#else
                CONSOLE_OUT ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 75:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_h6();
#else
                CONSOLE_OUT ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 100:
                CONSOLE_OUT("Enable cross transport inputs (1/0): ");
                CONSOLE_IN("%d", &choice);
                appl_smp_xtxp_enable = (UCHAR)choice;

                BT_IGNORE_UNUSED_PARAM(appl_smp_xtxp_enable);
                break;

            case 101:
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                {
                    /* Get the BD handle */
                    retval = BT_smp_get_link_handle(&g_bd_addr, &bd_handle, SMP_LINK_BREDR);

                    if (API_SUCCESS == retval)
                    {
                        auth.security = 0x00U;
                        auth.bonding = 0x00U;
                        auth.pair_mode = 0x00U;
                        auth.transport = SMP_LINK_BREDR;
                        auth.ekey_size = SMP_MAX_ENCRYPTION_KEY_SIZE;
#ifdef SMP_ENABLE_BLURTOOTH_VU_UPDATE
                        auth.role = BT_DEVICE_ROLE_MASTER;
#endif /* SMP_ENABLE_BLURTOOTH_VU_UPDATE */
                        auth.xtx_info = SMP_XTX_KEYGEN_MASK | SMP_XTX_H7_MASK;

                        retval = BT_smp_authenticate
                        (
                            &bd_handle,
                            &auth
                        );
                        CONSOLE_OUT("BT_smp_authenticate, Retval - 0x%04X\n", retval);
                    }
                }
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
                break;

            case 199:
                CONSOLE_OUT("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                CONSOLE_OUT("Enter BD_ADDR type (0: Public, 1: Random): ");
                CONSOLE_IN("%u", &read_val);
                bd_addr.type = (UCHAR)read_val;

                appl_hci_le_connect(&bd_addr, 0x00U);
                break;

            case 200:
                {
                    UINT16         conn_handle;
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                    CONSOLE_OUT("Enter BD_ADDR : ");
                    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

                    CONSOLE_OUT("Enter BD_ADDR type (0: Public, 1: Random): ");
                    CONSOLE_IN("%u", &read_val);
                    bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                    BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                    retval = BT_hci_get_le_connection_handle
                             (
                                 &bd_addr,
                                 &conn_handle
                             );

                    if ((API_SUCCESS == retval) &&
                        (HCI_INVALID_CONNECTION_HANDLE != conn_handle))
                    {
                        /* Initiate BLE Disconnection - Step 2(f) */
                        retval = BT_hci_disconnect(conn_handle, 0x13U);

                        if (API_SUCCESS != retval)
                        {
                            APPL_ERR(
                            "Failed to Disconnect connection with %02X:%02X:%02X:%02X::%02X"
                            ":%02X, reason 0x%04X\n", bd_addr.addr[5U],
                            bd_addr.addr[4U], bd_addr.addr[3U], bd_addr.addr[2U],
                            bd_addr.addr[1U], bd_addr.addr[0U], retval);
                        }
                    }
                }
                break;

            default:
                CONSOLE_OUT("Invalid Choice try again..\n");
                break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }

        if(API_SUCCESS != retval)
        {
            APPL_TRC("FAILED !!! retval = 0x%04X\n", retval);
        }
        else
        {
            APPL_TRC("Success !!!\n");
        }
    }
}

#ifdef SMP_DATA_SIGNING
void appl_smp_generate_sign_data (UCHAR * data, UINT16 datalen)
{
    SMP_KEY_DIST * key_info;

    /* Allocate signing data buffer */
    sign_datalen = datalen + sizeof (sign_create_counter);
    sign_data = BT_alloc_mem (sign_datalen);

    if (NULL != sign_data)
    {
        /* Copy the application data */
        BT_mem_copy (sign_data, data, datalen);

        /* Update the sign counter */
        BT_PACK_LE_4_BYTE(&sign_data[datalen], &sign_create_counter);
        BT_PACK_LE_4_BYTE(signature, &sign_create_counter);

        /* Get the platform signing key */
        (BT_IGNORE_RETURN_VALUE) BT_smp_get_key_exchange_info_pl (&key_info);
        sign_key = key_info->sign_info;

        /* Call to sign the data */
        (BT_IGNORE_RETURN_VALUE) BT_smp_sign_data(sign_data, sign_datalen, sign_key, sign_mac);
    }
}


void appl_smp_verify_sign_data (UCHAR * data, UINT16 datalen)
{
    /* Allocate signing data buffer */
    sign_datalen = datalen - SMP_SIGN_MAC_SIZE;
    sign_data = BT_alloc_mem (sign_datalen);

    if (NULL != sign_data)
    {
        /* Copy the application data */
        BT_mem_copy (sign_data, data, sign_datalen);

        /* Update the MAC with received MAC */
        BT_mem_copy (sign_mac, &data[sign_datalen], SMP_SIGN_MAC_SIZE);

        /* Get the peer device signing key */
        sign_key = peer_key_info.sign_info;

        /* Call to verify the sign data */
        (BT_IGNORE_RETURN_VALUE) BT_smp_verify_sign_data (sign_data, sign_datalen, sign_key, sign_mac);
    }
}
#endif /* SMP_DATA_SIGNING */

void appl_smp_reset (void)
{
#ifdef SMP_DATA_SIGNING
    sign_create_counter = 0U;
    sign_verify_counter = 0U;
#endif /* SMP_DATA_SIGNING */
}

void appl_smp_register (void)
{
    (BT_IGNORE_RETURN_VALUE) BT_smp_register_user_interface (appl_smp_cb);
}

void appl_smp_configure_platform (void)
{
    SMP_BD_ADDR l_addr;

    /* Update local SMP capabilities using smp_pl APIs if required here */
    (BT_IGNORE_RETURN_VALUE) BT_smp_set_io_cap_pl(SMP_IO_CAPABILITY_DISPLAY_YESNO);

    /* Setting the local Public Address as Identity Address */
    (BT_IGNORE_RETURN_VALUE) BT_hci_get_local_bd_addr
    (
        l_addr.addr
    );
    l_addr.type = BT_BD_PUBLIC_ADDRESS_TYPE;

    (BT_IGNORE_RETURN_VALUE) BT_smp_set_local_identity_addr(&l_addr);
}

API_RESULT appl_smp_security_request
           (
               SMP_BD_HANDLE * handle,
               SMP_AUTH_INFO * auth
           )
{
    /*
     * TODO:
     *
     * If bonding is remembered in application and BT_smp_encrypt is exposed
     * in SMP, then have check here whether to call BT_smp_authenticate or
     * BT_smp_encrypt by getting db data from the handle reference
     */

#ifndef SMP_STORAGE
     /* If storage not supported, make no-bonding */
     auth->bonding = SMP_BONDING_NONE;
#endif /* SMP_STORAGE */

    /* Request for authentication to SMP */
    return BT_smp_authenticate (handle, auth);
}

API_RESULT appl_smp_initiate_pairing (void)
{
    unsigned int  read_val;
    SMP_BD_ADDR   bd_addr;
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth;
    API_RESULT    retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&auth, 0, sizeof(SMP_AUTH_INFO));

#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
    CONSOLE_OUT("Enter BD_ADDR : ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr.addr);

    CONSOLE_OUT("\n0 - Public\n1 - Random\n");
    CONSOLE_OUT("Enter BD_ADDR type : ");
    CONSOLE_IN("%u", &read_val);
    bd_addr.type = (UCHAR) read_val;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
    BT_COPY_BD_ADDR_AND_TYPE(&bd_addr, &g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

#ifdef SMP_LESC
    CONSOLE_OUT("0 - Legacy LE SMP Procedure\n");
    CONSOLE_OUT("1 - LE Secure Connections Mode\n");
    CONSOLE_OUT("Enter SMP Pairing Mode :");
    CONSOLE_IN("%u", &read_val);

    /* Call SMP LE Secure Connetions Mode Setting API here */
    auth.pair_mode = (UCHAR)((read_val)? SMP_LESC_MODE : SMP_LEGACY_MODE);
#endif /* SMP_LESC */

    CONSOLE_OUT(" 0 - Encryption Only/Unauthenticated (Without MITM)\n");
    CONSOLE_OUT(" 1 - Authenticated (With MITM)\n");
    CONSOLE_OUT("Enter Security level required : ");
    CONSOLE_IN("%u", &read_val);
    auth.security = (UCHAR) ((read_val)? SMP_SEC_LEVEL_2 : SMP_SEC_LEVEL_1);

    CONSOLE_OUT("\n0 - non-Bonding\n1 - Bonding\n");
    CONSOLE_OUT("Enter Bonding type : ");
    CONSOLE_IN("%u", &read_val);
    auth.bonding = (UCHAR) read_val;

#ifndef SMP_STORAGE
     /* If storage not supported, make no-bonding */
     auth->bonding = SMP_BONDING_NONE;
#endif /* SMP_STORAGE */

    CONSOLE_OUT("Enter Encryption Key size required : ");
    CONSOLE_IN("%u", &read_val);
    auth.ekey_size = (UCHAR)read_val;

    /* Get the BD handle */
    (BT_IGNORE_RETURN_VALUE) BT_smp_get_bd_handle (&bd_addr, &bd_handle);

    retval = BT_smp_authenticate
             (
                 &bd_handle,
                 &auth
             );

    CONSOLE_OUT ("BT_smp_authenticate, Retval - 0x%04X\n", retval);

    return retval;
}

API_RESULT appl_smp_cb
           (
               /* IN */ SMP_BD_HANDLE   * bd_handle,
               /* IN */ UCHAR      event,
               /* IN */ API_RESULT status,
               /* IN */ void     * eventdata,
               /* IN */ UINT16     data_len
           )
{
    API_RESULT retval;

    UINT16   ediv;
    UCHAR  * peer_rand;
    UCHAR    ltk[SMP_LTK_SIZE];
    UCHAR    ltk_null;

    SMP_KEY_DIST * key_info;
    SMP_AUTH_INFO * auth;
    SMP_AUTH_INFO info;
    SMP_BD_ADDR bdaddr;
    SMP_KEY_XCHG_PARAM * kx_param;
    APPL_HANDLE   appl_handle;

    UCHAR smp_accept;

#ifdef SMP_LESC
    UCHAR ntf_val;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
    UCHAR link_key[BT_LINK_KEY_SIZE];
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

    APPL_EVENT_PARAM    fsm_param;

#ifndef DONT_USE_STANDARD_IO
    UCHAR * bd_addr;
    UCHAR   bd_addr_type;
#endif /* DONT_USE_STANDARD_IO */

    UCHAR * event_data = (UCHAR *)eventdata;

    SMP_KEY_DIST  p_key_info;
    UCHAR         p_keys;
    UCHAR         req_pair_mode;
    UCHAR         link_type, role;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    appl_handle = 0U;
    link_type = DQ_LINK_ANY;

    /* Get the BD Address from handle */
    BT_INIT_BD_ADDR(&bdaddr);
    (BT_IGNORE_RETURN_VALUE)BT_smp_get_bd_addr (bd_handle, &bdaddr);

    /* Get the Link Type */
    device_queue_get_link_type(&link_type, bd_handle);

    /* Set device role from HCI */
    BT_hci_get_device_role(&bdaddr, link_type, &role);

#ifndef DONT_USE_STANDARD_IO
    bd_addr = bdaddr.addr;
    bd_addr_type = bdaddr.type;
#endif /* DONT_USE_STANDARD_IO */
    switch(event)
    {
    case SMP_AUTHENTICATION_ERROR:
        APPL_TRC("\nRecvd SMP_AUTHENTICATION_ERROR\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");
        APPL_TRC("Status : %04X\n", status);
        break;

    case SMP_AUTHENTICATION_COMPLETE:
        APPL_TRC("\nRecvd SMP_AUTHENTICATION_COMPLETE\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");
        APPL_TRC("Status : %04X\n", status);

        (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle (*bd_handle,&appl_handle);

#ifdef APPL_FSM_SUPPORT
        fsm_param.handle = appl_handle;
        fsm_param.data_param = NULL;
        fsm_param.data_len = 0U;
        fsm_param.direction = 0xFFU;
#endif /* APPL_FSM_SUPPORT */

        /* The direction member of FSM Event holds the status of Authentication Procedure here */
        if (NULL != event_data)
        {
            if(API_SUCCESS == status)
            {
                auth = (SMP_AUTH_INFO *)event_data;

                APPL_TRC("Authentication type : %s\n",
                (SMP_SEC_LEVEL_2 == (auth->security & 0x0FU))?  "With MITM":
                "Encryption Only (without MITM)");

#ifdef SMP_LESC
                APPL_TRC("Pairing Mode : %s\n",
                (SMP_LESC_MODE == (auth->pair_mode))? "LE SEC Pairing Mode":
                "LEGACY Pairing Mode");
#endif /* SMP_LESC */

                APPL_TRC("Bonding type : %s\n",
                (auth->bonding)? "Bonding": "Non-Bonding");

                APPL_TRC("Encryption Key size : %d\n", auth->ekey_size);

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                APPL_TRC("Transport : %d\n", auth->transport);
                APPL_TRC("Cross Transport info: %d\n", auth->xtx_info);

                if (0U != (SMP_XTX_KEYGEN_MASK & auth->xtx_info))
                {
                    /* Save the BD Address */
                    BT_COPY_BD_ADDR_AND_TYPE(&appl_smp_bd_addr, &bdaddr);

#ifdef CLASSIC_SEC_MANAGER
                    /* Compare key strengths before generating */
                    if (SMP_LINK_BREDR == auth->transport)
                    {
                        SM_DEVICE_STATE state;

                        retval = BT_sm_get_device_security_state(bd_addr, &state);
                        if ((API_SUCCESS != retval) || (16U != state.ekey_size))
                        {
                            APPL_ERR("EncKey Size check failed for LTK generation.\n");
                            break;
                        }

                        /* Get the BREDR link key for the device */
                        retval = BT_sm_get_device_link_key(bd_addr, link_key);
                        if (API_SUCCESS != retval)
                        {
                            printf("FAILED ! Reason = 0x%04X\n", retval);
                            break;
                        }
                        else
                        {
                            (BT_IGNORE_RETURN_VALUE)BT_smp_get_ltk_from_lk_pl
                            (
                                link_key,
                                appl_smp_lesc_xtxp_ltk_complete,
                                (auth->xtx_info & SMP_XTX_H7_MASK)
                            );
                        }
                    }
                    else
#endif /* CLASSIC_SEC_MANAGER */
                    {
                        retval = BT_smp_get_device_keys
                                 (
                                     bd_handle,
                                     &p_keys,
                                     &p_key_info
                                 );

                        if (API_SUCCESS != retval)
                        {
                            APPL_ERR("Failed to get Peer Device Keys!!\n");
                        }
                        else
                        {
                            if (16U != auth->ekey_size)
                            {
#ifdef APPL_SMP_VALIDATE_KEYSIZE_FOR_CTKD
                                APPL_ERR("EncKey Size check failed for LinkKey generation.\n");
                                break;
#else /* APPL_SMP_VALIDATE_KEYSIZE_FOR_CTKD */
                                BT_smp_get_raw_lesc_ltk(bd_handle, p_key_info.enc_info);
#endif /* APPL_SMP_VALIDATE_KEYSIZE_FOR_CTKD */
                            }

                            /* Save the Identity BD Address if valid */
                            if (SMP_DIST_MASK_ID_KEY & p_keys)
                            {
                                BT_COPY_BD_ADDR(appl_smp_bd_addr.addr, &p_key_info.id_addr_info[1]);
                                appl_smp_bd_addr.type = p_key_info.id_addr_info[0];
                            }

                            (BT_IGNORE_RETURN_VALUE) BT_smp_get_lk_from_ltk_pl
                            (
                                p_key_info.enc_info,
                                appl_smp_lesc_xtxp_lk_complete,
                                (auth->xtx_info & SMP_XTX_H7_MASK)
                            );
                        }
                    }
                }
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

#ifdef HAVE_SMP_REPEAT_TRACKING
                /* Decrease timeout value by the fraction */
                if (SMP_REPEAT_ATTEMPT_TIMEOUT_MIN < smp_repeat_attempt_timeout)
                {
                    smp_repeat_attempt_timeout >>= SMP_REPEAT_ATTEMPT_TIMEOUT_FRACTION;
                }
#endif /* HAVE_SMP_REPEAT_TRACKING */

                /**
                 * If the Bonding Feature is Set for this Pairing, then Automatically
                 * 1. Added the device to Resolving.
                 * 2. Set the privacy Mode to Device Privacy Mode.
                 */
#ifdef APPL_SMP_AUTO_ADD_TO_RSLV_LIST
                if (0U != auth->bonding)
                {
                    SMP_BD_ADDR t_bd_addr;

                    /* check if the device has shared the keys */
                    /* Get platform data of key informations */
                    BT_smp_get_key_exchange_info_pl (&key_info);

                    BT_COPY_BD_ADDR(&t_bd_addr.addr, &peer_key_info.id_addr_info[1U]);
                    BT_COPY_TYPE(t_bd_addr.type, peer_key_info.id_addr_info[0U]);

                    retval = BT_hci_le_add_device_to_resolving_list
                             (
                                 t_bd_addr.type,
                                 &t_bd_addr.addr[0U],
                                 peer_key_info.id_info,
                                 key_info->id_info
                             );

                    if(API_SUCCESS != retval)
                    {
                        APPL_TRC("FAILED !!! Error code = %04X\n", retval);
                    }
                    else
                    {
                        APPL_TRC("Added Device with Identity Address: "
                        BT_DEVICE_ADDR_FRMT_SPECIFIER " to Resolving List!\n",
                        BT_DEVICE_ADDR_PRINT_STR(&t_bd_addr));

                        retval = BT_hci_le_set_privacy_mode
                                 (
                                     t_bd_addr.type,
                                     t_bd_addr.addr,
                                     0x01U /* Device Privacy Mode */
                                 );

                        APPL_TRC("Setting Device Privacy Mode returned with "
                        "0x%04X\n", retval);
                    }
                }
#endif /* APPL_SMP_AUTO_ADD_TO_RSLV_LIST */

#ifdef APPL_FSM_SUPPORT
                fsm_param.data_param = (UCHAR *)auth;
                fsm_param.data_len = sizeof (SMP_AUTH_INFO);

                /* Update status in direction parameter */
                fsm_param.direction = 0x00U;
#endif /* APPL_FSM_SUPPORT */
            }
#ifdef HAVE_SMP_REPEAT_TRACKING
            else
            {
                if (BT_TIMER_HANDLE_INIT_VAL == smp_repeat_timer)
                {
                    /* Start timer to track repeat attempt */
                    BT_start_timer
                    (
                        &smp_repeat_timer,
                        smp_repeat_attempt_timeout,
                        smp_repeat_timeout_handler,
                        NULL, 0U
                    );

                    APPL_TRC ("Repeat Tracking Timer Started for %d seconds.\n",
                        smp_repeat_attempt_timeout);

                    /* Increase timeout value by the fraction */
                    if (SMP_REPEAT_ATTEMPT_TIMEOUT_MAX > smp_repeat_attempt_timeout)
                    {
                        smp_repeat_attempt_timeout <<= SMP_REPEAT_ATTEMPT_TIMEOUT_FRACTION;
                    }
                }
            }
#endif /* HAVE_SMP_REPEAT_TRACKING */
        }
        else
        {
            if (API_SUCCESS == status)
            {
                APPL_TRC("\nConfirmed Authentication using Encryption\n");

#ifdef APPL_FSM_SUPPORT
                /* Update status in direction parameter */
                fsm_param.direction = 0x00U;
#endif /* APPL_FSM_SUPPORT */
            }
            else if (SMP_REMOTE_SIDE_PIN_KEY_MISSING == status)
            {
                APPL_TRC("\nPeer Device Lost previous Bonding Information!\n");
                APPL_TRC("Deleting Local Bond Information of Peer...\n");

                retval = BT_smp_mark_device_untrusted_pl(bd_handle);

                APPL_TRC("Marked Device Untrusted with result 0x%04X\n",retval);

                if (API_SUCCESS == retval)
                {
                    APPL_TRC("Initiate Pairing Again...\n");
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
        }

        /*
         * Post service connection only if Authentication or Encryption
         * successfuly for BLE connection
         */
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
        if ((API_SUCCESS == status) &&
            ((NULL == event_data) ||
             (SMP_LINK_BREDR != auth->transport)))
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
         if ((API_SUCCESS == status) &&
             (NULL == event_data))
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
        {
#ifdef APPL_FSM_SUPPORT
            /* Post Transport Configuration Ind from SMP */
            appl_fsm_post_event
            (
                ev_appl_transport_configuration_ind,
                (void *)(&fsm_param)
            );
#else
            fsm_param.handle = appl_handle;
            appl_service_connect(&fsm_param.handle);
#endif /* APPL_FSM_SUPPORT */
        }
        break;

    case SMP_AUTHENTICATION_RESPONSE:
        APPL_TRC("\nRecvd SMP_AUTHENTICATION_RESPONSE\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        auth = (SMP_AUTH_INFO *)event_data;

        APPL_TRC("Authentication type : %s\n",
        (SMP_SEC_LEVEL_2 == (auth->security & 0x0FU))?  "With MITM":
        "Encryption Only (without MITM)");

        APPL_TRC("Bonding type : %s\n",
        (auth->bonding)? "Bonding": "Non-Bonding");

        APPL_TRC("Encryption Key size : %d\n", auth->ekey_size);

#ifdef SMP_LESC
        APPL_TRC("Pairing Mode : %s\n",
        (SMP_LESC_MODE == (auth->pair_mode))? "LE SEC Pairing Mode":
        "LEGACY Pairing Mode");

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
        APPL_TRC("Transport : 0x%02X\n", auth->transport);
        APPL_TRC("Cross Transport Info : 0x%02X\n", auth->xtx_info);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */
        break;

    case SMP_AUTHENTICATION_REQUEST:
        APPL_TRC("\nRecvd SMP_AUTHENTICATION_REQUEST\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        auth = (SMP_AUTH_INFO *)event_data;

        APPL_TRC("Authentication type : %s\n",
        (SMP_SEC_LEVEL_2 == (auth->security & 0x0FU))?  "With MITM":
        "Encryption Only (without MITM)");

        APPL_TRC("Bonding type : %s\n",
        (auth->bonding)? "Bonding": "Non-Bonding");

        APPL_TRC("Encryption Key size : %d\n", auth->ekey_size);

#ifdef SMP_LESC
        APPL_TRC("Pairing Mode : %s\n",
        (SMP_LESC_MODE == (auth->pair_mode))? "LE SEC Pairing Mode":
        "LEGACY Pairing Mode");

        /* Copy the requested pair mode */
        req_pair_mode = auth->pair_mode;

        /**
         * Choose the Pairing Mode based on what is configured by the
         * application.
         */
        (auth->pair_mode) = g_appl_pair_mode;
#endif /* SMP_LESC */

        /* Check if automatic mode is set */
        if (SMP_TRUE != automatic_mode)
        {
            APPL_TRC("Automatic Mode is False\n");
            break;
        }

        /* Accept Authentication */
        auth->param = SMP_ERROR_NONE;

#ifdef SMP_LESC
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
        APPL_TRC("Transport : 0x%02X\n", auth->transport);
        APPL_TRC("Cross Transport Info : 0x%02X\n", auth->xtx_info);

        if (BT_FALSE == g_appl_xtx_mode)
        {
            auth->xtx_info = SMP_XTX_DISABLE;
        }
#ifdef SMP_SAVE_REMOTE_IOCAP
        else
        {
            SMP_IOCAPS le_iocaps;
            SM_IO_CAPS br_iocaps;

            /* Get the remote IO Capabilities over the BLE */
            retval = BT_smp_get_remote_iocaps_pl(&bdaddr, &le_iocaps);

            if (API_SUCCESS == retval)
            {
                /* Get the remote IO Capabilities over the BREDR */
                retval = BT_sm_get_remote_iocaps_pl(bdaddr.addr, &br_iocaps);
                if (API_SUCCESS == retval)
                {
                    if (le_iocaps.iocap != br_iocaps.io_cap)
                    {
                        auth->xtx_info = SMP_XTX_DISABLE;
                    }
                }
            }
        }
#endif /* SMP_SAVE_REMOTE_IOCAP */

        if (SMP_XTX_DISABLE != auth->xtx_info)
        {
            /* Is transport on BR/EDR? */
            if (SMP_LINK_BREDR == auth->transport)
            {
                UCHAR lk[BT_LINK_KEY_SIZE];
                UCHAR lk_type;

                /* Check if P256 authentication on BR/EDR link */
                retval = BT_sm_get_device_link_key_and_type(bd_addr, lk, &lk_type);

                if ((API_SUCCESS != retval) ||
                    ((HCI_LINK_KEY_UNAUTHENTICATED_P_256 != lk_type) &&
                    (HCI_LINK_KEY_AUTHENTICATED_P_256 != lk_type)))
                {
                    auth->xtx_info = SMP_XTX_DISABLE;
                }
            }
            else
            {
                /* Is LE Secure connection requested on BLE link */
                if (SMP_LESC_MODE != auth->pair_mode)
                {
                    auth->xtx_info = SMP_XTX_DISABLE;
                }
            }
        }

        /* Enable H7 support if Cross transport keygen is requested */
        if (0U != (auth->xtx_info & SMP_XTX_KEYGEN_MASK))
        {
            auth->xtx_info |= SMP_XTX_H7_MASK;
        }

        if (SMP_LINK_BREDR == auth->transport)
        {
            if (SMP_XTX_DISABLE == auth->xtx_info)
            {
                /* auth->param = SMP_ERROR_CT_KEY_GEN_NOT_ALLOWED; */
            }
        }
        else
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */
        {
            /* Get the application handle reference */
            (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle (*bd_handle,&appl_handle);

            /* Initialize */
            BT_mem_set(&info, 0x00, sizeof(SMP_AUTH_INFO));

            /* Get current security state for the link */
            retval = BT_smp_get_device_security_info (bd_handle, &info);
            if (API_SUCCESS == retval)
            {
                /* Check if the link is authenticated */
                if (info.param != SMP_ENTITY_AUTH_ON)
                {
                    /**
                     *  Here, the application logic selected is to delete
                     *  bond of an exisitng peer device from the bonded
                     *  device list on receiving an Authentication Request
                     *  from it again on an UnAuthenticated link.
                     *  i.e. If a device initiates Pairing again after a
                     *  disconnection, even though it was previously bonded.
                     *  This could happen, if the peer device has lost the
                     *  Bonding informations inbetween connections.
                     *  Typically, popular smart phones/commercial devices and application
                     *  guidelines will not delete the bonding information here
                     *  without the approval/intervention from the user and
                     *  send out a "PAIRING FAILED" error with error code
                     *   - "SMP_ERROR_UNSPECIFIED_REASON", or
                     *   - "SMP_ERROR_PAIRING_NOT_SUPPORTED".
                     *  Also, such implementations would have some UI control to delete the
                     *  bonding on local side (eg: Button press combinations
                     *  etc.).
                     *  The following application logic is added, in this example
                     *  application which can also run on embedded or constrained system
                     *  without scope of User Intervention.
                     *
                     *  Application writer, needs to choose a better
                     *  alternative approach instead of deleteing peer bond information
                     *  automatically.
                     *
                     *  NOTE-1: This application logic/rational choosen for ease
                     *  of use, has security holes and power inefficiency as
                     *  pairing is allowed to happen multiple times with the
                     *  same device.
                     *
                     *  NOTE-2: Inorder to send pairing failure in case of
                     *  pairing request from an existing bonded device,
                     *  the application can set,
                     *  auth->param = SMP_ERROR_UNSPECIFIED_REASON or
                     *  auth->param = SMP_ERROR_PAIRING_NOT_SUPPORTED
                     *
                     *  while calling BT_smp_authentication_request_reply(...) API
                     *  is called.
                     */
                    /* Check if bonded */
                    if ((SMP_BONDING == info.bonding) && (BT_DEVICE_ROLE_SLAVE == role))
                    {
                        /**
                         * The application logic choosen here is to delete the
                         * bonding information of the peer device.
                         * This needs to be modified according to the
                         * suitable requirements and platform capabilities
                         * according to the Application writer as explained in
                         * the above comment.
                         */
                        retval = BT_smp_mark_device_untrusted_pl (bd_handle);

                        if (API_SUCCESS == retval)
                        {
                            /**
                             * On Successfull deletion of bond of a given peer
                             * device. We are clearning the data maintained by
                             * GATT with respect to that peer.
                             * This will reset all the CCCD etc. which are
                             * configured by that peer.
                             * i.e. all the "Indications" or "Notifications"
                             * enabled will be "Turned-OFF" at this location.
                             *
                             * As mentioned above, this section also needs to
                             * be modified according to the requirements and
                             * platform capabilities by the Application writer.
                             */
                            (BT_IGNORE_RETURN_VALUE) BT_gatt_db_peer_session_shutdown_handler(&(APPL_GET_ATT_INSTANCE(appl_handle)));
                        }
                    }
                }
            }

#ifndef SMP_STORAGE
            /* If storage not supported, make no-bonding */
            auth->bonding = SMP_BONDING_NONE;
#endif /* SMP_STORAGE */

#ifdef HAVE_SMP_REPEAT_TRACKING
            /* Check if repeat attempt tracking is on */
            if (BT_TIMER_HANDLE_INIT_VAL != smp_repeat_timer)
            {
                auth->param = SMP_ERROR_REPEATED_ATTEMTS;
            } else
#endif /* HAVE_SMP_REPEAT_TRACKING */
            /* Check if in pairable mode */
            if (SMP_TRUE != pairable_mode)
            {
                auth->param = SMP_ERROR_PAIRING_NOT_SUPPORTED;
            }

#ifdef SMP_LESC
            /* Check if Application has set Secure Connections only mode */
            if (SMP_PL_LESC_STRICT == BT_smp_get_lesc_policy_pl())
            {
                /* If requested pair mode is not LESC then send failure */
                if (SMP_LESC_MODE != req_pair_mode)
                {
                    /* Set error if incoming request is not LESC */
                    auth->param = SMP_ERROR_AUTHENTICATION_REQUIREMENTS;
                }
                else
                {
                    /**
                     * If local role is Master, then this would be Slave Security Request
                     * and ekey_size in that case would be set to 0x07.
                     */
                    if ((SMP_MAX_ENCRYPTION_KEY_SIZE != auth->ekey_size) && (BT_DEVICE_ROLE_SLAVE == role))
                    {
                        /* Set error if incoming request is not with MAX key size */
                        auth->param = SMP_ERROR_ENCRYPTION_KEY_SIZE;
                    }
                }
            }
#endif /* SMP_LESC */

#ifdef BT_SECURITY_VU_VALIDATION
            if (BT_SECURITY_VU_UNEXPECTED_PUBKEY == BT_security_vu_get())
            {
                /* Update to legcy mode to check vulnerability */
                (auth->pair_mode) = SMP_LEGACY_MODE;
            }
#endif /* BT_SECURITY_VU_VALIDATION */
        }

        APPL_TRC("\n\nSending +ve Authentication request reply.\n");
        retval = BT_smp_authentication_request_reply
                 (
                     bd_handle,
                     auth
                 );

        APPL_TRC("BT_smp_authentication_request_reply, Retval - 0x%04X\n", retval);

#ifdef APPL_FSM_SUPPORT
        /* Notify application state machine */
        fsm_param.handle = appl_handle;
        fsm_param.data_param = NULL;
        fsm_param.data_len = 0U;
        fsm_param.direction = 0x01U;

        /* Post Transport Configuration Request for SMP */
        appl_fsm_post_event
        (
            ev_appl_transport_configuration_req,
            (void *)(&fsm_param)
        );
#endif /* APPL_FSM_SUPPORT */
        break;

    case SMP_PASSKEY_ENTRY_REQUEST:
        APPL_TRC("\nEvent   : SMP_PASSKEY_ENTRY_REQUEST\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        if (BT_TRUE == automatic_mode)
        {
            /* Send the Passkey Entry Request Reply Automatically */
#ifdef SMP_LESC
            ntf_val = SMP_LESC_PASSKEY_ENTRY_STARTED;
            /* Send KeyPress Notification */
            (BT_IGNORE_RETURN_VALUE) BT_smp_keypress_notification(bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */

#ifdef SMP_LESC
            ntf_val = SMP_LESC_PASSKEY_ENTRY_COMPLETED;
            /* Send KeyPress Notification */
            (BT_IGNORE_RETURN_VALUE) BT_smp_keypress_notification(bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */
        }
        break;

    case SMP_PASSKEY_DISPLAY_REQUEST:
        APPL_TRC("\nEvent   : SMP_PASSKEY_DISPLAY_REQUEST\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        APPL_TRC("Passkey : %06u\n", *((UINT32 *)event_data));

#ifdef APPL_SMP_UPDATE_PASSKEY
        /* Update a 6 digit passkey - Max 999999 */
        *((UINT32 *)event_data) = APPL_SMP_PASSKEY;

        APPL_TRC("Updated Passkey: %06u", *((UINT32 *)event_data));

        retval = SMP_DISPLAY_PASSKEY_UPDATED;
#endif /* APPL_SMP_UPDATE_PASSKEY */

        break;

    case SMP_KEY_EXCHANGE_INFO_REQUEST:
        APPL_TRC("\nEvent   : SMP_KEY_EXCHANGE_INFO_REQUEST\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        /* Reference the event data */
        kx_param = (SMP_KEY_XCHG_PARAM *) event_data;

        APPL_TRC("Local keys negotiated - 0x%02X\n", kx_param->keys);
        APPL_TRC("Encryption Key Size negotiated - 0x%02X\n",
                kx_param->ekey_size);

        /* Save the local key distribution information */
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
        local_keys = kx_param->keys;
#endif

        /* Check if automatic mode is set */
        if(SMP_TRUE == automatic_mode)
        {
            /* Get platform data of key informations */
            (BT_IGNORE_RETURN_VALUE) BT_smp_get_key_exchange_info_pl (&key_info);

            /* Copy the Local Key Info into a local struct */
            BT_mem_copy(&local_key_info,key_info,sizeof(local_key_info));

            /* Mask the to be exchanged LTK according to the negotiated key size */
            BT_mem_set
            (
                (&local_key_info.enc_info[0U] + kx_param->ekey_size),
                0x00,
                (SMP_LTK_SIZE - kx_param->ekey_size)
            );

            (BT_IGNORE_RETURN_VALUE) BT_smp_key_exchange_info_request_reply (bd_handle, &local_key_info);
        }
        break;

    case SMP_LONG_TERM_KEY_REQUEST:

        /* Copy parameters to local variables */
        smp_unpack_2_byte_param(&ediv, &event_data[8U]);
        peer_rand = event_data;

        APPL_TRC("\nEvent   : SMP_LONG_TERM_KEY_REQUEST\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");
        APPL_TRC("Div  : 0x%04X\n", ediv);
        APPL_TRC("Rand : %02X %02X %02X %02X %02X %02X %02X %02X\n",
        peer_rand[0U], peer_rand[1U], peer_rand[2U], peer_rand[3U],
        peer_rand[4U], peer_rand[5U], peer_rand[6U], peer_rand[7U]);

        /* Do not process if status is failre */
        if (API_SUCCESS != status)
        {
            APPL_TRC("Long Term Key request with Error - 0x%04X. Dropping.\n", status);
            break;
        }

        /* Check if automatic mode is set */
        if(SMP_TRUE == automatic_mode)
        {
            /* Initialize */
            BT_mem_set(&info, 0x00, sizeof(SMP_AUTH_INFO));

            retval = BT_smp_get_device_security_info (bd_handle, &info);

            if (API_SUCCESS == retval)
            {
                /* Check if the link is authenticated */
                if ((SMP_ENTITY_AUTH_ON == info.param) ||
                    (SMP_TRUE == info.bonding))
                {
#ifdef SMP_LESC
                    if (info.pair_mode == SMP_LESC_MODE)
                    {
                        retval = BT_smp_get_device_keys
                                 (
                                     bd_handle,
                                     &p_keys,
                                     &p_key_info
                                 );

                        if (API_SUCCESS != retval)
                        {
                            APPL_ERR("Failed to get Peer Device Keys!!\n");
                        }
                        else
                        {
                            BT_mem_copy(ltk, p_key_info.enc_info, 16U);

                            /*
                             * NOTE: To check if masking of LTK according to negotiated key size
                             *       is needed when in secure connections only mode.
                             */
                        }
                    }
                    else
#endif /* SMP_LESC */
                    {
                        /* Get LTK for remote device */
                        retval = BT_smp_get_long_term_key_pl
                                 (
                                     peer_rand,
                                     ediv,
                                     ltk
                                 );

                        if(API_SUCCESS == retval)
                        {
                            /* Mask the key according to the negotiated key size */
                            BT_mem_set
                            (
                                (&ltk[info.ekey_size]),
                                0x00,
                                (SMP_LTK_SIZE - info.ekey_size)
                            );
                        }
                    }

                    if (API_SUCCESS == retval)
                    {
                        APPL_TRC("\n\nSending +ve LTK request reply.\n");
                        retval = BT_smp_long_term_key_request_reply
                                 (
                                     bd_handle,
                                     ltk,
                                     SMP_TRUE
                                 );
                    }
                }
                else
                {
                    retval = API_FAILURE;
                }
            }

            if (API_SUCCESS != retval)
            {
                APPL_TRC("\n\nSending -ve LTK request reply.\n");
                ltk_null = 0U;
                retval = BT_smp_long_term_key_request_reply
                         (
                             bd_handle,
                             &ltk_null,
                             SMP_FALSE
                         );
            }

            /* Notify application state machine */
            (BT_IGNORE_RETURN_VALUE) appl_get_handle_from_device_handle (*bd_handle,&appl_handle);

#ifdef APPL_FSM_SUPPORT
            fsm_param.handle = appl_handle;
            fsm_param.data_param = NULL;
            fsm_param.data_len = 0U;
            fsm_param.direction = 0x01U;

            /* Post Transport Configuration Request for SMP */
            appl_fsm_post_event
            (
                 ev_appl_transport_configuration_req,
                 (void *)(&fsm_param)
            );
#endif /* APPL_FSM_SUPPORT */
        }
        break;

    case SMP_RESOLVABLE_PVT_ADDR_CREATE_CNF:
        APPL_TRC("\nEvent   : SMP_RESOLVABLE_PVT_ADDR_CREATE_CNF\n");
        APPL_TRC("Status : %04X\n", status);
        if(API_SUCCESS == status)
        {
#ifndef DONT_USE_STANDARD_IO
            bd_addr = (UCHAR *) event_data;
#endif /* DONT_USE_STANDARD_IO */

            APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
            bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],
            bd_addr[5U]);
        }
        break;

    case SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF:
        APPL_TRC("\nEvent   : SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF\n");
        APPL_TRC("Status : %04X\n", status);
        if(API_SUCCESS == status)
        {
#ifndef DONT_USE_STANDARD_IO
            bd_addr = (UCHAR *) event_data;
#endif /* DONT_USE_STANDARD_IO */

            APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
            bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],
            bd_addr[5U]);
        }
        break;

#ifdef SMP_DATA_SIGNING
    case SMP_DATA_SIGNING_COMPLETE:
        APPL_TRC("\nEvent   : SMP_SIGNING_DATA_COMPLETE\n");
        APPL_TRC("Status : %04X\n", status);
        if(API_SUCCESS == status)
        {
            /* Update sign counter */
            sign_create_counter ++;

            APPL_TRC("Message Authentication Code : ");
            appl_dump_bytes
            (
                event_data,
                data_len
            );

            /* Form the signature */
            BT_mem_copy
            (&signature[sizeof(sign_create_counter)], event_data, data_len);

            APPL_TRC("Message Authentication Code : ");
            appl_dump_bytes
            (
                signature,
                sizeof (signature)
            );
        }

        /* Call gatt signature procedure handler */
        appl_gatt_signing_complete
        (status, signature, sizeof(signature));

        if (NULL != sign_data)
        {
            BT_free_mem (sign_data);
            sign_data = NULL;
        }

        break;

    case SMP_SIGN_DATA_VERIFICATION_COMPLETE:
        APPL_TRC("\nEvent   : SMP_SIGN_DATA_VERIFICATION_COMPLETE\n");
        APPL_TRC("Status : %04X\n", status);

        if (API_SUCCESS == status)
        {
            /* Form the signature */
            BT_PACK_LE_4_BYTE(signature, &sign_verify_counter);
            BT_mem_copy
            (&signature[sizeof(sign_verify_counter)], event_data, data_len);

            /* Update sign counter */
            sign_verify_counter ++;

            APPL_TRC("Message Authentication Code : ");
            appl_dump_bytes
            (
                event_data,
                data_len
            );
        }

        /* Call gatt signature procedure handler */
        appl_gatt_signing_verification_complete
        (status, signature, sizeof(signature));

        if (NULL != sign_data)
        {
            BT_free_mem (sign_data);
            sign_data = NULL;
        }

        break;
#endif /* SMP_DATA_SIGNING */

    case SMP_KEY_EXCHANGE_INFO:
        APPL_TRC ("\nRecvd SMP_KEY_EXCHANGE_INFO\n");
        APPL_TRC ("Status - 0x%04X\n", status);

        /* Reference the event data */
        kx_param = (SMP_KEY_XCHG_PARAM *) event_data;

        APPL_TRC("Remote keys negotiated - 0x%02X\n", kx_param->keys);
        APPL_TRC("Encryption Key Size negotiated - 0x%02X\n",
                kx_param->ekey_size);

        /* Reference the key information */
        key_info = kx_param->keys_info;

        /* Store the peer keys */
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
        peer_keys = kx_param->keys;
#endif
        BT_mem_copy (&peer_key_info, key_info, sizeof (SMP_KEY_DIST));

        APPL_TRC ("Encryption Info:\n");
        appl_dump_bytes(key_info->enc_info, sizeof (key_info->enc_info));
        APPL_TRC ("Master Identification Info:\n");
        appl_dump_bytes(key_info->mid_info, sizeof (key_info->mid_info));
        APPL_TRC ("Identity Info:\n");
        appl_dump_bytes(key_info->id_info, sizeof (key_info->id_info));
        APPL_TRC ("Identity Address Info:\n");
        appl_dump_bytes(key_info->id_addr_info, sizeof (key_info->id_addr_info));
        APPL_TRC ("Signature Info:\n");
        appl_dump_bytes(key_info->sign_info, sizeof (key_info->sign_info));

        break;

#ifdef SMP_LESC
    case SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST:
        APPL_TRC("\nEvent   : SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        APPL_TRC("Numeric Code : %06u\n", (*((UINT32 *)event_data) % 1000000U));

        /* If Automatic Mode , Accept the Key Comparison request */
        if ((SMP_TRUE == automatic_mode) && (SMP_TRUE == nc_automatic_mode))
        {
            smp_accept = SMP_NUM_COMP_CNF_POSITIVE;

            retval = BT_smp_nkey_comp_cnf_reply
                     (
                         bd_handle,
                         (void *)&smp_accept
                     );
        }
        break;

    case SMP_KEY_PRESS_NOTIFICATION_EVENT:
        APPL_TRC("\nEvent   : SMP_KEY_PRESS_NOTIFICATION_EVENT\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0U],bd_addr[1U],bd_addr[2U],bd_addr[3U],bd_addr[4U],bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
        (0U == bd_addr_type)? "Public Address": "Random Address");

        APPL_TRC("KeyPress Notification Value is:");

        switch(*(UCHAR *)event_data)
        {
            case SMP_LESC_PASSKEY_ENTRY_STARTED:
                APPL_TRC("SMP_LESC_PASSKEY_ENTRY_STARTED\n");
                break;
            case SMP_LESC_PASSKEY_DIGIT_ENTERED:
                APPL_TRC("SMP_LESC_PASSKEY_DIGIT_ENTERED\n");
                break;
            case SMP_LESC_PASSKEY_DIGIT_ERASED:
                APPL_TRC("SMP_LESC_PASSKEY_DIGIT_ERASED\n");
                break;
            case SMP_LESC_PASSKEY_CLEARED:
                APPL_TRC("SMP_LESC_PASSKEY_CLEARED\n");
                break;
            case SMP_LESC_PASSKEY_ENTRY_COMPLETED:
                APPL_TRC("SMP_LESC_PASSKEY_ENTRY_COMPLETED\n");
                break;
            default:
                APPL_TRC("Unknown KeyPress Value 0x%02X Received\n",*(UCHAR *)event_data);
                break;
        }
        break;
#endif /* SMP_LESC */

    case SMP_INVALID_FSM_TRANSITION:
        APPL_TRC("\nEvent   : SMP_INVALID_FSM_TRANSITION\n");
        APPL_TRC("BD Address : %02X %02X %02X %02X %02X %02X\n",
            bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        APPL_TRC("BD addr type : %s\n",
            (0U == bd_addr_type) ? "Public Address" : "Random Address");

        break;

    default:
        APPL_TRC("\nERROR!!! Received unknown event. event = %02X\n", event);
        break;
    }

    return API_SUCCESS;
}

#ifdef HAVE_SMP_REPEAT_TRACKING
DECL_STATIC void smp_repeat_timeout_handler (void * data_param, UINT16 datalen)
{
    /* Reinitialize timer handle */
    smp_repeat_timer = BT_TIMER_HANDLE_INIT_VAL;

    APPL_TRC ("Repeat Tracking Timer expired.\n");
}
#endif /* HAVE_SMP_REPEAT_TRACKING */

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS

void appl_smp_invert_byte_stream (UCHAR *buffer, UINT16 buffer_len)
{
    UINT16 index;
    UCHAR  temp;

    for (index = 0U; index < (buffer_len/2U); index ++)
    {
        temp = buffer[index];
        buffer[index] = buffer[buffer_len - (index + 1U)];
        buffer[buffer_len - (index + 1U)] = temp;
    }
}

void appl_smp_test_f4 (void)
{
    /* This function emulates the F4 function defined in LESC Spec */
    /* U || V || Z */
    API_RESULT retval;
    UCHAR * conf_val;
    UCHAR pkax[32U] = {
                         0x20U, 0xB0U, 0x03U, 0xD2U, 0xF2U, 0x97U, 0xBEU, 0x2CU,
                         0x5EU, 0x2CU, 0x83U, 0xA7U, 0xE9U, 0xF9U, 0xA5U, 0xB9U,
                         0xEFU, 0xF4U, 0x91U, 0x11U, 0xACU, 0xF4U, 0xFDU, 0xDBU,
                         0xCCU, 0x03U, 0x01U, 0x48U, 0x0EU, 0x35U, 0x9DU, 0xE6U
                     };
    UCHAR pkbx[32U] = {
                         0x55U, 0x18U, 0x8BU, 0x3DU, 0x32U, 0xF6U, 0xBBU, 0x9AU,
                         0x90U, 0x0AU, 0xFCU, 0xFBU, 0xEEU, 0xD4U, 0xE7U, 0x2AU,
                         0x59U, 0xCBU, 0x9AU, 0xC2U, 0xF1U, 0x9DU, 0x7CU, 0xFBU,
                         0x6BU, 0x4FU, 0xDDU, 0x49U, 0xF4U, 0x7FU, 0xC5U, 0xFDU
                     };
    UCHAR rand_key[16U] = {
                             0xD5U, 0xCBU, 0x84U, 0x54U, 0xD1U, 0x77U, 0x73U, 0x3EU,
                             0xFFU, 0xFFU, 0xB2U, 0xECU, 0x71U, 0x2BU, 0xAEU, 0xABU
                         };
    UCHAR z = 0x00U;
    UINT16 text_len;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (32U + 32U + 1U);

    appl_smp_invert_byte_stream (pkax, sizeof(pkax));
    appl_smp_invert_byte_stream (pkbx, sizeof(pkbx));
    appl_smp_invert_byte_stream (rand_key, sizeof(rand_key));

    CONSOLE_OUT ("smp_tbx_f4_eval()\n");
    CONSOLE_OUT ("PKa:\n");
    appl_dump_bytes (pkax, SMP_LESC_PUBLIC_KEY_X_SIZE);
    CONSOLE_OUT ("PKb:\n");
    appl_dump_bytes (pkbx, SMP_LESC_PUBLIC_KEY_X_SIZE);
    CONSOLE_OUT ("Random Key:\n");
    appl_dump_bytes (rand_key, 16U);
    CONSOLE_OUT ("Z: 0x%02X\n", z);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    conf_val   = BT_alloc_mem(16U);

    /* Populate Plain Text */
    plain_text[0U] = z;
    BT_mem_copy(&plain_text[1U],pkbx,SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_copy(&plain_text[1U + SMP_LESC_PUBLIC_KEY_X_SIZE], pkax,SMP_LESC_PUBLIC_KEY_X_SIZE);

    /* Populate Encryption Key */
    BT_mem_copy(key,rand_key,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 conf_val,
                 SMP_AES_CMAC_LESC_CONF_VAL_GEN_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}

void appl_smp_test_f6 (void)
{
    /* This function emulates the F4 function defined in LESC Spec */
    /* N1 || N2 || R || IOCap || A1 || A2 */
    API_RESULT retval;
    UCHAR N1[16U] = {
                         0xD5U, 0xCBU, 0x84U, 0x54U, 0xD1U, 0x77U, 0x73U, 0x3EU,
                         0xFFU, 0xFFU, 0xB2U, 0xECU, 0x71U, 0x2BU, 0xAEU, 0xABU
                   };
    UCHAR N2[16U] = {
                         0xA6U, 0xE8U, 0xE7U, 0xCCU, 0x25U, 0xA7U, 0x5FU, 0x6EU,
                         0x21U, 0x65U, 0x83U, 0xF7U, 0xFFU, 0x3DU, 0xC4U, 0xCFU
                   };
    UCHAR mackey[16U] = {
                             0x29U, 0x65U, 0xF1U, 0x76U, 0xA1U, 0x08U, 0x4AU, 0x02U,
                             0xFDU, 0x3FU, 0x6AU, 0x20U, 0xCEU, 0x63U, 0x6EU, 0x20U
                     };
    UCHAR rand[16U] = {
                             0x12U, 0xA3U, 0x34U, 0x3BU, 0xB4U, 0x53U, 0xBBU, 0x54U,
                             0x08U, 0xDAU, 0x42U, 0xD2U, 0x0CU, 0x2DU, 0x0FU, 0xC8U
                     };
    UCHAR io_cap[3U] = {0x02U, 0x01U, 0x01U};

    BT_DEVICE_ADDR l_bd_addr = { {0xCEU, 0xBFU, 0x37U, 0x37U, 0x12U, 0x56U}, 0x00U};
    BT_DEVICE_ADDR r_bd_addr = { {0xC1U, 0xCFU, 0x2DU, 0x70U, 0x13U, 0xA7U}, 0x00U};

    UINT16 text_len, offset;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (16U + 16U + 16U + 3U + 7U + 7U);
    offset = 0U;

    appl_smp_invert_byte_stream (N1, sizeof(N1));
    appl_smp_invert_byte_stream (N2, sizeof(N2));
    appl_smp_invert_byte_stream (rand, sizeof(rand));
    appl_smp_invert_byte_stream (mackey, sizeof(mackey));

    CONSOLE_OUT ("Dumping DH key\n");
    appl_dump_bytes (mackey, 16U);
    CONSOLE_OUT("Dumping L RAND\n");
    appl_dump_bytes (N1, 16U);
    CONSOLE_OUT("Dumping R RAND\n");
    appl_dump_bytes (N2, 16U);
    CONSOLE_OUT ("Dumping RAND\n");
    appl_dump_bytes (rand, 16U);
    CONSOLE_OUT("Dumping IO CAPAB\n");
    appl_dump_bytes (io_cap, 3U);
    CONSOLE_OUT ("Dumping l_bd_addr\n");
    appl_dump_bytes (l_bd_addr.addr, 6U);
    CONSOLE_OUT ("Dumping r_bd_addr\n");
    appl_dump_bytes (r_bd_addr.addr, 6U);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    enc_out   = BT_alloc_mem(16U);

    /* Populate Plain Text */
    /* A2 */
    BT_COPY_BD_ADDR(&plain_text[offset],r_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],r_bd_addr.type);
    offset += 1U;

    /* A1 */
    BT_COPY_BD_ADDR(&plain_text[offset],l_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;

    BT_COPY_TYPE(plain_text[offset],l_bd_addr.type);
    offset += 1U;

    /* IOCap */
    BT_mem_copy(&plain_text[offset],io_cap,0x03U);
    offset += 0x03U;

    /* R */
    BT_mem_copy(&plain_text[offset], rand, SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* N2 */
    BT_mem_copy(&plain_text[offset],N2,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* N1 */
    BT_mem_copy(&plain_text[offset],N1,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* Populate Encryption Key */
    BT_mem_copy(key,mackey,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 enc_out,
                 SMP_AES_CMAC_LESC_CHK_VAL_GEN_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}

void appl_smp_test_g2 (void)
{
    /* This function emulates the g2() function defined in LESC Spec */
    /* U || V || Y */
    API_RESULT retval;
    UCHAR pkax[32U] = {
                         0x20U, 0xB0U, 0x03U, 0xD2U, 0xF2U, 0x97U, 0xBEU, 0x2CU,
                         0x5EU, 0x2CU, 0x83U, 0xA7U, 0xE9U, 0xF9U, 0xA5U, 0xB9U,
                         0xEFU, 0xF4U, 0x91U, 0x11U, 0xACU, 0xF4U, 0xFDU, 0xDBU,
                         0xCCU, 0x03U, 0x01U, 0x48U, 0x0EU, 0x35U, 0x9DU, 0xE6U
                     };
    UCHAR pkbx[32U] = {
                         0x55U, 0x18U, 0x8BU, 0x3DU, 0x32U, 0xF6U, 0xBBU, 0x9AU,
                         0x90U, 0x0AU, 0xFCU, 0xFBU, 0xEEU, 0xD4U, 0xE7U, 0x2AU,
                         0x59U, 0xCBU, 0x9AU, 0xC2U, 0xF1U, 0x9DU, 0x7CU, 0xFBU,
                         0x6BU, 0x4FU, 0xDDU, 0x49U, 0xF4U, 0x7FU, 0xC5U, 0xFDU
                     };

    UCHAR lesc_g2_key[16U] = {
                         0xD5U, 0xCBU, 0x84U, 0x54U, 0xD1U, 0x77U, 0x73U, 0x3EU,
                         0xFFU, 0xFFU, 0xB2U, 0xECU, 0x71U, 0x2BU, 0xAEU, 0xABU
                   };
    UCHAR rand[16U] = {
                          0xA6U, 0xE8U, 0xE7U, 0xCCU, 0x25U, 0xA7U, 0x5FU, 0x6EU,
                          0x21U, 0x65U, 0x83U, 0xF7U, 0xFFU, 0x3DU, 0xC4U, 0xCFU
                     };

    UINT16 text_len;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (32U + 32U + 16U);

    appl_smp_invert_byte_stream (pkax, sizeof(pkax));
    appl_smp_invert_byte_stream (pkbx, sizeof(pkbx));
    appl_smp_invert_byte_stream (lesc_g2_key, sizeof(lesc_g2_key));
    appl_smp_invert_byte_stream (rand, sizeof(rand));

    CONSOLE_OUT ("smp_tbx_g2_eval()\n");
    CONSOLE_OUT ("PKa:\n");
    appl_dump_bytes (pkax, SMP_LESC_PUBLIC_KEY_X_SIZE);
    CONSOLE_OUT ("PKb:\n");
    appl_dump_bytes (pkbx, SMP_LESC_PUBLIC_KEY_X_SIZE);
    CONSOLE_OUT ("Key:\n");
    appl_dump_bytes (lesc_g2_key, 16U);
    CONSOLE_OUT ("Rand:\n");
    appl_dump_bytes (rand, 16U);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    enc_out   = BT_alloc_mem(16U);

    /* Populate Plain Text */
    BT_mem_copy(&plain_text[0U], rand, 16U);
    BT_mem_copy(&plain_text[16U], pkbx, SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_copy(&plain_text[16U + SMP_LESC_PUBLIC_KEY_X_SIZE], pkax,SMP_LESC_PUBLIC_KEY_X_SIZE);

    /* Populate Encryption Key */
    BT_mem_copy(key,lesc_g2_key,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 enc_out,
                 SMP_AES_CMAC_LESC_NUM_COMP_VAL_GEN_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}

void appl_smp_test_h6 (void)
{
    /* This function emulates the h6() function defined in LESC Spec */

    API_RESULT retval;
    UCHAR key_id[4U] = {0x72U, 0x62U, 0x65U, 0x6CU};
    UCHAR lesc_h6_key[16U] = {
                                0x9BU, 0x7DU, 0x39U, 0x0AU,
                                0xA6U, 0x10U, 0x10U, 0x34U,
                                0x05U, 0xADU, 0xC8U, 0x57U,
                                0xA3U, 0x34U, 0x02U, 0xECU,
                            };

    UINT16 text_len;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = sizeof(key_id);

    /* appl_smp_invert_byte_stream (key_id, sizeof(key_id)); */
    /* appl_smp_invert_byte_stream (lesc_h6_key, sizeof(lesc_h6_key)); */

    CONSOLE_OUT ("smp_tbx_h6_eval()\n");
    CONSOLE_OUT ("Key_Id:\n");
    appl_dump_bytes (key_id, sizeof(key_id));
    CONSOLE_OUT ("Key:\n");
    appl_dump_bytes (lesc_h6_key, 16U);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    enc_out   = BT_alloc_mem(16U);

    /* Populate Plain Text */
    BT_mem_copy(&plain_text[0U], key_id, sizeof(key_id));

    /* Populate Encryption Key */
    BT_mem_copy(key,lesc_h6_key,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 enc_out,
                 SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}

void appl_smp_test_f5 (void)
{
    appl_smp_test_f5_temp();
}

void appl_smp_test_f5_temp (void)
{
    /* This function emulates the h6() function defined in LESC Spec */

    API_RESULT retval;
    UCHAR salt[16U] = {
                         0xBEU, 0x83U, 0x60U, 0x5AU, 0xDBU, 0x0BU, 0x37U, 0x60U,
                         0x38U, 0xA5U, 0xF5U, 0xAAU, 0x91U, 0x83U, 0x88U, 0x6CU
                     };
    UCHAR W[32U] = {
                      0xECU, 0x02U, 0x34U, 0xA3U, 0x57U, 0xC8U, 0xADU, 0x05U,
                      0x34U, 0x10U, 0x10U, 0xA6U, 0x0AU, 0x39U, 0x7DU, 0x9BU,
                      0x99U, 0x79U, 0x6BU, 0x13U, 0xB4U, 0xF8U, 0x66U, 0xF1U,
                      0x86U, 0x8DU, 0x34U, 0xF3U, 0x73U, 0xBFU, 0xA6U, 0x98U
                  };

    UINT16 text_len;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = sizeof(W);

    appl_smp_invert_byte_stream (W, sizeof(W));

    CONSOLE_OUT ("smp_tbx_f5_eval()\n");
    CONSOLE_OUT ("DHkey:\n");
    appl_dump_bytes (W, sizeof(W));
    CONSOLE_OUT ("SALT:\n");
    appl_dump_bytes (salt, 16U);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    enc_out    = BT_alloc_mem(16U);

    /* Populate Plain Text */
    BT_mem_copy(&plain_text[0U], W, sizeof(W));

    /* Populate Encryption Key */
    BT_mem_copy(key,salt,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 enc_out,
                 SMP_AES_CMAC_LESC_T_KEY_GEN_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}

void appl_smp_test_f5_ltk (UCHAR * enc_key)
{
    /* This function emulates the h6() function defined in LESC Spec */

    API_RESULT retval;
    UCHAR N1[16U] = {
                         0xD5U, 0xCBU, 0x84U, 0x54U, 0xD1U, 0x77U, 0x73U, 0x3EU,
                         0xFFU, 0xFFU, 0xB2U, 0xECU, 0x71U, 0x2BU, 0xAEU, 0xABU
                   };
    UCHAR N2[16U] = {
                         0xA6U, 0xE8U, 0xE7U, 0xCCU, 0x25U, 0xA7U, 0x5FU, 0x6EU,
                         0x21U, 0x65U, 0x83U, 0xF7U, 0xFFU, 0x3DU, 0xC4U, 0xCFU
                   };
    BT_DEVICE_ADDR l_bd_addr = { {0xCEU, 0xBFU, 0x37U, 0x37U, 0x12U, 0x56U}, 0x00U};
    BT_DEVICE_ADDR r_bd_addr = { {0xC1U, 0xCFU, 0x2DU, 0x70U, 0x13U, 0xA7U}, 0x00U};

    UCHAR keyID[] = {0x65U, 0x6CU, 0x74U, 0x62U};

    UINT16 text_len;
    UCHAR  offset;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (1U + 4U + 16U + 16U + 7U + 7U + 2U);
    offset = 0U;

    appl_smp_invert_byte_stream (N1, sizeof(N1));
    appl_smp_invert_byte_stream (N2, sizeof(N2));

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    enc_out    = BT_alloc_mem(16U);

    /* Populate Plain Text */
    /* Length 256 = 0x0100 */
    plain_text[offset] = 0x00U;
    offset ++;
    plain_text[offset] = 0x01U;
    offset ++;

    /* A2 */
    BT_COPY_BD_ADDR(&plain_text[offset],r_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],r_bd_addr.type);
    offset += 1U;

    /* A1 */
    BT_COPY_BD_ADDR(&plain_text[offset],l_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],l_bd_addr.type);
    offset += 1U;

    /* N2 */
    BT_mem_copy(&plain_text[offset],N2,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* N1 */
    BT_mem_copy(&plain_text[offset],N1,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* KeyID */
    BT_mem_copy(&plain_text[offset],keyID, 4U);
    offset += 4U;

    /* Counter = 1 */
    plain_text[offset] = 0x01U;

    /* Populate Encryption Key */
    BT_mem_copy(key,enc_key,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 enc_out,
                 SMP_AES_CMAC_LESC_LTK_GEN_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}

void appl_smp_test_f5_mackey (UCHAR * enc_key)
{
    /* This function emulates the h6() function defined in LESC Spec */

    API_RESULT retval;
    UCHAR N1[16U] = {
                         0xD5U, 0xCBU, 0x84U, 0x54U, 0xD1U, 0x77U, 0x73U, 0x3EU,
                         0xFFU, 0xFFU, 0xB2U, 0xECU, 0x71U, 0x2BU, 0xAEU, 0xABU
                   };
    UCHAR N2[16U] = {
                         0xA6U, 0xE8U, 0xE7U, 0xCCU, 0x25U, 0xA7U, 0x5FU, 0x6EU,
                         0x21U, 0x65U, 0x83U, 0xF7U, 0xFFU, 0x3DU, 0xC4U, 0xCFU
                   };
    BT_DEVICE_ADDR l_bd_addr = { {0xCEU, 0xBFU, 0x37U, 0x37U, 0x12U, 0x56U}, 0x00U};
    BT_DEVICE_ADDR r_bd_addr = { {0xC1U, 0xCFU, 0x2DU, 0x70U, 0x13U, 0xA7U}, 0x00U};

    UCHAR keyID[] = {0x65U, 0x6CU, 0x74U, 0x62U};

    UINT16 text_len;
    UCHAR  offset;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (1U + 4U + 16U + 16U + 7U + 7U + 2U);
    offset = 0U;

    appl_smp_invert_byte_stream (N1, sizeof(N1));
    appl_smp_invert_byte_stream (N2, sizeof(N2));

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16U);
    enc_out    = BT_alloc_mem(16U);

    /* Populate Plain Text */
    /* Length 256 = 0x0100 */
    plain_text[offset] = 0x00U;
    offset ++;
    plain_text[offset] = 0x01U;
    offset ++;

    /* A2 */
    BT_COPY_BD_ADDR(&plain_text[offset],r_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],r_bd_addr.type);
    offset += 1U;

    /* A1 */
    BT_COPY_BD_ADDR(&plain_text[offset],l_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],l_bd_addr.type);
    offset += 1U;

    /* N2 */
    BT_mem_copy(&plain_text[offset],N2,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* N1 */
    BT_mem_copy(&plain_text[offset],N1,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* KeyID */
    BT_mem_copy(&plain_text[offset],keyID, 4U);
    offset += 4U;

    /* Counter = 0 */
    plain_text[offset] = 0x00U;

    /* Populate Encryption Key */
    BT_mem_copy(key,enc_key,16U);

    retval = smp_tbx_test_lesc_funcs
             (
                 plain_text,
                 text_len,
                 key,
                 enc_out,
                 SMP_AES_CMAC_LESC_MACKEY_GEN_OP,
                 appl_smp_lesc_test_funcs_complete
             );
}


void appl_smp_lesc_test_funcs_complete
     (
         UCHAR                        * plain_text,
         UINT16                       plain_text_len,
         UCHAR                        * key,
         UCHAR                        aes_op,
         UCHAR                        * enc_out
     )
 {

    CONSOLE_OUT("\nSMP CMAC Test Complete CB 0x%02X received\n", aes_op);

    /* Check the AES CMAC operation initiated */
    switch(aes_op)
    {
        case SMP_AES_CMAC_SIGN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_SIGN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_CONF_VAL_GEN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_CONF_VAL_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_T_KEY_GEN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_T_KEY_GEN_OP\n");
            BT_mem_copy(appl_smp_lesc_test_f5_T,enc_out,16U);
            appl_smp_test_f5_ltk(appl_smp_lesc_test_f5_T);
            break;

        case SMP_AES_CMAC_LESC_LTK_GEN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_LTK_GEN_OP\n");
            appl_smp_test_f5_mackey(appl_smp_lesc_test_f5_T);
            break;

        case SMP_AES_CMAC_LESC_MACKEY_GEN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_MACKEY_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_CHK_VAL_GEN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_CHK_VAL_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_NUM_COMP_VAL_GEN_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_NUM_COMP_VAL_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP:
            CONSOLE_OUT ("SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP\n");
            break;

        default:
            CONSOLE_OUT("Invalid Operation\n");
            break;
    }

    CONSOLE_OUT ("Data:\n");
    appl_dump_bytes(plain_text, plain_text_len);

    CONSOLE_OUT ("Key:\n");
    appl_dump_bytes(key, 16U);

    CONSOLE_OUT ("MAC:\n");
    appl_dump_bytes(enc_out, 16U);

    BT_free_mem(plain_text);
    BT_free_mem(key);
    BT_free_mem(enc_out);

    return;
}
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

#endif /* SMP */


