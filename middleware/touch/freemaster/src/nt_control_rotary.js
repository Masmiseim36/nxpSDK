/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTControl_rotary class */ 

function NTControl_rotary(symbol, addr)
{
    // call base when not typed dynamically (see change_class_type()) 
    // otherwise, the base object is already constructed
    if(!this._dynamically_typed)
        NTControl(symbol, addr)

    this._gui_id0 = get_unique_id(); // position text box
    this._range = this._electrodes.length * 2;

    // define RAM type
    pcm_define_symbol(this._symbol, this._addr, "nt_control_data*");
    pcm_read_ptr(this._symbol + "->data");
    pcm_define_symbol(this._symbol + "->data", pcm.LastVariable_vValue, "nt_control_rotary_data*");
    debug_print("The Rotary Control has been created.", false);
}

NTControl_rotary.prototype.GetRange = function()
{
    return this._range;
}

NTControl_rotary.prototype.GetPosition = function()
{
   if(!pcm_read_var(this._symbol + "->data->position", 1))
        return 0;
        
    return pcm.LastVariable_vValue;
}


// The main information in top of control table
NTControl_rotary.prototype.GetGuiTag = function()
{
    var str = "";
    str = "<img src=\"./img/rotate.svg\" class=\"perifery-icon\"/>";
    return str;
}

NTControl_rotary.prototype.GetGuiTagsLabels = function()
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

NTControl_rotary.prototype.GetGuiTags = function()
{
    var str = "";
    str += "<div class=\"mtl-type-sh\"><p>"+this._type+"</p></div>";
    str += "<div class=\"mtl-position\" id=" + this._gui_id0 + "pos><p>0</p></div>";
    str += "<div class=\"mtl-rang\" id=" +  this._gui_id0 + "range><p>0</p></div>";
    str += "<div class=\"mtl-touch\"><div class=\"empty-touch\" id=" + this._gui_id0 + "Touch></div></div>";
    str += "<div class=\"mtl-movement\"><p><img id=" + this._gui_id0 + "Movement class=\"movement-icon\" src=\"./img/movement.svg\"</p></div>";
    str += "<div class=\"mtl-direction\" id=" + this._gui_id0 + "Direction><p>right</p></div>";
    str += "<div class=\"mtl-invalid-touch\" ><p><img id=" + this._gui_id0 + "Invalid class=\"triangle-icon\" src=\"./img/triangle.svg\"</p></div>";
    return str;
}

NTControl_rotary.prototype.UpdateGui = function()
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

NTControl_rotary.prototype.DefinePositionVariable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->data->position");
}

NTControl_rotary.prototype.OnLinkClicked = function()
{
    var pname = this._name + "_position";
    var ok = this.DefinePositionVariable(pname);

    // scope variables
    var vars = [ 
        {"variable":pname, "visible":true, "y_block":0 } 
    ]; 

    // up to 7 electrode signals
    var el_count = this._electrodes.length;
    if(el_count > 7)
        el_count = 7;
        
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
        { "laxis_label":"signal", "join_class":1, "laxis_min_auto":true, "laxis_max_auto":true },
    ];

    // scope definition
    var def = {};
    def["var_info"] = vars;
    def["yblock_info"] = yblocks;
    def["scope_period"] = 0.025;
    def["auto_delete"] = true;
    def["href"] = DOC_PATH+"group__rotary.html#details";

    var item = this._name + " Details";
    ok = ok && pcm.DefineScope(item, JSON.stringify(def));
    
    if(ok)
        pcm.SelectItem(item, "scope");
    else
        alert("Could not create graph with selected signals"); 
}

