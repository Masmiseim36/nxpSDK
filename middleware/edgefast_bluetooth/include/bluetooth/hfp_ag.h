/** @file
 *  @brief Handsfree AG Profile handling.
 */

/*
 * Copyright (c) 2021 NXP Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef INCLUDE_BLUETOOTH_HFP_AG_H_
#define INCLUDE_BLUETOOTH_HFP_AG_H_

/**
 * @brief Hands Free AG Profile (HFP AG)
 * @defgroup bt_hfp Hands Free Profile (HFP)
 * @ingroup bluetooth
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief bt hfp ag volume type */
typedef enum _hf_volume_type_t
{
    /*speaker */
    hf_volume_type_speaker, 
    /* mic */
    hf_volume_type_mic, 
} hf_volume_type_t;

/** @brief bt hf call status */
typedef enum _hfp_ag_call_status_t
{
    /*bt hf call end */
    hfp_ag_call_call_end = 0,
    /*bt hf call active */
    hfp_ag_call_call_active,
    /*bt hf call in coming call */
    hfp_ag_call_call_incoming,
    /*bt hf call out going call */
    hfp_ag_call_call_outgoing,
} hfp_ag_call_status_t;

/** @brief bt ag configure setting */
typedef struct _hfp_ag_get_config
{
	/*bt hfp ag phone number */
    char bt_hfp_ag_phnum[16];
	/*bt hfp ag phone number tag */
    char bt_hfp_ag_phnum_tag[16];
	/*bt hfp ag ring inband */
    uint8_t bt_hfp_ag_inband;
	/*bt hfp ag volume of speaker setting */
    uint8_t bt_hfp_ag_vgs;
	/*bt hfp ag volume of mic setting*/
    uint8_t bt_hfp_ag_vgm;
	/*bt hfp ag codec support */
    uint8_t bt_hfp_ag_codec;
	/*bt hfp ag codec negotiate support*/
    uint8_t bt_hfp_ag_codec_negotiate;
	/*bt hfp ag noise reduction and echo canceling support*/
    uint8_t bt_hfp_ag_nrec;
	/*bt hfp ag noise dial support*/
    uint8_t bt_hfp_ag_dial;
	/*bt hfp ag server channel value*/
    uint8_t server_channel;
} hfp_ag_get_config;

/** @brief bt ag call setup status */
typedef enum
{
    /*no  call setup in progress */
    HFP_AG_CALL_SETUP_STATUS_IDLE              = 0, 
    /* incoming call setup in progress */
    HFP_AG_CALL_SETUP_STATUS_INCOMING          = 1, 
    /*outgoing call setup in dialing state */
    HFP_AG_CALL_SETUP_STATUS_OUTGOING_DIALING  = 2, 
    /*outgoing call setup in alerting state */
    HFP_AG_CALL_SETUP_STATUS_OUTGOING_ALERTING = 3, 
} hfp_ag_call_setup_status_t;
/** @brief bt hf call status */
typedef struct _hfp_ag_cind_t
{
    /*bt hfp server  */
    uint8_t server;
    /*bt hfp call state  */
    uint8_t call_state;
    /*bt hfp call setup state  */
    hfp_ag_call_setup_status_t call_setup_state;
    /*bt hfp call held state */
    uint8_t call_held_state;
    /*bt hfp signal state */
    uint8_t signal;
    /*bt hfp roam state */
    uint8_t roam;
    /*bt hfp battery level */
    uint8_t batt_lev;
} hfp_ag_cind_t;

struct bt_hfp_ag;

/** @brief HFP profile application callback */
struct bt_hfp_ag_cb
{
    /** AG connected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection completes.
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     */
    void (*connected)(struct bt_hfp_ag *hfp_ag);
    /** AG disconnected callback to application
     *
     *  If this callback is provided it will be called whenever the
     *  connection gets disconnected, including when a connection gets
     *  rejected or cancelled or any error in SLC establisment.
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     */
    void (*disconnected)(struct bt_hfp_ag *hfp_ag);

    /** AG volume_control Callback
     *
     *  This callback provides volume_control indicator value to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param type the hfp volue type, for speaker or mic.
     *  @param value service indicator value received from the AG.
     */
    void (*volume_control)(struct bt_hfp_ag *hfp_ag, hf_volume_type_t type, int value);
    /** AG remote support feature Callback
     *
     *  This callback provides the remote hfp unit supported feature
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param value call indicator he remote hfp unit supported feature received from the AG.
     */
    void (*hfu_brsf)(struct bt_hfp_ag *hfp_ag, uint32_t value);
    /** AG remote call is answered Callback
     *
     *  This callback provides call indicator the call is answered to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     */
    void (*ata_response)(struct bt_hfp_ag *hfp_ag);
    /** AG remote call is answered Callback
     *
     *  This callback provides call indicator the call is rejected to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     */
    void (*chup_response)(struct bt_hfp_ag *hfp_ag);
    /** AG remote call is answered Callback
     *
     *  This callback provides call indicator the call is rejected to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param value  call information.
     */
    void (*dial)(struct bt_hfp_ag *hfp_ag, char *number);
    
    /** AG remote voice recognition activation Callback
     *
     *  This callback provides call indicator voice recognition activation of peer HF to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param value  voice recognition activation information.
     */    
     void (*brva)(struct bt_hfp_ag *hfp_ag, uint32_t value);
     
     /** AG remote noise reduction and echo canceling Callback
     *
     *  This callback provides call indicator voice recognition activation of peer HF to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param value  Noise Reduction and Echo Canceling information.
     */    
     void (*nrec)(struct bt_hfp_ag *hfp_ag, uint32_t value);
     /** AG remote codec negotiate Callback
     *
     *  This callback provides codec negotiate information of peer HF to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param value  codec index of peer HF.
     */    
     void (*codec_negotiate)(struct bt_hfp_ag *hfp_ag, uint32_t value);     
     
    
     /** AG multiparty call status indicator Callback
     *
     *  This callback provides multiparty call status indicator Callback of peer HF to the application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param option  Multiparty call option.
     *  @param index   Multiparty call index.
     */    
     void (*chld)(struct bt_hfp_ag *hfp_ag, uint8_t option, uint8_t index);
    /** AG unkown at Callback
     *
     *  This callback provides AG unkown at  value to the application, the unkown at command could be handled by application
     *
     *  @param hfp_ag  bt hfp ag Connection object.
     *  @param value unknow AT string buffer
     *  @param length unknow AT string length.
     */
    void (*unkown_at)(struct bt_hfp_ag *hfp_ag, char *value, uint32_t length);
};

/** @brief hfp_ag discover callback function 
 *
 *  @param conn   Pointer to bt_conn structure.
 *  @param channel the server channel of hfp ag
 */

typedef int (*bt_hfp_ag_discover_callback)(struct bt_conn *conn, uint8_t channel);

/** BT HFP AG Initialize 
 *
 *  This function called to initialize bt hfp ag
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */

int bt_hfp_ag_init(void);

/** BT HFP AG Deinitialize 
 *
 *  This function called to initialize bt hfp ag
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */

int bt_hfp_ag_deinit(void);

/** @brief hfp ag Connect.
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish hfp ag
 *  connection between devices.
 *  This function only establish RFCOM connection.
 *  After connection success, the callback that is registered by
 *  bt_hfp_ag_register_connect_callback is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param config  bt hfp ag congigure
 *  @param cb  bt hfp ag congigure
 *  @param phfp_ag  Pointer to pointer of bt hfp ag Connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_connect(struct bt_conn *conn,
                      hfp_ag_get_config *config,
                      struct bt_hfp_ag_cb *cb,
                      struct bt_hfp_ag **phfp_ag);
/** @brief hfp ag DisConnect.
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish hfp ag
 *  connection between devices.
 *  This function only establish RFCOM connection.
 *  After connection success, the callback that is registered by
 *  bt_hfp_ag_register_connect_callback is called.
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_disconnect(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag discover 
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish hfp ag
 *  connection between devices.
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param discoverCallback  pointer to discover callback function,defined in application
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_discover(struct bt_conn *conn, bt_hfp_ag_discover_callback discoverCallback);

/** @brief hfp ag open audio for codec 
 *
 *  This function is to open audio codec for hfp funciton
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *.
 */
void bt_hfp_ag_open_audio(struct bt_hfp_ag *hfp_ag, uint8_t codec);

/** @brief hfp ag close audio for codec 
 *
 *  This function is to close audio codec for hfp funciton
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *.
 */
void bt_hfp_ag_close_audio(struct bt_hfp_ag *hfp_ag);

/** @brief configure hfp ag supported features.
 *
 * if the function is not called, will use default supported features
 */

/** @brief hfp ag to configure hfp ag supported features
 *
 *  This function is to be configure hfp ag supported features.
 *  If the function is not called, will use default supported features
 *  
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param supported_features  suppported features of hfp ag
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_register_supp_features(struct bt_hfp_ag *hfp_ag, uint32_t supported_features);

 /** @brief hfp ag to get peer hfp hp support feautes 
  *
  *  This function is to be called to get hfp hp support feautes 
  *
  *  @param phfp_ag  pointer to bt hfp ag connection object
  *
  *  @return the supported feature of hfp ag
  */
uint32_t bt_hfp_ag_get_peer_supp_features(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag to configure hfp ag supported features
 *
 *  This function is to be configure hfp ag cind setting supported features.
 *  If the function is not called, will use default supported features
 *  
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param cind     pointer to  hfp ag cwind
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_register_cind_features(struct bt_hfp_ag *hfp_ag, char *cind);

/** @brief hfp ag to disable voice recognition 
  *
  *  This function is o disabl voice recognition 
  *
  *  @param phfp_ag  pointer to bt hfp ag connection object
  *
  *  @return 0 in case of success or otherwise in case
  *  of error.
  */
int bt_hfp_ag_send_disable_voice_recognition(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag to enable voice recognition 
 *
 *  This function is used to enable voice recognition
 *
 *  @param phfp_ag  pointer to bt hfp ag Connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_enable_voice_recognition(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag to disable noise reduction and echo canceling 
 *
 *  This function is o noise reduction and echo canceling  
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_disable_voice_ecnr(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag to enable noise reduction and echo canceling 
 *
 *  This function is to enable noise reduction and echo canceling 
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_enable_voice_ecnr(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag to set the name of the currently selected Network operator by AG
 *
 *  This function is to set the name of the currently selected Network operator by AG
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param name     the name of the currently selected Network operator by AG
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_set_cops(struct bt_hfp_ag *hfp_ag, char *name);

/** @brief hfp ag to set volue of hfp hp
 *
 *  This function is to set volue of hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param type     the hfp hp volume type
 *  @param value    the volue of volume
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int  bt_hfp_ag_set_volume_control(struct bt_hfp_ag *hfp_ag, hf_volume_type_t type, int value);

/** @brief hfp ag to set inband ring tone support
 *
 *  This function is to set inband ring tone support
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value   the inband ring tone type
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int  bt_hfp_ag_set_inband_ring_tone(struct bt_hfp_ag *hfp_ag, int value);

/** @brief hfp ag to set the attach a phone number to a voice Tag
 *
 *  This function is to set the attach a phone number to a voice Tag
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param name     the name of attach a phone number to a voice Tag
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int  bt_hfp_ag_set_phnum_tag(struct bt_hfp_ag *hfp_ag, char *name);

/** @brief hfp ag to set the call status
 *
 *  This function is to set the call status
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param status   the ag call status
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
void bt_hfp_ag_call_status_pl(struct bt_hfp_ag *hfp_ag, hfp_ag_call_status_t status);

/** @brief hfp ag to set the status of the "Response and Hold" state of the AG.
 *
 *  This function is to hfp ag to set the status of the "Response and Hold" state of the AG.
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param option   the hfp ag  "Response and Hold" state of the AG
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_handle_btrh(struct bt_hfp_ag *hfp_ag, uint8_t option);

/** @brief hfp ag to set the status of the "Response and Hold" state of the AG.
  *
  *  This function is to hfp ag to set the status of the "Response and Hold" state of the AG.
  *
  *  @param phfp_ag  pointer to bt hfp ag connection object
  *  @param item    1 for Enhanced Safety, 2 for Battery Level
  *  @param enable  1 for enable
  *
  *  @return 0 in case of success or otherwise in case
  *  of error.
  */
int bt_hfp_ag_handle_indicator_enable(struct bt_hfp_ag *hfp_ag, uint8_t index, uint8_t enable);

/** @brief hfp ag to set ring command to hfp hp
 *
 *  This function is hfp ag to set ring command to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *
 */
void bt_hfp_ag_send_callring(struct bt_hfp_ag *hfp_ag);

/** @brief hfp ag set call indicator to hfp hp
 *
 *  This function is  hfp ag set call indicator to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of call indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_call_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set call setup indicator to hfp hp
 *
 *  This function is  hfp ag set call setup indicator to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of call setup indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_callsetup_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set service indicator to hfp hp
 *
 *  This function is hfp ag set service indicator to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of service indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_service_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set signal strength indicator to hfp hp
 *
 *  This function is hfp ag set signal strength indicator to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of signal strength indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_signal_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set roaming indicator to hfp hp
 *
 *  This function is hfp ag set roaming indicator to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of roaming indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_roaming_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set battery level indicator to hfp hp
 *
 *  This function is hfp ag set battery level indicator to hfp hp
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of battery level indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_battery_indicator(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set ccwa indicator to hfp hp
 *
 *  This function is hfp ag set ccwa indicator to hfp hp for mutiple call
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of battery level indicator
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_send_ccwa_indicator(struct bt_hfp_ag *hfp_ag, char *number);

/** @brief hfp ag set codec selector to hfp hp
 *
 *  This function is hfp ag set odec selector to hfp hp for codec negotiation
 *
 *  @param phfp_ag  pointer to bt hfp ag connection object
 *  @param value    value of codec selector
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_codec_selector(struct bt_hfp_ag *hfp_ag, uint8_t value);

/** @brief hfp ag set unknown at command response to hfp fp
 *
 *  This function is hfp ag set unknown at command response to hfp fp, the command is not supported on hfp ag profile,
 *  Need handle the unknown command on application
 *
 *  @param phfp_ag             pointer to bt hfp ag connection object
 *  @param unknow_at_rsp       string of unkown at command response
 *  @param unknow_at_rsplen    string length of unkown at command response
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_ag_unknown_at_response(struct bt_hfp_ag *hfp_ag, uint8_t *unknow_at_rsp, uint16_t unknow_at_rsplen);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* INCLUDE_BLUETOOTH_HFP_AG_H_ */
