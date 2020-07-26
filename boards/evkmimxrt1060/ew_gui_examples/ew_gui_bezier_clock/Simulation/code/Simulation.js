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
);if(EmWiApp.un)throw new Error("The unit file 'Simulation.js' included twice!");
EmWiApp.un=(function(){var B=EmWiApp;var E={};
var AJ=[216,69,696,341];var Be=[0,0,750,500];var Dh=[0,0,800,480];var CK=[0,230,800
,340];var Ft="\uF25A";var Iy=[0,100,800,195];var Jg="Touch here...";
E.GR={Gm:null,Az:null,GR:null,CY:function(aArg){this.Gm=B._NewObject(E.Gm,0);this.
Gm.P(AJ);this.Z(this.Gm,0);},DriveCursorHitting:function(Ex,T,Bq){if(!!this.Gm){
this.G3(this.Gm);this.Gm=null;}return B.Core.Root.DriveCursorHitting.call(this,Ex
,T,Bq);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.uj.FQ._Init.call(
this.Az={M:this},0);B.BezierClock.K0._Init.call(this.GR={M:this},0);this.__proto__=
E.GR;this.P(Be);this.Az.P(Be);this.GR.P(AJ);this.Z(this.Az,0);this.Z(this.GR,0);
this.Az.Dq(B.s$(E.Az));this.CY(aArg);},_Done:function(){this.__proto__=B.Core.Root;
this.Az._Done();this.GR._Done();B.Core.Root._Done.call(this);},_ReInit:function(
){B.Core.Root._ReInit.call(this);this.Az._ReInit();this.GR._ReInit();},_Mark:function(
D){var A;B.Core.Root._Mark.call(this,D);if((A=this.Gm)&&(A._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.Az)._cycle!=D)A._Mark(A._cycle=D);if((A=this.GR)._cycle!=D)
A._Mark(A._cycle=D);},_className:"Simulation::Application"};E.Az={_class:function(
){return B.ui.An;},0:{FileName:"./res/SimulationBackground.png",Format:B.ch,NoOfFrames:
1,FrameSize:[750,500],FrameDelay:0,_this:null}};E.Gm={Az:null,EV:null,Cg:null,Text:
null,BE:null,SQ:0,TO:function(C){this.EV.Aa((this.EV.Kb()&0x00FFFFFF)|((C&0xFF)<<
24));this.Cg.GZ(C);},Tr:function(){return this.SQ;},_Init:function(aArg){B.Core.
J._Init.call(this,aArg);B.uj.Fn._Init.call(this.Az={M:this},0);B.uj.EV._Init.call(
this.EV={M:this},0);B.uj.Text._Init.call(this.Cg={M:this},0);B.uj.Text._Init.call(
this.Text={M:this},0);B.uk.BE._Init.call(this.BE={M:this},0);this.__proto__=E.Gm;
this.P(Dh);this.Az.Dc(0x17);this.Az.P(Dh);this.Az.Aa(0xDE2D2D2D);this.EV.Dc(0x17
);this.EV.P(Dh);this.EV.CI(3);this.Cg.Dc(0xD);this.Cg.P(CK);this.Cg.Cu(Ft);this.
Text.Dc(0xD);this.Text.P(Iy);this.Text.Cu(Jg);this.BE.Kr=true;this.BE.Kk(24);this.
BE.FU(750);this.BE.TP(750);this.BE.BG(true);this.BE.BZ=0;this.BE.BN=255;this.Z(this.
Az,0);this.Z(this.EV,0);this.Z(this.Cg,0);this.Z(this.Text,0);this.Cg.Fl(B.s$(E.
Cg));this.Text.Fl(B.s$(E.BW));this.BE.Av=[this,this.Tr,this.TO];},_Done:function(
){this.__proto__=B.Core.J;this.Az._Done();this.EV._Done();this.Cg._Done();this.Text.
_Done();this.BE._Done();B.Core.J._Done.call(this);},_ReInit:function(){B.Core.J.
_ReInit.call(this);this.Az._ReInit();this.EV._ReInit();this.Cg._ReInit();this.Text.
_ReInit();this.BE._ReInit();},_Mark:function(D){var A;B.Core.J._Mark.call(this,D
);if((A=this.Az)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EV)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.Cg)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.BE)._cycle!=D)A._Mark(A._cycle=D);},_className:
"Simulation::Hint"};E.Cg={_class:function(){return B.ui.BW;},0:{Data:function(){
return B.ur;},Cache:[],_this:null}};E.BW={_class:function(){return B.ui.BW;},0:{
Data:function(){return B.uq;},Cache:[],_this:null}};
E._Init=function(){E.GR.__proto__=B.Core.Root;E.Gm.__proto__=B.Core.J;};E.DC=function(
D){var A;if((A=E.Az[0]._this)&&(A._cycle!=D))A._Done(E.Az[0]._this=null);if((A=E.
Cg[0]._this)&&(A._cycle!=D))A._Done(E.Cg[0]._this=null);if((A=E.BW[0]._this)&&(A.
_cycle!=D))A._Done(E.BW[0]._this=null);};return E;})();

/* Embedded Wizard */