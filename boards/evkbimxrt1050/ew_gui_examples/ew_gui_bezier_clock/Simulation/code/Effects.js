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
uk=(function(){var B=EmWiApp;var E={};
var AJ=[100,100];var Be="Trying to remove a fader from a task, which is actually running";
var Dh="The fader doesn\'t belong to this task";var CK="Trying to add a fader to a task, which is actually running";
var Ft="The fader belongs already to a task";
E.GV={C9:null,timer:null,PV:null,GX:null,I5:null,LQ:0,Ib:0,E_:0,DU:0,LV:0.001000,
FF:0,Bf:0,Bm:0,J5:0.500000,K1:3,It:3,Ie:0.500000,Es:3.000000,OG:0,Kt:0,Ks:0,Fk:0
,Hq:1000,Ht:0,Ii:0,F0:0,B5:false,Kr:false,OT:false,JW:false,NU:false,EH:function(
Af){var A;if(!this.timer)return;if(!this.Ib){if(this.OT)this.Ib=-1;else this.Ib=
1;this.LQ=this.Ib;this.Bm=this.timer.Ah;this.Bf=0;this.FF=-1.000000;}var Ed=false;
if((this.Ib>0)&&(this.LQ>0))Ed=this.V5();else if((this.Ib<0)&&(this.LQ<0))Ed=this.
V8();else if(this.Ib>0)Ed=this.V6();else Ed=this.V7();if(Ed){this.BG(false);(A=this.
PV)?A[1].call(A[0],this):null;(A=this.GX)?A[1].call(A[0],this):null;}},NR:function(
L){var A;if(!this.C9&&(((this.F0===19)||(this.F0===20))||(this.F0===21))){var M_=
this.K1+1;var VB=Math.sqrt(this.J5);var JV=0.500000;var I;this.C9=B._NewObject(E.
M0,0);this.C9.DZ.Set(0,1.000000);this.C9.IC.Set(0,1.000000);for(I=1;I<M_;I=I+1){
this.C9.DZ.Set(I,this.C9.DZ.Get(I-1)*VB);this.C9.IC.Set(I,this.C9.IC.Get(I-1)*this.
J5);JV=JV+this.C9.DZ.Get(I);}var VG=1.000000/JV;this.C9.DZ.Set(0,0.500000);JV=0.000000;
for(I=0;I<M_;I=I+1){this.C9.DZ.Set(I,this.C9.DZ.Get(I)*VG);JV=JV+this.C9.DZ.Get(
I);}this.C9.DZ.Set(M_,this.C9.DZ.Get(M_)+(1.000000-JV));}if(this.Kr){if(L<0.500000
)L=L*2.000000;else L=2.000000-(L*2.000000);}switch(this.F0){case 1:{L=Math.pow(L
,this.Es);}break;case 2:{L=1.000000-L;L=1.000000-Math.pow(L,this.Es);}break;case
3:{L=L*2.000000;if(L<1.000000)L=Math.pow(L,this.Es)/2.000000;else{L=2.000000-L;L=(
2.000000-Math.pow(L,this.Es))*0.500000;}}break;case 4:{L=(Math.pow(2.718000,this.
Es*L)-1.000000)/(Math.pow(2.718000,this.Es)-1.000000);}break;case 5:{L=1.000000-
L;L=1.000000-((Math.pow(2.718000,this.Es*L)-1.000000)/(Math.pow(2.718000,this.Es
)-1.000000));}break;case 6:{L=L*2.000000;if(L<1.000000)L=((Math.pow(2.718000,this.
Es*L)-1.000000)/(Math.pow(2.718000,this.Es)-1.000000))/2.000000;else{L=2.000000-
L;L=(2.000000-((Math.pow(2.718000,this.Es*L)-1.000000)/(Math.pow(2.718000,this.Es
)-1.000000)))*0.500000;}}break;case 7:{L=1.000000-Math.cos((L*90.000000)*B.ii);}
break;case 8:{L=Math.sin((L*90.000000)*B.ii);}break;case 9:{L=L*2.000000;if(L<1.000000
){L=1.000000-Math.cos((L*90.000000)*B.ii);L=L*0.500000;}else{L=2.000000-L;L=1.000000-
Math.cos((L*90.000000)*B.ii);L=(2.000000-L)*0.500000;}}break;case 10:{L=1.000000-
Math.sqrt(1.000000-(L*L));}break;case 11:{L=1.000000-L;L=Math.sqrt(1.000000-(L*L
));}break;case 12:{L=L*2.000000;if(L<1.000000){L=1.000000-Math.sqrt(1.000000-(L*
L));L=L*0.500000;}else{L=2.000000-L;L=1.000000-Math.sqrt(1.000000-(L*L));L=(2.000000-
L)*0.500000;}}break;case 13:{L=((L*L)*L)-((L*this.Ie)*Math.sin((L*180.000000)*B.
ii));}break;case 14:{L=1.000000-L;L=((L*L)*L)-((L*this.Ie)*Math.sin((L*180.000000
)*B.ii));L=1.000000-L;}break;case 15:{L=L*2.000000;if(L<1.000000){L=((L*L)*L)-((
L*this.Ie)*Math.sin((L*180.000000)*B.ii));L=L*0.500000;}else{L=2.000000-L;L=((L*
L)*L)-((L*this.Ie)*Math.sin((L*180.000000)*B.ii));L=(2.000000-L)*0.500000;}}break;
case 16:{L=((L*L)*L)*Math.sin(((L*90.000000)*(1+(4*this.It)))*B.ii);}break;case 17:{
L=1.000000-L;L=1.000000-(((L*L)*L)*Math.sin(((L*90.000000)*(1+(4*this.It)))*B.ii
));}break;case 18:{L=L*2.000000;if(L<1.000000){L=((L*L)*L)*Math.sin(((L*90.000000
)*(1+(4*this.It)))*B.ii);L=L*0.500000;}else{L=2.000000-L;L=((L*L)*L)*Math.sin(((
L*90.000000)*(1+(4*this.It)))*B.ii);L=(2.000000-L)*0.500000;}}break;case 19:{var
GH=this.C9;var I=0;var GN=0.000000;var En=GH.DZ.Get(0);var D0=1.000000-L;while(D0>
En){I=I+1;GN=En;En=En+GH.DZ.Get(I);}L=(D0-GN)/(En-GN);if(!I)L=1.000000-(L*L);else{
L=(2.000000*L)-1.000000;L=GH.IC.Get(I)*(1.000000-(L*L));}}break;case 20:{var GH=
this.C9;var I=0;var GN=0.000000;var En=GH.DZ.Get(0);while(L>En){I=I+1;GN=En;En=En+
GH.DZ.Get(I);}L=(L-GN)/(En-GN);if(!I)L=L*L;else{L=(2.000000*L)-1.000000;L=1.000000-(
GH.IC.Get(I)*(1.000000-(L*L)));}}break;case 21:{var GH=this.C9;var I=0;var GN=0.000000;
var En=GH.DZ.Get(0);var A$=L*2.000000;var D0=A$-1.000000;if(A$<1.000000)D0=-D0;while(
D0>En){I=I+1;GN=En;En=En+GH.DZ.Get(I);}D0=(D0-GN)/(En-GN);if(!I)D0=D0*D0;else{D0=(
2.000000*D0)-1.000000;D0=1.000000-(GH.IC.Get(I)*(1.000000-(D0*D0)));}if(A$<1.000000
)L=0.500000*(1.000000-D0);else L=0.500000*(1.000000+D0);}break;default:{if(this.
JW){var A$=L;var L3=1.000000-A$;L=((L3*A$)*(this.DU+1.000000))+(A$*A$);}else if(
this.NU){var A$=L;var L3=1.000000-A$;var RA=A$*A$;var VJ=L3*L3;L=(((VJ*A$)*(this.
DU+1.000000))+((L3*RA)*(this.E_+2.000000)))+(RA*A$);}}}if(((this.OG>0.000000)&&(
L>0.000000))&&(L<1.000000))L=L+B.tO(-this.OG*0.100000,this.OG*0.100000);this.Mj(
L);(A=this.I5)?A[1].call(A[0],this):null;},V8:function(){var Au=(this.timer.Ah-this.
Bm)|0;if(Au<0)Au=-Au;var EQ=this.Ii;var FJ=this.Ii+this.Hq;var Cd=this.Ht+this.Hq;
var Ed=false;var BI=this.FF;if(!this.Bf&&(Au>=FJ)){this.Bf=1;Au=Au-FJ;this.Bm=this.
Bm+FJ;}if((this.Bf>0)&&(Au>=Cd)){var Cv=(Au/Cd)|0;this.Bf=this.Bf+Cv;Au=Au-(Cv*Cd
);this.Bm=this.Bm+(Cv*Cd);}if((this.Bf>2)&&!this.Fk)this.Bf=1;if(this.Bf>0)EQ=this.
Ht;if((this.Bf>=this.Fk)&&(this.Fk>0)){Ed=true;BI=0.000000;}else if(Au>=EQ)BI=1.000000-((
Au-EQ)*this.LV);else if(BI>=0.000000)BI=0.000000;if(BI!==this.FF){this.FF=BI;this.
NR(BI);}return Ed;},V7:function(){var Au=(this.Bm-this.timer.Ah)|0;var EQ=this.Ii;
var FJ=this.Ii+this.Hq;var Cd=this.Ht+this.Hq;var Ed=false;var BI=this.FF;if((this.
Bf>1)&&(Au<0)){var Cv=(((-Au+Cd)-1)/Cd)|0;if((this.Bf-Cv)<=0)Cv=this.Bf-1;this.Bf=
this.Bf-Cv;Au=Au+(Cv*Cd);this.Bm=this.Bm+(Cv*Cd);}if(((this.Bf===1)&&(Au<0))&&(this.
Fk>0)){this.Bf=0;Au=Au+FJ;this.Bm=this.Bm+FJ;}if(((this.Bf===1)&&(Au<0))&&!this.
Fk){var Cv=(((-Au+Cd)-1)/Cd)|0;Au=Au+(Cv*Cd);this.Bm=this.Bm+(Cv*Cd);}if(this.Bf>
0)EQ=this.Ht;if(Au<0){Ed=true;BI=1.000000;}else if(Au>=EQ)BI=1.000000-((Au-EQ)*this.
LV);else if(BI>=0.000000)BI=1.000000;if(BI!==this.FF){this.FF=BI;this.NR(BI);}return Ed;
},V6:function(){var Au=(this.Bm-this.timer.Ah)|0;var EQ=this.Ii;var FJ=this.Ii+this.
Hq;var Cd=this.Ht+this.Hq;var Ed=false;var BI=this.FF;if((this.Bf>1)&&(Au<0)){var
Cv=(((-Au+Cd)-1)/Cd)|0;if((this.Bf-Cv)<=0)Cv=this.Bf-1;this.Bf=this.Bf-Cv;Au=Au+(
Cv*Cd);this.Bm=this.Bm+(Cv*Cd);}if(((this.Bf===1)&&(Au<0))&&(this.Fk>0)){this.Bf=
0;Au=Au+FJ;this.Bm=this.Bm+FJ;}if(((this.Bf===1)&&(Au<0))&&!this.Fk){var Cv=(((-
Au+Cd)-1)/Cd)|0;Au=Au+(Cv*Cd);this.Bm=this.Bm+(Cv*Cd);}if(this.Bf>0)EQ=this.Ht;if(
Au<0){Ed=true;BI=0.000000;}else if(Au>=EQ)BI=(Au-EQ)*this.LV;else if(BI>=0.000000
)BI=0.000000;if(BI!==this.FF){this.FF=BI;this.NR(BI);}return Ed;},V5:function(){
var Au=(this.timer.Ah-this.Bm)|0;if(Au<0)Au=-Au;var EQ=this.Ii;var FJ=this.Ii+this.
Hq;var Cd=this.Ht+this.Hq;var Ed=false;var BI=this.FF;if(!this.Bf&&(Au>=FJ)){this.
Bf=1;Au=Au-FJ;this.Bm=this.Bm+FJ;}if((this.Bf>0)&&(Au>=Cd)){var Cv=(Au/Cd)|0;this.
Bf=this.Bf+Cv;Au=Au-(Cv*Cd);this.Bm=this.Bm+(Cv*Cd);}if((this.Bf>2)&&!this.Fk)this.
Bf=1;if(this.Bf>0)EQ=this.Ht;if((this.Bf>=this.Fk)&&(this.Fk>0)){Ed=true;BI=1.000000;
}else if(Au>=EQ)BI=(Au-EQ)*this.LV;else if(BI>=0.000000)BI=1.000000;if(BI!==this.
FF){this.FF=BI;this.NR(BI);}return Ed;},QY:function(C){if(this.OT===C)return;this.
OT=C;if(!this.B5||!this.Ib)return;if(C)this.LQ=-1;else this.LQ=1;this.Bm=(this.timer.
Lf()*2)-this.Bm;},QO:function(C){if(C<0.000000)C=0.000000;if(C>1.000000)C=1.000000;
this.J5=C;},QM:function(C){if(C<1)C=1;if(C>10)C=10;this.K1=C;this.C9=null;},QV:function(
C){if(C<1)C=1;if(C>10)C=10;this.It=C;},QL:function(C){if(C<0.000000)C=0.000000;if(
C>10.000000)C=10.000000;this.Ie=C;},QR:function(C){if(C<1.000000)C=1.000000;if(C>
100.000000)C=100.000000;this.Es=C;},Q0:function(C){if(this.Kt===C)return;this.Kt=
C;if(this.F0===26){this.E_=C;this.JW=(this.DU===this.E_)&&(this.DU!==0.000000);this.
NU=!this.JW&&(this.DU!==this.E_);}},QZ:function(C){if(this.Ks===C)return;this.Ks=
C;if(this.F0===26){this.DU=C;this.JW=(this.DU===this.E_)&&(this.DU!==0.000000);this.
NU=!this.JW&&(this.DU!==this.E_);}},Kk:function(C){if(this.F0===C)return;this.F0=
C;this.C9=null;switch(C){case 24:{this.DU=-1.100000;this.E_=1.100000;}break;case
22:{this.DU=-1.000000;this.E_=-2.000000;}break;case 23:{this.DU=2.000000;this.E_=
1.000000;}break;case 25:{this.DU=1.100000;this.E_=-1.100000;}break;case 0:{this.
DU=0.000000;this.E_=0.000000;}break;default:{this.DU=this.Ks;this.E_=this.Kt;}}this.
JW=(this.DU===this.E_)&&(this.DU!==0.000000);this.NU=!this.JW&&(this.DU!==this.E_
);},Lt:function(C){if(C<0)C=0;this.Fk=C;},FU:function(C){if(C<15)C=15;this.Hq=C;
this.LV=1.000000/C;},TP:function(C){if(C<0)C=0;this.Ht=C;},BG:function(C){if(this.
B5===C)return;this.B5=C;if(!C&&!!this.timer){B.sM([this,this.EH],this.timer,0);this.
timer=null;}if(C){this.timer=B._GetAutoObject(E.J4);B.sz([this,this.EH],this.timer
,0);this.Ib=0;B.lq([this,this.EH],this);}},Mj:function(Jn){},OY:function(Af){if(
this.B5)this.BG(false);this.BG(true);},_Init:function(aArg){this.__proto__=E.GV;
B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:
function(D){var A;if((A=this.C9)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.timer
)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.PV)&&((A=A[0])._cycle!=D))A._Mark(
A._cycle=D);if((A=this.GX)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.
I5)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(
A._cycle=D);},M:null,_cycle:0,_observers:null,_className:"Effects::Effect"};E.BE={
Av:null,Df:0,BZ:255,BN:0,Mj:function(Jn){var A;this.Df=this.BN+(Math.round((this.
BZ-this.BN)*Jn)|0);if(!!this.Av)(A=this.Av,A[2].call(A[0],this.Df));},_Init:function(
aArg){E.GV._Init.call(this,aArg);this.__proto__=E.BE;},_Mark:function(D){var A;E.
GV._Mark.call(this,D);if((A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);}
,_className:"Effects::Int32Effect"};E.MQ={Av:null,Df:B.qx,BZ:AJ,BN:B.qx,Mj:function(
Jn){var A;var JY=this.BN[0];var Gg=this.BN[1];JY=JY+(Math.round((this.BZ[0]-JY)*
Jn)|0);Gg=Gg+(Math.round((this.BZ[1]-Gg)*Jn)|0);this.Df=[JY,Gg];if(!!this.Av)(A=
this.Av,A[2].call(A[0],this.Df));},_Init:function(aArg){E.GV._Init.call(this,aArg
);this.__proto__=E.MQ;},_Mark:function(D){var A;E.GV._Mark.call(this,D);if((A=this.
Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::PointEffect"
};E.DK={Av:null,Df:0,BZ:1.000000,BN:0,Mj:function(Jn){var A;this.Df=this.BN+((this.
BZ-this.BN)*Jn);if(!!this.Av)(A=this.Av,A[2].call(A[0],this.Df));},_Init:function(
aArg){E.GV._Init.call(this,aArg);this.__proto__=E.DK;},_Mark:function(D){var A;E.
GV._Mark.call(this,D);if((A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);}
,_className:"Effects::FloatEffect"};E.Hr={Fc:null,Ag:null,Ad:null,G:null,J:null,
JJ:null,PM:null,Lp:null,BT:true,Ku:true,Mi:false,Mk:true,Km:true,I_:true,Gi:function(
){if(!!this.Fc)this.Fc.Gi();},On:function(){return true;},Et:function(){},Dr:function(
){this.Gi();},_Init:function(aArg){this.__proto__=E.Hr;B.gv++;},_Done:function(){
this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=
this.Fc)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Ag)&&(A._cycle!=D))A._Mark(
A._cycle=D);if((A=this.Ad)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.G)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.J)&&(A._cycle!=D))A._Mark(A._cycle=D);
if((A=this.JJ)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.PM)&&((A=A[0
])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Lp)&&((A=A[0])._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:
null,_className:"Effects::Fader"};E.Kv={OQ:B.qx,Jb:true,I4:false,IF:false,On:function(
){return this.IF;},Et:function(){if((this.BT&&!this.Jb)&&!this.Ku)this.J.P(B.tX(
this.J.H,this.OQ));if(this.BT&&!this.Jb){this.J.GZ(255);this.J.Hx(true);}if((!this.
BT&&!this.I4)&&!!this.J.G)this.J.Hx(false);if((!this.BT&&this.I_)&&!!this.J.G)this.
J.G.G3(this.J);},Dr:function(){if((this.BT||this.Mi)&&!this.J.G){this.J.Hx(false
);this.G.Z(this.J,0);}if(this.BT&&this.Km)this.J.G.HF(this.J);if(this.BT&&this.Mk
)this.J.G.D_(this.J);if((this.BT&&this.Jb)&&!this.Ku)this.J.P(B.tX(this.J.H,this.
OQ));if(this.BT&&this.Jb){this.J.GZ(255);this.J.Hx(true);}if((!this.BT&&this.I4)&&
!!this.J.G)this.J.Hx(false);if(!this.BT&&(this.G.Db===this.J))this.G.D_(null);this.
IF=true;this.Gi();},_Init:function(aArg){E.Hr._Init.call(this,aArg);this.__proto__=
E.Kv;},_className:"Effects::VisibilityFader"};E.Kl={Bo:null,AI:null,Cn:false,IF:
false,Sf:false,On:function(){return this.IF;},Et:function(){if(this.BT){this.J.GZ(
this.Bo.BZ);this.J.P(B.tX(this.J.H,this.AI.BZ));}if(!this.BT&&!this.J.Dd)this.J.
Hx(false);if((!this.BT&&this.I_)&&!!this.J.G)this.J.G.G3(this.J);if(this.Cn)this.
J.Lr(this.Sf);},Dr:function(){var A;if(this.Ku){if(this.J.Kc()&&!!this.J.G)this.
Bo.BN=this.J.Dd;else this.Bo.BN=0;this.AI.BN=this.J.H.slice(0,2);}var VH=((!this.
J.G||!this.J.Dd)||!this.J.Kc())||(((A=B.il(this.J.H,[0,0,(A=this.G.H)[2]-A[0],A[
3]-A[1]]))[0]>=A[2])||(A[1]>=A[3]));var VI=((!this.BT&&((!this.J.G||!this.J.Kc()
)||this.I_))||!this.Bo.BZ)||(((A=B.il(B.tz([0,0,(A=this.J.H)[2]-A[0],A[3]-A[1]],
this.AI.BZ),[0,0,(A=this.G.H)[2]-A[0],A[3]-A[1]]))[0]>=A[2])||(A[1]>=A[3]));if(VH&&
VI){this.Bo.BN=0;this.Bo.BZ=0;this.AI.BN=this.AI.BZ;}this.Sf=this.J.Tp();if((this.
BT||this.Mi)&&!this.J.G){this.J.Hx(false);this.G.Z(this.J,0);}if(this.BT&&this.Km
)this.J.G.HF(this.J);if(this.BT&&this.Mk)this.J.G.D_(this.J);if(!this.BT&&(this.
G.Db===this.J))this.G.D_(null);if(this.BT&&!this.J.Kc()){this.J.P(B.tX(this.J.H,
this.AI.BN));this.J.GZ(this.Bo.BN);this.J.Hx(true);}if(!this.BT&&((!this.J.G||!this.
J.Kc())||!this.J.Dd)){this.IF=true;this.Gi();return;}if(!this.BT&&B.tl(this.AI.BN
,this.AI.BZ))this.J.P(B.tX(this.J.H,this.AI.BN));if(!this.BT&&(this.Bo.BN===this.
Bo.BZ))this.J.GZ(this.Bo.BN);if((this.J.Dd===this.Bo.BZ)&&B.tl(this.J.H.slice(0,
2),this.AI.BZ)){this.IF=true;this.Gi();return;}if((this.Bo.BN===this.Bo.BZ)&&B.tl(
this.AI.BN,this.AI.BZ)){this.IF=true;this.Gi();return;}if(!this.Bo.Fk)this.Bo.Lt(
1);if(!this.AI.Fk)this.AI.Lt(1);if(this.Cn)this.J.Lr(true);this.AI.Av=null;this.
AI.QY(false);this.AI.Kr=false;this.AI.GX=[this,this.RM];this.AI.I5=[this,this.ND
];this.Bo.Av=[A=this.J,A.Tu,A.Vk];this.Bo.QY(false);this.Bo.Kr=false;this.Bo.GX=[
this,this.RM];this.Bo.I5=null;this.AI.BG(!B.tl(this.AI.BN,this.AI.BZ));this.Bo.BG(
this.Bo.BN!==this.Bo.BZ);},RM:function(Af){this.IF=!this.Bo.B5&&!this.AI.B5;this.
Gi();},ND:function(Af){this.J.P(B.tX(this.J.H,this.AI.Df));},_Init:function(aArg
){E.Hr._Init.call(this,aArg);E.BE._Init.call(this.Bo={M:this},0);E.MQ._Init.call(
this.AI={M:this},0);this.__proto__=E.Kl;},_Done:function(){this.__proto__=E.Hr;this.
Bo._Done();this.AI._Done();E.Hr._Done.call(this);},_ReInit:function(){E.Hr._ReInit.
call(this);this.Bo._ReInit();this.AI._ReInit();},_Mark:function(D){var A;E.Hr._Mark.
call(this,D);if((A=this.Bo)._cycle!=D)A._Mark(A._cycle=D);if((A=this.AI)._cycle!=
D)A._Mark(A._cycle=D);},_className:"Effects::PositionFader"};E.Iw={Hp:function(){
return null;},J0:function(){return null;},K3:function(){return this.Hp();},K2:function(
){return this.J0();},_Init:function(aArg){this.__proto__=E.Iw;B.gv++;},_Done:function(
){this.__proto__=null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((
A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:
"Effects::Transition"};E.OV={OA:0,OC:0,OD:0,OB:0,Fd:0x12,Jb:true,I4:false,Hp:function(
){var Q=B._NewObject(E.Kv,0);Q.BT=true;Q.Mk=false;Q.Km=true;Q.Ku=false;Q.Jb=this.
Jb;Q.Lp=[this,this.PQ];return Q;},J0:function(){var Q=B._NewObject(E.Kv,0);Q.BT=
false;Q.I4=this.I4;Q.I_=true;return Q;},K3:function(){var Q=E.Iw.K3.call(this);Q.
Km=false;return Q;},K2:function(){var Q=B._NewObject(E.Kv,0);Q.BT=false;Q.Mi=true;
Q.I_=false;Q.I4=this.I4;return Q;},PQ:function(Af){var A;var Q=(E.Kv.isPrototypeOf(
Af)?Af:null);var AH=this.Fd;var U=[0,0,(A=Q.G.H)[2]-A[0],A[3]-A[1]];var size=[(A=
Q.J.H)[2]-A[0],A[3]-A[1]];U=[].concat(U[0]+this.OB,U.slice(1,4));U=B.t1(U,U[2]-this.
OC);U=B.t3(U,U[1]+this.OD);U=[].concat(U.slice(0,3),U[3]-this.OA);var S=U.slice(
0,2);if(((AH&0x4)===0x4))S=[U[2]-size[0],S[1]];else if(((AH&0x2)===0x2))S=[(U[0]+(((
U[2]-U[0])/2)|0))-((size[0]/2)|0),S[1]];if(((AH&0x20)===0x20))S=[S[0],U[3]-size[
1]];else if(((AH&0x10)===0x10))S=[S[0],(U[1]+(((U[3]-U[1])/2)|0))-((size[1]/2)|0
)];Q.OQ=S;},_Init:function(aArg){E.Iw._Init.call(this,aArg);this.__proto__=E.OV;
},_className:"Effects::ShowHideTransition"};E.Kq={OA:0,OC:0,OD:0,OB:0,Fd:0x12,J5:
0.500000,K1:3,It:3,Ie:0.500000,Es:3.000000,Kt:0,Ks:0,Mp:500,I2:0,F0:23,Cn:false,
Hp:function(){var Q=B._NewObject(E.Kl,0);Q.BT=true;Q.Mk=false;Q.Km=true;Q.Ku=true;
Q.Cn=this.Cn;Q.Lp=[this,this.PQ];Q.Bo.FU(this.Mp);Q.Bo.BN=0;Q.Bo.BZ=255;Q.AI.FU(
this.Mp);Q.AI.Kk(this.F0);Q.AI.QZ(this.Ks);Q.AI.Q0(this.Kt);Q.AI.QR(this.Es);Q.AI.
QL(this.Ie);Q.AI.QV(this.It);Q.AI.QM(this.K1);Q.AI.QO(this.J5);return Q;},J0:function(
){var Q=B._NewObject(E.Kl,0);Q.BT=false;Q.I_=true;Q.Ku=true;Q.Cn=this.Cn;Q.Lp=[this
,this.VR];Q.Bo.BN=255;Q.Bo.BZ=0;Q.Bo.FU(this.Mp);Q.AI.FU(this.Mp);Q.AI.Kk(this.F0
);Q.AI.QZ(this.Ks);Q.AI.Q0(this.Kt);Q.AI.QR(this.Es);Q.AI.QL(this.Ie);Q.AI.QV(this.
It);Q.AI.QM(this.K1);Q.AI.QO(this.J5);return Q;},K3:function(){var Q=E.Iw.K3.call(
this);Q.Km=false;return Q;},K2:function(){var Q=E.Iw.K2.call(this);Q.Mi=true;Q.I_=
false;return Q;},PQ:function(Af){var A;var Q=(E.Kl.isPrototypeOf(Af)?Af:null);var
AH=this.Fd;var U=[0,0,(A=Q.G.H)[2]-A[0],A[3]-A[1]];var size=[(A=Q.J.H)[2]-A[0],A[
3]-A[1]];U=[].concat(U[0]+this.OB,U.slice(1,4));U=B.t1(U,U[2]-this.OC);U=B.t3(U,
U[1]+this.OD);U=[].concat(U.slice(0,3),U[3]-this.OA);var S=U.slice(0,2);if(((AH&
0x4)===0x4))S=[U[2]-size[0],S[1]];else if(((AH&0x2)===0x2))S=[(U[0]+(((U[2]-U[0]
)/2)|0))-((size[0]/2)|0),S[1]];if(((AH&0x20)===0x20))S=[S[0],U[3]-size[1]];else if(((
AH&0x10)===0x10))S=[S[0],(U[1]+(((U[3]-U[1])/2)|0))-((size[1]/2)|0)];Q.AI.BZ=S;if(((
!Q.J.G||!Q.J.Kc())||!Q.J.Dd)||(((A=B.il(Q.J.H,[0,0,(A=Q.G.H)[2]-A[0],A[3]-A[1]])
)[0]>=A[2])||(A[1]>=A[3]))){var CM=[(A=Q.G.H)[2]-A[0],A[3]-A[1]];switch(this.I2){
case 5:S=[S[0],-size[1]];break;case 3:{S=[S[0],-size[1]];S=[CM[0],S[1]];}break;case
8:S=[-size[0],-size[1]];break;case 2:S=[CM[0],S[1]];break;case 7:S=[-size[0],S[1
]];break;case 4:S=[S[0],CM[1]];break;case 1:{S=[S[0],CM[1]];S=[CM[0],S[1]];}break;
case 6:{S=[S[0],CM[1]];S=[-size[0],S[1]];}break;default:;}Q.J.P(B.tX(Q.J.H,S));}
},VR:function(Af){var A;var Q=(E.Kl.isPrototypeOf(Af)?Af:null);var S=Q.J.H.slice(
0,2);var size=[(A=Q.J.H)[2]-A[0],A[3]-A[1]];var CM=[(A=Q.G.H)[2]-A[0],A[3]-A[1]];
switch(this.I2){case 5:S=[S[0],CM[1]];break;case 3:{S=[S[0],CM[1]];S=[-size[0],S[
1]];}break;case 8:{S=[S[0],CM[1]];S=[CM[0],S[1]];}break;case 2:S=[-size[0],S[1]];
break;case 7:S=[CM[0],S[1]];break;case 4:S=[S[0],-size[1]];break;case 1:S=[-size[
0],-size[1]];break;case 6:{S=[S[0],-size[1]];S=[CM[0],S[1]];}break;default:;}Q.AI.
BZ=S;},_Init:function(aArg){E.Iw._Init.call(this,aArg);this.__proto__=E.Kq;},_className:
"Effects::SlideTransition"};E.F0={W7:0,Xk:1,Xm:2,Xl:3,WR:4,WT:5,WS:6,XB:7,XD:8,XC:
9,WE:10,WG:11,WF:12,Ws:13,Wu:14,Wt:15,WM:16,WO:17,WN:18,WB:19,WD:20,WC:21,WL:22,
WU:23,WK:24,WV:25,WH:26};E.WJ={P8:0x1,P7:0x2,P9:0x4,Qa:0x8,P$:0x10,P_:0x20};E.N0={
Trigger:function(){B.Core.Timer.Trigger.call(this);B.qw(this,0);},_Init:function(
aArg){B.Core.Timer._Init.call(this,aArg);this.__proto__=E.N0;},_className:"Effects::EffectTimerClass"
};E.J4={_Init:function(){E.N0._Init.call(this,0);this.Lu(15);this.BG(true);},_variants:
function(){return this;},_this:null};E.M0={DZ:B.tA(12,0,null),IC:B.tA(12,0,null)
,_Init:function(aArg){(this.DZ=[]).__proto__=E.M0.DZ;(this.IC=[]).__proto__=E.M0.
IC;this.__proto__=E.M0;B.gv++;},_Done:function(){this.__proto__=null;B.gv--;},_ReInit:
function(){},_Mark:function(D){var A;if((A=this.M)&&(A._cycle!=D))A._Mark(A._cycle=
D);},M:null,_cycle:0,_observers:null,_className:"Effects::TimingList"};E.Mq={AB:
null,Aw:null,QG:function(LI){var Q=this.Aw;while(!!Q){Q.Et();Q.J.F=Q.J.F&~0x40000;
Q=Q.Ad;}Q=this.Aw;while(!!Q){B.lq(Q.PM,Q);Q=Q.Ad;}},QF:function(LI){var Q=this.Aw;
while(!!Q){B.lq(Q.JJ,Q);Q=Q.Ad;}this.Aw=null;this.AB=null;},Dr:function(LI){var A;
if(!this.Aw)this.Gi();var Q=this.Aw;while(!!Q){Q.J.F=(Q.J.F|0x40000)&~0x20000;Q.
J.JR=null;Q=Q.Ad;}Q=this.Aw;while(!!Q){(A=Q.Lp)?A[1].call(A[0],Q):null;Q.Dr();Q=
Q.Ad;}},Gi:function(){var Q=this.Aw;while(!!Q&&Q.On())Q=Q.Ad;if(!Q)B.Core.LC.Gi.
call(this);},T3:function(Bc){if(!Bc)return;if(this.Om())throw new Error(Be);if(Bc.
Fc!==this)throw new Error(Dh);if(!!Bc.Ad)Bc.Ad.Ag=Bc.Ag;else this.AB=Bc.Ag;if(!!
Bc.Ag)Bc.Ag.Ad=Bc.Ad;else this.Aw=Bc.Ad;Bc.Fc=null;Bc.Ad=null;Bc.Ag=null;if(!!Bc.
J)Bc.J.JR=null;B.lq(Bc.JJ,Bc);if(!this.Aw)this.Qe();},P6:function(Bc){if(!Bc)return;
if(this.Om())throw new Error(CK);if(!!Bc.Fc)throw new Error(Ft);Bc.Ag=this.AB;Bc.
Ad=null;if(!!this.AB)this.AB.Ad=Bc;else this.Aw=Bc;this.AB=Bc;Bc.Fc=this;},_Init:
function(aArg){B.Core.LC._Init.call(this,aArg);this.__proto__=E.Mq;},_Mark:function(
D){var A;B.Core.LC._Mark.call(this,D);if((A=this.AB)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Aw)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Effects::FaderTask"
};E.MV={_Init:function(){E.OV._Init.call(this,0);},_variants:function(){return this;
},_this:null};E.Ko={_Init:function(){E.Kq._Init.call(this,0);this.I2=2;},_variants:
function(){return this;},_this:null};E.Kp={_Init:function(){E.Kq._Init.call(this
,0);this.I2=7;},_variants:function(){return this;},_this:null};E.G5={_Init:function(
){E.Kq._Init.call(this,0);this.I2=4;},_variants:function(){return this;},_this:null
};E.Gr={_Init:function(){E.Kq._Init.call(this,0);this.I2=5;},_variants:function(
){return this;},_this:null};
E._Init=function(){E.BE.__proto__=E.GV;E.MQ.__proto__=E.GV;E.DK.__proto__=E.GV;E.
Kv.__proto__=E.Hr;E.Kl.__proto__=E.Hr;E.OV.__proto__=E.Iw;E.Kq.__proto__=E.Iw;E.
N0.__proto__=B.Core.Timer;E.Mq.__proto__=B.Core.LC;};E.DC=function(D){var A;if((
A=E.J4._this)&&(A._cycle!=D))A._Done(E.J4._this=null);if((A=E.MV._this)&&(A._cycle
!=D))A._Done(E.MV._this=null);if((A=E.Ko._this)&&(A._cycle!=D))A._Done(E.Ko._this=
null);if((A=E.Kp._this)&&(A._cycle!=D))A._Done(E.Kp._this=null);if((A=E.G5._this
)&&(A._cycle!=D))A._Done(E.G5._this=null);if((A=E.Gr._this)&&(A._cycle!=D))A._Done(
E.Gr._this=null);};return E;})();

/* Embedded Wizard */