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
);EmWiApp.Graphics=(function(){var B=EmWiApp;var E={};
var AJ=[0,0];var Be="Can not resize explicitly attached graphics engine bitmaps";
var Dh=[0,0,0,0];var CK="No graphics engine bitmap attached to this canvas";var Ft=
"The canvas is already initialized with a graphics engine bitmap";var Iy="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Jg="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Kx="DrawThickLine() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Ky=[1,1];var LD=[2,2];
E.Canvas={MG:null,EK:B.qy,Gk:0,LK:false,D3:function(){if(this.LK)this.DetachBitmap(
);},CY:function(aArg){this.Ij=true;},Ls:function(C){if((C[0]<=0)||(C[1]<=0))C=AJ;
if(B.tl(C,this.FrameSize))return;if(this.LK)throw new Error(Be);this.FrameSize=C;
this.Fe=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.sW(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.nc(B.ch,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=AJ;this.FrameDelay=
0;this.NoOfFrames=1;}this.EK=[].concat(AJ,this.FrameSize);}if(!(((A=this.EK)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.MG
)?A[1].call(A[0],this):null;this.EK=Dh;}},DetachBitmap:function(){if(!this.LK)throw new
Error(CK);this.bitmap=null;this.LK=false;this.FrameSize=AJ;this.FrameDelay=0;this.
NoOfFrames=1;this.Fe=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(Ft);if(!aBitmap)return this;this.bitmap=aBitmap;this.LK=true;
var noOfFrames=1;var frameSize=AJ;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.Fe=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},Qk:function(aClip,Pg,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,Pi,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!Pg||!Pg.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(Pi===1)orient=90;else
if(Pi===2)orient=180;else if(Pi===3)orient=270;var dstFrameNo=this.Gk;var dstBitmap=
this.bitmap;var srcFont=Pg.font;{B.nf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},Sp:function(aClip,aBitmap,aFrameNr,aDstRect,Kz,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){var A;if(!this.bitmap)this.Update();if(!this.bitmap)return;if((((!aBitmap||
!aBitmap.bitmap)||!Kz)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var
dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Gk;var srcRect=[
].concat(AJ,aBitmap.FrameSize);var left=((Kz&0x1)===0x1);var top=((Kz&0x2)===0x2
);var right=((Kz&0x4)===0x4);var bottom=((Kz&0x8)===0x8);var interior=((Kz&0x10)===
0x10);{B.sQ(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,aDstRect,srcRect,left,
top,right,bottom,interior,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},CJ:function(
aClip,Jm,aDstRect,aFlipY,aOffset,aWidth,Vp,Vl,Vn,aMiterLimit,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend,aAntialiased){if(!this.bitmap)this.Update();if(!this.bitmap)return;
if(!Jm||!Jm.path)return;var style=0;switch(Vp){case 1:style=style|1;break;case 3:
style=style|2;break;case 2:style=style|3;break;default:;}switch(Vl){case 1:style=
style|256;break;case 3:style=style|512;break;case 2:style=style|768;break;default:;
}switch(Vn){case 1:style=style|65536;break;case 2:style=style|131072;break;default:;
}var dstBitmap=this.bitmap;var dstFrameNo=this.Gk;var path=Jm.path;{B.ud(dstBitmap
,path,dstFrameNo,aClip,aDstRect,aFlipY,aOffset,aWidth,style,aMiterLimit,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend,aAntialiased);}},O0:function(aClip,aBitmap,aFrameNr
,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4
,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter){if(!this.bitmap)this.Update(
);if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=
aBitmap.NoOfFrames))return;var x1=aDstX1;var x2=aDstX2;var y1=aDstY1;var y2=aDstY2;
if(aDstX2<x1)x1=aDstX2;if(aDstX3<x1)x1=aDstX3;if(aDstX4<x1)x1=aDstX4;if(aDstX2>x2
)x2=aDstX2;if(aDstX3>x2)x2=aDstX3;if(aDstX4>x2)x2=aDstX4;if(aDstY2<y1)y1=aDstY2;
if(aDstY3<y1)y1=aDstY3;if(aDstY4<y1)y1=aDstY4;if(aDstY2>y2)y2=aDstY2;if(aDstY3>y2
)y2=aDstY3;if(aDstY4>y2)y2=aDstY4;if(((((x2-x1)>4096.000000)||((x2-x1)<-4096.000000
))||((y2-y1)>4096.000000))||((y2-y1)<-4096.000000)){B.uf("%s",Iy);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Gk;{B.ug(dstBitmap,
srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3
,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend
,aFilter);}},T9:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap
)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames
))return;var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=
aDstRect[3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-
top)>4096.000000))||((bottom-top)<-4096.000000)){B.uf("%s",Jg);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Gk;{B.ug(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},Mm:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Gk;{B.fF(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
Cf:function(aClip,Jm,aDstRect,aFlipY,aOffset,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend,aAntialiased,Vm){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(
!Jm||!Jm.path)return;var nonZeroWinding=Vm===1;var dstBitmap=this.bitmap;var dstFrameNo=
this.Gk;var path=Jm.path;{B.sT(dstBitmap,path,dstFrameNo,aClip,aDstRect,aFlipY,aOffset
,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aAntialiased,nonZeroWinding);}},Qj:function(
aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;var dstFrameNo=this.
Gk;{B.sR(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend);}},Mr:function(aClip,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.
bitmap;var dstFrameNo=this.Gk;{B.hn(dstBitmap,dstFrameNo,aClip,aDstRect,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},Sq:function(aClip,aDstPos1,aDstPos2,Vr,Vs
,aColor1,aColor2,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;var
Hm=Vr;var Hn=Vs;var EF=aDstPos1;var EG=aDstPos2;if(B.tl(EF,EG))return;if(Hm<=1)Hm=
1;if(Hn<=1)Hn=1;var HX=Hm/2.000000;var HY=Hn/2.000000;var Jz=EF[0];var JA=EF[1];
var JB=EG[0];var JC=EG[1];var Eb=JB-Jz;var Ec=JC-JA;var Cy=Math.sqrt((Eb*Eb)+(Ec
*Ec));if(((Hm>4096)||(Hn>4096))||(Cy>4096.000000)){B.uf("%s",Kx);return;}Eb=Eb/Cy;
Ec=Ec/Cy;var x1=Jz+(Ec*HX);var y1=JA-(Eb*HX);var x2=JB+(Ec*HY);var y2=JC-(Eb*HY);
var x3=JB-(Ec*HY);var y3=JC+(Eb*HY);var x4=Jz-(Ec*HX);var y4=JA+(Eb*HX);var RP=B.
s$(B.ui.M1);var srcRect=[].concat(Ky,B.tw(RP.FrameSize,LD));var dstBitmap=this.bitmap;
var dstFrameNo=this.Gk;var srcBitmap=RP.bitmap;{B.ug(dstBitmap,srcBitmap,dstFrameNo
,0,aClip,x1,y1,1.0,x2,y2,1.0,x3,y3,1.0,x4,y4,1.0,srcRect,aColor1,aColor2,aColor2
,aColor1,aBlend,false);}},Mo:function(aClip,aDstPos1,aDstPos2,aColor1,aColor2,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.Gk;{B.sS(dstBitmap,dstFrameNo,aClip,aDstPos1,aDstPos2,aColor1
,aColor2,aBlend);}},_Init:function(aArg){B.ui.An._Init.call(this,aArg);this.__proto__=
E.Canvas;this.CY(aArg);},_Done:function(){this.D3();this.__proto__=B.ui.An;B.ui.
An._Done.call(this);},_Mark:function(D){var A;B.ui.An._Mark.call(this,D);if((A=this.
MG)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Graphics::Canvas"};E.
AK={path:null,JP:function(Af){B.qw(this,0);},D3:function(){if(!this.path)return;
var handle=this.path;B.le(handle);this.path=null;},A4:function(aSubPathNo,aCenterX
,aCenterY,aRadiusX,aRadiusY,aStartAngle,aEndAngle,aNoOfEdges){B.lq([this,this.JP
],this);var handle=this.path;var result=0;{result=B.qi(handle,aSubPathNo,aCenterX
,aCenterY,aRadiusX,aRadiusY,aStartAngle,aEndAngle,aNoOfEdges);}return result;},Mh:
function(aSubPathNo,aCP1X,aCP1Y,aCP2X,aCP2Y,aX,aY,aNoOfEdges){B.lq([this,this.JP
],this);var handle=this.path;var result=0;{result=B.m6(handle,aSubPathNo,aCP1X,aCP1Y
,aCP2X,aCP2Y,aX,aY,aNoOfEdges);}return result;},Id:function(aSubPathNo,aX,aY){B.
lq([this,this.JP],this);var handle=this.path;var result=0;result=B.jH(handle,aSubPathNo
,aX,aY);return result;},EJ:function(aSubPathNo){B.lq([this,this.JP],this);var handle=
this.path;B.nb(handle,aSubPathNo);},Ff:function(aSubPathNo,aX,aY){B.lq([this,this.
JP],this);var handle=this.path;B.m8(handle,aSubPathNo,aX,aY);},BD:function(aSubPathNo
,aMaxNoOfEdges){if(!this.path&&!aSubPathNo)this.MT(1);B.lq([this,this.JP],this);
var handle=this.path;var result=false;result=B.ik(handle,aSubPathNo,aMaxNoOfEdges
);return result;},SP:function(){if(!this.path)return 1;var handle=this.path;var result=
0;result=B.s3(handle);return result;},MT:function(aMaxNoOfSubPaths){if(aMaxNoOfSubPaths<
1)aMaxNoOfSubPaths=1;if(!!this.path&&(this.SP()===aMaxNoOfSubPaths))return true;
var handle=this.path;{B.le(handle);handle=B.nd(aMaxNoOfSubPaths);}this.path=handle;
B.lq([this,this.JP],this);return!!this.path;},_Init:function(aArg){this.__proto__=
E.AK;B.gv++;},_Done:function(){this.D3();this.__proto__=null;B.gv--;},_ReInit:function(
){},_Mark:function(D){var A;if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:
null,_cycle:0,_observers:null,_className:"Graphics::Path"};E.GU={Left:0x1,Uh:0x2
,Right:0x4,Sl:0x8,W2:0x10};E.Ob={WQ:0,W$:1};E.Xc={Wv:0,W9:1,T4:2};E.Xb={WX:0,XE:
1,T4:2,XN:3};E.Wr={Wq:0,XA:1,Xg:2,Xh:3,Xj:4,Xi:5};E.CG={OF:0,CN:360.000000,Ao:0,
Dp:0,Do:0,AZ:0,A6:0,CC:0,Eq:function(Af){var AR=this.CN-this.Ao;var AT=this.OF;if((
AR>=360.000000)||(AR<=-360.000000))AR=360.000000;this.MT(2);if(!AT){var N=this.A6;
if(this.AZ>N)N=this.AZ;if(AR===360.000000)AT=N|0;else AT=((AR*N)/360.000000)|0;if(
AT<0)AT=-AT;AT=AT+1;}if((((AR===0.000000)||(this.A6===0.000000))||(this.AZ===0.000000
))||(((((this.CC===2)||(this.CC===3))||(this.CC===4))||(this.CC===5))&&((this.Do>=
this.A6)||(this.Dp>=this.AZ)))){this.BD(0,0);this.BD(1,0);return;}if((AR===360.000000
)&&((((((this.CC!==2)&&(this.CC!==3))&&(this.CC!==4))&&(this.CC!==5))||(this.Do===
0.000000))||(this.Dp===0.000000))){this.BD(0,AT+1);this.BD(1,0);this.A4(0,0.000000
,0.000000,this.A6,this.AZ,0.000000,360.000000,AT);this.EJ(0);return;}if((((AR===
360.000000)&&((((this.CC===2)||(this.CC===3))||(this.CC===4))||(this.CC===5)))&&(
this.Do>0.000000))&&(this.Dp>0.000000)){this.BD(0,AT+1);this.BD(1,AT+1);this.A4(
0,0.000000,0.000000,this.A6,this.AZ,0.000000,360.000000,AT);this.A4(1,0.000000,0.000000
,this.Do,this.Dp,360.000000,0.000000,AT);this.EJ(0);this.EJ(1);return;}if(!this.
CC){this.BD(0,AT+1);this.BD(1,0);this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.
Ao,this.Ao+AR,AT);return;}if(this.CC===1){this.BD(0,AT+1);this.BD(1,0);this.A4(0
,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR,AT);this.EJ(0);return;}if((
this.CC===2)&&((this.Do===0.000000)||(this.Dp===0.000000))){this.BD(0,AT+1);this.
BD(1,0);this.Ff(0,0.000000,0.000000);this.A4(0,0.000000,0.000000,this.A6,this.AZ
,this.Ao,this.Ao+AR,AT);this.EJ(0);return;}if((this.CC===4)&&((this.Do===0.000000
)||(this.Dp===0.000000))){var DF=this.A6*Math.cos(this.Ao*B.ii);var DG=this.AZ*Math.
sin(this.Ao*B.ii);var CL=Math.sqrt((DF*DF)+(DG*DG));var ER=((CL*0.250000)|0)+3;this.
BD(0,(AT+ER)+2);this.BD(1,0);this.Ff(0,0.000000,0.000000);if(AR>=0.000000)this.A4(
0,DF/2.000000,DG/2.000000,CL/2.000000,CL/2.000000,this.Ao+180.000000,this.Ao+360.000000
,ER);else this.A4(0,DF/2.000000,DG/2.000000,CL/2.000000,CL/2.000000,this.Ao+180.000000
,this.Ao,ER);this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR,AT);
this.EJ(0);return;}if((this.CC===5)&&((this.Do===0.000000)||(this.Dp===0.000000)
)){var Cb=this.Ao+AR;var DF=this.A6*Math.cos(Cb*B.ii);var DG=this.AZ*Math.sin(Cb
*B.ii);var CL=Math.sqrt((DF*DF)+(DG*DG));var ER=((CL*0.250000)|0)+3;this.BD(0,(AT+
ER)+2);this.BD(1,0);this.Ff(0,0.000000,0.000000);this.A4(0,0.000000,0.000000,this.
A6,this.AZ,this.Ao,this.Ao+AR,AT);if(AR>=0.000000)this.A4(0,DF/2.000000,DG/2.000000
,CL/2.000000,CL/2.000000,Cb,Cb+180.000000,ER);else this.A4(0,DF/2.000000,DG/2.000000
,CL/2.000000,CL/2.000000,Cb,Cb-180.000000,ER);this.EJ(0);return;}if((this.CC===3
)&&((this.Do===0.000000)||(this.Dp===0.000000))){var H$=this.A6*Math.cos(this.Ao
*B.ii);var Ia=this.AZ*Math.sin(this.Ao*B.ii);var GM=Math.sqrt((H$*H$)+(Ia*Ia));var
Cb=this.Ao+AR;var HT=this.A6*Math.cos(Cb*B.ii);var HU=this.AZ*Math.sin(Cb*B.ii);
var GD=Math.sqrt((HT*HT)+(HU*HU));var JH=((GM*0.250000)|0)+3;var JG=((GD*0.250000
)|0)+3;this.BD(0,((AT+JH)+JG)+10);this.BD(1,0);this.Ff(0,0.000000,0.000000);if(AR>=
0.000000){this.A4(0,H$/2.000000,Ia/2.000000,GM/2.000000,GM/2.000000,this.Ao+180.000000
,this.Ao+360.000000,JH);this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.
Ao+AR,AT);this.A4(0,HT/2.000000,HU/2.000000,GD/2.000000,GD/2.000000,Cb,Cb+180.000000
,JG);}else{this.A4(0,H$/2.000000,Ia/2.000000,GM/2.000000,GM/2.000000,this.Ao+180.000000
,this.Ao,JH);this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR,AT);
this.A4(0,HT/2.000000,HU/2.000000,GD/2.000000,GD/2.000000,Cb,Cb-180.000000,JG);}
this.EJ(0);return;}if(this.CC===2){this.BD(0,(AT*2)+1);this.BD(1,0);this.A4(0,0.000000
,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR,AT);this.A4(0,0.000000,0.000000,this.
Do,this.Dp,this.CN,this.CN-AR,AT);this.EJ(0);return;}if(this.CC===4){var sin=Math.
sin(this.Ao*B.ii);var cos=Math.cos(this.Ao*B.ii);var FD=this.Do*cos;var FE=this.
Dp*sin;var DF=this.A6*cos;var DG=this.AZ*sin;var CL=Math.sqrt(((DF-FD)*(DF-FD))+((
DG-FE)*(DG-FE)));var ER=((CL*0.250000)|0)+3;this.BD(0,((AT*2)+ER)+2);this.BD(1,0
);if(AR>=0.000000){this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR
,AT);this.A4(0,0.000000,0.000000,this.Do,this.Dp,this.CN,this.CN-AR,AT);this.A4(
0,FD+((DF-FD)/2.000000),FE+((DG-FE)/2.000000),CL/2.000000,CL/2.000000,this.Ao+180.000000
,this.Ao+360.000000,ER);}else{this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao
,this.Ao+AR,AT);this.A4(0,0.000000,0.000000,this.Do,this.Dp,this.CN,this.CN-AR,AT
);this.A4(0,FD+((DF-FD)/2.000000),FE+((DG-FE)/2.000000),CL/2.000000,CL/2.000000,
this.Ao+180.000000,this.Ao,ER);}this.EJ(0);return;}if(this.CC===5){var Cb=this.Ao+
AR;var sin=Math.sin(Cb*B.ii);var cos=Math.cos(Cb*B.ii);var FD=this.Do*cos;var FE=
this.Dp*sin;var DF=this.A6*cos;var DG=this.AZ*sin;var CL=Math.sqrt(((DF-FD)*(DF-
FD))+((DG-FE)*(DG-FE)));var ER=((CL*0.250000)|0)+3;this.BD(0,((AT*2)+ER)+2);this.
BD(1,0);if(AR>=0.000000){this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.
Ao+AR,AT);this.A4(0,FD+((DF-FD)/2.000000),FE+((DG-FE)/2.000000),CL/2.000000,CL/2.000000
,Cb,Cb+180.000000,ER);this.A4(0,0.000000,0.000000,this.Do,this.Dp,this.CN,this.CN-
AR,AT);}else{this.A4(0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR,AT);
this.A4(0,FD+((DF-FD)/2.000000),FE+((DG-FE)/2.000000),CL/2.000000,CL/2.000000,Cb
,Cb-180.000000,ER);this.A4(0,0.000000,0.000000,this.Do,this.Dp,this.CN,this.CN-AR
,AT);}this.EJ(0);return;}if(this.CC===3){var Cb=this.Ao+AR;var R9=Math.sin(this.
Ao*B.ii);var R1=Math.cos(this.Ao*B.ii);var Rz=Math.sin(Cb*B.ii);var Ry=Math.cos(
Cb*B.ii);var KR=this.Do*R1;var KS=this.Dp*R9;var H$=this.A6*R1;var Ia=this.AZ*R9;
var KF=this.Do*Ry;var KG=this.Dp*Rz;var HT=this.A6*Ry;var HU=this.AZ*Rz;var GM=Math.
sqrt(((H$-KR)*(H$-KR))+((Ia-KS)*(Ia-KS)));var GD=Math.sqrt(((HT-KF)*(HT-KF))+((HU-
KG)*(HU-KG)));var JH=((GM*0.250000)|0)+3;var JG=((GD*0.250000)|0)+3;this.BD(0,(((
AT*2)+JH)+JG)+4);this.BD(1,0);if(AR>=0.000000){this.A4(0,0.000000,0.000000,this.
A6,this.AZ,this.Ao,this.Ao+AR,AT);this.A4(0,KF+((HT-KF)/2.000000),KG+((HU-KG)/2.000000
),GD/2.000000,GD/2.000000,Cb,Cb+180.000000,JG);this.A4(0,0.000000,0.000000,this.
Do,this.Dp,this.CN,this.CN-AR,AT);this.A4(0,KR+((H$-KR)/2.000000),KS+((Ia-KS)/2.000000
),GM/2.000000,GM/2.000000,this.Ao+180.000000,this.Ao+360.000000,JH);}else{this.A4(
0,0.000000,0.000000,this.A6,this.AZ,this.Ao,this.Ao+AR,AT);this.A4(0,KF+((HT-KF)
/2.000000),KG+((HU-KG)/2.000000),GD/2.000000,GD/2.000000,Cb,Cb-180.000000,JG);this.
A4(0,0.000000,0.000000,this.Do,this.Dp,this.CN,this.CN-AR,AT);this.A4(0,KR+((H$-
KR)/2.000000),KS+((Ia-KS)/2.000000),GM/2.000000,GM/2.000000,this.Ao+180.000000,this.
Ao,JH);}this.EJ(0);return;}},QU:function(C){if(C<0)C=0;if(C===this.OF)return;this.
OF=C;if(((this.Ao!==this.CN)&&(this.A6>0.000000))&&(this.AZ>0.000000))B.lq([this
,this.Eq],this);},CB:function(C){if(C===this.CN)return;this.CN=C;if((this.A6>0.000000
)&&(this.AZ>0.000000))B.lq([this,this.Eq],this);},I9:function(C){if(C===this.Ao)
return;this.Ao=C;if((this.A6>0.000000)&&(this.AZ>0.000000))B.lq([this,this.Eq],this
);},Ki:function(C){if(C<0.000000)C=0.000000;if(C===this.AZ)return;this.AZ=C;if((
this.Ao!==this.CN)&&(this.A6>0.000000))B.lq([this,this.Eq],this);},Kh:function(C
){if(C<0.000000)C=0.000000;if(C===this.A6)return;this.A6=C;if((this.Ao!==this.CN
)&&(this.AZ>0.000000))B.lq([this,this.Eq],this);},OM:function(C){if((C===this.A6
)&&(C===this.AZ))return;this.A6=C;this.AZ=C;if(this.Ao!==this.CN)B.lq([this,this.
Eq],this);},Ir:function(C){if(C===this.CC)return;this.CC=C;if(((this.Ao!==this.CN
)&&(this.A6>0.000000))&&(this.AZ>0.000000))B.lq([this,this.Eq],this);},_Init:function(
aArg){E.AK._Init.call(this,aArg);this.__proto__=E.CG;},_className:"Graphics::ArcPath"
};
E._Init=function(){E.Canvas.__proto__=B.ui.An;E.CG.__proto__=E.AK;};E.DC=function(
D){};return E;})();

/* Embedded Wizard */