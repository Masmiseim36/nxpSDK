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
);if(EmWiApp.un)throw new Error("The unit file 'Accelerator.js' included twice!"
);EmWiApp.un=(function(){var B=EmWiApp;var D={};
var Aw=" fps";var A1=" %%";var BX="-";var Dy="Rectangle Fill";var Df="Rectangle Blend";
var Dz="RGB565 Bitmap";var DA="Native Bitmap";var I0="RGB565 Bitmap + Opacity";var
I1="Native Bitmap + Opacity";var I2="Alpha8 Bitmap";var Kf=[0,0,480,272];var Mw=[
0,40,360,272];var Mx=[375,5,475,25];var My="Framerate";var Mz=[370,25,470,65];var
MA="0 fps";var MB=[375,65,475,85];var MC="CPU Load";var MD=[370,85,470,125];var ME=
"0 %%";var MF=[375,200,475,220];var MG="Scenario";var MH=[375,120,470,160];var MI=
"Graphics Accelerator";var MJ=[10,0,310,36];var MK="Alpha8 Blend";var ML=[370,225
,470,265];var MM="Next";var MN=[385,150,455,210];var Gi=[0,0];var MO=[0,0,400,240
];
D.HN={H5:null,C7:null,BD:null,Fe:null,DN:null,Dt:null,Ds:null,Cj:null,DW:null,Dc:
null,Cd:null,Cw:null,Ex:null,Cx:null,Lg:16,Lf:8,Ld:8,Lb:8,Le:8,Lc:8,La:8,Ja:5,Hg:
function(aSize){this.II(this);},BG:function(aArg){this.Dc.Ga(false);this.Cx.Ga(false
);this.Ds.Ga(false);this.Cj.Ga(false);this.EE(this.Cw);},Mm:function(AV){var Gq=
this.H5.K3();if(!!this.FL())this.Dt.Bw(this.FL().GetFPS().toFixed()+Aw);if((Gq>=
0)&&(Gq<=100)){this.Cj.Bw(Gq.toFixed()+A1);var BH=((((Gq*255)/100)|0)&0xFF)|((((((
100-Gq)*255)/100)|0)&0xFF)<<8)|(0<<16)|(255<<24);this.Cj.BP(BH);}else{this.Cj.Bw(
BX);this.Cj.BP(0xFFFFFFFF);}},II:function(AV){var E3=this.Ja+1;while(E3!==this.Ja
){if(this.KN(E3))this.Ja=E3;else E3=E3+1;if(E3>6)E3=0;}},KN:function(AW){var Ew=
6;var Cm=0;if((AW<0)||(AW>6))return false;switch(AW){case 0:{Cm=this.Lg;if(Cm<=0
)return false;Ew=0;this.Cd.Bw(Dy);}break;case 1:{Cm=this.Lf;if(Cm<=0)return false;
Ew=1;this.Cd.Bw(Df);}break;case 2:{Cm=this.Ld;if(Cm<=0)return false;Ew=2;this.Cd.
Bw(Dz);}break;case 3:{Cm=this.Lb;if(Cm<=0)return false;Ew=4;this.Cd.Bw(DA);}break;
case 4:{Cm=this.Le;if(Cm<=0)return false;Ew=3;this.Cd.Bw(I0);}break;case 5:{Cm=this.
Lc;if(Cm<=0)return false;Ew=5;this.Cd.Bw(I1);}break;default:{Cm=this.La;if(Cm<=0
)return false;Ew=6;this.Cd.Bw(I2);}}this.BD.Gg=Ew;this.BD.Mg(Cm);return true;},_Init:
function(aArg){B.Core.Ag._Init.call(this,aArg);B.uj.C7._Init.call(this.C7={Ad:this
},0);D.BD._Init.call(this.BD={Ad:this},0);B.Core.Timer._Init.call(this.Fe={Ad:this
},0);B.uj.Text._Init.call(this.DN={Ad:this},0);B.uj.Text._Init.call(this.Dt={Ad:
this},0);B.uj.Text._Init.call(this.Ds={Ad:this},0);B.uj.Text._Init.call(this.Cj={
Ad:this},0);B.uj.Text._Init.call(this.DW={Ad:this},0);B.uj.Text._Init.call(this.
Dc={Ad:this},0);B.uj.Text._Init.call(this.Cd={Ad:this},0);B.ul.Cw._Init.call(this.
Cw={Ad:this},0);B.Core.Timer._Init.call(this.Ex={Ad:this},0);B.ul.Cx._Init.call(
this.Cx={Ad:this},0);this.__proto__=D.HN;var A;this.Ah(Kf);this.C7.Ah(Kf);this.C7.
BP(0xFF200808);this.BD.Ah(Mw);this.Fe.Bv(true);this.DN.Ah(Mx);this.DN.Cv(0x11);this.
DN.Bw(My);this.Dt.Ah(Mz);this.Dt.Cv(0x12);this.Dt.Bw(MA);this.Ds.Ah(MB);this.Ds.
Cv(0x11);this.Ds.Bw(MC);this.Cj.Ah(MD);this.Cj.Cv(0x12);this.Cj.Bw(ME);this.Cj.BP(
0xFFFFFFFF);this.DW.Ah(MF);this.DW.Cv(0x11);this.DW.Bw(MG);this.Dc.Ah(MH);this.Dc.
IR(true);this.Dc.Cv(0x11);this.Dc.Bw(MI);this.Cd.Ah(MJ);this.Cd.Cv(0x11);this.Cd.
Bw(MK);this.Cw.Ah(ML);this.Cw.IO(MM);this.Ex.F$(5000);this.Ex.ED(0);this.Cx.Ah(MN
);this.Cx.IO(B.hm);this.Bc(this.C7,0);this.Bc(this.BD,0);this.Bc(this.DN,0);this.
Bc(this.Dt,0);this.Bc(this.Ds,0);this.Bc(this.Cj,0);this.Bc(this.DW,0);this.Bc(this.
Dc,0);this.Bc(this.Cd,0);this.Bc(this.Cw,0);this.Bc(this.Cx,0);this.Fe.EG=[this,
this.Mm];this.DN.De(B.s$(D.E8));this.Dt.De(B.s$(D.GZ));this.Ds.De(B.s$(D.E8));this.
Cj.De(B.s$(D.GZ));this.DW.De(B.s$(D.E8));this.Dc.De(B.s$(D.E8));this.Cd.De(B.s$(
D.DM));this.Cw.EC=[this,this.II];this.Cw.IL(B._GetAutoObject(B.ul.IT));this.Ex.EG=[
this,this.II];this.H5=B._GetAutoObject(B.GraphicsSystem.Device);this.Cx.Mi([A=B.
_GetAutoObject(B.GraphicsSystem.Device),A.J5,A.J6]);this.Cx.IL(B._GetAutoObject(
B.ul.IW));this.BG(aArg);},_Done:function(){this.__proto__=B.Core.Ag;this.C7._Done(
);this.BD._Done();this.Fe._Done();this.DN._Done();this.Dt._Done();this.Ds._Done(
);this.Cj._Done();this.DW._Done();this.Dc._Done();this.Cd._Done();this.Cw._Done(
);this.Ex._Done();this.Cx._Done();B.Core.Ag._Done.call(this);},_ReInit:function(
){B.Core.Ag._ReInit.call(this);this.C7._ReInit();this.BD._ReInit();this.Fe._ReInit(
);this.DN._ReInit();this.Dt._ReInit();this.Ds._ReInit();this.Cj._ReInit();this.DW.
_ReInit();this.Dc._ReInit();this.Cd._ReInit();this.Cw._ReInit();this.Ex._ReInit(
);this.Cx._ReInit();},_Mark:function(E){var A;B.Core.Ag._Mark.call(this,E);if((A=
this.H5)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.C7)._cycle!=E)A._Mark(A._cycle=
E);if((A=this.BD)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Fe)._cycle!=E)A._Mark(
A._cycle=E);if((A=this.DN)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Dt)._cycle!=
E)A._Mark(A._cycle=E);if((A=this.Ds)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Cj
)._cycle!=E)A._Mark(A._cycle=E);if((A=this.DW)._cycle!=E)A._Mark(A._cycle=E);if((
A=this.Dc)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Cd)._cycle!=E)A._Mark(A._cycle=
E);if((A=this.Cw)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Ex)._cycle!=E)A._Mark(
A._cycle=E);if((A=this.Cx)._cycle!=E)A._Mark(A._cycle=E);},_className:"Accelerator::Accelerator"
};D.GZ={_class:function(){return B.ui.AT;},0:{Data:function(){return B.up;},Cache:[
],_this:null}};D.IG={_class:function(){return B.ui.V;},0:{FileName:"./res/AcceleratorLogo.png"
,Format:B.lp,NoOfFrames:1,FrameSize:[330,80],FrameDelay:0,_this:null}};D.E8={_class:
function(){return B.ui.AT;},0:{Data:function(){return B.ur;},Cache:[],_this:null
}};D.BD={Cu:B.tA(7,null,null),C6:B.tA(5,null,null),GY:null,Bx:B.tA(64,B.qx,null)
,F0:1,GV:B.tA(64,B.qx,null),CD:B.tA(64,0,null),C8:B.tA(64,B.qx,null),Hf:B.tA(64,
0,null),Gg:6,DK:B.tA(64,0,null),Ck:function(Aq,aClip,aOffset,AC,aBlend){var A;var
Ae=0;B.Core.Ag.Ck.call(this,Aq,aClip,aOffset,AC,aBlend);aOffset=B.tx(aOffset,this.
M.slice(0,2));while(Ae<64){var bmp;var Au=B.tx(this.Bx.Get(Ae),aOffset);var Y=this.
C8.Get(Ae);var A2=this.CD.Get(Ae);if(this.Bx.Get(Ae)[1]<=((A=this.M)[3]-A[1])){switch(
this.Hf.Get(Ae)){case 0:{Aq.FK(aClip,[].concat(Au,B.tx(Au,Y)),A2,A2,A2,A2,aBlend
);}break;case 1:{Aq.FK(aClip,[].concat(Au,B.tx(Au,Y)),A2,A2,A2,A2,aBlend);}break;
case 2:{bmp=this.C6.Get(Ae%5);Aq.Ey(aClip,bmp,0,[].concat(Au,B.tx(Au,Y)),Gi,A2,A2
,A2,A2,aBlend);}break;case 4:{bmp=this.Cu.Get(Ae%7);Aq.Ey(aClip,bmp,0,[].concat(
Au,B.tx(Au,Y)),Gi,A2,A2,A2,A2,aBlend);}break;case 3:{bmp=this.C6.Get(Ae%5);Aq.Ey(
aClip,bmp,0,[].concat(Au,B.tx(Au,Y)),Gi,A2,A2,A2,A2,aBlend);}break;case 5:{bmp=this.
Cu.Get(Ae%7);Aq.Ey(aClip,bmp,0,[].concat(Au,B.tx(Au,Y)),Gi,A2,A2,A2,A2,aBlend);}
break;case 6:{bmp=this.GY;Aq.Ey(aClip,bmp,0,[].concat(Au,B.tx(Au,Y)),Gi,A2,A2,A2
,A2,aBlend);}break;default:;}}Ae=Ae+1;}B.tg([this,this.Kc],this);},Hg:function(aSize
){var A;var Ae;for(Ae=0;Ae<this.F0;Ae=Ae+1){this.IU(Ae);this.Bx.Set(Ae,[this.Bx.
Get(Ae)[0],B.tP(0,((A=this.M)[3]-A[1])-this.C8.Get(Ae)[1])]);}B.tg([this,this.Kc
],this);},Mg:function(C){var Ae;if(C<0)C=0;if(C>64)C=64;if(this.F0===C)return;for(
Ae=this.F0;Ae<C;Ae=Ae+1)this.IU(Ae);this.F0=C;},Kc:function(AV){var A;this.Ap([0
,0,(A=this.M)[2]-A[0],A[3]-A[1]]);var Ae=0;while(Ae<64){var Y=this.C8.Get(Ae);if(
this.Bx.Get(Ae)[1]<=((A=this.M)[3]-A[1])){this.Bx.Set(Ae,B.tx(this.Bx.Get(Ae),this.
GV.Get(Ae)));if(((this.Bx.Get(Ae)[0]+Y[0])<0)||(this.Bx.Get(Ae)[0]>((A=this.M)[2
]-A[0])))this.Bx.Set(Ae,[this.Bx.Get(Ae)[0],((A=this.M)[3]-A[1])+1]);}if((this.Bx.
Get(Ae)[1]>((A=this.M)[3]-A[1]))&&(Ae<this.F0))this.IU(Ae);if((this.Gg===3)||(this.
Gg===5)){var Gm=((this.CD.Get(Ae)>>24)&0xFF)+this.DK.Get(Ae);if(Gm>255){Gm=255;this.
DK.Set(Ae,-this.DK.Get(Ae));}if(Gm<0){Gm=0;this.DK.Set(Ae,-this.DK.Get(Ae));}this.
CD.Set(Ae,(this.CD.Get(Ae)&0x00FFFFFF)|((Gm&0xFF)<<24));}Ae=Ae+1;}},IU:function(
AW){var A;var Y;switch(this.Gg){case 0:{Y=[B.tP((((A=this.M)[2]-A[0])/3)|0,((2*((
A=this.M)[2]-A[0]))/3)|0),B.tP((((A=this.M)[3]-A[1])/3)|0,((2*((A=this.M)[3]-A[1
]))/3)|0)];this.Bx.Set(AW,[B.tP(0,((A=this.M)[2]-A[0])-Y[0]),-Y[1]]);this.C8.Set(
AW,Y);this.CD.Set(AW,(B.tP(0,255)&0xFF)|((B.tP(0,255)&0xFF)<<8)|((B.tP(0,255)&0xFF
)<<16)|(255<<24));}break;case 1:{Y=[B.tP((((A=this.M)[2]-A[0])/3)|0,((2*((A=this.
M)[2]-A[0]))/3)|0),B.tP((((A=this.M)[3]-A[1])/3)|0,((2*((A=this.M)[3]-A[1]))/3)|
0)];this.Bx.Set(AW,[B.tP(0,((A=this.M)[2]-A[0])-Y[0]),-Y[1]]);this.C8.Set(AW,Y);
this.CD.Set(AW,(B.tP(0,255)&0xFF)|((B.tP(0,255)&0xFF)<<8)|((B.tP(0,255)&0xFF)<<16
)|(170<<24));}break;case 2:{Y=this.C6.Get(0).FrameSize;this.Bx.Set(AW,[B.tP(0,((
A=this.M)[2]-A[0])-Y[0]),-Y[1]]);this.C8.Set(AW,Y);this.CD.Set(AW,0xFFFFFFFF);}break;
case 4:{Y=this.Cu.Get(0).FrameSize;this.Bx.Set(AW,[B.tP(0,((A=this.M)[2]-A[0])-Y[
0]),-Y[1]]);this.C8.Set(AW,Y);this.CD.Set(AW,0xFFFFFFFF);}break;case 3:{Y=this.C6.
Get(0).FrameSize;this.Bx.Set(AW,[B.tP(0,((A=this.M)[2]-A[0])-Y[0]),-Y[1]]);this.
C8.Set(AW,Y);this.CD.Set(AW,255|(255<<8)|(255<<16)|((B.tP(0,255)&0xFF)<<24));this.
DK.Set(AW,((A=B.tP(-5,5))&0x80)?A|0xFFFFFF00:A&0xFF);}break;case 5:{Y=this.Cu.Get(
0).FrameSize;this.Bx.Set(AW,[B.tP(0,((A=this.M)[2]-A[0])-Y[0]),-Y[1]]);this.C8.Set(
AW,Y);this.CD.Set(AW,255|(255<<8)|(255<<16)|((B.tP(0,255)&0xFF)<<24));this.DK.Set(
AW,((A=B.tP(-5,5))&0x80)?A|0xFFFFFF00:A&0xFF);}break;case 6:{Y=this.GY.FrameSize;
this.Bx.Set(AW,[B.tP(0,((A=this.M)[2]-A[0])-Y[0]),-Y[1]]);this.C8.Set(AW,Y);this.
CD.Set(AW,(B.tP(0,255)&0xFF)|((B.tP(0,255)&0xFF)<<8)|((B.tP(0,255)&0xFF)<<16)|(170<<
24));}break;default:;}this.GV.Set(AW,[B.tP(-2,2),B.tP(1,(((A=this.M)[3]-A[1])/40
)|0)]);this.Hf.Set(AW,this.Gg);},_Init:function(aArg){B.Core.Ag._Init.call(this,
aArg);(this.Cu=[]).__proto__=D.BD.Cu;(this.C6=[]).__proto__=D.BD.C6;(this.Bx=[]).
__proto__=D.BD.Bx;(this.GV=[]).__proto__=D.BD.GV;(this.CD=[]).__proto__=D.BD.CD;(
this.C8=[]).__proto__=D.BD.C8;(this.Hf=[]).__proto__=D.BD.Hf;(this.DK=[]).__proto__=
D.BD.DK;this.__proto__=D.BD;this.Ah(MO);this.Cu.Set(0,B.s$(D.Ie));this.Cu.Set(1,
B.s$(D.If));this.Cu.Set(2,B.s$(D.Ig));this.Cu.Set(3,B.s$(D.Ih));this.Cu.Set(4,B.
s$(D.Ii));this.Cu.Set(5,B.s$(D.Ij));this.Cu.Set(6,B.s$(D.Ik));this.C6.Set(0,B.s$(
D.Il));this.C6.Set(1,B.s$(D.Im));this.C6.Set(2,B.s$(D.In));this.C6.Set(3,B.s$(D.
Io));this.C6.Set(4,B.s$(D.Ip));this.GY=B.s$(D.IG);},_Mark:function(E){var A;B.Core.
Ag._Mark.call(this,E);B.ts(this.Cu,E);B.ts(this.C6,E);if((A=this.GY)&&(A._cycle!=
E))A._Mark(A._cycle=E);},_className:"Accelerator::GraphicsFactory"};D.NK={N7:0,N6:
1,N4:2,N5:3,NU:4,NV:5,Ne:6};D.Ie={_class:function(){return B.ui.V;},0:{FileName:
"./res/AcceleratorImgBlend1.png",Format:B.ch,NoOfFrames:1,FrameSize:[160,160],FrameDelay:
0,_this:null}};D.If={_class:function(){return B.ui.V;},0:{FileName:"./res/AcceleratorImgBlend2.png"
,Format:B.ch,NoOfFrames:1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.Ig={_class:
function(){return B.ui.V;},0:{FileName:"./res/AcceleratorImgBlend3.png",Format:B.
ch,NoOfFrames:1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.Ih={_class:function(
){return B.ui.V;},0:{FileName:"./res/AcceleratorImgBlend4.png",Format:B.ch,NoOfFrames:
1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.Ii={_class:function(){return B.
ui.V;},0:{FileName:"./res/AcceleratorImgBlend5.png",Format:B.ch,NoOfFrames:1,FrameSize:[
160,160],FrameDelay:0,_this:null}};D.Ij={_class:function(){return B.ui.V;},0:{FileName:
"./res/AcceleratorImgBlend6.png",Format:B.ch,NoOfFrames:1,FrameSize:[160,160],FrameDelay:
0,_this:null}};D.Ik={_class:function(){return B.ui.V;},0:{FileName:"./res/AcceleratorImgBlend7.png"
,Format:B.ch,NoOfFrames:1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.Il={_class:
function(){return B.ui.V;},0:{FileName:"./res/AcceleratorImgCopy1.jpg",Format:B.
ch,NoOfFrames:1,FrameSize:[160,120],FrameDelay:0,_this:null}};D.Im={_class:function(
){return B.ui.V;},0:{FileName:"./res/AcceleratorImgCopy2.jpg",Format:B.ch,NoOfFrames:
1,FrameSize:[160,120],FrameDelay:0,_this:null}};D.In={_class:function(){return B.
ui.V;},0:{FileName:"./res/AcceleratorImgCopy3.jpg",Format:B.ch,NoOfFrames:1,FrameSize:[
160,120],FrameDelay:0,_this:null}};D.Io={_class:function(){return B.ui.V;},0:{FileName:
"./res/AcceleratorImgCopy4.jpg",Format:B.ch,NoOfFrames:1,FrameSize:[160,120],FrameDelay:
0,_this:null}};D.Ip={_class:function(){return B.ui.V;},0:{FileName:"./res/AcceleratorImgCopy5.jpg"
,Format:B.ch,NoOfFrames:1,FrameSize:[160,120],FrameDelay:0,_this:null}};D.DM={_class:
function(){return B.ui.AT;},0:{Data:function(){return B.uq;},Cache:[],_this:null
}};
D._Init=function(){D.HN.__proto__=B.Core.Ag;D.BD.__proto__=B.Core.Ag;};D.By=function(
E){var A;if((A=D.GZ[0]._this)&&(A._cycle!=E))A._Done(D.GZ[0]._this=null);if((A=D.
IG[0]._this)&&(A._cycle!=E))A._Done(D.IG[0]._this=null);if((A=D.E8[0]._this)&&(A.
_cycle!=E))A._Done(D.E8[0]._this=null);if((A=D.Ie[0]._this)&&(A._cycle!=E))A._Done(
D.Ie[0]._this=null);if((A=D.If[0]._this)&&(A._cycle!=E))A._Done(D.If[0]._this=null
);if((A=D.Ig[0]._this)&&(A._cycle!=E))A._Done(D.Ig[0]._this=null);if((A=D.Ih[0].
_this)&&(A._cycle!=E))A._Done(D.Ih[0]._this=null);if((A=D.Ii[0]._this)&&(A._cycle
!=E))A._Done(D.Ii[0]._this=null);if((A=D.Ij[0]._this)&&(A._cycle!=E))A._Done(D.Ij[
0]._this=null);if((A=D.Ik[0]._this)&&(A._cycle!=E))A._Done(D.Ik[0]._this=null);if((
A=D.Il[0]._this)&&(A._cycle!=E))A._Done(D.Il[0]._this=null);if((A=D.Im[0]._this)&&(
A._cycle!=E))A._Done(D.Im[0]._this=null);if((A=D.In[0]._this)&&(A._cycle!=E))A._Done(
D.In[0]._this=null);if((A=D.Io[0]._this)&&(A._cycle!=E))A._Done(D.Io[0]._this=null
);if((A=D.Ip[0]._this)&&(A._cycle!=E))A._Done(D.Ip[0]._this=null);if((A=D.DM[0].
_this)&&(A._cycle!=E))A._Done(D.DM[0]._this=null);};return D;})();

/* Embedded Wizard */