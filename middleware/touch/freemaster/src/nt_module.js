/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTElectrode class */
  
function NTModule(symbol, addr)
{
    this._id = get_unique_id();
    this._valid = false;
    this._symbol = symbol;
    this._addr = addr;
    this._romaddr = null;
    this._type = "";
    this._name = "";
    this._electrodes = new Array();
    this._struct_type = "nt_module_data";

    if(!pcm_define_symbol(symbol, addr, this._struct_type + "*"))
        return;
    
    debug_print("Reads pointer: " + symbol + "->rom", false);

    if(!pcm_read_ptr(symbol + "->rom"))
        return;
        
    // remember our ROM pointer
    this._romaddr = pcm.LastVariable_vValue;

    
    if(!pcm_define_symbol(symbol + "->rom", this._romaddr, "nt_module*"))
        return;

    // try to determine control type dynamically
    if(pcm_read_ptr(symbol + "->rom->interface"))
    {
        var itf = pcm.LastVariable_vValue;

        // by comparing interface pointers with known ones
        for(var mt in MODULE_TYPES)
        {
            if(pcm_get_symbol_info("nt_module_" + MODULE_TYPES[mt] + "_interface"))
            {
                if(itf == pcm.LastSymbolInfo_addr)
                {
                    debug_print("Module at " + this._symbol + " is " + MODULE_TYPES[mt]);
                    this._type = MODULE_TYPES[mt];
                    break;
                }
            }
        }
    }

    if(pcm_read_ptr(symbol + "->rom->recalib_config"))
    {
    	if(pcm_define_symbol(symbol + "->rom->recalib_config", pcm.LastVariable_vValue, "nt_tsi_recalib_config_t*"))
    		debug_print("Defined " + symbol + "->rom->recalib_config at is 0x" + pcm.LastVariable_vValue.toString(16));    		
    }
    
    // create the module name
    this._name = "module_" + this._type + "_"+ NTModule.All.length;

    // read x->electrodes array
    if(!pcm_read_var(symbol + "->electrodes_cnt", 1))
      return false;
    var el_cnt = pcm.LastVariable_vValue;
    debug_print("Read " + this._symbol + "->electrodes_cnt" + "..." + el_cnt);
    var elarrname = symbol + "->electrodes";
    var eix, elarr = read_ptr_array_cnt(elarrname, el_cnt);
    for(eix=0; eix<elarr.length; eix++)
    {
        // pointer to the nt_electrode object
        var eptr = elarr[eix];

        // electrode already loaded?
        var el = NTElectrode.ByAddr[eptr];

        debug_print("NTModule uses electrode #" + eix + " at 0x" + eptr.toString(16) + ", " + (el ? el.GetName() : "creating..."), false);
        
        // no, create nt_electrode type
        if(!el)
            el = new NTElectrode(this, elarrname + "["+eix+"]", eptr, this._type, true);
        
        if(el.IsValid())
            this._electrodes.push(el);
        else
            debug_print("NTModule uses invalid electrode? 0x" + eptr.toString(16), true);
    }

    // success!
    this._valid = true;

    debug_print("NTModule defined at " + this._symbol + ", name=" + this._name + ", electrodes=" + this._electrodes.length, false);

    if(NTModule.ByAddr[addr])
        alert("NTModule already exists");
    
    NTModule.All.push(this);
    NTModule.ById[this._id] = this;
    NTModule.ByName[this._name] = this;
    NTModule.ByAddr[this._addr] = this;
    NTModule.BySymbol[this._symbol] = this;
}

NTModule.All = new Array();
NTModule.ById = new Array();
NTModule.ByName = new Array();
NTModule.ByAddr = new Array();
NTModule.BySymbol = new Array();
document.NTModule = NTModule;

NTModule.prototype.IsValid = function()
{
    return this._valid;
}

NTModule.prototype.GetName = function()
{
    return this._name;
}

NTModule.prototype.GetSymbol = function()
{
    return this._symbol;
}

NTModule.prototype.GetType = function()
{
	return this._type;
}

NTModule.prototype.GetGui = function()
{
    var pEl = this._electrodes;

    var str="<div class=\"modules\">";
    str += "<div class=\"top-modal-section\">";
    str += "<p class=\"modal-headline\"> "+ this._type +": "+this._name+"</p>";
    str += "<p class=\"modal-state-ok\"><img src=\"./img/detected.svg\" class=\"icon-detected\">detected</p>";
    str += "</div>";
    str += "<div class=\"modules-table\">";
    str += "<div class=\"modules-table-headline\">";
    str += "<div class=\"mhl-name\"><p>Electrode name</p></div>";
    str += "<div class=\"mhl-keydetector\"><p>Keydetector</p></div>";
    str += "<div class=\"mhl-raw-cnt\"><p>Raw Cnt</p></div>";
    str += "<div class=\"mhl-baseline-cnt\"><p>Baseline Cnt</p></div>";
    str += "<div class=\"mhl-signal\"><p>Signal</p></div>";
    str += "<div class=\"mhl-touch\"><p>Touch</p></div>";
    str += "<div class=\"mhl-flags\"><p>Flags</p></div>";
    str += "</div>";
    for(el in pEl)
    {
        str += "<div class=\"modules-table-line\">";
        str += "<div class=\"mtl-name\">"+pEl[el].GetLink()+"</div>";
        str += "<div class=\"mtl-keydetector\"><p>"+pEl[el].GetKeydetType()+"</p></div>";
        str += "<div class=\"mtl-raw-cnt\"><p>"+pEl[el].GetGuiVar("raw_signal")+"</p></div>";
        str += "<div class=\"mtl-baseline-cnt\"><p>"+pEl[el].GetGuiVar("baseline")+"</p></div>";
        str += "<div class=\"mtl-signal\"><p>"+pEl[el].GetGuiVar("signal")+"</p></div>";
        str += "<div class=\"mtl-touch\">"+pEl[el].GetGuiTouchSt()+"</div>";
        str += "<div class=\"mtl-flags\"><p>"+pEl[el].GetGuiVar("flags")+"</p></div>";
        str += "</div>";
    }

    str += "</div>";
    str += "</div>";

    return str;
}

