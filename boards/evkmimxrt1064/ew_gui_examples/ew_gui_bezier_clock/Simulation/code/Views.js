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
);if(EmWiApp.uj)throw new Error("The unit file 'Views.js' included twice!");EmWiApp.
uj=(function(){var B=EmWiApp;var E={};
var AJ=[1,1];var Be=[0,0];var Dh=[0,0,0,0];var CK="\uFEFF";
E.Cr={Ic:null,M3:1,M2:1,NX:0xFFFFFFFF,NW:0xFFFFFFFF,Da:function(AA,aClip,aOffset,
Aj,aBlend){var A;var Hm=this.M2;var Hn=this.M3;var EF=B.tx(this.BM,aOffset);var EG=
B.tx(this.Bp,aOffset);var ID=this.NW;var LM=this.NX;if(B.tl(EF,EG))return;aBlend=
aBlend&&((this.F&0x2)===0x2);Aj=Aj+1;if(Aj<256){ID=(ID&0x00FFFFFF)|((((Aj*((ID>>
24)&0xFF))>>8)&0xFF)<<24);LM=(LM&0x00FFFFFF)|((((Aj*((LM>>24)&0xFF))>>8)&0xFF)<<
24);}if((Hm===1)&&(Hn===1))AA.Mo(aClip,EF,EG,ID,LM,aBlend);else AA.Sq(aClip,EF,EG
,Hm,Hn,ID,LM,aBlend);},GetExtent:function(){var Hm=this.M2;var Hn=this.M3;var EF=
this.BM;var EG=this.Bp;if(((Hm===1)&&(Hn===1))||B.tl(EF,EG))return B.Core.J$.GetExtent.
call(this);var HX=Hm/2.000000;var HY=Hn/2.000000;var Jz=EF[0];var JA=EF[1];var JB=
EG[0];var JC=EG[1];var Eb=JB-Jz;var Ec=JC-JA;var Cy=Math.sqrt((Eb*Eb)+(Ec*Ec));Eb=
Eb/Cy;Ec=Ec/Cy;var Rt=Jz+(Ec*HX);var Ru=JA-(Eb*HX);var M$=JB+(Ec*HY);var Na=JC-(
Eb*HY);var Nb=JB-(Ec*HY);var Nc=JC+(Eb*HY);var Nd=Jz-(Ec*HX);var Ne=JA+(Eb*HX);var
left=Rt;var right=Rt;var top=Ru;var bottom=Ru;if(M$<left)left=M$;if(Nb<left)left=
Nb;if(Nd<left)left=Nd;if(M$>right)right=M$;if(Nb>right)right=Nb;if(Nd>right)right=
Nd;if(Na<top)top=Na;if(Nc<top)top=Nc;if(Ne<top)top=Ne;if(Na>bottom)bottom=Na;if(
Nc>bottom)bottom=Nc;if(Ne>bottom)bottom=Ne;var N=[left|0,top|0,right|0,bottom|0];
N=[].concat(N.slice(0,2),B.tx(N.slice(2,4),AJ));return N;},IL:function(Af){var A;
if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());},CI:function(C){var
A;if(C<1)C=1;if((C===this.M2)&&(C===this.M3))return;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ai(this.GetExtent());this.M2=C;this.M3=C;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ai(this.GetExtent());if((C!==1)&&!this.Ic){this.Ic=B.s$(B.ui.M1);if(
this.Ic.Ij)B.sz([this,this.IL],this.Ic,0);}if((C===1)&&!!this.Ic){if(this.Ic.Ij)
B.sM([this,this.IL],this.Ic,0);this.Ic=null;}},Aa:function(C){var A;if((C===this.
NW)&&(C===this.NX))return;this.NW=C;this.NX=C;if(!!this.G&&((this.F&0x1)===0x1))
this.G.Ai(this.GetExtent());},_Init:function(aArg){B.Core.J$._Init.call(this,aArg
);this.__proto__=E.Cr;},_Mark:function(D){var A;B.Core.J$._Mark.call(this,D);if((
A=this.Ic)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Line"};E.Fn={
BP:0xFFFFFFFF,BQ:0xFFFFFFFF,BR:0xFFFFFFFF,BC:0xFFFFFFFF,Da:function(AA,aClip,aOffset
,Aj,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Aj=Aj+1;if(Aj<256){var A2=
this.BC;var A3=this.BR;var A0=this.BP;var A1=this.BQ;A2=(A2&0x00FFFFFF)|((((Aj*((
A2>>24)&0xFF))>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((Aj*((A3>>24)&0xFF))>>8)&0xFF
)<<24);A0=(A0&0x00FFFFFF)|((((Aj*((A0>>24)&0xFF))>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF
)|((((Aj*((A1>>24)&0xFF))>>8)&0xFF)<<24);AA.Mr(aClip,B.tz(this.H,aOffset),A2,A3,
A1,A0,aBlend);}else AA.Mr(aClip,B.tz(this.H,aOffset),this.BC,this.BR,this.BQ,this.
BP,aBlend);},Aa:function(C){var A;if((((C===this.BC)&&(C===this.BR))&&(C===this.
BP))&&(C===this.BQ))return;this.BC=C;this.BR=C;this.BP=C;this.BQ=C;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ai(this.H);},_Init:function(aArg){B.Core.AQ._Init.call(
this,aArg);this.__proto__=E.Fn;},_className:"Views::Rectangle"};E.EV={Width:1,BP:
0xFFFFFFFF,BQ:0xFFFFFFFF,BR:0xFFFFFFFF,BC:0xFFFFFFFF,Da:function(AA,aClip,aOffset
,Aj,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Aj=Aj+1;if(Aj<256){var A2=
this.BC;var A3=this.BR;var A0=this.BP;var A1=this.BQ;A2=(A2&0x00FFFFFF)|((((Aj*((
A2>>24)&0xFF))>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((Aj*((A3>>24)&0xFF))>>8)&0xFF
)<<24);A0=(A0&0x00FFFFFF)|((((Aj*((A0>>24)&0xFF))>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF
)|((((Aj*((A1>>24)&0xFF))>>8)&0xFF)<<24);AA.Qj(aClip,B.tz(this.H,aOffset),this.Width
,A2,A3,A1,A0,aBlend);}else AA.Qj(aClip,B.tz(this.H,aOffset),this.Width,this.BC,this.
BR,this.BQ,this.BP,aBlend);},CI:function(C){var A;if(C<0)C=0;if(C===this.Width)return;
this.Width=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},Kb:function(){
return this.BC;},Aa:function(C){var A;if((((C===this.BC)&&(C===this.BR))&&(C===this.
BP))&&(C===this.BQ))return;this.BC=C;this.BR=C;this.BP=C;this.BQ=C;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ai(this.H);},_Init:function(aArg){B.Core.AQ._Init.call(
this,aArg);this.__proto__=E.EV;},_className:"Views::Border"};E.Mt={timer:null,An:
null,GX:null,C5:-1,BP:0xFFFFFFFF,BQ:0xFFFFFFFF,BR:0xFFFFFFFF,BC:0xFFFFFFFF,GU:0x1F
,Hs:0,Dd:255,Bm:0,OE:B.qx,Fe:false,Dn:true,Da:function(AA,aClip,aOffset,Aj,aBlend
){var A;var Cw=this.Hs;if(this.C5>=0)Cw=this.C5;if(!this.An||(Cw>=this.An.NoOfFrames
))return;this.An.Update();var A2=this.BC;var A3=this.BR;var A1=this.BQ;var A0=this.
BP;var Ej=(((Aj+1)*this.Dd)>>8)+1;var DD=this.GU;var N=B.tz(this.H,aOffset);var NS=
B.tw([N[2]-N[0],N[3]-N[1]],this.OE);aBlend=aBlend&&((this.F&0x2)===0x2);if(Ej<256
){A2=(A2&0x00FFFFFF)|((((((A2>>24)&0xFF)*Ej)>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((((
A3>>24)&0xFF)*Ej)>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*Ej)>>8)&
0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*Ej)>>8)&0xFF)<<24);}if(((this.
OE[0]>0)&&(NS[0]>0))&&!((DD&0x5)===0x5)){var CR=((this.An.FrameSize[0]/3)|0)-NS[
0];if(((DD&0x1)===0x1)){if(aClip[2]>N[2])aClip=B.t1(aClip,N[2]);if(CR>0)N=B.t1(N
,N[2]+CR);DD=DD|0x4;}else if(((DD&0x4)===0x4)){if(aClip[0]<N[0])aClip=[].concat(
N[0],aClip.slice(1,4));if(CR>0)N=[].concat(N[0]-CR,N.slice(1,4));DD=DD|0x1;}else{
if(aClip[0]<N[0])aClip=[].concat(N[0],aClip.slice(1,4));if(aClip[2]>N[2])aClip=B.
t1(aClip,N[2]);if(CR>0){N=[].concat(N[0]-((CR/2)|0),N.slice(1,4));N=B.t1(N,(N[2]+
CR)-((CR/2)|0));}DD=DD|0x5;}}if(((this.OE[1]>0)&&(NS[1]>0))&&!((DD&0xA)===0xA)){
var CR=((this.An.FrameSize[1]/3)|0)-NS[1];if(((DD&0x2)===0x2)){if(aClip[3]>N[3])
aClip=[].concat(aClip.slice(0,3),N[3]);if(CR>0)N=[].concat(N.slice(0,3),N[3]+CR);
DD=DD|0x8;}else if(((DD&0x8)===0x8)){if(aClip[1]<N[1])aClip=B.t3(aClip,N[1]);if(
CR>0)N=B.t3(N,N[1]-CR);DD=DD|0x2;}else{if(aClip[1]<N[1])aClip=B.t3(aClip,N[1]);if(
aClip[3]>N[3])aClip=[].concat(aClip.slice(0,3),N[3]);if(CR>0){N=B.t3(N,N[1]-((CR
/2)|0));N=[].concat(N.slice(0,3),(N[3]+CR)-((CR/2)|0));}DD=DD|0xA;}}AA.Sp(aClip,
this.An,Cw,N,DD,A2,A3,A1,A0,aBlend);},IL:function(Af){var A;if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ai(this.H);},EH:function(Af){var A;var Cw=this.C5;var ES=0;
if(!!this.An)ES=this.An.NoOfFrames*this.An.FrameDelay;if((!!this.timer&&(this.C5<
0))&&(ES>0))this.Bm=this.timer.Ah-(this.Hs*this.An.FrameDelay);if(!!this.timer&&(
ES>0)){var Au=(this.timer.Ah-this.Bm)|0;Cw=(Au/this.An.FrameDelay)|0;if(Au>=ES){
Cw=Cw%this.An.NoOfFrames;this.Bm=this.timer.Ah-(Au%ES);if(!this.Dn){Cw=this.An.NoOfFrames-
1;ES=0;}}}if(((Cw!==this.C5)&&!!this.G)&&((this.F&0x1)===0x1))this.G.Ai(this.H);
this.C5=Cw;if(!ES&&!!this.timer){B.sM([this,this.EH],this.timer,0);this.timer=null;(
A=this.GX)?A[1].call(A[0],this):null;}},Aa:function(C){var A;if((((C===this.BC)&&(
C===this.BR))&&(C===this.BP))&&(C===this.BQ))return;this.BC=C;this.BR=C;this.BP=
C;this.BQ=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},Hv:function(C){
var A;if(this.Fe===C)return;this.Fe=C;this.C5=-1;if(!C&&!!this.timer){B.sM([this
,this.EH],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(B.uk.
J4);B.sz([this,this.EH],this.timer,0);B.lq([this,this.EH],this);}if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ai(this.H);},Fm:function(C){var A;if(C<0)C=0;if((C===this.
Hs)&&(this.C5===-1))return;this.Hs=C;if(!this.timer)this.C5=-1;if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ai(this.H);},Dq:function(C){var A;if(C===this.An)return;if(
!!this.An&&this.An.Ij)B.sM([this,this.IL],this.An,0);this.An=C;this.C5=-1;if(!!C&&
C.Ij)B.sz([this,this.IL],C,0);if(this.Fe){this.Hv(false);this.Hv(true);}if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},_Init:function(aArg){B.Core.AQ._Init.
call(this,aArg);this.__proto__=E.Mt;},_Mark:function(D){var A;B.Core.AQ._Mark.call(
this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.An)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.GX)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Views::Frame"};E.FQ={timer:null,An:null,GX:null,Bm:0,C5:0,BP:0xFFFFFFFF
,BQ:0xFFFFFFFF,BR:0xFFFFFFFF,BC:0xFFFFFFFF,AW:B.qx,Fd:0x12,Hs:0,Dd:255,Hy:0,Fe:false
,Dn:true,JZ:false,G2:true,Da:function(AA,aClip,aOffset,Aj,aBlend){var A;var Cw=this.
Hs;if(this.C5>=0)Cw=this.C5;if(!this.An||(Cw>=this.An.NoOfFrames))return;this.An.
Update();var R=this.J6();var size=this.An.FrameSize;var orient=this.Hy;if((R[0]>=
R[2])||(R[1]>=R[3]))return;var A2=this.BC;var A3=this.BR;var A1=this.BQ;var A0=this.
BP;var Ej=(((Aj+1)*this.Dd)>>8)+1;aBlend=aBlend&&((this.F&0x2)===0x2);if(Ej<256){
A2=(A2&0x00FFFFFF)|((((((A2>>24)&0xFF)*Ej)>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((((
A3>>24)&0xFF)*Ej)>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*Ej)>>8)&
0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*Ej)>>8)&0xFF)<<24);}if(B.tl([R[
2]-R[0],R[3]-R[1]],size)&&!orient)AA.Mm(aClip,this.An,Cw,B.tz(this.H,aOffset),B.
tw(this.H.slice(0,2),R.slice(0,2)),A2,A3,A1,A0,aBlend);else if(!orient)AA.T9(aClip
,this.An,Cw,B.tz(R,aOffset),[].concat(Be,size),A2,A3,A1,A0,aBlend,this.G2);else{
var left=R[0]+aOffset[0];var top=R[1]+aOffset[1];var right=R[2]+aOffset[0];var bottom=
R[3]+aOffset[1];if(orient===1)AA.O0(aClip,this.An,Cw,left,bottom,1.000000,left,top
,1.000000,right,top,1.000000,right,bottom,1.000000,[].concat(Be,size),A0,A2,A3,A1
,aBlend,this.G2);else if(orient===2)AA.O0(aClip,this.An,Cw,right,bottom,1.000000
,left,bottom,1.000000,left,top,1.000000,right,top,1.000000,[].concat(Be,size),A1
,A0,A2,A3,aBlend,this.G2);else if(orient===3)AA.O0(aClip,this.An,Cw,right,top,1.000000
,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000,[].concat(Be,size)
,A3,A1,A0,A2,aBlend,this.G2);}},IL:function(Af){var A;if(((this.JZ&&!!this.An)&&(
this.An.FrameSize[0]>0))&&(this.An.FrameSize[1]>0))this.P(B.ty(this.J6(),this.AW
));if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},EH:function(Af){var A;var
Cw=this.C5;var ES=0;if(!!this.An)ES=this.An.NoOfFrames*this.An.FrameDelay;if((!!
this.timer&&(this.C5<0))&&(ES>0))this.Bm=this.timer.Ah-(this.Hs*this.An.FrameDelay
);if(!!this.timer&&(ES>0)){var Au=(this.timer.Ah-this.Bm)|0;Cw=(Au/this.An.FrameDelay
)|0;if(Au>=ES){Cw=Cw%this.An.NoOfFrames;this.Bm=this.timer.Ah-(Au%ES);if(!this.Dn
){Cw=this.An.NoOfFrames-1;ES=0;}}}if(((Cw!==this.C5)&&!!this.G)&&((this.F&0x1)===
0x1))this.G.Ai(this.H);this.C5=Cw;if(!ES&&!!this.timer){B.sM([this,this.EH],this.
timer,0);this.timer=null;(A=this.GX)?A[1].call(A[0],this):null;}},Aa:function(C){
var A;if((((C===this.BC)&&(C===this.BR))&&(C===this.BP))&&(C===this.BQ))return;this.
BC=C;this.BR=C;this.BP=C;this.BQ=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.
H);},Hv:function(C){var A;if(this.Fe===C)return;this.Fe=C;this.C5=-1;if(!C&&!!this.
timer){B.sM([this,this.EH],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(
B.uk.J4);B.sz([this,this.EH],this.timer,0);B.lq([this,this.EH],this);}if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},BY:function(C){var A;if(B.tl(C,this.
AW))return;this.AW=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},EZ:function(
C){var A;if(C===this.Fd)return;this.Fd=C;if(!!this.G&&((this.F&0x1)===0x1))this.
G.Ai(this.H);},Fm:function(C){var A;if(C<0)C=0;if((C===this.Hs)&&(this.C5===-1))
return;this.Hs=C;if(!this.timer)this.C5=-1;if(!!this.G&&((this.F&0x1)===0x1))this.
G.Ai(this.H);},Dq:function(C){var A;if(C===this.An)return;if(!!this.An&&this.An.
Ij)B.sM([this,this.IL],this.An,0);this.An=C;this.C5=-1;if(!!C&&C.Ij)B.sz([this,this.
IL],C,0);if(this.Fe){this.Hv(false);this.Hv(true);}if(((this.JZ&&!!C)&&(C.FrameSize[
0]>0))&&(C.FrameSize[1]>0))this.P(B.ty(this.J6(),this.AW));if(!!this.G&&((this.F&
0x1)===0x1))this.G.Ai(this.H);},J6:function(){var A;if(!this.An)return Dh;var orient=
this.Hy;var AH=this.Fd;var size=this.An.FrameSize;var U=this.H;var width=U[2]-U[
0];var height=U[3]-U[1];if(!size[0]||!size[1])return Dh;if((orient===1)||(orient===
3)){width=height;height=U[2]-U[0];}var AL=[0,0,width,height];var Ae=AL;if(((AH&0x40
)===0x40)){var P0=((((AL[2]-AL[0])<<16)+((size[0]/2)|0))/size[0])|0;var Me=((((AL[
3]-AL[1])<<16)+((size[1]/2)|0))/size[1])|0;var H9=P0;if(Me>H9)H9=Me;Ae=B.tZ(Ae,((
size[0]*H9)+32768)>>16);Ae=B.tW(Ae,((size[1]*H9)+32768)>>16);}else if(((AH&0x80)===
0x80)){var P0=((((AL[2]-AL[0])<<16)+((size[0]/2)|0))/size[0])|0;var Me=((((AL[3]-
AL[1])<<16)+((size[1]/2)|0))/size[1])|0;var H9=P0;if(Me<H9)H9=Me;Ae=B.tZ(Ae,((size[
0]*H9)+32768)>>16);Ae=B.tW(Ae,((size[1]*H9)+32768)>>16);}else if(!((AH&0x100)===
0x100))Ae=B.tY(Ae,size);if((Ae[2]-Ae[0])!==(AL[2]-AL[0])){if(((AH&0x4)===0x4))Ae=
B.t0(Ae,AL[2]-(Ae[2]-Ae[0]));else if(((AH&0x2)===0x2))Ae=B.t0(Ae,(AL[0]+(((AL[2]-
AL[0])/2)|0))-(((Ae[2]-Ae[0])/2)|0));}if((Ae[3]-Ae[1])!==(AL[3]-AL[1])){if(((AH&
0x20)===0x20))Ae=B.t2(Ae,AL[3]-(Ae[3]-Ae[1]));else if(((AH&0x10)===0x10))Ae=B.t2(
Ae,(AL[1]+(((AL[3]-AL[1])/2)|0))-(((Ae[3]-Ae[1])/2)|0));}if(!orient)Ae=B.tz(Ae,U.
slice(0,2));else if(orient===1){var Ek=[U[0]+Ae[1],U[3]-Ae[2]];Ae=[].concat(Ek,B.
tx(Ek,[Ae[3]-Ae[1],Ae[2]-Ae[0]]));}else if(orient===2){var Ek=[U[2]-Ae[2],U[3]-Ae[
3]];Ae=[].concat(Ek,B.tx(Ek,[Ae[2]-Ae[0],Ae[3]-Ae[1]]));}else if(orient===3){var
Ek=[U[2]-Ae[3],U[1]+Ae[0]];Ae=[].concat(Ek,B.tx(Ek,[Ae[3]-Ae[1],Ae[2]-Ae[0]]));}
return B.tz(Ae,this.AW);},Tv:function(){return this.AW;},_Init:function(aArg){B.
Core.AQ._Init.call(this,aArg);this.__proto__=E.FQ;},_Mark:function(D){var A;B.Core.
AQ._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.An)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.GX)&&((A=A[0])._cycle!=D)
)A._Mark(A._cycle=D);},_className:"Views::Image"};E.Text={BW:null,DO:null,By:B.hm
,String:B.hm,EP:null,FK:B.qx,Lw:0,I$:0,BP:0xFFFFFFFF,BQ:0xFFFFFFFF,BR:0xFFFFFFFF
,BC:0xFFFFFFFF,Jf:0,AW:B.qx,Fd:0x12,Dd:255,Hy:0,Kw:false,JZ:false,K8:false,N1:false
,DY:false,Da:function(AA,aClip,aOffset,Aj,aBlend){var A;if((this.By===B.hm)||!this.
BW)return;var AH=this.Fd;var orient=this.Hy;var font=this.BW;var AL=B.tz(this.H,
aOffset);var A2=this.BC;var A3=this.BR;var A1=this.BQ;var A0=this.BP;var Ej=(((Aj+
1)*this.Dd)>>8)+1;var GL=this.By.charCodeAt(0)||0;var R=B.tz(this.J6(),aOffset);
var BL=[AL[0]-R[0],(AL[1]-R[1])-font.Ascent];if(GL<1)return;if(Ej<256){A2=(A2&0x00FFFFFF
)|((((((A2>>24)&0xFF)*Ej)>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((((A3>>24)&0xFF)*
Ej)>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*Ej)>>8)&0xFF)<<24);A0=(
A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*Ej)>>8)&0xFF)<<24);}if(((AH&0x80)===0x80)){if(
this.Ol())AH=(AH&~0x80)|0x4;else AH=(AH&~0x80)|0x1;}if(((GL===1)&&!((AH&0x40)===
0x40))&&!orient){AA.Qk(aClip,font,this.By,2,(this.By.charCodeAt(1)||0)-1,AL,BL,0
,orient,A2,A3,A1,A0,true);return;}var leading=font.Leading;if(this.I$>0)leading=(
this.I$-font.Ascent)-font.Descent;var PZ=(font.Ascent+font.Descent)+leading;var Ni=
aClip[1]-R[1];var Nj=aClip[3]-R[1];var LJ=R[2]-R[0];var Gg=0;var I=1;var Ca=this.
By.charCodeAt(I)||0;if(orient===1){BL=[R[3]-AL[3],(AL[0]-R[0])-font.Ascent];Ni=aClip[
0]-R[0];Nj=aClip[2]-R[0];LJ=R[3]-R[1];}else if(orient===2){BL=[R[2]-AL[2],(R[3]-
AL[3])-font.Ascent];Ni=R[3]-aClip[3];Nj=R[3]-aClip[1];}else if(orient===3){BL=[AL[
1]-R[1],(R[2]-AL[2])-font.Ascent];Ni=R[2]-aClip[2];Nj=R[2]-aClip[0];LJ=R[3]-R[1];
}while(((Gg+PZ)<Ni)&&(Ca>0)){I=I+Ca;Gg=Gg+PZ;Ca=this.By.charCodeAt(I)||0;}while((
Gg<Nj)&&(Ca>0)){var JI=B.tw(BL,[0,Gg]);var RZ=0;var Nw=false;if(((((AH&0x40)===0x40
)&&((this.By.charCodeAt((I+Ca)-1)||0)!==0x0A))&&((this.By.charCodeAt(I+1)||0)!==
0x0A))&&((this.By.charCodeAt(I+Ca)||0)!==0x00))Nw=true;if(Nw&&!!(AH&0x6)){var RY=
I+Ca;var Rr=this.By.indexOf(String.fromCharCode(0x20),I+1);var Rs=this.By.indexOf(
String.fromCharCode(0xA0),I+1);if(((Rr<0)||(Rr>=RY))&&((Rs<0)||(Rs>=RY)))Nw=false;
}if(Nw)RZ=LJ;else if(((AH&0x4)===0x4))JI=[(JI[0]-LJ)+font.Lg(this.By,I+1,Ca-1),JI[
1]];else if(((AH&0x2)===0x2))JI=[(JI[0]-((LJ/2)|0))+((font.Lg(this.By,I+1,Ca-1)/
2)|0),JI[1]];AA.Qk(aClip,font,this.By,I+1,Ca-1,AL,JI,RZ,orient,A2,A3,A1,A0,true);
I=I+Ca;Gg=Gg+PZ;Ca=this.By.charCodeAt(I)||0;}},P:function(C){var A;if(B.tm(C,this.
H))return;var PY=false;if(!this.Hy||(this.Hy===2))PY=((A=this.H)[2]-A[0])!==(C[2
]-C[0]);else PY=((A=this.H)[3]-A[1])!==(C[3]-C[1]);if((((PY&&!this.Jf)&&this.Kw)&&
this.DY)&&!((this.F&0x2000)===0x2000)){this.By=B.hm;this.DY=false;B.lq([this,this.
IP],this);}if(((this.K8&&this.DY)&&!B.tl([(A=this.H)[2]-A[0],A[3]-A[1]],[C[2]-C[
0],C[3]-C[1]]))&&!((this.F&0x2000)===0x2000)){this.By=B.hm;this.DY=false;B.lq([this
,this.IP],this);}B.Core.AQ.P.call(this,C);B.lq([this,this.NH],this);},D3:function(
){if(!!this.EP){this.Pr(this.EP);this.EP=null;}},Pr:function(aBidi){if(!aBidi)return;
B.ng(aBidi);},Vw:function(aSize){var bidi=null;bidi=B.qk(aSize);return bidi;},NH:
function(Af){B.lq(this.DO,this);},IP:function(Af){B.lq([this,this.NK],this);},NK:
function(Af){var A;if(this.DY)return;var orient=this.Hy;var width=(A=this.H)[2]-
A[0];var height=(A=this.H)[3]-A[1];var FG=-1;if((orient===1)||(orient===3)){width=
height;height=(A=this.H)[2]-A[0];}if(this.Kw){if(this.Jf>0)FG=this.Jf;else if(!this.
JZ)FG=width-(this.Lw*2);else FG=width;if(FG<0)FG=1;}if(!!this.EP){this.Pr(this.EP
);this.EP=null;}this.DY=true;if((this.String!==B.hm)&&!!this.BW){var length=this.
String.length;if(this.N1)this.EP=this.Vw(length);this.By=this.BW.T2(this.String,
0,FG,length,this.EP);if(!!this.EP&&!this.S4()){this.Pr(this.EP);this.EP=null;}}else
this.By=B.hm;this.FK=Be;if(((this.K8&&(this.By!==B.hm))&&!this.JZ)&&!!this.BW){var
AH=this.Fd;var font=this.BW;var leading=font.Leading;var BO=this.By;var NQ=this.
Ol();if(((AH&0x80)===0x80)){if(NQ)AH=(AH&~0x80)|0x4;else AH=(AH&~0x80)|0x1;}if(this.
I$>0)leading=(this.I$-font.Ascent)-font.Descent;var Mb=(font.Ascent+font.Descent
)+leading;var GL=BO.charCodeAt(0)||0;var II=((height+leading)/Mb)|0;var Po=false;
var Nh=false;if(II<=0)II=1;if(GL>II){var Gf=0;var Mc=0;var NP=GL-1;var CF=0;var Dj=
BO.length;var tmp=B.hm;if(((AH&0x20)===0x20))Mc=GL-II;else if(((AH&0x10)===0x10)
){Mc=((GL-II)/2)|0;NP=(Mc+II)-1;}else NP=II-1;Po=Mc>0;Nh=NP<(GL-1);for(CF=1;Gf<Mc;
Gf=Gf+1)CF=CF+(BO.charCodeAt(CF)||0);if(Nh)for(Dj=CF;Gf<NP;Gf=Gf+1)Dj=Dj+(BO.charCodeAt(
Dj)||0);if(Po){var Cy=BO.charCodeAt(CF)||0;tmp=(CK+B.t9(BO,CF,Cy))+CK;tmp=B.t4(tmp
,0,(Cy+2)&0xFFFF);CF=CF+Cy;if((tmp.charCodeAt(Cy)||0)===0x0A){tmp=B.t4(tmp,Cy,0xFEFF
);tmp=B.t4(tmp,Cy+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.t4(tmp,2,0xFEFF
);tmp=B.t4(tmp,1,0x0A);}else tmp=B.t4(tmp,1,0xFEFF);}tmp=tmp+B.t9(BO,CF,Dj-CF);if(
Nh&&(Dj>=CF)){var Cy=BO.charCodeAt(Dj)||0;var Ep=(CK+B.t9(BO,Dj,Cy))+CK;Ep=B.t4(
Ep,0,(Cy+2)&0xFFFF);Ep=B.t4(Ep,1,0xFEFF);if((Ep.charCodeAt(Cy)||0)===0x0A){Ep=B.
t4(Ep,Cy,0xFEFF);Ep=B.t4(Ep,Cy+1,0x0A);}if((Ep.charCodeAt(2)||0)===0x0A){Ep=B.t4(
Ep,2,0xFEFF);Ep=B.t4(Ep,1,0x0A);}else Ep=B.t4(Ep,1,0xFEFF);tmp=tmp+Ep;}BO=String.
fromCharCode(II&0xFFFF)+tmp;}var Gf=0;var DW=1;var NA=width-(this.Lw*2);if(this.
Kw&&(this.Jf>0))NA=this.Jf;GL=BO.charCodeAt(0)||0;for(;Gf<GL;Gf=Gf+1){var IR=Po&&
!Gf;var IS=Nh&&(Gf===(GL-1));var Fw=false;var Fx=false;var KO=NQ;if((NQ&&IR)&&!IS
){IR=false;IS=true;}else if((NQ&&IS)&&!IR){IS=false;IR=true;}var IT=DW+1;var Cy=
BO.charCodeAt(DW)||0;var CF=IT;var Dj=(IT+Cy)-2;var Py=-1;var Pz=-1;if(!this.Kw&&(
font.Lg(BO,IT,Cy-1)>NA)){if(((AH&0x4)===0x4))Fw=true;else if(((AH&0x2)===0x2)){Fw=
true;Fx=true;}else Fx=true;}if((BO.charCodeAt(CF)||0)===0x0A)CF=CF+1;if((BO.charCodeAt(
Dj)||0)===0x0A)Dj=Dj-1;while(Fw&&((BO.charCodeAt(CF)||0)===0xFEFF))CF=CF+1;while(
Fx&&((BO.charCodeAt(Dj)||0)===0xFEFF))Dj=Dj-1;Fw=Fw&&!IS;Fx=Fx&&!IR;while((((Fw||
Fx)||IR)||IS)&&(CF<Dj)){if((Fw&&(KO||!Fx))||IR){if(Py>0)BO=B.t4(BO,Py,0xFEFF);BO=
B.t4(BO,CF,0x2026);Py=CF;CF=CF+1;KO=!KO;IR=false;if(font.Lg(BO,IT,Cy-1)<=NA){Fw=
false;Fx=false;}else Fw=Fw||!Fx;}if((Fx&&(!KO||!Fw))||IS){if(Pz>0)BO=B.t4(BO,Pz,
0xFEFF);BO=B.t4(BO,Dj,0x2026);Pz=Dj;Dj=Dj-1;KO=!KO;IS=false;if(font.Lg(BO,IT,Cy-
1)<=NA){Fw=false;Fx=false;}else Fx=Fx||!Fw;}}DW=DW+Cy;}this.FK=[font.Qo(BO),((BO.
charCodeAt(0)||0)*Mb)-leading];this.By=BO;}if(this.JZ&&(this.By!==B.hm)){var H4=[
this.Lw,0];if((orient===1)||(orient===3)){H4=[H4[0],H4[0]];H4=[0,H4[1]];}this.F=
this.F|0x2000;this.P(B.ty(B.th(this.J6(),H4),this.AW));this.F=this.F&~0x2000;}if(
!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);B.lq([this,this.NH],this);},QQ:
function(C){if(C===this.N1)return;this.N1=C;this.By=B.hm;this.DY=false;B.lq([this
,this.IP],this);},QP:function(C){if(C===this.K8)return;this.K8=C;this.By=B.hm;this.
DY=false;B.lq([this,this.IP],this);},Q1:function(C){if(C===this.Kw)return;this.Kw=
C;if(this.DY){this.By=B.hm;this.DY=false;B.lq([this,this.IP],this);}if(C&&!this.
Jf)this.F=this.F&~0x100;else this.F=this.F|0x100;},BY:function(C){var A;if(B.tl(
C,this.AW))return;this.AW=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);
if(this.DY)B.lq([this,this.NH],this);},EZ:function(C){var A;if(C===this.Fd)return;
this.Fd=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);if(this.K8){this.By=
B.hm;this.DY=false;B.lq([this,this.IP],this);}if(this.DY)B.lq([this,this.NH],this
);},Cu:function(C){if(C===this.String)return;this.String=C;this.By=B.hm;this.DY=
false;B.lq([this,this.IP],this);},Fl:function(C){if(C===this.BW)return;this.BW=C;
this.By=B.hm;this.DY=false;B.lq([this,this.IP],this);},Aa:function(C){var A;if((((
C===this.BC)&&(C===this.BR))&&(C===this.BP))&&(C===this.BQ))return;this.BC=C;this.
BR=C;this.BP=C;this.BQ=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},GZ:
function(C){var A;if(C>255)C=255;if(C<0)C=0;if(C===this.Dd)return;this.Dd=C;if(!
!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);},Ol:function(){if(!this.DY)this.
NK(this);if(!this.EP)return false;var result=false;var bidi=this.EP;result=B.qj(
bidi);return result;},S4:function(){if(!this.DY)this.NK(this);if(!this.EP)return false;
var result=false;var bidi=this.EP;result=B.sD(bidi);return result;},J6:function(
){var A;if((this.String===B.hm)||!this.BW)return Dh;if(!this.DY)this.NK(this);if(
this.By===B.hm)return Dh;var leading=this.BW.Leading;var Mb=(this.BW.Ascent+this.
BW.Descent)+this.BW.Leading;if(this.I$>0){leading=(this.I$-this.BW.Ascent)-this.
BW.Descent;Mb=this.I$;}if(B.tl(this.FK,Be))this.FK=[this.BW.Qo(this.By),this.FK[
1]];this.FK=[this.FK[0],((this.By.charCodeAt(0)||0)*Mb)-leading];var AH=this.Fd;
var orient=this.Hy;var U=this.H;var H4=this.Lw;var width=U[2]-U[0];var height=U[
3]-U[1];if((orient===1)||(orient===3)){width=height;height=U[2]-U[0];}var AL=[H4
,0,width-H4,height];var Ae=[].concat(AL.slice(0,2),B.tx(AL.slice(0,2),this.FK));
if(((AH&0x80)===0x80)){if(this.Ol())AH=(AH&~0x80)|0x4;else AH=(AH&~0x80)|0x1;}if(((
AH&0x40)===0x40)){var FG=this.Jf;if(FG<=0)FG=width-(this.Lw*2);if(FG<0)FG=0;if(FG>(
Ae[2]-Ae[0]))Ae=B.tZ(Ae,FG);}if((Ae[2]-Ae[0])!==(AL[2]-AL[0])){if(((AH&0x4)===0x4
))Ae=B.t0(Ae,AL[2]-(Ae[2]-Ae[0]));else if(((AH&0x2)===0x2))Ae=B.t0(Ae,(AL[0]+(((
AL[2]-AL[0])/2)|0))-(((Ae[2]-Ae[0])/2)|0));}if((Ae[3]-Ae[1])!==(AL[3]-AL[1])){if(((
AH&0x20)===0x20))Ae=B.t2(Ae,AL[3]-(Ae[3]-Ae[1]));else if(((AH&0x10)===0x10))Ae=B.
t2(Ae,(AL[1]+(((AL[3]-AL[1])/2)|0))-(((Ae[3]-Ae[1])/2)|0));}if(!orient)Ae=B.tz(Ae
,U.slice(0,2));else if(orient===1){var Ek=[U[0]+Ae[1],U[3]-Ae[2]];Ae=[].concat(Ek
,B.tx(Ek,[this.FK[1],this.FK[0]]));}else if(orient===2){var Ek=[U[2]-Ae[2],U[3]-
Ae[3]];Ae=[].concat(Ek,B.tx(Ek,this.FK));}else if(orient===3){var Ek=[U[2]-Ae[3]
,U[1]+Ae[0]];Ae=[].concat(Ek,B.tx(Ek,[this.FK[1],this.FK[0]]));}return B.tz(Ae,this.
AW);},_Init:function(aArg){B.Core.AQ._Init.call(this,aArg);this.__proto__=E.Text;
},_Done:function(){this.D3();this.__proto__=B.Core.AQ;B.Core.AQ._Done.call(this);
},_Mark:function(D){var A;B.Core.AQ._Mark.call(this,D);if((A=this.BW)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.DO)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);},_className:"Views::Text"};E.Cf={AK:null,AU:null,BP:0xFFFFFFFF,BQ:0xFFFFFFFF,
BR:0xFFFFFFFF,BC:0xFFFFFFFF,As:B.qx,Ob:0,Cn:false,G2:true,Le:false,Da:function(AA
,aClip,aOffset,Aj,aBlend){if(!this.AK||(this.Cn&&!this.AU))return;var GA=null;var
A2=this.BC;var A3=this.BR;var A0=this.BP;var A1=this.BQ;if(this.Cn){GA=B._NewObject(
B.Graphics.Canvas,0);GA.AttachBitmap(this.AU);}Aj=Aj+1;if(Aj<256){A2=(A2&0x00FFFFFF
)|((((Aj*((A2>>24)&0xFF))>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((Aj*((A3>>24)&0xFF
))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((Aj*((A0>>24)&0xFF))>>8)&0xFF)<<24);A1=(
A1&0x00FFFFFF)|((((Aj*((A1>>24)&0xFF))>>8)&0xFF)<<24);}if(!GA)AA.Cf(aClip,this.AK
,B.tz(this.H,aOffset),this.Le,this.As,A2,A3,A1,A0,true,this.G2,this.Ob);else{AA.
Mm(aClip,GA,0,B.tz(this.H,aOffset),Be,A2,A3,A1,A0,true);GA.DetachBitmap();}},P:function(
C){var A;if(this.Cn&&!B.tl([(A=this.H)[2]-A[0],A[3]-A[1]],[C[2]-C[0],C[3]-C[1]])
)B.lq([this,this.GP],this);B.Core.AQ.P.call(this,C);},D3:function(){this.LP();},
LP:function(){if(!this.AU)return;var handle=this.AU;B.sW(handle);this.AU=null;},
GP:function(Af){var A;this.LP();if(this.Cn&&!!this.AK){var handle=null;var path=
this.AK.path;var size=[(A=this.H)[2]-A[0],A[3]-A[1]];var quality=this.G2;var flipY=
this.Le;var offset=this.As;var nzwind=this.Ob===1;handle=B.qn(size,path,flipY,offset
,quality,nzwind);this.AU=handle;}},Eq:function(Af){var A;if(!!this.G&&((this.F&0x1
)===0x1))this.G.Ai(this.H);if(this.Cn)B.lq([this,this.GP],this);},Aa:function(C){
var A;if((((C===this.BC)&&(C===this.BR))&&(C===this.BP))&&(C===this.BQ))return;this.
BC=C;this.BR=C;this.BP=C;this.BQ=C;if((!!this.G&&((this.F&0x1)===0x1))&&!!this.AK
)this.G.Ai(this.H);},Eu:function(C){var A;if(B.tl(C,this.As))return;this.As=C;if((
!!this.G&&((this.F&0x1)===0x1))&&!!this.AK)this.G.Ai(this.H);if(this.Cn)B.lq([this
,this.GP],this);},EL:function(C){var A;if(C===this.AK)return;if(!!this.AK)B.sM([
this,this.Eq],this.AK,0);this.AK=C;if(!!this.AK)B.sz([this,this.Eq],this.AK,0);if(
!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);if(this.Cn)B.lq([this,this.GP],
this);},_Init:function(aArg){B.Core.AQ._Init.call(this,aArg);this.__proto__=E.Cf;
},_Done:function(){this.D3();this.__proto__=B.Core.AQ;B.Core.AQ._Done.call(this);
},_Mark:function(D){var A;B.Core.AQ._Mark.call(this,D);if((A=this.AK)&&(A._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::FillPath"};E.CJ={AK:null,AU:null,BP:
0xFFFFFFFF,BQ:0xFFFFFFFF,BR:0xFFFFFFFF,BC:0xFFFFFFFF,As:B.qx,QB:3.000000,Width:1.000000
,Q8:0,Ql:0,MC:0,Cn:false,G2:true,Le:false,Da:function(AA,aClip,aOffset,Aj,aBlend
){if(!this.AK||(this.Cn&&!this.AU))return;var GA=null;var A2=this.BC;var A3=this.
BR;var A0=this.BP;var A1=this.BQ;if(this.Cn){GA=B._NewObject(B.Graphics.Canvas,0
);GA.AttachBitmap(this.AU);}Aj=Aj+1;if(Aj<256){A2=(A2&0x00FFFFFF)|((((Aj*((A2>>24
)&0xFF))>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((Aj*((A3>>24)&0xFF))>>8)&0xFF)<<24
);A0=(A0&0x00FFFFFF)|((((Aj*((A0>>24)&0xFF))>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((
Aj*((A1>>24)&0xFF))>>8)&0xFF)<<24);}if(!GA)AA.CJ(aClip,this.AK,B.tz(this.H,aOffset
),this.Le,this.As,this.Width,this.Q8,this.Ql,this.MC,this.QB,A2,A3,A1,A0,true,this.
G2);else{AA.Mm(aClip,GA,0,B.tz(this.H,aOffset),Be,A2,A3,A1,A0,true);GA.DetachBitmap(
);}},P:function(C){var A;if(this.Cn&&!B.tl([(A=this.H)[2]-A[0],A[3]-A[1]],[C[2]-
C[0],C[3]-C[1]]))B.lq([this,this.GP],this);B.Core.AQ.P.call(this,C);},D3:function(
){this.LP();},LP:function(){if(!this.AU)return;var handle=this.AU;B.sW(handle);this.
AU=null;},GP:function(Af){var A;this.LP();if(this.Cn&&!!this.AK){var handle=null;
var path=this.AK.path;var size=[(A=this.H)[2]-A[0],A[3]-A[1]];var quality=this.G2;
var flipY=this.Le;var offset=this.As;var width=this.Width;var miterLimit=this.QB;
var style=0;switch(this.Q8){case 1:style=style|1;break;case 3:style=style|2;break;
case 2:style=style|3;break;default:;}switch(this.Ql){case 1:style=style|256;break;
case 3:style=style|512;break;case 2:style=style|768;break;default:;}switch(this.
MC){case 1:style=style|65536;break;case 2:style=style|131072;break;default:;}handle=
B.qo(size,path,flipY,offset,width,style,miterLimit,quality);this.AU=handle;}},Eq:
function(Af){var A;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);if(this.Cn
)B.lq([this,this.GP],this);},Kb:function(){return this.BC;},Aa:function(C){var A;
if((((C===this.BC)&&(C===this.BR))&&(C===this.BP))&&(C===this.BQ))return;this.BC=
C;this.BR=C;this.BP=C;this.BQ=C;if((!!this.G&&((this.F&0x1)===0x1))&&!!this.AK)this.
G.Ai(this.H);},Eu:function(C){var A;if(B.tl(C,this.As))return;this.As=C;if((!!this.
G&&((this.F&0x1)===0x1))&&!!this.AK)this.G.Ai(this.H);if(this.Cn)B.lq([this,this.
GP],this);},TQ:function(C){var A;if(C===this.MC)return;this.MC=C;if((!!this.G&&((
this.F&0x1)===0x1))&&!!this.AK)this.G.Ai(this.H);if(this.Cn)B.lq([this,this.GP],
this);},CI:function(C){var A;if(C<0.000000)C=0.000000;if(C===this.Width)return;this.
Width=C;if((!!this.G&&((this.F&0x1)===0x1))&&!!this.AK)this.G.Ai(this.H);if(this.
Cn)B.lq([this,this.GP],this);},EL:function(C){var A;if(C===this.AK)return;if(!!this.
AK)B.sM([this,this.Eq],this.AK,0);this.AK=C;if(!!this.AK)B.sz([this,this.Eq],this.
AK,0);if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.H);if(this.Cn)B.lq([this,
this.GP],this);},_Init:function(aArg){B.Core.AQ._Init.call(this,aArg);this.__proto__=
E.CJ;},_Done:function(){this.D3();this.__proto__=B.Core.AQ;B.Core.AQ._Done.call(
this);},_Mark:function(D){var A;B.Core.AQ._Mark.call(this,D);if((A=this.AK)&&(A.
_cycle!=D))A._Mark(A._cycle=D);},_className:"Views::StrokePath"};E.W1={P8:0x1,P7:
0x2,P9:0x4,Qa:0x8,P$:0x10,P_:0x20,Xy:0x40,Xz:0x80,XF:0x100};E.XG={P8:0x1,P7:0x2,
P9:0x4,Qa:0x8,P$:0x10,P_:0x20,Wk:0x40,Wj:0x80};E.Hy={Xa:0,Xw:1,Xu:2,Xv:3};
E._Init=function(){E.Cr.__proto__=B.Core.J$;E.Fn.__proto__=B.Core.AQ;E.EV.__proto__=
B.Core.AQ;E.Mt.__proto__=B.Core.AQ;E.FQ.__proto__=B.Core.AQ;E.Text.__proto__=B.Core.
AQ;E.Cf.__proto__=B.Core.AQ;E.CJ.__proto__=B.Core.AQ;};E.DC=function(D){};return E;
})();

/* Embedded Wizard */