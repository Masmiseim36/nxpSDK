/*
 * Copyright 2020-2024 NXP.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "mpp_heap.h"
#include "hal_os.h"
#include "mpp_debug.h"

extern hal_sema_t stats_lock[];

_mpp_t *rc_prio_lst[MAX_MPP_HEAP_PRIO];         /* array of head elements to execute in RC task */
_mpp_t *preempt_prio_lst[MAX_MPP_HEAP_PRIO];    /* array of head elements to execute in PR task */

int mpp_memory_alloc(_mpp_t *mpp);
void mpp_memory_free(_mpp_t *mpp);
int mpp_memory_check(_mpp_t *mpp);

void mpp_heap_init(_mpp_t *prio_lst[])
{
    for(int i = 0; i < MAX_MPP_HEAP_PRIO ; i++)
    {
        prio_lst[i] = NULL;
    }
}

/* add new elements to the heap */
int mpp_heap_insert(_mpp_t *mpp, _mpp_t *prio_lst[])
{
    int prio = 0;
    while(prio_lst[prio] != NULL)
    {
        prio++;
        if (prio == MAX_MPP_HEAP_PRIO)
            return MPP_ERROR;
    }
    prio_lst[prio] = mpp;
    mpp->prio = prio;
    return MPP_SUCCESS;
}

/* move one element from a prio to another */
void mpp_heap_move(_mpp_t *mpp, _mpp_t *prio_lst[], unsigned int dst_prio)
{
    /* remove elem from past position */
    for(int i = 0; i < MAX_MPP_HEAP_PRIO; i++)
    {
        if(prio_lst[i] == mpp)
        {
            prio_lst[i] = NULL;
        }
    }
    prio_lst[dst_prio] = mpp;
    mpp->prio = dst_prio;
}

void HAL_DCACHE_CleanInvalidateByRange(uint32_t addr, uint32_t size);

/* ask the pipeline source for frame completion:
 * returns true when the source has finished capturing the full frame (all stripes)
 * else returns false.
 */
static int stripe_cnt = 0;

static bool mpp_is_done(_mpp_t *mpp)
{
    if (mpp == NULL) return true;
    _elem_t *elem = mpp->first_elem;

    if (elem->io.out_buf[0]->stripe_num == 0)
        return true;
    else
    {
        stripe_cnt++;
        if (stripe_cnt >= MPP_STRIPE_NUM)
        {
            stripe_cnt = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

/* MPP serial execution */
void mpp_execute(_mpp_t *mpp)
{
    int ret = MPP_SUCCESS;
    int i = 0;
    bool busy = false;
    bool update = false;
    /**/
    _elem_t *elem = mpp->first_elem;

    if (__builtin_expect((mpp->oper_status != MPP_RUNNING), 0))
        return;

    /* lock the pipeline status while processing */
    bool released = hal_sema_take(mpp->status_sema, 0);
    if (!released)
    {
        MPP_LOGI("Pipeline status previously locked\r\n");
    }

    /* source dequeue */
    /* dequeue HAL function */
    /* Dequeue executes more frequently, hence it should check
     * for its own rate limitation.
     */
    MPP_LOGD_IF(RLMT_CHECK(1), "Dequeue from src@%p\n", elem);
    if(elem->type == MPP_TYPE_SOURCE && elem->src_dequeue != NULL)
    {
        ret = elem->src_dequeue(mpp);
        /* no buffer has been dequeued */
        if (ret != MPP_SUCCESS)
        {
            MPP_LOGD("\nNo buffer dequeued from source\n");
            return;
        }
        elem = elem->next[0];
    }

    bool rlmt_log_on = RLMT_CHECK(1);
    uint32_t start_time, end_time;
    mpp_stats_t *stats;

    /* loop over processing elements in mpp */
    while ((elem != NULL) && (elem->mpp == mpp) && (elem->type == MPP_TYPE_PROC))
    {
        busy = false;
        update = false;
        MPP_LOGD_IF(rlmt_log_on, "\t\telem@%p\n", elem);

        /* check and update buffer status in atomic block */
        {
            hal_atomic_enter();
            for (i = 0; i < elem->io.nb_in_buf; i++)
            {
                if (elem->io.in_buf[i]->status == MPP_BUFFER_WRITTING) busy = true;
                /* check at least one input frame is new versus last id recorded */
                if (elem->io.last_frame_id[i] != elem->io.in_buf[i]->frame_id) update = true;
            }
            for (i = 0; i < elem->io.nb_out_buf; i++)
            {
                if (elem->io.out_buf[i]->status == MPP_BUFFER_READING) busy = true;
            }
            if (busy)
            {
                hal_atomic_exit();
                MPP_LOGD("element %s: input or output buffer busy! skip processing.\n", elem_name(elem->proc_typ));
                elem = elem->next[0];
                continue;
            }
            else if (!update)
            {
                hal_atomic_exit();
                MPP_LOGD("element %s: no input buffer update! skip processing.\n", elem_name(elem->proc_typ));
                elem = elem->next[0];
                continue;
            }
            else
            {
                for (i = 0; i < elem->io.nb_in_buf; i++)
                {
                    elem->io.in_buf[i]->status = MPP_BUFFER_READING;
                    MPP_LOGD("In mpp %d, Element %s starts processing input frame %d\n", mpp->prio, elem_name(elem->proc_typ), elem->io.in_buf[i]->frame_id);
                }
                for (i = 0; i < elem->io.nb_out_buf; i++)
                {
                    elem->io.out_buf[i]->status = MPP_BUFFER_WRITTING;
                }
                hal_atomic_exit();
            }
        }

        /* invalidate cache for input buffer */
        for (i = 0; i < elem->io.nb_in_buf; i++)
        {
            /* cacheable buffer, read by CPU */
            if (elem->io.in_buf[i]->hw->cacheable)
            {
                int bufsize = elem->io.in_buf[i]->hw->stride * elem->io.in_buf[i]->width;
                HAL_DCACHE_CleanInvalidateByRange((uint32_t) elem->io.in_buf[i]->hw->heap_p, bufsize);
            }
        }

        stats = elem->params.stats;
        if (stats) start_time = hal_get_exec_time();

        /* do the processing */
        elem->entry(elem);

        if (stats) end_time = hal_get_exec_time();
        if (stats && hal_sema_take(stats_lock[MPP_STATS_GRP_ELEMENT], 0)) {
            stats->elem.elem_exec_time = end_time - start_time;
            hal_sema_give(stats_lock[MPP_STATS_GRP_ELEMENT]);
        }

        /* flush cache for output buffer */
        for (i = 0; i < elem->io.nb_out_buf; i++)
        {
            /* cacheable buffer, written by CPU */
            if (elem->io.out_buf[i]->hw->cacheable)
            {
                int bufsize = elem->io.out_buf[i]->hw->stride * elem->io.out_buf[i]->width;
                HAL_DCACHE_CleanInvalidateByRange((uint32_t) elem->io.out_buf[i]->hw->heap_p, bufsize);
            }
        }

        /* update status again in atomic block */
        {
            hal_atomic_enter();
            unsigned short latest_id = 0; /* records highest id from different inputs */
            for (i = 0; i < elem->io.nb_in_buf; i++)
            {
                elem->io.in_buf[i]->status = MPP_BUFFER_EMPTY;
                /* record last input frame id processed */
                elem->io.last_frame_id[i] = elem->io.in_buf[i]->frame_id;
                /* output id will be most recent frame id */
                if (elem->io.in_buf[i]->frame_id > latest_id) latest_id = elem->io.in_buf[i]->frame_id;
            }
            for (i = 0; i < elem->io.nb_out_buf; i++)
            {
                elem->io.out_buf[i]->status = MPP_BUFFER_READY;
                /* update output frame id */
                elem->io.out_buf[i]->frame_id = latest_id;
            }
            hal_atomic_exit();
        }
        if (elem == mpp->last_elem)
            break;
        else
            elem = elem->next[0];
    }

    /* sink enqueue */
    MPP_LOGD_IF(rlmt_log_on, "Enqueue to sink @%p\n", elem);
    if (elem->type == MPP_TYPE_SINK && elem->sink_enqueue) elem->sink_enqueue(mpp);

    released = hal_sema_give(mpp->status_sema);
    if (!released)
    {
        MPP_LOGD("Pipeline unlocking issue\r\n");
    }
}

void mpp_execute_heap(_mpp_t *prio_lst[])
{
    int i;
    uint32_t start_time, end_time;
    mpp_stats_t *stats;
    bool done = true;

    do
    {
        for (i = 0; i < MAX_MPP_HEAP_PRIO; i++)
        {
            if (prio_lst[i] != NULL)
            {
                _mpp_t *mpp = prio_lst[i];
                /**/
                stats = mpp->params.stats;
                if (stats) start_time = hal_get_exec_time();
                mpp_execute(mpp);
                if (stats) end_time = hal_get_exec_time();
                if (stats && hal_sema_take(stats_lock[MPP_STATS_GRP_MPP], 0))
                {
                    stats->mpp.mpp = (mpp_t)mpp;
                    stats->mpp.mpp_exec_time = end_time - start_time;
                    hal_sema_give(stats_lock[MPP_STATS_GRP_MPP]);
                }
            }
            /**/
        }
        done = mpp_is_done(prio_lst[0]); /* check final stripe (first mpp has the source) */
    }
    while(!done);   /* continue on next stripe */

}

/* gives the number of elements in mpp */
static int mpp_get_nbelem(_mpp_t *mpp)
{
    _elem_t *cur = mpp->first_elem;
    int cnt = 0;
    while((cur != NULL) && (cur->mpp == mpp))
    {
        cnt++;
        cur = cur->next[0];
    }
    return cnt;
}

void mpp_dump_heap(_mpp_t *prio_lst[])
{
    int i;

    for (i = 0; i < MAX_MPP_HEAP_PRIO; i++)
    {
        if (prio_lst[i])
        {
            MPP_LOGI("prio %d : %lu items\r\n", i, mpp_get_nbelem(prio_lst[i]));
            _mpp_t *mpp = prio_lst[i];
            _elem_t *elem = mpp->first_elem;
            /* loop over elements in mpp */
            while ((elem != NULL) && (elem->mpp == mpp))
            {
                MPP_LOGI("\tmpp@%p\r\n", elem->mpp);
                switch (elem->type)
                {
                case MPP_TYPE_SOURCE:
                    MPP_LOGI("\t\tsrc_type %d src %p\r\n", elem->src_typ, elem);
                    break;
                case MPP_TYPE_SINK:
                    MPP_LOGI("\t\tsink_type %d sink %p\r\n", elem->sink_typ, elem);
                    break;
                case MPP_TYPE_PROC:
                    MPP_LOGI("\t\tproc_type %d proc %p\r\n", elem->proc_typ, elem);
                    break;
                default:
                    MPP_LOGE("\t\tERROR: element %p type unknown \r\n", elem);
                    break;
                }

                elem = elem->next[0];
            };
            /**/

        }
    }

}

/**
 * Assign or allocates buffers to the elements of each MPP object
 * returns 0 if succeeded, else returns the error code.
 **/
int mpp_memory_manage_heap(_mpp_t *prio_lst[])
{
    /* browse the mpp's and assign buffers for each element */
    int i;
    int ret = MPP_SUCCESS;

    for (i = 0; i < MAX_MPP_HEAP_PRIO; i++)
    {
        _mpp_t *mpp = prio_lst[i];
        if (mpp)
        {
            mpp = prio_lst[i];
            ret = mpp_memory_alloc(mpp);
            if (ret != MPP_SUCCESS)
            {
                mpp_memory_free(mpp);
                return ret;
            }
        }
    }

    return ret;
}

/**
 * Check buffer allocation of all branches from a list
 **/
int mpp_memory_check_list(_mpp_t *prio_lst[])
{
    /* browse the mpp's and check for buffer's allocation */
    int i;
    int ret = MPP_SUCCESS;

    for (i = 0; i < MAX_MPP_HEAP_PRIO; i++)
    {
        _mpp_t *mpp = prio_lst[i];
        if (mpp)
        {
            mpp_memory_check(mpp);
        }
    }

    return ret;
}
