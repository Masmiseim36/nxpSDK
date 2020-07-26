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
);if(EmWiApp.uj)throw new Error("The unit file 'Views.js' included twice!");EmWiApp.
uj=(function(){var B=EmWiApp;var D={};
var Aw=[0,0];var A1=[0,0,0,0];var BX="\uFEFF";
D.C7={Br:0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bn:0xFFFFFFFF,Ck:function(Aq,aClip
,aOffset,AC,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);AC=AC+1;if(AC<256){
var AZ=this.Bn;var A0=this.Bt;var AX=this.Br;var AY=this.Bs;AZ=(AZ&0x00FFFFFF)|((((
AC*((AZ>>24)&0xFF))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((AC*((A0>>24)&0xFF))>>8
)&0xFF)<<24);AX=(AX&0x00FFFFFF)|((((AC*((AX>>24)&0xFF))>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF
)|((((AC*((AY>>24)&0xFF))>>8)&0xFF)<<24);Aq.FK(aClip,B.tz(this.M,aOffset),AZ,A0,
AY,AX,aBlend);}else Aq.FK(aClip,B.tz(this.M,aOffset),this.Bn,this.Bt,this.Bs,this.
Br,aBlend);},BP:function(C){var A;if((((C===this.Bn)&&(C===this.Bt))&&(C===this.
Br))&&(C===this.Bs))return;this.Bn=C;this.Bt=C;this.Br=C;this.Bs=C;if(!!this.J&&((
this.F&0x1)===0x1))this.J.Ap(this.M);},_Init:function(aArg){B.Core.A5._Init.call(
this,aArg);this.__proto__=D.C7;},_className:"Views::Rectangle"};D.Ct={Width:1,Br:
0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bn:0xFFFFFFFF,Ck:function(Aq,aClip,aOffset
,AC,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);AC=AC+1;if(AC<256){var AZ=
this.Bn;var A0=this.Bt;var AX=this.Br;var AY=this.Bs;AZ=(AZ&0x00FFFFFF)|((((AC*((
AZ>>24)&0xFF))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((AC*((A0>>24)&0xFF))>>8)&0xFF
)<<24);AX=(AX&0x00FFFFFF)|((((AC*((AX>>24)&0xFF))>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF
)|((((AC*((AY>>24)&0xFF))>>8)&0xFF)<<24);Aq.JI(aClip,B.tz(this.M,aOffset),this.Width
,AZ,A0,AY,AX,aBlend);}else Aq.JI(aClip,B.tz(this.M,aOffset),this.Width,this.Bn,this.
Bt,this.Bs,this.Br,aBlend);},Mk:function(C){var A;if(C<0)C=0;if(C===this.Width)return;
this.Width=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},Lh:function(){
return this.Bn;},BP:function(C){var A;if((((C===this.Bn)&&(C===this.Bt))&&(C===this.
Br))&&(C===this.Bs))return;this.Bn=C;this.Bt=C;this.Br=C;this.Bs=C;if(!!this.J&&((
this.F&0x1)===0x1))this.J.Ap(this.M);},_Init:function(aArg){B.Core.A5._Init.call(
this,aArg);this.__proto__=D.Ct;},_className:"Views::Border"};D.G1={timer:null,V:
null,Eb:null,Bz:-1,Br:0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF,Bn:0xFFFFFFFF,JK:0x1F
,DO:0,DT:255,AL:0,IJ:B.qx,CC:false,HS:true,Ck:function(Aq,aClip,aOffset,AC,aBlend
){var A;var Bh=this.DO;if(this.Bz>=0)Bh=this.Bz;if(!this.V||(Bh>=this.V.NoOfFrames
))return;this.V.Update();var AZ=this.Bn;var A0=this.Bt;var AY=this.Bs;var AX=this.
Br;var B7=(((AC+1)*this.DT)>>8)+1;var BJ=this.JK;var L=B.tz(this.M,aOffset);var HJ=
B.tw([L[2]-L[0],L[3]-L[1]],this.IJ);aBlend=aBlend&&((this.F&0x2)===0x2);if(B7<256
){AZ=(AZ&0x00FFFFFF)|((((((AZ>>24)&0xFF)*B7)>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((
A0>>24)&0xFF)*B7)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*B7)>>8)&
0xFF)<<24);AX=(AX&0x00FFFFFF)|((((((AX>>24)&0xFF)*B7)>>8)&0xFF)<<24);}if(((this.
IJ[0]>0)&&(HJ[0]>0))&&!((BJ&0x5)===0x5)){var BZ=((this.V.FrameSize[0]/3)|0)-HJ[0
];if(((BJ&0x1)===0x1)){if(aClip[2]>L[2])aClip=B.t1(aClip,L[2]);if(BZ>0)L=B.t1(L,
L[2]+BZ);BJ=BJ|0x4;}else if(((BJ&0x4)===0x4)){if(aClip[0]<L[0])aClip=[].concat(L[
0],aClip.slice(1,4));if(BZ>0)L=[].concat(L[0]-BZ,L.slice(1,4));BJ=BJ|0x1;}else{if(
aClip[0]<L[0])aClip=[].concat(L[0],aClip.slice(1,4));if(aClip[2]>L[2])aClip=B.t1(
aClip,L[2]);if(BZ>0){L=[].concat(L[0]-((BZ/2)|0),L.slice(1,4));L=B.t1(L,(L[2]+BZ
)-((BZ/2)|0));}BJ=BJ|0x5;}}if(((this.IJ[1]>0)&&(HJ[1]>0))&&!((BJ&0xA)===0xA)){var
BZ=((this.V.FrameSize[1]/3)|0)-HJ[1];if(((BJ&0x2)===0x2)){if(aClip[3]>L[3])aClip=[
].concat(aClip.slice(0,3),L[3]);if(BZ>0)L=[].concat(L.slice(0,3),L[3]+BZ);BJ=BJ|
0x8;}else if(((BJ&0x8)===0x8)){if(aClip[1]<L[1])aClip=B.t3(aClip,L[1]);if(BZ>0)L=
B.t3(L,L[1]-BZ);BJ=BJ|0x2;}else{if(aClip[1]<L[1])aClip=B.t3(aClip,L[1]);if(aClip[
3]>L[3])aClip=[].concat(aClip.slice(0,3),L[3]);if(BZ>0){L=B.t3(L,L[1]-((BZ/2)|0)
);L=[].concat(L.slice(0,3),(L[3]+BZ)-((BZ/2)|0));}BJ=BJ|0xA;}}Aq.KQ(aClip,this.V
,Bh,L,BJ,AZ,A0,AY,AX,aBlend);},GD:function(AV){var A;if(!!this.J&&((this.F&0x1)===
0x1))this.J.Ap(this.M);},CZ:function(AV){var A;var Bh=this.Bz;var Cq=0;if(!!this.
V)Cq=this.V.NoOfFrames*this.V.FrameDelay;if((!!this.timer&&(this.Bz<0))&&(Cq>0))
this.AL=this.timer.AE-(this.DO*this.V.FrameDelay);if(!!this.timer&&(Cq>0)){var Ab=(
this.timer.AE-this.AL)|0;Bh=(Ab/this.V.FrameDelay)|0;if(Ab>=Cq){Bh=Bh%this.V.NoOfFrames;
this.AL=this.timer.AE-(Ab%Cq);if(!this.HS){Bh=this.V.NoOfFrames-1;Cq=0;}}}if(((Bh
!==this.Bz)&&!!this.J)&&((this.F&0x1)===0x1))this.J.Ap(this.M);this.Bz=Bh;if(!Cq&&
!!this.timer){B.sM([this,this.CZ],this.timer,0);this.timer=null;(A=this.Eb)?A[1].
call(A[0],this):null;}},BP:function(C){var A;if((((C===this.Bn)&&(C===this.Bt))&&(
C===this.Br))&&(C===this.Bs))return;this.Bn=C;this.Bt=C;this.Br=C;this.Bs=C;if(!
!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},DS:function(C){var A;if(this.CC===
C)return;this.CC=C;this.Bz=-1;if(!C&&!!this.timer){B.sM([this,this.CZ],this.timer
,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(B.um.Fv);B.sz([this,this.
CZ],this.timer,0);B.lq([this,this.CZ],this);}if(!!this.J&&((this.F&0x1)===0x1))this.
J.Ap(this.M);},EF:function(C){var A;if(C<0)C=0;if((C===this.DO)&&(this.Bz===-1))
return;this.DO=C;if(!this.timer)this.Bz=-1;if(!!this.J&&((this.F&0x1)===0x1))this.
J.Ap(this.M);},Ec:function(C){var A;if(C===this.V)return;if(!!this.V&&this.V.E$)
B.sM([this,this.GD],this.V,0);this.V=C;this.Bz=-1;if(!!C&&C.E$)B.sz([this,this.GD
],C,0);if(this.CC){this.DS(false);this.DS(true);}if(!!this.J&&((this.F&0x1)===0x1
))this.J.Ap(this.M);},_Init:function(aArg){B.Core.A5._Init.call(this,aArg);this.
__proto__=D.G1;},_Mark:function(E){var A;B.Core.A5._Mark.call(this,E);if((A=this.
timer)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.V)&&(A._cycle!=E))A._Mark(A.
_cycle=E);if((A=this.Eb)&&((A=A[0])._cycle!=E))A._Mark(A._cycle=E);},_className:
"Views::Frame"};D.FR={timer:null,V:null,Eb:null,AL:0,Bz:0,Br:0xFFFFFFFF,Bs:0xFFFFFFFF
,Bt:0xFFFFFFFF,Bn:0xFFFFFFFF,DX:B.qx,DH:0x12,DO:0,DT:255,DU:0,CC:false,HS:true,E7:
false,Hb:true,Ck:function(Aq,aClip,aOffset,AC,aBlend){var A;var Bh=this.DO;if(this.
Bz>=0)Bh=this.Bz;if(!this.V||(Bh>=this.V.NoOfFrames))return;this.V.Update();var U=
this.E9();var Y=this.V.FrameSize;var orient=this.DU;if((U[0]>=U[2])||(U[1]>=U[3]
))return;var AZ=this.Bn;var A0=this.Bt;var AY=this.Bs;var AX=this.Br;var B7=(((AC+
1)*this.DT)>>8)+1;aBlend=aBlend&&((this.F&0x2)===0x2);if(B7<256){AZ=(AZ&0x00FFFFFF
)|((((((AZ>>24)&0xFF)*B7)>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*
B7)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*B7)>>8)&0xFF)<<24);AX=(
AX&0x00FFFFFF)|((((((AX>>24)&0xFF)*B7)>>8)&0xFF)<<24);}if(B.tl([U[2]-U[0],U[3]-U[
1]],Y)&&!orient)Aq.Ey(aClip,this.V,Bh,B.tz(this.M,aOffset),B.tw(this.M.slice(0,2
),U.slice(0,2)),AZ,A0,AY,AX,aBlend);else if(!orient)Aq.Mp(aClip,this.V,Bh,B.tz(U
,aOffset),[].concat(Aw,Y),AZ,A0,AY,AX,aBlend,this.Hb);else{var left=U[0]+aOffset[
0];var top=U[1]+aOffset[1];var right=U[2]+aOffset[0];var bottom=U[3]+aOffset[1];
if(orient===1)Aq.IZ(aClip,this.V,Bh,left,bottom,1.000000,left,top,1.000000,right
,top,1.000000,right,bottom,1.000000,[].concat(Aw,Y),AX,AZ,A0,AY,aBlend,this.Hb);
else if(orient===2)Aq.IZ(aClip,this.V,Bh,right,bottom,1.000000,left,bottom,1.000000
,left,top,1.000000,right,top,1.000000,[].concat(Aw,Y),AY,AX,AZ,A0,aBlend,this.Hb
);else if(orient===3)Aq.IZ(aClip,this.V,Bh,right,top,1.000000,right,bottom,1.000000
,left,bottom,1.000000,left,top,1.000000,[].concat(Aw,Y),A0,AY,AX,AZ,aBlend,this.
Hb);}},GD:function(AV){var A;if(((this.E7&&!!this.V)&&(this.V.FrameSize[0]>0))&&(
this.V.FrameSize[1]>0))this.Ah(B.ty(this.E9(),this.DX));if(!!this.J&&((this.F&0x1
)===0x1))this.J.Ap(this.M);},CZ:function(AV){var A;var Bh=this.Bz;var Cq=0;if(!!
this.V)Cq=this.V.NoOfFrames*this.V.FrameDelay;if((!!this.timer&&(this.Bz<0))&&(Cq>
0))this.AL=this.timer.AE-(this.DO*this.V.FrameDelay);if(!!this.timer&&(Cq>0)){var
Ab=(this.timer.AE-this.AL)|0;Bh=(Ab/this.V.FrameDelay)|0;if(Ab>=Cq){Bh=Bh%this.V.
NoOfFrames;this.AL=this.timer.AE-(Ab%Cq);if(!this.HS){Bh=this.V.NoOfFrames-1;Cq=
0;}}}if(((Bh!==this.Bz)&&!!this.J)&&((this.F&0x1)===0x1))this.J.Ap(this.M);this.
Bz=Bh;if(!Cq&&!!this.timer){B.sM([this,this.CZ],this.timer,0);this.timer=null;(A=
this.Eb)?A[1].call(A[0],this):null;}},BP:function(C){var A;if((((C===this.Bn)&&(
C===this.Bt))&&(C===this.Br))&&(C===this.Bs))return;this.Bn=C;this.Bt=C;this.Br=
C;this.Bs=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},DS:function(C){
var A;if(this.CC===C)return;this.CC=C;this.Bz=-1;if(!C&&!!this.timer){B.sM([this
,this.CZ],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(B.um.
Fv);B.sz([this,this.CZ],this.timer,0);B.lq([this,this.CZ],this);}if(!!this.J&&((
this.F&0x1)===0x1))this.J.Ap(this.M);},Cv:function(C){var A;if(C===this.DH)return;
this.DH=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},EF:function(C){var
A;if(C<0)C=0;if((C===this.DO)&&(this.Bz===-1))return;this.DO=C;if(!this.timer)this.
Bz=-1;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},Ec:function(C){var A;
if(C===this.V)return;if(!!this.V&&this.V.E$)B.sM([this,this.GD],this.V,0);this.V=
C;this.Bz=-1;if(!!C&&C.E$)B.sz([this,this.GD],C,0);if(this.CC){this.DS(false);this.
DS(true);}if(((this.E7&&!!C)&&(C.FrameSize[0]>0))&&(C.FrameSize[1]>0))this.Ah(B.
ty(this.E9(),this.DX));if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},E9:
function(){var A;if(!this.V)return A1;var orient=this.DU;var Ax=this.DH;var Y=this.
V.FrameSize;var Af=this.M;var width=Af[2]-Af[0];var height=Af[3]-Af[1];if(!Y[0]||
!Y[1])return A1;if((orient===1)||(orient===3)){width=height;height=Af[2]-Af[0];}
var Al=[0,0,width,height];var P=Al;if(((Ax&0x40)===0x40)){var JB=((((Al[2]-Al[0]
)<<16)+((Y[0]/2)|0))/Y[0])|0;var GM=((((Al[3]-Al[1])<<16)+((Y[1]/2)|0))/Y[1])|0;
var D_=JB;if(GM>D_)D_=GM;P=B.tZ(P,((Y[0]*D_)+32768)>>16);P=B.tW(P,((Y[1]*D_)+32768
)>>16);}else if(((Ax&0x80)===0x80)){var JB=((((Al[2]-Al[0])<<16)+((Y[0]/2)|0))/Y[
0])|0;var GM=((((Al[3]-Al[1])<<16)+((Y[1]/2)|0))/Y[1])|0;var D_=JB;if(GM<D_)D_=GM;
P=B.tZ(P,((Y[0]*D_)+32768)>>16);P=B.tW(P,((Y[1]*D_)+32768)>>16);}else if(!((Ax&0x100
)===0x100))P=B.tY(P,Y);if((P[2]-P[0])!==(Al[2]-Al[0])){if(((Ax&0x4)===0x4))P=B.t0(
P,Al[2]-(P[2]-P[0]));else if(((Ax&0x2)===0x2))P=B.t0(P,(Al[0]+(((Al[2]-Al[0])/2)|
0))-(((P[2]-P[0])/2)|0));}if((P[3]-P[1])!==(Al[3]-Al[1])){if(((Ax&0x20)===0x20))
P=B.t2(P,Al[3]-(P[3]-P[1]));else if(((Ax&0x10)===0x10))P=B.t2(P,(Al[1]+(((Al[3]-
Al[1])/2)|0))-(((P[3]-P[1])/2)|0));}if(!orient)P=B.tz(P,Af.slice(0,2));else if(orient===
1){var B8=[Af[0]+P[1],Af[3]-P[2]];P=[].concat(B8,B.tx(B8,[P[3]-P[1],P[2]-P[0]]));
}else if(orient===2){var B8=[Af[2]-P[2],Af[3]-P[3]];P=[].concat(B8,B.tx(B8,[P[2]-
P[0],P[3]-P[1]]));}else if(orient===3){var B8=[Af[2]-P[3],Af[1]+P[0]];P=[].concat(
B8,B.tx(B8,[P[3]-P[1],P[2]-P[0]]));}return B.tz(P,this.DX);},_Init:function(aArg
){B.Core.A5._Init.call(this,aArg);this.__proto__=D.FR;},_Mark:function(E){var A;
B.Core.A5._Mark.call(this,E);if((A=this.timer)&&(A._cycle!=E))A._Mark(A._cycle=E
);if((A=this.V)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Eb)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);},_className:"Views::Image"};D.Text={AT:null,Ed:null,AN:
B.hm,String:B.hm,Cl:null,CY:B.qx,Gc:0,EI:0,Br:0xFFFFFFFF,Bs:0xFFFFFFFF,Bt:0xFFFFFFFF
,Bn:0xFFFFFFFF,EM:0,DX:B.qx,DH:0x12,DT:255,DU:0,Fh:false,E7:false,Fw:false,HQ:false
,B9:false,Ck:function(Aq,aClip,aOffset,AC,aBlend){var A;if((this.AN===B.hm)||!this.
AT)return;var Ax=this.DH;var orient=this.DU;var font=this.AT;var Al=B.tz(this.M,
aOffset);var AZ=this.Bn;var A0=this.Bt;var AY=this.Bs;var AX=this.Br;var B7=(((AC+
1)*this.DT)>>8)+1;var Dm=this.AN.charCodeAt(0)||0;var U=B.tz(this.E9(),aOffset);
var Cp=[Al[0]-U[0],(Al[1]-U[1])-font.Ascent];if(Dm<1)return;if(B7<256){AZ=(AZ&0x00FFFFFF
)|((((((AZ>>24)&0xFF)*B7)>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*
B7)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*B7)>>8)&0xFF)<<24);AX=(
AX&0x00FFFFFF)|((((((AX>>24)&0xFF)*B7)>>8)&0xFF)<<24);}if(((Ax&0x80)===0x80)){if(
this.Iq())Ax=(Ax&~0x80)|0x4;else Ax=(Ax&~0x80)|0x1;}if(((Dm===1)&&!((Ax&0x40)===
0x40))&&!orient){Aq.JJ(aClip,font,this.AN,2,(this.AN.charCodeAt(1)||0)-1,Al,Cp,0
,orient,AZ,A0,AY,AX,true);return;}var leading=font.Leading;if(this.EI>0)leading=(
this.EI-font.Ascent)-font.Descent;var JA=(font.Ascent+font.Descent)+leading;var Hm=
aClip[1]-U[1];var Hn=aClip[3]-U[1];var Gn=U[2]-U[0];var E6=0;var I=1;var BH=this.
AN.charCodeAt(I)||0;if(orient===1){Cp=[U[3]-Al[3],(Al[0]-U[0])-font.Ascent];Hm=aClip[
0]-U[0];Hn=aClip[2]-U[0];Gn=U[3]-U[1];}else if(orient===2){Cp=[U[2]-Al[2],(U[3]-
Al[3])-font.Ascent];Hm=U[3]-aClip[3];Hn=U[3]-aClip[1];}else if(orient===3){Cp=[Al[
1]-U[1],(U[2]-Al[2])-font.Ascent];Hm=U[2]-aClip[2];Hn=U[2]-aClip[0];Gn=U[3]-U[1];
}while(((E6+JA)<Hm)&&(BH>0)){I=I+BH;E6=E6+JA;BH=this.AN.charCodeAt(I)||0;}while((
E6<Hn)&&(BH>0)){var EX=B.tw(Cp,[0,E6]);var KA=0;var Hw=false;if(((((Ax&0x40)===0x40
)&&((this.AN.charCodeAt((I+BH)-1)||0)!==0x0A))&&((this.AN.charCodeAt(I+1)||0)!==
0x0A))&&((this.AN.charCodeAt(I+BH)||0)!==0x00))Hw=true;if(Hw&&!!(Ax&0x6)){var Kz=
I+BH;var Kl=this.AN.indexOf(String.fromCharCode(0x20),I+1);var Km=this.AN.indexOf(
String.fromCharCode(0xA0),I+1);if(((Kl<0)||(Kl>=Kz))&&((Km<0)||(Km>=Kz)))Hw=false;
}if(Hw)KA=Gn;else if(((Ax&0x4)===0x4))EX=[(EX[0]-Gn)+font.FM(this.AN,I+1,BH-1),EX[
1]];else if(((Ax&0x2)===0x2))EX=[(EX[0]-((Gn/2)|0))+((font.FM(this.AN,I+1,BH-1)/
2)|0),EX[1]];Aq.JJ(aClip,font,this.AN,I+1,BH-1,Al,EX,KA,orient,AZ,A0,AY,AX,true);
I=I+BH;E6=E6+JA;BH=this.AN.charCodeAt(I)||0;}},Ah:function(C){var A;if(B.tm(C,this.
M))return;var Jz=false;if(!this.DU||(this.DU===2))Jz=((A=this.M)[2]-A[0])!==(C[2
]-C[0]);else Jz=((A=this.M)[3]-A[1])!==(C[3]-C[1]);if((((Jz&&!this.EM)&&this.Fh)&&
this.B9)&&!((this.F&0x2000)===0x2000)){this.AN=B.hm;this.B9=false;B.lq([this,this.
Eq],this);}if(((this.Fw&&this.B9)&&!B.tl([(A=this.M)[2]-A[0],A[3]-A[1]],[C[2]-C[
0],C[3]-C[1]]))&&!((this.F&0x2000)===0x2000)){this.AN=B.hm;this.B9=false;B.lq([this
,this.Eq],this);}B.Core.A5.Ah.call(this,C);B.lq([this,this.Jw],this);},CE:function(
){if(!!this.Cl){this.Jc(this.Cl);this.Cl=null;}},Jc:function(aBidi){if(!aBidi)return;
B.ng(aBidi);},MS:function(aSize){var bidi=null;bidi=B.qk(aSize);return bidi;},Jw:
function(AV){B.lq(this.Ed,this);},Eq:function(AV){B.lq([this,this.HD],this);},HD:
function(AV){var A;if(this.B9)return;var orient=this.DU;var width=(A=this.M)[2]-
A[0];var height=(A=this.M)[3]-A[1];var CV=-1;if((orient===1)||(orient===3)){width=
height;height=(A=this.M)[2]-A[0];}if(this.Fh){if(this.EM>0)CV=this.EM;else if(!this.
E7)CV=width-(this.Gc*2);else CV=width;if(CV<0)CV=1;}if(!!this.Cl){this.Jc(this.Cl
);this.Cl=null;}this.B9=true;if((this.String!==B.hm)&&!!this.AT){var length=this.
String.length;if(this.HQ)this.Cl=this.MS(length);this.AN=this.AT.Ml(this.String,
0,CV,length,this.Cl);if(!!this.Cl&&!this.K8()){this.Jc(this.Cl);this.Cl=null;}}else
this.AN=B.hm;this.CY=Aw;if(((this.Fw&&(this.AN!==B.hm))&&!this.E7)&&!!this.AT){var
Ax=this.DH;var font=this.AT;var leading=font.Leading;var AU=this.AN;var HH=this.
Iq();if(((Ax&0x80)===0x80)){if(HH)Ax=(Ax&~0x80)|0x4;else Ax=(Ax&~0x80)|0x1;}if(this.
EI>0)leading=(this.EI-font.Ascent)-font.Descent;var GJ=(font.Ascent+font.Descent
)+leading;var Dm=AU.charCodeAt(0)||0;var En=((height+leading)/GJ)|0;var I_=false;
var Hl=false;if(En<=0)En=1;if(Dm>En){var Db=0;var GK=0;var HG=Dm-1;var Bm=0;var BE=
AU.length;var tmp=B.hm;if(((Ax&0x20)===0x20))GK=Dm-En;else if(((Ax&0x10)===0x10)
){GK=((Dm-En)/2)|0;HG=(GK+En)-1;}else HG=En-1;I_=GK>0;Hl=HG<(Dm-1);for(Bm=1;Db<GK;
Db=Db+1)Bm=Bm+(AU.charCodeAt(Bm)||0);if(Hl)for(BE=Bm;Db<HG;Db=Db+1)BE=BE+(AU.charCodeAt(
BE)||0);if(I_){var BS=AU.charCodeAt(Bm)||0;tmp=(BX+B.t9(AU,Bm,BS))+BX;tmp=B.t4(tmp
,0,(BS+2)&0xFFFF);Bm=Bm+BS;if((tmp.charCodeAt(BS)||0)===0x0A){tmp=B.t4(tmp,BS,0xFEFF
);tmp=B.t4(tmp,BS+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.t4(tmp,2,0xFEFF
);tmp=B.t4(tmp,1,0x0A);}else tmp=B.t4(tmp,1,0xFEFF);}tmp=tmp+B.t9(AU,Bm,BE-Bm);if(
Hl&&(BE>=Bm)){var BS=AU.charCodeAt(BE)||0;var B$=(BX+B.t9(AU,BE,BS))+BX;B$=B.t4(
B$,0,(BS+2)&0xFFFF);B$=B.t4(B$,1,0xFEFF);if((B$.charCodeAt(BS)||0)===0x0A){B$=B.
t4(B$,BS,0xFEFF);B$=B.t4(B$,BS+1,0x0A);}if((B$.charCodeAt(2)||0)===0x0A){B$=B.t4(
B$,2,0xFEFF);B$=B.t4(B$,1,0x0A);}else B$=B.t4(B$,1,0xFEFF);tmp=tmp+B$;}AU=String.
fromCharCode(En&0xFFFF)+tmp;}var Db=0;var Ae=1;var HA=width-(this.Gc*2);if(this.
Fh&&(this.EM>0))HA=this.EM;Dm=AU.charCodeAt(0)||0;for(;Db<Dm;Db=Db+1){var Et=I_&&
!Db;var Eu=Hl&&(Db===(Dm-1));var CN=false;var CO=false;var Fs=HH;if((HH&&Et)&&!Eu
){Et=false;Eu=true;}else if((HH&&Eu)&&!Et){Eu=false;Et=true;}var GN=Ae+1;var BS=
AU.charCodeAt(Ae)||0;var Bm=GN;var BE=(GN+BS)-2;var Ji=-1;var Jj=-1;if(!this.Fh&&(
font.FM(AU,GN,BS-1)>HA)){if(((Ax&0x4)===0x4))CN=true;else if(((Ax&0x2)===0x2)){CN=
true;CO=true;}else CO=true;}if((AU.charCodeAt(Bm)||0)===0x0A)Bm=Bm+1;if((AU.charCodeAt(
BE)||0)===0x0A)BE=BE-1;while(CN&&((AU.charCodeAt(Bm)||0)===0xFEFF))Bm=Bm+1;while(
CO&&((AU.charCodeAt(BE)||0)===0xFEFF))BE=BE-1;CN=CN&&!Eu;CO=CO&&!Et;while((((CN||
CO)||Et)||Eu)&&(Bm<BE)){if((CN&&(Fs||!CO))||Et){if(Ji>0)AU=B.t4(AU,Ji,0xFEFF);AU=
B.t4(AU,Bm,0x2026);Ji=Bm;Bm=Bm+1;Fs=!Fs;Et=false;if(font.FM(AU,GN,BS-1)<=HA){CN=
false;CO=false;}else CN=CN||!CO;}if((CO&&(!Fs||!CN))||Eu){if(Jj>0)AU=B.t4(AU,Jj,
0xFEFF);AU=B.t4(AU,BE,0x2026);Jj=BE;BE=BE-1;Fs=!Fs;Eu=false;if(font.FM(AU,GN,BS-
1)<=HA){CN=false;CO=false;}else CO=CO||!CN;}}Ae=Ae+BS;}this.CY=[font.JN(AU),((AU.
charCodeAt(0)||0)*GJ)-leading];this.AN=AU;}if(this.E7&&(this.AN!==B.hm)){var D7=[
this.Gc,0];if((orient===1)||(orient===3)){D7=[D7[0],D7[0]];D7=[0,D7[1]];}this.F=
this.F|0x2000;this.Ah(B.ty(B.th(this.E9(),D7),this.DX));this.F=this.F&~0x2000;}if(
!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);B.lq([this,this.Jw],this);},J8:
function(C){if(C===this.HQ)return;this.HQ=C;this.AN=B.hm;this.B9=false;B.lq([this
,this.Eq],this);},J7:function(C){if(C===this.Fw)return;this.Fw=C;this.AN=B.hm;this.
B9=false;B.lq([this,this.Eq],this);},IR:function(C){if(C===this.Fh)return;this.Fh=
C;if(this.B9){this.AN=B.hm;this.B9=false;B.lq([this,this.Eq],this);}if(C&&!this.
EM)this.F=this.F&~0x100;else this.F=this.F|0x100;},Cv:function(C){var A;if(C===this.
DH)return;this.DH=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);if(this.
Fw){this.AN=B.hm;this.B9=false;B.lq([this,this.Eq],this);}if(this.B9)B.lq([this,
this.Jw],this);},Bw:function(C){if(C===this.String)return;this.String=C;this.AN=
B.hm;this.B9=false;B.lq([this,this.Eq],this);},De:function(C){if(C===this.AT)return;
this.AT=C;this.AN=B.hm;this.B9=false;B.lq([this,this.Eq],this);},BP:function(C){
var A;if((((C===this.Bn)&&(C===this.Bt))&&(C===this.Br))&&(C===this.Bs))return;this.
Bn=C;this.Bt=C;this.Br=C;this.Bs=C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.
M);},Mh:function(C){var A;if(C>255)C=255;if(C<0)C=0;if(C===this.DT)return;this.DT=
C;if(!!this.J&&((this.F&0x1)===0x1))this.J.Ap(this.M);},Ga:function(C){if(C)this.
A3(0x1,0x0);else this.A3(0x0,0x1);},Iq:function(){if(!this.B9)this.HD(this);if(!
this.Cl)return false;var result=false;var bidi=this.Cl;result=B.qj(bidi);return result;
},K8:function(){if(!this.B9)this.HD(this);if(!this.Cl)return false;var result=false;
var bidi=this.Cl;result=B.sD(bidi);return result;},E9:function(){var A;if((this.
String===B.hm)||!this.AT)return A1;if(!this.B9)this.HD(this);if(this.AN===B.hm)return A1;
var leading=this.AT.Leading;var GJ=(this.AT.Ascent+this.AT.Descent)+this.AT.Leading;
if(this.EI>0){leading=(this.EI-this.AT.Ascent)-this.AT.Descent;GJ=this.EI;}if(B.
tl(this.CY,Aw))this.CY=[this.AT.JN(this.AN),this.CY[1]];this.CY=[this.CY[0],((this.
AN.charCodeAt(0)||0)*GJ)-leading];var Ax=this.DH;var orient=this.DU;var Af=this.
M;var D7=this.Gc;var width=Af[2]-Af[0];var height=Af[3]-Af[1];if((orient===1)||(
orient===3)){width=height;height=Af[2]-Af[0];}var Al=[D7,0,width-D7,height];var P=[
].concat(Al.slice(0,2),B.tx(Al.slice(0,2),this.CY));if(((Ax&0x80)===0x80)){if(this.
Iq())Ax=(Ax&~0x80)|0x4;else Ax=(Ax&~0x80)|0x1;}if(((Ax&0x40)===0x40)){var CV=this.
EM;if(CV<=0)CV=width-(this.Gc*2);if(CV<0)CV=0;if(CV>(P[2]-P[0]))P=B.tZ(P,CV);}if((
P[2]-P[0])!==(Al[2]-Al[0])){if(((Ax&0x4)===0x4))P=B.t0(P,Al[2]-(P[2]-P[0]));else
if(((Ax&0x2)===0x2))P=B.t0(P,(Al[0]+(((Al[2]-Al[0])/2)|0))-(((P[2]-P[0])/2)|0));
}if((P[3]-P[1])!==(Al[3]-Al[1])){if(((Ax&0x20)===0x20))P=B.t2(P,Al[3]-(P[3]-P[1]
));else if(((Ax&0x10)===0x10))P=B.t2(P,(Al[1]+(((Al[3]-Al[1])/2)|0))-(((P[3]-P[1
])/2)|0));}if(!orient)P=B.tz(P,Af.slice(0,2));else if(orient===1){var B8=[Af[0]+
P[1],Af[3]-P[2]];P=[].concat(B8,B.tx(B8,[this.CY[1],this.CY[0]]));}else if(orient===
2){var B8=[Af[2]-P[2],Af[3]-P[3]];P=[].concat(B8,B.tx(B8,this.CY));}else if(orient===
3){var B8=[Af[2]-P[3],Af[1]+P[0]];P=[].concat(B8,B.tx(B8,[this.CY[1],this.CY[0]]
));}return B.tz(P,this.DX);},_Init:function(aArg){B.Core.A5._Init.call(this,aArg
);this.__proto__=D.Text;},_Done:function(){this.CE();this.__proto__=B.Core.A5;B.
Core.A5._Done.call(this);},_Mark:function(E){var A;B.Core.A5._Mark.call(this,E);
if((A=this.AT)&&(A._cycle!=E))A._Mark(A._cycle=E);if((A=this.Ed)&&((A=A[0])._cycle
!=E))A._Mark(A._cycle=E);},_className:"Views::Text"};D.NL={KH:0x1,KG:0x2,KI:0x4,
KL:0x8,KK:0x10,KJ:0x20,Oh:0x40,Oi:0x80,Oo:0x100};D.Op={KH:0x1,KG:0x2,KI:0x4,KL:0x8
,KK:0x10,KJ:0x20,M$:0x40,M_:0x80};D.DU={NX:0,Of:1,Od:2,Oe:3};
D._Init=function(){D.C7.__proto__=B.Core.A5;D.Ct.__proto__=B.Core.A5;D.G1.__proto__=
B.Core.A5;D.FR.__proto__=B.Core.A5;D.Text.__proto__=B.Core.A5;};D.By=function(E){
};return D;})();

/* Embedded Wizard */