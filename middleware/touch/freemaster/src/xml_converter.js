/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

function XmlStrToDataObj(str)
{
	var xmlDoc;
	if (window.DOMParser)
	{
	    var parser = new DOMParser();
	    xmlDoc = parser.parseFromString(str, "text/xml");
	}
	else // Internet Explorer
	{
	    xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
	    xmlDoc.async = false;
	    xmlDoc.loadXML(str);
	}
    
    var x = xmlDoc.documentElement.childNodes;
    console.log("parsed");
    var out = GetDataFromNode(xmlDoc.documentElement, 0)
    //console.log(out);
    //PrintXmlDataNode(out)
	return out;
}

function GetDataFromNode(node,level) // argument level only for debug purpose
{
    /*    1  ELEMENT_NODE
    2   ATTRIBUTE_NODE
    3   TEXT_NODE
    4   CDATA_SECTION_NODE
    5   ENTITY_REFERENCE_NODE
    6   ENTITY_NODE
    7   PROCESSING_INSTRUCTION_NODE
    8   COMMENT_NODE
    9   DOCUMENT_NODE
    10  DOCUMENT_TYPE_NODE
    11  DOCUMENT_FRAGMENT_NODE
    12  NOTATION_NOD*/
	var data = {};
    var childs = node.childNodes;
    var debug = 0;

    if(debug)
        console.log("lvl:" + level +" nodename:"+node.nodeValue + "  " + childs.length);
    if(childs.length == 1) //TEXT_NODE
    {
        if ('textContent' in node)
            return node.textContent;
        else
            return recordNode.text;
    }

    if(node.nodeValue != null)
        return node.nodeValue;
    if(debug)
        console.log("lvl:" + level +" childscnt:"+childs.length);
    for(var i=0; i<childs.length; i++)
    {
    	if(childs[i].nodeType == 1) //ELEMENT_NODE
        {
    		if(debug)
    			  console.log("new");
    		if((typeof data[childs[i].nodeName]) === 'object')
    		{
    			if(!(data[childs[i].nodeName].constructor === Array))
    			{
                    var obj = data[childs[i].nodeName];
                    if(debug)
                        console.log(data[childs[i].nodeName]);
                    data[childs[i].nodeName] = new Array();
                    data[childs[i].nodeName].push(obj);
    			}
    			if(debug)
    			 console.log("push");
                data[childs[i].nodeName].push(GetDataFromNode(childs[i],level+Number(1)));
                if(debug)
                    console.log(data[childs[i].nodeName]);

    		}
    		else
    		{
    			data[childs[i].nodeName] = GetDataFromNode(childs[i],level+Number(1));
    		}
    		if(debug)
    			  console.log("lvl:" + level +" childname:"+name);
        }
    }
    if(debug)
        console.log(data);
	return data;
}

function PrintXmlDataNode(out)
{
	console.log("res:");
	for( key in out)
	{
		console.log("key:" + key);
		console.log(out[key]);
		out2 = out[key]
		if(typeof out2 === 'object')
		{
	        for( key2 in out2)
	        {
	            console.log("key2:" + key2);
	            console.log(out2[key2]);
	            out3 = out2[key2];
	            if(typeof out3 === 'object')
	            {
	                for( key3 in out3)
	                {
	                    console.log("key3:" + key3);
	                    console.log(out3[key3]);
	                    out4 = out3[key3];
	                    if(typeof out4 === 'object')
	                    {
	                        for( key4 in out4)
	                        {
	                            console.log("key4:" + key4);
	                            console.log(out4[key4]);
	                        }                   
	                    }
	                }               
	            }
	        }    		
		}
	}
	//console.log(x);
	console.log("x.nodeType:" + x.nodeType)
	console.log("x.length:" + x.length);
	console.log("resp2:");
	var data = {};
	for(i=0; i<x.length; i++)
	{
	    if(x[i].nodeType == 1) //ELEMENT_NODE
	    {
	        var name = x[i].nodeName;
	        var value = x[i].nodeValue;
	        if(value == null)
	        {
	            //call funct, which returns dict data array with subnodes 
	            value = GetDataFromNode(x[i]);
	        }
	        console.log(name);
	        data[name] = value
	        //console.log("x["+i+"].nodeName:" + x[i].nodeName);
	        //console.log("x["+i+"].nodeType:" + x[i].nodeType);
	        //console.log("x["+i+"].nodeValue:"+x[i].nodeValue);
//	        y = x[i].childNodes;
//	        console.log("y.length:" + y.length);
	    }

	}
	console.log(data);
}

