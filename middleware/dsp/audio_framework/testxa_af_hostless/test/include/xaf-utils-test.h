/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __XAF_UTILS_TEST_H__
#define __XAF_UTILS_TEST_H__

#define __XF_H
#include "osal-thread.h"
#undef __XF_H

#include "xa_type_def.h"

/* ...debugging facility */
#include "xf-debug.h"

#include "xaf-test.h"
#include "xaf-api.h"
#include "xaf-mem.h"
#include "xaf-app-threads-priority.h"

#ifdef __XCC__
#include <xtensa/hal.h>
#include <xtensa/config/core-isa.h>

#ifdef __TOOLS_RF2__
#define TOOLS_SUFFIX    "_RF2"
#else
#define TOOLS_SUFFIX "_"XTENSA_RELEASE_NAME
#endif

#if XCHAL_HAVE_FUSION
#define BUILD_STRING "XTENSA_FUSION" TOOLS_SUFFIX
#elif XCHAL_HAVE_HIFI5
#define BUILD_STRING "XTENSA_HIFI5" TOOLS_SUFFIX
#elif XCHAL_HAVE_HIFI4
#define BUILD_STRING "XTENSA_HIFI4" TOOLS_SUFFIX
#elif XCHAL_HAVE_HIFI3
#define BUILD_STRING "XTENSA_HIFI3" TOOLS_SUFFIX
#elif XCHAL_HAVE_HIFI_MINI
#define BUILD_STRING "XTENSA_HIFI_MINI" TOOLS_SUFFIX
#elif XCHAL_HAVE_HIFI2EP
#define BUILD_STRING "XTENSA_HIFI2EP" TOOLS_SUFFIX
#elif XCHAL_HAVE_HIFI2
#define BUILD_STRING "XTENSA_HIFI2" TOOLS_SUFFIX
#else
#define BUILD_STRING "XTENSA_NON_HIFI" TOOLS_SUFFIX
#endif

#else
#define BUILD_STRING "NON_XTENSA"
#endif

#if defined(HAVE_XOS)
#define BUILD_RTOS  "XOS"
#elif defined(HAVE_FREERTOS)
#define BUILD_RTOS  "FreeRTOS"
#else
#error "error: RTOS is neither XOS nor FreeRTOS"
#endif

#ifdef XAF_PROFILE
#include <xtensa/sim.h>
#include "xaf-clk-test.h"
#endif

#if (XF_CFG_CORES_NUM > 1)
#include "xf-shared.h"
#else
#ifndef XF_SHMEM_SIZE
#define XF_SHMEM_SIZE  0
#endif
extern void *shared_mem;
extern char perf_stats[];
#endif

#ifndef STACK_SIZE 
#define STACK_SIZE          (10 * 1024)
#endif

#define _MIN(a, b)	(((a) < (b))?(a):(b))

#define RUNTIME_MAX_COMMANDS 20 
#define MAX_NUM_COMP_IN_GRAPH 20
#define MAX_EVENTS          256

#define ADEV_CLOSE_SIGNAL  -1

#define NUM_THREAD_ARGS     7

#ifndef XF_CORE_ID
#define XF_CORE_ID          0
#endif

static inline void runtime_param_usage(void)
{
    fprintf(stdout,"Optional Parameters:\n");
    fprintf(stdout,"--------------------\n");
    fprintf(stdout, "-pr:<Component_ID>,<Port_No>,<Pause_Time>,<Resume_Time> : Enable Pause and Resume on the specified port of component \n");
    fprintf(stdout," \t Component_ID : Component Identifier as displayed above \n");
    fprintf(stdout," \t Port_No      : Port number to be paused and resumed \n");
    fprintf(stdout," \t Pause_Time   : Pause time in milliseconds(absolute time) after pipeline creation \n");
    fprintf(stdout," \t Resume_Time  : Resume time in milliseconds(absolute time), must be greater than Pause_time \n");
    fprintf(stdout," \t Example      : -pr:0,0,100,200 -pr:2,1,250,300  \n");
    fprintf(stdout,"\n");    
    fprintf(stdout, "-probe-cfg:<Component_ID>,<Port_No,Port_No,...> : Enable probe feature on the specified component \n");
    fprintf(stdout," \t Component_ID : Component Identifier as displayed above \n");
    fprintf(stdout," \t Port_No      : Comma separated list of port numbers to be probed \n");   
    fprintf(stdout," \t Example      : -probe-cfg:2,0,2,5 -probe-cfg:3,2,4  \n");
    fprintf(stdout,"\n");    
    fprintf(stdout, "-probe:<Component_ID>,<Start_Time>,<Stop_Time> : Probe start and stop time for the specified component \n");
    fprintf(stdout," \t Component_ID : Component Identifier as displayed above (Probe feature must be enabled through -probe-cfg option) \n");
    fprintf(stdout," \t Start_Time   : Probe start time in milliseconds(absolute time) after pipeline creation \n");
    fprintf(stdout," \t Stop_Time    : Probe stop time in milliseconds(absolute time). Negative value for Stop_Time indicate probe till end \n");
    fprintf(stdout," \t Example      : -probe:2,150,250 -probe:3,200,-1  \n");
    fprintf(stdout,"\n");    
    fprintf(stdout, "-core-cfg:<Core_ID>,<Component_ID,Component_ID,..> \n");
    fprintf(stdout," \t Core_ID      : Core Identifier. Allowed Core_ID's are from 0, upto Number of Cores-1 \n");
    fprintf(stdout," \t Component_ID : Component Identifier as displayed above. By default components are created on Master Core(default 0) \n");
    fprintf(stdout," \t Example      : -core-cfg:1,2,3 -core-cfg:2,0,4 \n");


    fprintf(stdout,"\n");
}

static inline void runtime_param_reconnect_usage(void)
{
    fprintf(stdout, "-D:<Src_comp_ID>,<Src_out_port>,<Dst_comp_ID>,<Dst_in_port>,<Disconnect_Time>,<DELETE_INFO>: Disconnect the components with specified ports \n");
    fprintf(stdout," \t Src_comp_ID     : Component Identifier for source component as displayed above \n");
    fprintf(stdout," \t Src_out_port    : Output port number of source component (to be disconnected) \n");
    fprintf(stdout," \t Dst_comp_ID     : Component Identifier for destination component as displayed above \n");
    fprintf(stdout," \t Dst_in_port     : Input port number of destination component (connected to output port of source component above) \n");
    fprintf(stdout," \t Disconnect_Time : Disconnect time in milliseconds(absolute time) after pipeline creation \n");
    fprintf(stdout," \t DELETE_INFO     : Supported values are 0 or 1 . \
                  \n \t                 : Value '0' indicates disconnect but do not delete the disconnected component (so it can be reconnected at later time)  \
                  \n \t                 : Value '1' indicates disconnect and delete the disconnected component. \
                  \n \t                   For value '1' option above: \
                  \n \t                   NOTE1: For components receiving input data from testbench, input_over should be forced so that the associated thread in \
                  \n \t                   application exits gracefully before disconnect and deletion of component. \
                  \n \t                   NOTE2: Probe, if started should be stopped so that the associated thread in application (if any) exits gracefully \
                  \n \t                   before disconnect and deletion of component. \n");
    fprintf(stdout," \t Example         : -D:0,1,4,0,100,1 \n");
    fprintf(stdout,"\n");
    fprintf(stdout, "-C:<Src_comp_ID>,<Src_out_port>,<Dst_comp_ID>,<Dst_in_port>,<Connect_Time>,<DELETE_INFO>: Connect the components with specified ports \n");
    fprintf(stdout," \t Src_comp_ID     : Component Identifier for source component as displayed above \n");
    fprintf(stdout," \t Src_out_port    : Output port number of source component (to be connected) \n");
    fprintf(stdout," \t Dst_comp_ID     : Component Identifier for destination component as displayed above \n");
    fprintf(stdout," \t Dst_in_port     : Input port number of destination component (to be connected to output port of source component above)  \n");
    fprintf(stdout," \t Connect_Time    : Connect time in milliseconds(absolute time) after pipeline creation \n");
    fprintf(stdout," \t DELETE_INFO     : Supported values are 0 or 1 . \
                  \n \t                 : Value '0' indicates connect is being issued on existing component (disconnected earlier). This indicates that there is \
                  \n \t                   no need to create and initialize the component or create associated thread in application (if required). \
                  \n \t                 : Value '1' indicates connect is being issued on new component (disconnected with deletion earlier or first time connect). \
                  \n \t                   This indicates that the component should be created and initialized before connect and associated thread should be \
                  \n \t                   created in application (if required). \n");
    fprintf(stdout," \t Example         : -C:0,1,4,0,200,1 \n");
    fprintf(stdout,"\n");
    fprintf(stdout,"Note 1: Runtime disconnect and connect commands must not be issued when data processing pipeline is in flushing or closure state naturally. \
                  \n      (For e.g. if input over is received on all input ports of components in recorder-usecase or if last active stream connected to mixer is \
                  \n      disconnected in playback-usecase) as these command operations may conflict with pipeline state. \n");
    fprintf(stdout,"Note 2: If a -D: is issued without a corresponding -C: command involving the same component, DELETE_INFO is changed to 1 internally. \
                  \n      This is an optimization at application level to teardown the part of pipeline that are not going to be active again. \
                  \n      Similarly if a -D: command has a corresponding -C: command involving the same component, then the DELETE_INFO of the -C: command is \
                  \n      assumed for both the commands, which means if a component needs to be re-created, then first it must be deleted and if component \
                  \n      re-creation is not required, then deletion is not required.\n");
    fprintf(stdout,"\n");
}
    
static inline void runtime_param_footnote(void)
{
    fprintf(stdout,"Note: Ports are numbered starting with zero from first input port to last output port.\n");
    fprintf(stdout,"\n");    
}

static inline void mixer_component_footnote(void)
{
    fprintf(stdout,"Note: Mixer component allows start of processing once at least one of the input ports is connected and valid input is available (among the 4 input ports).\
                  \nThe connections and data arrival instances on input ports can vary between single-core and multi-core execution, due to which the output of the mixer can differ.\n");
    fprintf(stdout,"\n");
}

static inline void mimo_mix_component_footnote(void)
{
    fprintf(stdout,"Note: MIMO-Mixer component has 2 input-ports. The component waits for inputs to be available on both the ports before consuming.\n");
    fprintf(stdout,"\n");
}

enum
{
    IGNORE = 0,
    PAUSE,
    RESUME,
    PROBE_START,
    PROBE_STOP,
    DISCONNECT,
    CONNECT,
};

typedef enum {               
    COMP_INVALID    =0,      
    COMP_DELETED    =1,      
    COMP_CREATED    =2,      
}COMP_STATE;

#define XAF_CFG_CODEC_SCRATCHMEM_SIZE ((UWORD32)56<<10)

typedef struct xaf_format_s {
    UWORD32             sample_rate;
    UWORD32             channels;
    UWORD32             pcm_width;
    UWORD32             input_length;
    UWORD32             output_length;
    UWORD32             output_produced;
} xaf_format_t;

typedef struct
{
    int action;
    int component_id;
    int port;
    int component_dest_id;
    int port_dest;  
    int time;
    int comp_create_delete_flag;
    
}cmd_info_t;

typedef struct
{
    cmd_info_t **commands;
    int size;
}cmd_array_t;

typedef struct event_info event_info_t;

struct event_info
{
    event_info_t *next;
    UWORD32 comp_addr;
    UWORD32 comp_error_flag;
    UWORD32 event_id;
    UWORD32 buf_size;
    void *event_buf;
};

typedef struct
{
    event_info_t **events;
    int num_events;
    int curr_idx;
}event_list_t;

/* Types */
typedef int xa_app_event_handler_fxn_t(event_info_t *event, void *arg);

/* global variable proto */
extern int g_force_input_over[MAX_NUM_COMP_IN_GRAPH];
extern int (*gpcomp_connect)(int comp_id_src, int port_src, int comp_id_dest, int port_dest, int create_delete_flag);
extern int (*gpcomp_disconnect)(int comp_id_src, int port_src, int comp_id_dest, int port_dest, int create_delete_flag);
extern UWORD32 g_active_disconnect_comp[MAX_NUM_COMP_IN_GRAPH];
extern int g_num_comps_in_graph;
extern xf_thread_t *g_comp_thread;
extern event_list_t *g_event_list;
extern xa_app_event_handler_fxn_t *g_app_handler_fn;
extern UWORD32 worker_thread_scratch_size[XAF_MAX_WORKER_THREADS];

#ifndef XA_DISABLE_EVENT
extern UWORD32 g_enable_error_channel_flag;
extern UWORD32  g_event_handler_exit;
#endif
 /* ...format structure used by the common MCPS calculator function, 
 * to be updated by the main_task of test-application */
extern xaf_format_t comp_format_mcps;

/* function proto */
int parse_runtime_params(void **runtime_params, int argc, char **argv, int num_comp );
void sort_runtime_action_commands(cmd_array_t *command_array);
int execute_runtime_actions(void *command_array, void *p_adev, void **comp_ptr, int *comp_nbufs, void **comp_threads, int num_threads, void *comp_thread_args[], int num_thread_args, unsigned char *comp_stack);
int abort_blocked_threads();
int all_threads_exited(void **comp_threads, int num_threads);
void set_wbna(int *argc, char **argv);
int print_verinfo(pUWORD8 ver_info[],pUWORD8 app_name);
int read_input(void *p_buf, int buf_length, int *read_length, void *p_input, xaf_comp_type comp_type);
double compute_comp_mcps(unsigned int num_bytes, long long comp_cycles, xaf_format_t comp_format, double *strm_duration);
int print_mem_mcps_info(mem_obj_t* mem_handle, int num_comp);
void *comp_process_entry(void *arg);
void *comp_process_entry_recorder(void *arg);
void *comp_disconnect_entry(void *arg);
void *event_handler_entry(void *arg);
int init_rtos(int argc, char **argv, int (*main_task)(int argc, char **argv));
unsigned short start_rtos(void);
int main_task(int argc, char **argv);
int submit_action_cmd(int src_cid, int src_port, int dest_cid, int dest_port, int action, int time);
void xa_app_initialize_event_list(int num_comp);
int xa_app_receive_events_cb(void *comp, UWORD32 event_id, void *buf, UWORD32 buf_size, UWORD32 comp_error_flag);
void xa_app_process_events(void *arg);
void xa_app_free_event_list(void);

#ifndef XA_DISABLE_EVENT
#define TST_CHK_API_COMP_CREATE(p_adev, _core, pp_comp, _comp_id, _num_input_buf, _num_output_buf, _pp_inbuf, _comp_type, error_string) {\
        xaf_comp_config_t comp_config;\
        TST_CHK_API(xaf_comp_config_default_init(&comp_config), "xaf_comp_config_default_init");\
		comp_config.error_channel_ctl = g_enable_error_channel_flag;\
		comp_config.comp_id = _comp_id;\
		comp_config.core = _core;\
		comp_config.comp_type = _comp_type;\
		comp_config.num_input_buffers = _num_input_buf;\
		comp_config.num_output_buffers = _num_output_buf;\
		comp_config.pp_inbuf = (pVOID (*)[XAF_MAX_INBUFS])_pp_inbuf;\
        TST_CHK_API(xaf_comp_create(p_adev, pp_comp, &comp_config), error_string);\
    }

#else 

#define TST_CHK_API_COMP_CREATE(p_adev, _core, pp_comp, _comp_id, _num_input_buf, _num_output_buf, _pp_inbuf, _comp_type, error_string) {\
        xaf_comp_config_t comp_config;\
        TST_CHK_API(xaf_comp_config_default_init(&comp_config), "xaf_comp_config_default_init");\
		comp_config.comp_id = _comp_id;\
		comp_config.core = _core;\
		comp_config.comp_type = _comp_type;\
		comp_config.num_input_buffers = _num_input_buf;\
		comp_config.num_output_buffers = _num_output_buf;\
		comp_config.pp_inbuf = (pVOID (*)[XAF_MAX_INBUFS])_pp_inbuf;\
        TST_CHK_API(xaf_comp_create(p_adev, pp_comp, &comp_config), error_string);\
    }
#endif

/* ...prevent instructions reordering */
#define barrier()                           \
    __asm__ __volatile__("": : : "memory")

/* ...memory barrier */
#define XF_IPC_BARRIER()                  \
    __asm__ __volatile__("memw": : : "memory")

#define XF_IPC_FLUSH(buf, length) \
        ({ if ((length)) { barrier(); xthal_dcache_region_writeback((buf), (length)); XF_IPC_BARRIER(); } buf; })

#define XF_IPC_INVALIDATE(buf, length) \
        ({ if ((length)) { xthal_dcache_region_invalidate((buf), (length)); barrier(); } buf; })

#endif /* __XAF_UTILS_TEST_H__ */
