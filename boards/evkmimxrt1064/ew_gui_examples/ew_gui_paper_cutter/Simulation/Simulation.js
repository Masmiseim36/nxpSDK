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
);if(EmWiApp.ti)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.ti=(function(){var B=EmWiApp;var C={};
var AO=[216,69,696,341];var BF=[0,0,750,500];var Dh=[0,0,800,480];var Fh=[0,230,800
,340];var Dw="\uF25A";var FH=[0,100,800,195];var FI="Touch here...";
C.DY={DL:null,At:null,DY:null,Bb:function(aArg){this.DL=B._NewObject(C.DL,0);this.
DL.H(AO);this.N(this.DL,0);},DriveCursorHitting:function(CP,S,By){if(!!this.DL){
this.GP(this.DL);this.DL=null;}return B.Core.Root.DriveCursorHitting.call(this,CP
,S,By);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.te.Aq._Init.call(
this.At={J:this},0);B.th.Hr._Init.call(this.DY={J:this},0);this.__proto__=C.DY;this.
H(BF);this.At.H(BF);this.DY.H(AO);this.N(this.At,0);this.N(this.DY,0);this.At.CW(
B.r0(C.At));this.Bb(aArg);},_Done:function(){this.__proto__=C.DY;this.At._Done();
this.DY._Done();B.Core.Root._Done.call(this);},_ReInit:function(){B.Core.Root._ReInit.
call(this);this.At._ReInit();this.DY._ReInit();},_Mark:function(D){var A;B.Core.
Root._Mark.call(this,D);if((A=this.DL)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.At)._cycle!=D)A._Mark(A._cycle=D);if((A=this.DY)._cycle!=D)A._Mark(A._cycle=
D);},_className:"Simulation::Application"};C.At={_class:function(){return B.tf.An;
},0:{FileName:"./res/SimulationBackground.png",Format:B.b9,NoOfFrames:1,FrameSize:[
750,500],FrameDelay:0,_this:null}};C.DL={At:null,Ad:null,Ao:null,Text:null,Ck:null
,K6:0,Lj:function(E){this.Ad.R((this.Ad.Ld()&0x00FFFFFF)|((E&0xFF)<<24));this.Ao.
Ar(E);},Lf:function(){return this.K6;},_Init:function(aArg){B.Core.P._Init.call(
this,aArg);B.te.Av._Init.call(this.At={J:this},0);B.te.Ad._Init.call(this.Ad={J:
this},0);B.te.Text._Init.call(this.Ao={J:this},0);B.te.Text._Init.call(this.Text={
J:this},0);B.tg.Ck._Init.call(this.Ck={J:this},0);this.__proto__=C.DL;this.H(Dh);
this.At.T(0x17);this.At.H(Dh);this.At.R(0xDE2D2D2D);this.Ad.T(0x17);this.Ad.H(Dh
);this.Ad.JN(3);this.Ao.T(0xD);this.Ao.H(Fh);this.Ao.Z(Dw);this.Text.T(0xD);this.
Text.H(FH);this.Text.Z(FI);this.Ck.JR=true;this.Ck.HH(24);this.Ck.HE(750);this.Ck.
Ll(750);this.Ck.A8(true);this.Ck.Ev=0;this.Ck.EV=255;this.N(this.At,0);this.N(this.
Ad,0);this.N(this.Ao,0);this.N(this.Text,0);this.Ao.Aw(B.r0(C.Ao));this.Text.Aw(
B.r0(C.A3));this.Ck.AK=[this,this.Lf,this.Lj];},_Done:function(){this.__proto__=
C.DL;this.At._Done();this.Ad._Done();this.Ao._Done();this.Text._Done();this.Ck._Done(
);B.Core.P._Done.call(this);},_ReInit:function(){B.Core.P._ReInit.call(this);this.
At._ReInit();this.Ad._ReInit();this.Ao._ReInit();this.Text._ReInit();this.Ck._ReInit(
);},_Mark:function(D){var A;B.Core.P._Mark.call(this,D);if((A=this.At)._cycle!=D
)A._Mark(A._cycle=D);if((A=this.Ad)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ao).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Ck)._cycle!=D)A._Mark(A._cycle=D);},_className:"Simulation::Hint"};C.Ao={
_class:function(){return B.tf.A3;},0:{Data:function(){return B.tr;},Cache:[],_this:
null}};C.A3={_class:function(){return B.tf.A3;},0:{Data:function(){return B.tq;}
,Cache:[],_this:null}};
C._Init=function(){C.DY.__proto__=B.Core.Root;C.DL.__proto__=B.Core.P;};C.CY=function(
D){var A;if((A=C.At[0]._this)&&(A._cycle!=D))A._Done(C.At[0]._this=null);if((A=C.
Ao[0]._this)&&(A._cycle!=D))A._Done(C.Ao[0]._this=null);if((A=C.A3[0]._this)&&(A.
_cycle!=D))A._Done(C.A3[0]._this=null);};return C;})();

/* Embedded Wizard */