/*******************************************************************************
* Copyright (c) 2015-2020 Cadence Design Systems, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to use this Software with Cadence processor cores only and 
* not with any other processors and platforms, subject to
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

******************************************************************************/
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

#ifdef __XCC__
#include <xtensa/hal.h>

#ifdef __TOOLS_RF2__
#define TOOLS_SUFFIX    "_RF2"
#else
#define TOOLS_SUFFIX    "_RI2"
#endif

#if XCHAL_HAVE_HIFI5
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

#ifndef STACK_SIZE 
#define STACK_SIZE          8192
#endif

#define RUNTIME_MAX_COMMANDS 20 
#define MAX_NUM_COMP_IN_GRAPH 20

#define NUM_THREAD_ARGS     7

#define DEVICE_ID    (0x180201FC) //Vender ID register for realtek board
#define VENDER_ID    (0x10EC)     //Vender ID value indicating realtek board


static inline void runtime_param_usage(void)
{
    fprintf(stdout,"Optional Parameters:\n");
    fprintf(stdout,"--------------------\n");
    fprintf(stdout, "-pr:<Component_ID>,<Port_No>,<Pause_Time>,<Resume_Time> : Enable Pause and Resume on the specified port of component \n");
    fprintf(stdout," \t Component_ID : Component Identifier as displayed above \n");
    fprintf(stdout," \t Port_No      : Port number to be paused and resumed \n");
    fprintf(stdout," \t Pause_Time   : Pause time in milliseconds(absolute time) after pipeline creation \n");
    fprintf(stdout," \t Resume_Time  : Resume time in milliseconds(absolute time), must be greater than Pause_time \n");
    fprintf(stdout," \t Example      : -pr:0,0,100,200 -pr:1,1,250,300  \n");
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
    fprintf(stdout,"Note: Runtime disconnect and connect commands must not be issued when data processing pipeline is in flushing or closure state naturally. \
                  \n      (For e.g. if input over is received on all input ports of components in recorder-usecase or if last active stream connected to mixer is \
                  \n      disconnected in playback-usecase) as these command operations may conflict with pipeline state. \n");
    fprintf(stdout,"\n");
}
    
static inline void runtime_param_footnote(void)
{
    fprintf(stdout,"Note: Ports are numbered starting with zero from first input port to last output port.\n");
    fprintf(stdout,"\n");    
}

enum
{
    PAUSE = 1,
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

/* global variable proto */
extern int g_force_input_over[MAX_NUM_COMP_IN_GRAPH];
extern int (*gpcomp_connect)(int comp_id_src, int port_src, int comp_id_dest, int port_dest, int create_delete_flag);
extern int (*gpcomp_disconnect)(int comp_id_src, int port_src, int comp_id_dest, int port_dest, int create_delete_flag);
extern int g_num_comps_in_graph;
extern xf_thread_t *g_comp_thread;

/* function proto */
int parse_runtime_params(void **runtime_params, int argc, char **argv, int num_comp );
void sort_runtime_action_commands(cmd_array_t *command_array);
int execute_runtime_actions(void *command_array, void *p_adev, void **comp_ptr, int *comp_nbufs, void **comp_threads, int num_threads, void *comp_thread_args[], int num_thread_args, unsigned char *comp_stack);
int abort_blocked_threads();
void set_wbna(int *argc, char **argv);
int print_verinfo(pUWORD8 ver_info[],pUWORD8 app_name);
int read_input(void *p_buf, int buf_length, int *read_length, void *p_input, xaf_comp_type comp_type);
double compute_comp_mcps(unsigned int num_bytes, int comp_cycles, xaf_format_t comp_format, double *strm_duration);
int print_mem_mcps_info(mem_obj_t* mem_handle, int num_comp);
void *comp_process_entry(void *arg);
void *comp_process_entry_recorder(void *arg);
int init_rtos(int argc, char **argv, int (*main_task)(int argc, char **argv));
unsigned short start_rtos(void);
int main_task(int argc, char **argv);


#endif /* __XAF_UTILS_TEST_H__ */
