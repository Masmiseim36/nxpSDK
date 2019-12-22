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
);if(EmWiApp.tj)throw new Error("The unit file 'Accelerator.js' included twice!"
);EmWiApp.tj=(function(){var B=EmWiApp;var D={};
var Az=" fps";var A3=" %%";var BY="-";var CI="Rectangle Fill";var CJ="Rectangle Blend";
var Du="RGB565 Bitmap";var Dv="Native Bitmap";var IJ="RGB565 Bitmap + Opacity";var
IK="Native Bitmap + Opacity";var IL="Alpha8 Bitmap";var G4=[0,0,480,272];var Mc=[
0,40,360,272];var Md=[375,5,475,25];var Me="Framerate";var Mf=[370,25,470,65];var
Mg="0 fps";var Mh=[375,65,475,85];var Mi="CPU Load";var Mj=[370,85,470,125];var Mk=
"0 %%";var Ml=[375,200,475,220];var Mm="Scenario";var Mn=[375,120,470,160];var Mo=
"Graphics Accelerator";var Mp=[10,0,310,36];var Mq="Alpha8 Blend";var Mr=[370,225
,470,265];var Ms="Next";var Mt=[385,150,455,210];var F0=[0,0];var Mu=[0,0,400,240
];
D.GC={HN:null,C5:null,BC:null,E8:null,DI:null,Dp:null,Dn:null,Ch:null,DQ:null,C$:
null,Cc:null,Ck:null,Er:null,Cl:null,K2:16,K1:8,KZ:8,KX:8,K0:8,KY:8,KW:8,IW:5,G3:
function(aSize){this.Io(this);},BF:function(aArg){this.C$.FQ(false);this.Cl.FQ(false
);this.Dn.FQ(false);this.Ch.FQ(false);this.Ez(this.Ck);},L8:function(AT){var Ge=
this.HN.KO();if(!!this.D4())this.Dp.Bu(this.D4().GetFPS().toFixed()+Az);if((Ge>=
0)&&(Ge<=100)){this.Ch.Bu(Ge.toFixed()+A3);var BH=((((Ge*255)/100)|0)&0xFF)|((((((
100-Ge)*255)/100)|0)&0xFF)<<8)|(0<<16)|(255<<24);this.Ch.BP(BH);}else{this.Ch.Bu(
BY);this.Ch.BP(0xFFFFFFFF);}},Io:function(AT){var EX=this.IW+1;while(EX!==this.IW
){if(this.Ky(EX))this.IW=EX;else EX=EX+1;if(EX>6)EX=0;}},Ky:function(AU){var Eq=
6;var Cn=0;if((AU<0)||(AU>6))return false;switch(AU){case 0:{Cn=this.K2;if(Cn<=0
)return false;Eq=0;this.Cc.Bu(CI);}break;case 1:{Cn=this.K1;if(Cn<=0)return false;
Eq=1;this.Cc.Bu(CJ);}break;case 2:{Cn=this.KZ;if(Cn<=0)return false;Eq=2;this.Cc.
Bu(Du);}break;case 3:{Cn=this.KX;if(Cn<=0)return false;Eq=4;this.Cc.Bu(Dv);}break;
case 4:{Cn=this.K0;if(Cn<=0)return false;Eq=3;this.Cc.Bu(IJ);}break;case 5:{Cn=this.
KY;if(Cn<=0)return false;Eq=5;this.Cc.Bu(IK);}break;default:{Cn=this.KW;if(Cn<=0
)return false;Eq=6;this.Cc.Bu(IL);}}this.BC.FY=Eq;this.BC.L2(Cn);return true;},_Init:
function(aArg){B.Core.Aj._Init.call(this,aArg);B.tf.C5._Init.call(this.C5={Ae:this
},0);D.BC._Init.call(this.BC={Ae:this},0);B.Core.Timer._Init.call(this.E8={Ae:this
},0);B.tf.Text._Init.call(this.DI={Ae:this},0);B.tf.Text._Init.call(this.Dp={Ae:
this},0);B.tf.Text._Init.call(this.Dn={Ae:this},0);B.tf.Text._Init.call(this.Ch={
Ae:this},0);B.tf.Text._Init.call(this.DQ={Ae:this},0);B.tf.Text._Init.call(this.
C$={Ae:this},0);B.tf.Text._Init.call(this.Cc={Ae:this},0);B.tg.Ck._Init.call(this.
Ck={Ae:this},0);B.Core.Timer._Init.call(this.Er={Ae:this},0);B.tg.Cl._Init.call(
this.Cl={Ae:this},0);this.__proto__=D.GC;var A;this.Ai(G4);this.C5.Ai(G4);this.C5.
BP(0xFF200808);this.BC.Ai(Mc);this.E8.Bn(true);this.DI.Ai(Md);this.DI.Cw(0x11);this.
DI.Bu(Me);this.Dp.Ai(Mf);this.Dp.Cw(0x12);this.Dp.Bu(Mg);this.Dn.Ai(Mh);this.Dn.
Cw(0x11);this.Dn.Bu(Mi);this.Ch.Ai(Mj);this.Ch.Cw(0x12);this.Ch.Bu(Mk);this.Ch.BP(
0xFFFFFFFF);this.DQ.Ai(Ml);this.DQ.Cw(0x11);this.DQ.Bu(Mm);this.C$.Ai(Mn);this.C$.
Ix(true);this.C$.Cw(0x11);this.C$.Bu(Mo);this.Cc.Ai(Mp);this.Cc.Cw(0x11);this.Cc.
Bu(Mq);this.Ck.Ai(Mr);this.Ck.Iu(Ms);this.Er.FP(5000);this.Er.Ey(0);this.Cl.Ai(Mt
);this.Cl.Iu(B.g3);this.AZ(this.C5,0);this.AZ(this.BC,0);this.AZ(this.DI,0);this.
AZ(this.Dp,0);this.AZ(this.Dn,0);this.AZ(this.Ch,0);this.AZ(this.DQ,0);this.AZ(this.
C$,0);this.AZ(this.Cc,0);this.AZ(this.Ck,0);this.AZ(this.Cl,0);this.E8.EB=[this,
this.L8];this.DI.Dd(B.r0(D.E1));this.Dp.Dd(B.r0(D.GM));this.Dn.Dd(B.r0(D.E1));this.
Ch.Dd(B.r0(D.GM));this.DQ.Dd(B.r0(D.E1));this.C$.Dd(B.r0(D.E1));this.Cc.Dd(B.r0(
D.DH));this.Ck.Ex=[this,this.Io];this.Ck.Ir(B._GetAutoObject(B.tg.Iz));this.Er.EB=[
this,this.Io];this.HN=B._GetAutoObject(B.GraphicsSystem.Device);this.Cl.L4([A=B.
_GetAutoObject(B.GraphicsSystem.Device),A.JR,A.GU]);this.Cl.Ir(B._GetAutoObject(
B.tg.IE));this.BF(aArg);},_Done:function(){this.__proto__=D.GC;this.C5._Done();this.
BC._Done();this.E8._Done();this.DI._Done();this.Dp._Done();this.Dn._Done();this.
Ch._Done();this.DQ._Done();this.C$._Done();this.Cc._Done();this.Ck._Done();this.
Er._Done();this.Cl._Done();B.Core.Aj._Done.call(this);},_ReInit:function(){B.Core.
Aj._ReInit.call(this);this.C5._ReInit();this.BC._ReInit();this.E8._ReInit();this.
DI._ReInit();this.Dp._ReInit();this.Dn._ReInit();this.Ch._ReInit();this.DQ._ReInit(
);this.C$._ReInit();this.Cc._ReInit();this.Ck._ReInit();this.Er._ReInit();this.Cl.
_ReInit();},_Mark:function(E){var A;B.Core.Aj._Mark.call(this,E);if((A=this.HN)&&(
A._cycle!=E))A._Mark(A._cycle=E);if((A=this.C5)._cycle!=E)A._Mark(A._cycle=E);if((
A=this.BC)._cycle!=E)A._Mark(A._cycle=E);if((A=this.E8)._cycle!=E)A._Mark(A._cycle=
E);if((A=this.DI)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Dp)._cycle!=E)A._Mark(
A._cycle=E);if((A=this.Dn)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Ch)._cycle!=
E)A._Mark(A._cycle=E);if((A=this.DQ)._cycle!=E)A._Mark(A._cycle=E);if((A=this.C$
)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Cc)._cycle!=E)A._Mark(A._cycle=E);if((
A=this.Ck)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Er)._cycle!=E)A._Mark(A._cycle=
E);if((A=this.Cl)._cycle!=E)A._Mark(A._cycle=E);},_className:"Accelerator::Accelerator"
};D.GM={_class:function(){return B.te.AR;},0:{Data:function(){return B.tl;},Cache:[
],_this:null}};D.Im={_class:function(){return B.te.W;},0:{FileName:"./res/AcceleratorLogo.png"
,Format:B.kC,NoOfFrames:1,FrameSize:[330,80],FrameDelay:0,_this:null}};D.E1={_class:
function(){return B.te.AR;},0:{Data:function(){return B.tn;},Cache:[],_this:null
}};D.BC={Cv:B.sr(7,null,null),C4:B.sr(5,null,null),GL:null,Bv:B.sr(64,B.pm,null)
,FN:1,GJ:B.sr(64,B.pm,null),CB:B.sr(64,0,null),C6:B.sr(64,B.pm,null),G2:B.sr(64,
0,null),FY:6,DG:B.sr(64,0,null),Ci:function(Ar,aClip,aOffset,AD,aBlend){var A;var
Af=0;B.Core.Aj.Ci.call(this,Ar,aClip,aOffset,AD,aBlend);aOffset=B.so(aOffset,this.
N.slice(0,2));while(Af<64){var bmp;var Av=B.so(this.Bv.Get(Af),aOffset);var Z=this.
C6.Get(Af);var A0=this.CB.Get(Af);if(this.Bv.Get(Af)[1]<=((A=this.N)[3]-A[1])){switch(
this.G2.Get(Af)){case 0:{Ar.Fx(aClip,[].concat(Av,B.so(Av,Z)),A0,A0,A0,A0,aBlend
);}break;case 1:{Ar.Fx(aClip,[].concat(Av,B.so(Av,Z)),A0,A0,A0,A0,aBlend);}break;
case 2:{bmp=this.C4.Get(Af%5);Ar.Es(aClip,bmp,0,[].concat(Av,B.so(Av,Z)),F0,A0,A0
,A0,A0,aBlend);}break;case 4:{bmp=this.Cv.Get(Af%7);Ar.Es(aClip,bmp,0,[].concat(
Av,B.so(Av,Z)),F0,A0,A0,A0,A0,aBlend);}break;case 3:{bmp=this.C4.Get(Af%5);Ar.Es(
aClip,bmp,0,[].concat(Av,B.so(Av,Z)),F0,A0,A0,A0,A0,aBlend);}break;case 5:{bmp=this.
Cv.Get(Af%7);Ar.Es(aClip,bmp,0,[].concat(Av,B.so(Av,Z)),F0,A0,A0,A0,A0,aBlend);}
break;case 6:{bmp=this.GL;Ar.Es(aClip,bmp,0,[].concat(Av,B.so(Av,Z)),F0,A0,A0,A0
,A0,aBlend);}break;default:;}}Af=Af+1;}B.r6([this,this.J1],this);},G3:function(aSize
){var A;var Af;for(Af=0;Af<this.FN;Af=Af+1){this.IA(Af);this.Bv.Set(Af,[this.Bv.
Get(Af)[0],B.sJ(0,((A=this.N)[3]-A[1])-this.C6.Get(Af)[1])]);}B.r6([this,this.J1
],this);},L2:function(C){var Af;if(C<0)C=0;if(C>64)C=64;if(this.FN===C)return;for(
Af=this.FN;Af<C;Af=Af+1)this.IA(Af);this.FN=C;},J1:function(AT){var A;this.An([0
,0,(A=this.N)[2]-A[0],A[3]-A[1]]);var Af=0;while(Af<64){var Z=this.C6.Get(Af);if(
this.Bv.Get(Af)[1]<=((A=this.N)[3]-A[1])){this.Bv.Set(Af,B.so(this.Bv.Get(Af),this.
GJ.Get(Af)));if(((this.Bv.Get(Af)[0]+Z[0])<0)||(this.Bv.Get(Af)[0]>((A=this.N)[2
]-A[0])))this.Bv.Set(Af,[this.Bv.Get(Af)[0],((A=this.N)[3]-A[1])+1]);}if((this.Bv.
Get(Af)[1]>((A=this.N)[3]-A[1]))&&(Af<this.FN))this.IA(Af);if((this.FY===3)||(this.
FY===5)){var Gb=((this.CB.Get(Af)>>24)&0xFF)+this.DG.Get(Af);if(Gb>255){Gb=255;this.
DG.Set(Af,-this.DG.Get(Af));}if(Gb<0){Gb=0;this.DG.Set(Af,-this.DG.Get(Af));}this.
CB.Set(Af,(this.CB.Get(Af)&0x00FFFFFF)|((Gb&0xFF)<<24));}Af=Af+1;}},IA:function(
AU){var A;var Z;switch(this.FY){case 0:{Z=[B.sJ((((A=this.N)[2]-A[0])/3)|0,((2*((
A=this.N)[2]-A[0]))/3)|0),B.sJ((((A=this.N)[3]-A[1])/3)|0,((2*((A=this.N)[3]-A[1
]))/3)|0)];this.Bv.Set(AU,[B.sJ(0,((A=this.N)[2]-A[0])-Z[0]),-Z[1]]);this.C6.Set(
AU,Z);this.CB.Set(AU,(B.sJ(0,255)&0xFF)|((B.sJ(0,255)&0xFF)<<8)|((B.sJ(0,255)&0xFF
)<<16)|(255<<24));}break;case 1:{Z=[B.sJ((((A=this.N)[2]-A[0])/3)|0,((2*((A=this.
N)[2]-A[0]))/3)|0),B.sJ((((A=this.N)[3]-A[1])/3)|0,((2*((A=this.N)[3]-A[1]))/3)|
0)];this.Bv.Set(AU,[B.sJ(0,((A=this.N)[2]-A[0])-Z[0]),-Z[1]]);this.C6.Set(AU,Z);
this.CB.Set(AU,(B.sJ(0,255)&0xFF)|((B.sJ(0,255)&0xFF)<<8)|((B.sJ(0,255)&0xFF)<<16
)|(170<<24));}break;case 2:{Z=this.C4.Get(0).FrameSize;this.Bv.Set(AU,[B.sJ(0,((
A=this.N)[2]-A[0])-Z[0]),-Z[1]]);this.C6.Set(AU,Z);this.CB.Set(AU,0xFFFFFFFF);}break;
case 4:{Z=this.Cv.Get(0).FrameSize;this.Bv.Set(AU,[B.sJ(0,((A=this.N)[2]-A[0])-Z[
0]),-Z[1]]);this.C6.Set(AU,Z);this.CB.Set(AU,0xFFFFFFFF);}break;case 3:{Z=this.C4.
Get(0).FrameSize;this.Bv.Set(AU,[B.sJ(0,((A=this.N)[2]-A[0])-Z[0]),-Z[1]]);this.
C6.Set(AU,Z);this.CB.Set(AU,255|(255<<8)|(255<<16)|((B.sJ(0,255)&0xFF)<<24));this.
DG.Set(AU,((A=B.sJ(-5,5))&0x80)?A|0xFFFFFF00:A&0xFF);}break;case 5:{Z=this.Cv.Get(
0).FrameSize;this.Bv.Set(AU,[B.sJ(0,((A=this.N)[2]-A[0])-Z[0]),-Z[1]]);this.C6.Set(
AU,Z);this.CB.Set(AU,255|(255<<8)|(255<<16)|((B.sJ(0,255)&0xFF)<<24));this.DG.Set(
AU,((A=B.sJ(-5,5))&0x80)?A|0xFFFFFF00:A&0xFF);}break;case 6:{Z=this.GL.FrameSize;
this.Bv.Set(AU,[B.sJ(0,((A=this.N)[2]-A[0])-Z[0]),-Z[1]]);this.C6.Set(AU,Z);this.
CB.Set(AU,(B.sJ(0,255)&0xFF)|((B.sJ(0,255)&0xFF)<<8)|((B.sJ(0,255)&0xFF)<<16)|(170<<
24));}break;default:;}this.GJ.Set(AU,[B.sJ(-2,2),B.sJ(1,(((A=this.N)[3]-A[1])/40
)|0)]);this.G2.Set(AU,this.FY);},_Init:function(aArg){B.Core.Aj._Init.call(this,
aArg);(this.Cv=[]).__proto__=D.BC.Cv;(this.C4=[]).__proto__=D.BC.C4;(this.Bv=[]).
__proto__=D.BC.Bv;(this.GJ=[]).__proto__=D.BC.GJ;(this.CB=[]).__proto__=D.BC.CB;(
this.C6=[]).__proto__=D.BC.C6;(this.G2=[]).__proto__=D.BC.G2;(this.DG=[]).__proto__=
D.BC.DG;this.__proto__=D.BC;this.Ai(Mu);this.Cv.Set(0,B.r0(D.HY));this.Cv.Set(1,
B.r0(D.HZ));this.Cv.Set(2,B.r0(D.H0));this.Cv.Set(3,B.r0(D.H1));this.Cv.Set(4,B.
r0(D.H2));this.Cv.Set(5,B.r0(D.H3));this.Cv.Set(6,B.r0(D.H4));this.C4.Set(0,B.r0(
D.H5));this.C4.Set(1,B.r0(D.H6));this.C4.Set(2,B.r0(D.H7));this.C4.Set(3,B.r0(D.
H8));this.C4.Set(4,B.r0(D.H9));this.GL=B.r0(D.Im);},_Mark:function(E){var A;B.Core.
Aj._Mark.call(this,E);B.sj(this.Cv,E);B.sj(this.C4,E);if((A=this.GL)&&(A._cycle!=
E))A._Mark(A._cycle=E);},_className:"Accelerator::GraphicsFactory"};D.Nm={NH:0,NG:
1,NE:2,NF:3,Nu:4,Nv:5,MV:6};D.HY={_class:function(){return B.te.W;},0:{FileName:
"./res/AcceleratorImgBlend1.png",Format:B.b9,NoOfFrames:1,FrameSize:[160,160],FrameDelay:
0,_this:null}};D.HZ={_class:function(){return B.te.W;},0:{FileName:"./res/AcceleratorImgBlend2.png"
,Format:B.b9,NoOfFrames:1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.H0={_class:
function(){return B.te.W;},0:{FileName:"./res/AcceleratorImgBlend3.png",Format:B.
b9,NoOfFrames:1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.H1={_class:function(
){return B.te.W;},0:{FileName:"./res/AcceleratorImgBlend4.png",Format:B.b9,NoOfFrames:
1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.H2={_class:function(){return B.
te.W;},0:{FileName:"./res/AcceleratorImgBlend5.png",Format:B.b9,NoOfFrames:1,FrameSize:[
160,160],FrameDelay:0,_this:null}};D.H3={_class:function(){return B.te.W;},0:{FileName:
"./res/AcceleratorImgBlend6.png",Format:B.b9,NoOfFrames:1,FrameSize:[160,160],FrameDelay:
0,_this:null}};D.H4={_class:function(){return B.te.W;},0:{FileName:"./res/AcceleratorImgBlend7.png"
,Format:B.b9,NoOfFrames:1,FrameSize:[160,160],FrameDelay:0,_this:null}};D.H5={_class:
function(){return B.te.W;},0:{FileName:"./res/AcceleratorImgCopy1.jpg",Format:B.
b9,NoOfFrames:1,FrameSize:[160,120],FrameDelay:0,_this:null}};D.H6={_class:function(
){return B.te.W;},0:{FileName:"./res/AcceleratorImgCopy2.jpg",Format:B.b9,NoOfFrames:
1,FrameSize:[160,120],FrameDelay:0,_this:null}};D.H7={_class:function(){return B.
te.W;},0:{FileName:"./res/AcceleratorImgCopy3.jpg",Format:B.b9,NoOfFrames:1,FrameSize:[
160,120],FrameDelay:0,_this:null}};D.H8={_class:function(){return B.te.W;},0:{FileName:
"./res/AcceleratorImgCopy4.jpg",Format:B.b9,NoOfFrames:1,FrameSize:[160,120],FrameDelay:
0,_this:null}};D.H9={_class:function(){return B.te.W;},0:{FileName:"./res/AcceleratorImgCopy5.jpg"
,Format:B.b9,NoOfFrames:1,FrameSize:[160,120],FrameDelay:0,_this:null}};D.DH={_class:
function(){return B.te.AR;},0:{Data:function(){return B.tm;},Cache:[],_this:null
}};
D._Init=function(){D.GC.__proto__=B.Core.Aj;D.BC.__proto__=B.Core.Aj;};D.Bw=function(
E){var A;if((A=D.GM[0]._this)&&(A._cycle!=E))A._Done(D.GM[0]._this=null);if((A=D.
Im[0]._this)&&(A._cycle!=E))A._Done(D.Im[0]._this=null);if((A=D.E1[0]._this)&&(A.
_cycle!=E))A._Done(D.E1[0]._this=null);if((A=D.HY[0]._this)&&(A._cycle!=E))A._Done(
D.HY[0]._this=null);if((A=D.HZ[0]._this)&&(A._cycle!=E))A._Done(D.HZ[0]._this=null
);if((A=D.H0[0]._this)&&(A._cycle!=E))A._Done(D.H0[0]._this=null);if((A=D.H1[0].
_this)&&(A._cycle!=E))A._Done(D.H1[0]._this=null);if((A=D.H2[0]._this)&&(A._cycle
!=E))A._Done(D.H2[0]._this=null);if((A=D.H3[0]._this)&&(A._cycle!=E))A._Done(D.H3[
0]._this=null);if((A=D.H4[0]._this)&&(A._cycle!=E))A._Done(D.H4[0]._this=null);if((
A=D.H5[0]._this)&&(A._cycle!=E))A._Done(D.H5[0]._this=null);if((A=D.H6[0]._this)&&(
A._cycle!=E))A._Done(D.H6[0]._this=null);if((A=D.H7[0]._this)&&(A._cycle!=E))A._Done(
D.H7[0]._this=null);if((A=D.H8[0]._this)&&(A._cycle!=E))A._Done(D.H8[0]._this=null
);if((A=D.H9[0]._this)&&(A._cycle!=E))A._Done(D.H9[0]._this=null);if((A=D.DH[0].
_this)&&(A._cycle!=E))A._Done(D.DH[0]._this=null);};return D;})();

/* Embedded Wizard */