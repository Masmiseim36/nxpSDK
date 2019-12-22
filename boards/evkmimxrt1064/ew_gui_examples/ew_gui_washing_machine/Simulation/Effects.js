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
);if(EmWiApp.tf)throw new Error("The unit file 'Effects.js' included twice!");EmWiApp.
tf=(function(){var B=EmWiApp;var C={};
var AX=[0,0,100,100];
C.Fb={Cq:null,timer:null,GV:null,Es:null,D1:null,HD:0,E9:0,EW:0,Ei:0,HJ:0.001000,
DP:0,A8:0,Ba:0,KE:0.500000,LY:3,Ic:3,H0:0.500000,DX:3.000000,Jt:0,MN:0,MM:0,Eb:0
,EF:1000,EI:0,EH:0,F0:0,Ck:false,KW:false,JJ:false,Kw:false,LO:false,Dc:function(
Aj){var A;if(!this.timer)return;if(!this.E9){if(this.JJ)this.E9=-1;else this.E9=
1;this.HD=this.E9;this.Ba=this.timer.AK;this.A8=0;this.DP=-1.000000;}var CU=false;
if((this.E9>0)&&(this.HD>0))CU=this.Oj();else if((this.E9<0)&&(this.HD<0))CU=this.
On();else if(this.E9>0)CU=this.Ol();else CU=this.Om();if(CU){this.Ak(false);(A=this.
GV)?A[1].call(A[0],this):null;(A=this.Es)?A[1].call(A[0],this):null;}},I8:function(
G){var A;if(!this.Cq&&(((this.F0===19)||(this.F0===20))||(this.F0===21))){var IC=
this.LY+1;var N8=Math.sqrt(this.KE);var Gu=0.500000;var K;this.Cq=B._NewObject(C.
Hu,0);this.Cq.CI.Set(0,1.000000);this.Cq.Fn.Set(0,1.000000);for(K=1;K<IC;K=K+1){
this.Cq.CI.Set(K,this.Cq.CI.Get(K-1)*N8);this.Cq.Fn.Set(K,this.Cq.Fn.Get(K-1)*this.
KE);Gu=Gu+this.Cq.CI.Get(K);}var N_=1.000000/Gu;this.Cq.CI.Set(0,0.500000);Gu=0.000000;
for(K=0;K<IC;K=K+1){this.Cq.CI.Set(K,this.Cq.CI.Get(K)*N_);Gu=Gu+this.Cq.CI.Get(
K);}this.Cq.CI.Set(IC,this.Cq.CI.Get(IC)+(1.000000-Gu));}if(this.KW){if(G<0.500000
)G=G*2.000000;else G=2.000000-(G*2.000000);}switch(this.F0){case 1:{G=Math.pow(G
,this.DX);}break;case 2:{G=1.000000-G;G=1.000000-Math.pow(G,this.DX);}break;case
3:{G=G*2.000000;if(G<1.000000)G=Math.pow(G,this.DX)/2.000000;else{G=2.000000-G;G=(
2.000000-Math.pow(G,this.DX))*0.500000;}}break;case 4:{G=(Math.pow(2.718000,this.
DX*G)-1.000000)/(Math.pow(2.718000,this.DX)-1.000000);}break;case 5:{G=1.000000-
G;G=1.000000-((Math.pow(2.718000,this.DX*G)-1.000000)/(Math.pow(2.718000,this.DX
)-1.000000));}break;case 6:{G=G*2.000000;if(G<1.000000)G=((Math.pow(2.718000,this.
DX*G)-1.000000)/(Math.pow(2.718000,this.DX)-1.000000))/2.000000;else{G=2.000000-
G;G=(2.000000-((Math.pow(2.718000,this.DX*G)-1.000000)/(Math.pow(2.718000,this.DX
)-1.000000)))*0.500000;}}break;case 7:{G=1.000000-Math.cos((G*90.000000)*B.ks);}
break;case 8:{G=Math.sin((G*90.000000)*B.ks);}break;case 9:{G=G*2.000000;if(G<1.000000
){G=1.000000-Math.cos((G*90.000000)*B.ks);G=G*0.500000;}else{G=2.000000-G;G=1.000000-
Math.cos((G*90.000000)*B.ks);G=(2.000000-G)*0.500000;}}break;case 10:{G=1.000000-
Math.sqrt(1.000000-(G*G));}break;case 11:{G=1.000000-G;G=Math.sqrt(1.000000-(G*G
));}break;case 12:{G=G*2.000000;if(G<1.000000){G=1.000000-Math.sqrt(1.000000-(G*
G));G=G*0.500000;}else{G=2.000000-G;G=1.000000-Math.sqrt(1.000000-(G*G));G=(2.000000-
G)*0.500000;}}break;case 13:{G=((G*G)*G)-((G*this.H0)*Math.sin((G*180.000000)*B.
ks));}break;case 14:{G=1.000000-G;G=((G*G)*G)-((G*this.H0)*Math.sin((G*180.000000
)*B.ks));G=1.000000-G;}break;case 15:{G=G*2.000000;if(G<1.000000){G=((G*G)*G)-((
G*this.H0)*Math.sin((G*180.000000)*B.ks));G=G*0.500000;}else{G=2.000000-G;G=((G*
G)*G)-((G*this.H0)*Math.sin((G*180.000000)*B.ks));G=(2.000000-G)*0.500000;}}break;
case 16:{G=((G*G)*G)*Math.sin(((G*90.000000)*(1+(4*this.Ic)))*B.ks);}break;case 17:{
G=1.000000-G;G=1.000000-(((G*G)*G)*Math.sin(((G*90.000000)*(1+(4*this.Ic)))*B.ks
));}break;case 18:{G=G*2.000000;if(G<1.000000){G=((G*G)*G)*Math.sin(((G*90.000000
)*(1+(4*this.Ic)))*B.ks);G=G*0.500000;}else{G=2.000000-G;G=((G*G)*G)*Math.sin(((
G*90.000000)*(1+(4*this.Ic)))*B.ks);G=(2.000000-G)*0.500000;}}break;case 19:{var
Ek=this.Cq;var K=0;var Ep=0.000000;var C2=Ek.CI.Get(0);var CJ=1.000000-G;while(CJ>
C2){K=K+1;Ep=C2;C2=C2+Ek.CI.Get(K);}G=(CJ-Ep)/(C2-Ep);if(!K)G=1.000000-(G*G);else{
G=(2.000000*G)-1.000000;G=Ek.Fn.Get(K)*(1.000000-(G*G));}}break;case 20:{var Ek=
this.Cq;var K=0;var Ep=0.000000;var C2=Ek.CI.Get(0);while(G>C2){K=K+1;Ep=C2;C2=C2+
Ek.CI.Get(K);}G=(G-Ep)/(C2-Ep);if(!K)G=G*G;else{G=(2.000000*G)-1.000000;G=1.000000-(
Ek.Fn.Get(K)*(1.000000-(G*G)));}}break;case 21:{var Ek=this.Cq;var K=0;var Ep=0.000000;
var C2=Ek.CI.Get(0);var CF=G*2.000000;var CJ=CF-1.000000;if(CF<1.000000)CJ=-CJ;while(
CJ>C2){K=K+1;Ep=C2;C2=C2+Ek.CI.Get(K);}CJ=(CJ-Ep)/(C2-Ep);if(!K)CJ=CJ*CJ;else{CJ=(
2.000000*CJ)-1.000000;CJ=1.000000-(Ek.Fn.Get(K)*(1.000000-(CJ*CJ)));}if(CF<1.000000
)G=0.500000*(1.000000-CJ);else G=0.500000*(1.000000+CJ);}break;default:{if(this.
Kw){var CF=G;var HM=1.000000-CF;G=((HM*CF)*(this.Ei+1.000000))+(CF*CF);}else if(
this.LO){var CF=G;var HM=1.000000-CF;var Ls=CF*CF;var N$=HM*HM;G=(((N$*CF)*(this.
Ei+1.000000))+((HM*Ls)*(this.EW+2.000000)))+(Ls*CF);}}}if(((this.Jt>0.000000)&&(
G>0.000000))&&(G<1.000000))G=G+B.sI(-this.Jt*0.100000,this.Jt*0.100000);this.Ja(
G);(A=this.D1)?A[1].call(A[0],this):null;},On:function(){var At=(this.timer.AK-this.
Ba)|0;if(At<0)At=-At;var Dn=this.EH;var DT=this.EH+this.EF;var BI=this.EI+this.EF;
var CU=false;var Bk=this.DP;if(!this.A8&&(At>=DT)){this.A8=1;At=At-DT;this.Ba=this.
Ba+DT;}if((this.A8>0)&&(At>=BI)){var BT=(At/BI)|0;this.A8=this.A8+BT;At=At-(BT*BI
);this.Ba=this.Ba+(BT*BI);}if((this.A8>2)&&!this.Eb)this.A8=1;if(this.A8>0)Dn=this.
EI;if((this.A8>=this.Eb)&&(this.Eb>0)){CU=true;Bk=0.000000;}else if(At>=Dn)Bk=1.000000-((
At-Dn)*this.HJ);else if(Bk>=0.000000)Bk=0.000000;if(Bk!==this.DP){this.DP=Bk;this.
I8(Bk);}return CU;},Om:function(){var At=(this.Ba-this.timer.AK)|0;var Dn=this.EH;
var DT=this.EH+this.EF;var BI=this.EI+this.EF;var CU=false;var Bk=this.DP;if((this.
A8>1)&&(At<0)){var BT=(((-At+BI)-1)/BI)|0;if((this.A8-BT)<=0)BT=this.A8-1;this.A8=
this.A8-BT;At=At+(BT*BI);this.Ba=this.Ba+(BT*BI);}if(((this.A8===1)&&(At<0))&&(this.
Eb>0)){this.A8=0;At=At+DT;this.Ba=this.Ba+DT;}if(((this.A8===1)&&(At<0))&&!this.
Eb){var BT=(((-At+BI)-1)/BI)|0;At=At+(BT*BI);this.Ba=this.Ba+(BT*BI);}if(this.A8>
0)Dn=this.EI;if(At<0){CU=true;Bk=1.000000;}else if(At>=Dn)Bk=1.000000-((At-Dn)*this.
HJ);else if(Bk>=0.000000)Bk=1.000000;if(Bk!==this.DP){this.DP=Bk;this.I8(Bk);}return CU;
},Ol:function(){var At=(this.Ba-this.timer.AK)|0;var Dn=this.EH;var DT=this.EH+this.
EF;var BI=this.EI+this.EF;var CU=false;var Bk=this.DP;if((this.A8>1)&&(At<0)){var
BT=(((-At+BI)-1)/BI)|0;if((this.A8-BT)<=0)BT=this.A8-1;this.A8=this.A8-BT;At=At+(
BT*BI);this.Ba=this.Ba+(BT*BI);}if(((this.A8===1)&&(At<0))&&(this.Eb>0)){this.A8=
0;At=At+DT;this.Ba=this.Ba+DT;}if(((this.A8===1)&&(At<0))&&!this.Eb){var BT=(((-
At+BI)-1)/BI)|0;At=At+(BT*BI);this.Ba=this.Ba+(BT*BI);}if(this.A8>0)Dn=this.EI;if(
At<0){CU=true;Bk=0.000000;}else if(At>=Dn)Bk=(At-Dn)*this.HJ;else if(Bk>=0.000000
)Bk=0.000000;if(Bk!==this.DP){this.DP=Bk;this.I8(Bk);}return CU;},Oj:function(){
var At=(this.timer.AK-this.Ba)|0;if(At<0)At=-At;var Dn=this.EH;var DT=this.EH+this.
EF;var BI=this.EI+this.EF;var CU=false;var Bk=this.DP;if(!this.A8&&(At>=DT)){this.
A8=1;At=At-DT;this.Ba=this.Ba+DT;}if((this.A8>0)&&(At>=BI)){var BT=(At/BI)|0;this.
A8=this.A8+BT;At=At-(BT*BI);this.Ba=this.Ba+(BT*BI);}if((this.A8>2)&&!this.Eb)this.
A8=1;if(this.A8>0)Dn=this.EI;if((this.A8>=this.Eb)&&(this.Eb>0)){CU=true;Bk=1.000000;
}else if(At>=Dn)Bk=(At-Dn)*this.HJ;else if(Bk>=0.000000)Bk=1.000000;if(Bk!==this.
DP){this.DP=Bk;this.I8(Bk);}return CU;},JC:function(E){if(this.JJ===E)return;this.
JJ=E;if(!this.Ck||!this.E9)return;if(E)this.HD=-1;else this.HD=1;this.Ba=(this.timer.
G7()*2)-this.Ba;},Bv:function(E){if(this.F0===E)return;this.F0=E;this.Cq=null;switch(
E){case 24:{this.Ei=-1.100000;this.EW=1.100000;}break;case 22:{this.Ei=-1.000000;
this.EW=-2.000000;}break;case 23:{this.Ei=2.000000;this.EW=1.000000;}break;case 25:{
this.Ei=1.100000;this.EW=-1.100000;}break;case 0:{this.Ei=0.000000;this.EW=0.000000;
}break;default:{this.Ei=this.MM;this.EW=this.MN;}}this.Kw=(this.Ei===this.EW)&&(
this.Ei!==0.000000);this.LO=!this.Kw&&(this.Ei!==this.EW);},BL:function(E){if(E<
0)E=0;this.Eb=E;},Bu:function(E){if(E<15)E=15;this.EF=E;this.HJ=1.000000/E;},MA:
function(E){if(E<0)E=0;this.EI=E;},GG:function(E){if(E<0)E=0;this.EH=E;},Ak:function(
E){if(this.Ck===E)return;this.Ck=E;if(!E&&!!this.timer){B.rA([this,this.Dc],this.
timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(C.Gz);B.rl([this,this.
Dc],this.timer,0);this.E9=0;B.kD([this,this.Dc],this);}},Ja:function(Gg){},MK:function(
Aj){if(this.Ck)this.Ak(false);this.Ak(true);},_Init:function(aArg){this.__proto__=
C.Fb;B.gb++;},_Done:function(){this.__proto__=C.Fb;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Cq)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.GV)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.Es)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.D1)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Effects::Effect"
};C.Ah={A0:null,GL:0,Bn:255,AW:0,Ja:function(Gg){var A;this.GL=this.AW+(Math.round((
this.Bn-this.AW)*Gg)|0);if(!!this.A0)(A=this.A0,A[2].call(A[0],this.GL));},_Init:
function(aArg){C.Fb._Init.call(this,aArg);this.__proto__=C.Ah;},_Mark:function(D
){var A;C.Fb._Mark.call(this,D);if((A=this.A0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Effects::Int32Effect"};C.JF={A0:null,GL:B.pn,Bn:AX,AW:B.pn,Ja:function(
Gg){var A;var Z=this.AW[0];var Aa=this.AW[1];var Ac=this.AW[2];var Ad=this.AW[3];
Z=Z+(Math.round((this.Bn[0]-Z)*Gg)|0);Aa=Aa+(Math.round((this.Bn[1]-Aa)*Gg)|0);Ac=
Ac+(Math.round((this.Bn[2]-Ac)*Gg)|0);Ad=Ad+(Math.round((this.Bn[3]-Ad)*Gg)|0);this.
GL=[Z,Aa,Ac,Ad];if(!!this.A0)(A=this.A0,A[2].call(A[0],this.GL));},_Init:function(
aArg){C.Fb._Init.call(this,aArg);this.__proto__=C.JF;},_Mark:function(D){var A;C.
Fb._Mark.call(this,D);if((A=this.A0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);}
,_className:"Effects::RectEffect"};C.F0={O8:0,Pe:1,Pg:2,Pf:3,OW:4,OY:5,OX:6,Pt:7
,Pv:8,Pu:9,OL:10,ON:11,OM:12,OD:13,OF:14,OE:15,OS:16,OU:17,OT:18,OI:19,OK:20,OJ:
21,OR:22,OZ:23,OQ:24,O0:25,OO:26};C.Jj={Trigger:function(){B.Core.Timer.Trigger.
call(this);B.pl(this,0);},_Init:function(aArg){B.Core.Timer._Init.call(this,aArg
);this.__proto__=C.Jj;},_className:"Effects::EffectTimerClass"};C.Gz={_Init:function(
){C.Jj._Init.call(this,0);this.JB(15);this.Ak(true);},_variants:function(){return this;
},_this:null};C.Hu={CI:B.sr(12,0,null),Fn:B.sr(12,0,null),_Init:function(aArg){(
this.CI=[]).__proto__=C.Hu.CI;(this.Fn=[]).__proto__=C.Hu.Fn;this.__proto__=C.Hu;
B.gb++;},_Done:function(){this.__proto__=C.Hu;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:
0,_observers:null,_className:"Effects::TimingList"};
C._Init=function(){C.Ah.__proto__=C.Fb;C.JF.__proto__=C.Fb;C.Jj.__proto__=B.Core.
Timer;};C.Dh=function(D){var A;if((A=C.Gz._this)&&(A._cycle!=D))A._Done(C.Gz._this=
null);};return C;})();

/* Embedded Wizard */