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
);if(EmWiApp.Graphics)throw new Error("The unit file 'Graphics.js' included twice!"
);EmWiApp.Graphics=(function(){var B=EmWiApp;var D={};
var Az=[0,0];var A3="Can not resize explicitly attached graphics engine bitmaps";
var BY=[0,0,0,0];var CI="No graphics engine bitmap attached to this canvas";var CJ=
"The canvas is already initialized with a graphics engine bitmap";var Du="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Dv="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
D.Canvas={JQ:null,Dc:B.pn,Et:0,Gd:false,CC:function(){if(this.Gd)this.DetachBitmap(
);},BF:function(aArg){this.E3=true;},Is:function(C){if((C[0]<=0)||(C[1]<=0))C=Az;
if(B.sa(C,this.FrameSize))return;if(this.Gd)throw new Error(A3);this.FrameSize=C;
this.CA=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.md(B.b9,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=Az;this.FrameDelay=
0;this.NoOfFrames=1;}this.Dc=[].concat(Az,this.FrameSize);}if(!(((A=this.Dc)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.JQ
)?A[1].call(A[0],this):null;this.Dc=BY;}},DetachBitmap:function(){if(!this.Gd)throw new
Error(CI);this.bitmap=null;this.Gd=false;this.FrameSize=Az;this.FrameDelay=0;this.
NoOfFrames=1;this.CA=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(CJ);if(!aBitmap)return this;this.bitmap=aBitmap;this.Gd=true;
var noOfFrames=1;var frameSize=Az;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.CA=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},Jt:function(aClip,IM,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,IO,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!IM||!IM.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(IO===1)orient=90;else
if(IO===2)orient=180;else if(IO===3)orient=270;var dstFrameNo=this.Et;var dstBitmap=
this.bitmap;var srcFont=IM.font;{B.mf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},KA:function(aClip,aBitmap,aFrameNr,aDstRect,Fa,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){var A;if(!this.bitmap)this.Update();if(!this.bitmap)return;if((((!aBitmap||
!aBitmap.bitmap)||!Fa)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var
dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Et;var srcRect=[
].concat(Az,aBitmap.FrameSize);var left=((Fa&0x1)===0x1);var top=((Fa&0x2)===0x2
);var right=((Fa&0x4)===0x4);var bottom=((Fa&0x8)===0x8);var interior=((Fa&0x10)===
0x10);{B.rE(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,aDstRect,srcRect,left,
top,right,bottom,interior,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},II:function(
aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3
,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var Q=aDstX1;var S=aDstX2;
var R=aDstY1;var T=aDstY2;if(aDstX2<Q)Q=aDstX2;if(aDstX3<Q)Q=aDstX3;if(aDstX4<Q)
Q=aDstX4;if(aDstX2>S)S=aDstX2;if(aDstX3>S)S=aDstX3;if(aDstX4>S)S=aDstX4;if(aDstY2<
R)R=aDstY2;if(aDstY3<R)R=aDstY3;if(aDstY4<R)R=aDstY4;if(aDstY2>T)T=aDstY2;if(aDstY3>
T)T=aDstY3;if(aDstY4>T)T=aDstY4;if(((((S-Q)>4096.000000)||((S-Q)<-4096.000000))||((
T-R)>4096.000000))||((T-R)<-4096.000000)){B.ta("%s",Du);return;}var dstBitmap=this.
bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Et;{B.tc(dstBitmap,srcBitmap
,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3
,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter
);}},L$:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(((
!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;
var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=aDstRect[
3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-top)>
4096.000000))||((bottom-top)<-4096.000000)){B.ta("%s",Dv);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Et;{B.tc(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},Es:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Et;{B.fo(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
Js:function(aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.Et;{B.rF(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},Fx:function(aClip,aDstRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;
var dstBitmap=this.bitmap;var dstFrameNo=this.Et;{B.g4(dstBitmap,dstFrameNo,aClip
,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},_Init:function(aArg){B.
te.W._Init.call(this,aArg);this.__proto__=D.Canvas;this.BF(aArg);},_Done:function(
){this.__proto__=D.Canvas;this.CC();B.te.W._Done.call(this);},_Mark:function(E){
var A;B.te.W._Mark.call(this,E);if((A=this.JQ)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=
E);},_className:"Graphics::Canvas"};D.Ju={Left:0x1,NZ:0x2,Right:0x4,M0:0x8,No:0x10
};
D._Init=function(){D.Canvas.__proto__=B.te.W;};D.Bw=function(E){};return D;})();

/* Embedded Wizard */