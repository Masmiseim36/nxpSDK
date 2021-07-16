/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DEMO_CONFIG_H_
#define _DEMO_CONFIG_H_

/* Set SAMPLE_NUM number meassured sample  */
#define SAMPLE_NUM 1000
/* Set DATA_COLLECT to true for collecting data from sensors. */
#define DATA_COLLECT       false /* Set SAMPLE_NUM number meassured sample  */
#define EXAMPLE_NAME       "Anomaly detection"
#define FRAMEWORK_NAME     "TensorFlow Lite Micro"
#define DETECTION_TRESHOLD 5
#define PATCH_SIZE         5
#define NUM_CHANELS        6
#define NUM_RESULTS        1
#define DEMO_VERBOSE       false
#define MICRO_BUFF
#define EOL "\r\n"

#endif // _DEMO_CONFIG_H_
