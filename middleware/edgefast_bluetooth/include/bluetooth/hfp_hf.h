/** @file
 *  @brief Handsfree Profile handling.
 */

/*
 * Copyright 2024 NXP
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_HFP_HF_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_HFP_HF_H_

/**
 * @brief Hands Free Profile (HFP)
 * @defgroup bt_hfp Hands Free Profile (HFP)
 * @ingroup bluetooth
 * @{
 */

#include <bluetooth/bluetooth.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AT Commands */
enum bt_hfp_hf_at_cmd {
	BT_HFP_HF_ATA,
	BT_HFP_HF_AT_CHUP,
};

#define HFP_HF_DIGIT_ARRAY_SIZE                       32
#define HFP_HF_MAX_OPERATOR_NAME_LEN                  16

/*
 * Command complete types for the application
 */
#define HFP_HF_CMD_OK             0
#define HFP_HF_CMD_ERROR          1
#define HFP_HF_CMD_CME_ERROR      2
#define HFP_HF_CMD_UNKNOWN_ERROR  4

/** @brief HFP HF Command completion field */
struct bt_hfp_hf_cmd_complete {
	/* Command complete status */
	uint8_t type;
	/* CME error number to be added */
	uint8_t cme;
};
/** @brief bt hfp volume type */
typedef enum _hf_volume_type_t
{
    /*speaker */
    hf_volume_type_speaker = 0,
    /* mic */
    hf_volume_type_mic,
} hf_volume_type_t;

/** @brief bt hfp ag volume type */
typedef enum _hf_multiparty_call_option_t
{
    /*Release All Call and set UDUB */
    hf_multiparty_call_option_one,
    /* Release All Active Call and accepts held/waiting Call */
    hf_multiparty_call_option_two,
    /* Place Active Call(if any) on hold and accepts held/waiting Call */
    hf_multiparty_call_option_three,
    /* Add Call to the Conefrencing */
    hf_multiparty_call_option_four,
    /* Connect two Call and disconnect Send event HFP_HFU_HOLD_CALL (with input value) */
    hf_multiparty_call_option_five,
} hf_multiparty_call_option_t;

/** @brief bt hf configure setting */
typedef struct _hfp_hf_get_config
{
    /*bt hfp hf volume of speaker setting */
    uint8_t bt_hfp_hf_vgs;
    /*bt hfp hf volume of mic setting*/
    uint8_t bt_hfp_hf_vgm;
} hfp_hf_get_config;

typedef struct _hf_waiting_call_state_t
{
    char     number[HFP_HF_DIGIT_ARRAY_SIZE];
    uint8_t   type;
    uint8_t   voice_class;
    uint8_t   alpha[HFP_HF_MAX_OPERATOR_NAME_LEN];
} hf_waiting_call_state_t;

typedef struct _hf_indicator_status_t
{
    /* "service" indicator */
    uint8_t service;
    /* "call" indicator */
    uint8_t call;
    /* "callsetup" or "call_setup" indicator */
    uint8_t callsetup;
    /* "signal" indicator */
    uint8_t signal;
    /* "roam" indicator */
    uint8_t roam;
    /* "battchg" indicator */
    uint8_t battchg;
    /* "callheld" indicator */
    uint8_t callheld;
} hf_indicator_status_t;

/** @brief HFP profile application callback
  *
  * @note Please note that blocking function calls should be not be called
  *  the callback function, otherwise, tThese blocking callbacks can potentially
  *  cause issues with the entire HFP AT Command state machine, preventing it from
  *  continuing to process subsequent AT commands.
  *
  */
struct bt_hfp_hf_cb {
	/** HF connected callback to application
	 *
	 *  If this callback is provided it will be called whenever the
	 *  connection completes.
	 *
	 *  @param conn Connection object.
	 */
	void (*connected)(struct bt_conn *conn);
	/** HF disconnected callback to application
	 *
	 *  If this callback is provided it will be called whenever the
	 *  connection gets disconnected, including when a connection gets
	 *  rejected or cancelled or any error in SLC establishment.
	 *
	 *  @param conn Connection object.
	 */
	void (*disconnected)(struct bt_conn *conn);
	/** HF SCO/eSCO connected Callback
	 *
	 *  If this callback is provided it will be called whenever the
	 *  SCO/eSCO connection completes.
	 *
	 *  @param conn Connection object.
	 *  @param sco_conn SCO/eSCO Connection object.
	 */
	void (*sco_connected)(struct bt_conn *conn, struct bt_conn *sco_conn);
	/** HF SCO/eSCO disconnected Callback
	 *
	 *  If this callback is provided it will be called whenever the
	 *  SCO/eSCO connection gets disconnected.
	 *
	 *  @param conn Connection object.
	 *  @param reason BT_HCI_ERR_* reason for the disconnection.
	 */
	void (*sco_disconnected)(struct bt_conn *sco_conn, uint8_t reason);
	/** HF indicator Callback
	 *
	 *  This callback provides service indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value service indicator value received from the AG.
	 */
	void (*service)(struct bt_conn *conn, uint32_t value);
	/** HF indicator Callback
	 *
	 *  This callback provides call indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value call indicator value received from the AG.
	 */
	void (*call)(struct bt_conn *conn, uint32_t value);
	/** HF indicator Callback
	 *
	 *  This callback provides call setup indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value call setup indicator value received from the AG.
	 */
	void (*call_setup)(struct bt_conn *conn, uint32_t value);
	/** HF indicator Callback
	 *
	 *  This callback provides call held indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value call held indicator value received from the AG.
	 */
	void (*call_held)(struct bt_conn *conn, uint32_t value);
	/** HF indicator Callback
	 *
	 *  This callback provides signal indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value signal indicator value received from the AG.
	 */
	void (*signal)(struct bt_conn *conn, uint32_t value);
	/** HF indicator Callback
	 *
	 *  This callback provides roaming indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value roaming indicator value received from the AG.
	 */
	void (*roam)(struct bt_conn *conn, uint32_t value);
	/** HF indicator Callback
	 *
	 *  This callback battery service indicator value to the application
	 *
	 *  @param conn Connection object.
	 *  @param value battery indicator value received from the AG.
	 */
	void (*battery)(struct bt_conn *conn, uint32_t value);

 	/** HF voice tag phnum indicator Callback
	 *
	 *  This callback voice tag phnum indicator to the application
	 *
	 * @param conn Connection object.
	 * @param voice tag phnum value received from the AG.
	 */
	void (*voicetag_phnum)(struct bt_conn *conn, char* number);

	/** HF calling phone number string indication callback to application
	 *
	 *  If this callback is provided it will be called whenever there
	 *  is an incoming call and bt_hfp_hf_enable_clip_notification is called.
	 *
	 * @param conn Connection object.
	 * @param char to phone number string.
	 */
       void (*call_phnum)(struct bt_conn *conn, char* number);

      /** HF waiting call indication callback to application
       *
       *  If this callback is provided it will be called in waiting call state
       *
       * @param  conn Connection object.
       * @param pointer to waiting call state information.
       */

      void (*waiting_call)(struct bt_conn *conn, hf_waiting_call_state_t* wcs);
      /** HF incoming call Ring indication callback to application
       *
       *  If this callback is provided it will be called whenever there
       *  is an incoming call.
       *
       * @param conn Connection object.
       */

      void (*ring_indication)(struct bt_conn *conn);

      /** HF volume update indication callback to application
       *
       *  If this callback is provided it will be called whenever there
       *  is an volume update event.
       *
       * @param conn Connection object.
       * @param type bt hfp volume type.
       * @param volume bt hfp volume level value.
       */

      void (*volume_update)(struct bt_conn *conn,  hf_volume_type_t type, int volume);

      /** Indicator status.
       *
       *  This callback is used to notify the peer's indicators' status
       *
       * @param conn Connection object.
       * @param status structure contains the result of indicator status.
       */
      void (*indicator_status)(struct bt_conn *conn,
                               hf_indicator_status_t *status);

      /** HF notify command completed callback to application
       *
       *  The command sent from the application is notified about its status
       *
       * @param conn Connection object.
       * @param cmd structure contains status of the command including cme.
       */
      void (*cmd_complete_cb)(struct bt_conn *conn,
                            struct bt_hfp_hf_cmd_complete *cmd);

      /** HF notify inband ringtone set status callback to application
       *
       *  If this callback is provided it will be called whenever there
       *  is an volume update event.
       *
       * @param conn Connection object.
       * @param set inband ringtone set status
       */      
      void (*ringtone_inband_set)(struct bt_conn *conn, uint8_t set);

      /** Get config before connection.
       *
       *  This callback is used to get config #hfp_hf_get_config
       *
       *  @param conn Connection object.
       *  @param config get the config from upper layer.
       */
      void (*get_config)(hfp_hf_get_config **config);
};

/** @brief Register HFP HF profile
 *
 *  Register Handsfree profile callbacks to monitor the state and get the
 *  required HFP details to display.
 *
 *  @param cb callback structure.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_hfp_hf_register(struct bt_hfp_hf_cb *cb);


/** @brief hfp hf Connect.
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to establish hfp hf
 *  connection between devices.
 *  This function only establish RFCOM connection.
 *  After connection success, the callback that is registered by
 *  bt_hfp_hf_register is called.
 *
 *  @param conn Pointer to bt_conn structure.
 *  @param channel The refcom service channel.
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_hf_connect(struct bt_conn *conn, uint8_t channel);
/** @brief hfp hf DisConnect.
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to disconnect hfp ag
 *  connection between devices.
 *  This function only disconnect RFCOM connection.
 *  After connection success, the callback that is registered by
 *  bt_hfp_hf_register is called.
 *
 *  @param conn  pointer to connection object
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_hf_disconnect(struct bt_conn *conn);

/** @brief hfp_hf discover callback function
 *
 *  @param conn   Pointer to bt_conn structure.
 *  @param channel the server channel of hfp ag
 */

typedef int (*bt_hfp_hf_discover_callback)(struct bt_conn *conn, uint8_t channel);

/** @brief hfp hf discover
 *
 *  This function is to be called after the conn parameter is obtained by
 *  performing a GAP procedure. The API is to be used to find the ag's rfcomm
 *  service channel.
 *
 *  @param conn  pointer to bt connection object
 *  @param discoverCallback  pointer to discover callback function,defined in application
 *
 *  @return 0 in case of success or otherwise in case
 *  of error.
 */
int bt_hfp_hf_discover(struct bt_conn *conn, bt_hfp_hf_discover_callback discoverCallback);

/** @brief Handsfree client Send AT
 *
 *  Send specific AT commands to handsfree client profile.
 *
 *  @param conn Connection object.
 *  @param cmd AT command to be sent.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  @note Please note that send_cmd function and the following HFP APIs may return -EBUSY indicating a
 *  failure in sending with busy, requiring the application to retry sending. The possible reasons for this
 *  failure could be that an AT command is currently being sent by the application, or it could be
 *  caused by the HFP profile internally sending AT commands (such as AT+VGS, AT+BCS, AT+CLIP, AT+CCWA),
 *  resulting in the application's AT command being in a busy state.
 *
*/
int bt_hfp_hf_send_cmd(struct bt_conn *conn, enum bt_hfp_hf_at_cmd cmd);

/**
 *
 * @brief   Handsfree to enable voice recognition in the AG.
 *
 * @param conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_start_voice_recognition(struct bt_conn *conn);

/**
 *
 * @brief  Handsfree to Disable voice recognition in the AG.
 *
 * @param conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_stop_voice_recognition(struct bt_conn *conn);

/**
 *
 * @brief           Handsfree to update Volume  with AG.
 *
 * @param     conn Connection object.
 * @param     type volume control target, speaker or microphone
 * @param     volume gain of the speaker of microphone, ranges 0 to 15
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_volume_update(struct bt_conn *conn, hf_volume_type_t type, int volume);

/**
 *
 * @brief           Place a call with a specified number, if number is NULL, last called number is
 *                  called.  As a precondition to use this API, Service Level Connection shall
 *                  exist with AG
 *
 * @param       conn Connection object.
 * @param       number number string of the call. If NULL, the last number is called(aka re-dial)
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_dial(struct bt_conn *conn, const char *number);

/**
 *
 * @brief           Place a call with number specified by location(speed dial). As a precondition,
 *                  to use this API, Service Level Connection shall exist with AG
 *
 * @param       conn Connection object.
 * @param       location location of the number in the memory
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_dial_memory(struct bt_conn *conn, int location);

/**
 *
 * @brief           Place a call with number specified by location(speed dial). As a precondition,
 *                  to use this API, Service Level connection shall exist with AG
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_last_dial(struct bt_conn *conn);

/**
 *
 * @brief           Place a call with number specified by location(speed dial). As a precondition,
 *                  to use this API, Service Level Connection shall exist with AG
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_multiparty_call_option(struct bt_conn *conn, hf_multiparty_call_option_t option);

/**
 *
 * @brief      Enable the CLIP notification
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_enable_clip_notification(struct bt_conn *conn);
/**
 *
 * @brief       Disable the CLIP notification
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_disable_clip_notification(struct bt_conn *conn);
/**
 *
 * @brief     Enable the call waiting notification
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_enable_call_waiting_notification(struct bt_conn *conn);

/**
 *
 * @brief     Disable the call waiting notification
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_disable_call_waiting_notification(struct bt_conn *conn);

/**
 *
 * @brief     Get the last voice tag nubmer, the mubmer will be fill callback event voicetag_phnum.
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_get_last_voice_tag_number(struct bt_conn *conn);

/**
 *
 * @brief     Trigger codec connection
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_trigger_codec_connection(struct bt_conn *conn);

/**
 *
 * @brief     Get peer indicators' status
 *
 *
 * @param       conn Connection object.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
 
int bt_hfp_hf_get_peer_indicator_status(struct bt_conn *conn);

/** @brief hfp hf open audio for codec
 *
 *  This function is to open audio codec for hfp funciton
 *
 * @param  conn  Connection object.
 * @param  codec HFP Codec Id.
 *
 *  @return 0 in case of success or negative value in case of error.
 */
int bt_hfp_hf_open_audio(struct bt_conn *conn, uint8_t codec);

/** @brief hfp hf close audio for codec
 *
 *  This function is to close audio codec for hfp funciton
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_hfp_hf_close_audio(struct bt_conn *sco_conn);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_HFP_HF_H_ */
