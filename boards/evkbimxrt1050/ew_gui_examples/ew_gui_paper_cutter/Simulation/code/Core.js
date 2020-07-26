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
);if(EmWiApp.Core)throw new Error("The unit file 'Core.js' included twice!");EmWiApp.
Core=(function(){var B=EmWiApp;var C={};
var AM=[0,0];var BD=[0,0,0,0];var Dj="The view does not belong to this group";var
FO="No view to remove";var DV="View is not in this group";var FP="No view to add";
var FQ="View already in a group";var I$="Recursive invalidate during active update cycle.";
var Gt="The KeyPressHandler is embedded within an object not being derived "+"from Core::Group.";
C.CS={BL:null,AY:null,L:null,A6:null,F:0x103,G0:0,Fe:0x14,G_:function(Ab,H6){},S:
function(E){var A;var N2=E^this.Fe;if(!N2)return;this.Fe=E;if(!!this.A6&&!((this.
F&0x400)===0x400)){this.L.F=this.L.F|0x5000;B.lq([A=this.L,A.CM],this);this.L.AL([
0,0,(A=this.L.G)[2]-A[0],A[3]-A[1]]);}if(!!this.A6&&((this.F&0x400)===0x400)){this.
A6.Gb.F=this.A6.Gb.F|0x1000;this.L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM],this);
}},IN:function(){var Ag=this.L;while(!!Ag){var KU=(C.Root.isPrototypeOf(Ag)?Ag:null
);if(!!KU)return KU;Ag=Ag.L;}return null;},DK:function(Bi,aClip,aOffset,A_,aBlend
){},BZ:function(Aw){return null;},EQ:function(AD,R,CU,Gv,FV){return null;},Ht:function(
Ab,Dk){return AM;},IT:function(aOffset,H5){},GetExtent:function(){return BD;},BW:
function(E0,FT){var A;if(((this.F&0x200)===0x200))E0=E0&~0x400;var Jy=(this.F&~FT
)|E0;var Fr=Jy^this.F;this.F=Jy;if(!!this.L&&!!(Fr&0x14)){var KK=((this.F&0x14)===
0x14);if(KK&&!this.L.Dr)this.L.Gl(this);if(!KK&&(this.L.Dr===this))this.L.Gl(this.
L.JW(this,0x14));}if(!!this.L&&!!(Fr&0x403))this.L.AL(this.GetExtent());if(((!!this.
A6&&!!this.L)&&((Jy&0x400)===0x400))&&((Fr&0x1)===0x1)){this.F=this.F|0x800;this.
L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM],this);}if(!!this.L&&((Fr&0x400)===0x400
)){this.A6=null;this.F=this.F|0x800;this.L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM
],this);}},_Init:function(aArg){this.__proto__=C.CS;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.
BL)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.AY)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.L)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.A6)&&(A._cycle!=D
))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,
_cycle:0,_observers:null,_className:"Core::View"};C.GW={B2:B.qx,Cm:B.qx,B1:B.qx,
Cl:B.qx,G_:function(Ab,H6){var Af=B._NewObject(C.HI,0);this.A6=null;Af.CB=this.GetExtent(
);Af.As=Ab;Af.Gb=H6;Af.Hh=this.Cl;Af.Hi=this.B1;Af.Hj=this.Cm;Af.Hk=this.B2;this.
A6=Af;},Ht:function(Ab,Dk){var A;var BK=this.Fe;var Af=(C.HI.isPrototypeOf(A=this.
A6)?A:null);var U=Af.CB[0];var V=Af.CB[1];var W=Af.CB[2];var X=Af.CB[3];var CA=[
Ab[2]-Ab[0],Ab[3]-Ab[1]];var Ah=W-U;var Ak=X-V;if(!Dk){var DI=[(A=Af.As)[2]-A[0]
,A[3]-A[1]];U=U-Af.As[0];V=V-Af.As[1];if(DI[0]!==CA[0]){var C4=((BK&0x4)===0x4);
var C5=((BK&0x8)===0x8);var Ej=((BK&0x1)===0x1);if(!C4&&(Ej||!C5))U=((U*CA[0])/DI[
0])|0;if(!C5&&(Ej||!C4)){W=W-Af.As[0];W=((W*CA[0])/DI[0])|0;W=W-CA[0];}else W=W-
Af.As[2];U=U+Ab[0];W=W+Ab[2];if(!Ej){if(C4&&!C5)W=U+Ah;else if(!C4&&C5)U=W-Ah;else{
U=U+((((W-U)-Ah)/2)|0);W=U+Ah;}}}else{W=W-Af.As[2];U=U+Ab[0];W=W+Ab[2];}if(DI[1]
!==CA[1]){var C6=((BK&0x10)===0x10);var C3=((BK&0x20)===0x20);var Ek=((BK&0x2)===
0x2);if(!C6&&(Ek||!C3))V=((V*CA[1])/DI[1])|0;if(!C3&&(Ek||!C6)){X=X-Af.As[1];X=((
X*CA[1])/DI[1])|0;X=X-CA[1];}else X=X-Af.As[3];V=V+Ab[1];X=X+Ab[3];if(!Ek){if(C6&&
!C3)X=V+Ak;else if(!C6&&C3)V=X-Ak;else{V=V+((((X-V)-Ak)/2)|0);X=V+Ak;}}}else{X=X-
Af.As[3];V=V+Ab[1];X=X+Ab[3];}}else{switch(Dk){case 3:{U=Ab[0];W=U+Ah;}break;case
4:{W=Ab[2];U=W-Ah;}break;case 1:{V=Ab[1];X=V+Ak;}break;case 2:{X=Ab[3];V=X-Ak;}break;
default:;}if((Dk===3)||(Dk===4)){var C6=((BK&0x10)===0x10);var C3=((BK&0x20)===0x20
);var Ek=((BK&0x2)===0x2);if(Ek){V=Ab[1];X=Ab[3];}else if(C6&&!C3){V=Ab[1];X=V+Ak;
}else if(C3&&!C6){X=Ab[3];V=X-Ak;}else{V=Ab[1]+((((Ab[3]-Ab[1])-Ak)/2)|0);X=V+Ak;
}}if((Dk===1)||(Dk===2)){var C4=((BK&0x4)===0x4);var C5=((BK&0x8)===0x8);var Ej=((
BK&0x1)===0x1);if(Ej){U=Ab[0];W=Ab[2];}else if(C4&&!C5){U=Ab[0];W=U+Ah;}else if(
C5&&!C4){W=Ab[2];U=W-Ah;}else{U=Ab[0]+((((Ab[2]-Ab[0])-Ah)/2)|0);W=U+Ah;}}}Af.isEmpty=(
U>=W)||(V>=X);Ah=(W-U)-1;Ak=(X-V)-1;var E4=Af.CB[0];var E5=Af.CB[1];var EI=(Af.CB[
2]-E4)-1;var EH=(Af.CB[3]-E5)-1;if(!EI)EI=1;if(!EH)EH=1;if(((this.F&0x100)===0x100
)){this.Cl=[U+((((Af.Hh[0]-E4)*Ah)/EI)|0),V+((((Af.Hh[1]-E5)*Ak)/EH)|0)];this.B1=[
U+((((Af.Hi[0]-E4)*Ah)/EI)|0),V+((((Af.Hi[1]-E5)*Ak)/EH)|0)];this.Cm=[U+((((Af.Hj[
0]-E4)*Ah)/EI)|0),V+((((Af.Hj[1]-E5)*Ak)/EH)|0)];this.B2=[U+((((Af.Hk[0]-E4)*Ah)
/EI)|0),V+((((Af.Hk[1]-E5)*Ak)/EH)|0)];}else{this.De([U+((((Af.Hh[0]-E4)*Ah)/EI)|
0),V+((((Af.Hh[1]-E5)*Ak)/EH)|0)]);this.Df([U+((((Af.Hi[0]-E4)*Ah)/EI)|0),V+((((
Af.Hi[1]-E5)*Ak)/EH)|0)]);this.Dg([U+((((Af.Hj[0]-E4)*Ah)/EI)|0),V+((((Af.Hj[1]-
E5)*Ak)/EH)|0)]);this.Dh([U+((((Af.Hk[0]-E4)*Ah)/EI)|0),V+((((Af.Hk[1]-E5)*Ak)/EH
)|0)]);this.A6=Af;}return[Ah+1,Ak+1];},IT:function(aOffset,H5){if(H5){this.Cl=B.
tx(this.Cl,aOffset);this.B1=B.tx(this.B1,aOffset);this.Cm=B.tx(this.Cm,aOffset);
this.B2=B.tx(this.B2,aOffset);}else{this.De(B.tx(this.Cl,aOffset));this.Df(B.tx(
this.B1,aOffset));this.Dg(B.tx(this.Cm,aOffset));this.Dh(B.tx(this.B2,aOffset));
}},GetExtent:function(){if(!!this.A6&&this.A6.isEmpty)return BD;var U=this.Cl[0];
var V=this.Cl[1];var W=this.Cm[0];var X=this.Cm[1];if((((this.B2[0]!==U)||(this.
B1[1]!==V))||(this.B1[0]!==W))||(this.B2[1]!==X)){if(this.B1[0]<U)U=this.B1[0];if(
this.Cm[0]<U)U=this.Cm[0];if(this.B2[0]<U)U=this.B2[0];if(this.B1[1]<V)V=this.B1[
1];if(this.Cm[1]<V)V=this.Cm[1];if(this.B2[1]<V)V=this.B2[1];if(this.Cl[0]>W)W=this.
Cl[0];if(this.B1[0]>W)W=this.B1[0];if(this.B2[0]>W)W=this.B2[0];if(this.Cl[1]>X)
X=this.Cl[1];if(this.B1[1]>X)X=this.B1[1];if(this.B2[1]>X)X=this.B2[1];}else{var
tmp;if(W<U){tmp=U;U=W;W=tmp;}if(X<V){tmp=V;V=X;X=tmp;}}return[U,V,W+1,X+1];},Dh:
function(E){var A;if(B.tl(E,this.B2))return;if(!!this.L&&((this.F&0x1)===0x1))this.
L.AL(this.GetExtent());this.A6=null;this.B2=E;if(!!this.L&&((this.F&0x1)===0x1))
this.L.AL(this.GetExtent());if((!!this.L&&((this.F&0x400)===0x400))&&!((this.L.F&
0x2000)===0x2000)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;B.lq([A=this.L,A.
CM],this);}},Dg:function(E){var A;if(B.tl(E,this.Cm))return;if(!!this.L&&((this.
F&0x1)===0x1))this.L.AL(this.GetExtent());this.A6=null;this.Cm=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AL(this.GetExtent());if((!!this.L&&((this.F&0x400)===0x400
))&&!((this.L.F&0x2000)===0x2000)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;
B.lq([A=this.L,A.CM],this);}},Df:function(E){var A;if(B.tl(E,this.B1))return;if(
!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.GetExtent());this.A6=null;this.B1=
E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.GetExtent());if((!!this.L&&((
this.F&0x400)===0x400))&&!((this.L.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM],this);}},De:function(E){var A;if(B.tl(E
,this.Cl))return;if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.GetExtent());this.
A6=null;this.Cl=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.GetExtent());
if((!!this.L&&((this.F&0x400)===0x400))&&!((this.L.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM],this);}},IP:function(Ea){var
A7=B.tA(4,B.qx,null);var K=0;var Ba=3;var Jt=false;var Ju=false;A7.Set(0,this.Cl
);A7.Set(1,this.B1);A7.Set(2,this.Cm);A7.Set(3,this.B2);while(K<4){var K1=A7.Get(
K)[0];var IA=A7.Get(K)[1];var Oq=A7.Get(Ba)[0];var JL=A7.Get(Ba)[1];if(((IA>Ea[1
])!==(JL>Ea[1]))||((IA<Ea[1])!==(JL<Ea[1]))){var Ay=((((Oq-K1)*(Ea[1]-IA))/(JL-IA
))|0)+K1;if(Ea[0]>Ay)Jt=!Jt;if(Ea[0]<Ay)Ju=!Ju;}Ba=K;K=K+1;}return Jt||Ju;},Ll:function(
){return((((this.Cl[0]===this.B2[0])&&(this.B1[0]===this.Cm[0]))&&(this.Cl[1]===
this.B1[1]))&&(this.Cm[1]===this.B2[1]))||((((this.Cl[0]===this.B1[0])&&(this.Cm[
0]===this.B2[0]))&&(this.Cl[1]===this.B2[1]))&&(this.B1[1]===this.Cm[1]));},_Init:
function(aArg){C.CS._Init.call(this,aArg);this.__proto__=C.GW;},_className:"Core::QuadView"
};C.BO={G:B.qy,G_:function(Ab,H6){var Af=B._NewObject(C.HH,0);Af.CB=this.G;Af.As=
Ab;Af.Gb=H6;this.A6=Af;},Ht:function(Ab,Dk){var A;var BK=this.Fe;var Af=this.A6;
var U=Af.CB[0];var V=Af.CB[1];var W=Af.CB[2];var X=Af.CB[3];var CA=[Ab[2]-Ab[0],
Ab[3]-Ab[1]];var Ah=W-U;var Ak=X-V;if(!Dk){var DI=[(A=Af.As)[2]-A[0],A[3]-A[1]];
U=U-Af.As[0];V=V-Af.As[1];if(DI[0]!==CA[0]){var C4=((BK&0x4)===0x4);var C5=((BK&
0x8)===0x8);var Ej=((BK&0x1)===0x1);if(!C4&&(Ej||!C5))U=((U*CA[0])/DI[0])|0;if(!
C5&&(Ej||!C4)){W=W-Af.As[0];W=((W*CA[0])/DI[0])|0;W=W-CA[0];}else W=W-Af.As[2];U=
U+Ab[0];W=W+Ab[2];if(!Ej){if(C4&&!C5)W=U+Ah;else if(!C4&&C5)U=W-Ah;else{U=U+((((
W-U)-Ah)/2)|0);W=U+Ah;}}}else{W=W-Af.As[2];U=U+Ab[0];W=W+Ab[2];}if(DI[1]!==CA[1]
){var C6=((BK&0x10)===0x10);var C3=((BK&0x20)===0x20);var Ek=((BK&0x2)===0x2);if(
!C6&&(Ek||!C3))V=((V*CA[1])/DI[1])|0;if(!C3&&(Ek||!C6)){X=X-Af.As[1];X=((X*CA[1]
)/DI[1])|0;X=X-CA[1];}else X=X-Af.As[3];V=V+Ab[1];X=X+Ab[3];if(!Ek){if(C6&&!C3)X=
V+Ak;else if(!C6&&C3)V=X-Ak;else{V=V+((((X-V)-Ak)/2)|0);X=V+Ak;}}}else{X=X-Af.As[
3];V=V+Ab[1];X=X+Ab[3];}}else{switch(Dk){case 3:{U=Ab[0];W=U+Ah;}break;case 4:{W=
Ab[2];U=W-Ah;}break;case 1:{V=Ab[1];X=V+Ak;}break;case 2:{X=Ab[3];V=X-Ak;}break;
default:;}if((Dk===3)||(Dk===4)){var C6=((BK&0x10)===0x10);var C3=((BK&0x20)===0x20
);var Ek=((BK&0x2)===0x2);if(Ek){V=Ab[1];X=Ab[3];}else if(C6&&!C3){V=Ab[1];X=V+Ak;
}else if(C3&&!C6){X=Ab[3];V=X-Ak;}else{V=Ab[1]+((((Ab[3]-Ab[1])-Ak)/2)|0);X=V+Ak;
}}if((Dk===1)||(Dk===2)){var C4=((BK&0x4)===0x4);var C5=((BK&0x8)===0x8);var Ej=((
BK&0x1)===0x1);if(Ej){U=Ab[0];W=Ab[2];}else if(C4&&!C5){U=Ab[0];W=U+Ah;}else if(
C5&&!C4){W=Ab[2];U=W-Ah;}else{U=Ab[0]+((((Ab[2]-Ab[0])-Ah)/2)|0);W=U+Ah;}}}Af.isEmpty=(
U>=W)||(V>=X);if(((this.F&0x100)===0x100)){this.G=[U,V,W,X];}else{this.H([U,V,W,
X]);this.A6=Af;}return[W-U,X-V];},IT:function(aOffset,H5){if(H5)this.G=B.tz(this.
G,aOffset);else this.H(B.tz(this.G,aOffset));},GetExtent:function(){return this.
G;},H:function(E){var A;if(B.tm(E,this.G))return;if(!!this.L&&((this.F&0x1)===0x1
))this.L.AL(this.G);this.A6=null;this.G=E;if(!!this.L&&((this.F&0x1)===0x1))this.
L.AL(this.G);if((!!this.L&&((this.F&0x400)===0x400))&&!((this.L.F&0x2000)===0x2000
)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM],this);}},NT:
function(JK){this.H(JK);},Lt:function(){return this.G;},_Init:function(aArg){C.CS.
_Init.call(this,aArg);this.__proto__=C.BO;},_className:"Core::RectView"};C.M={DE:
null,CG:null,Ik:null,Dl:null,Fs:null,F0:null,Dr:null,Be:255,DK:function(Bi,aClip
,aOffset,A_,aBlend){var A;A_=((A_+1)*this.Be)>>8;aBlend=aBlend&&((this.F&0x2)===
0x2);if(!this.Dl)this.N3(Bi,aClip,B.tx(aOffset,this.G.slice(0,2)),A_,aBlend);else{
var Cy=255|(255<<8)|(255<<16)|((A_&0xFF)<<24);this.Dl.Update();Bi.JQ(aClip,this.
Dl,0,B.tz(this.G,aOffset),AM,Cy,Cy,Cy,Cy,aBlend);}},EQ:function(AD,R,CU,Gv,FV){var
A;var N=this.CG;var F_=null;var Al=BD;var BA=null;var Jx=!!this.F0&&(!!this.F0.Jn||
!!this.F0.DE);if(((A=B.il(AD,this.G))[0]>=A[2])||(A[1]>=A[3]))return null;AD=B.ty(
AD,this.G.slice(0,2));while(!!N){if(((N.F&0x400)===0x400)&&!BA){BA=N.AY;while(!!
BA&&!((BA.F&0x200)===0x200))BA=BA.AY;if(!!BA)Al=B.il(AD,BA.GetExtent());else Al=
BD;}if(BA===N){BA=null;Al=BD;}if((((((N.F&0x8)===0x8)&&((N.F&0x10)===0x10))&&!((
N.F&0x40000)===0x40000))&&!((N.F&0x20000)===0x20000))&&(!((N.F&0x10000)===0x10000
)||((this.Fs.CW===N)&&!Jx))){var CB=N.GetExtent();var Ic=Gv;var FY=null;if(Ic===
N)Ic=null;if(((N.F&0x400)===0x400)){if(!(((A=B.il(CB,Al))[0]>=A[2])||(A[1]>=A[3]
)))FY=N.EQ(Al,R,CU,Ic,FV);}else{if(!(((A=B.il(CB,AD))[0]>=A[2])||(A[1]>=A[3]))||(
Gv===N))FY=N.EQ(AD,R,CU,Ic,FV);}N=N.AY;if(!!FY){if(!F_||((FY.GI<F_.GI)&&(FY.GI>=
0)))F_=FY;if(!FY.GI)N=null;}}else N=N.AY;}return F_;},BW:function(E0,FT){var A;var
N$=this.F;C.BO.BW.call(this,E0,FT);var Fr=this.F^N$;if(!!this.Dr&&((Fr&0x40)===0x40
)){if(((this.F&0x40)===0x40))this.Dr.BW(0x40,0x0);else this.Dr.BW(0x0,0x40);}if(
!!this.Fs&&((Fr&0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.Fs.CW.F&0x14)===
0x14))this.Fs.CW.BW(0x40,0x0);else this.Fs.CW.BW(0x0,0x40);}if(!!Fr){this.F=this.
F|0x8000;B.lq([this,this.CM],this);}},H:function(E){var A;if(B.tm(E,this.G))return;
var Fx=[(A=this.G)[2]-A[0],A[3]-A[1]];var Io=[E[2]-E[0],E[3]-E[1]];var Gf=!B.tl(
Fx,Io);if(Gf&&!!this.Dl){this.Dl.IZ(Io);B.qw(this,0);B.qw(this.Dl,0);}C.BO.H.call(
this,E);if((Gf&&(Fx[0]>0))&&(Fx[1]>0)){var As=[].concat(AM,Fx);var N=this.DE;var
G9=0x14;while(!!N){if((!N.A6&&(N.Fe!==G9))&&!((N.F&0x400)===0x400))N.G_(As,null);
N=N.BL;}}if(Gf){this.F=this.F|0x5000;B.lq([this,this.CM],this);}},JD:function(Aw
){var KN=(C.KeyEvent.isPrototypeOf(Aw)?Aw:null);var DZ=this.Ik;if(!KN)return null;
while(!!DZ&&(!DZ.Ex||!DZ.BZ(KN)))DZ=DZ.BL;return DZ;},N3:function(Bi,aClip,aOffset
,A_,aBlend){var A;var N=this.DE;var KF=BD;var KR=true;while(!!N){if(((N.F&0x200)===
0x200)){var KQ=(C.GV.isPrototypeOf(N)?N:null);KF=B.il(aClip,B.tz(KQ.G,aOffset));
KR=((KQ.F&0x1)===0x1);}if(((N.F&0x1)===0x1)){if(((N.F&0x400)===0x400)){if(KR){var
Al=B.il(B.tz(N.GetExtent(),aOffset),KF);if(!((Al[0]>=Al[2])||(Al[1]>=Al[3])))N.DK(
Bi,Al,aOffset,A_,aBlend);}}else{var Al=B.il(B.tz(N.GetExtent(),aOffset),aClip);if(
!((Al[0]>=Al[2])||(Al[1]>=Al[3])))N.DK(Bi,Al,aOffset,A_,aBlend);}}N=N.BL;}},Oc:function(
){var A;var Jr=((this.F&0x1000)===0x1000);var E6=[0,0,(A=this.G)[2]-A[0],A[3]-A[
1]];var DX=false;var Ee=BD;var BU=BD;var DY=AM;var GB=0;var GC=0;var GA=0;var CC=
0;var N=this.CG;var BA=null;var G9=0x14;var Fu=null;while(!!N){if(((N.F&0x800)===
0x800)){DX=true;N.F=N.F&~0x800;}if(DX&&((N.F&0x200)===0x200)){DX=false;if(!!(C.GV.
isPrototypeOf(N)?N:null).HD)N.F=N.F|0x1000;}N=N.AY;}DX=false;N=this.DE;if(Jr){this.
F=this.F&~0x1000;Jr=!((E6[0]>=E6[2])||(E6[1]>=E6[3]));}this.F=this.F|0x2000;while(
!!N){if(!Fu&&(GA!==CC)){var Em=N;var It=0;var Hn=Ee[2]-Ee[0];var G7=Ee[3]-Ee[1];
var Ia=0;var Gh=AM;do{if(((Em.F&0x200)===0x200))Em=null;else if(((Em.F&0x401)===
0x401)){Gh=[(A=Em.GetExtent())[2]-A[0],A[3]-A[1]];if((CC===3)||(CC===4))Hn=Hn-Gh[
0];if((CC===1)||(CC===2))G7=G7-Gh[1];if(!Fu||((Hn>=0)&&(G7>=0))){Fu=Em;Em=Em.BL;
if((CC===3)||(CC===4)){Hn=Hn-GB;if(Gh[1]>It)It=Gh[1];}if((CC===1)||(CC===2)){G7=
G7-GC;if(Gh[0]>Ia)Ia=Gh[0];}}else Em=null;}else Em=Em.BL;}while(!!Em);if(!Fu)Fu=
BA;BU=Ee;switch(GA){case 9:case 11:BU=[].concat(BU.slice(0,3),BU[1]+It);break;case
10:case 12:BU=B.t3(BU,BU[3]-It);break;case 5:case 7:BU=B.t1(BU,BU[0]+Ia);break;case
6:case 8:BU=[].concat(BU[2]-Ia,BU.slice(1,4));break;default:;}}if(((N.F&0x400)===
0x400)){if(!!N.A6&&(N.A6.Gb!==BA))N.A6=null;if((!N.A6&&DX)&&((N.Fe!==G9)||!!CC))
N.G_(BU,BA);}if(!!N.A6){if(Jr&&!((N.F&0x400)===0x400))N.Ht(E6,0);if(DX&&((N.F&0x400
)===0x400)){var BF=N.Ht(B.tz(BU,DY),CC);if(((N.F&0x1)===0x1)){var Bb=AM;switch(CC
){case 3:Bb=[BF[0]+GB,Bb[1]];break;case 4:Bb=[-BF[0]-GB,Bb[1]];break;case 1:Bb=[
Bb[0],BF[1]+GC];break;case 2:Bb=[Bb[0],-BF[1]-GC];break;default:;}DY=B.tx(DY,Bb);
}}}if(((N.F&0x200)===0x200)){if(DX)B.lq(BA.Fk,BA);DX=((N.F&0x1000)===0x1000);BA=(
C.GV.isPrototypeOf(N)?N:null);if(DX){N.F=N.F&~0x1000;Ee=B.tz(BA.G,BA.D_);BU=Ee;DY=
AM;GA=BA.HD;CC=GA;GB=BA.Space+BA.LO;GC=BA.Space+BA.LP;DX=!((Ee[0]>=Ee[2])||(Ee[1
]>=Ee[3]));Fu=null;switch(GA){case 9:case 10:CC=3;break;case 11:case 12:CC=4;break;
case 5:case 6:CC=1;break;case 7:case 8:CC=2;break;default:;}}if(DX){this.AL(BA.G
);}}if(N===Fu){switch(GA){case 9:case 11:DY=[0,(DY[1]+(BU[3]-BU[1]))+GC];break;case
10:case 12:DY=[0,(DY[1]-(BU[3]-BU[1]))-GC];break;case 5:case 7:DY=[(DY[0]+(BU[2]-
BU[0]))+GB,0];break;case 6:case 8:DY=[(DY[0]-(BU[2]-BU[0]))-GB,0];break;default:;
}Fu=null;}N=N.BL;}if(DX)B.lq(BA.Fk,BA);this.F=this.F&~0x2000;this.DU([E6[2]-E6[0
],E6[3]-E6[1]]);},CM:function(Z){var A;var Op=((this.F&0x1000)===0x1000);if(((this.
F&0x4000)===0x4000)){this.F=this.F&~0x4000;this.Oc();}if(((this.F&0x8000)===0x8000
)||Op){this.F=this.F&~0x8000;this.EC(this.F);}},Gl:function(E){var A;if(!!E&&(E.
L!==this))throw new Error(Dj);if(!!E&&!((E.F&0x14)===0x14))E=null;if(!!E&&((E.F&
0x10000)===0x10000))E=null;if(E===this.Dr)return;if(!!this.Dr)this.Dr.BW(0x0,0x60
);this.Dr=E;if(!!E){if(((this.F&0x40)===0x40))E.BW(0x60,0x0);else E.BW(0x20,0x0);
}},Bd:function(E){if(E)this.BW(0x10,0x0);else this.BW(0x0,0x10);},Ar:function(E){
var A;if(E>255)E=255;if(E<0)E=0;if(E===this.Be)return;this.Be=E;if(!!this.L&&((this.
F&0x1)===0x1))this.L.AL(this.G);},NU:function(JK){this.Ar(JK);},IR:function(Ea){
var tmp=this;while(!!tmp){Ea=B.tw(Ea,tmp.G.slice(0,2));tmp=tmp.L;}return Ea;},DispatchEvent:
function(Aw){var A;var N=this.Dr;var Ag=(C.M.isPrototypeOf(N)?N:null);var AJ=null;
var Jx=!!this.F0&&(!!this.F0.Jn||!!this.F0.DE);if(!!N&&((((N.F&0x10000)===0x10000
)||((N.F&0x40000)===0x40000))||((N.F&0x20000)===0x20000))){N=null;Ag=null;}if(!!
this.Fs&&!Jx)AJ=this.Fs.CW.DispatchEvent(Aw);if(!AJ&&!!Ag)AJ=Ag.DispatchEvent(Aw
);else if(!AJ&&!!N)AJ=N.BZ(Aw);if(!AJ)AJ=this.BZ(Aw);if(!AJ)AJ=this.JD(Aw);return AJ;
},BroadcastEventAtPosition:function(Aw,KA,aFilter){var A;var N=this.CG;var AJ=null;
while(!!N&&!AJ){if((!aFilter||((A=aFilter)&&((N.F&A)===A)))&&B.qu(N.GetExtent(),
KA)){var Ag=(C.M.isPrototypeOf(N)?N:null);if(!!Ag)AJ=Ag.BroadcastEventAtPosition(
Aw,B.tw(KA,Ag.G.slice(0,2)),aFilter);else AJ=N.BZ(Aw);}N=N.AY;}if(!AJ)AJ=this.BZ(
Aw);return AJ;},BroadcastEvent:function(Aw,aFilter){var A;var N=this.CG;var AJ=null;
while(!!N&&!AJ){if(!aFilter||((A=aFilter)&&((N.F&A)===A))){var Ag=(C.M.isPrototypeOf(
N)?N:null);if(!!Ag)AJ=Ag.BroadcastEvent(Aw,aFilter);else AJ=N.BZ(Aw);}N=N.AY;}if(
!AJ)AJ=this.BZ(Aw);if(!AJ)AJ=this.JD(Aw);return AJ;},DU:function(aSize){},EC:function(
Dz){},BG:function(){this.F=this.F|0x8000;B.lq([this,this.CM],this);},AL:function(
AD){var A;var Ag=this;while(!!Ag&&!((AD[0]>=AD[2])||(AD[1]>=AD[3]))){var FW=Ag.Dl;
if(!Ag.L&&(Ag!==this)){Ag.AL(AD);return;}if(!!FW){var Jq=false;var N_=FW.DN;if(Jq
)FW.DN=[0,0,(A=Ag.G)[2]-A[0],A[3]-A[1]];else FW.DN=B.qR(FW.DN,AD);if(!B.tm(N_,FW.
DN)){B.qw(Ag,0);B.qw(FW,0);}}if(!((Ag.F&0x1)===0x1))return;AD=B.il(B.tz(AD,Ag.G.
slice(0,2)),Ag.G);Ag=Ag.L;}},Bc:function(aArg){this.BG();},JW:function(Ae,aFilter
){var A;if(!Ae||(Ae.L!==this))return null;var Fw=Ae.BL;var Fy=Ae.AY;var Hd=0x10000;
if(((aFilter&0x10000)===0x10000))Hd=0x0;while(!!Fw||!!Fy){if((!!Fw&&(!aFilter||((
A=aFilter)&&((Fw.F&A)===A))))&&(!Hd||!((A=Hd)&&((Fw.F&A)===A))))return Fw;if((!!
Fy&&(!aFilter||((A=aFilter)&&((Fy.F&A)===A))))&&(!Hd||!((A=Hd)&&((Fy.F&A)===A)))
)return Fy;if(!!Fw)Fw=Fw.BL;if(!!Fy)Fy=Fy.AY;}return null;},GY:function(Ae){var A;
if(!Ae)throw new Error(FO);if(Ae.L!==this)throw new Error(DV);if((((Ae.F&0x401)===
0x401)&&!!Ae.AY)&&!!Ae.A6){Ae.AY.F=Ae.AY.F|0x800;this.F=this.F|0x4000;B.lq([this
,this.CM],this);}if(((Ae.F&0x200)===0x200)){if(!!Ae.AY)Ae.AY.F=Ae.AY.F|0x800;this.
F=this.F|0x4000;B.lq([this,this.CM],this);}Ae.A6=null;if(this.Dr===Ae)this.Gl(this.
JW(Ae,0x14));if(!!Ae.AY)Ae.AY.BL=Ae.BL;if(!!Ae.BL)Ae.BL.AY=Ae.AY;if(this.DE===Ae
)this.DE=Ae.BL;if(this.CG===Ae)this.CG=Ae.AY;Ae.L=null;Ae.BL=null;Ae.AY=null;if(((
Ae.F&0x1)===0x1))this.AL(Ae.GetExtent());},O:function(Ae,Gw){var A;if(!Ae)throw new
Error(FP);if(!!Ae.L)throw new Error(FQ);var Cx=null;var Ix=Ae.G0;if(((Gw<0)&&!!this.
CG)&&(this.CG.G0>=Ix)){Cx=this.CG;Gw=Gw+1;}while((((Gw<0)&&!!Cx)&&!!Cx.AY)&&(Cx.
AY.G0>=Ix)){Cx=Cx.AY;Gw=Gw+1;}if((!Cx&&!!this.CG)&&(this.CG.G0>Ix))Cx=this.CG;while((
!!Cx&&!!Cx.AY)&&(Cx.AY.G0>Ix))Cx=Cx.AY;if(!Cx){Ae.L=this;Ae.AY=this.CG;if(!!this.
CG)this.CG.BL=Ae;if(!this.DE)this.DE=Ae;this.CG=Ae;}else{Ae.L=this;Ae.AY=Cx.AY;Ae.
BL=Cx;Cx.AY=Ae;if(!!Ae.AY)Ae.AY.BL=Ae;else this.DE=Ae;}if(((Ae.F&0x1)===0x1))this.
AL(Ae.GetExtent());if(((!this.Dr&&((Ae.F&0x4)===0x4))&&((Ae.F&0x10)===0x10))&&!((
Ae.F&0x10000)===0x10000))this.Gl(Ae);if(((Ae.F&0x401)===0x401)){Ae.F=Ae.F|0x800;
this.F=this.F|0x4000;B.lq([this,this.CM],this);}if(((Ae.F&0x200)===0x200)){Ae.F=
Ae.F|0x800;this.F=this.F|0x4000;B.lq([this,this.CM],this);}},Lx:function(){return this.
Be;},_Init:function(aArg){C.BO._Init.call(this,aArg);this.__proto__=C.M;this.F=0x1F;
this.Bc(aArg);},_Mark:function(D){var A;C.BO._Mark.call(this,D);if((A=this.DE)&&(
A._cycle!=D))A._Mark(A._cycle=D);if((A=this.CG)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Ik)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Dl)&&(A._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Fs)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
F0)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Dr)&&(A._cycle!=D))A._Mark(A._cycle=
D);},_className:"Core::Group"};C.Root={CF:null,E8:null,Eb:null,A5:B.tA(10,null,null
),Gx:null,Dx:null,E2:null,Hr:0,Jj:0,A$:0,BT:B.tA(10,0,null),Ib:B.tA(10,B.qy,null
),DD:B.tA(10,0,null),EG:B.tA(10,B.qx,null),Gz:B.tA(10,0,null),E1:B.tA(10,B.qx,null
),DC:B.tA(10,B.qx,null),CV:B.tA(10,B.qx,null),Ec:B.tA(10,B.qx,null),AE:0,If:0,Ie:
0,Im:B.tA(3,B.qy,null),KO:0,Cg:B.tA(4,0,null),BM:B.tA(4,B.qy,null),BE:0,Fb:8,Lc:
250,Ft:0,E7:0,Js:true,Il:false,IN:function(){return this;},DK:function(Bi,aClip,
aOffset,A_,aBlend){var fullScreenUpdate=false;fullScreenUpdate=B.jI;if(!fullScreenUpdate
)Bi.IL(aClip,B.tz(B.tz(aClip,aOffset),this.G.slice(0,2)),0x00000000,0x00000000,0x00000000
,0x00000000,false);C.M.DK.call(this,Bi,aClip,aOffset,A_,aBlend);},BW:function(E0
,FT){var A;C.M.BW.call(this,E0,FT);if(!this.L&&(((E0&0x1)===0x1)||((FT&0x1)===0x1
)))this.AL([0,0,(A=this.G)[2]-A[0],A[3]-A[1]]);if(!this.L&&(((E0&0x2)===0x2)||((
FT&0x2)===0x2)))this.AL([0,0,(A=this.G)[2]-A[0],A[3]-A[1]]);},Gl:function(E){if((
E!==this.Dx)||!E)C.M.Gl.call(this,E);},Ar:function(E){var A;var Ga=this.Be;C.M.Ar.
call(this,E);if(((Ga!==this.Be)&&!this.L)&&((this.F&0x1)===0x1))this.AL([0,0,(A=
this.G)[2]-A[0],A[3]-A[1]]);},DispatchEvent:function(Aw){if((this.Jj>0)&&!!(C.KeyEvent.
isPrototypeOf(Aw)?Aw:null))return null;if(!!Aw){Aw.GL=!!this.A$;if(!!this.A$)Aw.
Bq=this.A$;}var AJ=null;if(!!this.Dx){AJ=this.Dx.DispatchEvent(Aw);if(!!AJ){this.
A$=0;return AJ;}}if(!!this.E8){AJ=this.E8.CW.DispatchEvent(Aw);if(!AJ)AJ=this.BZ(
Aw);if(!AJ)AJ=this.JD(Aw);this.A$=0;return AJ;}AJ=C.M.DispatchEvent.call(this,Aw
);this.A$=0;return AJ;},BroadcastEvent:function(Aw,aFilter){if(!!Aw){Aw.GL=!!this.
A$;if(!!this.A$)Aw.Bq=this.A$;}var AJ=C.M.BroadcastEvent.call(this,Aw,aFilter);this.
A$=0;return AJ;},AL:function(AD){var A;if(this.Hr>0)throw new Error(I$);if(!!this.
Dl&&!this.L){if(((A=this.Dl.DN)[0]>=A[2])||(A[1]>=A[3])){B.qw(this,0);B.qw(this.
Dl,0);}var Jq=false;if(Jq)this.Dl.DN=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];else this.
Dl.DN=B.qR(this.Dl.DN,AD);}var fullScreenUpdate=false;fullScreenUpdate=B.jI;if(fullScreenUpdate
)AD=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];if(!!this.L){C.M.AL.call(this,AD);return;
}AD=B.il(B.tz(AD,this.G.slice(0,2)),this.G);if((AD[0]>=AD[2])||(AD[1]>=AD[3]))return;
var K;for(K=0;K<this.BE;K=K+1)if(!(((A=B.il(this.BM.Get(K),AD))[0]>=A[2])||(A[1]>=
A[3]))){this.BM.Set(K,B.qR(this.BM.Get(K),AD));this.Cg.Set(K,B.s9(this.BM.Get(K)
));return;}if(this.BE<3){this.BM.Set(this.BE,AD);this.Cg.Set(this.BE,B.s9(AD));this.
BE=this.BE+1;return;}var Ba;var CE;var Ha=0;var Hb=0;var KB=2147483647;this.BM.Set(
this.BE,AD);this.Cg.Set(this.BE,B.s9(AD));for(Ba=0;Ba<=this.BE;Ba=Ba+1)for(CE=Ba+
1;CE<=this.BE;CE=CE+1){var Iz=B.s9(B.qR(this.BM.Get(Ba),this.BM.Get(CE)));var KS=((
Iz<<8)/(this.Cg.Get(Ba)+this.Cg.Get(CE)))|0;if(KS<KB){KB=KS;Ha=Ba;Hb=CE;}}this.BM.
Set(Ha,B.qR(this.BM.Get(Ha),this.BM.Get(Hb)));this.Cg.Set(Ha,B.s9(this.BM.Get(Ha
)));if(Hb!==this.BE){this.BM.Set(Hb,this.BM.Get(this.BE));this.Cg.Set(Hb,this.Cg.
Get(this.BE));}},N0:function(){var Cc=B._NewObject(C.HA,0);Cc.GL=!!this.A$;if(!!
this.A$)Cc.Bq=this.A$;return Cc;},FX:function(){var Cc=B._NewObject(C.Hy,0);Cc.GL=
!!this.A$;if(!!this.A$)Cc.Bq=this.A$;return Cc;},Gy:function(){var Cc=B._NewObject(
C.Hz,0);Cc.GL=!!this.A$;if(!!this.A$)Cc.Bq=this.A$;return Cc;},N1:function(Z){var
K;var F_=false;for(K=0;K<10;K=K+1)if(!!this.A5.Get(K)){var AV=this.CV.Get(K);var
Ag=this.A5.Get(K).L;while(!!Ag&&(Ag!==this)){AV=B.tw(AV,Ag.G.slice(0,2));Ag=Ag.L;
}if(!Ag&&(this.A5.Get(K)!==this)){var tmp=this.A5.Get(K);this.AE=K;this.A5.Set(K
,null);tmp.BZ(this.FX().InitializeUp(K,this.E1.Get(K),this.EG.Get(K),this.DD.Get(
K),this.BT.Get(K)+1,this.DC.Get(K),false,this.CV.Get(K),this.Ec.Get(K)));if(tmp===
this.Eb)this.Eb=null;this.BroadcastEvent(this.Gy().InitializeUp(K,this.BT.Get(K)+
1,false,tmp,this.CV.Get(K)),0x18);}else{this.DD.Set(K,(this.E2.Bq-this.Gz.Get(K)
)|0);if(this.DD.Get(K)<10)this.DD.Set(K,10);this.AE=K;this.A5.Get(K).BZ(this.FX(
).InitializeHold(K,AV,this.EG.Get(K),this.DD.Get(K),this.BT.Get(K)+1,this.DC.Get(
K),this.CV.Get(K),this.Ec.Get(K)));F_=true;}}if(!F_)this.E2.Bd(false);},GetFPS:function(
){var ticksCount=0;var KL=0;ticksCount=((new Date).getTime()-B.qt)|0;if(!!this.If&&(
ticksCount>this.If))KL=((this.Ie*1000)/((ticksCount-this.If)|0))|0;this.Ie=0;this.
If=ticksCount;return KL;},Update:function(){var A;if(!this.Gx){this.Gx=B._NewObject(
B.Graphics.Canvas,0);this.Gx.IZ([(A=this.G)[2]-A[0],A[3]-A[1]]);}this.Gx.Update(
);return this.UpdateGE20(this.Gx);},UpdateGE20:function(Bi){if(!this.BeginUpdate(
))return BD;var EK=this.UpdateCanvas(Bi,AM);this.EndUpdate();return EK;},EndUpdate:
function(){if(this.BE>0){this.Ie=this.Ie+1;this.BE=0;}},UpdateCanvas:function(Bi
,aOffset){var A;var EK=BD;var NY=[].concat(aOffset,B.tx(Bi.FrameSize,aOffset));var
K;var Ba=this.BE;this.Hr=this.Hr+1;for(K=0;(K<Ba)&&(K<4);K=K+1){if(this.Cg.Get(K
)>0){this.DK(Bi,B.ty(this.BM.Get(K),aOffset),[-aOffset[0],-aOffset[1]],255,true);
EK=B.qR(EK,B.il(NY,this.BM.Get(K)));}else Ba=Ba+1;}this.Hr=this.Hr-1;if(!((EK[0]>=
EK[2])||(EK[1]>=EK[3])))return B.ty(EK,aOffset);else return EK;},GetUpdateRegion:
function(H7){var K;var Ba=this.BE;if(H7<0)return BD;for(K=0;(K<Ba)&&(K<4);K=K+1){
if(!this.Cg.Get(K)){Ba=Ba+1;H7=H7+1;}else if(K===H7)return this.BM.Get(K);}return BD;
},BeginUpdate:function(){var Ob=true;var fullScreenUpdate=false;var K;if((!Ob&&!
fullScreenUpdate)&&(this.BE>0)){var KZ=B.tA(3,B.qy,null);var JI=this.BE;for(K=0;
K<JI;K=K+1)KZ.Set(K,this.BM.Get(K));for(K=0;K<this.KO;K=K+1)this.AL(this.Im.Get(
K));for(K=0;K<JI;K=K+1)this.Im.Set(K,KZ.Get(K));this.KO=JI;}var Ba;var CE;for(Ba=
0;Ba<(this.BE-1);Ba=Ba+1)if(this.Cg.Get(Ba)>0)for(CE=Ba+1;CE<this.BE;CE=CE+1)if(
this.Cg.Get(CE)>0){var Iz=B.s9(B.qR(this.BM.Get(Ba),this.BM.Get(CE)));if(((Iz-this.
Cg.Get(Ba))-this.Cg.Get(CE))<0){this.BM.Set(Ba,B.qR(this.BM.Get(Ba),this.BM.Get(
CE)));this.Cg.Set(Ba,Iz);this.Cg.Set(CE,0);}}for(K=this.BE-1;K>=0;K=K-1)if(!this.
Cg.Get(K))this.BE=this.BE-1;return this.BE;},DoesNeedUpdate:function(){if(this.BE>
0)return true;return false;},Initialize:function(aSize){this.H([].concat(AM,aSize
));if(this.Js)this.F=this.F|0x60;else this.F=this.F|0x20;this.AL(this.G);return this;
},SetRootFocus:function(Jf){if(Jf===this.Js)return false;this.Js=Jf;if(!Jf){if(!
!this.Dx)this.Dx.BW(0x0,0x40);if(!!this.E8)this.E8.CW.BW(0x0,0x40);else this.BW(
0x0,0x40);}else{if(!!this.E8)this.E8.CW.BW(0x40,0x0);else this.BW(0x40,0x0);if(!
!this.Dx)this.Dx.BW(0x40,0x0);}return true;},SetUserInputTimestamp:function(NX){
this.A$=NX;},DriveKeyboardHitting:function(By,FS,CT){var A;var JE=!!this.CF;if(!
!this.CF&&((!CT||(this.Ft!==By))||(this.E7!==FS))){var Cc=null;var N=(C.CS.isPrototypeOf(
A=this.CF)?A:null);var DZ=(C.HG.isPrototypeOf(A=this.CF)?A:null);if(!!this.Ft)Cc=
B._NewObject(C.KeyEvent,0).Initialize(this.Ft,false);if(this.E7!==0x00)Cc=B._NewObject(
C.KeyEvent,0).Initialize2(this.E7,false);if(!!DZ)DZ.BZ(Cc);else if(!!N)N.BZ(Cc);
this.Ft=0;this.E7=0x00;this.CF=null;}if(!!this.CF){var Cc=null;var N=(C.CS.isPrototypeOf(
A=this.CF)?A:null);var DZ=(C.HG.isPrototypeOf(A=this.CF)?A:null);if(!!By)Cc=B._NewObject(
C.KeyEvent,0).Initialize(By,true);if(this.E7!==0x00)Cc=B._NewObject(C.KeyEvent,0
).Initialize2(FS,true);if(!!DZ)DZ.BZ(Cc);else if(!!N)N.BZ(Cc);}if(this.Il&&((!CT||(
this.Ft!==By))||(this.E7!==FS))){this.Ft=0;this.E7=0x00;this.Il=false;}if((!this.
CF&&CT)&&(this.Jj>0)){this.Ft=By;this.E7=FS;this.Il=true;}if((!this.CF&&CT)&&!this.
Il){if(!!By)this.CF=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize(By,
true));if(FS!==0x00)this.CF=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize2(
FS,true));if(!(C.HG.isPrototypeOf(A=this.CF)?A:null)&&!(C.CS.isPrototypeOf(A=this.
CF)?A:null))this.CF=null;this.Ft=By;this.E7=FS;JE=JE||!!this.CF;}this.A$=0;return JE;
},DriveCursorMovement:function(Bz){return this.DriveMultiTouchMovement(this.AE,Bz
);},DriveMultiTouchMovement:function(R,Bz){if((R<0)||(R>9)){this.A$=0;return false;
}var C9=B.tw(Bz,this.CV.Get(R));this.CV.Set(R,Bz);if(!this.A5.Get(R)||B.tl(C9,AM
)){this.A$=0;return false;}var AV=Bz;var Ag=this.A5.Get(R).L;while(!!Ag&&(Ag!==this
)){AV=B.tw(AV,Ag.G.slice(0,2));Ag=Ag.L;}if(!Ag&&(this.A5.Get(R)!==this)){var tmp=
this.A5.Get(R);this.AE=R;this.A5.Set(R,null);tmp.BZ(this.FX().InitializeUp(R,this.
E1.Get(R),this.EG.Get(R),this.DD.Get(R),this.BT.Get(R)+1,this.DC.Get(R),false,this.
CV.Get(R),this.Ec.Get(R)));if(tmp===this.Eb)this.Eb=null;this.BroadcastEvent(this.
Gy().InitializeUp(R,this.BT.Get(R)+1,false,tmp,Bz),0x18);}else{this.E1.Set(R,AV);
this.AE=R;this.A5.Get(R).BZ(this.N0().Initialize(R,AV,this.EG.Get(R),C9,this.DD.
Get(R),this.BT.Get(R)+1,this.DC.Get(R),Bz,this.Ec.Get(R)));}this.A$=0;return true;
},DriveCursorHitting:function(CT,R,Bz){return this.DriveMultiTouchHitting(CT,R,Bz
);},DriveMultiTouchHitting:function(CT,R,Bz){var A;if((R<0)||(R>9)){this.A$=0;return false;
}var ticksCount=this.A$;var FZ=[].concat([-this.Fb,-this.Fb],[this.Fb+1,this.Fb+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.qt)|0;}var Oh=this.A$;this.
DriveMultiTouchMovement(R,Bz);Bz=this.CV.Get(R);this.A$=Oh;if(CT)this.Ec.Set(R,Bz
);if((CT&&!this.A5.Get(R))&&!this.Jj){var CD=null;var AV=Bz;if(B.qu(this.Ib.Get(
R),Bz)&&((ticksCount-this.Gz.Get(R))<=(((A=this.Lc)<0)?A+0x100000000:A)))this.BT.
Set(R,this.BT.Get(R)+1);else this.BT.Set(R,0);this.Ib.Set(R,B.tz(FZ,Bz));this.Gz.
Set(R,ticksCount);if((!!this.Dx&&!!this.Dx.L)&&((this.Dx.F&0x18)===0x18)){var Al=
B.tz(FZ,this.Dx.L.IR(Bz));CD=this.Dx.EQ(Al,R,this.BT.Get(R)+1,null,0x0);}if(!CD){
if(!!this.Eb&&!!this.Eb.L){if(((this.Eb.F&0x8)===0x8)&&((this.Eb.F&0x10)===0x10)
){var Al=B.tz(FZ,this.Eb.L.IR(Bz));CD=this.Eb.EQ(Al,R,this.BT.Get(R)+1,null,0x0);
}}else if(!!this.E8)CD=this.EQ(B.tz(FZ,Bz),R,this.BT.Get(R)+1,this.E8.CW,0x0);else
CD=this.EQ(B.tz(FZ,Bz),R,this.BT.Get(R)+1,null,0x0);}if(!!CD){this.BroadcastEvent(
this.Gy().InitializeDown(R,this.BT.Get(R)+1,false,CD.CS,Bz),0x18);this.A5.Set(R,
CD.CS);this.DC.Set(R,CD.EV);}else{this.A5.Set(R,null);this.DC.Set(R,AM);this.A$=
0;return false;}var Ag=CD.CS.L;while(!!Ag&&(Ag!==this)){AV=B.tw(AV,Ag.G.slice(0,
2));Ag=Ag.L;}this.EG.Set(R,AV);this.E1.Set(R,AV);this.DD.Set(R,0);this.E2.Bd(true
);this.AE=R;this.A5.Get(R).BZ(this.FX().InitializeDown(R,AV,this.BT.Get(R)+1,this.
DC.Get(R),false,Bz));this.A$=0;return true;}if(!CT&&!!this.A5.Get(R)){var AV=Bz;
var Ag=this.A5.Get(R).L;while(!!Ag&&(Ag!==this)){AV=B.tw(AV,Ag.G.slice(0,2));Ag=
Ag.L;}if(!Ag)AV=this.E1.Get(R);this.AE=R;var tmp=this.A5.Get(R);this.A5.Set(R,null
);tmp.BZ(this.FX().InitializeUp(R,AV,this.EG.Get(R),this.DD.Get(R),this.BT.Get(R
)+1,this.DC.Get(R),false,Bz,this.Ec.Get(R)));this.BroadcastEvent(this.Gy().InitializeUp(
R,this.BT.Get(R)+1,false,tmp,Bz),0x18);this.A$=0;return true;}this.A$=0;return false;
},J6:function(EE,Kz,FV){var FZ=[].concat([-this.Fb,-this.Fb],[this.Fb+1,this.Fb+
1]);if(EE===this)EE=null;if(!this.A5.Get(this.AE))return;var CD;CD=this.EQ(B.tz(
FZ,this.CV.Get(this.AE)),this.AE,1,EE,FV);if(!!CD&&(this.A5.Get(this.AE)!==CD.CS
))this.JR(CD.CS,CD.EV);if(!CD&&(this.A5.Get(this.AE)!==Kz))this.JR(Kz,AM);},JR:function(
EE,Dy){if(!this.A5.Get(this.AE)||(this.A5.Get(this.AE)===EE))return;var tmp=this.
A5.Get(this.AE);this.A5.Set(this.AE,null);tmp.BZ(this.FX().InitializeUp(this.AE,
this.E1.Get(this.AE),this.EG.Get(this.AE),this.DD.Get(this.AE),this.BT.Get(this.
AE)+1,this.DC.Get(this.AE),true,this.CV.Get(this.AE),this.Ec.Get(this.AE)));this.
BroadcastEvent(this.Gy().InitializeUp(this.AE,this.BT.Get(this.AE)+1,true,tmp,this.
CV.Get(this.AE)),0x18);var AV=this.CV.Get(this.AE);var Ag=null;if(!!EE)Ag=EE.L;while(
!!Ag&&(Ag!==this)){AV=B.tw(AV,Ag.G.slice(0,2));Ag=Ag.L;}if(!Ag&&(EE!==this)){this.
A5.Set(this.AE,null);return;}this.BroadcastEvent(this.Gy().InitializeDown(this.AE
,this.BT.Get(this.AE)+1,true,EE,this.CV.Get(this.AE)),0x18);var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;this.A5.Set(this.AE,EE);this.DC.Set(this.AE,Dy);this.
EG.Set(this.AE,AV);this.E1.Set(this.AE,AV);this.BT.Set(this.AE,0);this.DD.Set(this.
AE,0);this.Gz.Set(this.AE,ticksCount);this.Ec.Set(this.AE,this.CV.Get(this.AE));
this.A5.Get(this.AE).BZ(this.FX().InitializeDown(this.AE,AV,this.BT.Get(this.AE)+
1,this.DC.Get(this.AE),true,this.Ec.Get(this.AE)));},_Init:function(aArg){C.M._Init.
call(this,aArg);C.Timer._Init.call(this.E2={J:this},0);(this.A5=[]).__proto__=C.
Root.A5;(this.BT=[]).__proto__=C.Root.BT;(this.Ib=[]).__proto__=C.Root.Ib;(this.
DD=[]).__proto__=C.Root.DD;(this.EG=[]).__proto__=C.Root.EG;(this.Gz=[]).__proto__=
C.Root.Gz;(this.E1=[]).__proto__=C.Root.E1;(this.DC=[]).__proto__=C.Root.DC;(this.
CV=[]).__proto__=C.Root.CV;(this.Ec=[]).__proto__=C.Root.Ec;(this.Im=[]).__proto__=
C.Root.Im;(this.Cg=[]).__proto__=C.Root.Cg;(this.BM=[]).__proto__=C.Root.BM;this.
__proto__=C.Root;this.F=0x7F;this.E2.GS(50);this.E2.FK=[this,this.N1];},_Done:function(
){this.__proto__=C.M;this.E2._Done();C.M._Done.call(this);},_ReInit:function(){C.
M._ReInit.call(this);this.E2._ReInit();},_Mark:function(D){var A;C.M._Mark.call(
this,D);if((A=this.CF)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.E8)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Eb)&&(A._cycle!=D))A._Mark(A._cycle=D);B.ts(
this.A5,D);if((A=this.Gx)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Dx)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.E2)._cycle!=D)A._Mark(A._cycle=D);},_className:
"Core::Root"};C.Event={Bq:0,GL:false,HF:function(){var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;return ticksCount;},Bc:function(aArg){this.Bq=this.HF(
);},_Init:function(aArg){this.__proto__=C.Event;this.Bc(aArg);B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::Event"};C.KeyEvent={BC:0,AK:0,Down:false,Initialize2:function(By,CT){this.
BC=0;this.AK=By;this.Down=CT;if((By>=0x30)&&(By<=0x39))this.BC=(10+By)-48;if((By>=
0x41)&&(By<=0x5A))this.BC=(105+By)-65;if((By>=0x61)&&(By<=0x7A))this.BC=(105+By)-
97;if(By===0x20)this.BC=131;if(!this.BC)switch(By){case 0x2B:this.BC=132;break;case
0x2D:this.BC=133;break;case 0x2A:this.BC=134;break;case 0x2F:this.BC=135;break;case
0x3D:this.BC=136;break;case 0x2E:this.BC=137;break;case 0x2C:this.BC=138;break;case
0x3A:this.BC=139;break;case 0x3B:this.BC=140;break;default:;}return this;},Initialize:
function(By,CT){this.BC=By;this.Down=CT;this.AK=0x00;var Jl=By-10;var Jk=By-105;
if((Jl>=0)&&(Jl<=9))this.AK=(48+Jl)&0xFFFF;if((Jk>=0)&&(Jk<=25))this.AK=(65+Jk)&
0xFFFF;if(By===131)this.AK=0x20;if(this.AK===0x00)switch(By){case 132:this.AK=0x2B;
break;case 133:this.AK=0x2D;break;case 134:this.AK=0x2A;break;case 135:this.AK=0x2F;
break;case 136:this.AK=0x3D;break;case 137:this.AK=0x2E;break;case 138:this.AK=0x2C;
break;case 139:this.AK=0x3A;break;case 140:this.AK=0x3B;break;default:;}return this;
},Lo:function(Ky){switch(Ky){case 141:return((this.AK>=0x41)&&(this.AK<=0x5A))||((
this.AK>=0x61)&&(this.AK<=0x7A));case 142:return(((this.AK>=0x41)&&(this.AK<=0x5A
))||((this.AK>=0x61)&&(this.AK<=0x7A)))||((this.AK>=0x30)&&(this.AK<=0x39));case
143:return(this.AK>=0x30)&&(this.AK<=0x39);case 144:return(((this.AK>=0x41)&&(this.
AK<=0x46))||((this.AK>=0x61)&&(this.AK<=0x66)))||((this.AK>=0x30)&&(this.AK<=0x39
));case 145:return this.AK!==0x00;case 146:return(this.AK===0x00)&&!!this.BC;case
147:return(((this.BC===6)||(this.BC===7))||(this.BC===4))||(this.BC===5);case 148:
return(this.AK!==0x00)||!!this.BC;default:;}return Ky===this.BC;},_Init:function(
aArg){C.Event._Init.call(this,aArg);this.__proto__=C.KeyEvent;},_className:"Core::KeyEvent"
};C.Hz={HW:null,Ds:B.qx,Du:0,Ci:0,Down:false,Eo:false,InitializeUp:function(R,CU
,FR,Ji,DW){this.Down=false;this.Ci=R;this.Du=CU;this.Ds=DW;this.HW=Ji;this.Eo=FR;
return this;},InitializeDown:function(R,CU,FR,Ji,DW){this.Down=true;this.Ci=R;this.
Du=CU;this.Ds=DW;this.HW=Ji;this.Eo=FR;return this;},_Init:function(aArg){C.Event.
_Init.call(this,aArg);this.__proto__=C.Hz;},_Mark:function(D){var A;C.Event._Mark.
call(this,D);if((A=this.HW)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"
};C.Hy={ET:B.qx,Ds:B.qx,Du:0,Cs:0,DM:B.qx,CY:B.qx,Ci:0,Down:false,Eo:false,InitializeHold:
function(R,Fo,G3,G4,CU,Dy,DW,G2){this.Down=true;this.Ci=R;this.CY=B.tx(Fo,Dy);this.
DM=B.tx(G3,Dy);this.Cs=G4;this.Du=CU;this.Ds=DW;this.ET=G2;return this;},InitializeUp:
function(R,Fo,G3,G4,CU,Dy,FR,DW,G2){this.Down=false;this.Ci=R;this.CY=B.tx(Fo,Dy
);this.DM=B.tx(G3,Dy);this.Cs=G4;this.Du=CU;this.Eo=FR;this.Ds=DW;this.ET=G2;return this;
},InitializeDown:function(R,Fo,CU,Dy,FR,DW){this.Down=true;this.Ci=R;this.CY=B.tx(
Fo,Dy);this.DM=B.tx(Fo,Dy);this.Cs=0;this.Du=CU;this.Eo=FR;this.Ds=DW;this.ET=DW;
return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=
C.Hy;},_className:"Core::CursorEvent"};C.HA={ET:B.qx,Ds:B.qx,Du:0,Cs:0,EV:B.qx,DM:
B.qx,CY:B.qx,Ci:0,Initialize:function(R,Fo,G3,aOffset,G4,NV,Dy,DW,G2){this.Ci=R;
this.CY=B.tx(Fo,Dy);this.DM=B.tx(G3,Dy);this.EV=aOffset;this.Cs=G4;this.Du=NV;this.
Ds=DW;this.ET=G2;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg
);this.__proto__=C.HA;},_className:"Core::DragEvent"};C.GV={Fk:null,D_:B.qx,LP:0
,LO:0,Space:0,HD:0,DK:function(Bi,aClip,aOffset,A_,aBlend){},H:function(E){var A;
if(B.tm(E,this.G))return;var Fx=[(A=this.G)[2]-A[0],A[3]-A[1]];var Io=[E[2]-E[0]
,E[3]-E[1]];var Gf=!B.tl(Fx,Io);var C9=B.tw(E.slice(0,2),this.G.slice(0,2));if(!
B.tl(C9,AM)&&!Gf){var N=this.BL;while(!!N&&!((N.F&0x200)===0x200)){if(((N.F&0x400
)===0x400)){var tmp=((N.F&0x100)===0x100);N.IT(C9,tmp);}N=N.BL;}B.lq(this.Fk,this
);}if((Gf&&(Fx[0]>0))&&(Fx[1]>0)){var As=B.tz(this.G,this.D_);var N=this.BL;var G9=
0x14;while(!!N&&!((N.F&0x200)===0x200)){if(((N.F&0x400)===0x400)){if(!!N.A6&&(N.
A6.Gb!==this))N.A6=null;if(!N.A6&&((N.Fe!==G9)||!!this.HD))N.G_(As,this);}N=N.BL;
}B.lq(this.Fk,this);}C.BO.H.call(this,E);if(!!this.L&&Gf){this.F=this.F|0x1000;if(
!((this.L.F&0x2000)===0x2000)){this.L.F=this.L.F|0x4000;B.lq([A=this.L,A.CM],this
);}}},_Init:function(aArg){C.BO._Init.call(this,aArg);this.__proto__=C.GV;this.F=
0x203;},_Mark:function(D){var A;C.BO._Mark.call(this,D);if((A=this.Fk)&&((A=A[0]
)._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Outline"};C.Ad={GN:null,Fh:
null,Fg:null,GR:null,Db:null,Da:null,Iy:0,AZ:0,Ci:0,Bq:0,Du:0,Cs:0,EV:B.qx,DM:B.
qx,CY:B.qx,LK:1000,HT:8,Fl:0,J0:1,IQ:-1,Lp:1,Lq:1,IJ:false,Down:false,Cj:false,Eo:
false,DK:function(Bi,aClip,aOffset,A_,aBlend){},BZ:function(Aw){var A;var AU=(C.
Hy.isPrototypeOf(Aw)?Aw:null);var BJ=(C.HA.isPrototypeOf(Aw)?Aw:null);var Ih=this.
Cj;var Iq;var GD;var KY;var E9;var KI;if(!AU&&!BJ)return null;Iq=(!!AU&&AU.Down)&&
!AU.Cs;GD=(!!AU&&AU.Down)&&(AU.Cs>0);KY=(!!AU&&AU.Down)&&(AU.Cs>50);E9=!!AU&&!AU.
Down;KI=!!BJ;if(!this.IJ){if((((this.Fl&0x20)===0x20)&&(this.AZ>0))&&(this.AZ<33554432
)){var Id=(C.Hz.isPrototypeOf(Aw)?Aw:null);if(((!!Id&&Id.Down)&&(Id.HW!==this))&&
B.qu(this.GetExtent(),this.L.IR(Id.Ds))){this.Iy=0x20;this.AZ=this.AZ|67108864;return null;
}}if(Iq){var Jz=0;var G8;this.AZ=this.AZ|(1<<AU.Ci);for(G8=this.AZ&4095;G8>0;G8=
G8>>1)if(!!(G8&1))Jz=Jz+1;if(Jz===this.J0)this.AZ=(this.AZ|16777216)|(4096<<AU.Ci
);}if(E9)this.AZ=(this.AZ&~(1<<AU.Ci))|33554432;if(KY&&(this.AZ<16777216))this.AZ=
this.AZ|67108864;if(E9&&AU.Eo)this.AZ=this.AZ|67108864;if(E9&&!(this.AZ&4095))this.
Iy=0x0;if(E9&&!(this.AZ&16777215))this.AZ=0;if(GD&&(this.AZ>=67108864))this.IN().
J6(null,null,this.Iy);if((GD&&!!(this.AZ&16777216))&&!!(this.AZ&33554432)){GD=false;
E9=true;}if(!!AU&&!(this.AZ&(4096<<AU.Ci)))return this;if(!!BJ&&!(this.AZ&(4096<<
BJ.Ci)))return this;if(E9&&!(this.AZ&16777216))return this;if(((Iq||KI)||GD)&&((
this.AZ<16777216)||(this.AZ>=33554432)))return this;if(E9)this.AZ=this.AZ&3758100479;
if(E9&&!(this.AZ&16777215))this.AZ=0;}if(!!AU){this.Down=Iq||GD;this.Cj=this.IP(
AU.CY);this.DM=AU.DM;this.CY=AU.CY;this.EV=AM;this.Cs=AU.Cs;this.Du=AU.Du;this.Eo=
AU.Eo;this.Ci=AU.Ci;this.Bq=AU.Bq;if(AU.Down&&!AU.Cs)Ih=false;}if(!!BJ){this.Down=
true;this.Cj=this.IP(BJ.CY);this.DM=BJ.DM;this.CY=BJ.CY;this.EV=BJ.EV;this.Cs=BJ.
Cs;this.Du=BJ.Du;this.Ci=BJ.Ci;this.Eo=false;this.Bq=BJ.Bq;(A=this.GN)?A[1].call(
A[0],this):null;}if((!!AU&&this.Down)&&!this.Cs)(A=this.Da)?A[1].call(A[0],this):
null;if((!!AU&&this.Down)&&(this.Cs>0))(A=this.GR)?A[1].call(A[0],this):null;if((
this.Down&&this.Cj)&&!Ih)(A=this.Fg)?A[1].call(A[0],this):null;if(((this.Down&&!
this.Cj)&&Ih)||((!this.Down&&this.Cj)&&Ih))(A=this.Fh)?A[1].call(A[0],this):null;
if(!!AU&&!this.Down)(A=this.Db)?A[1].call(A[0],this):null;if(!!this.Fl){var Fz=0x0;
if(((((this.Fl&0x10)===0x10)&&!!AU)&&AU.Down)&&(AU.Cs>=this.LK))Fz=0x10;if((((this.
Fl&0x1)===0x1)&&!!BJ)&&((BJ.Ds[1]-BJ.ET[1])<=-this.HT))Fz=0x1;if((((this.Fl&0x2)===
0x2)&&!!BJ)&&((BJ.Ds[1]-BJ.ET[1])>=this.HT))Fz=0x2;if((((this.Fl&0x4)===0x4)&&!!
BJ)&&((BJ.Ds[0]-BJ.ET[0])<=-this.HT))Fz=0x4;if((((this.Fl&0x8)===0x8)&&!!BJ)&&((
BJ.Ds[0]-BJ.ET[0])>=this.HT))Fz=0x8;if(!!Fz){this.Iy=Fz;this.IN().J6(null,this,Fz
);}}return this;},EQ:function(AD,R,CU,Gv,FV){var A;if(!!Gv&&(Gv!==this))return null;
if((CU<this.Lq)||(CU>this.Lp))return null;if((this.IQ>=0)&&(this.J0>1))return null;
if((this.IQ>=0)&&(R!==this.IQ))return null;if(!this.IJ&&(this.AZ>=33554432))return null;
if((!this.IJ&&(this.AZ>=16777216))&&!(this.AZ&(4096<<R)))return null;if(!!(FV&this.
Fl))return null;if(this.Ll()){var D1=B.il(AD,this.GetExtent());if(!((D1[0]>=D1[2
])||(D1[1]>=D1[3]))){var Fq=B.s_(AD);var C9=AM;if(Fq[0]<D1[0])C9=[D1[0]-Fq[0],C9[
1]];if(Fq[0]>=D1[2])C9=[(D1[2]-Fq[0])-1,C9[1]];if(Fq[1]<D1[1])C9=[C9[0],D1[1]-Fq[
1]];if(Fq[1]>=D1[3])C9=[C9[0],(D1[3]-Fq[1])-1];return B._NewObject(C.IG,0).Initialize(
this,C9);}}else{var A7=B.tA(9,B.qx,null);var K;A7.Set(0,B.s_(AD));A7.Set(1,A7.Get(
0));A7.Set(2,A7.Get(0));A7.Set(3,A7.Get(0));A7.Set(4,A7.Get(0));A7.Set(1,[AD[0],
A7.Get(1)[1]]);A7.Set(2,[A7.Get(2)[0],AD[1]]);A7.Set(3,[AD[2],A7.Get(3)[1]]);A7.
Set(4,[A7.Get(4)[0],AD[3]]);A7.Set(5,AD.slice(0,2));A7.Set(6,[AD[2],AD[1]]);A7.Set(
7,[AD[0],AD[3]]);A7.Set(8,AD.slice(2,4));for(K=0;K<9;K=K+1)if(this.IP(A7.Get(K))
)return B._NewObject(C.IG,0).Initialize(this,B.tw(A7.Get(K),A7.Get(0)));}return null;
},Bd:function(E){if(E)this.BW(0x10,0x0);else this.BW(0x0,0x10);},_Init:function(
aArg){C.GW._Init.call(this,aArg);this.__proto__=C.Ad;this.F=0x11B;},_Mark:function(
D){var A;C.GW._Mark.call(this,D);if((A=this.GN)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.Fh)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fg
)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.GR)&&((A=A[0])._cycle!=D)
)A._Mark(A._cycle=D);if((A=this.Db)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((
A=this.Da)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SimpleTouchHandler"
};C.HG={BL:null,Db:null,Da:null,GR:null,Ge:0,Bq:0,J5:0,Lj:148,BC:0,AK:0,Ex:true,
Down:false,I6:false,Hx:false,BZ:function(Aw){var A;if(!!Aw&&Aw.Lo(this.Lj)){this.
Down=Aw.Down;this.BC=Aw.BC;this.AK=Aw.AK;this.Bq=Aw.Bq;this.Hx=false;if(Aw.Down){
this.J5=this.Ge;this.I6=this.Ge>0;if(this.I6)(A=this.GR)?A[1].call(A[0],this):null;
else(A=this.Da)?A[1].call(A[0],this):null;if(!this.Hx)this.Ge=this.Ge+1;return!this.
Hx;}if(!Aw.Down){this.I6=this.Ge>1;this.J5=this.Ge-1;this.Ge=0;(A=this.Db)?A[1].
call(A[0],this):null;return!this.Hx;}}return false;},Bc:function(aArg){var A;var
CW=(C.M.isPrototypeOf(A=this.J)?A:null);if(!CW)throw new Error(Gt);this.BL=CW.Ik;
CW.Ik=this;},_Init:function(aArg){this.__proto__=C.HG;this.Bc(aArg);B.gv++;},_Done:
function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var
A;if((A=this.BL)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Db)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Da)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.GR)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"
};C.IG={CS:null,GI:0,EV:B.qx,Initialize:function(Ae,aOffset){this.CS=Ae;this.EV=
aOffset;this.GI=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:
function(aArg){this.__proto__=C.IG;B.gv++;},_Done:function(){this.__proto__=null;
B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.CS)&&(A._cycle!=
D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null
,_cycle:0,_observers:null,_className:"Core::CursorHit"};C.Lr={CW:null,_Init:function(
aArg){this.__proto__=C.Lr;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.CW)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::ModalContext"};C.HH={Gb:null,CB:B.qy,As:B.qy,isEmpty:false
,_Init:function(aArg){this.__proto__=C.HH;B.gv++;},_Done:function(){this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.Gb)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null
,_cycle:0,_observers:null,_className:"Core::LayoutContext"};C.HI={Hk:B.qx,Hj:B.qx
,Hi:B.qx,Hh:B.qx,_Init:function(aArg){C.HH._Init.call(this,aArg);this.__proto__=
C.HI;},_className:"Core::LayoutQuadContext"};C.Le={CW:null,_Init:function(aArg){
this.__proto__=C.Le;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.CW)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::DialogContext"};C.LS={Jn:null,DE:null,_Init:function(aArg
){this.__proto__=C.LS;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.Jn)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.DE)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D
))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::TaskQueue"
};C.LR={_Init:function(aArg){this.__proto__=C.LR;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.
J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::Task"};C.EY={resource:null,D3:function(){this.resource=null;},Bc:function(
aArg){this.resource=aArg;},_Init:function(aArg){this.__proto__=C.EY;this.Bc(aArg
);B.gv++;},_Done:function(){this.D3();this.__proto__=null;B.gv--;},_ReInit:function(
){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:
null,_cycle:0,_observers:null,_className:"Core::Resource"};C.Timer={FK:null,timer:
null,Bq:0,Period:1000,Hv:0,Ex:false,D3:function(){var tmp=this.timer;if(!!tmp)tmp.
DestroyTimer();this.timer=null;},Is:function(aBegin,aPeriod){if(aBegin<0)aBegin=
0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>0)||(aPeriod>0)))tmp=
B.sL(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(aBegin,aPeriod
);}this.timer=tmp;},GS:function(E){if(E<0)E=0;if(E===this.Period)return;this.Period=
E;if(this.Ex)this.Is(this.Hv,E);},J3:function(E){if(E<0)E=0;if(E===this.Hv)return;
this.Hv=E;if(this.Ex)this.Is(E,this.Period);},Bd:function(E){if(E===this.Ex)return;
this.Ex=E;if(E)this.Is(this.Hv,this.Period);else this.Is(0,0);this.Bq=this.HF();
},HF:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.qt)|0;return ticksCount;
},Trigger:function(){var A;this.Bq=this.HF();if(!this.Period)this.Bd(false);(A=this.
FK)?A[1].call(A[0],this):null;},_Init:function(aArg){this.__proto__=C.Timer;B.gv++;
},_Done:function(){this.D3();this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.FK)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=
this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::Timer"};C.Bq={LN:0,JZ:0,JY:0,Ld:0,Ls:0,LV:0,N6:function(){var result;result=((
new Date).getTime()/1000)|0;return result;},KH:function(Jg){switch(Jg){case 0:return 0;
case 1:return 31;case 2:return 60;case 3:return 91;case 4:return 121;case 5:return 152;
case 6:return 182;case 7:return 213;case 8:return 244;case 9:return 274;case 10:
return 305;case 11:return 335;case 12:return 366;default:return 0;}},KG:function(
Jg){switch(Jg){case 0:return 0;case 1:return 31;case 2:return 59;case 3:return 90;
case 4:return 120;case 5:return 151;case 6:return 181;case 7:return 212;case 8:return 243;
case 9:return 273;case 10:return 304;case 11:return 334;case 12:return 365;default:
return 0;}},LG:function(E){var Dm=(((E/86400)|0)+365)|0;var K2=(Dm/1461)|0;var FE;
var EJ;Dm=Dm-(K2*1461);FE=(Dm/365)|0;if(FE===4)FE=3;Dm=Dm-(FE*365);FE=((K2*4)+FE
)+1969;EJ=Dm>>5;if(!!(FE%4)){if(Dm>=this.KG(EJ+1))EJ=EJ+1;Dm=Dm-this.KG(EJ);}else{
if(Dm>=this.KH(EJ+1))EJ=EJ+1;Dm=Dm-this.KH(EJ);}this.LH(FE);this.LE(EJ+1);this.Lz(
Dm+1);this.LB((((E/3600)|0)%24)|0);this.LD((((E/60)|0)%60)|0);this.LF((E%60)|0);
},Lv:function(){return B._NewObject(C.Bq,0).Initialize(this.N6());},LF:function(
E){if(E<0)E=0;if(E>59)E=59;this.LN=E;},LD:function(E){if(E<0)E=0;if(E>59)E=59;this.
JZ=E;},LB:function(E){if(E<0)E=0;if(E>23)E=23;this.JY=E;},Lz:function(E){if(E<1)
E=1;if(E>31)E=31;this.Ld=E;},LE:function(E){if(E<1)E=1;if(E>12)E=12;this.Ls=E;},
LH:function(E){if(E<1970)E=1970;this.LV=E;},Initialize:function(NW){this.LG(NW);
return this;},_Init:function(aArg){this.__proto__=C.Bq;B.gv++;},_Done:function(){
this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=
this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::Time"};C.PH={PJ:0x1,Ox:0x2,O0:0x4,PF:0x8,Ex:0x10,I7:0x20,O1:0x40,O_:0x80,
OZ:0x100,O4:0x200,OS:0x400,Pj:0x800,DU:0x1000,PG:0x2000,Pd:0x4000,Pe:0x8000,OM:0x10000
,Pc:0x20000,Pt:0x40000};C.Fe={Pk:0x1,Pl:0x2,Ou:0x4,Ov:0x8,Ow:0x10,Ot:0x20};C.HD={
O$:0,PC:1,OB:2,O5:3,Pn:4,PD:5,PE:6,OC:7,OD:8,O7:9,O6:10,Pp:11,Po:12};C.KeyCode={
NoKey:0,Ok:1,Exit:2,Menu:3,Up:4,Down:5,Left:6,Right:7,PageUp:8,PageDown:9,Key0:10
,Key1:11,Key2:12,Key3:13,Key4:14,Key5:15,Key6:16,Key7:17,Key8:18,Key9:19,Red:20,
Green:21,Blue:22,Yellow:23,White:24,Magenta:25,F1:26,F2:27,F3:28,F4:29,F5:30,F6:
31,F7:32,F8:33,F9:34,F10:35,ChannelUp:36,ChannelDown:37,Display:38,SkipPrev:39,SkipNext:
40,Home:41,End:42,Insert:43,Delete:44,Clear:45,VolumeUp:46,VolumeDown:47,Show:48
,Hide:49,Play:50,Pause:51,Record:52,Stop:53,Rev:54,Fwd:55,SlowRev:56,SlowFwd:57,
SkipBwd:58,SkipFwd:59,Repeat:60,Eject:61,Help:62,TV:63,DVD:64,VCR:65,EPG:66,OSD:
67,Text:68,PIP:69,Audio:70,Clock:71,Timer:72,Navigation:73,Karaoke:74,Game:75,Subtitle:
76,Zoom:77,Index:78,Info:79,Power:80,Setup:81,Angle:82,Mode:83,Mute:84,User0:85,
User1:86,User2:87,User3:88,User4:89,User5:90,User6:91,User7:92,User8:93,User9:94
,User10:95,User11:96,User12:97,User13:98,User14:99,User15:100,User16:101,User17:
102,User18:103,User19:104,KeyA:105,KeyB:106,KeyC:107,KeyD:108,KeyE:109,KeyF:110,
KeyG:111,KeyH:112,KeyI:113,KeyJ:114,KeyK:115,KeyL:116,KeyM:117,KeyN:118,KeyO:119
,KeyP:120,KeyQ:121,KeyR:122,KeyS:123,KeyT:124,KeyU:125,KeyV:126,KeyW:127,KeyX:128
,KeyY:129,KeyZ:130,Space:131,Plus:132,Minus:133,Multiply:134,Divide:135,Equals:136
,Period:137,Comma:138,Colon:139,Semicolon:140,AlphaKeys:141,AlphaOrDigitKeys:142
,DigitKeys:143,HexDigitKeys:144,CharacterKeys:145,ControlKeys:146,CursorKeys:147
,AnyKey:148,Enter:149,Escape:150,Backspace:151,Tab:152,CtrlKeyA:153,CtrlKeyB:154
,CtrlKeyC:155,CtrlKeyD:156,CtrlKeyE:157,CtrlKeyF:158,CtrlKeyG:159,CtrlKeyH:160,CtrlKeyI:
161,CtrlKeyJ:162,CtrlKeyK:163,CtrlKeyL:164,CtrlKeyM:165,CtrlKeyN:166,CtrlKeyO:167
,CtrlKeyP:168,CtrlKeyQ:169,CtrlKeyR:170,CtrlKeyS:171,CtrlKeyT:172,CtrlKeyU:173,CtrlKeyV:
174,CtrlKeyW:175,CtrlKeyX:176,CtrlKeyY:177,CtrlKeyZ:178,CtrlSpace:179,CtrlKey0:180
,CtrlKey1:181,CtrlKey2:182,CtrlKey3:183,CtrlKey4:184,CtrlKey5:185,CtrlKey6:186,CtrlKey7:
187,CtrlKey8:188,CtrlKey9:189,CtrlF1:190,CtrlF2:191,CtrlF3:192,CtrlF4:193,CtrlF5:
194,CtrlF6:195,CtrlF7:196,CtrlF8:197,CtrlF9:198,CtrlF10:199,CtrlEnter:200,CtrlEscape:
201,CtrlUp:202,CtrlDown:203,CtrlLeft:204,CtrlRight:205,CtrlPageUp:206,CtrlPageDown:
207,CtrlBackspace:208,CtrlInsert:209,CtrlDelete:210,CtrlHome:211,CtrlEnd:212,CtrlTab:
213,CtrlShiftKeyA:214,CtrlShiftKeyB:215,CtrlShiftKeyC:216,CtrlShiftKeyD:217,CtrlShiftKeyE:
218,CtrlShiftKeyF:219,CtrlShiftKeyG:220,CtrlShiftKeyH:221,CtrlShiftKeyI:222,CtrlShiftKeyJ:
223,CtrlShiftKeyK:224,CtrlShiftKeyL:225,CtrlShiftKeyM:226,CtrlShiftKeyN:227,CtrlShiftKeyO:
228,CtrlShiftKeyP:229,CtrlShiftKeyQ:230,CtrlShiftKeyR:231,CtrlShiftKeyS:232,CtrlShiftKeyT:
233,CtrlShiftKeyU:234,CtrlShiftKeyV:235,CtrlShiftKeyW:236,CtrlShiftKeyX:237,CtrlShiftKeyY:
238,CtrlShiftKeyZ:239,CtrlShiftSpace:240,CtrlShiftKey0:241,CtrlShiftKey1:242,CtrlShiftKey2:
243,CtrlShiftKey3:244,CtrlShiftKey4:245,CtrlShiftKey5:246,CtrlShiftKey6:247,CtrlShiftKey7:
248,CtrlShiftKey8:249,CtrlShiftKey9:250,CtrlShiftF1:251,CtrlShiftF2:252,CtrlShiftF3:
253,CtrlShiftF4:254,CtrlShiftF5:255,CtrlShiftF6:256,CtrlShiftF7:257,CtrlShiftF8:
258,CtrlShiftF9:259,CtrlShiftF10:260,CtrlShiftEnter:261,CtrlShiftEscape:262,CtrlShiftUp:
263,CtrlShiftDown:264,CtrlShiftLeft:265,CtrlShiftRight:266,CtrlShiftPageUp:267,CtrlShiftPageDown:
268,CtrlShiftBackspace:269,CtrlShiftInsert:270,CtrlShiftDelete:271,CtrlShiftHome:
272,CtrlShiftEnd:273,CtrlShiftTab:274,AltF1:275,AltF2:276,AltF3:277,AltF4:278,AltF5:
279,AltF6:280,AltF7:281,AltF8:282,AltF9:283,AltF10:284,AltEnter:285,AltEscape:286
,AltUp:287,AltDown:288,AltLeft:289,AltRight:290,AltPageUp:291,AltPageDown:292,AltBackspace:
293,AltInsert:294,AltDelete:295,AltHome:296,AltEnd:297,AltTab:298,AltShiftF1:299
,AltShiftF2:300,AltShiftF3:301,AltShiftF4:302,AltShiftF5:303,AltShiftF6:304,AltShiftF7:
305,AltShiftF8:306,AltShiftF9:307,AltShiftF10:308,AltShiftEnter:309,AltShiftEscape:
310,AltShiftUp:311,AltShiftDown:312,AltShiftLeft:313,AltShiftRight:314,AltShiftPageUp:
315,AltShiftPageDown:316,AltShiftBackspace:317,AltShiftInsert:318,AltShiftDelete:
319,AltShiftHome:320,AltShiftEnd:321,AltShiftTab:322,ShiftF1:323,ShiftF2:324,ShiftF3:
325,ShiftF4:326,ShiftF5:327,ShiftF6:328,ShiftF7:329,ShiftF8:330,ShiftF9:331,ShiftF10:
332,ShiftEnter:333,ShiftEscape:334,ShiftUp:335,ShiftDown:336,ShiftLeft:337,ShiftRight:
338,ShiftPageUp:339,ShiftPageDown:340,ShiftBackspace:341,ShiftInsert:342,ShiftDelete:
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};C.Pm={PN:0x1,PK:0x2,PL:0x4,PM:0x8,O9:
0x10,O2:0x20};
C._Init=function(){C.GW.__proto__=C.CS;C.BO.__proto__=C.CS;C.M.__proto__=C.BO;C.Root.
__proto__=C.M;C.KeyEvent.__proto__=C.Event;C.Hz.__proto__=C.Event;C.Hy.__proto__=
C.Event;C.HA.__proto__=C.Event;C.GV.__proto__=C.BO;C.Ad.__proto__=C.GW;C.HI.__proto__=
C.HH;};C.C2=function(D){};return C;})();

/* Embedded Wizard */