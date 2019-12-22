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
);EmWiApp.Graphics=(function(){var B=EmWiApp;var E={};
var AJ=[0,0];var Be="Can not resize explicitly attached graphics engine bitmaps";
var Db=[0,0,0,0];var CD="No graphics engine bitmap attached to this canvas";var Fk=
"The canvas is already initialized with a graphics engine bitmap";var Ip="WarpBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var I9="ScaleBitmap() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var HE="DrawThickLine() operation has been omitted due to the resulting area "+
"being wider or higher than 4069 pixel.";var Iq=[1,1];var Lm=[2,2];
E.Canvas={Mq:null,EP:B.pn,Gf:0,Lt:false,DX:function(){if(this.Lt)this.DetachBitmap(
);},CU:function(aArg){this.Ia=true;},K_:function(C){if((C[0]<=0)||(C[1]<=0))C=AJ;
if(B.sa(C,this.FrameSize))return;if(this.Lt)throw new Error(Be);this.FrameSize=C;
this.E8=(((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))&&(this.FrameDelay>0))&&(
this.NoOfFrames>1);if(!this.bitmap)return;var handle=this.bitmap;B.rK(handle);this.
bitmap=null;},Update:function(){var A;if((!this.bitmap&&(this.FrameSize[0]>0))&&(
this.FrameSize[1]>0)){var frameSize=this.FrameSize;var noOfFrames=this.NoOfFrames;
var frameDelay=this.FrameDelay;var handle=null;{handle=B.md(B.b9,frameSize,frameDelay
,noOfFrames);}this.bitmap=handle;if(!this.bitmap){this.FrameSize=AJ;this.FrameDelay=
0;this.NoOfFrames=1;}this.EP=[].concat(AJ,this.FrameSize);}if(!(((A=this.EP)[0]>=
A[2])||(A[1]>=A[3]))){if((this.FrameSize[0]>0)&&(this.FrameSize[1]>0))(A=this.Mq
)?A[1].call(A[0],this):null;this.EP=Db;}},DetachBitmap:function(){if(!this.Lt)throw new
Error(CD);this.bitmap=null;this.Lt=false;this.FrameSize=AJ;this.FrameDelay=0;this.
NoOfFrames=1;this.E8=false;return this;},AttachBitmap:function(aBitmap){if(!!this.
bitmap)throw new Error(Fk);if(!aBitmap)return this;this.bitmap=aBitmap;this.Lt=true;
var noOfFrames=1;var frameSize=AJ;var frameDelay=0;{noOfFrames=aBitmap.NoOfFrames;
frameSize=aBitmap.FrameSize;frameDelay=aBitmap.FrameDelay;}this.NoOfFrames=noOfFrames;
this.FrameSize=frameSize;this.FrameDelay=frameDelay;this.E8=(this.FrameDelay>0)&&(
this.NoOfFrames>1);return this;},P4:function(aClip,O1,aString,aOffset,aCount,aDstRect
,aSrcPos,aMinWidth,O3,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;if(aOffset<0)aOffset=0;if((!O1||!O1.font)||((
aOffset>0)&&(aOffset>=aString.length)))return;var orient=0;if(O3===1)orient=90;else
if(O3===2)orient=180;else if(O3===3)orient=270;var dstFrameNo=this.Gf;var dstBitmap=
this.bitmap;var srcFont=O1.font;{B.mf(dstBitmap,srcFont,aString,aOffset,aCount,dstFrameNo
,aClip,aDstRect,aSrcPos,aMinWidth,orient,aColorTL,aColorTR,aColorBR,aColorBL,aBlend
);}},R8:function(aClip,aBitmap,aFrameNr,aDstRect,Kk,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){var A;if(!this.bitmap)this.Update();if(!this.bitmap)return;if((((!aBitmap||
!aBitmap.bitmap)||!Kk)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var
dstBitmap=this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Gf;var srcRect=[
].concat(AJ,aBitmap.FrameSize);var left=((Kk&0x1)===0x1);var top=((Kk&0x2)===0x2
);var right=((Kk&0x4)===0x4);var bottom=((Kk&0x8)===0x8);var interior=((Kk&0x10)===
0x10);{B.rE(dstBitmap,srcBitmap,dstFrameNo,aFrameNr,aClip,aDstRect,srcRect,left,
top,right,bottom,interior,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},Cu:function(
aClip,Jc,aDstRect,aFlipY,aOffset,aWidth,U4,U0,U2,aMiterLimit,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend,aAntialiased){if(!this.bitmap)this.Update();if(!this.bitmap)return;
if(!Jc||!Jc.path)return;var style=0;switch(U4){case 1:style=style|1;break;case 3:
style=style|2;break;case 2:style=style|3;break;default:;}switch(U0){case 1:style=
style|256;break;case 3:style=style|512;break;case 2:style=style|768;break;default:;
}switch(U2){case 1:style=style|65536;break;case 2:style=style|131072;break;default:;
}var dstBitmap=this.bitmap;var dstFrameNo=this.Gf;var path=Jc.path;{B.s_(dstBitmap
,path,dstFrameNo,aClip,aDstRect,aFlipY,aOffset,aWidth,style,aMiterLimit,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend,aAntialiased);}},OH:function(aClip,aBitmap,aFrameNr
,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4
,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend,aFilter){if(!this.bitmap)this.Update(
);if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=
aBitmap.NoOfFrames))return;var x1=aDstX1;var x2=aDstX2;var y1=aDstY1;var y2=aDstY2;
if(aDstX2<x1)x1=aDstX2;if(aDstX3<x1)x1=aDstX3;if(aDstX4<x1)x1=aDstX4;if(aDstX2>x2
)x2=aDstX2;if(aDstX3>x2)x2=aDstX3;if(aDstX4>x2)x2=aDstX4;if(aDstY2<y1)y1=aDstY2;
if(aDstY3<y1)y1=aDstY3;if(aDstY4<y1)y1=aDstY4;if(aDstY2>y2)y2=aDstY2;if(aDstY3>y2
)y2=aDstY3;if(aDstY4>y2)y2=aDstY4;if(((((x2-x1)>4096.000000)||((x2-x1)<-4096.000000
))||((y2-y1)>4096.000000))||((y2-y1)<-4096.000000)){B.ta("%s",Ip);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Gf;{B.tc(dstBitmap,
srcBitmap,dstFrameNr,aFrameNr,aClip,aDstX1,aDstY1,aDstW1,aDstX2,aDstY2,aDstW2,aDstX3
,aDstY3,aDstW3,aDstX4,aDstY4,aDstW4,aSrcRect,aColor1,aColor2,aColor3,aColor4,aBlend
,aFilter);}},TP:function(aClip,aBitmap,aFrameNr,aDstRect,aSrcRect,aColorTL,aColorTR
,aColorBR,aColorBL,aBlend,aFilter){if(!this.bitmap)this.Update();if(!this.bitmap
)return;if(((!aBitmap||!aBitmap.bitmap)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames
))return;var top=aDstRect[1];var left=aDstRect[0];var right=aDstRect[2];var bottom=
aDstRect[3];if(((((right-left)>4096.000000)||((right-left)<-4096.000000))||((bottom-
top)>4096.000000))||((bottom-top)<-4096.000000)){B.ta("%s",I9);return;}var dstBitmap=
this.bitmap;var srcBitmap=aBitmap.bitmap;var dstFrameNo=this.Gf;{B.tc(dstBitmap,
srcBitmap,dstFrameNo,aFrameNr,aClip,left,top,1,right,top,1,right,bottom,1,left,bottom
,1,aSrcRect,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aFilter);}},L6:function(aClip
,aBitmap,aFrameNr,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(
!this.bitmap)this.Update();if(!this.bitmap)return;if(((!aBitmap||!aBitmap.bitmap
)||(aFrameNr<0))||(aFrameNr>=aBitmap.NoOfFrames))return;var dstBitmap=this.bitmap;
var srcBitmap=aBitmap.bitmap;var dstFrameNr=this.Gf;{B.fo(dstBitmap,srcBitmap,dstFrameNr
,aFrameNr,aClip,aDstRect,aSrcPos,aColorTL,aColorTR,aColorBR,aColorBL,aBlend);}},
B1:function(aClip,Jc,aDstRect,aFlipY,aOffset,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend,aAntialiased,U1){if(!this.bitmap)this.Update();if(!this.bitmap)return;if(
!Jc||!Jc.path)return;var nonZeroWinding=U1===1;var dstBitmap=this.bitmap;var dstFrameNo=
this.Gf;var path=Jc.path;{B.rH(dstBitmap,path,dstFrameNo,aClip,aDstRect,aFlipY,aOffset
,aColorTL,aColorTR,aColorBR,aColorBL,aBlend,aAntialiased,nonZeroWinding);}},P3:function(
aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR,aColorBL,aBlend){if(!this.bitmap
)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;var dstFrameNo=this.
Gf;{B.rF(dstBitmap,dstFrameNo,aClip,aDstRect,aEdgeWidth,aColorTL,aColorTR,aColorBR
,aColorBL,aBlend);}},Mb:function(aClip,aDstRect,aColorTL,aColorTR,aColorBR,aColorBL
,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.
bitmap;var dstFrameNo=this.Gf;{B.g4(dstBitmap,dstFrameNo,aClip,aDstRect,aColorTL
,aColorTR,aColorBR,aColorBL,aBlend);}},R9:function(aClip,aDstPos1,aDstPos2,U6,U7
,aColor1,aColor2,aBlend){if(!this.bitmap)this.Update();if(!this.bitmap)return;var
Hf=U6;var Hg=U7;var Ew=aDstPos1;var Ex=aDstPos2;if(B.sa(Ew,Ex))return;if(Hf<=1)Hf=
1;if(Hg<=1)Hg=1;var HN=Hf/2.000000;var HO=Hg/2.000000;var Jp=Ew[0];var Jq=Ew[1];
var Jr=Ex[0];var Js=Ex[1];var D7=Jr-Jp;var D8=Js-Jq;var Cq=Math.sqrt((D7*D7)+(D8
*D8));if(((Hf>4096)||(Hg>4096))||(Cq>4096.000000)){B.ta("%s",HE);return;}D7=D7/Cq;
D8=D8/Cq;var x1=Jp+(D8*HN);var y1=Jq-(D7*HN);var x2=Jr+(D8*HO);var y2=Js-(D7*HO);
var x3=Jr-(D8*HO);var y3=Js+(D7*HO);var x4=Jp-(D8*HN);var y4=Jq+(D7*HN);var Rw=B.
r0(B.te.MK);var srcRect=[].concat(Iq,B.sn(Rw.FrameSize,Lm));var dstBitmap=this.bitmap;
var dstFrameNo=this.Gf;var srcBitmap=Rw.bitmap;{B.tc(dstBitmap,srcBitmap,dstFrameNo
,0,aClip,x1,y1,1.0,x2,y2,1.0,x3,y3,1.0,x4,y4,1.0,srcRect,aColor1,aColor2,aColor2
,aColor1,aBlend,false);}},L_:function(aClip,aDstPos1,aDstPos2,aColor1,aColor2,aBlend
){if(!this.bitmap)this.Update();if(!this.bitmap)return;var dstBitmap=this.bitmap;
var dstFrameNo=this.Gf;{B.rG(dstBitmap,dstFrameNo,aClip,aDstPos1,aDstPos2,aColor1
,aColor2,aBlend);}},_Init:function(aArg){B.te.An._Init.call(this,aArg);this.__proto__=
E.Canvas;this.CU(aArg);},_Done:function(){this.__proto__=E.Canvas;this.DX();B.te.
An._Done.call(this);},_Mark:function(D){var A;B.te.An._Mark.call(this,D);if((A=this.
Mq)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Graphics::Canvas"};E.
AH={path:null,JF:function(Ae){B.pl(this,0);},DX:function(){if(!this.path)return;
var handle=this.path;B.rM(handle);this.path=null;},A3:function(aSubPathNo,aCenterX
,aCenterY,aRadiusX,aRadiusY,aStartAngle,aEndAngle,aNoOfEdges){B.kD([this,this.JF
],this);var handle=this.path;var result=0;{result=B.rf(handle,aSubPathNo,aCenterX
,aCenterY,aRadiusX,aRadiusY,aStartAngle,aEndAngle,aNoOfEdges);}return result;},L1:
function(aSubPathNo,aCP1X,aCP1Y,aCP2X,aCP2Y,aX,aY,aNoOfEdges){B.kD([this,this.JF
],this);var handle=this.path;var result=0;{result=B.rh(handle,aSubPathNo,aCP1X,aCP1Y
,aCP2X,aCP2Y,aX,aY,aNoOfEdges);}return result;},H5:function(aSubPathNo,aX,aY){B.
kD([this,this.JF],this);var handle=this.path;var result=0;result=B.rj(handle,aSubPathNo
,aX,aY);return result;},EA:function(aSubPathNo){B.kD([this,this.JF],this);var handle=
this.path;B.rw(handle,aSubPathNo);},E9:function(aSubPathNo,aX,aY){B.kD([this,this.
JF],this);var handle=this.path;B.ro(handle,aSubPathNo,aX,aY);},BA:function(aSubPathNo
,aMaxNoOfEdges){if(!this.path&&!aSubPathNo)this.ME(1);B.kD([this,this.JF],this);
var handle=this.path;var result=false;result=B.r9(handle,aSubPathNo,aMaxNoOfEdges
);return result;},Sx:function(){if(!this.path)return 1;var handle=this.path;var result=
0;result=B.rS(handle);return result;},ME:function(aMaxNoOfSubPaths){if(aMaxNoOfSubPaths<
1)aMaxNoOfSubPaths=1;if(!!this.path&&(this.Sx()===aMaxNoOfSubPaths))return true;
var handle=this.path;{B.rM(handle);handle=B.ry(aMaxNoOfSubPaths);}this.path=handle;
B.kD([this,this.JF],this);return!!this.path;},_Init:function(aArg){this.__proto__=
E.AH;B.gb++;},_Done:function(){this.__proto__=E.AH;this.DX();B.gb--;},_ReInit:function(
){},_Mark:function(D){var A;if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:
null,_cycle:0,_observers:null,_className:"Graphics::Path"};E.GM={Left:0x1,TY:0x2
,Right:0x4,R4:0x8,WA:0x10};E.NT={Wp:0,WH:1};E.WK={V8:0,WF:1,TK:2};E.WJ={Ww:0,W_:
1,TK:2,Xf:3};E.V4={V3:0,W6:1,WO:2,WP:3,WR:4,WQ:5};E.Cz={Om:0,CG:360.000000,Ao:0,
Dk:0,Dj:0,AX:0,A5:0,Cv:0,Ei:function(Ae){var AQ=this.CG-this.Ao;var AS=this.Om;if((
AQ>=360.000000)||(AQ<=-360.000000))AQ=360.000000;this.ME(2);if(!AS){var P=this.A5;
if(this.AX>P)P=this.AX;if(AQ===360.000000)AS=P|0;else AS=((AQ*P)/360.000000)|0;if(
AS<0)AS=-AS;AS=AS+1;}if((((AQ===0.000000)||(this.A5===0.000000))||(this.AX===0.000000
))||(((((this.Cv===2)||(this.Cv===3))||(this.Cv===4))||(this.Cv===5))&&((this.Dj>=
this.A5)||(this.Dk>=this.AX)))){this.BA(0,0);this.BA(1,0);return;}if((AQ===360.000000
)&&((((((this.Cv!==2)&&(this.Cv!==3))&&(this.Cv!==4))&&(this.Cv!==5))||(this.Dj===
0.000000))||(this.Dk===0.000000))){this.BA(0,AS+1);this.BA(1,0);this.A3(0,0.000000
,0.000000,this.A5,this.AX,0.000000,360.000000,AS);this.EA(0);return;}if((((AQ===
360.000000)&&((((this.Cv===2)||(this.Cv===3))||(this.Cv===4))||(this.Cv===5)))&&(
this.Dj>0.000000))&&(this.Dk>0.000000)){this.BA(0,AS+1);this.BA(1,AS+1);this.A3(
0,0.000000,0.000000,this.A5,this.AX,0.000000,360.000000,AS);this.A3(1,0.000000,0.000000
,this.Dj,this.Dk,360.000000,0.000000,AS);this.EA(0);this.EA(1);return;}if(!this.
Cv){this.BA(0,AS+1);this.BA(1,0);this.A3(0,0.000000,0.000000,this.A5,this.AX,this.
Ao,this.Ao+AQ,AS);return;}if(this.Cv===1){this.BA(0,AS+1);this.BA(1,0);this.A3(0
,0.000000,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ,AS);this.EA(0);return;}if((
this.Cv===2)&&((this.Dj===0.000000)||(this.Dk===0.000000))){this.BA(0,AS+1);this.
BA(1,0);this.E9(0,0.000000,0.000000);this.A3(0,0.000000,0.000000,this.A5,this.AX
,this.Ao,this.Ao+AQ,AS);this.EA(0);return;}if((this.Cv===4)&&((this.Dj===0.000000
)||(this.Dk===0.000000))){var DC=this.A5*Math.cos(this.Ao*B.ks);var DD=this.AX*Math.
sin(this.Ao*B.ks);var CE=Math.sqrt((DC*DC)+(DD*DD));var EI=((CE*0.250000)|0)+3;this.
BA(0,(AS+EI)+2);this.BA(1,0);this.E9(0,0.000000,0.000000);if(AQ>=0.000000)this.A3(
0,DC/2.000000,DD/2.000000,CE/2.000000,CE/2.000000,this.Ao+180.000000,this.Ao+360.000000
,EI);else this.A3(0,DC/2.000000,DD/2.000000,CE/2.000000,CE/2.000000,this.Ao+180.000000
,this.Ao,EI);this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ,AS);
this.EA(0);return;}if((this.Cv===5)&&((this.Dj===0.000000)||(this.Dk===0.000000)
)){var B9=this.Ao+AQ;var DC=this.A5*Math.cos(B9*B.ks);var DD=this.AX*Math.sin(B9
*B.ks);var CE=Math.sqrt((DC*DC)+(DD*DD));var EI=((CE*0.250000)|0)+3;this.BA(0,(AS+
EI)+2);this.BA(1,0);this.E9(0,0.000000,0.000000);this.A3(0,0.000000,0.000000,this.
A5,this.AX,this.Ao,this.Ao+AQ,AS);if(AQ>=0.000000)this.A3(0,DC/2.000000,DD/2.000000
,CE/2.000000,CE/2.000000,B9,B9+180.000000,EI);else this.A3(0,DC/2.000000,DD/2.000000
,CE/2.000000,CE/2.000000,B9,B9-180.000000,EI);this.EA(0);return;}if((this.Cv===3
)&&((this.Dj===0.000000)||(this.Dk===0.000000))){var H1=this.A5*Math.cos(this.Ao
*B.ks);var H2=this.AX*Math.sin(this.Ao*B.ks);var GH=Math.sqrt((H1*H1)+(H2*H2));var
B9=this.Ao+AQ;var HJ=this.A5*Math.cos(B9*B.ks);var HK=this.AX*Math.sin(B9*B.ks);
var GA=Math.sqrt((HJ*HJ)+(HK*HK));var Jx=((GH*0.250000)|0)+3;var Jw=((GA*0.250000
)|0)+3;this.BA(0,((AS+Jx)+Jw)+10);this.BA(1,0);this.E9(0,0.000000,0.000000);if(AQ>=
0.000000){this.A3(0,H1/2.000000,H2/2.000000,GH/2.000000,GH/2.000000,this.Ao+180.000000
,this.Ao+360.000000,Jx);this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.
Ao+AQ,AS);this.A3(0,HJ/2.000000,HK/2.000000,GA/2.000000,GA/2.000000,B9,B9+180.000000
,Jw);}else{this.A3(0,H1/2.000000,H2/2.000000,GH/2.000000,GH/2.000000,this.Ao+180.000000
,this.Ao,Jx);this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ,AS);
this.A3(0,HJ/2.000000,HK/2.000000,GA/2.000000,GA/2.000000,B9,B9-180.000000,Jw);}
this.EA(0);return;}if(this.Cv===2){this.BA(0,(AS*2)+1);this.BA(1,0);this.A3(0,0.000000
,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ,AS);this.A3(0,0.000000,0.000000,this.
Dj,this.Dk,this.CG,this.CG-AQ,AS);this.EA(0);return;}if(this.Cv===4){var sin=Math.
sin(this.Ao*B.ks);var cos=Math.cos(this.Ao*B.ks);var Fw=this.Dj*cos;var Fx=this.
Dk*sin;var DC=this.A5*cos;var DD=this.AX*sin;var CE=Math.sqrt(((DC-Fw)*(DC-Fw))+((
DD-Fx)*(DD-Fx)));var EI=((CE*0.250000)|0)+3;this.BA(0,((AS*2)+EI)+2);this.BA(1,0
);if(AQ>=0.000000){this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ
,AS);this.A3(0,0.000000,0.000000,this.Dj,this.Dk,this.CG,this.CG-AQ,AS);this.A3(
0,Fw+((DC-Fw)/2.000000),Fx+((DD-Fx)/2.000000),CE/2.000000,CE/2.000000,this.Ao+180.000000
,this.Ao+360.000000,EI);}else{this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao
,this.Ao+AQ,AS);this.A3(0,0.000000,0.000000,this.Dj,this.Dk,this.CG,this.CG-AQ,AS
);this.A3(0,Fw+((DC-Fw)/2.000000),Fx+((DD-Fx)/2.000000),CE/2.000000,CE/2.000000,
this.Ao+180.000000,this.Ao,EI);}this.EA(0);return;}if(this.Cv===5){var B9=this.Ao+
AQ;var sin=Math.sin(B9*B.ks);var cos=Math.cos(B9*B.ks);var Fw=this.Dj*cos;var Fx=
this.Dk*sin;var DC=this.A5*cos;var DD=this.AX*sin;var CE=Math.sqrt(((DC-Fw)*(DC-
Fw))+((DD-Fx)*(DD-Fx)));var EI=((CE*0.250000)|0)+3;this.BA(0,((AS*2)+EI)+2);this.
BA(1,0);if(AQ>=0.000000){this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.
Ao+AQ,AS);this.A3(0,Fw+((DC-Fw)/2.000000),Fx+((DD-Fx)/2.000000),CE/2.000000,CE/2.000000
,B9,B9+180.000000,EI);this.A3(0,0.000000,0.000000,this.Dj,this.Dk,this.CG,this.CG-
AQ,AS);}else{this.A3(0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ,AS);
this.A3(0,Fw+((DC-Fw)/2.000000),Fx+((DD-Fx)/2.000000),CE/2.000000,CE/2.000000,B9
,B9-180.000000,EI);this.A3(0,0.000000,0.000000,this.Dj,this.Dk,this.CG,this.CG-AQ
,AS);}this.EA(0);return;}if(this.Cv===3){var B9=this.Ao+AQ;var RQ=Math.sin(this.
Ao*B.ks);var RI=Math.cos(this.Ao*B.ks);var Rg=Math.sin(B9*B.ks);var Rf=Math.cos(
B9*B.ks);var Kz=this.Dj*RI;var KA=this.Dk*RQ;var H1=this.A5*RI;var H2=this.AX*RQ;
var Kr=this.Dj*Rf;var Ks=this.Dk*Rg;var HJ=this.A5*Rf;var HK=this.AX*Rg;var GH=Math.
sqrt(((H1-Kz)*(H1-Kz))+((H2-KA)*(H2-KA)));var GA=Math.sqrt(((HJ-Kr)*(HJ-Kr))+((HK-
Ks)*(HK-Ks)));var Jx=((GH*0.250000)|0)+3;var Jw=((GA*0.250000)|0)+3;this.BA(0,(((
AS*2)+Jx)+Jw)+4);this.BA(1,0);if(AQ>=0.000000){this.A3(0,0.000000,0.000000,this.
A5,this.AX,this.Ao,this.Ao+AQ,AS);this.A3(0,Kr+((HJ-Kr)/2.000000),Ks+((HK-Ks)/2.000000
),GA/2.000000,GA/2.000000,B9,B9+180.000000,Jw);this.A3(0,0.000000,0.000000,this.
Dj,this.Dk,this.CG,this.CG-AQ,AS);this.A3(0,Kz+((H1-Kz)/2.000000),KA+((H2-KA)/2.000000
),GH/2.000000,GH/2.000000,this.Ao+180.000000,this.Ao+360.000000,Jx);}else{this.A3(
0,0.000000,0.000000,this.A5,this.AX,this.Ao,this.Ao+AQ,AS);this.A3(0,Kr+((HJ-Kr)
/2.000000),Ks+((HK-Ks)/2.000000),GA/2.000000,GA/2.000000,B9,B9-180.000000,Jw);this.
A3(0,0.000000,0.000000,this.Dj,this.Dk,this.CG,this.CG-AQ,AS);this.A3(0,Kz+((H1-
Kz)/2.000000),KA+((H2-KA)/2.000000),GH/2.000000,GH/2.000000,this.Ao+180.000000,this.
Ao,Jx);}this.EA(0);return;}},QD:function(C){if(C<0)C=0;if(C===this.Om)return;this.
Om=C;if(((this.Ao!==this.CG)&&(this.A5>0.000000))&&(this.AX>0.000000))B.kD([this
,this.Ei],this);},Ct:function(C){if(C===this.CG)return;this.CG=C;if((this.A5>0.000000
)&&(this.AX>0.000000))B.kD([this,this.Ei],this);},I2:function(C){if(C===this.Ao)
return;this.Ao=C;if((this.A5>0.000000)&&(this.AX>0.000000))B.kD([this,this.Ei],this
);},J7:function(C){if(C<0.000000)C=0.000000;if(C===this.AX)return;this.AX=C;if((
this.Ao!==this.CG)&&(this.A5>0.000000))B.kD([this,this.Ei],this);},J6:function(C
){if(C<0.000000)C=0.000000;if(C===this.A5)return;this.A5=C;if((this.Ao!==this.CG
)&&(this.AX>0.000000))B.kD([this,this.Ei],this);},Ot:function(C){if((C===this.A5
)&&(C===this.AX))return;this.A5=C;this.AX=C;if(this.Ao!==this.CG)B.kD([this,this.
Ei],this);},Ih:function(C){if(C===this.Cv)return;this.Cv=C;if(((this.Ao!==this.CG
)&&(this.A5>0.000000))&&(this.AX>0.000000))B.kD([this,this.Ei],this);},_Init:function(
aArg){E.AH._Init.call(this,aArg);this.__proto__=E.Cz;},_className:"Graphics::ArcPath"
};
E._Init=function(){E.Canvas.__proto__=B.te.An;E.Cz.__proto__=E.AH;};E.Dy=function(
D){};return E;})();

/* Embedded Wizard */