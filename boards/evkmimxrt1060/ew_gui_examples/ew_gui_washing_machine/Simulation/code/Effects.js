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
);if(EmWiApp.uj)throw new Error("The unit file 'Effects.js' included twice!");EmWiApp.
uj=(function(){var B=EmWiApp;var C={};
var AX=[0,0,100,100];
C.FN={Cv:null,timer:null,G$:null,ED:null,D3:null,HN:0,Fi:0,E6:0,Ep:0,HS:0.001000,
DR:0,A8:0,Bb:0,KV:0.500000,Mc:3,Is:3,H$:0.500000,DZ:3.000000,JM:0,M8:0,M7:0,Ei:0
,EO:1000,ER:0,EQ:0,F_:0,Cq:false,Lb:false,J1:false,KO:false,L4:false,Dg:function(
Ai){var A;if(!this.timer)return;if(!this.Fi){if(this.J1)this.Fi=-1;else this.Fi=
1;this.HN=this.Fi;this.Bb=this.timer.AJ;this.A8=0;this.DR=-1.000000;}var CW=false;
if((this.Fi>0)&&(this.HN>0))CW=this.OH();else if((this.Fi<0)&&(this.HN<0))CW=this.
OK();else if(this.Fi>0)CW=this.OI();else CW=this.OJ();if(CW){this.Aj(false);(A=this.
G$)?A[1].call(A[0],this):null;(A=this.ED)?A[1].call(A[0],this):null;}},Jo:function(
G){var A;if(!this.Cv&&(((this.F_===19)||(this.F_===20))||(this.F_===21))){var IV=
this.Mc+1;var Ot=Math.sqrt(this.KV);var Gz=0.500000;var K;this.Cv=B._NewObject(C.
IH,0);this.Cv.CL.Set(0,1.000000);this.Cv.Ft.Set(0,1.000000);for(K=1;K<IV;K=K+1){
this.Cv.CL.Set(K,this.Cv.CL.Get(K-1)*Ot);this.Cv.Ft.Set(K,this.Cv.Ft.Get(K-1)*this.
KV);Gz=Gz+this.Cv.CL.Get(K);}var Ov=1.000000/Gz;this.Cv.CL.Set(0,0.500000);Gz=0.000000;
for(K=0;K<IV;K=K+1){this.Cv.CL.Set(K,this.Cv.CL.Get(K)*Ov);Gz=Gz+this.Cv.CL.Get(
K);}this.Cv.CL.Set(IV,this.Cv.CL.Get(IV)+(1.000000-Gz));}if(this.Lb){if(G<0.500000
)G=G*2.000000;else G=2.000000-(G*2.000000);}switch(this.F_){case 1:{G=Math.pow(G
,this.DZ);}break;case 2:{G=1.000000-G;G=1.000000-Math.pow(G,this.DZ);}break;case
3:{G=G*2.000000;if(G<1.000000)G=Math.pow(G,this.DZ)/2.000000;else{G=2.000000-G;G=(
2.000000-Math.pow(G,this.DZ))*0.500000;}}break;case 4:{G=(Math.pow(2.718000,this.
DZ*G)-1.000000)/(Math.pow(2.718000,this.DZ)-1.000000);}break;case 5:{G=1.000000-
G;G=1.000000-((Math.pow(2.718000,this.DZ*G)-1.000000)/(Math.pow(2.718000,this.DZ
)-1.000000));}break;case 6:{G=G*2.000000;if(G<1.000000)G=((Math.pow(2.718000,this.
DZ*G)-1.000000)/(Math.pow(2.718000,this.DZ)-1.000000))/2.000000;else{G=2.000000-
G;G=(2.000000-((Math.pow(2.718000,this.DZ*G)-1.000000)/(Math.pow(2.718000,this.DZ
)-1.000000)))*0.500000;}}break;case 7:{G=1.000000-Math.cos((G*90.000000)*B.ii);}
break;case 8:{G=Math.sin((G*90.000000)*B.ii);}break;case 9:{G=G*2.000000;if(G<1.000000
){G=1.000000-Math.cos((G*90.000000)*B.ii);G=G*0.500000;}else{G=2.000000-G;G=1.000000-
Math.cos((G*90.000000)*B.ii);G=(2.000000-G)*0.500000;}}break;case 10:{G=1.000000-
Math.sqrt(1.000000-(G*G));}break;case 11:{G=1.000000-G;G=Math.sqrt(1.000000-(G*G
));}break;case 12:{G=G*2.000000;if(G<1.000000){G=1.000000-Math.sqrt(1.000000-(G*
G));G=G*0.500000;}else{G=2.000000-G;G=1.000000-Math.sqrt(1.000000-(G*G));G=(2.000000-
G)*0.500000;}}break;case 13:{G=((G*G)*G)-((G*this.H$)*Math.sin((G*180.000000)*B.
ii));}break;case 14:{G=1.000000-G;G=((G*G)*G)-((G*this.H$)*Math.sin((G*180.000000
)*B.ii));G=1.000000-G;}break;case 15:{G=G*2.000000;if(G<1.000000){G=((G*G)*G)-((
G*this.H$)*Math.sin((G*180.000000)*B.ii));G=G*0.500000;}else{G=2.000000-G;G=((G*
G)*G)-((G*this.H$)*Math.sin((G*180.000000)*B.ii));G=(2.000000-G)*0.500000;}}break;
case 16:{G=((G*G)*G)*Math.sin(((G*90.000000)*(1+(4*this.Is)))*B.ii);}break;case 17:{
G=1.000000-G;G=1.000000-(((G*G)*G)*Math.sin(((G*90.000000)*(1+(4*this.Is)))*B.ii
));}break;case 18:{G=G*2.000000;if(G<1.000000){G=((G*G)*G)*Math.sin(((G*90.000000
)*(1+(4*this.Is)))*B.ii);G=G*0.500000;}else{G=2.000000-G;G=((G*G)*G)*Math.sin(((
G*90.000000)*(1+(4*this.Is)))*B.ii);G=(2.000000-G)*0.500000;}}break;case 19:{var
Et=this.Cv;var K=0;var Ey=0.000000;var C6=Et.CL.Get(0);var CM=1.000000-G;while(CM>
C6){K=K+1;Ey=C6;C6=C6+Et.CL.Get(K);}G=(CM-Ey)/(C6-Ey);if(!K)G=1.000000-(G*G);else{
G=(2.000000*G)-1.000000;G=Et.Ft.Get(K)*(1.000000-(G*G));}}break;case 20:{var Et=
this.Cv;var K=0;var Ey=0.000000;var C6=Et.CL.Get(0);while(G>C6){K=K+1;Ey=C6;C6=C6+
Et.CL.Get(K);}G=(G-Ey)/(C6-Ey);if(!K)G=G*G;else{G=(2.000000*G)-1.000000;G=1.000000-(
Et.Ft.Get(K)*(1.000000-(G*G)));}}break;case 21:{var Et=this.Cv;var K=0;var Ey=0.000000;
var C6=Et.CL.Get(0);var CI=G*2.000000;var CM=CI-1.000000;if(CI<1.000000)CM=-CM;while(
CM>C6){K=K+1;Ey=C6;C6=C6+Et.CL.Get(K);}CM=(CM-Ey)/(C6-Ey);if(!K)CM=CM*CM;else{CM=(
2.000000*CM)-1.000000;CM=1.000000-(Et.Ft.Get(K)*(1.000000-(CM*CM)));}if(CI<1.000000
)G=0.500000*(1.000000-CM);else G=0.500000*(1.000000+CM);}break;default:{if(this.
KO){var CI=G;var HV=1.000000-CI;G=((HV*CI)*(this.Ep+1.000000))+(CI*CI);}else if(
this.L4){var CI=G;var HV=1.000000-CI;var LI=CI*CI;var Ow=HV*HV;G=(((Ow*CI)*(this.
Ep+1.000000))+((HV*LI)*(this.E6+2.000000)))+(LI*CI);}}}if(((this.JM>0.000000)&&(
G>0.000000))&&(G<1.000000))G=G+B.tO(-this.JM*0.100000,this.JM*0.100000);this.Jt(
G);(A=this.D3)?A[1].call(A[0],this):null;},OK:function(){var Av=(this.timer.AJ-this.
Bb)|0;if(Av<0)Av=-Av;var Dp=this.EQ;var DV=this.EQ+this.EO;var BM=this.ER+this.EO;
var CW=false;var Bm=this.DR;if(!this.A8&&(Av>=DV)){this.A8=1;Av=Av-DV;this.Bb=this.
Bb+DV;}if((this.A8>0)&&(Av>=BM)){var BX=(Av/BM)|0;this.A8=this.A8+BX;Av=Av-(BX*BM
);this.Bb=this.Bb+(BX*BM);}if((this.A8>2)&&!this.Ei)this.A8=1;if(this.A8>0)Dp=this.
ER;if((this.A8>=this.Ei)&&(this.Ei>0)){CW=true;Bm=0.000000;}else if(Av>=Dp)Bm=1.000000-((
Av-Dp)*this.HS);else if(Bm>=0.000000)Bm=0.000000;if(Bm!==this.DR){this.DR=Bm;this.
Jo(Bm);}return CW;},OJ:function(){var Av=(this.Bb-this.timer.AJ)|0;var Dp=this.EQ;
var DV=this.EQ+this.EO;var BM=this.ER+this.EO;var CW=false;var Bm=this.DR;if((this.
A8>1)&&(Av<0)){var BX=(((-Av+BM)-1)/BM)|0;if((this.A8-BX)<=0)BX=this.A8-1;this.A8=
this.A8-BX;Av=Av+(BX*BM);this.Bb=this.Bb+(BX*BM);}if(((this.A8===1)&&(Av<0))&&(this.
Ei>0)){this.A8=0;Av=Av+DV;this.Bb=this.Bb+DV;}if(((this.A8===1)&&(Av<0))&&!this.
Ei){var BX=(((-Av+BM)-1)/BM)|0;Av=Av+(BX*BM);this.Bb=this.Bb+(BX*BM);}if(this.A8>
0)Dp=this.ER;if(Av<0){CW=true;Bm=1.000000;}else if(Av>=Dp)Bm=1.000000-((Av-Dp)*this.
HS);else if(Bm>=0.000000)Bm=1.000000;if(Bm!==this.DR){this.DR=Bm;this.Jo(Bm);}return CW;
},OI:function(){var Av=(this.Bb-this.timer.AJ)|0;var Dp=this.EQ;var DV=this.EQ+this.
EO;var BM=this.ER+this.EO;var CW=false;var Bm=this.DR;if((this.A8>1)&&(Av<0)){var
BX=(((-Av+BM)-1)/BM)|0;if((this.A8-BX)<=0)BX=this.A8-1;this.A8=this.A8-BX;Av=Av+(
BX*BM);this.Bb=this.Bb+(BX*BM);}if(((this.A8===1)&&(Av<0))&&(this.Ei>0)){this.A8=
0;Av=Av+DV;this.Bb=this.Bb+DV;}if(((this.A8===1)&&(Av<0))&&!this.Ei){var BX=(((-
Av+BM)-1)/BM)|0;Av=Av+(BX*BM);this.Bb=this.Bb+(BX*BM);}if(this.A8>0)Dp=this.ER;if(
Av<0){CW=true;Bm=0.000000;}else if(Av>=Dp)Bm=(Av-Dp)*this.HS;else if(Bm>=0.000000
)Bm=0.000000;if(Bm!==this.DR){this.DR=Bm;this.Jo(Bm);}return CW;},OH:function(){
var Av=(this.timer.AJ-this.Bb)|0;if(Av<0)Av=-Av;var Dp=this.EQ;var DV=this.EQ+this.
EO;var BM=this.ER+this.EO;var CW=false;var Bm=this.DR;if(!this.A8&&(Av>=DV)){this.
A8=1;Av=Av-DV;this.Bb=this.Bb+DV;}if((this.A8>0)&&(Av>=BM)){var BX=(Av/BM)|0;this.
A8=this.A8+BX;Av=Av-(BX*BM);this.Bb=this.Bb+(BX*BM);}if((this.A8>2)&&!this.Ei)this.
A8=1;if(this.A8>0)Dp=this.ER;if((this.A8>=this.Ei)&&(this.Ei>0)){CW=true;Bm=1.000000;
}else if(Av>=Dp)Bm=(Av-Dp)*this.HS;else if(Bm>=0.000000)Bm=1.000000;if(Bm!==this.
DR){this.DR=Bm;this.Jo(Bm);}return CW;},JV:function(E){if(this.J1===E)return;this.
J1=E;if(!this.Cq||!this.Fi)return;if(E)this.HN=-1;else this.HN=1;this.Bb=(this.timer.
Hl()*2)-this.Bb;},Bz:function(E){if(this.F_===E)return;this.F_=E;this.Cv=null;switch(
E){case 24:{this.Ep=-1.100000;this.E6=1.100000;}break;case 22:{this.Ep=-1.000000;
this.E6=-2.000000;}break;case 23:{this.Ep=2.000000;this.E6=1.000000;}break;case 25:{
this.Ep=1.100000;this.E6=-1.100000;}break;case 0:{this.Ep=0.000000;this.E6=0.000000;
}break;default:{this.Ep=this.M7;this.E6=this.M8;}}this.KO=(this.Ep===this.E6)&&(
this.Ep!==0.000000);this.L4=!this.KO&&(this.Ep!==this.E6);},BP:function(E){if(E<
0)E=0;this.Ei=E;},By:function(E){if(E<15)E=15;this.EO=E;this.HS=1.000000/E;},MR:
function(E){if(E<0)E=0;this.ER=E;},GP:function(E){if(E<0)E=0;this.EQ=E;},Aj:function(
E){if(this.Cq===E)return;this.Cq=E;if(!E&&!!this.timer){B.sM([this,this.Dg],this.
timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(C.GG);B.sz([this,this.
Dg],this.timer,0);this.Fi=0;B.lq([this,this.Dg],this);}},Jt:function(Gk){},M3:function(
Ai){if(this.Cq)this.Aj(false);this.Aj(true);},_Init:function(aArg){this.__proto__=
C.FN;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.Cv)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.G$)&&((A=A[0])._cycle!=
D))A._Mark(A._cycle=D);if((A=this.ED)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);
if((A=this.D3)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle
!=D))A._Mark(A._cycle=D);},J:null,_cycle:0,_observers:null,_className:"Effects::Effect"
};C.Ag={A0:null,GZ:0,Br:255,AW:0,Jt:function(Gk){var A;this.GZ=this.AW+(Math.round((
this.Br-this.AW)*Gk)|0);if(!!this.A0)(A=this.A0,A[2].call(A[0],this.GZ));},_Init:
function(aArg){C.FN._Init.call(this,aArg);this.__proto__=C.Ag;},_Mark:function(D
){var A;C.FN._Mark.call(this,D);if((A=this.A0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Effects::Int32Effect"};C.JX={A0:null,GZ:B.qy,Br:AX,AW:B.qy,Jt:function(
Gk){var A;var Y=this.AW[0];var Z=this.AW[1];var Ab=this.AW[2];var Ac=this.AW[3];
Y=Y+(Math.round((this.Br[0]-Y)*Gk)|0);Z=Z+(Math.round((this.Br[1]-Z)*Gk)|0);Ab=Ab+(
Math.round((this.Br[2]-Ab)*Gk)|0);Ac=Ac+(Math.round((this.Br[3]-Ac)*Gk)|0);this.
GZ=[Y,Z,Ab,Ac];if(!!this.A0)(A=this.A0,A[2].call(A[0],this.GZ));},_Init:function(
aArg){C.FN._Init.call(this,aArg);this.__proto__=C.JX;},_Mark:function(D){var A;C.
FN._Mark.call(this,D);if((A=this.A0)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);}
,_className:"Effects::RectEffect"};C.F_={Py:0,PG:1,PJ:2,PH:3,Pj:4,Pl:5,Pk:6,PY:7
,P0:8,PZ:9,O_:10,Pa:11,O$:12,O0:13,O2:14,O1:15,Pf:16,Ph:17,Pg:18,O7:19,O9:20,O8:
21,Pe:22,Pm:23,Pd:24,Pn:25,Pb:26};C.JC={Trigger:function(){B.Core.Timer.Trigger.
call(this);B.qw(this,0);},_Init:function(aArg){B.Core.Timer._Init.call(this,aArg
);this.__proto__=C.JC;},_className:"Effects::EffectTimerClass"};C.GG={_Init:function(
){C.JC._Init.call(this,0);this.JU(15);this.Aj(true);},_variants:function(){return this;
},_this:null};C.IH={CL:B.tA(12,0,null),Ft:B.tA(12,0,null),_Init:function(aArg){(
this.CL=[]).__proto__=C.IH.CL;(this.Ft=[]).__proto__=C.IH.Ft;this.__proto__=C.IH;
B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);},J:null,_cycle:
0,_observers:null,_className:"Effects::TimingList"};
C._Init=function(){C.Ag.__proto__=C.FN;C.JX.__proto__=C.FN;C.JC.__proto__=B.Core.
Timer;};C.Dj=function(D){var A;if((A=C.GG._this)&&(A._cycle!=D))A._Done(C.GG._this=
null);};return C;})();

/* Embedded Wizard */