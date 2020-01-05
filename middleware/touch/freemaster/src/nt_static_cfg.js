/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
var data = {};
function nt_static_cfg_init()
{
  //var paramFile = pcm.LocalFileOpen("src/news.xml","r");
  var paramFile = pcm.LocalFileOpen("src/test1.xml","r");
  // read file content and store it in string
  var charNummber = pcm.LocalFileReadString(paramFile);
  var fileStatus = pcm.LocalFileClose(paramFile);
  var inputFileString = "";
  if(charNummber>0)
    inputFileString = pcm.LastLocalFile_string; //.split("\n");
  data = XmlStrToDataObj(inputFileString);
}


function GenerateVars()
{
	var label = "nt_system";
    romSystemName = "nt_kernel_data.rom";
    if(!pcm_read_ptr(romSystemName, SIZEOF_PTR))
        return false;
    var nt_system_rom_ptr = pcm.LastVariable_vValue;
    
    // map nt_symbol type to the obtained pointer
    var tssvar = "_nt_system";
    if(!pcm_define_symbol(tssvar, nt_system_rom_ptr, "nt_system"))
        return false;
/*    if(!pcm_read_var("_nt_system.time_period", 1))
    	return false;
    console.log(pcm.LastVariable_vValue);
    debug_print("_nt_system.time_period:0x"+pcm.LastVariable_vValue, true);*/
    
    // define all config items in TSI modules type as tsi_config_t*
    for(i=0;i<NTModule.All.length;i++)
    {
        if(NTModule.All[i].GetType() == "tsi")
        {
          if(pcm_read_ptr("nt_kernel_data.modules["+i+"]->rom->config"))
          {
        	  debug_print("nt_kernel_data.modules["+i+"]->rom->config:0x"+pcm.LastVariable_vValue.toString(16), true);
            var tssvar = "_nt_module_data";
            //assign config as 
            if(!pcm_define_symbol("nt_kernel_data.modules["+i+"]->rom->config", pcm.LastVariable_vValue, "tsi_config_t*"))
                return false;

//            if(pcm_read_ptr("nt_kernel_data.modules[0]->rom->config->thresl"))
//            	debug_print("nt_kernel_data.modules[0]->rom->config->thresl:0x"+pcm.LastVariable_vValue.toString(16), true);
          }
        }else if(NTModule.All[i].GetType() == "cs")
        {
          if(pcm_read_ptr("nt_kernel_data.modules["+i+"]->rom->config"))
          {
        	  debug_print("nt_kernel_data.modules["+i+"]->rom->config:0x"+pcm.LastVariable_vValue.toString(16), true);
            var tssvar = "_nt_module_data";
            //assign config as 
            if(!pcm_define_symbol("nt_kernel_data.modules["+i+"]->rom->config", pcm.LastVariable_vValue, "cs_config_t*"))
                return false;

//            if(pcm_read_ptr("nt_kernel_data.modules[0]->rom->config->thresl"))
//            	debug_print("nt_kernel_data.modules[0]->rom->config->thresl:0x"+pcm.LastVariable_vValue.toString(16), true);
          }
        }
    	
    }

    var i=0;
    //get type of controls
/*    for(i=0;i<NTControl.All.length;i++)
    {
        var control = NTControl.All[i];
        //get item from controls
        if(pcm_read_var(control._symbol + "->rom->control_params."+control._type+".groups_size", 1))
        {
        	debug_print(control._symbol + "->rom->control_params."+control._type+".groups_size:0x"+pcm.LastVariable_vValue.toString(16), true);
        }
    }*/

    //read electrode
/*    for(i=0;i<NTElectrode.All.length;i++)
    {
        var electrode = NTElectrode.All[i];

        //get item from controls
        if(pcm_read_var(electrode._symbol + "->rom->multiplier", 1))
        	console.log(pcm.LastVariable_vValue);
        if(pcm_read_var(electrode._symbol + "->rom->keydetector_params."+electrode._keydettype+".entry_event_cnt", 1))
            console.log(pcm.LastVariable_vValue);
    }*/
    //Todo: define paths to !!buttons!!
    //HW config
}

function GetModeFromWeb(formid, rbid)
{
	var i = -1;
	var formhtmlobj1 = document.getElementById(formid);
	if(formhtmlobj1)
  {
  	//get basic expert mode from both HW configurations
    var opt1 = formhtmlobj1.elements[rbid];
    if(opt1)
    {
      for(i=0; i<opt1.length; i++)
      {
        if(opt1[i].checked)
          break;
      }
    }
  }
  return i;
}

function onchangemode(obj, id, val)
{
  var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);

	var selected1 = GetModeFromWeb(path+"-1_form", path+"-1rb");
  var selected2 = GetModeFromWeb(path+"-2_form", path+"-2rb");
  var opt1 = 0;
  var formhtmlobj1 = document.getElementById(path+"-1_form");
  if(formhtmlobj1)
    opt1 = formhtmlobj1.elements[path+"-1rb"];
  var opt2 = 0;
  var formhtmlobj2 = document.getElementById(path+"-2_form");
  if(formhtmlobj2)
    opt2 = formhtmlobj2.elements[path+"-2rb"];

  //when equel do nothing
  if(selected2 == selected1)
    return;
  //sync second
  if((rndid == "1") && opt2) 
  {
    for(i=0; i<opt2.length; i++)
    {
      if(selected1 == i)
        opt2[i].checked = true;
      else
        opt2[i].checked = false;
    }
  }
  //sync first
  if((rndid == "2") && opt1)
  {
    for(i=0; i<opt1.length; i++)
    {
      if(selected2 == i)
        opt1[i].checked = true;
      else
        opt1[i].checked = false;
    }
  }
  
  
  //list all xml, disable/enable ids
  //todo refresh mode on add new electrode/new controls
  var mode = val;
  var label = "nt_system";
  SetModeToWeb(data[label], label, label, data[label]["rndid"], mode, 'mode');
  label = "nt_module";
  SetModeToWeb(data[label], label, label, data[label]["rndid"], mode, 'mode');
  label = "nt_control";
  SetModeToWeb(data[label], label, label, data[label]["rndid"], mode, 'mode');
  label = "nt_electrode";
  SetModeToWeb(data[label], label, label, data[label]["rndid"], mode, 'mode');
  if(Number(mode))
    EnableAllElCheckboxes("nt_control.electrodes");
  UpdateSafetyVisibility();
}

function UpdateSafetyVisibility()
{
  var selected1 = GetModeFromWeb("modeid-1_form", "modeid-1rb");
  var mode = selected1;

  var tsi_type = true;
  if(GetValFromPage(data["nt_module"]["rndid"], "nt_module.interface") != "&nt_module_tsi_interface")
    tsi_type = false;

  var safetyhtmllabelobj = document.getElementById("nt_module.safety_params.gpio-"+data["nt_module"]["rndid"]);
  if(safetyhtmllabelobj)
    safetyhtmllabelobj.style.display = !(tsi_type && mode) ? 'None' : 'block';  
  var safetyhtmlobj = document.getElementById("nt_module_safety_gpio_params.tablestart");
  if(safetyhtmlobj)
    safetyhtmlobj.style.display = !(tsi_type && mode) ? 'None' : 'block';
  safetyhtmllabelobj = document.getElementById("nt_system.safety_period_multiple-"+data["nt_system"]["rndid"]+"tr");
  if(safetyhtmllabelobj)
    safetyhtmllabelobj.style.display = !(tsi_type && mode) ? 'None' : 'block';  
  safetyhtmllabelobj = document.getElementById("nt_system.safety_crc_hw-"+data["nt_system"]["rndid"]+"tr");
  if(safetyhtmllabelobj)
    safetyhtmllabelobj.style.display = !(tsi_type && mode) ? 'None' : 'block';    
    
  var label = "nt_electrode";
  SetModeToWeb(data[label], label, label, data[label]["rndid"], !(tsi_type && mode) ? 0 : 1, 'showtsi');    
}

function PutStaricCfgToPage()
{
    //put line with name of config file
	var id = "cfgfile";
    var obj = {};
    obj['nodepath'] = id;
    obj['rndid'] = "";
    var stricon = Mustache.to_html(templates["sticon"], obj);
    obj = {};
    obj['nodepath'] = "modeid";
    obj['rndid'] = "";
    obj['onchange'] = "onchangemode";
    var modestr = Mustache.to_html(templates["rb_basic_exp"], obj);
    document.write(modestr);
    document.write("<table>");
    document.write("<tr><td>touch tool configuration (..\\frdm_ke15z.xml):</td><td><input type=\"text\" name=\"fname\" id=\""+id+"-\"  value=\"..\\frdm_ke15z.xml\"></td><td> <input type=\"button\" onclick=\"LoadCfgData('"+id+"-')\" value=\"Load\"><input type=\"button\" onclick=\"SaveCfgData('"+id+"-')\" value=\"Save\">"+stricon+"</td></tr>");

    obj = document.getElementById(id);
    if((document.cookie)&&(obj))
    	obj.value = document.cookie;

    
    var label = "paths";
    data[label]["rndid"] = GetRandObjId();
    var paths = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"]);
    document.write(paths);
    document.write("</table>");
    id = "boardcfg";
    obj = {};
    obj['nodepath'] = id;
    obj['rndid'] = "";
    stricon = Mustache.to_html(templates["sticon"], obj)
    	
    document.write( "<input type=\"button\" onclick=\"LoadCfgBoard('"+id+"-')\" title=\"The actual configuration will be revriten by the configuration from connected Board (requires enabled FreeMASTER communication)\" value=\"Read Configuration from Board\">"+stricon+"<br>");

         
    //parseboardcfgupdatepins("paths.boardfile"+"-"+data['paths']['boardfile']["rndid"]);
    var label = "nt_system";
    data[label]["rndid"] = GetRandObjId();
    var h = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"]);
    document.write(h);
    //console.log(h);

    label = "nt_module";
    data[label]["rndid"] = GetRandObjId();
    var nt_module = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"]);
    //var nt_module = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"], "nt_module");
    label = "tsi_config_t";
//    nt_module = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"], "TSI_analog_self.png");
//    nt_module = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"], "TSI_analog_mult.png");

    label = "nt_control";
    data[label]["rndid"] = GetRandObjId();
    label = "nt_controls";
    data[label]["rndid"] = GetRandObjId();
    var h2 = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"]);
    document.write( h2 );
    console.log( h2 );
    label = "nt_electrodes";
    data[label]["rndid"] = GetRandObjId();
    var h3 = GetWebContentFromObj2(data[label], label, "template", data[label]["rndid"]);
    document.write( h3 );
    document.write( nt_module );
}

function LoadCfgBoard(id)
{
    var board_detected = pcm.IsBoardDetected();

    if(board_detected != false)
    {
    	
    	//delete all electrodes
    	var label = "nt_electrodes";
    	/*var obj = GetDescrObj("nt_electrode");
    	for(var index in obj['links'])
    	{
    		var rndid = 0;
    		for(rndid in obj['links'][index])
    			break;
    		incrementerdel("nt_electrodes.tablediv", rndid, 0);    		
    	}*/
    	var obj = GetDescrObj(label+".tablediv");
    	DeleteDynSubStruct(obj, label, 2); //argument 2 resets the index to 0
    	incrementerSetCnt("nt_electrodes.tablediv", obj['rndid'], 0);
    	//add all electrodes
    	for(var i in NTElectrode.All)
    		incrementerinc("nt_electrodes.control_count",data[label]["rndid"], 1);
    	
    	//delete controls
    	label = "nt_controls";
    	obj = GetDescrObj(label+".tablediv");
    	DeleteDynSubStruct(obj, label, 2); //argument 2 resets the index to 0
    	incrementerSetCnt("nt_controls.tablediv", obj['rndid'], 0);
    	//add all default controls
    	for(var i in NTControl.All)
    		incrementerinc("nt_controls.control_count",data[label]["rndid"], 1);
    	SetStatusLineRaw(id, STATUS_OK, "Data from board was loaded.");
    	
    	//updatetype of controls
    	var basepath = "nt_control";
    	var pathinterface = basepath+".interface";
    	obj = GetDescrObj(pathinterface);
    	var objbase = GetDescrObj(basepath);
    	for(var i in NTControl.All)
    	{
    		rndid = GetLinkId(objbase['links'][i]);
    		var objstrname = GetDescrObj(basepath+".sructname");
    		obj['lasttext'] = GetHtmlObjVal(basepath+".sructname-"+rndid, objstrname['template']).slice(0, -2); //read the controls name and remove last 2 chars "_1"
    		SetHtmlObjVal(pathinterface+"-"+rndid, obj['template'], "&nt_control_"+NTControl.All[i].GetType()+"_interface");
    		enumonchange(id, pathinterface+"-"+rndid);
    	}
    	
    	//updatetype of electrodes
    	basepath = "nt_electrode";
    	pathinterface = basepath+".keydetector_interface";
    	obj = GetDescrObj(pathinterface);
    	objbase = GetDescrObj(basepath);
    	for(var i in NTElectrode.All)
    	{
    		rndid = GetLinkId(objbase['links'][i]);
    		var objstrname = GetDescrObj(basepath+".sructname");
    		SetHtmlObjVal(pathinterface+"-"+rndid, obj['template'], "&nt_keydetector_"+NTElectrode.All[i].GetKeydetType()+"_interface");
    		enumonchange(id, pathinterface+"-"+rndid);
    	}
    	//updatetype of shielding electrodes
    	var electrode_basepath = "nt_electrode";
    	var electrodebaseobj = GetDescrObj(electrode_basepath);
    	for(var el_i in NTElectrode.All)
    	{
        	//read pointer for shielding and check with addr of electrode and select the right el/index in enum
    		var elid = GetLinkId(electrodebaseobj['links'][el_i]);
    		var out = GetVarStr(0, "nt_electrode.shielding_electrode-"+elid);
    		var strname = out.name;
    		if(!pcm_get_symbol_info(strname))
    		{
    			var msg = "Symbol "+strname+" not found."+pcm.LastRetMsg;
    			debug_print(msg, true);
    			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
    			return false;
    		}

    		if(!pcm_read_var(strname, pcm.LastSymbolInfo_size))
    		{
    			var msg = "Failed read "+strname + ", check connection. Reported error:"+pcm.LastRetMsg;
    			debug_print(msg, true);
    			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
    			return false;
    		}
    		el_addr = pcm.LastVariable_vValue;
    		if(el_addr == 0)
    			continue;
    		for(var el_check_i in NTElectrode.All)
    		{
    			if(NTElectrode.All[el_check_i]._romaddr == el_addr)
    			{
    				var elenumindex = Number(el_check_i);
    				if(el_check_i<el_i)
    					elenumindex = Number(el_check_i)+Number(1);
    				SetHtmlObjNum("nt_electrode.shielding_electrode-"+elid, "tableenum", elenumindex);
    				break;
    			}
    		}
    	}
    	
    	
    	//update checkboxes/electrodes in controls
    	var control_basepath = "nt_control";
    	var controlbaseobj = GetDescrObj(control_basepath);
    	var electrode_basepath = "nt_electrode";
    	var electrodebaseobj = GetDescrObj(electrode_basepath);
    	
    	for(var ctrl_i in NTControl.All)
    	{
    		var ctrl_els_arr = NTControl.All[ctrl_i]._electrodes;
    		//for(elindex in electrodebaseobj)
    		for(var el_i in NTElectrode.All)
    		{
    			for(var control_el_i in ctrl_els_arr)
    			{
    				if(NTElectrode.All[el_i]._addr == ctrl_els_arr[control_el_i]._addr)
        			{
        				var ctrlid = GetLinkId(controlbaseobj['links'][ctrl_i]);
        				var elid = GetLinkId(electrodebaseobj['links'][el_i]);
        				SetHtmlObjVal(ctrlid+"_"+elid, "checkboxitem", 1);
        				checkboxitemonchange(1,"nt_control.electrodes-"+ctrlid+"_"+elid);
        			}
    			}
    		}
    	}
      //update rest of items
    	var label = "nt_system";
    	ReadDatafromBoardToWeb(data[label], label, label, data[label]["rndid"]);
    	label = "nt_module";
    	ReadDatafromBoardToWeb(data[label], label, label, data[label]["rndid"]);
    	label = "nt_control";
    	ReadDatafromBoardToWeb(data[label], label, label, data[label]["rndid"]);
    	label = "nt_electrode";
    	ReadDatafromBoardToWeb(data[label], label, label, data[label]["rndid"]);

    }
    else
    {
    	SetStatusLineRaw(id, STATUS_ERROR, "Board not connected.")
    }
}

function LoadCfgData(id)
{
    var str = "";
    //load file
    var filename = "nt_cfg.xml";
    var obj = document.getElementById(id);
    if(obj)
        filename = obj.value;
    var paramFile = pcm.LocalFileOpen("src/"+filename,"r");
    if(paramFile == 0)
    {
    	SetStatusLineRaw(id, STATUS_ERROR, "Config file "+filename+" was not found");
    	return;
    }
    // read file content and store it in string
    var charNummber = pcm.LocalFileReadString(paramFile);
    var fileStatus = pcm.LocalFileClose(paramFile);
    var fileString = "";
    if(charNummber>0)
        fileString = pcm.LastLocalFile_string;

    //json to dict array
    var cfgdata = JSON.parse(fileString);
    //set data to web
    var label = "paths";
    SetDataToWeb2(data[label], cfgdata[label][0], label, label, data[label]['rndid']);
    parseboardcfgupdatepins("paths.boardfile-"+data[label]['rndid']);
    label = "nt_system";
    SetDataToWeb2(data[label], cfgdata[label][0], label, label, data[label]['rndid']);
    label = "nt_module";
    SetDataToWeb2(data[label], cfgdata[label][0], label, label, data[label]['rndid']);
    label = "nt_controls";
    SetDataToWeb2(data[label], cfgdata[label][0], label, label, data[label]['rndid']);
    label = "nt_electrodes";
    SetDataToWeb2(data[label], cfgdata[label][0], label, label, data[label]['rndid']);
    label = "nt_electrodes";
    SetDataToWeb2(data[label], cfgdata[label][0], label, label, data[label]['rndid']);
    label = "nt_electrode.share_keydet_cfg";
    SyncOnChange(data[label], label)
    SetStatusLineRaw(id, STATUS_OK, "Data from config file "+filename+" was loaded.");
}

function SaveCfgData(id)
{
    var cfgdata = {}; 
    //convert to dict array
    var label = "nt_system";
    cfgdata[label] = GetDataFromWeb2(data[label], label, label, data[label]['rndid']);
    label = "nt_module";
    cfgdata[label] = GetDataFromWeb2(data[label], label, label, data[label]['rndid']);
    label = "nt_controls";
    cfgdata[label] = GetDataFromWeb2(data[label], label, label, data[label]['rndid']);
    label = "nt_electrodes";
    cfgdata[label] = GetDataFromWeb2(data[label], label, label, data[label]['rndid']);

    label = "paths";
    cfgdata[label] = GetDataFromWeb2(data[label], label, label, data[label]['rndid']);

    // convert to json
    var str = JSON.stringify(cfgdata);
    console.log(str);
    var filename = "nt_cfg.xml";
    var obj = document.getElementById(id);
    if(obj)
        filename = obj.value;

    //save to file
    
    var cfgFile = pcm.LocalFileOpen("src/"+filename,"w");
    var charNummber = pcm.LocalFileWriteString (cfgFile, str);
    var fileStatus = pcm.LocalFileClose(cfgFile);
    document.cookie = filename;
    SetStatusLineRaw(id, STATUS_OK, "Data to config file "+filename+" was saved.");
}

function GenerateOut(id)
{
  var str = "/*\n  * Copyright 2013 - 2016, Freescale Semiconductor, Inc. \n  * Copyright 2016-2019 NXP \n  * All rights reserved.\n * \n  * SPDX-License-Identifier: BSD-3-Clause \n */ \n /* \n Static configuration c file for the NXP Touch Library \n generated by the NXP Touch GUI Tool\n*/\n";
  var str = str + "#include \"nt_setup.h\"\n#include \"board.h\"\n\n"
  
  var label;
  label = "nt_keydetector_afid";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_keydetector_safa";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_keydetector_usafa";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_electrode";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "tsi_config_t";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "cs_config_t";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  if(GetValFromPage(data["nt_module"]["rndid"], "nt_module.interface") == "&nt_module_tsi_interface")
  {
    label = "nt_tsi_recalib_config";
    str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
    if(GetModeFromWeb("modeid-1_form", "modeid-1rb"))
    {
      str = str + "extern struct nt_module_gpio_user_interface gpio_interface;\n";
      label = "nt_module_safety_gpio_params";
      str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);    
    }
  }
    
  label = "nt_control_el_arr";
  data[label]["rndid"] = 4;
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_control_arotary";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_control_aslider";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_control_keypad";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_control_proxi";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_control_matrix";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  
  label = "nt_control";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  
  label = "nt_module_el_arr";
  data[label]["rndid"] = 3;
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_module";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_control_arr";
  data[label]["rndid"] = 1;
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_module_arr";
  data[label]["rndid"] = 2;
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  label = "nt_system";
  str = str + GenFileContent2(data[label], label, label, data[label]["rndid"]);
  
  str = str + "void nt_enable(void)\n{\n";
  label = "nt_electrode";
  str = str + GenEnFunctContent(data[label], label, label, data[label]["rndid"]);
  label = "nt_control";
  str = str + GenEnFunctContent(data[label], label, label, data[label]["rndid"]);
  str = str + "}\n";
  
  //generate template callbacks
  str = str + "\n//Following functions are generate, copy them to your application and implement there behaviour of events\n"
  str = str + callbacktemplates['System'];
  label = "nt_control";
  str = str + GenTemplateFunctContent(data[label], label, label, data[label]["rndid"]);
  
  //find template to get ID, the list
  
  console.log(str);
  var file = document.getElementById(id);
  var filename="";
  if(file)
    filename = file.value;
  
  //save to c file
  var genFile = pcm.LocalFileOpen("src/"+filename+".c","w");
  var charNummber = pcm.LocalFileWriteString (genFile, str);
  var fileStatus = pcm.LocalFileClose(genFile);
  if(document.getElementById("gencfgid"))
  {
     var res = str.split("\n");
     var finalstr = "<p class=\"debug-log\">" + res.join("</p><p class=\"debug-log\">") + "</p>";
     gencfgid.innerHTML = finalstr;
     
  }
  
  var strh = "/*\n  * Copyright 2013 - 2016, Freescale Semiconductor, Inc. \n  * Copyright 2016-2019 NXP \n  * All rights reserved.\n * \n  * SPDX-License-Identifier: BSD-3-Clause \n */ \n /* \n Static configuration header file for the NXP Touch Library \n generated by the NXP Touch GUI Tool\n*/\n";
  strh = strh + "#ifndef NT_SETUP_H\n";
  strh = strh + "#define NT_SETUP_H\n\n";
  strh = strh + "#include \"nt.h\"\n";

	  
  var labelh;
  labelh = "nt_electrode";
  strh = strh + GenHFileContent(data[labelh], labelh, labelh, data[labelh]["rndid"]);
  labelh = "nt_control";
  strh = strh + GenHFileContent(data[labelh], labelh, labelh, data[labelh]["rndid"]);
  labelh = "nt_module";
  strh = strh + GenHFileContent(data[labelh], labelh, labelh, data[labelh]["rndid"]);
  labelh = "nt_system";
  strh = strh + GenHFileContent(data[labelh], labelh, labelh, data[labelh]["rndid"]);
  strh = strh + "void nt_enable(void);\n";

  var labelm = "nt_control";
  strh = strh + GetMacroConfigInit(data[labelm], labelm, labelm, data[labelm]["rndid"], data["nt_control_macro_arr"]);
  strh = strh + "\n\n#endif\n";
  //save to c file
  var genFileh = pcm.LocalFileOpen("src/"+filename+".h","w");
  var charNummberh = pcm.LocalFileWriteString (genFileh, strh);
  var fileStatush = pcm.LocalFileClose(genFileh);
  SetStatusLineRaw(id, STATUS_OK, "Data to static file "+filename+" was saved.");
}

function ChangePinInput(id)
{
	parseboardcfgupdatepins(id);
	SyncNodeContentToWeb("nt_electrode.pin_input");
}

function parseboardcfgupdatepins(id)
{
	var file = document.getElementById(id + "boardfile");
	var filename="";
	if(file)
	    filename = file.value;

    var paramFile = pcm.LocalFileOpen("src/"+filename,"r");
    if(paramFile == 0)
    {
    	SetStatusLineRaw(id, STATUS_ERROR, "Config file "+filename+" was not found");
    	return;
    }
    // read file content and store it in string
    var charNummber = pcm.LocalFileReadString(paramFile);
    var fileStatus = pcm.LocalFileClose(paramFile);
    var fileString = "";
    if(charNummber>0)
        fileString = pcm.LastLocalFile_string;
    var filelines = fileString.split("\n");
    //remove cr, lf
    for(line in filelines)
    	filelines[line] = filelines[line].replace(/[n\r]+/g, '');
    
    //get interesting lines (with TF_TSI_MUTUAL_CAP or TF_TSI_SELF_CAP)
    var electrodelines = [];
    
    for(line in filelines)
    {
    	if(filelines[line].indexOf("TF_TSI_MUTUAL_CAP_") > 1)
    		electrodelines.push(filelines[line]);
        if(filelines[line].indexOf("TF_TSI_SELF_CAP_") > 1)
            electrodelines.push(filelines[line]);

    }
    
    //get list of electrodes
    var electrodes = [];
    for(electrodedef in electrodelines)
    {
    	var ellinearr = electrodelines[electrodedef].split(" ")
    	electrodes.push(ellinearr[1])
    }
    
    //update data dict array by the pin names in electrodes array
    PutPinDefinitionsToElectrode("nt_electrode.pin_input", electrodes);
    SetStatusLineRaw(id, STATUS_OK, "Pin configuration file "+filename+" was loaded.");
}