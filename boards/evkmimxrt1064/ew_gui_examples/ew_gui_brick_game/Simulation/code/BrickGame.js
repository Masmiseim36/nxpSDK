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
);if(EmWiApp.BrickGame)throw new Error("The unit file 'BrickGame.js' included twice!"
);EmWiApp.BrickGame=(function(){var B=EmWiApp;var C={};
var Ar=[0,0,480,24];var AT=[0,0,60,24];var Cs=[60,0,120,24];var Eu=[120,0,180,24];
var Ev=[180,0,240,24];var Ew=[240,0,300,24];var Ex=[300,0,360,24];var Hm=[360,0,
420,24];var Hn=[420,0,480,24];var Jt=[0,0,480,272];var Ju=[0,228,480,272];var Jv=[
0,0,480,228];var Jw=[0,192,480,228];var Jx=[0,40,480,64];var Jy=[0,64,480,88];var
Jz=[0,88,480,112];var JA=[0,112,480,136];var JB=[0,136,480,160];var JC=[0,200];var
JD=[480,200];var JE=[480,272];var JF=[0,272];var JG=[120,220,144,244];var JH=[200
,230,290,254];var JI=[200,5,340,30];var JJ=[145,4,195,24];var JK="Score:";var JL=[
60,5,120,30];var JM=[5,4,55,24];var JN="Level:";var JO=[370,5,430,30];var JP=[315
,4,365,24];var JQ="Lost:";var JR=[0,0,100,30];var JS=[75,0,92,24];var JT=[60,0,77
,24];var JU=[45,0,62,24];var JV=[30,0,47,24];var JW=[15,0,32,24];var JX=[0,0,17,
24];
C.E_={_class:function(){return B.uj.AN;},0:{Data:function(){return B.un;},Cache:[
],_this:null}};C.Dh={_class:function(){return B.uj.Ae;},0:{FileName:"./res/BrickGameBricks.png"
,Format:B.ch,NoOfFrames:10,FrameSize:[60,24],FrameDelay:0,_this:null}};C.Ek={A_:
null,A$:null,Ba:null,Bb:null,Bc:null,Bd:null,Be:null,Bf:null,AB:8,El:function(V){
var A;var CK=B.tj(V.G,this.G);if((CK[0]>=CK[2])||(CK[1]>=CK[3]))return this.AB;var
J=this.Cw;while(!!J){var CY=(B.ui.Bh.isPrototypeOf(J)?J:null);if(!!CY&&CY.I_()){
var Ab=B.tz(CY.G,this.G.slice(0,2));CK=B.tj(V.G,Ab);if(!((CK[0]>=CK[2])||(CK[1]>=
CK[3]))){if((CK[2]-CK[0])<(CK[3]-CK[1])){if((V.G[0]<Ab[0])&&(V.AL>0.000000))V.AL=-
V.AL;if((V.G[2]>Ab[2])&&(V.AL<0.000000))V.AL=-V.AL;}else{if((V.G[1]<Ab[1])&&(V.Bn>
0.000000))V.Bn=-V.Bn;if((V.G[3]>Ab[3])&&(V.Bn<0.000000))V.Bn=-V.Bn;}if(CY.Dj===9
)return this.AB;if(CY.Dj===7)CY.L(CY.Dj-1);else if(CY.Dj===6)CY.L(CY.Dj-1);else{
B._GetAutoObject(C.Data).Fh((B._GetAutoObject(C.Data).EY+CY.Dj)+1);CY.Bi(false);
this.AB=this.AB-1;}return this.AB;}}J=J.AU;}return this.AB;},Ac:function(In){var
Ej=!!In;this.A_.Bi(Ej);this.A$.Bi(Ej);this.Ba.Bi(Ej);this.Bb.Bi(Ej);this.Bc.Bi(Ej
);this.Bd.Bi(Ej);this.Be.Bi(Ej);this.Bf.Bi(Ej);switch(In){case 1:{this.A_.L(0);this.
A$.L(0);this.Ba.L(0);this.Bb.L(0);this.Bc.L(0);this.Bd.L(0);this.Be.L(0);this.Bf.
L(0);this.AB=8;}break;case 2:{this.A_.L(1);this.A$.L(1);this.Ba.L(1);this.Bb.L(1
);this.Bc.L(1);this.Bd.L(1);this.Be.L(1);this.Bf.L(1);this.AB=8;}break;case 3:{this.
A_.L(2);this.A$.L(2);this.Ba.L(2);this.Bb.L(2);this.Bc.L(2);this.Bd.L(2);this.Be.
L(2);this.Bf.L(2);this.AB=8;}break;case 4:{this.A_.L(3);this.A$.L(3);this.Ba.L(3
);this.Bb.L(3);this.Bc.L(3);this.Bd.L(3);this.Be.L(3);this.Bf.L(3);this.AB=8;}break;
case 5:{this.A_.L(4);this.A$.L(4);this.Ba.L(4);this.Bb.L(4);this.Bc.L(4);this.Bd.
L(4);this.Be.L(4);this.Bf.L(4);this.AB=8;}break;case 6:{this.A_.L(7);this.A$.L(7
);this.Ba.L(7);this.Bb.L(7);this.Bc.L(7);this.Bd.L(7);this.Be.L(7);this.Bf.L(7);
this.AB=8;}break;case 7:{this.A_.L(8);this.A$.L(0);this.Ba.L(8);this.Bb.L(0);this.
Bc.L(0);this.Bd.L(8);this.Be.L(0);this.Bf.L(8);this.AB=8;}break;case 8:{this.A_.
L(0);this.A$.L(9);this.Ba.L(0);this.Bb.L(0);this.Bc.L(0);this.Bd.L(0);this.Be.L(
9);this.Bf.L(0);this.AB=6;}break;case 9:{this.A_.L(9);this.A$.L(0);this.Ba.L(0);
this.Bb.L(9);this.Bc.L(9);this.Bd.L(0);this.Be.L(0);this.Bf.L(9);this.AB=4;}break;
case 10:{this.A_.L(2);this.A$.L(1);this.Ba.L(2);this.Bb.L(1);this.Bc.L(2);this.Bd.
L(1);this.Be.L(2);this.Bf.L(1);this.AB=8;}break;case 11:{this.A_.L(4);this.A$.L(
8);this.Ba.L(4);this.Bb.L(8);this.Bc.L(8);this.Bd.L(4);this.Be.L(8);this.Bf.L(4);
this.AB=8;}break;case 12:{this.A_.L(1);this.A$.L(2);this.Ba.L(1);this.Bb.L(2);this.
Bc.L(1);this.Bd.L(2);this.Be.L(1);this.Bf.L(2);this.AB=8;}break;case 13:{this.A_.
L(9);this.A$.L(9);this.Ba.L(9);this.Bb.L(7);this.Bc.L(7);this.Bd.L(9);this.Be.L(
9);this.Bf.L(9);this.AB=2;}break;default:this.AB=0;}},_Init:function(aArg){B.Core.
Ai._Init.call(this,aArg);B.ui.Bh._Init.call(this.A_={S:this},0);B.ui.Bh._Init.call(
this.A$={S:this},0);B.ui.Bh._Init.call(this.Ba={S:this},0);B.ui.Bh._Init.call(this.
Bb={S:this},0);B.ui.Bh._Init.call(this.Bc={S:this},0);B.ui.Bh._Init.call(this.Bd={
S:this},0);B.ui.Bh._Init.call(this.Be={S:this},0);B.ui.Bh._Init.call(this.Bf={S:
this},0);this.__proto__=C.Ek;this.T(Ar);this.A_.T(AT);this.A_.Cf(true);this.A_.L(
0);this.A$.T(Cs);this.A$.Cf(true);this.A$.L(0);this.Ba.T(Eu);this.Ba.Cf(true);this.
Ba.L(0);this.Bb.T(Ev);this.Bb.Cf(true);this.Bb.L(0);this.Bc.T(Ew);this.Bc.Cf(true
);this.Bc.L(0);this.Bd.T(Ex);this.Bd.Cf(true);this.Bd.L(0);this.Be.T(Hm);this.Be.
Cf(true);this.Be.L(0);this.Bf.T(Hn);this.Bf.Cf(true);this.Bf.L(0);this.An(this.A_
,0);this.An(this.A$,0);this.An(this.Ba,0);this.An(this.Bb,0);this.An(this.Bc,0);
this.An(this.Bd,0);this.An(this.Be,0);this.An(this.Bf,0);this.A_.Bs(B.s$(C.Dh));
this.A$.Bs(B.s$(C.Dh));this.Ba.Bs(B.s$(C.Dh));this.Bb.Bs(B.s$(C.Dh));this.Bc.Bs(
B.s$(C.Dh));this.Bd.Bs(B.s$(C.Dh));this.Be.Bs(B.s$(C.Dh));this.Bf.Bs(B.s$(C.Dh));
},_Done:function(){this.__proto__=B.Core.Ai;this.A_._Done();this.A$._Done();this.
Ba._Done();this.Bb._Done();this.Bc._Done();this.Bd._Done();this.Be._Done();this.
Bf._Done();B.Core.Ai._Done.call(this);},_ReInit:function(){B.Core.Ai._ReInit.call(
this);this.A_._ReInit();this.A$._ReInit();this.Ba._ReInit();this.Bb._ReInit();this.
Bc._ReInit();this.Bd._ReInit();this.Be._ReInit();this.Bf._ReInit();},_Mark:function(
D){var A;B.Core.Ai._Mark.call(this,D);if((A=this.A_)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.A$)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ba)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.Bb)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Bc)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.Bd)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Be
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Bf)._cycle!=D)A._Mark(A._cycle=D);},_className:
"BrickGame::BrickRow"};C.AM={G:B.qy,AL:0,Bn:0,_Init:function(aArg){this.__proto__=
C.AM;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.S)&&(A._cycle!=D))A._Mark(A._cycle=D);},S:null
,_cycle:0,_observers:null,_className:"BrickGame::BallPosition"};C.GS={Data:null,
De:null,AM:null,CT:null,Aq:null,Br:null,BD:null,BE:null,BF:null,BG:null,BH:null,
BN:null,AC:null,Aj:null,En:null,Ep:null,ET:null,DG:null,DI:null,Do:null,DB:null,
Dl:null,DC:null,Dm:null,C1:null,HI:0,GO:0.150000,FC:0,Cc:400,Gn:true,Gi:function(
Hr){var A;B.Core.Ai.Gi.call(this,Hr);B.lq([this,this.Ic],this);this.AC.T(B.t0(this.
AC.G,B.s_(this.Aj.G)[0]-((((A=this.AC.G)[2]-A[0])/2)|0)));this.AC.T(B.t2(this.AC.
G,this.Aj.G[1]-((A=this.AC.G)[3]-A[1])));this.AM.AL=0.200000;this.AM.Bn=-1.000000;
this.De.Ch(true);this.HI=this.De.AI;this.Cc=B.s_(this.Aj.G)[0];},IC:function(B0){
var A;this.C1.Ch(false);this.Cc=this.BN.Cy[0];if(this.Cc<0)this.Cc=0;if(this.Cc>((
A=this.G)[2]-A[0]))this.Cc=(A=this.G)[2]-A[0];},FP:function(B0){var A;var D8;var
It;var Is=this.De.AI-this.HI;this.HI=this.De.AI;if(this.Gn===true){D8=Math.round((
Is*this.AM.AL)*this.GO)|0;It=Math.round((Is*this.AM.Bn)*this.GO)|0;this.AC.T(B.t0(
this.AC.G,this.AC.G[0]+D8));this.AC.T(B.t2(this.AC.G,this.AC.G[1]+It));this.AM.G=
this.AC.G;this.El(this.AM);}else{this.AC.T(B.t0(this.AC.G,B.s_(this.Aj.G)[0]-((((
A=this.AC.G)[2]-A[0])/2)|0)));this.AC.T(B.t2(this.AC.G,this.Aj.G[1]-((A=this.AC.
G)[3]-A[1])));this.AM.G=this.AC.G;}D8=((B.s_(this.Aj.G)[0]-this.Cc)/2)|0;this.Aj.
T(B.t0(this.Aj.G,this.Aj.G[0]-D8));var Bt=(((((A=this.Aq.G)[2]-A[0])-this.Aq.Ae.
FrameSize[0])*B.s_(this.Aj.G)[0])/((A=this.G)[2]-A[0]))|0;D8=((this.Aq.Bz[0]-Bt)
/4)|0;this.Aq.Ib([this.Aq.Bz[0]-D8,this.Aq.Bz[1]]);Bt=(((((A=this.Br.G)[2]-A[0])-
this.Br.Ae.FrameSize[0])*B.s_(this.Aj.G)[0])/((A=this.G)[2]-A[0]))|0;D8=((this.Br.
Bz[0]-Bt)/4)|0;this.Br.Ib([this.Br.Bz[0]-D8,this.Br.Bz[1]]);},Ic:function(B0){switch(
this.Data.EU%10){case 1:{this.BD.Ac(1);this.BE.Ac(0);this.BF.Ac(2);this.BG.Ac(0);
this.BH.Ac(1);}break;case 2:{this.BD.Ac(1);this.BE.Ac(2);this.BF.Ac(3);this.BG.Ac(
4);this.BH.Ac(5);}break;case 3:{this.BD.Ac(7);this.BE.Ac(0);this.BF.Ac(6);this.BG.
Ac(2);this.BH.Ac(0);}break;case 4:{this.BD.Ac(7);this.BE.Ac(0);this.BF.Ac(6);this.
BG.Ac(2);this.BH.Ac(8);}break;case 5:{this.BD.Ac(1);this.BE.Ac(0);this.BF.Ac(9);
this.BG.Ac(0);this.BH.Ac(9);}break;case 6:{this.BD.Ac(7);this.BE.Ac(8);this.BF.Ac(
4);this.BG.Ac(0);this.BH.Ac(9);}break;case 7:{this.BD.Ac(6);this.BE.Ac(6);this.BF.
Ac(0);this.BG.Ac(8);this.BH.Ac(9);}break;case 8:{this.BD.Ac(11);this.BE.Ac(7);this.
BF.Ac(11);this.BG.Ac(0);this.BH.Ac(9);}break;case 9:{this.BD.Ac(10);this.BE.Ac(12
);this.BF.Ac(10);this.BG.Ac(12);this.BH.Ac(13);}break;default:{this.BD.Ac(7);this.
BE.Ac(9);this.BF.Ac(11);this.BG.Ac(1);this.BH.Ac(13);}}},El:function(V){var A;var
AB=0;AB=AB+this.BD.El(V);AB=AB+this.BE.El(V);AB=AB+this.BF.El(V);AB=AB+this.BG.El(
V);AB=AB+this.BH.El(V);if(!AB){this.Data.Fh(this.Data.EY+50);this.Data.G$(this.Data.
EU+1);B.lq([this,this.Ic],this);if(this.Data.EU<25)this.GO=this.GO+0.005000;}if(((
V.G[0]<=0)&&(V.AL<0.000000))||((V.G[2]>=((A=this.G)[2]-A[0]))&&(V.AL>0.000000)))
V.AL=-V.AL;if((V.G[1]<=0)&&(V.Bn<0.000000))V.Bn=-V.Bn;if(((V.G[3]>this.Aj.G[1])&&(
V.G[3]<this.Aj.G[3]))&&(V.Bn>0.000000)){if((V.G[0]<this.Aj.G[0])&&(V.G[2]>this.Aj.
G[0])){var Hz=V.G[2]-this.Aj.G[0];var Ft=V.G[3]-this.Aj.G[1];if(((Math.pow(Hz,2.000000
)+Math.pow(Ft,2.000000))<Math.pow((A=V.G)[2]-A[0],2.000000))&&(Ft<((((A=this.Aj.
G)[3]-A[1])/2)|0))){V.Bn=-V.Bn;V.AL=V.AL+((B.s_(V.G)[0]-B.s_(this.Aj.G)[0])/((A=
this.Aj.G)[2]-A[0]));}}if((V.G[2]>this.Aj.G[2])&&(V.G[0]<this.Aj.G[2])){var Hz=this.
Aj.G[2]-V.G[0];var Ft=V.G[3]-this.Aj.G[1];if(((Math.pow(Hz,2.000000)+Math.pow(Ft
,2.000000))<Math.pow((A=V.G)[2]-A[0],2.000000))&&(Ft<((((A=this.Aj.G)[3]-A[1])/2
)|0))){V.Bn=-V.Bn;V.AL=V.AL+((B.s_(V.G)[0]-B.s_(this.Aj.G)[0])/((A=this.Aj.G)[2]-
A[0]));}}else if((V.G[0]>this.Aj.G[0])&&(V.G[2]<this.Aj.G[2])){V.Bn=-V.Bn;V.AL=V.
AL+((B.s_(V.G)[0]-B.s_(this.Aj.G)[0])/((A=this.Aj.G)[2]-A[0]));}}if(V.G[1]>=this.
G[3]){this.Gn=false;this.Data.Ha(this.Data.Ga+1);this.Data.Fh(this.Data.EY-100);
}},J7:function(B0){var A;this.C1.Ch(false);this.FC=(-((A=this.G)[2]-A[0])/64)|0;
this.HU(this);this.DG.Ch(true);},Kb:function(B0){var A;this.C1.Ch(false);this.FC=(((
A=this.G)[2]-A[0])/64)|0;this.HU(this);this.DG.Ch(true);},Iz:function(B0){this.DG.
Ch(false);},HU:function(B0){var A;if(((this.Cc>0)&&(this.FC<0))||((this.Cc<((A=this.
G)[2]-A[0]))&&(this.FC>0)))this.Cc=this.Cc+this.FC;if(this.Cc<0)this.Cc=0;if(this.
Cc>((A=this.G)[2]-A[0]))this.Cc=(A=this.G)[2]-A[0];},ID:function(B0){if(this.Gn===
false){var Cp=this.AM.Bn;if(Cp<0.000000)Cp=-Cp;if(this.AM.AL<-Cp)this.AM.AL=-Cp;
if(this.AM.AL>Cp)this.AM.AL=Cp;}this.Gn=true;},IV:function(B0){this.Cc=B.s_(this.
AM.G)[0];var Cp=this.AM.Bn;if(Cp<0.000000)Cp=-Cp;if(this.AM.AL<-Cp)this.AM.AL=-Cp;
if(this.AM.AL>Cp)this.AM.AL=Cp;this.AM.AL=this.AM.AL+B.tO(-0.010000,0.010000);},
_Init:function(aArg){B.Core.Ai._Init.call(this,aArg);B.Core.Timer._Init.call(this.
De={S:this},0);C.AM._Init.call(this.AM={S:this},0);B.ui.CT._Init.call(this.CT={S:
this},0);B.ui.Bh._Init.call(this.Aq={S:this},0);B.ui.Bh._Init.call(this.Br={S:this
},0);C.Ek._Init.call(this.BD={S:this},0);C.Ek._Init.call(this.BE={S:this},0);C.Ek.
_Init.call(this.BF={S:this},0);C.Ek._Init.call(this.BG={S:this},0);C.Ek._Init.call(
this.BH={S:this},0);B.Core.BN._Init.call(this.BN={S:this},0);B.ui.Bh._Init.call(
this.AC={S:this},0);B.ui.Bh._Init.call(this.Aj={S:this},0);B.Core.Eo._Init.call(
this.En={S:this},0);B.Core.Eo._Init.call(this.Ep={S:this},0);B.Core.Eo._Init.call(
this.ET={S:this},0);B.Core.Timer._Init.call(this.DG={S:this},0);C.Fc._Init.call(
this.DI={S:this},0);B.ui.Text._Init.call(this.Do={S:this},0);C.Fc._Init.call(this.
DB={S:this},0);B.ui.Text._Init.call(this.Dl={S:this},0);C.Fc._Init.call(this.DC={
S:this},0);B.ui.Text._Init.call(this.Dm={S:this},0);B.Core.Timer._Init.call(this.
C1={S:this},0);this.__proto__=C.GS;var A;this.T(Jt);this.De.Fg(15);this.CT.T(Ju);
this.CT.Cg(0xFF000000);this.Aq.T(Jv);this.Aq.EV(0x11);this.Aq.Bi(true);this.Br.T(
Jw);this.Br.EV(0x9);this.Br.Bi(true);this.BD.T(Jx);this.BE.T(Jy);this.BF.T(Jz);this.
BG.T(JA);this.BH.T(JB);this.BN.Hg(JC);this.BN.Hf(JD);this.BN.He(JE);this.BN.Hd(JF
);this.AC.T(JG);this.Aj.T(JH);this.Aj.Cf(true);this.En.FY=6;this.Ep.FY=7;this.ET.
FY=131;this.DG.Fg(15);this.DI.T(JI);this.DI.Hb(5);this.DI.Hh(0xFFCCCCCC);this.Do.
T(JJ);this.Do.EV(0xC);this.Do.Fi(JK);this.Do.Cg(0xFFCCCCCC);this.DB.T(JL);this.DB.
Hb(2);this.DB.Hh(0xFFCCCCCC);this.Dl.T(JM);this.Dl.EV(0xC);this.Dl.Fi(JN);this.Dl.
Cg(0xFFCCCCCC);this.DC.T(JO);this.DC.Hb(2);this.DC.Hh(0xFFCCCCCC);this.Dm.T(JP);
this.Dm.EV(0xC);this.Dm.Fi(JQ);this.Dm.Cg(0xFFCCCCCC);this.C1.Fg(10);this.C1.I$(
0);this.C1.Ch(true);this.An(this.CT,0);this.An(this.Aq,0);this.An(this.Br,0);this.
An(this.BD,0);this.An(this.BE,0);this.An(this.BF,0);this.An(this.BG,0);this.An(this.
BH,0);this.An(this.BN,0);this.An(this.AC,0);this.An(this.Aj,0);this.An(this.DI,0
);this.An(this.Do,0);this.An(this.DB,0);this.An(this.Dl,0);this.An(this.DC,0);this.
An(this.Dm,0);this.De.EX=[this,this.FP];this.Aq.Bs(B.s$(C.Hj));this.Br.Bs(B.s$(C.
Br));this.BN.G9=[this,this.IC];this.BN.D0=[this,this.ID];this.BN.DE=[this,this.IC
];this.AC.Bs(B.s$(C.AC));this.Aj.Bs(B.s$(C.Aj));this.Data=B._GetAutoObject(C.Data
);this.En.D0=[this,this.Iz];this.En.DE=[this,this.J7];this.Ep.D0=[this,this.Iz];
this.Ep.DE=[this,this.Kb];this.ET.DE=[this,this.ID];this.DG.EX=[this,this.HU];this.
DI.Hc([A=B._GetAutoObject(C.Data),A.H_,A.Fh]);this.Do.Ff(B.s$(C.E_));this.DB.Hc([
A=B._GetAutoObject(C.Data),A.H8,A.G$]);this.Dl.Ff(B.s$(C.E_));this.DC.Hc([A=B._GetAutoObject(
C.Data),A.H9,A.Ha]);this.Dm.Ff(B.s$(C.E_));this.C1.EX=[this,this.IV];},_Done:function(
){this.__proto__=B.Core.Ai;this.De._Done();this.AM._Done();this.CT._Done();this.
Aq._Done();this.Br._Done();this.BD._Done();this.BE._Done();this.BF._Done();this.
BG._Done();this.BH._Done();this.BN._Done();this.AC._Done();this.Aj._Done();this.
En._Done();this.Ep._Done();this.ET._Done();this.DG._Done();this.DI._Done();this.
Do._Done();this.DB._Done();this.Dl._Done();this.DC._Done();this.Dm._Done();this.
C1._Done();B.Core.Ai._Done.call(this);},_ReInit:function(){B.Core.Ai._ReInit.call(
this);this.De._ReInit();this.AM._ReInit();this.CT._ReInit();this.Aq._ReInit();this.
Br._ReInit();this.BD._ReInit();this.BE._ReInit();this.BF._ReInit();this.BG._ReInit(
);this.BH._ReInit();this.BN._ReInit();this.AC._ReInit();this.Aj._ReInit();this.En.
_ReInit();this.Ep._ReInit();this.ET._ReInit();this.DG._ReInit();this.DI._ReInit(
);this.Do._ReInit();this.DB._ReInit();this.Dl._ReInit();this.DC._ReInit();this.Dm.
_ReInit();this.C1._ReInit();},_Mark:function(D){var A;B.Core.Ai._Mark.call(this,
D);if((A=this.Data)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.De)._cycle!=D)
A._Mark(A._cycle=D);if((A=this.AM)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CT).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.Aq)._cycle!=D)A._Mark(A._cycle=D);if((A=
this.Br)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BD)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.BE)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BF)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.BG)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BH)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.BN)._cycle!=D)A._Mark(A._cycle=D);if((A=this.AC
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Aj)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.En)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ep)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.ET)._cycle!=D)A._Mark(A._cycle=D);if((A=this.DG)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.DI)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Do)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.DB)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Dl
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.DC)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Dm)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C1)._cycle!=D)A._Mark(A._cycle=
D);},_className:"BrickGame::BrickGame"};C.Aj={_class:function(){return B.uj.Ae;}
,0:{FileName:"./res/BrickGamePaddle.png",Format:B.ch,NoOfFrames:1,FrameSize:[90,
24],FrameDelay:0,_this:null}};C.AC={_class:function(){return B.uj.Ae;},0:{FileName:
"./res/BrickGameBall.png",Format:B.ch,NoOfFrames:1,FrameSize:[24,24],FrameDelay:
0,_this:null}};C.GV={EY:0,Ga:0,EU:1,Fh:function(E){if(E<0)E=0;if(this.EY===E)return;
this.EY=E;B.tG([this,this.H_,this.Fh],0);},Ha:function(E){if(this.Ga===E)return;
this.Ga=E;B.tG([this,this.H9,this.Ha],0);},G$:function(E){if(this.EU===E)return;
this.EU=E;B.tG([this,this.H8,this.G$],0);},H_:function(){return this.EY;},H9:function(
){return this.Ga;},H8:function(){return this.EU;},_Init:function(aArg){B.ul.GW._Init.
call(this,aArg);this.__proto__=C.GV;},_className:"BrickGame::DataClass"};C.Data={
_Init:function(){C.GV._Init.call(this,0);},_variants:function(){return this;},_this:
null};C.Lo={KY:0,KW:1,Blue:2,Green:3,Yellow:4,White:5,KD:6,KL:7,LC:8,LD:9,Kt:10,
LH:11,KX:12,K9:13};C.Br={_class:function(){return B.uj.Ae;},0:{FileName:"./res/BrickGameLandscape.png"
,Format:B.ch,NoOfFrames:1,FrameSize:[540,36],FrameDelay:0,_this:null}};C.Hj={_class:
function(){return B.uj.Ae;},0:{FileName:"./res/BrickGameStars.png",Format:B.ch,NoOfFrames:
1,FrameSize:[600,228],FrameDelay:0,_this:null}};C.Fc={CS:null,Cd:null,B4:null,B5:
null,B6:null,B7:null,BL:null,Eq:6,GT:0,Jq:0xFFFFFFFF,Gi:function(Hr){var A;var Bm=
0;var EO=this.GT;var D6=2;this.Cd.L(EO%10);this.B4.L(((EO/10)|0)%10);this.B5.L(((
EO/100)|0)%10);this.B6.L(((EO/1000)|0)%10);this.B7.L(((EO/10000)|0)%10);this.BL.
L(((EO/100000)|0)%10);if(EO<0){this.Cd.L(10);this.B4.L(10);this.B5.L(10);this.B6.
L(10);this.B7.L(10);this.BL.L(10);}if(this.Eq>5){this.BL.Bi(true);this.BL.T(B.t0(
this.BL.G,Bm));Bm=(Bm+((A=this.BL.G)[2]-A[0]))-D6;}else this.BL.Bi(false);if(this.
Eq>4){this.B7.Bi(true);this.B7.T(B.t0(this.B7.G,Bm));Bm=(Bm+((A=this.B7.G)[2]-A[
0]))-D6;}else this.B7.Bi(false);if(this.Eq>3){this.B6.Bi(true);this.B6.T(B.t0(this.
B6.G,Bm));Bm=(Bm+((A=this.B6.G)[2]-A[0]))-D6;}else this.B6.Bi(false);if(this.Eq>
2){this.B5.Bi(true);this.B5.T(B.t0(this.B5.G,Bm));Bm=(Bm+((A=this.B5.G)[2]-A[0])
)-D6;}else this.B5.Bi(false);if(this.Eq>1){this.B4.Bi(true);this.B4.T(B.t0(this.
B4.G,Bm));Bm=(Bm+((A=this.B4.G)[2]-A[0]))-D6;}else this.BL.Bi(false);this.Cd.Bi(
true);this.Cd.T(B.t0(this.Cd.G,Bm));Bm=(Bm+((A=this.Cd.G)[2]-A[0]))-D6;},Hb:function(
E){if(this.Eq===E)return;if(E<1)E=1;if(E>6)E=6;this.Eq=E;this.G2();},Ja:function(
E){if(this.GT===E)return;this.GT=E;this.G2();},HM:function(B0){var A;if(!!this.CS
)this.Ja((A=this.CS,A[1].call(A[0])));},Hc:function(E){if(B.tn(this.CS,E))return;
if(!!this.CS)B.sO([this,this.HM],this.CS,0);this.CS=E;if(!!E)B.sB([this,this.HM]
,E,0);if(!!E)B.lq([this,this.HM],this);},Hh:function(E){this.Jq=E;this.Cd.Cg(E);
this.B4.Cg(E);this.B5.Cg(E);this.B6.Cg(E);this.B7.Cg(E);this.BL.Cg(E);},_Init:function(
aArg){B.Core.Ai._Init.call(this,aArg);B.ui.Bh._Init.call(this.Cd={S:this},0);B.ui.
Bh._Init.call(this.B4={S:this},0);B.ui.Bh._Init.call(this.B5={S:this},0);B.ui.Bh.
_Init.call(this.B6={S:this},0);B.ui.Bh._Init.call(this.B7={S:this},0);B.ui.Bh._Init.
call(this.BL={S:this},0);this.__proto__=C.Fc;this.T(JR);this.Cd.T(JS);this.Cd.Cf(
true);this.B4.T(JT);this.B4.Cf(true);this.B4.L(0);this.B5.T(JU);this.B5.Cf(true);
this.B6.T(JV);this.B6.Cf(true);this.B7.T(JW);this.B7.Cf(true);this.BL.T(JX);this.
BL.Cf(true);this.An(this.Cd,0);this.An(this.B4,0);this.An(this.B5,0);this.An(this.
B6,0);this.An(this.B7,0);this.An(this.BL,0);this.Cd.Bs(B.s$(C.DZ));this.B4.Bs(B.
s$(C.DZ));this.B5.Bs(B.s$(C.DZ));this.B6.Bs(B.s$(C.DZ));this.B7.Bs(B.s$(C.DZ));this.
BL.Bs(B.s$(C.DZ));},_Done:function(){this.__proto__=B.Core.Ai;this.Cd._Done();this.
B4._Done();this.B5._Done();this.B6._Done();this.B7._Done();this.BL._Done();B.Core.
Ai._Done.call(this);},_ReInit:function(){B.Core.Ai._ReInit.call(this);this.Cd._ReInit(
);this.B4._ReInit();this.B5._ReInit();this.B6._ReInit();this.B7._ReInit();this.BL.
_ReInit();},_Mark:function(D){var A;B.Core.Ai._Mark.call(this,D);if((A=this.CS)&&((
A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cd)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.B4)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B5)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.B6)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B7)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.BL)._cycle!=D)A._Mark(A._cycle=D);},_className:
"BrickGame::LcdDisplay"};C.DZ={_class:function(){return B.uj.Ae;},0:{FileName:"./res/BrickGameLcdNumbers.png"
,Format:B.lp,NoOfFrames:11,FrameSize:[17,24],FrameDelay:0,_this:null}};
C._Init=function(){C.Ek.__proto__=B.Core.Ai;C.GS.__proto__=B.Core.Ai;C.GV.__proto__=
B.ul.GW;C.Fc.__proto__=B.Core.Ai;};C.BO=function(D){var A;if((A=C.E_[0]._this)&&(
A._cycle!=D))A._Done(C.E_[0]._this=null);if((A=C.Dh[0]._this)&&(A._cycle!=D))A._Done(
C.Dh[0]._this=null);if((A=C.Aj[0]._this)&&(A._cycle!=D))A._Done(C.Aj[0]._this=null
);if((A=C.AC[0]._this)&&(A._cycle!=D))A._Done(C.AC[0]._this=null);if((A=C.Data._this
)&&(A._cycle!=D))A._Done(C.Data._this=null);if((A=C.Br[0]._this)&&(A._cycle!=D))
A._Done(C.Br[0]._this=null);if((A=C.Hj[0]._this)&&(A._cycle!=D))A._Done(C.Hj[0].
_this=null);if((A=C.DZ[0]._this)&&(A._cycle!=D))A._Done(C.DZ[0]._this=null);};return C;
})();

/* Embedded Wizard */