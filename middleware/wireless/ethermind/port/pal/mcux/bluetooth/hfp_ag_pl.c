
/**
 *  \file hfp_ag_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "hfp_ag_pl.h"
#include "nwsim.h"

#ifdef HFP_AG
/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */


/* --------------------------------------------- Functions */
void hfp_ag_init_pl
     (
         void (* read_cb)(UCHAR *rsp, UINT16 rsplen),
         void (* call_cb)(UCHAR status)
     )
{
    /* Initialize the modem/simulator */
    nwsim_init(read_cb, call_cb);

    return;
}

void hfp_ag_start_pl (UINT16 sf)
{
    /* Start the modem/simulator */
    nwsim_start(sf);
}

void hfp_ag_stop_pl(void)
{
    /* Stop the modem/simulator */
    nwsim_stop();
}

void hfp_ag_write_pl (UCHAR * cmd, UINT16 cmdlen)
{
    /* Write to the modem/simulator */
    nwsim_command_send(cmd, cmdlen);
}

void hfp_ag_access_pl(void)
{
    nwsim_operations();
}
#endif /* HFP_AG */
