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
);if(EmWiApp.tf)throw new Error("The unit file 'Views.js' included twice!");EmWiApp.
tf=(function(){var B=EmWiApp;var D={};
var Az=[0,0];var A3=[0,0,0,0];var BY="\uFEFF";
D.C5={Br:0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bl:0xFFFFFFFF,Ci:function(Ar,aClip
,aOffset,AD,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);AD=AD+1;if(AD<256){
var AX=this.Bl;var AY=this.Bt;var AV=this.Br;var AW=this.Bs;AX=(AX&0x00FFFFFF)|((((
AD*((AX>>24)&0xFF))>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((AD*((AY>>24)&0xFF))>>8
)&0xFF)<<24);AV=(AV&0x00FFFFFF)|((((AD*((AV>>24)&0xFF))>>8)&0xFF)<<24);AW=(AW&0x00FFFFFF
)|((((AD*((AW>>24)&0xFF))>>8)&0xFF)<<24);Ar.Fx(aClip,B.sq(this.N,aOffset),AX,AY,
AW,AV,aBlend);}else Ar.Fx(aClip,B.sq(this.N,aOffset),this.Bl,this.Bt,this.Bs,this.
Br,aBlend);},BP:function(C){var A;if((((C===this.Bl)&&(C===this.Bt))&&(C===this.
Br))&&(C===this.Bs))return;this.Bl=C;this.Bt=C;this.Br=C;this.Bs=C;if(!!this.K&&((
this.F&0x1)===0x1))this.K.An(this.N);},_Init:function(aArg){B.Core.Bb._Init.call(
this,aArg);this.__proto__=D.C5;},_className:"Views::Rectangle"};D.Cu={Width:1,Br:
0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bl:0xFFFFFFFF,Ci:function(Ar,aClip,aOffset
,AD,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);AD=AD+1;if(AD<256){var AX=
this.Bl;var AY=this.Bt;var AV=this.Br;var AW=this.Bs;AX=(AX&0x00FFFFFF)|((((AD*((
AX>>24)&0xFF))>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((AD*((AY>>24)&0xFF))>>8)&0xFF
)<<24);AV=(AV&0x00FFFFFF)|((((AD*((AV>>24)&0xFF))>>8)&0xFF)<<24);AW=(AW&0x00FFFFFF
)|((((AD*((AW>>24)&0xFF))>>8)&0xFF)<<24);Ar.Js(aClip,B.sq(this.N,aOffset),this.Width
,AX,AY,AW,AV,aBlend);}else Ar.Js(aClip,B.sq(this.N,aOffset),this.Width,this.Bl,this.
Bt,this.Bs,this.Br,aBlend);},L6:function(C){var A;if(C<0)C=0;if(C===this.Width)return;
this.Width=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},K3:function(){
return this.Bl;},BP:function(C){var A;if((((C===this.Bl)&&(C===this.Bt))&&(C===this.
Br))&&(C===this.Bs))return;this.Bl=C;this.Bt=C;this.Br=C;this.Bs=C;if(!!this.K&&((
this.F&0x1)===0x1))this.K.An(this.N);},_Init:function(aArg){B.Core.Bb._Init.call(
this,aArg);this.__proto__=D.Cu;},_className:"Views::Border"};D.GO={timer:null,W:
null,D6:null,Bx:-1,Br:0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bl:0xFFFFFFFF,Ju:0x1F
,DJ:0,DO:255,AJ:0,Ip:B.pm,CA:false,HA:true,Ci:function(Ar,aClip,aOffset,AD,aBlend
){var A;var Bd=this.DJ;if(this.Bx>=0)Bd=this.Bx;if(!this.W||(Bd>=this.W.NoOfFrames
))return;this.W.Update();var AX=this.Bl;var AY=this.Bt;var AW=this.Bs;var AV=this.
Br;var B6=(((AD+1)*this.DO)>>8)+1;var BJ=this.Ju;var M=B.sq(this.N,aOffset);var Hu=
B.sn([M[2]-M[0],M[3]-M[1]],this.Ip);aBlend=aBlend&&((this.F&0x2)===0x2);if(B6<256
){AX=(AX&0x00FFFFFF)|((((((AX>>24)&0xFF)*B6)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((
AY>>24)&0xFF)*B6)>>8)&0xFF)<<24);AW=(AW&0x00FFFFFF)|((((((AW>>24)&0xFF)*B6)>>8)&
0xFF)<<24);AV=(AV&0x00FFFFFF)|((((((AV>>24)&0xFF)*B6)>>8)&0xFF)<<24);}if(((this.
Ip[0]>0)&&(Hu[0]>0))&&!((BJ&0x5)===0x5)){var BZ=((this.W.FrameSize[0]/3)|0)-Hu[0
];if(((BJ&0x1)===0x1)){if(aClip[2]>M[2])aClip=B.sW(aClip,M[2]);if(BZ>0)M=B.sW(M,
M[2]+BZ);BJ=BJ|0x4;}else if(((BJ&0x4)===0x4)){if(aClip[0]<M[0])aClip=[].concat(M[
0],aClip.slice(1,4));if(BZ>0)M=[].concat(M[0]-BZ,M.slice(1,4));BJ=BJ|0x1;}else{if(
aClip[0]<M[0])aClip=[].concat(M[0],aClip.slice(1,4));if(aClip[2]>M[2])aClip=B.sW(
aClip,M[2]);if(BZ>0){M=[].concat(M[0]-((BZ/2)|0),M.slice(1,4));M=B.sW(M,(M[2]+BZ
)-((BZ/2)|0));}BJ=BJ|0x5;}}if(((this.Ip[1]>0)&&(Hu[1]>0))&&!((BJ&0xA)===0xA)){var
BZ=((this.W.FrameSize[1]/3)|0)-Hu[1];if(((BJ&0x2)===0x2)){if(aClip[3]>M[3])aClip=[
].concat(aClip.slice(0,3),M[3]);if(BZ>0)M=[].concat(M.slice(0,3),M[3]+BZ);BJ=BJ|
0x8;}else if(((BJ&0x8)===0x8)){if(aClip[1]<M[1])aClip=B.sY(aClip,M[1]);if(BZ>0)M=
B.sY(M,M[1]-BZ);BJ=BJ|0x2;}else{if(aClip[1]<M[1])aClip=B.sY(aClip,M[1]);if(aClip[
3]>M[3])aClip=[].concat(aClip.slice(0,3),M[3]);if(BZ>0){M=B.sY(M,M[1]-((BZ/2)|0)
);M=[].concat(M.slice(0,3),(M[3]+BZ)-((BZ/2)|0));}BJ=BJ|0xA;}}Ar.KA(aClip,this.W
,Bd,M,BJ,AX,AY,AW,AV,aBlend);},Gr:function(AT){var A;if(!!this.K&&((this.F&0x1)===
0x1))this.K.An(this.N);},CX:function(AT){var A;var Bd=this.Bx;var Cr=0;if(!!this.
W)Cr=this.W.NoOfFrames*this.W.FrameDelay;if((!!this.timer&&(this.Bx<0))&&(Cr>0))
this.AJ=this.timer.AF-(this.DJ*this.W.FrameDelay);if(!!this.timer&&(Cr>0)){var Ac=(
this.timer.AF-this.AJ)|0;Bd=(Ac/this.W.FrameDelay)|0;if(Ac>=Cr){Bd=Bd%this.W.NoOfFrames;
this.AJ=this.timer.AF-(Ac%Cr);if(!this.HA){Bd=this.W.NoOfFrames-1;Cr=0;}}}if(((Bd
!==this.Bx)&&!!this.K)&&((this.F&0x1)===0x1))this.K.An(this.N);this.Bx=Bd;if(!Cr&&
!!this.timer){B.rA([this,this.CX],this.timer,0);this.timer=null;(A=this.D6)?A[1].
call(A[0],this):null;}},BP:function(C){var A;if((((C===this.Bl)&&(C===this.Bt))&&(
C===this.Br))&&(C===this.Bs))return;this.Bl=C;this.Bt=C;this.Br=C;this.Bs=C;if(!
!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},DN:function(C){var A;if(this.CA===
C)return;this.CA=C;this.Bx=-1;if(!C&&!!this.timer){B.rA([this,this.CX],this.timer
,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(B.th.Fi);B.rl([this,this.
CX],this.timer,0);B.kD([this,this.CX],this);}if(!!this.K&&((this.F&0x1)===0x1))this.
K.An(this.N);},EA:function(C){var A;if(C<0)C=0;if((C===this.DJ)&&(this.Bx===-1))
return;this.DJ=C;if(!this.timer)this.Bx=-1;if(!!this.K&&((this.F&0x1)===0x1))this.
K.An(this.N);},D7:function(C){var A;if(C===this.W)return;if(!!this.W&&this.W.E3)
B.rA([this,this.Gr],this.W,0);this.W=C;this.Bx=-1;if(!!C&&C.E3)B.rl([this,this.Gr
],C,0);if(this.CA){this.DN(false);this.DN(true);}if(!!this.K&&((this.F&0x1)===0x1
))this.K.An(this.N);},_Init:function(aArg){B.Core.Bb._Init.call(this,aArg);this.
__proto__=D.GO;},_Mark:function(E){var A;B.Core.Bb._Mark.call(this,E);if((A=this.
timer)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.W)&&(A._cycle!=E))A._Mark(A.
_cycle=E);if((A=this.D6)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);},_className:
"Views::Frame"};D.FD={timer:null,W:null,D6:null,AJ:0,Bx:0,Br:0xFFFFFFFF,Bs:0xFFFFFFFF
,Bt:0xFFFFFFFF,Bl:0xFFFFFFFF,DR:B.pm,DD:0x12,DJ:0,DO:255,DP:0,CA:false,HA:true,E0:
false,G0:true,Ci:function(Ar,aClip,aOffset,AD,aBlend){var A;var Bd=this.DJ;if(this.
Bx>=0)Bd=this.Bx;if(!this.W||(Bd>=this.W.NoOfFrames))return;this.W.Update();var U=
this.E2();var Z=this.W.FrameSize;var orient=this.DP;if((U[0]>=U[2])||(U[1]>=U[3]
))return;var AX=this.Bl;var AY=this.Bt;var AW=this.Bs;var AV=this.Br;var B6=(((AD+
1)*this.DO)>>8)+1;aBlend=aBlend&&((this.F&0x2)===0x2);if(B6<256){AX=(AX&0x00FFFFFF
)|((((((AX>>24)&0xFF)*B6)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*
B6)>>8)&0xFF)<<24);AW=(AW&0x00FFFFFF)|((((((AW>>24)&0xFF)*B6)>>8)&0xFF)<<24);AV=(
AV&0x00FFFFFF)|((((((AV>>24)&0xFF)*B6)>>8)&0xFF)<<24);}if(B.sa([U[2]-U[0],U[3]-U[
1]],Z)&&!orient)Ar.Es(aClip,this.W,Bd,B.sq(this.N,aOffset),B.sn(this.N.slice(0,2
),U.slice(0,2)),AX,AY,AW,AV,aBlend);else if(!orient)Ar.L$(aClip,this.W,Bd,B.sq(U
,aOffset),[].concat(Az,Z),AX,AY,AW,AV,aBlend,this.G0);else{var left=U[0]+aOffset[
0];var top=U[1]+aOffset[1];var right=U[2]+aOffset[0];var bottom=U[3]+aOffset[1];
if(orient===1)Ar.II(aClip,this.W,Bd,left,bottom,1.000000,left,top,1.000000,right
,top,1.000000,right,bottom,1.000000,[].concat(Az,Z),AV,AX,AY,AW,aBlend,this.G0);
else if(orient===2)Ar.II(aClip,this.W,Bd,right,bottom,1.000000,left,bottom,1.000000
,left,top,1.000000,right,top,1.000000,[].concat(Az,Z),AW,AV,AX,AY,aBlend,this.G0
);else if(orient===3)Ar.II(aClip,this.W,Bd,right,top,1.000000,right,bottom,1.000000
,left,bottom,1.000000,left,top,1.000000,[].concat(Az,Z),AY,AW,AV,AX,aBlend,this.
G0);}},Gr:function(AT){var A;if(((this.E0&&!!this.W)&&(this.W.FrameSize[0]>0))&&(
this.W.FrameSize[1]>0))this.Ai(B.sp(this.E2(),this.DR));if(!!this.K&&((this.F&0x1
)===0x1))this.K.An(this.N);},CX:function(AT){var A;var Bd=this.Bx;var Cr=0;if(!!
this.W)Cr=this.W.NoOfFrames*this.W.FrameDelay;if((!!this.timer&&(this.Bx<0))&&(Cr>
0))this.AJ=this.timer.AF-(this.DJ*this.W.FrameDelay);if(!!this.timer&&(Cr>0)){var
Ac=(this.timer.AF-this.AJ)|0;Bd=(Ac/this.W.FrameDelay)|0;if(Ac>=Cr){Bd=Bd%this.W.
NoOfFrames;this.AJ=this.timer.AF-(Ac%Cr);if(!this.HA){Bd=this.W.NoOfFrames-1;Cr=
0;}}}if(((Bd!==this.Bx)&&!!this.K)&&((this.F&0x1)===0x1))this.K.An(this.N);this.
Bx=Bd;if(!Cr&&!!this.timer){B.rA([this,this.CX],this.timer,0);this.timer=null;(A=
this.D6)?A[1].call(A[0],this):null;}},BP:function(C){var A;if((((C===this.Bl)&&(
C===this.Bt))&&(C===this.Br))&&(C===this.Bs))return;this.Bl=C;this.Bt=C;this.Br=
C;this.Bs=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},DN:function(C){
var A;if(this.CA===C)return;this.CA=C;this.Bx=-1;if(!C&&!!this.timer){B.rA([this
,this.CX],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(B.th.
Fi);B.rl([this,this.CX],this.timer,0);B.kD([this,this.CX],this);}if(!!this.K&&((
this.F&0x1)===0x1))this.K.An(this.N);},Cw:function(C){var A;if(C===this.DD)return;
this.DD=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},EA:function(C){var
A;if(C<0)C=0;if((C===this.DJ)&&(this.Bx===-1))return;this.DJ=C;if(!this.timer)this.
Bx=-1;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},D7:function(C){var A;
if(C===this.W)return;if(!!this.W&&this.W.E3)B.rA([this,this.Gr],this.W,0);this.W=
C;this.Bx=-1;if(!!C&&C.E3)B.rl([this,this.Gr],C,0);if(this.CA){this.DN(false);this.
DN(true);}if(((this.E0&&!!C)&&(C.FrameSize[0]>0))&&(C.FrameSize[1]>0))this.Ai(B.
sp(this.E2(),this.DR));if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},E2:
function(){var A;if(!this.W)return A3;var orient=this.DP;var Ax=this.DD;var Z=this.
W.FrameSize;var Ah=this.N;var width=Ah[2]-Ah[0];var height=Ah[3]-Ah[1];if(!Z[0]||
!Z[1])return A3;if((orient===1)||(orient===3)){width=height;height=Ah[2]-Ah[0];}
var Am=[0,0,width,height];var P=Am;if(((Ax&0x40)===0x40)){var Jk=((((Am[2]-Am[0]
)<<16)+((Z[0]/2)|0))/Z[0])|0;var Gz=((((Am[3]-Am[1])<<16)+((Z[1]/2)|0))/Z[1])|0;
var D2=Jk;if(Gz>D2)D2=Gz;P=B.sU(P,((Z[0]*D2)+32768)>>16);P=B.sR(P,((Z[1]*D2)+32768
)>>16);}else if(((Ax&0x80)===0x80)){var Jk=((((Am[2]-Am[0])<<16)+((Z[0]/2)|0))/Z[
0])|0;var Gz=((((Am[3]-Am[1])<<16)+((Z[1]/2)|0))/Z[1])|0;var D2=Jk;if(Gz<D2)D2=Gz;
P=B.sU(P,((Z[0]*D2)+32768)>>16);P=B.sR(P,((Z[1]*D2)+32768)>>16);}else if(!((Ax&0x100
)===0x100))P=B.sT(P,Z);if((P[2]-P[0])!==(Am[2]-Am[0])){if(((Ax&0x4)===0x4))P=B.sV(
P,Am[2]-(P[2]-P[0]));else if(((Ax&0x2)===0x2))P=B.sV(P,(Am[0]+(((Am[2]-Am[0])/2)|
0))-(((P[2]-P[0])/2)|0));}if((P[3]-P[1])!==(Am[3]-Am[1])){if(((Ax&0x20)===0x20))
P=B.sX(P,Am[3]-(P[3]-P[1]));else if(((Ax&0x10)===0x10))P=B.sX(P,(Am[1]+(((Am[3]-
Am[1])/2)|0))-(((P[3]-P[1])/2)|0));}if(!orient)P=B.sq(P,Ah.slice(0,2));else if(orient===
1){var B7=[Ah[0]+P[1],Ah[3]-P[2]];P=[].concat(B7,B.so(B7,[P[3]-P[1],P[2]-P[0]]));
}else if(orient===2){var B7=[Ah[2]-P[2],Ah[3]-P[3]];P=[].concat(B7,B.so(B7,[P[2]-
P[0],P[3]-P[1]]));}else if(orient===3){var B7=[Ah[2]-P[3],Ah[1]+P[0]];P=[].concat(
B7,B.so(B7,[P[3]-P[1],P[2]-P[0]]));}return B.sq(P,this.DR);},_Init:function(aArg
){B.Core.Bb._Init.call(this,aArg);this.__proto__=D.FD;},_Mark:function(E){var A;
B.Core.Bb._Mark.call(this,E);if((A=this.timer)&&(A._cycle!=E))A._Mark(A._cycle=E
);if((A=this.W)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.D6)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);},_className:"Views::Image"};D.Text={AR:null,JW:null,Cm:
null,CW:B.pm,AL:B.g3,FS:0,EE:0,Br:0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bl:0xFFFFFFFF
,EG:0,DR:B.pm,DD:0x12,String:B.g3,DO:255,DP:0,E_:false,E0:false,Fj:false,Hz:false
,B8:false,Ci:function(Ar,aClip,aOffset,AD,aBlend){var A;if((this.AL===B.g3)||!this.
AR)return;var Ax=this.DD;var orient=this.DP;var font=this.AR;var Am=B.sq(this.N,
aOffset);var AX=this.Bl;var AY=this.Bt;var AW=this.Bs;var AV=this.Br;var B6=(((AD+
1)*this.DO)>>8)+1;var Dj=this.AL.charCodeAt(0)||0;var U=B.sq(this.E2(),aOffset);
var Cq=[Am[0]-U[0],(Am[1]-U[1])-font.Ascent];if(Dj<1)return;if(B6<256){AX=(AX&0x00FFFFFF
)|((((((AX>>24)&0xFF)*B6)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*
B6)>>8)&0xFF)<<24);AW=(AW&0x00FFFFFF)|((((((AW>>24)&0xFF)*B6)>>8)&0xFF)<<24);AV=(
AV&0x00FFFFFF)|((((((AV>>24)&0xFF)*B6)>>8)&0xFF)<<24);}if(((Ax&0x80)===0x80)){if(
this.H_())Ax=(Ax&~0x80)|0x4;else Ax=(Ax&~0x80)|0x1;}if(((Dj===1)&&!((Ax&0x40)===
0x40))&&!orient){Ar.Jt(aClip,font,this.AL,2,(this.AL.charCodeAt(1)||0)-1,Am,Cq,0
,orient,AX,AY,AW,AV,true);return;}var leading=font.Leading;if(this.EE>0)leading=(
this.EE-font.Ascent)-font.Descent;var Jj=(font.Ascent+font.Descent)+leading;var G_=
aClip[1]-U[1];var G$=aClip[3]-U[1];var Gc=U[2]-U[0];var EZ=0;var J=1;var BH=this.
AL.charCodeAt(J)||0;if(orient===1){Cq=[U[3]-Am[3],(Am[0]-U[0])-font.Ascent];G_=aClip[
0]-U[0];G$=aClip[2]-U[0];Gc=U[3]-U[1];}else if(orient===2){Cq=[U[2]-Am[2],(U[3]-
Am[3])-font.Ascent];G_=U[3]-aClip[3];G$=U[3]-aClip[1];}else if(orient===3){Cq=[Am[
1]-U[1],(U[2]-Am[2])-font.Ascent];G_=U[2]-aClip[2];G$=U[2]-aClip[0];Gc=U[3]-U[1];
}while(((EZ+Jj)<G_)&&(BH>0)){J=J+BH;EZ=EZ+Jj;BH=this.AL.charCodeAt(J)||0;}while((
EZ<G$)&&(BH>0)){var ER=B.sn(Cq,[0,EZ]);var Km=0;var Hi=false;if(((((Ax&0x40)===0x40
)&&((this.AL.charCodeAt((J+BH)-1)||0)!==0x0A))&&((this.AL.charCodeAt(J+1)||0)!==
0x0A))&&((this.AL.charCodeAt(J+BH)||0)!==0x00))Hi=true;if(Hi&&!!(Ax&0x6)){var Kl=
J+BH;var J_=this.AL.indexOf(String.fromCharCode(0x20),J+1);var J$=this.AL.indexOf(
String.fromCharCode(0xA0),J+1);if(((J_<0)||(J_>=Kl))&&((J$<0)||(J$>=Kl)))Hi=false;
}if(Hi)Km=Gc;else if(((Ax&0x4)===0x4))ER=[(ER[0]-Gc)+font.Fy(this.AL,J+1,BH-1),ER[
1]];else if(((Ax&0x2)===0x2))ER=[(ER[0]-((Gc/2)|0))+((font.Fy(this.AL,J+1,BH-1)/
2)|0),ER[1]];Ar.Jt(aClip,font,this.AL,J+1,BH-1,Am,ER,Km,orient,AX,AY,AW,AV,true);
J=J+BH;EZ=EZ+Jj;BH=this.AL.charCodeAt(J)||0;}},Ai:function(C){var A;if(B.sb(C,this.
N))return;var Ji=false;if(!this.DP||(this.DP===2))Ji=((A=this.N)[2]-A[0])!==(C[2
]-C[0]);else Ji=((A=this.N)[3]-A[1])!==(C[3]-C[1]);if((((Ji&&!this.EG)&&this.E_)&&
this.B8)&&!((this.F&0x2000)===0x2000)){this.AL=B.g3;this.B8=false;B.kD([this,this.
El],this);}if(((this.Fj&&this.B8)&&!B.sa([(A=this.N)[2]-A[0],A[3]-A[1]],[C[2]-C[
0],C[3]-C[1]]))&&!((this.F&0x2000)===0x2000)){this.AL=B.g3;this.B8=false;B.kD([this
,this.El],this);}B.Core.Bb.Ai.call(this,C);B.kD([this,this.Jf],this);},CC:function(
){if(!!this.Cm){this.IY(this.Cm);this.Cm=null;}},IY:function(aBidi){if(!aBidi)return;
B.mg(aBidi);},My:function(aSize){var bidi=null;bidi=B.pd(aSize);return bidi;},Jf:
function(AT){B.kD(this.JW,this);},El:function(AT){B.kD([this,this.Hp],this);},Hp:
function(AT){var A;if(this.B8)return;var orient=this.DP;var width=(A=this.N)[2]-
A[0];var height=(A=this.N)[3]-A[1];var CT=-1;if((orient===1)||(orient===3)){width=
height;height=(A=this.N)[2]-A[0];}if(this.E_){if(this.EG>0)CT=this.EG;else if(!this.
E0)CT=width-(this.FS*2);else CT=width;if(CT<0)CT=1;}if(!!this.Cm);{this.IY(this.
Cm);this.Cm=null;}this.B8=true;if((this.String!==B.g3)&&!!this.AR){var length=this.
String.length;if(this.Hz)this.Cm=this.My(length);this.AL=this.AR.L7(this.String,
0,CT,length,this.Cm);if(!!this.Cm&&!this.KT()){this.IY(this.Cm);this.Cm=null;}}else
this.AL=B.g3;this.CW=Az;if((this.Fj&&(this.AL!==B.g3))&&!this.E0){var Ax=this.DD;
var font=this.AR;var leading=font.Leading;var AS=this.AL;var Hs=this.H_();if(((Ax&
0x80)===0x80)){if(Hs)Ax=(Ax&~0x80)|0x4;else Ax=(Ax&~0x80)|0x1;}if(this.EE>0)leading=(
this.EE-font.Ascent)-font.Descent;var Gx=(font.Ascent+font.Descent)+leading;var Dj=
AS.charCodeAt(0)||0;var Ei=((height+leading)/Gx)|0;var IU=false;var G9=false;if(
Ei<=0)Ei=1;if(Dj>Ei){var C_=0;var Gy=0;var Hr=Dj-1;var Bi=0;var BD=AS.length;var
tmp=B.g3;if(((Ax&0x20)===0x20))Gy=Dj-Ei;else if(((Ax&0x10)===0x10)){Gy=((Dj-Ei)/
2)|0;Hr=(Gy+Ei)-1;}else Hr=Ei-1;IU=Gy>0;G9=Hr<(Dj-1);for(Bi=1;C_<Gy;C_=C_+1)Bi=Bi+(
AS.charCodeAt(Bi)||0);if(G9)for(BD=Bi;C_<Hr;C_=C_+1)BD=BD+(AS.charCodeAt(BD)||0);
if(IU){var BS=AS.charCodeAt(Bi)||0;tmp=(BY+B.s4(AS,Bi,BS))+BY;tmp=B.sZ(tmp,0,(BS+
2)&0xFFFF);Bi=Bi+BS;if((tmp.charCodeAt(BS)||0)===0x0A){tmp=B.sZ(tmp,BS,0xFEFF);tmp=
B.sZ(tmp,BS+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.sZ(tmp,2,0xFEFF);tmp=
B.sZ(tmp,1,0x0A);}else tmp=B.sZ(tmp,1,0xFEFF);}tmp=tmp+B.s4(AS,Bi,BD-Bi);if(G9&&(
BD>=Bi)){var BS=AS.charCodeAt(BD)||0;var B_=(BY+B.s4(AS,BD,BS))+BY;B_=B.sZ(B_,0,(
BS+2)&0xFFFF);B_=B.sZ(B_,1,0xFEFF);if((B_.charCodeAt(BS)||0)===0x0A){B_=B.sZ(B_,
BS,0xFEFF);B_=B.sZ(B_,BS+1,0x0A);}if((B_.charCodeAt(2)||0)===0x0A){B_=B.sZ(B_,2,
0xFEFF);B_=B.sZ(B_,1,0x0A);}else B_=B.sZ(B_,1,0xFEFF);tmp=tmp+B_;}AS=String.fromCharCode(
Ei&0xFFFF)+tmp;}var C_=0;var Af=1;var Hm=width-(this.FS*2);if(this.E_&&(this.EG>
0))Hm=this.EG;Dj=AS.charCodeAt(0)||0;for(;C_<Dj;C_=C_+1){var En=IU&&!C_;var Eo=G9&&(
C_===(Dj-1));var CL=false;var CM=false;var Ff=Hs;if((Hs&&En)&&!Eo){En=false;Eo=true;
}else if((Hs&&Eo)&&!En){Eo=false;En=true;}var GA=Af+1;var BS=AS.charCodeAt(Af)||
0;var Bi=GA;var BD=(GA+BS)-2;var I4=-1;var I5=-1;if(!this.E_&&(font.Fy(AS,GA,BS-
1)>Hm)){if(((Ax&0x4)===0x4))CL=true;else if(((Ax&0x2)===0x2)){CL=true;CM=true;}else
CM=true;}if((AS.charCodeAt(Bi)||0)===0x0A)Bi=Bi+1;if((AS.charCodeAt(BD)||0)===0x0A
)BD=BD-1;while(CL&&((AS.charCodeAt(Bi)||0)===0xFEFF))Bi=Bi+1;while(CM&&((AS.charCodeAt(
BD)||0)===0xFEFF))BD=BD-1;CL=CL&&!Eo;CM=CM&&!En;while((((CL||CM)||En)||Eo)&&(Bi<
BD)){if((CL&&(Ff||!CM))||En){if(I4>0)AS=B.sZ(AS,I4,0xFEFF);AS=B.sZ(AS,Bi,0x2026);
I4=Bi;Bi=Bi+1;Ff=!Ff;En=false;if(font.Fy(AS,GA,BS-1)<=Hm){CL=false;CM=false;}else
CL=CL||!CM;}if((CM&&(!Ff||!CL))||Eo){if(I5>0)AS=B.sZ(AS,I5,0xFEFF);AS=B.sZ(AS,BD
,0x2026);I5=BD;BD=BD-1;Ff=!Ff;Eo=false;if(font.Fy(AS,GA,BS-1)<=Hm){CL=false;CM=false;
}else CM=CM||!CL;}}Af=Af+BS;}this.CW=[font.Jx(AS),((AS.charCodeAt(0)||0)*Gx)-leading
];this.AL=AS;}if(this.E0&&(this.AL!==B.g3)){var D0=[this.FS,0];if((orient===1)||(
orient===3)){D0=[D0[0],D0[0]];D0=[0,D0[1]];}this.F=this.F|0x2000;this.Ai(B.sp(B.
r7(this.E2(),D0),this.DR));this.F=this.F&~0x2000;}if(!!this.K&&((this.F&0x1)===0x1
))this.K.An(this.N);B.kD([this,this.Jf],this);},JT:function(C){if(C===this.Hz)return;
this.Hz=C;this.AL=B.g3;this.B8=false;B.kD([this,this.El],this);},JS:function(C){
if(C===this.Fj)return;this.Fj=C;this.AL=B.g3;this.B8=false;B.kD([this,this.El],this
);},Ix:function(C){if(C===this.E_)return;this.E_=C;if(this.B8){this.AL=B.g3;this.
B8=false;B.kD([this,this.El],this);}if(C&&!this.EG)this.F=this.F&~0x100;else this.
F=this.F|0x100;},Cw:function(C){var A;if(C===this.DD)return;this.DD=C;if(!!this.
K&&((this.F&0x1)===0x1))this.K.An(this.N);if(this.Fj){this.AL=B.g3;this.B8=false;
B.kD([this,this.El],this);}if(this.B8)B.kD([this,this.Jf],this);},Bu:function(C){
if(C===this.String)return;this.String=C;this.AL=B.g3;this.B8=false;B.kD([this,this.
El],this);},Dd:function(C){if(C===this.AR)return;this.AR=C;this.AL=B.g3;this.B8=
false;B.kD([this,this.El],this);},BP:function(C){var A;if((((C===this.Bl)&&(C===
this.Bt))&&(C===this.Br))&&(C===this.Bs))return;this.Bl=C;this.Bt=C;this.Br=C;this.
Bs=C;if(!!this.K&&((this.F&0x1)===0x1))this.K.An(this.N);},L3:function(C){var A;
if(C>255)C=255;if(C<0)C=0;if(C===this.DO)return;this.DO=C;if(!!this.K&&((this.F&
0x1)===0x1))this.K.An(this.N);},FQ:function(C){if(C)this.A1(0x1,0x0);else this.A1(
0x0,0x1);},H_:function(){if(!this.B8)this.Hp(this);if(!this.Cm)return false;var result=
false;var bidi=this.Cm;result=B.pc(bidi);return result;},KT:function(){if(!this.
B8)this.Hp(this);if(!this.Cm)return false;var result=false;var bidi=this.Cm;result=
B.rq(bidi);return result;},E2:function(){var A;if((this.String===B.g3)||!this.AR
)return A3;if(!this.B8)this.Hp(this);if(this.AL===B.g3)return A3;var leading=this.
AR.Leading;var Gx=(this.AR.Ascent+this.AR.Descent)+this.AR.Leading;if(this.EE>0){
leading=(this.EE-this.AR.Ascent)-this.AR.Descent;Gx=this.EE;}if(B.sa(this.CW,Az)
)this.CW=[this.AR.Jx(this.AL),this.CW[1]];this.CW=[this.CW[0],((this.AL.charCodeAt(
0)||0)*Gx)-leading];var Ax=this.DD;var orient=this.DP;var Ah=this.N;var D0=this.
FS;var width=Ah[2]-Ah[0];var height=Ah[3]-Ah[1];if((orient===1)||(orient===3)){width=
height;height=Ah[2]-Ah[0];}var Am=[D0,0,width-D0,height];var P=[].concat(Am.slice(
0,2),B.so(Am.slice(0,2),this.CW));if(((Ax&0x80)===0x80)){if(this.H_())Ax=(Ax&~0x80
)|0x4;else Ax=(Ax&~0x80)|0x1;}if(((Ax&0x40)===0x40)){var CT=this.EG;if(CT<=0)CT=
width-(this.FS*2);if(CT<0)CT=0;if(CT>(P[2]-P[0]))P=B.sU(P,CT);}if((P[2]-P[0])!==(
Am[2]-Am[0])){if(((Ax&0x4)===0x4))P=B.sV(P,Am[2]-(P[2]-P[0]));else if(((Ax&0x2)===
0x2))P=B.sV(P,(Am[0]+(((Am[2]-Am[0])/2)|0))-(((P[2]-P[0])/2)|0));}if((P[3]-P[1])
!==(Am[3]-Am[1])){if(((Ax&0x20)===0x20))P=B.sX(P,Am[3]-(P[3]-P[1]));else if(((Ax&
0x10)===0x10))P=B.sX(P,(Am[1]+(((Am[3]-Am[1])/2)|0))-(((P[3]-P[1])/2)|0));}if(!orient
)P=B.sq(P,Ah.slice(0,2));else if(orient===1){var B7=[Ah[0]+P[1],Ah[3]-P[2]];P=[].
concat(B7,B.so(B7,[this.CW[1],this.CW[0]]));}else if(orient===2){var B7=[Ah[2]-P[
2],Ah[3]-P[3]];P=[].concat(B7,B.so(B7,this.CW));}else if(orient===3){var B7=[Ah[
2]-P[3],Ah[1]+P[0]];P=[].concat(B7,B.so(B7,[this.CW[1],this.CW[0]]));}return B.sq(
P,this.DR);},_Init:function(aArg){B.Core.Bb._Init.call(this,aArg);this.__proto__=
D.Text;},_Done:function(){this.__proto__=D.Text;this.CC();B.Core.Bb._Done.call(this
);},_Mark:function(E){var A;B.Core.Bb._Mark.call(this,E);if((A=this.AR)&&(A._cycle
!=E))A._Mark(A._cycle=E);if((A=this.JW)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E
);},_className:"Views::Text"};D.Nn={Ks:0x1,Kr:0x2,Kt:0x4,Kw:0x8,Kv:0x10,Ku:0x20,
NR:0x40,NS:0x80,NX:0x100};D.NY={Ks:0x1,Kr:0x2,Kt:0x4,Kw:0x8,Kv:0x10,Ku:0x20,MQ:0x40
,MP:0x80};D.DP={Nx:0,NP:1,NN:2,NO:3};
D._Init=function(){D.C5.__proto__=B.Core.Bb;D.Cu.__proto__=B.Core.Bb;D.GO.__proto__=
B.Core.Bb;D.FD.__proto__=B.Core.Bb;D.Text.__proto__=B.Core.Bb;};D.Bw=function(E){
};return D;})();

/* Embedded Wizard */