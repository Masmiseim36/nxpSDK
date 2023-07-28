
/**
 *  \file appl_rfcomm.c
 *
 *  Source File for RFCOMM Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_rfcomm.h"
#include "appl_utils.h"

/* #define RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
/* #define APPL_RFCOMM_READ_SERVER_CHANNEL */
#define RFCOMM_FILE_OPERATION

#ifdef RFCOMM
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
static const char main_rfcomm_options[] = "\n\
========= RFCOMM Menu =============== \n\
    0.  Exit. \n\
    1.  Refresh \n\
 \n\
    2.  RFCOMM Channel Open. \n\
    3.  RFCOMM Channel Close. \n\
    4.  RFCOMM Channel Accept. \n\
    5.  RFCOMM Channel Accept Cancel. \n\
 \n\
    11. RFCOMM Channel Write. \n\
    12. RFCOMM Receive Rx Data to File. \n\
    13. RFCOMM Send File. \n\
    14. RFCOMM Stop Receiving Rx Data to File. \n\
 \n\
    20. RFCOMM Session Open. \n\
    21. RFCOMM Session Close. \n\
 \n\
    22. RFCOMM DLC Open. \n\
    23. RFCOMM DLC Close. \n\
 \n\
    30. RFCOMM Session FC Off. \n\
    31. RFCOMM Session FC On. \n\
    32. RFCOMM Session Test. \n\
 \n\
    40. RFCOMM Send DLC PN. \n\
    41. RFCOMM Send DLC RPN. \n\
    42. RFCOMM Send DLC MSC. \n\
    43. RFCOMM Send DLC RLS. \n\
 \n\
    50. RFCOMM Enable Credit Tx. \n\
    51. RFCOMM Disable Credit Tx. \n\
Your Option ? ";

#ifdef RFCOMM_FILE_OPERATION
DECL_STATIC char rfcomm_recv_file_name[APPL_RFCOMM_FILE_NAME_MAX_LEN];

DECL_STATIC BT_fops_file_handle rfcomm_send_fp;
DECL_STATIC BT_fops_file_handle rfcomm_recv_fp;
DECL_STATIC UCHAR rfcomm_send_file_operation;
DECL_STATIC UCHAR rfcomm_recv_file_operation;
DECL_STATIC APPL_RFCOMM_SEND_FILE_STAT send_file_stat;

DECL_STATIC UCHAR rfcomm_write_buffer[APPL_RFCOMM_WRITE_BUF_SIZE];
DECL_STATIC UINT16 rfcomm_read_len;
#endif /* RFCOMM_FILE_OPERATION */

DECL_STATIC RFCOMM_HANDLE appl_rfcomm_handle;

/* Global variables used for RFCOMM Write Operation in iteration */
/* Number of bytes in each RFCOMM write */
DECL_STATIC UINT16 appl_nbytes;
/* Number of times the RFCOMM Write to be performed */
DECL_STATIC UINT16 appl_loop;
/* Counter to maintain the number of RFCOMM Write/Read has completed */
DECL_STATIC UINT16 appl_count;

/* ----------------------------------------- Functions */
void main_rfcomm_operations (void)
{
    int choice, menu_choice;
    static UCHAR first_time = 0x0U;

    if (0x0U == first_time)
    {
        BT_mem_set (&appl_rfcomm_handle, 0x0, sizeof (RFCOMM_HANDLE));
        first_time = 0x1U;
    }

    BT_LOOP_FOREVER()
    {
        printf("%s", main_rfcomm_options);
        scanf("%d", &choice);
        menu_choice = choice;

        if (choice < 0)
        {
            printf("*** Invalid Choice. Try Again.\n");
            continue;
        }

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            appl_rfcomm_open();
            break;

        case 3:
            appl_rfcomm_close();
            break;

        case 4:
            appl_rfcomm_accept();
            break;

        case 5:
            appl_rfcomm_accept_cancel();
            break;

        case 11:
            appl_rfcomm_write();
            break;

        case 12:
            appl_rfcomm_recv_file();
            break;

        case 13:
            appl_rfcomm_send_file();
            break;

        case 14:
            appl_rfcomm_stop_recv_file();
            break;

        case 20:
            appl_rfcomm_session_open();
            break;

        case 21:
            appl_rfcomm_session_close();
            break;

        case 22:
            appl_rfcomm_dlc_open();
            break;

        case 23:
            appl_rfcomm_dlc_close();
            break;

        case 30:
            appl_rfcomm_session_fc_off();
            break;

        case 31:
            appl_rfcomm_session_fc_on();
            break;

        case 32:
            appl_rfcomm_session_test();
            break;

        case 40:
            appl_rfcomm_send_dlc_pn();
            break;

        case 41:
            appl_rfcomm_send_dlc_rpn();
            break;

        case 42:
            appl_rfcomm_send_dlc_msc();
            break;

        case 43:
            appl_rfcomm_send_dlc_rls();
            break;

        case 50:
        #ifdef RFCOMM_ENABLE_FC_FROM_APP
            appl_rfcomm_enable_credit_tx();
        #else
            printf("RFCOMM_ENABLE_FC_FROM_APP is not defined\n");
        #endif /* RFCOMM_ENABLE_FC_FROM_APP */
            break;

        case 51:
        #ifdef RFCOMM_ENABLE_FC_FROM_APP
            appl_rfcomm_disable_credit_tx();
        #else
            printf("RFCOMM_ENABLE_FC_FROM_APP is not defined\n");
        #endif /* RFCOMM_ENABLE_FC_FROM_APP */
            break;

        default:
            printf ("Invalid option %d\n", choice);
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


void appl_rfcomm_open (void)
{
    UCHAR server_ch;
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;
#ifdef APPL_RFCOMM_READ_SERVER_CHANNEL
    int read_input;
#endif /* APPL_RFCOMM_READ_SERVER_CHANNEL */

    server_ch = 0x1U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

    printf("Enter BD_ADDR: ");
    retval = appl_get_bd_addr(bd_addr);
    if (API_SUCCESS != retval)
    {
        printf("Invalid BD_ADDR Address Entered\n");
        /* return; */
    }
    else
    {
        RFCOMM_SET_HANDLE(&appl_rfcomm_handle, bd_addr, server_ch, appl_rfcomm_callback);

#ifdef APPL_RFCOMM_READ_SERVER_CHANNEL
        printf("Enter Server Channel: (in HEX)\n");
        scanf("%x", &read_input);

        server_ch = (UCHAR)read_input;
#endif /* APPL_RFCOMM_READ_SERVER_CHANNEL */

#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
        appl_rfcomm_handle.blob = "RFCOMM OPEN";
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */

        printf("Initiating RFCOMM Channel Open ... "); fflush(stdout);
        retval = BT_rfcomm_open(bd_addr, server_ch, &appl_rfcomm_handle);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for DLC Event Indication.\n");
        }
    }

    return;
}


void appl_rfcomm_close (void)
{
    API_RESULT retval;

    printf("Initiating RFCOMM Channel Close ... "); fflush (stdout);
#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
    appl_rfcomm_handle.blob = "RFCOMM CLOSE";
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */
    retval = BT_rfcomm_close (&appl_rfcomm_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
}


void appl_rfcomm_accept (void)
{
    UCHAR server_ch;
    API_RESULT retval;
    UCHAR bd_addr[BT_BD_ADDR_SIZE] = {0x0U, 0x0U, 0x0U, 0x0U, 0x0U, 0x0U};
#ifdef APPL_RFCOMM_READ_SERVER_CHANNEL
    int read_input;
#endif /* APPL_RFCOMM_READ_SERVER_CHANNEL */

    server_ch = 0x1U;

#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
    appl_rfcomm_handle.blob = "RFCOMM ACCEPT";
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */

#ifdef APPL_RFCOMM_READ_SERVER_CHANNEL
    printf("Enter Server Channel: (in HEX)\n");
    scanf ("%x", &read_input);

    server_ch = (UCHAR)read_input;
#endif /* APPL_RFCOMM_READ_SERVER_CHANNEL */

    printf("Initiating RFCOMM Channel Accept ... "); fflush (stdout);
    RFCOMM_SET_HANDLE(&appl_rfcomm_handle, bd_addr, server_ch, appl_rfcomm_callback);
    retval = BT_rfcomm_accept (&appl_rfcomm_handle, server_ch);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
}

void appl_rfcomm_accept_cancel (void)
{
    UCHAR server_ch;
    API_RESULT retval;

    server_ch = 0x1U;
    printf("Initiating RFCOMM Channel Accept Cancel ... "); fflush (stdout);
    retval = BT_rfcomm_accept_cancel (server_ch);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
}

void appl_rfcomm_write (void)
{
    int read_val;
    API_RESULT retval;
    UCHAR *buffer = NULL;
    UINT16 actual;

    printf("No of Bytes to Write = "); fflush (stdout);
    scanf("%d", &read_val);
    if (read_val < 0)
    {
        printf("No. of Bytes to Write is equal to 0\n");
        /* return; */
    }
    else
    {
        appl_nbytes = (UINT16)read_val;
        appl_count = 0U;
        actual = 0U;

        printf("Loop  = "); fflush(stdout);
        scanf("%d", &read_val);
        appl_loop = ((read_val < 0) ? (UINT16)1U : (UINT16)read_val);

        /* Return if 0 loop */
        if (0U == appl_loop)
        {
            printf("No. of loops = 0\n");
            /* return; */
        }
        else
        {

#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
            appl_rfcomm_handle.blob = "RFCOMM WRITE";
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */

            buffer = (UCHAR *)BT_alloc_mem(appl_nbytes);
            if (NULL == buffer)
            {
                printf("*** Buffer Allocation FAILED. !!\n");
                /* return; */
            }
            else
            {
                /* Set the appl data */
                if (2U > appl_nbytes)
                {
                    BT_mem_set(buffer, appl_count, appl_nbytes);
                }
                else
                {
                    BT_PACK_BE_2_BYTE_VAL(buffer, appl_count);
                    BT_mem_set((&buffer[2U]), appl_count, (appl_nbytes - 2U));
                }

                printf("Allocated Buffer = %p\n", buffer);

                printf("Initiating [%d]th RFCOMM Channel Write  ... ", appl_count);
                retval = BT_rfcomm_write(&appl_rfcomm_handle, buffer, appl_nbytes, &actual);
                if (API_SUCCESS != retval)
                {
                    printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                    BT_free_mem(buffer);
                }
                else
                {
                    printf("Succeeded !!\n");
                    printf("Wait for Write Complete.\n");
                }
            }
        }
    }

    return;
}


void appl_rfcomm_recv_file (void)
{
#ifdef RFCOMM_FILE_OPERATION
    API_RESULT retval;

    BT_mem_set(rfcomm_recv_file_name, 0, sizeof(rfcomm_recv_file_name));
    BT_str_print(rfcomm_recv_file_name, "%s%s", BT_FOPS_BASE, BT_FOPS_PATH_SEP);

    /* Read the name of the file to store */
    printf("Enter File Name to Receive Rx Data\n");
    scanf("%s", &rfcomm_recv_file_name[BT_str_len(rfcomm_recv_file_name)]);

    retval = BT_fops_file_open
             (
                 (UCHAR *)rfcomm_recv_file_name,
                 (UCHAR *)"wb",
                 &rfcomm_recv_fp
             );
    if(API_SUCCESS != retval)
    {
        printf("Failed to open file: %s\n", rfcomm_recv_file_name);

        /* return; */
    }
    else
    {
        rfcomm_recv_file_operation = APPL_RFCOMM_FILE_RECV;
    }
#endif /* RFCOMM_FILE_OPERATION */
}


void appl_rfcomm_send_file (void)
{
#ifdef RFCOMM_FILE_OPERATION
    API_RESULT retval;

    send_file_stat.bytes_send = 0U;

    BT_mem_set(send_file_stat.rfcomm_send_file_name, 0, sizeof(rfcomm_recv_file_name));
    BT_str_print(send_file_stat.rfcomm_send_file_name, "%s%s", BT_FOPS_BASE, BT_FOPS_PATH_SEP);

    /* Read the name of the file to store */
    printf("Enter File Name\n");
    scanf("%s", send_file_stat.rfcomm_send_file_name + BT_str_len(send_file_stat.rfcomm_send_file_name));

    retval = BT_fops_file_open
             (
                 (UCHAR *)send_file_stat.rfcomm_send_file_name,
                 (UCHAR *)"rb",
                 &rfcomm_send_fp
             );
    if(API_SUCCESS != retval)
    {
        printf("Failed to open file: %s\n",
        send_file_stat.rfcomm_send_file_name);

        /* return; */
    }
    else
    {
        rfcomm_send_file_operation = APPL_RFCOMM_FILE_SEND;

        /* Time in seconds from epoc */
        BT_get_current_time(&(send_file_stat.start_time));

        /* File will be written in a boot strapping fashion */
        (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(rfcomm_write_buffer,
                      APPL_RFCOMM_WRITE_BUF_SIZE,
                      rfcomm_send_fp, &rfcomm_read_len);

        if(rfcomm_read_len <= 0U)
        {
            printf("End of file reached\n");
            rfcomm_send_file_operation = APPL_RFCOMM_FILE_NOOP;

            BT_get_current_time(&(send_file_stat.end_time));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(rfcomm_send_fp);
            appl_print_file_transfer_stat();
        }
        else if(API_SUCCESS !=
                BT_rfcomm_write(&appl_rfcomm_handle,
                                rfcomm_write_buffer,
                                (UINT16)rfcomm_read_len, NULL))
        {
            printf("RFCOMM Write failed\n");

            /* Time in seconds from epoc */
            BT_get_current_time(&(send_file_stat.end_time));

            /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
            (void)BT_fops_file_close(rfcomm_send_fp);
            appl_print_file_transfer_stat();
        }
        else
        {
            send_file_stat.bytes_send += rfcomm_read_len;
        }
    }
#endif /* RFCOMM_FILE_OPERATION */
}


void appl_rfcomm_stop_recv_file (void)
{
#ifdef RFCOMM_FILE_OPERATION
    /* Read the name of the file to store */
    printf("Stop Receive Rx Data to File\n");

    rfcomm_recv_file_operation = APPL_RFCOMM_FILE_NOOP;

    if(NULL != rfcomm_recv_fp)
    {
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void)BT_fops_file_close(rfcomm_recv_fp);
    }

    rfcomm_recv_fp = NULL;
#endif /* RFCOMM_FILE_OPERATION */
}


void appl_print_file_transfer_stat (void)
{
#ifdef RFCOMM_FILE_OPERATION
    BT_time_type time_taken = 0U;
    printf("\n\n");
    printf("File Send: %s\n", send_file_stat.rfcomm_send_file_name);
    printf("File Size: %d\n", send_file_stat.bytes_send);

#if 0
    time_taken = send_file_stat.end_time - send_file_stat.start_time;
    printf("Time Taken is Second: %lu\n", time_taken);

    if(0 != time_taken)
    {
        printf("Transfer Rate: %lu(Bytes/Sec)\n",
               (send_file_stat.bytes_send) / time_taken);
    }
    else
    {
        printf("Transferred in the same second\n");
    }
#endif /* 0 */
#endif /* RFCOMM_FILE_OPERATION */

    BT_IGNORE_UNUSED_PARAM(time_taken);
}


void appl_rfcomm_session_open (void)
{
#ifdef RFCOMM_ENABLE_SUPPORT_APIS
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    printf("Enter BD_ADDR: ");
    appl_get_bd_addr(bd_addr);
    RFCOMM_SET_HANDLE(&appl_rfcomm_handle, bd_addr, 0x0U, appl_rfcomm_callback);

    printf("Initiating RFCOMM Session Open ... "); fflush (stdout);
    retval = bt_rfcomm_session_open (&appl_rfcomm_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for Session Event Indication.\n");
    }

#endif /* RFCOMM_ENABLE_SUPPORT_APIS */
}


void appl_rfcomm_session_close (void)
{
#ifdef RFCOMM_ENABLE_SUPPORT_APIS
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    printf("Enter BD_ADDR: ");
    appl_get_bd_addr(bd_addr);
    RFCOMM_SET_HANDLE(&appl_rfcomm_handle, bd_addr, 0x0U, appl_rfcomm_callback);

    printf("Initiating RFCOMM Session Close ... "); fflush (stdout);
    retval = bt_rfcomm_session_close (&appl_rfcomm_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for Session Event Indication.\n");
    }
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */
}


void appl_rfcomm_dlc_open (void)
{
#ifdef RFCOMM_ENABLE_SUPPORT_APIS
    API_RESULT retval;

    appl_rfcomm_handle.server_channel = 0x1U;
    printf("Enter BD_ADDR: ");
    appl_get_bd_addr(appl_rfcomm_handle.bd_addr);
    RFCOMM_SET_HANDLE(&appl_rfcomm_handle, appl_rfcomm_handle.bd_addr,
                      0x1U, appl_rfcomm_callback);

    printf("Initiating RFCOMM DLC Open ... "); fflush (stdout);
    retval = bt_rfcomm_dlc_open (&appl_rfcomm_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */
}


void appl_rfcomm_dlc_close (void)
{
#ifdef RFCOMM_ENABLE_SUPPORT_APIS
    API_RESULT retval;

    printf("Initiating RFCOMM DLC Close ... "); fflush (stdout);
    retval = bt_rfcomm_dlc_close (&appl_rfcomm_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */
}


void appl_rfcomm_session_fc_off (void)
{
#ifndef RFCOMM_NO_SEND_FC
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;
    RFCOMM_HANDLE local_handle;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

    printf("Enter BD_ADDR: ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr);
    RFCOMM_SET_HANDLE(&local_handle, bd_addr, 0x0U, appl_rfcomm_callback);

    printf("Initiating RFCOMM Session FC Off ... "); fflush (stdout);
    retval = BT_rfcomm_send_fc_off (&local_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for Session Event Indication.\n");
    }
#else
    printf("RFCOMM_NO_SEND_FC flag is defined\n");
#endif /* RFCOMM_NO_SEND_FC */
}


void appl_rfcomm_session_fc_on (void)
{
#ifndef RFCOMM_NO_SEND_FC

    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;
    RFCOMM_HANDLE local_handle;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

    printf("Enter BD_ADDR: ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr);
    RFCOMM_SET_HANDLE(&local_handle, bd_addr, 0x0U, appl_rfcomm_callback);

    printf("Initiating RFCOMM Session FC On ... "); fflush (stdout);
    retval = BT_rfcomm_send_fc_on (&local_handle);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for Session Event Indication.\n");
    }
#else
    printf("RFCOMM_NO_SEND_FC flag is defined\n");
#endif /* RFCOMM_NO_SEND_FC */
}


void appl_rfcomm_session_test (void)
{
#ifndef RFCOMM_NO_SEND_TEST
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;
    RFCOMM_HANDLE local_handle;

    int test_datalen;
    RFCOMM_TEST test_req;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

    printf("Test Data Length = "); fflush (stdout);
    scanf("%d", &test_datalen);
    if (0 > test_datalen)
    {
        printf("Invalid Data Length \n");
        /* return; */
    }
    else
    {
        test_req.datalen = (UINT16)test_datalen;
        test_req.data = (UCHAR *) BT_alloc_mem (test_req.datalen);
        if(NULL == test_req.data)
        {
            printf("*** FAILED to Allocate Memory for Test Data.\n");
            /* return; */
        }
        else
        {
            BT_mem_set(test_req.data, 0xABU, test_req.datalen);

            printf("Enter BD_ADDR: ");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(bd_addr);
            RFCOMM_SET_HANDLE(&local_handle, bd_addr, 0x0, appl_rfcomm_callback);

            printf("Initiating RFCOMM Session Test ... "); fflush(stdout);
            retval = BT_rfcomm_send_test(&local_handle, &test_req);
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for Session Event Indication.\n");
            }

            BT_free_mem(test_req.data);
        }
    }

#else
    printf("RFCOMM_NO_SEND_TEST flag is defined\n");
#endif /* RFCOMM_NO_SEND_TEST*/
}


void appl_rfcomm_send_dlc_pn (void)
{
#ifndef RFCOMM_NO_SEND_PN
    RFCOMM_PN pn;
    API_RESULT retval;

    BT_mem_set (&pn, 0x0, sizeof (RFCOMM_PN));

    pn.frame_size = 500U;
    appl_rfcomm_handle.server_channel = 0x1U;
    printf("Enter BD_ADDR: ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_rfcomm_handle.bd_addr);

    printf("Initiating RFCOMM Send PN ... "); fflush (stdout);
    retval = BT_rfcomm_send_pn (&appl_rfcomm_handle, &pn);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }

#else
    printf("RFCOMM_NO_SEND_PN flag is defined\n");
#endif /* RFCOMM_NO_SEND_PN */
}


void appl_rfcomm_send_dlc_rpn (void)
{
#ifndef RFCOMM_NO_SEND_RPN
    int choice;
    RFCOMM_RPN rpn;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;
    appl_rfcomm_handle.server_channel = 0x1U;

    printf("Enter BD_ADDR: ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_rfcomm_handle.bd_addr);
    BT_mem_set(&rpn, 0x0, sizeof(RFCOMM_RPN));

    printf("Choose RPN Option:\n");
    printf("  [1] RPN Request.\n");
    printf("  [2] RPN Command.\n");
    printf("Your Choice = "); fflush(stdout);
    scanf("%d", &choice);

    switch(choice)
    {
    case 1:
        rpn.request = RFCOMM_RPN_REQUEST_OPTION;
    break;

    case 2:
        rpn.baud_rate = RFCOMM_PORT_BAUD_RATE_230400;
        rpn.data_bits = RFCOMM_PORT_DATA_BITS_7;
        rpn.stop_bit = RFCOMM_PORT_STOP_BIT_1_5;
        rpn.parity = RFCOMM_PORT_PARITY_SET;
        rpn.parity_type = RFCOMM_PORT_PARITY_TYPE_MARK;
        rpn.flc = RFCOMM_PORT_FLC_X_ON_X_OFF_INPUT;
        rpn.x_on = 0xAAU;
        rpn.x_off = 0xBBU;
        rpn.request = RFCOMM_RPN_COMMAND_OPTION;
    break;

    default:
        printf("Invalid Option: %d\n", choice);
        retval = API_FAILURE;
        /* return; */
    }

    if (API_SUCCESS == retval)
    {
        printf("Initiating RFCOMM Send RPN ... "); fflush(stdout);
        retval = BT_rfcomm_send_rpn(&appl_rfcomm_handle, &rpn);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for DLC Event Indication.\n");
        }
    }

#else
    printf("RFCOMM_NO_SEND_RPN flag is defined\n");
#endif /* RFCOMM_NO_SEND_RPN */
}


void appl_rfcomm_send_dlc_msc (void)
{
#ifndef RFCOMM_NO_SEND_MSC
    RFCOMM_MSC msc;
    API_RESULT retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&msc, 0, sizeof(RFCOMM_MSC));

    msc.fc = 0x0U;

    printf("Initiating RFCOMM Send MSC ... "); fflush (stdout);
    retval = BT_rfcomm_send_msc (&appl_rfcomm_handle, &msc);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
#else
    printf("RFCOMM_NO_SEND_MSC flag is defined\n");
#endif /* RFCOMM_NO_SEND_MSC */
}


void appl_rfcomm_send_dlc_rls (void)
{
#ifndef RFCOMM_NO_SEND_RLS
    RFCOMM_RLS rls;
    API_RESULT retval;

    /**
     *    Remote Line Status Octet:
     *    +---------+---------+---------+---------+---------+---------+---------+---------+
     *    |  Bit 1  |  Bit 2  |  Bit 3  |  Bit 4  |  Bit 5  |  Bit 6  |  Bit 7  |  Bit 8  |
     *    +---------+---------+---------+---------+---------+---------+---------+---------+
     *    |    L1   |    L2   |    L3   |    L4   |   res   |   res   |   res   |   res   |
     *    +---------+---------+---------+---------+---------+---------+---------+---------+
     *
     *    L1 = 0 (No Error), L1 = 1 (Error)
     *
     *    L2-L4:
     *    100: Overrun Error - Received character overwrote an unread character
     *    010: Parity Error - Received character's parity was incorrect
     *    001: Framing Error - a character did not terminate with a stop bit
     */

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(&rls, 0, sizeof(RFCOMM_RLS));

    /**
     * PTS expects RLS Staus as Framing Error for an RLS related test case.
     * Updated the RLS value accordingly.
     */
    rls.line_status = 0x09U;
    printf("Initiating RFCOMM Send RLS ... "); fflush (stdout);
    retval = BT_rfcomm_send_rls (&appl_rfcomm_handle, &rls);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
        printf("Wait for DLC Event Indication.\n");
    }
#else
    printf("RFCOMM_NO_SEND_RLS flag is defined\n");
#endif /* RFCOMM_NO_SEND_RLS */
}

#ifdef RFCOMM_ENABLE_FC_FROM_APP
void appl_rfcomm_enable_credit_tx(void)
{
    API_RESULT retval;
    UCHAR      flag;

    appl_rfcomm_handle.server_channel = 0x1U;
    printf("Enter BD_ADDR: ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_rfcomm_handle.bd_addr);

    printf("Enabling RFCOMM Credit Tx ... "); fflush(stdout);
    flag = RFCOMM_CREDIT_TX_ENABLED;
    retval = BT_rfcomm_credit_tx(&appl_rfcomm_handle, flag);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
    }
}

void appl_rfcomm_disable_credit_tx(void)
{
    API_RESULT retval;
    UCHAR      flag;

    appl_rfcomm_handle.server_channel = 0x1U;
    printf("Enter BD_ADDR: ");
    (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(appl_rfcomm_handle.bd_addr);

    printf("Disabling RFCOMM Credit Tx ... "); fflush(stdout);
    flag = RFCOMM_CREDIT_TX_DISABLED;
    retval = BT_rfcomm_credit_tx(&appl_rfcomm_handle, flag);
    if (API_SUCCESS != retval)
    {
        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
    }
    else
    {
        printf("Succeeded !!\n");
    }
}
#endif /* RFCOMM_ENABLE_FC_FROM_APP */

API_RESULT appl_rfcomm_callback
           (
               UCHAR   event_type,
               RFCOMM_HANDLE *  handle,
               UINT16  result,
               UCHAR * data,
               UINT16  datalen
           )
{
    API_RESULT retval;
    UCHAR *bd_addr = handle->bd_addr;
    RFCOMM_RPN rpn;
    /* UINT8 index; */

    /* Init */
    retval = API_SUCCESS;

    if (API_SUCCESS != result)
    {
        printf("*** Received Result = 0x%04x, Notify Event Type = 0x%02x\n",
        result, event_type);
        appl_rfcomm_print_handle(handle);

        /* return API_SUCCESS; */
    }
    else
    {
        switch (event_type)
        {
#ifdef RFCOMM_ENABLE_SUPPORT_APIS
        case RFCOMM_SESSION_OPEN:
            printf("Received Complete for RFCOMM Session Open => 0x%04x\n",
            result);
            printf("    Remote " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
            break;

        case RFCOMM_SESSION_CLOSE:
            printf("Received Complete for RFCOMM Session Close => 0x%04x\n",
            result);
            printf("    Remote " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
            break;
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */

        case RFCOMM_SEND_FC_OFF:
            printf("Received Complete for RFCOMM Session FC Off => 0x%04x\n",
            result);
            printf("    Remote " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (bd_addr));
            break;

        case RFCOMM_SEND_FC_ON:
            printf("Received Complete for RFCOMM Session FC On => 0x%04x\n",
            result);
            printf("    Remote " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR(bd_addr));
            break;

        case RFCOMM_SEND_TEST:
            printf("Received Complete for RFCOMM Session Test => 0x%04X\n",
            result);
            printf("    Remote " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR(bd_addr));
            break;

        case RFCOMM_OPEN:
            printf("Received Complete for RFCOMM Open => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            BT_mem_copy (appl_rfcomm_handle.bd_addr, handle->bd_addr, 6U);
            appl_rfcomm_handle.server_channel = handle->server_channel;
            appl_rfcomm_handle.dlci = handle->dlci;

            rfcomm_send_file_operation = APPL_RFCOMM_FILE_NOOP;
            rfcomm_recv_file_operation = APPL_RFCOMM_FILE_NOOP;

            break;

        case RFCOMM_CLOSE:
            printf("Received Complete for RFCOMM Close => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            BT_mem_set (&appl_rfcomm_handle, 0x0, sizeof (RFCOMM_HANDLE));
            break; /* return API_SUCCESS; */

        case RFCOMM_ACCEPT:
            printf("Received Complete for RFCOMM Accept => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            BT_mem_copy (appl_rfcomm_handle.bd_addr, handle->bd_addr, 6U);
            appl_rfcomm_handle.server_channel = handle->server_channel;
            appl_rfcomm_handle.dlci = handle->dlci;

            rfcomm_send_file_operation = APPL_RFCOMM_FILE_NOOP;
            rfcomm_recv_file_operation = APPL_RFCOMM_FILE_NOOP;

            break;

#ifdef RFCOMM_ENABLE_SUPPORT_APIS
        case RFCOMM_DLC_OPEN:
            printf("Received Complete for RFCOMM DLC Open => 0x%04x\n", result);
            BT_mem_copy
            (
                appl_rfcomm_handle.bd_addr,
                handle->bd_addr,
                BT_BD_ADDR_SIZE
            );

            appl_rfcomm_handle.server_channel = handle->server_channel;
            appl_rfcomm_handle.dlci = handle->dlci;
            break;

        case RFCOMM_DLC_CLOSE:
            printf("Received Complete for RFCOMM DLC Close => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            BT_mem_set (&appl_rfcomm_handle, 0x0, sizeof (RFCOMM_HANDLE));
            return API_SUCCESS;
#endif /* RFCOMM_ENABLE_SUPPORT_APIS */

        case RFCOMM_SEND_PN:
            printf("Received Complete for RFCOMM Send PN => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            appl_rfcomm_handle.dlci = handle->dlci;
            break;

        case RFCOMM_SEND_RPN:
            printf("Received Complete for RFCOMM Send RPN => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            appl_rfcomm_handle.dlci = handle->dlci;
            if ((NULL != data) && (sizeof(RFCOMM_RPN) == datalen))
            {
                BT_mem_copy(&rpn, data, sizeof(RFCOMM_RPN));
                appl_rfcomm_print_rpn (&rpn);
            }
            else
            {
                printf("*** Did NOT Receive RPN Correctly.\n");
            }
            break;

        case RFCOMM_SEND_MSC:
            printf("Received Complete for RFCOMM Send MSC => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            break;

        case RFCOMM_SEND_RLS:
            printf("Received Complete for RFCOMM Send RLS => 0x%04x\n", result);
            appl_rfcomm_print_handle(handle);
            break;

        case RFCOMM_READ:
#ifdef RFCOMM_FILE_OPERATION
            if (APPL_RFCOMM_FILE_RECV == rfcomm_recv_file_operation)
            {
                UINT16 written;

                /* File will be read in a boot strapping fashion */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, datalen, rfcomm_recv_fp, &written);

                printf (". ");
            }
            else
#endif /* RFCOMM_FILE_OPERATION */
            {
#ifndef APPL_LIMIT_LOGS
                UINT16 len;

                printf("Received RFCOMM Data Read IND.\n");
                appl_rfcomm_print_handle(handle);
                printf("    Data   = %p\n", data);
                printf("    Length = %d\n", datalen);
                printf("\n");
#else
                printf(".");
#endif /* APPL_LIMIT_LOGS */


        #if 0
                printf("\n----------------CHAR DUMP-----------------------\n");
                for(index = 0; index <datalen; index++ )
                {
                    printf("%c ",data[index]);
                }
                printf("\n------------------------------------------------\n");
                printf("\n----------------HEX DUMP------------------------\n");
                for(index = 0; index <datalen; index++ )
                {
                    printf("%02X ",data[index]);
                }
                printf("\n------------------------------------------------\n");
        #else /* 0 */
#ifndef APPL_LIMIT_LOGS
                len = (datalen > APPL_RFCOMM_DUMP_CHARS_MAX_LENGTH) ? APPL_RFCOMM_DUMP_CHARS_MAX_LENGTH : datalen;
                printf("Received data length is %d, the first %d data is dumped here.\n", datalen, len);
                appl_dump_bytes(data, len);
#endif /* APPL_LIMIT_LOGS */
        #endif /* 0 */
            }
            break;

        case RFCOMM_WRITE:
#ifdef RFCOMM_FILE_OPERATION
            if (APPL_RFCOMM_FILE_SEND == rfcomm_send_file_operation)
            {
                /* File will be written in a boot strapping fashion */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_read(rfcomm_write_buffer,
                      APPL_RFCOMM_WRITE_BUF_SIZE,
                      rfcomm_send_fp, &rfcomm_read_len);

                if(rfcomm_read_len <= 0U)
                {
                    printf("End of file reached\n");
                    rfcomm_send_file_operation = APPL_RFCOMM_FILE_NOOP;

                    BT_get_current_time(&(send_file_stat.end_time));

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close(rfcomm_send_fp);
                    appl_print_file_transfer_stat();
                }
                else if(API_SUCCESS !=
                        BT_rfcomm_write(&appl_rfcomm_handle,
                                        rfcomm_write_buffer,
                                        (UINT16)rfcomm_read_len, NULL))
                {
                    printf("RFCOMM Write failed\n");
                    BT_get_current_time(&(send_file_stat.end_time));

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close(rfcomm_send_fp);
                    appl_print_file_transfer_stat();
                }
                else
                {
                    printf(". ");
                    send_file_stat.bytes_send += rfcomm_read_len;
                }
            }
            else
#endif /* RFCOMM_FILE_OPERATION */
            {
                UCHAR * buffer;
                UINT16 actual;
                /* API_RESULT retval; */

#ifdef RFCOMM_ENABLE_DATA_WRITE_CNF_LOG
                printf("Received RFCOMM Data Write CNF. Result = 0x%04x\n", result);
                appl_rfcomm_print_handle(handle);
                printf("    Buffer = %p\n", data);
                printf("    Actual = %d\n", datalen);
#else /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
                printf(". ");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */

                BT_free_mem(data);
                appl_count++;

                if (appl_count == appl_loop)
                {
                    printf("\n*** RFCOMM Write for %d times has completed!!!\n", appl_loop);
                    break;
                }

                /* Send next chunk */
                buffer = (UCHAR *)BT_alloc_mem(appl_nbytes);
                if (NULL == buffer)
                {
                    printf("*** Buffer Allocation FAILED. !!\n");
                    break;
                }

                /* Set the appl data */
                if (2U > appl_nbytes)
                {
                    BT_mem_set(buffer, appl_count, appl_nbytes);
                }
                else
                {
                    BT_PACK_BE_2_BYTE_VAL(buffer, appl_count);
                    BT_mem_set((&buffer[2U]), appl_count, (appl_nbytes - 2U));
                }

#ifdef RFCOMM_ENABLE_DATA_WRITE_CNF_LOG
                printf("Allocated Buffer = %p\n", buffer);
                printf("Initiating [%d]th RFCOMM Channel Write ... ", appl_count);
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */

                retval = BT_rfcomm_write(&appl_rfcomm_handle, buffer, appl_nbytes, &actual);
                if (API_SUCCESS != retval)
                {
                    printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                    BT_free_mem(buffer);
                }
                else
                {
#ifdef RFCOMM_ENABLE_DATA_WRITE_CNF_LOG
                    printf("Succeeded !!\n");
                    printf("Wait for Write Complete.\n");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
                }
            }

            break;

        case RFCOMM_RECVD_MSC:
            printf("Received RFCOMM MSC.\n");
            appl_rfcomm_print_handle(handle);
            break;

        case RFCOMM_RECVD_RLS:
            printf("Received RFCOMM RLS.\n");
            appl_rfcomm_print_handle(handle);
            break;

        case RFCOMM_RESET:
            printf("Received RFCOMM Reset => 0x%04X\n", result);
            appl_rfcomm_print_handle(handle);
            BT_mem_set (&appl_rfcomm_handle, 0x0, sizeof (RFCOMM_HANDLE));
            break;

        default:
            printf("*** Received Notify Event Type = 0x%02x, Result = 0x%04x\n",
            event_type, result);
            break;
        }
    }

    return retval;
}


void appl_rfcomm_print_rpn (RFCOMM_RPN *rpn)
{
    printf("DLCI       : 0x%02X\n", rpn->dlci);

    printf("Request    : 0x%02X -> ", rpn->request);
    switch (rpn->request)
    {
    case RFCOMM_RPN_REQUEST_OPTION:
        printf("Request\n");
        break;
    case RFCOMM_RPN_COMMAND_OPTION:
        printf("Command\n");
        break;
    case RFCOMM_RPN_RESPONSE_OPTION:
        printf("Response\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("Baud Rate  : 0x%02X -> ", rpn->baud_rate);
    switch (rpn->baud_rate)
    {
    case RFCOMM_PORT_BAUD_RATE_2400:
        printf("2400\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_4800:
        printf("4800\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_7200:
        printf("7200\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_9600:
        printf("9600 (Default)\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_19200:
        printf("19200\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_38400:
        printf("38400\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_57600:
        printf("57600\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_115200:
        printf("115200\n");
        break;
    case RFCOMM_PORT_BAUD_RATE_230400:
        printf("230400\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("Data Bits  : 0x%02X -> ", rpn->data_bits);
    switch (rpn->data_bits)
    {
    case RFCOMM_PORT_DATA_BITS_5:
        printf("5\n");
        break;
    case RFCOMM_PORT_DATA_BITS_6:
        printf("6\n");
        break;
    case RFCOMM_PORT_DATA_BITS_7:
        printf("7\n");
        break;
    case RFCOMM_PORT_DATA_BITS_8:
        printf("8 (Default)\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("Stop Bit   : 0x%02X -> ", rpn->stop_bit);
    switch (rpn->stop_bit)
    {
    case RFCOMM_PORT_STOP_BIT_1:
        printf("1 (Default)\n");
        break;
    case RFCOMM_PORT_STOP_BIT_1_5:
        printf("1/5\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("Parity     : 0x%02X -> ", rpn->parity);
    switch (rpn->parity)
    {
    case RFCOMM_PORT_PARITY_NONE:
        printf("None (Default)\n");
        break;
    case RFCOMM_PORT_PARITY_SET:
        printf("Set\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("Parity Type: 0x%02X -> ", rpn->parity_type);
    switch (rpn->parity_type)
    {
    case RFCOMM_PORT_PARITY_TYPE_ODD:
        if (RFCOMM_PORT_PARITY_NONE == rpn->parity)
        {
            printf("None (Default)\n");
        }
        else
        {
            printf("Odd\n");
        }
        break;
    case RFCOMM_PORT_PARITY_TYPE_EVEN:
        printf("Even\n");
        break;
    case RFCOMM_PORT_PARITY_TYPE_MARK:
        printf("Mark\n");
        break;
    case RFCOMM_PORT_PARITY_TYPE_SPACE:
        printf("Space\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("FLC        : 0x%02X -> ", rpn->flc);
    switch (rpn->flc)
    {
    case 0x00U:
        printf("None (Default)\n");
        break;
    case RFCOMM_PORT_FLC_X_ON_X_OFF_INPUT:
        printf("X-On/Off (Input)\n");
        break;
    case RFCOMM_PORT_FLC_X_ON_X_OFF_OUTPUT:
        printf("X-On/Off (Output)\n");
        break;
    case RFCOMM_PORT_FLC_RTR_INPUT:
        printf("RTR (Input)\n");
        break;
    case RFCOMM_PORT_FLC_RTR_OUTPUT:
        printf("RTR (Output)\n");
        break;
    case RFCOMM_PORT_FLC_RTC_INPUT:
        printf("RTC (Input)\n");
        break;
    case RFCOMM_PORT_FLC_RTC_OUTPUT:
        printf("RTC (Output)\n");
        break;
    default:
        printf("????\n");
        break;
    }

    printf("XON        : 0x%02X\n", rpn->x_on);
    printf("XOFF       : 0x%02X\n", rpn->x_off);
    printf("Mask[0]    : 0x%02X\n", rpn->mask[0U]);
    printf("Mask[1]    : 0x%02X\n", rpn->mask[1U]);
}


void appl_rfcomm_print_handle (RFCOMM_HANDLE *handle)
{
    printf("RFCOMM Handle ===>\n");
    printf("    " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
    BT_DEVICE_ADDR_ONLY_PRINT_STR (handle->bd_addr));
    printf("    Channel: 0x%02X (%d)\n",
    handle->server_channel, handle->server_channel);
    printf("    DLCI   : 0x%02X\n",
    handle->dlci);

#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
    /*
     * In this sample application, user context in RFCOMM HANDLE
     * is given as string.
     *
     * If the user context is provided as a specific value like
     * lookup table index etc. the below print also need to updated
     * accordingly.
     */
    if (NULL != handle->blob)
    {
        printf ("    BLOB   : %s\n",
        (UCHAR *)handle->blob);
    }
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */
}

#endif /* RFCOMM */
