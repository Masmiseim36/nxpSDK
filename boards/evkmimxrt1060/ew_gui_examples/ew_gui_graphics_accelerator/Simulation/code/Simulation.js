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
);if(EmWiApp.uo)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.uo=(function(){var B=EmWiApp;var D={};
var Aw=[216,69,696,341];var A1=[0,0,750,500];var BX=[0,0,800,480];var Dy=[0,230,800
,340];var Df="\uF25A";var Dz=[0,100,800,195];var DA="Touch here...";
D.Dr={Dd:null,BB:null,Dr:null,BG:function(aArg){this.Dd=B._NewObject(D.Dd,0);this.
Dd.Ah(Aw);this.Bc(this.Dd,0);},DriveCursorHitting:function(Cf,N,A7){if(!!this.Dd
){this.EH(this.Dd);this.Dd=null;}return B.Core.Root.DriveCursorHitting.call(this
,Cf,N,A7);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.uj.FR._Init.
call(this.BB={Ad:this},0);B.un.HN._Init.call(this.Dr={Ad:this},0);this.__proto__=
D.Dr;this.Ah(A1);this.BB.Ah(A1);this.Dr.Ah(Aw);this.Bc(this.BB,0);this.Bc(this.Dr
,0);this.BB.Ec(B.s$(D.BB));this.BG(aArg);},_Done:function(){this.__proto__=B.Core.
Root;this.BB._Done();this.Dr._Done();B.Core.Root._Done.call(this);},_ReInit:function(
){B.Core.Root._ReInit.call(this);this.BB._ReInit();this.Dr._ReInit();},_Mark:function(
E){var A;B.Core.Root._Mark.call(this,E);if((A=this.Dd)&&(A._cycle!=E))A._Mark(A.
_cycle=E);if((A=this.BB)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Dr)._cycle!=E)
A._Mark(A._cycle=E);},_className:"Simulation::Application"};D.BB={_class:function(
){return B.ui.V;},0:{FileName:"./res/SimulationBackground.png",Format:B.ch,NoOfFrames:
1,FrameSize:[750,500],FrameDelay:0,_this:null}};D.Dd={BB:null,Ct:null,Bj:null,Text:
null,Ca:null,K4:0,LJ:function(C){this.Ct.BP((this.Ct.Lh()&0x00FFFFFF)|((C&0xFF)<<
24));this.Bj.Mh(C);},Li:function(){return this.K4;},_Init:function(aArg){B.Core.
Ag._Init.call(this,aArg);B.uj.C7._Init.call(this.BB={Ad:this},0);B.uj.Ct._Init.call(
this.Ct={Ad:this},0);B.uj.Text._Init.call(this.Bj={Ad:this},0);B.uj.Text._Init.call(
this.Text={Ad:this},0);B.um.Ca._Init.call(this.Ca={Ad:this},0);this.__proto__=D.
Dd;this.Ah(BX);this.BB.Fb(0x17);this.BB.Ah(BX);this.BB.BP(0xDE2D2D2D);this.Ct.Fb(
0x17);this.Ct.Ah(BX);this.Ct.Mk(3);this.Bj.Fb(0xD);this.Bj.Ah(Dy);this.Bj.Bw(Df);
this.Text.Fb(0xD);this.Text.Ah(Dz);this.Text.Bw(DA);this.Ca.Kb=true;this.Ca.Mj(24
);this.Ca.Lj(750);this.Ca.LY(750);this.Ca.Bv(true);this.Ca.Ke=0;this.Ca.IY=255;this.
Bc(this.BB,0);this.Bc(this.Ct,0);this.Bc(this.Bj,0);this.Bc(this.Text,0);this.Bj.
De(B.s$(D.Bj));this.Text.De(B.s$(D.AT));this.Ca.A4=[this,this.Li,this.LJ];},_Done:
function(){this.__proto__=B.Core.Ag;this.BB._Done();this.Ct._Done();this.Bj._Done(
);this.Text._Done();this.Ca._Done();B.Core.Ag._Done.call(this);},_ReInit:function(
){B.Core.Ag._ReInit.call(this);this.BB._ReInit();this.Ct._ReInit();this.Bj._ReInit(
);this.Text._ReInit();this.Ca._ReInit();},_Mark:function(E){var A;B.Core.Ag._Mark.
call(this,E);if((A=this.BB)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Ct)._cycle!=
E)A._Mark(A._cycle=E);if((A=this.Bj)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Text
)._cycle!=E)A._Mark(A._cycle=E);if((A=this.Ca)._cycle!=E)A._Mark(A._cycle=E);},_className:
"Simulation::Hint"};D.Bj={_class:function(){return B.ui.AT;},0:{Data:function(){
return B.uu;},Cache:[],_this:null}};D.AT={_class:function(){return B.ui.AT;},0:{
Data:function(){return B.ut;},Cache:[],_this:null}};
D._Init=function(){D.Dr.__proto__=B.Core.Root;D.Dd.__proto__=B.Core.Ag;};D.By=function(
E){var A;if((A=D.BB[0]._this)&&(A._cycle!=E))A._Done(D.BB[0]._this=null);if((A=D.
Bj[0]._this)&&(A._cycle!=E))A._Done(D.Bj[0]._this=null);if((A=D.AT[0]._this)&&(A.
_cycle!=E))A._Done(D.AT[0]._this=null);};return D;})();

/* Embedded Wizard */