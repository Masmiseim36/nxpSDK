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
);if(EmWiApp.BrickGame)throw new Error("The unit file 'BrickGame.js' included twice!"
);EmWiApp.BrickGame=(function(){var B=EmWiApp;var C={};
var At=[0,0,480,24];var AT=[0,0,60,24];var Cq=[60,0,120,24];var DW=[120,0,180,24];
var DX=[180,0,240,24];var Eo=[240,0,300,24];var Ep=[300,0,360,24];var G4=[360,0,
420,24];var G5=[420,0,480,24];var G6=[0,0,480,272];var I$=[0,228,480,272];var Ja=[
0,0,480,228];var Jb=[0,192,480,228];var Jc=[0,40,480,64];var Jd=[0,64,480,88];var
Je=[0,88,480,112];var Jf=[0,112,480,136];var Jg=[0,136,480,160];var Jh=[0,200];var
Ji=[480,200];var Jj=[480,272];var Jk=[0,272];var Jl=[120,220,144,244];var Jm=[200
,230,290,254];var Jn=[200,5,340,30];var Jo=[145,4,195,24];var Jp="Score:";var Jq=[
60,5,120,30];var Jr=[5,4,55,24];var Js="Level:";var Jt=[370,5,430,30];var Ju=[315
,4,365,24];var Jv="Lost:";var Jw=[0,0,100,30];var Jx=[75,0,92,24];var Jy=[60,0,77
,24];var Jz=[45,0,62,24];var JA=[30,0,47,24];var JB=[15,0,32,24];var JC=[0,0,17,
24];
C.EZ={_class:function(){return B.tf.AM;},0:{Data:function(){return B.tj;},Cache:[
],_this:null}};C.C$={_class:function(){return B.tf.Ae;},0:{FileName:"./res/BrickGameBricks.png"
,Format:B.b9,NoOfFrames:10,FrameSize:[60,24],FrameDelay:0,_this:null}};C.DO={A4:
null,A5:null,A6:null,A7:null,A8:null,A9:null,A_:null,A$:null,AA:8,Ef:function(W){
var A;var CG=B.r_(W.G,this.G);if((CG[0]>=CG[2])||(CG[1]>=CG[3]))return this.AA;var
J=this.B9;while(!!J){var CT=(B.te.Bc.isPrototypeOf(J)?J:null);if(!!CT&&CT.IU()){
var Ab=B.sq(CT.G,this.G.slice(0,2));CG=B.r_(W.G,Ab);if(!((CG[0]>=CG[2])||(CG[1]>=
CG[3]))){if((CG[2]-CG[0])<(CG[3]-CG[1])){if((W.G[0]<Ab[0])&&(W.AL>0.000000))W.AL=-
W.AL;if((W.G[2]>Ab[2])&&(W.AL<0.000000))W.AL=-W.AL;}else{if((W.G[1]<Ab[1])&&(W.Bk>
0.000000))W.Bk=-W.Bk;if((W.G[3]>Ab[3])&&(W.Bk<0.000000))W.Bk=-W.Bk;}if(CT.Db===9
)return this.AA;if(CT.Db===7)CT.L(CT.Db-1);else if(CT.Db===6)CT.L(CT.Db-1);else{
B._GetAutoObject(C.Data).E9((B._GetAutoObject(C.Data).ER+CT.Db)+1);CT.Bd(false);
this.AA=this.AA-1;}return this.AA;}}J=J.Ap;}return this.AA;},Ac:function(H$){var
Ee=!!H$;this.A4.Bd(Ee);this.A5.Bd(Ee);this.A6.Bd(Ee);this.A7.Bd(Ee);this.A8.Bd(Ee
);this.A9.Bd(Ee);this.A_.Bd(Ee);this.A$.Bd(Ee);switch(H$){case 1:{this.A4.L(0);this.
A5.L(0);this.A6.L(0);this.A7.L(0);this.A8.L(0);this.A9.L(0);this.A_.L(0);this.A$.
L(0);this.AA=8;}break;case 2:{this.A4.L(1);this.A5.L(1);this.A6.L(1);this.A7.L(1
);this.A8.L(1);this.A9.L(1);this.A_.L(1);this.A$.L(1);this.AA=8;}break;case 3:{this.
A4.L(2);this.A5.L(2);this.A6.L(2);this.A7.L(2);this.A8.L(2);this.A9.L(2);this.A_.
L(2);this.A$.L(2);this.AA=8;}break;case 4:{this.A4.L(3);this.A5.L(3);this.A6.L(3
);this.A7.L(3);this.A8.L(3);this.A9.L(3);this.A_.L(3);this.A$.L(3);this.AA=8;}break;
case 5:{this.A4.L(4);this.A5.L(4);this.A6.L(4);this.A7.L(4);this.A8.L(4);this.A9.
L(4);this.A_.L(4);this.A$.L(4);this.AA=8;}break;case 6:{this.A4.L(7);this.A5.L(7
);this.A6.L(7);this.A7.L(7);this.A8.L(7);this.A9.L(7);this.A_.L(7);this.A$.L(7);
this.AA=8;}break;case 7:{this.A4.L(8);this.A5.L(0);this.A6.L(8);this.A7.L(0);this.
A8.L(0);this.A9.L(8);this.A_.L(0);this.A$.L(8);this.AA=8;}break;case 8:{this.A4.
L(0);this.A5.L(9);this.A6.L(0);this.A7.L(0);this.A8.L(0);this.A9.L(0);this.A_.L(
9);this.A$.L(0);this.AA=6;}break;case 9:{this.A4.L(9);this.A5.L(0);this.A6.L(0);
this.A7.L(9);this.A8.L(9);this.A9.L(0);this.A_.L(0);this.A$.L(9);this.AA=4;}break;
case 10:{this.A4.L(2);this.A5.L(1);this.A6.L(2);this.A7.L(1);this.A8.L(2);this.A9.
L(1);this.A_.L(2);this.A$.L(1);this.AA=8;}break;case 11:{this.A4.L(4);this.A5.L(
8);this.A6.L(4);this.A7.L(8);this.A8.L(8);this.A9.L(4);this.A_.L(8);this.A$.L(4);
this.AA=8;}break;case 12:{this.A4.L(1);this.A5.L(2);this.A6.L(1);this.A7.L(2);this.
A8.L(1);this.A9.L(2);this.A_.L(1);this.A$.L(2);this.AA=8;}break;case 13:{this.A4.
L(9);this.A5.L(9);this.A6.L(9);this.A7.L(7);this.A8.L(7);this.A9.L(9);this.A_.L(
9);this.A$.L(9);this.AA=2;}break;default:this.AA=0;}},_Init:function(aArg){B.Core.
Al._Init.call(this,aArg);B.te.Bc._Init.call(this.A4={T:this},0);B.te.Bc._Init.call(
this.A5={T:this},0);B.te.Bc._Init.call(this.A6={T:this},0);B.te.Bc._Init.call(this.
A7={T:this},0);B.te.Bc._Init.call(this.A8={T:this},0);B.te.Bc._Init.call(this.A9={
T:this},0);B.te.Bc._Init.call(this.A_={T:this},0);B.te.Bc._Init.call(this.A$={T:
this},0);this.__proto__=C.DO;this.U(At);this.A4.U(AT);this.A4.Cd(true);this.A4.L(
0);this.A5.U(Cq);this.A5.Cd(true);this.A5.L(0);this.A6.U(DW);this.A6.Cd(true);this.
A6.L(0);this.A7.U(DX);this.A7.Cd(true);this.A7.L(0);this.A8.U(Eo);this.A8.Cd(true
);this.A8.L(0);this.A9.U(Ep);this.A9.Cd(true);this.A9.L(0);this.A_.U(G4);this.A_.
Cd(true);this.A_.L(0);this.A$.U(G5);this.A$.Cd(true);this.A$.L(0);this.Ak(this.A4
,0);this.Ak(this.A5,0);this.Ak(this.A6,0);this.Ak(this.A7,0);this.Ak(this.A8,0);
this.Ak(this.A9,0);this.Ak(this.A_,0);this.Ak(this.A$,0);this.A4.Bp(B.r0(C.C$));
this.A5.Bp(B.r0(C.C$));this.A6.Bp(B.r0(C.C$));this.A7.Bp(B.r0(C.C$));this.A8.Bp(
B.r0(C.C$));this.A9.Bp(B.r0(C.C$));this.A_.Bp(B.r0(C.C$));this.A$.Bp(B.r0(C.C$));
},_Done:function(){this.__proto__=C.DO;this.A4._Done();this.A5._Done();this.A6._Done(
);this.A7._Done();this.A8._Done();this.A9._Done();this.A_._Done();this.A$._Done(
);B.Core.Al._Done.call(this);},_ReInit:function(){B.Core.Al._ReInit.call(this);this.
A4._ReInit();this.A5._ReInit();this.A6._ReInit();this.A7._ReInit();this.A8._ReInit(
);this.A9._ReInit();this.A_._ReInit();this.A$._ReInit();},_Mark:function(D){var A;
B.Core.Al._Mark.call(this,D);if((A=this.A4)._cycle!=D)A._Mark(A._cycle=D);if((A=
this.A5)._cycle!=D)A._Mark(A._cycle=D);if((A=this.A6)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.A7)._cycle!=D)A._Mark(A._cycle=D);if((A=this.A8)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.A9)._cycle!=D)A._Mark(A._cycle=D);if((A=this.A_)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.A$)._cycle!=D)A._Mark(A._cycle=D);},_className:
"BrickGame::BrickRow"};C.AK={G:B.pn,AL:0,Bk:0,_Init:function(aArg){this.__proto__=
C.AK;B.gb++;},_Done:function(){this.__proto__=C.AK;B.gb--;},_ReInit:function(){}
,_Mark:function(D){var A;if((A=this.T)&&(A._cycle!=D))A._Mark(A._cycle=D);},T:null
,_cycle:0,_observers:null,_className:"BrickGame::BallPosition"};C.FH={Data:null,
C_:null,AK:null,CQ:null,As:null,Bo:null,BD:null,BE:null,BF:null,BG:null,BH:null,
BM:null,AB:null,Aj:null,Eh:null,Ei:null,EL:null,DB:null,DC:null,Dg:null,Dw:null,
Dd:null,Dx:null,De:null,CX:null,Hq:0,Gz:0.150000,Fs:0,B$:400,Ga:true,FY:function(
G_){var A;B.Core.Al.FY.call(this,G_);B.kD([this,this.HZ],this);this.AB.U(B.sV(this.
AB.G,B.rZ(this.Aj.G)[0]-((((A=this.AB.G)[2]-A[0])/2)|0)));this.AB.U(B.sX(this.AB.
G,this.Aj.G[1]-((A=this.AB.G)[3]-A[1])));this.AK.AL=0.200000;this.AK.Bk=-1.000000;
this.C_.B5(true);this.Hq=this.C_.AH;this.B$=B.rZ(this.Aj.G)[0];},Io:function(BX){
var A;this.CX.B5(false);this.B$=this.BM.Cw[0];if(this.B$<0)this.B$=0;if(this.B$>((
A=this.G)[2]-A[0]))this.B$=(A=this.G)[2]-A[0];},FE:function(BX){var A;var D5;var
Ig;var If=this.C_.AH-this.Hq;this.Hq=this.C_.AH;if(this.Ga===true){D5=Math.round((
If*this.AK.AL)*this.Gz)|0;Ig=Math.round((If*this.AK.Bk)*this.Gz)|0;this.AB.U(B.sV(
this.AB.G,this.AB.G[0]+D5));this.AB.U(B.sX(this.AB.G,this.AB.G[1]+Ig));this.AK.G=
this.AB.G;this.Ef(this.AK);}else{this.AB.U(B.sV(this.AB.G,B.rZ(this.Aj.G)[0]-((((
A=this.AB.G)[2]-A[0])/2)|0)));this.AB.U(B.sX(this.AB.G,this.Aj.G[1]-((A=this.AB.
G)[3]-A[1])));this.AK.G=this.AB.G;}D5=((B.rZ(this.Aj.G)[0]-this.B$)/2)|0;this.Aj.
U(B.sV(this.Aj.G,this.Aj.G[0]-D5));var Bs=(((((A=this.As.G)[2]-A[0])-this.As.Ae.
FrameSize[0])*B.rZ(this.Aj.G)[0])/((A=this.G)[2]-A[0]))|0;D5=((this.As.By[0]-Bs)
/4)|0;this.As.HX([this.As.By[0]-D5,this.As.By[1]]);Bs=(((((A=this.Bo.G)[2]-A[0])-
this.Bo.Ae.FrameSize[0])*B.rZ(this.Aj.G)[0])/((A=this.G)[2]-A[0]))|0;D5=((this.Bo.
By[0]-Bs)/4)|0;this.Bo.HX([this.Bo.By[0]-D5,this.Bo.By[1]]);},HZ:function(BX){switch(
this.Data.EN%10){case 1:{this.BD.Ac(1);this.BE.Ac(0);this.BF.Ac(2);this.BG.Ac(0);
this.BH.Ac(1);}break;case 2:{this.BD.Ac(1);this.BE.Ac(2);this.BF.Ac(3);this.BG.Ac(
4);this.BH.Ac(5);}break;case 3:{this.BD.Ac(7);this.BE.Ac(0);this.BF.Ac(6);this.BG.
Ac(2);this.BH.Ac(0);}break;case 4:{this.BD.Ac(7);this.BE.Ac(0);this.BF.Ac(6);this.
BG.Ac(2);this.BH.Ac(8);}break;case 5:{this.BD.Ac(1);this.BE.Ac(0);this.BF.Ac(9);
this.BG.Ac(0);this.BH.Ac(9);}break;case 6:{this.BD.Ac(7);this.BE.Ac(8);this.BF.Ac(
4);this.BG.Ac(0);this.BH.Ac(9);}break;case 7:{this.BD.Ac(6);this.BE.Ac(6);this.BF.
Ac(0);this.BG.Ac(8);this.BH.Ac(9);}break;case 8:{this.BD.Ac(11);this.BE.Ac(7);this.
BF.Ac(11);this.BG.Ac(0);this.BH.Ac(9);}break;case 9:{this.BD.Ac(10);this.BE.Ac(12
);this.BF.Ac(10);this.BG.Ac(12);this.BH.Ac(13);}break;default:{this.BD.Ac(7);this.
BE.Ac(9);this.BF.Ac(11);this.BG.Ac(1);this.BH.Ac(13);}}},Ef:function(W){var A;var
AA=0;AA=AA+this.BD.Ef(W);AA=AA+this.BE.Ef(W);AA=AA+this.BF.Ef(W);AA=AA+this.BG.Ef(
W);AA=AA+this.BH.Ef(W);if(!AA){this.Data.E9(this.Data.ER+50);this.Data.GP(this.Data.
EN+1);B.kD([this,this.HZ],this);if(this.Data.EN<25)this.Gz=this.Gz+0.005000;}if(((
W.G[0]<=0)&&(W.AL<0.000000))||((W.G[2]>=((A=this.G)[2]-A[0]))&&(W.AL>0.000000)))
W.AL=-W.AL;if((W.G[1]<=0)&&(W.Bk<0.000000))W.Bk=-W.Bk;if(((W.G[3]>this.Aj.G[1])&&(
W.G[3]<this.Aj.G[3]))&&(W.Bk>0.000000)){if((W.G[0]<this.Aj.G[0])&&(W.G[2]>this.Aj.
G[0])){var Hh=W.G[2]-this.Aj.G[0];var Fj=W.G[3]-this.Aj.G[1];if(((Math.pow(Hh,2.000000
)+Math.pow(Fj,2.000000))<Math.pow((A=W.G)[2]-A[0],2.000000))&&(Fj<((((A=this.Aj.
G)[3]-A[1])/2)|0))){W.Bk=-W.Bk;W.AL=W.AL+((B.rZ(W.G)[0]-B.rZ(this.Aj.G)[0])/((A=
this.Aj.G)[2]-A[0]));}}if((W.G[2]>this.Aj.G[2])&&(W.G[0]<this.Aj.G[2])){var Hh=this.
Aj.G[2]-W.G[0];var Fj=W.G[3]-this.Aj.G[1];if(((Math.pow(Hh,2.000000)+Math.pow(Fj
,2.000000))<Math.pow((A=W.G)[2]-A[0],2.000000))&&(Fj<((((A=this.Aj.G)[3]-A[1])/2
)|0))){W.Bk=-W.Bk;W.AL=W.AL+((B.rZ(W.G)[0]-B.rZ(this.Aj.G)[0])/((A=this.Aj.G)[2]-
A[0]));}}else if((W.G[0]>this.Aj.G[0])&&(W.G[2]<this.Aj.G[2])){W.Bk=-W.Bk;W.AL=W.
AL+((B.rZ(W.G)[0]-B.rZ(this.Aj.G)[0])/((A=this.Aj.G)[2]-A[0]));}}if(W.G[1]>=this.
G[3]){this.Ga=false;this.Data.GQ(this.Data.FS+1);this.Data.E9(this.Data.ER-100);
}},JM:function(BX){var A;this.CX.B5(false);this.Fs=(-((A=this.G)[2]-A[0])/64)|0;
this.HB(this);this.DB.B5(true);},JR:function(BX){var A;this.CX.B5(false);this.Fs=(((
A=this.G)[2]-A[0])/64)|0;this.HB(this);this.DB.B5(true);},Il:function(BX){this.DB.
B5(false);},HB:function(BX){var A;if(((this.B$>0)&&(this.Fs<0))||((this.B$<((A=this.
G)[2]-A[0]))&&(this.Fs>0)))this.B$=this.B$+this.Fs;if(this.B$<0)this.B$=0;if(this.
B$>((A=this.G)[2]-A[0]))this.B$=(A=this.G)[2]-A[0];},Ip:function(BX){if(this.Ga===
false){var Cm=this.AK.Bk;if(Cm<0.000000)Cm=-Cm;if(this.AK.AL<-Cm)this.AK.AL=-Cm;
if(this.AK.AL>Cm)this.AK.AL=Cm;}this.Ga=true;},IG:function(BX){this.B$=B.rZ(this.
AK.G)[0];var Cm=this.AK.Bk;if(Cm<0.000000)Cm=-Cm;if(this.AK.AL<-Cm)this.AK.AL=-Cm;
if(this.AK.AL>Cm)this.AK.AL=Cm;this.AK.AL=this.AK.AL+B.sI(-0.010000,0.010000);},
_Init:function(aArg){B.Core.Al._Init.call(this,aArg);B.Core.Timer._Init.call(this.
C_={T:this},0);C.AK._Init.call(this.AK={T:this},0);B.te.CQ._Init.call(this.CQ={T:
this},0);B.te.Bc._Init.call(this.As={T:this},0);B.te.Bc._Init.call(this.Bo={T:this
},0);C.DO._Init.call(this.BD={T:this},0);C.DO._Init.call(this.BE={T:this},0);C.DO.
_Init.call(this.BF={T:this},0);C.DO._Init.call(this.BG={T:this},0);C.DO._Init.call(
this.BH={T:this},0);B.Core.BM._Init.call(this.BM={T:this},0);B.te.Bc._Init.call(
this.AB={T:this},0);B.te.Bc._Init.call(this.Aj={T:this},0);B.Core.DR._Init.call(
this.Eh={T:this},0);B.Core.DR._Init.call(this.Ei={T:this},0);B.Core.DR._Init.call(
this.EL={T:this},0);B.Core.Timer._Init.call(this.DB={T:this},0);C.EM._Init.call(
this.DC={T:this},0);B.te.Text._Init.call(this.Dg={T:this},0);C.EM._Init.call(this.
Dw={T:this},0);B.te.Text._Init.call(this.Dd={T:this},0);C.EM._Init.call(this.Dx={
T:this},0);B.te.Text._Init.call(this.De={T:this},0);B.Core.Timer._Init.call(this.
CX={T:this},0);this.__proto__=C.FH;var A;this.U(G6);this.C_.E8(15);this.CQ.U(I$);
this.CQ.Ce(0xFF000000);this.As.U(Ja);this.As.EO(0x11);this.As.Bd(true);this.Bo.U(
Jb);this.Bo.EO(0x9);this.Bo.Bd(true);this.BD.U(Jc);this.BE.U(Jd);this.BF.U(Je);this.
BG.U(Jf);this.BH.U(Jg);this.BM.GW(Jh);this.BM.GV(Ji);this.BM.GU(Jj);this.BM.GT(Jk
);this.AB.U(Jl);this.Aj.U(Jm);this.Aj.Cd(true);this.Eh.FO=6;this.Ei.FO=7;this.EL.
FO=131;this.DB.E8(15);this.DC.U(Jn);this.DC.GR(5);this.DC.GX(0xFFCCCCCC);this.Dg.
U(Jo);this.Dg.EO(0xC);this.Dg.E_(Jp);this.Dg.Ce(0xFFCCCCCC);this.Dw.U(Jq);this.Dw.
GR(2);this.Dw.GX(0xFFCCCCCC);this.Dd.U(Jr);this.Dd.EO(0xC);this.Dd.E_(Js);this.Dd.
Ce(0xFFCCCCCC);this.Dx.U(Jt);this.Dx.GR(2);this.Dx.GX(0xFFCCCCCC);this.De.U(Ju);
this.De.EO(0xC);this.De.E_(Jv);this.De.Ce(0xFFCCCCCC);this.CX.E8(10);this.CX.IV(
0);this.CX.B5(true);this.Ak(this.CQ,0);this.Ak(this.As,0);this.Ak(this.Bo,0);this.
Ak(this.BD,0);this.Ak(this.BE,0);this.Ak(this.BF,0);this.Ak(this.BG,0);this.Ak(this.
BH,0);this.Ak(this.BM,0);this.Ak(this.AB,0);this.Ak(this.Aj,0);this.Ak(this.DC,0
);this.Ak(this.Dg,0);this.Ak(this.Dw,0);this.Ak(this.Dd,0);this.Ak(this.Dx,0);this.
Ak(this.De,0);this.C_.EQ=[this,this.FE];this.As.Bp(B.r0(C.G0));this.Bo.Bp(B.r0(C.
Bo));this.BM.GN=[this,this.Io];this.BM.DU=[this,this.Ip];this.BM.Dz=[this,this.Io
];this.AB.Bp(B.r0(C.AB));this.Aj.Bp(B.r0(C.Aj));this.Data=B._GetAutoObject(C.Data
);this.Eh.DU=[this,this.Il];this.Eh.Dz=[this,this.JM];this.Ei.DU=[this,this.Il];
this.Ei.Dz=[this,this.JR];this.EL.Dz=[this,this.Ip];this.DB.EQ=[this,this.HB];this.
DC.GS([A=B._GetAutoObject(C.Data),A.HU,A.E9]);this.Dg.E7(B.r0(C.EZ));this.Dw.GS([
A=B._GetAutoObject(C.Data),A.HS,A.GP]);this.Dd.E7(B.r0(C.EZ));this.Dx.GS([A=B._GetAutoObject(
C.Data),A.HT,A.GQ]);this.De.E7(B.r0(C.EZ));this.CX.EQ=[this,this.IG];},_Done:function(
){this.__proto__=C.FH;this.C_._Done();this.AK._Done();this.CQ._Done();this.As._Done(
);this.Bo._Done();this.BD._Done();this.BE._Done();this.BF._Done();this.BG._Done(
);this.BH._Done();this.BM._Done();this.AB._Done();this.Aj._Done();this.Eh._Done(
);this.Ei._Done();this.EL._Done();this.DB._Done();this.DC._Done();this.Dg._Done(
);this.Dw._Done();this.Dd._Done();this.Dx._Done();this.De._Done();this.CX._Done(
);B.Core.Al._Done.call(this);},_ReInit:function(){B.Core.Al._ReInit.call(this);this.
C_._ReInit();this.AK._ReInit();this.CQ._ReInit();this.As._ReInit();this.Bo._ReInit(
);this.BD._ReInit();this.BE._ReInit();this.BF._ReInit();this.BG._ReInit();this.BH.
_ReInit();this.BM._ReInit();this.AB._ReInit();this.Aj._ReInit();this.Eh._ReInit(
);this.Ei._ReInit();this.EL._ReInit();this.DB._ReInit();this.DC._ReInit();this.Dg.
_ReInit();this.Dw._ReInit();this.Dd._ReInit();this.Dx._ReInit();this.De._ReInit(
);this.CX._ReInit();},_Mark:function(D){var A;B.Core.Al._Mark.call(this,D);if((A=
this.Data)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.C_)._cycle!=D)A._Mark(A.
_cycle=D);if((A=this.AK)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CQ)._cycle!=D)
A._Mark(A._cycle=D);if((A=this.As)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Bo).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.BD)._cycle!=D)A._Mark(A._cycle=D);if((A=
this.BE)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BF)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.BG)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BH)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.BM)._cycle!=D)A._Mark(A._cycle=D);if((A=this.AB)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.Aj)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Eh
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ei)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.EL)._cycle!=D)A._Mark(A._cycle=D);if((A=this.DB)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.DC)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Dg)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.Dw)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Dd)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.Dx)._cycle!=D)A._Mark(A._cycle=D);if((A=this.De
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CX)._cycle!=D)A._Mark(A._cycle=D);},_className:
"BrickGame::BrickGame"};C.Aj={_class:function(){return B.tf.Ae;},0:{FileName:"./res/BrickGamePaddle.png"
,Format:B.b9,NoOfFrames:1,FrameSize:[90,24],FrameDelay:0,_this:null}};C.AB={_class:
function(){return B.tf.Ae;},0:{FileName:"./res/BrickGameBall.png",Format:B.b9,NoOfFrames:
1,FrameSize:[24,24],FrameDelay:0,_this:null}};C.GE={ER:0,FS:0,EN:1,E9:function(E
){if(E<0)E=0;if(this.ER===E)return;this.ER=E;B.sx([this,this.HU,this.E9],0);},GQ:
function(E){if(this.FS===E)return;this.FS=E;B.sx([this,this.HT,this.GQ],0);},GP:
function(E){if(this.EN===E)return;this.EN=E;B.sx([this,this.HS,this.GP],0);},HU:
function(){return this.ER;},HT:function(){return this.FS;},HS:function(){return this.
EN;},_Init:function(aArg){B.th.FL._Init.call(this,aArg);this.__proto__=C.GE;},_className:
"BrickGame::DataClass"};C.Data={_Init:function(){C.GE._Init.call(this,0);},_variants:
function(){return this;},_this:null};C.KX={Kz:0,Kx:1,Blue:2,Green:3,Yellow:4,White:
5,Kf:6,Kn:7,K9:8,K_:9,J9:10,Lc:11,Ky:12,KI:13};C.Bo={_class:function(){return B.
tf.Ae;},0:{FileName:"./res/BrickGameLandscape.png",Format:B.b9,NoOfFrames:1,FrameSize:[
540,36],FrameDelay:0,_this:null}};C.G0={_class:function(){return B.tf.Ae;},0:{FileName:
"./res/BrickGameStars.png",Format:B.b9,NoOfFrames:1,FrameSize:[600,228],FrameDelay:
0,_this:null}};C.EM={CP:null,Ca:null,B0:null,B1:null,B2:null,B3:null,BL:null,Ej:
6,GC:0,I8:0xFFFFFFFF,FY:function(G_){var A;var Bj=0;var EG=this.GC;var D3=2;this.
Ca.L(EG%10);this.B0.L(((EG/10)|0)%10);this.B1.L(((EG/100)|0)%10);this.B2.L(((EG/
1000)|0)%10);this.B3.L(((EG/10000)|0)%10);this.BL.L(((EG/100000)|0)%10);if(EG<0){
this.Ca.L(10);this.B0.L(10);this.B1.L(10);this.B2.L(10);this.B3.L(10);this.BL.L(
10);}if(this.Ej>5){this.BL.Bd(true);this.BL.U(B.sV(this.BL.G,Bj));Bj=(Bj+((A=this.
BL.G)[2]-A[0]))-D3;}else this.BL.Bd(false);if(this.Ej>4){this.B3.Bd(true);this.B3.
U(B.sV(this.B3.G,Bj));Bj=(Bj+((A=this.B3.G)[2]-A[0]))-D3;}else this.B3.Bd(false);
if(this.Ej>3){this.B2.Bd(true);this.B2.U(B.sV(this.B2.G,Bj));Bj=(Bj+((A=this.B2.
G)[2]-A[0]))-D3;}else this.B2.Bd(false);if(this.Ej>2){this.B1.Bd(true);this.B1.U(
B.sV(this.B1.G,Bj));Bj=(Bj+((A=this.B1.G)[2]-A[0]))-D3;}else this.B1.Bd(false);if(
this.Ej>1){this.B0.Bd(true);this.B0.U(B.sV(this.B0.G,Bj));Bj=(Bj+((A=this.B0.G)[
2]-A[0]))-D3;}else this.BL.Bd(false);this.Ca.Bd(true);this.Ca.U(B.sV(this.Ca.G,Bj
));Bj=(Bj+((A=this.Ca.G)[2]-A[0]))-D3;},GR:function(E){if(this.Ej===E)return;if(
E<1)E=1;if(E>6)E=6;this.Ej=E;this.GI();},IW:function(E){if(this.GC===E)return;this.
GC=E;this.GI();},Ht:function(BX){var A;if(!!this.CP)this.IW((A=this.CP,A[1].call(
A[0])));},GS:function(E){if(B.sc(this.CP,E))return;if(!!this.CP)B.rC([this,this.
Ht],this.CP,0);this.CP=E;if(!!E)B.rn([this,this.Ht],E,0);if(!!E)B.kD([this,this.
Ht],this);},GX:function(E){this.I8=E;this.Ca.Ce(E);this.B0.Ce(E);this.B1.Ce(E);this.
B2.Ce(E);this.B3.Ce(E);this.BL.Ce(E);},_Init:function(aArg){B.Core.Al._Init.call(
this,aArg);B.te.Bc._Init.call(this.Ca={T:this},0);B.te.Bc._Init.call(this.B0={T:
this},0);B.te.Bc._Init.call(this.B1={T:this},0);B.te.Bc._Init.call(this.B2={T:this
},0);B.te.Bc._Init.call(this.B3={T:this},0);B.te.Bc._Init.call(this.BL={T:this},
0);this.__proto__=C.EM;this.U(Jw);this.Ca.U(Jx);this.Ca.Cd(true);this.B0.U(Jy);this.
B0.Cd(true);this.B0.L(0);this.B1.U(Jz);this.B1.Cd(true);this.B2.U(JA);this.B2.Cd(
true);this.B3.U(JB);this.B3.Cd(true);this.BL.U(JC);this.BL.Cd(true);this.Ak(this.
Ca,0);this.Ak(this.B0,0);this.Ak(this.B1,0);this.Ak(this.B2,0);this.Ak(this.B3,0
);this.Ak(this.BL,0);this.Ca.Bp(B.r0(C.DT));this.B0.Bp(B.r0(C.DT));this.B1.Bp(B.
r0(C.DT));this.B2.Bp(B.r0(C.DT));this.B3.Bp(B.r0(C.DT));this.BL.Bp(B.r0(C.DT));}
,_Done:function(){this.__proto__=C.EM;this.Ca._Done();this.B0._Done();this.B1._Done(
);this.B2._Done();this.B3._Done();this.BL._Done();B.Core.Al._Done.call(this);},_ReInit:
function(){B.Core.Al._ReInit.call(this);this.Ca._ReInit();this.B0._ReInit();this.
B1._ReInit();this.B2._ReInit();this.B3._ReInit();this.BL._ReInit();},_Mark:function(
D){var A;B.Core.Al._Mark.call(this,D);if((A=this.CP)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Ca)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B0)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.B1)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B2
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B3)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.BL)._cycle!=D)A._Mark(A._cycle=D);},_className:"BrickGame::LcdDisplay"};C.
DT={_class:function(){return B.tf.Ae;},0:{FileName:"./res/BrickGameLcdNumbers.png"
,Format:B.kC,NoOfFrames:11,FrameSize:[17,24],FrameDelay:0,_this:null}};
C._Init=function(){C.DO.__proto__=B.Core.Al;C.FH.__proto__=B.Core.Al;C.GE.__proto__=
B.th.FL;C.EM.__proto__=B.Core.Al;};C.BO=function(D){var A;if((A=C.EZ[0]._this)&&(
A._cycle!=D))A._Done(C.EZ[0]._this=null);if((A=C.C$[0]._this)&&(A._cycle!=D))A._Done(
C.C$[0]._this=null);if((A=C.Aj[0]._this)&&(A._cycle!=D))A._Done(C.Aj[0]._this=null
);if((A=C.AB[0]._this)&&(A._cycle!=D))A._Done(C.AB[0]._this=null);if((A=C.Data._this
)&&(A._cycle!=D))A._Done(C.Data._this=null);if((A=C.Bo[0]._this)&&(A._cycle!=D))
A._Done(C.Bo[0]._this=null);if((A=C.G0[0]._this)&&(A._cycle!=D))A._Done(C.G0[0].
_this=null);if((A=C.DT[0]._this)&&(A._cycle!=D))A._Done(C.DT[0]._this=null);};return C;
})();

/* Embedded Wizard */