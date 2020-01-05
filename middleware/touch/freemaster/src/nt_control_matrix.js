/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /* NTControl_matrix class */

function NTControl_matrix(symbol, addr)
{
    // call base when not typed dynamically (see change_class_type()) 
    // otherwise, the base object is already constructed
    if(!this._dynamically_typed)
        NTControl(symbol, addr)

    this._gui_id0 = get_unique_id(); // span
    // define RAM type
    pcm_define_symbol(this._symbol, this._addr, "nt_control_data*");
    pcm_read_ptr(this._symbol + "->data");
    pcm_define_symbol(this._symbol + "->data", pcm.LastVariable_vValue, "nt_control_matrix_data");

    pcm_read_ptr(this._symbol + "->rom->control_params");
    pcm_define_symbol(this._symbol + "->rom->control_params", pcm.LastVariable_vValue, "nt_control_matrix*");
    
    debug_print("The matrix Control has been created.", false);
}
/**********************************************************************************************************************/
NTControl_matrix.prototype.GetSizeHor= function()
{                                                 /**/
   if(!pcm_read_var(this._symbol + "->rom->control_params->touchpad_size_horizontal", 1))
        return 0;      
    return pcm.LastVariable_vValue;
}
NTControl_matrix.prototype.GetSizeVer= function()
{                                                 /**/
   if(!pcm_read_var(this._symbol + "->rom->control_params->touchpad_size_vertical", 1))
        return 0;     
    return pcm.LastVariable_vValue;
}

NTControl_matrix.prototype.GetPosX= function()
{
   if(!pcm_read_var(this._symbol + "->data.actual_position[0].x", 1))
        return 0;       
    return pcm.LastVariable_vValue;
}

NTControl_matrix.prototype.GetPosY= function()
{
   if(!pcm_read_var(this._symbol + "->data.actual_position[0].y", 1))
        return 0;        
    return pcm.LastVariable_vValue;
}

NTControl_matrix.prototype.GetPosX_2= function()
{
   if(!pcm_read_var(this._symbol + "->data.actual_position[1].x", 1))
        return 0;      
    return pcm.LastVariable_vValue;
}

NTControl_matrix.prototype.GetPosY_2= function()
{
   if(!pcm_read_var(this._symbol + "->data.actual_position[1].y", 1))
        return 0;      
    return pcm.LastVariable_vValue;
}

NTControl_matrix.prototype.GetElState= function()
{
   if(!pcm_read_var(this._symbol + "->data.last_electode_states", 4))
        return 0;    
    return pcm.LastVariable_vValue;
}


NTControl_matrix.prototype.GetRecognized_gesture = function()
{
   if(!pcm_read_var(this._symbol + "->data.recognized_gesture", 1))
        return 0;   
    return pcm.LastVariable_vValue;
}

NTControl_matrix.prototype.GetGuiTag = function()
{
    var str = "";
    str = "<img src=\"./img/matrix.svg\" class=\"perifery-icon\"/>";
    return str;
}

NTControl.prototype.GetGuiTags = function()
{
  var str = "";
  str += "<div class=\"mtl-range\"><p>"+this._type+"</p></div>";
  str += "<div class=\"mtl-range\"><div class=\"empty-touch\" id=" + this._gui_id0 + "Touch></div></div>";
  str += "<div class=\"mtl-range\" id=" +  this._gui_id0 + "PosX><p>?</p></div>";
  str += "<div class=\"mtl-range\" id=" +  this._gui_id0 + "PosY><p>?</p></div>";
  str += "<div class=\"mtl-range\" id=" +  this._gui_id0 + "PosX_2><p>?</p></div>";
  str += "<div class=\"mtl-range\" id=" +  this._gui_id0 + "PosY_2><p>?</p></div>";
  str += "<div class=\"mtl-range\" id=" +  this._gui_id0 + "Gestcure><p>?</p></div>";
  return str;  
}

NTControl_matrix.prototype.GetGuiTagsLabels = function()
{
  var str = "";
  str += "<div class=\"mhl-range\"><p>Type</p></div>";
  str += "<div class=\"mhl-range\"><p>Touch</p></div>";
  str += "<div class=\"mhl-range\"><p>Position X1</p></div>";
  str += "<div class=\"mhl-range\"><p>Position Y1</p></div>";
  str += "<div class=\"mhl-range\"><p>Position X2</p></div>";
  str += "<div class=\"mhl-range\"><p>Position Y2</p></div>";
  str += "<div class=\"mhl-range\"><p>Gesture</p></div>";
  return str;
}

NTControl_matrix.prototype.UpdateGui = function()
{
    var gest = this.GetRecognized_gesture();
    var posX = this.GetPosX();
    var posY = this.GetPosY();
    var posX_2 = this.GetPosX_2();
    var posY_2 = this.GetPosY_2();
    var hor = this.GetSizeHor();
    var ver = this.GetSizeVer();
    var Elstate =  this.GetElState();
    var first_touch = 0;
    
    kobj = document.getElementById(this._gui_id0+"Touch");    
    if(kobj)
    {
        kobj.className = Elstate ? "full-touch" : "empty-touch";       
    }
    kobj = document.getElementById(this._gui_id0+"PosX");    
    if(kobj)
    {
        if (Elstate)
        {
            kobj.innerHTML = "<p>"+posX.toString(10)+"</p>";
        }
        else
        {
            kobj.innerHTML = '<p>?</p>';
        }
    }        
    kobj = document.getElementById(this._gui_id0+"PosY");    
    if(kobj)
    {
        if (Elstate)
        {
            kobj.innerHTML = "<p>"+posY.toString(10)+"</p>";
        }
        else
        {
            kobj.innerHTML = '<p>?</p>';
        }
    }
    
    kobj = document.getElementById(this._gui_id0+"PosX_2");    
    if(kobj)
    {
        if (Elstate)
        {
        	kobj.innerHTML = "<p>"+posX_2.toString(10)+"</p>";
        }
        else
        {
            kobj.innerHTML = '<p>?</p>';
        }
    }    
    kobj = document.getElementById(this._gui_id0+"PosY_2");    
    if(kobj)
    {
        if (Elstate)
        {
            kobj.innerHTML = "<p>"+posY_2.toString(10)+"</p>";  
        }
        else
        {
            kobj.innerHTML = '<p>?</p>';
        }
    
    }
        
    kobj = document.getElementById(this._gui_id0+"Gestcure");    
    if(kobj)
    {
        if (!Elstate)
        {
            kobj.innerHTML = "<p>"+gest.toString(10)+"</p>";  
        }
        else
        {
            kobj.innerHTML = '<p>?</p>';
        }
    }   
}

NTControl_matrix.prototype.DefinePosX1Variable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->data.actual_position[0].x");
}

NTControl_matrix.prototype.DefinePosY1Variable = function(vname)
{
    return pcm_define_variable(vname, this._symbol + "->data.actual_position[0].y");
}

NTControl_matrix.prototype.OnLinkClicked = function()
{
    var PosX1 = this._name + "->data.actual_position[0].x"
    var PosY1 = this._name + "->data.actual_position[0].y";

 //   var ok = this.DefinePosX1Variable(PosX1);
    var ok =
        this.DefinePosX1Variable(PosX1) &&
        this.DefinePosY1Variable(PosY1);

    // scope variables
    var vars = [ 
        {"variable":PosY1, "visible":true},,,,,,,
        {"variable":PosX1, "visible":false},
    ]; 

     // scope Y-blocks
    var yblocks = [
       { "laxis_label":"Actual finger position Y", "join_class":0, "laxis_min":0, "laxis_max":11, "laxis_min_auto":false, "laxis_max_auto":false },
    ];

    // scope definition
    var def = {};
    def["graph_type"] = 1;
    def["var_info"] = vars;
    def["yblock_info"] = yblocks;

    def["scope_period"] = 0.025;
    def["graph_buffer"] = 50;    
    def["auto_delete"] = true;
    def["href"] = DOC_PATH+"group__matrix.html#details";

    var item = this._name + " Details";
    ok = ok && pcm.DefineScope(item, JSON.stringify(def));
    
    if(ok)
        pcm.SelectItem(item, "scope");
    else
        alert("Could not create graph with selected signals"); 
}

