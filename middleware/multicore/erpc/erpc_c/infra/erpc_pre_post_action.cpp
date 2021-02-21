/*
 * Copyright 2020 NXP
 * Copyright 2020 ACRIOS Systems s.r.o.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_pre_post_action.h"

#include "erpc_config_internal.h"
#if ERPC_PRE_POST_ACTION_DEFAULT
#include "erpc_setup_extensions.h"
#endif

using namespace erpc;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void PrePostAction::addPreCB(pre_post_action_cb preCB)
{
    if (preCB)
    {
        m_preCB = preCB;
    }
#if ERPC_PRE_POST_ACTION_DEFAULT
    else
    {
        m_preCB = erpc_pre_cb_default;
    }
#endif
}

void PrePostAction::addPostCB(pre_post_action_cb postCB)
{
    if (postCB)
    {
        m_postCB = postCB;
    }
#if ERPC_PRE_POST_ACTION_DEFAULT
    else
    {
        m_postCB = erpc_post_cb_default;
    }
#endif
}