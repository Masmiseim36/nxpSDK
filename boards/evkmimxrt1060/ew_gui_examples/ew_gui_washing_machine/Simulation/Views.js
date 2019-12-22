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
);if(EmWiApp.te)throw new Error("The unit file 'Views.js' included twice!");EmWiApp.
te=(function(){var B=EmWiApp;var C={};
var AX=[0,0];var BM=[0,0,0,0];var Dx="\uFEFF";
C.U={B3:0xFFFFFFFF,B4:0xFFFFFFFF,B5:0xFFFFFFFF,BR:0xFFFFFFFF,C6:function(A_,aClip
,aOffset,A7,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A7=A7+1;if(A7<256){
var BA=this.BR;var BB=this.B5;var By=this.B3;var Bz=this.B4;BA=(BA&0x00FFFFFF)|((((
A7*((BA>>24)&0xFF))>>8)&0xFF)<<24);BB=(BB&0x00FFFFFF)|((((A7*((BB>>24)&0xFF))>>8
)&0xFF)<<24);By=(By&0x00FFFFFF)|((((A7*((By>>24)&0xFF))>>8)&0xFF)<<24);Bz=(Bz&0x00FFFFFF
)|((((A7*((Bz>>24)&0xFF))>>8)&0xFF)<<24);A_.Jl(aClip,B.sq(this.H,aOffset),BA,BB,
Bz,By,aBlend);}else A_.Jl(aClip,B.sq(this.H,aOffset),this.BR,this.B5,this.B4,this.
B3,aBlend);},Mv:function(E){var A;if(E===this.B3)return;this.B3=E;if(!!this.I&&((
this.F&0x1)===0x1))this.I.AE(this.H);},Mw:function(E){var A;if(E===this.B4)return;
this.B4=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},My:function(E){var
A;if(E===this.B5)return;this.B5=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.
H);},Mx:function(E){var A;if(E===this.BR)return;this.BR=E;if(!!this.I&&((this.F&
0x1)===0x1))this.I.AE(this.H);},Aw:function(E){var A;if((((E===this.BR)&&(E===this.
B5))&&(E===this.B3))&&(E===this.B4))return;this.BR=E;this.B5=E;this.B3=E;this.B4=
E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},_Init:function(aArg){B.Core.
BD._Init.call(this,aArg);this.__proto__=C.U;},_className:"Views::Rectangle"};C.BQ={
Width:1,B3:0xFFFFFFFF,B4:0xFFFFFFFF,B5:0xFFFFFFFF,BR:0xFFFFFFFF,C6:function(A_,aClip
,aOffset,A7,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A7=A7+1;if(A7<256){
var BA=this.BR;var BB=this.B5;var By=this.B3;var Bz=this.B4;BA=(BA&0x00FFFFFF)|((((
A7*((BA>>24)&0xFF))>>8)&0xFF)<<24);BB=(BB&0x00FFFFFF)|((((A7*((BB>>24)&0xFF))>>8
)&0xFF)<<24);By=(By&0x00FFFFFF)|((((A7*((By>>24)&0xFF))>>8)&0xFF)<<24);Bz=(Bz&0x00FFFFFF
)|((((A7*((Bz>>24)&0xFF))>>8)&0xFF)<<24);A_.KC(aClip,B.sq(this.H,aOffset),this.Width
,BA,BB,Bz,By,aBlend);}else A_.KC(aClip,B.sq(this.H,aOffset),this.Width,this.BR,this.
B5,this.B4,this.B3,aBlend);},KR:function(E){var A;if(E<0)E=0;if(E===this.Width)return;
this.Width=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},H_:function(){
return this.BR;},Aw:function(E){var A;if((((E===this.BR)&&(E===this.B5))&&(E===this.
B3))&&(E===this.B4))return;this.BR=E;this.B5=E;this.B3=E;this.B4=E;if(!!this.I&&((
this.F&0x1)===0x1))this.I.AE(this.H);},_Init:function(aArg){B.Core.BD._Init.call(
this,aArg);this.__proto__=C.BQ;},_className:"Views::Border"};C.BX={timer:null,Aq:
null,Es:null,CR:-1,B3:0xFFFFFFFF,B4:0xFFFFFFFF,B5:0xFFFFFFFF,BR:0xFFFFFFFF,H6:0x1F
,G6:0,Ap:255,Ba:0,Js:B.pm,DB:false,Dd:true,C6:function(A_,aClip,aOffset,A7,aBlend
){var A;var BU=this.G6;if(this.CR>=0)BU=this.CR;if(!this.Aq||(BU>=this.Aq.NoOfFrames
))return;this.Aq.Update();var BA=this.BR;var BB=this.B5;var Bz=this.B4;var By=this.
B3;var C0=(((A7+1)*this.Ap)>>8)+1;var Ct=this.H6;var Ai=B.sq(this.H,aOffset);var
I9=B.sn([Ai[2]-Ai[0],Ai[3]-Ai[1]],this.Js);aBlend=aBlend&&((this.F&0x2)===0x2);if(
C0<256){BA=(BA&0x00FFFFFF)|((((((BA>>24)&0xFF)*C0)>>8)&0xFF)<<24);BB=(BB&0x00FFFFFF
)|((((((BB>>24)&0xFF)*C0)>>8)&0xFF)<<24);Bz=(Bz&0x00FFFFFF)|((((((Bz>>24)&0xFF)*
C0)>>8)&0xFF)<<24);By=(By&0x00FFFFFF)|((((((By>>24)&0xFF)*C0)>>8)&0xFF)<<24);}if(((
this.Js[0]>0)&&(I9[0]>0))&&!((Ct&0x5)===0x5)){var B9=((this.Aq.FrameSize[0]/3)|0
)-I9[0];if(((Ct&0x1)===0x1)){if(aClip[2]>Ai[2])aClip=B.sW(aClip,Ai[2]);if(B9>0)Ai=
B.sW(Ai,Ai[2]+B9);Ct=Ct|0x4;}else if(((Ct&0x4)===0x4)){if(aClip[0]<Ai[0])aClip=[
].concat(Ai[0],aClip.slice(1,4));if(B9>0)Ai=[].concat(Ai[0]-B9,Ai.slice(1,4));Ct=
Ct|0x1;}else{if(aClip[0]<Ai[0])aClip=[].concat(Ai[0],aClip.slice(1,4));if(aClip[
2]>Ai[2])aClip=B.sW(aClip,Ai[2]);if(B9>0){Ai=[].concat(Ai[0]-((B9/2)|0),Ai.slice(
1,4));Ai=B.sW(Ai,(Ai[2]+B9)-((B9/2)|0));}Ct=Ct|0x5;}}if(((this.Js[1]>0)&&(I9[1]>
0))&&!((Ct&0xA)===0xA)){var B9=((this.Aq.FrameSize[1]/3)|0)-I9[1];if(((Ct&0x2)===
0x2)){if(aClip[3]>Ai[3])aClip=[].concat(aClip.slice(0,3),Ai[3]);if(B9>0)Ai=[].concat(
Ai.slice(0,3),Ai[3]+B9);Ct=Ct|0x8;}else if(((Ct&0x8)===0x8)){if(aClip[1]<Ai[1])aClip=
B.sY(aClip,Ai[1]);if(B9>0)Ai=B.sY(Ai,Ai[1]-B9);Ct=Ct|0x2;}else{if(aClip[1]<Ai[1]
)aClip=B.sY(aClip,Ai[1]);if(aClip[3]>Ai[3])aClip=[].concat(aClip.slice(0,3),Ai[3
]);if(B9>0){Ai=B.sY(Ai,Ai[1]-((B9/2)|0));Ai=[].concat(Ai.slice(0,3),(Ai[3]+B9)-((
B9/2)|0));}Ct=Ct|0xA;}}A_.L2(aClip,this.Aq,BU,Ai,Ct,BA,BB,Bz,By,aBlend);},HO:function(
Aj){var A;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},Dc:function(Aj){
var A;var BU=this.CR;var Do=0;if(!!this.Aq)Do=this.Aq.NoOfFrames*this.Aq.FrameDelay;
if((!!this.timer&&(this.CR<0))&&(Do>0))this.Ba=this.timer.AK-(this.G6*this.Aq.FrameDelay
);if(!!this.timer&&(Do>0)){var At=(this.timer.AK-this.Ba)|0;BU=(At/this.Aq.FrameDelay
)|0;if(At>=Do){BU=BU%this.Aq.NoOfFrames;this.Ba=this.timer.AK-(At%Do);if(!this.Dd
){BU=this.Aq.NoOfFrames-1;Do=0;}}}if(((BU!==this.CR)&&!!this.I)&&((this.F&0x1)===
0x1))this.I.AE(this.H);this.CR=BU;if(!Do&&!!this.timer){B.rA([this,this.Dc],this.
timer,0);this.timer=null;(A=this.Es)?A[1].call(A[0],this):null;}},Aw:function(E){
var A;if((((E===this.BR)&&(E===this.B5))&&(E===this.B3))&&(E===this.B4))return;this.
BR=E;this.B5=E;this.B3=E;this.B4=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.
H);},Hh:function(E){var A;if(this.DB===E)return;this.DB=E;this.CR=-1;if(!E&&!!this.
timer){B.rA([this,this.Dc],this.timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(
B.tf.Gz);B.rl([this,this.Dc],this.timer,0);B.kD([this,this.Dc],this);}if(!!this.
I&&((this.F&0x1)===0x1))this.I.AE(this.H);},KL:function(E){var A;if(E===this.H6)
return;this.H6=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},DD:function(
E){var A;if(E===this.Aq)return;if(!!this.Aq&&this.Aq.GC)B.rA([this,this.HO],this.
Aq,0);this.Aq=E;this.CR=-1;if(!!E&&E.GC)B.rl([this,this.HO],E,0);if(this.DB){this.
Hh(false);this.Hh(true);}if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},_Init:
function(aArg){B.Core.BD._Init.call(this,aArg);this.__proto__=C.BX;},_Mark:function(
D){var A;B.Core.BD._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A.
_cycle=D);if((A=this.Aq)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Es)&&((A=
A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Frame"};C.BS={timer:null
,Aq:null,Es:null,Ba:0,CR:0,B3:0xFFFFFFFF,B4:0xFFFFFFFF,B5:0xFFFFFFFF,BR:0xFFFFFFFF
,AB:B.pm,FE:0x12,G6:0,Ap:255,EN:0,DB:false,Dd:true,Gw:false,Id:true,C6:function(
A_,aClip,aOffset,A7,aBlend){var A;var BU=this.G6;if(this.CR>=0)BU=this.CR;if(!this.
Aq||(BU>=this.Aq.NoOfFrames))return;this.Aq.Update();var X=this.GA();var BP=this.
Aq.FrameSize;var orient=this.EN;if((X[0]>=X[2])||(X[1]>=X[3]))return;var BA=this.
BR;var BB=this.B5;var Bz=this.B4;var By=this.B3;var C0=(((A7+1)*this.Ap)>>8)+1;aBlend=
aBlend&&((this.F&0x2)===0x2);if(C0<256){BA=(BA&0x00FFFFFF)|((((((BA>>24)&0xFF)*C0
)>>8)&0xFF)<<24);BB=(BB&0x00FFFFFF)|((((((BB>>24)&0xFF)*C0)>>8)&0xFF)<<24);Bz=(Bz&
0x00FFFFFF)|((((((Bz>>24)&0xFF)*C0)>>8)&0xFF)<<24);By=(By&0x00FFFFFF)|((((((By>>
24)&0xFF)*C0)>>8)&0xFF)<<24);}if(B.sa([X[2]-X[0],X[3]-X[1]],BP)&&!orient)A_.Kz(aClip
,this.Aq,BU,B.sq(this.H,aOffset),B.sn(this.H.slice(0,2),X.slice(0,2)),BA,BB,Bz,By
,aBlend);else if(!orient)A_.MI(aClip,this.Aq,BU,B.sq(X,aOffset),[].concat(AX,BP)
,BA,BB,Bz,By,aBlend,this.Id);else{var left=X[0]+aOffset[0];var top=X[1]+aOffset[
1];var right=X[2]+aOffset[0];var bottom=X[3]+aOffset[1];if(orient===1)A_.JO(aClip
,this.Aq,BU,left,bottom,1.000000,left,top,1.000000,right,top,1.000000,right,bottom
,1.000000,[].concat(AX,BP),By,BA,BB,Bz,aBlend,this.Id);else if(orient===2)A_.JO(
aClip,this.Aq,BU,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000,right
,top,1.000000,[].concat(AX,BP),Bz,By,BA,BB,aBlend,this.Id);else if(orient===3)A_.
JO(aClip,this.Aq,BU,right,top,1.000000,right,bottom,1.000000,left,bottom,1.000000
,left,top,1.000000,[].concat(AX,BP),BB,Bz,By,BA,aBlend,this.Id);}},HO:function(Aj
){var A;if(((this.Gw&&!!this.Aq)&&(this.Aq.FrameSize[0]>0))&&(this.Aq.FrameSize[
1]>0))this.L(B.sp(this.GA(),this.AB));if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(
this.H);},Dc:function(Aj){var A;var BU=this.CR;var Do=0;if(!!this.Aq)Do=this.Aq.
NoOfFrames*this.Aq.FrameDelay;if((!!this.timer&&(this.CR<0))&&(Do>0))this.Ba=this.
timer.AK-(this.G6*this.Aq.FrameDelay);if(!!this.timer&&(Do>0)){var At=(this.timer.
AK-this.Ba)|0;BU=(At/this.Aq.FrameDelay)|0;if(At>=Do){BU=BU%this.Aq.NoOfFrames;this.
Ba=this.timer.AK-(At%Do);if(!this.Dd){BU=this.Aq.NoOfFrames-1;Do=0;}}}if(((BU!==
this.CR)&&!!this.I)&&((this.F&0x1)===0x1))this.I.AE(this.H);this.CR=BU;if(!Do&&!
!this.timer){B.rA([this,this.Dc],this.timer,0);this.timer=null;(A=this.Es)?A[1].
call(A[0],this):null;}},Aw:function(E){var A;if((((E===this.BR)&&(E===this.B5))&&(
E===this.B3))&&(E===this.B4))return;this.BR=E;this.B5=E;this.B3=E;this.B4=E;if(!
!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},Hh:function(E){var A;if(this.DB===
E)return;this.DB=E;this.CR=-1;if(!E&&!!this.timer){B.rA([this,this.Dc],this.timer
,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(B.tf.Gz);B.rl([this,this.
Dc],this.timer,0);B.kD([this,this.Dc],this);}if(!!this.I&&((this.F&0x1)===0x1))this.
I.AE(this.H);},DD:function(E){var A;if(E===this.Aq)return;if(!!this.Aq&&this.Aq.
GC)B.rA([this,this.HO],this.Aq,0);this.Aq=E;this.CR=-1;if(!!E&&E.GC)B.rl([this,this.
HO],E,0);if(this.DB){this.Hh(false);this.Hh(true);}if(((this.Gw&&!!E)&&(E.FrameSize[
0]>0))&&(E.FrameSize[1]>0))this.L(B.sp(this.GA(),this.AB));if(!!this.I&&((this.F&
0x1)===0x1))this.I.AE(this.H);},A4:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(
E===this.Ap)return;this.Ap=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);
},GA:function(){var A;if(!this.Aq)return BM;var orient=this.EN;var A1=this.FE;var
BP=this.Aq.FrameSize;var AA=this.H;var width=AA[2]-AA[0];var height=AA[3]-AA[1];
if(!BP[0]||!BP[1])return BM;if((orient===1)||(orient===3)){width=height;height=AA[
2]-AA[0];}var AO=[0,0,width,height];var Y=AO;if(((A1&0x40)===0x40)){var Kt=((((AO[
2]-AO[0])<<16)+((BP[0]/2)|0))/BP[0])|0;var HX=((((AO[3]-AO[1])<<16)+((BP[1]/2)|0
))/BP[1])|0;var E7=Kt;if(HX>E7)E7=HX;Y=B.sU(Y,((BP[0]*E7)+32768)>>16);Y=B.sR(Y,((
BP[1]*E7)+32768)>>16);}else if(((A1&0x80)===0x80)){var Kt=((((AO[2]-AO[0])<<16)+((
BP[0]/2)|0))/BP[0])|0;var HX=((((AO[3]-AO[1])<<16)+((BP[1]/2)|0))/BP[1])|0;var E7=
Kt;if(HX<E7)E7=HX;Y=B.sU(Y,((BP[0]*E7)+32768)>>16);Y=B.sR(Y,((BP[1]*E7)+32768)>>
16);}else if(!((A1&0x100)===0x100))Y=B.sT(Y,BP);if((Y[2]-Y[0])!==(AO[2]-AO[0])){
if(((A1&0x4)===0x4))Y=B.sV(Y,AO[2]-(Y[2]-Y[0]));else if(((A1&0x2)===0x2))Y=B.sV(
Y,(AO[0]+(((AO[2]-AO[0])/2)|0))-(((Y[2]-Y[0])/2)|0));}if((Y[3]-Y[1])!==(AO[3]-AO[
1])){if(((A1&0x20)===0x20))Y=B.sX(Y,AO[3]-(Y[3]-Y[1]));else if(((A1&0x10)===0x10
))Y=B.sX(Y,(AO[1]+(((AO[3]-AO[1])/2)|0))-(((Y[3]-Y[1])/2)|0));}if(!orient)Y=B.sq(
Y,AA.slice(0,2));else if(orient===1){var C1=[AA[0]+Y[1],AA[3]-Y[2]];Y=[].concat(
C1,B.so(C1,[Y[3]-Y[1],Y[2]-Y[0]]));}else if(orient===2){var C1=[AA[2]-Y[2],AA[3]-
Y[3]];Y=[].concat(C1,B.so(C1,[Y[2]-Y[0],Y[3]-Y[1]]));}else if(orient===3){var C1=[
AA[2]-Y[3],AA[1]+Y[0]];Y=[].concat(C1,B.so(C1,[Y[3]-Y[1],Y[2]-Y[0]]));}return B.
sq(Y,this.AB);},Cl:function(){return this.Ap;},_Init:function(aArg){B.Core.BD._Init.
call(this,aArg);this.__proto__=C.BS;},_Mark:function(D){var A;B.Core.BD._Mark.call(
this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Aq)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.Es)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Views::Image"};C.Text={Bj:null,Cn:null,Dm:null,DV:B.pm,Bo:B.g3,
Hl:0,FS:0,B3:0xFFFFFFFF,B4:0xFFFFFFFF,B5:0xFFFFFFFF,BR:0xFFFFFFFF,F$:0,AB:B.pm,FE:
0x12,String:B.g3,Ap:255,EN:0,GM:false,Gw:false,Jk:false,L3:false,Dp:false,C6:function(
A_,aClip,aOffset,A7,aBlend){var A;if((this.Bo===B.g3)||!this.Bj)return;var A1=this.
FE;var orient=this.EN;var font=this.Bj;var AO=B.sq(this.H,aOffset);var BA=this.BR;
var BB=this.B5;var Bz=this.B4;var By=this.B3;var C0=(((A7+1)*this.Ap)>>8)+1;var Em=
this.Bo.charCodeAt(0)||0;var X=B.sq(this.GA(),aOffset);var A9=[AO[0]-X[0],(AO[1]-
X[1])-font.Ascent];if(Em<1)return;if(C0<256){BA=(BA&0x00FFFFFF)|((((((BA>>24)&0xFF
)*C0)>>8)&0xFF)<<24);BB=(BB&0x00FFFFFF)|((((((BB>>24)&0xFF)*C0)>>8)&0xFF)<<24);Bz=(
Bz&0x00FFFFFF)|((((((Bz>>24)&0xFF)*C0)>>8)&0xFF)<<24);By=(By&0x00FFFFFF)|((((((By>>
24)&0xFF)*C0)>>8)&0xFF)<<24);}if(((A1&0x80)===0x80)){if(this.Jo())A1=(A1&~0x80)|
0x4;else A1=(A1&~0x80)|0x1;}if(((Em===1)&&!((A1&0x40)===0x40))&&!orient){A_.KD(aClip
,font,this.Bo,2,(this.Bo.charCodeAt(1)||0)-1,AO,A9,0,orient,BA,BB,Bz,By,true);return;
}var leading=font.Leading;if(this.FS>0)leading=(this.FS-font.Ascent)-font.Descent;
var Ks=(font.Ascent+font.Descent)+leading;var IG=aClip[1]-X[1];var IH=aClip[3]-X[
1];var HB=X[2]-X[0];var Gv=0;var K=1;var CS=this.Bo.charCodeAt(K)||0;if(orient===
1){A9=[X[3]-AO[3],(AO[0]-X[0])-font.Ascent];IG=aClip[0]-X[0];IH=aClip[2]-X[0];HB=
X[3]-X[1];}else if(orient===2){A9=[X[2]-AO[2],(X[3]-AO[3])-font.Ascent];IG=X[3]-
aClip[3];IH=X[3]-aClip[1];}else if(orient===3){A9=[AO[1]-X[1],(X[2]-AO[2])-font.
Ascent];IG=X[2]-aClip[2];IH=X[2]-aClip[0];HB=X[3]-X[1];}while(((Gv+Ks)<IG)&&(CS>
0)){K=K+CS;Gv=Gv+Ks;CS=this.Bo.charCodeAt(K)||0;}while((Gv<IH)&&(CS>0)){var Gp=B.
sn(A9,[0,Gv]);var LK=0;var IR=false;if(((((A1&0x40)===0x40)&&((this.Bo.charCodeAt((
K+CS)-1)||0)!==0x0A))&&((this.Bo.charCodeAt(K+1)||0)!==0x0A))&&((this.Bo.charCodeAt(
K+CS)||0)!==0x00))IR=true;if(IR&&!!(A1&0x6)){var LJ=K+CS;var Lm=this.Bo.indexOf(
String.fromCharCode(0x20),K+1);var Ln=this.Bo.indexOf(String.fromCharCode(0xA0),
K+1);if(((Lm<0)||(Lm>=LJ))&&((Ln<0)||(Ln>=LJ)))IR=false;}if(IR)LK=HB;else if(((A1&
0x4)===0x4))Gp=[(Gp[0]-HB)+font.G8(this.Bo,K+1,CS-1),Gp[1]];else if(((A1&0x2)===
0x2))Gp=[(Gp[0]-((HB/2)|0))+((font.G8(this.Bo,K+1,CS-1)/2)|0),Gp[1]];A_.KD(aClip
,font,this.Bo,K+1,CS-1,AO,Gp,LK,orient,BA,BB,Bz,By,true);K=K+CS;Gv=Gv+Ks;CS=this.
Bo.charCodeAt(K)||0;}},L:function(E){var A;if(B.sb(E,this.H))return;var Kr=false;
if(!this.EN||(this.EN===2))Kr=((A=this.H)[2]-A[0])!==(E[2]-E[0]);else Kr=((A=this.
H)[3]-A[1])!==(E[3]-E[1]);if((((Kr&&!this.F$)&&this.GM)&&this.Dp)&&!((this.F&0x2000
)===0x2000)){this.Bo=B.g3;this.Dp=false;B.kD([this,this.GU],this);}if(((this.Jk&&
this.Dp)&&!B.sa([(A=this.H)[2]-A[0],A[3]-A[1]],[E[2]-E[0],E[3]-E[1]]))&&!((this.
F&0x2000)===0x2000)){this.Bo=B.g3;this.Dp=false;B.kD([this,this.GU],this);}B.Core.
BD.L.call(this,E);B.kD([this,this.Ko],this);},D_:function(){if(!!this.Dm){this.J9(
this.Dm);this.Dm=null;}},J9:function(aBidi){if(!aBidi)return;B.mg(aBidi);},N4:function(
aSize){var bidi=null;bidi=B.pd(aSize);return bidi;},Ko:function(Aj){B.kD(this.Cn
,this);},GU:function(Aj){B.kD([this,this.I2],this);},I2:function(Aj){var A;if(this.
Dp)return;var orient=this.EN;var width=(A=this.H)[2]-A[0];var height=(A=this.H)[
3]-A[1];var DQ=-1;if((orient===1)||(orient===3)){width=height;height=(A=this.H)[
2]-A[0];}if(this.GM){if(this.F$>0)DQ=this.F$;else if(!this.Gw)DQ=width-(this.Hl*
2);else DQ=width;if(DQ<0)DQ=1;}if(!!this.Dm);{this.J9(this.Dm);this.Dm=null;}this.
Dp=true;if((this.String!==B.g3)&&!!this.Bj){var length=this.String.length;if(this.
L3)this.Dm=this.N4(length);this.Bo=this.Bj.MF(this.String,0,DQ,length,this.Dm);if(
!!this.Dm&&!this.L8()){this.J9(this.Dm);this.Dm=null;}}else this.Bo=B.g3;this.DV=
AX;if((this.Jk&&(this.Bo!==B.g3))&&!this.Gw){var A1=this.FE;var font=this.Bj;var
leading=font.Leading;var Bp=this.Bo;var I7=this.Jo();if(((A1&0x80)===0x80)){if(I7
)A1=(A1&~0x80)|0x4;else A1=(A1&~0x80)|0x1;}if(this.FS>0)leading=(this.FS-font.Ascent
)-font.Descent;var HV=(font.Ascent+font.Descent)+leading;var Em=Bp.charCodeAt(0)||
0;var Ft=((height+leading)/HV)|0;var J7=false;var IF=false;if(Ft<=0)Ft=1;if(Em>Ft
){var D8=0;var HW=0;var I6=Em-1;var B2=0;var Co=Bp.length;var tmp=B.g3;if(((A1&0x20
)===0x20))HW=Em-Ft;else if(((A1&0x10)===0x10)){HW=((Em-Ft)/2)|0;I6=(HW+Ft)-1;}else
I6=Ft-1;J7=HW>0;IF=I6<(Em-1);for(B2=1;D8<HW;D8=D8+1)B2=B2+(Bp.charCodeAt(B2)||0);
if(IF)for(Co=B2;D8<I6;D8=D8+1)Co=Co+(Bp.charCodeAt(Co)||0);if(J7){var CH=Bp.charCodeAt(
B2)||0;tmp=(Dx+B.s4(Bp,B2,CH))+Dx;tmp=B.sZ(tmp,0,(CH+2)&0xFFFF);B2=B2+CH;if((tmp.
charCodeAt(CH)||0)===0x0A){tmp=B.sZ(tmp,CH,0xFEFF);tmp=B.sZ(tmp,CH+1,0x0A);}if((
tmp.charCodeAt(2)||0)===0x0A){tmp=B.sZ(tmp,2,0xFEFF);tmp=B.sZ(tmp,1,0x0A);}else tmp=
B.sZ(tmp,1,0xFEFF);}tmp=tmp+B.s4(Bp,B2,Co-B2);if(IF&&(Co>=B2)){var CH=Bp.charCodeAt(
Co)||0;var C4=(Dx+B.s4(Bp,Co,CH))+Dx;C4=B.sZ(C4,0,(CH+2)&0xFFFF);C4=B.sZ(C4,1,0xFEFF
);if((C4.charCodeAt(CH)||0)===0x0A){C4=B.sZ(C4,CH,0xFEFF);C4=B.sZ(C4,CH+1,0x0A);
}if((C4.charCodeAt(2)||0)===0x0A){C4=B.sZ(C4,2,0xFEFF);C4=B.sZ(C4,1,0x0A);}else C4=
B.sZ(C4,1,0xFEFF);tmp=tmp+C4;}Bp=String.fromCharCode(Ft&0xFFFF)+tmp;}var D8=0;var
CG=1;var IV=width-(this.Hl*2);if(this.GM&&(this.F$>0))IV=this.F$;Em=Bp.charCodeAt(
0)||0;for(;D8<Em;D8=D8+1){var Fx=J7&&!D8;var Fy=IF&&(D8===(Em-1));var DL=false;var
DM=false;var GX=I7;if((I7&&Fx)&&!Fy){Fx=false;Fy=true;}else if((I7&&Fy)&&!Fx){Fy=
false;Fx=true;}var HY=CG+1;var CH=Bp.charCodeAt(CG)||0;var B2=HY;var Co=(HY+CH)-
2;var Kf=-1;var Kg=-1;if(!this.GM&&(font.G8(Bp,HY,CH-1)>IV)){if(((A1&0x4)===0x4)
)DL=true;else if(((A1&0x2)===0x2)){DL=true;DM=true;}else DM=true;}if((Bp.charCodeAt(
B2)||0)===0x0A)B2=B2+1;if((Bp.charCodeAt(Co)||0)===0x0A)Co=Co-1;while(DL&&((Bp.charCodeAt(
B2)||0)===0xFEFF))B2=B2+1;while(DM&&((Bp.charCodeAt(Co)||0)===0xFEFF))Co=Co-1;DL=
DL&&!Fy;DM=DM&&!Fx;while((((DL||DM)||Fx)||Fy)&&(B2<Co)){if((DL&&(GX||!DM))||Fx){
if(Kf>0)Bp=B.sZ(Bp,Kf,0xFEFF);Bp=B.sZ(Bp,B2,0x2026);Kf=B2;B2=B2+1;GX=!GX;Fx=false;
if(font.G8(Bp,HY,CH-1)<=IV){DL=false;DM=false;}else DL=DL||!DM;}if((DM&&(!GX||!DL
))||Fy){if(Kg>0)Bp=B.sZ(Bp,Kg,0xFEFF);Bp=B.sZ(Bp,Co,0x2026);Kg=Co;Co=Co-1;GX=!GX;
Fy=false;if(font.G8(Bp,HY,CH-1)<=IV){DL=false;DM=false;}else DM=DM||!DL;}}CG=CG+
CH;}this.DV=[font.KG(Bp),((Bp.charCodeAt(0)||0)*HV)-leading];this.Bo=Bp;}if(this.
Gw&&(this.Bo!==B.g3)){var E5=[this.Hl,0];if((orient===1)||(orient===3)){E5=[E5[0
],E5[0]];E5=[0,E5[1]];}this.F=this.F|0x2000;this.L(B.sp(B.r7(this.GA(),E5),this.
AB));this.F=this.F&~0x2000;}if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);
B.kD([this,this.Ko],this);},Ed:function(E){if(E===this.GM)return;this.GM=E;if(this.
Dp){this.Bo=B.g3;this.Dp=false;B.kD([this,this.GU],this);}if(E&&!this.F$)this.F=
this.F&~0x100;else this.F=this.F|0x100;},B$:function(E){var A;if(E===this.FE)return;
this.FE=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);if(this.Jk){this.Bo=
B.g3;this.Dp=false;B.kD([this,this.GU],this);}if(this.Dp)B.kD([this,this.Ko],this
);},A6:function(E){if(E===this.String)return;this.String=E;this.Bo=B.g3;this.Dp=
false;B.kD([this,this.GU],this);},B0:function(E){if(E===this.Bj)return;this.Bj=E;
this.Bo=B.g3;this.Dp=false;B.kD([this,this.GU],this);},H_:function(){return this.
BR;},Aw:function(E){var A;if((((E===this.BR)&&(E===this.B5))&&(E===this.B3))&&(E===
this.B4))return;this.BR=E;this.B5=E;this.B3=E;this.B4=E;if(!!this.I&&((this.F&0x1
)===0x1))this.I.AE(this.H);},A4:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(E===
this.Ap)return;this.Ap=E;if(!!this.I&&((this.F&0x1)===0x1))this.I.AE(this.H);},Jo:
function(){if(!this.Dp)this.I2(this);if(!this.Dm)return false;var result=false;var
bidi=this.Dm;result=B.pc(bidi);return result;},L8:function(){if(!this.Dp)this.I2(
this);if(!this.Dm)return false;var result=false;var bidi=this.Dm;result=B.rq(bidi
);return result;},GA:function(){var A;if((this.String===B.g3)||!this.Bj)return BM;
if(!this.Dp)this.I2(this);if(this.Bo===B.g3)return BM;var leading=this.Bj.Leading;
var HV=(this.Bj.Ascent+this.Bj.Descent)+this.Bj.Leading;if(this.FS>0){leading=(this.
FS-this.Bj.Ascent)-this.Bj.Descent;HV=this.FS;}if(B.sa(this.DV,AX))this.DV=[this.
Bj.KG(this.Bo),this.DV[1]];this.DV=[this.DV[0],((this.Bo.charCodeAt(0)||0)*HV)-leading
];var A1=this.FE;var orient=this.EN;var AA=this.H;var E5=this.Hl;var width=AA[2]-
AA[0];var height=AA[3]-AA[1];if((orient===1)||(orient===3)){width=height;height=
AA[2]-AA[0];}var AO=[E5,0,width-E5,height];var Y=[].concat(AO.slice(0,2),B.so(AO.
slice(0,2),this.DV));if(((A1&0x80)===0x80)){if(this.Jo())A1=(A1&~0x80)|0x4;else A1=(
A1&~0x80)|0x1;}if(((A1&0x40)===0x40)){var DQ=this.F$;if(DQ<=0)DQ=width-(this.Hl*
2);if(DQ<0)DQ=0;if(DQ>(Y[2]-Y[0]))Y=B.sU(Y,DQ);}if((Y[2]-Y[0])!==(AO[2]-AO[0])){
if(((A1&0x4)===0x4))Y=B.sV(Y,AO[2]-(Y[2]-Y[0]));else if(((A1&0x2)===0x2))Y=B.sV(
Y,(AO[0]+(((AO[2]-AO[0])/2)|0))-(((Y[2]-Y[0])/2)|0));}if((Y[3]-Y[1])!==(AO[3]-AO[
1])){if(((A1&0x20)===0x20))Y=B.sX(Y,AO[3]-(Y[3]-Y[1]));else if(((A1&0x10)===0x10
))Y=B.sX(Y,(AO[1]+(((AO[3]-AO[1])/2)|0))-(((Y[3]-Y[1])/2)|0));}if(!orient)Y=B.sq(
Y,AA.slice(0,2));else if(orient===1){var C1=[AA[0]+Y[1],AA[3]-Y[2]];Y=[].concat(
C1,B.so(C1,[this.DV[1],this.DV[0]]));}else if(orient===2){var C1=[AA[2]-Y[2],AA[
3]-Y[3]];Y=[].concat(C1,B.so(C1,this.DV));}else if(orient===3){var C1=[AA[2]-Y[3
],AA[1]+Y[0]];Y=[].concat(C1,B.so(C1,[this.DV[1],this.DV[0]]));}return B.sq(Y,this.
AB);},Cl:function(){return this.Ap;},_Init:function(aArg){B.Core.BD._Init.call(this
,aArg);this.__proto__=C.Text;},_Done:function(){this.__proto__=C.Text;this.D_();
B.Core.BD._Done.call(this);},_Mark:function(D){var A;B.Core.BD._Mark.call(this,D
);if((A=this.Bj)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Cn)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::Text"};C.O4={LS:0x1,LR:0x2,LT:0x4,
LW:0x8,LV:0x10,LU:0x20,Pq:0x40,Pr:0x80,Pw:0x100};C.Px={LS:0x1,LR:0x2,LT:0x4,LW:0x8
,LV:0x10,LU:0x20,Ox:0x40,Ow:0x80};C.EN={Pa:0,Po:1,Pm:2,Pn:3};
C._Init=function(){C.U.__proto__=B.Core.BD;C.BQ.__proto__=B.Core.BD;C.BX.__proto__=
B.Core.BD;C.BS.__proto__=B.Core.BD;C.Text.__proto__=B.Core.BD;};C.Dh=function(D){
};return C;})();

/* Embedded Wizard */