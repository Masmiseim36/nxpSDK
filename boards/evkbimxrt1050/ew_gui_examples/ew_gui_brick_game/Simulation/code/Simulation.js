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
);if(EmWiApp.um)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.um=(function(){var B=EmWiApp;var C={};
var Ar=[216,69,696,341];var AT=[0,0,750,500];var Cs=[0,0,800,480];var Eu=[0,230,800
,340];var Ev="\uF25A";var Ew=[0,100,800,195];var Ex="Touch here...";
C.Df={C3:null,Aq:null,Df:null,Ce:function(aArg){this.C3=B._NewObject(C.C3,0);this.
C3.T(Ar);this.An(this.C3,0);},DriveCursorHitting:function(B_,M,AP){if(!!this.C3){
this.Ji(this.C3);this.C3=null;}return B.Core.Root.DriveCursorHitting.call(this,B_
,M,AP);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.ui.Bh._Init.call(
this.Aq={S:this},0);B.BrickGame.GS._Init.call(this.Df={S:this},0);this.__proto__=
C.Df;this.T(AT);this.Aq.T(AT);this.Df.T(Ar);this.An(this.Aq,0);this.An(this.Df,0
);this.Aq.Bs(B.s$(C.Aq));this.Ce(aArg);},_Done:function(){this.__proto__=B.Core.
Root;this.Aq._Done();this.Df._Done();B.Core.Root._Done.call(this);},_ReInit:function(
){B.Core.Root._ReInit.call(this);this.Aq._ReInit();this.Df._ReInit();},_Mark:function(
D){var A;B.Core.Root._Mark.call(this,D);if((A=this.C3)&&(A._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.Aq)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Df)._cycle!=D)
A._Mark(A._cycle=D);},_className:"Simulation::Application"};C.Aq={_class:function(
){return B.uj.Ae;},0:{FileName:"./res/SimulationBackground.png",Format:B.ch,NoOfFrames:
1,FrameSize:[750,500],FrameDelay:0,_this:null}};C.C3={Aq:null,Cq:null,Cr:null,Text:
null,B8:null,I1:0,Jc:function(E){this.Cq.Cg((this.Cq.I8()&0x00FFFFFF)|((E&0xFF)<<
24));this.Cr.Je(E);},I9:function(){return this.I1;},_Init:function(aArg){B.Core.
Ai._Init.call(this,aArg);B.ui.CT._Init.call(this.Aq={S:this},0);B.ui.Cq._Init.call(
this.Cq={S:this},0);B.ui.Text._Init.call(this.Cr={S:this},0);B.ui.Text._Init.call(
this.Text={S:this},0);B.uk.B8._Init.call(this.B8={S:this},0);this.__proto__=C.C3;
this.T(Cs);this.Aq.Gc(0x17);this.Aq.T(Cs);this.Aq.Cg(0xDE2D2D2D);this.Cq.Gc(0x17
);this.Cq.T(Cs);this.Cq.Jg(3);this.Cr.Gc(0xD);this.Cr.T(Eu);this.Cr.Fi(Ev);this.
Text.Gc(0xD);this.Text.T(Ew);this.Text.Fi(Ex);this.B8.If=true;this.B8.Jf(24);this.
B8.Jb(750);this.B8.Jd(750);this.B8.Ch(true);this.B8.Ii=0;this.B8.Hk=255;this.An(
this.Aq,0);this.An(this.Cq,0);this.An(this.Cr,0);this.An(this.Text,0);this.Cr.Ff(
B.s$(C.Cr));this.Text.Ff(B.s$(C.AN));this.B8.CS=[this,this.I9,this.Jc];},_Done:function(
){this.__proto__=B.Core.Ai;this.Aq._Done();this.Cq._Done();this.Cr._Done();this.
Text._Done();this.B8._Done();B.Core.Ai._Done.call(this);},_ReInit:function(){B.Core.
Ai._ReInit.call(this);this.Aq._ReInit();this.Cq._ReInit();this.Cr._ReInit();this.
Text._ReInit();this.B8._ReInit();},_Mark:function(D){var A;B.Core.Ai._Mark.call(
this,D);if((A=this.Aq)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Cq)._cycle!=D)A.
_Mark(A._cycle=D);if((A=this.Cr)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Text).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.B8)._cycle!=D)A._Mark(A._cycle=D);},_className:
"Simulation::Hint"};C.Cr={_class:function(){return B.uj.AN;},0:{Data:function(){
return B.up;},Cache:[],_this:null}};C.AN={_class:function(){return B.uj.AN;},0:{
Data:function(){return B.uo;},Cache:[],_this:null}};
C._Init=function(){C.Df.__proto__=B.Core.Root;C.C3.__proto__=B.Core.Ai;};C.BO=function(
D){var A;if((A=C.Aq[0]._this)&&(A._cycle!=D))A._Done(C.Aq[0]._this=null);if((A=C.
Cr[0]._this)&&(A._cycle!=D))A._Done(C.Cr[0]._this=null);if((A=C.AN[0]._this)&&(A.
_cycle!=D))A._Done(C.AN[0]._this=null);};return C;})();

/* Embedded Wizard */