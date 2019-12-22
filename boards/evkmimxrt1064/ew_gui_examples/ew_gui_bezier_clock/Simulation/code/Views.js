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
tf=(function(){var B=EmWiApp;var E={};
var AJ=[1,1];var Be=[0,0];var Db=[0,0,0,0];var CD="\uFEFF";
E.Ck={H4:null,MM:1,ML:1,NE:0xFFFFFFFF,ND:0xFFFFFFFF,C7:function(AA,aClip,aOffset,
Ah,aBlend){var A;var Hf=this.ML;var Hg=this.MM;var Ew=B.so(this.BI,aOffset);var Ex=
B.so(this.Bn,aOffset);var Ix=this.ND;var Lv=this.NE;if(B.sa(Ew,Ex))return;aBlend=
aBlend&&((this.F&0x2)===0x2);Ah=Ah+1;if(Ah<256){Ix=(Ix&0x00FFFFFF)|((((Ah*((Ix>>
24)&0xFF))>>8)&0xFF)<<24);Lv=(Lv&0x00FFFFFF)|((((Ah*((Lv>>24)&0xFF))>>8)&0xFF)<<
24);}if((Hf===1)&&(Hg===1))AA.L_(aClip,Ew,Ex,Ix,Lv,aBlend);else AA.R9(aClip,Ew,Ex
,Hf,Hg,Ix,Lv,aBlend);},GetExtent:function(){var Hf=this.ML;var Hg=this.MM;var Ew=
this.BI;var Ex=this.Bn;if(((Hf===1)&&(Hg===1))||B.sa(Ew,Ex))return B.Core.JZ.GetExtent.
call(this);var HN=Hf/2.000000;var HO=Hg/2.000000;var Jp=Ew[0];var Jq=Ew[1];var Jr=
Ex[0];var Js=Ex[1];var D7=Jr-Jp;var D8=Js-Jq;var Cq=Math.sqrt((D7*D7)+(D8*D8));D7=
D7/Cq;D8=D8/Cq;var Ra=Jp+(D8*HN);var Rb=Jq-(D7*HN);var MS=Jr+(D8*HO);var MT=Js-(
D7*HO);var MU=Jr-(D8*HO);var MV=Js+(D7*HO);var MW=Jp-(D8*HN);var MX=Jq+(D7*HN);var
left=Ra;var right=Ra;var top=Rb;var bottom=Rb;if(MS<left)left=MS;if(MU<left)left=
MU;if(MW<left)left=MW;if(MS>right)right=MS;if(MU>right)right=MU;if(MW>right)right=
MW;if(MT<top)top=MT;if(MV<top)top=MV;if(MX<top)top=MX;if(MT>bottom)bottom=MT;if(
MV>bottom)bottom=MV;if(MX>bottom)bottom=MX;var P=[left|0,top|0,right|0,bottom|0];
P=[].concat(P.slice(0,2),B.so(P.slice(2,4),AJ));return P;},IE:function(Ae){var A;
if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.GetExtent());},CB:function(C){var
A;if(C<1)C=1;if((C===this.ML)&&(C===this.MM))return;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ag(this.GetExtent());this.ML=C;this.MM=C;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ag(this.GetExtent());if((C!==1)&&!this.H4){this.H4=B.r0(B.te.MK);if(
this.H4.Ia)B.rl([this,this.IE],this.H4,0);}if((C===1)&&!!this.H4){if(this.H4.Ia)
B.rA([this,this.IE],this.H4,0);this.H4=null;}},Ab:function(C){var A;if((C===this.
ND)&&(C===this.NE))return;this.ND=C;this.NE=C;if(!!this.G&&((this.F&0x1)===0x1))
this.G.Ag(this.GetExtent());},_Init:function(aArg){B.Core.JZ._Init.call(this,aArg
);this.__proto__=E.Ck;},_Mark:function(D){var A;B.Core.JZ._Mark.call(this,D);if((
A=this.H4)&&(A._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Line"};E.Ff={
BL:0xFFFFFFFF,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF,C7:function(AA,aClip,aOffset
,Ah,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Ah=Ah+1;if(Ah<256){var A1=
this.Bz;var A2=this.BN;var AZ=this.BL;var A0=this.BM;A1=(A1&0x00FFFFFF)|((((Ah*((
A1>>24)&0xFF))>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF)|((((Ah*((A2>>24)&0xFF))>>8)&0xFF
)<<24);AZ=(AZ&0x00FFFFFF)|((((Ah*((AZ>>24)&0xFF))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF
)|((((Ah*((A0>>24)&0xFF))>>8)&0xFF)<<24);AA.Mb(aClip,B.sq(this.H,aOffset),A1,A2,
A0,AZ,aBlend);}else AA.Mb(aClip,B.sq(this.H,aOffset),this.Bz,this.BN,this.BM,this.
BL,aBlend);},Ab:function(C){var A;if((((C===this.Bz)&&(C===this.BN))&&(C===this.
BL))&&(C===this.BM))return;this.Bz=C;this.BN=C;this.BL=C;this.BM=C;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ag(this.H);},_Init:function(aArg){B.Core.AY._Init.call(
this,aArg);this.__proto__=E.Ff;},_className:"Views::Rectangle"};E.EM={Width:1,BL:
0xFFFFFFFF,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF,C7:function(AA,aClip,aOffset
,Ah,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Ah=Ah+1;if(Ah<256){var A1=
this.Bz;var A2=this.BN;var AZ=this.BL;var A0=this.BM;A1=(A1&0x00FFFFFF)|((((Ah*((
A1>>24)&0xFF))>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF)|((((Ah*((A2>>24)&0xFF))>>8)&0xFF
)<<24);AZ=(AZ&0x00FFFFFF)|((((Ah*((AZ>>24)&0xFF))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF
)|((((Ah*((A0>>24)&0xFF))>>8)&0xFF)<<24);AA.P3(aClip,B.sq(this.H,aOffset),this.Width
,A1,A2,A0,AZ,aBlend);}else AA.P3(aClip,B.sq(this.H,aOffset),this.Width,this.Bz,this.
BN,this.BM,this.BL,aBlend);},CB:function(C){var A;if(C<0)C=0;if(C===this.Width)return;
this.Width=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},J0:function(){
return this.Bz;},Ab:function(C){var A;if((((C===this.Bz)&&(C===this.BN))&&(C===this.
BL))&&(C===this.BM))return;this.Bz=C;this.BN=C;this.BL=C;this.BM=C;if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ag(this.H);},_Init:function(aArg){B.Core.AY._Init.call(
this,aArg);this.__proto__=E.EM;},_className:"Views::Border"};E.Md={timer:null,An:
null,GO:null,C2:-1,BL:0xFFFFFFFF,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF,GM:0x1F
,Hl:0,C_:255,Bk:0,Ol:B.pm,E8:false,Dh:true,C7:function(AA,aClip,aOffset,Ah,aBlend
){var A;var Cp=this.Hl;if(this.C2>=0)Cp=this.C2;if(!this.An||(Cp>=this.An.NoOfFrames
))return;this.An.Update();var A1=this.Bz;var A2=this.BN;var A0=this.BM;var AZ=this.
BL;var Eb=(((Ah+1)*this.C_)>>8)+1;var Dz=this.GM;var P=B.sq(this.H,aOffset);var Nz=
B.sn([P[2]-P[0],P[3]-P[1]],this.Ol);aBlend=aBlend&&((this.F&0x2)===0x2);if(Eb<256
){A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*Eb)>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF)|((((((
A2>>24)&0xFF)*Eb)>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*Eb)>>8)&
0xFF)<<24);AZ=(AZ&0x00FFFFFF)|((((((AZ>>24)&0xFF)*Eb)>>8)&0xFF)<<24);}if(((this.
Ol[0]>0)&&(Nz[0]>0))&&!((Dz&0x5)===0x5)){var CL=((this.An.FrameSize[0]/3)|0)-Nz[
0];if(((Dz&0x1)===0x1)){if(aClip[2]>P[2])aClip=B.sW(aClip,P[2]);if(CL>0)P=B.sW(P
,P[2]+CL);Dz=Dz|0x4;}else if(((Dz&0x4)===0x4)){if(aClip[0]<P[0])aClip=[].concat(
P[0],aClip.slice(1,4));if(CL>0)P=[].concat(P[0]-CL,P.slice(1,4));Dz=Dz|0x1;}else{
if(aClip[0]<P[0])aClip=[].concat(P[0],aClip.slice(1,4));if(aClip[2]>P[2])aClip=B.
sW(aClip,P[2]);if(CL>0){P=[].concat(P[0]-((CL/2)|0),P.slice(1,4));P=B.sW(P,(P[2]+
CL)-((CL/2)|0));}Dz=Dz|0x5;}}if(((this.Ol[1]>0)&&(Nz[1]>0))&&!((Dz&0xA)===0xA)){
var CL=((this.An.FrameSize[1]/3)|0)-Nz[1];if(((Dz&0x2)===0x2)){if(aClip[3]>P[3])
aClip=[].concat(aClip.slice(0,3),P[3]);if(CL>0)P=[].concat(P.slice(0,3),P[3]+CL);
Dz=Dz|0x8;}else if(((Dz&0x8)===0x8)){if(aClip[1]<P[1])aClip=B.sY(aClip,P[1]);if(
CL>0)P=B.sY(P,P[1]-CL);Dz=Dz|0x2;}else{if(aClip[1]<P[1])aClip=B.sY(aClip,P[1]);if(
aClip[3]>P[3])aClip=[].concat(aClip.slice(0,3),P[3]);if(CL>0){P=B.sY(P,P[1]-((CL
/2)|0));P=[].concat(P.slice(0,3),(P[3]+CL)-((CL/2)|0));}Dz=Dz|0xA;}}AA.R8(aClip,
this.An,Cp,P,Dz,A1,A2,A0,AZ,aBlend);},IE:function(Ae){var A;if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ag(this.H);},Ey:function(Ae){var A;var Cp=this.C2;var EJ=0;
if(!!this.An)EJ=this.An.NoOfFrames*this.An.FrameDelay;if((!!this.timer&&(this.C2<
0))&&(EJ>0))this.Bk=this.timer.Af-(this.Hl*this.An.FrameDelay);if(!!this.timer&&(
EJ>0)){var Au=(this.timer.Af-this.Bk)|0;Cp=(Au/this.An.FrameDelay)|0;if(Au>=EJ){
Cp=Cp%this.An.NoOfFrames;this.Bk=this.timer.Af-(Au%EJ);if(!this.Dh){Cp=this.An.NoOfFrames-
1;EJ=0;}}}if(((Cp!==this.C2)&&!!this.G)&&((this.F&0x1)===0x1))this.G.Ag(this.H);
this.C2=Cp;if(!EJ&&!!this.timer){B.rA([this,this.Ey],this.timer,0);this.timer=null;(
A=this.GO)?A[1].call(A[0],this):null;}},Ab:function(C){var A;if((((C===this.Bz)&&(
C===this.BN))&&(C===this.BL))&&(C===this.BM))return;this.Bz=C;this.BN=C;this.BL=
C;this.BM=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},Ho:function(C){
var A;if(this.E8===C)return;this.E8=C;this.C2=-1;if(!C&&!!this.timer){B.rA([this
,this.Ey],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(B.tg.
JU);B.rl([this,this.Ey],this.timer,0);B.kD([this,this.Ey],this);}if(!!this.G&&((
this.F&0x1)===0x1))this.G.Ag(this.H);},Fe:function(C){var A;if(C<0)C=0;if((C===this.
Hl)&&(this.C2===-1))return;this.Hl=C;if(!this.timer)this.C2=-1;if(!!this.G&&((this.
F&0x1)===0x1))this.G.Ag(this.H);},Dl:function(C){var A;if(C===this.An)return;if(
!!this.An&&this.An.Ia)B.rA([this,this.IE],this.An,0);this.An=C;this.C2=-1;if(!!C&&
C.Ia)B.rl([this,this.IE],C,0);if(this.E8){this.Ho(false);this.Ho(true);}if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},_Init:function(aArg){B.Core.AY._Init.
call(this,aArg);this.__proto__=E.Md;},_Mark:function(D){var A;B.Core.AY._Mark.call(
this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.An)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.GO)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Views::Frame"};E.FK={timer:null,An:null,GO:null,Bk:0,C2:0,BL:0xFFFFFFFF
,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF,AU:B.pm,E7:0x12,Hl:0,C_:255,Hr:0,E8:false
,Dh:true,JO:false,GU:true,C7:function(AA,aClip,aOffset,Ah,aBlend){var A;var Cp=this.
Hl;if(this.C2>=0)Cp=this.C2;if(!this.An||(Cp>=this.An.NoOfFrames))return;this.An.
Update();var S=this.JW();var size=this.An.FrameSize;var orient=this.Hr;if((S[0]>=
S[2])||(S[1]>=S[3]))return;var A1=this.Bz;var A2=this.BN;var A0=this.BM;var AZ=this.
BL;var Eb=(((Ah+1)*this.C_)>>8)+1;aBlend=aBlend&&((this.F&0x2)===0x2);if(Eb<256){
A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*Eb)>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF)|((((((
A2>>24)&0xFF)*Eb)>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*Eb)>>8)&
0xFF)<<24);AZ=(AZ&0x00FFFFFF)|((((((AZ>>24)&0xFF)*Eb)>>8)&0xFF)<<24);}if(B.sa([S[
2]-S[0],S[3]-S[1]],size)&&!orient)AA.L6(aClip,this.An,Cp,B.sq(this.H,aOffset),B.
sn(this.H.slice(0,2),S.slice(0,2)),A1,A2,A0,AZ,aBlend);else if(!orient)AA.TP(aClip
,this.An,Cp,B.sq(S,aOffset),[].concat(Be,size),A1,A2,A0,AZ,aBlend,this.GU);else{
var left=S[0]+aOffset[0];var top=S[1]+aOffset[1];var right=S[2]+aOffset[0];var bottom=
S[3]+aOffset[1];if(orient===1)AA.OH(aClip,this.An,Cp,left,bottom,1.000000,left,top
,1.000000,right,top,1.000000,right,bottom,1.000000,[].concat(Be,size),AZ,A1,A2,A0
,aBlend,this.GU);else if(orient===2)AA.OH(aClip,this.An,Cp,right,bottom,1.000000
,left,bottom,1.000000,left,top,1.000000,right,top,1.000000,[].concat(Be,size),A0
,AZ,A1,A2,aBlend,this.GU);else if(orient===3)AA.OH(aClip,this.An,Cp,right,top,1.000000
,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000,[].concat(Be,size)
,A2,A0,AZ,A1,aBlend,this.GU);}},IE:function(Ae){var A;if(((this.JO&&!!this.An)&&(
this.An.FrameSize[0]>0))&&(this.An.FrameSize[1]>0))this.Q(B.sp(this.JW(),this.AU
));if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},Ey:function(Ae){var A;var
Cp=this.C2;var EJ=0;if(!!this.An)EJ=this.An.NoOfFrames*this.An.FrameDelay;if((!!
this.timer&&(this.C2<0))&&(EJ>0))this.Bk=this.timer.Af-(this.Hl*this.An.FrameDelay
);if(!!this.timer&&(EJ>0)){var Au=(this.timer.Af-this.Bk)|0;Cp=(Au/this.An.FrameDelay
)|0;if(Au>=EJ){Cp=Cp%this.An.NoOfFrames;this.Bk=this.timer.Af-(Au%EJ);if(!this.Dh
){Cp=this.An.NoOfFrames-1;EJ=0;}}}if(((Cp!==this.C2)&&!!this.G)&&((this.F&0x1)===
0x1))this.G.Ag(this.H);this.C2=Cp;if(!EJ&&!!this.timer){B.rA([this,this.Ey],this.
timer,0);this.timer=null;(A=this.GO)?A[1].call(A[0],this):null;}},Ab:function(C){
var A;if((((C===this.Bz)&&(C===this.BN))&&(C===this.BL))&&(C===this.BM))return;this.
Bz=C;this.BN=C;this.BL=C;this.BM=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.
H);},Ho:function(C){var A;if(this.E8===C)return;this.E8=C;this.C2=-1;if(!C&&!!this.
timer){B.rA([this,this.Ey],this.timer,0);this.timer=null;}if(C){this.timer=B._GetAutoObject(
B.tg.JU);B.rl([this,this.Ey],this.timer,0);B.kD([this,this.Ey],this);}if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},BT:function(C){var A;if(B.sa(C,this.
AU))return;this.AU=C;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},ER:function(
C){var A;if(C===this.E7)return;this.E7=C;if(!!this.G&&((this.F&0x1)===0x1))this.
G.Ag(this.H);},Fe:function(C){var A;if(C<0)C=0;if((C===this.Hl)&&(this.C2===-1))
return;this.Hl=C;if(!this.timer)this.C2=-1;if(!!this.G&&((this.F&0x1)===0x1))this.
G.Ag(this.H);},Dl:function(C){var A;if(C===this.An)return;if(!!this.An&&this.An.
Ia)B.rA([this,this.IE],this.An,0);this.An=C;this.C2=-1;if(!!C&&C.Ia)B.rl([this,this.
IE],C,0);if(this.E8){this.Ho(false);this.Ho(true);}if(((this.JO&&!!C)&&(C.FrameSize[
0]>0))&&(C.FrameSize[1]>0))this.Q(B.sp(this.JW(),this.AU));if(!!this.G&&((this.F&
0x1)===0x1))this.G.Ag(this.H);},JW:function(){var A;if(!this.An)return Db;var orient=
this.Hr;var AG=this.E7;var size=this.An.FrameSize;var W=this.H;var width=W[2]-W[
0];var height=W[3]-W[1];if(!size[0]||!size[1])return Db;if((orient===1)||(orient===
3)){width=height;height=W[2]-W[0];}var AK=[0,0,width,height];var Ad=AK;if(((AG&0x40
)===0x40)){var PJ=((((AK[2]-AK[0])<<16)+((size[0]/2)|0))/size[0])|0;var LY=((((AK[
3]-AK[1])<<16)+((size[1]/2)|0))/size[1])|0;var HZ=PJ;if(LY>HZ)HZ=LY;Ad=B.sU(Ad,((
size[0]*HZ)+32768)>>16);Ad=B.sR(Ad,((size[1]*HZ)+32768)>>16);}else if(((AG&0x80)===
0x80)){var PJ=((((AK[2]-AK[0])<<16)+((size[0]/2)|0))/size[0])|0;var LY=((((AK[3]-
AK[1])<<16)+((size[1]/2)|0))/size[1])|0;var HZ=PJ;if(LY<HZ)HZ=LY;Ad=B.sU(Ad,((size[
0]*HZ)+32768)>>16);Ad=B.sR(Ad,((size[1]*HZ)+32768)>>16);}else if(!((AG&0x100)===
0x100))Ad=B.sT(Ad,size);if((Ad[2]-Ad[0])!==(AK[2]-AK[0])){if(((AG&0x4)===0x4))Ad=
B.sV(Ad,AK[2]-(Ad[2]-Ad[0]));else if(((AG&0x2)===0x2))Ad=B.sV(Ad,(AK[0]+(((AK[2]-
AK[0])/2)|0))-(((Ad[2]-Ad[0])/2)|0));}if((Ad[3]-Ad[1])!==(AK[3]-AK[1])){if(((AG&
0x20)===0x20))Ad=B.sX(Ad,AK[3]-(Ad[3]-Ad[1]));else if(((AG&0x10)===0x10))Ad=B.sX(
Ad,(AK[1]+(((AK[3]-AK[1])/2)|0))-(((Ad[3]-Ad[1])/2)|0));}if(!orient)Ad=B.sq(Ad,W.
slice(0,2));else if(orient===1){var Ec=[W[0]+Ad[1],W[3]-Ad[2]];Ad=[].concat(Ec,B.
so(Ec,[Ad[3]-Ad[1],Ad[2]-Ad[0]]));}else if(orient===2){var Ec=[W[2]-Ad[2],W[3]-Ad[
3]];Ad=[].concat(Ec,B.so(Ec,[Ad[2]-Ad[0],Ad[3]-Ad[1]]));}else if(orient===3){var
Ec=[W[2]-Ad[3],W[1]+Ad[0]];Ad=[].concat(Ec,B.so(Ec,[Ad[3]-Ad[1],Ad[2]-Ad[0]]));}
return B.sq(Ad,this.AU);},Tc:function(){return this.AU;},_Init:function(aArg){B.
Core.AY._Init.call(this,aArg);this.__proto__=E.FK;},_Mark:function(D){var A;B.Core.
AY._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=
this.An)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.GO)&&((A=A[0])._cycle!=D)
)A._Mark(A._cycle=D);},_className:"Views::Image"};E.Text={BR:null,FU:null,EG:null
,FD:B.pm,Bv:B.g3,Lc:0,I5:0,BL:0xFFFFFFFF,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF
,I8:0,AU:B.pm,E7:0x12,String:B.g3,C_:255,Hr:0,Kj:false,JO:false,KO:false,NI:false
,DR:false,C7:function(AA,aClip,aOffset,Ah,aBlend){var A;if((this.Bv===B.g3)||!this.
BR)return;var AG=this.E7;var orient=this.Hr;var font=this.BR;var AK=B.sq(this.H,
aOffset);var A1=this.Bz;var A2=this.BN;var A0=this.BM;var AZ=this.BL;var Eb=(((Ah+
1)*this.C_)>>8)+1;var GG=this.Bv.charCodeAt(0)||0;var S=B.sq(this.JW(),aOffset);
var BH=[AK[0]-S[0],(AK[1]-S[1])-font.Ascent];if(GG<1)return;if(Eb<256){A1=(A1&0x00FFFFFF
)|((((((A1>>24)&0xFF)*Eb)>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF)|((((((A2>>24)&0xFF)*
Eb)>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*Eb)>>8)&0xFF)<<24);AZ=(
AZ&0x00FFFFFF)|((((((AZ>>24)&0xFF)*Eb)>>8)&0xFF)<<24);}if(((AG&0x80)===0x80)){if(
this.N2())AG=(AG&~0x80)|0x4;else AG=(AG&~0x80)|0x1;}if(((GG===1)&&!((AG&0x40)===
0x40))&&!orient){AA.P4(aClip,font,this.Bv,2,(this.Bv.charCodeAt(1)||0)-1,AK,BH,0
,orient,A1,A2,A0,AZ,true);return;}var leading=font.Leading;if(this.I5>0)leading=(
this.I5-font.Ascent)-font.Descent;var PH=(font.Ascent+font.Descent)+leading;var M1=
aClip[1]-S[1];var M2=aClip[3]-S[1];var Ls=S[2]-S[0];var F$=0;var I=1;var B8=this.
Bv.charCodeAt(I)||0;if(orient===1){BH=[S[3]-AK[3],(AK[0]-S[0])-font.Ascent];M1=aClip[
0]-S[0];M2=aClip[2]-S[0];Ls=S[3]-S[1];}else if(orient===2){BH=[S[2]-AK[2],(S[3]-
AK[3])-font.Ascent];M1=S[3]-aClip[3];M2=S[3]-aClip[1];}else if(orient===3){BH=[AK[
1]-S[1],(S[2]-AK[2])-font.Ascent];M1=S[2]-aClip[2];M2=S[2]-aClip[0];Ls=S[3]-S[1];
}while(((F$+PH)<M1)&&(B8>0)){I=I+B8;F$=F$+PH;B8=this.Bv.charCodeAt(I)||0;}while((
F$<M2)&&(B8>0)){var Jy=B.sn(BH,[0,F$]);var RG=0;var Ne=false;if(((((AG&0x40)===0x40
)&&((this.Bv.charCodeAt((I+B8)-1)||0)!==0x0A))&&((this.Bv.charCodeAt(I+1)||0)!==
0x0A))&&((this.Bv.charCodeAt(I+B8)||0)!==0x00))Ne=true;if(Ne&&!!(AG&0x6)){var RF=
I+B8;var Q_=this.Bv.indexOf(String.fromCharCode(0x20),I+1);var Q$=this.Bv.indexOf(
String.fromCharCode(0xA0),I+1);if(((Q_<0)||(Q_>=RF))&&((Q$<0)||(Q$>=RF)))Ne=false;
}if(Ne)RG=Ls;else if(((AG&0x4)===0x4))Jy=[(Jy[0]-Ls)+font.KY(this.Bv,I+1,B8-1),Jy[
1]];else if(((AG&0x2)===0x2))Jy=[(Jy[0]-((Ls/2)|0))+((font.KY(this.Bv,I+1,B8-1)/
2)|0),Jy[1]];AA.P4(aClip,font,this.Bv,I+1,B8-1,AK,Jy,RG,orient,A1,A2,A0,AZ,true);
I=I+B8;F$=F$+PH;B8=this.Bv.charCodeAt(I)||0;}},Q:function(C){var A;if(B.sb(C,this.
H))return;var PG=false;if(!this.Hr||(this.Hr===2))PG=((A=this.H)[2]-A[0])!==(C[2
]-C[0]);else PG=((A=this.H)[3]-A[1])!==(C[3]-C[1]);if((((PG&&!this.I8)&&this.Kj)&&
this.DR)&&!((this.F&0x2000)===0x2000)){this.Bv=B.g3;this.DR=false;B.kD([this,this.
II],this);}if(((this.KO&&this.DR)&&!B.sa([(A=this.H)[2]-A[0],A[3]-A[1]],[C[2]-C[
0],C[3]-C[1]]))&&!((this.F&0x2000)===0x2000)){this.Bv=B.g3;this.DR=false;B.kD([this
,this.II],this);}B.Core.AY.Q.call(this,C);B.kD([this,this.Nq],this);},DX:function(
){if(!!this.EG){this.Pa(this.EG);this.EG=null;}},Pa:function(aBidi){if(!aBidi)return;
B.mg(aBidi);},U$:function(aSize){var bidi=null;bidi=B.pd(aSize);return bidi;},Nq:
function(Ae){B.kD(this.FU,this);},II:function(Ae){B.kD([this,this.Ns],this);},Ns:
function(Ae){var A;if(this.DR)return;var orient=this.Hr;var width=(A=this.H)[2]-
A[0];var height=(A=this.H)[3]-A[1];var Fz=-1;if((orient===1)||(orient===3)){width=
height;height=(A=this.H)[2]-A[0];}if(this.Kj){if(this.I8>0)Fz=this.I8;else if(!this.
JO)Fz=width-(this.Lc*2);else Fz=width;if(Fz<0)Fz=1;}if(!!this.EG);{this.Pa(this.
EG);this.EG=null;}this.DR=true;if((this.String!==B.g3)&&!!this.BR){var length=this.
String.length;if(this.NI)this.EG=this.U$(length);this.Bv=this.BR.TI(this.String,
0,Fz,length,this.EG);if(!!this.EG&&!this.SM()){this.Pa(this.EG);this.EG=null;}}else
this.Bv=B.g3;this.FD=Be;if((this.KO&&(this.Bv!==B.g3))&&!this.JO){var AG=this.E7;
var font=this.BR;var leading=font.Leading;var BK=this.Bv;var Nx=this.N2();if(((AG&
0x80)===0x80)){if(Nx)AG=(AG&~0x80)|0x4;else AG=(AG&~0x80)|0x1;}if(this.I5>0)leading=(
this.I5-font.Ascent)-font.Descent;var LW=(font.Ascent+font.Descent)+leading;var GG=
BK.charCodeAt(0)||0;var IB=((height+leading)/LW)|0;var O9=false;var M0=false;if(
IB<=0)IB=1;if(GG>IB){var F_=0;var LX=0;var Nw=GG-1;var Cy=0;var Dd=BK.length;var
tmp=B.g3;if(((AG&0x20)===0x20))LX=GG-IB;else if(((AG&0x10)===0x10)){LX=((GG-IB)/
2)|0;Nw=(LX+IB)-1;}else Nw=IB-1;O9=LX>0;M0=Nw<(GG-1);for(Cy=1;F_<LX;F_=F_+1)Cy=Cy+(
BK.charCodeAt(Cy)||0);if(M0)for(Dd=Cy;F_<Nw;F_=F_+1)Dd=Dd+(BK.charCodeAt(Dd)||0);
if(O9){var Cq=BK.charCodeAt(Cy)||0;tmp=(CD+B.s4(BK,Cy,Cq))+CD;tmp=B.sZ(tmp,0,(Cq+
2)&0xFFFF);Cy=Cy+Cq;if((tmp.charCodeAt(Cq)||0)===0x0A){tmp=B.sZ(tmp,Cq,0xFEFF);tmp=
B.sZ(tmp,Cq+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.sZ(tmp,2,0xFEFF);tmp=
B.sZ(tmp,1,0x0A);}else tmp=B.sZ(tmp,1,0xFEFF);}tmp=tmp+B.s4(BK,Cy,Dd-Cy);if(M0&&(
Dd>=Cy)){var Cq=BK.charCodeAt(Dd)||0;var Eh=(CD+B.s4(BK,Dd,Cq))+CD;Eh=B.sZ(Eh,0,(
Cq+2)&0xFFFF);Eh=B.sZ(Eh,1,0xFEFF);if((Eh.charCodeAt(Cq)||0)===0x0A){Eh=B.sZ(Eh,
Cq,0xFEFF);Eh=B.sZ(Eh,Cq+1,0x0A);}if((Eh.charCodeAt(2)||0)===0x0A){Eh=B.sZ(Eh,2,
0xFEFF);Eh=B.sZ(Eh,1,0x0A);}else Eh=B.sZ(Eh,1,0xFEFF);tmp=tmp+Eh;}BK=String.fromCharCode(
IB&0xFFFF)+tmp;}var F_=0;var DQ=1;var Ni=width-(this.Lc*2);if(this.Kj&&(this.I8>
0))Ni=this.I8;GG=BK.charCodeAt(0)||0;for(;F_<GG;F_=F_+1){var IK=O9&&!F_;var IL=M0&&(
F_===(GG-1));var Fp=false;var Fq=false;var Kx=Nx;if((Nx&&IK)&&!IL){IK=false;IL=true;
}else if((Nx&&IL)&&!IK){IL=false;IK=true;}var IM=DQ+1;var Cq=BK.charCodeAt(DQ)||
0;var Cy=IM;var Dd=(IM+Cq)-2;var Ph=-1;var Pi=-1;if(!this.Kj&&(font.KY(BK,IM,Cq-
1)>Ni)){if(((AG&0x4)===0x4))Fp=true;else if(((AG&0x2)===0x2)){Fp=true;Fq=true;}else
Fq=true;}if((BK.charCodeAt(Cy)||0)===0x0A)Cy=Cy+1;if((BK.charCodeAt(Dd)||0)===0x0A
)Dd=Dd-1;while(Fp&&((BK.charCodeAt(Cy)||0)===0xFEFF))Cy=Cy+1;while(Fq&&((BK.charCodeAt(
Dd)||0)===0xFEFF))Dd=Dd-1;Fp=Fp&&!IL;Fq=Fq&&!IK;while((((Fp||Fq)||IK)||IL)&&(Cy<
Dd)){if((Fp&&(Kx||!Fq))||IK){if(Ph>0)BK=B.sZ(BK,Ph,0xFEFF);BK=B.sZ(BK,Cy,0x2026);
Ph=Cy;Cy=Cy+1;Kx=!Kx;IK=false;if(font.KY(BK,IM,Cq-1)<=Ni){Fp=false;Fq=false;}else
Fp=Fp||!Fq;}if((Fq&&(!Kx||!Fp))||IL){if(Pi>0)BK=B.sZ(BK,Pi,0xFEFF);BK=B.sZ(BK,Dd
,0x2026);Pi=Dd;Dd=Dd-1;Kx=!Kx;IL=false;if(font.KY(BK,IM,Cq-1)<=Ni){Fp=false;Fq=false;
}else Fq=Fq||!Fp;}}DQ=DQ+Cq;}this.FD=[font.P8(BK),((BK.charCodeAt(0)||0)*LW)-leading
];this.Bv=BK;}if(this.JO&&(this.Bv!==B.g3)){var HU=[this.Lc,0];if((orient===1)||(
orient===3)){HU=[HU[0],HU[0]];HU=[0,HU[1]];}this.F=this.F|0x2000;this.Q(B.sp(B.r7(
this.JW(),HU),this.AU));this.F=this.F&~0x2000;}if(!!this.G&&((this.F&0x1)===0x1)
)this.G.Ag(this.H);B.kD([this,this.Nq],this);},Qz:function(C){if(C===this.NI)return;
this.NI=C;this.Bv=B.g3;this.DR=false;B.kD([this,this.II],this);},Qy:function(C){
if(C===this.KO)return;this.KO=C;this.Bv=B.g3;this.DR=false;B.kD([this,this.II],this
);},QK:function(C){if(C===this.Kj)return;this.Kj=C;if(this.DR){this.Bv=B.g3;this.
DR=false;B.kD([this,this.II],this);}if(C&&!this.I8)this.F=this.F&~0x100;else this.
F=this.F|0x100;},BT:function(C){var A;if(B.sa(C,this.AU))return;this.AU=C;if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.H);if(this.DR)B.kD([this,this.Nq],this);}
,ER:function(C){var A;if(C===this.E7)return;this.E7=C;if(!!this.G&&((this.F&0x1)===
0x1))this.G.Ag(this.H);if(this.KO){this.Bv=B.g3;this.DR=false;B.kD([this,this.II
],this);}if(this.DR)B.kD([this,this.Nq],this);},Cn:function(C){if(C===this.String
)return;this.String=C;this.Bv=B.g3;this.DR=false;B.kD([this,this.II],this);},Fd:
function(C){if(C===this.BR)return;this.BR=C;this.Bv=B.g3;this.DR=false;B.kD([this
,this.II],this);},Ab:function(C){var A;if((((C===this.Bz)&&(C===this.BN))&&(C===
this.BL))&&(C===this.BM))return;this.Bz=C;this.BN=C;this.BL=C;this.BM=C;if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.H);},GQ:function(C){var A;if(C>255)C=255;
if(C<0)C=0;if(C===this.C_)return;this.C_=C;if(!!this.G&&((this.F&0x1)===0x1))this.
G.Ag(this.H);},N2:function(){if(!this.DR)this.Ns(this);if(!this.EG)return false;
var result=false;var bidi=this.EG;result=B.pc(bidi);return result;},SM:function(
){if(!this.DR)this.Ns(this);if(!this.EG)return false;var result=false;var bidi=this.
EG;result=B.rq(bidi);return result;},JW:function(){var A;if((this.String===B.g3)||
!this.BR)return Db;if(!this.DR)this.Ns(this);if(this.Bv===B.g3)return Db;var leading=
this.BR.Leading;var LW=(this.BR.Ascent+this.BR.Descent)+this.BR.Leading;if(this.
I5>0){leading=(this.I5-this.BR.Ascent)-this.BR.Descent;LW=this.I5;}if(B.sa(this.
FD,Be))this.FD=[this.BR.P8(this.Bv),this.FD[1]];this.FD=[this.FD[0],((this.Bv.charCodeAt(
0)||0)*LW)-leading];var AG=this.E7;var orient=this.Hr;var W=this.H;var HU=this.Lc;
var width=W[2]-W[0];var height=W[3]-W[1];if((orient===1)||(orient===3)){width=height;
height=W[2]-W[0];}var AK=[HU,0,width-HU,height];var Ad=[].concat(AK.slice(0,2),B.
so(AK.slice(0,2),this.FD));if(((AG&0x80)===0x80)){if(this.N2())AG=(AG&~0x80)|0x4;
else AG=(AG&~0x80)|0x1;}if(((AG&0x40)===0x40)){var Fz=this.I8;if(Fz<=0)Fz=width-(
this.Lc*2);if(Fz<0)Fz=0;if(Fz>(Ad[2]-Ad[0]))Ad=B.sU(Ad,Fz);}if((Ad[2]-Ad[0])!==(
AK[2]-AK[0])){if(((AG&0x4)===0x4))Ad=B.sV(Ad,AK[2]-(Ad[2]-Ad[0]));else if(((AG&0x2
)===0x2))Ad=B.sV(Ad,(AK[0]+(((AK[2]-AK[0])/2)|0))-(((Ad[2]-Ad[0])/2)|0));}if((Ad[
3]-Ad[1])!==(AK[3]-AK[1])){if(((AG&0x20)===0x20))Ad=B.sX(Ad,AK[3]-(Ad[3]-Ad[1]));
else if(((AG&0x10)===0x10))Ad=B.sX(Ad,(AK[1]+(((AK[3]-AK[1])/2)|0))-(((Ad[3]-Ad[
1])/2)|0));}if(!orient)Ad=B.sq(Ad,W.slice(0,2));else if(orient===1){var Ec=[W[0]+
Ad[1],W[3]-Ad[2]];Ad=[].concat(Ec,B.so(Ec,[this.FD[1],this.FD[0]]));}else if(orient===
2){var Ec=[W[2]-Ad[2],W[3]-Ad[3]];Ad=[].concat(Ec,B.so(Ec,this.FD));}else if(orient===
3){var Ec=[W[2]-Ad[3],W[1]+Ad[0]];Ad=[].concat(Ec,B.so(Ec,[this.FD[1],this.FD[0]
]));}return B.sq(Ad,this.AU);},_Init:function(aArg){B.Core.AY._Init.call(this,aArg
);this.__proto__=E.Text;},_Done:function(){this.__proto__=E.Text;this.DX();B.Core.
AY._Done.call(this);},_Mark:function(D){var A;B.Core.AY._Mark.call(this,D);if((A=
this.BR)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.FU)&&((A=A[0])._cycle!=D)
)A._Mark(A._cycle=D);},_className:"Views::Text"};E.B1={AH:null,AT:null,BL:0xFFFFFFFF
,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF,As:B.pm,NT:0,Ch:false,GU:true,KW:false
,C7:function(AA,aClip,aOffset,Ah,aBlend){if(!this.AH||(this.Ch&&!this.AT))return;
var Gx=null;var A1=this.Bz;var A2=this.BN;var AZ=this.BL;var A0=this.BM;if(this.
Ch){Gx=B._NewObject(B.Graphics.Canvas,0);Gx.AttachBitmap(this.AT);}Ah=Ah+1;if(Ah<
256){A1=(A1&0x00FFFFFF)|((((Ah*((A1>>24)&0xFF))>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF
)|((((Ah*((A2>>24)&0xFF))>>8)&0xFF)<<24);AZ=(AZ&0x00FFFFFF)|((((Ah*((AZ>>24)&0xFF
))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((Ah*((A0>>24)&0xFF))>>8)&0xFF)<<24);}if(
!Gx)AA.B1(aClip,this.AH,B.sq(this.H,aOffset),this.KW,this.As,A1,A2,A0,AZ,true,this.
GU,this.NT);else{AA.L6(aClip,Gx,0,B.sq(this.H,aOffset),Be,A1,A2,A0,AZ,true);Gx.DetachBitmap(
);}},Q:function(C){var A;if(this.Ch&&!B.sa([(A=this.H)[2]-A[0],A[3]-A[1]],[C[2]-
C[0],C[3]-C[1]]))B.kD([this,this.GK],this);B.Core.AY.Q.call(this,C);},DX:function(
){this.Lx();},Lx:function(){if(!this.AT)return;var handle=this.AT;B.rK(handle);this.
AT=null;},GK:function(Ae){var A;this.Lx();if(this.Ch&&!!this.AH){var handle=null;
var path=this.AH.path;var size=[(A=this.H)[2]-A[0],A[3]-A[1]];var quality=this.GU;
var flipY=this.KW;var offset=this.As;var nzwind=this.NT===1;handle=B.pg(size,path
,flipY,offset,quality,nzwind);this.AT=handle;}},Ei:function(Ae){var A;if(!!this.
G&&((this.F&0x1)===0x1))this.G.Ag(this.H);if(this.Ch)B.kD([this,this.GK],this);}
,Ab:function(C){var A;if((((C===this.Bz)&&(C===this.BN))&&(C===this.BL))&&(C===this.
BM))return;this.Bz=C;this.BN=C;this.BL=C;this.BM=C;if((!!this.G&&((this.F&0x1)===
0x1))&&!!this.AH)this.G.Ag(this.H);},En:function(C){var A;if(B.sa(C,this.As))return;
this.As=C;if((!!this.G&&((this.F&0x1)===0x1))&&!!this.AH)this.G.Ag(this.H);if(this.
Ch)B.kD([this,this.GK],this);},EB:function(C){var A;if(C===this.AH)return;if(!!this.
AH)B.rA([this,this.Ei],this.AH,0);this.AH=C;if(!!this.AH)B.rl([this,this.Ei],this.
AH,0);if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);if(this.Ch)B.kD([this,
this.GK],this);},_Init:function(aArg){B.Core.AY._Init.call(this,aArg);this.__proto__=
E.B1;},_Done:function(){this.__proto__=E.B1;this.DX();B.Core.AY._Done.call(this);
},_Mark:function(D){var A;B.Core.AY._Mark.call(this,D);if((A=this.AH)&&(A._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::FillPath"};E.Cu={AH:null,AT:null,BL:
0xFFFFFFFF,BM:0xFFFFFFFF,BN:0xFFFFFFFF,Bz:0xFFFFFFFF,As:B.pm,Qj:3.000000,Width:1.000000
,QQ:0,P5:0,Mm:0,Ch:false,GU:true,KW:false,C7:function(AA,aClip,aOffset,Ah,aBlend
){if(!this.AH||(this.Ch&&!this.AT))return;var Gx=null;var A1=this.Bz;var A2=this.
BN;var AZ=this.BL;var A0=this.BM;if(this.Ch){Gx=B._NewObject(B.Graphics.Canvas,0
);Gx.AttachBitmap(this.AT);}Ah=Ah+1;if(Ah<256){A1=(A1&0x00FFFFFF)|((((Ah*((A1>>24
)&0xFF))>>8)&0xFF)<<24);A2=(A2&0x00FFFFFF)|((((Ah*((A2>>24)&0xFF))>>8)&0xFF)<<24
);AZ=(AZ&0x00FFFFFF)|((((Ah*((AZ>>24)&0xFF))>>8)&0xFF)<<24);A0=(A0&0x00FFFFFF)|((((
Ah*((A0>>24)&0xFF))>>8)&0xFF)<<24);}if(!Gx)AA.Cu(aClip,this.AH,B.sq(this.H,aOffset
),this.KW,this.As,this.Width,this.QQ,this.P5,this.Mm,this.Qj,A1,A2,A0,AZ,true,this.
GU);else{AA.L6(aClip,Gx,0,B.sq(this.H,aOffset),Be,A1,A2,A0,AZ,true);Gx.DetachBitmap(
);}},Q:function(C){var A;if(this.Ch&&!B.sa([(A=this.H)[2]-A[0],A[3]-A[1]],[C[2]-
C[0],C[3]-C[1]]))B.kD([this,this.GK],this);B.Core.AY.Q.call(this,C);},DX:function(
){this.Lx();},Lx:function(){if(!this.AT)return;var handle=this.AT;B.rK(handle);this.
AT=null;},GK:function(Ae){var A;this.Lx();if(this.Ch&&!!this.AH){var handle=null;
var path=this.AH.path;var size=[(A=this.H)[2]-A[0],A[3]-A[1]];var quality=this.GU;
var flipY=this.KW;var offset=this.As;var width=this.Width;var miterLimit=this.Qj;
var style=0;switch(this.QQ){case 1:style=style|1;break;case 3:style=style|2;break;
case 2:style=style|3;break;default:;}switch(this.P5){case 1:style=style|256;break;
case 3:style=style|512;break;case 2:style=style|768;break;default:;}switch(this.
Mm){case 1:style=style|65536;break;case 2:style=style|131072;break;default:;}handle=
B.ph(size,path,flipY,offset,width,style,miterLimit,quality);this.AT=handle;}},Ei:
function(Ae){var A;if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);if(this.Ch
)B.kD([this,this.GK],this);},J0:function(){return this.Bz;},Ab:function(C){var A;
if((((C===this.Bz)&&(C===this.BN))&&(C===this.BL))&&(C===this.BM))return;this.Bz=
C;this.BN=C;this.BL=C;this.BM=C;if((!!this.G&&((this.F&0x1)===0x1))&&!!this.AH)this.
G.Ag(this.H);},En:function(C){var A;if(B.sa(C,this.As))return;this.As=C;if((!!this.
G&&((this.F&0x1)===0x1))&&!!this.AH)this.G.Ag(this.H);if(this.Ch)B.kD([this,this.
GK],this);},Tx:function(C){var A;if(C===this.Mm)return;this.Mm=C;if((!!this.G&&((
this.F&0x1)===0x1))&&!!this.AH)this.G.Ag(this.H);if(this.Ch)B.kD([this,this.GK],
this);},CB:function(C){var A;if(C<0.000000)C=0.000000;if(C===this.Width)return;this.
Width=C;if((!!this.G&&((this.F&0x1)===0x1))&&!!this.AH)this.G.Ag(this.H);if(this.
Ch)B.kD([this,this.GK],this);},EB:function(C){var A;if(C===this.AH)return;if(!!this.
AH)B.rA([this,this.Ei],this.AH,0);this.AH=C;if(!!this.AH)B.rl([this,this.Ei],this.
AH,0);if(!!this.G&&((this.F&0x1)===0x1))this.G.Ag(this.H);if(this.Ch)B.kD([this,
this.GK],this);},_Init:function(aArg){B.Core.AY._Init.call(this,aArg);this.__proto__=
E.Cu;},_Done:function(){this.__proto__=E.Cu;this.DX();B.Core.AY._Done.call(this);
},_Mark:function(D){var A;B.Core.AY._Mark.call(this,D);if((A=this.AH)&&(A._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::StrokePath"};E.Wz={PR:0x1,PQ:0x2,PS:
0x4,PV:0x8,PU:0x10,PT:0x20,W4:0x40,W5:0x80,W$:0x100};E.Xa={PR:0x1,PQ:0x2,PS:0x4,
PV:0x8,PU:0x10,PT:0x20,VX:0x40,VW:0x80};E.Hr={WI:0,W2:1,W0:2,W1:3};
E._Init=function(){E.Ck.__proto__=B.Core.JZ;E.Ff.__proto__=B.Core.AY;E.EM.__proto__=
B.Core.AY;E.Md.__proto__=B.Core.AY;E.FK.__proto__=B.Core.AY;E.Text.__proto__=B.Core.
AY;E.B1.__proto__=B.Core.AY;E.Cu.__proto__=B.Core.AY;};E.Dy=function(D){};return E;
})();

/* Embedded Wizard */