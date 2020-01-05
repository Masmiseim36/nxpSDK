/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTControl class  */
  
function NTControl(symbol, addr)
{
    this._id = get_unique_id();
    this._valid = false;
    this._symbol = symbol;
    this._addr = addr;
    this._romaddr = null;
    this._type = "";
    this._name = "";
    this._electrodes = new Array();

    // sanity check
    if(NTControl.ByAddr[addr])
        alert("NTControl already exists");
    
    if(!pcm_define_symbol(symbol, addr, "nt_control_data*"))
        return;

    if(!pcm_read_ptr(symbol + "->rom"))
        return;
        
    // remember our ROM pointer
    this._romaddr = pcm.LastVariable_vValue;

    if(!pcm_define_symbol(symbol + "->rom", this._romaddr, "nt_control*"))
        return;

    // read x->electrodes array
    var elarrname = symbol + "->electrodes";
    if(!pcm_read_var(symbol + "->electrodes_size", 1))
      return false;
    var el_cnt = pcm.LastVariable_vValue;
    
    var eix, elarr = read_ptr_array_cnt(elarrname, el_cnt);

    for(eix=0; eix<elarr.length; eix++)
    {
        // pointer to the nt_electrode object
        var eptr = elarr[eix];

        // electrode already loaded?
        var el = NTElectrode.ByAddr[eptr];

        debug_print("NTControl uses electrode #" + eix + " at 0x" + eptr.toString(16) + ", " + (el ? el.GetName() : "creating..."), false);

        // no, create basic nt_electrode type without parent module
        if(!el)
            el = new NTElectrode(null, elarrname + "["+eix+"]", eptr, null, true);

        if(el.IsValid())
            this._electrodes.push(el);
    }

    // success!
    this._valid = true;
    
    // try to determine control type dynamically
    if(pcm_read_ptr(symbol + "->rom->interface"))
    {
        var itf = pcm.LastVariable_vValue;

        debug_print("Pointer of interface: 0x" + itf.toString(16), false);

        // by comparing interface pointers with known ones
        for(ct in CONTROL_TYPES)
        {
            if(pcm_get_symbol_info("nt_control_" + CONTROL_TYPES[ct] + "_interface"))
            {
                debug_print("Found interface: " + CONTROL_TYPES[ct], false);
                if(itf == pcm.LastSymbolInfo_addr)
                {
                    this._type = CONTROL_TYPES[ct];
                    debug_print("This interface is good one: " + CONTROL_TYPES[ct], false);

                    this._controlsstatsymbol = symbol + "->rom->control_params";
                    // define symbol for control parameters
                    if(!pcm_get_symbol_info(this._controlsstatsymbol))
                        return;
                    var control_params_addr = pcm.LastSymbolInfo_addr;
                    if(pcm_read_ptr(control_params_addr))
                    {
                        if(!pcm_define_symbol(this._controlsstatsymbol+"."+this._type, pcm.LastVariable_vValue, "nt_control_"+this._type))
                            return;
                    }
                    
                    break;
                }
            }
        }                    
    }

    // create the control name
     this._name = "control_" + this._type + "_" + NTControl.All.length;

    // change our class to the real one wrapping the functionality specific for the control type
    if(this._type)
    {
        var classname = "NTControl_" + this._type;
        change_class_type(this, classname);
    }

    debug_print("NTControl defined at " + this._symbol + ", name=" + this._name + ", electrodes=" + this._electrodes.length, false);

    NTControl.All.push(this);
    NTControl.ById[this._id] = this;
    NTControl.ByName[this._name] = this;
    NTControl.ByAddr[this._addr] = this;
    NTControl.BySymbol[this._symbol] = this;
}

NTControl.UpdateGuiAll = function()
{
    for(c in NTControl.All)
        NTControl.All[c].UpdateGui();
}

NTControl.All = new Array();
NTControl.ById = new Array();
NTControl.ByName = new Array();
NTControl.ByAddr = new Array();
NTControl.BySymbol = new Array();
document.NTControl = NTControl;


NTControl.prototype._className = "NTControl";

NTControl.prototype.IsValid = function()
{
    return this._valid;
}

NTControl.prototype.GetName = function()
{
    return this._name;
}

NTControl.prototype.GetSymbol = function()
{
    return this._symbol;
}

NTControl.prototype.GetElectrodes = function()
{
    return this._electrodes;
}

NTControl.prototype.GetFlags = function()
{
   if(!pcm_read_var(this._symbol + "->flags", 4))
        return 0;
        
    return pcm.LastVariable_vValue;
}

NTControl.prototype.GetLink = function()
{
    var code = "NTControl.ById[\"" + this._id + "\"].OnLinkClicked()";
    return "<a href='pcmaster:void' onclick='" + code + "'><p>" + this._name + "</p></a>";
}

NTControl.prototype.GetType = function()
{
    return this._type;
}

NTControl.prototype.UpdateGui = function()
{
    // this function is typically overriden
}

NTControl.prototype.GetGuiTag = function()
{
    // this function is typically overriden
    return "void";
}
NTControl.prototype.GetGuiTags = function()
{
    // this function is typically overriden
    return "";
}
NTControl.prototype.GetGuiTagsLabels = function()
{
    // this function is typically overriden
    return "";
}

NTControl.prototype.OnLinkClicked = function()
{
    // this function is typically overriden
    alert("no graph avaliable for generic control type");
}

NTControl.prototype.GetGui = function()
{
    var str = "";
    var ctr = this;
    str += "<div class=\"top-modal-section\">";
    str += "<div class=\"top-modal-section-wrap\">";
    str += "<p class=\"modal-headline\">"+ctr._type+"</p>";
    str += "<p class=\"modal-state-ok\"><img src=\"./img/detected.svg\" class=\"icon-detected\">detected</p>";
    str += "</div>";
    str += ctr.GetGuiTag();
    str += "</div>";
    str += "<div class=\"modules-table\">";
    str += "<div class=\"modules-table-headline\">";
    str += "<div class=\"mhl-name\"><p>Name</p></div>";
    str += "<div class=\"mhl-electrodes\"><p>Electrodes</p></div>";
    str += ctr.GetGuiTagsLabels();
    str += "</div>";
    str += "<div class=\"modules-table-line\">";
    str += "<div class=\"mtl-name\">"+ctr.GetLink()+"</div>";
    var electrodes = "";
    for(e=0; e<ctr._electrodes.length; e++)
        electrodes += ctr._electrodes[e].GetLink();
    str += "<div class=\"mtl-electrodes\">";
    str += electrodes;
    str += "</div>";
    str += ctr.GetGuiTags();
    str += "<div class=\"cistic\"></div>";
    str += "</div>";
    str += "</div>";
  return str;
}


