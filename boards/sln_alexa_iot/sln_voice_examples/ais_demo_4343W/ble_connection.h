/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef BLE_CONNECTION_H_
#define BLE_CONNECTION_H_

/**
 * @brief Runs the BLE job for wifi provisioning
 *
 * The job purpose is to get the wifi credentials from the companion app:
 * -The device starts the ble server
 * -The WIFI CHARACTERISTIC within COMMISSIONING GATT is used for wifi credentials
 * -The job waits for the ble_cred_received_event which will be set after the
 *  WIFI CHARACTERISTIC was received
 * -The characteristic is parsed to check if a valid JSON was received
 * -The wifi credentials are then saved in flash
 *
 * This functions will block the thread execution until wifi credentials are received
 */
void BleProvisioning(void);

#endif /* BLE_CONNECTION_H_ */
