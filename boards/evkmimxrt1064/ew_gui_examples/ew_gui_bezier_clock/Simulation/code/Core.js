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
Core=(function(){var B=EmWiApp;var E={};
var AJ=[0,0];var Be=[0,0,0,0];var Db="The view does not belong to this group";var
CD="The dialog component is already presented";var Fk="No fader to perform the fade-in/out operation";
var Ip="Trying to use the same fader twice";var I9="Trying to fade-in/out a group which belongs to another owner";
var HE="No view to restack";var Iq="View is not in this group";var Lm="Sibling view does not belong to the group";
var MN="No view to remove";var OJ="No view to add";var OK="View already in a group";
var OL="Recursive invalidate during active update cycle.";var OM="The method RestackBehind() is not available in Core::VerticalList.";
var ON="The method RestackTop() is not available in Core::VerticalList.";var OO=
"The method Restack() is not available in Core::VerticalList.";var OP="The method Remove() is not available in Core::VerticalList.";
var OQ="The method Add() is not available in Core::VerticalList.";var OR="The Slide Touch Handler:";
var OS="is already connected with a view.";var OT="The Slide Touch Handler is already connected with a view to scroll "+
"its content.";var OU=[0,0,170,120];var Gq="The KeyPressHandler is embedded within an object not being derived "+
"from Core::Group.";var OV="Trying to cancel a task not belonging to this queue!";
var OW="Trying to enqueue a task twice!";
E.AD={U:null,X:null,G:null,AN:null,F:0x103,D1:0x14,Ku:function(Y,Km){},C9:function(
C){var A;var Bf=C^this.D1;if(!Bf)return;this.D1=C;if(!!this.AN&&!((this.F&0x400)===
0x400)){this.G.F=this.G.F|0x5000;B.kD([A=this.G,A.Cs],this);this.G.Ag([0,0,(A=this.
G.H)[2]-A[0],A[3]-A[1]]);}if(!!this.AN&&((this.F&0x400)===0x400)){this.AN.IH.F=this.
AN.IH.F|0x1000;this.G.F=this.G.F|0x4000;B.kD([A=this.G,A.Cs],this);}},El:function(
){var Ai=this.G;while(!!Ai){var RE=(E.Root.isPrototypeOf(Ai)?Ai:null);if(!!RE)return RE;
Ai=Ai.G;}return null;},C7:function(AA,aClip,aOffset,Ah,aBlend){},Cj:function(Aq){
return null;},Ge:function(AB,V,DN,FX,Fm){return null;},KF:function(Y,C0){return AJ;
},K5:function(aOffset,Kl){},GetExtent:function(){return Be;},By:function(HH,Ja){
var A;if(((this.F&0x200)===0x200))HH=HH&~0x400;var Pt=(this.F&~Ja)|HH;var Iy=Pt^
this.F;this.F=Pt;if(!!this.G&&!!(Iy&0x14)){var Rj=((this.F&0x14)===0x14);if(Rj&&
!this.G.C8)this.G.D4(this);if(!Rj&&(this.G.C8===this))this.G.D4(this.G.NU(this,0x14
));}if(!!this.G&&!!(Iy&0x403))this.G.Ag(this.GetExtent());if((!!this.AN&&((Pt&0x400
)===0x400))&&((Iy&0x1)===0x1)){this.F=this.F|0x800;this.G.F=this.G.F|0x4000;B.kD([
A=this.G,A.Cs],this);}if(!!this.G&&((Iy&0x400)===0x400)){this.AN=null;this.F=this.
F|0x800;this.G.F=this.G.F|0x4000;B.kD([A=this.G,A.Cs],this);}},_Init:function(aArg
){this.__proto__=E.AD;B.gb++;},_Done:function(){this.__proto__=E.AD;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.U)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.X)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.G)&&(A._cycle!=D)
)A._Mark(A._cycle=D);if((A=this.AN)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:
"Core::View"};E.JZ={Bn:B.pm,BI:B.pm,Ku:function(Y,Km){var Ac=B._NewObject(E.Mo,0
);this.AN=null;Ac.CM=this.getExtent();Ac.W=Y;Ac.IH=Km;Ac.HV=this.BI;Ac.HW=this.Bn;
this.AN=Ac;},KF:function(Y,C0){var A;var AC=this.D1;var Ac=(E.Mo.isPrototypeOf(A=
this.AN)?A:null);var x1=Ac.CM[0];var y1=Ac.CM[1];var x2=Ac.CM[2];var y2=Ac.CM[3];
var Ce=[Y[2]-Y[0],Y[3]-Y[1]];var Aw=x2-x1;var At=y2-y1;var tmp;if(!C0){var Df=[(
A=Ac.W)[2]-A[0],A[3]-A[1]];x1=x1-Ac.W[0];y1=y1-Ac.W[1];if(Df[0]!==Ce[0]){var Bh=((
AC&0x4)===0x4);var Bi=((AC&0x8)===0x8);var Ed=((AC&0x1)===0x1);if(!Bh&&(Ed||!Bi)
)x1=((x1*Ce[0])/Df[0])|0;if(!Bi&&(Ed||!Bh)){x2=x2-Ac.W[0];x2=((x2*Ce[0])/Df[0])|
0;x2=x2-Ce[0];}else x2=x2-Ac.W[2];x1=x1+Y[0];x2=x2+Y[2];if(!Ed){if(Bh&&!Bi)x2=x1+
Aw;else if(!Bh&&Bi)x1=x2-Aw;else{x1=x1+((((x2-x1)-Aw)/2)|0);x2=x1+Aw;}}}else{x2=
x2-Ac.W[2];x1=x1+Y[0];x2=x2+Y[2];}if(Df[1]!==Ce[1]){var Bj=((AC&0x10)===0x10);var
Bg=((AC&0x20)===0x20);var Ee=((AC&0x2)===0x2);if(!Bj&&(Ee||!Bg))y1=((y1*Ce[1])/Df[
1])|0;if(!Bg&&(Ee||!Bj)){y2=y2-Ac.W[1];y2=((y2*Ce[1])/Df[1])|0;y2=y2-Ce[1];}else
y2=y2-Ac.W[3];y1=y1+Y[1];y2=y2+Y[3];if(!Ee){if(Bj&&!Bg)y2=y1+At;else if(!Bj&&Bg)
y1=y2-At;else{y1=y1+((((y2-y1)-At)/2)|0);y2=y1+At;}}}else{y2=y2-Ac.W[3];y1=y1+Y[
1];y2=y2+Y[3];}}else{switch(C0){case 3:{x1=Y[0];x2=x1+Aw;}break;case 4:{x2=Y[2];
x1=x2-Aw;}break;case 1:{y1=Y[1];y2=y1+At;}break;case 2:{y2=Y[3];y1=y2-At;}break;
default:;}if((C0===3)||(C0===4)){var Bj=((AC&0x10)===0x10);var Bg=((AC&0x20)===0x20
);var Ee=((AC&0x2)===0x2);if(Ee){y1=Y[1];y2=Y[3];}else if(Bj&&!Bg){y1=Y[1];y2=y1+
At;}else if(Bg&&!Bj){y2=Y[3];y1=y2-At;}else{y1=Y[1]+((((Y[3]-Y[1])-At)/2)|0);y2=
y1+At;}}if((C0===1)||(C0===2)){var Bh=((AC&0x4)===0x4);var Bi=((AC&0x8)===0x8);var
Ed=((AC&0x1)===0x1);if(Ed){x1=Y[0];x2=Y[2];}else if(Bh&&!Bi){x1=Y[0];x2=x1+Aw;}else
if(Bi&&!Bh){x2=Y[2];x1=x2-Aw;}else{x1=Y[0]+((((Y[2]-Y[0])-Aw)/2)|0);x2=x1+Aw;}}}
Ac.isEmpty=(x1>=x2)||(y1>=y2);Aw=x2-x1;At=y2-y1;if(Ac.HW[0]<Ac.HV[0]){tmp=x1;x1=
x2-1;x2=tmp-1;}if(Ac.HW[1]<Ac.HV[1]){tmp=y1;y1=y2-1;y2=tmp-1;}if((x2-x1)===1)x2=
x1;if((y2-y1)===1)y2=y1;if(((this.F&0x100)===0x100)){this.BI=[x1,y1];this.Bn=[x2
,y2];}else{this.Cl([x1,y1]);this.Cm([x2,y2]);this.AN=Ac;}return[Aw,At];},K5:function(
aOffset,Kl){if(Kl){this.BI=B.so(this.BI,aOffset);this.Bn=B.so(this.Bn,aOffset);}
else{this.Cl(B.so(this.BI,aOffset));this.Cm(B.so(this.Bn,aOffset));}},GetExtent:
function(){if(!!this.AN&&this.AN.isEmpty)return Be;return this.getExtent();},getExtent:
function(){var x1=this.BI[0];var y1=this.BI[1];var x2=this.Bn[0];var y2=this.Bn[
1];var Aw=x2-x1;var At=y2-y1;var tmp;if(Aw<0)Aw=-Aw;if(At<0)At=-At;if(Aw>=At){if(
x2>x1)x2=x2-1;if(x2<x1)x2=x2+1;}if(At>=Aw){if(y2>y1)y2=y2-1;if(y2<y1)y2=y2+1;}if(
x2<x1){tmp=x1;x1=x2;x2=tmp;}if(y2<y1){tmp=y1;y1=y2;y2=tmp;}return[x1,y1,x2+1,y2+
1];},Cm:function(C){var A;if(B.sa(C,this.Bn))return;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ag(this.GetExtent());this.AN=null;this.Bn=C;if(!!this.G&&((this.F&0x1
)===0x1))this.G.Ag(this.GetExtent());if((!!this.G&&((this.F&0x400)===0x400))&&!((
this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=this.G.F|0x4000;B.kD([A=
this.G,A.Cs],this);}},Cl:function(C){var A;if(B.sa(C,this.BI))return;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ag(this.GetExtent());this.AN=null;this.BI=C;if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.GetExtent());if((!!this.G&&((this.F&0x400
)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=this.G.
F|0x4000;B.kD([A=this.G,A.Cs],this);}},_Init:function(aArg){E.AD._Init.call(this
,aArg);this.__proto__=E.JZ;},_className:"Core::LineView"};E.Lf={CI:B.pm,DK:B.pm,
Bn:B.pm,BI:B.pm,Ku:function(Y,Km){var Ac=B._NewObject(E.Mp,0);this.AN=null;Ac.CM=
this.GetExtent();Ac.W=Y;Ac.IH=Km;Ac.HV=this.BI;Ac.HW=this.Bn;Ac.LT=this.DK;Ac.LU=
this.CI;this.AN=Ac;},KF:function(Y,C0){var A;var AC=this.D1;var Ac=(E.Mp.isPrototypeOf(
A=this.AN)?A:null);var x1=Ac.CM[0];var y1=Ac.CM[1];var x2=Ac.CM[2];var y2=Ac.CM[
3];var Ce=[Y[2]-Y[0],Y[3]-Y[1]];var Aw=x2-x1;var At=y2-y1;if(!C0){var Df=[(A=Ac.
W)[2]-A[0],A[3]-A[1]];x1=x1-Ac.W[0];y1=y1-Ac.W[1];if(Df[0]!==Ce[0]){var Bh=((AC&
0x4)===0x4);var Bi=((AC&0x8)===0x8);var Ed=((AC&0x1)===0x1);if(!Bh&&(Ed||!Bi))x1=((
x1*Ce[0])/Df[0])|0;if(!Bi&&(Ed||!Bh)){x2=x2-Ac.W[0];x2=((x2*Ce[0])/Df[0])|0;x2=x2-
Ce[0];}else x2=x2-Ac.W[2];x1=x1+Y[0];x2=x2+Y[2];if(!Ed){if(Bh&&!Bi)x2=x1+Aw;else
if(!Bh&&Bi)x1=x2-Aw;else{x1=x1+((((x2-x1)-Aw)/2)|0);x2=x1+Aw;}}}else{x2=x2-Ac.W[
2];x1=x1+Y[0];x2=x2+Y[2];}if(Df[1]!==Ce[1]){var Bj=((AC&0x10)===0x10);var Bg=((AC&
0x20)===0x20);var Ee=((AC&0x2)===0x2);if(!Bj&&(Ee||!Bg))y1=((y1*Ce[1])/Df[1])|0;
if(!Bg&&(Ee||!Bj)){y2=y2-Ac.W[1];y2=((y2*Ce[1])/Df[1])|0;y2=y2-Ce[1];}else y2=y2-
Ac.W[3];y1=y1+Y[1];y2=y2+Y[3];if(!Ee){if(Bj&&!Bg)y2=y1+At;else if(!Bj&&Bg)y1=y2-
At;else{y1=y1+((((y2-y1)-At)/2)|0);y2=y1+At;}}}else{y2=y2-Ac.W[3];y1=y1+Y[1];y2=
y2+Y[3];}}else{switch(C0){case 3:{x1=Y[0];x2=x1+Aw;}break;case 4:{x2=Y[2];x1=x2-
Aw;}break;case 1:{y1=Y[1];y2=y1+At;}break;case 2:{y2=Y[3];y1=y2-At;}break;default:;
}if((C0===3)||(C0===4)){var Bj=((AC&0x10)===0x10);var Bg=((AC&0x20)===0x20);var Ee=((
AC&0x2)===0x2);if(Ee){y1=Y[1];y2=Y[3];}else if(Bj&&!Bg){y1=Y[1];y2=y1+At;}else if(
Bg&&!Bj){y2=Y[3];y1=y2-At;}else{y1=Y[1]+((((Y[3]-Y[1])-At)/2)|0);y2=y1+At;}}if((
C0===1)||(C0===2)){var Bh=((AC&0x4)===0x4);var Bi=((AC&0x8)===0x8);var Ed=((AC&0x1
)===0x1);if(Ed){x1=Y[0];x2=Y[2];}else if(Bh&&!Bi){x1=Y[0];x2=x1+Aw;}else if(Bi&&
!Bh){x2=Y[2];x1=x2-Aw;}else{x1=Y[0]+((((Y[2]-Y[0])-Aw)/2)|0);x2=x1+Aw;}}}Ac.isEmpty=(
x1>=x2)||(y1>=y2);Aw=(x2-x1)-1;At=(y2-y1)-1;var HL=Ac.CM[0];var HM=Ac.CM[1];var Hb=(
Ac.CM[2]-HL)-1;var Ha=(Ac.CM[3]-HM)-1;if(!Hb)Hb=1;if(!Ha)Ha=1;if(((this.F&0x100)===
0x100)){this.BI=[x1+((((Ac.HV[0]-HL)*Aw)/Hb)|0),y1+((((Ac.HV[1]-HM)*At)/Ha)|0)];
this.Bn=[x1+((((Ac.HW[0]-HL)*Aw)/Hb)|0),y1+((((Ac.HW[1]-HM)*At)/Ha)|0)];this.DK=[
x1+((((Ac.LT[0]-HL)*Aw)/Hb)|0),y1+((((Ac.LT[1]-HM)*At)/Ha)|0)];this.CI=[x1+((((Ac.
LU[0]-HL)*Aw)/Hb)|0),y1+((((Ac.LU[1]-HM)*At)/Ha)|0)];}else{this.Cl([x1+((((Ac.HV[
0]-HL)*Aw)/Hb)|0),y1+((((Ac.HV[1]-HM)*At)/Ha)|0)]);this.Cm([x1+((((Ac.HW[0]-HL)*
Aw)/Hb)|0),y1+((((Ac.HW[1]-HM)*At)/Ha)|0)]);this.GR([x1+((((Ac.LT[0]-HL)*Aw)/Hb)|
0),y1+((((Ac.LT[1]-HM)*At)/Ha)|0)]);this.GS([x1+((((Ac.LU[0]-HL)*Aw)/Hb)|0),y1+((((
Ac.LU[1]-HM)*At)/Ha)|0)]);this.AN=Ac;}return[Aw+1,At+1];},K5:function(aOffset,Kl
){if(Kl){this.BI=B.so(this.BI,aOffset);this.Bn=B.so(this.Bn,aOffset);this.DK=B.so(
this.DK,aOffset);this.CI=B.so(this.CI,aOffset);}else{this.Cl(B.so(this.BI,aOffset
));this.Cm(B.so(this.Bn,aOffset));this.GR(B.so(this.DK,aOffset));this.GS(B.so(this.
CI,aOffset));}},GetExtent:function(){if(!!this.AN&&this.AN.isEmpty)return Be;var
x1=this.BI[0];var y1=this.BI[1];var x2=this.DK[0];var y2=this.DK[1];if((((this.CI[
0]!==x1)||(this.Bn[1]!==y1))||(this.Bn[0]!==x2))||(this.CI[1]!==y2)){if(this.Bn[
0]<x1)x1=this.Bn[0];if(this.DK[0]<x1)x1=this.DK[0];if(this.CI[0]<x1)x1=this.CI[0
];if(this.Bn[1]<y1)y1=this.Bn[1];if(this.DK[1]<y1)y1=this.DK[1];if(this.CI[1]<y1
)y1=this.CI[1];if(this.BI[0]>x2)x2=this.BI[0];if(this.Bn[0]>x2)x2=this.Bn[0];if(
this.CI[0]>x2)x2=this.CI[0];if(this.BI[1]>y2)y2=this.BI[1];if(this.Bn[1]>y2)y2=this.
Bn[1];if(this.CI[1]>y2)y2=this.CI[1];}else{var tmp;if(x2<x1){tmp=x1;x1=x2;x2=tmp;
}if(y2<y1){tmp=y1;y1=y2;y2=tmp;}}return[x1,y1,x2+1,y2+1];},GS:function(C){var A;
if(B.sa(C,this.CI))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.GetExtent(
));this.AN=null;this.CI=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.GetExtent(
));if((!!this.G&&((this.F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.G.F=this.G.F|0x4000;B.kD([A=this.G,A.Cs],this);}},GR:function(
C){var A;if(B.sa(C,this.DK))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.
GetExtent());this.AN=null;this.DK=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(
this.GetExtent());if((!!this.G&&((this.F&0x400)===0x400))&&!((this.G.F&0x2000)===
0x2000)){this.F=this.F|0x800;this.G.F=this.G.F|0x4000;B.kD([A=this.G,A.Cs],this);
}},Cm:function(C){var A;if(B.sa(C,this.Bn))return;if(!!this.G&&((this.F&0x1)===0x1
))this.G.Ag(this.GetExtent());this.AN=null;this.Bn=C;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ag(this.GetExtent());if((!!this.G&&((this.F&0x400)===0x400))&&!((this.
G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=this.G.F|0x4000;B.kD([A=this.
G,A.Cs],this);}},Cl:function(C){var A;if(B.sa(C,this.BI))return;if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ag(this.GetExtent());this.AN=null;this.BI=C;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ag(this.GetExtent());if((!!this.G&&((this.F&0x400)===0x400
))&&!((this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=this.G.F|0x4000;
B.kD([A=this.G,A.Cs],this);}},N5:function(Ep){var Br=B.sr(4,B.pm,null);var I=0;var
Bx=3;var Pd=false;var Pe=false;Br.Set(0,this.BI);Br.Set(1,this.Bn);Br.Set(2,this.
DK);Br.Set(3,this.CI);while(I<4){var RX=Br.Get(I)[0];var NC=Br.Get(I)[1];var VV=
Br.Get(Bx)[0];var PO=Br.Get(Bx)[1];if(((NC>Ep[1])!==(PO>Ep[1]))||((NC<Ep[1])!==(
PO<Ep[1]))){var JN=((((VV-RX)*(Ep[1]-NC))/(PO-NC))|0)+RX;if(Ep[0]>JN)Pd=!Pd;if(Ep[
0]<JN)Pe=!Pe;}Bx=I;I=I+1;}return Pd||Pe;},Sz:function(){return((((this.BI[0]===this.
CI[0])&&(this.Bn[0]===this.DK[0]))&&(this.BI[1]===this.Bn[1]))&&(this.DK[1]===this.
CI[1]))||((((this.BI[0]===this.Bn[0])&&(this.DK[0]===this.CI[0]))&&(this.BI[1]===
this.CI[1]))&&(this.Bn[1]===this.DK[1]));},_Init:function(aArg){E.AD._Init.call(
this,aArg);this.__proto__=E.Lf;},_className:"Core::QuadView"};E.AY={H:B.pn,Ku:function(
Y,Km){var Ac=B._NewObject(E.IX,0);Ac.CM=this.H;Ac.W=Y;Ac.IH=Km;this.AN=Ac;},KF:function(
Y,C0){var A;var AC=this.D1;var Ac=this.AN;var x1=Ac.CM[0];var y1=Ac.CM[1];var x2=
Ac.CM[2];var y2=Ac.CM[3];var Ce=[Y[2]-Y[0],Y[3]-Y[1]];var Aw=x2-x1;var At=y2-y1;
if(!C0){var Df=[(A=Ac.W)[2]-A[0],A[3]-A[1]];x1=x1-Ac.W[0];y1=y1-Ac.W[1];if(Df[0]
!==Ce[0]){var Bh=((AC&0x4)===0x4);var Bi=((AC&0x8)===0x8);var Ed=((AC&0x1)===0x1
);if(!Bh&&(Ed||!Bi))x1=((x1*Ce[0])/Df[0])|0;if(!Bi&&(Ed||!Bh)){x2=x2-Ac.W[0];x2=((
x2*Ce[0])/Df[0])|0;x2=x2-Ce[0];}else x2=x2-Ac.W[2];x1=x1+Y[0];x2=x2+Y[2];if(!Ed){
if(Bh&&!Bi)x2=x1+Aw;else if(!Bh&&Bi)x1=x2-Aw;else{x1=x1+((((x2-x1)-Aw)/2)|0);x2=
x1+Aw;}}}else{x2=x2-Ac.W[2];x1=x1+Y[0];x2=x2+Y[2];}if(Df[1]!==Ce[1]){var Bj=((AC&
0x10)===0x10);var Bg=((AC&0x20)===0x20);var Ee=((AC&0x2)===0x2);if(!Bj&&(Ee||!Bg
))y1=((y1*Ce[1])/Df[1])|0;if(!Bg&&(Ee||!Bj)){y2=y2-Ac.W[1];y2=((y2*Ce[1])/Df[1])|
0;y2=y2-Ce[1];}else y2=y2-Ac.W[3];y1=y1+Y[1];y2=y2+Y[3];if(!Ee){if(Bj&&!Bg)y2=y1+
At;else if(!Bj&&Bg)y1=y2-At;else{y1=y1+((((y2-y1)-At)/2)|0);y2=y1+At;}}}else{y2=
y2-Ac.W[3];y1=y1+Y[1];y2=y2+Y[3];}}else{switch(C0){case 3:{x1=Y[0];x2=x1+Aw;}break;
case 4:{x2=Y[2];x1=x2-Aw;}break;case 1:{y1=Y[1];y2=y1+At;}break;case 2:{y2=Y[3];
y1=y2-At;}break;default:;}if((C0===3)||(C0===4)){var Bj=((AC&0x10)===0x10);var Bg=((
AC&0x20)===0x20);var Ee=((AC&0x2)===0x2);if(Ee){y1=Y[1];y2=Y[3];}else if(Bj&&!Bg
){y1=Y[1];y2=y1+At;}else if(Bg&&!Bj){y2=Y[3];y1=y2-At;}else{y1=Y[1]+((((Y[3]-Y[1
])-At)/2)|0);y2=y1+At;}}if((C0===1)||(C0===2)){var Bh=((AC&0x4)===0x4);var Bi=((
AC&0x8)===0x8);var Ed=((AC&0x1)===0x1);if(Ed){x1=Y[0];x2=Y[2];}else if(Bh&&!Bi){
x1=Y[0];x2=x1+Aw;}else if(Bi&&!Bh){x2=Y[2];x1=x2-Aw;}else{x1=Y[0]+((((Y[2]-Y[0])-
Aw)/2)|0);x2=x1+Aw;}}}Ac.isEmpty=(x1>=x2)||(y1>=y2);if(((this.F&0x100)===0x100)){
this.H=[x1,y1,x2,y2];}else{this.Q([x1,y1,x2,y2]);this.AN=Ac;}return[x2-x1,y2-y1];
},K5:function(aOffset,Kl){if(Kl)this.H=B.sq(this.H,aOffset);else this.Q(B.sq(this.
H,aOffset));},GetExtent:function(){return this.H;},Q:function(C){var A;if(B.sb(C
,this.H))return;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);this.AN=null;
this.H=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);if((!!this.G&&((this.
F&0x400)===0x400))&&!((this.G.F&0x2000)===0x2000)){this.F=this.F|0x800;this.G.F=
this.G.F|0x4000;B.kD([A=this.G,A.Cs],this);}},_Init:function(aArg){E.AD._Init.call(
this,aArg);this.__proto__=E.AY;},_className:"Core::RectView"};E.K={Am:null,Az:null
,Nf:null,AT:null,Cd:null,Ft:null,JH:null,C8:null,C_:255,C7:function(AA,aClip,aOffset
,Ah,aBlend){var A;Ah=((Ah+1)*this.C_)>>8;aBlend=aBlend&&((this.F&0x2)===0x2);if(
!this.AT)this.M6(AA,aClip,B.so(aOffset,this.H.slice(0,2)),Ah,aBlend);else{var B8=
255|(255<<8)|(255<<16)|((Ah&0xFF)<<24);this.AT.Update();AA.L6(aClip,this.AT,0,B.
sq(this.H,aOffset),AJ,B8,B8,B8,B8,aBlend);}},Ge:function(AB,V,DN,FX,Fm){var A;var
L=this.Az;var Jo=null;var S=Be;var DA=null;var Pq=!!this.Ft&&(!!this.Ft.Eq||!!this.
Ft.Am);if(((A=B.hV(AB,this.H))[0]>=A[2])||(A[1]>=A[3]))return null;AB=B.sp(AB,this.
H.slice(0,2));while(!!L){if(((L.F&0x400)===0x400)&&!DA){DA=L.X;while(!!DA&&!((DA.
F&0x200)===0x200))DA=DA.X;if(!!DA)S=B.hV(AB,DA.GetExtent());else S=Be;}if(DA===L
){DA=null;S=Be;}if((((((L.F&0x8)===0x8)&&((L.F&0x10)===0x10))&&!((L.F&0x40000)===
0x40000))&&!((L.F&0x20000)===0x20000))&&(!((L.F&0x10000)===0x10000)||((this.Cd.Bw===
L)&&!Pq))){var CM=L.GetExtent();var M5=FX;var Jh=null;if(M5===L)M5=null;if(((L.F&
0x400)===0x400)){if(!(((A=B.hV(CM,S))[0]>=A[2])||(A[1]>=A[3])))Jh=L.Ge(S,V,DN,M5
,Fm);}else{if(!(((A=B.hV(CM,AB))[0]>=A[2])||(A[1]>=A[3]))||(FX===L))Jh=L.Ge(AB,V
,DN,M5,Fm);}L=L.X;if(!!Jh){if(!Jo||((Jh.KN<Jo.KN)&&(Jh.KN>=0)))Jo=Jh;if(!Jh.KN)L=
null;}}else L=L.X;}return Jo;},By:function(HH,Ja){var A;var Vo=this.F;E.AY.By.call(
this,HH,Ja);var Iy=this.F^Vo;if(!!this.C8&&((Iy&0x40)===0x40)){if(((this.F&0x40)===
0x40))this.C8.By(0x40,0x0);else this.C8.By(0x0,0x40);}if(!!this.Cd&&((Iy&0x40)===
0x40)){if(((this.F&0x40)===0x40)&&((this.Cd.Bw.F&0x14)===0x14))this.Cd.Bw.By(0x40
,0x0);else this.Cd.Bw.By(0x0,0x40);}if(!!Iy){this.F=this.F|0x8000;B.kD([this,this.
Cs],this);}},Q:function(C){var A;if(B.sb(C,this.H))return;var IG=[(A=this.H)[2]-
A[0],A[3]-A[1]];var Nj=[C[2]-C[0],C[3]-C[1]];var JK=!B.sa(IG,Nj);if(JK&&!!this.AT
){this.AT.K_(Nj);B.pl(this,0);B.pl(this.AT,0);}E.AY.Q.call(this,C);if((JK&&(IG[0
]>0))&&(IG[1]>0)){var W=[].concat(AJ,IG);var L=this.Am;var LB=0x14;while(!!L){if((
!L.AN&&(L.D1!==LB))&&!((L.F&0x400)===0x400))L.Ku(W,null);L=L.U;}}if(JK){this.F=this.
F|0x5000;B.kD([this,this.Cs],this);}},PE:function(Aq){var Ro=(E.KeyEvent.isPrototypeOf(
Aq)?Aq:null);var F0=this.Nf;if(!Ro)return null;while(!!F0&&(!F0.B0||!F0.Cj(Ro)))
F0=F0.U;return F0;},VR:function(Ae){if(!!this.AT){var LD=false;if(!LD)this.AT.Mb(
this.AT.EP,this.AT.EP,0x00000000,0x00000000,0x00000000,0x00000000,false);this.M6(
this.AT,this.AT.EP,AJ,255,true);}},M6:function(AA,aClip,aOffset,Ah,aBlend){var A;
var L=this.Am;var Rc=Be;var Ry=true;while(!!L){if(((L.F&0x200)===0x200)){var Rx=(
E.Lb.isPrototypeOf(L)?L:null);Rc=B.hV(aClip,B.sq(Rx.H,aOffset));Ry=((Rx.F&0x1)===
0x1);}if(((L.F&0x1)===0x1)){if(((L.F&0x400)===0x400)){if(Ry){var S=B.hV(B.sq(L.GetExtent(
),aOffset),Rc);if(!((S[0]>=S[2])||(S[1]>=S[3])))L.C7(AA,S,aOffset,Ah,aBlend);}}else{
var S=B.hV(B.sq(L.GetExtent(),aOffset),aClip);if(!((S[0]>=S[2])||(S[1]>=S[3])))L.
C7(AA,S,aOffset,Ah,aBlend);}}L=L.U;}},VE:function(){var A;var Pb=((this.F&0x1000
)===0x1000);var HP=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];var Hc=false;var Jn=Be;var
M8=AJ;var LC=0;var M7=0;var L=this.Az;var DA=null;var LB=0x14;while(!!L){if(((L.
F&0x800)===0x800)){Hc=true;L.F=L.F&~0x800;}if(Hc&&((L.F&0x200)===0x200)){Hc=false;
if(!!(E.Lb.isPrototypeOf(L)?L:null).Mc)L.F=L.F|0x1000;}L=L.X;}Hc=false;L=this.Am;
if(Pb){this.F=this.F&~0x1000;Pb=!((HP[0]>=HP[2])||(HP[1]>=HP[3]));}this.F=this.F|
0x2000;while(!!L){if(((L.F&0x400)===0x400)){if(!!L.AN&&(L.AN.IH!==DA))L.AN=null;
if((!L.AN&&Hc)&&((L.D1!==LB)||!!M7))L.Ku(Jn,DA);}if(!!L.AN){if(Pb&&!((L.F&0x400)===
0x400))L.KF(HP,0);if(Hc&&((L.F&0x400)===0x400)){var size=L.KF(B.sq(Jn,M8),M7);if(((
L.F&0x1)===0x1)){var BH=AJ;switch(M7){case 3:BH=[size[0]+LC,BH[1]];break;case 4:
BH=[-size[0]-LC,BH[1]];break;case 1:BH=[BH[0],size[1]+LC];break;case 2:BH=[BH[0]
,-size[1]-LC];break;default:;}M8=B.so(M8,BH);}}}if(((L.F&0x200)===0x200)){Hc=((L.
F&0x1000)===0x1000);DA=(E.Lb.isPrototypeOf(L)?L:null);if(Hc){L.F=L.F&~0x1000;Jn=
B.sq(DA.H,DA.AU);M8=AJ;M7=DA.Mc;LC=DA.Space;Hc=!((Jn[0]>=Jn[2])||(Jn[1]>=Jn[3]));
}if(Hc){this.Ag(DA.H);}}L=L.U;}this.F=this.F&~0x2000;this.G4([HP[2]-HP[0],HP[3]-
HP[1]]);},Cs:function(Ae){var A;var VS=((this.F&0x1000)===0x1000);if(((this.F&0x4000
)===0x4000)){this.F=this.F&~0x4000;this.VE();}if(((this.F&0x8000)===0x8000)||VS){
this.F=this.F&~0x8000;this.DM(this.F);}},D4:function(C){var A;if(!!C&&(C.G!==this
))throw new Error(Db);if(!!C&&!((C.F&0x14)===0x14))C=null;if(!!C&&((C.F&0x10000)===
0x10000))C=null;if(C===this.C8)return;if(!!this.C8)this.C8.By(0x0,0x60);this.C8=
C;if(!!C){if(((this.F&0x40)===0x40))C.By(0x60,0x0);else C.By(0x20,0x0);}},S8:function(
){return!!this.AT;},K9:function(C){var A;if(!!this.AT===C)return;if(C){this.AT=B.
_NewObject(B.Graphics.Canvas,0);this.AT.K_([(A=this.H)[2]-A[0],A[3]-A[1]]);this.
AT.Mq=[this,this.VR];}else{this.AT.Mq=null;this.AT.K_(AJ);this.AT=null;}if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.H);B.pl(this,0);},GQ:function(C){var A;if(
C>255)C=255;if(C<0)C=0;if(C===this.C_)return;this.C_=C;if(!!this.G&&((this.F&0x1
)===0x1))this.G.Ag(this.H);},UZ:function(VU){this.GQ(VU);},J1:function(){var A;return((
this.F&0x1)===0x1);},Hq:function(C){if(C)this.By(0x1,0x0);else this.By(0x0,0x1);
},Hn:function(Q6){var A;return(((this.F&0x10000)===0x10000)&&!!this.G)&&(!Q6||this.
G.Hn(true));},EU:function(Dc,EF,HF,Gu,Gv,Q4,HG,Jb,Gt,Gr,Gs){var A;if(!this.Cd){this.
QL(Dc,EF,HF,Gu,Gv,null,null,Gt,Gr,Gs);return;}var IF=this.Cd;var Jv=IF.U;if(((Dc.
F&0x10000)===0x10000)&&(this.Cd.Bw!==Dc))throw new Error(CD);var GF=B._NewObject(
E.L9,0);var G$=IF.G$;var HY=null;var Ev=null;if(!!Jv){HY=Jv.HY;Ev=Jv.Ev;}if(!!Jv&&
!!IF.LR)HY=IF.LR;if(!!Jv&&!!HG)Ev=HG;if(!!Q4)G$=Q4;if(!EF)EF=B._GetAutoObject(B.
tg.MF);if(!HF)HF=EF;if(!Gv)Gv=Gu;if(!Gu)Gu=Gv;GF.G$=HF;GF.Ev=Gu;GF.HY=Gv;GF.LR=Jb;
GF.Bw=Dc;GF.U=this.Cd.U;this.Cd.U=null;this.Cd=GF;if(this.C8===Dc)this.D4(null);
IF.Bw.By(0x0,0x10040);if(((this.F&0x40)===0x40)&&((Dc.F&0x14)===0x14))Dc.By(0x10040
,0x0);else Dc.By(0x10000,0x0);if(!!Ev){this.FI(Jv.Bw,Ev.KJ(),null,null,Gs);this.
FI(IF.Bw,G$.JQ(),null,null,true);this.FI(GF.Bw,EF.Hi(),Gt,Gr,true);}else if(!!HY
){this.FI(Jv.Bw,HY.KK(),null,null,Gs);this.FI(IF.Bw,G$.JQ(),null,null,true);this.
FI(GF.Bw,EF.Hi(),Gt,Gr,true);}else if(!!G$){this.FI(IF.Bw,G$.JQ(),null,null,Gs);
this.FI(GF.Bw,EF.Hi(),Gt,Gr,true);}else this.FI(GF.Bw,EF.Hi(),Gt,Gr,Gs);},QL:function(
Dc,EF,HF,Gu,Gv,HG,Jb,Gt,Gr,Gs){var A;if(!Dc)return;if(!!this.Cd&&(this.Cd.Bw===Dc
)){this.EU(Dc,EF,HF,Gu,Gv,null,HG,Jb,Gt,Gr,Gs);return;}if(((Dc.F&0x10000)===0x10000
))throw new Error(CD);var Jj=B._NewObject(E.L9,0);if(!!this.Cd&&!this.Cd.Ev){if(
!Jb)Jb=HG;if(!HG)HG=Jb;}var Ev=null;if(!!this.Cd)Ev=this.Cd.Ev;if(!!this.Cd&&!!HG
)Ev=HG;if(!EF)EF=B._GetAutoObject(B.tg.MF);if(!HF)HF=EF;if(!Gv)Gv=Gu;if(!Gu)Gu=Gv;
Jj.G$=HF;Jj.Ev=Gu;Jj.HY=Gv;Jj.LR=Jb;if(this.C8===Dc)this.D4(null);if(!!this.Cd)this.
Cd.Bw.By(0x0,0x40);if(((this.F&0x40)===0x40)&&((Dc.F&0x14)===0x14))Dc.By(0x10040
,0x0);else Dc.By(0x10000,0x0);Jj.Bw=Dc;Jj.U=this.Cd;this.Cd=Jj;if(!!Ev){this.FI(
this.Cd.U.Bw,Ev.KJ(),null,null,Gs);this.FI(Dc,EF.Hi(),Gt,Gr,true);}else this.FI(
Dc,EF.Hi(),Gt,Gr,Gs);},Of:function(Ep){var tmp=this;while(!!tmp){Ep=B.sn(Ep,tmp.
H.slice(0,2));tmp=tmp.G;}return Ep;},DispatchEvent:function(Aq){var A;var L=this.
C8;var Ai=(E.K.isPrototypeOf(L)?L:null);var A6=null;var Pq=!!this.Ft&&(!!this.Ft.
Eq||!!this.Ft.Am);if(!!L&&((((L.F&0x10000)===0x10000)||((L.F&0x40000)===0x40000)
)||((L.F&0x20000)===0x20000))){L=null;Ai=null;}if(!!this.Cd&&!Pq)A6=this.Cd.Bw.DispatchEvent(
Aq);if(!A6&&!!Ai)A6=Ai.DispatchEvent(Aq);else if(!A6&&!!L)A6=L.Cj(Aq);if(!A6)A6=
this.Cj(Aq);if(!A6)A6=this.PE(Aq);return A6;},BroadcastEventAtPosition:function(
Aq,Q5,aFilter){var A;var L=this.Az;var A6=null;while(!!L&&!A6){if((!aFilter||((A=
aFilter)&&((L.F&A)===A)))&&B.se(L.GetExtent(),Q5)){var Ai=(E.K.isPrototypeOf(L)?
L:null);if(!!Ai)A6=Ai.BroadcastEventAtPosition(Aq,B.sn(Q5,Ai.H.slice(0,2)),aFilter
);else A6=L.Cj(Aq);}L=L.X;}if(!A6)A6=this.Cj(Aq);return A6;},BroadcastEvent:function(
Aq,aFilter){var A;var L=this.Az;var A6=null;while(!!L&&!A6){if(!aFilter||((A=aFilter
)&&((L.F&A)===A))){var Ai=(E.K.isPrototypeOf(L)?L:null);if(!!Ai)A6=Ai.BroadcastEvent(
Aq,aFilter);else A6=L.Cj(Aq);}L=L.X;}if(!A6)A6=this.Cj(Aq);if(!A6)A6=this.PE(Aq);
return A6;},G4:function(aSize){},DM:function(B7){},Bl:function(){this.F=this.F|0x8000;
B.kD([this,this.Cs],this);},Ag:function(AB){var A;var Ai=this;while(!!Ai&&!((AB[
0]>=AB[2])||(AB[1]>=AB[3]))){var Kn=Ai.AT;if(!Ai.G&&(Ai!==this)){Ai.Ag(AB);return;
}if(!!Kn){if(((A=Kn.EP)[0]>=A[2])||(A[1]>=A[3])){B.pl(Ai,0);B.pl(Kn,0);}var LD=false;
if(LD)Kn.EP=[0,0,(A=Ai.H)[2]-A[0],A[3]-A[1]];else Kn.EP=B.pA(Kn.EP,AB);}if(!((Ai.
F&0x1)===0x1))return;AB=B.hV(B.sq(AB,Ai.H.slice(0,2)),Ai.H);Ai=Ai.G;}},CU:function(
aArg){this.Bl();},NU:function(J,aFilter){var A;if(!J||(J.G!==this))return null;var
ID=J.U;var IJ=J.X;var LN=0x10000;if(((aFilter&0x10000)===0x10000))LN=0x0;while(!
!ID||!!IJ){if((!!ID&&(!aFilter||((A=aFilter)&&((ID.F&A)===A))))&&(!LN||!((A=LN)&&((
ID.F&A)===A))))return ID;if((!!IJ&&(!aFilter||((A=aFilter)&&((IJ.F&A)===A))))&&(
!LN||!((A=LN)&&((IJ.F&A)===A))))return IJ;if(!!ID)ID=ID.U;if(!!IJ)IJ=IJ.X;}return null;
},FI:function(G5,Ba,Gt,Gr,Gs){var A;if(!G5)return;if(!Ba)throw new Error(Fk);if((
!!Ba.K||!!Ba.G)||!!Ba.E6)throw new Error(Ip);if(!!G5.G&&(G5.G!==this))throw new Error(
I9);if(!this.Ft)this.Ft=B._NewObject(E.OG,0);Ba.G=this;Ba.K=G5;Ba.Jz=Gr;Ba.Pu=Gt;
if(!!G5.JH)G5.JH.E6.TJ(G5.JH);G5.JH=Ba;G5.F=G5.F|0x20000;if((Gs&&!!this.Ft.Az)&&
!this.Ft.Az.N3())(B.tg.Ma.isPrototypeOf(A=this.Ft.Az)?A:null).PP(Ba);else{var E6=
B._NewObject(B.tg.Ma,0);E6.PP(Ba);this.Ft.TQ(E6,false);}},MD:function(J,Fn){var A;
if(!J)throw new Error(HE);if(J.G!==this)throw new Error(Iq);if(!!Fn&&(Fn.G!==this
))throw new Error(Lm);if(J.U===Fn)return;if(((J.F&0x401)===0x401)){if(!!J.X&&!!J.
AN)J.X.F=J.X.F|0x800;J.F=J.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cs],this
);}if(((J.F&0x200)===0x200)){if(!!J.X)J.X.F=J.X.F|0x800;J.F=J.F|0x800;this.F=this.
F|0x4000;B.kD([this,this.Cs],this);}if(!!J.U)J.U.X=J.X;else this.Az=J.X;if(!!J.X
)J.X.U=J.U;else this.Am=J.U;if(!Fn){if(!!this.Az)this.Az.U=J;J.X=this.Az;J.U=null;
this.Az=J;}else{J.U=Fn;J.X=Fn.X;Fn.X=J;if(!!J.X)J.X.U=J;}if(this.Am===Fn)this.Am=
J;if(((J.F&0x1)===0x1))this.Ag(J.GetExtent());},Gl:function(J){var A;if(!J)throw new
Error(HE);if(J.G!==this)throw new Error(Iq);if(!J.U)return;if(((J.F&0x401)===0x401
)){if(!!J.X&&!!J.AN)J.X.F=J.X.F|0x800;J.F=J.F|0x800;this.F=this.F|0x4000;B.kD([this
,this.Cs],this);}if(((J.F&0x200)===0x200)){if(!!J.X)J.X.F=J.X.F|0x800;this.F=this.
F|0x4000;B.kD([this,this.Cs],this);}if(!!J.X)J.X.U=J.U;else this.Am=J.U;J.U.X=J.
X;J.X=this.Az;J.U=null;this.Az.U=J;this.Az=J;if(((J.F&0x1)===0x1))this.Ag(J.GetExtent(
));},MC:function(J,C1){var A;if(!J)throw new Error(HE);if(J.G!==this)throw new Error(
Iq);var Iu=J;var Iw=J;while((C1>0)&&!!Iu.U){Iu=Iu.U;C1=C1-1;}while((C1<0)&&!!Iw.
X){Iw=Iw.X;C1=C1+1;}if((Iu===J)&&(Iw===J))return;if(((J.F&0x401)===0x401)){if(!!
J.X&&!!J.AN)J.X.F=J.X.F|0x800;J.F=J.F|0x800;this.F=this.F|0x4000;B.kD([this,this.
Cs],this);}if(((J.F&0x200)===0x200)){if(!!J.X)J.X.F=J.X.F|0x800;J.F=J.F|0x800;this.
F=this.F|0x4000;B.kD([this,this.Cs],this);}if(!!J.X)J.X.U=J.U;if(!!J.U)J.U.X=J.X;
if(this.Am===J)this.Am=J.U;if(this.Az===J)this.Az=J.X;if(Iu!==J){J.U=Iu.U;J.X=Iu;
Iu.U=J;if(!!J.U)J.U.X=J;}if(Iw!==J){J.U=Iw;J.X=Iw.X;Iw.X=J;if(!!J.X)J.X.U=J;}if(
!J.U)this.Az=J;if(!J.X)this.Am=J;if(((J.F&0x1)===0x1))this.Ag(J.GetExtent());},GV:
function(J){var A;if(!J)throw new Error(MN);if(J.G!==this)throw new Error(Iq);if((((
J.F&0x401)===0x401)&&!!J.X)&&!!J.AN){J.X.F=J.X.F|0x800;this.F=this.F|0x4000;B.kD([
this,this.Cs],this);}if(((J.F&0x200)===0x200)){if(!!J.X)J.X.F=J.X.F|0x800;this.F=
this.F|0x4000;B.kD([this,this.Cs],this);}J.AN=null;if(this.C8===J)this.D4(this.NU(
J,0x14));if(!!J.X)J.X.U=J.U;if(!!J.U)J.U.X=J.X;if(this.Am===J)this.Am=J.U;if(this.
Az===J)this.Az=J.X;J.G=null;J.U=null;J.X=null;if(((J.F&0x1)===0x1))this.Ag(J.GetExtent(
));},Aa:function(J,C1){var A;if(!J)throw new Error(OJ);if(!!J.G)throw new Error(
OK);J.G=this;J.X=this.Az;if(!!this.Az)this.Az.U=J;this.Az=J;if(!this.Am)this.Am=
J;if(!!C1)this.MC(J,C1);if(((J.F&0x1)===0x1))this.Ag(J.GetExtent());if(((!this.C8&&((
J.F&0x4)===0x4))&&((J.F&0x10)===0x10))&&!((J.F&0x10000)===0x10000))this.D4(J);if(((
J.F&0x401)===0x401)){J.F=J.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cs],this
);}if(((J.F&0x200)===0x200)){J.F=J.F|0x800;this.F=this.F|0x4000;B.kD([this,this.
Cs],this);}},Tb:function(){return this.C_;},_Init:function(aArg){E.AY._Init.call(
this,aArg);this.__proto__=E.K;this.F=0x1F;this.CU(aArg);},_Mark:function(D){var A;
E.AY._Mark.call(this,D);if((A=this.Am)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Az)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Nf)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.AT)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cd)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.Ft)&&(A._cycle!=D))A._Mark(A._cycle=D);
if((A=this.JH)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.C8)&&(A._cycle!=D))
A._Mark(A._cycle=D);},_className:"Core::Group"};E.Root={Ea:null,HR:null,Gy:null,
Bq:B.sr(10,null,null),Ko:null,Cw:null,G_:null,L0:0,O6:0,Bu:0,Cx:B.sr(10,0,null),
M4:B.sr(10,B.pn,null),Fs:B.sr(10,0,null),G9:B.sr(10,B.pm,null),Kq:B.sr(10,0,null
),HI:B.sr(10,B.pm,null),Fr:B.sr(10,B.pm,null),Er:B.sr(10,B.pm,null),Gz:B.sr(10,B.
pm,null),AM:0,M_:0,M9:0,Nh:B.sr(3,B.pn,null),Rq:0,DE:B.sr(4,0,null),Cr:B.sr(4,B.
pn,null),Cg:0,H7:8,R6:250,IA:0,HQ:0,Pc:true,Ng:false,El:function(){return this;}
,C7:function(AA,aClip,aOffset,Ah,aBlend){var fullScreenUpdate=false;fullScreenUpdate=
B.i6;if(!fullScreenUpdate)AA.Mb(aClip,B.sq(B.sq(aClip,aOffset),this.H.slice(0,2)
),0x00000000,0x00000000,0x00000000,0x00000000,false);E.K.C7.call(this,AA,aClip,aOffset
,Ah,aBlend);},By:function(HH,Ja){var A;E.K.By.call(this,HH,Ja);if(!this.G&&(((HH&
0x1)===0x1)||((Ja&0x1)===0x1)))this.Ag([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);if(!this.
G&&(((HH&0x2)===0x2)||((Ja&0x2)===0x2)))this.Ag([0,0,(A=this.H)[2]-A[0],A[3]-A[1
]]);},D4:function(C){if((C!==this.Cw)||!C)E.K.D4.call(this,C);},K9:function(C){var
A;var Vn=this.AT;E.K.K9.call(this,C);if(((Vn!==this.AT)&&!this.G)&&((this.F&0x1)===
0x1))this.Ag([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},GQ:function(C){var A;var Nk=this.
C_;E.K.GQ.call(this,C);if(((Nk!==this.C_)&&!this.G)&&((this.F&0x1)===0x1))this.Ag([
0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},Hn:function(Q6){return true;},DispatchEvent:
function(Aq){if((this.O6>0)&&!!(E.KeyEvent.isPrototypeOf(Aq)?Aq:null))return null;
if(!!Aq){Aq.K0=!!this.Bu;if(!!this.Bu)Aq.Af=this.Bu;}var A6=null;if(!!this.Cw){A6=
this.Cw.DispatchEvent(Aq);if(!!A6){this.Bu=0;return A6;}}if(!!this.HR){A6=this.HR.
Bw.DispatchEvent(Aq);if(!A6)A6=this.Cj(Aq);if(!A6)A6=this.PE(Aq);this.Bu=0;return A6;
}A6=E.K.DispatchEvent.call(this,Aq);this.Bu=0;return A6;},BroadcastEvent:function(
Aq,aFilter){if(!!Aq){Aq.K0=!!this.Bu;if(!!this.Bu)Aq.Af=this.Bu;}var A6=E.K.BroadcastEvent.
call(this,Aq,aFilter);this.Bu=0;return A6;},Ag:function(AB){var A;if(this.L0>0)throw new
Error(OL);if(!!this.AT&&!this.G){if(((A=this.AT.EP)[0]>=A[2])||(A[1]>=A[3])){B.pl(
this,0);B.pl(this.AT,0);}var LD=false;if(LD)this.AT.EP=[0,0,(A=this.H)[2]-A[0],A[
3]-A[1]];else this.AT.EP=B.pA(this.AT.EP,AB);}var fullScreenUpdate=false;fullScreenUpdate=
B.i6;if(fullScreenUpdate)AB=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(!!this.G){E.K.
Ag.call(this,AB);return;}AB=B.hV(B.sq(AB,this.H.slice(0,2)),this.H);if((AB[0]>=AB[
2])||(AB[1]>=AB[3]))return;var I;for(I=0;I<this.Cg;I=I+1)if(!(((A=B.hV(this.Cr.Get(
I),AB))[0]>=A[2])||(A[1]>=A[3]))){this.Cr.Set(I,B.pA(this.Cr.Get(I),AB));this.DE.
Set(I,B.rY(this.Cr.Get(I)));return;}if(this.Cg<3){this.Cr.Set(this.Cg,AB);this.DE.
Set(this.Cg,B.rY(AB));this.Cg=this.Cg+1;return;}var Bx;var D$;var LG=0;var LH=0;
var Q9=2147483647;this.Cr.Set(this.Cg,AB);this.DE.Set(this.Cg,B.rY(AB));for(Bx=0;
Bx<=this.Cg;Bx=Bx+1)for(D$=Bx+1;D$<=this.Cg;D$=D$+1){var NA=B.rY(B.pA(this.Cr.Get(
Bx),this.Cr.Get(D$)));var Rz=((NA<<8)/(this.DE.Get(Bx)+this.DE.Get(D$)))|0;if(Rz<
Q9){Q9=Rz;LG=Bx;LH=D$;}}this.Cr.Set(LG,B.pA(this.Cr.Get(LG),this.Cr.Get(LH)));this.
DE.Set(LG,B.rY(this.Cr.Get(LG)));if(LH!==this.Cg){this.Cr.Set(LH,this.Cr.Get(this.
Cg));this.DE.Set(LH,this.DE.Get(this.Cg));}},MD:function(J,Fn){if((!Fn&&!!this.Cw
)&&(this.Cw.G===this))Fn=this.Cw;E.K.MD.call(this,J,Fn);},Gl:function(J){if(!!this.
Cw&&(this.Cw.G===this))this.MD(J,this.Cw);else E.K.Gl.call(this,J);},MC:function(
J,C1){if(!J)throw new Error(HE);if(((C1>0)&&!!this.Cw)&&(this.Cw.G===this)){var L=
J;var Nn=C1;while((L.U!==this.Cw)&&(Nn>0)){L=L.U;Nn=Nn-1;}C1=C1-Nn;}E.K.MC.call(
this,J,C1);},Aa:function(J,C1){if(((C1>=0)&&!!this.Cw)&&(this.Cw.G===this))C1=-1;
E.K.Aa.call(this,J,C1);},Va:function(){var C4=B._NewObject(E.JT,0);C4.K0=!!this.
Bu;if(!!this.Bu)C4.Af=this.Bu;return C4;},Jg:function(){var C4=B._NewObject(E.JR
,0);C4.K0=!!this.Bu;if(!!this.Bu)C4.Af=this.Bu;return C4;},Kp:function(){var C4=
B._NewObject(E.L7,0);C4.K0=!!this.Bu;if(!!this.Bu)C4.Af=this.Bu;return C4;},Vb:function(
Ae){var I;var Jo=false;for(I=0;I<10;I=I+1)if(!!this.Bq.Get(I)){var T=this.Er.Get(
I);var Ai=this.Bq.Get(I).G;while(!!Ai&&(Ai!==this)){T=B.sn(T,Ai.H.slice(0,2));Ai=
Ai.G;}if(!Ai&&(this.Bq.Get(I)!==this)){var tmp=this.Bq.Get(I);this.AM=I;this.Bq.
Set(I,null);tmp.Cj(this.Jg().InitializeUp(I,this.HI.Get(I),this.G9.Get(I),this.Fs.
Get(I),this.Cx.Get(I)+1,this.Fr.Get(I),false,this.Er.Get(I),this.Gz.Get(I)));if(
tmp===this.Gy)this.Gy=null;this.BroadcastEvent(this.Kp().InitializeUp(I,this.Cx.
Get(I)+1,false,tmp,this.Er.Get(I)),0x18);}else{this.Fs.Set(I,(this.G_.Af-this.Kq.
Get(I))|0);if(this.Fs.Get(I)<10)this.Fs.Set(I,10);this.AM=I;this.Bq.Get(I).Cj(this.
Jg().InitializeHold(I,T,this.G9.Get(I),this.Fs.Get(I),this.Cx.Get(I)+1,this.Fr.Get(
I),this.Er.Get(I),this.Gz.Get(I)));Jo=true;}}if(!Jo)this.G_.Bs(false);},GetFPS:function(
){var ticksCount=0;var Rk=0;ticksCount=((new Date).getTime()-B.pk)|0;if(!!this.M_&&(
ticksCount>this.M_))Rk=((this.M9*1000)/((ticksCount-this.M_)|0))|0;this.M9=0;this.
M_=ticksCount;return Rk;},Update:function(){var A;if(!this.Ko){this.Ko=B._NewObject(
B.Graphics.Canvas,0);this.Ko.K_([(A=this.H)[2]-A[0],A[3]-A[1]]);}this.Ko.Update(
);return this.UpdateGE20(this.Ko);},UpdateGE20:function(AA){if(!this.BeginUpdate(
))return Be;var Hh=this.UpdateCanvas(AA,AJ);this.EndUpdate();return Hh;},EndUpdate:
function(){if(this.Cg>0){this.M9=this.M9+1;this.Cg=0;}},UpdateCanvas:function(AA
,aOffset){var A;var Hh=Be;var U9=[].concat(aOffset,B.so(AA.FrameSize,aOffset));var
I;var Bx=this.Cg;this.L0=this.L0+1;for(I=0;(I<Bx)&&(I<4);I=I+1){if(this.DE.Get(I
)>0){this.C7(AA,B.sp(this.Cr.Get(I),aOffset),[-aOffset[0],-aOffset[1]],255,true);
Hh=B.pA(Hh,B.hV(U9,this.Cr.Get(I)));}else Bx=Bx+1;}this.L0=this.L0-1;if(!((Hh[0]>=
Hh[2])||(Hh[1]>=Hh[3])))return B.sp(Hh,aOffset);else return Hh;},GetUpdateRegion:
function(MQ){var I;var Bx=this.Cg;if(MQ<0)return Be;for(I=0;(I<Bx)&&(I<4);I=I+1){
if(!this.DE.Get(I)){Bx=Bx+1;MQ=MQ+1;}else if(I===MQ)return this.Cr.Get(I);}return Be;
},BeginUpdate:function(){var VD=true;var fullScreenUpdate=false;var I;if((!VD&&!
fullScreenUpdate)&&(this.Cg>0)){var RU=B.sr(3,B.pn,null);var PM=this.Cg;for(I=0;
I<PM;I=I+1)RU.Set(I,this.Cr.Get(I));for(I=0;I<this.Rq;I=I+1)this.Ag(this.Nh.Get(
I));for(I=0;I<PM;I=I+1)this.Nh.Set(I,RU.Get(I));this.Rq=PM;}var Bx;var D$;for(Bx=
0;Bx<(this.Cg-1);Bx=Bx+1)if(this.DE.Get(Bx)>0)for(D$=Bx+1;D$<this.Cg;D$=D$+1)if(
this.DE.Get(D$)>0){var NA=B.rY(B.pA(this.Cr.Get(Bx),this.Cr.Get(D$)));if(((NA-this.
DE.Get(Bx))-this.DE.Get(D$))<0){this.Cr.Set(Bx,B.pA(this.Cr.Get(Bx),this.Cr.Get(
D$)));this.DE.Set(Bx,NA);this.DE.Set(D$,0);}}for(I=this.Cg-1;I>=0;I=I-1)if(!this.
DE.Get(I))this.Cg=this.Cg-1;return this.Cg;},DoesNeedUpdate:function(){if(this.Cg>
0)return true;return false;},Initialize:function(aSize){this.Q([].concat(AJ,aSize
));if(this.Pc)this.F=this.F|0x60;else this.F=this.F|0x20;this.Ag(this.H);return this;
},SetRootFocus:function(O2){if(O2===this.Pc)return false;this.Pc=O2;if(!O2){if(!
!this.Cw)this.Cw.By(0x0,0x40);if(!!this.HR)this.HR.Bw.By(0x0,0x40);else this.By(
0x0,0x40);}else{if(!!this.HR)this.HR.Bw.By(0x40,0x0);else this.By(0x40,0x0);if(!
!this.Cw)this.Cw.By(0x40,0x0);}return true;},SetUserInputTimestamp:function(U5){
this.Bu=U5;},DriveKeyboardHitting:function(B6,I$,Eo){var A;var PF=!!this.Ea;if(!
!this.Ea&&((!Eo||(this.IA!==B6))||(this.HQ!==I$))){var C4=null;var L=(E.AD.isPrototypeOf(
A=this.Ea)?A:null);var F0=(E.IW.isPrototypeOf(A=this.Ea)?A:null);if(!!this.IA)C4=
B._NewObject(E.KeyEvent,0).Initialize(this.IA,false);if(this.HQ!==0x00)C4=B._NewObject(
E.KeyEvent,0).Initialize2(this.HQ,false);if(!!F0)F0.Cj(C4);else if(!!L)L.Cj(C4);
this.IA=0;this.HQ=0x00;this.Ea=null;}if(!!this.Ea){var C4=null;var L=(E.AD.isPrototypeOf(
A=this.Ea)?A:null);var F0=(E.IW.isPrototypeOf(A=this.Ea)?A:null);if(!!B6)C4=B._NewObject(
E.KeyEvent,0).Initialize(B6,true);if(this.HQ!==0x00)C4=B._NewObject(E.KeyEvent,0
).Initialize2(I$,true);if(!!F0)F0.Cj(C4);else if(!!L)L.Cj(C4);}if(this.Ng&&((!Eo||(
this.IA!==B6))||(this.HQ!==I$))){this.IA=0;this.HQ=0x00;this.Ng=false;}if((!this.
Ea&&Eo)&&(this.O6>0)){this.IA=B6;this.HQ=I$;this.Ng=true;}if((!this.Ea&&Eo)&&!this.
Ng){if(!!B6)this.Ea=this.DispatchEvent(B._NewObject(E.KeyEvent,0).Initialize(B6,
true));if(I$!==0x00)this.Ea=this.DispatchEvent(B._NewObject(E.KeyEvent,0).Initialize2(
I$,true));if(!(E.IW.isPrototypeOf(A=this.Ea)?A:null)&&!(E.AD.isPrototypeOf(A=this.
Ea)?A:null))this.Ea=null;this.IA=B6;this.HQ=I$;PF=PF||!!this.Ea;}this.Bu=0;return PF;
},DriveCursorMovement:function(Bo){return this.DriveMultiTouchMovement(this.AM,Bo
);},DriveMultiTouchMovement:function(V,Bo){if((V<0)||(V>9)){this.Bu=0;return false;
}var offset=B.sn(Bo,this.Er.Get(V));this.Er.Set(V,Bo);if(!this.Bq.Get(V)||B.sa(offset
,AJ)){this.Bu=0;return false;}var T=Bo;var Ai=this.Bq.Get(V).G;while(!!Ai&&(Ai!==
this)){T=B.sn(T,Ai.H.slice(0,2));Ai=Ai.G;}if(!Ai&&(this.Bq.Get(V)!==this)){var tmp=
this.Bq.Get(V);this.AM=V;this.Bq.Set(V,null);tmp.Cj(this.Jg().InitializeUp(V,this.
HI.Get(V),this.G9.Get(V),this.Fs.Get(V),this.Cx.Get(V)+1,this.Fr.Get(V),false,this.
Er.Get(V),this.Gz.Get(V)));if(tmp===this.Gy)this.Gy=null;this.BroadcastEvent(this.
Kp().InitializeUp(V,this.Cx.Get(V)+1,false,tmp,Bo),0x18);}else{this.HI.Set(V,T);
this.AM=V;this.Bq.Get(V).Cj(this.Va().Initialize(V,T,this.G9.Get(V),offset,this.
Fs.Get(V),this.Cx.Get(V)+1,this.Fr.Get(V),Bo,this.Gz.Get(V)));}this.Bu=0;return true;
},DriveCursorHitting:function(Eo,V,Bo){return this.DriveMultiTouchHitting(Eo,V,Bo
);},DriveMultiTouchHitting:function(Eo,V,Bo){var A;if((V<0)||(V>9)){this.Bu=0;return false;
}var ticksCount=this.Bu;var Jk=[].concat([-this.H7,-this.H7],[this.H7+1,this.H7+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.pk)|0;}var VK=this.Bu;this.
DriveMultiTouchMovement(V,Bo);Bo=this.Er.Get(V);this.Bu=VK;if(Eo)this.Gz.Set(V,Bo
);if((Eo&&!this.Bq.Get(V))&&!this.O6){var D_=null;var T=Bo;if(B.se(this.M4.Get(V
),Bo)&&((ticksCount-this.Kq.Get(V))<=(((A=this.R6)<0)?A+0x100000000:A)))this.Cx.
Set(V,this.Cx.Get(V)+1);else this.Cx.Set(V,0);this.M4.Set(V,B.sq(Jk,Bo));this.Kq.
Set(V,ticksCount);if((!!this.Cw&&!!this.Cw.G)&&((this.Cw.F&0x18)===0x18)){var S=
B.sq(Jk,this.Cw.G.Of(Bo));D_=this.Cw.Ge(S,V,this.Cx.Get(V)+1,null,0x0);}if(!D_){
if(!!this.Gy&&!!this.Gy.G){if(((this.Gy.F&0x8)===0x8)&&((this.Gy.F&0x10)===0x10)
){var S=B.sq(Jk,this.Gy.G.Of(Bo));D_=this.Gy.Ge(S,V,this.Cx.Get(V)+1,null,0x0);}
}else if(!!this.HR)D_=this.Ge(B.sq(Jk,Bo),V,this.Cx.Get(V)+1,this.HR.Bw,0x0);else
D_=this.Ge(B.sq(Jk,Bo),V,this.Cx.Get(V)+1,null,0x0);}if(!!D_){this.BroadcastEvent(
this.Kp().InitializeDown(V,this.Cx.Get(V)+1,false,D_.AD,Bo),0x18);this.Bq.Set(V,
D_.AD);this.Fr.Set(V,D_.As);}else{this.Bq.Set(V,null);this.Fr.Set(V,AJ);this.Bu=
0;return false;}var Ai=D_.AD.G;while(!!Ai&&(Ai!==this)){T=B.sn(T,Ai.H.slice(0,2)
);Ai=Ai.G;}this.G9.Set(V,T);this.HI.Set(V,T);this.Fs.Set(V,0);this.G_.Bs(true);this.
AM=V;this.Bq.Get(V).Cj(this.Jg().InitializeDown(V,T,this.Cx.Get(V)+1,this.Fr.Get(
V),false,Bo));this.Bu=0;return true;}if(!Eo&&!!this.Bq.Get(V)){var T=Bo;var Ai=this.
Bq.Get(V).G;while(!!Ai&&(Ai!==this)){T=B.sn(T,Ai.H.slice(0,2));Ai=Ai.G;}if(!Ai)T=
this.HI.Get(V);this.AM=V;var tmp=this.Bq.Get(V);this.Bq.Set(V,null);tmp.Cj(this.
Jg().InitializeUp(V,T,this.G9.Get(V),this.Fs.Get(V),this.Cx.Get(V)+1,this.Fr.Get(
V),false,Bo,this.Gz.Get(V)));this.BroadcastEvent(this.Kp().InitializeUp(V,this.Cx.
Get(V)+1,false,tmp,Bo),0x18);this.Bu=0;return true;}this.Bu=0;return false;},ED:
function(G6,Q3,Fm){var Jk=[].concat([-this.H7,-this.H7],[this.H7+1,this.H7+1]);if(
G6===this)G6=null;if(!this.Bq.Get(this.AM))return;var D_;D_=this.Ge(B.sq(Jk,this.
Er.Get(this.AM)),this.AM,1,G6,Fm);if(!!D_&&(this.Bq.Get(this.AM)!==D_.AD))this.P2(
D_.AD,D_.As);if(!D_&&(this.Bq.Get(this.AM)!==Q3))this.P2(Q3,AJ);},P2:function(G6
,Fl){if(!this.Bq.Get(this.AM)||(this.Bq.Get(this.AM)===G6))return;var tmp=this.Bq.
Get(this.AM);this.Bq.Set(this.AM,null);tmp.Cj(this.Jg().InitializeUp(this.AM,this.
HI.Get(this.AM),this.G9.Get(this.AM),this.Fs.Get(this.AM),this.Cx.Get(this.AM)+1
,this.Fr.Get(this.AM),true,this.Er.Get(this.AM),this.Gz.Get(this.AM)));this.BroadcastEvent(
this.Kp().InitializeUp(this.AM,this.Cx.Get(this.AM)+1,true,tmp,this.Er.Get(this.
AM)),0x18);var T=this.Er.Get(this.AM);var Ai=null;if(!!G6)Ai=G6.G;while(!!Ai&&(Ai
!==this)){T=B.sn(T,Ai.H.slice(0,2));Ai=Ai.G;}if(!Ai&&(G6!==this)){this.Bq.Set(this.
AM,null);this.G_.Bs(false);return;}this.BroadcastEvent(this.Kp().InitializeDown(
this.AM,this.Cx.Get(this.AM)+1,true,G6,this.Er.Get(this.AM)),0x18);var ticksCount=
0;ticksCount=((new Date).getTime()-B.pk)|0;this.Bq.Set(this.AM,G6);this.Fr.Set(this.
AM,Fl);this.G9.Set(this.AM,T);this.HI.Set(this.AM,T);this.Cx.Set(this.AM,0);this.
Fs.Set(this.AM,0);this.Kq.Set(this.AM,ticksCount);this.Gz.Set(this.AM,this.Er.Get(
this.AM));this.Bq.Get(this.AM).Cj(this.Jg().InitializeDown(this.AM,T,this.Cx.Get(
this.AM)+1,this.Fr.Get(this.AM),true,this.Gz.Get(this.AM)));},_Init:function(aArg
){E.K._Init.call(this,aArg);E.Timer._Init.call(this.G_={N:this},0);(this.Bq=[]).
__proto__=E.Root.Bq;(this.Cx=[]).__proto__=E.Root.Cx;(this.M4=[]).__proto__=E.Root.
M4;(this.Fs=[]).__proto__=E.Root.Fs;(this.G9=[]).__proto__=E.Root.G9;(this.Kq=[]
).__proto__=E.Root.Kq;(this.HI=[]).__proto__=E.Root.HI;(this.Fr=[]).__proto__=E.
Root.Fr;(this.Er=[]).__proto__=E.Root.Er;(this.Gz=[]).__proto__=E.Root.Gz;(this.
Nh=[]).__proto__=E.Root.Nh;(this.DE=[]).__proto__=E.Root.DE;(this.Cr=[]).__proto__=
E.Root.Cr;this.__proto__=E.Root;this.F=0x7F;this.G_.La(50);this.G_.Ii=[this,this.
Vb];},_Done:function(){this.__proto__=E.Root;this.G_._Done();E.K._Done.call(this
);},_ReInit:function(){E.K._ReInit.call(this);this.G_._ReInit();},_Mark:function(
D){var A;E.K._Mark.call(this,D);if((A=this.Ea)&&(A._cycle!=D))A._Mark(A._cycle=D
);if((A=this.HR)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Gy)&&(A._cycle!=D
))A._Mark(A._cycle=D);B.sj(this.Bq,D);if((A=this.Ko)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Cw)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.G_)._cycle!=D)A.
_Mark(A._cycle=D);},_className:"Core::Root"};E.Event={Af:0,K0:false,KX:function(
){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk)|0;return ticksCount;},
CU:function(aArg){this.Af=this.KX();},_Init:function(aArg){this.__proto__=E.Event;
this.CU(aArg);B.gb++;},_Done:function(){this.__proto__=E.Event;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=
D);},N:null,_cycle:0,_observers:null,_className:"Core::Event"};E.KeyEvent={B$:0,
A8:0,Down:false,Initialize2:function(B6,Eo){this.B$=0;this.A8=B6;this.Down=Eo;if((
B6>=0x30)&&(B6<=0x39))this.B$=(10+B6)-48;if((B6>=0x41)&&(B6<=0x5A))this.B$=(105+
B6)-65;if((B6>=0x61)&&(B6<=0x7A))this.B$=(105+B6)-97;if(B6===0x20)this.B$=131;if(
!this.B$)switch(B6){case 0x2B:this.B$=132;break;case 0x2D:this.B$=133;break;case
0x2A:this.B$=134;break;case 0x2F:this.B$=135;break;case 0x3D:this.B$=136;break;case
0x2E:this.B$=137;break;case 0x2C:this.B$=138;break;case 0x3A:this.B$=139;break;case
0x3B:this.B$=140;break;default:;}return this;},Initialize:function(B6,Eo){this.B$=
B6;this.Down=Eo;this.A8=0x00;var O8=B6-10;var O7=B6-105;if((O8>=0)&&(O8<=9))this.
A8=(48+O8)&0xFFFF;if((O7>=0)&&(O7<=25))this.A8=(65+O7)&0xFFFF;if(B6===131)this.A8=
0x20;if(this.A8===0x00)switch(B6){case 132:this.A8=0x2B;break;case 133:this.A8=0x2D;
break;case 134:this.A8=0x2A;break;case 135:this.A8=0x2F;break;case 136:this.A8=0x3D;
break;case 137:this.A8=0x2E;break;case 138:this.A8=0x2C;break;case 139:this.A8=0x3A;
break;case 140:this.A8=0x3B;break;default:;}return this;},SN:function(Q1){switch(
Q1){case 141:return((this.A8>=0x41)&&(this.A8<=0x5A))||((this.A8>=0x61)&&(this.A8<=
0x7A));case 142:return(((this.A8>=0x41)&&(this.A8<=0x5A))||((this.A8>=0x61)&&(this.
A8<=0x7A)))||((this.A8>=0x30)&&(this.A8<=0x39));case 143:return(this.A8>=0x30)&&(
this.A8<=0x39);case 144:return(((this.A8>=0x41)&&(this.A8<=0x46))||((this.A8>=0x61
)&&(this.A8<=0x66)))||((this.A8>=0x30)&&(this.A8<=0x39));case 145:return this.A8
!==0x00;case 146:return(this.A8===0x00)&&!!this.B$;case 147:return(((this.B$===6
)||(this.B$===7))||(this.B$===4))||(this.B$===5);case 148:return(this.A8!==0x00)||
!!this.B$;default:;}return Q1===this.B$;},_Init:function(aArg){E.Event._Init.call(
this,aArg);this.__proto__=E.KeyEvent;},_className:"Core::KeyEvent"};E.L7={MI:null
,CT:B.pm,Fi:0,CS:0,Down:false,Ej:false,InitializeUp:function(V,DN,I_,O4,FY){this.
Down=false;this.CS=V;this.Fi=DN;this.CT=FY;this.MI=O4;this.Ej=I_;return this;},InitializeDown:
function(V,DN,I_,O4,FY){this.Down=true;this.CS=V;this.Fi=DN;this.CT=FY;this.MI=O4;
this.Ej=I_;return this;},_Init:function(aArg){E.Event._Init.call(this,aArg);this.
__proto__=E.L7;},_Mark:function(D){var A;E.Event._Mark.call(this,D);if((A=this.MI
)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"};E.JR={
DY:B.pm,CT:B.pm,Fi:0,CH:0,Di:B.pm,DF:B.pm,CS:0,Down:false,Ej:false,InitializeHold:
function(V,Is,Lp,Lq,DN,Fl,FY,Lo){this.Down=true;this.CS=V;this.DF=B.so(Is,Fl);this.
Di=B.so(Lp,Fl);this.CH=Lq;this.Fi=DN;this.CT=FY;this.DY=Lo;return this;},InitializeUp:
function(V,Is,Lp,Lq,DN,Fl,I_,FY,Lo){this.Down=false;this.CS=V;this.DF=B.so(Is,Fl
);this.Di=B.so(Lp,Fl);this.CH=Lq;this.Fi=DN;this.Ej=I_;this.CT=FY;this.DY=Lo;return this;
},InitializeDown:function(V,Is,DN,Fl,I_,FY){this.Down=true;this.CS=V;this.DF=B.so(
Is,Fl);this.Di=B.so(Is,Fl);this.CH=0;this.Fi=DN;this.Ej=I_;this.CT=FY;this.DY=FY;
return this;},_Init:function(aArg){E.Event._Init.call(this,aArg);this.__proto__=
E.JR;},_className:"Core::CursorEvent"};E.JT={DY:B.pm,CT:B.pm,Fi:0,CH:0,As:B.pm,Di:
B.pm,DF:B.pm,CS:0,Initialize:function(V,Is,Lp,aOffset,Lq,U3,Fl,FY,Lo){this.CS=V;
this.DF=B.so(Is,Fl);this.Di=B.so(Lp,Fl);this.As=aOffset;this.CH=Lq;this.Fi=U3;this.
CT=FY;this.DY=Lo;return this;},_Init:function(aArg){E.Event._Init.call(this,aArg
);this.__proto__=E.JT;},_className:"Core::DragEvent"};E.Lb={AU:B.pm,Space:0,Mc:0
,C7:function(AA,aClip,aOffset,Ah,aBlend){},Q:function(C){var A;if(B.sb(C,this.H)
)return;var IG=[(A=this.H)[2]-A[0],A[3]-A[1]];var Nj=[C[2]-C[0],C[3]-C[1]];var JK=
!B.sa(IG,Nj);var offset=B.sn(C.slice(0,2),this.H.slice(0,2));if(!B.sa(offset,AJ)&&
!JK){var L=this.U;while(!!L&&!((L.F&0x200)===0x200)){if(((L.F&0x400)===0x400)){var
tmp=((L.F&0x100)===0x100);L.K5(offset,tmp);}L=L.U;}}if((JK&&(IG[0]>0))&&(IG[1]>0
)){var W=B.sq(this.H,this.AU);var L=this.U;var LB=0x14;while(!!L&&!((L.F&0x200)===
0x200)){if(((L.F&0x400)===0x400)){if(!!L.AN&&(L.AN.IH!==this))L.AN=null;if(!L.AN&&((
L.D1!==LB)||!!this.Mc))L.Ku(W,this);}L=L.U;}}E.AY.Q.call(this,C);if(!!this.G&&JK
){this.F=this.F|0x1000;if(!((this.G.F&0x2000)===0x2000)){this.G.F=this.G.F|0x4000;
B.kD([A=this.G,A.Cs],this);}}},_Init:function(aArg){E.AY._Init.call(this,aArg);this.
__proto__=E.Lb;this.F=0x203;},_className:"Core::Outline"};E.HB={Ju:B.sr(8,null,null
),Ky:null,AD:null,Fg:null,FU:null,FN:null,Pg:-1,Pf:0,Ez:-1,Dg:0,Et:0,EQ:-1,TH:0,
AU:0,I6:-1,Ik:0,AW:24,A4:0,K1:null,Dh:false,Pp:false,C7:function(AA,aClip,aOffset
,Ah,aBlend){E.K.C7.call(this,AA,aClip,aOffset,Ah,aBlend);},DispatchEvent:function(
Aq){var A;var result=null;if(((this.I6>=0)&&(this.I6<this.A4))&&!this.C8){this.EQ=
this.I6;this.AD=(E.AD.isPrototypeOf(A=B._NewObject(this.K1,0))?A:null);if(!!this.
Az)this.Az.U=this.AD;else this.Am=this.AD;this.AD.X=this.Az;this.Az=this.AD;this.
AD.G=this;(A=this.FN)?A[1].call(A[0],this):null;var Ai=(E.K.isPrototypeOf(A=this.
AD)?A:null);if(!!Ai)result=Ai.DispatchEvent(Aq);else result=this.AD.Cj(Aq);if(!!
this.AD.X)this.AD.X.U=null;else this.Am=null;this.Az=this.AD.X;this.AD.X=null;this.
AD.G=null;this.AD=null;}if(!result)result=E.K.DispatchEvent.call(this,Aq);return result;
},DM:function(B7){var A;if(!this.K1){B.kD(this.FU,this);return;}this.Pp=true;var
LS=0;if(!this.Dh)LS=this.Ik;var DB=((-LS-this.AU)/this.AW)|0;var Eg=((((((A=this.
H)[3]-A[1])-LS)-this.AU)-1)/this.AW)|0;var M3=(((((A=this.H)[3]-A[1])+this.AW)-1
)/this.AW)|0;if(M3<1)M3=1;var MY=(M3/2)|0;var MZ=(M3/3)|0;if(!MY)MY=1;if(!MZ)MZ=
1;if(DB<this.Dg){DB=DB-MY;Eg=Eg+MZ;}else if(Eg>this.Ez){DB=DB-MZ;Eg=Eg+MY;}else{
DB=this.Dg;Eg=this.Ez;}if(!this.Dh){if(DB>=this.A4){DB=0;Eg=-1;}else if(Eg<0){DB=
0;Eg=-1;}if(Eg>=this.A4)Eg=this.A4-1;if(DB<0)DB=0;}else if(this.A4<=0){DB=0;Eg=-
1;}var Nu=this.Dg;var Nv=this.Ez;var Pn=0;var Po=-1;if(DB>Nu)Nu=DB;if(Eg<Nv)Nv=Eg;
if(Nu<=Nv){while((this.Ez<Eg)&&(this.Dg<Nu)){this.LV();this.Re();}while((this.Dg>
DB)&&(this.Ez>Nv)){this.RD();this.Rd();}}else{this.Ez=(this.Ez-this.Dg)+DB;this.
Dg=DB;Pn=this.Dg;Po=this.Ez;}while(this.Dg<DB)this.LV();while(this.Ez>Eg)this.RD(
);while(this.Dg>DB)this.Rd();while(this.Ez<Eg)this.Re();var Z=this.Am;var DQ=this.
Dg;var T=[0,(LS+this.AU)+(DQ*this.AW)];var At=(A=this.H)[3]-A[1];var Rl=this.AW;
var LA=null;while(!!Z){var BH=B.sn(T,Z.GetExtent().slice(0,2));var Pj=T[1];var Pk=
T[1]+Rl;var C5=DQ;if(this.Dh){if(C5<0)C5=this.A4-(-C5%this.A4);if(C5>0)C5=C5%this.
A4;}if(!B.sa(BH,AJ))Z.K5(BH,true);if(((((C5>=this.Pf)&&(C5<=this.Pg))||((DQ>=Pn)&&(
DQ<=Po)))&&(Pj<At))&&(Pk>0)){this.AD=Z;this.EQ=C5;(A=this.FN)?A[1].call(A[0],this
):null;}Z=Z.U;DQ=DQ+1;T=[T[0],T[1]+this.AW];}DQ=this.Dg;Z=this.Am;T=[0,(LS+this.
AU)+(DQ*this.AW)];while(!!Z){var Pj=T[1];var Pk=T[1]+Rl;var C5=DQ;if(this.Dh){if(
C5<0)C5=this.A4-(-C5%this.A4);if(C5>0)C5=C5%this.A4;}if((((C5>=this.Pf)&&(C5<=this.
Pg))||((DQ>=Pn)&&(DQ<=Po)))&&!((Pj<At)&&(Pk>0))){this.AD=Z;this.EQ=C5;(A=this.FN
)?A[1].call(A[0],this):null;}if(((C5===this.I6)&&this.Dh)&&!!LA){var Q8=[0,0,(A=
this.H)[2]-A[0],A[3]-A[1]];if(B.rY(B.hV(Z.GetExtent(),Q8))>B.rY(B.hV(LA.GetExtent(
),Q8)))LA=Z;}else if(C5===this.I6)LA=Z;Z=Z.U;DQ=DQ+1;T=[T[0],T[1]+this.AW];}this.
Pf=0;this.Pg=-1;this.AD=null;this.EQ=-1;this.D4(LA);this.Pp=false;B.kD(this.FU,this
);},NU:function(J,aFilter){return null;},MD:function(J,Fn){throw new Error(OM);}
,Gl:function(J){throw new Error(ON);},MC:function(J,C1){throw new Error(OO);},GV:
function(J){throw new Error(OP);},Aa:function(J,C1){throw new Error(OQ);},RD:function(
){var Z=this.Az;if(!Z)return null;if(Z===this.C8)this.D4(null);this.Ez=this.Ez-1;
if(!!Z.X)Z.X.U=null;else this.Am=null;this.Az=Z.X;Z.X=null;Z.G=null;Z.AN=null;if(
this.Et<8){this.Ju.Set(this.Et,Z);this.Et=this.Et+1;}return Z;},Re:function(){var
A;var Z=null;if(!this.Et){Z=(E.AD.isPrototypeOf(A=B._NewObject(this.K1,0))?A:null
);Z.C9(0x1D);}else{this.Et=this.Et-1;Z=this.Ju.Get(this.Et);}this.Ez=this.Ez+1;var
IQ=this.Ez;if(this.Dh){if(IQ<0)IQ=this.A4-(-IQ%this.A4);if(IQ>0)IQ=IQ%this.A4;}this.
EQ=IQ;this.AD=Z;(A=this.FN)?A[1].call(A[0],this):null;this.AD=null;this.EQ=-1;if(
!!this.Az)this.Az.U=Z;Z.G=this;Z.X=this.Az;this.Az=Z;if(!this.Am)this.Am=Z;if(IQ===
this.I6)this.D4(Z);return Z;},LV:function(){var Z=this.Am;if(!Z)return null;if(Z===
this.C8)this.D4(null);this.Dg=this.Dg+1;if(!!Z.U)Z.U.X=null;else this.Az=null;this.
Am=Z.U;Z.U=null;Z.G=null;Z.AN=null;if(this.Et<8){this.Ju.Set(this.Et,Z);this.Et=
this.Et+1;}return Z;},Rd:function(){var A;var Z=null;if(!this.Et){Z=(E.AD.isPrototypeOf(
A=B._NewObject(this.K1,0))?A:null);Z.C9(0x1D);}else{this.Et=this.Et-1;Z=this.Ju.
Get(this.Et);}this.Dg=this.Dg-1;var DU=this.Dg;if(this.Dh){if(DU<0)DU=this.A4-(-
DU%this.A4);if(DU>0)DU=DU%this.A4;}this.EQ=DU;this.AD=Z;(A=this.FN)?A[1].call(A[
0],this):null;this.AD=null;this.EQ=-1;if(!!this.Am)this.Am.X=Z;Z.G=this;Z.U=this.
Am;this.Am=Z;if(!this.Az)this.Az=Z;if(DU===this.I6)this.D4(Z);return Z;},VA:function(
Ae){this.BT(this.Fg.As[1]);},VB:function(Ae){var A;if(!!this.Ky){this.Ky.Bs(false
);this.Ky.Av=null;this.Ky.PD=null;this.Ky=null;}if(!this.Dh){var S=this.Sw(0,this.
A4-1);var No=this.H.slice(0,2);S=B.sY(S,S[1]-this.Ik);S=[].concat(S.slice(0,3),S[
3]+this.TH);if(S[0]>No[0])S=[].concat(No[0],S.slice(1,4));if(S[1]>No[1])S=B.sY(S
,No[1]);var offset=S[1]-this.H[1];var Ps=((A=this.H)[3]-A[1])-(S[3]-S[1]);if(Ps>
0)Ps=0;this.Fg.As=[0,this.AU];this.Fg.A$=[0,(this.AU+Ps)-offset];this.Fg.A_=[0,this.
AU-offset];}else{var offset=32000-(32000%this.AW);this.Fg.As=[0,this.AU];this.Fg.
A$=[0,this.AU-offset];this.Fg.A_=[0,this.AU+offset];}},J4:function(C){var A;if(this.
Dh===C)return;this.Dh=C;while(!!this.LV());this.Bl();this.Ag([0,0,(A=this.H)[2]-
A[0],A[3]-A[1]]);},J8:function(C){if(C===this.Fg)return;if(!!C&&!!C.Kw){B.ta("%s%$%*%s"
,OR,C,OS);throw new Error(OT);}if(!!this.Fg){this.Fg.Nr=null;this.Fg.Kw=null;}this.
Fg=C;if(!!C){C.Nr=[this,this.VB];C.Kw=[this,this.VA];}},BT:function(C){var A;if(
this.Dh&&(this.A4>0)){var CF=this.A4*this.AW;if(C<0)C=CF-(-C%CF);if(C>0)C=C%CF;if(
C>0)C=C-CF;}if(C===this.AU)return;this.AU=C;this.Bl();this.Ag([0,0,(A=this.H)[2]-
A[0],A[3]-A[1]]);},FR:function(C){var A;if(C<1)C=1;if(C===this.AW)return;this.AW=
C;while(!!this.LV());this.Bl();this.Ag([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},Hp:
function(C){var A;if(C<0)C=0;if(C===this.A4)return;var S=[0,0,(A=this.H)[2]-A[0]
,A[3]-A[1]];if(!this.Dh){if(C>this.A4){S=B.sY(S,(this.Ik+this.AU)+(this.A4*this.
AW));S=[].concat(S.slice(0,3),(this.Ik+this.AU)+(C*this.AW));}else{S=B.sY(S,(this.
Ik+this.AU)+(C*this.AW));S=[].concat(S.slice(0,3),(this.Ik+this.AU)+(this.A4*this.
AW));}}else while(!!this.LV());this.A4=C;this.Bl();this.Ag(S);},JY:function(It){
if(this.Pp)return null;if((It<0)||(It>=this.A4))return null;if(!this.Dh){if((It<
this.Dg)||(It>this.Ez))return null;var L=this.Am;while(It>this.Dg){L=L.U;It=It-1;
}return L;}else{var DU=this.Dg;if(DU<0)DU=this.A4-(-DU%this.A4);if(DU>0)DU=DU%this.
A4;var L=this.Am;while(!!L){if((DU%this.A4)===It)return L;L=L.U;DU=DU+1;}return null;
}},JX:function(Bo){if(((this.A4<=0)||(Bo[0]<this.H[0]))||(Bo[0]>=this.H[2]))return-
1;Bo=B.sn(Bo,this.H.slice(0,2));if(!this.Dh){var Z=(Bo[1]-this.AU)-this.Ik;if(Z>
0)Z=(Z/this.AW)|0;if((Z<0)||(Z>=this.A4))return-1;return Z;}var Z=Bo[1]-this.AU;
if(Z>0)Z=(Z/this.AW)|0;if(Z<0)Z=((Z/this.AW)|0)-1;if(Z<0)Z=this.A4-(-Z%this.A4);
if(Z>0)Z=Z%this.A4;return Z;},Sw:function(Ln,MP){if(Ln<0)Ln=0;if(MP>=this.A4)MP=
this.A4-1;if(Ln>MP)return Be;var S=this.H;var BH=this.AU;if(this.Dh){var CF=this.
A4*this.AW;if(BH<0)BH=CF-(-BH%CF);if(BH>0)BH=BH%CF;if(BH>0)BH=BH-CF;}else BH=BH+
this.Ik;S=B.sY(S,(S[1]+BH)+(Ln*this.AW));S=[].concat(S.slice(0,3),S[1]+(((MP-Ln)+
1)*this.AW));return S;},_Init:function(aArg){E.K._Init.call(this,aArg);(this.Ju=[
]).__proto__=E.HB.Ju;this.__proto__=E.HB;this.Q(OU);this.K1=B.tf.Text;},_Mark:function(
D){var A;E.K._Mark.call(this,D);B.sj(this.Ju,D);if((A=this.Ky)&&(A._cycle!=D))A.
_Mark(A._cycle=D);if((A=this.AD)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fg
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.FU)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.FN)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:
"Core::VerticalList"};E.GW={Qr:null,FM:null,FL:null,K6:null,D3:null,CA:null,Fo:0
,CS:0,Af:0,Fi:0,CH:0,As:B.pm,Di:B.pm,DF:B.pm,Lg:1000,EE:8,CC:0,H$:-1,Qh:1,S1:1,R_:
false,Down:false,Fb:false,Ej:false,Gw:false,C7:function(AA,aClip,aOffset,Ah,aBlend
){},Cj:function(Aq){var A;var Ak=(E.JR.isPrototypeOf(Aq)?Aq:null);var Al=(E.JT.isPrototypeOf(
Aq)?Aq:null);var Nb=this.Fb;if(!Ak&&!Al)return null;if(!!Ak){this.Gw=Ak.Down;this.
Down=Ak.Down;this.Fb=this.N5(Ak.DF);this.Di=Ak.Di;this.DF=Ak.DF;this.As=AJ;this.
CH=Ak.CH;this.Fi=Ak.Fi;this.Ej=Ak.Ej;this.CS=Ak.CS;this.Fo=Ak.CS;this.Af=Ak.Af;if(
Ak.Down&&!Ak.CH)Nb=false;}if(!!Al){this.Down=true;this.Fb=this.N5(Al.DF);this.Di=
Al.Di;this.DF=Al.DF;this.As=Al.As;this.CH=Al.CH;this.Fi=Al.Fi;this.CS=Al.CS;this.
Fo=Al.CS;this.Ej=false;this.Af=Al.Af;(A=this.Qr)?A[1].call(A[0],this):null;}if((
!!Ak&&this.Down)&&!this.CH)(A=this.CA)?A[1].call(A[0],this):null;if((!!Ak&&this.
Down)&&(this.CH>0))(A=this.K6)?A[1].call(A[0],this):null;if((this.Down&&this.Fb)&&
!Nb)(A=this.FL)?A[1].call(A[0],this):null;if(((this.Down&&!this.Fb)&&Nb)||((!this.
Down&&this.Fb)&&Nb))(A=this.FM)?A[1].call(A[0],this):null;if(!!Ak&&!this.Down)(A=
this.D3)?A[1].call(A[0],this):null;if(!!this.CC){if(((((this.CC&0x10)===0x10)&&!
!Ak)&&Ak.Down)&&(Ak.CH>=this.Lg))this.El().ED(null,this,0x10);if((((this.CC&0x1)===
0x1)&&!!Al)&&((Al.CT[1]-Al.DY[1])<=-this.EE))this.El().ED(null,this,0x1);if((((this.
CC&0x2)===0x2)&&!!Al)&&((Al.CT[1]-Al.DY[1])>=this.EE))this.El().ED(null,this,0x2
);if((((this.CC&0x4)===0x4)&&!!Al)&&((Al.CT[0]-Al.DY[0])<=-this.EE))this.El().ED(
null,this,0x4);if((((this.CC&0x8)===0x8)&&!!Al)&&((Al.CT[0]-Al.DY[0])>=this.EE))
this.El().ED(null,this,0x8);}return this;},Ge:function(AB,V,DN,FX,Fm){var A;if(!
!FX&&(FX!==this))return null;if((DN<this.S1)||(DN>this.Qh))return null;if((this.
H$>=0)&&(V!==this.H$))return null;if((!this.R_&&this.Gw)&&(V!==this.Fo))return null;
if(!!(Fm&this.CC))return null;if(this.Sz()){var P=B.hV(AB,this.GetExtent());if(!((
P[0]>=P[2])||(P[1]>=P[3]))){var Cc=B.rZ(AB);var offset=AJ;if(Cc[0]<P[0])offset=[
P[0]-Cc[0],offset[1]];if(Cc[0]>=P[2])offset=[(P[2]-Cc[0])-1,offset[1]];if(Cc[1]<
P[1])offset=[offset[0],P[1]-Cc[1]];if(Cc[1]>=P[3])offset=[offset[0],(P[3]-Cc[1])-
1];return B._NewObject(E.JS,0).Initialize(this,offset);}}else{var Br=B.sr(9,B.pm
,null);var I;Br.Set(0,B.rZ(AB));Br.Set(1,Br.Get(0));Br.Set(2,Br.Get(0));Br.Set(3
,Br.Get(0));Br.Set(4,Br.Get(0));Br.Set(1,[AB[0],Br.Get(1)[1]]);Br.Set(2,[Br.Get(
2)[0],AB[1]]);Br.Set(3,[AB[2],Br.Get(3)[1]]);Br.Set(4,[Br.Get(4)[0],AB[3]]);Br.Set(
5,AB.slice(0,2));Br.Set(6,[AB[2],AB[1]]);Br.Set(7,[AB[0],AB[3]]);Br.Set(8,AB.slice(
2,4));for(I=0;I<9;I=I+1)if(this.N5(Br.Get(I)))return B._NewObject(E.JS,0).Initialize(
this,B.sn(Br.Get(I),Br.Get(0)));}return null;},QG:function(C){if(C<1)C=1;this.EE=
C;},QC:function(C){if(C<1)C=1;this.Qh=C;},Bs:function(C){if(C)this.By(0x10,0x0);
else this.By(0x0,0x10);},_Init:function(aArg){E.Lf._Init.call(this,aArg);this.__proto__=
E.GW;this.F=0x11B;},_Mark:function(D){var A;E.Lf._Mark.call(this,D);if((A=this.Qr
)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.FM)&&((A=A[0])._cycle!=D)
)A._Mark(A._cycle=D);if((A=this.FL)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((
A=this.K6)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.D3)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.CA)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);},_className:"Core::SimpleTouchHandler"};E.OB={Oq:null,Em:null,Dm:null,Fo:0,Di:
B.pm,KM:0,MB:0,Angle:0,Lg:1000,EE:8,CC:0,H$:-1,Down:false,Fb:false,Ej:false,Gw:false
,C7:function(AA,aClip,aOffset,Ah,aBlend){},Cj:function(Aq){var A;var Ak=(E.JR.isPrototypeOf(
Aq)?Aq:null);var Al=(E.JT.isPrototypeOf(Aq)?Aq:null);var T=AJ;var AQ=this.Angle;
var Bf;if(!Ak&&!Al)return null;if(!!Ak){this.Gw=Ak.Down;this.Down=Ak.Down;this.Fb=
B.se(this.H,Ak.DF);this.Ej=Ak.Ej;this.Di=Ak.Di;T=B.sn(Ak.DF,B.rZ(this.H));this.Fo=
Ak.CS;}if(!!Al){this.Fb=B.se(this.H,Al.DF);T=B.sn(Al.DF,B.rZ(this.H));this.Fo=Al.
CS;}var P=Math.sqrt((T[0]*T[0])+(T[1]*T[1]));if(P>=8.000000){AQ=(Math.acos(T[0]/
P)*B.sH)|0;if(T[1]>0)AQ=360-AQ;}Bf=AQ-this.Angle;if(Bf>180)Bf=Bf-360;if(Bf<-180)
Bf=Bf+360;this.Angle=AQ;this.KM=Bf;this.MB=this.MB+Bf;if((!!Ak&&Ak.Down)&&!Ak.CH
){this.KM=0;this.MB=0;(A=this.Dm)?A[1].call(A[0],this):null;}if(!!Bf)(A=this.Oq)?
A[1].call(A[0],this):null;if(!this.Down)(A=this.Em)?A[1].call(A[0],this):null;if(
!!this.CC){if(((((this.CC&0x10)===0x10)&&!!Ak)&&Ak.Down)&&(Ak.CH>=this.Lg))this.
El().ED(null,this,0x10);if((((this.CC&0x1)===0x1)&&!!Al)&&((Al.CT[1]-Al.DY[1])<=-
this.EE))this.El().ED(null,this,0x1);if((((this.CC&0x2)===0x2)&&!!Al)&&((Al.CT[1
]-Al.DY[1])>=this.EE))this.El().ED(null,this,0x2);if((((this.CC&0x4)===0x4)&&!!Al
)&&((Al.CT[0]-Al.DY[0])<=-this.EE))this.El().ED(null,this,0x4);if((((this.CC&0x8
)===0x8)&&!!Al)&&((Al.CT[0]-Al.DY[0])>=this.EE))this.El().ED(null,this,0x8);}return this;
},Ge:function(AB,V,DN,FX,Fm){var A;if(!!FX&&(FX!==this))return null;if((this.H$>=
0)&&(V!==this.H$))return null;if(this.Gw&&(V!==this.Fo))return null;if(!!(Fm&this.
CC))return null;var P=B.hV(AB,this.H);if(!((P[0]>=P[2])||(P[1]>=P[3]))){var Cc=B.
rZ(AB);var offset=AJ;if(Cc[0]<P[0])offset=[P[0]-Cc[0],offset[1]];if(Cc[0]>=P[2])
offset=[(P[2]-Cc[0])-1,offset[1]];if(Cc[1]<P[1])offset=[offset[0],P[1]-Cc[1]];if(
Cc[1]>=P[3])offset=[offset[0],(P[3]-Cc[1])-1];return B._NewObject(E.JS,0).Initialize(
this,offset);}return null;},_Init:function(aArg){E.AY._Init.call(this,aArg);this.
__proto__=E.OB;this.F=0x11B;},_Mark:function(D){var A;E.AY._Mark.call(this,D);if((
A=this.Oq)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Em)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Dm)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);},_className:"Core::RotateTouchHandler"};E.Im={timer:null,Nr:null,RA:null,Kw:null
,Ov:null,Em:null,Dm:null,Fo:0,GB:5000.000000,JJ:0,Na:B.pm,Jm:0,G8:0,AF:0,IP:0,KB:
0,Jl:0,G7:0,AE:0,IO:0,IN:0,Di:B.pm,KM:B.pm,H$:-1,A_:B.pm,A$:B.pm,As:B.pm,MH:B.pm
,Fh:B.pm,MG:B.pm,QO:5.000000,TL:500,Lg:1000,EE:8,CC:0,Oz:-1,QN:200,Hz:0,P7:0.500000
,TM:true,OE:true,OD:true,DL:false,FA:false,FB:false,Gw:false,C7:function(AA,aClip
,aOffset,Ah,aBlend){},Cj:function(Aq){var A;var Ak=(E.JR.isPrototypeOf(Aq)?Aq:null
);var Al=(E.JT.isPrototypeOf(Aq)?Aq:null);var Cf=(E.L7.isPrototypeOf(Aq)?Aq:null
);if(!!Cf&&((Cf.MI===this)||Cf.Ej))return null;if((!!Cf&&Cf.Down)&&(this.Gw||!this.
DL))return null;if((!!Cf&&Cf.Down)&&!B.se(this.H,this.G.Of(Cf.CT)))return null;if((
!!Cf&&!Cf.Down)&&(!this.Gw||(this.Fo!==Cf.CS)))return null;if((!Ak&&!Al)&&!Cf)return null;
if(!!Ak){this.Gw=Ak.Down;this.Fo=Ak.CS;this.Di=Ak.Di;}if(!!Al)this.Fo=Al.CS;if(!
!Cf){this.Gw=Cf.Down;this.Fo=Cf.CS;}if(!!Cf&&Cf.Down){this.PL();this.AE=(((Cf.Af-
this.IN)*0.001000)*this.G7)+this.AE;this.AF=(((Cf.Af-this.KB)*0.001000)*this.G8)+
this.AF;if(this.FA)this.AE=0.000000;if(this.FB)this.AF=0.000000;this.G7=0.000000;
this.G8=0.000000;this.FA=false;this.FB=false;this.Na=this.As;this.JJ=Cf.Af;return this;
}if((!!Ak&&Ak.Down)&&!Ak.CH){this.PL();this.AE=(((Ak.Af-this.IN)*0.001000)*this.
G7)+this.AE;this.AF=(((Ak.Af-this.KB)*0.001000)*this.G8)+this.AF;if(this.FA||!this.
DL)this.AE=0.000000;if(this.FB||!this.DL)this.AF=0.000000;this.G7=0.000000;this.
G8=0.000000;this.FA=false;this.FB=false;if(!this.DL){this.DL=true;(A=this.Dm)?A[
1].call(A[0],this):null;(A=this.Nr)?A[1].call(A[0],this):null;}this.Na=this.As;this.
JJ=Ak.Af;}if(!!Al){var Bf=B.sn(Al.DF,Al.Di);var Aj=this.As;if(this.OD)Aj=[this.Na[
0]+Bf[0],Aj[1]];if(this.OE)Aj=[Aj[0],this.Na[1]+Bf[1]];if(this.TM){if(Aj[0]<this.
A$[0])Aj=[this.A$[0]+(((Aj[0]-this.A$[0])/2)|0),Aj[1]];else if(Aj[0]>this.A_[0])
Aj=[this.A_[0]+(((Aj[0]-this.A_[0])/2)|0),Aj[1]];if(Aj[1]<this.A$[1])Aj=[Aj[0],this.
A$[1]+(((Aj[1]-this.A$[1])/2)|0)];else if(Aj[1]>this.A_[1])Aj=[Aj[0],this.A_[1]+(((
Aj[1]-this.A_[1])/2)|0)];}else{if(Aj[0]<this.A$[0])Aj=[this.A$[0],Aj[1]];else if(
Aj[0]>this.A_[0])Aj=[this.A_[0],Aj[1]];if(Aj[1]<this.A$[1])Aj=[Aj[0],this.A$[1]];
else if(Aj[1]>this.A_[1])Aj=[Aj[0],this.A_[1]];}if(!B.sa(Aj,this.As)){this.KM=B.
sn(Aj,this.As);this.As=Aj;(A=this.Kw)?A[1].call(A[0],this):null;(A=this.Ov)?A[1].
call(A[0],this):null;}}if((!!Cf&&!Cf.Down)&&((Cf.Af-this.JJ)>=(((A=this.QN)<0)?A+
0x100000000:A))){this.AE=0.000000;this.AF=0.000000;}if((!!Ak&&Ak.Down)&&((Ak.Af-
this.JJ)>=(((A=this.QN)<0)?A+0x100000000:A))){this.AE=0.000000;this.AF=0.000000;
}if((!!Ak&&!Ak.Down)&&(this.Oz>=0)){var CL=B.sn(Ak.DF,Ak.Di);if(((CL[0]*CL[0])+(
CL[1]*CL[1]))<=(this.Oz*this.Oz)){this.AE=0.000000;this.AF=0.000000;}}if(!!Al&&(
Al.Af>this.JJ)){var Bf=Al.As;var Rm=1000.000000/(Al.Af-this.JJ);var HS=0.000000;
var HT=0.000000;if(this.OD)HS=Bf[0]*Rm;if(this.OE)HT=Bf[1]*Rm;if((HS*this.AE)<0.000000
)this.AE=0.000000;if((HT*this.AF)<0.000000)this.AF=0.000000;this.AE=(this.AE+HS)
*0.500000;this.AF=(this.AF+HT)*0.500000;if(this.Hz>0.000000){if(this.AE>this.Hz)
this.AE=this.Hz;if(this.AE<-this.Hz)this.AE=-this.Hz;if(this.AF>this.Hz)this.AF=
this.Hz;if(this.AF<-this.Hz)this.AF=-this.Hz;}this.JJ=Al.Af;}if(!!this.CC){if(((((
this.CC&0x10)===0x10)&&!!Ak)&&Ak.Down)&&(Ak.CH>=this.Lg))this.El().ED(null,this,
0x10);if((((this.CC&0x1)===0x1)&&!!Al)&&((Al.CT[1]-Al.DY[1])<=-this.EE))this.El(
).ED(null,this,0x1);if((((this.CC&0x2)===0x2)&&!!Al)&&((Al.CT[1]-Al.DY[1])>=this.
EE))this.El().ED(null,this,0x2);if((((this.CC&0x4)===0x4)&&!!Al)&&((Al.CT[0]-Al.
DY[0])<=-this.EE))this.El().ED(null,this,0x4);if((((this.CC&0x8)===0x8)&&!!Al)&&((
Al.CT[0]-Al.DY[0])>=this.EE))this.El().ED(null,this,0x8);}if((!Ak||Ak.Down)&&!Cf
)return this;if(!!Ak&&Ak.Ej){this.AE=0.000000;this.AF=0.000000;}if((this.As[0]<=
this.A$[0])||(this.As[0]>=this.A_[0]))this.AE=0.000000;else if(this.AE===0.000000
){var AO=this.As[0];var AV=this.A$[0]+this.MH[0];var AL=this.A_[0]-this.MG[0];if(
AO<AV)AL=this.A$[0];else if(AO>AL)AV=this.A_[0];else if(this.Fh[0]<=1){AV=AO;AL=
AO;}else{var GE=(AL-AO)%this.Fh[0];AL=(AO-this.Fh[0])+GE;AV=AO+GE;if(AL<this.A$[
0])AL=this.A$[0];if(AV>this.A_[0])AV=this.A_[0];}if((AV-AO)<=(AO-AL))AO=AV;else AO=
AL;if(AO!==this.As[0]){var C3=AO-this.As[0];if(C3>0.000000)this.AE=Math.sqrt((C3
*2.000000)*this.GB)+20.000000;if(C3<0.000000)this.AE=-Math.sqrt((-C3*2.000000)*this.
GB)-20.000000;this.G7=(400.000000-(this.AE*this.AE))/(2.000000*C3);this.Jl=AO;}}
else{var VM=this.AE*this.AE;var C3=VM/(2.000000*this.GB);var AO=this.As[0];if(this.
AE>0.000000)AO=AO+(C3|0);if(this.AE<0.000000)AO=AO-(C3|0);if(AO>this.A_[0])AO=this.
A_[0];else if(AO<this.A$[0])AO=this.A$[0];var RR=AO;var AV=this.A$[0]+this.MH[0];
var AL=this.A_[0]-this.MG[0];if(AO<AV)AL=this.A$[0];else if(AO>AL)AV=this.A_[0];
else if(this.Fh[0]<=1){AV=AO;AL=AO;}else{var GE=(AL-AO)%this.Fh[0];AL=(AO-this.Fh[
0])+GE;AV=AO+GE;if(AL<this.A$[0])AL=this.A$[0];if(AV>this.A_[0])AV=this.A_[0];}if(
this.AE>0.000000)if(AL<=this.As[0])AO=AV;else if((AO-AL)<(AV-AO))AO=AL;else AO=AV;
else if(AV>=this.As[0])AO=AL;else if((AO-AL)>(AV-AO))AO=AV;else AO=AL;if(AO!==this.
As[0]){C3=AO-this.As[0];if(AO!==RR){var H0=AO-RR;if(H0>0.000000)this.AE=this.AE+
Math.sqrt((H0*2.000000)*this.GB);if(H0<0.000000)this.AE=this.AE-Math.sqrt((-H0*2.000000
)*this.GB);}if(this.AE>0.000000)this.AE=this.AE+20.000000;if(this.AE<0.000000)this.
AE=this.AE-20.000000;this.G7=(400.000000-(this.AE*this.AE))/(2.000000*C3);this.Jl=
AO;}else this.AE=0.000000;}if((this.As[1]<=this.A$[1])||(this.As[1]>=this.A_[1])
)this.AF=0.000000;else if(this.AF===0.000000){var AP=this.As[1];var AV=this.A$[1
]+this.MH[1];var AL=this.A_[1]-this.MG[1];if(AP<AV)AL=this.A$[1];else if(AP>AL)AV=
this.A_[1];else if(this.Fh[1]<=1){AV=AP;AL=AP;}else{var GE=(AL-AP)%this.Fh[1];AL=(
AP-this.Fh[1])+GE;AV=AP+GE;if(AL<this.A$[1])AL=this.A$[1];if(AV>this.A_[1])AV=this.
A_[1];}if((AV-AP)<=(AP-AL))AP=AV;else AP=AL;if(AP!==this.As[1]){var C3=AP-this.As[
1];if(C3>0.000000)this.AF=Math.sqrt((C3*2.000000)*this.GB)+20.000000;if(C3<0.000000
)this.AF=-Math.sqrt((-C3*2.000000)*this.GB)-20.000000;this.G8=(400.000000-(this.
AF*this.AF))/(2.000000*C3);this.Jm=AP;}}else{var VN=this.AF*this.AF;var C3=VN/(2.000000
*this.GB);var AP=this.As[1];if(this.AF>0.000000)AP=AP+(C3|0);if(this.AF<0.000000
)AP=AP-(C3|0);if(AP>this.A_[1])AP=this.A_[1];else if(AP<this.A$[1])AP=this.A$[1];
var RS=AP;var AV=this.A$[1]+this.MH[1];var AL=this.A_[1]-this.MG[1];if(AP<AV)AL=
this.A$[1];else if(AP>AL)AV=this.A_[1];else if(this.Fh[1]<=1){AV=AP;AL=AP;}else{
var GE=(AL-AP)%this.Fh[1];AL=(AP-this.Fh[1])+GE;AV=AP+GE;if(AL<this.A$[1])AL=this.
A$[1];if(AV>this.A_[1])AV=this.A_[1];}if(this.AF>0.000000)if(AL<=this.As[1])AP=AV;
else if((AP-AL)<(AV-AP))AP=AL;else AP=AV;else if(AV>=this.As[1])AP=AL;else if((AP-
AL)>(AV-AP))AP=AV;else AP=AL;if(AP!==this.As[1]){C3=AP-this.As[1];if(AP!==RS){var
H0=AP-RS;if(H0>0.000000)this.AF=this.AF+Math.sqrt((H0*2.000000)*this.GB);if(H0<0.000000
)this.AF=this.AF-Math.sqrt((-H0*2.000000)*this.GB);}if(this.AF>0.000000)this.AF=
this.AF+20.000000;if(this.AF<0.000000)this.AF=this.AF-20.000000;this.G8=(400.000000-(
this.AF*this.AF))/(2.000000*C3);this.Jm=AP;}else this.AF=0.000000;}if(!!Ak)this.
IN=Ak.Af;if(!!Cf)this.IN=Cf.Af;this.KB=this.IN;this.IO=this.As[0];this.IP=this.As[
1];this.VO();return this;},Ge:function(AB,V,DN,FX,Fm){var A;if(!!FX&&(FX!==this)
)return null;if((this.H$>=0)&&(V!==this.H$))return null;if(this.Gw&&(V!==this.Fo
))return null;if(!!(Fm&this.CC))return null;if(!this.OE&&!!(Fm&0x3))return null;
if(!this.OD&&!!(Fm&0xC))return null;var P=B.hV(AB,this.H);if(!((P[0]>=P[2])||(P[
1]>=P[3]))){var Cc=B.rZ(AB);var offset=AJ;if(Cc[0]<P[0])offset=[P[0]-Cc[0],offset[
1]];if(Cc[0]>=P[2])offset=[(P[2]-Cc[0])-1,offset[1]];if(Cc[1]<P[1])offset=[offset[
0],P[1]-Cc[1]];if(Cc[1]>=P[3])offset=[offset[0],(P[3]-Cc[1])-1];return B._NewObject(
E.JS,0).Initialize(this,offset);}return null;},PL:function(){if(!!this.timer){B.
rA([this,this.Ey],this.timer,0);this.timer=null;}},VO:function(){this.timer=B._GetAutoObject(
B.tg.JU);B.rl([this,this.Ey],this.timer,0);},Ey:function(Ae){var A;if(!this.timer
)return;var KC=(this.timer.Af-this.IN)*0.001000;var KD=(this.timer.Af-this.KB)*0.001000;
var VP=KC*KC;var VQ=KD*KD;var HS=(this.G7*KC)+this.AE;var HT=(this.G8*KD)+this.AF;
var Aj=[((((this.G7*0.500000)*VP)+(this.AE*KC))+this.IO)|0,((((this.G8*0.500000)
*VQ)+(this.AF*KD))+this.IP)|0];var Np=this.TL*0.001000;if(this.FA&&(KC>=Np)){Aj=[
this.Jl,Aj[1]];this.AE=0.000000;this.G7=0.000000;this.IO=Aj[0];this.FA=false;}else
if(this.FA){var A9=1.000000-Math.pow(1.000000-(KC/Np),this.QO);Aj=[(this.IO+((this.
Jl-this.IO)*A9))|0,Aj[1]];}if(this.FB&&(KD>=Np)){Aj=[Aj[0],this.Jm];this.AF=0.000000;
this.G8=0.000000;this.IP=Aj[1];this.FB=false;}else if(this.FB){var A9=1.000000-Math.
pow(1.000000-(KD/Np),this.QO);Aj=[Aj[0],(this.IP+((this.Jm-this.IP)*A9))|0];}if(((
this.AE>0.000000)&&(HS<0.000000))||((this.AE<0.000000)&&(HS>0.000000))){HS=0.000000;
Aj=[this.As[0],Aj[1]];}if(((this.AF>0.000000)&&(HT<0.000000))||((this.AF<0.000000
)&&(HT>0.000000))){HT=0.000000;Aj=[Aj[0],this.As[1]];}if(!this.FA&&(Aj[0]<this.A$[
0])){this.IO=Aj[0];this.Jl=this.A$[0];this.IN=this.timer.Af;this.FA=true;}else if(
!this.FA&&(Aj[0]>this.A_[0])){this.IO=Aj[0];this.Jl=this.A_[0];this.IN=this.timer.
Af;this.FA=true;}if(!this.FB&&(Aj[1]<this.A$[1])){this.IP=Aj[1];this.Jm=this.A$[
1];this.KB=this.timer.Af;this.FB=true;}else if(!this.FB&&(Aj[1]>this.A_[1])){this.
IP=Aj[1];this.Jm=this.A_[1];this.KB=this.timer.Af;this.FB=true;}if(((!this.FA&&(
this.AE!==0.000000))&&(HS>-20.000000))&&(HS<20.000000)){Aj=[this.Jl,Aj[1]];this.
AE=0.000000;this.G7=0.000000;this.IO=Aj[0];}if(((!this.FB&&(this.AF!==0.000000))&&(
HT>-20.000000))&&(HT<20.000000)){Aj=[Aj[0],this.Jm];this.AF=0.000000;this.G8=0.000000;
this.IP=Aj[1];}if(!B.sa(Aj,this.As)){this.KM=B.sn(Aj,this.As);this.As=Aj;(A=this.
Kw)?A[1].call(A[0],this):null;(A=this.Ov)?A[1].call(A[0],this):null;}if((((this.
AE===0.000000)&&(this.AF===0.000000))&&!this.FA)&&!this.FB){this.PL();this.DL=false;(
A=this.RA)?A[1].call(A[0],this):null;(A=this.Em)?A[1].call(A[0],this):null;}},FS:
function(C){if(C[0]<0)C=[0,C[1]];if(C[1]<0)C=[C[0],0];this.Fh=C;},J5:function(C){
if(C<0.000000)C=0.000000;if(C>1.000000)C=1.000000;if(C===this.P7)return;this.P7=
C;if(C<0.000000)C=0.000000;this.GB=C*10000.000000;},_Init:function(aArg){E.AY._Init.
call(this,aArg);this.__proto__=E.Im;this.F=0x11B;},_Mark:function(D){var A;E.AY.
_Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Nr)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.RA)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Kw)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.Ov)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Em)&&((A=A[0
])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Dm)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Core::SlideTouchHandler"};E.IW={U:null,D3:null,CA:null,K6:
null,JI:0,Af:0,QM:0,Fa:148,B$:0,A8:0,B0:true,Down:false,Oy:false,L5:false,Cj:function(
Aq){var A;if(!!Aq&&Aq.SN(this.Fa)){this.Down=Aq.Down;this.B$=Aq.B$;this.A8=Aq.A8;
this.Af=Aq.Af;this.L5=false;if(Aq.Down){this.QM=this.JI;this.Oy=this.JI>0;if(this.
Oy)(A=this.K6)?A[1].call(A[0],this):null;else(A=this.CA)?A[1].call(A[0],this):null;
if(!this.L5)this.JI=this.JI+1;return!this.L5;}if(!Aq.Down){this.Oy=this.JI>1;this.
QM=this.JI-1;this.JI=0;(A=this.D3)?A[1].call(A[0],this):null;return!this.L5;}}return false;
},CU:function(aArg){var A;var Bw=(E.K.isPrototypeOf(A=this.N)?A:null);if(!Bw)throw new
Error(Gq);this.U=Bw.Nf;Bw.Nf=this;},_Init:function(aArg){this.__proto__=E.IW;this.
CU(aArg);B.gb++;},_Done:function(){this.__proto__=E.IW;B.gb--;},_ReInit:function(
){},_Mark:function(D){var A;if((A=this.U)&&(A._cycle!=D))A._Mark(A._cycle=D);if((
A=this.D3)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.CA)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.K6)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:
null,_className:"Core::KeyPressHandler"};E.JS={AD:null,KN:0,As:B.pm,Initialize:function(
J,aOffset){this.AD=J;this.As=aOffset;this.KN=(aOffset[0]*aOffset[0])+(aOffset[1]
*aOffset[1]);return this;},_Init:function(aArg){this.__proto__=E.JS;B.gb++;},_Done:
function(){this.__proto__=E.JS;B.gb--;},_ReInit:function(){},_Mark:function(D){var
A;if((A=this.AD)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D)
)A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"Core::CursorHit"
};E.Qk={Bw:null,_Init:function(aArg){this.__proto__=E.Qk;B.gb++;},_Done:function(
){this.__proto__=E.Qk;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Bw)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D))A._Mark(
A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"Core::ModalContext"};E.
IX={IH:null,CM:B.pn,W:B.pn,isEmpty:false,_Init:function(aArg){this.__proto__=E.IX;
B.gb++;},_Done:function(){this.__proto__=E.IX;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.IH)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.N
)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:
"Core::LayoutContext"};E.Mo={HW:B.pm,HV:B.pm,_Init:function(aArg){E.IX._Init.call(
this,aArg);this.__proto__=E.Mo;},_className:"Core::LayoutLineContext"};E.Mp={LU:
B.pm,LT:B.pm,HW:B.pm,HV:B.pm,_Init:function(aArg){E.IX._Init.call(this,aArg);this.
__proto__=E.Mp;},_className:"Core::LayoutQuadContext"};E.L9={Bw:null,U:null,G$:null
,HY:null,Ev:null,LR:null,_Init:function(aArg){this.__proto__=E.L9;B.gb++;},_Done:
function(){this.__proto__=E.L9;B.gb--;},_ReInit:function(){},_Mark:function(D){var
A;if((A=this.Bw)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.U)&&(A._cycle!=D)
)A._Mark(A._cycle=D);if((A=this.G$)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
HY)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ev)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.LR)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D
))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"Core::DialogContext"
};E.OG={PN:null,Eq:null,Az:null,Am:null,Qq:null,Nd:false,U_:function(){if(!this.
Eq)return;var E6=this.Eq;this.Eq.E5=null;this.Eq=null;this.PN=null;if(this.Nd)B.
kD([this,this.Ru],this);else B.kD([this,this.Nm],this);E6.Qp(this);},Nm:function(
Ae){if(!!this.Eq)return;if(!this.Am){B.kD(this.Qq,this);return;}this.Eq=this.Am;
this.Am=this.Am.U;if(!!this.Am)this.Am.X=null;else this.Az=null;this.Eq.U=null;this.
Nd=true;this.Eq.Dm(this);this.Nd=false;},Ru:function(Ae){B.kD([this,this.Nm],this
);},R5:function(BV){if(!BV||!BV.E5)return;if(BV.E5!==this)throw new Error(OV);var
RZ=false;if(this.Eq===BV){this.Eq=null;this.PN=null;RZ=true;if(this.Nd)B.kD([this
,this.Ru],this);else B.kD([this,this.Nm],this);}else{if(!!BV.U)BV.U.X=BV.X;else this.
Az=BV.X;if(!!BV.X)BV.X.U=BV.U;else this.Am=BV.U;BV.X=null;BV.U=null;}BV.E5=null;
if(RZ)BV.Qo(this);},TQ:function(BV,U8){if(!BV)return;if(!!BV.E5)throw new Error(
OW);BV.E5=this;if(U8){BV.U=this.Am;if(!!this.Am)this.Am.X=BV;else this.Az=BV;this.
Am=BV;}else{BV.X=this.Az;if(!!this.Az)this.Az.U=BV;else this.Am=BV;this.Az=BV;}if(
!this.Eq)B.kD([this,this.Nm],this);},_Init:function(aArg){this.__proto__=E.OG;B.
gb++;},_Done:function(){this.__proto__=E.OG;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.PN)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Eq
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Az)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Am)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Qq)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null
,_cycle:0,_observers:null,_className:"Core::TaskQueue"};E.Ke={E5:null,X:null,U:null
,Qp:function(Lr){},Qo:function(Lr){},Dm:function(Lr){this.PZ();},Gd:function(){if(
!!this.E5&&(this.E5.Eq===this))this.E5.U_();},PZ:function(){if(!!this.E5)this.E5.
R5(this);},N3:function(){return!!this.E5&&(this.E5.Eq===this);},_Init:function(aArg
){this.__proto__=E.Ke;B.gb++;},_Done:function(){this.__proto__=E.Ke;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.E5)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.X)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.U)&&(A._cycle!=D)
)A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:
0,_observers:null,_className:"Core::Task"};E.Il={resource:null,DX:function(){this.
resource=null;},CU:function(aArg){this.resource=aArg;},_Init:function(aArg){this.
__proto__=E.Il;this.CU(aArg);B.gb++;},_Done:function(){this.__proto__=E.Il;this.
DX();B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.N)&&(A._cycle
!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"Core::Resource"
};E.Timer={Ii:null,timer:null,Af:0,Period:1000,E9:0,B0:false,DX:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},Nt:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.rz(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},La:function(C){if(C<0)C=0;if(C===this.Period)return;
this.Period=C;if(this.B0)this.Nt(this.E9,C);},If:function(C){if(C<0)C=0;if(C===this.
E9)return;this.E9=C;if(this.B0)this.Nt(C,this.Period);},Bs:function(C){if(C===this.
B0)return;this.B0=C;if(C)this.Nt(this.E9,this.Period);else this.Nt(0,0);this.Af=
this.KX();},KX:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk
)|0;return ticksCount;},Trigger:function(){var A;this.Af=this.KX();if(!this.Period
)this.Bs(false);(A=this.Ii)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=E.Timer;B.gb++;},_Done:function(){this.__proto__=E.Timer;this.DX();B.gb--;
},_ReInit:function(){},_Mark:function(D){var A;if((A=this.Ii)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null
,_cycle:0,_observers:null,_className:"Core::Timer"};E.Af={Lh:0,K3:0,KZ:0,KL:0,K4:
0,OI:0,Vi:function(FZ,O0){if(O0)switch(FZ){case 1:return B.te.Qb;case 2:return B.
te.P6;case 3:return B.te.Qg;case 4:return B.te.PX;case 5:return B.te.Qi;case 6:return B.
te.Qd;case 7:return B.te.Qc;case 8:return B.te.PY;case 9:return B.te.QP;case 10:
return B.te.Qm;case 11:return B.te.Ql;case 12:return B.te.P1;default:return B.g3;
}else switch(FZ){case 1:return B.te.SO;case 2:return B.te.St;case 3:return B.te.
SZ;case 4:return B.te.R1;case 5:return B.te.S0;case 6:return B.te.SQ;case 7:return B.
te.SP;case 8:return B.te.R2;case 9:return B.te.TR;case 10:return B.te.S6;case 11:
return B.te.S5;case 12:return B.te.R7;default:return B.g3;}},Vh:function(Q2,O0){
if(O0)switch(Q2){case 1:return B.te.S3;case 2:return B.te.T0;case 3:return B.te.
T2;case 4:return B.te.TX;case 5:return B.te.Sv;case 6:return B.te.TO;case 0:return B.
te.TU;default:return B.g3;}else switch(Q2){case 1:return B.te.S2;case 2:return B.
te.TZ;case 3:return B.te.T1;case 4:return B.te.TW;case 5:return B.te.Su;case 6:return B.
te.TN;case 0:return B.te.TT;default:return B.g3;}},Vg:function(){var result;result=((
new Date).getTime()/1000)|0;return result;},O$:function(FZ){switch(FZ){case 0:return 0;
case 1:return 31;case 2:return 60;case 3:return 91;case 4:return 121;case 5:return 152;
case 6:return 182;case 7:return 213;case 8:return 244;case 9:return 274;case 10:
return 305;case 11:return 335;case 12:return 366;default:return 0;}},O_:function(
FZ){switch(FZ){case 0:return 0;case 1:return 31;case 2:return 59;case 3:return 90;
case 4:return 120;case 5:return 151;case 6:return 181;case 7:return 212;case 8:return 243;
case 9:return 273;case 10:return 304;case 11:return 334;case 12:return 365;default:
return 0;}},Qt:function(){var DV=this.OI;var Ji;if(!!(DV%4)){DV=DV-1970;Ji=(DV*365
)+(((DV+1)/4)|0);Ji=((Ji+this.O_(this.K4-1))+this.KL)-1;}else{DV=DV-1970;Ji=(DV*
365)+(((DV+1)/4)|0);Ji=((Ji+this.O$(this.K4-1))+this.KL)-1;}return((((((((A=Ji)<
0)?A+0x100000000:A)*60)*60)*24)+(((((A=this.KZ)<0)?A+0x100000000:A)*60)*60))+((((
A=this.K3)<0)?A+0x100000000:A)*60))+(((A=this.Lh)<0)?A+0x100000000:A);},TG:function(
C){var Es=(((C/86400)|0)+365)|0;var R0=(Es/1461)|0;var DV;var Hd;Es=Es-(R0*1461);
DV=(Es/365)|0;if(DV===4)DV=3;Es=Es-(DV*365);DV=((R0*4)+DV)+1969;Hd=Es>>5;if(!!(DV
%4)){if(Es>=this.O_(Hd+1))Hd=Hd+1;Es=Es-this.O_(Hd);}else{if(Es>=this.O$(Hd+1))Hd=
Hd+1;Es=Es-this.O$(Hd);}this.My(DV);this.Mw(Hd+1);this.I0(Es+1);this.Mu((((C/3600
)|0)%24)|0);this.Mv((((C/60)|0)%60)|0);this.Mx((C%60)|0);},Qs:function(){return B.
_NewObject(E.Af,0).Initialize(this.Vg());},S9:function(){return((((this.Qt()/86400
)|0)+4)%7)|0;},Mx:function(C){if(C<0)C=0;if(C>59)C=59;this.Lh=C;},Mv:function(C){
if(C<0)C=0;if(C>59)C=59;this.K3=C;},Mu:function(C){if(C<0)C=0;if(C>23)C=23;this.
KZ=C;},I0:function(C){if(C<1)C=1;if(C>31)C=31;this.KL=C;},Mw:function(C){if(C<1)
C=1;if(C>12)C=12;this.K4=C;},My:function(C){if(C<1970)C=1970;this.OI=C;},Initialize:
function(Q7){this.TG(Q7);return this;},_Init:function(aArg){this.__proto__=E.Af;
B.gb++;},_Done:function(){this.__proto__=E.Af;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:
0,_observers:null,_className:"Core::Time"};E.GT={Oo:null,Av:null,B0:true,Rs:function(
Ae){var A;(A=this.Oo)?A[1].call(A[0],this):null;},I1:function(C){if(B.sc(this.Av
,C))return;if(!!this.Av&&this.B0)B.rC([this,this.Rs],this.Av,0);this.Av=C;if(!!this.
Av&&this.B0)B.rn([this,this.Rs],this.Av,0);},_Init:function(aArg){this.__proto__=
E.GT;B.gb++;},_Done:function(){this.__proto__=E.GT;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Oo)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle
!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"Core::PropertyObserver"
};E.Xh={BO:0x1,V2:0x2,Wx:0x4,Xe:0x8,B0:0x10,Hw:0x20,Wy:0x40,WG:0x80,Wv:0x100,WB:
0x200,Wo:0x400,WV:0x800,G4:0x1000,Xg:0x2000,WM:0x4000,WN:0x8000,Wh:0x10000,WL:0x20000
,W3:0x40000};E.D1={WW:0x1,WX:0x2,VZ:0x4,V0:0x8,V1:0x10,VY:0x20};E.Mc={S4:0,Xd:1,
V$:2,WC:3,WZ:4};E.IU={S4:0,Xb:1,Left:2,V9:3,TY:4,R4:5,Xc:6,Right:7,V_:8};E.KeyCode={
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
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};E.WY={Xl:0x1,Xi:0x2,Xj:0x4,Xk:0x8,WE:
0x10};
E._Init=function(){E.JZ.__proto__=E.AD;E.Lf.__proto__=E.AD;E.AY.__proto__=E.AD;E.
K.__proto__=E.AY;E.Root.__proto__=E.K;E.KeyEvent.__proto__=E.Event;E.L7.__proto__=
E.Event;E.JR.__proto__=E.Event;E.JT.__proto__=E.Event;E.Lb.__proto__=E.AY;E.HB.__proto__=
E.K;E.GW.__proto__=E.Lf;E.OB.__proto__=E.AY;E.Im.__proto__=E.AY;E.Mo.__proto__=E.
IX;E.Mp.__proto__=E.IX;};E.Dy=function(D){};return E;})();

/* Embedded Wizard */