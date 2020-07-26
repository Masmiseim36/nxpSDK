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
);if(EmWiApp.ul)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.ul=(function(){var B=EmWiApp;var C={};
var AX=[216,69,696,341];var BI=[0,0,750,500];var Dz=[0,0,800,480];var Gb=[0,230,800
,340];var Gc="\uF25A";var Gd=[0,100,800,195];var Ge="Touch here...";
C.EA={Eg:null,Bk:null,EA:null,CP:function(aArg){this.Eg=B._NewObject(C.Eg,0);this.
Eg.M(AX);this.R(this.Eg,0);},DriveCursorHitting:function(Dd,Q,BK){if(!!this.Eg){
this.K9(this.Eg);this.Eg=null;}return B.Core.Root.DriveCursorHitting.call(this,Dd
,Q,BK);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.ui.BW._Init.call(
this.Bk={J:this},0);B.um.J7._Init.call(this.EA={J:this},0);this.__proto__=C.EA;this.
M(BI);this.Bk.M(BI);this.EA.M(AX);this.R(this.Bk,0);this.R(this.EA,0);this.Bk.DF(
B.s$(C.Bk));this.CP(aArg);},_Done:function(){this.__proto__=B.Core.Root;this.Bk.
_Done();this.EA._Done();B.Core.Root._Done.call(this);},_ReInit:function(){B.Core.
Root._ReInit.call(this);this.Bk._ReInit();this.EA._ReInit();},_Mark:function(D){
var A;B.Core.Root._Mark.call(this,D);if((A=this.Eg)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.Bk)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EA)._cycle!=D)A._Mark(
A._cycle=D);},_className:"Simulation::Application"};C.Bk={_class:function(){return B.
uk.As;},0:{FileName:"./res/SimulationBackground.png",Format:B.ch,NoOfFrames:1,FrameSize:[
750,500],FrameDelay:0,_this:null}};C.Eg={Bk:null,BU:null,Dw:null,Text:null,Ag:null
,Mk:0,MQ:function(E){this.BU.Ay((this.BU.In()&0x00FFFFFF)|((E&0xFF)<<24));this.Dw.
A4(E);},Mu:function(){return this.Mk;},_Init:function(aArg){B.Core.L._Init.call(
this,aArg);B.ui.T._Init.call(this.Bk={J:this},0);B.ui.BU._Init.call(this.BU={J:this
},0);B.ui.Text._Init.call(this.Dw={J:this},0);B.ui.Text._Init.call(this.Text={J:
this},0);B.uj.Ag._Init.call(this.Ag={J:this},0);this.__proto__=C.Eg;this.M(Dz);this.
Bk.AT(0x17);this.Bk.M(Dz);this.Bk.Ay(0xDE2D2D2D);this.BU.AT(0x17);this.BU.M(Dz);
this.BU.K7(3);this.Dw.AT(0xD);this.Dw.M(Gb);this.Dw.A6(Gc);this.Text.AT(0xD);this.
Text.M(Gd);this.Text.A6(Ge);this.Ag.Lb=true;this.Ag.Bz(24);this.Ag.By(750);this.
Ag.MR(750);this.Ag.Aj(true);this.Ag.Br=0;this.Ag.AW=255;this.R(this.Bk,0);this.R(
this.BU,0);this.R(this.Dw,0);this.R(this.Text,0);this.Dw.B4(B.s$(C.Dw));this.Text.
B4(B.s$(C.Bl));this.Ag.A0=[this,this.Mu,this.MQ];},_Done:function(){this.__proto__=
B.Core.L;this.Bk._Done();this.BU._Done();this.Dw._Done();this.Text._Done();this.
Ag._Done();B.Core.L._Done.call(this);},_ReInit:function(){B.Core.L._ReInit.call(
this);this.Bk._ReInit();this.BU._ReInit();this.Dw._ReInit();this.Text._ReInit();
this.Ag._ReInit();},_Mark:function(D){var A;B.Core.L._Mark.call(this,D);if((A=this.
Bk)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BU)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Dw)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Ag)._cycle!=D)A._Mark(A._cycle=D);},_className:"Simulation::Hint"};
C.Dw={_class:function(){return B.uk.Bl;},0:{Data:function(){return B.uo;},Cache:[
],_this:null}};C.Bl={_class:function(){return B.uk.Bl;},0:{Data:function(){return B.
un;},Cache:[],_this:null}};
C._Init=function(){C.EA.__proto__=B.Core.Root;C.Eg.__proto__=B.Core.L;};C.Dj=function(
D){var A;if((A=C.Bk[0]._this)&&(A._cycle!=D))A._Done(C.Bk[0]._this=null);if((A=C.
Dw[0]._this)&&(A._cycle!=D))A._Done(C.Dw[0]._this=null);if((A=C.Bl[0]._this)&&(A.
_cycle!=D))A._Done(C.Bl[0]._this=null);};return C;})();

/* Embedded Wizard */