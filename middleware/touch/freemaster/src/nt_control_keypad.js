/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTControl_keypad class */

function NTControl_keypad(symbol, addr)
{
    // call base when not typed dynamically (see change_class_type()) 
    // otherwise, the base object is already constructed
    if(!this._dynamically_typed)
        NTControl(symbol, addr)

    this._gui_id0 = get_unique_id(); // span
    // define RAM type
    pcm_define_symbol(this._symbol, this._addr, "nt_control_data*");
    pcm_read_ptr(this._symbol + "->data");
    pcm_define_symbol(this._symbol + "->data", pcm.LastVariable_vValue, "nt_control_keypad_data");

    pcm_read_ptr(this._symbol + "->rom->control_params");
    pcm_define_symbol(this._symbol + "->rom->control_params", pcm.LastVariable_vValue, "nt_control_keypad*");
    
    debug_print("The Keypad Control has been created.", false);
}

NTControl_keypad.prototype.GetKeyState = function()
{
   if(!pcm_read_var(this._symbol + "->data.last_state", 4))
        return 0;
        
    return pcm.LastVariable_vValue;
}

NTControl_keypad.prototype.GetKeyAutorepRate = function()
{
   if(!pcm_read_var(this._symbol + "->data.autorepeat_rate", 2))
        return 0;
        
    return pcm.LastVariable_vValue;
}

NTControl_keypad.prototype.GetGuiTag = function()
{
    var str = "";
    str = "<img src=\"./img/keypad.svg\" class=\"perifery-icon\"/>";
    return str;
}

NTControl_keypad.prototype.GetGuiTags = function()
{
    var str_grouped_electrodes = "img/gui_yes_hidden.png";
    var grp_els = "no";

    // get maximum range
    if(pcm_read_var(this._symbol + "->rom->control_params->groups_size", 1) && pcm.LastVariable_vValue)
    {
        str_grouped_electrodes = "img/gui_yes.png";
        grp_els = "yes";
    }

    var str = "";
    str += "<div class=\"mtl-type\"><p>"+this._type+"</p></div>";
    str += "<div class=\"mtl-state\" id=\"" + this._gui_id0 + "st\"><div class=\"empty-touch\"></div></div>";
    str += "<div class=\"mtl-rate\" id=\"" +  this._gui_id0 + "autorep_rate\"><p>?</p></div>";
    str += "<div class=\"mtl-touch\"><div class=\"empty-touch\" id=\"" + this._gui_id0 + "TouchObj\"></div></div>";
    str += "<div class=\"mtl-group\"><p>"+grp_els+"</p><img id=" + this._gui_id0 + "GroupE src=\""+str_grouped_electrodes+"\" style=\"height:24px;width:24px;\"></div>";
    return str;  
}

NTControl_keypad.prototype.GetGuiTagsLabels = function()
{
    var str = "";
        str += "<div class=\"mhl-type\"><p>Type</p></div>";
        str += "<div class=\"mhl-state\"><p>State</p></div>";
        str += "<div class=\"mhl-rate\"><p>Autorepear rate</p></div>";
        str += "<div class=\"mhl-touch\"><p>Touch</p></div>";
        str += "<div class=\"mhl-group\"><p>Grouped electrodes</p></div>";
    return str;
}

NTControl_keypad.prototype.UpdateGui = function()
{
    var kst = this.GetKeyState();
    var kobj = document.getElementById(this._gui_id0+"st");
    
    if(kobj)
        kobj.innerHTML = "<p>0x"+ kst.toString(16)+"</p>";
    kobj = document.getElementById(this._gui_id0+"autorep_rate");
    
    if(kobj)
        kobj.innerHTML = "<p>"+this.GetKeyAutorepRate().toString(10)+"</p>";

    kobj = document.getElementById(this._gui_id0+"TouchObj");    
    if(kobj)
    {
        kobj.className = (kst != 0) ? "full-touch" : "empty-touch";         
    }
}

NTControl_keypad.prototype.DefineKeyStateVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->data.last_state");
}

NTControl_keypad.prototype.OnLinkClicked = function()
{
    var pname = this._name + "_keys_state";
    var ok = this.DefineKeyStateVariable(pname);

    // scope variables
    var vars = [ 
        {"variable":pname, "visible":true, "y_block":0 } 
    ]; 

    // up to 7 electrodes, keypad may have more
    var ec = this._electrodes.length < 7 ? this._electrodes.length : 7;  
    
    for(var e=0; e<this._electrodes.length; e++)
    {
        var ename = this._name + "_electrode" + e + "_signal";
        ok = ok && this._electrodes[e].DefineSignalVariable(ename);
        
        var edef = { "variable":ename, "visible":true, "y_block":1 }; 
        vars.push(edef);
    }

    // scope Y-blocks
    var yblocks = [
        { "laxis_label":"key state", "join_class":0, "laxis_min":0, "laxis_min_auto":false, "laxis_max_auto":true },
        { "laxis_label":"signal", "join_class":1, "laxis_min_auto":true, "laxis_max_auto":true },
    ];

    // scope definition
    var def = {};
    def["var_info"] = vars;
    def["yblock_info"] = yblocks;
    def["scope_period"] = 0.025;
    def["auto_delete"] = true;
    def["href"] = DOC_PATH+"group__keypad.html#details";

    var item = this._name + " Details";
    ok = ok && pcm.DefineScope(item, JSON.stringify(def));
    
    if(ok)
        pcm.SelectItem(item, "scope");
    else
        alert("Could not create graph with selected signals"); 
}

