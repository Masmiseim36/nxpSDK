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
var AX=[0,0];var BM="Can not resize explicitly attached graphics engine bitmaps";
var Dx=[0,0,0,0];var Fi="No graphics engine bitmap attached to this canvas";var Fj=
"The canvas is already initialized with a graphics engine bitmap";var Ga="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Gb="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
C.Canvas={KJ:null,D$:B.pn,FH:0,HC:false,D_:function(){if(this.HC)this.DetachBitmap(
);},CP:function(aArg){this.GC=true;},JA:function(E){if((E[0]<=0)||(E[1]<=0))E=AX;
if(B.sa(E,this.FrameSize))return;if(this.HC)throw new Error(BM);this.FrameSize=E;
this.DB=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.md(B.b9,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=AX;this.FrameDelay=
0;this.NoOfFrames=1;}this.D$=[].concat(AX,this.FrameSize);}if(!(((A=this.D$)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.KJ
)?A[1].call(A[0],this):null;this.D$=Dx;}},DetachBitmap:function(){if(!this.HC)throw new
Error(Fi);this.bitmap=null;this.HC=false;this.FrameSize=AX;this.FrameDelay=0;this.
NoOfFrames=1;this.DB=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(Fj);if(!aBitmap)return this;this.bitmap=aBitmap;this.HC=true;
var noOfFrames=1;var frameSize=AX;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.DB=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},KD:function(aClip,J0,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,J2,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!J0||!J0.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(J2===1)orient=90;else
if(J2===2)orient=180;else if(J2===3)orient=270;var dstFrameNo=this.FH;var dstBitmap=
this.bitmap;var srcFont=J0.font;{B.mf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},L2:function(aClip,aBitmap,aFrameNr,aDstRect,GP,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){var A;if(!this.bitmap)this.Update();if(!this.bitmap)return;if((((!aBitmap||
!aBitmap.bitmap)||!GP)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var
dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.FH;var srcRect=[
].concat(AX,aBitmap.FrameSize);var left=((GP&0x1)===0x1);var top=((GP&0x2)===0x2
);var right=((GP&0x4)===0x4);var bottom=((GP&0x8)===0x8);var interior=((GP&0x10)===
0x10);{B.rE(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,aDstRect,srcRect,left,
top,right,bottom,interior,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},JO:function(
aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3
,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var Z=aDstX1;var Ac=aDstX2;
var Aa=aDstY1;var Ad=aDstY2;if(aDstX2<Z)Z=aDstX2;if(aDstX3<Z)Z=aDstX3;if(aDstX4<
Z)Z=aDstX4;if(aDstX2>Ac)Ac=aDstX2;if(aDstX3>Ac)Ac=aDstX3;if(aDstX4>Ac)Ac=aDstX4;
if(aDstY2<Aa)Aa=aDstY2;if(aDstY3<Aa)Aa=aDstY3;if(aDstY4<Aa)Aa=aDstY4;if(aDstY2>Ad
)Ad=aDstY2;if(aDstY3>Ad)Ad=aDstY3;if(aDstY4>Ad)Ad=aDstY4;if(((((Ac-Z)>4096.000000
)||((Ac-Z)<-4096.000000))||((Ad-Aa)>4096.000000))||((Ad-Aa)<-4096.000000)){B.ta(
"%s",Ga);return;}var dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=
this.FH;{B.tc(dstBitmap,srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1
,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1
,aColor2,aColor3,aColor4,aBlend,aFilter);}},MI:function(aClip,aBitmap,aFrameNr,aDstRect
,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.
Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(
aFrameNr>=aBitmap.NoOfFrames))return;var top=aDstRect[1];var left=aDstRect[0];var
right=aDstRect[2];var bottom=aDstRect[3];if(((((right-left)>4096.000000)||((right-
left)<-4096.000000))||((bottom-top)>4096.000000))||((bottom-top)<-4096.000000)){
B.ta("%s",Gb);return;}var dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var
dstFrameNo=this.FH;{B.tc(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,
1,right,top,1,right,bottom,1,left,bottom,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend,aFilter);}},Kz:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap
)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames
))return;var dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.
FH;{B.fo(dstBitmap,srcBitmap,dstFrameNr,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},KC:function(aClip,aDstRect,aEdgeWidth,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap
)return;var dstBitmap=this.bitmap;var dstFrameNo=this.FH;{B.rF(dstBitmap,dstFrameNo
,aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},Jl:function(
aClip,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.
Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;var dstFrameNo=this.FH;{
B.g4(dstBitmap,dstFrameNo,aClip,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},_Init:function(aArg){B.tg.Aq._Init.call(this,aArg);this.__proto__=C.Canvas;
this.CP(aArg);},_Done:function(){this.__proto__=C.Canvas;this.D_();B.tg.Aq._Done.
call(this);},_Mark:function(D){var A;B.tg.Aq._Mark.call(this,D);if((A=this.KJ)&&((
A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Graphics::Canvas"};C.H6={Left:
0x1,Py:0x2,Right:0x4,OG:0x8,O5:0x10};
C._Init=function(){C.Canvas.__proto__=B.tg.Aq;};C.Dh=function(D){};return C;})();

/* Embedded Wizard */