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
var At=[0,0];var AT=[0,0,0,0];var Cq="The view does not belong to this group";var
DW="No view to restack";var DX="View is not in this group";var Eo="No view to remove";
var Ep="No view to add";var G4="View already in a group";var G5="Recursive invalidate during active update cycle.";
var G6="The KeyPressHandler is embedded within an object not being derived "+"from Core::Group.";
C.BN={Ap:null,Ai:null,K:null,Au:null,F:0x103,DS:0x14,Fm:function(V,F_){},FU:function(
E){var A;var D3=E^this.DS;if(!D3)return;this.DS=E;if(!!this.Au&&!((this.F&0x400)===
0x400)){this.K.F=this.K.F|0x5000;B.kD([A=this.K,A.Bu],this);this.K.Aq([0,0,(A=this.
K.G)[2]-A[0],A[3]-A[1]]);}if(!!this.Au&&((this.F&0x400)===0x400)){this.Au.EB.F=this.
Au.EB.F|0x1000;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);}},EJ:function(
){var Y=this.K;while(!!Y){var Iu=(C.Root.isPrototypeOf(Y)?Y:null);if(!!Iu)return Iu;
Y=Y.K;}return null;},CL:function(AC,aClip,aOffset,Ax,aBlend){},Bb:function(Ad){return null;
},Ds:function(Ag,N,B7,ET,Et){return null;},FF:function(V,Cr){return At;},GL:function(
aOffset,F0){},GetExtent:function(){return AT;},Bn:function(DD,Es){var A;if(((this.
F&0x200)===0x200))DD=DD&~0x400;var Hs=(this.F&~Es)|DD;var D4=Hs^this.F;this.F=Hs;
if(!!this.K&&!!(D4&0x14)){var Ii=((this.F&0x14)===0x14);if(Ii&&!this.K.Cx)this.K.
EP(this);if(!Ii&&(this.K.Cx===this))this.K.EP(this.K.HK(this,0x14));}if(!!this.K&&
!!(D4&0x403))this.K.Aq(this.GetExtent());if((!!this.Au&&((Hs&0x400)===0x400))&&((
D4&0x1)===0x1)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu
],this);}if(!!this.K&&((D4&0x400)===0x400)){this.Au=null;this.F=this.F|0x800;this.
K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);}},_Init:function(aArg){this.__proto__=
C.BN;B.gb++;},_Done:function(){this.__proto__=C.BN;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Ap)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Ai)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.K)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Au)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.T)&&(A.
_cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:null,_className:"Core::View"
};C.Fb={Bf:B.pm,Bx:B.pm,Be:B.pm,Bw:B.pm,Fm:function(V,F_){var X=B._NewObject(C.FR
,0);this.Au=null;X.BS=this.GetExtent();X.Ab=V;X.EB=F_;X.Ft=this.Bw;X.Fu=this.Be;
X.Fv=this.Bx;X.Fw=this.Bf;this.Au=X;},FF:function(V,Cr){var A;var AV=this.DS;var
X=(C.FR.isPrototypeOf(A=this.Au)?A:null);var P=X.BS[0];var Q=X.BS[1];var R=X.BS[
2];var S=X.BS[3];var BR=[V[2]-V[0],V[3]-V[1]];var AF=R-P;var AE=S-Q;if(!Cr){var CJ=[(
A=X.Ab)[2]-A[0],A[3]-A[1]];P=P-X.Ab[0];Q=Q-X.Ab[1];if(CJ[0]!==BR[0]){var Cg=((AV&
0x4)===0x4);var Ch=((AV&0x8)===0x8);var C7=((AV&0x1)===0x1);if(!Cg&&(C7||!Ch))P=((
P*BR[0])/CJ[0])|0;if(!Ch&&(C7||!Cg)){R=R-X.Ab[0];R=((R*BR[0])/CJ[0])|0;R=R-BR[0];
}else R=R-X.Ab[2];P=P+V[0];R=R+V[2];if(!C7){if(Cg&&!Ch)R=P+AF;else if(!Cg&&Ch)P=
R-AF;else{P=P+((((R-P)-AF)/2)|0);R=P+AF;}}}else{R=R-X.Ab[2];P=P+V[0];R=R+V[2];}if(
CJ[1]!==BR[1]){var Ci=((AV&0x10)===0x10);var Cf=((AV&0x20)===0x20);var C8=((AV&0x2
)===0x2);if(!Ci&&(C8||!Cf))Q=((Q*BR[1])/CJ[1])|0;if(!Cf&&(C8||!Ci)){S=S-X.Ab[1];
S=((S*BR[1])/CJ[1])|0;S=S-BR[1];}else S=S-X.Ab[3];Q=Q+V[1];S=S+V[3];if(!C8){if(Ci&&
!Cf)S=Q+AE;else if(!Ci&&Cf)Q=S-AE;else{Q=Q+((((S-Q)-AE)/2)|0);S=Q+AE;}}}else{S=S-
X.Ab[3];Q=Q+V[1];S=S+V[3];}}else{switch(Cr){case 3:{P=V[0];R=P+AF;}break;case 4:{
R=V[2];P=R-AF;}break;case 1:{Q=V[1];S=Q+AE;}break;case 2:{S=V[3];Q=S-AE;}break;default:;
}if((Cr===3)||(Cr===4)){var Ci=((AV&0x10)===0x10);var Cf=((AV&0x20)===0x20);var C8=((
AV&0x2)===0x2);if(C8){Q=V[1];S=V[3];}else if(Ci&&!Cf){Q=V[1];S=Q+AE;}else if(Cf&&
!Ci){S=V[3];Q=S-AE;}else{Q=V[1]+((((V[3]-V[1])-AE)/2)|0);S=Q+AE;}}if((Cr===1)||(
Cr===2)){var Cg=((AV&0x4)===0x4);var Ch=((AV&0x8)===0x8);var C7=((AV&0x1)===0x1);
if(C7){P=V[0];R=V[2];}else if(Cg&&!Ch){P=V[0];R=P+AF;}else if(Ch&&!Cg){R=V[2];P=
R-AF;}else{P=V[0]+((((V[2]-V[0])-AF)/2)|0);R=P+AF;}}}X.isEmpty=(P>=R)||(Q>=S);AF=(
R-P)-1;AE=(S-Q)-1;var DG=X.BS[0];var DH=X.BS[1];var Dm=(X.BS[2]-DG)-1;var Dl=(X.
BS[3]-DH)-1;if(!Dm)Dm=1;if(!Dl)Dl=1;if(((this.F&0x100)===0x100)){this.Bw=[P+((((
X.Ft[0]-DG)*AF)/Dm)|0),Q+((((X.Ft[1]-DH)*AE)/Dl)|0)];this.Be=[P+((((X.Fu[0]-DG)*
AF)/Dm)|0),Q+((((X.Fu[1]-DH)*AE)/Dl)|0)];this.Bx=[P+((((X.Fv[0]-DG)*AF)/Dm)|0),Q+((((
X.Fv[1]-DH)*AE)/Dl)|0)];this.Bf=[P+((((X.Fw[0]-DG)*AF)/Dm)|0),Q+((((X.Fw[1]-DH)*
AE)/Dl)|0)];}else{this.GT([P+((((X.Ft[0]-DG)*AF)/Dm)|0),Q+((((X.Ft[1]-DH)*AE)/Dl
)|0)]);this.GU([P+((((X.Fu[0]-DG)*AF)/Dm)|0),Q+((((X.Fu[1]-DH)*AE)/Dl)|0)]);this.
GV([P+((((X.Fv[0]-DG)*AF)/Dm)|0),Q+((((X.Fv[1]-DH)*AE)/Dl)|0)]);this.GW([P+((((X.
Fw[0]-DG)*AF)/Dm)|0),Q+((((X.Fw[1]-DH)*AE)/Dl)|0)]);this.Au=X;}return[AF+1,AE+1];
},GL:function(aOffset,F0){if(F0){this.Bw=B.so(this.Bw,aOffset);this.Be=B.so(this.
Be,aOffset);this.Bx=B.so(this.Bx,aOffset);this.Bf=B.so(this.Bf,aOffset);}else{this.
GT(B.so(this.Bw,aOffset));this.GU(B.so(this.Be,aOffset));this.GV(B.so(this.Bx,aOffset
));this.GW(B.so(this.Bf,aOffset));}},GetExtent:function(){if(!!this.Au&&this.Au.
isEmpty)return AT;var P=this.Bw[0];var Q=this.Bw[1];var R=this.Bx[0];var S=this.
Bx[1];if((((this.Bf[0]!==P)||(this.Be[1]!==Q))||(this.Be[0]!==R))||(this.Bf[1]!==
S)){if(this.Be[0]<P)P=this.Be[0];if(this.Bx[0]<P)P=this.Bx[0];if(this.Bf[0]<P)P=
this.Bf[0];if(this.Be[1]<Q)Q=this.Be[1];if(this.Bx[1]<Q)Q=this.Bx[1];if(this.Bf[
1]<Q)Q=this.Bf[1];if(this.Bw[0]>R)R=this.Bw[0];if(this.Be[0]>R)R=this.Be[0];if(this.
Bf[0]>R)R=this.Bf[0];if(this.Bw[1]>S)S=this.Bw[1];if(this.Be[1]>S)S=this.Be[1];if(
this.Bf[1]>S)S=this.Bf[1];}else{var tmp;if(R<P){tmp=P;P=R;R=tmp;}if(S<Q){tmp=Q;Q=
S;S=tmp;}}return[P,Q,R+1,S+1];},GW:function(E){var A;if(B.sa(E,this.Bf))return;if(
!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.GetExtent());this.Au=null;this.Bf=
E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.GetExtent());if((!!this.K&&((
this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);}},GV:function(E){var A;if(B.sa(E
,this.Bx))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.GetExtent());this.
Au=null;this.Bx=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.GetExtent());
if((!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);}},GU:function(E){var
A;if(B.sa(E,this.Be))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.GetExtent(
));this.Au=null;this.Be=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.GetExtent(
));if((!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);}},GT:function(
E){var A;if(B.sa(E,this.Bw))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.
GetExtent());this.Au=null;this.Bw=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(
this.GetExtent());if((!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===
0x2000)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);
}},GK:function(C0){var Aw=B.sr(4,B.pm,null);var I=0;var Az=3;var Hm=false;var Hn=
false;Aw.Set(0,this.Bw);Aw.Set(1,this.Be);Aw.Set(2,this.Bx);Aw.Set(3,this.Bf);while(
I<4){var Iz=Aw.Get(I)[0];var GB=Aw.Get(I)[1];var JY=Aw.Get(Az)[0];var HD=Aw.Get(
Az)[1];if(((GB>C0[1])!==(HD>C0[1]))||((GB<C0[1])!==(HD<C0[1]))){var Bj=((((JY-Iz
)*(C0[1]-GB))/(HD-GB))|0)+Iz;if(C0[0]>Bj)Hm=!Hm;if(C0[0]<Bj)Hn=!Hn;}Az=I;I=I+1;}
return Hm||Hn;},IN:function(){return((((this.Bw[0]===this.Bf[0])&&(this.Be[0]===
this.Bx[0]))&&(this.Bw[1]===this.Be[1]))&&(this.Bx[1]===this.Bf[1]))||((((this.Bw[
0]===this.Be[0])&&(this.Bx[0]===this.Bf[0]))&&(this.Bw[1]===this.Bf[1]))&&(this.
Be[1]===this.Bx[1]));},_Init:function(aArg){C.BN._Init.call(this,aArg);this.__proto__=
C.Fb;},_className:"Core::QuadView"};C.Bg={G:B.pn,Fm:function(V,F_){var X=B._NewObject(
C.E4,0);X.BS=this.G;X.Ab=V;X.EB=F_;this.Au=X;},FF:function(V,Cr){var A;var AV=this.
DS;var X=this.Au;var P=X.BS[0];var Q=X.BS[1];var R=X.BS[2];var S=X.BS[3];var BR=[
V[2]-V[0],V[3]-V[1]];var AF=R-P;var AE=S-Q;if(!Cr){var CJ=[(A=X.Ab)[2]-A[0],A[3]-
A[1]];P=P-X.Ab[0];Q=Q-X.Ab[1];if(CJ[0]!==BR[0]){var Cg=((AV&0x4)===0x4);var Ch=((
AV&0x8)===0x8);var C7=((AV&0x1)===0x1);if(!Cg&&(C7||!Ch))P=((P*BR[0])/CJ[0])|0;if(
!Ch&&(C7||!Cg)){R=R-X.Ab[0];R=((R*BR[0])/CJ[0])|0;R=R-BR[0];}else R=R-X.Ab[2];P=
P+V[0];R=R+V[2];if(!C7){if(Cg&&!Ch)R=P+AF;else if(!Cg&&Ch)P=R-AF;else{P=P+((((R-
P)-AF)/2)|0);R=P+AF;}}}else{R=R-X.Ab[2];P=P+V[0];R=R+V[2];}if(CJ[1]!==BR[1]){var
Ci=((AV&0x10)===0x10);var Cf=((AV&0x20)===0x20);var C8=((AV&0x2)===0x2);if(!Ci&&(
C8||!Cf))Q=((Q*BR[1])/CJ[1])|0;if(!Cf&&(C8||!Ci)){S=S-X.Ab[1];S=((S*BR[1])/CJ[1]
)|0;S=S-BR[1];}else S=S-X.Ab[3];Q=Q+V[1];S=S+V[3];if(!C8){if(Ci&&!Cf)S=Q+AE;else
if(!Ci&&Cf)Q=S-AE;else{Q=Q+((((S-Q)-AE)/2)|0);S=Q+AE;}}}else{S=S-X.Ab[3];Q=Q+V[1
];S=S+V[3];}}else{switch(Cr){case 3:{P=V[0];R=P+AF;}break;case 4:{R=V[2];P=R-AF;
}break;case 1:{Q=V[1];S=Q+AE;}break;case 2:{S=V[3];Q=S-AE;}break;default:;}if((Cr===
3)||(Cr===4)){var Ci=((AV&0x10)===0x10);var Cf=((AV&0x20)===0x20);var C8=((AV&0x2
)===0x2);if(C8){Q=V[1];S=V[3];}else if(Ci&&!Cf){Q=V[1];S=Q+AE;}else if(Cf&&!Ci){
S=V[3];Q=S-AE;}else{Q=V[1]+((((V[3]-V[1])-AE)/2)|0);S=Q+AE;}}if((Cr===1)||(Cr===
2)){var Cg=((AV&0x4)===0x4);var Ch=((AV&0x8)===0x8);var C7=((AV&0x1)===0x1);if(C7
){P=V[0];R=V[2];}else if(Cg&&!Ch){P=V[0];R=P+AF;}else if(Ch&&!Cg){R=V[2];P=R-AF;
}else{P=V[0]+((((V[2]-V[0])-AF)/2)|0);R=P+AF;}}}X.isEmpty=(P>=R)||(Q>=S);if(((this.
F&0x100)===0x100)){this.G=[P,Q,R,S];}else{this.U([P,Q,R,S]);this.Au=X;}return[R-
P,S-Q];},GL:function(aOffset,F0){if(F0)this.G=B.sq(this.G,aOffset);else this.U(B.
sq(this.G,aOffset));},GetExtent:function(){return this.G;},U:function(E){var A;if(
B.sb(E,this.G))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);this.Au=
null;this.G=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);if((!!this.K&&((
this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.
K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],this);}},_Init:function(aArg){C.BN._Init.
call(this,aArg);this.__proto__=C.Bg;},_className:"Core::RectView"};C.Al={B9:null
,Cv:null,Go:null,Cs:null,D6:null,Ex:null,Cx:null,Ek:255,CL:function(AC,aClip,aOffset
,Ax,aBlend){var A;Ax=((Ax+1)*this.Ek)>>8;aBlend=aBlend&&((this.F&0x2)===0x2);if(
!this.Cs)this.JJ(AC,aClip,B.so(aOffset,this.G.slice(0,2)),Ax,aBlend);else{var BP=
255|(255<<8)|(255<<16)|((Ax&0xFF)<<24);this.Cs.Update();AC.HE(aClip,this.Cs,0,B.
sq(this.G,aOffset),At,BP,BP,BP,BP,aBlend);}},Ds:function(Ag,N,B7,ET,Et){var A;var
J=this.Cv;var Ez=null;var Z=AT;var Br=null;var Hr=!!this.Ex&&(!!this.Ex.Hf||!!this.
Ex.B9);if(((A=B.hV(Ag,this.G))[0]>=A[2])||(A[1]>=A[3]))return null;Ag=B.sp(Ag,this.
G.slice(0,2));while(!!J){if(((J.F&0x400)===0x400)&&!Br){Br=J.Ai;while(!!Br&&!((Br.
F&0x200)===0x200))Br=Br.Ai;if(!!Br)Z=B.hV(Ag,Br.GetExtent());else Z=AT;}if(Br===
J){Br=null;Z=AT;}if((((((J.F&0x8)===0x8)&&((J.F&0x10)===0x10))&&!((J.F&0x40000)===
0x40000))&&!((J.F&0x20000)===0x20000))&&(!((J.F&0x10000)===0x10000)||((this.D6.B_===
J)&&!Hr))){var BS=J.GetExtent();var Gg=ET;var Ev=null;if(Gg===J)Gg=null;if(((J.F&
0x400)===0x400)){if(!(((A=B.hV(BS,Z))[0]>=A[2])||(A[1]>=A[3])))Ev=J.Ds(Z,N,B7,Gg
,Et);}else{if(!(((A=B.hV(BS,Ag))[0]>=A[2])||(A[1]>=A[3]))||(ET===J))Ev=J.Ds(Ag,N
,B7,Gg,Et);}J=J.Ai;if(!!Ev){if(!Ez||((Ev.E0<Ez.E0)&&(Ev.E0>=0)))Ez=Ev;if(!Ev.E0)
J=null;}}else J=J.Ai;}return Ez;},Bn:function(DD,Es){var A;var JO=this.F;C.Bg.Bn.
call(this,DD,Es);var D4=this.F^JO;if(!!this.Cx&&((D4&0x40)===0x40)){if(((this.F&
0x40)===0x40))this.Cx.Bn(0x40,0x0);else this.Cx.Bn(0x0,0x40);}if(!!this.D6&&((D4&
0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.D6.B_.F&0x14)===0x14))this.D6.B_.
Bn(0x40,0x0);else this.D6.B_.Bn(0x0,0x40);}if(!!D4){this.F=this.F|0x8000;B.kD([this
,this.Bu],this);}},U:function(E){var A;if(B.sb(E,this.G))return;var D_=[(A=this.
G)[2]-A[0],A[3]-A[1]];var Gs=[E[2]-E[0],E[3]-E[1]];var EE=!B.sa(D_,Gs);if(EE&&!!
this.Cs){this.Cs.GO(Gs);B.pl(this,0);B.pl(this.Cs,0);}C.Bg.U.call(this,E);if((EE&&(
D_[0]>0))&&(D_[1]>0)){var Ab=[].concat(At,D_);var J=this.B9;var Fk=0x14;while(!!
J){if((!J.Au&&(J.DS!==Fk))&&!((J.F&0x400)===0x400))J.Fm(Ab,null);J=J.Ap;}}if(EE){
this.F=this.F|0x5000;B.kD([this,this.Bu],this);}},Hv:function(Ad){var Ik=(C.KeyEvent.
isPrototypeOf(Ad)?Ad:null);var CS=this.Go;if(!Ik)return null;while(!!CS&&(!CS.Da||
!CS.Bb(Ik)))CS=CS.Ap;return CS;},JJ:function(AC,aClip,aOffset,Ax,aBlend){var A;var
J=this.B9;var Ie=AT;var Ir=true;while(!!J){if(((J.F&0x200)===0x200)){var Iq=(C.E$.
isPrototypeOf(J)?J:null);Ie=B.hV(aClip,B.sq(Iq.G,aOffset));Ir=((Iq.F&0x1)===0x1);
}if(((J.F&0x1)===0x1)){if(((J.F&0x400)===0x400)){if(Ir){var Z=B.hV(B.sq(J.GetExtent(
),aOffset),Ie);if(!((Z[0]>=Z[2])||(Z[1]>=Z[3])))J.CL(AC,Z,aOffset,Ax,aBlend);}}else{
var Z=B.hV(B.sq(J.GetExtent(),aOffset),aClip);if(!((Z[0]>=Z[2])||(Z[1]>=Z[3])))J.
CL(AC,Z,aOffset,Ax,aBlend);}}J=J.Ap;}},JQ:function(){var A;var Hk=((this.F&0x1000
)===0x1000);var DI=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];var Dn=false;var Ey=AT;var
Gi=At;var Fl=0;var Gh=0;var J=this.Cv;var Br=null;var Fk=0x14;while(!!J){if(((J.
F&0x800)===0x800)){Dn=true;J.F=J.F&~0x800;}if(Dn&&((J.F&0x200)===0x200)){Dn=false;
if(!!(C.E$.isPrototypeOf(J)?J:null).FP)J.F=J.F|0x1000;}J=J.Ai;}Dn=false;J=this.B9;
if(Hk){this.F=this.F&~0x1000;Hk=!((DI[0]>=DI[2])||(DI[1]>=DI[3]));}this.F=this.F|
0x2000;while(!!J){if(((J.F&0x400)===0x400)){if(!!J.Au&&(J.Au.EB!==Br))J.Au=null;
if((!J.Au&&Dn)&&((J.DS!==Fk)||!!Gh))J.Fm(Ey,Br);}if(!!J.Au){if(Hk&&!((J.F&0x400)===
0x400))J.FF(DI,0);if(Dn&&((J.F&0x400)===0x400)){var AS=J.FF(B.sq(Ey,Gi),Gh);if(((
J.F&0x1)===0x1)){var Cl=At;switch(Gh){case 3:Cl=[AS[0]+Fl,Cl[1]];break;case 4:Cl=[-
AS[0]-Fl,Cl[1]];break;case 1:Cl=[Cl[0],AS[1]+Fl];break;case 2:Cl=[Cl[0],-AS[1]-Fl
];break;default:;}Gi=B.so(Gi,Cl);}}}if(((J.F&0x200)===0x200)){Dn=((J.F&0x1000)===
0x1000);Br=(C.E$.isPrototypeOf(J)?J:null);if(Dn){J.F=J.F&~0x1000;Ey=B.sq(Br.G,Br.
By);Gi=At;Gh=Br.FP;Fl=Br.Space;Dn=!((Ey[0]>=Ey[2])||(Ey[1]>=Ey[3]));}if(Dn){this.
Aq(Br.G);}}J=J.Ap;}this.F=this.F&~0x2000;this.H4([DI[2]-DI[0],DI[3]-DI[1]]);},Bu:
function(BX){var A;var JX=((this.F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000
)){this.F=this.F&~0x4000;this.JQ();}if(((this.F&0x8000)===0x8000)||JX){this.F=this.
F&~0x8000;this.FY(this.F);}},EP:function(E){var A;if(!!E&&(E.K!==this))throw new
Error(Cq);if(!!E&&!((E.F&0x14)===0x14))E=null;if(!!E&&((E.F&0x10000)===0x10000))
E=null;if(E===this.Cx)return;if(!!this.Cx)this.Cx.Bn(0x0,0x60);this.Cx=E;if(!!E){
if(((this.F&0x40)===0x40))E.Bn(0x60,0x0);else E.Bn(0x20,0x0);}},HN:function(C0){
var tmp=this;while(!!tmp){C0=B.sn(C0,tmp.G.slice(0,2));tmp=tmp.K;}return C0;},DispatchEvent:
function(Ad){var A;var J=this.Cx;var Y=(C.Al.isPrototypeOf(J)?J:null);var Am=null;
var Hr=!!this.Ex&&(!!this.Ex.Hf||!!this.Ex.B9);if(!!J&&((((J.F&0x10000)===0x10000
)||((J.F&0x40000)===0x40000))||((J.F&0x20000)===0x20000))){J=null;Y=null;}if(!!this.
D6&&!Hr)Am=this.D6.B_.DispatchEvent(Ad);if(!Am&&!!Y)Am=Y.DispatchEvent(Ad);else if(
!Am&&!!J)Am=J.Bb(Ad);if(!Am)Am=this.Bb(Ad);if(!Am)Am=this.Hv(Ad);return Am;},BroadcastEventAtPosition:
function(Ad,H9,aFilter){var A;var J=this.Cv;var Am=null;while(!!J&&!Am){if((!aFilter||((
A=aFilter)&&((J.F&A)===A)))&&B.se(J.GetExtent(),H9)){var Y=(C.Al.isPrototypeOf(J
)?J:null);if(!!Y)Am=Y.BroadcastEventAtPosition(Ad,B.sn(H9,Y.G.slice(0,2)),aFilter
);else Am=J.Bb(Ad);}J=J.Ai;}if(!Am)Am=this.Bb(Ad);return Am;},BroadcastEvent:function(
Ad,aFilter){var A;var J=this.Cv;var Am=null;while(!!J&&!Am){if(!aFilter||((A=aFilter
)&&((J.F&A)===A))){var Y=(C.Al.isPrototypeOf(J)?J:null);if(!!Y)Am=Y.BroadcastEvent(
Ad,aFilter);else Am=J.Bb(Ad);}J=J.Ai;}if(!Am)Am=this.Bb(Ad);if(!Am)Am=this.Hv(Ad
);return Am;},H4:function(aSize){},FY:function(G_){},GI:function(){this.F=this.F|
0x8000;B.kD([this,this.Bu],this);},Aq:function(Ag){var A;var Y=this;while(!!Y&&!((
Ag[0]>=Ag[2])||(Ag[1]>=Ag[3]))){var EU=Y.Cs;if(!Y.K&&(Y!==this)){Y.Aq(Ag);return;
}if(!!EU){if(((A=EU.CZ)[0]>=A[2])||(A[1]>=A[3])){B.pl(Y,0);B.pl(EU,0);}var Hj=false;
if(Hj)EU.CZ=[0,0,(A=Y.G)[2]-A[0],A[3]-A[1]];else EU.CZ=B.pA(EU.CZ,Ag);}if(!((Y.F&
0x1)===0x1))return;Ag=B.hV(B.sq(Ag,Y.G.slice(0,2)),Y.G);Y=Y.K;}},Cc:function(aArg
){this.GI();},HK:function(M,aFilter){var A;if(!M||(M.K!==this))return null;var D9=
M.Ap;var D$=M.Ai;var Fr=0x10000;if(((aFilter&0x10000)===0x10000))Fr=0x0;while(!!
D9||!!D$){if((!!D9&&(!aFilter||((A=aFilter)&&((D9.F&A)===A))))&&(!Fr||!((A=Fr)&&((
D9.F&A)===A))))return D9;if((!!D$&&(!aFilter||((A=aFilter)&&((D$.F&A)===A))))&&(
!Fr||!((A=Fr)&&((D$.F&A)===A))))return D$;if(!!D9)D9=D9.Ap;if(!!D$)D$=D$.Ai;}return null;
},GZ:function(M,Bq){var A;if(!M)throw new Error(DW);if(M.K!==this)throw new Error(
DX);var DZ=M;var D1=M;while((Bq>0)&&!!DZ.Ap){DZ=DZ.Ap;Bq=Bq-1;}while((Bq<0)&&!!D1.
Ai){D1=D1.Ai;Bq=Bq+1;}if((DZ===M)&&(D1===M))return;if(((M.F&0x401)===0x401)){if(
!!M.Ai&&!!M.Au)M.Ai.F=M.Ai.F|0x800;M.F=M.F|0x800;this.F=this.F|0x4000;B.kD([this
,this.Bu],this);}if(((M.F&0x200)===0x200)){if(!!M.Ai)M.Ai.F=M.Ai.F|0x800;M.F=M.F|
0x800;this.F=this.F|0x4000;B.kD([this,this.Bu],this);}if(!!M.Ai)M.Ai.Ap=M.Ap;if(
!!M.Ap)M.Ap.Ai=M.Ai;if(this.B9===M)this.B9=M.Ap;if(this.Cv===M)this.Cv=M.Ai;if(DZ
!==M){M.Ap=DZ.Ap;M.Ai=DZ;DZ.Ap=M;if(!!M.Ap)M.Ap.Ai=M;}if(D1!==M){M.Ap=D1;M.Ai=D1.
Ai;D1.Ai=M;if(!!M.Ai)M.Ai.Ap=M;}if(!M.Ap)this.Cv=M;if(!M.Ai)this.B9=M;if(((M.F&0x1
)===0x1))this.Aq(M.GetExtent());},I4:function(M){var A;if(!M)throw new Error(Eo);
if(M.K!==this)throw new Error(DX);if((((M.F&0x401)===0x401)&&!!M.Ai)&&!!M.Au){M.
Ai.F=M.Ai.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bu],this);}if(((M.F&0x200
)===0x200)){if(!!M.Ai)M.Ai.F=M.Ai.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bu
],this);}M.Au=null;if(this.Cx===M)this.EP(this.HK(M,0x14));if(!!M.Ai)M.Ai.Ap=M.Ap;
if(!!M.Ap)M.Ap.Ai=M.Ai;if(this.B9===M)this.B9=M.Ap;if(this.Cv===M)this.Cv=M.Ai;M.
K=null;M.Ap=null;M.Ai=null;if(((M.F&0x1)===0x1))this.Aq(M.GetExtent());},Ak:function(
M,Bq){var A;if(!M)throw new Error(Ep);if(!!M.K)throw new Error(G4);M.K=this;M.Ai=
this.Cv;if(!!this.Cv)this.Cv.Ap=M;this.Cv=M;if(!this.B9)this.B9=M;if(!!Bq)this.GZ(
M,Bq);if(((M.F&0x1)===0x1))this.Aq(M.GetExtent());if(((!this.Cx&&((M.F&0x4)===0x4
))&&((M.F&0x10)===0x10))&&!((M.F&0x10000)===0x10000))this.EP(M);if(((M.F&0x401)===
0x401)){M.F=M.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bu],this);}if(((M.F&0x200
)===0x200)){M.F=M.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bu],this);}},_Init:
function(aArg){C.Bg._Init.call(this,aArg);this.__proto__=C.Al;this.F=0x1F;this.Cc(
aArg);},_Mark:function(D){var A;C.Bg._Mark.call(this,D);if((A=this.B9)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Cv)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Go)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cs)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.D6)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ex)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.Cx)&&(A._cycle!=D))A._Mark(A._cycle=D);
},_className:"Core::Group"};C.Root={BV:null,DK:null,C1:null,Av:B.sr(10,null,null
),EV:null,Bz:null,Dk:null,FC:0,Hb:0,Ay:0,A2:B.sr(10,0,null),Gf:B.sr(10,B.pn,null
),CD:B.sr(10,0,null),Dj:B.sr(10,B.pm,null),EX:B.sr(10,0,null),DE:B.sr(10,B.pm,null
),CC:B.sr(10,B.pm,null),B8:B.sr(10,B.pm,null),C2:B.sr(10,B.pm,null),Ah:0,Gk:0,Gj:
0,Gq:B.sr(3,B.pn,null),Im:0,Bt:B.sr(4,0,null),AX:B.sr(4,B.pn,null),AR:0,DP:8,II:
250,D7:0,DJ:0,Hl:true,Gp:false,EJ:function(){return this;},CL:function(AC,aClip,
aOffset,Ax,aBlend){var fullScreenUpdate=false;fullScreenUpdate=B.i6;if(!fullScreenUpdate
)AC.GH(aClip,B.sq(B.sq(aClip,aOffset),this.G.slice(0,2)),0x00000000,0x00000000,0x00000000
,0x00000000,false);C.Al.CL.call(this,AC,aClip,aOffset,Ax,aBlend);},Bn:function(DD
,Es){var A;C.Al.Bn.call(this,DD,Es);if(!this.K&&(((DD&0x1)===0x1)||((Es&0x1)===0x1
)))this.Aq([0,0,(A=this.G)[2]-A[0],A[3]-A[1]]);if(!this.K&&(((DD&0x2)===0x2)||((
Es&0x2)===0x2)))this.Aq([0,0,(A=this.G)[2]-A[0],A[3]-A[1]]);},EP:function(E){if((
E!==this.Bz)||!E)C.Al.EP.call(this,E);},DispatchEvent:function(Ad){if((this.Hb>0
)&&!!(C.KeyEvent.isPrototypeOf(Ad)?Ad:null))return null;if(!!Ad){Ad.E3=!!this.Ay;
if(!!this.Ay)Ad.AH=this.Ay;}var Am=null;if(!!this.Bz){Am=this.Bz.DispatchEvent(Ad
);if(!!Am){this.Ay=0;return Am;}}if(!!this.DK){Am=this.DK.B_.DispatchEvent(Ad);if(
!Am)Am=this.Bb(Ad);if(!Am)Am=this.Hv(Ad);this.Ay=0;return Am;}Am=C.Al.DispatchEvent.
call(this,Ad);this.Ay=0;return Am;},BroadcastEvent:function(Ad,aFilter){if(!!Ad){
Ad.E3=!!this.Ay;if(!!this.Ay)Ad.AH=this.Ay;}var Am=C.Al.BroadcastEvent.call(this
,Ad,aFilter);this.Ay=0;return Am;},Aq:function(Ag){var A;if(this.FC>0)throw new Error(
G5);if(!!this.Cs&&!this.K){if(((A=this.Cs.CZ)[0]>=A[2])||(A[1]>=A[3])){B.pl(this
,0);B.pl(this.Cs,0);}var Hj=false;if(Hj)this.Cs.CZ=[0,0,(A=this.G)[2]-A[0],A[3]-
A[1]];else this.Cs.CZ=B.pA(this.Cs.CZ,Ag);}var fullScreenUpdate=false;fullScreenUpdate=
B.i6;if(fullScreenUpdate)Ag=[0,0,(A=this.G)[2]-A[0],A[3]-A[1]];if(!!this.K){C.Al.
Aq.call(this,Ag);return;}Ag=B.hV(B.sq(Ag,this.G.slice(0,2)),this.G);if((Ag[0]>=Ag[
2])||(Ag[1]>=Ag[3]))return;var I;for(I=0;I<this.AR;I=I+1)if(!(((A=B.hV(this.AX.Get(
I),Ag))[0]>=A[2])||(A[1]>=A[3]))){this.AX.Set(I,B.pA(this.AX.Get(I),Ag));this.Bt.
Set(I,B.rY(this.AX.Get(I)));return;}if(this.AR<3){this.AX.Set(this.AR,Ag);this.Bt.
Set(this.AR,B.rY(Ag));this.AR=this.AR+1;return;}var Az;var BU;var Fo=0;var Fp=0;
var Ib=2147483647;this.AX.Set(this.AR,Ag);this.Bt.Set(this.AR,B.rY(Ag));for(Az=0;
Az<=this.AR;Az=Az+1)for(BU=Az+1;BU<=this.AR;BU=BU+1){var GA=B.rY(B.pA(this.AX.Get(
Az),this.AX.Get(BU)));var Is=((GA<<8)/(this.Bt.Get(Az)+this.Bt.Get(BU)))|0;if(Is<
Ib){Ib=Is;Fo=Az;Fp=BU;}}this.AX.Set(Fo,B.pA(this.AX.Get(Fo),this.AX.Get(Fp)));this.
Bt.Set(Fo,B.rY(this.AX.Get(Fo)));if(Fp!==this.AR){this.AX.Set(Fp,this.AX.Get(this.
AR));this.Bt.Set(Fp,this.Bt.Get(this.AR));}},GZ:function(M,Bq){if(!M)throw new Error(
DW);if(((Bq>0)&&!!this.Bz)&&(this.Bz.K===this)){var J=M;var Gt=Bq;while((J.Ap!==
this.Bz)&&(Gt>0)){J=J.Ap;Gt=Gt-1;}Bq=Bq-Gt;}C.Al.GZ.call(this,M,Bq);},Ak:function(
M,Bq){if(((Bq>=0)&&!!this.Bz)&&(this.Bz.K===this))Bq=-1;C.Al.Ak.call(this,M,Bq);
},JH:function(){var Bh=B._NewObject(C.FM,0);Bh.E3=!!this.Ay;if(!!this.Ay)Bh.AH=this.
Ay;return Bh;},Eu:function(){var Bh=B._NewObject(C.FJ,0);Bh.E3=!!this.Ay;if(!!this.
Ay)Bh.AH=this.Ay;return Bh;},EW:function(){var Bh=B._NewObject(C.GD,0);Bh.E3=!!this.
Ay;if(!!this.Ay)Bh.AH=this.Ay;return Bh;},JI:function(BX){var I;var Ez=false;for(
I=0;I<10;I=I+1)if(!!this.Av.Get(I)){var AW=this.B8.Get(I);var Y=this.Av.Get(I).K;
while(!!Y&&(Y!==this)){AW=B.sn(AW,Y.G.slice(0,2));Y=Y.K;}if(!Y&&(this.Av.Get(I)!==
this)){var tmp=this.Av.Get(I);this.Ah=I;this.Av.Set(I,null);tmp.Bb(this.Eu().InitializeUp(
I,this.DE.Get(I),this.Dj.Get(I),this.CD.Get(I),this.A2.Get(I)+1,this.CC.Get(I),false
,this.B8.Get(I),this.C2.Get(I)));if(tmp===this.C1)this.C1=null;this.BroadcastEvent(
this.EW().InitializeUp(I,this.A2.Get(I)+1,false,tmp,this.B8.Get(I)),0x18);}else{
this.CD.Set(I,(this.Dk.AH-this.EX.Get(I))|0);if(this.CD.Get(I)<10)this.CD.Set(I,
10);this.Ah=I;this.Av.Get(I).Bb(this.Eu().InitializeHold(I,AW,this.Dj.Get(I),this.
CD.Get(I),this.A2.Get(I)+1,this.CC.Get(I),this.B8.Get(I),this.C2.Get(I)));Ez=true;
}}if(!Ez)this.Dk.B5(false);},GetFPS:function(){var ticksCount=0;var Ij=0;ticksCount=((
new Date).getTime()-B.pk)|0;if(!!this.Gk&&(ticksCount>this.Gk))Ij=((this.Gj*1000
)/((ticksCount-this.Gk)|0))|0;this.Gj=0;this.Gk=ticksCount;return Ij;},Update:function(
){var A;if(!this.EV){this.EV=B._NewObject(B.Graphics.Canvas,0);this.EV.GO([(A=this.
G)[2]-A[0],A[3]-A[1]]);}this.EV.Update();return this.UpdateGE20(this.EV);},UpdateGE20:
function(AC){if(!this.BeginUpdate())return AT;var Do=this.UpdateCanvas(AC,At);this.
EndUpdate();return Do;},EndUpdate:function(){if(this.AR>0){this.Gj=this.Gj+1;this.
AR=0;}},UpdateCanvas:function(AC,aOffset){var A;var Do=AT;var JF=[].concat(aOffset
,B.so(AC.FrameSize,aOffset));var I;var Az=this.AR;this.FC=this.FC+1;for(I=0;(I<Az
)&&(I<4);I=I+1){if(this.Bt.Get(I)>0){this.CL(AC,B.sp(this.AX.Get(I),aOffset),[-aOffset[
0],-aOffset[1]],255,true);Do=B.pA(Do,B.hV(JF,this.AX.Get(I)));}else Az=Az+1;}this.
FC=this.FC-1;if(!((Do[0]>=Do[2])||(Do[1]>=Do[3])))return B.sp(Do,aOffset);else return Do;
},GetUpdateRegion:function(F$){var I;var Az=this.AR;if(F$<0)return AT;for(I=0;(I<
Az)&&(I<4);I=I+1){if(!this.Bt.Get(I)){Az=Az+1;F$=F$+1;}else if(I===F$)return this.
AX.Get(I);}return AT;},BeginUpdate:function(){var JP=true;var fullScreenUpdate=false;
var I;if((!JP&&!fullScreenUpdate)&&(this.AR>0)){var Ix=B.sr(3,B.pn,null);var HA=
this.AR;for(I=0;I<HA;I=I+1)Ix.Set(I,this.AX.Get(I));for(I=0;I<this.Im;I=I+1)this.
Aq(this.Gq.Get(I));for(I=0;I<HA;I=I+1)this.Gq.Set(I,Ix.Get(I));this.Im=HA;}var Az;
var BU;for(Az=0;Az<(this.AR-1);Az=Az+1)if(this.Bt.Get(Az)>0)for(BU=Az+1;BU<this.
AR;BU=BU+1)if(this.Bt.Get(BU)>0){var GA=B.rY(B.pA(this.AX.Get(Az),this.AX.Get(BU
)));if(((GA-this.Bt.Get(Az))-this.Bt.Get(BU))<0){this.AX.Set(Az,B.pA(this.AX.Get(
Az),this.AX.Get(BU)));this.Bt.Set(Az,GA);this.Bt.Set(BU,0);}}for(I=this.AR-1;I>=
0;I=I-1)if(!this.Bt.Get(I))this.AR=this.AR-1;return this.AR;},DoesNeedUpdate:function(
){if(this.AR>0)return true;return false;},Initialize:function(aSize){this.U([].concat(
At,aSize));if(this.Hl)this.F=this.F|0x60;else this.F=this.F|0x20;this.Aq(this.G);
return this;},SetRootFocus:function(G8){if(G8===this.Hl)return false;this.Hl=G8;
if(!G8){if(!!this.Bz)this.Bz.Bn(0x0,0x40);if(!!this.DK)this.DK.B_.Bn(0x0,0x40);else
this.Bn(0x0,0x40);}else{if(!!this.DK)this.DK.B_.Bn(0x40,0x0);else this.Bn(0x40,0x0
);if(!!this.Bz)this.Bz.Bn(0x40,0x0);}return true;},SetUserInputTimestamp:function(
JE){this.Ay=JE;},DriveKeyboardHitting:function(AN,Er,B6){var A;var Hw=!!this.BV;
if(!!this.BV&&((!B6||(this.D7!==AN))||(this.DJ!==Er))){var Bh=null;var J=(C.BN.isPrototypeOf(
A=this.BV)?A:null);var CS=(C.DR.isPrototypeOf(A=this.BV)?A:null);if(!!this.D7)Bh=
B._NewObject(C.KeyEvent,0).Initialize(this.D7,false);if(this.DJ!==0x00)Bh=B._NewObject(
C.KeyEvent,0).Initialize2(this.DJ,false);if(!!CS)CS.Bb(Bh);else if(!!J)J.Bb(Bh);
this.D7=0;this.DJ=0x00;this.BV=null;}if(!!this.BV){var Bh=null;var J=(C.BN.isPrototypeOf(
A=this.BV)?A:null);var CS=(C.DR.isPrototypeOf(A=this.BV)?A:null);if(!!AN)Bh=B._NewObject(
C.KeyEvent,0).Initialize(AN,true);if(this.DJ!==0x00)Bh=B._NewObject(C.KeyEvent,0
).Initialize2(Er,true);if(!!CS)CS.Bb(Bh);else if(!!J)J.Bb(Bh);}if(this.Gp&&((!B6||(
this.D7!==AN))||(this.DJ!==Er))){this.D7=0;this.DJ=0x00;this.Gp=false;}if((!this.
BV&&B6)&&(this.Hb>0)){this.D7=AN;this.DJ=Er;this.Gp=true;}if((!this.BV&&B6)&&!this.
Gp){if(!!AN)this.BV=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize(AN,
true));if(Er!==0x00)this.BV=this.DispatchEvent(B._NewObject(C.KeyEvent,0).Initialize2(
Er,true));if(!(C.DR.isPrototypeOf(A=this.BV)?A:null)&&!(C.BN.isPrototypeOf(A=this.
BV)?A:null))this.BV=null;this.D7=AN;this.DJ=Er;Hw=Hw||!!this.BV;}this.Ay=0;return Hw;
},DriveCursorMovement:function(AO){return this.DriveMultiTouchMovement(this.Ah,AO
);},DriveMultiTouchMovement:function(N,AO){if((N<0)||(N>9)){this.Ay=0;return false;
}var Bs=B.sn(AO,this.B8.Get(N));this.B8.Set(N,AO);if(!this.Av.Get(N)||B.sa(Bs,At
)){this.Ay=0;return false;}var AW=AO;var Y=this.Av.Get(N).K;while(!!Y&&(Y!==this
)){AW=B.sn(AW,Y.G.slice(0,2));Y=Y.K;}if(!Y&&(this.Av.Get(N)!==this)){var tmp=this.
Av.Get(N);this.Ah=N;this.Av.Set(N,null);tmp.Bb(this.Eu().InitializeUp(N,this.DE.
Get(N),this.Dj.Get(N),this.CD.Get(N),this.A2.Get(N)+1,this.CC.Get(N),false,this.
B8.Get(N),this.C2.Get(N)));if(tmp===this.C1)this.C1=null;this.BroadcastEvent(this.
EW().InitializeUp(N,this.A2.Get(N)+1,false,tmp,AO),0x18);}else{this.DE.Set(N,AW);
this.Ah=N;this.Av.Get(N).Bb(this.JH().Initialize(N,AW,this.Dj.Get(N),Bs,this.CD.
Get(N),this.A2.Get(N)+1,this.CC.Get(N),AO,this.C2.Get(N)));}this.Ay=0;return true;
},DriveCursorHitting:function(B6,N,AO){return this.DriveMultiTouchHitting(B6,N,AO
);},DriveMultiTouchHitting:function(B6,N,AO){var A;if((N<0)||(N>9)){this.Ay=0;return false;
}var ticksCount=this.Ay;var Ew=[].concat([-this.DP,-this.DP],[this.DP+1,this.DP+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.pk)|0;}var JW=this.Ay;this.
DriveMultiTouchMovement(N,AO);AO=this.B8.Get(N);this.Ay=JW;if(B6)this.C2.Set(N,AO
);if((B6&&!this.Av.Get(N))&&!this.Hb){var BT=null;var AW=AO;if(B.se(this.Gf.Get(
N),AO)&&((ticksCount-this.EX.Get(N))<=(((A=this.II)<0)?A+0x100000000:A)))this.A2.
Set(N,this.A2.Get(N)+1);else this.A2.Set(N,0);this.Gf.Set(N,B.sq(Ew,AO));this.EX.
Set(N,ticksCount);if((!!this.Bz&&!!this.Bz.K)&&((this.Bz.F&0x18)===0x18)){var Z=
B.sq(Ew,this.Bz.K.HN(AO));BT=this.Bz.Ds(Z,N,this.A2.Get(N)+1,null,0x0);}if(!BT){
if(!!this.C1&&!!this.C1.K){if(((this.C1.F&0x8)===0x8)&&((this.C1.F&0x10)===0x10)
){var Z=B.sq(Ew,this.C1.K.HN(AO));BT=this.C1.Ds(Z,N,this.A2.Get(N)+1,null,0x0);}
}else if(!!this.DK)BT=this.Ds(B.sq(Ew,AO),N,this.A2.Get(N)+1,this.DK.B_,0x0);else
BT=this.Ds(B.sq(Ew,AO),N,this.A2.Get(N)+1,null,0x0);}if(!!BT){this.BroadcastEvent(
this.EW().InitializeDown(N,this.A2.Get(N)+1,false,BT.BN,AO),0x18);this.Av.Set(N,
BT.BN);this.CC.Set(N,BT.Dy);}else{this.Av.Set(N,null);this.CC.Set(N,At);this.Ay=
0;return false;}var Y=BT.BN.K;while(!!Y&&(Y!==this)){AW=B.sn(AW,Y.G.slice(0,2));
Y=Y.K;}this.Dj.Set(N,AW);this.DE.Set(N,AW);this.CD.Set(N,0);this.Dk.B5(true);this.
Ah=N;this.Av.Get(N).Bb(this.Eu().InitializeDown(N,AW,this.A2.Get(N)+1,this.CC.Get(
N),false,AO));this.Ay=0;return true;}if(!B6&&!!this.Av.Get(N)){var AW=AO;var Y=this.
Av.Get(N).K;while(!!Y&&(Y!==this)){AW=B.sn(AW,Y.G.slice(0,2));Y=Y.K;}if(!Y)AW=this.
DE.Get(N);this.Ah=N;var tmp=this.Av.Get(N);this.Av.Set(N,null);tmp.Bb(this.Eu().
InitializeUp(N,AW,this.Dj.Get(N),this.CD.Get(N),this.A2.Get(N)+1,this.CC.Get(N),
false,AO,this.C2.Get(N)));this.BroadcastEvent(this.EW().InitializeUp(N,this.A2.Get(
N)+1,false,tmp,AO),0x18);this.Ay=0;return true;}this.Ay=0;return false;},Fc:function(
Dh,H8,Et){var Ew=[].concat([-this.DP,-this.DP],[this.DP+1,this.DP+1]);if(Dh===this
)Dh=null;if(!this.Av.Get(this.Ah))return;var BT;BT=this.Ds(B.sq(Ew,this.B8.Get(this.
Ah)),this.Ah,1,Dh,Et);if(!!BT&&(this.Av.Get(this.Ah)!==BT.BN))this.HF(BT.BN,BT.Dy
);if(!BT&&(this.Av.Get(this.Ah)!==H8))this.HF(H8,At);},HF:function(Dh,Cz){if(!this.
Av.Get(this.Ah)||(this.Av.Get(this.Ah)===Dh))return;var tmp=this.Av.Get(this.Ah);
this.Av.Set(this.Ah,null);tmp.Bb(this.Eu().InitializeUp(this.Ah,this.DE.Get(this.
Ah),this.Dj.Get(this.Ah),this.CD.Get(this.Ah),this.A2.Get(this.Ah)+1,this.CC.Get(
this.Ah),true,this.B8.Get(this.Ah),this.C2.Get(this.Ah)));this.BroadcastEvent(this.
EW().InitializeUp(this.Ah,this.A2.Get(this.Ah)+1,true,tmp,this.B8.Get(this.Ah)),
0x18);var AW=this.B8.Get(this.Ah);var Y=null;if(!!Dh)Y=Dh.K;while(!!Y&&(Y!==this
)){AW=B.sn(AW,Y.G.slice(0,2));Y=Y.K;}if(!Y&&(Dh!==this)){this.Av.Set(this.Ah,null
);this.Dk.B5(false);return;}this.BroadcastEvent(this.EW().InitializeDown(this.Ah
,this.A2.Get(this.Ah)+1,true,Dh,this.B8.Get(this.Ah)),0x18);var ticksCount=0;ticksCount=((
new Date).getTime()-B.pk)|0;this.Av.Set(this.Ah,Dh);this.CC.Set(this.Ah,Cz);this.
Dj.Set(this.Ah,AW);this.DE.Set(this.Ah,AW);this.A2.Set(this.Ah,0);this.CD.Set(this.
Ah,0);this.EX.Set(this.Ah,ticksCount);this.C2.Set(this.Ah,this.B8.Get(this.Ah));
this.Av.Get(this.Ah).Bb(this.Eu().InitializeDown(this.Ah,AW,this.A2.Get(this.Ah)+
1,this.CC.Get(this.Ah),true,this.C2.Get(this.Ah)));},_Init:function(aArg){C.Al._Init.
call(this,aArg);C.Timer._Init.call(this.Dk={T:this},0);(this.Av=[]).__proto__=C.
Root.Av;(this.A2=[]).__proto__=C.Root.A2;(this.Gf=[]).__proto__=C.Root.Gf;(this.
CD=[]).__proto__=C.Root.CD;(this.Dj=[]).__proto__=C.Root.Dj;(this.EX=[]).__proto__=
C.Root.EX;(this.DE=[]).__proto__=C.Root.DE;(this.CC=[]).__proto__=C.Root.CC;(this.
B8=[]).__proto__=C.Root.B8;(this.C2=[]).__proto__=C.Root.C2;(this.Gq=[]).__proto__=
C.Root.Gq;(this.Bt=[]).__proto__=C.Root.Bt;(this.AX=[]).__proto__=C.Root.AX;this.
__proto__=C.Root;this.F=0x7F;this.Dk.E8(50);this.Dk.EQ=[this,this.JI];},_Done:function(
){this.__proto__=C.Root;this.Dk._Done();C.Al._Done.call(this);},_ReInit:function(
){C.Al._ReInit.call(this);this.Dk._ReInit();},_Mark:function(D){var A;C.Al._Mark.
call(this,D);if((A=this.BV)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DK)&&(
A._cycle!=D))A._Mark(A._cycle=D);if((A=this.C1)&&(A._cycle!=D))A._Mark(A._cycle=
D);B.sj(this.Av,D);if((A=this.EV)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Bz)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Dk)._cycle!=D)A._Mark(A._cycle=
D);},_className:"Core::Root"};C.Event={AH:0,E3:false,FQ:function(){var ticksCount=
0;ticksCount=((new Date).getTime()-B.pk)|0;return ticksCount;},Cc:function(aArg){
this.AH=this.FQ();},_Init:function(aArg){this.__proto__=C.Event;this.Cc(aArg);B.
gb++;},_Done:function(){this.__proto__=C.Event;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:
0,_observers:null,_className:"Core::Event"};C.KeyEvent={AQ:0,An:0,Down:false,Initialize2:
function(AN,B6){this.AQ=0;this.An=AN;this.Down=B6;if((AN>=0x30)&&(AN<=0x39))this.
AQ=(10+AN)-48;if((AN>=0x41)&&(AN<=0x5A))this.AQ=(105+AN)-65;if((AN>=0x61)&&(AN<=
0x7A))this.AQ=(105+AN)-97;if(AN===0x20)this.AQ=131;if(!this.AQ)switch(AN){case 0x2B:
this.AQ=132;break;case 0x2D:this.AQ=133;break;case 0x2A:this.AQ=134;break;case 0x2F:
this.AQ=135;break;case 0x3D:this.AQ=136;break;case 0x2E:this.AQ=137;break;case 0x2C:
this.AQ=138;break;case 0x3A:this.AQ=139;break;case 0x3B:this.AQ=140;break;default:;
}return this;},Initialize:function(AN,B6){this.AQ=AN;this.Down=B6;this.An=0x00;var
Hd=AN-10;var Hc=AN-105;if((Hd>=0)&&(Hd<=9))this.An=(48+Hd)&0xFFFF;if((Hc>=0)&&(Hc<=
25))this.An=(65+Hc)&0xFFFF;if(AN===131)this.An=0x20;if(this.An===0x00)switch(AN){
case 132:this.An=0x2B;break;case 133:this.An=0x2D;break;case 134:this.An=0x2A;break;
case 135:this.An=0x2F;break;case 136:this.An=0x3D;break;case 137:this.An=0x2E;break;
case 138:this.An=0x2C;break;case 139:this.An=0x3A;break;case 140:this.An=0x3B;break;
default:;}return this;},IP:function(H7){switch(H7){case 141:return((this.An>=0x41
)&&(this.An<=0x5A))||((this.An>=0x61)&&(this.An<=0x7A));case 142:return(((this.An>=
0x41)&&(this.An<=0x5A))||((this.An>=0x61)&&(this.An<=0x7A)))||((this.An>=0x30)&&(
this.An<=0x39));case 143:return(this.An>=0x30)&&(this.An<=0x39);case 144:return(((
this.An>=0x41)&&(this.An<=0x46))||((this.An>=0x61)&&(this.An<=0x66)))||((this.An>=
0x30)&&(this.An<=0x39));case 145:return this.An!==0x00;case 146:return(this.An===
0x00)&&!!this.AQ;case 147:return(((this.AQ===6)||(this.AQ===7))||(this.AQ===4))||(
this.AQ===5);case 148:return(this.An!==0x00)||!!this.AQ;default:;}return H7===this.
AQ;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=C.KeyEvent;
},_className:"Core::KeyEvent"};C.GD={G1:null,CN:B.pm,Cy:0,Cb:0,Down:false,Dr:false
,InitializeUp:function(N,B7,Eq,G$,CR){this.Down=false;this.Cb=N;this.Cy=B7;this.
CN=CR;this.G1=G$;this.Dr=Eq;return this;},InitializeDown:function(N,B7,Eq,G$,CR){
this.Down=true;this.Cb=N;this.Cy=B7;this.CN=CR;this.G1=G$;this.Dr=Eq;return this;
},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=C.GD;},_Mark:
function(D){var A;C.Event._Mark.call(this,D);if((A=this.G1)&&(A._cycle!=D))A._Mark(
A._cycle=D);},_className:"Core::CursorGrabEvent"};C.FJ={Du:B.pm,CN:B.pm,Cy:0,Co:
0,Dc:B.pm,Cw:B.pm,Cb:0,Down:false,Dr:false,InitializeHold:function(N,DY,Ff,Fg,B7
,Cz,CR,Fe){this.Down=true;this.Cb=N;this.Cw=B.so(DY,Cz);this.Dc=B.so(Ff,Cz);this.
Co=Fg;this.Cy=B7;this.CN=CR;this.Du=Fe;return this;},InitializeUp:function(N,DY,
Ff,Fg,B7,Cz,Eq,CR,Fe){this.Down=false;this.Cb=N;this.Cw=B.so(DY,Cz);this.Dc=B.so(
Ff,Cz);this.Co=Fg;this.Cy=B7;this.Dr=Eq;this.CN=CR;this.Du=Fe;return this;},InitializeDown:
function(N,DY,B7,Cz,Eq,CR){this.Down=true;this.Cb=N;this.Cw=B.so(DY,Cz);this.Dc=
B.so(DY,Cz);this.Co=0;this.Cy=B7;this.Dr=Eq;this.CN=CR;this.Du=CR;return this;},
_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=C.FJ;},_className:
"Core::CursorEvent"};C.FM={Du:B.pm,CN:B.pm,Cy:0,Co:0,Dy:B.pm,Dc:B.pm,Cw:B.pm,Cb:
0,Initialize:function(N,DY,Ff,aOffset,Fg,JD,Cz,CR,Fe){this.Cb=N;this.Cw=B.so(DY,
Cz);this.Dc=B.so(Ff,Cz);this.Dy=aOffset;this.Co=Fg;this.Cy=JD;this.CN=CR;this.Du=
Fe;return this;},_Init:function(aArg){C.Event._Init.call(this,aArg);this.__proto__=
C.FM;},_className:"Core::DragEvent"};C.E$={By:B.pm,Space:0,FP:0,CL:function(AC,aClip
,aOffset,Ax,aBlend){},U:function(E){var A;if(B.sb(E,this.G))return;var D_=[(A=this.
G)[2]-A[0],A[3]-A[1]];var Gs=[E[2]-E[0],E[3]-E[1]];var EE=!B.sa(D_,Gs);var Bs=B.
sn(E.slice(0,2),this.G.slice(0,2));if(!B.sa(Bs,At)&&!EE){var J=this.Ap;while(!!J&&
!((J.F&0x200)===0x200)){if(((J.F&0x400)===0x400)){var tmp=((J.F&0x100)===0x100);
J.GL(Bs,tmp);}J=J.Ap;}}if((EE&&(D_[0]>0))&&(D_[1]>0)){var Ab=B.sq(this.G,this.By
);var J=this.Ap;var Fk=0x14;while(!!J&&!((J.F&0x200)===0x200)){if(((J.F&0x400)===
0x400)){if(!!J.Au&&(J.Au.EB!==this))J.Au=null;if(!J.Au&&((J.DS!==Fk)||!!this.FP)
)J.Fm(Ab,this);}J=J.Ap;}}C.Bg.U.call(this,E);if(!!this.K&&EE){this.F=this.F|0x1000;
if(!((this.K.F&0x2000)===0x2000)){this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bu],
this);}}},_Init:function(aArg){C.Bg._Init.call(this,aArg);this.__proto__=C.E$;this.
F=0x203;},_className:"Core::Outline"};C.BM={GN:null,HV:null,HR:null,E6:null,DU:null
,Dz:null,Ha:0,Cb:0,AH:0,Cy:0,Co:0,Dy:B.pm,Dc:B.pm,Cw:B.pm,I5:1000,FX:8,El:0,HM:-
1,IQ:1,IR:1,IK:false,Down:false,EK:false,Dr:false,Ia:false,CL:function(AC,aClip,
aOffset,Ax,aBlend){},Bb:function(Ad){var A;var Bi=(C.FJ.isPrototypeOf(Ad)?Ad:null
);var Ba=(C.FM.isPrototypeOf(Ad)?Ad:null);var Gl=this.EK;if(!Bi&&!Ba)return null;
if(!!Bi){this.Ia=Bi.Down;this.Down=Bi.Down;this.EK=this.GK(Bi.Cw);this.Dc=Bi.Dc;
this.Cw=Bi.Cw;this.Dy=At;this.Co=Bi.Co;this.Cy=Bi.Cy;this.Dr=Bi.Dr;this.Cb=Bi.Cb;
this.Ha=Bi.Cb;this.AH=Bi.AH;if(Bi.Down&&!Bi.Co)Gl=false;}if(!!Ba){this.Down=true;
this.EK=this.GK(Ba.Cw);this.Dc=Ba.Dc;this.Cw=Ba.Cw;this.Dy=Ba.Dy;this.Co=Ba.Co;this.
Cy=Ba.Cy;this.Cb=Ba.Cb;this.Ha=Ba.Cb;this.Dr=false;this.AH=Ba.AH;(A=this.GN)?A[1
].call(A[0],this):null;}if((!!Bi&&this.Down)&&!this.Co)(A=this.Dz)?A[1].call(A[0
],this):null;if((!!Bi&&this.Down)&&(this.Co>0))(A=this.E6)?A[1].call(A[0],this):
null;if((this.Down&&this.EK)&&!Gl)(A=this.HR)?A[1].call(A[0],this):null;if(((this.
Down&&!this.EK)&&Gl)||((!this.Down&&this.EK)&&Gl))(A=this.HV)?A[1].call(A[0],this
):null;if(!!Bi&&!this.Down)(A=this.DU)?A[1].call(A[0],this):null;if(!!this.El){if(((((
this.El&0x10)===0x10)&&!!Bi)&&Bi.Down)&&(Bi.Co>=this.I5))this.EJ().Fc(null,this,
0x10);if((((this.El&0x1)===0x1)&&!!Ba)&&((Ba.CN[1]-Ba.Du[1])<=-this.FX))this.EJ(
).Fc(null,this,0x1);if((((this.El&0x2)===0x2)&&!!Ba)&&((Ba.CN[1]-Ba.Du[1])>=this.
FX))this.EJ().Fc(null,this,0x2);if((((this.El&0x4)===0x4)&&!!Ba)&&((Ba.CN[0]-Ba.
Du[0])<=-this.FX))this.EJ().Fc(null,this,0x4);if((((this.El&0x8)===0x8)&&!!Ba)&&((
Ba.CN[0]-Ba.Du[0])>=this.FX))this.EJ().Fc(null,this,0x8);}return this;},Ds:function(
Ag,N,B7,ET,Et){var A;if(!!ET&&(ET!==this))return null;if((B7<this.IR)||(B7>this.
IQ))return null;if((this.HM>=0)&&(N!==this.HM))return null;if((!this.IK&&this.Ia
)&&(N!==this.Ha))return null;if(!!(Et&this.El))return null;if(this.IN()){var CU=
B.hV(Ag,this.GetExtent());if(!((CU[0]>=CU[2])||(CU[1]>=CU[3]))){var D2=B.rZ(Ag);
var Bs=At;if(D2[0]<CU[0])Bs=[CU[0]-D2[0],Bs[1]];if(D2[0]>=CU[2])Bs=[(CU[2]-D2[0]
)-1,Bs[1]];if(D2[1]<CU[1])Bs=[Bs[0],CU[1]-D2[1]];if(D2[1]>=CU[3])Bs=[Bs[0],(CU[3
]-D2[1])-1];return B._NewObject(C.FK,0).Initialize(this,Bs);}}else{var Aw=B.sr(9
,B.pm,null);var I;Aw.Set(0,B.rZ(Ag));Aw.Set(1,Aw.Get(0));Aw.Set(2,Aw.Get(0));Aw.
Set(3,Aw.Get(0));Aw.Set(4,Aw.Get(0));Aw.Set(1,[Ag[0],Aw.Get(1)[1]]);Aw.Set(2,[Aw.
Get(2)[0],Ag[1]]);Aw.Set(3,[Ag[2],Aw.Get(3)[1]]);Aw.Set(4,[Aw.Get(4)[0],Ag[3]]);
Aw.Set(5,Ag.slice(0,2));Aw.Set(6,[Ag[2],Ag[1]]);Aw.Set(7,[Ag[0],Ag[3]]);Aw.Set(8
,Ag.slice(2,4));for(I=0;I<9;I=I+1)if(this.GK(Aw.Get(I)))return B._NewObject(C.FK
,0).Initialize(this,B.sn(Aw.Get(I),Aw.Get(0)));}return null;},_Init:function(aArg
){C.Fb._Init.call(this,aArg);this.__proto__=C.BM;this.F=0x11B;},_Mark:function(D
){var A;C.Fb._Mark.call(this,D);if((A=this.GN)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);if((A=this.HV)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.HR)&&((A=
A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.E6)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.DU)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Dz)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Core::SimpleTouchHandler"
};C.DR={Ap:null,DU:null,Dz:null,E6:null,ED:0,AH:0,H0:0,FO:148,AQ:0,An:0,Da:true,
Down:false,GY:false,FI:false,Bb:function(Ad){var A;if(!!Ad&&Ad.IP(this.FO)){this.
Down=Ad.Down;this.AQ=Ad.AQ;this.An=Ad.An;this.AH=Ad.AH;this.FI=false;if(Ad.Down){
this.H0=this.ED;this.GY=this.ED>0;if(this.GY)(A=this.E6)?A[1].call(A[0],this):null;
else(A=this.Dz)?A[1].call(A[0],this):null;if(!this.FI)this.ED=this.ED+1;return!this.
FI;}if(!Ad.Down){this.GY=this.ED>1;this.H0=this.ED-1;this.ED=0;(A=this.DU)?A[1].
call(A[0],this):null;return!this.FI;}}return false;},Cc:function(aArg){var A;var
B_=(C.Al.isPrototypeOf(A=this.T)?A:null);if(!B_)throw new Error(G6);this.Ap=B_.Go;
B_.Go=this;},_Init:function(aArg){this.__proto__=C.DR;this.Cc(aArg);B.gb++;},_Done:
function(){this.__proto__=C.DR;B.gb--;},_ReInit:function(){},_Mark:function(D){var
A;if((A=this.Ap)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.DU)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.Dz)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.E6)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.T)&&(A._cycle
!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"
};C.FK={BN:null,E0:0,Dy:B.pm,Initialize:function(M,aOffset){this.BN=M;this.Dy=aOffset;
this.E0=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:function(
aArg){this.__proto__=C.FK;B.gb++;},_Done:function(){this.__proto__=C.FK;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.BN)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:
null,_className:"Core::CursorHit"};C.HO={B_:null,_Init:function(aArg){this.__proto__=
C.HO;B.gb++;},_Done:function(){this.__proto__=C.HO;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.B_)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:null,_className:
"Core::ModalContext"};C.E4={EB:null,BS:B.pn,Ab:B.pn,isEmpty:false,_Init:function(
aArg){this.__proto__=C.E4;B.gb++;},_Done:function(){this.__proto__=C.E4;B.gb--;}
,_ReInit:function(){},_Mark:function(D){var A;if((A=this.EB)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:
null,_className:"Core::LayoutContext"};C.FR={Fw:B.pm,Fv:B.pm,Fu:B.pm,Ft:B.pm,_Init:
function(aArg){C.E4._Init.call(this,aArg);this.__proto__=C.FR;},_className:"Core::LayoutQuadContext"
};C.HG={B_:null,_Init:function(aArg){this.__proto__=C.HG;B.gb++;},_Done:function(
){this.__proto__=C.HG;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.B_)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.T)&&(A._cycle!=D))A._Mark(
A._cycle=D);},T:null,_cycle:0,_observers:null,_className:"Core::DialogContext"};
C.H3={Hf:null,B9:null,_Init:function(aArg){this.__proto__=C.H3;B.gb++;},_Done:function(
){this.__proto__=C.H3;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Hf)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.B9)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:
null,_className:"Core::TaskQueue"};C.H2={_Init:function(aArg){this.__proto__=C.H2;
B.gb++;},_Done:function(){this.__proto__=C.H2;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null,_cycle:
0,_observers:null,_className:"Core::Task"};C.DV={resource:null,CY:function(){this.
resource=null;},Cc:function(aArg){this.resource=aArg;},_Init:function(aArg){this.
__proto__=C.DV;this.Cc(aArg);B.gb++;},_Done:function(){this.__proto__=C.DV;this.
CY();B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.T)&&(A._cycle
!=D))A._Mark(A._cycle=D);},T:null,_cycle:0,_observers:null,_className:"Core::Resource"
};C.Timer={EQ:null,timer:null,AH:0,Period:1000,FG:0,Da:false,CY:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},Gv:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.rz(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},E8:function(E){if(E<0)E=0;if(E===this.Period)return;
this.Period=E;if(this.Da)this.Gv(this.FG,E);},IV:function(E){if(E<0)E=0;if(E===this.
FG)return;this.FG=E;if(this.Da)this.Gv(E,this.Period);},B5:function(E){if(E===this.
Da)return;this.Da=E;if(E)this.Gv(this.FG,this.Period);else this.Gv(0,0);this.AH=
this.FQ();},FQ:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk
)|0;return ticksCount;},Trigger:function(){var A;this.AH=this.FQ();if(!this.Period
)this.B5(false);(A=this.EQ)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=C.Timer;B.gb++;},_Done:function(){this.__proto__=C.Timer;this.CY();B.gb--;
},_ReInit:function(){},_Mark:function(D){var A;if((A=this.EQ)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null
,_cycle:0,_observers:null,_className:"Core::Timer"};C.La={Lb:0x1,J5:0x2,Kv:0x4,K8:
0x8,Da:0x10,K1:0x20,Kw:0x40,KF:0x80,Ku:0x100,KB:0x200,Ko:0x400,KP:0x800,H4:0x1000
,K$:0x2000,KK:0x4000,KL:0x8000,Kh:0x10000,KJ:0x20000,KY:0x40000};C.DS={KQ:0x1,KR:
0x2,J2:0x4,J3:0x8,J4:0x10,J1:0x20};C.FP={KG:0,K7:1,J_:2,KC:3,KT:4};C.KeyCode={NoKey:
0,Ok:1,Exit:2,Menu:3,Up:4,Down:5,Left:6,Right:7,PageUp:8,PageDown:9,Key0:10,Key1:
11,Key2:12,Key3:13,Key4:14,Key5:15,Key6:16,Key7:17,Key8:18,Key9:19,Red:20,Green:
21,Blue:22,Yellow:23,White:24,Magenta:25,F1:26,F2:27,F3:28,F4:29,F5:30,F6:31,F7:
32,F8:33,F9:34,F10:35,ChannelUp:36,ChannelDown:37,Display:38,SkipPrev:39,SkipNext:
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
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};C.KS={Lg:0x1,Ld:0x2,Le:0x4,Lf:0x8,KE:
0x10};
C._Init=function(){C.Fb.__proto__=C.BN;C.Bg.__proto__=C.BN;C.Al.__proto__=C.Bg;C.
Root.__proto__=C.Al;C.KeyEvent.__proto__=C.Event;C.GD.__proto__=C.Event;C.FJ.__proto__=
C.Event;C.FM.__proto__=C.Event;C.E$.__proto__=C.Bg;C.BM.__proto__=C.Fb;C.FR.__proto__=
C.E4;};C.BO=function(D){};return C;})();

/* Embedded Wizard */