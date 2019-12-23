/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @file   mainInclude.js
*
* @brief  Main include file containing all local JSripts files
*
******************************************************************************/

/******************************************************************************
* List of functions
******************************************************************************
* includeJSfiles() 
* build_includeFile_line(jsFileName)
*
*******************************************************************************/
 
/***************************************************************************//*!
*
* @brief   The function includes all required js files
* @param   
* @return  None
* @remarks 
******************************************************************************/
function includeJSfiles()
{
    build_includeFile_line('config.js');
    build_includeFile_line('calculations.js');
    build_includeFile_line('fileProcessing.js');
    build_includeFile_line('hFileConfig.js');
    build_includeFile_line('settings.js');
    build_includeFile_line('formCalculations.js');       
    build_includeFile_line('inner_Parameters.js');
    build_includeFile_line('inner_CLoop.js');
    build_includeFile_line('inner_SLoop.js');
    build_includeFile_line('inner_FLoop.js');
    build_includeFile_line('inner_PoSpeSensor.js');
    build_includeFile_line('inner_PoSpeRFO.js');
    build_includeFile_line('inner_CtrlStruc.js');
    build_includeFile_line('inner_MID.js');
}

/***************************************************************************//*!
*
* @brief   The function build script for including js file
* @param   
* @return  None
* @remarks Function required the same source folder
******************************************************************************/
function build_includeFile_line(jsFileName)
{
  document.write('<scr'+'ipt type="text/javascript" src="JS_functions/' + jsFileName + '" ></scr'+'ipt>');    
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
 