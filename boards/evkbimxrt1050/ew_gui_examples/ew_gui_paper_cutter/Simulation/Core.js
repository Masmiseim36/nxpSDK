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
);if(EmWiApp.Core)throw new Error("The unit file 'Core.js' included twice!");EmWiApp.
Core=(function(){var B=EmWiApp;var C={};
var AO=[0,0];var BF=[0,0,0,0];var Dh="The view does not belong to this group";var
Fh="No view to restack";var Dw="View is not in this group";var FH="No view to remove";
var FI="No view to add";var IS="View already in a group";var Gl="Recursive invalidate during active update cycle.";
var Gm="The KeyPressHandler is embedded within an object not being derived "+"from Core::Group.";
C.Cw={AP:null,AF:null,L:null,A2:null,F:0x103,E$:0x14,G1:function(Ac,HW){},T:function(
E){var A;var NH=E^this.E$;if(!NH)return;this.E$=E;if(!!this.A2&&!((this.F&0x400)===
0x400)){this.L.F=this.L.F|0x5000;B.kD([A=this.L,A.Cg],this);this.L.AJ([0,0,(A=this.
L.G)[2]-A[0],A[3]-A[1]]);}if(!!this.A2&&((this.F&0x400)===0x400)){this.A2.FW.F=this.
A2.FW.F|0x1000;this.L.F=this.L.F|0x4000;B.kD([A=this.L,A.Cg],this);}},Ga:function(
){var Ag=this.L;while(!!Ag){var KF=(C.Root.isPrototypeOf(Ag)?Ag:null);if(!!KF)return KF;
Ag=Ag.L;}return null;},DI:function(Bi,aClip,aOffset,A9,aBlend){},BU:function(Az){
return null;},EM:function(AD,S,CQ,Go,FN){return null;},Hj:function(Ac,Di){return AO;
},Iz:function(aOffset,HV){},GetExtent:function(){return BF;},BQ:function(EW,FL){
var A;if(((this.F&0x200)===0x200))EW=EW&~0x400;var Jg=(this.F&~FL)|EW;var Fn=Jg^
this.F;this.F=Jg;if(!!this.L&&!!(Fn&0x14)){var Kv=((this.F&0x14)===0x14);if(Kv&&
!this.L.Dq)this.L.Gd(this);if(!Kv&&(this.L.Dq===this))this.L.Gd(this.L.JD(this,0x14
));}if(!!this.L&&!!(Fn&0x403))this.L.AJ(this.GetExtent());if((!!this.A2&&((Jg&0x400
)===0x400))&&((Fn&0x1)===0x1)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;B.kD([
A=this.L,A.Cg],this);}if(!!this.L&&((Fn&0x400)===0x400)){this.A2=null;this.F=this.
F|0x800;this.L.F=this.L.F|0x4000;B.kD([A=this.L,A.Cg],this);}},_Init:function(aArg
){this.__proto__=C.Cw;B.gb++;},_Done:function(){this.__proto__=C.Cw;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.AP)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.AF)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.L)&&(A._cycle!=D
))A._Mark(A._cycle=D);if((A=this.A2)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::View"};C.GN={BY:B.pm,Cm:B.pm,BX:B.pm,Cl:B.pm,G1:function(Ac,HW){var Af=B.
_NewObject(C.HA,0);this.A2=null;Af.CA=this.GetExtent();Af.As=Ac;Af.FW=HW;Af.G_=this.
Cl;Af.G$=this.BX;Af.Ha=this.Cm;Af.Hb=this.BY;this.A2=Af;},Hj:function(Ac,Di){var
A;var BH=this.E$;var Af=(C.HA.isPrototypeOf(A=this.A2)?A:null);var V=Af.CA[0];var
W=Af.CA[1];var X=Af.CA[2];var Y=Af.CA[3];var Cz=[Ac[2]-Ac[0],Ac[3]-Ac[1]];var Ah=
X-V;var Ak=Y-W;if(!Di){var DG=[(A=Af.As)[2]-A[0],A[3]-A[1]];V=V-Af.As[0];W=W-Af.
As[1];if(DG[0]!==Cz[0]){var C0=((BH&0x4)===0x4);var C1=((BH&0x8)===0x8);var Ee=((
BH&0x1)===0x1);if(!C0&&(Ee||!C1))V=((V*Cz[0])/DG[0])|0;if(!C1&&(Ee||!C0)){X=X-Af.
As[0];X=((X*Cz[0])/DG[0])|0;X=X-Cz[0];}else X=X-Af.As[2];V=V+Ac[0];X=X+Ac[2];if(
!Ee){if(C0&&!C1)X=V+Ah;else if(!C0&&C1)V=X-Ah;else{V=V+((((X-V)-Ah)/2)|0);X=V+Ah;
}}}else{X=X-Af.As[2];V=V+Ac[0];X=X+Ac[2];}if(DG[1]!==Cz[1]){var C2=((BH&0x10)===
0x10);var CZ=((BH&0x20)===0x20);var Ef=((BH&0x2)===0x2);if(!C2&&(Ef||!CZ))W=((W*
Cz[1])/DG[1])|0;if(!CZ&&(Ef||!C2)){Y=Y-Af.As[1];Y=((Y*Cz[1])/DG[1])|0;Y=Y-Cz[1];
}else Y=Y-Af.As[3];W=W+Ac[1];Y=Y+Ac[3];if(!Ef){if(C2&&!CZ)Y=W+Ak;else if(!C2&&CZ
)W=Y-Ak;else{W=W+((((Y-W)-Ak)/2)|0);Y=W+Ak;}}}else{Y=Y-Af.As[3];W=W+Ac[1];Y=Y+Ac[
3];}}else{switch(Di){case 3:{V=Ac[0];X=V+Ah;}break;case 4:{X=Ac[2];V=X-Ah;}break;
case 1:{W=Ac[1];Y=W+Ak;}break;case 2:{Y=Ac[3];W=Y-Ak;}break;default:;}if((Di===3
)||(Di===4)){var C2=((BH&0x10)===0x10);var CZ=((BH&0x20)===0x20);var Ef=((BH&0x2
)===0x2);if(Ef){W=Ac[1];Y=Ac[3];}else if(C2&&!CZ){W=Ac[1];Y=W+Ak;}else if(CZ&&!C2
){Y=Ac[3];W=Y-Ak;}else{W=Ac[1]+((((Ac[3]-Ac[1])-Ak)/2)|0);Y=W+Ak;}}if((Di===1)||(
Di===2)){var C0=((BH&0x4)===0x4);var C1=((BH&0x8)===0x8);var Ee=((BH&0x1)===0x1);
if(Ee){V=Ac[0];X=Ac[2];}else if(C0&&!C1){V=Ac[0];X=V+Ah;}else if(C1&&!C0){X=Ac[2
];V=X-Ah;}else{V=Ac[0]+((((Ac[2]-Ac[0])-Ah)/2)|0);X=V+Ah;}}}Af.isEmpty=(V>=X)||(
W>=Y);Ah=(X-V)-1;Ak=(Y-W)-1;var EZ=Af.CA[0];var E0=Af.CA[1];var EB=(Af.CA[2]-EZ)-
1;var EA=(Af.CA[3]-E0)-1;if(!EB)EB=1;if(!EA)EA=1;if(((this.F&0x100)===0x100)){this.
Cl=[V+((((Af.G_[0]-EZ)*Ah)/EB)|0),W+((((Af.G_[1]-E0)*Ak)/EA)|0)];this.BX=[V+((((
Af.G$[0]-EZ)*Ah)/EB)|0),W+((((Af.G$[1]-E0)*Ak)/EA)|0)];this.Cm=[V+((((Af.Ha[0]-EZ
)*Ah)/EB)|0),W+((((Af.Ha[1]-E0)*Ak)/EA)|0)];this.BY=[V+((((Af.Hb[0]-EZ)*Ah)/EB)|
0),W+((((Af.Hb[1]-E0)*Ak)/EA)|0)];}else{this.Dc([V+((((Af.G_[0]-EZ)*Ah)/EB)|0),W+((((
Af.G_[1]-E0)*Ak)/EA)|0)]);this.Dd([V+((((Af.G$[0]-EZ)*Ah)/EB)|0),W+((((Af.G$[1]-
E0)*Ak)/EA)|0)]);this.De([V+((((Af.Ha[0]-EZ)*Ah)/EB)|0),W+((((Af.Ha[1]-E0)*Ak)/EA
)|0)]);this.Df([V+((((Af.Hb[0]-EZ)*Ah)/EB)|0),W+((((Af.Hb[1]-E0)*Ak)/EA)|0)]);this.
A2=Af;}return[Ah+1,Ak+1];},Iz:function(aOffset,HV){if(HV){this.Cl=B.so(this.Cl,aOffset
);this.BX=B.so(this.BX,aOffset);this.Cm=B.so(this.Cm,aOffset);this.BY=B.so(this.
BY,aOffset);}else{this.Dc(B.so(this.Cl,aOffset));this.Dd(B.so(this.BX,aOffset));
this.De(B.so(this.Cm,aOffset));this.Df(B.so(this.BY,aOffset));}},GetExtent:function(
){if(!!this.A2&&this.A2.isEmpty)return BF;var V=this.Cl[0];var W=this.Cl[1];var X=
this.Cm[0];var Y=this.Cm[1];if((((this.BY[0]!==V)||(this.BX[1]!==W))||(this.BX[0
]!==X))||(this.BY[1]!==Y)){if(this.BX[0]<V)V=this.BX[0];if(this.Cm[0]<V)V=this.Cm[
0];if(this.BY[0]<V)V=this.BY[0];if(this.BX[1]<W)W=this.BX[1];if(this.Cm[1]<W)W=this.
Cm[1];if(this.BY[1]<W)W=this.BY[1];if(this.Cl[0]>X)X=this.Cl[0];if(this.BX[0]>X)
X=this.BX[0];if(this.BY[0]>X)X=this.BY[0];if(this.Cl[1]>Y)Y=this.Cl[1];if(this.BX[
1]>Y)Y=this.BX[1];if(this.BY[1]>Y)Y=this.BY[1];}else{var tmp;if(X<V){tmp=V;V=X;X=
tmp;}if(Y<W){tmp=W;W=Y;Y=tmp;}}return[V,W,X+1,Y+1];},Df:function(E){var A;if(B.sa(
E,this.BY))return;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.GetExtent());
this.A2=null;this.BY=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.GetExtent(
));if((!!this.L&&((this.F&0x400)===0x400))&&!((this.L.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.L.F=this.L.F|0x4000;B.kD([A=this.L,A.Cg],this);}},De:function(
E){var A;if(B.sa(E,this.Cm))return;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.
GetExtent());this.A2=null;this.Cm=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(
this.GetExtent());if((!!this.L&&((this.F&0x400)===0x400))&&!((this.L.F&0x2000)===
0x2000)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;B.kD([A=this.L,A.Cg],this);
}},Dd:function(E){var A;if(B.sa(E,this.BX))return;if(!!this.L&&((this.F&0x1)===0x1
))this.L.AJ(this.GetExtent());this.A2=null;this.BX=E;if(!!this.L&&((this.F&0x1)===
0x1))this.L.AJ(this.GetExtent());if((!!this.L&&((this.F&0x400)===0x400))&&!((this.
L.F&0x2000)===0x2000)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;B.kD([A=this.
L,A.Cg],this);}},Dc:function(E){var A;if(B.sa(E,this.Cl))return;if(!!this.L&&((this.
F&0x1)===0x1))this.L.AJ(this.GetExtent());this.A2=null;this.Cl=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AJ(this.GetExtent());if((!!this.L&&((this.F&0x400)===0x400
))&&!((this.L.F&0x2000)===0x2000)){this.F=this.F|0x800;this.L.F=this.L.F|0x4000;
B.kD([A=this.L,A.Cg],this);}},Ix:function(D8){var A5=B.sr(4,B.pm,null);var K=0;var
A$=3;var Jb=false;var Jc=false;A5.Set(0,this.Cl);A5.Set(1,this.BX);A5.Set(2,this.
Cm);A5.Set(3,this.BY);while(K<4){var KL=A5.Get(K)[0];var In=A5.Get(K)[1];var N5=
A5.Get(A$)[0];var Js=A5.Get(A$)[1];if(((In>D8[1])!==(Js>D8[1]))||((In<D8[1])!==(
Js<D8[1]))){var Ax=((((N5-KL)*(D8[1]-In))/(Js-In))|0)+KL;if(D8[0]>Ax)Jb=!Jb;if(D8[
0]<Ax)Jc=!Jc;}A$=K;K=K+1;}return Jb||Jc;},K7:function(){return((((this.Cl[0]===this.
BY[0])&&(this.BX[0]===this.Cm[0]))&&(this.Cl[1]===this.BX[1]))&&(this.Cm[1]===this.
BY[1]))||((((this.Cl[0]===this.BX[0])&&(this.Cm[0]===this.BY[0]))&&(this.Cl[1]===
this.BY[1]))&&(this.BX[1]===this.Cm[1]));},_Init:function(aArg){C.Cw._Init.call(
this,aArg);this.__proto__=C.GN;},_className:"Core::QuadView"};C.BZ={G:B.pn,G1:function(
Ac,HW){var Af=B._NewObject(C.GD,0);Af.CA=this.G;Af.As=Ac;Af.FW=HW;this.A2=Af;},Hj:
function(Ac,Di){var A;var BH=this.E$;var Af=this.A2;var V=Af.CA[0];var W=Af.CA[1
];var X=Af.CA[2];var Y=Af.CA[3];var Cz=[Ac[2]-Ac[0],Ac[3]-Ac[1]];var Ah=X-V;var Ak=
Y-W;if(!Di){var DG=[(A=Af.As)[2]-A[0],A[3]-A[1]];V=V-Af.As[0];W=W-Af.As[1];if(DG[
0]!==Cz[0]){var C0=((BH&0x4)===0x4);var C1=((BH&0x8)===0x8);var Ee=((BH&0x1)===0x1
);if(!C0&&(Ee||!C1))V=((V*Cz[0])/DG[0])|0;if(!C1&&(Ee||!C0)){X=X-Af.As[0];X=((X*
Cz[0])/DG[0])|0;X=X-Cz[0];}else X=X-Af.As[2];V=V+Ac[0];X=X+Ac[2];if(!Ee){if(C0&&
!C1)X=V+Ah;else if(!C0&&C1)V=X-Ah;else{V=V+((((X-V)-Ah)/2)|0);X=V+Ah;}}}else{X=X-
Af.As[2];V=V+Ac[0];X=X+Ac[2];}if(DG[1]!==Cz[1]){var C2=((BH&0x10)===0x10);var CZ=((
BH&0x20)===0x20);var Ef=((BH&0x2)===0x2);if(!C2&&(Ef||!CZ))W=((W*Cz[1])/DG[1])|0;
if(!CZ&&(Ef||!C2)){Y=Y-Af.As[1];Y=((Y*Cz[1])/DG[1])|0;Y=Y-Cz[1];}else Y=Y-Af.As[
3];W=W+Ac[1];Y=Y+Ac[3];if(!Ef){if(C2&&!CZ)Y=W+Ak;else if(!C2&&CZ)W=Y-Ak;else{W=W+((((
Y-W)-Ak)/2)|0);Y=W+Ak;}}}else{Y=Y-Af.As[3];W=W+Ac[1];Y=Y+Ac[3];}}else{switch(Di){
case 3:{V=Ac[0];X=V+Ah;}break;case 4:{X=Ac[2];V=X-Ah;}break;case 1:{W=Ac[1];Y=W+
Ak;}break;case 2:{Y=Ac[3];W=Y-Ak;}break;default:;}if((Di===3)||(Di===4)){var C2=((
BH&0x10)===0x10);var CZ=((BH&0x20)===0x20);var Ef=((BH&0x2)===0x2);if(Ef){W=Ac[1
];Y=Ac[3];}else if(C2&&!CZ){W=Ac[1];Y=W+Ak;}else if(CZ&&!C2){Y=Ac[3];W=Y-Ak;}else{
W=Ac[1]+((((Ac[3]-Ac[1])-Ak)/2)|0);Y=W+Ak;}}if((Di===1)||(Di===2)){var C0=((BH&0x4
)===0x4);var C1=((BH&0x8)===0x8);var Ee=((BH&0x1)===0x1);if(Ee){V=Ac[0];X=Ac[2];
}else if(C0&&!C1){V=Ac[0];X=V+Ah;}else if(C1&&!C0){X=Ac[2];V=X-Ah;}else{V=Ac[0]+((((
Ac[2]-Ac[0])-Ah)/2)|0);X=V+Ah;}}}Af.isEmpty=(V>=X)||(W>=Y);if(((this.F&0x100)===
0x100)){this.G=[V,W,X,Y];}else{this.H([V,W,X,Y]);this.A2=Af;}return[X-V,Y-W];},Iz:
function(aOffset,HV){if(HV)this.G=B.sq(this.G,aOffset);else this.H(B.sq(this.G,aOffset
));},GetExtent:function(){return this.G;},H:function(E){var A;if(B.sb(E,this.G))
return;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);this.A2=null;this.G=E;
if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);if((!!this.L&&((this.F&0x400
)===0x400))&&!((this.L.F&0x2000)===0x2000)){this.F=this.F|0x800;this.L.F=this.L.
F|0x4000;B.kD([A=this.L,A.Cg],this);}},Ny:function(Jr){this.H(Jr);},Lc:function(
){return this.G;},_Init:function(aArg){C.Cw._Init.call(this,aArg);this.__proto__=
C.BZ;},_className:"Core::RectView"};C.P={C5:null,Do:null,Ia:null,Dj:null,Fo:null
,FR:null,Dq:null,Bc:255,DI:function(Bi,aClip,aOffset,A9,aBlend){var A;A9=((A9+1)
*this.Bc)>>8;aBlend=aBlend&&((this.F&0x2)===0x2);if(!this.Dj)this.NI(Bi,aClip,B.
so(aOffset,this.G.slice(0,2)),A9,aBlend);else{var Cx=255|(255<<8)|(255<<16)|((A9&
0xFF)<<24);this.Dj.Update();Bi.Jx(aClip,this.Dj,0,B.sq(this.G,aOffset),AO,Cx,Cx,
Cx,Cx,aBlend);}},EM:function(AD,S,CQ,Go,FN){var A;var M=this.Do;var FT=null;var Al=
BF;var Ce=null;var Jf=!!this.FR&&(!!this.FR.I7||!!this.FR.C5);if(((A=B.hV(AD,this.
G))[0]>=A[2])||(A[1]>=A[3]))return null;AD=B.sp(AD,this.G.slice(0,2));while(!!M){
if(((M.F&0x400)===0x400)&&!Ce){Ce=M.AF;while(!!Ce&&!((Ce.F&0x200)===0x200))Ce=Ce.
AF;if(!!Ce)Al=B.hV(AD,Ce.GetExtent());else Al=BF;}if(Ce===M){Ce=null;Al=BF;}if((((((
M.F&0x8)===0x8)&&((M.F&0x10)===0x10))&&!((M.F&0x40000)===0x40000))&&!((M.F&0x20000
)===0x20000))&&(!((M.F&0x10000)===0x10000)||((this.Fo.CS===M)&&!Jf))){var CA=M.GetExtent(
);var H3=Go;var FP=null;if(H3===M)H3=null;if(((M.F&0x400)===0x400)){if(!(((A=B.hV(
CA,Al))[0]>=A[2])||(A[1]>=A[3])))FP=M.EM(Al,S,CQ,H3,FN);}else{if(!(((A=B.hV(CA,AD
))[0]>=A[2])||(A[1]>=A[3]))||(Go===M))FP=M.EM(AD,S,CQ,H3,FN);}M=M.AF;if(!!FP){if(
!FT||((FP.Gx<FT.Gx)&&(FP.Gx>=0)))FT=FP;if(!FP.Gx)M=null;}}else M=M.AF;}return FT;
},BQ:function(EW,FL){var A;var NP=this.F;C.BZ.BQ.call(this,EW,FL);var Fn=this.F^
NP;if(!!this.Dq&&((Fn&0x40)===0x40)){if(((this.F&0x40)===0x40))this.Dq.BQ(0x40,0x0
);else this.Dq.BQ(0x0,0x40);}if(!!this.Fo&&((Fn&0x40)===0x40)){if(((this.F&0x40)===
0x40)&&((this.Fo.CS.F&0x14)===0x14))this.Fo.CS.BQ(0x40,0x0);else this.Fo.CS.BQ(0x0
,0x40);}if(!!Fn){this.F=this.F|0x8000;B.kD([this,this.Cg],this);}},H:function(E){
var A;if(B.sb(E,this.G))return;var Fs=[(A=this.G)[2]-A[0],A[3]-A[1]];var Ie=[E[2
]-E[0],E[3]-E[1]];var F0=!B.sa(Fs,Ie);if(F0&&!!this.Dj){this.Dj.IF(Ie);B.pl(this
,0);B.pl(this.Dj,0);}C.BZ.H.call(this,E);if((F0&&(Fs[0]>0))&&(Fs[1]>0)){var As=[
].concat(AO,Fs);var M=this.C5;var GZ=0x14;while(!!M){if((!M.A2&&(M.E$!==GZ))&&!((
M.F&0x400)===0x400))M.G1(As,null);M=M.AP;}}if(F0){this.F=this.F|0x5000;B.kD([this
,this.Cg],this);}},Jk:function(Az){var Ky=(C.KeyEvent.isPrototypeOf(Az)?Az:null);
var DU=this.Ia;if(!Ky)return null;while(!!DU&&(!DU.Ep||!DU.BU(Ky)))DU=DU.AP;return DU;
},NI:function(Bi,aClip,aOffset,A9,aBlend){var A;var M=this.C5;var Kr=BF;var KC=true;
while(!!M){if(((M.F&0x200)===0x200)){var KB=(C.GM.isPrototypeOf(M)?M:null);Kr=B.
hV(aClip,B.sq(KB.G,aOffset));KC=((KB.F&0x1)===0x1);}if(((M.F&0x1)===0x1)){if(((M.
F&0x400)===0x400)){if(KC){var Al=B.hV(B.sq(M.GetExtent(),aOffset),Kr);if(!((Al[0
]>=Al[2])||(Al[1]>=Al[3])))M.DI(Bi,Al,aOffset,A9,aBlend);}}else{var Al=B.hV(B.sq(
M.GetExtent(),aOffset),aClip);if(!((Al[0]>=Al[2])||(Al[1]>=Al[3])))M.DI(Bi,Al,aOffset
,A9,aBlend);}}M=M.AP;}},NS:function(){var A;var I$=((this.F&0x1000)===0x1000);var
E1=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];var EC=false;var FS=BF;var H5=AO;var G0=0;
var H4=0;var M=this.Do;var Ce=null;var GZ=0x14;while(!!M){if(((M.F&0x800)===0x800
)){EC=true;M.F=M.F&~0x800;}if(EC&&((M.F&0x200)===0x200)){EC=false;if(!!(C.GM.isPrototypeOf(
M)?M:null).Hx)M.F=M.F|0x1000;}M=M.AF;}EC=false;M=this.C5;if(I$){this.F=this.F&~0x1000;
I$=!((E1[0]>=E1[2])||(E1[1]>=E1[3]));}this.F=this.F|0x2000;while(!!M){if(((M.F&0x400
)===0x400)){if(!!M.A2&&(M.A2.FW!==Ce))M.A2=null;if((!M.A2&&EC)&&((M.E$!==GZ)||!!
H4))M.G1(FS,Ce);}if(!!M.A2){if(I$&&!((M.F&0x400)===0x400))M.Hj(E1,0);if(EC&&((M.
F&0x400)===0x400)){var BC=M.Hj(B.sq(FS,H5),H4);if(((M.F&0x1)===0x1)){var Ba=AO;switch(
H4){case 3:Ba=[BC[0]+G0,Ba[1]];break;case 4:Ba=[-BC[0]-G0,Ba[1]];break;case 1:Ba=[
Ba[0],BC[1]+G0];break;case 2:Ba=[Ba[0],-BC[1]-G0];break;default:;}H5=B.so(H5,Ba);
}}}if(((M.F&0x200)===0x200)){EC=((M.F&0x1000)===0x1000);Ce=(C.GM.isPrototypeOf(M
)?M:null);if(EC){M.F=M.F&~0x1000;FS=B.sq(Ce.G,Ce.D6);H5=AO;H4=Ce.Hx;G0=Ce.Space;
EC=!((FS[0]>=FS[2])||(FS[1]>=FS[3]));}if(EC){this.AJ(Ce.G);}}M=M.AP;}this.F=this.
F&~0x2000;this.DS([E1[2]-E1[0],E1[3]-E1[1]]);},Cg:function(Aa){var A;var N4=((this.
F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000)){this.F=this.F&~0x4000;this.NS(
);}if(((this.F&0x8000)===0x8000)||N4){this.F=this.F&~0x8000;this.Eu(this.F);}},Gd:
function(E){var A;if(!!E&&(E.L!==this))throw new Error(Dh);if(!!E&&!((E.F&0x14)===
0x14))E=null;if(!!E&&((E.F&0x10000)===0x10000))E=null;if(E===this.Dq)return;if(!
!this.Dq)this.Dq.BQ(0x0,0x60);this.Dq=E;if(!!E){if(((this.F&0x40)===0x40))E.BQ(0x60
,0x0);else E.BQ(0x20,0x0);}},A8:function(E){if(E)this.BQ(0x10,0x0);else this.BQ(
0x0,0x10);},Ar:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(E===this.Bc)return;
this.Bc=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);},Nz:function(Jr){
this.Ar(Jr);},JH:function(D8){var tmp=this;while(!!tmp){D8=B.sn(D8,tmp.G.slice(0
,2));tmp=tmp.L;}return D8;},DispatchEvent:function(Az){var A;var M=this.Dq;var Ag=(
C.P.isPrototypeOf(M)?M:null);var AL=null;var Jf=!!this.FR&&(!!this.FR.I7||!!this.
FR.C5);if(!!M&&((((M.F&0x10000)===0x10000)||((M.F&0x40000)===0x40000))||((M.F&0x20000
)===0x20000))){M=null;Ag=null;}if(!!this.Fo&&!Jf)AL=this.Fo.CS.DispatchEvent(Az);
if(!AL&&!!Ag)AL=Ag.DispatchEvent(Az);else if(!AL&&!!M)AL=M.BU(Az);if(!AL)AL=this.
BU(Az);if(!AL)AL=this.Jk(Az);return AL;},BroadcastEventAtPosition:function(Az,Kl
,aFilter){var A;var M=this.Do;var AL=null;while(!!M&&!AL){if((!aFilter||((A=aFilter
)&&((M.F&A)===A)))&&B.se(M.GetExtent(),Kl)){var Ag=(C.P.isPrototypeOf(M)?M:null);
if(!!Ag)AL=Ag.BroadcastEventAtPosition(Az,B.sn(Kl,Ag.G.slice(0,2)),aFilter);else
AL=M.BU(Az);}M=M.AF;}if(!AL)AL=this.BU(Az);return AL;},BroadcastEvent:function(Az
,aFilter){var A;var M=this.Do;var AL=null;while(!!M&&!AL){if(!aFilter||((A=aFilter
)&&((M.F&A)===A))){var Ag=(C.P.isPrototypeOf(M)?M:null);if(!!Ag)AL=Ag.BroadcastEvent(
Az,aFilter);else AL=M.BU(Az);}M=M.AF;}if(!AL)AL=this.BU(Az);if(!AL)AL=this.Jk(Az
);return AL;},DS:function(aSize){},Eu:function(Dy){},BD:function(){this.F=this.F|
0x8000;B.kD([this,this.Cg],this);},AJ:function(AD){var A;var Ag=this;while(!!Ag&&
!((AD[0]>=AD[2])||(AD[1]>=AD[3]))){var Gp=Ag.Dj;if(!Ag.L&&(Ag!==this)){Ag.AJ(AD);
return;}if(!!Gp){if(((A=Gp.D2)[0]>=A[2])||(A[1]>=A[3])){B.pl(Ag,0);B.pl(Gp,0);}var
I_=false;if(I_)Gp.D2=[0,0,(A=Ag.G)[2]-A[0],A[3]-A[1]];else Gp.D2=B.pA(Gp.D2,AD);
}if(!((Ag.F&0x1)===0x1))return;AD=B.hV(B.sq(AD,Ag.G.slice(0,2)),Ag.G);Ag=Ag.L;}}
,Bb:function(aArg){this.BD();},JD:function(Q,aFilter){var A;if(!Q||(Q.L!==this))
return null;var Fr=Q.AP;var Ft=Q.AF;var G6=0x10000;if(((aFilter&0x10000)===0x10000
))G6=0x0;while(!!Fr||!!Ft){if((!!Fr&&(!aFilter||((A=aFilter)&&((Fr.F&A)===A))))&&(
!G6||!((A=G6)&&((Fr.F&A)===A))))return Fr;if((!!Ft&&(!aFilter||((A=aFilter)&&((Ft.
F&A)===A))))&&(!G6||!((A=G6)&&((Ft.F&A)===A))))return Ft;if(!!Fr)Fr=Fr.AP;if(!!Ft
)Ft=Ft.AF;}return null;},IN:function(Q,Cd){var A;if(!Q)throw new Error(Fh);if(Q.
L!==this)throw new Error(Dw);var Fj=Q;var Fl=Q;while((Cd>0)&&!!Fj.AP){Fj=Fj.AP;Cd=
Cd-1;}while((Cd<0)&&!!Fl.AF){Fl=Fl.AF;Cd=Cd+1;}if((Fj===Q)&&(Fl===Q))return;if(((
Q.F&0x401)===0x401)){if(!!Q.AF&&!!Q.A2)Q.AF.F=Q.AF.F|0x800;Q.F=Q.F|0x800;this.F=
this.F|0x4000;B.kD([this,this.Cg],this);}if(((Q.F&0x200)===0x200)){if(!!Q.AF)Q.AF.
F=Q.AF.F|0x800;Q.F=Q.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cg],this);}if(
!!Q.AF)Q.AF.AP=Q.AP;if(!!Q.AP)Q.AP.AF=Q.AF;if(this.C5===Q)this.C5=Q.AP;if(this.Do===
Q)this.Do=Q.AF;if(Fj!==Q){Q.AP=Fj.AP;Q.AF=Fj;Fj.AP=Q;if(!!Q.AP)Q.AP.AF=Q;}if(Fl!==
Q){Q.AP=Fl;Q.AF=Fl.AF;Fl.AF=Q;if(!!Q.AF)Q.AF.AP=Q;}if(!Q.AP)this.Do=Q;if(!Q.AF)this.
C5=Q;if(((Q.F&0x1)===0x1))this.AJ(Q.GetExtent());},GP:function(Q){var A;if(!Q)throw new
Error(FH);if(Q.L!==this)throw new Error(Dw);if((((Q.F&0x401)===0x401)&&!!Q.AF)&&
!!Q.A2){Q.AF.F=Q.AF.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cg],this);}if(((
Q.F&0x200)===0x200)){if(!!Q.AF)Q.AF.F=Q.AF.F|0x800;this.F=this.F|0x4000;B.kD([this
,this.Cg],this);}Q.A2=null;if(this.Dq===Q)this.Gd(this.JD(Q,0x14));if(!!Q.AF)Q.AF.
AP=Q.AP;if(!!Q.AP)Q.AP.AF=Q.AF;if(this.C5===Q)this.C5=Q.AP;if(this.Do===Q)this.Do=
Q.AF;Q.L=null;Q.AP=null;Q.AF=null;if(((Q.F&0x1)===0x1))this.AJ(Q.GetExtent());},
N:function(Q,Cd){var A;if(!Q)throw new Error(FI);if(!!Q.L)throw new Error(IS);Q.
L=this;Q.AF=this.Do;if(!!this.Do)this.Do.AP=Q;this.Do=Q;if(!this.C5)this.C5=Q;if(
!!Cd)this.IN(Q,Cd);if(((Q.F&0x1)===0x1))this.AJ(Q.GetExtent());if(((!this.Dq&&((
Q.F&0x4)===0x4))&&((Q.F&0x10)===0x10))&&!((Q.F&0x10000)===0x10000))this.Gd(Q);if(((
Q.F&0x401)===0x401)){Q.F=Q.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cg],this
);}if(((Q.F&0x200)===0x200)){Q.F=Q.F|0x800;this.F=this.F|0x4000;B.kD([this,this.
Cg],this);}},Lg:function(){return this.Bc;},_Init:function(aArg){C.BZ._Init.call(
this,aArg);this.__proto__=C.P;this.F=0x1F;this.Bb(aArg);},_Mark:function(D){var A;
C.BZ._Mark.call(this,D);if((A=this.C5)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Do)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ia)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Dj)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fo)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.FR)&&(A._cycle!=D))A._Mark(A._cycle=D);
if((A=this.Dq)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Group"};C.
Root={CD:null,E3:null,D9:null,A4:B.sr(10,null,null),Gq:null,Co:null,Ez:null,Hh:0
,I3:0,A_:0,BO:B.sr(10,0,null),H2:B.sr(10,B.pn,null),DC:B.sr(10,0,null),Ey:B.sr(10
,B.pm,null),Gs:B.sr(10,0,null),EX:B.sr(10,B.pm,null),DB:B.sr(10,B.pm,null),CR:B.
sr(10,B.pm,null),D_:B.sr(10,B.pm,null),AE:0,H7:0,H6:0,Ic:B.sr(3,B.pn,null),Kz:0,
Cf:B.sr(4,0,null),BI:B.sr(4,B.pn,null),BB:0,E6:8,KY:250,Fp:0,E2:0,Ja:true,Ib:false
,Ga:function(){return this;},DI:function(Bi,aClip,aOffset,A9,aBlend){var fullScreenUpdate=
false;fullScreenUpdate=B.i6;if(!fullScreenUpdate)Bi.Iv(aClip,B.sq(B.sq(aClip,aOffset
),this.G.slice(0,2)),0x00000000,0x00000000,0x00000000,0x00000000,false);C.P.DI.call(
this,Bi,aClip,aOffset,A9,aBlend);},BQ:function(EW,FL){var A;C.P.BQ.call(this,EW,
FL);if(!this.L&&(((EW&0x1)===0x1)||((FL&0x1)===0x1)))this.AJ([0,0,(A=this.G)[2]-
A[0],A[3]-A[1]]);if(!this.L&&(((EW&0x2)===0x2)||((FL&0x2)===0x2)))this.AJ([0,0,(
A=this.G)[2]-A[0],A[3]-A[1]]);},Gd:function(E){if((E!==this.Co)||!E)C.P.Gd.call(
this,E);},Ar:function(E){var A;var FV=this.Bc;C.P.Ar.call(this,E);if(((FV!==this.
Bc)&&!this.L)&&((this.F&0x1)===0x1))this.AJ([0,0,(A=this.G)[2]-A[0],A[3]-A[1]]);
},DispatchEvent:function(Az){if((this.I3>0)&&!!(C.KeyEvent.isPrototypeOf(Az)?Az:
null))return null;if(!!Az){Az.GA=!!this.A_;if(!!this.A_)Az.Bh=this.A_;}var AL=null;
if(!!this.Co){AL=this.Co.DispatchEvent(Az);if(!!AL){this.A_=0;return AL;}}if(!!this.
E3){AL=this.E3.CS.DispatchEvent(Az);if(!AL)AL=this.BU(Az);if(!AL)AL=this.Jk(Az);
this.A_=0;return AL;}AL=C.P.DispatchEvent.call(this,Az);this.A_=0;return AL;},BroadcastEvent:
function(Az,aFilter){if(!!Az){Az.GA=!!this.A_;if(!!this.A_)Az.Bh=this.A_;}var AL=
C.P.BroadcastEvent.call(this,Az,aFilter);this.A_=0;return AL;},AJ:function(AD){var
A;if(this.Hh>0)throw new Error(Gl);if(!!this.Dj&&!this.L){if(((A=this.Dj.D2)[0]>=
A[2])||(A[1]>=A[3])){B.pl(this,0);B.pl(this.Dj,0);}var I_=false;if(I_)this.Dj.D2=[
0,0,(A=this.G)[2]-A[0],A[3]-A[1]];else this.Dj.D2=B.pA(this.Dj.D2,AD);}var fullScreenUpdate=
false;fullScreenUpdate=B.i6;if(fullScreenUpdate)AD=[0,0,(A=this.G)[2]-A[0],A[3]-
A[1]];if(!!this.L){C.P.AJ.call(this,AD);return;}AD=B.hV(B.sq(AD,this.G.slice(0,2
)),this.G);if((AD[0]>=AD[2])||(AD[1]>=AD[3]))return;var K;for(K=0;K<this.BB;K=K+
1)if(!(((A=B.hV(this.BI.Get(K),AD))[0]>=A[2])||(A[1]>=A[3]))){this.BI.Set(K,B.pA(
this.BI.Get(K),AD));this.Cf.Set(K,B.rY(this.BI.Get(K)));return;}if(this.BB<3){this.
BI.Set(this.BB,AD);this.Cf.Set(this.BB,B.rY(AD));this.BB=this.BB+1;return;}var A$;
var CC;var G3=0;var G4=0;var Kn=2147483647;this.BI.Set(this.BB,AD);this.Cf.Set(this.
BB,B.rY(AD));for(A$=0;A$<=this.BB;A$=A$+1)for(CC=A$+1;CC<=this.BB;CC=CC+1){var Im=
B.rY(B.pA(this.BI.Get(A$),this.BI.Get(CC)));var KD=((Im<<8)/(this.Cf.Get(A$)+this.
Cf.Get(CC)))|0;if(KD<Kn){Kn=KD;G3=A$;G4=CC;}}this.BI.Set(G3,B.pA(this.BI.Get(G3)
,this.BI.Get(G4)));this.Cf.Set(G3,B.rY(this.BI.Get(G3)));if(G4!==this.BB){this.BI.
Set(G4,this.BI.Get(this.BB));this.Cf.Set(G4,this.Cf.Get(this.BB));}},IN:function(
Q,Cd){if(!Q)throw new Error(Fh);if(((Cd>0)&&!!this.Co)&&(this.Co.L===this)){var M=
Q;var Ig=Cd;while((M.AP!==this.Co)&&(Ig>0)){M=M.AP;Ig=Ig-1;}Cd=Cd-Ig;}C.P.IN.call(
this,Q,Cd);},N:function(Q,Cd){if(((Cd>=0)&&!!this.Co)&&(this.Co.L===this))Cd=-1;
C.P.N.call(this,Q,Cd);},NF:function(){var B_=B._NewObject(C.Hs,0);B_.GA=!!this.A_;
if(!!this.A_)B_.Bh=this.A_;return B_;},FO:function(){var B_=B._NewObject(C.Hp,0);
B_.GA=!!this.A_;if(!!this.A_)B_.Bh=this.A_;return B_;},Gr:function(){var B_=B._NewObject(
C.Is,0);B_.GA=!!this.A_;if(!!this.A_)B_.Bh=this.A_;return B_;},NG:function(Aa){var
K;var FT=false;for(K=0;K<10;K=K+1)if(!!this.A4.Get(K)){var AX=this.CR.Get(K);var
Ag=this.A4.Get(K).L;while(!!Ag&&(Ag!==this)){AX=B.sn(AX,Ag.G.slice(0,2));Ag=Ag.L;
}if(!Ag&&(this.A4.Get(K)!==this)){var tmp=this.A4.Get(K);this.AE=K;this.A4.Set(K
,null);tmp.BU(this.FO().InitializeUp(K,this.EX.Get(K),this.Ey.Get(K),this.DC.Get(
K),this.BO.Get(K)+1,this.DB.Get(K),false,this.CR.Get(K),this.D_.Get(K)));if(tmp===
this.D9)this.D9=null;this.BroadcastEvent(this.Gr().InitializeUp(K,this.BO.Get(K)+
1,false,tmp,this.CR.Get(K)),0x18);}else{this.DC.Set(K,(this.Ez.Bh-this.Gs.Get(K)
)|0);if(this.DC.Get(K)<10)this.DC.Set(K,10);this.AE=K;this.A4.Get(K).BU(this.FO(
).InitializeHold(K,AX,this.Ey.Get(K),this.DC.Get(K),this.BO.Get(K)+1,this.DB.Get(
K),this.CR.Get(K),this.D_.Get(K)));FT=true;}}if(!FT)this.Ez.A8(false);},GetFPS:function(
){var ticksCount=0;var Kw=0;ticksCount=((new Date).getTime()-B.pk)|0;if(!!this.H7&&(
ticksCount>this.H7))Kw=((this.H6*1000)/((ticksCount-this.H7)|0))|0;this.H6=0;this.
H7=ticksCount;return Kw;},Update:function(){var A;if(!this.Gq){this.Gq=B._NewObject(
B.Graphics.Canvas,0);this.Gq.IF([(A=this.G)[2]-A[0],A[3]-A[1]]);}this.Gq.Update(
);return this.UpdateGE20(this.Gq);},UpdateGE20:function(Bi){if(!this.BeginUpdate(
))return BF;var EE=this.UpdateCanvas(Bi,AO);this.EndUpdate();return EE;},EndUpdate:
function(){if(this.BB>0){this.H6=this.H6+1;this.BB=0;}},UpdateCanvas:function(Bi
,aOffset){var A;var EE=BF;var ND=[].concat(aOffset,B.so(Bi.FrameSize,aOffset));var
K;var A$=this.BB;this.Hh=this.Hh+1;for(K=0;(K<A$)&&(K<4);K=K+1){if(this.Cf.Get(K
)>0){this.DI(Bi,B.sp(this.BI.Get(K),aOffset),[-aOffset[0],-aOffset[1]],255,true);
EE=B.pA(EE,B.hV(ND,this.BI.Get(K)));}else A$=A$+1;}this.Hh=this.Hh-1;if(!((EE[0]>=
EE[2])||(EE[1]>=EE[3])))return B.sp(EE,aOffset);else return EE;},GetUpdateRegion:
function(HX){var K;var A$=this.BB;if(HX<0)return BF;for(K=0;(K<A$)&&(K<4);K=K+1){
if(!this.Cf.Get(K)){A$=A$+1;HX=HX+1;}else if(K===HX)return this.BI.Get(K);}return BF;
},BeginUpdate:function(){var NR=true;var fullScreenUpdate=false;var K;if((!NR&&!
fullScreenUpdate)&&(this.BB>0)){var KJ=B.sr(3,B.pn,null);var Jp=this.BB;for(K=0;
K<Jp;K=K+1)KJ.Set(K,this.BI.Get(K));for(K=0;K<this.Kz;K=K+1)this.AJ(this.Ic.Get(
K));for(K=0;K<Jp;K=K+1)this.Ic.Set(K,KJ.Get(K));this.Kz=Jp;}var A$;var CC;for(A$=
0;A$<(this.BB-1);A$=A$+1)if(this.Cf.Get(A$)>0)for(CC=A$+1;CC<this.BB;CC=CC+1)if(
this.Cf.Get(CC)>0){var Im=B.rY(B.pA(this.BI.Get(A$),this.BI.Get(CC)));if(((Im-this.
Cf.Get(A$))-this.Cf.Get(CC))<0){this.BI.Set(A$,B.pA(this.BI.Get(A$),this.BI.Get(
CC)));this.Cf.Set(A$,Im);this.Cf.Set(CC,0);}}for(K=this.BB-1;K>=0;K=K-1)if(!this.
Cf.Get(K))this.BB=this.BB-1;return this.BB;},DoesNeedUpdate:function(){if(this.BB>
0)return true;return false;},Initialize:function(aSize){this.H([].concat(AO,aSize
));if(this.Ja)this.F=this.F|0x60;else this.F=this.F|0x20;this.AJ(this.G);return this;
},SetRootFocus:function(IY){if(IY===this.Ja)return false;this.Ja=IY;if(!IY){if(!
!this.Co)this.Co.BQ(0x0,0x40);if(!!this.E3)this.E3.CS.BQ(0x0,0x40);else this.BQ(
0x0,0x40);}else{if(!!this.E3)this.E3.CS.BQ(0x40,0x0);else this.BQ(0x40,0x0);if(!
!this.Co)this.Co.BQ(0x40,0x0);}return true;},SetUserInputTimestamp:function(NC){
this.A_=NC;},DriveKeyboardHitting:function(Bx,FK,CP){var A;var Jl=!!this.CD;if(!
!this.CD&&((!CP||(this.Fp!==Bx))||(this.E2!==FK))){var B_=null;var M=(C.Cw.isPrototypeOf(
A=this.CD)?A:null);var DU=(C.GB.isPrototypeOf(A=this.CD)?A:null);if(!!this.Fp)B_=
B._NewObject(C.KeyEvent,0).Initialize(this.Fp,false);if(this.E2!==0x00)B_=B._NewObject(
C.KeyEvent,0).Initialize2(this.E2,false);if(!!DU)DU.BU(B_);else if(!!M)M.BU(B_);
this.Fp=0;this.E2=0x00;this.CD=null;}if(!!this.CD){var B_=null;var M=(C.Cw.isPrototypeOf(
A=this.CD)?A:null);var DU=(C.GB.isPrototypeOf(A=this.CD)?A:null);if(!!Bx)B_=B._NewObject(
C.KeyEvent,0).Initialize(Bx,true);if(this.E2!==0x00)B_=B._NewObject(C.KeyEvent,0
).Initialize2(FK,true);if(!!DU)DU.BU(B_);else if(!!M)M.BU(B_);}if(this.Ib&&((!CP||(
this.Fp!==Bx))||(this.E2!==FK))){this.Fp=0;this.E2=0x00;this.Ib=false;}if((!this.
CD&&CP)&&(this.I3>0)){this.Fp=Bx;this.E2=FK;this.Ib=true;}if((!this.CD&&CP)&&!this.
Ib){if(!!Bx)this.CD=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize(Bx,
true));if(FK!==0x00)this.CD=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize2(
FK,true));if(!(C.GB.isPrototypeOf(A=this.CD)?A:null)&&!(C.Cw.isPrototypeOf(A=this.
CD)?A:null))this.CD=null;this.Fp=Bx;this.E2=FK;Jl=Jl||!!this.CD;}this.A_=0;return Jl;
},DriveCursorMovement:function(By){return this.DriveMultiTouchMovement(this.AE,By
);},DriveMultiTouchMovement:function(S,By){if((S<0)||(S>9)){this.A_=0;return false;
}var C6=B.sn(By,this.CR.Get(S));this.CR.Set(S,By);if(!this.A4.Get(S)||B.sa(C6,AO
)){this.A_=0;return false;}var AX=By;var Ag=this.A4.Get(S).L;while(!!Ag&&(Ag!==this
)){AX=B.sn(AX,Ag.G.slice(0,2));Ag=Ag.L;}if(!Ag&&(this.A4.Get(S)!==this)){var tmp=
this.A4.Get(S);this.AE=S;this.A4.Set(S,null);tmp.BU(this.FO().InitializeUp(S,this.
EX.Get(S),this.Ey.Get(S),this.DC.Get(S),this.BO.Get(S)+1,this.DB.Get(S),false,this.
CR.Get(S),this.D_.Get(S)));if(tmp===this.D9)this.D9=null;this.BroadcastEvent(this.
Gr().InitializeUp(S,this.BO.Get(S)+1,false,tmp,By),0x18);}else{this.EX.Set(S,AX);
this.AE=S;this.A4.Get(S).BU(this.NF().Initialize(S,AX,this.Ey.Get(S),C6,this.DC.
Get(S),this.BO.Get(S)+1,this.DB.Get(S),By,this.D_.Get(S)));}this.A_=0;return true;
},DriveCursorHitting:function(CP,S,By){return this.DriveMultiTouchHitting(CP,S,By
);},DriveMultiTouchHitting:function(CP,S,By){var A;if((S<0)||(S>9)){this.A_=0;return false;
}var ticksCount=this.A_;var FQ=[].concat([-this.E6,-this.E6],[this.E6+1,this.E6+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.pk)|0;}var NX=this.A_;this.
DriveMultiTouchMovement(S,By);By=this.CR.Get(S);this.A_=NX;if(CP)this.D_.Set(S,By
);if((CP&&!this.A4.Get(S))&&!this.I3){var CB=null;var AX=By;if(B.se(this.H2.Get(
S),By)&&((ticksCount-this.Gs.Get(S))<=(((A=this.KY)<0)?A+0x100000000:A)))this.BO.
Set(S,this.BO.Get(S)+1);else this.BO.Set(S,0);this.H2.Set(S,B.sq(FQ,By));this.Gs.
Set(S,ticksCount);if((!!this.Co&&!!this.Co.L)&&((this.Co.F&0x18)===0x18)){var Al=
B.sq(FQ,this.Co.L.JH(By));CB=this.Co.EM(Al,S,this.BO.Get(S)+1,null,0x0);}if(!CB){
if(!!this.D9&&!!this.D9.L){if(((this.D9.F&0x8)===0x8)&&((this.D9.F&0x10)===0x10)
){var Al=B.sq(FQ,this.D9.L.JH(By));CB=this.D9.EM(Al,S,this.BO.Get(S)+1,null,0x0);
}}else if(!!this.E3)CB=this.EM(B.sq(FQ,By),S,this.BO.Get(S)+1,this.E3.CS,0x0);else
CB=this.EM(B.sq(FQ,By),S,this.BO.Get(S)+1,null,0x0);}if(!!CB){this.BroadcastEvent(
this.Gr().InitializeDown(S,this.BO.Get(S)+1,false,CB.Cw,By),0x18);this.A4.Set(S,
CB.Cw);this.DB.Set(S,CB.ER);}else{this.A4.Set(S,null);this.DB.Set(S,AO);this.A_=
0;return false;}var Ag=CB.Cw.L;while(!!Ag&&(Ag!==this)){AX=B.sn(AX,Ag.G.slice(0,
2));Ag=Ag.L;}this.Ey.Set(S,AX);this.EX.Set(S,AX);this.DC.Set(S,0);this.Ez.A8(true
);this.AE=S;this.A4.Get(S).BU(this.FO().InitializeDown(S,AX,this.BO.Get(S)+1,this.
DB.Get(S),false,By));this.A_=0;return true;}if(!CP&&!!this.A4.Get(S)){var AX=By;
var Ag=this.A4.Get(S).L;while(!!Ag&&(Ag!==this)){AX=B.sn(AX,Ag.G.slice(0,2));Ag=
Ag.L;}if(!Ag)AX=this.EX.Get(S);this.AE=S;var tmp=this.A4.Get(S);this.A4.Set(S,null
);tmp.BU(this.FO().InitializeUp(S,AX,this.Ey.Get(S),this.DC.Get(S),this.BO.Get(S
)+1,this.DB.Get(S),false,By,this.D_.Get(S)));this.BroadcastEvent(this.Gr().InitializeUp(
S,this.BO.Get(S)+1,false,tmp,By),0x18);this.A_=0;return true;}this.A_=0;return false;
},GQ:function(Ew,Kk,FN){var FQ=[].concat([-this.E6,-this.E6],[this.E6+1,this.E6+
1]);if(Ew===this)Ew=null;if(!this.A4.Get(this.AE))return;var CB;CB=this.EM(B.sq(
FQ,this.CR.Get(this.AE)),this.AE,1,Ew,FN);if(!!CB&&(this.A4.Get(this.AE)!==CB.Cw
))this.Jy(CB.Cw,CB.ER);if(!CB&&(this.A4.Get(this.AE)!==Kk))this.Jy(Kk,AO);},Jy:function(
Ew,Dx){if(!this.A4.Get(this.AE)||(this.A4.Get(this.AE)===Ew))return;var tmp=this.
A4.Get(this.AE);this.A4.Set(this.AE,null);tmp.BU(this.FO().InitializeUp(this.AE,
this.EX.Get(this.AE),this.Ey.Get(this.AE),this.DC.Get(this.AE),this.BO.Get(this.
AE)+1,this.DB.Get(this.AE),true,this.CR.Get(this.AE),this.D_.Get(this.AE)));this.
BroadcastEvent(this.Gr().InitializeUp(this.AE,this.BO.Get(this.AE)+1,true,tmp,this.
CR.Get(this.AE)),0x18);var AX=this.CR.Get(this.AE);var Ag=null;if(!!Ew)Ag=Ew.L;while(
!!Ag&&(Ag!==this)){AX=B.sn(AX,Ag.G.slice(0,2));Ag=Ag.L;}if(!Ag&&(Ew!==this)){this.
A4.Set(this.AE,null);this.Ez.A8(false);return;}this.BroadcastEvent(this.Gr().InitializeDown(
this.AE,this.BO.Get(this.AE)+1,true,Ew,this.CR.Get(this.AE)),0x18);var ticksCount=
0;ticksCount=((new Date).getTime()-B.pk)|0;this.A4.Set(this.AE,Ew);this.DB.Set(this.
AE,Dx);this.Ey.Set(this.AE,AX);this.EX.Set(this.AE,AX);this.BO.Set(this.AE,0);this.
DC.Set(this.AE,0);this.Gs.Set(this.AE,ticksCount);this.D_.Set(this.AE,this.CR.Get(
this.AE));this.A4.Get(this.AE).BU(this.FO().InitializeDown(this.AE,AX,this.BO.Get(
this.AE)+1,this.DB.Get(this.AE),true,this.D_.Get(this.AE)));},_Init:function(aArg
){C.P._Init.call(this,aArg);C.Timer._Init.call(this.Ez={J:this},0);(this.A4=[]).
__proto__=C.Root.A4;(this.BO=[]).__proto__=C.Root.BO;(this.H2=[]).__proto__=C.Root.
H2;(this.DC=[]).__proto__=C.Root.DC;(this.Ey=[]).__proto__=C.Root.Ey;(this.Gs=[]
).__proto__=C.Root.Gs;(this.EX=[]).__proto__=C.Root.EX;(this.DB=[]).__proto__=C.
Root.DB;(this.CR=[]).__proto__=C.Root.CR;(this.D_=[]).__proto__=C.Root.D_;(this.
Ic=[]).__proto__=C.Root.Ic;(this.Cf=[]).__proto__=C.Root.Cf;(this.BI=[]).__proto__=
C.Root.BI;this.__proto__=C.Root;this.F=0x7F;this.Ez.GJ(50);this.Ez.FC=[this,this.
NG];},_Done:function(){this.__proto__=C.Root;this.Ez._Done();C.P._Done.call(this
);},_ReInit:function(){C.P._ReInit.call(this);this.Ez._ReInit();},_Mark:function(
D){var A;C.P._Mark.call(this,D);if((A=this.CD)&&(A._cycle!=D))A._Mark(A._cycle=D
);if((A=this.E3)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.D9)&&(A._cycle!=D
))A._Mark(A._cycle=D);B.sj(this.A4,D);if((A=this.Gq)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Co)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ez)._cycle!=D)A.
_Mark(A._cycle=D);},_className:"Core::Root"};C.Event={Bh:0,GA:false,Hz:function(
){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk)|0;return ticksCount;},
Bb:function(aArg){this.Bh=this.Hz();},_Init:function(aArg){this.__proto__=C.Event;
this.Bb(aArg);B.gb++;},_Done:function(){this.__proto__=C.Event;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=
D);},J:null,_cycle:0,_observers:null,_className:"Core::Event"};C.KeyEvent={BA:0,
AM:0,Down:false,Initialize2:function(Bx,CP){this.BA=0;this.AM=Bx;this.Down=CP;if((
Bx>=0x30)&&(Bx<=0x39))this.BA=(10+Bx)-48;if((Bx>=0x41)&&(Bx<=0x5A))this.BA=(105+
Bx)-65;if((Bx>=0x61)&&(Bx<=0x7A))this.BA=(105+Bx)-97;if(Bx===0x20)this.BA=131;if(
!this.BA)switch(Bx){case 0x2B:this.BA=132;break;case 0x2D:this.BA=133;break;case
0x2A:this.BA=134;break;case 0x2F:this.BA=135;break;case 0x3D:this.BA=136;break;case
0x2E:this.BA=137;break;case 0x2C:this.BA=138;break;case 0x3A:this.BA=139;break;case
0x3B:this.BA=140;break;default:;}return this;},Initialize:function(Bx,CP){this.BA=
Bx;this.Down=CP;this.AM=0x00;var I5=Bx-10;var I4=Bx-105;if((I5>=0)&&(I5<=9))this.
AM=(48+I5)&0xFFFF;if((I4>=0)&&(I4<=25))this.AM=(65+I4)&0xFFFF;if(Bx===131)this.AM=
0x20;if(this.AM===0x00)switch(Bx){case 132:this.AM=0x2B;break;case 133:this.AM=0x2D;
break;case 134:this.AM=0x2A;break;case 135:this.AM=0x2F;break;case 136:this.AM=0x3D;
break;case 137:this.AM=0x2E;break;case 138:this.AM=0x2C;break;case 139:this.AM=0x3A;
break;case 140:this.AM=0x3B;break;default:;}return this;},K_:function(Kj){switch(
Kj){case 141:return((this.AM>=0x41)&&(this.AM<=0x5A))||((this.AM>=0x61)&&(this.AM<=
0x7A));case 142:return(((this.AM>=0x41)&&(this.AM<=0x5A))||((this.AM>=0x61)&&(this.
AM<=0x7A)))||((this.AM>=0x30)&&(this.AM<=0x39));case 143:return(this.AM>=0x30)&&(
this.AM<=0x39);case 144:return(((this.AM>=0x41)&&(this.AM<=0x46))||((this.AM>=0x61
)&&(this.AM<=0x66)))||((this.AM>=0x30)&&(this.AM<=0x39));case 145:return this.AM
!==0x00;case 146:return(this.AM===0x00)&&!!this.BA;case 147:return(((this.BA===6
)||(this.BA===7))||(this.BA===4))||(this.BA===5);case 148:return(this.AM!==0x00)||
!!this.BA;default:;}return Kj===this.BA;},_Init:function(aArg){C.Event._Init.call(
this,aArg);this.__proto__=C.KeyEvent;},_className:"Core::KeyEvent"};C.Is={IQ:null
,DK:B.pm,Dt:0,CV:0,Down:false,EH:false,InitializeUp:function(S,CQ,FJ,I1,DT){this.
Down=false;this.CV=S;this.Dt=CQ;this.DK=DT;this.IQ=I1;this.EH=FJ;return this;},InitializeDown:
function(S,CQ,FJ,I1,DT){this.Down=true;this.CV=S;this.Dt=CQ;this.DK=DT;this.IQ=I1;
this.EH=FJ;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.
__proto__=C.Is;},_Mark:function(D){var A;C.Event._Mark.call(this,D);if((A=this.IQ
)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"};C.Hp={
EP:B.pm,DK:B.pm,Dt:0,C9:0,DM:B.pm,CU:B.pm,CV:0,Down:false,EH:false,InitializeHold:
function(S,Fi,GV,GW,CQ,Dx,DT,GU){this.Down=true;this.CV=S;this.CU=B.so(Fi,Dx);this.
DM=B.so(GV,Dx);this.C9=GW;this.Dt=CQ;this.DK=DT;this.EP=GU;return this;},InitializeUp:
function(S,Fi,GV,GW,CQ,Dx,FJ,DT,GU){this.Down=false;this.CV=S;this.CU=B.so(Fi,Dx
);this.DM=B.so(GV,Dx);this.C9=GW;this.Dt=CQ;this.EH=FJ;this.DK=DT;this.EP=GU;return this;
},InitializeDown:function(S,Fi,CQ,Dx,FJ,DT){this.Down=true;this.CV=S;this.CU=B.so(
Fi,Dx);this.DM=B.so(Fi,Dx);this.C9=0;this.Dt=CQ;this.EH=FJ;this.DK=DT;this.EP=DT;
return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=
C.Hp;},_className:"Core::CursorEvent"};C.Hs={EP:B.pm,DK:B.pm,Dt:0,C9:0,ER:B.pm,DM:
B.pm,CU:B.pm,CV:0,Initialize:function(S,Fi,GV,aOffset,GW,NA,Dx,DT,GU){this.CV=S;
this.CU=B.so(Fi,Dx);this.DM=B.so(GV,Dx);this.ER=aOffset;this.C9=GW;this.Dt=NA;this.
DK=DT;this.EP=GU;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg
);this.__proto__=C.Hs;},_className:"Core::DragEvent"};C.GM={D6:B.pm,Space:0,Hx:0
,DI:function(Bi,aClip,aOffset,A9,aBlend){},H:function(E){var A;if(B.sb(E,this.G)
)return;var Fs=[(A=this.G)[2]-A[0],A[3]-A[1]];var Ie=[E[2]-E[0],E[3]-E[1]];var F0=
!B.sa(Fs,Ie);var C6=B.sn(E.slice(0,2),this.G.slice(0,2));if(!B.sa(C6,AO)&&!F0){var
M=this.AP;while(!!M&&!((M.F&0x200)===0x200)){if(((M.F&0x400)===0x400)){var tmp=((
M.F&0x100)===0x100);M.Iz(C6,tmp);}M=M.AP;}}if((F0&&(Fs[0]>0))&&(Fs[1]>0)){var As=
B.sq(this.G,this.D6);var M=this.AP;var GZ=0x14;while(!!M&&!((M.F&0x200)===0x200)
){if(((M.F&0x400)===0x400)){if(!!M.A2&&(M.A2.FW!==this))M.A2=null;if(!M.A2&&((M.
E$!==GZ)||!!this.Hx))M.G1(As,this);}M=M.AP;}}C.BZ.H.call(this,E);if(!!this.L&&F0
){this.F=this.F|0x1000;if(!((this.L.F&0x2000)===0x2000)){this.L.F=this.L.F|0x4000;
B.kD([A=this.L,A.Cg],this);}}},_Init:function(aArg){C.BZ._Init.call(this,aArg);this.
__proto__=C.GM;this.F=0x203;},_className:"Core::Outline"};C.Ae={GE:null,Fc:null,
Fb:null,GI:null,C$:null,C_:null,I2:0,CV:0,Bh:0,Dt:0,C9:0,ER:B.pm,DM:B.pm,CU:B.pm
,Lt:1000,HL:8,FD:0,JG:-1,K$:1,La:1,K1:false,Down:false,Cj:false,EH:false,Km:false
,DI:function(Bi,aClip,aOffset,A9,aBlend){},BU:function(Az){var A;var B$=(C.Hp.isPrototypeOf(
Az)?Az:null);var BS=(C.Hs.isPrototypeOf(Az)?Az:null);var H9=this.Cj;if(!B$&&!BS)
return null;if(!!B$){this.Km=B$.Down;this.Down=B$.Down;this.Cj=this.Ix(B$.CU);this.
DM=B$.DM;this.CU=B$.CU;this.ER=AO;this.C9=B$.C9;this.Dt=B$.Dt;this.EH=B$.EH;this.
CV=B$.CV;this.I2=B$.CV;this.Bh=B$.Bh;if(B$.Down&&!B$.C9)H9=false;}if(!!BS){this.
Down=true;this.Cj=this.Ix(BS.CU);this.DM=BS.DM;this.CU=BS.CU;this.ER=BS.ER;this.
C9=BS.C9;this.Dt=BS.Dt;this.CV=BS.CV;this.I2=BS.CV;this.EH=false;this.Bh=BS.Bh;(
A=this.GE)?A[1].call(A[0],this):null;}if((!!B$&&this.Down)&&!this.C9)(A=this.C_)?
A[1].call(A[0],this):null;if((!!B$&&this.Down)&&(this.C9>0))(A=this.GI)?A[1].call(
A[0],this):null;if((this.Down&&this.Cj)&&!H9)(A=this.Fb)?A[1].call(A[0],this):null;
if(((this.Down&&!this.Cj)&&H9)||((!this.Down&&this.Cj)&&H9))(A=this.Fc)?A[1].call(
A[0],this):null;if(!!B$&&!this.Down)(A=this.C$)?A[1].call(A[0],this):null;if(!!this.
FD){if(((((this.FD&0x10)===0x10)&&!!B$)&&B$.Down)&&(B$.C9>=this.Lt))this.Ga().GQ(
null,this,0x10);if((((this.FD&0x1)===0x1)&&!!BS)&&((BS.DK[1]-BS.EP[1])<=-this.HL
))this.Ga().GQ(null,this,0x1);if((((this.FD&0x2)===0x2)&&!!BS)&&((BS.DK[1]-BS.EP[
1])>=this.HL))this.Ga().GQ(null,this,0x2);if((((this.FD&0x4)===0x4)&&!!BS)&&((BS.
DK[0]-BS.EP[0])<=-this.HL))this.Ga().GQ(null,this,0x4);if((((this.FD&0x8)===0x8)&&
!!BS)&&((BS.DK[0]-BS.EP[0])>=this.HL))this.Ga().GQ(null,this,0x8);}return this;}
,EM:function(AD,S,CQ,Go,FN){var A;if(!!Go&&(Go!==this))return null;if((CQ<this.La
)||(CQ>this.K$))return null;if((this.JG>=0)&&(S!==this.JG))return null;if((!this.
K1&&this.Km)&&(S!==this.I2))return null;if(!!(FN&this.FD))return null;if(this.K7(
)){var DW=B.hV(AD,this.GetExtent());if(!((DW[0]>=DW[2])||(DW[1]>=DW[3]))){var Fm=
B.rZ(AD);var C6=AO;if(Fm[0]<DW[0])C6=[DW[0]-Fm[0],C6[1]];if(Fm[0]>=DW[2])C6=[(DW[
2]-Fm[0])-1,C6[1]];if(Fm[1]<DW[1])C6=[C6[0],DW[1]-Fm[1]];if(Fm[1]>=DW[3])C6=[C6[
0],(DW[3]-Fm[1])-1];return B._NewObject(C.Hq,0).Initialize(this,C6);}}else{var A5=
B.sr(9,B.pm,null);var K;A5.Set(0,B.rZ(AD));A5.Set(1,A5.Get(0));A5.Set(2,A5.Get(0
));A5.Set(3,A5.Get(0));A5.Set(4,A5.Get(0));A5.Set(1,[AD[0],A5.Get(1)[1]]);A5.Set(
2,[A5.Get(2)[0],AD[1]]);A5.Set(3,[AD[2],A5.Get(3)[1]]);A5.Set(4,[A5.Get(4)[0],AD[
3]]);A5.Set(5,AD.slice(0,2));A5.Set(6,[AD[2],AD[1]]);A5.Set(7,[AD[0],AD[3]]);A5.
Set(8,AD.slice(2,4));for(K=0;K<9;K=K+1)if(this.Ix(A5.Get(K)))return B._NewObject(
C.Hq,0).Initialize(this,B.sn(A5.Get(K),A5.Get(0)));}return null;},A8:function(E){
if(E)this.BQ(0x10,0x0);else this.BQ(0x0,0x10);},_Init:function(aArg){C.GN._Init.
call(this,aArg);this.__proto__=C.Ae;this.F=0x11B;},_Mark:function(D){var A;C.GN.
_Mark.call(this,D);if((A=this.GE)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((
A=this.Fc)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fb)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.GI)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.C$)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.C_)&&((A=A[
0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SimpleTouchHandler"};C.GB={
AP:null,C$:null,C_:null,GI:null,FZ:0,Bh:0,JP:0,K5:148,BA:0,AM:0,Ep:true,Down:false
,IM:false,Ho:false,BU:function(Az){var A;if(!!Az&&Az.K_(this.K5)){this.Down=Az.Down;
this.BA=Az.BA;this.AM=Az.AM;this.Bh=Az.Bh;this.Ho=false;if(Az.Down){this.JP=this.
FZ;this.IM=this.FZ>0;if(this.IM)(A=this.GI)?A[1].call(A[0],this):null;else(A=this.
C_)?A[1].call(A[0],this):null;if(!this.Ho)this.FZ=this.FZ+1;return!this.Ho;}if(!
Az.Down){this.IM=this.FZ>1;this.JP=this.FZ-1;this.FZ=0;(A=this.C$)?A[1].call(A[0
],this):null;return!this.Ho;}}return false;},Bb:function(aArg){var A;var CS=(C.P.
isPrototypeOf(A=this.J)?A:null);if(!CS)throw new Error(Gm);this.AP=CS.Ia;CS.Ia=this;
},_Init:function(aArg){this.__proto__=C.GB;this.Bb(aArg);B.gb++;},_Done:function(
){this.__proto__=C.GB;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.AP)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.C$)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.C_)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.GI)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"
};C.Hq={Cw:null,Gx:0,ER:B.pm,Initialize:function(Q,aOffset){this.Cw=Q;this.ER=aOffset;
this.Gx=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:function(
aArg){this.__proto__=C.Hq;B.gb++;},_Done:function(){this.__proto__=C.Hq;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.Cw)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::CursorHit"};C.JJ={CS:null,_Init:function(aArg){this.__proto__=
C.JJ;B.gb++;},_Done:function(){this.__proto__=C.JJ;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.CS)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::ModalContext"};C.GD={FW:null,CA:B.pn,As:B.pn,isEmpty:false,_Init:function(
aArg){this.__proto__=C.GD;B.gb++;},_Done:function(){this.__proto__=C.GD;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.FW)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::LayoutContext"};C.HA={Hb:B.pm,Ha:B.pm,G$:B.pm,G_:B.pm,_Init:
function(aArg){C.GD._Init.call(this,aArg);this.__proto__=C.HA;},_className:"Core::LayoutQuadContext"
};C.Jz={CS:null,_Init:function(aArg){this.__proto__=C.Jz;B.gb++;},_Done:function(
){this.__proto__=C.Jz;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.CS)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(
A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::DialogContext"};
C.JT={I7:null,C5:null,_Init:function(aArg){this.__proto__=C.JT;B.gb++;},_Done:function(
){this.__proto__=C.JT;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.I7)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.C5)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::TaskQueue"};C.JS={_Init:function(aArg){this.__proto__=C.JS;
B.gb++;},_Done:function(){this.__proto__=C.JS;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:
0,_observers:null,_className:"Core::Task"};C.Ff={resource:null,DZ:function(){this.
resource=null;},Bb:function(aArg){this.resource=aArg;},_Init:function(aArg){this.
__proto__=C.Ff;this.Bb(aArg);B.gb++;},_Done:function(){this.__proto__=C.Ff;this.
DZ();B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::Resource"
};C.Timer={FC:null,timer:null,Bh:0,Period:1000,Hm:0,Ep:false,DZ:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},Ii:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.rz(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},GJ:function(E){if(E<0)E=0;if(E===this.Period)return;
this.Period=E;if(this.Ep)this.Ii(this.Hm,E);},JM:function(E){if(E<0)E=0;if(E===this.
Hm)return;this.Hm=E;if(this.Ep)this.Ii(E,this.Period);},A8:function(E){if(E===this.
Ep)return;this.Ep=E;if(E)this.Ii(this.Hm,this.Period);else this.Ii(0,0);this.Bh=
this.Hz();},Hz:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk
)|0;return ticksCount;},Trigger:function(){var A;this.Bh=this.Hz();if(!this.Period
)this.A8(false);(A=this.FC)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=C.Timer;B.gb++;},_Done:function(){this.__proto__=C.Timer;this.DZ();B.gb--;
},_ReInit:function(){},_Mark:function(D){var A;if((A=this.FC)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null
,_cycle:0,_observers:null,_className:"Core::Timer"};C.Bh={Lw:0,JI:0,JF:0,KZ:0,Lb:
0,LA:0,NL:function(){var result;result=((new Date).getTime()/1000)|0;return result;
},Kt:function(IZ){switch(IZ){case 0:return 0;case 1:return 31;case 2:return 60;case
3:return 91;case 4:return 121;case 5:return 152;case 6:return 182;case 7:return 213;
case 8:return 244;case 9:return 274;case 10:return 305;case 11:return 335;case 12:
return 366;default:return 0;}},Ks:function(IZ){switch(IZ){case 0:return 0;case 1:
return 31;case 2:return 59;case 3:return 90;case 4:return 120;case 5:return 151;
case 6:return 181;case 7:return 212;case 8:return 243;case 9:return 273;case 10:
return 304;case 11:return 334;case 12:return 365;default:return 0;}},Lp:function(
E){var Dk=(((E/86400)|0)+365)|0;var KM=(Dk/1461)|0;var Fy;var ED;Dk=Dk-(KM*1461);
Fy=(Dk/365)|0;if(Fy===4)Fy=3;Dk=Dk-(Fy*365);Fy=((KM*4)+Fy)+1969;ED=Dk>>5;if(!!(Fy
%4)){if(Dk>=this.Ks(ED+1))ED=ED+1;Dk=Dk-this.Ks(ED);}else{if(Dk>=this.Kt(ED+1))ED=
ED+1;Dk=Dk-this.Kt(ED);}this.Lq(Fy);this.Ln(ED+1);this.Li(Dk+1);this.Lk((((E/3600
)|0)%24)|0);this.Lm((((E/60)|0)%60)|0);this.Lo((E%60)|0);},Le:function(){return B.
_NewObject(C.Bh,0).Initialize(this.NL());},Lo:function(E){if(E<0)E=0;if(E>59)E=59;
this.Lw=E;},Lm:function(E){if(E<0)E=0;if(E>59)E=59;this.JI=E;},Lk:function(E){if(
E<0)E=0;if(E>23)E=23;this.JF=E;},Li:function(E){if(E<1)E=1;if(E>31)E=31;this.KZ=
E;},Ln:function(E){if(E<1)E=1;if(E>12)E=12;this.Lb=E;},Lq:function(E){if(E<1970)
E=1970;this.LA=E;},Initialize:function(NB){this.Lp(NB);return this;},_Init:function(
aArg){this.__proto__=C.Bh;B.gb++;},_Done:function(){this.__proto__=C.Bh;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(
A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::Time"};C.Pc={Pd:
0x1,Oa:0x2,OC:0x4,Pa:0x8,Ep:0x10,IO:0x20,OD:0x40,OJ:0x80,OB:0x100,OF:0x200,Ou:0x400
,OU:0x800,DS:0x1000,Pb:0x2000,OO:0x4000,OP:0x8000,Oo:0x10000,ON:0x20000,O2:0x40000
};C.E$={OV:0x1,OW:0x2,N9:0x4,N_:0x8,N$:0x10,N8:0x20};C.Hx={OK:0,O$:1,Oe:2,OG:3,OY:
4};C.KeyCode={NoKey:0,Ok:1,Exit:2,Menu:3,Up:4,Down:5,Left:6,Right:7,PageUp:8,PageDown:
9,Key0:10,Key1:11,Key2:12,Key3:13,Key4:14,Key5:15,Key6:16,Key7:17,Key8:18,Key9:19
,Red:20,Green:21,Blue:22,Yellow:23,White:24,Magenta:25,F1:26,F2:27,F3:28,F4:29,F5:
30,F6:31,F7:32,F8:33,F9:34,F10:35,ChannelUp:36,ChannelDown:37,Display:38,SkipPrev:
39,SkipNext:40,Home:41,End:42,Insert:43,Delete:44,Clear:45,VolumeUp:46,VolumeDown:
47,Show:48,Hide:49,Play:50,Pause:51,Record:52,Stop:53,Rev:54,Fwd:55,SlowRev:56,SlowFwd:
57,SkipBwd:58,SkipFwd:59,Repeat:60,Eject:61,Help:62,TV:63,DVD:64,VCR:65,EPG:66,OSD:
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
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};C.OX={Ph:0x1,Pe:0x2,Pf:0x4,Pg:0x8,OI:
0x10};
C._Init=function(){C.GN.__proto__=C.Cw;C.BZ.__proto__=C.Cw;C.P.__proto__=C.BZ;C.Root.
__proto__=C.P;C.KeyEvent.__proto__=C.Event;C.Is.__proto__=C.Event;C.Hp.__proto__=
C.Event;C.Hs.__proto__=C.Event;C.GM.__proto__=C.BZ;C.Ae.__proto__=C.GN;C.HA.__proto__=
C.GD;};C.CY=function(D){};return C;})();

/* Embedded Wizard */