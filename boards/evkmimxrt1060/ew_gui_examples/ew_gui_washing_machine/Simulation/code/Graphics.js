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
var AX=[0,0];var BI="Can not resize explicitly attached graphics engine bitmaps";
var Dz=[0,0,0,0];var Gb="No graphics engine bitmap attached to this canvas";var Gc=
"The canvas is already initialized with a graphics engine bitmap";var Gd="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Ge="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
C.Canvas={KZ:null,D0:B.qy,FM:0,HL:false,Ee:function(){if(this.HL)this.DetachBitmap(
);},CP:function(aArg){this.GL=true;},JT:function(E){if((E[0]<=0)||(E[1]<=0))E=AX;
if(B.tl(E,this.FrameSize))return;if(this.HL)throw new Error(BI);this.FrameSize=E;
this.DD=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.nc(B.ch,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=AX;this.FrameDelay=
0;this.NoOfFrames=1;}this.D0=[].concat(AX,this.FrameSize);}if(!(((A=this.D0)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.KZ
)?A[1].call(A[0],this):null;this.D0=Dz;}},DetachBitmap:function(){if(!this.HL)throw new
Error(Gb);this.bitmap=null;this.HL=false;this.FrameSize=AX;this.FrameDelay=0;this.
NoOfFrames=1;this.DD=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(Gc);if(!aBitmap)return this;this.bitmap=aBitmap;this.HL=true;
var noOfFrames=1;var frameSize=AX;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.DD=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},KU:function(aClip,Kg,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,Ki,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!Kg||!Kg.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(Ki===1)orient=90;else
if(Ki===2)orient=180;else if(Ki===3)orient=270;var dstFrameNo=this.FM;var dstBitmap=
this.bitmap;var srcFont=Kg.font;{B.nf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},Mh:function(aClip,aBitmap,aFrameNr,aDstRect,G3,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){var A;if(!this.bitmap)this.Update();if(!this.bitmap)return;if((((!aBitmap||
!aBitmap.bitmap)||!G3)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var
dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.FM;var srcRect=[
].concat(AX,aBitmap.FrameSize);var left=((G3&0x1)===0x1);var top=((G3&0x2)===0x2
);var right=((G3&0x4)===0x4);var bottom=((G3&0x8)===0x8);var interior=((G3&0x10)===
0x10);{B.sQ(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,aDstRect,srcRect,left,
top,right,bottom,interior,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},J6:function(
aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3
,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var Y=aDstX1;var Ab=aDstX2;
var Z=aDstY1;var Ac=aDstY2;if(aDstX2<Y)Y=aDstX2;if(aDstX3<Y)Y=aDstX3;if(aDstX4<Y
)Y=aDstX4;if(aDstX2>Ab)Ab=aDstX2;if(aDstX3>Ab)Ab=aDstX3;if(aDstX4>Ab)Ab=aDstX4;if(
aDstY2<Z)Z=aDstY2;if(aDstY3<Z)Z=aDstY3;if(aDstY4<Z)Z=aDstY4;if(aDstY2>Ac)Ac=aDstY2;
if(aDstY3>Ac)Ac=aDstY3;if(aDstY4>Ac)Ac=aDstY4;if(((((Ab-Y)>4096.000000)||((Ab-Y)<-
4096.000000))||((Ac-Z)>4096.000000))||((Ac-Z)<-4096.000000)){B.uf("%s",Gd);return;
}var dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.FM;{
B.ug(dstBitmap,srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2
,aDstW2,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3
,aColor4,aBlend,aFilter);}},MZ:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect
,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update(
);if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=
aBitmap.NoOfFrames))return;var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[
2];var bottom=aDstRect[3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000
))||((bottom-top)>4096.000000))||((bottom-top)<-4096.000000)){B.uf("%s",Ge);return;
}var dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.FM;{
B.ug(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,
bottom,1,left,bottom,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter
);}},KR:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||
!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.FM;{B.fF(dstBitmap,
srcBitmap,dstFrameNr,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,
aColorBL,aBlend);}},KT:function(aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=
this.bitmap;var dstFrameNo=this.FM;{B.sR(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth
,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},JF:function(aClip,aDstRect,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap
)return;var dstBitmap=this.bitmap;var dstFrameNo=this.FM;{B.hn(dstBitmap,dstFrameNo
,aClip,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},_Init:function(aArg
){B.uk.As._Init.call(this,aArg);this.__proto__=C.Canvas;this.CP(aArg);},_Done:function(
){this.Ee();this.__proto__=B.uk.As;B.uk.As._Done.call(this);},_Mark:function(D){
var A;B.uk.As._Mark.call(this,D);if((A=this.KZ)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Graphics::Canvas"};C.If={Left:0x1,P3:0x2,Right:0x4,O3:0x8
,Pt:0x10};
C._Init=function(){C.Canvas.__proto__=B.uk.As;};C.Dj=function(D){};return C;})();

/* Embedded Wizard */