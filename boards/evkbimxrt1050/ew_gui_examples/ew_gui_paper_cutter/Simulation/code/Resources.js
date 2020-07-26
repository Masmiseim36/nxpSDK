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
var AM=[0,0];var BD="The property \'FrameSize\' is READ ONLY.";
C.An={bitmap:null,FrameDelay:0,NoOfFrames:1,FrameSize:B.qx,EM:false,HJ:false,D3:function(
){if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.bitmap=null;this.
FrameSize=AM;this.FrameDelay=0;this.NoOfFrames=1;this.EM=false;},Bc:function(aArg
){if(!aArg)return;var handle=null;var noOfFrames=1;var frameSize=AM;var frameDelay=
0;{var bmp=B.tp(aArg,this);if(bmp){noOfFrames=bmp.NoOfFrames;frameSize=bmp.FrameSize;
frameDelay=bmp.FrameDelay;}handle=bmp;}this.bitmap=handle;this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.HJ=true;this.EM=(!!this.
bitmap&&(this.FrameDelay>0))&&(this.NoOfFrames>1);},IZ:function(E){throw new Error(
BD);},Update:function(){},_Init:function(aArg){B.Core.EY._Init.call(this,aArg);this.
__proto__=C.An;this.Bc(aArg);},_Done:function(){this.D3();this.__proto__=B.Core.
EY;B.Core.EY._Done.call(this);},_className:"Resources::Bitmap"};C.A2={font:null,
Leading:0,Descent:0,Ascent:0,D3:function(){if(!this.font)return;var handle=this.
font;B.sX(handle);this.font=null;this.Ascent=0;this.Descent=0;this.Leading=0;},Bc:
function(aArg){if(!aArg)return;var handle=null;var ascent=0;var descent=0;var leading=
0;{var font=B.tr(aArg);if(font){ascent=font.Ascent;descent=font.Descent;leading=
font.Leading;}handle=font;}this.font=handle;this.Ascent=ascent;this.Descent=descent;
this.Leading=leading;},JX:function(aFlowString){if(!this.font)return 0;var handle=
this.font;var advance=0;advance=B.s2(handle,aFlowString);return advance;},LI:function(
aString,aOffset,aWidth,aMaxNoOfRows,aBidi){if(aOffset<0)aOffset=0;if(!this.font||((
aOffset>0)&&(aOffset>=aString.length)))return B.hm;var handle=this.font;var result=
B.hm;result=B.tM(handle,aString,aOffset,aWidth,aMaxNoOfRows,aBidi);return result;
},GK:function(aString,aOffset,aCount){if(aOffset<0)aOffset=0;if(!this.font||((aOffset>
0)&&(aOffset>=aString.length)))return 0;var handle=this.font;var advance=0;advance=
B.jJ(handle,aString,aOffset,aCount);return advance;},OnGetLeading:function(){return this.
Leading;},OnGetDescent:function(){return this.Descent;},OnGetAscent:function(){return this.
Ascent;},_Init:function(aArg){B.Core.EY._Init.call(this,aArg);this.__proto__=C.A2;
this.Bc(aArg);},_Done:function(){this.D3();this.__proto__=B.Core.EY;B.Core.EY._Done.
call(this);},_className:"Resources::Font"};
C._Init=function(){C.An.__proto__=B.Core.EY;C.A2.__proto__=B.Core.EY;};C.C2=function(
D){};return C;})();

/* Embedded Wizard */