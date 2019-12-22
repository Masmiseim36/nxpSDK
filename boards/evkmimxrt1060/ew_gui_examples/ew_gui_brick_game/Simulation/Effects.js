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
);if(EmWiApp.tg)throw new Error("The unit file 'Effects.js' included twice!");EmWiApp.
tg=(function(){var B=EmWiApp;var C={};

C.E1={Bm:null,timer:null,It:null,E5:null,HP:null,Hg:0,Ec:0,DF:0,C3:0,Fn:0.001000,
CE:0,Ar:0,AJ:0,HJ:0.500000,IH:3,FV:3,FD:0.500000,CM:3.000000,GM:0,I_:0,I9:0,Df:0
,Dt:1000,Dv:0,DQ:0,En:0,Da:false,H1:false,I6:false,HC:false,Iy:false,Ed:function(
BX){var A;if(!this.timer)return;if(!this.Ec){if(this.I6)this.Ec=-1;else this.Ec=
1;this.Hg=this.Ec;this.AJ=this.timer.AH;this.Ar=0;this.CE=-1.000000;}var BQ=false;
if((this.Ec>0)&&(this.Hg>0))BQ=this.JS();else if((this.Ec<0)&&(this.Hg<0))BQ=this.
JV();else if(this.Ec>0)BQ=this.JT();else BQ=this.JU();if(BQ){this.B5(false);(A=this.
It)?A[1].call(A[0],this):null;(A=this.E5)?A[1].call(A[0],this):null;}},Gy:function(
H){var A;if(!this.Bm&&(((this.En===19)||(this.En===20))||(this.En===21))){var Gb=
this.IH+1;var JK=Math.sqrt(this.HJ);var EF=0.500000;var I;this.Bm=B._NewObject(C.
Fd,0);this.Bm.BB.Set(0,1.000000);this.Bm.D0.Set(0,1.000000);for(I=1;I<Gb;I=I+1){
this.Bm.BB.Set(I,this.Bm.BB.Get(I-1)*JK);this.Bm.D0.Set(I,this.Bm.D0.Get(I-1)*this.
HJ);EF=EF+this.Bm.BB.Get(I);}var JL=1.000000/EF;this.Bm.BB.Set(0,0.500000);EF=0.000000;
for(I=0;I<Gb;I=I+1){this.Bm.BB.Set(I,this.Bm.BB.Get(I)*JL);EF=EF+this.Bm.BB.Get(
I);}this.Bm.BB.Set(Gb,this.Bm.BB.Get(Gb)+(1.000000-EF));}if(this.H1){if(H<0.500000
)H=H*2.000000;else H=2.000000-(H*2.000000);}switch(this.En){case 1:{H=Math.pow(H
,this.CM);}break;case 2:{H=1.000000-H;H=1.000000-Math.pow(H,this.CM);}break;case
3:{H=H*2.000000;if(H<1.000000)H=Math.pow(H,this.CM)/2.000000;else{H=2.000000-H;H=(
2.000000-Math.pow(H,this.CM))*0.500000;}}break;case 4:{H=(Math.pow(2.718000,this.
CM*H)-1.000000)/(Math.pow(2.718000,this.CM)-1.000000);}break;case 5:{H=1.000000-
H;H=1.000000-((Math.pow(2.718000,this.CM*H)-1.000000)/(Math.pow(2.718000,this.CM
)-1.000000));}break;case 6:{H=H*2.000000;if(H<1.000000)H=((Math.pow(2.718000,this.
CM*H)-1.000000)/(Math.pow(2.718000,this.CM)-1.000000))/2.000000;else{H=2.000000-
H;H=(2.000000-((Math.pow(2.718000,this.CM*H)-1.000000)/(Math.pow(2.718000,this.CM
)-1.000000)))*0.500000;}}break;case 7:{H=1.000000-Math.cos((H*90.000000)*B.ks);}
break;case 8:{H=Math.sin((H*90.000000)*B.ks);}break;case 9:{H=H*2.000000;if(H<1.000000
){H=1.000000-Math.cos((H*90.000000)*B.ks);H=H*0.500000;}else{H=2.000000-H;H=1.000000-
Math.cos((H*90.000000)*B.ks);H=(2.000000-H)*0.500000;}}break;case 10:{H=1.000000-
Math.sqrt(1.000000-(H*H));}break;case 11:{H=1.000000-H;H=Math.sqrt(1.000000-(H*H
));}break;case 12:{H=H*2.000000;if(H<1.000000){H=1.000000-Math.sqrt(1.000000-(H*
H));H=H*0.500000;}else{H=2.000000-H;H=1.000000-Math.sqrt(1.000000-(H*H));H=(2.000000-
H)*0.500000;}}break;case 13:{H=((H*H)*H)-((H*this.FD)*Math.sin((H*180.000000)*B.
ks));}break;case 14:{H=1.000000-H;H=((H*H)*H)-((H*this.FD)*Math.sin((H*180.000000
)*B.ks));H=1.000000-H;}break;case 15:{H=H*2.000000;if(H<1.000000){H=((H*H)*H)-((
H*this.FD)*Math.sin((H*180.000000)*B.ks));H=H*0.500000;}else{H=2.000000-H;H=((H*
H)*H)-((H*this.FD)*Math.sin((H*180.000000)*B.ks));H=(2.000000-H)*0.500000;}}break;
case 16:{H=((H*H)*H)*Math.sin(((H*90.000000)*(1+(4*this.FV)))*B.ks);}break;case 17:{
H=1.000000-H;H=1.000000-(((H*H)*H)*Math.sin(((H*90.000000)*(1+(4*this.FV)))*B.ks
));}break;case 18:{H=H*2.000000;if(H<1.000000){H=((H*H)*H)*Math.sin(((H*90.000000
)*(1+(4*this.FV)))*B.ks);H=H*0.500000;}else{H=2.000000-H;H=((H*H)*H)*Math.sin(((
H*90.000000)*(1+(4*this.FV)))*B.ks);H=(2.000000-H)*0.500000;}}break;case 19:{var
C4=this.Bm;var I=0;var C9=0.000000;var BY=C4.BB.Get(0);var BC=1.000000-H;while(BC>
BY){I=I+1;C9=BY;BY=BY+C4.BB.Get(I);}H=(BC-C9)/(BY-C9);if(!I)H=1.000000-(H*H);else{
H=(2.000000*H)-1.000000;H=C4.D0.Get(I)*(1.000000-(H*H));}}break;case 20:{var C4=
this.Bm;var I=0;var C9=0.000000;var BY=C4.BB.Get(0);while(H>BY){I=I+1;C9=BY;BY=BY+
C4.BB.Get(I);}H=(H-C9)/(BY-C9);if(!I)H=H*H;else{H=(2.000000*H)-1.000000;H=1.000000-(
C4.D0.Get(I)*(1.000000-(H*H)));}}break;case 21:{var C4=this.Bm;var I=0;var C9=0.000000;
var BY=C4.BB.Get(0);var Ct=H*2.000000;var BC=Ct-1.000000;if(Ct<1.000000)BC=-BC;while(
BC>BY){I=I+1;C9=BY;BY=BY+C4.BB.Get(I);}BC=(BC-C9)/(BY-C9);if(!I)BC=BC*BC;else{BC=(
2.000000*BC)-1.000000;BC=1.000000-(C4.D0.Get(I)*(1.000000-(BC*BC)));}if(Ct<1.000000
)H=0.500000*(1.000000-BC);else H=0.500000*(1.000000+BC);}break;default:{if(this.
HC){var Ct=H;var Fq=1.000000-Ct;H=((Fq*Ct)*(this.C3+1.000000))+(Ct*Ct);}else if(
this.Iy){var Ct=H;var Fq=1.000000-Ct;var Ih=Ct*Ct;var JN=Fq*Fq;H=(((JN*Ct)*(this.
C3+1.000000))+((Fq*Ih)*(this.DF+2.000000)))+(Ih*Ct);}}}if(((this.GM>0.000000)&&(
H>0.000000))&&(H<1.000000))H=H+B.sI(-this.GM*0.100000,this.GM*0.100000);this.FE(
H);(A=this.HP)?A[1].call(A[0],this):null;},JV:function(){var Aa=(this.timer.AH-this.
AJ)|0;if(Aa<0)Aa=-Aa;var Ck=this.DQ;var CH=this.DQ+this.Dt;var AP=this.Dv+this.Dt;
var BQ=false;var AD=this.CE;if(!this.Ar&&(Aa>=CH)){this.Ar=1;Aa=Aa-CH;this.AJ=this.
AJ+CH;}if((this.Ar>0)&&(Aa>=AP)){var AU=(Aa/AP)|0;this.Ar=this.Ar+AU;Aa=Aa-(AU*AP
);this.AJ=this.AJ+(AU*AP);}if((this.Ar>2)&&!this.Df)this.Ar=1;if(this.Ar>0)Ck=this.
Dv;if((this.Ar>=this.Df)&&(this.Df>0)){BQ=true;AD=0.000000;}else if(Aa>=Ck)AD=1.000000-((
Aa-Ck)*this.Fn);else if(AD>=0.000000)AD=0.000000;if(AD!==this.CE){this.CE=AD;this.
Gy(AD);}return BQ;},JU:function(){var Aa=(this.AJ-this.timer.AH)|0;var Ck=this.DQ;
var CH=this.DQ+this.Dt;var AP=this.Dv+this.Dt;var BQ=false;var AD=this.CE;if((this.
Ar>1)&&(Aa<0)){var AU=(((-Aa+AP)-1)/AP)|0;if((this.Ar-AU)<=0)AU=this.Ar-1;this.Ar=
this.Ar-AU;Aa=Aa+(AU*AP);this.AJ=this.AJ+(AU*AP);}if(((this.Ar===1)&&(Aa<0))&&(this.
Df>0)){this.Ar=0;Aa=Aa+CH;this.AJ=this.AJ+CH;}if(((this.Ar===1)&&(Aa<0))&&!this.
Df){var AU=(((-Aa+AP)-1)/AP)|0;Aa=Aa+(AU*AP);this.AJ=this.AJ+(AU*AP);}if(this.Ar>
0)Ck=this.Dv;if(Aa<0){BQ=true;AD=1.000000;}else if(Aa>=Ck)AD=1.000000-((Aa-Ck)*this.
Fn);else if(AD>=0.000000)AD=1.000000;if(AD!==this.CE){this.CE=AD;this.Gy(AD);}return BQ;
},JT:function(){var Aa=(this.AJ-this.timer.AH)|0;var Ck=this.DQ;var CH=this.DQ+this.
Dt;var AP=this.Dv+this.Dt;var BQ=false;var AD=this.CE;if((this.Ar>1)&&(Aa<0)){var
AU=(((-Aa+AP)-1)/AP)|0;if((this.Ar-AU)<=0)AU=this.Ar-1;this.Ar=this.Ar-AU;Aa=Aa+(
AU*AP);this.AJ=this.AJ+(AU*AP);}if(((this.Ar===1)&&(Aa<0))&&(this.Df>0)){this.Ar=
0;Aa=Aa+CH;this.AJ=this.AJ+CH;}if(((this.Ar===1)&&(Aa<0))&&!this.Df){var AU=(((-
Aa+AP)-1)/AP)|0;Aa=Aa+(AU*AP);this.AJ=this.AJ+(AU*AP);}if(this.Ar>0)Ck=this.Dv;if(
Aa<0){BQ=true;AD=0.000000;}else if(Aa>=Ck)AD=(Aa-Ck)*this.Fn;else if(AD>=0.000000
)AD=0.000000;if(AD!==this.CE){this.CE=AD;this.Gy(AD);}return BQ;},JS:function(){
var Aa=(this.timer.AH-this.AJ)|0;if(Aa<0)Aa=-Aa;var Ck=this.DQ;var CH=this.DQ+this.
Dt;var AP=this.Dv+this.Dt;var BQ=false;var AD=this.CE;if(!this.Ar&&(Aa>=CH)){this.
Ar=1;Aa=Aa-CH;this.AJ=this.AJ+CH;}if((this.Ar>0)&&(Aa>=AP)){var AU=(Aa/AP)|0;this.
Ar=this.Ar+AU;Aa=Aa-(AU*AP);this.AJ=this.AJ+(AU*AP);}if((this.Ar>2)&&!this.Df)this.
Ar=1;if(this.Ar>0)Ck=this.Dv;if((this.Ar>=this.Df)&&(this.Df>0)){BQ=true;AD=1.000000;
}else if(Aa>=Ck)AD=(Aa-Ck)*this.Fn;else if(AD>=0.000000)AD=1.000000;if(AD!==this.
CE){this.CE=AD;this.Gy(AD);}return BQ;},I1:function(E){if(this.En===E)return;this.
En=E;this.Bm=null;switch(E){case 24:{this.C3=-1.100000;this.DF=1.100000;}break;case
22:{this.C3=-1.000000;this.DF=-2.000000;}break;case 23:{this.C3=2.000000;this.DF=
1.000000;}break;case 25:{this.C3=1.100000;this.DF=-1.100000;}break;case 0:{this.
C3=0.000000;this.DF=0.000000;}break;default:{this.C3=this.I9;this.DF=this.I_;}}this.
HC=(this.C3===this.DF)&&(this.C3!==0.000000);this.Iy=!this.HC&&(this.C3!==this.DF
);},IX:function(E){if(E<15)E=15;this.Dt=E;this.Fn=1.000000/E;},IZ:function(E){if(
E<0)E=0;this.Dv=E;},B5:function(E){if(this.Da===E)return;this.Da=E;if(!E&&!!this.
timer){B.rA([this,this.Ed],this.timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(
C.FN);B.rl([this,this.Ed],this.timer,0);this.Ec=0;B.kD([this,this.Ed],this);}},FE:
function(H_){},_Init:function(aArg){this.__proto__=C.E1;B.gb++;},_Done:function(
){this.__proto__=C.E1;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Bm)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.timer)&&(A._cycle!=D))A.
_Mark(A._cycle=D);if((A=this.It)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=
this.E5)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.HP)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null
,_cycle:0,_observers:null,_className:"Effects::Effect"};C.B4={CP:null,H5:0,H6:255
,G2:0,FE:function(H_){var A;this.H5=this.G2+(Math.round((this.H6-this.G2)*H_)|0);
if(!!this.CP)(A=this.CP,A[2].call(A[0],this.H5));},_Init:function(aArg){C.E1._Init.
call(this,aArg);this.__proto__=C.B4;},_Mark:function(D){var A;C.E1._Mark.call(this
,D);if((A=this.CP)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::Int32Effect"
};C.En={KD:0,KM:1,KO:2,KN:3,Kp:4,Kr:5,Kq:6,K2:7,K4:8,K3:9,Kc:10,Ke:11,Kd:12,J6:13
,J8:14,J7:15,Kk:16,Km:17,Kl:18,J$:19,Kb:20,Ka:21,Kj:22,Ks:23,Ki:24,Kt:25,Kg:26};
C.GF={Trigger:function(){B.Core.Timer.Trigger.call(this);B.pl(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=C.GF;},_className:"Effects::EffectTimerClass"
};C.FN={_Init:function(){C.GF._Init.call(this,0);this.E8(15);this.B5(true);},_variants:
function(){return this;},_this:null};C.Fd={BB:B.sr(12,0,null),D0:B.sr(12,0,null)
,_Init:function(aArg){(this.BB=[]).__proto__=C.Fd.BB;(this.D0=[]).__proto__=C.Fd.
D0;this.__proto__=C.Fd;B.gb++;},_Done:function(){this.__proto__=C.Fd;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=
D);},T:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};
C._Init=function(){C.B4.__proto__=C.E1;C.GF.__proto__=B.Core.Timer;};C.BO=function(
D){var A;if((A=C.FN._this)&&(A._cycle!=D))A._Done(C.FN._this=null);};return C;})(
);

/* Embedded Wizard */