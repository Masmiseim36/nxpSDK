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
);EmWiApp.Graphics=(function(){var B=EmWiApp;var D={};
var Aw=[0,0];var A1="Can not resize explicitly attached graphics engine bitmaps";
var BX=[0,0,0,0];var Dy="No graphics engine bitmap attached to this canvas";var Df=
"The canvas is already initialized with a graphics engine bitmap";var Dz="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var DA="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";
D.Canvas={J4:null,C3:B.qy,Ez:0,Go:false,CE:function(){if(this.Go)this.DetachBitmap(
);},BG:function(aArg){this.E$=true;},IM:function(C){if((C[0]<=0)||(C[1]<=0))C=Aw;
if(B.tl(C,this.FrameSize))return;if(this.Go)throw new Error(A1);this.FrameSize=C;
this.CC=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.nc(B.ch,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=Aw;this.FrameDelay=
0;this.NoOfFrames=1;}this.C3=[].concat(Aw,this.FrameSize);}if(!(((A=this.C3)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.J4
)?A[1].call(A[0],this):null;this.C3=BX;}},DetachBitmap:function(){if(!this.Go)throw new
Error(Dy);this.bitmap=null;this.Go=false;this.FrameSize=Aw;this.FrameDelay=0;this.
NoOfFrames=1;this.CC=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(Df);if(!aBitmap)return this;this.bitmap=aBitmap;this.Go=true;
var noOfFrames=1;var frameSize=Aw;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.CC=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},JJ:function(aClip,I3,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,I5,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!I3||!I3.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(I5===1)orient=90;else
if(I5===2)orient=180;else if(I5===3)orient=270;var dstFrameNo=this.Ez;var dstBitmap=
this.bitmap;var srcFont=I3.font;{B.nf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},KQ:function(aClip,aBitmap,aFrameNr,aDstRect,Fj,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){var A;if(!this.bitmap)this.Update();if(!this.bitmap)return;if((((!aBitmap||
!aBitmap.bitmap)||!Fj)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var
dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Ez;var srcRect=[
].concat(Aw,aBitmap.FrameSize);var left=((Fj&0x1)===0x1);var top=((Fj&0x2)===0x2
);var right=((Fj&0x4)===0x4);var bottom=((Fj&0x8)===0x8);var interior=((Fj&0x10)===
0x10);{B.sQ(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,aDstRect,srcRect,left,
top,right,bottom,interior,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},IZ:function(
aClip,aBitmap,aFrameNr,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3
,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var Q=aDstX1;var S=aDstX2;
var R=aDstY1;var T=aDstY2;if(aDstX2<Q)Q=aDstX2;if(aDstX3<Q)Q=aDstX3;if(aDstX4<Q)
Q=aDstX4;if(aDstX2>S)S=aDstX2;if(aDstX3>S)S=aDstX3;if(aDstX4>S)S=aDstX4;if(aDstY2<
R)R=aDstY2;if(aDstY3<R)R=aDstY3;if(aDstY4<R)R=aDstY4;if(aDstY2>T)T=aDstY2;if(aDstY3>
T)T=aDstY3;if(aDstY4>T)T=aDstY4;if(((((S-Q)>4096.000000)||((S-Q)<-4096.000000))||((
T-R)>4096.000000))||((T-R)<-4096.000000)){B.uf("%s",Dz);return;}var dstBitmap=this.
bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Ez;{B.ug(dstBitmap,srcBitmap
,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3
,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter
);}},Mp:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(((
!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;
var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=aDstRect[
3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-top)>
4096.000000))||((bottom-top)<-4096.000000)){B.uf("%s",DA);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Ez;{B.ug(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},Ey:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Ez;{B.fF(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
JI:function(aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.Ez;{B.sR(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},FK:function(aClip,aDstRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;
var dstBitmap=this.bitmap;var dstFrameNo=this.Ez;{B.hn(dstBitmap,dstFrameNo,aClip
,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},_Init:function(aArg){B.
ui.V._Init.call(this,aArg);this.__proto__=D.Canvas;this.BG(aArg);},_Done:function(
){this.CE();this.__proto__=B.ui.V;B.ui.V._Done.call(this);},_Mark:function(E){var
A;B.ui.V._Mark.call(this,E);if((A=this.J4)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=
E);},_className:"Graphics::Canvas"};D.JK={Left:0x1,Oq:0x2,Right:0x4,Nj:0x8,NM:0x10
};
D._Init=function(){D.Canvas.__proto__=B.ui.V;};D.By=function(E){};return D;})();

/* Embedded Wizard */