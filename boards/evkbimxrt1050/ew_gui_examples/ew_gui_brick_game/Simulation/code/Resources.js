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
* Version  : 9.30
* Profile  : WebGL
* Platform : Tara.WebGL.RGBA8888
*
*******************************************************************************/

var EmWiApp;if(!EmWiApp)throw new Error("The application file '_project.js' isn't yet loaded!"
);if(EmWiApp.uj)throw new Error("The unit file 'Resources.js' included twice!");
EmWiApp.uj=(function(){var B=EmWiApp;var C={};
var Ar=[0,0];var AT="The property \'FrameSize\' is READ ONLY.";
C.Ae={bitmap:null,FrameDelay:0,NoOfFrames:1,FrameSize:B.qx,Dw:false,Gb:false,C2:function(
){if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.bitmap=null;this.
FrameSize=Ar;this.FrameDelay=0;this.NoOfFrames=1;this.Dw=false;},Ce:function(aArg
){if(!aArg)return;var handle=null;var noOfFrames=1;var frameSize=Ar;var frameDelay=
0;{var bmp=B.tp(aArg,this);if(bmp){noOfFrames=bmp.NoOfFrames;frameSize=bmp.FrameSize;
frameDelay=bmp.FrameDelay;}handle=bmp;}this.bitmap=handle;this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.Gb=true;this.Dw=(!!this.
bitmap&&(this.FrameDelay>0))&&(this.NoOfFrames>1);},G_:function(E){throw new Error(
AT);},Update:function(){},_Init:function(aArg){B.Core.DH._Init.call(this,aArg);this.
__proto__=C.Ae;this.Ce(aArg);},_Done:function(){this.C2();this.__proto__=B.Core.
DH;B.Core.DH._Done.call(this);},_className:"Resources::Bitmap"};C.AN={font:null,
Leading:0,Descent:0,Ascent:0,C2:function(){if(!this.font)return;var handle=this.
font;B.sX(handle);this.font=null;this.Ascent=0;this.Descent=0;this.Leading=0;},Ce:
function(aArg){if(!aArg)return;var handle=null;var ascent=0;var descent=0;var leading=
0;{var font=B.tr(aArg);if(font){ascent=font.Ascent;descent=font.Descent;leading=
font.Leading;}handle=font;}this.font=handle;this.Ascent=ascent;this.Descent=descent;
this.Leading=leading;},H3:function(aFlowString){if(!this.font)return 0;var handle=
this.font;var advance=0;advance=B.s2(handle,aFlowString);return advance;},Jh:function(
aString,aOffset,aWidth,aMaxNoOfRows,aBidi){if(aOffset<0)aOffset=0;if(!this.font||((
aOffset>0)&&(aOffset>=aString.length)))return B.hm;var handle=this.font;var result=
B.hm;result=B.tM(handle,aString,aOffset,aWidth,aMaxNoOfRows,aBidi);return result;
},Fa:function(aString,aOffset,aCount){if(aOffset<0)aOffset=0;if(!this.font||((aOffset>
0)&&(aOffset>=aString.length)))return 0;var handle=this.font;var advance=0;advance=
B.jJ(handle,aString,aOffset,aCount);return advance;},OnGetLeading:function(){return this.
Leading;},OnGetDescent:function(){return this.Descent;},OnGetAscent:function(){return this.
Ascent;},_Init:function(aArg){B.Core.DH._Init.call(this,aArg);this.__proto__=C.AN;
this.Ce(aArg);},_Done:function(){this.C2();this.__proto__=B.Core.DH;B.Core.DH._Done.
call(this);},_className:"Resources::Font"};
C._Init=function(){C.Ae.__proto__=B.Core.DH;C.AN.__proto__=B.Core.DH;};C.BO=function(
D){};return C;})();

/* Embedded Wizard */