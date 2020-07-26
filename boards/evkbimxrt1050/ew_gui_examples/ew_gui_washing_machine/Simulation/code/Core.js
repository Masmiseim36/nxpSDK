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
var AX=[0,0];var BI=[0,0,0,0];var Dz="The view does not belong to this group";var
Gb="No view to remove";var Gc="View is not in this group";var Gd="No view to add";
var Ge="View already in a group";var J8="Recursive invalidate during active update cycle.";
var J9="The method Remove() is not available in Core::VerticalList.";var J_="The method Add() is not available in Core::VerticalList.";
var J$="The Slide Touch Handler:";var Ka="is already connected with a view.";var
Kb="The Slide Touch Handler is already connected with a view to scroll "+"its content.";
var Kc=[0,0,170,120];var II="The KeyPressHandler is embedded within an object not being derived "+
"from Core::Group.";
C.AB={A$:null,AQ:null,I:null,Ba:null,F:0x103,HB:0,Fq:0x14,HQ:function(Ao,IQ){},AT:
function(E){var A;var AE=E^this.Fq;if(!AE)return;this.Fq=E;if(!!this.Ba&&!((this.
F&0x400)===0x400)){this.I.F=this.I.F|0x5000;B.lq([A=this.I,A.C9],this);this.I.AF([
0,0,(A=this.I.H)[2]-A[0],A[3]-A[1]]);}if(!!this.Ba&&((this.F&0x400)===0x400)){this.
Ba.Gv.F=this.Ba.Gv.F|0x1000;this.I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this);
}},Ef:function(){var Ap=this.I;while(!!Ap){var LY=(C.Root.isPrototypeOf(Ap)?Ap:null
);if(!!LY)return LY;Ap=Ap.I;}return null;},C$:function(A_,aClip,aOffset,A7,aBlend
){},B2:function(Az){return null;},EB:function(AI,Q,CR,E0,Em){return null;},Ia:function(
Ao,DA){return AX;},Im:function(aOffset,IN){},GetExtent:function(){return BI;},Cw:
function(E1,Gh){var A;if(((this.F&0x200)===0x200))E1=E1&~0x400;var KF=(this.F&~Gh
)|E1;var Fu=KF^this.F;this.F=KF;if(!!this.I&&!!(Fu&0x14)){var LJ=((this.F&0x14)===
0x14);if(LJ&&!this.I.Db)this.I.Ej(this);if(!LJ&&(this.I.Db===this))this.I.Ej(this.
I.JG(this,0x14));}if(!!this.I&&!!(Fu&0x403))this.I.AF(this.GetExtent());if(((!!this.
Ba&&!!this.I)&&((KF&0x400)===0x400))&&((Fu&0x1)===0x1)){this.F=this.F|0x800;this.
I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this);}if(!!this.I&&((Fu&0x400)===0x400
)){this.Ba=null;this.F=this.F|0x800;this.I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9
],this);}},_Init:function(aArg){this.__proto__=C.AB;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.
A$)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.AQ)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.I)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ba)&&(A._cycle!=D
))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,
_cycle:0,_observers:null,_className:"Core::View"};C.Hz={Cb:B.qx,CG:B.qx,Ca:B.qx,
CF:B.qx,HQ:function(Ao,IQ){var At=B._NewObject(C.Ik,0);this.Ba=null;At.CY=this.GetExtent(
);At.AC=Ao;At.Gv=IQ;At.HZ=this.CF;At.H0=this.Ca;At.H1=this.CG;At.H2=this.Cb;this.
Ba=At;},Ia:function(Ao,DA){var A;var BZ=this.Fq;var At=(C.Ik.isPrototypeOf(A=this.
Ba)?A:null);var Y=At.CY[0];var Z=At.CY[1];var Ab=At.CY[2];var Ac=At.CY[3];var CX=[
Ao[2]-Ao[0],Ao[3]-Ao[1]];var Bp=Ab-Y;var Be=Ac-Z;if(!DA){var DW=[(A=At.AC)[2]-A[
0],A[3]-A[1]];Y=Y-At.AC[0];Z=Z-At.AC[1];if(DW[0]!==CX[0]){var Dl=((BZ&0x4)===0x4
);var Dm=((BZ&0x8)===0x8);var Ew=((BZ&0x1)===0x1);if(!Dl&&(Ew||!Dm))Y=((Y*CX[0])
/DW[0])|0;if(!Dm&&(Ew||!Dl)){Ab=Ab-At.AC[0];Ab=((Ab*CX[0])/DW[0])|0;Ab=Ab-CX[0];
}else Ab=Ab-At.AC[2];Y=Y+Ao[0];Ab=Ab+Ao[2];if(!Ew){if(Dl&&!Dm)Ab=Y+Bp;else if(!Dl&&
Dm)Y=Ab-Bp;else{Y=Y+((((Ab-Y)-Bp)/2)|0);Ab=Y+Bp;}}}else{Ab=Ab-At.AC[2];Y=Y+Ao[0];
Ab=Ab+Ao[2];}if(DW[1]!==CX[1]){var Dn=((BZ&0x10)===0x10);var Dk=((BZ&0x20)===0x20
);var Ex=((BZ&0x2)===0x2);if(!Dn&&(Ex||!Dk))Z=((Z*CX[1])/DW[1])|0;if(!Dk&&(Ex||!
Dn)){Ac=Ac-At.AC[1];Ac=((Ac*CX[1])/DW[1])|0;Ac=Ac-CX[1];}else Ac=Ac-At.AC[3];Z=Z+
Ao[1];Ac=Ac+Ao[3];if(!Ex){if(Dn&&!Dk)Ac=Z+Be;else if(!Dn&&Dk)Z=Ac-Be;else{Z=Z+((((
Ac-Z)-Be)/2)|0);Ac=Z+Be;}}}else{Ac=Ac-At.AC[3];Z=Z+Ao[1];Ac=Ac+Ao[3];}}else{switch(
DA){case 3:{Y=Ao[0];Ab=Y+Bp;}break;case 4:{Ab=Ao[2];Y=Ab-Bp;}break;case 1:{Z=Ao[
1];Ac=Z+Be;}break;case 2:{Ac=Ao[3];Z=Ac-Be;}break;default:;}if((DA===3)||(DA===4
)){var Dn=((BZ&0x10)===0x10);var Dk=((BZ&0x20)===0x20);var Ex=((BZ&0x2)===0x2);if(
Ex){Z=Ao[1];Ac=Ao[3];}else if(Dn&&!Dk){Z=Ao[1];Ac=Z+Be;}else if(Dk&&!Dn){Ac=Ao[3
];Z=Ac-Be;}else{Z=Ao[1]+((((Ao[3]-Ao[1])-Be)/2)|0);Ac=Z+Be;}}if((DA===1)||(DA===
2)){var Dl=((BZ&0x4)===0x4);var Dm=((BZ&0x8)===0x8);var Ew=((BZ&0x1)===0x1);if(Ew
){Y=Ao[0];Ab=Ao[2];}else if(Dl&&!Dm){Y=Ao[0];Ab=Y+Bp;}else if(Dm&&!Dl){Ab=Ao[2];
Y=Ab-Bp;}else{Y=Ao[0]+((((Ao[2]-Ao[0])-Bp)/2)|0);Ab=Y+Bp;}}}At.isEmpty=(Y>=Ab)||(
Z>=Ac);Bp=(Ab-Y)-1;Be=(Ac-Z)-1;var E9=At.CY[0];var E_=At.CY[1];var EL=(At.CY[2]-
E9)-1;var EK=(At.CY[3]-E_)-1;if(!EL)EL=1;if(!EK)EK=1;if(((this.F&0x100)===0x100)
){this.CF=[Y+((((At.HZ[0]-E9)*Bp)/EL)|0),Z+((((At.HZ[1]-E_)*Be)/EK)|0)];this.Ca=[
Y+((((At.H0[0]-E9)*Bp)/EL)|0),Z+((((At.H0[1]-E_)*Be)/EK)|0)];this.CG=[Y+((((At.H1[
0]-E9)*Bp)/EL)|0),Z+((((At.H1[1]-E_)*Be)/EK)|0)];this.Cb=[Y+((((At.H2[0]-E9)*Bp)
/EL)|0),Z+((((At.H2[1]-E_)*Be)/EK)|0)];}else{this.ES([Y+((((At.HZ[0]-E9)*Bp)/EL)|
0),Z+((((At.HZ[1]-E_)*Be)/EK)|0)]);this.ET([Y+((((At.H0[0]-E9)*Bp)/EL)|0),Z+((((
At.H0[1]-E_)*Be)/EK)|0)]);this.EU([Y+((((At.H1[0]-E9)*Bp)/EL)|0),Z+((((At.H1[1]-
E_)*Be)/EK)|0)]);this.EV([Y+((((At.H2[0]-E9)*Bp)/EL)|0),Z+((((At.H2[1]-E_)*Be)/EK
)|0)]);this.Ba=At;}return[Bp+1,Be+1];},Im:function(aOffset,IN){if(IN){this.CF=B.
tx(this.CF,aOffset);this.Ca=B.tx(this.Ca,aOffset);this.CG=B.tx(this.CG,aOffset);
this.Cb=B.tx(this.Cb,aOffset);}else{this.ES(B.tx(this.CF,aOffset));this.ET(B.tx(
this.Ca,aOffset));this.EU(B.tx(this.CG,aOffset));this.EV(B.tx(this.Cb,aOffset));
}},GetExtent:function(){if(!!this.Ba&&this.Ba.isEmpty)return BI;var Y=this.CF[0];
var Z=this.CF[1];var Ab=this.CG[0];var Ac=this.CG[1];if((((this.Cb[0]!==Y)||(this.
Ca[1]!==Z))||(this.Ca[0]!==Ab))||(this.Cb[1]!==Ac)){if(this.Ca[0]<Y)Y=this.Ca[0];
if(this.CG[0]<Y)Y=this.CG[0];if(this.Cb[0]<Y)Y=this.Cb[0];if(this.Ca[1]<Z)Z=this.
Ca[1];if(this.CG[1]<Z)Z=this.CG[1];if(this.Cb[1]<Z)Z=this.Cb[1];if(this.CF[0]>Ab
)Ab=this.CF[0];if(this.Ca[0]>Ab)Ab=this.Ca[0];if(this.Cb[0]>Ab)Ab=this.Cb[0];if(
this.CF[1]>Ac)Ac=this.CF[1];if(this.Ca[1]>Ac)Ac=this.Ca[1];if(this.Cb[1]>Ac)Ac=this.
Cb[1];}else{var tmp;if(Ab<Y){tmp=Y;Y=Ab;Ab=tmp;}if(Ac<Z){tmp=Z;Z=Ac;Ac=tmp;}}return[
Y,Z,Ab+1,Ac+1];},EV:function(E){var A;if(B.tl(E,this.Cb))return;if(!!this.I&&((this.
F&0x1)===0x1))this.I.AF(this.GetExtent());this.Ba=null;this.Cb=E;if(!!this.I&&((
this.F&0x1)===0x1))this.I.AF(this.GetExtent());if((!!this.I&&((this.F&0x400)===0x400
))&&!((this.I.F&0x2000)===0x2000)){this.F=this.F|0x800;this.I.F=this.I.F|0x4000;
B.lq([A=this.I,A.C9],this);}},EU:function(E){var A;if(B.tl(E,this.CG))return;if(
!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.GetExtent());this.Ba=null;this.CG=
E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.GetExtent());if((!!this.I&&((
this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this);}},ET:function(E){var A;if(B.tl(E
,this.Ca))return;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.GetExtent());this.
Ba=null;this.Ca=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.GetExtent());
if((!!this.I&&((this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this);}},ES:function(E){var
A;if(B.tl(E,this.CF))return;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.GetExtent(
));this.Ba=null;this.CF=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.GetExtent(
));if((!!this.I&&((this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this);}},JJ:function(
El){var Bg=B.tA(4,B.qx,null);var K=0;var Bi=3;var Kt=false;var Ku=false;Bg.Set(0
,this.CF);Bg.Set(1,this.Ca);Bg.Set(2,this.CG);Bg.Set(3,this.Cb);while(K<4){var L5=
Bg.Get(K)[0];var Js=Bg.Get(K)[1];var OS=Bg.Get(Bi)[0];var KQ=Bg.Get(Bi)[1];if(((
Js>El[1])!==(KQ>El[1]))||((Js<El[1])!==(KQ<El[1]))){var L6=((((OS-L5)*(El[1]-Js)
)/(KQ-Js))|0)+L5;if(El[0]>L6)Kt=!Kt;if(El[0]<L6)Ku=!Ku;}Bi=K;K=K+1;}return Kt||Ku;
},Ml:function(){return((((this.CF[0]===this.Cb[0])&&(this.Ca[0]===this.CG[0]))&&(
this.CF[1]===this.Ca[1]))&&(this.CG[1]===this.Cb[1]))||((((this.CF[0]===this.Ca[
0])&&(this.CG[0]===this.Cb[0]))&&(this.CF[1]===this.Cb[1]))&&(this.Ca[1]===this.
CG[1]));},_Init:function(aArg){C.AB._Init.call(this,aArg);this.__proto__=C.Hz;},
_className:"Core::QuadView"};C.Bq={H:B.qy,HQ:function(Ao,IQ){var At=B._NewObject(
C.Ij,0);At.CY=this.H;At.AC=Ao;At.Gv=IQ;this.Ba=At;},Ia:function(Ao,DA){var A;var
BZ=this.Fq;var At=this.Ba;var Y=At.CY[0];var Z=At.CY[1];var Ab=At.CY[2];var Ac=At.
CY[3];var CX=[Ao[2]-Ao[0],Ao[3]-Ao[1]];var Bp=Ab-Y;var Be=Ac-Z;if(!DA){var DW=[(
A=At.AC)[2]-A[0],A[3]-A[1]];Y=Y-At.AC[0];Z=Z-At.AC[1];if(DW[0]!==CX[0]){var Dl=((
BZ&0x4)===0x4);var Dm=((BZ&0x8)===0x8);var Ew=((BZ&0x1)===0x1);if(!Dl&&(Ew||!Dm)
)Y=((Y*CX[0])/DW[0])|0;if(!Dm&&(Ew||!Dl)){Ab=Ab-At.AC[0];Ab=((Ab*CX[0])/DW[0])|0;
Ab=Ab-CX[0];}else Ab=Ab-At.AC[2];Y=Y+Ao[0];Ab=Ab+Ao[2];if(!Ew){if(Dl&&!Dm)Ab=Y+Bp;
else if(!Dl&&Dm)Y=Ab-Bp;else{Y=Y+((((Ab-Y)-Bp)/2)|0);Ab=Y+Bp;}}}else{Ab=Ab-At.AC[
2];Y=Y+Ao[0];Ab=Ab+Ao[2];}if(DW[1]!==CX[1]){var Dn=((BZ&0x10)===0x10);var Dk=((BZ&
0x20)===0x20);var Ex=((BZ&0x2)===0x2);if(!Dn&&(Ex||!Dk))Z=((Z*CX[1])/DW[1])|0;if(
!Dk&&(Ex||!Dn)){Ac=Ac-At.AC[1];Ac=((Ac*CX[1])/DW[1])|0;Ac=Ac-CX[1];}else Ac=Ac-At.
AC[3];Z=Z+Ao[1];Ac=Ac+Ao[3];if(!Ex){if(Dn&&!Dk)Ac=Z+Be;else if(!Dn&&Dk)Z=Ac-Be;else{
Z=Z+((((Ac-Z)-Be)/2)|0);Ac=Z+Be;}}}else{Ac=Ac-At.AC[3];Z=Z+Ao[1];Ac=Ac+Ao[3];}}else{
switch(DA){case 3:{Y=Ao[0];Ab=Y+Bp;}break;case 4:{Ab=Ao[2];Y=Ab-Bp;}break;case 1:{
Z=Ao[1];Ac=Z+Be;}break;case 2:{Ac=Ao[3];Z=Ac-Be;}break;default:;}if((DA===3)||(DA===
4)){var Dn=((BZ&0x10)===0x10);var Dk=((BZ&0x20)===0x20);var Ex=((BZ&0x2)===0x2);
if(Ex){Z=Ao[1];Ac=Ao[3];}else if(Dn&&!Dk){Z=Ao[1];Ac=Z+Be;}else if(Dk&&!Dn){Ac=Ao[
3];Z=Ac-Be;}else{Z=Ao[1]+((((Ao[3]-Ao[1])-Be)/2)|0);Ac=Z+Be;}}if((DA===1)||(DA===
2)){var Dl=((BZ&0x4)===0x4);var Dm=((BZ&0x8)===0x8);var Ew=((BZ&0x1)===0x1);if(Ew
){Y=Ao[0];Ab=Ao[2];}else if(Dl&&!Dm){Y=Ao[0];Ab=Y+Bp;}else if(Dm&&!Dl){Ab=Ao[2];
Y=Ab-Bp;}else{Y=Ao[0]+((((Ao[2]-Ao[0])-Bp)/2)|0);Ab=Y+Bp;}}}At.isEmpty=(Y>=Ab)||(
Z>=Ac);if(((this.F&0x100)===0x100)){this.H=[Y,Z,Ab,Ac];}else{this.M([Y,Z,Ab,Ac]);
this.Ba=At;}return[Ab-Y,Ac-Z];},Im:function(aOffset,IN){if(IN)this.H=B.tz(this.H
,aOffset);else this.M(B.tz(this.H,aOffset));},GetExtent:function(){return this.H;
},M:function(E){var A;if(B.tm(E,this.H))return;if(!!this.I&&((this.F&0x1)===0x1)
)this.I.AF(this.H);this.Ba=null;this.H=E;if(!!this.I&&((this.F&0x1)===0x1))this.
I.AF(this.H);if((!!this.I&&((this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000
)){this.F=this.F|0x800;this.I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this);}},Oh:
function(KP){this.M(KP);},Mt:function(){return this.H;},_Init:function(aArg){C.AB.
_Init.call(this,aArg);this.__proto__=C.Bq;},_className:"Core::RectView"};C.L={BR:
null,Bn:null,I_:null,DB:null,Fv:null,Gr:null,Db:null,Ar:255,C$:function(A_,aClip
,aOffset,A7,aBlend){var A;A7=((A7+1)*this.Ar)>>8;aBlend=aBlend&&((this.F&0x2)===
0x2);if(!this.DB)this.Os(A_,aClip,B.tx(aOffset,this.H.slice(0,2)),A7,aBlend);else{
var CU=255|(255<<8)|(255<<16)|((A7&0xFF)<<24);this.DB.Update();A_.KR(aClip,this.
DB,0,B.tz(this.H,aOffset),AX,CU,CU,CU,CU,aBlend);}},EB:function(AI,Q,CR,E0,Em){var
A;var O=this.Bn;var Gs=null;var W=BI;var BL=null;var KD=!!this.Gr&&(!!this.Gr.Ko||
!!this.Gr.BR);if(((A=B.il(AI,this.H))[0]>=A[2])||(A[1]>=A[3]))return null;AI=B.ty(
AI,this.H.slice(0,2));while(!!O){if(((O.F&0x400)===0x400)&&!BL){BL=O.AQ;while(!!
BL&&!((BL.F&0x200)===0x200))BL=BL.AQ;if(!!BL)W=B.il(AI,BL.GetExtent());else W=BI;
}if(BL===O){BL=null;W=BI;}if((((((O.F&0x8)===0x8)&&((O.F&0x10)===0x10))&&!((O.F&
0x40000)===0x40000))&&!((O.F&0x20000)===0x20000))&&(!((O.F&0x10000)===0x10000)||((
this.Fv.Df===O)&&!KD))){var CY=O.GetExtent();var I4=E0;var Gn=null;if(I4===O)I4=
null;if(((O.F&0x400)===0x400)){if(!(((A=B.il(CY,W))[0]>=A[2])||(A[1]>=A[3])))Gn=
O.EB(W,Q,CR,I4,Em);}else{if(!(((A=B.il(CY,AI))[0]>=A[2])||(A[1]>=A[3]))||(E0===O
))Gn=O.EB(AI,Q,CR,I4,Em);}O=O.AQ;if(!!Gn){if(!Gs||((Gn.Hi<Gs.Hi)&&(Gn.Hi>=0)))Gs=
Gn;if(!Gn.Hi)O=null;}}else O=O.AQ;}return Gs;},Cw:function(E1,Gh){var A;var Oy=this.
F;C.Bq.Cw.call(this,E1,Gh);var Fu=this.F^Oy;if(!!this.Db&&((Fu&0x40)===0x40)){if(((
this.F&0x40)===0x40))this.Db.Cw(0x40,0x0);else this.Db.Cw(0x0,0x40);}if(!!this.Fv&&((
Fu&0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.Fv.Df.F&0x14)===0x14))this.Fv.
Df.Cw(0x40,0x0);else this.Fv.Df.Cw(0x0,0x40);}if(!!Fu){this.F=this.F|0x8000;B.lq([
this,this.C9],this);}},M:function(E){var A;if(B.tm(E,this.H))return;var FB=[(A=this.
H)[2]-A[0],A[3]-A[1]];var Jc=[E[2]-E[0],E[3]-E[1]];var Gy=!B.tl(FB,Jc);if(Gy&&!!
this.DB){this.DB.JT(Jc);B.qw(this,0);B.qw(this.DB,0);}C.Bq.M.call(this,E);if((Gy&&(
FB[0]>0))&&(FB[1]>0)){var AC=[].concat(AX,FB);var O=this.BR;var HP=0x14;while(!!
O){if((!O.Ba&&(O.Fq!==HP))&&!((O.F&0x400)===0x400))O.HQ(AC,null);O=O.A$;}}if(Gy){
this.F=this.F|0x5000;B.lq([this,this.C9],this);}},KH:function(Az){var LN=(C.KeyEvent.
isPrototypeOf(Az)?Az:null);var Ec=this.I_;if(!LN)return null;while(!!Ec&&(!Ec.Cq||
!Ec.B2(LN)))Ec=Ec.A$;return Ec;},Os:function(A_,aClip,aOffset,A7,aBlend){var A;var
O=this.BR;var LE=BI;var LT=true;while(!!O){if(((O.F&0x200)===0x200)){var LS=(C.Hw.
isPrototypeOf(O)?O:null);LE=B.il(aClip,B.tz(LS.H,aOffset));LT=((LS.F&0x1)===0x1);
}if(((O.F&0x1)===0x1)){if(((O.F&0x400)===0x400)){if(LT){var W=B.il(B.tz(O.GetExtent(
),aOffset),LE);if(!((W[0]>=W[2])||(W[1]>=W[3])))O.C$(A_,W,aOffset,A7,aBlend);}}else{
var W=B.il(B.tz(O.GetExtent(),aOffset),aClip);if(!((W[0]>=W[2])||(W[1]>=W[3])))O.
C$(A_,W,aOffset,A7,aBlend);}}O=O.A$;}},OG:function(){var A;var Kr=((this.F&0x1000
)===0x1000);var E$=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];var Ea=false;var Er=BI;var
B6=BI;var Eb=AX;var G8=0;var G9=0;var G7=0;var CZ=0;var O=this.Bn;var BL=null;var
HP=0x14;var Fy=null;while(!!O){if(((O.F&0x800)===0x800)){Ea=true;O.F=O.F&~0x800;
}if(Ea&&((O.F&0x200)===0x200)){Ea=false;if(!!(C.Hw.isPrototypeOf(O)?O:null).Ig)O.
F=O.F|0x1000;}O=O.AQ;}Ea=false;O=this.BR;if(Kr){this.F=this.F&~0x1000;Kr=!((E$[0
]>=E$[2])||(E$[1]>=E$[3]));}this.F=this.F|0x2000;while(!!O){if(!Fy&&(G7!==CZ)){var
Ez=O;var Jl=0;var H6=Er[2]-Er[0];var HM=Er[3]-Er[1];var I1=0;var GA=AX;do{if(((Ez.
F&0x200)===0x200))Ez=null;else if(((Ez.F&0x401)===0x401)){GA=[(A=Ez.GetExtent())[
2]-A[0],A[3]-A[1]];if((CZ===3)||(CZ===4))H6=H6-GA[0];if((CZ===1)||(CZ===2))HM=HM-
GA[1];if(!Fy||((H6>=0)&&(HM>=0))){Fy=Ez;Ez=Ez.A$;if((CZ===3)||(CZ===4)){H6=H6-G8;
if(GA[1]>Jl)Jl=GA[1];}if((CZ===1)||(CZ===2)){HM=HM-G9;if(GA[0]>I1)I1=GA[0];}}else
Ez=null;}else Ez=Ez.A$;}while(!!Ez);if(!Fy)Fy=BL;B6=Er;switch(G7){case 9:case 11:
B6=[].concat(B6.slice(0,3),B6[1]+Jl);break;case 10:case 12:B6=B.t3(B6,B6[3]-Jl);
break;case 5:case 7:B6=B.t1(B6,B6[0]+I1);break;case 6:case 8:B6=[].concat(B6[2]-
I1,B6.slice(1,4));break;default:;}}if(((O.F&0x400)===0x400)){if(!!O.Ba&&(O.Ba.Gv
!==BL))O.Ba=null;if((!O.Ba&&Ea)&&((O.Fq!==HP)||!!CZ))O.HQ(B6,BL);}if(!!O.Ba){if(
Kr&&!((O.F&0x400)===0x400))O.Ia(E$,0);if(Ea&&((O.F&0x400)===0x400)){var BT=O.Ia(
B.tz(B6,Eb),CZ);if(((O.F&0x1)===0x1)){var A9=AX;switch(CZ){case 3:A9=[BT[0]+G8,A9[
1]];break;case 4:A9=[-BT[0]-G8,A9[1]];break;case 1:A9=[A9[0],BT[1]+G9];break;case
2:A9=[A9[0],-BT[1]-G9];break;default:;}Eb=B.tx(Eb,A9);}}}if(((O.F&0x200)===0x200
)){if(Ea)B.lq(BL.BQ,BL);Ea=((O.F&0x1000)===0x1000);BL=(C.Hw.isPrototypeOf(O)?O:null
);if(Ea){O.F=O.F&~0x1000;Er=B.tz(BL.H,BL.AD);B6=Er;Eb=AX;G7=BL.Ig;CZ=G7;G8=BL.Space+
BL.M0;G9=BL.Space+BL.M1;Ea=!((Er[0]>=Er[2])||(Er[1]>=Er[3]));Fy=null;switch(G7){
case 9:case 10:CZ=3;break;case 11:case 12:CZ=4;break;case 5:case 6:CZ=1;break;case
7:case 8:CZ=2;break;default:;}}if(Ea){this.AF(BL.H);}}if(O===Fy){switch(G7){case
9:case 11:Eb=[0,(Eb[1]+(B6[3]-B6[1]))+G9];break;case 10:case 12:Eb=[0,(Eb[1]-(B6[
3]-B6[1]))-G9];break;case 5:case 7:Eb=[(Eb[0]+(B6[2]-B6[0]))+G8,0];break;case 6:
case 8:Eb=[(Eb[0]-(B6[2]-B6[0]))-G8,0];break;default:;}Fy=null;}O=O.A$;}if(Ea)B.
lq(BL.BQ,BL);this.F=this.F&~0x2000;this.Lc([E$[2]-E$[0],E$[3]-E$[1]]);},C9:function(
Ai){var A;var OR=((this.F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000)){this.
F=this.F&~0x4000;this.OG();}if(((this.F&0x8000)===0x8000)||OR){this.F=this.F&~0x8000;
this.J5(this.F);}},Ej:function(E){var A;if(!!E&&(E.I!==this))throw new Error(Dz);
if(!!E&&!((E.F&0x14)===0x14))E=null;if(!!E&&((E.F&0x10000)===0x10000))E=null;if(
E===this.Db)return;if(!!this.Db)this.Db.Cw(0x0,0x60);this.Db=E;if(!!E){if(((this.
F&0x40)===0x40))E.Cw(0x60,0x0);else E.Cw(0x20,0x0);}},A4:function(E){var A;if(E>
255)E=255;if(E<0)E=0;if(E===this.Ar)return;this.Ar=E;if(!!this.I&&((this.F&0x1)===
0x1))this.I.AF(this.H);},EF:function(KP){this.A4(KP);},Hr:function(El){var tmp=this;
while(!!tmp){El=B.tw(El,tmp.H.slice(0,2));tmp=tmp.I;}return El;},DispatchEvent:function(
Az){var A;var O=this.Db;var Ap=(C.L.isPrototypeOf(O)?O:null);var AY=null;var KD=
!!this.Gr&&(!!this.Gr.Ko||!!this.Gr.BR);if(!!O&&((((O.F&0x10000)===0x10000)||((O.
F&0x40000)===0x40000))||((O.F&0x20000)===0x20000))){O=null;Ap=null;}if(!!this.Fv&&
!KD)AY=this.Fv.Df.DispatchEvent(Az);if(!AY&&!!Ap)AY=Ap.DispatchEvent(Az);else if(
!AY&&!!O)AY=O.B2(Az);if(!AY)AY=this.B2(Az);if(!AY)AY=this.KH(Az);return AY;},BroadcastEventAtPosition:
function(Az,Lz,aFilter){var A;var O=this.Bn;var AY=null;while(!!O&&!AY){if((!aFilter||((
A=aFilter)&&((O.F&A)===A)))&&B.qu(O.GetExtent(),Lz)){var Ap=(C.L.isPrototypeOf(O
)?O:null);if(!!Ap)AY=Ap.BroadcastEventAtPosition(Az,B.tw(Lz,Ap.H.slice(0,2)),aFilter
);else AY=O.B2(Az);}O=O.AQ;}if(!AY)AY=this.B2(Az);return AY;},BroadcastEvent:function(
Az,aFilter){var A;var O=this.Bn;var AY=null;while(!!O&&!AY){if(!aFilter||((A=aFilter
)&&((O.F&A)===A))){var Ap=(C.L.isPrototypeOf(O)?O:null);if(!!Ap)AY=Ap.BroadcastEvent(
Az,aFilter);else AY=O.B2(Az);}O=O.AQ;}if(!AY)AY=this.B2(Az);if(!AY)AY=this.KH(Az
);return AY;},Lc:function(aSize){},J5:function(LA){},GJ:function(){this.F=this.F|
0x8000;B.lq([this,this.C9],this);},AF:function(AI){var A;var Ap=this;while(!!Ap&&
!((AI[0]>=AI[2])||(AI[1]>=AI[3]))){var Gl=Ap.DB;if(!Ap.I&&(Ap!==this)){Ap.AF(AI);
return;}if(!!Gl){var Kq=false;var Ox=Gl.D0;if(Kq)Gl.D0=[0,0,(A=Ap.H)[2]-A[0],A[3
]-A[1]];else Gl.D0=B.qR(Gl.D0,AI);if(!B.tm(Ox,Gl.D0)){B.qw(Ap,0);B.qw(Gl,0);}}if(
!((Ap.F&0x1)===0x1))return;AI=B.il(B.tz(AI,Ap.H.slice(0,2)),Ap.H);Ap=Ap.I;}},CP:
function(aArg){this.GJ();},JG:function(Am,aFilter){var A;if(!Am||(Am.I!==this))return null;
var FA=Am.A$;var FC=Am.AQ;var HW=0x10000;if(((aFilter&0x10000)===0x10000))HW=0x0;
while(!!FA||!!FC){if((!!FA&&(!aFilter||((A=aFilter)&&((FA.F&A)===A))))&&(!HW||!((
A=HW)&&((FA.F&A)===A))))return FA;if((!!FC&&(!aFilter||((A=aFilter)&&((FC.F&A)===
A))))&&(!HW||!((A=HW)&&((FC.F&A)===A))))return FC;if(!!FA)FA=FA.A$;if(!!FC)FC=FC.
AQ;}return null;},K9:function(Am){var A;if(!Am)throw new Error(Gb);if(Am.I!==this
)throw new Error(Gc);if((((Am.F&0x401)===0x401)&&!!Am.AQ)&&!!Am.Ba){Am.AQ.F=Am.AQ.
F|0x800;this.F=this.F|0x4000;B.lq([this,this.C9],this);}if(((Am.F&0x200)===0x200
)){if(!!Am.AQ)Am.AQ.F=Am.AQ.F|0x800;this.F=this.F|0x4000;B.lq([this,this.C9],this
);}Am.Ba=null;if(this.Db===Am)this.Ej(this.JG(Am,0x14));if(!!Am.AQ)Am.AQ.A$=Am.A$;
if(!!Am.A$)Am.A$.AQ=Am.AQ;if(this.BR===Am)this.BR=Am.A$;if(this.Bn===Am)this.Bn=
Am.AQ;Am.I=null;Am.A$=null;Am.AQ=null;if(((Am.F&0x1)===0x1))this.AF(Am.GetExtent(
));},R:function(Am,Gj){var A;if(!Am)throw new Error(Gd);if(!!Am.I)throw new Error(
Ge);var CT=null;var Jq=Am.HB;if(((Gj<0)&&!!this.Bn)&&(this.Bn.HB>=Jq)){CT=this.Bn;
Gj=Gj+1;}while((((Gj<0)&&!!CT)&&!!CT.AQ)&&(CT.AQ.HB>=Jq)){CT=CT.AQ;Gj=Gj+1;}if((
!CT&&!!this.Bn)&&(this.Bn.HB>Jq))CT=this.Bn;while((!!CT&&!!CT.AQ)&&(CT.AQ.HB>Jq)
)CT=CT.AQ;if(!CT){Am.I=this;Am.AQ=this.Bn;if(!!this.Bn)this.Bn.A$=Am;if(!this.BR
)this.BR=Am;this.Bn=Am;}else{Am.I=this;Am.AQ=CT.AQ;Am.A$=CT;CT.AQ=Am;if(!!Am.AQ)
Am.AQ.A$=Am;else this.BR=Am;}if(((Am.F&0x1)===0x1))this.AF(Am.GetExtent());if(((
!this.Db&&((Am.F&0x4)===0x4))&&((Am.F&0x10)===0x10))&&!((Am.F&0x10000)===0x10000
))this.Ej(Am);if(((Am.F&0x401)===0x401)){Am.F=Am.F|0x800;this.F=this.F|0x4000;B.
lq([this,this.C9],this);}if(((Am.F&0x200)===0x200)){Am.F=Am.F|0x800;this.F=this.
F|0x4000;B.lq([this,this.C9],this);}},Cr:function(){return this.Ar;},_Init:function(
aArg){C.Bq._Init.call(this,aArg);this.__proto__=C.L;this.F=0x1F;this.CP(aArg);},
_Mark:function(D){var A;C.Bq._Mark.call(this,D);if((A=this.BR)&&(A._cycle!=D))A.
_Mark(A._cycle=D);if((A=this.Bn)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.I_
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DB)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Fv)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Gr)&&(A._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Db)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:
"Core::Group"};C.Root={C2:null,Fb:null,En:null,Bf:B.tA(10,null,null),G4:null,DL:
null,E5:null,H_:0,Kk:0,Bh:0,B5:B.tA(10,0,null),I3:B.tA(10,B.qy,null),DQ:B.tA(10,
0,null),EJ:B.tA(10,B.qx,null),G6:B.tA(10,0,null),E4:B.tA(10,B.qx,null),DP:B.tA(10
,B.qx,null),De:B.tA(10,B.qx,null),Eo:B.tA(10,B.qx,null),AP:0,I6:0,I5:0,Ja:B.tA(3
,B.qy,null),LP:0,CA:B.tA(4,0,null),B0:B.tA(4,B.qy,null),BS:0,Fl:8,Mf:250,Fx:0,Fa:
0,Ks:true,I$:false,Ef:function(){return this;},C$:function(A_,aClip,aOffset,A7,aBlend
){var fullScreenUpdate=false;fullScreenUpdate=B.jI;if(!fullScreenUpdate)A_.JF(aClip
,B.tz(B.tz(aClip,aOffset),this.H.slice(0,2)),0x00000000,0x00000000,0x00000000,0x00000000
,false);C.L.C$.call(this,A_,aClip,aOffset,A7,aBlend);},Cw:function(E1,Gh){var A;
C.L.Cw.call(this,E1,Gh);if(!this.I&&(((E1&0x1)===0x1)||((Gh&0x1)===0x1)))this.AF([
0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);if(!this.I&&(((E1&0x2)===0x2)||((Gh&0x2)===0x2
)))this.AF([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},Ej:function(E){if((E!==this.DL)||
!E)C.L.Ej.call(this,E);},A4:function(E){var A;var Oz=this.Ar;C.L.A4.call(this,E);
if(((Oz!==this.Ar)&&!this.I)&&((this.F&0x1)===0x1))this.AF([0,0,(A=this.H)[2]-A[
0],A[3]-A[1]]);},DispatchEvent:function(Az){if((this.Kk>0)&&!!(C.KeyEvent.isPrototypeOf(
Az)?Az:null))return null;if(!!Az){Az.Hn=!!this.Bh;if(!!this.Bh)Az.AJ=this.Bh;}var
AY=null;if(!!this.DL){AY=this.DL.DispatchEvent(Az);if(!!AY){this.Bh=0;return AY;
}}if(!!this.Fb){AY=this.Fb.Df.DispatchEvent(Az);if(!AY)AY=this.B2(Az);if(!AY)AY=
this.KH(Az);this.Bh=0;return AY;}AY=C.L.DispatchEvent.call(this,Az);this.Bh=0;return AY;
},BroadcastEvent:function(Az,aFilter){if(!!Az){Az.Hn=!!this.Bh;if(!!this.Bh)Az.AJ=
this.Bh;}var AY=C.L.BroadcastEvent.call(this,Az,aFilter);this.Bh=0;return AY;},AF:
function(AI){var A;if(this.H_>0)throw new Error(J8);if(!!this.DB&&!this.I){if(((
A=this.DB.D0)[0]>=A[2])||(A[1]>=A[3])){B.qw(this,0);B.qw(this.DB,0);}var Kq=false;
if(Kq)this.DB.D0=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];else this.DB.D0=B.qR(this.DB.
D0,AI);}var fullScreenUpdate=false;fullScreenUpdate=B.jI;if(fullScreenUpdate)AI=[
0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(!!this.I){C.L.AF.call(this,AI);return;}AI=B.
il(B.tz(AI,this.H.slice(0,2)),this.H);if((AI[0]>=AI[2])||(AI[1]>=AI[3]))return;var
K;for(K=0;K<this.BS;K=K+1)if(!(((A=B.il(this.B0.Get(K),AI))[0]>=A[2])||(A[1]>=A[
3]))){this.B0.Set(K,B.qR(this.B0.Get(K),AI));this.CA.Set(K,B.s9(this.B0.Get(K)));
return;}if(this.BS<3){this.B0.Set(this.BS,AI);this.CA.Set(this.BS,B.s9(AI));this.
BS=this.BS+1;return;}var Bi;var C1;var HT=0;var HU=0;var LB=2147483647;this.B0.Set(
this.BS,AI);this.CA.Set(this.BS,B.s9(AI));for(Bi=0;Bi<=this.BS;Bi=Bi+1)for(C1=Bi+
1;C1<=this.BS;C1=C1+1){var Jr=B.s9(B.qR(this.B0.Get(Bi),this.B0.Get(C1)));var LU=((
Jr<<8)/(this.CA.Get(Bi)+this.CA.Get(C1)))|0;if(LU<LB){LB=LU;HT=Bi;HU=C1;}}this.B0.
Set(HT,B.qR(this.B0.Get(HT),this.B0.Get(HU)));this.CA.Set(HT,B.s9(this.B0.Get(HT
)));if(HU!==this.BS){this.B0.Set(HU,this.B0.Get(this.BS));this.CA.Set(HU,this.CA.
Get(this.BS));}},Oq:function(){var Cl=B._NewObject(C.Hj,0);Cl.Hn=!!this.Bh;if(!!
this.Bh)Cl.AJ=this.Bh;return Cl;},Gm:function(){var Cl=B._NewObject(C.Hg,0);Cl.Hn=
!!this.Bh;if(!!this.Bh)Cl.AJ=this.Bh;return Cl;},G5:function(){var Cl=B._NewObject(
C.Hh,0);Cl.Hn=!!this.Bh;if(!!this.Bh)Cl.AJ=this.Bh;return Cl;},Or:function(Ai){var
K;var Gs=false;for(K=0;K<10;K=K+1)if(!!this.Bf.Get(K)){var A5=this.De.Get(K);var
Ap=this.Bf.Get(K).I;while(!!Ap&&(Ap!==this)){A5=B.tw(A5,Ap.H.slice(0,2));Ap=Ap.I;
}if(!Ap&&(this.Bf.Get(K)!==this)){var tmp=this.Bf.Get(K);this.AP=K;this.Bf.Set(K
,null);tmp.B2(this.Gm().InitializeUp(K,this.E4.Get(K),this.EJ.Get(K),this.DQ.Get(
K),this.B5.Get(K)+1,this.DP.Get(K),false,this.De.Get(K),this.Eo.Get(K)));if(tmp===
this.En)this.En=null;this.BroadcastEvent(this.G5().InitializeUp(K,this.B5.Get(K)+
1,false,tmp,this.De.Get(K)),0x18);}else{this.DQ.Set(K,(this.E5.AJ-this.G6.Get(K)
)|0);if(this.DQ.Get(K)<10)this.DQ.Set(K,10);this.AP=K;this.Bf.Get(K).B2(this.Gm(
).InitializeHold(K,A5,this.EJ.Get(K),this.DQ.Get(K),this.B5.Get(K)+1,this.DP.Get(
K),this.De.Get(K),this.Eo.Get(K)));Gs=true;}}if(!Gs)this.E5.Aj(false);},GetFPS:function(
){var ticksCount=0;var LK=0;ticksCount=((new Date).getTime()-B.qt)|0;if(!!this.I6&&(
ticksCount>this.I6))LK=((this.I5*1000)/((ticksCount-this.I6)|0))|0;this.I5=0;this.
I6=ticksCount;return LK;},Update:function(){var A;if(!this.G4){this.G4=B._NewObject(
B.Graphics.Canvas,0);this.G4.JT([(A=this.H)[2]-A[0],A[3]-A[1]]);}this.G4.Update(
);return this.UpdateGE20(this.G4);},UpdateGE20:function(A_){if(!this.BeginUpdate(
))return BI;var EN=this.UpdateCanvas(A_,AX);this.EndUpdate();return EN;},EndUpdate:
function(){if(this.BS>0){this.I5=this.I5+1;this.BS=0;}},UpdateCanvas:function(A_
,aOffset){var A;var EN=BI;var Om=[].concat(aOffset,B.tx(A_.FrameSize,aOffset));var
K;var Bi=this.BS;this.H_=this.H_+1;for(K=0;(K<Bi)&&(K<4);K=K+1){if(this.CA.Get(K
)>0){this.C$(A_,B.ty(this.B0.Get(K),aOffset),[-aOffset[0],-aOffset[1]],255,true);
EN=B.qR(EN,B.il(Om,this.B0.Get(K)));}else Bi=Bi+1;}this.H_=this.H_-1;if(!((EN[0]>=
EN[2])||(EN[1]>=EN[3])))return B.ty(EN,aOffset);else return EN;},GetUpdateRegion:
function(IR){var K;var Bi=this.BS;if(IR<0)return BI;for(K=0;(K<Bi)&&(K<4);K=K+1){
if(!this.CA.Get(K)){Bi=Bi+1;IR=IR+1;}else if(K===IR)return this.B0.Get(K);}return BI;
},BeginUpdate:function(){var OF=true;var fullScreenUpdate=false;var K;if((!OF&&!
fullScreenUpdate)&&(this.BS>0)){var L3=B.tA(3,B.qy,null);var KN=this.BS;for(K=0;
K<KN;K=K+1)L3.Set(K,this.B0.Get(K));for(K=0;K<this.LP;K=K+1)this.AF(this.Ja.Get(
K));for(K=0;K<KN;K=K+1)this.Ja.Set(K,L3.Get(K));this.LP=KN;}var Bi;var C1;for(Bi=
0;Bi<(this.BS-1);Bi=Bi+1)if(this.CA.Get(Bi)>0)for(C1=Bi+1;C1<this.BS;C1=C1+1)if(
this.CA.Get(C1)>0){var Jr=B.s9(B.qR(this.B0.Get(Bi),this.B0.Get(C1)));if(((Jr-this.
CA.Get(Bi))-this.CA.Get(C1))<0){this.B0.Set(Bi,B.qR(this.B0.Get(Bi),this.B0.Get(
C1)));this.CA.Set(Bi,Jr);this.CA.Set(C1,0);}}for(K=this.BS-1;K>=0;K=K-1)if(!this.
CA.Get(K))this.BS=this.BS-1;return this.BS;},DoesNeedUpdate:function(){if(this.BS>
0)return true;return false;},Initialize:function(aSize){this.M([].concat(AX,aSize
));if(this.Ks)this.F=this.F|0x60;else this.F=this.F|0x20;this.AF(this.H);return this;
},SetRootFocus:function(Kh){if(Kh===this.Ks)return false;this.Ks=Kh;if(!Kh){if(!
!this.DL)this.DL.Cw(0x0,0x40);if(!!this.Fb)this.Fb.Df.Cw(0x0,0x40);else this.Cw(
0x0,0x40);}else{if(!!this.Fb)this.Fb.Df.Cw(0x40,0x0);else this.Cw(0x40,0x0);if(!
!this.DL)this.DL.Cw(0x40,0x0);}return true;},SetUserInputTimestamp:function(Ol){
this.Bh=Ol;},DriveKeyboardHitting:function(BJ,Gg,Dd){var A;var KI=!!this.C2;if(!
!this.C2&&((!Dd||(this.Fx!==BJ))||(this.Fa!==Gg))){var Cl=null;var O=(C.AB.isPrototypeOf(
A=this.C2)?A:null);var Ec=(C.Ii.isPrototypeOf(A=this.C2)?A:null);if(!!this.Fx)Cl=
B._NewObject(C.KeyEvent,0).Initialize(this.Fx,false);if(this.Fa!==0x00)Cl=B._NewObject(
C.KeyEvent,0).Initialize2(this.Fa,false);if(!!Ec)Ec.B2(Cl);else if(!!O)O.B2(Cl);
this.Fx=0;this.Fa=0x00;this.C2=null;}if(!!this.C2){var Cl=null;var O=(C.AB.isPrototypeOf(
A=this.C2)?A:null);var Ec=(C.Ii.isPrototypeOf(A=this.C2)?A:null);if(!!BJ)Cl=B._NewObject(
C.KeyEvent,0).Initialize(BJ,true);if(this.Fa!==0x00)Cl=B._NewObject(C.KeyEvent,0
).Initialize2(Gg,true);if(!!Ec)Ec.B2(Cl);else if(!!O)O.B2(Cl);}if(this.I$&&((!Dd||(
this.Fx!==BJ))||(this.Fa!==Gg))){this.Fx=0;this.Fa=0x00;this.I$=false;}if((!this.
C2&&Dd)&&(this.Kk>0)){this.Fx=BJ;this.Fa=Gg;this.I$=true;}if((!this.C2&&Dd)&&!this.
I$){if(!!BJ)this.C2=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize(BJ,
true));if(Gg!==0x00)this.C2=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize2(
Gg,true));if(!(C.Ii.isPrototypeOf(A=this.C2)?A:null)&&!(C.AB.isPrototypeOf(A=this.
C2)?A:null))this.C2=null;this.Fx=BJ;this.Fa=Gg;KI=KI||!!this.C2;}this.Bh=0;return KI;
},DriveCursorMovement:function(BK){return this.DriveMultiTouchMovement(this.AP,BK
);},DriveMultiTouchMovement:function(Q,BK){if((Q<0)||(Q>9)){this.Bh=0;return false;
}var Bo=B.tw(BK,this.De.Get(Q));this.De.Set(Q,BK);if(!this.Bf.Get(Q)||B.tl(Bo,AX
)){this.Bh=0;return false;}var A5=BK;var Ap=this.Bf.Get(Q).I;while(!!Ap&&(Ap!==this
)){A5=B.tw(A5,Ap.H.slice(0,2));Ap=Ap.I;}if(!Ap&&(this.Bf.Get(Q)!==this)){var tmp=
this.Bf.Get(Q);this.AP=Q;this.Bf.Set(Q,null);tmp.B2(this.Gm().InitializeUp(Q,this.
E4.Get(Q),this.EJ.Get(Q),this.DQ.Get(Q),this.B5.Get(Q)+1,this.DP.Get(Q),false,this.
De.Get(Q),this.Eo.Get(Q)));if(tmp===this.En)this.En=null;this.BroadcastEvent(this.
G5().InitializeUp(Q,this.B5.Get(Q)+1,false,tmp,BK),0x18);}else{this.E4.Set(Q,A5);
this.AP=Q;this.Bf.Get(Q).B2(this.Oq().Initialize(Q,A5,this.EJ.Get(Q),Bo,this.DQ.
Get(Q),this.B5.Get(Q)+1,this.DP.Get(Q),BK,this.Eo.Get(Q)));}this.Bh=0;return true;
},DriveCursorHitting:function(Dd,Q,BK){return this.DriveMultiTouchHitting(Dd,Q,BK
);},DriveMultiTouchHitting:function(Dd,Q,BK){var A;if((Q<0)||(Q>9)){this.Bh=0;return false;
}var ticksCount=this.Bh;var Go=[].concat([-this.Fl,-this.Fl],[this.Fl+1,this.Fl+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.qt)|0;}var OL=this.Bh;this.
DriveMultiTouchMovement(Q,BK);BK=this.De.Get(Q);this.Bh=OL;if(Dd)this.Eo.Set(Q,BK
);if((Dd&&!this.Bf.Get(Q))&&!this.Kk){var C0=null;var A5=BK;if(B.qu(this.I3.Get(
Q),BK)&&((ticksCount-this.G6.Get(Q))<=(((A=this.Mf)<0)?A+0x100000000:A)))this.B5.
Set(Q,this.B5.Get(Q)+1);else this.B5.Set(Q,0);this.I3.Set(Q,B.tz(Go,BK));this.G6.
Set(Q,ticksCount);if((!!this.DL&&!!this.DL.I)&&((this.DL.F&0x18)===0x18)){var W=
B.tz(Go,this.DL.I.Hr(BK));C0=this.DL.EB(W,Q,this.B5.Get(Q)+1,null,0x0);}if(!C0){
if(!!this.En&&!!this.En.I){if(((this.En.F&0x8)===0x8)&&((this.En.F&0x10)===0x10)
){var W=B.tz(Go,this.En.I.Hr(BK));C0=this.En.EB(W,Q,this.B5.Get(Q)+1,null,0x0);}
}else if(!!this.Fb)C0=this.EB(B.tz(Go,BK),Q,this.B5.Get(Q)+1,this.Fb.Df,0x0);else
C0=this.EB(B.tz(Go,BK),Q,this.B5.Get(Q)+1,null,0x0);}if(!!C0){this.BroadcastEvent(
this.G5().InitializeDown(Q,this.B5.Get(Q)+1,false,C0.AB,BK),0x18);this.Bf.Set(Q,
C0.AB);this.DP.Set(Q,C0.AL);}else{this.Bf.Set(Q,null);this.DP.Set(Q,AX);this.Bh=
0;return false;}var Ap=C0.AB.I;while(!!Ap&&(Ap!==this)){A5=B.tw(A5,Ap.H.slice(0,
2));Ap=Ap.I;}this.EJ.Set(Q,A5);this.E4.Set(Q,A5);this.DQ.Set(Q,0);this.E5.Aj(true
);this.AP=Q;this.Bf.Get(Q).B2(this.Gm().InitializeDown(Q,A5,this.B5.Get(Q)+1,this.
DP.Get(Q),false,BK));this.Bh=0;return true;}if(!Dd&&!!this.Bf.Get(Q)){var A5=BK;
var Ap=this.Bf.Get(Q).I;while(!!Ap&&(Ap!==this)){A5=B.tw(A5,Ap.H.slice(0,2));Ap=
Ap.I;}if(!Ap)A5=this.E4.Get(Q);this.AP=Q;var tmp=this.Bf.Get(Q);this.Bf.Set(Q,null
);tmp.B2(this.Gm().InitializeUp(Q,A5,this.EJ.Get(Q),this.DQ.Get(Q),this.B5.Get(Q
)+1,this.DP.Get(Q),false,BK,this.Eo.Get(Q)));this.BroadcastEvent(this.G5().InitializeUp(
Q,this.B5.Get(Q)+1,false,tmp,BK),0x18);this.Bh=0;return true;}this.Bh=0;return false;
},Iv:function(EG,Lx,Em){var Go=[].concat([-this.Fl,-this.Fl],[this.Fl+1,this.Fl+
1]);if(EG===this)EG=null;if(!this.Bf.Get(this.AP))return;var C0;C0=this.EB(B.tz(
Go,this.De.Get(this.AP)),this.AP,1,EG,Em);if(!!C0&&(this.Bf.Get(this.AP)!==C0.AB
))this.Fm(C0.AB,C0.AL);if(!C0&&(this.Bf.Get(this.AP)!==Lx))this.Fm(Lx,AX);},Fm:function(
EG,DM){if(!this.Bf.Get(this.AP)||(this.Bf.Get(this.AP)===EG))return;var tmp=this.
Bf.Get(this.AP);this.Bf.Set(this.AP,null);tmp.B2(this.Gm().InitializeUp(this.AP,
this.E4.Get(this.AP),this.EJ.Get(this.AP),this.DQ.Get(this.AP),this.B5.Get(this.
AP)+1,this.DP.Get(this.AP),true,this.De.Get(this.AP),this.Eo.Get(this.AP)));this.
BroadcastEvent(this.G5().InitializeUp(this.AP,this.B5.Get(this.AP)+1,true,tmp,this.
De.Get(this.AP)),0x18);var A5=this.De.Get(this.AP);var Ap=null;if(!!EG)Ap=EG.I;while(
!!Ap&&(Ap!==this)){A5=B.tw(A5,Ap.H.slice(0,2));Ap=Ap.I;}if(!Ap&&(EG!==this)){this.
Bf.Set(this.AP,null);return;}this.BroadcastEvent(this.G5().InitializeDown(this.AP
,this.B5.Get(this.AP)+1,true,EG,this.De.Get(this.AP)),0x18);var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;this.Bf.Set(this.AP,EG);this.DP.Set(this.AP,DM);this.
EJ.Set(this.AP,A5);this.E4.Set(this.AP,A5);this.B5.Set(this.AP,0);this.DQ.Set(this.
AP,0);this.G6.Set(this.AP,ticksCount);this.Eo.Set(this.AP,this.De.Get(this.AP));
this.Bf.Get(this.AP).B2(this.Gm().InitializeDown(this.AP,A5,this.B5.Get(this.AP)+
1,this.DP.Get(this.AP),true,this.Eo.Get(this.AP)));},_Init:function(aArg){C.L._Init.
call(this,aArg);C.Timer._Init.call(this.E5={J:this},0);(this.Bf=[]).__proto__=C.
Root.Bf;(this.B5=[]).__proto__=C.Root.B5;(this.I3=[]).__proto__=C.Root.I3;(this.
DQ=[]).__proto__=C.Root.DQ;(this.EJ=[]).__proto__=C.Root.EJ;(this.G6=[]).__proto__=
C.Root.G6;(this.E4=[]).__proto__=C.Root.E4;(this.DP=[]).__proto__=C.Root.DP;(this.
De=[]).__proto__=C.Root.De;(this.Eo=[]).__proto__=C.Root.Eo;(this.Ja=[]).__proto__=
C.Root.Ja;(this.CA=[]).__proto__=C.Root.CA;(this.B0=[]).__proto__=C.Root.B0;this.
__proto__=C.Root;this.F=0x7F;this.E5.JU(50);this.E5.Ip=[this,this.Or];},_Done:function(
){this.__proto__=C.L;this.E5._Done();C.L._Done.call(this);},_ReInit:function(){C.
L._ReInit.call(this);this.E5._ReInit();},_Mark:function(D){var A;C.L._Mark.call(
this,D);if((A=this.C2)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fb)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.En)&&(A._cycle!=D))A._Mark(A._cycle=D);B.ts(
this.Bf,D);if((A=this.G4)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DL)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.E5)._cycle!=D)A._Mark(A._cycle=D);},_className:
"Core::Root"};C.Event={AJ:0,Hn:false,Hl:function(){var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;return ticksCount;},CP:function(aArg){this.AJ=this.Hl(
);},_Init:function(aArg){this.__proto__=C.Event;this.CP(aArg);B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::Event"};C.KeyEvent={BN:0,AZ:0,Down:false,Initialize2:function(BJ,Dd){this.
BN=0;this.AZ=BJ;this.Down=Dd;if((BJ>=0x30)&&(BJ<=0x39))this.BN=(10+BJ)-48;if((BJ>=
0x41)&&(BJ<=0x5A))this.BN=(105+BJ)-65;if((BJ>=0x61)&&(BJ<=0x7A))this.BN=(105+BJ)-
97;if(BJ===0x20)this.BN=131;if(!this.BN)switch(BJ){case 0x2B:this.BN=132;break;case
0x2D:this.BN=133;break;case 0x2A:this.BN=134;break;case 0x2F:this.BN=135;break;case
0x3D:this.BN=136;break;case 0x2E:this.BN=137;break;case 0x2C:this.BN=138;break;case
0x3A:this.BN=139;break;case 0x3B:this.BN=140;break;default:;}return this;},Initialize:
function(BJ,Dd){this.BN=BJ;this.Down=Dd;this.AZ=0x00;var Km=BJ-10;var Kl=BJ-105;
if((Km>=0)&&(Km<=9))this.AZ=(48+Km)&0xFFFF;if((Kl>=0)&&(Kl<=25))this.AZ=(65+Kl)&
0xFFFF;if(BJ===131)this.AZ=0x20;if(this.AZ===0x00)switch(BJ){case 132:this.AZ=0x2B;
break;case 133:this.AZ=0x2D;break;case 134:this.AZ=0x2A;break;case 135:this.AZ=0x2F;
break;case 136:this.AZ=0x3D;break;case 137:this.AZ=0x2E;break;case 138:this.AZ=0x2C;
break;case 139:this.AZ=0x3A;break;case 140:this.AZ=0x3B;break;default:;}return this;
},Mn:function(Lw){switch(Lw){case 141:return((this.AZ>=0x41)&&(this.AZ<=0x5A))||((
this.AZ>=0x61)&&(this.AZ<=0x7A));case 142:return(((this.AZ>=0x41)&&(this.AZ<=0x5A
))||((this.AZ>=0x61)&&(this.AZ<=0x7A)))||((this.AZ>=0x30)&&(this.AZ<=0x39));case
143:return(this.AZ>=0x30)&&(this.AZ<=0x39);case 144:return(((this.AZ>=0x41)&&(this.
AZ<=0x46))||((this.AZ>=0x61)&&(this.AZ<=0x66)))||((this.AZ>=0x30)&&(this.AZ<=0x39
));case 145:return this.AZ!==0x00;case 146:return(this.AZ===0x00)&&!!this.BN;case
147:return(((this.BN===6)||(this.BN===7))||(this.BN===4))||(this.BN===5);case 148:
return(this.AZ!==0x00)||!!this.BN;default:;}return Lw===this.BN;},_Init:function(
aArg){C.Event._Init.call(this,aArg);this.__proto__=C.KeyEvent;},_className:"Core::KeyEvent"
};C.Hh={GX:null,Cx:B.qx,DK:0,BH:0,Down:false,CB:false,InitializeUp:function(Q,CR
,Gf,Kj,D$){this.Down=false;this.BH=Q;this.DK=CR;this.Cx=D$;this.GX=Kj;this.CB=Gf;
return this;},InitializeDown:function(Q,CR,Gf,Kj,D$){this.Down=true;this.BH=Q;this.
DK=CR;this.Cx=D$;this.GX=Kj;this.CB=Gf;return this;},_Init:function(aArg){C.Event.
_Init.call(this,aArg);this.__proto__=C.Hh;},_Mark:function(D){var A;C.Event._Mark.
call(this,D);if((A=this.GX)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"
};C.Hg={DE:B.qx,Cx:B.qx,DK:0,Ce:0,Cd:B.qx,AU:B.qx,BH:0,Down:false,CB:false,InitializeHold:
function(Q,Fs,HI,HJ,CR,DM,D$,HH){this.Down=true;this.BH=Q;this.AU=B.tx(Fs,DM);this.
Cd=B.tx(HI,DM);this.Ce=HJ;this.DK=CR;this.Cx=D$;this.DE=HH;return this;},InitializeUp:
function(Q,Fs,HI,HJ,CR,DM,Gf,D$,HH){this.Down=false;this.BH=Q;this.AU=B.tx(Fs,DM
);this.Cd=B.tx(HI,DM);this.Ce=HJ;this.DK=CR;this.CB=Gf;this.Cx=D$;this.DE=HH;return this;
},InitializeDown:function(Q,Fs,CR,DM,Gf,D$){this.Down=true;this.BH=Q;this.AU=B.tx(
Fs,DM);this.Cd=B.tx(Fs,DM);this.Ce=0;this.DK=CR;this.CB=Gf;this.Cx=D$;this.DE=D$;
return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=
C.Hg;},_className:"Core::CursorEvent"};C.Hj={DE:B.qx,Cx:B.qx,DK:0,Ce:0,AL:B.qx,Cd:
B.qx,AU:B.qx,BH:0,Initialize:function(Q,Fs,HI,aOffset,HJ,Oj,DM,D$,HH){this.BH=Q;
this.AU=B.tx(Fs,DM);this.Cd=B.tx(HI,DM);this.AL=aOffset;this.Ce=HJ;this.DK=Oj;this.
Cx=D$;this.DE=HH;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg
);this.__proto__=C.Hj;},_className:"Core::DragEvent"};C.Hw={BQ:null,AD:B.qx,M1:0
,M0:0,Space:0,Ig:0,C$:function(A_,aClip,aOffset,A7,aBlend){},M:function(E){var A;
if(B.tm(E,this.H))return;var FB=[(A=this.H)[2]-A[0],A[3]-A[1]];var Jc=[E[2]-E[0]
,E[3]-E[1]];var Gy=!B.tl(FB,Jc);var Bo=B.tw(E.slice(0,2),this.H.slice(0,2));if(!
B.tl(Bo,AX)&&!Gy){var O=this.A$;while(!!O&&!((O.F&0x200)===0x200)){if(((O.F&0x400
)===0x400)){var tmp=((O.F&0x100)===0x100);O.Im(Bo,tmp);}O=O.A$;}B.lq(this.BQ,this
);}if((Gy&&(FB[0]>0))&&(FB[1]>0)){var AC=B.tz(this.H,this.AD);var O=this.A$;var HP=
0x14;while(!!O&&!((O.F&0x200)===0x200)){if(((O.F&0x400)===0x400)){if(!!O.Ba&&(O.
Ba.Gv!==this))O.Ba=null;if(!O.Ba&&((O.Fq!==HP)||!!this.Ig))O.HQ(AC,this);}O=O.A$;
}B.lq(this.BQ,this);}C.Bq.M.call(this,E);if(!!this.I&&Gy){this.F=this.F|0x1000;if(
!((this.I.F&0x2000)===0x2000)){this.I.F=this.I.F|0x4000;B.lq([A=this.I,A.C9],this
);}}},_Init:function(aArg){C.Bq._Init.call(this,aArg);this.__proto__=C.Hw;this.F=
0x203;},_Mark:function(D){var A;C.Bq._Mark.call(this,D);if((A=this.BQ)&&((A=A[0]
)._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Outline"};C.Cj={Fw:B.tA(8,null
,null),B$:null,AB:null,DH:null,Jd:null,BQ:null,B3:null,Kw:-1,Kv:0,Du:-1,C_:0,Cu:
0,BO:-1,MV:0,AD:0,FW:-1,FU:0,S:24,Bw:0,FO:null,Dh:false,LO:false,C$:function(A_,
aClip,aOffset,A7,aBlend){C.L.C$.call(this,A_,aClip,aOffset,A7,aBlend);},DispatchEvent:
function(Az){var A;var result=null;if(((this.FW>=0)&&(this.FW<this.Bw))&&!this.Db
){this.BO=this.FW;this.AB=(C.AB.isPrototypeOf(A=B._NewObject(this.FO,0))?A:null);
if(!!this.Bn)this.Bn.A$=this.AB;else this.BR=this.AB;this.AB.AQ=this.Bn;this.Bn=
this.AB;this.AB.I=this;(A=this.B3)?A[1].call(A[0],this):null;var Ap=(C.L.isPrototypeOf(
A=this.AB)?A:null);if(!!Ap)result=Ap.DispatchEvent(Az);else result=this.AB.B2(Az
);if(!!this.AB.AQ)this.AB.AQ.A$=null;else this.BR=null;this.Bn=this.AB.AQ;this.AB.
AQ=null;this.AB.I=null;this.AB=null;}if(!result)result=C.L.DispatchEvent.call(this
,Az);return result;},J5:function(LA){var A;if(!this.FO){B.lq(this.BQ,this);return;
}this.LO=true;var HY=0;if(!this.Dh)HY=this.FU;var Cz=((-HY-this.AD)/this.S)|0;var
C7=((((((A=this.H)[3]-A[1])-HY)-this.AD)-1)/this.S)|0;var I2=(((((A=this.H)[3]-A[
1])+this.S)-1)/this.S)|0;if(I2<1)I2=1;var IW=(I2/2)|0;var IX=(I2/3)|0;if(!IW)IW=
1;if(!IX)IX=1;if(Cz<this.C_){Cz=Cz-IW;C7=C7+IX;}else if(C7>this.Du){Cz=Cz-IX;C7=
C7+IW;}else{Cz=this.C_;C7=this.Du;}if(!this.Dh){if(Cz>=this.Bw){Cz=0;C7=-1;}else
if(C7<0){Cz=0;C7=-1;}if(C7>=this.Bw)C7=this.Bw-1;if(Cz<0)Cz=0;}else if(this.Bw<=
0){Cz=0;C7=-1;}var Jj=this.C_;var Jk=this.Du;var KB=0;var KC=-1;if(Cz>Jj)Jj=Cz;if(
C7<Jk)Jk=C7;if(Jj<=Jk){while((this.Du<C7)&&(this.C_<Jj)){this.H3();this.LG();}while((
this.C_>Cz)&&(this.Du>Jk)){this.LX();this.LF();}}else{this.Du=(this.Du-this.C_)+
Cz;this.C_=Cz;KB=this.C_;KC=this.Du;}while(this.C_<Cz)this.H3();while(this.Du>C7
)this.LX();while(this.C_>Cz)this.LF();while(this.Du<C7)this.LG();var Ak=this.BR;
var CJ=this.C_;var A5=[0,(HY+this.AD)+(CJ*this.S)];var Be=(A=this.H)[3]-A[1];var
LL=this.S;var HO=null;while(!!Ak){var A9=B.tw(A5,Ak.GetExtent().slice(0,2));var Kz=
A5[1];var KA=A5[1]+LL;var Cm=CJ;if(this.Dh){if(Cm<0)Cm=this.Bw-(-Cm%this.Bw);if(
Cm>0)Cm=Cm%this.Bw;}if(!B.tl(A9,AX))Ak.Im(A9,true);if(((((Cm>=this.Kv)&&(Cm<=this.
Kw))||((CJ>=KB)&&(CJ<=KC)))&&(Kz<Be))&&(KA>0)){this.AB=Ak;this.BO=Cm;(A=this.B3)?
A[1].call(A[0],this):null;}Ak=Ak.A$;CJ=CJ+1;A5=[A5[0],A5[1]+this.S];}CJ=this.C_;
Ak=this.BR;A5=[0,(HY+this.AD)+(CJ*this.S)];while(!!Ak){var Kz=A5[1];var KA=A5[1]+
LL;var Cm=CJ;if(this.Dh){if(Cm<0)Cm=this.Bw-(-Cm%this.Bw);if(Cm>0)Cm=Cm%this.Bw;
}if((((Cm>=this.Kv)&&(Cm<=this.Kw))||((CJ>=KB)&&(CJ<=KC)))&&!((Kz<Be)&&(KA>0))){
this.AB=Ak;this.BO=Cm;(A=this.B3)?A[1].call(A[0],this):null;}if(((Cm===this.FW)&&
this.Dh)&&!!HO){var IU=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(B.s9(B.il(Ak.GetExtent(
),IU))>B.s9(B.il(HO.GetExtent(),IU)))HO=Ak;}else if(Cm===this.FW)HO=Ak;Ak=Ak.A$;
CJ=CJ+1;A5=[A5[0],A5[1]+this.S];}this.Kv=0;this.Kw=-1;this.AB=null;this.BO=-1;this.
Ej(HO);this.LO=false;B.lq(this.BQ,this);},JG:function(Am,aFilter){return null;},
K9:function(Am){throw new Error(J9);},R:function(Am,Gj){throw new Error(J_);},LX:
function(){var Ak=this.Bn;if(!Ak)return null;if(Ak===this.Db)this.Ej(null);this.
Du=this.Du-1;if(!!Ak.AQ)Ak.AQ.A$=null;else this.BR=null;this.Bn=Ak.AQ;Ak.AQ=null;
Ak.I=null;Ak.Ba=null;if(this.Cu<8){this.Fw.Set(this.Cu,Ak);this.Cu=this.Cu+1;}return Ak;
},LG:function(){var A;var Ak=null;if(!this.Cu){Ak=(C.AB.isPrototypeOf(A=B._NewObject(
this.FO,0))?A:null);Ak.AT(0x1D);}else{this.Cu=this.Cu-1;Ak=this.Fw.Get(this.Cu);
}this.Du=this.Du+1;var FJ=this.Du;if(this.Dh){if(FJ<0)FJ=this.Bw-(-FJ%this.Bw);if(
FJ>0)FJ=FJ%this.Bw;}this.BO=FJ;this.AB=Ak;(A=this.B3)?A[1].call(A[0],this):null;
this.AB=null;this.BO=-1;if(!!this.Bn)this.Bn.A$=Ak;Ak.I=this;Ak.AQ=this.Bn;this.
Bn=Ak;if(!this.BR)this.BR=Ak;if(FJ===this.FW)this.Ej(Ak);return Ak;},Oo:function(
){while(this.Cu>0){this.Cu=this.Cu-1;this.Fw.Set(this.Cu,null);}},H3:function(){
var Ak=this.BR;if(!Ak)return null;if(Ak===this.Db)this.Ej(null);this.C_=this.C_+
1;if(!!Ak.A$)Ak.A$.AQ=null;else this.Bn=null;this.BR=Ak.A$;Ak.A$=null;Ak.I=null;
Ak.Ba=null;if(this.Cu<8){this.Fw.Set(this.Cu,Ak);this.Cu=this.Cu+1;}return Ak;},
LF:function(){var A;var Ak=null;if(!this.Cu){Ak=(C.AB.isPrototypeOf(A=B._NewObject(
this.FO,0))?A:null);Ak.AT(0x1D);}else{this.Cu=this.Cu-1;Ak=this.Fw.Get(this.Cu);
}this.C_=this.C_-1;var FI=this.C_;if(this.Dh){if(FI<0)FI=this.Bw-(-FI%this.Bw);if(
FI>0)FI=FI%this.Bw;}this.BO=FI;this.AB=Ak;(A=this.B3)?A[1].call(A[0],this):null;
this.AB=null;this.BO=-1;if(!!this.BR)this.BR.AQ=Ak;Ak.I=this;Ak.A$=this.BR;this.
BR=Ak;if(!this.Bn)this.Bn=Ak;if(FI===this.FW)this.Ej(Ak);return Ak;},OA:function(
Ai){var A;this.B$.A0=null;this.B$.G$=null;this.B$=null;(A=this.Jd)?A[1].call(A[0
],this):null;},OC:function(Ai){this.Di(this.DH.AL[1]);},OE:function(Ai){var A;if(
!!this.B$){this.B$.Aj(false);this.B$.A0=null;this.B$.G$=null;this.B$=null;}if(!this.
Dh){var W=this.KY(0,this.Bw-1);var Je=this.H.slice(0,2);W=B.t3(W,W[1]-this.FU);W=[
].concat(W.slice(0,3),W[3]+this.MV);if(W[0]>Je[0])W=[].concat(Je[0],W.slice(1,4)
);if(W[1]>Je[1])W=B.t3(W,Je[1]);var Bo=W[1]-this.H[1];var KE=((A=this.H)[3]-A[1]
)-(W[3]-W[1]);if(KE>0)KE=0;this.DH.AL=[0,this.AD];this.DH.A3=[0,(this.AD+KE)-Bo];
this.DH.A2=[0,this.AD-Bo];}else{var Bo=32000-(32000%this.S);this.DH.AL=[0,this.AD
];this.DH.A3=[0,this.AD-Bo];this.DH.A2=[0,this.AD+Bo];}},GQ:function(E){if(E===this.
DH)return;if(!!E&&!!E.Ha){B.uf("%s%$%*%s",J$,E,Ka);throw new Error(Kb);}if(!!this.
DH){this.DH.Jg=null;this.DH.Ha=null;}this.DH=E;if(!!E){E.Jg=[this,this.OE];E.Ha=[
this,this.OC];}},Di:function(E){var A;if(this.Dh&&(this.Bw>0)){var Ff=this.Bw*this.
S;if(E<0)E=Ff-(-E%Ff);if(E>0)E=E%Ff;if(E>0)E=E-Ff;}if(E===this.AD)return;this.AD=
E;this.GJ();this.AF([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},CD:function(E){var A;if(
E<1)E=1;if(E===this.S)return;this.S=E;while(!!this.H3());this.GJ();this.AF([0,0,(
A=this.H)[2]-A[0],A[3]-A[1]]);},CE:function(E){var A;if(E<0)E=0;if(E===this.Bw)return;
var W=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(!this.Dh){if(E>this.Bw){W=B.t3(W,(this.
FU+this.AD)+(this.Bw*this.S));W=[].concat(W.slice(0,3),(this.FU+this.AD)+(E*this.
S));}else{W=B.t3(W,(this.FU+this.AD)+(E*this.S));W=[].concat(W.slice(0,3),(this.
FU+this.AD)+(this.Bw*this.S));}}else while(!!this.H3());this.Bw=E;this.GJ();this.
AF(W);},CC:function(E){var A;if(E===this.FO)return;this.FO=E;while(!!this.H3());
this.Oo();this.GJ();this.AF([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},EC:function(IO
,Ly,Oi,Gi){var A;if((IO<0)||(IO>=this.Bw))return;var Ah=this.KY(IO,IO);var AC=this.
H;var HR=B.il(Ah,AC);if((!Ly&&!((HR[0]>=HR[2])||(HR[1]>=HR[3])))||(Ly&&B.tm(HR,Ah
))){(Gi)?Gi[1].call(Gi[0],this):null;return;}var A9=0;if(Ah[3]>AC[3])A9=Ah[3]-AC[
3];if(A9>(Ah[1]-AC[1]))A9=Ah[1]-AC[1];if(!!this.B$){this.B$.Aj(false);this.B$.A0=
null;this.B$.G$=null;this.Jd=null;}this.B$=Oi;if(!this.B$){this.Di(this.AD-A9);(
Gi)?Gi[1].call(Gi[0],this):null;}else{this.B$.Aj(false);this.B$.BP(1);this.B$.A0=[
this,this.My,this.Di];this.B$.AW=this.AD;this.B$.Br=this.AD-A9;this.B$.G$=[this,
this.OA];this.B$.JV(false);this.B$.Aj(true);this.Jd=Gi;}},KY:function(HG,IP){if(
HG<0)HG=0;if(IP>=this.Bw)IP=this.Bw-1;if(HG>IP)return BI;var W=this.H;var A9=this.
AD;if(this.Dh){var Ff=this.Bw*this.S;if(A9<0)A9=Ff-(-A9%Ff);if(A9>0)A9=A9%Ff;if(
A9>0)A9=A9-Ff;}else A9=A9+this.FU;W=B.t3(W,(W[1]+A9)+(HG*this.S));W=[].concat(W.
slice(0,3),W[1]+(((IP-HG)+1)*this.S));return W;},My:function(){return this.AD;},
_Init:function(aArg){C.L._Init.call(this,aArg);(this.Fw=[]).__proto__=C.Cj.Fw;this.
__proto__=C.Cj;this.M(Kc);this.FO=B.ui.Text;},_Mark:function(D){var A;C.L._Mark.
call(this,D);B.ts(this.Fw,D);if((A=this.B$)&&(A._cycle!=D))A._Mark(A._cycle=D);if((
A=this.AB)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DH)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Jd)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
BQ)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.B3)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);},_className:"Core::VerticalList"};C.U={Dc:null,JS:null,JQ:
null,Ht:null,Cs:null,D4:null,Fj:0,Al:0,BH:0,AJ:0,DK:0,Ce:0,AL:B.qx,Cd:B.qx,AU:B.
qx,J0:1000,EY:8,CQ:0,Hs:1,FP:-1,Mo:1,Mp:1,JE:false,Down:false,Fn:false,CB:false,
C$:function(A_,aClip,aOffset,A7,aBlend){},B2:function(Az){var A;var An=(C.Hg.isPrototypeOf(
Az)?Az:null);var AK=(C.Hj.isPrototypeOf(Az)?Az:null);var I8=this.Fn;var EM;var Dq;
var H9;var Cn;var E7;if(!An&&!AK)return null;EM=(!!An&&An.Down)&&!An.Ce;Dq=(!!An&&
An.Down)&&(An.Ce>0);H9=(!!An&&An.Down)&&(An.Ce>50);Cn=!!An&&!An.Down;E7=!!AK;if(
!this.JE){if((((this.CQ&0x20)===0x20)&&(this.Al>0))&&(this.Al<33554432)){var Bd=(
C.Hh.isPrototypeOf(Az)?Az:null);if(((!!Bd&&Bd.Down)&&(Bd.GX!==this))&&B.qu(this.
GetExtent(),this.I.Hr(Bd.Cx))){this.Fj=0x20;this.Al=this.Al|67108864;return null;
}}if(EM){var Gt=0;var Eq;this.Al=this.Al|(1<<An.BH);for(Eq=this.Al&4095;Eq>0;Eq=
Eq>>1)if(!!(Eq&1))Gt=Gt+1;if(Gt===this.Hs)this.Al=(this.Al|16777216)|(4096<<An.BH
);}if(Cn)this.Al=(this.Al&~(1<<An.BH))|33554432;if(H9&&(this.Al<16777216))this.Al=
this.Al|67108864;if(Cn&&An.CB)this.Al=this.Al|67108864;if(Cn&&!(this.Al&4095))this.
Fj=0x0;if(Cn&&!(this.Al&16777215))this.Al=0;if(Dq&&(this.Al>=67108864))this.Ef().
Iv(null,null,this.Fj);if((Dq&&!!(this.Al&16777216))&&!!(this.Al&33554432)){Dq=false;
Cn=true;}if(!!An&&!(this.Al&(4096<<An.BH)))return this;if(!!AK&&!(this.Al&(4096<<
AK.BH)))return this;if(Cn&&!(this.Al&16777216))return this;if(((EM||E7)||Dq)&&((
this.Al<16777216)||(this.Al>=33554432)))return this;if(Cn)this.Al=this.Al&3758100479;
if(Cn&&!(this.Al&16777215))this.Al=0;}if(!!An){this.Down=EM||Dq;this.Fn=this.JJ(
An.AU);this.Cd=An.Cd;this.AU=An.AU;this.AL=AX;this.Ce=An.Ce;this.DK=An.DK;this.CB=
An.CB;this.BH=An.BH;this.AJ=An.AJ;if(An.Down&&!An.Ce)I8=false;}if(!!AK){this.Down=
true;this.Fn=this.JJ(AK.AU);this.Cd=AK.Cd;this.AU=AK.AU;this.AL=AK.AL;this.Ce=AK.
Ce;this.DK=AK.DK;this.BH=AK.BH;this.CB=false;this.AJ=AK.AJ;(A=this.Dc)?A[1].call(
A[0],this):null;}if((!!An&&this.Down)&&!this.Ce)(A=this.D4)?A[1].call(A[0],this):
null;if((!!An&&this.Down)&&(this.Ce>0))(A=this.Ht)?A[1].call(A[0],this):null;if((
this.Down&&this.Fn)&&!I8)(A=this.JQ)?A[1].call(A[0],this):null;if(((this.Down&&!
this.Fn)&&I8)||((!this.Down&&this.Fn)&&I8))(A=this.JS)?A[1].call(A[0],this):null;
if(!!An&&!this.Down)(A=this.Cs)?A[1].call(A[0],this):null;if(!!this.CQ){var C5=0x0;
if(((((this.CQ&0x10)===0x10)&&!!An)&&An.Down)&&(An.Ce>=this.J0))C5=0x10;if((((this.
CQ&0x1)===0x1)&&!!AK)&&((AK.Cx[1]-AK.DE[1])<=-this.EY))C5=0x1;if((((this.CQ&0x2)===
0x2)&&!!AK)&&((AK.Cx[1]-AK.DE[1])>=this.EY))C5=0x2;if((((this.CQ&0x4)===0x4)&&!!
AK)&&((AK.Cx[0]-AK.DE[0])<=-this.EY))C5=0x4;if((((this.CQ&0x8)===0x8)&&!!AK)&&((
AK.Cx[0]-AK.DE[0])>=this.EY))C5=0x8;if(!!C5){this.Fj=C5;this.Ef().Iv(null,this,C5
);}}return this;},EB:function(AI,Q,CR,E0,Em){var A;if(!!E0&&(E0!==this))return null;
if((CR<this.Mp)||(CR>this.Mo))return null;if((this.FP>=0)&&(this.Hs>1))return null;
if((this.FP>=0)&&(Q!==this.FP))return null;if(!this.JE&&(this.Al>=33554432))return null;
if((!this.JE&&(this.Al>=16777216))&&!(this.Al&(4096<<Q)))return null;if(!!(Em&this.
CQ))return null;if(this.Ml()){var Ah=B.il(AI,this.GetExtent());if(!((Ah[0]>=Ah[2
])||(Ah[1]>=Ah[3]))){var CV=B.s_(AI);var Bo=AX;if(CV[0]<Ah[0])Bo=[Ah[0]-CV[0],Bo[
1]];if(CV[0]>=Ah[2])Bo=[(Ah[2]-CV[0])-1,Bo[1]];if(CV[1]<Ah[1])Bo=[Bo[0],Ah[1]-CV[
1]];if(CV[1]>=Ah[3])Bo=[Bo[0],(Ah[3]-CV[1])-1];return B._NewObject(C.Ie,0).Initialize(
this,Bo);}}else{var Bg=B.tA(9,B.qx,null);var K;Bg.Set(0,B.s_(AI));Bg.Set(1,Bg.Get(
0));Bg.Set(2,Bg.Get(0));Bg.Set(3,Bg.Get(0));Bg.Set(4,Bg.Get(0));Bg.Set(1,[AI[0],
Bg.Get(1)[1]]);Bg.Set(2,[Bg.Get(2)[0],AI[1]]);Bg.Set(3,[AI[2],Bg.Get(3)[1]]);Bg.
Set(4,[Bg.Get(4)[0],AI[3]]);Bg.Set(5,AI.slice(0,2));Bg.Set(6,[AI[2],AI[1]]);Bg.Set(
7,[AI[0],AI[3]]);Bg.Set(8,AI.slice(2,4));for(K=0;K<9;K=K+1)if(this.JJ(Bg.Get(K))
)return B._NewObject(C.Ie,0).Initialize(this,B.tw(Bg.Get(K),Bg.Get(0)));}return null;
},Aj:function(E){if(E)this.Cw(0x10,0x0);else this.Cw(0x0,0x10);},_Init:function(
aArg){C.Hz._Init.call(this,aArg);this.__proto__=C.U;this.F=0x11B;},_Mark:function(
D){var A;C.Hz._Mark.call(this,D);if((A=this.Dc)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.JS)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.JQ
)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ht)&&((A=A[0])._cycle!=D)
)A._Mark(A._cycle=D);if((A=this.Cs)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((
A=this.D4)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SimpleTouchHandler"
};C.AA={timer:null,Jg:null,LW:null,Ha:null,JW:null,BG:null,BA:null,Fj:0,Al:0,IT:
0,Es:5000.000000,Gx:0,I7:B.qx,Gq:0,EI:0,AH:0,FH:0,Hc:0,Gp:0,EH:0,AG:0,FG:0,FF:0,
Cd:B.qx,KS:B.qx,Hs:1,FP:-1,A2:B.qx,A3:B.qx,AL:B.qx,Iy:B.qx,DI:B.qx,Ix:B.qx,La:5.000000
,MX:500,J0:1000,EY:8,CQ:0,JZ:-1,K$:200,EZ:0,KW:0.500000,MY:true,J3:true,J2:true,
GV:false,DT:false,DU:false,IS:false,C$:function(A_,aClip,aOffset,A7,aBlend){},B2:
function(Az){var A;var An=(C.Hg.isPrototypeOf(Az)?Az:null);var AK=(C.Hj.isPrototypeOf(
Az)?Az:null);var Bd=(C.Hh.isPrototypeOf(Az)?Az:null);var EM;var Dq;var H9;var Cn;
var E7;if((((((((this.CQ&0x20)===0x20)&&(this.Al>0))&&(this.Al<33554432))&&!!Bd)&&
Bd.Down)&&(Bd.GX!==this))&&B.qu(this.H,this.I.Hr(Bd.Cx))){this.Fj=0x20;this.Al=this.
Al|67108864;}EM=(!!An&&An.Down)&&!An.Ce;Dq=(!!An&&An.Down)&&(An.Ce>0);H9=(!!An&&
An.Down)&&(An.Ce>50);Cn=!!An&&!An.Down;E7=!!AK;if(EM){var Gt=0;var Eq;this.Al=this.
Al|(1<<An.BH);for(Eq=this.Al&4095;Eq>0;Eq=Eq>>1)if(!!(Eq&1))Gt=Gt+1;if(Gt===this.
Hs)this.Al=(this.Al|16777216)|(4096<<An.BH);}if(Cn)this.Al=(this.Al&~(1<<An.BH))|
33554432;if(H9&&(this.Al<16777216))this.Al=this.Al|67108864;if(Cn&&An.CB)this.Al=
this.Al|67108864;if(Cn&&!(this.Al&4095))this.Fj=0x0;if(Cn&&!(this.Al&16777215))this.
Al=0;if(Dq&&(this.Al>=67108864))this.Ef().Iv(null,null,this.Fj);if((Dq&&!!(this.
Al&16777216))&&!!(this.Al&33554432)){Dq=false;Cn=true;}if(!!An&&!(this.Al&(4096<<
An.BH)))return this;if(!!AK&&!(this.Al&(4096<<AK.BH)))return this;if(Cn&&!(this.
Al&16777216))return this;if(((EM||E7)||Dq)&&((this.Al<16777216)||(this.Al>=33554432
)))return this;if(Cn)this.Al=this.Al&3758100479;if(Cn&&!(this.Al&16777215))this.
Al=0;if(!!Bd&&(Bd.GX===this))return null;if((!!Bd&&Bd.Down)&&(this.IS||!this.GV)
)return null;if((!!Bd&&Bd.Down)&&!B.qu(this.H,this.I.Hr(Bd.Cx)))return null;if((
!!Bd&&!Bd.Down)&&(!this.IS||(this.IT!==Bd.BH)))return null;if((!An&&!AK)&&!Bd)return null;
if(!!An){this.IS=EM||Dq;this.IT=An.BH;this.Cd=An.Cd;}if(!!AK)this.IT=AK.BH;if(!!
Bd){this.IS=Bd.Down;this.IT=Bd.BH;}if(!!Bd&&Bd.Down){this.KM();this.AG=(((Bd.AJ-
this.FF)*0.001000)*this.EH)+this.AG;this.AH=(((Bd.AJ-this.Hc)*0.001000)*this.EI)+
this.AH;if(this.DT)this.AG=0.000000;if(this.DU)this.AH=0.000000;this.EH=0.000000;
this.EI=0.000000;this.DT=false;this.DU=false;this.I7=this.AL;this.Gx=Bd.AJ;return this;
}if(EM){this.KM();this.AG=(((An.AJ-this.FF)*0.001000)*this.EH)+this.AG;this.AH=(((
An.AJ-this.Hc)*0.001000)*this.EI)+this.AH;if(this.DT||!this.GV)this.AG=0.000000;
if(this.DU||!this.GV)this.AH=0.000000;this.EH=0.000000;this.EI=0.000000;this.DT=
false;this.DU=false;if(!this.GV){this.GV=true;(A=this.BA)?A[1].call(A[0],this):null;(
A=this.Jg)?A[1].call(A[0],this):null;}this.I7=this.AL;this.Gx=An.AJ;}if(!!AK){var
AE=B.tw(AK.AU,AK.Cd);var Aq=this.AL;if(this.J2)Aq=[this.I7[0]+AE[0],Aq[1]];if(this.
J3)Aq=[Aq[0],this.I7[1]+AE[1]];if(this.MY){if(Aq[0]<this.A3[0])Aq=[this.A3[0]+(((
Aq[0]-this.A3[0])/2)|0),Aq[1]];else if(Aq[0]>this.A2[0])Aq=[this.A2[0]+(((Aq[0]-
this.A2[0])/2)|0),Aq[1]];if(Aq[1]<this.A3[1])Aq=[Aq[0],this.A3[1]+(((Aq[1]-this.
A3[1])/2)|0)];else if(Aq[1]>this.A2[1])Aq=[Aq[0],this.A2[1]+(((Aq[1]-this.A2[1])
/2)|0)];}else{if(Aq[0]<this.A3[0])Aq=[this.A3[0],Aq[1]];else if(Aq[0]>this.A2[0]
)Aq=[this.A2[0],Aq[1]];if(Aq[1]<this.A3[1])Aq=[Aq[0],this.A3[1]];else if(Aq[1]>this.
A2[1])Aq=[Aq[0],this.A2[1]];}if(!B.tl(Aq,this.AL)){this.KS=B.tw(Aq,this.AL);this.
AL=Aq;(A=this.Ha)?A[1].call(A[0],this):null;(A=this.JW)?A[1].call(A[0],this):null;
}}if((!!Bd&&!Bd.Down)&&((Bd.AJ-this.Gx)>=(((A=this.K$)<0)?A+0x100000000:A))){this.
AG=0.000000;this.AH=0.000000;}if(Dq&&((An.AJ-this.Gx)>=(((A=this.K$)<0)?A+0x100000000:
A))){this.AG=0.000000;this.AH=0.000000;}if(Cn&&(this.JZ>=0)){var Cc=B.tw(An.AU,An.
Cd);if(((Cc[0]*Cc[0])+(Cc[1]*Cc[1]))<=(this.JZ*this.JZ)){this.AG=0.000000;this.AH=
0.000000;}}if(!!AK&&(AK.AJ>this.Gx)){var AE=AK.AL;var LM=1000.000000/(AK.AJ-this.
Gx);var Fc=0.000000;var Fd=0.000000;if(this.J2)Fc=AE[0]*LM;if(this.J3)Fd=AE[1]*LM;
if((Fc*this.AG)<0.000000)this.AG=0.000000;if((Fd*this.AH)<0.000000)this.AH=0.000000;
this.AG=(this.AG+Fc)*0.500000;this.AH=(this.AH+Fd)*0.500000;if(this.EZ>0.000000){
if(this.AG>this.EZ)this.AG=this.EZ;if(this.AG<-this.EZ)this.AG=-this.EZ;if(this.
AH>this.EZ)this.AH=this.EZ;if(this.AH<-this.EZ)this.AH=-this.EZ;}this.Gx=AK.AJ;}
if(!!this.CQ){var C5=0x0;if((Dq&&((this.CQ&0x10)===0x10))&&(An.Ce>=this.J0))C5=0x10;
if((E7&&((this.CQ&0x1)===0x1))&&((AK.Cx[1]-AK.DE[1])<=-this.EY))C5=0x1;if((E7&&((
this.CQ&0x2)===0x2))&&((AK.Cx[1]-AK.DE[1])>=this.EY))C5=0x2;if((E7&&((this.CQ&0x4
)===0x4))&&((AK.Cx[0]-AK.DE[0])<=-this.EY))C5=0x4;if((E7&&((this.CQ&0x8)===0x8))&&((
AK.Cx[0]-AK.DE[0])>=this.EY))C5=0x8;if(!!C5){this.Fj=C5;this.Ef().Iv(null,this,C5
);}}if(!Cn&&!Bd)return this;if(!!An&&An.CB){this.AG=0.000000;this.AH=0.000000;}if((
this.AL[0]<=this.A3[0])||(this.AL[0]>=this.A2[0]))this.AG=0.000000;else if(this.
AG===0.000000){var AR=this.AL[0];var AV=this.A3[0]+this.Iy[0];var AO=this.A2[0]-
this.Ix[0];if(AR<AV)AO=this.A3[0];else if(AR>AO)AV=this.A2[0];else if(this.DI[0]<=
1){AV=AR;AO=AR;}else{var Eu=(AO-AR)%this.DI[0];AO=(AR-this.DI[0])+Eu;AV=AR+Eu;if(
AO<this.A3[0])AO=this.A3[0];if(AV>this.A2[0])AV=this.A2[0];}if((AV-AR)<=(AR-AO))
AR=AV;else AR=AO;if(AR!==this.AL[0]){var Ck=AR-this.AL[0];if(Ck>0.000000)this.AG=
Math.sqrt((Ck*2.000000)*this.Es)+20.000000;if(Ck<0.000000)this.AG=-Math.sqrt((-Ck
*2.000000)*this.Es)-20.000000;this.EH=(400.000000-(this.AG*this.AG))/(2.000000*Ck
);this.Gp=AR;}}else{var OM=this.AG*this.AG;var Ck=OM/(2.000000*this.Es);var AR=this.
AL[0];if(this.AG>0.000000)AR=AR+(Ck|0);if(this.AG<0.000000)AR=AR-(Ck|0);if(AR>this.
A2[0])AR=this.A2[0];else if(AR<this.A3[0])AR=this.A3[0];var L1=AR;var AV=this.A3[
0]+this.Iy[0];var AO=this.A2[0]-this.Ix[0];if(AR<AV)AO=this.A3[0];else if(AR>AO)
AV=this.A2[0];else if(this.DI[0]<=1){AV=AR;AO=AR;}else{var Eu=(AO-AR)%this.DI[0];
AO=(AR-this.DI[0])+Eu;AV=AR+Eu;if(AO<this.A3[0])AO=this.A3[0];if(AV>this.A2[0])AV=
this.A2[0];}if(this.AG>0.000000)if(AO<=this.AL[0])AR=AV;else if((AR-AO)<(AV-AR))
AR=AO;else AR=AV;else if(AV>=this.AL[0])AR=AO;else if((AR-AO)>(AV-AR))AR=AV;else
AR=AO;if(AR!==this.AL[0]){Ck=AR-this.AL[0];if(AR!==L1){var Fh=AR-L1;if(Fh>0.000000
)this.AG=this.AG+Math.sqrt((Fh*2.000000)*this.Es);if(Fh<0.000000)this.AG=this.AG-
Math.sqrt((-Fh*2.000000)*this.Es);}if(this.AG>0.000000)this.AG=this.AG+20.000000;
if(this.AG<0.000000)this.AG=this.AG-20.000000;this.EH=(400.000000-(this.AG*this.
AG))/(2.000000*Ck);this.Gp=AR;}else this.AG=0.000000;}if((this.AL[1]<=this.A3[1]
)||(this.AL[1]>=this.A2[1]))this.AH=0.000000;else if(this.AH===0.000000){var AS=
this.AL[1];var AV=this.A3[1]+this.Iy[1];var AO=this.A2[1]-this.Ix[1];if(AS<AV)AO=
this.A3[1];else if(AS>AO)AV=this.A2[1];else if(this.DI[1]<=1){AV=AS;AO=AS;}else{
var Eu=(AO-AS)%this.DI[1];AO=(AS-this.DI[1])+Eu;AV=AS+Eu;if(AO<this.A3[1])AO=this.
A3[1];if(AV>this.A2[1])AV=this.A2[1];}if((AV-AS)<=(AS-AO))AS=AV;else AS=AO;if(AS
!==this.AL[1]){var Ck=AS-this.AL[1];if(Ck>0.000000)this.AH=Math.sqrt((Ck*2.000000
)*this.Es)+20.000000;if(Ck<0.000000)this.AH=-Math.sqrt((-Ck*2.000000)*this.Es)-20.000000;
this.EI=(400.000000-(this.AH*this.AH))/(2.000000*Ck);this.Gq=AS;}}else{var ON=this.
AH*this.AH;var Ck=ON/(2.000000*this.Es);var AS=this.AL[1];if(this.AH>0.000000)AS=
AS+(Ck|0);if(this.AH<0.000000)AS=AS-(Ck|0);if(AS>this.A2[1])AS=this.A2[1];else if(
AS<this.A3[1])AS=this.A3[1];var L2=AS;var AV=this.A3[1]+this.Iy[1];var AO=this.A2[
1]-this.Ix[1];if(AS<AV)AO=this.A3[1];else if(AS>AO)AV=this.A2[1];else if(this.DI[
1]<=1){AV=AS;AO=AS;}else{var Eu=(AO-AS)%this.DI[1];AO=(AS-this.DI[1])+Eu;AV=AS+Eu;
if(AO<this.A3[1])AO=this.A3[1];if(AV>this.A2[1])AV=this.A2[1];}if(this.AH>0.000000
)if(AO<=this.AL[1])AS=AV;else if((AS-AO)<(AV-AS))AS=AO;else AS=AV;else if(AV>=this.
AL[1])AS=AO;else if((AS-AO)>(AV-AS))AS=AV;else AS=AO;if(AS!==this.AL[1]){Ck=AS-this.
AL[1];if(AS!==L2){var Fh=AS-L2;if(Fh>0.000000)this.AH=this.AH+Math.sqrt((Fh*2.000000
)*this.Es);if(Fh<0.000000)this.AH=this.AH-Math.sqrt((-Fh*2.000000)*this.Es);}if(
this.AH>0.000000)this.AH=this.AH+20.000000;if(this.AH<0.000000)this.AH=this.AH-20.000000;
this.EI=(400.000000-(this.AH*this.AH))/(2.000000*Ck);this.Gq=AS;}else this.AH=0.000000;
}if(!!An)this.FF=An.AJ;if(!!Bd)this.FF=Bd.AJ;this.Hc=this.FF;this.FG=this.AL[0];
this.FH=this.AL[1];this.OO();return this;},EB:function(AI,Q,CR,E0,Em){var A;if(!
!E0&&(E0!==this))return null;if((this.FP>=0)&&(this.Hs>1))return null;if((this.FP>=
0)&&(Q!==this.FP))return null;if(this.Al>=33554432)return null;if((this.Al>=16777216
)&&!(this.Al&(4096<<Q)))return null;if(!!(Em&this.CQ))return null;if(!this.J3&&!
!(Em&0x3))return null;if(!this.J2&&!!(Em&0xC))return null;var Ah=B.il(AI,this.H);
if(!((Ah[0]>=Ah[2])||(Ah[1]>=Ah[3]))){var CV=B.s_(AI);var Bo=AX;if(CV[0]<Ah[0])Bo=[
Ah[0]-CV[0],Bo[1]];if(CV[0]>=Ah[2])Bo=[(Ah[2]-CV[0])-1,Bo[1]];if(CV[1]<Ah[1])Bo=[
Bo[0],Ah[1]-CV[1]];if(CV[1]>=Ah[3])Bo=[Bo[0],(Ah[3]-CV[1])-1];return B._NewObject(
C.Ie,0).Initialize(this,Bo);}return null;},KM:function(){if(!!this.timer){B.sM([
this,this.Dg],this.timer,0);this.timer=null;}},OO:function(){this.timer=B._GetAutoObject(
B.uj.GG);B.sz([this,this.Dg],this.timer,0);},Dg:function(Ai){var A;if(!this.timer
)return;var Hd=(this.timer.AJ-this.FF)*0.001000;var He=(this.timer.AJ-this.Hc)*0.001000;
var OP=Hd*Hd;var OQ=He*He;var Fc=(this.EH*Hd)+this.AG;var Fd=(this.EI*He)+this.AH;
var Aq=[((((this.EH*0.500000)*OP)+(this.AG*Hd))+this.FG)|0,((((this.EI*0.500000)
*OQ)+(this.AH*He))+this.FH)|0];var Jf=this.MX*0.001000;if(this.DT&&(Hd>=Jf)){Aq=[
this.Gp,Aq[1]];this.AG=0.000000;this.EH=0.000000;this.FG=Aq[0];this.DT=false;}else
if(this.DT){var CI=1.000000-Math.pow(1.000000-(Hd/Jf),this.La);Aq=[(this.FG+((this.
Gp-this.FG)*CI))|0,Aq[1]];}if(this.DU&&(He>=Jf)){Aq=[Aq[0],this.Gq];this.AH=0.000000;
this.EI=0.000000;this.FH=Aq[1];this.DU=false;}else if(this.DU){var CI=1.000000-Math.
pow(1.000000-(He/Jf),this.La);Aq=[Aq[0],(this.FH+((this.Gq-this.FH)*CI))|0];}if(((
this.AG>0.000000)&&(Fc<0.000000))||((this.AG<0.000000)&&(Fc>0.000000))){Fc=0.000000;
Aq=[this.AL[0],Aq[1]];}if(((this.AH>0.000000)&&(Fd<0.000000))||((this.AH<0.000000
)&&(Fd>0.000000))){Fd=0.000000;Aq=[Aq[0],this.AL[1]];}if(!this.DT&&(Aq[0]<this.A3[
0])){this.FG=Aq[0];this.Gp=this.A3[0];this.FF=this.timer.AJ;this.DT=true;}else if(
!this.DT&&(Aq[0]>this.A2[0])){this.FG=Aq[0];this.Gp=this.A2[0];this.FF=this.timer.
AJ;this.DT=true;}if(!this.DU&&(Aq[1]<this.A3[1])){this.FH=Aq[1];this.Gq=this.A3[
1];this.Hc=this.timer.AJ;this.DU=true;}else if(!this.DU&&(Aq[1]>this.A2[1])){this.
FH=Aq[1];this.Gq=this.A2[1];this.Hc=this.timer.AJ;this.DU=true;}if(((!this.DT&&(
this.AG!==0.000000))&&(Fc>-20.000000))&&(Fc<20.000000)){Aq=[this.Gp,Aq[1]];this.
AG=0.000000;this.EH=0.000000;this.FG=Aq[0];}if(((!this.DU&&(this.AH!==0.000000))&&(
Fd>-20.000000))&&(Fd<20.000000)){Aq=[Aq[0],this.Gq];this.AH=0.000000;this.EI=0.000000;
this.FH=Aq[1];}if(!B.tl(Aq,this.AL)){this.KS=B.tw(Aq,this.AL);this.AL=Aq;(A=this.
Ha)?A[1].call(A[0],this):null;(A=this.JW)?A[1].call(A[0],this):null;}if((((this.
AG===0.000000)&&(this.AH===0.000000))&&!this.DT)&&!this.DU){this.KM();this.GV=false;(
A=this.LW)?A[1].call(A[0],this):null;(A=this.BG)?A[1].call(A[0],this):null;}},GR:
function(E){if(E[0]<0)E=[0,E[1]];if(E[1]<0)E=[E[0],0];this.DI=E;},GO:function(E){
if(E<0.000000)E=0.000000;if(E>1.000000)E=1.000000;if(E===this.KW)return;this.KW=
E;if(E<0.000000)E=0.000000;this.Es=E*10000.000000;},_Init:function(aArg){C.Bq._Init.
call(this,aArg);this.__proto__=C.AA;this.F=0x11B;},_Mark:function(D){var A;C.Bq.
_Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Jg)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.LW)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Ha)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.JW)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.BG)&&((A=A[0
])._cycle!=D))A._Mark(A._cycle=D);if((A=this.BA)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);},_className:"Core::SlideTouchHandler"};C.Ii={A$:null,Cs:null,D4:null,
Ht:null,Gw:0,AJ:0,K_:0,Mj:148,BN:0,AZ:0,Cq:true,Down:false,JY:false,Id:false,B2:
function(Az){var A;if(!!Az&&Az.Mn(this.Mj)){this.Down=Az.Down;this.BN=Az.BN;this.
AZ=Az.AZ;this.AJ=Az.AJ;this.Id=false;if(Az.Down){this.K_=this.Gw;this.JY=this.Gw>
0;if(this.JY)(A=this.Ht)?A[1].call(A[0],this):null;else(A=this.D4)?A[1].call(A[0
],this):null;if(!this.Id)this.Gw=this.Gw+1;return!this.Id;}if(!Az.Down){this.JY=
this.Gw>1;this.K_=this.Gw-1;this.Gw=0;(A=this.Cs)?A[1].call(A[0],this):null;return!
this.Id;}}return false;},CP:function(aArg){var A;var Df=(C.L.isPrototypeOf(A=this.
J)?A:null);if(!Df)throw new Error(II);this.A$=Df.I_;Df.I_=this;},_Init:function(
aArg){this.__proto__=C.Ii;this.CP(aArg);B.gv++;},_Done:function(){this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.A$)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Cs)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.D4)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ht)&&((A=A[
0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=
D);},J:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"};C.Ie={AB:
null,Hi:0,AL:B.qx,Initialize:function(Am,aOffset){this.AB=Am;this.AL=aOffset;this.
Hi=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:function(
aArg){this.__proto__=C.Ie;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.AB)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::CursorHit"};C.Mq={Df:null,_Init:function(aArg){this.__proto__=
C.Mq;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Df)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::ModalContext"};C.Ij={Gv:null,CY:B.qy,AC:B.qy,isEmpty:false,_Init:function(
aArg){this.__proto__=C.Ij;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.Gv)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::LayoutContext"};C.Ik={H2:B.qx,H1:B.qx,H0:B.qx,HZ:B.qx,_Init:
function(aArg){C.Ij._Init.call(this,aArg);this.__proto__=C.Ik;},_className:"Core::LayoutQuadContext"
};C.Mg={Df:null,_Init:function(aArg){this.__proto__=C.Mg;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Df)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(
A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::DialogContext"};
C.M6={Ko:null,BR:null,_Init:function(aArg){this.__proto__=C.M6;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Ko)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.BR)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::TaskQueue"};C.M5={_Init:function(aArg){this.__proto__=C.M5;
B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:
0,_observers:null,_className:"Core::Task"};C.EX={resource:null,Ee:function(){this.
resource=null;},CP:function(aArg){this.resource=aArg;},_Init:function(aArg){this.
__proto__=C.EX;this.CP(aArg);B.gv++;},_Done:function(){this.Ee();this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::Resource"
};C.Timer={Ip:null,timer:null,AJ:0,Period:1000,Ib:0,Cq:false,Ee:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},Ji:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.sL(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},JU:function(E){if(E<0)E=0;if(E===this.Period)return;
this.Period=E;if(this.Cq)this.Ji(this.Ib,E);},ML:function(E){if(E<0)E=0;if(E===this.
Ib)return;this.Ib=E;if(this.Cq)this.Ji(E,this.Period);},Aj:function(E){if(E===this.
Cq)return;this.Cq=E;if(E)this.Ji(this.Ib,this.Period);else this.Ji(0,0);this.AJ=
this.Hl();},Hl:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.qt
)|0;return ticksCount;},Trigger:function(){var A;this.AJ=this.Hl();if(!this.Period
)this.Aj(false);(A=this.Ip)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=C.Timer;B.gv++;},_Done:function(){this.Ee();this.__proto__=null;B.gv--;
},_ReInit:function(){},_Mark:function(D){var A;if((A=this.Ip)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null
,_cycle:0,_observers:null,_className:"Core::Timer"};C.K8={JR:null,A0:null,Cq:true
,LQ:function(Ai){var A;(A=this.JR)?A[1].call(A[0],this):null;},MS:function(E){if(
B.tn(this.A0,E))return;if(!!this.A0&&this.Cq)B.sO([this,this.LQ],this.A0,0);this.
A0=E;if(!!this.A0&&this.Cq)B.sB([this,this.LQ],this.A0,0);},_Init:function(aArg){
this.__proto__=C.K8;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.JR)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.A0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::PropertyObserver"};C.P9={P_:0x1,OZ:0x2,Pp:0x4,P7:0x8,Cq:0x10,PX:0x20,Pq:0x40
,PA:0x80,Po:0x100,Pu:0x200,Pi:0x400,PK:0x800,Lc:0x1000,P8:0x2000,PE:0x4000,PF:0x8000
,Pc:0x10000,PD:0x20000,PU:0x40000};C.Fq={PL:0x1,PM:0x2,OW:0x4,OX:0x8,OY:0x10,OV:
0x20};C.Ig={PB:0,P4:1,O4:2,Pv:3,PO:4,P5:5,P6:6,O5:7,O6:8,Px:9,Pw:10,PQ:11,PP:12};
C.KeyCode={NoKey:0,Ok:1,Exit:2,Menu:3,Up:4,Down:5,Left:6,Right:7,PageUp:8,PageDown:
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
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};C.PN={Qc:0x1,P$:0x2,Qa:0x4,Qb:0x8,Pz:
0x10,Pr:0x20};
C._Init=function(){C.Hz.__proto__=C.AB;C.Bq.__proto__=C.AB;C.L.__proto__=C.Bq;C.Root.
__proto__=C.L;C.KeyEvent.__proto__=C.Event;C.Hh.__proto__=C.Event;C.Hg.__proto__=
C.Event;C.Hj.__proto__=C.Event;C.Hw.__proto__=C.Bq;C.Cj.__proto__=C.L;C.U.__proto__=
C.Hz;C.AA.__proto__=C.Bq;C.Ik.__proto__=C.Ij;};C.Dj=function(D){};return C;})();

/* Embedded Wizard */