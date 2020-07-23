/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FMSTR_DBG_MSG_SCRIPT_PRE   \
    "<html><head><style>.r{color: #E00;} .g{color: #080;} .b{color: #008;} "\
    ".y{color: #880;}</style><script type='text/javascript'>function"\
    " updateConsole(e){try{if(pcm.PipeReadString(vP,vR,vM)){window.clearInterval(vT);"\
    "var n=pcm.LastPipe_data.split('\\n');n=('00'+(vC+=1).toString()).slice(-3)+'"\
    ":&nbsp;&nbsp;&nbsp;'+vX[n[0].charCodeAt()-1-32];var t=document.getElementById"\
    "('IDC');(n=(t.innerHTML+'<BR>'+n).split('<BR>')).length>vN&&(n=n.splice"\
    "(n.length-vN,vN)),n=n.join('<BR>'),t.innerHTML=n,vT=window.setInterval"\
    "(updateConsole,vR)}}catch(err){location.reload();}}function initConsole()"\
    "{var e=document.getElementById"\
    "('IDC');0==pcm.PipeOpen(vP)&&(e.innerHTML='<BR>FreeMASTER pipe could not "\
    "be opened!')}function toggle(e){'OFF'==e.value?(e.value='ON',e.innerHTML="\
    "'ON',vT=window.setInterval(updateConsole,vR)):'ON'==e.value&&(e.value='OFF'"\
    ",e.innerHTML='OFF',window.clearInterval(vT))}var vX=["

#define FMSTR_DBG_MSG_SCRIPT_POST  \
    "],vN=20,vR=100,vM=1,vT,vC=0,vP=0;window.onbeforeun"\
    "load=function(e){pcm.PipeClose(vP),window.clearInterval(vT)};</script>"\
    "</head><body id='mainPageTab' onload='initConsole()' style='font-family:"\
    "Calibri,monospace;font-size:16px;font-style:normal;font-variant:normal;"\
    "font-weight:500;line-height:20px'><object id='MCB_PC_Master_Services1' "\
    "height='1' width='1' classid='clsid:48A185F1-FFDB-11D3-80E3-00C04F176153' "\
    "name='pcm'></object><table style='width:100%'><tr><td style='width:50%' "\
    "valign='top'>"\
    "<h2><span style='color:#ffcc00'>N</span><span style='color:#66ccff'>X</span>"\
    "<span style='color:#99cc00'>P</span><span> Three-phase PMSM Safety Reference "\
    "Design V0.2.0.</span></h2>is the safety motor control reference design"\
    " for Kinetis KV11 and KV31 MCUs. The reference design features a sensorless "\
    "closed-loop Field-Oriented Control (FOC) algorithm with the IEC60730 class "\
    "B safety. Application contains also motor parameter identification software "\
    "to identify parameters of user motor. The application is by default tuned for MIGE "\
    "AC servo motor 60CST-M01330.<h3>FreeMASTER Project Tree</h3>contains several "\
    "blocks divided according specific purpose. In variable watch are variables "\
    "marked by different colors.<ul><li><span style='color:red;font-weight:bold;'>The "\
    "red</span> - variables needed for control the motor as required speed, "\
      "required currents etc.<li><span style='background-color:#FFFFE0'>The yellow"\
    "</span> - variables intended for configuration of the control parameters "\
    "as fault tresholds, estimator parametrs etc.<li><span style='background-"\
    "color:#C0FFFF'>Blue</span> and black variables has read-only character.<li>"\
    "<span style='color:#990000;font-weight:bold;'>Dark red</span> represent pending "\
    "and captured faults.</ul><p>The following sections are available in "\
    "FreeMASTER Project Tree:</p><ul><li><b>MCU Safety Diagnostics</b> - contains "\
    "list of all safety related variables</li><li><b>Motor Control</b> - contains "\
    "all variables needed for control motor<ul><li>Measure (Meas) - contain "\
    "recorders and list of measured variables (voltages and currents).</li><li>"\
    "Estimate (EST) - contain recorders, scopes and variables needed for "\
    "tunning of the BEMF observer, tracking observer, speed filters and speed "\
    "ramps.</li><li>Diagnose Faults (DIAG) - contains variables needed for configuration "\
    "of fault tresholds (over-voltage, over-current,...). </li><li>Control (FOC) "\
    "- contains several scopes, recorders and all variables needed for control "\
    "of rotor speed, tuning of parameters of PI controllers "\
    "etc. </li><li>PWM Generation (ACT) - contain information about actual duty "\
    "cycles and SVM sectors</li></ul></li><li><b>Motor identification (MID)</b> "\
    "- project tree intended for identification motor parameters. Currently available only for KV31.</li></ul><h3>"\
    "Main control variables description</h3><ul><li><b><span style='color:red'>"\
    "APP Command</span></b> - Application command set by the user (RUN/READY/STOP)"\
    "</li><li><b><span style='color:red'>APP Control Mode</span></b> - Selected "\
    "control method (speed FOC, current FOC, scalar,...)</li><li><b>"\
    "<span style='color:red'>APP Cmd Speed FOC</span></b> - Required speed"\
    "</li><li><b>APP Qty Cmd Status</b> - Validation of user's input"\
    "</li><li><b><span style='color:#990000'>DIAG: Faults</span></b> - Captured "\
    "and pending faults, where captured faults can cleared</li></ul><h3>Running "\
    "demo in speed FOC</h3><ol><li>Check that all faults are <b>OFF</b>.</li><li>"\
    "Set <b>APP Control Mode</b> to <b>CL SPEED FOC</b></li><li>Set <b>RUN</b> "\
    "mode in <b>APP Command</b></li><li>Set the valid speed of the rotor in <b>APP "\
    "Cmd Speed FOC</b></li></ol><h3>Console</h3>To show the application states "\
    "during operation <b>switch on</b> the console button in the right top corner.<h3>More "\
    "information</h3>For more information and documentation visit the page: <a "\
    "href='http://www.nxp.com/motorcontrol_pmsm'>http://www.nxp.com/motorcontrol_pmsm"\
    "</a><br><a href='http://www.nxp.com/iec60730'>http://www.nxp.com/iec60730"\
    "</a><p><a href='https://forms.office.com/Pages/ResponsePage.aspx?id=06FuaCu8"\
    "b0ypLNmcXDAWNRtX7GIvW5NIhyLFq8A5Qw9UMUJNTVdPVFJVWVRZT1gzWVdLS0lUVTRVVSQlQCN0PWcu'"\
    ">Please, suggest improvements and give us feedback here!</a></p></td>"\
    "<td valign='top'><h3>CONSOLE&nbsp;<button onclick='toggle(this)' value="\
    "'OFF' style='background-color:#fff;border:1px solid #555;padding:0 2px;"\
    "text-align:center;display:inline-block;font-weight:bold;font-size:15px;"\
    "cursor:pointer'>OFF</button></h3><div id='IDC'></div></td></tr></table>"\
    "</body></html>"

/*! @struct Debug pipe console. */
typedef struct _dbg_console
{
    /* FreeMASTER pipe structure.  */
    FMSTR_HPIPE sFMSTRPipe;
    /* Transmission buffer. */
    char    ui8FMSTRPipeTxBuffer[64U];
    /* Receive buffer (receive not expected). */
    char    ui8FMSTRPipeRxBuffer;
} dbg_console_t;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* The readme page. */
static const char c_readme_html[] = FMSTR_DBG_MSG_SCRIPT_PRE\
	                                  FMSTR_DBG_MESSAGES\
                                    FMSTR_DBG_MSG_SCRIPT_POST;

/* FreeMASTER pipe console. */
static dbg_console_t  s_sDbgConsole;



/*!
 * @brief   The FreeMASTER pipe-based debug console initialization.
 *
 * @param   void
 *
 * @return  none
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
void FMSTR_PipeMessage_Init(void)
{
    /* Initialization of debug console via FreeMASTER. */
    s_sDbgConsole.sFMSTRPipe = FMSTR_PipeOpen(0U, NULL,
                                (FMSTR_ADDR) &s_sDbgConsole.ui8FMSTRPipeRxBuffer,
                                sizeof(s_sDbgConsole.ui8FMSTRPipeRxBuffer),
                                (FMSTR_ADDR) s_sDbgConsole.ui8FMSTRPipeTxBuffer,
                                sizeof(s_sDbgConsole.ui8FMSTRPipeTxBuffer),0U, "DBG");
}
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*!
 * @brief   The FreeMASTER pipe-based debug console.
 *
 * @param   ui8MsgIdx - Message index.
 *
 * @return  none
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
void FMSTR_PipeMessage(uint8_t ui8MsgIdx)
{
    char cMsg[2U]; /* Initialize the message buffer. */

    /* Pass the message index as null-terminated string. */
    cMsg[0U] = (char)ui8MsgIdx;
    cMsg[1U] = 0;

    /* Push message via FMSTR pipe. */
    FMSTR_PipePuts(s_sDbgConsole.sFMSTRPipe, (char const*)(&cMsg));
}
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*!
 * @brief   The debug FreeMASTER TSA table static function.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   TSA table identificator.
 *
 * @return  FMSTR_TSA_ENTRY - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN(TSA_DEBUG)

    /* The readme control page. */
    FMSTR_TSA_MEMFILE("readme.html", c_readme_html, sizeof(c_readme_html))

FMSTR_TSA_TABLE_END()
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */


/*!
 * @brief   The list of FreeMASTER TSA tables static function.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   void
 *
 * @return  FMSTR_TSA_ENTRY - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_LIST_BEGIN()
    FMSTR_TSA_TABLE(TSA_MCSM)
    FMSTR_TSA_TABLE(TSA_FOC)
    FMSTR_TSA_TABLE(TSA_MAIN)
    FMSTR_TSA_TABLE(TSA_DEBUG)
    FMSTR_TSA_TABLE(TSA_SAFETY)
    FMSTR_TSA_TABLE(TSA_ADC)
    FMSTR_TSA_TABLE(TSA_APPINIT)
#if defined(MID)
    FMSTR_TSA_TABLE(TSA_MID)
    FMSTR_TSA_TABLE(TSA_MID_API_CONN)
#endif
FMSTR_TSA_TABLE_LIST_END()
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */
