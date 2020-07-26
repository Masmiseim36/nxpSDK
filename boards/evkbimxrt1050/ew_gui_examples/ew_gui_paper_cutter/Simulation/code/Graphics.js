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
);if(EmWiApp.Graphics)throw new Error("The unit file 'Graphics.js' included twice!"
);EmWiApp.Graphics=(function(){var B=EmWiApp;var C={};
var AM=[0,0];var BD="Can not resize explicitly attached graphics engine bitmaps";
var Dj=[0,0,0,0];var FO="No graphics engine bitmap attached to this canvas";var DV=
"The canvas is already initialized with a graphics engine bitmap";var FP="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var FQ="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
C.Canvas={J1:null,DN:B.qy,Gi:0,G6:false,D3:function(){if(this.G6)this.DetachBitmap(
);},Bc:function(aArg){this.HJ=true;},IZ:function(E){if((E[0]<=0)||(E[1]<=0))E=AM;
if(B.tl(E,this.FrameSize))return;if(this.G6)throw new Error(BD);this.FrameSize=E;
this.EM=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.nc(B.ch,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=AM;this.FrameDelay=
0;this.NoOfFrames=1;}this.DN=[].concat(AM,this.FrameSize);}if(!(((A=this.DN)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.J1
)?A[1].call(A[0],this):null;this.DN=Dj;}},DetachBitmap:function(){if(!this.G6)throw new
Error(FO);this.bitmap=null;this.G6=false;this.FrameSize=AM;this.FrameDelay=0;this.
NoOfFrames=1;this.EM=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(DV);if(!aBitmap)return this;this.bitmap=aBitmap;this.G6=true;
var noOfFrames=1;var frameSize=AM;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.EM=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},JU:function(aClip,Je,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,Jh,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!Je||!Je.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(Jh===1)orient=90;else
if(Jh===2)orient=180;else if(Jh===3)orient=270;var dstFrameNo=this.Gi;var dstBitmap=
this.bitmap;var srcFont=Je.font;{B.nf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},I_:function(aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2
,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4
,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||
!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var U=aDstX1;
var W=aDstX2;var V=aDstY1;var X=aDstY2;if(aDstX2<U)U=aDstX2;if(aDstX3<U)U=aDstX3;
if(aDstX4<U)U=aDstX4;if(aDstX2>W)W=aDstX2;if(aDstX3>W)W=aDstX3;if(aDstX4>W)W=aDstX4;
if(aDstY2<V)V=aDstY2;if(aDstY3<V)V=aDstY3;if(aDstY4<V)V=aDstY4;if(aDstY2>X)X=aDstY2;
if(aDstY3>X)X=aDstY3;if(aDstY4>X)X=aDstY4;if(((((W-U)>4096.000000)||((W-U)<-4096.000000
))||((X-V)>4096.000000))||((X-V)<-4096.000000)){B.uf("%s",FP);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Gi;{B.ug(dstBitmap,
srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3
,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend
,aFilter);}},LM:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap
)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames
))return;var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=
aDstRect[3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-
top)>4096.000000))||((bottom-top)<-4096.000000)){B.uf("%s",FQ);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Gi;{B.ug(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},JQ:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Gi;{B.fF(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
JT:function(aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.Gi;{B.sR(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},IL:function(aClip,aDstRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;
var dstBitmap=this.bitmap;var dstFrameNo=this.Gi;{B.hn(dstBitmap,dstFrameNo,aClip
,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},_Init:function(aArg){B.
uj.An._Init.call(this,aArg);this.__proto__=C.Canvas;this.Bc(aArg);},_Done:function(
){this.D3();this.__proto__=B.uj.An;B.uj.An._Done.call(this);},_Mark:function(D){
var A;B.uj.An._Mark.call(this,D);if((A=this.J1)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Graphics::Canvas"};
C._Init=function(){C.Canvas.__proto__=B.uj.An;};C.C2=function(D){};return C;})();

/* Embedded Wizard */