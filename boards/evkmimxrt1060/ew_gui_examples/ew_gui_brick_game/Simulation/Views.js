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
var At=[0,0];var AT=[0,0,0,0];var Cq="\uFEFF";
C.CQ={BI:0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF,CL:function(AC,aClip
,aOffset,Ax,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Ax=Ax+1;if(Ax<256){
var A0=this.Bv;var A1=this.BK;var AY=this.BI;var AZ=this.BJ;A0=(A0&0x00FFFFFF)|((((
Ax*((A0>>24)&0xFF))>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((Ax*((A1>>24)&0xFF))>>8
)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((Ax*((AY>>24)&0xFF))>>8)&0xFF)<<24);AZ=(AZ&0x00FFFFFF
)|((((Ax*((AZ>>24)&0xFF))>>8)&0xFF)<<24);AC.GH(aClip,B.sq(this.G,aOffset),A0,A1,
AZ,AY,aBlend);}else AC.GH(aClip,B.sq(this.G,aOffset),this.Bv,this.BK,this.BJ,this.
BI,aBlend);},Ce:function(E){var A;if((((E===this.Bv)&&(E===this.BK))&&(E===this.
BI))&&(E===this.BJ))return;this.Bv=E;this.BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((
this.F&0x1)===0x1))this.K.Aq(this.G);},_Init:function(aArg){B.Core.Bg._Init.call(
this,aArg);this.__proto__=C.CQ;},_className:"Views::Rectangle"};C.Cn={Width:1,BI:
0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF,CL:function(AC,aClip,aOffset
,Ax,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Ax=Ax+1;if(Ax<256){var A0=
this.Bv;var A1=this.BK;var AY=this.BI;var AZ=this.BJ;A0=(A0&0x00FFFFFF)|((((Ax*((
A0>>24)&0xFF))>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((Ax*((A1>>24)&0xFF))>>8)&0xFF
)<<24);AY=(AY&0x00FFFFFF)|((((Ax*((AY>>24)&0xFF))>>8)&0xFF)<<24);AZ=(AZ&0x00FFFFFF
)|((((Ax*((AZ>>24)&0xFF))>>8)&0xFF)<<24);AC.HH(aClip,B.sq(this.G,aOffset),this.Width
,A0,A1,AZ,AY,aBlend);}else AC.HH(aClip,B.sq(this.G,aOffset),this.Width,this.Bv,this.
BK,this.BJ,this.BI,aBlend);},I2:function(E){var A;if(E<0)E=0;if(E===this.Width)return;
this.Width=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);},IS:function(){
return this.Bv;},Ce:function(E){var A;if((((E===this.Bv)&&(E===this.BK))&&(E===this.
BI))&&(E===this.BJ))return;this.Bv=E;this.BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((
this.F&0x1)===0x1))this.K.Aq(this.G);},_Init:function(aArg){B.Core.Bg._Init.call(
this,aArg);this.__proto__=C.Cn;},_className:"Views::Border"};C.Bc={timer:null,Ae:
null,E5:null,AJ:0,Di:0,BI:0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF,By:
B.pm,Dp:0x12,Db:0,Ek:255,DA:0,Dq:false,IL:true,DN:false,FW:true,CL:function(AC,aClip
,aOffset,Ax,aBlend){var A;var Cu=this.Db;if(this.Di>=0)Cu=this.Di;if(!this.Ae||(
Cu>=this.Ae.NoOfFrames))return;this.Ae.Update();var Z=this.Eg();var AS=this.Ae.FrameSize;
var orient=this.DA;if((Z[0]>=Z[2])||(Z[1]>=Z[3]))return;var A0=this.Bv;var A1=this.
BK;var AZ=this.BJ;var AY=this.BI;var C6=(((Ax+1)*this.Ek)>>8)+1;aBlend=aBlend&&((
this.F&0x2)===0x2);if(C6<256){A0=(A0&0x00FFFFFF)|((((((A0>>24)&0xFF)*C6)>>8)&0xFF
)<<24);A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*C6)>>8)&0xFF)<<24);AZ=(AZ&0x00FFFFFF
)|((((((AZ>>24)&0xFF)*C6)>>8)&0xFF)<<24);AY=(AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*
C6)>>8)&0xFF)<<24);}if(B.sa([Z[2]-Z[0],Z[3]-Z[1]],AS)&&!orient)AC.HE(aClip,this.
Ae,Cu,B.sq(this.G,aOffset),B.sn(this.G.slice(0,2),Z.slice(0,2)),A0,A1,AZ,AY,aBlend
);else if(!orient)AC.I7(aClip,this.Ae,Cu,B.sq(Z,aOffset),[].concat(At,AS),A0,A1,
AZ,AY,aBlend,this.FW);else{var left=Z[0]+aOffset[0];var top=Z[1]+aOffset[1];var right=
Z[2]+aOffset[0];var bottom=Z[3]+aOffset[1];if(orient===1)AC.G3(aClip,this.Ae,Cu,
left,bottom,1.000000,left,top,1.000000,right,top,1.000000,right,bottom,1.000000,[
].concat(At,AS),AY,A0,A1,AZ,aBlend,this.FW);else if(orient===2)AC.G3(aClip,this.
Ae,Cu,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000,right,top,1.000000
,[].concat(At,AS),AZ,AY,A0,A1,aBlend,this.FW);else if(orient===3)AC.G3(aClip,this.
Ae,Cu,right,top,1.000000,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000
,[].concat(At,AS),A1,AZ,AY,A0,aBlend,this.FW);}},In:function(BX){var A;if(((this.
DN&&!!this.Ae)&&(this.Ae.FrameSize[0]>0))&&(this.Ae.FrameSize[1]>0))this.U(B.sp(
this.Eg(),this.By));if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);},Ed:function(
BX){var A;var Cu=this.Di;var EC=0;if(!!this.Ae)EC=this.Ae.NoOfFrames*this.Ae.FrameDelay;
if((!!this.timer&&(this.Di<0))&&(EC>0))this.AJ=this.timer.AH-(this.Db*this.Ae.FrameDelay
);if(!!this.timer&&(EC>0)){var Aa=(this.timer.AH-this.AJ)|0;Cu=(Aa/this.Ae.FrameDelay
)|0;if(Aa>=EC){Cu=Cu%this.Ae.NoOfFrames;this.AJ=this.timer.AH-(Aa%EC);if(!this.IL
){Cu=this.Ae.NoOfFrames-1;EC=0;}}}if(((Cu!==this.Di)&&!!this.K)&&((this.F&0x1)===
0x1))this.K.Aq(this.G);this.Di=Cu;if(!EC&&!!this.timer){B.rA([this,this.Ed],this.
timer,0);this.timer=null;(A=this.E5)?A[1].call(A[0],this):null;}},Cd:function(E){
if(E===this.DN)return;this.DN=E;if(((E&&!!this.Ae)&&(this.Ae.FrameSize[0]>0))&&(
this.Ae.FrameSize[1]>0))this.U(B.sp(this.Eg(),this.By));},Ce:function(E){var A;if((((
E===this.Bv)&&(E===this.BK))&&(E===this.BI))&&(E===this.BJ))return;this.Bv=E;this.
BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);},HW:
function(E){var A;if(this.Dq===E)return;this.Dq=E;this.Di=-1;if(!E&&!!this.timer
){B.rA([this,this.Ed],this.timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(
B.tg.FN);B.rl([this,this.Ed],this.timer,0);B.kD([this,this.Ed],this);}if(!!this.
K&&((this.F&0x1)===0x1))this.K.Aq(this.G);},HX:function(E){var A;if(B.sa(E,this.
By))return;this.By=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);},EO:function(
E){var A;if(E===this.Dp)return;this.Dp=E;if(!!this.K&&((this.F&0x1)===0x1))this.
K.Aq(this.G);},L:function(E){var A;if(E<0)E=0;if((E===this.Db)&&(this.Di===-1))return;
this.Db=E;if(!this.timer)this.Di=-1;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(
this.G);},Bp:function(E){var A;if(E===this.Ae)return;if(!!this.Ae&&this.Ae.FT)B.
rA([this,this.In],this.Ae,0);this.Ae=E;this.Di=-1;if(!!E&&E.FT)B.rl([this,this.In
],E,0);if(this.Dq){this.HW(false);this.HW(true);}if(((this.DN&&!!E)&&(E.FrameSize[
0]>0))&&(E.FrameSize[1]>0))this.U(B.sp(this.Eg(),this.By));if(!!this.K&&((this.F&
0x1)===0x1))this.K.Aq(this.G);},IU:function(){var A;return((this.F&0x1)===0x1);}
,Bd:function(E){if(E)this.Bn(0x1,0x0);else this.Bn(0x0,0x1);},Eg:function(){var A;
if(!this.Ae)return AT;var orient=this.DA;var Ao=this.Dp;var AS=this.Ae.FrameSize;
var Ab=this.G;var width=Ab[2]-Ab[0];var height=Ab[3]-Ab[1];if(!AS[0]||!AS[1])return AT;
if((orient===1)||(orient===3)){width=height;height=Ab[2]-Ab[0];}var Af=[0,0,width
,height];var O=Af;if(((Ao&0x40)===0x40)){var Hz=((((Af[2]-Af[0])<<16)+((AS[0]/2)|
0))/AS[0])|0;var FA=((((Af[3]-Af[1])<<16)+((AS[1]/2)|0))/AS[1])|0;var DM=Hz;if(FA>
DM)DM=FA;O=B.sU(O,((AS[0]*DM)+32768)>>16);O=B.sR(O,((AS[1]*DM)+32768)>>16);}else
if(((Ao&0x80)===0x80)){var Hz=((((Af[2]-Af[0])<<16)+((AS[0]/2)|0))/AS[0])|0;var FA=((((
Af[3]-Af[1])<<16)+((AS[1]/2)|0))/AS[1])|0;var DM=Hz;if(FA<DM)DM=FA;O=B.sU(O,((AS[
0]*DM)+32768)>>16);O=B.sR(O,((AS[1]*DM)+32768)>>16);}else if(!((Ao&0x100)===0x100
))O=B.sT(O,AS);if((O[2]-O[0])!==(Af[2]-Af[0])){if(((Ao&0x4)===0x4))O=B.sV(O,Af[2
]-(O[2]-O[0]));else if(((Ao&0x2)===0x2))O=B.sV(O,(Af[0]+(((Af[2]-Af[0])/2)|0))-(((
O[2]-O[0])/2)|0));}if((O[3]-O[1])!==(Af[3]-Af[1])){if(((Ao&0x20)===0x20))O=B.sX(
O,Af[3]-(O[3]-O[1]));else if(((Ao&0x10)===0x10))O=B.sX(O,(Af[1]+(((Af[3]-Af[1])/
2)|0))-(((O[3]-O[1])/2)|0));}if(!orient)O=B.sq(O,Ab.slice(0,2));else if(orient===
1){var BW=[Ab[0]+O[1],Ab[3]-O[2]];O=[].concat(BW,B.so(BW,[O[3]-O[1],O[2]-O[0]]));
}else if(orient===2){var BW=[Ab[2]-O[2],Ab[3]-O[3]];O=[].concat(BW,B.so(BW,[O[2]-
O[0],O[3]-O[1]]));}else if(orient===3){var BW=[Ab[2]-O[3],Ab[1]+O[0]];O=[].concat(
BW,B.so(BW,[O[3]-O[1],O[2]-O[0]]));}return B.sq(O,this.By);},_Init:function(aArg
){B.Core.Bg._Init.call(this,aArg);this.__proto__=C.Bc;},_Mark:function(D){var A;
B.Core.Bg._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D
);if((A=this.Ae)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.E5)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::Image"};C.Text={AM:null,HY:null,Cj:
null,CK:B.pm,AI:B.g3,Fa:0,Em:0,BI:0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF
,ES:0,By:B.pm,Dp:0x12,String:B.g3,Ek:255,DA:0,FZ:false,DN:false,GG:false,IJ:false
,CI:false,CL:function(AC,aClip,aOffset,Ax,aBlend){var A;if((this.AI===B.g3)||!this.
AM)return;var Ao=this.Dp;var orient=this.DA;var font=this.AM;var Af=B.sq(this.G,
aOffset);var A0=this.Bv;var A1=this.BK;var AZ=this.BJ;var AY=this.BI;var C6=(((Ax+
1)*this.Ek)>>8)+1;var C5=this.AI.charCodeAt(0)||0;var Z=B.sq(this.Eg(),aOffset);
var Cl=[Af[0]-Z[0],(Af[1]-Z[1])-font.Ascent];if(C5<1)return;if(C6<256){A0=(A0&0x00FFFFFF
)|((((((A0>>24)&0xFF)*C6)>>8)&0xFF)<<24);A1=(A1&0x00FFFFFF)|((((((A1>>24)&0xFF)*
C6)>>8)&0xFF)<<24);AZ=(AZ&0x00FFFFFF)|((((((AZ>>24)&0xFF)*C6)>>8)&0xFF)<<24);AY=(
AY&0x00FFFFFF)|((((((AY>>24)&0xFF)*C6)>>8)&0xFF)<<24);}if(((Ao&0x80)===0x80)){if(
this.GJ())Ao=(Ao&~0x80)|0x4;else Ao=(Ao&~0x80)|0x1;}if(((C5===1)&&!((Ao&0x40)===
0x40))&&!orient){AC.HI(aClip,font,this.AI,2,(this.AI.charCodeAt(1)||0)-1,Af,Cl,0
,orient,A0,A1,AZ,AY,true);return;}var leading=font.Leading;if(this.Em>0)leading=(
this.Em-font.Ascent)-font.Descent;var Hy=(font.Ascent+font.Descent)+leading;var Gd=
aClip[1]-Z[1];var Ge=aClip[3]-Z[1];var Fh=Z[2]-Z[0];var EH=0;var I=1;var BP=this.
AI.charCodeAt(I)||0;if(orient===1){Cl=[Z[3]-Af[3],(Af[0]-Z[0])-font.Ascent];Gd=aClip[
0]-Z[0];Ge=aClip[2]-Z[0];Fh=Z[3]-Z[1];}else if(orient===2){Cl=[Z[2]-Af[2],(Z[3]-
Af[3])-font.Ascent];Gd=Z[3]-aClip[3];Ge=Z[3]-aClip[1];}else if(orient===3){Cl=[Af[
1]-Z[1],(Z[2]-Af[2])-font.Ascent];Gd=Z[2]-aClip[2];Ge=Z[2]-aClip[0];Fh=Z[3]-Z[1];
}while(((EH+Hy)<Gd)&&(BP>0)){I=I+BP;EH=EH+Hy;BP=this.AI.charCodeAt(I)||0;}while((
EH<Ge)&&(BP>0)){var EA=B.sn(Cl,[0,EH]);var Iw=0;var Gn=false;if(((((Ao&0x40)===0x40
)&&((this.AI.charCodeAt((I+BP)-1)||0)!==0x0A))&&((this.AI.charCodeAt(I+1)||0)!==
0x0A))&&((this.AI.charCodeAt(I+BP)||0)!==0x00))Gn=true;if(Gn&&!!(Ao&0x6)){var Iv=
I+BP;var Ic=this.AI.indexOf(String.fromCharCode(0x20),I+1);var Id=this.AI.indexOf(
String.fromCharCode(0xA0),I+1);if(((Ic<0)||(Ic>=Iv))&&((Id<0)||(Id>=Iv)))Gn=false;
}if(Gn)Iw=Fh;else if(((Ao&0x4)===0x4))EA=[(EA[0]-Fh)+font.E2(this.AI,I+1,BP-1),EA[
1]];else if(((Ao&0x2)===0x2))EA=[(EA[0]-((Fh/2)|0))+((font.E2(this.AI,I+1,BP-1)/
2)|0),EA[1]];AC.HI(aClip,font,this.AI,I+1,BP-1,Af,EA,Iw,orient,A0,A1,AZ,AY,true);
I=I+BP;EH=EH+Hy;BP=this.AI.charCodeAt(I)||0;}},U:function(E){var A;if(B.sb(E,this.
G))return;var Hx=false;if(!this.DA||(this.DA===2))Hx=((A=this.G)[2]-A[0])!==(E[2
]-E[0]);else Hx=((A=this.G)[3]-A[1])!==(E[3]-E[1]);if((((Hx&&!this.ES)&&this.FZ)&&
this.CI)&&!((this.F&0x2000)===0x2000)){this.AI=B.g3;this.CI=false;B.kD([this,this.
Fx],this);}if(((this.GG&&this.CI)&&!B.sa([(A=this.G)[2]-A[0],A[3]-A[1]],[E[2]-E[
0],E[3]-E[1]]))&&!((this.F&0x2000)===0x2000)){this.AI=B.g3;this.CI=false;B.kD([this
,this.Fx],this);}B.Core.Bg.U.call(this,E);B.kD([this,this.Hu],this);},CY:function(
){if(!!this.Cj){this.Hi(this.Cj);this.Cj=null;}},Hi:function(aBidi){if(!aBidi)return;
B.mg(aBidi);},JG:function(aSize){var bidi=null;bidi=B.pd(aSize);return bidi;},Hu:
function(BX){B.kD(this.HY,this);},Fx:function(BX){B.kD([this,this.Gu],this);},Gu:
function(BX){var A;if(this.CI)return;var orient=this.DA;var width=(A=this.G)[2]-
A[0];var height=(A=this.G)[3]-A[1];var CF=-1;if((orient===1)||(orient===3)){width=
height;height=(A=this.G)[2]-A[0];}if(this.FZ){if(this.ES>0)CF=this.ES;else if(!this.
DN)CF=width-(this.Fa*2);else CF=width;if(CF<0)CF=1;}if(!!this.Cj);{this.Hi(this.
Cj);this.Cj=null;}this.CI=true;if((this.String!==B.g3)&&!!this.AM){var length=this.
String.length;if(this.IJ)this.Cj=this.JG(length);this.AI=this.AM.I3(this.String,
0,CF,length,this.Cj);if(!!this.Cj&&!this.IO()){this.Hi(this.Cj);this.Cj=null;}}else
this.AI=B.g3;this.CK=At;if((this.GG&&(this.AI!==B.g3))&&!this.DN){var Ao=this.Dp;
var font=this.AM;var leading=font.Leading;var AG=this.AI;var Gx=this.GJ();if(((Ao&
0x80)===0x80)){if(Gx)Ao=(Ao&~0x80)|0x4;else Ao=(Ao&~0x80)|0x1;}if(this.Em>0)leading=(
this.Em-font.Ascent)-font.Descent;var Fy=(font.Ascent+font.Descent)+leading;var C5=
AG.charCodeAt(0)||0;var D8=((height+leading)/Fy)|0;var He=false;var Gc=false;if(
D8<=0)D8=1;if(C5>D8){var CV=0;var Fz=0;var Gw=C5-1;var A3=0;var Bl=AG.length;var
tmp=B.g3;if(((Ao&0x20)===0x20))Fz=C5-D8;else if(((Ao&0x10)===0x10)){Fz=((C5-D8)/
2)|0;Gw=(Fz+D8)-1;}else Gw=D8-1;He=Fz>0;Gc=Gw<(C5-1);for(A3=1;CV<Fz;CV=CV+1)A3=A3+(
AG.charCodeAt(A3)||0);if(Gc)for(Bl=A3;CV<Gw;CV=CV+1)Bl=Bl+(AG.charCodeAt(Bl)||0);
if(He){var BA=AG.charCodeAt(A3)||0;tmp=(Cq+B.s4(AG,A3,BA))+Cq;tmp=B.sZ(tmp,0,(BA+
2)&0xFFFF);A3=A3+BA;if((tmp.charCodeAt(BA)||0)===0x0A){tmp=B.sZ(tmp,BA,0xFEFF);tmp=
B.sZ(tmp,BA+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.sZ(tmp,2,0xFEFF);tmp=
B.sZ(tmp,1,0x0A);}else tmp=B.sZ(tmp,1,0xFEFF);}tmp=tmp+B.s4(AG,A3,Bl-A3);if(Gc&&(
Bl>=A3)){var BA=AG.charCodeAt(Bl)||0;var BZ=(Cq+B.s4(AG,Bl,BA))+Cq;BZ=B.sZ(BZ,0,(
BA+2)&0xFFFF);BZ=B.sZ(BZ,1,0xFEFF);if((BZ.charCodeAt(BA)||0)===0x0A){BZ=B.sZ(BZ,
BA,0xFEFF);BZ=B.sZ(BZ,BA+1,0x0A);}if((BZ.charCodeAt(2)||0)===0x0A){BZ=B.sZ(BZ,2,
0xFEFF);BZ=B.sZ(BZ,1,0x0A);}else BZ=B.sZ(BZ,1,0xFEFF);tmp=tmp+BZ;}AG=String.fromCharCode(
D8&0xFFFF)+tmp;}var CV=0;var Gm=1;var Gr=width-(this.Fa*2);if(this.FZ&&(this.ES>
0))Gr=this.ES;C5=AG.charCodeAt(0)||0;for(;CV<C5;CV=CV+1){var Ea=He&&!CV;var Eb=Gc&&(
CV===(C5-1));var CA=false;var CB=false;var EY=Gx;if((Gx&&Ea)&&!Eb){Ea=false;Eb=true;
}else if((Gx&&Eb)&&!Ea){Eb=false;Ea=true;}var FB=Gm+1;var BA=AG.charCodeAt(Gm)||
0;var A3=FB;var Bl=(FB+BA)-2;var Ho=-1;var Hp=-1;if(!this.FZ&&(font.E2(AG,FB,BA-
1)>Gr)){if(((Ao&0x4)===0x4))CA=true;else if(((Ao&0x2)===0x2)){CA=true;CB=true;}else
CB=true;}if((AG.charCodeAt(A3)||0)===0x0A)A3=A3+1;if((AG.charCodeAt(Bl)||0)===0x0A
)Bl=Bl-1;while(CA&&((AG.charCodeAt(A3)||0)===0xFEFF))A3=A3+1;while(CB&&((AG.charCodeAt(
Bl)||0)===0xFEFF))Bl=Bl-1;CA=CA&&!Eb;CB=CB&&!Ea;while((((CA||CB)||Ea)||Eb)&&(A3<
Bl)){if((CA&&(EY||!CB))||Ea){if(Ho>0)AG=B.sZ(AG,Ho,0xFEFF);AG=B.sZ(AG,A3,0x2026);
Ho=A3;A3=A3+1;EY=!EY;Ea=false;if(font.E2(AG,FB,BA-1)<=Gr){CA=false;CB=false;}else
CA=CA||!CB;}if((CB&&(!EY||!CA))||Eb){if(Hp>0)AG=B.sZ(AG,Hp,0xFEFF);AG=B.sZ(AG,Bl
,0x2026);Hp=Bl;Bl=Bl-1;EY=!EY;Eb=false;if(font.E2(AG,FB,BA-1)<=Gr){CA=false;CB=false;
}else CB=CB||!CA;}}Gm=Gm+BA;}this.CK=[font.HL(AG),((AG.charCodeAt(0)||0)*Fy)-leading
];this.AI=AG;}if(this.DN&&(this.AI!==B.g3)){var DL=[this.Fa,0];if((orient===1)||(
orient===3)){DL=[DL[0],DL[0]];DL=[0,DL[1]];}this.F=this.F|0x2000;this.U(B.sp(B.r7(
this.Eg(),DL),this.By));this.F=this.F&~0x2000;}if(!!this.K&&((this.F&0x1)===0x1)
)this.K.Aq(this.G);B.kD([this,this.Hu],this);},EO:function(E){var A;if(E===this.
Dp)return;this.Dp=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);if(this.
GG){this.AI=B.g3;this.CI=false;B.kD([this,this.Fx],this);}if(this.CI)B.kD([this,
this.Hu],this);},E_:function(E){if(E===this.String)return;this.String=E;this.AI=
B.g3;this.CI=false;B.kD([this,this.Fx],this);},E7:function(E){if(E===this.AM)return;
this.AM=E;this.AI=B.g3;this.CI=false;B.kD([this,this.Fx],this);},Ce:function(E){
var A;if((((E===this.Bv)&&(E===this.BK))&&(E===this.BI))&&(E===this.BJ))return;this.
Bv=E;this.BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.
G);},I0:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(E===this.Ek)return;this.Ek=
E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Aq(this.G);},GJ:function(){if(!this.CI
)this.Gu(this);if(!this.Cj)return false;var result=false;var bidi=this.Cj;result=
B.pc(bidi);return result;},IO:function(){if(!this.CI)this.Gu(this);if(!this.Cj)return false;
var result=false;var bidi=this.Cj;result=B.rq(bidi);return result;},Eg:function(
){var A;if((this.String===B.g3)||!this.AM)return AT;if(!this.CI)this.Gu(this);if(
this.AI===B.g3)return AT;var leading=this.AM.Leading;var Fy=(this.AM.Ascent+this.
AM.Descent)+this.AM.Leading;if(this.Em>0){leading=(this.Em-this.AM.Ascent)-this.
AM.Descent;Fy=this.Em;}if(B.sa(this.CK,At))this.CK=[this.AM.HL(this.AI),this.CK[
1]];this.CK=[this.CK[0],((this.AI.charCodeAt(0)||0)*Fy)-leading];var Ao=this.Dp;
var orient=this.DA;var Ab=this.G;var DL=this.Fa;var width=Ab[2]-Ab[0];var height=
Ab[3]-Ab[1];if((orient===1)||(orient===3)){width=height;height=Ab[2]-Ab[0];}var Af=[
DL,0,width-DL,height];var O=[].concat(Af.slice(0,2),B.so(Af.slice(0,2),this.CK));
if(((Ao&0x80)===0x80)){if(this.GJ())Ao=(Ao&~0x80)|0x4;else Ao=(Ao&~0x80)|0x1;}if(((
Ao&0x40)===0x40)){var CF=this.ES;if(CF<=0)CF=width-(this.Fa*2);if(CF<0)CF=0;if(CF>(
O[2]-O[0]))O=B.sU(O,CF);}if((O[2]-O[0])!==(Af[2]-Af[0])){if(((Ao&0x4)===0x4))O=B.
sV(O,Af[2]-(O[2]-O[0]));else if(((Ao&0x2)===0x2))O=B.sV(O,(Af[0]+(((Af[2]-Af[0])
/2)|0))-(((O[2]-O[0])/2)|0));}if((O[3]-O[1])!==(Af[3]-Af[1])){if(((Ao&0x20)===0x20
))O=B.sX(O,Af[3]-(O[3]-O[1]));else if(((Ao&0x10)===0x10))O=B.sX(O,(Af[1]+(((Af[3
]-Af[1])/2)|0))-(((O[3]-O[1])/2)|0));}if(!orient)O=B.sq(O,Ab.slice(0,2));else if(
orient===1){var BW=[Ab[0]+O[1],Ab[3]-O[2]];O=[].concat(BW,B.so(BW,[this.CK[1],this.
CK[0]]));}else if(orient===2){var BW=[Ab[2]-O[2],Ab[3]-O[3]];O=[].concat(BW,B.so(
BW,this.CK));}else if(orient===3){var BW=[Ab[2]-O[3],Ab[1]+O[0]];O=[].concat(BW,
B.so(BW,[this.CK[1],this.CK[0]]));}return B.sq(O,this.By);},_Init:function(aArg){
B.Core.Bg._Init.call(this,aArg);this.__proto__=C.Text;},_Done:function(){this.__proto__=
C.Text;this.CY();B.Core.Bg._Done.call(this);},_Mark:function(D){var A;B.Core.Bg.
_Mark.call(this,D);if((A=this.AM)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.
HY)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Text"};C.KA={
IB:0x1,IA:0x2,IC:0x4,IF:0x8,IE:0x10,ID:0x20,KZ:0x40,K0:0x80,K5:0x100};C.K6={IB:0x1
,IA:0x2,IC:0x4,IF:0x8,IE:0x10,ID:0x20,J0:0x40,JZ:0x80};C.DA={KH:0,KW:1,KU:2,KV:3
};
C._Init=function(){C.CQ.__proto__=B.Core.Bg;C.Cn.__proto__=B.Core.Bg;C.Bc.__proto__=
B.Core.Bg;C.Text.__proto__=B.Core.Bg;};C.BO=function(D){};return C;})();

/* Embedded Wizard */