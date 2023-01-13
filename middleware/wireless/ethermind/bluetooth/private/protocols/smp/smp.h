/**
 *  \file smp.h
 *
 *  This file contains the fuction definitions which are exported to other
 *  EtherMind modules for interfacing with Security Manager Protocol.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_SMP_
#define _H_SMP_

/** ============================================= Header File Inclusion */
#include "BT_common.h"

/** ============================================= Global Definitions */

/** ============================================= Structures/Data Types */

/** ============================================= Macros */

/* Create 8 octets of random number */
#define smp_hci_rand BT_hci_le_rand

/* Encry the plain text */
#define smp_hci_encrypt BT_hci_le_encrypt

/** ============================================= Internal Functions */

/** ============================================= API Declarations */

void em_smp_init (void);
#ifdef BT_HAVE_SHUTDOWN
void em_smp_shutdown(void);
#endif /* BT_HAVE_SHUTDOWN */
void smp_bt_init (void);

void smp_bt_shutdown (void);

void smp_command_complete
     (
         UCHAR  * packet
     );

void smp_command_status
     (
         UCHAR  * packet
     );

void smp_encryption_complete
     (
         UINT16 handle,
         UCHAR status
     );

void smp_long_term_key_requested (UCHAR * packet);

#ifdef SMP_LESC
void smp_p256_pub_key_read_complete_event (UCHAR * packet, UCHAR status);
void smp_gen_dhkey_complete_event (UCHAR * packet, UCHAR status);
void smp_hndle_aes_cmac_complete (void);

#ifdef SMP_TBX_TEST_LESC_FUNCTIONS
void smp_lesc_test_aes_cmac_complete (void);
#endif /* SMP_TBX_TEST_LESC_FUNCTIONS */
#endif /* SMP_LESC */

#endif /* _H_SMP_ */
