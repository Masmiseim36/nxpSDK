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
var Ar=[0,0];var AT=[0,0,0,0];var Cs="\uFEFF";
C.CT={BI:0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF,CP:function(AD,aClip
,aOffset,Ay,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Ay=Ay+1;if(Ay<256){
var A5=this.Bv;var A6=this.BK;var A3=this.BI;var A4=this.BJ;A5=(A5&0x00FFFFFF)|((((
Ay*((A5>>24)&0xFF))>>8)&0xFF)<<24);A6=(A6&0x00FFFFFF)|((((Ay*((A6>>24)&0xFF))>>8
)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((Ay*((A3>>24)&0xFF))>>8)&0xFF)<<24);A4=(A4&0x00FFFFFF
)|((((Ay*((A4>>24)&0xFF))>>8)&0xFF)<<24);AD.G0(aClip,B.tz(this.G,aOffset),A5,A6,
A4,A3,aBlend);}else AD.G0(aClip,B.tz(this.G,aOffset),this.Bv,this.BK,this.BJ,this.
BI,aBlend);},Cg:function(E){var A;if((((E===this.Bv)&&(E===this.BK))&&(E===this.
BI))&&(E===this.BJ))return;this.Bv=E;this.BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((
this.F&0x1)===0x1))this.K.Ao(this.G);},_Init:function(aArg){B.Core.A2._Init.call(
this,aArg);this.__proto__=C.CT;},_className:"Views::Rectangle"};C.Cq={Width:1,BI:
0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF,CP:function(AD,aClip,aOffset
,Ay,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);Ay=Ay+1;if(Ay<256){var A5=
this.Bv;var A6=this.BK;var A3=this.BI;var A4=this.BJ;A5=(A5&0x00FFFFFF)|((((Ay*((
A5>>24)&0xFF))>>8)&0xFF)<<24);A6=(A6&0x00FFFFFF)|((((Ay*((A6>>24)&0xFF))>>8)&0xFF
)<<24);A3=(A3&0x00FFFFFF)|((((Ay*((A3>>24)&0xFF))>>8)&0xFF)<<24);A4=(A4&0x00FFFFFF
)|((((Ay*((A4>>24)&0xFF))>>8)&0xFF)<<24);AD.HZ(aClip,B.tz(this.G,aOffset),this.Width
,A5,A6,A4,A3,aBlend);}else AD.HZ(aClip,B.tz(this.G,aOffset),this.Width,this.Bv,this.
BK,this.BJ,this.BI,aBlend);},Jg:function(E){var A;if(E<0)E=0;if(E===this.Width)return;
this.Width=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);},I8:function(){
return this.Bv;},Cg:function(E){var A;if((((E===this.Bv)&&(E===this.BK))&&(E===this.
BI))&&(E===this.BJ))return;this.Bv=E;this.BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((
this.F&0x1)===0x1))this.K.Ao(this.G);},_Init:function(aArg){B.Core.A2._Init.call(
this,aArg);this.__proto__=C.Cq;},_className:"Views::Border"};C.Bh={timer:null,Ae:
null,Fd:null,AK:0,Dq:0,BI:0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF,Bv:0xFFFFFFFF,Bz:
B.qx,Dv:0x12,Dj:0,Er:255,DF:0,Dw:false,I0:true,DV:false,Ge:true,CP:function(AD,aClip
,aOffset,Ay,aBlend){var A;var Cx=this.Dj;if(this.Dq>=0)Cx=this.Dq;if(!this.Ae||(
Cx>=this.Ae.NoOfFrames))return;this.Ae.Update();var Z=this.Em();var AW=this.Ae.FrameSize;
var orient=this.DF;if((Z[0]>=Z[2])||(Z[1]>=Z[3]))return;var A5=this.Bv;var A6=this.
BK;var A4=this.BJ;var A3=this.BI;var C$=(((Ay+1)*this.Er)>>8)+1;aBlend=aBlend&&((
this.F&0x2)===0x2);if(C$<256){A5=(A5&0x00FFFFFF)|((((((A5>>24)&0xFF)*C$)>>8)&0xFF
)<<24);A6=(A6&0x00FFFFFF)|((((((A6>>24)&0xFF)*C$)>>8)&0xFF)<<24);A4=(A4&0x00FFFFFF
)|((((((A4>>24)&0xFF)*C$)>>8)&0xFF)<<24);A3=(A3&0x00FFFFFF)|((((((A3>>24)&0xFF)*
C$)>>8)&0xFF)<<24);}if(B.tl([Z[2]-Z[0],Z[3]-Z[1]],AW)&&!orient)AD.HX(aClip,this.
Ae,Cx,B.tz(this.G,aOffset),B.tw(this.G.slice(0,2),Z.slice(0,2)),A5,A6,A4,A3,aBlend
);else if(!orient)AD.Jl(aClip,this.Ae,Cx,B.tz(Z,aOffset),[].concat(Ar,AW),A5,A6,
A4,A3,aBlend,this.Ge);else{var left=Z[0]+aOffset[0];var top=Z[1]+aOffset[1];var right=
Z[2]+aOffset[0];var bottom=Z[3]+aOffset[1];if(orient===1)AD.Hl(aClip,this.Ae,Cx,
left,bottom,1.000000,left,top,1.000000,right,top,1.000000,right,bottom,1.000000,[
].concat(Ar,AW),A3,A5,A6,A4,aBlend,this.Ge);else if(orient===2)AD.Hl(aClip,this.
Ae,Cx,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000,right,top,1.000000
,[].concat(Ar,AW),A4,A3,A5,A6,aBlend,this.Ge);else if(orient===3)AD.Hl(aClip,this.
Ae,Cx,right,top,1.000000,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000
,[].concat(Ar,AW),A6,A4,A3,A5,aBlend,this.Ge);}},IB:function(B0){var A;if(((this.
DV&&!!this.Ae)&&(this.Ae.FrameSize[0]>0))&&(this.Ae.FrameSize[1]>0))this.T(B.ty(
this.Em(),this.Bz));if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);},Ei:function(
B0){var A;var Cx=this.Dq;var EK=0;if(!!this.Ae)EK=this.Ae.NoOfFrames*this.Ae.FrameDelay;
if((!!this.timer&&(this.Dq<0))&&(EK>0))this.AK=this.timer.AI-(this.Dj*this.Ae.FrameDelay
);if(!!this.timer&&(EK>0)){var Aa=(this.timer.AI-this.AK)|0;Cx=(Aa/this.Ae.FrameDelay
)|0;if(Aa>=EK){Cx=Cx%this.Ae.NoOfFrames;this.AK=this.timer.AI-(Aa%EK);if(!this.I0
){Cx=this.Ae.NoOfFrames-1;EK=0;}}}if(((Cx!==this.Dq)&&!!this.K)&&((this.F&0x1)===
0x1))this.K.Ao(this.G);this.Dq=Cx;if(!EK&&!!this.timer){B.sM([this,this.Ei],this.
timer,0);this.timer=null;(A=this.Fd)?A[1].call(A[0],this):null;}},Cf:function(E){
if(E===this.DV)return;this.DV=E;if(((E&&!!this.Ae)&&(this.Ae.FrameSize[0]>0))&&(
this.Ae.FrameSize[1]>0))this.T(B.ty(this.Em(),this.Bz));},Cg:function(E){var A;if((((
E===this.Bv)&&(E===this.BK))&&(E===this.BI))&&(E===this.BJ))return;this.Bv=E;this.
BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);},Ia:
function(E){var A;if(this.Dw===E)return;this.Dw=E;this.Dq=-1;if(!E&&!!this.timer
){B.sM([this,this.Ei],this.timer,0);this.timer=null;}if(E){this.timer=B._GetAutoObject(
B.uk.FX);B.sz([this,this.Ei],this.timer,0);B.lq([this,this.Ei],this);}if(!!this.
K&&((this.F&0x1)===0x1))this.K.Ao(this.G);},Ib:function(E){var A;if(B.tl(E,this.
Bz))return;this.Bz=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);},EV:function(
E){var A;if(E===this.Dv)return;this.Dv=E;if(!!this.K&&((this.F&0x1)===0x1))this.
K.Ao(this.G);},L:function(E){var A;if(E<0)E=0;if((E===this.Dj)&&(this.Dq===-1))return;
this.Dj=E;if(!this.timer)this.Dq=-1;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(
this.G);},Bs:function(E){var A;if(E===this.Ae)return;if(!!this.Ae&&this.Ae.Gb)B.
sM([this,this.IB],this.Ae,0);this.Ae=E;this.Dq=-1;if(!!E&&E.Gb)B.sz([this,this.IB
],E,0);if(this.Dw){this.Ia(false);this.Ia(true);}if(((this.DV&&!!E)&&(E.FrameSize[
0]>0))&&(E.FrameSize[1]>0))this.T(B.ty(this.Em(),this.Bz));if(!!this.K&&((this.F&
0x1)===0x1))this.K.Ao(this.G);},I_:function(){var A;return((this.F&0x1)===0x1);}
,Bi:function(E){if(E)this.Bq(0x1,0x0);else this.Bq(0x0,0x1);},Em:function(){var A;
if(!this.Ae)return AT;var orient=this.DF;var Am=this.Dv;var AW=this.Ae.FrameSize;
var Ab=this.G;var width=Ab[2]-Ab[0];var height=Ab[3]-Ab[1];if(!AW[0]||!AW[1])return AT;
if((orient===1)||(orient===3)){width=height;height=Ab[2]-Ab[0];}var Af=[0,0,width
,height];var N=Af;if(((Am&0x40)===0x40)){var HS=((((Af[2]-Af[0])<<16)+((AW[0]/2)|
0))/AW[0])|0;var FL=((((Af[3]-Af[1])<<16)+((AW[1]/2)|0))/AW[1])|0;var DU=HS;if(FL>
DU)DU=FL;N=B.tZ(N,((AW[0]*DU)+32768)>>16);N=B.tW(N,((AW[1]*DU)+32768)>>16);}else
if(((Am&0x80)===0x80)){var HS=((((Af[2]-Af[0])<<16)+((AW[0]/2)|0))/AW[0])|0;var FL=((((
Af[3]-Af[1])<<16)+((AW[1]/2)|0))/AW[1])|0;var DU=HS;if(FL<DU)DU=FL;N=B.tZ(N,((AW[
0]*DU)+32768)>>16);N=B.tW(N,((AW[1]*DU)+32768)>>16);}else if(!((Am&0x100)===0x100
))N=B.tY(N,AW);if((N[2]-N[0])!==(Af[2]-Af[0])){if(((Am&0x4)===0x4))N=B.t0(N,Af[2
]-(N[2]-N[0]));else if(((Am&0x2)===0x2))N=B.t0(N,(Af[0]+(((Af[2]-Af[0])/2)|0))-(((
N[2]-N[0])/2)|0));}if((N[3]-N[1])!==(Af[3]-Af[1])){if(((Am&0x20)===0x20))N=B.t2(
N,Af[3]-(N[3]-N[1]));else if(((Am&0x10)===0x10))N=B.t2(N,(Af[1]+(((Af[3]-Af[1])/
2)|0))-(((N[3]-N[1])/2)|0));}if(!orient)N=B.tz(N,Ab.slice(0,2));else if(orient===
1){var BZ=[Ab[0]+N[1],Ab[3]-N[2]];N=[].concat(BZ,B.tx(BZ,[N[3]-N[1],N[2]-N[0]]));
}else if(orient===2){var BZ=[Ab[2]-N[2],Ab[3]-N[3]];N=[].concat(BZ,B.tx(BZ,[N[2]-
N[0],N[3]-N[1]]));}else if(orient===3){var BZ=[Ab[2]-N[3],Ab[1]+N[0]];N=[].concat(
BZ,B.tx(BZ,[N[3]-N[1],N[2]-N[0]]));}return B.tz(N,this.Bz);},_Init:function(aArg
){B.Core.A2._Init.call(this,aArg);this.__proto__=C.Bh;},_Mark:function(D){var A;
B.Core.A2._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D
);if((A=this.Ae)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.Fd)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::Image"};C.Text={AN:null,D1:null,AJ:
B.hm,String:B.hm,Cm:null,CO:B.qx,Fk:0,Es:0,BI:0xFFFFFFFF,BJ:0xFFFFFFFF,BK:0xFFFFFFFF
,Bv:0xFFFFFFFF,EZ:0,Bz:B.qx,Dv:0x12,Er:255,DF:0,Gj:false,DV:false,GY:false,IZ:false
,CM:false,CP:function(AD,aClip,aOffset,Ay,aBlend){var A;if((this.AJ===B.hm)||!this.
AN)return;var Am=this.Dv;var orient=this.DF;var font=this.AN;var Af=B.tz(this.G,
aOffset);var A5=this.Bv;var A6=this.BK;var A4=this.BJ;var A3=this.BI;var C$=(((Ay+
1)*this.Er)>>8)+1;var C_=this.AJ.charCodeAt(0)||0;var Z=B.tz(this.Em(),aOffset);
var Co=[Af[0]-Z[0],(Af[1]-Z[1])-font.Ascent];if(C_<1)return;if(C$<256){A5=(A5&0x00FFFFFF
)|((((((A5>>24)&0xFF)*C$)>>8)&0xFF)<<24);A6=(A6&0x00FFFFFF)|((((((A6>>24)&0xFF)*
C$)>>8)&0xFF)<<24);A4=(A4&0x00FFFFFF)|((((((A4>>24)&0xFF)*C$)>>8)&0xFF)<<24);A3=(
A3&0x00FFFFFF)|((((((A3>>24)&0xFF)*C$)>>8)&0xFF)<<24);}if(((Am&0x80)===0x80)){if(
this.G3())Am=(Am&~0x80)|0x4;else Am=(Am&~0x80)|0x1;}if(((C_===1)&&!((Am&0x40)===
0x40))&&!orient){AD.H0(aClip,font,this.AJ,2,(this.AJ.charCodeAt(1)||0)-1,Af,Co,0
,orient,A5,A6,A4,A3,true);return;}var leading=font.Leading;if(this.Es>0)leading=(
this.Es-font.Ascent)-font.Descent;var HR=(font.Ascent+font.Descent)+leading;var Gq=
aClip[1]-Z[1];var Gr=aClip[3]-Z[1];var Fq=Z[2]-Z[0];var EQ=0;var I=1;var BQ=this.
AJ.charCodeAt(I)||0;if(orient===1){Co=[Z[3]-Af[3],(Af[0]-Z[0])-font.Ascent];Gq=aClip[
0]-Z[0];Gr=aClip[2]-Z[0];Fq=Z[3]-Z[1];}else if(orient===2){Co=[Z[2]-Af[2],(Z[3]-
Af[3])-font.Ascent];Gq=Z[3]-aClip[3];Gr=Z[3]-aClip[1];}else if(orient===3){Co=[Af[
1]-Z[1],(Z[2]-Af[2])-font.Ascent];Gq=Z[2]-aClip[2];Gr=Z[2]-aClip[0];Fq=Z[3]-Z[1];
}while(((EQ+HR)<Gq)&&(BQ>0)){I=I+BQ;EQ=EQ+HR;BQ=this.AJ.charCodeAt(I)||0;}while((
EQ<Gr)&&(BQ>0)){var EI=B.tw(Co,[0,EQ]);var IK=0;var GA=false;if(((((Am&0x40)===0x40
)&&((this.AJ.charCodeAt((I+BQ)-1)||0)!==0x0A))&&((this.AJ.charCodeAt(I+1)||0)!==
0x0A))&&((this.AJ.charCodeAt(I+BQ)||0)!==0x00))GA=true;if(GA&&!!(Am&0x6)){var IJ=
I+BQ;var Ip=this.AJ.indexOf(String.fromCharCode(0x20),I+1);var Iq=this.AJ.indexOf(
String.fromCharCode(0xA0),I+1);if(((Ip<0)||(Ip>=IJ))&&((Iq<0)||(Iq>=IJ)))GA=false;
}if(GA)IK=Fq;else if(((Am&0x4)===0x4))EI=[(EI[0]-Fq)+font.Fa(this.AJ,I+1,BQ-1),EI[
1]];else if(((Am&0x2)===0x2))EI=[(EI[0]-((Fq/2)|0))+((font.Fa(this.AJ,I+1,BQ-1)/
2)|0),EI[1]];AD.H0(aClip,font,this.AJ,I+1,BQ-1,Af,EI,IK,orient,A5,A6,A4,A3,true);
I=I+BQ;EQ=EQ+HR;BQ=this.AJ.charCodeAt(I)||0;}},T:function(E){var A;if(B.tm(E,this.
G))return;var HQ=false;if(!this.DF||(this.DF===2))HQ=((A=this.G)[2]-A[0])!==(E[2
]-E[0]);else HQ=((A=this.G)[3]-A[1])!==(E[3]-E[1]);if((((HQ&&!this.EZ)&&this.Gj)&&
this.CM)&&!((this.F&0x2000)===0x2000)){this.AJ=B.hm;this.CM=false;B.lq([this,this.
FH],this);}if(((this.GY&&this.CM)&&!B.tl([(A=this.G)[2]-A[0],A[3]-A[1]],[E[2]-E[
0],E[3]-E[1]]))&&!((this.F&0x2000)===0x2000)){this.AJ=B.hm;this.CM=false;B.lq([this
,this.FH],this);}B.Core.A2.T.call(this,E);B.lq([this,this.HN],this);},C2:function(
){if(!!this.Cm){this.HA(this.Cm);this.Cm=null;}},HA:function(aBidi){if(!aBidi)return;
B.ng(aBidi);},J1:function(aSize){var bidi=null;bidi=B.qk(aSize);return bidi;},HN:
function(B0){B.lq(this.D1,this);},FH:function(B0){B.lq([this,this.GH],this);},GH:
function(B0){var A;if(this.CM)return;var orient=this.DF;var width=(A=this.G)[2]-
A[0];var height=(A=this.G)[3]-A[1];var CJ=-1;if((orient===1)||(orient===3)){width=
height;height=(A=this.G)[2]-A[0];}if(this.Gj){if(this.EZ>0)CJ=this.EZ;else if(!this.
DV)CJ=width-(this.Fk*2);else CJ=width;if(CJ<0)CJ=1;}if(!!this.Cm){this.HA(this.Cm
);this.Cm=null;}this.CM=true;if((this.String!==B.hm)&&!!this.AN){var length=this.
String.length;if(this.IZ)this.Cm=this.J1(length);this.AJ=this.AN.Jh(this.String,
0,CJ,length,this.Cm);if(!!this.Cm&&!this.I3()){this.HA(this.Cm);this.Cm=null;}}else
this.AJ=B.hm;this.CO=Ar;if(((this.GY&&(this.AJ!==B.hm))&&!this.DV)&&!!this.AN){var
Am=this.Dv;var font=this.AN;var leading=font.Leading;var AH=this.AJ;var GL=this.
G3();if(((Am&0x80)===0x80)){if(GL)Am=(Am&~0x80)|0x4;else Am=(Am&~0x80)|0x1;}if(this.
Es>0)leading=(this.Es-font.Ascent)-font.Descent;var FI=(font.Ascent+font.Descent
)+leading;var C_=AH.charCodeAt(0)||0;var Ea=((height+leading)/FI)|0;var Hw=false;
var Gp=false;if(Ea<=0)Ea=1;if(C_>Ea){var C0=0;var FJ=0;var GK=C_-1;var A9=0;var Bo=
AH.length;var tmp=B.hm;if(((Am&0x20)===0x20))FJ=C_-Ea;else if(((Am&0x10)===0x10)
){FJ=((C_-Ea)/2)|0;GK=(FJ+Ea)-1;}else GK=Ea-1;Hw=FJ>0;Gp=GK<(C_-1);for(A9=1;C0<FJ;
C0=C0+1)A9=A9+(AH.charCodeAt(A9)||0);if(Gp)for(Bo=A9;C0<GK;C0=C0+1)Bo=Bo+(AH.charCodeAt(
Bo)||0);if(Hw){var BA=AH.charCodeAt(A9)||0;tmp=(Cs+B.t9(AH,A9,BA))+Cs;tmp=B.t4(tmp
,0,(BA+2)&0xFFFF);A9=A9+BA;if((tmp.charCodeAt(BA)||0)===0x0A){tmp=B.t4(tmp,BA,0xFEFF
);tmp=B.t4(tmp,BA+1,0x0A);}if((tmp.charCodeAt(2)||0)===0x0A){tmp=B.t4(tmp,2,0xFEFF
);tmp=B.t4(tmp,1,0x0A);}else tmp=B.t4(tmp,1,0xFEFF);}tmp=tmp+B.t9(AH,A9,Bo-A9);if(
Gp&&(Bo>=A9)){var BA=AH.charCodeAt(Bo)||0;var B2=(Cs+B.t9(AH,Bo,BA))+Cs;B2=B.t4(
B2,0,(BA+2)&0xFFFF);B2=B.t4(B2,1,0xFEFF);if((B2.charCodeAt(BA)||0)===0x0A){B2=B.
t4(B2,BA,0xFEFF);B2=B.t4(B2,BA+1,0x0A);}if((B2.charCodeAt(2)||0)===0x0A){B2=B.t4(
B2,2,0xFEFF);B2=B.t4(B2,1,0x0A);}else B2=B.t4(B2,1,0xFEFF);tmp=tmp+B2;}AH=String.
fromCharCode(Ea&0xFFFF)+tmp;}var C0=0;var Gz=1;var GE=width-(this.Fk*2);if(this.
Gj&&(this.EZ>0))GE=this.EZ;C_=AH.charCodeAt(0)||0;for(;C0<C_;C0=C0+1){var Ef=Hw&&
!C0;var Eg=Gp&&(C0===(C_-1));var CE=false;var CF=false;var E9=GL;if((GL&&Ef)&&!Eg
){Ef=false;Eg=true;}else if((GL&&Eg)&&!Ef){Eg=false;Ef=true;}var FM=Gz+1;var BA=
AH.charCodeAt(Gz)||0;var A9=FM;var Bo=(FM+BA)-2;var HG=-1;var HH=-1;if(!this.Gj&&(
font.Fa(AH,FM,BA-1)>GE)){if(((Am&0x4)===0x4))CE=true;else if(((Am&0x2)===0x2)){CE=
true;CF=true;}else CF=true;}if((AH.charCodeAt(A9)||0)===0x0A)A9=A9+1;if((AH.charCodeAt(
Bo)||0)===0x0A)Bo=Bo-1;while(CE&&((AH.charCodeAt(A9)||0)===0xFEFF))A9=A9+1;while(
CF&&((AH.charCodeAt(Bo)||0)===0xFEFF))Bo=Bo-1;CE=CE&&!Eg;CF=CF&&!Ef;while((((CE||
CF)||Ef)||Eg)&&(A9<Bo)){if((CE&&(E9||!CF))||Ef){if(HG>0)AH=B.t4(AH,HG,0xFEFF);AH=
B.t4(AH,A9,0x2026);HG=A9;A9=A9+1;E9=!E9;Ef=false;if(font.Fa(AH,FM,BA-1)<=GE){CE=
false;CF=false;}else CE=CE||!CF;}if((CF&&(!E9||!CE))||Eg){if(HH>0)AH=B.t4(AH,HH,
0xFEFF);AH=B.t4(AH,Bo,0x2026);HH=Bo;Bo=Bo-1;E9=!E9;Eg=false;if(font.Fa(AH,FM,BA-
1)<=GE){CE=false;CF=false;}else CF=CF||!CE;}}Gz=Gz+BA;}this.CO=[font.H3(AH),((AH.
charCodeAt(0)||0)*FI)-leading];this.AJ=AH;}if(this.DV&&(this.AJ!==B.hm)){var DS=[
this.Fk,0];if((orient===1)||(orient===3)){DS=[DS[0],DS[0]];DS=[0,DS[1]];}this.F=
this.F|0x2000;this.T(B.ty(B.th(this.Em(),DS),this.Bz));this.F=this.F&~0x2000;}if(
!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.G);B.lq([this,this.HN],this);},EV:
function(E){var A;if(E===this.Dv)return;this.Dv=E;if(!!this.K&&((this.F&0x1)===0x1
))this.K.Ao(this.G);if(this.GY){this.AJ=B.hm;this.CM=false;B.lq([this,this.FH],this
);}if(this.CM)B.lq([this,this.HN],this);},Fi:function(E){if(E===this.String)return;
this.String=E;this.AJ=B.hm;this.CM=false;B.lq([this,this.FH],this);},Ff:function(
E){if(E===this.AN)return;this.AN=E;this.AJ=B.hm;this.CM=false;B.lq([this,this.FH
],this);},Cg:function(E){var A;if((((E===this.Bv)&&(E===this.BK))&&(E===this.BI)
)&&(E===this.BJ))return;this.Bv=E;this.BK=E;this.BI=E;this.BJ=E;if(!!this.K&&((this.
F&0x1)===0x1))this.K.Ao(this.G);},Je:function(E){var A;if(E>255)E=255;if(E<0)E=0;
if(E===this.Er)return;this.Er=E;if(!!this.K&&((this.F&0x1)===0x1))this.K.Ao(this.
G);},G3:function(){if(!this.CM)this.GH(this);if(!this.Cm)return false;var result=
false;var bidi=this.Cm;result=B.qj(bidi);return result;},I3:function(){if(!this.
CM)this.GH(this);if(!this.Cm)return false;var result=false;var bidi=this.Cm;result=
B.sD(bidi);return result;},Em:function(){var A;if((this.String===B.hm)||!this.AN
)return AT;if(!this.CM)this.GH(this);if(this.AJ===B.hm)return AT;var leading=this.
AN.Leading;var FI=(this.AN.Ascent+this.AN.Descent)+this.AN.Leading;if(this.Es>0){
leading=(this.Es-this.AN.Ascent)-this.AN.Descent;FI=this.Es;}if(B.tl(this.CO,Ar)
)this.CO=[this.AN.H3(this.AJ),this.CO[1]];this.CO=[this.CO[0],((this.AJ.charCodeAt(
0)||0)*FI)-leading];var Am=this.Dv;var orient=this.DF;var Ab=this.G;var DS=this.
Fk;var width=Ab[2]-Ab[0];var height=Ab[3]-Ab[1];if((orient===1)||(orient===3)){width=
height;height=Ab[2]-Ab[0];}var Af=[DS,0,width-DS,height];var N=[].concat(Af.slice(
0,2),B.tx(Af.slice(0,2),this.CO));if(((Am&0x80)===0x80)){if(this.G3())Am=(Am&~0x80
)|0x4;else Am=(Am&~0x80)|0x1;}if(((Am&0x40)===0x40)){var CJ=this.EZ;if(CJ<=0)CJ=
width-(this.Fk*2);if(CJ<0)CJ=0;if(CJ>(N[2]-N[0]))N=B.tZ(N,CJ);}if((N[2]-N[0])!==(
Af[2]-Af[0])){if(((Am&0x4)===0x4))N=B.t0(N,Af[2]-(N[2]-N[0]));else if(((Am&0x2)===
0x2))N=B.t0(N,(Af[0]+(((Af[2]-Af[0])/2)|0))-(((N[2]-N[0])/2)|0));}if((N[3]-N[1])
!==(Af[3]-Af[1])){if(((Am&0x20)===0x20))N=B.t2(N,Af[3]-(N[3]-N[1]));else if(((Am&
0x10)===0x10))N=B.t2(N,(Af[1]+(((Af[3]-Af[1])/2)|0))-(((N[3]-N[1])/2)|0));}if(!orient
)N=B.tz(N,Ab.slice(0,2));else if(orient===1){var BZ=[Ab[0]+N[1],Ab[3]-N[2]];N=[].
concat(BZ,B.tx(BZ,[this.CO[1],this.CO[0]]));}else if(orient===2){var BZ=[Ab[2]-N[
2],Ab[3]-N[3]];N=[].concat(BZ,B.tx(BZ,this.CO));}else if(orient===3){var BZ=[Ab[
2]-N[3],Ab[1]+N[0]];N=[].concat(BZ,B.tx(BZ,[this.CO[1],this.CO[0]]));}return B.tz(
N,this.Bz);},_Init:function(aArg){B.Core.A2._Init.call(this,aArg);this.__proto__=
C.Text;},_Done:function(){this.C2();this.__proto__=B.Core.A2;B.Core.A2._Done.call(
this);},_Mark:function(D){var A;B.Core.A2._Mark.call(this,D);if((A=this.AN)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.D1)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Views::Text"};C.KZ={IQ:0x1,IP:0x2,IR:0x4,IU:0x8,IT:0x10,IS:0x20
,Lq:0x40,Lr:0x80,Lw:0x100};C.Lx={IQ:0x1,IP:0x2,IR:0x4,IU:0x8,IT:0x10,IS:0x20,Kk:
0x40,Kj:0x80};C.DF={K8:0,Ln:1,Ll:2,Lm:3};
C._Init=function(){C.CT.__proto__=B.Core.A2;C.Cq.__proto__=B.Core.A2;C.Bh.__proto__=
B.Core.A2;C.Text.__proto__=B.Core.A2;};C.BO=function(D){};return C;})();

/* Embedded Wizard */