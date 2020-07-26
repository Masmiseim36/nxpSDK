/** @file wifi-debug.c
 *
 *  @brief This file provides WIFI debug APIs.
 *
 *  Copyright 2008-2020 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */
#include <wm_os.h>

#include <wifi-debug.h>

#ifdef CONFIG_WIFI_EXTRA_DEBUG
static const char *const status_table[] = {
    [0]  = "Successful",
    [1]  = "Unspecified failure",
    [10] = "Information field does not support all requested capabilities.",
    [11] = "Could not confirm existing assiciation. Denied reassociation.",
    [12] = "Denied Association. Reason beyond scope of Standard.",
    [13] = "Auth Algo not supported by responding station.",
    [14] = "Transaction sequence number of received authentication frame is out of expected sequence.",
    [15] = "Challenge failure. Rejected Auth.",
    [16] = "Timeout waiting for next frame in sequence. Rejected Auth.",
    [17] = "Failure at AP to to handle additional associated STA. Rejected Assoc.",
    [18] = "Requesting STA does not support all BSSBasicRateSet data rated. Denied Assoc.",
    [19] = "Requesting STA does not support short preamble option. Denied Assoc.",
    [20] = "Requesting STA does not support PBCC modulation option. Denied Assoc.",
    [21] = "Requesting STA does not support channel agility option. Denied Assoc.",
    [22] = "Spectrum Management capability needed. Rejected Assoc.",
    [23] = "Power Capability element info is unacceptable. Rejected Assoc.",
    [24] = "Supported Channels element info is unacceptable. Rejected Assoc.",
    [25] = "Requesting STA does not support short slot time option. Rejected Assoc.",
    [26] = "Requesting STA does not support ER-PBCC modulation option. Rejected Assoc.",
    [27] = "Requesting STA does not support HT features. Denied Assoc.",
    [28] = "Unreachable R0KH",
    [29] = "AP needs PCO transition support. Requesting STA does not support it. Denied Assoc.",
    [30] = "Try again later. Temporarily Rejected Assoc.",
    [31] = "Violation of policy for Robust Management frame",
    [32] = "Unspecified, QoS related failure",
    [33] = "Insufficient bandwidth with QAP to handle another QSTA. Denied Assoc.",
    [34] = "Poor channel conditions resulting in Assoic Denied.",
    [35] = "Requesting STA does not support QoS facility. Denied Assoc (with QBSS).",
    [37] = "The request has been declined",
    [38] = "The failed. One or more parameters have invalid values",
    [39] =
        "The TS has not been created because the request cannot be "
        "honored. However, a suggested Tspec is provided so that the "
        "initiating QSTA may attempt to send another TS with the suggested "
        "changes to the TSpec",
    [40] = "Information Element is Invalid",
    [41] = "Invalid Group Cipher",
    [42] = "Invalid Pairwise Cipher",
    [43] = "Invalid AKMP",
    [44] = "RSN IE version unsupported ",
    [45] = "RSN IE Capabilities are invalid ",
    [46] = "Security policy does not allows rejected Cipher suite",
    [47] =
        "The TS has not been created. However, the HC may be capable "
        "of creating a TS, in response to a request, after the time "
        "indicated in the TS Delay element",
    [48] = "By policy BSS not allow Direct link",
    [49] = "QBSS does not include Destination STA",
    [50] = "The destination STA is not a QSTA",
    [51] = "Listen Interval is too large. Assoc Denied",
    [52] = "Fast BSS Transition Action Frame Count Invalid",
    [53] = "PMKID Invalid",
    [54] = "MDIE Invalid",
    [55] = "FTIE Invalid",
};

const char *get_status_str(uint16_t status)
{
    if (status < sizeof(status_table) / sizeof(status_table[0]) && status_table[status])
        return status_table[status];
    return "<unknown>";
}
#endif /* CONFIG_WIFI_EXTRA_DEBUG */

void wifi_show_assoc_fail_reason(int status)
{
#ifdef CONFIG_WIFI_EXTRA_DEBUG
    PRINTF("[wifi] Assoc Status: %s\n\r", get_status_str(status));

    switch (status)
    {
        case 13:
            PRINTF("Assoc failed: Network not found.\n\r");
            break;
        case 18:
            PRINTF("(11n disabled ?)\n\r");
            break;
    }
#endif /* CONFIG_WIFI_EXTRA_DEBUG */
}

#ifdef DUMP_PACKET_MAC
void dump_mac_addr(const char *msg, unsigned char *addr)
{
    if (msg)
        PRINTF("%s: ", msg);
    else
        PRINTF("mac: ", msg);

    int i;
    for (i = 0; i < MLAN_MAC_ADDR_LENGTH; i++)
    {
        PRINTF("%x", addr[i]);
        if (i != (MLAN_MAC_ADDR_LENGTH - 1))
            PRINTF(":");
    }

    PRINTF("\n\r");
}
#endif /* DUMP_PACKET_MAC */

#ifdef DEBUG_11N_AGGR
void dump_packet_header(const HostCmd_DS_COMMAND *cmd)
{
    PRINTF("Command ----- : %x\n\r", cmd->command);
    PRINTF("Size -------- : %d\n\r", cmd->size);
    PRINTF("Sequence ---- : %d\n\r", cmd->seq_num);
    PRINTF("Result ------ : %x\n\r", cmd->result);
}
#endif /* DEBUG_11N_AGGR */

#ifdef DEBUG_11N_AGGR
void dump_addba_req_rsp_packet(const HostCmd_DS_COMMAND *cmd)
{
    dump_packet_header(cmd);

    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp = (HostCmd_DS_11N_ADDBA_RSP *)&cmd->params.add_ba_rsp;
    PRINTF("add_rsp_result: %x\n\r", padd_ba_rsp->add_rsp_result);
    PRINTF("peer mac address: ");
    int i;
    for (i = 0; i < MLAN_MAC_ADDR_LENGTH; i++)
        PRINTF("%x ", padd_ba_rsp->peer_mac_addr[i]);
    PRINTF("\n\r");
    PRINTF("Dialog token: %x\n\r", padd_ba_rsp->dialog_token);
    PRINTF("Status code: %x\n\r", padd_ba_rsp->status_code);
    PRINTF("block_ack_param_set: %x\n\r", padd_ba_rsp->block_ack_param_set);
    if (padd_ba_rsp->block_ack_param_set & 0x1)
        PRINTF("         AMSDU supported\n\r");
    else
        PRINTF("         AMSDU _NOT_ supported\n\r");
    if (padd_ba_rsp->block_ack_param_set & 0x2)
        PRINTF("         Immediate block ack\n\r");
    else
        PRINTF("         Delayed block ack\n\r");
    PRINTF("         TID: %d\n\r", (padd_ba_rsp->block_ack_param_set & 0x3C) >> 2);
    PRINTF("         Buffer size: %d\n\r", (padd_ba_rsp->block_ack_param_set & 0xFFC) >> 6);
    PRINTF("block_ack_tmo: %x\n\r", padd_ba_rsp->block_ack_tmo);
    PRINTF("ssn: %x\n\r", padd_ba_rsp->ssn);
    PRINTF("*************************\n\r");
}
#endif /* DEBUG_11N_AGGR */

#ifdef DEBUG_11N_ASSOC
void dump_htcap_info(const MrvlIETypes_HTCap_t *htcap)
{
    PRINTF("******* HT Cap ******\n\r");
    PRINTF("Type: %d\n\r", htcap->header.type);
    PRINTF("Len: %d\n\r", htcap->header.len);
    PRINTF("ht cap info: 0%x\n\r", htcap->ht_cap.ht_cap_info);
    PRINTF("ampdu_param: 0%x\n\r", htcap->ht_cap.ampdu_param);
    PRINTF("supported_mcs_set: ");
    int i;
    for (i = 0; i < 16; i++)
        PRINTF("0x%x ", htcap->ht_cap.supported_mcs_set[i]);
    PRINTF("\n\r");
    PRINTF("ht_ext_cap: 0%x\n\r", htcap->ht_cap.ht_ext_cap);
    PRINTF("tx_bf_cap: 0x%x\n\r", htcap->ht_cap.tx_bf_cap);
    PRINTF("asel: 0x%x\n\r", htcap->ht_cap.asel);
}

void dump_ht_info(const MrvlIETypes_HTInfo_t *htinfo)
{
    PRINTF("******* HT Info ******\n\r");
    PRINTF("Type: %d\n\r", htinfo->header.type);
    PRINTF("Len: %d\n\r", htinfo->header.len);
    PRINTF("pri_chan: %d\n\r", htinfo->ht_info.pri_chan);
    PRINTF("field2: 0%x\n\r", htinfo->ht_info.field2);
    PRINTF("field3: 0%x\n\r", htinfo->ht_info.field3);
    PRINTF("field4: 0%x\n\r", htinfo->ht_info.field4);
    PRINTF("basic_mcs_set: ");
    int i;
    for (i = 0; i < 16; i++)
        PRINTF("0x%x ", htinfo->ht_info.basic_mcs_set[i]);
    PRINTF("\n\r");
}
#endif /* DEBUG_11N_ASSOC */
