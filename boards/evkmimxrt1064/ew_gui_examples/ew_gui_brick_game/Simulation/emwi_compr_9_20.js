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

EmWiCompr_9_10=(function(){var d={};d._SetLanguage=function(mA){if(this.kt===mA)return;
this.kt=mA;var eQ=this.g7;while(eQ){eQ._ReInit();eQ=eQ._next;}};d._SetStyles=function(
pZ){this.s$=pZ;};d.r5=function(h3){if(h3._variants)h3=h3._variants();var gP=h3[this.
kt];if(gP!==undefined)return gP;else return h3[0];};d._GetAutoObject=function(gc
){if(gc._variants)gc=gc._variants();if(!gc._this)gc._Init.call(gc._this={});return gc.
_this;};d.r0=function(dl){if(dl._variants)dl=dl._variants();var hK=dl[this.kt];if(
hK===undefined)hK=dl[0];if(!hK._this)dl._class()._Init.call(hK._this={},hK);return hK.
_this;};d._NewObject=function(mp,pC){var g;if(!mp)throw"No class specified to create the object";
mp._Init.call(g={},pC);g._next=this.g7;this.g7=g;return g;};d._LockObject=function(
bZ){if(bZ)this.g9.push(bZ);};d._UnlockObject=function(bZ){var nX=this.g9;var g;if(
!bZ)return;for(g=nX.length-1;g>=0;g--)if(nX[g]===bZ)break;if(g>=0)this.g9.splice(
g,1);};d.sj=function(cN,dZ){var g;for(var h in cN)if(!isNaN(h)&&(g=cN[h])&&(g._cycle
!==dZ))g._Mark(g._cycle=dZ);};d.sl=function(cN,dZ){var g;for(var h in cN)if(!isNaN(
h)&&(g=cN[h])&&(g=g[0])&&(g._cycle!==dZ))g._Mark(g._cycle=dZ);};d.sk=function(cN
,dZ){var g;for(var h in cN)if(!isNaN(h)&&(g=cN[h])&&(g=g[0])&&(g._cycle!==dZ))g.
_Mark(g._cycle=dZ);};d.mi=function(){var ca=(this.me+=1);var qY=this.gb;var aV=null;
var a9=null;var g=null;function ki(mB,dZ){var y=mB;var nA=0;var g;for(;y;y=y._next
){if((g=y.ek)&&(g._cycle!==dZ)&&(y[0]._cycle===dZ)){g._Mark(g._cycle=dZ);y=mB;nA++;
}}return nA;}for(aV=this.g9.length-1;aV>=0;aV--)if((a9=this.g9[aV])&&(a9._cycle!==
ca))a9._Mark(a9._cycle=ca);ki(this.c0,ca);while((ki(this.dG,ca)>0)&&(ki(this.c0,
ca)>0));while((aV=this.c0)&&((aV[0]._cycle!==ca)||((g=aV.ek)&&(g._cycle!==ca))))
this.c0=aV._next;while((a9=this.dG)&&((a9[0]._cycle!==ca)||((g=a9.ek)&&(g._cycle
!==ca))))this.dG=a9._next;while(aV&&(g=aV._next))if((g.ek&&(g.ek._cycle!==ca))||(
g[0]._cycle!==ca))aV._next=g._next;else aV=g;while(a9&&(g=a9._next))if((g.ek&&(g.
ek._cycle!==ca))||(g[0]._cycle!==ca))a9._next=g._next;else a9=g;while((aV=this.g7
)&&(aV._cycle!==ca)){this.g7=aV._next;aV._Done();}if(aV)while(a9=aV._next){if(a9.
_cycle!==ca){aV._next=a9._next;a9._Done();}else aV=a9;}this.sK(ca);while((aV=this.
fq)&&(aV[0]._cycle!==ca))this.fq=aV._next;if(aV)while(a9=aV._next)if(a9[0]._cycle
!==ca)aV._next=a9._next;else aV=a9;while((aV=this.g8)&&(aV._cycle!==ca))this.g8=
aV.h1;while(aV){var j6;while((a9=aV._observers)&&(a9[0]._cycle!==ca))aV._observers=
a9._next;if(a9)while(j6=a9._next)if(j6[0]._cycle!==ca)a9._next=j6._next;else a9=
j6;while((a9=aV.h1)&&(a9._cycle!==ca))aV.h1=a9.h1;aV=a9;}if(this.a4)console.log(
"Living objects: "+this.gb+"("+(this.gb-qY)+")");};d.kD=function(bb,ig){if(!bb)return;
var y=this.c0;var eQ=bb[0];var iB=bb[1];var g=null;if(y&&(y[0]===eQ)&&(y[1]===iB
)){if(!y.kG)y.ek=ig;return;}if(y)while((g=y._next)&&((g[0]!==eQ)||(g[1]!==iB)))y=
g;if(g&&g.kG)return;if(g){y._next=g._next;g._next=null;}bb._next=this.c0;bb.ek=ig;
this.c0=bb;};d.r6=function(bb,ig){if(!bb)return;var y=this.dG;var eQ=bb[0];var iB=
bb[1];var g;if(y&&(y[0]===eQ)&&(y[1]===iB)){if(!y.kG)y.ek=ig;return;}if(y)while((
g=y._next)&&((g[0]!==eQ)||(g[1]!==iB)))y=g;if(g){y._next=g._next;g._next=null;}bb.
_next=this.dG;bb.ek=ig;this.dG=bb;};d.pz=function(){var hB=[];if(!this.c0){if(this.
dG){this.c0=this.dG;this.dG=null;}return false;}for(;;){var iA=this.c0;var lp=null;
var g;while(iA&&(iA!==hB)){g=iA;iA=iA._next;g._next=lp;lp=g;}hB._next=lp;this.c0=
hB;while(g=hB._next){g.kG=true;g[1].call(g[0],g.ek);hB._next=g._next;}if(this.c0===
hB){this.c0=null;break;}}this.c0=this.dG;this.dG=null;return true;};d.rn=function(
bb,en,bY){var ds;if(!bb||!en||!(ds=en[0]))return;var y=ds._observers;var cV=bb[0
];var cW=bb[1];var gK=en[1];while(y&&((y[0]!==cV)||(y[1]!==cW)||(y[2]!==bY)||(y[
3]!==gK)))y=y._next;if(y)return;y=[cV,cW,bY,gK];y._next=ds._observers;ds._observers=
y;if(!ds.kF){ds.h1=this.g8;ds.kF=true;this.g8=ds;}};d.rl=function(bb,bZ,bY){if(!
bb||!bZ)return;var y=bZ._observers;var cV=bb[0];var cW=bb[1];while(y&&((y[0]!==cV
)||(y[1]!==cW)||(y[2]!==bY)||y[3]))y=y._next;if(y)return;y=[cV,cW,bY,null];y._next=
bZ._observers;bZ._observers=y;if(!bZ.kF){bZ.h1=this.g8;bZ.kF=true;this.g8=bZ;}};
d.rm=function(bb,bY){if(!bb)return;var y=this.fq;var cV=bb[0];var cW=bb[1];while(
y&&((y[0]!==cV)||(y[1]!==cW)||(y[2]!==bY)))y=y._next;if(y)return;y=[cV,cW,bY];y.
_next=this.fq;this.fq=y;};d.rC=function(bb,en,bY){var ds;var y;if(!bb||!en||!(ds=
en[0])||!(y=ds._observers))return;var cV=bb[0];var cW=bb[1];var gK=en[1];var bA;
if((y[0]===cV)&&(y[1]===cW)&&(y[2]===bY)&&(y[3]===gK)){ds._observers=y._next;y._next=
null;return;}while((bA=y._next)&&((bA[0]!==cV)||(bA[1]!==cW)||(bA[2]!==bY)||(bA[
3]!==gK)))y=bA;if(!bA)return;y._next=bA._next;bA._next=null;};d.rA=function(bb,bZ
,bY){var y;if(!bb||!bZ||!(y=bZ._observers))return;var cV=bb[0];var cW=bb[1];var bA;
if((y[0]===cV)&&(y[1]===cW)&&(y[2]===bY)&&!y[3]){bZ._observers=y._next;y._next=null;
return;}while((bA=y._next)&&((bA[0]!==cV)||(bA[1]!==cW)||(bA[2]!==bY)||bA[3]))y=
bA;if(!bA)return;y._next=bA._next;bA._next=null;};d.rB=function(bb,bY){var y;if(
!bb||!(y=this.fq))return;var cV=bb[0];var cW=bb[1];var bA;if((y[0]===cV)&&(y[1]===
cW)&&(y[2]===bY)){this.fq=y._next;y._next=null;return;}while((bA=y._next)&&((bA[
0]!==cV)||(bA[1]!==cW)||(bA[2]!==bY)))y=bA;if(!bA)return;y._next=bA._next;bA._next=
null;};d.sx=function(en,bY){var ds;var y;if(!en||!(ds=en[0])||!(y=ds._observers)
)return;if(this.a4)console.log("NotifyRefObserver()");var gK=en[1];while(y){if((
y[2]===bY)&&(y[3]===gK))this.kD(y.slice(0,2),null);y=y._next;}};d.pl=function(bZ
,bY){var y;if(!bZ||!(y=bZ._observers))return;if(this.a4)console.log("NotifyObjObserver()"
);while(y){if((y[2]===bY)&&!y[3])this.kD(y.slice(0,2),null);y=y._next;}};d.sw=function(
bY){var y;if(!(y=this.fq))return;if(this.a4)console.log("NotifyObserver()");while(
y){if(y[2]===bY)this.kD(y.slice(0,2),null);y=y._next;}};function StartTimer(je,he
){var iL=this;if((je>0)&&(he>0))this.fn=window.setTimeout(function(){oV(iL,he);}
,je);else if(je>0)this.fn=window.setTimeout(function(){oW(iL);},je);else if(he>0
)this.fj=window.setInterval(function(){l5(iL);},he);}function DestroyTimer(){if(
this.fj)window.clearInterval(this.fj);this.fj=0;if(this.fn)window.clearTimeout(this.
fn);this.fn=0;this.hN.gb--;this.iY=null;this.iW=null;this.hN=null;}function ResetTimer(
){if(this.fj)window.clearInterval(this.fj);this.fj=0;if(this.fn)window.clearTimeout(
this.fn);this.fn=0;}function oV(dn,he){dn.o4=null;dn.fj=window.setInterval(function(
){l5(dn);},he);try{dn.iW.call(dn.iY);dn.hN._RequestUpdate();}catch(bs){console.log(
"EmWi error: "+bs+"!");}}function oW(dn){try{dn.o4=null;dn.iW.call(dn.iY);dn.hN.
_RequestUpdate();}catch(bs){console.log("EmWi error: "+bs+"!");}}function l5(dn){
try{dn.iW.call(dn.iY);dn.hN._RequestUpdate();}catch(bs){console.log("EmWi error: "+
bs+"!");}}d.rz=function(bZ,pN){var iL={DestroyTimer:DestroyTimer,ResetTimer:ResetTimer
,StartTimer:StartTimer,hN:this,iY:bZ,iW:pN,fj:0,fn:0};this.gb++;return iL;};function
of(){var fO=this.lT;var eP=fO.length;var gz=0;if(arguments.length!==eP)return this.
kc;for(var h=0;h<eP;h++){var it=fO[h];var q=arguments[h];if((q<0)||(q>=it))return this.
kc;gz=(gz*it)+q;}var gP=this[gz];if(gP===undefined)return this.kc;return gP;};function
og(){var fO=this.lT;var eP=fO.length;var gz=0;if((arguments.length-1)!==eP)return;
for(var h=0;h<eP;h++){var it=fO[h];var q=arguments[h];if((q<0)||(q>=it))return;gz=(
gz*it)+q;}return this[gz]=arguments[eP];};d.sr=function(){var eP=arguments.length-
2;var qz=arguments[eP];var li=arguments[eP+1];var fO=[];for(var h=0;h<eP;h++)fO[
h]=arguments[h];var hh=[];hh.lT=fO;hh.kc=qz;if(li)for(var h in li)hh[h]=li[h];hh.
Get=of;hh.Set=og;return hh;};d.ta=function(){function eZ(d4,pP){var g=d4.toString(
16).toUpperCase();var nS=pP-g.length;if(nS>0)g=d.jd.slice(0,nS)+g;return g;}var jM=
arguments[0];var bL="EmWi trace: ";var q=0;var cj=1;while(jM&&(q<jM.length)&&(cj<
arguments.length)){if(jM[q++]==='%'){if(cj>1)bL+=", ";switch(jM[q++]){case'i':bL+=
arguments[cj].toFixed();break;case'u':bL+=arguments[cj].toFixed();break;case'f':
bL+=arguments[cj];break;case'e':bL+="enum:0x"+eZ(arguments[cj],8);break;case't':
bL+="set:0x"+eZ(arguments[cj],8);break;case'0':bL+="null";break;case'b':bL+=arguments[
cj];break;case'h':bL+="handle:"+arguments[cj];break;case'c':{var g=arguments[cj];
if((g>=0x20)&&(g<=0x7F))bL+="\'"+String.fromCharCode(g)+"\'";else bL+="\'\\x"+eZ(
g,4)+"\'";}break;case'o':{var g=arguments[cj];var aw=g&0xFF;var cy=(g>>8)&0xFF;var
aW=(g>>16)&0xFF;var bG=(g>>24)&0xFF;bL+="#"+eZ(aw,2)+eZ(cy,2)+eZ(aW,2)+eZ(bG,2);
}break;case'p':{var g=arguments[cj];bL+="<"+g[0]+","+g[1]+">";}break;case'r':{var
g=arguments[cj];bL+="<"+g[0]+","+g[1]+","+g[2]+","+g[3]+">";}break;case'l':{bL+=
"language:0x"+eZ(arguments[cj],8);}break;case'g':{bL+="styles:0x"+eZ(arguments[cj
],8);}break;case'^':{var g=arguments[cj];if(!g||!g[0])bL+="null";else bL+="ref:(Object of class '"+
g[0]._className+"', OnGet:'"+g[1].nE+"', OnSet:'"+g[2].nE+"')";}break;case'*':{var
g=arguments[cj];if(!g)bL+="null";else bL+="Object of class '"+g._className+"'";}
break;case'$':{var g=arguments[cj];if(!g)bL+="null";else bL+=g._className;}break;
case'&':{var g=arguments[cj];if(!g||!g[0])bL+="null";else bL+="slot:(Object of class '"+
g[0]._className+"', SlotProc:'"+g[1].nE+"')";}break;case's':{var g=arguments[cj];
bL+="\""+g+"\"";}break;}cj++;}}console.log(bL);};d._RequestUpdate=function(){if(
this.hX)return;var d$=this;var nV=window.requestAnimationFrame||window.webkitRequestAnimationFrame||
window.mozRequestAnimationFrame||window.oRequestAnimationFrame||window.msRequestAnimationFrame;
function n_(){d$.px();}if(nV)nV(n_);else window.setTimeout(n_,15);this.hX=true;};
d.mj=function(){if(this.hW)return;var d$=this;function qZ(){d$.pt();}window.setTimeout(
qZ,1000);this.hW=true;};d.px=function(){if(!this.hX)return;this.hX=false;try{var
nm=this.pz();if(this.f_&&this.bM.DoesNeedUpdate()){var aD=this._NewObject(this.Graphics.
Canvas,0);this.ba.lW=null;this.ba.fm=null;if(this.i6)this.f_.cx[0].iR=true;aD.AttachBitmap(
this.f_);this.bM.UpdateGE20(aD);aD.DetachBitmap();if(this.f_.cx[0].iR){this.ba.clear(
this.ba.COLOR_BUFFER_BIT);this.f_.cx[0].iR=false;}nm=true;}if(nm)this.mj();if(this.
c0||this.dG)this._RequestUpdate();}catch(bs){console.log("EmWi error: "+bs+"!");
}};d.pt=function(){if(!this.hW)return;this.hW=false;this.mi();};function o5(kR,t
){var ny=[t.Enter,t.Escape,t.Backspace,t.CtrlShiftPageUp,t.CtrlShiftPageDown,t.CtrlShiftUp
,t.CtrlShiftDown,t.CtrlShiftKeyP,t.CtrlShiftKeyR,t.CtrlShiftLeft,t.CtrlShiftRight
,t.CtrlShiftBackspace,t.CtrlShiftKeyE,t.CtrlShiftKeyT,t.CtrlShiftKeyN,t.CtrlShiftKeyK
,t.CtrlShiftKeyG,t.CtrlShiftKeyU,t.CtrlShiftKeyA,t.CtrlShiftKeyD,t.CtrlShiftKeyM
,t.CtrlShiftKey0,t.CtrlShiftKey1,t.CtrlShiftKey2,t.CtrlShiftKey3,t.CtrlShiftKey4
,t.CtrlShiftKey5,t.CtrlShiftKey6,t.CtrlShiftKey7,t.CtrlShiftKey8,t.CtrlShiftKey9
,t.CtrlKeyM,t.CtrlPageUp,t.CtrlPageDown,t.CtrlInsert,t.CtrlDelete,t.CtrlKeyP,t.CtrlKeyS
,t.CtrlLeft,t.CtrlRight,t.CtrlUp,t.CtrlDown,t.CtrlKeyH,t.CtrlKeyR,t.CtrlKeyG,t.CtrlKeyB
,t.CtrlKeyY,t.CtrlKeyL,t.CtrlKeyN,t.CtrlKeyW,t.CtrlKeyD,t.CtrlKeyV,t.CtrlKeyE,t.
CtrlKeyO,t.CtrlKeyT,t.CtrlKeyQ,t.CtrlKeyA,t.CtrlKeyC,t.CtrlKeyU,t.CtrlKeyZ,t.CtrlKeyX
,t.CtrlKeyI,t.CtrlHome,t.CtrlEnd,t.CtrlKey0,t.CtrlKey1,t.CtrlKey2,t.CtrlKey3,t.CtrlKey4
,t.CtrlKey5,t.CtrlKey6,t.CtrlKey7,t.CtrlKey8,t.CtrlKey9];var qT=[t.Ok,t.Exit,t.Clear
,t.ChannelUp,t.ChannelDown,t.SkipPrev,t.SkipNext,t.Pause,t.Record,t.SlowRev,t.SlowFwd
,t.Eject,t.Repeat,t.Timer,t.Navigation,t.Karaoke,t.Game,t.Setup,t.Angle,t.Mode,t.
Mute,t.User10,t.User11,t.User12,t.User13,t.User14,t.User15,t.User16,t.User17,t.User18
,t.User19,t.Menu,t.VolumeUp,t.VolumeDown,t.Show,t.Hide,t.Play,t.Stop,t.Rev,t.Fwd
,t.SkipBwd,t.SkipFwd,t.Help,t.Red,t.Green,t.Blue,t.Yellow,t.White,t.Magenta,t.TV
,t.DVD,t.VCR,t.EPG,t.OSD,t.Text,t.PIP,t.Audio,t.Clock,t.Subtitle,t.Zoom,t.Index,
t.Info,t.Display,t.Power,t.User0,t.User1,t.User2,t.User3,t.User4,t.User5,t.User6
,t.User7,t.User8,t.User9];var q=ny.length-1;while((q>=0)&&(ny[q]!==kR))q--;if(q>=
0)kR=qT[q];return kR;}d.pp=function(ao){var c=this.Core.KeyCode;var lk=ao.type===
"keyup";var keyCode=c.NoKey;var gO=c.NoKey;var charCode=0;if(((ao.type==="keydown"
)||(ao.type==="keypress"))&&this.kv){var aa=this.kv.call(this,ao);if(aa&&aa.KeyCode
)keyCode=aa.KeyCode;else if(aa&&aa.dS)charCode=aa.dS;}if((ao.type==="keypress")&&
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
}gO=o5(keyCode,c);if(gO===this.dh)keyCode=this.dh;else if(keyCode===this.dh)gO=this.
dh;}if((keyCode===c.NoKey)&&!charCode&&(!lk||((this.dh===c.NoKey)&&!this.eH)))return;
try{if((this.dh!==c.NoKey)&&((keyCode!==this.dh)||lk)){this.bM.DriveKeyboardHitting(
this.dh,0,false);this.dh=c.NoKey;}if(this.eH&&((charCode!==this.eH)||lk)){this.bM.
DriveKeyboardHitting(c.NoKey,this.eH,false);this.eH=0;}if(charCode&&this.bM.DriveKeyboardHitting(
c.NoKey,charCode,true))this.eH=charCode;else if((gO!==keyCode)&&(gO!==c.NoKey)&&
this.bM.DriveKeyboardHitting(gO,0,true))this.dh=gO;else if((keyCode!==c.NoKey)&&
this.bM.DriveKeyboardHitting(keyCode,0,true))this.dh=keyCode;this._RequestUpdate(
);}catch(bs){console.log("EmWi error: "+bs+"!");}ao.stopPropagation();ao.preventDefault(
);};d.po=function(ao){var c=this.Core.KeyCode;try{if(this.dh!==c.NoKey){if(this.
bM.DriveKeyboardHitting)this.bM.DriveKeyboardHitting(this.dh,0,false);else{var iv=
this._NewObject(this.Core.KeyEvent,0);iv.Initialize(this.dh,false);this.bM.DispatchEvent(
iv);}this.dh=c.NoKey;this._RequestUpdate();}if(this.eH){if(this.bM.DriveKeyboardHitting
)this.bM.DriveKeyboardHitting(c.NoKey,this.eH,false);else{var iv=this._NewObject(
this.Core.KeyEvent,0);iv.Initialize2(this.eH,false);this.bM.DispatchEvent(iv);}this.
eH=0;this._RequestUpdate();}}catch(bs){console.log("EmWi error: "+bs+"!");}};function
kf(ao,di){ao.stopPropagation();ao.preventDefault();var an=ao.clientX+window.pageXOffset;
var aG=ao.clientY+window.pageYOffset;var es=di;for(;es;es=es.offsetParent){an-=es.
offsetLeft;aG-=es.offsetTop;}an=((di.width*an)/di.offsetWidth)|0;aG=((di.height*
aG)/di.offsetHeight)|0;return[an,aG];};d.pq=function(ao){if(this.ej||ao.p8)return;
this.dX.focus();document.addEventListener("mousemove",this.g6,true);document.addEventListener(
"mouseup",this.i$,true);this.ej=true;var ak=kf(ao,this.dX);try{this.bM.DriveCursorHitting(
true,0,ak);this._RequestUpdate();}catch(bs){console.log("EmWi error: "+bs+"!");}
};d.ps=function(ao){if(!this.ej||ao.p8)return;document.removeEventListener("mousemove"
,this.g6,true);document.removeEventListener("mouseup",this.i$,true);this.ej=false;
var ak=kf(ao,this.dX);try{this.bM.DriveCursorHitting(false,0,ak);this._RequestUpdate(
);}catch(bs){console.log("EmWi error: "+bs+"!");}};d.pr=function(ao){if(!this.ej
)return;var ak=kf(ao,this.dX);try{this.bM.DriveCursorMovement(ak);this._RequestUpdate(
);}catch(bs){console.log("EmWi error: "+bs+"!");}};function kh(ao,mJ,di){var touches=
ao.changedTouches;var iM=null;for(var h=0;!iM&&(h<touches.length);h++)if((mJ===-
1)||(touches[h].qN===mJ))iM=touches[h];if(!iM)return null;ao.stopPropagation();ao.
preventDefault();var an=iM.clientX;var aG=iM.clientY;var es=di;for(;es;es=es.offsetParent
){an-=es.offsetLeft;aG-=es.offsetTop;}an=((di.width*an)/di.offsetWidth)|0;aG=((di.
height*aG)/di.offsetHeight)|0;return[an,aG];}d.pw=function(ao){var ak;if(this.ej||
!(ak=kh(ao,-1,this.dX)))return;this.dX.focus();this.g_=ao.touches[0].qN;this.ej=
true;try{this.bM.DriveCursorHitting(true,0,ak);this._RequestUpdate();}catch(bs){
console.log("EmWi error: "+bs+"!");}};d.pu=function(ao){var ak;if(!this.ej||(this.
g_===-1)||!(ak=kh(ao,this.g_,this.dX)))return;this.ej=false;this.g_=-1;try{this.
bM.DriveCursorHitting(false,0,ak);this._RequestUpdate();}catch(bs){console.log("EmWi error: "+
bs+"!");}};d.pv=function(ao){var ak;if(!this.ej||(this.g_===-1)||!(ak=kh(ao,this.
g_,this.dX)))return;try{this.bM.DriveCursorMovement(ak);this._RequestUpdate();}catch(
bs){console.log("EmWi error: "+bs+"!");}};d._Init=function(di,d3){var d$=this;var
aD=document.getElementById(di);var mP=this.l_();var a=null;var at={};var bitmap={
};this.pk=(new Date()).getTime();if(d3&&d3.hasOwnProperty("Debug"))this.a4=d3.Debug;
if(d3&&d3.hasOwnProperty("FullScreenUpdate"))this.i6=d3.FullScreenUpdate;if(d3&&
d3.hasOwnProperty("Alpha"))this.kq=d3.Alpha;if(d3&&d3.hasOwnProperty("OnKeyEvent"
))this.kv=d3.OnKeyEvent;if(this.a4)console.log("EmWi debug: Initializing application '"+
mP._className+"'.");console.log("EmWi info: Init application for canvas '"+di+"'. "+
"FullScreenUpdate="+this.i6+", "+"Alpha="+this.kq+", "+"Debug="+this.a4+".");if(
!aD){console.log("EmWi error: Initialization failed. The canvas element '"+di+"' doesn't exist!"
);return false;}var fE={depth:false,antialias:false,alpha:this.kq,preserveDrawingBuffer:
!this.i6};a=aD.getContext("webgl",fE)||aD.getContext("experimental-webgl",fE);if(
!a){console.log("EmWi error: WebGL context creation failed.");return false;}this.
ga=a.getParameter(a.MAX_TEXTURE_SIZE);if(this.pj>this.ga)this.pj=this.ga;this.pi=(
this.i9/this.f$)|0;a.clearColor(0.0,0.0,0.0,0.0);a.pixelStorei(a.UNPACK_PREMULTIPLY_ALPHA_WEBGL
,true);a.pixelStorei(a.UNPACK_ALIGNMENT,1);a.blendEquation(a.FUNC_ADD);a.blendFunc(
a.ONE,a.ONE_MINUS_SRC_ALPHA);a.disable(a.BLEND);a.cs=new ArrayBuffer(14*6*4);a.gQ=
new Float32Array(a.cs);a.hM=new Uint32Array(a.cs);a.j9=0;a.j$=1;a.j_=2;a.bindBuffer(
a.ARRAY_BUFFER,a.createBuffer());a.bufferData(a.ARRAY_BUFFER,a.cs,a.STREAM_DRAW);
a.vertexAttribPointer(a.j9,2,a.FLOAT,false,6*4,0*4);a.vertexAttribPointer(a.j$,3
,a.FLOAT,false,6*4,2*4);a.vertexAttribPointer(a.j_,4,a.UNSIGNED_BYTE,true,6*4,5*
4);a.enableVertexAttribArray(a.j9);a.enableVertexAttribArray(a.j$);a.enableVertexAttribArray(
a.j_);this.hU=a.createTexture();a.bindTexture(a.TEXTURE_2D,this.hU);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_MIN_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MAG_FILTER
,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.CLAMP_TO_EDGE);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_WRAP_T,a.CLAMP_TO_EDGE);a.texImage2D(a.TEXTURE_2D,0,a.ALPHA
,this.i9,this.i9,0,a.ALPHA,a.UNSIGNED_BYTE,null);this.g5=new Uint32Array(this.f$
*this.f$/32);at.Framebuffer=a.getParameter(a.FRAMEBUFFER_BINDING);at.Format=this.
b9;at.Width=aD.width;at.Height=aD.height;at.hS=aD.width;at.hR=aD.height;at.dW=1/
aD.width;at.dV=1/aD.height;at.eE=2/aD.width;at.eD=2/aD.height;at.cB=null;bitmap.
Format=this.b9;bitmap.cx=[at];bitmap.FrameSize=[aD.width,aD.height];bitmap.FrameDelay=
0;bitmap.NoOfFrames=1;bitmap.bT=[{aH:bitmap.cx[0],aL:[0,0]}];if(!oP(a)){console.
log("EmWi error: WebGL initialization failed.");return false;}if(this.a4)ei(a);a.
or=[0,0,0,-1,0,0,0,-1,0,0,0,0,0,0,0,1];this.dX=aD;this.ba=a;this.f_=bitmap;try{this.
bM=this._NewObject(mP,0);this.bM.Initialize(this.sN);}catch(bs){console.log("EmWi error: "+
bs+"!");this._Done();return false;}function nD(ao){d$.pq(ao);}function qX(ao){d$.
ps(ao);}function ln(ao){d$.pr(ao);}function n7(ao){d$.pw(ao);}function ly(ao){d$.
pu(ao);}function n6(ao){d$.pv(ao);}function jU(ao){d$.pp(ao);}function mR(ao){d$.
po(ao);}this.ku=nD;this.i$=qX;this.g6=ln;this.ml=n7;this.kE=ly;this.mk=n6;this.i_=
jU;this.rv=mR;aD.addEventListener("mousedown",nD,true);aD.addEventListener("mousemove"
,ln,false);aD.addEventListener("mouseout",ln,false);aD.addEventListener("touchstart"
,n7,true);aD.addEventListener("touchend",ly,false);aD.addEventListener("touchcancel"
,ly,false);aD.addEventListener("touchmove",n6,false);aD.addEventListener("keydown"
,jU,true);aD.addEventListener("keyup",jU,false);aD.addEventListener("keypress",jU
,false);aD.addEventListener("blur",mR,false);this._RequestUpdate();this.mj();this.
_LockObject(this.bM);aD.focus();return true;};d._Done=function(){if(this.a4)console.
log("EmWi debug: De-Initializing application '"+this.l_()._className+"'.");if(this.
dX&&this.ku){var aD=this.dX;aD.removeEventListener("mousedown",this.ku,true);aD.
removeEventListener("mousemove",this.g6,false);aD.removeEventListener("mouseout"
,this.g6,false);aD.removeEventListener("touchstart",this.ml,true);aD.removeEventListener(
"touchend",this.kE,false);aD.removeEventListener("touchcancel",this.kE,false);aD.
removeEventListener("touchmove",this.mk,false);aD.removeEventListener("keydown",
this.i_,true);aD.removeEventListener("keyup",this.i_,false);aD.removeEventListener(
"keypress",this.i_,false);document.removeEventListener("mousemove",this.g6,true);
document.removeEventListener("mouseup",this.i$,true);delete this.ku;delete this.
i$;delete this.g6;delete this.ml;delete this.kE;delete this.mk;delete this.i_;}if(
this.ba){g1(this.ba,null);dt(this.ba,null,null);this.ba.clear(this.ba.COLOR_BUFFER_BIT
);this.ba.flush();this.ba.finish();this.ba.hQ=null;this.ba.gY=null;this.ba.gZ=null;
this.ba.i0=null;this.ba.g0=null;this.ba.i1=null;this.ba.f9=null;this.ba.i2=null;
ei(this.ba);}while(this.fp)ko(this,this.fp);this.hU=null;this.g5=null;this.fp=null;
this.i8=null;this.f_=null;this.ba=null;this.dX=null;this.c0=null;this.dG=null;if(
this.bM)this._UnlockObject(this.bM);this.bM=null;try{this.mi();}catch(bs){console.
log("EmWi error: "+bs+"!");}if(this.a4)console.log("EmWi debug: "+this.gb+" objects after the final GC run."
);this.g9=[];this.g7=null;this.gb=0;this.me=0;this.hX=false;this.hW=false;this.ej=
false;};d.sq=function(ax,af){return[ax[0]+af[0],ax[1]+af[1],ax[2]+af[0],ax[3]+af[
1]];};d.sp=function(ax,af){return[ax[0]-af[0],ax[1]-af[1],ax[2]-af[0],ax[3]-af[1
]];};d.hV=function(ch,ci){var bP=ch[0];var bR=ch[1];var bQ=ch[2];var bS=ch[3];var
dc=ci[0];var de=ci[1];var dd=ci[2];var df=ci[3];if(bQ<=bP)bP=bQ=0;if(bS<=bR)bR=bS=
0;if(dd<=dc)dc=dd=0;if(df<=de)de=df=0;if(bP<dc)bP=dc;if(bQ>dd)bQ=dd;if(bR<de)bR=
de;if(bS>df)bS=df;if(bQ<=bP)bP=bQ=0;if(bS<=bR)bR=bS=0;return[bP,bR,bQ,bS];};d.r_=
function(ch,ci){var bP=ch[0];var bR=ch[1];var bQ=ch[2];var bS=ch[3];var dc=ci[0];
var de=ci[1];var dd=ci[2];var df=ci[3];if(bQ<=bP)bP=bQ=0;if(bS<=bR)bR=bS=0;if(dd<=
dc)dc=dd=0;if(df<=de)de=df=0;if((bP===bQ)||(bR===bS))return ci;if((dc===dd)||(de===
df))return ch;if(bP<dc)bP=dc;if(bQ>dd)bQ=dd;if(bR<de)bR=de;if(bS>df)bS=df;if(bQ<=
bP)bP=bQ=0;if(bS<=bR)bR=bS=0;return[bP,bR,bQ,bS];};d.pA=function(ch,ci){var bP=ch[
0];var bR=ch[1];var bQ=ch[2];var bS=ch[3];var dc=ci[0];var de=ci[1];var dd=ci[2];
var df=ci[3];if(bQ<=bP)bP=bQ=0;if(bS<=bR)bR=bS=0;if(dd<=dc)dc=dd=0;if(df<=de)de=
df=0;if((bP===bQ)||(bR===bS))return ci;if((dc===dd)||(de===df))return ch;if(bP>dc
)bP=dc;if(bQ<dd)bQ=dd;if(bR>de)bR=de;if(bS<df)bS=df;if(bQ<=bP)bP=bQ=0;if(bS<=bR)
bR=bS=0;return[bP,bR,bQ,bS];};d.r7=function(ax,jf){return[ax[0]-jf[0],ax[1]-jf[1
],ax[2]+jf[0],ax[3]+jf[1]];};d.sY=function(ax,p5){return[ax[0],p5,ax[2],ax[3]];};
d.sW=function(ax,p4){return[ax[0],ax[1],p4,ax[3]];};d.sV=function(ax,aY){return[
aY,ax[1],aY+ax[2]-ax[0],ax[3]];};d.sX=function(ax,bk){return[ax[0],bk,ax[2],bk+ax[
3]-ax[1]];};d.sU=function(ax,p3){return[ax[0],ax[1],ax[0]+p3,ax[3]];};d.sR=function(
ax,pJ){return[ax[0],ax[1],ax[2],ax[1]+pJ];};d.sS=function(ax,jj){return[jj[0],jj[
1],jj[0]+ax[2]-ax[0],jj[1]+ax[3]-ax[1]];};d.sT=function(ax,bf){return[ax[0],ax[1
],ax[0]+bf[0],ax[1]+bf[1]];};d.sb=function(ch,ci){return(ch[0]===ci[0])&&(ch[1]===
ci[1])&&(ch[2]===ci[2])&&(ch[3]===ci[3]);};d.se=function(ax,eI){if((eI[0]<ax[0])||(
eI[0]>=ax[2]))return false;if((eI[1]<ax[1])||(eI[1]>=ax[3]))return false;return true;
};d.rY=function(ax){return(ax[2]-ax[0])*(ax[3]-ax[1]);};d.rZ=function(ax){var an=
ax[0];var aG=ax[1];return[an+((ax[2]-an)>>1),aG+((ax[3]-aG)>>1)];};d.so=function(
eI,af){return[eI[0]+af[0],eI[1]+af[1]];};d.sn=function(eI,af){return[eI[0]-af[0]
,eI[1]-af[1]];};d.sa=function(mE,mF){return(mE[0]===mF[0])&&(mE[1]===mF[1]);};d.
sE=function(a5,a_){var aw=(a5&0xFF)+(a_&0xFF);var cy=((a5>>8)&0xFF)+((a_>>8)&0xFF
);var aW=((a5>>16)&0xFF)+((a_>>16)&0xFF);var bG=((a5>>24)&0xFF)+((a_>>24)&0xFF);
if(aw>255)aw=255;if(cy>255)cy=255;if(aW>255)aW=255;if(bG>255)bG=255;return aw|(cy<<
8)|(aW<<16)|(bG<<24);};d.sm=function(a5,a_){var aw=(a5&0xFF)-(a_&0xFF);var cy=((
a5>>8)&0xFF)-((a_>>8)&0xFF);var aW=((a5>>16)&0xFF)-((a_>>16)&0xFF);var bG=((a5>>
24)&0xFF)-((a_>>24)&0xFF);if(aw<0)aw=0;if(cy<0)cy=0;if(aW<0)aW=0;if(bG<0)bG=0;return aw|(
cy<<8)|(aW<<16)|(bG<<24);};d.rt=function(a5,a_){var aw=(a_&0xFF);var cy=((a_>>8)&
0xFF);var aW=((a_>>16)&0xFF);var bG=((a_>>24)&0xFF);var jV=255-bG;aw=((aw*bG)+((
a5&0xFF)*jV))/255;cy=((cy*bG)+(((a5>>8)&0xFF)*jV))/255;aW=((aW*bG)+(((a5>>16)&0xFF
)*jV))/255;bG=255-((jV*(255-((a5>>24)&0xFF)))/255);if(aw>255)aw=255;if(cy>255)cy=
255;if(aW>255)aW=255;if(bG>255)bG=255;return aw|(cy<<8)|(aW<<16)|(bG<<24);};d.ru=
function(cC,fr){var aw=(cC&0xFF);var cy=((cC>>8)&0xFF);var aW=((cC>>16)&0xFF);var
bG=((cC>>24)&0xFF);if(fr<0)fr=0;if(fr>255)fr=255;aw=(aw*fr)/255;cy=(cy*fr)/255;aW=(
aW*fr)/255;bG=(bG*fr)/255;return aw|(cy<<8)|(aW<<16)|(bG<<24);};d.sv=function(d4
,ag,b0){if((b0!==2)&&(b0!==8)&&(b0!==16))b0=10;if(ag>128)ag=128;var g=d4.toString(
b0);var f7=ag-g.length;if(f7>0)g=d.jd.slice(0,f7)+g;return g;};d.su=function(d4,
ag,b0){if((b0!==2)&&(b0!==8)&&(b0!==16))b0=10;var gP=(d4<0)?-d4:d4;if(ag>128)ag=
128;var g=gP.toString(b0);var f7=ag-g.length-((d4<0)?1:0);if(f7>0)g=d.jd.slice(0
,f7)+g;if(d4<0)return"-"+g;else return g;};d.st=function(d4,ag,h_){if(ag<0)ag=0;
if(ag>32)ag=32;if(h_<0)h_=0;if(h_>32)h_=32;var n0=(d4<0)?"-":"";var g=Math.abs(d4
).toFixed(h_);var f7=ag-g.length-n0.length;if(f7>0)g=d.jd.slice(0,f7)+g;return n0+
g;};d.ss=function(kP,ag){ag=Math.floor(ag);if(ag<=0)return this.g3;return new Array(
ag+1).join(String.fromCharCode(kP));};d.s3=function(l,ag){if(ag<=0)return this.g3;
if(ag>=l.length)return l;return l.slice(0,ag|0);};d.s9=function(l,ag){if(ag<=0)return this.
g3;if(ag>=l.length)return l;return l.slice(-(ag|0));};d.s4=function(l,a$,ag){var
aX=(a$>0)?(a$|0):0;var cI=(a$+ag)|0;var bc=l.length;if((aX>=bc)||(cI<=0)||(aX>=cI
))return this.g3;if((aX===0)&&(cI>=bc))return l;return l.slice(aX,cI);};d.s2=function(
hf,ik,a$){var n2=hf.length;var q0=ik.length;if(q0<=0)return hf;if(n2<=0)return ik;
if(a$<0)return ik+hf;if(a$>=n2)return hf+ik;return hf.slice(0,(a$|=0))+ik+hf.slice(
a$);};d.s8=function(l,a$,ag){var aX=(a$>=0)?(a$|0):0;var cI=(a$+ag)|0;var bc=l.length;
if((aX>=bc)||(cI<=0)||(aX>=cI))return l;return l.slice(0,aX)+l.slice(cI);};d.s6=
function(l,h4,b0){if((b0!==2)&&(b0!==8)&&(b0!==16))b0=10;var g=parseInt(l,b0);if(
!isNaN(g))return g;return h4;};d.s7=function(l,h4,b0){if((b0!==2)&&(b0!==8)&&(b0
!==16))b0=10;var g=parseInt(l,b0);if(!isNaN(g)){if(g<0)g+=0x100000000;return g;}
return h4;};d.s5=function(l,h4){var g=parseFloat(l);if(!isNaN(g))return g;return h4;
};d.sZ=function(l,a$,kP){if((a$<0)||(a$>=l.length))return l;return l.slice(0,a$)+
String.fromCharCode(kP)+l.slice(a$+1);};d.sc=function(h$,ia){if(!h$&&!ia)return true;
if(!h$||!ia)return false;return(h$[0]===ia[0])&&(h$[1]===ia[1])&&(h$[2]===ia[2]);
};d.sd=function(jl,jm){if(!jl&&!jm)return true;if(!jl||!jm)return false;return(jl[
0]===jm[0])&&(jl[1]===jm[1]);};d.sJ=function(il,k3){return(il+((k3-il)*Math.random(
)))|0;};d.sI=function(il,k3){return il+((k3-il)*Math.random());};function ei(m){
var mO=false;var nn;while((nn=m.getError())!==m.NO_ERROR){console.log("EmWi error: WebGL error '"+
nn+"' reported.");mO=true;}return mO;}var o9="precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+
"attribute highp   vec2 dst_pos;\n"+"\n"+"void main()\n"+"{\n"+"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+
"}\n";var o_="precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+"attribute highp   vec2 dst_pos;\n"+
"attribute lowp    vec4 src_clr;\n"+"varying   lowp    vec4 src_clr_var;\n"+"\n"+
"void main()\n"+"{\n"+"  src_clr_var = vec4( src_clr.rgb * src_clr.a, src_clr.a );\n"+
"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+"}\n";var o$=
"precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+"attribute highp   vec2 dst_pos;\n"+
"attribute highp   vec3 src_pos;\n"+"varying   highp   vec3 src_pos_var;\n"+"\n"+
"void main()\n"+"{\n"+"  src_pos_var = src_pos;\n"+"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+
"}\n";var pa="precision mediump float;\n"+"uniform   highp   mat4 dst_map;\n"+"attribute highp   vec2 dst_pos;\n"+
"attribute highp   vec3 src_pos;\n"+"attribute lowp    vec4 src_clr;\n"+"varying   highp   vec3 src_pos_var;\n"+
"varying   lowp    vec4 src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  src_clr_var = vec4( src_clr.rgb * src_clr.a, src_clr.a );\n"+
"  src_pos_var = src_pos;\n"+"  gl_Position = vec4( dst_pos.x, dst_pos.y, 0.0, 1.0 ) * dst_map;\n"+
"}\n";var ox="precision mediump float;\n"+"void main()\n"+"{\n"+"  gl_FragColor = vec4( 0.0, 0.0, 0.0, 0.0 );\n"+
"}\n";var oy="precision mediump float;\n"+"varying   lowp vec4 src_clr_var;\n"+"\n"+
"void main()\n"+"{\n"+"  gl_FragColor = src_clr_var;\n"+"}\n";var oz="precision mediump float;\n"+
"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_pos_var.xy );\n"+
"}\n";var oA="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2DProj( src, src_pos_var );\n"+
"}\n";var oB="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_pos_var.xy ) * src_clr_var.a;\n"+
"}\n";var oC="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2DProj( src, src_pos_var ) * src_clr_var.a;\n"+
"}\n";var oD="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_pos_var.xy ).a * src_clr_var;\n"+
"}\n";var oE="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2DProj( src, src_pos_var ).a * src_clr_var;\n"+
"}\n";var oF="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"uniform   highp   vec2      src_size;\n"+
"uniform   highp   vec2      src_ofs;\n"+"varying   highp   vec3      src_pos_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_ofs + fract( src_pos_var.xy ) * src_size );\n"+
"}\n";var oG="precision mediump float;\n"+"uniform   lowp    sampler2D src;\n"+"uniform   highp   vec2      src_size;\n"+
"uniform   highp   vec2      src_ofs;\n"+"varying   highp   vec3      src_pos_var;\n"+
"varying   lowp    vec4      src_clr_var;\n"+"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_ofs + fract( src_pos_var.xy ) * src_size )\n"+
"                            * src_clr_var.a;\n"+"}\n";var oH="precision mediump float;\n"+
"uniform   lowp    sampler2D src;\n"+"uniform   highp   vec2      src_size;\n"+"uniform   highp   vec2      src_ofs;\n"+
"varying   highp   vec3      src_pos_var;\n"+"varying   lowp    vec4      src_clr_var;\n"+
"\n"+"void main()\n"+"{\n"+"  gl_FragColor = texture2D( src, src_ofs + fract( src_pos_var.xy ) * src_size ).a\n"+
"                            * src_clr_var;\n"+"}\n";function oP(m){var q_=dg(m,
m.VERTEX_SHADER,o9);var q$=dg(m,m.VERTEX_SHADER,o_);var lA=dg(m,m.VERTEX_SHADER,
o$);var hL=dg(m,m.VERTEX_SHADER,pa);var qB=dg(m,m.FRAGMENT_SHADER,ox);var qC=dg(
m,m.FRAGMENT_SHADER,oy);var qD=dg(m,m.FRAGMENT_SHADER,oz);var qE=dg(m,m.FRAGMENT_SHADER
,oA);var qF=dg(m,m.FRAGMENT_SHADER,oB);var qG=dg(m,m.FRAGMENT_SHADER,oC);var qH=
dg(m,m.FRAGMENT_SHADER,oD);var qI=dg(m,m.FRAGMENT_SHADER,oE);var qJ=dg(m,m.FRAGMENT_SHADER
,oF);var qK=dg(m,m.FRAGMENT_SHADER,oG);var qL=dg(m,m.FRAGMENT_SHADER,oH);m.hQ=eg(
m,q_,qB);m.gY=eg(m,q$,qC);m.gZ=eg(m,lA,qD);m.i0=eg(m,lA,qE);m.g0=eg(m,hL,qF);m.i1=
eg(m,hL,qG);m.f9=eg(m,hL,qH);m.i2=eg(m,hL,qI);m.kl=eg(m,lA,qJ);m.km=eg(m,hL,qK);
m.kn=eg(m,hL,qL);if(m.hQ&&m.gY&&m.gZ&&m.i0&&m.g0&&m.i1&&m.f9&&m.i2&&m.kl&&m.km&&
m.kn)return true;m.hQ=null;m.gY=null;m.gZ=null;m.i0=null;m.g0=null;m.i1=null;m.f9=
null;m.i2=null;m.kl=null;m.km=null;m.kn=null;return false;}function dg(m,pV,pW){
var iD=m.createShader(pV);m.shaderSource(iD,pW);m.compileShader(iD);var a8=m.getShaderParameter(
iD,m.COMPILE_STATUS);var log=m.getShaderInfoLog(iD);if(a8&&log.length)console.log(
"EmWi info: "+log);if(!a8)console.log("EmWi error: "+log);if(!a8)return null;return iD;
}function eg(m,mL,mw){var cU=m.createProgram();if(!mL||!mw)return null;m.attachShader(
cU,mL);m.attachShader(cU,mw);m.bindAttribLocation(cU,m.j9,"dst_pos");m.bindAttribLocation(
cU,m.j$,"src_pos");m.bindAttribLocation(cU,m.j_,"src_clr");m.linkProgram(cU);var
a8=m.getProgramParameter(cU,m.LINK_STATUS);var log=m.getProgramInfoLog(cU);if(a8&&
log.length)console.log("EmWi info: "+log);if(!a8)console.log("EmWi error: "+log);
if(!a8)return null;var q5=m.getUniformLocation(cU,"dst_map");var n9=m.getUniformLocation(
cU,"src");var q6=m.getUniformLocation(cU,"src_ofs");var q7=m.getUniformLocation(
cU,"src_size");m.useProgram(cU);if(n9>=0)m.uniform1i(n9,0);cU.o6=q5;cU.o7=q6;cU.
o8=q7;return cU;}function dt(m,x,em){if(x!==m.lW){if(x&&x.cB&&!x.Framebuffer){x.
Framebuffer=m.createFramebuffer();m.bindFramebuffer(m.FRAMEBUFFER,x.Framebuffer);
m.framebufferTexture2D(m.FRAMEBUFFER,m.COLOR_ATTACHMENT0,m.TEXTURE_2D,x.cB,0);}else
if(x)m.bindFramebuffer(m.FRAMEBUFFER,x?x.Framebuffer:null);if(x)m.viewport(0,0,x.
hS,x.hR);m.lW=x;}if(m.om!==em){m.useProgram(em);m.om=em;}if(x&&em&&((x.eE!==em.eE
)||(x.eD!==em.eD)||(!!x.cB!==!!em.cB))){var gF=m.or;if(x.cB){gF[0]=x.eE;gF[5]=x.
eD;gF[7]=-1;}else{gF[0]=x.eE;gF[5]=-x.eD;gF[7]=1;}m.uniformMatrix4fv(em.o6,false
,gF);em.eE=x.eE;em.eD=x.eD;em.cB=!!x.cB;}if(x&&x.iR){m.clear(m.COLOR_BUFFER_BIT);
x.iR=false;}}function g1(m,aC,fu){if(aC!==m.fm){if(aC)m.bindTexture(m.TEXTURE_2D
,aC.cB);m.fm=aC;}if(aC&&(!aC.os!==!fu)){if(fu){m.texParameteri(m.TEXTURE_2D,m.TEXTURE_MIN_FILTER
,m.LINEAR);m.texParameteri(m.TEXTURE_2D,m.TEXTURE_MAG_FILTER,m.LINEAR);}else{m.texParameteri(
m.TEXTURE_2D,m.TEXTURE_MIN_FILTER,m.NEAREST);m.texParameteri(m.TEXTURE_2D,m.TEXTURE_MAG_FILTER
,m.NEAREST);}aC.os=fu;}}function aQ(a5,a_,hd){if(hd<=0)return a5;if(hd>=1)return a_;
var k7=(a5&0xFF);var k8=((a5>>8)&0xFF);var k9=((a5>>16)&0xFF);var k_=((a5>>24)&0xFF
);var p9=(a_&0xFF);var p_=((a_>>8)&0xFF);var p$=((a_>>16)&0xFF);var qa=((a_>>24)&
0xFF);k7+=(p9-k7)*hd;k8+=(p_-k8)*hd;k9+=(p$-k9)*hd;k_+=(qa-k_)*hd;return k7|(k8<<
8)|(k9<<16)|(k_<<24);}function iU(bj,gr,cg,k0,dJ){for(var h=0;h<(cg-(k0?1:0));h++
){var bK=h;var cJ=(h+1)%cg;var jN=bj[bK*6];var lf=bj[cJ*6];if(((dJ>jN)&&(dJ<lf))||((
dJ<jN)&&(dJ>lf))){var dM=(dJ-jN)/(lf-jN);var e$=1.0-dM;var ev=(cg-h-1)*6;if(ev>0
){var aX=(h+1)*6;bj.set(bj.subarray(aX,aX+ev),aX+6);}cg++;if(cJ)cJ++;h++;var j=bj[
bK=bK*6+1];var b3=bj[bK+=1];var b4=bj[bK+=1];var ff=bj[bK+=1];var b1=gr[bK+1];var
n=bj[cJ=cJ*6+1];var cz=bj[cJ+=1];var bN=bj[cJ+=1];var iO=bj[cJ+=1];var d5=gr[cJ+
1];if(j!==n)j=j*e$+n*dM;if(b3!==cz)b3=b3*e$+cz*dM;if(b4!==bN)b4=b4*e$+bN*dM;if(ff
!==iO)ff=ff*e$+iO*dM;if(b1!==d5)b1=aQ(b1,d5,dM);bj[bK=h*6]=dJ;bj[bK+=1]=j;bj[bK+=
1]=b3;bj[bK+=1]=b4;bj[bK+=1]=ff;gr[bK+1]=b1;}}return cg;}function iT(bj,gr,cg,k0
,dJ){for(var h=0;h<(cg-(k0?1:0));h++){var bK=h;var cJ=(h+1)%cg;var jO=bj[bK*6+1];
var lg=bj[cJ*6+1];if(((dJ>jO)&&(dJ<lg))||((dJ<jO)&&(dJ>lg))){var dM=(dJ-jO)/(lg-
jO);var e$=1.0-dM;var ev=(cg-h-1)*6;if(ev>0){var aX=(h+1)*6;bj.set(bj.subarray(aX
,aX+ev),aX+6);}cg++;if(cJ)cJ++;h++;var k=bj[bK*=6];var b3=bj[bK+=2];var b4=bj[bK+=
1];var ff=bj[bK+=1];var b1=gr[bK+1];var p=bj[cJ*=6];var cz=bj[cJ+=2];var bN=bj[cJ+=
1];var iO=bj[cJ+=1];var d5=gr[cJ+1];if(k!==p)k=k*e$+p*dM;if(b3!==cz)b3=b3*e$+cz*
dM;if(b4!==bN)b4=b4*e$+bN*dM;if(ff!==iO)ff=ff*e$+iO*dM;if(b1!==d5)b1=aQ(b1,d5,dM
);bj[bK=h*6]=k;bj[bK+=1]=dJ;bj[bK+=1]=b3;bj[bK+=1]=b4;bj[bK+=1]=ff;gr[bK+1]=b1;}
}return cg;}function lP(s,cg,pL,pU){for(var h=0;h<cg;h++){var an=s[h*6];if((an<pL
)||(an>pU)){var ev=(cg-h-1)*6;if(ev>0){var aX=(h+1)*6;s.set(s.subarray(aX,aX+ev)
,aX-6);}h--;cg--;}}return cg;}function lO(s,cg,p0,pD){for(var h=0;h<cg;h++){var aG=
s[h*6+1];if((aG<p0)||(aG>pD)){var ev=(cg-h-1)*6;if(ev>0){var aX=(h+1)*6;s.set(s.
subarray(aX,aX+ev),aX-6);}h--;cg--;}}return cg;}var ka=[0x00000000,0x00000001,0x00000003
,0x00000007,0x0000000F,0x0000001F,0x0000003F,0x0000007F,0x000000FF,0x000001FF,0x000003FF
,0x000007FF,0x00000FFF,0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,0x0001FFFF,0x0003FFFF
,0x0007FFFF,0x000FFFFF,0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF,0x01FFFFFF,0x03FFFFFF
,0x07FFFFFF,0x0FFFFFFF,0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF,0xFFFFFFFF];function ov(
al,gn,fx){var qQ=al.f$-fx;var gD=al.f$>>5;for(var aw=0;aw<=qQ;aw++)for(var bJ=0;
bJ<gD;bJ++){var am=32-gn;var fc=ka[gn];var n$=al.g5[aw*gD+bJ];var ew;do{while(am&&(
n$&fc)){fc<<=1;am--;}ew=!(n$&fc);for(var lv=aw+1;ew&&(lv<(aw+fx));lv++)ew=!(al.g5[
lv*gD+bJ]&fc);if(!ew){fc<<=1;am--;}}while(!ew&&(am>0));if(ew)return[(bJ*32)+32-gn-
am,aw];}return null;}function o3(al,e0,ie,gn,fx){var fc=ka[gn]<<(e0&0x1F);var bJ=
e0>>5;var gD=al.f$>>5;for(;fx>0;ie++,fx--)al.g5[ie*gD+bJ]|=fc;}function o2(al,e0
,ie,gn,fx){var fc=~(ka[gn]<<(e0&0x1F));var bJ=e0>>5;var gD=al.f$>>5;for(;fx>0;ie++
,fx--)al.g5[ie*gD+bJ]&=fc;}function lJ(al,cf){cf.gW=al.fp;cf.eV=null;if(al.fp)al.
fp.eV=cf;else al.i8=cf;al.fp=cf;cf.Cache[cf.dS]=cf;}function ko(al,cf){if(cf.gW)
cf.gW.eV=cf.eV;else al.i8=cf.eV;if(cf.eV)cf.eV.gW=cf.gW;else al.fp=cf.gW;cf.gW=null;
cf.eV=null;cf.Cache[cf.dS]=null;}function dT(a1,e3,ag){var jP=0;var min=0;var max=
ag-1;var c5=-1;var ak;while(max>=min){jP=(max+min)>>1;ak=jP*8;c5=a1-e3.charCodeAt(
ak);if(!c5)return ak;if(c5>0)min=jP+1;else if(c5<0)max=jP-1;}return-1;}var gT=[[
0x10,0x01],[0x02,0x05],[0x03,0x04],[0x11,0x12],[0x14,0x18],[0x06,0x09],[0x07,0x08
],[0x13,0x16],[0x17,0x1C],[0x0A,0x0B],[0x1E,0x1F],[0x0C,0x0D],[0x15,0x19],[0x1A,
0x0E],[0x1B,0x1D]];function on(aC,aB,af,ag){var e;var q=0;var eC=0;var dL=0;var hp=
aB.length;eC=af>>4;af&=0xF;e=aC.charCodeAt(eC++)>>af;while(ag--){if(e&1)q=gT[q][
1];else q=gT[q][0];if(q>15){q-=16;aB[dL++]=(q&0x01)?0xFF:0x00;if(dL!==hp)aB[dL++
]=(q&0x02)?0xFF:0x00;if(dL!==hp)aB[dL++]=(q&0x04)?0xFF:0x00;if(dL!==hp)aB[dL++]=(
q&0x08)?0xFF:0x00;q=0;}if((af=(af+1))&0x0F)e>>=1;else e=aC.charCodeAt(eC++);}}function
oo(aC,aB,af,ag){var n8=[0x00,0x55,0xAA,0xFF];var e;var q=0;var eC=0;var dL=0;var
hp=aB.length;eC=af>>4;af&=0xF;e=aC.charCodeAt(eC++)>>af;while(ag--){if(e&1)q=gT[
q][1];else q=gT[q][0];if(q>15){q-=16;aB[dL++]=n8[q&0x03];if(dL!==hp)aB[dL++]=n8[
q>>2];q=0;}if((af=(af+1))&0x0F)e>>=1;else e=aC.charCodeAt(eC++);}}function op(aC
,aB,af,ag){var e;var q=0;var eC=0;var dL=0;var hp=aB.length;eC=af>>4;af&=0xF;e=aC.
charCodeAt(eC++)>>af;while(ag--){if(e&1)q=gT[q][1];else q=gT[q][0];if(q>15){q-=16;
aB[dL++]=q|(q<<4);q=0;}if((af=(af+1))&0x0F)e>>=1;else e=aC.charCodeAt(eC++);}}function
pb(be,ag,aA){var dN=0;var d7=aA;ag-=aA;while(ag--)be[d7++]^=be[dN++];}d.md=function(
jh,mx,pG,pQ){var bitmap={cx:[],bT:[]};var jY=Math.max(pQ,1);var width=mx[0];var height=
mx[1];var a=this.ba;if((jh!==d.b9)&&(jh!==d.kC)){console.log("EmWi error: Wrong pixel format of bitmap to create."
);return null;}if((width<=0)||(width>(this.ga-2))||(height<=0)||(height>(this.ga-
2))){console.log("EmWi error: Invalid size of bitmap to create.");return null;}var
fX=((this.ga-1)/(width+1))|0;var lm=((this.ga-1)/(height+1))|0;var hw=fX*lm;var nu=(
jY%hw)||hw;var nv=Math.min(nu,fX);var nw=((nu+fX-1)/fX)|0;var qW=fX*width+fX+1;var
qV=lm*height+lm+1;var qS=nv*width+nv+1;var qR=nw*height+nw+1;var nN=((jY+hw-1)/hw
)|0;for(var h=0;h<nN;h++){a.getError();var gN=qW;var gM=qV;var d_=a.createTexture(
);var at={};if(h===(nN-1)){gN=qS;gM=qR;}a.fm=null;a.bindTexture(a.TEXTURE_2D,d_);
a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MIN_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D
,a.TEXTURE_MAG_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.
CLAMP_TO_EDGE);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_T,a.CLAMP_TO_EDGE);if(
jh===this.b9)a.texImage2D(a.TEXTURE_2D,0,a.RGBA,gN,gM,0,a.RGBA,a.UNSIGNED_BYTE,null
);else a.texImage2D(a.TEXTURE_2D,0,a.ALPHA,gN,gM,0,a.ALPHA,a.UNSIGNED_BYTE,null);
at.Format=jh;at.Width=gN;at.Height=gM;at.hS=gN;at.hR=gM;at.dW=1/gN;at.dV=1/gM;at.
eE=2/gN;at.eD=2/gM;at.cB=d_;at.Framebuffer=null;bitmap.cx[h]=at;if(this.a4)ei(a);
}for(var h=0;h<jY;h++){var fT=h%hw;var nQ=fT%fX;var nR=(fT/fX)|0;var ck={};ck.aH=
bitmap.cx[(h/hw)|0];ck.aL=[(nQ*width)+nQ+1,(nR*height)+nR+1];bitmap.bT[h]=ck;}bitmap.
FrameSize=[width,height];bitmap.FrameDelay=pG;bitmap.NoOfFrames=jY;return bitmap;
};function l4(al,bH){try{var m9=bH.eX;var ey=bH.Image;var width=ey.width;var height=
ey.height;var gA=bH.FrameSize[0];var hr=bH.FrameSize[1];var nK=(width/gA)|0;var a=
al.ba;delete bH.eX;delete bH.Image;if(!m9.bitmap)return;if((ey.width===gA)&&(ey.
height===hr)){var ck=bH.bT[0];var d_=ck.aH.cB;var aU=ck.aL[0];var aE=ck.aL[1];a.
bindTexture(a.TEXTURE_2D,d_);a.fm=null;if(ck.aH.Format===al.b9)a.texSubImage2D(a.
TEXTURE_2D,0,aU,aE,a.RGBA,a.UNSIGNED_BYTE,ey);else a.texSubImage2D(a.TEXTURE_2D,
0,aU,aE,a.ALPHA,a.UNSIGNED_BYTE,ey);}else{var aD=document.createElement('canvas'
);var lb;aD.width=gA;aD.height=hr;lb=aD.getContext("2d");for(var h=0;h<bH.NoOfFrames;
h++){var ck=bH.bT[h];var d_=ck.aH.cB;var aU=ck.aL[0];var aE=ck.aL[1];var j2=(h/nK
)|0;var k$=h-(j2*nK);var q1=k$*gA;var q2=j2*hr;lb.clearRect(0,0,gA,hr);lb.drawImage(
ey,q1,q2,gA,hr,0,0,gA,hr);a.bindTexture(a.TEXTURE_2D,d_);a.fm=null;if(ck.aH.Format===
al.b9)a.texSubImage2D(a.TEXTURE_2D,0,aU,aE,a.RGBA,a.UNSIGNED_BYTE,aD);else a.texSubImage2D(
a.TEXTURE_2D,0,aU,aE,a.ALPHA,a.UNSIGNED_BYTE,aD);}}al.pl(m9,0);al._RequestUpdate(
);}catch(bs){console.log("EmWi error: "+bs+"!");}}function oU(al,bH){try{var ey=
bH.Image;var height=ey.height;var bitmap=al.md(al.b9,[ey.width,ey.height],0,1);bH.
cx=bitmap.cx;bH.bT=bitmap.bT;bH.FrameSize=bitmap.FrameSize;bH.eX.FrameSize=bitmap.
FrameSize;l4(al,bH);}catch(bs){console.log("EmWi error: "+bs+"!");}}d.sg=function(
dl,kQ){var lw=this;var bitmap=this.md(dl.Format,dl.FrameSize,dl.FrameDelay,dl.NoOfFrames
);if(!bitmap)return;bitmap.eX=kQ;bitmap.Image=new Image();bitmap.Image.addEventListener(
'load',function(){l4(lw,bitmap);});bitmap.Image.src=dl.FileName;return bitmap;};
d.rK=function(bH){var a=this.ba;if(!bH)return;for(var h=0;bH.cx&&(h<bH.cx.length
);h++){var at=bH.cx[h];if(at.cB)a.deleteTexture(at.cB);if(at.Framebuffer)a.deleteFramebuffer(
at.Framebuffer);}bH.cx=null;bH.bT=null;bH.NoOfFrames=0;};d.sh=function(pO,kQ){var
lw=this;var bitmap={cx:[],bT:[]};var at={};var ck={};at.Format=this.b9;at.Width=
1;at.Height=1;at.hS=1;at.hR=1;at.dW=1;at.dV=1;at.eE=2;at.eD=2;at.cB=null;at.Framebuffer=
null;ck.aH=at;ck.aL=[0,0];bitmap.cx[0]=at;bitmap.bT[0]=ck;bitmap.FrameSize=[1,1];
bitmap.FrameDelay=0;bitmap.NoOfFrames=1;bitmap.eX=kQ;bitmap.Image=new Image();bitmap.
Image.addEventListener('load',function(){oU(lw,bitmap);});bitmap.Image.src=pO;return bitmap;
};d.si=function(dl){var e=dl.Data();var ad={Cache:dl.Cache,Ascent:e[0],Descent:e[
1],Leading:e[2],fk:e[5],ee:e[4],fh:e[6],iZ:e[7],l2:e[8],oN:e[9],gU:null};if(e[3]===
2)ad.kb=on;if(e[3]===4)ad.kb=oo;if(e[3]===16)ad.kb=op;return ad;};d.rL=function(
z){};d.mh=function(z,a1){if(!z){if(this.a4)console.log("EmWi error: Invalid font resource."
);return null;}var w=z.Cache[a1];if(w){ko(this,w);lJ(this,w);return w;}if(!(w=this.
dF(z,a1)))return null;var d6=this.pi;var j3=((w.Height+d6-1)/d6)|0;var jI=((w.Width+
d6-1)/d6)|0;var ak=null;var f4=this.i8;var lc=null;do{if(!(ak=lc=ov(this,jI,j3))
){while(f4&&f4.i4)f4=f4.eV;if(f4){var w=f4;var j2=(w.iQ/d6)|0;var k$=(w.iP/d6)|0;
var j3=((w.Height+d6-1)/d6)|0;var jI=((w.Width+d6-1)/d6)|0;if(this.a4)console.log(
"EmWi info: DiscardGlyph( "+w.dS+", "+w.Width+", "+w.Height+" )");lc=w;f4=f4.eV;
ko(this,w);o2(this,k$,j2,jI,j3);}}}while(!ak&&lc);if(!ak)return null;z.gU=null;var
jS=1/this.i9;w.Cache=z.Cache;w.iP=ak[0]*d6;w.iQ=ak[1]*d6;w.lK=w.iP*jS;w.lM=w.iQ*
jS;w.lL=(w.iP+w.Width)*jS;w.lN=(w.iQ+w.Height)*jS;w.i4=0;if((w.Width>0)&&(w.Height>
0)){var n1=w.Width*w.Height;var lt=new Uint8Array(n1);var a=this.ba;z.kb(z.iZ,lt
,w.iZ,w.l3);pb(lt,n1,w.Width);a.bindTexture(a.TEXTURE_2D,this.hU);a.texSubImage2D(
a.TEXTURE_2D,0,w.iP,w.iQ,w.Width,w.Height,a.ALPHA,a.UNSIGNED_BYTE,lt);a.fm=null;
}o3(this,ak[0],ak[1],jI,j3);lJ(this,w);if(this.a4)console.log("EmWi info: CreateGlyph( "+
w.dS+", "+w.Width+", "+w.Height+" )");return w;};d.eG=function(z,a1){if(!z){if(this.
a4)console.log("EmWi error: Invalid font resource.");return false;}if((a1===0x061C
)||(a1===0xFEFF)||((a1>=0x200B)&&(a1<=0x200F))||((a1>=0x202A)&&(a1<=0x202E))||((
a1>=0x2066)&&(a1<=0x2069))||(a1===0xA))return false;if(z.Cache[a1])return true;return dT(
a1,z.fh,z.fk)>=0;};d.dF=function(z,a1){if(!z){if(this.a4)console.log("EmWi error: Invalid font resource."
);return null;}if((a1===0x061C)||((a1>=0x200B)&&(a1<=0x200F))||((a1>=0x202A)&&(a1<=
0x202E))||((a1>=0x2066)&&(a1<=0x2069))||(a1===0xA))a1=0xFEFF;var w=z.Cache[a1];if(
w)return w;if(!z.gU||(z.gU.dS!==a1)){var gE=a1;var aP=z.fh;var dP=z.fk;var ak=-1;
if(a1!==0xFEFF)while((ak=dT(gE,aP,dP))<0){if(gE===0xAD)gE=0x002D;else if(gE===0xA0
)return this.dF(z,0x20);else if(gE===0x0020)gE=z.ee;else if(gE!==z.ee)return this.
dF(z,z.ee);else break;}if(ak>=0){var rc=aP.charCodeAt(ak+6);var rd=aP.charCodeAt(
ak+7);var ra=aP.charCodeAt(ak+14);var rb=aP.charCodeAt(ak+15);var aX=rd|(rc<<16);
var cI=rb|(ra<<16);z.gU={dS:a1,OriginX:aP.charCodeAt(ak+1)-32768,OriginY:aP.charCodeAt(
ak+2)-32768,Width:aP.charCodeAt(ak+3),Height:aP.charCodeAt(ak+4),Advance:aP.charCodeAt(
ak+5)-32768,iZ:aX,l3:cI-aX};}else z.gU={dS:a1,OriginX:0,OriginY:0,Width:0,Height:
0,Advance:0,iZ:0,l3:0};}return z.gU;};d.bn=function(z,mn,mo){if(!z){if(this.a4)console.
log("EmWi error: Invalid font resource.");return 0;}if(!z.l2||(!mn&&!mo))return 0;
var c=z.l2;var q8=z.oN;var qP=mn|(mo<<16);var q=0;var min=0;var max=(c.length>>1
)-1;var c5=-1;while(max>=min){q=(max+min)>>1;c5=qP-(c.charCodeAt(q*2)|(c.charCodeAt(
q*2+1)<<16));if(!c5)return q8.charCodeAt(q)-128;if(c5<0)max=q-1;else min=q+1;}return 0;
};d.i7=function(z,l,af,ag){var an=0;var aN=0;var bc=l.length;if(!z){if(this.a4)console.
log("EmWi error: Invalid font resource.");return 0;}if(ag===-1)ag=bc;if(af<0){ag+=
af;af=0;}var aP=z.fh;var dP=z.fk;for(;(ag>0)&&(af<bc);af++,ag--){var f=l.charCodeAt(
af);var aR=1;if((f===0x061C)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A
)&&(f<=0x202E))||((f>=0x2066)&&(f<=0x2069))||(f===0xA))continue;if((f===0x2026)&&
!z.Cache[f]&&(dT(f,aP,dP)<0)){f=0x2E;aR=3;}for(;;){var w=z.Cache[f];if(w&&!w.Advance&&
!w.OriginX&&!w.Width)break;if(w){an+=aR*w.Advance+this.bn(z,aN,f)+((aR>1)?this.bn(
z,f,f)*(aR-1):0);aN=f;break;}var ak=dT(f,aP,dP);var fC;if((ak>=0)&&!(fC=(aP.charCodeAt(
ak+5)-32768))&&(aP.charCodeAt(ak+1)===32768)&&(aP.charCodeAt(ak+3)===32768))break;
if(ak>=0){an+=aR*fC+this.bn(z,aN,f)+((aR>1)?this.bn(z,f,f)*(aR-1):0);aN=f;break;
}if(f===z.ee)break;if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=z.ee;}}return an;
};d.r4=function(z,l,af,ag){var eM=[0,0,0,0];var an=0;var aN=0;var bc=l.length;if(
!z){if(this.a4)console.log("EmWi error: Invalid font resource.");return this.pn;
}if(ag===-1)ag=bc;if(af<0){ag+=af;af=0;}var aP=z.fh;var dP=z.fk;for(;(ag>0)&&(af<
bc);af++,ag--){var k,j,p,n,b$;var f=l.charCodeAt(af);var aR=1;if((f===0x061C)||(
f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E))||((f>=0x2066
)&&(f<=0x2069))||(f===0xA))continue;if((f===0x2026)&&!z.Cache[f]&&(dT(f,aP,dP)<0
)){f=0x2E;aR=3;}for(;;){var w=z.Cache[f];if(w){k=w.OriginX;j=w.OriginY;p=w.Width+
k;n=w.Height+j;b$=w.Advance;break;}var ak=dT(f,aP,dP);if(ak>=0){k=aP.charCodeAt(
ak+1)-32768;j=aP.charCodeAt(ak+2)-32768;p=aP.charCodeAt(ak+3)+k;n=aP.charCodeAt(
ak+4)+j;b$=aP.charCodeAt(ak+5)-32768;break;}if(f===z.ee){k=0;j=0;p=0;n=0;b$=0;break;
}if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=z.ee;}if(!k&&!p&&!b$)continue;
if(!an)eM[0]=k;if(j<eM[1])eM[1]=j;if(n>eM[3])eM[3]=n;var lj=this.bn(z,aN,f);var ht=(
aR>1)?this.bn(z,f,f):0;k+=lj;p+=lj+(aR-1)*(b$+ht);b$=b$*aR+lj+(aR-1)*ht;if((b$||
k||p)&&((an+p)>eM[2]))eM[2]=an+p;an+=b$;if(!p&&b$&&(an>eM[2]))eM[2]=an;aN=f;}return eM;
};d.r3=function(z,l,af,ag,aY,dv){var dq=0;var c_=0x8000;var na=0;var bc=l.length;
var aN=0;if(!z){if(this.a4)console.log("EmWi error: Invalid font resource.");return 0;
}if(ag===-1)ag=bc;if(af<0){ag+=af;af=0;}var aP=z.fh;var dP=z.fk;if(aY<0)return-1;
if(dv>0)dv-=this.i7(z,l,af,ag);if(dv>0){var bB=af;var am=ag;var ez=0;for(;am;bB++
,am--){var f=l.charCodeAt(bB);if((f===32)||(f===0xA0))ez++;}if(ez)dq=((dv<<16)/ez
)|0;}for(;(ag>0)&&(af<bc);af++,ag--,na++){var f=l.charCodeAt(af);var aR=1;var b$;
if((f===0x061C)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E
))||((f>=0x2066)&&(f<=0x2069))||(f===0xA))continue;if((f===0x2026)&&!z.Cache[f]&&(
dT(f,aP,dP)<0)){f=0x2E;aR=3;}for(;;){var w=z.Cache[f];if(w&&!w.Advance&&!w.OriginX&&
!w.Width)break;if(w){b$=aR*w.Advance+this.bn(z,aN,f)+((aR>1)?this.bn(z,f,f)*(aR-
1):0);aN=f;break;}var ak=dT(f,aP,dP);var fC;if((ak>=0)&&!(fC=(aP.charCodeAt(ak+5
)===32768))&&(aP.charCodeAt(ak+1)===32768)&&(aP.charCodeAt(ak+3)===32768))break;
if(ak>=0){b$=aR*fC+this.bn(z,aN,f)+((aR>1)?this.bn(z,f,f)*(aR-1):0);aN=f;break;}
if(f===z.ee){b$=0;break;}if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=z.ee;
}if(dq&&((f===0x20)||(f===0xA0))){c_+=dq;b$+=c_>>16;c_&=0xFFFF;}if(aY<((b$>>1)+1
))break;aY-=b$;}return na;};d.rX=function(z,l,af,ag,e0,dv){var dq=0;var c_=0x8000;
var an=0;var bc=l.length;var aN=0;if(!z){if(this.a4)console.log("EmWi error: Invalid font resource."
);return 0;}if(ag===-1)ag=bc;if(af<0){ag+=af;af=0;}var aP=z.fh;var dP=z.fk;if(e0<
0)return-1;if(dv>0)dv-=this.i7(z,l,af,ag);if(dv>0){var bB=af;var am=ag;var ez=0;
for(;am;bB++,am--){var f=l.charCodeAt(bB);if((f===32)||(f===0xA0))ez++;}if(ez)dq=((
dv<<16)/ez)|0;}for(;(ag>0)&&(af<bc)&&e0;af++,ag--,e0--){var f=l.charCodeAt(af);var
aR=1;var b$;if((f===0x061C)||(f===0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A
)&&(f<=0x202E))||((f>=0x2066)&&(f<=0x2069))||(f===0xA))continue;if((f===0x2026)&&
!z.Cache[f]&&(dT(f,aP,dP)<0)){f=0x2E;aR=3;}if(dq&&((f===0x20)||(f===0xA0))){c_+=
dq;an+=c_>>16;c_&=0xFFFF;}for(;;){var w=z.Cache[f];if(w&&!w.Advance&&!w.OriginX&&
!w.Width)break;if(w){b$=aR*w.Advance+this.bn(z,aN,f)+((aR>1)?this.bn(z,f,f)*(aR-
1):0);aN=f;break;}var ak=dT(f,aP,dP);var fC;if((ak>=0)&&!(fC=(aP.charCodeAt(ak+5
)===32768))&&(aP.charCodeAt(ak+1)===32768)&&(aP.charCodeAt(ak+3)===32768))break;
if(ak>=0){b$=aR*fC+this.bn(z,aN,f)+((aR>1)?this.bn(z,f,f)*(aR-1):0);aN=f;break;}
if(f===z.ee){b$=0;break;}if(f===0xAD)f=0x2D;else if(f===0xA0)f=0x20;else f=z.ee;
}an+=b$;}return an;};d.sD=function(z,l,af,aA,mC,ai){var nC=(aA<0)?0x7FFFFFFE:aA;
var nB=(mC<0)?0x7FFFFFFF:mC;var iz=Math.max(af,0);var hG=Math.max(l.length-iz,0);
var ld=hG*2+1;var ab=new Uint16Array(ld+1);var src=ab;var eq=1;var cb=eq+1;var hF=
ld-hG;var bC=hF;var bO=0;var dD=false;var aN=0;var cK=-1;var width=0;var am=0;var
f=0;var q=0;if(!z||!hG||!nB)return"";for(q=0;q<hG;q++)ab[q+bC]=l.charCodeAt(q+iz
);ab[ld]=0;for(q=0;q<hG;q++)switch(src[q+bC]){case 0x25:src[bC+q++]=0xFEFF;break;
case 0x5E:src[bC+q]=0x200B;break;case 0x7E:src[bC+q]=0x00AD;break;}if(ai&&!this.
ma(ai,src,bC,hG,-1))ai=null;if(ai){this.mc(ai,src,bC);this.mb(ai,src,bC);this.l$(
ai,src,bC,null,[z]);}for(q=0;f=src[bC];){if(f!==0xA){var w;var e5=0;var qu=bC;if((
f===0x061C)||((f>=0x200C)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E))||((f>=0x2066
)&&(f<=0x2069)))f=0xFEFF;if(dD&&(f!==0xFEFF)&&(f!==0x200B)&&(f!==0x00AD)&&(f!==0x20
))dD=false;if((f===0x200B)&&width&&!dD){if(bO&&(ab[bO]===0xAD))ab[bO]=0xFEFF;bO=
cb;dD=true;}if((f===0x20)&&width){if(bO&&(ab[bO]===0xAD))ab[bO]=0xFEFF;bO=cb;dD=
true;}if((f===0xAD)&&(!width||dD))f=0xFEFF;if(f===0x00AD){var hE=0;if((w=this.dF(
z,0x00AD))!==null){var aj=ai?this.g2(ai,bC-hF):0;hE=w.Advance;if(aj===cK)if(aj&1
)hE+=this.bn(z,f,aN);else hE+=this.bn(z,aN,f);}if(((width+hE)<=nC)&&hE||!bO){if(
bO&&(ab[bO]===0xAD))ab[bO]=0xFEFF;width-=hE;bO=cb;dD=true;}}if(f===0x200B)f=0xFEFF;
if((f===0x2026)&&!this.eG(z,f)&&(w=this.dF(z,0x2E))){var aj=ai?this.g2(ai,bC-hF):
0;e5=w.Advance*3;e5+=this.bn(z,0x2E,0x2E)*2;if(aj===cK)if(aj&1)e5+=this.bn(z,0x2E
,aN);else e5+=this.bn(z,aN,0x2E);aN=0x2E;cK=aj;}else if((f!==0xFEFF)&&(w=this.dF(
z,f))){var aj=ai?this.g2(ai,bC-hF):0;e5=w.Advance;if(aj===cK)if(aj&1)e5+=this.bn(
z,f,aN);else e5+=this.bn(z,aN,f);aN=f;cK=aj;}bC++;if(((e5+width)<=nC)||!width){width+=
e5;ab[cb++]=f;}else if(bO){if(ab[bO]===0x20)ab[bO]=0xFEFF;for(bC-=2,cb--;cb>bO;bC--
,cb--)src[bC]=ab[cb];bO=0;f=0xA;bC++;cb++;}else{bC=qu;f=0xA;}if(f===0xA){if(src[
bC]===0x20){ab[cb++]=0xFEFF;bC++;}while(((f=src[bC])===0xAD)||(f===0x061C)||(f===
0xFEFF)||((f>=0x200B)&&(f<=0x200F))||((f>=0x202A)&&(f<=0x202E))||((f>=0x2066)&&(
f<=0x2069))){ab[cb++]=0xFEFF;bC++;}f=0xA;}}else{bC++;ab[cb++]=f;}if((f===0xA)&&src[
bC]){if(--nB<=0)break;if(bO&&(ab[bO]===0xAD))ab[bO]=0xFEFF;if(ai){var eu=bC-hF;var
fd=eu-(cb-eq)+1;this.kr(ai,fd,eu);this.hT(ai,fd,eu,ab,eq+1);}ab[eq]=cb-eq;ab[cb]=
0;eq=cb;cb+=1;bO=0;dD=false;width=0;aN=0;cK=-1;am++;}}if(bO&&(ab[bO]===0xAD))ab[
bO]=0xFEFF;if((cb-eq)>1){if(ai){var eu=bC-hF;var fd=eu-(cb-eq)+1;this.kr(ai,fd,eu
);this.hT(ai,fd,eu,ab,eq+1);}ab[eq]=cb-eq;ab[cb]=0;am++;}ab[0]=am;var nW="";for(
q=0;q<cb;q++)nW+=String.fromCharCode(ab[q]);return nW;};d.rR=function(z,kU){var bc=
kU.length;var q=1;var db=0;if(!bc)return 0;if(!z){if(this.a4)console.log("EmWi error: Invalid font resource."
);return 0;}while(q<bc){var iz=kU.charCodeAt(q);var nZ=iz-1;var nY=q+1;var lu;if((
nY+nZ)>bc)return 0;lu=this.i7(z,kU,nY,nZ);if(lu>db)db=lu;q+=iz;}return db;};d.rx=
function(h9,h7,h8){if(h9<0)h9=0;if(h7<0)h7=0;if(h8<0)h8=0;if(h9>256)h9=256;if(h7>
256)h7=256;if(h8>256)h8=256;return{eF:h9,f8:h7,hP:h8,dU:[],fg:[],gS:[]};};d.rI=function(
ah){if(ah){delete ah.dU;delete ah.fg;delete ah.gS;}};d.sP=function(ah,kS,cC){if(
!ah||(kS<0))return;if(kS<ah.hP)ah.gS[kS]=cC;};d.sO=function(ah,kI,bH){if(!ah||(kI<
0))return;if(kI<ah.f8)ah.fg[kI]=bH;};d.sQ=function(ah,kV,z){if(!ah||(kV<0))return;
if(kV<ah.eF)ah.dU[kV]=z;};function eW(l,i,au,eo){var g=0;var bJ=l.charCodeAt(i);
while((bJ>=0x30)&&(bJ<=0x39)){g=(g*10)+(bJ-0x30);bJ=l.charCodeAt(++i);}au[eo]=g;
return i;}function kk(l,i,pT,au,eo){i=eW(l,i,au,eo);if(l.charCodeAt(i)===0x25){var
nO=au[eo]*pT;au[eo]=(nO/100)|0;if((nO-(au[eo]*100))>50)au[eo]++;i++;}return i;}function
l8(l,i,aA,au){au[0]=0;au[1]=0;au[2]=0;au[3]=0;i=kk(l,i,aA,au,0);if(l.charCodeAt(
i)===0x2C){i=kk(l,i+1,aA,au,1);if(l.charCodeAt(i)===0x2C){i=eW(l,i+1,au,2);if(l.
charCodeAt(i)===0x2C)i=eW(l,i+1,au,3);}}if(au[0]>aA)au[0]=aA;if(au[1]>(aA-au[0])
)au[1]=aA-au[0];return i;}function oX(l,i,au,eo){var g=0x6C;var b1=l.charCodeAt(
i);var d5=b1?l.charCodeAt(i+1):0;if(((b1===0x6C)||(b1===0x72)||(b1===0x63)||(b1===
0x61))&&(d5===0x6A)){g=b1-0x20;i+=2;}else if((b1===0x6A)&&((d5===0x6C)||(d5===0x72
)||(d5===0x63)||(d5===0x61))){g=d5-0x20;i+=2;}else if((b1===0x6C)||(b1===0x72)||(
b1===0x63)||(b1===0x61)){g=b1;i++;}else if(b1===0x6A){g=0x4C;i++;}au[eo]=g;return i;
}function l7(l,i,au,eo){var g=0x6D;var bJ=l.charCodeAt(i);if((bJ===0x74)||(bJ===
0x6D)||(bJ===0x62)){g=bJ;i++;}au[eo]=g;return i;}function oY(l,i,aA,gh){var g=[0
,0,0];var bJ=l.charCodeAt(i);var h=0;var b2=0;var jL=aA;var iu=0;var bG=[0];while((
h<3)&&(((bJ>=0x30)&&(bJ<=0x39))||(bJ===0x2A))){if(bJ===0x2A){g[h]=-1;iu++;i++;}else{
i=kk(l,i,aA,g,h);jL=jL-g[h];}if((bJ=l.charCodeAt(i))===0x2C)bJ=l.charCodeAt(++i);
h++;}if(!h){g[0]=aA;h=1;}for(;b2<iu;b2++){var db=(jL/iu)|0;if(b2===(iu-1))db=jL-(
iu-1)*db;if(g[0]<0)g[0]=db;else if(g[1]<0)g[1]=db;else if(g[2]<0)g[2]=db;}for(h=
0;h<3;h++){if(g[h]<0)g[h]=0;if(g[h]>aA)g[h]=aA;aA=aA-g[h];gh[h]=g[h];}return i;}
function cY(l,i,mz){return l.slice(i,i+mz.length)===mz;}function ou(l,i){var f;for(;(
f=l.charCodeAt(i));i++){if((f===0x7B)&&(cY(l,i+1,"par")||cY(l,i+1,"lay")||cY(l,i+
1,"col")||cY(l,i+1,"end")))break;if((f===0x25)&&(i<(l.length-1)))i++;}return i;}
function oZ(l,i,kT,ic,ib,id,du){var td=i;var c9=0;var dx=0;var cr=0;var iN=false;
var hv=false;var dp=[0,0,0];while(i<kT){var f=l.charCodeAt(i);if(f===0x7B){if(cY(
l,i,"{img")){i=l7(l,i+4,dp,0);i=eW(l,i,dp,1);if(l.charCodeAt(i)===0x2E){i++;i=eW(
l,i,dp,2);}if(l.charCodeAt(i)===0x7D)i++;if(dp[2]>4095)dp[2]=0;if(dp[1]>255)dp[1
]=0;ic[c9]=0xFEFF;ib[c9]=((dp[0]===0x74)?5:(dp[0]===0x62)?4:3)|(dx<<4)|(dp[1]<<12
)|(dp[2]<<20);id[c9++]=hv?du[0]:0;continue;}if(cY(l,i,"{fnt")){i=eW(l,i+4,dp,0);
if(l.charCodeAt(i)===0x7D)i++;cr=dp[0];if(cr>255)cr=0;continue;}if(cY(l,i,"{clr"
)){i=eW(l,i+4,dp,0);if(l.charCodeAt(i)===0x7D)i++;dx=dp[0];if(dx>255)dx=0;continue;
}if(cY(l,i,"{ul+}")){iN=true;i+=5;continue;}if(cY(l,i,"{ul-}")){iN=false;i+=5;continue;
}if(cY(l,i,"{lnk:")){i+=5;hv=true;du[0]+=1;while((i<kT)&&((f=l.charCodeAt(i))!==
0x7D)&&(f!==0x7B))i++;if(l.charCodeAt(i)===0x7D)i++;continue;}if(cY(l,i,"{lnk}")
){i+=5;hv=false;continue;}}if(f===0x5E){ic[c9]=0x200B;ib[c9]=(iN?2:1)|(dx<<4)|(cr<<
12);id[c9++]=hv?du[0]:0;i++;continue;}if(f===0x7E){ic[c9]=0xAD;ib[c9]=(iN?2:1)|(
dx<<4)|(cr<<12);id[c9++]=hv?du[0]:0;i++;continue;}if(f===0x25){f=l.charCodeAt(++
i);if(i===kT)break;}ic[c9]=f;ib[c9]=(iN?2:1)|(dx<<4)|(cr<<12);id[c9++]=hv?du[0]:
0;i++;}ic[c9]=0;ib[c9]=0;id[c9]=0;return c9;}function lI(al,ah,l,i,aA,e2,aB,d0,pK
,fw,du){var aG=[0];var cA=aB.length;aB.push(0x47,0,0,aA,0,0,0);i=l6(al,ah,l,i,aA
,0x6C,e2,aB,aG,fw,du);if(aB.length>(cA+7)){aB[cA+4]=aG[0];aB[cA+5]=aB.length-cA;
}else aB.splice(cA,aB.length-cA);while(i<l.length){if(cY(l,i,"{par")){var cF=[0,
0,0,0];var e4=[-1];var mM=[];var width;var height=[0];i=oX(l,i+4,mM,0);i=l8(l,i,
aA,cF);if(l.charCodeAt(i)===0x3A)i=eW(l,i+1,e4,0);if(l.charCodeAt(i)===0x7D)i++;
if(e4[0]!==-1)aB.push(0x52,0,aG[0],aA,0,e4[0]);cA=aB.length;width=aA-cF[0]-cF[1];
aB.push(0x47,0,aG[0]+cF[2],aA,0,0,cF[0]);i=l6(al,ah,l,i,width,mM[0],e2,aB,height
,fw,du);aG[0]+=cF[2]+height[0]+cF[3];aB[cA+4]=aG[0]-cF[3];aB[cA+5]=aB.length-cA;
if(e4[0]!==-1)aB[cA-2]=aG[0];}else if(cY(l,i,"{lay")){var nb=[];var height=[0];cA=
aB.length;i=oY(l,i+4,aA,nb);if(l.charCodeAt(i)===0x7D)i++;aB.push(0x47,0,aG[0],aA
,0,0,0);i=oO(al,ah,l,i,e2,nb,aB,height,fw,du);aG[0]+=height[0];aB[cA+4]=aG[0];aB[
cA+5]=aB.length-cA;}else break;}d0[pK]=aG[0];return i;}function oO(al,ah,l,i,e2,
gh,aB,d0,fw,du){var mN=[];var io=[];var im=[];var e4=[-1,-1,-1];var h=0;var b2=0;
var an=0;var height=0;while((i<l.length)&&(h<gh.length)){if(cY(l,i,"{col")){var cF=[
0,0,0,0];var width;i=l7(l,i+4,mN,h);i=l8(l,i,gh[h],cF);if(l.charCodeAt(i)===0x3A
)i=eW(l,i+1,e4,h);if(l.charCodeAt(i)===0x7D)i++;if(e4[h]!==-1)aB.push(0x52,an,0,
an+gh[h],0,e4[h]);io[h]=aB.length;width=gh[h]-cF[0]-cF[1];aB.push(0x47,an+cF[0],
cF[2],an+cF[0]+width,0,0,0);i=lI(al,ah,l,i,width,e2,aB,im,h,fw,du);im[h]+=cF[2]+
cF[3];height=Math.max(height,im[h]);aB[io[h]+4]=im[h]-cF[3];aB[io[h]+5]=aB.length-
io[h];an+=gh[h];h++;}else break;}for(b2=0;b2<h;b2++){var cA=io[b2];var jZ=height-
im[b2];switch(mN[b2]){case 0x74:jZ=0;break;case 0x6D:jZ>>=1;break;}aB[cA+2]+=jZ;
aB[cA+4]+=jZ;if(e4[b2]!==-1)aB[cA-2]=height;}d0[0]=height;if(cY(l,i,"{end}"))i+=
5;return i;}function o1(al,ah,dY,el,cR,mD,ai,au){var fd=cR;var ad=null;var cr=-1;
var cX=0;var lF=0;var oa=0;var lE=0;var lB=0;var lC=0;var lD=0;var dD=false;var e8=
0;var aN=0;var cK=-1;au[0]=0;au[1]=0;au[2]=0;au[3]=0;au[4]=0;for(;dY[cR];cR++){var
eL=el[cR];var aa=dY[cR];var type=eL&0xF;var cT=0;var ni=0;var nl=0;var ip=0;var iq=
0;var ir=0;if(((type===1)||(type===2))&&(((eL>>12)&0xFF)!==cr)){cr=(eL>>12)&0xFF;
ad=null;if(ah&&(cr>=0)&&(cr<ah.eF))ad=ah.dU[cr];if(!ad){cr=0;ad=(ah&&(ah.eF>0))?
ah.dU[0]:null;}cK=-1;aN=0;}if((aa===0xA)&&ad){au[3]=Math.max(au[3],ad.Ascent);au[
4]=Math.max(au[4],ad.Leading);e8=Math.max(e8,ad.Descent);}if(aa===0xA)break;if((
type>=3)&&(type<=5)){var fF=(eL>>12)&0xFF;var bw=null;if(ah&&(fF>=0)&&(fF<ah.f8)
)bw=ah.fg[fF];if(!bw&&ah&&(ah.f8>0))bw=ah.fg[0];if(bw){cT=bw.FrameSize[0];ni=bw.
FrameSize[1];}cK=-1;aN=0;}else{var w;if((aa===0x200B)&&(!au[0]||dD))aa=0xFEFF;if((
aa===0xAD)&&(!au[0]||dD))aa=0xFEFF;if((aa===0x20)&&!au[0])aa=0xA0;if((aa===0x20)||(
aa===0x200B)){if(cX&&(dY[cX]===0xAD))dY[cX]=0xFEFF;cX=cR;lF=au[0];oa=au[1];lE=au[
2];lB=au[3];lD=au[4];lC=e8;dD=true;}if(aa===0xAD){if(ad&&au[0]&&(w=al.dF(ad,0xAD
))){var aj=ai?al.g2(ai,cR):0;cT=w.Advance;ip=ad.Ascent;iq=ad.Descent;ir=ad.Leading;
if(aj===cK)if(aj&1)cT+=al.bn(ad,aa,aN);else cT+=al.bn(ad,aN,aa);}if(((au[0]+cT)<=
mD)&&cT){if(cX&&(dY[cX]===0xAD))dY[cX]=0xFEFF;cX=cR;lF=au[0]+cT;lB=Math.max(au[3
],ip);lD=Math.max(au[4],ir);lC=Math.max(e8,iq);lE=au[2];dD=true;}continue;}if((aa===
0x061C)||((aa>=0x200B)&&(aa<=0x200F))||((aa>=0x202A)&&(aa<=0x202E))||((aa>=0x2066
)&&(aa<=0x2069)))aa=0xFEFF;if(aa===0xFEFF){dY[cR]=0xFEFF;continue;}if(cX!==cR)dD=
false;if(ad&&(aa===0x2026)&&!ad.Cache[aa]&&(dT(aa,ad.fh,ad.fk)<0)&&(w=al.dF(ad,0x2E
))){var aj=ai?al.g2(ai,cR):0;cT=w.Advance*3;cT+=al.bn(ad,0x2E,0x2E)*2;ip=ad.Ascent;
iq=ad.Descent;ir=ad.Leading;if(aj===cK)if(aj&1)cT+=al.bn(ad,0x2E,aN);else cT+=al.
bn(ad,aN,0x2E);aN=0x2E;cK=aj;}else if(ad&&(w=al.dF(ad,aa))){var aj=ai?al.g2(ai,cR
):0;cT=w.Advance;ip=ad.Ascent;iq=ad.Descent;ir=ad.Leading;if((aa===0x20)||(aa===
0xA0))nl=cT;if(aj===cK)if(aj&1)cT+=al.bn(ad,aa,aN);else cT+=al.bn(ad,aN,aa);aN=aa;
cK=aj;}}if(((cT+au[0])<=mD)||!au[0]){au[0]+=cT;au[1]=Math.max(au[1],ni);au[3]=Math.
max(au[3],ip);au[4]=Math.max(au[4],ir);e8=Math.max(e8,iq);au[2]+=nl;continue;}if(
cX){au[0]=lF;au[1]=oa;au[2]=lE;au[3]=lB;au[4]=lD;e8=lC;cR=cX+1;if(dY[cX]===0x20)
dY[cX]=0xFEFF;while((((type=(el[cR]&0xF))===1)||(type===2))&&(((aa=dY[cR])===0xFEFF
)||(aa===0x200B)||(aa===0xAD)))cR++;cX=0;}break;}if(cX&&(dY[cX]===0xAD))dY[cX]=0xFEFF;
au[1]=Math.max(au[1],au[3]+e8);au[3]+=(au[1]-au[3]-e8)>>1;return cR-fd;}function
l6(al,ah,l,i,aA,pB,e2,aB,d0,fw,du){var eu=ou(l,i);var ad=(ah&&(ah.eF>0))?ah.dU[0
]:null;var bc=eu-i+1;var ep=new Uint16Array(bc);var fE=new Uint32Array(bc);var ll=
new Uint16Array(bc);var cE=null;var gC=false;var q=0;var cr=0;var dx=0;var fe=0;
var fb=0;var nx=[0];var c8=[0,0,0,0,0];bc=oZ(l,i,eu,ep,fE,ll,nx);if(bc&&e2)cE=al.
pd(bc);if(cE&&!al.ma(cE,ep,0,bc,-1)){al.mg(cE);cE=null;}if(cE){al.mc(cE,ep,0);al.
mb(cE,ep,0);al.l$(cE,ep,0,fE,ah.dU);gC=al.pc(cE);if(fw[0]<0)fw[0]=gC?1:0;}d0[0]=
0;while(q<bc){var d8=pB;var an=0;var nz=0;var f1=0;var j4=0;var n3=0;var aN=0;var
am=o1(al,ah,ep,fE,q,aA,cE,c8);if(cE){al.kr(cE,q,q+am);al.hT(cE,q,q+am,ep,q);al.hT(
cE,q,q+am,fE,q);al.hT(cE,q,q+am,ll,q);}if(d8===0x61)d8=gC?0x72:0x6C;if(d8===0x41
)d8=gC?0x52:0x4C;if((d8===0x4C)||(d8===0x52)||(d8===0x43))if((!ep[q+am]||(ep[q+am
]===0xA)||!c8[2])){d8+=0x20;c8[2]=0;}else d8=0x4C;if(d8===0x72)an+=aA-c8[0];else
if(d8===0x63)an+=(aA-c8[0])>>1;for(;am>=0;am--,q++){var aa=am?ep[q]:0;var eL=am?
fE[q]:0;var type=eL&0xF;var lo=dx;var fY=cr;var jX=fe;var jW=am?ll[q]:0;var w;if((
type>=1)&&(type<=5))lo=(eL>>4)&0xFF;if((type===1)||(type===2))fY=(eL>>12)&0xFF;if(
type===1)jX=0;if(type===2)jX=1;if(f1&&(((type!==1)&&(type!==2))||(dx!==lo)||(cr!==
fY)||(fe!==jX)||(fb!==jW))){if(j4&&(d8===0x4C)){var aV=(j4*(aA-c8[0]))+n3;var a9=(
aV/c8[2])|0;n3=aV-(a9*c8[2]);an+=a9;aB[f1]=0x4A;aB.push(an-aB[f1+1]);}if(fe){if(
ad){var w;if(w=al.dF(ad,0x2D))fe=w.Height;}if(fe<=0)fe=1;aB.push(0x52,aB[f1+1],d0[
0]+c8[3]+fe,an,d0[0]+c8[3]+(fe*2),dx);}f1=0;j4=0;}if(cr!==fY){ad=null;if(ah&&(fY>=
0)&&(fY<ah.eF))ad=ah.dU[fY];if(!ad)ad=(ah&&(ah.eF>0))?ah.dU[0]:null;}if(fb&&(fb!==
jW))aB.push(0x41,fb+du[0],nz,d0[0],an,d0[0]+c8[1],c8[3]);if(fb!==jW)nz=an;if(cr!==
fY)aN=0;dx=lo;cr=fY;fe=jX;fb=jW;if((type>=3)&&(type<=5)){var fF=(eL>>12)&0xFF;var
fT=(eL>>20)&0xFFF;var bw=null;if(ah&&(fF>=0)&&(fF<ah.f8))bw=ah.fg[fF];if(!bw&&ah&&(
ah.f8>0))bw=ah.fg[0];if(bw&&(fT>=bw.NoOfFrames))fT=0;if(bw){var bB=c8[1]-bw.FrameSize[
1];switch(type){case 3:bB>>=1;break;case 5:bB=0;break;}aB.push(0x49,an,d0[0]+bB,
fF,fT,dx);an+=bw.FrameSize[0];}aN=0;}if(((type===1)||(type===2))&&ad&&(aa!==0xFEFF
)){var aR=1;if(ad&&(aa===0x2026)&&!ad.Cache[aa]&&(dT(aa,ad.fh,ad.fk)<0)&&(w=al.dF(
ad,0x2E))){aa=0x2E;aR=3;}if(w=al.dF(ad,aa)){var c7=al.bn(ad,aN,aa);if(!f1){f1=aB.
length;aB.push(0x54,an+c7,d0[0]+c8[3],"",cr,dx);}while(aR--){aB[f1+3]+=String.fromCharCode(
aa);an+=w.Advance+c7;if((aa===0x20)||(aa===0xA0))j4+=w.Advance;if(aR)c7=al.bn(ad
,aa,aa);}aN=aa;}}}if(ep[q-1]!==0xA)q--;d0[0]+=c8[1]+c8[4];}if(cE)al.mg(cE);du[0]+=
nx[0];return eu;}function ol(l,i,ji){var f;while((f=l.charCodeAt(i))){if((f===0x7B
)&&cY(l,i,"{lnk:")){var fd=i+=5;while(((f=l.charCodeAt(i)))&&(f!==0x7D)&&(f!==0x7B
))i++;ji.push({iS:0,i3:0xFFFFFFFE,iX:0,kp:0,i5:0,oR:l.slice(fd,i)});continue;}if((
f===0x25)&&((i+1)<l.length))i++;i++;}}function lQ(ji,d1,d2,aT,az,ft){while(az<ft
){switch(aT[az]){case 0x47:{var am=aT[az+5];var an=aT[az+1]+aT[az+6]+d1;var aG=aT[
az+2]+d2;lQ(ji,an,aG,aT,az+7,az+am);az+=am;}break;case 0x54:az+=6;break;case 0x4A:
az+=7;break;case 0x49:az+=6;break;case 0x52:az+=6;break;case 0x41:{var cd=ji[aT[
az+1]-1];cd.i3=Math.min(cd.i3,az);cd.iS=Math.max(cd.iS,az+7);cd.kp=d1;cd.i5=d2;cd.
iX++;az+=7;}break;}}}function l9(co,eJ,aT,pY,mv){var ae=pY;while(ae<mv){switch(aT[
ae]){case 0x54:ae+=6;break;case 0x4A:ae+=7;break;case 0x49:ae+=6;break;case 0x52:
ae+=6;break;case 0x41:{var fb=aT[ae+1];if((fb===co)&&!eJ)return ae;if(fb===co)eJ--;
ae+=7;}break;default:ae=mv;}}return-1;}function oq(al,x,av,ac,cn,d1,d2,ah,aT,az,
ft,ay,b_){function cZ(dI,aY,bk,cC){var hg=dI.ob;var mQ=dI.od;if(aY<0)aY=0;if(bk<
0)bk=0;if(aY>dI.Width)aY=dI.Width;if(bk>dI.Height)bk=dI.Height;if(dI.l1&&bk)hg+=
bk*dI.oc;if(dI.l1&&dI.l0&&bk)mQ+=bk*dI.oe;if(dI.l0&&aY)hg+=(mQ-hg)*dI.oL*aY;if(hg>=
255)return cC;if(hg<=0)return cC&0xFFFFFF;var qr=((((cC>>24)&0xFF)*hg/255)<<24)|(
cC&0xFFFFFF);return qr;}function lU(al,x,av,ac,cn,d1,d2,ah,aT,az,ft,ay,b_){while(
az<ft){var f=aT[az];if((f===0x47)||(f===0x52)){var aw=[aT[az+1]+d1,aT[az+2]+d2,aT[
az+3]+d1,aT[az+4]+d2];switch(f){case 0x47:{var am=aT[az+5];if((ac[0]<ac[2])&&(ac[
1]<ac[3]))lU(al,x,av,ac,cn,aw[0]+aT[az+6],aw[1],ah,aT,az+7,az+am,ay,b_);az+=am;}
break;case 0x52:{var a6=aT[az+5];var k=aw[0]-cn[0];var p=aw[2]-cn[0];var j=aw[1]-
cn[1];var n=aw[3]-cn[1];if((a6>=0)&&(a6<ah.hP))a6=ah.gS[a6];else a6=0;var gw=cZ(
b_,k,j,a6);var gx=cZ(b_,p,j,a6);var gv=cZ(b_,p,n,a6);var gu=cZ(b_,k,n,a6);al.g4(
x,av,ac,aw,gw,gx,gv,gu,ay);az+=6;}break;}}else if((f===0x54)||(f===0x49))az+=6;else
if(f===0x4A)az+=7;else if(f===0x41)az+=7;}}function lV(al,x,av,ac,cn,d1,d2,ah,aT
,az,ft,ay,b_){while(az<ft){var f=aT[az];if((f===0x47)||(f===0x52)){var aw=[aT[az+
1]+d1,aT[az+2]+d2,aT[az+3]+d1,aT[az+4]+d2];switch(f){case 0x47:{var am=aT[az+5];
if((ac[0]<ac[2])&&(ac[1]<ac[3]))lV(al,x,av,ac,cn,aw[0]+aT[az+6],aw[1],ah,aT,az+7
,az+am,ay,b_);az+=am;}break;case 0x52:{az+=6;}break;}}else if((f===0x54)||(f===0x4A
)||(f===0x49)){var ak=[aT[az+1]+d1,aT[az+2]+d2];switch(f){case 0x54:{var ad=aT[az+
4];var a6=aT[az+5];var k=ac[0]-cn[0];var j=ac[1]-cn[1];var p=ac[2]-cn[0];var n=ac[
3]-cn[1];if((a6>=0)&&(a6<ah.hP))a6=ah.gS[a6];else a6=0;if((ad>=0)&&(ad<ah.eF))ad=
ah.dU[ad];else ad=null;if(!ad&&ah.eF)ad=ah.dU[0];ak[0]=ac[0]-ak[0];ak[1]=ac[1]-ak[
1];if(ad){var gw=cZ(b_,k,j,a6);var gx=cZ(b_,p,j,a6);var gv=cZ(b_,p,n,a6);var gu=
cZ(b_,k,n,a6);al.mf(x,ad,aT[az+3],0,-1,av,ac,ac,ak,0,0,gw,gx,gv,gu,ay);}az+=6;}break;
case 0x4A:{var ad=aT[az+4];var a6=aT[az+5];var k=ac[0]-cn[0];var j=ac[1]-cn[1];var
p=ac[2]-cn[0];var n=ac[3]-cn[1];var width=aT[az+6];if((a6>=0)&&(a6<ah.hP))a6=ah.
gS[a6];else a6=0;if((ad>=0)&&(ad<ah.eF))ad=ah.dU[ad];else ad=null;if(!ad&&ah.eF)
ad=ah.dU[0];ak[0]=ac[0]-ak[0];ak[1]=ac[1]-ak[1];if(ad){var gw=cZ(b_,k,j,a6);var gx=
cZ(b_,p,j,a6);var gv=cZ(b_,p,n,a6);var gu=cZ(b_,k,n,a6);al.mf(x,ad,aT[az+3],0,-1
,av,ac,ac,ak,width,0,gw,gx,gv,gu,ay);}az+=7;}break;case 0x49:{var bw=aT[az+3];var
fT=aT[az+4];var a6=aT[az+5];if((a6>=0)&&(a6<ah.hP))a6=ah.gS[a6];else a6=0;if((bw>=
0)&&(bw<ah.f8))bw=ah.fg[bw];else bw=null;if(!bw&&ah.f8)bw=ah.fg[0];if(bw){var k=
ak[0]-cn[0];var j=ak[1]-cn[1];var p=k+bw.FrameSize[0];var n=j+bw.FrameSize[1];var
gw=cZ(b_,k,j,a6);var gx=cZ(b_,p,j,a6);var gv=cZ(b_,p,n,a6);var gu=cZ(b_,k,n,a6);
var cq=[ak[0],ak[1],ak[0]+bw.FrameSize[0],ak[1]+bw.FrameSize[1]];al.fo(x,bw,av,fT
,ac,cq,[0,0],gw,gx,gv,gu,ay);}az+=6;}break;}}else if(f===0x41)az+=7;}}lU(al,x,av
,ac,cn,d1,d2,ah,aT,az,ft,ay,b_);lV(al,x,av,ac,cn,d1,d2,ah,aT,az,ft,ay,b_);}d.sC=
function(ah,l,aA,e2){var mS=[];var fD={};var height=[0];var nL=[0];var gC=[-1];if(
!ah||!l.length||(aA<=0))return null;lI(this,ah,l,0,aA,e2,mS,height,0,gC,nL);fD.Data=
mS;fD.eh=[];fD.oM=gC[0]===1;if(nL){ol(l,0,fD.eh);lQ(fD.eh,0,0,fD.Data,0,fD.Data.
length);}return fD;};d.rJ=function(a0){};d.rQ=function(a0){if(!a0)return this.pm;
if(!a0.eY){var e=a0?a0.Data:null;var ae=0;var cI=e?e.length:0;var cm=[0,0,0,0];while(
ae<cI){var k=e[ae+1];var j=e[ae+2];var p=e[ae+3];var n=e[ae+4];if((k<p)&&(j<n)){
if(cm[0]>k)cm[0]=k;if(cm[2]<p)cm[2]=p;if(cm[1]>j)cm[1]=j;if(cm[3]<n)cm[3]=n;}if(
e[ae]===0x52)ae+=6;else ae+=e[ae+5];}var db=cm[2]-cm[0];var hs=cm[3]-cm[1];if(db<
0)db=0;if(hs<0)hs=0;a0.eY=[db,hs];}return a0.eY;};d.rU=function(a0){if(!a0)return 0;
return a0.eh.length;};d.rO=function(a0,co){if(!a0)return this.g3;if((co<0)||(co>=
a0.eh.length))return this.g3;return a0.eh[co].oR;};d.rT=function(a0,co){if(!a0)return 0;
if((co<0)||(co>=a0.eh.length))return 0;return a0.eh[co].iX;};d.rP=function(a0,co
,eJ){var cd;var ae;var cm=[0,0,0,0];if(!a0)return cm;if((co<0)||(co>=a0.eh.length
))return cm;cd=a0.eh[co];if((eJ<0)||(eJ>=cd.iX))return cm;ae=l9(co+1,eJ,a0.Data,
cd.i3,cd.iS);if(ae>=0){cm[0]=a0.Data[ae+2]+cd.kp;cm[1]=a0.Data[ae+3]+cd.i5;cm[2]=
a0.Data[ae+4]+cd.kp;cm[3]=a0.Data[ae+5]+cd.i5;}return cm;};d.rN=function(a0,co,eJ
){var cd;var ae;if(!a0)return 0;if((co<0)||(co>=a0.eh.length))return 0;cd=a0.eh[
co];if((eJ<0)||(eJ>=cd.iX))return 0;ae=l9(co+1,eJ,a0.Data,cd.i3,cd.iS);if(ae>=0)
return cd.i5+a0.Data[ae+6];return 0;};d.r$=function(a0){return a0&&a0.oM;};var oj=
"\u0000\u0009\u000A\u000B\u000C\u000D\u000E\u001C\u001F\u0020"+"\u0021\u0023\u0026\u0028\u0029\u002A\u002B\u002C\u002D\u002E"+
"\u0030\u003A\u003B\u003C\u003D\u003E\u003F\u0041\u005B\u005C"+"\u005D\u005E\u0061\u007B\u007C\u007D\u007E\u007F\u0085\u0086"+
"\u00A0\u00A1\u00A2\u00A6\u00AA\u00AB\u00AC\u00AD\u00AE\u00B0"+"\u00B2\u00B4\u00B5\u00B6\u00B9\u00BA\u00BB\u00BC\u00C0\u00D7"+
"\u00D8\u00F7\u00F8\u02B9\u02BB\u02C2\u02D0\u02D2\u02E0\u02E5"+"\u02EE\u02EF\u0300\u0370\u0374\u0376\u037E\u037F\u0384\u0386"+
"\u0387\u0388\u03F6\u03F7\u0483\u048A\u058A\u058B\u058D\u058F"+"\u0590\u0591\u05BE\u05BF\u05C0\u05C1\u05C3\u05C4\u05C6\u05C7"+
"\u05C8\u0600\u0606\u0608\u0609\u060B\u060C\u060D\u060E\u0610"+"\u061B\u061C\u061D\u0620\u0621\u0622\u0626\u0627\u0628\u0629"+
"\u062A\u062F\u0633\u0640\u0641\u0648\u0649\u064B\u0660\u066A"+"\u066B\u066D\u066E\u0670\u0671\u0674\u0675\u0678\u0688\u069A"+
"\u06C0\u06C1\u06C3\u06CC\u06CD\u06CE\u06CF\u06D0\u06D2\u06D4"+"\u06D5\u06D6\u06DD\u06DE\u06DF\u06E5\u06E7\u06E9\u06EA\u06EE"+
"\u06F0\u06FA\u06FD\u06FF\u0700\u070F\u0710\u0711\u0712\u0715"+"\u071A\u071E\u071F\u0728\u0729\u072A\u072B\u072C\u072D\u072F"+
"\u0730\u074B\u074D\u074E\u0759\u075C\u076B\u076D\u0771\u0772"+"\u0773\u0775\u0778\u077A\u0780\u07A6\u07B1\u07C0\u07EB\u07F4"+
"\u07F6\u07FA\u0816\u081A\u081B\u0824\u0825\u0828\u0829\u082E"+"\u0859\u085C\u0860\u0861\u0862\u0866\u0867\u0868\u0869\u086B"+
"\u0870\u08A0\u08AA\u08AD\u08AE\u08AF\u08B1\u08B3\u08B5\u08B6"+"\u08B9\u08BA\u08BE\u08D4\u08E2\u08E3\u0903\u093A\u093B\u093C"+
"\u093D\u0941\u0949\u094D\u094E\u0951\u0958\u0962\u0964\u0981"+"\u0982\u09BC\u09BD\u09C1\u09C5\u09CD\u09CE\u09E2\u09E4\u09F2"+
"\u09F4\u09FB\u09FC\u0A01\u0A03\u0A3C\u0A3D\u0A41\u0A43\u0A47"+"\u0A49\u0A4B\u0A4E\u0A51\u0A52\u0A70\u0A72\u0A75\u0A76\u0A81"+
"\u0A83\u0ABC\u0ABD\u0AC1\u0AC6\u0AC7\u0AC9\u0ACD\u0ACE\u0AE2"+"\u0AE4\u0AF1\u0AF2\u0AFA\u0B00\u0B01\u0B02\u0B3C\u0B3D\u0B3F"+
"\u0B40\u0B41\u0B45\u0B4D\u0B4E\u0B56\u0B57\u0B62\u0B64\u0B82"+"\u0B83\u0BC0\u0BC1\u0BCD\u0BCE\u0BF3\u0BF9\u0BFA\u0BFB\u0C00"+
"\u0C01\u0C3E\u0C41\u0C46\u0C49\u0C4A\u0C4E\u0C55\u0C57\u0C62"+"\u0C64\u0C78\u0C7F\u0C81\u0C82\u0CBC\u0CBD\u0CBF\u0CC0\u0CC6"+
"\u0CC7\u0CCC\u0CCE\u0CE2\u0CE4\u0D00\u0D02\u0D3B\u0D3D\u0D41"+"\u0D45\u0D4D\u0D4E\u0D62\u0D64\u0DCA\u0DCB\u0DD2\u0DD5\u0DD6"+
"\u0DD7\u0E31\u0E32\u0E34\u0E3B\u0E3F\u0E40\u0E47\u0E4F\u0EB1"+"\u0EB2\u0EB4\u0EBA\u0EBB\u0EBD\u0EC8\u0ECE\u0F18\u0F1A\u0F35"+
"\u0F36\u0F37\u0F38\u0F39\u0F3A\u0F3B\u0F3C\u0F3D\u0F3E\u0F71"+"\u0F7F\u0F80\u0F85\u0F86\u0F88\u0F8D\u0F98\u0F99\u0FBD\u0FC6"+
"\u0FC7\u102D\u1031\u1032\u1038\u1039\u103B\u103D\u103F\u1058"+"\u105A\u105E\u1061\u1071\u1075\u1082\u1083\u1085\u1087\u108D"+
"\u108E\u109D\u109E\u135D\u1360\u1390\u139A\u1400\u1401\u1680"+"\u1681\u169B\u169C\u169D\u1712\u1715\u1732\u1735\u1752\u1754"+
"\u1772\u1774\u17B4\u17B6\u17B7\u17BE\u17C6\u17C7\u17C9\u17D4"+"\u17DB\u17DC\u17DD\u17DE\u17F0\u17FA\u1800\u180B\u180E\u180F"+
"\u1885\u1887\u18A9\u18AA\u1920\u1923\u1927\u1929\u1932\u1933"+"\u1939\u193C\u1940\u1941\u1944\u1946\u19DE\u1A00\u1A17\u1A19"+
"\u1A1B\u1A1C\u1A56\u1A57\u1A58\u1A5F\u1A60\u1A61\u1A62\u1A63"+"\u1A65\u1A6D\u1A73\u1A7D\u1A7F\u1A80\u1AB0\u1ABF\u1B00\u1B04"+
"\u1B34\u1B35\u1B36\u1B3B\u1B3C\u1B3D\u1B42\u1B43\u1B6B\u1B74"+"\u1B80\u1B82\u1BA2\u1BA6\u1BA8\u1BAA\u1BAB\u1BAE\u1BE6\u1BE7"+
"\u1BE8\u1BEA\u1BED\u1BEE\u1BEF\u1BF2\u1C2C\u1C34\u1C36\u1C38"+"\u1CD0\u1CD3\u1CD4\u1CE1\u1CE2\u1CE9\u1CED\u1CEE\u1CF4\u1CF5"+
"\u1CF8\u1CFA\u1DC0\u1DFA\u1DFB\u1E00\u1FBD\u1FBE\u1FBF\u1FC2"+"\u1FCD\u1FD0\u1FDD\u1FE0\u1FED\u1FF0\u1FFD\u1FFF\u2000\u200B"+
"\u200C\u200D\u200E\u200F\u2010\u2028\u2029\u202A\u202B\u202C"+"\u202D\u202E\u202F\u2030\u2035\u2039\u203B\u2044\u2045\u2046"+
"\u2047\u205F\u2060\u2065\u2066\u2067\u2068\u2069\u206A\u2070"+"\u2071\u2074\u207A\u207C\u207D\u207E\u207F\u2080\u208A\u208C"+
"\u208D\u208E\u208F\u20A0\u20D0\u20F1\u2100\u2102\u2103\u2107"+"\u2108\u210A\u2114\u2115\u2116\u2119\u211E\u2124\u2125\u2126"+
"\u2127\u2128\u2129\u212A\u212E\u212F\u213A\u213C\u2140\u2145"+"\u214A\u214E\u2150\u2160\u2189\u218C\u2190\u2208\u220E\u2212"+
"\u2213\u2214\u2215\u2216\u223C\u223E\u2243\u2244\u2252\u2256"+"\u2264\u226C\u226E\u228C\u228F\u2293\u2298\u2299\u22A2\u22A4"+
"\u22A6\u22A7\u22A8\u22AA\u22AB\u22AC\u22B0\u22B8\u22C9\u22CE"+"\u22D0\u22D2\u22D6\u22EE\u22F0\u22F5\u22F6\u22F8\u22FA\u22FF"+
"\u2308\u2309\u230A\u230B\u230C\u2329\u232A\u232B\u2336\u237B"+"\u2395\u2396\u2427\u2440\u244B\u2460\u2488\u249C\u24EA\u26AC"+
"\u26AD\u2768\u2769\u276A\u276B\u276C\u276D\u276E\u276F\u2770"+"\u2771\u2772\u2773\u2774\u2775\u2776\u27C3\u27C5\u27C6\u27C7"+
"\u27C8\u27CA\u27CB\u27CC\u27CD\u27CE\u27D5\u27D7\u27DD\u27DF"+"\u27E2\u27E6\u27E7\u27E8\u27E9\u27EA\u27EB\u27EC\u27ED\u27EE"+
"\u27EF\u27F0\u2800\u2900\u2983\u2984\u2985\u2986\u2987\u2988"+"\u2989\u298A\u298B\u298C\u298D\u298E\u298F\u2990\u2991\u2992"+
"\u2993\u2994\u2995\u2996\u2997\u2998\u2999\u29B8\u29B9\u29C0"+"\u29C2\u29C4\u29C6\u29CF\u29D3\u29D4\u29D6\u29D8\u29D9\u29DA"+
"\u29DB\u29DC\u29F5\u29F6\u29F8\u29FA\u29FC\u29FD\u29FE\u2A2B"+"\u2A2F\u2A34\u2A36\u2A3C\u2A3E\u2A64\u2A66\u2A79\u2A7B\u2A7D"+
"\u2A85\u2A8B\u2A8D\u2A91\u2A9D\u2AA1\u2AA3\u2AA6\u2AAE\u2AAF"+"\u2AB1\u2AB3\u2AB5\u2ABB\u2AC7\u2ACD\u2AD7\u2ADE\u2ADF\u2AE3"+
"\u2AE6\u2AEC\u2AEE\u2AF7\u2AFB\u2B74\u2B76\u2B96\u2B98\u2BBA"+"\u2BBD\u2BC9\u2BCA\u2BD3\u2BEC\u2BF0\u2CE5\u2CEB\u2CEF\u2CF2"+
"\u2CF9\u2D00\u2D7F\u2D80\u2DE0\u2E00\u2E02\u2E06\u2E09\u2E0B"+"\u2E0C\u2E0E\u2E1C\u2E1E\u2E20\u2E22\u2E23\u2E24\u2E25\u2E26"+
"\u2E27\u2E28\u2E29\u2E2A\u2E4A\u2E80\u2E9A\u2E9B\u2EF4\u2F00"+"\u2FD6\u2FF0\u2FFC\u3000\u3001\u3005\u3008\u3009\u300A\u300B"+
"\u300C\u300D\u300E\u300F\u3010\u3011\u3012\u3014\u3015\u3016"+"\u3017\u3018\u3019\u301A\u301B\u301C\u3021\u302A\u302E\u3030"+
"\u3031\u3036\u3038\u303D\u3040\u3099\u309B\u309D\u30A0\u30A1"+"\u30FB\u30FC\u31C0\u31E4\u321D\u321F\u3250\u3260\u327C\u327F"+
"\u32B1\u32C0\u32CC\u32D0\u3377\u337B\u33DE\u33E0\u33FF\u3400"+"\u4DC0\u4E00\uA490\uA4C7\uA60D\uA610\uA66F\uA673\uA674\uA67E"+
"\uA680\uA69E\uA6A0\uA6F0\uA6F2\uA700\uA722\uA788\uA789\uA802"+"\uA803\uA806\uA807\uA80B\uA80C\uA825\uA827\uA828\uA82C\uA838"+
"\uA83A\uA874\uA878\uA8C4\uA8C6\uA8E0\uA8F2\uA926\uA92E\uA947"+"\uA952\uA980\uA983\uA9B3\uA9B4\uA9B6\uA9BA\uA9BC\uA9BD\uA9E5"+
"\uA9E6\uAA29\uAA2F\uAA31\uAA33\uAA35\uAA37\uAA43\uAA44\uAA4C"+"\uAA4D\uAA7C\uAA7D\uAAB0\uAAB1\uAAB2\uAAB5\uAAB7\uAAB9\uAABE"+
"\uAAC0\uAAC1\uAAC2\uAAEC\uAAEE\uAAF6\uAAF7\uABE5\uABE6\uABE8"+"\uABE9\uABED\uABEE\uFB1D\uFB1E\uFB1F\uFB29\uFB2A\uFB50\uFD3E"+
"\uFD40\uFDD0\uFDF0\uFDFD\uFDFE\uFE00\uFE10\uFE1A\uFE20\uFE30"+"\uFE50\uFE51\uFE52\uFE53\uFE54\uFE55\uFE56\uFE59\uFE5A\uFE5B"+
"\uFE5C\uFE5D\uFE5E\uFE5F\uFE60\uFE62\uFE64\uFE66\uFE67\uFE68"+"\uFE69\uFE6B\uFE6C\uFE70\uFEFF\uFF00\uFF01\uFF03\uFF06\uFF08"+
"\uFF09\uFF0A\uFF0B\uFF0C\uFF0D\uFF0E\uFF10\uFF1A\uFF1B\uFF1C"+"\uFF1D\uFF1E\uFF1F\uFF21\uFF3B\uFF3C\uFF3D\uFF3E\uFF41\uFF5B"+
"\uFF5C\uFF5D\uFF5E\uFF5F\uFF60\uFF61\uFF62\uFF63\uFF64\uFF66"+"\uFFE0\uFFE2\uFFE5\uFFE7\uFFE8\uFFEF\uFFF9\uFFFC\uFFFE\uFFFF";
var ok="\x0A\x0C\x0B\x0C\x0D\x0B\x0A\x0B\x0C\x0D\x0E\x06\x0E"+"\x2E\x4E\x0E\x05\x08\x05\x08\x04\x08\x0E\x6E\x0E\x6E"+
"\x0E\x01\x2E\x0E\x4E\x0E\x01\x2E\x0E\x4E\x0E\x0A\x0B"+"\x0A\x08\x0E\x06\x0E\x01\x6E\x0E\x8A\x0E\x06\x04\x0E"+
"\x01\x0E\x04\x01\x6E\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+"\x0E\x01\x0E\x01\x0E\x01\x0E\x89\x01\x0E\x01\x0E\x01"+
"\x0E\x01\x0E\x01\x0E\x01\x89\x01\x0E\x01\x0E\x06\x02"+"\x89\x02\x89\x02\x89\x02\x89\x02\x89\x02\x07\x0E\x03"+
"\x06\x03\x08\x03\x0E\x89\x03\x83\x03\x1A\x03\x18\x1A"+"\x18\x1A\x18\x1A\x18\x1A\x1B\x1A\x18\x1A\x89\x07\x06"+
"\x07\x03\x1A\x89\x18\x03\x18\x1A\x18\x1A\x18\x1A\x18"+"\x1A\x18\x1A\x18\x1A\x18\x03\x18\x89\x07\x0E\x89\x03"+
"\x89\x0E\x89\x18\x04\x1A\x03\x1A\x03\x83\x18\x89\x1A"+"\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x89\x03"+
"\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x18\x1A\x03"+"\x89\x03\x02\x89\x02\x0E\x02\x89\x02\x89\x02\x89\x02"+
"\x89\x02\x89\x02\x1A\x03\x1A\x03\x18\x1A\x18\x03\x02"+"\x1A\x18\x03\x18\x1A\x18\x1A\x03\x1A\x18\x1A\x03\x89"+
"\x07\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x06"+
"\x01\x06\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x89\x01\x89\x01\x06\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x89\x01\x0E\x06\x0E\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x0E\x01\x89\x01\x89\x01\x81"+
"\x01\x81\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x06\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x2E\x4E\x2E\x4E\x01\x89"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x0E\x01\x0E\x01"+"\x0D\x01\x2E\x4E\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x06\x01\x89\x01\x0E"+"\x01\x0E\x89\x0A\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x0E\x01\x0E\x01\x0E\x01\x89\x01\x89"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+
"\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0D"+"\x8A\x0A\x8A\x81\x82\x0E\x0D\x0B\x8F\x91\x93\x90\x92"+
"\x08\x06\x0E\x6E\x0E\x08\x2E\x4E\x0E\x0D\x8A\x01\x14"+"\x15\x16\x17\x8A\x04\x01\x04\x05\x0E\x2E\x4E\x01\x04"+
"\x05\x0E\x2E\x4E\x01\x06\x89\x01\x0E\x01\x0E\x01\x0E"+"\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+
"\x06\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E"+"\x6E\x0E\x05\x06\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+
"\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E"+"\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+
"\x6E\x0E\x6E\x0E\x2E\x4E\x2E\x4E\x0E\x2E\x4E\x0E\x01"+"\x0E\x01\x0E\x01\x0E\x01\x0E\x04\x01\x0E\x01\x0E\x2E"+
"\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+"\x0E\x6E\x2E\x4E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+
"\x6E\x0E\x6E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+"\x0E\x01\x0E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+
"\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x0E"+"\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x2E\x4E\x2E"+
"\x4E\x0E\x6E\x0E\x6E\x0E\x2E\x4E\x0E\x6E\x0E\x6E\x0E"+"\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E"+
"\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E"+"\x6E\x0E\x6E\x0E\x6E\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+
"\x0E\x01\x0E\x01\x0E\x01\x89\x01\x0E\x01\x89\x01\x89"+"\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x0E\x6E\x2E\x4E\x2E"+
"\x4E\x2E\x4E\x2E\x4E\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+"\x0E\x01\x0D\x0E\x01\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E"+
"\x2E\x4E\x0E\x2E\x4E\x2E\x4E\x2E\x4E\x2E\x4E\x0E\x01"+"\x89\x01\x0E\x01\x0E\x01\x0E\x01\x89\x0E\x01\x0E\x01"+
"\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E"+"\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01\x0E\x01"+
"\x89\x0E\x89\x0E\x01\x89\x01\x89\x01\x0E\x01\x0E\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x0E\x01\x06\x01\x0E"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89"+
"\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01"+"\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x89\x01\x02"+
"\x89\x02\x05\x02\x03\x0E\x03\x01\x03\x0E\x03\x89\x0E"+"\x01\x89\x0E\x08\x0E\x08\x01\x0E\x08\x0E\x2E\x4E\x2E"+
"\x4E\x2E\x4E\x06\x0E\x05\x6E\x0E\x01\x0E\x06\x0E\x01"+"\x03\x8A\x01\x0E\x06\x0E\x2E\x4E\x0E\x05\x08\x05\x08"+
"\x04\x08\x0E\x6E\x0E\x6E\x0E\x01\x2E\x0E\x4E\x0E\x01"+"\x2E\x0E\x4E\x0E\x2E\x4E\x0E\x2E\x4E\x0E\x01\x06\x0E"+
"\x06\x01\x0E\x01\x8E\x0E\x01\x01";var oh="\u0028\u0029\u0029\u0028\u003C\u003E\u003E\u003C\u005B\u005D"+
"\u005D\u005B\u007B\u007D\u007D\u007B\u00AB\u00BB\u00BB\u00AB"+"\u0F3A\u0F3B\u0F3B\u0F3A\u0F3C\u0F3D\u0F3D\u0F3C\u169B\u169C"+
"\u169C\u169B\u2039\u203A\u203A\u2039\u2045\u2046\u2046\u2045"+"\u207D\u207E\u207E\u207D\u208D\u208E\u208E\u208D\u2208\u220B"+
"\u2209\u220C\u220A\u220D\u220B\u2208\u220C\u2209\u220D\u220A"+"\u2215\u29F5\u223C\u223D\u223D\u223C\u2243\u22CD\u2252\u2253"+
"\u2253\u2252\u2254\u2255\u2255\u2254\u2264\u2265\u2265\u2264"+"\u2266\u2267\u2267\u2266\u2268\u2269\u2269\u2268\u226A\u226B"+
"\u226B\u226A\u226E\u226F\u226F\u226E\u2270\u2271\u2271\u2270"+"\u2272\u2273\u2273\u2272\u2274\u2275\u2275\u2274\u2276\u2277"+
"\u2277\u2276\u2278\u2279\u2279\u2278\u227A\u227B\u227B\u227A"+"\u227C\u227D\u227D\u227C\u227E\u227F\u227F\u227E\u2280\u2281"+
"\u2281\u2280\u2282\u2283\u2283\u2282\u2284\u2285\u2285\u2284"+"\u2286\u2287\u2287\u2286\u2288\u2289\u2289\u2288\u228A\u228B"+
"\u228B\u228A\u228F\u2290\u2290\u228F\u2291\u2292\u2292\u2291"+"\u2298\u29B8\u22A2\u22A3\u22A3\u22A2\u22A6\u2ADE\u22A8\u2AE4"+
"\u22A9\u2AE3\u22AB\u2AE5\u22B0\u22B1\u22B1\u22B0\u22B2\u22B3"+"\u22B3\u22B2\u22B4\u22B5\u22B5\u22B4\u22B6\u22B7\u22B7\u22B6"+
"\u22C9\u22CA\u22CA\u22C9\u22CB\u22CC\u22CC\u22CB\u22CD\u2243"+"\u22D0\u22D1\u22D1\u22D0\u22D6\u22D7\u22D7\u22D6\u22D8\u22D9"+
"\u22D9\u22D8\u22DA\u22DB\u22DB\u22DA\u22DC\u22DD\u22DD\u22DC"+"\u22DE\u22DF\u22DF\u22DE\u22E0\u22E1\u22E1\u22E0\u22E2\u22E3"+
"\u22E3\u22E2\u22E4\u22E5\u22E5\u22E4\u22E6\u22E7\u22E7\u22E6"+"\u22E8\u22E9\u22E9\u22E8\u22EA\u22EB\u22EB\u22EA\u22EC\u22ED"+
"\u22ED\u22EC\u22F0\u22F1\u22F1\u22F0\u22F2\u22FA\u22F3\u22FB"+"\u22F4\u22FC\u22F6\u22FD\u22F7\u22FE\u22FA\u22F2\u22FB\u22F3"+
"\u22FC\u22F4\u22FD\u22F6\u22FE\u22F7\u2308\u2309\u2309\u2308"+"\u230A\u230B\u230B\u230A\u2329\u232A\u232A\u2329\u2768\u2769"+
"\u2769\u2768\u276A\u276B\u276B\u276A\u276C\u276D\u276D\u276C"+"\u276E\u276F\u276F\u276E\u2770\u2771\u2771\u2770\u2772\u2773"+
"\u2773\u2772\u2774\u2775\u2775\u2774\u27C3\u27C4\u27C4\u27C3"+"\u27C5\u27C6\u27C6\u27C5\u27C8\u27C9\u27C9\u27C8\u27CB\u27CD"+
"\u27CD\u27CB\u27D5\u27D6\u27D6\u27D5\u27DD\u27DE\u27DE\u27DD"+"\u27E2\u27E3\u27E3\u27E2\u27E4\u27E5\u27E5\u27E4\u27E6\u27E7"+
"\u27E7\u27E6\u27E8\u27E9\u27E9\u27E8\u27EA\u27EB\u27EB\u27EA"+"\u27EC\u27ED\u27ED\u27EC\u27EE\u27EF\u27EF\u27EE\u2983\u2984"+
"\u2984\u2983\u2985\u2986\u2986\u2985\u2987\u2988\u2988\u2987"+"\u2989\u298A\u298A\u2989\u298B\u298C\u298C\u298B\u298D\u2990"+
"\u298E\u298F\u298F\u298E\u2990\u298D\u2991\u2992\u2992\u2991"+"\u2993\u2994\u2994\u2993\u2995\u2996\u2996\u2995\u2997\u2998"+
"\u2998\u2997\u29B8\u2298\u29C0\u29C1\u29C1\u29C0\u29C4\u29C5"+"\u29C5\u29C4\u29CF\u29D0\u29D0\u29CF\u29D1\u29D2\u29D2\u29D1"+
"\u29D4\u29D5\u29D5\u29D4\u29D8\u29D9\u29D9\u29D8\u29DA\u29DB"+"\u29DB\u29DA\u29F5\u2215\u29F8\u29F9\u29F9\u29F8\u29FC\u29FD"+
"\u29FD\u29FC\u2A2B\u2A2C\u2A2C\u2A2B\u2A2D\u2A2E\u2A2E\u2A2D"+"\u2A34\u2A35\u2A35\u2A34\u2A3C\u2A3D\u2A3D\u2A3C\u2A64\u2A65"+
"\u2A65\u2A64\u2A79\u2A7A\u2A7A\u2A79\u2A7D\u2A7E\u2A7E\u2A7D"+"\u2A7F\u2A80\u2A80\u2A7F\u2A81\u2A82\u2A82\u2A81\u2A83\u2A84"+
"\u2A84\u2A83\u2A8B\u2A8C\u2A8C\u2A8B\u2A91\u2A92\u2A92\u2A91"+"\u2A93\u2A94\u2A94\u2A93\u2A95\u2A96\u2A96\u2A95\u2A97\u2A98"+
"\u2A98\u2A97\u2A99\u2A9A\u2A9A\u2A99\u2A9B\u2A9C\u2A9C\u2A9B"+"\u2AA1\u2AA2\u2AA2\u2AA1\u2AA6\u2AA7\u2AA7\u2AA6\u2AA8\u2AA9"+
"\u2AA9\u2AA8\u2AAA\u2AAB\u2AAB\u2AAA\u2AAC\u2AAD\u2AAD\u2AAC"+"\u2AAF\u2AB0\u2AB0\u2AAF\u2AB3\u2AB4\u2AB4\u2AB3\u2ABB\u2ABC"+
"\u2ABC\u2ABB\u2ABD\u2ABE\u2ABE\u2ABD\u2ABF\u2AC0\u2AC0\u2ABF"+"\u2AC1\u2AC2\u2AC2\u2AC1\u2AC3\u2AC4\u2AC4\u2AC3\u2AC5\u2AC6"+
"\u2AC6\u2AC5\u2ACD\u2ACE\u2ACE\u2ACD\u2ACF\u2AD0\u2AD0\u2ACF"+"\u2AD1\u2AD2\u2AD2\u2AD1\u2AD3\u2AD4\u2AD4\u2AD3\u2AD5\u2AD6"+
"\u2AD6\u2AD5\u2ADE\u22A6\u2AE3\u22A9\u2AE4\u22A8\u2AE5\u22AB"+"\u2AEC\u2AED\u2AED\u2AEC\u2AF7\u2AF8\u2AF8\u2AF7\u2AF9\u2AFA"+
"\u2AFA\u2AF9\u2E02\u2E03\u2E03\u2E02\u2E04\u2E05\u2E05\u2E04"+"\u2E09\u2E0A\u2E0A\u2E09\u2E0C\u2E0D\u2E0D\u2E0C\u2E1C\u2E1D"+
"\u2E1D\u2E1C\u2E20\u2E21\u2E21\u2E20\u2E22\u2E23\u2E23\u2E22"+"\u2E24\u2E25\u2E25\u2E24\u2E26\u2E27\u2E27\u2E26\u2E28\u2E29"+
"\u2E29\u2E28\u3008\u3009\u3009\u3008\u300A\u300B\u300B\u300A"+"\u300C\u300D\u300D\u300C\u300E\u300F\u300F\u300E\u3010\u3011"+
"\u3011\u3010\u3014\u3015\u3015\u3014\u3016\u3017\u3017\u3016"+"\u3018\u3019\u3019\u3018\u301A\u301B\u301B\u301A\uFE59\uFE5A"+
"\uFE5A\uFE59\uFE5B\uFE5C\uFE5C\uFE5B\uFE5D\uFE5E\uFE5E\uFE5D"+"\uFE64\uFE65\uFE65\uFE64\uFF08\uFF09\uFF09\uFF08\uFF1C\uFF1E"+
"\uFF1E\uFF1C\uFF3B\uFF3D\uFF3D\uFF3B\uFF5B\uFF5D\uFF5D\uFF5B"+"\uFF5F\uFF60\uFF60\uFF5F\uFF62\uFF63\uFF63\uFF62";
var oi=[0x0621,"\u0000\u0000\u0000",0x0622,"\u0000\u0000\uFE82",0x0623,"\u0000\u0000\uFE84"
,0x0624,"\u0000\u0000\uFE86",0x0625,"\u0000\u0000\uFE88",0x0626,"\uFE8B\uFE8C\uFE8A"
,0x0627,"\u0000\u0000\uFE8E",0x0628,"\uFE91\uFE92\uFE90",0x0629,"\u0000\u0000\uFE94"
,0x062A,"\uFE97\uFE98\uFE96",0x062B,"\uFE9B\uFE9C\uFE9A",0x062C,"\uFE9F\uFEA0\uFE9E"
,0x062D,"\uFEA3\uFEA4\uFEA2",0x062E,"\uFEA7\uFEA8\uFEA6",0x062F,"\u0000\u0000\uFEAA"
,0x0630,"\u0000\u0000\uFEAC",0x0631,"\u0000\u0000\uFEAE",0x0632,"\u0000\u0000\uFEB0"
,0x0633,"\uFEB3\uFEB4\uFEB2",0x0634,"\uFEB7\uFEB8\uFEB6",0x0635,"\uFEBB\uFEBC\uFEBA"
,0x0636,"\uFEBF\uFEC0\uFEBE",0x0637,"\uFEC3\uFEC4\uFEC2",0x0638,"\uFEC7\uFEC8\uFEC6"
,0x0639,"\uFECB\uFECC\uFECA",0x063A,"\uFECF\uFED0\uFECE",0x0641,"\uFED3\uFED4\uFED2"
,0x0642,"\uFED7\uFED8\uFED6",0x0643,"\uFEDB\uFEDC\uFEDA",0x0644,"\uFEDF\uFEE0\uFEDE"
,0x0645,"\uFEE3\uFEE4\uFEE2",0x0646,"\uFEE7\uFEE8\uFEE6",0x0647,"\uFEEB\uFEEC\uFEEA"
,0x0648,"\u0000\u0000\uFEEE",0x0649,"\uFBE8\uFBE9\uFEF0",0x064A,"\uFEF3\uFEF4\uFEF2"
,0x0671,"\u0000\u0000\uFB51",0x0677,"\u0000\u0000\u0000",0x0679,"\uFB68\uFB69\uFB67"
,0x067A,"\uFB60\uFB61\uFB5F",0x067B,"\uFB54\uFB55\uFB53",0x067E,"\uFB58\uFB59\uFB57"
,0x067F,"\uFB64\uFB65\uFB63",0x0680,"\uFB5C\uFB5D\uFB5B",0x0683,"\uFB78\uFB79\uFB77"
,0x0684,"\uFB74\uFB75\uFB73",0x0686,"\uFB7C\uFB7D\uFB7B",0x0687,"\uFB80\uFB81\uFB7F"
,0x0688,"\u0000\u0000\uFB89",0x068C,"\u0000\u0000\uFB85",0x068D,"\u0000\u0000\uFB83"
,0x068E,"\u0000\u0000\uFB87",0x0691,"\u0000\u0000\uFB8D",0x0698,"\u0000\u0000\uFB8B"
,0x06A4,"\uFB6C\uFB6D\uFB6B",0x06A6,"\uFB70\uFB71\uFB6F",0x06A9,"\uFB90\uFB91\uFB8F"
,0x06AD,"\uFBD5\uFBD6\uFBD4",0x06AF,"\uFB94\uFB95\uFB93",0x06B1,"\uFB9C\uFB9D\uFB9B"
,0x06B3,"\uFB98\uFB99\uFB97",0x06BA,"\u0000\u0000\uFB9F",0x06BB,"\uFBA2\uFBA3\uFBA1"
,0x06BE,"\uFBAC\uFBAD\uFBAB",0x06C0,"\u0000\u0000\uFBA5",0x06C1,"\uFBA8\uFBA9\uFBA7"
,0x06C5,"\u0000\u0000\uFBE1",0x06C6,"\u0000\u0000\uFBDA",0x06C7,"\u0000\u0000\uFBD8"
,0x06C8,"\u0000\u0000\uFBDC",0x06C9,"\u0000\u0000\uFBE3",0x06CB,"\u0000\u0000\uFBDF"
,0x06CC,"\uFBFE\uFBFF\uFBFD",0x06D0,"\uFBE6\uFBE7\uFBE5",0x06D2,"\u0000\u0000\uFBAF"
,0x06D3,"\u0000\u0000\uFBB1",0xFEF5,"\u0000\u0000\uFEF6",0xFEF7,"\u0000\u0000\uFEF8"
,0xFEF9,"\u0000\u0000\uFEFA",0xFEFB,"\u0000\u0000\uFEFC"];function oK(a1){var q=
0;var min=0;var e=oj;var max=e.length-1;while(max>=min){q=(max+min)>>1;if(a1===e.
charCodeAt(q))break;else if(a1<e.charCodeAt(q))max=q-1;else if((a1>=e.charCodeAt(
q+1)))min=q+1;else break;}return ok.charCodeAt(q);}function lY(a1){var q=0;var min=
0;var e=oh;var max=(e.length>>1)-1;var c5=-1;while(max>=min){q=(max+min)>>1;c5=a1-
e.charCodeAt(q*2);if(!c5)return e.charCodeAt(q*2+1);if(c5<0)max=q-1;else min=q+1;
}return 0;}function kg(a1){var q=0;var min=0;var e=oi;var max=(e.length>>1)-1;var
c5=-1;while(max>=min){q=(max+min)>>1;c5=a1-e[q*2];if(!c5)return e[q*2+1];if(c5<0
)max=q-1;else min=q+1;}return"";}function ow(be,bi,k2){var fa=0;var aM=be[bi*4];
if(aM!==23)return 0;for(;bi>=k2;bi--){aM=be[bi*4];if(aM===23)fa--;if(fa&&((aM===
20)||(aM===21)||(aM===22)))if(++fa===0)return bi;}return 0;}function ke(be,bi,eK
){var fa=0;var aM=be[bi*4];if((aM!==20)&&(aM!==21)&&(aM!==22))return 0;for(;bi<eK;
bi++){aM=be[bi*4];if((aM===20)||(aM===21)||(aM===22))fa++;else if(fa&&(aM===23)){
if(fa===1)return bi;if(fa>1)fa--;}}return 0;}function lX(be,bi,eK){for(;bi<eK;bi++
){var aM=be[bi*4];if((aM===1)||(aM===2)||(aM===3))return bi;if((aM!==20)&&(aM!==
21)&&(aM!==22))continue;if(!(bi=ke(be,bi,eK)))bi=eK;}return 0;}function kd(be,bi
,eK){var aj=be[bi*4+1];if(bi>=eK)return 0;while((bi<eK)&&(be[bi*4+1]===aj))bi++;
return bi-1;}function o0(ih,dK,jk,be,l,i,pH,pI,my){var e6=[];var jK=[];var bF=-1;
var am=0;var bB=0;for(;dK<jk;dK++){var aF=ih[dK];while((my>0)&&((aF+bB)>=pH[pI-bB
])){bB++;my--;}if(be[aF*4]!==14)continue;var a8=be[aF*4+2];if(a8===1){if(bF>=(63-
1))break;jK.push(aF);e6.push(lY(l[i+aF-3+bB]));bF++;}else if(a8===2){var aa=l[i+
aF-3+bB];var eS=bF;while((eS>0)&&(aa!==e6[eS])&&((aa!==0x232A)||(e6[eS]!==0x3009
))&&((aa!==0x3009)||(e6[eS]!==0x232A)))eS--;if((eS>=0)&&((aa===e6[eS])||((aa===0x232A
)&&(e6[eS]===0x3009))||((aa===0x3009)&&(e6[eS]===0x232A)))){while(bF>eS){be[jK.pop(
)*4+2]=3;e6.pop();bF--;}e6.pop();jK.pop();bF--;am++;}else be[aF*4+2]=3;}}while(bF>=
0){be[jK.pop()*4+2]=3;bF--;}for(;dK<jk;dK++){var aF=ih[dK]*4;if((be[aF]===14)&&((
be[aF+2]===1)||(be[aF+2]===2)))be[aF+2]=3;}return am;}function ot(ih,dK,jk,be){var
aF=ih[dK]*4;var am=1;if((be[aF]!==14)||(be[aF+2]!==1))return 0;for(dK++;dK<jk;dK++
){var aF=ih[dK]*4;var type=be[aF];var a8=be[aF+2];if((type===14)&&(a8===1))am++;
else if((type===14)&&(a8===2))if(!--am)return dK;}return 0;}function oJ(be,bi,k2
,k1){var aj=be[bi*4+1];var cK=k1;if(bi>k2)cK=be[bi*4-3];if(cK>aj)aj=cK;if(aj&1)return 2;
else return 1;}function oI(be,bi,eK,k1){var aj=be[bi*4+1];var lq=k1;var type=be[
bi*4];if((bi<(eK-1))&&(type!==21)&&(type!==20)&&(type!==22))lq=be[bi*4+5];if(lq>
aj)aj=lq;if(aj&1)return 2;else return 1;}d.pd=function(kY){if(kY<=0)return null;
return{Data:new Uint8Array((kY+4)*4),oQ:kY,eY:0,lZ:false,gX:0};};d.mg=function(ai
){};d.rq=function(ai){return ai&&ai.lZ;};d.pc=function(ai){return ai&&(ai.gX===1
);};d.g2=function(ai,a$){if(!ai)return 0;if((a$<0)||(a$>=ai.eY))return ai.gX;return ai.
Data[a$*4+13];};d.ma=function(ai,l,i,ag,kH){var e=ai?ai.Data:null;var qU=ai?ai.oQ:
0;var length=(ag<0)?(l.length-i-1):ag;var ew=kH>0;var q=0;var fN=12;if(qU<length
)return false;for(;q<length;q++){var lh=oK(l[i+q]);var type=lh&31;var k6=lh&96;var
qO=lh&128;var a8=0;if(type===24)a8=8;else if(type===25)a8=9;else if(type===26)a8=
10;else if(type===27)a8=11;else if(k6===32)a8=1;else if(k6===64)a8=2;else if(k6===
96)a8=3;else if(type===9)a8=4;else if(type===13)a8=5;else if(type===12)a8=6;else
if(type===11)a8=6;else if((type===21)||(type===20)||(type===22)||(type===23))a8=
7;else if(qO)a8=12;if((type>=24)&&(type<=27))type=3;if(!ew&&((type===3)||(type===
2)||(type===17)||(type===18)||(type===21)))ew=1;e[fN++]=type;e[fN++]=0;e[fN++]=a8;
e[fN++]=0;}e[fN++]=0;e[fN++]=0;e[fN++]=0;e[fN++]=0;ai.eY=length;ai.lZ=ew;ai.gX=(
kH>0)?1:0;if(!ew)return false;if((kH<0)&&(q=lX(e,3,length+3)))ai.gX=(e[q*4]===1)?
0:1;return true;};d.mc=function(ai,l,i){var bW=[];var length=ai?ai.eY:0;var gJ=ai?
ai.gX:0;var e=ai?ai.Data:null;var cL=3+length;var aF=new Uint32Array(length+2);var
bF=0;var gI=0;var hz=0;var lz=0;var gB=0;var ae;var bu;bW.push({gV:gJ,fl:0,iV:false
});for(ae=3;ae<cL;ae++){var aM=e[ae*4];if(aM===22){var nU=ke(e,ae,cL);var g=lX(e
,ae+1,nU?nU:cL);if(g&&((e[g*4]===3)||(e[g*4]===2)))aM=21;else aM=20;}if((aM===17
)||(aM===15)||(aM===18)||(aM===16)){var aj=bW[bF].gV;var type=0;if((aM===17)||(aM===
18))aj+=(aj&1)?2:1;else aj+=(aj&1)?1:2;if(aM===18)type=2;if(aM===16)type=1;if((aj<=
125)&&!gI&&!hz){bF++;bW.push({gV:aj,fl:type,iV:false});}if((aj>125)&&!gI)hz++;}else
if((aM===21)||(aM===20)){var aj=bW[bF].gV;e[ae*4+1]=aj;if(bW[bF].fl)e[ae*4]=bW[bF
].fl;if(aM===21)aj+=(aj&1)?2:1;else aj+=(aj&1)?1:2;if((aj<=125)&&!gI&&!hz){lz++;
bF++;bW.push({gV:aj,fl:0,iV:true});}else gI++;}else if(aM===23){if(gI)gI--;else if(
lz){hz=0;while(!bW[bF].iV){bW.pop();bF--;}bW.pop();bF--;lz--;}e[ae*4+1]=bW[bF].gV;
if(bW[bF].fl)e[ae*4]=bW[bF].fl;}else if(aM===19){if(!gI){if(hz)hz--;else if((bF&&
!bW[bF].iV)){bW.pop();bF--;}}}else if((aM!==11)&&(aM!==10)){e[ae*4+1]=bW[bF].gV;
if(bW[bF].fl)e[ae*4]=bW[bF].fl;}}for(bu=3,ae=3;ae<cL;ae++,bu++){var aM=e[ae*4];if((
aM===17)||(aM===15)||(aM===18)||(aM===16)||(aM===19)||(aM===10)){aF[length+2-++gB
]=ae;bu--;}else if(bu<ae){e[bu*4]=e[ae*4];e[bu*4+1]=e[ae*4+1];e[bu*4+2]=e[ae*4+2
];}}cL-=gB;for(ae=3;ae<cL;){var aO=0;var eR;var eN;var iE;var hq;var dr;e[4]=0;e[
5]=0;e[6]=0;e[8]=0;e[9]=0;e[10]=0;if((e[ae*4]===23)&&ow(e,ae,3)){ae=kd(e,ae,cL)+
1;continue;}e[4]=oJ(e,ae,3,gJ);aF[aO++]=1;hq=kd(e,ae,cL);for(;ae<=hq;ae++)aF[aO++
]=ae;while(iE=ke(e,hq,cL)){hq=kd(e,iE,cL);for(;iE<=hq;iE++)aF[aO++]=iE;}e[8]=oI(
e,hq,cL,gJ);aF[aO++]=2;eR=aO;for(dr=e[4],aO=1;aO<eR;aO++){var bl=aF[aO];if(e[bl*
4]===9){var hA=e[aF[aO-1]*4];if((hA===21)||(hA===20)||(hA===22)||(hA===23))hA=14;
e[bl*4]=hA;}}for(dr=e[4],aO=1;aO<eR;aO++){var bl=aF[aO];var jJ=aF[aO-1];var type=
e[bl*4];if((type===1)||(type===2)||(type===3))dr=type;if((type===4)&&(dr===3))type=
e[bl*4]=7;if(type===3)type=e[bl*4]=2;if((type===4)&&(e[jJ*4]===5)&&(e[aF[aO-2]*4
]===type))e[jJ*4]=type;if(((type===7)||(type===4))&&(e[jJ*4]===8)&&(e[aF[aO-2]*4
]===type))e[jJ*4]=type;}for(aO=1;aO<eR;aO++){var bl=aF[aO];var type=e[bl*4];if(type===
4){var dC=aO-1;while(e[aF[dC]*4]===6)e[aF[dC--]*4]=4;}if((type===6)&&(e[aF[aO-1]
*4]===4))e[bl*4]=4;}for(aO=1;aO<eR;aO++){var bl=aF[aO];var type=e[bl*4];if((type===
6)||(type===5)||(type===8))e[bl*4]=14;}for(dr=e[4],aO=1;aO<eR;aO++){var bl=aF[aO
];var type=e[bl*4];if((type===4)&&(dr===1))e[bl*4]=1;if((type===1)||(type===2)||(
type===3))dr=type;}if(o0(aF,aO=1,eR-1,e,l,i,aF,length+1,gB)){var et=(e[aF[aO]*4+
1]&1)?2:1;var dr=e[8];for(;aO<eR;aO++){var hi=ot(aF,aO,eR,e);var dC=aO+1;var type=
e[aF[aO]*4];var iJ=0;var gL=0;var hx=0;if((type===4)||(type===7))type=2;if((type===
2)||(type===1))dr=type;if(!hi)continue;for(;(dC<hi)&&(!iJ||!gL);dC++){var j7=e[aF[
dC]*4];if(j7===1)iJ++;if(j7===2)gL++;if(j7===7)gL++;if(j7===4)gL++;}if(((et===1)&&
iJ)||((et===2)&&gL)){e[aF[aO]*4]=et;e[aF[hi]*4]=et;hx=et;dr=et;}else if((iJ||gL)&&(
dr!==et)){e[aF[aO]*4]=dr;e[aF[hi]*4]=dr;hx=dr;}else if(iJ||gL){e[aF[aO]*4]=et;e[
aF[hi]*4]=et;hx=et;dr=et;}if(hx){for(dC=aO+1;e[aF[dC]*4+2]===4;dC++)e[aF[dC]*4]=
hx;for(dC=hi+1;e[aF[dC]*4+2]===4;dC++)e[aF[dC]*4]=hx;}}}for(eN=0,aO=1;aO<eR;aO++
){var bl=aF[aO];var type=e[bl*4];if(!eN&&((type===11)||(type===12)||(type===13)||(
type===14)||(type===22)||(type===20)||(type===21)||(type===23)))eN=aO;if(eN&&(type
!==11)&&(type!==12)&&(type!==13)&&(type!==14)&&(type!==22)&&(type!==20)&&(type!==
21)&&(type!==23)){var f5=e[aF[eN-1]*4];var j8=type;if((f5===4)||(f5===7))f5=2;if((
j8===4)||(j8===7))j8=2;if((f5!==j8)||((f5!==1)&&(f5!==2)))f5=(e[aF[eN]*4+1]&1)?2:
1;for(;eN<aO;eN++)e[aF[eN]*4]=f5;eN=0;}}}for(ae=3;ae<cL;ae++){var type=e[ae*4];if(
e[ae*4+1]&1)if((type===1)||(type===4)||(type===7))e[ae*4+1]++;else;else if(type===
2)e[ae*4+1]++;else if((type===4)||(type===7))e[ae*4+1]+=2;}for(ae=cL-1,bu=ae+gB;
gB>0;ae--,bu--){if(aF[length+2-gB]===bu){var aj=gJ;var aa=l[i+bu-3];if((ae>=3)&&(
e[ae*4+1]>aj))aj=e[ae*4+1];if((bu<(cL-1))&&(e[(bu+1)*4+1]>aj))aj=e[(bu+1)*4+1];if(
aa===0x200D)e[bu*4+2]=11;else if(((aa<0x0000)||(aa>0x0008))&&((aa<0x000E)||(aa>0x001B
))&&((aa<0x007F)||(aa>0x0084))&&((aa<0x0086)||(aa>0x009F))&&(aa!==0x180E)&&(aa!==
0x200C))e[bu*4+2]=12;else e[bu*4+2]=0;e[bu*4]=31;e[bu*4+1]=aj;ae++;gB--;cL++;}else
if(bu>ae){e[bu*4]=e[ae*4];e[bu*4+1]=e[ae*4+1];e[bu*4+2]=e[ae*4+2];}}};d.l$=function(
ai,l,i,el,fv){var length=ai?ai.eY:0;var e=ai?ai.Data:null;var cL=3+length;var ae=
3;var bu=3;var aj=e[13];for(;(ae<=cL);ae++){var f0=0;var bl=0;var gG=0;if((aj===
e[ae*4+1])&&(ae<cL))continue;for(;(bu<ae)||bl||f0||gG;bu++){var dy;var dA;if(bu<
ae){var a8=e[bu*4+2];if((a8===12)||(a8===4))continue;f0=bl;bl=gG;gG=bu;}else{f0=
bl;bl=gG;gG=0;}if(aj&1){dy=gG;dA=f0;}else{dy=f0;dA=gG;}if(bl&&(e[bl*4+2]===8)){var
q=bl-3;var aa=l[i+q];var c=kg(aa);var ad=el?fv[(el[i+q]>>12)&0xFF]:null;if(!ad)ad=
fv[0];if(c.charCodeAt(2)&&dA&&((e[dA*4+2]===9)||(e[dA*4+2]===10)||(e[dA*4+2]===11
))&&(!ad||this.eG(ad,c.charCodeAt(2))))l[i+q]=c.charCodeAt(2);}else if(bl&&(e[bl
*4+2]===9)){var q=bl-3;var aa=l[i+q];var c=kg(aa);var ad=el?fv[(el[i+q]>>12)&0xFF
]:null;if(!ad)ad=fv[0];if(c.charCodeAt(0)&&dy&&((e[dy*4+2]===8)||(e[dy*4+2]===10
)||(e[dy*4+2]===11))&&(!ad||this.eG(ad,c.charCodeAt(0))))l[i+q]=c.charCodeAt(0);
}else if(bl&&(e[bl*4+2]===10)){var q=bl-3;var aa=l[i+q];var c=kg(aa);var ad=el?fv[(
el[i+q]>>12)&0xFF]:null;if(!ad)ad=fv[0];if(c.charCodeAt(1)&&dA&&dy&&((e[dA*4+2]===
9)||(e[dA*4+2]===10)||(e[dA*4+2]===11))&&((e[dy*4+2]===8)||(e[dy*4+2]===10)||(e[
dy*4+2]===11))){if(!ad||this.eG(ad,c.charCodeAt(1)))l[i+q]=c.charCodeAt(1);}else
if(c.charCodeAt(2)&&dA&&((e[dA*4+2]===9)||(e[dA*4+2]===10)||(e[dA*4+2]===11))){if(
!ad||this.eG(ad,c.charCodeAt(2)))l[i+q]=c.charCodeAt(2);}else if(c.charCodeAt(0)&&
dy&&((e[dy*4+2]===8)||(e[dy*4+2]===10)||(e[dy*4+2]===11))){if(!ad||this.eG(ad,c.
charCodeAt(0)))l[i+q]=c.charCodeAt(0);}}if(bl&&f0){var dN=((aj&1)?bl:f0)-3;var d7=((
aj&1)?f0:bl)-3;var m_=l[i+dN];var m$=l[i+d7];var dO=0;if(m$===0xFEDF)switch(m_){
case 0xFE82:dO=0xFEF5;break;case 0xFE84:dO=0xFEF7;break;case 0xFE88:dO=0xFEF9;break;
case 0xFE8E:dO=0xFEFB;break;}if(m$===0xFEE0)switch(m_){case 0xFE82:dO=0xFEF6;break;
case 0xFE84:dO=0xFEF8;break;case 0xFE88:dO=0xFEFA;break;case 0xFE8E:dO=0xFEFC;break;
}if(dO){var ad=el?fv[(el[i+dN]>>12)&0xFF]:null;if(!ad)ad=fv[0];if(ad&&!this.eG(ad
,dO))dO=0;}if(dO){l[i+dN]=dO;l[i+d7]=0xFEFF;}}}bu=ae;aj=e[ae*4+1];}};d.mb=function(
ai,l,i){var length=ai?ai.eY:0;var e=ai?ai.Data:null;var e7=3+length;var bp;if(!e
)return;for(bp=e7-1;bp>=3;bp--){var a8=e[bp*4+2];if((e[bp*4+1]&1)&&((a8===1)||(a8===
2)||(a8===3))){var nT=lY(l[i+bp-3]);if(nT)l[i+bp-3]=nT;}}};d.kr=function(ai,fA,gs
){var e=ai?ai.Data:null;var c6=fA+3;var e7=gs+3;var gJ=ai?ai.gX:0;var k4=true;var
bp;if(!e)return;for(bp=e7-1;bp>=c6;bp--){var a8=e[bp*4+2];if(a8===6){e[bp*4+1]=gJ;
k4=true;}else if(k4&&((a8===5)||(a8===7)))e[bp*4+1]=gJ;else if(e[bp*4]!==31)k4=0;
}};d.hT=function(ai,fA,gs,cN,fs){var bW=[];var e=ai?ai.Data:null;var c6=fA+3;var
e7=gs+3;var aX=c6;var aj=0;var eB=0;var eA=0;var bp;if(!e)return;for(bp=c6;bp<=e7;
bp++){var is=(bp<e7)?e[bp*4+1]:0;while((is>aj)&&(aj<(125+2))){bW.push(aX);aX=bp;
aj++;}while((is<aj)&&(aj>0)){var iF=aX;aX=bW.pop();aj--;if((bp-iF)>1){var n4=iF-
c6;var n5=bp-c6-1;if((n4===eB)&&(n5===eA)){eB=0;eA=0;continue;}for(;eB<eA;eB++,eA--
){var g=cN[eB+fs];cN[eB+fs]=cN[eA+fs];cN[eA+fs]=g;}eB=n4;eA=n5;}}}for(;eB<eA;eB++
,eA--){var g=cN[eB+fs];cN[eB+fs]=cN[eA+fs];cN[eA+fs]=g;}};d.rr=function(ai,fA,gs
,a$){var bW=[];var e=ai?ai.Data:null;var c6=fA+3;var e7=gs+3;var aX=c6;var aj=0;
var bp;if(!e)return a$;for(bp=c6;bp<=e7;bp++){var is=(bp<e7)?e[bp*4+1]:0;while((
is>aj)&&(aj<(125+2))){bW.push(aX);aX=bp;aj++;}while((is<aj)&&(aj>0)){var iF=aX;aX=
bW.pop();aj--;if((bp-iF)>1){var dN=iF-c6;var d7=bp-c6-1;if((a$>=dN)&&(a$<=d7))a$=
d7-(a$-dN);}}}return a$;};d.rs=function(ai,fA,gs,a$){var lx=[];var le=[];var e=ai?
ai.Data:null;var c6=fA+3;var aX=0;var cI=gs-fA-1;var aj=0;var q=0;if(!e)return a$;
if(cI>65535)return a$;while(aX<cI){var nk=e[(c6+aX)*4+1];var nj=e[(c6+cI)*4+1];if((
nk>aj)&&(nj>aj)&&(aj<(125+2))){lx.push(aX);le.push(cI);aj++;}if(nk<=aj)aX++;if(nj<=
aj)cI--;}while((q<aj)&&(a$>=lx[q])&&(a$<=le[q])){var p7=lx[q];var p6=le[q];var dN=
a$;var d7=a$;while((dN>p7)&&(e[(c6+dN-1)*4+1]>q))dN--;while((d7<p6)&&(e[(c6+d7+1
)*4+1]>q))d7++;a$=d7-(a$-dN);q++;}return a$;};d.rp=function(ai,p2,pM){var e=ai?ai.
Data:null;var bc=ai?ai.eY:0;var q=0;while(q<bc){p2[q]=e[q*4+12];pM[q]=e[q*4+13];
q++;}};d.rG=function(x,av,ac,mt,mu,a5,a_,ay){var a=this.ba;var ab=null;var e9=mt[
0];var e_=mt[1];var hn=mu[0];var ho=mu[1];var aZ=hn;var a2=ho;var db=hn-e9;var hs=
ho-e_;var k,j,p,n;if(x&&(av>=0)&&(av<x.NoOfFrames))ab=x.bT[av];if(!ab||(ab.aH.Format
!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(Math.abs(hn-e9)>Math.abs(ho-e_)){if(aZ>e9)aZ--;if(aZ<e9)aZ++;}else{
if(a2>e_)a2--;if(a2<e_)a2++;}k=Math.max(ac[0],Math.min(e9,aZ),0);j=Math.max(ac[1
],Math.min(e_,a2),0);p=Math.min(ac[2],Math.max(e9,aZ)+1,x.FrameSize[0]);n=Math.min(
ac[3],Math.max(e_,a2)+1,x.FrameSize[1]);if((p<=k)||(n<=j))return;var cG=ab.aL[0];
var cH=ab.aL[1];var b=a.gQ;var aK=a.hM;var am=2;b[0]=e9+cG+0.5;b[1]=e_+cH+0.5;b[
6]=hn+cG+0.5;b[7]=ho+cH+0.5;aK[5]=a5;aK[11]=a_;if(db>0)p+=0.5;if(db<0)k-=0.5;if(
hs>0)n+=0.5;if(hs<0)j-=0.5;if((e9<k)||(e9>p)||(e_<j)||(e_>n)||(hn<k)||(hn>p)||(ho<
j)||(ho>n)){am=iU(b,aK,am,true,k+cG);am=iU(b,aK,am,true,p+cG);am=lP(b,am,k+cG,p+
cG);am=iT(b,aK,am,true,j+cH);am=iT(b,aK,am,true,n+cH);am=lO(b,am,j+cH,n+cH);if(am
!==2)return;}dt(a,ab.aH,a.gY);if(a.bX&&!ay)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(
a.BLEND);a.bX=ay;a.bufferSubData(a.ARRAY_BUFFER,0,a.cs);a.drawArrays(a.LINES,0,am
);if(this.a4)ei(a);};d.g4=function(x,av,ac,r,as,aq,ap,ar,ay){var a=this.ba;var ab=
null;var k,j,p,n;if(x&&(av>=0)&&(av<x.NoOfFrames))ab=x.bT[av];if(!ab||(ab.aH.Format
!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}k=Math.max(ac[0],r[0],0);j=Math.max(ac[1],r[1],0);p=Math.min(ac[2],r[2
],x.FrameSize[0]);n=Math.min(ac[3],r[3],x.FrameSize[1]);var clear=!as&&!aq&&!ar&&
!ap;if((p<=k)||(n<=j)||(clear&&ay))return;if(clear){var b=a.gQ;b[0]=b[12]=k+ab.aL[
0];b[1]=b[7]=j+ab.aL[1];b[6]=b[18]=p+ab.aL[0];b[19]=b[13]=n+ab.aL[1];if(a.bX&&!ay
)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(a.BLEND);a.bX=ay;dt(a,ab.aH,clear?a.hQ:
a.gY);a.bufferSubData(a.ARRAY_BUFFER,0,a.cs);a.drawArrays(a.TRIANGLE_STRIP,0,4);
if(this.a4)ei(a);return;}var f6=(as!==ar)||(aq!==ap);var fU=(as!==aq)||(ar!==ap);
var ex=f6||fU;var q9=f6&&((j!==r[1])||(n!==r[3]));var qM=fU&&((k!==r[0])||(p!==r[
2]));if(q9){var bx=1/(r[3]-r[1]);var bd=(j-r[1])*bx;var aW=(r[3]-n)*bx;if(as!==ar
){var cS=as;if(bd)as=aQ(cS,ar,bd);if(aW)ar=aQ(ar,cS,aW);}if(aq!==ap){var fM=aq;if(
bd)aq=aQ(fM,ap,bd);if(aW)ap=aQ(ap,fM,aW);}}if(qM){var bx=1/(r[2]-r[0]);var cc=(k-
r[0])*bx;var aw=(r[2]-p)*bx;if(as!==aq){var cS=as;if(cc)as=aQ(cS,aq,cc);if(aw)aq=
aQ(aq,cS,aw);}if(ar!==ap){var fH=ar;if(cc)ar=aQ(fH,ap,cc);if(aw)ap=aQ(ap,fH,aw);
}}var b=a.gQ;var aK=a.hM;b[0]=b[12]=k+ab.aL[0];b[1]=b[7]=j+ab.aL[1];b[6]=b[18]=p+
ab.aL[0];b[19]=b[13]=n+ab.aL[1];aK[5]=as;aK[11]=aq;aK[23]=ap;aK[17]=ar;if(a.bX&&
!ay)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(a.BLEND);a.bX=ay;dt(a,ab.aH,clear?a.
hQ:a.gY);a.bufferSubData(a.ARRAY_BUFFER,0,a.cs);a.drawArrays(a.TRIANGLE_STRIP,0,
4);if(this.a4)ei(a);};d.rF=function(x,av,ac,r,e1,as,aq,ap,ar,ay){if(e1<=0)return;
if(((e1*2)>=(r[2]-r[0]))||((e1*2)>=(r[3]-r[1]))){this.g4(x,av,ac,r,as,aq,ap,ar,ay
);return;}var cq=[0,0,0,0];var fG;cq[0]=r[0];cq[1]=r[1];cq[2]=r[2];cq[3]=r[1]+e1;
fG=this.hV(ac,cq);this.g4(x,av,fG,r,as,aq,ap,ar,ay);cq[0]=r[0];cq[1]=r[3]-e1;cq[
2]=r[2];cq[3]=r[3];fG=this.hV(ac,cq);this.g4(x,av,fG,r,as,aq,ap,ar,ay);cq[0]=r[0
];cq[1]=r[1]+e1;cq[2]=r[0]+e1;cq[3]=r[3]-e1;fG=this.hV(ac,cq);this.g4(x,av,fG,r,
as,aq,ap,ar,ay);cq[0]=r[2]-e1;cq[2]=r[2];fG=this.hV(ac,cq);this.g4(x,av,fG,r,as,
aq,ap,ar,ay);};d.ry=function(kX){if(kX<1)return null;return{hO:kX,oT:0,oS:0,aJ:0
,bU:true,by:[0,0,0,0],dE:[1.0,0.0,0.0,0.0,1.0,0.0],kj:[],ce:new Array(kX)};};d.rM=
function(s){if(s){delete s.ce;delete s.kj;}};d.rS=function(s){if(!s)return 0;return s.
hO;};d.r9=function(s,bq,h6){if(!s||(bq>=s.hO)||(bq<0))return false;if(h6<=0)h6=0;
var v=null;if(h6)v={gR:h6,aJ:0,fi:false,ef:false,cM:false,bU:true,by:[0,0,0,0],Data:
new Float32Array((h6+1)*8)};if(s.ce[bq])s.bU=false;s.ce[bq]=v;return false;};d.ro=
function(s,bq,aY,bk){var v=s?s.ce[bq]:null;if(!v)return;var o=s.dE;var an=aY;var
aG=bk;if((o[0]!==1)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1)||(o[5]!==0)){
an=aY*o[0]+bk*o[1]+o[2];aG=aY*o[3]+bk*o[4]+o[5];}v.cM=false;v.ef=false;v.bU=false;
v.aJ=0;v.fi=true;v.Data[0]=an;v.Data[1]=aG;s.bU=false;};d.rj=function(s,bq,aY,bk
){var v=s?s.ce[bq]:null;if(!v||v.cM||((v.aJ+1)>v.gR))return 0;var o=s.dE;var aI=
v.Data;var bv=v.aJ*2+2;var an=aY;var aG=bk;v.fi=true;v.ef=true;v.bU=false;s.bU=false;
if((o[0]!==1)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1)||(o[5]!==0)){an=aY
*o[0]+bk*o[1]+o[2];aG=aY*o[3]+bk*o[4]+o[5];}aI[bv]=an;aI[bv+1]=aG;return v.aJ++;
};d.rg=function(s,bq,kN,kO,aY,bk,bI){var v=s?s.ce[bq]:null;if((bI<=0)||!v||v.cM||((
v.aJ+bI)>v.gR))return 0;var o=s.dE;var aI=v.Data;var bv=v.aJ*2;var ng=kN;var nh=
kO;var an=aY;var aG=bk;var dB=1/bI;var bd;var h;v.fi=true;v.ef=true;v.bU=false;s.
bU=false;if((o[0]!==1)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1)||(o[5]!==
0)){ng=kN*o[0]+kO*o[1]+o[2];nh=kN*o[3]+kO*o[4]+o[5];an=aY*o[0]+bk*o[1]+o[2];aG=aY
*o[3]+bk*o[4]+o[5];}var lG=aI[bv++];var lH=aI[bv++];for(bd=dB,h=1;h<bI;h++,bd+=dB
){var fZ=1-bd;var iy=fZ*fZ;var bN=bd*bd;var iw=2*fZ*bd;aI[bv++]=iy*lG+iw*ng+bN*an;
aI[bv++]=iy*lH+iw*nh+bN*aG;}aI[bv++]=an;aI[bv++]=aG;return(v.aJ+=h)-h;};d.rh=function(
s,bq,kJ,kK,kL,kM,aY,bk,bI){var v=s?s.ce[bq]:null;if((bI<=0)||!v||v.cM||((v.aJ+bI
)>v.gR))return 0;var o=s.dE;var aI=v.Data;var bv=v.aJ*2;var nc=kJ;var nd=kK;var ne=
kL;var nf=kM;var an=aY;var aG=bk;var dB=1/bI;var bd;var h;v.fi=true;v.ef=true;v.
bU=false;s.bU=false;if((o[0]!==1)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1
)||(o[5]!==0)){nc=kJ*o[0]+kK*o[1]+o[2];nd=kJ*o[3]+kK*o[4]+o[5];ne=kL*o[0]+kM*o[1
]+o[2];nf=kL*o[3]+kM*o[4]+o[5];an=aY*o[0]+bk*o[1]+o[2];aG=aY*o[3]+bk*o[4]+o[5];}
var lG=aI[bv++];var lH=aI[bv++];for(bd=dB,h=1;h<bI;h++,bd+=dB){var fZ=1-bd;var bN=
bd*bd;var iy=fZ*fZ;var no=fZ*iy;var iw=3*iy*bd;var np=3*fZ*bN;var nq=bd*bN;aI[bv++
]=no*lG+iw*nc+np*ne+nq*an;aI[bv++]=no*lH+iw*nd+np*nf+nq*aG;}aI[bv++]=an;aI[bv++]=
aG;return(v.aJ+=h)-h;};d.rf=function(s,bq,pE,pF,pR,pS,ii,jg,bI){var v=s?s.ce[bq]:
null;var dz=bI;if(v&&v.fi)dz++;if((bI<=0)||(ii===jg)||!v||v.cM||((v.aJ+dz)>v.gR)
)return 0;ii*=this.ks;jg*=this.ks;var o=s.dE;var aI=v.Data;var bv=v.aJ*2+(dz-bI)
*2;var dB=(jg-ii)/bI;var h;v.fi=true;v.ef=true;v.bU=false;s.bU=false;var qA=(o[0
]!==1)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1)||(o[5]!==0);for(h=0;h<=bI;
h++,ii+=dB){var bG=(h===bI)?jg:ii;var an=pE+pR*Math.cos(bG);var aG=pF+pS*Math.sin(
bG);if(qA){aI[bv++]=an*o[0]+aG*o[1]+o[2];aI[bv++]=an*o[3]+aG*o[4]+o[5];}else{aI[
bv++]=an;aI[bv++]=aG;}}return(v.aJ+=dz)-bI;};d.ri=function(s,bq,mI,pX,jn,bI){var
er=s?s.ce[bq]:null;var j5=mI?mI.ce[pX]:null;var dz=bI;if(!bI||!er||!j5||(jn<0))return 0;
if(bI<0)dz=bI=j5.aJ-jn;if((jn+dz)>j5.aJ)return 0;if(er.fi)dz++;if(((er.aJ+dz)>er.
gR)||er.cM)return 0;er.fi=true;er.ef=true;er.bU=false;s.bU=false;var o=s.dE;var aI=
er.Data;var dB=j5.Data;var bv=er.aJ*2+(dz-bI)*2;var aw=jn*2;var h=0;if((o[0]!==1
)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1)||(o[5]!==0))for(;h<=bI;h++,aw+=
2){aI[bv++]=dB[aw]*o[0]+dB[aw+1]*o[1]+o[2];aI[bv++]=dB[aw]*o[3]+dB[aw+1]*o[4]+o[
5];}else for(;h<=bI;h++){aI[bv++]=dB[aw++];aI[bv++]=dB[aw++];}return(er.aJ+=dz)-
bI;};d.rw=function(s,bq){var v=s?s.ce[bq]:null;if(!v||v.cM||!v.ef)return;v.ef=false;
v.cM=true;s.bU=false;var aI=v.Data;var cG=aI[v.aJ*2+0]-aI[0];var cH=aI[v.aJ*2+1]-
aI[1];if((cG>0.001)||(cG<-0.001)||(cH>0.001)||(cH<-0.001)){v.aJ++;aI[v.aJ*2+0]=aI[
0];aI[v.aJ*2+1]=aI[1];}else{aI[v.aJ*2+0]=aI[0];aI[v.aJ*2+1]=aI[1];}};d.s1=function(
s,bq,go,mK,p1){var v=s?s.ce[bq]:null;if(!v||v.cM||!v.ef)return 0;if(go<=0)return 0;
if(go>v.aJ)go=v.aJ;v.Data.copyWithin(0,go*2);if(mK!==0.0){var aI=v.Data;var h;for(
h=0;h<aI.length;h+=2){aI[h]+=mK;aI[h+1]+=p1;}}v.ef=go<v.aJ;v.aJ=v.aJ-go;v.bU=false;
s.bU=false;return go;};d.s0=function(s,bq,dw,aY,bk){var v=s?s.ce[bq]:null;if(!v||(
dw>v.aJ)||(dw<0))return;var o=s.dE;var aI=v.Data;var an=aY;var aG=bk;if((o[0]!==
1)||(o[1]!==0)||(o[2]!==0)||(o[3]!==0)||(o[4]!==1)||(o[5]!==0)){an=aY*o[0]+bk*o[
1]+o[2];aG=aY*o[3]+bk*o[4]+o[5];}aI[dw*2+0]=an;aI[dw*2+1]=aG;if(!dw&&v.cM){aI[v.
aJ*2+0]=an;aI[v.aJ*2+1]=aG;}if((dw===v.aJ)&&v.cM){aI[0]=an;aI[1]=aG;}v.bU=false;
s.bU=false;};d.r1=function(s,bq,dw){var v=s?s.ce[bq]:null;if(!v||(dw>v.aJ)||(dw<
0))return 0.0;return v.Data[dw*2+0];};d.r2=function(s,bq,dw){var v=s?s.ce[bq]:null;
if(!v||(dw>v.aJ)||(dw<0))return 0.0;return v.Data[dw*2+1];};d.rW=function(s,bq){
var v=s?s.ce[bq]:null;return v?v.aJ:0;};d.rV=function(s,bq){var v=s?s.ce[bq]:null;
if(!v||v.cM)return 0;return v.gR-v.aJ;};d.sf=function(s,bq){var v=s?s.ce[bq]:null;
return v?v.cM:false;};d.tb=function(s,mq,mr){if(!s)return;var o=s.dE;o[2]=o[2]+(
o[0]*mq)+(o[1]*mr);o[5]=o[5]+(o[3]*mq)+(o[4]*mr);};d.sM=function(s,mG,mH){if(!s)
return;var o=s.dE;o[0]*=mG;o[1]*=mH;o[3]*=mG;o[4]*=mH;};d.sL=function(s,mm){if(!
s)return;var o=s.dE;var sin=Math.sin(mm*=this.ks);var cos=Math.cos(mm);var nF,nG
,nH,nI;nF=(o[0]*cos)+(o[1]*sin);nG=(o[0]*-sin)+(o[1]*cos);nH=(o[3]*cos)+(o[4]*sin
);nI=(o[3]*-sin)+(o[4]*cos);o[0]=nF;o[1]=nG;o[3]=nH;o[4]=nI;};d.sG=function(s){if(
!s)return;var o=s.dE;s.kj.push([o[0],o[1],o[2],o[3],o[4],o[5]]);};d.sF=function(
s){if(!s)return;var o=s.dE;var gH=s.kj.pop();if(gH){o[0]=gH[0];o[1]=gH[1];o[2]=gH[
2];o[3]=gH[3];o[4]=gH[4];o[5]=gH[5];}else{o[0]=1;o[1]=0;o[2]=0;o[3]=0;o[4]=1;o[5
]=0;}};d.r8=function(s){if(!s)return;var o=s.dE;o[0]=1;o[1]=0;o[2]=0;o[3]=0;o[4]=
1;o[5]=0;};d.jc=function(s){if(!s||s.bU)return;var hH=s.ce;var hy=s.hO;var k5=[0
,0,0,0];var nM=0;var nJ=0;var dz=0;var h;for(h=0;h<hy;h++){var v=hH[h];if(v&&!v.
bU&&v.aJ){var aI=v.Data;var bv=2;var k=aI[0];var j=aI[1];var p=k;var n=j;var am=
v.aJ;for(;am>0;am--,bv+=2){var an=aI[bv];var aG=aI[bv+1];if(an<k)k=an;if(an>p)p=
an;if(aG<j)j=aG;if(aG>n)n=aG;}v.by=[k|0,j|0,(p+1)|0,(n+1)|0];v.bU=true;}if(v&&v.
bU){nM+=v.ef?1:0;nJ+=v.cM?1:0;dz+=v.aJ;k5=this.pA(k5,v.by);}}s.oT=nM;s.oS=nJ;s.aJ=
dz;s.by=k5;s.bU=true;};d.pg=function(bf,s,dH,af,h2,kZ){var aU=af[0];var aE=af[1];
var k,j,p,n;if((bf[0]<=0)||(bf[1]<=0)||!s)return null;this.jc(s);if(!dH){j=aE+s.
by[1]-1;n=aE+s.by[3]+1;}else{aE+=bf[1];j=aE-s.by[3]-1;n=aE-s.by[1]+1;}k=aU+s.by[
0]-1;p=aU+s.by[2]+1;k=Math.max(k,0);j=Math.max(j,0);p=Math.min(p,bf[0]);n=Math.min(
n,bf[1]);if((p<=k)||(n<=j)||!s.aJ)return null;var a=this.ba;var aD=document.createElement(
"canvas");var d_=a.createTexture();var at={};var ck={aH:at,aL:[0,0]};var bitmap={
FrameSize:bf,FrameDelay:0,NoOfFrames:1,cx:[at],bT:[ck]};aD.width=bf[0];aD.height=
bf[1];var aS=aD.getContext("2d");var hH=s.ce;var hy=s.hO;var h;aS.translate(aU,aE
);aS.scale(1,dH?-1:1);aS.beginPath();for(h=0;h<hy;h++){var v=hH[h];if(v&&v.aJ){var
e=v.Data;var lr=v.aJ*2+2;var b2;aS.moveTo(e[0],e[1]);for(b2=2;b2<lr;b2+=2)aS.lineTo(
e[b2],e[b2+1]);if(v.cM)aS.closePath();}}aS.fill(kZ?"nonzero":"evenodd");a.fm=null;
a.bindTexture(a.TEXTURE_2D,d_);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MIN_FILTER
,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MAG_FILTER,a.NEAREST);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.CLAMP_TO_EDGE);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_T
,a.CLAMP_TO_EDGE);a.texImage2D(a.TEXTURE_2D,0,a.RGBA,a.RGBA,a.UNSIGNED_BYTE,aD);
at.Format=d.kC;at.Width=bf[0];at.Height=bf[1];at.hS=bf[0];at.hR=bf[1];at.dW=1/bf[
0];at.dV=1/bf[1];at.eE=2/bf[0];at.eD=2/bf[1];at.cB=d_;at.Framebuffer=null;return bitmap;
};d.ph=function(bf,s,dH,af,aA,fB,gm,h2){var da=fB&0x0000FF;var c$=fB&0x00FF00;var
eT=fB&0xFF0000;var aU=af[0];var aE=af[1];var k,j,p,n;var cl;if((bf[0]<=0)||(bf[1
]<=0)||!s||(aA<=0))return null;if((da===this.jb)&&(aA<4))da=this.ky;if((c$===this.
ja)&&(aA<4))c$=this.kw;if((eT===this.kB)&&(aA<3))eT=this.kz;if(eT===this.kA){if(
gm>1)cl=(gm*aA*0.5)|0;else cl=(aA*0.5)|0;}else if((da===this.hZ)||(c$===this.hY)
)cl=(aA*0.75)|0;else cl=(aA+0.5)|0;this.jc(s);if(!dH){j=aE+s.by[1]-cl-1;n=aE+s.by[
3]+cl+1;}else{aE+=bf[1];j=aE-s.by[3]-cl-1;n=aE-s.by[1]+cl+1;}k=aU+s.by[0]-cl-1;p=
aU+s.by[2]+cl+1;k=Math.max(k,0);j=Math.max(j,0);p=Math.min(p,bf[0]);n=Math.min(n
,bf[1]);if((p<=k)||(n<=j)||!s.aJ)return;var a=this.ba;var aD=document.createElement(
"canvas");var d_=a.createTexture();var at={};var ck={aH:at,aL:[0,0]};var bitmap={
FrameSize:bf,FrameDelay:0,NoOfFrames:1,cx:[at],bT:[ck]};aD.width=bf[0];aD.height=
bf[1];var aS=aD.getContext("2d");var hH=s.ce;var hy=s.hO;var h;aS.translate(aU,aE
);aS.scale(1,dH?-1:1);aS.beginPath();for(h=0;h<hy;h++){var v=hH[h];if(v&&v.aJ){var
e=v.Data;var lr=v.aJ*2+2;var b2;aS.moveTo(e[0],e[1]);for(b2=2;b2<lr;b2+=2)aS.lineTo(
e[b2],e[b2+1]);if(v.cM)aS.closePath();}}if(aA!==1.0)aS.lineWidth=aA;if(eT===this.
kA)aS.miterLimit=gm;else if(eT===this.kz)aS.lineJoin="bevel";else if(eT===this.kB
)aS.lineJoin="round";if((da===this.jb)&&(c$===this.ja)){da=c$=this.kx;aS.lineCap=
"round";}else if((da===this.hZ)&&(c$===this.hY)){da=c$=this.kx;aS.lineCap="square";
}aS.stroke();if((da!==this.kx)||(c$!==this.py)){aS.beginPath();for(h=0;h<hy;h++){
var v=hH[h];if(v&&v.aJ&&!v.cM){var e=v.Data;var hD=0;var hC=v.aJ*2+1;var eb=e[hD++
];var ed=e[hD++];var ec=e[hC--];var ea=e[hC--];if((da===this.hZ)||(da===this.ky)
){var cu,cw;do{cu=e[hD++];cw=e[hD++];}while((cu===eb)&&(cw===ed)&&(hD<hC));cu=eb-
cu;cw=ed-cw;if(cu||cw){var bc=Math.sqrt(cu*cu+cw*cw);var dQ,dR;dQ=cu/bc;cu=dQ*aA
*0.5;dR=cw/bc;cw=dR*aA*0.5;if(da===this.hZ){aS.moveTo(eb+cw-dQ,ed-cu-dR);aS.lineTo(
eb-cw-dQ,ed+cu-dR);aS.lineTo(eb-cw+cu,ed+cu+cw);aS.lineTo(eb+cw+cu,ed-cu+cw);}else{
aS.moveTo(eb+cw,ed-cu);aS.lineTo(eb+cw-dQ,ed-cu-dR);aS.lineTo(eb-cw-dQ,ed+cu-dR);
aS.lineTo(eb-cw,ed+cu);aS.lineTo(eb+cu,ed+cw);}aS.closePath();}}else if(da===this.
jb)aS.arc(eb,ed,aA/2,0,2*Math.PI);if((c$===this.hY)||(c$===this.kw)){var ct,cv;do{
cv=e[hC--];ct=e[hC--];}while((ct===ea)&&(cv===ec)&&(hD<hC));ct=ea-ct;cv=ec-cv;if(
ct||cv){var bc=Math.sqrt(ct*ct+cv*cv);var dQ,dR;dQ=ct/bc;ct=dQ*aA*0.5;dR=cv/bc;cv=
dR*aA*0.5;if(c$===this.hY){aS.moveTo(ea+cv-dQ,ec-ct-dR);aS.lineTo(ea-cv-dQ,ec+ct-
dR);aS.lineTo(ea-cv+ct,ec+ct+cv);aS.lineTo(ea+cv+ct,ec-ct+cv);}else{aS.moveTo(ea+
cv,ec-ct);aS.lineTo(ea+cv-dQ,ec-ct-dR);aS.lineTo(ea-cv-dQ,ec+ct-dR);aS.lineTo(ea-
cv,ec+ct);aS.lineTo(ea+ct,ec+cv);}aS.closePath();}}else if(c$===this.ja)aS.arc(ea
,ec,aA/2,0,2*Math.PI);}}aS.fill();}a.fm=null;a.bindTexture(a.TEXTURE_2D,d_);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_MIN_FILTER,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_MAG_FILTER
,a.NEAREST);a.texParameteri(a.TEXTURE_2D,a.TEXTURE_WRAP_S,a.CLAMP_TO_EDGE);a.texParameteri(
a.TEXTURE_2D,a.TEXTURE_WRAP_T,a.CLAMP_TO_EDGE);a.texImage2D(a.TEXTURE_2D,0,a.RGBA
,a.RGBA,a.UNSIGNED_BYTE,aD);at.Format=d.kC;at.Width=bf[0];at.Height=bf[1];at.hS=
bf[0];at.hR=bf[1];at.dW=1/bf[0];at.dV=1/bf[1];at.eE=2/bf[0];at.eD=2/bf[1];at.cB=
d_;at.Framebuffer=null;return bitmap;};d.rH=function(x,s,av,ac,r,dH,af,as,aq,ap,
ar,ay,h2,kZ){var a=this.ba;var ab=null;var aU=af[0];var aE=af[1];var k,j,p,n;if(
x&&(av>=0)&&(av<x.NoOfFrames))ab=x.bT[av];if(!ab||(ab.aH.Format!==this.b9)){console.
log("EmWi error: Invalid destination bitmap in the drawing operation.");return;}
if(!s){if(this.a4)console.log("EmWi error: Invalid path object in the drawing operation."
);return;}this.jc(s);if(!dH){aE+=r[1];j=aE+s.by[1]-1;n=aE+s.by[3]+1;}else{aE+=r[
3];j=aE-s.by[3]-1;n=aE-s.by[1]+1;}aU+=r[0];k=aU+s.by[0]-1;p=aU+s.by[2]+1;k=Math.
max(ac[0],r[0],k,0);j=Math.max(ac[1],r[1],j,0);p=Math.min(ac[2],r[2],p,x.FrameSize[
0]);n=Math.min(ac[3],r[3],n,x.FrameSize[1]);aU-=k;if((p<=k)||(n<=j)||!s.aJ)return;
if(dH)aE-=n;else aE-=j;var bitmap=this.pg([p-k,n-j],s,dH,[aU,aE],h2,kZ);if(bitmap===
null)return;this.fo(x,bitmap,av,0,[k,j,p,n],r,[r[0]-k,r[1]-j],as,aq,ap,ar,ay);a.
deleteTexture(bitmap.cx[0].cB);};d.s_=function(x,s,av,ac,r,dH,af,aA,fB,gm,as,aq,
ap,ar,ay,h2){var da=fB&0x0000FF;var c$=fB&0x00FF00;var eT=fB&0xFF0000;var a=this.
ba;var ab=null;var aU=af[0];var aE=af[1];var k,j,p,n;var cl;if(x&&(av>=0)&&(av<x.
NoOfFrames))ab=x.bT[av];if(!ab||(ab.aH.Format!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!s){if(this.a4)console.log("EmWi error: Invalid path object in the drawing operation."
);return;}this.jc(s);if((da===this.jb)&&(aA<4))da=this.ky;if((c$===this.ja)&&(aA<
4))c$=this.kw;if((eT===this.kB)&&(aA<3))eT=this.kz;if(eT===this.kA){if(gm>1)cl=(
gm*aA*0.5)|0;else cl=(aA*0.5)|0;}else if((da===this.hZ)||(c$===this.hY))cl=(aA*0.75
)|0;else cl=(aA+0.5)|0;if(!dH){aE+=r[1];j=aE+s.by[1]-cl-1;n=aE+s.by[3]+cl+1;}else{
aE+=r[3];j=aE-s.by[3]-cl-1;n=aE-s.by[1]+cl+1;}aU+=r[0];k=aU+s.by[0]-cl-1;p=aU+s.
by[2]+cl+1;k=Math.max(ac[0],r[0],k,0);j=Math.max(ac[1],r[1],j,0);p=Math.min(ac[2
],r[2],p,x.FrameSize[0]);n=Math.min(ac[3],r[3],n,x.FrameSize[1]);aU-=k;if((p<=k)||(
n<=j)||!s.aJ||(aA<=0))return;if(dH)aE-=n;else aE-=j;var bitmap=this.ph([p-k,n-j]
,s,dH,[aU,aE],aA,fB,gm,h2);if(bitmap===null)return;this.fo(x,bitmap,av,0,[k,j,p,
n],r,[r[0]-k,r[1]-j],as,aq,ap,ar,ay);a.deleteTexture(bitmap.cx[0].cB);};d.fo=function(
x,aC,av,bD,ac,r,dm,as,aq,ap,ar,ay){var a=this.ba;var ab=null;var src=null;var k=
r[0];var j=r[1];var p=r[2];var n=r[3];var d9=dm[0];var eU=dm[1];if(x&&(av>=0)&&(
av<x.NoOfFrames))ab=x.bT[av];if(aC&&(bD>=0)&&(bD<aC.NoOfFrames))src=aC.bT[bD];if(
aC.eX)return;if(!ab||(ab.aH.Format!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a4)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}p=Math.min(p,k+aC.FrameSize[0]-dm[0]);n=Math.min(n,j+aC.FrameSize[1]-dm[
1]);k=Math.max(k,k-dm[0]);j=Math.max(j,j-dm[1]);k=Math.max(ac[0],k,0);j=Math.max(
ac[1],j,0);p=Math.min(ac[2],p,x.FrameSize[0]);n=Math.min(ac[3],n,x.FrameSize[1]);
if((p<=k)||(n<=j))return;var f6=(as!==ar)||(aq!==ap);var fU=(as!==aq)||(ar!==ap);
var ex=f6||fU;var gy=ex||(((as>>24)&0xFF)<255);d9+=k-r[0]+src.aL[0];eU+=j-r[1]+src.
aL[1];var a7=d9*src.aH.dW;var a3=eU*src.aH.dV;var bo=(d9+p-k)*src.aH.dW;var bm=(
eU+n-j)*src.aH.dV;if(f6&&((j!==r[1])||(n!==r[3]))){var bx=1/(r[3]-r[1]);var bd=(
j-r[1])*bx;var aW=(r[3]-n)*bx;if(as!==ar){var cS=as;if(bd)as=aQ(cS,ar,bd);if(aW)
ar=aQ(ar,cS,aW);}if(aq!==ap){var fM=aq;if(bd)aq=aQ(fM,ap,bd);if(aW)ap=aQ(ap,fM,aW
);}}if(fU&&((k!==r[0])||(p!==r[2]))){var bx=1/(r[2]-r[0]);var cc=(k-r[0])*bx;var
aw=(r[2]-p)*bx;if(as!==aq){var cS=as;if(cc)as=aQ(cS,aq,cc);if(aw)aq=aQ(aq,cS,aw);
}if(ar!==ap){var fH=ar;if(cc)ar=aQ(fH,ap,cc);if(aw)ap=aQ(ap,fH,aw);}}var b=a.gQ;
var aK=a.hM;b[0]=b[12]=k+ab.aL[0];b[1]=b[7]=j+ab.aL[1];b[6]=b[18]=p+ab.aL[0];b[19
]=b[13]=n+ab.aL[1];b[2]=a7;b[3]=a3;b[4]=1;b[8]=bo;b[9]=a3;b[10]=1;b[20]=bo;b[21]=
bm;b[22]=1;b[14]=a7;b[15]=bm;b[16]=1;aK[5]=as;aK[11]=aq;aK[23]=ap;aK[17]=ar;if(src.
aH.Format===this.b9)dt(a,ab.aH,gy?a.g0:a.gZ);else dt(a,ab.aH,a.f9);g1(a,src.aH,false
);if(a.bX&&!ay)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(a.BLEND);a.bX=ay;a.bufferSubData(
a.ARRAY_BUFFER,0,a.cs);a.drawArrays(a.TRIANGLE_STRIP,0,4);if(this.a4)ei(a);};d.h0=
function(x,aC,av,bD,ac,r,cp,dm,as,aq,ap,ar,ay){var a=this.ba;var ab=null;var src=
null;var k=r[0];var j=r[1];var p=r[2];var n=r[3];var f2=cp[0];var f3=cp[1];var hI=
cp[2];var hJ=cp[3];if(x&&(av>=0)&&(av<x.NoOfFrames))ab=x.bT[av];if(aC&&(bD>=0)&&(
bD<aC.NoOfFrames))src=aC.bT[bD];if(aC.eX)return;if(!ab||(ab.aH.Format!==this.b9)
){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a4)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}f2=Math.max(f2,0);f3=Math.max(f3,0);hI=Math.min(hI,aC.FrameSize[0]);hJ=
Math.min(hJ,aC.FrameSize[1]);var iK=hI-f2;var iC=hJ-f3;k=Math.max(ac[0],k,0);j=Math.
max(ac[1],j,0);p=Math.min(ac[2],p,x.FrameSize[0]);n=Math.min(ac[3],n,x.FrameSize[
1]);if((p<=k)||(n<=j)||(iK<=0)||(iC<=0))return;var f6=(as!==ar)||(aq!==ap);var fU=(
as!==aq)||(ar!==ap);var ex=f6||fU;var gy=ex||(((as>>24)&0xFF)<255);var j1;if(f6&&((
j!==r[1])||(n!==r[3]))){var bx=1/(r[3]-r[1]);var bd=(j-r[1])*bx;var aW=(r[3]-n)*
bx;if(as!==ar){var cS=as;if(bd)as=aQ(cS,ar,bd);if(aW)ar=aQ(ar,cS,aW);}if(aq!==ap
){var fM=aq;if(bd)aq=aQ(fM,ap,bd);if(aW)ap=aQ(ap,fM,aW);}}if(fU&&((k!==r[0])||(p
!==r[2]))){var bx=1/(r[2]-r[0]);var cc=(k-r[0])*bx;var aw=(r[2]-p)*bx;if(as!==aq
){var cS=as;if(cc)as=aQ(cS,aq,cc);if(aw)aq=aQ(aq,cS,aw);}if(ar!==ap){var fH=ar;if(
cc)ar=aQ(fH,ap,cc);if(aw)ap=aQ(ap,fH,aw);}}var b=a.gQ;var aK=a.hM;if(src.aH.Format===
this.b9)dt(a,ab.aH,j1=(gy?a.km:a.kl));else dt(a,ab.aH,j1=a.kn);g1(a,src.aH,false
);a.uniform2f(j1.o8,iK*src.aH.dW,iC*src.aH.dV);a.uniform2f(j1.o7,(f2+src.aL[0])*
src.aH.dW,(f3+src.aL[1])*src.aH.dV);f2=(dm[0]+k-r[0]+iK)/iK;f3=(dm[1]+j-r[1]+iC)
/iC;hI=f2+((p-k)/iK);hJ=f3+((n-j)/iC);b[0]=b[12]=k+ab.aL[0];b[1]=b[7]=j+ab.aL[1];
b[6]=b[18]=p+ab.aL[0];b[19]=b[13]=n+ab.aL[1];b[2]=f2;b[3]=f3;b[4]=1;b[8]=hI;b[9]=
f3;b[10]=1;b[20]=hI;b[21]=hJ;b[22]=1;b[14]=f2;b[15]=hJ;b[16]=1;aK[5]=as;aK[11]=aq;
aK[23]=ap;aK[17]=ar;if(a.bX&&!ay)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(a.BLEND
);a.bX=ay;a.bufferSubData(a.ARRAY_BUFFER,0,a.cs);a.drawArrays(a.TRIANGLE_STRIP,0
,4);if(this.a4)ei(a);};d.rE=function(x,aC,av,bD,ac,r,cp,cO,cP,c2,c1,ms,as,aq,ap,
ar,ay){var bB=[0,0];var bg=r[0];var bh=r[1];var aZ=r[2];var a2=r[3];var a7=cp[0];
var a3=cp[1];var bo=cp[2];var bm=cp[3];var bz=aZ-bg;var bt=a2-bh;var bE=((bo-a7)
/3)|0;var br=((bm-a3)/3)|0;var fW=(bo-a7-bE*2)|0;var fV=(bm-a3-br*2)|0;var nP=0;
var ls=0;var fK=as;var fL=aq;var fI=ar;var fJ=ap;var hl=as;var hm=aq;var hj=ar;var
hk=ap;var jF=as;var jH=aq;var jE=ar;var jG=ap;if((bz<=0)||(bt<=0)||(fW<=0)||(fV<=
0))return;if(aC&&aC.eX)return;if(cO&&c2&&cP&&c1&&ms&&(bz===(bo-a7))&&(bt===(bm-a3
))){this.fo(x,aC,av,bD,ac,r,[a7,a3],as,aq,ap,ar,ay);return;}if((as!==aq)||(ap!==
ar)||(aq!==ar)){if(cO){hl=aQ(as,aq,bE/bz);hj=aQ(ar,ap,bE/bz);}if(c2){hm=aQ(aq,as
,bE/bz);hk=aQ(ap,ar,bE/bz);}if(cP){jF=aQ(as,ar,br/bt);jH=aQ(aq,ap,br/bt);}if(c1){
jE=aQ(ar,as,br/bt);jG=aQ(ap,aq,br/bt);}if(!cO&&!cP)fK=as;if(cO&&!cP)fK=hl;if(!cO&&
cP)fK=jF;if(!c2&&!cP)fL=aq;if(c2&&!cP)fL=hm;if(!c2&&cP)fL=jH;if(!cO&&!c1)fI=ar;if(
cO&&!c1)fI=hj;if(!cO&&c1)fI=jE;if(!c2&&!c1)fJ=ap;if(c2&&!c1)fJ=hk;if(!c2&&c1)fJ=
jG;if(cO&&cP)fK=aQ(hl,hj,br/bt);if(cO&&c1)fI=aQ(hj,hl,br/bt);if(c2&&cP)fL=aQ(hm,
hk,br/bt);if(c2&&c1)fJ=aQ(hk,hm,br/bt);}if(c2&&!cO)nP=bE-bz;if(c1&&!cP)ls=br-bt;
bo=a7+bE;bm=a3+br;aZ=bg+bE;a2=bh+br;if(cO&&cP)this.fo(x,aC,av,bD,ac,[bg,bh,aZ,a2
],[a7,a3],as,hl,fK,jF,ay);bg+=bz-bE;aZ+=bz-bE;a7+=bE+fW;bo+=bE+fW;if(c2&&cP)this.
fo(x,aC,av,bD,ac,[bg,bh,aZ,a2],[a7,a3],hm,aq,jH,fL,ay);bh+=bt-br;a2+=bt-br;a3+=br+
fV;bm+=br+fV;if(c2&&c1)this.fo(x,aC,av,bD,ac,[bg,bh,aZ,a2],[a7,a3],fJ,jG,ap,hk,ay
);bg-=bz-bE;aZ-=bz-bE;a7-=bE+fW;bo-=bE+fW;if(cO&&c1)this.fo(x,aC,av,bD,ac,[bg,bh
,aZ,a2],[a7,a3],jE,fI,hj,ar,ay);bh-=bt-br;a3-=fV;bm-=br;bB[1]=ls;if(cP)bh+=br;if(
c1)a2-=br;if(cO)this.h0(x,aC,av,bD,ac,[bg,bh,aZ,a2],[a7,a3,bo,bm],bB,jF,fK,fI,jE
,ay);bg+=bz-bE;aZ+=bz-bE;a7+=bE+fW;bo+=bE+fW;if(c2)this.h0(x,aC,av,bD,ac,[bg,bh,
aZ,a2],[a7,a3,bo,bm],bB,fL,jH,jG,fJ,ay);if(cP)bh-=br;a2=bh+br;bg-=bz-bE;a7-=fW;bo-=
bE;a3-=br;bm-=fV;bB[0]=nP;bB[1]=0;if(cO)bg+=bE;if(c2)aZ-=bE;if(cP)this.h0(x,aC,av
,bD,ac,[bg,bh,aZ,a2],[a7,a3,bo,bm],bB,hl,hm,fL,fK,ay);bh+=bt-br;a2+=bt-br;a3+=br+
fV;bm+=br+fV;if(c1)this.h0(x,aC,av,bD,ac,[bg,bh,aZ,a2],[a7,a3,bo,bm],bB,fI,fJ,hk
,hj,ay);if(cO)bg-=bE;aZ=bg+bz;bh-=bt-br;a3-=fV;bm-=br;bB[1]=ls;if(cO)bg+=bE;if(cP
)bh+=br;if(c2)aZ-=bE;if(c1)a2-=br;if(ms)this.h0(x,aC,av,bD,ac,[bg,bh,aZ,a2],[a7,
a3,bo,bm],bB,fK,fL,fJ,fI,ay);};d.tc=function(x,aC,av,bD,ac,gi,gk,c3,gj,hb,cQ,g$,
gl,cD,ha,hc,c4,cp,a5,a_,dj,dk,ay,fu){if((gi===ha)&&(gj===g$)&&(gk===hb)&&(gl===hc
)&&(c3===cQ)&&(cQ===cD)&&(cD===c4))this.pe.apply(this,arguments);else this.pf.apply(
this,arguments);};d.pe=function(x,aC,av,bD,ac,gi,gk,c3,gj,hb,cQ,g$,gl,cD,ha,hc,c4
,cp,a5,a_,dj,dk,ay,fu){var a=this.ba;var ab=null;var src=null;var bg=gi;var bh=gk;
var aZ=gj;var a2=gl;var a7=cp[0];var a3=cp[1];var bo=cp[2];var bm=cp[3];var k=Math.
min(bg,aZ);var j=Math.min(bh,a2);var p=Math.max(bg,aZ);var n=Math.max(bh,a2);if(
x&&(av>=0)&&(av<x.NoOfFrames))ab=x.bT[av];if(aC&&(bD>=0)&&(bD<aC.NoOfFrames))src=
aC.bT[bD];if(aC.eX)return;if(!ab||(ab.aH.Format!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a4)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}a7=Math.max(a7,0);a3=Math.max(a3,0);bo=Math.min(bo,aC.FrameSize[0]);bm=
Math.min(bm,aC.FrameSize[1]);k=Math.max(ac[0],k,0);j=Math.max(ac[1],j,0);p=Math.
min(ac[2],p,x.FrameSize[0]);n=Math.min(ac[3],n,x.FrameSize[1]);if((p<=k)||(n<=j)
)return;if((j>bh)||(n<bh)||(j>a2)||(n<a2)){var bx=1/(a2-bh);var bd,aW;if(bx>0){bd=(
j-bh)*bx;aW=(a2-n)*bx;}else{bd=(n-bh)*bx;aW=(a2-j)*bx;}if(a5!==dk){var la=a5;if(
bd)a5=aQ(a5,dk,bd);if(aW)dk=aQ(dk,la,aW);}if(a_!==dj){var qs=a_;if(bd)a_=aQ(a_,dj
,bd);if(aW)dj=aQ(dj,qs,aW);}var q4=a3;a3=a3+(bm-a3)*bd;bm=bm+(q4-bm)*aW;if(j>bh)
bh=j;else if(n<bh)bh=n;if(j>a2)a2=j;else if(n<a2)a2=n;}if((k>bg)||(p<bg)||(k>aZ)||(
p<aZ)){var bx=1/(aZ-bg);var cc,aw;if(bx>0){cc=(k-bg)*bx;aw=(aZ-p)*bx;}else{cc=(p-
bg)*bx;aw=(aZ-k)*bx;}if(a5!==a_){var la=a5;if(cc)a5=aQ(a5,a_,cc);if(aw)a_=aQ(a_,
la,aw);}if(dk!==dj){var qt=dk;if(cc)dk=aQ(dk,dj,cc);if(aw)dj=aQ(dj,qt,aw);}var q3=
a7;a7=a7+(bo-a7)*cc;bo=bo+(q3-bo)*aw;if(k>bg)bg=k;else if(p<bg)bg=p;if(k>aZ)aZ=k;
else if(p<aZ)aZ=p;}a7=(a7+src.aL[0])*src.aH.dW;a3=(a3+src.aL[1])*src.aH.dV;bo=(bo+
src.aL[0])*src.aH.dW;bm=(bm+src.aL[1])*src.aH.dV;bg+=ab.aL[0];aZ+=ab.aL[0];bh+=ab.
aL[1];a2+=ab.aL[1];var gy=(((a5&a_&dj&dk)>>24)&0xFF)<255;var b=a.gQ;var aK=a.hM;
b[0]=bg;b[6]=aZ;b[1]=bh;b[7]=bh;b[2]=a7;b[8]=bo;b[3]=a3;b[9]=a3;b[4]=1;b[10]=1;aK[
5]=a5;aK[11]=a_;b[12]=bg;b[18]=aZ;b[13]=a2;b[19]=a2;b[14]=a7;b[20]=bo;b[15]=bm;b[
21]=bm;b[16]=1;b[22]=1;aK[17]=dk;aK[23]=dj;if(src.aH.Format===this.b9)dt(a,ab.aH
,gy?a.g0:a.gZ);else dt(a,ab.aH,a.f9);g1(a,src.aH,fu);if(a.bX&&!ay)a.disable(a.BLEND
);if(!a.bX&&ay)a.enable(a.BLEND);a.bX=ay;a.bufferSubData(a.ARRAY_BUFFER,0,a.cs);
a.drawArrays(a.TRIANGLE_STRIP,0,4);if(this.a4)ei(a);};d.pf=function(x,aC,av,bD,ac
,gi,gk,c3,gj,hb,cQ,g$,gl,cD,ha,hc,c4,cp,a5,a_,dj,dk,ay,fu){var a=this.ba;var ab=
null;var src=null;var bg=Math.min(gi,gj,g$,ha);var bh=Math.min(gk,hb,gl,hc);var aZ=
Math.max(gi,gj,g$,ha);var a2=Math.max(gk,hb,gl,hc);var a7=cp[0];var a3=cp[1];var
bo=cp[2];var bm=cp[3];var k=Math.round(bg);var j=Math.round(bh);var p=Math.round(
aZ);var n=Math.round(a2);var qv=k;var qx=j;var qw=p;var qy=n;if(x&&(av>=0)&&(av<
x.NoOfFrames))ab=x.bT[av];if(aC&&(bD>=0)&&(bD<aC.NoOfFrames))src=aC.bT[bD];if(aC.
eX)return;if(!ab||(ab.aH.Format!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!src){if(this.a4)console.log("EmWi error: Invalid source bitmap in the drawing operation."
);return;}a7=Math.max(a7,0);a3=Math.max(a3,0);bo=Math.min(bo,aC.FrameSize[0]);bm=
Math.min(bm,aC.FrameSize[1]);k=Math.max(ac[0],k,0);j=Math.max(ac[1],j,0);p=Math.
min(ac[2],p,x.FrameSize[0]);n=Math.min(ac[3],n,x.FrameSize[1]);if((p<=k)||(n<=j)
)return;if((c3<=0)&&(cQ<=0)&&(cD<=0)&&(c4<=0)){c3=-c3;cQ=-cQ;cD=-cD;c4=-c4;}if((
c3<0)||(cQ<0)||(cD<0)||(c4<0)){console.log("Emwi error: Not convex destination polygon to warp bitmap."
);return;}var j0=(c3!==cQ)||(cD!==c4)||(c3!==cD);var ex=(a5!==a_)||(dj!==dk)||(a5
!==dk)||(a_!==dj);var gy=ex||(((a5>>24)&0xFF)<255);if(j0){var eO=c3;if(cQ<eO)eO=
cQ;if(cD<eO)eO=cD;if(c4<eO)eO=c4;c3=eO/c3;cQ=eO/cQ;cD=eO/cD;c4=eO/c4;}else{c3=1;
cQ=1;cD=1;c4=1;}a7=(a7+src.aL[0])*src.aH.dW;a3=(a3+src.aL[1])*src.aH.dV;bo=(bo+src.
aL[0])*src.aH.dW;bm=(bm+src.aL[1])*src.aH.dV;var b=a.gQ;var aK=a.hM;var cG=ab.aL[
0];var cH=ab.aL[1];var am=4;b[0]=gi+cG;b[6]=gj+cG;b[1]=gk+cH;b[7]=hb+cH;b[2]=a7*
c3;b[8]=bo*cQ;b[3]=a3*c3;b[9]=a3*cQ;b[4]=c3;b[10]=cQ;aK[5]=a5;aK[11]=a_;b[18]=ha+
cG;b[12]=g$+cG;b[19]=hc+cH;b[13]=gl+cH;b[20]=a7*c4;b[14]=bo*cD;b[21]=bm*c4;b[15]=
bm*cD;b[22]=c4;b[16]=cD;aK[23]=dk;aK[17]=dj;if((k>qv)||(j>qx)||(p<qw)||(n<qy)){am=
iU(b,aK,am,false,k+cG);am=iU(b,aK,am,false,p+cG);am=lP(b,am,k+cG,p+cG);am=iT(b,aK
,am,false,j+cH);am=iT(b,aK,am,false,n+cH);am=lO(b,am,j+cH,n+cH);if(am<3)return;}
if((src.aH.Format===this.b9)&&!gy)dt(a,ab.aH,j0?a.i0:a.gZ);else if(src.aH.Format===
this.b9)dt(a,ab.aH,j0?a.i1:a.g0);else dt(a,ab.aH,j0?a.i2:a.f9);g1(a,src.aH,fu);if(
a.bX&&!ay)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(a.BLEND);a.bX=ay;switch(am){case
4:{b.set(b.subarray(12,24),18);b.set(b.subarray(24,30),12);break;}case 5:{b.set(
b.subarray(12,30),18);b.set(b.subarray(30,36),12);break;}case 6:{b.set(b.subarray(
12,36),36);b.set(b.subarray(54,60),12);b.set(b.subarray(36,42),18);b.set(b.subarray(
48,54),24);b.set(b.subarray(42,48),30);break;}case 7:{b.set(b.subarray(12,42),42
);b.set(b.subarray(66,72),12);b.set(b.subarray(42,48),18);b.set(b.subarray(60,66
),24);b.set(b.subarray(48,60),30);break;}case 8:{b.set(b.subarray(12,48),48);b.set(
b.subarray(78,84),12);b.set(b.subarray(48,54),18);b.set(b.subarray(72,80),24);b.
set(b.subarray(54,60),30);b.set(b.subarray(66,72),36);b.set(b.subarray(60,66),42
);break;}}a.bufferSubData(a.ARRAY_BUFFER,0,a.cs);a.drawArrays(a.TRIANGLE_STRIP,0
,am);if(this.a4)ei(a);};function lR(m,e3,kW,h5,aY,bk,bV,ij,gt,gd,gf,ge,gg,r,cC){
var ix=e3.length*6;var cs=new ArrayBuffer(ix*6*4);var b=new Float32Array(cs);var
aK=new Uint32Array(cs);var aU=h5[0]+r[0];var aE=h5[1]+r[1];var bz=r[2]-r[0];var bt=
r[3]-r[1];for(var h=0;h<e3.length;h++){var u=h*36;var w=e3[h];var c7=kW[h];var k=
aY+w.OriginX+c7;var j=bk+w.OriginY;var p=k+w.Width;var n=j+w.Height;var b3=w.lK;
var cz=w.lL;var b4=w.lM;var bN=w.lN;if((j<gf)&&(n!==j)){b4+=((gf-j)/(n-j))*(bN-b4
);j=gf;}if((n>gg)&&(n!==j)){bN+=((gg-n)/(n-j))*(bN-b4);n=gg;}if((k<gd)&&(k!==p)){
b3+=((gd-k)/(p-k))*(cz-b3);k=gd;}if((p>ge)&&(k!==p)){cz+=((ge-p)/(p-k))*(cz-b3);
p=ge;}if(k>p)k=p;if(j>n)j=n;if(bV===0){k+=aU;j+=aE;p+=aU;n+=aE;b[u+0]=k;b[u+6]=p;
b[u+1]=j;b[u+7]=j;b[u+12]=k;b[u+18]=k;b[u+13]=n;b[u+19]=n;b[u+24]=p;b[u+30]=p;b[
u+25]=j;b[u+31]=n;}else if(bV===90){var b5=aU+j;var b7=aE+bt-k;var b6=aU+n;var b8=
aE+bt-p;b[u+0]=b5;b[u+6]=b5;b[u+1]=b7;b[u+7]=b8;b[u+12]=b6;b[u+18]=b6;b[u+13]=b7;
b[u+19]=b7;b[u+24]=b5;b[u+30]=b6;b[u+25]=b8;b[u+31]=b8;}else if(bV===180){var b5=
aU+bz-k;var b7=aE+bt-j;var b6=aU+bz-p;var b8=aE+bt-n;b[u+0]=b5;b[u+6]=b6;b[u+1]=
b7;b[u+7]=b7;b[u+12]=b5;b[u+18]=b5;b[u+13]=b8;b[u+19]=b8;b[u+24]=b6;b[u+30]=b6;b[
u+25]=b7;b[u+31]=b8;}else if(bV===270){var b5=aU+bz-j;var b7=aE+k;var b6=aU+bz-n;
var b8=aE+p;b[u+0]=b5;b[u+6]=b5;b[u+1]=b7;b[u+7]=b8;b[u+12]=b6;b[u+18]=b6;b[u+13
]=b7;b[u+19]=b7;b[u+24]=b5;b[u+30]=b6;b[u+25]=b8;b[u+31]=b8;}b[u+2]=b3;b[u+8]=cz;
b[u+3]=b4;b[u+9]=b4;b[u+4]=1;b[u+10]=1;aK[u+5]=cC;aK[u+11]=cC;b[u+14]=b3;b[u+20]=
b3;b[u+15]=bN;b[u+21]=bN;b[u+16]=1;b[u+22]=1;aK[u+17]=cC;aK[u+23]=cC;b[u+26]=cz;
b[u+32]=cz;b[u+27]=b4;b[u+33]=bN;b[u+28]=1;b[u+34]=1;aK[u+29]=cC;aK[u+35]=cC;aY+=
w.Advance+c7;if(ij&&((w.dS===0x20)||(w.dS===0xA0))){gt+=ij;aY+=gt>>16;gt&=0xFFFF;
}}m.bufferData(m.ARRAY_BUFFER,cs,m.STREAM_DRAW);m.drawArrays(m.TRIANGLES,0,ix);m.
bufferData(m.ARRAY_BUFFER,m.cs,m.STREAM_DRAW);}function lS(m,e3,kW,h5,aY,bk,bV,ij
,gt,gd,gf,ge,gg,r,as,aq,ap,ar){var ix=e3.length*6;var cs=new ArrayBuffer(ix*6*4);
var b=new Float32Array(cs);var aK=new Uint32Array(cs);var aU=h5[0]+r[0];var aE=h5[
1]+r[1];var bz=r[2]-r[0];var bt=r[3]-r[1];var jT=1/bz;var jQ=1/bt;if(bV===90){var
g=as;as=ar;ar=ap;ap=aq;aq=g;jT=1/bt;jQ=1/bz;}else if(bV===180){var aV=as;var a9=
ar;as=ap;ap=aV;ar=aq;aq=a9;}else if(bV===270){var g=as;as=aq;aq=ap;ap=ar;ar=g;jT=
1/bt;jQ=1/bz;}var jo=(as&0xFF);var jp=((as>>8)&0xFF);var jq=((as>>16)&0xFF);var jr=((
as>>24)&0xFF);var js=(aq&0xFF);var jt=((aq>>8)&0xFF);var ju=((aq>>16)&0xFF);var jv=((
aq>>24)&0xFF);var mT=(ar&0xFF);var mU=((ar>>8)&0xFF);var mV=((ar>>16)&0xFF);var mW=((
ar>>24)&0xFF);var mX=(ap&0xFF);var mY=((ap>>8)&0xFF);var mZ=((ap>>16)&0xFF);var m0=((
ap>>24)&0xFF);for(var h=0;h<e3.length;h++){var u=h*36;var w=e3[h];var c7=kW[h];var
k=aY+w.OriginX+c7;var j=bk+w.OriginY;var p=k+w.Width;var n=j+w.Height;var b3=w.lK;
var cz=w.lL;var b4=w.lM;var bN=w.lN;if((j<gf)&&(n!==j)){b4+=((gf-j)/(n-j))*(bN-b4
);j=gf;}if((n>gg)&&(n!==j)){bN+=((gg-n)/(n-j))*(bN-b4);n=gg;}if((k<gd)&&(k!==p)){
b3+=((gd-k)/(p-k))*(cz-b3);k=gd;}if((p>ge)&&(k!==p)){cz+=((ge-p)/(p-k))*(cz-b3);
p=ge;}if(k>p)k=p;if(j>n)j=n;var fS=j*jQ;var fP=n*jQ;var fQ=k*jT;var fR=p*jT;var jA=
jo+(mT-jo)*fS;var jB=jp+(mU-jp)*fS;var jC=jq+(mV-jq)*fS;var jD=jr+(mW-jr)*fS;var
m5=js+(mX-js)*fS;var m6=jt+(mY-jt)*fS;var m7=ju+(mZ-ju)*fS;var m8=jv+(m0-jv)*fS;
var jw=jo+(mT-jo)*fP;var jx=jp+(mU-jp)*fP;var jy=jq+(mV-jq)*fP;var jz=jr+(mW-jr)
*fP;var m1=js+(mX-js)*fP;var m2=jt+(mY-jt)*fP;var m3=ju+(mZ-ju)*fP;var m4=jv+(m0-
jv)*fP;var qj=jA+(m5-jA)*fQ;var qk=jB+(m6-jB)*fQ;var ql=jC+(m7-jC)*fQ;var qm=jD+(
m8-jD)*fQ;var qn=jA+(m5-jA)*fR;var qo=jB+(m6-jB)*fR;var qp=jC+(m7-jC)*fR;var qq=
jD+(m8-jD)*fR;var qb=jw+(m1-jw)*fQ;var qc=jx+(m2-jx)*fQ;var qd=jy+(m3-jy)*fQ;var
qe=jz+(m4-jz)*fQ;var qf=jw+(m1-jw)*fR;var qg=jx+(m2-jx)*fR;var qh=jy+(m3-jy)*fR;
var qi=jz+(m4-jz)*fR;as=qj|(qk<<8)|(ql<<16)|(qm<<24);aq=qn|(qo<<8)|(qp<<16)|(qq<<
24);ap=qf|(qg<<8)|(qh<<16)|(qi<<24);ar=qb|(qc<<8)|(qd<<16)|(qe<<24);if(bV===0){k+=
aU;j+=aE;p+=aU;n+=aE;b[u+0]=k;b[u+6]=p;b[u+1]=j;b[u+7]=j;b[u+12]=k;b[u+18]=k;b[u+
13]=n;b[u+19]=n;b[u+24]=p;b[u+30]=p;b[u+25]=j;b[u+31]=n;}else if(bV===90){var b5=
aU+j;var b7=aE+bt-k;var b6=aU+n;var b8=aE+bt-p;b[u+0]=b5;b[u+6]=b5;b[u+1]=b7;b[u+
7]=b8;b[u+12]=b6;b[u+18]=b6;b[u+13]=b7;b[u+19]=b7;b[u+24]=b5;b[u+30]=b6;b[u+25]=
b8;b[u+31]=b8;}else if(bV===180){var b5=aU+bz-k;var b7=aE+bt-j;var b6=aU+bz-p;var
b8=aE+bt-n;b[u+0]=b5;b[u+6]=b6;b[u+1]=b7;b[u+7]=b7;b[u+12]=b5;b[u+18]=b5;b[u+13]=
b8;b[u+19]=b8;b[u+24]=b6;b[u+30]=b6;b[u+25]=b7;b[u+31]=b8;}else if(bV===270){var
b5=aU+bz-j;var b7=aE+k;var b6=aU+bz-n;var b8=aE+p;b[u+0]=b5;b[u+6]=b5;b[u+1]=b7;
b[u+7]=b8;b[u+12]=b6;b[u+18]=b6;b[u+13]=b7;b[u+19]=b7;b[u+24]=b5;b[u+30]=b6;b[u+
25]=b8;b[u+31]=b8;}b[u+2]=b3;b[u+8]=cz;b[u+3]=b4;b[u+9]=b4;b[u+4]=1;b[u+10]=1;aK[
u+5]=as;aK[u+11]=aq;b[u+14]=b3;b[u+20]=b3;b[u+15]=bN;b[u+21]=bN;b[u+16]=1;b[u+22
]=1;aK[u+17]=ar;aK[u+23]=ar;b[u+26]=cz;b[u+32]=cz;b[u+27]=b4;b[u+33]=bN;b[u+28]=
1;b[u+34]=1;aK[u+29]=aq;aK[u+35]=ap;aY+=w.Advance+c7;if(ij&&((w.dS===0x20)||(w.dS===
0xA0))){gt+=ij;aY+=gt>>16;gt&=0xFFFF;}}m.bufferData(m.ARRAY_BUFFER,cs,m.STREAM_DRAW
);m.drawArrays(m.TRIANGLES,0,ix);m.bufferData(m.ARRAY_BUFFER,m.cs,m.STREAM_DRAW);
}d.mf=function(x,z,l,af,ag,av,ac,r,dm,dv,bV,as,aq,ap,ar,ay){var a=this.ba;var ab=
null;var k=r[0];var j=r[1];var p=r[2];var n=r[3];var d9=-dm[0];var eU=-dm[1];var
dq=0;var c_=0x8000;var aa=0;var aR=1;var aN=0;var c7=0;if(x&&(av>=0)&&(av<x.NoOfFrames
))ab=x.bT[av];if(!ab||(ab.aH.Format!==this.b9)){console.log("EmWi error: Invalid destination bitmap in the drawing operation."
);return;}if(!z){if(this.a4)console.log("EmWi error: Invalid font in the drawing operation."
);return;}if(ag===-1)ag=l.length;if(af<0){ag+=af;af=0;}ag=Math.min(ag,l.length-af
);if((bV!==90)&&(bV!==180)&&(bV!==270))bV=0;k=Math.max(ac[0],k,0);j=Math.max(ac[
1],j,0);p=Math.min(ac[2],p,x.FrameSize[0]);n=Math.min(ac[3],n,x.FrameSize[1]);if(
bV===90){var g=j;j=k-r[0];k=r[3]-n;n=p-r[0];p=r[3]-g;}else if(bV===180){var aV=k;
var a9=j;k=r[2]-p;j=r[3]-n;p=r[2]-aV;n=r[3]-a9;}else if(bV===270){var g=k;k=j-r[
1];j=r[2]-p;p=n-r[1];n=r[2]-g;}else{k-=r[0];j-=r[1];p-=r[0];n-=r[1];}j=Math.max(
j,eU-z.Ascent);n=Math.min(n,eU+z.Descent+1);if((p<=k)||(n<=j)||(ag<=0))return;if(
dv>0)dv-=this.i7(z,l,af,ag);if(dv>0){var bB=af;var am=ag;var ez=0;for(;am;bB++,am--
){var f=l.charCodeAt(bB);if((f===32)||(f===0xA0))ez++;}if(ez)dq=((dv<<16)/ez)|0;
}while(ag&&(aa=l.charCodeAt(af))){if((aa===0x2026)&&!this.eG(z,aa)){aa=0x2E;aR=3;
}else aR=1;var w=this.dF(z,aa);if(!w){if(this.a4)console.log("EmWi error: Default glyph not found in the font."
);return;}if(!w.Advance&&!w.OriginX&&!w.Width){af++;ag--;continue;}var ht=c7=this.
bn(z,aN,aa);if(aR>1)ht+=(aR-1)*this.bn(z,aa,aa);if((d9+ht+((aR-1)*w.Advance)+w.OriginX+
w.Width)>k)break;if(dq&&((aa===0x20)||(aa===0xA0))){c_+=dq;d9+=c_>>16;c_&=0xFFFF;
}d9+=ht+aR*w.Advance;af++;ag--;aN=aa;}if(!ag)return;var ex=(as!==ar)||(aq!==ap)||(
as!==aq);dt(a,ab.aH,a.f9);g1(a,null,false);a.bindTexture(a.TEXTURE_2D,this.hU);if(
a.bX&&!ay)a.disable(a.BLEND);if(!a.bX&&ay)a.enable(a.BLEND);a.bX=ay;var aP=[];var
hu=[];var iI=d9;var iG=c_;var iH=dq;do{var w=this.mh(z,aa);if(!w){if(ex)lS(a,aP,
hu,ab.aL,iI,eU,bV,iH,iG,k,j,p,n,r,as,aq,ap,ar);else lR(a,aP,hu,ab.aL,iI,eU,bV,iH
,iG,k,j,p,n,r,as);for(var h=0;h<aP.length;h++)aP[h].i4--;aP=[];hu=[];iI=d9;iG=c_;
iH=dq;w=this.mh(z,aa);}if(!w){if(this.a4)console.log("EmWi error: Default glyph not found in the font."
);return;}if(!w.Advance&&!w.OriginX&&!w.Width){if(--ag>0)aa=l.charCodeAt(++af);else
aa=0;if((aa===0x2026)&&!this.eG(z,aa)){aa=0x2E;aR=3;}else aR=1;continue;}c7=this.
bn(z,aN,aa);if((d9+c7+w.OriginX)>=p)break;while(aR-->0){w.i4++;aP.push(w);hu.push(
c7);if(dq&&((aa===0x20)||(aa===0xA0))){c_+=dq;d9+=c_>>16;c_&=0xFFFF;}d9+=c7+w.Advance;
if(aR>0)c7=this.bn(z,aa,aa);aN=aa;}if(--ag>0)aa=l.charCodeAt(++af);else aa=0;if((
aa===0x2026)&&!this.eG(z,aa)){aa=0x2E;aR=3;}else aR=1;}while(ag>0);if(ex)lS(a,aP
,hu,ab.aL,iI,eU,bV,iH,iG,k,j,p,n,r,as,aq,ap,ar);else lR(a,aP,hu,ab.aL,iI,eU,bV,iH
,iG,k,j,p,n,r,as);for(var h=0;h<aP.length;h++)aP[h].i4--;if(this.a4)ei(a);};d.rD=
function(x,ah,a0,av,ac,r,dm,fy,fz,gq,gp,ay){if(!x||!ah||!a0||(av<0)||(av>=x.NoOfFrames
))return;ac=this.hV(ac,r);if((ac[2]<=ac[0])||(ac[3]<=ac[1]))return;if(fy<0)fy=0;
if(fy>255)fy=255;if(fz<0)fz=0;if(fz>255)fz=255;if(gp<0)gp=0;if(gp>255)gp=255;if(
gq<0)gq=0;if(gq>255)gq=255;var width=r[2]-r[0];var height=r[3]-r[1];var nr=1;var
jR=1;var nt=(fy!==gp)||(fz!==gq);var ns=(fy!==fz)||(gp!==gq);if(width&&ns)nr/=width;
if(height&&nt)jR/=height;var ex={l0:ns,l1:nt,oL:nr,re:jR,Width:width,Height:height
,ob:fy,oc:(gp-fy)*jR,od:fz,oe:(gq-fz)*jR};oq(this,x,av,ac,r,r[0]-dm[0],r[1]-dm[1
],ah,a0.Data,0,a0.Data.length,ay,ex);};d.pm=[0,0];d.pn=[0,0,0,0];d.g3="";d.ks=3.1415926535
/180;d.sH=180/3.1415926535;d.jd=new Array(129).join('0');d.g9=[];d.g7=null;d.gb=
0;d.me=0;d.c0=null;d.dG=null;d.fq=null;d.g8=null;d.l_=null;d.rk="";d.bM=null;d.pk=
0;d.dX=null;d.ba=null;d.f_=null;d.hX=false;d.hW=false;d.ej=false;d.g_=-1;d.dh=0;
d.eH=0;d.kv=null;d.i6=true;d.kq=false;d.a4=false;d.ga=1024;d.i9=1024;d.f$=64;d.hU=
null;d.fp=null;d.i8=null;d.g5=null;d.b9=0;d.kC=1;d.kx=0x00000000;d.hZ=0x00000001;
d.ky=0x00000002;d.jb=0x00000003;d.py=0x00000000;d.hY=0x00000100;d.kw=0x00000200;
d.ja=0x00000300;d.sy=0x00000000;d.sA=0x00000101;d.sB=0x00000202;d.sz=0x00000303;
d.kz=0x00000000;d.kA=0x00010000;d.kB=0x00020000;return d;})();

/* Embedded Wizard */