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
);if(EmWiApp.ui)throw new Error("The unit file 'Resources.js' included twice!");
EmWiApp.ui=(function(){var B=EmWiApp;var E={};
var AJ=[0,0];var Be="The property \'FrameSize\' is READ ONLY.";
E.An={bitmap:null,FrameDelay:0,NoOfFrames:1,FrameSize:B.qx,Fe:false,Ij:false,D3:function(
){if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.bitmap=null;this.
FrameSize=AJ;this.FrameDelay=0;this.NoOfFrames=1;this.Fe=false;},CY:function(aArg
){if(!aArg)return;var handle=null;var noOfFrames=1;var frameSize=AJ;var frameDelay=
0;{var bmp=B.tp(aArg,this);if(bmp){noOfFrames=bmp.NoOfFrames;frameSize=bmp.FrameSize;
frameDelay=bmp.FrameDelay;}handle=bmp;}this.bitmap=handle;this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.Ij=true;this.Fe=(!!this.
bitmap&&(this.FrameDelay>0))&&(this.NoOfFrames>1);},Ls:function(C){throw new Error(
Be);},Update:function(){},_Init:function(aArg){B.Core.HE._Init.call(this,aArg);this.
__proto__=E.An;this.CY(aArg);},_Done:function(){this.D3();this.__proto__=B.Core.
HE;B.Core.HE._Done.call(this);},_className:"Resources::Bitmap"};E.BW={font:null,
Leading:0,Descent:0,Ascent:0,D3:function(){if(!this.font)return;var handle=this.
font;B.sX(handle);this.font=null;this.Ascent=0;this.Descent=0;this.Leading=0;},CY:
function(aArg){if(!aArg)return;var handle=null;var ascent=0;var descent=0;var leading=
0;{var font=B.tr(aArg);if(font){ascent=font.Ascent;descent=font.Descent;leading=
font.Leading;}handle=font;}this.font=handle;this.Ascent=ascent;this.Descent=descent;
this.Leading=leading;},Qo:function(aFlowString){if(!this.font)return 0;var handle=
this.font;var advance=0;advance=B.s2(handle,aFlowString);return advance;},T2:function(
aString,aOffset,aWidth,aMaxNoOfRows,aBidi){if(aOffset<0)aOffset=0;if(!this.font||((
aOffset>0)&&(aOffset>=aString.length)))return B.hm;var handle=this.font;var result=
B.hm;result=B.tM(handle,aString,aOffset,aWidth,aMaxNoOfRows,aBidi);return result;
},Lg:function(aString,aOffset,aCount){if(aOffset<0)aOffset=0;if(!this.font||((aOffset>
0)&&(aOffset>=aString.length)))return 0;var handle=this.font;var advance=0;advance=
B.jJ(handle,aString,aOffset,aCount);return advance;},OnGetLeading:function(){return this.
Leading;},OnGetDescent:function(){return this.Descent;},OnGetAscent:function(){return this.
Ascent;},_Init:function(aArg){B.Core.HE._Init.call(this,aArg);this.__proto__=E.BW;
this.CY(aArg);},_Done:function(){this.D3();this.__proto__=B.Core.HE;B.Core.HE._Done.
call(this);},_className:"Resources::Font"};E.Ix={_class:function(){return E.An;}
,0:{FileName:"./res/ResourcesWhiteBitmap.png",Format:B.lp,NoOfFrames:1,FrameSize:[
254,254],FrameDelay:0,_this:null}};E.M1={_class:function(){return E.An;},0:{FileName:
"./res/ResourcesWhiteBitmapStripe.png",Format:B.lp,NoOfFrames:1,FrameSize:[256,4
],FrameDelay:0,_this:null}};E.Ik={_class:function(){return E.An;},0:{FileName:"./res/ResourcesNavigationIconsMedium.png"
,Format:B.lp,NoOfFrames:20,FrameSize:[36,36],FrameDelay:0,_this:null}};E.Tj="Monday";
E.Ui="Tuesday";E.Uk="Wednesday";E.Uf="Thursday";E.SM="Friday";E.T7="Saturday";E.
Ud="Sunday";E.S6="January";E.SL="February";E.Tf="March";E.Si="April";E.Tg="May";
E.S8="June";E.S7="July";E.Sj="August";E.T$="September";E.Tn="October";E.Tm="November";
E.So="December";E.Tk="Mon";E.Uj="Tue";E.Ul="Wed";E.Ug="Thu";E.SN="Fri";E.T8="Sat";
E.Ue="Sun";E.Qt="Jan";E.Qm="Feb";E.Qy="Mar";E.Qc="Apr";E.QA="May";E.Qv="Jun";E.Qu=
"Jul";E.Qd="Aug";E.Q7="Sep";E.QD="Oct";E.QC="Nov";E.Qg="Dec";
E._Init=function(){E.An.__proto__=B.Core.HE;E.BW.__proto__=B.Core.HE;};E.DC=function(
D){var A;if((A=E.Ix[0]._this)&&(A._cycle!=D))A._Done(E.Ix[0]._this=null);if((A=E.
M1[0]._this)&&(A._cycle!=D))A._Done(E.M1[0]._this=null);if((A=E.Ik[0]._this)&&(A.
_cycle!=D))A._Done(E.Ik[0]._this=null);};return E;})();

/* Embedded Wizard */