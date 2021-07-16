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

#include "appl_service.h"

#ifdef AES_CMAC_UTIL
#include "aes_cmac.h"
#endif /* AES_CMAC_UTIL */

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_INFO
#endif

#define LIBRARY_LOG_NAME         ( "APPL_SMP" )
#include "iot_logging_setup.h"

#ifdef SMP

/* --------------------------------------------- External Global Variables */
/* Application compilation switch to enable deterministic PASSKEY support */
/* #define APPL_SMP_UPDATE_PASSKEY */

/* Application compilation switch to enable auto population of Peer Address */
#define APPL_SMP_AUTO_FILL_BD_ADDR

/**
 * Application compilation switch to enable auto confirmation of Numeric
 * Comparison Value
 */
#define APPL_SMP_NC_AUTO_REPLY

/**
 * The below piece of flag is needed only for SMP LESC OOB Testcases
 * while testing against PTS v 7.6.1.
 */
/* Application compilation switch to enable */
/* #define APPL_SMP_SUPPORT_REVERSE_INPUT_STRING */

#if ((defined HCI_LE_PRIVACY_1_2_SUPPORT) && (define HCI_LE_SET_PRIVACY_MODE_SUPPORT))
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
 *   2. HCI_LE_SET_PRIVACY_MODE_SUPPORT -> For setting the privacy mode
 *
 * This feature is disabled by default.
 * It is observed in few controllers that, after a device is added to a
 * resolving list. Any ADV packet with the added Peer Identity addresss is not
 * forwarded to the Host as an ADV event even if
 *   1. the IUT is not scanning with privacy enabled and
 *   2. the Host has not enabled the address resolution.
 */
/* #define APPL_SMP_AUTO_ADD_TO_RSLV_LIST */
#endif /* ((defined HCI_LE_PRIVACY_1_2_SUPPORT) && (define HCI_LE_SET_PRIVACY_MODE_SUPPORT)) */

/* --------------------------------------------- External Global Variables */
#ifdef APPL_SMP_AUTO_FILL_BD_ADDR
extern BT_DEVICE_ADDR g_bd_addr;
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/* Application automatic SMP handling identifier */
DECL_STATIC UCHAR automatic_mode = SMP_TRUE;

/* Application SMP pairable mode */
DECL_STATIC UCHAR pairable_mode = SMP_TRUE;

#ifdef SMP_LESC
/* Applcation SMP Pairing Mode [Legacy or LESC] */
DECL_STATIC UCHAR g_appl_pair_mode = SMP_LESC_MODE;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
DECL_STATIC SMP_LESC_LK_LTK_GEN_PL g_smp_lk_ltk;
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#ifdef HAVE_SMP_REPEAT_TRACKING
/* Application Repeat Attempt track timer */
#define SMP_REPEAT_ATTEMPT_TIMEOUT_MIN        8
#define SMP_REPEAT_ATTEMPT_TIMEOUT_MAX        1024
#define SMP_REPEAT_ATTEMPT_TIMEOUT_FRACTION   1 /* Power of 2 */

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

#ifdef APPL_SMP_UPDATE_PASSKEY
#define APPL_SMP_PASSKEY                        999999
#endif /* APPL_SMP_UPDATE_PASSKEY */

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
UCHAR appl_smp_lesc_test_f5_state = 0xFF;
UCHAR appl_smp_lesc_test_f5_T[16];
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

#if 0
/*fix build warning: declared but never used.*/
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
static const UCHAR appl_smp_menu[] = "\n\
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
 \n\
 200. Initiate Disconnection \n\
 \n\
Enter Your Choice: ";
#else /* SMP_TBX_TEST_LESC_FUNCTIONS */
static const UCHAR appl_smp_menu[] = "\n\
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
 \n\
 200. Initiate Disconnection \n\
 \n\
Enter Your Choice: ";
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

static const UCHAR appl_smp_pl_menu[] = "\n\
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
 \n\
 50. Reverse Byte Stream \n\
Enter Your Choice: ";
#endif /* #if 0 */
/* Global BD Address of the SMP procedure */
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
DECL_STATIC BT_DEVICE_ADDR appl_smp_bd_addr;
#endif

#if 0
/* Enable/Disable Cross transport inputs */
DECL_STATIC UCHAR appl_smp_xtxp_enable;
#endif

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

    IotLogDebug("\n Generated SMP OOB CONFIRM Value: ");
    IotLogDebug("\nCNF Value [LSB-MSB]: ");
    for (i = 0; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
    {
        IotLogDebug("%02X ", appl_lesc_oob->cnf_val[i]);
    }
    IotLogDebug("\n CNF Value[MSB-LSB]:(spaced) ");
    for (i = 0; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
    {
        IotLogDebug("%02X ", appl_lesc_oob->cnf_val[(SMP_OOB_CONFIRM_VAL_SIZE - 1) - i]);
    }
    IotLogDebug("\n CNF Value[MSB-LSB]: ");
    for (i = 0; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
    {
        IotLogDebug("%02X", appl_lesc_oob->cnf_val[(SMP_OOB_CONFIRM_VAL_SIZE - 1) - i]);
    }

    IotLogDebug("\n Generated SMP OOB RAND Value: ");
    IotLogDebug("\nRAND Value [LSB-MSB]: ");
    for (i = 0; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
    {
        IotLogDebug("%02X ", appl_lesc_oob->rand[i]);
    }
    IotLogDebug("\nRAND Value [MSB-LSB]:(spaced) ");
    for (i = 0; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
    {
        IotLogDebug("%02X ", appl_lesc_oob->rand[(SMP_OOB_RANDOM_VAL_SIZE - 1) - i]);
    }
    IotLogDebug("\nRAND Value [MSB-LSB]: ");
    for (i = 0; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
    {
        IotLogDebug("%02X", appl_lesc_oob->rand[(SMP_OOB_RANDOM_VAL_SIZE - 1) - i]);
    }

    IotLogDebug("\n\n");
}
#endif /* (defined SMP_LESC) && (defined SMP_HAVE_OOB_SUPPORT) */

#ifdef SMP_LESC
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
void appl_smp_lesc_xtxp_ltk_complete(SMP_LESC_LK_LTK_GEN_PL * xtxp)
{
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth_info;

    IotLogDebug("\n LTK of the device is ...\n");
    appl_dump_bytes(xtxp->ltk, SMP_LTK_SIZE);

    IotLogDebug("\n LK of the device is ...\n");
    appl_dump_bytes(xtxp->lk, SMP_LK_SIZE);

    /* Update the LTK. TODO: Check the key strength */
    BT_smp_add_device(&appl_smp_bd_addr, &bd_handle);

    auth_info.bonding = SMP_BONDING;
    auth_info.pair_mode = SMP_LESC_MODE;
    auth_info.security = SMP_SEC_LEVEL_3;
    BT_smp_update_ltk(&bd_handle, xtxp->ltk, 16, &auth_info);
}

void appl_smp_lesc_xtxp_lk_complete(SMP_LESC_LK_LTK_GEN_PL * xtxp)
{
    IotLogDebug("\n LK of the device is ...\n");
    appl_dump_bytes(xtxp->lk, SMP_LK_SIZE);

    IotLogDebug("\n LTK of the device is ...\n");
    appl_dump_bytes(xtxp->ltk, SMP_LTK_SIZE);

    /* Update the LTK. TODO: Check the key strength */
    IotLogDebug("Adding Device to Device DB .. ");
    BT_sm_add_device(appl_smp_bd_addr.addr);
    BT_sm_set_device_link_key(appl_smp_bd_addr.addr, xtxp->lk);
}

void appl_smp_lesc_txp_key_gen_complete (SMP_LESC_LK_LTK_GEN_PL * appl_txp_key)
{
    IotLogDebug("\n LTK of the device is ...\n");
    appl_dump_bytes(appl_txp_key->ltk,SMP_LTK_SIZE);

    IotLogDebug("\n LK of the device is ...\n");
    appl_dump_bytes(appl_txp_key->lk,SMP_LK_SIZE);

    IotLogDebug("\n Temp output of the h6 is ...\n");
    appl_dump_bytes(appl_txp_key->temp,SMP_AES_CMAC_MAC_SIZE);

    /* Assign Keys to Global struct */
    g_smp_lk_ltk = *appl_txp_key;
    (void)g_smp_lk_ltk;
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
API_RESULT appl_smp_strtoarray
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
    base = 16;
    c0 = 0;
    c1 = 0;

    /* Fill with Zeros */
    BT_mem_set(output_array, 0, output_array_len);

    /* Check the length */
    if (data_length > (2 * output_array_len))
    {
        return API_FAILURE;
    }

    /* Process from end */
    output_index = output_array_len - 1;
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
            c0 = (c0 - 'a' + 10);
        }
        else if (IS_UPPER(c0))
        {
            c0 = (c0 - 'A' + 10);
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
            c1 = (c1 - 'a' + 10);
        }
        else if (IS_UPPER(c1))
        {
            c1 = (c1 - 'A' + 10);
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
#if 0
void smp_pl_operations(void)
{
    unsigned int  choice;
    unsigned int  read;

    SMP_KEY_DIST * key_info;

    SMP_BD_ADDR   bd_addr;
    SMP_BD_HANDLE bd_handle;

    API_RESULT    retval;
    UCHAR iocap, oob, keys, ekeysize;

#ifdef SMP_HAVE_OOB_SUPPORT
    SMP_OOB_DATA oob_val;
    UCHAR         i;
#endif /* SMP_HAVE_OOB_SUPPORT */

    BT_LOOP_FOREVER()
    {
        retval = API_SUCCESS;

        IotLogDebug("%s", appl_smp_pl_menu);
        CONSOLE_IN("%u", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                IotLogDebug("Refreshing...\n");
                break;

            case 2:
#ifdef SMP_HAVE_OOB_SUPPORT
                IotLogDebug ("Enter peer device handle for OOB capability: ");
                CONSOLE_IN("%u", &read);
#endif /* SMP_HAVE_OOB_SUPPORT */

                BT_smp_get_local_capability_pl(&iocap, &keys, &ekeysize);

#ifdef SMP_HAVE_OOB_SUPPORT
                bd_handle = (SMP_BD_HANDLE)read;
                smp_get_peer_oob_availability_pl (&bd_handle, &oob);
#else /* SMP_HAVE_OOB_SUPPORT */
                oob = 0x00;
#endif /* SMP_HAVE_OOB_SUPPORT */

                IotLogDebug ("IO Capability : 0x%02X\n", iocap);
                IotLogDebug ("OOB : 0x%02X\n", oob);

#ifdef SMP_HAVE_OOB_SUPPORT
                if (oob)
                {
                    smp_get_oob_tk_pl (&bd_handle, oob_val.temp_key);

                    IotLogDebug ("OOB Data : ");
                    for (i = 0; i < SMP_OOB_TEMP_KEY_SIZE; i++)
                    {
                        IotLogDebug ("%x ", oob_val.temp_key[i]);
                    }
                    IotLogDebug ("\n");
                }
#endif /* SMP_HAVE_OOB_SUPPORT */

                IotLogDebug ("Key Exchange : 0x%02X\n", keys);
                IotLogDebug ("Max Enc Key Size : 0x%02X\n", ekeysize);
                break;

            case 3:
                IotLogDebug("Enter Platform MITM policy\n");
                IotLogDebug("(0-MITM Best Effor, 1-MITM Strict): ");
                CONSOLE_IN("%u", &read);

                BT_smp_set_mitm_policy_pl ((UCHAR)read);
                break;

            case 4:
#ifdef SMP_LESC
                IotLogDebug("Enter Platform LESC policy\n");
                IotLogDebug("(0-LESC Best Effor, 1-LESC Strict): ");
                CONSOLE_IN("%u", &read);

                BT_smp_set_lesc_policy_pl ((UCHAR)read);
#else /* SMP_LESC */
                IotLogDebug("\nSMP LE Secure Connections not enabled!!!\n");
#endif /* SMP_LESC */
                break;

            case 5:
                IotLogDebug("Enter Platform IO Capability\n");
                IotLogDebug("(0-DisplayOnly, 1-DisplayYesNo, 2-KeybdOnly, 3-NoIO, 4-KeybdDisplay, 5-Default): ");
                CONSOLE_IN("%u", &read);

                BT_smp_set_io_cap_pl ((UCHAR)read);
                break;

            case 6:
#ifdef SMP_HAVE_OOB_SUPPORT
                IotLogDebug ("Enter peer BD Address: ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug ("Enter peer BD Type: ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR)read;

                IotLogDebug("Enter OOB Flag\n");
                IotLogDebug("(1: OOB data present, 0: OOB data not present): ");
                CONSOLE_IN("%u", &read);

                if (read)
                {
                    unsigned int sub_read;

                    IotLogDebug("Is TK value Present?\n");
                    IotLogDebug("(1:  TK present, 0: TK not present): ");
                    CONSOLE_IN("%u", &sub_read);

                    if (sub_read)
                    {
#ifdef APPL_SMP_HAVE_CONST_OOB_TK_VAL
                        BT_mem_set(oob_val.temp_key, 0x11, SMP_OOB_TEMP_KEY_SIZE);
#else /* APPL_SMP_HAVE_CONST_OOB_TK_VAL */
                        IotLogDebug ("Enter %d octet OOB Tk (in HEX): ", SMP_OOB_TEMP_KEY_SIZE);
                        for (i = 0; i < SMP_OOB_TEMP_KEY_SIZE; i++)
                        {
                            CONSOLE_IN ("%x", &choice);
                            oob_val.temp_key[i] = (UCHAR)choice;
                        }
#endif /* APPL_SMP_HAVE_CONST_OOB_TK_VAL */
                    }
#ifdef SMP_LESC
                    IotLogDebug("Is LESC CNF value Present?\n");
                    IotLogDebug("(1:  CNF present, 0: CNF not present): ");
                    CONSOLE_IN("%u", &sub_read);

                    if (sub_read)
                    {
                        IotLogDebug ("Enter %d octet OOB CNF (in HEX): ", SMP_OOB_CONFIRM_VAL_SIZE);
                        for (i = 0; i < SMP_OOB_CONFIRM_VAL_SIZE; i++)
                        {
                            CONSOLE_IN ("%x", &choice);
                            oob_val.lesc_cnf_val[i] = (UCHAR)choice;
                        }

                        IotLogDebug("Is LESC Rand value Present?\n");
                        IotLogDebug("(1:  Rand present, 0: Rand not present): ");
                        CONSOLE_IN("%u", &sub_read);

                        if (sub_read)
                        {
                            IotLogDebug ("Enter %d octet OOB Rand (in HEX): ", SMP_OOB_RANDOM_VAL_SIZE);
                            for (i = 0; i < SMP_OOB_RANDOM_VAL_SIZE; i++)
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

                retval = BT_smp_add_device_pl(&bd_addr);

                IotLogDebug("\n Adding Device to PL List returned 0x%04X.\n",retval);

                if (API_SUCCESS == retval)
                {
                    retval = BT_smp_set_oob_data_pl (&bd_addr, (UCHAR)read, &oob_val, NULL, 0);

                    IotLogDebug("Peer OOB Data Setting returned 0x%04X.\n",retval);
                }

#else /* SMP_HAVE_OOB_SUPPORT */
                IotLogDebug ("'SMP_HAVE_OOB_SUPPORT' flag not defined\n");
#endif /* SMP_HAVE_OOB_SUPPORT */

                break;

            case 7:
                IotLogDebug("Enter Encryption Key Size (7 < keysize < 16): \n");
                CONSOLE_IN("%u", &read);

                BT_smp_set_max_encryption_key_size_pl ((UCHAR)read);
                break;

            case 8:
                IotLogDebug("Enter 1 octet Key Distribution flag (in HEX): \n");
                IotLogDebug(" BIT 0-Local Encryption Info\n");
                IotLogDebug(" BIT 1-Local Identity Info\n");
                IotLogDebug(" BIT 2-Local Signing Info\n");
                IotLogDebug(" BIT 4-Remote Encryption Info\n");
                IotLogDebug(" BIT 5-Remote Identity Info\n");
                IotLogDebug(" BIT 6-Remote Signing Info\n");
                CONSOLE_IN("%x", &read);

                BT_smp_set_key_distribution_flag_pl ((UCHAR)read);
                break;

            case 9:
#ifdef SMP_LESC
                IotLogDebug("Enter Platform KeyPress Notification Support\n");
                IotLogDebug("(0-RESET, 1-SET): ");
                CONSOLE_IN("%u", &read);

                BT_smp_set_key_press_ntf_pl ((UCHAR)read);
#else /* SMP_LESC */
                IotLogDebug("\nSMP LE Secure Connections not enabled!!!\n");
#endif /* SMP_LESC */
                break;

            case 10:
                IotLogDebug("Enter BD_ADDR: ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                retval = BT_smp_mark_device_untrusted_pl(&bd_handle);

                IotLogDebug("Made Device Untrusted with result 0x%04X",retval);

                break;

            case 15:
#ifdef SMP_LESC
                IotLogDebug("\nGenerate SMP LESC OOB Data from PL...\n");
                BT_smp_generate_lesc_oob_local_data_pl(appl_smp_lesc_oob_data_complete);
#else /* SMP_LESC */
#endif /* SMP_LESC */
                break;

            case 16:
#ifdef SMP_LESC
#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                IotLogDebug("Enter the key to be Generated ...\n");
                IotLogDebug("\t0. LTK\n\t1. LK\n");

                IotLogDebug("Generate SMP LESC LK ...\n");

                CONSOLE_IN("%u", &read);

                if (read)
                {
                    UCHAR key[SMP_LTK_SIZE];
                    UCHAR ct2;

                    IotLogDebug ("Enter %d octet of LTK (in HEX): ", SMP_LTK_SIZE);
                    for (i = 0; i < SMP_LTK_SIZE; i++)
                    {
                        CONSOLE_IN ("%x", &choice);
                        key[i] = (UCHAR)choice;
                    }

                    IotLogDebug("Enter the CT2: ");
                    CONSOLE_IN("%x", &choice);
                    ct2 = (UCHAR)choice;

                    IotLogDebug("Generate SMP LESC LK ...\n");
                    BT_smp_get_lk_from_ltk_pl(key,appl_smp_lesc_txp_key_gen_complete,ct2);
                }
                else
                {
                    UCHAR key[SMP_LK_SIZE];
                    UCHAR ct2;

                    IotLogDebug ("Enter %d octet of LK (in HEX): ", SMP_LK_SIZE);
                    for (i = 0; i < SMP_LK_SIZE; i++)
                    {
                        CONSOLE_IN ("%x", &choice);
                        key[i] = (UCHAR)choice;
                    }

                    IotLogDebug("Enter the CT2: ");
                    CONSOLE_IN("%x", &choice);
                    ct2 = (UCHAR)choice;

                    IotLogDebug("Generate SMP LESC LTK ...\n");
                    BT_smp_get_ltk_from_lk_pl(key,appl_smp_lesc_txp_key_gen_complete,ct2);
                }
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */
                IotLogDebug("SMP_LESC_CROSS_TXP_KEY_GEN not enabled");
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#else /* SMP_LESC */
                IotLogDebug("SMP_LESC_CROSS_TXP_KEY_GEN not enabled");
#endif /* SMP_LESC */
                break;

            case 20:
                BT_smp_get_key_exchange_info_pl (&key_info);

                IotLogDebug ("Encryption Info:\n");
                appl_dump_bytes(key_info->enc_info, sizeof (key_info->enc_info));
                IotLogDebug ("Master Identification Info:\n");
                appl_dump_bytes(key_info->mid_info, sizeof (key_info->mid_info));
                IotLogDebug ("Identity Info:\n");
                appl_dump_bytes(key_info->id_info, sizeof (key_info->id_info));
                IotLogDebug ("Identity Address Info:\n");
                appl_dump_bytes(key_info->id_addr_info, sizeof (key_info->id_addr_info));
                IotLogDebug ("Signature Info:\n");
                appl_dump_bytes(key_info->sign_info, sizeof (key_info->sign_info));

                break;

            case 21:
#ifdef SMP_PL_ENABLE_KEY_GENERATION
                IotLogDebug ("Enter Diversifier value: ");
                CONSOLE_IN("%d", &choice);
                BT_smp_generate_keys_pl((UINT16)choice);
#else /* SMP_PL_ENABLE_KEY_GENERATION */
                IotLogDebug ("SMP_PL_ENABLE_KEY_GENERATION not defined\n");
#endif /* SMP_PL_ENABLE_KEY_GENERATION */
                break;

            case 30:
#ifdef SMP_HAVE_OOB_SUPPORT
                IotLogDebug ("Enter peer BD Address: ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;

                retval = BT_smp_add_device_pl (&bd_addr);
                IotLogDebug ("Add device - 0x%04X\n", retval);
#else /* SMP_HAVE_OOB_SUPPORT */
                IotLogDebug ("'SMP_HAVE_OOB_SUPPORT' flag not defined\n");
#endif /* SMP_HAVE_OOB_SUPPORT */
                break;

            case 31:
#ifdef SMP_HAVE_OOB_SUPPORT
                IotLogDebug ("Enter peer BD Address: ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;

                retval = BT_smp_remove_device_pl (&bd_addr);
                IotLogDebug ("Remove device - 0x%04X\n", retval);
#else /* SMP_HAVE_OOB_SUPPORT */
                IotLogDebug ("'SMP_HAVE_OOB_SUPPORT' flag not defined\n");
#endif /* SMP_HAVE_OOB_SUPPORT */
                break;

            case 40:
                {
                    UCHAR mode;

                    IotLogDebug("\nEnter the Platform Mode for Passkey Updation : ");
                    IotLogDebug("\n 1 - Use Modulo Operations");
                    IotLogDebug("\n 2 - Use Masking Operations");
                    IotLogDebug("\nYour Choice?");
                    CONSOLE_IN("%u", &read);
                    mode = (UCHAR) read;

                    BT_smp_set_passkey_revamp_mode_pl(mode);
                }
                break;

            case 41:
            #ifdef SMP_LESC
                {
                    IotLogDebug("\nEnter the desired Pairing Mode for Platform : ");
                    IotLogDebug("\n 1 - Use SMP Legacy Mode");
                    IotLogDebug("\n 2 - Use SMP LE Secure Connections Mode");
                    IotLogDebug("\nYour Choice?");
                    CONSOLE_IN("%u", &read);

                    g_appl_pair_mode = (0x01 == (UCHAR) read) ? SMP_LEGACY_MODE : SMP_LESC_MODE;
                }
            #else
                IotLogDebug("\nSMP_LESC feature flag is not defined.\n");
            #endif /* SMP_LESC */
                break;

            case 50:
#ifdef APPL_SMP_SUPPORT_REVERSE_INPUT_STRING
                {
                    UCHAR test_byte[100];
                    UCHAR op[16];
                    UINT16 len;

                    BT_mem_set(test_byte, 0x0, sizeof(test_byte));

                    IotLogDebug("\nEnter the octet stream to be reversed (in HEX) : \n");
                    /**
                     * Typically, this needs to be fscaf from STDIN.
                     * To remove warnings its changed to CONSOLE_IN and the
                     * entire block is protected by a flag.
                     */
                    /* fscanf(stdin, "%99[^\n]%*c", test_byte); */
                    CONSOLE_IN("%99[^\n]%*c", test_byte);

                    len = (UINT16)BT_str_len(test_byte);

                    IotLogDebug ("Total Length of Data Entered is: %d", len);

                    IotLogDebug("\nEntered value in reverse is: ");
                    appl_smp_strtoarray
                    (
                        test_byte,
                        len,
                        &op[0],
                        16
                    );

                    for (i = 0; i < 16; i++)
                    {
                        IotLogDebug("%02X ", op[(16 - 1) - i]);
                    }

                    IotLogDebug("\n\n");
                }
#else /* APPL_SMP_SUPPORT_REVERSE_INPUT_STRING */
                IotLogDebug("APPL_SMP_SUPPORT_REVERSE_INPUT_STRING feature flag is not defined.\n");
#endif /* APPL_SMP_SUPPORT_REVERSE_INPUT_STRING */
                break;

            default:
                break;
        }
    }
}
#endif

#if 0
void main_smp_operations(void)
{
    unsigned int  choice;
    unsigned int  read;

    UINT32        read_passkey;
    API_RESULT    retval;

    SMP_KEY_DIST  * key_info;
    SMP_BD_ADDR   bd_addr;
#ifdef APPL_SMP_PARALLEL_AUTH
    SMP_BD_ADDR   bd_addr_1;
#endif /* APPL_SMP_PARALLEL_AUTH */
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth;

    UCHAR         accept, i;
    UCHAR         irk[SMP_IRK_SIZE];

#ifdef SMP_HAVE_ENCRYPT_API
    UINT16        ediv;
    UCHAR         rnd[SMP_RAND_64B_SIZE];
    UCHAR         ltk[SMP_LTK_SIZE];
#endif /* SMP_HAVE_ENCRYPT_API */

#ifdef SMP_LESC
    UCHAR ntf_val;
#endif /* SMP_LESC */

    BT_LOOP_FOREVER()
    {
        retval = API_SUCCESS;

        IotLogDebug("%s", appl_smp_menu);
        CONSOLE_IN("%u", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                IotLogDebug("Refreshing...\n");
                break;

            case 2:
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("\n0 - Public\n1 - Random\n");
                IotLogDebug("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

#ifdef APPL_SMP_PARALLEL_AUTH
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr_1.addr);

                IotLogDebug("\n0 - Public\n1 - Random\n");
                IotLogDebug("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read);
                bd_addr_1.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr_1,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */
#endif /* APPL_SMP_PARALLEL_AUTH */

#ifdef HAVE_SMP_REPEAT_TRACKING
                /* Check if repeat attempt tracking is on */
                if (BT_TIMER_HANDLE_INIT_VAL != smp_repeat_timer)
                {
                    BT_stop_timer (smp_repeat_timer);
                    smp_repeat_timer = BT_TIMER_HANDLE_INIT_VAL;
                }
#endif /* HAVE_SMP_REPEAT_TRACKING */

#ifdef SMP_LESC
                IotLogDebug("0 - Legacy LE SMP Procedure\n");
                IotLogDebug("1 - LE Secure Connections Mode\n");
                IotLogDebug("Enter SMP Pairing Mode :");
                CONSOLE_IN("%u", &read);

                /* Call SMP LE Secure Connetions Mode Setting API here */
                auth.pair_mode = (UCHAR)((read)?SMP_LESC_MODE:SMP_LEGACY_MODE);
#endif /* SMP_LESC */

                IotLogDebug(" 0 - Encryption Only/Unauthenticated (Without MITM)\n");
                IotLogDebug(" 1 - Authenticated (With MITM)\n");
                IotLogDebug("Enter Security level required : ");
                CONSOLE_IN("%u", &read);
                auth.security = (UCHAR) ((read)?SMP_SEC_LEVEL_2: SMP_SEC_LEVEL_1);

                IotLogDebug("\n0 - non-Bonding\n1 - Bonding\n");
                IotLogDebug("Enter Bonding type : ");
                CONSOLE_IN("%u", &read);
                auth.bonding = (UCHAR) read;

                IotLogDebug("Enter Encryption Key size required : ");
                CONSOLE_IN("%u", &read);
                auth.ekey_size = (UCHAR)read;

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                if (BT_TRUE == appl_smp_xtxp_enable)
                {
                    IotLogDebug("Enter transport (1-BREDR, 2-BLE): ");
                    CONSOLE_IN("%u", &read);
                    auth.transport = (UCHAR)read;

                    IotLogDebug("Enter Cross Transport KeyGen (0-No, 1-Yes): ");
                    CONSOLE_IN("%u", &read);
                    auth.xtx_info = (read) ? SMP_XTX_KEYGEN_MASK : SMP_XTX_DISABLE;
                    auth.xtx_info |= SMP_XTX_H7_MASK;
                }
                else
                {
                    auth.transport = 2;
                    auth.xtx_info = 0;
                }

                /* Get the BD handle */
                BT_smp_get_link_handle(&bd_addr, &bd_handle, auth.transport);
#else /* SMP_LESC_CROSS_TXP_KEY_GEN */

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */

                retval = BT_smp_authenticate
                         (
                             &bd_handle,
                             &auth
                         );

                IotLogDebug ("BT_smp_authenticate, Retval - 0x%04X\n", retval);

#ifdef APPL_SMP_PARALLEL_AUTH
                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr_1, &bd_handle);

                retval = BT_smp_authenticate
                         (
                             &bd_handle,
                             &auth
                         );

                IotLogDebug ("BT_smp_authenticate, Retval - 0x%04X\n", retval);
#endif /* APPL_SMP_PARALLEL_AUTH */
                break;

            case 3:
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("\n0 - Public\n1 - Random\n");
                IotLogDebug("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

#ifdef SMP_LESC
                IotLogDebug("0 - Legacy LE SMP Procedure\n");
                IotLogDebug("1 - LE Secure Connections Mode\n");
                IotLogDebug("Enter SMP Pairing Mode :");
                CONSOLE_IN("%u", &read);

                /* Call SMP LE Secure Connetions Mode Setting API here */
                auth.pair_mode = (UCHAR)((read)?SMP_LESC_MODE:SMP_LEGACY_MODE);
#endif /* SMP_LESC */

                IotLogDebug(" 0 - Encryption Only/Unauthenticated (Without MITM)\n");
                IotLogDebug(" 1 - Authenticated (With MITM)\n");
                IotLogDebug("Enter Security level required : ");
                CONSOLE_IN("%u", &read);
                auth.security = (UCHAR) ((read)?SMP_SEC_LEVEL_2: SMP_SEC_LEVEL_1);

                IotLogDebug("\n0 - non-Bonding\n1 - Bonding\n");
                IotLogDebug("Enter Bonding type : ");
                CONSOLE_IN("%u", &read);
                auth.bonding = (UCHAR) read;

                IotLogDebug("Enter Authentication (0 - Accept/ErrorCode - Reject)  : ");
                CONSOLE_IN("%x", &read);
                auth.param = (UCHAR)read;

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                retval = BT_smp_authentication_request_reply
                         (
                             &bd_handle,
                             &auth
                         );
                break;

            case 5:
#ifdef SMP_HAVE_ENCRYPT_API
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("\n0 - Public\n1 - Random\n");
                IotLogDebug("Enter BD_ADDR type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                IotLogDebug("Enter EDIV (UINT16 in HEX): ");
                CONSOLE_IN("%x", &read);
                ediv = read;

                IotLogDebug("Enter RAND (8 octet in HEX): ");
                for (i = 0; i < SMP_RAND_64B_SIZE; i++)
                {
                    CONSOLE_IN("%x", &read);
                    rnd[i] = read;
                }

                IotLogDebug("Enter LTK (16 octet in HEX): ");
                for (i = 0; i < SMP_LTK_SIZE; i++)
                {
                    CONSOLE_IN("%x", &read);
                    ltk[i] = read;
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
                IotLogDebug("BT_smp_encrypt retval - 0x%04X\n", retval);
#else /* SMP_HAVE_ENCRYPT_API */
                IotLogDebug("SMP_HAVE_ENCRYPT_API not defined\n");
#endif /* SMP_HAVE_ENCRYPT_API */
                break;

            /* User Pass Key Request Reply */
            case 10:
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter BD_ADDR type (0: Public, 1: Random): ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                IotLogDebug("1 - Accept/<ERROR> - Reject)  : ");
                CONSOLE_IN("%u", &read);
                accept = (UCHAR)read;

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);

#ifdef SMP_LESC
                ntf_val = SMP_LESC_PASSKEY_ENTRY_STARTED;
                /* Send KeyPress Notification */
                BT_smp_keypress_notification(&bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */

                if (accept)
                {
                    IotLogDebug("Enter Passkey : ");
                    CONSOLE_IN("%u", &read_passkey);
                }

#ifdef SMP_LESC
                ntf_val = SMP_LESC_PASSKEY_ENTRY_COMPLETED;
                /* Send KeyPress Notification */
                BT_smp_keypress_notification(&bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */

                retval = BT_smp_passkey_entry_request_reply
                         (
                             &bd_handle,
                             &read_passkey,
                             accept
                         );
                break;

            /* Numeric Comparison Confirmation Reply */
            case 11:
#ifdef SMP_LESC
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter BD_ADDR type (0: Public, 1: Random): ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
                BT_COPY_BD_ADDR_AND_TYPE(&bd_addr,&g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

                IotLogDebug("1 - Accept/<ERROR> - Reject)  : ");
                CONSOLE_IN("%u", &read);
                accept = (1 == read) ?
                         SMP_NUM_COMP_CNF_POSITIVE:
                         SMP_NUM_COMP_CNF_NEGATIVE;

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                retval = BT_smp_nkey_comp_cnf_reply
                         (
                             &bd_handle,
                             (void *)&accept
                         );
#else /* SMP_LESC */
                IotLogDebug("SMP_LESC Compilation Flag not defined!!!\n");
#endif /* SMP_LESC */
                break;

            /* Register callback */
            case 20:
                retval = BT_smp_register_user_interface(appl_smp_cb);
                break;

            case 21:
                IotLogDebug("Enter mode (0: Non-automatic, 1: Automatic) : ");
                CONSOLE_IN("%d", &read);
                automatic_mode = (UCHAR)read;
                break;

            case 22:
                IotLogDebug("Enter Pairable Mode (1/0): ");
                CONSOLE_IN("%d", &read);
                pairable_mode = (UCHAR)read;
                break;

            case 30:
                BT_smp_get_key_exchange_info_pl (&key_info);
                retval = BT_smp_create_resolvable_pvt_addr(key_info->id_info);
                break;

            case 31:
                IotLogDebug("Enter BD_ADDR: ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter IRK (16 octets): ");
                for (i = 0; i < SMP_IRK_SIZE; i++)
                {
                    CONSOLE_IN ("%x", &choice);
                    irk[i] = (UCHAR)choice;
                }

                retval = BT_smp_verify_resolvable_pvt_addr(bd_addr.addr, irk);
                break;

            case 40:
                IotLogDebug("Enter BD_ADDR : ");
                appl_get_bd_addr(bd_addr.addr);

                IotLogDebug("Enter bd_addr type : ");
                CONSOLE_IN("%u", &read);
                bd_addr.type = (UCHAR) read;

                /* Get the BD handle */
                BT_smp_get_bd_handle (&bd_addr, &bd_handle);

                retval = BT_smp_get_device_security_info
                         (
                             &bd_handle,
                             &auth
                         );
                if (API_SUCCESS == retval)
                {
                    IotLogDebug ("Security - 0x%02X\n", auth.security);
                    IotLogDebug ("Bonding - 0x%02X\n", auth.bonding);
                    IotLogDebug ("Encryption Key Size  - 0x%02X\n", auth.ekey_size);
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

                    IotLogDebug ("Enter data length: ");
                    CONSOLE_IN("%d", &choice);
                    datalen = (UINT16) choice;
                    data = (UCHAR *) BT_alloc_mem (datalen);

                    if (NULL != data)
                    {
                        IotLogDebug ("Enter the data (In Hex): ");
                        for (i = 0; i < datalen; i++)
                        {
                            CONSOLE_IN("%x", &choice);
                            data[i] = (UCHAR)choice;
                        }
                    }

                    IotLogDebug ("Signing Counter: %d", sign_create_counter);

                    /* Call to sign the data */
                    appl_smp_generate_sign_data(data, datalen);

                    BT_free_mem (data);
                }

#else /* SMP_DATA_SIGNING */
                IotLogDebug ("SMP_DATA_SIGNING not defined\n");
#endif /* SMP_DATA_SIGNING */
                break;

            case 61:
#ifdef SMP_DATA_SIGNING
                {
                    UCHAR * data;
                    UINT16 datalen;

                    IotLogDebug ("Enter signed data length: ");
                    CONSOLE_IN("%d", &choice);
                    datalen = (UINT16) choice;
                    data = (UCHAR *) BT_alloc_mem (datalen);

                    if (NULL != data)
                    {
                        IotLogDebug ("Enter the data (In Hex): ");
                        for (i = 0; i < datalen; i++)
                        {
                            CONSOLE_IN("%x", &choice);
                            data[i] = (UCHAR)choice;
                        }
                    }

                    IotLogDebug ("Signing Counter: %d", sign_verify_counter);

                    /* Call to sign the data */
                    appl_smp_verify_sign_data(data, datalen);

                    BT_free_mem (data);
                }

#else /* SMP_DATA_SIGNING */
                IotLogDebug ("SMP_DATA_SIGNING not defined\n");
#endif /* SMP_DATA_SIGNING */
                break;

            case 70:
#ifdef AES_CMAC_UTIL
                 /* aes_cmac_test(); */
#else /* AES_CMAC_UTIL */
                IotLogDebug ("AES_CMAC_UTIL not defined\n");
#endif /* AES_CMAC_UTIL */
                break;

            case 71:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_f4();
#else
                IotLogDebug ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 72:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_f5();
#else
                IotLogDebug ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 73:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_f6();
#else
                IotLogDebug ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 74:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_g2();
#else
                IotLogDebug ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 75:
#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
                appl_smp_test_h6();
#else
                IotLogDebug ("SMP_TBX_TEST_LESC_FUNCTIONS not defined\n");
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
                break;

            case 100:
                IotLogDebug("Enable cross transport inputs (1/0): ");
                CONSOLE_IN("%d", &choice);
                appl_smp_xtxp_enable = (UCHAR)choice;

                BT_IGNORE_UNUSED_PARAM(appl_smp_xtxp_enable);
                break;

            case 200:
                {
                    UINT16         conn_handle;
#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
                    IotLogDebug("Enter BD_ADDR : ");
                    appl_get_bd_addr(bd_addr.addr);

                    IotLogDebug("Enter BD_ADDR type (0: Public, 1: Random): ");
                    CONSOLE_IN("%u", &read);
                    bd_addr.type = (UCHAR) read;
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
                        retval = BT_hci_disconnect(conn_handle, 0x13);

                        if (API_SUCCESS != retval)
                        {
                            IotLogError(
                            "Failed to Disconnect connection with %02X:%02X:%02X:%02X::%02X"
                            ":%02X, reason 0x%04X\n", bd_addr.addr[5],
                            bd_addr.addr[4], bd_addr.addr[3], bd_addr.addr[2],
                            bd_addr.addr[1], bd_addr.addr[0], retval);
                        }
                    }
                }
                break;

            default:
                IotLogDebug("Invalid Choice try again..\n");
                break;
        }

        if(API_SUCCESS != retval)
        {
            IotLogDebug("FAILED !!! retval = 0x%04X\n", retval);
        }
        else
        {
            IotLogDebug("Success !!!\n");
        }
    }
}
#endif

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
        BT_smp_get_key_exchange_info_pl (&key_info);
        sign_key = key_info->sign_info;

        /* Call to sign the data */
        BT_smp_sign_data(sign_data, sign_datalen, sign_key, sign_mac);
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
        BT_smp_verify_sign_data (sign_data, sign_datalen, sign_key, sign_mac);
    }
}
#endif /* SMP_DATA_SIGNING */

void appl_smp_reset (void)
{
#ifdef SMP_DATA_SIGNING
    sign_create_counter = 0;
    sign_verify_counter = 0;
#endif /* SMP_DATA_SIGNING */
}

void appl_smp_register (void)
{
    BT_smp_register_user_interface (appl_smp_cb);
}

void appl_smp_configure_platform (void)
{
    SMP_BD_ADDR l_addr;

    /* Update local SMP capabilities using smp_pl APIs if required here */
    BT_smp_set_io_cap_pl(SMP_IO_CAPABILITY_DISPLAY_YESNO);

    /* Setting the local Public Address as Identity Address */
    BT_hci_get_local_bd_addr
    (
        l_addr.addr
    );
    l_addr.type = BT_BD_PUBLIC_ADDRESS_TYPE;

    BT_smp_set_local_identity_addr(&l_addr);
}


API_RESULT appl_smp_assert_security
           (
               SMP_BD_HANDLE * handle,
               UCHAR           security,
               UCHAR           ekey_size
           )
{
    API_RESULT retval;

#ifdef APPL_PERIPHERAL_INITIATE_SECURITY_REQUEST
    SMP_AUTH_INFO  auth;
#ifdef APPL_FSM_SUPPORT
    APPL_EVENT_PARAM    fsm_param;
#endif /* APPL_FSM_SUPPORT */
    APPL_HANDLE    appl_handle;
#endif /* APPL_PERIPHERAL_INITIATE_SECURITY_REQUEST */

    /* Verify if security requirements are available with the link */
    retval = appl_smp_check_security_access
             (
                 handle,
                 (security & SMP_SEC_LEVEL_MASK),
                 ekey_size
             );

    /* Security requirements not satisfied */
    if (API_SUCCESS != retval)
    {
#ifdef APPL_PERIPHERAL_INITIATE_SECURITY_REQUEST
        APPL_EVENT_PARAM_INIT(&fsm_param);

        /* Get required security for service */
        auth.security = security;
        auth.ekey_size = ekey_size;

        /*
         * Set bond requirement
         *
         * TODO: Take this from some application configuration?
         */
        auth.bonding = SMP_BONDING;

        appl_get_handle_from_device_handle (handle.device_id,&appl_handle);

#ifdef APPL_FSM_SUPPORT
        fsm_param.handle = appl_handle;
        fsm_param.data = &auth;
        fsm_param.data_len = sizeof (SMP_AUTH_INFO);
        fsm_param.direction = 0x00;

        /* Post Transport Configuration Request for SMP */
        appl_fsm_post_event
        (
             ev_appl_transport_configuration_req,
             (void *)(&fsm_param)
        );
#else /* APPL_FSM_SUPPORT */
#endif /* APPL_FSM_SUPPORT */
#endif /* APPL_PERIPHERAL_INITIATE_SECURITY_REQUEST */
    }

    return retval;
}


API_RESULT appl_smp_check_security_access
          (
              SMP_BD_HANDLE * handle,
              UCHAR sec_level,
              UCHAR ekey_size
          )
{
    SMP_AUTH_INFO info;
    API_RESULT retval;

    /* TODO: Have proper application error codes */

    if (0x00 == sec_level)
    {
        return API_SUCCESS;
    }

    /* Get current security state for the link */
    retval = BT_smp_get_device_security_info (handle, &info);

    if (API_SUCCESS == retval)
    {
        /* Check if the security requirement is sufficient */
        if ((info.param != SMP_ENTITY_AUTH_ON) ||
            (info.security < (sec_level & SMP_SEC_LEVEL_MASK)) ||
            (info.ekey_size < ekey_size))
        {
            return API_FAILURE;
        }

        return API_SUCCESS;
    }

    return API_FAILURE;
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
    unsigned int  read;
    SMP_BD_ADDR   bd_addr;
    SMP_BD_HANDLE bd_handle;
    SMP_AUTH_INFO auth;
    API_RESULT    retval;

    retval = API_FAILURE;

#ifndef APPL_SMP_AUTO_FILL_BD_ADDR
    IotLogDebug("Enter BD_ADDR : ");
    appl_get_bd_addr(bd_addr.addr);

    IotLogDebug("\n0 - Public\n1 - Random\n");
    IotLogDebug("Enter BD_ADDR type : ");
    CONSOLE_IN("%u", &read);
    bd_addr.type = (UCHAR) read;
#else /* APPL_SMP_AUTO_FILL_BD_ADDR */
    BT_COPY_BD_ADDR_AND_TYPE(&bd_addr, &g_bd_addr);
#endif /* APPL_SMP_AUTO_FILL_BD_ADDR */

#ifdef SMP_LESC
    IotLogDebug("0 - Legacy LE SMP Procedure\n");
    IotLogDebug("1 - LE Secure Connections Mode\n");
    IotLogDebug("Enter SMP Pairing Mode :");
    CONSOLE_IN("%u", &read);

    /* Call SMP LE Secure Connetions Mode Setting API here */
    auth.pair_mode = (UCHAR)((read)? SMP_LESC_MODE : SMP_LEGACY_MODE);
#endif /* SMP_LESC */

    IotLogDebug(" 0 - Encryption Only/Unauthenticated (Without MITM)\n");
    IotLogDebug(" 1 - Authenticated (With MITM)\n");
    IotLogDebug("Enter Security level required : ");
    CONSOLE_IN("%u", &read);
    auth.security = (UCHAR) ((read)? SMP_SEC_LEVEL_2 : SMP_SEC_LEVEL_1);

    IotLogDebug("\n0 - non-Bonding\n1 - Bonding\n");
    IotLogDebug("Enter Bonding type : ");
    CONSOLE_IN("%u", &read);
    auth.bonding = (UCHAR) read;

#ifndef SMP_STORAGE
     /* If storage not supported, make no-bonding */
     auth->bonding = SMP_BONDING_NONE;
#endif /* SMP_STORAGE */

    IotLogDebug("Enter Encryption Key size required : ");
    CONSOLE_IN("%u", &read);
    auth.ekey_size = (UCHAR)read;

    /* Get the BD handle */
    BT_smp_get_bd_handle (&bd_addr, &bd_handle);

    retval = BT_smp_authenticate
             (
                 &bd_handle,
                 &auth
             );

    IotLogDebug ("BT_smp_authenticate, Retval - 0x%04X\n", retval);

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
    SMP_KEY_DIST local_key_info;
    SMP_AUTH_INFO * auth;
    SMP_AUTH_INFO info;
    SMP_BD_ADDR bdaddr;
    SMP_KEY_XCHG_PARAM * kx_param;
    APPL_HANDLE   appl_handle;

    UCHAR accept;

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

    /* Get the BD Address from handle */
    BT_smp_get_bd_addr (bd_handle, &bdaddr);

#ifndef DONT_USE_STANDARD_IO
    bd_addr = bdaddr.addr;
    bd_addr_type = bdaddr.type;
    (void)bd_addr;         /*fix build warning: set but never used.*/
    (void)bd_addr_type;    /*fix build warning: set but never used.*/
#endif /* DONT_USE_STANDARD_IO */
    switch(event)
    {
    case SMP_AUTHENTICATION_COMPLETE:
        IotLogDebug("\nRecvd SMP_AUTHENTICATION_COMPLETE\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");
        IotLogDebug("Status : %04X\n", status);

        appl_get_handle_from_device_handle (*bd_handle,&appl_handle);

#ifdef APPL_FSM_SUPPORT
        fsm_param.handle = appl_handle;
        fsm_param.data_param = NULL;
        fsm_param.data_len = 0;
        fsm_param.direction = 0xFF;
#endif /* APPL_FSM_SUPPORT */

        /* The direction member of FSM Event holds the status of Authentication Procedure here */
        if (NULL != event_data)
        {
            if(API_SUCCESS == status)
            {
                auth = (SMP_AUTH_INFO *)event_data;

                IotLogDebug("Authentication type : %s\n",
                (SMP_SEC_LEVEL_2 == (auth->security & 0x0F))?  "With MITM":
                "Encryption Only (without MITM)");

#ifdef SMP_LESC
                IotLogDebug("Pairing Mode : %s\n",
                (SMP_LESC_MODE == (auth->pair_mode))? "LE SEC Pairing Mode":
                "LEGACY Pairing Mode");
#endif /* SMP_LESC */

                IotLogDebug("Bonding type : %s\n",
                (auth->bonding)? "Bonding": "Non-Bonding");

                IotLogDebug("Encryption Key size : %d\n", auth->ekey_size);

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
                IotLogDebug("Transport : %d\n", auth->transport);
                IotLogDebug("Cross Transport info: %d\n", auth->xtx_info);

                if (SMP_XTX_KEYGEN_MASK & auth->xtx_info)
                {
                    /* Save the BD Address */
                    BT_COPY_BD_ADDR_AND_TYPE(&appl_smp_bd_addr, &bdaddr);

                    /* TODO: Compare key strengths before generating */

                    if (SMP_LINK_BREDR == auth->transport)
                    {
                        /* Get the BREDR link key for the device */
                        retval = BT_sm_get_device_link_key(bd_addr, link_key);
                        if (API_SUCCESS != retval)
                        {
                            printf("FAILED ! Reason = 0x%04X\n", retval);
                            break;
                        }
                        else
                        {
                            BT_smp_get_ltk_from_lk_pl
                            (
                                link_key,
                                appl_smp_lesc_xtxp_ltk_complete,
                                (auth->xtx_info & SMP_XTX_H7_MASK)
                            );
                        }
                    }
                    else
                    {
                        retval = BT_smp_get_device_keys
                                 (
                                     bd_handle,
                                     &p_keys,
                                     &p_key_info
                                 );

                        if (API_SUCCESS != retval)
                        {
                            IotLogError("Failed to get Peer Device Keys!!\n");
                        }
                        else
                        {
                            BT_smp_get_lk_from_ltk_pl
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
                if (auth->bonding)
                {
                    SMP_BD_ADDR t_bd_addr;

                    /* check if the device has shared the keys */
                    /* Get platform data of key informations */
                    BT_smp_get_key_exchange_info_pl (&key_info);

                    BT_COPY_BD_ADDR(&t_bd_addr.addr, &peer_key_info.id_addr_info[1]);
                    BT_COPY_TYPE(t_bd_addr.type, peer_key_info.id_addr_info[0]);

                    retval = BT_hci_le_add_device_to_resolving_list
                             (
                                 t_bd_addr.type,
                                 &t_bd_addr.addr,
                                 peer_key_info.id_info,
                                 key_info->id_info
                             );

                    if(API_SUCCESS != retval)
                    {
                        IotLogDebug("FAILED !!! Error code = %04X\n", retval);
                    }
                    else
                    {
                        IotLogDebug("Added Device with Identity Address: "
                        BT_DEVICE_ADDR_FRMT_SPECIFIER " to Resolving List!\n",
                        BT_DEVICE_ADDR_PRINT_STR(&t_bd_addr));

                        retval = BT_hci_le_set_privacy_mode
                                 (
                                     peer_identity_addr_type,
                                     peer_identity_addr,
                                     0x01 /* Device Privacy Mode */
                                 );

                        IotLogDebug("Setting Device Privacy Mode returned with "
                        "0x%04X\n", retval)

                    }
                }
#endif /* APPL_SMP_AUTO_ADD_TO_RSLV_LIST */

#ifdef APPL_FSM_SUPPORT
                fsm_param.data_param = (UCHAR *)auth;
                fsm_param.data_len = sizeof (SMP_AUTH_INFO);

                /* Update status in direction parameter */
                fsm_param.direction = 0x00;
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
                        NULL, 0
                    );

                    IotLogDebug ("Repeat Tracking Timer Started for %d seconds.\n",
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
                IotLogDebug("\nConfirmed Authentication using Encryption\n");

#ifdef APPL_FSM_SUPPORT
                /* Update status in direction parameter */
                fsm_param.direction = 0x00;
#endif /* APPL_FSM_SUPPORT */
            }
            else if (SMP_REMOTE_SIDE_PIN_KEY_MISSING == status)
            {
                IotLogDebug("\nPeer Device Lost previous Bonding Information!\n");
                IotLogDebug("Deleting Local Bond Information of Peer...\n");

                retval = BT_smp_mark_device_untrusted_pl(bd_handle);

                IotLogDebug("Marked Device Untrusted with result 0x%04X\n",retval);

                if (API_SUCCESS == retval)
                {
                    IotLogDebug("Initiate Pairing Again...\n");
                }
            }
        }

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

        break;

    case SMP_AUTHENTICATION_REQUEST:
        IotLogDebug("\nRecvd SMP_AUTHENTICATION_REQUEST\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        auth = (SMP_AUTH_INFO *)event_data;

        IotLogDebug("Authentication type : %s\n",
        (SMP_SEC_LEVEL_2 == (auth->security & 0x0F))?  "With MITM":
        "Encryption Only (without MITM)");

        IotLogDebug("Bonding type : %s\n",
        (auth->bonding)? "Bonding": "Non-Bonding");

#ifdef SMP_LESC
        IotLogDebug("Pairing Mode : %s\n",
        (SMP_LESC_MODE == (auth->pair_mode))? "LE SEC Pairing Mode":
        "LEGACY Pairing Mode");

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
        IotLogDebug("Transport : 0x%02X\n", auth->transport);
        IotLogDebug("Cross Transport Info : 0x%02X\n", auth->xtx_info);

        /* Enable H7 support if Cross transport keygen is requested */
        if (auth->xtx_info & SMP_XTX_KEYGEN_MASK)
        {
            auth->xtx_info |= SMP_XTX_H7_MASK;
        }
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

        IotLogDebug("Encryption Key size : %d\n", auth->ekey_size);

#ifdef SMP_LESC
        /**
         * Choose the Pairing Mode based on what is configured by the
         * application.
         */
        (auth->pair_mode) = g_appl_pair_mode;
#endif /* SMP_LESC */

        /* Check if automatic mode is set */
        if(SMP_TRUE == automatic_mode)
        {
            /* Get the application handle reference */
            appl_get_handle_from_device_handle (*bd_handle,&appl_handle);

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
                    if (SMP_BONDING == info.bonding)
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
                            BT_gatt_db_peer_session_shutdown_handler(&(APPL_GET_ATT_INSTANCE(appl_handle)));
                        }
                    }
                }
            }

            /* Accept Authentication */
            auth->param = SMP_ERROR_NONE;

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
                if (SMP_LESC_MODE != auth->pair_mode)
                {
                    /* Set error if incoming request is not LESC */
                    auth->param = SMP_ERROR_AUTHENTICATION_REQUIREMENTS;
                }
                else
                {
                    if (SMP_MAX_ENCRYPTION_KEY_SIZE != auth->ekey_size)
                    {
                        /* Set error if incoming request is not with MAX key size */
                        auth->param = SMP_ERROR_ENCRYPTION_KEY_SIZE;
                    }
                }
            }
#endif /* SMP_LESC */

            IotLogDebug("\n\nSending +ve Authentication request reply.\n");
            retval = BT_smp_authentication_request_reply
                     (
                         bd_handle,
                         auth
                     );

#ifdef APPL_FSM_SUPPORT
            /* Notify application state machine */
            fsm_param.handle = appl_handle;
            fsm_param.data_param = NULL;
            fsm_param.data_len = 0;
            fsm_param.direction = 0x01;

            /* Post Transport Configuration Request for SMP */
            appl_fsm_post_event
            (
                 ev_appl_transport_configuration_req,
                 (void *)(&fsm_param)
            );
#endif /* APPL_FSM_SUPPORT */
        }
        break;

    case SMP_PASSKEY_ENTRY_REQUEST:
        IotLogDebug("\nEvent   : SMP_PASSKEY_ENTRY_REQUEST\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        if (BT_TRUE == automatic_mode)
        {
            /* Send the Passkey Entry Request Reply Automatically */
#ifdef SMP_LESC
            ntf_val = SMP_LESC_PASSKEY_ENTRY_STARTED;
            /* Send KeyPress Notification */
            BT_smp_keypress_notification(bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */

#ifdef SMP_LESC
            ntf_val = SMP_LESC_PASSKEY_ENTRY_COMPLETED;
            /* Send KeyPress Notification */
            BT_smp_keypress_notification(bd_handle,(void *)&ntf_val);
#endif /* SMP_LESC */
        }
        break;

    case SMP_PASSKEY_DISPLAY_REQUEST:
        IotLogDebug("\nEvent   : SMP_PASSKEY_DISPLAY_REQUEST\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        IotLogInfo("Passkey : %06u\n", *((UINT32 *)event_data));

#ifdef APPL_SMP_UPDATE_PASSKEY
        /* Update a 6 digit passkey - Max 999999 */
        *((UINT32 *)event_data) = APPL_SMP_PASSKEY;

        IotLogDebug("Updated Passkey: %06u", *((UINT32 *)event_data));

        retval = SMP_DISPLAY_PASSKEY_UPDATED;
#endif /* APPL_SMP_UPDATE_PASSKEY */

        break;

    case SMP_KEY_EXCHANGE_INFO_REQUEST:
        IotLogDebug("\nEvent   : SMP_KEY_EXCHANGE_INFO_REQUEST\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        /* Reference the event data */
        kx_param = (SMP_KEY_XCHG_PARAM *) event_data;

        IotLogDebug("Local keys negotiated - 0x%02X\n", kx_param->keys);
        IotLogDebug("Encryption Key Size negotiated - 0x%02X\n",
                kx_param->ekey_size);

        /* Check if automatic mode is set */
        if(SMP_TRUE == automatic_mode)
        {
            /* Get platform data of key informations */
            BT_smp_get_key_exchange_info_pl (&key_info);

            /* Copy the Local Key Info into a local struct */
            BT_mem_copy(&local_key_info,key_info,sizeof(local_key_info));

            /* Mask the to be exchanged LTK according to the negotiated key size */
            BT_mem_set
            (
                (&local_key_info.enc_info[0] + kx_param->ekey_size),
                0x00,
                (SMP_LTK_SIZE - kx_param->ekey_size)
            );

            BT_smp_key_exchange_info_request_reply (bd_handle, &local_key_info);
        }
        break;

    case SMP_LONG_TERM_KEY_REQUEST:

        /* Copy parameters to local variables */
        smp_unpack_2_byte_param(&ediv, &event_data[8]);
        peer_rand = event_data;

        IotLogDebug("\nEvent   : SMP_LONG_TERM_KEY_REQUEST\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");
        IotLogDebug("Div  : 0x%04X\n", ediv);
        IotLogDebug("Rand : %02X %02X %02X %02X %02X %02X %02X %02X\n",
        peer_rand[0], peer_rand[1], peer_rand[2], peer_rand[3],
        peer_rand[4], peer_rand[5], peer_rand[6], peer_rand[7]);

        /* Do not process if status is failre */
        if (API_SUCCESS != status)
        {
            IotLogDebug("Long Term Key request with Error - 0x%04X. Dropping.\n", status);
            break;
        }

        /* Check if automatic mode is set */
        if(SMP_TRUE == automatic_mode)
        {
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
                            IotLogError("Failed to get Peer Device Keys!!\n");
                        }
                        else
                        {
                            BT_mem_copy(ltk, p_key_info.enc_info, 16);

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
                                (ltk + info.ekey_size),
                                0x00,
                                (SMP_LTK_SIZE - info.ekey_size)
                            );
                        }
                    }

                    if (API_SUCCESS == retval)
                    {
                        IotLogDebug("\n\nSending +ve LTK request reply.\n");
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
                IotLogDebug("\n\nSending -ve LTK request reply.\n");
                ltk_null = 0;
                retval = BT_smp_long_term_key_request_reply
                         (
                             bd_handle,
                             &ltk_null,
                             SMP_FALSE
                         );
            }

            /* Notify application state machine */
            appl_get_handle_from_device_handle (*bd_handle,&appl_handle);

#ifdef APPL_FSM_SUPPORT
            fsm_param.handle = appl_handle;
            fsm_param.data_param = NULL;
            fsm_param.data_len = 0;
            fsm_param.direction = 0x01;

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
        IotLogDebug("\nEvent   : SMP_RESOLVABLE_PVT_ADDR_CREATE_CNF\n");
        IotLogDebug("Status : %04X\n", status);
        if(API_SUCCESS == status)
        {
#ifndef DONT_USE_STANDARD_IO
            bd_addr = (UCHAR *) event_data;
#endif /* DONT_USE_STANDARD_IO */

            IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
            bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],
            bd_addr[5]);
        }
        break;

    case SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF:
        IotLogDebug("\nEvent   : SMP_RESOLVABLE_PVT_ADDR_VERIFY_CNF\n");
        IotLogDebug("Status : %04X\n", status);
        if(API_SUCCESS == status)
        {
#ifndef DONT_USE_STANDARD_IO
            bd_addr = (UCHAR *) event_data;
#endif /* DONT_USE_STANDARD_IO */

            IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
            bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],
            bd_addr[5]);
        }
        break;

#ifdef SMP_DATA_SIGNING
    case SMP_DATA_SIGNING_COMPLETE:
        IotLogDebug("\nEvent   : SMP_SIGNING_DATA_COMPLETE\n");
        IotLogDebug("Status : %04X\n", status);
        if(API_SUCCESS == status)
        {
            /* Update sign counter */
            sign_create_counter ++;

            IotLogDebug("Message Authentication Code : ");
            appl_dump_bytes
            (
                event_data,
                data_len
            );

            /* Form the signature */
            BT_mem_copy
            (&signature[sizeof(sign_create_counter)], event_data, data_len);

            IotLogDebug("Message Authentication Code : ");
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
        IotLogDebug("\nEvent   : SMP_SIGN_DATA_VERIFICATION_COMPLETE\n");
        IotLogDebug("Status : %04X\n", status);

        if (API_SUCCESS == status)
        {
            /* Form the signature */
            BT_PACK_LE_4_BYTE(signature, &sign_verify_counter);
            BT_mem_copy
            (&signature[sizeof(sign_verify_counter)], event_data, data_len);

            /* Update sign counter */
            sign_verify_counter ++;

            IotLogDebug("Message Authentication Code : ");
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
        IotLogDebug ("\nRecvd SMP_KEY_EXCHANGE_INFO\n");
        IotLogDebug ("Status - 0x%04X\n", status);

        /* Reference the event data */
        kx_param = (SMP_KEY_XCHG_PARAM *) event_data;

        IotLogDebug("Remote keys negotiated - 0x%02X\n", kx_param->keys);
        IotLogDebug("Encryption Key Size negotiated - 0x%02X\n",
                kx_param->ekey_size);

        /* Reference the key information */
        key_info = kx_param->keys_info;

        /* Store the peer keys */
        BT_mem_copy (&peer_key_info, key_info, sizeof (SMP_KEY_DIST));

        IotLogDebug ("Encryption Info:\n");
        appl_dump_bytes(key_info->enc_info, sizeof (key_info->enc_info));
        IotLogDebug ("Master Identification Info:\n");
        appl_dump_bytes(key_info->mid_info, sizeof (key_info->mid_info));
        IotLogDebug ("Identity Info:\n");
        appl_dump_bytes(key_info->id_info, sizeof (key_info->id_info));
        IotLogDebug ("Identity Address Info:\n");
        appl_dump_bytes(key_info->id_addr_info, sizeof (key_info->id_addr_info));
        IotLogDebug ("Signature Info:\n");
        appl_dump_bytes(key_info->sign_info, sizeof (key_info->sign_info));

        break;

#ifdef SMP_LESC
    case SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST:
        IotLogDebug("\nEvent   : SMP_NUMERIC_KEY_COMPARISON_CNF_REQUEST\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        IotLogDebug("Numeric Code : %06u\n", (*((UINT32 *)event_data) % 1000000));

        /* If Automatic Mode , Accept the Key Comparison request */
#ifdef APPL_SMP_NC_AUTO_REPLY
        if (BT_TRUE == automatic_mode)
        {
            accept = SMP_NUM_COMP_CNF_POSITIVE;

            retval = BT_smp_nkey_comp_cnf_reply
                     (
                         bd_handle,
                         (void *)&accept
                     );
        }
#endif /* APPL_SMP_NC_AUTO_REPLY */
        break;

    case SMP_KEY_PRESS_NOTIFICATION_EVENT:
        IotLogDebug("\nEvent   : SMP_KEY_PRESS_NOTIFICATION_EVENT\n");
        IotLogDebug("BD Address : %02X %02X %02X %02X %02X %02X\n",
        bd_addr[0],bd_addr[1],bd_addr[2],bd_addr[3],bd_addr[4],bd_addr[5]);
        IotLogDebug("BD addr type : %s\n",
        (0 == bd_addr_type)? "Public Address": "Random Address");

        IotLogDebug("KeyPress Notification Value is:");

        switch(*(UCHAR *)event_data)
        {
            case SMP_LESC_PASSKEY_ENTRY_STARTED:
                IotLogDebug("SMP_LESC_PASSKEY_ENTRY_STARTED\n");
                break;
            case SMP_LESC_PASSKEY_DIGIT_ENTERED:
                IotLogDebug("SMP_LESC_PASSKEY_DIGIT_ENTERED\n");
                break;
            case SMP_LESC_PASSKEY_DIGIT_ERASED:
                IotLogDebug("SMP_LESC_PASSKEY_DIGIT_ERASED\n");
                break;
            case SMP_LESC_PASSKEY_CLEARED:
                IotLogDebug("SMP_LESC_PASSKEY_CLEARED\n");
                break;
            case SMP_LESC_PASSKEY_ENTRY_COMPLETED:
                IotLogDebug("SMP_LESC_PASSKEY_ENTRY_COMPLETED\n");
                break;
            default:
                IotLogDebug("Unknown KeyPress Value 0x%02X Received\n",*(UCHAR *)event_data);
                break;
        }
        break;
#endif /* SMP_LESC */

    default:
        IotLogDebug("\nERROR!!! Received unknown event. event = %02X\n", event);
    }

    return API_SUCCESS;
}

#ifdef HAVE_SMP_REPEAT_TRACKING
DECL_STATIC void smp_repeat_timeout_handler (void * data_param, UINT16 datalen)
{
    /* Reinitialize timer handle */
    smp_repeat_timer = BT_TIMER_HANDLE_INIT_VAL;

    IotLogDebug ("Repeat Tracking Timer expired.\n");
}
#endif /* HAVE_SMP_REPEAT_TRACKING */

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS

void appl_smp_invert_byte_stream (UCHAR *buffer, UINT16 buffer_len)
{
    UINT16 index;
    UCHAR  temp;

    for (index = 0; index < (buffer_len/2); index ++)
    {
        temp = buffer[index];
        buffer[index] = buffer[buffer_len - (index + 1)];
        buffer[buffer_len - (index + 1)] = temp;
    }
}

void appl_smp_test_f4 (void)
{
    /* This function emulates the F4 function defined in LESC Spec */
    /* U || V || Z */
    API_RESULT retval;
    UCHAR * conf_val;
    UCHAR pkax[32] = {
                         0x20, 0xB0, 0x03, 0xD2, 0xF2, 0x97, 0xBE, 0x2C,
                         0x5E, 0x2C, 0x83, 0xA7, 0xE9, 0xF9, 0xA5, 0xB9,
                         0xEF, 0xF4, 0x91, 0x11, 0xAC, 0xF4, 0xFD, 0xDB,
                         0xCC, 0x03, 0x01, 0x48, 0x0E, 0x35, 0x9D, 0xE6
                     };
    UCHAR pkbx[32] = {
                         0x55, 0x18, 0x8B, 0x3D, 0x32, 0xF6, 0xBB, 0x9A,
                         0x90, 0x0A, 0xFC, 0xFB, 0xEE, 0xD4, 0xE7, 0x2A,
                         0x59, 0xCB, 0x9A, 0xC2, 0xF1, 0x9D, 0x7C, 0xFB,
                         0x6B, 0x4F, 0xDD, 0x49, 0xF4, 0x7F, 0xC5, 0xFD
                     };
    UCHAR rand_key[16] = {
                             0xD5, 0xCB, 0x84, 0x54, 0xD1, 0x77, 0x73, 0x3E,
                             0xFF, 0xFF, 0xB2, 0xEC, 0x71, 0x2B, 0xAE, 0xAB
                         };
    UCHAR z = 0x00;
    UINT16 text_len;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (32 + 32 + 1);

    appl_smp_invert_byte_stream (pkax, sizeof(pkax));
    appl_smp_invert_byte_stream (pkbx, sizeof(pkbx));
    appl_smp_invert_byte_stream (rand_key, sizeof(rand_key));

    IotLogDebug ("smp_tbx_f4_eval()\n");
    IotLogDebug ("PKa:\n");
    appl_dump_bytes (pkax, SMP_LESC_PUBLIC_KEY_X_SIZE);
    IotLogDebug ("PKb:\n");
    appl_dump_bytes (pkbx, SMP_LESC_PUBLIC_KEY_X_SIZE);
    IotLogDebug ("Random Key:\n");
    appl_dump_bytes (rand_key, 16);
    IotLogDebug ("Z: 0x%02X\n", z);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    conf_val   = BT_alloc_mem(16);

    /* Populate Plain Text */
    plain_text[0] = z;
    BT_mem_copy(&plain_text[1],pkbx,SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_copy(&plain_text[1 + SMP_LESC_PUBLIC_KEY_X_SIZE], pkax,SMP_LESC_PUBLIC_KEY_X_SIZE);

    /* Populate Encryption Key */
    BT_mem_copy(key,rand_key,16);

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
    UCHAR N1[16] = {
                         0xD5, 0xCB, 0x84, 0x54, 0xD1, 0x77, 0x73, 0x3E,
                         0xFF, 0xFF, 0xB2, 0xEC, 0x71, 0x2B, 0xAE, 0xAB
                   };
    UCHAR N2[16] = {
                         0xA6, 0xE8, 0xE7, 0xCC, 0x25, 0xA7, 0x5F, 0x6E,
                         0x21, 0x65, 0x83, 0xF7, 0xFF, 0x3D, 0xC4, 0xCF
                   };
    UCHAR mackey[16] = {
                             0x29, 0x65, 0xF1, 0x76, 0xA1, 0x08, 0x4A, 0x02,
                             0xFD, 0x3F, 0x6A, 0x20, 0xCE, 0x63, 0x6E, 0x20
                     };
    UCHAR rand[16] = {
                             0x12, 0xA3, 0x34, 0x3B, 0xB4, 0x53, 0xBB, 0x54,
                             0x08, 0xDA, 0x42, 0xD2, 0x0C, 0x2D, 0x0F, 0xC8
                     };
    UCHAR io_cap[3] = {0x02, 0x01, 0x01};

    BT_DEVICE_ADDR l_bd_addr = { {0xCE, 0xBF, 0x37, 0x37, 0x12, 0x56}, 0x00};
    BT_DEVICE_ADDR r_bd_addr = { {0xC1, 0xCF, 0x2D, 0x70, 0x13, 0xA7}, 0x00};

    UINT16 text_len, offset;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (16 + 16 + 16 + 3 + 7 + 7);
    offset = 0;

    appl_smp_invert_byte_stream (N1, sizeof(N1));
    appl_smp_invert_byte_stream (N2, sizeof(N2));
    appl_smp_invert_byte_stream (rand, sizeof(rand));
    appl_smp_invert_byte_stream (mackey, sizeof(mackey));

    IotLogDebug ("Dumping DH key\n");
    appl_dump_bytes (mackey, 16);
    IotLogDebug("Dumping L RAND\n");
    appl_dump_bytes (N1, 16);
    IotLogDebug("Dumping R RAND\n");
    appl_dump_bytes (N2, 16);
    IotLogDebug ("Dumping RAND\n");
    appl_dump_bytes (rand, 16);
    IotLogDebug("Dumping IO CAPAB\n");
    appl_dump_bytes (io_cap, 3);
    IotLogDebug ("Dumping l_bd_addr\n");
    appl_dump_bytes (l_bd_addr.addr, 6);
    IotLogDebug ("Dumping r_bd_addr\n");
    appl_dump_bytes (r_bd_addr.addr, 6);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    enc_out   = BT_alloc_mem(16);

    /* Populate Plain Text */
    /* A2 */
    BT_COPY_BD_ADDR(&plain_text[offset],r_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],r_bd_addr.type);
    offset += 1;

    /* A1 */
    BT_COPY_BD_ADDR(&plain_text[offset],l_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;

    BT_COPY_TYPE(plain_text[offset],l_bd_addr.type);
    offset += 1;

    /* IOCap */
    BT_mem_copy(&plain_text[offset],io_cap,0x03);
    offset += 0x03;

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
    BT_mem_copy(key,mackey,16);

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
    UCHAR pkax[32] = {
                         0x20, 0xB0, 0x03, 0xD2, 0xF2, 0x97, 0xBE, 0x2C,
                         0x5E, 0x2C, 0x83, 0xA7, 0xE9, 0xF9, 0xA5, 0xB9,
                         0xEF, 0xF4, 0x91, 0x11, 0xAC, 0xF4, 0xFD, 0xDB,
                         0xCC, 0x03, 0x01, 0x48, 0x0E, 0x35, 0x9D, 0xE6
                     };
    UCHAR pkbx[32] = {
                         0x55, 0x18, 0x8B, 0x3D, 0x32, 0xF6, 0xBB, 0x9A,
                         0x90, 0x0A, 0xFC, 0xFB, 0xEE, 0xD4, 0xE7, 0x2A,
                         0x59, 0xCB, 0x9A, 0xC2, 0xF1, 0x9D, 0x7C, 0xFB,
                         0x6B, 0x4F, 0xDD, 0x49, 0xF4, 0x7F, 0xC5, 0xFD
                     };

    UCHAR lesc_g2_key[16] = {
                         0xD5, 0xCB, 0x84, 0x54, 0xD1, 0x77, 0x73, 0x3E,
                         0xFF, 0xFF, 0xB2, 0xEC, 0x71, 0x2B, 0xAE, 0xAB
                   };
    UCHAR rand[16] = {
                          0xA6, 0xE8, 0xE7, 0xCC, 0x25, 0xA7, 0x5F, 0x6E,
                          0x21, 0x65, 0x83, 0xF7, 0xFF, 0x3D, 0xC4, 0xCF
                     };

    UINT16 text_len;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (32 + 32 + 16);

    appl_smp_invert_byte_stream (pkax, sizeof(pkax));
    appl_smp_invert_byte_stream (pkbx, sizeof(pkbx));
    appl_smp_invert_byte_stream (lesc_g2_key, sizeof(lesc_g2_key));
    appl_smp_invert_byte_stream (rand, sizeof(rand));

    IotLogDebug ("smp_tbx_g2_eval()\n");
    IotLogDebug ("PKa:\n");
    appl_dump_bytes (pkax, SMP_LESC_PUBLIC_KEY_X_SIZE);
    IotLogDebug ("PKb:\n");
    appl_dump_bytes (pkbx, SMP_LESC_PUBLIC_KEY_X_SIZE);
    IotLogDebug ("Key:\n");
    appl_dump_bytes (lesc_g2_key, 16);
    IotLogDebug ("Rand:\n");
    appl_dump_bytes (rand, 16);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    enc_out   = BT_alloc_mem(16);

    /* Populate Plain Text */
    BT_mem_copy(&plain_text[0], rand, 16);
    BT_mem_copy(&plain_text[16], pkbx, SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_copy(&plain_text[16 + SMP_LESC_PUBLIC_KEY_X_SIZE], pkax,SMP_LESC_PUBLIC_KEY_X_SIZE);

    /* Populate Encryption Key */
    BT_mem_copy(key,lesc_g2_key,16);

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
    UCHAR key_id[4] = {0x72, 0x62, 0x65, 0x6C};
    UCHAR lesc_h6_key[16] = {
                                0x9B, 0x7D, 0x39, 0x0A,
                                0xA6, 0x10, 0x10, 0x34,
                                0x05, 0xAD, 0xC8, 0x57,
                                0xA3, 0x34, 0x02, 0xEC,
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

    IotLogDebug ("smp_tbx_h6_eval()\n");
    IotLogDebug ("Key_Id:\n");
    appl_dump_bytes (key_id, sizeof(key_id));
    IotLogDebug ("Key:\n");
    appl_dump_bytes (lesc_h6_key, 16);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    enc_out   = BT_alloc_mem(16);

    /* Populate Plain Text */
    BT_mem_copy(&plain_text[0], key_id, sizeof(key_id));

    /* Populate Encryption Key */
    BT_mem_copy(key,lesc_h6_key,16);

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
    UCHAR salt[16] = {
                         0xBE, 0x83, 0x60, 0x5A, 0xDB, 0x0B, 0x37, 0x60,
                         0x38, 0xA5, 0xF5, 0xAA, 0x91, 0x83, 0x88, 0x6C
                     };
    UCHAR W[32] = {
                      0xEC, 0x02, 0x34, 0xA3, 0x57, 0xC8, 0xAD, 0x05,
                      0x34, 0x10, 0x10, 0xA6, 0x0A, 0x39, 0x7D, 0x9B,
                      0x99, 0x79, 0x6B, 0x13, 0xB4, 0xF8, 0x66, 0xF1,
                      0x86, 0x8D, 0x34, 0xF3, 0x73, 0xBF, 0xA6, 0x98
                  };

    UINT16 text_len;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = sizeof(W);

    appl_smp_invert_byte_stream (W, sizeof(W));

    IotLogDebug ("smp_tbx_f5_eval()\n");
    IotLogDebug ("DHkey:\n");
    appl_dump_bytes (W, sizeof(W));
    IotLogDebug ("SALT:\n");
    appl_dump_bytes (salt, 16);

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    enc_out    = BT_alloc_mem(16);

    /* Populate Plain Text */
    BT_mem_copy(&plain_text[0], W, sizeof(W));

    /* Populate Encryption Key */
    BT_mem_copy(key,salt,16);

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
    UCHAR N1[16] = {
                         0xD5, 0xCB, 0x84, 0x54, 0xD1, 0x77, 0x73, 0x3E,
                         0xFF, 0xFF, 0xB2, 0xEC, 0x71, 0x2B, 0xAE, 0xAB
                   };
    UCHAR N2[16] = {
                         0xA6, 0xE8, 0xE7, 0xCC, 0x25, 0xA7, 0x5F, 0x6E,
                         0x21, 0x65, 0x83, 0xF7, 0xFF, 0x3D, 0xC4, 0xCF
                   };
    BT_DEVICE_ADDR l_bd_addr = { {0xCE, 0xBF, 0x37, 0x37, 0x12, 0x56}, 0x00};
    BT_DEVICE_ADDR r_bd_addr = { {0xC1, 0xCF, 0x2D, 0x70, 0x13, 0xA7}, 0x00};

    UCHAR keyID[] = {0x65, 0x6C, 0x74, 0x62};

    UINT16 text_len;
    UCHAR  offset;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (1 + 4 + 16 + 16 + 7 + 7 + 2);
    offset = 0;

    appl_smp_invert_byte_stream (N1, sizeof(N1));
    appl_smp_invert_byte_stream (N2, sizeof(N2));

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    enc_out    = BT_alloc_mem(16);

    /* Populate Plain Text */
    /* Length 256 = 0x0100 */
    plain_text[offset] = 0x00;
    offset ++;
    plain_text[offset] = 0x01;
    offset ++;

    /* A2 */
    BT_COPY_BD_ADDR(&plain_text[offset],r_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],r_bd_addr.type);
    offset += 1;

    /* A1 */
    BT_COPY_BD_ADDR(&plain_text[offset],l_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],l_bd_addr.type);
    offset += 1;

    /* N2 */
    BT_mem_copy(&plain_text[offset],N2,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* N1 */
    BT_mem_copy(&plain_text[offset],N1,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* KeyID */
    BT_mem_copy(&plain_text[offset],keyID, 4);
    offset += 4;

    /* Counter = 1 */
    plain_text[offset] = 0x01;

    /* Populate Encryption Key */
    BT_mem_copy(key,enc_key,16);

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
    UCHAR N1[16] = {
                         0xD5, 0xCB, 0x84, 0x54, 0xD1, 0x77, 0x73, 0x3E,
                         0xFF, 0xFF, 0xB2, 0xEC, 0x71, 0x2B, 0xAE, 0xAB
                   };
    UCHAR N2[16] = {
                         0xA6, 0xE8, 0xE7, 0xCC, 0x25, 0xA7, 0x5F, 0x6E,
                         0x21, 0x65, 0x83, 0xF7, 0xFF, 0x3D, 0xC4, 0xCF
                   };
    BT_DEVICE_ADDR l_bd_addr = { {0xCE, 0xBF, 0x37, 0x37, 0x12, 0x56}, 0x00};
    BT_DEVICE_ADDR r_bd_addr = { {0xC1, 0xCF, 0x2D, 0x70, 0x13, 0xA7}, 0x00};

    UCHAR keyID[] = {0x65, 0x6C, 0x74, 0x62};

    UINT16 text_len;
    UCHAR  offset;
    UCHAR * enc_out;
    UCHAR * plain_text;
    UCHAR * key;

    /* Initialize */
    retval = API_SUCCESS;
    text_len = (1 + 4 + 16 + 16 + 7 + 7 + 2);
    offset = 0;

    appl_smp_invert_byte_stream (N1, sizeof(N1));
    appl_smp_invert_byte_stream (N2, sizeof(N2));

    /* allocate mem for Plain text, key and enc_output */
    plain_text = BT_alloc_mem(text_len);
    key        = BT_alloc_mem(16);
    enc_out    = BT_alloc_mem(16);

    /* Populate Plain Text */
    /* Length 256 = 0x0100 */
    plain_text[offset] = 0x00;
    offset ++;
    plain_text[offset] = 0x01;
    offset ++;

    /* A2 */
    BT_COPY_BD_ADDR(&plain_text[offset],r_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],r_bd_addr.type);
    offset += 1;

    /* A1 */
    BT_COPY_BD_ADDR(&plain_text[offset],l_bd_addr.addr);
    offset += BT_BD_ADDR_SIZE;
    BT_COPY_TYPE(plain_text[offset],l_bd_addr.type);
    offset += 1;

    /* N2 */
    BT_mem_copy(&plain_text[offset],N2,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* N1 */
    BT_mem_copy(&plain_text[offset],N1,SMP_RAND_128B_SIZE);
    offset += SMP_RAND_128B_SIZE;

    /* KeyID */
    BT_mem_copy(&plain_text[offset],keyID, 4);
    offset += 4;

    /* Counter = 0 */
    plain_text[offset] = 0x00;

    /* Populate Encryption Key */
    BT_mem_copy(key,enc_key,16);

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

    IotLogDebug("\nSMP CMAC Test Complete CB 0x%02X received\n", aes_op);

    /* Check the AES CMAC operation initiated */
    switch(aes_op)
    {
        case SMP_AES_CMAC_SIGN_OP:
            IotLogDebug ("SMP_AES_CMAC_SIGN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_CONF_VAL_GEN_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_CONF_VAL_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_T_KEY_GEN_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_T_KEY_GEN_OP\n");
            BT_mem_copy(appl_smp_lesc_test_f5_T,enc_out,16);
            appl_smp_test_f5_ltk(appl_smp_lesc_test_f5_T);
            break;

        case SMP_AES_CMAC_LESC_LTK_GEN_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_LTK_GEN_OP\n");
            appl_smp_test_f5_mackey(appl_smp_lesc_test_f5_T);
            break;

        case SMP_AES_CMAC_LESC_MACKEY_GEN_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_MACKEY_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_CHK_VAL_GEN_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_CHK_VAL_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_NUM_COMP_VAL_GEN_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_NUM_COMP_VAL_GEN_OP\n");
            break;

        case SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP:
            IotLogDebug ("SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP\n");
            break;

        default:
            break;
    }

    IotLogDebug ("Data:\n");
    appl_dump_bytes(plain_text, plain_text_len);

    IotLogDebug ("Key:\n");
    appl_dump_bytes(key, 16);

    IotLogDebug ("MAC:\n");
    appl_dump_bytes(enc_out, 16);

    BT_free_mem(plain_text);
    BT_free_mem(key);
    BT_free_mem(enc_out);

    return;
}
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */

#endif /* SMP */

