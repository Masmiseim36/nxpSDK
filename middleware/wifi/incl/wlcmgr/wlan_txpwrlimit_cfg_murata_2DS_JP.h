/** @file wlan_txpwrlimit_cfg_murata_2DS_JP.h
 *
 *  @brief  This file provides Murata 2DS WLAN JP Tx Power Limit APIs.
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

static wifi_txpwrlimit_t
    wwsm_txpwrlimit_2g_cfg =
        {
            .subband   = (wifi_SubBand_t)0x00,
            .num_chans = 14,
            .txpwrlimit_config[0] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 1,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[1] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 2,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[2] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 3,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[3] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 4,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[4] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 5,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[5] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 6,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[6] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 7,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[7] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 8,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[8] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 9,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[9] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 10,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[10] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 11,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[11] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 12,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[12] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 13,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
            .txpwrlimit_config[13] =
                {
                    .num_mod_grps = 10,
                    .chan_desc =
                        {
                            .start_freq = 2414,
                            .chan_width = 20,
                            .chan_num   = 14,
                        },
                    .txpwrlimit_entry =
                        {{0, 14}, {1, 15}, {2, 14}, {3, 14}, {4, 15}, {5, 15}, {6, 14}, {7, 15}, {8, 15}, {9, 14}},
                },
};
