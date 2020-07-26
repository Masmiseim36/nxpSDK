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
);if(EmWiApp.ui)throw new Error("The unit file 'Views.js' included twice!");EmWiApp.
ui=(function(){var B=EmWiApp;var C={};
var AX=[0,0];var BI=[0,0,0,0];var Dz="\uFEFF";
C.T={B8:0xFFFFFFFF,B9:0xFFFFFFFF,B_:0xFFFFFFFF,BV:0xFFFFFFFF,C$:function(A_,aClip
,aOffset,A7,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A7=A7+1;if(A7<256){
var BE=this.BV;var BF=this.B_;var BC=this.B8;var BD=this.B9;BE=(BE&0x00FFFFFF)|((((
A7*((BE>>24)&0xFF))>>8)&0xFF)<<24);BF=(BF&0x00FFFFFF)|((((A7*((BF>>24)&0xFF))>>8
)&0xFF)<<24);BC=(BC&0x00FFFFFF)|((((A7*((BC>>24)&0xFF))>>8)&0xFF)<<24);BD=(BD&0x00FFFFFF
)|((((A7*((BD>>24)&0xFF))>>8)&0xFF)<<24);A_.JF(aClip,B.tz(this.H,aOffset),BE,BF,
BD,BC,aBlend);}else A_.JF(aClip,B.tz(this.H,aOffset),this.BV,this.B_,this.B9,this.
B8,aBlend);},MM:function(E){var A;if(E===this.B8)return;this.B8=E;if(!!this.I&&((
this.F&0x1)===0x1))this.I.AF(this.H);},MN:function(E){var A;if(E===this.B9)return;
this.B9=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},MP:function(E){var
A;if(E===this.B_)return;this.B_=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.
H);},MO:function(E){var A;if(E===this.BV)return;this.BV=E;if(!!this.I&&((this.F&
0x1)===0x1))this.I.AF(this.H);},Ay:function(E){var A;if((((E===this.BV)&&(E===this.
B_))&&(E===this.B8))&&(E===this.B9))return;this.BV=E;this.B_=E;this.B8=E;this.B9=
E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},_Init:function(aArg){B.Core.
Bq._Init.call(this,aArg);this.__proto__=C.T;},_className:"Views::Rectangle"};C.BU={
Width:1,B8:0xFFFFFFFF,B9:0xFFFFFFFF,B_:0xFFFFFFFF,BV:0xFFFFFFFF,C$:function(A_,aClip
,aOffset,A7,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A7=A7+1;if(A7<256){
var BE=this.BV;var BF=this.B_;var BC=this.B8;var BD=this.B9;BE=(BE&0x00FFFFFF)|((((
A7*((BE>>24)&0xFF))>>8)&0xFF)<<24);BF=(BF&0x00FFFFFF)|((((A7*((BF>>24)&0xFF))>>8
)&0xFF)<<24);BC=(BC&0x00FFFFFF)|((((A7*((BC>>24)&0xFF))>>8)&0xFF)<<24);BD=(BD&0x00FFFFFF
)|((((A7*((BD>>24)&0xFF))>>8)&0xFF)<<24);A_.KT(aClip,B.tz(this.H,aOffset),this.Width
,BE,BF,BD,BC,aBlend);}else A_.KT(aClip,B.tz(this.H,aOffset),this.Width,this.BV,this.
B_,this.B9,this.B8,aBlend);},K7:function(E){var A;if(E<0)E=0;if(E===this.Width)return;
this.Width=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},In:function(){
return this.BV;},Ay:function(E){var A;if((((E===this.BV)&&(E===this.B_))&&(E===this.
B8))&&(E===this.B9))return;this.BV=E;this.B_=E;this.B8=E;this.B9=E;if(!!this.I&&((
this.F&0x1)===0x1))this.I.AF(this.H);},_Init:function(aArg){B.Core.Bq._Init.call(
this,aArg);this.__proto__=C.BU;},_className:"Views::Border"};C.B1={timer:null,As:
null,ED:null,CS:-1,B8:0xFFFFFFFF,B9:0xFFFFFFFF,B_:0xFFFFFFFF,BV:0xFFFFFFFF,If:0x1F
,Hk:0,Ar:255,Bb:0,JL:B.qx,DD:false,Dh:true,C$:function(A_,aClip,aOffset,A7,aBlend
){var A;var BY=this.Hk;if(this.CS>=0)BY=this.CS;if(!this.As||(BY>=this.As.NoOfFrames
))return;this.As.Update();var BE=this.BV;var BF=this.B_;var BD=this.B9;var BC=this.
B8;var C3=(((A7+1)*this.Ar)>>8)+1;var Cy=this.If;var Ah=B.tz(this.H,aOffset);var
Jp=B.tw([Ah[2]-Ah[0],Ah[3]-Ah[1]],this.JL);aBlend=aBlend&&((this.F&0x2)===0x2);if(
C3<256){BE=(BE&0x00FFFFFF)|((((((BE>>24)&0xFF)*C3)>>8)&0xFF)<<24);BF=(BF&0x00FFFFFF
)|((((((BF>>24)&0xFF)*C3)>>8)&0xFF)<<24);BD=(BD&0x00FFFFFF)|((((((BD>>24)&0xFF)*
C3)>>8)&0xFF)<<24);BC=(BC&0x00FFFFFF)|((((((BC>>24)&0xFF)*C3)>>8)&0xFF)<<24);}if(((
this.JL[0]>0)&&(Jp[0]>0))&&!((Cy&0x5)===0x5)){var Cc=((this.As.FrameSize[0]/3)|0
)-Jp[0];if(((Cy&0x1)===0x1)){if(aClip[2]>Ah[2])aClip=B.t1(aClip,Ah[2]);if(Cc>0)Ah=
B.t1(Ah,Ah[2]+Cc);Cy=Cy|0x4;}else if(((Cy&0x4)===0x4)){if(aClip[0]<Ah[0])aClip=[
].concat(Ah[0],aClip.slice(1,4));if(Cc>0)Ah=[].concat(Ah[0]-Cc,Ah.slice(1,4));Cy=
Cy|0x1;}else{if(aClip[0]<Ah[0])aClip=[].concat(Ah[0],aClip.slice(1,4));if(aClip[
2]>Ah[2])aClip=B.t1(aClip,Ah[2]);if(Cc>0){Ah=[].concat(Ah[0]-((Cc/2)|0),Ah.slice(
1,4));Ah=B.t1(Ah,(Ah[2]+Cc)-((Cc/2)|0));}Cy=Cy|0x5;}}if(((this.JL[1]>0)&&(Jp[1]>
0))&&!((Cy&0xA)===0xA)){var Cc=((this.As.FrameSize[1]/3)|0)-Jp[1];if(((Cy&0x2)===
0x2)){if(aClip[3]>Ah[3])aClip=[].concat(aClip.slice(0,3),Ah[3]);if(Cc>0)Ah=[].concat(
Ah.slice(0,3),Ah[3]+Cc);Cy=Cy|0x8;}else if(((Cy&0x8)===0x8)){if(aClip[1]<Ah[1])aClip=
B.t3(aClip,Ah[1]);if(Cc>0)Ah=B.t3(Ah,Ah[1]-Cc);Cy=Cy|0x2;}else{if(aClip[1]<Ah[1]
)aClip=B.t3(aClip,Ah[1]);if(aClip[3]>Ah[3])aClip=[].concat(aClip.slice(0,3),Ah[3
]);if(Cc>0){Ah=B.t3(Ah,Ah[1]-((Cc/2)|0));Ah=[].concat(Ah.slice(0,3),(Ah[3]+Cc)-((
Cc/2)|0));}Cy=Cy|0xA;}}A_.Mh(aClip,this.As,BY,Ah,Cy,BE,BF,BD,BC,aBlend);},HX:function(
Ai){var A;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},Dg:function(Ai){
var A;var BY=this.CS;var Dr=0;if(!!this.As)Dr=this.As.NoOfFrames*this.As.FrameDelay;
if((!!this.timer&&(this.CS<0))&&(Dr>0))this.Bb=this.timer.AJ-(this.Hk*this.As.FrameDelay
);if(!!this.timer&&(Dr>0)){var Av=(this.timer.AJ-this.Bb)|0;BY=(Av/this.As.FrameDelay
)|0;if(Av>=Dr){BY=BY%this.As.NoOfFrames;this.Bb=this.timer.AJ-(Av%Dr);if(!this.Dh
){BY=this.As.NoOfFrames-1;Dr=0;}}}if(((BY!==this.CS)&&!!this.I)&&((this.F&0x1)===
0x1))this.I.AF(this.H);this.CS=BY;if(!Dr&&!!this.timer){B.sM([this,this.Dg],this.
timer,0);this.timer=null;(A=this.ED)?A[1].call(A[0],this):null;}},Ay:function(E){
var A;if((((E===this.BV)&&(E===this.B_))&&(E===this.B8))&&(E===this.B9))return;this.
BV=E;this.B_=E;this.B8=E;this.B9=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.
H);},Hu:function(E){var A;if(this.DD===E)return;this.DD=E;this.CS=-1;if(!E&&!!this.
timer){B.sM([this,this.Dg],this.timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(
B.uj.GG);B.sz([this,this.Dg],this.timer,0);B.lq([this,this.Dg],this);}if(!!this.
I&&((this.F&0x1)===0x1))this.I.AF(this.H);},K1:function(E){var A;if(E===this.If)
return;this.If=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},DF:function(
E){var A;if(E===this.As)return;if(!!this.As&&this.As.GL)B.sM([this,this.HX],this.
As,0);this.As=E;this.CS=-1;if(!!E&&E.GL)B.sz([this,this.HX],E,0);if(this.DD){this.
Hu(false);this.Hu(true);}if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},_Init:
function(aArg){B.Core.Bq._Init.call(this,aArg);this.__proto__=C.B1;},_Mark:function(
D){var A;B.Core.Bq._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.As)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.ED)&&((A=
A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Frame"};C.BW={timer:null
,As:null,ED:null,Bb:0,CS:0,B8:0xFFFFFFFF,B9:0xFFFFFFFF,B_:0xFFFFFFFF,BV:0xFFFFFFFF
,AD:B.qx,FK:0x12,Hk:0,Ar:255,EW:0,DD:false,Dh:true,GC:false,Iu:true,C$:function(
A_,aClip,aOffset,A7,aBlend){var A;var BY=this.Hk;if(this.CS>=0)BY=this.CS;if(!this.
As||(BY>=this.As.NoOfFrames))return;this.As.Update();var W=this.GH();var BT=this.
As.FrameSize;var orient=this.EW;if((W[0]>=W[2])||(W[1]>=W[3]))return;var BE=this.
BV;var BF=this.B_;var BD=this.B9;var BC=this.B8;var C3=(((A7+1)*this.Ar)>>8)+1;aBlend=
aBlend&&((this.F&0x2)===0x2);if(C3<256){BE=(BE&0x00FFFFFF)|((((((BE>>24)&0xFF)*C3
)>>8)&0xFF)<<24);BF=(BF&0x00FFFFFF)|((((((BF>>24)&0xFF)*C3)>>8)&0xFF)<<24);BD=(BD&
0x00FFFFFF)|((((((BD>>24)&0xFF)*C3)>>8)&0xFF)<<24);BC=(BC&0x00FFFFFF)|((((((BC>>
24)&0xFF)*C3)>>8)&0xFF)<<24);}if(B.tl([W[2]-W[0],W[3]-W[1]],BT)&&!orient)A_.KR(aClip
,this.As,BY,B.tz(this.H,aOffset),B.tw(this.H.slice(0,2),W.slice(0,2)),BE,BF,BD,BC
,aBlend);else if(!orient)A_.MZ(aClip,this.As,BY,B.tz(W,aOffset),[].concat(AX,BT)
,BE,BF,BD,BC,aBlend,this.Iu);else{var left=W[0]+aOffset[0];var top=W[1]+aOffset[
1];var right=W[2]+aOffset[0];var bottom=W[3]+aOffset[1];if(orient===1)A_.J6(aClip
,this.As,BY,left,bottom,1.000000,left,top,1.000000,right,top,1.000000,right,bottom
,1.000000,[].concat(AX,BT),BC,BE,BF,BD,aBlend,this.Iu);else if(orient===2)A_.J6(
aClip,this.As,BY,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000,right
,top,1.000000,[].concat(AX,BT),BD,BC,BE,BF,aBlend,this.Iu);else if(orient===3)A_.
J6(aClip,this.As,BY,right,top,1.000000,right,bottom,1.000000,left,bottom,1.000000
,left,top,1.000000,[].concat(AX,BT),BF,BD,BC,BE,aBlend,this.Iu);}},HX:function(Ai
){var A;if(((this.GC&&!!this.As)&&(this.As.FrameSize[0]>0))&&(this.As.FrameSize[
1]>0))this.M(B.ty(this.GH(),this.AD));if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(
this.H);},Dg:function(Ai){var A;var BY=this.CS;var Dr=0;if(!!this.As)Dr=this.As.
NoOfFrames*this.As.FrameDelay;if((!!this.timer&&(this.CS<0))&&(Dr>0))this.Bb=this.
timer.AJ-(this.Hk*this.As.FrameDelay);if(!!this.timer&&(Dr>0)){var Av=(this.timer.
AJ-this.Bb)|0;BY=(Av/this.As.FrameDelay)|0;if(Av>=Dr){BY=BY%this.As.NoOfFrames;this.
Bb=this.timer.AJ-(Av%Dr);if(!this.Dh){BY=this.As.NoOfFrames-1;Dr=0;}}}if(((BY!==
this.CS)&&!!this.I)&&((this.F&0x1)===0x1))this.I.AF(this.H);this.CS=BY;if(!Dr&&!
!this.timer){B.sM([this,this.Dg],this.timer,0);this.timer=null;(A=this.ED)?A[1].
call(A[0],this):null;}},Ay:function(E){var A;if((((E===this.BV)&&(E===this.B_))&&(
E===this.B8))&&(E===this.B9))return;this.BV=E;this.B_=E;this.B8=E;this.B9=E;if(!
!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);},Hu:function(E){var A;if(this.DD===
E)return;this.DD=E;this.CS=-1;if(!E&&!!this.timer){B.sM([this,this.Dg],this.timer
,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(B.uj.GG);B.sz([this,this.
Dg],this.timer,0);B.lq([this,this.Dg],this);}if(!!this.I&&((this.F&0x1)===0x1))this.
I.AF(this.H);},DF:function(E){var A;if(E===this.As)return;if(!!this.As&&this.As.
GL)B.sM([this,this.HX],this.As,0);this.As=E;this.CS=-1;if(!!E&&E.GL)B.sz([this,this.
HX],E,0);if(this.DD){this.Hu(false);this.Hu(true);}if(((this.GC&&!!E)&&(E.FrameSize[
0]>0))&&(E.FrameSize[1]>0))this.M(B.ty(this.GH(),this.AD));if(!!this.I&&((this.F&
0x1)===0x1))this.I.AF(this.H);},A4:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(
E===this.Ar)return;this.Ar=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);
},GH:function(){var A;if(!this.As)return BI;var orient=this.EW;var A1=this.FK;var
BT=this.As.FrameSize;var AC=this.H;var width=AC[2]-AC[0];var height=AC[3]-AC[1];
if(!BT[0]||!BT[1])return BI;if((orient===1)||(orient===3)){width=height;height=AC[
2]-AC[0];}var AN=[0,0,width,height];var X=AN;if(((A1&0x40)===0x40)){var KL=((((AN[
2]-AN[0])<<16)+((BT[0]/2)|0))/BT[0])|0;var H7=((((AN[3]-AN[1])<<16)+((BT[1]/2)|0
))/BT[1])|0;var Fg=KL;if(H7>Fg)Fg=H7;X=B.tZ(X,((BT[0]*Fg)+32768)>>16);X=B.tW(X,((
BT[1]*Fg)+32768)>>16);}else if(((A1&0x80)===0x80)){var KL=((((AN[2]-AN[0])<<16)+((
BT[0]/2)|0))/BT[0])|0;var H7=((((AN[3]-AN[1])<<16)+((BT[1]/2)|0))/BT[1])|0;var Fg=
KL;if(H7<Fg)Fg=H7;X=B.tZ(X,((BT[0]*Fg)+32768)>>16);X=B.tW(X,((BT[1]*Fg)+32768)>>
16);}else if(!((A1&0x100)===0x100))X=B.tY(X,BT);if((X[2]-X[0])!==(AN[2]-AN[0])){
if(((A1&0x4)===0x4))X=B.t0(X,AN[2]-(X[2]-X[0]));else if(((A1&0x2)===0x2))X=B.t0(
X,(AN[0]+(((AN[2]-AN[0])/2)|0))-(((X[2]-X[0])/2)|0));}if((X[3]-X[1])!==(AN[3]-AN[
1])){if(((A1&0x20)===0x20))X=B.t2(X,AN[3]-(X[3]-X[1]));else if(((A1&0x10)===0x10
))X=B.t2(X,(AN[1]+(((AN[3]-AN[1])/2)|0))-(((X[3]-X[1])/2)|0));}if(!orient)X=B.tz(
X,AC.slice(0,2));else if(orient===1){var C4=[AC[0]+X[1],AC[3]-X[2]];X=[].concat(
C4,B.tx(C4,[X[3]-X[1],X[2]-X[0]]));}else if(orient===2){var C4=[AC[2]-X[2],AC[3]-
X[3]];X=[].concat(C4,B.tx(C4,[X[2]-X[0],X[3]-X[1]]));}else if(orient===3){var C4=[
AC[2]-X[3],AC[1]+X[0]];X=[].concat(C4,B.tx(C4,[X[3]-X[1],X[2]-X[0]]));}return B.
tz(X,this.AD);},Cr:function(){return this.Ar;},_Init:function(aArg){B.Core.Bq._Init.
call(this,aArg);this.__proto__=C.BW;},_Mark:function(D){var A;B.Core.Bq._Mark.call(
this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.As)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.ED)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Views::Image"};C.Text={Bl:null,BQ:null,Bs:B.hm,String:B.hm,Do:null
,DX:B.qx,Hx:0,FV:0,B8:0xFFFFFFFF,B9:0xFFFFFFFF,B_:0xFFFFFFFF,BV:0xFFFFFFFF,Ga:0,
AD:B.qx,FK:0x12,Ar:255,EW:0,G0:false,GC:false,JD:false,Mi:false,Ds:false,C$:function(
A_,aClip,aOffset,A7,aBlend){var A;if((this.Bs===B.hm)||!this.Bl)return;var A1=this.
FK;var orient=this.EW;var font=this.Bl;var AN=B.tz(this.H,aOffset);var BE=this.BV;
var BF=this.B_;var BD=this.B9;var BC=this.B8;var C3=(((A7+1)*this.Ar)>>8)+1;var Ev=
this.Bs.charCodeAt(0)||0;var W=B.tz(this.GH(),aOffset);var A9=[AN[0]-W[0],(AN[1]-
W[1])-font.Ascent];if(Ev<1)return;if(C3<256){BE=(BE&0x00FFFFFF)|((((((BE>>24)&0xFF
)*C3)>>8)&0xFF)<<24);BF=(BF&0x00FFFFFF)|((((((BF>>24)&0xFF)*C3)>>8)&0xFF)<<24);BD=(
BD&0x00FFFFFF)|((((((BD>>24)&0xFF)*C3)>>8)&0xFF)<<24);BC=(BC&0x00FFFFFF)|((((((BC>>
24)&0xFF)*C3)>>8)&0xFF)<<24);}if(((A1&0x80)===0x80)){if(this.JI())A1=(A1&~0x80)|
0x4;else A1=(A1&~0x80)|0x1;}if(((Ev===1)&&!((A1&0x40)===0x40))&&!orient){A_.KU(aClip
,font,this.Bs,2,(this.Bs.charCodeAt(1)||0)-1,AN,A9,0,orient,BE,BF,BD,BC,true);return;
}var leading=font.Leading;if(this.FV>0)leading=(this.FV-font.Ascent)-font.Descent;
var KK=(font.Ascent+font.Descent)+leading;var IZ=aClip[1]-W[1];var I0=aClip[3]-W[
1];var HK=W[2]-W[0];var GB=0;var K=1;var CU=this.Bs.charCodeAt(K)||0;if(orient===
1){A9=[W[3]-AN[3],(AN[0]-W[0])-font.Ascent];IZ=aClip[0]-W[0];I0=aClip[2]-W[0];HK=
W[3]-W[1];}else if(orient===2){A9=[W[2]-AN[2],(W[3]-AN[3])-font.Ascent];IZ=W[3]-
aClip[3];I0=W[3]-aClip[1];}else if(orient===3){A9=[AN[1]-W[1],(W[2]-AN[2])-font.
Ascent];IZ=W[2]-aClip[2];I0=W[2]-aClip[0];HK=W[3]-W[1];}while(((GB+KK)<IZ)&&(CU>
0)){K=K+CU;GB=GB+KK;CU=this.Bs.charCodeAt(K)||0;}while((GB<I0)&&(CU>0)){var Gu=B.
tw(A9,[0,GB]);var L0=0;var I9=false;if(((((A1&0x40)===0x40)&&((this.Bs.charCodeAt((
K+CU)-1)||0)!==0x0A))&&((this.Bs.charCodeAt(K+1)||0)!==0x0A))&&((this.Bs.charCodeAt(
K+CU)||0)!==0x00))I9=true;if(I9&&!!(A1&0x6)){var LZ=K+CU;var LC=this.Bs.indexOf(
String.fromCharCode(0x20),K+1);var LD=this.Bs.indexOf(String.fromCharCode(0xA0),
K+1);if(((LC<0)||(LC>=LZ))&&((LD<0)||(LD>=LZ)))I9=false;}if(I9)L0=HK;else if(((A1&
0x4)===0x4))Gu=[(Gu[0]-HK)+font.Hm(this.Bs,K+1,CU-1),Gu[1]];else if(((A1&0x2)===
0x2))Gu=[(Gu[0]-((HK/2)|0))+((font.Hm(this.Bs,K+1,CU-1)/2)|0),Gu[1]];A_.KU(aClip
,font,this.Bs,K+1,CU-1,AN,Gu,L0,orient,BE,BF,BD,BC,true);K=K+CU;GB=GB+KK;CU=this.
Bs.charCodeAt(K)||0;}},M:function(E){var A;if(B.tm(E,this.H))return;var KJ=false;
if(!this.EW||(this.EW===2))KJ=((A=this.H)[2]-A[0])!==(E[2]-E[0]);else KJ=((A=this.
H)[3]-A[1])!==(E[3]-E[1]);if((((KJ&&!this.Ga)&&this.G0)&&this.Ds)&&!((this.F&0x2000
)===0x2000)){this.Bs=B.hm;this.Ds=false;B.lq([this,this.G_],this);}if(((this.JD&&
this.Ds)&&!B.tl([(A=this.H)[2]-A[0],A[3]-A[1]],[E[2]-E[0],E[3]-E[1]]))&&!((this.
F&0x2000)===0x2000)){this.Bs=B.hm;this.Ds=false;B.lq([this,this.G_],this);}B.Core.
Bq.M.call(this,E);B.lq([this,this.KG],this);},Ee:function(){if(!!this.Do){this.Kp(
this.Do);this.Do=null;}},Kp:function(aBidi){if(!aBidi)return;B.ng(aBidi);},Op:function(
aSize){var bidi=null;bidi=B.qk(aSize);return bidi;},KG:function(Ai){B.lq(this.BQ
,this);},G_:function(Ai){B.lq([this,this.Jh],this);},Jh:function(Ai){var A;if(this.
Ds)return;var orient=this.EW;var width=(A=this.H)[2]-A[0];var height=(A=this.H)[
3]-A[1];var DS=-1;if((orient===1)||(orient===3)){width=height;height=(A=this.H)[
2]-A[0];}if(this.G0){if(this.Ga>0)DS=this.Ga;else if(!this.GC)DS=width-(this.Hx*
2);else DS=width;if(DS<0)DS=1;}if(!!this.Do){this.Kp(this.Do);this.Do=null;}this.
Ds=true;if((this.String!==B.hm)&&!!this.Bl){var length=this.String.length;if(this.
Mi)this.Do=this.Op(length);this.Bs=this.Bl.MW(this.String,0,DS,length,this.Do);if(
!!this.Do&&!this.Mm()){this.Kp(this.Do);this.Do=null;}}else this.Bs=B.hm;this.DX=
AX;if(((this.JD&&(this.Bs!==B.hm))&&!this.GC)&&!!this.Bl){var A1=this.FK;var font=
this.Bl;var leading=font.Leading;var Bt=this.Bs;var Jn=this.JI();if(((A1&0x80)===
0x80)){if(Jn)A1=(A1&~0x80)|0x4;else A1=(A1&~0x80)|0x1;}if(this.FV>0)leading=(this.
FV-font.Ascent)-font.Descent;var H4=(font.Ascent+font.Descent)+leading;var Ev=Bt.
charCodeAt(0)||0;var Fz=((height+leading)/H4)|0;var Kn=false;var IY=false;if(Fz<=
0)Fz=1;if(Ev>Fz){var Ed=0;var H5=0;var Jm=Ev-1;var B7=0;var Ct=Bt.length;var tmp=
B.hm;if(((A1&0x20)===0x20))H5=Ev-Fz;else if(((A1&0x10)===0x10)){H5=((Ev-Fz)/2)|0;
Jm=(H5+Fz)-1;}else Jm=Fz-1;Kn=H5>0;IY=Jm<(Ev-1);for(B7=1;Ed<H5;Ed=Ed+1)B7=B7+(Bt.
charCodeAt(B7)||0);if(IY)for(Ct=B7;Ed<Jm;Ed=Ed+1)Ct=Ct+(Bt.charCodeAt(Ct)||0);if(
Kn){var CK=Bt.charCodeAt(B7)||0;tmp=(Dz+B.t9(Bt,B7,CK))+Dz;tmp=B.t4(tmp,0,(CK+2)&
0xFFFF);B7=B7+CK;if((tmp.charCodeAt(CK)||0)===0x0A){tmp=B.t4(tmp,CK,0xFEFF);tmp=
B.t4(tmp,CK+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.t4(tmp,2,0xFEFF);tmp=
B.t4(tmp,1,0x0A);}else tmp=B.t4(tmp,1,0xFEFF);}tmp=tmp+B.t9(Bt,B7,Ct-B7);if(IY&&(
Ct>=B7)){var CK=Bt.charCodeAt(Ct)||0;var C8=(Dz+B.t9(Bt,Ct,CK))+Dz;C8=B.t4(C8,0,(
CK+2)&0xFFFF);C8=B.t4(C8,1,0xFEFF);if((C8.charCodeAt(CK)||0)===0x0A){C8=B.t4(C8,
CK,0xFEFF);C8=B.t4(C8,CK+1,0x0A);}if((C8.charCodeAt(2)||0)===0x0A){C8=B.t4(C8,2,
0xFEFF);C8=B.t4(C8,1,0x0A);}else C8=B.t4(C8,1,0xFEFF);tmp=tmp+C8;}Bt=String.fromCharCode(
Fz&0xFFFF)+tmp;}var Ed=0;var CJ=1;var Jb=width-(this.Hx*2);if(this.G0&&(this.Ga>
0))Jb=this.Ga;Ev=Bt.charCodeAt(0)||0;for(;Ed<Ev;Ed=Ed+1){var FD=Kn&&!Ed;var FE=IY&&(
Ed===(Ev-1));var DN=false;var DO=false;var Hb=Jn;if((Jn&&FD)&&!FE){FD=false;FE=true;
}else if((Jn&&FE)&&!FD){FE=false;FD=true;}var H8=CJ+1;var CK=Bt.charCodeAt(CJ)||
0;var B7=H8;var Ct=(H8+CK)-2;var Kx=-1;var Ky=-1;if(!this.G0&&(font.Hm(Bt,H8,CK-
1)>Jb)){if(((A1&0x4)===0x4))DN=true;else if(((A1&0x2)===0x2)){DN=true;DO=true;}else
DO=true;}if((Bt.charCodeAt(B7)||0)===0x0A)B7=B7+1;if((Bt.charCodeAt(Ct)||0)===0x0A
)Ct=Ct-1;while(DN&&((Bt.charCodeAt(B7)||0)===0xFEFF))B7=B7+1;while(DO&&((Bt.charCodeAt(
Ct)||0)===0xFEFF))Ct=Ct-1;DN=DN&&!FE;DO=DO&&!FD;while((((DN||DO)||FD)||FE)&&(B7<
Ct)){if((DN&&(Hb||!DO))||FD){if(Kx>0)Bt=B.t4(Bt,Kx,0xFEFF);Bt=B.t4(Bt,B7,0x2026);
Kx=B7;B7=B7+1;Hb=!Hb;FD=false;if(font.Hm(Bt,H8,CK-1)<=Jb){DN=false;DO=false;}else
DN=DN||!DO;}if((DO&&(!Hb||!DN))||FE){if(Ky>0)Bt=B.t4(Bt,Ky,0xFEFF);Bt=B.t4(Bt,Ct
,0x2026);Ky=Ct;Ct=Ct-1;Hb=!Hb;FE=false;if(font.Hm(Bt,H8,CK-1)<=Jb){DN=false;DO=false;
}else DO=DO||!DN;}}CJ=CJ+CK;}this.DX=[font.KX(Bt),((Bt.charCodeAt(0)||0)*H4)-leading
];this.Bs=Bt;}if(this.GC&&(this.Bs!==B.hm)){var Fe=[this.Hx,0];if((orient===1)||(
orient===3)){Fe=[Fe[0],Fe[0]];Fe=[0,Fe[1]];}this.F=this.F|0x2000;this.M(B.ty(B.th(
this.GH(),Fe),this.AD));this.F=this.F&~0x2000;}if(!!this.I&&((this.F&0x1)===0x1)
)this.I.AF(this.H);B.lq([this,this.KG],this);},Ek:function(E){if(E===this.G0)return;
this.G0=E;if(this.Ds){this.Bs=B.hm;this.Ds=false;B.lq([this,this.G_],this);}if(E&&
!this.Ga)this.F=this.F&~0x100;else this.F=this.F|0x100;},Cf:function(E){var A;if(
E===this.FK)return;this.FK=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AF(this.H);
if(this.JD){this.Bs=B.hm;this.Ds=false;B.lq([this,this.G_],this);}if(this.Ds)B.lq([
this,this.KG],this);},A6:function(E){if(E===this.String)return;this.String=E;this.
Bs=B.hm;this.Ds=false;B.lq([this,this.G_],this);},B4:function(E){if(E===this.Bl)
return;this.Bl=E;this.Bs=B.hm;this.Ds=false;B.lq([this,this.G_],this);},In:function(
){return this.BV;},Ay:function(E){var A;if((((E===this.BV)&&(E===this.B_))&&(E===
this.B8))&&(E===this.B9))return;this.BV=E;this.B_=E;this.B8=E;this.B9=E;if(!!this.
I&&((this.F&0x1)===0x1))this.I.AF(this.H);},A4:function(E){var A;if(E>255)E=255;
if(E<0)E=0;if(E===this.Ar)return;this.Ar=E;if(!!this.I&&((this.F&0x1)===0x1))this.
I.AF(this.H);},JI:function(){if(!this.Ds)this.Jh(this);if(!this.Do)return false;
var result=false;var bidi=this.Do;result=B.qj(bidi);return result;},Mm:function(
){if(!this.Ds)this.Jh(this);if(!this.Do)return false;var result=false;var bidi=this.
Do;result=B.sD(bidi);return result;},GH:function(){var A;if((this.String===B.hm)||
!this.Bl)return BI;if(!this.Ds)this.Jh(this);if(this.Bs===B.hm)return BI;var leading=
this.Bl.Leading;var H4=(this.Bl.Ascent+this.Bl.Descent)+this.Bl.Leading;if(this.
FV>0){leading=(this.FV-this.Bl.Ascent)-this.Bl.Descent;H4=this.FV;}if(B.tl(this.
DX,AX))this.DX=[this.Bl.KX(this.Bs),this.DX[1]];this.DX=[this.DX[0],((this.Bs.charCodeAt(
0)||0)*H4)-leading];var A1=this.FK;var orient=this.EW;var AC=this.H;var Fe=this.
Hx;var width=AC[2]-AC[0];var height=AC[3]-AC[1];if((orient===1)||(orient===3)){width=
height;height=AC[2]-AC[0];}var AN=[Fe,0,width-Fe,height];var X=[].concat(AN.slice(
0,2),B.tx(AN.slice(0,2),this.DX));if(((A1&0x80)===0x80)){if(this.JI())A1=(A1&~0x80
)|0x4;else A1=(A1&~0x80)|0x1;}if(((A1&0x40)===0x40)){var DS=this.Ga;if(DS<=0)DS=
width-(this.Hx*2);if(DS<0)DS=0;if(DS>(X[2]-X[0]))X=B.tZ(X,DS);}if((X[2]-X[0])!==(
AN[2]-AN[0])){if(((A1&0x4)===0x4))X=B.t0(X,AN[2]-(X[2]-X[0]));else if(((A1&0x2)===
0x2))X=B.t0(X,(AN[0]+(((AN[2]-AN[0])/2)|0))-(((X[2]-X[0])/2)|0));}if((X[3]-X[1])
!==(AN[3]-AN[1])){if(((A1&0x20)===0x20))X=B.t2(X,AN[3]-(X[3]-X[1]));else if(((A1&
0x10)===0x10))X=B.t2(X,(AN[1]+(((AN[3]-AN[1])/2)|0))-(((X[3]-X[1])/2)|0));}if(!orient
)X=B.tz(X,AC.slice(0,2));else if(orient===1){var C4=[AC[0]+X[1],AC[3]-X[2]];X=[].
concat(C4,B.tx(C4,[this.DX[1],this.DX[0]]));}else if(orient===2){var C4=[AC[2]-X[
2],AC[3]-X[3]];X=[].concat(C4,B.tx(C4,this.DX));}else if(orient===3){var C4=[AC[
2]-X[3],AC[1]+X[0]];X=[].concat(C4,B.tx(C4,[this.DX[1],this.DX[0]]));}return B.tz(
X,this.AD);},Cr:function(){return this.Ar;},_Init:function(aArg){B.Core.Bq._Init.
call(this,aArg);this.__proto__=C.Text;},_Done:function(){this.Ee();this.__proto__=
B.Core.Bq;B.Core.Bq._Done.call(this);},_Mark:function(D){var A;B.Core.Bq._Mark.call(
this,D);if((A=this.Bl)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.BQ)&&((A=A[
0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Text"};C.Ps={L8:0x1,L7:0x2
,L9:0x4,Ma:0x8,L$:0x10,L_:0x20,PV:0x40,PW:0x80,P1:0x100};C.P2={L8:0x1,L7:0x2,L9:
0x4,Ma:0x8,L$:0x10,L_:0x20,OU:0x40,OT:0x80};C.EW={PC:0,PT:1,PR:2,PS:3};
C._Init=function(){C.T.__proto__=B.Core.Bq;C.BU.__proto__=B.Core.Bq;C.B1.__proto__=
B.Core.Bq;C.BW.__proto__=B.Core.Bq;C.Text.__proto__=B.Core.Bq;};C.Dj=function(D){
};return C;})();

/* Embedded Wizard */