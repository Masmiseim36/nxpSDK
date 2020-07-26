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

EmWiCompr_9_30=(function(){var d={};d._SetLanguage=function(nC){if(this.lg===nC)return;
this.lg=nC;var e5=this.hq;while(e5){e5._ReInit();e5=e5._next;}};d._SetStyles=function(
re){this.ue=re;};d.tf=function(iw){if(iw._variants)iw=iw._variants();var g9=iw[this.
lg];if(g9!==undefined)return g9;else return iw[0];};d._GetAutoObject=function(gw
){if(gw._variants)gw=gw._variants();if(!gw._this)gw._Init.call(gw._this={});return gw.
_this;};d.s$=function(c4){if(c4._variants)c4=c4._variants();var h9=c4[this.lg];if(
h9===undefined)h9=c4[0];if(!h9._this)c4._class()._Init.call(h9._this={},h9);return h9.
_this;};d._NewObject=function(nt,qT){var g;if(!nt)throw"No class specified to create the object";
nt._Init.call(g={},qT);g._next=this.hq;this.hq=g;return g;};d._LockObject=function(
b9){if(b9)this.hs.push(b9);};d._UnlockObject=function(b9){var o1=this.hs;var g;if(
!b9)return;for(g=o1.length-1;g>=0;g--)if(o1[g]===b9)break;if(g>=0)this.hs.splice(
g,1);};d.ts=function(cZ,ec){var g;for(var h in cZ)if(!isNaN(h)&&(g=cZ[h])&&(g._cycle
!==ec))g._Mark(g._cycle=ec);};d.tu=function(cZ,ec){var g;for(var h in cZ)if(!isNaN(
h)&&(g=cZ[h])&&(g=g[0])&&(g._cycle!==ec))g._Mark(g._cycle=ec);};d.tt=function(cZ
,ec){var g;for(var h in cZ)if(!isNaN(h)&&(g=cZ[h])&&(g=g[0])&&(g._cycle!==ec))g.
_Mark(g._cycle=ec);};d.nj=function(){var ck=(this.ne+=1);var sf=this.gv;var a0=null;
var be=null;var g=null;function k6(nD,ec){var ab=nD;var oF=0;var g;for(;ab;ab=ab.
_next){if((g=ab.ey)&&(g._cycle!==ec)&&(ab[0]._cycle===ec)){g._Mark(g._cycle=ec);
ab=nD;oF++;}}return oF;}for(a0=this.hs.length-1;a0>=0;a0--)if((be=this.hs[a0])&&(
be._cycle!==ck))be._Mark(be._cycle=ck);k6(this.db,ck);while((k6(this.dU,ck)>0)&&(
k6(this.db,ck)>0));while((a0=this.db)&&((a0[0]._cycle!==ck)||((g=a0.ey)&&(g._cycle
!==ck))))this.db=a0._next;while((be=this.dU)&&((be[0]._cycle!==ck)||((g=be.ey)&&(
g._cycle!==ck))))this.dU=be._next;while(a0&&(g=a0._next))if((g.ey&&(g.ey._cycle!==
ck))||(g[0]._cycle!==ck))a0._next=g._next;else a0=g;while(be&&(g=be._next))if((g.
ey&&(g.ey._cycle!==ck))||(g[0]._cycle!==ck))be._next=g._next;else be=g;while((a0=
this.hq)&&(a0._cycle!==ck)){this.hq=a0._next;a0._Done();}if(a0)while(be=a0._next
){if(be._cycle!==ck){a0._next=be._next;be._Done();}else a0=be;}this.tQ(ck);while((
a0=this.fH)&&(a0[0]._cycle!==ck))this.fH=a0._next;if(a0)while(be=a0._next)if(be[
0]._cycle!==ck)a0._next=be._next;else a0=be;while((a0=this.hr)&&(a0._cycle!==ck)
)this.hr=a0.it;while(a0){var kQ;while((be=a0._observers)&&(be[0]._cycle!==ck))a0.
_observers=be._next;if(be)while(kQ=be._next)if(kQ[0]._cycle!==ck)be._next=kQ._next;
else be=kQ;while((be=a0.it)&&(be._cycle!==ck))a0.it=be.it;a0=be;}if(this.a8)console.
log("Living objects: "+this.gv+"("+(this.gv-sf)+")");};d.lq=function(bk,iM){if(!
bk)return;var ab=this.db;var e5=bk[0];var i8=bk[1];var g=null;if(ab&&(ab[0]===e5
)&&(ab[1]===i8)){if(!ab.lu)ab.ey=iM;return;}if(ab)while((g=ab._next)&&((g[0]!==e5
)||(g[1]!==i8)))ab=g;if(g&&g.lu)return;if(g){ab._next=g._next;g._next=null;}bk._next=
this.db;bk.ey=iM;this.db=bk;};d.tg=function(bk,iM){if(!bk)return;var ab=this.dU;
var e5=bk[0];var i8=bk[1];var g;if(ab&&(ab[0]===e5)&&(ab[1]===i8)){if(!ab.lu)ab.
ey=iM;return;}if(ab)while((g=ab._next)&&((g[0]!==e5)||(g[1]!==i8)))ab=g;if(g){ab.
_next=g._next;g._next=null;}bk._next=this.dU;bk.ey=iM;this.dU=bk;};d.qL=function(
){var h0=[];if(!this.db){if(this.dU){this.db=this.dU;this.dU=null;}return false;
}for(;;){var i6=this.db;var me=null;var g;while(i6&&(i6!==h0)){g=i6;i6=i6._next;
g._next=me;me=g;}h0._next=me;this.db=h0;while(g=h0._next){g.lu=true;g[1].call(g[
0],g.ey);h0._next=g._next;}if(this.db===h0){this.db=null;break;}}this.db=this.dU;
this.dU=null;return true;};d.sB=function(bk,eB,b8){var dG;if(!bk||!eB||!(dG=eB[0
]))return;var ab=dG._observers;var c8=bk[0];var c9=bk[1];var g4=eB[1];while(ab&&((
ab[0]!==c8)||(ab[1]!==c9)||(ab[2]!==b8)||(ab[3]!==g4)))ab=ab._next;if(ab)return;
ab=[c8,c9,b8,g4];ab._next=dG._observers;dG._observers=ab;if(!dG.lt){dG.it=this.hr;
dG.lt=true;this.hr=dG;}};d.sz=function(bk,b9,b8){if(!bk||!b9)return;var ab=b9._observers;
var c8=bk[0];var c9=bk[1];while(ab&&((ab[0]!==c8)||(ab[1]!==c9)||(ab[2]!==b8)||ab[
3]))ab=ab._next;if(ab)return;ab=[c8,c9,b8,null];ab._next=b9._observers;b9._observers=
ab;if(!b9.lt){b9.it=this.hr;b9.lt=true;this.hr=b9;}};d.sA=function(bk,b8){if(!bk
)return;var ab=this.fH;var c8=bk[0];var c9=bk[1];while(ab&&((ab[0]!==c8)||(ab[1]
!==c9)||(ab[2]!==b8)))ab=ab._next;if(ab)return;ab=[c8,c9,b8];ab._next=this.fH;this.
fH=ab;};d.sO=function(bk,eB,b8){var dG;var ab;if(!bk||!eB||!(dG=eB[0])||!(ab=dG.
_observers))return;var c8=bk[0];var c9=bk[1];var g4=eB[1];var bM;if((ab[0]===c8)&&(
ab[1]===c9)&&(ab[2]===b8)&&(ab[3]===g4)){dG._observers=ab._next;ab._next=null;return;
}while((bM=ab._next)&&((bM[0]!==c8)||(bM[1]!==c9)||(bM[2]!==b8)||(bM[3]!==g4)))ab=
bM;if(!bM)return;ab._next=bM._next;bM._next=null;};d.sM=function(bk,b9,b8){var ab;
if(!bk||!b9||!(ab=b9._observers))return;var c8=bk[0];var c9=bk[1];var bM;if((ab[
0]===c8)&&(ab[1]===c9)&&(ab[2]===b8)&&!ab[3]){b9._observers=ab._next;ab._next=null;
return;}while((bM=ab._next)&&((bM[0]!==c8)||(bM[1]!==c9)||(bM[2]!==b8)||bM[3]))ab=
bM;if(!bM)return;ab._next=bM._next;bM._next=null;};d.sN=function(bk,b8){var ab;if(
!bk||!(ab=this.fH))return;var c8=bk[0];var c9=bk[1];var bM;if((ab[0]===c8)&&(ab[
1]===c9)&&(ab[2]===b8)){this.fH=ab._next;ab._next=null;return;}while((bM=ab._next
)&&((bM[0]!==c8)||(bM[1]!==c9)||(bM[2]!==b8)))ab=bM;if(!bM)return;ab._next=bM._next;
bM._next=null;};d.tG=function(eB,b8){var dG;var ab;if(!eB||!(dG=eB[0])||!(ab=dG.
_observers))return;if(this.a8)console.log("NotifyRefObserver()");var g4=eB[1];while(
ab){if((ab[2]===b8)&&(ab[3]===g4))this.lq(ab.slice(0,2),null);ab=ab._next;}};d.qw=
function(b9,b8){var ab;if(!b9||!(ab=b9._observers))return;if(this.a8)console.log(
"NotifyObjObserver()");while(ab){if((ab[2]===b8)&&!ab[3])this.lq(ab.slice(0,2),null
);ab=ab._next;}};d.tF=function(b8){var ab;if(!(ab=this.fH))return;if(this.a8)console.
log("NotifyObserver()");while(ab){if(ab[2]===b8)this.lq(ab.slice(0,2),null);ab=ab.
_next;}};function StartTimer(jR,hC){var jh=this;if((jR>0)&&(hC>0))this.fE=window.
setTimeout(function(){p0(jh,hC);},jR);else if(jR>0)this.fE=window.setTimeout(function(
){p1(jh);},jR);else if(hC>0)this.fA=window.setInterval(function(){mZ(jh);},hC);}
function DestroyTimer(){if(this.fA)window.clearInterval(this.fA);this.fA=0;if(this.
fE)window.clearTimeout(this.fE);this.fE=0;this.ia.gv--;this.jw=null;this.ju=null;
this.ia=null;}function ResetTimer(){if(this.fA)window.clearInterval(this.fA);this.
fA=0;if(this.fE)window.clearTimeout(this.fE);this.fE=0;}function p0(dC,hC){dC.p_=
null;dC.fA=window.setInterval(function(){mZ(dC);},hC);try{dC.ju.call(dC.jw);dC.ia.
_RequestUpdate();}catch(bB){console.log("EmWi error: "+bB+"!");}}function p1(dC){
try{dC.p_=null;dC.ju.call(dC.jw);dC.ia._RequestUpdate();}catch(bB){console.log("EmWi error: "+
bB+"!");}}function mZ(dC){try{dC.ju.call(dC.jw);dC.ia._RequestUpdate();}catch(bB
){console.log("EmWi error: "+bB+"!");}}d.sL=function(b9,q4){var jh={DestroyTimer:
DestroyTimer,ResetTimer:ResetTimer,StartTimer:StartTimer,ia:this,jw:b9,ju:q4,fA:
0,fE:0};this.gv++;return jh;};function pk(){var f5=this.mN;var e4=f5.length;var gT=
0;if(arguments.length!==e4)return this.k0;for(var h=0;h<e4;h++){var iZ=f5[h];var
s=arguments[h];if((s<0)||(s>=iZ))return this.k0;gT=(gT*iZ)+s;}var g9=this[gT];if(
g9===undefined)return this.k0;return g9;};function pl(){var f5=this.mN;var e4=f5.
length;var gT=0;if((arguments.length-1)!==e4)return;for(var h=0;h<e4;h++){var iZ=
f5[h];var s=arguments[h];if((s<0)||(s>=iZ))return;gT=(gT*iZ)+s;}return this[gT]=
arguments[e4];};d.tA=function(){var e4=arguments.length-2;var rQ=arguments[e4];var
l8=arguments[e4+1];var f5=[];for(var h=0;h<e4;h++)f5[h]=arguments[h];var hG=[];hG.
mN=f5;hG.k0=rQ;if(l8)for(var h in l8)hG[h]=l8[h];hG.Get=pk;hG.Set=pl;return hG;};
d.uf=function(){function fe(eh,q6){var g=eh.toString(16).toUpperCase();var oW=q6-
g.length;if(oW>0)g=d.jQ.slice(0,oW)+g;return g;}var kt=arguments[0];var bU="EmWi trace: ";
var s=0;var cs=1;while(kt&&(s<kt.length)&&(cs<arguments.length)){if(kt[s++]==='%'
){if(cs>1)bU+=", ";switch(kt[s++]){case'i':bU+=arguments[cs].toFixed();break;case
'u':bU+=arguments[cs].toFixed();break;case'f':bU+=arguments[cs];break;case'e':bU+=
"enum:0x"+fe(arguments[cs],8);break;case't':bU+="set:0x"+fe(arguments[cs],8);break;
case'0':bU+="null";break;case'b':bU+=arguments[cs];break;case'h':bU+="handle:"+arguments[
cs];break;case'c':{var g=arguments[cs];if((g>=0x20)&&(g<=0x7F))bU+="\'"+String.fromCharCode(
g)+"\'";else bU+="\'\\x"+fe(g,4)+"\'";}break;case'o':{var g=arguments[cs];var ay=
g&0xFF;var cI=(g>>8)&0xFF;var a1=(g>>16)&0xFF;var bR=(g>>24)&0xFF;bU+="#"+fe(ay,
2)+fe(cI,2)+fe(a1,2)+fe(bR,2);}break;case'p':{var g=arguments[cs];bU+="<"+g[0]+","+
g[1]+">";}break;case'r':{var g=arguments[cs];bU+="<"+g[0]+","+g[1]+","+g[2]+","+
g[3]+">";}break;case'l':{bU+="language:0x"+fe(arguments[cs],8);}break;case'g':{bU+=
"styles:0x"+fe(arguments[cs],8);}break;case'^':{var g=arguments[cs];if(!g||!g[0]
)bU+="null";else bU+="ref:(Object of class '"+g[0]._className+"', OnGet:'"+g[1].
oJ+"', OnSet:'"+g[2].oJ+"')";}break;case'*':{var g=arguments[cs];if(!g||!g._className
)bU+="null";else bU+="Object of class '"+g._className+"'";}break;case'$':{var g=
arguments[cs];if(!g||!g._className)bU+="null";else bU+=g._className;}break;case'&':{
var g=arguments[cs];if(!g||!g[0])bU+="null";else bU+="slot:(Object of class '"+g[
0]._className+"', SlotProc:'"+g[1].oJ+"')";}break;case's':{var g=arguments[cs];bU+=
"\""+g+"\"";}break;}cs++;}}console.log(bU);};d._RequestUpdate=function(){if(this.
io)return;var eo=this;var oZ=window.requestAnimationFrame||window.webkitRequestAnimationFrame||
window.mozRequestAnimationFrame||window.oRequestAnimationFrame||window.msRequestAnimationFrame;
function pd(){eo.qI();}if(oZ)oZ(pd);else window.setTimeout(pd,15);this.io=true;};
d.nl=function(){if(this.im)return;var eo=this;function sg(){eo.qE();}window.setTimeout(
sg,1000);this.im=true;};d.qI=function(){if(!this.io)return;this.io=false;try{var
op=this.qL();if(this.gr&&this.bV.DoesNeedUpdate()){var aF=this._NewObject(this.Graphics.
Canvas,0);this.bj.mQ=null;this.bj.fD=null;if(this.jI)this.gr.cH[0].jp=true;aF.AttachBitmap(
this.gr);this.bV.UpdateGE20(aF);aF.DetachBitmap();if(this.gr.cH[0].jp){this.bj.clear(
this.bj.COLOR_BUFFER_BIT);this.gr.cH[0].jp=false;}op=true;}if(op)this.nl();if(this.
db||this.dU)this._RequestUpdate();}catch(bB){console.log("EmWi error: "+bB+"!");
}};d.qE=function(){if(!this.im)return;this.im=false;this.nj();};function p$(lF,w
){var oD=[w.Enter,w.Escape,w.Backspace,w.CtrlShiftPageUp,w.CtrlShiftPageDown,w.CtrlShiftUp
,w.CtrlShiftDown,w.CtrlShiftKeyP,w.CtrlShiftKeyR,w.CtrlShiftLeft,w.CtrlShiftRight
,w.CtrlShiftBackspace,w.CtrlShiftKeyE,w.CtrlShiftKeyT,w.CtrlShiftKeyN,w.CtrlShiftKeyK
,w.CtrlShiftKeyG,w.CtrlShiftKeyU,w.CtrlShiftKeyA,w.CtrlShiftKeyD,w.CtrlShiftKeyM
,w.CtrlShiftKey0,w.CtrlShiftKey1,w.CtrlShiftKey2,w.CtrlShiftKey3,w.CtrlShiftKey4
,w.CtrlShiftKey5,w.CtrlShiftKey6,w.CtrlShiftKey7,w.CtrlShiftKey8,w.CtrlShiftKey9
,w.CtrlKeyM,w.CtrlPageUp,w.CtrlPageDown,w.CtrlInsert,w.CtrlDelete,w.CtrlKeyP,w.CtrlKeyS
,w.CtrlLeft,w.CtrlRight,w.CtrlUp,w.CtrlDown,w.CtrlKeyH,w.CtrlKeyR,w.CtrlKeyG,w.CtrlKeyB
,w.CtrlKeyY,w.CtrlKeyL,w.CtrlKeyN,w.CtrlKeyW,w.CtrlKeyD,w.CtrlKeyV,w.CtrlKeyE,w.
CtrlKeyO,w.CtrlKeyT,w.CtrlKeyQ,w.CtrlKeyA,w.CtrlKeyC,w.CtrlKeyU,w.CtrlKeyZ,w.CtrlKeyX
,w.CtrlKeyI,w.CtrlHome,w.CtrlEnd,w.CtrlKey0,w.CtrlKey1,w.CtrlKey2,w.CtrlKey3,w.CtrlKey4
,w.CtrlKey5,w.CtrlKey6,w.CtrlKey7,w.CtrlKey8,w.CtrlKey9];var r$=[w.Ok,w.Exit,w.Clear
,w.ChannelUp,w.ChannelDown,w.SkipPrev,w.SkipNext,w.Pause,w.Record,w.SlowRev,w.SlowFwd
,w.Eject,w.Repeat,w.Timer,w.Navigation,w.Karaoke,w.Game,w.Setup,w.Angle,w.Mode,w.
Mute,w.User10,w.User11,w.User12,w.User13,w.User14,w.User15,w.User16,w.User17,w.User18
,w.User19,w.Menu,w.VolumeUp,w.VolumeDown,w.Show,w.Hide,w.Play,w.Stop,w.Rev,w.Fwd
,w.SkipBwd,w.SkipFwd,w.Help,w.Red,w.Green,w.Blue,w.Yellow,w.White,w.Magenta,w.TV
,w.DVD,w.VCR,w.EPG,w.OSD,w.Text,w.PIP,w.Audio,w.Clock,w.Subtitle,w.Zoom,w.Index,
w.Info,w.Display,w.Power,w.User0,w.User1,w.User2,w.User3,w.User4,w.User5,w.User6
,w.User7,w.User8,w.User9];var s=oD.length-1;while((s>=0)&&(oD[s]!==lF))s--;if(s>=
0)lF=r$[s];return lF;}d.qA=function(ao){var c=this.Core.KeyCode;var l_=ao.type===
"keyup";var keyCode=c.NoKey;var g8=c.NoKey;var charCode=0;if(((ao.type==="keydown"
)||(ao.type==="keypress"))&&this.li){var ad=this.li.call(this,ao);if(ad&&ad.KeyCode
)keyCode=ad.KeyCode;else if(ad&&ad.d6)charCode=ad.d6;}if((ao.type==="keypress")&&
!ao.ctrlKey&&(ao.charCode!==13)&&(ao.charCode!==8)&&(keyCode===c.NoKey)&&!charCode
)charCode=ao.charCode;else if((ao.type==="keydown")&&(keyCode===c.NoKey)&&!charCode
){if(ao.ctrlKey&&ao.shiftKey&&!ao.altKey)switch(ao.keyCode){case 13:keyCode=c.CtrlShiftEnter;
break;case 27:keyCode=c.CtrlShiftEscape;break;case 8:keyCode=c.CtrlShiftBackspace;
break;case 9:keyCode=c.CtrlShiftTab;break;case 40:keyCode=c.CtrlShiftDown;break;
case 38:keyCode=c.CtrlShiftUp;break;case 37:keyCode=c.CtrlShiftLeft;break;case 39:
keyCode=c.CtrlShiftRight;break;case 36:keyCode=c.CtrlShiftHome;break;case 35:keyCode=
c.CtrlShiftEnd;break;case 33:keyCode=c.CtrlShiftPageUp;break;case 34:keyCode=c.CtrlShiftPageDown;
break;case 45:keyCode=c.CtrlShiftInsert;break;case 46:keyCode=c.CtrlShiftDelete;
break;case 112:keyCode=c.CtrlShiftF1;break;case 113:keyCode=c.CtrlShiftF2;break;
case 114:keyCode=c.CtrlShiftF3;break;case 115:keyCode=c.CtrlShiftF4;break;case 116:
keyCode=c.CtrlShiftF5;break;case 117:keyCode=c.CtrlShiftF6;break;case 118:keyCode=
c.CtrlShiftF7;break;case 119:keyCode=c.CtrlShiftF8;break;case 120:keyCode=c.CtrlShiftF9;
break;case 121:keyCode=c.CtrlShiftF10;break;case 32:keyCode=c.CtrlShiftSpace;break;
case 48:keyCode=c.CtrlShiftKey0;break;case 49:keyCode=c.CtrlShiftKey1;break;case
50:keyCode=c.CtrlShiftKey2;break;case 51:keyCode=c.CtrlShiftKey3;break;case 52:keyCode=
c.CtrlShiftKey4;break;case 53:keyCode=c.CtrlShiftKey5;break;case 54:keyCode=c.CtrlShiftKey6;
break;case 55:keyCode=c.CtrlShiftKey7;break;case 56:keyCode=c.CtrlShiftKey8;break;
case 57:keyCode=c.CtrlShiftKey9;break;case 96:keyCode=c.CtrlShiftKey0;break;case
97:keyCode=c.CtrlShiftKey1;break;case 98:keyCode=c.CtrlShiftKey2;break;case 99:keyCode=
c.CtrlShiftKey3;break;case 100:keyCode=c.CtrlShiftKey4;break;case 101:keyCode=c.
CtrlShiftKey5;break;case 102:keyCode=c.CtrlShiftKey6;break;case 103:keyCode=c.CtrlShiftKey7;
break;case 104:keyCode=c.CtrlShiftKey8;break;case 105:keyCode=c.CtrlShiftKey9;break;
case 65:keyCode=c.CtrlShiftKeyA;break;case 66:keyCode=c.CtrlShiftKeyB;break;case
67:keyCode=c.CtrlShiftKeyC;break;case 68:keyCode=c.CtrlShiftKeyD;break;case 69:keyCode=
c.CtrlShiftKeyE;break;case 70:keyCode=c.CtrlShiftKeyF;break;case 71:keyCode=c.CtrlShiftKeyG;
break;case 72:keyCode=c.CtrlShiftKeyH;break;case 73:keyCode=c.CtrlShiftKeyI;break;
case 74:keyCode=c.CtrlShiftKeyJ;break;case 75:keyCode=c.CtrlShiftKeyK;break;case
76:keyCode=c.CtrlShiftKeyL;break;case 77:keyCode=c.CtrlShiftKeyM;break;case 78:keyCode=
c.CtrlShiftKeyN;break;case 79:keyCode=c.CtrlShiftKeyO;break;case 80:keyCode=c.CtrlShiftKeyP;
break;case 81:keyCode=c.CtrlShiftKeyQ;break;case 82:keyCode=c.CtrlShiftKeyR;break;
case 83:keyCode=c.CtrlShiftKeyS;break;case 84:keyCode=c.CtrlShiftKeyT;break;case
85:keyCode=c.CtrlShiftKeyU;break;case 86:keyCode=c.CtrlShiftKeyV;break;case 87:keyCode=
c.CtrlShiftKeyW;break;case 88:keyCode=c.CtrlShiftKeyX;break;case 89:keyCode=c.CtrlShiftKeyY;
break;case 90:keyCode=c.CtrlShiftKeyZ;break;}else if(ao.ctrlKey&&!ao.shiftKey&&!
ao.altKey)switch(ao.keyCode){case 13:keyCode=c.CtrlEnter;break;case 27:keyCode=c.
CtrlEscape;break;case 8:keyCode=c.CtrlBackspace;break;case 9:keyCode=c.CtrlTab;break;
case 40:keyCode=c.CtrlDown;break;case 38:keyCode=c.CtrlUp;break;case 37:keyCode=
c.CtrlLeft;break;case 39:keyCode=c.CtrlRight;break;case 36:keyCode=c.CtrlHome;break;
case 35:keyCode=c.CtrlEnd;break;case 33:keyCode=c.CtrlPageUp;break;case 34:keyCode=
c.CtrlPageDown;break;case 45:keyCode=c.CtrlInsert;break;case 46:keyCode=c.CtrlDelete;
break;case 112:keyCode=c.CtrlF1;break;case 113:keyCode=c.CtrlF2;break;case 114:keyCode=
c.CtrlF3;break;case 115:keyCode=c.CtrlF4;break;case 116:keyCode=c.CtrlF5;break;case
117:keyCode=c.CtrlF6;break;case 118:keyCode=c.CtrlF7;break;case 119:keyCode=c.CtrlF8;
break;case 120:keyCode=c.CtrlF9;break;case 121:keyCode=c.CtrlF10;break;case 32:keyCode=
c.CtrlSpace;break;case 48:keyCode=c.CtrlKey0;break;case 49:keyCode=c.CtrlKey1;break;
case 50:keyCode=c.CtrlKey2;break;case 51:keyCode=c.CtrlKey3;break;case 52:keyCode=
c.CtrlKey4;break;case 53:keyCode=c.CtrlKey5;break;case 54:keyCode=c.CtrlKey6;break;
case 55:keyCode=c.CtrlKey7;break;case 56:keyCode=c.CtrlKey8;break;case 57:keyCode=
c.CtrlKey9;break;case 96:keyCode=c.CtrlKey0;break;case 97:keyCode=c.CtrlKey1;break;
case 98:keyCode=c.CtrlKey2;break;case 99:keyCode=c.CtrlKey3;break;case 100:keyCode=
c.CtrlKey4;break;case 101:keyCode=c.CtrlKey5;break;case 102:keyCode=c.CtrlKey6;break;
case 103:keyCode=c.CtrlKey7;break;case 104:keyCode=c.CtrlKey8;break;case 105:keyCode=
c.CtrlKey9;break;case 65:keyCode=c.CtrlKeyA;break;case 66:keyCode=c.CtrlKeyB;break;
case 67:keyCode=c.CtrlKeyC;break;case 68:keyCode=c.CtrlKeyD;break;case 69:keyCode=
c.CtrlKeyE;break;case 70:keyCode=c.CtrlKeyF;break;case 71:keyCode=c.CtrlKeyG;break;
case 72:keyCode=c.CtrlKeyH;break;case 73:keyCode=c.CtrlKeyI;break;case 74:keyCode=
c.CtrlKeyJ;break;case 75:keyCode=c.CtrlKeyK;break;case 76:keyCode=c.CtrlKeyL;break;
case 77:keyCode=c.CtrlKeyM;break;case 78:keyCode=c.CtrlKeyN;break;case 79:keyCode=
c.CtrlKeyO;break;case 80:keyCode=c.CtrlKeyP;break;case 81:keyCode=c.CtrlKeyQ;break;
case 82:keyCode=c.CtrlKeyR;break;case 83:keyCode=c.CtrlKeyS;break;case 84:keyCode=
c.CtrlKeyT;break;case 85:keyCode=c.CtrlKeyU;break;case 86:keyCode=c.CtrlKeyV;break;
case 87:keyCode=c.CtrlKeyW;break;case 88:keyCode=c.CtrlKeyX;break;case 89:keyCode=
c.CtrlKeyY;break;case 90:keyCode=c.CtrlKeyZ;break;}else if(!ao.ctrlKey&&!ao.shiftKey&&
!ao.altKey)switch(ao.keyCode){case 13:keyCode=c.Enter;break;case 27:keyCode=c.Escape;
break;case 8:keyCode=c.Backspace;break;case 9:keyCode=c.Tab;break;case 40:keyCode=
c.Down;break;case 38:keyCode=c.Up;break;case 37:keyCode=c.Left;break;case 39:keyCode=
c.Right;break;case 36:keyCode=c.Home;break;case 35:keyCode=c.End;break;case 33:keyCode=
c.PageUp;break;case 34:keyCode=c.PageDown;break;case 45:keyCode=c.Insert;break;case
46:keyCode=c.Delete;break;case 112:keyCode=c.F1;break;case 113:keyCode=c.F2;break;
case 114:keyCode=c.F3;break;case 115:keyCode=c.F4;break;case 116:keyCode=c.F5;break;
case 117:keyCode=c.F6;break;case 118:keyCode=c.F7;break;case 119:keyCode=c.F8;break;
case 120:keyCode=c.F9;break;case 121:keyCode=c.F10;break;}else if(!ao.ctrlKey&&!
ao.shiftKey&&ao.altKey)switch(ao.keyCode){case 13:keyCode=c.AltEnter;break;case 27:
keyCode=c.AltEscape;break;case 8:keyCode=c.AltBackspace;break;case 9:keyCode=c.AltTab;
break;case 40:keyCode=c.AltDown;break;case 38:keyCode=c.AltUp;break;case 37:keyCode=
c.AltLeft;break;case 39:keyCode=c.AltRight;break;case 36:keyCode=c.AltHome;break;
case 35:keyCode=c.AltEnd;break;case 33:keyCode=c.AltPageUp;break;case 34:keyCode=
c.AltPageDown;break;case 45:keyCode=c.AltInsert;break;case 46:keyCode=c.AltDelete;
break;case 112:keyCode=c.AltF1;break;case 113:keyCode=c.AltF2;break;case 114:keyCode=
c.AltF3;break;case 115:keyCode=c.AltF4;break;case 116:keyCode=c.AltF5;break;case
117:keyCode=c.AltF6;break;case 118:keyCode=c.AltF7;break;case 119:keyCode=c.AltF8;
break;case 120:keyCode=c.AltF9;break;case 121:keyCode=c.AltF10;break;}else if(!ao.
ctrlKey&&ao.shiftKey&&ao.altKey)switch(ao.keyCode){case 13:keyCode=c.AltShiftEnter;
break;case 27:keyCode=c.AltShiftEscape;break;case 8:keyCode=c.AltShiftBackspace;
break;case 9:keyCode=c.AltShiftTab;break;case 40:keyCode=c.AltShiftDown;break;case
38:keyCode=c.AltShiftUp;break;case 37:keyCode=c.AltShiftLeft;break;case 39:keyCode=
c.AltShiftRight;break;case 36:keyCode=c.AltShiftHome;break;case 35:keyCode=c.AltShiftEnd;
break;case 33:keyCode=c.AltShiftPageUp;break;case 34:keyCode=c.AltShiftPageDown;
break;case 45:keyCode=c.AltShiftInsert;break;case 46:keyCode=c.AltShiftDelete;break;
case 112:keyCode=c.AltShiftF1;break;case 113:keyCode=c.AltShiftF2;break;case 114:
keyCode=c.AltShiftF3;break;case 115:keyCode=c.AltShiftF4;break;case 116:keyCode=
c.AltShiftF5;break;case 117:keyCode=c.AltShiftF6;break;case 118:keyCode=c.AltShiftF7;
break;case 119:keyCode=c.AltShiftF8;break;case 120:keyCode=c.AltShiftF9;break;case
121:keyCode=c.AltShiftF10;break;}else if(!ao.ctrlKey&&ao.shiftKey&&!ao.altKey)switch(
ao.keyCode){case 13:keyCode=c.ShiftEnter;break;case 27:keyCode=c.ShiftEscape;break;
case 8:keyCode=c.ShiftBackspace;break;case 9:keyCode=c.ShiftTab;break;case 40:keyCode=
c.ShiftDown;break;case 38:keyCode=c.ShiftUp;break;case 37:keyCode=c.ShiftLeft;break;
case 39:keyCode=c.ShiftRight;break;case 36:keyCode=c.ShiftHome;break;case 35:keyCode=
c.ShiftEnd;break;case 33:keyCode=c.ShiftPageUp;break;case 34:keyCode=c.ShiftPageDown;
break;case 45:keyCode=c.ShiftInsert;break;case 46:keyCode=c.ShiftDelete;break;case
112:keyCode=c.ShiftF1;break;case 113:keyCode=c.ShiftF2;break;case 114:keyCode=c.
ShiftF3;break;case 115:keyCode=c.ShiftF4;break;case 116:keyCode=c.ShiftF5;break;
case 117:keyCode=c.ShiftF6;break;case 118:keyCode=c.ShiftF7;break;case 119:keyCode=
c.ShiftF8;break;case 120:keyCode=c.ShiftF9;break;case 121:keyCode=c.ShiftF10;break;
}g8=p$(keyCode,c);if(g8===this.dw)keyCode=this.dw;else if(keyCode===this.dw)g8=this.
dw;}if((keyCode===c.NoKey)&&!charCode&&(!l_||((this.dw===c.NoKey)&&!this.eW)))return;
try{if((this.dw!==c.NoKey)&&((keyCode!==this.dw)||l_)){this.bV.DriveKeyboardHitting(
this.dw,0,false);this.dw=c.NoKey;}if(this.eW&&((charCode!==this.eW)||l_)){this.bV.
DriveKeyboardHitting(c.NoKey,this.eW,false);this.eW=0;}if(charCode&&this.bV.DriveKeyboardHitting(
c.NoKey,charCode,true))this.eW=charCode;else if((g8!==keyCode)&&(g8!==c.NoKey)&&
this.bV.DriveKeyboardHitting(g8,0,true))this.dw=g8;else if((keyCode!==c.NoKey)&&
this.bV.DriveKeyboardHitting(keyCode,0,true))this.dw=keyCode;this._RequestUpdate(
);}catch(bB){console.log("EmWi error: "+bB+"!");}ao.stopPropagation();ao.preventDefault(
);};d.qz=function(ao){var c=this.Core.KeyCode;try{if(this.dw!==c.NoKey){if(this.
bV.DriveKeyboardHitting)this.bV.DriveKeyboardHitting(this.dw,0,false);else{var i1=
this._NewObject(this.Core.KeyEvent,0);i1.Initialize(this.dw,false);this.bV.DispatchEvent(
i1);}this.dw=c.NoKey;this._RequestUpdate();}if(this.eW){if(this.bV.DriveKeyboardHitting
)this.bV.DriveKeyboardHitting(c.NoKey,this.eW,false);else{var i1=this._NewObject(
this.Core.KeyEvent,0);i1.Initialize2(this.eW,false);this.bV.DispatchEvent(i1);}this.
eW=0;this._RequestUpdate();}}catch(bB){console.log("EmWi error: "+bB+"!");}};function
k3(ao,dx){ao.stopPropagation();ao.preventDefault();var l=ao.clientX+window.pageXOffset;
var x=ao.clientY+window.pageYOffset;var eF=dx;for(;eF;eF=eF.offsetParent){l-=eF.
offsetLeft;x-=eF.offsetTop;}l=((dx.width*l)/dx.offsetWidth)|0;x=((dx.height*x)/dx.
offsetHeight)|0;return[l,x];};d.qB=function(ao){if(this.gu||ao.rl||this.ea)return;
this.d$.focus();document.addEventListener("mousemove",this.hp,true);document.addEventListener(
"mouseup",this.jN,true);this.gu=true;var ax=k3(ao,this.d$);try{this.bV.DriveCursorHitting(
true,0,ax);this._RequestUpdate();}catch(bB){console.log("EmWi error: "+bB+"!");}
};d.qD=function(ao){if(!this.gu||ao.rl)return;document.removeEventListener("mousemove"
,this.hp,true);document.removeEventListener("mouseup",this.jN,true);this.gu=false;
var ax=k3(ao,this.d$);try{this.bV.DriveCursorHitting(false,0,ax);this._RequestUpdate(
);}catch(bB){console.log("EmWi error: "+bB+"!");}};d.qC=function(ao){if(!this.gu
)return;var ax=k3(ao,this.d$);try{this.bV.DriveCursorMovement(ax);this._RequestUpdate(
);}catch(bB){console.log("EmWi error: "+bB+"!");}};function k5(nJ,dx){var l=nJ.clientX;
var x=nJ.clientY;var eF=dx;for(;eF;eF=eF.offsetParent){l-=eF.offsetLeft;x-=eF.offsetTop;
}l=((dx.width*l)/dx.offsetWidth)|0;x=((dx.height*x)/dx.offsetHeight)|0;return[l,
x];}d.qH=function(ao){var ji=this.ea;var mr=(new Date()).getTime();if(this.gu||(
this.ea===10))return;for(var h=0;h<ao.changedTouches.length;h++){var dQ=ao.changedTouches[
h];var bI=0;var ri=[dQ.screenX-16,dQ.screenY-16,dQ.screenX+32,dQ.screenY+32];while((
bI<10)&&((this.hu[bI]!==undefined)||(this.lr[bI]===undefined)||((mr-this.nk[bI])>
500)||!this.qu(ri,this.lr[bI])))bI++;if(bI==10)for(bI=0;(bI<10)&&(this.hu[bI]!==
undefined);)bI++;if(bI==10)return;this.hu[bI]=dQ.identifier;this.ea++;try{this.bV.
DriveMultiTouchHitting(true,bI,k5(dQ,this.d$));this._RequestUpdate();}catch(bB){
console.log("EmWi error: "+bB+"!");}}if(ji!==this.ea){this.d$.focus();ao.stopPropagation(
);ao.preventDefault();}};d.qF=function(ao){var ji=this.ea;var mr=(new Date()).getTime(
);if(this.ea===0)return;for(var h=0;h<ao.changedTouches.length;h++){var dQ=ao.changedTouches[
h];var bI=9;while((bI>=0)&&(this.hu[bI]!==dQ.identifier))bI--;if(bI>=0){this.lr[
bI]=[dQ.screenX,dQ.screenY];this.nk[bI]=mr;this.hu[bI]=undefined;this.ea--;try{this.
bV.DriveMultiTouchHitting(false,bI,k5(dQ,this.d$));this._RequestUpdate();}catch(
bB){console.log("EmWi error: "+bB+"!");}}}if(ji!==this.ea){ao.stopPropagation();
ao.preventDefault();}};d.qG=function(ao){var ji=this.ea;if(this.ea===0)return;for(
var h=0;h<ao.changedTouches.length;h++){var dQ=ao.changedTouches[h];var bI=9;while((
bI>=0)&&(this.hu[bI]!==dQ.identifier))bI--;if(bI>=0){try{this.bV.DriveMultiTouchMovement(
bI,k5(dQ,this.d$));this._RequestUpdate();}catch(bB){console.log("EmWi error: "+bB+
"!");}}}if(ji!==this.ea){ao.stopPropagation();ao.preventDefault();}};d._Init=function(
dx,eg){var eo=this;var aF=document.getElementById(dx);var nS=this.m7();var a=null;
var aw={};var bitmap={};this.qt=(new Date()).getTime();if(eg&&eg.hasOwnProperty(
"Debug"))this.a8=eg.Debug;if(eg&&eg.hasOwnProperty("FullScreenUpdate"))this.jI=eg.
FullScreenUpdate;if(eg&&eg.hasOwnProperty("Alpha"))this.lc=eg.Alpha;if(eg&&eg.hasOwnProperty(
"OnKeyEvent"))this.li=eg.OnKeyEvent;if(this.a8)console.log("EmWi debug: Initializing application '"+
nS._className+"'.");console.log("EmWi info: Init application for canvas '"+dx+"'. "+
"FullScreenUpdate="+this.jI+", "+"Alpha="+this.lc+", "+"Debug="+this.a8+".");if(
!aF){console.log("EmWi error: Initialization failed. The canvas element '"+dx+"' doesn't exist!"
);return false;}var fV={depth:false,antialias:false,alpha:this.lc,preserveDrawingBuffer:
!this.jI};a=aF.getContext("webgl",fV)||aF.getContext("experimental-webgl",fV);if(
!a){console.log("EmWi error: WebGL context creation failed.");return false;}this.
gt=a.getParameter(a.MAX_TEXTURE_SIZE);if(this.qs>this.gt)this.qs=this.gt;this.qr=(
this.jL/this.gs)|0;a.clearColor(0.0,0.0,0.0,0.0);a.pixelStorei(a.UNPACK_PREMULTIPLY_ALPHA_WEBGL
,true);a.pixelStorei(a.UNPACK_ALIGNMENT,1);a.blendEquation(a.FUNC_ADD);a.blendFunc(
a.ONE,a.ONE_MINUS_SRC_ALPHA);a.disable(a.BLEND);a.cC=new ArrayBuffer(14*6*4);a.g_=
new Float32Array(a.cC);a.h$=new Uint32Array(a.cC);a.kV=0;a.kX=1;a.kW=2;a.bindBuffer(
a.ARRAY_BUFFER,a.createBuffer());a.bufferData(a.ARRAY_BUFFER,a.cC,a.STREAM_DRAW);
a.vertexAttribPointer(a.kV,2,a.FLOAT,false,6*4,0*4);a.vertexAttribPointer(a.kX,3
,a.FLOAT,false,6*4,2*4);a.vertexAttribPointer(a.kW,4,a.UNSIGNED_BYTE,true,6*4,5*
4);a.enableVertexAttribArray(a.kV);a.enableVertexAttribArray(a.kX);a.enableVertexAttribArray(
a.kW);this.ij=a.createTexture();a.bindTexture(a.TEXTURE_2D,this.ij);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_MIN_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MAG_FILTER
,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.CLAMP_TO_EDGE);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_WRAP_T,a.CLAMP_TO_EDGE);a.texImage2D(a.TEXTURE_2D,0,a.ALPHA
,this.jL,this.jL,0,a.ALPHA,a.UNSIGNED_BYTE,null);this.ho=new Uint32Array(this.gs
*this.gs/32);aw.Framebuffer=a.getParameter(a.FRAMEBUFFER_BINDING);aw.Format=this.
ch;aw.Width=aF.width;aw.Height=aF.height;aw.ig=aF.width;aw.ie=aF.height;aw.d_=1/
aF.width;aw.d9=1/aF.height;aw.eT=2/aF.width;aw.eS=2/aF.height;aw.cL=null;bitmap.
Format=this.ch;bitmap.cH=[aw];bitmap.FrameSize=[aF.width,aF.height];bitmap.FrameDelay=
0;bitmap.NoOfFrames=1;bitmap.b2=[{aJ:bitmap.cH[0],aM:[0,0]}];if(!pU(a)){console.
log("EmWi error: WebGL initialization failed.");return false;}if(this.a8)ex(a);a.
pw=[0,0,0,-1,0,0,0,-1,0,0,0,0,0,0,0,1];this.d$=aF;this.bj=a;this.gr=bitmap;try{this.
bV=this._NewObject(nS,0);this.bV.Initialize(this.tS);}catch(bB){console.log("EmWi error: "+
bB+"!");this._Done();return false;}function oI(ao){eo.qB(ao);}function sd(ao){eo.
qD(ao);}function mc(ao){eo.qC(ao);}function pa(ao){eo.qH(ao);}function ms(ao){eo.
qF(ao);}function o$(ao){eo.qG(ao);}function kB(ao){eo.qA(ao);}function nU(ao){eo.
qz(ao);}this.lh=oI;this.jN=sd;this.hp=mc;this.np=pa;this.ls=ms;this.no=o$;this.jM=
kB;this.sI=nU;aF.addEventListener("mousedown",oI,true);aF.addEventListener("mousemove"
,mc,false);aF.addEventListener("mouseout",mc,false);aF.addEventListener("touchstart"
,pa,true);aF.addEventListener("touchend",ms,false);aF.addEventListener("touchcancel"
,ms,false);aF.addEventListener("touchmove",o$,false);aF.addEventListener("keydown"
,kB,true);aF.addEventListener("keyup",kB,false);aF.addEventListener("keypress",kB
,false);aF.addEventListener("blur",nU,false);this._RequestUpdate();this.nl();this.
_LockObject(this.bV);aF.focus();return true;};d._Done=function(){if(this.a8)console.
log("EmWi debug: De-Initializing application '"+this.m7()._className+"'.");if(this.
d$&&this.lh){var aF=this.d$;aF.removeEventListener("mousedown",this.lh,true);aF.
removeEventListener("mousemove",this.hp,false);aF.removeEventListener("mouseout"
,this.hp,false);aF.removeEventListener("touchstart",this.np,true);aF.removeEventListener(
"touchend",this.ls,false);aF.removeEventListener("touchcancel",this.ls,false);aF.
removeEventListener("touchmove",this.no,false);aF.removeEventListener("keydown",
this.jM,true);aF.removeEventListener("keyup",this.jM,false);aF.removeEventListener(
"keypress",this.jM,false);document.removeEventListener("mousemove",this.hp,true);
document.removeEventListener("mouseup",this.jN,true);delete this.lh;delete this.
jN;delete this.hp;delete this.np;delete this.ls;delete this.no;delete this.jM;}if(
this.bj){hk(this.bj,null);dH(this.bj,null,null);this.bj.clear(this.bj.COLOR_BUFFER_BIT
);this.bj.flush();this.bj.finish();this.bj.id=null;this.bj.hh=null;this.bj.hi=null;
this.bj.jy=null;this.bj.hj=null;this.bj.jz=null;this.bj.gq=null;this.bj.jA=null;
ex(this.bj);}while(this.fG)la(this,this.fG);this.ij=null;this.ho=null;this.fG=null;
this.jK=null;this.gr=null;this.bj=null;this.d$=null;this.db=null;this.dU=null;if(
this.bV)this._UnlockObject(this.bV);this.bV=null;try{this.nj();}catch(bB){console.
log("EmWi error: "+bB+"!");}if(this.a8)console.log("EmWi debug: "+this.gv+" objects after the final GC run."
);this.hs=[];this.hq=null;this.gv=0;this.ne=0;this.io=false;this.im=false;this.gu=
false;};d.tz=function(az,ak){return[az[0]+ak[0],az[1]+ak[1],az[2]+ak[0],az[3]+ak[
1]];};d.ty=function(az,ak){return[az[0]-ak[0],az[1]-ak[1],az[2]-ak[0],az[3]-ak[1
]];};d.il=function(cq,cr){var bY=cq[0];var b0=cq[1];var bZ=cq[2];var b1=cq[3];var
dr=cr[0];var dt=cr[1];var ds=cr[2];var du=cr[3];if(bZ<=bY)bY=bZ=0;if(b1<=b0)b0=b1=
0;if(ds<=dr)dr=ds=0;if(du<=dt)dt=du=0;if(bY<dr)bY=dr;if(bZ>ds)bZ=ds;if(b0<dt)b0=
dt;if(b1>du)b1=du;if(bZ<=bY)bY=bZ=0;if(b1<=b0)b0=b1=0;return[bY,b0,bZ,b1];};d.tj=
function(cq,cr){var bY=cq[0];var b0=cq[1];var bZ=cq[2];var b1=cq[3];var dr=cr[0];
var dt=cr[1];var ds=cr[2];var du=cr[3];if(bZ<=bY)bY=bZ=0;if(b1<=b0)b0=b1=0;if(ds<=
dr)dr=ds=0;if(du<=dt)dt=du=0;if((bY===bZ)||(b0===b1))return cr;if((dr===ds)||(dt===
du))return cq;if(bY<dr)bY=dr;if(bZ>ds)bZ=ds;if(b0<dt)b0=dt;if(b1>du)b1=du;if(bZ<=
bY)bY=bZ=0;if(b1<=b0)b0=b1=0;return[bY,b0,bZ,b1];};d.qR=function(cq,cr){var bY=cq[
0];var b0=cq[1];var bZ=cq[2];var b1=cq[3];var dr=cr[0];var dt=cr[1];var ds=cr[2];
var du=cr[3];if(bZ<=bY)bY=bZ=0;if(b1<=b0)b0=b1=0;if(ds<=dr)dr=ds=0;if(du<=dt)dt=
du=0;if((bY===bZ)||(b0===b1))return cr;if((dr===ds)||(dt===du))return cq;if(bY>dr
)bY=dr;if(bZ<ds)bZ=ds;if(b0>dt)b0=dt;if(b1<du)b1=du;if(bZ<=bY)bY=bZ=0;if(b1<=b0)
b0=b1=0;return[bY,b0,bZ,b1];};d.th=function(az,jS){return[az[0]-jS[0],az[1]-jS[1
],az[2]+jS[0],az[3]+jS[1]];};d.t3=function(az,j2){return[az[0],j2,az[2],az[3]];};
d.t1=function(az,j1){return[az[0],az[1],j1,az[3]];};d.t0=function(az,a2){return[
a2,az[1],a2+az[2]-az[0],az[3]];};d.t2=function(az,bu){return[az[0],bu,az[2],bu+az[
3]-az[1]];};d.tZ=function(az,rh){return[az[0],az[1],az[0]+rh,az[3]];};d.tW=function(
az,q0){return[az[0],az[1],az[2],az[1]+q0];};d.tX=function(az,jW){return[jW[0],jW[
1],jW[0]+az[2]-az[0],jW[1]+az[3]-az[1]];};d.tY=function(az,bo){return[az[0],az[1
],az[0]+bo[0],az[1]+bo[1]];};d.tm=function(cq,cr){return(cq[0]===cr[0])&&(cq[1]===
cr[1])&&(cq[2]===cr[2])&&(cq[3]===cr[3]);};d.qu=function(az,eX){if((eX[0]<az[0])||(
eX[0]>=az[2]))return false;if((eX[1]<az[1])||(eX[1]>=az[3]))return false;return true;
};d.s9=function(az){return(az[2]-az[0])*(az[3]-az[1]);};d.s_=function(az){var l=
az[0];var x=az[1];return[l+((az[2]-l)>>1),x+((az[3]-x)>>1)];};d.tx=function(eX,ak
){return[eX[0]+ak[0],eX[1]+ak[1]];};d.tw=function(eX,ak){return[eX[0]-ak[0],eX[1
]-ak[1]];};d.tl=function(nG,nH){return(nG[0]===nH[0])&&(nG[1]===nH[1]);};d.tN=function(
a9,bf){var ay=(a9&0xFF)+(bf&0xFF);var cI=((a9>>8)&0xFF)+((bf>>8)&0xFF);var a1=((
a9>>16)&0xFF)+((bf>>16)&0xFF);var bR=((a9>>24)&0xFF)+((bf>>24)&0xFF);if(ay>255)ay=
255;if(cI>255)cI=255;if(a1>255)a1=255;if(bR>255)bR=255;return ay|(cI<<8)|(a1<<16
)|(bR<<24);};d.tv=function(a9,bf){var ay=(a9&0xFF)-(bf&0xFF);var cI=((a9>>8)&0xFF
)-((bf>>8)&0xFF);var a1=((a9>>16)&0xFF)-((bf>>16)&0xFF);var bR=((a9>>24)&0xFF)-((
bf>>24)&0xFF);if(ay<0)ay=0;if(cI<0)cI=0;if(a1<0)a1=0;if(bR<0)bR=0;return ay|(cI<<
8)|(a1<<16)|(bR<<24);};d.sG=function(a9,bf){var ay=(bf&0xFF);var cI=((bf>>8)&0xFF
);var a1=((bf>>16)&0xFF);var bR=((bf>>24)&0xFF);var kC=255-bR;ay=((ay*bR)+((a9&0xFF
)*kC))/255;cI=((cI*bR)+(((a9>>8)&0xFF)*kC))/255;a1=((a1*bR)+(((a9>>16)&0xFF)*kC)
)/255;bR=255-((kC*(255-((a9>>24)&0xFF)))/255);if(ay>255)ay=255;if(cI>255)cI=255;
if(a1>255)a1=255;if(bR>255)bR=255;return ay|(cI<<8)|(a1<<16)|(bR<<24);};d.sH=function(
cM,fI){var ay=(cM&0xFF);var cI=((cM>>8)&0xFF);var a1=((cM>>16)&0xFF);var bR=((cM>>
24)&0xFF);if(fI<0)fI=0;if(fI>255)fI=255;ay=(ay*fI)/255;cI=(cI*fI)/255;a1=(a1*fI)
/255;bR=(bR*fI)/255;return ay|(cI<<8)|(a1<<16)|(bR<<24);};d.tE=function(eh,al,b_
){if((b_!==2)&&(b_!==8)&&(b_!==16))b_=10;if(al>128)al=128;var g=eh.toString(b_);
var gn=al-g.length;if(gn>0)g=d.jQ.slice(0,gn)+g;return g;};d.tD=function(eh,al,b_
){if((b_!==2)&&(b_!==8)&&(b_!==16))b_=10;var g9=(eh<0)?-eh:eh;if(al>128)al=128;var
g=g9.toString(b_);var gn=al-g.length-((eh<0)?1:0);if(gn>0)g=d.jQ.slice(0,gn)+g;if(
eh<0)return"-"+g;else return g;};d.tC=function(eh,al,iE){if(al<0)al=0;if(al>32)al=
32;if(iE<0)iE=0;if(iE>32)iE=32;var i$=(eh<0)?"-":"";var g=Math.abs(eh).toFixed(iE
);var gn=al-g.length-i$.length;if(gn>0)g=d.jQ.slice(0,gn)+g;return i$+g;};d.tB=function(
lD,al){al=Math.floor(al);if(al<=0)return this.hm;return new Array(al+1).join(String.
fromCharCode(lD));};d.t8=function(m,al){if(al<=0)return this.hm;if(al>=m.length)
return m;return m.slice(0,al|0);};d.uc=function(m,al){if(al<=0)return this.hm;if(
al>=m.length)return m;return m.slice(-(al|0));};d.t9=function(m,bg,al){var aQ=(bg>
0)?(bg|0):0;var cT=(bg+al)|0;var bl=m.length;if((aQ>=bl)||(cT<=0)||(aQ>=cT))return this.
hm;if((aQ===0)&&(cT>=bl))return m;return m.slice(aQ,cT);};d.t7=function(hE,iQ,bg
){var o6=hE.length;var sh=iQ.length;if(sh<=0)return hE;if(o6<=0)return iQ;if(bg<
0)return iQ+hE;if(bg>=o6)return hE+iQ;return hE.slice(0,(bg|=0))+iQ+hE.slice(bg);
};d.ub=function(m,bg,al){var aQ=(bg>=0)?(bg|0):0;var cT=(bg+al)|0;var bl=m.length;
if((aQ>=bl)||(cT<=0)||(aQ>=cT))return m;return m.slice(0,aQ)+m.slice(cT);};d.t$=
function(m,ix,b_){if((b_!==2)&&(b_!==8)&&(b_!==16))b_=10;var g=parseInt(m,b_);if(
!isNaN(g))return g;return ix;};d.ua=function(m,ix,b_){if((b_!==2)&&(b_!==8)&&(b_
!==16))b_=10;var g=parseInt(m,b_);if(!isNaN(g)){if(g<0)g+=0x100000000;return g;}
return ix;};d.t_=function(m,ix){var g=parseFloat(m);if(!isNaN(g))return g;return ix;
};d.t4=function(m,bg,lD){if((bg<0)||(bg>=m.length))return m;return m.slice(0,bg)+
String.fromCharCode(lD)+m.slice(bg+1);};d.tn=function(iF,iG){if(!iF&&!iG)return true;
if(!iF||!iG)return false;return(iF[0]===iG[0])&&(iF[1]===iG[1])&&(iF[2]===iG[2]);
};d.to=function(jY,jZ){if(!jY&&!jZ)return true;if(!jY||!jZ)return false;return(jY[
0]===jZ[0])&&(jY[1]===jZ[1]);};d.tP=function(iR,lR){return(iR+((lR-iR)*Math.random(
)))|0;};d.tO=function(iR,lR){return iR+((lR-iR)*Math.random());};function ex(o){
var nR=false;var oq;while((oq=o.getError())!==o.NO_ERROR){console.log("EmWi error: WebGL error '"+
oq+"' reported.");nR=true;}return nR;}var qd="precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+
"attribute highp   vec2 dst_pos;\n"+"\n"+"void main()\n"+"{\n"+"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+
"}\n";var qe="precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+"attribute highp   vec2 dst_pos;\n"+
"attribute lowp    vec4 src_clr;\n"+"varying   lowp    vec4 src_clr_var;\n"+"\n"+
"void main()\n"+"{\n"+"  src_clr_var = vec4( src_clr.rgb * src_clr.a, src_clr.a );\n"+
"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+"}\n";var qf=
"precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+"attribute highp   vec2 dst_pos;\n"+
"attribute highp   vec3 src_pos;\n"+"varying   highp   vec3 src_pos_var;\n"+"\n"+
"void main()\n"+"{\n"+"  src_pos_var = src_pos;\n"+"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+
"}\n";var qg="precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+"attribute highp   vec2 dst_pos;\n"+
"attribute highp   vec3 src_pos;\n"+"attribute lowp    vec4 src_clr;\n"+"varying   highp   vec3 src_pos_var;\n"+
"varying   lowp    vec4 src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  src_clr_var = vec4( src_clr.rgb * src_clr.a, src_clr.a );\n"+
"  src_pos_var = src_pos;\n"+"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+
"}\n";var pC="precision mediump float;\n"+"void main()\n"+"{\n"+"  gl_FragColor = vec4( 0.0, 0.0, 0.0, 0.0 );\n"+
"}\n";var pD="precision mediump float;\n"+"varying   lowp vec4 src_clr_var;\n"+"\n"+
"void main()\n"+"{\n"+"  gl_FragColor = src_clr_var;\n"+"}\n";var pE="precision mediump float;\n"+
"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_pos_var.xy );\n"+
"}\n";var pF="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2DProj( src, src_pos_var );\n"+
"}\n";var pG="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_pos_var.xy ) * src_clr_var.a;\n"+
"}\n";var pH="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2DProj( src, src_pos_var ) * src_clr_var.a;\n"+
"}\n";var pI="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_pos_var.xy ).a * src_clr_var;\n"+
"}\n";var pJ="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2DProj( src, src_pos_var ).a * src_clr_var;\n"+
"}\n";var pK="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"uniform   highp   vec2      src_size;\n"+
"uniform   highp   vec2      src_ofs;\n"+"varying   highp   vec3      src_pos_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_ofs + fract( src_pos_var.xy ) * src_size );\n"+
"}\n";var pL="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"uniform   highp   vec2      src_size;\n"+
"uniform   highp   vec2      src_ofs;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_ofs + fract( src_pos_var.xy ) * src_size )\n"+
"                            * src_clr_var.a;\n"+"}\n";var pM="precision mediump float;\n"+
"uniform   lowp    sampler2D src;\n"+"uniform   highp   vec2      src_size;\n"+"uniform   highp   vec2      src_ofs;\n"+
"varying   highp   vec3      src_pos_var;\n"+"varying   lowp    vec4      src_clr_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_ofs + fract( src_pos_var.xy ) * src_size ).a\n"+
"                            * src_clr_var;\n"+"}\n";function pU(o){var sr=dv(o,
o.VERTEX_SHADER,qd);var ss=dv(o,o.VERTEX_SHADER,qe);var mu=dv(o,o.VERTEX_SHADER,
qf);var h_=dv(o,o.VERTEX_SHADER,qg);var rS=dv(o,o.FRAGMENT_SHADER,pC);var rT=dv(
o,o.FRAGMENT_SHADER,pD);var rU=dv(o,o.FRAGMENT_SHADER,pE);var rV=dv(o,o.FRAGMENT_SHADER
,pF);var rW=dv(o,o.FRAGMENT_SHADER,pG);var rX=dv(o,o.FRAGMENT_SHADER,pH);var rY=
dv(o,o.FRAGMENT_SHADER,pI);var rZ=dv(o,o.FRAGMENT_SHADER,pJ);var r0=dv(o,o.FRAGMENT_SHADER
,pK);var r1=dv(o,o.FRAGMENT_SHADER,pL);var r2=dv(o,o.FRAGMENT_SHADER,pM);o.id=ev(
o,sr,rS);o.hh=ev(o,ss,rT);o.hi=ev(o,mu,rU);o.jy=ev(o,mu,rV);o.hj=ev(o,h_,rW);o.jz=
ev(o,h_,rX);o.gq=ev(o,h_,rY);o.jA=ev(o,h_,rZ);o.k9=ev(o,mu,r0);o.k_=ev(o,h_,r1);
o.k$=ev(o,h_,r2);if(o.id&&o.hh&&o.hi&&o.jy&&o.hj&&o.jz&&o.gq&&o.jA&&o.k9&&o.k_&&
o.k$)return true;o.id=null;o.hh=null;o.hi=null;o.jy=null;o.hj=null;o.jz=null;o.gq=
null;o.jA=null;o.k9=null;o.k_=null;o.k$=null;return false;}function dv(o,ra,rb){
var i_=o.createShader(ra);o.shaderSource(i_,rb);o.compileShader(i_);var bd=o.getShaderParameter(
i_,o.COMPILE_STATUS);var log=o.getShaderInfoLog(i_);if(bd&&log.length)console.log(
"EmWi info: "+log);if(!bd)console.log("EmWi error: "+log);if(!bd)return null;return i_;
}function ev(o,nM,ny){var c7=o.createProgram();if(!nM||!ny)return null;o.attachShader(
c7,nM);o.attachShader(c7,ny);o.bindAttribLocation(c7,o.kV,"dst_pos");o.bindAttribLocation(
c7,o.kX,"src_pos");o.bindAttribLocation(c7,o.kW,"src_clr");o.linkProgram(c7);var
bd=o.getProgramParameter(c7,o.LINK_STATUS);var log=o.getProgramInfoLog(c7);if(bd&&
log.length)console.log("EmWi info: "+log);if(!bd)console.log("EmWi error: "+log);
if(!bd)return null;var sm=o.getUniformLocation(c7,"dst_map");var pc=o.getUniformLocation(
c7,"src");var sn=o.getUniformLocation(c7,"src_ofs");var so=o.getUniformLocation(
c7,"src_size");o.useProgram(c7);if(pc>=0)o.uniform1i(pc,0);c7.qa=sm;c7.qb=sn;c7.
qc=so;return c7;}function dH(o,aa,eA){if(aa!==o.mQ){if(aa&&aa.cL&&!aa.Framebuffer
){aa.Framebuffer=o.createFramebuffer();o.bindFramebuffer(o.FRAMEBUFFER,aa.Framebuffer
);o.framebufferTexture2D(o.FRAMEBUFFER,o.COLOR_ATTACHMENT0,o.TEXTURE_2D,aa.cL,0);
}else if(aa)o.bindFramebuffer(o.FRAMEBUFFER,aa?aa.Framebuffer:null);if(aa)o.viewport(
0,0,aa.ig,aa.ie);o.mQ=aa;}if(o.pr!==eA){o.useProgram(eA);o.pr=eA;}if(aa&&eA&&((aa.
eT!==eA.eT)||(aa.eS!==eA.eS)||(!!aa.cL!==!!eA.cL))){var gZ=o.pw;if(aa.cL){gZ[0]=
aa.eT;gZ[5]=aa.eS;gZ[7]=-1;}else{gZ[0]=aa.eT;gZ[5]=-aa.eS;gZ[7]=1;}o.uniformMatrix4fv(
eA.qa,false,gZ);eA.eT=aa.eT;eA.eS=aa.eS;eA.cL=!!aa.cL;}if(aa&&aa.jp){o.clear(o.COLOR_BUFFER_BIT
);aa.jp=false;}}function hk(o,aC,fL){if(aC!==o.fD){if(aC)o.bindTexture(o.TEXTURE_2D
,aC.cL);o.fD=aC;}if(aC&&(!aC.px!==!fL)){if(fL){o.texParameteri(o.TEXTURE_2D,o.TEXTURE_MIN_FILTER
,o.LINEAR);o.texParameteri(o.TEXTURE_2D,o.TEXTURE_MAG_FILTER,o.LINEAR);}else{o.texParameteri(
o.TEXTURE_2D,o.TEXTURE_MIN_FILTER,o.NEAREST);o.texParameteri(o.TEXTURE_2D,o.TEXTURE_MAG_FILTER
,o.NEAREST);}aC.px=fL;}}function aS(a9,bf,hB){if(hB<=0)return a9;if(hB>=1)return bf;
var lV=(a9&0xFF);var lW=((a9>>8)&0xFF);var lX=((a9>>16)&0xFF);var lY=((a9>>24)&0xFF
);var rm=(bf&0xFF);var rn=((bf>>8)&0xFF);var ro=((bf>>16)&0xFF);var rp=((bf>>24)&
0xFF);lV+=(rm-lV)*hB;lW+=(rn-lW)*hB;lX+=(ro-lX)*hB;lY+=(rp-lY)*hB;return lV|(lW<<
8)|(lX<<16)|(lY<<24);}function js(bt,gL,cp,lO,dX){for(var h=0;h<(cp-(lO?1:0));h++
){var bT=h;var cU=(h+1)%cp;var ku=bt[bT*6];var l5=bt[cU*6];if(((dX>ku)&&(dX<l5))||((
dX<ku)&&(dX>l5))){var cA=(dX-ku)/(l5-ku);var fq=1.0-cA;var eI=(cp-h-1)*6;if(eI>0
){var aQ=(h+1)*6;bt.set(bt.subarray(aQ,aQ+eI),aQ+6);}cp++;if(cU)cU++;h++;var j=bt[
bT=bT*6+1];var cb=bt[bT+=1];var cc=bt[bT+=1];var fw=bt[bT+=1];var b$=gL[bT+1];var
p=bt[cU=cU*6+1];var cJ=bt[cU+=1];var bW=bt[cU+=1];var jk=bt[cU+=1];var ei=gL[cU+
1];if(j!==p)j=j*fq+p*cA;if(cb!==cJ)cb=cb*fq+cJ*cA;if(cc!==bW)cc=cc*fq+bW*cA;if(fw
!==jk)fw=fw*fq+jk*cA;if(b$!==ei)b$=aS(b$,ei,cA);bt[bT=h*6]=dX;bt[bT+=1]=j;bt[bT+=
1]=cb;bt[bT+=1]=cc;bt[bT+=1]=fw;gL[bT+1]=b$;}}return cp;}function jr(bt,gL,cp,lO
,dX){for(var h=0;h<(cp-(lO?1:0));h++){var bT=h;var cU=(h+1)%cp;var kv=bt[bT*6+1];
var l6=bt[cU*6+1];if(((dX>kv)&&(dX<l6))||((dX<kv)&&(dX>l6))){var cA=(dX-kv)/(l6-
kv);var fq=1.0-cA;var eI=(cp-h-1)*6;if(eI>0){var aQ=(h+1)*6;bt.set(bt.subarray(aQ
,aQ+eI),aQ+6);}cp++;if(cU)cU++;h++;var k=bt[bT*=6];var cb=bt[bT+=2];var cc=bt[bT+=
1];var fw=bt[bT+=1];var b$=gL[bT+1];var r=bt[cU*=6];var cJ=bt[cU+=2];var bW=bt[cU+=
1];var jk=bt[cU+=1];var ei=gL[cU+1];if(k!==r)k=k*fq+r*cA;if(cb!==cJ)cb=cb*fq+cJ*
cA;if(cc!==bW)cc=cc*fq+bW*cA;if(fw!==jk)fw=fw*fq+jk*cA;if(b$!==ei)b$=aS(b$,ei,cA
);bt[bT=h*6]=k;bt[bT+=1]=dX;bt[bT+=1]=cb;bt[bT+=1]=cc;bt[bT+=1]=fw;gL[bT+1]=b$;}
}return cp;}function mJ(n,cp,q2,q$){for(var h=0;h<cp;h++){var l=n[h*6];if((l<q2)||(
l>q$)){var eI=(cp-h-1)*6;if(eI>0){var aQ=(h+1)*6;n.set(n.subarray(aQ,aQ+eI),aQ-6
);}h--;cp--;}}return cp;}function mI(n,cp,rf,qU){for(var h=0;h<cp;h++){var x=n[h
*6+1];if((x<rf)||(x>qU)){var eI=(cp-h-1)*6;if(eI>0){var aQ=(h+1)*6;n.set(n.subarray(
aQ,aQ+eI),aQ-6);}h--;cp--;}}return cp;}var kY=[0x00000000,0x00000001,0x00000003,
0x00000007,0x0000000F,0x0000001F,0x0000003F,0x0000007F,0x000000FF,0x000001FF,0x000003FF
,0x000007FF,0x00000FFF,0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,0x0001FFFF,0x0003FFFF
,0x0007FFFF,0x000FFFFF,0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF,0x01FFFFFF,0x03FFFFFF
,0x07FFFFFF,0x0FFFFFFF,0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF,0xFFFFFFFF];function pA(
aj,gH,fO){var r8=aj.gs-fO;var gX=aj.gs>>5;for(var ay=0;ay<=r8;ay++)for(var at=0;
at<gX;at++){var ai=32-gH;var ft=kY[gH];var pe=aj.ho[ay*gX+at];var eJ;do{while(ai&&(
pe&ft)){ft<<=1;ai--;}eJ=!(pe&ft);for(var ml=ay+1;eJ&&(ml<(ay+fO));ml++)eJ=!(aj.ho[
ml*gX+at]&ft);if(!eJ){ft<<=1;ai--;}}while(!eJ&&(ai>0));if(eJ)return[(at*32)+32-gH-
ai,ay];}return null;}function p9(aj,ff,iK,gH,fO){var ft=kY[gH]<<(ff&0x1F);var at=
ff>>5;var gX=aj.gs>>5;for(;fO>0;iK++,fO--)aj.ho[iK*gX+at]|=ft;}function p8(aj,ff
,iK,gH,fO){var ft=~(kY[gH]<<(ff&0x1F));var at=ff>>5;var gX=aj.gs>>5;for(;fO>0;iK++
,fO--)aj.ho[iK*gX+at]&=ft;}function mD(aj,co){co.hf=aj.fG;co.fa=null;if(aj.fG)aj.
fG.fa=co;else aj.jK=co;aj.fG=co;co.Cache[co.d6]=co;}function la(aj,co){if(co.hf)
co.hf.fa=co.fa;else aj.jK=co.fa;if(co.fa)co.fa.hf=co.hf;else aj.fG=co.hf;co.hf=null;
co.fa=null;co.Cache[co.d6]=null;}function d7(a5,fi,al){var kw=0;var min=0;var max=
al-1;var dg=-1;var ax;while(max>=min){kw=(max+min)>>1;ax=kw*8;dg=a5-fi.charCodeAt(
ax);if(!dg)return ax;if(dg>0)min=kw+1;else if(dg<0)max=kw-1;}return-1;}var hc=[[
0x10,0x01],[0x02,0x05],[0x03,0x04],[0x11,0x12],[0x14,0x18],[0x06,0x09],[0x07,0x08
],[0x13,0x16],[0x17,0x1C],[0x0A,0x0B],[0x1E,0x1F],[0x0C,0x0D],[0x15,0x19],[0x1A,
0x0E],[0x1B,0x1D]];function ps(aC,aE,ak,al){var e;var s=0;var eQ=0;var dZ=0;var hO=
aE.length;eQ=ak>>4;ak&=0xF;e=aC.charCodeAt(eQ++)>>ak;while(al--){if(e&1)s=hc[s][
1];else s=hc[s][0];if(s>15){s-=16;aE[dZ++]=(s&0x01)?0xFF:0x00;if(dZ!==hO)aE[dZ++
]=(s&0x02)?0xFF:0x00;if(dZ!==hO)aE[dZ++]=(s&0x04)?0xFF:0x00;if(dZ!==hO)aE[dZ++]=(
s&0x08)?0xFF:0x00;s=0;}if((ak=(ak+1))&0x0F)e>>=1;else e=aC.charCodeAt(eQ++);}}function
pt(aC,aE,ak,al){var pb=[0x00,0x55,0xAA,0xFF];var e;var s=0;var eQ=0;var dZ=0;var
hO=aE.length;eQ=ak>>4;ak&=0xF;e=aC.charCodeAt(eQ++)>>ak;while(al--){if(e&1)s=hc[
s][1];else s=hc[s][0];if(s>15){s-=16;aE[dZ++]=pb[s&0x03];if(dZ!==hO)aE[dZ++]=pb[
s>>2];s=0;}if((ak=(ak+1))&0x0F)e>>=1;else e=aC.charCodeAt(eQ++);}}function pu(aC
,aE,ak,al){var e;var s=0;var eQ=0;var dZ=0;var hO=aE.length;eQ=ak>>4;ak&=0xF;e=aC.
charCodeAt(eQ++)>>ak;while(al--){if(e&1)s=hc[s][1];else s=hc[s][0];if(s>15){s-=16;
aE[dZ++]=s|(s<<4);s=0;}if((ak=(ak+1))&0x0F)e>>=1;else e=aC.charCodeAt(eQ++);}}function
qh(bn,al,aD){var d0=0;var ek=aD;al-=aD;while(al--)bn[ek++]^=bn[d0++];}d.nc=function(
jU,nz,qX,q7){var bitmap={cH:[],b2:[],bK:null};var kF=Math.max(q7,1);var width=nz[
0];var height=nz[1];var a=this.bj;if((jU!==d.ch)&&(jU!==d.lp)){console.log("EmWi error: Wrong pixel format of bitmap to create."
);return null;}if((width<=0)||(width>(this.gt-2))||(height<=0)||(height>(this.gt-
2))){console.log("EmWi error: Invalid size of bitmap to create.");return null;}var
gc=((this.gt-1)/(width+1))|0;var mb=((this.gt-1)/(height+1))|0;var hV=gc*mb;var oz=(
kF%hV)||hV;var oA=Math.min(oz,gc);var oB=((oz+gc-1)/gc)|0;var sc=gc*width+gc+1;var
sb=mb*height+mb+1;var r_=oA*width+oA+1;var r9=oB*height+oB+1;var oS=((kF+hV-1)/hV
)|0;for(var h=0;h<oS;h++){a.getError();var g7=sc;var g6=sb;var en=a.createTexture(
);var aw={};if(h===(oS-1)){g7=r_;g6=r9;}a.fD=null;a.bindTexture(a.TEXTURE_2D,en);
a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MIN_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D
,a.TEXTURE_MAG_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.
CLAMP_TO_EDGE);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_T,a.CLAMP_TO_EDGE);if(
jU===this.ch)a.texImage2D(a.TEXTURE_2D,0,a.RGBA,g7,g6,0,a.RGBA,a.UNSIGNED_BYTE,null
);else a.texImage2D(a.TEXTURE_2D,0,a.ALPHA,g7,g6,0,a.ALPHA,a.UNSIGNED_BYTE,null);
aw.Format=jU;aw.Width=g7;aw.Height=g6;aw.ig=g7;aw.ie=g6;aw.d_=1/g7;aw.d9=1/g6;aw.
eT=2/g7;aw.eS=2/g6;aw.cL=en;aw.Framebuffer=null;bitmap.cH[h]=aw;if(this.a8)ex(a);
}for(var h=0;h<kF;h++){var f_=h%hV;var oU=f_%gc;var oV=(f_/gc)|0;var ct={};ct.aJ=
bitmap.cH[(h/hV)|0];ct.aM=[(oU*width)+oU+1,(oV*height)+oV+1];bitmap.b2[h]=ct;}bitmap.
FrameSize=[width,height];bitmap.FrameDelay=qX;bitmap.NoOfFrames=kF;return bitmap;
};function mY(aj,bS){try{var oa=bS.fc;var eL=bS.Image;var width=eL.width;var height=
eL.height;var gU=bS.FrameSize[0];var hQ=bS.FrameSize[1];var oP=(width/gU)|0;var a=
aj.bj;delete bS.fc;delete bS.Image;if(!oa.bitmap)return;if((eL.width===gU)&&(eL.
height===hQ)){var ct=bS.b2[0];var en=ct.aJ.cL;var aY=ct.aM[0];var aG=ct.aM[1];a.
bindTexture(a.TEXTURE_2D,en);a.fD=null;if(ct.aJ.Format===aj.ch)a.texSubImage2D(a.
TEXTURE_2D,0,aY,aG,a.RGBA,a.UNSIGNED_BYTE,eL);else a.texSubImage2D(a.TEXTURE_2D,
0,aY,aG,a.ALPHA,a.UNSIGNED_BYTE,eL);}else{var aF=document.createElement('canvas'
);var l1;aF.width=gU;aF.height=hQ;l1=aF.getContext("2d");for(var h=0;h<bS.p7;h++
){var ct=bS.b2[h];var en=ct.aJ.cL;var aY=ct.aM[0];var aG=ct.aM[1];var kK=(h/oP)|
0;var lZ=h-(kK*oP);var si=lZ*gU;var sj=kK*hQ;l1.clearRect(0,0,gU,hQ);l1.drawImage(
eL,si,sj,gU,hQ,0,0,gU,hQ);a.bindTexture(a.TEXTURE_2D,en);a.fD=null;if(ct.aJ.Format===
aj.ch)a.texSubImage2D(a.TEXTURE_2D,0,aY,aG,a.RGBA,a.UNSIGNED_BYTE,aF);else a.texSubImage2D(
a.TEXTURE_2D,0,aY,aG,a.ALPHA,a.UNSIGNED_BYTE,aF);}}aj.qw(oa,0);aj._RequestUpdate(
);}catch(bB){console.log("EmWi error: "+bB+"!");}}function pZ(aj,bS){try{var eL=
bS.Image;var height=eL.height;var bitmap=aj.nc(aj.ch,[eL.width,eL.height],0,1);bS.
cH=bitmap.cH;bS.b2=bitmap.b2;bS.FrameSize=bitmap.FrameSize;bS.fc.FrameSize=bitmap.
FrameSize;mY(aj,bS);}catch(bB){console.log("EmWi error: "+bB+"!");}}d.tp=function(
c4,lE){var mo=this;var bitmap=this.nc(c4.Format,c4.FrameSize,c4.FrameDelay,c4.NoOfFrames
);if(!bitmap)return;bitmap.p7=bitmap.NoOfFrames;bitmap.fc=lE;bitmap.Image=new Image(
);bitmap.Image.addEventListener('load',function(){mY(mo,bitmap);});bitmap.Image.
src=c4.FileName;if(c4.FrameMapping){bitmap.bK=c4.FrameMapping;bitmap.NoOfFrames=
bitmap.bK.length;}return bitmap;};d.sW=function(bS){var a=this.bj;if(!bS)return;
for(var h=0;bS.cH&&(h<bS.cH.length);h++){var aw=bS.cH[h];if(aw.cL)a.deleteTexture(
aw.cL);if(aw.Framebuffer)a.deleteFramebuffer(aw.Framebuffer);}bS.cH=null;bS.b2=null;
bS.NoOfFrames=0;};d.tq=function(q5,lE){var mo=this;var bitmap={cH:[],b2:[]};var aw={
};var ct={};aw.Format=this.ch;aw.Width=1;aw.Height=1;aw.ig=1;aw.ie=1;aw.d_=1;aw.
d9=1;aw.eT=2;aw.eS=2;aw.cL=null;aw.Framebuffer=null;ct.aJ=aw;ct.aM=[0,0];bitmap.
cH[0]=aw;bitmap.b2[0]=ct;bitmap.FrameSize=[1,1];bitmap.FrameDelay=0;bitmap.NoOfFrames=
1;bitmap.fc=lE;bitmap.Image=new Image();bitmap.Image.addEventListener('load',function(
){pZ(mo,bitmap);});bitmap.Image.src=q5;return bitmap;};d.tr=function(c4){var e=c4.
Data();var ag={Cache:c4.Cache,Ascent:e[0],Descent:e[1],Leading:e[2],fB:e[5],et:e[
4],fy:e[6],jx:e[7],mW:e[8],pS:e[9],hd:null};if(e[3]===2)ag.kZ=ps;if(e[3]===4)ag.
kZ=pt;if(e[3]===16)ag.kZ=pu;return ag;};d.sX=function(ac){};d.nh=function(ac,a5){
if(!ac){if(this.a8)console.log("EmWi error: Invalid font resource.");return null;
}var z=ac.Cache[a5];if(z){la(this,z);mD(this,z);return z;}if(!(z=this.dT(ac,a5))
)return null;var ej=this.qr;var kL=((z.Height+ej-1)/ej)|0;var kn=((z.Width+ej-1)
/ej)|0;var ax=null;var gk=this.jK;var l2=null;do{if(!(ax=l2=pA(this,kn,kL))){while(
gk&&gk.jE)gk=gk.fa;if(gk){var z=gk;var kK=(z.jo/ej)|0;var lZ=(z.jn/ej)|0;var kL=((
z.Height+ej-1)/ej)|0;var kn=((z.Width+ej-1)/ej)|0;if(this.a8)console.log("EmWi info: DiscardGlyph( "+
z.d6+", "+z.Width+", "+z.Height+" )");l2=z;gk=gk.fa;la(this,z);p8(this,lZ,kK,kn,
kL);}}}while(!ax&&l2);if(!ax)return null;ac.hd=null;var kz=1/this.jL;z.Cache=ac.
Cache;z.jn=ax[0]*ej;z.jo=ax[1]*ej;z.mE=z.jn*kz;z.mG=z.jo*kz;z.mF=(z.jn+z.Width)*
kz;z.mH=(z.jo+z.Height)*kz;z.jE=0;if((z.Width>0)&&(z.Height>0)){var o5=z.Width*z.
Height;var mj=new Uint8Array(o5);var a=this.bj;ac.kZ(ac.jx,mj,z.jx,z.mX);qh(mj,o5
,z.Width);a.bindTexture(a.TEXTURE_2D,this.ij);a.texSubImage2D(a.TEXTURE_2D,0,z.jn
,z.jo,z.Width,z.Height,a.ALPHA,a.UNSIGNED_BYTE,mj);a.fD=null;}p9(this,ax[0],ax[1
],kn,kL);mD(this,z);if(this.a8)console.log("EmWi info: CreateGlyph( "+z.d6+", "+
z.Width+", "+z.Height+" )");return z;};d.eV=function(ac,a5){if(!ac){if(this.a8)console.
log("EmWi error: Invalid font resource.");return false;}if((a5===0x061C)||(a5===
0xFEFF)||((a5>=0x200B)&&(a5<=0x200F))||((a5>=0x202A)&&(a5<=0x202E))||((a5>=0x2066
)&&(a5<=0x2069))||(a5===0xA))return false;if(ac.Cache[a5])return true;return d7(
a5,ac.fy,ac.fB)>=0;};d.dT=function(ac,a5){if(!ac){if(this.a8)console.log("EmWi error: Invalid font resource."
);return null;}if((a5===0x061C)||((a5>=0x200B)&&(a5<=0x200F))||((a5>=0x202A)&&(a5<=
0x202E))||((a5>=0x2066)&&(a5<=0x2069))||(a5===0xA))a5=0xFEFF;var z=ac.Cache[a5];
if(z)return z;if(!ac.hd||(ac.hd.d6!==a5)){var gY=a5;var aR=ac.fy;var d2=ac.fB;var
ax=-1;if(a5!==0xFEFF)while((ax=d7(gY,aR,d2))<0){if(gY===0xAD)gY=0x002D;else if(gY===
0xA0)return this.dT(ac,0x20);else if(gY===0x0020)gY=ac.et;else if(gY!==ac.et)return this.
dT(ac,ac.et);else break;}if(ax>=0){var sv=aR.charCodeAt(ax+6);var sw=aR.charCodeAt(
ax+7);var st=aR.charCodeAt(ax+14);var su=aR.charCodeAt(ax+15);var aQ=sw|(sv<<16);
var cT=su|(st<<16);ac.hd={d6:a5,OriginX:aR.charCodeAt(ax+1)-32768,OriginY:aR.charCodeAt(
ax+2)-32768,Width:aR.charCodeAt(ax+3),Height:aR.charCodeAt(ax+4),Advance:aR.charCodeAt(
ax+5)-32768,jx:aQ,mX:cT-aQ};}else ac.hd={d6:a5,OriginX:0,OriginY:0,Width:0,Height:
0,Advance:0,jx:0,mX:0};}return ac.hd;};d.bx=function(ac,nr,ns){if(!ac){if(this.a8
)console.log("EmWi error: Invalid font resource.");return 0;}if(!ac.mW||(!nr&&!ns
))return 0;var c=ac.mW;var sp=ac.pS;var r7=nr|(ns<<16);var s=0;var min=0;var max=(
c.length>>1)-1;var dg=-1;while(max>=min){s=(max+min)>>1;dg=r7-(c.charCodeAt(s*2)|(
c.charCodeAt(s*2+1)<<16));if(!dg)return sp.charCodeAt(s)-128;if(dg<0)max=s-1;else
min=s+1;}return 0;};d.jJ=function(ac,m,ak,al){var l=0;var aO=0;var bl=m.length;if(
!ac){if(this.a8)console.log("EmWi error: Invalid font resource.");return 0;}if(al===-
1)al=bl;if(ak<0){al+=ak;ak=0;}var aR=ac.fy;var d2=ac.fB;for(;(al>0)&&(ak<bl);ak++
,al--){var f=m.charCodeAt(ak);var aT=1;if((f===0x061C)||(f===0xFEFF)||((f>=0x200B
)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E))||((f>=0x2066)&&(f<=0x2069))||(f===0xA
))continue;if((f===0x2026)&&!ac.Cache[f]&&(d7(f,aR,d2)<0)){f=0x2E;aT=3;}for(;;){
var z=ac.Cache[f];if(z&&!z.Advance&&!z.OriginX&&!z.Width)break;if(z){l+=aT*z.Advance+
this.bx(ac,aO,f)+((aT>1)?this.bx(ac,f,f)*(aT-1):0);aO=f;break;}var ax=d7(f,aR,d2
);var fT;if((ax>=0)&&!(fT=(aR.charCodeAt(ax+5)-32768))&&(aR.charCodeAt(ax+1)===32768
)&&(aR.charCodeAt(ax+3)===32768))break;if(ax>=0){l+=aT*fT+this.bx(ac,aO,f)+((aT>
1)?this.bx(ac,f,f)*(aT-1):0);aO=f;break;}if(f===ac.et)break;if(f===0xAD)f=0x2D;else
if(f===0xA0)f=0x20;else f=ac.et;}}return l;};d.te=function(ac,m,ak,al){var e1=[0
,0,0,0];var l=0;var aO=0;var bl=m.length;if(!ac){if(this.a8)console.log("EmWi error: Invalid font resource."
);return this.qy;}if(al===-1)al=bl;if(ak<0){al+=ak;ak=0;}var aR=ac.fy;var d2=ac.
fB;for(;(al>0)&&(ak<bl);ak++,al--){var k,j,r,p,cj;var f=m.charCodeAt(ak);var aT=
1;if((f===0x061C)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E
))||((f>=0x2066)&&(f<=0x2069))||(f===0xA))continue;if((f===0x2026)&&!ac.Cache[f]&&(
d7(f,aR,d2)<0)){f=0x2E;aT=3;}for(;;){var z=ac.Cache[f];if(z){k=z.OriginX;j=z.OriginY;
r=z.Width+k;p=z.Height+j;cj=z.Advance;break;}var ax=d7(f,aR,d2);if(ax>=0){k=aR.charCodeAt(
ax+1)-32768;j=aR.charCodeAt(ax+2)-32768;r=aR.charCodeAt(ax+3)+k;p=aR.charCodeAt(
ax+4)+j;cj=aR.charCodeAt(ax+5)-32768;break;}if(f===ac.et){k=0;j=0;r=0;p=0;cj=0;break;
}if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=ac.et;}if(!k&&!r&&!cj)continue;
if(!l)e1[0]=k;if(j<e1[1])e1[1]=j;if(p>e1[3])e1[3]=p;var l9=this.bx(ac,aO,f);var hS=(
aT>1)?this.bx(ac,f,f):0;k+=l9;r+=l9+(aT-1)*(cj+hS);cj=cj*aT+l9+(aT-1)*hS;if((cj||
k||r)&&((l+r)>e1[2]))e1[2]=l+r;l+=cj;if(!r&&cj&&(l>e1[2]))e1[2]=l;aO=f;}return e1;
};d.td=function(ac,m,ak,al,a2,dJ){var dE=0;var dm=0x8000;var of=0;var bl=m.length;
var aO=0;if(!ac){if(this.a8)console.log("EmWi error: Invalid font resource.");return 0;
}if(al===-1)al=bl;if(ak<0){al+=ak;ak=0;}var aR=ac.fy;var d2=ac.fB;if(a2<0)return-
1;if(dJ>0)dJ-=this.jJ(ac,m,ak,al);if(dJ>0){var bN=ak;var ai=al;var eM=0;for(;ai;
bN++,ai--){var f=m.charCodeAt(bN);if((f===32)||(f===0xA0))eM++;}if(eM)dE=((dJ<<16
)/eM)|0;}for(;(al>0)&&(ak<bl);ak++,al--,of++){var f=m.charCodeAt(ak);var aT=1;var
cj;if((f===0x061C)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A)&&(f<=
0x202E))||((f>=0x2066)&&(f<=0x2069))||(f===0xA))continue;if((f===0x2026)&&!ac.Cache[
f]&&(d7(f,aR,d2)<0)){f=0x2E;aT=3;}for(;;){var z=ac.Cache[f];if(z&&!z.Advance&&!z.
OriginX&&!z.Width)break;if(z){cj=aT*z.Advance+this.bx(ac,aO,f)+((aT>1)?this.bx(ac
,f,f)*(aT-1):0);aO=f;break;}var ax=d7(f,aR,d2);var fT;if((ax>=0)&&!(fT=(aR.charCodeAt(
ax+5)===32768))&&(aR.charCodeAt(ax+1)===32768)&&(aR.charCodeAt(ax+3)===32768))break;
if(ax>=0){cj=aT*fT+this.bx(ac,aO,f)+((aT>1)?this.bx(ac,f,f)*(aT-1):0);aO=f;break;
}if(f===ac.et){cj=0;break;}if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=ac.
et;}if(dE&&((f===0x20)||(f===0xA0))){dm+=dE;cj+=dm>>16;dm&=0xFFFF;}if(a2<((cj>>1
)+1))break;a2-=cj;}return of;};d.s8=function(ac,m,ak,al,ff,dJ){var dE=0;var dm=0x8000;
var l=0;var bl=m.length;var aO=0;if(!ac){if(this.a8)console.log("EmWi error: Invalid font resource."
);return 0;}if(al===-1)al=bl;if(ak<0){al+=ak;ak=0;}var aR=ac.fy;var d2=ac.fB;if(
ff<0)return-1;if(dJ>0)dJ-=this.jJ(ac,m,ak,al);if(dJ>0){var bN=ak;var ai=al;var eM=
0;for(;ai;bN++,ai--){var f=m.charCodeAt(bN);if((f===32)||(f===0xA0))eM++;}if(eM)
dE=((dJ<<16)/eM)|0;}for(;(al>0)&&(ak<bl)&&ff;ak++,al--,ff--){var f=m.charCodeAt(
ak);var aT=1;var cj;if((f===0x061C)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((
f>=0x202A)&&(f<=0x202E))||((f>=0x2066)&&(f<=0x2069))||(f===0xA))continue;if((f===
0x2026)&&!ac.Cache[f]&&(d7(f,aR,d2)<0)){f=0x2E;aT=3;}if(dE&&((f===0x20)||(f===0xA0
))){dm+=dE;l+=dm>>16;dm&=0xFFFF;}for(;;){var z=ac.Cache[f];if(z&&!z.Advance&&!z.
OriginX&&!z.Width)break;if(z){cj=aT*z.Advance+this.bx(ac,aO,f)+((aT>1)?this.bx(ac
,f,f)*(aT-1):0);aO=f;break;}var ax=d7(f,aR,d2);var fT;if((ax>=0)&&!(fT=(aR.charCodeAt(
ax+5)===32768))&&(aR.charCodeAt(ax+1)===32768)&&(aR.charCodeAt(ax+3)===32768))break;
if(ax>=0){cj=aT*fT+this.bx(ac,aO,f)+((aT>1)?this.bx(ac,f,f)*(aT-1):0);aO=f;break;
}if(f===ac.et){cj=0;break;}if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=ac.
et;}l+=cj;}return l;};d.tM=function(ac,m,ak,aD,nE,an){var oH=(aD<0)?0x7FFFFFFE:aD;
var oG=(nE<0)?0x7FFFFFFF:nE;var i5=Math.max(ak,0);var h5=Math.max(m.length-i5,0);
var l3=h5*2+1;var ae=new Uint16Array(l3+1);var src=ae;var eD=1;var cl=eD+1;var h4=
l3-h5;var bO=h4;var bX=0;var dR=false;var aO=0;var cV=-1;var width=0;var ai=0;var
f=0;var s=0;if(!ac||!h5||!oG)return"";for(s=0;s<h5;s++)ae[s+bO]=m.charCodeAt(s+i5
);ae[l3]=0;for(s=0;s<h5;s++)switch(src[s+bO]){case 0x25:src[bO+s++]=0xFEFF;break;
case 0x5E:src[bO+s]=0x200B;break;case 0x7E:src[bO+s]=0x00AD;break;}if(an&&!this.
m_(an,src,bO,h5,-1))an=null;if(an){this.na(an,src,bO);this.m$(an,src,bO);this.m9(
an,src,bO,null,[ac]);}for(s=0;f=src[bO];){if(f!==0xA){var z;var fk=0;var rJ=bO;if((
f===0x061C)||((f>=0x200C)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E))||((f>=0x2066
)&&(f<=0x2069)))f=0xFEFF;if(dR&&(f!==0xFEFF)&&(f!==0x200B)&&(f!==0x00AD)&&(f!==0x20
))dR=false;if((f===0x200B)&&width&&!dR){if(bX&&(ae[bX]===0xAD))ae[bX]=0xFEFF;bX=
cl;dR=true;}if((f===0x20)&&width){if(bX&&(ae[bX]===0xAD))ae[bX]=0xFEFF;bX=cl;dR=
true;}if((f===0xAD)&&(!width||dR))f=0xFEFF;if(f===0x00AD){var h3=0;if((z=this.dT(
ac,0x00AD))!==null){var ap=an?this.hl(an,bO-h4):0;h3=z.Advance;if(ap===cV)if(ap&
1)h3+=this.bx(ac,f,aO);else h3+=this.bx(ac,aO,f);}if(((width+h3)<=oH)&&h3||!bX){
if(bX&&(ae[bX]===0xAD))ae[bX]=0xFEFF;width-=h3;bX=cl;dR=true;}}if(f===0x200B)f=0xFEFF;
if((f===0x2026)&&!this.eV(ac,f)&&(z=this.dT(ac,0x2E))){var ap=an?this.hl(an,bO-h4
):0;fk=z.Advance*3;fk+=this.bx(ac,0x2E,0x2E)*2;if(ap===cV)if(ap&1)fk+=this.bx(ac
,0x2E,aO);else fk+=this.bx(ac,aO,0x2E);aO=0x2E;cV=ap;}else if((f!==0xFEFF)&&(z=this.
dT(ac,f))){var ap=an?this.hl(an,bO-h4):0;fk=z.Advance;if(ap===cV)if(ap&1)fk+=this.
bx(ac,f,aO);else fk+=this.bx(ac,aO,f);aO=f;cV=ap;}bO++;if(((fk+width)<=oH)||!width
){width+=fk;ae[cl++]=f;}else if(bX){if(ae[bX]===0x20)ae[bX]=0xFEFF;for(bO-=2,cl--;
cl>bX;bO--,cl--)src[bO]=ae[cl];bX=0;f=0xA;bO++;cl++;}else{bO=rJ;f=0xA;}if(f===0xA
){if(src[bO]===0x20){ae[cl++]=0xFEFF;bO++;}while(((f=src[bO])===0xAD)||(f===0x061C
)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E))||((f>=0x2066
)&&(f<=0x2069))){ae[cl++]=0xFEFF;bO++;}f=0xA;}}else{bO++;ae[cl++]=f;}if((f===0xA
)&&src[bO]){if(--oG<=0)break;if(bX&&(ae[bX]===0xAD))ae[bX]=0xFEFF;if(an){var eH=
bO-h4;var fu=eH-(cl-eD)+1;this.ld(an,fu,eH);this.ih(an,fu,eH,ae,eD+1);}ae[eD]=cl-
eD;ae[cl]=0;eD=cl;cl+=1;bX=0;dR=false;width=0;aO=0;cV=-1;ai++;}}if(bX&&(ae[bX]===
0xAD))ae[bX]=0xFEFF;if((cl-eD)>1){if(an){var eH=bO-h4;var fu=eH-(cl-eD)+1;this.ld(
an,fu,eH);this.ih(an,fu,eH,ae,eD+1);}ae[eD]=cl-eD;ae[cl]=0;ai++;}ae[0]=ai;var o0=
"";for(s=0;s<cl;s++)o0+=String.fromCharCode(ae[s]);return o0;};d.s2=function(ac,
lI){var bl=lI.length;var s=1;var dq=0;if(!bl)return 0;if(!ac){if(this.a8)console.
log("EmWi error: Invalid font resource.");return 0;}while(s<bl){var i5=lI.charCodeAt(
s);var o3=i5-1;var o2=s+1;var mk;if((o2+o3)>bl)return 0;mk=this.jJ(ac,lI,o2,o3);
if(mk>dq)dq=mk;s+=i5;}return dq;};d.sJ=function(iD,iB,iC){if(iD<0)iD=0;if(iB<0)iB=
0;if(iC<0)iC=0;if(iD>256)iD=256;if(iB>256)iB=256;if(iC>256)iC=256;return{eU:iD,gp:
iB,ic:iC,d8:[],fx:[],hb:[]};};d.sU=function(am){if(am){delete am.d8;delete am.fx;
delete am.hb;}};d.tU=function(am,lG,cM){if(!am||(lG<0))return;if(lG<am.ic)am.hb[
lG]=cM;};d.tT=function(am,lw,bS){if(!am||(lw<0))return;if(lw<am.gp)am.fx[lw]=bS;
};d.tV=function(am,lJ,ac){if(!am||(lJ<0))return;if(lJ<am.eU)am.d8[lJ]=ac;};function
fb(m,i,as,dA){var g=0;var at=m.charCodeAt(i);while((at>=0x30)&&(at<=0x39)){g=(g*
10)+(at-0x30);at=m.charCodeAt(++i);}as[dA]=g;return i;}function k8(m,i,q_,as,dA){
i=fb(m,i,as,dA);if(m.charCodeAt(i)===0x25){var gf=as[dA]*q_;as[dA]=(gf/100)|0;if((
gf-(as[dA]*100))>50)as[dA]++;i++;}return i;}function m2(m,i,aD,as){as[0]=0;as[1]=
0;as[2]=0;as[3]=0;i=k8(m,i,aD,as,0);if(m.charCodeAt(i)===0x2C){i=k8(m,i+1,aD,as,
1);if(m.charCodeAt(i)===0x2C){i=fb(m,i+1,as,2);if(m.charCodeAt(i)===0x2C)i=fb(m,
i+1,as,3);}}if(as[0]>aD)as[0]=aD;if(as[1]>(aD-as[0]))as[1]=aD-as[0];return i;}function
p2(m,i,as,dA){var g=0x6C;var b$=m.charCodeAt(i);var ei=b$?m.charCodeAt(i+1):0;if(((
b$===0x6C)||(b$===0x72)||(b$===0x63)||(b$===0x61))&&(ei===0x6A)){g=b$-0x20;i+=2;
}else if((b$===0x6A)&&((ei===0x6C)||(ei===0x72)||(ei===0x63)||(ei===0x61))){g=ei-
0x20;i+=2;}else if((b$===0x6C)||(b$===0x72)||(b$===0x63)||(b$===0x61)){g=b$;i++;
}else if(b$===0x6A){g=0x4C;i++;}as[dA]=g;return i;}function m1(m,i,as,dA){var g=
0x6D;var at=m.charCodeAt(i);if((at===0x74)||(at===0x6D)||(at===0x62)){g=at;i++;}
as[dA]=g;return i;}function p3(m,i,aD,gB){var g=[0,0,0];var at=m.charCodeAt(i);var
h=0;var ca=0;var ks=aD;var i0=0;var bR=[0];while((h<3)&&(((at>=0x30)&&(at<=0x39)
)||(at===0x2A))){if(at===0x2A){g[h]=-1;i0++;i++;}else{i=k8(m,i,aD,g,h);ks=ks-g[h
];}if((at=m.charCodeAt(i))===0x2C)at=m.charCodeAt(++i);h++;}if(!h){g[0]=aD;h=1;}
for(;ca<i0;ca++){var dq=(ks/i0)|0;if(ca===(i0-1))dq=ks-(i0-1)*dq;if(g[0]<0)g[0]=
dq;else if(g[1]<0)g[1]=dq;else if(g[2]<0)g[2]=dq;}for(h=0;h<3;h++){if(g[h]<0)g[h
]=0;if(g[h]>aD)g[h]=aD;aD=aD-g[h];gB[h]=g[h];}return i;}function c$(m,i,nB){return m.
slice(i,i+nB.length)===nB;}function pz(m,i){var f;for(;(f=m.charCodeAt(i));i++){
if((f===0x7B)&&(c$(m,i+1,"par")||c$(m,i+1,"lay")||c$(m,i+1,"col")||c$(m,i+1,"end"
)))break;if((f===0x25)&&(i<(m.length-1)))i++;}return i;}function p4(m,i,lH,iI,iH
,iJ,dI){var uh=i;var dl=0;var dL=0;var cB=0;var jj=false;var hU=false;var dD=[0,
0,0];while(i<lH){var f=m.charCodeAt(i);if(f===0x7B){if(c$(m,i,"{img")){i=m1(m,i+
4,dD,0);i=fb(m,i,dD,1);if(m.charCodeAt(i)===0x2E){i++;i=fb(m,i,dD,2);}if(m.charCodeAt(
i)===0x7D)i++;if(dD[2]>4095)dD[2]=0;if(dD[1]>255)dD[1]=0;iI[dl]=0xFEFF;iH[dl]=((
dD[0]===0x74)?5:(dD[0]===0x62)?4:3)|(dL<<4)|(dD[1]<<12)|(dD[2]<<20);iJ[dl++]=hU?
dI[0]:0;continue;}if(c$(m,i,"{fnt")){i=fb(m,i+4,dD,0);if(m.charCodeAt(i)===0x7D)
i++;cB=dD[0];if(cB>255)cB=0;continue;}if(c$(m,i,"{clr")){i=fb(m,i+4,dD,0);if(m.charCodeAt(
i)===0x7D)i++;dL=dD[0];if(dL>255)dL=0;continue;}if(c$(m,i,"{ul+}")){jj=true;i+=5;
continue;}if(c$(m,i,"{ul-}")){jj=false;i+=5;continue;}if(c$(m,i,"{lnk:")){i+=5;hU=
true;dI[0]+=1;while((i<lH)&&((f=m.charCodeAt(i))!==0x7D)&&(f!==0x7B))i++;if(m.charCodeAt(
i)===0x7D)i++;continue;}if(c$(m,i,"{lnk}")){i+=5;hU=false;continue;}}if(f===0x5E
){iI[dl]=0x200B;iH[dl]=(jj?2:1)|(dL<<4)|(cB<<12);iJ[dl++]=hU?dI[0]:0;i++;continue;
}if(f===0x7E){iI[dl]=0xAD;iH[dl]=(jj?2:1)|(dL<<4)|(cB<<12);iJ[dl++]=hU?dI[0]:0;i++;
continue;}if(f===0x25){f=m.charCodeAt(++i);if(i===lH)break;}iI[dl]=f;iH[dl]=(jj?
2:1)|(dL<<4)|(cB<<12);iJ[dl++]=hU?dI[0]:0;i++;}iI[dl]=0;iH[dl]=0;iJ[dl]=0;return dl;
}function mC(aj,am,m,i,aD,fh,aE,ed,q1,fN,dI){var x=[0];var cK=aE.length;aE.push(
0x47,0,0,aD,0,0,0);i=m0(aj,am,m,i,aD,0x6C,fh,aE,x,fN,dI);if(aE.length>(cK+7)){aE[
cK+4]=x[0];aE[cK+5]=aE.length-cK;}else aE.splice(cK,aE.length-cK);while(i<m.length
){if(c$(m,i,"{par")){var cQ=[0,0,0,0];var fj=[-1];var nP=[];var width;var height=[
0];i=p2(m,i+4,nP,0);i=m2(m,i,aD,cQ);if(m.charCodeAt(i)===0x3A)i=fb(m,i+1,fj,0);if(
m.charCodeAt(i)===0x7D)i++;if(fj[0]!==-1)aE.push(0x52,0,x[0],aD,0,fj[0]);cK=aE.length;
width=aD-cQ[0]-cQ[1];aE.push(0x47,0,x[0]+cQ[2],aD,0,0,cQ[0]);i=m0(aj,am,m,i,width
,nP[0],fh,aE,height,fN,dI);x[0]+=cQ[2]+height[0]+cQ[3];aE[cK+4]=x[0]-cQ[3];aE[cK+
5]=aE.length-cK;if(fj[0]!==-1)aE[cK-2]=x[0];}else if(c$(m,i,"{lay")){var og=[];var
height=[0];cK=aE.length;i=p3(m,i+4,aD,og);if(m.charCodeAt(i)===0x7D)i++;aE.push(
0x47,0,x[0],aD,0,0,0);i=pT(aj,am,m,i,fh,og,aE,height,fN,dI);x[0]+=height[0];aE[cK+
4]=x[0];aE[cK+5]=aE.length-cK;}else break;}ed[q1]=x[0];return i;}function pT(aj,
am,m,i,fh,gB,aE,ed,fN,dI){var nQ=[];var iU=[];var iT=[];var fj=[-1,-1,-1];var h=
0;var ca=0;var l=0;var height=0;while((i<m.length)&&(h<gB.length)){if(c$(m,i,"{col"
)){var cQ=[0,0,0,0];var width;i=m1(m,i+4,nQ,h);i=m2(m,i,gB[h],cQ);if(m.charCodeAt(
i)===0x3A)i=fb(m,i+1,fj,h);if(m.charCodeAt(i)===0x7D)i++;if(fj[h]!==-1)aE.push(0x52
,l,0,l+gB[h],0,fj[h]);iU[h]=aE.length;width=gB[h]-cQ[0]-cQ[1];aE.push(0x47,l+cQ[
0],cQ[2],l+cQ[0]+width,0,0,0);i=mC(aj,am,m,i,width,fh,aE,iT,h,fN,dI);iT[h]+=cQ[2
]+cQ[3];height=Math.max(height,iT[h]);aE[iU[h]+4]=iT[h]-cQ[3];aE[iU[h]+5]=aE.length-
iU[h];l+=gB[h];h++;}else break;}for(ca=0;ca<h;ca++){var cK=iU[ca];var kG=height-
iT[ca];switch(nQ[ca]){case 0x74:kG=0;break;case 0x6D:kG>>=1;break;}aE[cK+2]+=kG;
aE[cK+4]+=kG;if(fj[ca]!==-1)aE[cK-2]=height;}ed[0]=height;if(c$(m,i,"{end}"))i+=
5;return i;}function p6(aj,am,eb,ez,c3,nF,an,as){var fu=c3;var ag=null;var cB=-1;
var c_=0;var mz=0;var pf=0;var my=0;var mv=0;var mw=0;var mx=0;var dR=false;var fn=
0;var aO=0;var cV=-1;as[0]=0;as[1]=0;as[2]=0;as[3]=0;as[4]=0;for(;eb[c3];c3++){var
e0=ez[c3];var ad=eb[c3];var type=e0&0xF;var c6=0;var ol=0;var oo=0;var iV=0;var iW=
0;var iX=0;if(((type===1)||(type===2))&&(((e0>>12)&0xFF)!==cB)){cB=(e0>>12)&0xFF;
ag=null;if(am&&(cB>=0)&&(cB<am.eU))ag=am.d8[cB];if(!ag){cB=0;ag=(am&&(am.eU>0))?
am.d8[0]:null;}cV=-1;aO=0;}if((ad===0xA)&&ag){as[3]=Math.max(as[3],ag.Ascent);as[
4]=Math.max(as[4],ag.Leading);fn=Math.max(fn,ag.Descent);}if(ad===0xA)break;if((
type>=3)&&(type<=5)){var fW=(e0>>12)&0xFF;var bH=null;if(am&&(fW>=0)&&(fW<am.gp)
)bH=am.fx[fW];if(!bH&&am&&(am.gp>0))bH=am.fx[0];if(bH){c6=bH.FrameSize[0];ol=bH.
FrameSize[1];}cV=-1;aO=0;}else{var z;if((ad===0x200B)&&(!as[0]||dR))ad=0xFEFF;if((
ad===0xAD)&&(!as[0]||dR))ad=0xFEFF;if((ad===0x20)&&!as[0])ad=0xA0;if((ad===0x20)||(
ad===0x200B)){if(c_&&(eb[c_]===0xAD))eb[c_]=0xFEFF;c_=c3;mz=as[0];pf=as[1];my=as[
2];mv=as[3];mx=as[4];mw=fn;dR=true;}if(ad===0xAD){if(ag&&as[0]&&(z=aj.dT(ag,0xAD
))){var ap=an?aj.hl(an,c3):0;c6=z.Advance;iV=ag.Ascent;iW=ag.Descent;iX=ag.Leading;
if(ap===cV)if(ap&1)c6+=aj.bx(ag,ad,aO);else c6+=aj.bx(ag,aO,ad);}if(((as[0]+c6)<=
nF)&&c6){if(c_&&(eb[c_]===0xAD))eb[c_]=0xFEFF;c_=c3;mz=as[0]+c6;mv=Math.max(as[3
],iV);mx=Math.max(as[4],iX);mw=Math.max(fn,iW);my=as[2];dR=true;}continue;}if((ad===
0x061C)||((ad>=0x200B)&&(ad<=0x200F))||((ad>=0x202A)&&(ad<=0x202E))||((ad>=0x2066
)&&(ad<=0x2069)))ad=0xFEFF;if(ad===0xFEFF){eb[c3]=0xFEFF;continue;}if(c_!==c3)dR=
false;if(ag&&(ad===0x2026)&&!ag.Cache[ad]&&(d7(ad,ag.fy,ag.fB)<0)&&(z=aj.dT(ag,0x2E
))){var ap=an?aj.hl(an,c3):0;c6=z.Advance*3;c6+=aj.bx(ag,0x2E,0x2E)*2;iV=ag.Ascent;
iW=ag.Descent;iX=ag.Leading;if(ap===cV)if(ap&1)c6+=aj.bx(ag,0x2E,aO);else c6+=aj.
bx(ag,aO,0x2E);aO=0x2E;cV=ap;}else if(ag&&(z=aj.dT(ag,ad))){var ap=an?aj.hl(an,c3
):0;c6=z.Advance;iV=ag.Ascent;iW=ag.Descent;iX=ag.Leading;if((ad===0x20)||(ad===
0xA0))oo=c6;if(ap===cV)if(ap&1)c6+=aj.bx(ag,ad,aO);else c6+=aj.bx(ag,aO,ad);aO=ad;
cV=ap;}}if(((c6+as[0])<=nF)||!as[0]){as[0]+=c6;as[1]=Math.max(as[1],ol);as[3]=Math.
max(as[3],iV);as[4]=Math.max(as[4],iX);fn=Math.max(fn,iW);as[2]+=oo;continue;}if(
c_){as[0]=mz;as[1]=pf;as[2]=my;as[3]=mv;as[4]=mx;fn=mw;c3=c_+1;if(eb[c_]===0x20)
eb[c_]=0xFEFF;while((((type=(ez[c3]&0xF))===1)||(type===2))&&(((ad=eb[c3])===0xFEFF
)||(ad===0x200B)||(ad===0xAD)))c3++;c_=0;}break;}if(c_&&(eb[c_]===0xAD))eb[c_]=0xFEFF;
as[1]=Math.max(as[1],as[3]+fn);as[3]+=(as[1]-as[3]-fn)>>1;return c3-fu;}function
m0(aj,am,m,i,aD,qS,fh,aE,ed,fN,dI){var eH=pz(m,i);var ag=(am&&(am.eU>0))?am.d8[0
]:null;var bl=eH-i+1;var eC=new Uint16Array(bl);var fV=new Uint32Array(bl);var ma=
new Uint16Array(bl);var cP=null;var gW=false;var s=0;var cB=0;var dL=0;var fv=0;
var fs=0;var oC=[0];var dk=[0,0,0,0,0];bl=p4(m,i,eH,eC,fV,ma,oC);if(bl&&fh)cP=aj.
qk(bl);if(cP&&!aj.m_(cP,eC,0,bl,-1)){aj.ng(cP);cP=null;}if(cP){aj.na(cP,eC,0);aj.
m$(cP,eC,0);aj.m9(cP,eC,0,fV,am.d8);gW=aj.qj(cP);if(fN[0]<0)fN[0]=gW?1:0;}ed[0]=
0;while(s<bl){var el=qS;var l=0;var oE=0;var gh=0;var kN=0;var o7=0;var aO=0;var
ai=p6(aj,am,eC,fV,s,aD,cP,dk);if(cP){aj.ld(cP,s,s+ai);aj.ih(cP,s,s+ai,eC,s);aj.ih(
cP,s,s+ai,fV,s);aj.ih(cP,s,s+ai,ma,s);}if(el===0x61)el=gW?0x72:0x6C;if(el===0x41
)el=gW?0x52:0x4C;if((el===0x4C)||(el===0x52)||(el===0x43))if((!eC[s+ai]||(eC[s+ai
]===0xA)||!dk[2])){el+=0x20;dk[2]=0;}else el=0x4C;if(el===0x72)l+=aD-dk[0];else if(
el===0x63)l+=(aD-dk[0])>>1;for(;ai>=0;ai--,s++){var ad=ai?eC[s]:0;var e0=ai?fV[s
]:0;var type=e0&0xF;var md=dL;var gd=cB;var kE=fv;var kD=ai?ma[s]:0;var z;if((type>=
1)&&(type<=5))md=(e0>>4)&0xFF;if((type===1)||(type===2))gd=(e0>>12)&0xFF;if(type===
1)kE=0;if(type===2)kE=1;if(gh&&(((type!==1)&&(type!==2))||(dL!==md)||(cB!==gd)||(
fv!==kE)||(fs!==kD))){if(kN&&(el===0x4C)){var a0=(kN*(aD-dk[0]))+o7;var be=(a0/dk[
2])|0;o7=a0-(be*dk[2]);l+=be;aE[gh]=0x4A;aE.push(l-aE[gh+1]);}if(fv){if(ag){var z;
if(z=aj.dT(ag,0x2D))fv=z.Height;}if(fv<=0)fv=1;aE.push(0x52,aE[gh+1],ed[0]+dk[3]+
fv,l,ed[0]+dk[3]+(fv*2),dL);}gh=0;kN=0;}if(cB!==gd){ag=null;if(am&&(gd>=0)&&(gd<
am.eU))ag=am.d8[gd];if(!ag)ag=(am&&(am.eU>0))?am.d8[0]:null;}if(fs&&(fs!==kD))aE.
push(0x41,fs+dI[0],oE,ed[0],l,ed[0]+dk[1],dk[3]);if(fs!==kD)oE=l;if(cB!==gd)aO=0;
dL=md;cB=gd;fv=kE;fs=kD;if((type>=3)&&(type<=5)){var fW=(e0>>12)&0xFF;var f_=(e0>>
20)&0xFFF;var bH=null;if(am&&(fW>=0)&&(fW<am.gp))bH=am.fx[fW];if(!bH&&am&&(am.gp>
0))bH=am.fx[0];if(bH&&(f_>=bH.NoOfFrames))f_=0;if(bH){var bN=dk[1]-bH.FrameSize[
1];switch(type){case 3:bN>>=1;break;case 5:bN=0;break;}aE.push(0x49,l,ed[0]+bN,fW
,f_,dL);l+=bH.FrameSize[0];}aO=0;}if(((type===1)||(type===2))&&ag&&(ad!==0xFEFF)
){var aT=1;if(ag&&(ad===0x2026)&&!ag.Cache[ad]&&(d7(ad,ag.fy,ag.fB)<0)&&(z=aj.dT(
ag,0x2E))){ad=0x2E;aT=3;}if(z=aj.dT(ag,ad)){var dj=aj.bx(ag,aO,ad);if(!gh){gh=aE.
length;aE.push(0x54,l+dj,ed[0]+dk[3],"",cB,dL);}while(aT--){aE[gh+3]+=String.fromCharCode(
ad);l+=z.Advance+dj;if((ad===0x20)||(ad===0xA0))kN+=z.Advance;if(aT)dj=aj.bx(ag,
ad,ad);}aO=ad;}}}if(eC[s-1]!==0xA)s--;ed[0]+=dk[1]+dk[4];}if(cP)aj.ng(cP);dI[0]+=
oC[0];return eH;}function pq(m,i,jV){var f;while((f=m.charCodeAt(i))){if((f===0x7B
)&&c$(m,i,"{lnk:")){var fu=i+=5;while(((f=m.charCodeAt(i)))&&(f!==0x7D)&&(f!==0x7B
))i++;jV.push({jq:0,jD:0xFFFFFFFE,jv:0,lb:0,jF:0,pW:m.slice(fu,i)});continue;}if((
f===0x25)&&((i+1)<m.length))i++;i++;}}function mK(jV,ee,ef,aV,aB,fK){while(aB<fK
){switch(aV[aB]){case 0x47:{var ai=aV[aB+5];var l=aV[aB+1]+aV[aB+6]+ee;var x=aV[
aB+2]+ef;mK(jV,l,x,aV,aB+7,aB+ai);aB+=ai;}break;case 0x54:aB+=6;break;case 0x4A:
aB+=7;break;case 0x49:aB+=6;break;case 0x52:aB+=6;break;case 0x41:{var cn=jV[aV[
aB+1]-1];cn.jD=Math.min(cn.jD,aB);cn.jq=Math.max(cn.jq,aB+7);cn.lb=ee;cn.jF=ef;cn.
jv++;aB+=7;}break;}}}function m4(cx,eY,aV,rd,nx){var t=rd;while(t<nx){switch(aV[
t]){case 0x54:t+=6;break;case 0x4A:t+=7;break;case 0x49:t+=6;break;case 0x52:t+=
6;break;case 0x41:{var fs=aV[t+1];if((fs===cx)&&!eY)return t;if(fs===cx)eY--;t+=
7;}break;default:t=nx;}}return-1;}function pv(aj,aa,ah,af,cw,ee,ef,am,aV,aB,fK,aA
,ci){function da(dW,a2,bu,cM){var hF=dW.pg;var nT=dW.pi;if(a2<0)a2=0;if(bu<0)bu=
0;if(a2>dW.Width)a2=dW.Width;if(bu>dW.Height)bu=dW.Height;if(dW.mV&&bu)hF+=bu*dW.
ph;if(dW.mV&&dW.mU&&bu)nT+=bu*dW.pj;if(dW.mU&&a2)hF+=(nT-hF)*dW.pQ*a2;if(hF>=255
)return cM;if(hF<=0)return cM&0xFFFFFF;var rG=((((cM>>24)&0xFF)*hF/255)<<24)|(cM&
0xFFFFFF);return rG;}function mO(aj,aa,ah,af,cw,ee,ef,am,aV,aB,fK,aA,ci){while(aB<
fK){var f=aV[aB];if((f===0x47)||(f===0x52)){var ay=[aV[aB+1]+ee,aV[aB+2]+ef,aV[aB+
3]+ee,aV[aB+4]+ef];switch(f){case 0x47:{var ai=aV[aB+5];if((af[0]<af[2])&&(af[1]<
af[3]))mO(aj,aa,ah,af,cw,ay[0]+aV[aB+6],ay[1],am,aV,aB+7,aB+ai,aA,ci);aB+=ai;}break;
case 0x52:{var bb=aV[aB+5];var k=ay[0]-cw[0];var r=ay[2]-cw[0];var j=ay[1]-cw[1];
var p=ay[3]-cw[1];if((bb>=0)&&(bb<am.ic))bb=am.hb[bb];else bb=0;var gQ=da(ci,k,j
,bb);var gR=da(ci,r,j,bb);var gP=da(ci,r,p,bb);var gO=da(ci,k,p,bb);aj.hn(aa,ah,
af,ay,gQ,gR,gP,gO,aA);aB+=6;}break;}}else if((f===0x54)||(f===0x49))aB+=6;else if(
f===0x4A)aB+=7;else if(f===0x41)aB+=7;}}function mP(aj,aa,ah,af,cw,ee,ef,am,aV,aB
,fK,aA,ci){while(aB<fK){var f=aV[aB];if((f===0x47)||(f===0x52)){var ay=[aV[aB+1]+
ee,aV[aB+2]+ef,aV[aB+3]+ee,aV[aB+4]+ef];switch(f){case 0x47:{var ai=aV[aB+5];if((
af[0]<af[2])&&(af[1]<af[3]))mP(aj,aa,ah,af,cw,ay[0]+aV[aB+6],ay[1],am,aV,aB+7,aB+
ai,aA,ci);aB+=ai;}break;case 0x52:{aB+=6;}break;}}else if((f===0x54)||(f===0x4A)||(
f===0x49)){var ax=[aV[aB+1]+ee,aV[aB+2]+ef];switch(f){case 0x54:{var ag=aV[aB+4];
var bb=aV[aB+5];var k=af[0]-cw[0];var j=af[1]-cw[1];var r=af[2]-cw[0];var p=af[3
]-cw[1];if((bb>=0)&&(bb<am.ic))bb=am.hb[bb];else bb=0;if((ag>=0)&&(ag<am.eU))ag=
am.d8[ag];else ag=null;if(!ag&&am.eU)ag=am.d8[0];ax[0]=af[0]-ax[0];ax[1]=af[1]-ax[
1];if(ag){var gQ=da(ci,k,j,bb);var gR=da(ci,r,j,bb);var gP=da(ci,r,p,bb);var gO=
da(ci,k,p,bb);aj.nf(aa,ag,aV[aB+3],0,-1,ah,af,af,ax,0,0,gQ,gR,gP,gO,aA);}aB+=6;}
break;case 0x4A:{var ag=aV[aB+4];var bb=aV[aB+5];var k=af[0]-cw[0];var j=af[1]-cw[
1];var r=af[2]-cw[0];var p=af[3]-cw[1];var width=aV[aB+6];if((bb>=0)&&(bb<am.ic)
)bb=am.hb[bb];else bb=0;if((ag>=0)&&(ag<am.eU))ag=am.d8[ag];else ag=null;if(!ag&&
am.eU)ag=am.d8[0];ax[0]=af[0]-ax[0];ax[1]=af[1]-ax[1];if(ag){var gQ=da(ci,k,j,bb
);var gR=da(ci,r,j,bb);var gP=da(ci,r,p,bb);var gO=da(ci,k,p,bb);aj.nf(aa,ag,aV[
aB+3],0,-1,ah,af,af,ax,width,0,gQ,gR,gP,gO,aA);}aB+=7;}break;case 0x49:{var bH=aV[
aB+3];var f_=aV[aB+4];var bb=aV[aB+5];if((bb>=0)&&(bb<am.ic))bb=am.hb[bb];else bb=
0;if((bH>=0)&&(bH<am.gp))bH=am.fx[bH];else bH=null;if(!bH&&am.gp)bH=am.fx[0];if(
bH){var k=ax[0]-cw[0];var j=ax[1]-cw[1];var r=k+bH.FrameSize[0];var p=j+bH.FrameSize[
1];var gQ=da(ci,k,j,bb);var gR=da(ci,r,j,bb);var gP=da(ci,r,p,bb);var gO=da(ci,k
,p,bb);var cz=[ax[0],ax[1],ax[0]+bH.FrameSize[0],ax[1]+bH.FrameSize[1]];aj.fF(aa
,bH,ah,f_,af,cz,[0,0],gQ,gR,gP,gO,aA);}aB+=6;}break;}}else if(f===0x41)aB+=7;}}mO(
aj,aa,ah,af,cw,ee,ef,am,aV,aB,fK,aA,ci);mP(aj,aa,ah,af,cw,ee,ef,am,aV,aB,fK,aA,ci
);}d.tL=function(am,m,aD,fh){var nV=[];var fU={};var height=[0];var oQ=[0];var gW=[-
1];if(!am||!m.length||(aD<=0))return null;mC(this,am,m,0,aD,fh,nV,height,0,gW,oQ
);fU.Data=nV;fU.ew=[];fU.pR=gW[0]===1;if(oQ){pq(m,0,fU.ew);mK(fU.ew,0,0,fU.Data,
0,fU.Data.length);}return fU;};d.sV=function(a4){};d.s1=function(a4){if(!a4)return this.
qx;if(!a4.fd){var e=a4?a4.Data:null;var t=0;var cT=e?e.length:0;var cv=[0,0,0,0];
while(t<cT){var k=e[t+1];var j=e[t+2];var r=e[t+3];var p=e[t+4];if((k<r)&&(j<p)){
if(cv[0]>k)cv[0]=k;if(cv[2]<r)cv[2]=r;if(cv[1]>j)cv[1]=j;if(cv[3]<p)cv[3]=p;}if(
e[t]===0x52)t+=6;else t+=e[t+5];}var dq=cv[2]-cv[0];var hR=cv[3]-cv[1];if(dq<0)dq=
0;if(hR<0)hR=0;a4.fd=[dq,hR];}return a4.fd;};d.s5=function(a4){if(!a4)return 0;return a4.
ew.length;};d.sZ=function(a4,cx){if(!a4)return this.hm;if((cx<0)||(cx>=a4.ew.length
))return this.hm;return a4.ew[cx].pW;};d.s4=function(a4,cx){if(!a4)return 0;if((
cx<0)||(cx>=a4.ew.length))return 0;return a4.ew[cx].jv;};d.s0=function(a4,cx,eY){
var cn;var t;var cv=[0,0,0,0];if(!a4)return cv;if((cx<0)||(cx>=a4.ew.length))return cv;
cn=a4.ew[cx];if((eY<0)||(eY>=cn.jv))return cv;t=m4(cx+1,eY,a4.Data,cn.jD,cn.jq);
if(t>=0){cv[0]=a4.Data[t+2]+cn.lb;cv[1]=a4.Data[t+3]+cn.jF;cv[2]=a4.Data[t+4]+cn.
lb;cv[3]=a4.Data[t+5]+cn.jF;}return cv;};d.sY=function(a4,cx,eY){var cn;var t;if(
!a4)return 0;if((cx<0)||(cx>=a4.ew.length))return 0;cn=a4.ew[cx];if((eY<0)||(eY>=
cn.jv))return 0;t=m4(cx+1,eY,a4.Data,cn.jD,cn.jq);if(t>=0)return cn.jF+a4.Data[t+
6];return 0;};d.tk=function(a4){return a4&&a4.pR;};var po="\u0000\u0009\u000A\u000B\u000C\u000D\u000E\u001C\u001F\u0020"+
"\u0021\u0023\u0026\u0028\u0029\u002A\u002B\u002C\u002D\u002E"+"\u0030\u003A\u003B\u003C\u003D\u003E\u003F\u0041\u005B\u005C"+
"\u005D\u005E\u0061\u007B\u007C\u007D\u007E\u007F\u0085\u0086"+"\u00A0\u00A1\u00A2\u00A6\u00AA\u00AB\u00AC\u00AD\u00AE\u00B0"+
"\u00B2\u00B4\u00B5\u00B6\u00B9\u00BA\u00BB\u00BC\u00C0\u00D7"+"\u00D8\u00F7\u00F8\u02B9\u02BB\u02C2\u02D0\u02D2\u02E0\u02E5"+
"\u02EE\u02EF\u0300\u0370\u0374\u0376\u037E\u037F\u0384\u0386"+"\u0387\u0388\u03F6\u03F7\u0483\u048A\u058A\u058B\u058D\u058F"+
"\u0590\u0591\u05BE\u05BF\u05C0\u05C1\u05C3\u05C4\u05C6\u05C7"+"\u05C8\u0600\u0606\u0608\u0609\u060B\u060C\u060D\u060E\u0610"+
"\u061B\u061C\u061D\u0620\u0621\u0622\u0626\u0627\u0628\u0629"+"\u062A\u062F\u0633\u0640\u0641\u0648\u0649\u064B\u0660\u066A"+
"\u066B\u066D\u066E\u0670\u0671\u0674\u0675\u0678\u0688\u069A"+"\u06C0\u06C1\u06C3\u06CC\u06CD\u06CE\u06CF\u06D0\u06D2\u06D4"+
"\u06D5\u06D6\u06DD\u06DE\u06DF\u06E5\u06E7\u06E9\u06EA\u06EE"+"\u06F0\u06FA\u06FD\u06FF\u0700\u070F\u0710\u0711\u0712\u0715"+
"\u071A\u071E\u071F\u0728\u0729\u072A\u072B\u072C\u072D\u072F"+"\u0730\u074B\u074D\u074E\u0759\u075C\u076B\u076D\u0771\u0772"+
"\u0773\u0775\u0778\u077A\u0780\u07A6\u07B1\u07C0\u07EB\u07F4"+"\u07F6\u07FA\u0816\u081A\u081B\u0824\u0825\u0828\u0829\u082E"+
"\u0859\u085C\u0860\u0861\u0862\u0866\u0867\u0868\u0869\u086B"+"\u0870\u08A0\u08AA\u08AD\u08AE\u08AF\u08B1\u08B3\u08B5\u08B6"+
"\u08B9\u08BA\u08BE\u08D4\u08E2\u08E3\u0903\u093A\u093B\u093C"+"\u093D\u0941\u0949\u094D\u094E\u0951\u0958\u0962\u0964\u0981"+
"\u0982\u09BC\u09BD\u09C1\u09C5\u09CD\u09CE\u09E2\u09E4\u09F2"+"\u09F4\u09FB\u09FC\u0A01\u0A03\u0A3C\u0A3D\u0A41\u0A43\u0A47"+
"\u0A49\u0A4B\u0A4E\u0A51\u0A52\u0A70\u0A72\u0A75\u0A76\u0A81"+"\u0A83\u0ABC\u0ABD\u0AC1\u0AC6\u0AC7\u0AC9\u0ACD\u0ACE\u0AE2"+
"\u0AE4\u0AF1\u0AF2\u0AFA\u0B00\u0B01\u0B02\u0B3C\u0B3D\u0B3F"+"\u0B40\u0B41\u0B45\u0B4D\u0B4E\u0B56\u0B57\u0B62\u0B64\u0B82"+
"\u0B83\u0BC0\u0BC1\u0BCD\u0BCE\u0BF3\u0BF9\u0BFA\u0BFB\u0C00"+"\u0C01\u0C3E\u0C41\u0C46\u0C49\u0C4A\u0C4E\u0C55\u0C57\u0C62"+
"\u0C64\u0C78\u0C7F\u0C81\u0C82\u0CBC\u0CBD\u0CBF\u0CC0\u0CC6"+"\u0CC7\u0CCC\u0CCE\u0CE2\u0CE4\u0D00\u0D02\u0D3B\u0D3D\u0D41"+
"\u0D45\u0D4D\u0D4E\u0D62\u0D64\u0DCA\u0DCB\u0DD2\u0DD5\u0DD6"+"\u0DD7\u0E31\u0E32\u0E34\u0E3B\u0E3F\u0E40\u0E47\u0E4F\u0EB1"+
"\u0EB2\u0EB4\u0EBA\u0EBB\u0EBD\u0EC8\u0ECE\u0F18\u0F1A\u0F35"+"\u0F36\u0F37\u0F38\u0F39\u0F3A\u0F3B\u0F3C\u0F3D\u0F3E\u0F71"+
"\u0F7F\u0F80\u0F85\u0F86\u0F88\u0F8D\u0F98\u0F99\u0FBD\u0FC6"+"\u0FC7\u102D\u1031\u1032\u1038\u1039\u103B\u103D\u103F\u1058"+
"\u105A\u105E\u1061\u1071\u1075\u1082\u1083\u1085\u1087\u108D"+"\u108E\u109D\u109E\u135D\u1360\u1390\u139A\u1400\u1401\u1680"+
"\u1681\u169B\u169C\u169D\u1712\u1715\u1732\u1735\u1752\u1754"+"\u1772\u1774\u17B4\u17B6\u17B7\u17BE\u17C6\u17C7\u17C9\u17D4"+
"\u17DB\u17DC\u17DD\u17DE\u17F0\u17FA\u1800\u180B\u180E\u180F"+"\u1885\u1887\u18A9\u18AA\u1920\u1923\u1927\u1929\u1932\u1933"+
"\u1939\u193C\u1940\u1941\u1944\u1946\u19DE\u1A00\u1A17\u1A19"+"\u1A1B\u1A1C\u1A56\u1A57\u1A58\u1A5F\u1A60\u1A61\u1A62\u1A63"+
"\u1A65\u1A6D\u1A73\u1A7D\u1A7F\u1A80\u1AB0\u1ABF\u1B00\u1B04"+"\u1B34\u1B35\u1B36\u1B3B\u1B3C\u1B3D\u1B42\u1B43\u1B6B\u1B74"+
"\u1B80\u1B82\u1BA2\u1BA6\u1BA8\u1BAA\u1BAB\u1BAE\u1BE6\u1BE7"+"\u1BE8\u1BEA\u1BED\u1BEE\u1BEF\u1BF2\u1C2C\u1C34\u1C36\u1C38"+
"\u1CD0\u1CD3\u1CD4\u1CE1\u1CE2\u1CE9\u1CED\u1CEE\u1CF4\u1CF5"+"\u1CF8\u1CFA\u1DC0\u1DFA\u1DFB\u1E00\u1FBD\u1FBE\u1FBF\u1FC2"+
"\u1FCD\u1FD0\u1FDD\u1FE0\u1FED\u1FF0\u1FFD\u1FFF\u2000\u200B"+"\u200C\u200D\u200E\u200F\u2010\u2028\u2029\u202A\u202B\u202C"+
"\u202D\u202E\u202F\u2030\u2035\u2039\u203B\u2044\u2045\u2046"+"\u2047\u205F\u2060\u2065\u2066\u2067\u2068\u2069\u206A\u2070"+
"\u2071\u2074\u207A\u207C\u207D\u207E\u207F\u2080\u208A\u208C"+"\u208D\u208E\u208F\u20A0\u20D0\u20F1\u2100\u2102\u2103\u2107"+
"\u2108\u210A\u2114\u2115\u2116\u2119\u211E\u2124\u2125\u2126"+"\u2127\u2128\u2129\u212A\u212E\u212F\u213A\u213C\u2140\u2145"+
"\u214A\u214E\u2150\u2160\u2189\u218C\u2190\u2208\u220E\u2212"+"\u2213\u2214\u2215\u2216\u223C\u223E\u2243\u2244\u2252\u2256"+
"\u2264\u226C\u226E\u228C\u228F\u2293\u2298\u2299\u22A2\u22A4"+"\u22A6\u22A7\u22A8\u22AA\u22AB\u22AC\u22B0\u22B8\u22C9\u22CE"+
"\u22D0\u22D2\u22D6\u22EE\u22F0\u22F5\u22F6\u22F8\u22FA\u22FF"+"\u2308\u2309\u230A\u230B\u230C\u2329\u232A\u232B\u2336\u237B"+
"\u2395\u2396\u2427\u2440\u244B\u2460\u2488\u249C\u24EA\u26AC"+"\u26AD\u2768\u2769\u276A\u276B\u276C\u276D\u276E\u276F\u2770"+
"\u2771\u2772\u2773\u2774\u2775\u2776\u27C3\u27C5\u27C6\u27C7"+"\u27C8\u27CA\u27CB\u27CC\u27CD\u27CE\u27D5\u27D7\u27DD\u27DF"+
"\u27E2\u27E6\u27E7\u27E8\u27E9\u27EA\u27EB\u27EC\u27ED\u27EE"+"\u27EF\u27F0\u2800\u2900\u2983\u2984\u2985\u2986\u2987\u2988"+
"\u2989\u298A\u298B\u298C\u298D\u298E\u298F\u2990\u2991\u2992"+"\u2993\u2994\u2995\u2996\u2997\u2998\u2999\u29B8\u29B9\u29C0"+
"\u29C2\u29C4\u29C6\u29CF\u29D3\u29D4\u29D6\u29D8\u29D9\u29DA"+"\u29DB\u29DC\u29F5\u29F6\u29F8\u29FA\u29FC\u29FD\u29FE\u2A2B"+
"\u2A2F\u2A34\u2A36\u2A3C\u2A3E\u2A64\u2A66\u2A79\u2A7B\u2A7D"+"\u2A85\u2A8B\u2A8D\u2A91\u2A9D\u2AA1\u2AA3\u2AA6\u2AAE\u2AAF"+
"\u2AB1\u2AB3\u2AB5\u2ABB\u2AC7\u2ACD\u2AD7\u2ADE\u2ADF\u2AE3"+"\u2AE6\u2AEC\u2AEE\u2AF7\u2AFB\u2B74\u2B76\u2B96\u2B98\u2BBA"+
"\u2BBD\u2BC9\u2BCA\u2BD3\u2BEC\u2BF0\u2CE5\u2CEB\u2CEF\u2CF2"+"\u2CF9\u2D00\u2D7F\u2D80\u2DE0\u2E00\u2E02\u2E06\u2E09\u2E0B"+
"\u2E0C\u2E0E\u2E1C\u2E1E\u2E20\u2E22\u2E23\u2E24\u2E25\u2E26"+"\u2E27\u2E28\u2E29\u2E2A\u2E4A\u2E80\u2E9A\u2E9B\u2EF4\u2F00"+
"\u2FD6\u2FF0\u2FFC\u3000\u3001\u3005\u3008\u3009\u300A\u300B"+"\u300C\u300D\u300E\u300F\u3010\u3011\u3012\u3014\u3015\u3016"+
"\u3017\u3018\u3019\u301A\u301B\u301C\u3021\u302A\u302E\u3030"+"\u3031\u3036\u3038\u303D\u3040\u3099\u309B\u309D\u30A0\u30A1"+
"\u30FB\u30FC\u31C0\u31E4\u321D\u321F\u3250\u3260\u327C\u327F"+"\u32B1\u32C0\u32CC\u32D0\u3377\u337B\u33DE\u33E0\u33FF\u3400"+
"\u4DC0\u4E00\uA490\uA4C7\uA60D\uA610\uA66F\uA673\uA674\uA67E"+"\uA680\uA69E\uA6A0\uA6F0\uA6F2\uA700\uA722\uA788\uA789\uA802"+
"\uA803\uA806\uA807\uA80B\uA80C\uA825\uA827\uA828\uA82C\uA838"+"\uA83A\uA874\uA878\uA8C4\uA8C6\uA8E0\uA8F2\uA926\uA92E\uA947"+
"\uA952\uA980\uA983\uA9B3\uA9B4\uA9B6\uA9BA\uA9BC\uA9BD\uA9E5"+"\uA9E6\uAA29\uAA2F\uAA31\uAA33\uAA35\uAA37\uAA43\uAA44\uAA4C"+
"\uAA4D\uAA7C\uAA7D\uAAB0\uAAB1\uAAB2\uAAB5\uAAB7\uAAB9\uAABE"+"\uAAC0\uAAC1\uAAC2\uAAEC\uAAEE\uAAF6\uAAF7\uABE5\uABE6\uABE8"+
"\uABE9\uABED\uABEE\uFB1D\uFB1E\uFB1F\uFB29\uFB2A\uFB50\uFD3E"+"\uFD40\uFDD0\uFDF0\uFDFD\uFDFE\uFE00\uFE10\uFE1A\uFE20\uFE30"+
"\uFE50\uFE51\uFE52\uFE53\uFE54\uFE55\uFE56\uFE59\uFE5A\uFE5B"+"\uFE5C\uFE5D\uFE5E\uFE5F\uFE60\uFE62\uFE64\uFE66\uFE67\uFE68"+
"\uFE69\uFE6B\uFE6C\uFE70\uFEFF\uFF00\uFF01\uFF03\uFF06\uFF08"+"\uFF09\uFF0A\uFF0B\uFF0C\uFF0D\uFF0E\uFF10\uFF1A\uFF1B\uFF1C"+
"\uFF1D\uFF1E\uFF1F\uFF21\uFF3B\uFF3C\uFF3D\uFF3E\uFF41\uFF5B"+"\uFF5C\uFF5D\uFF5E\uFF5F\uFF60\uFF61\uFF62\uFF63\uFF64\uFF66"+
"\uFFE0\uFFE2\uFFE5\uFFE7\uFFE8\uFFEF\uFFF9\uFFFC\uFFFE\uFFFF";var pp="\x0A\x0C\x0B\x0C\x0D\x0B\x0A\x0B\x0C\x0D\x0E\x06\x0E"+
"\x2E\x4E\x0E\x05\x08\x05\x08\x04\x08\x0E\x6E\x0E\x6E"+"\x0E\x01\x2E\x0E\x4E\x0E\x01\x2E\x0E\x4E\x0E\x0A\x0B"+
"\x0A\x08\x0E\x06\x0E\x01\x6E\x0E\x8A\x0E\x06\x04\x0E"+"\x01\x0E\x04\x01\x6E\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+
"\x0E\x01\x0E\x01\x0E\x01\x0E\x89\x01\x0E\x01\x0E\x01"+"\x0E\x01\x0E\x01\x0E\x01\x89\x01\x0E\x01\x0E\x06\x02"+
"\x89\x02\x89\x02\x89\x02\x89\x02\x89\x02\x07\x0E\x03"+"\x06\x03\x08\x03\x0E\x89\x03\x83\x03\x1A\x03\x18\x1A"+
"\x18\x1A\x18\x1A\x18\x1A\x1B\x1A\x18\x1A\x89\x07\x06"+"\x07\x03\x1A\x89\x18\x03\x18\x1A\x18\x1A\x18\x1A\x18"+
"\x1A\x18\x1A\x18\x1A\x18\x03\x18\x89\x07\x0E\x89\x03"+"\x89\x0E\x89\x18\x04\x1A\x03\x1A\x03\x83\x18\x89\x1A"+
"\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x89\x03"+"\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x03"+
"\x89\x03\x02\x89\x02\x0E\x02\x89\x02\x89\x02\x89\x02"+"\x89\x02\x89\x02\x1A\x03\x1A\x03\x18\x1A\x18\x03\x02"+
"\x1A\x18\x03\x18\x1A\x18\x1A\x03\x1A\x18\x1A\x03\x89"+"\x07\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x06"+"\x01\x06\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x89\x01\x89\x01\x06\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x89\x01\x0E\x06\x0E\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x0E\x01\x89\x01\x89\x01\x81"+"\x01\x81\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x06\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x2E\x4E\x2E\x4E\x01\x89"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x0E\x01\x0E\x01"+
"\x0D\x01\x2E\x4E\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x06\x01\x89\x01\x0E"+
"\x01\x0E\x89\x0A\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x0E\x01\x0E\x01\x0E\x01\x89\x01\x89"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0D"+
"\x8A\x0A\x8A\x81\x82\x0E\x0D\x0B\x8F\x91\x93\x90\x92"+"\x08\x06\x0E\x6E\x0E\x08\x2E\x4E\x0E\x0D\x8A\x01\x14"+
"\x15\x16\x17\x8A\x04\x01\x04\x05\x0E\x2E\x4E\x01\x04"+"\x05\x0E\x2E\x4E\x01\x06\x89\x01\x0E\x01\x0E\x01\x0E"+
"\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+"\x06\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E"+
"\x6E\x0E\x05\x06\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+"\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E"+
"\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+"\x6E\x0E\x6E\x0E\x2E\x4E\x2E\x4E\x0E\x2E\x4E\x0E\x01"+
"\x0E\x01\x0E\x01\x0E\x01\x0E\x04\x01\x0E\x01\x0E\x2E"+"\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+
"\x0E\x6E\x2E\x4E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+"\x6E\x0E\x6E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+
"\x0E\x01\x0E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+"\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x0E"+
"\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x2E\x4E\x2E"+"\x4E\x0E\x6E\x0E\x6E\x0E\x2E\x4E\x0E\x6E\x0E\x6E\x0E"+
"\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E"+"\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+
"\x6E\x0E\x6E\x0E\x6E\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+"\x0E\x01\x0E\x01\x0E\x01\x89\x01\x0E\x01\x89\x01\x89"+
"\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x2E\x4E\x2E"+"\x4E\x2E\x4E\x2E\x4E\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+
"\x0E\x01\x0D\x0E\x01\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+"\x2E\x4E\x0E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x0E\x01"+
"\x89\x01\x0E\x01\x0E\x01\x0E\x01\x89\x0E\x01\x0E\x01"+"\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E"+
"\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+"\x89\x0E\x89\x0E\x01\x89\x01\x89\x01\x0E\x01\x0E\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x0E\x01\x06\x01\x0E"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x02"+"\x89\x02\x05\x02\x03\x0E\x03\x01\x03\x0E\x03\x89\x0E"+
"\x01\x89\x0E\x08\x0E\x08\x01\x0E\x08\x0E\x2E\x4E\x2E"+"\x4E\x2E\x4E\x06\x0E\x05\x6E\x0E\x01\x0E\x06\x0E\x01"+
"\x03\x8A\x01\x0E\x06\x0E\x2E\x4E\x0E\x05\x08\x05\x08"+"\x04\x08\x0E\x6E\x0E\x6E\x0E\x01\x2E\x0E\x4E\x0E\x01"+
"\x2E\x0E\x4E\x0E\x2E\x4E\x0E\x2E\x4E\x0E\x01\x06\x0E"+"\x06\x01\x0E\x01\x8E\x0E\x01\x01";
var pm="\u0028\u0029\u0029\u0028\u003C\u003E\u003E\u003C\u005B\u005D"+"\u005D\u005B\u007B\u007D\u007D\u007B\u00AB\u00BB\u00BB\u00AB"+
"\u0F3A\u0F3B\u0F3B\u0F3A\u0F3C\u0F3D\u0F3D\u0F3C\u169B\u169C"+"\u169C\u169B\u2039\u203A\u203A\u2039\u2045\u2046\u2046\u2045"+
"\u207D\u207E\u207E\u207D\u208D\u208E\u208E\u208D\u2208\u220B"+"\u2209\u220C\u220A\u220D\u220B\u2208\u220C\u2209\u220D\u220A"+
"\u2215\u29F5\u223C\u223D\u223D\u223C\u2243\u22CD\u2252\u2253"+"\u2253\u2252\u2254\u2255\u2255\u2254\u2264\u2265\u2265\u2264"+
"\u2266\u2267\u2267\u2266\u2268\u2269\u2269\u2268\u226A\u226B"+"\u226B\u226A\u226E\u226F\u226F\u226E\u2270\u2271\u2271\u2270"+
"\u2272\u2273\u2273\u2272\u2274\u2275\u2275\u2274\u2276\u2277"+"\u2277\u2276\u2278\u2279\u2279\u2278\u227A\u227B\u227B\u227A"+
"\u227C\u227D\u227D\u227C\u227E\u227F\u227F\u227E\u2280\u2281"+"\u2281\u2280\u2282\u2283\u2283\u2282\u2284\u2285\u2285\u2284"+
"\u2286\u2287\u2287\u2286\u2288\u2289\u2289\u2288\u228A\u228B"+"\u228B\u228A\u228F\u2290\u2290\u228F\u2291\u2292\u2292\u2291"+
"\u2298\u29B8\u22A2\u22A3\u22A3\u22A2\u22A6\u2ADE\u22A8\u2AE4"+"\u22A9\u2AE3\u22AB\u2AE5\u22B0\u22B1\u22B1\u22B0\u22B2\u22B3"+
"\u22B3\u22B2\u22B4\u22B5\u22B5\u22B4\u22B6\u22B7\u22B7\u22B6"+"\u22C9\u22CA\u22CA\u22C9\u22CB\u22CC\u22CC\u22CB\u22CD\u2243"+
"\u22D0\u22D1\u22D1\u22D0\u22D6\u22D7\u22D7\u22D6\u22D8\u22D9"+"\u22D9\u22D8\u22DA\u22DB\u22DB\u22DA\u22DC\u22DD\u22DD\u22DC"+
"\u22DE\u22DF\u22DF\u22DE\u22E0\u22E1\u22E1\u22E0\u22E2\u22E3"+"\u22E3\u22E2\u22E4\u22E5\u22E5\u22E4\u22E6\u22E7\u22E7\u22E6"+
"\u22E8\u22E9\u22E9\u22E8\u22EA\u22EB\u22EB\u22EA\u22EC\u22ED"+"\u22ED\u22EC\u22F0\u22F1\u22F1\u22F0\u22F2\u22FA\u22F3\u22FB"+
"\u22F4\u22FC\u22F6\u22FD\u22F7\u22FE\u22FA\u22F2\u22FB\u22F3"+"\u22FC\u22F4\u22FD\u22F6\u22FE\u22F7\u2308\u2309\u2309\u2308"+
"\u230A\u230B\u230B\u230A\u2329\u232A\u232A\u2329\u2768\u2769"+"\u2769\u2768\u276A\u276B\u276B\u276A\u276C\u276D\u276D\u276C"+
"\u276E\u276F\u276F\u276E\u2770\u2771\u2771\u2770\u2772\u2773"+"\u2773\u2772\u2774\u2775\u2775\u2774\u27C3\u27C4\u27C4\u27C3"+
"\u27C5\u27C6\u27C6\u27C5\u27C8\u27C9\u27C9\u27C8\u27CB\u27CD"+"\u27CD\u27CB\u27D5\u27D6\u27D6\u27D5\u27DD\u27DE\u27DE\u27DD"+
"\u27E2\u27E3\u27E3\u27E2\u27E4\u27E5\u27E5\u27E4\u27E6\u27E7"+"\u27E7\u27E6\u27E8\u27E9\u27E9\u27E8\u27EA\u27EB\u27EB\u27EA"+
"\u27EC\u27ED\u27ED\u27EC\u27EE\u27EF\u27EF\u27EE\u2983\u2984"+"\u2984\u2983\u2985\u2986\u2986\u2985\u2987\u2988\u2988\u2987"+
"\u2989\u298A\u298A\u2989\u298B\u298C\u298C\u298B\u298D\u2990"+"\u298E\u298F\u298F\u298E\u2990\u298D\u2991\u2992\u2992\u2991"+
"\u2993\u2994\u2994\u2993\u2995\u2996\u2996\u2995\u2997\u2998"+"\u2998\u2997\u29B8\u2298\u29C0\u29C1\u29C1\u29C0\u29C4\u29C5"+
"\u29C5\u29C4\u29CF\u29D0\u29D0\u29CF\u29D1\u29D2\u29D2\u29D1"+"\u29D4\u29D5\u29D5\u29D4\u29D8\u29D9\u29D9\u29D8\u29DA\u29DB"+
"\u29DB\u29DA\u29F5\u2215\u29F8\u29F9\u29F9\u29F8\u29FC\u29FD"+"\u29FD\u29FC\u2A2B\u2A2C\u2A2C\u2A2B\u2A2D\u2A2E\u2A2E\u2A2D"+
"\u2A34\u2A35\u2A35\u2A34\u2A3C\u2A3D\u2A3D\u2A3C\u2A64\u2A65"+"\u2A65\u2A64\u2A79\u2A7A\u2A7A\u2A79\u2A7D\u2A7E\u2A7E\u2A7D"+
"\u2A7F\u2A80\u2A80\u2A7F\u2A81\u2A82\u2A82\u2A81\u2A83\u2A84"+"\u2A84\u2A83\u2A8B\u2A8C\u2A8C\u2A8B\u2A91\u2A92\u2A92\u2A91"+
"\u2A93\u2A94\u2A94\u2A93\u2A95\u2A96\u2A96\u2A95\u2A97\u2A98"+"\u2A98\u2A97\u2A99\u2A9A\u2A9A\u2A99\u2A9B\u2A9C\u2A9C\u2A9B"+
"\u2AA1\u2AA2\u2AA2\u2AA1\u2AA6\u2AA7\u2AA7\u2AA6\u2AA8\u2AA9"+"\u2AA9\u2AA8\u2AAA\u2AAB\u2AAB\u2AAA\u2AAC\u2AAD\u2AAD\u2AAC"+
"\u2AAF\u2AB0\u2AB0\u2AAF\u2AB3\u2AB4\u2AB4\u2AB3\u2ABB\u2ABC"+"\u2ABC\u2ABB\u2ABD\u2ABE\u2ABE\u2ABD\u2ABF\u2AC0\u2AC0\u2ABF"+
"\u2AC1\u2AC2\u2AC2\u2AC1\u2AC3\u2AC4\u2AC4\u2AC3\u2AC5\u2AC6"+"\u2AC6\u2AC5\u2ACD\u2ACE\u2ACE\u2ACD\u2ACF\u2AD0\u2AD0\u2ACF"+
"\u2AD1\u2AD2\u2AD2\u2AD1\u2AD3\u2AD4\u2AD4\u2AD3\u2AD5\u2AD6"+"\u2AD6\u2AD5\u2ADE\u22A6\u2AE3\u22A9\u2AE4\u22A8\u2AE5\u22AB"+
"\u2AEC\u2AED\u2AED\u2AEC\u2AF7\u2AF8\u2AF8\u2AF7\u2AF9\u2AFA"+"\u2AFA\u2AF9\u2E02\u2E03\u2E03\u2E02\u2E04\u2E05\u2E05\u2E04"+
"\u2E09\u2E0A\u2E0A\u2E09\u2E0C\u2E0D\u2E0D\u2E0C\u2E1C\u2E1D"+"\u2E1D\u2E1C\u2E20\u2E21\u2E21\u2E20\u2E22\u2E23\u2E23\u2E22"+
"\u2E24\u2E25\u2E25\u2E24\u2E26\u2E27\u2E27\u2E26\u2E28\u2E29"+"\u2E29\u2E28\u3008\u3009\u3009\u3008\u300A\u300B\u300B\u300A"+
"\u300C\u300D\u300D\u300C\u300E\u300F\u300F\u300E\u3010\u3011"+"\u3011\u3010\u3014\u3015\u3015\u3014\u3016\u3017\u3017\u3016"+
"\u3018\u3019\u3019\u3018\u301A\u301B\u301B\u301A\uFE59\uFE5A"+"\uFE5A\uFE59\uFE5B\uFE5C\uFE5C\uFE5B\uFE5D\uFE5E\uFE5E\uFE5D"+
"\uFE64\uFE65\uFE65\uFE64\uFF08\uFF09\uFF09\uFF08\uFF1C\uFF1E"+"\uFF1E\uFF1C\uFF3B\uFF3D\uFF3D\uFF3B\uFF5B\uFF5D\uFF5D\uFF5B"+
"\uFF5F\uFF60\uFF60\uFF5F\uFF62\uFF63\uFF63\uFF62";var pn=[0x0621,"\u0000\u0000\u0000"
,0x0622,"\u0000\u0000\uFE82",0x0623,"\u0000\u0000\uFE84",0x0624,"\u0000\u0000\uFE86"
,0x0625,"\u0000\u0000\uFE88",0x0626,"\uFE8B\uFE8C\uFE8A",0x0627,"\u0000\u0000\uFE8E"
,0x0628,"\uFE91\uFE92\uFE90",0x0629,"\u0000\u0000\uFE94",0x062A,"\uFE97\uFE98\uFE96"
,0x062B,"\uFE9B\uFE9C\uFE9A",0x062C,"\uFE9F\uFEA0\uFE9E",0x062D,"\uFEA3\uFEA4\uFEA2"
,0x062E,"\uFEA7\uFEA8\uFEA6",0x062F,"\u0000\u0000\uFEAA",0x0630,"\u0000\u0000\uFEAC"
,0x0631,"\u0000\u0000\uFEAE",0x0632,"\u0000\u0000\uFEB0",0x0633,"\uFEB3\uFEB4\uFEB2"
,0x0634,"\uFEB7\uFEB8\uFEB6",0x0635,"\uFEBB\uFEBC\uFEBA",0x0636,"\uFEBF\uFEC0\uFEBE"
,0x0637,"\uFEC3\uFEC4\uFEC2",0x0638,"\uFEC7\uFEC8\uFEC6",0x0639,"\uFECB\uFECC\uFECA"
,0x063A,"\uFECF\uFED0\uFECE",0x0641,"\uFED3\uFED4\uFED2",0x0642,"\uFED7\uFED8\uFED6"
,0x0643,"\uFEDB\uFEDC\uFEDA",0x0644,"\uFEDF\uFEE0\uFEDE",0x0645,"\uFEE3\uFEE4\uFEE2"
,0x0646,"\uFEE7\uFEE8\uFEE6",0x0647,"\uFEEB\uFEEC\uFEEA",0x0648,"\u0000\u0000\uFEEE"
,0x0649,"\uFBE8\uFBE9\uFEF0",0x064A,"\uFEF3\uFEF4\uFEF2",0x0671,"\u0000\u0000\uFB51"
,0x0677,"\u0000\u0000\u0000",0x0679,"\uFB68\uFB69\uFB67",0x067A,"\uFB60\uFB61\uFB5F"
,0x067B,"\uFB54\uFB55\uFB53",0x067E,"\uFB58\uFB59\uFB57",0x067F,"\uFB64\uFB65\uFB63"
,0x0680,"\uFB5C\uFB5D\uFB5B",0x0683,"\uFB78\uFB79\uFB77",0x0684,"\uFB74\uFB75\uFB73"
,0x0686,"\uFB7C\uFB7D\uFB7B",0x0687,"\uFB80\uFB81\uFB7F",0x0688,"\u0000\u0000\uFB89"
,0x068C,"\u0000\u0000\uFB85",0x068D,"\u0000\u0000\uFB83",0x068E,"\u0000\u0000\uFB87"
,0x0691,"\u0000\u0000\uFB8D",0x0698,"\u0000\u0000\uFB8B",0x06A4,"\uFB6C\uFB6D\uFB6B"
,0x06A6,"\uFB70\uFB71\uFB6F",0x06A9,"\uFB90\uFB91\uFB8F",0x06AD,"\uFBD5\uFBD6\uFBD4"
,0x06AF,"\uFB94\uFB95\uFB93",0x06B1,"\uFB9C\uFB9D\uFB9B",0x06B3,"\uFB98\uFB99\uFB97"
,0x06BA,"\u0000\u0000\uFB9F",0x06BB,"\uFBA2\uFBA3\uFBA1",0x06BE,"\uFBAC\uFBAD\uFBAB"
,0x06C0,"\u0000\u0000\uFBA5",0x06C1,"\uFBA8\uFBA9\uFBA7",0x06C5,"\u0000\u0000\uFBE1"
,0x06C6,"\u0000\u0000\uFBDA",0x06C7,"\u0000\u0000\uFBD8",0x06C8,"\u0000\u0000\uFBDC"
,0x06C9,"\u0000\u0000\uFBE3",0x06CB,"\u0000\u0000\uFBDF",0x06CC,"\uFBFE\uFBFF\uFBFD"
,0x06D0,"\uFBE6\uFBE7\uFBE5",0x06D2,"\u0000\u0000\uFBAF",0x06D3,"\u0000\u0000\uFBB1"
,0xFEF5,"\u0000\u0000\uFEF6",0xFEF7,"\u0000\u0000\uFEF8",0xFEF9,"\u0000\u0000\uFEFA"
,0xFEFB,"\u0000\u0000\uFEFC"];function pP(a5){var s=0;var min=0;var e=po;var max=
e.length-1;while(max>=min){s=(max+min)>>1;if(a5===e.charCodeAt(s))break;else if(
a5<e.charCodeAt(s))max=s-1;else if((a5>=e.charCodeAt(s+1)))min=s+1;else break;}return pp.
charCodeAt(s);}function mS(a5){var s=0;var min=0;var e=pm;var max=(e.length>>1)-
1;var dg=-1;while(max>=min){s=(max+min)>>1;dg=a5-e.charCodeAt(s*2);if(!dg)return e.
charCodeAt(s*2+1);if(dg<0)max=s-1;else min=s+1;}return 0;}function k4(a5){var s=
0;var min=0;var e=pn;var max=(e.length>>1)-1;var dg=-1;while(max>=min){s=(max+min
)>>1;dg=a5-e[s*2];if(!dg)return e[s*2+1];if(dg<0)max=s-1;else min=s+1;}return"";
}function pB(bn,bs,lQ){var fr=0;var aN=bn[bs*4];if(aN!==23)return 0;for(;bs>=lQ;
bs--){aN=bn[bs*4];if(aN===23)fr--;if(fr&&((aN===20)||(aN===21)||(aN===22)))if(++
fr===0)return bs;}return 0;}function k2(bn,bs,eZ){var fr=0;var aN=bn[bs*4];if((aN
!==20)&&(aN!==21)&&(aN!==22))return 0;for(;bs<eZ;bs++){aN=bn[bs*4];if((aN===20)||(
aN===21)||(aN===22))fr++;else if(fr&&(aN===23)){if(fr===1)return bs;if(fr>1)fr--;
}}return 0;}function mR(bn,bs,eZ){for(;bs<eZ;bs++){var aN=bn[bs*4];if((aN===1)||(
aN===2)||(aN===3))return bs;if((aN!==20)&&(aN!==21)&&(aN!==22))continue;if(!(bs=
k2(bn,bs,eZ)))bs=eZ;}return 0;}function k1(bn,bs,eZ){var ap=bn[bs*4+1];if(bs>=eZ
)return 0;while((bs<eZ)&&(bn[bs*4+1]===ap))bs++;return bs-1;}function p5(iN,dY,jX
,bn,m,i,qY,qZ,nA){var fl=[];var kr=[];var bQ=-1;var ai=0;var bN=0;for(;dY<jX;dY++
){var aH=iN[dY];while((nA>0)&&((aH+bN)>=qY[qZ-bN])){bN++;nA--;}if(bn[aH*4]!==14)
continue;var bd=bn[aH*4+2];if(bd===1){if(bQ>=(63-1))break;kr.push(aH);fl.push(mS(
m[i+aH-3+bN]));bQ++;}else if(bd===2){var ad=m[i+aH-3+bN];var e7=bQ;while((e7>0)&&(
ad!==fl[e7])&&((ad!==0x232A)||(fl[e7]!==0x3009))&&((ad!==0x3009)||(fl[e7]!==0x232A
)))e7--;if((e7>=0)&&((ad===fl[e7])||((ad===0x232A)&&(fl[e7]===0x3009))||((ad===0x3009
)&&(fl[e7]===0x232A)))){while(bQ>e7){bn[kr.pop()*4+2]=3;fl.pop();bQ--;}fl.pop();
kr.pop();bQ--;ai++;}else bn[aH*4+2]=3;}}while(bQ>=0){bn[kr.pop()*4+2]=3;bQ--;}for(;
dY<jX;dY++){var aH=iN[dY]*4;if((bn[aH]===14)&&((bn[aH+2]===1)||(bn[aH+2]===2)))bn[
aH+2]=3;}return ai;}function py(iN,dY,jX,bn){var aH=iN[dY]*4;var ai=1;if((bn[aH]
!==14)||(bn[aH+2]!==1))return 0;for(dY++;dY<jX;dY++){var aH=iN[dY]*4;var type=bn[
aH];var bd=bn[aH+2];if((type===14)&&(bd===1))ai++;else if((type===14)&&(bd===2))
if(!--ai)return dY;}return 0;}function pO(bn,bs,lQ,lP){var ap=bn[bs*4+1];var cV=
lP;if(bs>lQ)cV=bn[bs*4-3];if(cV>ap)ap=cV;if(ap&1)return 2;else return 1;}function
pN(bn,bs,eZ,lP){var ap=bn[bs*4+1];var mg=lP;var type=bn[bs*4];if((bs<(eZ-1))&&(type
!==21)&&(type!==20)&&(type!==22))mg=bn[bs*4+5];if(mg>ap)ap=mg;if(ap&1)return 2;else
return 1;}d.qk=function(lM){if(lM<=0)return null;return{Data:new Uint8Array((lM+
4)*4),pV:lM,fd:0,mT:false,hg:0};};d.ng=function(an){};d.sD=function(an){return an&&
an.mT;};d.qj=function(an){return an&&(an.hg===1);};d.hl=function(an,bg){if(!an)return 0;
if((bg<0)||(bg>=an.fd))return an.hg;return an.Data[bg*4+13];};d.m_=function(an,m
,i,al,lv){var e=an?an.Data:null;var sa=an?an.pV:0;var length=(al<0)?(m.length-i-
1):al;var eJ=lv>0;var s=0;var f4=12;if(sa<length)return false;for(;s<length;s++){
var l7=pP(m[i+s]);var type=l7&31;var lU=l7&96;var r6=l7&128;var bd=0;if(type===24
)bd=8;else if(type===25)bd=9;else if(type===26)bd=10;else if(type===27)bd=11;else
if(lU===32)bd=1;else if(lU===64)bd=2;else if(lU===96)bd=3;else if(type===9)bd=4;
else if(type===13)bd=5;else if(type===12)bd=6;else if(type===11)bd=6;else if((type===
21)||(type===20)||(type===22)||(type===23))bd=7;else if(r6)bd=12;if((type>=24)&&(
type<=27))type=3;if(!eJ&&((type===3)||(type===2)||(type===17)||(type===18)||(type===
21)))eJ=1;e[f4++]=type;e[f4++]=0;e[f4++]=bd;e[f4++]=0;}e[f4++]=0;e[f4++]=0;e[f4++
]=0;e[f4++]=0;an.fd=length;an.mT=eJ;an.hg=(lv>0)?1:0;if(!eJ)return false;if((lv<
0)&&(s=mR(e,3,length+3)))an.hg=(e[s*4]===1)?0:1;return true;};d.na=function(an,m
,i){var b5=[];var length=an?an.fd:0;var g3=an?an.hg:0;var e=an?an.Data:null;var cW=
3+length;var aH=new Uint32Array(length+2);var bQ=0;var g2=0;var hY=0;var mt=0;var
gV=0;var t;var bE;b5.push({he:g3,fC:0,jt:false});for(t=3;t<cW;t++){var aN=e[t*4];
if(aN===22){var oY=k2(e,t,cW);var g=mR(e,t+1,oY?oY:cW);if(g&&((e[g*4]===3)||(e[g
*4]===2)))aN=21;else aN=20;}if((aN===17)||(aN===15)||(aN===18)||(aN===16)){var ap=
b5[bQ].he;var type=0;if((aN===17)||(aN===18))ap+=(ap&1)?2:1;else ap+=(ap&1)?1:2;
if(aN===18)type=2;if(aN===16)type=1;if((ap<=125)&&!g2&&!hY){bQ++;b5.push({he:ap,
fC:type,jt:false});}if((ap>125)&&!g2)hY++;}else if((aN===21)||(aN===20)){var ap=
b5[bQ].he;e[t*4+1]=ap;if(b5[bQ].fC)e[t*4]=b5[bQ].fC;if(aN===21)ap+=(ap&1)?2:1;else
ap+=(ap&1)?1:2;if((ap<=125)&&!g2&&!hY){mt++;bQ++;b5.push({he:ap,fC:0,jt:true});}
else g2++;}else if(aN===23){if(g2)g2--;else if(mt){hY=0;while(!b5[bQ].jt){b5.pop(
);bQ--;}b5.pop();bQ--;mt--;}e[t*4+1]=b5[bQ].he;if(b5[bQ].fC)e[t*4]=b5[bQ].fC;}else
if(aN===19){if(!g2){if(hY)hY--;else if((bQ&&!b5[bQ].jt)){b5.pop();bQ--;}}}else if((
aN!==11)&&(aN!==10)){e[t*4+1]=b5[bQ].he;if(b5[bQ].fC)e[t*4]=b5[bQ].fC;}}for(bE=3
,t=3;t<cW;t++,bE++){var aN=e[t*4];if((aN===17)||(aN===15)||(aN===18)||(aN===16)||(
aN===19)||(aN===10)){aH[length+2-++gV]=t;bE--;}else if(bE<t){e[bE*4]=e[t*4];e[bE
*4+1]=e[t*4+1];e[bE*4+2]=e[t*4+2];}}cW-=gV;for(t=3;t<cW;){var aP=0;var e6;var e2;
var ja;var hP;var dF;e[4]=0;e[5]=0;e[6]=0;e[8]=0;e[9]=0;e[10]=0;if((e[t*4]===23)&&
pB(e,t,3)){t=k1(e,t,cW)+1;continue;}e[4]=pO(e,t,3,g3);aH[aP++]=1;hP=k1(e,t,cW);for(;
t<=hP;t++)aH[aP++]=t;while(ja=k2(e,hP,cW)){hP=k1(e,ja,cW);for(;ja<=hP;ja++)aH[aP++
]=ja;}e[8]=pN(e,hP,cW,g3);aH[aP++]=2;e6=aP;for(dF=e[4],aP=1;aP<e6;aP++){var bv=aH[
aP];if(e[bv*4]===9){var hZ=e[aH[aP-1]*4];if((hZ===21)||(hZ===20)||(hZ===22)||(hZ===
23))hZ=14;e[bv*4]=hZ;}}for(dF=e[4],aP=1;aP<e6;aP++){var bv=aH[aP];var ko=aH[aP-1
];var type=e[bv*4];if((type===1)||(type===2)||(type===3))dF=type;if((type===4)&&(
dF===3))type=e[bv*4]=7;if(type===3)type=e[bv*4]=2;if((type===4)&&(e[ko*4]===5)&&(
e[aH[aP-2]*4]===type))e[ko*4]=type;if(((type===7)||(type===4))&&(e[ko*4]===8)&&(
e[aH[aP-2]*4]===type))e[ko*4]=type;}for(aP=1;aP<e6;aP++){var bv=aH[aP];var type=
e[bv*4];if(type===4){var dP=aP-1;while(e[aH[dP]*4]===6)e[aH[dP--]*4]=4;}if((type===
6)&&(e[aH[aP-1]*4]===4))e[bv*4]=4;}for(aP=1;aP<e6;aP++){var bv=aH[aP];var type=e[
bv*4];if((type===6)||(type===5)||(type===8))e[bv*4]=14;}for(dF=e[4],aP=1;aP<e6;aP++
){var bv=aH[aP];var type=e[bv*4];if((type===4)&&(dF===1))e[bv*4]=1;if((type===1)||(
type===2)||(type===3))dF=type;}if(p5(aH,aP=1,e6-1,e,m,i,aH,length+1,gV)){var eG=(
e[aH[aP]*4+1]&1)?2:1;var dF=e[8];for(;aP<e6;aP++){var hH=py(aH,aP,e6,e);var dP=aP+
1;var type=e[aH[aP]*4];var jf=0;var g5=0;var hW=0;if((type===4)||(type===7))type=
2;if((type===2)||(type===1))dF=type;if(!hH)continue;for(;(dP<hH)&&(!jf||!g5);dP++
){var kR=e[aH[dP]*4];if(kR===1)jf++;if(kR===2)g5++;if(kR===7)g5++;if(kR===4)g5++;
}if(((eG===1)&&jf)||((eG===2)&&g5)){e[aH[aP]*4]=eG;e[aH[hH]*4]=eG;hW=eG;dF=eG;}else
if((jf||g5)&&(dF!==eG)){e[aH[aP]*4]=dF;e[aH[hH]*4]=dF;hW=dF;}else if(jf||g5){e[aH[
aP]*4]=eG;e[aH[hH]*4]=eG;hW=eG;dF=eG;}if(hW){for(dP=aP+1;e[aH[dP]*4+2]===4;dP++)
e[aH[dP]*4]=hW;for(dP=hH+1;e[aH[dP]*4+2]===4;dP++)e[aH[dP]*4]=hW;}}}for(e2=0,aP=
1;aP<e6;aP++){var bv=aH[aP];var type=e[bv*4];if(!e2&&((type===11)||(type===12)||(
type===13)||(type===14)||(type===22)||(type===20)||(type===21)||(type===23)))e2=
aP;if(e2&&(type!==11)&&(type!==12)&&(type!==13)&&(type!==14)&&(type!==22)&&(type
!==20)&&(type!==21)&&(type!==23)){var gl=e[aH[e2-1]*4];var kS=type;if((gl===4)||(
gl===7))gl=2;if((kS===4)||(kS===7))kS=2;if((gl!==kS)||((gl!==1)&&(gl!==2)))gl=(e[
aH[e2]*4+1]&1)?2:1;for(;e2<aP;e2++)e[aH[e2]*4]=gl;e2=0;}}}for(t=3;t<cW;t++){var type=
e[t*4];if(e[t*4+1]&1)if((type===1)||(type===4)||(type===7))e[t*4+1]++;else;else if(
type===2)e[t*4+1]++;else if((type===4)||(type===7))e[t*4+1]+=2;}for(t=cW-1,bE=t+
gV;gV>0;t--,bE--){if(aH[length+2-gV]===bE){var ap=g3;var ad=m[i+bE-3];if((t>=3)&&(
e[t*4+1]>ap))ap=e[t*4+1];if((bE<(cW-1))&&(e[(bE+1)*4+1]>ap))ap=e[(bE+1)*4+1];if(
ad===0x200D)e[bE*4+2]=11;else if(((ad<0x0000)||(ad>0x0008))&&((ad<0x000E)||(ad>0x001B
))&&((ad<0x007F)||(ad>0x0084))&&((ad<0x0086)||(ad>0x009F))&&(ad!==0x180E)&&(ad!==
0x200C))e[bE*4+2]=12;else e[bE*4+2]=0;e[bE*4]=31;e[bE*4+1]=ap;t++;gV--;cW++;}else
if(bE>t){e[bE*4]=e[t*4];e[bE*4+1]=e[t*4+1];e[bE*4+2]=e[t*4+2];}}};d.m9=function(
an,m,i,ez,fM){var length=an?an.fd:0;var e=an?an.Data:null;var cW=3+length;var t=
3;var bE=3;var ap=e[13];for(;(t<=cW);t++){var gg=0;var bv=0;var g0=0;if((ap===e[
t*4+1])&&(t<cW))continue;for(;(bE<t)||bv||gg||g0;bE++){var dM;var dN;if(bE<t){var
bd=e[bE*4+2];if((bd===12)||(bd===4))continue;gg=bv;bv=g0;g0=bE;}else{gg=bv;bv=g0;
g0=0;}if(ap&1){dM=g0;dN=gg;}else{dM=gg;dN=g0;}if(bv&&(e[bv*4+2]===8)){var s=bv-3;
var ad=m[i+s];var c=k4(ad);var ag=ez?fM[(ez[i+s]>>12)&0xFF]:null;if(!ag)ag=fM[0];
if(c.charCodeAt(2)&&dN&&((e[dN*4+2]===9)||(e[dN*4+2]===10)||(e[dN*4+2]===11))&&(
!ag||this.eV(ag,c.charCodeAt(2))))m[i+s]=c.charCodeAt(2);}else if(bv&&(e[bv*4+2]===
9)){var s=bv-3;var ad=m[i+s];var c=k4(ad);var ag=ez?fM[(ez[i+s]>>12)&0xFF]:null;
if(!ag)ag=fM[0];if(c.charCodeAt(0)&&dM&&((e[dM*4+2]===8)||(e[dM*4+2]===10)||(e[dM
*4+2]===11))&&(!ag||this.eV(ag,c.charCodeAt(0))))m[i+s]=c.charCodeAt(0);}else if(
bv&&(e[bv*4+2]===10)){var s=bv-3;var ad=m[i+s];var c=k4(ad);var ag=ez?fM[(ez[i+s
]>>12)&0xFF]:null;if(!ag)ag=fM[0];if(c.charCodeAt(1)&&dN&&dM&&((e[dN*4+2]===9)||(
e[dN*4+2]===10)||(e[dN*4+2]===11))&&((e[dM*4+2]===8)||(e[dM*4+2]===10)||(e[dM*4+
2]===11))){if(!ag||this.eV(ag,c.charCodeAt(1)))m[i+s]=c.charCodeAt(1);}else if(c.
charCodeAt(2)&&dN&&((e[dN*4+2]===9)||(e[dN*4+2]===10)||(e[dN*4+2]===11))){if(!ag||
this.eV(ag,c.charCodeAt(2)))m[i+s]=c.charCodeAt(2);}else if(c.charCodeAt(0)&&dM&&((
e[dM*4+2]===8)||(e[dM*4+2]===10)||(e[dM*4+2]===11))){if(!ag||this.eV(ag,c.charCodeAt(
0)))m[i+s]=c.charCodeAt(0);}}if(bv&&gg){var d0=((ap&1)?bv:gg)-3;var ek=((ap&1)?gg:
bv)-3;var od=m[i+d0];var oe=m[i+ek];var d1=0;if(oe===0xFEDF)switch(od){case 0xFE82:
d1=0xFEF5;break;case 0xFE84:d1=0xFEF7;break;case 0xFE88:d1=0xFEF9;break;case 0xFE8E:
d1=0xFEFB;break;}if(oe===0xFEE0)switch(od){case 0xFE82:d1=0xFEF6;break;case 0xFE84:
d1=0xFEF8;break;case 0xFE88:d1=0xFEFA;break;case 0xFE8E:d1=0xFEFC;break;}if(d1){
var ag=ez?fM[(ez[i+d0]>>12)&0xFF]:null;if(!ag)ag=fM[0];if(ag&&!this.eV(ag,d1))d1=
0;}if(d1){m[i+d0]=d1;m[i+ek]=0xFEFF;}}}bE=t;ap=e[t*4+1];}};d.m$=function(an,m,i){
var length=an?an.fd:0;var e=an?an.Data:null;var fm=3+length;var bz;if(!e)return;
for(bz=fm-1;bz>=3;bz--){var bd=e[bz*4+2];if((e[bz*4+1]&1)&&((bd===1)||(bd===2)||(
bd===3))){var oX=mS(m[i+bz-3]);if(oX)m[i+bz-3]=oX;}}};d.ld=function(an,fR,gM){var
e=an?an.Data:null;var dh=fR+3;var fm=gM+3;var g3=an?an.hg:0;var lS=true;var bz;if(
!e)return;for(bz=fm-1;bz>=dh;bz--){var bd=e[bz*4+2];if(bd===6){e[bz*4+1]=g3;lS=true;
}else if(lS&&((bd===5)||(bd===7)))e[bz*4+1]=g3;else if(e[bz*4]!==31)lS=0;}};d.ih=
function(an,fR,gM,cZ,fJ){var b5=[];var e=an?an.Data:null;var dh=fR+3;var fm=gM+3;
var aQ=dh;var ap=0;var eP=0;var eO=0;var bz;if(!e)return;for(bz=dh;bz<=fm;bz++){
var iY=(bz<fm)?e[bz*4+1]:0;while((iY>ap)&&(ap<(125+2))){b5.push(aQ);aQ=bz;ap++;}
while((iY<ap)&&(ap>0)){var jb=aQ;aQ=b5.pop();ap--;if((bz-jb)>1){var o8=jb-dh;var
o9=bz-dh-1;if((o8===eP)&&(o9===eO)){eP=0;eO=0;continue;}for(;eP<eO;eP++,eO--){var
g=cZ[eP+fJ];cZ[eP+fJ]=cZ[eO+fJ];cZ[eO+fJ]=g;}eP=o8;eO=o9;}}}for(;eP<eO;eP++,eO--
){var g=cZ[eP+fJ];cZ[eP+fJ]=cZ[eO+fJ];cZ[eO+fJ]=g;}};d.sE=function(an,fR,gM,bg){
var b5=[];var e=an?an.Data:null;var dh=fR+3;var fm=gM+3;var aQ=dh;var ap=0;var bz;
if(!e)return bg;for(bz=dh;bz<=fm;bz++){var iY=(bz<fm)?e[bz*4+1]:0;while((iY>ap)&&(
ap<(125+2))){b5.push(aQ);aQ=bz;ap++;}while((iY<ap)&&(ap>0)){var jb=aQ;aQ=b5.pop(
);ap--;if((bz-jb)>1){var d0=jb-dh;var ek=bz-dh-1;if((bg>=d0)&&(bg<=ek))bg=ek-(bg-
d0);}}}return bg;};d.sF=function(an,fR,gM,bg){var mp=[];var l4=[];var e=an?an.Data:
null;var dh=fR+3;var aQ=0;var cT=gM-fR-1;var ap=0;var s=0;if(!e)return bg;if(cT>
65535)return bg;while(aQ<cT){var on=e[(dh+aQ)*4+1];var om=e[(dh+cT)*4+1];if((on>
ap)&&(om>ap)&&(ap<(125+2))){mp.push(aQ);l4.push(cT);ap++;}if(on<=ap)aQ++;if(om<=
ap)cT--;}while((s<ap)&&(bg>=mp[s])&&(bg<=l4[s])){var rk=mp[s];var rj=l4[s];var d0=
bg;var ek=bg;while((d0>rk)&&(e[(dh+d0-1)*4+1]>s))d0--;while((ek<rj)&&(e[(dh+ek+1
)*4+1]>s))ek++;bg=ek-(bg-d0);s++;}return bg;};d.sC=function(an,rg,q3){var e=an?an.
Data:null;var bl=an?an.fd:0;var s=0;while(s<bl){rg[s]=e[s*4+12];q3[s]=e[s*4+13];
s++;}};d.sS=function(aa,ah,af,nv,nw,a9,bf,aA){var a=this.bj;var ae=null;var fo=nv[
0];var fp=nv[1];var hM=nw[0];var hN=nw[1];var a3=hM;var a6=hN;var dq=hM-fo;var hR=
hN-fp;var k,j,r,p;if(aa&&(ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.
b2[ah];}if(!ae||(ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(Math.abs(hM-fo)>Math.abs(hN-fp)){if(a3>fo)a3--;if(a3<fo)a3++;}else{
if(a6>fp)a6--;if(a6<fp)a6++;}k=Math.max(af[0],Math.min(fo,a3),0);j=Math.max(af[1
],Math.min(fp,a6),0);r=Math.min(af[2],Math.max(fo,a3)+1,aa.FrameSize[0]);p=Math.
min(af[3],Math.max(fp,a6)+1,aa.FrameSize[1]);if((r<=k)||(p<=j))return;var cR=ae.
aM[0];var cS=ae.aM[1];var b=a.g_;var aL=a.h$;var ai=2;b[0]=fo+cR+0.5;b[1]=fp+cS+
0.5;b[6]=hM+cR+0.5;b[7]=hN+cS+0.5;aL[5]=a9;aL[11]=bf;if(dq>0)r+=0.5;if(dq<0)k-=0.5;
if(hR>0)p+=0.5;if(hR<0)j-=0.5;if((fo<k)||(fo>r)||(fp<j)||(fp>p)||(hM<k)||(hM>r)||(
hN<j)||(hN>p)){ai=js(b,aL,ai,true,k+cR);ai=js(b,aL,ai,true,r+cR);ai=mJ(b,ai,k+cR
,r+cR);ai=jr(b,aL,ai,true,j+cS);ai=jr(b,aL,ai,true,p+cS);ai=mI(b,ai,j+cS,p+cS);if(
ai!==2)return;}dH(a,ae.aJ,a.hh);if(a.b6&&!aA)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(
a.BLEND);a.b6=aA;a.bufferSubData(a.ARRAY_BUFFER,0,a.cC);a.drawArrays(a.LINES,0,ai
);if(this.a8)ex(a);};d.hn=function(aa,ah,af,u,av,ar,aq,au,aA){var a=this.bj;var ae=
null;var k,j,r,p;if(aa&&(ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.
b2[ah];}if(!ae||(ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}k=Math.max(af[0],u[0],0);j=Math.max(af[1],u[1],0);r=Math.min(af[2],u[2
],aa.FrameSize[0]);p=Math.min(af[3],u[3],aa.FrameSize[1]);var clear=!av&&!ar&&!au&&
!aq;if((r<=k)||(p<=j)||(clear&&aA))return;if(clear){var b=a.g_;b[0]=b[12]=k+ae.aM[
0];b[1]=b[7]=j+ae.aM[1];b[6]=b[18]=r+ae.aM[0];b[19]=b[13]=p+ae.aM[1];if(a.b6&&!aA
)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND);a.b6=aA;dH(a,ae.aJ,clear?a.id:
a.hh);a.bufferSubData(a.ARRAY_BUFFER,0,a.cC);a.drawArrays(a.TRIANGLE_STRIP,0,4);
if(this.a8)ex(a);return;}var gm=(av!==au)||(ar!==aq);var f$=(av!==ar)||(au!==aq);
var eK=gm||f$;var sq=gm&&((j!==u[1])||(p!==u[3]));var r3=f$&&((k!==u[0])||(r!==u[
2]));if(sq){var bJ=1/(u[3]-u[1]);var bm=(j-u[1])*bJ;var a1=(u[3]-p)*bJ;if(av!==au
){var c5=av;if(bm)av=aS(c5,au,bm);if(a1)au=aS(au,c5,a1);}if(ar!==aq){var f3=ar;if(
bm)ar=aS(f3,aq,bm);if(a1)aq=aS(aq,f3,a1);}}if(r3){var bJ=1/(u[2]-u[0]);var cm=(k-
u[0])*bJ;var ay=(u[2]-r)*bJ;if(av!==ar){var c5=av;if(cm)av=aS(c5,ar,cm);if(ay)ar=
aS(ar,c5,ay);}if(au!==aq){var fY=au;if(cm)au=aS(fY,aq,cm);if(ay)aq=aS(aq,fY,ay);
}}var b=a.g_;var aL=a.h$;b[0]=b[12]=k+ae.aM[0];b[1]=b[7]=j+ae.aM[1];b[6]=b[18]=r+
ae.aM[0];b[19]=b[13]=p+ae.aM[1];aL[5]=av;aL[11]=ar;aL[23]=aq;aL[17]=au;if(a.b6&&
!aA)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND);a.b6=aA;dH(a,ae.aJ,clear?a.
id:a.hh);a.bufferSubData(a.ARRAY_BUFFER,0,a.cC);a.drawArrays(a.TRIANGLE_STRIP,0,
4);if(this.a8)ex(a);};d.sR=function(aa,ah,af,u,fg,av,ar,aq,au,aA){if(fg<=0)return;
if(((fg*2)>=(u[2]-u[0]))||((fg*2)>=(u[3]-u[1]))){this.hn(aa,ah,af,u,av,ar,aq,au,
aA);return;}var cz=[0,0,0,0];var fX;cz[0]=u[0];cz[1]=u[1];cz[2]=u[2];cz[3]=u[1]+
fg;fX=this.il(af,cz);this.hn(aa,ah,fX,u,av,ar,aq,au,aA);cz[0]=u[0];cz[1]=u[3]-fg;
cz[2]=u[2];cz[3]=u[3];fX=this.il(af,cz);this.hn(aa,ah,fX,u,av,ar,aq,au,aA);cz[0]=
u[0];cz[1]=u[1]+fg;cz[2]=u[0]+fg;cz[3]=u[3]-fg;fX=this.il(af,cz);this.hn(aa,ah,fX
,u,av,ar,aq,au,aA);cz[0]=u[2]-fg;cz[2]=u[2];fX=this.il(af,cz);this.hn(aa,ah,fX,u
,av,ar,aq,au,aA);};d.nd=function(lL){if(lL<1)return null;return{ib:lL,pY:0,pX:0,
aI:0,b3:true,bC:[0,0,0,0],dS:[1.0,0.0,0.0,0.0,1.0,0.0],k7:[],b7:new Array(lL)};};
function jC(m,i){var at=m.charCodeAt(i);while((at===0x20)||(at===0x09)||(at===0x0A
)||(at===0x0D)||(at===0x2C)||(at===0x0C))at=m.charCodeAt(++i);return i;}function
bi(cO,as,dA){var e8=cO[0];var aQ=jC(e8,cO[1]);var t=aQ;var gf=0.0;var exp=0.1;var
ai=0;var i$=1;var at=e8.charCodeAt(t);if((at===0x2B)||(at===0x2D)){i$=(at===0x2D
)?-1:1;at=e8.charCodeAt(++t);}while((at>=0x30)&&(at<=0x39)){gf=gf*10+(at-0x30);at=
e8.charCodeAt(++t);ai++;}if(at===0x2E)at=e8.charCodeAt(++t);while((at>=0x30)&&(at<=
0x39)){gf+=exp*(at-0x30);exp*=0.1;at=e8.charCodeAt(++t);ai++;}if(!ai){cO[1]=aQ;return false;
}cO[1]=t;as[dA]=(i$<0)?-gf:gf;return true;}function m3(cO,as,dA){var e8=cO[0];var
aQ=jC(e8,cO[1]);var t=aQ;var at=e8.charCodeAt(t);if((at===0x30)||(at==0x31)){as[
dA]=(at===0x31);at=e8.charCodeAt(++t);}cO[1]=t;return t>aQ;}function e$(n,a$,bG){
var v=n.b7[a$];var se=v.go-v.aI;if(se>=bG)return a$;var mf=(a$+1)%2;this.ik(n,mf
,v.aI+bG+256);this.jG(n,mf,n,a$,0,aZ);return mf;}d.sK=function(cO,hD,iL,iy,hw,iu
){var o_=[cO,jC(cO,0)];var t=[cO,o_[1]];var oc=0;var i7=(Math.abs(hD)+Math.abs(iL
))/16;var kH=false;var ai=0;var iS=0.0;var aW=0.0;var aX=0.0;var l=[0.0,0.0,0.0,
0.0];var x=[0.0,0.0,0.0,0.0];var iS=[0.0];var cA=[false,false,false];var at=cO.charCodeAt(
t[1]);var aZ;function eR(nN,j2,j1,nO){return Math.sqrt((nN-j1)*(nN-j1)+(j2-nO)*(
j2-nO));}this.ht=this.qP;this.ir=-1;if(!at){this.ht=this.qN;return null;}if((at!==
0x6D)&&(at!==0x4D)){this.ht=this.nn;this.ir=t[1];return null;}for(;at;at=cO.charCodeAt(++
t[1])){at=((at>=0x61)&&(at<=0x7A))?at-0x20:at;if(at===0x4D){kH=false;ai++;}else if(
at===0x5A)kH=true;else if(kH&&((at===0x4C)||(at===0x48)||(at===0x56)||(at===0x43
)||(at===0x53)||(at===0x51)||(at===0x54)||(at===0x41))){ai++;kH=false;}}var d3=this.
nd(ai);var bh=this.nd(2);var ba=0;if((iy!==0.0)||(hw!==0.0)||(hD!==1.0)||(hw!==1.0
)||(iu!==0.0)){this.nq(d3,iy,hw);this.nm(d3,iu);this.qQ(d3,hD,iL);}this.ik(bh,0,
256);for(ai=0,t[1]=o_[1];cO.charCodeAt(t[1]);ai++){var eN=0;var bp=0;var ob=0;this.
m8(bh,ba,aW,aX);for(;cO.charCodeAt(t[1]);ob++){eN=bp;bp=cO.charCodeAt(t[1]);if(ob&&((
bp===0x6D)||(bp===0x4D)))break;t[1]++;if((bp===0x6D)||(bp===0x4D)){if(!bi(t,l,1)||
!bi(t,x,1))return cY(this);if(bp===0x4D){aW=l[1];aX=x[1];}else{aW+=l[1];aX+=x[1];
}this.m8(bh,ba,aW,aX);while(bi(t,l,1)){if(!bi(t,x,1))return cY(this);ba=e$(bh,ba
,1);if(bp===0x4D){aW=l[1];aX=x[1];}else{aW+=l[1];aX+=x[1];}this.jH(bh,ba,aW,aX);
}}else if((bp===0x6C)||(bp===0x4C)){if(!bi(t,l,1))return cY(this);do{if(!bi(t,x,
1))return cY(this);ba=e$(bh,ba,1);if(bp===0x4C){aW=l[1];aX=x[1];}else{aW+=l[1];aX+=
x[1];}this.jH(bh,ba,aW,aX);}while(bi(t,l,1));}else if((bp===0x68)||(bp===0x48)){
if(!bi(t,l,1))return cY(this);do{ba=e$(bh,ba,1);if(bp===0x48)aW=l[1];else aW+=l[
1];this.jH(bh,ba,aW,aX);}while(bi(t,l,1));}else if((bp===0x76)||(bp===0x56)){if(
!bi(t,x,1))return cY(this);do{ba=e$(bh,ba,1);if(bp===0x56)aX=x[1];else aX+=x[1];
this.jH(bh,ba,aW,aX);}while(bi(t,x,1));}else if((bp===0x63)||(bp==0x43)){if(!bi(
t,l,1))return cY(this);do{if(!bi(t,x,1)||!bi(t,l,2)||!bi(t,x,2)||!bi(t,l,3)||!bi(
t,x,3))return cY(this);if(bp===0x63)l[1]+=aW,x[1]+=aX,l[2]+=aW,x[2]+=aX,l[3]+=aW
,x[3]+=aX;aZ=((eR(l[1],x[1],aW,aX)+eR(l[2],x[2],l[1],x[1])+eR(l[3],x[3],l[2],x[2
]))*i7)|0;ba=e$(bh,ba,aZ+1);this.m6(bh,ba,l[1],x[1],l[2],x[2],l[3],x[3],aZ+1);aW=
l[3];aX=x[3];}while(bi(t,l,1));}else if((bp===0x73)||(bp===0x53)){l[1]=l[2];x[1]=
x[2];if(!bi(t,l,2))return cY(this);do{if(!bi(t,x,2)||!bi(t,l,3)||!bi(t,x,3))return cY(
this);if(bp===0x73)l[2]+=aW,x[2]+=aX,l[3]+=aW,x[3]+=aX;if((eN===0x43)||(eN===0x63
)||(eN===0x53)||(eN===0x73)){l[1]=aW-(l[1]-aW);x[1]=aX-(x[1]-aX);}else l[1]=aW,x[
1]=aX;aZ=((eR(l[1],x[1],aW,aX)+eR(l[2],x[2],l[1],x[1])+eR(l[3],x[3],l[2],x[2]))*
i7)|0;ba=e$(bh,ba,aZ+1);this.m6(bh,ba,l[1],x[1],l[2],x[2],l[3],x[3],aZ+1);aW=l[3
];aX=x[3];k=l[2];j=x[2];eN=bp;}while(bi(t,l,2));}else if((bp===0x71)||(bp===0x51
)){if(!bi(t,l,1))return cY(this);do{if(!bi(t,x,1)||!bi(t,l,2)||!bi(t,x,2))return cY(
this);if(bp===0x71)l[1]+=aW,x[1]+=aX,l[2]+=aW,x[2]+=aX;aZ=((eR(l[1],x[1],aW,aX)+
eR(l[2],x[2],l[1],x[1]))*i7)|0;ba=e$(bh,ba,aZ+1);this.m5(bh,ba,l[1],x[1],l[2],x[
2],aZ+1);aW=l[2];aX=x[2];}while(bi(t,l,1));}else if((bp===0x74)||(bp===0x54)){if(
!bi(t,l,2))return cY(this);do{if(!bi(t,x,2))return cY(this);if(bp===0x74)l[2]+=aW
,x[2]+=aX;if((eN===0x51)||(eN===0x71)||(eN===0x54)||(eN===0x74)){l[1]=aW-(l[1]-aW
);x[1]=aX-(x[1]-aX);}else l[1]=aW,x[1]=aX;aZ=((eR(l[1],x[1],aW,aX)+eR(l[2],x[2],
l[1],x[1]))*i7)|0;ba=e$(bh,ba,aZ+1);this.m5(bh,ba,l[1],x[1],l[2],x[2],aZ+1);aW=l[
2];aX=x[2];eN=bp;}while(bi(t,l,2));}else if((bp===0x61)||(bp===0x41)){oc=t[1]-1;
if(!bi(t,l,0))return cY(this);do{l[1]=aW;x[1]=aX;if(!bi(t,x,0)||!bi(t,iS,0)||!m3(
t,cA,1)||!m3(t,cA,2)||!bi(t,l,2)||!bi(t,x,2))return cY(this);if(bp===0x61)l[2]+=
aW,x[2]+=aX;if((l[0]<=0)||(x[0]<=0))return jB(this);var kM=Math.sin(iS[0]*this.ii
);var j$=Math.cos(iS[0]*this.ii);var ou=(l[1]-l[2])/2;var ov=(x[1]-x[2])/2;var r4=(
l[1]+l[2])/2;var r5=(x[1]+x[2])/2;var jl=j$*ou+kM*ov;var jm=j$*ov-kM*ou;var l$=(
jl*jl)/(l[0]*l[0])+(jm*jm)/(x[0]*x[0]);if(l$>1){l[0]=l[0]*Math.sqrt(l$);x[0]=x[0
]*Math.sqrt(l$);}var o4=l[0]*x[0];var mm=l[0]*jm;var mn=x[0]*jl;var mq=mm*mm+mn*
mn;if(mq===0.0)return jB(this);var abs=(o4*o4-mq)/mq;if(abs<0.0)abs=-abs;var ki=
Math.sqrt(abs);if(cA[1]===cA[2])ki=-ki;var kp=ki*mm/x[0];var kq=-ki*mn/l[0];var rK=
j$*kp-kM*kq+r4;var rN=kM*kp+j$*kq+r5;var g$=(jl-kp)/l[0];var kT=(jl+kp)/l[0];var
ha=(jm-kq)/x[0];var kU=(jm+kq)/x[0];var g=g$*g$+ha*ha;if(g===0.0)return jB(this);
var kP=Math.acos(g$/Math.sqrt(g))*this.ni;if(ha<0.0)kP=-kP;g=(g$*g$+ha*ha)*(kT*kT+
kU*kU);if(g===0.0)return jB(this);var di=Math.acos((g$*-kT+ha*-kU)/Math.sqrt(g))
*this.ni;if((g$*-kU-ha*-kT)<0)di=-di;while(di>360){di=di-360;}while(di<-360){di=
di+360;}if(!cA[2]&&(di>0))di=di-360;if(cA[2]&&(di<0))di=di+360;aZ=((l[0]+x[0])*di
*(3.14/360)*i7)|0;if(aZ<0)aZ=-aZ;ba=e$(bh,ba,aZ+1);this.qM(bh);this.nq(bh,rK,rN);
this.nm(bh,iS);this.qi(bh,ba,0,0,l[0],x[0],kP,kP+di,aZ+1);this.qK(bh);aW=l[2];aX=
x[2];}while(bi(t,l,0));}else if((bp===0x5A)||(bp==0x7A)){t[1]=jC(cO,t[1]);this.nb(
bh,ba);aW=this.qp(bh,ba,0);aX=this.qq(bh,ba,0);break;}else{t[1]--;return cY(this
);}}aZ=this.lf(bh,ba);this.ik(d3,ai,aZ);this.jG(d3,ai,bh,ba,0,aZ);if(this.qv(bh,
ba))this.nb(d3,ai);}this.le(bh);return d3;function cY(aj){aZ=aj.lf(bh,ba);aj.ik(
d3,ai,aZ);aj.jG(d3,ai,bh,ba,0,aZ);aj.ht=aj.nn;aj.ir=t[1];aj.le(bh);return d3;}function
jB(aj){aZ=aj.lf(bh,ba);aj.ik(d3,ai,aZ);aj.jG(d3,ai,bh,ba,0,aZ);aj.ht=aj.qO;aj.ir=
oc;aj.le(bh);return d3;}};d.tb=function(){return this.ht;};d.ta=function(){return this.
ir;};d.le=function(n){if(n){delete n.b7;delete n.k7;}};d.s3=function(n){if(!n)return 0;
return n.ib;};d.ik=function(n,a$,iA){if(!n||(a$>=n.ib)||(a$<0))return false;if(iA<=
0)iA=0;var v=null;if(iA)v={go:iA,aI:0,fz:false,eu:false,cX:false,b3:true,bC:[0,0
,0,0],Data:new Float32Array((iA+1)*8)};if(n.b7[a$])n.b3=false;n.b7[a$]=v;return true;
};d.m8=function(n,a$,a2,bu){var v=n?n.b7[a$]:null;if(!v)return;var q=n.dS;var l=
a2;var x=bu;if((q[0]!==1)||(q[1]!==0)||(q[2]!==0)||(q[3]!==0)||(q[4]!==1)||(q[5]
!==0)){l=a2*q[0]+bu*q[1]+q[2];x=a2*q[3]+bu*q[4]+q[5];}v.cX=false;v.eu=false;v.b3=
false;v.aI=0;v.fz=true;v.Data[0]=l;v.Data[1]=x;n.b3=false;};d.jH=function(n,a$,a2
,bu){var v=n?n.b7[a$]:null;if(!v||v.cX||((v.aI+1)>v.go))return 0;var q=n.dS;var aK=
v.Data;var bF=v.aI*2+2;var l=a2;var x=bu;v.fz=true;v.eu=true;v.b3=false;n.b3=false;
if((q[0]!==1)||(q[1]!==0)||(q[2]!==0)||(q[3]!==0)||(q[4]!==1)||(q[5]!==0)){l=a2*
q[0]+bu*q[1]+q[2];x=a2*q[3]+bu*q[4]+q[5];}aK[bF]=l;aK[bF+1]=x;return v.aI++;};d.
m5=function(n,a$,lB,lC,a2,bu,bG){var v=n?n.b7[a$]:null;if((bG<=0)||!v||v.cX||((v.
aI+bG)>v.go))return 0;var q=n.dS;var aK=v.Data;var bF=v.aI*2;var aW=lB;var aX=lC;
var l=a2;var x=bu;var dO=1/bG;var bm;var h;v.fz=true;v.eu=true;v.b3=false;n.b3=false;
if((q[0]!==1)||(q[1]!==0)||(q[2]!==0)||(q[3]!==0)||(q[4]!==1)||(q[5]!==0)){aW=lB
*q[0]+lC*q[1]+q[2];aX=lB*q[3]+lC*q[4]+q[5];l=a2*q[0]+bu*q[1]+q[2];x=a2*q[3]+bu*q[
4]+q[5];}var mA=aK[bF++];var mB=aK[bF++];for(bm=dO,h=1;h<bG;h++,bm+=dO){var ge=1-
bm;var i4=ge*ge;var bW=bm*bm;var i2=2*ge*bm;aK[bF++]=i4*mA+i2*aW+bW*l;aK[bF++]=i4
*mB+i2*aX+bW*x;}aK[bF++]=l;aK[bF++]=x;return(v.aI+=h)-h;};d.m6=function(n,a$,lx,
ly,lz,lA,a2,bu,bG){var v=n?n.b7[a$]:null;if((bG<=0)||!v||v.cX||((v.aI+bG)>v.go))
return 0;var q=n.dS;var aK=v.Data;var bF=v.aI*2;var oh=lx;var oi=ly;var oj=lz;var
ok=lA;var l=a2;var x=bu;var dO=1/bG;var bm;var h;v.fz=true;v.eu=true;v.b3=false;
n.b3=false;if((q[0]!==1)||(q[1]!==0)||(q[2]!==0)||(q[3]!==0)||(q[4]!==1)||(q[5]!==
0)){oh=lx*q[0]+ly*q[1]+q[2];oi=lx*q[3]+ly*q[4]+q[5];oj=lz*q[0]+lA*q[1]+q[2];ok=lz
*q[3]+lA*q[4]+q[5];l=a2*q[0]+bu*q[1]+q[2];x=a2*q[3]+bu*q[4]+q[5];}var mA=aK[bF++
];var mB=aK[bF++];for(bm=dO,h=1;h<bG;h++,bm+=dO){var ge=1-bm;var bW=bm*bm;var i4=
ge*ge;var or=ge*i4;var i2=3*i4*bm;var os=3*ge*bW;var ot=bm*bW;aK[bF++]=or*mA+i2*
oh+os*oj+ot*l;aK[bF++]=or*mB+i2*oi+os*ok+ot*x;}aK[bF++]=l;aK[bF++]=x;return(v.aI+=
h)-h;};d.qi=function(n,a$,qV,qW,q8,q9,iO,jT,bG){var v=n?n.b7[a$]:null;var aZ=bG;
if(v&&v.fz)aZ++;if((bG<=0)||(iO===jT)||!v||v.cX||((v.aI+aZ)>v.go))return 0;iO*=this.
ii;jT*=this.ii;var q=n.dS;var aK=v.Data;var bF=v.aI*2+(aZ-bG)*2;var dO=(jT-iO)/bG;
var h;v.fz=true;v.eu=true;v.b3=false;n.b3=false;var rR=(q[0]!==1)||(q[1]!==0)||(
q[2]!==0)||(q[3]!==0)||(q[4]!==1)||(q[5]!==0);for(h=0;h<=bG;h++,iO+=dO){var bR=(
h===bG)?jT:iO;var l=qV+q8*Math.cos(bR);var x=qW+q9*Math.sin(bR);if(rR){aK[bF++]=
l*q[0]+x*q[1]+q[2];aK[bF++]=l*q[3]+x*q[4]+q[5];}else{aK[bF++]=l;aK[bF++]=x;}}return(
v.aI+=aZ)-bG;};d.jG=function(n,a$,nI,rc,j0,bG){var eE=n?n.b7[a$]:null;var kO=nI?
nI.b7[rc]:null;var aZ=bG;if(!bG||!eE||!kO||(j0<0))return 0;if(bG<0)aZ=bG=kO.aI-j0;
if((j0+aZ)>kO.aI)return 0;if(eE.fz)aZ++;if(((eE.aI+aZ)>eE.go)||eE.cX)return 0;eE.
fz=true;eE.eu=true;eE.b3=false;n.b3=false;var q=n.dS;var aK=eE.Data;var dO=kO.Data;
var bF=eE.aI*2+(aZ-bG)*2;var ay=j0*2;var h=0;if((q[0]!==1)||(q[1]!==0)||(q[2]!==
0)||(q[3]!==0)||(q[4]!==1)||(q[5]!==0))for(;h<=bG;h++,ay+=2){aK[bF++]=dO[ay]*q[0
]+dO[ay+1]*q[1]+q[2];aK[bF++]=dO[ay]*q[3]+dO[ay+1]*q[4]+q[5];}else for(;h<=bG;h++
){aK[bF++]=dO[ay++];aK[bF++]=dO[ay++];}return(eE.aI+=aZ)-bG;};d.nb=function(n,a$
){var v=n?n.b7[a$]:null;if(!v||v.cX||!v.eu)return;v.eu=false;v.cX=true;n.b3=false;
var aK=v.Data;var cR=aK[v.aI*2+0]-aK[0];var cS=aK[v.aI*2+1]-aK[1];if((cR>0.001)||(
cR<-0.001)||(cS>0.001)||(cS<-0.001)){v.aI++;aK[v.aI*2+0]=aK[0];aK[v.aI*2+1]=aK[1
];}else{aK[v.aI*2+0]=aK[0];aK[v.aI*2+1]=aK[1];}};d.t6=function(n,a$,gI,nK,nL){var
v=n?n.b7[a$]:null;if(!v||v.cX||!v.eu)return 0;if(gI<=0)return 0;if(gI>v.aI)gI=v.
aI;v.Data.copyWithin(0,gI*2);if((nK!==0.0)||(nL!==0.0)){var aK=v.Data;var h;for(
h=0;h<aK.length;h+=2){aK[h]+=nK;aK[h+1]+=nL;}}v.eu=gI<v.aI;v.aI=v.aI-gI;v.b3=false;
n.b3=false;return gI;};d.t5=function(n,a$,dK,a2,bu){var v=n?n.b7[a$]:null;if(!v||(
dK>v.aI)||(dK<0))return;var q=n.dS;var aK=v.Data;var l=a2;var x=bu;if((q[0]!==1)||(
q[1]!==0)||(q[2]!==0)||(q[3]!==0)||(q[4]!==1)||(q[5]!==0)){l=a2*q[0]+bu*q[1]+q[2
];x=a2*q[3]+bu*q[4]+q[5];}aK[dK*2+0]=l;aK[dK*2+1]=x;if(!dK&&v.cX){aK[v.aI*2+0]=l;
aK[v.aI*2+1]=x;}if((dK===v.aI)&&v.cX){aK[0]=l;aK[1]=x;}v.b3=false;n.b3=false;};d.
qp=function(n,a$,dK){var v=n?n.b7[a$]:null;if(!v||(dK>v.aI)||(dK<0))return 0.0;return v.
Data[dK*2+0];};d.qq=function(n,a$,dK){var v=n?n.b7[a$]:null;if(!v||(dK>v.aI)||(dK<
0))return 0.0;return v.Data[dK*2+1];};d.lf=function(n,a$){var v=n?n.b7[a$]:null;
return v?v.aI:0;};d.s6=function(n,a$){var v=n?n.b7[a$]:null;if(!v||v.cX)return 0;
return v.go-v.aI;};d.qv=function(n,a$){var v=n?n.b7[a$]:null;return v?v.cX:false;
};d.tc=function(n,a$){var v=n?n.b7[a$]:null;this.hv(n);return v?v.bC:[0,0,0,0];};
d.s7=function(n){this.hv(n);return n?n.bC:[0,0,0,0];};d.nq=function(n,iy,hw){if(
!n)return;var q=n.dS;q[2]=q[2]+(q[0]*iy)+(q[1]*hw);q[5]=q[5]+(q[3]*iy)+(q[4]*hw);
};d.qQ=function(n,hD,iL){if(!n)return;var q=n.dS;q[0]*=hD;q[1]*=iL;q[3]*=hD;q[4]
*=iL;};d.nm=function(n,iu){if(!n)return;var q=n.dS;var sin=Math.sin(iu*=this.ii);
var cos=Math.cos(iu);var oK,oL,oM,oN;oK=(q[0]*cos)+(q[1]*sin);oL=(q[0]*-sin)+(q[
1]*cos);oM=(q[3]*cos)+(q[4]*sin);oN=(q[3]*-sin)+(q[4]*cos);q[0]=oK;q[1]=oL;q[3]=
oM;q[4]=oN;};d.qM=function(n){if(!n)return;var q=n.dS;n.k7.push([q[0],q[1],q[2],
q[3],q[4],q[5]]);};d.qK=function(n){if(!n)return;var q=n.dS;var g1=n.k7.pop();if(
g1){q[0]=g1[0];q[1]=g1[1];q[2]=g1[2];q[3]=g1[3];q[4]=g1[4];q[5]=g1[5];}else{q[0]=
1;q[1]=0;q[2]=0;q[3]=0;q[4]=1;q[5]=0;}};d.ti=function(n){if(!n)return;var q=n.dS;
q[0]=1;q[1]=0;q[2]=0;q[3]=0;q[4]=1;q[5]=0;};d.hv=function(n){if(!n||n.b3)return;
var h6=n.b7;var hX=n.ib;var lT=[0,0,0,0];var oR=0;var oO=0;var aZ=0;var h;for(h=
0;h<hX;h++){var v=h6[h];if(v&&!v.b3&&v.aI){var aK=v.Data;var bF=2;var k=aK[0];var
j=aK[1];var r=k;var p=j;var ai=v.aI;for(;ai>0;ai--,bF+=2){var l=aK[bF];var x=aK[
bF+1];if(l<k)k=l;if(l>r)r=l;if(x<j)j=x;if(x>p)p=x;}v.bC=[k|0,j|0,(r+1)|0,(p+1)|0
];v.b3=true;}if(v&&v.b3){oR+=v.eu?1:0;oO+=v.cX?1:0;aZ+=v.aI;lT=this.qR(lT,v.bC);
}}n.pY=oR;n.pX=oO;n.aI=aZ;n.bC=lT;n.b3=true;};d.qn=function(bo,n,dV,ak,iv,lN){var
aY=ak[0];var aG=ak[1];var k,j,r,p;if((bo[0]<=0)||(bo[1]<=0)||!n)return null;this.
hv(n);if(!dV){j=aG+n.bC[1]-1;p=aG+n.bC[3]+1;}else{aG+=bo[1];j=aG-n.bC[3]-1;p=aG-
n.bC[1]+1;}k=aY+n.bC[0]-1;r=aY+n.bC[2]+1;k=Math.max(k,0);j=Math.max(j,0);r=Math.
min(r,bo[0]);p=Math.min(p,bo[1]);if((r<=k)||(p<=j)||!n.aI)return null;var a=this.
bj;var aF=document.createElement("canvas");var en=a.createTexture();var aw={};var
ct={aJ:aw,aM:[0,0]};var bitmap={FrameSize:bo,FrameDelay:0,NoOfFrames:1,cH:[aw],b2:[
ct]};aF.width=bo[0];aF.height=bo[1];var aU=aF.getContext("2d");var h6=n.b7;var hX=
n.ib;var h;aU.translate(aY,aG);aU.scale(1,dV?-1:1);aU.beginPath();for(h=0;h<hX;h++
){var v=h6[h];if(v&&v.aI){var e=v.Data;var mh=v.aI*2+2;var ca;aU.moveTo(e[0],e[1
]);for(ca=2;ca<mh;ca+=2)aU.lineTo(e[ca],e[ca+1]);if(v.cX)aU.closePath();}}aU.fill(
lN?"nonzero":"evenodd");a.fD=null;a.bindTexture(a.TEXTURE_2D,en);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_MIN_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MAG_FILTER
,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.CLAMP_TO_EDGE);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_WRAP_T,a.CLAMP_TO_EDGE);a.texImage2D(a.TEXTURE_2D,0,a.RGBA
,a.RGBA,a.UNSIGNED_BYTE,aF);aw.Format=d.lp;aw.Width=bo[0];aw.Height=bo[1];aw.ig=
bo[0];aw.ie=bo[1];aw.d_=1/bo[0];aw.d9=1/bo[1];aw.eT=2/bo[0];aw.eS=2/bo[1];aw.cL=
en;aw.Framebuffer=null;return bitmap;};d.qo=function(bo,n,dV,ak,aD,fS,gG,iv){var
dp=fS&0x0000FF;var dn=fS&0x00FF00;var e9=fS&0xFF0000;var aY=ak[0];var aG=ak[1];var
k,j,r,p;var cu;if((bo[0]<=0)||(bo[1]<=0)||!n||(aD<=0))return null;if((dp===this.
jP)&&(aD<4))dp=this.ll;if((dn===this.jO)&&(aD<4))dn=this.lj;if((e9===this.lo)&&(
aD<3))e9=this.lm;if(e9===this.ln){if(gG>1)cu=(gG*aD*0.5)|0;else cu=(aD*0.5)|0;}else
if((dp===this.iq)||(dn===this.ip))cu=(aD*0.75)|0;else cu=(aD+0.5)|0;this.hv(n);if(
!dV){j=aG+n.bC[1]-cu-1;p=aG+n.bC[3]+cu+1;}else{aG+=bo[1];j=aG-n.bC[3]-cu-1;p=aG-
n.bC[1]+cu+1;}k=aY+n.bC[0]-cu-1;r=aY+n.bC[2]+cu+1;k=Math.max(k,0);j=Math.max(j,0
);r=Math.min(r,bo[0]);p=Math.min(p,bo[1]);if((r<=k)||(p<=j)||!n.aI)return;var a=
this.bj;var aF=document.createElement("canvas");var en=a.createTexture();var aw={
};var ct={aJ:aw,aM:[0,0]};var bitmap={FrameSize:bo,FrameDelay:0,NoOfFrames:1,cH:[
aw],b2:[ct]};aF.width=bo[0];aF.height=bo[1];var aU=aF.getContext("2d");var h6=n.
b7;var hX=n.ib;var h;aU.translate(aY,aG);aU.scale(1,dV?-1:1);aU.beginPath();for(
h=0;h<hX;h++){var v=h6[h];if(v&&v.aI){var e=v.Data;var mh=v.aI*2+2;var ca;aU.moveTo(
e[0],e[1]);for(ca=2;ca<mh;ca+=2)aU.lineTo(e[ca],e[ca+1]);if(v.cX)aU.closePath();
}}if(aD!==1.0)aU.lineWidth=aD;if(e9===this.ln)aU.miterLimit=gG;else if(e9===this.
lm)aU.lineJoin="bevel";else if(e9===this.lo)aU.lineJoin="round";if((dp===this.jP
)&&(dn===this.jO)){dp=dn=this.lk;aU.lineCap="round";}else if((dp===this.iq)&&(dn===
this.ip)){dp=dn=this.lk;aU.lineCap="square";}aU.stroke();if((dp!==this.lk)||(dn!==
this.qJ)){aU.beginPath();for(h=0;h<hX;h++){var v=h6[h];if(v&&v.aI&&!v.cX){var e=
v.Data;var h2=0;var h1=v.aI*2+1;var eq=e[h2++];var es=e[h2++];var er=e[h1--];var
ep=e[h1--];if((dp===this.iq)||(dp===this.ll)){var cE,cG;do{cE=e[h2++];cG=e[h2++];
}while((cE===eq)&&(cG===es)&&(h2<h1));cE=eq-cE;cG=es-cG;if(cE||cG){var bl=Math.sqrt(
cE*cE+cG*cG);var d4,d5;d4=cE/bl;cE=d4*aD*0.5;d5=cG/bl;cG=d5*aD*0.5;if(dp===this.
iq){aU.moveTo(eq+cG-d4,es-cE-d5);aU.lineTo(eq-cG-d4,es+cE-d5);aU.lineTo(eq-cG+cE
,es+cE+cG);aU.lineTo(eq+cG+cE,es-cE+cG);}else{aU.moveTo(eq+cG,es-cE);aU.lineTo(eq+
cG-d4,es-cE-d5);aU.lineTo(eq-cG-d4,es+cE-d5);aU.lineTo(eq-cG,es+cE);aU.lineTo(eq+
cE,es+cG);}aU.closePath();}}else if(dp===this.jP)aU.arc(eq,es,aD/2,0,2*Math.PI);
if((dn===this.ip)||(dn===this.lj)){var cD,cF;do{cF=e[h1--];cD=e[h1--];}while((cD===
ep)&&(cF===er)&&(h2<h1));cD=ep-cD;cF=er-cF;if(cD||cF){var bl=Math.sqrt(cD*cD+cF*
cF);var d4,d5;d4=cD/bl;cD=d4*aD*0.5;d5=cF/bl;cF=d5*aD*0.5;if(dn===this.ip){aU.moveTo(
ep+cF-d4,er-cD-d5);aU.lineTo(ep-cF-d4,er+cD-d5);aU.lineTo(ep-cF+cD,er+cD+cF);aU.
lineTo(ep+cF+cD,er-cD+cF);}else{aU.moveTo(ep+cF,er-cD);aU.lineTo(ep+cF-d4,er-cD-
d5);aU.lineTo(ep-cF-d4,er+cD-d5);aU.lineTo(ep-cF,er+cD);aU.lineTo(ep+cD,er+cF);}
aU.closePath();}}else if(dn===this.jO)aU.arc(ep,er,aD/2,0,2*Math.PI);}}aU.fill();
}a.fD=null;a.bindTexture(a.TEXTURE_2D,en);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MIN_FILTER
,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MAG_FILTER,a.NEAREST);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.CLAMP_TO_EDGE);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_T
,a.CLAMP_TO_EDGE);a.texImage2D(a.TEXTURE_2D,0,a.RGBA,a.RGBA,a.UNSIGNED_BYTE,aF);
aw.Format=d.lp;aw.Width=bo[0];aw.Height=bo[1];aw.ig=bo[0];aw.ie=bo[1];aw.d_=1/bo[
0];aw.d9=1/bo[1];aw.eT=2/bo[0];aw.eS=2/bo[1];aw.cL=en;aw.Framebuffer=null;return bitmap;
};d.sT=function(aa,n,ah,af,u,dV,ak,av,ar,aq,au,aA,iv,lN){var a=this.bj;var ae=null;
var aY=ak[0];var aG=ak[1];var k,j,r,p;if(aa&&(ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.
bK?aa.bK[ah]:ah;ae=aa.b2[ah];}if(!ae||(ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!n){if(this.a8)console.log("EmWi error: Invalid path object in the drawing operation."
);return;}this.hv(n);if(!dV){aG+=u[1];j=aG+n.bC[1]-1;p=aG+n.bC[3]+1;}else{aG+=u[
3];j=aG-n.bC[3]-1;p=aG-n.bC[1]+1;}aY+=u[0];k=aY+n.bC[0]-1;r=aY+n.bC[2]+1;k=Math.
max(af[0],u[0],k,0);j=Math.max(af[1],u[1],j,0);r=Math.min(af[2],u[2],r,aa.FrameSize[
0]);p=Math.min(af[3],u[3],p,aa.FrameSize[1]);aY-=k;if((r<=k)||(p<=j)||!n.aI)return;
if(dV)aG-=p;else aG-=j;var bitmap=this.qn([r-k,p-j],n,dV,[aY,aG],iv,lN);if(bitmap===
null)return;this.fF(aa,bitmap,ah,0,[k,j,r,p],u,[u[0]-k,u[1]-j],av,ar,aq,au,aA);a.
deleteTexture(bitmap.cH[0].cL);};d.ud=function(aa,n,ah,af,u,dV,ak,aD,fS,gG,av,ar
,aq,au,aA,iv){var dp=fS&0x0000FF;var dn=fS&0x00FF00;var e9=fS&0xFF0000;var a=this.
bj;var ae=null;var aY=ak[0];var aG=ak[1];var k,j,r,p;var cu;if(aa&&(ah>=0)&&(ah<
aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.b2[ah];}if(!ae||(ae.aJ.Format!==this.
ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!n){if(this.a8)console.log("EmWi error: Invalid path object in the drawing operation."
);return;}this.hv(n);if((dp===this.jP)&&(aD<4))dp=this.ll;if((dn===this.jO)&&(aD<
4))dn=this.lj;if((e9===this.lo)&&(aD<3))e9=this.lm;if(e9===this.ln){if(gG>1)cu=(
gG*aD*0.5)|0;else cu=(aD*0.5)|0;}else if((dp===this.iq)||(dn===this.ip))cu=(aD*0.75
)|0;else cu=(aD+0.5)|0;if(!dV){aG+=u[1];j=aG+n.bC[1]-cu-1;p=aG+n.bC[3]+cu+1;}else{
aG+=u[3];j=aG-n.bC[3]-cu-1;p=aG-n.bC[1]+cu+1;}aY+=u[0];k=aY+n.bC[0]-cu-1;r=aY+n.
bC[2]+cu+1;k=Math.max(af[0],u[0],k,0);j=Math.max(af[1],u[1],j,0);r=Math.min(af[2
],u[2],r,aa.FrameSize[0]);p=Math.min(af[3],u[3],p,aa.FrameSize[1]);aY-=k;if((r<=
k)||(p<=j)||!n.aI||(aD<=0))return;if(dV)aG-=p;else aG-=j;var bitmap=this.qo([r-k
,p-j],n,dV,[aY,aG],aD,fS,gG,iv);if(bitmap===null)return;this.fF(aa,bitmap,ah,0,[
k,j,r,p],u,[u[0]-k,u[1]-j],av,ar,aq,au,aA);a.deleteTexture(bitmap.cH[0].cL);};d.
fF=function(aa,aC,ah,a_,af,u,dB,av,ar,aq,au,aA){var a=this.bj;var ae=null;var src=
null;var k=u[0];var j=u[1];var r=u[2];var p=u[3];var em=dB[0];var e_=dB[1];if(aa&&(
ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.b2[ah];}if(aC&&(a_>=0)&&(
a_<aC.NoOfFrames)){a_=aC.bK?aC.bK[a_]:a_;src=aC.b2[a_];}if(aC.fc)return;if(!ae||(
ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a8)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}r=Math.min(r,k+aC.FrameSize[0]-dB[0]);p=Math.min(p,j+aC.FrameSize[1]-dB[
1]);k=Math.max(k,k-dB[0]);j=Math.max(j,j-dB[1]);k=Math.max(af[0],k,0);j=Math.max(
af[1],j,0);r=Math.min(af[2],r,aa.FrameSize[0]);p=Math.min(af[3],p,aa.FrameSize[1
]);if((r<=k)||(p<=j))return;var gm=(av!==au)||(ar!==aq);var f$=(av!==ar)||(au!==
aq);var eK=gm||f$;var gS=eK||(((av>>24)&0xFF)<255);em+=k-u[0]+src.aM[0];e_+=j-u[
1]+src.aM[1];var bc=em*src.aJ.d_;var a7=e_*src.aJ.d9;var by=(em+r-k)*src.aJ.d_;var
bw=(e_+p-j)*src.aJ.d9;if(gm&&((j!==u[1])||(p!==u[3]))){var bJ=1/(u[3]-u[1]);var bm=(
j-u[1])*bJ;var a1=(u[3]-p)*bJ;if(av!==au){var c5=av;if(bm)av=aS(c5,au,bm);if(a1)
au=aS(au,c5,a1);}if(ar!==aq){var f3=ar;if(bm)ar=aS(f3,aq,bm);if(a1)aq=aS(aq,f3,a1
);}}if(f$&&((k!==u[0])||(r!==u[2]))){var bJ=1/(u[2]-u[0]);var cm=(k-u[0])*bJ;var
ay=(u[2]-r)*bJ;if(av!==ar){var c5=av;if(cm)av=aS(c5,ar,cm);if(ay)ar=aS(ar,c5,ay);
}if(au!==aq){var fY=au;if(cm)au=aS(fY,aq,cm);if(ay)aq=aS(aq,fY,ay);}}var b=a.g_;
var aL=a.h$;b[0]=b[12]=k+ae.aM[0];b[1]=b[7]=j+ae.aM[1];b[6]=b[18]=r+ae.aM[0];b[19
]=b[13]=p+ae.aM[1];b[2]=bc;b[3]=a7;b[4]=1;b[8]=by;b[9]=a7;b[10]=1;b[20]=by;b[21]=
bw;b[22]=1;b[14]=bc;b[15]=bw;b[16]=1;aL[5]=av;aL[11]=ar;aL[23]=aq;aL[17]=au;if(src.
aJ.Format===this.ch)dH(a,ae.aJ,gS?a.hj:a.hi);else dH(a,ae.aJ,a.gq);hk(a,src.aJ,false
);if(a.b6&&!aA)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND);a.b6=aA;a.bufferSubData(
a.ARRAY_BUFFER,0,a.cC);a.drawArrays(a.TRIANGLE_STRIP,0,4);if(this.a8)ex(a);};d.is=
function(aa,aC,ah,a_,af,u,cy,dB,av,ar,aq,au,aA){var a=this.bj;var ae=null;var src=
null;var k=u[0];var j=u[1];var r=u[2];var p=u[3];var gi=cy[0];var gj=cy[1];var h7=
cy[2];var h8=cy[3];if(aa&&(ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=
aa.b2[ah];}if(aC&&(a_>=0)&&(a_<aC.NoOfFrames)){a_=aC.bK?aC.bK[a_]:a_;src=aC.b2[a_
];}if(aC.fc)return;if(!ae||(ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a8)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}gi=Math.max(gi,0);gj=Math.max(gj,0);h7=Math.min(h7,aC.FrameSize[0]);h8=
Math.min(h8,aC.FrameSize[1]);var jg=h7-gi;var i9=h8-gj;k=Math.max(af[0],k,0);j=Math.
max(af[1],j,0);r=Math.min(af[2],r,aa.FrameSize[0]);p=Math.min(af[3],p,aa.FrameSize[
1]);if((r<=k)||(p<=j)||(jg<=0)||(i9<=0))return;var gm=(av!==au)||(ar!==aq);var f$=(
av!==ar)||(au!==aq);var eK=gm||f$;var gS=eK||(((av>>24)&0xFF)<255);var kJ;if(gm&&((
j!==u[1])||(p!==u[3]))){var bJ=1/(u[3]-u[1]);var bm=(j-u[1])*bJ;var a1=(u[3]-p)*
bJ;if(av!==au){var c5=av;if(bm)av=aS(c5,au,bm);if(a1)au=aS(au,c5,a1);}if(ar!==aq
){var f3=ar;if(bm)ar=aS(f3,aq,bm);if(a1)aq=aS(aq,f3,a1);}}if(f$&&((k!==u[0])||(r
!==u[2]))){var bJ=1/(u[2]-u[0]);var cm=(k-u[0])*bJ;var ay=(u[2]-r)*bJ;if(av!==ar
){var c5=av;if(cm)av=aS(c5,ar,cm);if(ay)ar=aS(ar,c5,ay);}if(au!==aq){var fY=au;if(
cm)au=aS(fY,aq,cm);if(ay)aq=aS(aq,fY,ay);}}var b=a.g_;var aL=a.h$;if(src.aJ.Format===
this.ch)dH(a,ae.aJ,kJ=(gS?a.k_:a.k9));else dH(a,ae.aJ,kJ=a.k$);hk(a,src.aJ,false
);a.uniform2f(kJ.qc,jg*src.aJ.d_,i9*src.aJ.d9);a.uniform2f(kJ.qb,(gi+src.aM[0])*
src.aJ.d_,(gj+src.aM[1])*src.aJ.d9);gi=(dB[0]+k-u[0]+jg)/jg;gj=(dB[1]+j-u[1]+i9)
/i9;h7=gi+((r-k)/jg);h8=gj+((p-j)/i9);b[0]=b[12]=k+ae.aM[0];b[1]=b[7]=j+ae.aM[1];
b[6]=b[18]=r+ae.aM[0];b[19]=b[13]=p+ae.aM[1];b[2]=gi;b[3]=gj;b[4]=1;b[8]=h7;b[9]=
gj;b[10]=1;b[20]=h7;b[21]=h8;b[22]=1;b[14]=gi;b[15]=h8;b[16]=1;aL[5]=av;aL[11]=ar;
aL[23]=aq;aL[17]=au;if(a.b6&&!aA)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND
);a.b6=aA;a.bufferSubData(a.ARRAY_BUFFER,0,a.cC);a.drawArrays(a.TRIANGLE_STRIP,0
,4);if(this.a8)ex(a);};d.sQ=function(aa,aC,ah,a_,af,u,cy,c0,c1,dd,dc,nu,av,ar,aq
,au,aA){var bN=[0,0];var bq=u[0];var br=u[1];var a3=u[2];var a6=u[3];var bc=cy[0
];var a7=cy[1];var by=cy[2];var bw=cy[3];var bL=a3-bq;var bD=a6-br;var bP=((by-bc
)/3)|0;var bA=((bw-a7)/3)|0;var gb=(by-bc-bP*2)|0;var ga=(bw-a7-bA*2)|0;var oT=0;
var mi=0;var f1=av;var f2=ar;var fZ=au;var f0=aq;var hK=av;var hL=ar;var hI=au;var
hJ=aq;var kk=av;var km=ar;var kj=au;var kl=aq;if((bL<=0)||(bD<=0)||(gb<=0)||(ga<=
0))return;if(aC&&aC.fc)return;if(c0&&dd&&c1&&dc&&nu&&(bL===(by-bc))&&(bD===(bw-a7
))){this.fF(aa,aC,ah,a_,af,u,[bc,a7],av,ar,aq,au,aA);return;}if((av!==ar)||(aq!==
au)||(ar!==au)){if(c0){hK=aS(av,ar,bP/bL);hI=aS(au,aq,bP/bL);}if(dd){hL=aS(ar,av
,bP/bL);hJ=aS(aq,au,bP/bL);}if(c1){kk=aS(av,au,bA/bD);km=aS(ar,aq,bA/bD);}if(dc){
kj=aS(au,av,bA/bD);kl=aS(aq,ar,bA/bD);}if(!c0&&!c1)f1=av;if(c0&&!c1)f1=hK;if(!c0&&
c1)f1=kk;if(!dd&&!c1)f2=ar;if(dd&&!c1)f2=hL;if(!dd&&c1)f2=km;if(!c0&&!dc)fZ=au;if(
c0&&!dc)fZ=hI;if(!c0&&dc)fZ=kj;if(!dd&&!dc)f0=aq;if(dd&&!dc)f0=hJ;if(!dd&&dc)f0=
kl;if(c0&&c1)f1=aS(hK,hI,bA/bD);if(c0&&dc)fZ=aS(hI,hK,bA/bD);if(dd&&c1)f2=aS(hL,
hJ,bA/bD);if(dd&&dc)f0=aS(hJ,hL,bA/bD);}if(dd&&!c0)oT=bP-bL;if(dc&&!c1)mi=bA-bD;
by=bc+bP;bw=a7+bA;a3=bq+bP;a6=br+bA;if(c0&&c1)this.fF(aa,aC,ah,a_,af,[bq,br,a3,a6
],[bc,a7],av,hK,f1,kk,aA);bq+=bL-bP;a3+=bL-bP;bc+=bP+gb;by+=bP+gb;if(dd&&c1)this.
fF(aa,aC,ah,a_,af,[bq,br,a3,a6],[bc,a7],hL,ar,km,f2,aA);br+=bD-bA;a6+=bD-bA;a7+=
bA+ga;bw+=bA+ga;if(dd&&dc)this.fF(aa,aC,ah,a_,af,[bq,br,a3,a6],[bc,a7],f0,kl,aq,
hJ,aA);bq-=bL-bP;a3-=bL-bP;bc-=bP+gb;by-=bP+gb;if(c0&&dc)this.fF(aa,aC,ah,a_,af,[
bq,br,a3,a6],[bc,a7],kj,fZ,hI,au,aA);br-=bD-bA;a7-=ga;bw-=bA;bN[1]=mi;if(c1)br+=
bA;if(dc)a6-=bA;if(c0)this.is(aa,aC,ah,a_,af,[bq,br,a3,a6],[bc,a7,by,bw],bN,kk,f1
,fZ,kj,aA);bq+=bL-bP;a3+=bL-bP;bc+=bP+gb;by+=bP+gb;if(dd)this.is(aa,aC,ah,a_,af,[
bq,br,a3,a6],[bc,a7,by,bw],bN,f2,km,kl,f0,aA);if(c1)br-=bA;a6=br+bA;bq-=bL-bP;bc-=
gb;by-=bP;a7-=bA;bw-=ga;bN[0]=oT;bN[1]=0;if(c0)bq+=bP;if(dd)a3-=bP;if(c1)this.is(
aa,aC,ah,a_,af,[bq,br,a3,a6],[bc,a7,by,bw],bN,hK,hL,f2,f1,aA);br+=bD-bA;a6+=bD-bA;
a7+=bA+ga;bw+=bA+ga;if(dc)this.is(aa,aC,ah,a_,af,[bq,br,a3,a6],[bc,a7,by,bw],bN,
fZ,f0,hJ,hI,aA);if(c0)bq-=bP;a3=bq+bL;br-=bD-bA;a7-=ga;bw-=bA;bN[1]=mi;if(c0)bq+=
bP;if(c1)br+=bA;if(dd)a3-=bP;if(dc)a6-=bA;if(nu)this.is(aa,aC,ah,a_,af,[bq,br,a3
,a6],[bc,a7,by,bw],bN,f1,f2,f0,fZ,aA);};d.ug=function(aa,aC,ah,a_,af,gC,gE,de,gD
,hz,c2,hx,gF,cN,hy,hA,df,cy,a9,bf,dy,dz,aA,fL){if((gC===hy)&&(gD===hx)&&(gE===hz
)&&(gF===hA)&&(de===c2)&&(c2===cN)&&(cN===df))this.ql.apply(this,arguments);else
this.qm.apply(this,arguments);};d.ql=function(aa,aC,ah,a_,af,gC,gE,de,gD,hz,c2,hx
,gF,cN,hy,hA,df,cy,a9,bf,dy,dz,aA,fL){var a=this.bj;var ae=null;var src=null;var
bq=gC;var br=gE;var a3=gD;var a6=gF;var bc=cy[0];var a7=cy[1];var by=cy[2];var bw=
cy[3];var k=Math.min(bq,a3);var j=Math.min(br,a6);var r=Math.max(bq,a3);var p=Math.
max(br,a6);if(aa&&(ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.b2[ah
];}if(aC&&(a_>=0)&&(a_<aC.NoOfFrames)){a_=aC.bK?aC.bK[a_]:a_;src=aC.b2[a_];}if(aC.
fc)return;if(!ae||(ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a8)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}bc=Math.max(bc,0);a7=Math.max(a7,0);by=Math.min(by,aC.FrameSize[0]);bw=
Math.min(bw,aC.FrameSize[1]);k=Math.max(af[0],k,0);j=Math.max(af[1],j,0);r=Math.
min(af[2],r,aa.FrameSize[0]);p=Math.min(af[3],p,aa.FrameSize[1]);if((r<=k)||(p<=
j))return;if((j>br)||(p<br)||(j>a6)||(p<a6)){var bJ=1/(a6-br);var bm,a1;if(bJ>0){
bm=(j-br)*bJ;a1=(a6-p)*bJ;}else{bm=(p-br)*bJ;a1=(a6-j)*bJ;}if(a9!==dz){var l0=a9;
if(bm)a9=aS(a9,dz,bm);if(a1)dz=aS(dz,l0,a1);}if(bf!==dy){var rH=bf;if(bm)bf=aS(bf
,dy,bm);if(a1)dy=aS(dy,rH,a1);}var sl=a7;a7=a7+(bw-a7)*bm;bw=bw+(sl-bw)*a1;if(j>
br)br=j;else if(p<br)br=p;if(j>a6)a6=j;else if(p<a6)a6=p;}if((k>bq)||(r<bq)||(k>
a3)||(r<a3)){var bJ=1/(a3-bq);var cm,ay;if(bJ>0){cm=(k-bq)*bJ;ay=(a3-r)*bJ;}else{
cm=(r-bq)*bJ;ay=(a3-k)*bJ;}if(a9!==bf){var l0=a9;if(cm)a9=aS(a9,bf,cm);if(ay)bf=
aS(bf,l0,ay);}if(dz!==dy){var rI=dz;if(cm)dz=aS(dz,dy,cm);if(ay)dy=aS(dy,rI,ay);
}var sk=bc;bc=bc+(by-bc)*cm;by=by+(sk-by)*ay;if(k>bq)bq=k;else if(r<bq)bq=r;if(k>
a3)a3=k;else if(r<a3)a3=r;}bc=(bc+src.aM[0])*src.aJ.d_;a7=(a7+src.aM[1])*src.aJ.
d9;by=(by+src.aM[0])*src.aJ.d_;bw=(bw+src.aM[1])*src.aJ.d9;bq+=ae.aM[0];a3+=ae.aM[
0];br+=ae.aM[1];a6+=ae.aM[1];var gS=(((a9&bf&dy&dz)>>24)&0xFF)<255;var b=a.g_;var
aL=a.h$;b[0]=bq;b[6]=a3;b[1]=br;b[7]=br;b[2]=bc;b[8]=by;b[3]=a7;b[9]=a7;b[4]=1;b[
10]=1;aL[5]=a9;aL[11]=bf;b[12]=bq;b[18]=a3;b[13]=a6;b[19]=a6;b[14]=bc;b[20]=by;b[
15]=bw;b[21]=bw;b[16]=1;b[22]=1;aL[17]=dz;aL[23]=dy;if(src.aJ.Format===this.ch)dH(
a,ae.aJ,gS?a.hj:a.hi);else dH(a,ae.aJ,a.gq);hk(a,src.aJ,fL);if(a.b6&&!aA)a.disable(
a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND);a.b6=aA;a.bufferSubData(a.ARRAY_BUFFER,0
,a.cC);a.drawArrays(a.TRIANGLE_STRIP,0,4);if(this.a8)ex(a);};d.qm=function(aa,aC
,ah,a_,af,gC,gE,de,gD,hz,c2,hx,gF,cN,hy,hA,df,cy,a9,bf,dy,dz,aA,fL){var a=this.bj;
var ae=null;var src=null;var bq=Math.min(gC,gD,hx,hy);var br=Math.min(gE,hz,gF,hA
);var a3=Math.max(gC,gD,hx,hy);var a6=Math.max(gE,hz,gF,hA);var bc=cy[0];var a7=
cy[1];var by=cy[2];var bw=cy[3];var k=Math.round(bq);var j=Math.round(br);var r=
Math.round(a3);var p=Math.round(a6);var rL=k;var rO=j;var rM=r;var rP=p;if(aa&&(
ah>=0)&&(ah<aa.NoOfFrames)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.b2[ah];}if(aC&&(a_>=0)&&(
a_<aC.NoOfFrames)){a_=aC.bK?aC.bK[a_]:a_;src=aC.b2[a_];}if(aC.fc)return;if(!ae||(
ae.aJ.Format!==this.ch)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a8)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}bc=Math.max(bc,0);a7=Math.max(a7,0);by=Math.min(by,aC.FrameSize[0]);bw=
Math.min(bw,aC.FrameSize[1]);k=Math.max(af[0],k,0);j=Math.max(af[1],j,0);r=Math.
min(af[2],r,aa.FrameSize[0]);p=Math.min(af[3],p,aa.FrameSize[1]);if((r<=k)||(p<=
j))return;if((de<=0)&&(c2<=0)&&(cN<=0)&&(df<=0)){de=-de;c2=-c2;cN=-cN;df=-df;}if((
de<0)||(c2<0)||(cN<0)||(df<0)){console.log("Emwi error: Not convex destination polygon to warp bitmap."
);return;}var kI=(de!==c2)||(cN!==df)||(de!==cN);var eK=(a9!==bf)||(dy!==dz)||(a9
!==dz)||(bf!==dy);var gS=eK||(((a9>>24)&0xFF)<255);if(kI){var e3=de;if(c2<e3)e3=
c2;if(cN<e3)e3=cN;if(df<e3)e3=df;de=e3/de;c2=e3/c2;cN=e3/cN;df=e3/df;}else{de=1;
c2=1;cN=1;df=1;}bc=(bc+src.aM[0])*src.aJ.d_;a7=(a7+src.aM[1])*src.aJ.d9;by=(by+src.
aM[0])*src.aJ.d_;bw=(bw+src.aM[1])*src.aJ.d9;var b=a.g_;var aL=a.h$;var cR=ae.aM[
0];var cS=ae.aM[1];var ai=4;b[0]=gC+cR;b[6]=gD+cR;b[1]=gE+cS;b[7]=hz+cS;b[2]=bc*
de;b[8]=by*c2;b[3]=a7*de;b[9]=a7*c2;b[4]=de;b[10]=c2;aL[5]=a9;aL[11]=bf;b[18]=hy+
cR;b[12]=hx+cR;b[19]=hA+cS;b[13]=gF+cS;b[20]=bc*df;b[14]=by*cN;b[21]=bw*df;b[15]=
bw*cN;b[22]=df;b[16]=cN;aL[23]=dz;aL[17]=dy;if((k>rL)||(j>rO)||(r<rM)||(p<rP)){ai=
js(b,aL,ai,false,k+cR);ai=js(b,aL,ai,false,r+cR);ai=mJ(b,ai,k+cR,r+cR);ai=jr(b,aL
,ai,false,j+cS);ai=jr(b,aL,ai,false,p+cS);ai=mI(b,ai,j+cS,p+cS);if(ai<3)return;}
if((src.aJ.Format===this.ch)&&!gS)dH(a,ae.aJ,kI?a.jy:a.hi);else if(src.aJ.Format===
this.ch)dH(a,ae.aJ,kI?a.jz:a.hj);else dH(a,ae.aJ,kI?a.jA:a.gq);hk(a,src.aJ,fL);if(
a.b6&&!aA)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND);a.b6=aA;switch(ai){case
4:{b.set(b.subarray(12,24),18);b.set(b.subarray(24,30),12);break;}case 5:{b.set(
b.subarray(12,30),18);b.set(b.subarray(30,36),12);break;}case 6:{b.set(b.subarray(
12,36),36);b.set(b.subarray(54,60),12);b.set(b.subarray(36,42),18);b.set(b.subarray(
48,54),24);b.set(b.subarray(42,48),30);break;}case 7:{b.set(b.subarray(12,42),42
);b.set(b.subarray(66,72),12);b.set(b.subarray(42,48),18);b.set(b.subarray(60,66
),24);b.set(b.subarray(48,60),30);break;}case 8:{b.set(b.subarray(12,48),48);b.set(
b.subarray(78,84),12);b.set(b.subarray(48,54),18);b.set(b.subarray(72,80),24);b.
set(b.subarray(54,60),30);b.set(b.subarray(66,72),36);b.set(b.subarray(60,66),42
);break;}}a.bufferSubData(a.ARRAY_BUFFER,0,a.cC);a.drawArrays(a.TRIANGLE_STRIP,0
,ai);if(this.a8)ex(a);};function mL(o,fi,lK,iz,a2,bu,b4,iP,gN,gx,gz,gy,gA,u,cM){
var i3=fi.length*6;var cC=new ArrayBuffer(i3*6*4);var b=new Float32Array(cC);var
aL=new Uint32Array(cC);var aY=iz[0]+u[0];var aG=iz[1]+u[1];var bL=u[2]-u[0];var bD=
u[3]-u[1];for(var h=0;h<fi.length;h++){var y=h*36;var z=fi[h];var dj=lK[h];var k=
a2+z.OriginX+dj;var j=bu+z.OriginY;var r=k+z.Width;var p=j+z.Height;var cb=z.mE;
var cJ=z.mF;var cc=z.mG;var bW=z.mH;if((j<gz)&&(p!==j)){cc+=((gz-j)/(p-j))*(bW-cc
);j=gz;}if((p>gA)&&(p!==j)){bW+=((gA-p)/(p-j))*(bW-cc);p=gA;}if((k<gx)&&(k!==r)){
cb+=((gx-k)/(r-k))*(cJ-cb);k=gx;}if((r>gy)&&(k!==r)){cJ+=((gy-r)/(r-k))*(cJ-cb);
r=gy;}if(k>r)k=r;if(j>p)j=p;if(b4===0){k+=aY;j+=aG;r+=aY;p+=aG;b[y+0]=k;b[y+6]=r;
b[y+1]=j;b[y+7]=j;b[y+12]=k;b[y+18]=k;b[y+13]=p;b[y+19]=p;b[y+24]=r;b[y+30]=r;b[
y+25]=j;b[y+31]=p;}else if(b4===90){var cd=aY+j;var cf=aG+bD-k;var ce=aY+p;var cg=
aG+bD-r;b[y+0]=cd;b[y+6]=cd;b[y+1]=cf;b[y+7]=cg;b[y+12]=ce;b[y+18]=ce;b[y+13]=cf;
b[y+19]=cf;b[y+24]=cd;b[y+30]=ce;b[y+25]=cg;b[y+31]=cg;}else if(b4===180){var cd=
aY+bL-k;var cf=aG+bD-j;var ce=aY+bL-r;var cg=aG+bD-p;b[y+0]=cd;b[y+6]=ce;b[y+1]=
cf;b[y+7]=cf;b[y+12]=cd;b[y+18]=cd;b[y+13]=cg;b[y+19]=cg;b[y+24]=ce;b[y+30]=ce;b[
y+25]=cf;b[y+31]=cg;}else if(b4===270){var cd=aY+bL-j;var cf=aG+k;var ce=aY+bL-p;
var cg=aG+r;b[y+0]=cd;b[y+6]=cd;b[y+1]=cf;b[y+7]=cg;b[y+12]=ce;b[y+18]=ce;b[y+13
]=cf;b[y+19]=cf;b[y+24]=cd;b[y+30]=ce;b[y+25]=cg;b[y+31]=cg;}b[y+2]=cb;b[y+8]=cJ;
b[y+3]=cc;b[y+9]=cc;b[y+4]=1;b[y+10]=1;aL[y+5]=cM;aL[y+11]=cM;b[y+14]=cb;b[y+20]=
cb;b[y+15]=bW;b[y+21]=bW;b[y+16]=1;b[y+22]=1;aL[y+17]=cM;aL[y+23]=cM;b[y+26]=cJ;
b[y+32]=cJ;b[y+27]=cc;b[y+33]=bW;b[y+28]=1;b[y+34]=1;aL[y+29]=cM;aL[y+35]=cM;a2+=
z.Advance+dj;if(iP&&((z.d6===0x20)||(z.d6===0xA0))){gN+=iP;a2+=gN>>16;gN&=0xFFFF;
}}o.bufferData(o.ARRAY_BUFFER,cC,o.STREAM_DRAW);o.drawArrays(o.TRIANGLES,0,i3);o.
bufferData(o.ARRAY_BUFFER,o.cC,o.STREAM_DRAW);}function mM(o,fi,lK,iz,a2,bu,b4,iP
,gN,gx,gz,gy,gA,u,av,ar,aq,au){var i3=fi.length*6;var cC=new ArrayBuffer(i3*6*4);
var b=new Float32Array(cC);var aL=new Uint32Array(cC);var aY=iz[0]+u[0];var aG=iz[
1]+u[1];var bL=u[2]-u[0];var bD=u[3]-u[1];var kA=1/bL;var kx=1/bD;if(b4===90){var
g=av;av=au;au=aq;aq=ar;ar=g;kA=1/bD;kx=1/bL;}else if(b4===180){var a0=av;var be=
au;av=aq;aq=a0;au=ar;ar=be;}else if(b4===270){var g=av;av=ar;ar=aq;aq=au;au=g;kA=
1/bD;kx=1/bL;}var j3=(av&0xFF);var j4=((av>>8)&0xFF);var j5=((av>>16)&0xFF);var j6=((
av>>24)&0xFF);var j7=(ar&0xFF);var j8=((ar>>8)&0xFF);var j9=((ar>>16)&0xFF);var j_=((
ar>>24)&0xFF);var nW=(au&0xFF);var nX=((au>>8)&0xFF);var nY=((au>>16)&0xFF);var nZ=((
au>>24)&0xFF);var n0=(aq&0xFF);var n1=((aq>>8)&0xFF);var n2=((aq>>16)&0xFF);var n3=((
aq>>24)&0xFF);for(var h=0;h<fi.length;h++){var y=h*36;var z=fi[h];var dj=lK[h];var
k=a2+z.OriginX+dj;var j=bu+z.OriginY;var r=k+z.Width;var p=j+z.Height;var cb=z.mE;
var cJ=z.mF;var cc=z.mG;var bW=z.mH;if((j<gz)&&(p!==j)){cc+=((gz-j)/(p-j))*(bW-cc
);j=gz;}if((p>gA)&&(p!==j)){bW+=((gA-p)/(p-j))*(bW-cc);p=gA;}if((k<gx)&&(k!==r)){
cb+=((gx-k)/(r-k))*(cJ-cb);k=gx;}if((r>gy)&&(k!==r)){cJ+=((gy-r)/(r-k))*(cJ-cb);
r=gy;}if(k>r)k=r;if(j>p)j=p;var f9=j*kx;var f6=p*kx;var f7=k*kA;var f8=r*kA;var ke=
j3+(nW-j3)*f9;var kf=j4+(nX-j4)*f9;var kg=j5+(nY-j5)*f9;var kh=j6+(nZ-j6)*f9;var
n8=j7+(n0-j7)*f9;var n9=j8+(n1-j8)*f9;var n_=j9+(n2-j9)*f9;var n$=j_+(n3-j_)*f9;
var ka=j3+(nW-j3)*f6;var kb=j4+(nX-j4)*f6;var kc=j5+(nY-j5)*f6;var kd=j6+(nZ-j6)
*f6;var n4=j7+(n0-j7)*f6;var n5=j8+(n1-j8)*f6;var n6=j9+(n2-j9)*f6;var n7=j_+(n3-
j_)*f6;var ry=ke+(n8-ke)*f7;var rz=kf+(n9-kf)*f7;var rA=kg+(n_-kg)*f7;var rB=kh+(
n$-kh)*f7;var rC=ke+(n8-ke)*f8;var rD=kf+(n9-kf)*f8;var rE=kg+(n_-kg)*f8;var rF=
kh+(n$-kh)*f8;var rq=ka+(n4-ka)*f7;var rr=kb+(n5-kb)*f7;var rs=kc+(n6-kc)*f7;var
rt=kd+(n7-kd)*f7;var ru=ka+(n4-ka)*f8;var rv=kb+(n5-kb)*f8;var rw=kc+(n6-kc)*f8;
var rx=kd+(n7-kd)*f8;av=ry|(rz<<8)|(rA<<16)|(rB<<24);ar=rC|(rD<<8)|(rE<<16)|(rF<<
24);aq=ru|(rv<<8)|(rw<<16)|(rx<<24);au=rq|(rr<<8)|(rs<<16)|(rt<<24);if(b4===0){k+=
aY;j+=aG;r+=aY;p+=aG;b[y+0]=k;b[y+6]=r;b[y+1]=j;b[y+7]=j;b[y+12]=k;b[y+18]=k;b[y+
13]=p;b[y+19]=p;b[y+24]=r;b[y+30]=r;b[y+25]=j;b[y+31]=p;}else if(b4===90){var cd=
aY+j;var cf=aG+bD-k;var ce=aY+p;var cg=aG+bD-r;b[y+0]=cd;b[y+6]=cd;b[y+1]=cf;b[y+
7]=cg;b[y+12]=ce;b[y+18]=ce;b[y+13]=cf;b[y+19]=cf;b[y+24]=cd;b[y+30]=ce;b[y+25]=
cg;b[y+31]=cg;}else if(b4===180){var cd=aY+bL-k;var cf=aG+bD-j;var ce=aY+bL-r;var
cg=aG+bD-p;b[y+0]=cd;b[y+6]=ce;b[y+1]=cf;b[y+7]=cf;b[y+12]=cd;b[y+18]=cd;b[y+13]=
cg;b[y+19]=cg;b[y+24]=ce;b[y+30]=ce;b[y+25]=cf;b[y+31]=cg;}else if(b4===270){var
cd=aY+bL-j;var cf=aG+k;var ce=aY+bL-p;var cg=aG+r;b[y+0]=cd;b[y+6]=cd;b[y+1]=cf;
b[y+7]=cg;b[y+12]=ce;b[y+18]=ce;b[y+13]=cf;b[y+19]=cf;b[y+24]=cd;b[y+30]=ce;b[y+
25]=cg;b[y+31]=cg;}b[y+2]=cb;b[y+8]=cJ;b[y+3]=cc;b[y+9]=cc;b[y+4]=1;b[y+10]=1;aL[
y+5]=av;aL[y+11]=ar;b[y+14]=cb;b[y+20]=cb;b[y+15]=bW;b[y+21]=bW;b[y+16]=1;b[y+22
]=1;aL[y+17]=au;aL[y+23]=au;b[y+26]=cJ;b[y+32]=cJ;b[y+27]=cc;b[y+33]=bW;b[y+28]=
1;b[y+34]=1;aL[y+29]=ar;aL[y+35]=aq;a2+=z.Advance+dj;if(iP&&((z.d6===0x20)||(z.d6===
0xA0))){gN+=iP;a2+=gN>>16;gN&=0xFFFF;}}o.bufferData(o.ARRAY_BUFFER,cC,o.STREAM_DRAW
);o.drawArrays(o.TRIANGLES,0,i3);o.bufferData(o.ARRAY_BUFFER,o.cC,o.STREAM_DRAW);
}d.nf=function(aa,ac,m,ak,al,ah,af,u,dB,dJ,b4,av,ar,aq,au,aA){var a=this.bj;var ae=
null;var k=u[0];var j=u[1];var r=u[2];var p=u[3];var em=-dB[0];var e_=-dB[1];var
dE=0;var dm=0x8000;var ad=0;var aT=1;var aO=0;var dj=0;if(aa&&(ah>=0)&&(ah<aa.NoOfFrames
)){ah=aa.bK?aa.bK[ah]:ah;ae=aa.b2[ah];}if(!ae||(ae.aJ.Format!==this.ch)){console.
log("EmWi error: Invalid destination bitmap in the drawing operation.");return;}
if(!ac){if(this.a8)console.log("EmWi error: Invalid font in the drawing operation."
);return;}if(al===-1)al=m.length;if(ak<0){al+=ak;ak=0;}al=Math.min(al,m.length-ak
);if((b4!==90)&&(b4!==180)&&(b4!==270))b4=0;k=Math.max(af[0],k,0);j=Math.max(af[
1],j,0);r=Math.min(af[2],r,aa.FrameSize[0]);p=Math.min(af[3],p,aa.FrameSize[1]);
if(b4===90){var g=j;j=k-u[0];k=u[3]-p;p=r-u[0];r=u[3]-g;}else if(b4===180){var a0=
k;var be=j;k=u[2]-r;j=u[3]-p;r=u[2]-a0;p=u[3]-be;}else if(b4===270){var g=k;k=j-
u[1];j=u[2]-r;r=p-u[1];p=u[2]-g;}else{k-=u[0];j-=u[1];r-=u[0];p-=u[1];}j=Math.max(
j,e_-ac.Ascent);p=Math.min(p,e_+ac.Descent+1);if((r<=k)||(p<=j)||(al<=0))return;
if(dJ>0)dJ-=this.jJ(ac,m,ak,al);if(dJ>0){var bN=ak;var ai=al;var eM=0;for(;ai;bN++
,ai--){var f=m.charCodeAt(bN);if((f===32)||(f===0xA0))eM++;}if(eM)dE=((dJ<<16)/eM
)|0;}while(al&&(ad=m.charCodeAt(ak))){if((ad===0x2026)&&!this.eV(ac,ad)){ad=0x2E;
aT=3;}else aT=1;var z=this.dT(ac,ad);if(!z){if(this.a8)console.log("EmWi error: Default glyph not found in the font."
);return;}if(!z.Advance&&!z.OriginX&&!z.Width){ak++;al--;continue;}var hS=dj=this.
bx(ac,aO,ad);if(aT>1)hS+=(aT-1)*this.bx(ac,ad,ad);if((em+hS+((aT-1)*z.Advance)+z.
OriginX+z.Width)>k)break;if(dE&&((ad===0x20)||(ad===0xA0))){dm+=dE;em+=dm>>16;dm&=
0xFFFF;}em+=hS+aT*z.Advance;ak++;al--;aO=ad;}if(!al)return;var eK=(av!==au)||(ar
!==aq)||(av!==ar);dH(a,ae.aJ,a.gq);hk(a,null,false);a.bindTexture(a.TEXTURE_2D,this.
ij);if(a.b6&&!aA)a.disable(a.BLEND);if(!a.b6&&aA)a.enable(a.BLEND);a.b6=aA;var aR=[
];var hT=[];var je=em;var jc=dm;var jd=dE;do{var z=this.nh(ac,ad);if(!z){if(eK)mM(
a,aR,hT,ae.aM,je,e_,b4,jd,jc,k,j,r,p,u,av,ar,aq,au);else mL(a,aR,hT,ae.aM,je,e_,
b4,jd,jc,k,j,r,p,u,av);for(var h=0;h<aR.length;h++)aR[h].jE--;aR=[];hT=[];je=em;
jc=dm;jd=dE;z=this.nh(ac,ad);}if(!z){if(this.a8)console.log("EmWi error: Default glyph not found in the font."
);return;}if(!z.Advance&&!z.OriginX&&!z.Width){if(--al>0)ad=m.charCodeAt(++ak);else
ad=0;if((ad===0x2026)&&!this.eV(ac,ad)){ad=0x2E;aT=3;}else aT=1;continue;}dj=this.
bx(ac,aO,ad);if((em+dj+z.OriginX)>=r)break;while(aT-->0){z.jE++;aR.push(z);hT.push(
dj);if(dE&&((ad===0x20)||(ad===0xA0))){dm+=dE;em+=dm>>16;dm&=0xFFFF;}em+=dj+z.Advance;
if(aT>0)dj=this.bx(ac,ad,ad);aO=ad;}if(--al>0)ad=m.charCodeAt(++ak);else ad=0;if((
ad===0x2026)&&!this.eV(ac,ad)){ad=0x2E;aT=3;}else aT=1;}while(al>0);if(eK)mM(a,aR
,hT,ae.aM,je,e_,b4,jd,jc,k,j,r,p,u,av,ar,aq,au);else mL(a,aR,hT,ae.aM,je,e_,b4,jd
,jc,k,j,r,p,u,av);for(var h=0;h<aR.length;h++)aR[h].jE--;if(this.a8)ex(a);};d.sP=
function(aa,am,a4,ah,af,u,dB,fP,fQ,gK,gJ,aA){if(!aa||!am||!a4||(ah<0)||(ah>=aa.NoOfFrames
))return;af=this.il(af,u);if((af[2]<=af[0])||(af[3]<=af[1]))return;if(fP<0)fP=0;
if(fP>255)fP=255;if(fQ<0)fQ=0;if(fQ>255)fQ=255;if(gJ<0)gJ=0;if(gJ>255)gJ=255;if(
gK<0)gK=0;if(gK>255)gK=255;var width=u[2]-u[0];var height=u[3]-u[1];var ow=1;var
ky=1;var oy=(fP!==gJ)||(fQ!==gK);var ox=(fP!==fQ)||(gJ!==gK);if(width&&ox)ow/=width;
if(height&&oy)ky/=height;var eK={mU:ox,mV:oy,pQ:ow,sx:ky,Width:width,Height:height
,pg:fP,ph:(gJ-fP)*ky,pi:fQ,pj:(gK-fQ)*ky};pv(this,aa,ah,af,u,u[0]-dB[0],u[1]-dB[
1],am,a4.Data,0,a4.Data.length,aA,eK);};d.qx=[0,0];d.qy=[0,0,0,0];d.hm="";d.ii=3.1415926535
/180;d.ni=180/3.1415926535;d.jQ=new Array(129).join('0');d.hs=[];d.hq=null;d.gv=
0;d.ne=0;d.db=null;d.dU=null;d.fH=null;d.hr=null;d.m7=null;d.sy="";d.bV=null;d.qt=
0;d.d$=null;d.bj=null;d.gr=null;d.io=false;d.im=false;d.gu=false;d.ea=0;d.hu=[];
d.lr=[];d.nk=[];d.dw=0;d.eW=0;d.li=null;d.jI=true;d.lc=false;d.a8=false;d.gt=1024;
d.jL=1024;d.gs=64;d.ij=null;d.fG=null;d.jK=null;d.ho=null;d.ht=0;d.ir=-1;d.ch=0;
d.lp=1;d.lk=0x00000000;d.iq=0x00000001;d.ll=0x00000002;d.jP=0x00000003;d.qJ=0x00000000;
d.ip=0x00000100;d.lj=0x00000200;d.jO=0x00000300;d.tH=0x00000000;d.tJ=0x00000101;
d.tK=0x00000202;d.tI=0x00000303;d.lm=0x00000000;d.ln=0x00010000;d.lo=0x00020000;
d.qP=0;d.qN=1;d.nn=2;d.qO=3;d.tR=4;return d;})();

/* Embedded Wizard */