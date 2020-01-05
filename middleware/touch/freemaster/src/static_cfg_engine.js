/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

function ReducePath(pathstr, numitems)
{
  pathout = "";
  var patharr = pathstr.split(".");
  var i = 0;
  if(patharr.length<=numitems)
	return patharr[0];
  for(i = 0; i < (patharr.length-numitems); i++)
  {
	if(i != 0)
	  pathout = pathout + ".";
	pathout = pathout + patharr[i];
  }
  return pathout;
}

function IsValidDescrObjPath(idpath)
{
	return IsValidDataPath(data, idpath);
}

function GetDescrObj(idpath)
{
    return GetDataObj(data, idpath);
}

function GetDescrObj2(idpath)
{
    return GetDataObj2(data, idpath);
}

function GetPathFromPathId(pathid)
{
    return pathid.substring(0, pathid.indexOf("-"));
}

function GetIdFromPathId(pathid)
{
    return pathid.substring(pathid.indexOf("-")+Number(1));
}

function GetListOfLinks(obj)
{
	var out = [];
	var index = 0;
	for(linkarr in obj)
	{
		for(linkid in obj[linkarr])
		{
			var arr = {};
			arr['key'] = linkid;
			arr['val'] = obj[linkarr][linkid];
			arr['index'] = index;
			out.push(arr);
		}
	}
	return out;
}

function GetLinkId(linkobj)
{
	if(!linkobj)
		return -1;
	for(id in linkobj)
		return id;
	return -1;
}

function InitLinks(rndid)
{
	var link = {};
	link[rndid] = "";
	var tmp = [];
	tmp.push(link);
	return tmp;
}

function GetWebContentFromObj2(obj, nodepath, templatekey, rndid, filter)
{
    var str = "";
    if(typeof(obj) != 'object')
    	return "";
    console.log(obj[templatekey] +"   :"+typeof(obj));
    
    if(templatekey in obj)
    {
        obj['nodepath'] = nodepath;
        obj['rndid'] = rndid;
        var insertitem = true;
        if(("imgdescr" in obj) && filter)
        {
          if(obj["imgdescr"] != filter)
            insertitem = false;
        }
        if(insertitem)
          str = str + Mustache.to_html(templates[obj[templatekey]], obj)
    }
    /*if("inclsubstructenum" in obj)
    {
        var subdivDescrPathObj = GetDescrObj(obj['inclsubstructenum']);
        if(!("links" in subdivDescrPathObj))
        {
            subdivDescrPathObj["links"] = [];
        }
        var link = {};
        link[rndid] = nodepath;
        subdivDescrPathObj["links"].push(link);
        subdivDescrPathObj['rndid'] = rndid;
        str = GetWebContentFromObj2(subdivDescrPathObj, obj['inclsubstructenum'], "template", subdivDescrPathObj['rndid']);

    }*/
    if(("inclsubstruct" in obj) && ("template" in obj) && (filter != "noincl"))
    {
    	console.log(obj["inclsubstruct"])
    	var InclSubStrDescrPathObj = GetDescrObj(obj["inclsubstruct"]);
    	if(!("links" in InclSubStrDescrPathObj))
    	{
    		InclSubStrDescrPathObj["links"] = [];
    	}
        //TODO!! use template to define subtable!!!
        var insertitem = true;
        if(("imgdescr" in obj) && filter)
        {
          if(obj["imgdescr"] != filter)
            insertitem = false;
        }
        if(insertitem)
        {
          var link = {};
          rndid = GetRandObjId();
          link[rndid] = nodepath;
          InclSubStrDescrPathObj["links"].push(link);
          InclSubStrDescrPathObj['rndid'] = rndid;
    	    str = str + "<tr><td colspan = \"2\">" + GetWebContentFromObj2(InclSubStrDescrPathObj, obj["inclsubstruct"], templatekey, rndid, filter) + "</td></tr>";
        }
    }
    for( key in obj)
    {
    	console.log("key:"+key);
    	str = str + GetWebContentFromObj2(obj[key], nodepath+"."+key, templatekey, rndid, filter);
    }
    return str;
}

function GetWebContentFromObj(obj, nodepath, templatekey)
{
	var str = "";
    console.log(obj[templatekey]);
    if(templatekey in obj)
	{
		console.log(obj[templatekey]);
		obj['nodepath'] = nodepath;
    	str = str + Mustache.to_html(templates[obj[templatekey]], obj)
	}
    if('table' in obj)
	{
	    var subobj = obj["table"];
		for( key in subobj)
	    {
			str = str + GetWebContentFromObj(subobj[key], nodepath+".table."+key, templatekey);
	    }
		
	}
    return str;
}

function GetDataFromWeb2(obj, nodepath, key, rndid, nolinks)
{
    //list all items, when linked folow the linked
    if(typeof(obj) != 'object')
        return "";
    if(obj['cfgfile'])
        console.log("cfgfile:" + obj['cfgfile'] + " :"+rndid);
    
    if(!rndid)
    	rndid = 1;

    var linkarr = [];
    if((!('links' in obj))||(nolinks))
    {
        var link = {};
        link['key'] = rndid
        link['val'] = nodepath;
        linkarr.push(link);
    }
    else
    {
    	linkarr = GetListOfLinks(obj["links"]);
    }
    
    var arr = [];
    for( link in linkarr)
    {
        str = "";
        rndid = linkarr[link]['key'];
        if(("inclsubstruct" in obj) || ("inclsubstructdyn" in obj))
        {
            var structname = "";
            if("inclsubstruct" in obj)
                structname = obj["inclsubstruct"];
            if("inclsubstructdyn" in obj)
                structname = obj["inclsubstructdyn"];
        	var InclSubStrDescrPathObj = GetDescrObj(structname);
            //todo get the rnd id from data or from web.
        	if(!("links" in InclSubStrDescrPathObj))
          {
        		InclSubStrDescrPathObj["links"] = [];
            if("sructname" in InclSubStrDescrPathObj)
            {
              if("name" in InclSubStrDescrPathObj["sructname"])
              {
                if(InclSubStrDescrPathObj["sructname"]["name"] == "tsi_hw_config")
                {
                  var linksobj = InitLinks(rndid);
    	             InclSubStrDescrPathObj["links"] = linksobj;
                }
              }
            }
          }
        	
            var linksobj = InclSubStrDescrPathObj["links"]
            var listitems = linksobj.childNodes
            var val = GetDataFromWeb2(InclSubStrDescrPathObj, structname, "", InclSubStrDescrPathObj['rndid']);
            arr.push(val)
            continue;
        }
        if('cfgfile' in obj)
        {
        	var val = GetHtmlObjVal(nodepath, obj['cfgfile']);
            arr.push(val);
            continue;
        }
        var outarr = {};
        for( key in obj)
        {
            if(typeof(obj) != 'object')
                continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;
            console.log("key:"+key);

            if('cfgfile' in subobj)
            {
            	var val = GetDataFromWeb2(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);

            	//change the val, when checkboxes are expected on page, on path in sync array
            	if((subobj['cfgfile'] == "synccheckbox") && (key == "checkbox") && ('syncs' in obj))
            	{
            		//iterate all sync paths
            		var cbsyncout = [];
            		for(syncindex in obj['syncs'])
            		{
            			var syncObjDescrPathObj = GetDescrObj(obj['syncs'][syncindex]);
            			//skip when electrodes not detected
                  if(!syncObjDescrPathObj)
                    continue;
            			if(!('activetmpl' in syncObjDescrPathObj))
            				continue;
            			
            			var cbsyncitemout = [];
            			var basepath = ReducePath(obj['syncs'][syncindex], 1);
            			var baseDescrPathObj = GetDescrObj(basepath);
            			var controls = GetListOfLinks(baseDescrPathObj["links"]);
            			for(control in controls)
            			{
            				var cbval = GetHtmlObjVal(controls[control]['key'] + "_" + rndid, syncObjDescrPathObj["activetmpl"]);
            				console.log(controls[control]['key'] + " " + rndid + ":"+cbval);
            				var cbarr = {};
            				if(!(controls[control]['val']))
            					controls[control]['val'] = "a";
            				cbarr[controls[control]['val']] = cbval;
            				cbsyncitemout.push(cbarr);
            			}
            			var cbsyncitemdict = {};
            			cbsyncitemdict[obj['syncs'][syncindex]] = cbsyncitemout;
            			cbsyncout.push(cbsyncitemdict);
            		}
            		val = cbsyncout;
            	}
            	outarr[key] = val;
            	if(subobj['cfgfile'] == "tableenumsubstr")
            	{
                    //get index of enum
                    var index = -1;
                    for(objitem in subobj["default"])
                    {
                        if(subobj["default"][objitem]['item'] == val[0])
                        {
                            index = objitem;
                            break;
                        }
                    }
                    if(index>=0)
                    {
                    	if('inclsubstructenum' in subobj['default'][index])
                    	{
                        	var substructpath = subobj['default'][index]['inclsubstructenum'];
                        	var subsructDescrPathObj = GetDescrObj(substructpath);
                        	outarr["tableenumsubstr"] = GetDataFromWeb2(subsructDescrPathObj, substructpath, substructpath, rndid, 1);                    		
                    	}
                    }
            	}
            }
        }
        if(outarr)
        	arr.push(outarr);
    }
    return arr;
}        

function DeleteDynSubStruct(obj, path, resetindex)
{
    if("inclsubstructdyn" in obj)
    {
    	//destroy all dyn content, create new, count is defined by
    	var substructDescrPathObj = GetDescrObj(obj['inclsubstructdyn']);
    	
    	if("links" in substructDescrPathObj)
    	{
    		var linksobj = substructDescrPathObj["links"]
    		var len = linksobj.length;
    		for(linkindex=0; linkindex<len; linkindex++)
    		{
    	        str = "";
    	        var substructrndid = GetLinkId(linksobj[0]);
    	        DelSubStructTab(path, substructrndid, resetindex);
    		}
    	}
    }
}

function SetDataToWeb2(obj, cfgdata, key, id, rndid, nolinkarg)
{
	if(typeof(obj) != 'object')
        return "";
    var nolink = nolinkarg;
	if(obj['cfgfile'])
        console.log("cfgfile:" + obj['cfgfile'] + " :"+rndid + "  :" + id);
    if(!rndid)
    	rndid = 1;

    if("inclsubstructdyn" in obj)
    {
        var path = GetPathFromPathId(id);
    	//destroy all dyn content, create new, count is defined by
    	DeleteDynSubStruct(obj, path)
    	
    	//get cnt in nt_electrodes.control_count-303583225
    	var basepath = ReducePath(path, 1);
    	var ctrlCntDescrPathObj = GetDescrObj(basepath+".control_count");
    	var cnt = GetDataFromWeb2(ctrlCntDescrPathObj, basepath+".control_count-"+rndid, "control_count", rndid);
    	//build tabs with default values
    	for(var index = 0; index<cnt;index++)
    	{
    		AddSubStructTab(basepath, index+Number(1));
    	}
        //todo update content
    	substructDescrPathObj = GetDescrObj(obj['inclsubstructdyn']);
    	if(!('links' in substructDescrPathObj))
    		substructDescrPathObj['links'] = {};
    	var linksobj = substructDescrPathObj["links"];
    	for(link in cfgdata[0])
    	{
            var substructrndid = GetLinkId(linksobj[link]);
            SetDataToWeb2(substructDescrPathObj, cfgdata[0][link], obj['inclsubstructdyn'], obj['inclsubstructdyn'], substructrndid, 1);
    	}
    	//sync content
    	for(i = 0; i<cnt;i++)
    		SyncSubStructTab(basepath, i);
    }
    
    var linksobj = obj["links"];
    if(!('links' in obj))
    {
        linksobj = InitLinks(rndid);
        obj["links"] = linksobj;
    }
    else if (nolink)
    {
    	linksobj = InitLinks(rndid);
    }
    
    
    for( link in linksobj)
    {
        var linkrndid = GetLinkId(linksobj[link]);

        //var sobj = cfgdata[0];
        if(("inclsubstruct" in obj) || ("inclsubstructdyn" in obj))
        {
        	var structname = "";
        	if("inclsubstruct" in obj)
        		structname = obj["inclsubstruct"];
            if("inclsubstructdyn" in obj)
                structname = obj["inclsubstructdyn"];
        	//todo create new tab and div
        	var substructDescrPathObj = GetDescrObj(structname);
            //todo get the rnd id from data or from web.
            var substructlinksobj = substructDescrPathObj["links"];
            if(typeof(substructlinksobj) != 'object')
            	continue;
            for( link2 in substructlinksobj)
            {
                var linkrndid = GetLinkId(substructlinksobj[link2]);

                //var val = GetDataFromWeb2(ctrlCntDescrPathObj, obj["inclsubstruct"], "", ctrlCntDescrPathObj['rndid']);
                //todo delete the content and generate requred count of items
                SetDataToWeb2(substructDescrPathObj, cfgdata[0][link2], structname, structname, linkrndid, 1);
            }
            //todo sync the electrode names

            continue;
        }
    	//alert(cfgdata + "  " + key + "   " + data['cfgfile'])
    	if((cfgdata) && ('cfgfile' in obj))
        {
    		var val = SetHtmlObjVal(id, obj['cfgfile'], cfgdata[0]);
            continue;
        }
        for( key in obj)
        {
            console.log("key:"+key);

        	if(typeof(obj) != 'object')
                continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;
            if(!(key in cfgdata))
            	continue;
            var subcfgobj = cfgdata[key];

            if('cfgfile' in subobj)
            {
                //SetDataToWeb2(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
                //alert(key+"  "+subobj)
                SetDataToWeb2(subobj, subcfgobj, key, id+"."+key+"-"+rndid, rndid);
                if((subobj['cfgfile'] == "tableenumsubstr"))
                {
                	//update dyn substructure by the loaded config
                	if(('onfocuscbck' in subobj)&&(subobj['cfgfile'] == "tableenumsubstrshareditem"))
                	{
                        var finalArgs = [];
                        finalArgs.push(null);
                        finalArgs.push(id+"."+key+"-"+rndid);
                        var val = CallFunctStr(subobj["onfocuscbck"], finalArgs);
                	}
                	if('onchangecbck' in subobj)
                	{
                        var finalArgs = [];
                        finalArgs.push(null);
                        finalArgs.push(id+"."+key+"-"+rndid);
                        var val = CallFunctStr(subobj["onchangecbck"], finalArgs);
                	}
                    var substructname = "";
                    for (substritem in subobj['default'])
                    {
                    	if(subobj['default'][substritem]['item']== subcfgobj[0])
                    	{
                            if('inclsubstructenum' in subobj['default'][substritem])
                            {
                        		substructname = subobj['default'][substritem]['inclsubstructenum'];                            	
                            }
                            else
                            {
                            	substructname = "NULL";
                            }
                    		break;
                    	}
                    	
                    }
                    if((substructname != "NULL") && (subobj['cfgfile'] == "tableenumsubstr"))
                    {
                        var substructDescrPathObj = GetDescrObj(substructname);
                    	SetDataToWeb2(substructDescrPathObj, cfgdata['tableenumsubstr'][0], key, substructname, linkrndid, 1);
                    	//update also checkboxes for electrodes
                    	if(('checkbox' in obj) && ('checkbox' in cfgdata))
                    	{
                    		for(cboxlink in cfgdata['checkbox'])
                    		{
                    			for(linkedpath in cfgdata['checkbox'][cboxlink])
                    			{
                    				var xboxDescrPathObj = GetDescrObj(linkedpath);
                    				base_linkedpath = ReducePath(linkedpath, 1);
                    				controlslinkDescrPathObj = GetDescrObj(base_linkedpath+".links");
                    				var rndidindex = 0;
                    				for(forindex in controlslinkDescrPathObj)
                    				{
                        				var val = 1;
                        				for(xboxitem in cfgdata['checkbox'][cboxlink][linkedpath][forindex])
                        				{
                        					if(xboxitem in cfgdata['checkbox'][cboxlink][linkedpath][forindex])
                        					{
                        						if(cfgdata['checkbox'][cboxlink][linkedpath][forindex][xboxitem])
                        						{
                        							val = cfgdata['checkbox'][cboxlink][linkedpath][forindex][xboxitem];
                        							rndidindex = GetLinkId(controlslinkDescrPathObj[forindex]);
                        							if((xboxDescrPathObj['nodepath'].indexOf("nt_control.") == 0) && (val&1))
                        							{
                        								controlcheckboxitem(0, controlslinkDescrPathObj, obj['links'], 0, rndid);
                        								controltableeditshilding(0, obj['links'], obj['links'], 0, rndid);
                        								var elbase = GetDescrObj(xboxDescrPathObj['datasrc']);
                        								if(!('nt_control_disabled' in elbase))
                        									elbase['nt_control_disabled'] = {};
                        								elbase['nt_control_disabled'][rndidindex + "_" + rndid] = rndid;
                        								if(!('nt_electrode_disabled' in elbase))
                        									elbase['nt_electrode_disabled'] = {};
                        								elbase['nt_electrode_disabled'][rndidindex + "_" + rndid] = rndid;

                        								//list all electrodes and disable electrode in shielding enum for all electrodes
                        							}
                        						}
                        						else
                        						{
                        							val = 0;
                        						}
                        					}
                        				}

                        				rndidindex = GetLinkId(controlslinkDescrPathObj[forindex]);
                        				SetHtmlObjVal(rndidindex + "_" + rndid, xboxDescrPathObj["activetmpl"], val);
                    				}
                    			}
                    		}
                    	}
                    	if(('shielding_electrode' in obj) && ('shielding_electrode' in cfgdata) && (subobj['cfgfile'] == "tableenumsubstr"))
                    	{
                    		if(cfgdata['shielding_electrode'] != 'NULL')
							{
	                    		//kdyz neni null, znamena to, ze je vybrana elektroda
								//musi se najit v konfiguraci, jake ID ma vybrana elektroda
								//ID pak vlozit do pole [nt_control_disabled]
                    			var elid = GetEllIdByNametableeditshilding(obj, cfgdata['shielding_electrode'][0])
                    			if(elid)
                    			{
    								if(!('nt_control_disabled' in obj))
    									obj['nt_control_disabled'] = {};
                    				obj['nt_control_disabled'][elid+"_"+elid] = elid;
                    			}
	                    	}
                    	}
                    }
                }
            }
        }
    }
}

function GenFileContent2(obj, nodepath, base_key, rndid)
{
	//list all items, when linked folow the linked
	var structnamestr = "";
	var structendstr = "";
	var str = "";
	var finalstr = "";
    if(typeof(obj) != 'object')
        return "";
	if(obj['outfile'])
        console.log("outfile:" + obj['outfile']);
    if(!rndid)
    	rndid = 1;

	//todo get nodepath and rndid from linked list
    var linksobj = obj["links"];
    if((!('links' in obj)) && (rndid))
    {
    	linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
    }
    for( link in linksobj)
    {
    	str = "";
    	rndid = GetLinkId(linksobj[link]);
        if('sructname' in obj)
        {
        	var subobj = obj['sructname'];
        	if('name' in subobj)
        	{
                var structstring = "struct";
                if('nostruct' in subobj)
                	structstring = "";
        		var conststr = "const";
        		if('noconst' in subobj)
        			conststr = "";
        		if('constopt' in subobj)
        		{
        			var constval = GetHtmlObjVal( subobj['constopt'], "checkboxitem");
        			if(constval)
        				conststr = "";
        		}
        		
        		//this code can read option of const, when sub structure has enum
        		var baseDescrPathObj = GetDescrObj(nodepath);
        		if(baseDescrPathObj)
        		{
        			if('const' in baseDescrPathObj)
        			{
        				var constval = "";
        				if('template' in baseDescrPathObj['const'])
        					constval = GetHtmlObjVal( nodepath + ".const-"+ rndid, baseDescrPathObj['const']['template']);
            			if(constval == "false")
            				conststr = "";
        			}
        		}
        		structnamestr = conststr + " "+structstring+" "+ nodepath + " " +subobj['name']+ " = { \n";
        		structendstr = "};\n";
        	}else if('linkedname' in subobj)
        	{
        		var linkedpath = linksobj[link][rndid];
        		var base_path = ReducePath(linkedpath, 2);
        		var sructnameDescrPathObj = GetDescrObj(base_path+".sructname");
        		var elname = GetHtmlObjVal(base_path+".sructname-"+rndid, sructnameDescrPathObj["outfile"]);
        		var conststr = "const ";
        		if('constopt' in subobj)
        		{
        			var constval = GetHtmlObjVal( subobj['constopt'], "checkboxitem");
        			if(constval)
        				conststr = "";
        		}
        		structnamestr = conststr + "struct "+ nodepath + " " +subobj['linkedname']+"_"+elname+ " = { \n";
                structendstr = "};\n";        		
        	}
        	else if('outfile' in subobj)
        	{
        		var val = GetHtmlObjVal(nodepath+".sructname-"+rndid, subobj['outfile']);
        	    //alert(nodepath+".sructname-"+rndid + "   :" + val+"  :"+subobj['outfile'])
                var structstring = "struct";
                if('nostruct' in subobj)
                	structstring = "";
        		var conststr = "const";
        		if('noconst' in subobj)
        			conststr = "";
        		if('constopt' in subobj)
        		{
        			var constval = GetHtmlObjVal( subobj['constopt'], "checkboxitem");
        			if(constval)
        				conststr = "";
        		}
        		structnamestr = conststr + " "+structstring+" "+ nodepath + " " +val+ " = { \n";
    		    structendstr = "};\n";
    		}
        }
        else if (('outfile' in obj)&& (base_key != 'sructname'))
        {
            var expconfigstr = "";
            var ignoreitem = false;
        	  if(("conditionpath" in obj) && ("conditionval" in obj))
            {
              var condObj = GetDescrObj(obj["conditionpath"]);
              var condrndid = 0;
              if(condObj)
                condrndid = condObj["rndid"];
              if(GetValFromPage(condrndid, obj["conditionpath"]) != obj["conditionval"])
                ignoreitem = true;
              var tsi_type = true;
              
              if(!GetModeFromWeb("modeid-1_form", "modeid-1rb"))
                tsi_type = false;
              if((tsi_type == false) || ignoreitem)
                ignoreitem = true;
            }
            if("subtablelabel" == obj['outfile'])
            {
            	var not_tsi_type = true;
              if((obj['inclsubstruct'] == "nt_tsi_recalib_config") || (obj['inclsubstruct'] == "nt_module_safety_gpio_params"))
              {
                var base_path = GetDescrObj("nt_module");
                if(GetValFromPage(base_path['rndid'], "nt_module.interface") != "&nt_module_tsi_interface")
                  not_tsi_type = false;
              }
              if(obj['inclsubstruct']&&not_tsi_type&&(!ignoreitem))
            	{
            	    var path = obj['inclsubstruct']+ ".sructname.name";//used by hw config
            	    var name =  GetDescrObj(path);
            	    if(!(name))
            	    {
                		var sructnameDescrPathObj = GetDescrObj(obj['inclsubstruct']+".sructname");
                		name = GetHtmlObjVal(obj['inclsubstruct']+".sructname-"+sructnameDescrPathObj['rndid'], sructnameDescrPathObj["outfile"]);            	    	
            	    }
        	    	finalstr = finalstr + "  ." + base_key + " = (void*)&" + name + ",\n";
            	}
            	continue;
            }
            else if(obj['outfile'] == 'hiddenarray')
            {
            	var base_path = obj['structpath'];
            	var baseDescrPathObj = GetDescrObj(base_path);
            	var asignedDescrPathObj = GetDescrObj(obj['assigned']);
            	
            	if(!('links' in asignedDescrPathObj))
            	{
            		asignedDescrPathObj['links'] = [];
            		var arr = {};
            		arr[asignedDescrPathObj['rndid']] = asignedDescrPathObj['sructname']['nodepath'];
            		asignedDescrPathObj['links'].push(arr);
            	}
            	for(linkitem in asignedDescrPathObj['links'])
            	{
            		for(baseid in asignedDescrPathObj['links'][linkitem])
            		{
                    	var assignedstructname = GetHtmlObjVal( asignedDescrPathObj['sructname']['nodepath'] + "-"+ baseid, asignedDescrPathObj['sructname']['template']);
                		str = str +  "const struct "+ obj['structtype'] + " * const " +assignedstructname+"_"+obj['structname'] + "[] = { \n";
                		var separator = ""

                    	var insert = true;
                		for(key in baseDescrPathObj['links'])
                    	{
                    		for(id in baseDescrPathObj['links'][key])
                    		{
                    			var structname = GetHtmlObjVal( baseDescrPathObj['sructname']['nodepath'] + "-"+ id, baseDescrPathObj['sructname']['template']);
                    			if('electrodes' in asignedDescrPathObj)
                    			{
                    				var val = GetHtmlObjVal(baseid + "_" + id, asignedDescrPathObj['electrodes']['activetmpl'])
                    				if(val == 1)
                    					insert = true;
                    				else
                    					insert = false;
                    			}
                    			if(insert)
                    			{
                            		str = str + separator +  "  &"+structname;
                            		separator = ",";
                    			}
                    		}
                    	}
                		str = str +  ", NULL\n};\n";   

            		}
            	}
            }
            else
            {
            	if(!ignoreitem)
              {
                var val = GetHtmlObjVal(nodepath, obj['outfile']);
            	  expconfigstr = "  ." + base_key + " = " + val + ",\n";
              
              }
            }
            //tableenumsubstr also generate item in struct to pass down config structure
            if("tableenumsubstr" == obj['outfile'])
            {
                var base_path = ReducePath(nodepath, 1);
                var baseDescrPathObj = GetDescrObj(base_path);
                var sructnameDescrPathObj = GetDescrObj(base_path+".sructname");
                var elname = "";
                var val = "";
                if("share_keydet_cfg" in baseDescrPathObj)
                {
                	var sharedkeydetcfg = GetHtmlObjVal(base_path+".share_keydet_cfg-"+GetIdFromPathId(nodepath), baseDescrPathObj["share_keydet_cfg"]["template"]);
                	if(sharedkeydetcfg)
                    sharedkeydetcfg = sharedkeydetcfg.replace("&", "");
                  else
                	   sharedkeydetcfg = "None";
                  
                	if(sharedkeydetcfg == "None")
                	{
                		elname = GetHtmlObjVal(base_path+".sructname-"+GetIdFromPathId(nodepath), sructnameDescrPathObj["outfile"]);
                		val = GetHtmlObjVal(nodepath, obj['outfile']);
                	}
                	else
                	{
                		elname = sharedkeydetcfg;
                		//list all electrodes to match the electrode name, to find type of the electrode in variable "val"
                		if('links' in baseDescrPathObj)
                		{
                			for(elindex in baseDescrPathObj['links'])
                			{
                				var elid = GetLinkId(baseDescrPathObj['links'][elindex])
                				var elnamecurr = GetHtmlObjVal(base_path+".sructname-"+elid, sructnameDescrPathObj["outfile"]);
                				if(elnamecurr == elname)
                				{
                					val = GetHtmlObjVal(GetPathFromPathId(nodepath)+"-"+elid, obj['outfile']);
                	            	var valkeydet = GetHtmlObjVal(GetPathFromPathId(nodepath)+"-"+elid, obj['outfile']);
                	            	expconfigstr = "  ." + base_key + " = " + valkeydet + ",\n";
                					break;
                				}
                			}
                		}
                	}
                }
                else
                {
                	val = GetHtmlObjVal(nodepath, obj['outfile']);
                	elname = GetHtmlObjVal(base_path+".sructname-"+GetIdFromPathId(nodepath), sructnameDescrPathObj["outfile"]);
                }
                //get index of enum
                var index = -1;
                for(objitem in obj["default"])
                {
                    if(obj["default"][objitem]['item'] == val)
                    {
                        index = objitem;
                        break;
                    }
                }
                if(index>=0)
                {
                    var itemname = obj["default"][index]["inclsubstructitem"];

                	if("inclsubstructenum" in obj["default"][index])
                    {
                    	var structpath = obj["default"][index]["inclsubstructenum"];
                        str = str  + "  ." + itemname + " = &"+structpath+"_" + elname + ",\n";                    	
                    }
                	else
                	{
                        str = str  + "  ." + itemname + " = NULL,\n";                    	
                	}
                    
                    //var val = GetHtmlObjVal(nodepath, subobj['outfile']);
                    //todo link the structure
                }
            }
            if(expconfigstr)
            	str = str+expconfigstr;
        }
    	
        if("inclsubstruct" in obj)
        {
            console.log(obj["inclsubstruct"])
            var inclSubStrDescrPathObj = GetDescrObj(obj["inclsubstruct"]);
            //todo get the rnd id from data or from web.
            if(!("links" in inclSubStrDescrPathObj))
            	inclSubStrDescrPathObj["links"] = [];
            var linksobj = inclSubStrDescrPathObj["links"];
//        alert(linksobj)
            var listitems = linksobj.childNodes
        //alert(listitems[0])
        //alert(inclSubStrDescrPathObj["links"])
            str = str + GenFileContent2(inclSubStrDescrPathObj, nodepath+"."+obj["inclsubstruct"], "", inclSubStrDescrPathObj['rndid']);
        }

        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if('outfile' in subobj)
                str = str + GenFileContent2(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
        }
        finalstr = finalstr + structnamestr + str + structendstr;
    }
    return finalstr;
}

function GenHFileContent(obj, nodepath, base_key, rndid)
{
	//list all items, when linked folow the linked
	var structnamestr = "";
	var structendstr = "";
	var str = "";
	var finalstr = "";
    if(typeof(obj) != 'object')
        return "";
	if(obj['outhfile'])
        console.log("outhfile:" + obj['outhfile']);
    if(!rndid)
    	rndid = 1;

	//todo get nodepath and rndid from linked list
    var linksobj = obj["links"];
    if((!('links' in obj)) && (rndid))
    {
    	linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
    }
    for( link in linksobj)
    {
    	str = "";
    	rndid = GetLinkId(linksobj[link]);
        if('sructname' in obj)
        {
        	var subobj = obj['sructname'];
        	if('outhfile' in subobj)
        	{
        		var val = GetHtmlObjVal(nodepath+".sructname-"+rndid, subobj['outhfile']);
        	    //alert(nodepath+".sructname-"+rndid + "   :" + val+"  :"+subobj['outfile'])
        		var conststr = "const";
        		if('noconst' in subobj)
        			conststr = "";
        		if('constopt' in subobj)
        		{
        			var constval = GetHtmlObjVal( subobj['constopt'], "checkboxitem");
        			if(constval)
        				conststr = "";
        		}
    		    structnamestr = "extern "+conststr+" struct "+ nodepath + " " +val+ ";\n";
    		}
        }

        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if('outhfile' in subobj)
                str = str + GenFileContent2(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
        }
        finalstr = finalstr + structnamestr;
    }
    return finalstr;
}

function GenEnFunctContent(obj, nodepath, base_key, rndid)
{
	//list all items, when linked folow the linked
	var structnamestr = "";
	var structendstr = "";
	var str = "";
	var finalstr = "";
    if(typeof(obj) != 'object')
        return "";
	if(obj['outenfn'])
        console.log("outenfn:" + obj['outenfn']);
    if(!rndid)
    	rndid = 1;

	//todo get nodepath and rndid from linked list
	var linksobj = obj["links"];
    if((!('links' in obj)) && (rndid))
    {
    	linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
    }
    for( link in linksobj)
    {
    	str = "";
    	rndid = GetLinkId(linksobj[link]);

        if('sructname' in obj)
        {
        	var subobj = obj['sructname'];
        	if('outenfn' in subobj)
        	{
        		var val = GetHtmlObjVal(nodepath+".sructname-"+rndid, subobj['outenfn']);
    		    if(nodepath == "nt_electrode")
    		    	structnamestr = "  " + nodepath + "_enable(&" +val+ ", 0);\n";
    		    else
    		    	structnamestr = "  " + nodepath + "_enable(&" +val+ ");\n";
    		}
        }

        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if('outenfn' in subobj)
                str = str + GenFileContent2(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
        }
        finalstr = finalstr + structnamestr;
    }
    return finalstr;
}

function GenTemplateFunctContent(obj, nodepath, base_key, rndid)
{
    //list all items, when linked folow the linked
    var structnamestr = "";
    var structendstr = "";
    var str = "";
    var finalstr = "";
    if(typeof(obj) != 'object')
        return "";
    if(obj['outenfn'])
        console.log("outenfn:" + obj['outenfn']);
    if(!rndid)
        rndid = 1;

    //todo get nodepath and rndid from linked list
    var linksobj = obj["links"];
    if((!('links' in obj)) && (rndid))
    {
    	linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
    }
    for( link in linksobj)
    {
        str = "";
        rndid = GetLinkId(linksobj[link]);

        if('sructname' in obj)
        {
          var subobj = obj['sructname'];
          if('templatefn' in subobj)
          {
            var name = GetHtmlObjVal(nodepath+".sructname-"+rndid, subobj['templatefn']);
            if('interface' in obj)
            {
              var interfaceobj = obj['interface'];
              if('templatefn' in interfaceobj)
              {
                var index = GetHtmlObjNum(nodepath+".interface-"+rndid, interfaceobj['templatefn']);
                var cfgobj={};
                if(name)
                  cfgobj['name'] = name;
                else
                  cfgobj['name'] = "NA";
                if(index)
                  structnamestr = "\n" + Mustache.to_html(callbacktemplates[index], cfgobj)
              }
              
            }
          }
        }

        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if('templatefn' in subobj)
                str = str + GenFileContent2(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
        }
        finalstr = finalstr + structnamestr;
    }
    return finalstr;
}


function PutPinDefinitionsToElectrode(path, elnames)
{
	var pinObj = GetDescrObj(path);
	if(!('template' in pinObj))
		return;
	if(pinObj['template'] != "tablepininput")
		return;
	pinObj['default'] = [];
	for(elname in elnames)
	{
		var enumitem = {};
		enumitem['item'] = elnames[elname];
		enumitem['sublabel'] = elnames[elname];
		pinObj['default'].push(enumitem);
	}
}

function SyncNodeContentToWeb(path)
{
	var dataObj = GetDescrObj(path);
	var base_path = ReducePath(path, 1);
	
	var dataBaseObj = GetDescrObj(base_path);
	
	
	if(!('links' in dataBaseObj))
		return;
	
	for(elidindex in dataBaseObj['links'])
	{
		for( rndid in dataBaseObj['links'][elidindex])
		{
			var str = GetWebContentFromObj2(dataObj, path, "templateitem", rndid);
			//!!not generic implementation, depends on template, expects that template generates 2 columns and second column contains web content to replace
			//solution is to passdown IDs to rows of table and replace whole row.(should be simple, needs to add ID generatino to all templates and ID should end  by "row")
			//var tablecolumns = str.split("<td>");
			//var cellcontent = tablecolumns[1].split("</td>");
			//console.log(cellcontent[0]);
			var obj = document.getElementById(path+"-"+rndid);
			var lastopt = false
			if(obj)
				lastopt = obj.disabled

			UpdateContentHtmlObjVal(path+"-"+rndid, dataObj["template"], str);
			obj = document.getElementById(path+"-"+rndid);
			if(obj)
				obj.disabled = lastopt;
		}
	}
}


function ReadDatafromBoardToWeb(obj, nodepath, base_key, rndid)
{
	//list all items, when linked folow the linked
	var structnamestr = "";
	var structendstr = "";
	var str = "";
	var finalstr = "";
    if(typeof(obj) != 'object')
        return "";
	if(obj['boardrw'])
        console.log("descr:" + obj['boardrw']);
    if(!rndid)
    	rndid = 1;
    //todo get nodepath and rndid from linked list
	var linksobj = obj["links"];
    if((!('links' in obj)) && (rndid))
    {
    	linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
    }
    for( link in linksobj)
    {
    	str = "";
    	rndid = GetLinkId(linksobj[link]);
    	if ('boardrw' in obj)
    	{
    		
            //tableenumsubstr also generate item in struct to pass down config structure
            if("tableenumsubstr" == obj['boardrw'])
            {
            	var val = GetHtmlObjVal(nodepath, obj['boardrw']);
                //get index of enum
                var index = -1;
                for(objitem in obj["default"])
                {
                    if(obj["default"][objitem]['item'] == val)
                    {
                        index = objitem;
                        break;
                    }
                }
                if(index>=0)
                {
                    var itemname = obj["default"][index]["inclsubstructitem"];

                	if("inclsubstructenum" in obj["default"][index])
                    {
                    	var incstructobj = GetDescrObj(obj["default"][index]['inclsubstructenum']);
                        for( key in incstructobj)
                        {
                            console.log("key:"+key);
                            if(typeof(incstructobj) != 'object')
                            	continue;
                            var incsubobj = incstructobj[key];
                            if(typeof(incsubobj) != 'object')
                                continue;

                            if('boardrw' in incsubobj)
                            	ReadDatafromBoardToWeb(incstructobj[key], obj["default"][index]['inclsubstructenum']+"."+key+"-"+GetIdFromPathId(nodepath), key, GetIdFromPathId(nodepath));
                        }
                    }
                }
            }
            else if("subtablelabel" == obj['boardrw'])
            {
            	var incstructobj = GetDescrObj(obj['inclsubstruct']);
            	ReadDatafromBoardToWeb(incstructobj, obj['inclsubstruct'], obj['inclsubstruct'], incstructobj['rndid']);
            }
            else
            {
            	OnClickRead(null, nodepath);
            }

    	}
        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if('boardrw' in subobj)
            	ReadDatafromBoardToWeb(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
        }
    }
    return finalstr;
}

function GetMacroConfigInit(obj, nodepath, base_key, rndid, outobj)
{
	str = "";
	outobj["out"] = {};
	GetMacroConfig(obj, nodepath, base_key, rndid);
	for(index in outobj["out"])
		str = str + "#define "+ index + "  " + outobj["out"][index]+"\n";
	return str;
}

function GetMacroConfig(obj, nodepath, base_key, rndid)
{
	var outstr = "";
    if(typeof(obj) != 'object')
        return "";
	if(obj['macro'])
        console.log("descr:" + obj['macro']);
    //todo get nodepath and rndid from linked list
    if(!rndid)
    	rndid = 1;
    var linksobj = obj["links"];
    if((!('links' in obj)) && (rndid))
    {
    	linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
    }
	if('macro' in obj)
	{
        if(obj['macro'] == "tableenumsubstr")
        {
        	var val = GetHtmlObjVal(nodepath, obj['macro']);
            //get index of enum
            var index = -1;
            for(objitem in obj["default"])
            {
                if(obj["default"][objitem]['item'] == val)
                {
                    index = objitem;
                    break;
                }
            }
            if(index >= 0)
            {
            	if((obj["default"][index]['sublabel'] == "Proxi") || (obj["default"][index]['sublabel'] == "Matrix"))
            	{
            		var subbase = obj["default"][index]['inclsubstructenum'];
            		var subbaseObj = GetDescrObj(subbase);
            		outstr = outstr + GetMacroConfig(subbaseObj, subbase, subbase, rndid);
            	}
            }
            	
        }
        else
        {
        	var type = "";
        	var val = 0;
        	var nodepathtmp = GetPathFromPathId(nodepath);
        	var rndidtmp = GetIdFromPathId(nodepath);
        	if(obj['macroname'] == "NT_PROXI_ELECTRODES")
        	{
            	var controlbasepath = ReducePath(nodepathtmp, 1);
            	var interfaceObj = GetDescrObj(controlbasepath+".interface");
            	var vala = GetHtmlObjVal(controlbasepath+".interface-"+rndidtmp, interfaceObj['macro']);
                //get index of enum
                for(objitem in interfaceObj["default"])
                {
                    if(interfaceObj["default"][objitem]['item'] == vala)
                    {
                        type = interfaceObj["default"][objitem]['sublabel'];
                        break;
                    }
                }    		
        	}

            if((obj['macroname'] == "NT_PROXI_ELECTRODES") && (type == "Proxi"))
        	{
        		var baseElPathObj = GetDescrObj(obj['datasrc']);
            	if(baseElPathObj)
            	{
            	    var elLinksobj = baseElPathObj["links"];
            	    for( link in elLinksobj)
            	    {
            	    	var elrndid = GetLinkId(elLinksobj[link]);
            	    	var elidbaseid = GetIdFromPathId(nodepath);
            	    	if(GetHtmlObjVal( elidbaseid+"_"+elrndid , obj['macro'])&1)
            	    		val = val+Number(1);
            	    }
            	}
        		var outobj = GetDescrObj(obj['macroout']);
        		if(!('out' in outobj))
        			outobj['out'] = {};

        		if((outobj['out'][obj['macroname']]<len) || (!(obj['macroname'] in outobj['out'])))
        			outobj['out'][obj['macroname']] = val;
          	
//            	outstr = outstr + "#define " + obj['macroname'] + " "+ val +" \n";
        	}
        	else if(obj['macroname'] == "NT_PROXI_GESTURES")
        	{
            	val = GetHtmlObjVal(nodepath, obj['macro']);
            	arr = val.split(",");
            	var len = arr.length-1;
        		var outobj = GetDescrObj(obj['macroout']);
        		if(!('out' in outobj))
        			outobj['out'] = {};
        		if((outobj['out'][obj['macroname']]<len) || (!(obj['macroname'] in outobj['out'])))
        			outobj['out'][obj['macroname']] = len;
//            	outstr = outstr + "#define " + obj['macroname'] + " "+ len +" \n";
        	}
        	else if(obj['macroname'] == "NT_MATRIX_UNLOCK_GESTURES")
        	{
            	val = GetHtmlObjVal(nodepath, obj['macro']);
            	arr = val.split(",");
            	var len = (arr.length/4)-1;
        		var outobj = GetDescrObj(obj['macroout']);
        		if(!('out' in outobj))
        			outobj['out'] = {};
        		if((outobj['out'][obj['macroname']]<len) || (!(obj['macroname'] in outobj['out'])))
        			outobj['out'][obj['macroname']] = len;
          	//outstr = outstr + "#define " + obj['macroname'] + " "+ len +" \n";
        	}
        }
	}

    var linksobj = obj["links"];
    for( link in linksobj)
    {
    	str = "";
    	rndid = GetLinkId(linksobj[link]);
        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if('macro' in subobj)
            {
                outstr = outstr + GetMacroConfig(obj[key], nodepath+"."+key+"-"+rndid, key, rndid);
                
            }

        }
    }
	
	return outstr;
}

function SyncOnChange(dataobj, path)
{
	var basepath = ReducePath(path, 1);
	var basedataobj = GetDescrObj(basepath);
	dataobj = GetDescrObj(path);
	if(!('links' in basedataobj))
		return;
	if(!('cfgfile' in dataobj))
		return;

	
    var ellinksobj = basedataobj["links"];
    for( link in ellinksobj)
    {
    	rndid = GetLinkId(ellinksobj[link]);
    	if(('onfocuscbck' in dataobj)&&(dataobj['cfgfile'] == "tableenumsubstrshareditem"))
    	{
            var finalArgs = [];
            finalArgs.push(null);
            finalArgs.push(path+"-"+rndid);
            var val = CallFunctStr(dataobj["onfocuscbck"], finalArgs);
    	}
    	if(('onchangecbck' in dataobj)&&(dataobj['cfgfile'] == "tableenumsubstrshareditem"))
    	{
            var finalArgs = [];
            finalArgs.push(null);
            finalArgs.push(path+"-"+rndid);
            var val = CallFunctStr(dataobj["onchangecbck"], finalArgs);
    	}
    }
}


//function ReadDatafromBoardToWeb(obj, nodepath, base_key, rndid, mode)
function SetModeToWeb(obj, nodepath, base_key, rndid, mode, modekey)
{
	//list all items, when linked folow the linked
	var structnamestr = "";
	var structendstr = "";
	var str = "";
	var finalstr = "";
	if(typeof(obj) != 'object')
		return "";
	if(obj['boardrw'])
		console.log("descr:" + obj['boardrw']);
	if(!rndid)
		rndid = 1;
    //todo get nodepath and rndid from linked list
	var linksobj = obj["links"];
	if((!('links' in obj)) && (rndid))
	{
    	if((nodepath == "nt_electrode")||(nodepath == "nt_control")) // do nothing, when no electrode/controls added
        return;
      linksobj = InitLinks(rndid);
    	obj["links"] = linksobj;
	}
    for( link in linksobj)
    {
    	str = "";
    	rndid = GetLinkId(linksobj[link]);
    	if (modekey in obj)
    	{
    		
            //tableenumsubstr also generate item in struct to pass down config structure
            if(("tableenumsubstr" == obj['boardrw']) || ("tableenumhwinterface" == obj['boardrw']))
            {
            	var val = GetHtmlObjVal(nodepath, obj['boardrw']);
                //get index of enum
                var index = -1;
                for(objitem in obj["default"])
                {
                    if(obj["default"][objitem]['item'] == val)
                    {
                        index = objitem;
                        break;
                    }
                }
                if(index>=0)
                {
                    var itemname = obj["default"][index]["inclsubstructitem"];

                    if("inclsubstructenum" in obj["default"][index])
                    {
                    	var incstructobj = GetDescrObj(obj["default"][index]['inclsubstructenum']);
                        for( key in incstructobj)
                        {
                            console.log("key:"+key);
                            if(typeof(incstructobj) != 'object')
                            	continue;
                            var incsubobj = incstructobj[key];
                            if(typeof(incsubobj) != 'object')
                                continue;

                            if(modekey in incsubobj)
                            	SetModeToWeb(incstructobj[key], obj["default"][index]['inclsubstructenum']+"."+key+"-"+GetIdFromPathId(nodepath), key, GetIdFromPathId(nodepath), mode, modekey);
                        }
                    }
                }
            }
            else
            {
              var trStr = 'tr';
              if("subtablelabel" == obj['boardrw'])
            	{
                var incstructobj = GetDescrObj(obj['inclsubstruct']);
            	  SetModeToWeb(incstructobj, obj['inclsubstruct'], obj['inclsubstruct'], incstructobj['rndid'], mode, modekey);
                trStr = "";
              }
            	if(obj[modekey] == 'x')
              {
                var tr_obj = document.getElementById(nodepath+trStr);
                
                if(tr_obj)
                {
                  if(Number(mode)) // not zero is expert mode
                  {
                    tr_obj.style.display ='';
                  }
                  else
                  {
                    tr_obj.style.display ='None';
                    //reset to default
                    SetHtmlObjDefVal(nodepath, obj['boardrw']);
                  }
                } 
              }
            }

    	}
        for( key in obj)
        {
            console.log("key:"+key);
            if(typeof(obj) != 'object')
            	continue;
            var subobj = obj[key];
            if(typeof(subobj) != 'object')
                continue;

            if(modekey in subobj)
            	SetModeToWeb(obj[key], nodepath+"."+key+"-"+rndid, key, rndid, mode, modekey);
        }
    }
    return finalstr;
}

function GetValFromPage(rndid, path)
{
  var dataobj = GetDescrObj(path);
  if(dataobj)
    return GetHtmlObjVal( path+"-"+rndid, dataobj["template"])
  return 0;
}
