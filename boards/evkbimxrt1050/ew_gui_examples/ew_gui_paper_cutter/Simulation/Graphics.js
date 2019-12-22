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
);EmWiApp.Graphics=(function(){var B=EmWiApp;var C={};
var AO=[0,0];var BF="Can not resize explicitly attached graphics engine bitmaps";
var Dh=[0,0,0,0];var Fh="No graphics engine bitmap attached to this canvas";var Dw=
"The canvas is already initialized with a graphics engine bitmap";var FH="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var FI="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
C.Canvas={JK:null,D2:B.pn,F$:0,GY:false,DZ:function(){if(this.GY)this.DetachBitmap(
);},Bb:function(aArg){this.HB=true;},IF:function(E){if((E[0]<=0)||(E[1]<=0))E=AO;
if(B.sa(E,this.FrameSize))return;if(this.GY)throw new Error(BF);this.FrameSize=E;
this.EG=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.md(B.b9,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=AO;this.FrameDelay=
0;this.NoOfFrames=1;}this.D2=[].concat(AO,this.FrameSize);}if(!(((A=this.D2)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.JK
)?A[1].call(A[0],this):null;this.D2=Dh;}},DetachBitmap:function(){if(!this.GY)throw new
Error(Fh);this.bitmap=null;this.GY=false;this.FrameSize=AO;this.FrameDelay=0;this.
NoOfFrames=1;this.EG=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(Dw);if(!aBitmap)return this;this.bitmap=aBitmap;this.GY=true;
var noOfFrames=1;var frameSize=AO;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.EG=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},JB:function(aClip,IX,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,I0,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!IX||!IX.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(I0===1)orient=90;else
if(I0===2)orient=180;else if(I0===3)orient=270;var dstFrameNo=this.F$;var dstBitmap=
this.bitmap;var srcFont=IX.font;{B.mf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},IR:function(aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2
,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4
,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||
!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var V=aDstX1;
var X=aDstX2;var W=aDstY1;var Y=aDstY2;if(aDstX2<V)V=aDstX2;if(aDstX3<V)V=aDstX3;
if(aDstX4<V)V=aDstX4;if(aDstX2>X)X=aDstX2;if(aDstX3>X)X=aDstX3;if(aDstX4>X)X=aDstX4;
if(aDstY2<W)W=aDstY2;if(aDstY3<W)W=aDstY3;if(aDstY4<W)W=aDstY4;if(aDstY2>Y)Y=aDstY2;
if(aDstY3>Y)Y=aDstY3;if(aDstY4>Y)Y=aDstY4;if(((((X-V)>4096.000000)||((X-V)<-4096.000000
))||((Y-W)>4096.000000))||((Y-W)<-4096.000000)){B.ta("%s",FH);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.F$;{B.tc(dstBitmap,
srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3
,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend
,aFilter);}},Lv:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap
)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames
))return;var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=
aDstRect[3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-
top)>4096.000000))||((bottom-top)<-4096.000000)){B.ta("%s",FI);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.F$;{B.tc(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},Jx:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.F$;{B.fo(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
JA:function(aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.F$;{B.rF(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},Iv:function(aClip,aDstRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;
var dstBitmap=this.bitmap;var dstFrameNo=this.F$;{B.g4(dstBitmap,dstFrameNo,aClip
,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},_Init:function(aArg){B.
tf.An._Init.call(this,aArg);this.__proto__=C.Canvas;this.Bb(aArg);},_Done:function(
){this.__proto__=C.Canvas;this.DZ();B.tf.An._Done.call(this);},_Mark:function(D){
var A;B.tf.An._Mark.call(this,D);if((A=this.JK)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Graphics::Canvas"};
C._Init=function(){C.Canvas.__proto__=B.tf.An;};C.CY=function(D){};return C;})();

/* Embedded Wizard */