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
);if(EmWiApp.uk)throw new Error("The unit file 'Effects.js' included twice!");EmWiApp.
uk=(function(){var B=EmWiApp;var C={};

C.FW={Bp:null,timer:null,IH:null,Fd:null,H5:null,Hy:0,Eh:0,DM:0,C7:0,Fx:0.001000,
CI:0,Ap:0,AK:0,H1:0.500000,IW:3,Gd:3,FO:0.500000,CQ:3.000000,G8:0,Js:0,Jr:0,Dn:0
,Dy:1000,DA:0,DX:0,Et:0,Di:false,If:false,Jk:false,HV:false,IN:false,Ei:function(
B0){var A;if(!this.timer)return;if(!this.Eh){if(this.Jk)this.Eh=-1;else this.Eh=
1;this.Hy=this.Eh;this.AK=this.timer.AI;this.Ap=0;this.CI=-1.000000;}var BR=false;
if((this.Eh>0)&&(this.Hy>0))BR=this.Kc();else if((this.Eh<0)&&(this.Hy<0))BR=this.
Kf();else if(this.Eh>0)BR=this.Kd();else BR=this.Ke();if(BR){this.Ch(false);(A=this.
IH)?A[1].call(A[0],this):null;(A=this.Fd)?A[1].call(A[0],this):null;}},GM:function(
H){var A;if(!this.Bp&&(((this.Et===19)||(this.Et===20))||(this.Et===21))){var Go=
this.IW+1;var J5=Math.sqrt(this.H1);var EN=0.500000;var I;this.Bp=B._NewObject(C.
Gh,0);this.Bp.BB.Set(0,1.000000);this.Bp.D4.Set(0,1.000000);for(I=1;I<Go;I=I+1){
this.Bp.BB.Set(I,this.Bp.BB.Get(I-1)*J5);this.Bp.D4.Set(I,this.Bp.D4.Get(I-1)*this.
H1);EN=EN+this.Bp.BB.Get(I);}var J6=1.000000/EN;this.Bp.BB.Set(0,0.500000);EN=0.000000;
for(I=0;I<Go;I=I+1){this.Bp.BB.Set(I,this.Bp.BB.Get(I)*J6);EN=EN+this.Bp.BB.Get(
I);}this.Bp.BB.Set(Go,this.Bp.BB.Get(Go)+(1.000000-EN));}if(this.If){if(H<0.500000
)H=H*2.000000;else H=2.000000-(H*2.000000);}switch(this.Et){case 1:{H=Math.pow(H
,this.CQ);}break;case 2:{H=1.000000-H;H=1.000000-Math.pow(H,this.CQ);}break;case
3:{H=H*2.000000;if(H<1.000000)H=Math.pow(H,this.CQ)/2.000000;else{H=2.000000-H;H=(
2.000000-Math.pow(H,this.CQ))*0.500000;}}break;case 4:{H=(Math.pow(2.718000,this.
CQ*H)-1.000000)/(Math.pow(2.718000,this.CQ)-1.000000);}break;case 5:{H=1.000000-
H;H=1.000000-((Math.pow(2.718000,this.CQ*H)-1.000000)/(Math.pow(2.718000,this.CQ
)-1.000000));}break;case 6:{H=H*2.000000;if(H<1.000000)H=((Math.pow(2.718000,this.
CQ*H)-1.000000)/(Math.pow(2.718000,this.CQ)-1.000000))/2.000000;else{H=2.000000-
H;H=(2.000000-((Math.pow(2.718000,this.CQ*H)-1.000000)/(Math.pow(2.718000,this.CQ
)-1.000000)))*0.500000;}}break;case 7:{H=1.000000-Math.cos((H*90.000000)*B.ii);}
break;case 8:{H=Math.sin((H*90.000000)*B.ii);}break;case 9:{H=H*2.000000;if(H<1.000000
){H=1.000000-Math.cos((H*90.000000)*B.ii);H=H*0.500000;}else{H=2.000000-H;H=1.000000-
Math.cos((H*90.000000)*B.ii);H=(2.000000-H)*0.500000;}}break;case 10:{H=1.000000-
Math.sqrt(1.000000-(H*H));}break;case 11:{H=1.000000-H;H=Math.sqrt(1.000000-(H*H
));}break;case 12:{H=H*2.000000;if(H<1.000000){H=1.000000-Math.sqrt(1.000000-(H*
H));H=H*0.500000;}else{H=2.000000-H;H=1.000000-Math.sqrt(1.000000-(H*H));H=(2.000000-
H)*0.500000;}}break;case 13:{H=((H*H)*H)-((H*this.FO)*Math.sin((H*180.000000)*B.
ii));}break;case 14:{H=1.000000-H;H=((H*H)*H)-((H*this.FO)*Math.sin((H*180.000000
)*B.ii));H=1.000000-H;}break;case 15:{H=H*2.000000;if(H<1.000000){H=((H*H)*H)-((
H*this.FO)*Math.sin((H*180.000000)*B.ii));H=H*0.500000;}else{H=2.000000-H;H=((H*
H)*H)-((H*this.FO)*Math.sin((H*180.000000)*B.ii));H=(2.000000-H)*0.500000;}}break;
case 16:{H=((H*H)*H)*Math.sin(((H*90.000000)*(1+(4*this.Gd)))*B.ii);}break;case 17:{
H=1.000000-H;H=1.000000-(((H*H)*H)*Math.sin(((H*90.000000)*(1+(4*this.Gd)))*B.ii
));}break;case 18:{H=H*2.000000;if(H<1.000000){H=((H*H)*H)*Math.sin(((H*90.000000
)*(1+(4*this.Gd)))*B.ii);H=H*0.500000;}else{H=2.000000-H;H=((H*H)*H)*Math.sin(((
H*90.000000)*(1+(4*this.Gd)))*B.ii);H=(2.000000-H)*0.500000;}}break;case 19:{var
C9=this.Bp;var I=0;var Dc=0.000000;var B1=C9.BB.Get(0);var BC=1.000000-H;while(BC>
B1){I=I+1;Dc=B1;B1=B1+C9.BB.Get(I);}H=(BC-Dc)/(B1-Dc);if(!I)H=1.000000-(H*H);else{
H=(2.000000*H)-1.000000;H=C9.D4.Get(I)*(1.000000-(H*H));}}break;case 20:{var C9=
this.Bp;var I=0;var Dc=0.000000;var B1=C9.BB.Get(0);while(H>B1){I=I+1;Dc=B1;B1=B1+
C9.BB.Get(I);}H=(H-Dc)/(B1-Dc);if(!I)H=H*H;else{H=(2.000000*H)-1.000000;H=1.000000-(
C9.D4.Get(I)*(1.000000-(H*H)));}}break;case 21:{var C9=this.Bp;var I=0;var Dc=0.000000;
var B1=C9.BB.Get(0);var Cv=H*2.000000;var BC=Cv-1.000000;if(Cv<1.000000)BC=-BC;while(
BC>B1){I=I+1;Dc=B1;B1=B1+C9.BB.Get(I);}BC=(BC-Dc)/(B1-Dc);if(!I)BC=BC*BC;else{BC=(
2.000000*BC)-1.000000;BC=1.000000-(C9.D4.Get(I)*(1.000000-(BC*BC)));}if(Cv<1.000000
)H=0.500000*(1.000000-BC);else H=0.500000*(1.000000+BC);}break;default:{if(this.
HV){var Cv=H;var FA=1.000000-Cv;H=((FA*Cv)*(this.C7+1.000000))+(Cv*Cv);}else if(
this.IN){var Cv=H;var FA=1.000000-Cv;var Iv=Cv*Cv;var J8=FA*FA;H=(((J8*Cv)*(this.
C7+1.000000))+((FA*Iv)*(this.DM+2.000000)))+(Iv*Cv);}}}if(((this.G8>0.000000)&&(
H>0.000000))&&(H<1.000000))H=H+B.tO(-this.G8*0.100000,this.G8*0.100000);this.FP(
H);(A=this.H5)?A[1].call(A[0],this):null;},Kf:function(){var Aa=(this.timer.AI-this.
AK)|0;if(Aa<0)Aa=-Aa;var Cn=this.DX;var CL=this.DX+this.Dy;var AR=this.DA+this.Dy;
var BR=false;var AE=this.CI;if(!this.Ap&&(Aa>=CL)){this.Ap=1;Aa=Aa-CL;this.AK=this.
AK+CL;}if((this.Ap>0)&&(Aa>=AR)){var AX=(Aa/AR)|0;this.Ap=this.Ap+AX;Aa=Aa-(AX*AR
);this.AK=this.AK+(AX*AR);}if((this.Ap>2)&&!this.Dn)this.Ap=1;if(this.Ap>0)Cn=this.
DA;if((this.Ap>=this.Dn)&&(this.Dn>0)){BR=true;AE=0.000000;}else if(Aa>=Cn)AE=1.000000-((
Aa-Cn)*this.Fx);else if(AE>=0.000000)AE=0.000000;if(AE!==this.CI){this.CI=AE;this.
GM(AE);}return BR;},Ke:function(){var Aa=(this.AK-this.timer.AI)|0;var Cn=this.DX;
var CL=this.DX+this.Dy;var AR=this.DA+this.Dy;var BR=false;var AE=this.CI;if((this.
Ap>1)&&(Aa<0)){var AX=(((-Aa+AR)-1)/AR)|0;if((this.Ap-AX)<=0)AX=this.Ap-1;this.Ap=
this.Ap-AX;Aa=Aa+(AX*AR);this.AK=this.AK+(AX*AR);}if(((this.Ap===1)&&(Aa<0))&&(this.
Dn>0)){this.Ap=0;Aa=Aa+CL;this.AK=this.AK+CL;}if(((this.Ap===1)&&(Aa<0))&&!this.
Dn){var AX=(((-Aa+AR)-1)/AR)|0;Aa=Aa+(AX*AR);this.AK=this.AK+(AX*AR);}if(this.Ap>
0)Cn=this.DA;if(Aa<0){BR=true;AE=1.000000;}else if(Aa>=Cn)AE=1.000000-((Aa-Cn)*this.
Fx);else if(AE>=0.000000)AE=1.000000;if(AE!==this.CI){this.CI=AE;this.GM(AE);}return BR;
},Kd:function(){var Aa=(this.AK-this.timer.AI)|0;var Cn=this.DX;var CL=this.DX+this.
Dy;var AR=this.DA+this.Dy;var BR=false;var AE=this.CI;if((this.Ap>1)&&(Aa<0)){var
AX=(((-Aa+AR)-1)/AR)|0;if((this.Ap-AX)<=0)AX=this.Ap-1;this.Ap=this.Ap-AX;Aa=Aa+(
AX*AR);this.AK=this.AK+(AX*AR);}if(((this.Ap===1)&&(Aa<0))&&(this.Dn>0)){this.Ap=
0;Aa=Aa+CL;this.AK=this.AK+CL;}if(((this.Ap===1)&&(Aa<0))&&!this.Dn){var AX=(((-
Aa+AR)-1)/AR)|0;Aa=Aa+(AX*AR);this.AK=this.AK+(AX*AR);}if(this.Ap>0)Cn=this.DA;if(
Aa<0){BR=true;AE=0.000000;}else if(Aa>=Cn)AE=(Aa-Cn)*this.Fx;else if(AE>=0.000000
)AE=0.000000;if(AE!==this.CI){this.CI=AE;this.GM(AE);}return BR;},Kc:function(){
var Aa=(this.timer.AI-this.AK)|0;if(Aa<0)Aa=-Aa;var Cn=this.DX;var CL=this.DX+this.
Dy;var AR=this.DA+this.Dy;var BR=false;var AE=this.CI;if(!this.Ap&&(Aa>=CL)){this.
Ap=1;Aa=Aa-CL;this.AK=this.AK+CL;}if((this.Ap>0)&&(Aa>=AR)){var AX=(Aa/AR)|0;this.
Ap=this.Ap+AX;Aa=Aa-(AX*AR);this.AK=this.AK+(AX*AR);}if((this.Ap>2)&&!this.Dn)this.
Ap=1;if(this.Ap>0)Cn=this.DA;if((this.Ap>=this.Dn)&&(this.Dn>0)){BR=true;AE=1.000000;
}else if(Aa>=Cn)AE=(Aa-Cn)*this.Fx;else if(AE>=0.000000)AE=1.000000;if(AE!==this.
CI){this.CI=AE;this.GM(AE);}return BR;},Jf:function(E){if(this.Et===E)return;this.
Et=E;this.Bp=null;switch(E){case 24:{this.C7=-1.100000;this.DM=1.100000;}break;case
22:{this.C7=-1.000000;this.DM=-2.000000;}break;case 23:{this.C7=2.000000;this.DM=
1.000000;}break;case 25:{this.C7=1.100000;this.DM=-1.100000;}break;case 0:{this.
C7=0.000000;this.DM=0.000000;}break;default:{this.C7=this.Jr;this.DM=this.Js;}}this.
HV=(this.C7===this.DM)&&(this.C7!==0.000000);this.IN=!this.HV&&(this.C7!==this.DM
);},Jb:function(E){if(E<15)E=15;this.Dy=E;this.Fx=1.000000/E;},Jd:function(E){if(
E<0)E=0;this.DA=E;},Ch:function(E){if(this.Di===E)return;this.Di=E;if(!E&&!!this.
timer){B.sM([this,this.Ei],this.timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(
C.FX);B.sz([this,this.Ei],this.timer,0);this.Eh=0;B.lq([this,this.Ei],this);}},FP:
function(Im){},_Init:function(aArg){this.__proto__=C.FW;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.Bp)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.timer)&&(A._cycle!=D))A.
_Mark(A._cycle=D);if((A=this.IH)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Fd)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.H5)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null
,_cycle:0,_observers:null,_className:"Effects::Effect"};C.B8={CS:null,Ih:0,Ii:255
,Hk:0,FP:function(Im){var A;this.Ih=this.Hk+(Math.round((this.Ii-this.Hk)*Im)|0);
if(!!this.CS)(A=this.CS,A[2].call(A[0],this.Ih));},_Init:function(aArg){C.FW._Init.
call(this,aArg);this.__proto__=C.B8;},_Mark:function(D){var A;C.FW._Mark.call(this
,D);if((A=this.CS)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::Int32Effect"
};C.Et={K4:0,Lb:1,Ld:2,Lc:3,KN:4,KP:5,KO:6,Lt:7,Lv:8,Lu:9,KA:10,KC:11,KB:12,Kq:13
,Ks:14,Kr:15,KI:16,KK:17,KJ:18,Kx:19,Kz:20,Ky:21,KH:22,KQ:23,KG:24,KR:25,KE:26};
C.GX={Trigger:function(){B.Core.Timer.Trigger.call(this);B.qw(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=C.GX;},_className:"Effects::EffectTimerClass"
};C.FX={_Init:function(){C.GX._Init.call(this,0);this.Fg(15);this.Ch(true);},_variants:
function(){return this;},_this:null};C.Gh={BB:B.tA(12,0,null),D4:B.tA(12,0,null)
,_Init:function(aArg){(this.BB=[]).__proto__=C.Gh.BB;(this.D4=[]).__proto__=C.Gh.
D4;this.__proto__=C.Gh;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=
D);},S:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};
C._Init=function(){C.B8.__proto__=C.FW;C.GX.__proto__=B.Core.Timer;};C.BO=function(
D){var A;if((A=C.FX._this)&&(A._cycle!=D))A._Done(C.FX._this=null);};return C;})(
);

/* Embedded Wizard */