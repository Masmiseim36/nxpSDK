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
);if(EmWiApp.tk)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.tk=(function(){var B=EmWiApp;var D={};
var Az=[216,69,696,341];var A3=[0,0,750,500];var BY=[0,0,800,480];var CI=[0,230,800
,340];var CJ="\uF25A";var Du=[0,100,800,195];var Dv="Touch here...";
D.Da={C1:null,BA:null,Da:null,BF:function(aArg){this.C1=B._NewObject(D.C1,0);this.
C1.Ai(Az);this.AZ(this.C1,0);},DriveCursorHitting:function(Cd,O,A5){if(!!this.C1
){this.EC(this.C1);this.C1=null;}return B.Core.Root.DriveCursorHitting.call(this
,Cd,O,A5);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.tf.FD._Init.
call(this.BA={Ae:this},0);B.tj.GC._Init.call(this.Da={Ae:this},0);this.__proto__=
D.Da;this.Ai(A3);this.BA.Ai(A3);this.Da.Ai(Az);this.AZ(this.BA,0);this.AZ(this.Da
,0);this.BA.D7(B.r0(D.BA));this.BF(aArg);},_Done:function(){this.__proto__=D.Da;
this.BA._Done();this.Da._Done();B.Core.Root._Done.call(this);},_ReInit:function(
){B.Core.Root._ReInit.call(this);this.BA._ReInit();this.Da._ReInit();},_Mark:function(
E){var A;B.Core.Root._Mark.call(this,E);if((A=this.C1)&&(A._cycle!=E))A._Mark(A.
_cycle=E);if((A=this.BA)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Da)._cycle!=E)
A._Mark(A._cycle=E);},_className:"Simulation::Application"};D.BA={_class:function(
){return B.te.W;},0:{FileName:"./res/SimulationBackground.png",Format:B.b9,NoOfFrames:
1,FrameSize:[750,500],FrameDelay:0,_this:null}};D.C1={BA:null,Cu:null,Bf:null,Text:
null,B$:null,KP:0,Lt:function(C){this.Cu.BP((this.Cu.K3()&0x00FFFFFF)|((C&0xFF)<<
24));this.Bf.L3(C);},K4:function(){return this.KP;},_Init:function(aArg){B.Core.
Aj._Init.call(this,aArg);B.tf.C5._Init.call(this.BA={Ae:this},0);B.tf.Cu._Init.call(
this.Cu={Ae:this},0);B.tf.Text._Init.call(this.Bf={Ae:this},0);B.tf.Text._Init.call(
this.Text={Ae:this},0);B.th.B$._Init.call(this.B$={Ae:this},0);this.__proto__=D.
C1;this.Ai(BY);this.BA.E5(0x17);this.BA.Ai(BY);this.BA.BP(0xDE2D2D2D);this.Cu.E5(
0x17);this.Cu.Ai(BY);this.Cu.L6(3);this.Bf.E5(0xD);this.Bf.Ai(CI);this.Bf.Bu(CJ);
this.Text.E5(0xD);this.Text.Ai(Du);this.Text.Bu(Dv);this.B$.JY=true;this.B$.L5(24
);this.B$.K5(750);this.B$.LI(750);this.B$.Bn(true);this.B$.J3=0;this.B$.IH=255;this.
AZ(this.BA,0);this.AZ(this.Cu,0);this.AZ(this.Bf,0);this.AZ(this.Text,0);this.Bf.
Dd(B.r0(D.Bf));this.Text.Dd(B.r0(D.AR));this.B$.A2=[this,this.K4,this.Lt];},_Done:
function(){this.__proto__=D.C1;this.BA._Done();this.Cu._Done();this.Bf._Done();this.
Text._Done();this.B$._Done();B.Core.Aj._Done.call(this);},_ReInit:function(){B.Core.
Aj._ReInit.call(this);this.BA._ReInit();this.Cu._ReInit();this.Bf._ReInit();this.
Text._ReInit();this.B$._ReInit();},_Mark:function(E){var A;B.Core.Aj._Mark.call(
this,E);if((A=this.BA)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Cu)._cycle!=E)A.
_Mark(A._cycle=E);if((A=this.Bf)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Text).
_cycle!=E)A._Mark(A._cycle=E);if((A=this.B$)._cycle!=E)A._Mark(A._cycle=E);},_className:
"Simulation::Hint"};D.Bf={_class:function(){return B.te.AR;},0:{Data:function(){
return B.tq;},Cache:[],_this:null}};D.AR={_class:function(){return B.te.AR;},0:{
Data:function(){return B.tp;},Cache:[],_this:null}};
D._Init=function(){D.Da.__proto__=B.Core.Root;D.C1.__proto__=B.Core.Aj;};D.Bw=function(
E){var A;if((A=D.BA[0]._this)&&(A._cycle!=E))A._Done(D.BA[0]._this=null);if((A=D.
Bf[0]._this)&&(A._cycle!=E))A._Done(D.Bf[0]._this=null);if((A=D.AR[0]._this)&&(A.
_cycle!=E))A._Done(D.AR[0]._this=null);};return D;})();

/* Embedded Wizard */