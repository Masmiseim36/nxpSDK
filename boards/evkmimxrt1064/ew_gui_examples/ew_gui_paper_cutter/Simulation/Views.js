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
var AO=[0,0];var BF=[0,0,0,0];var Dh="\uFEFF";
C.Av={Cr:0xFFFFFFFF,Cs:0xFFFFFFFF,Ct:0xFFFFFFFF,Ch:0xFFFFFFFF,DI:function(Bi,aClip
,aOffset,A9,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A9=A9+1;if(A9<256){
var BM=this.Ch;var BN=this.Ct;var BK=this.Cr;var BL=this.Cs;BM=(BM&0x00FFFFFF)|((((
A9*((BM>>24)&0xFF))>>8)&0xFF)<<24);BN=(BN&0x00FFFFFF)|((((A9*((BN>>24)&0xFF))>>8
)&0xFF)<<24);BK=(BK&0x00FFFFFF)|((((A9*((BK>>24)&0xFF))>>8)&0xFF)<<24);BL=(BL&0x00FFFFFF
)|((((A9*((BL>>24)&0xFF))>>8)&0xFF)<<24);Bi.Iv(aClip,B.sq(this.G,aOffset),BM,BN,
BL,BK,aBlend);}else Bi.Iv(aClip,B.sq(this.G,aOffset),this.Ch,this.Ct,this.Cs,this.
Cr,aBlend);},R:function(E){var A;if((((E===this.Ch)&&(E===this.Ct))&&(E===this.Cr
))&&(E===this.Cs))return;this.Ch=E;this.Ct=E;this.Cr=E;this.Cs=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AJ(this.G);},_Init:function(aArg){B.Core.BZ._Init.call(
this,aArg);this.__proto__=C.Av;},_className:"Views::Rectangle"};C.Ad={Width:1,Cr:
0xFFFFFFFF,Cs:0xFFFFFFFF,Ct:0xFFFFFFFF,Ch:0xFFFFFFFF,DI:function(Bi,aClip,aOffset
,A9,aBlend){var A;aBlend=aBlend&&((this.F&0x2)===0x2);A9=A9+1;if(A9<256){var BM=
this.Ch;var BN=this.Ct;var BK=this.Cr;var BL=this.Cs;BM=(BM&0x00FFFFFF)|((((A9*((
BM>>24)&0xFF))>>8)&0xFF)<<24);BN=(BN&0x00FFFFFF)|((((A9*((BN>>24)&0xFF))>>8)&0xFF
)<<24);BK=(BK&0x00FFFFFF)|((((A9*((BK>>24)&0xFF))>>8)&0xFF)<<24);BL=(BL&0x00FFFFFF
)|((((A9*((BL>>24)&0xFF))>>8)&0xFF)<<24);Bi.JA(aClip,B.sq(this.G,aOffset),this.Width
,BM,BN,BL,BK,aBlend);}else Bi.JA(aClip,B.sq(this.G,aOffset),this.Width,this.Ch,this.
Ct,this.Cs,this.Cr,aBlend);},JN:function(E){var A;if(E<0)E=0;if(E===this.Width)return;
this.Width=E;if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);},Ld:function(){
return this.Ch;},R:function(E){var A;if((((E===this.Ch)&&(E===this.Ct))&&(E===this.
Cr))&&(E===this.Cs))return;this.Ch=E;this.Ct=E;this.Cr=E;this.Cs=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AJ(this.G);},_Init:function(aArg){B.Core.BZ._Init.call(
this,aArg);this.__proto__=C.Ad;},_className:"Views::Border"};C.Aq={timer:null,An:
null,GF:null,Bs:0,Ex:0,Cr:0xFFFFFFFF,Cs:0xFFFFFFFF,Ct:0xFFFFFFFF,Ch:0xFFFFFFFF,D6:
B.pm,EF:0x12,Hy:0,Bc:255,D5:0,EG:false,K2:true,Eh:false,HK:true,DI:function(Bi,aClip
,aOffset,A9,aBlend){var A;var Dn=this.Hy;if(this.Ex>=0)Dn=this.Ex;if(!this.An||(
Dn>=this.An.NoOfFrames))return;this.An.Update();var Al=this.E9();var BC=this.An.
FrameSize;var orient=this.D5;if((Al[0]>=Al[2])||(Al[1]>=Al[3]))return;var BM=this.
Ch;var BN=this.Ct;var BL=this.Cs;var BK=this.Cr;var Ec=(((A9+1)*this.Bc)>>8)+1;aBlend=
aBlend&&((this.F&0x2)===0x2);if(Ec<256){BM=(BM&0x00FFFFFF)|((((((BM>>24)&0xFF)*Ec
)>>8)&0xFF)<<24);BN=(BN&0x00FFFFFF)|((((((BN>>24)&0xFF)*Ec)>>8)&0xFF)<<24);BL=(BL&
0x00FFFFFF)|((((((BL>>24)&0xFF)*Ec)>>8)&0xFF)<<24);BK=(BK&0x00FFFFFF)|((((((BK>>
24)&0xFF)*Ec)>>8)&0xFF)<<24);}if(B.sa([Al[2]-Al[0],Al[3]-Al[1]],BC)&&!orient)Bi.
Jx(aClip,this.An,Dn,B.sq(this.G,aOffset),B.sn(this.G.slice(0,2),Al.slice(0,2)),BM
,BN,BL,BK,aBlend);else if(!orient)Bi.Lv(aClip,this.An,Dn,B.sq(Al,aOffset),[].concat(
AO,BC),BM,BN,BL,BK,aBlend,this.HK);else{var left=Al[0]+aOffset[0];var top=Al[1]+
aOffset[1];var right=Al[2]+aOffset[0];var bottom=Al[3]+aOffset[1];if(orient===1)
Bi.IR(aClip,this.An,Dn,left,bottom,1.000000,left,top,1.000000,right,top,1.000000
,right,bottom,1.000000,[].concat(AO,BC),BK,BM,BN,BL,aBlend,this.HK);else if(orient===
2)Bi.IR(aClip,this.An,Dn,right,bottom,1.000000,left,bottom,1.000000,left,top,1.000000
,right,top,1.000000,[].concat(AO,BC),BL,BK,BM,BN,aBlend,this.HK);else if(orient===
3)Bi.IR(aClip,this.An,Dn,right,top,1.000000,right,bottom,1.000000,left,bottom,1.000000
,left,top,1.000000,[].concat(AO,BC),BN,BL,BK,BM,aBlend,this.HK);}},KA:function(Aa
){var A;if(((this.Eh&&!!this.An)&&(this.An.FrameSize[0]>0))&&(this.An.FrameSize[
1]>0))this.H(B.sp(this.E9(),this.D6));if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(
this.G);},Fx:function(Aa){var A;var Dn=this.Ex;var FX=0;if(!!this.An)FX=this.An.
NoOfFrames*this.An.FrameDelay;if((!!this.timer&&(this.Ex<0))&&(FX>0))this.Bs=this.
timer.Bh-(this.Hy*this.An.FrameDelay);if(!!this.timer&&(FX>0)){var Am=(this.timer.
Bh-this.Bs)|0;Dn=(Am/this.An.FrameDelay)|0;if(Am>=FX){Dn=Dn%this.An.NoOfFrames;this.
Bs=this.timer.Bh-(Am%FX);if(!this.K2){Dn=this.An.NoOfFrames-1;FX=0;}}}if(((Dn!==
this.Ex)&&!!this.L)&&((this.F&0x1)===0x1))this.L.AJ(this.G);this.Ex=Dn;if(!FX&&!
!this.timer){B.rA([this,this.Fx],this.timer,0);this.timer=null;(A=this.GF)?A[1].
call(A[0],this):null;}},DN:function(E){if(E===this.Eh)return;this.Eh=E;if(((E&&!
!this.An)&&(this.An.FrameSize[0]>0))&&(this.An.FrameSize[1]>0))this.H(B.sp(this.
E9(),this.D6));},R:function(E){var A;if((((E===this.Ch)&&(E===this.Ct))&&(E===this.
Cr))&&(E===this.Cs))return;this.Ch=E;this.Ct=E;this.Cr=E;this.Cs=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AJ(this.G);},JL:function(E){var A;if(this.EG===E)return;
this.EG=E;this.Ex=-1;if(!E&&!!this.timer){B.rA([this,this.Fx],this.timer,0);this.
timer=null;}if(E){this.timer=B._GetAutoObject(B.tg.Ht);B.rl([this,this.Fx],this.
timer,0);B.kD([this,this.Fx],this);}if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(
this.G);},AV:function(E){var A;if(E===this.EF)return;this.EF=E;if(!!this.L&&((this.
F&0x1)===0x1))this.L.AJ(this.G);},HF:function(E){var A;if(E<0)E=0;if((E===this.Hy
)&&(this.Ex===-1))return;this.Hy=E;if(!this.timer)this.Ex=-1;if(!!this.L&&((this.
F&0x1)===0x1))this.L.AJ(this.G);},CW:function(E){var A;if(E===this.An)return;if(
!!this.An&&this.An.HB)B.rA([this,this.KA],this.An,0);this.An=E;this.Ex=-1;if(!!E&&
E.HB)B.rl([this,this.KA],E,0);if(this.EG){this.JL(false);this.JL(true);}if(((this.
Eh&&!!E)&&(E.FrameSize[0]>0))&&(E.FrameSize[1]>0))this.H(B.sp(this.E9(),this.D6)
);if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);},Ar:function(E){var A;if(
E>255)E=255;if(E<0)E=0;if(E===this.Bc)return;this.Bc=E;if(!!this.L&&((this.F&0x1
)===0x1))this.L.AJ(this.G);},GL:function(E){if(E)this.BQ(0x1,0x0);else this.BQ(0x0
,0x1);},E9:function(){var A;if(!this.An)return BF;var orient=this.D5;var AN=this.
EF;var BC=this.An.FrameSize;var As=this.G;var width=As[2]-As[0];var height=As[3]-
As[1];if(!BC[0]||!BC[1])return BF;if((orient===1)||(orient===3)){width=height;height=
As[2]-As[0];}var AB=[0,0,width,height];var U=AB;if(((AN&0x40)===0x40)){var Jo=((((
AB[2]-AB[0])<<16)+((BC[0]/2)|0))/BC[0])|0;var He=((((AB[3]-AB[1])<<16)+((BC[1]/2
)|0))/BC[1])|0;var E4=Jo;if(He>E4)E4=He;U=B.sU(U,((BC[0]*E4)+32768)>>16);U=B.sR(
U,((BC[1]*E4)+32768)>>16);}else if(((AN&0x80)===0x80)){var Jo=((((AB[2]-AB[0])<<
16)+((BC[0]/2)|0))/BC[0])|0;var He=((((AB[3]-AB[1])<<16)+((BC[1]/2)|0))/BC[1])|0;
var E4=Jo;if(He<E4)E4=He;U=B.sU(U,((BC[0]*E4)+32768)>>16);U=B.sR(U,((BC[1]*E4)+32768
)>>16);}else if(!((AN&0x100)===0x100))U=B.sT(U,BC);if((U[2]-U[0])!==(AB[2]-AB[0]
)){if(((AN&0x4)===0x4))U=B.sV(U,AB[2]-(U[2]-U[0]));else if(((AN&0x2)===0x2))U=B.
sV(U,(AB[0]+(((AB[2]-AB[0])/2)|0))-(((U[2]-U[0])/2)|0));}if((U[3]-U[1])!==(AB[3]-
AB[1])){if(((AN&0x20)===0x20))U=B.sX(U,AB[3]-(U[3]-U[1]));else if(((AN&0x10)===0x10
))U=B.sX(U,(AB[1]+(((AB[3]-AB[1])/2)|0))-(((U[3]-U[1])/2)|0));}if(!orient)U=B.sq(
U,As.slice(0,2));else if(orient===1){var CE=[As[0]+U[1],As[3]-U[2]];U=[].concat(
CE,B.so(CE,[U[3]-U[1],U[2]-U[0]]));}else if(orient===2){var CE=[As[2]-U[2],As[3]-
U[3]];U=[].concat(CE,B.so(CE,[U[2]-U[0],U[3]-U[1]]));}else if(orient===3){var CE=[
As[2]-U[3],As[1]+U[0]];U=[].concat(CE,B.so(CE,[U[3]-U[1],U[2]-U[0]]));}return B.
sq(U,this.D6);},_Init:function(aArg){B.Core.BZ._Init.call(this,aArg);this.__proto__=
C.Aq;},_Mark:function(D){var A;B.Core.BZ._Mark.call(this,D);if((A=this.timer)&&(
A._cycle!=D))A._Mark(A._cycle=D);if((A=this.An)&&(A._cycle!=D))A._Mark(A._cycle=
D);if((A=this.GF)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);},_className:"Views::Image"
};C.Text={A3:null,JO:null,C3:null,DH:B.pm,Bk:B.g3,Gi:0,FE:0,Cr:0xFFFFFFFF,Cs:0xFFFFFFFF
,Ct:0xFFFFFFFF,Ch:0xFFFFFFFF,FG:0,D6:B.pm,EF:0x12,String:B.g3,Bc:255,D5:0,Gk:false
,Eh:false,Hu:false,K0:false,CG:false,DI:function(Bi,aClip,aOffset,A9,aBlend){var
A;if((this.Bk===B.g3)||!this.A3)return;var AN=this.EF;var orient=this.D5;var font=
this.A3;var AB=B.sq(this.G,aOffset);var BM=this.Ch;var BN=this.Ct;var BL=this.Cs;
var BK=this.Cr;var Ec=(((A9+1)*this.Bc)>>8)+1;var Eb=this.Bk.charCodeAt(0)||0;var
Al=B.sq(this.E9(),aOffset);var Ba=[AB[0]-Al[0],(AB[1]-Al[1])-font.Ascent];if(Eb<
1)return;if(Ec<256){BM=(BM&0x00FFFFFF)|((((((BM>>24)&0xFF)*Ec)>>8)&0xFF)<<24);BN=(
BN&0x00FFFFFF)|((((((BN>>24)&0xFF)*Ec)>>8)&0xFF)<<24);BL=(BL&0x00FFFFFF)|((((((BL>>
24)&0xFF)*Ec)>>8)&0xFF)<<24);BK=(BK&0x00FFFFFF)|((((((BK>>24)&0xFF)*Ec)>>8)&0xFF
)<<24);}if(((AN&0x80)===0x80)){if(this.Iw())AN=(AN&~0x80)|0x4;else AN=(AN&~0x80)|
0x1;}if(((Eb===1)&&!((AN&0x40)===0x40))&&!orient){Bi.JB(aClip,font,this.Bk,2,(this.
Bk.charCodeAt(1)||0)-1,AB,Ba,0,orient,BM,BN,BL,BK,true);return;}var leading=font.
Leading;if(this.FE>0)leading=(this.FE-font.Ascent)-font.Descent;var Jn=(font.Ascent+
font.Descent)+leading;var H0=aClip[1]-Al[1];var H1=aClip[3]-Al[1];var GX=Al[2]-Al[
0];var AC=0;var K=1;var Cx=this.Bk.charCodeAt(K)||0;if(orient===1){Ba=[Al[3]-AB[
3],(AB[0]-Al[0])-font.Ascent];H0=aClip[0]-Al[0];H1=aClip[2]-Al[0];GX=Al[3]-Al[1];
}else if(orient===2){Ba=[Al[2]-AB[2],(Al[3]-AB[3])-font.Ascent];H0=Al[3]-aClip[3
];H1=Al[3]-aClip[1];}else if(orient===3){Ba=[AB[1]-Al[1],(Al[2]-AB[2])-font.Ascent
];H0=Al[2]-aClip[2];H1=Al[2]-aClip[0];GX=Al[3]-Al[1];}while(((AC+Jn)<H0)&&(Cx>0)
){K=K+Cx;AC=AC+Jn;Cx=this.Bk.charCodeAt(K)||0;}while((AC<H1)&&(Cx>0)){var FU=B.sn(
Ba,[0,AC]);var KH=0;var H$=false;if(((((AN&0x40)===0x40)&&((this.Bk.charCodeAt((
K+Cx)-1)||0)!==0x0A))&&((this.Bk.charCodeAt(K+1)||0)!==0x0A))&&((this.Bk.charCodeAt(
K+Cx)||0)!==0x00))H$=true;if(H$&&!!(AN&0x6)){var KG=K+Cx;var Ko=this.Bk.indexOf(
String.fromCharCode(0x20),K+1);var Kp=this.Bk.indexOf(String.fromCharCode(0xA0),
K+1);if(((Ko<0)||(Ko>=KG))&&((Kp<0)||(Kp>=KG)))H$=false;}if(H$)KH=GX;else if(((AN&
0x4)===0x4))FU=[(FU[0]-GX)+font.Gz(this.Bk,K+1,Cx-1),FU[1]];else if(((AN&0x2)===
0x2))FU=[(FU[0]-((GX/2)|0))+((font.Gz(this.Bk,K+1,Cx-1)/2)|0),FU[1]];Bi.JB(aClip
,font,this.Bk,K+1,Cx-1,AB,FU,KH,orient,BM,BN,BL,BK,true);K=K+Cx;AC=AC+Jn;Cx=this.
Bk.charCodeAt(K)||0;}},H:function(E){var A;if(B.sb(E,this.G))return;var Jm=false;
if(!this.D5||(this.D5===2))Jm=((A=this.G)[2]-A[0])!==(E[2]-E[0]);else Jm=((A=this.
G)[3]-A[1])!==(E[3]-E[1]);if((((Jm&&!this.FG)&&this.Gk)&&this.CG)&&!((this.F&0x2000
)===0x2000)){this.Bk=B.g3;this.CG=false;B.kD([this,this.FY],this);}if(((this.Hu&&
this.CG)&&!B.sa([(A=this.G)[2]-A[0],A[3]-A[1]],[E[2]-E[0],E[3]-E[1]]))&&!((this.
F&0x2000)===0x2000)){this.Bk=B.g3;this.CG=false;B.kD([this,this.FY],this);}B.Core.
BZ.H.call(this,E);B.kD([this,this.Jj],this);},DZ:function(){if(!!this.C3){this.I9(
this.C3);this.C3=null;}},I9:function(aBidi){if(!aBidi)return;B.mg(aBidi);},NE:function(
aSize){var bidi=null;bidi=B.pd(aSize);return bidi;},Jj:function(Aa){B.kD(this.JO
,this);},FY:function(Aa){B.kD([this,this.Ih],this);},Ih:function(Aa){var A;if(this.
CG)return;var orient=this.D5;var width=(A=this.G)[2]-A[0];var height=(A=this.G)[
3]-A[1];var DE=-1;if((orient===1)||(orient===3)){width=height;height=(A=this.G)[
2]-A[0];}if(this.Gk){if(this.FG>0)DE=this.FG;else if(!this.Eh)DE=width-(this.Gi*
2);else DE=width;if(DE<0)DE=1;}if(!!this.C3);{this.I9(this.C3);this.C3=null;}this.
CG=true;if((this.String!==B.g3)&&!!this.A3){var length=this.String.length;if(this.
K0)this.C3=this.NE(length);this.Bk=this.A3.Lr(this.String,0,DE,length,this.C3);if(
!!this.C3&&!this.K9()){this.I9(this.C3);this.C3=null;}}else this.Bk=B.g3;this.DH=
AO;if((this.Hu&&(this.Bk!==B.g3))&&!this.Eh){var AN=this.EF;var font=this.A3;var
leading=font.Leading;var Bp=this.Bk;var Ik=this.Iw();if(((AN&0x80)===0x80)){if(Ik
)AN=(AN&~0x80)|0x4;else AN=(AN&~0x80)|0x1;}if(this.FE>0)leading=(this.FE-font.Ascent
)-font.Descent;var Hc=(font.Ascent+font.Descent)+leading;var Eb=Bp.charCodeAt(0)||
0;var Fq=((height+leading)/Hc)|0;var I6=false;var HZ=false;if(Fq<=0)Fq=1;if(Eb>Fq
){var DX=0;var Hd=0;var Ij=Eb-1;var BP=0;var Ca=Bp.length;var tmp=B.g3;if(((AN&0x20
)===0x20))Hd=Eb-Fq;else if(((AN&0x10)===0x10)){Hd=((Eb-Fq)/2)|0;Ij=(Hd+Fq)-1;}else
Ij=Fq-1;I6=Hd>0;HZ=Ij<(Eb-1);for(BP=1;DX<Hd;DX=DX+1)BP=BP+(Bp.charCodeAt(BP)||0);
if(HZ)for(Ca=BP;DX<Ij;DX=DX+1)Ca=Ca+(Bp.charCodeAt(Ca)||0);if(I6){var Cp=Bp.charCodeAt(
BP)||0;tmp=(Dh+B.s4(Bp,BP,Cp))+Dh;tmp=B.sZ(tmp,0,(Cp+2)&0xFFFF);BP=BP+Cp;if((tmp.
charCodeAt(Cp)||0)===0x0A){tmp=B.sZ(tmp,Cp,0xFEFF);tmp=B.sZ(tmp,Cp+1,0x0A);}if((
tmp.charCodeAt(2)||0)===0x0A){tmp=B.sZ(tmp,2,0xFEFF);tmp=B.sZ(tmp,1,0x0A);}else tmp=
B.sZ(tmp,1,0xFEFF);}tmp=tmp+B.s4(Bp,BP,Ca-BP);if(HZ&&(Ca>=BP)){var Cp=Bp.charCodeAt(
Ca)||0;var CI=(Dh+B.s4(Bp,Ca,Cp))+Dh;CI=B.sZ(CI,0,(Cp+2)&0xFFFF);CI=B.sZ(CI,1,0xFEFF
);if((CI.charCodeAt(Cp)||0)===0x0A){CI=B.sZ(CI,Cp,0xFEFF);CI=B.sZ(CI,Cp+1,0x0A);
}if((CI.charCodeAt(2)||0)===0x0A){CI=B.sZ(CI,2,0xFEFF);CI=B.sZ(CI,1,0x0A);}else CI=
B.sZ(CI,1,0xFEFF);tmp=tmp+CI;}Bp=String.fromCharCode(Fq&0xFFFF)+tmp;}var DX=0;var
H_=1;var Id=width-(this.Gi*2);if(this.Gk&&(this.FG>0))Id=this.FG;Eb=Bp.charCodeAt(
0)||0;for(;DX<Eb;DX=DX+1){var Fu=I6&&!DX;var Fv=HZ&&(DX===(Eb-1));var Dz=false;var
DA=false;var Gu=Ik;if((Ik&&Fu)&&!Fv){Fu=false;Fv=true;}else if((Ik&&Fv)&&!Fu){Fv=
false;Fu=true;}var Hf=H_+1;var Cp=Bp.charCodeAt(H_)||0;var BP=Hf;var Ca=(Hf+Cp)-
2;var Jd=-1;var Je=-1;if(!this.Gk&&(font.Gz(Bp,Hf,Cp-1)>Id)){if(((AN&0x4)===0x4)
)Dz=true;else if(((AN&0x2)===0x2)){Dz=true;DA=true;}else DA=true;}if((Bp.charCodeAt(
BP)||0)===0x0A)BP=BP+1;if((Bp.charCodeAt(Ca)||0)===0x0A)Ca=Ca-1;while(Dz&&((Bp.charCodeAt(
BP)||0)===0xFEFF))BP=BP+1;while(DA&&((Bp.charCodeAt(Ca)||0)===0xFEFF))Ca=Ca-1;Dz=
Dz&&!Fv;DA=DA&&!Fu;while((((Dz||DA)||Fu)||Fv)&&(BP<Ca)){if((Dz&&(Gu||!DA))||Fu){
if(Jd>0)Bp=B.sZ(Bp,Jd,0xFEFF);Bp=B.sZ(Bp,BP,0x2026);Jd=BP;BP=BP+1;Gu=!Gu;Fu=false;
if(font.Gz(Bp,Hf,Cp-1)<=Id){Dz=false;DA=false;}else Dz=Dz||!DA;}if((DA&&(!Gu||!Dz
))||Fv){if(Je>0)Bp=B.sZ(Bp,Je,0xFEFF);Bp=B.sZ(Bp,Ca,0x2026);Je=Ca;Ca=Ca-1;Gu=!Gu;
Fv=false;if(font.Gz(Bp,Hf,Cp-1)<=Id){Dz=false;DA=false;}else DA=DA||!Dz;}}H_=H_+
Cp;}this.DH=[font.JE(Bp),((Bp.charCodeAt(0)||0)*Hc)-leading];this.Bk=Bp;}if(this.
Eh&&(this.Bk!==B.g3)){var CT=[this.Gi,0];if((orient===1)||(orient===3)){CT=[CT[0
],CT[0]];CT=[0,CT[1]];}this.F=this.F|0x2000;this.H(B.sp(B.r7(this.E9(),CT),this.
D6));this.F=this.F&~0x2000;}if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);
B.kD([this,this.Jj],this);},DN:function(E){if(E===this.Eh)return;this.Eh=E;if(E&&
this.Hu){this.Bk=B.g3;this.CG=false;B.kD([this,this.FY],this);}if(E&&this.CG){var
CT=[this.Gi,0];if((this.D5===1)||(this.D5===3)){CT=[CT[0],CT[0]];CT=[0,CT[1]];}this.
F=this.F|0x2000;this.H(B.sp(B.r7(this.E9(),CT),this.D6));this.F=this.F&~0x2000;}
},BW:function(E){if(E===this.Gk)return;this.Gk=E;if(this.CG){this.Bk=B.g3;this.CG=
false;B.kD([this,this.FY],this);}if(E&&!this.FG)this.F=this.F&~0x100;else this.F=
this.F|0x100;},AV:function(E){var A;if(E===this.EF)return;this.EF=E;if(!!this.L&&((
this.F&0x1)===0x1))this.L.AJ(this.G);if(this.Hu){this.Bk=B.g3;this.CG=false;B.kD([
this,this.FY],this);}if(this.CG)B.kD([this,this.Jj],this);},Z:function(E){if(E===
this.String)return;this.String=E;this.Bk=B.g3;this.CG=false;B.kD([this,this.FY],
this);},Aw:function(E){if(E===this.A3)return;this.A3=E;this.Bk=B.g3;this.CG=false;
B.kD([this,this.FY],this);},R:function(E){var A;if((((E===this.Ch)&&(E===this.Ct
))&&(E===this.Cr))&&(E===this.Cs))return;this.Ch=E;this.Ct=E;this.Cr=E;this.Cs=E;
if(!!this.L&&((this.F&0x1)===0x1))this.L.AJ(this.G);},Ar:function(E){var A;if(E>
255)E=255;if(E<0)E=0;if(E===this.Bc)return;this.Bc=E;if(!!this.L&&((this.F&0x1)===
0x1))this.L.AJ(this.G);},Iw:function(){if(!this.CG)this.Ih(this);if(!this.C3)return false;
var result=false;var bidi=this.C3;result=B.pc(bidi);return result;},K9:function(
){if(!this.CG)this.Ih(this);if(!this.C3)return false;var result=false;var bidi=this.
C3;result=B.rq(bidi);return result;},E9:function(){var A;if((this.String===B.g3)||
!this.A3)return BF;if(!this.CG)this.Ih(this);if(this.Bk===B.g3)return BF;var leading=
this.A3.Leading;var Hc=(this.A3.Ascent+this.A3.Descent)+this.A3.Leading;if(this.
FE>0){leading=(this.FE-this.A3.Ascent)-this.A3.Descent;Hc=this.FE;}if(B.sa(this.
DH,AO))this.DH=[this.A3.JE(this.Bk),this.DH[1]];this.DH=[this.DH[0],((this.Bk.charCodeAt(
0)||0)*Hc)-leading];var AN=this.EF;var orient=this.D5;var As=this.G;var CT=this.
Gi;var width=As[2]-As[0];var height=As[3]-As[1];if((orient===1)||(orient===3)){width=
height;height=As[2]-As[0];}var AB=[CT,0,width-CT,height];var U=[].concat(AB.slice(
0,2),B.so(AB.slice(0,2),this.DH));if(((AN&0x80)===0x80)){if(this.Iw())AN=(AN&~0x80
)|0x4;else AN=(AN&~0x80)|0x1;}if(((AN&0x40)===0x40)){var DE=this.FG;if(DE<=0)DE=
width-(this.Gi*2);if(DE<0)DE=0;if(DE>(U[2]-U[0]))U=B.sU(U,DE);}if((U[2]-U[0])!==(
AB[2]-AB[0])){if(((AN&0x4)===0x4))U=B.sV(U,AB[2]-(U[2]-U[0]));else if(((AN&0x2)===
0x2))U=B.sV(U,(AB[0]+(((AB[2]-AB[0])/2)|0))-(((U[2]-U[0])/2)|0));}if((U[3]-U[1])
!==(AB[3]-AB[1])){if(((AN&0x20)===0x20))U=B.sX(U,AB[3]-(U[3]-U[1]));else if(((AN&
0x10)===0x10))U=B.sX(U,(AB[1]+(((AB[3]-AB[1])/2)|0))-(((U[3]-U[1])/2)|0));}if(!orient
)U=B.sq(U,As.slice(0,2));else if(orient===1){var CE=[As[0]+U[1],As[3]-U[2]];U=[].
concat(CE,B.so(CE,[this.DH[1],this.DH[0]]));}else if(orient===2){var CE=[As[2]-U[
2],As[3]-U[3]];U=[].concat(CE,B.so(CE,this.DH));}else if(orient===3){var CE=[As[
2]-U[3],As[1]+U[0]];U=[].concat(CE,B.so(CE,[this.DH[1],this.DH[0]]));}return B.sq(
U,this.D6);},_Init:function(aArg){B.Core.BZ._Init.call(this,aArg);this.__proto__=
C.Text;},_Done:function(){this.__proto__=C.Text;this.DZ();B.Core.BZ._Done.call(this
);},_Mark:function(D){var A;B.Core.BZ._Mark.call(this,D);if((A=this.A3)&&(A._cycle
!=D))A._Mark(A._cycle=D);if((A=this.JO)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D
);},_className:"Views::Text"};C.OE={KO:0x1,KN:0x2,KP:0x4,KS:0x8,KR:0x10,KQ:0x20,
O3:0x40,O4:0x80,O9:0x100};C.O_={KO:0x1,KN:0x2,KP:0x4,KS:0x8,KR:0x10,KQ:0x20,N7:0x40
,N6:0x80};C.D5={OL:0,O1:1,OZ:2,O0:3};
C._Init=function(){C.Av.__proto__=B.Core.BZ;C.Ad.__proto__=B.Core.BZ;C.Aq.__proto__=
B.Core.BZ;C.Text.__proto__=B.Core.BZ;};C.CY=function(D){};return C;})();

/* Embedded Wizard */