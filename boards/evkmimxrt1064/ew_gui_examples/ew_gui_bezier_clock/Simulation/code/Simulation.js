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
);if(EmWiApp.tj)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.tj=(function(){var B=EmWiApp;var E={};
var AJ=[216,69,696,341];var Be=[0,0,750,500];var Db=[0,0,800,480];var CD=[0,230,800
,340];var Fk="\uF25A";var Ip=[0,100,800,195];var I9="Touch here...";
E.Ga={FJ:null,Ay:null,Ga:null,CU:function(aArg){this.FJ=B._NewObject(E.FJ,0);this.
FJ.Q(AJ);this.Aa(this.FJ,0);},DriveCursorHitting:function(Eo,V,Bo){if(!!this.FJ){
this.GV(this.FJ);this.FJ=null;}return B.Core.Root.DriveCursorHitting.call(this,Eo
,V,Bo);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.tf.FK._Init.call(
this.Ay={N:this},0);B.BezierClock.JP._Init.call(this.Ga={N:this},0);this.__proto__=
E.Ga;this.Q(Be);this.Ay.Q(Be);this.Ga.Q(AJ);this.Aa(this.Ay,0);this.Aa(this.Ga,0
);this.Ay.Dl(B.r0(E.Ay));this.CU(aArg);},_Done:function(){this.__proto__=E.Ga;this.
Ay._Done();this.Ga._Done();B.Core.Root._Done.call(this);},_ReInit:function(){B.Core.
Root._ReInit.call(this);this.Ay._ReInit();this.Ga._ReInit();},_Mark:function(D){
var A;B.Core.Root._Mark.call(this,D);if((A=this.FJ)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Ay)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ga)._cycle!=D)A._Mark(
A._cycle=D);},_className:"Simulation::Application"};E.Ay={_class:function(){return B.
te.An;},0:{FileName:"./res/SimulationBackground.png",Format:B.b9,NoOfFrames:1,FrameSize:[
750,500],FrameDelay:0,_this:null}};E.FJ={Ay:null,EM:null,Cb:null,Text:null,BB:null
,Sy:0,Tv:function(C){this.EM.Ab((this.EM.J0()&0x00FFFFFF)|((C&0xFF)<<24));this.Cb.
GQ(C);},S_:function(){return this.Sy;},_Init:function(aArg){B.Core.K._Init.call(
this,aArg);B.tf.Ff._Init.call(this.Ay={N:this},0);B.tf.EM._Init.call(this.EM={N:
this},0);B.tf.Text._Init.call(this.Cb={N:this},0);B.tf.Text._Init.call(this.Text={
N:this},0);B.tg.BB._Init.call(this.BB={N:this},0);this.__proto__=E.FJ;this.Q(Db);
this.Ay.C9(0x17);this.Ay.Q(Db);this.Ay.Ab(0xDE2D2D2D);this.EM.C9(0x17);this.EM.Q(
Db);this.EM.CB(3);this.Cb.C9(0xD);this.Cb.Q(CD);this.Cb.Cn(Fk);this.Text.C9(0xD);
this.Text.Q(Ip);this.Text.Cn(I9);this.BB.Kd=true;this.BB.J9(24);this.BB.FO(750);
this.BB.Tw(750);this.BB.Bs(true);this.BB.BU=0;this.BB.BJ=255;this.Aa(this.Ay,0);
this.Aa(this.EM,0);this.Aa(this.Cb,0);this.Aa(this.Text,0);this.Cb.Fd(B.r0(E.Cb)
);this.Text.Fd(B.r0(E.BR));this.BB.Av=[this,this.S_,this.Tv];},_Done:function(){
this.__proto__=E.FJ;this.Ay._Done();this.EM._Done();this.Cb._Done();this.Text._Done(
);this.BB._Done();B.Core.K._Done.call(this);},_ReInit:function(){B.Core.K._ReInit.
call(this);this.Ay._ReInit();this.EM._ReInit();this.Cb._ReInit();this.Text._ReInit(
);this.BB._ReInit();},_Mark:function(D){var A;B.Core.K._Mark.call(this,D);if((A=
this.Ay)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EM)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Cb)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.BB)._cycle!=D)A._Mark(A._cycle=D);},_className:"Simulation::Hint"
};E.Cb={_class:function(){return B.te.BR;},0:{Data:function(){return B.tn;},Cache:[
],_this:null}};E.BR={_class:function(){return B.te.BR;},0:{Data:function(){return B.
tm;},Cache:[],_this:null}};
E._Init=function(){E.Ga.__proto__=B.Core.Root;E.FJ.__proto__=B.Core.K;};E.Dy=function(
D){var A;if((A=E.Ay[0]._this)&&(A._cycle!=D))A._Done(E.Ay[0]._this=null);if((A=E.
Cb[0]._this)&&(A._cycle!=D))A._Done(E.Cb[0]._this=null);if((A=E.BR[0]._this)&&(A.
_cycle!=D))A._Done(E.BR[0]._this=null);};return E;})();

/* Embedded Wizard */