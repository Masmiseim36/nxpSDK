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
Core=(function(){var B=EmWiApp;var E={};
var AJ=[0,0];var Be=[0,0,0,0];var Dh="The view does not belong to this group";var
CK="The dialog component is already presented";var Ft="No fader to perform the fade-in/out operation";
var Iy="Trying to use the same fader twice";var Jg="Trying to fade-in/out a group which belongs to another owner";
var Kx="No view to restack";var Ky="View is not in this group";var LD="No view to remove";
var M4="No view to add";var O2="View already in a group";var O3="Recursive invalidate during active update cycle.";
var O4="The method RestackTop() is not available in Core::VerticalList.";var O5=
"The method Remove() is not available in Core::VerticalList.";var O6="The method Add() is not available in Core::VerticalList.";
var O7="The Slide Touch Handler:";var O8="is already connected with a view.";var
O9="The Slide Touch Handler is already connected with a view to scroll "+"its content.";
var O_=[0,0,170,120];var O$="The KeyPressHandler is embedded within an object not being derived "+
"from Core::Group.";var Pa="Trying to cancel a task not belonging to this queue!";
var Pb="Trying to enqueue a task twice!";
E.AE={Ad:null,Ag:null,G:null,AV:null,F:0x103,Jc:0,D7:0x14,KL:function(X,KB){},Dc:
function(C){var A;var Bh=C^this.D7;if(!Bh)return;this.D7=C;if(!!this.AV&&!((this.
F&0x400)===0x400)){this.G.F=this.G.F|0x5000;B.lq([A=this.G,A.C_],this);this.G.Ai([
0,0,(A=this.G.H)[2]-A[0],A[3]-A[1]]);}if(!!this.AV&&((this.F&0x400)===0x400)){this.
AV.IO.F=this.AV.IO.F|0x1000;this.G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);
}},I3:function(){var Al=this.G;while(!!Al){var RX=(E.Root.isPrototypeOf(Al)?Al:null
);if(!!RX)return RX;Al=Al.G;}return null;},Da:function(AA,aClip,aOffset,Aj,aBlend
){},Cq:function(Ak){return null;},Gj:function(AC,T,DS,F$,Fv){return null;},KX:function(
X,C4){return AJ;},Ln:function(aOffset,KA){},GetExtent:function(){return Be;},BB:
function(HQ,Jj){var A;if(((this.F&0x200)===0x200))HQ=HQ&~0x400;var PL=(this.F&~Jj
)|HQ;var IE=PL^this.F;this.F=PL;if(!!this.G&&!!(IE&0x14)){var RC=((this.F&0x14)===
0x14);if(RC&&!this.G.Db)this.G.D_(this);if(!RC&&(this.G.Db===this))this.G.D_(this.
G.Oc(this,0x14));}if(!!this.G&&!!(IE&0x403))this.G.Ai(this.GetExtent());if(((!!this.
AV&&!!this.G)&&((PL&0x400)===0x400))&&((IE&0x1)===0x1)){this.F=this.F|0x800;this.
G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);}if(!!this.G&&((IE&0x400)===0x400
)){this.AV=null;this.F=this.F|0x800;this.G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_
],this);}},_Init:function(aArg){this.__proto__=E.AE;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.
Ad)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ag)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.G)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.AV)&&(A._cycle!=D
))A._Mark(A._cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,
_cycle:0,_observers:null,_className:"Core::View"};E.J$={Bp:B.qx,BM:B.qx,KL:function(
X,KB){var Ab=B._NewObject(E.ME,0);this.AV=null;Ab.CS=this.getExtent();Ab.U=X;Ab.
IO=KB;Ab.H5=this.BM;Ab.H6=this.Bp;this.AV=Ab;},KX:function(X,C4){var A;var AD=this.
D7;var Ab=(E.ME.isPrototypeOf(A=this.AV)?A:null);var x1=Ab.CS[0];var y1=Ab.CS[1];
var x2=Ab.CS[2];var y2=Ab.CS[3];var Ck=[X[2]-X[0],X[3]-X[1]];var Ax=x2-x1;var At=
y2-y1;var tmp;if(!C4){var Dl=[(A=Ab.U)[2]-A[0],A[3]-A[1]];x1=x1-Ab.U[0];y1=y1-Ab.
U[1];if(Dl[0]!==Ck[0]){var Bj=((AD&0x4)===0x4);var Bk=((AD&0x8)===0x8);var El=((
AD&0x1)===0x1);if(!Bj&&(El||!Bk))x1=((x1*Ck[0])/Dl[0])|0;if(!Bk&&(El||!Bj)){x2=x2-
Ab.U[0];x2=((x2*Ck[0])/Dl[0])|0;x2=x2-Ck[0];}else x2=x2-Ab.U[2];x1=x1+X[0];x2=x2+
X[2];if(!El){if(Bj&&!Bk)x2=x1+Ax;else if(!Bj&&Bk)x1=x2-Ax;else{x1=x1+((((x2-x1)-
Ax)/2)|0);x2=x1+Ax;}}}else{x2=x2-Ab.U[2];x1=x1+X[0];x2=x2+X[2];}if(Dl[1]!==Ck[1]
){var Bl=((AD&0x10)===0x10);var Bi=((AD&0x20)===0x20);var Em=((AD&0x2)===0x2);if(
!Bl&&(Em||!Bi))y1=((y1*Ck[1])/Dl[1])|0;if(!Bi&&(Em||!Bl)){y2=y2-Ab.U[1];y2=((y2*
Ck[1])/Dl[1])|0;y2=y2-Ck[1];}else y2=y2-Ab.U[3];y1=y1+X[1];y2=y2+X[3];if(!Em){if(
Bl&&!Bi)y2=y1+At;else if(!Bl&&Bi)y1=y2-At;else{y1=y1+((((y2-y1)-At)/2)|0);y2=y1+
At;}}}else{y2=y2-Ab.U[3];y1=y1+X[1];y2=y2+X[3];}}else{switch(C4){case 3:{x1=X[0];
x2=x1+Ax;}break;case 4:{x2=X[2];x1=x2-Ax;}break;case 1:{y1=X[1];y2=y1+At;}break;
case 2:{y2=X[3];y1=y2-At;}break;default:;}if((C4===3)||(C4===4)){var Bl=((AD&0x10
)===0x10);var Bi=((AD&0x20)===0x20);var Em=((AD&0x2)===0x2);if(Em){y1=X[1];y2=X[
3];}else if(Bl&&!Bi){y1=X[1];y2=y1+At;}else if(Bi&&!Bl){y2=X[3];y1=y2-At;}else{y1=
X[1]+((((X[3]-X[1])-At)/2)|0);y2=y1+At;}}if((C4===1)||(C4===2)){var Bj=((AD&0x4)===
0x4);var Bk=((AD&0x8)===0x8);var El=((AD&0x1)===0x1);if(El){x1=X[0];x2=X[2];}else
if(Bj&&!Bk){x1=X[0];x2=x1+Ax;}else if(Bk&&!Bj){x2=X[2];x1=x2-Ax;}else{x1=X[0]+((((
X[2]-X[0])-Ax)/2)|0);x2=x1+Ax;}}}Ab.isEmpty=(x1>=x2)||(y1>=y2);Ax=x2-x1;At=y2-y1;
if(Ab.H6[0]<Ab.H5[0]){tmp=x1;x1=x2-1;x2=tmp-1;}if(Ab.H6[1]<Ab.H5[1]){tmp=y1;y1=y2-
1;y2=tmp-1;}if((x2-x1)===1)x2=x1;if((y2-y1)===1)y2=y1;if(((this.F&0x100)===0x100
)){this.BM=[x1,y1];this.Bp=[x2,y2];}else{this.Cs([x1,y1]);this.Ct([x2,y2]);this.
AV=Ab;}return[Ax,At];},Ln:function(aOffset,KA){if(KA){this.BM=B.tx(this.BM,aOffset
);this.Bp=B.tx(this.Bp,aOffset);}else{this.Cs(B.tx(this.BM,aOffset));this.Ct(B.tx(
this.Bp,aOffset));}},GetExtent:function(){if(!!this.AV&&this.AV.isEmpty)return Be;
return this.getExtent();},getExtent:function(){var x1=this.BM[0];var y1=this.BM[
1];var x2=this.Bp[0];var y2=this.Bp[1];var Ax=x2-x1;var At=y2-y1;var tmp;if(Ax<0
)Ax=-Ax;if(At<0)At=-At;if(Ax>=At){if(x2>x1)x2=x2-1;if(x2<x1)x2=x2+1;}if(At>=Ax){
if(y2>y1)y2=y2-1;if(y2<y1)y2=y2+1;}if(x2<x1){tmp=x1;x1=x2;x2=tmp;}if(y2<y1){tmp=
y1;y1=y2;y2=tmp;}return[x1,y1,x2+1,y2+1];},Ct:function(C){var A;if(B.tl(C,this.Bp
))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());this.AV=null;
this.Bp=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());if((!!this.
G&&((this.F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;
this.G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);}},Cs:function(C){var A;if(B.
tl(C,this.BM))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent(
));this.AV=null;this.BM=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent(
));if((!!this.G&&((this.F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);}},_Init:function(
aArg){E.AE._Init.call(this,aArg);this.__proto__=E.J$;},_className:"Core::LineView"
};E.Ly={CO:B.qx,DP:B.qx,Bp:B.qx,BM:B.qx,KL:function(X,KB){var Ab=B._NewObject(E.
MF,0);this.AV=null;Ab.CS=this.GetExtent();Ab.U=X;Ab.IO=KB;Ab.H5=this.BM;Ab.H6=this.
Bp;Ab.L_=this.DP;Ab.L$=this.CO;this.AV=Ab;},KX:function(X,C4){var A;var AD=this.
D7;var Ab=(E.MF.isPrototypeOf(A=this.AV)?A:null);var x1=Ab.CS[0];var y1=Ab.CS[1];
var x2=Ab.CS[2];var y2=Ab.CS[3];var Ck=[X[2]-X[0],X[3]-X[1]];var Ax=x2-x1;var At=
y2-y1;if(!C4){var Dl=[(A=Ab.U)[2]-A[0],A[3]-A[1]];x1=x1-Ab.U[0];y1=y1-Ab.U[1];if(
Dl[0]!==Ck[0]){var Bj=((AD&0x4)===0x4);var Bk=((AD&0x8)===0x8);var El=((AD&0x1)===
0x1);if(!Bj&&(El||!Bk))x1=((x1*Ck[0])/Dl[0])|0;if(!Bk&&(El||!Bj)){x2=x2-Ab.U[0];
x2=((x2*Ck[0])/Dl[0])|0;x2=x2-Ck[0];}else x2=x2-Ab.U[2];x1=x1+X[0];x2=x2+X[2];if(
!El){if(Bj&&!Bk)x2=x1+Ax;else if(!Bj&&Bk)x1=x2-Ax;else{x1=x1+((((x2-x1)-Ax)/2)|0
);x2=x1+Ax;}}}else{x2=x2-Ab.U[2];x1=x1+X[0];x2=x2+X[2];}if(Dl[1]!==Ck[1]){var Bl=((
AD&0x10)===0x10);var Bi=((AD&0x20)===0x20);var Em=((AD&0x2)===0x2);if(!Bl&&(Em||
!Bi))y1=((y1*Ck[1])/Dl[1])|0;if(!Bi&&(Em||!Bl)){y2=y2-Ab.U[1];y2=((y2*Ck[1])/Dl[
1])|0;y2=y2-Ck[1];}else y2=y2-Ab.U[3];y1=y1+X[1];y2=y2+X[3];if(!Em){if(Bl&&!Bi)y2=
y1+At;else if(!Bl&&Bi)y1=y2-At;else{y1=y1+((((y2-y1)-At)/2)|0);y2=y1+At;}}}else{
y2=y2-Ab.U[3];y1=y1+X[1];y2=y2+X[3];}}else{switch(C4){case 3:{x1=X[0];x2=x1+Ax;}
break;case 4:{x2=X[2];x1=x2-Ax;}break;case 1:{y1=X[1];y2=y1+At;}break;case 2:{y2=
X[3];y1=y2-At;}break;default:;}if((C4===3)||(C4===4)){var Bl=((AD&0x10)===0x10);
var Bi=((AD&0x20)===0x20);var Em=((AD&0x2)===0x2);if(Em){y1=X[1];y2=X[3];}else if(
Bl&&!Bi){y1=X[1];y2=y1+At;}else if(Bi&&!Bl){y2=X[3];y1=y2-At;}else{y1=X[1]+((((X[
3]-X[1])-At)/2)|0);y2=y1+At;}}if((C4===1)||(C4===2)){var Bj=((AD&0x4)===0x4);var
Bk=((AD&0x8)===0x8);var El=((AD&0x1)===0x1);if(El){x1=X[0];x2=X[2];}else if(Bj&&
!Bk){x1=X[0];x2=x1+Ax;}else if(Bk&&!Bj){x2=X[2];x1=x2-Ax;}else{x1=X[0]+((((X[2]-
X[0])-Ax)/2)|0);x2=x1+Ax;}}}Ab.isEmpty=(x1>=x2)||(y1>=y2);Ax=(x2-x1)-1;At=(y2-y1
)-1;var HV=Ab.CS[0];var HW=Ab.CS[1];var Hj=(Ab.CS[2]-HV)-1;var Hi=(Ab.CS[3]-HW)-
1;if(!Hj)Hj=1;if(!Hi)Hi=1;if(((this.F&0x100)===0x100)){this.BM=[x1+((((Ab.H5[0]-
HV)*Ax)/Hj)|0),y1+((((Ab.H5[1]-HW)*At)/Hi)|0)];this.Bp=[x1+((((Ab.H6[0]-HV)*Ax)/
Hj)|0),y1+((((Ab.H6[1]-HW)*At)/Hi)|0)];this.DP=[x1+((((Ab.L_[0]-HV)*Ax)/Hj)|0),y1+((((
Ab.L_[1]-HW)*At)/Hi)|0)];this.CO=[x1+((((Ab.L$[0]-HV)*Ax)/Hj)|0),y1+((((Ab.L$[1]-
HW)*At)/Hi)|0)];}else{this.Cs([x1+((((Ab.H5[0]-HV)*Ax)/Hj)|0),y1+((((Ab.H5[1]-HW
)*At)/Hi)|0)]);this.Ct([x1+((((Ab.H6[0]-HV)*Ax)/Hj)|0),y1+((((Ab.H6[1]-HW)*At)/Hi
)|0)]);this.G0([x1+((((Ab.L_[0]-HV)*Ax)/Hj)|0),y1+((((Ab.L_[1]-HW)*At)/Hi)|0)]);
this.G1([x1+((((Ab.L$[0]-HV)*Ax)/Hj)|0),y1+((((Ab.L$[1]-HW)*At)/Hi)|0)]);this.AV=
Ab;}return[Ax+1,At+1];},Ln:function(aOffset,KA){if(KA){this.BM=B.tx(this.BM,aOffset
);this.Bp=B.tx(this.Bp,aOffset);this.DP=B.tx(this.DP,aOffset);this.CO=B.tx(this.
CO,aOffset);}else{this.Cs(B.tx(this.BM,aOffset));this.Ct(B.tx(this.Bp,aOffset));
this.G0(B.tx(this.DP,aOffset));this.G1(B.tx(this.CO,aOffset));}},GetExtent:function(
){if(!!this.AV&&this.AV.isEmpty)return Be;var x1=this.BM[0];var y1=this.BM[1];var
x2=this.DP[0];var y2=this.DP[1];if((((this.CO[0]!==x1)||(this.Bp[1]!==y1))||(this.
Bp[0]!==x2))||(this.CO[1]!==y2)){if(this.Bp[0]<x1)x1=this.Bp[0];if(this.DP[0]<x1
)x1=this.DP[0];if(this.CO[0]<x1)x1=this.CO[0];if(this.Bp[1]<y1)y1=this.Bp[1];if(
this.DP[1]<y1)y1=this.DP[1];if(this.CO[1]<y1)y1=this.CO[1];if(this.BM[0]>x2)x2=this.
BM[0];if(this.Bp[0]>x2)x2=this.Bp[0];if(this.CO[0]>x2)x2=this.CO[0];if(this.BM[1
]>y2)y2=this.BM[1];if(this.Bp[1]>y2)y2=this.Bp[1];if(this.CO[1]>y2)y2=this.CO[1];
}else{var tmp;if(x2<x1){tmp=x1;x1=x2;x2=tmp;}if(y2<y1){tmp=y1;y1=y2;y2=tmp;}}return[
x1,y1,x2+1,y2+1];},G1:function(C){var A;if(B.tl(C,this.CO))return;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ai(this.GetExtent());this.AV=null;this.CO=C;if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());if((!!this.G&&((this.F&0x400
)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=this.G.
F|0x4000;B.lq([A=this.G,A.C_],this);}},G0:function(C){var A;if(B.tl(C,this.DP))return;
if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());this.AV=null;this.
DP=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());if((!!this.G&&((
this.F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);}},Ct:function(C){var A;if(B.tl(C
,this.Bp))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());this.
AV=null;this.Bp=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent());
if((!!this.G&&((this.F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);}},Cs:function(C){var
A;if(B.tl(C,this.BM))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent(
));this.AV=null;this.BM=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ai(this.GetExtent(
));if((!!this.G&&((this.F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.G.F=this.G.F|0x4000;B.lq([A=this.G,A.C_],this);}},Oo:function(
Ey){var Bu=B.tA(4,B.qx,null);var I=0;var BA=3;var Pu=false;var Pv=false;Bu.Set(0
,this.BM);Bu.Set(1,this.Bp);Bu.Set(2,this.DP);Bu.Set(3,this.CO);while(I<4){var Se=
Bu.Get(I)[0];var NV=Bu.Get(I)[1];var Wi=Bu.Get(BA)[0];var P5=Bu.Get(BA)[1];if(((
NV>Ey[1])!==(P5>Ey[1]))||((NV<Ey[1])!==(P5<Ey[1]))){var JY=((((Wi-Se)*(Ey[1]-NV)
)/(P5-NV))|0)+Se;if(Ey[0]>JY)Pu=!Pu;if(Ey[0]<JY)Pv=!Pv;}BA=I;I=I+1;}return Pu||Pv;
},SR:function(){return((((this.BM[0]===this.CO[0])&&(this.Bp[0]===this.DP[0]))&&(
this.BM[1]===this.Bp[1]))&&(this.DP[1]===this.CO[1]))||((((this.BM[0]===this.Bp[
0])&&(this.DP[0]===this.CO[0]))&&(this.BM[1]===this.CO[1]))&&(this.Bp[1]===this.
DP[1]));},_Init:function(aArg){E.AE._Init.call(this,aArg);this.__proto__=E.Ly;},
_className:"Core::QuadView"};E.AQ={H:B.qy,KL:function(X,KB){var Ab=B._NewObject(
E.J_,0);Ab.CS=this.H;Ab.U=X;Ab.IO=KB;this.AV=Ab;},KX:function(X,C4){var A;var AD=
this.D7;var Ab=this.AV;var x1=Ab.CS[0];var y1=Ab.CS[1];var x2=Ab.CS[2];var y2=Ab.
CS[3];var Ck=[X[2]-X[0],X[3]-X[1]];var Ax=x2-x1;var At=y2-y1;if(!C4){var Dl=[(A=
Ab.U)[2]-A[0],A[3]-A[1]];x1=x1-Ab.U[0];y1=y1-Ab.U[1];if(Dl[0]!==Ck[0]){var Bj=((
AD&0x4)===0x4);var Bk=((AD&0x8)===0x8);var El=((AD&0x1)===0x1);if(!Bj&&(El||!Bk)
)x1=((x1*Ck[0])/Dl[0])|0;if(!Bk&&(El||!Bj)){x2=x2-Ab.U[0];x2=((x2*Ck[0])/Dl[0])|
0;x2=x2-Ck[0];}else x2=x2-Ab.U[2];x1=x1+X[0];x2=x2+X[2];if(!El){if(Bj&&!Bk)x2=x1+
Ax;else if(!Bj&&Bk)x1=x2-Ax;else{x1=x1+((((x2-x1)-Ax)/2)|0);x2=x1+Ax;}}}else{x2=
x2-Ab.U[2];x1=x1+X[0];x2=x2+X[2];}if(Dl[1]!==Ck[1]){var Bl=((AD&0x10)===0x10);var
Bi=((AD&0x20)===0x20);var Em=((AD&0x2)===0x2);if(!Bl&&(Em||!Bi))y1=((y1*Ck[1])/Dl[
1])|0;if(!Bi&&(Em||!Bl)){y2=y2-Ab.U[1];y2=((y2*Ck[1])/Dl[1])|0;y2=y2-Ck[1];}else
y2=y2-Ab.U[3];y1=y1+X[1];y2=y2+X[3];if(!Em){if(Bl&&!Bi)y2=y1+At;else if(!Bl&&Bi)
y1=y2-At;else{y1=y1+((((y2-y1)-At)/2)|0);y2=y1+At;}}}else{y2=y2-Ab.U[3];y1=y1+X[
1];y2=y2+X[3];}}else{switch(C4){case 3:{x1=X[0];x2=x1+Ax;}break;case 4:{x2=X[2];
x1=x2-Ax;}break;case 1:{y1=X[1];y2=y1+At;}break;case 2:{y2=X[3];y1=y2-At;}break;
default:;}if((C4===3)||(C4===4)){var Bl=((AD&0x10)===0x10);var Bi=((AD&0x20)===0x20
);var Em=((AD&0x2)===0x2);if(Em){y1=X[1];y2=X[3];}else if(Bl&&!Bi){y1=X[1];y2=y1+
At;}else if(Bi&&!Bl){y2=X[3];y1=y2-At;}else{y1=X[1]+((((X[3]-X[1])-At)/2)|0);y2=
y1+At;}}if((C4===1)||(C4===2)){var Bj=((AD&0x4)===0x4);var Bk=((AD&0x8)===0x8);var
El=((AD&0x1)===0x1);if(El){x1=X[0];x2=X[2];}else if(Bj&&!Bk){x1=X[0];x2=x1+Ax;}else
if(Bk&&!Bj){x2=X[2];x1=x2-Ax;}else{x1=X[0]+((((X[2]-X[0])-Ax)/2)|0);x2=x1+Ax;}}}
Ab.isEmpty=(x1>=x2)||(y1>=y2);if(((this.F&0x100)===0x100)){this.H=[x1,y1,x2,y2];
}else{this.P([x1,y1,x2,y2]);this.AV=Ab;}return[x2-x1,y2-y1];},Ln:function(aOffset
,KA){if(KA)this.H=B.tz(this.H,aOffset);else this.P(B.tz(this.H,aOffset));},GetExtent:
function(){return this.H;},P:function(C){var A;if(B.tm(C,this.H))return;if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ai(this.H);this.AV=null;this.H=C;if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ai(this.H);if((!!this.G&&((this.F&0x400)===0x400))&&!((this.
G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=this.G.F|0x4000;B.lq([A=this.
G,A.C_],this);}},_Init:function(aArg){E.AE._Init.call(this,aArg);this.__proto__=
E.AQ;},_className:"Core::RectView"};E.J={Aw:null,AB:null,Nx:null,AU:null,Cj:null
,FA:null,JR:null,Db:null,Dd:255,Da:function(AA,aClip,aOffset,Aj,aBlend){var A;Aj=((
Aj+1)*this.Dd)>>8;aBlend=aBlend&&((this.F&0x2)===0x2);if(!this.AU)this.No(AA,aClip
,B.tx(aOffset,this.H.slice(0,2)),Aj,aBlend);else{var Ca=255|(255<<8)|(255<<16)|((
Aj&0xFF)<<24);this.AU.Update();AA.Mm(aClip,this.AU,0,B.tz(this.H,aOffset),AJ,Ca,
Ca,Ca,Ca,aBlend);}},Gj:function(AC,T,DS,F$,Fv){var A;var K=this.AB;var Jy=null;var
R=Be;var Cc=null;var PH=!!this.FA&&(!!this.FA.Ez||!!this.FA.Aw);if(((A=B.il(AC,this.
H))[0]>=A[2])||(A[1]>=A[3]))return null;AC=B.ty(AC,this.H.slice(0,2));while(!!K){
if(((K.F&0x400)===0x400)&&!Cc){Cc=K.Ag;while(!!Cc&&!((Cc.F&0x200)===0x200))Cc=Cc.
Ag;if(!!Cc)R=B.il(AC,Cc.GetExtent());else R=Be;}if(Cc===K){Cc=null;R=Be;}if((((((
K.F&0x8)===0x8)&&((K.F&0x10)===0x10))&&!((K.F&0x40000)===0x40000))&&!((K.F&0x20000
)===0x20000))&&(!((K.F&0x10000)===0x10000)||((this.Cj.Bz===K)&&!PH))){var CS=K.GetExtent(
);var Nn=F$;var Js=null;if(Nn===K)Nn=null;if(((K.F&0x400)===0x400)){if(!(((A=B.il(
CS,R))[0]>=A[2])||(A[1]>=A[3])))Js=K.Gj(R,T,DS,Nn,Fv);}else{if(!(((A=B.il(CS,AC)
)[0]>=A[2])||(A[1]>=A[3]))||(F$===K))Js=K.Gj(AC,T,DS,Nn,Fv);}K=K.Ag;if(!!Js){if(
!Jy||((Js.K7<Jy.K7)&&(Js.K7>=0)))Jy=Js;if(!Js.K7)K=null;}}else K=K.Ag;}return Jy;
},BB:function(HQ,Jj){var A;var VN=this.F;E.AQ.BB.call(this,HQ,Jj);var IE=this.F^
VN;if(!!this.Db&&((IE&0x40)===0x40)){if(((this.F&0x40)===0x40))this.Db.BB(0x40,0x0
);else this.Db.BB(0x0,0x40);}if(!!this.Cj&&((IE&0x40)===0x40)){if(((this.F&0x40)===
0x40)&&((this.Cj.Bz.F&0x14)===0x14))this.Cj.Bz.BB(0x40,0x0);else this.Cj.Bz.BB(0x0
,0x40);}if(!!IE){this.F=this.F|0x8000;B.lq([this,this.C_],this);}},P:function(C){
var A;if(B.tm(C,this.H))return;var IN=[(A=this.H)[2]-A[0],A[3]-A[1]];var NB=[C[2
]-C[0],C[3]-C[1]];var JU=!B.tl(IN,NB);if(JU&&!!this.AU){this.AU.Ls(NB);B.qw(this
,0);B.qw(this.AU,0);}E.AQ.P.call(this,C);if((JU&&(IN[0]>0))&&(IN[1]>0)){var U=[].
concat(AJ,IN);var K=this.Aw;var LT=0x14;while(!!K){if((!K.AV&&(K.D7!==LT))&&!((K.
F&0x400)===0x400))K.KL(U,null);K=K.Ad;}}if(JU){this.F=this.F|0x5000;B.lq([this,this.
C_],this);}},PW:function(Ak){var RH=(E.KeyEvent.isPrototypeOf(Ak)?Ak:null);var Ge=
this.Nx;if(!RH)return null;while(!!Ge&&(!Ge.B5||!Ge.Cq(RH)))Ge=Ge.Ad;return Ge;}
,We:function(Af){if(!!this.AU){var LU=false;if(!LU)this.AU.Mr(this.AU.EK,this.AU.
EK,0x00000000,0x00000000,0x00000000,0x00000000,false);this.No(this.AU,this.AU.EK
,AJ,255,true);}},No:function(AA,aClip,aOffset,Aj,aBlend){var A;var K=this.Aw;var
Rv=Be;var RR=true;while(!!K){if(((K.F&0x200)===0x200)){var RQ=(E.Lv.isPrototypeOf(
K)?K:null);Rv=B.il(aClip,B.tz(RQ.H,aOffset));RR=((RQ.F&0x1)===0x1);}if(((K.F&0x1
)===0x1)){if(((K.F&0x400)===0x400)){if(RR){var R=B.il(B.tz(K.GetExtent(),aOffset
),Rv);if(!((R[0]>=R[2])||(R[1]>=R[3])))K.Da(AA,R,aOffset,Aj,aBlend);}}else{var R=
B.il(B.tz(K.GetExtent(),aOffset),aClip);if(!((R[0]>=R[2])||(R[1]>=R[3])))K.Da(AA
,R,aOffset,Aj,aBlend);}}K=K.Ad;}},V3:function(){var A;var Ps=((this.F&0x1000)===
0x1000);var HZ=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];var Gc=false;var GE=Be;var CE=
Be;var Gd=AJ;var KI=0;var KJ=0;var KH=0;var Ef=0;var K=this.AB;var Cc=null;var LT=
0x14;var IH=null;while(!!K){if(((K.F&0x800)===0x800)){Gc=true;K.F=K.F&~0x800;}if(
Gc&&((K.F&0x200)===0x200)){Gc=false;if(!!(E.Lv.isPrototypeOf(K)?K:null).Ms)K.F=K.
F|0x1000;}K=K.Ag;}Gc=false;K=this.Aw;if(Ps){this.F=this.F&~0x1000;Ps=!((HZ[0]>=HZ[
2])||(HZ[1]>=HZ[3]));}this.F=this.F|0x2000;while(!!K){if(!IH&&(KH!==Ef)){var GQ=
K;var NO=0;var Md=GE[2]-GE[0];var LN=GE[3]-GE[1];var Nk=0;var JX=AJ;do{if(((GQ.F&
0x200)===0x200))GQ=null;else if(((GQ.F&0x401)===0x401)){JX=[(A=GQ.GetExtent())[2
]-A[0],A[3]-A[1]];if((Ef===3)||(Ef===4))Md=Md-JX[0];if((Ef===1)||(Ef===2))LN=LN-
JX[1];if(!IH||((Md>=0)&&(LN>=0))){IH=GQ;GQ=GQ.Ad;if((Ef===3)||(Ef===4)){Md=Md-KI;
if(JX[1]>NO)NO=JX[1];}if((Ef===1)||(Ef===2)){LN=LN-KJ;if(JX[0]>Nk)Nk=JX[0];}}else
GQ=null;}else GQ=GQ.Ad;}while(!!GQ);if(!IH)IH=Cc;CE=GE;switch(KH){case 9:case 11:
CE=[].concat(CE.slice(0,3),CE[1]+NO);break;case 10:case 12:CE=B.t3(CE,CE[3]-NO);
break;case 5:case 7:CE=B.t1(CE,CE[0]+Nk);break;case 6:case 8:CE=[].concat(CE[2]-
Nk,CE.slice(1,4));break;default:;}}if(((K.F&0x400)===0x400)){if(!!K.AV&&(K.AV.IO
!==Cc))K.AV=null;if((!K.AV&&Gc)&&((K.D7!==LT)||!!Ef))K.KL(CE,Cc);}if(!!K.AV){if(
Ps&&!((K.F&0x400)===0x400))K.KX(HZ,0);if(Gc&&((K.F&0x400)===0x400)){var size=K.KX(
B.tz(CE,Gd),Ef);if(((K.F&0x1)===0x1)){var BL=AJ;switch(Ef){case 3:BL=[size[0]+KI
,BL[1]];break;case 4:BL=[-size[0]-KI,BL[1]];break;case 1:BL=[BL[0],size[1]+KJ];break;
case 2:BL=[BL[0],-size[1]-KJ];break;default:;}Gd=B.tx(Gd,BL);}}}if(((K.F&0x200)===
0x200)){if(Gc)B.lq(Cc.DO,Cc);Gc=((K.F&0x1000)===0x1000);Cc=(E.Lv.isPrototypeOf(K
)?K:null);if(Gc){K.F=K.F&~0x1000;GE=B.tz(Cc.H,Cc.AW);CE=GE;Gd=AJ;KH=Cc.Ms;Ef=KH;
KI=Cc.Space+Cc.Ub;KJ=Cc.Space+Cc.Uc;Gc=!((GE[0]>=GE[2])||(GE[1]>=GE[3]));IH=null;
switch(KH){case 9:case 10:Ef=3;break;case 11:case 12:Ef=4;break;case 5:case 6:Ef=
1;break;case 7:case 8:Ef=2;break;default:;}}if(Gc){this.Ai(Cc.H);}}if(K===IH){switch(
KH){case 9:case 11:Gd=[0,(Gd[1]+(CE[3]-CE[1]))+KJ];break;case 10:case 12:Gd=[0,(
Gd[1]-(CE[3]-CE[1]))-KJ];break;case 5:case 7:Gd=[(Gd[0]+(CE[2]-CE[0]))+KI,0];break;
case 6:case 8:Gd=[(Gd[0]-(CE[2]-CE[0]))-KI,0];break;default:;}IH=null;}K=K.Ad;}if(
Gc)B.lq(Cc.DO,Cc);this.F=this.F&~0x2000;this.Ha([HZ[2]-HZ[0],HZ[3]-HZ[1]]);},C_:
function(Af){var A;var Wf=((this.F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000
)){this.F=this.F&~0x4000;this.V3();}if(((this.F&0x8000)===0x8000)||Wf){this.F=this.
F&~0x8000;this.DR(this.F);}},D_:function(C){var A;if(!!C&&(C.G!==this))throw new
Error(Dh);if(!!C&&!((C.F&0x14)===0x14))C=null;if(!!C&&((C.F&0x10000)===0x10000))
C=null;if(C===this.Db)return;if(!!this.Db)this.Db.BB(0x0,0x60);this.Db=C;if(!!C){
if(((this.F&0x40)===0x40))C.BB(0x60,0x0);else C.BB(0x20,0x0);}},Tp:function(){return!
!this.AU;},Lr:function(C){var A;if(!!this.AU===C)return;if(C){this.AU=B._NewObject(
B.Graphics.Canvas,0);this.AU.Ls([(A=this.H)[2]-A[0],A[3]-A[1]]);this.AU.MG=[this
,this.We];}else{this.AU.MG=null;this.AU.Ls(AJ);this.AU=null;}if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ai(this.H);B.qw(this,0);},GZ:function(C){var A;if(C>255)C=255;
if(C<0)C=0;if(C===this.Dd)return;this.Dd=C;if(!!this.G&&((this.F&0x1)===0x1))this.
G.Ai(this.H);},Vk:function(Wh){this.GZ(Wh);},Kc:function(){var A;return((this.F&
0x1)===0x1);},Hx:function(C){if(C)this.BB(0x1,0x0);else this.BB(0x0,0x1);},Hu:function(
Rn){var A;return(((this.F&0x10000)===0x10000)&&!!this.G)&&(!Rn||this.G.Hu(true));
},E1:function(Di,EO,HO,Gy,Gz,Rl,HP,Jl,Gx,Gv,Gw){var A;if(!this.Cj){this.Q2(Di,EO
,HO,Gy,Gz,null,null,Gx,Gv,Gw);return;}var IM=this.Cj;var JF=IM.Ad;if(((Di.F&0x10000
)===0x10000)&&(this.Cj.Bz!==Di))throw new Error(CK);var GJ=B._NewObject(E.NZ,0);
var Hh=IM.Hh;var H8=null;var EE=null;if(!!JF){H8=JF.H8;EE=JF.EE;}if(!!JF&&!!IM.L8
)H8=IM.L8;if(!!JF&&!!HP)EE=HP;if(!!Rl)Hh=Rl;if(!EO)EO=B._GetAutoObject(B.uk.MV);
if(!HO)HO=EO;if(!Gz)Gz=Gy;if(!Gy)Gy=Gz;GJ.Hh=HO;GJ.EE=Gy;GJ.H8=Gz;GJ.L8=Jl;GJ.Bz=
Di;GJ.Ad=this.Cj.Ad;this.Cj.Ad=null;this.Cj=GJ;if(this.Db===Di)this.D_(null);IM.
Bz.BB(0x0,0x10040);if(((this.F&0x40)===0x40)&&((Di.F&0x14)===0x14))Di.BB(0x10040
,0x0);else Di.BB(0x10000,0x0);if(!!EE){this.FP(JF.Bz,EE.K2(),null,null,Gw);this.
FP(IM.Bz,Hh.J0(),null,null,true);this.FP(GJ.Bz,EO.Hp(),Gx,Gv,true);}else if(!!H8
){this.FP(JF.Bz,H8.K3(),null,null,Gw);this.FP(IM.Bz,Hh.J0(),null,null,true);this.
FP(GJ.Bz,EO.Hp(),Gx,Gv,true);}else if(!!Hh){this.FP(IM.Bz,Hh.J0(),null,null,Gw);
this.FP(GJ.Bz,EO.Hp(),Gx,Gv,true);}else this.FP(GJ.Bz,EO.Hp(),Gx,Gv,Gw);},Q2:function(
Di,EO,HO,Gy,Gz,HP,Jl,Gx,Gv,Gw){var A;if(!Di)return;if(!!this.Cj&&(this.Cj.Bz===Di
)){this.E1(Di,EO,HO,Gy,Gz,null,HP,Jl,Gx,Gv,Gw);return;}if(((Di.F&0x10000)===0x10000
))throw new Error(CK);var Ju=B._NewObject(E.NZ,0);if(!!this.Cj&&!this.Cj.EE){if(
!Jl)Jl=HP;if(!HP)HP=Jl;}var EE=null;if(!!this.Cj)EE=this.Cj.EE;if(!!this.Cj&&!!HP
)EE=HP;if(!EO)EO=B._GetAutoObject(B.uk.MV);if(!HO)HO=EO;if(!Gz)Gz=Gy;if(!Gy)Gy=Gz;
Ju.Hh=HO;Ju.EE=Gy;Ju.H8=Gz;Ju.L8=Jl;if(this.Db===Di)this.D_(null);if(!!this.Cj)this.
Cj.Bz.BB(0x0,0x40);if(((this.F&0x40)===0x40)&&((Di.F&0x14)===0x14))Di.BB(0x10040
,0x0);else Di.BB(0x10000,0x0);Ju.Bz=Di;Ju.Ad=this.Cj;this.Cj=Ju;if(!!EE){this.FP(
this.Cj.Ad.Bz,EE.K2(),null,null,Gw);this.FP(Di,EO.Hp(),Gx,Gv,true);}else this.FP(
Di,EO.Hp(),Gx,Gv,Gw);},Ka:function(Ey){var tmp=this;while(!!tmp){Ey=B.tw(Ey,tmp.
H.slice(0,2));tmp=tmp.G;}return Ey;},DispatchEvent:function(Ak){var A;var K=this.
Db;var Al=(E.J.isPrototypeOf(K)?K:null);var A7=null;var PH=!!this.FA&&(!!this.FA.
Ez||!!this.FA.Aw);if(!!K&&((((K.F&0x10000)===0x10000)||((K.F&0x40000)===0x40000)
)||((K.F&0x20000)===0x20000))){K=null;Al=null;}if(!!this.Cj&&!PH)A7=this.Cj.Bz.DispatchEvent(
Ak);if(!A7&&!!Al)A7=Al.DispatchEvent(Ak);else if(!A7&&!!K)A7=K.Cq(Ak);if(!A7)A7=
this.Cq(Ak);if(!A7)A7=this.PW(Ak);return A7;},BroadcastEventAtPosition:function(
Ak,Rm,aFilter){var A;var K=this.AB;var A7=null;while(!!K&&!A7){if((!aFilter||((A=
aFilter)&&((K.F&A)===A)))&&B.qu(K.GetExtent(),Rm)){var Al=(E.J.isPrototypeOf(K)?
K:null);if(!!Al)A7=Al.BroadcastEventAtPosition(Ak,B.tw(Rm,Al.H.slice(0,2)),aFilter
);else A7=K.Cq(Ak);}K=K.Ag;}if(!A7)A7=this.Cq(Ak);return A7;},BroadcastEvent:function(
Ak,aFilter){var A;var K=this.AB;var A7=null;while(!!K&&!A7){if(!aFilter||((A=aFilter
)&&((K.F&A)===A))){var Al=(E.J.isPrototypeOf(K)?K:null);if(!!Al)A7=Al.BroadcastEvent(
Ak,aFilter);else A7=K.Cq(Ak);}K=K.Ag;}if(!A7)A7=this.Cq(Ak);if(!A7)A7=this.PW(Ak
);return A7;},Ha:function(aSize){},DR:function(B$){},Bn:function(){this.F=this.F|
0x8000;B.lq([this,this.C_],this);},Ai:function(AC){var A;var Al=this;while(!!Al&&
!((AC[0]>=AC[2])||(AC[1]>=AC[3]))){var Jo=Al.AU;if(!Al.G&&(Al!==this)){Al.Ai(AC);
return;}if(!!Jo){var LU=false;var VL=Jo.EK;if(LU)Jo.EK=[0,0,(A=Al.H)[2]-A[0],A[3
]-A[1]];else Jo.EK=B.qR(Jo.EK,AC);if(!B.tm(VL,Jo.EK)){B.qw(Al,0);B.qw(Jo,0);}}if(
!((Al.F&0x1)===0x1))return;AC=B.il(B.tz(AC,Al.H.slice(0,2)),Al.H);Al=Al.G;}},CY:
function(aArg){this.Bn();},Oc:function(W,aFilter){var A;if(!W||(W.G!==this))return null;
var IK=W.Ad;var IQ=W.Ag;var L4=0x10000;if(((aFilter&0x10000)===0x10000))L4=0x0;while(
!!IK||!!IQ){if((!!IK&&(!aFilter||((A=aFilter)&&((IK.F&A)===A))))&&(!L4||!((A=L4)&&((
IK.F&A)===A))))return IK;if((!!IQ&&(!aFilter||((A=aFilter)&&((IQ.F&A)===A))))&&(
!L4||!((A=L4)&&((IQ.F&A)===A))))return IQ;if(!!IK)IK=IK.Ad;if(!!IQ)IQ=IQ.Ag;}return null;
},FP:function(Hb,Bc,Gx,Gv,Gw){var A;if(!Hb)return;if(!Bc)throw new Error(Ft);if((
!!Bc.J||!!Bc.G)||!!Bc.Fc)throw new Error(Iy);if(!!Hb.G&&(Hb.G!==this))throw new Error(
Jg);if(!this.FA)this.FA=B._NewObject(E.Q_,0);Bc.G=this;Bc.J=Hb;Bc.JJ=Gv;Bc.PM=Gx;
if(!!Hb.JR)Hb.JR.Fc.T3(Hb.JR);Hb.JR=Bc;Hb.F=Hb.F|0x20000;if((Gw&&!!this.FA.AB)&&
!this.FA.AB.Om())(B.uk.Mq.isPrototypeOf(A=this.FA.AB)?A:null).P6(Bc);else{var Fc=
B._NewObject(B.uk.Mq,0);Fc.P6(Bc);this.FA.T_(Fc,false);}},HF:function(W){var A;if(
!W)throw new Error(Kx);if(W.G!==this)throw new Error(Ky);if(!W.Ad)return;var Hf=
this.AB;var KQ=W.Jc;while(!!Hf&&(Hf.Jc>KQ))Hf=Hf.Ag;if(((Hf===W)||!Hf)||(Hf.Ad===
W))return;if(((W.F&0x401)===0x401)){if(!!W.Ag&&!!W.AV)W.Ag.F=W.Ag.F|0x800;W.F=W.
F|0x800;this.F=this.F|0x4000;B.lq([this,this.C_],this);}if(((W.F&0x200)===0x200)
){if(!!W.Ag)W.Ag.F=W.Ag.F|0x800;this.F=this.F|0x4000;B.lq([this,this.C_],this);}
if(!!W.Ag)W.Ag.Ad=W.Ad;else this.Aw=W.Ad;W.Ad.Ag=W.Ag;W.Ag=Hf;W.Ad=Hf.Ad;Hf.Ad=W;
if(!!W.Ad)W.Ad.Ag=W;else this.AB=W;if(((W.F&0x1)===0x1))this.Ai(W.GetExtent());}
,G3:function(W){var A;if(!W)throw new Error(LD);if(W.G!==this)throw new Error(Ky
);if((((W.F&0x401)===0x401)&&!!W.Ag)&&!!W.AV){W.Ag.F=W.Ag.F|0x800;this.F=this.F|
0x4000;B.lq([this,this.C_],this);}if(((W.F&0x200)===0x200)){if(!!W.Ag)W.Ag.F=W.Ag.
F|0x800;this.F=this.F|0x4000;B.lq([this,this.C_],this);}W.AV=null;if(this.Db===W
)this.D_(this.Oc(W,0x14));if(!!W.Ag)W.Ag.Ad=W.Ad;if(!!W.Ad)W.Ad.Ag=W.Ag;if(this.
Aw===W)this.Aw=W.Ad;if(this.AB===W)this.AB=W.Ag;W.G=null;W.Ad=null;W.Ag=null;if(((
W.F&0x1)===0x1))this.Ai(W.GetExtent());},Z:function(W,Jk){var A;if(!W)throw new Error(
M4);if(!!W.G)throw new Error(O2);var Ea=null;var KQ=W.Jc;if(((Jk<0)&&!!this.AB)&&(
this.AB.Jc>=KQ)){Ea=this.AB;Jk=Jk+1;}while((((Jk<0)&&!!Ea)&&!!Ea.Ag)&&(Ea.Ag.Jc>=
KQ)){Ea=Ea.Ag;Jk=Jk+1;}if((!Ea&&!!this.AB)&&(this.AB.Jc>KQ))Ea=this.AB;while((!!
Ea&&!!Ea.Ag)&&(Ea.Ag.Jc>KQ))Ea=Ea.Ag;if(!Ea){W.G=this;W.Ag=this.AB;if(!!this.AB)
this.AB.Ad=W;if(!this.Aw)this.Aw=W;this.AB=W;}else{W.G=this;W.Ag=Ea.Ag;W.Ad=Ea;Ea.
Ag=W;if(!!W.Ag)W.Ag.Ad=W;else this.Aw=W;}if(((W.F&0x1)===0x1))this.Ai(W.GetExtent(
));if(((!this.Db&&((W.F&0x4)===0x4))&&((W.F&0x10)===0x10))&&!((W.F&0x10000)===0x10000
))this.D_(W);if(((W.F&0x401)===0x401)){W.F=W.F|0x800;this.F=this.F|0x4000;B.lq([
this,this.C_],this);}if(((W.F&0x200)===0x200)){W.F=W.F|0x800;this.F=this.F|0x4000;
B.lq([this,this.C_],this);}},Tu:function(){return this.Dd;},_Init:function(aArg){
E.AQ._Init.call(this,aArg);this.__proto__=E.J;this.F=0x1F;this.CY(aArg);},_Mark:
function(D){var A;E.AQ._Mark.call(this,D);if((A=this.Aw)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.AB)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Nx)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.AU)&&(A._cycle!=D))A._Mark(A._cycle=D);
if((A=this.Cj)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.FA)&&(A._cycle!=D))
A._Mark(A._cycle=D);if((A=this.JR)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Db)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Group"};E.Root={Ei:null
,H1:null,GB:null,Bt:B.tA(10,null,null),KC:null,Fr:null,HS:null,Mg:0,Pl:0,Bx:0,CD:
B.tA(10,0,null),Nm:B.tA(10,B.qy,null),Fz:B.tA(10,0,null),Hg:B.tA(10,B.qx,null),KE:
B.tA(10,0,null),HR:B.tA(10,B.qx,null),Fy:B.tA(10,B.qx,null),EA:B.tA(10,B.qx,null
),GC:B.tA(10,B.qx,null),AN:0,Nq:0,Np:0,Nz:B.tA(3,B.qy,null),RJ:0,DH:B.tA(4,0,null
),Cz:B.tA(4,B.qy,null),Cl:0,If:8,Sn:250,IG:0,H0:0,Pt:true,Ny:false,I3:function(){
return this;},Da:function(AA,aClip,aOffset,Aj,aBlend){var fullScreenUpdate=false;
fullScreenUpdate=B.jI;if(!fullScreenUpdate)AA.Mr(aClip,B.tz(B.tz(aClip,aOffset),
this.H.slice(0,2)),0x00000000,0x00000000,0x00000000,0x00000000,false);E.J.Da.call(
this,AA,aClip,aOffset,Aj,aBlend);},BB:function(HQ,Jj){var A;E.J.BB.call(this,HQ,
Jj);if(!this.G&&(((HQ&0x1)===0x1)||((Jj&0x1)===0x1)))this.Ai([0,0,(A=this.H)[2]-
A[0],A[3]-A[1]]);if(!this.G&&(((HQ&0x2)===0x2)||((Jj&0x2)===0x2)))this.Ai([0,0,(
A=this.H)[2]-A[0],A[3]-A[1]]);},D_:function(C){if((C!==this.Fr)||!C)E.J.D_.call(
this,C);},Lr:function(C){var A;var VK=this.AU;E.J.Lr.call(this,C);if(((VK!==this.
AU)&&!this.G)&&((this.F&0x1)===0x1))this.Ai([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);
},GZ:function(C){var A;var NC=this.Dd;E.J.GZ.call(this,C);if(((NC!==this.Dd)&&!this.
G)&&((this.F&0x1)===0x1))this.Ai([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},Hu:function(
Rn){return true;},DispatchEvent:function(Ak){if((this.Pl>0)&&!!(E.KeyEvent.isPrototypeOf(
Ak)?Ak:null))return null;if(!!Ak){Ak.Li=!!this.Bx;if(!!this.Bx)Ak.Ah=this.Bx;}var
A7=null;if(!!this.Fr){A7=this.Fr.DispatchEvent(Ak);if(!!A7){this.Bx=0;return A7;
}}if(!!this.H1){A7=this.H1.Bz.DispatchEvent(Ak);if(!A7)A7=this.Cq(Ak);if(!A7)A7=
this.PW(Ak);this.Bx=0;return A7;}A7=E.J.DispatchEvent.call(this,Ak);this.Bx=0;return A7;
},BroadcastEvent:function(Ak,aFilter){if(!!Ak){Ak.Li=!!this.Bx;if(!!this.Bx)Ak.Ah=
this.Bx;}var A7=E.J.BroadcastEvent.call(this,Ak,aFilter);this.Bx=0;return A7;},Ai:
function(AC){var A;if(this.Mg>0)throw new Error(O3);if(!!this.AU&&!this.G){if(((
A=this.AU.EK)[0]>=A[2])||(A[1]>=A[3])){B.qw(this,0);B.qw(this.AU,0);}var LU=false;
if(LU)this.AU.EK=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];else this.AU.EK=B.qR(this.AU.
EK,AC);}var fullScreenUpdate=false;fullScreenUpdate=B.jI;if(fullScreenUpdate)AC=[
0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(!!this.G){E.J.Ai.call(this,AC);return;}AC=B.
il(B.tz(AC,this.H.slice(0,2)),this.H);if((AC[0]>=AC[2])||(AC[1]>=AC[3]))return;var
I;for(I=0;I<this.Cl;I=I+1)if(!(((A=B.il(this.Cz.Get(I),AC))[0]>=A[2])||(A[1]>=A[
3]))){this.Cz.Set(I,B.qR(this.Cz.Get(I),AC));this.DH.Set(I,B.s9(this.Cz.Get(I)));
return;}if(this.Cl<3){this.Cz.Set(this.Cl,AC);this.DH.Set(this.Cl,B.s9(AC));this.
Cl=this.Cl+1;return;}var BA;var Eh;var LX=0;var LY=0;var Rq=2147483647;this.Cz.Set(
this.Cl,AC);this.DH.Set(this.Cl,B.s9(AC));for(BA=0;BA<=this.Cl;BA=BA+1)for(Eh=BA+
1;Eh<=this.Cl;Eh=Eh+1){var NT=B.s9(B.qR(this.Cz.Get(BA),this.Cz.Get(Eh)));var RS=((
NT<<8)/(this.DH.Get(BA)+this.DH.Get(Eh)))|0;if(RS<Rq){Rq=RS;LX=BA;LY=Eh;}}this.Cz.
Set(LX,B.qR(this.Cz.Get(LX),this.Cz.Get(LY)));this.DH.Set(LX,B.s9(this.Cz.Get(LX
)));if(LY!==this.Cl){this.Cz.Set(LY,this.Cz.Get(this.Cl));this.DH.Set(LY,this.DH.
Get(this.Cl));}},Vx:function(){var C7=B._NewObject(E.J3,0);C7.Li=!!this.Bx;if(!!
this.Bx)C7.Ah=this.Bx;return C7;},Jr:function(){var C7=B._NewObject(E.J1,0);C7.Li=
!!this.Bx;if(!!this.Bx)C7.Ah=this.Bx;return C7;},KD:function(){var C7=B._NewObject(
E.J2,0);C7.Li=!!this.Bx;if(!!this.Bx)C7.Ah=this.Bx;return C7;},Vy:function(Af){var
I;var Jy=false;for(I=0;I<10;I=I+1)if(!!this.Bt.Get(I)){var S=this.EA.Get(I);var Al=
this.Bt.Get(I).G;while(!!Al&&(Al!==this)){S=B.tw(S,Al.H.slice(0,2));Al=Al.G;}if(
!Al&&(this.Bt.Get(I)!==this)){var tmp=this.Bt.Get(I);this.AN=I;this.Bt.Set(I,null
);tmp.Cq(this.Jr().InitializeUp(I,this.HR.Get(I),this.Hg.Get(I),this.Fz.Get(I),this.
CD.Get(I)+1,this.Fy.Get(I),false,this.EA.Get(I),this.GC.Get(I)));if(tmp===this.GB
)this.GB=null;this.BroadcastEvent(this.KD().InitializeUp(I,this.CD.Get(I)+1,false
,tmp,this.EA.Get(I)),0x18);}else{this.Fz.Set(I,(this.HS.Ah-this.KE.Get(I))|0);if(
this.Fz.Get(I)<10)this.Fz.Set(I,10);this.AN=I;this.Bt.Get(I).Cq(this.Jr().InitializeHold(
I,S,this.Hg.Get(I),this.Fz.Get(I),this.CD.Get(I)+1,this.Fy.Get(I),this.EA.Get(I)
,this.GC.Get(I)));Jy=true;}}if(!Jy)this.HS.BG(false);},GetFPS:function(){var ticksCount=
0;var RD=0;ticksCount=((new Date).getTime()-B.qt)|0;if(!!this.Nq&&(ticksCount>this.
Nq))RD=((this.Np*1000)/((ticksCount-this.Nq)|0))|0;this.Np=0;this.Nq=ticksCount;
return RD;},Update:function(){var A;if(!this.KC){this.KC=B._NewObject(B.Graphics.
Canvas,0);this.KC.Ls([(A=this.H)[2]-A[0],A[3]-A[1]]);}this.KC.Update();return this.
UpdateGE20(this.KC);},UpdateGE20:function(AA){if(!this.BeginUpdate())return Be;var
Ho=this.UpdateCanvas(AA,AJ);this.EndUpdate();return Ho;},EndUpdate:function(){if(
this.Cl>0){this.Np=this.Np+1;this.Cl=0;}},UpdateCanvas:function(AA,aOffset){var A;
var Ho=Be;var Vu=[].concat(aOffset,B.tx(AA.FrameSize,aOffset));var I;var BA=this.
Cl;this.Mg=this.Mg+1;for(I=0;(I<BA)&&(I<4);I=I+1){if(this.DH.Get(I)>0){this.Da(AA
,B.ty(this.Cz.Get(I),aOffset),[-aOffset[0],-aOffset[1]],255,true);Ho=B.qR(Ho,B.il(
Vu,this.Cz.Get(I)));}else BA=BA+1;}this.Mg=this.Mg-1;if(!((Ho[0]>=Ho[2])||(Ho[1]>=
Ho[3])))return B.ty(Ho,aOffset);else return Ho;},GetUpdateRegion:function(M7){var
I;var BA=this.Cl;if(M7<0)return Be;for(I=0;(I<BA)&&(I<4);I=I+1){if(!this.DH.Get(
I)){BA=BA+1;M7=M7+1;}else if(I===M7)return this.Cz.Get(I);}return Be;},BeginUpdate:
function(){var V2=true;var fullScreenUpdate=false;var I;if((!V2&&!fullScreenUpdate
)&&(this.Cl>0)){var Sb=B.tA(3,B.qy,null);var P3=this.Cl;for(I=0;I<P3;I=I+1)Sb.Set(
I,this.Cz.Get(I));for(I=0;I<this.RJ;I=I+1)this.Ai(this.Nz.Get(I));for(I=0;I<P3;I=
I+1)this.Nz.Set(I,Sb.Get(I));this.RJ=P3;}var BA;var Eh;for(BA=0;BA<(this.Cl-1);BA=
BA+1)if(this.DH.Get(BA)>0)for(Eh=BA+1;Eh<this.Cl;Eh=Eh+1)if(this.DH.Get(Eh)>0){var
NT=B.s9(B.qR(this.Cz.Get(BA),this.Cz.Get(Eh)));if(((NT-this.DH.Get(BA))-this.DH.
Get(Eh))<0){this.Cz.Set(BA,B.qR(this.Cz.Get(BA),this.Cz.Get(Eh)));this.DH.Set(BA
,NT);this.DH.Set(Eh,0);}}for(I=this.Cl-1;I>=0;I=I-1)if(!this.DH.Get(I))this.Cl=this.
Cl-1;return this.Cl;},DoesNeedUpdate:function(){if(this.Cl>0)return true;return false;
},Initialize:function(aSize){this.P([].concat(AJ,aSize));if(this.Pt)this.F=this.
F|0x60;else this.F=this.F|0x20;this.Ai(this.H);return this;},SetRootFocus:function(
Ph){if(Ph===this.Pt)return false;this.Pt=Ph;if(!Ph){if(!!this.Fr)this.Fr.BB(0x0,
0x40);if(!!this.H1)this.H1.Bz.BB(0x0,0x40);else this.BB(0x0,0x40);}else{if(!!this.
H1)this.H1.Bz.BB(0x40,0x0);else this.BB(0x40,0x0);if(!!this.Fr)this.Fr.BB(0x40,0x0
);}return true;},SetUserInputTimestamp:function(Vq){this.Bx=Vq;},DriveKeyboardHitting:
function(B_,Ji,Ex){var A;var PX=!!this.Ei;if(!!this.Ei&&((!Ex||(this.IG!==B_))||(
this.H0!==Ji))){var C7=null;var K=(E.AE.isPrototypeOf(A=this.Ei)?A:null);var Ge=(
E.J9.isPrototypeOf(A=this.Ei)?A:null);if(!!this.IG)C7=B._NewObject(E.KeyEvent,0).
Initialize(this.IG,false);if(this.H0!==0x00)C7=B._NewObject(E.KeyEvent,0).Initialize2(
this.H0,false);if(!!Ge)Ge.Cq(C7);else if(!!K)K.Cq(C7);this.IG=0;this.H0=0x00;this.
Ei=null;}if(!!this.Ei){var C7=null;var K=(E.AE.isPrototypeOf(A=this.Ei)?A:null);
var Ge=(E.J9.isPrototypeOf(A=this.Ei)?A:null);if(!!B_)C7=B._NewObject(E.KeyEvent
,0).Initialize(B_,true);if(this.H0!==0x00)C7=B._NewObject(E.KeyEvent,0).Initialize2(
Ji,true);if(!!Ge)Ge.Cq(C7);else if(!!K)K.Cq(C7);}if(this.Ny&&((!Ex||(this.IG!==B_
))||(this.H0!==Ji))){this.IG=0;this.H0=0x00;this.Ny=false;}if((!this.Ei&&Ex)&&(this.
Pl>0)){this.IG=B_;this.H0=Ji;this.Ny=true;}if((!this.Ei&&Ex)&&!this.Ny){if(!!B_)
this.Ei=this.DispatchEvent(B._NewObject(E.KeyEvent,0).Initialize(B_,true));if(Ji
!==0x00)this.Ei=this.DispatchEvent(B._NewObject(E.KeyEvent,0).Initialize2(Ji,true
));if(!(E.J9.isPrototypeOf(A=this.Ei)?A:null)&&!(E.AE.isPrototypeOf(A=this.Ei)?A:
null))this.Ei=null;this.IG=B_;this.H0=Ji;PX=PX||!!this.Ei;}this.Bx=0;return PX;}
,DriveCursorMovement:function(Bq){return this.DriveMultiTouchMovement(this.AN,Bq
);},DriveMultiTouchMovement:function(T,Bq){if((T<0)||(T>9)){this.Bx=0;return false;
}var offset=B.tw(Bq,this.EA.Get(T));this.EA.Set(T,Bq);if(!this.Bt.Get(T)||B.tl(offset
,AJ)){this.Bx=0;return false;}var S=Bq;var Al=this.Bt.Get(T).G;while(!!Al&&(Al!==
this)){S=B.tw(S,Al.H.slice(0,2));Al=Al.G;}if(!Al&&(this.Bt.Get(T)!==this)){var tmp=
this.Bt.Get(T);this.AN=T;this.Bt.Set(T,null);tmp.Cq(this.Jr().InitializeUp(T,this.
HR.Get(T),this.Hg.Get(T),this.Fz.Get(T),this.CD.Get(T)+1,this.Fy.Get(T),false,this.
EA.Get(T),this.GC.Get(T)));if(tmp===this.GB)this.GB=null;this.BroadcastEvent(this.
KD().InitializeUp(T,this.CD.Get(T)+1,false,tmp,Bq),0x18);}else{this.HR.Set(T,S);
this.AN=T;this.Bt.Get(T).Cq(this.Vx().Initialize(T,S,this.Hg.Get(T),offset,this.
Fz.Get(T),this.CD.Get(T)+1,this.Fy.Get(T),Bq,this.GC.Get(T)));}this.Bx=0;return true;
},DriveCursorHitting:function(Ex,T,Bq){return this.DriveMultiTouchHitting(Ex,T,Bq
);},DriveMultiTouchHitting:function(Ex,T,Bq){var A;if((T<0)||(T>9)){this.Bx=0;return false;
}var ticksCount=this.Bx;var Jv=[].concat([-this.If,-this.If],[this.If+1,this.If+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.qt)|0;}var V9=this.Bx;this.
DriveMultiTouchMovement(T,Bq);Bq=this.EA.Get(T);this.Bx=V9;if(Ex)this.GC.Set(T,Bq
);if((Ex&&!this.Bt.Get(T))&&!this.Pl){var Eg=null;var S=Bq;if(B.qu(this.Nm.Get(T
),Bq)&&((ticksCount-this.KE.Get(T))<=(((A=this.Sn)<0)?A+0x100000000:A)))this.CD.
Set(T,this.CD.Get(T)+1);else this.CD.Set(T,0);this.Nm.Set(T,B.tz(Jv,Bq));this.KE.
Set(T,ticksCount);if((!!this.Fr&&!!this.Fr.G)&&((this.Fr.F&0x18)===0x18)){var R=
B.tz(Jv,this.Fr.G.Ka(Bq));Eg=this.Fr.Gj(R,T,this.CD.Get(T)+1,null,0x0);}if(!Eg){
if(!!this.GB&&!!this.GB.G){if(((this.GB.F&0x8)===0x8)&&((this.GB.F&0x10)===0x10)
){var R=B.tz(Jv,this.GB.G.Ka(Bq));Eg=this.GB.Gj(R,T,this.CD.Get(T)+1,null,0x0);}
}else if(!!this.H1)Eg=this.Gj(B.tz(Jv,Bq),T,this.CD.Get(T)+1,this.H1.Bz,0x0);else
Eg=this.Gj(B.tz(Jv,Bq),T,this.CD.Get(T)+1,null,0x0);}if(!!Eg){this.BroadcastEvent(
this.KD().InitializeDown(T,this.CD.Get(T)+1,false,Eg.AE,Bq),0x18);this.Bt.Set(T,
Eg.AE);this.Fy.Set(T,Eg.As);}else{this.Bt.Set(T,null);this.Fy.Set(T,AJ);this.Bx=
0;return false;}var Al=Eg.AE.G;while(!!Al&&(Al!==this)){S=B.tw(S,Al.H.slice(0,2)
);Al=Al.G;}this.Hg.Set(T,S);this.HR.Set(T,S);this.Fz.Set(T,0);this.HS.BG(true);this.
AN=T;this.Bt.Get(T).Cq(this.Jr().InitializeDown(T,S,this.CD.Get(T)+1,this.Fy.Get(
T),false,Bq));this.Bx=0;return true;}if(!Ex&&!!this.Bt.Get(T)){var S=Bq;var Al=this.
Bt.Get(T).G;while(!!Al&&(Al!==this)){S=B.tw(S,Al.H.slice(0,2));Al=Al.G;}if(!Al)S=
this.HR.Get(T);this.AN=T;var tmp=this.Bt.Get(T);this.Bt.Set(T,null);tmp.Cq(this.
Jr().InitializeUp(T,S,this.Hg.Get(T),this.Fz.Get(T),this.CD.Get(T)+1,this.Fy.Get(
T),false,Bq,this.GC.Get(T)));this.BroadcastEvent(this.KD().InitializeUp(T,this.CD.
Get(T)+1,false,tmp,Bq),0x18);this.Bx=0;return true;}this.Bx=0;return false;},Kn:
function(Hc,Rk,Fv){var Jv=[].concat([-this.If,-this.If],[this.If+1,this.If+1]);if(
Hc===this)Hc=null;if(!this.Bt.Get(this.AN))return;var Eg;Eg=this.Gj(B.tz(Jv,this.
EA.Get(this.AN)),this.AN,1,Hc,Fv);if(!!Eg&&(this.Bt.Get(this.AN)!==Eg.AE))this.Qh(
Eg.AE,Eg.As);if(!Eg&&(this.Bt.Get(this.AN)!==Rk))this.Qh(Rk,AJ);},Qh:function(Hc
,Fu){if(!this.Bt.Get(this.AN)||(this.Bt.Get(this.AN)===Hc))return;var tmp=this.Bt.
Get(this.AN);this.Bt.Set(this.AN,null);tmp.Cq(this.Jr().InitializeUp(this.AN,this.
HR.Get(this.AN),this.Hg.Get(this.AN),this.Fz.Get(this.AN),this.CD.Get(this.AN)+1
,this.Fy.Get(this.AN),true,this.EA.Get(this.AN),this.GC.Get(this.AN)));this.BroadcastEvent(
this.KD().InitializeUp(this.AN,this.CD.Get(this.AN)+1,true,tmp,this.EA.Get(this.
AN)),0x18);var S=this.EA.Get(this.AN);var Al=null;if(!!Hc)Al=Hc.G;while(!!Al&&(Al
!==this)){S=B.tw(S,Al.H.slice(0,2));Al=Al.G;}if(!Al&&(Hc!==this)){this.Bt.Set(this.
AN,null);return;}this.BroadcastEvent(this.KD().InitializeDown(this.AN,this.CD.Get(
this.AN)+1,true,Hc,this.EA.Get(this.AN)),0x18);var ticksCount=0;ticksCount=((new
Date).getTime()-B.qt)|0;this.Bt.Set(this.AN,Hc);this.Fy.Set(this.AN,Fu);this.Hg.
Set(this.AN,S);this.HR.Set(this.AN,S);this.CD.Set(this.AN,0);this.Fz.Set(this.AN
,0);this.KE.Set(this.AN,ticksCount);this.GC.Set(this.AN,this.EA.Get(this.AN));this.
Bt.Get(this.AN).Cq(this.Jr().InitializeDown(this.AN,S,this.CD.Get(this.AN)+1,this.
Fy.Get(this.AN),true,this.GC.Get(this.AN)));},_Init:function(aArg){E.J._Init.call(
this,aArg);E.Timer._Init.call(this.HS={M:this},0);(this.Bt=[]).__proto__=E.Root.
Bt;(this.CD=[]).__proto__=E.Root.CD;(this.Nm=[]).__proto__=E.Root.Nm;(this.Fz=[]
).__proto__=E.Root.Fz;(this.Hg=[]).__proto__=E.Root.Hg;(this.KE=[]).__proto__=E.
Root.KE;(this.HR=[]).__proto__=E.Root.HR;(this.Fy=[]).__proto__=E.Root.Fy;(this.
EA=[]).__proto__=E.Root.EA;(this.GC=[]).__proto__=E.Root.GC;(this.Nz=[]).__proto__=
E.Root.Nz;(this.DH=[]).__proto__=E.Root.DH;(this.Cz=[]).__proto__=E.Root.Cz;this.
__proto__=E.Root;this.F=0x7F;this.HS.Lu(50);this.HS.Is=[this,this.Vy];},_Done:function(
){this.__proto__=E.J;this.HS._Done();E.J._Done.call(this);},_ReInit:function(){E.
J._ReInit.call(this);this.HS._ReInit();},_Mark:function(D){var A;E.J._Mark.call(
this,D);if((A=this.Ei)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.H1)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.GB)&&(A._cycle!=D))A._Mark(A._cycle=D);B.ts(
this.Bt,D);if((A=this.KC)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fr)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.HS)._cycle!=D)A._Mark(A._cycle=D);},_className:
"Core::Root"};E.Event={Ah:0,Li:false,Lf:function(){var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;return ticksCount;},CY:function(aArg){this.Ah=this.Lf(
);},_Init:function(aArg){this.__proto__=E.Event;this.CY(aArg);B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:
"Core::Event"};E.KeyEvent={Ce:0,A9:0,Down:false,Initialize2:function(B_,Ex){this.
Ce=0;this.A9=B_;this.Down=Ex;if((B_>=0x30)&&(B_<=0x39))this.Ce=(10+B_)-48;if((B_>=
0x41)&&(B_<=0x5A))this.Ce=(105+B_)-65;if((B_>=0x61)&&(B_<=0x7A))this.Ce=(105+B_)-
97;if(B_===0x20)this.Ce=131;if(!this.Ce)switch(B_){case 0x2B:this.Ce=132;break;case
0x2D:this.Ce=133;break;case 0x2A:this.Ce=134;break;case 0x2F:this.Ce=135;break;case
0x3D:this.Ce=136;break;case 0x2E:this.Ce=137;break;case 0x2C:this.Ce=138;break;case
0x3A:this.Ce=139;break;case 0x3B:this.Ce=140;break;default:;}return this;},Initialize:
function(B_,Ex){this.Ce=B_;this.Down=Ex;this.A9=0x00;var Pn=B_-10;var Pm=B_-105;
if((Pn>=0)&&(Pn<=9))this.A9=(48+Pn)&0xFFFF;if((Pm>=0)&&(Pm<=25))this.A9=(65+Pm)&
0xFFFF;if(B_===131)this.A9=0x20;if(this.A9===0x00)switch(B_){case 132:this.A9=0x2B;
break;case 133:this.A9=0x2D;break;case 134:this.A9=0x2A;break;case 135:this.A9=0x2F;
break;case 136:this.A9=0x3D;break;case 137:this.A9=0x2E;break;case 138:this.A9=0x2C;
break;case 139:this.A9=0x3A;break;case 140:this.A9=0x3B;break;default:;}return this;
},S5:function(Ri){switch(Ri){case 141:return((this.A9>=0x41)&&(this.A9<=0x5A))||((
this.A9>=0x61)&&(this.A9<=0x7A));case 142:return(((this.A9>=0x41)&&(this.A9<=0x5A
))||((this.A9>=0x61)&&(this.A9<=0x7A)))||((this.A9>=0x30)&&(this.A9<=0x39));case
143:return(this.A9>=0x30)&&(this.A9<=0x39);case 144:return(((this.A9>=0x41)&&(this.
A9<=0x46))||((this.A9>=0x61)&&(this.A9<=0x66)))||((this.A9>=0x30)&&(this.A9<=0x39
));case 145:return this.A9!==0x00;case 146:return(this.A9===0x00)&&!!this.Ce;case
147:return(((this.Ce===6)||(this.Ce===7))||(this.Ce===4))||(this.Ce===5);case 148:
return(this.A9!==0x00)||!!this.Ce;default:;}return Ri===this.Ce;},_Init:function(
aArg){E.Event._Init.call(this,aArg);this.__proto__=E.KeyEvent;},_className:"Core::KeyEvent"
};E.J2={Jd:null,CA:B.qx,Fq:0,Bv:0,Down:false,DI:false,InitializeUp:function(T,DS
,Jh,Pj,Ga){this.Down=false;this.Bv=T;this.Fq=DS;this.CA=Ga;this.Jd=Pj;this.DI=Jh;
return this;},InitializeDown:function(T,DS,Jh,Pj,Ga){this.Down=true;this.Bv=T;this.
Fq=DS;this.CA=Ga;this.Jd=Pj;this.DI=Jh;return this;},_Init:function(aArg){E.Event.
_Init.call(this,aArg);this.__proto__=E.J2;},_Mark:function(D){var A;E.Event._Mark.
call(this,D);if((A=this.Jd)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"
};E.J1={D4:B.qx,CA:B.qx,Fq:0,BS:0,CX:B.qx,DJ:B.qx,Bv:0,Down:false,DI:false,InitializeHold:
function(T,IA,LG,LH,DS,Fu,Ga,LF){this.Down=true;this.Bv=T;this.DJ=B.tx(IA,Fu);this.
CX=B.tx(LG,Fu);this.BS=LH;this.Fq=DS;this.CA=Ga;this.D4=LF;return this;},InitializeUp:
function(T,IA,LG,LH,DS,Fu,Jh,Ga,LF){this.Down=false;this.Bv=T;this.DJ=B.tx(IA,Fu
);this.CX=B.tx(LG,Fu);this.BS=LH;this.Fq=DS;this.DI=Jh;this.CA=Ga;this.D4=LF;return this;
},InitializeDown:function(T,IA,DS,Fu,Jh,Ga){this.Down=true;this.Bv=T;this.DJ=B.tx(
IA,Fu);this.CX=B.tx(IA,Fu);this.BS=0;this.Fq=DS;this.DI=Jh;this.CA=Ga;this.D4=Ga;
return this;},_Init:function(aArg){E.Event._Init.call(this,aArg);this.__proto__=
E.J1;},_className:"Core::CursorEvent"};E.J3={D4:B.qx,CA:B.qx,Fq:0,BS:0,As:B.qx,CX:
B.qx,DJ:B.qx,Bv:0,Initialize:function(T,IA,LG,aOffset,LH,Vo,Fu,Ga,LF){this.Bv=T;
this.DJ=B.tx(IA,Fu);this.CX=B.tx(LG,Fu);this.As=aOffset;this.BS=LH;this.Fq=Vo;this.
CA=Ga;this.D4=LF;return this;},_Init:function(aArg){E.Event._Init.call(this,aArg
);this.__proto__=E.J3;},_className:"Core::DragEvent"};E.Lv={DO:null,AW:B.qx,Uc:0
,Ub:0,Space:0,Ms:0,Da:function(AA,aClip,aOffset,Aj,aBlend){},P:function(C){var A;
if(B.tm(C,this.H))return;var IN=[(A=this.H)[2]-A[0],A[3]-A[1]];var NB=[C[2]-C[0]
,C[3]-C[1]];var JU=!B.tl(IN,NB);var offset=B.tw(C.slice(0,2),this.H.slice(0,2));
if(!B.tl(offset,AJ)&&!JU){var K=this.Ad;while(!!K&&!((K.F&0x200)===0x200)){if(((
K.F&0x400)===0x400)){var tmp=((K.F&0x100)===0x100);K.Ln(offset,tmp);}K=K.Ad;}B.lq(
this.DO,this);}if((JU&&(IN[0]>0))&&(IN[1]>0)){var U=B.tz(this.H,this.AW);var K=this.
Ad;var LT=0x14;while(!!K&&!((K.F&0x200)===0x200)){if(((K.F&0x400)===0x400)){if(!
!K.AV&&(K.AV.IO!==this))K.AV=null;if(!K.AV&&((K.D7!==LT)||!!this.Ms))K.KL(U,this
);}K=K.Ad;}B.lq(this.DO,this);}E.AQ.P.call(this,C);if(!!this.G&&JU){this.F=this.
F|0x1000;if(!((this.G.F&0x2000)===0x2000)){this.G.F=this.G.F|0x4000;B.lq([A=this.
G,A.C_],this);}}},_Init:function(aArg){E.AQ._Init.call(this,aArg);this.__proto__=
E.Lv;this.F=0x203;},_Mark:function(D){var A;E.AQ._Mark.call(this,D);if((A=this.DO
)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Outline"};E.HK={
JE:B.tA(8,null,null),KP:null,AE:null,Fo:null,DO:null,FT:null,Px:-1,Pw:0,EI:-1,Dm:
0,EC:0,EY:-1,T1:0,AW:0,Ja:-1,Iu:0,AY:24,A5:0,Lj:null,Dn:false,PG:false,Da:function(
AA,aClip,aOffset,Aj,aBlend){E.J.Da.call(this,AA,aClip,aOffset,Aj,aBlend);},DispatchEvent:
function(Ak){var A;var result=null;if(((this.Ja>=0)&&(this.Ja<this.A5))&&!this.Db
){this.EY=this.Ja;this.AE=(E.AE.isPrototypeOf(A=B._NewObject(this.Lj,0))?A:null);
if(!!this.AB)this.AB.Ad=this.AE;else this.Aw=this.AE;this.AE.Ag=this.AB;this.AB=
this.AE;this.AE.G=this;(A=this.FT)?A[1].call(A[0],this):null;var Al=(E.J.isPrototypeOf(
A=this.AE)?A:null);if(!!Al)result=Al.DispatchEvent(Ak);else result=this.AE.Cq(Ak
);if(!!this.AE.Ag)this.AE.Ag.Ad=null;else this.Aw=null;this.AB=this.AE.Ag;this.AE.
Ag=null;this.AE.G=null;this.AE=null;}if(!result)result=E.J.DispatchEvent.call(this
,Ak);return result;},DR:function(B$){var A;if(!this.Lj){B.lq(this.DO,this);return;
}this.PG=true;var L9=0;if(!this.Dn)L9=this.Iu;var DE=((-L9-this.AW)/this.AY)|0;var
Eo=((((((A=this.H)[3]-A[1])-L9)-this.AW)-1)/this.AY)|0;var Nl=(((((A=this.H)[3]-
A[1])+this.AY)-1)/this.AY)|0;if(Nl<1)Nl=1;var Nf=(Nl/2)|0;var Ng=(Nl/3)|0;if(!Nf
)Nf=1;if(!Ng)Ng=1;if(DE<this.Dm){DE=DE-Nf;Eo=Eo+Ng;}else if(Eo>this.EI){DE=DE-Ng;
Eo=Eo+Nf;}else{DE=this.Dm;Eo=this.EI;}if(!this.Dn){if(DE>=this.A5){DE=0;Eo=-1;}else
if(Eo<0){DE=0;Eo=-1;}if(Eo>=this.A5)Eo=this.A5-1;if(DE<0)DE=0;}else if(this.A5<=
0){DE=0;Eo=-1;}var NM=this.Dm;var NN=this.EI;var PE=0;var PF=-1;if(DE>NM)NM=DE;if(
Eo<NN)NN=Eo;if(NM<=NN){while((this.EI<Eo)&&(this.Dm<NM)){this.Ma();this.Rx();}while((
this.Dm>DE)&&(this.EI>NN)){this.RW();this.Rw();}}else{this.EI=(this.EI-this.Dm)+
DE;this.Dm=DE;PE=this.Dm;PF=this.EI;}while(this.Dm<DE)this.Ma();while(this.EI>Eo
)this.RW();while(this.Dm>DE)this.Rw();while(this.EI<Eo)this.Rx();var Y=this.Aw;var
DW=this.Dm;var S=[0,(L9+this.AW)+(DW*this.AY)];var At=(A=this.H)[3]-A[1];var RE=
this.AY;var LS=null;while(!!Y){var BL=B.tw(S,Y.GetExtent().slice(0,2));var PA=S[
1];var PB=S[1]+RE;var C8=DW;if(this.Dn){if(C8<0)C8=this.A5-(-C8%this.A5);if(C8>0
)C8=C8%this.A5;}if(!B.tl(BL,AJ))Y.Ln(BL,true);if(((((C8>=this.Pw)&&(C8<=this.Px)
)||((DW>=PE)&&(DW<=PF)))&&(PA<At))&&(PB>0)){this.AE=Y;this.EY=C8;(A=this.FT)?A[1
].call(A[0],this):null;}Y=Y.Ad;DW=DW+1;S=[S[0],S[1]+this.AY];}DW=this.Dm;Y=this.
Aw;S=[0,(L9+this.AW)+(DW*this.AY)];while(!!Y){var PA=S[1];var PB=S[1]+RE;var C8=
DW;if(this.Dn){if(C8<0)C8=this.A5-(-C8%this.A5);if(C8>0)C8=C8%this.A5;}if((((C8>=
this.Pw)&&(C8<=this.Px))||((DW>=PE)&&(DW<=PF)))&&!((PA<At)&&(PB>0))){this.AE=Y;this.
EY=C8;(A=this.FT)?A[1].call(A[0],this):null;}if(((C8===this.Ja)&&this.Dn)&&!!LS){
var Rp=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(B.s9(B.il(Y.GetExtent(),Rp))>B.s9(B.
il(LS.GetExtent(),Rp)))LS=Y;}else if(C8===this.Ja)LS=Y;Y=Y.Ad;DW=DW+1;S=[S[0],S[
1]+this.AY];}this.Pw=0;this.Px=-1;this.AE=null;this.EY=-1;this.D_(LS);this.PG=false;
B.lq(this.DO,this);},Oc:function(W,aFilter){return null;},HF:function(W){throw new
Error(O4);},G3:function(W){throw new Error(O5);},Z:function(W,Jk){throw new Error(
O6);},RW:function(){var Y=this.AB;if(!Y)return null;if(Y===this.Db)this.D_(null);
this.EI=this.EI-1;if(!!Y.Ag)Y.Ag.Ad=null;else this.Aw=null;this.AB=Y.Ag;Y.Ag=null;
Y.G=null;Y.AV=null;if(this.EC<8){this.JE.Set(this.EC,Y);this.EC=this.EC+1;}return Y;
},Rx:function(){var A;var Y=null;if(!this.EC){Y=(E.AE.isPrototypeOf(A=B._NewObject(
this.Lj,0))?A:null);Y.Dc(0x1D);}else{this.EC=this.EC-1;Y=this.JE.Get(this.EC);}this.
EI=this.EI+1;var IY=this.EI;if(this.Dn){if(IY<0)IY=this.A5-(-IY%this.A5);if(IY>0
)IY=IY%this.A5;}this.EY=IY;this.AE=Y;(A=this.FT)?A[1].call(A[0],this):null;this.
AE=null;this.EY=-1;if(!!this.AB)this.AB.Ad=Y;Y.G=this;Y.Ag=this.AB;this.AB=Y;if(
!this.Aw)this.Aw=Y;if(IY===this.Ja)this.D_(Y);return Y;},Ma:function(){var Y=this.
Aw;if(!Y)return null;if(Y===this.Db)this.D_(null);this.Dm=this.Dm+1;if(!!Y.Ad)Y.
Ad.Ag=null;else this.AB=null;this.Aw=Y.Ad;Y.Ad=null;Y.G=null;Y.AV=null;if(this.EC<
8){this.JE.Set(this.EC,Y);this.EC=this.EC+1;}return Y;},Rw:function(){var A;var Y=
null;if(!this.EC){Y=(E.AE.isPrototypeOf(A=B._NewObject(this.Lj,0))?A:null);Y.Dc(
0x1D);}else{this.EC=this.EC-1;Y=this.JE.Get(this.EC);}this.Dm=this.Dm-1;var D1=this.
Dm;if(this.Dn){if(D1<0)D1=this.A5-(-D1%this.A5);if(D1>0)D1=D1%this.A5;}this.EY=D1;
this.AE=Y;(A=this.FT)?A[1].call(A[0],this):null;this.AE=null;this.EY=-1;if(!!this.
Aw)this.Aw.Ag=Y;Y.G=this;Y.Ad=this.Aw;this.Aw=Y;if(!this.AB)this.AB=Y;if(D1===this.
Ja)this.D_(Y);return Y;},VZ:function(Af){this.BY(this.Fo.As[1]);},V0:function(Af
){var A;if(!!this.KP){this.KP.BG(false);this.KP.Av=null;this.KP.PV=null;this.KP=
null;}if(!this.Dn){var R=this.SO(0,this.A5-1);var NF=this.H.slice(0,2);R=B.t3(R,
R[1]-this.Iu);R=[].concat(R.slice(0,3),R[3]+this.T1);if(R[0]>NF[0])R=[].concat(NF[
0],R.slice(1,4));if(R[1]>NF[1])R=B.t3(R,NF[1]);var offset=R[1]-this.H[1];var PK=((
A=this.H)[3]-A[1])-(R[3]-R[1]);if(PK>0)PK=0;this.Fo.As=[0,this.AW];this.Fo.Bb=[0
,(this.AW+PK)-offset];this.Fo.Ba=[0,this.AW-offset];}else{var offset=32000-(32000
%this.AY);this.Fo.As=[0,this.AW];this.Fo.Bb=[0,this.AW-offset];this.Fo.Ba=[0,this.
AW+offset];}},Kf:function(C){var A;if(this.Dn===C)return;this.Dn=C;while(!!this.
Ma());this.Bn();this.Ai([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},Kj:function(C){if(
C===this.Fo)return;if(!!C&&!!C.KN){B.uf("%s%$%*%s",O7,C,O8);throw new Error(O9);
}if(!!this.Fo){this.Fo.NI=null;this.Fo.KN=null;}this.Fo=C;if(!!C){C.NI=[this,this.
V0];C.KN=[this,this.VZ];}},BY:function(C){var A;if(this.Dn&&(this.A5>0)){var CM=
this.A5*this.AY;if(C<0)C=CM-(-C%CM);if(C>0)C=C%CM;if(C>0)C=C-CM;}if(C===this.AW)
return;this.AW=C;this.Bn();this.Ai([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},FX:function(
C){var A;if(C<1)C=1;if(C===this.AY)return;this.AY=C;while(!!this.Ma());this.Bn();
this.Ai([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},Hw:function(C){var A;if(C<0)C=0;if(
C===this.A5)return;var R=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(!this.Dn){if(C>this.
A5){R=B.t3(R,(this.Iu+this.AW)+(this.A5*this.AY));R=[].concat(R.slice(0,3),(this.
Iu+this.AW)+(C*this.AY));}else{R=B.t3(R,(this.Iu+this.AW)+(C*this.AY));R=[].concat(
R.slice(0,3),(this.Iu+this.AW)+(this.A5*this.AY));}}else while(!!this.Ma());this.
A5=C;this.Bn();this.Ai(R);},J8:function(IB){if(this.PG)return null;if((IB<0)||(IB>=
this.A5))return null;if(!this.Dn){if((IB<this.Dm)||(IB>this.EI))return null;var K=
this.Aw;while(IB>this.Dm){K=K.Ad;IB=IB-1;}return K;}else{var D1=this.Dm;if(D1<0)
D1=this.A5-(-D1%this.A5);if(D1>0)D1=D1%this.A5;var K=this.Aw;while(!!K){if((D1%this.
A5)===IB)return K;K=K.Ad;D1=D1+1;}return null;}},J7:function(Bq){if(((this.A5<=0
)||(Bq[0]<this.H[0]))||(Bq[0]>=this.H[2]))return-1;Bq=B.tw(Bq,this.H.slice(0,2));
if(!this.Dn){var Y=(Bq[1]-this.AW)-this.Iu;if(Y>0)Y=(Y/this.AY)|0;if((Y<0)||(Y>=
this.A5))return-1;return Y;}var Y=Bq[1]-this.AW;if(Y>0)Y=(Y/this.AY)|0;if(Y<0)Y=((
Y/this.AY)|0)-1;if(Y<0)Y=this.A5-(-Y%this.A5);if(Y>0)Y=Y%this.A5;return Y;},SO:function(
LE,M6){if(LE<0)LE=0;if(M6>=this.A5)M6=this.A5-1;if(LE>M6)return Be;var R=this.H;
var BL=this.AW;if(this.Dn){var CM=this.A5*this.AY;if(BL<0)BL=CM-(-BL%CM);if(BL>0
)BL=BL%CM;if(BL>0)BL=BL-CM;}else BL=BL+this.Iu;R=B.t3(R,(R[1]+BL)+(LE*this.AY));
R=[].concat(R.slice(0,3),R[1]+(((M6-LE)+1)*this.AY));return R;},_Init:function(aArg
){E.J._Init.call(this,aArg);(this.JE=[]).__proto__=E.HK.JE;this.__proto__=E.HK;this.
P(O_);this.Lj=B.uj.Text;},_Mark:function(D){var A;E.J._Mark.call(this,D);B.ts(this.
JE,D);if((A=this.KP)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.AE)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Fo)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.DO)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.FT)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);},_className:"Core::VerticalList"};E.G4={QI:null,FS:null
,FR:null,Lo:null,D9:null,CH:null,Fb:0,V:0,Bv:0,Ah:0,Fq:0,BS:0,As:B.qx,CX:B.qx,DJ:
B.qx,LA:1000,EN:8,Ch:0,Il:1,Gn:-1,Qz:1,Th:1,N2:false,Down:false,Fj:false,DI:false
,Da:function(AA,aClip,aOffset,Aj,aBlend){},Cq:function(Ak){var A;var Ac=(E.J1.isPrototypeOf(
Ak)?Ak:null);var Aq=(E.J3.isPrototypeOf(Ak)?Ak:null);var Nt=this.Fj;var DX;var Cx;
var IX;var Br;var DV;if(!Ac&&!Aq)return null;DX=(!!Ac&&Ac.Down)&&!Ac.BS;Cx=(!!Ac&&
Ac.Down)&&(Ac.BS>0);IX=(!!Ac&&Ac.Down)&&(Ac.BS>50);Br=!!Ac&&!Ac.Down;DV=!!Aq;if(
!this.N2){if((((this.Ch&0x20)===0x20)&&(this.V>0))&&(this.V<33554432)){var A_=(E.
J2.isPrototypeOf(Ak)?Ak:null);if(((!!A_&&A_.Down)&&(A_.Jd!==this))&&B.qu(this.GetExtent(
),this.G.Ka(A_.CA))){this.Fb=0x20;this.V=this.V|67108864;return null;}}if(DX){var
GK=0;var Ee;this.V=this.V|(1<<Ac.Bv);for(Ee=this.V&4095;Ee>0;Ee=Ee>>1)if(!!(Ee&1
))GK=GK+1;if(GK===this.Il)this.V=(this.V|16777216)|(4096<<Ac.Bv);}if(Br)this.V=(
this.V&~(1<<Ac.Bv))|33554432;if(IX&&(this.V<16777216))this.V=this.V|67108864;if(
Br&&Ac.DI)this.V=this.V|67108864;if(Br&&!(this.V&4095))this.Fb=0x0;if(Br&&!(this.
V&16777215))this.V=0;if(Cx&&(this.V>=67108864))this.I3().Kn(null,null,this.Fb);if((
Cx&&!!(this.V&16777216))&&!!(this.V&33554432)){Cx=false;Br=true;}if(!!Ac&&!(this.
V&(4096<<Ac.Bv)))return this;if(!!Aq&&!(this.V&(4096<<Aq.Bv)))return this;if(Br&&
!(this.V&16777216))return this;if(((DX||DV)||Cx)&&((this.V<16777216)||(this.V>=33554432
)))return this;if(Br)this.V=this.V&3758100479;if(Br&&!(this.V&16777215))this.V=0;
}if(!!Ac){this.Down=DX||Cx;this.Fj=this.Oo(Ac.DJ);this.CX=Ac.CX;this.DJ=Ac.DJ;this.
As=AJ;this.BS=Ac.BS;this.Fq=Ac.Fq;this.DI=Ac.DI;this.Bv=Ac.Bv;this.Ah=Ac.Ah;if(Ac.
Down&&!Ac.BS)Nt=false;}if(!!Aq){this.Down=true;this.Fj=this.Oo(Aq.DJ);this.CX=Aq.
CX;this.DJ=Aq.DJ;this.As=Aq.As;this.BS=Aq.BS;this.Fq=Aq.Fq;this.Bv=Aq.Bv;this.DI=
false;this.Ah=Aq.Ah;(A=this.QI)?A[1].call(A[0],this):null;}if((!!Ac&&this.Down)&&
!this.BS)(A=this.CH)?A[1].call(A[0],this):null;if((!!Ac&&this.Down)&&(this.BS>0)
)(A=this.Lo)?A[1].call(A[0],this):null;if((this.Down&&this.Fj)&&!Nt)(A=this.FR)?
A[1].call(A[0],this):null;if(((this.Down&&!this.Fj)&&Nt)||((!this.Down&&this.Fj)&&
Nt))(A=this.FS)?A[1].call(A[0],this):null;if(!!Ac&&!this.Down)(A=this.D9)?A[1].call(
A[0],this):null;if(!!this.Ch){var Cm=0x0;if(((((this.Ch&0x10)===0x10)&&!!Ac)&&Ac.
Down)&&(Ac.BS>=this.LA))Cm=0x10;if((((this.Ch&0x1)===0x1)&&!!Aq)&&((Aq.CA[1]-Aq.
D4[1])<=-this.EN))Cm=0x1;if((((this.Ch&0x2)===0x2)&&!!Aq)&&((Aq.CA[1]-Aq.D4[1])>=
this.EN))Cm=0x2;if((((this.Ch&0x4)===0x4)&&!!Aq)&&((Aq.CA[0]-Aq.D4[0])<=-this.EN
))Cm=0x4;if((((this.Ch&0x8)===0x8)&&!!Aq)&&((Aq.CA[0]-Aq.D4[0])>=this.EN))Cm=0x8;
if(!!Cm){this.Fb=Cm;this.I3().Kn(null,this,Cm);}}return this;},Gj:function(AC,T,
DS,F$,Fv){var A;if(!!F$&&(F$!==this))return null;if((DS<this.Th)||(DS>this.Qz))return null;
if((this.Gn>=0)&&(this.Il>1))return null;if((this.Gn>=0)&&(T!==this.Gn))return null;
if(!this.N2&&(this.V>=33554432))return null;if((!this.N2&&(this.V>=16777216))&&!(
this.V&(4096<<T)))return null;if(!!(Fv&this.Ch))return null;if(this.SR()){var N=
B.il(AC,this.GetExtent());if(!((N[0]>=N[2])||(N[1]>=N[3]))){var Ci=B.s_(AC);var offset=
AJ;if(Ci[0]<N[0])offset=[N[0]-Ci[0],offset[1]];if(Ci[0]>=N[2])offset=[(N[2]-Ci[0
])-1,offset[1]];if(Ci[1]<N[1])offset=[offset[0],N[1]-Ci[1]];if(Ci[1]>=N[3])offset=[
offset[0],(N[3]-Ci[1])-1];return B._NewObject(E.K4,0).Initialize(this,offset);}}
else{var Bu=B.tA(9,B.qx,null);var I;Bu.Set(0,B.s_(AC));Bu.Set(1,Bu.Get(0));Bu.Set(
2,Bu.Get(0));Bu.Set(3,Bu.Get(0));Bu.Set(4,Bu.Get(0));Bu.Set(1,[AC[0],Bu.Get(1)[1
]]);Bu.Set(2,[Bu.Get(2)[0],AC[1]]);Bu.Set(3,[AC[2],Bu.Get(3)[1]]);Bu.Set(4,[Bu.Get(
4)[0],AC[3]]);Bu.Set(5,AC.slice(0,2));Bu.Set(6,[AC[2],AC[1]]);Bu.Set(7,[AC[0],AC[
3]]);Bu.Set(8,AC.slice(2,4));for(I=0;I<9;I=I+1)if(this.Oo(Bu.Get(I)))return B._NewObject(
E.K4,0).Initialize(this,B.tw(Bu.Get(I),Bu.Get(0)));}return null;},QX:function(C){
if(C<1)C=1;this.EN=C;},QT:function(C){if(C<1)C=1;this.Qz=C;},BG:function(C){if(C
)this.BB(0x10,0x0);else this.BB(0x0,0x10);},_Init:function(aArg){E.Ly._Init.call(
this,aArg);this.__proto__=E.G4;this.F=0x11B;},_Mark:function(D){var A;E.Ly._Mark.
call(this,D);if((A=this.QI)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
FS)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.FR)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Lo)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.D9)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.CH)&&((A=A[0
])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SimpleTouchHandler"};E.OU={
OJ:null,Et:null,Dr:null,Fb:0,V:0,Q3:0,CX:B.qx,K6:0,MS:0,Angle:0,LA:1000,EN:8,Ch:
0,Il:1,Gn:-1,Down:false,Fj:false,DI:false,Da:function(AA,aClip,aOffset,Aj,aBlend
){},Cq:function(Ak){var A;var Ac=(E.J1.isPrototypeOf(Ak)?Ak:null);var Aq=(E.J3.isPrototypeOf(
Ak)?Ak:null);var DX;var Cx;var IX;var Br;var DV;if((((this.Ch&0x20)===0x20)&&(this.
V>0))&&(this.V<33554432)){var A_=(E.J2.isPrototypeOf(Ak)?Ak:null);if(((!!A_&&A_.
Down)&&(A_.Jd!==this))&&B.qu(this.H,this.G.Ka(A_.CA))){this.Fb=0x20;this.V=this.
V|67108864;return null;}}if(!Ac&&!Aq)return null;DX=(!!Ac&&Ac.Down)&&!Ac.BS;Cx=(
!!Ac&&Ac.Down)&&(Ac.BS>0);IX=(!!Ac&&Ac.Down)&&(Ac.BS>50);Br=!!Ac&&!Ac.Down;DV=!!
Aq;if(DX){var GK=0;var Ee;this.V=this.V|(1<<Ac.Bv);for(Ee=this.V&4095;Ee>0;Ee=Ee>>
1)if(!!(Ee&1))GK=GK+1;if(GK===this.Il)this.V=(this.V|16777216)|(4096<<Ac.Bv);}if(
Br)this.V=(this.V&~(1<<Ac.Bv))|33554432;if(IX&&(this.V<16777216))this.V=this.V|67108864;
if(Br&&Ac.DI)this.V=this.V|67108864;if(Br&&!(this.V&4095))this.Fb=0x0;if(Br&&!(this.
V&16777215))this.V=0;if(Cx&&(this.V>=67108864))this.I3().Kn(null,null,this.Fb);if((
Cx&&!!(this.V&16777216))&&!!(this.V&33554432)){Cx=false;Br=true;}if(!!Ac&&!(this.
V&(4096<<Ac.Bv)))return this;if(!!Aq&&!(this.V&(4096<<Aq.Bv)))return this;if(Br&&
!(this.V&16777216))return this;if(((DX||DV)||Cx)&&((this.V<16777216)||(this.V>=33554432
)))return this;if(Br)this.V=this.V&3758100479;if(Br&&!(this.V&16777215))this.V=0;
if(((!DX&&!Br)&&!DV)&&!Cx)return this;var S=AJ;var AR=this.Angle;var NJ=this.Q3;
var VM=NJ;var Bh;if((DX||Br)||Cx){this.Fj=B.qu(this.H,Ac.DJ);this.DI=Ac.DI;this.
CX=Ac.CX;this.Down=DX||Cx;S=B.tw(Ac.DJ,B.s_(this.H));}if(DV){this.Fj=B.qu(this.H
,Aq.DJ);this.CX=Aq.CX;this.Down=true;S=B.tw(Aq.DJ,B.s_(this.H));}var N=Math.sqrt((
S[0]*S[0])+(S[1]*S[1]));if(N>=8.000000){NJ=N|0;AR=(Math.acos(S[0]/N)*B.ni)|0;if(
S[1]>0)AR=360-AR;}Bh=AR-this.Angle;if(Bh>180)Bh=Bh-360;if(Bh<-180)Bh=Bh+360;this.
Angle=AR;this.K6=Bh;this.MS=this.MS+Bh;this.Q3=NJ;if(DX){this.K6=0;this.MS=0;(A=
this.Dr)?A[1].call(A[0],this):null;}if(!!Bh||(NJ!==VM))(A=this.OJ)?A[1].call(A[0
],this):null;if(Br)(A=this.Et)?A[1].call(A[0],this):null;if(!!this.Ch){var Cm=0x0;
if((Cx&&((this.Ch&0x10)===0x10))&&(Ac.BS>=this.LA))Cm=0x10;if((DV&&((this.Ch&0x1
)===0x1))&&((Aq.CA[1]-Aq.D4[1])<=-this.EN))Cm=0x1;if((DV&&((this.Ch&0x2)===0x2))&&((
Aq.CA[1]-Aq.D4[1])>=this.EN))Cm=0x2;if((DV&&((this.Ch&0x4)===0x4))&&((Aq.CA[0]-Aq.
D4[0])<=-this.EN))Cm=0x4;if((DV&&((this.Ch&0x8)===0x8))&&((Aq.CA[0]-Aq.D4[0])>=this.
EN))Cm=0x8;if(!!Cm){this.Fb=Cm;this.I3().Kn(null,this,Cm);}}return this;},Gj:function(
AC,T,DS,F$,Fv){var A;if(!!F$&&(F$!==this))return null;if((this.Gn>=0)&&(this.Il>
1))return null;if((this.Gn>=0)&&(T!==this.Gn))return null;if(this.V>=33554432)return null;
if((this.V>=16777216)&&!(this.V&(4096<<T)))return null;if(!!(Fv&this.Ch))return null;
var N=B.il(AC,this.H);if(!((N[0]>=N[2])||(N[1]>=N[3]))){var Ci=B.s_(AC);var offset=
AJ;if(Ci[0]<N[0])offset=[N[0]-Ci[0],offset[1]];if(Ci[0]>=N[2])offset=[(N[2]-Ci[0
])-1,offset[1]];if(Ci[1]<N[1])offset=[offset[0],N[1]-Ci[1]];if(Ci[1]>=N[3])offset=[
offset[0],(N[3]-Ci[1])-1];return B._NewObject(E.K4,0).Initialize(this,offset);}return null;
},_Init:function(aArg){E.AQ._Init.call(this,aArg);this.__proto__=E.OU;this.F=0x11B;
},_Mark:function(D){var A;E.AQ._Mark.call(this,D);if((A=this.OJ)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Et)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.Dr)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::RotateTouchHandler"
};E.Iv={timer:null,NI:null,RT:null,KN:null,OO:null,Et:null,Dr:null,Fb:0,V:0,M9:0
,GF:5000.000000,JT:0,Ns:B.qx,Jx:0,He:0,AG:0,IW:0,KT:0,Jw:0,Hd:0,AF:0,IV:0,IU:0,CX:
B.qx,K6:B.qx,Il:1,Gn:-1,Ba:B.qx,Bb:B.qx,As:B.qx,MX:B.qx,Fp:B.qx,MW:B.qx,Q6:5.000000
,T5:500,LA:1000,EN:8,Ch:0,OS:-1,Q5:200,HJ:0,Qn:0.500000,T6:true,OX:true,OW:true,
DQ:false,FH:false,FI:false,M8:false,Da:function(AA,aClip,aOffset,Aj,aBlend){},Cq:
function(Ak){var A;var Ac=(E.J1.isPrototypeOf(Ak)?Ak:null);var Aq=(E.J3.isPrototypeOf(
Ak)?Ak:null);var A_=(E.J2.isPrototypeOf(Ak)?Ak:null);var DX;var Cx;var IX;var Br;
var DV;if((((((((this.Ch&0x20)===0x20)&&(this.V>0))&&(this.V<33554432))&&!!A_)&&
A_.Down)&&(A_.Jd!==this))&&B.qu(this.H,this.G.Ka(A_.CA))){this.Fb=0x20;this.V=this.
V|67108864;}DX=(!!Ac&&Ac.Down)&&!Ac.BS;Cx=(!!Ac&&Ac.Down)&&(Ac.BS>0);IX=(!!Ac&&Ac.
Down)&&(Ac.BS>50);Br=!!Ac&&!Ac.Down;DV=!!Aq;if(DX){var GK=0;var Ee;this.V=this.V|(
1<<Ac.Bv);for(Ee=this.V&4095;Ee>0;Ee=Ee>>1)if(!!(Ee&1))GK=GK+1;if(GK===this.Il)this.
V=(this.V|16777216)|(4096<<Ac.Bv);}if(Br)this.V=(this.V&~(1<<Ac.Bv))|33554432;if(
IX&&(this.V<16777216))this.V=this.V|67108864;if(Br&&Ac.DI)this.V=this.V|67108864;
if(Br&&!(this.V&4095))this.Fb=0x0;if(Br&&!(this.V&16777215))this.V=0;if(Cx&&(this.
V>=67108864))this.I3().Kn(null,null,this.Fb);if((Cx&&!!(this.V&16777216))&&!!(this.
V&33554432)){Cx=false;Br=true;}if(!!Ac&&!(this.V&(4096<<Ac.Bv)))return this;if(!
!Aq&&!(this.V&(4096<<Aq.Bv)))return this;if(Br&&!(this.V&16777216))return this;if(((
DX||DV)||Cx)&&((this.V<16777216)||(this.V>=33554432)))return this;if(Br)this.V=this.
V&3758100479;if(Br&&!(this.V&16777215))this.V=0;if(!!A_&&(A_.Jd===this))return null;
if((!!A_&&A_.Down)&&(this.M8||!this.DQ))return null;if((!!A_&&A_.Down)&&!B.qu(this.
H,this.G.Ka(A_.CA)))return null;if((!!A_&&!A_.Down)&&(!this.M8||(this.M9!==A_.Bv
)))return null;if((!Ac&&!Aq)&&!A_)return null;if(!!Ac){this.M8=DX||Cx;this.M9=Ac.
Bv;this.CX=Ac.CX;}if(!!Aq)this.M9=Aq.Bv;if(!!A_){this.M8=A_.Down;this.M9=A_.Bv;}
if(!!A_&&A_.Down){this.P2();this.AF=(((A_.Ah-this.IU)*0.001000)*this.Hd)+this.AF;
this.AG=(((A_.Ah-this.KT)*0.001000)*this.He)+this.AG;if(this.FH)this.AF=0.000000;
if(this.FI)this.AG=0.000000;this.Hd=0.000000;this.He=0.000000;this.FH=false;this.
FI=false;this.Ns=this.As;this.JT=A_.Ah;return this;}if(DX){this.P2();this.AF=(((
Ac.Ah-this.IU)*0.001000)*this.Hd)+this.AF;this.AG=(((Ac.Ah-this.KT)*0.001000)*this.
He)+this.AG;if(this.FH||!this.DQ)this.AF=0.000000;if(this.FI||!this.DQ)this.AG=0.000000;
this.Hd=0.000000;this.He=0.000000;this.FH=false;this.FI=false;if(!this.DQ){this.
DQ=true;(A=this.Dr)?A[1].call(A[0],this):null;(A=this.NI)?A[1].call(A[0],this):null;
}this.Ns=this.As;this.JT=Ac.Ah;}if(!!Aq){var Bh=B.tw(Aq.DJ,Aq.CX);var Am=this.As;
if(this.OW)Am=[this.Ns[0]+Bh[0],Am[1]];if(this.OX)Am=[Am[0],this.Ns[1]+Bh[1]];if(
this.T6){if(Am[0]<this.Bb[0])Am=[this.Bb[0]+(((Am[0]-this.Bb[0])/2)|0),Am[1]];else
if(Am[0]>this.Ba[0])Am=[this.Ba[0]+(((Am[0]-this.Ba[0])/2)|0),Am[1]];if(Am[1]<this.
Bb[1])Am=[Am[0],this.Bb[1]+(((Am[1]-this.Bb[1])/2)|0)];else if(Am[1]>this.Ba[1])
Am=[Am[0],this.Ba[1]+(((Am[1]-this.Ba[1])/2)|0)];}else{if(Am[0]<this.Bb[0])Am=[this.
Bb[0],Am[1]];else if(Am[0]>this.Ba[0])Am=[this.Ba[0],Am[1]];if(Am[1]<this.Bb[1])
Am=[Am[0],this.Bb[1]];else if(Am[1]>this.Ba[1])Am=[Am[0],this.Ba[1]];}if(!B.tl(Am
,this.As)){this.K6=B.tw(Am,this.As);this.As=Am;(A=this.KN)?A[1].call(A[0],this):
null;(A=this.OO)?A[1].call(A[0],this):null;}}if((!!A_&&!A_.Down)&&((A_.Ah-this.JT
)>=(((A=this.Q5)<0)?A+0x100000000:A))){this.AF=0.000000;this.AG=0.000000;}if(Cx&&((
Ac.Ah-this.JT)>=(((A=this.Q5)<0)?A+0x100000000:A))){this.AF=0.000000;this.AG=0.000000;
}if(Br&&(this.OS>=0)){var CR=B.tw(Ac.DJ,Ac.CX);if(((CR[0]*CR[0])+(CR[1]*CR[1]))<=(
this.OS*this.OS)){this.AF=0.000000;this.AG=0.000000;}}if(!!Aq&&(Aq.Ah>this.JT)){
var Bh=Aq.As;var RF=1000.000000/(Aq.Ah-this.JT);var H2=0.000000;var H3=0.000000;
if(this.OW)H2=Bh[0]*RF;if(this.OX)H3=Bh[1]*RF;if((H2*this.AF)<0.000000)this.AF=0.000000;
if((H3*this.AG)<0.000000)this.AG=0.000000;this.AF=(this.AF+H2)*0.500000;this.AG=(
this.AG+H3)*0.500000;if(this.HJ>0.000000){if(this.AF>this.HJ)this.AF=this.HJ;if(
this.AF<-this.HJ)this.AF=-this.HJ;if(this.AG>this.HJ)this.AG=this.HJ;if(this.AG<-
this.HJ)this.AG=-this.HJ;}this.JT=Aq.Ah;}if(!!this.Ch){var Cm=0x0;if((Cx&&((this.
Ch&0x10)===0x10))&&(Ac.BS>=this.LA))Cm=0x10;if((DV&&((this.Ch&0x1)===0x1))&&((Aq.
CA[1]-Aq.D4[1])<=-this.EN))Cm=0x1;if((DV&&((this.Ch&0x2)===0x2))&&((Aq.CA[1]-Aq.
D4[1])>=this.EN))Cm=0x2;if((DV&&((this.Ch&0x4)===0x4))&&((Aq.CA[0]-Aq.D4[0])<=-this.
EN))Cm=0x4;if((DV&&((this.Ch&0x8)===0x8))&&((Aq.CA[0]-Aq.D4[0])>=this.EN))Cm=0x8;
if(!!Cm){this.Fb=Cm;this.I3().Kn(null,this,Cm);}}if(!Br&&!A_)return this;if(!!Ac&&
Ac.DI){this.AF=0.000000;this.AG=0.000000;}if((this.As[0]<=this.Bb[0])||(this.As[
0]>=this.Ba[0]))this.AF=0.000000;else if(this.AF===0.000000){var AO=this.As[0];var
AX=this.Bb[0]+this.MX[0];var AM=this.Ba[0]-this.MW[0];if(AO<AX)AM=this.Bb[0];else
if(AO>AM)AX=this.Ba[0];else if(this.Fp[0]<=1){AX=AO;AM=AO;}else{var GI=(AM-AO)%this.
Fp[0];AM=(AO-this.Fp[0])+GI;AX=AO+GI;if(AM<this.Bb[0])AM=this.Bb[0];if(AX>this.Ba[
0])AX=this.Ba[0];}if((AX-AO)<=(AO-AM))AO=AX;else AO=AM;if(AO!==this.As[0]){var C6=
AO-this.As[0];if(C6>0.000000)this.AF=Math.sqrt((C6*2.000000)*this.GF)+20.000000;
if(C6<0.000000)this.AF=-Math.sqrt((-C6*2.000000)*this.GF)-20.000000;this.Hd=(400.000000-(
this.AF*this.AF))/(2.000000*C6);this.Jw=AO;}}else{var V$=this.AF*this.AF;var C6=
V$/(2.000000*this.GF);var AO=this.As[0];if(this.AF>0.000000)AO=AO+(C6|0);if(this.
AF<0.000000)AO=AO-(C6|0);if(AO>this.Ba[0])AO=this.Ba[0];else if(AO<this.Bb[0])AO=
this.Bb[0];var R_=AO;var AX=this.Bb[0]+this.MX[0];var AM=this.Ba[0]-this.MW[0];if(
AO<AX)AM=this.Bb[0];else if(AO>AM)AX=this.Ba[0];else if(this.Fp[0]<=1){AX=AO;AM=
AO;}else{var GI=(AM-AO)%this.Fp[0];AM=(AO-this.Fp[0])+GI;AX=AO+GI;if(AM<this.Bb[
0])AM=this.Bb[0];if(AX>this.Ba[0])AX=this.Ba[0];}if(this.AF>0.000000)if(AM<=this.
As[0])AO=AX;else if((AO-AM)<(AX-AO))AO=AM;else AO=AX;else if(AX>=this.As[0])AO=AM;
else if((AO-AM)>(AX-AO))AO=AX;else AO=AM;if(AO!==this.As[0]){C6=AO-this.As[0];if(
AO!==R_){var H_=AO-R_;if(H_>0.000000)this.AF=this.AF+Math.sqrt((H_*2.000000)*this.
GF);if(H_<0.000000)this.AF=this.AF-Math.sqrt((-H_*2.000000)*this.GF);}if(this.AF>
0.000000)this.AF=this.AF+20.000000;if(this.AF<0.000000)this.AF=this.AF-20.000000;
this.Hd=(400.000000-(this.AF*this.AF))/(2.000000*C6);this.Jw=AO;}else this.AF=0.000000;
}if((this.As[1]<=this.Bb[1])||(this.As[1]>=this.Ba[1]))this.AG=0.000000;else if(
this.AG===0.000000){var AP=this.As[1];var AX=this.Bb[1]+this.MX[1];var AM=this.Ba[
1]-this.MW[1];if(AP<AX)AM=this.Bb[1];else if(AP>AM)AX=this.Ba[1];else if(this.Fp[
1]<=1){AX=AP;AM=AP;}else{var GI=(AM-AP)%this.Fp[1];AM=(AP-this.Fp[1])+GI;AX=AP+GI;
if(AM<this.Bb[1])AM=this.Bb[1];if(AX>this.Ba[1])AX=this.Ba[1];}if((AX-AP)<=(AP-AM
))AP=AX;else AP=AM;if(AP!==this.As[1]){var C6=AP-this.As[1];if(C6>0.000000)this.
AG=Math.sqrt((C6*2.000000)*this.GF)+20.000000;if(C6<0.000000)this.AG=-Math.sqrt((-
C6*2.000000)*this.GF)-20.000000;this.He=(400.000000-(this.AG*this.AG))/(2.000000
*C6);this.Jx=AP;}}else{var Wa=this.AG*this.AG;var C6=Wa/(2.000000*this.GF);var AP=
this.As[1];if(this.AG>0.000000)AP=AP+(C6|0);if(this.AG<0.000000)AP=AP-(C6|0);if(
AP>this.Ba[1])AP=this.Ba[1];else if(AP<this.Bb[1])AP=this.Bb[1];var R$=AP;var AX=
this.Bb[1]+this.MX[1];var AM=this.Ba[1]-this.MW[1];if(AP<AX)AM=this.Bb[1];else if(
AP>AM)AX=this.Ba[1];else if(this.Fp[1]<=1){AX=AP;AM=AP;}else{var GI=(AM-AP)%this.
Fp[1];AM=(AP-this.Fp[1])+GI;AX=AP+GI;if(AM<this.Bb[1])AM=this.Bb[1];if(AX>this.Ba[
1])AX=this.Ba[1];}if(this.AG>0.000000)if(AM<=this.As[1])AP=AX;else if((AP-AM)<(AX-
AP))AP=AM;else AP=AX;else if(AX>=this.As[1])AP=AM;else if((AP-AM)>(AX-AP))AP=AX;
else AP=AM;if(AP!==this.As[1]){C6=AP-this.As[1];if(AP!==R$){var H_=AP-R$;if(H_>0.000000
)this.AG=this.AG+Math.sqrt((H_*2.000000)*this.GF);if(H_<0.000000)this.AG=this.AG-
Math.sqrt((-H_*2.000000)*this.GF);}if(this.AG>0.000000)this.AG=this.AG+20.000000;
if(this.AG<0.000000)this.AG=this.AG-20.000000;this.He=(400.000000-(this.AG*this.
AG))/(2.000000*C6);this.Jx=AP;}else this.AG=0.000000;}if(!!Ac)this.IU=Ac.Ah;if(!
!A_)this.IU=A_.Ah;this.KT=this.IU;this.IV=this.As[0];this.IW=this.As[1];this.Wb(
);return this;},Gj:function(AC,T,DS,F$,Fv){var A;if(!!F$&&(F$!==this))return null;
if((this.Gn>=0)&&(this.Il>1))return null;if((this.Gn>=0)&&(T!==this.Gn))return null;
if(this.V>=33554432)return null;if((this.V>=16777216)&&!(this.V&(4096<<T)))return null;
if(!!(Fv&this.Ch))return null;if(!this.OX&&!!(Fv&0x3))return null;if(!this.OW&&!
!(Fv&0xC))return null;var N=B.il(AC,this.H);if(!((N[0]>=N[2])||(N[1]>=N[3]))){var
Ci=B.s_(AC);var offset=AJ;if(Ci[0]<N[0])offset=[N[0]-Ci[0],offset[1]];if(Ci[0]>=
N[2])offset=[(N[2]-Ci[0])-1,offset[1]];if(Ci[1]<N[1])offset=[offset[0],N[1]-Ci[1
]];if(Ci[1]>=N[3])offset=[offset[0],(N[3]-Ci[1])-1];return B._NewObject(E.K4,0).
Initialize(this,offset);}return null;},P2:function(){if(!!this.timer){B.sM([this
,this.EH],this.timer,0);this.timer=null;}},Wb:function(){this.timer=B._GetAutoObject(
B.uk.J4);B.sz([this,this.EH],this.timer,0);},EH:function(Af){var A;if(!this.timer
)return;var KU=(this.timer.Ah-this.IU)*0.001000;var KV=(this.timer.Ah-this.KT)*0.001000;
var Wc=KU*KU;var Wd=KV*KV;var H2=(this.Hd*KU)+this.AF;var H3=(this.He*KV)+this.AG;
var Am=[((((this.Hd*0.500000)*Wc)+(this.AF*KU))+this.IV)|0,((((this.He*0.500000)
*Wd)+(this.AG*KV))+this.IW)|0];var NG=this.T5*0.001000;if(this.FH&&(KU>=NG)){Am=[
this.Jw,Am[1]];this.AF=0.000000;this.Hd=0.000000;this.IV=Am[0];this.FH=false;}else
if(this.FH){var A$=1.000000-Math.pow(1.000000-(KU/NG),this.Q6);Am=[(this.IV+((this.
Jw-this.IV)*A$))|0,Am[1]];}if(this.FI&&(KV>=NG)){Am=[Am[0],this.Jx];this.AG=0.000000;
this.He=0.000000;this.IW=Am[1];this.FI=false;}else if(this.FI){var A$=1.000000-Math.
pow(1.000000-(KV/NG),this.Q6);Am=[Am[0],(this.IW+((this.Jx-this.IW)*A$))|0];}if(((
this.AF>0.000000)&&(H2<0.000000))||((this.AF<0.000000)&&(H2>0.000000))){H2=0.000000;
Am=[this.As[0],Am[1]];}if(((this.AG>0.000000)&&(H3<0.000000))||((this.AG<0.000000
)&&(H3>0.000000))){H3=0.000000;Am=[Am[0],this.As[1]];}if(!this.FH&&(Am[0]<this.Bb[
0])){this.IV=Am[0];this.Jw=this.Bb[0];this.IU=this.timer.Ah;this.FH=true;}else if(
!this.FH&&(Am[0]>this.Ba[0])){this.IV=Am[0];this.Jw=this.Ba[0];this.IU=this.timer.
Ah;this.FH=true;}if(!this.FI&&(Am[1]<this.Bb[1])){this.IW=Am[1];this.Jx=this.Bb[
1];this.KT=this.timer.Ah;this.FI=true;}else if(!this.FI&&(Am[1]>this.Ba[1])){this.
IW=Am[1];this.Jx=this.Ba[1];this.KT=this.timer.Ah;this.FI=true;}if(((!this.FH&&(
this.AF!==0.000000))&&(H2>-20.000000))&&(H2<20.000000)){Am=[this.Jw,Am[1]];this.
AF=0.000000;this.Hd=0.000000;this.IV=Am[0];}if(((!this.FI&&(this.AG!==0.000000))&&(
H3>-20.000000))&&(H3<20.000000)){Am=[Am[0],this.Jx];this.AG=0.000000;this.He=0.000000;
this.IW=Am[1];}if(!B.tl(Am,this.As)){this.K6=B.tw(Am,this.As);this.As=Am;(A=this.
KN)?A[1].call(A[0],this):null;(A=this.OO)?A[1].call(A[0],this):null;}if((((this.
AF===0.000000)&&(this.AG===0.000000))&&!this.FH)&&!this.FI){this.P2();this.DQ=false;(
A=this.RT)?A[1].call(A[0],this):null;(A=this.Et)?A[1].call(A[0],this):null;}},FY:
function(C){if(C[0]<0)C=[0,C[1]];if(C[1]<0)C=[C[0],0];this.Fp=C;},Kg:function(C){
if(C<0.000000)C=0.000000;if(C>1.000000)C=1.000000;if(C===this.Qn)return;this.Qn=
C;if(C<0.000000)C=0.000000;this.GF=C*10000.000000;},_Init:function(aArg){E.AQ._Init.
call(this,aArg);this.__proto__=E.Iv;this.F=0x11B;},_Mark:function(D){var A;E.AQ.
_Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
NI)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.RT)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.KN)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.OO)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Et)&&((A=A[0
])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Dr)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Core::SlideTouchHandler"};E.J9={Ad:null,D9:null,CH:null,
Lo:null,JS:0,Ah:0,Q4:0,Fi:148,Ce:0,A9:0,B5:true,Down:false,OR:false,Ml:false,Cq:
function(Ak){var A;if(!!Ak&&Ak.S5(this.Fi)){this.Down=Ak.Down;this.Ce=Ak.Ce;this.
A9=Ak.A9;this.Ah=Ak.Ah;this.Ml=false;if(Ak.Down){this.Q4=this.JS;this.OR=this.JS>
0;if(this.OR)(A=this.Lo)?A[1].call(A[0],this):null;else(A=this.CH)?A[1].call(A[0
],this):null;if(!this.Ml)this.JS=this.JS+1;return!this.Ml;}if(!Ak.Down){this.OR=
this.JS>1;this.Q4=this.JS-1;this.JS=0;(A=this.D9)?A[1].call(A[0],this):null;return!
this.Ml;}}return false;},CY:function(aArg){var A;var Bz=(E.J.isPrototypeOf(A=this.
M)?A:null);if(!Bz)throw new Error(O$);this.Ad=Bz.Nx;Bz.Nx=this;},_Init:function(
aArg){this.__proto__=E.J9;this.CY(aArg);B.gv++;},_Done:function(){this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.Ad)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.D9)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.CH)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Lo)&&((A=A[
0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=
D);},M:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"};E.K4={AE:
null,K7:0,As:B.qx,Initialize:function(W,aOffset){this.AE=W;this.As=aOffset;this.
K7=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:function(
aArg){this.__proto__=E.K4;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.AE)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:
null,_className:"Core::CursorHit"};E.Ti={Bz:null,_Init:function(aArg){this.__proto__=
E.Ti;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Bz)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:
"Core::ModalContext"};E.J_={IO:null,CS:B.qy,U:B.qy,isEmpty:false,_Init:function(
aArg){this.__proto__=E.J_;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.IO)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:
null,_className:"Core::LayoutContext"};E.ME={H6:B.qx,H5:B.qx,_Init:function(aArg
){E.J_._Init.call(this,aArg);this.__proto__=E.ME;},_className:"Core::LayoutLineContext"
};E.MF={L$:B.qx,L_:B.qx,H6:B.qx,H5:B.qx,_Init:function(aArg){E.J_._Init.call(this
,aArg);this.__proto__=E.MF;},_className:"Core::LayoutQuadContext"};E.NZ={Bz:null
,Ad:null,Hh:null,H8:null,EE:null,L8:null,_Init:function(aArg){this.__proto__=E.NZ;
B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.Bz)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ad
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Hh)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.H8)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.EE)&&(A._cycle!=
D))A._Mark(A._cycle=D);if((A=this.L8)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:
"Core::DialogContext"};E.Q_={P4:null,Ez:null,AB:null,Aw:null,QH:null,Nv:false,Vv:
function(){if(!this.Ez)return;var Fc=this.Ez;this.Ez.Fa=null;this.Ez=null;this.P4=
null;if(this.Nv)B.lq([this,this.RN],this);else B.lq([this,this.NE],this);Fc.QG(this
);},NE:function(Af){if(!!this.Ez)return;if(!this.Aw){B.lq(this.QH,this);return;}
this.Ez=this.Aw;this.Aw=this.Aw.Ad;if(!!this.Aw)this.Aw.Ag=null;else this.AB=null;
this.Ez.Ad=null;this.Nv=true;this.Ez.Dr(this);this.Nv=false;},RN:function(Af){B.
lq([this,this.NE],this);},Sm:function(B0){if(!B0||!B0.Fa)return;if(B0.Fa!==this)
throw new Error(Pa);var Sg=false;if(this.Ez===B0){this.Ez=null;this.P4=null;Sg=true;
if(this.Nv)B.lq([this,this.RN],this);else B.lq([this,this.NE],this);}else{if(!!B0.
Ad)B0.Ad.Ag=B0.Ag;else this.AB=B0.Ag;if(!!B0.Ag)B0.Ag.Ad=B0.Ad;else this.Aw=B0.Ad;
B0.Ag=null;B0.Ad=null;}B0.Fa=null;if(Sg)B0.QF(this);},T_:function(B0,Vt){if(!B0)
return;if(!!B0.Fa)throw new Error(Pb);B0.Fa=this;if(Vt){B0.Ad=this.Aw;if(!!this.
Aw)this.Aw.Ag=B0;else this.AB=B0;this.Aw=B0;}else{B0.Ag=this.AB;if(!!this.AB)this.
AB.Ad=B0;else this.Aw=B0;this.AB=B0;}if(!this.Ez)B.lq([this,this.NE],this);},_Init:
function(aArg){this.__proto__=E.Q_;B.gv++;},_Done:function(){this.__proto__=null;
B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.P4)&&(A._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Ez)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
AB)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Aw)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.QH)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.M)&&(A._cycle
!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:"Core::TaskQueue"
};E.LC={Fa:null,Ag:null,Ad:null,QG:function(LI){},QF:function(LI){},Dr:function(
LI){this.Qe();},Gi:function(){if(!!this.Fa&&(this.Fa.Ez===this))this.Fa.Vv();},Qe:
function(){if(!!this.Fa)this.Fa.Sm(this);},Om:function(){return!!this.Fa&&(this.
Fa.Ez===this);},_Init:function(aArg){this.__proto__=E.LC;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Fa)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ag)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Ad)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.M)&&(A.
_cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:"Core::Task"
};E.HE={resource:null,D3:function(){this.resource=null;},CY:function(aArg){this.
resource=aArg;},_Init:function(aArg){this.__proto__=E.HE;this.CY(aArg);B.gv++;},
_Done:function(){this.D3();this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:
0,_observers:null,_className:"Core::Resource"};E.Timer={Is:null,timer:null,Ah:0,
Period:1000,Ff:0,B5:false,D3:function(){var tmp=this.timer;if(!!tmp)tmp.DestroyTimer(
);this.timer=null;},NL:function(aBegin,aPeriod){if(aBegin<0)aBegin=0;if(aPeriod<
0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>0)||(aPeriod>0)))tmp=B.sL(this,
this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(aBegin,aPeriod);}this.timer=
tmp;},Lu:function(C){if(C<0)C=0;if(C===this.Period)return;this.Period=C;if(this.
B5)this.NL(this.Ff,C);},Ip:function(C){if(C<0)C=0;if(C===this.Ff)return;this.Ff=
C;if(this.B5)this.NL(C,this.Period);},BG:function(C){if(C===this.B5)return;this.
B5=C;if(C)this.NL(this.Ff,this.Period);else this.NL(0,0);this.Ah=this.Lf();},Lf:
function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.qt)|0;return ticksCount;
},Trigger:function(){var A;this.Ah=this.Lf();if(!this.Period)this.BG(false);(A=this.
Is)?A[1].call(A[0],this):null;},_Init:function(aArg){this.__proto__=E.Timer;B.gv++;
},_Done:function(){this.D3();this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.Is)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=
this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:
"Core::Timer"};E.Ah={LB:0,Ll:0,Lh:0,K5:0,Lm:0,O1:0,VF:function(Gb,Pf){if(Pf)switch(
Gb){case 1:return B.ui.Qt;case 2:return B.ui.Qm;case 3:return B.ui.Qy;case 4:return B.
ui.Qc;case 5:return B.ui.QA;case 6:return B.ui.Qv;case 7:return B.ui.Qu;case 8:return B.
ui.Qd;case 9:return B.ui.Q7;case 10:return B.ui.QD;case 11:return B.ui.QC;case 12:
return B.ui.Qg;default:return B.hm;}else switch(Gb){case 1:return B.ui.S6;case 2:
return B.ui.SL;case 3:return B.ui.Tf;case 4:return B.ui.Si;case 5:return B.ui.Tg;
case 6:return B.ui.S8;case 7:return B.ui.S7;case 8:return B.ui.Sj;case 9:return B.
ui.T$;case 10:return B.ui.Tn;case 11:return B.ui.Tm;case 12:return B.ui.So;default:
return B.hm;}},VE:function(Rj,Pf){if(Pf)switch(Rj){case 1:return B.ui.Tk;case 2:
return B.ui.Uj;case 3:return B.ui.Ul;case 4:return B.ui.Ug;case 5:return B.ui.SN;
case 6:return B.ui.T8;case 0:return B.ui.Ue;default:return B.hm;}else switch(Rj){
case 1:return B.ui.Tj;case 2:return B.ui.Ui;case 3:return B.ui.Uk;case 4:return B.
ui.Uf;case 5:return B.ui.SM;case 6:return B.ui.T7;case 0:return B.ui.Ud;default:
return B.hm;}},VD:function(){var result;result=((new Date).getTime()/1000)|0;return result;
},Pq:function(Gb){switch(Gb){case 0:return 0;case 1:return 31;case 2:return 60;case
3:return 91;case 4:return 121;case 5:return 152;case 6:return 182;case 7:return 213;
case 8:return 244;case 9:return 274;case 10:return 305;case 11:return 335;case 12:
return 366;default:return 0;}},Pp:function(Gb){switch(Gb){case 0:return 0;case 1:
return 31;case 2:return 59;case 3:return 90;case 4:return 120;case 5:return 151;
case 6:return 181;case 7:return 212;case 8:return 243;case 9:return 273;case 10:
return 304;case 11:return 334;case 12:return 365;default:return 0;}},QK:function(
){var D2=this.O1;var Jt;if(!!(D2%4)){D2=D2-1970;Jt=(D2*365)+(((D2+1)/4)|0);Jt=((
Jt+this.Pp(this.Lm-1))+this.K5)-1;}else{D2=D2-1970;Jt=(D2*365)+(((D2+1)/4)|0);Jt=((
Jt+this.Pq(this.Lm-1))+this.K5)-1;}return((((((((A=Jt)<0)?A+0x100000000:A)*60)*60
)*24)+(((((A=this.Lh)<0)?A+0x100000000:A)*60)*60))+((((A=this.Ll)<0)?A+0x100000000:
A)*60))+(((A=this.LB)<0)?A+0x100000000:A);},T0:function(C){var EB=(((C/86400)|0)+
365)|0;var Sh=(EB/1461)|0;var D2;var Hk;EB=EB-(Sh*1461);D2=(EB/365)|0;if(D2===4)
D2=3;EB=EB-(D2*365);D2=((Sh*4)+D2)+1969;Hk=EB>>5;if(!!(D2%4)){if(EB>=this.Pp(Hk+
1))Hk=Hk+1;EB=EB-this.Pp(Hk);}else{if(EB>=this.Pq(Hk+1))Hk=Hk+1;EB=EB-this.Pq(Hk
);}this.MO(D2);this.MM(Hk+1);this.I7(EB+1);this.MK((((C/3600)|0)%24)|0);this.ML((((
C/60)|0)%60)|0);this.MN((C%60)|0);},QJ:function(){return B._NewObject(E.Ah,0).Initialize(
this.VD());},Tq:function(){return((((this.QK()/86400)|0)+4)%7)|0;},MN:function(C
){if(C<0)C=0;if(C>59)C=59;this.LB=C;},ML:function(C){if(C<0)C=0;if(C>59)C=59;this.
Ll=C;},MK:function(C){if(C<0)C=0;if(C>23)C=23;this.Lh=C;},I7:function(C){if(C<1)
C=1;if(C>31)C=31;this.K5=C;},MM:function(C){if(C<1)C=1;if(C>12)C=12;this.Lm=C;},
MO:function(C){if(C<1970)C=1970;this.O1=C;},Initialize:function(Ro){this.T0(Ro);
return this;},_Init:function(aArg){this.__proto__=E.Ah;B.gv++;},_Done:function(){
this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=
this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:
"Core::Time"};E.HB={OH:null,Av:null,B5:true,RL:function(Af){var A;(A=this.OH)?A[
1].call(A[0],this):null;},I8:function(C){if(B.tn(this.Av,C))return;if(!!this.Av&&
this.B5)B.sO([this,this.RL],this.Av,0);this.Av=C;if(!!this.Av&&this.B5)B.sB([this
,this.RL],this.Av,0);},_Init:function(aArg){this.__proto__=E.HB;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.OH)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Av)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null
,_cycle:0,_observers:null,_className:"Core::PropertyObserver"};E.XP={BT:0x1,Wp:0x2
,WY:0x4,XM:0x8,B5:0x10,HG:0x20,WZ:0x40,W_:0x80,WW:0x100,W3:0x200,WP:0x400,Xn:0x800
,Ha:0x1000,XO:0x2000,Xe:0x4000,Xf:0x8000,WI:0x10000,Xd:0x20000,Xx:0x40000};E.D7={
Xo:0x1,Xp:0x2,Wm:0x4,Wn:0x8,Wo:0x10,Wl:0x20};E.Ms={Tl:0,XJ:1,Wy:2,W4:3,Xr:4,XK:5
,XL:6,Wz:7,WA:8,W6:9,W5:10,Xt:11,Xs:12};E.I2={Tl:0,XH:1,Left:2,Ww:3,Uh:4,Sl:5,XI:
6,Right:7,Wx:8};E.KeyCode={NoKey:0,Ok:1,Exit:2,Menu:3,Up:4,Down:5,Left:6,Right:7
,PageUp:8,PageDown:9,Key0:10,Key1:11,Key2:12,Key3:13,Key4:14,Key5:15,Key6:16,Key7:
17,Key8:18,Key9:19,Red:20,Green:21,Blue:22,Yellow:23,White:24,Magenta:25,F1:26,F2:
27,F3:28,F4:29,F5:30,F6:31,F7:32,F8:33,F9:34,F10:35,ChannelUp:36,ChannelDown:37,
Display:38,SkipPrev:39,SkipNext:40,Home:41,End:42,Insert:43,Delete:44,Clear:45,VolumeUp:
46,VolumeDown:47,Show:48,Hide:49,Play:50,Pause:51,Record:52,Stop:53,Rev:54,Fwd:55
,SlowRev:56,SlowFwd:57,SkipBwd:58,SkipFwd:59,Repeat:60,Eject:61,Help:62,TV:63,DVD:
64,VCR:65,EPG:66,OSD:67,Text:68,PIP:69,Audio:70,Clock:71,Timer:72,Navigation:73,
Karaoke:74,Game:75,Subtitle:76,Zoom:77,Index:78,Info:79,Power:80,Setup:81,Angle:
82,Mode:83,Mute:84,User0:85,User1:86,User2:87,User3:88,User4:89,User5:90,User6:91
,User7:92,User8:93,User9:94,User10:95,User11:96,User12:97,User13:98,User14:99,User15:
100,User16:101,User17:102,User18:103,User19:104,KeyA:105,KeyB:106,KeyC:107,KeyD:
108,KeyE:109,KeyF:110,KeyG:111,KeyH:112,KeyI:113,KeyJ:114,KeyK:115,KeyL:116,KeyM:
117,KeyN:118,KeyO:119,KeyP:120,KeyQ:121,KeyR:122,KeyS:123,KeyT:124,KeyU:125,KeyV:
126,KeyW:127,KeyX:128,KeyY:129,KeyZ:130,Space:131,Plus:132,Minus:133,Multiply:134
,Divide:135,Equals:136,Period:137,Comma:138,Colon:139,Semicolon:140,AlphaKeys:141
,AlphaOrDigitKeys:142,DigitKeys:143,HexDigitKeys:144,CharacterKeys:145,ControlKeys:
146,CursorKeys:147,AnyKey:148,Enter:149,Escape:150,Backspace:151,Tab:152,CtrlKeyA:
153,CtrlKeyB:154,CtrlKeyC:155,CtrlKeyD:156,CtrlKeyE:157,CtrlKeyF:158,CtrlKeyG:159
,CtrlKeyH:160,CtrlKeyI:161,CtrlKeyJ:162,CtrlKeyK:163,CtrlKeyL:164,CtrlKeyM:165,CtrlKeyN:
166,CtrlKeyO:167,CtrlKeyP:168,CtrlKeyQ:169,CtrlKeyR:170,CtrlKeyS:171,CtrlKeyT:172
,CtrlKeyU:173,CtrlKeyV:174,CtrlKeyW:175,CtrlKeyX:176,CtrlKeyY:177,CtrlKeyZ:178,CtrlSpace:
179,CtrlKey0:180,CtrlKey1:181,CtrlKey2:182,CtrlKey3:183,CtrlKey4:184,CtrlKey5:185
,CtrlKey6:186,CtrlKey7:187,CtrlKey8:188,CtrlKey9:189,CtrlF1:190,CtrlF2:191,CtrlF3:
192,CtrlF4:193,CtrlF5:194,CtrlF6:195,CtrlF7:196,CtrlF8:197,CtrlF9:198,CtrlF10:199
,CtrlEnter:200,CtrlEscape:201,CtrlUp:202,CtrlDown:203,CtrlLeft:204,CtrlRight:205
,CtrlPageUp:206,CtrlPageDown:207,CtrlBackspace:208,CtrlInsert:209,CtrlDelete:210
,CtrlHome:211,CtrlEnd:212,CtrlTab:213,CtrlShiftKeyA:214,CtrlShiftKeyB:215,CtrlShiftKeyC:
216,CtrlShiftKeyD:217,CtrlShiftKeyE:218,CtrlShiftKeyF:219,CtrlShiftKeyG:220,CtrlShiftKeyH:
221,CtrlShiftKeyI:222,CtrlShiftKeyJ:223,CtrlShiftKeyK:224,CtrlShiftKeyL:225,CtrlShiftKeyM:
226,CtrlShiftKeyN:227,CtrlShiftKeyO:228,CtrlShiftKeyP:229,CtrlShiftKeyQ:230,CtrlShiftKeyR:
231,CtrlShiftKeyS:232,CtrlShiftKeyT:233,CtrlShiftKeyU:234,CtrlShiftKeyV:235,CtrlShiftKeyW:
236,CtrlShiftKeyX:237,CtrlShiftKeyY:238,CtrlShiftKeyZ:239,CtrlShiftSpace:240,CtrlShiftKey0:
241,CtrlShiftKey1:242,CtrlShiftKey2:243,CtrlShiftKey3:244,CtrlShiftKey4:245,CtrlShiftKey5:
246,CtrlShiftKey6:247,CtrlShiftKey7:248,CtrlShiftKey8:249,CtrlShiftKey9:250,CtrlShiftF1:
251,CtrlShiftF2:252,CtrlShiftF3:253,CtrlShiftF4:254,CtrlShiftF5:255,CtrlShiftF6:
256,CtrlShiftF7:257,CtrlShiftF8:258,CtrlShiftF9:259,CtrlShiftF10:260,CtrlShiftEnter:
261,CtrlShiftEscape:262,CtrlShiftUp:263,CtrlShiftDown:264,CtrlShiftLeft:265,CtrlShiftRight:
266,CtrlShiftPageUp:267,CtrlShiftPageDown:268,CtrlShiftBackspace:269,CtrlShiftInsert:
270,CtrlShiftDelete:271,CtrlShiftHome:272,CtrlShiftEnd:273,CtrlShiftTab:274,AltF1:
275,AltF2:276,AltF3:277,AltF4:278,AltF5:279,AltF6:280,AltF7:281,AltF8:282,AltF9:
283,AltF10:284,AltEnter:285,AltEscape:286,AltUp:287,AltDown:288,AltLeft:289,AltRight:
290,AltPageUp:291,AltPageDown:292,AltBackspace:293,AltInsert:294,AltDelete:295,AltHome:
296,AltEnd:297,AltTab:298,AltShiftF1:299,AltShiftF2:300,AltShiftF3:301,AltShiftF4:
302,AltShiftF5:303,AltShiftF6:304,AltShiftF7:305,AltShiftF8:306,AltShiftF9:307,AltShiftF10:
308,AltShiftEnter:309,AltShiftEscape:310,AltShiftUp:311,AltShiftDown:312,AltShiftLeft:
313,AltShiftRight:314,AltShiftPageUp:315,AltShiftPageDown:316,AltShiftBackspace:
317,AltShiftInsert:318,AltShiftDelete:319,AltShiftHome:320,AltShiftEnd:321,AltShiftTab:
322,ShiftF1:323,ShiftF2:324,ShiftF3:325,ShiftF4:326,ShiftF5:327,ShiftF6:328,ShiftF7:
329,ShiftF8:330,ShiftF9:331,ShiftF10:332,ShiftEnter:333,ShiftEscape:334,ShiftUp:
335,ShiftDown:336,ShiftLeft:337,ShiftRight:338,ShiftPageUp:339,ShiftPageDown:340
,ShiftBackspace:341,ShiftInsert:342,ShiftDelete:343,ShiftHome:344,ShiftEnd:345,ShiftTab:
346};E.Xq={XT:0x1,XQ:0x2,XR:0x4,XS:0x8,W8:0x10,W0:0x20};
E._Init=function(){E.J$.__proto__=E.AE;E.Ly.__proto__=E.AE;E.AQ.__proto__=E.AE;E.
J.__proto__=E.AQ;E.Root.__proto__=E.J;E.KeyEvent.__proto__=E.Event;E.J2.__proto__=
E.Event;E.J1.__proto__=E.Event;E.J3.__proto__=E.Event;E.Lv.__proto__=E.AQ;E.HK.__proto__=
E.J;E.G4.__proto__=E.Ly;E.OU.__proto__=E.AQ;E.Iv.__proto__=E.AQ;E.ME.__proto__=E.
J_;E.MF.__proto__=E.J_;};E.DC=function(D){};return E;})();

/* Embedded Wizard */