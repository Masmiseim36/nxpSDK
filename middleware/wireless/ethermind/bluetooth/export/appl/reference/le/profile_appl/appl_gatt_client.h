
/**
 *  \file appl_gatt_client.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GATT_CLIENT_
#define _H_APPL_GATT_CLIENT_

/* --------------------------------------------- Header File Inclusion */
#include "BT_att_api.h"
#include "BT_gatt_db_api.h"

#include "BT_smp_api.h"

#include "gatt.h"
#include "appl_utils.h"

/* --------------------------------------------- Global Definitions */
/* Flag that control enable disable of application event trace prints */
#define APPL_GATT_CLIENT_HAVE_EVT_TRC_SELECTION

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
#define APPL_NOTIFY_GATT_CONN_TO_PROFILE(a)               \
        a##_notify_gatt_conn()

#define APPL_NOTIFY_GATT_DISCONN_TO_PROFILE(a)            \
        a##_notify_gatt_disconn()

#define APPL_NOTIFY_GATT_SERVDATA_TO_PROFILE(a,b,c)       \
        a##_notify_gatt_servdata(b,c)

#define APPL_NOTIFY_GATT_CHARDATA_TO_PROFILE(a,b,c)       \
        a##_notify_gatt_chardata(b,c)

#define APPL_PARSE_NOTIFICATION_DATA_OF_PROFILE(a,b,c,d)  \
        a##_parse_notification_data(b, c, d)

#define APPL_PARSE_INDICATION_DATA_OF_PROFILE(a,b,c,d)    \
        a##_parse_indication_data((b), (c), (d))

#define APPL_PARSE_READ_DATA_OF_PROFILE(a,b,c)            \
        a##_parse_read_data((b), (c))

#define APPL_NOTIFY_WRITE_RSP_TO_PROFILE(a)               \
        a##_notify_write_rsp()

#define APPL_NOTIFY_EXECUTE_WRITE_RSP_TO_PROFILE(a)       \
        a##_notify_execute_write_rsp()

#define APPL_NOTIFY_ADV_REPORT(a,at,bd,dt,dl)             \
        a##_notify_adv_report((at), (bd), (dt), (dl))

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void appl_gatt_init(void);
void main_gatt_client_operations(void);

void appl_notify_gatt_conn(void);
void appl_notify_gatt_disconn(void);
void appl_notify_gatt_servdata (GATT_SERVICE_PARAM * service, UINT16 size);
void appl_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void appl_parse_notification_data(UINT16 handle, UCHAR * data, UINT16 datalen);
void appl_parse_indication_data(UINT16 handle, UCHAR * data, UINT16 datalen);
void appl_parse_read_data(UCHAR * data, UINT16 datalen);
void appl_notify_write_rsp(void);
void appl_notify_execute_write_rsp(void);
void appl_notify_adv_report
     (
         UCHAR          adv_type,
         BT_DEVICE_ADDR * bd,
         UCHAR          * data,
         UINT16         data_len
     );

#ifdef APPL_GATT_CLIENT_HAVE_EVT_TRC_SELECTION
void appl_set_gatt_client_evt_trc(UCHAR flag);
#endif /* APPL_GATT_CLIENT_HAVE_EVT_TRC_SELECTION */

#ifdef SMP_DATA_SIGNING
void appl_gatt_signing_complete (API_RESULT status, UCHAR * data, UINT16 datalen);
#endif /* SMP_DATA_SIGNING */

#ifdef HTC
void htc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void htc_profile_operations (void);
void htc_parse_notification_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen);
void htc_parse_indication_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen);
void htc_parse_read_data (UCHAR * data, UINT16 datalen);
#endif /* HTC */

#ifdef HRC
void hrc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void hrc_profile_operations (void);
#endif /* HRC */

#ifdef AMC
void amc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void amc_profile_operations (void);
#endif /* AMC */

#ifdef RSCC
void rscc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void rscc_profile_operations (void);
#endif /* RSCC */

#ifdef CSCC
void cscc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void cscc_profile_operations (void);
#endif /* CSCC */

#ifdef LNC
void lnc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void lnc_profile_operations (void);
#endif /* LNC */

#ifdef CPMC
void cpmc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void cpmc_profile_operations (void);
void cpmc_parse_indication_data (UINT16 handle, UCHAR  * data, UINT16 datalen);
void cpmc_notify_write_rsp(void);
void cpmc_notify_gatt_disconn(void);
void cpmc_notify_gatt_conn(void);
void cpmc_notify_adv_report
     (
         UCHAR          adv_type,
         BT_DEVICE_ADDR * bd,
         UCHAR          * data,
         UINT16         data_len
     );
#endif /* CPMC */

#ifdef WSC
void wsc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void wsc_profile_operations (void);
#endif /* WSC */

#ifdef ESC
void esc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void esc_profile_operations (void);
#endif /* ESC */

#ifdef BMC
void bmc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void bmc_profile_operations (void);
#endif /* BMC */

#ifdef AIOC
void aioc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void aioc_profile_operations (void);
#endif /* AIOC */

#ifdef IPC
void ipc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void ipc_profile_operations (void);
#endif /* IPC */

#ifdef PXM
void pxm_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void pxm_profile_operations (void);
void pxm_parse_notification_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen);
void pxm_parse_read_data (UCHAR * data, UINT16 datalen);
API_RESULT pxm_discover_service
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ ATT_UUID16 s_uuid
           );
API_RESULT pxm_rd_attr
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ UINT16     attr_hndl
           );
API_RESULT pxm_wr_alert_lvl
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ UINT16     attr_hndl,
               /* IN */ UCHAR      alert_lvl,
               /* IN */ UCHAR      rsp
           );
API_RESULT pxm_config_tps
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ UINT16     attr_hndl,
               /* IN */ UCHAR      enable
           );
API_RESULT pxm_get_rssi_complete
           (
               /* IN */ UCHAR  status,
               /* IN */ UINT16 conn_handle,
               /* IN */ UCHAR  rssi
           );
#endif /* PXM */

#ifdef FML
void fml_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void fml_profile_operations (void);
#endif /* FML */

#ifdef HIDH
void hidh_notify_gatt_conn (void);
void hidh_notify_gatt_disconn (void);
void hidh_notify_gatt_servdata (GATT_SERVICE_PARAM * service, UINT16 size);
void hidh_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void hidh_profile_operations (void);
void hidh_parse_read_data (UCHAR * data, UINT16 datalen);
#endif /* HIDH */

#ifdef ANP
void anp_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void anp_profile_operations (void);
#endif /* ANP */

#ifdef BPC
void bpc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void bpc_profile_operations (void);
#endif /* BPC */

#ifdef PASP
void pasp_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void pasp_profile_operations (void);
#endif /* PASS */

#ifdef POC
void poc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void poc_profile_operations (void);
#endif /* POC */

#ifdef SPC
void spc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void spc_profile_operations (void);
#endif /* SPC */

#ifdef GLC
void glc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void glc_profile_operations (void);
#endif /* GLC */

#ifdef TIC
void tic_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void tic_profile_operations (void);
#endif /* TIC */

#ifdef BAC
void bac_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void bac_profile_operations (void);
#endif /* BAC */

#ifdef DIC
void dic_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void dic_profile_operations (void);
#endif /* DIC */

#ifdef FTMC
void ftmc_notify_gatt_chardata(GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void ftmc_profile_operations(void);
#endif /* FTMC */

#ifdef ANCC
void ancc_notify_gatt_conn (void);
void ancc_notify_gatt_disconn (void);
void ancc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void ancc_profile_operations (void);
void ancc_parse_notification(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen);
#endif /* ANCC */

#ifdef RCC
void rcc_notify_gatt_chardata(GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void rcc_profile_operations(void);
#endif /* RCC */

#ifdef OTC
void otc_notify_gatt_chardata(GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void otc_profile_operations(void);
void otc_parse_indication_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen);
#endif /* OTC */

#ifdef IPSPR
void ipspr_notify_gatt_conn(void);
void ipspr_notify_gatt_disconn(void);
void ipspr_notify_gatt_servdata(GATT_SERVICE_PARAM * service, UINT16 size);
void ipspr_profile_operations(void);
#endif /* IPSPR */

#ifdef HPC
void hpc_notify_gatt_chardata(GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size);
void hpc_parse_notification_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen);
void hpc_parse_read_data (UCHAR * data, UINT16 datalen);
void hpc_profile_operations(void);
void hpc_notify_execute_write_rsp(void);
void hpc_notify_write_rsp(void);
#endif /* HPC*/

#endif /* _H_APPL_GATT_CLIENT_ */

