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
var Ar=[0,0];var AT=[0,0,0,0];var Cs="The view does not belong to this group";var
Eu="No view to remove";var Ev="View is not in this group";var Ew="No view to add";
var Ex="View already in a group";var Hm="Recursive invalidate during active update cycle.";
var Hn="The KeyPressHandler is embedded within an object not being derived "+"from Core::Group.";
C.B9={AU:null,At:null,K:null,Aw:null,F:0x103,Fm:0,DY:0x14,Fw:function(U,Gl){},Gc:
function(E){var A;var D6=E^this.DY;if(!D6)return;this.DY=E;if(!!this.Aw&&!((this.
F&0x400)===0x400)){this.K.F=this.K.F|0x5000;B.lq([A=this.K,A.B3],this);this.K.Ao([
0,0,(A=this.K.G)[2]-A[0],A[3]-A[1]]);}if(!!this.Aw&&((this.F&0x400)===0x400)){this.
Aw.EJ.F=this.Aw.EJ.F|0x1000;this.K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3],this);
}},G1:function(){var Y=this.K;while(!!Y){var II=(C.Root.isPrototypeOf(Y)?Y:null);
if(!!II)return II;Y=Y.K;}return null;},CP:function(AD,aClip,aOffset,Ay,aBlend){}
,Bg:function(Ad){return null;},Dx:function(Ag,M,B$,E0,EB){return null;},FQ:function(
U,Ct){return Ar;},G7:function(aOffset,Gk){},GetExtent:function(){return AT;},Bq:
function(DJ,EA){var A;if(((this.F&0x200)===0x200))DJ=DJ&~0x400;var HK=(this.F&~EA
)|DJ;var D7=HK^this.F;this.F=HK;if(!!this.K&&!!(D7&0x14)){var Iw=((this.F&0x14)===
0x14);if(Iw&&!this.K.Cz)this.K.EW(this);if(!Iw&&(this.K.Cz===this))this.K.EW(this.
K.H2(this,0x14));}if(!!this.K&&!!(D7&0x403))this.K.Ao(this.GetExtent());if(((!!this.
Aw&&!!this.K)&&((HK&0x400)===0x400))&&((D7&0x1)===0x1)){this.F=this.F|0x800;this.
K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3],this);}if(!!this.K&&((D7&0x400)===0x400
)){this.Aw=null;this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3
],this);}},_Init:function(aArg){this.__proto__=C.B9;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.
AU)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.At)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.K)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Aw)&&(A._cycle!=D
))A._Mark(A._cycle=D);if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null,
_cycle:0,_observers:null,_className:"Core::View"};C.Fl={Bk:B.qx,By:B.qx,Bj:B.qx,
Bx:B.qx,Fw:function(U,Gl){var X=B._NewObject(C.F$,0);this.Aw=null;X.BT=this.GetExtent(
);X.Ab=U;X.EJ=Gl;X.FD=this.Bx;X.FE=this.Bj;X.FF=this.By;X.FG=this.Bk;this.Aw=X;}
,FQ:function(U,Ct){var A;var AZ=this.DY;var X=(C.F$.isPrototypeOf(A=this.Aw)?A:null
);var O=X.BT[0];var P=X.BT[1];var Q=X.BT[2];var R=X.BT[3];var BS=[U[2]-U[0],U[3]-
U[1]];var AG=Q-O;var AF=R-P;if(!Ct){var CN=[(A=X.Ab)[2]-A[0],A[3]-A[1]];O=O-X.Ab[
0];P=P-X.Ab[1];if(CN[0]!==BS[0]){var Cj=((AZ&0x4)===0x4);var Ck=((AZ&0x8)===0x8);
var Da=((AZ&0x1)===0x1);if(!Cj&&(Da||!Ck))O=((O*BS[0])/CN[0])|0;if(!Ck&&(Da||!Cj
)){Q=Q-X.Ab[0];Q=((Q*BS[0])/CN[0])|0;Q=Q-BS[0];}else Q=Q-X.Ab[2];O=O+U[0];Q=Q+U[
2];if(!Da){if(Cj&&!Ck)Q=O+AG;else if(!Cj&&Ck)O=Q-AG;else{O=O+((((Q-O)-AG)/2)|0);
Q=O+AG;}}}else{Q=Q-X.Ab[2];O=O+U[0];Q=Q+U[2];}if(CN[1]!==BS[1]){var Cl=((AZ&0x10
)===0x10);var Ci=((AZ&0x20)===0x20);var Db=((AZ&0x2)===0x2);if(!Cl&&(Db||!Ci))P=((
P*BS[1])/CN[1])|0;if(!Ci&&(Db||!Cl)){R=R-X.Ab[1];R=((R*BS[1])/CN[1])|0;R=R-BS[1];
}else R=R-X.Ab[3];P=P+U[1];R=R+U[3];if(!Db){if(Cl&&!Ci)R=P+AF;else if(!Cl&&Ci)P=
R-AF;else{P=P+((((R-P)-AF)/2)|0);R=P+AF;}}}else{R=R-X.Ab[3];P=P+U[1];R=R+U[3];}}
else{switch(Ct){case 3:{O=U[0];Q=O+AG;}break;case 4:{Q=U[2];O=Q-AG;}break;case 1:{
P=U[1];R=P+AF;}break;case 2:{R=U[3];P=R-AF;}break;default:;}if((Ct===3)||(Ct===4
)){var Cl=((AZ&0x10)===0x10);var Ci=((AZ&0x20)===0x20);var Db=((AZ&0x2)===0x2);if(
Db){P=U[1];R=U[3];}else if(Cl&&!Ci){P=U[1];R=P+AF;}else if(Ci&&!Cl){R=U[3];P=R-AF;
}else{P=U[1]+((((U[3]-U[1])-AF)/2)|0);R=P+AF;}}if((Ct===1)||(Ct===2)){var Cj=((AZ&
0x4)===0x4);var Ck=((AZ&0x8)===0x8);var Da=((AZ&0x1)===0x1);if(Da){O=U[0];Q=U[2];
}else if(Cj&&!Ck){O=U[0];Q=O+AG;}else if(Ck&&!Cj){Q=U[2];O=Q-AG;}else{O=U[0]+((((
U[2]-U[0])-AG)/2)|0);Q=O+AG;}}}X.isEmpty=(O>=Q)||(P>=R);AG=(Q-O)-1;AF=(R-P)-1;var
DN=X.BT[0];var DO=X.BT[1];var Dt=(X.BT[2]-DN)-1;var Ds=(X.BT[3]-DO)-1;if(!Dt)Dt=
1;if(!Ds)Ds=1;if(((this.F&0x100)===0x100)){this.Bx=[O+((((X.FD[0]-DN)*AG)/Dt)|0)
,P+((((X.FD[1]-DO)*AF)/Ds)|0)];this.Bj=[O+((((X.FE[0]-DN)*AG)/Dt)|0),P+((((X.FE[
1]-DO)*AF)/Ds)|0)];this.By=[O+((((X.FF[0]-DN)*AG)/Dt)|0),P+((((X.FF[1]-DO)*AF)/Ds
)|0)];this.Bk=[O+((((X.FG[0]-DN)*AG)/Dt)|0),P+((((X.FG[1]-DO)*AF)/Ds)|0)];}else{
this.Hd([O+((((X.FD[0]-DN)*AG)/Dt)|0),P+((((X.FD[1]-DO)*AF)/Ds)|0)]);this.He([O+((((
X.FE[0]-DN)*AG)/Dt)|0),P+((((X.FE[1]-DO)*AF)/Ds)|0)]);this.Hf([O+((((X.FF[0]-DN)
*AG)/Dt)|0),P+((((X.FF[1]-DO)*AF)/Ds)|0)]);this.Hg([O+((((X.FG[0]-DN)*AG)/Dt)|0)
,P+((((X.FG[1]-DO)*AF)/Ds)|0)]);this.Aw=X;}return[AG+1,AF+1];},G7:function(aOffset
,Gk){if(Gk){this.Bx=B.tx(this.Bx,aOffset);this.Bj=B.tx(this.Bj,aOffset);this.By=
B.tx(this.By,aOffset);this.Bk=B.tx(this.Bk,aOffset);}else{this.Hd(B.tx(this.Bx,aOffset
));this.He(B.tx(this.Bj,aOffset));this.Hf(B.tx(this.By,aOffset));this.Hg(B.tx(this.
Bk,aOffset));}},GetExtent:function(){if(!!this.Aw&&this.Aw.isEmpty)return AT;var
O=this.Bx[0];var P=this.Bx[1];var Q=this.By[0];var R=this.By[1];if((((this.Bk[0]
!==O)||(this.Bj[1]!==P))||(this.Bj[0]!==Q))||(this.Bk[1]!==R)){if(this.Bj[0]<O)O=
this.Bj[0];if(this.By[0]<O)O=this.By[0];if(this.Bk[0]<O)O=this.Bk[0];if(this.Bj[
1]<P)P=this.Bj[1];if(this.By[1]<P)P=this.By[1];if(this.Bk[1]<P)P=this.Bk[1];if(this.
Bx[0]>Q)Q=this.Bx[0];if(this.Bj[0]>Q)Q=this.Bj[0];if(this.Bk[0]>Q)Q=this.Bk[0];if(
this.Bx[1]>R)R=this.Bx[1];if(this.Bj[1]>R)R=this.Bj[1];if(this.Bk[1]>R)R=this.Bk[
1];}else{var tmp;if(Q<O){tmp=O;O=Q;Q=tmp;}if(R<P){tmp=P;P=R;R=tmp;}}return[O,P,Q+
1,R+1];},Hg:function(E){var A;if(B.tl(E,this.Bk))return;if(!!this.K&&((this.F&0x1
)===0x1))this.K.Ao(this.GetExtent());this.Aw=null;this.Bk=E;if(!!this.K&&((this.
F&0x1)===0x1))this.K.Ao(this.GetExtent());if((!!this.K&&((this.F&0x400)===0x400)
)&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.
lq([A=this.K,A.B3],this);}},Hf:function(E){var A;if(B.tl(E,this.By))return;if(!!
this.K&&((this.F&0x1)===0x1))this.K.Ao(this.GetExtent());this.Aw=null;this.By=E;
if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.GetExtent());if((!!this.K&&((this.
F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.K.F=
this.K.F|0x4000;B.lq([A=this.K,A.B3],this);}},He:function(E){var A;if(B.tl(E,this.
Bj))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.GetExtent());this.Aw=
null;this.Bj=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.GetExtent());if((
!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3],this);}},Hd:function(E){var
A;if(B.tl(E,this.Bx))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.GetExtent(
));this.Aw=null;this.Bx=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.GetExtent(
));if((!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3],this);}},G4:function(
C4){var Ax=B.tA(4,B.qx,null);var I=0;var AA=3;var HE=false;var HF=false;Ax.Set(0
,this.Bx);Ax.Set(1,this.Bj);Ax.Set(2,this.By);Ax.Set(3,this.Bk);while(I<4){var IO=
Ax.Get(I)[0];var GR=Ax.Get(I)[1];var Ki=Ax.Get(AA)[0];var HW=Ax.Get(AA)[1];if(((
GR>C4[1])!==(HW>C4[1]))||((GR<C4[1])!==(HW<C4[1]))){var Bm=((((Ki-IO)*(C4[1]-GR)
)/(HW-GR))|0)+IO;if(C4[0]>Bm)HE=!HE;if(C4[0]<Bm)HF=!HF;}AA=I;I=I+1;}return HE||HF;
},I2:function(){return((((this.Bx[0]===this.Bk[0])&&(this.Bj[0]===this.By[0]))&&(
this.Bx[1]===this.Bj[1]))&&(this.By[1]===this.Bk[1]))||((((this.Bx[0]===this.Bj[
0])&&(this.By[0]===this.Bk[0]))&&(this.Bx[1]===this.Bk[1]))&&(this.Bj[1]===this.
By[1]));},_Init:function(aArg){C.B9._Init.call(this,aArg);this.__proto__=C.Fl;},
_className:"Core::QuadView"};C.A2={G:B.qy,Fw:function(U,Gl){var X=B._NewObject(C.
F_,0);X.BT=this.G;X.Ab=U;X.EJ=Gl;this.Aw=X;},FQ:function(U,Ct){var A;var AZ=this.
DY;var X=this.Aw;var O=X.BT[0];var P=X.BT[1];var Q=X.BT[2];var R=X.BT[3];var BS=[
U[2]-U[0],U[3]-U[1]];var AG=Q-O;var AF=R-P;if(!Ct){var CN=[(A=X.Ab)[2]-A[0],A[3]-
A[1]];O=O-X.Ab[0];P=P-X.Ab[1];if(CN[0]!==BS[0]){var Cj=((AZ&0x4)===0x4);var Ck=((
AZ&0x8)===0x8);var Da=((AZ&0x1)===0x1);if(!Cj&&(Da||!Ck))O=((O*BS[0])/CN[0])|0;if(
!Ck&&(Da||!Cj)){Q=Q-X.Ab[0];Q=((Q*BS[0])/CN[0])|0;Q=Q-BS[0];}else Q=Q-X.Ab[2];O=
O+U[0];Q=Q+U[2];if(!Da){if(Cj&&!Ck)Q=O+AG;else if(!Cj&&Ck)O=Q-AG;else{O=O+((((Q-
O)-AG)/2)|0);Q=O+AG;}}}else{Q=Q-X.Ab[2];O=O+U[0];Q=Q+U[2];}if(CN[1]!==BS[1]){var
Cl=((AZ&0x10)===0x10);var Ci=((AZ&0x20)===0x20);var Db=((AZ&0x2)===0x2);if(!Cl&&(
Db||!Ci))P=((P*BS[1])/CN[1])|0;if(!Ci&&(Db||!Cl)){R=R-X.Ab[1];R=((R*BS[1])/CN[1]
)|0;R=R-BS[1];}else R=R-X.Ab[3];P=P+U[1];R=R+U[3];if(!Db){if(Cl&&!Ci)R=P+AF;else
if(!Cl&&Ci)P=R-AF;else{P=P+((((R-P)-AF)/2)|0);R=P+AF;}}}else{R=R-X.Ab[3];P=P+U[1
];R=R+U[3];}}else{switch(Ct){case 3:{O=U[0];Q=O+AG;}break;case 4:{Q=U[2];O=Q-AG;
}break;case 1:{P=U[1];R=P+AF;}break;case 2:{R=U[3];P=R-AF;}break;default:;}if((Ct===
3)||(Ct===4)){var Cl=((AZ&0x10)===0x10);var Ci=((AZ&0x20)===0x20);var Db=((AZ&0x2
)===0x2);if(Db){P=U[1];R=U[3];}else if(Cl&&!Ci){P=U[1];R=P+AF;}else if(Ci&&!Cl){
R=U[3];P=R-AF;}else{P=U[1]+((((U[3]-U[1])-AF)/2)|0);R=P+AF;}}if((Ct===1)||(Ct===
2)){var Cj=((AZ&0x4)===0x4);var Ck=((AZ&0x8)===0x8);var Da=((AZ&0x1)===0x1);if(Da
){O=U[0];Q=U[2];}else if(Cj&&!Ck){O=U[0];Q=O+AG;}else if(Ck&&!Cj){Q=U[2];O=Q-AG;
}else{O=U[0]+((((U[2]-U[0])-AG)/2)|0);Q=O+AG;}}}X.isEmpty=(O>=Q)||(P>=R);if(((this.
F&0x100)===0x100)){this.G=[O,P,Q,R];}else{this.T([O,P,Q,R]);this.Aw=X;}return[Q-
O,R-P];},G7:function(aOffset,Gk){if(Gk)this.G=B.tz(this.G,aOffset);else this.T(B.
tz(this.G,aOffset));},GetExtent:function(){return this.G;},T:function(E){var A;if(
B.tm(E,this.G))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);this.Aw=
null;this.G=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);if((!!this.K&&((
this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3],this);}},_Init:function(aArg){C.B9._Init.
call(this,aArg);this.__proto__=C.A2;},_className:"Core::RectView"};C.Ai={Cw:null
,BY:null,GB:null,Cu:null,D9:null,EG:null,Cz:null,Er:255,CP:function(AD,aClip,aOffset
,Ay,aBlend){var A;Ay=((Ay+1)*this.Er)>>8;aBlend=aBlend&&((this.F&0x2)===0x2);if(
!this.Cu)this.J4(AD,aClip,B.tx(aOffset,this.G.slice(0,2)),Ay,aBlend);else{var BQ=
255|(255<<8)|(255<<16)|((Ay&0xFF)<<24);this.Cu.Update();AD.HX(aClip,this.Cu,0,B.
tz(this.G,aOffset),Ar,BQ,BQ,BQ,BQ,aBlend);}},Dx:function(Ag,M,B$,E0,EB){var A;var
J=this.BY;var EH=null;var Z=AT;var AQ=null;var HJ=!!this.EG&&(!!this.EG.Hx||!!this.
EG.Cw);if(((A=B.il(Ag,this.G))[0]>=A[2])||(A[1]>=A[3]))return null;Ag=B.ty(Ag,this.
G.slice(0,2));while(!!J){if(((J.F&0x400)===0x400)&&!AQ){AQ=J.At;while(!!AQ&&!((AQ.
F&0x200)===0x200))AQ=AQ.At;if(!!AQ)Z=B.il(Ag,AQ.GetExtent());else Z=AT;}if(AQ===
J){AQ=null;Z=AT;}if((((((J.F&0x8)===0x8)&&((J.F&0x10)===0x10))&&!((J.F&0x40000)===
0x40000))&&!((J.F&0x20000)===0x20000))&&(!((J.F&0x10000)===0x10000)||((this.D9.Cb===
J)&&!HJ))){var BT=J.GetExtent();var Gu=E0;var EE=null;if(Gu===J)Gu=null;if(((J.F&
0x400)===0x400)){if(!(((A=B.il(BT,Z))[0]>=A[2])||(A[1]>=A[3])))EE=J.Dx(Z,M,B$,Gu
,EB);}else{if(!(((A=B.il(BT,Ag))[0]>=A[2])||(A[1]>=A[3]))||(E0===J))EE=J.Dx(Ag,M
,B$,Gu,EB);}J=J.At;if(!!EE){if(!EH||((EE.E$<EH.E$)&&(EE.E$>=0)))EH=EE;if(!EE.E$)
J=null;}}else J=J.At;}return EH;},Bq:function(DJ,EA){var A;var J_=this.F;C.A2.Bq.
call(this,DJ,EA);var D7=this.F^J_;if(!!this.Cz&&((D7&0x40)===0x40)){if(((this.F&
0x40)===0x40))this.Cz.Bq(0x40,0x0);else this.Cz.Bq(0x0,0x40);}if(!!this.D9&&((D7&
0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.D9.Cb.F&0x14)===0x14))this.D9.Cb.
Bq(0x40,0x0);else this.D9.Cb.Bq(0x0,0x40);}if(!!D7){this.F=this.F|0x8000;B.lq([this
,this.B3],this);}},T:function(E){var A;if(B.tm(E,this.G))return;var Ec=[(A=this.
G)[2]-A[0],A[3]-A[1]];var GF=[E[2]-E[0],E[3]-E[1]];var EM=!B.tl(Ec,GF);if(EM&&!!
this.Cu){this.Cu.G_(GF);B.qw(this,0);B.qw(this.Cu,0);}C.A2.T.call(this,E);if((EM&&(
Ec[0]>0))&&(Ec[1]>0)){var Ab=[].concat(Ar,Ec);var J=this.Cw;var Fv=0x14;while(!!
J){if((!J.Aw&&(J.DY!==Fv))&&!((J.F&0x400)===0x400))J.Fw(Ab,null);J=J.AU;}}if(EM){
this.F=this.F|0x5000;B.lq([this,this.B3],this);}},HO:function(Ad){var Iy=(C.KeyEvent.
isPrototypeOf(Ad)?Ad:null);var CX=this.GB;if(!Iy)return null;while(!!CX&&(!CX.Di||
!CX.Bg(Iy)))CX=CX.AU;return CX;},J4:function(AD,aClip,aOffset,Ay,aBlend){var A;var
J=this.Cw;var Ir=AT;var IF=true;while(!!J){if(((J.F&0x200)===0x200)){var IE=(C.Fj.
isPrototypeOf(J)?J:null);Ir=B.il(aClip,B.tz(IE.G,aOffset));IF=((IE.F&0x1)===0x1);
}if(((J.F&0x1)===0x1)){if(((J.F&0x400)===0x400)){if(IF){var Z=B.il(B.tz(J.GetExtent(
),aOffset),Ir);if(!((Z[0]>=Z[2])||(Z[1]>=Z[3])))J.CP(AD,Z,aOffset,Ay,aBlend);}}else{
var Z=B.il(B.tz(J.GetExtent(),aOffset),aClip);if(!((Z[0]>=Z[2])||(Z[1]>=Z[3])))J.
CP(AD,Z,aOffset,Ay,aBlend);}}J=J.AU;}},Ka:function(){var A;var HC=((this.F&0x1000
)===0x1000);var DP=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];var CV=false;var C8=AT;var
A8=AT;var CW=Ar;var E6=0;var E7=0;var E5=0;var BU=0;var J=this.BY;var AQ=null;var
Fv=0x14;var D$=null;while(!!J){if(((J.F&0x800)===0x800)){CV=true;J.F=J.F&~0x800;
}if(CV&&((J.F&0x200)===0x200)){CV=false;if(!!(C.Fj.isPrototypeOf(J)?J:null).FZ)J.
F=J.F|0x1000;}J=J.At;}CV=false;J=this.Cw;if(HC){this.F=this.F&~0x1000;HC=!((DP[0
]>=DP[2])||(DP[1]>=DP[3]));}this.F=this.F|0x2000;while(!!J){if(!D$&&(E5!==BU)){var
Dd=J;var GJ=0;var FK=C8[2]-C8[0];var Fs=C8[3]-C8[1];var Gs=0;var EP=Ar;do{if(((Dd.
F&0x200)===0x200))Dd=null;else if(((Dd.F&0x401)===0x401)){EP=[(A=Dd.GetExtent())[
2]-A[0],A[3]-A[1]];if((BU===3)||(BU===4))FK=FK-EP[0];if((BU===1)||(BU===2))Fs=Fs-
EP[1];if(!D$||((FK>=0)&&(Fs>=0))){D$=Dd;Dd=Dd.AU;if((BU===3)||(BU===4)){FK=FK-E6;
if(EP[1]>GJ)GJ=EP[1];}if((BU===1)||(BU===2)){Fs=Fs-E7;if(EP[0]>Gs)Gs=EP[0];}}else
Dd=null;}else Dd=Dd.AU;}while(!!Dd);if(!D$)D$=AQ;A8=C8;switch(E5){case 9:case 11:
A8=[].concat(A8.slice(0,3),A8[1]+GJ);break;case 10:case 12:A8=B.t3(A8,A8[3]-GJ);
break;case 5:case 7:A8=B.t1(A8,A8[0]+Gs);break;case 6:case 8:A8=[].concat(A8[2]-
Gs,A8.slice(1,4));break;default:;}}if(((J.F&0x400)===0x400)){if(!!J.Aw&&(J.Aw.EJ
!==AQ))J.Aw=null;if((!J.Aw&&CV)&&((J.DY!==Fv)||!!BU))J.Fw(A8,AQ);}if(!!J.Aw){if(
HC&&!((J.F&0x400)===0x400))J.FQ(DP,0);if(CV&&((J.F&0x400)===0x400)){var AW=J.FQ(
B.tz(A8,CW),BU);if(((J.F&0x1)===0x1)){var Co=Ar;switch(BU){case 3:Co=[AW[0]+E6,Co[
1]];break;case 4:Co=[-AW[0]-E6,Co[1]];break;case 1:Co=[Co[0],AW[1]+E7];break;case
2:Co=[Co[0],-AW[1]-E7];break;default:;}CW=B.tx(CW,Co);}}}if(((J.F&0x200)===0x200
)){if(CV)B.lq(AQ.D1,AQ);CV=((J.F&0x1000)===0x1000);AQ=(C.Fj.isPrototypeOf(J)?J:null
);if(CV){J.F=J.F&~0x1000;C8=B.tz(AQ.G,AQ.Bz);A8=C8;CW=Ar;E5=AQ.FZ;BU=E5;E6=AQ.Space+
AQ.Jm;E7=AQ.Space+AQ.Jn;CV=!((C8[0]>=C8[2])||(C8[1]>=C8[3]));D$=null;switch(E5){
case 9:case 10:BU=3;break;case 11:case 12:BU=4;break;case 5:case 6:BU=1;break;case
7:case 8:BU=2;break;default:;}}if(CV){this.Ao(AQ.G);}}if(J===D$){switch(E5){case
9:case 11:CW=[0,(CW[1]+(A8[3]-A8[1]))+E7];break;case 10:case 12:CW=[0,(CW[1]-(A8[
3]-A8[1]))-E7];break;case 5:case 7:CW=[(CW[0]+(A8[2]-A8[0]))+E6,0];break;case 6:
case 8:CW=[(CW[0]-(A8[2]-A8[0]))-E6,0];break;default:;}D$=null;}J=J.AU;}if(CV)B.
lq(AQ.D1,AQ);this.F=this.F&~0x2000;this.Ig([DP[2]-DP[0],DP[3]-DP[1]]);},B3:function(
B0){var A;var Kh=((this.F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000)){this.
F=this.F&~0x4000;this.Ka();}if(((this.F&0x8000)===0x8000)||Kh){this.F=this.F&~0x8000;
this.Gi(this.F);}},EW:function(E){var A;if(!!E&&(E.K!==this))throw new Error(Cs);
if(!!E&&!((E.F&0x14)===0x14))E=null;if(!!E&&((E.F&0x10000)===0x10000))E=null;if(
E===this.Cz)return;if(!!this.Cz)this.Cz.Bq(0x0,0x60);this.Cz=E;if(!!E){if(((this.
F&0x40)===0x40))E.Bq(0x60,0x0);else E.Bq(0x20,0x0);}},G6:function(C4){var tmp=this;
while(!!tmp){C4=B.tw(C4,tmp.G.slice(0,2));tmp=tmp.K;}return C4;},DispatchEvent:function(
Ad){var A;var J=this.Cz;var Y=(C.Ai.isPrototypeOf(J)?J:null);var Ak=null;var HJ=
!!this.EG&&(!!this.EG.Hx||!!this.EG.Cw);if(!!J&&((((J.F&0x10000)===0x10000)||((J.
F&0x40000)===0x40000))||((J.F&0x20000)===0x20000))){J=null;Y=null;}if(!!this.D9&&
!HJ)Ak=this.D9.Cb.DispatchEvent(Ad);if(!Ak&&!!Y)Ak=Y.DispatchEvent(Ad);else if(!
Ak&&!!J)Ak=J.Bg(Ad);if(!Ak)Ak=this.Bg(Ad);if(!Ak)Ak=this.HO(Ad);return Ak;},BroadcastEventAtPosition:
function(Ad,Il,aFilter){var A;var J=this.BY;var Ak=null;while(!!J&&!Ak){if((!aFilter||((
A=aFilter)&&((J.F&A)===A)))&&B.qu(J.GetExtent(),Il)){var Y=(C.Ai.isPrototypeOf(J
)?J:null);if(!!Y)Ak=Y.BroadcastEventAtPosition(Ad,B.tw(Il,Y.G.slice(0,2)),aFilter
);else Ak=J.Bg(Ad);}J=J.At;}if(!Ak)Ak=this.Bg(Ad);return Ak;},BroadcastEvent:function(
Ad,aFilter){var A;var J=this.BY;var Ak=null;while(!!J&&!Ak){if(!aFilter||((A=aFilter
)&&((J.F&A)===A))){var Y=(C.Ai.isPrototypeOf(J)?J:null);if(!!Y)Ak=Y.BroadcastEvent(
Ad,aFilter);else Ak=J.Bg(Ad);}J=J.At;}if(!Ak)Ak=this.Bg(Ad);if(!Ak)Ak=this.HO(Ad
);return Ak;},Ig:function(aSize){},Gi:function(Hr){},G2:function(){this.F=this.F|
0x8000;B.lq([this,this.B3],this);},Ao:function(Ag){var A;var Y=this;while(!!Y&&!((
Ag[0]>=Ag[2])||(Ag[1]>=Ag[3]))){var EC=Y.Cu;if(!Y.K&&(Y!==this)){Y.Ao(Ag);return;
}if(!!EC){var HB=false;var J9=EC.CR;if(HB)EC.CR=[0,0,(A=Y.G)[2]-A[0],A[3]-A[1]];
else EC.CR=B.qR(EC.CR,Ag);if(!B.tm(J9,EC.CR)){B.qw(Y,0);B.qw(EC,0);}}if(!((Y.F&0x1
)===0x1))return;Ag=B.il(B.tz(Ag,Y.G.slice(0,2)),Y.G);Y=Y.K;}},Ce:function(aArg){
this.G2();},H2:function(W,aFilter){var A;if(!W||(W.K!==this))return null;var Eb=
W.AU;var Ed=W.At;var FB=0x10000;if(((aFilter&0x10000)===0x10000))FB=0x0;while(!!
Eb||!!Ed){if((!!Eb&&(!aFilter||((A=aFilter)&&((Eb.F&A)===A))))&&(!FB||!((A=FB)&&((
Eb.F&A)===A))))return Eb;if((!!Ed&&(!aFilter||((A=aFilter)&&((Ed.F&A)===A))))&&(
!FB||!((A=FB)&&((Ed.F&A)===A))))return Ed;if(!!Eb)Eb=Eb.AU;if(!!Ed)Ed=Ed.At;}return null;
},Ji:function(W){var A;if(!W)throw new Error(Eu);if(W.K!==this)throw new Error(Ev
);if((((W.F&0x401)===0x401)&&!!W.At)&&!!W.Aw){W.At.F=W.At.F|0x800;this.F=this.F|
0x4000;B.lq([this,this.B3],this);}if(((W.F&0x200)===0x200)){if(!!W.At)W.At.F=W.At.
F|0x800;this.F=this.F|0x4000;B.lq([this,this.B3],this);}W.Aw=null;if(this.Cz===W
)this.EW(this.H2(W,0x14));if(!!W.At)W.At.AU=W.AU;if(!!W.AU)W.AU.At=W.At;if(this.
Cw===W)this.Cw=W.AU;if(this.BY===W)this.BY=W.At;W.K=null;W.AU=null;W.At=null;if(((
W.F&0x1)===0x1))this.Ao(W.GetExtent());},An:function(W,E1){var A;if(!W)throw new
Error(Ew);if(!!W.K)throw new Error(Ex);var BP=null;var GN=W.Fm;if(((E1<0)&&!!this.
BY)&&(this.BY.Fm>=GN)){BP=this.BY;E1=E1+1;}while((((E1<0)&&!!BP)&&!!BP.At)&&(BP.
At.Fm>=GN)){BP=BP.At;E1=E1+1;}if((!BP&&!!this.BY)&&(this.BY.Fm>GN))BP=this.BY;while((
!!BP&&!!BP.At)&&(BP.At.Fm>GN))BP=BP.At;if(!BP){W.K=this;W.At=this.BY;if(!!this.BY
)this.BY.AU=W;if(!this.Cw)this.Cw=W;this.BY=W;}else{W.K=this;W.At=BP.At;W.AU=BP;
BP.At=W;if(!!W.At)W.At.AU=W;else this.Cw=W;}if(((W.F&0x1)===0x1))this.Ao(W.GetExtent(
));if(((!this.Cz&&((W.F&0x4)===0x4))&&((W.F&0x10)===0x10))&&!((W.F&0x10000)===0x10000
))this.EW(W);if(((W.F&0x401)===0x401)){W.F=W.F|0x800;this.F=this.F|0x4000;B.lq([
this,this.B3],this);}if(((W.F&0x200)===0x200)){W.F=W.F|0x800;this.F=this.F|0x4000;
B.lq([this,this.B3],this);}},_Init:function(aArg){C.A2._Init.call(this,aArg);this.
__proto__=C.Ai;this.F=0x1F;this.Ce(aArg);},_Mark:function(D){var A;C.A2._Mark.call(
this,D);if((A=this.Cw)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.BY)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.GB)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Cu)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.D9)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.EG)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cz)&&(A.
_cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Group"};C.Root={BX:null,DR:null
,C5:null,Av:B.tA(10,null,null),E2:null,CC:null,DL:null,FN:0,Ht:0,Az:0,A7:B.tA(10
,0,null),Gt:B.tA(10,B.qy,null),CH:B.tA(10,0,null),Dr:B.tA(10,B.qx,null),E4:B.tA(
10,0,null),DK:B.tA(10,B.qx,null),CG:B.tA(10,B.qx,null),Ca:B.tA(10,B.qx,null),C6:
B.tA(10,B.qx,null),Ah:0,Gx:0,Gw:0,GD:B.tA(3,B.qy,null),IA:0,Bu:B.tA(4,0,null),A1:
B.tA(4,B.qy,null),AV:0,DW:8,IX:250,D_:0,DQ:0,HD:true,GC:false,G1:function(){return this;
},CP:function(AD,aClip,aOffset,Ay,aBlend){var fullScreenUpdate=false;fullScreenUpdate=
B.jI;if(!fullScreenUpdate)AD.G0(aClip,B.tz(B.tz(aClip,aOffset),this.G.slice(0,2)
),0x00000000,0x00000000,0x00000000,0x00000000,false);C.Ai.CP.call(this,AD,aClip,
aOffset,Ay,aBlend);},Bq:function(DJ,EA){var A;C.Ai.Bq.call(this,DJ,EA);if(!this.
K&&(((DJ&0x1)===0x1)||((EA&0x1)===0x1)))this.Ao([0,0,(A=this.G)[2]-A[0],A[3]-A[1
]]);if(!this.K&&(((DJ&0x2)===0x2)||((EA&0x2)===0x2)))this.Ao([0,0,(A=this.G)[2]-
A[0],A[3]-A[1]]);},EW:function(E){if((E!==this.CC)||!E)C.Ai.EW.call(this,E);},DispatchEvent:
function(Ad){if((this.Ht>0)&&!!(C.KeyEvent.isPrototypeOf(Ad)?Ad:null))return null;
if(!!Ad){Ad.Fb=!!this.Az;if(!!this.Az)Ad.AI=this.Az;}var Ak=null;if(!!this.CC){Ak=
this.CC.DispatchEvent(Ad);if(!!Ak){this.Az=0;return Ak;}}if(!!this.DR){Ak=this.DR.
Cb.DispatchEvent(Ad);if(!Ak)Ak=this.Bg(Ad);if(!Ak)Ak=this.HO(Ad);this.Az=0;return Ak;
}Ak=C.Ai.DispatchEvent.call(this,Ad);this.Az=0;return Ak;},BroadcastEvent:function(
Ad,aFilter){if(!!Ad){Ad.Fb=!!this.Az;if(!!this.Az)Ad.AI=this.Az;}var Ak=C.Ai.BroadcastEvent.
call(this,Ad,aFilter);this.Az=0;return Ak;},Ao:function(Ag){var A;if(this.FN>0)throw new
Error(Hm);if(!!this.Cu&&!this.K){if(((A=this.Cu.CR)[0]>=A[2])||(A[1]>=A[3])){B.qw(
this,0);B.qw(this.Cu,0);}var HB=false;if(HB)this.Cu.CR=[0,0,(A=this.G)[2]-A[0],A[
3]-A[1]];else this.Cu.CR=B.qR(this.Cu.CR,Ag);}var fullScreenUpdate=false;fullScreenUpdate=
B.jI;if(fullScreenUpdate)Ag=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];if(!!this.K){C.Ai.
Ao.call(this,Ag);return;}Ag=B.il(B.tz(Ag,this.G.slice(0,2)),this.G);if((Ag[0]>=Ag[
2])||(Ag[1]>=Ag[3]))return;var I;for(I=0;I<this.AV;I=I+1)if(!(((A=B.il(this.A1.Get(
I),Ag))[0]>=A[2])||(A[1]>=A[3]))){this.A1.Set(I,B.qR(this.A1.Get(I),Ag));this.Bu.
Set(I,B.s9(this.A1.Get(I)));return;}if(this.AV<3){this.A1.Set(this.AV,Ag);this.Bu.
Set(this.AV,B.s9(Ag));this.AV=this.AV+1;return;}var AA;var BW;var Fy=0;var Fz=0;
var Io=2147483647;this.A1.Set(this.AV,Ag);this.Bu.Set(this.AV,B.s9(Ag));for(AA=0;
AA<=this.AV;AA=AA+1)for(BW=AA+1;BW<=this.AV;BW=BW+1){var GQ=B.s9(B.qR(this.A1.Get(
AA),this.A1.Get(BW)));var IG=((GQ<<8)/(this.Bu.Get(AA)+this.Bu.Get(BW)))|0;if(IG<
Io){Io=IG;Fy=AA;Fz=BW;}}this.A1.Set(Fy,B.qR(this.A1.Get(Fy),this.A1.Get(Fz)));this.
Bu.Set(Fy,B.s9(this.A1.Get(Fy)));if(Fz!==this.AV){this.A1.Set(Fz,this.A1.Get(this.
AV));this.Bu.Set(Fz,this.Bu.Get(this.AV));}},J2:function(){var Bl=B._NewObject(C.
FV,0);Bl.Fb=!!this.Az;if(!!this.Az)Bl.AI=this.Az;return Bl;},ED:function(){var Bl=
B._NewObject(C.FT,0);Bl.Fb=!!this.Az;if(!!this.Az)Bl.AI=this.Az;return Bl;},E3:function(
){var Bl=B._NewObject(C.FU,0);Bl.Fb=!!this.Az;if(!!this.Az)Bl.AI=this.Az;return Bl;
},J3:function(B0){var I;var EH=false;for(I=0;I<10;I=I+1)if(!!this.Av.Get(I)){var
A0=this.Ca.Get(I);var Y=this.Av.Get(I).K;while(!!Y&&(Y!==this)){A0=B.tw(A0,Y.G.slice(
0,2));Y=Y.K;}if(!Y&&(this.Av.Get(I)!==this)){var tmp=this.Av.Get(I);this.Ah=I;this.
Av.Set(I,null);tmp.Bg(this.ED().InitializeUp(I,this.DK.Get(I),this.Dr.Get(I),this.
CH.Get(I),this.A7.Get(I)+1,this.CG.Get(I),false,this.Ca.Get(I),this.C6.Get(I)));
if(tmp===this.C5)this.C5=null;this.BroadcastEvent(this.E3().InitializeUp(I,this.
A7.Get(I)+1,false,tmp,this.Ca.Get(I)),0x18);}else{this.CH.Set(I,(this.DL.AI-this.
E4.Get(I))|0);if(this.CH.Get(I)<10)this.CH.Set(I,10);this.Ah=I;this.Av.Get(I).Bg(
this.ED().InitializeHold(I,A0,this.Dr.Get(I),this.CH.Get(I),this.A7.Get(I)+1,this.
CG.Get(I),this.Ca.Get(I),this.C6.Get(I)));EH=true;}}if(!EH)this.DL.Ch(false);},GetFPS:
function(){var ticksCount=0;var Ix=0;ticksCount=((new Date).getTime()-B.qt)|0;if(
!!this.Gx&&(ticksCount>this.Gx))Ix=((this.Gw*1000)/((ticksCount-this.Gx)|0))|0;this.
Gw=0;this.Gx=ticksCount;return Ix;},Update:function(){var A;if(!this.E2){this.E2=
B._NewObject(B.Graphics.Canvas,0);this.E2.G_([(A=this.G)[2]-A[0],A[3]-A[1]]);}this.
E2.Update();return this.UpdateGE20(this.E2);},UpdateGE20:function(AD){if(!this.BeginUpdate(
))return AT;var Du=this.UpdateCanvas(AD,Ar);this.EndUpdate();return Du;},EndUpdate:
function(){if(this.AV>0){this.Gw=this.Gw+1;this.AV=0;}},UpdateCanvas:function(AD
,aOffset){var A;var Du=AT;var J0=[].concat(aOffset,B.tx(AD.FrameSize,aOffset));var
I;var AA=this.AV;this.FN=this.FN+1;for(I=0;(I<AA)&&(I<4);I=I+1){if(this.Bu.Get(I
)>0){this.CP(AD,B.ty(this.A1.Get(I),aOffset),[-aOffset[0],-aOffset[1]],255,true);
Du=B.qR(Du,B.il(J0,this.A1.Get(I)));}else AA=AA+1;}this.FN=this.FN-1;if(!((Du[0]>=
Du[2])||(Du[1]>=Du[3])))return B.ty(Du,aOffset);else return Du;},GetUpdateRegion:
function(Gm){var I;var AA=this.AV;if(Gm<0)return AT;for(I=0;(I<AA)&&(I<4);I=I+1){
if(!this.Bu.Get(I)){AA=AA+1;Gm=Gm+1;}else if(I===Gm)return this.A1.Get(I);}return AT;
},BeginUpdate:function(){var J$=true;var fullScreenUpdate=false;var I;if((!J$&&!
fullScreenUpdate)&&(this.AV>0)){var IM=B.tA(3,B.qy,null);var HT=this.AV;for(I=0;
I<HT;I=I+1)IM.Set(I,this.A1.Get(I));for(I=0;I<this.IA;I=I+1)this.Ao(this.GD.Get(
I));for(I=0;I<HT;I=I+1)this.GD.Set(I,IM.Get(I));this.IA=HT;}var AA;var BW;for(AA=
0;AA<(this.AV-1);AA=AA+1)if(this.Bu.Get(AA)>0)for(BW=AA+1;BW<this.AV;BW=BW+1)if(
this.Bu.Get(BW)>0){var GQ=B.s9(B.qR(this.A1.Get(AA),this.A1.Get(BW)));if(((GQ-this.
Bu.Get(AA))-this.Bu.Get(BW))<0){this.A1.Set(AA,B.qR(this.A1.Get(AA),this.A1.Get(
BW)));this.Bu.Set(AA,GQ);this.Bu.Set(BW,0);}}for(I=this.AV-1;I>=0;I=I-1)if(!this.
Bu.Get(I))this.AV=this.AV-1;return this.AV;},DoesNeedUpdate:function(){if(this.AV>
0)return true;return false;},Initialize:function(aSize){this.T([].concat(Ar,aSize
));if(this.HD)this.F=this.F|0x60;else this.F=this.F|0x20;this.Ao(this.G);return this;
},SetRootFocus:function(Hp){if(Hp===this.HD)return false;this.HD=Hp;if(!Hp){if(!
!this.CC)this.CC.Bq(0x0,0x40);if(!!this.DR)this.DR.Cb.Bq(0x0,0x40);else this.Bq(
0x0,0x40);}else{if(!!this.DR)this.DR.Cb.Bq(0x40,0x0);else this.Bq(0x40,0x0);if(!
!this.CC)this.CC.Bq(0x40,0x0);}return true;},SetUserInputTimestamp:function(JZ){
this.Az=JZ;},DriveKeyboardHitting:function(AO,Ez,B_){var A;var HP=!!this.BX;if(!
!this.BX&&((!B_||(this.D_!==AO))||(this.DQ!==Ez))){var Bl=null;var J=(C.B9.isPrototypeOf(
A=this.BX)?A:null);var CX=(C.Eo.isPrototypeOf(A=this.BX)?A:null);if(!!this.D_)Bl=
B._NewObject(C.KeyEvent,0).Initialize(this.D_,false);if(this.DQ!==0x00)Bl=B._NewObject(
C.KeyEvent,0).Initialize2(this.DQ,false);if(!!CX)CX.Bg(Bl);else if(!!J)J.Bg(Bl);
this.D_=0;this.DQ=0x00;this.BX=null;}if(!!this.BX){var Bl=null;var J=(C.B9.isPrototypeOf(
A=this.BX)?A:null);var CX=(C.Eo.isPrototypeOf(A=this.BX)?A:null);if(!!AO)Bl=B._NewObject(
C.KeyEvent,0).Initialize(AO,true);if(this.DQ!==0x00)Bl=B._NewObject(C.KeyEvent,0
).Initialize2(Ez,true);if(!!CX)CX.Bg(Bl);else if(!!J)J.Bg(Bl);}if(this.GC&&((!B_||(
this.D_!==AO))||(this.DQ!==Ez))){this.D_=0;this.DQ=0x00;this.GC=false;}if((!this.
BX&&B_)&&(this.Ht>0)){this.D_=AO;this.DQ=Ez;this.GC=true;}if((!this.BX&&B_)&&!this.
GC){if(!!AO)this.BX=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize(AO,
true));if(Ez!==0x00)this.BX=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize2(
Ez,true));if(!(C.Eo.isPrototypeOf(A=this.BX)?A:null)&&!(C.B9.isPrototypeOf(A=this.
BX)?A:null))this.BX=null;this.D_=AO;this.DQ=Ez;HP=HP||!!this.BX;}this.Az=0;return HP;
},DriveCursorMovement:function(AP){return this.DriveMultiTouchMovement(this.Ah,AP
);},DriveMultiTouchMovement:function(M,AP){if((M<0)||(M>9)){this.Az=0;return false;
}var Bt=B.tw(AP,this.Ca.Get(M));this.Ca.Set(M,AP);if(!this.Av.Get(M)||B.tl(Bt,Ar
)){this.Az=0;return false;}var A0=AP;var Y=this.Av.Get(M).K;while(!!Y&&(Y!==this
)){A0=B.tw(A0,Y.G.slice(0,2));Y=Y.K;}if(!Y&&(this.Av.Get(M)!==this)){var tmp=this.
Av.Get(M);this.Ah=M;this.Av.Set(M,null);tmp.Bg(this.ED().InitializeUp(M,this.DK.
Get(M),this.Dr.Get(M),this.CH.Get(M),this.A7.Get(M)+1,this.CG.Get(M),false,this.
Ca.Get(M),this.C6.Get(M)));if(tmp===this.C5)this.C5=null;this.BroadcastEvent(this.
E3().InitializeUp(M,this.A7.Get(M)+1,false,tmp,AP),0x18);}else{this.DK.Set(M,A0);
this.Ah=M;this.Av.Get(M).Bg(this.J2().Initialize(M,A0,this.Dr.Get(M),Bt,this.CH.
Get(M),this.A7.Get(M)+1,this.CG.Get(M),AP,this.C6.Get(M)));}this.Az=0;return true;
},DriveCursorHitting:function(B_,M,AP){return this.DriveMultiTouchHitting(B_,M,AP
);},DriveMultiTouchHitting:function(B_,M,AP){var A;if((M<0)||(M>9)){this.Az=0;return false;
}var ticksCount=this.Az;var EF=[].concat([-this.DW,-this.DW],[this.DW+1,this.DW+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.qt)|0;}var Kg=this.Az;this.
DriveMultiTouchMovement(M,AP);AP=this.Ca.Get(M);this.Az=Kg;if(B_)this.C6.Set(M,AP
);if((B_&&!this.Av.Get(M))&&!this.Ht){var BV=null;var A0=AP;if(B.qu(this.Gt.Get(
M),AP)&&((ticksCount-this.E4.Get(M))<=(((A=this.IX)<0)?A+0x100000000:A)))this.A7.
Set(M,this.A7.Get(M)+1);else this.A7.Set(M,0);this.Gt.Set(M,B.tz(EF,AP));this.E4.
Set(M,ticksCount);if((!!this.CC&&!!this.CC.K)&&((this.CC.F&0x18)===0x18)){var Z=
B.tz(EF,this.CC.K.G6(AP));BV=this.CC.Dx(Z,M,this.A7.Get(M)+1,null,0x0);}if(!BV){
if(!!this.C5&&!!this.C5.K){if(((this.C5.F&0x8)===0x8)&&((this.C5.F&0x10)===0x10)
){var Z=B.tz(EF,this.C5.K.G6(AP));BV=this.C5.Dx(Z,M,this.A7.Get(M)+1,null,0x0);}
}else if(!!this.DR)BV=this.Dx(B.tz(EF,AP),M,this.A7.Get(M)+1,this.DR.Cb,0x0);else
BV=this.Dx(B.tz(EF,AP),M,this.A7.Get(M)+1,null,0x0);}if(!!BV){this.BroadcastEvent(
this.E3().InitializeDown(M,this.A7.Get(M)+1,false,BV.B9,AP),0x18);this.Av.Set(M,
BV.B9);this.CG.Set(M,BV.DD);}else{this.Av.Set(M,null);this.CG.Set(M,Ar);this.Az=
0;return false;}var Y=BV.B9.K;while(!!Y&&(Y!==this)){A0=B.tw(A0,Y.G.slice(0,2));
Y=Y.K;}this.Dr.Set(M,A0);this.DK.Set(M,A0);this.CH.Set(M,0);this.DL.Ch(true);this.
Ah=M;this.Av.Get(M).Bg(this.ED().InitializeDown(M,A0,this.A7.Get(M)+1,this.CG.Get(
M),false,AP));this.Az=0;return true;}if(!B_&&!!this.Av.Get(M)){var A0=AP;var Y=this.
Av.Get(M).K;while(!!Y&&(Y!==this)){A0=B.tw(A0,Y.G.slice(0,2));Y=Y.K;}if(!Y)A0=this.
DK.Get(M);this.Ah=M;var tmp=this.Av.Get(M);this.Av.Set(M,null);tmp.Bg(this.ED().
InitializeUp(M,A0,this.Dr.Get(M),this.CH.Get(M),this.A7.Get(M)+1,this.CG.Get(M),
false,AP,this.C6.Get(M)));this.BroadcastEvent(this.E3().InitializeUp(M,this.A7.Get(
M)+1,false,tmp,AP),0x18);this.Az=0;return true;}this.Az=0;return false;},Ie:function(
Dp,Ik,EB){var EF=[].concat([-this.DW,-this.DW],[this.DW+1,this.DW+1]);if(Dp===this
)Dp=null;if(!this.Av.Get(this.Ah))return;var BV;BV=this.Dx(B.tz(EF,this.Ca.Get(this.
Ah)),this.Ah,1,Dp,EB);if(!!BV&&(this.Av.Get(this.Ah)!==BV.B9))this.HY(BV.B9,BV.DD
);if(!BV&&(this.Av.Get(this.Ah)!==Ik))this.HY(Ik,Ar);},HY:function(Dp,CD){if(!this.
Av.Get(this.Ah)||(this.Av.Get(this.Ah)===Dp))return;var tmp=this.Av.Get(this.Ah);
this.Av.Set(this.Ah,null);tmp.Bg(this.ED().InitializeUp(this.Ah,this.DK.Get(this.
Ah),this.Dr.Get(this.Ah),this.CH.Get(this.Ah),this.A7.Get(this.Ah)+1,this.CG.Get(
this.Ah),true,this.Ca.Get(this.Ah),this.C6.Get(this.Ah)));this.BroadcastEvent(this.
E3().InitializeUp(this.Ah,this.A7.Get(this.Ah)+1,true,tmp,this.Ca.Get(this.Ah)),
0x18);var A0=this.Ca.Get(this.Ah);var Y=null;if(!!Dp)Y=Dp.K;while(!!Y&&(Y!==this
)){A0=B.tw(A0,Y.G.slice(0,2));Y=Y.K;}if(!Y&&(Dp!==this)){this.Av.Set(this.Ah,null
);return;}this.BroadcastEvent(this.E3().InitializeDown(this.Ah,this.A7.Get(this.
Ah)+1,true,Dp,this.Ca.Get(this.Ah)),0x18);var ticksCount=0;ticksCount=((new Date
).getTime()-B.qt)|0;this.Av.Set(this.Ah,Dp);this.CG.Set(this.Ah,CD);this.Dr.Set(
this.Ah,A0);this.DK.Set(this.Ah,A0);this.A7.Set(this.Ah,0);this.CH.Set(this.Ah,0
);this.E4.Set(this.Ah,ticksCount);this.C6.Set(this.Ah,this.Ca.Get(this.Ah));this.
Av.Get(this.Ah).Bg(this.ED().InitializeDown(this.Ah,A0,this.A7.Get(this.Ah)+1,this.
CG.Get(this.Ah),true,this.C6.Get(this.Ah)));},_Init:function(aArg){C.Ai._Init.call(
this,aArg);C.Timer._Init.call(this.DL={S:this},0);(this.Av=[]).__proto__=C.Root.
Av;(this.A7=[]).__proto__=C.Root.A7;(this.Gt=[]).__proto__=C.Root.Gt;(this.CH=[]
).__proto__=C.Root.CH;(this.Dr=[]).__proto__=C.Root.Dr;(this.E4=[]).__proto__=C.
Root.E4;(this.DK=[]).__proto__=C.Root.DK;(this.CG=[]).__proto__=C.Root.CG;(this.
Ca=[]).__proto__=C.Root.Ca;(this.C6=[]).__proto__=C.Root.C6;(this.GD=[]).__proto__=
C.Root.GD;(this.Bu=[]).__proto__=C.Root.Bu;(this.A1=[]).__proto__=C.Root.A1;this.
__proto__=C.Root;this.F=0x7F;this.DL.Fg(50);this.DL.EX=[this,this.J3];},_Done:function(
){this.__proto__=C.Ai;this.DL._Done();C.Ai._Done.call(this);},_ReInit:function(){
C.Ai._ReInit.call(this);this.DL._ReInit();},_Mark:function(D){var A;C.Ai._Mark.call(
this,D);if((A=this.BX)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DR)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.C5)&&(A._cycle!=D))A._Mark(A._cycle=D);B.ts(
this.Av,D);if((A=this.E2)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.CC)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.DL)._cycle!=D)A._Mark(A._cycle=D);},_className:
"Core::Root"};C.Event={AI:0,Fb:false,F0:function(){var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;return ticksCount;},Ce:function(aArg){this.AI=this.F0(
);},_Init:function(aArg){this.__proto__=C.Event;this.Ce(aArg);B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null,_cycle:0,_observers:null,_className:
"Core::Event"};C.KeyEvent={AS:0,Al:0,Down:false,Initialize2:function(AO,B_){this.
AS=0;this.Al=AO;this.Down=B_;if((AO>=0x30)&&(AO<=0x39))this.AS=(10+AO)-48;if((AO>=
0x41)&&(AO<=0x5A))this.AS=(105+AO)-65;if((AO>=0x61)&&(AO<=0x7A))this.AS=(105+AO)-
97;if(AO===0x20)this.AS=131;if(!this.AS)switch(AO){case 0x2B:this.AS=132;break;case
0x2D:this.AS=133;break;case 0x2A:this.AS=134;break;case 0x2F:this.AS=135;break;case
0x3D:this.AS=136;break;case 0x2E:this.AS=137;break;case 0x2C:this.AS=138;break;case
0x3A:this.AS=139;break;case 0x3B:this.AS=140;break;default:;}return this;},Initialize:
function(AO,B_){this.AS=AO;this.Down=B_;this.Al=0x00;var Hv=AO-10;var Hu=AO-105;
if((Hv>=0)&&(Hv<=9))this.Al=(48+Hv)&0xFFFF;if((Hu>=0)&&(Hu<=25))this.Al=(65+Hu)&
0xFFFF;if(AO===131)this.Al=0x20;if(this.Al===0x00)switch(AO){case 132:this.Al=0x2B;
break;case 133:this.Al=0x2D;break;case 134:this.Al=0x2A;break;case 135:this.Al=0x2F;
break;case 136:this.Al=0x3D;break;case 137:this.Al=0x2E;break;case 138:this.Al=0x2C;
break;case 139:this.Al=0x3A;break;case 140:this.Al=0x3B;break;default:;}return this;
},I4:function(Ij){switch(Ij){case 141:return((this.Al>=0x41)&&(this.Al<=0x5A))||((
this.Al>=0x61)&&(this.Al<=0x7A));case 142:return(((this.Al>=0x41)&&(this.Al<=0x5A
))||((this.Al>=0x61)&&(this.Al<=0x7A)))||((this.Al>=0x30)&&(this.Al<=0x39));case
143:return(this.Al>=0x30)&&(this.Al<=0x39);case 144:return(((this.Al>=0x41)&&(this.
Al<=0x46))||((this.Al>=0x61)&&(this.Al<=0x66)))||((this.Al>=0x30)&&(this.Al<=0x39
));case 145:return this.Al!==0x00;case 146:return(this.Al===0x00)&&!!this.AS;case
147:return(((this.AS===6)||(this.AS===7))||(this.AS===4))||(this.AS===5);case 148:
return(this.Al!==0x00)||!!this.AS;default:;}return Ij===this.AS;},_Init:function(
aArg){C.Event._Init.call(this,aArg);this.__proto__=C.KeyEvent;},_className:"Core::KeyEvent"
};C.FU={Gg:null,CA:B.qx,CB:0,Bw:0,Down:false,Dg:false,InitializeUp:function(M,B$
,Ey,Hs,CU){this.Down=false;this.Bw=M;this.CB=B$;this.CA=CU;this.Gg=Hs;this.Dg=Ey;
return this;},InitializeDown:function(M,B$,Ey,Hs,CU){this.Down=true;this.Bw=M;this.
CB=B$;this.CA=CU;this.Gg=Hs;this.Dg=Ey;return this;},_Init:function(aArg){C.Event.
_Init.call(this,aArg);this.__proto__=C.FU;},_Mark:function(D){var A;C.Event._Mark.
call(this,D);if((A=this.Gg)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::CursorGrabEvent"
};C.FT={Dz:B.qx,CA:B.qx,CB:0,BM:0,Dk:B.qx,Cy:B.qx,Bw:0,Down:false,Dg:false,InitializeHold:
function(M,D3,Fo,Fp,B$,CD,CU,Fn){this.Down=true;this.Bw=M;this.Cy=B.tx(D3,CD);this.
Dk=B.tx(Fo,CD);this.BM=Fp;this.CB=B$;this.CA=CU;this.Dz=Fn;return this;},InitializeUp:
function(M,D3,Fo,Fp,B$,CD,Ey,CU,Fn){this.Down=false;this.Bw=M;this.Cy=B.tx(D3,CD
);this.Dk=B.tx(Fo,CD);this.BM=Fp;this.CB=B$;this.Dg=Ey;this.CA=CU;this.Dz=Fn;return this;
},InitializeDown:function(M,D3,B$,CD,Ey,CU){this.Down=true;this.Bw=M;this.Cy=B.tx(
D3,CD);this.Dk=B.tx(D3,CD);this.BM=0;this.CB=B$;this.Dg=Ey;this.CA=CU;this.Dz=CU;
return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=
C.FT;},_className:"Core::CursorEvent"};C.FV={Dz:B.qx,CA:B.qx,CB:0,BM:0,DD:B.qx,Dk:
B.qx,Cy:B.qx,Bw:0,Initialize:function(M,D3,Fo,aOffset,Fp,JY,CD,CU,Fn){this.Bw=M;
this.Cy=B.tx(D3,CD);this.Dk=B.tx(Fo,CD);this.DD=aOffset;this.BM=Fp;this.CB=JY;this.
CA=CU;this.Dz=Fn;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg
);this.__proto__=C.FV;},_className:"Core::DragEvent"};C.Fj={D1:null,Bz:B.qx,Jn:0
,Jm:0,Space:0,FZ:0,CP:function(AD,aClip,aOffset,Ay,aBlend){},T:function(E){var A;
if(B.tm(E,this.G))return;var Ec=[(A=this.G)[2]-A[0],A[3]-A[1]];var GF=[E[2]-E[0]
,E[3]-E[1]];var EM=!B.tl(Ec,GF);var Bt=B.tw(E.slice(0,2),this.G.slice(0,2));if(!
B.tl(Bt,Ar)&&!EM){var J=this.AU;while(!!J&&!((J.F&0x200)===0x200)){if(((J.F&0x400
)===0x400)){var tmp=((J.F&0x100)===0x100);J.G7(Bt,tmp);}J=J.AU;}B.lq(this.D1,this
);}if((EM&&(Ec[0]>0))&&(Ec[1]>0)){var Ab=B.tz(this.G,this.Bz);var J=this.AU;var Fv=
0x14;while(!!J&&!((J.F&0x200)===0x200)){if(((J.F&0x400)===0x400)){if(!!J.Aw&&(J.
Aw.EJ!==this))J.Aw=null;if(!J.Aw&&((J.DY!==Fv)||!!this.FZ))J.Fw(Ab,this);}J=J.AU;
}B.lq(this.D1,this);}C.A2.T.call(this,E);if(!!this.K&&EM){this.F=this.F|0x1000;if(
!((this.K.F&0x2000)===0x2000)){this.K.F=this.K.F|0x4000;B.lq([A=this.K,A.B3],this
);}}},_Init:function(aArg){C.A2._Init.call(this,aArg);this.__proto__=C.Fj;this.F=
0x203;},_Mark:function(D){var A;C.A2._Mark.call(this,D);if((A=this.D1)&&((A=A[0]
)._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::Outline"};C.BN={G9:null,H$:
null,H7:null,Fe:null,D0:null,DE:null,GP:0,Au:0,Bw:0,AI:0,CB:0,BM:0,DD:B.qx,Dk:B.
qx,Cy:B.qx,Jj:1000,Gf:8,D2:0,H4:1,G5:-1,I5:1,I6:1,GZ:false,Down:false,ES:false,Dg:
false,CP:function(AD,aClip,aOffset,Ay,aBlend){},Bg:function(Ad){var A;var As=(C.
FT.isPrototypeOf(Ad)?Ad:null);var AY=(C.FV.isPrototypeOf(Ad)?Ad:null);var Gy=this.
ES;var GG;var E8;var IL;var DT;var Iu;if(!As&&!AY)return null;GG=(!!As&&As.Down)&&
!As.BM;E8=(!!As&&As.Down)&&(As.BM>0);IL=(!!As&&As.Down)&&(As.BM>50);DT=!!As&&!As.
Down;Iu=!!AY;if(!this.GZ){if((((this.D2&0x20)===0x20)&&(this.Au>0))&&(this.Au<33554432
)){var Gv=(C.FU.isPrototypeOf(Ad)?Ad:null);if(((!!Gv&&Gv.Down)&&(Gv.Gg!==this))&&
B.qu(this.GetExtent(),this.K.G6(Gv.CA))){this.GP=0x20;this.Au=this.Au|67108864;return null;
}}if(GG){var HL=0;var Fu;this.Au=this.Au|(1<<As.Bw);for(Fu=this.Au&4095;Fu>0;Fu=
Fu>>1)if(!!(Fu&1))HL=HL+1;if(HL===this.H4)this.Au=(this.Au|16777216)|(4096<<As.Bw
);}if(DT)this.Au=(this.Au&~(1<<As.Bw))|33554432;if(IL&&(this.Au<16777216))this.Au=
this.Au|67108864;if(DT&&As.Dg)this.Au=this.Au|67108864;if(DT&&!(this.Au&4095))this.
GP=0x0;if(DT&&!(this.Au&16777215))this.Au=0;if(E8&&(this.Au>=67108864))this.G1().
Ie(null,null,this.GP);if((E8&&!!(this.Au&16777216))&&!!(this.Au&33554432)){E8=false;
DT=true;}if(!!As&&!(this.Au&(4096<<As.Bw)))return this;if(!!AY&&!(this.Au&(4096<<
AY.Bw)))return this;if(DT&&!(this.Au&16777216))return this;if(((GG||Iu)||E8)&&((
this.Au<16777216)||(this.Au>=33554432)))return this;if(DT)this.Au=this.Au&3758100479;
if(DT&&!(this.Au&16777215))this.Au=0;}if(!!As){this.Down=GG||E8;this.ES=this.G4(
As.Cy);this.Dk=As.Dk;this.Cy=As.Cy;this.DD=Ar;this.BM=As.BM;this.CB=As.CB;this.Dg=
As.Dg;this.Bw=As.Bw;this.AI=As.AI;if(As.Down&&!As.BM)Gy=false;}if(!!AY){this.Down=
true;this.ES=this.G4(AY.Cy);this.Dk=AY.Dk;this.Cy=AY.Cy;this.DD=AY.DD;this.BM=AY.
BM;this.CB=AY.CB;this.Bw=AY.Bw;this.Dg=false;this.AI=AY.AI;(A=this.G9)?A[1].call(
A[0],this):null;}if((!!As&&this.Down)&&!this.BM)(A=this.DE)?A[1].call(A[0],this):
null;if((!!As&&this.Down)&&(this.BM>0))(A=this.Fe)?A[1].call(A[0],this):null;if((
this.Down&&this.ES)&&!Gy)(A=this.H7)?A[1].call(A[0],this):null;if(((this.Down&&!
this.ES)&&Gy)||((!this.Down&&this.ES)&&Gy))(A=this.H$)?A[1].call(A[0],this):null;
if(!!As&&!this.Down)(A=this.D0)?A[1].call(A[0],this):null;if(!!this.D2){var Ee=0x0;
if(((((this.D2&0x10)===0x10)&&!!As)&&As.Down)&&(As.BM>=this.Jj))Ee=0x10;if((((this.
D2&0x1)===0x1)&&!!AY)&&((AY.CA[1]-AY.Dz[1])<=-this.Gf))Ee=0x1;if((((this.D2&0x2)===
0x2)&&!!AY)&&((AY.CA[1]-AY.Dz[1])>=this.Gf))Ee=0x2;if((((this.D2&0x4)===0x4)&&!!
AY)&&((AY.CA[0]-AY.Dz[0])<=-this.Gf))Ee=0x4;if((((this.D2&0x8)===0x8)&&!!AY)&&((
AY.CA[0]-AY.Dz[0])>=this.Gf))Ee=0x8;if(!!Ee){this.GP=Ee;this.G1().Ie(null,this,Ee
);}}return this;},Dx:function(Ag,M,B$,E0,EB){var A;if(!!E0&&(E0!==this))return null;
if((B$<this.I6)||(B$>this.I5))return null;if((this.G5>=0)&&(this.H4>1))return null;
if((this.G5>=0)&&(M!==this.G5))return null;if(!this.GZ&&(this.Au>=33554432))return null;
if((!this.GZ&&(this.Au>=16777216))&&!(this.Au&(4096<<M)))return null;if(!!(EB&this.
D2))return null;if(this.I2()){var CZ=B.il(Ag,this.GetExtent());if(!((CZ[0]>=CZ[2
])||(CZ[1]>=CZ[3]))){var D5=B.s_(Ag);var Bt=Ar;if(D5[0]<CZ[0])Bt=[CZ[0]-D5[0],Bt[
1]];if(D5[0]>=CZ[2])Bt=[(CZ[2]-D5[0])-1,Bt[1]];if(D5[1]<CZ[1])Bt=[Bt[0],CZ[1]-D5[
1]];if(D5[1]>=CZ[3])Bt=[Bt[0],(CZ[3]-D5[1])-1];return B._NewObject(C.GU,0).Initialize(
this,Bt);}}else{var Ax=B.tA(9,B.qx,null);var I;Ax.Set(0,B.s_(Ag));Ax.Set(1,Ax.Get(
0));Ax.Set(2,Ax.Get(0));Ax.Set(3,Ax.Get(0));Ax.Set(4,Ax.Get(0));Ax.Set(1,[Ag[0],
Ax.Get(1)[1]]);Ax.Set(2,[Ax.Get(2)[0],Ag[1]]);Ax.Set(3,[Ag[2],Ax.Get(3)[1]]);Ax.
Set(4,[Ax.Get(4)[0],Ag[3]]);Ax.Set(5,Ag.slice(0,2));Ax.Set(6,[Ag[2],Ag[1]]);Ax.Set(
7,[Ag[0],Ag[3]]);Ax.Set(8,Ag.slice(2,4));for(I=0;I<9;I=I+1)if(this.G4(Ax.Get(I))
)return B._NewObject(C.GU,0).Initialize(this,B.tw(Ax.Get(I),Ax.Get(0)));}return null;
},_Init:function(aArg){C.Fl._Init.call(this,aArg);this.__proto__=C.BN;this.F=0x11B;
},_Mark:function(D){var A;C.Fl._Mark.call(this,D);if((A=this.G9)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.H$)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.H7)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fe)&&((A=A[
0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.D0)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.DE)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:
"Core::SimpleTouchHandler"};C.Eo={AU:null,D0:null,DE:null,Fe:null,EL:0,AI:0,Id:0
,FY:148,AS:0,Al:0,Di:true,Down:false,Hi:false,FS:false,Bg:function(Ad){var A;if(
!!Ad&&Ad.I4(this.FY)){this.Down=Ad.Down;this.AS=Ad.AS;this.Al=Ad.Al;this.AI=Ad.AI;
this.FS=false;if(Ad.Down){this.Id=this.EL;this.Hi=this.EL>0;if(this.Hi)(A=this.Fe
)?A[1].call(A[0],this):null;else(A=this.DE)?A[1].call(A[0],this):null;if(!this.FS
)this.EL=this.EL+1;return!this.FS;}if(!Ad.Down){this.Hi=this.EL>1;this.Id=this.EL-
1;this.EL=0;(A=this.D0)?A[1].call(A[0],this):null;return!this.FS;}}return false;
},Ce:function(aArg){var A;var Cb=(C.Ai.isPrototypeOf(A=this.S)?A:null);if(!Cb)throw new
Error(Hn);this.AU=Cb.GB;Cb.GB=this;},_Init:function(aArg){this.__proto__=C.Eo;this.
Ce(aArg);B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(
){},_Mark:function(D){var A;if((A=this.AU)&&(A._cycle!=D))A._Mark(A._cycle=D);if((
A=this.D0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.DE)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Fe)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null,_cycle:0,_observers:
null,_className:"Core::KeyPressHandler"};C.GU={B9:null,E$:0,DD:B.qx,Initialize:function(
W,aOffset){this.B9=W;this.DD=aOffset;this.E$=(aOffset[0]*aOffset[0])+(aOffset[1]
*aOffset[1]);return this;},_Init:function(aArg){this.__proto__=C.GU;B.gv++;},_Done:
function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var
A;if((A=this.B9)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.S)&&(A._cycle!=D)
)A._Mark(A._cycle=D);},S:null,_cycle:0,_observers:null,_className:"Core::CursorHit"
};C.I7={Cb:null,_Init:function(aArg){this.__proto__=C.I7;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Cb)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.S)&&(A._cycle!=D))A._Mark(
A._cycle=D);},S:null,_cycle:0,_observers:null,_className:"Core::ModalContext"};C.
F_={EJ:null,BT:B.qy,Ab:B.qy,isEmpty:false,_Init:function(aArg){this.__proto__=C.
F_;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.EJ)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.S
)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null,_cycle:0,_observers:null,_className:
"Core::LayoutContext"};C.F$={FG:B.qx,FF:B.qx,FE:B.qx,FD:B.qx,_Init:function(aArg
){C.F_._Init.call(this,aArg);this.__proto__=C.F$;},_className:"Core::LayoutQuadContext"
};C.IY={Cb:null,_Init:function(aArg){this.__proto__=C.IY;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Cb)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.S)&&(A._cycle!=D))A._Mark(
A._cycle=D);},S:null,_cycle:0,_observers:null,_className:"Core::DialogContext"};
C.Jp={Hx:null,Cw:null,_Init:function(aArg){this.__proto__=C.Jp;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Hx)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cw)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null,_cycle:0,_observers:
null,_className:"Core::TaskQueue"};C.Jo={_Init:function(aArg){this.__proto__=C.Jo;
B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null,_cycle:
0,_observers:null,_className:"Core::Task"};C.DH={resource:null,C2:function(){this.
resource=null;},Ce:function(aArg){this.resource=aArg;},_Init:function(aArg){this.
__proto__=C.DH;this.Ce(aArg);B.gv++;},_Done:function(){this.C2();this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.S)&&(A._cycle
!=D))A._Mark(A._cycle=D);},S:null,_cycle:0,_observers:null,_className:"Core::Resource"
};C.Timer={EX:null,timer:null,AI:0,Period:1000,FR:0,Di:false,C2:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},GI:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.sL(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},Fg:function(E){if(E<0)E=0;if(E===this.Period)return;
this.Period=E;if(this.Di)this.GI(this.FR,E);},I$:function(E){if(E<0)E=0;if(E===this.
FR)return;this.FR=E;if(this.Di)this.GI(E,this.Period);},Ch:function(E){if(E===this.
Di)return;this.Di=E;if(E)this.GI(this.FR,this.Period);else this.GI(0,0);this.AI=
this.F0();},F0:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.qt
)|0;return ticksCount;},Trigger:function(){var A;this.AI=this.F0();if(!this.Period
)this.Ch(false);(A=this.EX)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=C.Timer;B.gv++;},_Done:function(){this.C2();this.__proto__=null;B.gv--;
},_ReInit:function(){},_Mark:function(D){var A;if((A=this.EX)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null
,_cycle:0,_observers:null,_className:"Core::Timer"};C.LF={LG:0x1,Kp:0x2,KT:0x4,LB:
0x8,Di:0x10,Ls:0x20,KU:0x40,K6:0x80,KS:0x100,K0:0x200,KM:0x400,Le:0x800,Ig:0x1000
,LE:0x2000,K$:0x4000,La:0x8000,KF:0x10000,K_:0x20000,Lp:0x40000};C.DY={Lf:0x1,Lg:
0x2,Km:0x4,Kn:0x8,Ko:0x10,Kl:0x20};C.FZ={K7:0,Ly:1,Ku:2,K1:3,Li:4,Lz:5,LA:6,Kv:7
,Kw:8,K3:9,K2:10,Lk:11,Lj:12};C.KeyCode={NoKey:0,Ok:1,Exit:2,Menu:3,Up:4,Down:5,
Left:6,Right:7,PageUp:8,PageDown:9,Key0:10,Key1:11,Key2:12,Key3:13,Key4:14,Key5:
15,Key6:16,Key7:17,Key8:18,Key9:19,Red:20,Green:21,Blue:22,Yellow:23,White:24,Magenta:
25,F1:26,F2:27,F3:28,F4:29,F5:30,F6:31,F7:32,F8:33,F9:34,F10:35,ChannelUp:36,ChannelDown:
37,Display:38,SkipPrev:39,SkipNext:40,Home:41,End:42,Insert:43,Delete:44,Clear:45
,VolumeUp:46,VolumeDown:47,Show:48,Hide:49,Play:50,Pause:51,Record:52,Stop:53,Rev:
54,Fwd:55,SlowRev:56,SlowFwd:57,SkipBwd:58,SkipFwd:59,Repeat:60,Eject:61,Help:62
,TV:63,DVD:64,VCR:65,EPG:66,OSD:67,Text:68,PIP:69,Audio:70,Clock:71,Timer:72,Navigation:
73,Karaoke:74,Game:75,Subtitle:76,Zoom:77,Index:78,Info:79,Power:80,Setup:81,Angle:
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
346};C.Lh={LL:0x1,LI:0x2,LJ:0x4,LK:0x8,K5:0x10,KV:0x20};
C._Init=function(){C.Fl.__proto__=C.B9;C.A2.__proto__=C.B9;C.Ai.__proto__=C.A2;C.
Root.__proto__=C.Ai;C.KeyEvent.__proto__=C.Event;C.FU.__proto__=C.Event;C.FT.__proto__=
C.Event;C.FV.__proto__=C.Event;C.Fj.__proto__=C.A2;C.BN.__proto__=C.Fl;C.F$.__proto__=
C.F_;};C.BO=function(D){};return C;})();

/* Embedded Wizard */