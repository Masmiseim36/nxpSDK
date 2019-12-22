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
var At=[216,69,696,341];var AT=[0,0,750,500];var Cq=[0,0,800,480];var DW=[0,230,800
,340];var DX="\uF25A";var Eo=[0,100,800,195];var Ep="Touch here...";
C.CW={CO:null,As:null,CW:null,Cc:function(aArg){this.CO=B._NewObject(C.CO,0);this.
CO.U(At);this.Ak(this.CO,0);},DriveCursorHitting:function(B6,N,AO){if(!!this.CO){
this.I4(this.CO);this.CO=null;}return B.Core.Root.DriveCursorHitting.call(this,B6
,N,AO);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.te.Bc._Init.call(
this.As={T:this},0);B.BrickGame.FH._Init.call(this.CW={T:this},0);this.__proto__=
C.CW;this.U(AT);this.As.U(AT);this.CW.U(At);this.Ak(this.As,0);this.Ak(this.CW,0
);this.As.Bp(B.r0(C.As));this.Cc(aArg);},_Done:function(){this.__proto__=C.CW;this.
As._Done();this.CW._Done();B.Core.Root._Done.call(this);},_ReInit:function(){B.Core.
Root._ReInit.call(this);this.As._ReInit();this.CW._ReInit();},_Mark:function(D){
var A;B.Core.Root._Mark.call(this,D);if((A=this.CO)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.As)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CW)._cycle!=D)A._Mark(
A._cycle=D);},_className:"Simulation::Application"};C.As={_class:function(){return B.
tf.Ae;},0:{FileName:"./res/SimulationBackground.png",Format:B.b9,NoOfFrames:1,FrameSize:[
750,500],FrameDelay:0,_this:null}};C.CO={As:null,Cn:null,Cp:null,Text:null,B4:null
,IM:0,IY:function(E){this.Cn.Ce((this.Cn.IS()&0x00FFFFFF)|((E&0xFF)<<24));this.Cp.
I0(E);},IT:function(){return this.IM;},_Init:function(aArg){B.Core.Al._Init.call(
this,aArg);B.te.CQ._Init.call(this.As={T:this},0);B.te.Cn._Init.call(this.Cn={T:
this},0);B.te.Text._Init.call(this.Cp={T:this},0);B.te.Text._Init.call(this.Text={
T:this},0);B.tg.B4._Init.call(this.B4={T:this},0);this.__proto__=C.CO;this.U(Cq);
this.As.FU(0x17);this.As.U(Cq);this.As.Ce(0xDE2D2D2D);this.Cn.FU(0x17);this.Cn.U(
Cq);this.Cn.I2(3);this.Cp.FU(0xD);this.Cp.U(DW);this.Cp.E_(DX);this.Text.FU(0xD);
this.Text.U(Eo);this.Text.E_(Ep);this.B4.H1=true;this.B4.I1(24);this.B4.IX(750);
this.B4.IZ(750);this.B4.B5(true);this.B4.H6=0;this.B4.G2=255;this.Ak(this.As,0);
this.Ak(this.Cn,0);this.Ak(this.Cp,0);this.Ak(this.Text,0);this.Cp.E7(B.r0(C.Cp)
);this.Text.E7(B.r0(C.AM));this.B4.CP=[this,this.IT,this.IY];},_Done:function(){
this.__proto__=C.CO;this.As._Done();this.Cn._Done();this.Cp._Done();this.Text._Done(
);this.B4._Done();B.Core.Al._Done.call(this);},_ReInit:function(){B.Core.Al._ReInit.
call(this);this.As._ReInit();this.Cn._ReInit();this.Cp._ReInit();this.Text._ReInit(
);this.B4._ReInit();},_Mark:function(D){var A;B.Core.Al._Mark.call(this,D);if((A=
this.As)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Cn)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Cp)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.B4)._cycle!=D)A._Mark(A._cycle=D);},_className:"Simulation::Hint"
};C.Cp={_class:function(){return B.tf.AM;},0:{Data:function(){return B.tl;},Cache:[
],_this:null}};C.AM={_class:function(){return B.tf.AM;},0:{Data:function(){return B.
tk;},Cache:[],_this:null}};
C._Init=function(){C.CW.__proto__=B.Core.Root;C.CO.__proto__=B.Core.Al;};C.BO=function(
D){var A;if((A=C.As[0]._this)&&(A._cycle!=D))A._Done(C.As[0]._this=null);if((A=C.
Cp[0]._this)&&(A._cycle!=D))A._Done(C.Cp[0]._this=null);if((A=C.AM[0]._this)&&(A.
_cycle!=D))A._Done(C.AM[0]._this=null);};return C;})();

/* Embedded Wizard */