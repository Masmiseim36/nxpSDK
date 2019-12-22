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
tg=(function(){var B=EmWiApp;var E={};
var AJ=[100,100];var Be="Trying to remove a fader from a task, which is actually running";
var Db="The fader doesn\'t belong to this task";var CD="Trying to add a fader to a task, which is actually running";
var Fk="The fader belongs already to a task";
E.Gg={C6:null,timer:null,PD:null,GO:null,IY:null,Ly:0,H3:0,E3:0,DP:0,LE:0.001000,
Fy:0,Bc:0,Bk:0,JV:0.500000,KI:3,Ij:3,H6:0.500000,Ek:3.000000,On:0,Kg:0,Kf:0,Fc:0
,Hj:1000,Hm:0,H_:0,FV:0,B0:false,Kd:false,OA:false,JM:false,NB:false,Ey:function(
Ae){var A;if(!this.timer)return;if(!this.H3){if(this.OA)this.H3=-1;else this.H3=
1;this.Ly=this.H3;this.Bk=this.timer.Af;this.Bc=0;this.Fy=-1.000000;}var D9=false;
if((this.H3>0)&&(this.Ly>0))D9=this.VG();else if((this.H3<0)&&(this.Ly<0))D9=this.
VJ();else if(this.H3>0)D9=this.VH();else D9=this.VI();if(D9){this.Bs(false);(A=this.
PD)?A[1].call(A[0],this):null;(A=this.GO)?A[1].call(A[0],this):null;}},Ny:function(
M){var A;if(!this.C6&&(((this.FV===19)||(this.FV===20))||(this.FV===21))){var MR=
this.KI+1;var Ve=Math.sqrt(this.JV);var JL=0.500000;var I;this.C6=B._NewObject(E.
Ll,0);this.C6.DS.Set(0,1.000000);this.C6.Iv.Set(0,1.000000);for(I=1;I<MR;I=I+1){
this.C6.DS.Set(I,this.C6.DS.Get(I-1)*Ve);this.C6.Iv.Set(I,this.C6.Iv.Get(I-1)*this.
JV);JL=JL+this.C6.DS.Get(I);}var Vj=1.000000/JL;this.C6.DS.Set(0,0.500000);JL=0.000000;
for(I=0;I<MR;I=I+1){this.C6.DS.Set(I,this.C6.DS.Get(I)*Vj);JL=JL+this.C6.DS.Get(
I);}this.C6.DS.Set(MR,this.C6.DS.Get(MR)+(1.000000-JL));}if(this.Kd){if(M<0.500000
)M=M*2.000000;else M=2.000000-(M*2.000000);}switch(this.FV){case 1:{M=Math.pow(M
,this.Ek);}break;case 2:{M=1.000000-M;M=1.000000-Math.pow(M,this.Ek);}break;case
3:{M=M*2.000000;if(M<1.000000)M=Math.pow(M,this.Ek)/2.000000;else{M=2.000000-M;M=(
2.000000-Math.pow(M,this.Ek))*0.500000;}}break;case 4:{M=(Math.pow(2.718000,this.
Ek*M)-1.000000)/(Math.pow(2.718000,this.Ek)-1.000000);}break;case 5:{M=1.000000-
M;M=1.000000-((Math.pow(2.718000,this.Ek*M)-1.000000)/(Math.pow(2.718000,this.Ek
)-1.000000));}break;case 6:{M=M*2.000000;if(M<1.000000)M=((Math.pow(2.718000,this.
Ek*M)-1.000000)/(Math.pow(2.718000,this.Ek)-1.000000))/2.000000;else{M=2.000000-
M;M=(2.000000-((Math.pow(2.718000,this.Ek*M)-1.000000)/(Math.pow(2.718000,this.Ek
)-1.000000)))*0.500000;}}break;case 7:{M=1.000000-Math.cos((M*90.000000)*B.ks);}
break;case 8:{M=Math.sin((M*90.000000)*B.ks);}break;case 9:{M=M*2.000000;if(M<1.000000
){M=1.000000-Math.cos((M*90.000000)*B.ks);M=M*0.500000;}else{M=2.000000-M;M=1.000000-
Math.cos((M*90.000000)*B.ks);M=(2.000000-M)*0.500000;}}break;case 10:{M=1.000000-
Math.sqrt(1.000000-(M*M));}break;case 11:{M=1.000000-M;M=Math.sqrt(1.000000-(M*M
));}break;case 12:{M=M*2.000000;if(M<1.000000){M=1.000000-Math.sqrt(1.000000-(M*
M));M=M*0.500000;}else{M=2.000000-M;M=1.000000-Math.sqrt(1.000000-(M*M));M=(2.000000-
M)*0.500000;}}break;case 13:{M=((M*M)*M)-((M*this.H6)*Math.sin((M*180.000000)*B.
ks));}break;case 14:{M=1.000000-M;M=((M*M)*M)-((M*this.H6)*Math.sin((M*180.000000
)*B.ks));M=1.000000-M;}break;case 15:{M=M*2.000000;if(M<1.000000){M=((M*M)*M)-((
M*this.H6)*Math.sin((M*180.000000)*B.ks));M=M*0.500000;}else{M=2.000000-M;M=((M*
M)*M)-((M*this.H6)*Math.sin((M*180.000000)*B.ks));M=(2.000000-M)*0.500000;}}break;
case 16:{M=((M*M)*M)*Math.sin(((M*90.000000)*(1+(4*this.Ij)))*B.ks);}break;case 17:{
M=1.000000-M;M=1.000000-(((M*M)*M)*Math.sin(((M*90.000000)*(1+(4*this.Ij)))*B.ks
));}break;case 18:{M=M*2.000000;if(M<1.000000){M=((M*M)*M)*Math.sin(((M*90.000000
)*(1+(4*this.Ij)))*B.ks);M=M*0.500000;}else{M=2.000000-M;M=((M*M)*M)*Math.sin(((
M*90.000000)*(1+(4*this.Ij)))*B.ks);M=(2.000000-M)*0.500000;}}break;case 19:{var
GD=this.C6;var I=0;var GI=0.000000;var Ef=GD.DS.Get(0);var DT=1.000000-M;while(DT>
Ef){I=I+1;GI=Ef;Ef=Ef+GD.DS.Get(I);}M=(DT-GI)/(Ef-GI);if(!I)M=1.000000-(M*M);else{
M=(2.000000*M)-1.000000;M=GD.Iv.Get(I)*(1.000000-(M*M));}}break;case 20:{var GD=
this.C6;var I=0;var GI=0.000000;var Ef=GD.DS.Get(0);while(M>Ef){I=I+1;GI=Ef;Ef=Ef+
GD.DS.Get(I);}M=(M-GI)/(Ef-GI);if(!I)M=M*M;else{M=(2.000000*M)-1.000000;M=1.000000-(
GD.Iv.Get(I)*(1.000000-(M*M)));}}break;case 21:{var GD=this.C6;var I=0;var GI=0.000000;
var Ef=GD.DS.Get(0);var A9=M*2.000000;var DT=A9-1.000000;if(A9<1.000000)DT=-DT;while(
DT>Ef){I=I+1;GI=Ef;Ef=Ef+GD.DS.Get(I);}DT=(DT-GI)/(Ef-GI);if(!I)DT=DT*DT;else{DT=(
2.000000*DT)-1.000000;DT=1.000000-(GD.Iv.Get(I)*(1.000000-(DT*DT)));}if(A9<1.000000
)M=0.500000*(1.000000-DT);else M=0.500000*(1.000000+DT);}break;default:{if(this.
JM){var A9=M;var LM=1.000000-A9;M=((LM*A9)*(this.DP+1.000000))+(A9*A9);}else if(
this.NB){var A9=M;var LM=1.000000-A9;var Rh=A9*A9;var Vm=LM*LM;M=(((Vm*A9)*(this.
DP+1.000000))+((LM*Rh)*(this.E3+2.000000)))+(Rh*A9);}}}if(((this.On>0.000000)&&(
M>0.000000))&&(M<1.000000))M=M+B.sI(-this.On*0.100000,this.On*0.100000);this.L3(
M);(A=this.IY)?A[1].call(A[0],this):null;},VJ:function(){var Au=(this.timer.Af-this.
Bk)|0;if(Au<0)Au=-Au;var EH=this.H_;var FC=this.H_+this.Hj;var B_=this.Hm+this.Hj;
var D9=false;var BE=this.Fy;if(!this.Bc&&(Au>=FC)){this.Bc=1;Au=Au-FC;this.Bk=this.
Bk+FC;}if((this.Bc>0)&&(Au>=B_)){var Co=(Au/B_)|0;this.Bc=this.Bc+Co;Au=Au-(Co*B_
);this.Bk=this.Bk+(Co*B_);}if((this.Bc>2)&&!this.Fc)this.Bc=1;if(this.Bc>0)EH=this.
Hm;if((this.Bc>=this.Fc)&&(this.Fc>0)){D9=true;BE=0.000000;}else if(Au>=EH)BE=1.000000-((
Au-EH)*this.LE);else if(BE>=0.000000)BE=0.000000;if(BE!==this.Fy){this.Fy=BE;this.
Ny(BE);}return D9;},VI:function(){var Au=(this.Bk-this.timer.Af)|0;var EH=this.H_;
var FC=this.H_+this.Hj;var B_=this.Hm+this.Hj;var D9=false;var BE=this.Fy;if((this.
Bc>1)&&(Au<0)){var Co=(((-Au+B_)-1)/B_)|0;if((this.Bc-Co)<=0)Co=this.Bc-1;this.Bc=
this.Bc-Co;Au=Au+(Co*B_);this.Bk=this.Bk+(Co*B_);}if(((this.Bc===1)&&(Au<0))&&(this.
Fc>0)){this.Bc=0;Au=Au+FC;this.Bk=this.Bk+FC;}if(((this.Bc===1)&&(Au<0))&&!this.
Fc){var Co=(((-Au+B_)-1)/B_)|0;Au=Au+(Co*B_);this.Bk=this.Bk+(Co*B_);}if(this.Bc>
0)EH=this.Hm;if(Au<0){D9=true;BE=1.000000;}else if(Au>=EH)BE=1.000000-((Au-EH)*this.
LE);else if(BE>=0.000000)BE=1.000000;if(BE!==this.Fy){this.Fy=BE;this.Ny(BE);}return D9;
},VH:function(){var Au=(this.Bk-this.timer.Af)|0;var EH=this.H_;var FC=this.H_+this.
Hj;var B_=this.Hm+this.Hj;var D9=false;var BE=this.Fy;if((this.Bc>1)&&(Au<0)){var
Co=(((-Au+B_)-1)/B_)|0;if((this.Bc-Co)<=0)Co=this.Bc-1;this.Bc=this.Bc-Co;Au=Au+(
Co*B_);this.Bk=this.Bk+(Co*B_);}if(((this.Bc===1)&&(Au<0))&&(this.Fc>0)){this.Bc=
0;Au=Au+FC;this.Bk=this.Bk+FC;}if(((this.Bc===1)&&(Au<0))&&!this.Fc){var Co=(((-
Au+B_)-1)/B_)|0;Au=Au+(Co*B_);this.Bk=this.Bk+(Co*B_);}if(this.Bc>0)EH=this.Hm;if(
Au<0){D9=true;BE=0.000000;}else if(Au>=EH)BE=(Au-EH)*this.LE;else if(BE>=0.000000
)BE=0.000000;if(BE!==this.Fy){this.Fy=BE;this.Ny(BE);}return D9;},VG:function(){
var Au=(this.timer.Af-this.Bk)|0;if(Au<0)Au=-Au;var EH=this.H_;var FC=this.H_+this.
Hj;var B_=this.Hm+this.Hj;var D9=false;var BE=this.Fy;if(!this.Bc&&(Au>=FC)){this.
Bc=1;Au=Au-FC;this.Bk=this.Bk+FC;}if((this.Bc>0)&&(Au>=B_)){var Co=(Au/B_)|0;this.
Bc=this.Bc+Co;Au=Au-(Co*B_);this.Bk=this.Bk+(Co*B_);}if((this.Bc>2)&&!this.Fc)this.
Bc=1;if(this.Bc>0)EH=this.Hm;if((this.Bc>=this.Fc)&&(this.Fc>0)){D9=true;BE=1.000000;
}else if(Au>=EH)BE=(Au-EH)*this.LE;else if(BE>=0.000000)BE=1.000000;if(BE!==this.
Fy){this.Fy=BE;this.Ny(BE);}return D9;},QH:function(C){if(this.OA===C)return;this.
OA=C;if(!this.B0||!this.H3)return;if(C)this.Ly=-1;else this.Ly=1;this.Bk=(this.timer.
KX()*2)-this.Bk;},Qx:function(C){if(C<0.000000)C=0.000000;if(C>1.000000)C=1.000000;
this.JV=C;},Qv:function(C){if(C<1)C=1;if(C>10)C=10;this.KI=C;this.C6=null;},QE:function(
C){if(C<1)C=1;if(C>10)C=10;this.Ij=C;},Qu:function(C){if(C<0.000000)C=0.000000;if(
C>10.000000)C=10.000000;this.H6=C;},QA:function(C){if(C<1.000000)C=1.000000;if(C>
100.000000)C=100.000000;this.Ek=C;},QJ:function(C){if(this.Kg===C)return;this.Kg=
C;if(this.FV===26){this.E3=C;this.JM=(this.DP===this.E3)&&(this.DP!==0.000000);this.
NB=!this.JM&&(this.DP!==this.E3);}},QI:function(C){if(this.Kf===C)return;this.Kf=
C;if(this.FV===26){this.DP=C;this.JM=(this.DP===this.E3)&&(this.DP!==0.000000);this.
NB=!this.JM&&(this.DP!==this.E3);}},J9:function(C){if(this.FV===C)return;this.FV=
C;this.C6=null;switch(C){case 24:{this.DP=-1.100000;this.E3=1.100000;}break;case
22:{this.DP=-1.000000;this.E3=-2.000000;}break;case 23:{this.DP=2.000000;this.E3=
1.000000;}break;case 25:{this.DP=1.100000;this.E3=-1.100000;}break;case 0:{this.
DP=0.000000;this.E3=0.000000;}break;default:{this.DP=this.Kf;this.E3=this.Kg;}}this.
JM=(this.DP===this.E3)&&(this.DP!==0.000000);this.NB=!this.JM&&(this.DP!==this.E3
);},K$:function(C){if(C<0)C=0;this.Fc=C;},FO:function(C){if(C<15)C=15;this.Hj=C;
this.LE=1.000000/C;},Tw:function(C){if(C<0)C=0;this.Hm=C;},Bs:function(C){if(this.
B0===C)return;this.B0=C;if(!C&&!!this.timer){B.rA([this,this.Ey],this.timer,0);this.
timer=null;}if(C){this.timer=B._GetAutoObject(E.JU);B.rl([this,this.Ey],this.timer
,0);this.H3=0;B.kD([this,this.Ey],this);}},L3:function(Jd){},OF:function(Ae){if(
this.B0)this.Bs(false);this.Bs(true);},_Init:function(aArg){this.__proto__=E.Gg;
B.gb++;},_Done:function(){this.__proto__=E.Gg;B.gb--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.C6)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.timer
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.PD)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.GO)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
IY)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.N)&&(A._cycle!=D))A._Mark(
A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"Effects::Effect"};E.BB={
Av:null,C$:0,BU:255,BJ:0,L3:function(Jd){var A;this.C$=this.BJ+(Math.round((this.
BU-this.BJ)*Jd)|0);if(!!this.Av)(A=this.Av,A[2].call(A[0],this.C$));},_Init:function(
aArg){E.Gg._Init.call(this,aArg);this.__proto__=E.BB;},_Mark:function(D){var A;E.
Gg._Mark.call(this,D);if((A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);}
,_className:"Effects::Int32Effect"};E.MA={Av:null,C$:B.pm,BU:AJ,BJ:B.pm,L3:function(
Jd){var A;var JN=this.BJ[0];var F$=this.BJ[1];JN=JN+(Math.round((this.BU[0]-JN)*
Jd)|0);F$=F$+(Math.round((this.BU[1]-F$)*Jd)|0);this.C$=[JN,F$];if(!!this.Av)(A=
this.Av,A[2].call(A[0],this.C$));},_Init:function(aArg){E.Gg._Init.call(this,aArg
);this.__proto__=E.MA;},_Mark:function(D){var A;E.Gg._Mark.call(this,D);if((A=this.
Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::PointEffect"
};E.DG={Av:null,C$:0,BU:1.000000,BJ:0,L3:function(Jd){var A;this.C$=this.BJ+((this.
BU-this.BJ)*Jd);if(!!this.Av)(A=this.Av,A[2].call(A[0],this.C$));},_Init:function(
aArg){E.Gg._Init.call(this,aArg);this.__proto__=E.DG;},_Mark:function(D){var A;E.
Gg._Mark.call(this,D);if((A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);}
,_className:"Effects::FloatEffect"};E.Hk={E6:null,X:null,U:null,G:null,K:null,Jz:
null,Pu:null,K7:null,BO:true,Kh:true,L2:false,L4:true,J$:true,I4:true,Gd:function(
){if(!!this.E6)this.E6.Gd();},N4:function(){return true;},Em:function(){},Dm:function(
){this.Gd();},_Init:function(aArg){this.__proto__=E.Hk;B.gb++;},_Done:function(){
this.__proto__=E.Hk;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((A=
this.E6)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.X)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.U)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.G)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.K)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Jz)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Pu)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.K7)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:
null,_className:"Effects::Fader"};E.Ki={Ox:B.pm,I7:true,IV:false,Iz:false,N4:function(
){return this.Iz;},Em:function(){if((this.BO&&!this.I7)&&!this.Kh)this.K.Q(B.sS(
this.K.H,this.Ox));if(this.BO&&!this.I7){this.K.GQ(255);this.K.Hq(true);}if((!this.
BO&&!this.IV)&&!!this.K.G)this.K.Hq(false);if((!this.BO&&this.I4)&&!!this.K.G)this.
K.G.GV(this.K);},Dm:function(){if((this.BO||this.L2)&&!this.K.G){this.K.Hq(false
);this.G.Aa(this.K,0);}if(this.BO&&this.J$)this.K.G.Gl(this.K);if(this.BO&&this.
L4)this.K.G.D4(this.K);if((this.BO&&this.I7)&&!this.Kh)this.K.Q(B.sS(this.K.H,this.
Ox));if(this.BO&&this.I7){this.K.GQ(255);this.K.Hq(true);}if((!this.BO&&this.IV)&&
!!this.K.G)this.K.Hq(false);if(!this.BO&&(this.G.C8===this.K))this.G.D4(null);this.
Iz=true;this.Gd();},_Init:function(aArg){E.Hk._Init.call(this,aArg);this.__proto__=
E.Ki;},_className:"Effects::VisibilityFader"};E.I3={Bm:null,AI:null,Ch:false,Iz:
false,RY:false,N4:function(){return this.Iz;},Em:function(){if(this.BO){this.K.GQ(
this.Bm.BU);this.K.Q(B.sS(this.K.H,this.AI.BU));}if(!this.BO&&!this.K.C_)this.K.
Hq(false);if((!this.BO&&this.I4)&&!!this.K.G)this.K.G.GV(this.K);if(this.Ch)this.
K.K9(this.RY);},Dm:function(){var A;if(this.Kh){if(this.K.J1()&&!!this.K.G)this.
Bm.BJ=this.K.C_;else this.Bm.BJ=0;this.AI.BJ=this.K.H.slice(0,2);}var Vk=((!this.
K.G||!this.K.C_)||!this.K.J1())||(((A=B.hV(this.K.H,[0,0,(A=this.G.H)[2]-A[0],A[
3]-A[1]]))[0]>=A[2])||(A[1]>=A[3]));var Vl=((!this.BO&&((!this.K.G||!this.K.J1()
)||this.I4))||!this.Bm.BU)||(((A=B.hV(B.sq([0,0,(A=this.K.H)[2]-A[0],A[3]-A[1]],
this.AI.BU),[0,0,(A=this.G.H)[2]-A[0],A[3]-A[1]]))[0]>=A[2])||(A[1]>=A[3]));if(Vk&&
Vl){this.Bm.BJ=0;this.Bm.BU=0;this.AI.BJ=this.AI.BU;}this.RY=this.K.S8();if((this.
BO||this.L2)&&!this.K.G){this.K.Hq(false);this.G.Aa(this.K,0);}if(this.BO&&this.
J$)this.K.G.Gl(this.K);if(this.BO&&this.L4)this.K.G.D4(this.K);if(!this.BO&&(this.
G.C8===this.K))this.G.D4(null);if(this.BO&&!this.K.J1()){this.K.Q(B.sS(this.K.H,
this.AI.BJ));this.K.GQ(this.Bm.BJ);this.K.Hq(true);}if(!this.BO&&((!this.K.G||!this.
K.J1())||!this.K.C_)){this.Iz=true;this.Gd();return;}if(!this.BO&&B.sa(this.AI.BJ
,this.AI.BU))this.K.Q(B.sS(this.K.H,this.AI.BJ));if(!this.BO&&(this.Bm.BJ===this.
Bm.BU))this.K.GQ(this.Bm.BJ);if((this.K.C_===this.Bm.BU)&&B.sa(this.K.H.slice(0,
2),this.AI.BU)){this.Iz=true;this.Gd();return;}if((this.Bm.BJ===this.Bm.BU)&&B.sa(
this.AI.BJ,this.AI.BU)){this.Iz=true;this.Gd();return;}if(!this.Bm.Fc)this.Bm.K$(
1);if(!this.AI.Fc)this.AI.K$(1);if(this.Ch)this.K.K9(true);this.AI.Av=null;this.
AI.QH(false);this.AI.Kd=false;this.AI.GO=[this,this.Rt];this.AI.IY=[this,this.Nl
];this.Bm.Av=[A=this.K,A.Tb,A.UZ];this.Bm.QH(false);this.Bm.Kd=false;this.Bm.GO=[
this,this.Rt];this.Bm.IY=null;this.AI.Bs(!B.sa(this.AI.BJ,this.AI.BU));this.Bm.Bs(
this.Bm.BJ!==this.Bm.BU);},Rt:function(Ae){this.Iz=!this.Bm.B0&&!this.AI.B0;this.
Gd();},Nl:function(Ae){this.K.Q(B.sS(this.K.H,this.AI.C$));},_Init:function(aArg
){E.Hk._Init.call(this,aArg);E.BB._Init.call(this.Bm={N:this},0);E.MA._Init.call(
this.AI={N:this},0);this.__proto__=E.I3;},_Done:function(){this.__proto__=E.I3;this.
Bm._Done();this.AI._Done();E.Hk._Done.call(this);},_ReInit:function(){E.Hk._ReInit.
call(this);this.Bm._ReInit();this.AI._ReInit();},_Mark:function(D){var A;E.Hk._Mark.
call(this,D);if((A=this.Bm)._cycle!=D)A._Mark(A._cycle=D);if((A=this.AI)._cycle!=
D)A._Mark(A._cycle=D);},_className:"Effects::PositionFader"};E.HA={Hi:function(){
return null;},JQ:function(){return null;},KK:function(){return this.Hi();},KJ:function(
){return this.JQ();},_Init:function(aArg){this.__proto__=E.HA;B.gb++;},_Done:function(
){this.__proto__=E.HA;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:
"Effects::Transition"};E.OC={Og:0,Oi:0,Oj:0,Oh:0,E7:0x12,I7:true,IV:false,Hi:function(
){var R=B._NewObject(E.Ki,0);R.BO=true;R.L4=false;R.J$=true;R.Kh=false;R.I7=this.
I7;R.K7=[this,this.Py];return R;},JQ:function(){var R=B._NewObject(E.Ki,0);R.BO=
false;R.IV=this.IV;R.I4=true;return R;},KK:function(){var R=E.HA.KK.call(this);R.
J$=false;return R;},KJ:function(){var R=B._NewObject(E.Ki,0);R.BO=false;R.L2=true;
R.I4=false;R.IV=this.IV;return R;},Py:function(Ae){var A;var R=(E.Ki.isPrototypeOf(
Ae)?Ae:null);var AG=this.E7;var W=[0,0,(A=R.G.H)[2]-A[0],A[3]-A[1]];var size=[(A=
R.K.H)[2]-A[0],A[3]-A[1]];W=[].concat(W[0]+this.Oh,W.slice(1,4));W=B.sW(W,W[2]-this.
Oi);W=B.sY(W,W[1]+this.Oj);W=[].concat(W.slice(0,3),W[3]-this.Og);var T=W.slice(
0,2);if(((AG&0x4)===0x4))T=[W[2]-size[0],T[1]];else if(((AG&0x2)===0x2))T=[(W[0]+(((
W[2]-W[0])/2)|0))-((size[0]/2)|0),T[1]];if(((AG&0x20)===0x20))T=[T[0],W[3]-size[
1]];else if(((AG&0x10)===0x10))T=[T[0],(W[1]+(((W[3]-W[1])/2)|0))-((size[1]/2)|0
)];R.Ox=T;},_Init:function(aArg){E.HA._Init.call(this,aArg);this.__proto__=E.OC;
},_className:"Effects::ShowHideTransition"};E.Kc={Og:0,Oi:0,Oj:0,Oh:0,E7:0x12,JV:
0.500000,KI:3,Ij:3,H6:0.500000,Ek:3.000000,Kg:0,Kf:0,L$:500,IU:0,FV:23,Ch:false,
Hi:function(){var R=B._NewObject(E.I3,0);R.BO=true;R.L4=false;R.J$=true;R.Kh=true;
R.Ch=this.Ch;R.K7=[this,this.Py];R.Bm.FO(this.L$);R.Bm.BJ=0;R.Bm.BU=255;R.AI.FO(
this.L$);R.AI.J9(this.FV);R.AI.QI(this.Kf);R.AI.QJ(this.Kg);R.AI.QA(this.Ek);R.AI.
Qu(this.H6);R.AI.QE(this.Ij);R.AI.Qv(this.KI);R.AI.Qx(this.JV);return R;},JQ:function(
){var R=B._NewObject(E.I3,0);R.BO=false;R.I4=true;R.Kh=true;R.Ch=this.Ch;R.K7=[this
,this.Vs];R.Bm.BJ=255;R.Bm.BU=0;R.Bm.FO(this.L$);R.AI.FO(this.L$);R.AI.J9(this.FV
);R.AI.QI(this.Kf);R.AI.QJ(this.Kg);R.AI.QA(this.Ek);R.AI.Qu(this.H6);R.AI.QE(this.
Ij);R.AI.Qv(this.KI);R.AI.Qx(this.JV);return R;},KK:function(){var R=E.HA.KK.call(
this);R.J$=false;return R;},KJ:function(){var R=E.HA.KJ.call(this);R.L2=true;R.I4=
false;return R;},Py:function(Ae){var A;var R=(E.I3.isPrototypeOf(Ae)?Ae:null);var
AG=this.E7;var W=[0,0,(A=R.G.H)[2]-A[0],A[3]-A[1]];var size=[(A=R.K.H)[2]-A[0],A[
3]-A[1]];W=[].concat(W[0]+this.Oh,W.slice(1,4));W=B.sW(W,W[2]-this.Oi);W=B.sY(W,
W[1]+this.Oj);W=[].concat(W.slice(0,3),W[3]-this.Og);var T=W.slice(0,2);if(((AG&
0x4)===0x4))T=[W[2]-size[0],T[1]];else if(((AG&0x2)===0x2))T=[(W[0]+(((W[2]-W[0]
)/2)|0))-((size[0]/2)|0),T[1]];if(((AG&0x20)===0x20))T=[T[0],W[3]-size[1]];else if(((
AG&0x10)===0x10))T=[T[0],(W[1]+(((W[3]-W[1])/2)|0))-((size[1]/2)|0)];R.AI.BU=T;if(((
!R.K.G||!R.K.J1())||!R.K.C_)||(((A=B.hV(R.K.H,[0,0,(A=R.G.H)[2]-A[0],A[3]-A[1]])
)[0]>=A[2])||(A[1]>=A[3]))){var CF=[(A=R.G.H)[2]-A[0],A[3]-A[1]];switch(this.IU){
case 5:T=[T[0],-size[1]];break;case 3:{T=[T[0],-size[1]];T=[CF[0],T[1]];}break;case
8:T=[-size[0],-size[1]];break;case 2:T=[CF[0],T[1]];break;case 7:T=[-size[0],T[1
]];break;case 4:T=[T[0],CF[1]];break;case 1:{T=[T[0],CF[1]];T=[CF[0],T[1]];}break;
case 6:{T=[T[0],CF[1]];T=[-size[0],T[1]];}break;default:;}R.K.Q(B.sS(R.K.H,T));}
},Vs:function(Ae){var A;var R=(E.I3.isPrototypeOf(Ae)?Ae:null);var T=R.K.H.slice(
0,2);var size=[(A=R.K.H)[2]-A[0],A[3]-A[1]];var CF=[(A=R.G.H)[2]-A[0],A[3]-A[1]];
switch(this.IU){case 5:T=[T[0],CF[1]];break;case 3:{T=[T[0],CF[1]];T=[-size[0],T[
1]];}break;case 8:{T=[T[0],CF[1]];T=[CF[0],T[1]];}break;case 2:T=[-size[0],T[1]];
break;case 7:T=[CF[0],T[1]];break;case 4:T=[T[0],-size[1]];break;case 1:T=[-size[
0],-size[1]];break;case 6:{T=[T[0],-size[1]];T=[CF[0],T[1]];}break;default:;}R.AI.
BU=T;},_Init:function(aArg){E.HA._Init.call(this,aArg);this.__proto__=E.Kc;},_className:
"Effects::SlideTransition"};E.FV={WD:0,WS:1,WU:2,WT:3,Wq:4,Ws:5,Wr:6,W7:7,W9:8,W8:
9,Wd:10,Wf:11,We:12,V5:13,V7:14,V6:15,Wl:16,Wn:17,Wm:18,Wa:19,Wc:20,Wb:21,Wk:22,
Wt:23,Wj:24,Wu:25,Wg:26};E.Wi={PR:0x1,PQ:0x2,PS:0x4,PV:0x8,PU:0x10,PT:0x20};E.NH={
Trigger:function(){B.Core.Timer.Trigger.call(this);B.pl(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=E.NH;},_className:"Effects::EffectTimerClass"
};E.JU={_Init:function(){E.NH._Init.call(this,0);this.La(15);this.Bs(true);},_variants:
function(){return this;},_this:null};E.Ll={DS:B.sr(12,0,null),Iv:B.sr(12,0,null)
,_Init:function(aArg){(this.DS=[]).__proto__=E.Ll.DS;(this.Iv=[]).__proto__=E.Ll.
Iv;this.__proto__=E.Ll;B.gb++;},_Done:function(){this.__proto__=E.Ll;B.gb--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.N)&&(A._cycle!=D))A._Mark(A._cycle=
D);},N:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};E.Ma={Az:
null,Am:null,Qp:function(Lr){var R=this.Am;while(!!R){R.Em();R.K.F=R.K.F&~0x40000;
R=R.U;}R=this.Am;while(!!R){B.kD(R.Pu,R);R=R.U;}},Qo:function(Lr){var R=this.Am;
while(!!R){B.kD(R.Jz,R);R=R.U;}this.Am=null;this.Az=null;},Dm:function(Lr){var A;
if(!this.Am)this.Gd();var R=this.Am;while(!!R){R.K.F=(R.K.F|0x40000)&~0x20000;R.
K.JH=null;R=R.U;}R=this.Am;while(!!R){(A=R.K7)?A[1].call(A[0],R):null;R.Dm();R=R.
U;}},Gd:function(){var R=this.Am;while(!!R&&R.N4())R=R.U;if(!R)B.Core.Ke.Gd.call(
this);},TJ:function(Ba){if(!Ba)return;if(this.N3())throw new Error(Be);if(Ba.E6!==
this)throw new Error(Db);if(!!Ba.U)Ba.U.X=Ba.X;else this.Az=Ba.X;if(!!Ba.X)Ba.X.
U=Ba.U;else this.Am=Ba.U;Ba.E6=null;Ba.U=null;Ba.X=null;if(!!Ba.K)Ba.K.JH=null;B.
kD(Ba.Jz,Ba);if(!this.Am)this.PZ();},PP:function(Ba){if(!Ba)return;if(this.N3())
throw new Error(CD);if(!!Ba.E6)throw new Error(Fk);Ba.X=this.Az;Ba.U=null;if(!!this.
Az)this.Az.U=Ba;else this.Am=Ba;this.Az=Ba;Ba.E6=this;},_Init:function(aArg){B.Core.
Ke._Init.call(this,aArg);this.__proto__=E.Ma;},_Mark:function(D){var A;B.Core.Ke.
_Mark.call(this,D);if((A=this.Az)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
Am)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::FaderTask"};E.MF={
_Init:function(){E.OC._Init.call(this,0);},_variants:function(){return this;},_this:
null};E.Ka={_Init:function(){E.Kc._Init.call(this,0);this.IU=2;},_variants:function(
){return this;},_this:null};E.Kb={_Init:function(){E.Kc._Init.call(this,0);this.
IU=7;},_variants:function(){return this;},_this:null};E.GX={_Init:function(){E.Kc.
_Init.call(this,0);this.IU=4;},_variants:function(){return this;},_this:null};E.
Gm={_Init:function(){E.Kc._Init.call(this,0);this.IU=5;},_variants:function(){return this;
},_this:null};
E._Init=function(){E.BB.__proto__=E.Gg;E.MA.__proto__=E.Gg;E.DG.__proto__=E.Gg;E.
Ki.__proto__=E.Hk;E.I3.__proto__=E.Hk;E.OC.__proto__=E.HA;E.Kc.__proto__=E.HA;E.
NH.__proto__=B.Core.Timer;E.Ma.__proto__=B.Core.Ke;};E.Dy=function(D){var A;if((
A=E.JU._this)&&(A._cycle!=D))A._Done(E.JU._this=null);if((A=E.MF._this)&&(A._cycle
!=D))A._Done(E.MF._this=null);if((A=E.Ka._this)&&(A._cycle!=D))A._Done(E.Ka._this=
null);if((A=E.Kb._this)&&(A._cycle!=D))A._Done(E.Kb._this=null);if((A=E.GX._this
)&&(A._cycle!=D))A._Done(E.GX._this=null);if((A=E.Gm._this)&&(A._cycle!=D))A._Done(
E.Gm._this=null);};return E;})();

/* Embedded Wizard */