/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTControl_aslider class */

function NTControl_aslider(symbol, addr)
{
    // call base when not typed dynamically (see change_class_type()) 
    // otherwise, the base object is already constructed
    if(!this._dynamically_typed)
        NTControl(symbol, addr)

    this._gui_id0 = get_unique_id(); // aslider image

    // define RAM type
    pcm_define_symbol(this._symbol, this._addr, "nt_control_data*");
    pcm_read_ptr(this._symbol + "->data");
    pcm_define_symbol(this._symbol + "->data", pcm.LastVariable_vValue, "nt_control_aslider_data");
    
    pcm_read_ptr(this._symbol + "->rom->control_params");
    pcm_define_symbol(this._symbol + "->rom->control_params", pcm.LastVariable_vValue, "nt_control_aslider*");
        
    pcm_define_variable_bit(this._name + "->flags_direction", this._symbol + "->flags", 17);
    pcm_define_variable_bit(this._name + "->flags_movement", this._symbol + "->flags", 18);
    pcm_define_variable_bit(this._name + "->flags_touch", this._symbol + "->flags", 19);

    // get maximum range
    if(pcm_read_var(this._symbol + "->rom->control_params->range", 1) && pcm.LastVariable_vValue)
    {
        this._range = pcm.LastVariable_vValue;
    }
}

NTControl_aslider.prototype.GetRange = function()
{
    return this._range;
}

NTControl_aslider.prototype.GetPosition = function()
{
   if(!pcm_read_var(this._symbol + "->data.position", 1))
        return 0;
        
    return pcm.LastVariable_vValue;
}

NTControl_aslider.prototype.GetGuiTag = function()
{
    var str = "";
    str = "<img src=\"./img/slider.svg\" class=\"perifery-icon\"/>";
    return str;
}

NTControl_aslider.prototype.GetGuiTagsLabels = function()
{
    var str = "";
    str += "<div class=\"mhl-type-sh\"><p>Type</p></div>";
    str += "<div class=\"mhl-position\"><p>Position</p></div>";
    str += "<div class=\"mhl-range\"><p>Range</p></div>";
    str += "<div class=\"mhl-touch\"><p>Touch</p></div>";
    str += "<div class=\"mhl-movement\"><p>Movement</p></div>";
    str += "<div class=\"mhl-direction\"><p>Direction</p></div>";
    str += "<div class=\"mhl-invalid-touch\"><p>Invalid touch</p></div>";
    return str;
}

NTControl_aslider.prototype.GetGuiTags = function()
{
    var str = "";
    str += "<div class=\"mtl-type-sh\"><p>"+this._type+"</p></div>";
    str += "<div class=\"mtl-position\" id=" + this._gui_id0 + "pos><p>0</p></div>";
    str += "<div class=\"mtl-range\" id=" +  this._gui_id0 + "range><p>0</p></div>";
    str += "<div class=\"mtl-touch\"><div class=\"empty-touch\" id=" + this._gui_id0 + "Touch></div></div>";
    str += "<div class=\"mtl-movement\"><p><img id=" + this._gui_id0 + "Movement class=\"movement-icon\" src=\"./img/movement.svg\"</p></div>";
    str += "<div class=\"mtl-direction\" id=" + this._gui_id0 + "Direction><p>right</p></div>";
    str += "<div class=\"mtl-invalid-touch\" ><p><img id=" + this._gui_id0 + "Invalid class=\"triangle-icon\" src=\"./img/triangle.svg\"</p></div>";
    return str;
}

NTControl_aslider.prototype.UpdateGui = function()
{
    var pos = this.GetPosition();
    var rng = this.GetRange();
    var flags = this.GetFlags();

    var kobj = document.getElementById(this._gui_id0+"pos");
    if(kobj)
        kobj.innerHTML = "<p>" + pos.toString(10) + "</p>";

    kobj = document.getElementById(this._gui_id0+"range");    
    if(kobj)
        kobj.innerHTML = "<p>" + rng.toString(10) + "</p>";

    kobj = document.getElementById(this._gui_id0+"Touch");    
    if(kobj)
    {
        kobj.className = (flags & (1<<19)) ? "full-touch" : "empty-touch";  
    }

    kobj = document.getElementById(this._gui_id0+"Movement");    
    if(kobj)
    {
        kobj.src = (flags & (1<<18)) ? "img/movement.svg" : "img/gui_arrow_move_hidden.png"; 
    }

    kobj = document.getElementById(this._gui_id0+"Direction");    
    if(kobj)
    {
        kobj.innerHTML = (flags & (1<<17)) ? "<p>right</p>" : "<p>left</p>"; 
    }

    kobj = document.getElementById(this._gui_id0+"Invalid");    
    if(kobj)
    {
        kobj.src = (flags & (1<<16)) ? "img/triangle.svg" : "img/gui_warning_hidden.png"; 
    }     
}

NTControl_aslider.prototype.DefinePositionVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->data.position");
}

NTControl_aslider.prototype.OnLinkClicked = function()
{
    var pname = this._name + "_position";
    var ok = this.DefinePositionVariable(pname);

    // scope variables
    var vars = [ 
        {"variable":pname, "visible":true, "y_block":0 } 
    ]; 

    var fname = this._name + "->flags_direction";
    var fdef = { "variable":fname, "visible":true, "y_block":2 };
    vars.push(fdef);

    fname = this._name + "->flags_movement";
    fdef = { "variable":fname, "visible":true, "y_block":2 };
    vars.push(fdef);

    fname = this._name + "->flags_touch";
    fdef = { "variable":fname, "visible":true, "y_block":2 };
    vars.push(fdef);

    // up to 4 electrode signals
    var el_count = this._electrodes.length;
    if(el_count > 4)
        el_count = 4;

    for(var e=0; e<el_count; e++)
    {
        var ename = this._name + "_electrode" + e + "_signal";
        ok = ok && this._electrodes[e].DefineSignalVariable(ename);
        
        var edef = { "variable":ename, "visible":true, "y_block":1 }; 
        vars.push(edef);
    }

    // scope Y-blocks
    var yblocks = [
        { "laxis_label":"position", "join_class":0, "laxis_min":0, "laxis_max":this.GetRange(), "laxis_min_auto":false, "laxis_max_auto":false },
        { "laxis_label":"flags", "join_class":1, "laxis_min":0, "laxis_max":2, "laxis_min_auto":false, "laxis_max_auto":false },
        { "laxis_label":"signals", "join_class":2, "laxis_min_auto":true, "laxis_max_auto":true },
    ];

    // scope definition
    var def = {};
    def["var_info"] = vars;
    def["yblock_info"] = yblocks;
    def["scope_period"] = 0.025;
    def["auto_delete"] = true;
    def["href"] = DOC_PATH+"group__aslider.html#details";

    var item = this._name + " Details";
    ok = ok && pcm.DefineScope(item, JSON.stringify(def));
    
    if(ok)
        pcm.SelectItem(item, "scope");
    else
        alert("Could not create graph with selected signals"); 
}

