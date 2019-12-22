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
var At=[0,0];var AT="Can not resize explicitly attached graphics engine bitmaps";
var Cq=[0,0,0,0];var DW="No graphics engine bitmap attached to this canvas";var DX=
"The canvas is already initialized with a graphics engine bitmap";var Eo="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Ep="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
C.Canvas={HQ:null,CZ:B.pn,EI:0,Fi:false,CY:function(){if(this.Fi)this.DetachBitmap(
);},Cc:function(aArg){this.FT=true;},GO:function(E){if((E[0]<=0)||(E[1]<=0))E=At;
if(B.sa(E,this.FrameSize))return;if(this.Fi)throw new Error(AT);this.FrameSize=E;
this.Dq=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.md(B.b9,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=At;this.FrameDelay=
0;this.NoOfFrames=1;}this.CZ=[].concat(At,this.FrameSize);}if(!(((A=this.CZ)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.HQ
)?A[1].call(A[0],this):null;this.CZ=Cq;}},DetachBitmap:function(){if(!this.Fi)throw new
Error(DW);this.bitmap=null;this.Fi=false;this.FrameSize=At;this.FrameDelay=0;this.
NoOfFrames=1;this.Dq=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(DX);if(!aBitmap)return this;this.bitmap=aBitmap;this.Fi=true;
var noOfFrames=1;var frameSize=At;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.Dq=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},HI:function(aClip,G7,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,G9,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!G7||!G7.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(G9===1)orient=90;else
if(G9===2)orient=180;else if(G9===3)orient=270;var dstFrameNo=this.EI;var dstBitmap=
this.bitmap;var srcFont=G7.font;{B.mf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},G3:function(aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2
,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4
,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||
!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var P=aDstX1;
var R=aDstX2;var Q=aDstY1;var S=aDstY2;if(aDstX2<P)P=aDstX2;if(aDstX3<P)P=aDstX3;
if(aDstX4<P)P=aDstX4;if(aDstX2>R)R=aDstX2;if(aDstX3>R)R=aDstX3;if(aDstX4>R)R=aDstX4;
if(aDstY2<Q)Q=aDstY2;if(aDstY3<Q)Q=aDstY3;if(aDstY4<Q)Q=aDstY4;if(aDstY2>S)S=aDstY2;
if(aDstY3>S)S=aDstY3;if(aDstY4>S)S=aDstY4;if(((((R-P)>4096.000000)||((R-P)<-4096.000000
))||((S-Q)>4096.000000))||((S-Q)<-4096.000000)){B.ta("%s",Eo);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.EI;{B.tc(dstBitmap,
srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3
,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend
,aFilter);}},I7:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap
)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames
))return;var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=
aDstRect[3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-
top)>4096.000000))||((bottom-top)<-4096.000000)){B.ta("%s",Ep);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.EI;{B.tc(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},HE:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.EI;{B.fo(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
HH:function(aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.EI;{B.rF(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},GH:function(aClip,aDstRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;
var dstBitmap=this.bitmap;var dstFrameNo=this.EI;{B.g4(dstBitmap,dstFrameNo,aClip
,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},_Init:function(aArg){B.
tf.Ae._Init.call(this,aArg);this.__proto__=C.Canvas;this.Cc(aArg);},_Done:function(
){this.__proto__=C.Canvas;this.CY();B.tf.Ae._Done.call(this);},_Mark:function(D){
var A;B.tf.Ae._Mark.call(this,D);if((A=this.HQ)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Graphics::Canvas"};
C._Init=function(){C.Canvas.__proto__=B.tf.Ae;};C.BO=function(D){};return C;})();

/* Embedded Wizard */