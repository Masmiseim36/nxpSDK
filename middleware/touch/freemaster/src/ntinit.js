/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
var nt_good = false;
var SIZEOF_PTR = 4;
var MODULE_TYPES = ["tsi", "gpio", "gpioint", "cs"];
var CONTROL_TYPES = ["keypad", "rotary", "slider", "arotary", "aslider", "proxi", "matrix"];
var KEYDETECTOR_TYPES = ["safa", "usafa", "afid"];
var DOC_PATH="../../doc/_OUTPUT_/html/"

function main()
{
    var ver = pcm.GetAppVersion();
    
    if(ver < 0x01040400)
    {
        if(confirm("You need FreeMASTER version 1.4.4 or later to display the TSS GUI.\n\n" + 
               "Do you want to navigate to FreeMASTER download page now?")) 
        {
            window.location.href = "http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FREEMASTER&tab=Design_Tools_Tab";
        }
        
        // abort the GUI
        return;
    }
    
    SetCurrenPageIndex(ActivePage);

    var board_detected = pcm.IsBoardDetected();

    if(board_detected != false)
    {
        debug_print("Board is detected.");
        // detect NT objects
        if(nt_symbols_init())
        {
          var str = "";
          for(m in NTModule.All)
            str += NTModule.All[m].GetGui();
          //debug_print(str);
          if(document.getElementById("tab1"))
          {
            tab1.innerHTML = str;
          }
          
          str = "";
          for(c in NTControl.All)
            str += NTControl.All[c].GetGui();
          //debug_print(str);
          if(document.getElementById("tab2"))
          {
            tab2.innerHTML = str;
          }
          NTElectrode.All[0].CreateProperty(3);

          GenerateVars();
          // periodic GUI refresh        
          setInterval(main_timer, 10);
        }
    }else
    {
        debug_print("No Board is detected!")        
    }
        
}

var timer_i=-1;
function main_timer()
{
    var ix;
    timer_i++;

    // update all electrode and control objects, one at each timer callback    
    if(((ix=timer_i-0) < NTElectrode.All.length)&& (ActivePage == 1))
        NTElectrode.All[ix].UpdateGui();
    else if(((ix=timer_i-NTElectrode.All.length) < NTControl.All.length)&& (ActivePage == 2))
    {
        if (ix>=0)
          NTControl.All[ix].UpdateGui();
    }
    else if(ActivePage == 3)
    {
        var tmp = 0;
        sel = document.getElementById("NTElectrode_select");
        if(sel)
          tmp = sel.selectedIndex;
        NTElectrode.All[tmp].UpdateGuiElectrode();
        NTElectrode.All[tmp].UpdateGuiShieldElectrode();
        NTElectrode.All[tmp].UpdateGuiKeydetRaw("");
    }
    else
        timer_i = -1;
}

function debug_print(msg, err)
{
    color = err ? "red" : "black";
    if(document.getElementById("msglog") && (pcm.IsCommPortOpen()))
    {
        msglog.innerHTML += "<p style=\"color:" + color + ";\" class=\"debug-log\">" + msg + "</p>";
    }
    return false;
}

function debug_assert(c, msg)
{
    if(!c)
        alert("Internal error: ASSERT failed " + msg ? msg : "!");
}

function pcm_write_var(name, size, val)
{
    if(!pcm.WriteUIntVariable(name, size, val))
    {
        debug_print("can not read variable \"" + name + "\", err=" + pcm.LastRetMsg, true);
        return false;
    }

    return true;
}

function pcm_read_var(name, size)
{
    if(!pcm.ReadUIntVariable(name, size))
    {
        debug_print("can not read variable \"" + name + "\", err=" + pcm.LastRetMsg, true);
        return false;
    }

    return true;
}

function pcm_read_ptr(name)
{
    if(!pcm.ReadUIntVariable(name, SIZEOF_PTR))
    {
        debug_print("can not read pointer \"" + name + "\", err=" + pcm.LastRetMsg, true);
        return false;
    }

    return true;
}

function pcm_define_symbol(name, addr, type, size)
{
    if(!pcm.DefineSymbol(name, addr, type, size))
    {        
        debug_print("can not find structure type \"" + type + "\"", true);
        return false;        
    }

    debug_print("Script symbol defined: " + name + " = 0x" + addr.toString(16) + "&nbsp;&nbsp;&nbsp;type:" + type, false);
    return true;
}

function pcm_duplicate_symbol(new_sym, existing_sym)
{
    var ok = false;
    var addr, size;
    
    if(pcm_get_symbol_info(existing_sym))
    {
        ok = pcm_define_symbol(new_sym, pcm.LastSymbolInfo_addr, null, pcm.LastSymbolInfo_size);
    }
    
    return ok;
}

function pcm_define_variable(name, symbol, symbol_info)
{
    var ok = false;
    
    if(pcm_get_symbol_info(symbol_info ? symbol_info : symbol))
    {
        var def = { 
            "address" : symbol,
            "byte_size" : pcm.LastSymbolInfo_size
        };
         
        ok = pcm.DefineVariable(name, JSON.stringify(def));
    }
    
    return ok;
}

function pcm_define_variable_bit(name, symbol, bit_shift, symbol_info)
{
    var ok = false;
    
    if(pcm_get_symbol_info(symbol_info ? symbol_info : symbol))
    {
        var def = { 
            "address" : symbol,
            "byte_size" : pcm.LastSymbolInfo_size,
            "bit_shift" : bit_shift,
            "bit_mask" : "one bit (0x1)"
        };
         
        ok = pcm.DefineVariable(name, JSON.stringify(def));
    }
    
    return ok;
}

function pcm_define_variable2(name, symbol, varType)
{
    var ok = false;
    
    var treatAsType = 0; //uint type
    if((varType == "SINT8") || (varType == "SINT16") || (varType == "SINT32") || (varType == "SINT64"))
      treatAsType = 1;
    else if((varType == "FLOAT") || (varType == "DOUBLE"))
      treatAsType = 2;
    else if((varType == "SFRAC16") || (varType == "SFRAC32"))
      treatAsType = 3;
    else if((varType == "UFRAC16") || (varType == "UFRAC32"))
      treatAsType = 4;
    
    var size = GetSizeFromType(varType);
    var def = { 
            "address" : symbol,
            "byte_size" : size,
            "treat_as" : treatAsType
    };
         
    ok = pcm.DefineVariable(name, JSON.stringify(def));
    return ok;
}

function pcm_get_address_info(addr, size)
{
    if(!pcm.GetAddressInfo(addr, size))
    {
        debug_print("can not find address info for 0x" + addr.toString(16) + " and size " + size, true);
        return false;
    }

    debug_print("Address info: 0x" + addr.toString(16) + " is " + pcm.LastAddressInfo_name, false);
    return true;
}

function pcm_get_symbol_info(name)
{
    if(!pcm.GetSymbolInfo(name))
    {
        debug_print("can not find symbol info \"" + name + "\"", true);
        return false;
    }
    
    return true;
}

// read NULL-terminated array of pointers, argument is name of variable which 
// stores the pointer to the first element 

function read_ptr_array(ptrvar, maxsize)
{
    if(!maxsize)
        maxsize = 100;
        
    if(!pcm_read_ptr(ptrvar, SIZEOF_PTR))
    {
        debug_print("can not read_ptr_array address info \"" + ptrvar + "\"", true);
        return new Array(); // empty array
    }

    if(!pcm.LastVariable_vValue)
    {
        debug_print("The read_ptr_array can not read NULL array from \"" + ptrvar + "\"", false);
        return new Array(); // empty array
    }

    // there is an array of pointers there
    return read_ptr_array_at(pcm.LastVariable_vValue, maxsize);
}

// read NULL-terminated array of pointers, argument is pointer to first element 

function read_ptr_array_at(addr, maxsize)
{
    var ix, ptr;
    var arr = new Array();
    
    for(ix=0; true; ix++, addr+=SIZEOF_PTR)
    {
        // read array[ix]
        if(!pcm_read_ptr(addr))
            break;

        // pointer to the nt_control object itself
        ptr = pcm.LastVariable_vValue;
        // NULL terminates the controls list
        if(!ptr)
            break;
            
        arr.push(ptr);
        
        if(ix > maxsize)
        {
            debug_print("The read_ptr_array_at \"" + addr.toString(16) + "\" failed on sanity check, more than " + maxsize + " entries found", false);
            arr = [];
            return arr;
        }
    }

    debug_print("The read_ptr_array_at \"" + addr.toString(16) + "\" returns " + arr.length + " entries", false);
    return arr;
}

// read array of pointers, argument is name of variable which 
// stores the pointer to the first element 

function read_ptr_array_cnt(ptrvar, size)
{
    if(!pcm_read_ptr(ptrvar, SIZEOF_PTR))
    {
        debug_print("can not read_ptr_array address info \"" + ptrvar + "\"", true);
        return new Array(); // empty array
    }

    if(!pcm.LastVariable_vValue)
    {
        debug_print("The read_ptr_array can not read NULL array from \"" + ptrvar + "\"", false);
        return new Array(); // empty array
    }

    // there is an array of pointers there
    return read_ptr_array_cnt_at(pcm.LastVariable_vValue, size);
}

// read array of pointers, argument is pointer to first element 

function read_ptr_array_cnt_at(addr, size)
{
    if(!size)
    {
        debug_print("Invalid size of array \"" + size + "\"", true);
        return new Array(); // empty array
    }

    var ix, ptr;
    var arr = new Array();
    
    for(ix=0; ix<size; ix++, addr+=SIZEOF_PTR)
    {
        // read array[ix]
        if(!pcm_read_ptr(addr))
            break;
        // pointer to the nt_control object itself
        ptr = pcm.LastVariable_vValue;
        arr.push(ptr);
    }

    debug_print("The read_ptr_array_at \"" + addr.toString(16) + "\" returns " + arr.length + " entries", false);
    return arr;
}

function nt_symbols_init()
{
    var ix;
    
    nt_good = false;

    debug_print("parsing symbols...")
    pcm.DeleteAllScriptSymbols();

    modulesName = "nt_kernel_data.modules";
    if(!pcm_read_ptr(modulesName, SIZEOF_PTR))
        return false;
    var nt_modules_ptr = pcm.LastVariable_vValue;

    debug_print("NT modules address: 0x" + nt_modules_ptr.toString(16), false);

    // map nt_symbol type to the obtained pointer
    var tssvar = "_nt_module_data";
    if(!pcm_define_symbol(tssvar, nt_modules_ptr, "nt_module_data"))
        return false;

    if(!pcm_read_var("nt_kernel_data.modules_cnt", 1))
      return false;
    var modules_cnt = pcm.LastVariable_vValue;
    debug_print("Read nt_kernel_data.modules..." + modules_cnt);
    var arr_modules_ptrs = read_ptr_array_cnt(modulesName, modules_cnt);

    // any module?
    for(ix=0; ix<arr_modules_ptrs.length; ix++)
        new NTModule(modulesName + "["+ix+"]", arr_modules_ptrs[ix]);        


   if(!pcm_read_var("nt_kernel_data.controls_cnt", 1))
      return false;
    var controls_cnt = pcm.LastVariable_vValue;
    debug_print("read nt_kernel_data.controls..." + controls_cnt);

    var cptrvar = "nt_kernel_data.controls";
    var arr_controls_ptrs = read_ptr_array_cnt(cptrvar, controls_cnt);
    // any control installed?
    TSS_NUM_CONTROLS = arr_controls_ptrs.length;
    for(ix=0; ix<arr_controls_ptrs.length; ix++)
        new NTControl(cptrvar + "["+ix+"]", arr_controls_ptrs[ix]);        

    nt_good = arr_modules_ptrs.length > 0;
    return nt_good;
}

function fill_electrodes_table(tbl_id)
{
    var tbl = document.getElementById(tbl_id);
    if(!tbl)
        return;
        
    for(e in NTElectrode.All)
    {
        var el = NTElectrode.All[e];
        var row = tbl.insertRow(-1);
    
        for(col=0; col<4; col++)
        {
            var cell = row.insertCell(col);
            
            switch(col)
            {
            case 0: // name 
                cell.innerHTML = el.GetLink();
                break;
            case 1: // symbol
                cell.innerHTML = el._symbol;
                break;
            case 2: // module
                debug_print("TEST2", true);
                cell.innerHTML = el._module ? (el._module._name + ",<br>type=" + el._module._type) : "<i>none</i>";
                break;
            case 3:
                cell.innerHTML = el.GetGuiTag();
                break;
            }
        }    
    }
}

function fill_controls_table(tbl_id)
{
    var tbl = document.getElementById(tbl_id);
    if(!tbl)
        return;
        
    for(c in NTControl.All)
    {
        var ctl = NTControl.All[c];
        var row = tbl.insertRow(-1);
        var code = "on_select_control(NTControl.All[" + c + "])";
        
        for(col=0; col<4; col++)
        {
            var cell = row.insertCell(col);
            
            switch(col)
            {
            case 0: // name 
                cell.innerHTML = ctl.GetLink();
                break;
            case 1: // symbol
                cell.innerHTML = ctl._symbol;
                break;
            case 2: // electrodes
                cell.innerHTML = "";
                for(e=0; e<ctl._electrodes.length; e++)
                    cell.innerHTML += ctl._electrodes[e].GetLink() + ", ";
                break;
            case 3:
                cell.innerHTML = ctl.GetGuiTag();
                break;
            }
        }    
    }
}

var id_counter = 0;
function get_unique_id()
{
    id_counter++;
    return "uid" + id_counter;
}

function change_class_type(obj, classname)
{
    // remember that the object is typed dynamically
    obj._dynamically_typed = true;
    
    // take all prototype methods and memebrs
    for(var p in eval(classname + ".prototype"))
        obj[p] = eval(classname + ".prototype[p]")
    
    // call new class constructor passing it an existing pointer as "this"
    eval(classname + ".call(obj)");
}

function load_script_file(filename)
{
    // Adding the script tag to the head as suggested before
    var head = document.getElementsByTagName('head')[0];
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = filename;

    // Fire the loading
    head.appendChild(script);
}

function ToSign16(tmp)
{
  return tmp >= Math.pow(2,15)?(tmp-Math.pow(2,16)):tmp;
}

function ToSign32(tmp)
{
  return tmp >= Math.pow(2,31)?(tmp-Math.pow(2,32)):tmp;
}

function GetSizeFromType(varType)
{
  //check if parameter is valid number
  if(!isNaN(varType))
    return varType;
  //decode size from string
  if ((varType == "UINT8") || (varType == "SINT8"))
    return 1;
  else if ((varType == "UINT16") || (varType == "SINT16") || (varType == "UFRAC16") || (varType == "SFRAC16"))
    return 2;
  else if ((varType == "UINT32") || (varType == "SINT32") || (varType == "FLOAT") || (varType == "UFRAC32") || (varType == "SFRAC32"))
    return 4;
  else if ((varType == "UINT64") || (varType == "SINT64") || (varType == "DOUBLE"))
    return 8;
  else
    alert("GetSizeFromType(varType):Unsupported type of variable:"+varType);
};
