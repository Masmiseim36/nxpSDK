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
var AM=[0,0];var BD=[0,0,0,0];var Dj="\uFEFF";
C.Av={Cp:0xFFFFFFFF,Cq:0xFFFFFFFF,Cr:0xFFFFFFFF,Ch:0xFFFFFFFF,DK:function(Bi,aClip
,aOffset,A_,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A_=A_+1;if(A_<256){
var BR=this.Ch;var BS=this.Cr;var BP=this.Cp;var BQ=this.Cq;BR=(BR&0x00FFFFFF)|((((
A_*((BR>>24)&0xFF))>>8)&0xFF)<<24);BS=(BS&0x00FFFFFF)|((((A_*((BS>>24)&0xFF))>>8
)&0xFF)<<24);BP=(BP&0x00FFFFFF)|((((A_*((BP>>24)&0xFF))>>8)&0xFF)<<24);BQ=(BQ&0x00FFFFFF
)|((((A_*((BQ>>24)&0xFF))>>8)&0xFF)<<24);Bi.IL(aClip,B.tz(this.G,aOffset),BR,BS,
BQ,BP,aBlend);}else Bi.IL(aClip,B.tz(this.G,aOffset),this.Ch,this.Cr,this.Cq,this.
Cp,aBlend);},Q:function(E){var A;if((((E===this.Ch)&&(E===this.Cr))&&(E===this.Cp
))&&(E===this.Cq))return;this.Ch=E;this.Cr=E;this.Cp=E;this.Cq=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AL(this.G);},_Init:function(aArg){B.Core.BO._Init.call(
this,aArg);this.__proto__=C.Av;},_className:"Views::Rectangle"};C.Ac={Width:1,Cp:
0xFFFFFFFF,Cq:0xFFFFFFFF,Cr:0xFFFFFFFF,Ch:0xFFFFFFFF,DK:function(Bi,aClip,aOffset
,A_,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A_=A_+1;if(A_<256){var BR=
this.Ch;var BS=this.Cr;var BP=this.Cp;var BQ=this.Cq;BR=(BR&0x00FFFFFF)|((((A_*((
BR>>24)&0xFF))>>8)&0xFF)<<24);BS=(BS&0x00FFFFFF)|((((A_*((BS>>24)&0xFF))>>8)&0xFF
)<<24);BP=(BP&0x00FFFFFF)|((((A_*((BP>>24)&0xFF))>>8)&0xFF)<<24);BQ=(BQ&0x00FFFFFF
)|((((A_*((BQ>>24)&0xFF))>>8)&0xFF)<<24);Bi.JT(aClip,B.tz(this.G,aOffset),this.Width
,BR,BS,BQ,BP,aBlend);}else Bi.JT(aClip,B.tz(this.G,aOffset),this.Width,this.Ch,this.
Cr,this.Cq,this.Cp,aBlend);},J4:function(E){var A;if(E<0)E=0;if(E===this.Width)return;
this.Width=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.G);},Lu:function(){
return this.Ch;},Q:function(E){var A;if((((E===this.Ch)&&(E===this.Cr))&&(E===this.
Cp))&&(E===this.Cq))return;this.Ch=E;this.Cr=E;this.Cp=E;this.Cq=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AL(this.G);},_Init:function(aArg){B.Core.BO._Init.call(
this,aArg);this.__proto__=C.Ac;},_className:"Views::Border"};C.Aq={timer:null,An:
null,GO:null,Bs:0,EF:0,Cp:0xFFFFFFFF,Cq:0xFFFFFFFF,Cr:0xFFFFFFFF,Ch:0xFFFFFFFF,D_:
B.qx,EL:0x12,HE:0,Be:255,D9:0,EM:false,Lg:true,Ep:false,HS:true,DK:function(Bi,aClip
,aOffset,A_,aBlend){var A;var Dp=this.HE;if(this.EF>=0)Dp=this.EF;if(!this.An||(
Dp>=this.An.NoOfFrames))return;this.An.Update();var Al=this.Fc();var BF=this.An.
FrameSize;var orient=this.D9;if((Al[0]>=Al[2])||(Al[1]>=Al[3]))return;var BR=this.
Ch;var BS=this.Cr;var BQ=this.Cq;var BP=this.Cp;var Eh=(((A_+1)*this.Be)>>8)+1;aBlend=
aBlend&&((this.F&0x2)===0x2);if(Eh<256){BR=(BR&0x00FFFFFF)|((((((BR>>24)&0xFF)*Eh
)>>8)&0xFF)<<24);BS=(BS&0x00FFFFFF)|((((((BS>>24)&0xFF)*Eh)>>8)&0xFF)<<24);BQ=(BQ&
0x00FFFFFF)|((((((BQ>>24)&0xFF)*Eh)>>8)&0xFF)<<24);BP=(BP&0x00FFFFFF)|((((((BP>>
24)&0xFF)*Eh)>>8)&0xFF)<<24);}if(B.tl([Al[2]-Al[0],Al[3]-Al[1]],BF)&&!orient)Bi.
JQ(aClip,this.An,Dp,B.tz(this.G,aOffset),B.tw(this.G.slice(0,2),Al.slice(0,2)),BR
,BS,BQ,BP,aBlend);else if(!orient)Bi.LM(aClip,this.An,Dp,B.tz(Al,aOffset),[].concat(
AM,BF),BR,BS,BQ,BP,aBlend,this.HS);else{var left=Al[0]+aOffset[0];var top=Al[1]+
aOffset[1];var right=Al[2]+aOffset[0];var bottom=Al[3]+aOffset[1];if(orient===1)
Bi.I_(aClip,this.An,Dp,left,bottom,1.000000,left,top,1.000000,right,top,1.000000
,right,bottom,1.000000,[].concat(AM,BF),BP,BR,BS,BQ,aBlend,this.HS);else if(orient===
2)Bi.I_(aClip,this.An,Dp,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000
,right,top,1.000000,[].concat(AM,BF),BQ,BP,BR,BS,aBlend,this.HS);else if(orient===
3)Bi.I_(aClip,this.An,Dp,right,top,1.000000,right,bottom,1.000000,left,bottom,1.000000
,left,top,1.000000,[].concat(AM,BF),BS,BQ,BP,BR,aBlend,this.HS);}},KP:function(Z
){var A;if(((this.Ep&&!!this.An)&&(this.An.FrameSize[0]>0))&&(this.An.FrameSize[
1]>0))this.H(B.ty(this.Fc(),this.D_));if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(
this.G);},FD:function(Z){var A;var Dp=this.EF;var Gc=0;if(!!this.An)Gc=this.An.NoOfFrames
*this.An.FrameDelay;if((!!this.timer&&(this.EF<0))&&(Gc>0))this.Bs=this.timer.Bq-(
this.HE*this.An.FrameDelay);if(!!this.timer&&(Gc>0)){var Am=(this.timer.Bq-this.
Bs)|0;Dp=(Am/this.An.FrameDelay)|0;if(Am>=Gc){Dp=Dp%this.An.NoOfFrames;this.Bs=this.
timer.Bq-(Am%Gc);if(!this.Lg){Dp=this.An.NoOfFrames-1;Gc=0;}}}if(((Dp!==this.EF)&&
!!this.L)&&((this.F&0x1)===0x1))this.L.AL(this.G);this.EF=Dp;if(!Gc&&!!this.timer
){B.sM([this,this.FD],this.timer,0);this.timer=null;(A=this.GO)?A[1].call(A[0],this
):null;}},DO:function(E){if(E===this.Ep)return;this.Ep=E;if(((E&&!!this.An)&&(this.
An.FrameSize[0]>0))&&(this.An.FrameSize[1]>0))this.H(B.ty(this.Fc(),this.D_));},
Q:function(E){var A;if((((E===this.Ch)&&(E===this.Cr))&&(E===this.Cp))&&(E===this.
Cq))return;this.Ch=E;this.Cr=E;this.Cp=E;this.Cq=E;if(!!this.L&&((this.F&0x1)===
0x1))this.L.AL(this.G);},J2:function(E){var A;if(this.EM===E)return;this.EM=E;this.
EF=-1;if(!E&&!!this.timer){B.sM([this,this.FD],this.timer,0);this.timer=null;}if(
E){this.timer=B._GetAutoObject(B.uk.HB);B.sz([this,this.FD],this.timer,0);B.lq([
this,this.FD],this);}if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.G);},AS:function(
E){var A;if(E===this.EL)return;this.EL=E;if(!!this.L&&((this.F&0x1)===0x1))this.
L.AL(this.G);},HN:function(E){var A;if(E<0)E=0;if((E===this.HE)&&(this.EF===-1))
return;this.HE=E;if(!this.timer)this.EF=-1;if(!!this.L&&((this.F&0x1)===0x1))this.
L.AL(this.G);},CZ:function(E){var A;if(E===this.An)return;if(!!this.An&&this.An.
HJ)B.sM([this,this.KP],this.An,0);this.An=E;this.EF=-1;if(!!E&&E.HJ)B.sz([this,this.
KP],E,0);if(this.EM){this.J2(false);this.J2(true);}if(((this.Ep&&!!E)&&(E.FrameSize[
0]>0))&&(E.FrameSize[1]>0))this.H(B.ty(this.Fc(),this.D_));if(!!this.L&&((this.F&
0x1)===0x1))this.L.AL(this.G);},Ar:function(E){var A;if(E>255)E=255;if(E<0)E=0;if(
E===this.Be)return;this.Be=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.G);
},GU:function(E){if(E)this.BW(0x1,0x0);else this.BW(0x0,0x1);},Fc:function(){var
A;if(!this.An)return BD;var orient=this.D9;var AN=this.EL;var BF=this.An.FrameSize;
var As=this.G;var width=As[2]-As[0];var height=As[3]-As[1];if(!BF[0]||!BF[1])return BD;
if((orient===1)||(orient===3)){width=height;height=As[2]-As[0];}var AB=[0,0,width
,height];var T=AB;if(((AN&0x40)===0x40)){var JH=((((AB[2]-AB[0])<<16)+((BF[0]/2)|
0))/BF[0])|0;var Ho=((((AB[3]-AB[1])<<16)+((BF[1]/2)|0))/BF[1])|0;var E_=JH;if(Ho>
E_)E_=Ho;T=B.tZ(T,((BF[0]*E_)+32768)>>16);T=B.tW(T,((BF[1]*E_)+32768)>>16);}else
if(((AN&0x80)===0x80)){var JH=((((AB[2]-AB[0])<<16)+((BF[0]/2)|0))/BF[0])|0;var Ho=((((
AB[3]-AB[1])<<16)+((BF[1]/2)|0))/BF[1])|0;var E_=JH;if(Ho<E_)E_=Ho;T=B.tZ(T,((BF[
0]*E_)+32768)>>16);T=B.tW(T,((BF[1]*E_)+32768)>>16);}else if(!((AN&0x100)===0x100
))T=B.tY(T,BF);if((T[2]-T[0])!==(AB[2]-AB[0])){if(((AN&0x4)===0x4))T=B.t0(T,AB[2
]-(T[2]-T[0]));else if(((AN&0x2)===0x2))T=B.t0(T,(AB[0]+(((AB[2]-AB[0])/2)|0))-(((
T[2]-T[0])/2)|0));}if((T[3]-T[1])!==(AB[3]-AB[1])){if(((AN&0x20)===0x20))T=B.t2(
T,AB[3]-(T[3]-T[1]));else if(((AN&0x10)===0x10))T=B.t2(T,(AB[1]+(((AB[3]-AB[1])/
2)|0))-(((T[3]-T[1])/2)|0));}if(!orient)T=B.tz(T,As.slice(0,2));else if(orient===
1){var CH=[As[0]+T[1],As[3]-T[2]];T=[].concat(CH,B.tx(CH,[T[3]-T[1],T[2]-T[0]]));
}else if(orient===2){var CH=[As[2]-T[2],As[3]-T[3]];T=[].concat(CH,B.tx(CH,[T[2]-
T[0],T[3]-T[1]]));}else if(orient===3){var CH=[As[2]-T[3],As[1]+T[0]];T=[].concat(
CH,B.tx(CH,[T[3]-T[1],T[2]-T[0]]));}return B.tz(T,this.D_);},_Init:function(aArg
){B.Core.BO._Init.call(this,aArg);this.__proto__=C.Aq;},_Mark:function(D){var A;
B.Core.BO._Mark.call(this,D);if((A=this.timer)&&(A._cycle!=D))A._Mark(A._cycle=D
);if((A=this.An)&&(A._cycle!=D))A._Mark(A._cycle=D);if((A=this.GO)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);},_className:"Views::Image"};C.Text={A2:null,Fk:null,Bk:
B.hm,String:B.hm,C7:null,DJ:B.qx,Gq:0,FL:0,Cp:0xFFFFFFFF,Cq:0xFFFFFFFF,Cr:0xFFFFFFFF
,Ch:0xFFFFFFFF,FN:0,D_:B.qx,EL:0x12,Be:255,D9:0,Gs:false,Ep:false,HC:false,Lf:false
,CJ:false,DK:function(Bi,aClip,aOffset,A_,aBlend){var A;if((this.Bk===B.hm)||!this.
A2)return;var AN=this.EL;var orient=this.D9;var font=this.A2;var AB=B.tz(this.G,
aOffset);var BR=this.Ch;var BS=this.Cr;var BQ=this.Cq;var BP=this.Cp;var Eh=(((A_+
1)*this.Be)>>8)+1;var Eg=this.Bk.charCodeAt(0)||0;var Al=B.tz(this.Fc(),aOffset);
var Bb=[AB[0]-Al[0],(AB[1]-Al[1])-font.Ascent];if(Eg<1)return;if(Eh<256){BR=(BR&
0x00FFFFFF)|((((((BR>>24)&0xFF)*Eh)>>8)&0xFF)<<24);BS=(BS&0x00FFFFFF)|((((((BS>>
24)&0xFF)*Eh)>>8)&0xFF)<<24);BQ=(BQ&0x00FFFFFF)|((((((BQ>>24)&0xFF)*Eh)>>8)&0xFF
)<<24);BP=(BP&0x00FFFFFF)|((((((BP>>24)&0xFF)*Eh)>>8)&0xFF)<<24);}if(((AN&0x80)===
0x80)){if(this.IO())AN=(AN&~0x80)|0x4;else AN=(AN&~0x80)|0x1;}if(((Eg===1)&&!((AN&
0x40)===0x40))&&!orient){Bi.JU(aClip,font,this.Bk,2,(this.Bk.charCodeAt(1)||0)-1
,AB,Bb,0,orient,BR,BS,BQ,BP,true);return;}var leading=font.Leading;if(this.FL>0)
leading=(this.FL-font.Ascent)-font.Descent;var JG=(font.Ascent+font.Descent)+leading;
var H_=aClip[1]-Al[1];var H$=aClip[3]-Al[1];var G5=Al[2]-Al[0];var AC=0;var K=1;
var Cy=this.Bk.charCodeAt(K)||0;if(orient===1){Bb=[Al[3]-AB[3],(AB[0]-Al[0])-font.
Ascent];H_=aClip[0]-Al[0];H$=aClip[2]-Al[0];G5=Al[3]-Al[1];}else if(orient===2){
Bb=[Al[2]-AB[2],(Al[3]-AB[3])-font.Ascent];H_=Al[3]-aClip[3];H$=Al[3]-aClip[1];}
else if(orient===3){Bb=[AB[1]-Al[1],(Al[2]-AB[2])-font.Ascent];H_=Al[2]-aClip[2];
H$=Al[2]-aClip[0];G5=Al[3]-Al[1];}while(((AC+JG)<H_)&&(Cy>0)){K=K+Cy;AC=AC+JG;Cy=
this.Bk.charCodeAt(K)||0;}while((AC<H$)&&(Cy>0)){var F$=B.tw(Bb,[0,AC]);var KW=0;
var Ij=false;if(((((AN&0x40)===0x40)&&((this.Bk.charCodeAt((K+Cy)-1)||0)!==0x0A)
)&&((this.Bk.charCodeAt(K+1)||0)!==0x0A))&&((this.Bk.charCodeAt(K+Cy)||0)!==0x00
))Ij=true;if(Ij&&!!(AN&0x6)){var KV=K+Cy;var KC=this.Bk.indexOf(String.fromCharCode(
0x20),K+1);var KD=this.Bk.indexOf(String.fromCharCode(0xA0),K+1);if(((KC<0)||(KC>=
KV))&&((KD<0)||(KD>=KV)))Ij=false;}if(Ij)KW=G5;else if(((AN&0x4)===0x4))F$=[(F$[
0]-G5)+font.GK(this.Bk,K+1,Cy-1),F$[1]];else if(((AN&0x2)===0x2))F$=[(F$[0]-((G5
/2)|0))+((font.GK(this.Bk,K+1,Cy-1)/2)|0),F$[1]];Bi.JU(aClip,font,this.Bk,K+1,Cy-
1,AB,F$,KW,orient,BR,BS,BQ,BP,true);K=K+Cy;AC=AC+JG;Cy=this.Bk.charCodeAt(K)||0;
}},H:function(E){var A;if(B.tm(E,this.G))return;var JF=false;if(!this.D9||(this.
D9===2))JF=((A=this.G)[2]-A[0])!==(E[2]-E[0]);else JF=((A=this.G)[3]-A[1])!==(E[
3]-E[1]);if((((JF&&!this.FN)&&this.Gs)&&this.CJ)&&!((this.F&0x2000)===0x2000)){this.
Bk=B.hm;this.CJ=false;B.lq([this,this.Gd],this);}if(((this.HC&&this.CJ)&&!B.tl([(
A=this.G)[2]-A[0],A[3]-A[1]],[E[2]-E[0],E[3]-E[1]]))&&!((this.F&0x2000)===0x2000
)){this.Bk=B.hm;this.CJ=false;B.lq([this,this.Gd],this);}B.Core.BO.H.call(this,E
);B.lq([this,this.JC],this);},D3:function(){if(!!this.C7){this.Jp(this.C7);this.
C7=null;}},Jp:function(aBidi){if(!aBidi)return;B.ng(aBidi);},NZ:function(aSize){
var bidi=null;bidi=B.qk(aSize);return bidi;},JC:function(Z){B.lq(this.Fk,this);}
,Gd:function(Z){B.lq([this,this.Ir],this);},Ir:function(Z){var A;if(this.CJ)return;
var orient=this.D9;var width=(A=this.G)[2]-A[0];var height=(A=this.G)[3]-A[1];var
DG=-1;if((orient===1)||(orient===3)){width=height;height=(A=this.G)[2]-A[0];}if(
this.Gs){if(this.FN>0)DG=this.FN;else if(!this.Ep)DG=width-(this.Gq*2);else DG=width;
if(DG<0)DG=1;}if(!!this.C7){this.Jp(this.C7);this.C7=null;}this.CJ=true;if((this.
String!==B.hm)&&!!this.A2){var length=this.String.length;if(this.Lf)this.C7=this.
NZ(length);this.Bk=this.A2.LI(this.String,0,DG,length,this.C7);if(!!this.C7&&!this.
Ln()){this.Jp(this.C7);this.C7=null;}}else this.Bk=B.hm;this.DJ=AM;if(((this.HC&&(
this.Bk!==B.hm))&&!this.Ep)&&!!this.A2){var AN=this.EL;var font=this.A2;var leading=
font.Leading;var Br=this.Bk;var Iv=this.IO();if(((AN&0x80)===0x80)){if(Iv)AN=(AN&
~0x80)|0x4;else AN=(AN&~0x80)|0x1;}if(this.FL>0)leading=(this.FL-font.Ascent)-font.
Descent;var Hl=(font.Ascent+font.Descent)+leading;var Eg=Br.charCodeAt(0)||0;var
Fv=((height+leading)/Hl)|0;var Jm=false;var H9=false;if(Fv<=0)Fv=1;if(Eg>Fv){var
D2=0;var Hm=0;var Iu=Eg-1;var BV=0;var Cd=Br.length;var tmp=B.hm;if(((AN&0x20)===
0x20))Hm=Eg-Fv;else if(((AN&0x10)===0x10)){Hm=((Eg-Fv)/2)|0;Iu=(Hm+Fv)-1;}else Iu=
Fv-1;Jm=Hm>0;H9=Iu<(Eg-1);for(BV=1;D2<Hm;D2=D2+1)BV=BV+(Br.charCodeAt(BV)||0);if(
H9)for(Cd=BV;D2<Iu;D2=D2+1)Cd=Cd+(Br.charCodeAt(Cd)||0);if(Jm){var Cn=Br.charCodeAt(
BV)||0;tmp=(Dj+B.t9(Br,BV,Cn))+Dj;tmp=B.t4(tmp,0,(Cn+2)&0xFFFF);BV=BV+Cn;if((tmp.
charCodeAt(Cn)||0)===0x0A){tmp=B.t4(tmp,Cn,0xFEFF);tmp=B.t4(tmp,Cn+1,0x0A);}if((
tmp.charCodeAt(2)||0)===0x0A){tmp=B.t4(tmp,2,0xFEFF);tmp=B.t4(tmp,1,0x0A);}else tmp=
B.t4(tmp,1,0xFEFF);}tmp=tmp+B.t9(Br,BV,Cd-BV);if(H9&&(Cd>=BV)){var Cn=Br.charCodeAt(
Cd)||0;var CL=(Dj+B.t9(Br,Cd,Cn))+Dj;CL=B.t4(CL,0,(Cn+2)&0xFFFF);CL=B.t4(CL,1,0xFEFF
);if((CL.charCodeAt(Cn)||0)===0x0A){CL=B.t4(CL,Cn,0xFEFF);CL=B.t4(CL,Cn+1,0x0A);
}if((CL.charCodeAt(2)||0)===0x0A){CL=B.t4(CL,2,0xFEFF);CL=B.t4(CL,1,0x0A);}else CL=
B.t4(CL,1,0xFEFF);tmp=tmp+CL;}Br=String.fromCharCode(Fv&0xFFFF)+tmp;}var D2=0;var
Ii=1;var In=width-(this.Gq*2);if(this.Gs&&(this.FN>0))In=this.FN;Eg=Br.charCodeAt(
0)||0;for(;D2<Eg;D2=D2+1){var FA=Jm&&!D2;var FB=H9&&(D2===(Eg-1));var DA=false;var
DB=false;var GF=Iv;if((Iv&&FA)&&!FB){FA=false;FB=true;}else if((Iv&&FB)&&!FA){FB=
false;FA=true;}var Hp=Ii+1;var Cn=Br.charCodeAt(Ii)||0;var BV=Hp;var Cd=(Hp+Cn)-
2;var Jv=-1;var Jw=-1;if(!this.Gs&&(font.GK(Br,Hp,Cn-1)>In)){if(((AN&0x4)===0x4)
)DA=true;else if(((AN&0x2)===0x2)){DA=true;DB=true;}else DB=true;}if((Br.charCodeAt(
BV)||0)===0x0A)BV=BV+1;if((Br.charCodeAt(Cd)||0)===0x0A)Cd=Cd-1;while(DA&&((Br.charCodeAt(
BV)||0)===0xFEFF))BV=BV+1;while(DB&&((Br.charCodeAt(Cd)||0)===0xFEFF))Cd=Cd-1;DA=
DA&&!FB;DB=DB&&!FA;while((((DA||DB)||FA)||FB)&&(BV<Cd)){if((DA&&(GF||!DB))||FA){
if(Jv>0)Br=B.t4(Br,Jv,0xFEFF);Br=B.t4(Br,BV,0x2026);Jv=BV;BV=BV+1;GF=!GF;FA=false;
if(font.GK(Br,Hp,Cn-1)<=In){DA=false;DB=false;}else DA=DA||!DB;}if((DB&&(!GF||!DA
))||FB){if(Jw>0)Br=B.t4(Br,Jw,0xFEFF);Br=B.t4(Br,Cd,0x2026);Jw=Cd;Cd=Cd-1;GF=!GF;
FB=false;if(font.GK(Br,Hp,Cn-1)<=In){DA=false;DB=false;}else DB=DB||!DA;}}Ii=Ii+
Cn;}this.DJ=[font.JX(Br),((Br.charCodeAt(0)||0)*Hl)-leading];this.Bk=Br;}if(this.
Ep&&(this.Bk!==B.hm)){var CX=[this.Gq,0];if((orient===1)||(orient===3)){CX=[CX[0
],CX[0]];CX=[0,CX[1]];}this.F=this.F|0x2000;this.H(B.ty(B.th(this.Fc(),CX),this.
D_));this.F=this.F&~0x2000;}if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.G);
B.lq([this,this.JC],this);},DO:function(E){if(E===this.Ep)return;this.Ep=E;if(E&&
this.HC){this.Bk=B.hm;this.CJ=false;B.lq([this,this.Gd],this);}if(E&&this.CJ){var
CX=[this.Gq,0];if((this.D9===1)||(this.D9===3)){CX=[CX[0],CX[0]];CX=[0,CX[1]];}this.
F=this.F|0x2000;this.H(B.ty(B.th(this.Fc(),CX),this.D_));this.F=this.F&~0x2000;}
},B0:function(E){if(E===this.Gs)return;this.Gs=E;if(this.CJ){this.Bk=B.hm;this.CJ=
false;B.lq([this,this.Gd],this);}if(E&&!this.FN)this.F=this.F&~0x100;else this.F=
this.F|0x100;},AS:function(E){var A;if(E===this.EL)return;this.EL=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AL(this.G);if(this.HC){this.Bk=B.hm;this.CJ=false;B.lq([
this,this.Gd],this);}if(this.CJ)B.lq([this,this.JC],this);},Y:function(E){if(E===
this.String)return;this.String=E;this.Bk=B.hm;this.CJ=false;B.lq([this,this.Gd],
this);},Ax:function(E){if(E===this.A2)return;this.A2=E;this.Bk=B.hm;this.CJ=false;
B.lq([this,this.Gd],this);},Q:function(E){var A;if((((E===this.Ch)&&(E===this.Cr
))&&(E===this.Cp))&&(E===this.Cq))return;this.Ch=E;this.Cr=E;this.Cp=E;this.Cq=E;
if(!!this.L&&((this.F&0x1)===0x1))this.L.AL(this.G);},Ar:function(E){var A;if(E>
255)E=255;if(E<0)E=0;if(E===this.Be)return;this.Be=E;if(!!this.L&&((this.F&0x1)===
0x1))this.L.AL(this.G);},IO:function(){if(!this.CJ)this.Ir(this);if(!this.C7)return false;
var result=false;var bidi=this.C7;result=B.qj(bidi);return result;},Ln:function(
){if(!this.CJ)this.Ir(this);if(!this.C7)return false;var result=false;var bidi=this.
C7;result=B.sD(bidi);return result;},Fc:function(){var A;if((this.String===B.hm)||
!this.A2)return BD;if(!this.CJ)this.Ir(this);if(this.Bk===B.hm)return BD;var leading=
this.A2.Leading;var Hl=(this.A2.Ascent+this.A2.Descent)+this.A2.Leading;if(this.
FL>0){leading=(this.FL-this.A2.Ascent)-this.A2.Descent;Hl=this.FL;}if(B.tl(this.
DJ,AM))this.DJ=[this.A2.JX(this.Bk),this.DJ[1]];this.DJ=[this.DJ[0],((this.Bk.charCodeAt(
0)||0)*Hl)-leading];var AN=this.EL;var orient=this.D9;var As=this.G;var CX=this.
Gq;var width=As[2]-As[0];var height=As[3]-As[1];if((orient===1)||(orient===3)){width=
height;height=As[2]-As[0];}var AB=[CX,0,width-CX,height];var T=[].concat(AB.slice(
0,2),B.tx(AB.slice(0,2),this.DJ));if(((AN&0x80)===0x80)){if(this.IO())AN=(AN&~0x80
)|0x4;else AN=(AN&~0x80)|0x1;}if(((AN&0x40)===0x40)){var DG=this.FN;if(DG<=0)DG=
width-(this.Gq*2);if(DG<0)DG=0;if(DG>(T[2]-T[0]))T=B.tZ(T,DG);}if((T[2]-T[0])!==(
AB[2]-AB[0])){if(((AN&0x4)===0x4))T=B.t0(T,AB[2]-(T[2]-T[0]));else if(((AN&0x2)===
0x2))T=B.t0(T,(AB[0]+(((AB[2]-AB[0])/2)|0))-(((T[2]-T[0])/2)|0));}if((T[3]-T[1])
!==(AB[3]-AB[1])){if(((AN&0x20)===0x20))T=B.t2(T,AB[3]-(T[3]-T[1]));else if(((AN&
0x10)===0x10))T=B.t2(T,(AB[1]+(((AB[3]-AB[1])/2)|0))-(((T[3]-T[1])/2)|0));}if(!orient
)T=B.tz(T,As.slice(0,2));else if(orient===1){var CH=[As[0]+T[1],As[3]-T[2]];T=[].
concat(CH,B.tx(CH,[this.DJ[1],this.DJ[0]]));}else if(orient===2){var CH=[As[2]-T[
2],As[3]-T[3]];T=[].concat(CH,B.tx(CH,this.DJ));}else if(orient===3){var CH=[As[
2]-T[3],As[1]+T[0]];T=[].concat(CH,B.tx(CH,[this.DJ[1],this.DJ[0]]));}return B.tz(
T,this.D_);},_Init:function(aArg){B.Core.BO._Init.call(this,aArg);this.__proto__=
C.Text;},_Done:function(){this.D3();this.__proto__=B.Core.BO;B.Core.BO._Done.call(
this);},_Mark:function(D){var A;B.Core.BO._Mark.call(this,D);if((A=this.A2)&&(A.
_cycle!=D))A._Mark(A._cycle=D);if((A=this.Fk)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=
D);},_className:"Views::Text"};C.O3={K4:0x1,K3:0x2,K5:0x4,K8:0x8,K7:0x10,K6:0x20
,Pu:0x40,Pv:0x80,PA:0x100};C.PB={K4:0x1,K3:0x2,K5:0x4,K8:0x8,K7:0x10,K6:0x20,Os:
0x40,Or:0x80};C.D9={Pa:0,Ps:1,Pq:2,Pr:3};
C._Init=function(){C.Av.__proto__=B.Core.BO;C.Ac.__proto__=B.Core.BO;C.Aq.__proto__=
B.Core.BO;C.Text.__proto__=B.Core.BO;};C.C2=function(D){};return C;})();

/* Embedded Wizard */