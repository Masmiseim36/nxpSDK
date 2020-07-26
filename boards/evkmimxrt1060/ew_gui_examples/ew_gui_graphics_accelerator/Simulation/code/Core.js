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
Core=(function(){var B=EmWiApp;var D={};
var Aw=[0,0];var A1=[0,0,0,0];var BX="The view does not belong to this group";var
Dy="No view to restack";var Df="View is not in this group";var Dz="No view to remove";
var DA="No view to add";var I0="View already in a group";var I1="Recursive invalidate during active update cycle.";
var I2="The KeyPressHandler is embedded within an object not being derived "+"from Core::Group.";
D.Ce={AD:null,Ak:null,J:null,AG:null,F:0x103,EJ:0,Cc:0x14,Gu:function(W,Hi){},Fb:
function(C){var A;var BI=C^this.Cc;if(!BI)return;this.Cc=C;if(!!this.AG&&!((this.
F&0x400)===0x400)){this.J.F=this.J.F|0x5000;B.lq([A=this.J,A.BM],this);this.J.Ap([
0,0,(A=this.J.M)[2]-A[0],A[3]-A[1]]);}if(!!this.AG&&((this.F&0x400)===0x400)){this.
AG.E0.F=this.AG.E0.F|0x1000;this.J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM],this);
}},FL:function(){var Z=this.J;while(!!Z){var Ky=(D.Root.isPrototypeOf(Z)?Z:null);
if(!!Ky)return Ky;Z=Z.J;}return null;},Ck:function(Aq,aClip,aOffset,AC,aBlend){}
,Bo:function(Ai){return null;},DI:function(Am,N,Cg,Fi,EQ){return null;},GQ:function(
W,Cy){return Aw;},IH:function(aOffset,Hh){},GetExtent:function(){return A1;},A3:
function(DY,EP){var A;if(((this.F&0x200)===0x200))DY=DY&~0x400;var Jm=(this.F&~EP
)|DY;var Ej=Jm^this.F;this.F=Jm;if(!!this.J&&!!(Ej&0x14)){var Kq=((this.F&0x14)===
0x14);if(Kq&&!this.J.CG)this.J.EE(this);if(!Kq&&(this.J.CG===this))this.J.EE(this.
J.JM(this,0x14));}if(!!this.J&&!!(Ej&0x403))this.J.Ap(this.GetExtent());if(((!!this.
AG&&!!this.J)&&((Jm&0x400)===0x400))&&((Ej&0x1)===0x1)){this.F=this.F|0x800;this.
J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM],this);}if(!!this.J&&((Ej&0x400)===0x400
)){this.AG=null;this.F=this.F|0x800;this.J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM
],this);}},_Init:function(aArg){this.__proto__=D.Ce;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(E){var A;if((A=this.
AD)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ak)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.J)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.AG)&&(A._cycle!=E
))A._Mark(A._cycle=E);if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null
,_cycle:0,_observers:null,_className:"Core::View"};D.Ge={Bq:B.qx,BR:B.qx,Bp:B.qx
,BQ:B.qx,Gu:function(W,Hi){var X=B._NewObject(D.G7,0);this.AG=null;X.B2=this.GetExtent(
);X.Af=W;X.E0=Hi;X.GF=this.BQ;X.GG=this.Bp;X.GH=this.BR;X.GI=this.Bq;this.AG=X;}
,GQ:function(W,Cy){var A;var Ay=this.Cc;var X=(D.G7.isPrototypeOf(A=this.AG)?A:null
);var Q=X.B2[0];var R=X.B2[1];var S=X.B2[2];var T=X.B2[3];var B1=[W[2]-W[0],W[3]-
W[1]];var As=S-Q;var Ar=T-R;if(!Cy){var CX=[(A=X.Af)[2]-A[0],A[3]-A[1]];Q=Q-X.Af[
0];R=R-X.Af[1];if(CX[0]!==B1[0]){var A9=((Ay&0x4)===0x4);var A_=((Ay&0x8)===0x8);
var Dn=((Ay&0x1)===0x1);if(!A9&&(Dn||!A_))Q=((Q*B1[0])/CX[0])|0;if(!A_&&(Dn||!A9
)){S=S-X.Af[0];S=((S*B1[0])/CX[0])|0;S=S-B1[0];}else S=S-X.Af[2];Q=Q+W[0];S=S+W[
2];if(!Dn){if(A9&&!A_)S=Q+As;else if(!A9&&A_)Q=S-As;else{Q=Q+((((S-Q)-As)/2)|0);
S=Q+As;}}}else{S=S-X.Af[2];Q=Q+W[0];S=S+W[2];}if(CX[1]!==B1[1]){var A$=((Ay&0x10
)===0x10);var A8=((Ay&0x20)===0x20);var Do=((Ay&0x2)===0x2);if(!A$&&(Do||!A8))R=((
R*B1[1])/CX[1])|0;if(!A8&&(Do||!A$)){T=T-X.Af[1];T=((T*B1[1])/CX[1])|0;T=T-B1[1];
}else T=T-X.Af[3];R=R+W[1];T=T+W[3];if(!Do){if(A$&&!A8)T=R+Ar;else if(!A$&&A8)R=
T-Ar;else{R=R+((((T-R)-Ar)/2)|0);T=R+Ar;}}}else{T=T-X.Af[3];R=R+W[1];T=T+W[3];}}
else{switch(Cy){case 3:{Q=W[0];S=Q+As;}break;case 4:{S=W[2];Q=S-As;}break;case 1:{
R=W[1];T=R+Ar;}break;case 2:{T=W[3];R=T-Ar;}break;default:;}if((Cy===3)||(Cy===4
)){var A$=((Ay&0x10)===0x10);var A8=((Ay&0x20)===0x20);var Do=((Ay&0x2)===0x2);if(
Do){R=W[1];T=W[3];}else if(A$&&!A8){R=W[1];T=R+Ar;}else if(A8&&!A$){T=W[3];R=T-Ar;
}else{R=W[1]+((((W[3]-W[1])-Ar)/2)|0);T=R+Ar;}}if((Cy===1)||(Cy===2)){var A9=((Ay&
0x4)===0x4);var A_=((Ay&0x8)===0x8);var Dn=((Ay&0x1)===0x1);if(Dn){Q=W[0];S=W[2];
}else if(A9&&!A_){Q=W[0];S=Q+As;}else if(A_&&!A9){S=W[2];Q=S-As;}else{Q=W[0]+((((
W[2]-W[0])-As)/2)|0);S=Q+As;}}}X.isEmpty=(Q>=S)||(R>=T);As=(S-Q)-1;Ar=(T-R)-1;var
D2=X.B2[0];var D3=X.B2[1];var DF=(X.B2[2]-D2)-1;var DE=(X.B2[3]-D3)-1;if(!DF)DF=
1;if(!DE)DE=1;if(((this.F&0x100)===0x100)){this.BQ=[Q+((((X.GF[0]-D2)*As)/DF)|0)
,R+((((X.GF[1]-D3)*Ar)/DE)|0)];this.Bp=[Q+((((X.GG[0]-D2)*As)/DF)|0),R+((((X.GG[
1]-D3)*Ar)/DE)|0)];this.BR=[Q+((((X.GH[0]-D2)*As)/DF)|0),R+((((X.GH[1]-D3)*Ar)/DE
)|0)];this.Bq=[Q+((((X.GI[0]-D2)*As)/DF)|0),R+((((X.GI[1]-D3)*Ar)/DE)|0)];}else{
this.G8([Q+((((X.GF[0]-D2)*As)/DF)|0),R+((((X.GF[1]-D3)*Ar)/DE)|0)]);this.G9([Q+((((
X.GG[0]-D2)*As)/DF)|0),R+((((X.GG[1]-D3)*Ar)/DE)|0)]);this.G_([Q+((((X.GH[0]-D2)
*As)/DF)|0),R+((((X.GH[1]-D3)*Ar)/DE)|0)]);this.G$([Q+((((X.GI[0]-D2)*As)/DF)|0)
,R+((((X.GI[1]-D3)*Ar)/DE)|0)]);this.AG=X;}return[As+1,Ar+1];},IH:function(aOffset
,Hh){if(Hh){this.BQ=B.tx(this.BQ,aOffset);this.Bp=B.tx(this.Bp,aOffset);this.BR=
B.tx(this.BR,aOffset);this.Bq=B.tx(this.Bq,aOffset);}else{this.G8(B.tx(this.BQ,aOffset
));this.G9(B.tx(this.Bp,aOffset));this.G_(B.tx(this.BR,aOffset));this.G$(B.tx(this.
Bq,aOffset));}},GetExtent:function(){if(!!this.AG&&this.AG.isEmpty)return A1;var
Q=this.BQ[0];var R=this.BQ[1];var S=this.BR[0];var T=this.BR[1];if((((this.Bq[0]
!==Q)||(this.Bp[1]!==R))||(this.Bp[0]!==S))||(this.Bq[1]!==T)){if(this.Bp[0]<Q)Q=
this.Bp[0];if(this.BR[0]<Q)Q=this.BR[0];if(this.Bq[0]<Q)Q=this.Bq[0];if(this.Bp[
1]<R)R=this.Bp[1];if(this.BR[1]<R)R=this.BR[1];if(this.Bq[1]<R)R=this.Bq[1];if(this.
BQ[0]>S)S=this.BQ[0];if(this.Bp[0]>S)S=this.Bp[0];if(this.Bq[0]>S)S=this.Bq[0];if(
this.BQ[1]>T)T=this.BQ[1];if(this.Bp[1]>T)T=this.Bp[1];if(this.Bq[1]>T)T=this.Bq[
1];}else{var tmp;if(S<Q){tmp=Q;Q=S;S=tmp;}if(T<R){tmp=R;R=T;T=tmp;}}return[Q,R,S+
1,T+1];},G$:function(C){var A;if(B.tl(C,this.Bq))return;if(!!this.J&&((this.F&0x1
)===0x1))this.J.Ap(this.GetExtent());this.AG=null;this.Bq=C;if(!!this.J&&((this.
F&0x1)===0x1))this.J.Ap(this.GetExtent());if((!!this.J&&((this.F&0x400)===0x400)
)&&!((this.J.F&0x2000)===0x2000)){this.F=this.F|0x800;this.J.F=this.J.F|0x4000;B.
lq([A=this.J,A.BM],this);}},G_:function(C){var A;if(B.tl(C,this.BR))return;if(!!
this.J&&((this.F&0x1)===0x1))this.J.Ap(this.GetExtent());this.AG=null;this.BR=C;
if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.GetExtent());if((!!this.J&&((this.
F&0x400)===0x400))&&!((this.J.F&0x2000)===0x2000)){this.F=this.F|0x800;this.J.F=
this.J.F|0x4000;B.lq([A=this.J,A.BM],this);}},G9:function(C){var A;if(B.tl(C,this.
Bp))return;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.GetExtent());this.AG=
null;this.Bp=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.GetExtent());if((
!!this.J&&((this.F&0x400)===0x400))&&!((this.J.F&0x2000)===0x2000)){this.F=this.
F|0x800;this.J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM],this);}},G8:function(C){var
A;if(B.tl(C,this.BQ))return;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.GetExtent(
));this.AG=null;this.BQ=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.GetExtent(
));if((!!this.J&&((this.F&0x400)===0x400))&&!((this.J.F&0x2000)===0x2000)){this.
F=this.F|0x800;this.J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM],this);}},Ir:function(
Dg){var AK=B.tA(4,B.qx,null);var I=0;var AO=3;var Jg=false;var Jh=false;AK.Set(0
,this.BQ);AK.Set(1,this.Bp);AK.Set(2,this.BR);AK.Set(3,this.Bq);while(I<4){var KE=
AK.Get(I)[0];var HM=AK.Get(I)[1];var M9=AK.Get(AO)[0];var JE=AK.Get(AO)[1];if(((
HM>Dg[1])!==(JE>Dg[1]))||((HM<Dg[1])!==(JE<Dg[1]))){var KF=((((M9-KE)*(Dg[1]-HM)
)/(JE-HM))|0)+KE;if(Dg[0]>KF)Jg=!Jg;if(Dg[0]<KF)Jh=!Jh;}AO=I;I=I+1;}return Jg||Jh;
},K5:function(){return((((this.BQ[0]===this.Bq[0])&&(this.Bp[0]===this.BR[0]))&&(
this.BQ[1]===this.Bp[1]))&&(this.BR[1]===this.Bq[1]))||((((this.BQ[0]===this.Bp[
0])&&(this.BR[0]===this.Bq[0]))&&(this.BQ[1]===this.Bq[1]))&&(this.Bp[1]===this.
BR[1]));},_Init:function(aArg){D.Ce._Init.call(this,aArg);this.__proto__=D.Ge;},
_className:"Core::QuadView"};D.A5={M:B.qy,Gu:function(W,Hi){var X=B._NewObject(D.
G6,0);X.B2=this.M;X.Af=W;X.E0=Hi;this.AG=X;},GQ:function(W,Cy){var A;var Ay=this.
Cc;var X=this.AG;var Q=X.B2[0];var R=X.B2[1];var S=X.B2[2];var T=X.B2[3];var B1=[
W[2]-W[0],W[3]-W[1]];var As=S-Q;var Ar=T-R;if(!Cy){var CX=[(A=X.Af)[2]-A[0],A[3]-
A[1]];Q=Q-X.Af[0];R=R-X.Af[1];if(CX[0]!==B1[0]){var A9=((Ay&0x4)===0x4);var A_=((
Ay&0x8)===0x8);var Dn=((Ay&0x1)===0x1);if(!A9&&(Dn||!A_))Q=((Q*B1[0])/CX[0])|0;if(
!A_&&(Dn||!A9)){S=S-X.Af[0];S=((S*B1[0])/CX[0])|0;S=S-B1[0];}else S=S-X.Af[2];Q=
Q+W[0];S=S+W[2];if(!Dn){if(A9&&!A_)S=Q+As;else if(!A9&&A_)Q=S-As;else{Q=Q+((((S-
Q)-As)/2)|0);S=Q+As;}}}else{S=S-X.Af[2];Q=Q+W[0];S=S+W[2];}if(CX[1]!==B1[1]){var
A$=((Ay&0x10)===0x10);var A8=((Ay&0x20)===0x20);var Do=((Ay&0x2)===0x2);if(!A$&&(
Do||!A8))R=((R*B1[1])/CX[1])|0;if(!A8&&(Do||!A$)){T=T-X.Af[1];T=((T*B1[1])/CX[1]
)|0;T=T-B1[1];}else T=T-X.Af[3];R=R+W[1];T=T+W[3];if(!Do){if(A$&&!A8)T=R+Ar;else
if(!A$&&A8)R=T-Ar;else{R=R+((((T-R)-Ar)/2)|0);T=R+Ar;}}}else{T=T-X.Af[3];R=R+W[1
];T=T+W[3];}}else{switch(Cy){case 3:{Q=W[0];S=Q+As;}break;case 4:{S=W[2];Q=S-As;
}break;case 1:{R=W[1];T=R+Ar;}break;case 2:{T=W[3];R=T-Ar;}break;default:;}if((Cy===
3)||(Cy===4)){var A$=((Ay&0x10)===0x10);var A8=((Ay&0x20)===0x20);var Do=((Ay&0x2
)===0x2);if(Do){R=W[1];T=W[3];}else if(A$&&!A8){R=W[1];T=R+Ar;}else if(A8&&!A$){
T=W[3];R=T-Ar;}else{R=W[1]+((((W[3]-W[1])-Ar)/2)|0);T=R+Ar;}}if((Cy===1)||(Cy===
2)){var A9=((Ay&0x4)===0x4);var A_=((Ay&0x8)===0x8);var Dn=((Ay&0x1)===0x1);if(Dn
){Q=W[0];S=W[2];}else if(A9&&!A_){Q=W[0];S=Q+As;}else if(A_&&!A9){S=W[2];Q=S-As;
}else{Q=W[0]+((((W[2]-W[0])-As)/2)|0);S=Q+As;}}}X.isEmpty=(Q>=S)||(R>=T);if(((this.
F&0x100)===0x100)){this.M=[Q,R,S,T];}else{this.Ah([Q,R,S,T]);this.AG=X;}return[S-
Q,T-R];},IH:function(aOffset,Hh){if(Hh)this.M=B.tz(this.M,aOffset);else this.Ah(
B.tz(this.M,aOffset));},GetExtent:function(){return this.M;},Ah:function(C){var A;
if(B.tm(C,this.M))return;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);this.
AG=null;this.M=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);if((!!this.
J&&((this.F&0x400)===0x400))&&!((this.J.F&0x2000)===0x2000)){this.F=this.F|0x800;
this.J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM],this);}},_Init:function(aArg){D.Ce.
_Init.call(this,aArg);this.__proto__=D.A5;},_className:"Core::RectView"};D.Ag={CB:
null,BK:null,Hx:null,Cz:null,Ek:null,EV:null,CG:null,DT:255,Ck:function(Aq,aClip
,aOffset,AC,aBlend){var A;AC=((AC+1)*this.DT)>>8;aBlend=aBlend&&((this.F&0x2)===
0x2);if(!this.Cz)this.MV(Aq,aClip,B.tx(aOffset,this.M.slice(0,2)),AC,aBlend);else{
var BH=255|(255<<8)|(255<<16)|((AC&0xFF)<<24);this.Cz.Update();Aq.Ey(aClip,this.
Cz,0,B.tz(this.M,aOffset),Aw,BH,BH,BH,BH,aBlend);}},DI:function(Am,N,Cg,Fi,EQ){var
A;var K=this.BK;var EW=null;var U=A1;var Ba=null;var Jl=!!this.EV&&(!!this.EV.I$||
!!this.EV.CB);if(((A=B.il(Am,this.M))[0]>=A[2])||(A[1]>=A[3]))return null;Am=B.ty(
Am,this.M.slice(0,2));while(!!K){if(((K.F&0x400)===0x400)&&!Ba){Ba=K.Ak;while(!!
Ba&&!((Ba.F&0x200)===0x200))Ba=Ba.Ak;if(!!Ba)U=B.il(Am,Ba.GetExtent());else U=A1;
}if(Ba===K){Ba=null;U=A1;}if((((((K.F&0x8)===0x8)&&((K.F&0x10)===0x10))&&!((K.F&
0x40000)===0x40000))&&!((K.F&0x20000)===0x20000))&&(!((K.F&0x10000)===0x10000)||((
this.Ek.Ci===K)&&!Jl))){var B2=K.GetExtent();var Hq=Fi;var ET=null;if(Hq===K)Hq=
null;if(((K.F&0x400)===0x400)){if(!(((A=B.il(B2,U))[0]>=A[2])||(A[1]>=A[3])))ET=
K.DI(U,N,Cg,Hq,EQ);}else{if(!(((A=B.il(B2,Am))[0]>=A[2])||(A[1]>=A[3]))||(Fi===K
))ET=K.DI(Am,N,Cg,Hq,EQ);}K=K.Ak;if(!!ET){if(!EW||((ET.Fu<EW.Fu)&&(ET.Fu>=0)))EW=
ET;if(!ET.Fu)K=null;}}else K=K.Ak;}return EW;},A3:function(DY,EP){var A;var M0=this.
F;D.A5.A3.call(this,DY,EP);var Ej=this.F^M0;if(!!this.CG&&((Ej&0x40)===0x40)){if(((
this.F&0x40)===0x40))this.CG.A3(0x40,0x0);else this.CG.A3(0x0,0x40);}if(!!this.Ek&&((
Ej&0x40)===0x40)){if(((this.F&0x40)===0x40)&&((this.Ek.Ci.F&0x14)===0x14))this.Ek.
Ci.A3(0x40,0x0);else this.Ek.Ci.A3(0x0,0x40);}if(!!Ej){this.F=this.F|0x8000;B.lq([
this,this.BM],this);}},Ah:function(C){var A;if(B.tm(C,this.M))return;var Ep=[(A=
this.M)[2]-A[0],A[3]-A[1]];var HB=[C[2]-C[0],C[3]-C[1]];var E2=!B.tl(Ep,HB);if(E2&&
!!this.Cz){this.Cz.IM(HB);B.qw(this,0);B.qw(this.Cz,0);}D.A5.Ah.call(this,C);if((
E2&&(Ep[0]>0))&&(Ep[1]>0)){var Af=[].concat(Aw,Ep);var K=this.CB;var Gt=0x14;while(
!!K){if((!K.AG&&(K.Cc!==Gt))&&!((K.F&0x400)===0x400))K.Gu(Af,null);K=K.AD;}}if(E2
){this.F=this.F|0x5000;B.lq([this,this.BM],this);}},Jx:function(Ai){var Ks=(D.KeyEvent.
isPrototypeOf(Ai)?Ai:null);var Da=this.Hx;if(!Ks)return null;while(!!Da&&(!Da.BC||
!Da.Bo(Ks)))Da=Da.AD;return Da;},MV:function(Aq,aClip,aOffset,AC,aBlend){var A;var
K=this.CB;var Kn=A1;var Kv=true;while(!!K){if(((K.F&0x200)===0x200)){var Ku=(D.Gb.
isPrototypeOf(K)?K:null);Kn=B.il(aClip,B.tz(Ku.M,aOffset));Kv=((Ku.F&0x1)===0x1);
}if(((K.F&0x1)===0x1)){if(((K.F&0x400)===0x400)){if(Kv){var U=B.il(B.tz(K.GetExtent(
),aOffset),Kn);if(!((U[0]>=U[2])||(U[1]>=U[3])))K.Ck(Aq,U,aOffset,AC,aBlend);}}else{
var U=B.il(B.tz(K.GetExtent(),aOffset),aClip);if(!((U[0]>=U[2])||(U[1]>=U[3])))K.
Ck(Aq,U,aOffset,AC,aBlend);}}K=K.AD;}},M2:function(){var A;var Je=((this.F&0x1000
)===0x1000);var D4=[0,0,(A=this.M)[2]-A[0],A[3]-A[1]];var C_=false;var Dk=A1;var
Bl=A1;var C$=Aw;var Fp=0;var Fq=0;var Fo=0;var B3=0;var K=this.BK;var Ba=null;var
Gt=0x14;var Em=null;while(!!K){if(((K.F&0x800)===0x800)){C_=true;K.F=K.F&~0x800;
}if(C_&&((K.F&0x200)===0x200)){C_=false;if(!!(D.Gb.isPrototypeOf(K)?K:null).G0)K.
F=K.F|0x1000;}K=K.Ak;}C_=false;K=this.CB;if(Je){this.F=this.F&~0x1000;Je=!((D4[0
]>=D4[2])||(D4[1]>=D4[3]));}this.F=this.F|0x2000;while(!!K){if(!Em&&(Fo!==B3)){var
Dq=K;var HF=0;var GL=Dk[2]-Dk[0];var Gp=Dk[3]-Dk[1];var Ho=0;var E5=Aw;do{if(((Dq.
F&0x200)===0x200))Dq=null;else if(((Dq.F&0x401)===0x401)){E5=[(A=Dq.GetExtent())[
2]-A[0],A[3]-A[1]];if((B3===3)||(B3===4))GL=GL-E5[0];if((B3===1)||(B3===2))Gp=Gp-
E5[1];if(!Em||((GL>=0)&&(Gp>=0))){Em=Dq;Dq=Dq.AD;if((B3===3)||(B3===4)){GL=GL-Fp;
if(E5[1]>HF)HF=E5[1];}if((B3===1)||(B3===2)){Gp=Gp-Fq;if(E5[0]>Ho)Ho=E5[0];}}else
Dq=null;}else Dq=Dq.AD;}while(!!Dq);if(!Em)Em=Ba;Bl=Dk;switch(Fo){case 9:case 11:
Bl=[].concat(Bl.slice(0,3),Bl[1]+HF);break;case 10:case 12:Bl=B.t3(Bl,Bl[3]-HF);
break;case 5:case 7:Bl=B.t1(Bl,Bl[0]+Ho);break;case 6:case 8:Bl=[].concat(Bl[2]-
Ho,Bl.slice(1,4));break;default:;}}if(((K.F&0x400)===0x400)){if(!!K.AG&&(K.AG.E0
!==Ba))K.AG=null;if((!K.AG&&C_)&&((K.Cc!==Gt)||!!B3))K.Gu(Bl,Ba);}if(!!K.AG){if(
Je&&!((K.F&0x400)===0x400))K.GQ(D4,0);if(C_&&((K.F&0x400)===0x400)){var Y=K.GQ(B.
tz(Bl,C$),B3);if(((K.F&0x1)===0x1)){var Cp=Aw;switch(B3){case 3:Cp=[Y[0]+Fp,Cp[1
]];break;case 4:Cp=[-Y[0]-Fp,Cp[1]];break;case 1:Cp=[Cp[0],Y[1]+Fq];break;case 2:
Cp=[Cp[0],-Y[1]-Fq];break;default:;}C$=B.tx(C$,Cp);}}}if(((K.F&0x200)===0x200)){
if(C_)B.lq(Ba.Ed,Ba);C_=((K.F&0x1000)===0x1000);Ba=(D.Gb.isPrototypeOf(K)?K:null
);if(C_){K.F=K.F&~0x1000;Dk=B.tz(Ba.M,Ba.DX);Bl=Dk;C$=Aw;Fo=Ba.G0;B3=Fo;Fp=Ba.Space+
Ba.Mq;Fq=Ba.Space+Ba.Mr;C_=!((Dk[0]>=Dk[2])||(Dk[1]>=Dk[3]));Em=null;switch(Fo){
case 9:case 10:B3=3;break;case 11:case 12:B3=4;break;case 5:case 6:B3=1;break;case
7:case 8:B3=2;break;default:;}}if(C_){this.Ap(Ba.M);}}if(K===Em){switch(Fo){case
9:case 11:C$=[0,(C$[1]+(Bl[3]-Bl[1]))+Fq];break;case 10:case 12:C$=[0,(C$[1]-(Bl[
3]-Bl[1]))-Fq];break;case 5:case 7:C$=[(C$[0]+(Bl[2]-Bl[0]))+Fp,0];break;case 6:
case 8:C$=[(C$[0]-(Bl[2]-Bl[0]))-Fp,0];break;default:;}Em=null;}K=K.AD;}if(C_)B.
lq(Ba.Ed,Ba);this.F=this.F&~0x2000;this.Hg([D4[2]-D4[0],D4[3]-D4[1]]);},BM:function(
AV){var A;var M8=((this.F&0x1000)===0x1000);if(((this.F&0x4000)===0x4000)){this.
F=this.F&~0x4000;this.M2();}if(((this.F&0x8000)===0x8000)||M8){this.F=this.F&~0x8000;
this.Gh(this.F);}},EE:function(C){var A;if(!!C&&(C.J!==this))throw new Error(BX);
if(!!C&&!((C.F&0x14)===0x14))C=null;if(!!C&&((C.F&0x10000)===0x10000))C=null;if(
C===this.CG)return;if(!!this.CG)this.CG.A3(0x0,0x60);this.CG=C;if(!!C){if(((this.
F&0x40)===0x40))C.A3(0x60,0x0);else C.A3(0x20,0x0);}},Ga:function(C){if(C)this.A3(
0x1,0x0);else this.A3(0x0,0x1);},IF:function(Dg){var tmp=this;while(!!tmp){Dg=B.
tw(Dg,tmp.M.slice(0,2));tmp=tmp.J;}return Dg;},DispatchEvent:function(Ai){var A;
var K=this.CG;var Z=(D.Ag.isPrototypeOf(K)?K:null);var At=null;var Jl=!!this.EV&&(
!!this.EV.I$||!!this.EV.CB);if(!!K&&((((K.F&0x10000)===0x10000)||((K.F&0x40000)===
0x40000))||((K.F&0x20000)===0x20000))){K=null;Z=null;}if(!!this.Ek&&!Jl)At=this.
Ek.Ci.DispatchEvent(Ai);if(!At&&!!Z)At=Z.DispatchEvent(Ai);else if(!At&&!!K)At=K.
Bo(Ai);if(!At)At=this.Bo(Ai);if(!At)At=this.Jx(Ai);return At;},BroadcastEventAtPosition:
function(Ai,Ki,aFilter){var A;var K=this.BK;var At=null;while(!!K&&!At){if((!aFilter||((
A=aFilter)&&((K.F&A)===A)))&&B.qu(K.GetExtent(),Ki)){var Z=(D.Ag.isPrototypeOf(K
)?K:null);if(!!Z)At=Z.BroadcastEventAtPosition(Ai,B.tw(Ki,Z.M.slice(0,2)),aFilter
);else At=K.Bo(Ai);}K=K.Ak;}if(!At)At=this.Bo(Ai);return At;},BroadcastEvent:function(
Ai,aFilter){var A;var K=this.BK;var At=null;while(!!K&&!At){if(!aFilter||((A=aFilter
)&&((K.F&A)===A))){var Z=(D.Ag.isPrototypeOf(K)?K:null);if(!!Z)At=Z.BroadcastEvent(
Ai,aFilter);else At=K.Bo(Ai);}K=K.Ak;}if(!At)At=this.Bo(Ai);if(!At)At=this.Jx(Ai
);return At;},Hg:function(aSize){},Gh:function(Eg){},Cb:function(){this.F=this.F|
0x8000;B.lq([this,this.BM],this);},Ap:function(Am){var A;var Z=this;while(!!Z&&!((
Am[0]>=Am[2])||(Am[1]>=Am[3]))){var ER=Z.Cz;if(!Z.J&&(Z!==this)){Z.Ap(Am);return;
}if(!!ER){var Jd=false;var MZ=ER.C3;if(Jd)ER.C3=[0,0,(A=Z.M)[2]-A[0],A[3]-A[1]];
else ER.C3=B.qR(ER.C3,Am);if(!B.tm(MZ,ER.C3)){B.qw(Z,0);B.qw(ER,0);}}if(!((Z.F&0x1
)===0x1))return;Am=B.il(B.tz(Am,Z.M.slice(0,2)),Z.M);Z=Z.J;}},BG:function(aArg){
this.Cb();},JM:function(O,aFilter){var A;if(!O||(O.J!==this))return null;var Eo=
O.AD;var Er=O.Ak;var GC=0x10000;if(((aFilter&0x10000)===0x10000))GC=0x0;while(!!
Eo||!!Er){if((!!Eo&&(!aFilter||((A=aFilter)&&((Eo.F&A)===A))))&&(!GC||!((A=GC)&&((
Eo.F&A)===A))))return Eo;if((!!Er&&(!aFilter||((A=aFilter)&&((Er.F&A)===A))))&&(
!GC||!((A=GC)&&((Er.F&A)===A))))return Er;if(!!Eo)Eo=Eo.AD;if(!!Er)Er=Er.Ak;}return null;
},Fg:function(O){var A;if(!O)throw new Error(Dy);if(O.J!==this)throw new Error(Df
);if(!O.AD)return;var DC=this.BK;var Ft=O.EJ;while(!!DC&&(DC.EJ>Ft))DC=DC.Ak;if(((
DC===O)||!DC)||(DC.AD===O))return;if(((O.F&0x401)===0x401)){if(!!O.Ak&&!!O.AG)O.
Ak.F=O.Ak.F|0x800;O.F=O.F|0x800;this.F=this.F|0x4000;B.lq([this,this.BM],this);}
if(((O.F&0x200)===0x200)){if(!!O.Ak)O.Ak.F=O.Ak.F|0x800;this.F=this.F|0x4000;B.lq([
this,this.BM],this);}if(!!O.Ak)O.Ak.AD=O.AD;else this.CB=O.AD;O.AD.Ak=O.Ak;O.Ak=
DC;O.AD=DC.AD;DC.AD=O;if(!!O.AD)O.AD.Ak=O;else this.BK=O;if(((O.F&0x1)===0x1))this.
Ap(O.GetExtent());},EH:function(O){var A;if(!O)throw new Error(Dz);if(O.J!==this
)throw new Error(Df);if((((O.F&0x401)===0x401)&&!!O.Ak)&&!!O.AG){O.Ak.F=O.Ak.F|0x800;
this.F=this.F|0x4000;B.lq([this,this.BM],this);}if(((O.F&0x200)===0x200)){if(!!O.
Ak)O.Ak.F=O.Ak.F|0x800;this.F=this.F|0x4000;B.lq([this,this.BM],this);}O.AG=null;
if(this.CG===O)this.EE(this.JM(O,0x14));if(!!O.Ak)O.Ak.AD=O.AD;if(!!O.AD)O.AD.Ak=
O.Ak;if(this.CB===O)this.CB=O.AD;if(this.BK===O)this.BK=O.Ak;O.J=null;O.AD=null;
O.Ak=null;if(((O.F&0x1)===0x1))this.Ap(O.GetExtent());},Bc:function(O,Fk){var A;
if(!O)throw new Error(DA);if(!!O.J)throw new Error(I0);var BY=null;var Ft=O.EJ;if(((
Fk<0)&&!!this.BK)&&(this.BK.EJ>=Ft)){BY=this.BK;Fk=Fk+1;}while((((Fk<0)&&!!BY)&&
!!BY.Ak)&&(BY.Ak.EJ>=Ft)){BY=BY.Ak;Fk=Fk+1;}if((!BY&&!!this.BK)&&(this.BK.EJ>Ft)
)BY=this.BK;while((!!BY&&!!BY.Ak)&&(BY.Ak.EJ>Ft))BY=BY.Ak;if(!BY){O.J=this;O.Ak=
this.BK;if(!!this.BK)this.BK.AD=O;if(!this.CB)this.CB=O;this.BK=O;}else{O.J=this;
O.Ak=BY.Ak;O.AD=BY;BY.Ak=O;if(!!O.Ak)O.Ak.AD=O;else this.CB=O;}if(((O.F&0x1)===0x1
))this.Ap(O.GetExtent());if(((!this.CG&&((O.F&0x4)===0x4))&&((O.F&0x10)===0x10))&&
!((O.F&0x10000)===0x10000))this.EE(O);if(((O.F&0x401)===0x401)){O.F=O.F|0x800;this.
F=this.F|0x4000;B.lq([this,this.BM],this);}if(((O.F&0x200)===0x200)){O.F=O.F|0x800;
this.F=this.F|0x4000;B.lq([this,this.BM],this);}},_Init:function(aArg){D.A5._Init.
call(this,aArg);this.__proto__=D.Ag;this.F=0x1F;this.BG(aArg);},_Mark:function(E
){var A;D.A5._Mark.call(this,E);if((A=this.CB)&&(A._cycle!=E))A._Mark(A._cycle=E
);if((A=this.BK)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Hx)&&(A._cycle!=E
))A._Mark(A._cycle=E);if((A=this.Cz)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.
Ek)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.EV)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.CG)&&(A._cycle!=E))A._Mark(A._cycle=E);},_className:"Core::Group"};
D.Root={B6:null,D6:null,Dh:null,AJ:B.tA(10,null,null),Fl:null,CJ:null,D0:null,GO:
0,I7:0,AM:0,Bk:B.tA(10,0,null),Hp:B.tA(10,B.qy,null),CQ:B.tA(10,0,null),DD:B.tA(
10,B.qx,null),Fn:B.tA(10,0,null),DZ:B.tA(10,B.qx,null),CP:B.tA(10,B.qx,null),Ch:
B.tA(10,B.qx,null),Di:B.tA(10,B.qx,null),An:0,Ht:0,Hs:0,Hz:B.tA(3,B.qy,null),Kt:
0,BL:B.tA(4,0,null),Bi:B.tA(4,B.qy,null),Be:0,D$:8,KO:250,El:0,D5:0,Jf:true,Hy:false
,FL:function(){return this;},Ck:function(Aq,aClip,aOffset,AC,aBlend){var fullScreenUpdate=
false;fullScreenUpdate=B.jI;if(!fullScreenUpdate)Aq.FK(aClip,B.tz(B.tz(aClip,aOffset
),this.M.slice(0,2)),0x00000000,0x00000000,0x00000000,0x00000000,false);D.Ag.Ck.
call(this,Aq,aClip,aOffset,AC,aBlend);},A3:function(DY,EP){var A;D.Ag.A3.call(this
,DY,EP);if(!this.J&&(((DY&0x1)===0x1)||((EP&0x1)===0x1)))this.Ap([0,0,(A=this.M)[
2]-A[0],A[3]-A[1]]);if(!this.J&&(((DY&0x2)===0x2)||((EP&0x2)===0x2)))this.Ap([0,
0,(A=this.M)[2]-A[0],A[3]-A[1]]);},EE:function(C){if((C!==this.CJ)||!C)D.Ag.EE.call(
this,C);},DispatchEvent:function(Ai){if((this.I7>0)&&!!(D.KeyEvent.isPrototypeOf(
Ai)?Ai:null))return null;if(!!Ai){Ai.FS=!!this.AM;if(!!this.AM)Ai.AE=this.AM;}var
At=null;if(!!this.CJ){At=this.CJ.DispatchEvent(Ai);if(!!At){this.AM=0;return At;
}}if(!!this.D6){At=this.D6.Ci.DispatchEvent(Ai);if(!At)At=this.Bo(Ai);if(!At)At=
this.Jx(Ai);this.AM=0;return At;}At=D.Ag.DispatchEvent.call(this,Ai);this.AM=0;return At;
},BroadcastEvent:function(Ai,aFilter){if(!!Ai){Ai.FS=!!this.AM;if(!!this.AM)Ai.AE=
this.AM;}var At=D.Ag.BroadcastEvent.call(this,Ai,aFilter);this.AM=0;return At;},
Ap:function(Am){var A;if(this.GO>0)throw new Error(I1);if(!!this.Cz&&!this.J){if(((
A=this.Cz.C3)[0]>=A[2])||(A[1]>=A[3])){B.qw(this,0);B.qw(this.Cz,0);}var Jd=false;
if(Jd)this.Cz.C3=[0,0,(A=this.M)[2]-A[0],A[3]-A[1]];else this.Cz.C3=B.qR(this.Cz.
C3,Am);}var fullScreenUpdate=false;fullScreenUpdate=B.jI;if(fullScreenUpdate)Am=[
0,0,(A=this.M)[2]-A[0],A[3]-A[1]];if(!!this.J){D.Ag.Ap.call(this,Am);return;}Am=
B.il(B.tz(Am,this.M.slice(0,2)),this.M);if((Am[0]>=Am[2])||(Am[1]>=Am[3]))return;
var I;for(I=0;I<this.Be;I=I+1)if(!(((A=B.il(this.Bi.Get(I),Am))[0]>=A[2])||(A[1]>=
A[3]))){this.Bi.Set(I,B.qR(this.Bi.Get(I),Am));this.BL.Set(I,B.s9(this.Bi.Get(I)
));return;}if(this.Be<3){this.Bi.Set(this.Be,Am);this.BL.Set(this.Be,B.s9(Am));this.
Be=this.Be+1;return;}var AO;var B5;var Gw=0;var Gx=0;var Kk=2147483647;this.Bi.Set(
this.Be,Am);this.BL.Set(this.Be,B.s9(Am));for(AO=0;AO<=this.Be;AO=AO+1)for(B5=AO+
1;B5<=this.Be;B5=B5+1){var HL=B.s9(B.qR(this.Bi.Get(AO),this.Bi.Get(B5)));var Kw=((
HL<<8)/(this.BL.Get(AO)+this.BL.Get(B5)))|0;if(Kw<Kk){Kk=Kw;Gw=AO;Gx=B5;}}this.Bi.
Set(Gw,B.qR(this.Bi.Get(Gw),this.Bi.Get(Gx)));this.BL.Set(Gw,B.s9(this.Bi.Get(Gw
)));if(Gx!==this.Be){this.Bi.Set(Gx,this.Bi.Get(this.Be));this.BL.Set(Gx,this.BL.
Get(this.Be));}},MT:function(){var BA=B._NewObject(D.GW,0);BA.FS=!!this.AM;if(!!
this.AM)BA.AE=this.AM;return BA;},ES:function(){var BA=B._NewObject(D.GT,0);BA.FS=
!!this.AM;if(!!this.AM)BA.AE=this.AM;return BA;},Fm:function(){var BA=B._NewObject(
D.GU,0);BA.FS=!!this.AM;if(!!this.AM)BA.AE=this.AM;return BA;},MU:function(AV){var
I;var EW=false;for(I=0;I<10;I=I+1)if(!!this.AJ.Get(I)){var Au=this.Ch.Get(I);var
Z=this.AJ.Get(I).J;while(!!Z&&(Z!==this)){Au=B.tw(Au,Z.M.slice(0,2));Z=Z.J;}if(!
Z&&(this.AJ.Get(I)!==this)){var tmp=this.AJ.Get(I);this.An=I;this.AJ.Set(I,null);
tmp.Bo(this.ES().InitializeUp(I,this.DZ.Get(I),this.DD.Get(I),this.CQ.Get(I),this.
Bk.Get(I)+1,this.CP.Get(I),false,this.Ch.Get(I),this.Di.Get(I)));if(tmp===this.Dh
)this.Dh=null;this.BroadcastEvent(this.Fm().InitializeUp(I,this.Bk.Get(I)+1,false
,tmp,this.Ch.Get(I)),0x18);}else{this.CQ.Set(I,(this.D0.AE-this.Fn.Get(I))|0);if(
this.CQ.Get(I)<10)this.CQ.Set(I,10);this.An=I;this.AJ.Get(I).Bo(this.ES().InitializeHold(
I,Au,this.DD.Get(I),this.CQ.Get(I),this.Bk.Get(I)+1,this.CP.Get(I),this.Ch.Get(I
),this.Di.Get(I)));EW=true;}}if(!EW)this.D0.Bv(false);},GetFPS:function(){var ticksCount=
0;var Kr=0;ticksCount=((new Date).getTime()-B.qt)|0;if(!!this.Ht&&(ticksCount>this.
Ht))Kr=((this.Hs*1000)/((ticksCount-this.Ht)|0))|0;this.Hs=0;this.Ht=ticksCount;
return Kr;},Update:function(){var A;if(!this.Fl){this.Fl=B._NewObject(B.Graphics.
Canvas,0);this.Fl.IM([(A=this.M)[2]-A[0],A[3]-A[1]]);}this.Fl.Update();return this.
UpdateGE20(this.Fl);},UpdateGE20:function(Aq){if(!this.BeginUpdate())return A1;var
DG=this.UpdateCanvas(Aq,Aw);this.EndUpdate();return DG;},EndUpdate:function(){if(
this.Be>0){this.Hs=this.Hs+1;this.Be=0;}},UpdateCanvas:function(Aq,aOffset){var A;
var DG=A1;var MR=[].concat(aOffset,B.tx(Aq.FrameSize,aOffset));var I;var AO=this.
Be;this.GO=this.GO+1;for(I=0;(I<AO)&&(I<4);I=I+1){if(this.BL.Get(I)>0){this.Ck(Aq
,B.ty(this.Bi.Get(I),aOffset),[-aOffset[0],-aOffset[1]],255,true);DG=B.qR(DG,B.il(
MR,this.Bi.Get(I)));}else AO=AO+1;}this.GO=this.GO-1;if(!((DG[0]>=DG[2])||(DG[1]>=
DG[3])))return B.ty(DG,aOffset);else return DG;},GetUpdateRegion:function(Hj){var
I;var AO=this.Be;if(Hj<0)return A1;for(I=0;(I<AO)&&(I<4);I=I+1){if(!this.BL.Get(
I)){AO=AO+1;Hj=Hj+1;}else if(I===Hj)return this.Bi.Get(I);}return A1;},BeginUpdate:
function(){var M1=true;var fullScreenUpdate=false;var I;if((!M1&&!fullScreenUpdate
)&&(this.Be>0)){var KC=B.tA(3,B.qy,null);var JC=this.Be;for(I=0;I<JC;I=I+1)KC.Set(
I,this.Bi.Get(I));for(I=0;I<this.Kt;I=I+1)this.Ap(this.Hz.Get(I));for(I=0;I<JC;I=
I+1)this.Hz.Set(I,KC.Get(I));this.Kt=JC;}var AO;var B5;for(AO=0;AO<(this.Be-1);AO=
AO+1)if(this.BL.Get(AO)>0)for(B5=AO+1;B5<this.Be;B5=B5+1)if(this.BL.Get(B5)>0){var
HL=B.s9(B.qR(this.Bi.Get(AO),this.Bi.Get(B5)));if(((HL-this.BL.Get(AO))-this.BL.
Get(B5))<0){this.Bi.Set(AO,B.qR(this.Bi.Get(AO),this.Bi.Get(B5)));this.BL.Set(AO
,HL);this.BL.Set(B5,0);}}for(I=this.Be-1;I>=0;I=I-1)if(!this.BL.Get(I))this.Be=this.
Be-1;return this.Be;},DoesNeedUpdate:function(){if(this.Be>0)return true;return false;
},Initialize:function(aSize){this.Ah([].concat(Aw,aSize));if(this.Jf)this.F=this.
F|0x60;else this.F=this.F|0x20;this.Ap(this.M);return this;},SetRootFocus:function(
I4){if(I4===this.Jf)return false;this.Jf=I4;if(!I4){if(!!this.CJ)this.CJ.A3(0x0,
0x40);if(!!this.D6)this.D6.Ci.A3(0x0,0x40);else this.A3(0x0,0x40);}else{if(!!this.
D6)this.D6.Ci.A3(0x40,0x0);else this.A3(0x40,0x0);if(!!this.CJ)this.CJ.A3(0x40,0x0
);}return true;},SetUserInputTimestamp:function(MQ){this.AM=MQ;},DriveKeyboardHitting:
function(A6,EO,Cf){var A;var Jy=!!this.B6;if(!!this.B6&&((!Cf||(this.El!==A6))||(
this.D5!==EO))){var BA=null;var K=(D.Ce.isPrototypeOf(A=this.B6)?A:null);var Da=(
D.E_.isPrototypeOf(A=this.B6)?A:null);if(!!this.El)BA=B._NewObject(D.KeyEvent,0).
Initialize(this.El,false);if(this.D5!==0x00)BA=B._NewObject(D.KeyEvent,0).Initialize2(
this.D5,false);if(!!Da)Da.Bo(BA);else if(!!K)K.Bo(BA);this.El=0;this.D5=0x00;this.
B6=null;}if(!!this.B6){var BA=null;var K=(D.Ce.isPrototypeOf(A=this.B6)?A:null);
var Da=(D.E_.isPrototypeOf(A=this.B6)?A:null);if(!!A6)BA=B._NewObject(D.KeyEvent
,0).Initialize(A6,true);if(this.D5!==0x00)BA=B._NewObject(D.KeyEvent,0).Initialize2(
EO,true);if(!!Da)Da.Bo(BA);else if(!!K)K.Bo(BA);}if(this.Hy&&((!Cf||(this.El!==A6
))||(this.D5!==EO))){this.El=0;this.D5=0x00;this.Hy=false;}if((!this.B6&&Cf)&&(this.
I7>0)){this.El=A6;this.D5=EO;this.Hy=true;}if((!this.B6&&Cf)&&!this.Hy){if(!!A6)
this.B6=this.DispatchEvent(B._NewObject(D.KeyEvent,0).Initialize(A6,true));if(EO
!==0x00)this.B6=this.DispatchEvent(B._NewObject(D.KeyEvent,0).Initialize2(EO,true
));if(!(D.E_.isPrototypeOf(A=this.B6)?A:null)&&!(D.Ce.isPrototypeOf(A=this.B6)?A:
null))this.B6=null;this.El=A6;this.D5=EO;Jy=Jy||!!this.B6;}this.AM=0;return Jy;}
,DriveCursorMovement:function(A7){return this.DriveMultiTouchMovement(this.An,A7
);},DriveMultiTouchMovement:function(N,A7){if((N<0)||(N>9)){this.AM=0;return false;
}var Co=B.tw(A7,this.Ch.Get(N));this.Ch.Set(N,A7);if(!this.AJ.Get(N)||B.tl(Co,Aw
)){this.AM=0;return false;}var Au=A7;var Z=this.AJ.Get(N).J;while(!!Z&&(Z!==this
)){Au=B.tw(Au,Z.M.slice(0,2));Z=Z.J;}if(!Z&&(this.AJ.Get(N)!==this)){var tmp=this.
AJ.Get(N);this.An=N;this.AJ.Set(N,null);tmp.Bo(this.ES().InitializeUp(N,this.DZ.
Get(N),this.DD.Get(N),this.CQ.Get(N),this.Bk.Get(N)+1,this.CP.Get(N),false,this.
Ch.Get(N),this.Di.Get(N)));if(tmp===this.Dh)this.Dh=null;this.BroadcastEvent(this.
Fm().InitializeUp(N,this.Bk.Get(N)+1,false,tmp,A7),0x18);}else{this.DZ.Set(N,Au);
this.An=N;this.AJ.Get(N).Bo(this.MT().Initialize(N,Au,this.DD.Get(N),Co,this.CQ.
Get(N),this.Bk.Get(N)+1,this.CP.Get(N),A7,this.Di.Get(N)));}this.AM=0;return true;
},DriveCursorHitting:function(Cf,N,A7){return this.DriveMultiTouchHitting(Cf,N,A7
);},DriveMultiTouchHitting:function(Cf,N,A7){var A;if((N<0)||(N>9)){this.AM=0;return false;
}var ticksCount=this.AM;var EU=[].concat([-this.D$,-this.D$],[this.D$+1,this.D$+
1]);if(!ticksCount){ticksCount=((new Date).getTime()-B.qt)|0;}var M7=this.AM;this.
DriveMultiTouchMovement(N,A7);A7=this.Ch.Get(N);this.AM=M7;if(Cf)this.Di.Set(N,A7
);if((Cf&&!this.AJ.Get(N))&&!this.I7){var B4=null;var Au=A7;if(B.qu(this.Hp.Get(
N),A7)&&((ticksCount-this.Fn.Get(N))<=(((A=this.KO)<0)?A+0x100000000:A)))this.Bk.
Set(N,this.Bk.Get(N)+1);else this.Bk.Set(N,0);this.Hp.Set(N,B.tz(EU,A7));this.Fn.
Set(N,ticksCount);if((!!this.CJ&&!!this.CJ.J)&&((this.CJ.F&0x18)===0x18)){var U=
B.tz(EU,this.CJ.J.IF(A7));B4=this.CJ.DI(U,N,this.Bk.Get(N)+1,null,0x0);}if(!B4){
if(!!this.Dh&&!!this.Dh.J){if(((this.Dh.F&0x8)===0x8)&&((this.Dh.F&0x10)===0x10)
){var U=B.tz(EU,this.Dh.J.IF(A7));B4=this.Dh.DI(U,N,this.Bk.Get(N)+1,null,0x0);}
}else if(!!this.D6)B4=this.DI(B.tz(EU,A7),N,this.Bk.Get(N)+1,this.D6.Ci,0x0);else
B4=this.DI(B.tz(EU,A7),N,this.Bk.Get(N)+1,null,0x0);}if(!!B4){this.BroadcastEvent(
this.Fm().InitializeDown(N,this.Bk.Get(N)+1,false,B4.Ce,A7),0x18);this.AJ.Set(N,
B4.Ce);this.CP.Set(N,B4.DR);}else{this.AJ.Set(N,null);this.CP.Set(N,Aw);this.AM=
0;return false;}var Z=B4.Ce.J;while(!!Z&&(Z!==this)){Au=B.tw(Au,Z.M.slice(0,2));
Z=Z.J;}this.DD.Set(N,Au);this.DZ.Set(N,Au);this.CQ.Set(N,0);this.D0.Bv(true);this.
An=N;this.AJ.Get(N).Bo(this.ES().InitializeDown(N,Au,this.Bk.Get(N)+1,this.CP.Get(
N),false,A7));this.AM=0;return true;}if(!Cf&&!!this.AJ.Get(N)){var Au=A7;var Z=this.
AJ.Get(N).J;while(!!Z&&(Z!==this)){Au=B.tw(Au,Z.M.slice(0,2));Z=Z.J;}if(!Z)Au=this.
DZ.Get(N);this.An=N;var tmp=this.AJ.Get(N);this.AJ.Set(N,null);tmp.Bo(this.ES().
InitializeUp(N,Au,this.DD.Get(N),this.CQ.Get(N),this.Bk.Get(N)+1,this.CP.Get(N),
false,A7,this.Di.Get(N)));this.BroadcastEvent(this.Fm().InitializeUp(N,this.Bk.Get(
N)+1,false,tmp,A7),0x18);this.AM=0;return true;}this.AM=0;return false;},Ka:function(
DB,Kh,EQ){var EU=[].concat([-this.D$,-this.D$],[this.D$+1,this.D$+1]);if(DB===this
)DB=null;if(!this.AJ.Get(this.An))return;var B4;B4=this.DI(B.tz(EU,this.Ch.Get(this.
An)),this.An,1,DB,EQ);if(!!B4&&(this.AJ.Get(this.An)!==B4.Ce))this.JH(B4.Ce,B4.DR
);if(!B4&&(this.AJ.Get(this.An)!==Kh))this.JH(Kh,Aw);},JH:function(DB,CM){if(!this.
AJ.Get(this.An)||(this.AJ.Get(this.An)===DB))return;var tmp=this.AJ.Get(this.An);
this.AJ.Set(this.An,null);tmp.Bo(this.ES().InitializeUp(this.An,this.DZ.Get(this.
An),this.DD.Get(this.An),this.CQ.Get(this.An),this.Bk.Get(this.An)+1,this.CP.Get(
this.An),true,this.Ch.Get(this.An),this.Di.Get(this.An)));this.BroadcastEvent(this.
Fm().InitializeUp(this.An,this.Bk.Get(this.An)+1,true,tmp,this.Ch.Get(this.An)),
0x18);var Au=this.Ch.Get(this.An);var Z=null;if(!!DB)Z=DB.J;while(!!Z&&(Z!==this
)){Au=B.tw(Au,Z.M.slice(0,2));Z=Z.J;}if(!Z&&(DB!==this)){this.AJ.Set(this.An,null
);return;}this.BroadcastEvent(this.Fm().InitializeDown(this.An,this.Bk.Get(this.
An)+1,true,DB,this.Ch.Get(this.An)),0x18);var ticksCount=0;ticksCount=((new Date
).getTime()-B.qt)|0;this.AJ.Set(this.An,DB);this.CP.Set(this.An,CM);this.DD.Set(
this.An,Au);this.DZ.Set(this.An,Au);this.Bk.Set(this.An,0);this.CQ.Set(this.An,0
);this.Fn.Set(this.An,ticksCount);this.Di.Set(this.An,this.Ch.Get(this.An));this.
AJ.Get(this.An).Bo(this.ES().InitializeDown(this.An,Au,this.Bk.Get(this.An)+1,this.
CP.Get(this.An),true,this.Di.Get(this.An)));},_Init:function(aArg){D.Ag._Init.call(
this,aArg);D.Timer._Init.call(this.D0={Ad:this},0);(this.AJ=[]).__proto__=D.Root.
AJ;(this.Bk=[]).__proto__=D.Root.Bk;(this.Hp=[]).__proto__=D.Root.Hp;(this.CQ=[]
).__proto__=D.Root.CQ;(this.DD=[]).__proto__=D.Root.DD;(this.Fn=[]).__proto__=D.
Root.Fn;(this.DZ=[]).__proto__=D.Root.DZ;(this.CP=[]).__proto__=D.Root.CP;(this.
Ch=[]).__proto__=D.Root.Ch;(this.Di=[]).__proto__=D.Root.Di;(this.Hz=[]).__proto__=
D.Root.Hz;(this.BL=[]).__proto__=D.Root.BL;(this.Bi=[]).__proto__=D.Root.Bi;this.
__proto__=D.Root;this.F=0x7F;this.D0.F$(50);this.D0.EG=[this,this.MU];},_Done:function(
){this.__proto__=D.Ag;this.D0._Done();D.Ag._Done.call(this);},_ReInit:function(){
D.Ag._ReInit.call(this);this.D0._ReInit();},_Mark:function(E){var A;D.Ag._Mark.call(
this,E);if((A=this.B6)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.D6)&&(A._cycle
!=E))A._Mark(A._cycle=E);if((A=this.Dh)&&(A._cycle!=E))A._Mark(A._cycle=E);B.ts(
this.AJ,E);if((A=this.Fl)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.CJ)&&(A.
_cycle!=E))A._Mark(A._cycle=E);if((A=this.D0)._cycle!=E)A._Mark(A._cycle=E);},_className:
"Core::Root"};D.Event={AE:0,FS:false,G2:function(){var ticksCount=0;ticksCount=((
new Date).getTime()-B.qt)|0;return ticksCount;},BG:function(aArg){this.AE=this.G2(
);},_Init:function(aArg){this.__proto__=D.Event;this.BG(aArg);B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:null
,_className:"Core::Event"};D.KeyEvent={Bd:0,Av:0,Down:false,Initialize2:function(
A6,Cf){this.Bd=0;this.Av=A6;this.Down=Cf;if((A6>=0x30)&&(A6<=0x39))this.Bd=(10+A6
)-48;if((A6>=0x41)&&(A6<=0x5A))this.Bd=(105+A6)-65;if((A6>=0x61)&&(A6<=0x7A))this.
Bd=(105+A6)-97;if(A6===0x20)this.Bd=131;if(!this.Bd)switch(A6){case 0x2B:this.Bd=
132;break;case 0x2D:this.Bd=133;break;case 0x2A:this.Bd=134;break;case 0x2F:this.
Bd=135;break;case 0x3D:this.Bd=136;break;case 0x2E:this.Bd=137;break;case 0x2C:this.
Bd=138;break;case 0x3A:this.Bd=139;break;case 0x3B:this.Bd=140;break;default:;}return this;
},Initialize:function(A6,Cf){this.Bd=A6;this.Down=Cf;this.Av=0x00;var I9=A6-10;var
I8=A6-105;if((I9>=0)&&(I9<=9))this.Av=(48+I9)&0xFFFF;if((I8>=0)&&(I8<=25))this.Av=(
65+I8)&0xFFFF;if(A6===131)this.Av=0x20;if(this.Av===0x00)switch(A6){case 132:this.
Av=0x2B;break;case 133:this.Av=0x2D;break;case 134:this.Av=0x2A;break;case 135:this.
Av=0x2F;break;case 136:this.Av=0x3D;break;case 137:this.Av=0x2E;break;case 138:this.
Av=0x2C;break;case 139:this.Av=0x3A;break;case 140:this.Av=0x3B;break;default:;}
return this;},K9:function(Kg){switch(Kg){case 141:return((this.Av>=0x41)&&(this.
Av<=0x5A))||((this.Av>=0x61)&&(this.Av<=0x7A));case 142:return(((this.Av>=0x41)&&(
this.Av<=0x5A))||((this.Av>=0x61)&&(this.Av<=0x7A)))||((this.Av>=0x30)&&(this.Av<=
0x39));case 143:return(this.Av>=0x30)&&(this.Av<=0x39);case 144:return(((this.Av>=
0x41)&&(this.Av<=0x46))||((this.Av>=0x61)&&(this.Av<=0x66)))||((this.Av>=0x30)&&(
this.Av<=0x39));case 145:return this.Av!==0x00;case 146:return(this.Av===0x00)&&
!!this.Bd;case 147:return(((this.Bd===6)||(this.Bd===7))||(this.Bd===4))||(this.
Bd===5);case 148:return(this.Av!==0x00)||!!this.Bd;default:;}return Kg===this.Bd;
},_Init:function(aArg){D.Event._Init.call(this,aArg);this.__proto__=D.KeyEvent;}
,_className:"Core::KeyEvent"};D.GU={Hd:null,CH:B.qx,CI:0,BO:0,Down:false,C0:false
,InitializeUp:function(N,Cg,EN,I6,C9){this.Down=false;this.BO=N;this.CI=Cg;this.
CH=C9;this.Hd=I6;this.C0=EN;return this;},InitializeDown:function(N,Cg,EN,I6,C9){
this.Down=true;this.BO=N;this.CI=Cg;this.CH=C9;this.Hd=I6;this.C0=EN;return this;
},_Init:function(aArg){D.Event._Init.call(this,aArg);this.__proto__=D.GU;},_Mark:
function(E){var A;D.Event._Mark.call(this,E);if((A=this.Hd)&&(A._cycle!=E))A._Mark(
A._cycle=E);},_className:"Core::CursorGrabEvent"};D.GT={DP:B.qx,CH:B.qx,CI:0,Bu:
0,Du:B.qx,C1:B.qx,BO:0,Down:false,C0:false,InitializeHold:function(N,Ef,Gk,Gl,Cg
,CM,C9,Gj){this.Down=true;this.BO=N;this.C1=B.tx(Ef,CM);this.Du=B.tx(Gk,CM);this.
Bu=Gl;this.CI=Cg;this.CH=C9;this.DP=Gj;return this;},InitializeUp:function(N,Ef,
Gk,Gl,Cg,CM,EN,C9,Gj){this.Down=false;this.BO=N;this.C1=B.tx(Ef,CM);this.Du=B.tx(
Gk,CM);this.Bu=Gl;this.CI=Cg;this.C0=EN;this.CH=C9;this.DP=Gj;return this;},InitializeDown:
function(N,Ef,Cg,CM,EN,C9){this.Down=true;this.BO=N;this.C1=B.tx(Ef,CM);this.Du=
B.tx(Ef,CM);this.Bu=0;this.CI=Cg;this.C0=EN;this.CH=C9;this.DP=C9;return this;},
_Init:function(aArg){D.Event._Init.call(this,aArg);this.__proto__=D.GT;},_className:
"Core::CursorEvent"};D.GW={DP:B.qx,CH:B.qx,CI:0,Bu:0,DR:B.qx,Du:B.qx,C1:B.qx,BO:
0,Initialize:function(N,Ef,Gk,aOffset,Gl,MP,CM,C9,Gj){this.BO=N;this.C1=B.tx(Ef,
CM);this.Du=B.tx(Gk,CM);this.DR=aOffset;this.Bu=Gl;this.CI=MP;this.CH=C9;this.DP=
Gj;return this;},_Init:function(aArg){D.Event._Init.call(this,aArg);this.__proto__=
D.GW;},_className:"Core::DragEvent"};D.Gb={Ed:null,DX:B.qx,Mr:0,Mq:0,Space:0,G0:
0,Ck:function(Aq,aClip,aOffset,AC,aBlend){},Ah:function(C){var A;if(B.tm(C,this.
M))return;var Ep=[(A=this.M)[2]-A[0],A[3]-A[1]];var HB=[C[2]-C[0],C[3]-C[1]];var
E2=!B.tl(Ep,HB);var Co=B.tw(C.slice(0,2),this.M.slice(0,2));if(!B.tl(Co,Aw)&&!E2
){var K=this.AD;while(!!K&&!((K.F&0x200)===0x200)){if(((K.F&0x400)===0x400)){var
tmp=((K.F&0x100)===0x100);K.IH(Co,tmp);}K=K.AD;}B.lq(this.Ed,this);}if((E2&&(Ep[
0]>0))&&(Ep[1]>0)){var Af=B.tz(this.M,this.DX);var K=this.AD;var Gt=0x14;while(!
!K&&!((K.F&0x200)===0x200)){if(((K.F&0x400)===0x400)){if(!!K.AG&&(K.AG.E0!==this
))K.AG=null;if(!K.AG&&((K.Cc!==Gt)||!!this.G0))K.Gu(Af,this);}K=K.AD;}B.lq(this.
Ed,this);}D.A5.Ah.call(this,C);if(!!this.J&&E2){this.F=this.F|0x1000;if(!((this.
J.F&0x2000)===0x2000)){this.J.F=this.J.F|0x4000;B.lq([A=this.J,A.BM],this);}}},_Init:
function(aArg){D.A5._Init.call(this,aArg);this.__proto__=D.Gb;this.F=0x203;},_Mark:
function(E){var A;D.A5._Mark.call(this,E);if((A=this.Ed)&&((A=A[0])._cycle!=E))A.
_Mark(A._cycle=E);},_className:"Core::Outline"};D.Hc={J3:null,C5:null,C4:null,F_:
null,BW:null,BV:null,HK:0,AH:0,BO:0,AE:0,CI:0,Bu:0,DR:B.qx,Du:B.qx,C1:B.qx,Mn:1000
,Gf:8,Ee:0,J1:1,IE:-1,J0:1,K_:1,HR:false,Down:false,Dv:false,C0:false,Ck:function(
Aq,aClip,aOffset,AC,aBlend){},Bo:function(Ai){var A;var AF=(D.GT.isPrototypeOf(Ai
)?Ai:null);var Bg=(D.GW.isPrototypeOf(Ai)?Ai:null);var Hu=this.Dv;var HC;var Fr;
var KB;var D8;var Ko;if(!AF&&!Bg)return null;HC=(!!AF&&AF.Down)&&!AF.Bu;Fr=(!!AF&&
AF.Down)&&(AF.Bu>0);KB=(!!AF&&AF.Down)&&(AF.Bu>50);D8=!!AF&&!AF.Down;Ko=!!Bg;if(
!this.HR){if((((this.Ee&0x20)===0x20)&&(this.AH>0))&&(this.AH<33554432)){var Hr=(
D.GU.isPrototypeOf(Ai)?Ai:null);if(((!!Hr&&Hr.Down)&&(Hr.Hd!==this))&&B.qu(this.
GetExtent(),this.J.IF(Hr.CH))){this.HK=0x20;this.AH=this.AH|67108864;return null;
}}if(HC){var Jn=0;var Gs;this.AH=this.AH|(1<<AF.BO);for(Gs=this.AH&4095;Gs>0;Gs=
Gs>>1)if(!!(Gs&1))Jn=Jn+1;if(Jn===this.J1)this.AH=(this.AH|16777216)|(4096<<AF.BO
);}if(D8)this.AH=(this.AH&~(1<<AF.BO))|33554432;if(KB&&(this.AH<16777216))this.AH=
this.AH|67108864;if(D8&&AF.C0)this.AH=this.AH|67108864;if(D8&&!(this.AH&4095))this.
HK=0x0;if(D8&&!(this.AH&16777215))this.AH=0;if(Fr&&(this.AH>=67108864))this.FL().
Ka(null,null,this.HK);if((Fr&&!!(this.AH&16777216))&&!!(this.AH&33554432)){Fr=false;
D8=true;}if(!!AF&&!(this.AH&(4096<<AF.BO)))return this;if(!!Bg&&!(this.AH&(4096<<
Bg.BO)))return this;if(D8&&!(this.AH&16777216))return this;if(((HC||Ko)||Fr)&&((
this.AH<16777216)||(this.AH>=33554432)))return this;if(D8)this.AH=this.AH&3758100479;
if(D8&&!(this.AH&16777215))this.AH=0;}if(!!AF){this.Down=HC||Fr;this.Dv=this.Ir(
AF.C1);this.Du=AF.Du;this.C1=AF.C1;this.DR=Aw;this.Bu=AF.Bu;this.CI=AF.CI;this.C0=
AF.C0;this.BO=AF.BO;this.AE=AF.AE;if(AF.Down&&!AF.Bu)Hu=false;}if(!!Bg){this.Down=
true;this.Dv=this.Ir(Bg.C1);this.Du=Bg.Du;this.C1=Bg.C1;this.DR=Bg.DR;this.Bu=Bg.
Bu;this.CI=Bg.CI;this.BO=Bg.BO;this.C0=false;this.AE=Bg.AE;(A=this.J3)?A[1].call(
A[0],this):null;}if((!!AF&&this.Down)&&!this.Bu)(A=this.BV)?A[1].call(A[0],this):
null;if((!!AF&&this.Down)&&(this.Bu>0))(A=this.F_)?A[1].call(A[0],this):null;if((
this.Down&&this.Dv)&&!Hu)(A=this.C4)?A[1].call(A[0],this):null;if(((this.Down&&!
this.Dv)&&Hu)||((!this.Down&&this.Dv)&&Hu))(A=this.C5)?A[1].call(A[0],this):null;
if(!!AF&&!this.Down)(A=this.BW)?A[1].call(A[0],this):null;if(!!this.Ee){var Es=0x0;
if(((((this.Ee&0x10)===0x10)&&!!AF)&&AF.Down)&&(AF.Bu>=this.Mn))Es=0x10;if((((this.
Ee&0x1)===0x1)&&!!Bg)&&((Bg.CH[1]-Bg.DP[1])<=-this.Gf))Es=0x1;if((((this.Ee&0x2)===
0x2)&&!!Bg)&&((Bg.CH[1]-Bg.DP[1])>=this.Gf))Es=0x2;if((((this.Ee&0x4)===0x4)&&!!
Bg)&&((Bg.CH[0]-Bg.DP[0])<=-this.Gf))Es=0x4;if((((this.Ee&0x8)===0x8)&&!!Bg)&&((
Bg.CH[0]-Bg.DP[0])>=this.Gf))Es=0x8;if(!!Es){this.HK=Es;this.FL().Ka(null,this,Es
);}}return this;},DI:function(Am,N,Cg,Fi,EQ){var A;if(!!Fi&&(Fi!==this))return null;
if((Cg<this.K_)||(Cg>this.J0))return null;if((this.IE>=0)&&(this.J1>1))return null;
if((this.IE>=0)&&(N!==this.IE))return null;if(!this.HR&&(this.AH>=33554432))return null;
if((!this.HR&&(this.AH>=16777216))&&!(this.AH&(4096<<N)))return null;if(!!(EQ&this.
Ee))return null;if(this.K5()){var L=B.il(Am,this.GetExtent());if(!((L[0]>=L[2])||(
L[1]>=L[3]))){var Ei=B.s_(Am);var Co=Aw;if(Ei[0]<L[0])Co=[L[0]-Ei[0],Co[1]];if(Ei[
0]>=L[2])Co=[(L[2]-Ei[0])-1,Co[1]];if(Ei[1]<L[1])Co=[Co[0],L[1]-Ei[1]];if(Ei[1]>=
L[3])Co=[Co[0],(L[3]-Ei[1])-1];return B._NewObject(D.HO,0).Initialize(this,Co);}
}else{var AK=B.tA(9,B.qx,null);var I;AK.Set(0,B.s_(Am));AK.Set(1,AK.Get(0));AK.Set(
2,AK.Get(0));AK.Set(3,AK.Get(0));AK.Set(4,AK.Get(0));AK.Set(1,[Am[0],AK.Get(1)[1
]]);AK.Set(2,[AK.Get(2)[0],Am[1]]);AK.Set(3,[Am[2],AK.Get(3)[1]]);AK.Set(4,[AK.Get(
4)[0],Am[3]]);AK.Set(5,Am.slice(0,2));AK.Set(6,[Am[2],Am[1]]);AK.Set(7,[Am[0],Am[
3]]);AK.Set(8,Am.slice(2,4));for(I=0;I<9;I=I+1)if(this.Ir(AK.Get(I)))return B._NewObject(
D.HO,0).Initialize(this,B.tw(AK.Get(I),AK.Get(0)));}return null;},J_:function(C){
if(C<1)C=1;this.Gf=C;},J9:function(C){if(C<1)C=1;this.J0=C;},Bv:function(C){if(C
)this.A3(0x10,0x0);else this.A3(0x0,0x10);},_Init:function(aArg){D.Ge._Init.call(
this,aArg);this.__proto__=D.Hc;this.F=0x11B;},_Mark:function(E){var A;D.Ge._Mark.
call(this,E);if((A=this.J3)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.
C5)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.C4)&&((A=A[0])._cycle!=
E))A._Mark(A._cycle=E);if((A=this.F_)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);
if((A=this.BW)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.BV)&&((A=A[0
])._cycle!=E))A._Mark(A._cycle=E);},_className:"Core::SimpleTouchHandler"};D.E_={
AD:null,BW:null,BV:null,F_:null,E1:0,AE:0,J$:0,CF:148,Bd:0,Av:0,BC:true,Down:false
,IV:false,GS:false,Bo:function(Ai){var A;if(!!Ai&&Ai.K9(this.CF)){this.Down=Ai.Down;
this.Bd=Ai.Bd;this.Av=Ai.Av;this.AE=Ai.AE;this.GS=false;if(Ai.Down){this.J$=this.
E1;this.IV=this.E1>0;if(this.IV)(A=this.F_)?A[1].call(A[0],this):null;else(A=this.
BV)?A[1].call(A[0],this):null;if(!this.GS)this.E1=this.E1+1;return!this.GS;}if(!
Ai.Down){this.IV=this.E1>1;this.J$=this.E1-1;this.E1=0;(A=this.BW)?A[1].call(A[0
],this):null;return!this.GS;}}return false;},BG:function(aArg){var A;var Ci=(D.Ag.
isPrototypeOf(A=this.Ad)?A:null);if(!Ci)throw new Error(I2);this.AD=Ci.Hx;Ci.Hx=
this;},_Init:function(aArg){this.__proto__=D.E_;this.BG(aArg);B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.AD)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.BW)&&((A=A[0])._cycle!=
E))A._Mark(A._cycle=E);if((A=this.BV)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);
if((A=this.F_)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ad)&&(A._cycle
!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:null,_className:"Core::KeyPressHandler"
};D.HO={Ce:null,Fu:0,DR:B.qx,Initialize:function(O,aOffset){this.Ce=O;this.DR=aOffset;
this.Fu=(aOffset[0]*aOffset[0])+(aOffset[1]*aOffset[1]);return this;},_Init:function(
aArg){this.__proto__=D.HO;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;}
,_ReInit:function(){},_Mark:function(E){var A;if((A=this.Ce)&&(A._cycle!=E))A._Mark(
A._cycle=E);if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0
,_observers:null,_className:"Core::CursorHit"};D.K$={Ci:null,_Init:function(aArg
){this.__proto__=D.K$;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.Ci)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:
null,_className:"Core::ModalContext"};D.G6={E0:null,B2:B.qy,Af:B.qy,isEmpty:false
,_Init:function(aArg){this.__proto__=D.G6;B.gv++;},_Done:function(){this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(E){var A;if((A=this.E0)&&(A._cycle
!=E))A._Mark(A._cycle=E);if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:
null,_cycle:0,_observers:null,_className:"Core::LayoutContext"};D.G7={GI:B.qx,GH:
B.qx,GG:B.qx,GF:B.qx,_Init:function(aArg){D.G6._Init.call(this,aArg);this.__proto__=
D.G7;},_className:"Core::LayoutQuadContext"};D.KP={Ci:null,_Init:function(aArg){
this.__proto__=D.KP;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.Ci)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:
null,_className:"Core::DialogContext"};D.Mt={I$:null,CB:null,_Init:function(aArg
){this.__proto__=D.Mt;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.I$)&&(A._cycle!=E))A._Mark(A._cycle=
E);if((A=this.CB)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ad)&&(A._cycle!=
E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:null,_className:"Core::TaskQueue"
};D.Ms={_Init:function(aArg){this.__proto__=D.Ms;B.gv++;},_Done:function(){this.
__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(E){var A;if((A=this.
Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:null,_className:
"Core::Task"};D.DV={resource:null,CE:function(){this.resource=null;},BG:function(
aArg){this.resource=aArg;},_Init:function(aArg){this.__proto__=D.DV;this.BG(aArg
);B.gv++;},_Done:function(){this.CE();this.__proto__=null;B.gv--;},_ReInit:function(
){},_Mark:function(E){var A;if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},
Ad:null,_cycle:0,_observers:null,_className:"Core::Resource"};D.Timer={EG:null,timer:
null,AE:0,Period:1000,GR:0,BC:false,CE:function(){var tmp=this.timer;if(!!tmp)tmp.
DestroyTimer();this.timer=null;},HE:function(aBegin,aPeriod){if(aBegin<0)aBegin=
0;if(aPeriod<0)aPeriod=0;var tmp=this.timer;if(!tmp&&((aBegin>0)||(aPeriod>0)))tmp=
B.sL(this,this.Trigger);if(!!tmp){tmp.ResetTimer();tmp.StartTimer(aBegin,aPeriod
);}this.timer=tmp;},F$:function(C){if(C<0)C=0;if(C===this.Period)return;this.Period=
C;if(this.BC)this.HE(this.GR,C);},ED:function(C){if(C<0)C=0;if(C===this.GR)return;
this.GR=C;if(this.BC)this.HE(C,this.Period);},Bv:function(C){if(C===this.BC)return;
this.BC=C;if(C)this.HE(this.GR,this.Period);else this.HE(0,0);this.AE=this.G2();
},G2:function(){var ticksCount=0;ticksCount=((new Date).getTime()-B.qt)|0;return ticksCount;
},Trigger:function(){var A;this.AE=this.G2();if(!this.Period)this.Bv(false);(A=this.
EG)?A[1].call(A[0],this):null;},_Init:function(aArg){this.__proto__=D.Timer;B.gv++;
},_Done:function(){this.CE();this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(E){var A;if((A=this.EG)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=
this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:null,_cycle:0,_observers:null,_className:
"Core::Timer"};D.Ow={Ox:0x1,Nf:0x2,NH:0x4,Ou:0x8,BC:0x10,Oj:0x20,NI:0x40,NT:0x80
,NG:0x100,NN:0x200,NA:0x400,N8:0x800,Hg:0x1000,Ov:0x2000,NZ:0x4000,N0:0x8000,Nu:
0x10000,NY:0x20000,Og:0x40000};D.Cc={N9:0x1,N_:0x2,Nb:0x4,Nc:0x8,Nd:0x10,Na:0x20
};D.G0={NW:0,Or:1,Nk:2,NO:3,Oa:4,Os:5,Ot:6,Nl:7,Nm:8,NQ:9,NP:10,Oc:11,Ob:12};D.KeyCode={
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
343,ShiftHome:344,ShiftEnd:345,ShiftTab:346};D.N$={OB:0x1,Oy:0x2,Oz:0x4,OA:0x8,NS:
0x10,NJ:0x20};
D._Init=function(){D.Ge.__proto__=D.Ce;D.A5.__proto__=D.Ce;D.Ag.__proto__=D.A5;D.
Root.__proto__=D.Ag;D.KeyEvent.__proto__=D.Event;D.GU.__proto__=D.Event;D.GT.__proto__=
D.Event;D.GW.__proto__=D.Event;D.Gb.__proto__=D.A5;D.Hc.__proto__=D.Ge;D.G7.__proto__=
D.G6;};D.By=function(E){};return D;})();

/* Embedded Wizard */