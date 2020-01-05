/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTElectrode class */
  
function NTElectrode(module, symbol, addr, moduleType)
{
    this._id = get_unique_id();
    this._valid = false;
    this._module = module;
    this._symbol = symbol;
    this._addr = addr;
    this._romaddr = null;
    this._moduleType = moduleType;
    this._keydettype = null;
    this._keydetsymbol = null;
    this._shielding_symbol = null;
    this._shielding_addr = null;
    this._name = "";
    this._ramstruct = "nt_electrode_data";
    this._romstruct = "nt_electrode";
    this._gui_id1 = get_unique_id();
    this._gui_id2 = get_unique_id();
    this._statusCnt = 1;
    this._electrode_colors_table = ["color-ball bg-green", "color-ball bg-blue", "color-ball bg-lblue", "color-ball bg-violet", "color-ball bg-pink", "color-ball bg-yellow", "color-ball bg-orange"];
    var el_cnt = 1;
    if(NTElectrode.All)
      el_cnt = NTElectrode.All.length + Number(1);
    this._electrode_color = this._electrode_colors_table[el_cnt%this._electrode_colors_table.length];

    if(this._moduleType)
        this._symbol = symbol;

    // generic structure at symbol
    if(!pcm_define_symbol(this._symbol, this._addr, this._ramstruct+"*"))
        return;
    // generic ROM address 
    if(!pcm_read_ptr(this._symbol + "->rom"))
        return;
    this._romaddr = pcm.LastVariable_vValue;
    if(!pcm_define_symbol(this._symbol + "->rom", this._romaddr, this._romstruct+"*"))
        return;

    // try to determine keydetector type dynamically
    if(pcm_read_ptr(symbol + "->rom->keydetector_interface"))
    {
        var itf = pcm.LastVariable_vValue;
        // by comparing interface pointers with known ones
        for(var mt in KEYDETECTOR_TYPES)
        {
            if(pcm_get_symbol_info("nt_keydetector_" + KEYDETECTOR_TYPES[mt] + "_interface"))
            {
                if(itf == pcm.LastSymbolInfo_addr)
                {
                    debug_print("Keydetector at " + this._symbol + " is " + KEYDETECTOR_TYPES[mt]);
                    this._keydettype = KEYDETECTOR_TYPES[mt];

                    var keydetAddr = null;
                    if(pcm_read_ptr(symbol + "->keydetector_data"))
                      keydetAddr = pcm.LastVariable_vValue;

                    this._keydetsymbol = this._symbol + "->keydetector_data";
                    if(!pcm_define_symbol(this._keydetsymbol, keydetAddr, "nt_keydetector_"+KEYDETECTOR_TYPES[mt]+"_data*"))
                      return;                  

                    // define symbol for control parameters
                    this._keydetstatsymbol = this._symbol + "->rom->keydetector_params";
                    if(!pcm_get_symbol_info(this._keydetstatsymbol))
                      return;
                    var electrode_params_addr = pcm.LastSymbolInfo_addr;
                    if(pcm_read_ptr(electrode_params_addr))
                    {
                      if(!pcm_define_symbol(this._keydetstatsymbol+"."+this._keydettype, pcm.LastVariable_vValue, "nt_keydetector_"+this._keydettype))
                        return;
                    }

                    break;
                }
            }
        }
        // change our class to the real one wrapping the functionality specific for the electrode type
        var classname = "NTElectrode_" + this._keydettype;
        change_class_type(this, classname);
    }

    // create the electrode name
    this._name = "electrode_" + this._keydettype + "_"+ NTElectrode.All.length;
    
    //get amount of status results
    if(pcm_get_symbol_info(symbol + "->status"))
    {
        var baseAddr = pcm.LastSymbolInfo_addr;
        var tmpSymbolSize = pcm.LastSymbolInfo_size;
        this._statusCnt = 1;
        if(pcm_get_symbol_info(symbol + "->status[1]"))
        {
          var statLen = pcm.LastSymbolInfo_addr-baseAddr;
          this._statusCnt = tmpSymbolSize/statLen;
        }
        debug_print("   ->status: " +this._statusCnt);
    }
    
    this._shielding_symbol = this._symbol + "->shielding_electrode"
    if(!pcm_read_ptr(this._shielding_symbol))
      return;
    this._shielding_addr = pcm.LastVariable_vValue;
    
    if(!pcm_define_symbol(this._shielding_symbol, this._shielding_addr, this._ramstruct+"*"))
        return;
    // success!
    this._valid = true;

    debug_print("NTElectrode defined at 0x" + this._addr.toString(16) + ", symbol=" + this._symbol + ", name=" + this._name, false);

    if(NTElectrode.ByAddr[addr])
        alert("Internal error: NTElectrode already exists");
    
    NTElectrode.All.push(this);
    NTElectrode.ById[this._id] = this;
    NTElectrode.ByName[this._name] = this;
    NTElectrode.ByAddr[this._addr] = this;
    NTElectrode.BySymbol[this._symbol] = this;
}

NTElectrode.All = new Array();
NTElectrode.ById = new Array();
NTElectrode.ByName = new Array();
NTElectrode.ByAddr = new Array();
NTElectrode.BySymbol = new Array();
document.NTElectrode = NTElectrode;


NTElectrode.UpdateGuiAll = function ()
{
    for(e in NTElectrode.All)
        NTElectrode.All[e].UpdateGui();
}

NTElectrode.prototype.IsValid = function()
{
    return this._valid;
}

NTElectrode.prototype.GetName = function()
{
    return this._name;
}

NTElectrode.prototype.GetSymbol = function()
{
    return this._symbol;
}

NTElectrode.prototype.GetLink = function()
{
    var code1 = "NTElectrode.ById[\"" + this._id + "\"].CreateProperty(3);SetCurrenPage(3)";
    var code2 = "NTElectrode.ById[\"" + this._id + "\"].OnLinkClicked()";
    return "<p><a href='pcmaster:void' onclick='"+code1+"'>" + this._name + "</a>, <a href='pcmaster:void' onclick='" + code2 + "'>scope</a></p>";
}
NTElectrode.prototype.GetKeydetType = function()
{
    return this._keydettype;
}

NTElectrode.prototype.GetGuiVar = function(id)
{
    return "<span id=" + this._gui_id1 + id + ">?</span>";
}


NTElectrode.prototype.GetGuiColor = function(id)
{
    return "<div id=" + this._gui_id1 + id + " class=\""+this._electrode_color+"\"></div>";
}

NTElectrode.prototype.GetGuiTouchSt = function()
{
    return "<div id=" + this._gui_id1 + "TouchSt class=\"empty-touch\"></div>";
}


NTElectrode.prototype.CreateScope = function(def_object)
{
    var item = this._name + " Details";
    var ok = pcm.DefineScope(item, JSON.stringify(def_object));
    
    if(ok)
        pcm.SelectItem(item, "scope");
    else
        alert("Could not create electrode graph item.");
        
    return ok; 
}

NTElectrode.prototype.GetTouchState = function()
{
   if(!pcm_read_var(this._symbol + "->status_index", 1))
        return false;
        
   var status_index = pcm.LastVariable_vValue; 

   if(!pcm_read_var(this._symbol + "->status[" + status_index + "].state", 4))
        return false;
        
    return pcm.LastVariable_vValue == 2;
}

NTElectrode.prototype.GetSignal = function()
{
   if(!pcm_read_var(this._symbol + "->signal"))
        return 0;
        
    return pcm.LastVariable_vValue;
}
NTElectrode.prototype.GetVar = function(param)
{
   if(!pcm_read_var(this._symbol + "->"+param))
        return 0;
        
    return pcm.LastVariable_vValue;
}

// signal is common to all electrodes
NTElectrode.prototype.DefineSignalVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->signal");
}

// status_index is common to all electrodes
NTElectrode.prototype.DefineStatusVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->status_index");
}

// baseline is common to all electrodes
NTElectrode.prototype.DefineBaselineVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->baseline");
}

// flags is common to all electrodes
NTElectrode.prototype.DefineFlagsVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->flags");
}

// raw_signal is common to all electrodes
NTElectrode.prototype.DefineRawSignalVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->raw_signal");
}

// keydetector_data is common to all electrodes
NTElectrode.prototype.DefineKeydetectorDataVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->keydetector_data");
}

// filtered signal is common to safa electrodes
NTElectrode.prototype.DefineFilteredSignalVariable = function(vname)
{
    debug_assert(this._keydetsymbol, "need typed keydetector");
    return pcm_define_variable(vname, this._keydetsymbol + "->predicted_signal");
}

// filtered signal is common to safa electrodes
NTElectrode.prototype.DefineNoiseVariable = function(vname)
{
    debug_assert(this._keydetsymbol, "need typed keydetector");
    return pcm_define_variable(vname, this._keydetsymbol + "->noise");
}

// filtered signal is common to safa electrodes
NTElectrode.prototype.DefineEventCntVariable = function(vname)
{
    debug_assert(this._keydetsymbol, "need typed keydetector");
    return pcm_define_variable(vname, this._keydetsymbol + "->entry_event_cnt");
}

// filtered signal is common to safa electrodes
NTElectrode.prototype.DefineDeadBandVariable = function(vname)
{
    debug_assert(this._keydetsymbol, "need typed keydetector");
    return pcm_define_variable(vname, this._keydetsymbol + "->deadband_cnt");
}

// DeadbandHigh signal is common to safa electrodes
NTElectrode.prototype.DefineDeadBandHighVariable = function(vname)
{
    debug_assert(this._keydetsymbol, "need typed keydetector");
    return pcm_define_variable(vname, this._keydetsymbol + "->deadband_h");
}

// DeadbandLow signal is common to safa electrodes
NTElectrode.prototype.DefineDeadBandLowVariable = function(vname)
{
    debug_assert(this._keydetsymbol, "need typed keydetector");
    return pcm_define_variable(vname, this._keydetsymbol + "->deadband_l");
}

// update common electrode GUI showing just a touch-release state and signal value
NTElectrode.prototype.UpdateGui = function()
{
    var img = document.getElementById(this._gui_id1+"TouchSt");
    if(img)
        img.className = this.GetTouchState() ? "full-touch" : "empty-touch"; 

    var obj = document.getElementById(this._gui_id1+"signal");
    if(obj)
        obj.innerHTML = "<p>"+this.GetVar("signal")+"</p>"; 
    obj = document.getElementById(this._gui_id1+"baseline");
    if(obj)
        obj.innerHTML = "<p>"+this.GetVar("baseline")+"</p>"; 
    obj = document.getElementById(this._gui_id1+"raw_signal");
    if(obj)
        obj.innerHTML = "<p>"+this.GetVar("raw_signal")+"</p>"; 
    obj = document.getElementById(this._gui_id1+"flags");
    if(obj)
        obj.innerHTML = this.GetVar("flags");
}

// define basic scope. this function is typically overridden by NTElectrode_xxx 
NTElectrode.prototype.OnLinkClicked = function()
{
    // give scope with basic electrode variables
    var vname_status  = this._name + "_status_index"; 
    var vname_signal  = this._name + "_signal"; 
    var vname_baseline = this._name + "_position";
    
    var ok =
        this.DefineStatusVariable(vname_status) && 
        this.DefineSignalVariable(vname_signal) && 
        this.DefineBaselineVariable(vname_baseline);

    // scope variables
    var vars = [ 
        {"variable":vname_signal, "visible":true, "y_block":0 }, 
        {"variable":vname_baseline, "visible":true, "y_block":0 },
        {"variable":vname_status, "visible":true, "y_block":1 },
    ]; 

    // scope Y-blocks
    var yblocks = [
        { "laxis_label":"raw signal", "join_class":0, "laxis_min_auto":true, "laxis_max_auto":true },
        { "laxis_label":"touch status", "join_class":1, "laxis_min_auto":true, "laxis_max_auto":true },
    ];
    
    // scope definition
    var def = {};
    def["var_info"] = vars;
    def["yblock_info"] = yblocks;
    def["scope_period"] = 0.025;
    def["href"] = DOC_PATH+"group__electrodes.html#details";

    this.CreateScope(def);
}

function ElectrodeButtonHandler(name)
{
  var pObj =NTElectrode.ByName[name];
  pObj.UpdateGuiProperty();
  
}

NTElectrode.prototype.CreateProperty = function(tab)
{
    var index = 0;
    var x = document.createElement("SELECT");

    //get selected electrode
    for (e in NTElectrode.All)
    {
		var opt;
		opt = document.createElement("option");
		opt.setAttribute("value", "1");
		opt.innerHTML = NTElectrode.All[e]._name;
		opt.className = "selection-name";
		if(NTElectrode.All[e]._name == this._name)
		{
                    index = e;
		}
		x.appendChild(opt);
    }
    x.id = "NTElectrode_select";
    x.selectedIndex = Number(index);
    x.className = "choose unselected";
    x.onchange = changeHandler;
    function changeHandler(){
        var tmp = 0;
        sel = document.getElementById("NTElectrode_select");
        if(sel)
          tmp = sel.selectedIndex;
        NTElectrode.All[tmp].CreateProperty(tab);
    }
    
    //create content of the electrode property
    var str;
    str = "<div class=\"top-modal-section\">";
    str += "<p class=\"describing\">Selected electrode: </p>";
    var code1 = "NTElectrode.ById[\"" + this._id + "\"].OnLinkClicked()";
    str += "<div class=\"select-wrap\" id=selectDiv>";
    str += "</div>";
    str += "<p class=\"modal-headline\"><a href='pcmaster:void' onclick='" + code1 + "'>Scope</a> of " + this._name+ " <span class=\"grey-text\">| measured data</span></p>"
    str += "<p class=\"modal-state-ok\"><img src=\"./img/detected.svg\" class=\"icon-detected\">detected</p>";
    str += "</div>";
    str += this.GetElectrodeGui("");
    str += this.GetKeydetGui("");
    if(this._shielding_addr)
    {
        str += "</br>Shielding electrode:";
        str += this.GetElectrodeGui("shielding_electrode->");
    }
    str += "</p></center>";

    // move property of the electrode to page
    var div = document.getElementById("tab"+String(tab));
    if(div)
        div.innerHTML=str;
    var div2 = document.getElementById("selectDiv");

    //update select object
    div2.appendChild(x);
}

NTElectrode.prototype.GetElectrodeGui = function(shielding)
{
    var str = "";
    var idbase = this._id+shielding+"elData";
    str += "<div class=\"modules-table\">";
    str += "<div class=\"modules-table-headline\">";
    str += "<div class=\"mhl-raw-signal\"><p>Raw_Signal</p></div>";
    str += "<div class=\"mhl-signal\"><p>Signal</p></div>";
    str += "<div class=\"mhl-baseline\"><p>Baseline</p></div>";
    str += "<div class=\"mhl-flags\"><p>Flags</p></div>";
    for(i=0; i<this._statusCnt; i++)
    {
	    str += "<div class=\"mhl-statex\"><p>State "+i+"</p></div>";
	    str += "<div class=\"mhl-tstamp\"><p>T_stamp "+i+"</p></div>";
    }
    str += "</div>";
    str += "<div class=\"modules-table-line\">";
    str += "<div class=\"mtl-raw-signal\" id=\""+idbase+"raw_signal\"><p>0</p></div>";
    str += "<div class=\"mtl-signal\" id=\""+idbase+"signal\"><p>0</p></div>";
    str += "<div class=\"mtl-baseline\" id=\""+idbase+"baseline\"><p>0</p></div>";
    str += "<div class=\"mtl-flags\" id=\""+idbase+"flags\"><p>0</p></div>";
    for(i=0; i<this._statusCnt; i++)
    {
	    str += "<div class=\"mtl-statex\" id=\""+idbase+"status["+i+"].state\"><p>0</p></div>";
	    str += "<div class=\"mtl-tstamp\" id=\""+idbase+"status["+i+"].time_stamp\"><p>0</p></div>";
    }
    str += "<div class=\"cistic\"></div>";
    str += "</div>";
    str += "</div>";
    
    return str;
}

NTElectrode.prototype.UpdateGuiElectrode = function()
{
    this.UpdateGuiElectrodeRaw("");
}

NTElectrode.prototype.UpdateGuiShieldElectrode = function()
{
    if(this._shielding_addr)
      this.UpdateGuiElectrodeRaw("shielding_electrode->");
}

NTElectrode.prototype.UpdateGuiElectrodeRaw = function(shieldding)
{
    var idbase = this._id+shieldding+"elData";
    
    var subids = ["raw_signal", "signal", "baseline", "flags"]; 
    for( subidindex in subids)
    {
        var obj = document.getElementById(idbase+subids[subidindex]);
        if(obj) 
            obj.innerHTML = "<p>"+this.GetVar(shieldding+subids[subidindex])+"</p>";
    }
    for(i=0; i<this._statusCnt; i++)
    {
		var obj = document.getElementById(idbase+"status["+i+"].state");
		if(obj)
		{
			obj.innerHTML = "<p>"+this.GetVar(shieldding+"status["+i+"].state")+"</p>"; 
			obj.style.backgroundColor = "azure";
		}
		obj =   document.getElementById(idbase+"status["+i+"].time_stamp"); 
		if(obj)
		{
			var t = this.GetVar(shieldding+"status["+i+"].time_stamp");
			tms = t%1000;
			ts = Math.floor(t/1000);
			tm = Math.floor(ts/60);
			th = Math.floor(tm/60);
			tstr = String(th) + ":" + String(tm%60) + ":" + String(ts%60) + "." + String(tms);
			obj.innerHTML = "<p>"+tstr+"</p>";
			obj.style.backgroundColor = "white";
		} 
    }
    var stIndex = this.GetVar(shieldding+"status_index");
    var obj = document.getElementById(idbase+"status["+stIndex+"].state");
    if(obj)
        obj.style.backgroundColor = "DarkGoldenRod"
    obj = document.getElementById(idbase+"status["+stIndex+"].time_stamp");
    if(obj)
        obj.style.backgroundColor = "yellow"    
}

function GetTimeFromMs(rawTime)
{
	var secs;
	var min;
	var houts;

	if(rawTime < 1000)
	{
		return rawTime.toString(10) + "ms";  
	}
	else if(rawTime < (1000 * 60))
	{
		secs = rawTime / 1000;
		return secs.toString(10)+"s";
	}
	else if(rawTime < (1000 * 60 * 60))
	{
		min = rawTime / (1000 * 60);
		min = Math.floor(min);
		rawTime -= min * (1000 * 60);
		secs = rawTime / 1000;
		return min.toString(10)+"m:"+secs.toString(10)+"s";
	}else
	{
		hours = rawTime / (1000 * 60 *60);
		hours = Math.floor(hours);
		rawTime -= hours * (1000 * 60 * 60);
		min = rawTime / (1000 * 60);
		min = Math.floor(min);
		rawTime -= min * (1000 * 60);
		secs = rawTime / 1000;
		return hours.toString(10)+"h:"+min.toString(10)+"m:"+secs.toString(10)+"s";
	}
}

function GetElectrodeState(state)
{
  switch(state)
  {
    case 0:
        return "Init";
    case 1:
        return "Release";
    case 2:
        return "Touch";
    default:
        return "Unknown";
  }
}