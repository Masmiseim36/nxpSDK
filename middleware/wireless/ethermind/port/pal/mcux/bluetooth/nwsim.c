
/**
 *  \file nwsim.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "nwsim.h"
#include "sco_audio_pl.h"
#include "BT_at_parser_api.h"

/* --------------------------------------------- Global Definitions */
/** Constant Response Strings */
#define NWSIM_OK_STR           "\r\nOK\r\n"
#define NWSIM_ERROR_STR        "\r\nERROR\r\n"
#define NWSIM_NOCARRIER_STR    "\r\nNO CARRIER\r\n"
#define NWSIM_BUSY_STR         "\r\nBUSY\r\n"
#define NWSIM_NOANSWER_STR     "\r\nNO ANSWER\r\n"
#define NWSIM_RING_STR         "\r\nRING\r\n"
#define NWSIM_CIND_TEST_STR    "\r\n+CIND: (\"service\",(0-1)), (\"call\",(0-1)), (\"callsetup\",(0-3)), (\"callheld\",(0-2)), (\"signal\",(0-5)), (\"roam\",(0-1)), (\"battchg\",(0-5))\r\n"
#define NWSIM_CHLD_TEST_STR    "\r\n+CHLD: (0,1,2,3,4)\r\n"
#define NWSIM_COPS_READ_STR    "\r\n+COPS: 0,0,\"EtherSIM\"\r\n"
#define NWSIM_CNUM_STR         "\r\n+CNUM: ,\"918067064000\",143,,4\r\n"

/* Variable Response Strings */
#define NWSIM_CLIP_STR         "\r\n+CLIP: "
#define NWSIM_CCWA_STR         "\r\n+CCWA: "
#define NWSIM_CIND_READ_STR    "\r\n+CIND: "
#define NWSIM_SERVICE_STR      "\r\n+CIEV: 1,"
#define NWSIM_CALL_STR         "\r\n+CIEV: 2,"
#define NWSIM_CALLSETUP_STR    "\r\n+CIEV: 3,"
#define NWSIM_CALLHELD_STR     "\r\n+CIEV: 4,"
#define NWSIM_SIGNAL_STR       "\r\n+CIEV: 5,"
#define NWSIM_ROAMING_STR      "\r\n+CIEV: 6,"
#define NWSIM_BATTERY_STR      "\r\n+CIEV: 7,"
#define NWSIM_CME_ERROR_STR    "\r\n+CME ERROR: "
#define NWSIM_CLCC_STR         "\r\n+CLCC: "

#define NWSIM_BTRH_STR         "\r\n+BTRH: "

/** Response code defines */
#define NWSIM_OK                0U
#define NWSIM_ERROR             1U
#define NWSIM_NOCARRIER         2U
#define NWSIM_BUSY              3U
#define NWSIM_NOANSWER          4U
#define NWSIM_RING              5U
#define NWSIM_CIND_TEST         6U
#define NWSIM_CHLD_TEST         7U
#define NWSIM_COPS_READ         8U
#define NWSIM_CLIP              9U
#define NWSIM_CCWA              10U
#define NWSIM_CIND_READ         11U
#define NWSIM_CALL              12U
#define NWSIM_CALLSETUP         13U
#define NWSIM_SERVICE           14U
#define NWSIM_SIGNAL            15U
#define NWSIM_ROAMING           16U
#define NWSIM_BATTERY           17U
#define NWSIM_CALLHELD          18U
#define NWSIM_CNUM              19U
#define NWSIM_CME_ERROR         20U
#define NWSIM_CLCC              21U

#define NWSIM_BTRH              22U

#define NWSIM_MAX_CALLS         3U

#define NWSIM_CIEV_SERVICE      (1U << 1U)
#define NWSIM_CIEV_CALL         (1U << 2U)
#define NWSIM_CIEV_CALLSETUP    (1U << 3U)
#define NWSIM_CIEV_CALLHELD     (1U << 4U)
#define NWSIM_CIEV_SIGNAL       (1U << 5U)
#define NWSIM_CIEV_ROAM         (1U << 6U)
#define NWSIM_CIEV_BATTERY      (1U << 7U)

#define NWSIM_CALLSIDLE          0x00U
#define NWSIM_CALLSINCOMING      0x01U
#define NWSIM_CALLSOUTGOING      0x02U
#define NWSIM_CALLSACTIVE        0x03U
#define NWSIM_CALLSWAITING       0x04U
#define NWSIM_CALLSHELD          0x08U

#define NWSIM_SETCALLSTATEIDLE() \
        nwsim_callstate &= (UCHAR)(~NWSIM_CALLSACTIVE)

#define NWSIM_SETCALLSTATE(state) \
        nwsim_callstate |= (state)

#define NWSIM_CALLSTATE() \
        (nwsim_callstate & 0x03U)

#define NWSIM_ISCALLSTATE(state) \
        ((state) == (nwsim_callstate & 0x03U))

#define NWSIM_SETCALLWAITING() \
        nwsim_callstate |= NWSIM_CALLSWAITING

#define NWSIM_RESETCALLWAITING() \
        nwsim_callstate &= (UCHAR)(~NWSIM_CALLSWAITING)

#define NWSIM_ISCALLWAITING() \
        (nwsim_callstate & NWSIM_CALLSWAITING)

#define NWSIM_SETCALLHELD() \
        nwsim_callstate |= NWSIM_CALLSHELD

#define NWSIM_RESETCALLHELD() \
        nwsim_callstate &= (UCHAR)(~NWSIM_CALLSHELD)

#define NWSIM_ISCALLHELD() \
        (nwsim_callstate & NWSIM_CALLSHELD)

#define NWSIM_INCOMING_ACCEPT           0x00U
#define NWSIM_INCOMING_END_LOCAL        0x01U
#define NWSIM_INCOMING_END_REMOTE       0x02U

#define NWSIM_OUTGOING_ACCEPT           0x03U
#define NWSIM_OUTGOING_END_LOCAL        0x04U
#define NWSIM_OUTGOING_END_REMOTE       0x05U

/* --------------------------------------------- External Global Variables */
/* Response lookup table */
DECL_STATIC CHAR * nwsim_rsp_table[] = {
    NWSIM_OK_STR,
    NWSIM_ERROR_STR,
    NWSIM_NOCARRIER_STR,
    NWSIM_BUSY_STR,
    NWSIM_NOANSWER_STR,
    NWSIM_RING_STR,
    NWSIM_CIND_TEST_STR,
    NWSIM_CHLD_TEST_STR,
    NWSIM_COPS_READ_STR,
    NWSIM_CLIP_STR,
    NWSIM_CCWA_STR,
    NWSIM_CIND_READ_STR,
    NWSIM_CALL_STR,
    NWSIM_CALLSETUP_STR,
    NWSIM_SERVICE_STR,
    NWSIM_SIGNAL_STR,
    NWSIM_ROAMING_STR,
    NWSIM_BATTERY_STR,
    NWSIM_CALLHELD_STR,
    NWSIM_CNUM_STR,
    NWSIM_CME_ERROR_STR,
    NWSIM_CLCC_STR,
    NWSIM_BTRH_STR
};

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/** Network parameters */
DECL_STATIC UCHAR nwsim_signal;
DECL_STATIC UCHAR nwsim_battery;
DECL_STATIC UCHAR nwsim_service;
DECL_STATIC UCHAR nwsim_roaming;
DECL_STATIC UINT16 nwsim_sf;

DECL_STATIC UCHAR nwsim_call;
DECL_STATIC UCHAR nwsim_callsetup;
DECL_STATIC UCHAR nwsim_callheld;

DECL_STATIC UCHAR nwsim_callcount;
DECL_STATIC NWSIM_CALLINFO nwsim_callinfo[NWSIM_MAX_CALLS];
DECL_STATIC UCHAR nwsim_setupcall_id;
DECL_STATIC UCHAR nwsim_activecall_id;
DECL_STATIC UCHAR nwsim_heldcall_id;
DECL_STATIC UCHAR nwsim_callstate;
DECL_STATIC UCHAR nwsim_conference;

DECL_STATIC UCHAR nwsim_cmer;
DECL_STATIC UCHAR nwsim_clip;
DECL_STATIC UCHAR nwsim_ccwa;
DECL_STATIC UCHAR nwsim_cops;
DECL_STATIC UINT32 nwsim_ciev;

DECL_STATIC UCHAR nwsim_btrh_status;

DECL_STATIC CHAR nwsim_clipnum[16U];
DECL_STATIC CHAR nwsim_ccwanum[16U];

DECL_STATIC BT_timer_handle nwsim_ring_timer;
DECL_STATIC BT_timer_handle nwsim_ccwa_timer;
DECL_STATIC BT_timer_handle nwsim_outgoing_ring_timer;
static const char nwsim_menu[] = "\n\
-------- NWSim Menu ------- \n\
    0.  Exit \n\
    1.  Refresh \n\
    2.  Reset NWSim \n\
 \n\
    5.  Setup Incoming Call \n\
    6.  Accept Incoming Call local \n\
    7.  Reject Incoming Call local \n\
    8.  Hangup Incoming Call remote (Not ACTIVE Call) \n\
 \n\
   15.  Setup Outgoing Call \n\
   16.  Accept Outgoing Call remote \n\
   17.  Reject Outgoing Call remote \n\
   18.  Hangup Outgoing Call local (Not ACTIVE Call)\n\
 \n\
   25.  Setup 3-Way Incoming Call \n\
   26.  Accept 3-Way Incoming Call local \n\
   27.  Reject 3-Way Incoming Call local \n\
   28.  Hangup 3-Way Incoming Call remote \n\
 \n\
   35.  Setup 3-Way Outgoing Call \n\
   36.  Accept 3-Way Outgoing Call remote \n\
   37.  Reject 3-Way Outgoing Call remote \n\
   38.  Hangup 3-Way Outgoing Call local \n\
 \n\
   40.  Hangup Active/Held Call \n\
   41.  Conference Calls \n\
   42.  Operate Response n Hold \n\
 \n\
   45.  Update Signal Strength \n\
   46.  Update Battery Charge \n\
   47.  Update Service Availability \n\
   48.  Update Roaming Status \n\
 \n\
   50.  No Carrier \n\
 \n\
   Enter your choice: ";

static void (* nwsim_rsp_iface)(UCHAR *rsp, UINT16 rsplen);
static void (* nwsim_callstatus_iface)(UCHAR status);

/* --------------------------------------------- Functions */
void nwsim_reset(void)
{
    UINT8 index;

    /* Initialize the network parameters */
    nwsim_signal = 0x05U;
    nwsim_battery = 0x05U;
    nwsim_service = 0x01U;
    nwsim_roaming = 0x00U;

    nwsim_call = 0x00U;
    nwsim_callsetup = 0x00U;
    nwsim_callheld = 0x00U;
    nwsim_conference = 0x00U;

    nwsim_callcount = 0U;
    BT_mem_set(nwsim_callinfo, 0, sizeof(nwsim_callinfo));

    for (index = 0U; index < NWSIM_MAX_CALLS; index++)
    {
        nwsim_callinfo[index].status = 0xFFU;
    }

    NWSIM_SETCALLSTATEIDLE();
}

void nwsim_init
     (
         void (* read_cb)(UCHAR *rsp, UINT16 rsplen),
         void (* call_control_cb)(UCHAR status)
     )
{
    static UCHAR done;

    /* Register callbacks */
    nwsim_rsp_iface = read_cb;
    nwsim_callstatus_iface = call_control_cb;

    if (0x00U == done)
    {
        /* Initialize the network parameters */
        nwsim_reset();

        done = 0x01U;
    }

    return;
}

void nwsim_start (UINT16 sf)
{
    /* TODO: Configure for the local supported features */
    nwsim_sf = sf;

    /* Initialize the session parameters */
    nwsim_clip = 0x00U;
    nwsim_ccwa = 0x00U;
    nwsim_ciev = 0x00U;
    nwsim_cops = 0x00U;

    nwsim_btrh_status = 0x00U;

    nwsim_ciev = (NWSIM_CIEV_CALL | NWSIM_CIEV_CALLSETUP | NWSIM_CIEV_CALLHELD |
        NWSIM_CIEV_SERVICE | NWSIM_CIEV_SIGNAL | NWSIM_CIEV_ROAM | NWSIM_CIEV_BATTERY);
}

void nwsim_stop(void)
{
    /* TODO */
    /* MISRA C - 2012 Rule 2.2 */
    NWSIM_TRC("nwsim_stop\n");

    return;
}

void nwsim_command_send (UCHAR * cmd, UINT16 cmdlen)
{
    AT_PARSER_RESPONSE at_response;
    UINT16 start_index, buffer_size, length, retval;
    UCHAR index, option;
    CHAR *ptr;

    /* Call the AT parser to parse the AT command sent from unit */
    length = cmdlen;
    buffer_size = cmdlen;
    start_index = 0U;

    retval = BT_ag_at_parser
             (
                 cmd,
                 start_index,
                 buffer_size,
                 &length,
                 &at_response
             );

    /* Handle the AT commands */
    if (API_SUCCESS != retval)
    {
        NWSIM_ERR ("Failed to parse command - 0x%04X\n", retval);
        return;
    }

    /* Handle the commands */
    switch (at_response.keyword_type)
    {
        case AT_CIND_TEST:
            nwsim_send_rsp (NWSIM_CIND_TEST, NULL);
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_CIND_READ:
            nwsim_send_rsp (NWSIM_CIND_READ, NULL);
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_CHLD_TEST:
            nwsim_send_rsp (NWSIM_CHLD_TEST, NULL);
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_CMEE:
            nwsim_send_rsp(NWSIM_OK, NULL);
            break;

        case AT_CMER:
            {
                UCHAR p[4U];

                /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
                BT_mem_set(p, 0, sizeof(p));

                ptr = (CHAR *)&at_response.global_at_str[at_response.param->start_of_value_index];
                index = 0U;
                while ('\r' != *ptr)
                {
                    if (',' == *ptr)
                    {
                        index++;
                    }
                    else if (' ' != *ptr)
                    {
                        p[index] = (*ptr - '0');
                    }
                    else
                    {
                        /* MISRA C-2012 Rule 15.7 */
                    }

                    ptr++;
                }

                if ((3U == p[0U]) && (1U == p[3U]))
                {
                    nwsim_cmer = 1U;
                }
                else
                {
                    nwsim_cmer = 0U;
                }

                nwsim_send_rsp (NWSIM_OK, NULL);

                /* Notify for voice if call active */
                if ((nwsim_call) || (0U != nwsim_callsetup))
                {
                    nwsim_callstatus_iface(0x01U);
                }
            }
            break;

        case ATA:
            nwsim_send_rsp(NWSIM_OK, NULL);
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_INCOMING_ACCEPT);
            break;

        case AT_CHUP:
            nwsim_send_rsp(NWSIM_OK, NULL);
            (BT_IGNORE_RETURN_VALUE) nwsim_end_call();
            break;

        case ATD:
            nwsim_send_rsp(NWSIM_OK, NULL);
            index = (UCHAR)BT_str_len((CHAR *)(at_response.global_at_str + at_response.param->start_of_value_index));
            index -= 2U;
            (at_response.global_at_str + at_response.param->start_of_value_index)[index] = '\0';
            (BT_IGNORE_RETURN_VALUE) nwsim_dial((CHAR *)&at_response.global_at_str[at_response.param->start_of_value_index]);
            break;

        case AT_CCWA:
            nwsim_ccwa = at_response.global_at_str[at_response.param->start_of_value_index] - '0';
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_COPS:
            if (('3' == at_response.global_at_str[at_response.param->start_of_value_index]) &&
                ('0' == at_response.global_at_str[at_response.param->start_of_value_index + 2U]))
            {
                nwsim_cops = 0x01U;
                nwsim_send_rsp(NWSIM_OK, NULL);
            }
            else
            {
                nwsim_send_rsp (NWSIM_ERROR, NULL);
            }
            break;

        case AT_COPS_READ:
            if (0x01U == nwsim_cops)
            {
                nwsim_send_rsp(NWSIM_COPS_READ, NULL);
                nwsim_send_rsp(NWSIM_OK, NULL);
            }
            else
            {
                nwsim_send_rsp(NWSIM_ERROR, NULL);
            }
            break;

        case AT_CLIP:
            nwsim_clip = at_response.global_at_str[at_response.param->start_of_value_index] - '0';
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_CHLD:
            option = at_response.global_at_str[at_response.param->start_of_value_index] - '0';
            index = at_response.global_at_str[at_response.param->start_of_value_index + 1U];

            if ((index != '\r') && !(nwsim_sf & 0x0080U))
            {
                nwsim_send_rsp(NWSIM_ERROR, NULL);
                break;
            }

            nwsim_send_rsp(NWSIM_OK, NULL);
            index = (index == '\r') ? 0U : index - '0';
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(option, index);
            break;

        case AT_CNUM:
            nwsim_send_rsp (NWSIM_CNUM, NULL);
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_CLCC:
            for (index = 0U; index < NWSIM_MAX_CALLS; index++)
            {
                if (0xFFU != nwsim_callinfo[index].status)
                {
                    nwsim_send_rsp (NWSIM_CLCC, &index);
                }
            }
            nwsim_send_rsp (NWSIM_OK, NULL);
            break;

        case AT_BTRH:
            retval = nwsim_handle_btrh(at_response.global_at_str[at_response.param->start_of_value_index] - '0');
            if (API_SUCCESS == retval)
            {
                nwsim_send_rsp(NWSIM_OK, NULL);
            }
            else
            {
                nwsim_send_rsp(NWSIM_ERROR, NULL);
            }
            break;

        case AT_VTS:
            nwsim_send_rsp(NWSIM_OK, NULL);
            break;

        case AT_BTRH_READ:
            nwsim_send_rsp(NWSIM_BTRH, &nwsim_btrh_status);
            nwsim_send_rsp(NWSIM_OK, NULL);
            break;

        case AT_BIA:
            ptr = (CHAR *)&at_response.global_at_str[at_response.param->start_of_value_index];
            index = 1U;
            while ('\r' != *ptr)
            {
                if ((',' == *ptr) ||
                    (1U < (*ptr - '0')))
                {
                    index++;
                }
                else if (1U == (*ptr - '0'))
                {
                    /* Update the indicator state */
                    nwsim_ciev |= (UINT32)(1U << index);
                }
                else if (0U == (*ptr - '0'))
                {
                    /* Update the indicator state */
                    nwsim_ciev &= (UINT32)(~(1U << index));
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                ptr++;
            }

            printf("NWSIM Indicators: 0x%08X\n", nwsim_ciev);
            nwsim_send_rsp(NWSIM_OK, NULL);
            break;

        default:
            nwsim_send_rsp(NWSIM_ERROR, NULL);
            break;
    }
}

void nwsim_operations (void)
{
    int choice;

    BT_LOOP_FOREVER()
    {
        printf("%s\n", nwsim_menu);
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            return;

        case 1:
            break;

        case 2:
            nwsim_reset();
            break;

        case 5:
            (BT_IGNORE_RETURN_VALUE) nwsim_setup_incoming_call("1234567");
            break;

        case 6:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_INCOMING_ACCEPT);
            break;

        case 7:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_INCOMING_END_LOCAL);
            break;

        case 8:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_INCOMING_END_REMOTE);
            break;

        case 15:
            (BT_IGNORE_RETURN_VALUE) nwsim_setup_outgoing_call("1234567");
            break;

        case 16:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_OUTGOING_ACCEPT);
            break;

        case 17:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_OUTGOING_END_REMOTE);
            break;

        case 18:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_OUTGOING_END_LOCAL);
            break;

        case 25:
            (BT_IGNORE_RETURN_VALUE) nwsim_setup_twc_incoming("7654321");
            break;

        case 26:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(2U, 0U);
            break;

        case 27:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(0U, 0U);
            break;

        case 28:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(0U, 0U);
            break;

        case 35:
            (BT_IGNORE_RETURN_VALUE) nwsim_setup_twc_outgoing("7654321");
            break;

        case 36:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(2U, 0U);
            break;

        case 37:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(0U, 0U);
            break;

        case 38:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(0U, 0U);
            break;

        case 40:
            (BT_IGNORE_RETURN_VALUE) nwsim_end_call();
            break;

        case 41:
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_twc(3U, 0U);
            break;

        case 42:
            printf("Enter RHH option (0 - Hold Incoming, 1 - Accept Held, 2 - Reject Held): ");
            scanf("%d", &choice);
            (BT_IGNORE_RETURN_VALUE) nwsim_handle_btrh((UCHAR)choice);
            break;

        case 45:
            printf("Enter the Signal Strength (0-5)[Current: %d]: ", nwsim_signal);
            scanf("%d", &choice);
            nwsim_signal = (UCHAR)choice;
            nwsim_send_rsp(NWSIM_SIGNAL, &nwsim_signal);
            break;

        case 46:
            printf("Enter the Battery Strength (0-5)[Current: %d]: ", nwsim_battery);
            scanf("%d", &choice);
            nwsim_battery = (UCHAR)choice;
            nwsim_send_rsp(NWSIM_BATTERY, &nwsim_battery);
            break;

        case 47:
            printf("Enter the Service Availability (0-1)[Current: %d]: ", nwsim_service);
            scanf("%d", &choice);
            nwsim_service = (UCHAR)choice;
            nwsim_send_rsp(NWSIM_SERVICE, &nwsim_service);
            break;

        case 48:
            printf("Enter the Signal Roaming (0-1)[Current: %d]: ", nwsim_roaming);
            scanf("%d", &choice);
            nwsim_roaming = (UCHAR)choice;
            nwsim_send_rsp(NWSIM_ROAMING, &nwsim_roaming);
            break;

        case 50:
            (BT_IGNORE_RETURN_VALUE) nwsim_end_call();
            nwsim_send_rsp(NWSIM_NOCARRIER, NULL);

        default:
            NWSIM_ERR("Invalid Choice\n");
            break;
        }
    }
}

void nwsim_send_rsp (UCHAR rsp_code, void * value)
{
    CHAR response[256U];
    UINT16 length;
    UCHAR index;

    /* Initialize the response */
    BT_mem_set(response, 0, sizeof(response));

    /* Copy the base response and length from the response table */
    BT_str_n_copy(response, nwsim_rsp_table[rsp_code], (sizeof(response) - 1));
    length = (UINT16)BT_str_len(nwsim_rsp_table[rsp_code]);

    switch (rsp_code)
    {
        case NWSIM_OK: /* Fall Through */
        case NWSIM_ERROR: /* Fall Through */
        case NWSIM_NOCARRIER: /* Fall Through */
        case NWSIM_BUSY: /* Fall Through */
        case NWSIM_NOANSWER: /* Fall Through */
        case NWSIM_RING: /* Fall Through */
        case NWSIM_CIND_TEST: /* Fall Through */
        case NWSIM_CHLD_TEST: /* Fall Through */
        case NWSIM_COPS_READ: /* Fall Through */
        case NWSIM_CNUM:
            break;

        case NWSIM_CIND_READ:
            BT_str_print((&response[length]), "%d,%d,%d,%d,%d,%d,%d\r\n",
                nwsim_service, nwsim_call, nwsim_callsetup, nwsim_callheld, nwsim_signal, nwsim_roaming, nwsim_battery);
            break;

        case NWSIM_CLIP: /* Fall Through */
        case NWSIM_CCWA:
            BT_str_print((&response[length]), "\"%s\",129\r\n", (CHAR *)value);
            break;

        case NWSIM_CLCC:
            index = *(UCHAR *)value;
            BT_str_print((&response[length]), "%d,%d,%d,%d,%d,\"%s\",%d\r\n",
            nwsim_callinfo[index].idx, nwsim_callinfo[index].dir, nwsim_callinfo[index].status, nwsim_callinfo[index].mode,
            nwsim_callinfo[index].mpty, nwsim_callinfo[index].num, nwsim_callinfo[index].type);
            break;

        case NWSIM_BTRH: /* Fall Through */
        case NWSIM_CALL: /* Fall Through */
        case NWSIM_CALLSETUP: /* Fall Through */
        case NWSIM_CALLHELD: /* Fall Through */
            if (0U == nwsim_cmer)
            {
                return;
            }
        case NWSIM_CME_ERROR:
            BT_str_print((&response[length]), "%d\r\n", *((UCHAR *)value));
            break;

        case NWSIM_SIGNAL:
            if ((!nwsim_cmer) ||
                (nwsim_ciev & NWSIM_CIEV_SIGNAL))
            {
                BT_str_print((&response[length]), "%d\r\n", *((UCHAR *)value));
            }
            else
            {
                return;
            }
            break;

        case NWSIM_BATTERY:
            if ((!nwsim_cmer) ||
                (nwsim_ciev & NWSIM_CIEV_BATTERY))
            {
                BT_str_print((&response[length]), "%d\r\n", *((UCHAR *)value));
            }
            else
            {
                return;
            }
            break;

        case NWSIM_SERVICE:
            if ((!nwsim_cmer) ||
                (nwsim_ciev & NWSIM_CIEV_SERVICE))
            {
                BT_str_print((&response[length]), "%d\r\n", *((UCHAR *)value));
            }
            else
            {
                return;
            }
            break;

        case NWSIM_ROAMING:
            if ((!nwsim_cmer) ||
                (nwsim_ciev & NWSIM_CIEV_ROAM))
            {
                BT_str_print((&response[length]), "%d\r\n", *((UCHAR *)value));
            }
            else
            {
                return;
            }
            break;

        default:
            NWSIM_ERR("Invalid Response Code\n");
            break;
    }

    /* Update length and send response to interface */
    length = (UINT16)BT_str_len(response);
    nwsim_rsp_iface((UCHAR*)response, length);
}

void nwsim_ring_timeout_handler(void * args, UINT16 size)
{
    /* Reset timer handle */
    nwsim_ring_timer = BT_TIMER_HANDLE_INIT_VAL;

    /* Quit if state not incoming */
    if (!NWSIM_ISCALLSTATE(NWSIM_CALLSINCOMING))
    {
        return;
    }

    /* Ring */
    nwsim_send_rsp(NWSIM_RING, NULL);

    /* CLIP if applicable */
    if (0U != nwsim_clip)
    {
        nwsim_send_rsp(NWSIM_CLIP, &nwsim_clipnum);
    }

    /* Send ring and  clip in loop till accept/reject */
    (BT_IGNORE_RETURN_VALUE) BT_start_timer(&nwsim_ring_timer, 1U, nwsim_ring_timeout_handler, NULL, 0U);

    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(size);
}

API_RESULT nwsim_setup_incoming_call(CHAR * num)
{
    UINT8 idx;
    NWSIM_CALLINFO * ctx;

    if (!NWSIM_ISCALLSTATE(NWSIM_CALLSIDLE))
    {
        return API_FAILURE;
    }

    /* Allocate a call context */
    ctx = nwsim_alloc_call_context(&idx);
    if (NULL == ctx)
    {
        return API_FAILURE;
    }

    /* Update the context */
    BT_str_n_copy(ctx->num, num, (sizeof(ctx->num) - 1));
    ctx->type = 129U;
    ctx->mode = 0U;
    ctx->dir = 1U;
    ctx->status = 4U;

    /* Update state */
    NWSIM_SETCALLSTATE(NWSIM_CALLSINCOMING);

    /* Update setup call ID */
    nwsim_setupcall_id = idx;

    /* CallSetup 1 */
    nwsim_callsetup = 1U;
    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

    /* Ring */
    nwsim_send_rsp(NWSIM_RING, NULL);

    /* CLIP if applicable */
    if (0U != nwsim_clip)
    {
        BT_str_n_copy(nwsim_clipnum, num, (sizeof(nwsim_clipnum) - 1));
        nwsim_send_rsp(NWSIM_CLIP, &nwsim_clipnum);
    }

    /* Send ring and  clip in loop till accept/reject */
    (BT_IGNORE_RETURN_VALUE) BT_start_timer(&nwsim_ring_timer, 1U, nwsim_ring_timeout_handler, NULL, 0U);

    nwsim_callstatus_iface(0x02U);

    nwsim_display_callinfo();

    return API_SUCCESS;
}


void nwsim_outgoing_call_ring_timeout_handler(void * args, UINT16 size)
{
    /* Reset timer handle */
    nwsim_outgoing_ring_timer = BT_TIMER_HANDLE_INIT_VAL;

    /* Quit if state not outgoing */
    if (!NWSIM_ISCALLSTATE(NWSIM_CALLSOUTGOING))
    {
        printf("Not in outgoing call state\n");
        (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_outgoing_ring_timer);
        sco_audio_play_ringtone_exit_pl();
        printf("nwsim_outgoing_call_ring_timeout_handler Return\n");
        return;
    }
    sco_audio_play_outgoing_ringtone_pl();
    /* Send ring and  clip in loop till accept/reject */
    (BT_IGNORE_RETURN_VALUE) BT_start_timer(&nwsim_outgoing_ring_timer, 1, nwsim_outgoing_call_ring_timeout_handler, NULL, 0);

    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(size);
}

API_RESULT nwsim_setup_outgoing_call(CHAR * num)
{
    UINT8 idx;
    NWSIM_CALLINFO * ctx;

    if (!NWSIM_ISCALLSTATE(NWSIM_CALLSIDLE))
    {
        return API_FAILURE;
    }

    /* Allocate a call context */
    ctx = nwsim_alloc_call_context(&idx);
    if (NULL == ctx)
    {
        return API_FAILURE;
    }

    /* Update the context */
    BT_str_n_copy(ctx->num, num, (sizeof(ctx->num) - 1));
    ctx->type = 129U;
    ctx->mode = 0U;
    ctx->dir = 0U;
    ctx->status = 3U;

    /* Update state */
    NWSIM_SETCALLSTATE(NWSIM_CALLSOUTGOING);

    /* Update setup call ID */
    nwsim_setupcall_id = idx;

    /* CallSetup 2 */
    nwsim_callsetup = 2U;
    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

    /* CallSetup 3 */
    nwsim_callsetup = 3U;
    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

    (BT_IGNORE_RETURN_VALUE) BT_start_timer(&nwsim_outgoing_ring_timer, 1, nwsim_outgoing_call_ring_timeout_handler, NULL, 0);

    nwsim_callstatus_iface(0x03U);

    nwsim_display_callinfo();

    return API_SUCCESS;
}

void nwsim_ccwa_timeout_handler(void * args, UINT16 size)
{
    /* Reset timer handle */
    nwsim_ccwa_timer = BT_TIMER_HANDLE_INIT_VAL;

    /* Quit if state not incoming */
    if (0U == (NWSIM_ISCALLWAITING()))
    {
        return;
    }

    nwsim_send_rsp(NWSIM_CCWA, &nwsim_ccwanum);

    /* Send ring and  clip in loop till accept/reject */
    (BT_IGNORE_RETURN_VALUE) BT_start_timer(&nwsim_ccwa_timer, 1U, nwsim_ccwa_timeout_handler, NULL, 0U);

    BT_IGNORE_UNUSED_PARAM(args);
    BT_IGNORE_UNUSED_PARAM(size);
}

API_RESULT nwsim_setup_twc_incoming(CHAR * num)
{
    UINT8 idx;
    NWSIM_CALLINFO * ctx;

    if (!NWSIM_ISCALLSTATE(NWSIM_CALLSACTIVE))
    {
        return API_FAILURE;
    }

    /* Allocate a call context */
    ctx = nwsim_alloc_call_context(&idx);
    if (NULL == ctx)
    {
        return API_FAILURE;
    }

    /* Update the context */
    BT_str_n_copy(ctx->num, num, (sizeof(ctx->num) - 1));
    ctx->type = 129U;
    ctx->mode = 0U;
    ctx->dir = 1U;
    ctx->status = 5U;

    /* Update state */
    NWSIM_SETCALLSTATE(NWSIM_CALLSWAITING);

    /* Update setup call ID */
    nwsim_setupcall_id = idx;

    /* CCWA if applicable */
    if (0U != nwsim_ccwa)
    {
        BT_str_n_copy(nwsim_ccwanum, num, (sizeof(nwsim_ccwanum) - 1));
        nwsim_send_rsp(NWSIM_CCWA, &nwsim_ccwanum);

        /* Send ring and  clip in loop till accept/reject */
        (BT_IGNORE_RETURN_VALUE) BT_start_timer(&nwsim_ccwa_timer, 1U, nwsim_ccwa_timeout_handler, NULL, 0U);
    }

    /* CallSetup 1 */
    nwsim_callsetup = 1U;
    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

    nwsim_display_callinfo();

    return API_SUCCESS;
}

API_RESULT nwsim_setup_twc_outgoing(CHAR * num)
{
    NWSIM_CALLINFO * ctx;
    UINT8 idx;

    if (!NWSIM_ISCALLSTATE(NWSIM_CALLSACTIVE))
    {
        return API_FAILURE;
    }

    /* Allocate a call context */
    ctx = nwsim_alloc_call_context(&idx);
    if (NULL == ctx)
    {
        return API_FAILURE;
    }

    /* Update the context */
    BT_str_n_copy(ctx->num, num, (sizeof(ctx->num) - 1));
    ctx->type = 129U;
    ctx->mode = 0U;
    ctx->dir = 0U;
    ctx->status = 3U;

    /* Make the active call to held */
    nwsim_callinfo[nwsim_activecall_id].status = 1U;

    /* Update state */
    NWSIM_SETCALLSTATE(NWSIM_CALLSHELD);

    /* Update most recent held call id as the active call id */
    nwsim_heldcall_id = nwsim_activecall_id;

    /* CallHeld 2 */
    nwsim_callheld = 2U;
    nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);

    /* Update state */
    NWSIM_SETCALLSTATEIDLE();
    NWSIM_SETCALLSTATE(NWSIM_CALLSOUTGOING);

    /* Update setup call ID */
    nwsim_setupcall_id = idx;

    /* CallSetup 2 */
    nwsim_callsetup = 2U;
    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

    /* CallSetup 3 */
    nwsim_callsetup = 3U;
    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

    nwsim_display_callinfo();

    return API_SUCCESS;
}

API_RESULT nwsim_handle_call (UCHAR action)
{
    switch (action)
    {
        case NWSIM_INCOMING_ACCEPT:
            if (!NWSIM_ISCALLSTATE(NWSIM_CALLSINCOMING))
            {
                return API_FAILURE;
            }

            /* Update state */
            NWSIM_SETCALLSTATE(NWSIM_CALLSACTIVE);

            /* Stop the timer */
            (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_ring_timer);
            nwsim_ring_timer = BT_TIMER_HANDLE_INIT_VAL;

            nwsim_callcount++;

            /* Update active call ID */
            nwsim_activecall_id = nwsim_setupcall_id;
            nwsim_callinfo[nwsim_activecall_id].status = 0U;

            /* Call 1 */
            nwsim_call = 1U;
            nwsim_send_rsp(NWSIM_CALL, &nwsim_call);

            /* CallSetup 0 */
            nwsim_callsetup = 0U;
            nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

            nwsim_callstatus_iface(0x01U);

            break;

        case NWSIM_INCOMING_END_LOCAL:
        case NWSIM_INCOMING_END_REMOTE:
            /* End incoming call. Validate state if incoming */
            if (!NWSIM_ISCALLSTATE(NWSIM_CALLSINCOMING))
            {
                return API_FAILURE;
            }

            /* Update state */
            NWSIM_SETCALLSTATEIDLE();

            /* Stop the timer */
            (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_ring_timer);
            nwsim_ring_timer = BT_TIMER_HANDLE_INIT_VAL;

            /* Update active call ID */
            nwsim_callinfo[nwsim_setupcall_id].status = 0xFFU;

            /* CallSetup 0 */
            nwsim_callsetup = 0U;
            nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

            nwsim_callstatus_iface(0x00U);

            break;

        case NWSIM_OUTGOING_ACCEPT:
            if (!NWSIM_ISCALLSTATE(NWSIM_CALLSOUTGOING))
            {
                return API_FAILURE;
            }

            /* Update state */
            NWSIM_SETCALLSTATE(NWSIM_CALLSACTIVE);

            nwsim_callcount++;

            /* Update active call ID */
            nwsim_activecall_id = nwsim_setupcall_id;
            nwsim_callinfo[nwsim_activecall_id].status = 0U;

            /* Call 1 */
            nwsim_call = 1U;
            nwsim_send_rsp(NWSIM_CALL, &nwsim_call);

            /* CallSetup 0 */
            nwsim_callsetup = 0U;
            nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

            nwsim_callstatus_iface(0x01U);

            break;

        case NWSIM_OUTGOING_END_LOCAL:
        case NWSIM_OUTGOING_END_REMOTE:
            /* End outgoing call. Validate state if outgoing */
            if (!NWSIM_ISCALLSTATE(NWSIM_CALLSOUTGOING))
            {
                return API_FAILURE;
            }

            /* Update state */
            NWSIM_SETCALLSTATEIDLE();

            /* Update active call ID */
            nwsim_callinfo[nwsim_setupcall_id].status = 0xFFU;

            /* CallSetup 0 */
            nwsim_callsetup = 0U;
            nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

            nwsim_callstatus_iface(0x00U);

            break;

        default:
            NWSIM_ERR("Invalid Action Type\n");
            break;
    }

    nwsim_display_callinfo();

    return API_SUCCESS;
}

API_RESULT nwsim_dial (CHAR * num)
{
    if (API_SUCCESS != nwsim_setup_outgoing_call(num))
    {
        if (API_SUCCESS != nwsim_setup_twc_outgoing(num))
        {
            NWSIM_ERR("Dial Failed.\n");
            return API_FAILURE;
        }
    }

    return API_SUCCESS;
}

API_RESULT nwsim_handle_twc (UCHAR action, UCHAR idx)
{
    UINT8 i, id;

    switch (action)
    {
        case 0U:
            if (0U != (NWSIM_ISCALLWAITING()))
            {
                nwsim_callinfo[nwsim_setupcall_id].status = 0xFFU;

                /* Update state */
                NWSIM_RESETCALLWAITING();

                /* Stop the timer */
                (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_ccwa_timer);
                nwsim_ccwa_timer = BT_TIMER_HANDLE_INIT_VAL;

                /* CallSetup 0 */
                nwsim_callsetup = 0U;
                nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

                /* UDUB tone */
            }
            else if (0U != (NWSIM_ISCALLHELD()))
            {
                /* Is call active? */
                if (NWSIM_ISCALLSTATE(NWSIM_CALLSACTIVE))
                {
                    /* Update state */
                    NWSIM_RESETCALLHELD();

                    /* Release all held calls */
                    for (i = 0U; i < NWSIM_MAX_CALLS; i++)
                    {
                        if (1U == nwsim_callinfo[i].status)
                        {
                            nwsim_callinfo[i].status = 0xFFU;
                        }
                    }

                    /* CallHeld 0 */
                    nwsim_callheld = 0U;
                    nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
                }
                else
                {
                    /* End TWC Outgoing */

                    nwsim_callinfo[nwsim_setupcall_id].status = 0xFFU;

                    /* Update state */
                    NWSIM_RESETCALLHELD();

                    /* CallSetup 0 */
                    nwsim_callsetup = 0U;
                    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

                    /* CallHeld 0 */
                    nwsim_callheld = 0U;
                    nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case 1U:

            id = (0U == idx) ? nwsim_activecall_id : (idx - 1U);

            if (0U != nwsim_callinfo[id].status)
            {
                return API_FAILURE;
            }

            /* Release the active call */
            nwsim_callinfo[id].status = 0xFFU;
            nwsim_callcount--;

            if (0U != (NWSIM_ISCALLWAITING()))
            {
                /* Update state */
                NWSIM_RESETCALLWAITING();

                /* Stop the timer */
                (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_ccwa_timer);
                nwsim_ccwa_timer = BT_TIMER_HANDLE_INIT_VAL;

                nwsim_activecall_id = nwsim_setupcall_id;
                nwsim_callinfo[nwsim_activecall_id].status = 0U;

                nwsim_callcount++;

                /* CallSetup 0 */
                nwsim_callsetup = 0U;
                nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);
            }
            else if (0U != (NWSIM_ISCALLHELD()))
            {
                /* Update state */
                NWSIM_RESETCALLHELD();

                nwsim_activecall_id = nwsim_heldcall_id;
                nwsim_callinfo[nwsim_activecall_id].status = 0U;

                /* CallHeld 0 */
                nwsim_callheld = 0U;
                nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case 2U:
            if (0U != idx)
            {
                for (i = 0U; i < NWSIM_MAX_CALLS; i++)
                {
                    /* TODO: Error check of invalid index */
                    if (0x00U == nwsim_callinfo[i].status)
                    {
                        if (i == (idx - 1U))
                        {
                            /* Enable private consultation */
                            nwsim_callinfo[i].status = 0U;
                            nwsim_activecall_id = i;
                        }
                        else
                        {
                            nwsim_callinfo[i].status = 1U;
                            if (0xFFU == nwsim_heldcall_id)
                            {
                                nwsim_heldcall_id = i;

                                NWSIM_SETCALLHELD();

                                /* CallHeld 1 */
                                nwsim_callheld = 1U;
                                nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
                            }
                        }
                    }
                }
            }
            else if (0U != (NWSIM_ISCALLWAITING()))
            {
                /* Make active call to held */
                nwsim_callinfo[nwsim_activecall_id].status = 1U;
                nwsim_heldcall_id = nwsim_activecall_id;

                nwsim_activecall_id = nwsim_setupcall_id;
                nwsim_callinfo[nwsim_activecall_id].status = 0U;

                nwsim_callcount++;

                /* Update State */
                NWSIM_RESETCALLWAITING();
                NWSIM_SETCALLHELD();

                /* Stop the timer */
                (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_ccwa_timer);
                nwsim_ccwa_timer = BT_TIMER_HANDLE_INIT_VAL;

                /* CallSetup 0 */
                nwsim_callsetup = 0U;
                nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

                /* CallHeld 1 */
                nwsim_callheld = 1U;
                nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
            }
            else if (0U != (NWSIM_ISCALLHELD()))
            {
                /* Is active call in place, then swap */
                if (NWSIM_ISCALLSTATE(NWSIM_CALLSACTIVE))
                {
                    /* Make active call to held */
                    nwsim_setupcall_id = nwsim_heldcall_id;

                    nwsim_callinfo[nwsim_activecall_id].status = 1U;
                    nwsim_heldcall_id = nwsim_activecall_id;

                    nwsim_activecall_id = nwsim_setupcall_id;
                    nwsim_callinfo[nwsim_activecall_id].status = 0U;

                    /* CallHeld 1 */
                    nwsim_callheld = 1U;
                    nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
                }
                else
                {
                    /* Accept TWC Outgoing */

                    /* CallSetup 0 */
                    nwsim_callsetup = 0U;
                    nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

                    /* Make the active call to held */
                    nwsim_activecall_id = nwsim_setupcall_id;
                    nwsim_callinfo[nwsim_activecall_id].status = 0U;

                    /* Update state */
                    NWSIM_SETCALLSTATE(NWSIM_CALLSACTIVE);

                    nwsim_callcount++;

                    /* CallHeld 0 */
                    nwsim_callheld = 1U;
                    nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case 3U:
            if (0U != (NWSIM_ISCALLHELD()))
            {
                nwsim_conference = 1U;
                nwsim_callinfo[nwsim_heldcall_id].status = 0U;

                /* Enable multiparty in active and held calls */
                nwsim_callinfo[nwsim_activecall_id].mpty = 1U;
                nwsim_callinfo[nwsim_heldcall_id].mpty = 1U;

                NWSIM_RESETCALLHELD();
                nwsim_heldcall_id = 0xFFU;

                /* CallHeld 0 */
                nwsim_callheld = 0U;
                nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);
            }

            break;

        case 4U:
            if (0U != (NWSIM_ISCALLHELD()))
            {
                NWSIM_RESETCALLHELD();

                nwsim_reset();

                /* Call 0 */
                nwsim_call = 0U;
                nwsim_send_rsp(NWSIM_CALL, &nwsim_call);

                nwsim_callstatus_iface(0x00U);
            }

            break;

        default:
            NWSIM_ERR("Invalid action type\n");
            break;
    }

    nwsim_display_callinfo();

    return API_SUCCESS;
}

API_RESULT nwsim_end_call(void)
{
    UCHAR i;

    if (NWSIM_ISCALLSTATE(NWSIM_CALLSACTIVE))
    {
        /* Update state */
        NWSIM_SETCALLSTATEIDLE();
        nwsim_activecall_id = 0xFFU;

        for (i = 0U; i < NWSIM_MAX_CALLS; i++)
        {
            if (0x00U != nwsim_callinfo[i].status)
            {
                continue;
            }

            nwsim_callinfo[i].status = 0xFFU;
            nwsim_callcount--;

            if (0U != nwsim_conference)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        nwsim_conference = 0U;
        if (0U == nwsim_callcount)
        {
            nwsim_call = 0U;
            nwsim_send_rsp(NWSIM_CALL, &nwsim_call);

            if (0U == (NWSIM_ISCALLWAITING()))
            {
                nwsim_callstatus_iface(0x00U);
            }
        }

        if (0U != (NWSIM_ISCALLWAITING()))
        {
            /* Update state */
            NWSIM_RESETCALLWAITING();
            NWSIM_SETCALLSTATE(NWSIM_CALLSINCOMING);

            /* Stop the timer */
            (BT_IGNORE_RETURN_VALUE) BT_stop_timer(nwsim_ccwa_timer);
            nwsim_ccwa_timer = BT_TIMER_HANDLE_INIT_VAL;
        }
    }
    else if ((NWSIM_ISCALLSTATE(NWSIM_CALLSINCOMING)) ||
        (NWSIM_ISCALLSTATE(NWSIM_CALLSOUTGOING)))
    {
        NWSIM_SETCALLSTATEIDLE();

        nwsim_callinfo[nwsim_setupcall_id].status = 0xFFU;
        nwsim_setupcall_id = 0xFFU;

        nwsim_callsetup = 0U;
        nwsim_send_rsp(NWSIM_CALLSETUP, &nwsim_callsetup);

        nwsim_callstatus_iface(0x00U);
    }
    else if (0U != (NWSIM_ISCALLHELD()))
    {
        /* Update held call status */
        NWSIM_RESETCALLHELD();

        nwsim_callinfo[nwsim_heldcall_id].status = 0xFFU;
        nwsim_heldcall_id = 0xFFU;
        nwsim_callcount--;

        nwsim_callheld = 0U;
        nwsim_send_rsp(NWSIM_CALLHELD, &nwsim_callheld);

        if (0U == nwsim_callcount)
        {
            nwsim_call = 0U;
            nwsim_send_rsp(NWSIM_CALL, &nwsim_call);

            nwsim_callstatus_iface(0x00U);
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }

    nwsim_display_callinfo();

    return API_SUCCESS;
}

API_RESULT nwsim_handle_btrh(UCHAR option)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    switch (option)
    {
    case 0U:
        if (4U == nwsim_callinfo[nwsim_setupcall_id].status)
        {
            nwsim_btrh_status = option;
            nwsim_send_rsp(NWSIM_BTRH, &nwsim_btrh_status);

            (BT_IGNORE_RETURN_VALUE) nwsim_handle_call(NWSIM_INCOMING_ACCEPT);

            nwsim_callinfo[nwsim_setupcall_id].status = 6U;
            NWSIM_SETCALLHELD();

            retval = API_SUCCESS;
        }
        break;

    case 1U:
        if (6U == nwsim_callinfo[nwsim_setupcall_id].status)
        {
            nwsim_btrh_status = option;
            nwsim_send_rsp(NWSIM_BTRH, &nwsim_btrh_status);

            nwsim_callinfo[nwsim_setupcall_id].status = 0U;
            NWSIM_RESETCALLHELD();

            retval = API_SUCCESS;
        }
        break;

    case 2U:
        if (6U == nwsim_callinfo[nwsim_setupcall_id].status)
        {
            nwsim_btrh_status = option;
            nwsim_send_rsp(NWSIM_BTRH, &nwsim_btrh_status);

            nwsim_callinfo[nwsim_setupcall_id].status = 0U;
            NWSIM_RESETCALLHELD();

            (BT_IGNORE_RETURN_VALUE) nwsim_end_call();

            retval = API_SUCCESS;
        }
        break;

    default:
        NWSIM_ERR("Invalid Option\n");
        break;
    }

    nwsim_display_callinfo();

    return retval;
}

void nwsim_display_callinfo(void)
{
    UINT8 index;
    CHAR * mode[] = { "Voice", "Data", "Fax" };
    CHAR * status[] = { "Active", "Held", "Dialing", "Alerting", "Incoming", "Waiting", "RH Held" };

    printf("------------------------------------ Call Info ----------------------------------\n");
    printf("Idx\tNumber\t\tType\tDirection\tStatus\t\tMode\tMultiparty\n");
    printf("---------------------------------------------------------------------------------\n");

    for (index = 0U; index < NWSIM_MAX_CALLS; index++)
    {
        if (0xFFU != nwsim_callinfo[index].status)
        {
            printf("%d\t%s\t\t%d\t%s\t%s\t\t%s\t%d\n",
                nwsim_callinfo[index].idx, nwsim_callinfo[index].num, nwsim_callinfo[index].type,
                (nwsim_callinfo[index].dir) ? "Incoming" : "Outgoing",
                status[nwsim_callinfo[index].status], mode[nwsim_callinfo[index].mode], nwsim_callinfo[index].mpty);
        }
    }

    printf("---------------------------------------------------------------------------------\n");
}

NWSIM_CALLINFO * nwsim_alloc_call_context(UINT8 * idx)
{
    NWSIM_CALLINFO * ctx;
    UINT8 index;

    ctx = NULL;
    for (index = 0U; index < NWSIM_MAX_CALLS; index++)
    {
        if (0xFFU == nwsim_callinfo[index].status)
        {
            NWSIM_TRC("Allocated Call Instance - %d\n", index);
            ctx = &nwsim_callinfo[index];
            ctx->idx = index + 1U;
            break;
        }
    }

    *idx = index;

    return ctx;
}
