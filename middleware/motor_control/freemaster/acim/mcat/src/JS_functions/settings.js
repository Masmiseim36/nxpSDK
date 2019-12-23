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
* @file   settings.js
*
* @brief  Setting panel functions of MCAT Tool IDE
* 
******************************************************************************/

/******************************************************************************
* List of functions
********************************************************************************
* conversionDataType(inPointer)
* conversionMotorsNo(inPointer)
* conversionMotorType(inPointer)
* conversionPItypes(inPointer)
* conversionAlignType(inPointer
* conversionFFwCLType(inPointer)
* conversionFFwSLType(inPointer)
* conversionWFilterType(inPointer)
* swapDT(in_formName, in_checkVal)
* swapNM(in_formName, in_checkVal)
* swapPI(in_formName, in_checkVal)
* initLoadSettings()
* selectMot(iD, inValue)
* build_Motor_tab_manager(tableNum, tabArray, tabNumber)
* setTabOrder(motorNumber)
* setAllTabOff()
* clickApply()
* clickCancel()
* generate_table_Motor()
* PospeFbckClick()
* PospeFbckInit()
* ClButtonClick(Mbutton_id)
* selectCFF(iD, inValue)
* selectSFF(iD, inValue)
* selectWF(iD, inValue)
* browseDirectory()                         
*******************************************************************************/
/***************************************************************************//*!
* @brief   The function converts "Sata type" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/   
function conversionDataType(inPointer)
{
    var arithmeticArray=new Array(3);
    arithmeticArray[0] = "Frac16";
    arithmeticArray[1] = "Frac32";
    arithmeticArray[2] = "Float";

    for(i=0;i<3;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
}
    
/***************************************************************************//*!
* @brief   The function converts "Number of motors" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/       
function conversionMotorsNo(inPointer)
{
    var arithmeticArray=new Array(3);
    arithmeticArray[0] = "Single";
    arithmeticArray[1] = "Dual";
    arithmeticArray[2] = "Triple";

    for(i=0;i<3;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
}
    
/***************************************************************************//*!
* @brief   The function converts "Type of motor" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/   
function conversionMotorType(inPointer)
{
    var arithmeticArray=new Array(3);
    arithmeticArray[0] = "none";
    arithmeticArray[1] = "PMSM";
    arithmeticArray[2] = "BLDC";
    arithmeticArray[3] = "ACIM";

    for(var i=0;i<4;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
}
    
/***************************************************************************//*!
* @brief   The function converts "Type of PI controller" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/     
function conversionPItypes(inPointer)
{
    var arithmeticArray=new Array(2);
    arithmeticArray[0] = "Parallel";
    arithmeticArray[1] = "Recurrent";
  
    for(i=0;i<2;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
 } 

/***************************************************************************//*!
* @brief   The function converts "Type of PI controller" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/     
function conversionAlignType(inPointer)
{
    var arithmeticArray=new Array(2);
    arithmeticArray[0] = "Voltage";
    arithmeticArray[1] = "Current";

    for(i=0;i<2;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
} 
  
/***************************************************************************//*!
* @brief   The function converts "Type of PI controller" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/     
function conversionDiscMethod(inPointer)
{
    var DiscMethodArray=new Array(3);
    DiscMethodArray[0] = "EulerBW";
    DiscMethodArray[1] = "EulerFW";
    DiscMethodArray[2] = "Trapezoidal";
    
    for(i=0;i<3;i++){
      if(inPointer== DiscMethodArray[i])     
        return (i);
    }
}       
/***************************************************************************//*!
* @brief   The function converts "Type of current feedforward path" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/   
function conversionFFwCLType(inPointer)
{
    var arithmeticArray=new Array(3);
    arithmeticArray[0] = "None";
    arithmeticArray[1] = "Zero Cancellation";

    for(var i=0;i<2;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
} 
    
/***************************************************************************//*!
* @brief   The function converts "Type of speed feedforward path" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/   
function conversionFFwSLType(inPointer)
{
    var arithmeticArray=new Array(3);
    arithmeticArray[0] = "None";
    arithmeticArray[1] = "Incremental Ramp";
    arithmeticArray[2] = "Dynamic Ramp";
    arithmeticArray[3] = "Zero Cancellation";            

    for(var i=0;i<4;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
}
  
/***************************************************************************//*!
* @brief   The function converts "Type of speed filter" from String to Number form
* @param   
* @return  None
* @remarks 
*******************************************************************************/   
function conversionWFilterType(inPointer)
{
    var arithmeticArray=new Array(3);
    arithmeticArray[0] = "MA Filter";
    arithmeticArray[1] = "IIR Filter";

    for(var i=0;i<2;i++){
      if(inPointer== arithmeticArray[i])     
        return (i);
    }
}                 
/***************************************************************************//*!
* @brief    Swap the Data Type checkboxes and stores selected aritmetic to the main tab
* @param    name of the form
*           check box value
* @return  None
* @remarks 
*******************************************************************************/     
function swapDT(in_formName, in_checkVal)
{
    var form_l         = in_formName.length;
    var formName       = in_formName.name; 
    
    for (i=0;i<form_l;i++)
    {
     in_formName.checkType[i].checked = false; 
      
     if(in_formName.checkType[i].value == in_checkVal)
          in_formName.checkType[i].checked = true;       
    }   

    parent.document.getElementById(formName).innerHTML = in_formName.checkType[in_checkVal-1].name;
}
    
/***************************************************************************//*!
* @brief    IIRx Filter scale selector and storing selected divider to the main tab
* @param    name of the form
*           check box value
* @return  None
* @remarks 
*******************************************************************************/     
function  selectIRRxScale(iD, inValue)
{
    parent.document.getElementById(iD).innerHTML = inValue;
    //document.getElementById("test_m").innerText =  inValue;
}    

/***************************************************************************//*!
* @brief    IIRx Filter scale selector and storing selected divider to the main tab
* @param    name of the form
*           check box value
* @return  None
* @remarks 
*******************************************************************************/     
function  selectDiscMethod(iD, inValue)
{
   switch(inValue)
    {
      case "0":
        parent.document.getElementById(iD).innerHTML = "EulerBW";
        break;
      case "1":
        parent.document.getElementById(iD).innerHTML = "EulerFW";
        break;
      case "2":
        parent.document.getElementById(iD).innerHTML = "Trapezoidal";
        break;
      default:
    }
}  
    
/***************************************************************************//*!
* @brief    Swap the Number of Motors checkboxes and stores selected value in the main tab
* @param    name of the form
*           check box value
* @return  None
* @remarks 
*******************************************************************************/     
function swapNM(in_formName, in_checkVal)
{
    var form_l         = in_formName.length;
    var formName       = in_formName.name;   //MotorsNo
    var object;
    var temp = 0;       
    
    /* Allows/disables the pop-up menu in Motor Types fieldset according to the number of motors*/
    for (var i=0;i<form_l;i++)
    {
     // object = Id of Motor Type selector
     object = document.getElementById("M"+[i+1]+"type");
     // default setting - all selectors are enabled
     object.disabled = false;
     // if second motor selected, bg color set
     if(i==1) object.style.background =  "rgb(40,113,138)";
     // if third motor selected, bg color set
     if(i==2) object.style.background =  "rgb(7,120,24)";
     // default setting - all checkboxes are unmarked        
     in_formName.checkType[i].checked = false;
     
     // Read the selected index (motor type) from the main page (parent document)  
     object.selectedIndex = conversionMotorType(parent.document.getElementById("M"+[i+1]+"type").innerText);
     
     // temp represents a moment (i) where  object (Motor Type selector) becames disabled
     if(temp){
     object.disabled = true;
     object.style.background  = "rgb(164,172,178)";
     object.selectedIndex     = "none"
     } 
           
     // find which Number of Motor checbox has been chosen        
     if(in_formName.checkType[i].value == in_checkVal)
      {
          // mark the selected checkbox
          in_formName.checkType[i].checked = true;
          // Motor Type selector for higher i will be disabled 
          temp = 1;
      }
    } 
    
    // store selected Number of Motors
    parent.document.getElementById(formName).innerHTML = in_formName.checkType[in_checkVal-1].name;
    
    /* Add/remove the number of tabels in TW Tab according to the selected motors*/
    for(var i=0;i<3;i++)
    {
        // Control loop settings - number of motors        
        document.getElementById('M'+[i+1]+'_ClButton').style.display = "none";
        
        if(i>0){
        parent.document.getElementById('idMotor'+[i+1]+'Rad').style.display = 'none';
        parent.document.getElementById('idMotor'+[i+1]+'Tab').style.display = 'none';
        }          
        
        if(i<in_checkVal)
        {
        // Control loop settings - number of motors        
        document.getElementById('M'+[i+1]+'_ClButton').style.display = "";

          if(i>0){
          parent.document.getElementById('idMotor'+[i+1]+'Rad').style.display = '';
          parent.document.getElementById('idMotor'+[i+1]+'Tab').style.display = '';
          }       
        }              
     }
       
    var MotorsNumber          = parent.document.getElementById("MotorsNo").innerText; 
    var MotorsNumber_No       = conversionMotorsNo(MotorsNumber);  
    
    
    
    // define default setting when Single motor option is choosen
    if (MotorsNumber_No == 0){
            document.getElementById('M1_ClButton').style.background = "rgb(80,87,94)";
            document.getElementById('M2_ClButton').style.background = "transparent";
            document.getElementById('M3_ClButton').style.background = "transparent";            
            document.getElementById('M1_ClButton').style.color = "white";
            document.getElementById('M2_ClButton').style.color = "black";
            document.getElementById('M3_ClButton').style.color = "black";
            document.getElementById('M1_ClSetting').style.display = "block";
            document.getElementById('M2_ClSetting').style.display = "none";
            document.getElementById('M3_ClSetting').style.display = "none"; 
            
            document.getElementById('tab11line').style.display = '';
            document.getElementById('tab12line').style.display = '';
            document.getElementById('tab13line').style.display = ''; 
          
            document.getElementById('tab21line').style.display = 'none';
            document.getElementById('tab22line').style.display = 'none';
            document.getElementById('tab23line').style.display = 'none';
        
            document.getElementById('tab31line').style.display = 'none';
            document.getElementById('tab32line').style.display = 'none';
            document.getElementById('tab33line').style.display = 'none';           
    } 
    // define default setting when Dual motor option is choosen
    if (MotorsNumber_No == 1){
            document.getElementById('M1_ClButton').style.background = "rgb(80,87,94)";
            document.getElementById('M2_ClButton').style.background = "transparent";
            document.getElementById('M3_ClButton').style.background = "transparent";            
            document.getElementById('M1_ClButton').style.color = "white";
            document.getElementById('M2_ClButton').style.color = "black";
            document.getElementById('M3_ClButton').style.color = "black";
            document.getElementById('M1_ClSetting').style.display = "block";
            document.getElementById('M2_ClSetting').style.display = "none";
            document.getElementById('M3_ClSetting').style.display = "none"; 
            
            document.getElementById('tab11line').style.display = '';
            document.getElementById('tab12line').style.display = '';
            document.getElementById('tab13line').style.display = ''; 
          
            document.getElementById('tab21line').style.display = 'none';
            document.getElementById('tab22line').style.display = 'none';
            document.getElementById('tab23line').style.display = 'none';
        
            document.getElementById('tab31line').style.display = 'none';
            document.getElementById('tab32line').style.display = 'none';
            document.getElementById('tab33line').style.display = 'none';           
    }       
    
    var valDec = new Array(4);
    valDec[1] = 0;
    valDec[2] = 0;
    valDec[3] = 0;       
       
     for (var j=0; j < (MotorsNumber_No+1); j++)
     {
        valDec[j+1]    = parent.document.getElementById('Tab'+[j+1]+'manager').innerHTML ;
        
        for(var i=0;i<10;i++)
        {
        var ChBObject  = document.getElementById("ChB"+[i]+"_"+[j+1]);
        
         if(((valDec[j+1])>>>i)&1)
          {
              ChBObject.checked = true;
          }
          else
          {
              ChBObject.checked = false;
          }
       }  
     } 
}
  
/***************************************************************************//*!
* @brief    Swap the PI controllers checkboxes and stores selected value to the main tab
* @param    name of the form
*           check box value
* @return  None
* @remarks 
*******************************************************************************/   
function swapPI(in_formName, in_checkVal)
{
    var form_l         = in_formName.length;
    var formName       = in_formName.name; 
    
    for (var i=0;i<form_l;i++)
    {
     in_formName.checkType[i].checked = false; 
      
     if(in_formName.checkType[i].value == in_checkVal)
          in_formName.checkType[i].checked = true;       
    }   

    parent.document.getElementById(formName).innerHTML = in_formName.checkType[in_checkVal].name;
}
    
/***************************************************************************//*!
* @brief    Initial settings provided with SettingPage load
* @param    
* @return   None
* @remarks 
*******************************************************************************/
 function initLoadSettings()
 {
    // init variables
    var ArithmeticType          = parent.document.getElementById("Arithmetic").innerText; 
    var MotorsNumber            = parent.document.getElementById("MotorsNo").innerText;
    
    var M1_CcontrollerType      = parent.document.getElementById("M1_Ccontroller").innerText;
    var M1_ScontrollerType      = parent.document.getElementById("M1_Scontroller").innerText;
    var M1_TOcontrollerType     = parent.document.getElementById("M1_TOcontroller").innerText;      
    var M2_CcontrollerType      = parent.document.getElementById("M2_Ccontroller").innerText;
    var M2_ScontrollerType      = parent.document.getElementById("M2_Scontroller").innerText;
    var M2_TOcontrollerType     = parent.document.getElementById("M2_TOcontroller").innerText;      
    var M3_CcontrollerType      = parent.document.getElementById("M3_Ccontroller").innerText;
    var M3_ScontrollerType      = parent.document.getElementById("M3_Scontroller").innerText;
    var M3_TOcontrollerType     = parent.document.getElementById("M3_TOcontroller").innerText;  

    var M1_AlignmentType        = parent.document.getElementById("M1_Alignment").innerText;
    var M2_AlignmentType        = parent.document.getElementById("M2_Alignment").innerText;
    var M3_AlignmentType        = parent.document.getElementById("M3_Alignment").innerText;
    
    // Current Loop feedforward path
    var M1_FFw_CLType           = parent.document.getElementById("M1_FFw_CL").innerText;
    var M2_FFw_CLType           = parent.document.getElementById("M2_FFw_CL").innerText;
    var M3_FFw_CLType           = parent.document.getElementById("M3_FFw_CL").innerText;
    // Speed Loop feedforward path 
    var M1_FFw_SLType           = parent.document.getElementById("M1_FFw_SL").innerText;
    var M2_FFw_SLType           = parent.document.getElementById("M2_FFw_SL").innerText;
    var M3_FFw_SLType           = parent.document.getElementById("M3_FFw_SL").innerText;
    // Speed Filter     
    var M1_WFiltType            = parent.document.getElementById("M1_WFilt").innerText;
    var M2_WFiltType            = parent.document.getElementById("M2_WFilt").innerText;
    var M3_WFiltType            = parent.document.getElementById("M3_WFilt").innerText;                   

    // conversion from a string form into the number form      
    var ArithmeticType_No       =   conversionDataType(ArithmeticType);
    var MotorsNumber_No         =   conversionMotorsNo(MotorsNumber);
        
    var M1_CcontrollerType_No   =   conversionPItypes(M1_CcontrollerType);
    var M1_ScontrollerType_No   =   conversionPItypes(M1_ScontrollerType);
    var M1_TOcontrollerType_No  =   conversionPItypes(M1_TOcontrollerType);
    var M2_CcontrollerType_No   =   conversionPItypes(M2_CcontrollerType);
    var M2_ScontrollerType_No   =   conversionPItypes(M2_ScontrollerType);
    var M2_TOcontrollerType_No  =   conversionPItypes(M2_TOcontrollerType);
    var M3_CcontrollerType_No   =   conversionPItypes(M3_CcontrollerType);
    var M3_ScontrollerType_No   =   conversionPItypes(M3_ScontrollerType);
    var M3_TOcontrollerType_No  =   conversionPItypes(M3_TOcontrollerType);
    
    var M1_AlignmentType_No     =   conversionAlignType(M1_AlignmentType);
    var M2_AlignmentType_No     =   conversionAlignType(M2_AlignmentType);
    var M3_AlignmentType_No     =   conversionAlignType(M3_AlignmentType);
    
    var M1_FFw_CLType_No        =   conversionFFwCLType(M1_FFw_CLType);
    var M2_FFw_CLType_No        =   conversionFFwCLType(M2_FFw_CLType);
    var M3_FFw_CLType_No        =   conversionFFwCLType(M3_FFw_CLType);    
    
    var M1_FFw_SLType_No        =   conversionFFwSLType(M1_FFw_SLType);
    var M2_FFw_SLType_No        =   conversionFFwSLType(M2_FFw_SLType);
    var M3_FFw_SLType_No        =   conversionFFwSLType(M3_FFw_SLType); 
    
    var M1_WFiltType_No         =   conversionWFilterType(M1_WFiltType);
    var M2_WFiltType_No         =   conversionWFilterType(M2_WFiltType);
    var M3_WFiltType_No         =   conversionWFilterType(M3_WFiltType);     
    
    // IIRx Filter Coeficient scale calculation - selector initialization
    var IIRxCoefsScaleType      = parent.document.getElementById("IIRxCoefsScale").innerText;
    var object                  = document.getElementById("IIRxCoefsScale");
    object.selectedIndex        = Math.ceil(Math.log(IIRxCoefsScaleType));   
    
    // IIRx Filter Coeficient scale calculation - selector initialization
    var DiscreteMethod          = parent.document.getElementById("DiscMethod").innerText;
    var objectDM                = document.getElementById("DiscMethod");
    objectDM.selectedIndex      = conversionDiscMethod(DiscreteMethod);
        
    // Tables of all motors in MCAT Tab Manager are not displayed
    document.getElementById('tab11line').style.display = '';
    document.getElementById('tab12line').style.display = '';
    document.getElementById('tab13line').style.display = ''; 
    document.getElementById('tab21line').style.display = 'none';
    document.getElementById('tab22line').style.display = 'none';
    document.getElementById('tab23line').style.display = 'none';  
    document.getElementById('tab31line').style.display = 'none';
    document.getElementById('tab32line').style.display = 'none';
    document.getElementById('tab33line').style.display = 'none';          
    
    for(var i=0;i<3;i++){
      // initialization of group pop-up menus in "Motor Types" fieldset    
      var object = document.getElementById("M"+[i+1]+"type");
      object.selectedIndex = conversionMotorType(parent.document.getElementById("M"+[i+1]+"type").innerText);

      var object1 = document.getElementById("M"+[i+1]+"_FFw_CL");       
      object1.selectedIndex = conversionFFwCLType(parent.document.getElementById("M"+[i+1]+"_FFw_CL").innerText);

      var object2 = document.getElementById("M"+[i+1]+"_FFw_SL");       
      object2.selectedIndex = conversionFFwSLType(parent.document.getElementById("M"+[i+1]+"_FFw_SL").innerText);

      var object3 = document.getElementById("M"+[i+1]+"_WFilt");       
      object3.selectedIndex = conversionWFilterType(parent.document.getElementById("M"+[i+1]+"_WFilt").innerText);
      
            
       // initialization of CheckBoxes in "Number of Motor" fieldset
      if (i == MotorsNumber_No){  
            MotorsNo.checkType[i].checked = true;
            
            for(var j=0;j<(i+1);j++){
              var object_j = document.getElementById("M"+[j+1]+"type");
              
              object_j.disabled = false;
              object_j.selectedIndex = conversionMotorType(parent.document.getElementById("M"+[j+1]+"type").innerText);
              
              // if first motor selected, bg color set
              if(j==0) document.getElementById("M"+[j+1]+"type").style.background =  "rgb(80,87,94)";
              // if second motor selected, bg color set
              if(j==1) document.getElementById("M"+[j+1]+"type").style.background =  "rgb(40,113,138)";
              // if third motor selected, bg color set
              if(j==2) document.getElementById("M"+[j+1]+"type").style.background =  "rgb(7,120,24)";
            }}  
      else{
            MotorsNo.checkType[i].checked = false;
            
            object.disabled           = true;
            object.style.background   = "rgb(164,172,178)";
            object.selectedIndex      = "none";
            }
                    
      // initialization of CheckBoxes in "Data Type Representation" fieldset                                  
      if (i == ArithmeticType_No) Arithmetic.checkType[i].checked = true;
      else                        Arithmetic.checkType[i].checked = false;
      


      // Control loop settings - buttons for motor selection are not displayed      
      document.getElementById('M'+[i+1]+'_ClButton').style.display = "none";
      
      if(i<(MotorsNumber_No+1)){
       // Control loop settings - buttons for motor selection are displayed        
       document.getElementById('M'+[i+1]+'_ClButton').style.display = "";
      }        
      

      
      // initialization of CheckBoxes in "Control Loop Settings" fieldset
       if (i<2){
        if (i == M1_CcontrollerType_No)     M1_Ccontroller.checkType[i].checked = true;
        else                                M1_Ccontroller.checkType[i].checked = false;
      
        if (i == M1_ScontrollerType_No)     M1_Scontroller.checkType[i].checked = true;
        else                                M1_Scontroller.checkType[i].checked = false;        

        if (i == M1_TOcontrollerType_No)    M1_TOcontroller.checkType[i].checked = true;
        else                                M1_TOcontroller.checkType[i].checked = false;
        
        if (i == M1_AlignmentType_No)       M1_Alignment.checkType[i].checked = true;
        else                                M1_Alignment.checkType[i].checked = false;                 

        if (i == M2_CcontrollerType_No)     M2_Ccontroller.checkType[i].checked = true;
        else                                M2_Ccontroller.checkType[i].checked = false;
      
        if (i == M2_ScontrollerType_No)     M2_Scontroller.checkType[i].checked = true;
        else                                M2_Scontroller.checkType[i].checked = false;        

        if (i == M2_TOcontrollerType_No)    M2_TOcontroller.checkType[i].checked = true;
        else                                M2_TOcontroller.checkType[i].checked = false; 
        
        if (i == M2_AlignmentType_No)       M2_Alignment.checkType[i].checked = true;
        else                                M2_Alignment.checkType[i].checked = false;               

        if (i == M3_CcontrollerType_No)     M3_Ccontroller.checkType[i].checked = true;
        else                                M3_Ccontroller.checkType[i].checked = false;
      
        if (i == M3_ScontrollerType_No)     M3_Scontroller.checkType[i].checked = true;
        else                                M3_Scontroller.checkType[i].checked = false;        

        if (i == M3_TOcontrollerType_No)    M3_TOcontroller.checkType[i].checked = true;
        else                                M3_TOcontroller.checkType[i].checked = false; 
        
        if (i == M3_AlignmentType_No)       M3_Alignment.checkType[i].checked = true;
        else                                M3_Alignment.checkType[i].checked = false;               
        }
      }

    /// 
    var valDec = new Array(4);
    valDec[1] = 0;
    valDec[2] = 0;
    valDec[3] = 0;
        
    for (var j=0; j < (MotorsNumber_No+1); j++)
     {
        valDec[j+1]    = parent.document.getElementById('Tab'+[j+1]+'manager').innerHTML ;
        var liObject   = parent.document.getElementById('tabMotor'+[j+1]).getElementsByTagName('a');
        
        for(var i=0;i<10;i++)
        {
        var ChBObject  = document.getElementById("ChB"+[i]+"_"+[j+1]);
        
         if(((valDec[j+1])>>>i)&1)
          {
              ChBObject.checked = true;
              liObject[i].style.display = '';
          }
          else
          {
              ChBObject.checked = false;
              liObject[i].style.display = 'none';
          }
         }  
     } 
     
    ////// Initialization of project path - read from parent doc table
    var pathRelativeFolder =  parent.parent.document.getElementById('ProjectPath').innerHTML;
    var pathRelativeFolderOutput = "{FM_project_loc}/" + pathRelativeFolder;
  
    if (pathRelativeFolderOutput.length>100)
    {
      for (var i=0;i<60;i++)
      {
          
          if (i>50 && pathRelativeFolderOutput.charAt(i)==='/')
          {
            var fLine = pathRelativeFolderOutput.substring(0,i+1);
            var sLine = pathRelativeFolderOutput.substring(i+1);
            document.getElementById('projectPathWrite1').innerHTML = fLine;
            document.getElementById('secLineProjPath').style.display = '';
            document.getElementById('projectPathWrite2').innerHTML = sLine;         
          }
      }
    }
    else 
    {
      document.getElementById('projectPathWrite1').innerHTML = pathRelativeFolderOutput;
      document.getElementById('projectPathWrite2').innerHTML = '';
    }
}
   
/***************************************************************************//*!
* @brief    get the name of selected motor and store it in the main page table/file 
* @param    iD  - of Motor Type pop-up menu 
*           inValue - value of selected motor
* @return  None
* @remarks 
*******************************************************************************/   
function  selectMot(iD, inValue)
{
    parent.document.getElementById(iD).innerHTML = document.getElementById(iD).options[inValue].text;
    
    // update the multi-motor selector name with name of selected motor
    parent.document.getElementById(iD+'TabName').innerHTML  = parent.document.getElementById(iD).innerHTML;    

}
    
/***************************************************************************//*!
* @brief    An engine for generating of one table with the names of the TW tabs   
* @param    tableNum  - the number of the table according to number of selected motors 
*           tabArray  - an array of the tab names
*           tabNumber - the number of the tab 
* @return   string representing one table
* @remarks 
*******************************************************************************/   
function build_Motor_tab_manager(tableNum, tabArray, tabNumber)
{
    var string = '';

    string = string + "<tr id=\"tab"+[tableNum]+"1line\">";
    string = string + "<td height=\"30\" colspan=\"10\" style=\"font-family: Arial; font-size: 12px; font-weight: bold;\"><div>Motor # "+[tableNum]+"</div></td>"
    string = string + "</tr>";
    
    string = string + "<tr id=\"tab"+[tableNum]+"2line\">"; 
  
    if(tableNum===1){    
      for(var i=0;i<tabNumber;i++){
      string = string + "<td valign=\"center\" width=\"11%\" style=\"font-family: Arial; font-size: 12px; background-color: rgb(80,87,94); color:white;\">" + tabArray[i] + "</td>";}
    }
    if(tableNum===2){    
      for(var i=0;i<tabNumber;i++){
      string = string + "<td valign=\"center\" width=\"11%\" style=\"font-family: Arial; font-size: 12px; background-color: rgb(40,113,138); color:white;\">" + tabArray[i] + "</td>";}
    }
    if(tableNum===3){    
      for(var i=0;i<tabNumber;i++){
      string = string + "<td valign=\"center\" width=\"11%\" style=\"font-family: Arial; font-size: 12px; background-color: rgb(79,120,24); color:white;\">" + tabArray[i] + "</td>";}
    }
    
    string = string + "</tr>";
    string = string + "<tr id=\"tab"+[tableNum]+"3line\">";
     
    for(var i=0;i<tabNumber;i++){
    string = string + "<td valign=\"center\"  style=\"border:thin inset inherit\"><input type=\"checkbox\" id=\"ChB"+[i]+"_"+ [tableNum]+ "\" value=\""+[i+1]+"\" onclick = \"setTabOrder("+[tableNum]+")\"></td>";
    }                                                                                                                           
    string = string + "</tr>";
                            
                                                            
  	return string;
}

/***************************************************************************//*!
* @brief    Function handles the menu tabs, their order according to the "Tuning Wizard Tab manager"   
* @param    motorNumber  - the number of the motor 
*
* @return   string representing one table
* @remarks 
*******************************************************************************/   
function setTabOrder(motorNumber)
{
   var valDec = new Array(4);
   var liObject     = parent.document.getElementById('tabMotor'+[motorNumber]).getElementsByTagName('a');
   
   valDec[1] = 0;
   valDec[2] = 0;
   valDec[3] = 0;
   
   for (var i=0; i<10; i++)
   {
    liObject[i].style.display = 'none';   

      if (document.getElementById('ChB'+[i]+"_"+[motorNumber]).checked == true)
      {
        valDec[motorNumber] =  valDec[motorNumber] + Math.pow(2,(document.getElementById('ChB'+[i]+"_"+[motorNumber]).value-1));
        liObject[i].style.display = '';
        
        parent.document.getElementById('Tab'+[motorNumber]+'manager').innerHTML = valDec[motorNumber];
        
      }                      
   }
  
}

/***************************************************************************//*!
* @brief    Function removes the tab line and moves up the active window 
* @param     
* @return   
* @remarks 
*******************************************************************************/   
function setAllTabOff()
{
   var activePage = parent.document.getElementById('mainFrame');
   activePage.style.position="absolute";
   activePage.style.top="-68px";
   activePage.style.left="0px";
   activePage.style.height="560px";
 }
 
/***************************************************************************//*!
* @brief    on click event of the Done button 
* @param
* @return   None
* @remarks 
*******************************************************************************/     
function clickApply()
{
  var activePage   = parent.document.getElementById('mainFrame');
  var radioMotor1Obj = parent.document.getElementById("radioMotor1");
  var radioMotor2Obj = parent.document.getElementById("radioMotor2");
  var radioMotor3Obj = parent.document.getElementById("radioMotor3");
  
  var tabMotor1Obj = parent.document.getElementById("tabMotor1");
  var tabMotor2Obj = parent.document.getElementById("tabMotor2");
  var tabMotor3Obj = parent.document.getElementById("tabMotor3"); 
  
  var menuObj = parent.document.getElementById("menu");
  var liObject     = parent.parent.document.getElementById('tabMotor1').getElementsByTagName('a');
      
  tabMotor1Obj.style.display='block';
  tabMotor2Obj.style.display='none';  
  tabMotor3Obj.style.display='none';      
  menuObj.style.backgroundColor='rgb(53,58,63)';
      
  if(!radioMotor1Obj.checked)
    {  
    radioMotor1Obj.checked = true;
    radioMotor2Obj.checked = false; 
    radioMotor3Obj.checked = false;
    }
      
  // set the active page      
  activePage.src = 'inner_Intro_M1.html';
  liObject[0].className = "active";
  
  for (var i=1; i<10; i++)
  {
    liObject[i].className = "";
  }
  
   var activePage = parent.document.getElementById('mainFrame');
   activePage.style.position="absolute";
   activePage.style.top="0px";
   activePage.style.left="0px";
   activePage.style.height="510px";   
/* copy new path to generated .h file from setting to M1_params_....txt file*/
   parent.parent.document.getElementById('M1_ProjectPath').innerHTML = parent.parent.document.getElementById('ProjectPath').innerHTML;

}

/***************************************************************************//*!
* @brief    on click event of the Cancel button 
* @param
* @return   None
* @remarks 
*******************************************************************************/     
function clickCancel()
{
  var liObject     = parent.parent.document.getElementById('tabMotor1').getElementsByTagName('a');
  var activePage   = parent.document.getElementById('mainFrame');
  // set the active page      
  activePage.src = 'inner_Intro_M1.html';
  liObject[0].className = "active";
  
  for (var i=1; i<9; i++)
  {
    liObject[i].className = "";
  }
  
   var activePage = parent.document.getElementById('mainFrame');
   activePage.style.position="absolute";
   activePage.style.top="0px";
   activePage.style.left="0px";
   activePage.style.height="490px"; 
   
   // Reload data in inner setting tab from storing file
   paramFileReadData('Setting_');  
}

/***************************************************************************//*!
* @brief    Function generates tables according to number of motors  
* @param     
* @return   
* @remarks 
*******************************************************************************/   
function generate_table_Motor()
{
     var MotorsNumber_No = 3;
     var NumberOfTabs    = 10;
          
      var tabNames = [ 
        "Intro",
        "MID",
        "Param",
        "Current Loop",
        "Speed Loop",
        "Sensors",
        "Sensorless",
        "Control Struct",
        "Out File",
        "App Control"
      ];
      
      for(var i=0;i<(MotorsNumber_No);i++){
          document.write( build_Motor_tab_manager(Number([i+1]), tabNames,NumberOfTabs));}
}
 
/***************************************************************************//*!
* @brief    Function check how many checkBoxes is selected, and store the 2^n value
*           to the main table in main page  
* @param     
* @return   
* @remarks 
*******************************************************************************/   
function PospeFbckClick()
{
    var M1_checkedBox_val = 0;
    var M2_checkedBox_val = 0;
    var M3_checkedBox_val = 0;
    
    for(var i=0;i<5;i++)
    {
      var M1_pospeCheckBox = document.getElementById('M1_pospeCheck'+[i]);
      if (M1_pospeCheckBox.checked == true)  M1_checkedBox_val = M1_checkedBox_val + Math.pow(2,i);
      
      var M2_pospeCheckBox = document.getElementById('M2_pospeCheck'+[i]);
      if (M2_pospeCheckBox.checked == true)  M2_checkedBox_val = M2_checkedBox_val + Math.pow(2,i);
      
      var M3_pospeCheckBox = document.getElementById('M3_pospeCheck'+[i]);
      if (M3_pospeCheckBox.checked == true)  M3_checkedBox_val = M3_checkedBox_val + Math.pow(2,i);            
    }    
    
    parent.document.getElementById('M1_PospeFbck').innerHTML = M1_checkedBox_val; 
    parent.document.getElementById('M2_PospeFbck').innerHTML = M2_checkedBox_val;
    parent.document.getElementById('M3_PospeFbck').innerHTML = M3_checkedBox_val;

} 

/***************************************************************************//*!
* @brief    Function check how many checkBoxes is selected, and store the 2^n value
*           to the main table in main page  
* @param     
* @return   
* @remarks 
*******************************************************************************/   
function PospeFbckInit()
{
    var M1_checkedBox_val = parent.document.getElementById('M1_PospeFbck').innerHTML;
    var M2_checkedBox_val = parent.document.getElementById('M2_PospeFbck').innerHTML;
    var M3_checkedBox_val = parent.document.getElementById('M3_PospeFbck').innerHTML;
    
    for(var i=0;i<5;i++)
    {
     document.getElementById('M1_pospeCheck'+[i]).checked = false;
     document.getElementById('M2_pospeCheck'+[i]).checked = false;
     document.getElementById('M3_pospeCheck'+[i]).checked = false;
    }
        
    for(var i=0;i<5;i++)
    {
     if ((M1_checkedBox_val>>>[i])&1)
     document.getElementById('M1_pospeCheck'+[i]).checked = true;
     
     if ((M2_checkedBox_val>>>[i])&1)
     document.getElementById('M2_pospeCheck'+[i]).checked = true;

     if ((M3_checkedBox_val>>>[i])&1)
     document.getElementById('M3_pospeCheck'+[i]).checked = true;
    }                
} 
  
/***************************************************************************//*!
* @brief    Control Loop Setting buttons managment.  
* @param    input: button ID given by selected motor     
* @return   
* @remarks 
*******************************************************************************/   
function ClButtonClick(Mbutton_id)
{
    document.getElementById('M1_ClButton').style.background = "transparent";
    document.getElementById('M2_ClButton').style.background = "transparent";
    document.getElementById('M3_ClButton').style.background = "transparent";
    document.getElementById('M1_ClButton').style.color = "black";
    document.getElementById('M2_ClButton').style.color = "black";
    document.getElementById('M3_ClButton').style.color = "black";
      
    if (Mbutton_id == 'M1_ClButton')
    {
        document.getElementById('M1_ClSetting').style.display = "";
        document.getElementById('M2_ClSetting').style.display = "none";
        document.getElementById('M3_ClSetting').style.display = "none";
        
        document.getElementById('M1_ClButton').style.background = "rgb(80,87,94)";
        document.getElementById('M1_ClButton').style.color = "white";
        
        document.getElementById('tab11line').style.display = '';
        document.getElementById('tab12line').style.display = '';
        document.getElementById('tab13line').style.display = ''; 
          
        document.getElementById('tab21line').style.display = 'none';
        document.getElementById('tab22line').style.display = 'none';
        document.getElementById('tab23line').style.display = 'none';
        
        document.getElementById('tab31line').style.display = 'none';
        document.getElementById('tab32line').style.display = 'none';
        document.getElementById('tab33line').style.display = 'none';
    }
    
    if (Mbutton_id == 'M2_ClButton')
    {
        document.getElementById('M1_ClSetting').style.display = "none";
        document.getElementById('M2_ClSetting').style.display = "";
        document.getElementById('M3_ClSetting').style.display = "none";
        
        document.getElementById('M2_ClButton').style.background = "rgb(40,113,138)";
        document.getElementById('M2_ClButton').style.color = "white";
        
        document.getElementById('tab11line').style.display = 'none';
        document.getElementById('tab12line').style.display = 'none';
        document.getElementById('tab13line').style.display = 'none'; 
          
        document.getElementById('tab21line').style.display = '';
        document.getElementById('tab22line').style.display = '';
        document.getElementById('tab23line').style.display = '';
        
        document.getElementById('tab31line').style.display = 'none';
        document.getElementById('tab32line').style.display = 'none';
        document.getElementById('tab33line').style.display = 'none';        
    }
    
    if (Mbutton_id == 'M3_ClButton')
    {
        document.getElementById('M1_ClSetting').style.display = "none";
        document.getElementById('M2_ClSetting').style.display = "none";
        document.getElementById('M3_ClSetting').style.display = "";
        
        document.getElementById('M3_ClButton').style.background = "rgb(79,120,24)";
        document.getElementById('M3_ClButton').style.color = "white";
        
        document.getElementById('tab11line').style.display = 'none';
        document.getElementById('tab12line').style.display = 'none';
        document.getElementById('tab13line').style.display = 'none'; 
          
        document.getElementById('tab21line').style.display = 'none';
        document.getElementById('tab22line').style.display = 'none';
        document.getElementById('tab23line').style.display = 'none';
        
        document.getElementById('tab31line').style.display = '';
        document.getElementById('tab32line').style.display = '';
        document.getElementById('tab33line').style.display = '';              
     }
  
}
 /***************************************************************************//*!
* @brief    Function check how many checkBoxes are selected, and store the 2^n value
*           to the main table in main page  
* @param     
* @return   
* @remarks 
*******************************************************************************/ 
function selectCFF(iD, inValue)
{
  parent.document.getElementById(iD).innerHTML = document.getElementById(iD).options[inValue].text;
}     

function selectSFF(iD, inValue)
{
  parent.document.getElementById(iD).innerHTML = document.getElementById(iD).options[inValue].text;
}

function selectWF(iD, inValue)
{
  parent.document.getElementById(iD).innerHTML = document.getElementById(iD).options[inValue].text;
}

 /***************************************************************************//*!
* @brief    Function check how many checkBoxes are selected, and store the 2^n value
*           to the main table in main page  
* @param     
* @return   
* @remarks 
*******************************************************************************/ 
function browseDirectory()
{
   var object  = document.getElementById('projectPathFrameSelect');
         
   object.style.top      = 12;
   object.style.left     = 150;         
         
   object.style.display  = "block";
   object.src            = 'form_PathSelector.html';
   
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
