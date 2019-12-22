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
);if(EmWiApp.th)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.th=(function(){var B=EmWiApp;var C={};
var AX=[216,69,696,341];var BM=[0,0,750,500];var Dx=[0,0,800,480];var Fi=[0,230,800
,340];var Fj="\uF25A";var Ga=[0,100,800,195];var Gb="Touch here...";
C.D9={DY:null,Bi:null,D9:null,CP:function(aArg){this.DY=B._NewObject(C.DY,0);this.
DY.L(AX);this.S(this.DY,0);},DriveCursorHitting:function(C$,R,BH){if(!!this.DY){
this.KS(this.DY);this.DY=null;}return B.Core.Root.DriveCursorHitting.call(this,C$
,R,BH);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.te.BS._Init.call(
this.Bi={J:this},0);B.ti.Ip._Init.call(this.D9={J:this},0);this.__proto__=C.D9;this.
L(BM);this.Bi.L(BM);this.D9.L(AX);this.S(this.Bi,0);this.S(this.D9,0);this.Bi.DD(
B.r0(C.Bi));this.CP(aArg);},_Done:function(){this.__proto__=C.D9;this.Bi._Done();
this.D9._Done();B.Core.Root._Done.call(this);},_ReInit:function(){B.Core.Root._ReInit.
call(this);this.Bi._ReInit();this.D9._ReInit();},_Mark:function(D){var A;B.Core.
Root._Mark.call(this,D);if((A=this.DY)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.Bi)._cycle!=D)A._Mark(A._cycle=D);if((A=this.D9)._cycle!=D)A._Mark(A._cycle=
D);},_className:"Simulation::Application"};C.Bi={_class:function(){return B.tg.Aq;
},0:{FileName:"./res/SimulationBackground.png",Format:B.b9,NoOfFrames:1,FrameSize:[
750,500],FrameDelay:0,_this:null}};C.DY={Bi:null,BQ:null,Dt:null,Text:null,Ah:null
,L6:0,Mz:function(E){this.BQ.Aw((this.BQ.H_()&0x00FFFFFF)|((E&0xFF)<<24));this.Dt.
A4(E);},Md:function(){return this.L6;},_Init:function(aArg){B.Core.O._Init.call(
this,aArg);B.te.U._Init.call(this.Bi={J:this},0);B.te.BQ._Init.call(this.BQ={J:this
},0);B.te.Text._Init.call(this.Dt={J:this},0);B.te.Text._Init.call(this.Text={J:
this},0);B.tf.Ah._Init.call(this.Ah={J:this},0);this.__proto__=C.DY;this.L(Dx);this.
Bi.AT(0x17);this.Bi.L(Dx);this.Bi.Aw(0xDE2D2D2D);this.BQ.AT(0x17);this.BQ.L(Dx);
this.BQ.KR(3);this.Dt.AT(0xD);this.Dt.L(Fi);this.Dt.A6(Fj);this.Text.AT(0xD);this.
Text.L(Ga);this.Text.A6(Gb);this.Ah.KW=true;this.Ah.Bv(24);this.Ah.Bu(750);this.
Ah.MA(750);this.Ah.Ak(true);this.Ah.Bn=0;this.Ah.AW=255;this.S(this.Bi,0);this.S(
this.BQ,0);this.S(this.Dt,0);this.S(this.Text,0);this.Dt.B0(B.r0(C.Dt));this.Text.
B0(B.r0(C.Bj));this.Ah.A0=[this,this.Md,this.Mz];},_Done:function(){this.__proto__=
C.DY;this.Bi._Done();this.BQ._Done();this.Dt._Done();this.Text._Done();this.Ah._Done(
);B.Core.O._Done.call(this);},_ReInit:function(){B.Core.O._ReInit.call(this);this.
Bi._ReInit();this.BQ._ReInit();this.Dt._ReInit();this.Text._ReInit();this.Ah._ReInit(
);},_Mark:function(D){var A;B.Core.O._Mark.call(this,D);if((A=this.Bi)._cycle!=D
)A._Mark(A._cycle=D);if((A=this.BQ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Dt).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Ah)._cycle!=D)A._Mark(A._cycle=D);},_className:"Simulation::Hint"};C.Dt={
_class:function(){return B.tg.Bj;},0:{Data:function(){return B.tk;},Cache:[],_this:
null}};C.Bj={_class:function(){return B.tg.Bj;},0:{Data:function(){return B.tj;}
,Cache:[],_this:null}};
C._Init=function(){C.D9.__proto__=B.Core.Root;C.DY.__proto__=B.Core.O;};C.Dh=function(
D){var A;if((A=C.Bi[0]._this)&&(A._cycle!=D))A._Done(C.Bi[0]._this=null);if((A=C.
Dt[0]._this)&&(A._cycle!=D))A._Done(C.Dt[0]._this=null);if((A=C.Bj[0]._this)&&(A.
_cycle!=D))A._Done(C.Bj[0]._this=null);};return C;})();

/* Embedded Wizard */