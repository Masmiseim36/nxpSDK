
/**
 *  \file smp_fsm.h
 *
 *  This file defines state and events related to SMP FSM.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SMP_FSM_
#define _H_SMP_FSM_


#if 0
char event_option[] =
"    --------- EVENTS ---------\n\
\n\
    0x01 -> ev_smp_pairing_req    \n\
    0x02 -> ev_smp_pairing_rsp    \n\
    0x03 -> ev_smp_pairing_rand    \n\
    0x04 -> ev_smp_pairing_confirm    \n\
    0x05 -> ev_smp_passkey    \n\
    0x06 -> ev_smp_pairing_stk    \n\
    0x07 -> ev_smp_encryption_complete    \n\
    0x08 -> ev_smp_ltk_req    \n\
    0x09 -> ev_smp_key_exchange_info    \n\
    0x0A -> ev_smp_pairing_failure    \n\
    0x0B -> ev_smp_public_key    \n\
    0x0C -> ev_smp_nkey_gen    \n\
    0x0D -> ev_smp_dhkey_check    \n\
    0x0E -> ev_smp_lesc_key_gen    \n\
    0x0F -> ev_smp_pairing_key_press    \n\
    0x10 -> ev_smp_nkey_cnf    \n\
    0xFF -> ev_smp_unknown    \n\
    18 -> Exit    \n\
\n\
Your Option - ";
#endif

/* Event Defines */
#if 0
typedef enum
{
    ev_smp_pairing_req = 0x01U,
    ev_smp_pairing_rsp = 0x02U,
    ev_smp_pairing_rand = 0x03U,
    ev_smp_pairing_confirm = 0x04U,
    ev_smp_passkey = 0x05U,
    ev_smp_pairing_stk = 0x06U,
    ev_smp_encryption_complete = 0x07U,
    ev_smp_ltk_req = 0x08U,
    ev_smp_key_exchange_info = 0x09U,
    ev_smp_pairing_failure = 0x0AU,
    ev_smp_public_key = 0x0BU,
    ev_smp_nkey_gen = 0x0CU,
    ev_smp_dhkey_check = 0x0DU,
    ev_smp_lesc_key_gen = 0x0EU,
    ev_smp_pairing_key_press = 0x0FU,
    ev_smp_nkey_cnf = 0x10U,
    ev_smp_unknown = 0xFFU

} SMP_EVENTS;
#else
#define ev_smp_pairing_req          0x01U
#define ev_smp_pairing_rsp          0x02U
#define ev_smp_pairing_rand         0x03U
#define ev_smp_pairing_confirm      0x04U
#define ev_smp_passkey              0x05U
#define ev_smp_pairing_stk          0x06U
#define ev_smp_encryption_complete  0x07U
#define ev_smp_ltk_req              0x08U
#define ev_smp_key_exchange_info    0x09U
#define ev_smp_pairing_failure      0x0AU
#define ev_smp_public_key           0x0BU
#define ev_smp_nkey_gen             0x0CU
#define ev_smp_dhkey_check          0x0DU
#define ev_smp_lesc_key_gen         0x0EU
#define ev_smp_pairing_key_press    0x0FU
#define ev_smp_nkey_cnf             0x10U
#define ev_smp_unknown              0xFFU
#endif /* 0 */
/* Level 0 State Defines */
#if 0
typedef enum
{
    SL_0_CONNECTED = 0x00000001U,
    SL_0_IN_PAIRING_RSP = 0x00000002U,
    SL_0_W4_PAIRING_RSP = 0x00000004U,
    SL_0_IN_RAND = 0x00000008U,
    SL_0_W4_RAND = 0x00000010U,
    SL_0_IN_CONFIRM = 0x00000020U,
    SL_0_W4_CONFIRM = 0x00000040U,
    SL_0_W4_RAND_CONFIRM = 0x00000080U,
    SL_0_IN_CONFIRMING = 0x00000100U,
    SL_0_IN_STK = 0x00000200U,
    SL_0_IN_PAIRING_ENC = 0x00000400U,
    SL_0_W4_PAIRING_ENC = 0x00000800U,
    SL_0_IN_ENCRYPTION = 0x00001000U,
    SL_0_IN_KEY_XCHG = 0x00002000U,
    SL_0_W4_KEY_XCHG = 0x00004000U,
    SL_0_IN_PUBLIC_KEY_XCHG = 0x00008000U,
    SL_0_W4_PUBLIC_KEY_XCHG = 0x00010000U,
    SL_0_W4_USER_PASSKEY = 0x00020000U,
    SL_0_IN_NKEY_GEN = 0x00040000U,
    SL_0_W4_NKEY_CONF = 0x00080000U,
    SL_0_IN_DHKEY_CHECK = 0x00100000U,
    SL_0_W4_DHKEY_CHECK = 0x00200000U,
    SL_0_IN_DHKEY_CHECK_CONFIRMING = 0x00400000U,
    SL_0_IN_LESC_KEY_GEN = 0x00800000U,
    SL_0_W4_NKEY_DHKEY_CONF = 0x01000000U
} SMP_STATES_LEVEL_0;
#else /* 0 */
#define SL_0_CONNECTED                  0x00000001U
#define SL_0_IN_PAIRING_RSP             0x00000002U
#define SL_0_W4_PAIRING_RSP             0x00000004U
#define SL_0_IN_RAND                    0x00000008U
#define SL_0_W4_RAND                    0x00000010U
#define SL_0_IN_CONFIRM                 0x00000020U
#define SL_0_W4_CONFIRM                 0x00000040U
#define SL_0_W4_RAND_CONFIRM            0x00000080U
#define SL_0_IN_CONFIRMING              0x00000100U
#define SL_0_IN_STK                     0x00000200U
#define SL_0_IN_PAIRING_ENC             0x00000400U
#define SL_0_W4_PAIRING_ENC             0x00000800U
#define SL_0_IN_ENCRYPTION              0x00001000U
#define SL_0_IN_KEY_XCHG                0x00002000U
#define SL_0_W4_KEY_XCHG                0x00004000U
#define SL_0_IN_PUBLIC_KEY_XCHG         0x00008000U
#define SL_0_W4_PUBLIC_KEY_XCHG         0x00010000U
#define SL_0_W4_USER_PASSKEY            0x00020000U
#define SL_0_IN_NKEY_GEN                0x00040000U
#define SL_0_W4_NKEY_CONF               0x00080000U
#define SL_0_IN_DHKEY_CHECK             0x00100000U
#define SL_0_W4_DHKEY_CHECK             0x00200000U
#define SL_0_IN_DHKEY_CHECK_CONFIRMING  0x00400000U
#define SL_0_IN_LESC_KEY_GEN            0x00800000U
#define SL_0_W4_NKEY_DHKEY_CONF         0x01000000U
#endif /* 0 */
#define NOP    0xFFU

#endif /* _H_SMP_FSM_ */

