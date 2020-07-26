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
var AM=[0,0,100,100];
C.FH={Ce:null,timer:null,KT:null,GO:null,IV:null,Jo:0,FC:0,E3:0,Ed:0,G$:0.001000,
DF:0,AP:0,Bs:0,JV:0.500000,K_:3,HQ:3,Hs:0.500000,DL:3.000000,IU:0,LU:0,LT:0,D7:0
,ER:1000,EU:0,Fd:0,FM:0,Ex:false,J8:false,LL:false,JJ:false,K0:false,FD:function(
Z){var A;if(!this.timer)return;if(!this.FC){if(this.LL)this.FC=-1;else this.FC=1;
this.Jo=this.FC;this.Bs=this.timer.Bq;this.AP=0;this.DF=-1.000000;}var Cz=false;
if((this.FC>0)&&(this.Jo>0))Cz=this.Od();else if((this.FC<0)&&(this.Jo<0))Cz=this.
Og();else if(this.FC>0)Cz=this.Oe();else Cz=this.Of();if(Cz){this.Bd(false);(A=this.
KT)?A[1].call(A[0],this):null;(A=this.GO)?A[1].call(A[0],this):null;}},Iw:function(
I){var A;if(!this.Ce&&(((this.FM===19)||(this.FM===20))||(this.FM===21))){var H8=
this.K_+1;var N4=Math.sqrt(this.JV);var Gg=0.500000;var K;this.Ce=B._NewObject(C.
HX,0);this.Ce.Co.Set(0,1.000000);this.Ce.Fp.Set(0,1.000000);for(K=1;K<H8;K=K+1){
this.Ce.Co.Set(K,this.Ce.Co.Get(K-1)*N4);this.Ce.Fp.Set(K,this.Ce.Fp.Get(K-1)*this.
JV);Gg=Gg+this.Ce.Co.Get(K);}var N7=1.000000/Gg;this.Ce.Co.Set(0,0.500000);Gg=0.000000;
for(K=0;K<H8;K=K+1){this.Ce.Co.Set(K,this.Ce.Co.Get(K)*N7);Gg=Gg+this.Ce.Co.Get(
K);}this.Ce.Co.Set(H8,this.Ce.Co.Get(H8)+(1.000000-Gg));}if(this.J8){if(I<0.500000
)I=I*2.000000;else I=2.000000-(I*2.000000);}switch(this.FM){case 1:{I=Math.pow(I
,this.DL);}break;case 2:{I=1.000000-I;I=1.000000-Math.pow(I,this.DL);}break;case
3:{I=I*2.000000;if(I<1.000000)I=Math.pow(I,this.DL)/2.000000;else{I=2.000000-I;I=(
2.000000-Math.pow(I,this.DL))*0.500000;}}break;case 4:{I=(Math.pow(2.718000,this.
DL*I)-1.000000)/(Math.pow(2.718000,this.DL)-1.000000);}break;case 5:{I=1.000000-
I;I=1.000000-((Math.pow(2.718000,this.DL*I)-1.000000)/(Math.pow(2.718000,this.DL
)-1.000000));}break;case 6:{I=I*2.000000;if(I<1.000000)I=((Math.pow(2.718000,this.
DL*I)-1.000000)/(Math.pow(2.718000,this.DL)-1.000000))/2.000000;else{I=2.000000-
I;I=(2.000000-((Math.pow(2.718000,this.DL*I)-1.000000)/(Math.pow(2.718000,this.DL
)-1.000000)))*0.500000;}}break;case 7:{I=1.000000-Math.cos((I*90.000000)*B.ii);}
break;case 8:{I=Math.sin((I*90.000000)*B.ii);}break;case 9:{I=I*2.000000;if(I<1.000000
){I=1.000000-Math.cos((I*90.000000)*B.ii);I=I*0.500000;}else{I=2.000000-I;I=1.000000-
Math.cos((I*90.000000)*B.ii);I=(2.000000-I)*0.500000;}}break;case 10:{I=1.000000-
Math.sqrt(1.000000-(I*I));}break;case 11:{I=1.000000-I;I=Math.sqrt(1.000000-(I*I
));}break;case 12:{I=I*2.000000;if(I<1.000000){I=1.000000-Math.sqrt(1.000000-(I*
I));I=I*0.500000;}else{I=2.000000-I;I=1.000000-Math.sqrt(1.000000-(I*I));I=(2.000000-
I)*0.500000;}}break;case 13:{I=((I*I)*I)-((I*this.Hs)*Math.sin((I*180.000000)*B.
ii));}break;case 14:{I=1.000000-I;I=((I*I)*I)-((I*this.Hs)*Math.sin((I*180.000000
)*B.ii));I=1.000000-I;}break;case 15:{I=I*2.000000;if(I<1.000000){I=((I*I)*I)-((
I*this.Hs)*Math.sin((I*180.000000)*B.ii));I=I*0.500000;}else{I=2.000000-I;I=((I*
I)*I)-((I*this.Hs)*Math.sin((I*180.000000)*B.ii));I=(2.000000-I)*0.500000;}}break;
case 16:{I=((I*I)*I)*Math.sin(((I*90.000000)*(1+(4*this.HQ)))*B.ii);}break;case 17:{
I=1.000000-I;I=1.000000-(((I*I)*I)*Math.sin(((I*90.000000)*(1+(4*this.HQ)))*B.ii
));}break;case 18:{I=I*2.000000;if(I<1.000000){I=((I*I)*I)*Math.sin(((I*90.000000
)*(1+(4*this.HQ)))*B.ii);I=I*0.500000;}else{I=2.000000-I;I=((I*I)*I)*Math.sin(((
I*90.000000)*(1+(4*this.HQ)))*B.ii);I=(2.000000-I)*0.500000;}}break;case 19:{var
Ef=this.Ce;var K=0;var El=0.000000;var CK=Ef.Co.Get(0);var BY=1.000000-I;while(BY>
CK){K=K+1;El=CK;CK=CK+Ef.Co.Get(K);}I=(BY-El)/(CK-El);if(!K)I=1.000000-(I*I);else{
I=(2.000000*I)-1.000000;I=Ef.Fp.Get(K)*(1.000000-(I*I));}}break;case 20:{var Ef=
this.Ce;var K=0;var El=0.000000;var CK=Ef.Co.Get(0);while(I>CK){K=K+1;El=CK;CK=CK+
Ef.Co.Get(K);}I=(I-El)/(CK-El);if(!K)I=I*I;else{I=(2.000000*I)-1.000000;I=1.000000-(
Ef.Fp.Get(K)*(1.000000-(I*I)));}}break;case 21:{var Ef=this.Ce;var K=0;var El=0.000000;
var CK=Ef.Co.Get(0);var Do=I*2.000000;var BY=Do-1.000000;if(Do<1.000000)BY=-BY;while(
BY>CK){K=K+1;El=CK;CK=CK+Ef.Co.Get(K);}BY=(BY-El)/(CK-El);if(!K)BY=BY*BY;else{BY=(
2.000000*BY)-1.000000;BY=1.000000-(Ef.Fp.Get(K)*(1.000000-(BY*BY)));}if(Do<1.000000
)I=0.500000*(1.000000-BY);else I=0.500000*(1.000000+BY);}break;default:{if(this.
JJ){var Do=I;var Hc=1.000000-Do;I=((Hc*Do)*(this.Ed+1.000000))+(Do*Do);}else if(
this.K0){var Do=I;var Hc=1.000000-Do;var KJ=Do*Do;var N9=Hc*Hc;I=(((N9*Do)*(this.
Ed+1.000000))+((Hc*KJ)*(this.E3+2.000000)))+(KJ*Do);}}}if(((this.IU>0.000000)&&(
I>0.000000))&&(I<1.000000))I=I+B.tO(-this.IU*0.100000,this.IU*0.100000);this.GG(
I);(A=this.IV)?A[1].call(A[0],this):null;},Og:function(){var Am=(this.timer.Bq-this.
Bs)|0;if(Am<0)Am=-Am;var C8=this.Fd;var DH=this.Fd+this.ER;var BB=this.EU+this.ER;
var Cz=false;var Bl=this.DF;if(!this.AP&&(Am>=DH)){this.AP=1;Am=Am-DH;this.Bs=this.
Bs+DH;}if((this.AP>0)&&(Am>=BB)){var BI=(Am/BB)|0;this.AP=this.AP+BI;Am=Am-(BI*BB
);this.Bs=this.Bs+(BI*BB);}if((this.AP>2)&&!this.D7)this.AP=1;if(this.AP>0)C8=this.
EU;if((this.AP>=this.D7)&&(this.D7>0)){Cz=true;Bl=0.000000;}else if(Am>=C8)Bl=1.000000-((
Am-C8)*this.G$);else if(Bl>=0.000000)Bl=0.000000;if(Bl!==this.DF){this.DF=Bl;this.
Iw(Bl);}return Cz;},Of:function(){var Am=(this.Bs-this.timer.Bq)|0;var C8=this.Fd;
var DH=this.Fd+this.ER;var BB=this.EU+this.ER;var Cz=false;var Bl=this.DF;if((this.
AP>1)&&(Am<0)){var BI=(((-Am+BB)-1)/BB)|0;if((this.AP-BI)<=0)BI=this.AP-1;this.AP=
this.AP-BI;Am=Am+(BI*BB);this.Bs=this.Bs+(BI*BB);}if(((this.AP===1)&&(Am<0))&&(this.
D7>0)){this.AP=0;Am=Am+DH;this.Bs=this.Bs+DH;}if(((this.AP===1)&&(Am<0))&&!this.
D7){var BI=(((-Am+BB)-1)/BB)|0;Am=Am+(BI*BB);this.Bs=this.Bs+(BI*BB);}if(this.AP>
0)C8=this.EU;if(Am<0){Cz=true;Bl=1.000000;}else if(Am>=C8)Bl=1.000000-((Am-C8)*this.
G$);else if(Bl>=0.000000)Bl=1.000000;if(Bl!==this.DF){this.DF=Bl;this.Iw(Bl);}return Cz;
},Oe:function(){var Am=(this.Bs-this.timer.Bq)|0;var C8=this.Fd;var DH=this.Fd+this.
ER;var BB=this.EU+this.ER;var Cz=false;var Bl=this.DF;if((this.AP>1)&&(Am<0)){var
BI=(((-Am+BB)-1)/BB)|0;if((this.AP-BI)<=0)BI=this.AP-1;this.AP=this.AP-BI;Am=Am+(
BI*BB);this.Bs=this.Bs+(BI*BB);}if(((this.AP===1)&&(Am<0))&&(this.D7>0)){this.AP=
0;Am=Am+DH;this.Bs=this.Bs+DH;}if(((this.AP===1)&&(Am<0))&&!this.D7){var BI=(((-
Am+BB)-1)/BB)|0;Am=Am+(BI*BB);this.Bs=this.Bs+(BI*BB);}if(this.AP>0)C8=this.EU;if(
Am<0){Cz=true;Bl=0.000000;}else if(Am>=C8)Bl=(Am-C8)*this.G$;else if(Bl>=0.000000
)Bl=0.000000;if(Bl!==this.DF){this.DF=Bl;this.Iw(Bl);}return Cz;},Od:function(){
var Am=(this.timer.Bq-this.Bs)|0;if(Am<0)Am=-Am;var C8=this.Fd;var DH=this.Fd+this.
ER;var BB=this.EU+this.ER;var Cz=false;var Bl=this.DF;if(!this.AP&&(Am>=DH)){this.
AP=1;Am=Am-DH;this.Bs=this.Bs+DH;}if((this.AP>0)&&(Am>=BB)){var BI=(Am/BB)|0;this.
AP=this.AP+BI;Am=Am-(BI*BB);this.Bs=this.Bs+(BI*BB);}if((this.AP>2)&&!this.D7)this.
AP=1;if(this.AP>0)C8=this.EU;if((this.AP>=this.D7)&&(this.D7>0)){Cz=true;Bl=1.000000;
}else if(Am>=C8)Bl=(Am-C8)*this.G$;else if(Bl>=0.000000)Bl=1.000000;if(Bl!==this.
DF){this.DF=Bl;this.Iw(Bl);}return Cz;},HP:function(E){if(this.FM===E)return;this.
FM=E;this.Ce=null;switch(E){case 24:{this.Ed=-1.100000;this.E3=1.100000;}break;case
22:{this.Ed=-1.000000;this.E3=-2.000000;}break;case 23:{this.Ed=2.000000;this.E3=
1.000000;}break;case 25:{this.Ed=1.100000;this.E3=-1.100000;}break;case 0:{this.
Ed=0.000000;this.E3=0.000000;}break;default:{this.Ed=this.LT;this.E3=this.LU;}}this.
JJ=(this.Ed===this.E3)&&(this.Ed!==0.000000);this.K0=!this.JJ&&(this.Ed!==this.E3
);},I0:function(E){if(E<0)E=0;this.D7=E;},HM:function(E){if(E<15)E=15;this.ER=E;
this.G$=1.000000/E;},LC:function(E){if(E<0)E=0;this.EU=E;},Bd:function(E){if(this.
Ex===E)return;this.Ex=E;if(!E&&!!this.timer){B.sM([this,this.FD],this.timer,0);this.
timer=null;}if(E){this.timer=B._GetAutoObject(C.HB);B.sz([this,this.FD],this.timer
,0);this.FC=0;B.lq([this,this.FD],this);}},GG:function(FU){},_Init:function(aArg
){this.__proto__=C.FH;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.Ce)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.KT)&&((A=A[0]
)._cycle!=D))A._Mark(A._cycle=D);if((A=this.GO)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.IV)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(
A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Effects::Effect"
};C.Ck={AI:null,Gr:0,ED:255,EZ:0,GG:function(FU){var A;this.Gr=this.EZ+(Math.round((
this.ED-this.EZ)*FU)|0);if(!!this.AI)(A=this.AI,A[2].call(A[0],this.Gr));},_Init:
function(aArg){C.FH._Init.call(this,aArg);this.__proto__=C.Ck;},_Mark:function(D
){var A;C.FH._Mark.call(this,D);if((A=this.AI)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Effects::Int32Effect"};C.CP={AI:null,Gr:B.qy,ED:AM,EZ:B.qy,GG:function(
FU){var A;var U=this.EZ[0];var V=this.EZ[1];var W=this.EZ[2];var X=this.EZ[3];U=
U+(Math.round((this.ED[0]-U)*FU)|0);V=V+(Math.round((this.ED[1]-V)*FU)|0);W=W+(Math.
round((this.ED[2]-W)*FU)|0);X=X+(Math.round((this.ED[3]-X)*FU)|0);this.Gr=[U,V,W
,X];if(!!this.AI)(A=this.AI,A[2].call(A[0],this.Gr));},_Init:function(aArg){C.FH.
_Init.call(this,aArg);this.__proto__=C.CP;},_Mark:function(D){var A;C.FH._Mark.call(
this,D);if((A=this.AI)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::RectEffect"
};C.FM={O8:0,Pf:1,Ph:2,Pg:3,OU:4,OW:5,OV:6,Pw:7,Py:8,Px:9,OI:10,OK:11,OJ:12,Oy:13
,OA:14,Oz:15,OP:16,OR:17,OQ:18,OE:19,OG:20,OF:21,OO:22,OX:23,ON:24,OY:25,OL:26};
C.II={Trigger:function(){B.Core.Timer.Trigger.call(this);B.qw(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=C.II;},_className:"Effects::EffectTimerClass"
};C.HB={_Init:function(){C.II._Init.call(this,0);this.GS(15);this.Bd(true);},_variants:
function(){return this;},_this:null};C.HX={Co:B.tA(12,0,null),Fp:B.tA(12,0,null)
,_Init:function(aArg){(this.Co=[]).__proto__=C.HX.Co;(this.Fp=[]).__proto__=C.HX.
Fp;this.__proto__=C.HX;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=
D);},J:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};
C._Init=function(){C.Ck.__proto__=C.FH;C.CP.__proto__=C.FH;C.II.__proto__=B.Core.
Timer;};C.C2=function(D){var A;if((A=C.HB._this)&&(A._cycle!=D))A._Done(C.HB._this=
null);};return C;})();

/* Embedded Wizard */