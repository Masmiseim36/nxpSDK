
/**
 *  \file appl_sm.c
 *
 *  Source File for SM Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_sm.h"
#include "appl_utils.h"

#ifdef HDP
/* For hdp_appl_delete_all_data_chnls() */
#include "appl_hdp.h"
#endif /* HDP */

#ifdef CLASSIC_SEC_MANAGER
/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
static const char sm_options[] = "\n\
========= SM Menu =================== \n\
    0.  Exit \n\
    1.  Refresh \n\
\n\
    2.  Set Security Mode \n\
    3.  Get Security Mode \n\
    4.  Set Default PIN Code \n\
    5.  Get Default PIN Code \n\
    6.  Set Device IO Capability \n\
    7.  Enable/Disable SSP Debug Mode \n\
    8.  Set Secure Connections Only Mode \n\
    9.  Get Secure Connections Only Mode \n\
\n\
    10. Add Device to Device DB \n\
    11. Set Device Trust \n\
    12. Set Device PIN Code \n\
    13. Get Device PIN Code \n\
    14. Set Device Link Key \n\
    15. Get Device Link Key \n\
    16. Delete Device Link Key \n\
    17. Get Device Attributes \n\
    18. Delete Device(s) from Device DB \n\
    19. Get List of Devices \n\
    20. Set Pairable Mode \n\
\n\
    25. Set Authentication Requirement \n\
    26. Reset Authentication Requirement \n\
\n\
    30. Add Service to Service DB \n\
    31. Delete Service from Service DB \n\
    32. Set Service Attributes \n\
    33. Get Service Attributes \n\
    34. Set UI Authorization Request Reply \n\
\n\
    40. Register UI Notification Callback \n\
    41. Set UI Connection Request Reply \n\
    42. Set UI User Conf Request Reply \n\
\n\
    50. Change Connection Link Key \n\
    51. Authentication Request \n\
    52. Encryption Request \n\
\n\
    53. User Confirmation Request Reply \n\
    54. User Passkey Request Reply \n\
\n\
    55. Set Remote OOB Data \n\
\n\
    60. Show Device Ranks \n\
\n\
Your Choice ? ";


DECL_STATIC SM_PEER_INFO appl_sm_dev_list [SM_MAX_DEVICES];

#ifdef SM_HAVE_MODE_2
DECL_STATIC SM_SERVICE appl_sm_service;
DECL_STATIC UCHAR appl_sm_ui_authorization_request_reply = 0x1U;
#endif /* SM_HAVE_MODE_2 */

DECL_STATIC UCHAR appl_sm_ui_connection_request_reply = 0x1U;

#ifdef BT_SSP
DECL_STATIC UCHAR appl_sm_ui_user_conf_request_reply = 0x1U;
DECL_STATIC UCHAR appl_sm_ui_user_conf_request_reply_bd_addr [BT_BD_ADDR_SIZE];
DECL_STATIC UCHAR appl_sm_ui_passkey_request_reply_bd_addr [BT_BD_ADDR_SIZE];
#endif /* BT_SSP */


/* ------------------------------------------ Functions */
void main_sm_operations (void)
{
    int choice, menu_choice;
    int ch;
    UINT32 rank;
    API_RESULT retval;

#ifdef SM_HAVE_MODE_2
    UCHAR service_id;
#endif /* SM_HAVE_MODE_2 */

    UCHAR flag, count;

#ifndef BT_SSP
    UCHAR sec_mode, enc_mode;
#endif  /* BT_SSP */

#ifdef SM_AUTHREQ_DYNAMIC
    UCHAR auth;
#endif /* SM_AUTHREQ_DYNAMIC */

    UCHAR bd_addr[BT_BD_ADDR_SIZE], *bd_addr_ptr;

    UCHAR *pin_ptr, pin_length, pin [BT_PIN_CODE_SIZE+1U];
    UCHAR link_key [BT_LINK_KEY_SIZE];
    int i;

    BT_LOOP_FOREVER()
    {
        printf("%s", sm_options);
        scanf("%u", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            printf("Refreshing ...\n");
            break;

        case 2:
        #ifndef BT_SSP
            printf("Enter Security Mode = "); fflush (stdout);
            scanf("%u", &choice);
            sec_mode = (UCHAR) choice;

            printf("Enter Encrypt Mode = "); fflush (stdout);
            scanf("%u", &choice);
            enc_mode = (UCHAR) choice;

            printf("Setting Security Mode .. "); fflush (stdout);
            retval = BT_sm_default_security_mode (sec_mode, enc_mode);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }
            #else
                printf("Setting Security Mode is not Supported\n");
            #endif /* BT_SSP */

            break;

        case 3:
        #ifndef BT_SSP
            printf("Getting Security Mode .. "); fflush (stdout);
            retval = BT_sm_get_security_mode (&sec_mode, &enc_mode);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    SM Security Mode = 0x%02X, Enc Mode = 0x%02X\n",
                sec_mode, enc_mode);
            }
        #else
                printf("Getting Security Mode is not Supported\n");
        #endif /* BT_SSP */

            break;

        case 4:
            printf("Enter PIN Code Length (Max %d Chars) = ",
            BT_PIN_CODE_SIZE); fflush(stdout);

            scanf("%u", &choice);

            pin_ptr = NULL;
            if (choice > 0U)
            {
                if (BT_PIN_CODE_SIZE < choice)
                {
                    printf("*** PIN Length Must be <= %d\n", BT_PIN_CODE_SIZE);
                    break;
                }

                printf("Enter PIN code = "); fflush (stdout);
                scanf("%s", (CHAR *) pin);

                if ((unsigned int)choice != BT_str_len(pin))
                {
                    printf("PIN Length is NOT %d Chars !\n", choice);
                    break;
                }

                pin_ptr = pin;
            }
            else if (choice < 0)
            {
                break;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            printf("Setting Default PIN Code ... "); fflush (stdout);
            retval = BT_sm_default_pin_code (pin_ptr, (UCHAR) choice);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 5:
            printf("Getting Default PIN Code ... "); fflush (stdout);
            retval = BT_sm_get_pin (pin, &pin_length);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    PIN Code Length = %d\n", pin_length);
                printf("    PIN Code = ");
                for (choice = 0U; choice < pin_length; choice ++)
                {
                     printf("%c", pin[choice]);
                }
                printf("\n");
            }

            break;

    #ifdef BT_SSP
        case 6:
        {
            unsigned int io_in, io_out;
            UCHAR io_cap;

            printf("Enter Device Input Capability\n");
            printf("  [1] No Input\n");
            printf("  [2] Yes/No\n");
            printf("  [3] Keyboard\n");
            printf("Your choice = "); fflush(stdout);
            scanf("%u", &io_in);

            printf("Enter Device Output Capability\n");
            printf("  [1] No Output\n");
            printf("  [2] Numeric Output\n");
            printf("Your choice = "); fflush(stdout);
            scanf("%u", &io_out);

            io_cap = 0xFFU;
            switch (io_out)
            {
            case 1:
                /* Output Cap = No Output */
                switch (io_in)
                {
                case 1:    /* Fall Through */
                case 2:
                    io_cap = SM_IO_CAPABILITY_NO_INPUT_NO_OUTPUT;
                    break;

                case 3:
                    io_cap = SM_IO_CAPABILITY_KEYBOARD_ONLY;
                    break;

                default:
                    printf("*** Invalid Input Capability - %u. Try again.\n",
                    io_in);
                    break;
                }

                break;

            case 2:
                /* Output Cap = Numeric Output */
                switch (io_in)
                {
                case 1:
                    io_cap = SM_IO_CAPABILITY_DISPLAY_ONLY;
                    break;

                case 2:    /* Fall Through */
                case 3:
                    io_cap = SM_IO_CAPABILITY_DISPLAY_YES_NO;
                    break;

                default:
                    printf("*** Invalid Input Capability - %u. Try again.\n",
                    io_in);
                    break;
                }

                break;

            default:
                printf("*** Invalid Output Capability - %u. Try again.\n",
                io_out);
            }

            if (0xFFU != io_cap)
            {
                printf("Setting Local IO Capability = 0x%02X\n", io_cap);
                (BT_IGNORE_RETURN_VALUE) BT_sm_set_local_io_cap (io_cap);
            }

            break;
        }
    #endif /* BT_SSP */

        case 7:
    #ifdef BT_SSP
    #ifdef BT_SSP_DEBUG
        {
            unsigned int ssp_debug_mode;

            printf("Enter Enable/Disable SSP Debug Mode\n");
            printf("  [0] Disable\n");
            printf("  [1] Enable\n");
            printf("Your choice = "); fflush(stdout);
            scanf("%u", &ssp_debug_mode);

            if (ssp_debug_mode > 1U)
            {
                printf("Incorrect Value %u\n", ssp_debug_mode);
            }
            else
            {
                printf("%s SSP Debug Mode\n",
                ((0U == ssp_debug_mode)? "Disabling": "Enabling"));

                retval = BT_hci_write_simple_pairing_debug_mode
                         (
                             (UCHAR)ssp_debug_mode
                         );

                if (API_SUCCESS != retval)
                {
                    printf("FAILED ! Reason = 0x%04X\n", retval);
                    break;
                }
                else
                {
                    printf("Initiated\n");
                }
            }
        }
    #else
        printf("Enable BT_SSP_DEBUG compilation flag\n");
    #endif /* BT_SSP_DEBUG */
    #endif /* BT_SSP */

        break;

        case 8:
    #ifdef BT_BRSC
        {
            unsigned int sc_only_mode;

            printf("Enter Enable/Disable Secure Connections Only Mode\n");
            printf("  [0] Disable\n");
            printf("  [1] Enable\n");
            printf("Your choice = "); fflush(stdout);
            scanf("%u", &sc_only_mode);

            if (sc_only_mode > 1U)
            {
                printf("Incorrect Value %u\n", sc_only_mode);
            }
            else
            {
                printf("%s Secure Connections Only Mode\n",
                ((0U == sc_only_mode)? "Disabling": "Enabling"));

                retval = BT_sm_set_secure_connections_only_mode
                         (
                             (UCHAR)sc_only_mode
                         );

                if (API_SUCCESS != retval)
                {
                    printf("FAILED ! Reason = 0x%04X\n", retval);
                    break;
                }
                else
                {
                    printf("Initiated\n");
                }
            }
        }
    #else
        printf("Enable BT_BRSC compilation flag\n");
    #endif /* BT_BRSC */
        break;

        case 9:
    #ifdef BT_BRSC
        {
            UCHAR sc_only_mode;

            printf("Getting Secure Connections Only Mode .. "); fflush(stdout);
            retval = BT_sm_get_secure_connections_only_mode(&sc_only_mode);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    SM Secure Connections Only Mode = 0x%02X\n",
                sc_only_mode);
            }
        }
    #else
        printf("Enable BT_BRSC compilation flag\n");
    #endif /* BT_BRSC */

            break;

        case 10:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Adding Device to Device DB .. "); fflush (stdout);
            retval = BT_sm_add_device (bd_addr);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 11:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Enter Device Trust Level (0/1) = "); fflush (stdout);
            scanf("%u", &choice);
            flag = (UCHAR) choice;

            printf("Setting Device Trust Level .. "); fflush (stdout);
            retval = BT_sm_set_device_trusted (bd_addr, flag);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 12:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Enter Device PIN Code Length (Max %d Chars) = ",
            BT_PIN_CODE_SIZE); fflush(stdout);

            scanf("%u", &choice);

            pin_ptr = NULL;
            if (choice > 0U)
            {
                if (BT_PIN_CODE_SIZE < choice)
                {
                    printf("*** Device PIN Length Must be <= %d\n",
                    BT_PIN_CODE_SIZE);

                    break;
                }

                printf("Enter Device PIN code = "); fflush (stdout);
                scanf("%s", (CHAR *) pin);

                if ((unsigned int)choice != BT_str_len(pin))
                {
                    printf("*** Device PIN Length is NOT %d Chars !\n",
                    choice);

                    break;
                }

                pin_ptr = pin;
            }
            else if (choice < 0)
            {
                break;
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            pin_length = (UCHAR) choice;

            printf("Setting Device PIN Code .. "); fflush (stdout);
            retval = BT_sm_set_device_pin_code (bd_addr, pin_ptr, pin_length);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 13:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Getting Device PIN Code ... "); fflush (stdout);
            retval = BT_sm_get_device_pin_code (bd_addr, pin, &pin_length);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    PIN Code Length = %d\n", pin_length);
                printf("    PIN Code = ");
                for (choice = 0U; choice < pin_length; choice ++)
                {
                     printf("%c", pin[choice]);
                }
                printf("\n");
            }

            break;

        case 14:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Enter Device Link Key (16 Hex Values) = ");
            fflush(stdout);

            for (flag = 0U; flag < BT_LINK_KEY_SIZE; flag ++)
            {
                scanf ("%x", &choice);
                link_key[flag] = (UCHAR) choice;
            }

            printf("Setting Device Link Key .. "); fflush (stdout);
            retval = BT_sm_set_device_link_key (bd_addr, link_key);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 15:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Getting Device Link Key .. "); fflush (stdout);
            retval = BT_sm_get_device_link_key (bd_addr, link_key);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    Device Link Key = ");
                for (choice = 0U; choice < BT_LINK_KEY_SIZE; choice ++)
                {
                    printf("%02X ", link_key[choice]);
                }
                printf("\n");
            }

            break;

        case 16:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Deleting Device Link Key .. "); fflush (stdout);
            retval = BT_sm_delete_device_link_key (bd_addr);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 17:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Getiing Device Attributes .. "); fflush (stdout);
            retval = BT_sm_get_device_attributes (bd_addr, &flag);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    Device Attributes = 0x%02X\n", flag);

                printf("        Trusted ? ");
                if (0x01U == (flag & 0x01U))
                {
                    printf("Yes.\n");
                }
                else
                {
                    printf("No.\n");
                }

                printf("        Authenticated ? ");
                if (0x02U == (flag & 0x02U))
                {
                    printf("Yes.\n");
                }
                else
                {
                    printf("No.\n");
                }

                printf("        Encrypted ? ");
                if (0x04U == (flag & 0x04U))
                {
                    printf("Yes.\n");
                }
                else
                {
                    printf("No.\n");
                }

                printf("        Link Key Available ? ");
                if (0x08U == (flag & 0x08U))
                {
                    printf("Yes.\n");
                }
                else
                {
                    printf("No.\n");
                }

                printf("        PIN Code Available ? ");
                if (0x10U == (flag & 0x10U))
                {
                    printf("Yes.\n");
                }
                else
                {
                    printf("No.\n");
                }
            }

            break;

        case 18:
            printf("Delete One (0) or All (1) Devices or All but Active (2) device? "); fflush (stdout);
            scanf ("%u", &choice);

            if ((0U == choice) || (2U == choice))
            {
                printf("Enter Device BD_ADDR: "); fflush (stdout);
                (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

                bd_addr_ptr = bd_addr;
            }
            else
            {
                bd_addr_ptr = NULL;
            }

            printf("Delete from Device List:\n");
            printf("  [0] Device List - Trusted\n");
            printf("  [1] Device List - Paired\n");
            printf("  [2] Device List - Any\n");
            printf("Your Choice = ");
            scanf ("%u", &ch);

            if ((ch < 0) || (ch > 2U))
            {
                printf("*** Invalid Choice - %d. Try again.\n", ch);
                break;
            }

            switch (ch)
            {
            case 0:
                /* Delete Device(s) - Trusted List */
                printf("Deleting Device(s) from Trusted List .. ");
                fflush (stdout);

                flag = SM_TRUSTED_LIST;
                break;

            case 1:
                /* Delete Device(s) - Paired List */
                printf("Deleting Device(s) from Paired List .. ");
                fflush (stdout);

                flag = SM_PAIR_LIST;
                break;

            case 2:
                /* Delete Device(s) - Any List */
                printf("Deleting Device(s) from Any/All List .. ");
                fflush (stdout);

                flag = SM_ANY_LIST;
                break;

            default:
                printf("Invalid Choice\n");
                break;
            }

            if ((0U == choice) || (1U == choice))
            {
                retval = BT_sm_delete_device (bd_addr_ptr, flag);
                if (API_SUCCESS != retval)
                {
                    printf("FAILED ! Reason = 0x%04X\n", retval);
                    break;
                }
                else
                {
                    printf("OK\n");
                    #ifdef HDP
                        printf("Deleted device(s) from the SM db\n");
                        if (SM_TRUSTED_LIST != flag)
                        {
                            hdp_appl_delete_all_data_chnls(bd_addr_ptr);
                        }
                    #endif /* HDP */
                }
            }
            else
            {
                /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
                count = 0U;

                retval = BT_sm_get_device_list (appl_sm_dev_list, flag, &count);
                if (API_SUCCESS == retval)
                {
                    for (i = 0U; i < count; i++)
                    {
                        if (0U != BT_mem_cmp(bd_addr_ptr, appl_sm_dev_list[i].bd_addr, BT_BD_ADDR_SIZE))
                        {
                            retval = BT_sm_delete_device (appl_sm_dev_list[i].bd_addr, flag);
                            if (API_SUCCESS != retval)
                            {
                                printf("FAILED ! Reason = 0x%04X\n", retval);
                                break;
                             }
                            else
                            {
                            #ifdef HDP
                                printf("Deleted all devices from the SM db except the active bonded device\n");
                                if (SM_TRUSTED_LIST != flag)
                                {
                                    hdp_appl_delete_all_data_chnls(appl_sm_dev_list[i].bd_addr);
                                }
                            #endif /* HDP */
                            }
                        }
                        else
                        {
                            printf("%d "BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER
                            "\n", i, BT_DEVICE_ADDR_ONLY_PRINT_STR \
                            (appl_sm_dev_list[i].bd_addr));
                            printf("is a active bonded device which is not deleted from SM database");
                        }
                        printf("\n");
                    }
                    printf("There are no bonded devices listed in the SM database");
                 }
            }

            break;

        case 19:
            printf("Which Device List:\n");
            printf("  [0] Device List - Trusted\n");
            printf("  [1] Device List - Paired\n");
            printf("  [2] Device List - Any\n");
            printf("Your Choice = ");
            scanf ("%u", &choice);

            if ((choice < 0) || (choice > 2U))
            {
                printf("*** Invalid Choice - %d. Try again.\n", choice);
                break;
            }

            switch (choice)
            {
            case 0:
                /* List Device(s) - Trusted List */
                printf("Listing Device(s) from Trusted List .. ");
                fflush (stdout);

                flag = SM_TRUSTED_LIST;
                break;

            case 1:
                /* List Device(s) - Paired List */
                printf("Listing Device(s) from Paired List .. ");
                fflush (stdout);

                flag = SM_PAIR_LIST;
                break;

            case 2:
                /* List Device(s) - Any List */
                printf("Listing Device(s) from Any/All List .. ");
                fflush (stdout);

                flag = SM_ANY_LIST;
                break;

            default:
                printf("Invalid Choice\n");
                break;
            }

            count = SM_MAX_DEVICES;
            retval = BT_sm_get_device_list (appl_sm_dev_list, flag, &count);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    Device List: %d Entries\n", count);
                for (choice = 0U; choice < count; choice ++)
                {
                    printf("      %03d. " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER
                    "\n", choice, BT_DEVICE_ADDR_ONLY_PRINT_STR \
                    (appl_sm_dev_list[choice].bd_addr));
                }
                printf("\n");
            }

            break;

        case 20:
            printf("Select Paiable Mode:\n");
            printf("  [0] Non-pairable\n");
            printf("  [1] Pairable, Non-bondable\n");
            printf("  [3] Pairable and Bondable\n");
            printf("Your Choice = ");
            scanf ("%u", &choice);

            retval = BT_sm_set_pairable((UCHAR)choice);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

#ifdef SM_AUTHREQ_DYNAMIC
        case 25:
            printf("Enter MITM Reqd (0/1): ");
            scanf("%d", &choice);
            auth = (UCHAR)choice;

            printf("Enter Bonding Reqd (0/1/2): ");
            scanf("%d", &choice);
            auth |= (UCHAR)(choice << 1U);

            (BT_IGNORE_RETURN_VALUE) BT_sm_set_local_authreq(0x01U, auth);
            break;

        case 26:
            (BT_IGNORE_RETURN_VALUE) BT_sm_set_local_authreq(0x00U, 0x00U);
            break;
#endif /* SM_AUTHREQ_DYNAMIC */

    #ifdef SM_HAVE_MODE_2
        case 30:
            printf("Enter L2CAP PSM (in Hex) = "); fflush (stdout);
            scanf("%x", &choice);
            appl_sm_service.psm = (UINT16) choice;

            printf("Enter RFCOMM Server Ch (in Dec, 1-30) = ");
            fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.server_ch = (UCHAR) choice;

        #ifdef BT_SSP

            printf("Authentication Required (0/1/2/3/4) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.authenticate = (UCHAR) choice;

            printf("Authorization Required ?\n"); fflush (stdout);
            printf("    [0] No Authorization\n");
            printf("    [1] Authorization Required\n");
            printf("Your Choice ? "); fflush (stdout);
            scanf("%u", &choice);
            switch (choice)
            {
            case 0:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_NONE;
                break;
            case 1:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_REQUIRED;
                break;
            default:
                printf("Invalid Choice - %d. Try again\n", choice);
                break;
            }

            /* Encryption is always ON under SSP */
            appl_sm_service.encrypt = 0x1U;

            /* Must Register UI Callback for SSP */
            appl_sm_service.service_sm_cb = appl_sm_service_cb;

            printf("Set 'No Bonding' (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.no_bonding = (UCHAR) choice;

        #else  /* BT_SSP */

            printf("Authentication Required (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.authenticate = (0U == choice) ? 0x0U : 0x1U;

            printf("Authorization Required ?\n"); fflush (stdout);
            printf("    [0] No Authorization\n");
            printf("    [1] Authorization Required (Mode 2)\n");
            printf("    [2] Authorization Required (Mode 1/3)\n");
            printf("Your Choice ? "); fflush (stdout);
            scanf("%u", &choice);
            switch (choice)
            {
            case 0:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_NONE;
                break;
            case 1:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_MODE_2;
                break;
            case 2:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_MODE_1_3;
                break;
            default:
                printf("Invalid Choice - %d. Try again\n", choice);
                break;
            }

            printf("Encryption Required (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.encrypt = (0U == choice) ? 0x0U : 0x1U;

            printf("Register UI Callback (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.service_sm_cb = (0U == choice)
                                            ? NULL : appl_sm_service_cb;

        #endif /* BT_SSP */

            printf("Adding SM Service ... "); fflush (stdout);
            retval = BT_sm_add_service (&appl_sm_service, &service_id);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("Service ID = 0x%02X\n", service_id);
            }

            break;

        case 31:
            printf("Enter Service ID (in Hex) = "); fflush (stdout);
            scanf("%x", &choice);
            service_id = (UCHAR) choice;

            printf("Deleting SM Service ... "); fflush (stdout);
            retval = BT_sm_delete_service (service_id);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 32:
            printf("Enter Service ID (in Hex) = "); fflush (stdout);
            scanf("%x", &choice);
            service_id = (UCHAR) choice;

        #ifdef BT_SSP

            printf("Authentication Required (0/1/2/3) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.authenticate = (UCHAR) choice;

            printf("Authorization Required ?\n"); fflush (stdout);
            printf("    [0] No Authorization\n");
            printf("    [1] Authorization Required\n");
            printf("Your Choice ? "); fflush (stdout);
            scanf("%u", &choice);
            switch (choice)
            {
            case 0:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_NONE;
                break;
            case 1:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_REQUIRED;
                break;
            default:
                printf("Invalid Choice - %d. Try again\n", choice);
                break;
            }

            /* Encryption is always ON under SSP */
            appl_sm_service.encrypt = 0x1U;

            /* Must Register UI Callback for SSP */
            appl_sm_service.service_sm_cb = appl_sm_service_cb;

            printf("Set 'No Bonding' (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.no_bonding = (UCHAR) choice;

        #else  /* BT_SSP */

            printf("Authentication Required (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.authenticate = (0U == choice) ? 0x0U : 0x1U;

            printf("Authorization Required ?\n"); fflush (stdout);
            printf("    [0] No Authorization\n");
            printf("    [1] Authorization Required (Mode 2)\n");
            printf("    [2] Authorization Required (Mode 1/3)\n");
            printf("Your Choice ? "); fflush (stdout);
            scanf("%u", &choice);
            switch (choice)
            {
            case 0:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_NONE;
                break;
            case 1:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_MODE_2;
                break;
            case 2:
                appl_sm_service.authorize = SM_SERVICE_AUTHORIZATION_MODE_1_3;
                break;
            default:
                printf("Invalid Choice - %d. Try again\n", choice);
                break;
            }

            printf("Encryption Required (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.encrypt = (0U == choice) ? 0x0U : 0x1U;

            printf("Register UI Callback (0/1) ? "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_service.service_sm_cb = (0U == choice)
                                            ? NULL : appl_sm_service_cb;

        #endif /* BT_SSP */

            printf("Setting Service Attributes ... "); fflush (stdout);
            retval = BT_sm_set_service_attributes
                     (service_id, &appl_sm_service);

            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 33:
            printf("Enter Service ID (in Hex) = "); fflush (stdout);
            scanf("%x", &choice);
            service_id = (UCHAR) choice;

            printf("Getting Service Attributes ... "); fflush (stdout);
            retval = BT_sm_get_service_attributes
                     (service_id, &appl_sm_service);

            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
                printf("    Service Attribute: PSM 0x%04X Server Ch 0x%02X\n",
                appl_sm_service.psm, appl_sm_service.server_ch);

                printf("      Authentication Required ? %s (0x%02X)\n",
                (0x0U == appl_sm_service.authenticate) ? "No" : "Yes",
                appl_sm_service.authenticate);

                printf("      Authorization Required ? %s (0x%02X)\n",
                (0x0U == appl_sm_service.authorize) ? "No" : "Yes",
                appl_sm_service.authorize);

                printf("      Encryption Required ? %s\n",
                (0x1U == appl_sm_service.encrypt) ? "Yes" : "No");

            #ifdef BT_SSP
                printf("      No Bonding ? %s\n",
                (0x1U == appl_sm_service.no_bonding) ? "Set" : "Not set");
            #endif /* BT_SSP */

                printf("      UI Callback Registered ? %s\n",
                (NULL == appl_sm_service.service_sm_cb) ? "No" : "Yes");
            }

            break;

        case 34:
            printf("[0] Set Authorization Request Reply - Reject\n");
            printf("[1] Set Authorization Request Reply - Accept\n");
            printf("Your Choice = "); fflush (stdout);
            scanf("%u", &choice);

            if ((choice < 0) || (choice > 1U))
            {
                printf("*** Invalid Choice. Try again\n");
                break;
            }

            appl_sm_ui_authorization_request_reply = (UCHAR) choice;

            break;

    #endif /* SM_HAVE_MODE_2 */


        case 40:
            printf("Registering UI Notification Callback ... ");
            fflush(stdout);
            retval = BT_sm_register_user_interface (appl_sm_ui_notify_cb);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 41:
            printf("[0] Set Connection Request Reply - Reject\n");
            printf("[1] Set Connection Request Reply - Accept\n");
            printf("Your Choice = "); fflush (stdout);
            scanf("%u", &choice);

            if ((choice < 0) || (choice > 1U))
            {
                printf("*** Invalid Choice. Try again\n");
                break;
            }

            appl_sm_ui_connection_request_reply = (UCHAR) choice;

            break;

    #ifdef BT_SSP
        case 42:
            printf("[0] Set User Conf Request Reply - Manual\n");
            printf("[1] Set User Conf Request Reply - Auto-Accept\n");
            printf("[2] Set User Conf Request Reply - Auto-Reject\n");
            printf("Your Choice = "); fflush (stdout);
            scanf("%u", &choice);
            appl_sm_ui_user_conf_request_reply = (UCHAR) choice;

            break;
    #endif /* BT_SSP */

        case 50:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Starting Change Connection Link Key ... ");
            fflush (stdout);

            retval = BT_sm_change_connection_link_key (bd_addr);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 51:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("Starting Authentication Request ... ");
            fflush (stdout);

            retval = BT_sm_authentication_request (bd_addr);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 52:
            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (bd_addr);

            printf("[0] Turn Link-level Encryption OFF\n");
            printf("[1] Turn Link-level Encryption ON\n");
            printf("Your Choice = "); fflush (stdout);
            scanf("%u", &choice);
            flag = (UCHAR) choice;

            printf("Starting Encryption Request ... ");
            fflush (stdout);

            retval = BT_sm_encryption_request (bd_addr, flag);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

    #ifdef BT_SSP
        case 53:
            printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR \
            (appl_sm_ui_user_conf_request_reply_bd_addr));

            printf("Confirmation: [0] No, [1] Yes ? "); fflush(stdout);
            scanf("%u", &choice);
            flag = (UCHAR) choice;

            printf("Starting User Conf Request Reply ... ");
            fflush (stdout);

            retval = BT_sm_user_conf_request_reply
                     (
                         appl_sm_ui_user_conf_request_reply_bd_addr,
                         flag
                     );

            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 54:
        {
#ifdef BT_SSP_PE
            UCHAR passkey[4U];

            printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR \
            (appl_sm_ui_passkey_request_reply_bd_addr));

            printf("Starting Send Keypress Notification: Started ...\n");
            fflush(stdout);

            retval = BT_hci_send_keypress_notification
                     (
                         appl_sm_ui_passkey_request_reply_bd_addr,
                         0x00U
                     );

            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            printf("Passkey (6 digits) ? "); fflush(stdout);
            scanf("%u", &choice);
            passkey[0U] = (UCHAR) choice;
            passkey[1U] = (UCHAR) (choice >> 8U);
            passkey[2U] = (UCHAR) (choice >> 16U);
            passkey[3U] = (UCHAR) (choice >> 24U);

            printf("Starting Send Keypress Notification: Completed ...\n");
            fflush(stdout);

            retval = BT_hci_send_keypress_notification
                     (
                         appl_sm_ui_passkey_request_reply_bd_addr,
                         0x04U
                     );

            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            printf("Starting User Conf Request Reply ... ");
            fflush (stdout);

            retval = BT_sm_user_passkey_request_reply
                     (
                         appl_sm_ui_passkey_request_reply_bd_addr,
                         passkey
                     );

            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }
#else
            printf("Define BT_SSP_PE feature flag\n");
#endif /* BT_SSP_PE */
        }

            break;

        case 55:
        {
#ifdef BT_SSP_OOB
            SM_OOB_INFO oob_info;

            printf("Enter Device BD_ADDR: "); fflush (stdout);
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr (oob_info.bd_addr);

            printf("Enter Device SSP Hash 'C' (16 Hex Values) = ");
            fflush(stdout);

            for (flag = 0U; flag < 16U; flag ++)
            {
                scanf ("%x", &choice);
                oob_info.ssp_c[flag] = (UCHAR) choice;
            }

            printf("Enter Device SSP Randomizer 'R' (16 Hex Values) = ");
            fflush(stdout);

            for (flag = 0U; flag < 16U; flag ++)
            {
                scanf ("%x", &choice);
                oob_info.ssp_r[flag] = (UCHAR) choice;
            }

            printf("Setting Device Remote OOB Data .. "); fflush (stdout);
            retval = BT_sm_set_device_oob_data (&oob_info);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }
#else
            printf("Define BT_SSP_OOB feature flag\n");
#endif /* BT_SSP_OOB */
        }

            break;
    #endif /* BT_SSP */

        case 60:
            (BT_IGNORE_RETURN_VALUE) BT_sm_get_rank_count_pl (&rank);
            for (count = 1U; count <= rank; count ++)
            {
                printf("Rank %u: ", count); fflush (stdout);
                retval = BT_sm_get_device_attr_pl (count, bd_addr);
                if (API_SUCCESS == retval)
                {
                    printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER,
                    BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
                }
                else
                {
                    printf("*** ERROR: 0x%04X\n", retval);
                }
            }

            break;

        default:
            printf("Invalid choice. Please try again\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }

        printf("\n");
        printf("Press ENTER to proceed ... ");
    }

    return;
}


API_RESULT appl_sm_ui_notify_cb
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UCHAR *    event_data
           )
{
#ifdef BT_SSP
    UINT32 numeric_val;
#endif /* BT_SSP */

    API_RESULT retval;
    UCHAR reason, flag, pin_len;
    UCHAR pin_code [BT_PIN_CODE_SIZE];
    UCHAR link_key [BT_LINK_KEY_SIZE];

    printf("\n");
    printf("Received SM Service UI Notification. Event Type 0x%02X\n",
    event_type);

    retval = API_SUCCESS;

    switch (event_type)
    {
    case SM_ACL_CONNECT_REQUEST_NTF:
        printf("Received UI Connection Request from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        reason = (0x1U == appl_sm_ui_connection_request_reply) ? 0x0U : 0x13U;

        printf("Replying to UI Connection Request ... "); fflush (stdout);
        retval = BT_sm_connection_request_reply
                 (bd_addr, appl_sm_ui_connection_request_reply, reason);

        if (API_SUCCESS != retval)
        {
            printf("FAILED ! Reason = 0x%04X\n", retval);
            break;
        }
        else
        {
            printf("OK\n");
        }

        break;

    case SM_PIN_CODE_REQUEST_NTF:
        printf("Received UI PIN Code Request from SM\n");
        printf( BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        flag = 0x1U;
        retval = BT_sm_get_device_link_key(bd_addr, link_key);

        /*
         * Respond negative if linkkey for device exists already.
         * Application to delete the link key for device and
         * then retry if it is an expected lost bond at the peer.
         */
        if (API_SUCCESS != retval)
        {
            /* Get PIN Code from SM */
            pin_len = BT_PIN_CODE_SIZE;
            retval = BT_sm_get_pin(pin_code, &pin_len);
            if (API_SUCCESS == retval)
            {
                printf("Replying to UI PIN Code Request (+Ve) ... ");
                fflush(stdout);
                retval = BT_sm_pin_code_request_reply(bd_addr, pin_code, pin_len);
                flag = 0x0U;
            }
        }

        if (0U != flag)
        {
            printf("Replying to UI PIN Code Request (-Ve) ... ");
            fflush (stdout);
            retval = BT_sm_pin_code_request_reply (bd_addr, NULL, 0U);
        }

        if (API_SUCCESS != retval)
        {
            printf("FAILED ! Reason = 0x%04X\n", retval);
            break;
        }
        else
        {
            printf("OK\n");
        }

        break;

    case SM_LINK_KEY_REQUEST_NTF:
        printf("Received UI Link Key Request from SM\n");
        printf( BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

       /* Get Link Key from Device Database */
        retval = BT_sm_get_device_link_key (bd_addr, link_key);
        flag = (API_SUCCESS == retval) ? 0x1U : 0x0U;

        printf("Replying to UI Link Key Request (%s) ... ",
        (API_SUCCESS == retval) ? "+Ve" : "-Ve"); fflush (stdout);
        retval = BT_sm_link_key_request_reply (bd_addr, link_key, flag);

        if (API_SUCCESS != retval)
        {
            printf("FAILED ! Reason = 0x%04X\n", retval);
            break;
        }
        else
        {
            printf("OK\n");
        }

        break;

#ifdef BT_SSP
#ifdef BT_SSP_NC
    case SM_USER_CONF_REQUEST_NTF:
        printf("Received UI User Conf Request from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        /* Get Numeric Value */
        numeric_val   = event_data[3U];
        numeric_val <<= 8U;
        numeric_val  |= event_data[2U];
        numeric_val <<= 8U;
        numeric_val  |= event_data[1U];
        numeric_val <<= 8U;
        numeric_val  |= event_data[0U];
        printf("Numeric Value = %06d (0x%08X)\n",
               (unsigned int) numeric_val, (unsigned int) numeric_val);

        switch (appl_sm_ui_user_conf_request_reply)
        {
        case 0x0U:
            /* Save BD_ADDR for Menu use */
            BT_COPY_BD_ADDR
            (
                appl_sm_ui_user_conf_request_reply_bd_addr,
                bd_addr
            );

            printf("Please Reply to User Conf Request using Menu Options\n");
            break;

        case 0x1U:
            printf("Replying to UI User Conf Request ... "); fflush (stdout);
            retval = BT_sm_user_conf_request_reply (bd_addr, 0x01U);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;

        case 0x2U:
        default:
            printf("Replying to UI User Conf Request (-Ve) ... "); fflush (stdout);
            retval = BT_sm_user_conf_request_reply (bd_addr, 0x00U);
            if (API_SUCCESS != retval)
            {
                printf("FAILED ! Reason = 0x%04X\n", retval);
                break;
            }
            else
            {
                printf("OK\n");
            }

            break;
        }

        break;
#endif /* BT_SSP_NC */

#ifdef BT_SSP_PE
    case SM_USER_PASSKEY_REQUEST_NTF:
        printf("Received UI User Passkey Request from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        /* Save BD_ADDR for Menu use */
        BT_COPY_BD_ADDR
        (
            appl_sm_ui_passkey_request_reply_bd_addr,
            bd_addr
        );

        printf("Please Reply to User Passkey Request using Menu Options\n");
        break;

    case SM_USER_PASSKEY_NTF:
        printf("Received UI User Passkey Notification from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        /* Get Numeric Value */
        numeric_val   = event_data[3U];
        numeric_val <<= 8U;
        numeric_val  |= event_data[2U];
        numeric_val <<= 8U;
        numeric_val  |= event_data[1U];
        numeric_val <<= 8U;
        numeric_val  |= event_data[0U];
        printf("Numeric Value = %u (0x%08X)\n",
        (unsigned int) numeric_val, (unsigned int) numeric_val);

        break;
#endif /* BT_SSP_PE */

#if 0
#ifdef BT_SSP_OOB
    case SM_REMOTE_OOB_DATA_REQUEST_NTF:
        break;
#endif /* BT_SSP_OOB */

    case SM_SIMPLE_PAIRING_COMPLETE_NTF:
        printf("Received UI Simple Pairing Complete from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
        printf("Status = 0x%02X\n", *event_data);

        break;
#endif /* BT_SSP */
#endif /* 0 */

    default:
        printf("*** Unknown/Undefined Event Type 0x%02X\n", event_type);
        break;
    }

    return retval;
}


#ifdef SM_HAVE_MODE_2
API_RESULT appl_sm_service_cb
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UCHAR *    event_data
           )
{
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(event_data);

    printf("\n");
    printf("Received SM Service UI Notification. Event Type 0x%02X\n",
    event_type);

    retval = API_SUCCESS;

    switch (event_type)
    {
    case SM_AUTHORIZATION_REQUEST_NTF:
        printf("Received UI Authorization Request from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        printf("Replying to UI Authorization Request ... "); fflush (stdout);
        retval = BT_sm_authorization_request_reply
                 (bd_addr, appl_sm_ui_authorization_request_reply);

        if (API_SUCCESS != retval)
        {
            printf("FAILED ! Reason = 0x%04X\n", retval);
            break;
        }
        else
        {
            printf("OK\n");
        }

        break;

#ifndef BT_SSP
    case SM_AUTHORIZATION_REQUEST_NTF_MODE_1_3:
        printf("Received UI Authorization Request (Mode 1/3) from SM\n");
        printf(BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));

        printf("Replying to UI Authorization Request ... "); fflush (stdout);
        retval = BT_sm_authorization_request_reply_mode_1_3
                 (bd_addr, appl_sm_ui_authorization_request_reply);

        if (API_SUCCESS != retval)
        {
            printf("FAILED ! Reason = 0x%04X\n", retval);
            break;
        }
        else
        {
            printf("OK\n");
        }

        break;
#endif  /* BT_SSP */

    default:
        printf("*** Unknown/Undefined Event Type 0x%02X\n", event_type);
        break;
    }

    return retval;
}
#endif /* SM_HAVE_MODE_2 */
#endif /* CLASSIC_SEC_MANAGER */


