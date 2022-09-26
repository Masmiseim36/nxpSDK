
/**
 *  \file appl_hfp_ag_atrsp.h
 *
 *  Internal header file of HFP AG application.
 *  This file contains the AT response string definitions
 *  used by HFP AG application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HFP_AG_ATRSP_
#define _H_APPL_HFP_AG_ATRSP_

/* --------------------------------------------- Header File Inclusion */
#include "BT_at_parser_api.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

#define HFP_1_5_RSP

#ifdef HFP_1_5_RSP

#define HFP_RING           (UCHAR*)"\r\nRING\r\n"
#define HFP_OK             (UCHAR*)"\r\nOK\r\n"
#define HFP_ERROR          (UCHAR*)"\r\nERROR\r\n"
#define HFP_BRSF           (UCHAR*)"\r\n+BRSF:33\r\n\r\nOK\r\n"
#define HFP_CIND_SUP       (UCHAR*)"\r\n+CIND: (\"call\",(0-1)),\
(\"callsetup\",(0-3)),(\"callheld\",(0-2)),(\"service\",(0-1))\r\n\r\nOK\r\n"
#define HFP_CIND_STATUS    (UCHAR*)"\r\n+CIND: 0,0,0,1\r\n\r\nOK\r\n"
#define HFP_BSIR_YES       (UCHAR*)"\r\n+BSIR: 1\r\n"
#define HFP_BSIR_NO        (UCHAR*)"\r\n+BSIR: 0\r\n"
#define HFP_CHLD           (UCHAR*)"\r\n+CHLD: (2)\r\n\r\nOK\r\n"
#define HFP_CCWA           (UCHAR*)"\r\n+CCWA: "",128\r\n"
#define HFP_CIEV_CALL(n)   (UCHAR*)"\r\n+CIEV: 1," #n "\r\n"
#define HFP_CIEV_CSETUP(n) (UCHAR*)"\r\n+CIEV: 2," #n "\r\n"
#define HFP_CIEV_CHELD(n)  (UCHAR*)"\r\n+CIEV: 3," #n "\r\n"
#define HFP_BTRH(n)        (UCHAR*)"\r\n+BTRH: " #n "\r\n"

#endif /* HFP_1_5_RSP */

#ifdef HFP_AG_1_0_RSP /* Response as per HFP 1.0 version */

char *at_cmd_option[28] = {
"",/* UnUsed - Reserved */
"",/* UnUsed - Reserved */
"",/* UnUsed - Reserved */
"\r\n+CIEV: 1,0\r\n",
"\r\n+CIEV: 1,1\r\n",
"\r\nRING\r\n",
"\r\n+BSIR: 1\r\n",
"\r\n+BSIR: 0\r\n",
"\r\n+BVRA: 1\r\n",
"\r\n+BVRA: 0\r\n",
"\r\nOK\r\n",
"\r\nERROR\r\n",
"\r\n+CCWA: \"123456789\",129\r\n",
"\r\n+BINP: 2387422834\r\n",
"\r\n+VGS: 7\r\n",
"\r\n+VGS: 0\r\n",
"\r\n+VGS: 15\r\n",
"\r\n+VGM: 7\r\n",
"\r\n+VGM: 0\r\n",
"\r\n+VGM: 15\r\n",
"\r\n+CIEV: 3,0\r\n",
"\r\n+CIEV: 3,1\r\n",
"\r\n+CIEV: 3,2\r\n",
"\r\n+CIEV: 3,3\r\n",
"\r\n+CLIP: \"+9108026722777\",145\r\n",
"\r\n+CIEV: 2,1 \r\n",
"\r\n+CIEV: 2,0 \r\n",
"\r\n+CIND: 0,1,0\r\n"
};


char *AT_cmd_menu =
"------------------------------------------\
\n 0  -> Return\
\n 1  -> Send the default command/response\
\n 2  -> \\r\\n+CIND: (\"call\",(0,1)),(\"service\",(0,1)),(\"call_setup\",(0,3))\
\\r\\n\\r\\nOK\\r\\n\
\n 3  -> \\r\\n+CIEV: 1,0\\r\\n\
\n 4  -> \\r\\n+CIEV: 1,1\\r\\n\
\n 5  -> \\r\\nRING\\r\\n\
\n 6  -> \\r\\n+BSIR: 1\\r\\n\
\n 7 -> \\r\\n+BSIR: 0\\r\\n\
\n 8 -> \\r\\n+BVRA: 1\\r\\n\
\n 9 -> \\r\\n+BVRA: 0\\r\\n\
\n 10 -> \\r\\nOK\\r\\n\
\n 11 -> \\r\\nERROR\\r\\n\
\n 12 -> \\r\\n+CCWA: \"123456789\",129\\r\\n\
\n 13 -> \\r\\n+BINP: 2387422834\\r\\n\
\n 14 -> \\r\\n+VGS: 7\\r\\n\
\n 15 -> \\r\\n+VGS: 15\\r\\n\
\n 16 -> \\r\\n+VGS: 0\\r\\n\
\n 17 -> \\r\\n+VGM: 7\\r\\n\
\n 18 -> \\r\\n+VGM: 15\\r\\n\
\n 19 -> \\r\\n+VGM: 0\\r\\n\
\n 20 -> \\r\\n+CIEV: 3,0\\r\\n\
\n 21 -> \\r\\n+CIEV: 3,1\\r\\n\
\n 22 -> \\r\\n+CIEV: 3,2\\r\\n\
\n 23 -> \\r\\n+CIEV: 3,3\\r\\n\
\n 24 -> \\r\\n+CLIP: \"+9108026722777\",145\\r\\n\
\n 25 -> \\r\\n +CIEV: 2,1 \\r\\n\
\n 26 -> \\r\\n +CIEV: 2,0 \\r\\n\
\n 27 -> \\r\\n+CIND: 0,1,0\\r\\n\
\n Enter your choice \n";

#endif /* HFP_AG_1_0_RSP */

#ifdef HFP_AG_1_5_RSP /* Response as per HFP 1.5 version */

char *at_cmd_option[54] = {
"",/* UnUsed - Reserved */
"",/* UnUsed - Reserved */
"",/* UnUsed - Reserved */
"\r\n+CIEV: 1,3\r\n",
"\r\n+CIEV: 1,0\r\n",
"\r\nRING\r\n",
"\r\n+BSIR: 1\r\n",
"\r\n+BSIR: 0\r\n",
"\r\n+BVRA: 1\r\n",
"\r\n+BVRA: 0\r\n",
"\r\nOK\r\n",
"\r\nERROR\r\n",
"\r\n+COPS: 0,0,\"NWSIM\"\r\n",
"\r\n+COPS: 0,0,\"NWsimulator\"\r\n",
"\r\n+CCWA: 123456789,129\r\n",
"\r\n+BINP: 2387422834\r\n",
"\r\n+VGS: 7\r\n",
"\r\n+VGS: 0\r\n",
"\r\n+VGS: 15\r\n",
"\r\n+VGM: 7\r\n",
"\r\n+VGM: 0\r\n",
"\r\n+VGM: 15\r\n",
"\r\n+CIEV: 2,0\r\n",
"\r\n+CIEV: 2,2\r\n",
"\r\n+CIEV: 2,3\r\n",
"\r\n+CIEV: 2,5\r\n",
"\r\n+BTRH: 0\r\n",
"\r\n+BTRH: 1\r\n",
"\r\n+BTRH: 2\r\n",
"\r\n+CNUM: \"+358501234567\",145,4\r\n",
"\r\n+CLIP: \"+9108026722777\",145\r\n",
"\r\n+CIEV: 9,0 \r\n",
"\r\n+CIEV: 9,1 \r\n",
"\r\n+CLCC: 1,1,1,0,0,\"+18015552000\",129\r\n",
"\r\n+CLCC: 1,1,0,0,1,\"+99080232456678\",145\r\n",
"\r\n+CIEV: 5,0 \r\n",
"\r\n+CIEV: 5,1 \r\n",
"\r\n+CIEV: 8,0 \r\n",
"\r\n+CIEV: 8,1 \r\n",
"\r\n+CIEV: 10,0 \r\n",
"\r\n+CIEV: 10,1 \r\n",
"\r\n+CIEV: 10,2 \r\n",
"\r\n+CIEV: 10,3 \r\n",
"\r\n+CIEV: 11,0 \r\n",
"\r\n+CIEV: 11,1 \r\n",
"\r\n+CIEV: 11,2 \r\n",
"\r\n+CME ERROR: 1\r\n",
"\r\n+CME ERROR: 3\r\n",
"\r\n+CME ERROR: 4\r\n",
"\r\n+CME ERROR: 26\r\n",
"\r\n+CME ERROR: 27\r\n",
"\r\n+CME ERROR: 30\r\n",
"\r\n+CME ERROR: 32\r\n",
"\r\n+CIND: 4,4,0,1,1,0,1,0,0,0,0\r\n"
};


char *AT_cmd_menu =
"------------------------------------------\
\n 0  -> Return\
\n 1  -> Send the default command/response\
\n 2  -> \r\n+CIND: (\"battchg\",(0-5)),(\"signal\",(0-5)),(\"batterywarning\",(0-1)),\
(\"chargerconnected\",(0-1)),(\"service\",(0-1)),(\"sounder\",(0-1)),(\"message\",(0-1)),\
(\"call\",(0-1)),(\"roam\",(0-1)),(\"callsetup\",(0-3)),(\"callheld\",(0-2))\\r\\n\
\n 3  -> \\r\\n+CIEV: 1,3\\r\\n\
\n 4  -> \\r\\n+CIEV: 1,0\\r\\n\
\n 5  -> \\r\\nRING\\r\\n\
\n 6  -> \\r\\n+BSIR: 1\\r\\n\
\n 7  -> \\r\\n+BSIR: 0\\r\\n\
\n 8  -> \\r\\n+BVRA: 1\\r\\n\
\n 9  -> \\r\\n+BVRA: 0\\r\\n\
\n 10 -> \\r\\nOK\\r\\n\
\n 11 -> \\r\\nERROR\\r\\n\
\n 12 -> \\r\\n+COPS: 0,0,\"NWSIM\"\\r\\n\
\n 13 -> \\r\\n+COPS: 0,0,\"NWsimulator\"\\r\\n\
\n 14 -> \\r\\n+CCWA: 123456789,129\\r\\n\
\n 15 -> \\r\\n+BINP: 2387422834\\r\\n\
\n 16 -> \\r\\n+VGS: 7\\r\\n\
\n 17 -> \\r\\n+VGS: 0\\r\\n\
\n 18 -> \\r\\n+VGS: 15\\r\\n\
\n 19 -> \\r\\n+VGM: 7\\r\\n\
\n 20 -> \\r\\n+VGM: 0\\r\\n\
\n 21 -> \\r\\n+VGM: 15\\r\\n\
\n 22 -> \\r\\n+CIEV: 2,0\\r\\n\
\n 23 -> \\r\\n+CIEV: 2,2\\r\\n\
\n 24 -> \\r\\n+CIEV: 2,3\\r\\n\
\n 25 -> \\r\\n+CIEV: 2,5\\r\\n\
\n 26 -> \\r\\n+BTRH: 0\\r\\n\
\n 27 -> \\r\\n+BTRH: 1\\r\\n\
\n 28 -> \\r\\n+BTRH: 2\\r\\n\
\n 29 -> \\r\\n+CNUM: ,\"+358501234567\",145,4\\r\\n\
\n 30 -> \\r\\n+CLIP: \"+9108026722777\",145\\r\\n\
\n 31 -> \\r\\n +CIEV: 9,0 \\r\\n\
\n 32 -> \\r\\n +CIEV: 9,1 \\r\\n\
\n 33 -> \\r\\n+CLCC: 1,1,1,0,0,\"+18015552000\",129\\r\\n\
\n 34 -> \\r\\n+CLCC: 1,1,0,0,1,\"+99080232456678\",145\\r\\n\
\n 35 -> \\r\\n+CIEV: 5,0 \\r\\n\
\n 36 -> \\r\\n+CIEV: 5,1 \\r\\n\
\n 37 -> \\r\\n+CIEV: 8,0 \\r\\n\
\n 38 -> \\r\\n+CIEV: 8,1 \\r\\n\
\n 39 -> \\r\\n+CIEV: 10,0 \\r\\n\
\n 40 -> \\r\\n+CIEV: 10,1 \\r\\n\
\n 41 -> \\r\\n+CIEV: 10,2 \\r\\n\
\n 42 -> \\r\\n+CIEV: 10,3 \\r\\n\
\n 43 -> \\r\\n+CIEV: 11,0 \\r\\n\
\n 44 -> \\r\\n+CIEV: 11,1 \\r\\n\
\n 45 -> \\r\\n+CIEV: 11,2 \\r\\n\
\n 46 -> \\r\\n+CME ERROR: 1 \\r\\n\
\n 47 -> \\r\\n+CME ERROR: 3 \\r\\n\
\n 48 -> \\r\\n+CME ERROR: 4 \\r\\n\
\n 49 -> \\r\\n+CME ERROR: 26 \\r\\n\
\n 50 -> \\r\\n+CME ERROR: 27 \\r\\n\
\n 51 -> \\r\\n+CME ERROR: 30 \\r\\n\
\n 52 -> \\r\\n+CME ERROR: 32 \\r\\n\
\n 53 -> \\r\\n+CIND: 4,4,0,1,1,0,1,0,0,0,0 \\r\\n\
\n Enter your choice \n";

#endif /* HFP_AG_1_5_RSP */

#endif /* _H_APPL_HFP_AG_ATRSP_ */


