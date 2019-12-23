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
* @file   inner_PoSpeRFO.js
*
* @brief  Closed Lopp Flux Observer engine
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormRFO() - init Sensorless page paramters and constants  
* clickCalculateRFO() - calculates control constants based on input parameters
* clickUpdateRFOFM() - update selected variables in FreeMASTER application
* writeRFOHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeRFOHeaderOutput(str) - write selected constants to output header file
*******************************************************************************/

/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID   
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormRFO()
{
   // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcPoSpeRFO();
   
   copyParent2InnerValById("RFO_KpGain");
   copyParent2InnerValById("RFO_KiGain");
   copyParent2InnerValById("CoeffRFO");
   
   copyParent2InnerValById("MRAS_KpGain");  
   copyParent2InnerValById("MRAS_KiGain");  
   
//   copyParent2InnerValById("RSESTIM_KpGain");  
//   copyParent2InnerValById("RSESTIM_KiGain"); 
//   copyParent2InnerValById("RSESTIM_max"); 
//   copyParent2InnerValById("RSESTIM_min"); 
  
   var typeArithmetic =  parent.document.getElementById("Arithmetic").innerText;
      
   // enable button enabling
   ReloadStoreButtonsOnOff(1);
   
   //calculate constants
   clickCalculateRFO();
}

/***************************************************************************//*!
*
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function markUpdateSLSField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("RFO_params").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("MRAS_params").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    
    document.getElementById("BemfObsrvUpdateFrm").title = "Red-legend constants will be updated on click"
    */
}

/***************************************************************************//*!
* @brief   UnMark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function unmarkUpdateSLSField()
{
    var obj = document.getElementById("RFO_params").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    var obj = document.getElementById("MRAS_params").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    
}
/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcPoSpeRFO()
{
    // replace and disable params
    switchParam2BasicMode("RFO_KpGain", 25);
    switchParam2BasicMode("RFO_KiGain", 0.01);
    switchParam2BasicMode("CoeffRFO", 3);
   
    switchParam2BasicMode("MRAS_KpGain",32750);  
    switchParam2BasicMode("MRAS_KiGain",12500);  

    
    
//    switchParam2BasicMode("RSESTIM_KpGain",1);  
//    switchParam2BasicMode("RSESTIM_KiGain",0.01); 
//    switchParam2BasicMode("RSESTIM_max",200); 
//    switchParam2BasicMode("RSESTIM_min",-25); 

}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/

/* --------------- Spee parameters routine ---------------- */
function clickCalculateRFO()
{
    var retMsg;
    var Imax_sc;
    var prefixM   = getActiveMotor();
    RFOKp     = getParentHtmlValue("RFO_KpGain");
    RFOKi     = getParentHtmlValue("RFO_KiGain");
     
//    RSEstKp   = getParentHtmlValue("RSESTIM_KpGain");  
//    RSEstKi   = getParentHtmlValue("RSESTIM_KiGain"); 
//    RSEstMax   = getParentHtmlValue("RSESTIM_max"); 
//    RSEstMin   = getParentHtmlValue("RSESTIM_min"); 
    
    MRASKp    = getParentHtmlValue("MRAS_KpGain");
    MRASKi    = getParentHtmlValue("MRAS_KiGain");
    
    var Rs        = getParentHtmlValue("RsIM");
    var Rr        = getParentHtmlValue("RrIM");
    var Ls        = getParentHtmlValue("LsIM");
    var Lr        = getParentHtmlValue("LrIM");
    var Lm        = getParentHtmlValue("LmIM");
    var CoeffRFO  = getParentHtmlValue("CoeffRFO");
    Ts            = getParentHtmlValue("CLOOP_Ts"); 
    var Pp        = getParentHtmlValue("pp"); 

    var Ls_leak = Ls-Lm;
    var Lr_leak = Lr-Lm;
    var sigma = (Ls*Lr - Lm*Lm)/(Ls*Lr);
    var Ls_leak_total = sigma*Ls;
    var Tr = Lr/Rr;
    var Kr = Lm/Lr;
    var T1 = 1/Math.PI;
    var Pi = Math.PI;
    
    /*RFO - begin*/
    Tr              = Lr/Rr;
    KrInv           = Lr/Lm;
    LsTotLeakGain   = KrInv*(Ls*(1.0-Lm*Lm/(Ls*Lr)));
    PsiRA1Gain      = Tr/(Tr + Ts);
    PsiRB1Gain      = (Lm*Ts)/(Tr + Ts);
    PsiSA1Gain      = (1.0/(2.0*Math.PI*CoeffRFO))/((1.0/(2.0*Math.PI*CoeffRFO))+Ts);
    PsiSA2Gain      = Ts*PsiSA1Gain;
    RsEst           = Rs;
    /*RFO - end*/
    
    SpdCnst = 60/(2*Math.PI*Pp);
    TrqCnst = 1.5*Pp*Lm/Lr;
    
    /* RS compensation */
//    if(RSEstMax<0){RSEstMax = 0; window.alert("Rsmax must be positive number!");}
//    RSEstMax = Math.round(((1+(RSEstMax/100))*Rs)*1000000000000)/1000000000000;
//    if(RSEstMin>0){RSEstMin = 0; window.alert("Rsmin must be negative number!");}
//    RSEstMin = Math.round(((1+(RSEstMin/100))*Rs)*1000000000000)/1000000000000;
    
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {  
      // write values to forms in current Html page
      setInnerHtmlValueAsText("RFO_COMP_KP_GAIN",0,RFOKp,Math.round(RFOKp*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_COMP_KI_GAIN",0,RFOKi,Math.round(RFOKi*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_PSI_RA1_GAIN",0,PsiRA1Gain,Math.round(PsiRA1Gain*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_PSI_RB1_GAIN",0,PsiRB1Gain,Math.round(PsiRB1Gain*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_PSI_SA1_GAIN",0,PsiSA1Gain,Math.round(PsiSA1Gain*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_PSI_SA2_GAIN",0,PsiSA2Gain,Math.round(PsiSA2Gain*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_KR_INV_GAIN",0,KrInv,Math.round(KrInv*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_LS_TOT_LEAK_GAIN",0,LsTotLeakGain,Math.round(LsTotLeakGain*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_RS_EST",0,RsEst,Math.round(RsEst*1e12)/1e12);
      setInnerHtmlValueAsText("RFO_TRQCNST",0,TrqCnst,Math.round(TrqCnst*1e12)/1e12);
      
      setInnerHtmlValueAsText("MRAS_KP_GAIN",0,MRASKp,Math.round(MRASKp*1e12)/1e12);
      setInnerHtmlValueAsText("MRAS_KI_GAIN",0,MRASKi,Math.round(MRASKi*1e12)/1e12);
      setInnerHtmlValueAsText("MRAS_PSI_RA1",0,PsiRA1Gain,Math.round(PsiRA1Gain*1e12)/1e12);
      setInnerHtmlValueAsText("MRAS_PSI_RB1",0,PsiRB1Gain,Math.round(PsiRB1Gain*1e12)/1e12);
      setInnerHtmlValueAsText("MRAS_TS",0,Ts,Math.round(Ts*1e12)/1e12);
      setInnerHtmlValueAsText("MRAS_SPDCNST",0,SpdCnst,Math.round(SpdCnst*1e12)/1e12);
      
//      setInnerHtmlValueAsText("RSESTIM_KP_GAIN",0,RSEstKp,RSEstKp);
//      setInnerHtmlValueAsText("RSESTIM_KI_GAIN",0,RSEstKi,RSEstKi);
//      setInnerHtmlValueAsText("RSESTIM_MAX",0,RSEstMax,RSEstMax);
//      setInnerHtmlValueAsText("RSESTIM_MIN",0,RSEstMin,RSEstMin);
    }
 }
 
/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdatePoSpeRFOFM()
{
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
    var errorArray = [];
     
    if(!pcm.IsCommPortOpen())
    {
        alert("Communication is stopped.\nPress Ctrl+K to start the communication");
    }
    else
    {
     
     
        // // calculate actual constant values
        clickCalculateRFO();    
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_KP_GAIN',RFOKp,RFOKp));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_KI_GAIN',RFOKi,RFOKi));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_PSI_RA1_GAIN',PsiRA1Gain,PsiRA1Gain));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_PSI_RB1_GAIN',PsiRB1Gain,PsiRB1Gain));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_PSI_SA1_GAIN',PsiSA1Gain,PsiSA1Gain));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_PSI_SA2_GAIN',PsiSA2Gain,PsiSA2Gain));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_KR_INV_GAIN',KrInv,KrInv));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_LS_TOT_LEAK_GAIN',LsTotLeakGain,LsTotLeakGain));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_RS_EST',RsEst,RsEst));
        errorArray.push(UpdateFMVariable(xmlDoc,'RFO_TRQCNST',TrqCnst,TrqCnst));
     
        
        errorArray.push(UpdateFMVariable(xmlDoc,'MRAS_KP_GAIN',MRASKp,MRASKp));
        errorArray.push(UpdateFMVariable(xmlDoc,'MRAS_KI_GAIN',MRASKi,MRASKi));
        errorArray.push(UpdateFMVariable(xmlDoc,'MRAS_PSI_RA1',PsiRA1Gain,PsiRA1Gain));
        errorArray.push(UpdateFMVariable(xmlDoc,'MRAS_PSI_RB1',PsiRB1Gain,PsiRB1Gain));
        errorArray.push(UpdateFMVariable(xmlDoc,'MRAS_TS',Ts,Ts));
        errorArray.push(UpdateFMVariable(xmlDoc,'MRAS_SPDCNST',SpdCnst,SpdCnst));
         
    
        /*errorArray.push(UpdateFMVariable(xmlDoc,'RSESTIM_KP_GAIN',RSEstKp,RSEstKp));
        errorArray.push(UpdateFMVariable(xmlDoc,'RSESTIM_KI_GAIN',RSEstKi,RSEstKi));
        errorArray.push(UpdateFMVariable(xmlDoc,'RSESTIM_MAX',RSEstMax,RSEstMax));
        errorArray.push(UpdateFMVariable(xmlDoc,'RSESTIM_MIN',RSEstMin,RSEstMin));*/
               
    
        // display error message                           
        UpdateError(errorArray);
    }
 }            
 
/***************************************************************************//*!
*
* @brief  The function reads values from input forms, scales them and write 
*         to output HTML form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writePoSpeRFOHTMLOutput(prefix,xmlObject)
{ 
  // Speed Loop Control
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("/* rotor flux observer & MRAS speed estimator","",""));
    document.write(HTML_write_comment_line_dash("*/")); 
    document.write(HTML_write_define_line_number(prefix,0,"RFO_COMP_KP_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_COMP_KI_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_KR_INV_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_LS_TOT_LEAK_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_PSI_RA1_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_PSI_RB1_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_PSI_SA1_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_PSI_SA2_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_RS_EST",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"RFO_TRQCNST",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MRAS_KP_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MRAS_KI_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MRAS_PSI_RA1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MRAS_PSI_RB1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MRAS_TS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MRAS_SPDCNST",xmlObject));
   
    
    
//    document.write(HTML_write_define_line_number(prefix,0,"RSESTIM_KP_GAIN",xmlObject));
//    document.write(HTML_write_define_line_number(prefix,0,"RSESTIM_KI_GAIN",xmlObject));
//    document.write(HTML_write_define_line_number(prefix,0,"RSESTIM_MAX",xmlObject));
//    document.write(HTML_write_define_line_number(prefix,0,"RSESTIM_MIN",xmlObject));
    
    clickCalculateRFO();
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writePoSpeRFOHeaderOutput(prefix, str)
{
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'/* rotor flux observer','');
   str = write_comment_line_dash(str,"*/");
    
   str = write_define_line_number(prefix, str,'RFO_COMP_KP_GAIN');
   str = write_define_line_number(prefix, str,'RFO_COMP_KI_GAIN');
   str = write_define_line_number(prefix, str,'RFO_KR_INV_GAIN');
   str = write_define_line_number(prefix, str,'RFO_LS_TOT_LEAK_GAIN');
   str = write_define_line_number(prefix, str,'RFO_PSI_RA1_GAIN');
   str = write_define_line_number(prefix, str,'RFO_PSI_RB1_GAIN');
   str = write_define_line_number(prefix, str,'RFO_PSI_SA1_GAIN');
   str = write_define_line_number(prefix, str,'RFO_PSI_SA2_GAIN');
   str = write_define_line_number(prefix, str,'RFO_RS_EST');
   str = write_define_line_number(prefix, str,'RFO_TRQCNST');
   
   str = write_define_line_number(prefix, str,'MRAS_KP_GAIN');
   str = write_define_line_number(prefix, str,'MRAS_KI_GAIN');
   str = write_define_line_number(prefix, str,'MRAS_PSI_RA1');
   str = write_define_line_number(prefix, str,'MRAS_PSI_RB1');
   str = write_define_line_number(prefix, str,'MRAS_TS');
   str = write_define_line_number(prefix, str,'MRAS_SPDCNST');
  

   
//   str = write_define_line_number(prefix, str,'RSESTIM_KP_GAIN');
//   str = write_define_line_number(prefix, str,'RSESTIM_KI_GAIN');
//   str = write_define_line_number(prefix, str,'RSESTIM_MAX');
//   str = write_define_line_number(prefix, str,'RSESTIM_MIN');
   
   return str;
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_RFObsrv()
{
   // update constants
   clickCalculateRFO();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
