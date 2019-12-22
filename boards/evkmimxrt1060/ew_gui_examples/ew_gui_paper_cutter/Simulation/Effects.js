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
var AO=[0,0,100,100];
C.E7={Cb:null,timer:null,KE:null,GF:null,IB:null,I8:0,Fw:0,EY:0,D$:0,G2:0.001000,
DD:0,AR:0,Bs:0,JC:0.500000,KU:3,HI:3,Hi:0.500000,DJ:3.000000,IA:0,Lz:0,Ly:0,D3:0
,EN:1000,EQ:0,E_:0,FF:0,Ep:false,JR:false,Lu:false,Jq:false,KK:false,Fx:function(
Aa){var A;if(!this.timer)return;if(!this.Fw){if(this.Lu)this.Fw=-1;else this.Fw=
1;this.I8=this.Fw;this.Bs=this.timer.Bh;this.AR=0;this.DD=-1.000000;}var Cy=false;
if((this.Fw>0)&&(this.I8>0))Cy=this.NT();else if((this.Fw<0)&&(this.I8<0))Cy=this.
NW();else if(this.Fw>0)Cy=this.NU();else Cy=this.NV();if(Cy){this.A8(false);(A=this.
KE)?A[1].call(A[0],this):null;(A=this.GF)?A[1].call(A[0],this):null;}},Il:function(
I){var A;if(!this.Cb&&(((this.FF===19)||(this.FF===20))||(this.FF===21))){var HY=
this.KU+1;var NJ=Math.sqrt(this.JC);var F_=0.500000;var K;this.Cb=B._NewObject(C.
GS,0);this.Cb.Cq.Set(0,1.000000);this.Cb.Fk.Set(0,1.000000);for(K=1;K<HY;K=K+1){
this.Cb.Cq.Set(K,this.Cb.Cq.Get(K-1)*NJ);this.Cb.Fk.Set(K,this.Cb.Fk.Get(K-1)*this.
JC);F_=F_+this.Cb.Cq.Get(K);}var NM=1.000000/F_;this.Cb.Cq.Set(0,0.500000);F_=0.000000;
for(K=0;K<HY;K=K+1){this.Cb.Cq.Set(K,this.Cb.Cq.Get(K)*NM);F_=F_+this.Cb.Cq.Get(
K);}this.Cb.Cq.Set(HY,this.Cb.Cq.Get(HY)+(1.000000-F_));}if(this.JR){if(I<0.500000
)I=I*2.000000;else I=2.000000-(I*2.000000);}switch(this.FF){case 1:{I=Math.pow(I
,this.DJ);}break;case 2:{I=1.000000-I;I=1.000000-Math.pow(I,this.DJ);}break;case
3:{I=I*2.000000;if(I<1.000000)I=Math.pow(I,this.DJ)/2.000000;else{I=2.000000-I;I=(
2.000000-Math.pow(I,this.DJ))*0.500000;}}break;case 4:{I=(Math.pow(2.718000,this.
DJ*I)-1.000000)/(Math.pow(2.718000,this.DJ)-1.000000);}break;case 5:{I=1.000000-
I;I=1.000000-((Math.pow(2.718000,this.DJ*I)-1.000000)/(Math.pow(2.718000,this.DJ
)-1.000000));}break;case 6:{I=I*2.000000;if(I<1.000000)I=((Math.pow(2.718000,this.
DJ*I)-1.000000)/(Math.pow(2.718000,this.DJ)-1.000000))/2.000000;else{I=2.000000-
I;I=(2.000000-((Math.pow(2.718000,this.DJ*I)-1.000000)/(Math.pow(2.718000,this.DJ
)-1.000000)))*0.500000;}}break;case 7:{I=1.000000-Math.cos((I*90.000000)*B.ks);}
break;case 8:{I=Math.sin((I*90.000000)*B.ks);}break;case 9:{I=I*2.000000;if(I<1.000000
){I=1.000000-Math.cos((I*90.000000)*B.ks);I=I*0.500000;}else{I=2.000000-I;I=1.000000-
Math.cos((I*90.000000)*B.ks);I=(2.000000-I)*0.500000;}}break;case 10:{I=1.000000-
Math.sqrt(1.000000-(I*I));}break;case 11:{I=1.000000-I;I=Math.sqrt(1.000000-(I*I
));}break;case 12:{I=I*2.000000;if(I<1.000000){I=1.000000-Math.sqrt(1.000000-(I*
I));I=I*0.500000;}else{I=2.000000-I;I=1.000000-Math.sqrt(1.000000-(I*I));I=(2.000000-
I)*0.500000;}}break;case 13:{I=((I*I)*I)-((I*this.Hi)*Math.sin((I*180.000000)*B.
ks));}break;case 14:{I=1.000000-I;I=((I*I)*I)-((I*this.Hi)*Math.sin((I*180.000000
)*B.ks));I=1.000000-I;}break;case 15:{I=I*2.000000;if(I<1.000000){I=((I*I)*I)-((
I*this.Hi)*Math.sin((I*180.000000)*B.ks));I=I*0.500000;}else{I=2.000000-I;I=((I*
I)*I)-((I*this.Hi)*Math.sin((I*180.000000)*B.ks));I=(2.000000-I)*0.500000;}}break;
case 16:{I=((I*I)*I)*Math.sin(((I*90.000000)*(1+(4*this.HI)))*B.ks);}break;case 17:{
I=1.000000-I;I=1.000000-(((I*I)*I)*Math.sin(((I*90.000000)*(1+(4*this.HI)))*B.ks
));}break;case 18:{I=I*2.000000;if(I<1.000000){I=((I*I)*I)*Math.sin(((I*90.000000
)*(1+(4*this.HI)))*B.ks);I=I*0.500000;}else{I=2.000000-I;I=((I*I)*I)*Math.sin(((
I*90.000000)*(1+(4*this.HI)))*B.ks);I=(2.000000-I)*0.500000;}}break;case 19:{var
Ea=this.Cb;var K=0;var Eg=0.000000;var CH=Ea.Cq.Get(0);var BT=1.000000-I;while(BT>
CH){K=K+1;Eg=CH;CH=CH+Ea.Cq.Get(K);}I=(BT-Eg)/(CH-Eg);if(!K)I=1.000000-(I*I);else{
I=(2.000000*I)-1.000000;I=Ea.Fk.Get(K)*(1.000000-(I*I));}}break;case 20:{var Ea=
this.Cb;var K=0;var Eg=0.000000;var CH=Ea.Cq.Get(0);while(I>CH){K=K+1;Eg=CH;CH=CH+
Ea.Cq.Get(K);}I=(I-Eg)/(CH-Eg);if(!K)I=I*I;else{I=(2.000000*I)-1.000000;I=1.000000-(
Ea.Fk.Get(K)*(1.000000-(I*I)));}}break;case 21:{var Ea=this.Cb;var K=0;var Eg=0.000000;
var CH=Ea.Cq.Get(0);var Dm=I*2.000000;var BT=Dm-1.000000;if(Dm<1.000000)BT=-BT;while(
BT>CH){K=K+1;Eg=CH;CH=CH+Ea.Cq.Get(K);}BT=(BT-Eg)/(CH-Eg);if(!K)BT=BT*BT;else{BT=(
2.000000*BT)-1.000000;BT=1.000000-(Ea.Fk.Get(K)*(1.000000-(BT*BT)));}if(Dm<1.000000
)I=0.500000*(1.000000-BT);else I=0.500000*(1.000000+BT);}break;default:{if(this.
Jq){var Dm=I;var G5=1.000000-Dm;I=((G5*Dm)*(this.D$+1.000000))+(Dm*Dm);}else if(
this.KK){var Dm=I;var G5=1.000000-Dm;var Ku=Dm*Dm;var NO=G5*G5;I=(((NO*Dm)*(this.
D$+1.000000))+((G5*Ku)*(this.EY+2.000000)))+(Ku*Dm);}}}if(((this.IA>0.000000)&&(
I>0.000000))&&(I<1.000000))I=I+B.sI(-this.IA*0.100000,this.IA*0.100000);this.Gv(
I);(A=this.IB)?A[1].call(A[0],this):null;},NW:function(){var Am=(this.timer.Bh-this.
Bs)|0;if(Am<0)Am=-Am;var C4=this.E_;var DF=this.E_+this.EN;var Bz=this.EQ+this.EN;
var Cy=false;var Bl=this.DD;if(!this.AR&&(Am>=DF)){this.AR=1;Am=Am-DF;this.Bs=this.
Bs+DF;}if((this.AR>0)&&(Am>=Bz)){var BG=(Am/Bz)|0;this.AR=this.AR+BG;Am=Am-(BG*Bz
);this.Bs=this.Bs+(BG*Bz);}if((this.AR>2)&&!this.D3)this.AR=1;if(this.AR>0)C4=this.
EQ;if((this.AR>=this.D3)&&(this.D3>0)){Cy=true;Bl=0.000000;}else if(Am>=C4)Bl=1.000000-((
Am-C4)*this.G2);else if(Bl>=0.000000)Bl=0.000000;if(Bl!==this.DD){this.DD=Bl;this.
Il(Bl);}return Cy;},NV:function(){var Am=(this.Bs-this.timer.Bh)|0;var C4=this.E_;
var DF=this.E_+this.EN;var Bz=this.EQ+this.EN;var Cy=false;var Bl=this.DD;if((this.
AR>1)&&(Am<0)){var BG=(((-Am+Bz)-1)/Bz)|0;if((this.AR-BG)<=0)BG=this.AR-1;this.AR=
this.AR-BG;Am=Am+(BG*Bz);this.Bs=this.Bs+(BG*Bz);}if(((this.AR===1)&&(Am<0))&&(this.
D3>0)){this.AR=0;Am=Am+DF;this.Bs=this.Bs+DF;}if(((this.AR===1)&&(Am<0))&&!this.
D3){var BG=(((-Am+Bz)-1)/Bz)|0;Am=Am+(BG*Bz);this.Bs=this.Bs+(BG*Bz);}if(this.AR>
0)C4=this.EQ;if(Am<0){Cy=true;Bl=1.000000;}else if(Am>=C4)Bl=1.000000-((Am-C4)*this.
G2);else if(Bl>=0.000000)Bl=1.000000;if(Bl!==this.DD){this.DD=Bl;this.Il(Bl);}return Cy;
},NU:function(){var Am=(this.Bs-this.timer.Bh)|0;var C4=this.E_;var DF=this.E_+this.
EN;var Bz=this.EQ+this.EN;var Cy=false;var Bl=this.DD;if((this.AR>1)&&(Am<0)){var
BG=(((-Am+Bz)-1)/Bz)|0;if((this.AR-BG)<=0)BG=this.AR-1;this.AR=this.AR-BG;Am=Am+(
BG*Bz);this.Bs=this.Bs+(BG*Bz);}if(((this.AR===1)&&(Am<0))&&(this.D3>0)){this.AR=
0;Am=Am+DF;this.Bs=this.Bs+DF;}if(((this.AR===1)&&(Am<0))&&!this.D3){var BG=(((-
Am+Bz)-1)/Bz)|0;Am=Am+(BG*Bz);this.Bs=this.Bs+(BG*Bz);}if(this.AR>0)C4=this.EQ;if(
Am<0){Cy=true;Bl=0.000000;}else if(Am>=C4)Bl=(Am-C4)*this.G2;else if(Bl>=0.000000
)Bl=0.000000;if(Bl!==this.DD){this.DD=Bl;this.Il(Bl);}return Cy;},NT:function(){
var Am=(this.timer.Bh-this.Bs)|0;if(Am<0)Am=-Am;var C4=this.E_;var DF=this.E_+this.
EN;var Bz=this.EQ+this.EN;var Cy=false;var Bl=this.DD;if(!this.AR&&(Am>=DF)){this.
AR=1;Am=Am-DF;this.Bs=this.Bs+DF;}if((this.AR>0)&&(Am>=Bz)){var BG=(Am/Bz)|0;this.
AR=this.AR+BG;Am=Am-(BG*Bz);this.Bs=this.Bs+(BG*Bz);}if((this.AR>2)&&!this.D3)this.
AR=1;if(this.AR>0)C4=this.EQ;if((this.AR>=this.D3)&&(this.D3>0)){Cy=true;Bl=1.000000;
}else if(Am>=C4)Bl=(Am-C4)*this.G2;else if(Bl>=0.000000)Bl=1.000000;if(Bl!==this.
DD){this.DD=Bl;this.Il(Bl);}return Cy;},HH:function(E){if(this.FF===E)return;this.
FF=E;this.Cb=null;switch(E){case 24:{this.D$=-1.100000;this.EY=1.100000;}break;case
22:{this.D$=-1.000000;this.EY=-2.000000;}break;case 23:{this.D$=2.000000;this.EY=
1.000000;}break;case 25:{this.D$=1.100000;this.EY=-1.100000;}break;case 0:{this.
D$=0.000000;this.EY=0.000000;}break;default:{this.D$=this.Ly;this.EY=this.Lz;}}this.
Jq=(this.D$===this.EY)&&(this.D$!==0.000000);this.KK=!this.Jq&&(this.D$!==this.EY
);},IG:function(E){if(E<0)E=0;this.D3=E;},HE:function(E){if(E<15)E=15;this.EN=E;
this.G2=1.000000/E;},Ll:function(E){if(E<0)E=0;this.EQ=E;},A8:function(E){if(this.
Ep===E)return;this.Ep=E;if(!E&&!!this.timer){B.rA([this,this.Fx],this.timer,0);this.
timer=null;}if(E){this.timer=B._GetAutoObject(C.Ht);B.rl([this,this.Fx],this.timer
,0);this.Fw=0;B.kD([this,this.Fx],this);}},Gv:function(FM){},_Init:function(aArg
){this.__proto__=C.E7;B.gb++;},_Done:function(){this.__proto__=C.E7;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.Cb)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.KE)&&((A=A[0]
)._cycle!=D))A._Mark(A._cycle=D);if((A=this.GF)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.IB)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(
A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Effects::Effect"
};C.Ck={AK:null,Gj:0,Ev:255,EV:0,Gv:function(FM){var A;this.Gj=this.EV+(Math.round((
this.Ev-this.EV)*FM)|0);if(!!this.AK)(A=this.AK,A[2].call(A[0],this.Gj));},_Init:
function(aArg){C.E7._Init.call(this,aArg);this.__proto__=C.Ck;},_Mark:function(D
){var A;C.E7._Mark.call(this,D);if((A=this.AK)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Effects::Int32Effect"};C.CM={AK:null,Gj:B.pn,Ev:AO,EV:B.pn,Gv:function(
FM){var A;var V=this.EV[0];var W=this.EV[1];var X=this.EV[2];var Y=this.EV[3];V=
V+(Math.round((this.Ev[0]-V)*FM)|0);W=W+(Math.round((this.Ev[1]-W)*FM)|0);X=X+(Math.
round((this.Ev[2]-X)*FM)|0);Y=Y+(Math.round((this.Ev[3]-Y)*FM)|0);this.Gj=[V,W,X
,Y];if(!!this.AK)(A=this.AK,A[2].call(A[0],this.Gj));},_Init:function(aArg){C.E7.
_Init.call(this,aArg);this.__proto__=C.CM;},_Mark:function(D){var A;C.E7._Mark.call(
this,D);if((A=this.AK)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::RectEffect"
};C.FF={OH:0,OQ:1,OS:2,OR:3,Ow:4,Oy:5,Ox:6,O5:7,O7:8,O6:9,Oj:10,Om:11,Ol:12,Ob:13
,Od:14,Oc:15,Or:16,Ot:17,Os:18,Of:19,Oh:20,Og:21,Oq:22,Oz:23,Op:24,OA:25,On:26};
C.Iu={Trigger:function(){B.Core.Timer.Trigger.call(this);B.pl(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=C.Iu;},_className:"Effects::EffectTimerClass"
};C.Ht={_Init:function(){C.Iu._Init.call(this,0);this.GJ(15);this.A8(true);},_variants:
function(){return this;},_this:null};C.GS={Cq:B.sr(12,0,null),Fk:B.sr(12,0,null)
,_Init:function(aArg){(this.Cq=[]).__proto__=C.GS.Cq;(this.Fk=[]).__proto__=C.GS.
Fk;this.__proto__=C.GS;B.gb++;},_Done:function(){this.__proto__=C.GS;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=
D);},J:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};
C._Init=function(){C.Ck.__proto__=C.E7;C.CM.__proto__=C.E7;C.Iu.__proto__=B.Core.
Timer;};C.CY=function(D){var A;if((A=C.Ht._this)&&(A._cycle!=D))A._Done(C.Ht._this=
null);};return C;})();

/* Embedded Wizard */