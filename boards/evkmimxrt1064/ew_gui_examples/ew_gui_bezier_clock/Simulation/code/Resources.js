/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.20
* Profile  : WebGL
* Platform : Tara.WebGL.RGBA8888
*
*******************************************************************************/

var EmWiApp;if(!EmWiApp)throw new Error("The application file '_project.js' isn't yet loaded!"
);if(EmWiApp.te)throw new Error("The unit file 'Resources.js' included twice!");
EmWiApp.te=(function(){var B=EmWiApp;var E={};
var AJ=[0,0];var Be="The property \'FrameSize\' is READ ONLY.";
E.An={bitmap:null,FrameDelay:0,NoOfFrames:1,FrameSize:B.pm,E8:false,Ia:false,DX:function(
){if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.bitmap=null;this.
FrameSize=AJ;this.FrameDelay=0;this.NoOfFrames=1;this.E8=false;},CU:function(aArg
){if(!aArg)return;var handle=null;var noOfFrames=1;var frameSize=AJ;var frameDelay=
0;{var bmp=B.sg(aArg,this);if(bmp){noOfFrames=bmp.NoOfFrames;frameSize=bmp.FrameSize;
frameDelay=bmp.FrameDelay;}handle=bmp;}this.bitmap=handle;this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.Ia=true;this.E8=(!!this.
bitmap&&(this.FrameDelay>0))&&(this.NoOfFrames>1);},K_:function(C){throw new Error(
Be);},Update:function(){},_Init:function(aArg){B.Core.Il._Init.call(this,aArg);this.
__proto__=E.An;this.CU(aArg);},_Done:function(){this.__proto__=E.An;this.DX();B.
Core.Il._Done.call(this);},_className:"Resources::Bitmap"};E.BR={font:null,Leading:
0,Descent:0,Ascent:0,DX:function(){if(!this.font)return;var handle=this.font;B.rL(
handle);this.font=null;this.Ascent=0;this.Descent=0;this.Leading=0;},CU:function(
aArg){if(!aArg)return;var handle=null;var ascent=0;var descent=0;var leading=0;{
var font=B.si(aArg);if(font){ascent=font.Ascent;descent=font.Descent;leading=font.
Leading;}handle=font;}this.font=handle;this.Ascent=ascent;this.Descent=descent;this.
Leading=leading;},P8:function(aFlowString){if(!this.font)return 0;var handle=this.
font;var advance=0;advance=B.rR(handle,aFlowString);return advance;},TI:function(
aString,aOffset,aWidth,aMaxNoOfRows,aBidi){if(aOffset<0)aOffset=0;if(!this.font||((
aOffset>0)&&(aOffset>=aString.length)))return B.g3;var handle=this.font;var result=
B.g3;result=B.sD(handle,aString,aOffset,aWidth,aMaxNoOfRows,aBidi);return result;
},KY:function(aString,aOffset,aCount){if(aOffset<0)aOffset=0;if(!this.font||((aOffset>
0)&&(aOffset>=aString.length)))return 0;var handle=this.font;var advance=0;advance=
B.i7(handle,aString,aOffset,aCount);return advance;},OnGetLeading:function(){return this.
Leading;},OnGetDescent:function(){return this.Descent;},OnGetAscent:function(){return this.
Ascent;},_Init:function(aArg){B.Core.Il._Init.call(this,aArg);this.__proto__=E.BR;
this.CU(aArg);},_Done:function(){this.__proto__=E.BR;this.DX();B.Core.Il._Done.call(
this);},_className:"Resources::Font"};E.In={_class:function(){return E.An;},0:{FileName:
"./res/ResourcesWhiteBitmap.png",Format:B.kC,NoOfFrames:1,FrameSize:[254,254],FrameDelay:
0,_this:null}};E.MK={_class:function(){return E.An;},0:{FileName:"./res/ResourcesWhiteBitmapStripe.png"
,Format:B.kC,NoOfFrames:1,FrameSize:[256,4],FrameDelay:0,_this:null}};E.Ib={_class:
function(){return E.An;},0:{FileName:"./res/ResourcesNavigationIconsMedium.png",
Format:B.kC,NoOfFrames:20,FrameSize:[36,36],FrameDelay:0,_this:null}};E.S2="Monday";
E.TZ="Tuesday";E.T1="Wednesday";E.TW="Thursday";E.Su="Friday";E.TN="Saturday";E.
TT="Sunday";E.SO="January";E.St="February";E.SZ="March";E.R1="April";E.S0="May";
E.SQ="June";E.SP="July";E.R2="August";E.TR="September";E.S6="October";E.S5="November";
E.R7="December";E.S3="Mon";E.T0="Tue";E.T2="Wed";E.TX="Thu";E.Sv="Fri";E.TO="Sat";
E.TU="Sun";E.Qb="Jan";E.P6="Feb";E.Qg="Mar";E.PX="Apr";E.Qi="May";E.Qd="Jun";E.Qc=
"Jul";E.PY="Aug";E.QP="Sep";E.Qm="Oct";E.Ql="Nov";E.P1="Dec";
E._Init=function(){E.An.__proto__=B.Core.Il;E.BR.__proto__=B.Core.Il;};E.Dy=function(
D){var A;if((A=E.In[0]._this)&&(A._cycle!=D))A._Done(E.In[0]._this=null);if((A=E.
MK[0]._this)&&(A._cycle!=D))A._Done(E.MK[0]._this=null);if((A=E.Ib[0]._this)&&(A.
_cycle!=D))A._Done(E.Ib[0]._this=null);};return E;})();

/* Embedded Wizard */