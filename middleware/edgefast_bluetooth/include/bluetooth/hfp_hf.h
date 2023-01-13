/** @file
 *  @brief Handsfree Profile handling.
 */

/*
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
/** @brief bt hfp ag volume type */
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


typedef struct _hf_waiting_call_state_t
{
    char     number[HFP_HF_DIGIT_ARRAY_SIZE];
    uint8_t   type;
    uint8_t   voice_class;
    uint8_t   alpha[HFP_HF_MAX_OPERATOR_NAME_LEN];
} hf_waiting_call_state_t;

/** @brief HFP profile application callback */
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
  
      /** HF notify command completed callback to application
       *
       *  The command sent from the application is notified about its status
       *
       * @param conn Connection object.
       * @param cmd structure contains status of the command including cme.
       */
      void (*cmd_complete_cb)(struct bt_conn *conn,
                            struct bt_hfp_hf_cmd_complete *cmd);
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

/** @brief Handsfree client Send AT
 *
 *  Send specific AT commands to handsfree client profile.
 *
 *  @param conn Connection object.
 *  @param cmd AT command to be sent.
 *
 *  @return 0 in case of success or negative value in case of error.
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

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_HFP_HF_H_ */
