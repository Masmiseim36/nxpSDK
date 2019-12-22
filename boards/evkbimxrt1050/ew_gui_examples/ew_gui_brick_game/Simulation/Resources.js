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
);if(EmWiApp.tf)throw new Error("The unit file 'Resources.js' included twice!");
EmWiApp.tf=(function(){var B=EmWiApp;var C={};
var At=[0,0];var AT="The property \'FrameSize\' is READ ONLY.";
C.Ae={bitmap:null,FrameDelay:0,NoOfFrames:1,FrameSize:B.pm,Dq:false,FT:false,CY:function(
){if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.bitmap=null;this.
FrameSize=At;this.FrameDelay=0;this.NoOfFrames=1;this.Dq=false;},Cc:function(aArg
){if(!aArg)return;var handle=null;var noOfFrames=1;var frameSize=At;var frameDelay=
0;{var bmp=B.sg(aArg,this);if(bmp){noOfFrames=bmp.NoOfFrames;frameSize=bmp.FrameSize;
frameDelay=bmp.FrameDelay;}handle=bmp;}this.bitmap=handle;this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.FT=true;this.Dq=(!!this.
bitmap&&(this.FrameDelay>0))&&(this.NoOfFrames>1);},GO:function(E){throw new Error(
AT);},Update:function(){},_Init:function(aArg){B.Core.DV._Init.call(this,aArg);this.
__proto__=C.Ae;this.Cc(aArg);},_Done:function(){this.__proto__=C.Ae;this.CY();B.
Core.DV._Done.call(this);},_className:"Resources::Bitmap"};C.AM={font:null,Leading:
0,Descent:0,Ascent:0,CY:function(){if(!this.font)return;var handle=this.font;B.rL(
handle);this.font=null;this.Ascent=0;this.Descent=0;this.Leading=0;},Cc:function(
aArg){if(!aArg)return;var handle=null;var ascent=0;var descent=0;var leading=0;{
var font=B.si(aArg);if(font){ascent=font.Ascent;descent=font.Descent;leading=font.
Leading;}handle=font;}this.font=handle;this.Ascent=ascent;this.Descent=descent;this.
Leading=leading;},HL:function(aFlowString){if(!this.font)return 0;var handle=this.
font;var advance=0;advance=B.rR(handle,aFlowString);return advance;},I3:function(
aString,aOffset,aWidth,aMaxNoOfRows,aBidi){if(aOffset<0)aOffset=0;if(!this.font||((
aOffset>0)&&(aOffset>=aString.length)))return B.g3;var handle=this.font;var result=
B.g3;result=B.sD(handle,aString,aOffset,aWidth,aMaxNoOfRows,aBidi);return result;
},E2:function(aString,aOffset,aCount){if(aOffset<0)aOffset=0;if(!this.font||((aOffset>
0)&&(aOffset>=aString.length)))return 0;var handle=this.font;var advance=0;advance=
B.i7(handle,aString,aOffset,aCount);return advance;},OnGetLeading:function(){return this.
Leading;},OnGetDescent:function(){return this.Descent;},OnGetAscent:function(){return this.
Ascent;},_Init:function(aArg){B.Core.DV._Init.call(this,aArg);this.__proto__=C.AM;
this.Cc(aArg);},_Done:function(){this.__proto__=C.AM;this.CY();B.Core.DV._Done.call(
this);},_className:"Resources::Font"};
C._Init=function(){C.Ae.__proto__=B.Core.DV;C.AM.__proto__=B.Core.DV;};C.BO=function(
D){};return C;})();

/* Embedded Wizard */