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
);if(EmWiApp.th)throw new Error("The unit file 'Effects.js' included twice!");EmWiApp.
th=(function(){var B=EmWiApp;var D={};

D.Fh={BE:null,timer:null,Kj:null,D6:null,JO:null,IX:0,Ep:0,DU:0,Dh:0,Gj:0.001000,
CS:0,AB:0,AJ:0,Jv:0.500000,Kx:3,GZ:3,GD:0.500000,CZ:3.000000,Iq:0,Mb:0,Ma:0,Ds:0
,DF:1000,DL:0,D5:0,EF:0,BB:false,JY:false,L_:false,Jm:false,Ko:false,CX:function(
AT){var A;if(!this.timer)return;if(!this.Ep){if(this.L_)this.Ep=-1;else this.Ep=
1;this.IX=this.Ep;this.AJ=this.timer.AF;this.AB=0;this.CS=-1.000000;}var B0=false;
if((this.Ep>0)&&(this.IX>0))B0=this.MI();else if((this.Ep<0)&&(this.IX<0))B0=this.
ML();else if(this.Ep>0)B0=this.MJ();else B0=this.MK();if(B0){this.Bn(false);(A=this.
Kj)?A[1].call(A[0],this):null;(A=this.D6)?A[1].call(A[0],this):null;}},Ht:function(
H){var A;if(!this.BE&&(((this.EF===19)||(this.EF===20))||(this.EF===21))){var G8=
this.Kx+1;var MC=Math.sqrt(this.Jv);var EY=0.500000;var J;this.BE=B._NewObject(D.
FX,0);this.BE.BT.Set(0,1.000000);this.BE.Ec.Set(0,1.000000);for(J=1;J<G8;J=J+1){
this.BE.BT.Set(J,this.BE.BT.Get(J-1)*MC);this.BE.Ec.Set(J,this.BE.Ec.Get(J-1)*this.
Jv);EY=EY+this.BE.BT.Get(J);}var MD=1.000000/EY;this.BE.BT.Set(0,0.500000);EY=0.000000;
for(J=0;J<G8;J=J+1){this.BE.BT.Set(J,this.BE.BT.Get(J)*MD);EY=EY+this.BE.BT.Get(
J);}this.BE.BT.Set(G8,this.BE.BT.Get(G8)+(1.000000-EY));}if(this.JY){if(H<0.500000
)H=H*2.000000;else H=2.000000-(H*2.000000);}switch(this.EF){case 1:{H=Math.pow(H
,this.CZ);}break;case 2:{H=1.000000-H;H=1.000000-Math.pow(H,this.CZ);}break;case
3:{H=H*2.000000;if(H<1.000000)H=Math.pow(H,this.CZ)/2.000000;else{H=2.000000-H;H=(
2.000000-Math.pow(H,this.CZ))*0.500000;}}break;case 4:{H=(Math.pow(2.718000,this.
CZ*H)-1.000000)/(Math.pow(2.718000,this.CZ)-1.000000);}break;case 5:{H=1.000000-
H;H=1.000000-((Math.pow(2.718000,this.CZ*H)-1.000000)/(Math.pow(2.718000,this.CZ
)-1.000000));}break;case 6:{H=H*2.000000;if(H<1.000000)H=((Math.pow(2.718000,this.
CZ*H)-1.000000)/(Math.pow(2.718000,this.CZ)-1.000000))/2.000000;else{H=2.000000-
H;H=(2.000000-((Math.pow(2.718000,this.CZ*H)-1.000000)/(Math.pow(2.718000,this.CZ
)-1.000000)))*0.500000;}}break;case 7:{H=1.000000-Math.cos((H*90.000000)*B.ks);}
break;case 8:{H=Math.sin((H*90.000000)*B.ks);}break;case 9:{H=H*2.000000;if(H<1.000000
){H=1.000000-Math.cos((H*90.000000)*B.ks);H=H*0.500000;}else{H=2.000000-H;H=1.000000-
Math.cos((H*90.000000)*B.ks);H=(2.000000-H)*0.500000;}}break;case 10:{H=1.000000-
Math.sqrt(1.000000-(H*H));}break;case 11:{H=1.000000-H;H=Math.sqrt(1.000000-(H*H
));}break;case 12:{H=H*2.000000;if(H<1.000000){H=1.000000-Math.sqrt(1.000000-(H*
H));H=H*0.500000;}else{H=2.000000-H;H=1.000000-Math.sqrt(1.000000-(H*H));H=(2.000000-
H)*0.500000;}}break;case 13:{H=((H*H)*H)-((H*this.GD)*Math.sin((H*180.000000)*B.
ks));}break;case 14:{H=1.000000-H;H=((H*H)*H)-((H*this.GD)*Math.sin((H*180.000000
)*B.ks));H=1.000000-H;}break;case 15:{H=H*2.000000;if(H<1.000000){H=((H*H)*H)-((
H*this.GD)*Math.sin((H*180.000000)*B.ks));H=H*0.500000;}else{H=2.000000-H;H=((H*
H)*H)-((H*this.GD)*Math.sin((H*180.000000)*B.ks));H=(2.000000-H)*0.500000;}}break;
case 16:{H=((H*H)*H)*Math.sin(((H*90.000000)*(1+(4*this.GZ)))*B.ks);}break;case 17:{
H=1.000000-H;H=1.000000-(((H*H)*H)*Math.sin(((H*90.000000)*(1+(4*this.GZ)))*B.ks
));}break;case 18:{H=H*2.000000;if(H<1.000000){H=((H*H)*H)*Math.sin(((H*90.000000
)*(1+(4*this.GZ)))*B.ks);H=H*0.500000;}else{H=2.000000-H;H=((H*H)*H)*Math.sin(((
H*90.000000)*(1+(4*this.GZ)))*B.ks);H=(2.000000-H)*0.500000;}}break;case 19:{var
Di=this.BE;var J=0;var Dm=0.000000;var B9=Di.BT.Get(0);var BU=1.000000-H;while(BU>
B9){J=J+1;Dm=B9;B9=B9+Di.BT.Get(J);}H=(BU-Dm)/(B9-Dm);if(!J)H=1.000000-(H*H);else{
H=(2.000000*H)-1.000000;H=Di.Ec.Get(J)*(1.000000-(H*H));}}break;case 20:{var Di=
this.BE;var J=0;var Dm=0.000000;var B9=Di.BT.Get(0);while(H>B9){J=J+1;Dm=B9;B9=B9+
Di.BT.Get(J);}H=(H-Dm)/(B9-Dm);if(!J)H=H*H;else{H=(2.000000*H)-1.000000;H=1.000000-(
Di.Ec.Get(J)*(1.000000-(H*H)));}}break;case 21:{var Di=this.BE;var J=0;var Dm=0.000000;
var B9=Di.BT.Get(0);var Cz=H*2.000000;var BU=Cz-1.000000;if(Cz<1.000000)BU=-BU;while(
BU>B9){J=J+1;Dm=B9;B9=B9+Di.BT.Get(J);}BU=(BU-Dm)/(B9-Dm);if(!J)BU=BU*BU;else{BU=(
2.000000*BU)-1.000000;BU=1.000000-(Di.Ec.Get(J)*(1.000000-(BU*BU)));}if(Cz<1.000000
)H=0.500000*(1.000000-BU);else H=0.500000*(1.000000+BU);}break;default:{if(this.
Jm){var Cz=H;var Gp=1.000000-Cz;H=((Gp*Cz)*(this.Dh+1.000000))+(Cz*Cz);}else if(
this.Ko){var Cz=H;var Gp=1.000000-Cz;var Kb=Cz*Cz;var ME=Gp*Gp;H=(((ME*Cz)*(this.
Dh+1.000000))+((Gp*Kb)*(this.DU+2.000000)))+(Kb*Cz);}}}if(((this.Iq>0.000000)&&(
H>0.000000))&&(H<1.000000))H=H+B.sI(-this.Iq*0.100000,this.Iq*0.100000);this.Jp(
H);(A=this.JO)?A[1].call(A[0],this):null;},ML:function(){var Ac=(this.timer.AF-this.
AJ)|0;if(Ac<0)Ac=-Ac;var Co=this.D5;var CU=this.D5+this.DF;var A_=this.DL+this.DF;
var B0=false;var AO=this.CS;if(!this.AB&&(Ac>=CU)){this.AB=1;Ac=Ac-CU;this.AJ=this.
AJ+CU;}if((this.AB>0)&&(Ac>=A_)){var Bc=(Ac/A_)|0;this.AB=this.AB+Bc;Ac=Ac-(Bc*A_
);this.AJ=this.AJ+(Bc*A_);}if((this.AB>2)&&!this.Ds)this.AB=1;if(this.AB>0)Co=this.
DL;if((this.AB>=this.Ds)&&(this.Ds>0)){B0=true;AO=0.000000;}else if(Ac>=Co)AO=1.000000-((
Ac-Co)*this.Gj);else if(AO>=0.000000)AO=0.000000;if(AO!==this.CS){this.CS=AO;this.
Ht(AO);}return B0;},MK:function(){var Ac=(this.AJ-this.timer.AF)|0;var Co=this.D5;
var CU=this.D5+this.DF;var A_=this.DL+this.DF;var B0=false;var AO=this.CS;if((this.
AB>1)&&(Ac<0)){var Bc=(((-Ac+A_)-1)/A_)|0;if((this.AB-Bc)<=0)Bc=this.AB-1;this.AB=
this.AB-Bc;Ac=Ac+(Bc*A_);this.AJ=this.AJ+(Bc*A_);}if(((this.AB===1)&&(Ac<0))&&(this.
Ds>0)){this.AB=0;Ac=Ac+CU;this.AJ=this.AJ+CU;}if(((this.AB===1)&&(Ac<0))&&!this.
Ds){var Bc=(((-Ac+A_)-1)/A_)|0;Ac=Ac+(Bc*A_);this.AJ=this.AJ+(Bc*A_);}if(this.AB>
0)Co=this.DL;if(Ac<0){B0=true;AO=1.000000;}else if(Ac>=Co)AO=1.000000-((Ac-Co)*this.
Gj);else if(AO>=0.000000)AO=1.000000;if(AO!==this.CS){this.CS=AO;this.Ht(AO);}return B0;
},MJ:function(){var Ac=(this.AJ-this.timer.AF)|0;var Co=this.D5;var CU=this.D5+this.
DF;var A_=this.DL+this.DF;var B0=false;var AO=this.CS;if((this.AB>1)&&(Ac<0)){var
Bc=(((-Ac+A_)-1)/A_)|0;if((this.AB-Bc)<=0)Bc=this.AB-1;this.AB=this.AB-Bc;Ac=Ac+(
Bc*A_);this.AJ=this.AJ+(Bc*A_);}if(((this.AB===1)&&(Ac<0))&&(this.Ds>0)){this.AB=
0;Ac=Ac+CU;this.AJ=this.AJ+CU;}if(((this.AB===1)&&(Ac<0))&&!this.Ds){var Bc=(((-
Ac+A_)-1)/A_)|0;Ac=Ac+(Bc*A_);this.AJ=this.AJ+(Bc*A_);}if(this.AB>0)Co=this.DL;if(
Ac<0){B0=true;AO=0.000000;}else if(Ac>=Co)AO=(Ac-Co)*this.Gj;else if(AO>=0.000000
)AO=0.000000;if(AO!==this.CS){this.CS=AO;this.Ht(AO);}return B0;},MI:function(){
var Ac=(this.timer.AF-this.AJ)|0;if(Ac<0)Ac=-Ac;var Co=this.D5;var CU=this.D5+this.
DF;var A_=this.DL+this.DF;var B0=false;var AO=this.CS;if(!this.AB&&(Ac>=CU)){this.
AB=1;Ac=Ac-CU;this.AJ=this.AJ+CU;}if((this.AB>0)&&(Ac>=A_)){var Bc=(Ac/A_)|0;this.
AB=this.AB+Bc;Ac=Ac-(Bc*A_);this.AJ=this.AJ+(Bc*A_);}if((this.AB>2)&&!this.Ds)this.
AB=1;if(this.AB>0)Co=this.DL;if((this.AB>=this.Ds)&&(this.Ds>0)){B0=true;AO=1.000000;
}else if(Ac>=Co)AO=(Ac-Co)*this.Gj;else if(AO>=0.000000)AO=1.000000;if(AO!==this.
CS){this.CS=AO;this.Ht(AO);}return B0;},L5:function(C){if(this.EF===C)return;this.
EF=C;this.BE=null;switch(C){case 24:{this.Dh=-1.100000;this.DU=1.100000;}break;case
22:{this.Dh=-1.000000;this.DU=-2.000000;}break;case 23:{this.Dh=2.000000;this.DU=
1.000000;}break;case 25:{this.Dh=1.100000;this.DU=-1.100000;}break;case 0:{this.
Dh=0.000000;this.DU=0.000000;}break;default:{this.Dh=this.Ma;this.DU=this.Mb;}}this.
Jm=(this.Dh===this.DU)&&(this.Dh!==0.000000);this.Ko=!this.Jm&&(this.Dh!==this.DU
);},K5:function(C){if(C<15)C=15;this.DF=C;this.Gj=1.000000/C;},LI:function(C){if(
C<0)C=0;this.DL=C;},Bn:function(C){if(this.BB===C)return;this.BB=C;if(!C&&!!this.
timer){B.rA([this,this.CX],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(
D.Fi);B.rl([this,this.CX],this.timer,0);this.Ep=0;B.kD([this,this.CX],this);}},Jp:
function(J7){},_Init:function(aArg){this.__proto__=D.Fh;B.gb++;},_Done:function(
){this.__proto__=D.Fh;B.gb--;},_ReInit:function(){},_Mark:function(E){var A;if((
A=this.BE)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.timer)&&(A._cycle!=E))A.
_Mark(A._cycle=E);if((A=this.Kj)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=
this.D6)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);if((A=this.JO)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=E);},Ae:
null,_cycle:0,_observers:null,_className:"Effects::Effect"};D.B$={A2:null,J2:0,J3:
255,IH:0,Jp:function(J7){var A;this.J2=this.IH+(Math.round((this.J3-this.IH)*J7)|
0);if(!!this.A2)(A=this.A2,A[2].call(A[0],this.J2));},_Init:function(aArg){D.Fh.
_Init.call(this,aArg);this.__proto__=D.B$;},_Mark:function(E){var A;D.Fh._Mark.call(
this,E);if((A=this.A2)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);},_className:"Effects::Int32Effect"
};D.EF={Nr:0,NB:1,ND:2,NC:3,Ne:4,Ng:5,Nf:6,NU:7,NW:8,NV:9,M5:10,M7:11,M6:12,MX:13
,MZ:14,MY:15,Na:16,Nc:17,Nb:18,M2:19,M4:20,M3:21,M$:22,Nh:23,M_:24,Ni:25,M8:26};
D.Hy={Trigger:function(){B.Core.Timer.Trigger.call(this);B.pl(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=D.Hy;},_className:"Effects::EffectTimerClass"
};D.Fi={_Init:function(){D.Hy._Init.call(this,0);this.FP(15);this.Bn(true);},_variants:
function(){return this;},_this:null};D.FX={BT:B.sr(12,0,null),Ec:B.sr(12,0,null)
,_Init:function(aArg){(this.BT=[]).__proto__=D.FX.BT;(this.Ec=[]).__proto__=D.FX.
Ec;this.__proto__=D.FX;B.gb++;},_Done:function(){this.__proto__=D.FX;B.gb--;},_ReInit:
function(){},_Mark:function(E){var A;if((A=this.Ae)&&(A._cycle!=E))A._Mark(A._cycle=
E);},Ae:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};
D._Init=function(){D.B$.__proto__=D.Fh;D.Hy.__proto__=B.Core.Timer;};D.Bw=function(
E){var A;if((A=D.Fi._this)&&(A._cycle!=E))A._Done(D.Fi._this=null);};return D;})(
);

/* Embedded Wizard */