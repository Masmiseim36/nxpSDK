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
Core=(function(){var B=EmWiApp;var D={};
var Az=[0,0];var A3=[0,0,0,0];var BY="The view does not belong to this group";var
CI="No view to restack";var CJ="View is not in this group";var Du="Sibling view does not belong to the group";
var Dv="No view to remove";var IJ="No view to add";var IK="View already in a group";
var IL="Recursive invalidate during active update cycle.";var G4="The KeyPressHandler is embedded within an object not being derived "+
"from Core::Group.";
D.BX={Ag:null,V:null,K:null,AE:null,F:0x103,Cb:0x14,Gi:function(X,G6){},E5:function(
C){var A;var BI=C^this.Cb;if(!BI)return;this.Cb=C;if(!!this.AE&&!((this.F&0x400)===
0x400)){this.K.F=this.K.F|0x5000;B.kD([A=this.K,A.Bk],this);this.K.An([0,0,(A=this.
K.N)[2]-A[0],A[3]-A[1]]);}if(!!this.AE&&((this.F&0x400)===0x400)){this.AE.EU.F=this.
AE.EU.F|0x1000;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bk],this);}},D4:function(
){var Aa=this.K;while(!!Aa){var Kk=(D.Root.isPrototypeOf(Aa)?Aa:null);if(!!Kk)return Kk;
Aa=Aa.K;}return null;},Ci:function(Ar,aClip,aOffset,AD,aBlend){},Bm:function(Al){
return null;},DE:function(Ao,O,Ce,E$,EK){return null;},GE:function(X,Cx){return Az;
},In:function(aOffset,G5){},GetExtent:function(){return A3;},A1:function(DS,EJ){
var A;if(((this.F&0x200)===0x200))DS=DS&~0x400;var I8=(this.F&~EJ)|DS;var Ef=I8^
this.F;this.F=I8;if(!!this.K&&!!(Ef&0x14)){var Kc=((this.F&0x14)===0x14);if(Kc&&
!this.K.CE)this.K.Ez(this);if(!Kc&&(this.K.CE===this))this.K.Ez(this.K.Jw(this,0x14
));}if(!!this.K&&!!(Ef&0x403))this.K.An(this.GetExtent());if((!!this.AE&&((I8&0x400
)===0x400))&&((Ef&0x1)===0x1)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([
A=this.K,A.Bk],this);}if(!!this.K&&((Ef&0x400)===0x400)){this.AE=null;this.F=this.
F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bk],this);}},_Init:function(aArg
){this.__proto__=D.BX;B.gb++;},_Done:function(){this.__proto__=D.BX;B.gb--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.Ag)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.V)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.K)&&(A._cycle!=E)
)A._Mark(A._cycle=E);if((A=this.AE)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.
Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:null,_cycle:0,_observers:null,_className:
"Core::View"};D.FU={Bp:B.pm,BR:B.pm,Bo:B.pm,BQ:B.pm,Gi:function(X,G6){var Y=B._NewObject(
D.GT,0);this.AE=null;Y.B2=this.GetExtent();Y.Ah=X;Y.EU=G6;Y.Gt=this.BQ;Y.Gu=this.
Bo;Y.Gv=this.BR;Y.Gw=this.Bp;this.AE=Y;},GE:function(X,Cx){var A;var Ay=this.Cb;
var Y=(D.GT.isPrototypeOf(A=this.AE)?A:null);var Q=Y.B2[0];var R=Y.B2[1];var S=Y.
B2[2];var T=Y.B2[3];var B1=[X[2]-X[0],X[3]-X[1]];var At=S-Q;var As=T-R;if(!Cx){var
CV=[(A=Y.Ah)[2]-A[0],A[3]-A[1]];Q=Q-Y.Ah[0];R=R-Y.Ah[1];if(CV[0]!==B1[0]){var A7=((
Ay&0x4)===0x4);var A8=((Ay&0x8)===0x8);var Dk=((Ay&0x1)===0x1);if(!A7&&(Dk||!A8)
)Q=((Q*B1[0])/CV[0])|0;if(!A8&&(Dk||!A7)){S=S-Y.Ah[0];S=((S*B1[0])/CV[0])|0;S=S-
B1[0];}else S=S-Y.Ah[2];Q=Q+X[0];S=S+X[2];if(!Dk){if(A7&&!A8)S=Q+At;else if(!A7&&
A8)Q=S-At;else{Q=Q+((((S-Q)-At)/2)|0);S=Q+At;}}}else{S=S-Y.Ah[2];Q=Q+X[0];S=S+X[
2];}if(CV[1]!==B1[1]){var A9=((Ay&0x10)===0x10);var A6=((Ay&0x20)===0x20);var Dl=((
Ay&0x2)===0x2);if(!A9&&(Dl||!A6))R=((R*B1[1])/CV[1])|0;if(!A6&&(Dl||!A9)){T=T-Y.
Ah[1];T=((T*B1[1])/CV[1])|0;T=T-B1[1];}else T=T-Y.Ah[3];R=R+X[1];T=T+X[3];if(!Dl
){if(A9&&!A6)T=R+As;else if(!A9&&A6)R=T-As;else{R=R+((((T-R)-As)/2)|0);T=R+As;}}
}else{T=T-Y.Ah[3];R=R+X[1];T=T+X[3];}}else{switch(Cx){case 3:{Q=X[0];S=Q+At;}break;
case 4:{S=X[2];Q=S-At;}break;case 1:{R=X[1];T=R+As;}break;case 2:{T=X[3];R=T-As;
}break;default:;}if((Cx===3)||(Cx===4)){var A9=((Ay&0x10)===0x10);var A6=((Ay&0x20
)===0x20);var Dl=((Ay&0x2)===0x2);if(Dl){R=X[1];T=X[3];}else if(A9&&!A6){R=X[1];
T=R+As;}else if(A6&&!A9){T=X[3];R=T-As;}else{R=X[1]+((((X[3]-X[1])-As)/2)|0);T=R+
As;}}if((Cx===1)||(Cx===2)){var A7=((Ay&0x4)===0x4);var A8=((Ay&0x8)===0x8);var Dk=((
Ay&0x1)===0x1);if(Dk){Q=X[0];S=X[2];}else if(A7&&!A8){Q=X[0];S=Q+At;}else if(A8&&
!A7){S=X[2];Q=S-At;}else{Q=X[0]+((((X[2]-X[0])-At)/2)|0);S=Q+At;}}}Y.isEmpty=(Q>=
S)||(R>=T);At=(S-Q)-1;As=(T-R)-1;var DV=Y.B2[0];var DW=Y.B2[1];var DA=(Y.B2[2]-DV
)-1;var Dz=(Y.B2[3]-DW)-1;if(!DA)DA=1;if(!Dz)Dz=1;if(((this.F&0x100)===0x100)){this.
BQ=[Q+((((Y.Gt[0]-DV)*At)/DA)|0),R+((((Y.Gt[1]-DW)*As)/Dz)|0)];this.Bo=[Q+((((Y.
Gu[0]-DV)*At)/DA)|0),R+((((Y.Gu[1]-DW)*As)/Dz)|0)];this.BR=[Q+((((Y.Gv[0]-DV)*At
)/DA)|0),R+((((Y.Gv[1]-DW)*As)/Dz)|0)];this.Bp=[Q+((((Y.Gw[0]-DV)*At)/DA)|0),R+((((
Y.Gw[1]-DW)*As)/Dz)|0)];}else{this.GV([Q+((((Y.Gt[0]-DV)*At)/DA)|0),R+((((Y.Gt[1
]-DW)*As)/Dz)|0)]);this.GW([Q+((((Y.Gu[0]-DV)*At)/DA)|0),R+((((Y.Gu[1]-DW)*As)/Dz
)|0)]);this.GX([Q+((((Y.Gv[0]-DV)*At)/DA)|0),R+((((Y.Gv[1]-DW)*As)/Dz)|0)]);this.
GY([Q+((((Y.Gw[0]-DV)*At)/DA)|0),R+((((Y.Gw[1]-DW)*As)/Dz)|0)]);this.AE=Y;}return[
At+1,As+1];},In:function(aOffset,G5){if(G5){this.BQ=B.so(this.BQ,aOffset);this.Bo=
B.so(this.Bo,aOffset);this.BR=B.so(this.BR,aOffset);this.Bp=B.so(this.Bp,aOffset
);}else{this.GV(B.so(this.BQ,aOffset));this.GW(B.so(this.Bo,aOffset));this.GX(B.
so(this.BR,aOffset));this.GY(B.so(this.Bp,aOffset));}},GetExtent:function(){if(!
!this.AE&&this.AE.isEmpty)return A3;var Q=this.BQ[0];var R=this.BQ[1];var S=this.
BR[0];var T=this.BR[1];if((((this.Bp[0]!==Q)||(this.Bo[1]!==R))||(this.Bo[0]!==S
))||(this.Bp[1]!==T)){if(this.Bo[0]<Q)Q=this.Bo[0];if(this.BR[0]<Q)Q=this.BR[0];
if(this.Bp[0]<Q)Q=this.Bp[0];if(this.Bo[1]<R)R=this.Bo[1];if(this.BR[1]<R)R=this.
BR[1];if(this.Bp[1]<R)R=this.Bp[1];if(this.BQ[0]>S)S=this.BQ[0];if(this.Bo[0]>S)
S=this.Bo[0];if(this.Bp[0]>S)S=this.Bp[0];if(this.BQ[1]>T)T=this.BQ[1];if(this.Bo[
1]>T)T=this.Bo[1];if(this.Bp[1]>T)T=this.Bp[1];}else{var tmp;if(S<Q){tmp=Q;Q=S;S=
tmp;}if(T<R){tmp=R;R=T;T=tmp;}}return[Q,R,S+1,T+1];},GY:function(C){var A;if(B.sa(
C,this.Bp))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.GetExtent());
this.AE=null;this.Bp=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.GetExtent(
));if((!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bk],this);}},GX:function(
C){var A;if(B.sa(C,this.BR))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.
GetExtent());this.AE=null;this.BR=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(
this.GetExtent());if((!!this.K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===
0x2000)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bk],this);
}},GW:function(C){var A;if(B.sa(C,this.Bo))return;if(!!this.K&&((this.F&0x1)===0x1
))this.K.An(this.GetExtent());this.AE=null;this.Bo=C;if(!!this.K&&((this.F&0x1)===
0x1))this.K.An(this.GetExtent());if((!!this.K&&((this.F&0x400)===0x400))&&!((this.
K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;B.kD([A=this.
K,A.Bk],this);}},GV:function(C){var A;if(B.sa(C,this.BQ))return;if(!!this.K&&((this.
F&0x1)===0x1))this.K.An(this.GetExtent());this.AE=null;this.BQ=C;if(!!this.K&&((
this.F&0x1)===0x1))this.K.An(this.GetExtent());if((!!this.K&&((this.F&0x400)===0x400
))&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;this.K.F=this.K.F|0x4000;
B.kD([A=this.K,A.Bk],this);}},H$:function(De){var AI=B.sr(4,B.pm,null);var J=0;var
AM=3;var I2=false;var I3=false;AI.Set(0,this.BQ);AI.Set(1,this.Bo);AI.Set(2,this.
BR);AI.Set(3,this.Bp);while(J<4){var Kp=AI.Get(J)[0];var Hw=AI.Get(J)[1];var MO=
AI.Get(AM)[0];var Jn=AI.Get(AM)[1];if(((Hw>De[1])!==(Jn>De[1]))||((Hw<De[1])!==(
Jn<De[1]))){var Kq=((((MO-Kp)*(De[1]-Hw))/(Jn-Hw))|0)+Kp;if(De[0]>Kq)I2=!I2;if(De[
0]<Kq)I3=!I3;}AM=J;J=J+1;}return I2||I3;},KQ:function(){return((((this.BQ[0]===this.
Bp[0])&&(this.Bo[0]===this.BR[0]))&&(this.BQ[1]===this.Bo[1]))&&(this.BR[1]===this.
Bp[1]))||((((this.BQ[0]===this.Bo[0])&&(this.BR[0]===this.Bp[0]))&&(this.BQ[1]===
this.Bp[1]))&&(this.Bo[1]===this.BR[1]));},_Init:function(aArg){D.BX._Init.call(
this,aArg);this.__proto__=D.FU;},_className:"Core::QuadView"};D.Bb={N:B.pn,Gi:function(
X,G6){var Y=B._NewObject(D.FM,0);Y.B2=this.N;Y.Ah=X;Y.EU=G6;this.AE=Y;},GE:function(
X,Cx){var A;var Ay=this.Cb;var Y=this.AE;var Q=Y.B2[0];var R=Y.B2[1];var S=Y.B2[
2];var T=Y.B2[3];var B1=[X[2]-X[0],X[3]-X[1]];var At=S-Q;var As=T-R;if(!Cx){var CV=[(
A=Y.Ah)[2]-A[0],A[3]-A[1]];Q=Q-Y.Ah[0];R=R-Y.Ah[1];if(CV[0]!==B1[0]){var A7=((Ay&
0x4)===0x4);var A8=((Ay&0x8)===0x8);var Dk=((Ay&0x1)===0x1);if(!A7&&(Dk||!A8))Q=((
Q*B1[0])/CV[0])|0;if(!A8&&(Dk||!A7)){S=S-Y.Ah[0];S=((S*B1[0])/CV[0])|0;S=S-B1[0];
}else S=S-Y.Ah[2];Q=Q+X[0];S=S+X[2];if(!Dk){if(A7&&!A8)S=Q+At;else if(!A7&&A8)Q=
S-At;else{Q=Q+((((S-Q)-At)/2)|0);S=Q+At;}}}else{S=S-Y.Ah[2];Q=Q+X[0];S=S+X[2];}if(
CV[1]!==B1[1]){var A9=((Ay&0x10)===0x10);var A6=((Ay&0x20)===0x20);var Dl=((Ay&0x2
)===0x2);if(!A9&&(Dl||!A6))R=((R*B1[1])/CV[1])|0;if(!A6&&(Dl||!A9)){T=T-Y.Ah[1];
T=((T*B1[1])/CV[1])|0;T=T-B1[1];}else T=T-Y.Ah[3];R=R+X[1];T=T+X[3];if(!Dl){if(A9&&
!A6)T=R+As;else if(!A9&&A6)R=T-As;else{R=R+((((T-R)-As)/2)|0);T=R+As;}}}else{T=T-
Y.Ah[3];R=R+X[1];T=T+X[3];}}else{switch(Cx){case 3:{Q=X[0];S=Q+At;}break;case 4:{
S=X[2];Q=S-At;}break;case 1:{R=X[1];T=R+As;}break;case 2:{T=X[3];R=T-As;}break;default:;
}if((Cx===3)||(Cx===4)){var A9=((Ay&0x10)===0x10);var A6=((Ay&0x20)===0x20);var Dl=((
Ay&0x2)===0x2);if(Dl){R=X[1];T=X[3];}else if(A9&&!A6){R=X[1];T=R+As;}else if(A6&&
!A9){T=X[3];R=T-As;}else{R=X[1]+((((X[3]-X[1])-As)/2)|0);T=R+As;}}if((Cx===1)||(
Cx===2)){var A7=((Ay&0x4)===0x4);var A8=((Ay&0x8)===0x8);var Dk=((Ay&0x1)===0x1);
if(Dk){Q=X[0];S=X[2];}else if(A7&&!A8){Q=X[0];S=Q+At;}else if(A8&&!A7){S=X[2];Q=
S-At;}else{Q=X[0]+((((X[2]-X[0])-At)/2)|0);S=Q+At;}}}Y.isEmpty=(Q>=S)||(R>=T);if(((
this.F&0x100)===0x100)){this.N=[Q,R,S,T];}else{this.Ai([Q,R,S,T]);this.AE=Y;}return[
S-Q,T-R];},In:function(aOffset,G5){if(G5)this.N=B.sq(this.N,aOffset);else this.Ai(
B.sq(this.N,aOffset));},GetExtent:function(){return this.N;},Ai:function(C){var A;
if(B.sb(C,this.N))return;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);this.
AE=null;this.N=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);if((!!this.
K&&((this.F&0x400)===0x400))&&!((this.K.F&0x2000)===0x2000)){this.F=this.F|0x800;
this.K.F=this.K.F|0x4000;B.kD([A=this.K,A.Bk],this);}},_Init:function(aArg){D.BX.
_Init.call(this,aArg);this.__proto__=D.Bb;},_className:"Core::RectView"};D.Aj={BK:
null,Bq:null,Hj:null,Cy:null,Eg:null,EO:null,CE:null,DO:255,Ci:function(Ar,aClip
,aOffset,AD,aBlend){var A;AD=((AD+1)*this.DO)>>8;aBlend=aBlend&&((this.F&0x2)===
0x2);if(!this.Cy)this.MB(Ar,aClip,B.so(aOffset,this.N.slice(0,2)),AD,aBlend);else{
var BH=255|(255<<8)|(255<<16)|((AD&0xFF)<<24);this.Cy.Update();Ar.Es(aClip,this.
Cy,0,B.sq(this.N,aOffset),Az,BH,BH,BH,BH,aBlend);}},DE:function(Ao,O,Ce,E$,EK){var
A;var L=this.Bq;var EQ=null;var U=A3;var BL=null;var I7=!!this.EO&&(!!this.EO.IV||
!!this.EO.BK);if(((A=B.hV(Ao,this.N))[0]>=A[2])||(A[1]>=A[3]))return null;Ao=B.sp(
Ao,this.N.slice(0,2));while(!!L){if(((L.F&0x400)===0x400)&&!BL){BL=L.V;while(!!BL&&
!((BL.F&0x200)===0x200))BL=BL.V;if(!!BL)U=B.hV(Ao,BL.GetExtent());else U=A3;}if(
BL===L){BL=null;U=A3;}if((((((L.F&0x8)===0x8)&&((L.F&0x10)===0x10))&&!((L.F&0x40000
)===0x40000))&&!((L.F&0x20000)===0x20000))&&(!((L.F&0x10000)===0x10000)||((this.
Eg.Cg===L)&&!I7))){var B2=L.GetExtent();var Hb=E$;var EM=null;if(Hb===L)Hb=null;
if(((L.F&0x400)===0x400)){if(!(((A=B.hV(B2,U))[0]>=A[2])||(A[1]>=A[3])))EM=L.DE(
U,O,Ce,Hb,EK);}else{if(!(((A=B.hV(B2,Ao))[0]>=A[2])||(A[1]>=A[3]))||(E$===L))EM=
L.DE(Ao,O,Ce,Hb,EK);}L=L.V;if(!!EM){if(!EQ||((EM.Fg<EQ.Fg)&&(EM.Fg>=0)))EQ=EM;if(
!EM.Fg)L=null;}}else L=L.V;}return EQ;},A1:function(DS,EJ){var A;var MF=this.F;D.
Bb.A1.call(this,DS,EJ);var Ef=this.F^MF;if(!!this.CE&&((Ef&0x40)===0x40)){if(((this.
F&0x40)===0x40))this.CE.A1(0x40,0x0);else this.CE.A1(0x0,0x40);}if(!!this.Eg&&((
Ef&0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.Eg.Cg.F&0x14)===0x14))this.Eg.
Cg.A1(0x40,0x0);else this.Eg.Cg.A1(0x0,0x40);}if(!!Ef){this.F=this.F|0x8000;B.kD([
this,this.Bk],this);}},Ai:function(C){var A;if(B.sb(C,this.N))return;var Ek=[(A=
this.N)[2]-A[0],A[3]-A[1]];var Hn=[C[2]-C[0],C[3]-C[1]];var EW=!B.sa(Ek,Hn);if(EW&&
!!this.Cy){this.Cy.Is(Hn);B.pl(this,0);B.pl(this.Cy,0);}D.Bb.Ai.call(this,C);if((
EW&&(Ek[0]>0))&&(Ek[1]>0)){var Ah=[].concat(Az,Ek);var L=this.BK;var Gg=0x14;while(
!!L){if((!L.AE&&(L.Cb!==Gg))&&!((L.F&0x400)===0x400))L.Gi(Ah,null);L=L.Ag;}}if(EW
){this.F=this.F|0x5000;B.kD([this,this.Bk],this);}},Jg:function(Al){var Ke=(D.KeyEvent.
isPrototypeOf(Al)?Al:null);var C9=this.Hj;if(!Ke)return null;while(!!C9&&(!C9.BB||
!C9.Bm(Ke)))C9=C9.Ag;return C9;},MB:function(Ar,aClip,aOffset,AD,aBlend){var A;var
L=this.BK;var Ka=A3;var Kh=true;while(!!L){if(((L.F&0x200)===0x200)){var Kg=(D.FR.
isPrototypeOf(L)?L:null);Ka=B.hV(aClip,B.sq(Kg.N,aOffset));Kh=((Kg.F&0x1)===0x1);
}if(((L.F&0x1)===0x1)){if(((L.F&0x400)===0x400)){if(Kh){var U=B.hV(B.sq(L.GetExtent(
),aOffset),Ka);if(!((U[0]>=U[2])||(U[1]>=U[3])))L.Ci(Ar,U,aOffset,AD,aBlend);}}else{
var U=B.hV(B.sq(L.GetExtent(),aOffset),aClip);if(!((U[0]>=U[2])||(U[1]>=U[3])))L.
Ci(Ar,U,aOffset,AD,aBlend);}}L=L.Ag;}},MH:function(){var A;var I0=((this.F&0x1000
)===0x1000);var DX=[0,0,(A=this.N)[2]-A[0],A[3]-A[1]];var DB=false;var EP=A3;var
Hd=Az;var Gh=0;var Hc=0;var L=this.Bq;var BL=null;var Gg=0x14;while(!!L){if(((L.
F&0x800)===0x800)){DB=true;L.F=L.F&~0x800;}if(DB&&((L.F&0x200)===0x200)){DB=false;
if(!!(D.FR.isPrototypeOf(L)?L:null).GN)L.F=L.F|0x1000;}L=L.V;}DB=false;L=this.BK;
if(I0){this.F=this.F&~0x1000;I0=!((DX[0]>=DX[2])||(DX[1]>=DX[3]));}this.F=this.F|
0x2000;while(!!L){if(((L.F&0x400)===0x400)){if(!!L.AE&&(L.AE.EU!==BL))L.AE=null;
if((!L.AE&&DB)&&((L.Cb!==Gg)||!!Hc))L.Gi(EP,BL);}if(!!L.AE){if(I0&&!((L.F&0x400)===
0x400))L.GE(DX,0);if(DB&&((L.F&0x400)===0x400)){var Z=L.GE(B.sq(EP,Hd),Hc);if(((
L.F&0x1)===0x1)){var Cq=Az;switch(Hc){case 3:Cq=[Z[0]+Gh,Cq[1]];break;case 4:Cq=[-
Z[0]-Gh,Cq[1]];break;case 1:Cq=[Cq[0],Z[1]+Gh];break;case 2:Cq=[Cq[0],-Z[1]-Gh];
break;default:;}Hd=B.so(Hd,Cq);}}}if(((L.F&0x200)===0x200)){DB=((L.F&0x1000)===0x1000
);BL=(D.FR.isPrototypeOf(L)?L:null);if(DB){L.F=L.F&~0x1000;EP=B.sq(BL.N,BL.DR);Hd=
Az;Hc=BL.GN;Gh=BL.Space;DB=!((EP[0]>=EP[2])||(EP[1]>=EP[3]));}if(DB){this.An(BL.
N);}}L=L.Ag;}this.F=this.F&~0x2000;this.G3([DX[2]-DX[0],DX[3]-DX[1]]);},Bk:function(
AT){var A;var MN=((this.F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000)){this.
F=this.F&~0x4000;this.MH();}if(((this.F&0x8000)===0x8000)||MN){this.F=this.F&~0x8000;
this.FZ(this.F);}},Ez:function(C){var A;if(!!C&&(C.K!==this))throw new Error(BY);
if(!!C&&!((C.F&0x14)===0x14))C=null;if(!!C&&((C.F&0x10000)===0x10000))C=null;if(
C===this.CE)return;if(!!this.CE)this.CE.A1(0x0,0x60);this.CE=C;if(!!C){if(((this.
F&0x40)===0x40))C.A1(0x60,0x0);else C.A1(0x20,0x0);}},FQ:function(C){if(C)this.A1(
0x1,0x0);else this.A1(0x0,0x1);},JL:function(De){var tmp=this;while(!!tmp){De=B.
sn(De,tmp.N.slice(0,2));tmp=tmp.K;}return De;},DispatchEvent:function(Al){var A;
var L=this.CE;var Aa=(D.Aj.isPrototypeOf(L)?L:null);var Au=null;var I7=!!this.EO&&(
!!this.EO.IV||!!this.EO.BK);if(!!L&&((((L.F&0x10000)===0x10000)||((L.F&0x40000)===
0x40000))||((L.F&0x20000)===0x20000))){L=null;Aa=null;}if(!!this.Eg&&!I7)Au=this.
Eg.Cg.DispatchEvent(Al);if(!Au&&!!Aa)Au=Aa.DispatchEvent(Al);else if(!Au&&!!L)Au=
L.Bm(Al);if(!Au)Au=this.Bm(Al);if(!Au)Au=this.Jg(Al);return Au;},BroadcastEventAtPosition:
function(Al,J6,aFilter){var A;var L=this.Bq;var Au=null;while(!!L&&!Au){if((!aFilter||((
A=aFilter)&&((L.F&A)===A)))&&B.se(L.GetExtent(),J6)){var Aa=(D.Aj.isPrototypeOf(
L)?L:null);if(!!Aa)Au=Aa.BroadcastEventAtPosition(Al,B.sn(J6,Aa.N.slice(0,2)),aFilter
);else Au=L.Bm(Al);}L=L.V;}if(!Au)Au=this.Bm(Al);return Au;},BroadcastEvent:function(
Al,aFilter){var A;var L=this.Bq;var Au=null;while(!!L&&!Au){if(!aFilter||((A=aFilter
)&&((L.F&A)===A))){var Aa=(D.Aj.isPrototypeOf(L)?L:null);if(!!Aa)Au=Aa.BroadcastEvent(
Al,aFilter);else Au=L.Bm(Al);}L=L.V;}if(!Au)Au=this.Bm(Al);if(!Au)Au=this.Jg(Al);
return Au;},G3:function(aSize){},FZ:function(Ea){},Ca:function(){this.F=this.F|0x8000;
B.kD([this,this.Bk],this);},An:function(Ao){var A;var Aa=this;while(!!Aa&&!((Ao[
0]>=Ao[2])||(Ao[1]>=Ao[3]))){var Fb=Aa.Cy;if(!Aa.K&&(Aa!==this)){Aa.An(Ao);return;
}if(!!Fb){if(((A=Fb.Dc)[0]>=A[2])||(A[1]>=A[3])){B.pl(Aa,0);B.pl(Fb,0);}var IZ=false;
if(IZ)Fb.Dc=[0,0,(A=Aa.N)[2]-A[0],A[3]-A[1]];else Fb.Dc=B.pA(Fb.Dc,Ao);}if(!((Aa.
F&0x1)===0x1))return;Ao=B.hV(B.sq(Ao,Aa.N.slice(0,2)),Aa.N);Aa=Aa.K;}},BF:function(
aArg){this.Ca();},Jw:function(G,aFilter){var A;if(!G||(G.K!==this))return null;var
Ej=G.Ag;var Em=G.V;var Gq=0x10000;if(((aFilter&0x10000)===0x10000))Gq=0x0;while(
!!Ej||!!Em){if((!!Ej&&(!aFilter||((A=aFilter)&&((Ej.F&A)===A))))&&(!Gq||!((A=Gq)&&((
Ej.F&A)===A))))return Ej;if((!!Em&&(!aFilter||((A=aFilter)&&((Em.F&A)===A))))&&(
!Gq||!((A=Gq)&&((Em.F&A)===A))))return Em;if(!!Ej)Ej=Ej.Ag;if(!!Em)Em=Em.V;}return null;
},ID:function(G,C8){var A;if(!G)throw new Error(CI);if(G.K!==this)throw new Error(
CJ);if(!!C8&&(C8.K!==this))throw new Error(Du);if(G.Ag===C8)return;if(((G.F&0x401
)===0x401)){if(!!G.V&&!!G.AE)G.V.F=G.V.F|0x800;G.F=G.F|0x800;this.F=this.F|0x4000;
B.kD([this,this.Bk],this);}if(((G.F&0x200)===0x200)){if(!!G.V)G.V.F=G.V.F|0x800;
G.F=G.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bk],this);}if(!!G.Ag)G.Ag.V=G.
V;else this.Bq=G.V;if(!!G.V)G.V.Ag=G.Ag;else this.BK=G.Ag;if(!C8){if(!!this.Bq)this.
Bq.Ag=G;G.V=this.Bq;G.Ag=null;this.Bq=G;}else{G.Ag=C8;G.V=C8.V;C8.V=G;if(!!G.V)G.
V.Ag=G;}if(this.BK===C8)this.BK=G;if(((G.F&0x1)===0x1))this.An(G.GetExtent());},
D9:function(G){var A;if(!G)throw new Error(CI);if(G.K!==this)throw new Error(CJ);
if(!G.Ag)return;if(((G.F&0x401)===0x401)){if(!!G.V&&!!G.AE)G.V.F=G.V.F|0x800;G.F=
G.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bk],this);}if(((G.F&0x200)===0x200
)){if(!!G.V)G.V.F=G.V.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bk],this);}if(
!!G.V)G.V.Ag=G.Ag;else this.BK=G.Ag;G.Ag.V=G.V;G.V=this.Bq;G.Ag=null;this.Bq.Ag=
G;this.Bq=G;if(((G.F&0x1)===0x1))this.An(G.GetExtent());},IC:function(G,BG){var A;
if(!G)throw new Error(CI);if(G.K!==this)throw new Error(CJ);var Eb=G;var Ed=G;while((
BG>0)&&!!Eb.Ag){Eb=Eb.Ag;BG=BG-1;}while((BG<0)&&!!Ed.V){Ed=Ed.V;BG=BG+1;}if((Eb===
G)&&(Ed===G))return;if(((G.F&0x401)===0x401)){if(!!G.V&&!!G.AE)G.V.F=G.V.F|0x800;
G.F=G.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bk],this);}if(((G.F&0x200)===
0x200)){if(!!G.V)G.V.F=G.V.F|0x800;G.F=G.F|0x800;this.F=this.F|0x4000;B.kD([this
,this.Bk],this);}if(!!G.V)G.V.Ag=G.Ag;if(!!G.Ag)G.Ag.V=G.V;if(this.BK===G)this.BK=
G.Ag;if(this.Bq===G)this.Bq=G.V;if(Eb!==G){G.Ag=Eb.Ag;G.V=Eb;Eb.Ag=G;if(!!G.Ag)G.
Ag.V=G;}if(Ed!==G){G.Ag=Ed;G.V=Ed.V;Ed.V=G;if(!!G.V)G.V.Ag=G;}if(!G.Ag)this.Bq=G;
if(!G.V)this.BK=G;if(((G.F&0x1)===0x1))this.An(G.GetExtent());},EC:function(G){var
A;if(!G)throw new Error(Dv);if(G.K!==this)throw new Error(CJ);if((((G.F&0x401)===
0x401)&&!!G.V)&&!!G.AE){G.V.F=G.V.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bk
],this);}if(((G.F&0x200)===0x200)){if(!!G.V)G.V.F=G.V.F|0x800;this.F=this.F|0x4000;
B.kD([this,this.Bk],this);}G.AE=null;if(this.CE===G)this.Ez(this.Jw(G,0x14));if(
!!G.V)G.V.Ag=G.Ag;if(!!G.Ag)G.Ag.V=G.V;if(this.BK===G)this.BK=G.Ag;if(this.Bq===
G)this.Bq=G.V;G.K=null;G.Ag=null;G.V=null;if(((G.F&0x1)===0x1))this.An(G.GetExtent(
));},AZ:function(G,BG){var A;if(!G)throw new Error(IJ);if(!!G.K)throw new Error(
IK);G.K=this;G.V=this.Bq;if(!!this.Bq)this.Bq.Ag=G;this.Bq=G;if(!this.BK)this.BK=
G;if(!!BG)this.IC(G,BG);if(((G.F&0x1)===0x1))this.An(G.GetExtent());if(((!this.CE&&((
G.F&0x4)===0x4))&&((G.F&0x10)===0x10))&&!((G.F&0x10000)===0x10000))this.Ez(G);if(((
G.F&0x401)===0x401)){G.F=G.F|0x800;this.F=this.F|0x4000;B.kD([this,this.Bk],this
);}if(((G.F&0x200)===0x200)){G.F=G.F|0x800;this.F=this.F|0x4000;B.kD([this,this.
Bk],this);}},_Init:function(aArg){D.Bb._Init.call(this,aArg);this.__proto__=D.Aj;
this.F=0x1F;this.BF(aArg);},_Mark:function(E){var A;D.Bb._Mark.call(this,E);if((
A=this.BK)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Bq)&&(A._cycle!=E))A._Mark(
A._cycle=E);if((A=this.Hj)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Cy)&&(A.
_cycle!=E))A._Mark(A._cycle=E);if((A=this.Eg)&&(A._cycle!=E))A._Mark(A._cycle=E);
if((A=this.EO)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.CE)&&(A._cycle!=E))
A._Mark(A._cycle=E);},_className:"Core::Group"};D.Root={B5:null,DZ:null,Df:null,
AH:B.sr(10,null,null),Fc:null,Bg:null,Dy:null,GB:0,IR:0,AK:0,Bh:B.sr(10,0,null),
Ha:B.sr(10,B.pn,null),CO:B.sr(10,0,null),Dx:B.sr(10,B.pm,null),Fe:B.sr(10,0,null
),DT:B.sr(10,B.pm,null),CN:B.sr(10,B.pm,null),Cf:B.sr(10,B.pm,null),Dg:B.sr(10,B.
pm,null),Ap:0,Hf:0,He:0,Hl:B.sr(3,B.pn,null),Kf:0,BM:B.sr(4,0,null),Be:B.sr(4,B.
pn,null),Ba:0,D3:8,Kz:250,Eh:0,DY:0,I1:true,Hk:false,D4:function(){return this;}
,Ci:function(Ar,aClip,aOffset,AD,aBlend){var fullScreenUpdate=false;fullScreenUpdate=
B.i6;if(!fullScreenUpdate)Ar.Fx(aClip,B.sq(B.sq(aClip,aOffset),this.N.slice(0,2)
),0x00000000,0x00000000,0x00000000,0x00000000,false);D.Aj.Ci.call(this,Ar,aClip,
aOffset,AD,aBlend);},A1:function(DS,EJ){var A;D.Aj.A1.call(this,DS,EJ);if(!this.
K&&(((DS&0x1)===0x1)||((EJ&0x1)===0x1)))this.An([0,0,(A=this.N)[2]-A[0],A[3]-A[1
]]);if(!this.K&&(((DS&0x2)===0x2)||((EJ&0x2)===0x2)))this.An([0,0,(A=this.N)[2]-
A[0],A[3]-A[1]]);},Ez:function(C){if((C!==this.Bg)||!C)D.Aj.Ez.call(this,C);},DispatchEvent:
function(Al){if((this.IR>0)&&!!(D.KeyEvent.isPrototypeOf(Al)?Al:null))return null;
if(!!Al){Al.FE=!!this.AK;if(!!this.AK)Al.AF=this.AK;}var Au=null;if(!!this.Bg){Au=
this.Bg.DispatchEvent(Al);if(!!Au){this.AK=0;return Au;}}if(!!this.DZ){Au=this.DZ.
Cg.DispatchEvent(Al);if(!Au)Au=this.Bm(Al);if(!Au)Au=this.Jg(Al);this.AK=0;return Au;
}Au=D.Aj.DispatchEvent.call(this,Al);this.AK=0;return Au;},BroadcastEvent:function(
Al,aFilter){if(!!Al){Al.FE=!!this.AK;if(!!this.AK)Al.AF=this.AK;}var Au=D.Aj.BroadcastEvent.
call(this,Al,aFilter);this.AK=0;return Au;},An:function(Ao){var A;if(this.GB>0)throw new
Error(IL);if(!!this.Cy&&!this.K){if(((A=this.Cy.Dc)[0]>=A[2])||(A[1]>=A[3])){B.pl(
this,0);B.pl(this.Cy,0);}var IZ=false;if(IZ)this.Cy.Dc=[0,0,(A=this.N)[2]-A[0],A[
3]-A[1]];else this.Cy.Dc=B.pA(this.Cy.Dc,Ao);}var fullScreenUpdate=false;fullScreenUpdate=
B.i6;if(fullScreenUpdate)Ao=[0,0,(A=this.N)[2]-A[0],A[3]-A[1]];if(!!this.K){D.Aj.
An.call(this,Ao);return;}Ao=B.hV(B.sq(Ao,this.N.slice(0,2)),this.N);if((Ao[0]>=Ao[
2])||(Ao[1]>=Ao[3]))return;var J;for(J=0;J<this.Ba;J=J+1)if(!(((A=B.hV(this.Be.Get(
J),Ao))[0]>=A[2])||(A[1]>=A[3]))){this.Be.Set(J,B.pA(this.Be.Get(J),Ao));this.BM.
Set(J,B.rY(this.Be.Get(J)));return;}if(this.Ba<3){this.Be.Set(this.Ba,Ao);this.BM.
Set(this.Ba,B.rY(Ao));this.Ba=this.Ba+1;return;}var AM;var B4;var Gk=0;var Gl=0;
var J9=2147483647;this.Be.Set(this.Ba,Ao);this.BM.Set(this.Ba,B.rY(Ao));for(AM=0;
AM<=this.Ba;AM=AM+1)for(B4=AM+1;B4<=this.Ba;B4=B4+1){var Hv=B.rY(B.pA(this.Be.Get(
AM),this.Be.Get(B4)));var Ki=((Hv<<8)/(this.BM.Get(AM)+this.BM.Get(B4)))|0;if(Ki<
J9){J9=Ki;Gk=AM;Gl=B4;}}this.Be.Set(Gk,B.pA(this.Be.Get(Gk),this.Be.Get(Gl)));this.
BM.Set(Gk,B.rY(this.Be.Get(Gk)));if(Gl!==this.Ba){this.Be.Set(Gl,this.Be.Get(this.
Ba));this.BM.Set(Gl,this.BM.Get(this.Ba));}},ID:function(G,C8){if((!C8&&!!this.Bg
)&&(this.Bg.K===this))C8=this.Bg;D.Aj.ID.call(this,G,C8);},D9:function(G){if(!!this.
Bg&&(this.Bg.K===this))this.ID(G,this.Bg);else D.Aj.D9.call(this,G);},IC:function(
G,BG){if(!G)throw new Error(CI);if(((BG>0)&&!!this.Bg)&&(this.Bg.K===this)){var L=
G;var Ho=BG;while((L.Ag!==this.Bg)&&(Ho>0)){L=L.Ag;Ho=Ho-1;}BG=BG-Ho;}D.Aj.IC.call(
this,G,BG);},AZ:function(G,BG){if(((BG>=0)&&!!this.Bg)&&(this.Bg.K===this))BG=-1;
D.Aj.AZ.call(this,G,BG);},Mz:function(){var By=B._NewObject(D.GK,0);By.FE=!!this.
AK;if(!!this.AK)By.AF=this.AK;return By;},EL:function(){var By=B._NewObject(D.GH
,0);By.FE=!!this.AK;if(!!this.AK)By.AF=this.AK;return By;},Fd:function(){var By=
B._NewObject(D.Hx,0);By.FE=!!this.AK;if(!!this.AK)By.AF=this.AK;return By;},MA:function(
AT){var J;var EQ=false;for(J=0;J<10;J=J+1)if(!!this.AH.Get(J)){var Av=this.Cf.Get(
J);var Aa=this.AH.Get(J).K;while(!!Aa&&(Aa!==this)){Av=B.sn(Av,Aa.N.slice(0,2));
Aa=Aa.K;}if(!Aa&&(this.AH.Get(J)!==this)){var tmp=this.AH.Get(J);this.Ap=J;this.
AH.Set(J,null);tmp.Bm(this.EL().InitializeUp(J,this.DT.Get(J),this.Dx.Get(J),this.
CO.Get(J),this.Bh.Get(J)+1,this.CN.Get(J),false,this.Cf.Get(J),this.Dg.Get(J)));
if(tmp===this.Df)this.Df=null;this.BroadcastEvent(this.Fd().InitializeUp(J,this.
Bh.Get(J)+1,false,tmp,this.Cf.Get(J)),0x18);}else{this.CO.Set(J,(this.Dy.AF-this.
Fe.Get(J))|0);if(this.CO.Get(J)<10)this.CO.Set(J,10);this.Ap=J;this.AH.Get(J).Bm(
this.EL().InitializeHold(J,Av,this.Dx.Get(J),this.CO.Get(J),this.Bh.Get(J)+1,this.
CN.Get(J),this.Cf.Get(J),this.Dg.Get(J)));EQ=true;}}if(!EQ)this.Dy.Bn(false);},GetFPS:
function(){var ticksCount=0;var Kd=0;ticksCount=((new Date).getTime()-B.pk)|0;if(
!!this.Hf&&(ticksCount>this.Hf))Kd=((this.He*1000)/((ticksCount-this.Hf)|0))|0;this.
He=0;this.Hf=ticksCount;return Kd;},Update:function(){var A;if(!this.Fc){this.Fc=
B._NewObject(B.Graphics.Canvas,0);this.Fc.Is([(A=this.N)[2]-A[0],A[3]-A[1]]);}this.
Fc.Update();return this.UpdateGE20(this.Fc);},UpdateGE20:function(Ar){if(!this.BeginUpdate(
))return A3;var DC=this.UpdateCanvas(Ar,Az);this.EndUpdate();return DC;},EndUpdate:
function(){if(this.Ba>0){this.He=this.He+1;this.Ba=0;}},UpdateCanvas:function(Ar
,aOffset){var A;var DC=A3;var Mx=[].concat(aOffset,B.so(Ar.FrameSize,aOffset));var
J;var AM=this.Ba;this.GB=this.GB+1;for(J=0;(J<AM)&&(J<4);J=J+1){if(this.BM.Get(J
)>0){this.Ci(Ar,B.sp(this.Be.Get(J),aOffset),[-aOffset[0],-aOffset[1]],255,true);
DC=B.pA(DC,B.hV(Mx,this.Be.Get(J)));}else AM=AM+1;}this.GB=this.GB-1;if(!((DC[0]>=
DC[2])||(DC[1]>=DC[3])))return B.sp(DC,aOffset);else return DC;},GetUpdateRegion:
function(G7){var J;var AM=this.Ba;if(G7<0)return A3;for(J=0;(J<AM)&&(J<4);J=J+1){
if(!this.BM.Get(J)){AM=AM+1;G7=G7+1;}else if(J===G7)return this.Be.Get(J);}return A3;
},BeginUpdate:function(){var MG=true;var fullScreenUpdate=false;var J;if((!MG&&!
fullScreenUpdate)&&(this.Ba>0)){var Kn=B.sr(3,B.pn,null);var Jl=this.Ba;for(J=0;
J<Jl;J=J+1)Kn.Set(J,this.Be.Get(J));for(J=0;J<this.Kf;J=J+1)this.An(this.Hl.Get(
J));for(J=0;J<Jl;J=J+1)this.Hl.Set(J,Kn.Get(J));this.Kf=Jl;}var AM;var B4;for(AM=
0;AM<(this.Ba-1);AM=AM+1)if(this.BM.Get(AM)>0)for(B4=AM+1;B4<this.Ba;B4=B4+1)if(
this.BM.Get(B4)>0){var Hv=B.rY(B.pA(this.Be.Get(AM),this.Be.Get(B4)));if(((Hv-this.
BM.Get(AM))-this.BM.Get(B4))<0){this.Be.Set(AM,B.pA(this.Be.Get(AM),this.Be.Get(
B4)));this.BM.Set(AM,Hv);this.BM.Set(B4,0);}}for(J=this.Ba-1;J>=0;J=J-1)if(!this.
BM.Get(J))this.Ba=this.Ba-1;return this.Ba;},DoesNeedUpdate:function(){if(this.Ba>
0)return true;return false;},Initialize:function(aSize){this.Ai([].concat(Az,aSize
));if(this.I1)this.F=this.F|0x60;else this.F=this.F|0x20;this.An(this.N);return this;
},SetRootFocus:function(IN){if(IN===this.I1)return false;this.I1=IN;if(!IN){if(!
!this.Bg)this.Bg.A1(0x0,0x40);if(!!this.DZ)this.DZ.Cg.A1(0x0,0x40);else this.A1(
0x0,0x40);}else{if(!!this.DZ)this.DZ.Cg.A1(0x40,0x0);else this.A1(0x40,0x0);if(!
!this.Bg)this.Bg.A1(0x40,0x0);}return true;},SetUserInputTimestamp:function(Mw){
this.AK=Mw;},DriveKeyboardHitting:function(A4,EI,Cd){var A;var Jh=!!this.B5;if(!
!this.B5&&((!Cd||(this.Eh!==A4))||(this.DY!==EI))){var By=null;var L=(D.BX.isPrototypeOf(
A=this.B5)?A:null);var C9=(D.Ev.isPrototypeOf(A=this.B5)?A:null);if(!!this.Eh)By=
B._NewObject(D.KeyEvent,0).Initialize(this.Eh,false);if(this.DY!==0x00)By=B._NewObject(
D.KeyEvent,0).Initialize2(this.DY,false);if(!!C9)C9.Bm(By);else if(!!L)L.Bm(By);
this.Eh=0;this.DY=0x00;this.B5=null;}if(!!this.B5){var By=null;var L=(D.BX.isPrototypeOf(
A=this.B5)?A:null);var C9=(D.Ev.isPrototypeOf(A=this.B5)?A:null);if(!!A4)By=B._NewObject(
D.KeyEvent,0).Initialize(A4,true);if(this.DY!==0x00)By=B._NewObject(D.KeyEvent,0
).Initialize2(EI,true);if(!!C9)C9.Bm(By);else if(!!L)L.Bm(By);}if(this.Hk&&((!Cd||(
this.Eh!==A4))||(this.DY!==EI))){this.Eh=0;this.DY=0x00;this.Hk=false;}if((!this.
B5&&Cd)&&(this.IR>0)){this.Eh=A4;this.DY=EI;this.Hk=true;}if((!this.B5&&Cd)&&!this.
Hk){if(!!A4)this.B5=this.DispatchEvent(B._NewObject(D.KeyEvent,0).Initialize(A4,
true));if(EI!==0x00)this.B5=this.DispatchEvent(B._NewObject(D.KeyEvent,0).Initialize2(
EI,true));if(!(D.Ev.isPrototypeOf(A=this.B5)?A:null)&&!(D.BX.isPrototypeOf(A=this.
B5)?A:null))this.B5=null;this.Eh=A4;this.DY=EI;Jh=Jh||!!this.B5;}this.AK=0;return Jh;
},DriveCursorMovement:function(A5){return this.DriveMultiTouchMovement(this.Ap,A5
);},DriveMultiTouchMovement:function(O,A5){if((O<0)||(O>9)){this.AK=0;return false;
}var Cp=B.sn(A5,this.Cf.Get(O));this.Cf.Set(O,A5);if(!this.AH.Get(O)||B.sa(Cp,Az
)){this.AK=0;return false;}var Av=A5;var Aa=this.AH.Get(O).K;while(!!Aa&&(Aa!==this
)){Av=B.sn(Av,Aa.N.slice(0,2));Aa=Aa.K;}if(!Aa&&(this.AH.Get(O)!==this)){var tmp=
this.AH.Get(O);this.Ap=O;this.AH.Set(O,null);tmp.Bm(this.EL().InitializeUp(O,this.
DT.Get(O),this.Dx.Get(O),this.CO.Get(O),this.Bh.Get(O)+1,this.CN.Get(O),false,this.
Cf.Get(O),this.Dg.Get(O)));if(tmp===this.Df)this.Df=null;this.BroadcastEvent(this.
Fd().InitializeUp(O,this.Bh.Get(O)+1,false,tmp,A5),0x18);}else{this.DT.Set(O,Av);
this.Ap=O;this.AH.Get(O).Bm(this.Mz().Initialize(O,Av,this.Dx.Get(O),Cp,this.CO.
Get(O),this.Bh.Get(O)+1,this.CN.Get(O),A5,this.Dg.Get(O)));}this.AK=0;return true;
},DriveCursorHitting:function(Cd,O,A5){return this.DriveMultiTouchHitting(Cd,O,A5
);},DriveMultiTouchHitting:function(Cd,O,A5){var A;if((O<0)||(O>9)){this.AK=0;return false;
}var ticksCount=this.AK;var EN=[].concat([-this.D3,-this.D3],[this.D3+1,this.D3+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.pk)|0;}var MM=this.AK;this.
DriveMultiTouchMovement(O,A5);A5=this.Cf.Get(O);this.AK=MM;if(Cd)this.Dg.Set(O,A5
);if((Cd&&!this.AH.Get(O))&&!this.IR){var B3=null;var Av=A5;if(B.se(this.Ha.Get(
O),A5)&&((ticksCount-this.Fe.Get(O))<=(((A=this.Kz)<0)?A+0x100000000:A)))this.Bh.
Set(O,this.Bh.Get(O)+1);else this.Bh.Set(O,0);this.Ha.Set(O,B.sq(EN,A5));this.Fe.
Set(O,ticksCount);if((!!this.Bg&&!!this.Bg.K)&&((this.Bg.F&0x18)===0x18)){var U=
B.sq(EN,this.Bg.K.JL(A5));B3=this.Bg.DE(U,O,this.Bh.Get(O)+1,null,0x0);}if(!B3){
if(!!this.Df&&!!this.Df.K){if(((this.Df.F&0x8)===0x8)&&((this.Df.F&0x10)===0x10)
){var U=B.sq(EN,this.Df.K.JL(A5));B3=this.Df.DE(U,O,this.Bh.Get(O)+1,null,0x0);}
}else if(!!this.DZ)B3=this.DE(B.sq(EN,A5),O,this.Bh.Get(O)+1,this.DZ.Cg,0x0);else
B3=this.DE(B.sq(EN,A5),O,this.Bh.Get(O)+1,null,0x0);}if(!!B3){this.BroadcastEvent(
this.Fd().InitializeDown(O,this.Bh.Get(O)+1,false,B3.BX,A5),0x18);this.AH.Set(O,
B3.BX);this.CN.Set(O,B3.DM);}else{this.AH.Set(O,null);this.CN.Set(O,Az);this.AK=
0;return false;}var Aa=B3.BX.K;while(!!Aa&&(Aa!==this)){Av=B.sn(Av,Aa.N.slice(0,
2));Aa=Aa.K;}this.Dx.Set(O,Av);this.DT.Set(O,Av);this.CO.Set(O,0);this.Dy.Bn(true
);this.Ap=O;this.AH.Get(O).Bm(this.EL().InitializeDown(O,Av,this.Bh.Get(O)+1,this.
CN.Get(O),false,A5));this.AK=0;return true;}if(!Cd&&!!this.AH.Get(O)){var Av=A5;
var Aa=this.AH.Get(O).K;while(!!Aa&&(Aa!==this)){Av=B.sn(Av,Aa.N.slice(0,2));Aa=
Aa.K;}if(!Aa)Av=this.DT.Get(O);this.Ap=O;var tmp=this.AH.Get(O);this.AH.Set(O,null
);tmp.Bm(this.EL().InitializeUp(O,Av,this.Dx.Get(O),this.CO.Get(O),this.Bh.Get(O
)+1,this.CN.Get(O),false,A5,this.Dg.Get(O)));this.BroadcastEvent(this.Fd().InitializeUp(
O,this.Bh.Get(O)+1,false,tmp,A5),0x18);this.AK=0;return true;}this.AK=0;return false;
},FV:function(Dw,J5,EK){var EN=[].concat([-this.D3,-this.D3],[this.D3+1,this.D3+
1]);if(Dw===this)Dw=null;if(!this.AH.Get(this.Ap))return;var B3;B3=this.DE(B.sq(
EN,this.Cf.Get(this.Ap)),this.Ap,1,Dw,EK);if(!!B3&&(this.AH.Get(this.Ap)!==B3.BX
))this.Jq(B3.BX,B3.DM);if(!B3&&(this.AH.Get(this.Ap)!==J5))this.Jq(J5,Az);},Jq:function(
Dw,CK){if(!this.AH.Get(this.Ap)||(this.AH.Get(this.Ap)===Dw))return;var tmp=this.
AH.Get(this.Ap);this.AH.Set(this.Ap,null);tmp.Bm(this.EL().InitializeUp(this.Ap,
this.DT.Get(this.Ap),this.Dx.Get(this.Ap),this.CO.Get(this.Ap),this.Bh.Get(this.
Ap)+1,this.CN.Get(this.Ap),true,this.Cf.Get(this.Ap),this.Dg.Get(this.Ap)));this.
BroadcastEvent(this.Fd().InitializeUp(this.Ap,this.Bh.Get(this.Ap)+1,true,tmp,this.
Cf.Get(this.Ap)),0x18);var Av=this.Cf.Get(this.Ap);var Aa=null;if(!!Dw)Aa=Dw.K;while(
!!Aa&&(Aa!==this)){Av=B.sn(Av,Aa.N.slice(0,2));Aa=Aa.K;}if(!Aa&&(Dw!==this)){this.
AH.Set(this.Ap,null);this.Dy.Bn(false);return;}this.BroadcastEvent(this.Fd().InitializeDown(
this.Ap,this.Bh.Get(this.Ap)+1,true,Dw,this.Cf.Get(this.Ap)),0x18);var ticksCount=
0;ticksCount=((new Date).getTime()-B.pk)|0;this.AH.Set(this.Ap,Dw);this.CN.Set(this.
Ap,CK);this.Dx.Set(this.Ap,Av);this.DT.Set(this.Ap,Av);this.Bh.Set(this.Ap,0);this.
CO.Set(this.Ap,0);this.Fe.Set(this.Ap,ticksCount);this.Dg.Set(this.Ap,this.Cf.Get(
this.Ap));this.AH.Get(this.Ap).Bm(this.EL().InitializeDown(this.Ap,Av,this.Bh.Get(
this.Ap)+1,this.CN.Get(this.Ap),true,this.Dg.Get(this.Ap)));},_Init:function(aArg
){D.Aj._Init.call(this,aArg);D.Timer._Init.call(this.Dy={Ae:this},0);(this.AH=[]
).__proto__=D.Root.AH;(this.Bh=[]).__proto__=D.Root.Bh;(this.Ha=[]).__proto__=D.
Root.Ha;(this.CO=[]).__proto__=D.Root.CO;(this.Dx=[]).__proto__=D.Root.Dx;(this.
Fe=[]).__proto__=D.Root.Fe;(this.DT=[]).__proto__=D.Root.DT;(this.CN=[]).__proto__=
D.Root.CN;(this.Cf=[]).__proto__=D.Root.Cf;(this.Dg=[]).__proto__=D.Root.Dg;(this.
Hl=[]).__proto__=D.Root.Hl;(this.BM=[]).__proto__=D.Root.BM;(this.Be=[]).__proto__=
D.Root.Be;this.__proto__=D.Root;this.F=0x7F;this.Dy.FP(50);this.Dy.EB=[this,this.
MA];},_Done:function(){this.__proto__=D.Root;this.Dy._Done();D.Aj._Done.call(this
);},_ReInit:function(){D.Aj._ReInit.call(this);this.Dy._ReInit();},_Mark:function(
E){var A;D.Aj._Mark.call(this,E);if((A=this.B5)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.DZ)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Df)&&(A._cycle!=
E))A._Mark(A._cycle=E);B.sj(this.AH,E);if((A=this.Fc)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.Bg)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Dy)._cycle!=E)A.
_Mark(A._cycle=E);},_className:"Core::Root"};D.Event={AF:0,FE:false,GP:function(
){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk)|0;return ticksCount;},
BF:function(aArg){this.AF=this.GP();},_Init:function(aArg){this.__proto__=D.Event;
this.BF(aArg);B.gb++;},_Done:function(){this.__proto__=D.Event;B.gb--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=
E);},Ae:null,_cycle:0,_observers:null,_className:"Core::Event"};D.KeyEvent={A$:0
,Aw:0,Down:false,Initialize2:function(A4,Cd){this.A$=0;this.Aw=A4;this.Down=Cd;if((
A4>=0x30)&&(A4<=0x39))this.A$=(10+A4)-48;if((A4>=0x41)&&(A4<=0x5A))this.A$=(105+
A4)-65;if((A4>=0x61)&&(A4<=0x7A))this.A$=(105+A4)-97;if(A4===0x20)this.A$=131;if(
!this.A$)switch(A4){case 0x2B:this.A$=132;break;case 0x2D:this.A$=133;break;case
0x2A:this.A$=134;break;case 0x2F:this.A$=135;break;case 0x3D:this.A$=136;break;case
0x2E:this.A$=137;break;case 0x2C:this.A$=138;break;case 0x3A:this.A$=139;break;case
0x3B:this.A$=140;break;default:;}return this;},Initialize:function(A4,Cd){this.A$=
A4;this.Down=Cd;this.Aw=0x00;var IT=A4-10;var IS=A4-105;if((IT>=0)&&(IT<=9))this.
Aw=(48+IT)&0xFFFF;if((IS>=0)&&(IS<=25))this.Aw=(65+IS)&0xFFFF;if(A4===131)this.Aw=
0x20;if(this.Aw===0x00)switch(A4){case 132:this.Aw=0x2B;break;case 133:this.Aw=0x2D;
break;case 134:this.Aw=0x2A;break;case 135:this.Aw=0x2F;break;case 136:this.Aw=0x3D;
break;case 137:this.Aw=0x2E;break;case 138:this.Aw=0x2C;break;case 139:this.Aw=0x3A;
break;case 140:this.Aw=0x3B;break;default:;}return this;},KU:function(J4){switch(
J4){case 141:return((this.Aw>=0x41)&&(this.Aw<=0x5A))||((this.Aw>=0x61)&&(this.Aw<=
0x7A));case 142:return(((this.Aw>=0x41)&&(this.Aw<=0x5A))||((this.Aw>=0x61)&&(this.
Aw<=0x7A)))||((this.Aw>=0x30)&&(this.Aw<=0x39));case 143:return(this.Aw>=0x30)&&(
this.Aw<=0x39);case 144:return(((this.Aw>=0x41)&&(this.Aw<=0x46))||((this.Aw>=0x61
)&&(this.Aw<=0x66)))||((this.Aw>=0x30)&&(this.Aw<=0x39));case 145:return this.Aw
!==0x00;case 146:return(this.Aw===0x00)&&!!this.A$;case 147:return(((this.A$===6
)||(this.A$===7))||(this.A$===4))||(this.A$===5);case 148:return(this.Aw!==0x00)||
!!this.A$;default:;}return J4===this.A$;},_Init:function(aArg){D.Event._Init.call(
this,aArg);this.__proto__=D.KeyEvent;},_className:"Core::KeyEvent"};D.Hx={IF:null
,C0:B.pm,CF:0,Cj:0,Down:false,Db:false,InitializeUp:function(O,Ce,EH,IP,C7){this.
Down=false;this.Cj=O;this.CF=Ce;this.C0=C7;this.IF=IP;this.Db=EH;return this;},InitializeDown:
function(O,Ce,EH,IP,C7){this.Down=true;this.Cj=O;this.CF=Ce;this.C0=C7;this.IF=IP;
this.Db=EH;return this;},_Init:function(aArg){D.Event._Init.call(this,aArg);this.
__proto__=D.Hx;},_Mark:function(E){var A;D.Event._Mark.call(this,E);if((A=this.IF
)&&(A._cycle!=E))A._Mark(A._cycle=E);},_className:"Core::CursorGrabEvent"};D.GH={
DK:B.pm,C0:B.pm,CF:0,BO:0,Dq:B.pm,CY:B.pm,Cj:0,Down:false,Db:false,InitializeHold:
function(O,D$,F$,Ga,Ce,CK,C7,F_){this.Down=true;this.Cj=O;this.CY=B.so(D$,CK);this.
Dq=B.so(F$,CK);this.BO=Ga;this.CF=Ce;this.C0=C7;this.DK=F_;return this;},InitializeUp:
function(O,D$,F$,Ga,Ce,CK,EH,C7,F_){this.Down=false;this.Cj=O;this.CY=B.so(D$,CK
);this.Dq=B.so(F$,CK);this.BO=Ga;this.CF=Ce;this.Db=EH;this.C0=C7;this.DK=F_;return this;
},InitializeDown:function(O,D$,Ce,CK,EH,C7){this.Down=true;this.Cj=O;this.CY=B.so(
D$,CK);this.Dq=B.so(D$,CK);this.BO=0;this.CF=Ce;this.Db=EH;this.C0=C7;this.DK=C7;
return this;},_Init:function(aArg){D.Event._Init.call(this,aArg);this.__proto__=
D.GH;},_className:"Core::CursorEvent"};D.GK={DK:B.pm,C0:B.pm,CF:0,BO:0,DM:B.pm,Dq:
B.pm,CY:B.pm,Cj:0,Initialize:function(O,D$,F$,aOffset,Ga,Mv,CK,C7,F_){this.Cj=O;
this.CY=B.so(D$,CK);this.Dq=B.so(F$,CK);this.DM=aOffset;this.BO=Ga;this.CF=Mv;this.
C0=C7;this.DK=F_;return this;},_Init:function(aArg){D.Event._Init.call(this,aArg
);this.__proto__=D.GK;},_className:"Core::DragEvent"};D.FR={DR:B.pm,Space:0,GN:0
,Ci:function(Ar,aClip,aOffset,AD,aBlend){},Ai:function(C){var A;if(B.sb(C,this.N
))return;var Ek=[(A=this.N)[2]-A[0],A[3]-A[1]];var Hn=[C[2]-C[0],C[3]-C[1]];var EW=
!B.sa(Ek,Hn);var Cp=B.sn(C.slice(0,2),this.N.slice(0,2));if(!B.sa(Cp,Az)&&!EW){var
L=this.Ag;while(!!L&&!((L.F&0x200)===0x200)){if(((L.F&0x400)===0x400)){var tmp=((
L.F&0x100)===0x100);L.In(Cp,tmp);}L=L.Ag;}}if((EW&&(Ek[0]>0))&&(Ek[1]>0)){var Ah=
B.sq(this.N,this.DR);var L=this.Ag;var Gg=0x14;while(!!L&&!((L.F&0x200)===0x200)
){if(((L.F&0x400)===0x400)){if(!!L.AE&&(L.AE.EU!==this))L.AE=null;if(!L.AE&&((L.
Cb!==Gg)||!!this.GN))L.Gi(Ah,this);}L=L.Ag;}}D.Bb.Ai.call(this,C);if(!!this.K&&EW
){this.F=this.F|0x1000;if(!((this.K.F&0x2000)===0x2000)){this.K.F=this.K.F|0x4000;
B.kD([A=this.K,A.Bk],this);}}},_Init:function(aArg){D.Bb._Init.call(this,aArg);this.
__proto__=D.FR;this.F=0x203;},_className:"Core::Outline"};D.G1={JP:null,C3:null,
C2:null,FO:null,BW:null,BV:null,IQ:0,Cj:0,AF:0,CF:0,BO:0,DM:B.pm,Dq:B.pm,CY:B.pm
,L9:1000,FW:8,ED:0,JK:-1,JM:1,KV:1,KB:false,Down:false,Dr:false,Db:false,J8:false
,Ci:function(Ar,aClip,aOffset,AD,aBlend){},Bm:function(Al){var A;var Bz=(D.GH.isPrototypeOf(
Al)?Al:null);var Bj=(D.GK.isPrototypeOf(Al)?Al:null);var Hg=this.Dr;if(!Bz&&!Bj)
return null;if(!!Bz){this.J8=Bz.Down;this.Down=Bz.Down;this.Dr=this.H$(Bz.CY);this.
Dq=Bz.Dq;this.CY=Bz.CY;this.DM=Az;this.BO=Bz.BO;this.CF=Bz.CF;this.Db=Bz.Db;this.
Cj=Bz.Cj;this.IQ=Bz.Cj;this.AF=Bz.AF;if(Bz.Down&&!Bz.BO)Hg=false;}if(!!Bj){this.
Down=true;this.Dr=this.H$(Bj.CY);this.Dq=Bj.Dq;this.CY=Bj.CY;this.DM=Bj.DM;this.
BO=Bj.BO;this.CF=Bj.CF;this.Cj=Bj.Cj;this.IQ=Bj.Cj;this.Db=false;this.AF=Bj.AF;(
A=this.JP)?A[1].call(A[0],this):null;}if((!!Bz&&this.Down)&&!this.BO)(A=this.BV)?
A[1].call(A[0],this):null;if((!!Bz&&this.Down)&&(this.BO>0))(A=this.FO)?A[1].call(
A[0],this):null;if((this.Down&&this.Dr)&&!Hg)(A=this.C2)?A[1].call(A[0],this):null;
if(((this.Down&&!this.Dr)&&Hg)||((!this.Down&&this.Dr)&&Hg))(A=this.C3)?A[1].call(
A[0],this):null;if(!!Bz&&!this.Down)(A=this.BW)?A[1].call(A[0],this):null;if(!!this.
ED){if(((((this.ED&0x10)===0x10)&&!!Bz)&&Bz.Down)&&(Bz.BO>=this.L9))this.D4().FV(
null,this,0x10);if((((this.ED&0x1)===0x1)&&!!Bj)&&((Bj.C0[1]-Bj.DK[1])<=-this.FW
))this.D4().FV(null,this,0x1);if((((this.ED&0x2)===0x2)&&!!Bj)&&((Bj.C0[1]-Bj.DK[
1])>=this.FW))this.D4().FV(null,this,0x2);if((((this.ED&0x4)===0x4)&&!!Bj)&&((Bj.
C0[0]-Bj.DK[0])<=-this.FW))this.D4().FV(null,this,0x4);if((((this.ED&0x8)===0x8)&&
!!Bj)&&((Bj.C0[0]-Bj.DK[0])>=this.FW))this.D4().FV(null,this,0x8);}return this;}
,DE:function(Ao,O,Ce,E$,EK){var A;if(!!E$&&(E$!==this))return null;if((Ce<this.KV
)||(Ce>this.JM))return null;if((this.JK>=0)&&(O!==this.JK))return null;if((!this.
KB&&this.J8)&&(O!==this.IQ))return null;if(!!(EK&this.ED))return null;if(this.KQ(
)){var M=B.hV(Ao,this.GetExtent());if(!((M[0]>=M[2])||(M[1]>=M[3]))){var Ee=B.rZ(
Ao);var Cp=Az;if(Ee[0]<M[0])Cp=[M[0]-Ee[0],Cp[1]];if(Ee[0]>=M[2])Cp=[(M[2]-Ee[0]
)-1,Cp[1]];if(Ee[1]<M[1])Cp=[Cp[0],M[1]-Ee[1]];if(Ee[1]>=M[3])Cp=[Cp[0],(M[3]-Ee[
1])-1];return B._NewObject(D.GI,0).Initialize(this,Cp);}}else{var AI=B.sr(9,B.pm
,null);var J;AI.Set(0,B.rZ(Ao));AI.Set(1,AI.Get(0));AI.Set(2,AI.Get(0));AI.Set(3
,AI.Get(0));AI.Set(4,AI.Get(0));AI.Set(1,[Ao[0],AI.Get(1)[1]]);AI.Set(2,[AI.Get(
2)[0],Ao[1]]);AI.Set(3,[Ao[2],AI.Get(3)[1]]);AI.Set(4,[AI.Get(4)[0],Ao[3]]);AI.Set(
5,Ao.slice(0,2));AI.Set(6,[Ao[2],Ao[1]]);AI.Set(7,[Ao[0],Ao[3]]);AI.Set(8,Ao.slice(
2,4));for(J=0;J<9;J=J+1)if(this.H$(AI.Get(J)))return B._NewObject(D.GI,0).Initialize(
this,B.sn(AI.Get(J),AI.Get(0)));}return null;},JV:function(C){if(C<1)C=1;this.FW=
C;},JU:function(C){if(C<1)C=1;this.JM=C;},Bn:function(C){if(C)this.A1(0x10,0x0);
else this.A1(0x0,0x10);},_Init:function(aArg){D.FU._Init.call(this,aArg);this.__proto__=
D.G1;this.F=0x11B;},_Mark:function(E){var A;D.FU._Mark.call(this,E);if((A=this.JP
)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.C3)&&((A=A[0])._cycle!=E)
)A._Mark(A._cycle=E);if((A=this.C2)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((
A=this.FO)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.BW)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);if((A=this.BV)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E
);},_className:"Core::SimpleTouchHandler"};D.Ev={Ag:null,BW:null,BV:null,FO:null
,EV:0,AF:0,JX:0,CD:148,A$:0,Aw:0,BB:true,Down:false,IB:false,GG:false,Bm:function(
Al){var A;if(!!Al&&Al.KU(this.CD)){this.Down=Al.Down;this.A$=Al.A$;this.Aw=Al.Aw;
this.AF=Al.AF;this.GG=false;if(Al.Down){this.JX=this.EV;this.IB=this.EV>0;if(this.
IB)(A=this.FO)?A[1].call(A[0],this):null;else(A=this.BV)?A[1].call(A[0],this):null;
if(!this.GG)this.EV=this.EV+1;return!this.GG;}if(!Al.Down){this.IB=this.EV>1;this.
JX=this.EV-1;this.EV=0;(A=this.BW)?A[1].call(A[0],this):null;return!this.GG;}}return false;
},BF:function(aArg){var A;var Cg=(D.Aj.isPrototypeOf(A=this.Ae)?A:null);if(!Cg)throw new
Error(G4);this.Ag=Cg.Hj;Cg.Hj=this;},_Init:function(aArg){this.__proto__=D.Ev;this.
BF(aArg);B.gb++;},_Done:function(){this.__proto__=D.Ev;B.gb--;},_ReInit:function(
){},_Mark:function(E){var A;if((A=this.Ag)&&(A._cycle!=E))A._Mark(A._cycle=E);if((
A=this.BW)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.BV)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);if((A=this.FO)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E
);if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:null,_cycle:0,_observers:
null,_className:"Core::KeyPressHandler"};D.GI={BX:null,Fg:0,DM:B.pm,Initialize:function(
G,aOffset){this.BX=G;this.DM=aOffset;this.Fg=(aOffset[0]*aOffset[0])+(aOffset[1]
*aOffset[1]);return this;},_Init:function(aArg){this.__proto__=D.GI;B.gb++;},_Done:
function(){this.__proto__=D.GI;B.gb--;},_ReInit:function(){},_Mark:function(E){var
A;if((A=this.BX)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ae)&&(A._cycle!=E
))A._Mark(A._cycle=E);},Ae:null,_cycle:0,_observers:null,_className:"Core::CursorHit"
};D.JN={Cg:null,_Init:function(aArg){this.__proto__=D.JN;B.gb++;},_Done:function(
){this.__proto__=D.JN;B.gb--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.Cg)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ae)&&(A._cycle!=E))A._Mark(
A._cycle=E);},Ae:null,_cycle:0,_observers:null,_className:"Core::ModalContext"};
D.FM={EU:null,B2:B.pn,Ah:B.pn,isEmpty:false,_Init:function(aArg){this.__proto__=
D.FM;B.gb++;},_Done:function(){this.__proto__=D.FM;B.gb--;},_ReInit:function(){}
,_Mark:function(E){var A;if((A=this.EU)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=
this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:null,_cycle:0,_observers:null,_className:
"Core::LayoutContext"};D.GT={Gw:B.pm,Gv:B.pm,Gu:B.pm,Gt:B.pm,_Init:function(aArg
){D.FM._Init.call(this,aArg);this.__proto__=D.GT;},_className:"Core::LayoutQuadContext"
};D.Jr={Cg:null,_Init:function(aArg){this.__proto__=D.Jr;B.gb++;},_Done:function(
){this.__proto__=D.Jr;B.gb--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.Cg)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ae)&&(A._cycle!=E))A._Mark(
A._cycle=E);},Ae:null,_cycle:0,_observers:null,_className:"Core::DialogContext"};
D.J0={IV:null,BK:null,_Init:function(aArg){this.__proto__=D.J0;B.gb++;},_Done:function(
){this.__proto__=D.J0;B.gb--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.IV)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.BK)&&(A._cycle!=E))A._Mark(
A._cycle=E);if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:null,_cycle:0
,_observers:null,_className:"Core::TaskQueue"};D.JZ={_Init:function(aArg){this.__proto__=
D.JZ;B.gb++;},_Done:function(){this.__proto__=D.JZ;B.gb--;},_ReInit:function(){}
,_Mark:function(E){var A;if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:
null,_cycle:0,_observers:null,_className:"Core::Task"};D.D8={resource:null,CC:function(
){this.resource=null;},BF:function(aArg){this.resource=aArg;},_Init:function(aArg
){this.__proto__=D.D8;this.BF(aArg);B.gb++;},_Done:function(){this.__proto__=D.D8;
this.CC();B.gb--;},_ReInit:function(){},_Mark:function(E){var A;if((A=this.Ae)&&(
A._cycle!=E))A._Mark(A._cycle=E);},Ae:null,_cycle:0,_observers:null,_className:"Core::Resource"
};D.Timer={EB:null,timer:null,AF:0,Period:1000,GF:0,BB:false,CC:function(){var tmp=
this.timer;if(!!tmp)tmp.DestroyTimer();this.timer=null;},Hq:function(aBegin,aPeriod
){if(aBegin<0)aBegin=0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>
0)||(aPeriod>0)))tmp=B.rz(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(
aBegin,aPeriod);}this.timer=tmp;},FP:function(C){if(C<0)C=0;if(C===this.Period)return;
this.Period=C;if(this.BB)this.Hq(this.GF,C);},Ey:function(C){if(C<0)C=0;if(C===this.
GF)return;this.GF=C;if(this.BB)this.Hq(C,this.Period);},Bn:function(C){if(C===this.
BB)return;this.BB=C;if(C)this.Hq(this.GF,this.Period);else this.Hq(0,0);this.AF=
this.GP();},GP:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.pk
)|0;return ticksCount;},Trigger:function(){var A;this.AF=this.GP();if(!this.Period
)this.Bn(false);(A=this.EB)?A[1].call(A[0],this):null;},_Init:function(aArg){this.
__proto__=D.Timer;B.gb++;},_Done:function(){this.__proto__=D.Timer;this.CC();B.gb--;
},_ReInit:function(){},_Mark:function(E){var A;if((A=this.EB)&&((A=A[0])._cycle!=
E))A._Mark(A._cycle=E);if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:null
,_cycle:0,_observers:null,_className:"Core::Timer"};D.N3={N4:0x1,MW:0x2,Nk:0x4,N1:
0x8,BB:0x10,NT:0x20,Nl:0x40,Nt:0x80,Nj:0x100,Np:0x200,Nd:0x400,NI:0x800,G3:0x1000
,N2:0x2000,Nz:0x4000,NA:0x8000,M9:0x10000,Ny:0x20000,NQ:0x40000};D.Cb={NJ:0x1,NK:
0x2,MS:0x4,MT:0x8,MU:0x10,MR:0x20};D.GN={Nw:0,N0:1,M1:2,Nq:3,NM:4};D.KeyCode={NoKey:
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
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};D.NL={N8:0x1,N5:0x2,N6:0x4,N7:0x8,Ns:
0x10};
D._Init=function(){D.FU.__proto__=D.BX;D.Bb.__proto__=D.BX;D.Aj.__proto__=D.Bb;D.
Root.__proto__=D.Aj;D.KeyEvent.__proto__=D.Event;D.Hx.__proto__=D.Event;D.GH.__proto__=
D.Event;D.GK.__proto__=D.Event;D.FR.__proto__=D.Bb;D.G1.__proto__=D.FU;D.GT.__proto__=
D.FM;};D.Bw=function(E){};return D;})();

/* Embedded Wizard */