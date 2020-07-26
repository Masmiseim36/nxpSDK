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
);if(EmWiApp.um)throw new Error("The unit file 'Effects.js' included twice!");EmWiApp.
um=(function(){var B=EmWiApp;var D={};

D.GX={BF:null,timer:null,Kx:null,Eb:null,J2:null,Jb:0,Ev:0,D1:0,Dj:0,Gv:0.001000,
CU:0,AA:0,AL:0,JL:0.500000,KM:3,Ha:3,GP:0.500000,C2:3.000000,IK:0,Mv:0,Mu:0,Dw:0
,DJ:1000,DQ:0,Ea:0,EK:0,BC:false,Kb:false,Mo:false,JD:false,KD:false,CZ:function(
AV){var A;if(!this.timer)return;if(!this.Ev){if(this.Mo)this.Ev=-1;else this.Ev=
1;this.Jb=this.Ev;this.AL=this.timer.AE;this.AA=0;this.CU=-1.000000;}var B0=false;
if((this.Ev>0)&&(this.Jb>0))B0=this.M3();else if((this.Ev<0)&&(this.Jb<0))B0=this.
M6();else if(this.Ev>0)B0=this.M4();else B0=this.M5();if(B0){this.Bv(false);(A=this.
Kx)?A[1].call(A[0],this):null;(A=this.Eb)?A[1].call(A[0],this):null;}},HI:function(
G){var A;if(!this.BF&&(((this.EK===19)||(this.EK===20))||(this.EK===21))){var Hk=
this.KM+1;var MW=Math.sqrt(this.JL);var E4=0.500000;var I;this.BF=B._NewObject(D.
He,0);this.BF.BT.Set(0,1.000000);this.BF.Eh.Set(0,1.000000);for(I=1;I<Hk;I=I+1){
this.BF.BT.Set(I,this.BF.BT.Get(I-1)*MW);this.BF.Eh.Set(I,this.BF.Eh.Get(I-1)*this.
JL);E4=E4+this.BF.BT.Get(I);}var MX=1.000000/E4;this.BF.BT.Set(0,0.500000);E4=0.000000;
for(I=0;I<Hk;I=I+1){this.BF.BT.Set(I,this.BF.BT.Get(I)*MX);E4=E4+this.BF.BT.Get(
I);}this.BF.BT.Set(Hk,this.BF.BT.Get(Hk)+(1.000000-E4));}if(this.Kb){if(G<0.500000
)G=G*2.000000;else G=2.000000-(G*2.000000);}switch(this.EK){case 1:{G=Math.pow(G
,this.C2);}break;case 2:{G=1.000000-G;G=1.000000-Math.pow(G,this.C2);}break;case
3:{G=G*2.000000;if(G<1.000000)G=Math.pow(G,this.C2)/2.000000;else{G=2.000000-G;G=(
2.000000-Math.pow(G,this.C2))*0.500000;}}break;case 4:{G=(Math.pow(2.718000,this.
C2*G)-1.000000)/(Math.pow(2.718000,this.C2)-1.000000);}break;case 5:{G=1.000000-
G;G=1.000000-((Math.pow(2.718000,this.C2*G)-1.000000)/(Math.pow(2.718000,this.C2
)-1.000000));}break;case 6:{G=G*2.000000;if(G<1.000000)G=((Math.pow(2.718000,this.
C2*G)-1.000000)/(Math.pow(2.718000,this.C2)-1.000000))/2.000000;else{G=2.000000-
G;G=(2.000000-((Math.pow(2.718000,this.C2*G)-1.000000)/(Math.pow(2.718000,this.C2
)-1.000000)))*0.500000;}}break;case 7:{G=1.000000-Math.cos((G*90.000000)*B.ii);}
break;case 8:{G=Math.sin((G*90.000000)*B.ii);}break;case 9:{G=G*2.000000;if(G<1.000000
){G=1.000000-Math.cos((G*90.000000)*B.ii);G=G*0.500000;}else{G=2.000000-G;G=1.000000-
Math.cos((G*90.000000)*B.ii);G=(2.000000-G)*0.500000;}}break;case 10:{G=1.000000-
Math.sqrt(1.000000-(G*G));}break;case 11:{G=1.000000-G;G=Math.sqrt(1.000000-(G*G
));}break;case 12:{G=G*2.000000;if(G<1.000000){G=1.000000-Math.sqrt(1.000000-(G*
G));G=G*0.500000;}else{G=2.000000-G;G=1.000000-Math.sqrt(1.000000-(G*G));G=(2.000000-
G)*0.500000;}}break;case 13:{G=((G*G)*G)-((G*this.GP)*Math.sin((G*180.000000)*B.
ii));}break;case 14:{G=1.000000-G;G=((G*G)*G)-((G*this.GP)*Math.sin((G*180.000000
)*B.ii));G=1.000000-G;}break;case 15:{G=G*2.000000;if(G<1.000000){G=((G*G)*G)-((
G*this.GP)*Math.sin((G*180.000000)*B.ii));G=G*0.500000;}else{G=2.000000-G;G=((G*
G)*G)-((G*this.GP)*Math.sin((G*180.000000)*B.ii));G=(2.000000-G)*0.500000;}}break;
case 16:{G=((G*G)*G)*Math.sin(((G*90.000000)*(1+(4*this.Ha)))*B.ii);}break;case 17:{
G=1.000000-G;G=1.000000-(((G*G)*G)*Math.sin(((G*90.000000)*(1+(4*this.Ha)))*B.ii
));}break;case 18:{G=G*2.000000;if(G<1.000000){G=((G*G)*G)*Math.sin(((G*90.000000
)*(1+(4*this.Ha)))*B.ii);G=G*0.500000;}else{G=2.000000-G;G=((G*G)*G)*Math.sin(((
G*90.000000)*(1+(4*this.Ha)))*B.ii);G=(2.000000-G)*0.500000;}}break;case 19:{var
Dl=this.BF;var I=0;var Dp=0.000000;var B_=Dl.BT.Get(0);var BU=1.000000-G;while(BU>
B_){I=I+1;Dp=B_;B_=B_+Dl.BT.Get(I);}G=(BU-Dp)/(B_-Dp);if(!I)G=1.000000-(G*G);else{
G=(2.000000*G)-1.000000;G=Dl.Eh.Get(I)*(1.000000-(G*G));}}break;case 20:{var Dl=
this.BF;var I=0;var Dp=0.000000;var B_=Dl.BT.Get(0);while(G>B_){I=I+1;Dp=B_;B_=B_+
Dl.BT.Get(I);}G=(G-Dp)/(B_-Dp);if(!I)G=G*G;else{G=(2.000000*G)-1.000000;G=1.000000-(
Dl.Eh.Get(I)*(1.000000-(G*G)));}}break;case 21:{var Dl=this.BF;var I=0;var Dp=0.000000;
var B_=Dl.BT.Get(0);var CA=G*2.000000;var BU=CA-1.000000;if(CA<1.000000)BU=-BU;while(
BU>B_){I=I+1;Dp=B_;B_=B_+Dl.BT.Get(I);}BU=(BU-Dp)/(B_-Dp);if(!I)BU=BU*BU;else{BU=(
2.000000*BU)-1.000000;BU=1.000000-(Dl.Eh.Get(I)*(1.000000-(BU*BU)));}if(CA<1.000000
)G=0.500000*(1.000000-BU);else G=0.500000*(1.000000+BU);}break;default:{if(this.
JD){var CA=G;var GB=1.000000-CA;G=((GB*CA)*(this.Dj+1.000000))+(CA*CA);}else if(
this.KD){var CA=G;var GB=1.000000-CA;var Kp=CA*CA;var MY=GB*GB;G=(((MY*CA)*(this.
Dj+1.000000))+((GB*Kp)*(this.D1+2.000000)))+(Kp*CA);}}}if(((this.IK>0.000000)&&(
G>0.000000))&&(G<1.000000))G=G+B.tO(-this.IK*0.100000,this.IK*0.100000);this.JG(
G);(A=this.J2)?A[1].call(A[0],this):null;},M6:function(){var Ab=(this.timer.AE-this.
AL)|0;if(Ab<0)Ab=-Ab;var Cn=this.Ea;var CW=this.Ea+this.DJ;var Bb=this.DQ+this.DJ;
var B0=false;var AQ=this.CU;if(!this.AA&&(Ab>=CW)){this.AA=1;Ab=Ab-CW;this.AL=this.
AL+CW;}if((this.AA>0)&&(Ab>=Bb)){var Bf=(Ab/Bb)|0;this.AA=this.AA+Bf;Ab=Ab-(Bf*Bb
);this.AL=this.AL+(Bf*Bb);}if((this.AA>2)&&!this.Dw)this.AA=1;if(this.AA>0)Cn=this.
DQ;if((this.AA>=this.Dw)&&(this.Dw>0)){B0=true;AQ=0.000000;}else if(Ab>=Cn)AQ=1.000000-((
Ab-Cn)*this.Gv);else if(AQ>=0.000000)AQ=0.000000;if(AQ!==this.CU){this.CU=AQ;this.
HI(AQ);}return B0;},M5:function(){var Ab=(this.AL-this.timer.AE)|0;var Cn=this.Ea;
var CW=this.Ea+this.DJ;var Bb=this.DQ+this.DJ;var B0=false;var AQ=this.CU;if((this.
AA>1)&&(Ab<0)){var Bf=(((-Ab+Bb)-1)/Bb)|0;if((this.AA-Bf)<=0)Bf=this.AA-1;this.AA=
this.AA-Bf;Ab=Ab+(Bf*Bb);this.AL=this.AL+(Bf*Bb);}if(((this.AA===1)&&(Ab<0))&&(this.
Dw>0)){this.AA=0;Ab=Ab+CW;this.AL=this.AL+CW;}if(((this.AA===1)&&(Ab<0))&&!this.
Dw){var Bf=(((-Ab+Bb)-1)/Bb)|0;Ab=Ab+(Bf*Bb);this.AL=this.AL+(Bf*Bb);}if(this.AA>
0)Cn=this.DQ;if(Ab<0){B0=true;AQ=1.000000;}else if(Ab>=Cn)AQ=1.000000-((Ab-Cn)*this.
Gv);else if(AQ>=0.000000)AQ=1.000000;if(AQ!==this.CU){this.CU=AQ;this.HI(AQ);}return B0;
},M4:function(){var Ab=(this.AL-this.timer.AE)|0;var Cn=this.Ea;var CW=this.Ea+this.
DJ;var Bb=this.DQ+this.DJ;var B0=false;var AQ=this.CU;if((this.AA>1)&&(Ab<0)){var
Bf=(((-Ab+Bb)-1)/Bb)|0;if((this.AA-Bf)<=0)Bf=this.AA-1;this.AA=this.AA-Bf;Ab=Ab+(
Bf*Bb);this.AL=this.AL+(Bf*Bb);}if(((this.AA===1)&&(Ab<0))&&(this.Dw>0)){this.AA=
0;Ab=Ab+CW;this.AL=this.AL+CW;}if(((this.AA===1)&&(Ab<0))&&!this.Dw){var Bf=(((-
Ab+Bb)-1)/Bb)|0;Ab=Ab+(Bf*Bb);this.AL=this.AL+(Bf*Bb);}if(this.AA>0)Cn=this.DQ;if(
Ab<0){B0=true;AQ=0.000000;}else if(Ab>=Cn)AQ=(Ab-Cn)*this.Gv;else if(AQ>=0.000000
)AQ=0.000000;if(AQ!==this.CU){this.CU=AQ;this.HI(AQ);}return B0;},M3:function(){
var Ab=(this.timer.AE-this.AL)|0;if(Ab<0)Ab=-Ab;var Cn=this.Ea;var CW=this.Ea+this.
DJ;var Bb=this.DQ+this.DJ;var B0=false;var AQ=this.CU;if(!this.AA&&(Ab>=CW)){this.
AA=1;Ab=Ab-CW;this.AL=this.AL+CW;}if((this.AA>0)&&(Ab>=Bb)){var Bf=(Ab/Bb)|0;this.
AA=this.AA+Bf;Ab=Ab-(Bf*Bb);this.AL=this.AL+(Bf*Bb);}if((this.AA>2)&&!this.Dw)this.
AA=1;if(this.AA>0)Cn=this.DQ;if((this.AA>=this.Dw)&&(this.Dw>0)){B0=true;AQ=1.000000;
}else if(Ab>=Cn)AQ=(Ab-Cn)*this.Gv;else if(AQ>=0.000000)AQ=1.000000;if(AQ!==this.
CU){this.CU=AQ;this.HI(AQ);}return B0;},Mj:function(C){if(this.EK===C)return;this.
EK=C;this.BF=null;switch(C){case 24:{this.Dj=-1.100000;this.D1=1.100000;}break;case
22:{this.Dj=-1.000000;this.D1=-2.000000;}break;case 23:{this.Dj=2.000000;this.D1=
1.000000;}break;case 25:{this.Dj=1.100000;this.D1=-1.100000;}break;case 0:{this.
Dj=0.000000;this.D1=0.000000;}break;default:{this.Dj=this.Mu;this.D1=this.Mv;}}this.
JD=(this.Dj===this.D1)&&(this.Dj!==0.000000);this.KD=!this.JD&&(this.Dj!==this.D1
);},Lj:function(C){if(C<15)C=15;this.DJ=C;this.Gv=1.000000/C;},LY:function(C){if(
C<0)C=0;this.DQ=C;},Bv:function(C){if(this.BC===C)return;this.BC=C;if(!C&&!!this.
timer){B.sM([this,this.CZ],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(
D.Fv);B.sz([this,this.CZ],this.timer,0);this.Ev=0;B.lq([this,this.CZ],this);}},JG:
function(Kj){},_Init:function(aArg){this.__proto__=D.GX;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.BF)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.timer)&&(A._cycle!=E))A.
_Mark(A._cycle=E);if((A=this.Kx)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=
this.Eb)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.J2)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ad:
null,_cycle:0,_observers:null,_className:"Effects::Effect"};D.Ca={A4:null,Kd:0,Ke:
255,IY:0,JG:function(Kj){var A;this.Kd=this.IY+(Math.round((this.Ke-this.IY)*Kj)|
0);if(!!this.A4)(A=this.A4,A[2].call(A[0],this.Kd));},_Init:function(aArg){D.GX.
_Init.call(this,aArg);this.__proto__=D.Ca;},_Mark:function(E){var A;D.GX._Mark.call(
this,E);if((A=this.A4)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);},_className:"Effects::Int32Effect"
};D.EK={NR:0,N1:1,N3:2,N2:3,NB:4,ND:5,NC:6,Ol:7,On:8,Om:9,Nq:10,Ns:11,Nr:12,Ng:13
,Ni:14,Nh:15,Nx:16,Nz:17,Ny:18,Nn:19,Np:20,No:21,Nw:22,NE:23,Nv:24,NF:25,Nt:26};
D.HP={Trigger:function(){B.Core.Timer.Trigger.call(this);B.qw(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=D.HP;},_className:"Effects::EffectTimerClass"
};D.Fv={_Init:function(){D.HP._Init.call(this,0);this.F$(15);this.Bv(true);},_variants:
function(){return this;},_this:null};D.He={BT:B.tA(12,0,null),Eh:B.tA(12,0,null)
,_Init:function(aArg){(this.BT=[]).__proto__=D.He.BT;(this.Eh=[]).__proto__=D.He.
Eh;this.__proto__=D.He;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.Ad)&&(A._cycle!=E))A._Mark(A._cycle=
E);},Ad:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};
D._Init=function(){D.Ca.__proto__=D.GX;D.HP.__proto__=B.Core.Timer;};D.By=function(
E){var A;if((A=D.Fv._this)&&(A._cycle!=E))A._Done(D.Fv._this=null);};return D;})(
);

/* Embedded Wizard */