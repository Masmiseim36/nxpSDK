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
var AM=[216,69,696,341];var BD=[0,0,750,500];var Dj=[0,0,800,480];var FO=[0,230,800
,340];var DV="\uF25A";var FP=[0,100,800,195];var FQ="Touch here...";
C.En={D4:null,At:null,En:null,Bc:function(aArg){this.D4=B._NewObject(C.D4,0);this.
D4.H(AM);this.O(this.D4,0);},DriveCursorHitting:function(CT,R,Bz){if(!!this.D4){
this.GY(this.D4);this.D4=null;}return B.Core.Root.DriveCursorHitting.call(this,CT
,R,Bz);},_Init:function(aArg){B.Core.Root._Init.call(this,aArg);B.ui.Aq._Init.call(
this.At={J:this},0);B.ul.IH._Init.call(this.En={J:this},0);this.__proto__=C.En;this.
H(BD);this.At.H(BD);this.En.H(AM);this.O(this.At,0);this.O(this.En,0);this.At.CZ(
B.s$(C.At));this.Bc(aArg);},_Done:function(){this.__proto__=B.Core.Root;this.At.
_Done();this.En._Done();B.Core.Root._Done.call(this);},_ReInit:function(){B.Core.
Root._ReInit.call(this);this.At._ReInit();this.En._ReInit();},_Mark:function(D){
var A;B.Core.Root._Mark.call(this,D);if((A=this.D4)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.At)._cycle!=D)A._Mark(A._cycle=D);if((A=this.En)._cycle!=D)A._Mark(
A._cycle=D);},_className:"Simulation::Application"};C.At={_class:function(){return B.
uj.An;},0:{FileName:"./res/SimulationBackground.png",Format:B.ch,NoOfFrames:1,FrameSize:[
750,500],FrameDelay:0,_this:null}};C.D4={At:null,Ac:null,Ao:null,Text:null,Ck:null
,Lk:0,LA:function(E){this.Ac.Q((this.Ac.Lu()&0x00FFFFFF)|((E&0xFF)<<24));this.Ao.
Ar(E);},Lw:function(){return this.Lk;},_Init:function(aArg){B.Core.M._Init.call(
this,aArg);B.ui.Av._Init.call(this.At={J:this},0);B.ui.Ac._Init.call(this.Ac={J:
this},0);B.ui.Text._Init.call(this.Ao={J:this},0);B.ui.Text._Init.call(this.Text={
J:this},0);B.uk.Ck._Init.call(this.Ck={J:this},0);this.__proto__=C.D4;this.H(Dj);
this.At.S(0x17);this.At.H(Dj);this.At.Q(0xDE2D2D2D);this.Ac.S(0x17);this.Ac.H(Dj
);this.Ac.J4(3);this.Ao.S(0xD);this.Ao.H(FO);this.Ao.Y(DV);this.Text.S(0xD);this.
Text.H(FP);this.Text.Y(FQ);this.Ck.J8=true;this.Ck.HP(24);this.Ck.HM(750);this.Ck.
LC(750);this.Ck.Bd(true);this.Ck.ED=0;this.Ck.EZ=255;this.O(this.At,0);this.O(this.
Ac,0);this.O(this.Ao,0);this.O(this.Text,0);this.Ao.Ax(B.s$(C.Ao));this.Text.Ax(
B.s$(C.A2));this.Ck.AI=[this,this.Lw,this.LA];},_Done:function(){this.__proto__=
B.Core.M;this.At._Done();this.Ac._Done();this.Ao._Done();this.Text._Done();this.
Ck._Done();B.Core.M._Done.call(this);},_ReInit:function(){B.Core.M._ReInit.call(
this);this.At._ReInit();this.Ac._ReInit();this.Ao._ReInit();this.Text._ReInit();
this.Ck._ReInit();},_Mark:function(D){var A;B.Core.M._Mark.call(this,D);if((A=this.
At)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ac)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Ao)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Text)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Ck)._cycle!=D)A._Mark(A._cycle=D);},_className:"Simulation::Hint"};
C.Ao={_class:function(){return B.uj.A2;},0:{Data:function(){return B.uv;},Cache:[
],_this:null}};C.A2={_class:function(){return B.uj.A2;},0:{Data:function(){return B.
uu;},Cache:[],_this:null}};
C._Init=function(){C.En.__proto__=B.Core.Root;C.D4.__proto__=B.Core.M;};C.C2=function(
D){var A;if((A=C.At[0]._this)&&(A._cycle!=D))A._Done(C.At[0]._this=null);if((A=C.
Ao[0]._this)&&(A._cycle!=D))A._Done(C.Ao[0]._this=null);if((A=C.A2[0]._this)&&(A.
_cycle!=D))A._Done(C.A2[0]._this=null);};return C;})();

/* Embedded Wizard */