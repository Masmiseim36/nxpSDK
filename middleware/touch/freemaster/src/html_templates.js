
function AddHtmlObjVal(nodepath, id, type, val)
{
    finalArgs = [];
    //(baseid, id, type, val)
    finalArgs.push(nodepath);
    finalArgs.push(id);
    finalArgs.push(1); // add without separator //type 4 was removed
    finalArgs.push(val); // val
    return CallFunctStr("adddelget"+type, finalArgs);
}

function UpdateHtmlObjVal(nodepath, id, type, val)
{
    finalArgs = [];
    //(baseid, id, type, val)
    finalArgs.push(nodepath);
    finalArgs.push(id);
    finalArgs.push(3); // update label
    finalArgs.push(val); // val
    return CallFunctStr("adddelget"+type, finalArgs);
}

function DelHtmlObjVal(nodepath, id, type)
{
    finalArgs = [];
    //(baseid, id, type, val)
    finalArgs.push(nodepath);
    finalArgs.push(id);
    finalArgs.push(2); // del
    finalArgs.push(0); // val
    return CallFunctStr("adddelget"+type, finalArgs);
}

function GetHtmlObjVal(nodepath, type)
{
    finalArgs = [];
    finalArgs.push(nodepath);
    finalArgs.push(0); // read
    finalArgs.push(0); // val
    return CallFunctStr("getset"+type, finalArgs);
}

function SetHtmlObjVal(nodepath, type, val)
{
    finalArgs = [];
    finalArgs.push(nodepath);
    finalArgs.push(1); // write
    finalArgs.push(val); // val
    CallFunctStr("getset"+type, finalArgs);
    return 0;
}

function SetHtmlObjDefVal(nodepath, type)
{
    finalArgs = [];
    finalArgs.push(nodepath);
    finalArgs.push(9); // set default
    finalArgs.push(0); // val
    CallFunctStr("getset"+type, finalArgs);
    return 0;
}

function VisibilityHtmlObjVal(nodepath, type, val)
{
    finalArgs = [];
    //(baseid,type, val)
    finalArgs.push(nodepath);
    finalArgs.push(5); // visibility
    finalArgs.push(val); // val
    return CallFunctStr("getset"+type, finalArgs);
}

function UpdateContentHtmlObjVal(nodepath, type, val)
{
    finalArgs = [];
    //(baseid, id, type, val)
    finalArgs.push(nodepath);
    finalArgs.push(6); // update content
    finalArgs.push(val); // val
    return CallFunctStr("getset"+type, finalArgs);
}

function GetHtmlObjNum(nodepath, type)
{
    finalArgs = [];
    finalArgs.push(nodepath);
    finalArgs.push(7); // read number
    finalArgs.push(0); // val
    return CallFunctStr("getset"+type, finalArgs);
}

function SetHtmlObjNum(nodepath, type, val)
{
    finalArgs = [];
    finalArgs.push(nodepath);
    finalArgs.push(8); // write number
    finalArgs.push(val); // val
    CallFunctStr("getset"+type, finalArgs);
    return 0;
}


function selectItemByValue(elmnt, value)
{
  for(var i=0; i < elmnt.options.length; i++)
  {
    if(elmnt.options[i].value === value) {
      elmnt.selectedIndex = i;
      return 1;
      break;
    }
  }
  return 0
}

function ShowSubDiv(divobj, id)
{
  var divitems = divobj.childNodes;
  //list all items
  for(tmp in divitems)
  {
    //apply hide on objects only
    if(typeof divitems[tmp] == "object")
    {
      if(divitems[tmp].id)
      {
        if(divitems[tmp].id == id)
        {
          divitems[tmp].style.visibility = 'visible';
          divitems[tmp].style.display = 'block';
        }
      }
    }
  }
}

function HideSubDivs(divobj)
{
  var divitems = divobj.childNodes;
  //list all items
  for(tmp in divitems)
  {
    //apply hide on objects only
    if(typeof divitems[tmp] == "object")
    {
        //alert(divitems[tmp])
        divitems[tmp].style.visibility = 'hidden';
        divitems[tmp].style.display = 'none'; //'block'
    }
  }
}

function SelectTab(ulobj, id)
{
  var ulitems = ulobj.childNodes;

  var i=0;
  for(i=0; i<ulitems.length; i++)
  {
	if(typeof ulitems[i] != "object")
		continue;
	if(ulitems[i].id)
    {
      if(ulitems[i].id == id)
    	ulitems[i].className = "tag tag-arotary active-tag";
    }
  }
}

function DeselectTabs(ulobj)
{
	var ulitems = ulobj.childNodes;

	var i=0;
	for(i=0; i<ulitems.length; i++)
	{
	    /*if (ActivePage == i)
	        ulitems[i].className = ulitems[i].className = "selected";
	    else*/
	        ulitems[i].className = ulitems[i].className.replace(/\active-tag\b/g, '');
	    
	}
}

function UpdateliNames(namesarr)
{
    for(key in namesarr)
    {
        var obj = document.getElementById(key+"liname");
        obj.innerHTML = namesarr[key];
    }
}

var templates = {};

/*1 set val, 0: return val*/
function getsetobjectarrayext(pathid, type, val)
{
	var path = GetPathFromPathId(pathid)
	var id = GetIdFromPathId(pathid)
	var descrPathObj = GetDescrObj(path);
    var objpath = descrPathObj['objectpath'];
    var objnode = GetDescrObj(objpath);
    var endofname = objnode['structname'];

    var basepath = ReducePath(path, 1);
    var structname = getsettableedit(basepath+".sructname-"+id, 0/* read */, 0);
	return "&"+structname + "_" + endofname+"[0]";
}
templates["constcheckbox"] = "<form action=\"\" method=\"get\">"+
						      "<span id=\"checkboxitem{{nodepath}}span\" title=\"Enables runtine debugging\"><input id=\"checkboxitem{{nodepath}}input\"  type=\"checkbox\" class=\"touchsw-headline-checkbox\" name=\"config\" value=\"RAM\"> Place configuration to RAM </span>"+
						      "</form>";
templates["sticon"] = " <img src=\"./img/Blank.png\" class=\"table-line-state\" id = \"{{nodepath}}-{{rndid}}StatusImg\" alt = \"status\">";
templates["sticonimg"] = " <img src=\"./img/Blank.png\" class=\"param-state\" id = \"{{nodepath}}-{{rndid}}StatusImg\" alt = \"status\">";
//templates["RWButtons"] = "<input type=\"button\" value=\"Write\" onclick=\"OnClickWrite(this, '{{nodepath}}-{{rndid}}')\"> <input type=\"button\" value=\"Read\" onclick=\"OnClickRead(this, '{{nodepath}}-{{rndid}}')\">" + templates["sticon"];
templates["RWButtons"] = templates["sticon"] + "<button class=\"touchsw-table-button\" type=\"submit\" onclick=\"OnClickWrite(this, '{{nodepath}}-{{rndid}}')\">Write</button><button class=\"touchsw-table-button\" type=\"submit\" onclick=\"OnClickRead(this, '{{nodepath}}-{{rndid}}')\">Read</button>";
templates["RWButtonsimg"] = "<button class=\"touchsw-theme-button\" type=\"submit\" onclick=\"OnClickWrite(this, '{{nodepath}}-{{rndid}}')\">Write</button>"+
								 		"<button class=\"touchsw-theme-button\" type=\"submit\" onclick=\"OnClickRead(this, '{{nodepath}}-{{rndid}}')\">Read</button>" + templates["sticonimg"];

templates["label"] = "<hr><div id=\"{{nodepath}}\" ></div>{{label}}{{#constcheckbox}}"+templates["constcheckbox"]+"{{/constcheckbox}}";
templates["labelpart1"] = "<div class=\"touchsw-table-header\" id=\"{{nodepath}}-{{rndid}}\" >"+
						      "<p class=\"touchsw-table-headline\">{{label}}</p>"+
						      "{{#constcheckbox}}"+templates["constcheckbox"]+"{{/constcheckbox}}";
templates["labelpart2"] = "</div>";
templates["label"] = templates["labelpart1"] + templates["labelpart2"];

templates["label2"] = "<hr><div id=\"{{nodepath}}\" ></div>{{label}}";
templates["sublabel"] = "<br><div id=\"{{nodepath}}\" ></div>{{label}}";
templates["filedef2"] = "<tr {{#hidden}}style=\"display:none;\"{{/hidden}}><td>{{label}} </td><td><input type=\"text\" id=\"{{nodepath}}-{{rndid}}\" name=\"fname\" value=\"{{default}}\"></td><td><input type=\"button\" onclick=\"{{onclick}}('{{nodepath}}-{{rndid}}')\" value=\"{{btnname}}\"> {{#newwindow}} <button onclick=\"openfiledef(this, '{{nodepath}}-{{rndid}}');\">{{newwindowlabel}}</button>{{/newwindow}}"+templates["sticon"]+"</td></tr>";
templates["filedef"] = "<div class=\"touchsw-table-line\" {{#hidden}}style=\"display:none;\"{{/hidden}}>"+
                        "<p class=\"touchsw-table-name\">{{label}} </p>"+
                        "<input class=\"touchsw-table-input\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}\" value=\"{{default}}\"></input>"+
                        "<button class=\"touchsw-table-button\" onclick=\"{{onclick}}('{{nodepath}}-{{rndid}}')\">{{btnname}}</button>"+
                        "{{#newwindow}} <button class=\"touchsw-table-button\" onclick=\"openfiledef(this, '{{nodepath}}-{{rndid}}');\" title=\"{{newwindowhint}}\" alt=\"{{newwindowhint}}\">{{newwindowlabel}}</button>{{/newwindow}}"+
                        templates["sticon"]+"</div>";
                         

function getsetfiledef(id, type, val)//get:type = 0 , set: type= 1, label: type= 2, visibility: type= 3
{
	var obj = document.getElementById(id);
	if(obj){ 
		if(type == 1){ 
			obj.value = val; 
		}
		else if( type == 0)
		{ 
      return obj.value; 
		}
		else
		{
			debug_print("getsetfiledef: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	} 	
}
function openfiledef(obj, id)
{
  var name = getsetfiledef(id, 0, 0)+".c";
  window.open( name,'_blank');
  
  
  /*var oNewWin = window.open("about:blank", "newwindow", "");
  var paramFile = pcm.LocalFileOpen("src/"+name,"r");
  // read file content and store it in string
  var charNummber = pcm.LocalFileReadString(paramFile);
  var fileStatus = pcm.LocalFileClose(paramFile);
  var inputFileString = "";
  if(charNummber>0)
    inputFileString = pcm.LastLocalFile_string; //.split("\n");
  //alert(name + ", " + paramFile + ",   " + inputFileString + ",   " + charNummber)
  inputFileString = inputFileString.replace(/\n/g, "<br />");
  oNewWin.document.open();
  oNewWin.document.write(inputFileString);
  oNewWin.document.close();*/
}
templates["list"] = "<ul class=\"list\" id=\"{{nodepath}}\"></ul><br><br><br>";
templates["list"] = "<div class=\"tag-wrap\" id=\"{{nodepath}}\"></div>";
//templates["listitem"] = "<li class=\"selected\" id=\"{{rndid}}\"><a href=\"#{{nodepath}}\" id=\"{{nodepath}}\" onclick=\"incrementersel('{{nodepath}}', '{{rndid}}');\" title=\"{{label}}\">{{default}}</a><button style=\"color: #CCCCFF; background-color: #CC3333; font-weight: 700;\" onclick=\"incrementerdel('{{nodepath}}', '{{rndid}}');\">x</button></li>";
templates["listitem"] = "<a id=\"{{rndid}}liname\" onclick=\"incrementersel('{{nodepath}}', '{{rndid}}');\" title=\"{{label}}\">{{default}}</a><button style=\"color: #CCCCFF; background-color: #CC3333; font-weight: 700;\" onclick=\"incrementerdel('{{nodepath}}', '{{rndid}}', this);\">x</button> ";
templates["listitem"] = "<p class=\"tag-text\" id=\"{{rndid}}liname\" onclick=\"incrementersel('{{nodepath}}', '{{rndid}}');\" title=\"{{label}}\">ARotary_1</p>"+
									"<p class=\"erase-tag\" onclick=\"incrementerdel('{{nodepath}}', '{{rndid}}', this);\">x</p>"

function getsetlistitem(pathid, type, val) /*1 set val, 0: return val*/{ 
  var obj = document.getElementById(pathid+'liname'); 
  if(obj){ 
    if(type) {
      obj.title = val;
      obj.innerHTML = val;
    } 
    else
    {
      return obj.innerHTML;  
    } 
  }
}
//templates["checkboxarray"] = "<tr><td>{{label}}</td><td><span id=\"{{nodepath}}-{{rndid}}\"></span></td></tr>";
templates["checkboxarray"] = "<div class=\"touchsw-table-line\">"+
                              "<p class=\"touchsw-table-name\">{{label}}</p>"+
                              "<form class=\"electrodes-field\">"+
                              "<span id=\"{{nodepath}}-{{rndid}}\"></span>"+
                              "</form>"+
                            "</div>";
function adddelgetcheckboxarray(baseid, id, type, val)
{
	var obj = document.getElementById(baseid);
    if(obj)
    {
        if(type == 1)//add
        {
        	var path = GetPathFromPathId(baseid);
        	var baseobj = GetDescrObj(path);
        	if(baseobj['activetmpl'])
            {
                var spanobj = document.createElement("span");
            	var templateparams = {};
            	templateparams['value'] = val;
            	templateparams['rndid'] = id;
            	templateparams['onchangecbck'] = baseobj['onchangecbck']
            	templateparams['nodepath'] = path;
            	var str = Mustache.to_html(templates[baseobj['activetmpl']], templateparams);
                spanobj.innerHTML = str;
                spanobj.setAttribute("id", "checkboxitem"+id+"span");
                obj.appendChild(spanobj);
            }
        }else if (type == 2)//del
        {
        	var spanobj = document.getElementById("checkboxitem" + id + "span");
        	if(spanobj)
        	{
            	spanobj.outerHTML = "";
                delete spanobj;
        	}
        }else if (type == 3)//upd
        {
			getsetcheckboxitem(id, 2, val)
			/*var baseobj = GetDescrObj(baseid);
			if(baseobj['activetmpl'])
			{
				alert(type)
			}*/
        }
        else
        {
        	debug_print("adddelgetcheckboxarray: type\"" + type + "\"not implemented, "+baseid+", "+ id +".", true);
        }
    }

}
templates["checkboxitem"] = "<nobr><input class=\"chk\" type=\"checkbox\" id=\"checkboxitem{{rndid}}input\" name=\"{{value}}\" value=\"{{value}}\" {{opt}}/ {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}}><p id=\"checkboxitem{{rndid}}label\" for=\"checkboxitem{{rndid}}input\">{{value}}</p></nobr>";
function checkboxitemonchange(obj, id)
{
	if(!obj)
		return;

	var path = GetPathFromPathId(id);
	if(path == "")
		return;
	var rndid = GetIdFromPathId(id);
	var descrPathObj = GetDescrObj(path);
	var val = getsetcheckboxitem(rndid, 0, 0); //GetHtmlObjVal(rndid, descrPathObj["template"]);
	var controlBaseObj = GetDescrObj("nt_control");
	var elBaseObj = GetDescrObj("nt_electrode");
	if(!('nt_control_disabled' in elBaseObj))
		elBaseObj['nt_control_disabled'] = {};
	if(!('nt_electrode_disabled' in elBaseObj))
		elBaseObj['nt_electrode_disabled'] = {};
	
	var arr = rndid.split("_");
	var id1 = arr[0];
	var id2 = arr[1];

	if(val)
	{
		controlcheckboxitem(rndid, controlBaseObj['links'], elBaseObj['links'], 0, id2);
		controltableeditshilding(rndid, elBaseObj['links'], elBaseObj['links'], 0, id2)
		elBaseObj['nt_control_disabled'][rndid] = id2;
		elBaseObj['nt_electrode_disabled'][rndid] = id2;
	}
	else
	{
		controlcheckboxitem(rndid, controlBaseObj['links'], elBaseObj['links'], id2, 0);
		controltableeditshilding(rndid, elBaseObj['links'], elBaseObj['links'], id2, 0);
		delete elBaseObj['nt_control_disabled'][rndid];
		delete elBaseObj['nt_electrode_disabled'][rndid];
	}
}

function EnableAllElCheckboxes(path) //nt_control.electrodes
{
	if(path == "")
		return;
	var controlBaseObj = GetDescrObj("nt_control");
	var elBaseObj = GetDescrObj("nt_electrode");
	if(!('nt_control_disabled' in elBaseObj))
		elBaseObj['nt_control_disabled'] = {};
	if(!('nt_electrode_disabled' in elBaseObj))
		elBaseObj['nt_electrode_disabled'] = {};
  if((elBaseObj) && (controlBaseObj))
  {
    var elid = 0
    //list all link in electrodes, links in controls
    for(linkindex in elBaseObj['links'])
    {
      if(elBaseObj['links'][linkindex])
      {
      	for(id in elBaseObj['links'][linkindex])
				{
					elid = id;
					break;
				}
      }
      controlcheckboxitem(0, controlBaseObj['links'], elBaseObj['links'], elid, 0);
    }
  }
}

function getsetcheckboxitem(id, type, val)//get:type = 0 , set: type= 1, label: type= 2, visibility: type= 3    val=0 unchecked editable, val=1 checked editable, val=2 unchecked disabled, val=3 checked disabled
{
    var spanobj = document.getElementById("checkboxitem"+id+"span");

    if(spanobj)
    {
        if((type == 1)|| (type == 8))//set
        {
            var inputobj = document.getElementById("checkboxitem"+id+"input")
            if(inputobj)
            {
            	if(val&1)
            		inputobj.checked = true;
            	else
            		inputobj.checked = false;
                if(val&2)
                    inputobj.disabled = true;
                else
                    inputobj.disabled = false;
            }
        }else if(type == 2)//update label
        {

        	var obj = document.getElementById("checkboxitem"+id+"label")
        	if(obj)
        		obj.innerHTML = val;
        	obj = document.getElementById("checkboxitem"+id+"input")
        	if(obj)
        	{
        		obj.value = val;
        		obj.name = val;
        	}
        }else if(type == 5)//update visibility
        {
            var inputobj = document.getElementById("checkboxitem"+id+"input")
            if(inputobj)
            {
                if(val)
                    inputobj.disabled = true;
                else
                    inputobj.disabled = false;
            }

        }else if((type == 0) || (type == 7))
        {
            //type == 0, read
        	var inputobj = document.getElementById("checkboxitem"+id+"input")
            if(inputobj)
            {
                var outval = 0;
            	if(inputobj.checked)
            		outval |= 1;
                if(inputobj.disabled)
                    outval |= 2;
                return outval;
            }
        }
        else
        {
        	debug_print("getsetcheckboxitem: type\"" + type + "\"not implemented, "+id+".", true);
        }
    }
    return 0;
}

templates["tablediv"] = "<div id=\"{{nodepath}}\"></div>";
templates["tablestart"] = "<div class=\"touchsw-table\" id=\"{{nodepath}}\">";

templates["subtablelabel"] = "<tr {{#hidden}}style=\"display:none;\"{{/hidden}} id=\"{{nodepath}}-{{rndid}}tr\"><td colspan = \"2\">{{label}}</td></tr>";
//templates["tableedit"] = "<tr {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\"><td {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</td><td>
//"<input type=\"text\" {{#descr}}title=\"{{descr}}\" {{/descr}} name=\"fname\" id=\"{{nodepath}}-{{rndid}}val\" value=\"{{default}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\')',1);\"{{/onchangecbck}}> {{#rwbase}}"+templates["RWButtons"]+"{{/rwbase}}</td></tr>";
templates["tableedit"] = "<div class=\"touchsw-table-line\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
						       "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
						       "<input class=\"touchsw-table-input\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}val\" value=\"{{default}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\')',1);\"{{/onchangecbck}}></input>"+
						       "{{#rwbase}}"+templates["RWButtons"]+"{{/rwbase}}"+
                   "</div>";
function getsettableedit(id, type, val){ 
	var obj = document.getElementById(id+"val"); 
	var path = GetPathFromPathId(id); 
	var descrPathObj = GetDescrObj2(path); 
	var validateNumber = false;
	if(descrPathObj)
	{
		if(('minlimit' in descrPathObj)&&('maxlimit' in descrPathObj))
			validateNumber = true;
	}
	if(obj){ 
		if((type == 1)||(type == 8)){ 
			obj.value = val; 
		}
		else if((type == 0)||(type == 7))
		{ 
			if(validateNumber)
			{
				if(Number(obj.value)<Number(descrPathObj['minlimit']))
					obj.value = descrPathObj['minlimit'];
				else if(Number(obj.value)>Number(descrPathObj['maxlimit']))
					obj.value = descrPathObj['maxlimit'];
			}
			return obj.value; 
		}if(type == 9)
    {
      obj.value = descrPathObj['default'];
    }
    else
		{
			debug_print("getsettableedit: type\"" + type + "\"not implemented, "+id+".", true);
		}
	} 
}

templates["tableeditimg"] = "<div class=\"parameter\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
										"<p class=\"parameter-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
										"<input name=\"parameter\" required='' type='text' class=\"popup-input\" id=\"{{nodepath}}-{{rndid}}val\" value=\"{{default}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\')',1);\"{{/onchangecbck}} />"+
								 		"{{#rwbase}}"+templates["RWButtonsimg"]+"{{/rwbase}}"+
									"</div>"+
									"<div class=\"cistic\"></div>";
function getsettableeditimg(id, type, val) {return getsettableedit(id, type, val);}

function GetVarStr(obj, nodeid)
{
	var path = GetPathFromPathId(nodeid);
	var rndid = GetIdFromPathId(nodeid);
	var basepath = ReducePath(path, 1);
	var descrPathObj = GetDescrObj(path);
	var descrBasePathObj = GetDescrObj(basepath);
	var descrStrNameObj = GetDescrObj(basepath+".sructname");
//	var strname = GetHtmlObjVal(basepath+".sructname"+"-"+rndid, descrStrNameObj['template']);
	var patharr = path.split(".");
	var strname = descrPathObj['rwbase']+patharr[1];

	if(basepath.indexOf("nt_control") == 0)
	{
		var linkedpath = "";
		//get linked path
		if("links" in descrBasePathObj)
		{
			for(index in descrBasePathObj["links"])
			{
				for(id in descrBasePathObj["links"][index])
				{
					linkedpath = descrBasePathObj["links"][index][id]
					break;
				}
			}
		}
		var linekdbasepath = ReducePath(linkedpath, 2);
		var descrLinkedBasePathObj = GetDescrObj(linekdbasepath);
		var cnt = 0;
		if("links" in descrLinkedBasePathObj)
		{
			for(index in descrLinkedBasePathObj["links"])
			{
				for(id in descrLinkedBasePathObj["links"][index])
				{
					if(id == rndid)
					{
						cnt = index;
						break;
					}
				}
			}
		}
		strname = strname.replace("replace1", String(cnt));
	}else if(basepath == "nt_electrode")
	{
		var cnt = 0;
		//get linked path
		if("links" in descrBasePathObj)
		{
			for(index in descrBasePathObj["links"])
			{
				for(id in descrBasePathObj["links"][index])
				{
					if(id == rndid)
					{
						cnt = index;
						break;
					}
				}
			}
		}
		
		strname = strname.replace("replace1", 0);
		strname = strname.replace("replace2", String(cnt));
	}
	else if (basepath.indexOf("nt_keydetector") == 0)
	{
		var basepath = ReducePath(path, 8);
		var descrPathObj = GetDescrObj2(path);
		strname = descrPathObj['rwbase']+patharr[1];
		if(patharr.length>2)
			strname = strname + "."+patharr[2];
		var descrBasePathObj = GetDescrObj(basepath);
		var cnt = 0;
		//get linked path
		if("links" in descrBasePathObj)
		{
			for(index in descrBasePathObj["links"])
			{
				for(id in descrBasePathObj["links"][index])
				{
					if(id == rndid)
					{
						cnt = index;
						break;
					}
				}
			}
		}
		
		strname = strname.replace("replace1", 0);
		strname = strname.replace("replace2", String(cnt));
	}
	else if ((basepath.indexOf("tsi_config_t") == 0) || (basepath.indexOf("cs_config_t") == 0))
	{
		var basepath = ReducePath(path, 8);
		var descrPathObj = GetDescrObj2(path);
		pathpart = path.replace(patharr[0]+".", "");
		strname = descrPathObj['rwbase']+pathpart;
		strname = strname.replace("replace1", 0);
		
	}
	return {name:strname, template:descrPathObj['template']};
}


function OnClickWrite(obj, nodeid)
{
	var out = GetVarStr(obj, nodeid);
	SetStatusLineRaw(nodeid, STATUS_CMD_SENDING, "Writing var.");
	var strname = out.name;
	if((nodeid.indexOf("nt_control_proxi.gesture") >=0) || (nodeid.indexOf("nt_control_keypad.multi_touch") >=0) || (nodeid.indexOf("nt_control_keypad.groups") >=0))
	{
		var i = 0;
		var ptr = 0;
		if(pcm_read_ptr(strname))
			ptr = pcm.LastVariable_vValue;
		if(ptr<0x800000)
		{
			var msg = "Variable "+strname+" is probably read only on address 0x"+ptr+".";
			debug_print(msg, true);
			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
			return false;
		}
		var val = GetHtmlObjNum(nodeid, out.template, 0);
		val = val.replace("uint32_t", "");
		var res = val.replace(/[^0-9a-fA-F.,|x|X]/g, '');
		arr = res.split(",");
		cnt = (arr.length-1);
		var failed = 0;
		for(j=0; j<cnt; j++)
		{
			i=j+Number(1);
			if(!pcm_write_var(ptr+Number(0), 4, Number(arr[0+(Number(j))])))
				failed = 1;
			if(failed)
			{
				var msg = "Failed write "+strname + ", check connection. Reported error:"+pcm.LastRetMsg;
				debug_print(msg, true);
				SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
				return false;
			}
			ptr=ptr + Number(4);
		}
	}
	else if(nodeid.indexOf("nt_control_matrix.gesture") >=0)
	{
		var i = 0;
		var ptr = 0;
		if(pcm_read_ptr(strname))
			ptr = pcm.LastVariable_vValue;
		if(ptr<0x800000)
		{
			var msg = "Variable "+strname+" is probably read only on address 0x"+ptr+".";
			debug_print(msg, true);
			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
			return false;
		}
		var val = GetHtmlObjNum(nodeid, out.template, 0);
		val = val.replace("nt_points64", "");
		var res = val.replace(/[^0-9a-fA-F.,|x|X]/g, '');
		arr = res.split(",");
		cnt = ((arr.length-1)/4);
		var failed = 0;
		for(j=0; j<cnt; j++)
		{
			i=j+Number(1);
			if(!pcm_write_var(ptr+Number(0), 8, Number(arr[0+(Number(j)*4)])))
				failed = 1;
			if(!pcm_write_var(ptr+Number(8), 8, Number(arr[1+(Number(j)*4)])))
				failed = 1;
			if(!pcm_write_var(ptr+Number(16), 8, Number(arr[2+(Number(j)*4)])))
				failed = 1;
			if(!pcm_write_var(ptr+Number(24), 8, Number(arr[3+(Number(j)*4)])))
				failed = 1;
			if(failed)
			{
				var msg = "Failed write "+strname + ", check connection. Reported error:"+pcm.LastRetMsg;
				debug_print(msg, true);
				SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
				return false;
			}
			ptr=ptr + Number(32);
		}
	}
	else
	{
		if(!pcm_get_symbol_info(strname))
		{
			var msg = "Symbol "+strname+" not found."+pcm.LastRetMsg;
			debug_print(msg, true);
			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
			return false;
		}
		
		if(pcm.LastSymbolInfo_addr<0x800000)
		{
			var msg = "Variable "+strname+" is probably read only on address 0x"+pcm.LastSymbolInfo_addr.toString(16)+".";
			debug_print(msg, true);
			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
			return false;
		}
		var val = GetHtmlObjNum(nodeid, out.template, 0);
		if(!pcm_write_var(strname, pcm.LastSymbolInfo_size, val))
		{
			var msg = "Failed write "+strname + ", check connection. Reported error:"+pcm.LastRetMsg;
			debug_print(msg, true);
			SetStatusLineRaw(nodeid, STATUS_ERROR, msg);
			return false;
		}
	}
	var msg = "Write to variable "+strname+"...0x"+pcm.LastSymbolInfo_addr.toString(16)+" ("+nodeid+") was successful.";
	debug_print(msg, false);
	SetStatusLineRaw(nodeid, STATUS_OK, msg);
}

function OnClickRead(obj, nodeid)
{
	SetStatusLineRaw(nodeid, STATUS_CMD_SENDING, "Reading var.");
	var out = GetVarStr(obj, nodeid);
	var strname = out.name;
	var data = 0;
  
	if((nodeid.indexOf("nt_control_proxi.gesture") >=0) || (nodeid.indexOf("nt_control_keypad.multi_touch") >=0) || (nodeid.indexOf("nt_control_keypad.groups") >=0))
	{
		var i = 0;
		var ptr = 0;
		if(pcm_read_ptr(strname))
			ptr = pcm.LastVariable_vValue;
		if(ptr)
		{
			data = "(uint32_t []){";
			for(i=0; i<20; i++)
			{
				var f1x=0;
				if(pcm_read_var(ptr+Number(0), 4))
					f1x = pcm.LastVariable_vValue;
				if(f1x==0)
					break;
				var str = "0x"+("0000000"+f1x.toString(16)).slice(-8);
				data+=str+" ,";
				ptr=ptr+Number(4);
			}
			data+="0}";
		}
	}
	else if(nodeid.indexOf("nt_control_matrix.gesture") >=0)
	{
		var i = 0;
		var ptr = 0;
		if(pcm_read_ptr(strname))
			ptr = pcm.LastVariable_vValue;
		if(ptr)
		{
			data = "(nt_points64 []){";
			for(i=0; i<20; i++)
			{
				var f1x=0;
				var f1y=0;
				var f2x=0;
				var f2y=0;
				if(pcm_read_var(ptr+Number(0), 8))
					f1x = pcm.LastVariable_vValue;
				if(pcm_read_var(ptr+Number(8), 8))
					f1y = pcm.LastVariable_vValue;
				if(pcm_read_var(ptr+Number(16), 8))
					f2x = pcm.LastVariable_vValue;
				if(pcm_read_var(ptr+Number(24), 8))
					f2y = pcm.LastVariable_vValue;
				if((f1x==0)&&(f1y==0)&&(f2x==0)&&(f2y==0))
					break;
				var str = "{{ 0x"+("000000000000000"+f1x.toString(16)).slice(-16)+",0x"+("000000000000000"+f1y.toString(16)).slice(-16)+"}, { 0x"+("000000000000000"+f2x.toString(16)).slice(-16)+",0x"+("000000000000000"+f2y.toString(16)).slice(-16)+"}}"
				data+=str+" ,";
				ptr=ptr+Number(32);
			}
			data+="{{0x0,0x0},{0x0,0x0}}}"
		}
	}
	else
	{
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
		data = pcm.LastVariable_vValue;
	}
	
	var val = SetHtmlObjNum(nodeid, out.template, data);
	var msg = "Read from variable "+strname+"...0x"+pcm.LastSymbolInfo_addr.toString(16)+" ("+nodeid+") was successful.";
	debug_print(msg, false);
	SetStatusLineRaw(nodeid, STATUS_OK, msg);
}
templates["tableeditshilding"] = "<tr><td {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</td><td><select id=\"{{nodepath}}-{{rndid}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}} {{#onfocuscbck}} onmouseover=\"{{onfocuscbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onfocuscbck}} ><option title=\"NULL\">NULL</option>{{#default}}<option {{#subrndid}}id=\"{{nodepath}}-{{rndid}}-{{subrndid}}\"{{/subrndid}} {{#descr}}title=\"{{descr}}\" {{/descr}}value=\"{{item}}\">{{sublabel}}</option>{{/default}}</select></td></tr>";
templates["tableeditshilding"] = "<div class=\"touchsw-table-line\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}} {{#onfocuscbck}} onmouseover=\"{{onfocuscbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onfocuscbck}}>"+
											"<option title=\"NULL\">NULL</option>"+
											"{{#default}}<option value=\"\" class=\"selection-name\" {{#subrndid}}id=\"{{nodepath}}-{{rndid}}-{{subrndid}}\"{{/subrndid}} {{#descr}}title=\"{{descr}}\" {{/descr}}value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>"+
							"</div>";

function controlcheckboxitem(itemid, controllinksarr, electrodelinksarr, enid, disid)
{
	// TODO add to nt_electrode list of disabled electrodes in controls and in shielding
	//itemid - ID to not touch
	//controllinksarr .. list of base items(controls/electrodes)
	//electrodelinksarr ..list of sub items(electrodes/electrodes)
	//enid .. id to enable
	//disid .. id to disable
	var mode = GetModeFromWeb("modeid-1_form", "modeid-1rb");
	for(ctrllinks in controllinksarr)
	{
		for(ctrlid in controllinksarr[ctrllinks])
		{
			var ctrlid_loc = ctrlid;
			for(ellinks in electrodelinksarr)
			{
				for(elid in electrodelinksarr[ellinks])
				{
					var elid_loc = elid;
					if((itemid!=(ctrlid_loc + "_"+elid_loc))&&((elid_loc == enid)||(elid == disid)))
    			{
						if(Number(mode))
            {
              VisibilityHtmlObjVal(ctrlid_loc + "_"+elid_loc, "checkboxitem", 0);
            }
            else
            {
              if(elid_loc == enid)
    						VisibilityHtmlObjVal(ctrlid_loc + "_"+elid_loc, "checkboxitem", 0);
    				  else if(elid_loc == disid)
    						VisibilityHtmlObjVal(ctrlid_loc + "_"+elid_loc, "checkboxitem", 1);
            }
    			}
				}
			}
			
		}
	}
}
function tableeditshildingonchange(obj, id)
{
	if(!obj)
		return;
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
	var descrObj = GetDescrObj(path);
	var basepath = ReducePath(path, 1);
	var descrBaseObj = GetDescrObj(basepath);
	var obj = document.getElementById(id); 
	if(obj)
	{ 
		var val = obj.options[obj.selectedIndex].value;
		var iditem = obj.options[obj.selectedIndex].id;
		var arr = [0,0];
		var idnew1 = 0;
		var idnew2 = 0;

		if(iditem != "")
		{
			arr = iditem.split("_");
			idnew1 = arr[0].replace("shiledingitem","");
			idnew2 = arr[1].replace("opt","");
		}
		var arrlast = [0,0];
		var idlast1 = 0;
		var idlast2 = 0;
		if('lastid' in descrObj)
		{
			if(descrObj['lastid'] != "")
			{
				arrlast = descrObj['lastid'].split("_");
				idlast1 = arrlast[0].replace("shiledingitem","");
				idlast2 = arrlast[1].replace("opt","");
			}
		}
		var lasttext = "NULL";
		if('lasttext' in descrObj)
			lasttext = descrObj['lasttext'];
		if(!('nt_control_disabled' in descrBaseObj))
			descrBaseObj['nt_control_disabled'] = {};
		var controlBaseObj = GetDescrObj("nt_control");
		if(lasttext == 'NULL')
		{
			controlcheckboxitem(0, controlBaseObj['links'], descrBaseObj['links'], 0, idnew2);
			descrBaseObj['nt_control_disabled'][idnew1+"_"+idnew2] = idnew2;
		}
		else if(val == 'NULL')
		{
			controlcheckboxitem(0, controlBaseObj['links'], descrBaseObj['links'], idlast2, 0);
			delete descrBaseObj['nt_control_disabled'][idlast1+"_"+idlast2];
		}
		else
		{
			controlcheckboxitem(0, controlBaseObj['links'], descrBaseObj['links'], idlast2, idnew2);
			descrBaseObj['nt_control_disabled'][idnew1+"_"+idnew2] = idnew2;
			delete descrBaseObj['nt_control_disabled'][idlast1+"_"+idlast2];
		}
	}
	//when last not null, enable all checkboxes/items in enum for last electrode, disable newly selected electrode in checboxes and items in enums 
}
//todo implement funct for checkboxes on change

function getsettableeditshilding(id, type, val)
{ 
	var obj = document.getElementById(id); 
	if(obj)
	{ 
		if(type == 1)
		{
			if(val.indexOf("&") == 0) 
				selectItemByValue(obj, val.substring(1));
			else
				selectItemByValue(obj, val);
		}
		else if(type == 5)//visibility
		{
			if(val.indexOf("&") == 0) 
				selectItemByValue(obj, val.substring(1));
			else
				selectItemByValue(obj, val);
		}
		else if(type == 0)
		{
			if(obj.options[obj.selectedIndex].value == "NULL")
				return obj.options[obj.selectedIndex].value;
			else
				return "&"+obj.options[obj.selectedIndex].value;
		}
		else
		{
			debug_print("getsettableeditshilding: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	}
	else if(type == 5)
	{
		obj = document.getElementById("shiledingitem"+id+"opt");
		if(obj)
		{
			if(val)
				obj.disabled = true;
			else
				obj.disabled = false;
		}
	}
}

function controltableeditshilding(itemid, electrodebaselinksarr, electrodelinksarr, enid, disid)
{
	for(elbaselinks in electrodebaselinksarr)
	{
		for(elbaseid in electrodebaselinksarr[elbaselinks])
		{
			var elbaseid_loc = elbaseid;
			for(ellinks in electrodelinksarr)
			{
				for(elid in electrodelinksarr[ellinks])
				{
					var elid_loc = elid;
					if(((elid_loc == enid)||(elid == disid)))
    				{
						if(elid_loc == enid)
    						VisibilityHtmlObjVal(elbaseid_loc + "_"+elid_loc, "tableeditshilding", 0);
    					else if(elid_loc == disid)
    						VisibilityHtmlObjVal(elbaseid_loc + "_"+elid_loc, "tableeditshilding", 1);
    				}
				}
			}
		}
	}
}

function GetEllIdByNametableeditshilding(baseobj, elname)
{
	
	for(elbaselinks in baseobj['links'])
	{
		for(elbaseid in baseobj['links'][elbaselinks])
		{
			var structnameobj = baseobj['sructname']
			var readelname = GetHtmlObjVal(structnameobj['nodepath']+"-"+elbaseid, structnameobj['template']);
			if(("&" + readelname) == elname)
				return elbaseid;
			
		}
	}
	return 0;
}

function adddelgettableeditshilding(baseid, id, type, val)
{
	var obj = document.getElementById(baseid);
    if(obj)
    {
        if(type == 1)//add
        {
        	var rndid = GetIdFromPathId(baseid);
        	if(String(rndid+"_"+rndid) != id)
        	{
        		var optionobj = document.createElement("option");
        		optionobj.text = val;
        		optionobj.setAttribute("id", "shiledingitem"+id+"opt");
                obj.appendChild(optionobj);
        	}
        }else if (type == 2)//del
        {
        	var spanobj = document.getElementById("shiledingitem" + id + "opt");
        	if(spanobj)
        	{
            	spanobj.outerHTML = "";
                delete spanobj;
        	}
        }else if (type == 3)//upd
        {
        	var spanobj = document.getElementById("shiledingitem" + id + "opt");
        	if(spanobj)
        		spanobj.text = val;
        }
        else
        {
        	debug_print("getsettableeditshilding: type\"" + type + "\"not implemented, "+ baseid +", "+ id +".", true);
        }
    }
}
templates["tableenum"] = "<div class=\"touchsw-table-line\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\">"+
											"{{#default}}<option class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}} value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>{{#rwbase}}"+templates["RWButtons"]+"{{/rwbase}}"+
							"</div>";

//pridat enum baset template, kteery vytvori 2 polozky v tabulce - sloucene 2 bunky pro nazev a sloucene 2 bunky pro sub tabulku. pak by se melo to ID a funkce zaregistrovat, aby se mohl obsah obnovit i loadcfg funkci. 
function getsettableenum(id, type, val)/*1 set val, 0: return val*/
{ 
	var obj = document.getElementById(id);
	var path = GetPathFromPathId(id); 
	var descrPathObj = GetDescrObj2(path);
	if(obj)
	{ 
		if(type == 1)
		{
			selectItemByValue(obj, val); 
		}
		else if(type == 6)
		{
			obj.outerHTML = val;
		}
		else if(type == 0)
		{
			return obj.options[obj.selectedIndex].value;  
		}
		else if(type == 7)
		{
			if("default" in descrPathObj)
			{
				if(descrPathObj["default"][obj.selectedIndex]["intval"])
					return descrPathObj["default"][obj.selectedIndex]["intval"];
			}
			return obj.selectedIndex;
		}
		else if(type == 8)
		{
			if("default" in descrPathObj)
			{
				if(descrPathObj["default"][0]["intval"])
				{
					for(key in descrPathObj["default"])
					{
						if(descrPathObj["default"][key]["intval"] == Number(val))
						{
							obj.selectedIndex = key;
							return;
						}
					}
				}
			}
			obj.selectedIndex = Number(val);
		}else if(type == 9)
    {
			var path = GetPathFromPathId(id); 
			var descrPathObj = GetDescrObj2(path);
      if(descrPathObj)
      {
        var i = 0;
        for(i = 0; i<descrPathObj["default"].length; i++)
        {
          if("selected" in descrPathObj["default"][i])
            break;
        }
        if(i)
          obj.selectedIndex = i;
      } 

    }
		else
		{
			debug_print("getsettableenum: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	} 
}

templates["popupenum"] = templates["tableenumimg"] = "<div class=\"parameter\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"parameter-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}}>"+
											"{{#default}}<option class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}} value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>{{#rwbase}}"+templates["RWButtonsimg"]+"{{/rwbase}}"+
							"</div>";
function getsetpopupenum(id, type, val)/*1 set val, 0: return val*/
{
  return getsettableenum(id, type, val);
}

templates["tableenumsubstrshareditem"] = "<div class=\"touchsw-table-line\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}} {{#onfocuscbck}} onmouseover=\"{{onfocuscbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onfocuscbck}}>"+
											"{{#default}}<option class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}} value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>{{#rwbase}}"+templates["RWButtons"]+"{{/rwbase}}"+
							"</div>";

function adddelgettableenumsubstrshareditem(baseid, id, type, val)
{
	var obj = document.getElementById(baseid);
    if(obj)
    {
        if(type == 1)//add
        {
        	var rndid = GetIdFromPathId(baseid);
        	if(String(rndid+"_"+rndid) != id)
        	{
        		var optionobj = document.createElement("option");
        		optionobj.text = val;
        		optionobj.setAttribute("id", "tableenumsubstrshareditem"+id+"opt");
                obj.appendChild(optionobj);
        	}
        }else if (type == 2)//del
        {
        	var spanobj = document.getElementById("tableenumsubstrshareditem" + id + "opt");
        	if(spanobj)
        	{
            	spanobj.outerHTML = "";
                delete spanobj;
        	}
        }else if (type == 3)//upd
        {
        	var spanobj = document.getElementById("tableenumsubstrshareditem" + id + "opt");
        	if(spanobj)
        		spanobj.text = val;
        }
        else
        {
        	debug_print("getsettableeditshilding: type\"" + type + "\"not implemented, "+ baseid +", "+ id +".", true);
        }
    }
}

function onchangetableenumsubstrshareditem(obj, id)
{
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
	var descrPathObj = GetDescrObj(path);
    var selectedElName = GetHtmlObjVal(id, descrPathObj['template']);
    selectedElName = selectedElName.replace("&", "");
	var basepath = ReducePath(path, 1);
	var elbasePathObj = GetDescrObj(basepath);

	//disable this electrode in the shared conf option from other electrodes
    for(linkindex in elbasePathObj['links'])
    {
        var elid = GetLinkId(elbasePathObj['links'][linkindex])
        var idElName = GetHtmlObjVal(basepath+".sructname-"+elid, elbasePathObj['sructname']['template']);
    	//if enum found report the insex
    	if(rndid != elid)
    	{
            if((descrPathObj['lasttext']  == 'None') && (selectedElName != 'None'))
            {
            	VisibilityHtmlObjVal(elid + "_" + rndid, descrPathObj['template'], 1);
            	if(!(rndid in elbasePathObj["keydetector_interface_electrode_disabled"]))
            		elbasePathObj["keydetector_interface_electrode_disabled"].push(rndid);
            }
            else if(selectedElName == 'None')
            {
            	VisibilityHtmlObjVal(elid + "_" + rndid, descrPathObj['template'], 0);
            	if(rndid in elbasePathObj["keydetector_interface_electrode_disabled"])
            		elbasePathObj["keydetector_interface_electrode_disabled"].splice(rndid,1);
            }
            else
            {
            	VisibilityHtmlObjVal(elid + "_" + rndid, descrPathObj['template'], 1);
            	if(!(rndid in elbasePathObj["keydetector_interface_electrode_disabled"]))
            		elbasePathObj["keydetector_interface_electrode_disabled"].push(rndid)
            }
    	}
    }
    //todo hide/show keydet config for this rndid
    var keytype = GetHtmlObjNum(basepath+".keydetector_interface-"+rndid, elbasePathObj['keydetector_interface']['template']);
    var keydettypepath = elbasePathObj['keydetector_interface']['default'][keytype]['inclsubstructenum']
    var keydetObj = GetDescrObj(keydettypepath);
    if(keydetObj)
    {
    	if (!('links' in keydetObj))
    		keydetObj['links'] = [];
		var idfound = -1;
		for(linkindex in keydetObj['links'])
		{
			var id = GetLinkId(keydetObj['links'][linkindex])
			if(id == rndid)
			{
				idfound = linkindex;
				keydetObj['links'].splice(linkindex,1);
				
			}
		}
		if(selectedElName == 'None')
		{
			//if(idfound == -1)
			{
				link = {};
				link[rndid] = basepath+".keydetector_interface.tablediv";
				keydetObj["links"].push(link);
			}
		}
    }
    var type = '';
    if(selectedElName != 'None')
    	type = 'none';
    
    var obj = document.getElementById(basepath+".keydetector_interface-"+rndid+"tr");
    if(obj)
      obj.style.display = type;
    obj = document.getElementById(basepath+".keydetector_interface-"+rndid+"substr");
    if(obj)
      obj.style.display = type;

}

function onfocustableenumsubstrshareditem(obj, id)
{
	var obj = document.getElementById(id);
	enumonfocus(obj, id);
}

function getsettableenumsubstrshareditem(id, type, val)
{
	var obj = document.getElementById(id); 
	if(obj)
	{ 
		if(type == 1)
		{
			if(val.indexOf("&") == 0) 
				selectItemByValue(obj, val.substring(1));
			else
				selectItemByValue(obj, val);
		}
		else if(type == 5)//visibility
		{
			if(val.indexOf("&") == 0) 
				selectItemByValue(obj, val.substring(1));
			else
				selectItemByValue(obj, val);
		}
		else if(type == 0)
		{
			if(obj.options[obj.selectedIndex].value == "NULL")  
				return obj.options[obj.selectedIndex].value;
			else
				return "&"+obj.options[obj.selectedIndex].value;
		}
		else
		{
			debug_print("getsettableenumsubstrshareditem: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	}
	else if(type == 5)
	{
		obj = document.getElementById("tableenumsubstrshareditem"+id+"opt");
		if(obj)
		{
			if(val)
				obj.disabled = true;
			else
				obj.disabled = false;
		}
	}
}

templates["tableenumsubstr"] = "<div class=\"touchsw-table-line\" id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\"  {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}} {{#onfocuscbck}} onmouseover=\"{{onfocuscbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onfocuscbck}}>"+
											"{{#default}}<option value=\"{{item}}\" class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>"+
							"</div><div id=\"{{nodepath}}-{{rndid}}substr\"></div>";
function getsettableenumsubstr(id, type, val) { return getsettableenum(id, type, val);}

templates["popupenum"] = templates["tableenumimg"] = "<div class=\"parameter\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"parameter-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onchangecbck}}>"+
											"{{#default}}<option class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}} value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>{{#rwbase}}"+templates["RWButtonsimg"]+"{{/rwbase}}"+
							"</div>";

templates["tableenummode"] = "<div class=\"parameter\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									 "<p class=\"parameter-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"select-wrap\">"+
										"<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\" onchange=\"onchangetableenummode(this, '{{nodepath}}-{{rndid}}')\">"+
											"{{#default}}<option value=\"{{item}}\" class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}value=\"{{item}}\"  {{selected}}>{{sublabel}}</option>{{/default}}"+
										"</select>"+
									"</div>{{#rwbase}}"+templates["RWButtonsimg"]+"{{/rwbase}}"+
								"</div>";
//pridat enum baset template, ktery vytvori 2 polozky v tabulce - sloucene 2 bunky pro nazev a sloucene 2 bunky pro sub tabulku. pak by se melo to ID a funkce zaregistrovat, aby se mohl obsah obnovit i loadcfg funkci. 
function getsettableenummode(id, type, val)/*1 set val, 0: return val*/
{
  if((type == 6) || (type == 8) || (type == 1))
  {
       onchangetableenummode(0, id);
  }
  return getsettableenum(id, type, val)
}

function onchangetableenummode(obj, id)/*1 set val, 0: return val*/
{
  //get the index
  var index = getsettableenum(id, 7, 0)
	var path = GetPathFromPathId(id);
  var rndid = GetIdFromPathId(id);
	var descrPathObj = GetDescrObj2(path);

  if(descrPathObj)
  {
    if(descrPathObj.controlitem)
    {
      for(var item = 0; item<descrPathObj.controlitem.length; item++  )
      {
        var tr_obj = document.getElementById(descrPathObj.controlitem[item].item+'-'+rndid+'tr');
        if(tr_obj)
        {
          if(index == 2)//when disabled, hide all, set default 
          {
            var itemDescrPathObj = GetDescrObj2(descrPathObj.controlitem[item].item);
            SetHtmlObjDefVal(descrPathObj.controlitem[item].item + "-" + rndid, itemDescrPathObj['boardrw']);
            tr_obj.style.display ='None';
          }
          else
          {
            tr_obj.style.display ='';
          }
        }    
      }
      if(index == 1)
      {
        var tr_obj = document.getElementById('tsi_config_t.configSelfCap.commonConfig.prbsOutsel'+'-'+rndid+'tr');
        if(tr_obj)
          tr_obj.style.display ='None';

      }
      if(index == 0)
      {
        var tr_obj = document.getElementById('tsi_config_t.configSelfCap.commonConfig.ssc_prescaler'+'-'+rndid+'tr');
        if(tr_obj)
          tr_obj.style.display ='None';
      }
    }
    if(descrPathObj["onchangecbck"])
    {
      var finalArgs = [];
      finalArgs.push(null);
      finalArgs.push(id);
      var val = CallFunctStr(descrPathObj["onchangecbck"], finalArgs);
    }
  }
  return; 
}
templates["tableenumhwinterface"] = templates["tableenumsubstr"]; 
function getsettableenumhwinterface(id, type, val)/*1 set val, 0: return val*/
{
  var val = getsettableenumsubstr(id, type, val)
  if((type == 6) || (type == 8) || (type == 1) || (type == 9))
  {
       onchangetableenumhwinterface(0, id);
  }
  return val;
}



function onchangetableenumhwinterface(obj, id)/*1 set val, 0: return val*/
{
  var path = GetPathFromPathId(id);
  var val = getsettableenum(id, 7, 0); //read the index
  var rndid = GetIdFromPathId(id);
  var tsiobj = document.getElementById(path+".TSI")
  if(tsiobj)
     tsiobj.style.display = val ? 'None' : 'block';
  var csobj = document.getElementById(path+".CS")
  if(csobj)
    csobj.style.display = val ? 'block' : 'None';
  var basepath = ReducePath(path, 1);
  var descrPathObj = GetDescrObj(basepath+".config");
  if(descrPathObj)
    descrPathObj["inclsubstruct"] = val ? 'cs_config_t' : 'tsi_config_t';
  var recalib_path = ReducePath(path, 1);
  var recalibhtmllabelobj = document.getElementById(recalib_path+".recalib_config-"+rndid);
  if(recalibhtmllabelobj)
    recalibhtmllabelobj.style.display = val ? 'None' : 'block';  
  var recalibhtmlobj = document.getElementById("nt_tsi_recalib_config.tablestart");
  if(recalibhtmlobj)
    recalibhtmlobj.style.display = val ? 'None' : 'block';

  UpdateSafetyVisibility();  
  
  var baseDescrPathObj = GetDescrObj("cs_config_t");
  if(baseDescrPathObj)
  {
    if(!('links' in baseDescrPathObj))
    {
        linksobj = InitLinks(rndid);
        baseDescrPathObj["links"] = linksobj;
    }
    baseDescrPathObj["links"] = [];
  }
  baseDescrPathObj = GetDescrObj("tsi_config_t");
  if(baseDescrPathObj)
  {
    if(!('links' in baseDescrPathObj))
    {
        linksobj = InitLinks(rndid);
        baseDescrPathObj["links"] = linksobj;
    }
    baseDescrPathObj["links"] = [];
  }
  
  if(descrPathObj)
  {
    baseDescrPathObj = GetDescrObj(descrPathObj["inclsubstruct"]);
    linksobj = InitLinks(rndid);
    baseDescrPathObj["links"] = linksobj;
  }
}


templates["tableend"] = "</div>"; 
templates["incrementer"] = "<input type=\"number\" name=\"quantity\" size=\"2\" min=\"1\" max=\"5\" id=\"{{nodepath}}-{{rndid}}val\" value=\"{{default}}\" readonly><input type=\"button\" id=\"{{nodepath}}inc\" onclick=\"incrementerinc('{{nodepath}}','{{rndid}}','1')\" value=\"+\"> <span id=\"checkboxitem{{nodepath}}span\">"+templates["constcheckbox"]+"<br>";
templates["incrementer"] = templates["labelpart1"] + "<div class=\"adjust-number bg-arrow\">"+
										 "<input type=\"text\" id=\"{{nodepath}}-{{rndid}}val\" value=\"{{default}}\" />"+
										 "<a id=\"increase-controls\" class=\"increase-controls\" id=\"{{nodepath}}inc\" onclick=\"incrementerinc('{{nodepath}}','{{rndid}}','1')\"><p>+</p></a>"+
									"</div>"+templates["constcheckbox"]+templates["labelpart2"];
function getsetincrementer(id, type, val){ return getsettableedit(id, type, val); }

function enumonfocus(obj, id)
{
	var index = obj.selectedIndex;
	var path = GetPathFromPathId(id);
	var descrPathObj = GetDescrObj(path);
	//save the current label of the dropdown
	if(('default' in descrPathObj)&&(index in descrPathObj['default']))
	{
		descrPathObj['lasttext'] = descrPathObj['default'][index]['sublabel'];
		descrPathObj['lastid'] = descrPathObj['default'][index]['nodeid']
	}
	else
	{
		descrPathObj['lasttext'] = obj.childNodes[index].label;
		descrPathObj['lastid'] = obj.childNodes[index].id;
	}
}

function enumonchange(obj, id)
{
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
	var descrPathObj = GetDescrObj(path);
	var val;

    //get string of struct
    if(!( 'default' in descrPathObj))
    	return;
    var datadefobj = descrPathObj['default'];
    var str = "";
    
    //get index of enum
    var index = -1;
    for(objitem in datadefobj)
    {
        var subdivdescrPathObj = 0;
        
        //unregister the id of controls from controls type
        if('inclsubstructenum' in datadefobj[objitem])
        {
        	subdivdescrPathObj = GetDescrObj(datadefobj[objitem]['inclsubstructenum']);
        	if(subdivdescrPathObj)
        	{
            	if(!("links" in subdivdescrPathObj))
                {
                    subdivdescrPathObj["links"] = [];
                }
                for(linkitem in subdivdescrPathObj["links"])
                {
                    if(rndid in subdivdescrPathObj["links"][linkitem])
                    	subdivdescrPathObj["links"].splice(linkitem, 1) //delete the item from array with index linkitem
                }        	
        	}
        }
        
        val = GetHtmlObjVal(id, descrPathObj['template']);
        //if enum found report the insex
    	if(datadefobj[objitem]['item'] == val)
    	{
            index = objitem;
            var link = {};
            link[rndid] = path+".tablediv";
            //register id to controls type
            if(subdivdescrPathObj)
            {
                subdivdescrPathObj["links"].push(link);
                subdivdescrPathObj['rndid'] = rndid;            	
            }
    	}
    	
    }
    
    //was index found?
    if(index >= 0)
    {
    	var basepath = ReducePath(path, 1);

    	if('inclsubstructenum' in datadefobj[index])
    	{
        	var subdivdescrPathObj = GetDescrObj(datadefobj[index]['inclsubstructenum']);
        	str = GetWebContentFromObj2(subdivdescrPathObj, datadefobj[index]['inclsubstructenum'], "template", subdivdescrPathObj['rndid']);
    	}
    	var structnamedescrPathObj = GetDescrObj(basepath+".sructname");
    	if(structnamedescrPathObj)
    	{
    		var val = GetHtmlObjVal(basepath+".sructname-"+rndid, structnamedescrPathObj['template']);
    		var res = val.replace(descrPathObj['lasttext'], datadefobj[index]['sublabel']);
    		SetHtmlObjVal(basepath+".sructname-"+rndid, structnamedescrPathObj['template'], res);
    		console.log(res);
    		//sync name
    		sructname_sync(null, basepath+".sructname-"+rndid);
    		//sync disabled checkboxes
    	}
    }

    //paste it to div
    var webdivobj = document.getElementById(id+"substr");
	  if(webdivobj)
    {
		  webdivobj.innerHTML = str;
    }
}

function enumonchangesynclinkedopt(obj, id)
{
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
  var descrPathObj = GetDescrObj2(path);
  if('linkedobject' in descrPathObj)
  {
    var val = GetHtmlObjVal(id, descrPathObj["template"]);
    var linkeddescrPathObj = GetDescrObj2(descrPathObj['linkedobject']);
    SetHtmlObjVal(descrPathObj['linkedobject']+"-"+rndid, linkeddescrPathObj["template"], val);
  }
}

function sructname_sync(thisobj, id)
{
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
	var subdivdescrPathObj = GetDescrObj(path);
	var val = GetHtmlObjVal(id, subdivdescrPathObj['template']);
	//get the path for sync
	var basepath = ReducePath(path, 1);
	var divdescrPathObj = GetDescrObj(basepath);
	for(key in divdescrPathObj['syncs'])
	{
		var index = key;
		var syncdescrPathBase = ReducePath(divdescrPathObj['syncs'][index], 1);
		var syncdescrPathObj = GetDescrObj(divdescrPathObj['syncs'][index]);
		if('templateitem' in syncdescrPathObj) // exception, item is baseof 2 templates, second template has field to update
		{
			var res = SetHtmlObjVal(rndid, syncdescrPathObj['templateitem'], val);
		}
/*		else if (syncdescrPathObj['template'] == 'checkboxarray')
		{
			//get object with selected electrodes/items
			var subdivdescrPathObja = GetDescrObj(syncdescrPathObj['objectpath']);
			//get object with all electrodes/items
			var subdivdescrPathObjb = GetDescrObj(syncdescrPathObj['objectallpath']);
			//build checkoboxes based on all, checkc all with selected
			//get array ['rndid', 'value' = true/false, 'opt' = enabled/disabled]
			arr = [];
			item = {};
			item['rndid'] = 1;
			item['value'] = val;
			item['opt'] = "enabled";//"disabled";
			arr.push(item)
			arr.push(item)
			var str = Mustache.to_html(templates['checkboxitem'], item);
			console.log(str)
        	var obj = document.getElementById(syncdescrPathObj['nodepath']);
        	if(obj)
        		obj.innerHTML = str;

			//var res = SetHtmlObjVal(subdivdescrPathObj['nodepath'], 'tableedit', str);
			//templates["checkboxitem"] = "<label id=\"checkboxitem{{rndid}}label\" for=\"checkboxitem{{rndid}}input\">{{value}}</label><input type=\"checkbox\" id=\checkboxitem{{rndid}}input\" name=\"{{value}}\" value=\"{{value}}\" disabled=\"{{opt}}\"/>";
			
		}*/
		else if (('activetmpl' in syncdescrPathObj) && (syncdescrPathObj['template'] == 'checkboxarray'))
		{
			var syncdescrPathBaseObj = GetDescrObj(syncdescrPathBase);
			if('links' in syncdescrPathBaseObj)
			{
				for(link in syncdescrPathBaseObj['links'])
		        {
		        	linkobj = syncdescrPathBaseObj['links'][link]
		        	for(linkid in linkobj)
		        		var res = UpdateHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + linkid, linkid + "_"+rndid, syncdescrPathObj['template'], val);
		        }				
			}
			else
			{
				var res = UpdateHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + syncdescrPathBaseObj["rndid"], syncdescrPathBaseObj["rndid"] + "_"+rndid, syncdescrPathObj['template'], val);
			}
			
	        //var res = SetHtmlObjVal(rndid, syncdescrPathObj['activetmpl'], val);
	    } 
		else if (('activetmpl' in syncdescrPathObj) && ((syncdescrPathObj['template'] == 'tableeditshilding')||(syncdescrPathObj['template'] == 'tableenumsubstrshareditem')))
		{
			var syncdescrPathBaseObj = GetDescrObj(syncdescrPathBase);
			if('links' in syncdescrPathBaseObj)
			{
				for(link in syncdescrPathBaseObj['links'])
		        {
		        	linkobj = syncdescrPathBaseObj['links'][link]
		        	for(linkid in linkobj)
		        		var res = UpdateHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + linkid, linkid + "_"+rndid, syncdescrPathObj['template'], val);
		        }				
			}
			else
			{
				var res = UpdateHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + syncdescrPathBaseObj["rndid"], syncdescrPathBaseObj["rndid"] + "_"+rndid, syncdescrPathObj['template'], val);
			}
			
	        //var res = SetHtmlObjVal(rndid, syncdescrPathObj['activetmpl'], val);
	    }
		else
		{
			var res = SetHtmlObjVal(id, syncdescrPathObj['template'], val);
		}
	}
}

//pole pro generovani seznamu elektrod budou popsane jen v decr xml, bude se tam jen append jednotlive polozky(odkazy na umisteni, aby se definovanym zpusobem mohl vytahnout nazev)
//do tohoto pole se budou vkladat elektrody pouze po zatrzeni.
//v links uz je seznam vsech elektord, to by melo byt zdrojem pro vytazeni checkbox template(u controlsu a module, tedy v controlu u checkbox bude odkaz na electrode.links). u checkbox template by mel byt link, kam se musi ukladat zatrzene elektrody.
//asi se musi v control zalozit nove pole, podle poctu controlu, kam se budou ukladat zatrzene elektrody(to se bude cist checkboxem v controlsech)
//definovat jednotnou cestu jak zasynchronizovat el names a nebo predat typ/podle ktereho by se zavolala splravna funkce pro sync

function incrementerSetCnt(path, id, val)
{
	var basepath = ReducePath(path, 1);
	var descrPathObj = GetDescrObj(basepath);
	var obj = document.getElementById(basepath+".control_count-"+descrPathObj['rndid']+"val"); 
    var resetindex = 0;
	if(obj)
		obj.value = val;
}

function incrementerdel(path, id, obj)
{
	//alert(path + ":" +  id + ":"+obj)
	var basepath = ReducePath(path, 1);
	var descrPathObj = GetDescrObj(basepath);
	var obj = document.getElementById(basepath+".control_count-"+descrPathObj['rndid']+"val"); 
    var resetindex = 0;
	if(obj)
    {
        if(Number(obj.value)>1)
        {
            obj.value = Number(obj.value) - Number(1);
            if(Number(obj.value) == 1)
            	resetindex = 1;
        }
        else
        {
        	resetindex = 1;
        	return ;
        }
    }
    else
    {
        return;
    }
    DelSubStructTab(path, id, resetindex);
}
function incrementersel(path, id)
{
	var basepath = ReducePath(path, 1);
	//insert one tab, insert div with table for tab
    if(IsValidDataPath(basepath, path+".listitem"))
    {
    	//var datanode = GetDescrObj(basepath+".listitem");
    	var tablelistsobj = document.getElementById(basepath+".list");
    	DeselectTabs(tablelistsobj)
    	var tabledivsobj = document.getElementById(basepath+".tablediv");
    	HideSubDivs(tabledivsobj);
    	SelectTab(tablelistsobj, id+"li");
    	ShowSubDiv(tabledivsobj, id+"div");
    }
}
function incrementerinc(path, id)
{
	var obj = document.getElementById(path+"-"+id+"val"); 
	if(obj) 
	{
			if(Number(obj.value)<64) 
				obj.value = Number(obj.value) + Number(1);
			else
				return;
	}
	else
	{
		return;
	}
    //remove incrementer from the path
	var basepath = ReducePath(path, 1);
	var res = AddSubStructTab(basepath,Number(obj.value));
}

function DelSubStructTab(path, id, resetindex)
{
    //delete tab
    var basepath = ReducePath(path, 1)
    var ulobj = document.getElementById(id+"li");
    if(ulobj)
    {
        ulobj.outerHTML = "";
        delete ulobj;
    }
    //delete div
    var divobj = document.getElementById(id+"div");
    if(divobj)
    {
        divobj.outerHTML = "";
        delete divobj;
    }
    //unregister id from list in data array for sub struct
    var divDescrPathObj = GetDescrObj(basepath+".tablediv");
    if ('inclsubstructdyn' in divDescrPathObj)
    {
        var linksDescrPathObj = GetDescrObj(divDescrPathObj['inclsubstructdyn']+".links");
        for ( subitem in linksDescrPathObj )
        {
            if(id in linksDescrPathObj[subitem])
            {
                linksDescrPathObj.splice(subitem, 1);
            }
        }
        var inclstructDescrPathObj = GetDescrObj(divDescrPathObj['inclsubstructdyn']);
        if(resetindex==1)
        	inclstructDescrPathObj['sructname']['increment'] = 1;
        else if(resetindex==2)
        	inclstructDescrPathObj['sructname']['increment'] = 0;
        
        var arr = GetSubnodePathsWithKey(inclstructDescrPathObj, divDescrPathObj['inclsubstructdyn'], "onchangecbck");
        for(arrindex in arr)
        {
    		var index = arrindex;
        	var substructdescrPathObj = GetDescrObj(arr[index]);
        	if(substructdescrPathObj["onchangecbck"] == 'enumonchange')
        	{
        		for(arrindex2 in substructdescrPathObj['default'])
        		{
        			var subtypedescrPathObj = GetDescrObj(substructdescrPathObj['default'][arrindex2]['inclsubstructenum']+".links");
        	        for ( subitem in subtypedescrPathObj )
        	        {
        	            if(id in subtypedescrPathObj[subitem])
        	            {
        	            	subtypedescrPathObj.splice(subitem, 1);
        	            }
        	        }
        		}
        	}
        }
        
        for(key in inclstructDescrPathObj['syncs'])
        {
    		var index = key;
    		var syncdescrPathBase = ReducePath(inclstructDescrPathObj['syncs'][index], 1);
        	var syncdescrPathObj = GetDescrObj(inclstructDescrPathObj['syncs'][index]);
        	if (('activetmpl' in syncdescrPathObj) && (syncdescrPathObj['template'] == 'checkboxarray'))
        	{
    			//delete from all controls
        		var syncdescrPathBaseObj = GetDescrObj(syncdescrPathBase);
    			if('links' in syncdescrPathBaseObj)
    			{
    				for(link in syncdescrPathBaseObj['links'])
    		        {
    		        	linkobj = syncdescrPathBaseObj['links'][link]
    		        	for(linkid in linkobj)
    		        		var res = DelHtmlObjVal(syncdescrPathObj['nodepath']+ "-" +linkid, linkid + "_"+id, syncdescrPathObj['template']);
    		        }
    			}
    			else
    			{
    				var res = DelHtmlObjVal(syncdescrPathObj['nodepath']+ "-" +syncdescrPathObj["rndid"], syncdescrPathObj["rndid"] + "_"+id, syncdescrPathObj['template']);
    			}
        	}
        	else if (('activetmpl' in syncdescrPathObj) && (syncdescrPathObj['template'] == 'tableeditshilding'))
        	{
    			var syncdescrPathBaseObj = GetDescrObj(syncdescrPathBase);
    			if('links' in syncdescrPathBaseObj)
    			{
    				for(link in syncdescrPathBaseObj['links'])
    		        {
    		        	linkobj = syncdescrPathBaseObj['links'][link]
    		        	for(linkid in linkobj)
    		        		var res = DelHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + linkid, linkid + "_"+id, syncdescrPathObj['template']);
    		        }				
    			}
    			else
    			{
    				var res = DelHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + syncdescrPathBaseObj["rndid"], syncdescrPathBaseObj["rndid"] + "_"+rndid, syncdescrPathObj['template']);
    			}
        		//unregister disabled electrode from nt_control array
        		if('nt_control_disabled' in syncdescrPathBaseObj)
        		{
        			for( elid in syncdescrPathBaseObj['nt_control_disabled'])
        			{
        				if(syncdescrPathBaseObj['nt_control_disabled'][elid] == id)
        				{
        					delete syncdescrPathBaseObj['nt_control_disabled'][elid];
        				}
        			}
        		}
        		//unregister disabled electrode from nt_electrode array
        		if('nt_electrode_disabled' in syncdescrPathBaseObj)
        		{
        			for( elid in syncdescrPathBaseObj['nt_electrode_disabled'])
        			{
        				if(syncdescrPathBaseObj['nt_electrode_disabled'][elid] == id)
        				{
        					delete syncdescrPathBaseObj['nt_electrode_disabled'][elid];
        				}
        			}
        		}
        	}
        	else if (('activetmpl' in syncdescrPathObj) && (syncdescrPathObj['template'] == 'tableenumsubstrshareditem'))
        	{
    			var syncdescrPathBaseObj = GetDescrObj(syncdescrPathBase);
    			if('links' in syncdescrPathBaseObj)
    			{
    				for(link in syncdescrPathBaseObj['links'])
    		        {
    		        	linkobj = syncdescrPathBaseObj['links'][link]
    		        	for(linkid in linkobj)
    		        		var res = DelHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + linkid, linkid + "_"+id, syncdescrPathObj['template']);
    		        }				
    			}
    			else
    			{
    				var res = DelHtmlObjVal(syncdescrPathObj['nodepath'] + "-" + syncdescrPathBaseObj["rndid"], syncdescrPathBaseObj["rndid"] + "_"+rndid, syncdescrPathObj['template']);
    			}
        	}
        		
        }
        
    }
}

function GetSubnodePathsWithKey(obj, path, key)
{
	var arr = [];
	for(keyitem in obj)
	{
		if(typeof obj[keyitem] === 'object')
		{
			var out = GetSubnodePathsWithKey(obj[keyitem], path+"."+keyitem, key);
			for(keyitem in out)
			    arr.push(out[keyitem]);
		}
		else if(key == keyitem)
		{
			arr.push(path);
		}
	}
	
	return arr;
}

function CheckLabel(path, label)
{
  var divDescrPathObj = GetDescrObj(path+".tablediv");
  if('inclsubstructdyn' in divDescrPathObj)
  {
    var subdivDescrPathObj = GetDescrObj(divDescrPathObj['inclsubstructdyn']);
    if(!("links" in subdivDescrPathObj))
    {
      subdivDescrPathObj["links"] = [];
    }
    for(linkbase in subdivDescrPathObj['links'])
    {
      for(linkid in subdivDescrPathObj['links'][linkbase])
      {
        var val = GetHtmlObjVal(subdivDescrPathObj['sructname']['nodepath'] + "-" + linkid, subdivDescrPathObj['sructname']['template']);
        if(val == label)
          return 0;
      }
    }
  }
  return 1;
}

function AddSubStructTab(path,index)
{
    var rndid = GetRandObjId();
    var label = ""; 
	//insert one tab, insert div with table for tab
	if(IsValidDescrObjPath(path+".list"))
	{
	    var divDescrPathObj = GetDescrObj(path+".tablediv");
	    var divobj = document.getElementById(path+".tablediv");
	    var subdivobj = document.createElement("div");

	    var subdivDescrPathObj = GetDescrObj(divDescrPathObj['inclsubstructdyn']);
	    var itemindex = subdivDescrPathObj['sructname']['increment']
	    itemindex++;
	    subdivDescrPathObj['sructname']['increment'] = itemindex;
      if('linkscope' in subdivDescrPathObj)
        subdivDescrPathObj['linkscope']['index'] = Number(itemindex)-1;
      subdivDescrPathObj['sructname']['increment'] = itemindex;
	    label = subdivDescrPathObj['sructname']['defaultbase']+"_" + itemindex;
      if(!CheckLabel(path, label))
      {
        if(CheckLabel(path, label+"a"))
          label = label + "a";
      }
      
        //get datanode
		var ulDescrPathObj = GetDescrObj(path+".list");
        //call mustache to get the string of the li,  append the li to ul
        ulDescrPathObj['nodepath'] = path+".list";
        ulDescrPathObj['rndid'] = rndid;
        ulDescrPathObj['default'] = label;
		var str = Mustache.to_html(templates[ulDescrPathObj["templateitem"]], ulDescrPathObj);

        var ulobj = document.getElementById(path+".list");
        DeselectTabs(ulobj)
        //create li, replace the content of the li
		var liobj = document.createElement("DIV");
		liobj.innerHTML = str;
		liobj.setAttribute("class", "tag tag-arotary active-tag");
		liobj.setAttribute("id", rndid+"li");
		ulobj.appendChild(liobj);
	    ulitems = ulobj.childNodes;

	    if('inclsubstructdyn' in divDescrPathObj)
	    {
	    	var subdivDescrPathObj = GetDescrObj(divDescrPathObj['inclsubstructdyn']);
            if(!("links" in subdivDescrPathObj))
            {
            	subdivDescrPathObj["links"] = [];
            }
            var link = {};
            link[rndid] = path+".tablediv";
            subdivDescrPathObj["links"].push(link);
            subdivDescrPathObj['rndid'] = rndid;
            subdivDescrPathObj['sructname']['default'] = label;
	    	var h = GetWebContentFromObj2(subdivDescrPathObj, divDescrPathObj['inclsubstructdyn'], "template", subdivDescrPathObj['rndid']);
            subdivobj.innerHTML = h;
            var divitems = divobj.childNodes;
            subdivobj.setAttribute("id", rndid+"div");
            
            HideSubDivs(divobj);
            divobj.appendChild(subdivobj);

            if (!("syncs" in subdivDescrPathObj)) {
                subdivDescrPathObj["syncs"] = [];
            }
            else if(typeof(subdivDescrPathObj["syncs"]) === 'string')
            {
            	var s = subdivDescrPathObj["syncs"];
            	subdivDescrPathObj["syncs"] = [];
            	subdivDescrPathObj["syncs"].push(s);
            	//workaround to passdown argument with syncs
            	if("syncs2" in subdivDescrPathObj)
            	{
                	var s2 = subdivDescrPathObj["syncs2"];
                	subdivDescrPathObj["syncs"].push(s2);
            	}
            	if("syncs3" in subdivDescrPathObj)
            	{
                	var s3 = subdivDescrPathObj["syncs3"];
                	subdivDescrPathObj["syncs"].push(s3);
            	}
            }
            var to_push = true;
            if('sructname' in subdivDescrPathObj)
            {
            	if('dynamic_sync' in subdivDescrPathObj['sructname'])
            	{
            		//push only once
            		for(i in subdivDescrPathObj["syncs"])
            		{
            			if(subdivDescrPathObj['sructname']['dynamic_sync'] == subdivDescrPathObj["syncs"][i])
            				to_push = false;
            		}
            		if(to_push)
            			subdivDescrPathObj["syncs"].push(subdivDescrPathObj['sructname']['dynamic_sync']);
            	}
            }
            //load all electrodes to new controls
            if ('electrodes' in subdivDescrPathObj) {
            	if('datasrc' in subdivDescrPathObj['electrodes'])
            	{
            		var electDescrPathObj = GetDescrObj(subdivDescrPathObj['electrodes']['datasrc']);
            		for(linkbase in electDescrPathObj['links'])
            		{
            			for(linkid in electDescrPathObj['links'][linkbase])
            			{
                			var syncDescrPathObj = subdivDescrPathObj['electrodes'];
                			var val = GetHtmlObjVal(electDescrPathObj['sructname']['nodepath'] + "-" + linkid, electDescrPathObj['sructname']['template']);
                			AddHtmlObjVal(syncDescrPathObj["nodepath"] +"-"+syncDescrPathObj["rndid"] , syncDescrPathObj["rndid"] + "_"+linkid, syncDescrPathObj['template'], val);
                			for(disabledid in electDescrPathObj["nt_control_disabled"])
                			{
                				if(electDescrPathObj["nt_control_disabled"][disabledid] == linkid)
                				{
                					VisibilityHtmlObjVal(syncDescrPathObj["rndid"] + "_"+linkid, syncDescrPathObj['activetmpl'], 1);
                					break;
                				}
                			}
            			}
            		}
            	}
            }
            
            //load all electrodes to shielding selector
            if ('shielding_electrode' in subdivDescrPathObj)
            {
            	for(linkbase in subdivDescrPathObj['links'])
        		{
        			for(linkid in subdivDescrPathObj['links'][linkbase])
        			{
            			if(rndid != linkid)
            			{
            				var syncDescrPathObj = subdivDescrPathObj;
                			var val = GetHtmlObjVal(subdivDescrPathObj['sructname']['nodepath'] + "-" + linkid, subdivDescrPathObj['sructname']['template']);
                			AddHtmlObjVal(divDescrPathObj['inclsubstructdyn'] +".shielding_electrode-"+rndid , rndid + "_"+linkid, subdivDescrPathObj['shielding_electrode']['template'], val);
                            for (disabledid in syncDescrPathObj["nt_electrode_disabled"]) {
                                if (syncDescrPathObj["nt_electrode_disabled"][disabledid] == linkid) {
                                    VisibilityHtmlObjVal(rndid + "_" + linkid, subdivDescrPathObj['shielding_electrode']['activetmpl'], 1);
                                    break;
                                }
                            }
            			}
        			}
        		}
            }
            if ('setmode' in subdivDescrPathObj)
            {
              //get mode
              var rbobj = 0;
              if('id' in subdivDescrPathObj['setmode'])
                rbobj = document.getElementById(subdivDescrPathObj['setmode']['id']);
              var mode = 0;
              if(rbobj && rbobj[0])
                mode = !rbobj[0].checked;
              if('base' in subdivDescrPathObj['setmode'])
                SetModeToWeb(subdivDescrPathObj, subdivDescrPathObj['setmode']['base'], subdivDescrPathObj['setmode']['base'], rndid, mode, 'mode');
            }
            //load all electrodes to keydetector selector
            if ('share_keydet_cfg' in subdivDescrPathObj)
            {
            	if(!("keydetector_interface_electrode_disabled" in subdivDescrPathObj))
            		subdivDescrPathObj["keydetector_interface_electrode_disabled"] = [];
            	for(linkbase in subdivDescrPathObj['links'])
        		{
        			for(linkid in subdivDescrPathObj['links'][linkbase])
        			{
            			if(rndid != linkid)
            			{
            				var syncDescrPathObj = subdivDescrPathObj;
                			var val = GetHtmlObjVal(subdivDescrPathObj['sructname']['nodepath'] + "-" + linkid, subdivDescrPathObj['sructname']['template']);
                			AddHtmlObjVal(divDescrPathObj['inclsubstructdyn'] +".share_keydet_cfg-"+rndid , rndid + "_"+linkid, subdivDescrPathObj['share_keydet_cfg']['template'], val);
                            for (disabledid in syncDescrPathObj["keydetector_interface_electrode_disabled"]) {
                                if ((syncDescrPathObj["keydetector_interface_electrode_disabled"][disabledid] == linkid)&&(rndid != linkid)) {
                                    VisibilityHtmlObjVal(rndid + "_" + linkid, subdivDescrPathObj['share_keydet_cfg']['activetmpl'], 1);
                                    break;
                                }
                            }
            			}
        			}
        		}
            }
            
            //add electrode/control to all sync objects
            for(key in subdivDescrPathObj["syncs"])
            {
            	index = key;
            	var syncDescrPathObj = GetDescrObj(subdivDescrPathObj["syncs"][index]);
            	if(syncDescrPathObj)
            	{
            		var basepath = ReducePath(subdivDescrPathObj["syncs"][index], 1);
                	var syncBaseDescrPathObj = GetDescrObj(basepath+".links");
                	if(syncBaseDescrPathObj)
                	{
                    	//add electrode to all controls
                		for(baseidindex in syncBaseDescrPathObj)
                		{
                			for(baseid in syncBaseDescrPathObj[baseidindex])
                			{
                				AddHtmlObjVal(syncDescrPathObj["nodepath"] + "-" + baseid, baseid + "_"+rndid, syncDescrPathObj['template'], subdivDescrPathObj['sructname']['default']);
                				SetHtmlObjVal(syncDescrPathObj["rndid"] + "_"+rndid , syncDescrPathObj['activetmpl'], syncDescrPathObj['initcheckbox']);
                			}
                		}
                	}
                	else
                	{
                    	AddHtmlObjVal(syncDescrPathObj["nodepath"] + "-"+syncDescrPathObj["rndid"] , syncDescrPathObj["rndid"] + "_"+rndid, syncDescrPathObj['template'], subdivDescrPathObj['sructname']['default']);
                    	SetHtmlObjVal(syncDescrPathObj["rndid"] + "_"+rndid , syncDescrPathObj['activetmpl'], syncDescrPathObj['initcheckbox']);
                	}
            	}
            }
            
    		//push only once
    		for(i in subdivDescrPathObj["syncs"])
    		{
    			if(path+".list" == subdivDescrPathObj["syncs"][i])
    				to_push = false;
    		}
    		if(to_push)
    			subdivDescrPathObj["syncs"].push(path+".list")

            //call on change event after creation
            var arr = GetSubnodePathsWithKey(subdivDescrPathObj, divDescrPathObj['inclsubstructdyn'], "onchangecbck");
            for(key in arr)
            {
                var index = key;
            	var nameDescrPathObj = GetDescrObj(arr[index]);

            	if(nameDescrPathObj["onchangecbck"] !== "sructname_sync")//temporary exception
            	{
                    var finalArgs = [];
                    finalArgs.push(null);
                    finalArgs.push(arr[index]+"-"+nameDescrPathObj['rndid']);
                    var val = CallFunctStr(nameDescrPathObj["onchangecbck"], finalArgs);
            	}
            }
	    }
	}
	else
	{
		return {rndid:0, label:""};
	}
	return {rndid:rndid, label:label};
}

function SyncSubStructTab(path,index)
{
	if(!(IsValidDescrObjPath(path+".list")))
		return;
	
    var divDescrPathObj = GetDescrObj(path+".tablediv");
    
    if(!('inclsubstructdyn' in divDescrPathObj))
    	return;
    var subdivDescrPathObj = GetDescrObj(divDescrPathObj['inclsubstructdyn']);
	//call on change event after creation
    var arr = GetSubnodePathsWithKey(subdivDescrPathObj, divDescrPathObj['inclsubstructdyn'], "onchangecbck");
    for(key in arr)
    {
        var index = key;
    	var nameDescrPathObj = GetDescrObj(arr[index]);

    	//if(nameDescrPathObj["onchangecbck"] !== "sructname_sync")//temporary exception
    	{
            var finalArgs = [];
            finalArgs.push(null);
            finalArgs.push(arr[index]+"-"+nameDescrPathObj['rndid']);
            var val = CallFunctStr(nameDescrPathObj["onchangecbck"], finalArgs);
    	}
    }
}


templates["tablegestureitem"] = "<div class=\"subline\">"+
											 "<p>{{index}}</p>"+
											 "<input class=\"touchsw-1-4-input\" placeholder=\"\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}{{index}}f1x\" value=\"{{f1x}}\"></input>"+
											 "<input class=\"touchsw-1-4-input\" placeholder=\"\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}{{index}}f1y\" value=\"{{f1y}}\"></input>"+
											 "<input class=\"touchsw-1-4-input\" placeholder=\"\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}{{index}}f2x\" value=\"{{f2x}}\"></input>"+
											 "<input class=\"touchsw-1-4-input\" placeholder=\"\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}{{index}}f2y\" value=\"{{f2y}}\"></input>"+
									 "</div>";
templates["tablegesture"] = "<div class=\"touchsw-table-line touchsw-special-line\">" +  
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"sub-block\">"+
										 "<div class=\"subline\">"+
										 		"<input class=\"touchsw-1-10-input\" placeholder=\"3\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}cnt\" value=\"0\" onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\')',1);\"></input>"+
										 		"<p class=\"subline-headline\">Finger1 x</p>"+
												"<p class=\"subline-headline\">Finger1 y</p>"+
												"<p class=\"subline-headline\">Finger2 x</p>"+
												"<p class=\"subline-headline\">Finger2 y</p>"+
									 	"</div>"+
                     "<div id=\"{{nodepath}}-{{rndid}}table\">"+
                     "</div>"+
									 "</div>"+ 
									 "{{#rwbase}}<div class=\"button-wrap\">"+templates["RWButtons"]+"</div>{{/rwbase}}"+
							"</div>";

function gestureonchange(obj, id)
{
	var htmlcntobj = document.getElementById(id+"cnt");
	var i;

	if(!htmlcntobj)
		return;
	var cnt = Number(htmlcntobj.value);
	var htmltableobj = document.getElementById(id+"table");
	if(!htmltableobj)
		return;
	
	var cntdel = 0;
	if((htmltableobj.children.length)>cnt)
		cntdel = (htmltableobj.children.length)-cnt;
		
	var cntadd = 0;
	if(cnt>(htmltableobj.children.length))
		cntadd = cnt-(htmltableobj.children.length);

	for(i=0; i<cntadd; i++)
	{
		var div = document.createElement('div');
    htmltableobj.appendChild(div);
		var trDescrPathObj = {};
		trDescrPathObj['nodepath'] = GetPathFromPathId(id);
		trDescrPathObj['rndid'] = GetIdFromPathId(id);
		trDescrPathObj['index'] = htmltableobj.children.length;
		trDescrPathObj['f1x'] = 0;
		trDescrPathObj['f1y'] = 0;
		trDescrPathObj['f2x'] = 0;
		trDescrPathObj['f2y'] = 0;
		var str = Mustache.to_html(templates["tablegestureitem"], trDescrPathObj);
		htmltableobj.children[htmltableobj.children.length-1].outerHTML = str;
//		alert(str)
		//htmltableobj.rows[htmltableobj.rows.length].innerHTML=str;
		
		
	}
	for(i=0; i<cntdel; i++)
	{
		htmltableobj.removeChild(htmltableobj.childNodes[htmltableobj.children.length-1]);
	}
	
	
	for(i=0; i<cnt; i++)
	{
		//insert rows to table with indexes
		//probably only add/dell columns in table when not equal with cnt
		//define cnt into ids
	}
	//alert(htmlcntobj.value)
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
	
	//alert(id)
}

function getsettablegesture(id, type, val)/*1 set val, 0: return val*/
{ 
	var tablehtmlobj = document.getElementById(id+"table");
	if(!tablehtmlobj)
		return "NA";
	var cnthtmlobj = document.getElementById(id+"cnt");
	if(!cnthtmlobj)
		return "NA";
	if(cnthtmlobj)
	{ 
		if((type == 1)||(type == 8))
		{
			if(val)
			{
				if(val == "NULL")
				{
				  cnthtmlobj.value = 0;
				  tablegestureproxionchange(0, id);
				}
				else
				{
				  val = val.replace("nt_points64", "");
				  var res = val.replace(/[^0-9a-fA-F.,|x|X]/g, '');
				  arr = res.split(",");
				  cnt = ((arr.length-1)/4);
				  cnthtmlobj.value = Math.round(cnt -1);
				  gestureonchange(0, id);
				  for(j=0; j<cnt; j++)
				  {
					 i=j+Number(1);
					 var obj = document.getElementById(id+String(i)+"f1x");
					 if(obj) obj.value = arr[0+(Number(j)*4)];
					 obj = document.getElementById(id+String(i)+"f1y");
					 if(obj) obj.value = arr[1+(Number(j)*4)];
					 obj = document.getElementById(id+String(i)+"f2x");
					 if(obj) obj.value = arr[2+(Number(j)*4)];
					 obj = document.getElementById(id+String(i)+"f2y");
					 if(obj) obj.value = arr[3+(Number(j)*4)];
				  }
        }
			}
		}
		else if(type == 6)
		{
			;
		}
		else if((type == 0)||(type == 7))
		{
			var cnt = cnthtmlobj.value
			var j;
			var out = "";
			if(cnt == 0)
			{
			  out = "NULL";
			}
			else
			{
			  out="(nt_points64 []){";
			  for(j=0;j<cnt;j++)
			  {
				  var item = "{";
				  i=j+Number(1);
				  var obj = document.getElementById(id+String(i)+"f1x");
				  if(obj) item = item + "{"+obj.value
				  obj = document.getElementById(id+String(i)+"f1y");
				  if(obj) item = item + ","+obj.value+"},"
				  obj = document.getElementById(id+String(i)+"f2x");
				  if(obj) item = item + "{"+obj.value
				  obj = document.getElementById(id+String(i)+"f2y");
				  if(obj) item = item + ","+obj.value+"}"
				  item = item + "},";
				  out = out + item;
			  }
			  out=out+"{{0,0},{0,0}}}";
      }
			return out;  
		}
		else
		{
			debug_print("getsettableenum: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	} 
}

templates["tablegestureproxiitem"] = "<div class=\"subline\">"+
											 "<p>{{index}}</p>"+
											 "<input class=\"touchsw-table-input\" placeholder=\"\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}{{index}}gest\" value=\"{{gest}}\"></input>"+
									 "</div>";

templates["tablegestureproxi"] = "<div class=\"touchsw-table-line touchsw-special-line\">" +  
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"sub-block\">"+
										 "<div class=\"subline\">"+
										 		"<p>number of gestures: </p>"+
										 		"<input class=\"touchsw-1-10-input\" placeholder=\"3\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}cnt\" value=\"0\" onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\')',1);\"></input>"+
									 	 "</div>"+
                     "<div id=\"{{nodepath}}-{{rndid}}table\">"+
                     "</div>"+
									 "</div>"+ 
									 "{{#rwbase}}<div class=\"button-wrap\">"+templates["RWButtons"]+"</div>{{/rwbase}}"+
							"</div>";
function tablegestureproxionchange(obj, id)
{
	var htmlcntobj = document.getElementById(id+"cnt");
	var i;

	if(!htmlcntobj)
		return;
	var cnt = Number(htmlcntobj.value);
	var htmltableobj = document.getElementById(id+"table");
	if(!htmltableobj)
		return;
	
	var cntdel = 0;
	if((htmltableobj.children.length)>cnt)
		cntdel = (htmltableobj.children.length)-cnt;
		
	var cntadd = 0;
	if(cnt>(htmltableobj.children.length))
		cntadd = cnt-(htmltableobj.children.length);

	for(i=0; i<cntadd; i++)
	{
		var div = document.createElement('div');
    htmltableobj.appendChild(div);
		var trDescrPathObj = {};
		trDescrPathObj['nodepath'] = GetPathFromPathId(id);
		trDescrPathObj['rndid'] = GetIdFromPathId(id);
		trDescrPathObj['index'] = htmltableobj.children.length;
		trDescrPathObj['gest'] = 0;
		var str = Mustache.to_html(templates["tablegestureproxiitem"], trDescrPathObj);
		htmltableobj.children[htmltableobj.children.length-1].outerHTML = str;
//		alert(str)
		//htmltableobj.rows[htmltableobj.rows.length].innerHTML=str;
		
		
	}
	for(i=0; i<cntdel; i++)
	{
		htmltableobj.removeChild(htmltableobj.childNodes[htmltableobj.children.length-1]);
	}
	
	
	for(i=0; i<cnt; i++)
	{
		//insert rows to table with indexes
		//probably only add/dell columns in table when not equal with cnt
		//define cnt into ids
	}
	//alert(htmlcntobj.value)
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
	
	//alert(id)
}

function getsettablegestureproxi(id, type, val)/*1 set val, 0: return val*/
{ 
	var tablehtmlobj = document.getElementById(id+"table");
	if(!tablehtmlobj)
		return "NA";
	var cnthtmlobj = document.getElementById(id+"cnt");
	if(!cnthtmlobj)
		return "NA";
	if(cnthtmlobj)
	{ 
		if((type == 1)||(type == 8))
		{
			if(val)
			{
				if(val == "NULL")
				{
				  cnthtmlobj.value = 0;
				  tablegestureproxionchange(0, id);
				}
				else
				{
				  val = val.replace("uint32_t", "");
				  var res = val.replace(/[^0-9a-fA-F.,|x|X]/g, '');
				  arr = res.split(",");
				  cnt = ((arr.length-1));
				  cnthtmlobj.value = cnt;
				  tablegestureproxionchange(0, id);
				  for(j=0; j<cnt; j++)
				  {
					 i=j+Number(1);
					 var obj = document.getElementById(id+String(i)+"gest");
					 if(obj) obj.value = arr[0+(Number(j))];
				  }
        }
				
			}
		}
		else if(type == 6)
		{
			;
		}
		else if((type == 0)||(type == 7))
		{
			var cnt = cnthtmlobj.value
			var j;
			var out = "";
			if(cnt == 0)
			{
			  out = "NULL";
			}
			else
			{
			 out="(uint32_t []){";
			 for(j=0;j<cnt;j++)
			 {
				var item = "";
				i=j+Number(1);
				var obj = document.getElementById(id+String(i)+"gest");
				if(obj) item = item + obj.value
				item = item + ",";
				out = out + item;
			 }
			 out=out+"0}";
			 return out;
      }  
		}
		else
		{
			debug_print("getsettablegestureproxi: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	} 
}

templates["tablegesturekeypad"] = "<div class=\"touchsw-table-line touchsw-special-line\">" +  
									 "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									 "<div class=\"sub-block\">"+
										 "<div class=\"subline\">"+
										 		"<p>{{label2}} </p>"+
										 		"<input class=\"touchsw-1-10-input\" placeholder=\"3\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}cnt\" value=\"0\" onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}');\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\')',1);\"></input>"+
									 	 "</div>"+
                     "<div id=\"{{nodepath}}-{{rndid}}table\">"+
                     "</div>"+
									 "</div>"+ 
									 "{{#rwbase}}<div class=\"button-wrap\">"+templates["RWButtons"]+"</div>{{/rwbase}}"+
							"</div>"; 

function tablegesturekeypadonchange(obj, id)
{
	var htmlcntobj = document.getElementById(id+"cnt");
	var i;

	if(!htmlcntobj)
		return;
	var cnt = Number(htmlcntobj.value);
	var htmltableobj = document.getElementById(id+"table");
	if(!htmltableobj)
		return;
	
	var cntdel = 0;
	if((htmltableobj.children.length)>cnt)
		cntdel = (htmltableobj.children.length)-cnt;
		
	var cntadd = 0;
	if(cnt>(htmltableobj.children.length))
		cntadd = cnt-(htmltableobj.children.length);

	for(i=0; i<cntadd; i++)
	{
		var div = document.createElement('div');
    htmltableobj.appendChild(div);
		var trDescrPathObj = {};
		trDescrPathObj['nodepath'] = GetPathFromPathId(id);
		trDescrPathObj['rndid'] = GetIdFromPathId(id);
		trDescrPathObj['index'] = htmltableobj.children.length;
		trDescrPathObj['gest'] = 0;
		var str = Mustache.to_html(templates["tablegestureproxiitem"], trDescrPathObj);
		htmltableobj.children[htmltableobj.children.length-1].outerHTML = str;
//		alert(str)
		//htmltableobj.rows[htmltableobj.rows.length].innerHTML=str;
		
		
	}
	for(i=0; i<cntdel; i++)
	{
		htmltableobj.removeChild(htmltableobj.childNodes[htmltableobj.children.length-1]);
	}
	
	
	for(i=0; i<cnt; i++)
	{
		//insert rows to table with indexes
		//probably only add/dell columns in table when not equal with cnt
		//define cnt into ids
	}
	//alert(htmlcntobj.value)
  //update also node with size value
	var path = GetPathFromPathId(id);
  var basepath = ReducePath(path, 1);
  var obj = GetDescrObj(path);
  if(obj)
  {
    if('linkedval' in obj)
    {
      var multi_touch_size_obj = GetDescrObj(basepath+"."+obj['linkedval']);
	    var rndid = GetIdFromPathId(id);
      SetHtmlObjVal(basepath+"."+obj['linkedval']+"-"+rndid, multi_touch_size_obj["template"], cnt);
    }
  }
	//alert(id)
}

function getsettablegesturekeypad(id, type, val)/*1 set val, 0: return val*/
{ 
	var tablehtmlobj = document.getElementById(id+"table");
	if(!tablehtmlobj)
		return "NA";
	var cnthtmlobj = document.getElementById(id+"cnt");
	if(!cnthtmlobj)
		return "NA";
	if(cnthtmlobj)
	{ 
		if((type == 1)||(type == 8))
		{
			if(val)
			{
				if(val == "NULL")
				{
				  cnthtmlobj.value = 0;
				  tablegesturekeypadonchange(0, id);
				}
				else
				{
			    val = val.replace("uint32_t", "");
				  var res = val.replace(/[^0-9a-fA-F.,|x|X]/g, '');
				  arr = res.split(",");
				  cnt = ((arr.length-1));
				  cnthtmlobj.value = cnt;
				  tablegesturekeypadonchange(0, id);
				  for(j=0; j<cnt; j++)
				  {
					 i=j+Number(1);
					 var obj = document.getElementById(id+String(i)+"gest");
					 if(obj) obj.value = arr[0+(Number(j))];
				  }
        }
			}
		}
		else if(type == 6)
		{
			;
		}
		else if((type == 0)||(type == 7))
		{
			var cnt = cnthtmlobj.value
			var j;
			var out;
			if(cnt == 0)
			{
			  out = "NULL";
			}
			else
			{
			 out="(uint32_t []){";
			 for(j=0;j<cnt;j++)
			 {
				var item = "";
				i=j+Number(1);
				var obj = document.getElementById(id+String(i)+"gest");
				if(obj) item = item + obj.value
				item = item + ",";
				out = out + item;
			 }
			 out=out+"0}";
      }
			return out;  
		}
		else
		{
			debug_print("getsettablegestureproxi: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	} 
}


templates["tablepininputenum"] = "<div class=\"select-wrap\">"+
											  "<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}\" disabled>"+
												  "{{#default}}<option value=\"{{item}}\" selected=\"selected\" class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{sublabel}}</option>{{/default}}"+
											  "</select>"+
										  "</div>"

templates["tablepininput"] = "<div class=\"touchsw-table-line touchsw-special-line\">"+
									   "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									   "<div class=\"sub-block\">"+
										   "<div class=\"subline\" ><form id=\"{{nodepath}}-{{rndid}}form\" name=\"pin selector\">"+
											   "<div class=\"radiobox\">"+
												   "<div class=\"cat-button\">"+
														   "<input class=\"checkbox-icon\" checked=\"checked\" value=\"selfcap\" name=\"{{nodepath}}-{{rndid}}rb\" {{#onclickcbck}} onclick=\"{{onclickcbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onclickcbck}} type=\"radio\" />"+
														   "<div class=\"checkbox-bg\"></div>"+
												   "</div>"+
													  "<p class=\"cat-name\">Selfcap<p>"+
											   "</div>"+
											   "<div class=\"radiobox\">"+
												   "<div class=\"cat-button\">"+
														   "<input class=\"checkbox-icon\" value=\"mutual\" name=\"{{nodepath}}-{{rndid}}rb\" {{#onclickcbck}} onclick=\"{{onclickcbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onclickcbck}} type=\"radio\" />"+
														   "<div class=\"checkbox-bg\"></div>"+
												   "</div>"+
												  "<p class=\"cat-name\">Mutual<p>"+
											   "</div>"+
											   "<div class=\"radiobox\">"+
												   "<div class=\"cat-button\">"+
														   "<input class=\"checkbox-icon\" value=\"board.h\" name=\"{{nodepath}}-{{rndid}}rb\"  {{#onclickcbck}} onclick=\"{{onclickcbck}}(this,'{{nodepath}}-{{rndid}}');\" {{/onclickcbck}} type=\"radio\" />"+
														   "<div class=\"checkbox-bg\"></div>"+
												   "</div>"+
												  "<p class=\"cat-name\">File:<p>"+
											   "</div>"+
										 		  "<input class=\"touchsw-file-input\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}boardfile\" value=\"initboarddefault.h\" {{#onchangecbck}} onchange=\"{{onchangecbck}}(this,'{{nodepath}}-{{rndid}}', 0);\" onKeyPress=\"setTimeout('{{onchangecbck}}(this, \\\'{{nodepath}}-{{rndid}}\\\', 1)',1);\"{{/onchangecbck}} disabled></input>"+
												  "<button class=\"touchsw-table-button parse-file\" type=\"button\" onclick=\"{{onclickparse}}('{{nodepath}}-{{rndid}}')\">Parse file</button>"+
										  "</form></div>"+
										  "<div class=\"subline\">"+
											   "<p>Selfcap</p>"+
											   //"<input class=\"touchsw-1-2-input\" placeholder=\"\" type=\"text\" name=\"system-period-time\" id=\"{{nodepath}}-{{rndid}}selfcap\" value=\"0\"></input>"+
                            "<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}selfcap\">"+
											         "<option class=\"selection-name\" title=\"0\" value=\"0\" >0</option>"+
                               "<option class=\"selection-name\" title=\"1\" value=\"1\" >1</option>"+
                               "<option class=\"selection-name\" title=\"2\" value=\"2\" >2</option>"+
                               "<option class=\"selection-name\" title=\"3\" value=\"3\" >3</option>"+
                               "<option class=\"selection-name\" title=\"4\" value=\"4\" >4</option>"+
                               "<option class=\"selection-name\" title=\"5\" value=\"5\" >5</option>"+
                               "<option class=\"selection-name\" title=\"6\" value=\"6\" >6</option>"+
                               "<option class=\"selection-name\" title=\"7\" value=\"7\" >7</option>"+
                               "<option class=\"selection-name\" title=\"8\" value=\"8\" >8</option>"+
                               "<option class=\"selection-name\" title=\"9\" value=\"9\" >9</option>"+
                               "<option class=\"selection-name\" title=\"10\" value=\"10\" >10</option>"+
                               "<option class=\"selection-name\" title=\"11\" value=\"11\" >11</option>"+
                               "<option class=\"selection-name\" title=\"12\" value=\"12\" >12</option>"+
                               "<option class=\"selection-name\" title=\"13\" value=\"13\" >13</option>"+
                               "<option class=\"selection-name\" title=\"14\" value=\"14\" >14</option>"+
                               "<option class=\"selection-name\" title=\"15\" value=\"15\" >15</option>"+
                               "<option class=\"selection-name\" title=\"16\" value=\"16\" >16</option>"+
                               "<option class=\"selection-name\" title=\"17\" value=\"17\" >17</option>"+
                               "<option class=\"selection-name\" title=\"18\" value=\"18\" >18</option>"+
                               "<option class=\"selection-name\" title=\"19\" value=\"19\" >19</option>"+
                               "<option class=\"selection-name\" title=\"20\" value=\"20\" >20</option>"+
                               "<option class=\"selection-name\" title=\"21\" value=\"21\" >21</option>"+
                               "<option class=\"selection-name\" title=\"22\" value=\"22\" >22</option>"+
                               "<option class=\"selection-name\" title=\"23\" value=\"23\" >23</option>"+
                               "<option class=\"selection-name\" title=\"24\" value=\"24\" >24</option>"+
                               "<option class=\"selection-name\" title=\"25\" value=\"25\" >25</option>"+
										        "</select>"+
									   "</div>"+
									   "<div class=\"subline\">"+
											  "<p>Mutual Tx:</p>"+
                            "<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}mutual1\" disabled>"+
											         "<option class=\"selection-name\" title=\"0\" value=\"0\" >0</option>"+
                               "<option class=\"selection-name\" title=\"1\" value=\"1\" >1</option>"+
                               "<option class=\"selection-name\" title=\"2\" value=\"2\" >2</option>"+
                               "<option class=\"selection-name\" title=\"3\" value=\"3\" >3</option>"+
                               "<option class=\"selection-name\" title=\"4\" value=\"4\" >4</option>"+
                               "<option class=\"selection-name\" title=\"5\" value=\"5\" >5</option>"+
										        "</select>"+
										  "</div>"+
										  "<div class=\"subline\">"+
											   "<p>Mutual Rx:</p>"+
                            "<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}mutual2\" disabled>"+
                               "<option class=\"selection-name\" title=\"6\" value=\"6\" >6</option>"+
                               "<option class=\"selection-name\" title=\"7\" value=\"7\" >7</option>"+
                               "<option class=\"selection-name\" title=\"8\" value=\"8\" >8</option>"+
                               "<option class=\"selection-name\" title=\"9\" value=\"9\" >9</option>"+
                               "<option class=\"selection-name\" title=\"10\" value=\"10\" >10</option>"+
                               "<option class=\"selection-name\" title=\"11\" value=\"11\" >11</option>"+
										        "</select>"+
										   "</div>"+
									  "<div class=\"subline\">"+
										   "<p>File options</p>"+
										   templates["tablepininputenum"]+
								   "</div>"+
									   "</div>"+
									   "<div class=\"button-wrap\">"+
										   "{{#rwbase}}"+templates["RWButtons"]+"{{/rwbase}}"+
									   "</div>"+
							  "</div>"
//pridat enum baset template, ktery vytvori 2 polozky v tabulce - sloucene 2 bunky pro nazev a sloucene 2 bunky pro sub tabulku. pak by se melo to ID a funkce zaregistrovat, aby se mohl obsah obnovit i loadcfg funkci. 
function onclicktablepininput(obj, id)
{
	var formhtmlobj = document.getElementById(id+"form");
	if(formhtmlobj)
	{
		var opt = formhtmlobj.elements[id+"rb"];
		var i = 0;
		for(i=0; i<opt.length; i++)
		{
			if(opt[i].checked)
				break;
		}
		var selected = opt[i];
		if(selected)
		{
			if(selected.value == "board.h")
			{
				var itemeobj = document.getElementById(id);
				if(itemeobj) itemeobj.disabled = false;
				itemeobj = document.getElementById(id+"boardfile");
				if(itemeobj) itemeobj.disabled = false;

				itemeobj = document.getElementById(id+"selfcap");
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"mutual1");
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"mutual2");
				if(itemeobj) itemeobj.disabled = true;
				
			}
			else if(selected.value == "selfcap")
			{
				var itemeobj = document.getElementById(id);
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"boardfile");
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"selfcap");
				if(itemeobj) itemeobj.disabled = false;
				itemeobj = document.getElementById(id+"mutual1");
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"mutual2");
				if(itemeobj) itemeobj.disabled = true;
			}
			else if(selected.value == "mutual")
			{
				var itemeobj = document.getElementById(id);
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"boardfile");
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"selfcap");
				if(itemeobj) itemeobj.disabled = true;
				itemeobj = document.getElementById(id+"mutual1");
				if(itemeobj) itemeobj.disabled = false;
				itemeobj = document.getElementById(id+"mutual2");
				if(itemeobj) itemeobj.disabled = false;
			}
		}
	}
}

function SelectRadiotablepininput(id, index)
{
	var formhtmlobj = document.getElementById(id+"form");
	var opt = formhtmlobj.elements[id+"rb"];
	var i;
	//deselect all radiobuttons
	for(i=0;i<opt.length; i++)
		opt[i].checked = false;
	//select the right one radiobutton
	opt[index].checked = true;
}

function getsettablepininput(id, type, val)/*1 set val, 0: return val*/
{ 
	var obj = document.getElementById(id);
	var formhtmlobj = document.getElementById(id+"form");
	//get config state of radiobutton
	var opt = 0;
  if(formhtmlobj)
    opt = formhtmlobj.elements[id+"rb"]
	var i = 0;
	for(i=0; i<opt.length; i++)
	{
		if(opt[i].checked)
			break;
	}
	var selected = opt[i];
	if(obj)
	{ 
		if((type == 1)||(type == 8)) // write(1), write number(8)
		{
			//if(typeof(val) === "string")
			//not sure, what "val" represents, enum in board.h/number for mselfcap/number for mutual
			parseboardcfgupdatepins(id);
			if(selectItemByValue(obj, val))
			{
				SelectRadiotablepininput(id, 2);
				onclicktablepininput(0, id);
			}
			else
			{
				if(isNaN(val))
				{
					//todo select default
				}
				else
				{
					var valnum = Number(val);
					if(valnum<25)
					{
						SelectRadiotablepininput(id, 0);
						onclicktablepininput(0, id);
						var inputhtmlobj = document.getElementById(id+"selfcap");
						//check limits
						if(valnum<obj["selfcaplowlimit"])
							valnum = obj["selfcaplowlimit"];
						if(valnum>obj["selfcaphighlimit"])
							valnum = obj["selfcaphighlimit"];
             
						if(inputhtmlobj) selectItemByValue(inputhtmlobj, String(valnum));
					}
					else
					{
						SelectRadiotablepininput(id, 1);
						onclicktablepininput(0, id);
						//valnum = 6* pin_input_tx + (pin_input_rx – 6) + 25

						var tx = Math.floor((valnum-25)/6);
						var rx =  valnum-25 - (tx*6)+6;
						tx = Math.floor(tx);
						//check limits
						if(tx<obj["mutuallowlimit"])
							tx = obj["mutuallowlimit"];
						if(tx>obj["mutualhighlimit"])
							tx = obj["mutualhighlimit"];
						if(rx<obj["mutuallowlimit"])
							rx = obj["mutuallowlimit"];
						if(rx>obj["mutualhighlimit"])
							rx = obj["mutualhighlimit"];
						var inputhtmlobj = document.getElementById(id+"mutual1");
						//if(inputhtmlobj) inputhtmlobj.value = tx;
            if(inputhtmlobj) selectItemByValue(inputhtmlobj, String(tx));
						inputhtmlobj = document.getElementById(id+"mutual2");
						//if(inputhtmlobj) inputhtmlobj.value = rx;
            if(inputhtmlobj) selectItemByValue(inputhtmlobj, String(rx));	
					}
				}
			}
		}
		else if(type == 6) //update content
		{
			obj.outerHTML = val;
		}
		else if((type == 0) || (type == 7)) // read(0), read number(7)
		{
			var datapath = GetPathFromPathId(id);
			var dataPathObj = GetDescrObj(datapath);

			if(selected.value == "board.h")
			{
				return obj.options[obj.selectedIndex].value;
			}
			else if(selected.value == "selfcap")
			{
				var objself = document.getElementById(id+"selfcap");
				if (objself)
				{
					return objself.selectedIndex;
				}
			}
			else if(selected.value == "mutual")
			{
				var objmuttx = document.getElementById(id+"mutual1");
				var objmutrx = document.getElementById(id+"mutual2");
				var tx = 0;
				var rx = 0;
        
				if (objmuttx) tx = Number(objmuttx.selectedIndex);
				if (objmutrx) rx = Number(objmutrx.selectedIndex)+Number(6);


				var res = 25;
				res = Number(res) + Number(tx*6);
				res = Number(res) + Number(rx-6);
				return res;
			}
		}
		else
		{
			debug_print("getsettableenum: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	} 
}

function onchangetablepininputboard(obj, id, keypress)
{
	var path = GetPathFromPathId(id);
	var rndid = GetIdFromPathId(id);
  var pinInputFilePathObj = GetDescrObj(path);
  if(pinInputFilePathObj && ("pininputboardfilepath" in pinInputFilePathObj))
  {
    var pinInputFilePathStorageObj = GetDescrObj(pinInputFilePathObj["pininputboardfilepath"]);
    var filepath = 0;
    var inputhtmlobj = document.getElementById(id+"boardfile");
    if(inputhtmlobj)
      filepath = inputhtmlobj.value;
    //when is path updated and not default from build html, set the option
    if((filepath) && (filepath != "initboarddefault.h") && (pinInputFilePathStorageObj))
    {
      SetHtmlObjVal(pinInputFilePathObj["pininputboardfilepath"]+"-"+pinInputFilePathStorageObj['rndid'], pinInputFilePathStorageObj['cfgfile'], filepath);
    }
    else if((filepath) && (filepath == "initboarddefault.h") && (keypress === undefined) && (pinInputFilePathStorageObj))
    {
      //get stored val
      filepath = GetHtmlObjVal(pinInputFilePathObj["pininputboardfilepath"]+"-"+pinInputFilePathStorageObj['rndid'], pinInputFilePathStorageObj['cfgfile'], 0);
      //set val to new el
      inputhtmlobj.value = filepath;
    }
    //sync all board.h
    if(keypress)
      syncpininnputboardpath(filepath, id);
  }
}

function syncpininnputboardpath(filepath, id)
{
	var path = GetPathFromPathId(id);
  var el_path = ReducePath(path, 1);
  var elObj = GetDescrObj(el_path);
	var linksobj = elObj["links"];
  var base_rndid = GetIdFromPathId(id);
  var pinInputFilePathObj = GetDescrObj(path);
  for( link in linksobj)
  {
      rndid = GetLinkId(linksobj[link]);
      //skip object with same id
      if(base_rndid == rndid)
        continue;
      var inputhtmlobj = document.getElementById(path+"-"+rndid+"boardfile");
      if(inputhtmlobj)
        inputhtmlobj.value = filepath;
  }
  //alert(filepath);

}

templates["tablegpioinputenumSC"] = "<div class=\"select-wrap\">"+
											  "<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}sc\">"+
												  "{{#default}}<option value=\"{{item}}\" class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}} {{selected}}>{{sublabel}}</option>{{/default}}"+
											  "</select>"+
										  "</div>"

templates["tablegpioinputenumMUTRX"] = "<div class=\"select-wrap\">"+
											  "<select class=\"choose unselected\" name=\"choose_user\" id=\"{{nodepath}}-{{rndid}}mutrx\">"+
												  "{{#default1}}<option value=\"{{item}}\" class=\"selection-name\" {{#descr}}title=\"{{descr}}\" {{/descr}} {{selected}}>{{sublabel}}</option>{{/default1}}"+
											  "</select>"+
										  "</div>"
templates["tablegpioinput"] ="<div class=\"touchsw-table-line touchsw-special-line\" {{#hidden}}style=\"display:none;\"{{/hidden}}  id=\"{{nodepath}}-{{rndid}}tr\">"+
									   "<p class=\"touchsw-table-name\" {{#descr}}title=\"{{descr}}\" {{/descr}}>{{label}}</p>"+
									   "<div class=\"sub-block\">"+
										  "<div class=\"subline\">"+
											   "<p>Selfcap/Mutual Tx</p>"+
											     templates["tablegpioinputenumSC"]+
									   "</div>"+
									   "<div class=\"subline\">"+
										  "<div class=\"subline\">"+
											   "<p>Mutual Rx:</p>"+
                            templates["tablegpioinputenumMUTRX"]+
										   "</div>"+
									   "</div>"+
									   "<div class=\"button-wrap\">"+
										   "{{#rwbase}}"+templates["RWButtons"]+"{{/rwbase}}"+
									   "</div>"+
							  "</div>"
function getsettablegpioinput(id, type, val)/*1 set val, 0: return val*/
{
	var enumhtmlscobj = document.getElementById(id+"sc");
  var enumhtmlmutrxobj = document.getElementById(id+"mutrx");
	if(enumhtmlscobj)
	{ 
		if((type == 1)||(type == 8)) // write(1), write number(8)
		{
			//if(typeof(val) === "string")
			//not sure, what "val" represents, enum in board.h/number for mselfcap/number for mutual

      var begin = val.indexOf('{');
      var end = val.indexOf('}');
      var sc_str = "";
      var mutrx_str = "";
      if((begin>=0) && (end>=0))
      {
        var params = val.substring(begin+Number(1), end);
        var paramsarr = params.split(",");
        if(paramsarr.length>0)
          sc_str = paramsarr[0];
        if(paramsarr.length>1)
          mutrx_str = paramsarr[1];
      }
      else
      {
        sc_str = val;
      }
      if(enumhtmlscobj && sc_str)
        selectItemByValue(enumhtmlscobj, sc_str);
      if(enumhtmlmutrxobj)
      {
        if(mutrx_str)
          selectItemByValue(enumhtmlmutrxobj, mutrx_str);
        else
          selectItemByValue(enumhtmlmutrxobj, "NONE");
      }
        
		}
		else if(type == 6) //update content
		{
			obj.outerHTML = val;
		}
		else if((type == 0) || (type == 7)) // read(0), read number(7)
		{
      var sc_str = "";
      var mutrx_str = "";
      
      if(enumhtmlscobj)
        sc_str = enumhtmlscobj.options[enumhtmlscobj.selectedIndex].value
      if(enumhtmlmutrxobj)
        mutrx_str = enumhtmlmutrxobj.options[enumhtmlmutrxobj.selectedIndex].value
      if((mutrx_str) && (mutrx_str != "NONE"))
        return "{"+sc_str+","+mutrx_str+"}";
      else
        return sc_str;
		}
		else
		{
			debug_print("getsettableenum: type\"" + type + "\"not implemented, "+ id +".", true);
		}
	}
}
templates["rb_basic_exp"] = "<form id = \"{{nodepath}}-{{rndid}}_form\"><div class=\"radiobox\">"+
								"<div class=\"cat-button\">"+
										"<input class=\"checkbox-icon\" name=\"radio\" type=\"radio\" id=\"{{nodepath}}-{{rndid}}rb\" value=\"Basic\" {{#onchange}}onchange=\"{{onchange}}(this, '{{nodepath}}-{{rndid}}', '0')\" {{/onchange}}/>"+
										"<div class=\"checkbox-bg\"></div>"+
								"</div>"+
								 "<p class=\"cat-name\">Basic<p>"+
							"</div>"+
							"<div class=\"radiobox\">"+
								"<div class=\"cat-button\">"+
										"<input class=\"checkbox-icon\" name=\"radio\" type=\"radio\" id=\"{{nodepath}}-{{rndid}}rb\" {{#onchange}}onchange=\"{{onchange}}(this, '{{nodepath}}-{{rndid}}', '1')\" {{/onchange}} value=\"Expert\" checked=\"checked\" />"+
										"<div class=\"checkbox-bg\"></div>"+
								"</div>"+
							 "<p class=\"cat-name\">Expert<p>"+
							"</div></form>"

//templates["rb_basic_exp"] = "<input type=\"radio\" name=\"mode\" id=\"{{nodepath}}-{{rndid}}_0\" value=\"Basic\" {{#onchange}}onchange=\"{{onchange}}(this, '{{nodepath}}-{{rndid}}', '0')\" {{/onchange}}> Basic <input type=\"radio\" name=\"mode\" id=\"{{nodepath}}-{{rndid}}_1\" {{#onchange}}onchange=\"{{onchange}}(this, '{{nodepath}}-{{rndid}}', '1')\" {{/onchange}} value=\"Expert\" checked=\"checked\"> Expert";
                                            
templates["tablelinkscope"] = "<div class=\"touchsw-table-line\">"+
									 "<p class=\"touchsw-table-name\">{{label}}</p>"+
									 "<p class=\"touchsw-table-link\"><a onclick='onclicktablelinkscope(\"{{index}}\");'>{{linklabel}} >></a></p>"+
							"</div>";
function onclicktablelinkscope(index)
{
  if(NTElectrode.All[Number(index)])
    NTElectrode.All[Number(index)].OnLinkClicked()
}

var callbacktemplates = [];
//the index of the array is dedicated to index in xml nt_control.interface.default
//keypad
callbacktemplates[4] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                              enum nt_control_keypad_event event,\n \
                              uint32_t index)\n \
{\n \
  switch(event)\n \
  {\n \
  case NT_KEYPAD_RELEASE:\n \
    switch (index) {\n \
        case 0:    \n \
        // your code     \n \
    break;\n \
        case 1:  \n \
        // your code     \n \
    break;\n \
        case 2:  \n \
        // your code     \n \
    break;\n \
        case 3:  \n \
        // your code     \n \
    break;\n \
        default:\n \
        // your code     \n \
    break;\n \
    }\n \
    break;\n \
  case NT_KEYPAD_TOUCH:\n \
    switch (index) {\n \
        case 0:    \n \
        // your code     \n \
    break;\n \
        case 1:  \n \
        // your code     \n \
    break;\n \
        case 2:  \n \
        // your code     \n \
    break;\n \
        case 3:  \n \
        // your code     \n \
    break;\n \
        default:\n \
        // your code  \n \
    }\n \
    break;   \n \
  }\n \
}\n \
*/\n \
\n";
//DSlider
callbacktemplates[3] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                               enum nt_control_aslider_event event,\n \
                               uint32_t position)\n \
{\n \
switch(event)\n \
    {\n \
    case NT_SLIDER_MOVEMENT:       \n \
        // your code     \n \
    break;\n \
    case NT_SLIDER_ALL_RELEASE:    \n \
        // your code     \n \
    break;\n \
    case NT_SLIDER_INITIAL_TOUCH:  \n \
        // your code     \n \
    default:\n \
        // your code     \n \
    break;  \n \
  }\n \
}\n \
*/\n \
\n";
//DRotary
callbacktemplates[1] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                               enum nt_control_arotary_event event, \n \
                               uint32_t position)\n \
{\n \
switch (event) {\n \
    case NT_ROTARY_MOVEMENT:\n \
        // your code     \n \
    break;\n \
    case NT_ROTARY_ALL_RELEASE:   \n \
        // your code     \n \
    break;\n \
    case NT_ROTARY_INITIAL_TOUCH: \n \
        // your code     \n \
    break;\n \
    default:\n \
        // your code     \n \
    break;\n \
  }\n \
}\n \
*/\n \
\n";
//ASlider
callbacktemplates[2] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                               enum nt_control_aslider_event event,\n \
                               uint32_t position)\n \
{\n \
switch(event)\n \
    {\n \
    case NT_ASLIDER_MOVEMENT:       \n \
        // your code     \n \
    break;\n \
    case NT_ASLIDER_ALL_RELEASE:    \n \
        // your code     \n \
    break;\n \
    case NT_ASLIDER_INITIAL_TOUCH:  \n \
        // your code     \n \
    default:\n \
        // your code     \n \
    break;   \n \
  }\n \
}\n \
*/\n \
\n";
//ARotary
callbacktemplates[0] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                               enum nt_control_arotary_event event, \n \
                               uint32_t position)\n \
{\n \
switch (event) {\n \
    case NT_AROTARY_MOVEMENT:\n \
        // your code     \n \
    break;\n \
    case NT_AROTARY_ALL_RELEASE:   \n \
        // your code     \n \
    break;\n \
    case NT_AROTARY_INITIAL_TOUCH: \n \
        // your code     \n \
    break;\n \
    default:\n \
        // your code     \n \
    break;\n \
  }\n \
}\n \
*/\n \
\n";
//Proxi
callbacktemplates[5] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                             enum nt_control_proxi_event event,\n \
                             uint32_t index_or_gesture, uint32_t proximity)\n \
{\n \
  switch(event)\n \
  {\n \
  case NT_PROXI_RELEASE:\n \
    switch (index_or_gesture) {\n \
        case 0:\n \
            // your code  \n \
        break;            \n \
        case 1:\n \
            // your code \n \
        break;            \n \
        case 2:\n \
            // your code \n \
        break;            \n \
        case 3:\n \
            // your code  \n \
        break;                       \n \
        default:\n \
        break;\n \
    }\n \
    break;\n \
  case NT_PROXI_TOUCH:\n \
    switch (index_or_gesture) {\n \
        case 0:\n \
            // your code  \n \
        break;            \n \
        case 1:\n \
            // your code \n \
        break;            \n \
        case 2:\n \
            // your code \n \
        break;            \n \
        case 3:\n \
            // your code  \n \
        break;                       \n \
        default:\n \
        break;\n \
    }\n \
    break;\n \
  case NT_PROXI_MOVEMENT:\n \
    switch (index_or_gesture) {\n \
        case 0:\n \
            // your code  \n \
        break;            \n \
        case 1:\n \
            // your code \n \
        break;            \n \
        case 2:\n \
            // your code \n \
        break;            \n \
        case 3:\n \
            // your code  \n \
        break;                       \n \
        default:\n \
        break;   \n \
    }\n \
    break;  \n \
  case NT_PROXI_GET_GESTURE:\n \
    switch (index_or_gesture) {\n \
        case 0:\n \
            // your code  \n \
        break;            \n \
        case 1:\n \
            // your code \n \
        break;            \n \
        case 2:\n \
            // your code \n \
        break;            \n \
        case 3:\n \
            // your code  \n \
        break;                       \n \
        default:\n \
        break;\n \
    }\n \
    break;  \n \
  case NT_PROXI_SET_GESTURE:\n \
    switch (index_or_gesture) {\n \
        case 0:\n \
            // your code  \n \
        break;            \n \
        case 1:\n \
            // your code \n \
        break;            \n \
        case 2:\n \
            // your code \n \
        break;            \n \
        case 3:\n \
            // your code  \n \
        break;                       \n \
        default:\n \
        break;\n \
    }    \n \
        // your code \n \
    break;  \n \
  break; \n \
  }\n \
}\n \
*/\n \
\n";
//Matrix
callbacktemplates[6] = "/*static void {{name}}_callback_template(const struct nt_control *control, \n \
                              enum nt_control_matrix_event event,\n \
                              nt_coordinates8_xy *position,\n \
                              uint32_t index_or_gesture)\n \
{ \n \
    // according to event switch to: \n \
    switch(event)\n \
    {\n \
    case NT_MATRIX_GET_CONTROL_GESTURE: \n \
    {   enum nt_control_matrix_control_gestures control_gesture = (enum nt_control_matrix_control_gestures)index_or_gesture;  \n \
        switch (control_gesture)\n \
        {\n \
        case NT_MATRIX_DOUBLE_TAP: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_COVER: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_SWIPE_UP: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_SWIPE_DOWN: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_SWIPE_LEFT: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_SWIPE_RIGHT: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_ZOOM_IN: \n \
            // your code \n \
        break;\n \
        case NT_MATRIX_ZOOM_OUT: \n \
            // your code \n \
        break;\n \
        }\n \
        break; \n \
    }\n \
    case NT_MATRIX_MOVEMENT:             \n \
            // your code    \n \
        break;            \n \
    case NT_MATRIX_RELEASE:                   \n \
            // your code  \n \
        break;\n \
    case NT_MATRIX_TOUCH:                   \n \
            // your code  \n \
        break;                           \n \
    case NT_MATRIX_RELEASE_SET_GESTURE:  \n \
        switch (index_or_gesture)\n \
        {\n \
        case 1: \n \
            // your code \n \
        break;\n \
        case 2: \n \
            // your code \n \
        break;\n \
        case 3: \n \
            // your code \n \
        break;\n \
        } \n \
        break;   \n \
    case NT_MATRIX_RELEASE_GET_GESTURE:  \n \
        switch (index_or_gesture)\n \
        {\n \
        case 1: \n \
            // your code \n \
        break;\n \
        case 2: \n \
            // your code \n \
        break;\n \
        case 3: \n \
            // your code \n \
        break;\n \
        }     \n \
        break;\n \
    case NT_MATRIX_GESTURE_TOO_LONG:     \n \
            // your code     \n \
        break;          \n \
    }\n \
}\n \
*/\n \
\n";
callbacktemplates['System'] = "/* Call on the TSI CNTR overflow 16-bit range (65535) */\n \
/*void system_0_callback_template(uint32_t event, union nt_system_event_context * context)\n \
{\n \
  switch(event)\n \
  {\n \
    case NT_SYSTEM_EVENT_OVERRUN:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_DATA_READY:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_MODULE_DATA_READY:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_DATA_OVERFLOW:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_SIGNAL_LOW:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_SIGNAL_HIGH:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_ELEC_SHORT_VDD:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_ELEC_SHORT_GND:\n \
        // your code \n \
    break;\n \
    case NT_SYSTEM_EVENT_ELEC_SHORT_ADJ:\n \
        // your code \n \
    break;\n \
  }\n \
}\n \
*/\n \
\n";