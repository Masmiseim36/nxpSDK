/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTControl_proxi class */

function NTControl_proxi(symbol, addr)
{
    // call base when not typed dynamically (see change_class_type()) 
    // otherwise, the base object is already constructed
    if(!this._dynamically_typed)
        NTControl(symbol, addr)

    this._gui_id0 = get_unique_id(); // proxi image

    // define RAM type
    pcm_define_symbol(this._symbol, this._addr, "nt_control_data*");
    pcm_read_ptr(this._symbol + "->data");
    pcm_define_symbol(this._symbol + "->data", pcm.LastVariable_vValue, "nt_control_proxi_data");
    
    pcm_read_ptr(this._symbol + "->rom->control_params");
    pcm_define_symbol(this._symbol + "->rom->control_params", pcm.LastVariable_vValue, "nt_control_proxi*");
        
    pcm_define_variable_bit(this._name + "->flags_direction", this._symbol + "->flags", 16);
    pcm_define_variable_bit(this._name + "->flags_movement", this._symbol + "->flags", 17);
    pcm_define_variable_bit(this._name + "->flags_touch", this._symbol + "->flags", 18);

    // get maximum range
    if(pcm_read_var(this._symbol + "->rom->control_params->range", 1) && pcm.LastVariable_vValue)
    {
        this._range = pcm.LastVariable_vValue;
    }
}

NTControl_proxi.prototype.GetRange = function()
{
    return this._range;
}

NTControl_proxi.prototype.GetProximity = function()
{
     if(!pcm_read_var(this._symbol + "->data.proximity", 1))
        return 0;
        
    return pcm.LastVariable_vValue; 
}


NTControl_proxi.prototype.GetIndex = function()
{
   if(!pcm_read_var(this._symbol + "->data.index", 1))
        return 0;
        
    return pcm.LastVariable_vValue;
}


NTControl_proxi.prototype.GetGuiTag = function()
{
    var str = "";
    str = "<img src=\"./img/proxi.svg\" class=\"perifery-icon\"/>";
    return str;
}

NTControl_proxi.prototype.GetGuiTagsLabels = function()
{
    var str = "";
  str += "<div class=\"mhl-type-sh\"><p>Type</p></div>";
  str += "<div class=\"mhl-position\"><p>Proximity</p></div>";
  str += "<div class=\"mhl-position\"><p>Index</p></div>";
  str += "<div class=\"mhl-touch\"><p>Touch</p></div>";
  str += "<div class=\"mhl-movement\"><p>Movement</p></div>";
  str += "<div class=\"mhl-direction\"><p>Direction</p></div>";
    return str;
}

NTControl_proxi.prototype.GetGuiTags = function()
{
  var str = "";
  str += "<div class=\"mtl-type-sh\"><p>"+this._type+"</p></div>";
  str += "<div class=\"mtl-position\" id=" + this._gui_id0 + "pos><p>0</p></div>";
  str += "<div class=\"mtl-position\" id=" +  this._gui_id0 + "ind><p>0</p></div>";
  str += "<div class=\"mtl-touch\"><div class=\"empty-touch\" id=" + this._gui_id0 + "Touch></div></div>";
  str += "<div class=\"mtl-movement\"><p><img id=" + this._gui_id0 + "Movement class=\"movement-icon\" src=\"./img/movement.svg\"</p></div>";
  str += "<div class=\"mtl-direction\" id=" + this._gui_id0 + "Direction><p>down</p></div>";
  return str;
}

NTControl_proxi.prototype.UpdateGui = function()
{
    var pos = this.GetProximity();
    var rng = this.GetRange();
    var flags = this.GetFlags();
    var ind = this.GetIndex();

    var kobj = document.getElementById(this._gui_id0+"pos");
    if(kobj)
        kobj.innerHTML = "<p>" + pos.toString(10) + "</p>";

    kobj = document.getElementById(this._gui_id0+"range");    
    if(kobj)
        kobj.innerHTML = "<p>" + rng.toString(10) + "</p>";

    kobj = document.getElementById(this._gui_id0+"ind");    
    if(kobj)
        kobj.innerHTML = "<p>"+ind.toString(10)+"</p>";


    kobj = document.getElementById(this._gui_id0+"Touch");    
    if(kobj)
    {
        kobj.className = (flags & (1<<18)) ? "full-touch" : "empty-touch";  
    }
  
    kobj = document.getElementById(this._gui_id0+"Movement");    
    if(kobj)
    {
        kobj.src = (flags & (1<<17)) ? "img/movement.svg" : "img/gui_arrow_move_hidden.png"; 
    }

    kobj = document.getElementById(this._gui_id0+"Direction");    
    if(kobj)
    {
        kobj.innerHTML = (flags & (1<<16)) ? "<p>down</p>" : "<p>up</p>"; 
    }
            
}

NTControl_proxi.prototype.DefineFlagsVariable = function(vname, nn)
{
    return pcm_define_variable(vname, this._symbol + "->data.proxi_flags["+ nn +"]");
}

NTControl_proxi.prototype.DefineElVariable = function(vname, nn)
{
    return pcm_define_variable(vname, this._symbol + "->data.proxi_curr["+ nn +"]");
}

NTControl_proxi.prototype.OnLinkClicked = function()
{
    var pname = "current proximity [0]";
    var ok = this.DefineElVariable(pname,0);

    // scope variables
    var vars = [ 
        {"variable":pname, "visible":true, "y_block":0 } 
    ]; 

    // up to 4 electrode signals
    var el_count = this._electrodes.length;
    if(el_count > 4)
        el_count = 4;

    for(var e=1; e<el_count; e++)
    {
        var ename = "current proximity ["+ e +"]";
        ok = ok && this.DefineElVariable(ename, e);

        var edef = { "variable":ename, "visible":true, "y_block":0 }; 
        vars.push(edef);
    }

    var fname = "flags [0]";
    var ok = ok &&  this.DefineFlagsVariable(fname,0);
    var fdef = { "variable":fname, "visible":true, "y_block":1 };
    vars.push(fdef);

    for(var e=1; e<el_count; e++)
    {        
        var ename = "flags ["+ e +"]";
        ok = ok && this.DefineFlagsVariable(ename, e);

        var edef = { "variable":ename, "visible":true, "y_block":1 }; 
        vars.push(edef);
    }

    // scope Y-blocks
    var yblocks = [
        { "laxis_label":"proximity", "join_class":0, "laxis_min":0, "laxis_max":this.GetIndex(), "laxis_min_auto":true, "laxis_max_auto":true },
        { "laxis_label":"flags", "join_class":1, "laxis_min":0, "laxis_max":this.GetIndex(), "laxis_min_auto":true, "laxis_max_auto":true },
    ];

    // scope definition
    var def = {};
    def["var_info"] = vars;
    def["yblock_info"] = yblocks;
    def["scope_period"] = 0.025;
    def["auto_delete"] = true;
    def["href"] = DOC_PATH+"group__proxi.html#details";

    var item = this._name + " Details";
    ok = ok && pcm.DefineScope(item, JSON.stringify(def));
    
    if(ok)
        pcm.SelectItem(item, "scope");
    else
        alert("Could not create graph with selected signals"); 
}

