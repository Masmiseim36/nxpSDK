/** @file mlan_wmm.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of wmm functionalities
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/****************************************************
Change log:
    10/24/2008: initial version
****************************************************/

#ifndef _MLAN_WMM_H_
#define _MLAN_WMM_H_

/**
 *  @brief This function gets the TID
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param ptr          A pointer to RA list table
 *
 *  @return 	    TID
 */
static INLINE int wlan_get_tid(pmlan_adapter pmadapter, raListTbl *ptr)
{
    pmlan_buffer mbuf;

    ENTER();
    mbuf = (pmlan_buffer)(void *)util_peek_list(pmadapter->pmoal_handle, &ptr->buf_head, MNULL, MNULL);
    LEAVE();

    return mbuf->priority;
}

/**
 *  @brief This function gets the length of a list
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param head         A pointer to mlan_list_head
 *
 *  @return 	    Length of list
 */
static INLINE int wlan_wmm_list_len(pmlan_adapter pmadapter, pmlan_list_head head)
{
    pmlan_linked_list pos;
    int count = 0;

    ENTER();

    pos = head->pnext;

    while (pos != (pmlan_linked_list)(void *)head)
    {
        ++count;
        pos = pos->pnext;
    }

    LEAVE();
    return count;
}

raListTbl *wlan_wmm_get_ralist_node(pmlan_private priv, t_u8 tid, t_u8 *ra_addr);
/** Initialize WMM */
t_void wlan_wmm_init(pmlan_adapter pmadapter);
/* Setup default queues */
void wlan_wmm_default_queue_priorities(pmlan_private priv);

/** Process WMM association request */
extern t_u32 wlan_wmm_process_association_req(pmlan_private priv,
                                              t_u8 **ppAssocBuf,
                                              IEEEtypes_WmmParameter_t *pWmmIE,
                                              IEEEtypes_HTCap_t *pHTCap);


/** WMM DELTS request command handler */
extern mlan_status wlan_cmd_wmm_delts_req(IN pmlan_private pmpriv, OUT HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);

#ifdef CONFIG_WMM_ENH
/* wmm enhance buffer pool */
#define MAX_WMM_BUF_NUM 32
#define WMM_DATA_LEN 1518
#define OUTBUF_WMM_LEN (sizeof(outbuf_t))

typedef struct {
    mlan_linked_list entry;
    t_u8 intf_header[INTF_HEADER_LEN];
    TxPD tx_pd;
    t_u8 data[WMM_DATA_LEN];
} outbuf_t;

/* transfer destination address to receive address */
void wifi_wmm_da_to_ra(uint8_t *da, uint8_t *ra);

/* wmm enhance get free buffer */
uint8_t *wifi_wmm_get_outbuf_enh(uint32_t *outbuf_len,
                                 mlan_wmm_ac_e queue,
                                 const uint8_t interface,
                                 uint8_t *ra,
                                 bool *is_tx_pause);

/* wmm enhance enqueue tx buffer */
int wlan_wmm_add_buf_txqueue_enh(const uint8_t interface,
                          const uint8_t *buffer,
                          const uint16_t len,
                          uint8_t pkt_prio);

/* wmm enhance buffer pool management */
outbuf_t *wifi_wmm_buf_get(void);
void wifi_wmm_buf_put(outbuf_t *buf);
int wifi_wmm_buf_pool_init(uint8_t *pool);
void wifi_wmm_buf_pool_deinit(void);

/* wmm enhance ralist operation */
void wlan_ralist_add_enh(mlan_private *priv, t_u8 *ra);
int wlan_ralist_update_enh(mlan_private *priv, t_u8 *old_ra, t_u8 *new_ra);
void wlan_ralist_pkts_free_enh(mlan_private *priv, raListTbl *ra_list, t_u8 ac);
void wlan_ralist_del_enh(mlan_private *priv, t_u8 *ra);
void wlan_ralist_del_all_enh(mlan_private *priv);
void wlan_ralist_deinit_enh(mlan_private *priv);

/* debug statistics */
void wifi_wmm_drop_err_mem(const uint8_t interface);
void wifi_wmm_drop_no_media(const uint8_t interface);
void wifi_wmm_drop_retried_drop(const uint8_t interface);
void wifi_wmm_drop_pause_drop(const uint8_t interface);
void wifi_wmm_drop_pause_replaced(const uint8_t interface);
#endif

#endif /* !_MLAN_WMM_H_ */
