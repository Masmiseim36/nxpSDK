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
var AX=[0,0];var BM=[0,0,0,0];var Dx="The view does not belong to this group";var
Fi="No view to restack";var Fj="View is not in this group";var Ga="No view to remove";
var Gb="No view to add";var JP="View already in a group";var JQ="Recursive invalidate during active update cycle.";
var JR="The method Restack() is not available in Core::VerticalList.";var JS="The method Remove() is not available in Core::VerticalList.";
var JT="The method Add() is not available in Core::VerticalList.";var JU="The Slide Touch Handler:";
var JV="is already connected with a view.";var Iq="The Slide Touch Handler is already connected with a view to scroll "+
"its content.";var Ir=[0,0,170,120];var JW="The KeyPressHandler is embedded within an object not being derived "+
"from Core::Group.";
C.Ax={AJ:null,AC:null,I:null,A$:null,F:0x103,Ff:0x14,HH:function(Am,Iz){},AT:function(
E){var A;var AD=E^this.Ff;if(!AD)return;this.Ff=E;if(!!this.A$&&!((this.F&0x400)===
0x400)){this.I.F=this.I.F|0x5000;B.kD([A=this.I,A.Cx],this);this.I.AE([0,0,(A=this.
I.H)[2]-A[0],A[3]-A[1]]);}if(!!this.A$&&((this.F&0x400)===0x400)){this.A$.Gq.F=this.
A$.Gq.F|0x1000;this.I.F=this.I.F|0x4000;B.kD([A=this.I,A.Cx],this);}},CN:function(
){var An=this.I;while(!!An){var LI=(C.Root.isPrototypeOf(An)?An:null);if(!!LI)return LI;
An=An.I;}return null;},C6:function(A_,aClip,aOffset,A7,aBlend){},BY:function(Az){
return null;},Eq:function(AH,R,CQ,EQ,Ef){return null;},H1:function(Am,Dy){return AX;
},H9:function(aOffset,Iw){},GetExtent:function(){return BM;},Cr:function(ER,Ge){
var A;if(((this.F&0x200)===0x200))ER=ER&~0x400;var Kn=(this.F&~Ge)|ER;var Fp=Kn^
this.F;this.F=Kn;if(!!this.I&&!!(Fp&0x14)){var Lt=((this.F&0x14)===0x14);if(Lt&&
!this.I.C8)this.I.Ec(this);if(!Lt&&(this.I.C8===this))this.I.Ec(this.I.Jm(this,0x14
));}if(!!this.I&&!!(Fp&0x403))this.I.AE(this.GetExtent());if((!!this.A$&&((Kn&0x400
)===0x400))&&((Fp&0x1)===0x1)){this.F=this.F|0x800;this.I.F=this.I.F|0x4000;B.kD([
A=this.I,A.Cx],this);}if(!!this.I&&((Fp&0x400)===0x400)){this.A$=null;this.F=this.
F|0x800;this.I.F=this.I.F|0x4000;B.kD([A=this.I,A.Cx],this);}},_Init:function(aArg
){this.__proto__=C.Ax;B.gb++;},_Done:function(){this.__proto__=C.Ax;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.AJ)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.AC)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.I)&&(A._cycle!=D
))A._Mark(A._cycle=D);if((A=this.A$)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::View"};C.Ho={B8:B.pm,CC:B.pm,B7:B.pm,CB:B.pm,HH:function(Am,Iz){var Ar=B.
_NewObject(C.H8,0);this.A$=null;Ar.CW=this.GetExtent();Ar.AA=Am;Ar.Gq=Iz;Ar.HQ=this.
CB;Ar.HR=this.B7;Ar.HS=this.CC;Ar.HT=this.B8;this.A$=Ar;},H1:function(Am,Dy){var
A;var BV=this.Ff;var Ar=(C.H8.isPrototypeOf(A=this.A$)?A:null);var Z=Ar.CW[0];var
Aa=Ar.CW[1];var Ac=Ar.CW[2];var Ad=Ar.CW[3];var CV=[Am[2]-Am[0],Am[3]-Am[1]];var
Bm=Ac-Z;var Bc=Ad-Aa;if(!Dy){var DU=[(A=Ar.AA)[2]-A[0],A[3]-A[1]];Z=Z-Ar.AA[0];Aa=
Aa-Ar.AA[1];if(DU[0]!==CV[0]){var Dj=((BV&0x4)===0x4);var Dk=((BV&0x8)===0x8);var
En=((BV&0x1)===0x1);if(!Dj&&(En||!Dk))Z=((Z*CV[0])/DU[0])|0;if(!Dk&&(En||!Dj)){Ac=
Ac-Ar.AA[0];Ac=((Ac*CV[0])/DU[0])|0;Ac=Ac-CV[0];}else Ac=Ac-Ar.AA[2];Z=Z+Am[0];Ac=
Ac+Am[2];if(!En){if(Dj&&!Dk)Ac=Z+Bm;else if(!Dj&&Dk)Z=Ac-Bm;else{Z=Z+((((Ac-Z)-Bm
)/2)|0);Ac=Z+Bm;}}}else{Ac=Ac-Ar.AA[2];Z=Z+Am[0];Ac=Ac+Am[2];}if(DU[1]!==CV[1]){
var Dl=((BV&0x10)===0x10);var Di=((BV&0x20)===0x20);var Eo=((BV&0x2)===0x2);if(!
Dl&&(Eo||!Di))Aa=((Aa*CV[1])/DU[1])|0;if(!Di&&(Eo||!Dl)){Ad=Ad-Ar.AA[1];Ad=((Ad*
CV[1])/DU[1])|0;Ad=Ad-CV[1];}else Ad=Ad-Ar.AA[3];Aa=Aa+Am[1];Ad=Ad+Am[3];if(!Eo){
if(Dl&&!Di)Ad=Aa+Bc;else if(!Dl&&Di)Aa=Ad-Bc;else{Aa=Aa+((((Ad-Aa)-Bc)/2)|0);Ad=
Aa+Bc;}}}else{Ad=Ad-Ar.AA[3];Aa=Aa+Am[1];Ad=Ad+Am[3];}}else{switch(Dy){case 3:{Z=
Am[0];Ac=Z+Bm;}break;case 4:{Ac=Am[2];Z=Ac-Bm;}break;case 1:{Aa=Am[1];Ad=Aa+Bc;}
break;case 2:{Ad=Am[3];Aa=Ad-Bc;}break;default:;}if((Dy===3)||(Dy===4)){var Dl=((
BV&0x10)===0x10);var Di=((BV&0x20)===0x20);var Eo=((BV&0x2)===0x2);if(Eo){Aa=Am[
1];Ad=Am[3];}else if(Dl&&!Di){Aa=Am[1];Ad=Aa+Bc;}else if(Di&&!Dl){Ad=Am[3];Aa=Ad-
Bc;}else{Aa=Am[1]+((((Am[3]-Am[1])-Bc)/2)|0);Ad=Aa+Bc;}}if((Dy===1)||(Dy===2)){var
Dj=((BV&0x4)===0x4);var Dk=((BV&0x8)===0x8);var En=((BV&0x1)===0x1);if(En){Z=Am[
0];Ac=Am[2];}else if(Dj&&!Dk){Z=Am[0];Ac=Z+Bm;}else if(Dk&&!Dj){Ac=Am[2];Z=Ac-Bm;
}else{Z=Am[0]+((((Am[2]-Am[0])-Bm)/2)|0);Ac=Z+Bm;}}}Ar.isEmpty=(Z>=Ac)||(Aa>=Ad);
Bm=(Ac-Z)-1;Bc=(Ad-Aa)-1;var EY=Ar.CW[0];var EZ=Ar.CW[1];var EC=(Ar.CW[2]-EY)-1;
var EB=(Ar.CW[3]-EZ)-1;if(!EC)EC=1;if(!EB)EB=1;if(((this.F&0x100)===0x100)){this.
CB=[Z+((((Ar.HQ[0]-EY)*Bm)/EC)|0),Aa+((((Ar.HQ[1]-EZ)*Bc)/EB)|0)];this.B7=[Z+((((
Ar.HR[0]-EY)*Bm)/EC)|0),Aa+((((Ar.HR[1]-EZ)*Bc)/EB)|0)];this.CC=[Z+((((Ar.HS[0]-
EY)*Bm)/EC)|0),Aa+((((Ar.HS[1]-EZ)*Bc)/EB)|0)];this.B8=[Z+((((Ar.HT[0]-EY)*Bm)/EC
)|0),Aa+((((Ar.HT[1]-EZ)*Bc)/EB)|0)];}else{this.EJ([Z+((((Ar.HQ[0]-EY)*Bm)/EC)|0
),Aa+((((Ar.HQ[1]-EZ)*Bc)/EB)|0)]);this.EK([Z+((((Ar.HR[0]-EY)*Bm)/EC)|0),Aa+((((
Ar.HR[1]-EZ)*Bc)/EB)|0)]);this.EL([Z+((((Ar.HS[0]-EY)*Bm)/EC)|0),Aa+((((Ar.HS[1]-
EZ)*Bc)/EB)|0)]);this.EM([Z+((((Ar.HT[0]-EY)*Bm)/EC)|0),Aa+((((Ar.HT[1]-EZ)*Bc)/
EB)|0)]);this.A$=Ar;}return[Bm+1,Bc+1];},H9:function(aOffset,Iw){if(Iw){this.CB=
B.so(this.CB,aOffset);this.B7=B.so(this.B7,aOffset);this.CC=B.so(this.CC,aOffset
);this.B8=B.so(this.B8,aOffset);}else{this.EJ(B.so(this.CB,aOffset));this.EK(B.so(
this.B7,aOffset));this.EL(B.so(this.CC,aOffset));this.EM(B.so(this.B8,aOffset));
}},GetExtent:function(){if(!!this.A$&&this.A$.isEmpty)return BM;var Z=this.CB[0];
var Aa=this.CB[1];var Ac=this.CC[0];var Ad=this.CC[1];if((((this.B8[0]!==Z)||(this.
B7[1]!==Aa))||(this.B7[0]!==Ac))||(this.B8[1]!==Ad)){if(this.B7[0]<Z)Z=this.B7[0
];if(this.CC[0]<Z)Z=this.CC[0];if(this.B8[0]<Z)Z=this.B8[0];if(this.B7[1]<Aa)Aa=
this.B7[1];if(this.CC[1]<Aa)Aa=this.CC[1];if(this.B8[1]<Aa)Aa=this.B8[1];if(this.
CB[0]>Ac)Ac=this.CB[0];if(this.B7[0]>Ac)Ac=this.B7[0];if(this.B8[0]>Ac)Ac=this.B8[
0];if(this.CB[1]>Ad)Ad=this.CB[1];if(this.B7[1]>Ad)Ad=this.B7[1];if(this.B8[1]>Ad
)Ad=this.B8[1];}else{var tmp;if(Ac<Z){tmp=Z;Z=Ac;Ac=tmp;}if(Ad<Aa){tmp=Aa;Aa=Ad;
Ad=tmp;}}return[Z,Aa,Ac+1,Ad+1];},EM:function(E){var A;if(B.sa(E,this.B8))return;
if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.GetExtent());this.A$=null;this.
B8=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.GetExtent());if((!!this.I&&((
this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
I.F=this.I.F|0x4000;B.kD([A=this.I,A.Cx],this);}},EL:function(E){var A;if(B.sa(E
,this.CC))return;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.GetExtent());this.
A$=null;this.CC=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.GetExtent());
if((!!this.I&&((this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.I.F=this.I.F|0x4000;B.kD([A=this.I,A.Cx],this);}},EK:function(E){var
A;if(B.sa(E,this.B7))return;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.GetExtent(
));this.A$=null;this.B7=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.GetExtent(
));if((!!this.I&&((this.F&0x400)===0x400))&&!((this.I.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.I.F=this.I.F|0x4000;B.kD([A=this.I,A.Cx],this);}},EJ:function(
E){var A;if(B.sa(E,this.CB))return;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.
GetExtent());this.A$=null;this.CB=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(
this.GetExtent());if((!!this.I&&((this.F&0x400)===0x400))&&!((this.I.F&0x2000)===
0x2000)){this.F=this.F|0x800;this.I.F=this.I.F|0x4000;B.kD([A=this.I,A.Cx],this);
}},Jp:function(Ee){var Be=B.sr(4,B.pm,null);var K=0;var Bg=3;var Kb=false;var Kc=
false;Be.Set(0,this.CB);Be.Set(1,this.B7);Be.Set(2,this.CC);Be.Set(3,this.B8);while(
K<4){var LP=Be.Get(K)[0];var I$=Be.Get(K)[1];var Ov=Be.Get(Bg)[0];var Ky=Be.Get(
Bg)[1];if(((I$>Ee[1])!==(Ky>Ee[1]))||((I$<Ee[1])!==(Ky<Ee[1]))){var LQ=((((Ov-LP
)*(Ee[1]-I$))/(Ky-I$))|0)+LP;if(Ee[0]>LQ)Kb=!Kb;if(Ee[0]<LQ)Kc=!Kc;}Bg=K;K=K+1;}
return Kb||Kc;},L7:function(){return((((this.CB[0]===this.B8[0])&&(this.B7[0]===
this.CC[0]))&&(this.CB[1]===this.B7[1]))&&(this.CC[1]===this.B8[1]))||((((this.CB[
0]===this.B7[0])&&(this.CC[0]===this.B8[0]))&&(this.CB[1]===this.B8[1]))&&(this.
B7[1]===this.CC[1]));},_Init:function(aArg){C.Ax._Init.call(this,aArg);this.__proto__=
C.Ho;},_className:"Core::QuadView"};C.BD={H:B.pn,HH:function(Am,Iz){var Ar=B._NewObject(
C.Ha,0);Ar.CW=this.H;Ar.AA=Am;Ar.Gq=Iz;this.A$=Ar;},H1:function(Am,Dy){var A;var
BV=this.Ff;var Ar=this.A$;var Z=Ar.CW[0];var Aa=Ar.CW[1];var Ac=Ar.CW[2];var Ad=
Ar.CW[3];var CV=[Am[2]-Am[0],Am[3]-Am[1]];var Bm=Ac-Z;var Bc=Ad-Aa;if(!Dy){var DU=[(
A=Ar.AA)[2]-A[0],A[3]-A[1]];Z=Z-Ar.AA[0];Aa=Aa-Ar.AA[1];if(DU[0]!==CV[0]){var Dj=((
BV&0x4)===0x4);var Dk=((BV&0x8)===0x8);var En=((BV&0x1)===0x1);if(!Dj&&(En||!Dk)
)Z=((Z*CV[0])/DU[0])|0;if(!Dk&&(En||!Dj)){Ac=Ac-Ar.AA[0];Ac=((Ac*CV[0])/DU[0])|0;
Ac=Ac-CV[0];}else Ac=Ac-Ar.AA[2];Z=Z+Am[0];Ac=Ac+Am[2];if(!En){if(Dj&&!Dk)Ac=Z+Bm;
else if(!Dj&&Dk)Z=Ac-Bm;else{Z=Z+((((Ac-Z)-Bm)/2)|0);Ac=Z+Bm;}}}else{Ac=Ac-Ar.AA[
2];Z=Z+Am[0];Ac=Ac+Am[2];}if(DU[1]!==CV[1]){var Dl=((BV&0x10)===0x10);var Di=((BV&
0x20)===0x20);var Eo=((BV&0x2)===0x2);if(!Dl&&(Eo||!Di))Aa=((Aa*CV[1])/DU[1])|0;
if(!Di&&(Eo||!Dl)){Ad=Ad-Ar.AA[1];Ad=((Ad*CV[1])/DU[1])|0;Ad=Ad-CV[1];}else Ad=Ad-
Ar.AA[3];Aa=Aa+Am[1];Ad=Ad+Am[3];if(!Eo){if(Dl&&!Di)Ad=Aa+Bc;else if(!Dl&&Di)Aa=
Ad-Bc;else{Aa=Aa+((((Ad-Aa)-Bc)/2)|0);Ad=Aa+Bc;}}}else{Ad=Ad-Ar.AA[3];Aa=Aa+Am[1
];Ad=Ad+Am[3];}}else{switch(Dy){case 3:{Z=Am[0];Ac=Z+Bm;}break;case 4:{Ac=Am[2];
Z=Ac-Bm;}break;case 1:{Aa=Am[1];Ad=Aa+Bc;}break;case 2:{Ad=Am[3];Aa=Ad-Bc;}break;
default:;}if((Dy===3)||(Dy===4)){var Dl=((BV&0x10)===0x10);var Di=((BV&0x20)===0x20
);var Eo=((BV&0x2)===0x2);if(Eo){Aa=Am[1];Ad=Am[3];}else if(Dl&&!Di){Aa=Am[1];Ad=
Aa+Bc;}else if(Di&&!Dl){Ad=Am[3];Aa=Ad-Bc;}else{Aa=Am[1]+((((Am[3]-Am[1])-Bc)/2)|
0);Ad=Aa+Bc;}}if((Dy===1)||(Dy===2)){var Dj=((BV&0x4)===0x4);var Dk=((BV&0x8)===
0x8);var En=((BV&0x1)===0x1);if(En){Z=Am[0];Ac=Am[2];}else if(Dj&&!Dk){Z=Am[0];Ac=
Z+Bm;}else if(Dk&&!Dj){Ac=Am[2];Z=Ac-Bm;}else{Z=Am[0]+((((Am[2]-Am[0])-Bm)/2)|0);
Ac=Z+Bm;}}}Ar.isEmpty=(Z>=Ac)||(Aa>=Ad);if(((this.F&0x100)===0x100)){this.H=[Z,Aa
,Ac,Ad];}else{this.L([Z,Aa,Ac,Ad]);this.A$=Ar;}return[Ac-Z,Ad-Aa];},H9:function(
aOffset,Iw){if(Iw)this.H=B.sq(this.H,aOffset);else this.L(B.sq(this.H,aOffset));
},GetExtent:function(){return this.H;},L:function(E){var A;if(B.sb(E,this.H))return;
if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);this.A$=null;this.H=E;if(!!this.
I&&((this.F&0x1)===0x1))this.I.AE(this.H);if((!!this.I&&((this.F&0x400)===0x400)
)&&!((this.I.F&0x2000)===0x2000)){this.F=this.F|0x800;this.I.F=this.I.F|0x4000;B.
kD([A=this.I,A.Cx],this);}},NX:function(Kx){this.L(Kx);},Mc:function(){return this.
H;},_Init:function(aArg){C.Ax._Init.call(this,aArg);this.__proto__=C.BD;},_className:
"Core::RectView"};C.O={BE:null,BF:null,IS:null,Dz:null,Fq:null,Gm:null,C8:null,Ap:
255,C6:function(A_,aClip,aOffset,A7,aBlend){var A;A7=((A7+1)*this.Ap)>>8;aBlend=
aBlend&&((this.F&0x2)===0x2);if(!this.Dz)this.N7(A_,aClip,B.so(aOffset,this.H.slice(
0,2)),A7,aBlend);else{var CS=255|(255<<8)|(255<<16)|((A7&0xFF)<<24);this.Dz.Update(
);A_.Kz(aClip,this.Dz,0,B.sq(this.H,aOffset),AX,CS,CS,CS,CS,aBlend);}},Eq:function(
AH,R,CQ,EQ,Ef){var A;var M=this.BF;var Go=null;var X=BM;var Cu=null;var Kl=!!this.
Gm&&(!!this.Gm.J8||!!this.Gm.BE);if(((A=B.hV(AH,this.H))[0]>=A[2])||(A[1]>=A[3])
)return null;AH=B.sp(AH,this.H.slice(0,2));while(!!M){if(((M.F&0x400)===0x400)&&
!Cu){Cu=M.AC;while(!!Cu&&!((Cu.F&0x200)===0x200))Cu=Cu.AC;if(!!Cu)X=B.hV(AH,Cu.GetExtent(
));else X=BM;}if(Cu===M){Cu=null;X=BM;}if((((((M.F&0x8)===0x8)&&((M.F&0x10)===0x10
))&&!((M.F&0x40000)===0x40000))&&!((M.F&0x20000)===0x20000))&&(!((M.F&0x10000)===
0x10000)||((this.Fq.Db===M)&&!Kl))){var CW=M.GetExtent();var IK=EQ;var Gi=null;if(
IK===M)IK=null;if(((M.F&0x400)===0x400)){if(!(((A=B.hV(CW,X))[0]>=A[2])||(A[1]>=
A[3])))Gi=M.Eq(X,R,CQ,IK,Ef);}else{if(!(((A=B.hV(CW,AH))[0]>=A[2])||(A[1]>=A[3])
)||(EQ===M))Gi=M.Eq(AH,R,CQ,IK,Ef);}M=M.AC;if(!!Gi){if(!Go||((Gi.G4<Go.G4)&&(Gi.
G4>=0)))Go=Gi;if(!Gi.G4)M=null;}}else M=M.AC;}return Go;},Cr:function(ER,Ge){var
A;var Oa=this.F;C.BD.Cr.call(this,ER,Ge);var Fp=this.F^Oa;if(!!this.C8&&((Fp&0x40
)===0x40)){if(((this.F&0x40)===0x40))this.C8.Cr(0x40,0x0);else this.C8.Cr(0x0,0x40
);}if(!!this.Fq&&((Fp&0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.Fq.Db.F&0x14
)===0x14))this.Fq.Db.Cr(0x40,0x0);else this.Fq.Db.Cr(0x0,0x40);}if(!!Fp){this.F=
this.F|0x8000;B.kD([this,this.Cx],this);}},L:function(E){var A;if(B.sb(E,this.H)
)return;var Fv=[(A=this.H)[2]-A[0],A[3]-A[1]];var IW=[E[2]-E[0],E[3]-E[1]];var Gt=
!B.sa(Fv,IW);if(Gt&&!!this.Dz){this.Dz.JA(IW);B.pl(this,0);B.pl(this.Dz,0);}C.BD.
L.call(this,E);if((Gt&&(Fv[0]>0))&&(Fv[1]>0)){var AA=[].concat(AX,Fv);var M=this.
BE;var HF=0x14;while(!!M){if((!M.A$&&(M.Ff!==HF))&&!((M.F&0x400)===0x400))M.HH(AA
,null);M=M.AJ;}}if(Gt){this.F=this.F|0x5000;B.kD([this,this.Cx],this);}},Kp:function(
Az){var Lx=(C.KeyEvent.isPrototypeOf(Az)?Az:null);var D7=this.IS;if(!Lx)return null;
while(!!D7&&(!D7.Ck||!D7.BY(Lx)))D7=D7.AJ;return D7;},N7:function(A_,aClip,aOffset
,A7,aBlend){var A;var M=this.BE;var Lo=BM;var LD=true;while(!!M){if(((M.F&0x200)===
0x200)){var LC=(C.Hk.isPrototypeOf(M)?M:null);Lo=B.hV(aClip,B.sq(LC.H,aOffset));
LD=((LC.F&0x1)===0x1);}if(((M.F&0x1)===0x1)){if(((M.F&0x400)===0x400)){if(LD){var
X=B.hV(B.sq(M.GetExtent(),aOffset),Lo);if(!((X[0]>=X[2])||(X[1]>=X[3])))M.C6(A_,
X,aOffset,A7,aBlend);}}else{var X=B.hV(B.sq(M.GetExtent(),aOffset),aClip);if(!((
X[0]>=X[2])||(X[1]>=X[3])))M.C6(A_,X,aOffset,A7,aBlend);}}M=M.AJ;}},Oi:function(
){var A;var J$=((this.F&0x1000)===0x1000);var E0=[0,0,(A=this.H)[2]-A[0],A[3]-A[
1]];var ED=false;var Gn=BM;var IM=AX;var HG=0;var IL=0;var M=this.BF;var Cu=null;
var HF=0x14;while(!!M){if(((M.F&0x800)===0x800)){ED=true;M.F=M.F&~0x800;}if(ED&&((
M.F&0x200)===0x200)){ED=false;if(!!(C.Hk.isPrototypeOf(M)?M:null).H7)M.F=M.F|0x1000;
}M=M.AC;}ED=false;M=this.BE;if(J$){this.F=this.F&~0x1000;J$=!((E0[0]>=E0[2])||(E0[
1]>=E0[3]));}this.F=this.F|0x2000;while(!!M){if(((M.F&0x400)===0x400)){if(!!M.A$&&(
M.A$.Gq!==Cu))M.A$=null;if((!M.A$&&ED)&&((M.Ff!==HF)||!!IL))M.HH(Gn,Cu);}if(!!M.
A$){if(J$&&!((M.F&0x400)===0x400))M.H1(E0,0);if(ED&&((M.F&0x400)===0x400)){var BP=
M.H1(B.sq(Gn,IM),IL);if(((M.F&0x1)===0x1)){var A9=AX;switch(IL){case 3:A9=[BP[0]+
HG,A9[1]];break;case 4:A9=[-BP[0]-HG,A9[1]];break;case 1:A9=[A9[0],BP[1]+HG];break;
case 2:A9=[A9[0],-BP[1]-HG];break;default:;}IM=B.so(IM,A9);}}}if(((M.F&0x200)===
0x200)){ED=((M.F&0x1000)===0x1000);Cu=(C.Hk.isPrototypeOf(M)?M:null);if(ED){M.F=
M.F&~0x1000;Gn=B.sq(Cu.H,Cu.AB);IM=AX;IL=Cu.H7;HG=Cu.Space;ED=!((Gn[0]>=Gn[2])||(
Gn[1]>=Gn[3]));}if(ED){this.AE(Cu.H);}}M=M.AJ;}this.F=this.F&~0x2000;this.KZ([E0[
2]-E0[0],E0[3]-E0[1]]);},Cx:function(Aj){var A;var Ou=((this.F&0x1000)===0x1000);
if(((this.F&0x4000)===0x4000)){this.F=this.F&~0x4000;this.Oi();}if(((this.F&0x8000
)===0x8000)||Ou){this.F=this.F&~0x8000;this.JN(this.F);}},Ec:function(E){var A;if(
!!E&&(E.I!==this))throw new Error(Dx);if(!!E&&!((E.F&0x14)===0x14))E=null;if(!!E&&((
E.F&0x10000)===0x10000))E=null;if(E===this.C8)return;if(!!this.C8)this.C8.Cr(0x0
,0x60);this.C8=E;if(!!E){if(((this.F&0x40)===0x40))E.Cr(0x60,0x0);else E.Cr(0x20
,0x0);}},A4:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(E===this.Ap)return;this.
Ap=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},Ev:function(Kx){this.
A4(Kx);},Jq:function(Ee){var tmp=this;while(!!tmp){Ee=B.sn(Ee,tmp.H.slice(0,2));
tmp=tmp.I;}return Ee;},DispatchEvent:function(Az){var A;var M=this.C8;var An=(C.
O.isPrototypeOf(M)?M:null);var AY=null;var Kl=!!this.Gm&&(!!this.Gm.J8||!!this.Gm.
BE);if(!!M&&((((M.F&0x10000)===0x10000)||((M.F&0x40000)===0x40000))||((M.F&0x20000
)===0x20000))){M=null;An=null;}if(!!this.Fq&&!Kl)AY=this.Fq.Db.DispatchEvent(Az);
if(!AY&&!!An)AY=An.DispatchEvent(Az);else if(!AY&&!!M)AY=M.BY(Az);if(!AY)AY=this.
BY(Az);if(!AY)AY=this.Kp(Az);return AY;},BroadcastEventAtPosition:function(Az,Lj
,aFilter){var A;var M=this.BF;var AY=null;while(!!M&&!AY){if((!aFilter||((A=aFilter
)&&((M.F&A)===A)))&&B.se(M.GetExtent(),Lj)){var An=(C.O.isPrototypeOf(M)?M:null);
if(!!An)AY=An.BroadcastEventAtPosition(Az,B.sn(Lj,An.H.slice(0,2)),aFilter);else
AY=M.BY(Az);}M=M.AC;}if(!AY)AY=this.BY(Az);return AY;},BroadcastEvent:function(Az
,aFilter){var A;var M=this.BF;var AY=null;while(!!M&&!AY){if(!aFilter||((A=aFilter
)&&((M.F&A)===A))){var An=(C.O.isPrototypeOf(M)?M:null);if(!!An)AY=An.BroadcastEvent(
Az,aFilter);else AY=M.BY(Az);}M=M.AC;}if(!AY)AY=this.BY(Az);if(!AY)AY=this.Kp(Az
);return AY;},KZ:function(aSize){},JN:function(Lk){},GB:function(){this.F=this.F|
0x8000;B.kD([this,this.Cx],this);},AE:function(AH){var A;var An=this;while(!!An&&
!((AH[0]>=AH[2])||(AH[1]>=AH[3]))){var GQ=An.Dz;if(!An.I&&(An!==this)){An.AE(AH);
return;}if(!!GQ){if(((A=GQ.D$)[0]>=A[2])||(A[1]>=A[3])){B.pl(An,0);B.pl(GQ,0);}var
J_=false;if(J_)GQ.D$=[0,0,(A=An.H)[2]-A[0],A[3]-A[1]];else GQ.D$=B.pA(GQ.D$,AH);
}if(!((An.F&0x1)===0x1))return;AH=B.hV(B.sq(AH,An.H.slice(0,2)),An.H);An=An.I;}}
,CP:function(aArg){this.GB();},Jm:function(P,aFilter){var A;if(!P||(P.I!==this))
return null;var Fu=P.AJ;var Fw=P.AC;var HN=0x10000;if(((aFilter&0x10000)===0x10000
))HN=0x0;while(!!Fu||!!Fw){if((!!Fu&&(!aFilter||((A=aFilter)&&((Fu.F&A)===A))))&&(
!HN||!((A=HN)&&((Fu.F&A)===A))))return Fu;if((!!Fw&&(!aFilter||((A=aFilter)&&((Fw.
F&A)===A))))&&(!HN||!((A=HN)&&((Fw.F&A)===A))))return Fw;if(!!Fu)Fu=Fu.AJ;if(!!Fw
)Fw=Fw.AC;}return null;},Ie:function(P,Ce){var A;if(!P)throw new Error(Fi);if(P.
I!==this)throw new Error(Fj);var Fm=P;var Fo=P;while((Ce>0)&&!!Fm.AJ){Fm=Fm.AJ;Ce=
Ce-1;}while((Ce<0)&&!!Fo.AC){Fo=Fo.AC;Ce=Ce+1;}if((Fm===P)&&(Fo===P))return;if(((
P.F&0x401)===0x401)){if(!!P.AC&&!!P.A$)P.AC.F=P.AC.F|0x800;P.F=P.F|0x800;this.F=
this.F|0x4000;B.kD([this,this.Cx],this);}if(((P.F&0x200)===0x200)){if(!!P.AC)P.AC.
F=P.AC.F|0x800;P.F=P.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cx],this);}if(
!!P.AC)P.AC.AJ=P.AJ;if(!!P.AJ)P.AJ.AC=P.AC;if(this.BE===P)this.BE=P.AJ;if(this.BF===
P)this.BF=P.AC;if(Fm!==P){P.AJ=Fm.AJ;P.AC=Fm;Fm.AJ=P;if(!!P.AJ)P.AJ.AC=P;}if(Fo!==
P){P.AJ=Fo;P.AC=Fo.AC;Fo.AC=P;if(!!P.AC)P.AC.AJ=P;}if(!P.AJ)this.BF=P;if(!P.AC)this.
BE=P;if(((P.F&0x1)===0x1))this.AE(P.GetExtent());},KS:function(P){var A;if(!P)throw new
Error(Ga);if(P.I!==this)throw new Error(Fj);if((((P.F&0x401)===0x401)&&!!P.AC)&&
!!P.A$){P.AC.F=P.AC.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cx],this);}if(((
P.F&0x200)===0x200)){if(!!P.AC)P.AC.F=P.AC.F|0x800;this.F=this.F|0x4000;B.kD([this
,this.Cx],this);}P.A$=null;if(this.C8===P)this.Ec(this.Jm(P,0x14));if(!!P.AC)P.AC.
AJ=P.AJ;if(!!P.AJ)P.AJ.AC=P.AC;if(this.BE===P)this.BE=P.AJ;if(this.BF===P)this.BF=
P.AC;P.I=null;P.AJ=null;P.AC=null;if(((P.F&0x1)===0x1))this.AE(P.GetExtent());},
S:function(P,Ce){var A;if(!P)throw new Error(Gb);if(!!P.I)throw new Error(JP);P.
I=this;P.AC=this.BF;if(!!this.BF)this.BF.AJ=P;this.BF=P;if(!this.BE)this.BE=P;if(
!!Ce)this.Ie(P,Ce);if(((P.F&0x1)===0x1))this.AE(P.GetExtent());if(((!this.C8&&((
P.F&0x4)===0x4))&&((P.F&0x10)===0x10))&&!((P.F&0x10000)===0x10000))this.Ec(P);if(((
P.F&0x401)===0x401)){P.F=P.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Cx],this
);}if(((P.F&0x200)===0x200)){P.F=P.F|0x800;this.F=this.F|0x4000;B.kD([this,this.
Cx],this);}},Cl:function(){return this.Ap;},_Init:function(aArg){C.BD._Init.call(
this,aArg);this.__proto__=C.O;this.F=0x1F;this.CP(aArg);},_Mark:function(D){var A;
C.BD._Mark.call(this,D);if((A=this.BE)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.BF)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.IS)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Dz)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fq)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.Gm)&&(A._cycle!=D))A._Mark(A._cycle=D);
if((A=this.C8)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Group"};C.
Root={CZ:null,E2:null,Eg:null,Bd:B.sr(10,null,null),GR:null,CE:null,EA:null,HZ:0
,J4:0,Bf:0,B1:B.sr(10,0,null),IJ:B.sr(10,B.pn,null),DO:B.sr(10,0,null),Ez:B.sr(10
,B.pm,null),GT:B.sr(10,0,null),EV:B.sr(10,B.pm,null),DN:B.sr(10,B.pm,null),Da:B.
sr(10,B.pm,null),Eh:B.sr(10,B.pm,null),AQ:0,IO:0,IN:0,IU:B.sr(3,B.pn,null),Lz:0,
Cw:B.sr(4,0,null),BW:B.sr(4,B.pn,null),BO:0,E$:8,L1:250,Fs:0,E1:0,Ka:true,IT:false
,CN:function(){return this;},C6:function(A_,aClip,aOffset,A7,aBlend){var fullScreenUpdate=
false;fullScreenUpdate=B.i6;if(!fullScreenUpdate)A_.Jl(aClip,B.sq(B.sq(aClip,aOffset
),this.H.slice(0,2)),0x00000000,0x00000000,0x00000000,0x00000000,false);C.O.C6.call(
this,A_,aClip,aOffset,A7,aBlend);},Cr:function(ER,Ge){var A;C.O.Cr.call(this,ER,
Ge);if(!this.I&&(((ER&0x1)===0x1)||((Ge&0x1)===0x1)))this.AE([0,0,(A=this.H)[2]-
A[0],A[3]-A[1]]);if(!this.I&&(((ER&0x2)===0x2)||((Ge&0x2)===0x2)))this.AE([0,0,(
A=this.H)[2]-A[0],A[3]-A[1]]);},Ec:function(E){if((E!==this.CE)||!E)C.O.Ec.call(
this,E);},A4:function(E){var A;var Ob=this.Ap;C.O.A4.call(this,E);if(((Ob!==this.
Ap)&&!this.I)&&((this.F&0x1)===0x1))this.AE([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);
},DispatchEvent:function(Az){if((this.J4>0)&&!!(C.KeyEvent.isPrototypeOf(Az)?Az:
null))return null;if(!!Az){Az.G_=!!this.Bf;if(!!this.Bf)Az.AK=this.Bf;}var AY=null;
if(!!this.CE){AY=this.CE.DispatchEvent(Az);if(!!AY){this.Bf=0;return AY;}}if(!!this.
E2){AY=this.E2.Db.DispatchEvent(Az);if(!AY)AY=this.BY(Az);if(!AY)AY=this.Kp(Az);
this.Bf=0;return AY;}AY=C.O.DispatchEvent.call(this,Az);this.Bf=0;return AY;},BroadcastEvent:
function(Az,aFilter){if(!!Az){Az.G_=!!this.Bf;if(!!this.Bf)Az.AK=this.Bf;}var AY=
C.O.BroadcastEvent.call(this,Az,aFilter);this.Bf=0;return AY;},AE:function(AH){var
A;if(this.HZ>0)throw new Error(JQ);if(!!this.Dz&&!this.I){if(((A=this.Dz.D$)[0]>=
A[2])||(A[1]>=A[3])){B.pl(this,0);B.pl(this.Dz,0);}var J_=false;if(J_)this.Dz.D$=[
0,0,(A=this.H)[2]-A[0],A[3]-A[1]];else this.Dz.D$=B.pA(this.Dz.D$,AH);}var fullScreenUpdate=
false;fullScreenUpdate=B.i6;if(fullScreenUpdate)AH=[0,0,(A=this.H)[2]-A[0],A[3]-
A[1]];if(!!this.I){C.O.AE.call(this,AH);return;}AH=B.hV(B.sq(AH,this.H.slice(0,2
)),this.H);if((AH[0]>=AH[2])||(AH[1]>=AH[3]))return;var K;for(K=0;K<this.BO;K=K+
1)if(!(((A=B.hV(this.BW.Get(K),AH))[0]>=A[2])||(A[1]>=A[3]))){this.BW.Set(K,B.pA(
this.BW.Get(K),AH));this.Cw.Set(K,B.rY(this.BW.Get(K)));return;}if(this.BO<3){this.
BW.Set(this.BO,AH);this.Cw.Set(this.BO,B.rY(AH));this.BO=this.BO+1;return;}var Bg;
var CY;var HK=0;var HL=0;var Ll=2147483647;this.BW.Set(this.BO,AH);this.Cw.Set(this.
BO,B.rY(AH));for(Bg=0;Bg<=this.BO;Bg=Bg+1)for(CY=Bg+1;CY<=this.BO;CY=CY+1){var I_=
B.rY(B.pA(this.BW.Get(Bg),this.BW.Get(CY)));var LE=((I_<<8)/(this.Cw.Get(Bg)+this.
Cw.Get(CY)))|0;if(LE<Ll){Ll=LE;HK=Bg;HL=CY;}}this.BW.Set(HK,B.pA(this.BW.Get(HK)
,this.BW.Get(HL)));this.Cw.Set(HK,B.rY(this.BW.Get(HK)));if(HL!==this.BO){this.BW.
Set(HL,this.BW.Get(this.BO));this.Cw.Set(HL,this.Cw.Get(this.BO));}},Ie:function(
P,Ce){if(!P)throw new Error(Fi);if(((Ce>0)&&!!this.CE)&&(this.CE.I===this)){var M=
P;var IY=Ce;while((M.AJ!==this.CE)&&(IY>0)){M=M.AJ;IY=IY-1;}Ce=Ce-IY;}C.O.Ie.call(
this,P,Ce);},S:function(P,Ce){if(((Ce>=0)&&!!this.CE)&&(this.CE.I===this))Ce=-1;
C.O.S.call(this,P,Ce);},N5:function(){var Cg=B._NewObject(C.G5,0);Cg.G_=!!this.Bf;
if(!!this.Bf)Cg.AK=this.Bf;return Cg;},Gh:function(){var Cg=B._NewObject(C.G2,0);
Cg.G_=!!this.Bf;if(!!this.Bf)Cg.AK=this.Bf;return Cg;},GS:function(){var Cg=B._NewObject(
C.H5,0);Cg.G_=!!this.Bf;if(!!this.Bf)Cg.AK=this.Bf;return Cg;},N6:function(Aj){var
K;var Go=false;for(K=0;K<10;K=K+1)if(!!this.Bd.Get(K)){var A5=this.Da.Get(K);var
An=this.Bd.Get(K).I;while(!!An&&(An!==this)){A5=B.sn(A5,An.H.slice(0,2));An=An.I;
}if(!An&&(this.Bd.Get(K)!==this)){var tmp=this.Bd.Get(K);this.AQ=K;this.Bd.Set(K
,null);tmp.BY(this.Gh().InitializeUp(K,this.EV.Get(K),this.Ez.Get(K),this.DO.Get(
K),this.B1.Get(K)+1,this.DN.Get(K),false,this.Da.Get(K),this.Eh.Get(K)));if(tmp===
this.Eg)this.Eg=null;this.BroadcastEvent(this.GS().InitializeUp(K,this.B1.Get(K)+
1,false,tmp,this.Da.Get(K)),0x18);}else{this.DO.Set(K,(this.EA.AK-this.GT.Get(K)
)|0);if(this.DO.Get(K)<10)this.DO.Set(K,10);this.AQ=K;this.Bd.Get(K).BY(this.Gh(
).InitializeHold(K,A5,this.Ez.Get(K),this.DO.Get(K),this.B1.Get(K)+1,this.DN.Get(
K),this.Da.Get(K),this.Eh.Get(K)));Go=true;}}if(!Go)this.EA.Ak(false);},GetFPS:function(
){var ticksCount=0;var Lu=0;ticksCount=((new Date).getTime()-B.pk)|0;if(!!this.IO&&(
ticksCount>this.IO))Lu=((this.IN*1000)/((ticksCount-this.IO)|0))|0;this.IN=0;this.
IO=ticksCount;return Lu;},Update:function(){var A;if(!this.GR){this.GR=B._NewObject(
B.Graphics.Canvas,0);this.GR.JA([(A=this.H)[2]-A[0],A[3]-A[1]]);}this.GR.Update(
);return this.UpdateGE20(this.GR);},UpdateGE20:function(A_){if(!this.BeginUpdate(
))return BM;var EE=this.UpdateCanvas(A_,AX);this.EndUpdate();return EE;},EndUpdate:
function(){if(this.BO>0){this.IN=this.IN+1;this.BO=0;}},UpdateCanvas:function(A_
,aOffset){var A;var EE=BM;var N1=[].concat(aOffset,B.so(A_.FrameSize,aOffset));var
K;var Bg=this.BO;this.HZ=this.HZ+1;for(K=0;(K<Bg)&&(K<4);K=K+1){if(this.Cw.Get(K
)>0){this.C6(A_,B.sp(this.BW.Get(K),aOffset),[-aOffset[0],-aOffset[1]],255,true);
EE=B.pA(EE,B.hV(N1,this.BW.Get(K)));}else Bg=Bg+1;}this.HZ=this.HZ-1;if(!((EE[0]>=
EE[2])||(EE[1]>=EE[3])))return B.sp(EE,aOffset);else return EE;},GetUpdateRegion:
function(IA){var K;var Bg=this.BO;if(IA<0)return BM;for(K=0;(K<Bg)&&(K<4);K=K+1){
if(!this.Cw.Get(K)){Bg=Bg+1;IA=IA+1;}else if(K===IA)return this.BW.Get(K);}return BM;
},BeginUpdate:function(){var Oh=true;var fullScreenUpdate=false;var K;if((!Oh&&!
fullScreenUpdate)&&(this.BO>0)){var LN=B.sr(3,B.pn,null);var Kv=this.BO;for(K=0;
K<Kv;K=K+1)LN.Set(K,this.BW.Get(K));for(K=0;K<this.Lz;K=K+1)this.AE(this.IU.Get(
K));for(K=0;K<Kv;K=K+1)this.IU.Set(K,LN.Get(K));this.Lz=Kv;}var Bg;var CY;for(Bg=
0;Bg<(this.BO-1);Bg=Bg+1)if(this.Cw.Get(Bg)>0)for(CY=Bg+1;CY<this.BO;CY=CY+1)if(
this.Cw.Get(CY)>0){var I_=B.rY(B.pA(this.BW.Get(Bg),this.BW.Get(CY)));if(((I_-this.
Cw.Get(Bg))-this.Cw.Get(CY))<0){this.BW.Set(Bg,B.pA(this.BW.Get(Bg),this.BW.Get(
CY)));this.Cw.Set(Bg,I_);this.Cw.Set(CY,0);}}for(K=this.BO-1;K>=0;K=K-1)if(!this.
Cw.Get(K))this.BO=this.BO-1;return this.BO;},DoesNeedUpdate:function(){if(this.BO>
0)return true;return false;},Initialize:function(aSize){this.L([].concat(AX,aSize
));if(this.Ka)this.F=this.F|0x60;else this.F=this.F|0x20;this.AE(this.H);return this;
},SetRootFocus:function(J1){if(J1===this.Ka)return false;this.Ka=J1;if(!J1){if(!
!this.CE)this.CE.Cr(0x0,0x40);if(!!this.E2)this.E2.Db.Cr(0x0,0x40);else this.Cr(
0x0,0x40);}else{if(!!this.E2)this.E2.Db.Cr(0x40,0x0);else this.Cr(0x40,0x0);if(!
!this.CE)this.CE.Cr(0x40,0x0);}return true;},SetUserInputTimestamp:function(N0){
this.Bf=N0;},DriveKeyboardHitting:function(BG,Gd,C$){var A;var Kq=!!this.CZ;if(!
!this.CZ&&((!C$||(this.Fs!==BG))||(this.E1!==Gd))){var Cg=null;var M=(C.Ax.isPrototypeOf(
A=this.CZ)?A:null);var D7=(C.G$.isPrototypeOf(A=this.CZ)?A:null);if(!!this.Fs)Cg=
B._NewObject(C.KeyEvent,0).Initialize(this.Fs,false);if(this.E1!==0x00)Cg=B._NewObject(
C.KeyEvent,0).Initialize2(this.E1,false);if(!!D7)D7.BY(Cg);else if(!!M)M.BY(Cg);
this.Fs=0;this.E1=0x00;this.CZ=null;}if(!!this.CZ){var Cg=null;var M=(C.Ax.isPrototypeOf(
A=this.CZ)?A:null);var D7=(C.G$.isPrototypeOf(A=this.CZ)?A:null);if(!!BG)Cg=B._NewObject(
C.KeyEvent,0).Initialize(BG,true);if(this.E1!==0x00)Cg=B._NewObject(C.KeyEvent,0
).Initialize2(Gd,true);if(!!D7)D7.BY(Cg);else if(!!M)M.BY(Cg);}if(this.IT&&((!C$||(
this.Fs!==BG))||(this.E1!==Gd))){this.Fs=0;this.E1=0x00;this.IT=false;}if((!this.
CZ&&C$)&&(this.J4>0)){this.Fs=BG;this.E1=Gd;this.IT=true;}if((!this.CZ&&C$)&&!this.
IT){if(!!BG)this.CZ=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize(BG,
true));if(Gd!==0x00)this.CZ=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize2(
Gd,true));if(!(C.G$.isPrototypeOf(A=this.CZ)?A:null)&&!(C.Ax.isPrototypeOf(A=this.
CZ)?A:null))this.CZ=null;this.Fs=BG;this.E1=Gd;Kq=Kq||!!this.CZ;}this.Bf=0;return Kq;
},DriveCursorMovement:function(BH){return this.DriveMultiTouchMovement(this.AQ,BH
);},DriveMultiTouchMovement:function(R,BH){if((R<0)||(R>9)){this.Bf=0;return false;
}var Bl=B.sn(BH,this.Da.Get(R));this.Da.Set(R,BH);if(!this.Bd.Get(R)||B.sa(Bl,AX
)){this.Bf=0;return false;}var A5=BH;var An=this.Bd.Get(R).I;while(!!An&&(An!==this
)){A5=B.sn(A5,An.H.slice(0,2));An=An.I;}if(!An&&(this.Bd.Get(R)!==this)){var tmp=
this.Bd.Get(R);this.AQ=R;this.Bd.Set(R,null);tmp.BY(this.Gh().InitializeUp(R,this.
EV.Get(R),this.Ez.Get(R),this.DO.Get(R),this.B1.Get(R)+1,this.DN.Get(R),false,this.
Da.Get(R),this.Eh.Get(R)));if(tmp===this.Eg)this.Eg=null;this.BroadcastEvent(this.
GS().InitializeUp(R,this.B1.Get(R)+1,false,tmp,BH),0x18);}else{this.EV.Set(R,A5);
this.AQ=R;this.Bd.Get(R).BY(this.N5().Initialize(R,A5,this.Ez.Get(R),Bl,this.DO.
Get(R),this.B1.Get(R)+1,this.DN.Get(R),BH,this.Eh.Get(R)));}this.Bf=0;return true;
},DriveCursorHitting:function(C$,R,BH){return this.DriveMultiTouchHitting(C$,R,BH
);},DriveMultiTouchHitting:function(C$,R,BH){var A;if((R<0)||(R>9)){this.Bf=0;return false;
}var ticksCount=this.Bf;var Gj=[].concat([-this.E$,-this.E$],[this.E$+1,this.E$+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.pk)|0;}var Oo=this.Bf;this.
DriveMultiTouchMovement(R,BH);BH=this.Da.Get(R);this.Bf=Oo;if(C$)this.Eh.Set(R,BH
);if((C$&&!this.Bd.Get(R))&&!this.J4){var CX=null;var A5=BH;if(B.se(this.IJ.Get(
R),BH)&&((ticksCount-this.GT.Get(R))<=(((A=this.L1)<0)?A+0x100000000:A)))this.B1.
Set(R,this.B1.Get(R)+1);else this.B1.Set(R,0);this.IJ.Set(R,B.sq(Gj,BH));this.GT.
Set(R,ticksCount);if((!!this.CE&&!!this.CE.I)&&((this.CE.F&0x18)===0x18)){var X=
B.sq(Gj,this.CE.I.Jq(BH));CX=this.CE.Eq(X,R,this.B1.Get(R)+1,null,0x0);}if(!CX){
if(!!this.Eg&&!!this.Eg.I){if(((this.Eg.F&0x8)===0x8)&&((this.Eg.F&0x10)===0x10)
){var X=B.sq(Gj,this.Eg.I.Jq(BH));CX=this.Eg.Eq(X,R,this.B1.Get(R)+1,null,0x0);}
}else if(!!this.E2)CX=this.Eq(B.sq(Gj,BH),R,this.B1.Get(R)+1,this.E2.Db,0x0);else
CX=this.Eq(B.sq(Gj,BH),R,this.B1.Get(R)+1,null,0x0);}if(!!CX){this.BroadcastEvent(
this.GS().InitializeDown(R,this.B1.Get(R)+1,false,CX.Ax,BH),0x18);this.Bd.Set(R,
CX.Ax);this.DN.Set(R,CX.AL);}else{this.Bd.Set(R,null);this.DN.Set(R,AX);this.Bf=
0;return false;}var An=CX.Ax.I;while(!!An&&(An!==this)){A5=B.sn(A5,An.H.slice(0,
2));An=An.I;}this.Ez.Set(R,A5);this.EV.Set(R,A5);this.DO.Set(R,0);this.EA.Ak(true
);this.AQ=R;this.Bd.Get(R).BY(this.Gh().InitializeDown(R,A5,this.B1.Get(R)+1,this.
DN.Get(R),false,BH));this.Bf=0;return true;}if(!C$&&!!this.Bd.Get(R)){var A5=BH;
var An=this.Bd.Get(R).I;while(!!An&&(An!==this)){A5=B.sn(A5,An.H.slice(0,2));An=
An.I;}if(!An)A5=this.EV.Get(R);this.AQ=R;var tmp=this.Bd.Get(R);this.Bd.Set(R,null
);tmp.BY(this.Gh().InitializeUp(R,A5,this.Ez.Get(R),this.DO.Get(R),this.B1.Get(R
)+1,this.DN.Get(R),false,BH,this.Eh.Get(R)));this.BroadcastEvent(this.GS().InitializeUp(
R,this.B1.Get(R)+1,false,tmp,BH),0x18);this.Bf=0;return true;}this.Bf=0;return false;
},Et:function(Ew,Lh,Ef){var Gj=[].concat([-this.E$,-this.E$],[this.E$+1,this.E$+
1]);if(Ew===this)Ew=null;if(!this.Bd.Get(this.AQ))return;var CX;CX=this.Eq(B.sq(
Gj,this.Da.Get(this.AQ)),this.AQ,1,Ew,Ef);if(!!CX&&(this.Bd.Get(this.AQ)!==CX.Ax
))this.Fa(CX.Ax,CX.AL);if(!CX&&(this.Bd.Get(this.AQ)!==Lh))this.Fa(Lh,AX);},Fa:function(
Ew,DK){if(!this.Bd.Get(this.AQ)||(this.Bd.Get(this.AQ)===Ew))return;var tmp=this.
Bd.Get(this.AQ);this.Bd.Set(this.AQ,null);tmp.BY(this.Gh().InitializeUp(this.AQ,
this.EV.Get(this.AQ),this.Ez.Get(this.AQ),this.DO.Get(this.AQ),this.B1.Get(this.
AQ)+1,this.DN.Get(this.AQ),true,this.Da.Get(this.AQ),this.Eh.Get(this.AQ)));this.
BroadcastEvent(this.GS().InitializeUp(this.AQ,this.B1.Get(this.AQ)+1,true,tmp,this.
Da.Get(this.AQ)),0x18);var A5=this.Da.Get(this.AQ);var An=null;if(!!Ew)An=Ew.I;while(
!!An&&(An!==this)){A5=B.sn(A5,An.H.slice(0,2));An=An.I;}if(!An&&(Ew!==this)){this.
Bd.Set(this.AQ,null);this.EA.Ak(false);return;}this.BroadcastEvent(this.GS().InitializeDown(
this.AQ,this.B1.Get(this.AQ)+1,true,Ew,this.Da.Get(this.AQ)),0x18);var ticksCount=
0;ticksCount=((new Date).getTime()-B.pk)|0;this.Bd.Set(this.AQ,Ew);this.DN.Set(this.
AQ,DK);this.Ez.Set(this.AQ,A5);this.EV.Set(this.AQ,A5);this.B1.Set(this.AQ,0);this.
DO.Set(this.AQ,0);this.GT.Set(this.AQ,ticksCount);this.Eh.Set(this.AQ,this.Da.Get(
this.AQ));this.Bd.Get(this.AQ).BY(this.Gh().InitializeDown(this.AQ,A5,this.B1.Get(
this.AQ)+1,this.DN.Get(this.AQ),true,this.Eh.Get(this.AQ)));},_Init:function(aArg
){C.O._Init.call(this,aArg);C.Timer._Init.call(this.EA={J:this},0);(this.Bd=[]).
__proto__=C.Root.Bd;(this.B1=[]).__proto__=C.Root.B1;(this.IJ=[]).__proto__=C.Root.
IJ;(this.DO=[]).__proto__=C.Root.DO;(this.Ez=[]).__proto__=C.Root.Ez;(this.GT=[]
).__proto__=C.Root.GT;(this.EV=[]).__proto__=C.Root.EV;(this.DN=[]).__proto__=C.
Root.DN;(this.Da=[]).__proto__=C.Root.Da;(this.Eh=[]).__proto__=C.Root.Eh;(this.
IU=[]).__proto__=C.Root.IU;(this.Cw=[]).__proto__=C.Root.Cw;(this.BW=[]).__proto__=
C.Root.BW;this.__proto__=C.Root;this.F=0x7F;this.EA.JB(50);this.EA.Ia=[this,this.
N6];},_Done:function(){this.__proto__=C.Root;this.EA._Done();C.O._Done.call(this
);},_ReInit:function(){C.O._ReInit.call(this);this.EA._ReInit();},_Mark:function(
D){var A;C.O._Mark.call(this,D);if((A=this.CZ)&&(A._cycle!=D))A._Mark(A._cycle=D
);if((A=this.E2)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Eg)&&(A._cycle!=D
))A._Mark(A._cycle=D);B.sj(this.Bd,D);if((A=this.GR)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.CE)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.EA)._cycle!=D)A.
_Mark(A._cycle=D);},_className:"Core::Root"};C.Event={AK:0,G_:false,G7:function(
){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk)|0;return ticksCount;},
CP:function(aArg){this.AK=this.G7();},_Init:function(aArg){this.__proto__=C.Event;
this.CP(aArg);B.gb++;},_Done:function(){this.__proto__=C.Event;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=
D);},J:null,_cycle:0,_observers:null,_className:"Core::Event"};C.KeyEvent={BJ:0,
AZ:0,Down:false,Initialize2:function(BG,C$){this.BJ=0;this.AZ=BG;this.Down=C$;if((
BG>=0x30)&&(BG<=0x39))this.BJ=(10+BG)-48;if((BG>=0x41)&&(BG<=0x5A))this.BJ=(105+
BG)-65;if((BG>=0x61)&&(BG<=0x7A))this.BJ=(105+BG)-97;if(BG===0x20)this.BJ=131;if(
!this.BJ)switch(BG){case 0x2B:this.BJ=132;break;case 0x2D:this.BJ=133;break;case
0x2A:this.BJ=134;break;case 0x2F:this.BJ=135;break;case 0x3D:this.BJ=136;break;case
0x2E:this.BJ=137;break;case 0x2C:this.BJ=138;break;case 0x3A:this.BJ=139;break;case
0x3B:this.BJ=140;break;default:;}return this;},Initialize:function(BG,C$){this.BJ=
BG;this.Down=C$;this.AZ=0x00;var J6=BG-10;var J5=BG-105;if((J6>=0)&&(J6<=9))this.
AZ=(48+J6)&0xFFFF;if((J5>=0)&&(J5<=25))this.AZ=(65+J5)&0xFFFF;if(BG===131)this.AZ=
0x20;if(this.AZ===0x00)switch(BG){case 132:this.AZ=0x2B;break;case 133:this.AZ=0x2D;
break;case 134:this.AZ=0x2A;break;case 135:this.AZ=0x2F;break;case 136:this.AZ=0x3D;
break;case 137:this.AZ=0x2E;break;case 138:this.AZ=0x2C;break;case 139:this.AZ=0x3A;
break;case 140:this.AZ=0x3B;break;default:;}return this;},L9:function(Lg){switch(
Lg){case 141:return((this.AZ>=0x41)&&(this.AZ<=0x5A))||((this.AZ>=0x61)&&(this.AZ<=
0x7A));case 142:return(((this.AZ>=0x41)&&(this.AZ<=0x5A))||((this.AZ>=0x61)&&(this.
AZ<=0x7A)))||((this.AZ>=0x30)&&(this.AZ<=0x39));case 143:return(this.AZ>=0x30)&&(
this.AZ<=0x39);case 144:return(((this.AZ>=0x41)&&(this.AZ<=0x46))||((this.AZ>=0x61
)&&(this.AZ<=0x66)))||((this.AZ>=0x30)&&(this.AZ<=0x39));case 145:return this.AZ
!==0x00;case 146:return(this.AZ===0x00)&&!!this.BJ;case 147:return(((this.BJ===6
)||(this.BJ===7))||(this.BJ===4))||(this.BJ===5);case 148:return(this.AZ!==0x00)||
!!this.BJ;default:;}return Lg===this.BJ;},_Init:function(aArg){C.Event._Init.call(
this,aArg);this.__proto__=C.KeyEvent;},_className:"Core::KeyEvent"};C.H5={Ij:null
,CO:B.pm,DI:0,Cs:0,Down:false,CK:false,InitializeUp:function(R,CQ,Gc,J3,D6){this.
Down=false;this.Cs=R;this.DI=CQ;this.CO=D6;this.Ij=J3;this.CK=Gc;return this;},InitializeDown:
function(R,CQ,Gc,J3,D6){this.Down=true;this.Cs=R;this.DI=CQ;this.CO=D6;this.Ij=J3;
this.CK=Gc;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.
__proto__=C.H5;},_Mark:function(D){var A;C.Event._Mark.call(this,D);if((A=this.Ij
)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"};C.G2={
DC:B.pm,CO:B.pm,DI:0,C9:0,B_:B.pm,AU:B.pm,Cs:0,Down:false,CK:false,InitializeHold:
function(R,Fk,Hz,HA,CQ,DK,D6,Hy){this.Down=true;this.Cs=R;this.AU=B.so(Fk,DK);this.
B_=B.so(Hz,DK);this.C9=HA;this.DI=CQ;this.CO=D6;this.DC=Hy;return this;},InitializeUp:
function(R,Fk,Hz,HA,CQ,DK,Gc,D6,Hy){this.Down=false;this.Cs=R;this.AU=B.so(Fk,DK
);this.B_=B.so(Hz,DK);this.C9=HA;this.DI=CQ;this.CK=Gc;this.CO=D6;this.DC=Hy;return this;
},InitializeDown:function(R,Fk,CQ,DK,Gc,D6){this.Down=true;this.Cs=R;this.AU=B.so(
Fk,DK);this.B_=B.so(Fk,DK);this.C9=0;this.DI=CQ;this.CK=Gc;this.CO=D6;this.DC=D6;
return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=
C.G2;},_className:"Core::CursorEvent"};C.G5={DC:B.pm,CO:B.pm,DI:0,C9:0,AL:B.pm,B_:
B.pm,AU:B.pm,Cs:0,Initialize:function(R,Fk,Hz,aOffset,HA,NZ,DK,D6,Hy){this.Cs=R;
this.AU=B.so(Fk,DK);this.B_=B.so(Hz,DK);this.AL=aOffset;this.C9=HA;this.DI=NZ;this.
CO=D6;this.DC=Hy;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg
);this.__proto__=C.G5;},_className:"Core::DragEvent"};C.Hk={AB:B.pm,Space:0,H7:0
,C6:function(A_,aClip,aOffset,A7,aBlend){},L:function(E){var A;if(B.sb(E,this.H)
)return;var Fv=[(A=this.H)[2]-A[0],A[3]-A[1]];var IW=[E[2]-E[0],E[3]-E[1]];var Gt=
!B.sa(Fv,IW);var Bl=B.sn(E.slice(0,2),this.H.slice(0,2));if(!B.sa(Bl,AX)&&!Gt){var
M=this.AJ;while(!!M&&!((M.F&0x200)===0x200)){if(((M.F&0x400)===0x400)){var tmp=((
M.F&0x100)===0x100);M.H9(Bl,tmp);}M=M.AJ;}}if((Gt&&(Fv[0]>0))&&(Fv[1]>0)){var AA=
B.sq(this.H,this.AB);var M=this.AJ;var HF=0x14;while(!!M&&!((M.F&0x200)===0x200)
){if(((M.F&0x400)===0x400)){if(!!M.A$&&(M.A$.Gq!==this))M.A$=null;if(!M.A$&&((M.
Ff!==HF)||!!this.H7))M.HH(AA,this);}M=M.AJ;}}C.BD.L.call(this,E);if(!!this.I&&Gt
){this.F=this.F|0x1000;if(!((this.I.F&0x2000)===0x2000)){this.I.F=this.I.F|0x4000;
B.kD([A=this.I,A.Cx],this);}}},_Init:function(aArg){C.BD._Init.call(this,aArg);this.
__proto__=C.Hk;this.F=0x203;},_className:"Core::Outline"};C.Cd={Fr:B.sr(8,null,null
),B6:null,Ax:null,DF:null,IX:null,Cn:null,BZ:null,Ke:-1,Kd:0,Dr:-1,C5:0,Cp:0,BK:-
1,ME:0,AB:0,FT:-1,FQ:0,T:24,Bs:0,FJ:null,Dd:false,Ly:false,C6:function(A_,aClip,
aOffset,A7,aBlend){C.O.C6.call(this,A_,aClip,aOffset,A7,aBlend);},DispatchEvent:
function(Az){var A;var result=null;if(((this.FT>=0)&&(this.FT<this.Bs))&&!this.C8
){this.BK=this.FT;this.Ax=(C.Ax.isPrototypeOf(A=B._NewObject(this.FJ,0))?A:null);
if(!!this.BF)this.BF.AJ=this.Ax;else this.BE=this.Ax;this.Ax.AC=this.BF;this.BF=
this.Ax;this.Ax.I=this;(A=this.BZ)?A[1].call(A[0],this):null;var An=(C.O.isPrototypeOf(
A=this.Ax)?A:null);if(!!An)result=An.DispatchEvent(Az);else result=this.Ax.BY(Az
);if(!!this.Ax.AC)this.Ax.AC.AJ=null;else this.BE=null;this.BF=this.Ax.AC;this.Ax.
AC=null;this.Ax.I=null;this.Ax=null;}if(!result)result=C.O.DispatchEvent.call(this
,Az);return result;},JN:function(Lk){var A;if(!this.FJ){B.kD(this.Cn,this);return;
}this.Ly=true;var HP=0;if(!this.Dd)HP=this.FQ;var Cv=((-HP-this.AB)/this.T)|0;var
C3=((((((A=this.H)[3]-A[1])-HP)-this.AB)-1)/this.T)|0;var II=(((((A=this.H)[3]-A[
1])+this.T)-1)/this.T)|0;if(II<1)II=1;var ID=(II/2)|0;var IE=(II/3)|0;if(!ID)ID=
1;if(!IE)IE=1;if(Cv<this.C5){Cv=Cv-ID;C3=C3+IE;}else if(C3>this.Dr){Cv=Cv-IE;C3=
C3+ID;}else{Cv=this.C5;C3=this.Dr;}if(!this.Dd){if(Cv>=this.Bs){Cv=0;C3=-1;}else
if(C3<0){Cv=0;C3=-1;}if(C3>=this.Bs)C3=this.Bs-1;if(Cv<0)Cv=0;}else if(this.Bs<=
0){Cv=0;C3=-1;}var I4=this.C5;var I5=this.Dr;var Kj=0;var Kk=-1;if(Cv>I4)I4=Cv;if(
C3<I5)I5=C3;if(I4<=I5){while((this.Dr<C3)&&(this.C5<I4)){this.HU();this.Lq();}while((
this.C5>Cv)&&(this.Dr>I5)){this.LH();this.Lp();}}else{this.Dr=(this.Dr-this.C5)+
Cv;this.C5=Cv;Kj=this.C5;Kk=this.Dr;}while(this.C5<Cv)this.HU();while(this.Dr>C3
)this.LH();while(this.C5>Cv)this.Lp();while(this.Dr<C3)this.Lq();var Al=this.BE;
var CG=this.C5;var A5=[0,(HP+this.AB)+(CG*this.T)];var Bc=(A=this.H)[3]-A[1];var
Lv=this.T;var HE=null;while(!!Al){var A9=B.sn(A5,Al.GetExtent().slice(0,2));var Kh=
A5[1];var Ki=A5[1]+Lv;var Ch=CG;if(this.Dd){if(Ch<0)Ch=this.Bs-(-Ch%this.Bs);if(
Ch>0)Ch=Ch%this.Bs;}if(!B.sa(A9,AX))Al.H9(A9,true);if(((((Ch>=this.Kd)&&(Ch<=this.
Ke))||((CG>=Kj)&&(CG<=Kk)))&&(Kh<Bc))&&(Ki>0)){this.Ax=Al;this.BK=Ch;(A=this.BZ)?
A[1].call(A[0],this):null;}Al=Al.AJ;CG=CG+1;A5=[A5[0],A5[1]+this.T];}CG=this.C5;
Al=this.BE;A5=[0,(HP+this.AB)+(CG*this.T)];while(!!Al){var Kh=A5[1];var Ki=A5[1]+
Lv;var Ch=CG;if(this.Dd){if(Ch<0)Ch=this.Bs-(-Ch%this.Bs);if(Ch>0)Ch=Ch%this.Bs;
}if((((Ch>=this.Kd)&&(Ch<=this.Ke))||((CG>=Kj)&&(CG<=Kk)))&&!((Kh<Bc)&&(Ki>0))){
this.Ax=Al;this.BK=Ch;(A=this.BZ)?A[1].call(A[0],this):null;}if(((Ch===this.FT)&&
this.Dd)&&!!HE){var IB=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(B.rY(B.hV(Al.GetExtent(
),IB))>B.rY(B.hV(HE.GetExtent(),IB)))HE=Al;}else if(Ch===this.FT)HE=Al;Al=Al.AJ;
CG=CG+1;A5=[A5[0],A5[1]+this.T];}this.Kd=0;this.Ke=-1;this.Ax=null;this.BK=-1;this.
Ec(HE);this.Ly=false;B.kD(this.Cn,this);},Jm:function(P,aFilter){return null;},Ie:
function(P,Ce){throw new Error(JR);},KS:function(P){throw new Error(JS);},S:function(
P,Ce){throw new Error(JT);},LH:function(){var Al=this.BF;if(!Al)return null;if(Al===
this.C8)this.Ec(null);this.Dr=this.Dr-1;if(!!Al.AC)Al.AC.AJ=null;else this.BE=null;
this.BF=Al.AC;Al.AC=null;Al.I=null;Al.A$=null;if(this.Cp<8){this.Fr.Set(this.Cp,
Al);this.Cp=this.Cp+1;}return Al;},Lq:function(){var A;var Al=null;if(!this.Cp){
Al=(C.Ax.isPrototypeOf(A=B._NewObject(this.FJ,0))?A:null);Al.AT(0x1D);}else{this.
Cp=this.Cp-1;Al=this.Fr.Get(this.Cp);}this.Dr=this.Dr+1;var FD=this.Dr;if(this.Dd
){if(FD<0)FD=this.Bs-(-FD%this.Bs);if(FD>0)FD=FD%this.Bs;}this.BK=FD;this.Ax=Al;(
A=this.BZ)?A[1].call(A[0],this):null;this.Ax=null;this.BK=-1;if(!!this.BF)this.BF.
AJ=Al;Al.I=this;Al.AC=this.BF;this.BF=Al;if(!this.BE)this.BE=Al;if(FD===this.FT)
this.Ec(Al);return Al;},N3:function(){while(this.Cp>0){this.Cp=this.Cp-1;this.Fr.
Set(this.Cp,null);}},HU:function(){var Al=this.BE;if(!Al)return null;if(Al===this.
C8)this.Ec(null);this.C5=this.C5+1;if(!!Al.AJ)Al.AJ.AC=null;else this.BF=null;this.
BE=Al.AJ;Al.AJ=null;Al.I=null;Al.A$=null;if(this.Cp<8){this.Fr.Set(this.Cp,Al);this.
Cp=this.Cp+1;}return Al;},Lp:function(){var A;var Al=null;if(!this.Cp){Al=(C.Ax.
isPrototypeOf(A=B._NewObject(this.FJ,0))?A:null);Al.AT(0x1D);}else{this.Cp=this.
Cp-1;Al=this.Fr.Get(this.Cp);}this.C5=this.C5-1;var FC=this.C5;if(this.Dd){if(FC<
0)FC=this.Bs-(-FC%this.Bs);if(FC>0)FC=FC%this.Bs;}this.BK=FC;this.Ax=Al;(A=this.
BZ)?A[1].call(A[0],this):null;this.Ax=null;this.BK=-1;if(!!this.BE)this.BE.AC=Al;
Al.I=this;Al.AJ=this.BE;this.BE=Al;if(!this.BF)this.BF=Al;if(FC===this.FT)this.Ec(
Al);return Al;},Oc:function(Aj){var A;this.B6.A0=null;this.B6.GV=null;this.B6=null;(
A=this.IX)?A[1].call(A[0],this):null;},Oe:function(Aj){this.De(this.DF.AL[1]);},
Og:function(Aj){var A;if(!!this.B6){this.B6.Ak(false);this.B6.A0=null;this.B6.GV=
null;this.B6=null;}if(!this.Dd){var X=this.KH(0,this.Bs-1);var IZ=this.H.slice(0
,2);X=B.sY(X,X[1]-this.FQ);X=[].concat(X.slice(0,3),X[3]+this.ME);if(X[0]>IZ[0])
X=[].concat(IZ[0],X.slice(1,4));if(X[1]>IZ[1])X=B.sY(X,IZ[1]);var Bl=X[1]-this.H[
1];var Km=((A=this.H)[3]-A[1])-(X[3]-X[1]);if(Km>0)Km=0;this.DF.AL=[0,this.AB];this.
DF.A3=[0,(this.AB+Km)-Bl];this.DF.A2=[0,this.AB-Bl];}else{var Bl=32000-(32000%this.
T);this.DF.AL=[0,this.AB];this.DF.A3=[0,this.AB-Bl];this.DF.A2=[0,this.AB+Bl];}}
,GH:function(E){if(E===this.DF)return;if(!!E&&!!E.GW){B.ta("%s%$%*%s",JU,E,JV);throw new
Error(Iq);}if(!!this.DF){this.DF.I1=null;this.DF.GW=null;}this.DF=E;if(!!E){E.I1=[
this,this.Og];E.GW=[this,this.Oe];}},De:function(E){var A;if(this.Dd&&(this.Bs>0
)){var E6=this.Bs*this.T;if(E<0)E=E6-(-E%E6);if(E>0)E=E%E6;if(E>0)E=E-E6;}if(E===
this.AB)return;this.AB=E;this.GB();this.AE([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);}
,Cz:function(E){var A;if(E<1)E=1;if(E===this.T)return;this.T=E;while(!!this.HU()
);this.GB();this.AE([0,0,(A=this.H)[2]-A[0],A[3]-A[1]]);},CA:function(E){var A;if(
E<0)E=0;if(E===this.Bs)return;var X=[0,0,(A=this.H)[2]-A[0],A[3]-A[1]];if(!this.
Dd){if(E>this.Bs){X=B.sY(X,(this.FQ+this.AB)+(this.Bs*this.T));X=[].concat(X.slice(
0,3),(this.FQ+this.AB)+(E*this.T));}else{X=B.sY(X,(this.FQ+this.AB)+(E*this.T));
X=[].concat(X.slice(0,3),(this.FQ+this.AB)+(this.Bs*this.T));}}else while(!!this.
HU());this.Bs=E;this.GB();this.AE(X);},Cy:function(E){var A;if(E===this.FJ)return;
this.FJ=E;while(!!this.HU());this.N3();this.GB();this.AE([0,0,(A=this.H)[2]-A[0]
,A[3]-A[1]]);},Er:function(Ix,Li,NY,Gf){var A;if((Ix<0)||(Ix>=this.Bs))return;var
Ai=this.KH(Ix,Ix);var AA=this.H;var HI=B.hV(Ai,AA);if((!Li&&!((HI[0]>=HI[2])||(HI[
1]>=HI[3])))||(Li&&B.sb(HI,Ai))){(Gf)?Gf[1].call(Gf[0],this):null;return;}var A9=
0;if(Ai[3]>AA[3])A9=Ai[3]-AA[3];if(A9>(Ai[1]-AA[1]))A9=Ai[1]-AA[1];if(!!this.B6){
this.B6.Ak(false);this.B6.A0=null;this.B6.GV=null;this.IX=null;}this.B6=NY;if(!this.
B6){this.De(this.AB-A9);(Gf)?Gf[1].call(Gf[0],this):null;}else{this.B6.Ak(false);
this.B6.BL(1);this.B6.A0=[this,this.Mh,this.De];this.B6.AW=this.AB;this.B6.Bn=this.
AB-A9;this.B6.GV=[this,this.Oc];this.B6.JC(false);this.B6.Ak(true);this.IX=Gf;}}
,KH:function(Hx,Iy){if(Hx<0)Hx=0;if(Iy>=this.Bs)Iy=this.Bs-1;if(Hx>Iy)return BM;
var X=this.H;var A9=this.AB;if(this.Dd){var E6=this.Bs*this.T;if(A9<0)A9=E6-(-A9
%E6);if(A9>0)A9=A9%E6;if(A9>0)A9=A9-E6;}else A9=A9+this.FQ;X=B.sY(X,(X[1]+A9)+(Hx
*this.T));X=[].concat(X.slice(0,3),X[1]+(((Iy-Hx)+1)*this.T));return X;},Mh:function(
){return this.AB;},_Init:function(aArg){C.O._Init.call(this,aArg);(this.Fr=[]).__proto__=
C.Cd.Fr;this.__proto__=C.Cd;this.L(Ir);this.FJ=B.te.Text;},_Mark:function(D){var
A;C.O._Mark.call(this,D);B.sj(this.Fr,D);if((A=this.B6)&&(A._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.Ax)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DF)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.IX)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.Cn)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.BZ)&&((A=A[
0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::VerticalList"};C.V={C_:null
,Jz:null,Jx:null,Hg:null,Cm:null,D2:null,ES:0,Cs:0,AK:0,DI:0,C9:0,AL:B.pm,B_:B.pm
,AU:B.pm,JI:1000,EO:8,Df:0,Hb:-1,L_:1,L$:1,L4:false,Down:false,Fc:false,CK:false
,Fl:false,C6:function(A_,aClip,aOffset,A7,aBlend){},BY:function(Az){var A;var AI=(
C.G2.isPrototypeOf(Az)?Az:null);var AN=(C.G5.isPrototypeOf(Az)?Az:null);var IQ=this.
Fc;if(!AI&&!AN)return null;if(!!AI){this.Fl=AI.Down;this.Down=AI.Down;this.Fc=this.
Jp(AI.AU);this.B_=AI.B_;this.AU=AI.AU;this.AL=AX;this.C9=AI.C9;this.DI=AI.DI;this.
CK=AI.CK;this.Cs=AI.Cs;this.ES=AI.Cs;this.AK=AI.AK;if(AI.Down&&!AI.C9)IQ=false;}
if(!!AN){this.Down=true;this.Fc=this.Jp(AN.AU);this.B_=AN.B_;this.AU=AN.AU;this.
AL=AN.AL;this.C9=AN.C9;this.DI=AN.DI;this.Cs=AN.Cs;this.ES=AN.Cs;this.CK=false;this.
AK=AN.AK;(A=this.C_)?A[1].call(A[0],this):null;}if((!!AI&&this.Down)&&!this.C9)(
A=this.D2)?A[1].call(A[0],this):null;if((!!AI&&this.Down)&&(this.C9>0))(A=this.Hg
)?A[1].call(A[0],this):null;if((this.Down&&this.Fc)&&!IQ)(A=this.Jx)?A[1].call(A[
0],this):null;if(((this.Down&&!this.Fc)&&IQ)||((!this.Down&&this.Fc)&&IQ))(A=this.
Jz)?A[1].call(A[0],this):null;if(!!AI&&!this.Down)(A=this.Cm)?A[1].call(A[0],this
):null;if(!!this.Df){if(((((this.Df&0x10)===0x10)&&!!AI)&&AI.Down)&&(AI.C9>=this.
JI))this.CN().Et(null,this,0x10);if((((this.Df&0x1)===0x1)&&!!AN)&&((AN.CO[1]-AN.
DC[1])<=-this.EO))this.CN().Et(null,this,0x1);if((((this.Df&0x2)===0x2)&&!!AN)&&((
AN.CO[1]-AN.DC[1])>=this.EO))this.CN().Et(null,this,0x2);if((((this.Df&0x4)===0x4
)&&!!AN)&&((AN.CO[0]-AN.DC[0])<=-this.EO))this.CN().Et(null,this,0x4);if((((this.
Df&0x8)===0x8)&&!!AN)&&((AN.CO[0]-AN.DC[0])>=this.EO))this.CN().Et(null,this,0x8
);}return this;},Eq:function(AH,R,CQ,EQ,Ef){var A;if(!!EQ&&(EQ!==this))return null;
if((CQ<this.L$)||(CQ>this.L_))return null;if((this.Hb>=0)&&(R!==this.Hb))return null;
if((!this.L4&&this.Fl)&&(R!==this.ES))return null;if(!!(Ef&this.Df))return null;
if(this.L7()){var Ai=B.hV(AH,this.GetExtent());if(!((Ai[0]>=Ai[2])||(Ai[1]>=Ai[3
]))){var CT=B.rZ(AH);var Bl=AX;if(CT[0]<Ai[0])Bl=[Ai[0]-CT[0],Bl[1]];if(CT[0]>=Ai[
2])Bl=[(Ai[2]-CT[0])-1,Bl[1]];if(CT[1]<Ai[1])Bl=[Bl[0],Ai[1]-CT[1]];if(CT[1]>=Ai[
3])Bl=[Bl[0],(Ai[3]-CT[1])-1];return B._NewObject(C.G3,0).Initialize(this,Bl);}}
else{var Be=B.sr(9,B.pm,null);var K;Be.Set(0,B.rZ(AH));Be.Set(1,Be.Get(0));Be.Set(
2,Be.Get(0));Be.Set(3,Be.Get(0));Be.Set(4,Be.Get(0));Be.Set(1,[AH[0],Be.Get(1)[1
]]);Be.Set(2,[Be.Get(2)[0],AH[1]]);Be.Set(3,[AH[2],Be.Get(3)[1]]);Be.Set(4,[Be.Get(
4)[0],AH[3]]);Be.Set(5,AH.slice(0,2));Be.Set(6,[AH[2],AH[1]]);Be.Set(7,[AH[0],AH[
3]]);Be.Set(8,AH.slice(2,4));for(K=0;K<9;K=K+1)if(this.Jp(Be.Get(K)))return B._NewObject(
C.G3,0).Initialize(this,B.sn(Be.Get(K),Be.Get(0)));}return null;},Ak:function(E){
if(E)this.Cr(0x10,0x0);else this.Cr(0x0,0x10);},_Init:function(aArg){C.Ho._Init.
call(this,aArg);this.__proto__=C.V;this.F=0x11B;},_Mark:function(D){var A;C.Ho._Mark.
call(this,D);if((A=this.C_)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Jz)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Jx)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Hg)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.Cm)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.D2)&&((A=A[0
])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SimpleTouchHandler"};C.Ay={
timer:null,I1:null,LG:null,GW:null,JD:null,BC:null,Bw:null,ES:0,Ej:5000.000000,Gs:
0,IP:B.pm,Gl:0,Ey:0,AG:0,FB:0,GY:0,Gk:0,Ex:0,AF:0,FA:0,Fz:0,B_:B.pm,KA:B.pm,Hb:-
1,A2:B.pm,A3:B.pm,AL:B.pm,Ih:B.pm,DG:B.pm,Ig:B.pm,KV:5.000000,MG:500,JI:1000,EO:
8,Df:0,JH:-1,KU:200,EP:0,KF:0.500000,MH:true,JL:true,JK:true,GK:false,DR:false,DS:
false,Fl:false,C6:function(A_,aClip,aOffset,A7,aBlend){},BY:function(Az){var A;var
AI=(C.G2.isPrototypeOf(Az)?Az:null);var AN=(C.G5.isPrototypeOf(Az)?Az:null);var BN=(
C.H5.isPrototypeOf(Az)?Az:null);if(!!BN&&((BN.Ij===this)||BN.CK))return null;if((
!!BN&&BN.Down)&&(this.Fl||!this.GK))return null;if((!!BN&&BN.Down)&&!B.se(this.H
,this.I.Jq(BN.CO)))return null;if((!!BN&&!BN.Down)&&(!this.Fl||(this.ES!==BN.Cs)
))return null;if((!AI&&!AN)&&!BN)return null;if(!!AI){this.Fl=AI.Down;this.ES=AI.
Cs;this.B_=AI.B_;}if(!!AN)this.ES=AN.Cs;if(!!BN){this.Fl=BN.Down;this.ES=BN.Cs;}
if(!!BN&&BN.Down){this.Ku();this.AF=(((BN.AK-this.Fz)*0.001000)*this.Ex)+this.AF;
this.AG=(((BN.AK-this.GY)*0.001000)*this.Ey)+this.AG;if(this.DR)this.AF=0.000000;
if(this.DS)this.AG=0.000000;this.Ex=0.000000;this.Ey=0.000000;this.DR=false;this.
DS=false;this.IP=this.AL;this.Gs=BN.AK;return this;}if((!!AI&&AI.Down)&&!AI.C9){
this.Ku();this.AF=(((AI.AK-this.Fz)*0.001000)*this.Ex)+this.AF;this.AG=(((AI.AK-
this.GY)*0.001000)*this.Ey)+this.AG;if(this.DR||!this.GK)this.AF=0.000000;if(this.
DS||!this.GK)this.AG=0.000000;this.Ex=0.000000;this.Ey=0.000000;this.DR=false;this.
DS=false;if(!this.GK){this.GK=true;(A=this.Bw)?A[1].call(A[0],this):null;(A=this.
I1)?A[1].call(A[0],this):null;}this.IP=this.AL;this.Gs=AI.AK;}if(!!AN){var AD=B.
sn(AN.AU,AN.B_);var Ao=this.AL;if(this.JK)Ao=[this.IP[0]+AD[0],Ao[1]];if(this.JL
)Ao=[Ao[0],this.IP[1]+AD[1]];if(this.MH){if(Ao[0]<this.A3[0])Ao=[this.A3[0]+(((Ao[
0]-this.A3[0])/2)|0),Ao[1]];else if(Ao[0]>this.A2[0])Ao=[this.A2[0]+(((Ao[0]-this.
A2[0])/2)|0),Ao[1]];if(Ao[1]<this.A3[1])Ao=[Ao[0],this.A3[1]+(((Ao[1]-this.A3[1]
)/2)|0)];else if(Ao[1]>this.A2[1])Ao=[Ao[0],this.A2[1]+(((Ao[1]-this.A2[1])/2)|0
)];}else{if(Ao[0]<this.A3[0])Ao=[this.A3[0],Ao[1]];else if(Ao[0]>this.A2[0])Ao=[
this.A2[0],Ao[1]];if(Ao[1]<this.A3[1])Ao=[Ao[0],this.A3[1]];else if(Ao[1]>this.A2[
1])Ao=[Ao[0],this.A2[1]];}if(!B.sa(Ao,this.AL)){this.KA=B.sn(Ao,this.AL);this.AL=
Ao;(A=this.GW)?A[1].call(A[0],this):null;(A=this.JD)?A[1].call(A[0],this):null;}
}if((!!BN&&!BN.Down)&&((BN.AK-this.Gs)>=(((A=this.KU)<0)?A+0x100000000:A))){this.
AF=0.000000;this.AG=0.000000;}if((!!AI&&AI.Down)&&((AI.AK-this.Gs)>=(((A=this.KU
)<0)?A+0x100000000:A))){this.AF=0.000000;this.AG=0.000000;}if((!!AI&&!AI.Down)&&(
this.JH>=0)){var B9=B.sn(AI.AU,AI.B_);if(((B9[0]*B9[0])+(B9[1]*B9[1]))<=(this.JH
*this.JH)){this.AF=0.000000;this.AG=0.000000;}}if(!!AN&&(AN.AK>this.Gs)){var AD=
AN.AL;var Lw=1000.000000/(AN.AK-this.Gs);var E3=0.000000;var E4=0.000000;if(this.
JK)E3=AD[0]*Lw;if(this.JL)E4=AD[1]*Lw;if((E3*this.AF)<0.000000)this.AF=0.000000;
if((E4*this.AG)<0.000000)this.AG=0.000000;this.AF=(this.AF+E3)*0.500000;this.AG=(
this.AG+E4)*0.500000;if(this.EP>0.000000){if(this.AF>this.EP)this.AF=this.EP;if(
this.AF<-this.EP)this.AF=-this.EP;if(this.AG>this.EP)this.AG=this.EP;if(this.AG<-
this.EP)this.AG=-this.EP;}this.Gs=AN.AK;}if(!!this.Df){if(((((this.Df&0x10)===0x10
)&&!!AI)&&AI.Down)&&(AI.C9>=this.JI))this.CN().Et(null,this,0x10);if((((this.Df&
0x1)===0x1)&&!!AN)&&((AN.CO[1]-AN.DC[1])<=-this.EO))this.CN().Et(null,this,0x1);
if((((this.Df&0x2)===0x2)&&!!AN)&&((AN.CO[1]-AN.DC[1])>=this.EO))this.CN().Et(null
,this,0x2);if((((this.Df&0x4)===0x4)&&!!AN)&&((AN.CO[0]-AN.DC[0])<=-this.EO))this.
CN().Et(null,this,0x4);if((((this.Df&0x8)===0x8)&&!!AN)&&((AN.CO[0]-AN.DC[0])>=this.
EO))this.CN().Et(null,this,0x8);}if((!AI||AI.Down)&&!BN)return this;if(!!AI&&AI.
CK){this.AF=0.000000;this.AG=0.000000;}if((this.AL[0]<=this.A3[0])||(this.AL[0]>=
this.A2[0]))this.AF=0.000000;else if(this.AF===0.000000){var AR=this.AL[0];var AV=
this.A3[0]+this.Ih[0];var AP=this.A2[0]-this.Ig[0];if(AR<AV)AP=this.A3[0];else if(
AR>AP)AV=this.A2[0];else if(this.DG[0]<=1){AV=AR;AP=AR;}else{var El=(AP-AR)%this.
DG[0];AP=(AR-this.DG[0])+El;AV=AR+El;if(AP<this.A3[0])AP=this.A3[0];if(AV>this.A2[
0])AV=this.A2[0];}if((AV-AR)<=(AR-AP))AR=AV;else AR=AP;if(AR!==this.AL[0]){var Cf=
AR-this.AL[0];if(Cf>0.000000)this.AF=Math.sqrt((Cf*2.000000)*this.Ej)+20.000000;
if(Cf<0.000000)this.AF=-Math.sqrt((-Cf*2.000000)*this.Ej)-20.000000;this.Ex=(400.000000-(
this.AF*this.AF))/(2.000000*Cf);this.Gk=AR;}}else{var Op=this.AF*this.AF;var Cf=
Op/(2.000000*this.Ej);var AR=this.AL[0];if(this.AF>0.000000)AR=AR+(Cf|0);if(this.
AF<0.000000)AR=AR-(Cf|0);if(AR>this.A2[0])AR=this.A2[0];else if(AR<this.A3[0])AR=
this.A3[0];var LL=AR;var AV=this.A3[0]+this.Ih[0];var AP=this.A2[0]-this.Ig[0];if(
AR<AV)AP=this.A3[0];else if(AR>AP)AV=this.A2[0];else if(this.DG[0]<=1){AV=AR;AP=
AR;}else{var El=(AP-AR)%this.DG[0];AP=(AR-this.DG[0])+El;AV=AR+El;if(AP<this.A3[
0])AP=this.A3[0];if(AV>this.A2[0])AV=this.A2[0];}if(this.AF>0.000000)if(AP<=this.
AL[0])AR=AV;else if((AR-AP)<(AV-AR))AR=AP;else AR=AV;else if(AV>=this.AL[0])AR=AP;
else if((AR-AP)>(AV-AR))AR=AV;else AR=AP;if(AR!==this.AL[0]){Cf=AR-this.AL[0];if(
AR!==LL){var E8=AR-LL;if(E8>0.000000)this.AF=this.AF+Math.sqrt((E8*2.000000)*this.
Ej);if(E8<0.000000)this.AF=this.AF-Math.sqrt((-E8*2.000000)*this.Ej);}if(this.AF>
0.000000)this.AF=this.AF+20.000000;if(this.AF<0.000000)this.AF=this.AF-20.000000;
this.Ex=(400.000000-(this.AF*this.AF))/(2.000000*Cf);this.Gk=AR;}else this.AF=0.000000;
}if((this.AL[1]<=this.A3[1])||(this.AL[1]>=this.A2[1]))this.AG=0.000000;else if(
this.AG===0.000000){var AS=this.AL[1];var AV=this.A3[1]+this.Ih[1];var AP=this.A2[
1]-this.Ig[1];if(AS<AV)AP=this.A3[1];else if(AS>AP)AV=this.A2[1];else if(this.DG[
1]<=1){AV=AS;AP=AS;}else{var El=(AP-AS)%this.DG[1];AP=(AS-this.DG[1])+El;AV=AS+El;
if(AP<this.A3[1])AP=this.A3[1];if(AV>this.A2[1])AV=this.A2[1];}if((AV-AS)<=(AS-AP
))AS=AV;else AS=AP;if(AS!==this.AL[1]){var Cf=AS-this.AL[1];if(Cf>0.000000)this.
AG=Math.sqrt((Cf*2.000000)*this.Ej)+20.000000;if(Cf<0.000000)this.AG=-Math.sqrt((-
Cf*2.000000)*this.Ej)-20.000000;this.Ey=(400.000000-(this.AG*this.AG))/(2.000000
*Cf);this.Gl=AS;}}else{var Oq=this.AG*this.AG;var Cf=Oq/(2.000000*this.Ej);var AS=
this.AL[1];if(this.AG>0.000000)AS=AS+(Cf|0);if(this.AG<0.000000)AS=AS-(Cf|0);if(
AS>this.A2[1])AS=this.A2[1];else if(AS<this.A3[1])AS=this.A3[1];var LM=AS;var AV=
this.A3[1]+this.Ih[1];var AP=this.A2[1]-this.Ig[1];if(AS<AV)AP=this.A3[1];else if(
AS>AP)AV=this.A2[1];else if(this.DG[1]<=1){AV=AS;AP=AS;}else{var El=(AP-AS)%this.
DG[1];AP=(AS-this.DG[1])+El;AV=AS+El;if(AP<this.A3[1])AP=this.A3[1];if(AV>this.A2[
1])AV=this.A2[1];}if(this.AG>0.000000)if(AP<=this.AL[1])AS=AV;else if((AS-AP)<(AV-
AS))AS=AP;else AS=AV;else if(AV>=this.AL[1])AS=AP;else if((AS-AP)>(AV-AS))AS=AV;
else AS=AP;if(AS!==this.AL[1]){Cf=AS-this.AL[1];if(AS!==LM){var E8=AS-LM;if(E8>0.000000
)this.AG=this.AG+Math.sqrt((E8*2.000000)*this.Ej);if(E8<0.000000)this.AG=this.AG-
Math.sqrt((-E8*2.000000)*this.Ej);}if(this.AG>0.000000)this.AG=this.AG+20.000000;
if(this.AG<0.000000)this.AG=this.AG-20.000000;this.Ey=(400.000000-(this.AG*this.
AG))/(2.000000*Cf);this.Gl=AS;}else this.AG=0.000000;}if(!!AI)this.Fz=AI.AK;if(!
!BN)this.Fz=BN.AK;this.GY=this.Fz;this.FA=this.AL[0];this.FB=this.AL[1];this.Or(
);return this;},Eq:function(AH,R,CQ,EQ,Ef){var A;if(!!EQ&&(EQ!==this))return null;
if((this.Hb>=0)&&(R!==this.Hb))return null;if(this.Fl&&(R!==this.ES))return null;
if(!!(Ef&this.Df))return null;if(!this.JL&&!!(Ef&0x3))return null;if(!this.JK&&!
!(Ef&0xC))return null;var Ai=B.hV(AH,this.H);if(!((Ai[0]>=Ai[2])||(Ai[1]>=Ai[3])
)){var CT=B.rZ(AH);var Bl=AX;if(CT[0]<Ai[0])Bl=[Ai[0]-CT[0],Bl[1]];if(CT[0]>=Ai[
2])Bl=[(Ai[2]-CT[0])-1,Bl[1]];if(CT[1]<Ai[1])Bl=[Bl[0],Ai[1]-CT[1]];if(CT[1]>=Ai[
3])Bl=[Bl[0],(Ai[3]-CT[1])-1];return B._NewObject(C.G3,0).Initialize(this,Bl);}return null;
},Ku:function(){if(!!this.timer){B.rA([this,this.Dc],this.timer,0);this.timer=null;
}},Or:function(){this.timer=B._GetAutoObject(B.tf.Gz);B.rl([this,this.Dc],this.timer
,0);},Dc:function(Aj){var A;if(!this.timer)return;var GZ=(this.timer.AK-this.Fz)
*0.001000;var G0=(this.timer.AK-this.GY)*0.001000;var Os=GZ*GZ;var Ot=G0*G0;var E3=(
this.Ex*GZ)+this.AF;var E4=(this.Ey*G0)+this.AG;var Ao=[((((this.Ex*0.500000)*Os
)+(this.AF*GZ))+this.FA)|0,((((this.Ey*0.500000)*Ot)+(this.AG*G0))+this.FB)|0];var
I0=this.MG*0.001000;if(this.DR&&(GZ>=I0)){Ao=[this.Gk,Ao[1]];this.AF=0.000000;this.
Ex=0.000000;this.FA=Ao[0];this.DR=false;}else if(this.DR){var CF=1.000000-Math.pow(
1.000000-(GZ/I0),this.KV);Ao=[(this.FA+((this.Gk-this.FA)*CF))|0,Ao[1]];}if(this.
DS&&(G0>=I0)){Ao=[Ao[0],this.Gl];this.AG=0.000000;this.Ey=0.000000;this.FB=Ao[1];
this.DS=false;}else if(this.DS){var CF=1.000000-Math.pow(1.000000-(G0/I0),this.KV
);Ao=[Ao[0],(this.FB+((this.Gl-this.FB)*CF))|0];}if(((this.AF>0.000000)&&(E3<0.000000
))||((this.AF<0.000000)&&(E3>0.000000))){E3=0.000000;Ao=[this.AL[0],Ao[1]];}if(((
this.AG>0.000000)&&(E4<0.000000))||((this.AG<0.000000)&&(E4>0.000000))){E4=0.000000;
Ao=[Ao[0],this.AL[1]];}if(!this.DR&&(Ao[0]<this.A3[0])){this.FA=Ao[0];this.Gk=this.
A3[0];this.Fz=this.timer.AK;this.DR=true;}else if(!this.DR&&(Ao[0]>this.A2[0])){
this.FA=Ao[0];this.Gk=this.A2[0];this.Fz=this.timer.AK;this.DR=true;}if(!this.DS&&(
Ao[1]<this.A3[1])){this.FB=Ao[1];this.Gl=this.A3[1];this.GY=this.timer.AK;this.DS=
true;}else if(!this.DS&&(Ao[1]>this.A2[1])){this.FB=Ao[1];this.Gl=this.A2[1];this.
GY=this.timer.AK;this.DS=true;}if(((!this.DR&&(this.AF!==0.000000))&&(E3>-20.000000
))&&(E3<20.000000)){Ao=[this.Gk,Ao[1]];this.AF=0.000000;this.Ex=0.000000;this.FA=
Ao[0];}if(((!this.DS&&(this.AG!==0.000000))&&(E4>-20.000000))&&(E4<20.000000)){Ao=[
Ao[0],this.Gl];this.AG=0.000000;this.Ey=0.000000;this.FB=Ao[1];}if(!B.sa(Ao,this.
AL)){this.KA=B.sn(Ao,this.AL);this.AL=Ao;(A=this.GW)?A[1].call(A[0],this):null;(
A=this.JD)?A[1].call(A[0],this):null;}if((((this.AF===0.000000)&&(this.AG===0.000000
))&&!this.DR)&&!this.DS){this.Ku();this.GK=false;(A=this.LG)?A[1].call(A[0],this
):null;(A=this.BC)?A[1].call(A[0],this):null;}},GI:function(E){if(E[0]<0)E=[0,E[
1]];if(E[1]<0)E=[E[0],0];this.DG=E;},GF:function(E){if(E<0.000000)E=0.000000;if(
E>1.000000)E=1.000000;if(E===this.KF)return;this.KF=E;if(E<0.000000)E=0.000000;this.
Ej=E*10000.000000;},_Init:function(aArg){C.BD._Init.call(this,aArg);this.__proto__=
C.Ay;this.F=0x11B;},_Mark:function(D){var A;C.BD._Mark.call(this,D);if((A=this.timer
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.I1)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.LG)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
GW)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.JD)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.BC)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.Bw)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SlideTouchHandler"
};C.G$={AJ:null,Cm:null,D2:null,Hg:null,Gr:0,AK:0,KT:0,L5:148,BJ:0,AZ:0,Ck:true,
Down:false,JG:false,H4:false,BY:function(Az){var A;if(!!Az&&Az.L9(this.L5)){this.
Down=Az.Down;this.BJ=Az.BJ;this.AZ=Az.AZ;this.AK=Az.AK;this.H4=false;if(Az.Down){
this.KT=this.Gr;this.JG=this.Gr>0;if(this.JG)(A=this.Hg)?A[1].call(A[0],this):null;
else(A=this.D2)?A[1].call(A[0],this):null;if(!this.H4)this.Gr=this.Gr+1;return!this.
H4;}if(!Az.Down){this.JG=this.Gr>1;this.KT=this.Gr-1;this.Gr=0;(A=this.Cm)?A[1].
call(A[0],this):null;return!this.H4;}}return false;},CP:function(aArg){var A;var
Db=(C.O.isPrototypeOf(A=this.J)?A:null);if(!Db)throw new Error(JW);this.AJ=Db.IS;
Db.IS=this;},_Init:function(aArg){this.__proto__=C.G$;this.CP(aArg);B.gb++;},_Done:
function(){this.__proto__=C.G$;B.gb--;},_ReInit:function(){},_Mark:function(D){var
A;if((A=this.AJ)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cm)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.D2)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.Hg)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"
};C.G3={Ax:null,G4:0,AL:B.pm,Initialize:function(P,aOffset){this.Ax=P;this.AL=aOffset;
this.G4=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:function(
aArg){this.__proto__=C.G3;B.gb++;},_Done:function(){this.__proto__=C.G3;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.Ax)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::CursorHit"};C.KI={Db:null,_Init:function(aArg){this.__proto__=
C.KI;B.gb++;},_Done:function(){this.__proto__=C.KI;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Db)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::ModalContext"};C.Ha={Gq:null,CW:B.pn,AA:B.pn,isEmpty:false,_Init:function(
aArg){this.__proto__=C.Ha;B.gb++;},_Done:function(){this.__proto__=C.Ha;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.Gq)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::LayoutContext"};C.H8={HT:B.pm,HS:B.pm,HR:B.pm,HQ:B.pm,_Init:
function(aArg){C.Ha._Init.call(this,aArg);this.__proto__=C.H8;},_className:"Core::LayoutQuadContext"
};C.KB={Db:null,_Init:function(aArg){this.__proto__=C.KB;B.gb++;},_Done:function(
){this.__proto__=C.KB;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Db)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(
A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::DialogContext"};
C.KY={J8:null,BE:null,_Init:function(aArg){this.__proto__=C.KY;B.gb++;},_Done:function(
){this.__proto__=C.KY;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.J8)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.BE)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:
null,_className:"Core::TaskQueue"};C.KX={_Init:function(aArg){this.__proto__=C.KX;
B.gb++;},_Done:function(){this.__proto__=C.KX;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:
0,_observers:null,_className:"Core::Task"};C.Fh={resource:null,D_:function(){this.
resource=null;},CP:function(aArg){this.resource=aArg;},_Init:function(aArg){this.
__proto__=C.Fh;this.CP(aArg);B.gb++;},_Done:function(){this.__proto__=C.Fh;this.
D_();B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Core::Resource"
};C.Timer={Ia:null,timer:null,AK:0,Period:1000,H2:0,Ck:false,D_:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},I3:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.rz(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},JB:function(E){if(E<0)E=0;if(E===this.Period)return;
this.Period=E;if(this.Ck)this.I3(this.H2,E);},Mu:function(E){if(E<0)E=0;if(E===this.
H2)return;this.H2=E;if(this.Ck)this.I3(E,this.Period);},Ak:function(E){if(E===this.
Ck)return;this.Ck=E;if(E)this.I3(this.H2,this.Period);else this.I3(0,0);this.AK=
this.G7();},G7:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk
)|0;return ticksCount;},Trigger:function(){var A;this.AK=this.G7();if(!this.Period
)this.Ak(false);(A=this.Ia)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=C.Timer;B.gb++;},_Done:function(){this.__proto__=C.Timer;this.D_();B.gb--;
},_ReInit:function(){},_Mark:function(D){var A;if((A=this.Ia)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null
,_cycle:0,_observers:null,_className:"Core::Timer"};C.JE={Jy:null,A0:null,Ck:true
,LA:function(Aj){var A;(A=this.Jy)?A[1].call(A[0],this):null;},MB:function(E){if(
B.sc(this.A0,E))return;if(!!this.A0&&this.Ck)B.rC([this,this.LA],this.A0,0);this.
A0=E;if(!!this.A0&&this.Ck)B.rn([this,this.LA],this.A0,0);},_Init:function(aArg){
this.__proto__=C.JE;B.gb++;},_Done:function(){this.__proto__=C.JE;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.Jy)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.A0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:
"Core::PropertyObserver"};C.PC={PD:0x1,OC:0x2,O2:0x4,PA:0x8,Ck:0x10,Ps:0x20,O3:0x40
,O_:0x80,O1:0x100,O6:0x200,OV:0x400,Ph:0x800,KZ:0x1000,PB:0x2000,Pc:0x4000,Pd:0x8000
,OP:0x10000,Pb:0x20000,Pp:0x40000};C.Ff={Pi:0x1,Pj:0x2,Oz:0x4,OA:0x8,OB:0x10,Oy:
0x20};C.H7={O$:0,Pz:1,OH:2,O7:3,Pl:4};C.KeyCode={NoKey:0,Ok:1,Exit:2,Menu:3,Up:4
,Down:5,Left:6,Right:7,PageUp:8,PageDown:9,Key0:10,Key1:11,Key2:12,Key3:13,Key4:
14,Key5:15,Key6:16,Key7:17,Key8:18,Key9:19,Red:20,Green:21,Blue:22,Yellow:23,White:
24,Magenta:25,F1:26,F2:27,F3:28,F4:29,F5:30,F6:31,F7:32,F8:33,F9:34,F10:35,ChannelUp:
36,ChannelDown:37,Display:38,SkipPrev:39,SkipNext:40,Home:41,End:42,Insert:43,Delete:
44,Clear:45,VolumeUp:46,VolumeDown:47,Show:48,Hide:49,Play:50,Pause:51,Record:52
,Stop:53,Rev:54,Fwd:55,SlowRev:56,SlowFwd:57,SkipBwd:58,SkipFwd:59,Repeat:60,Eject:
61,Help:62,TV:63,DVD:64,VCR:65,EPG:66,OSD:67,Text:68,PIP:69,Audio:70,Clock:71,Timer:
72,Navigation:73,Karaoke:74,Game:75,Subtitle:76,Zoom:77,Index:78,Info:79,Power:80
,Setup:81,Angle:82,Mode:83,Mute:84,User0:85,User1:86,User2:87,User3:88,User4:89,
User5:90,User6:91,User7:92,User8:93,User9:94,User10:95,User11:96,User12:97,User13:
98,User14:99,User15:100,User16:101,User17:102,User18:103,User19:104,KeyA:105,KeyB:
106,KeyC:107,KeyD:108,KeyE:109,KeyF:110,KeyG:111,KeyH:112,KeyI:113,KeyJ:114,KeyK:
115,KeyL:116,KeyM:117,KeyN:118,KeyO:119,KeyP:120,KeyQ:121,KeyR:122,KeyS:123,KeyT:
124,KeyU:125,KeyV:126,KeyW:127,KeyX:128,KeyY:129,KeyZ:130,Space:131,Plus:132,Minus:
133,Multiply:134,Divide:135,Equals:136,Period:137,Comma:138,Colon:139,Semicolon:
140,AlphaKeys:141,AlphaOrDigitKeys:142,DigitKeys:143,HexDigitKeys:144,CharacterKeys:
145,ControlKeys:146,CursorKeys:147,AnyKey:148,Enter:149,Escape:150,Backspace:151
,Tab:152,CtrlKeyA:153,CtrlKeyB:154,CtrlKeyC:155,CtrlKeyD:156,CtrlKeyE:157,CtrlKeyF:
158,CtrlKeyG:159,CtrlKeyH:160,CtrlKeyI:161,CtrlKeyJ:162,CtrlKeyK:163,CtrlKeyL:164
,CtrlKeyM:165,CtrlKeyN:166,CtrlKeyO:167,CtrlKeyP:168,CtrlKeyQ:169,CtrlKeyR:170,CtrlKeyS:
171,CtrlKeyT:172,CtrlKeyU:173,CtrlKeyV:174,CtrlKeyW:175,CtrlKeyX:176,CtrlKeyY:177
,CtrlKeyZ:178,CtrlSpace:179,CtrlKey0:180,CtrlKey1:181,CtrlKey2:182,CtrlKey3:183,
CtrlKey4:184,CtrlKey5:185,CtrlKey6:186,CtrlKey7:187,CtrlKey8:188,CtrlKey9:189,CtrlF1:
190,CtrlF2:191,CtrlF3:192,CtrlF4:193,CtrlF5:194,CtrlF6:195,CtrlF7:196,CtrlF8:197
,CtrlF9:198,CtrlF10:199,CtrlEnter:200,CtrlEscape:201,CtrlUp:202,CtrlDown:203,CtrlLeft:
204,CtrlRight:205,CtrlPageUp:206,CtrlPageDown:207,CtrlBackspace:208,CtrlInsert:209
,CtrlDelete:210,CtrlHome:211,CtrlEnd:212,CtrlTab:213,CtrlShiftKeyA:214,CtrlShiftKeyB:
215,CtrlShiftKeyC:216,CtrlShiftKeyD:217,CtrlShiftKeyE:218,CtrlShiftKeyF:219,CtrlShiftKeyG:
220,CtrlShiftKeyH:221,CtrlShiftKeyI:222,CtrlShiftKeyJ:223,CtrlShiftKeyK:224,CtrlShiftKeyL:
225,CtrlShiftKeyM:226,CtrlShiftKeyN:227,CtrlShiftKeyO:228,CtrlShiftKeyP:229,CtrlShiftKeyQ:
230,CtrlShiftKeyR:231,CtrlShiftKeyS:232,CtrlShiftKeyT:233,CtrlShiftKeyU:234,CtrlShiftKeyV:
235,CtrlShiftKeyW:236,CtrlShiftKeyX:237,CtrlShiftKeyY:238,CtrlShiftKeyZ:239,CtrlShiftSpace:
240,CtrlShiftKey0:241,CtrlShiftKey1:242,CtrlShiftKey2:243,CtrlShiftKey3:244,CtrlShiftKey4:
245,CtrlShiftKey5:246,CtrlShiftKey6:247,CtrlShiftKey7:248,CtrlShiftKey8:249,CtrlShiftKey9:
250,CtrlShiftF1:251,CtrlShiftF2:252,CtrlShiftF3:253,CtrlShiftF4:254,CtrlShiftF5:
255,CtrlShiftF6:256,CtrlShiftF7:257,CtrlShiftF8:258,CtrlShiftF9:259,CtrlShiftF10:
260,CtrlShiftEnter:261,CtrlShiftEscape:262,CtrlShiftUp:263,CtrlShiftDown:264,CtrlShiftLeft:
265,CtrlShiftRight:266,CtrlShiftPageUp:267,CtrlShiftPageDown:268,CtrlShiftBackspace:
269,CtrlShiftInsert:270,CtrlShiftDelete:271,CtrlShiftHome:272,CtrlShiftEnd:273,CtrlShiftTab:
274,AltF1:275,AltF2:276,AltF3:277,AltF4:278,AltF5:279,AltF6:280,AltF7:281,AltF8:
282,AltF9:283,AltF10:284,AltEnter:285,AltEscape:286,AltUp:287,AltDown:288,AltLeft:
289,AltRight:290,AltPageUp:291,AltPageDown:292,AltBackspace:293,AltInsert:294,AltDelete:
295,AltHome:296,AltEnd:297,AltTab:298,AltShiftF1:299,AltShiftF2:300,AltShiftF3:301
,AltShiftF4:302,AltShiftF5:303,AltShiftF6:304,AltShiftF7:305,AltShiftF8:306,AltShiftF9:
307,AltShiftF10:308,AltShiftEnter:309,AltShiftEscape:310,AltShiftUp:311,AltShiftDown:
312,AltShiftLeft:313,AltShiftRight:314,AltShiftPageUp:315,AltShiftPageDown:316,AltShiftBackspace:
317,AltShiftInsert:318,AltShiftDelete:319,AltShiftHome:320,AltShiftEnd:321,AltShiftTab:
322,ShiftF1:323,ShiftF2:324,ShiftF3:325,ShiftF4:326,ShiftF5:327,ShiftF6:328,ShiftF7:
329,ShiftF8:330,ShiftF9:331,ShiftF10:332,ShiftEnter:333,ShiftEscape:334,ShiftUp:
335,ShiftDown:336,ShiftLeft:337,ShiftRight:338,ShiftPageUp:339,ShiftPageDown:340
,ShiftBackspace:341,ShiftInsert:342,ShiftDelete:343,ShiftHome:344,ShiftEnd:345,ShiftTab:
346};C.Pk={PH:0x1,PE:0x2,PF:0x4,PG:0x8,O9:0x10};
C._Init=function(){C.Ho.__proto__=C.Ax;C.BD.__proto__=C.Ax;C.O.__proto__=C.BD;C.Root.
__proto__=C.O;C.KeyEvent.__proto__=C.Event;C.H5.__proto__=C.Event;C.G2.__proto__=
C.Event;C.G5.__proto__=C.Event;C.Hk.__proto__=C.BD;C.Cd.__proto__=C.O;C.V.__proto__=
C.Ho;C.Ay.__proto__=C.BD;C.H8.__proto__=C.Ha;};C.Dh=function(D){};return C;})();

/* Embedded Wizard */