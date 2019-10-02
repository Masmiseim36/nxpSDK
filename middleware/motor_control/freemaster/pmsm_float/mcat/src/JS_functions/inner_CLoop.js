/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause* 
*
****************************************************************************//*!
*
* @file   inner_CLoop.js
*
* @brief  Current control loop engine
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormCloop() - init Current Loop control page paramters and constants  
* clickCalculateCloop() - calculates control constants based on input parameters
* clickUpdateCloopFM() - update selected variables in FreeMASTER application
* writeCLoopHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeCLoopHeaderOutput(str,prefix) - write selected constants to output header file
* updateTab_CLoop() - callback function 
*******************************************************************************/

/***************************************************************************//*!
*
* @brief   The function loads values from inner storage table to forms based
*         on parameter ID
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormCloop()
{
   prefixM = getActiveMotor();
   var ControllerType = null;
   var FastLoopTs;
   
   /* get actual board Id if known board connected */
   var appId = parent.document.getElementById('M1typeTabName').innerHTML;
   
   // in basic mode, precalculate paramters
   if(getActiveMode()==0)
     basicModeCalcCLoop();
   
   /* If connected to target (sample time was read from target), disable param box */
   if(appId!="offline")
       disableInputParamBox("CLOOP_Ts");
    
    // Loop Sample Time
    copyParent2InnerValById("CLOOP_Ts");
   
   if(testVarValue('FFw_CL','Zero Cancellation'))
   {
     // Id of fieldset for both D and Q axis zero cancelation
     document.getElementById("Daxis_ZC").style.display = "";
     document.getElementById("Qaxis_ZC").style.display = "";
   } 
   else
   {
     document.getElementById("Daxis_ZC").style.display = "none";
     document.getElementById("Qaxis_ZC").style.display = "none";   
   } 
   
   if(testVarValue('Ccontroller','Parallel'))
   {
     document.getElementById("Daxis_PIparal").style.display = "";
     document.getElementById("Daxis_PIrecur").style.display = "none";   
     document.getElementById("Qaxis_PIparal").style.display = "";
     document.getElementById("Qaxis_PIrecur").style.display = "none";
     ControllerType = 'Parallel';
   }
   else
   {
     document.getElementById("Daxis_PIparal").style.display = "none";
     document.getElementById("Daxis_PIrecur").style.display = "";
     document.getElementById("Qaxis_PIparal").style.display = "none";
     document.getElementById("Qaxis_PIrecur").style.display = "";
     ControllerType = 'Recurrent';    
   }   
   
   copyParent2InnerValById("CLOOP_Ts");
   copyParent2InnerValById("CLOOP_F0");
   copyParent2InnerValById("CLOOP_Att");
   copyParent2InnerValById("CLOOP_LIM");

   //document.getElementById("Arithmetic").innerText = parent.document.getElementById("Arithmetic").innerText;
   document.getElementById("Ccontroller").innerText = ControllerType;
      
   // enable button enabling
   ReloadStoreButtonsOnOff(1);
     
   //calculate constants
   clickCalculateCloop();
  
}

/***************************************************************************//*!
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param
* @return  None
* @remarks
******************************************************************************/
function markUpdateCLField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("Daxis_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("Qaxis_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';  

    
    document.getElementById("CurrUpdateFrm").title = "Red-legend constants will be updated on click"
    */
}

/***************************************************************************//*!
* @brief   Unmark the Labels in the Fieldsets that's going to be updated
* @param
* @return  None
* @remarks
******************************************************************************/
function unmarkUpdateCLField()
{
    var obj = document.getElementById("Daxis_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    var obj = document.getElementById("Qaxis_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';  

    obj.className = 'fontControlLabel';       
}
/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcCLoop()
{
    var Rs            = getParentHtmlValue("Rs");
    var Lq            = getParentHtmlValue("Lq");
    var Ld            = getParentHtmlValue("Ld");
    var Ts            = getParentHtmlValue("CLOOP_Ts");
    var Ls;
    var Overshoot_dem = 20; // [%] 
    var Overshoot_err = 1;
    
    
    
    if (Ld>Lq)    Ls = Lq;
    else          Ls = Ld;
    
    if(testVarValue('FFw_CL','Zero Cancellation'))
    {
      var Att       = 1/Math.sqrt(2);
      var f0_min    = Math.round(Rs/(Ls*4*Math.PI*Att)+1);
      var w0_cal_0  = f0_min*2*Math.PI;

      while(Math.abs(Overshoot_err)>0.0001)
        {
          w0_cal  = w0_cal_0 + 0.9*Overshoot_err/Math.abs(Overshoot_err);
          w_d     = w0_cal*Math.sqrt(1-Math.pow(Att,2));
          tgt     = (-w_d*Ls)/(Rs-Att*Ls*w0_cal);
          
          if (tgt>0) t_ust = Math.atan(tgt)/w_d;
          else       t_ust = (Math.atan(tgt)+Math.PI)/w_d;            
          
          Overshoot_cal = Rs-Rs*Math.exp(-w0_cal*Att*t_ust)*(Math.cos(w_d*t_ust) + (Att*w0_cal-Math.pow(w0_cal,2)*Ls/Rs)*Math.sin(w_d*t_ust)/w_d);
          Overshoot_err = (1+Overshoot_dem/100)*Rs - Overshoot_cal;
          w0_cal_0  = w0_cal;
        }      

      f0_calc         = Math.round(w0_cal/(2*Math.PI)); 
      CLoop_bandwidth = (f0_min<f0_calc) ? f0_calc:f0_min;
      switchParam2BasicMode("CLOOP_Att",Math.round(Att*1000)/1000);      
    }
    else
    {
      var f0_min  = Math.round(Rs/(Ls*4*Math.PI)+1);
      var f0_calc = Math.round(((Overshoot_dem/100)*Rs + Rs)/(4*Math.PI*Ls));

      CLoop_bandwidth =  (f0_min<f0_calc) ? f0_calc:f0_min;
      switchParam2BasicMode("CLOOP_Att",1);            
    }
      
    // replace and disable params
    switchParam2BasicMode("CLOOP_F0",200);
    switchParam2BasicMode("CLOOP_LIM",90);
    switchParam2BasicMode("CLOOP_Ts",Ts);
}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters  
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculateCloop()
{
    var fo    = getParentHtmlValue("CLOOP_F0");
    var Att   = getParentHtmlValue("CLOOP_Att");
    var Ts    = getParentHtmlValue("CLOOP_Ts");       
    var Rs    = getParentHtmlValue("Rs");
    var Ld    = getParentHtmlValue("Ld");
    var Lq    = getParentHtmlValue("Lq");
    var Ulim  = getParentHtmlValue("CLOOP_LIM");
    var Imax  = getParentHtmlValue("I_max");
    var UDCmax        = getParentHtmlValue("UDC_max");
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    
    
    var IIRxCoefsScaleType     = parent.document.getElementById("IIRxCoefsScale").innerText;
    var Umax          = Math.round(UDCmax/UmaxCoeff*10)/10;  
            
   
    
    // current controller limit in percentage of DC_BUS voltage actual
    Ulim_out = Math.round(Ulim/100*100)/100;

    // self-checking of Att value, it must be in a range (0.5 - 2)
    if (Att<0.5){
      alert("Entered value of Damping factor cannot be less than: 0.5 ");
      var prefixM = getActiveMotor();
      Att = 0.5;
      document.getElementById(prefixM + "CLOOP_Att").value=Att;      
      parent.document.getElementById(prefixM + "CLOOP_Att").innerHTML=Att;      
      }
   if (Att>2){
      alert("Entered value of Damping factor cannot be more than: 2 ");
      var prefixM = getActiveMotor();
      Att = 2;
      document.getElementById(prefixM + "CLOOP_Att").value=Att;      
      parent.document.getElementById(prefixM + "CLOOP_Att").innerHTML=Att;      
      }

    // self-checking of F0 value, it cannot be less than F0_min calculated from Kp equation
    if (Ld>Lq)    Ls = Lq;
    else          Ls = Ld;
    
    var f0_min = Rs/(4*Att*Math.PI*Ls);
    
    if (fo<f0_min){
      fo = Math.round(f0_min);
      alert("Entered value of Natural frequency F0 cannot be less than: " + Math.round(f0_min));
      var prefixM = getActiveMotor();
      document.getElementById(prefixM + "CLOOP_F0").value=Math.round(f0_min+1);
      parent.document.getElementById(prefixM + "CLOOP_F0").innerHTML=Math.round(f0_min+1);      
       }

    /* check preset discretization method */
    var DiscMethod = parent.document.getElementById("DiscMethod").innerText;
    var DiscMethodFactor = 1;
    if(DiscMethod=="Trapezoidal")
      DiscMethodFactor = 2; 
      //////////////////////// Related to D axis ///////////////////////////////				
  	  D_Kps = 2*Att*2*Math.PI*fo*Ld-Rs;
  	  D_Kis = Math.pow((2*Math.PI*fo),2)*Ld;
      
      D_Kpz = D_Kps;
      D_Kiz = D_Kis*Ts/DiscMethodFactor;
     
      ////// PARALLEL PI CONTROLLER TYPE ///////
      D_Kpz_f = D_Kpz*Imax/Umax; 
      D_Kiz_f = D_Kiz*Imax/Umax;
      
      ///// ************** RECCURENT PI CONTROLLER TYPE ***************** ///////
      // scaling to scale and scale shift components
      D_CC1s =  Math.round((D_Kps + D_Kis*Ts/2)*1000000000000)/1000000000000;	
      D_CC2s =  Math.round((-D_Kps + D_Kis*Ts/2)*1000000000000)/1000000000000;
      	
      // scale constants
      D_CC1f = D_CC1s*Imax/Umax;
      D_CC2f = D_CC2s*Imax/Umax;
    
      // scale shift
      if ((Math.abs(D_CC1f)<1) && (Math.abs(D_CC2f)<1))   D_Nshift = 0;
      else
      {
        if (Math.abs(D_CC1f) > Math.abs(D_CC2f))
            D_Nshift = Math.ceil(Math.log(Math.abs(D_CC1f))/Math.log(2));
        else
            D_Nshift = Math.ceil(Math.log(Math.abs(D_CC2f))/Math.log(2));        
      }
      
      D_CC1_out = Math.round(D_CC1f/Math.pow(2,D_Nshift)*1000000000000)/1000000000000;
      D_CC2_out = Math.round(D_CC2f/Math.pow(2,D_Nshift)*1000000000000)/1000000000000;
      testFracValRange("D_CC1_out",D_CC1_out);
      testFracValRange("D_CC2_out",D_CC2_out,1);
      
      // Zero-cross cancelation filter constants
      D_ZC_B0_fl = D_Kis*Ts/(D_Kps+D_Kis*Ts);
      D_ZC_B1_fl = 0;
      D_ZC_A1_fl = -D_Kps/(D_Kps+D_Kis*Ts);
      D_ZC_B0_fl_out = Math.round(D_ZC_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
      D_ZC_B1_fl_out = Math.round(D_ZC_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
      D_ZC_A1_fl_out = Math.round(D_ZC_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
      D_ZC_B0_f_out  = Math.round(D_ZC_B0_fl_out*Imax/Umax*1000000000000)/1000000000000;
      D_ZC_B1_f_out  = Math.round(D_ZC_B1_fl_out*Imax/Umax*1000000000000)/1000000000000;
      D_ZC_A1_f_out  = Math.round(D_ZC_A1_fl_out*Imax/Umax*1000000000000)/1000000000000;
      testFracValRange("D_ZC_B0_f_out",D_ZC_B0_f_out,1);
      testFracValRange("D_ZC_B1_f_out",D_ZC_B1_f_out,1);
      testFracValRange("D_ZC_A1_f_out",D_ZC_A1_f_out,1);
      
      //////////////////////// Related to Q axis ///////////////////				
      Q_Kps = 2*Att*2*Math.PI*fo*Lq-Rs;
      Q_Kis = Math.pow((2*Math.PI*fo),2)*Lq;	
    		
      Q_Kpz = Q_Kps;
      Q_Kiz = Q_Kis*Ts/DiscMethodFactor;
      
      ////// PARALLEL PI CONTROLLER TYPE ///////
      Q_Kpz_f = Q_Kpz*Imax/Umax; 
      Q_Kiz_f = Q_Kiz*Imax/Umax;
      
      
      ////// RECCURENT PI CONTROLLER TYPE ///////
      // scaling to scale and scale shift components
      Q_CC1s = Math.round((Q_Kps + Q_Kis*Ts/2)*1000000000000)/1000000000000;	
    	Q_CC2s = Math.round((-Q_Kps + Q_Kis*Ts/2)*1000000000000)/1000000000000;
     
      Q_CC1f = Q_CC1s*Imax/Umax;
      Q_CC2f = Q_CC2s*Imax/Umax;
    
      // scale shift
      if ((Math.abs(Q_CC1f)<1) && (Math.abs(Q_CC2f)<1))  Q_Nshift = 0;
      else
      {
        if (Math.abs(Q_CC1f) > Math.abs(Q_CC2f))
            Q_Nshift = Math.ceil(Math.log(Math.abs(Q_CC1f))/Math.log(2));
        else
            Q_Nshift = Math.ceil(Math.log(Math.abs(Q_CC2f))/Math.log(2));        
      }
      
      Q_CC1_out = Math.round(Q_CC1f/Math.pow(2,Q_Nshift)*1000000000000)/1000000000000;
      Q_CC2_out = Math.round(Q_CC2f/Math.pow(2,Q_Nshift)*1000000000000)/1000000000000;
      testFracValRange("D_CC1_out",D_CC1_out);
      testFracValRange("D_CC2_out",D_CC2_out,1);
      
      // Zero-cross cancelation filter constants
      Q_ZC_B0_fl = Q_Kis*Ts/(Q_Kps+Q_Kis*Ts);
      Q_ZC_B1_fl = 0;
      Q_ZC_A1_fl = -Q_Kps/(Q_Kps+Q_Kis*Ts);
      Q_ZC_B0_fl_out = Math.round(Q_ZC_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
      Q_ZC_B1_fl_out = Math.round(Q_ZC_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
      Q_ZC_A1_fl_out = Math.round(Q_ZC_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
      Q_ZC_B0_f_out  = Math.round(Q_ZC_B0_fl_out*Imax/Umax*1000000000000)/1000000000000;
      Q_ZC_B1_f_out  = Math.round(Q_ZC_B1_fl_out*Imax/Umax*1000000000000)/1000000000000;
      Q_ZC_A1_f_out  = Math.round(Q_ZC_A1_fl_out*Imax/Umax*1000000000000)/1000000000000;
      testFracValRange("Q_ZC_B0_f_out",Q_ZC_B0_f_out,1);
      testFracValRange("Q_ZC_B1_f_out",Q_ZC_B1_f_out,1);
      testFracValRange("Q_ZC_A1_f_out",Q_ZC_A1_f_out,1);
      
     
     // If CURRENT LOOP tab is active ******************************************
      if(document.getElementById("CLoop") != undefined)
      {
        ////// RECCURENT PI CONTROLLER TYPE ///////
        setInnerHtmlValue("D_CC1",D_CC1s,D_CC1s);
        setInnerHtmlValue("D_CC2",D_CC2s,D_CC2s);
        
        setInnerHtmlValue("Q_CC1",Q_CC1s,Q_CC1s);
        setInnerHtmlValue("Q_CC2",Q_CC2s,Q_CC2s);
    
        ////// PARALLEL PI CONTROLLER TYPE ///////
        setInnerHtmlValue("D_Kp_g",D_Kpz_f,D_Kpz);
        setInnerHtmlValue("D_Ki_g",D_Kiz_f,D_Kiz);        
        
        setInnerHtmlValue("Q_Kp_g",Q_Kpz_f,Q_Kpz);
        setInnerHtmlValue("Q_Ki_g",Q_Kiz_f,Q_Kiz);        
    
       // Zero cancelation
        setInnerHtmlValue("D_ZC_B0",D_ZC_B0_f_out,D_ZC_B0_fl);
        setInnerHtmlValue("D_ZC_B1",D_ZC_B1_f_out,D_ZC_B1_fl);
        setInnerHtmlValue("D_ZC_A1",D_ZC_A1_f_out,D_ZC_A1_fl);
          
        setInnerHtmlValue("Q_ZC_B0",Q_ZC_B0_f_out,Q_ZC_B0_fl);
        setInnerHtmlValue("Q_ZC_B1",Q_ZC_B1_f_out,Q_ZC_B1_fl);
        setInnerHtmlValue("Q_ZC_A1",Q_ZC_A1_f_out,Q_ZC_A1_fl);               
      }      
  
      // If HEADER FILE tab is active ********************************************
      if(document.getElementById("HeaderFileTab") != undefined)
      {
        setInnerHtmlValueAsText("CLOOP_LIMIT",0,Ulim_out, Math.round((Ulim_out/UmaxCoeff)*1e12)/1e12);
        
        if(testVarValue('Ccontroller','Parallel')) // parallel type of PI controller
        {
            ////// PARALLEL PI CONTROLLER TYPE ///////
            setInnerHtmlValueAsText("D_KP_GAIN",0,D_Kpz_f,Math.round(D_Kpz*1e12)/1e12);
            setInnerHtmlValueAsText("D_KI_GAIN",0,D_Kiz_f,Math.round(D_Kiz*1e12)/1e12);
            
            setInnerHtmlValueAsText("Q_KP_GAIN",0,Q_Kpz_f,Math.round(Q_Kpz*1e12)/1e12);
            setInnerHtmlValueAsText("Q_KI_GAIN",0,Q_Kiz_f,Math.round(Q_Kiz*1e12)/1e12);  
         }
         else // reccurent type of PI controller
         {
            ////// RECCURENT PI CONTROLLER TYPE ///////
            setInnerHtmlValueAsText("D_NSHIFT",1,D_Nshift,'N/A');
            setInnerHtmlValueAsText("D_CC1SC",0,D_CC1_out,D_CC1s);
            setInnerHtmlValueAsText("D_CC2SC",0,D_CC2_out,D_CC2s);
            
            setInnerHtmlValueAsText("Q_NSHIFT",1,Q_Nshift,'N/A');
            setInnerHtmlValueAsText("Q_CC1SC",0,Q_CC1_out,Q_CC1s);
            setInnerHtmlValueAsText("Q_CC2SC",0,Q_CC2_out,Q_CC2s);
         }
          
         if(testVarValue('FFw_CL','Zero Cancelation'))
         {
            // Zero cancelation
            setInnerHtmlValueAsText("D_ZC_B0",0,D_ZC_B0_f_out,D_ZC_B0_fl);
            setInnerHtmlValueAsText("D_ZC_B1",0,D_ZC_B1_f_out,D_ZC_B1_fl);
            setInnerHtmlValueAsText("D_ZC_A1",0,D_ZC_A1_f_out,D_ZC_A1_fl);
            
            setInnerHtmlValueAsText("Q_ZC_B0",0,Q_ZC_B0_f_out,Q_ZC_B0_fl);
            setInnerHtmlValueAsText("Q_ZC_B1",0,Q_ZC_B1_f_out,Q_ZC_B1_fl);
            setInnerHtmlValueAsText("Q_ZC_A1",0,Q_ZC_A1_f_out,Q_ZC_A1_fl);
         }        
      }  
      
  //  } // end if(fo<fmin)  
} 

function clickUpdateCloopFM()
{
  var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");

  if(!pcm.IsCommPortOpen())
  {
      alert("Communication is stopped.\nPress Ctrl+K to start the communication");
  }
  else
  {
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml");
    var errorArray = [];   
    
    var aritType     = parent.document.getElementById("Arithmetic").innerText;
        
    errorArray.push(UpdateFMVariable(xmlDoc,'CL_LIMIT',Ulim_out,(Ulim_out/UmaxCoeff)));
    
    
    if(testVarValue('Ccontroller','Parallel')) // parallel type of PI controller
     {
        errorArray.push(UpdateFMVariable(xmlDoc,'D_Kp_g',D_Kpz_f,D_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'D_Ki_g',D_Kiz_f,D_Kiz));
        
     }
     else // reccurent type of PI controller
     {
        if (aritType!=='Float')
        errorArray.push(UpdateFMVariable(xmlDoc,'D_NSHIFT',D_Nshift));
        errorArray.push(UpdateFMVariable(xmlDoc,'D_CC1SC',D_CC1_out,D_CC1s));
        errorArray.push(UpdateFMVariable(xmlDoc,'D_CC2SC',D_CC2_out,D_CC2s));
     }
     
     // Zero cancelation filter enable
     if(testVarValue('FFw_CL','Zero Cancelation'))
     {
        errorArray.push(UpdateFMVariable(xmlDoc,'D_ZC_B0',D_ZC_B0_f_out,D_ZC_B0_fl));
        errorArray.push(UpdateFMVariable(xmlDoc,'D_ZC_A1',D_ZC_A1_f_out,D_ZC_A1_fl));
        errorArray.push(UpdateFMVariable(xmlDoc,'D_ZC_B1',D_ZC_B1_f_out,D_ZC_B1_fl));        
     }   
     
     
      //////////////////////////////////////////////////////////////////////////////////
     
     if(testVarValue('Ccontroller','Parallel')) // parallel type of PI controller
     {
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_Kp_g',Q_Kps,Q_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_Ki_g',Q_Kis,Q_Kiz));
        
     }
     else // reccurent type of PI controller
     {
        if (aritType!=='Float')
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_NSHIFT',Q_Nshift));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_CC1SC',Q_CC1_out,Q_CC1s));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_CC2SC',Q_CC2_out,Q_CC2s));
     }
     
     // Zero cancelation filter enable
     if(testVarValue('FFw_CL','Zero Cancelation'))
     {
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_ZC_B0',Q_ZC_B0_f_out,Q_ZC_B0_fl));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_ZC_A1',Q_ZC_A1_f_out,Q_ZC_A1_fl));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_ZC_B1',Q_ZC_B1_f_out,Q_ZC_B1_fl));        
     } 
    
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
function writeCLoopHTMLOutput(prefix,xmlObject)
{
      // Current Loop Control
      document.write(HTML_write_blank_line());              
      document.write(HTML_write_comment_line("Current Loop Control","",""));
      document.write(HTML_write_comment_line_dash());       
      
      document.write(HTML_write_comment_line("Loop bandwidth","CLOOP_F0",""));
      document.write(HTML_write_comment_line("Loop attenuation","CLOOP_Att",""));
      document.write(HTML_write_comment_line("Loop sample time","CLOOP_Ts",""));
      document.write(HTML_write_blank_line());
      document.write(HTML_write_define_line_number(prefix,0,"CLOOP_LIMIT",xmlObject));
      
      // D - axis
      document.write(HTML_write_blank_line()); 
      document.write(HTML_write_comment_line("D - axis parameters","",""));
      
      if(testVarValue('Ccontroller','Parallel'))
      {
        document.write(HTML_write_define_line_number(prefix,0,"D_KP_GAIN",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"D_KI_GAIN",xmlObject));
      }
      else
      {
        document.write(HTML_write_define_line_number(prefix,1,"D_NSHIFT",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"D_CC1SC",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"D_CC2SC",xmlObject));
      }
  
      if(testVarValue('FFw_CL','Zero Cancellation'))
      {              
        document.write(HTML_write_define_line_number(prefix,0,"D_ZC_B0",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"D_ZC_B1",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"D_ZC_A1",xmlObject));
      }      
        
      // Q - axis
      document.write(HTML_write_blank_line());                              
      document.write(HTML_write_comment_line("Q - axis parameters","",""));
     
      if(testVarValue('Ccontroller','Parallel'))
      {
        document.write(HTML_write_define_line_number(prefix,0,"Q_KP_GAIN",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"Q_KI_GAIN",xmlObject));
      }
      else
      {
        document.write(HTML_write_define_line_number(prefix,1,"Q_NSHIFT",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"Q_CC1SC",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"Q_CC2SC",xmlObject));
      }
                   
      if(testVarValue('FFw_CL','Zero Cancellation'))
      {              
        document.write(HTML_write_define_line_number(prefix,0,"Q_ZC_B0",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"Q_ZC_B1",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"Q_ZC_A1",xmlObject));
      } 
      
      copyParent2HeaderCfgById('CLOOP_F0','CLOOP_F0',' [Hz]',true);
      copyParent2HeaderCfgById('CLOOP_Att','CLOOP_Att',' [-]',true);
      copyParent2HeaderCfgById('CLOOP_Ts','CLOOP_Ts',' [sec]',true);
      clickCalculateCloop();

}
 
/***************************************************************************//*!
*
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeCLoopHeaderOutput(prefix, str) 
{
     str = write_blank_lines(str,1);     
     str = write_comment_text(str,'Current Loop Control','');
     str = write_comment_line_dash(str);
     str = write_comment_text(str,'Loop bandwidth','CLOOP_F0');    
     str = write_comment_text(str,'Loop attenuation','CLOOP_Att');
     str = write_comment_text(str,'Loop sample time','CLOOP_Ts');
     str = write_comment_line_dash(str);
   
     str = write_comment_text(str,'Current Controller Output Limit ','','');
     str = write_define_line_number(prefix, str,'CLOOP_LIMIT');
      
     //D-axis controller
     if(testVarValue('Ccontroller','Parallel'))
     {
        str = write_comment_text(str,'D-axis Controller - Parallel type','','');
        str = write_define_line_number(prefix, str,'D_KP_GAIN'); 
        str = write_define_line_number(prefix, str,'D_KI_GAIN');
      }
      else
      {
        str = write_comment_text(str,'D-axis Controller - Recurrent type','','');      
        str = write_define_line_number(prefix, str,'D_NSHIFT');
        str = write_define_line_number(prefix, str,'D_CC1SC');
        str = write_define_line_number(prefix, str,'D_CC2SC');
      }
      
      if(testVarValue('FFw_CL','Zero Cancelation'))
      {              
        str = write_define_line_number(prefix, str,'D_ZC_B0');
        str = write_define_line_number(prefix, str,'D_ZC_B1');
        str = write_define_line_number(prefix, str,'D_ZC_A1');
      }      
        
      // Q - axis
      if(testVarValue('Ccontroller','Parallel'))
     {
        str = write_comment_text(str,'Q-axis Controller - Parallel type','','');
        str = write_define_line_number(prefix, str,'Q_KP_GAIN'); 
        str = write_define_line_number(prefix, str,'Q_KI_GAIN');
      }
      else
      {
        str = write_comment_text(str,'Q-axis Controller - Recurrent type','','');
        str = write_define_line_number(prefix, str,'Q_NSHIFT');
        str = write_define_line_number(prefix, str,'Q_CC1SC');
        str = write_define_line_number(prefix, str,'Q_CC2SC');
      }
      
      if(testVarValue('FFw_CL','Zero Cancelation'))
      {              
        str = write_define_line_number(prefix, str,'Q_ZC_B0');
        str = write_define_line_number(prefix, str,'Q_ZC_B1');
        str = write_define_line_number(prefix, str,'Q_ZC_A1');
      }
      
    return str;  
}
/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_CLoop()
{
   // update constants
   clickCalculateCloop();
}


/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
