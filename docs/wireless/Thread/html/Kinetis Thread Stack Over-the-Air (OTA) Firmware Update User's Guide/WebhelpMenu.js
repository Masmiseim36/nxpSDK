/* =============================================================================
	 Trisoft CMS Web Help Script v1.0
	 
	 Copyright (c) 2008-2009 SDL Trisoft (http://www.trisoftcms.com)
	
	 Tested on windows with        IE6, IE7, FF2, FF3, Opera 9.62
	 Tested on Linux (ubuntu) with FF3
	 
	 You may only use this script library on web help content 
	 generated using Trisoft CMS
   ========================================================================== */

function ChangeMenu(tabname)
{
	var locString = '';
/*	alert(tabname);*/
	
	switch (tabname.toLowerCase()) {
	   case 'content': 
	   	locString = 'toc.html';
	   	break;
	   case 'index': 
	   	locString = 'indexpage.html';
	   	break;
	   case 'search': 
	   	locString = 'search.html';
	   	break;
	   default: 
	   	locString = '';
	 }
/*	 alert(locString); */
   var win = open(locString,'navigationwin');	 
/*   if (!parent.showNavWin) parent.ToggleNavWin(); */
   
   document.getElementById('content').className= '';
   document.getElementById('index').className= '';
   document.getElementById('search').className= '';
   document.getElementById(tabname.toLowerCase()).className= 'current';
/* change end li elements className value */
   document.getElementById('content-end').className= 'end';
   document.getElementById('index-end').className= 'end';
   document.getElementById('search-end').className= 'end';
   document.getElementById(tabname.toLowerCase()+'-end').className= 'endcurrent';
   
}

function showPrevTopic()
{
	var currentlocation = window.parent.contentwin.location.toString();
	var strippedCurrentTopicName = currentlocation.substring(currentlocation.lastIndexOf("/")+1,currentlocation.length);

	var CurrentPos = locateCurrentTopic(strippedCurrentTopicName);
	if (CurrentPos > -1)
	{
	  if (CurrentPos>0)
		{
		  NewPos = CurrentPos-1;
	    var locString = '';	
		  locString = FileSequence[NewPos];
	    var win = open(locString,'contentwin');
	  }
	  else
		{
		  alert('This is the first topic of your content.');
    }

   }

}

function showNextTopic() 
{
	var currentlocation = window.parent.contentwin.location.toString();
	var strippedCurrentTopicName = currentlocation.substring(currentlocation.lastIndexOf("/")+1,currentlocation.length);

	var CurrentPos = locateCurrentTopic(strippedCurrentTopicName);
	
	if (CurrentPos > -1)
	{
	  if ((FileSequence.length-1)>CurrentPos)
		{
		  NewPos = CurrentPos+1;
	    var locString = '';	
		  locString = FileSequence[NewPos];
	    var win = open(locString,'contentwin');		  
	  }
	  else
		{
			alert('This is the last topic of your content.');
    }
   }
}

function locateCurrentTopic(curTopic)
{
	var locationFound = -1;
	for (var arrayCounter=0; arrayCounter<FileSequence.length; arrayCounter++)
	{
		if (FileSequence[arrayCounter] == curTopic)
		{
			locationFound = arrayCounter;
			break;
		}
	}
	return locationFound;
}

function printTopic()
{
	if (window.print) 
	{
		window.parent.contentwin.focus();
		window.parent.contentwin.print();
	}
}

function firstTopic()
{
	locString = FileSequence[0];

/*	parent.LoadFrame(); 

	if (parent.contextSensitiveURL != "" && parent.contextSensitiveURL != undefined) {locString = parent.contextSensitiveURL;} */
	
	var win = open(locString,'contentwin');		
}