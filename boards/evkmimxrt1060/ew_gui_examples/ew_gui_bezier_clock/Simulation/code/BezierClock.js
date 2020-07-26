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
);if(EmWiApp.BezierClock)throw new Error("The unit file 'BezierClock.js' included twice!"
);EmWiApp.BezierClock=(function(){var B=EmWiApp;var E={};
var AJ=[0,-3];var Be=[0,0,172,150];var Dh=[119,0,162,150];var CK=[0,80];var Ft=[65
,0,107,150];var Iy=[10,0,53,150];var Jg=[102,57,122,89];var Kx=":";var Ky=[47,57
,67,89];var LD=[53,99];var M4=[10,99];var O2=[52,51];var O3=[9,51];var O4=[107,51
];var O5=[65,51];var O6=[107,99];var O7=[65,99];var O8=[162,99];var O9=[119,99];
var O_=[162,51];var O$=[119,51];var Pa=", ";var Pb=" ";var HN=[0,0,480,272];var Um=[
60,215];var Un=[419,215];var Q$=[419,271];var Uo=[60,271];var Ra=[419,0];var Uq=[
479,0];var Ur=[479,271];var Pc=[60,0];var Us=[419,56];var Ut=[60,56];var Uu=[0,172
,480,226];var Uv="Text";var Uw=[188,148,218,178];var M5=[15,15];var Ux=[188,112,
218,142];var Uy=[47,53,119,176];var Uz=[119,53,191,176];var UA=[215,53,287,176];
var UB=[287,53,359,176];var UC=[361,118,393,172];var UD=[399,118,431,172];var UE=[
0,0,480,20];var Rb=[0,250,480,272];var UF=[460,0,480,272];var UG=[348,200,468,250
];var UH="Confirm";var UI=[12,200,132,250];var UJ="Cancel";var UK=[289,39,461,189
];var UL=[19,39,279,189];var UM=[420,0];var UN=[420,36];var UO=[60,36];var UP=[0
,0,480,18];var UQ=[0,0,260,150];var Rc=[160,0,250,150];var Rd=[79,0,149,150];var
Re=[10,0,67,150];var UR=[67,51];var US=[10,51];var UT=[67,99];var UU=[149,99];var
UV=[79,99];var UW=[149,51];var UX=[79,51];var UY=[249,51];var UZ=[160,51];var U0=[
249,99];var U1=[160,99];var Rf=[10,0];var Rg=[0,10];var U2=[0,0,180,250];var U3=[
0,0,160,160];var Pd=[0,0,190,160];var Pe=[0,0];var U4=[190,270,310,390];var U5=[
225,144,255,174];var U6=[225,116,255,146];var U7=[133,99,177,169];var U8=[182,99
,226,169];var U9=[254,99,298,169];var U_=[302,99,346,169];var U$=[215,172,238,206
];var Va=[240,172,265,206];var Vb=[112,8,368,264];var Rh=[60,270];var Vc=[0,270];
var Vd=[0,0,20,270];var Iz=[0,0,200,200];var Ve=[60,224];var Vf=[420,224];var Vg=[
420,270];var Vh=[20,90,150,160];var Vi=[175,90,305,160];var Vj=[330,90,460,160];
E.Ew={In:null,E6:null,E4:null,E3:null,B9:null,B7:null,B6:null,HI:null,HH:null,CZ:
null,C0:null,C1:null,C2:null,C3:null,GW:null,I0:0xFFFFFFFF,I1:0x44FFFFFF,Fh:0xFFC1C1C1
,Fg:0xFFE1E1E1,LR:false,Mf:false,P1:false,Ha:function(aSize){this.B6.FX((aSize[1
]/3)|0);this.B7.FX((aSize[1]/3)|0);this.B9.FX((aSize[1]/3)|0);this.B6.BY((aSize[
1]/3)|0);this.B7.BY((aSize[1]/3)|0);this.B9.BY((aSize[1]/3)|0);this.E3.FY([0,(aSize[
1]/3)|0]);this.E4.FY([0,(aSize[1]/3)|0]);this.E6.FY([0,(aSize[1]/3)|0]);},DR:function(
B$){var A;B.ul.Ev.DR.call(this,B$);var Dk=((B$&0x10)===0x10);var KM=((B$&0x20)===
0x20);var LW=(this.E3.DQ||this.E4.DQ)||this.E6.DQ;if(LW){this.GW.Aa(this.Fh);this.
CZ.Aa(this.Fh);this.C0.Aa(this.Fh);this.C1.Aa(this.Fh);this.C2.Aa(this.Fh);this.
C3.Aa(this.Fh);}else{this.GW.Aa(this.Fg);this.CZ.Aa(this.Fg);this.C0.Aa(this.Fg);
this.C1.Aa(this.Fg);this.C2.Aa(this.Fg);this.C3.Aa(this.Fg);}this.LR=Dk;this.Mf=
KM;this.P1=LW;},JL:function(Af){this.Bn();if((!this.E3.DQ&&!this.E4.DQ)&&!this.E6.
DQ)B.lq(this.In,this);},JO:function(Af){this.Bn();},TB:function(Af){var A;var GG=
this.B9.EY;var Ar=(B.uj.Text.isPrototypeOf(A=this.B9.AE)?A:null);if(!Ar)return;Ar.
Cu(B.tD(GG,2,10));Ar.Fl(B.s$(E.Gl));Ar.Aa(0xFFFFFFFF);Ar.EZ(0x12);Ar.BY(AJ);Ar.P(
B.tY(Ar.H,[(A=this.B9.H)[2]-A[0],this.B9.AY]));},Tz:function(Af){var A;var GG=this.
B7.EY;var Ar=(B.uj.Text.isPrototypeOf(A=this.B7.AE)?A:null);if(!Ar)return;Ar.Cu(
B.tD(GG,2,10));Ar.Fl(B.s$(E.Gl));Ar.Aa(0xFFFFFFFF);Ar.EZ(0x12);Ar.BY(AJ);Ar.P(B.
tY(Ar.H,[(A=this.B7.H)[2]-A[0],this.B7.AY]));},Ty:function(Af){var A;var GG=this.
B6.EY;var Ar=(B.uj.Text.isPrototypeOf(A=this.B6.AE)?A:null);if(!Ar)return;Ar.Cu(
B.tD(GG,2,10));Ar.Fl(B.s$(E.Gl));Ar.Aa(0xFFFFFFFF);Ar.EZ(0x12);Ar.BY(AJ);Ar.P(B.
tY(Ar.H,[(A=this.B6.H)[2]-A[0],this.B6.AY]));},Tw:function(){return(((-this.B9.AW
/this.B9.AY)|0)+1)%60;},MN:function(C){if(C<0)C=0;if(C>59)C=59;this.B9.BY((C-1)*-
this.B9.AY);},Tt:function(){return(((-this.B7.AW/this.B7.AY)|0)+1)%60;},ML:function(
C){if(C<0)C=0;if(C>59)C=59;this.B7.BY((C-1)*-this.B7.AY);},Ts:function(){return(((-
this.B6.AW/this.B6.AY)|0)+1)%24;},MK:function(C){if(C<0)C=0;if(C>23)C=23;this.B6.
BY((C-1)*-this.B6.AY);},VQ:function(Af){var A;var K=this.B6.Aw;while(!!K){var Y=(
B.uj.Text.isPrototypeOf(K)?K:null);if(!!Y){Y.Aa(this.I1);}K=K.Ad;}var R4=(B.uj.Text.
isPrototypeOf(A=this.B6.J8(this.B6.J7(B.s_(this.B6.H))))?A:null);if(!!R4){R4.Aa(
this.I0);}},VS:function(Af){var A;var K=this.B7.Aw;while(!!K){var Y=(B.uj.Text.isPrototypeOf(
K)?K:null);if(!!Y){Y.Aa(this.I1);}K=K.Ad;}var R5=(B.uj.Text.isPrototypeOf(A=this.
B7.J8(this.B7.J7(B.s_(this.B7.H))))?A:null);if(!!R5){R5.Aa(this.I0);}},VV:function(
Af){var A;var K=this.B9.Aw;while(!!K){var Y=(B.uj.Text.isPrototypeOf(K)?K:null);
if(!!Y){Y.Aa(this.I1);}K=K.Ad;}var R7=(B.uj.Text.isPrototypeOf(A=this.B9.J8(this.
B9.J7(B.s_(this.B9.H))))?A:null);if(!!R7){R7.Aa(this.I0);}},_Init:function(aArg){
B.ul.Ev._Init.call(this,aArg);B.Core.Iv._Init.call(this.E6={M:this},0);B.Core.Iv.
_Init.call(this.E4={M:this},0);B.Core.Iv._Init.call(this.E3={M:this},0);B.Core.HK.
_Init.call(this.B9={M:this},0);B.Core.HK._Init.call(this.B7={M:this},0);B.Core.HK.
_Init.call(this.B6={M:this},0);B.uj.Text._Init.call(this.HI={M:this},0);B.uj.Text.
_Init.call(this.HH={M:this},0);B.uj.Cr._Init.call(this.CZ={M:this},0);B.uj.Cr._Init.
call(this.C0={M:this},0);B.uj.Cr._Init.call(this.C1={M:this},0);B.uj.Cr._Init.call(
this.C2={M:this},0);B.uj.Cr._Init.call(this.C3={M:this},0);B.uj.Cr._Init.call(this.
GW={M:this},0);this.__proto__=E.Ew;this.P(Be);this.E6.P(Dh);this.E6.FY(CK);this.
E6.Kg(0.200000);this.E4.P(Ft);this.E4.FY(CK);this.E4.Kg(0.200000);this.E3.P(Iy);
this.E3.FY(CK);this.E3.Kg(0.200000);this.B9.P(Dh);this.B9.Kf(true);this.B9.BY(80
);this.B9.FX(80);this.B9.Hw(60);this.B7.P(Ft);this.B7.Kf(true);this.B7.BY(80);this.
B7.FX(80);this.B7.Hw(60);this.B6.P(Iy);this.B6.Kf(true);this.B6.BY(80);this.B6.FX(
80);this.B6.Hw(24);this.HI.P(Jg);this.HI.Cu(Kx);this.HI.Aa(0xFFFFFFFF);this.HH.P(
Ky);this.HH.Cu(Kx);this.HH.Aa(0xFFFFFFFF);this.CZ.Ct(LD);this.CZ.Cs(M4);this.CZ.
CI(3);this.C0.Ct(O2);this.C0.Cs(O3);this.C0.CI(3);this.C1.Ct(O4);this.C1.Cs(O5);
this.C1.CI(3);this.C2.Ct(O6);this.C2.Cs(O7);this.C2.CI(3);this.C3.Ct(O8);this.C3.
Cs(O9);this.C3.CI(3);this.GW.Ct(O_);this.GW.Cs(O$);this.GW.CI(3);this.Z(this.E6,
0);this.Z(this.E4,0);this.Z(this.E3,0);this.Z(this.B9,0);this.Z(this.B7,0);this.
Z(this.B6,0);this.Z(this.HI,0);this.Z(this.HH,0);this.Z(this.CZ,0);this.Z(this.C0
,0);this.Z(this.C1,0);this.Z(this.C2,0);this.Z(this.C3,0);this.Z(this.GW,0);this.
E6.Et=[this,this.JL];this.E6.Dr=[this,this.JO];this.E4.Et=[this,this.JL];this.E4.
Dr=[this,this.JO];this.E3.Et=[this,this.JL];this.E3.Dr=[this,this.JO];this.B9.DO=[
this,this.VV];this.B9.FT=[this,this.TB];this.B9.Kj(this.E6);this.B7.DO=[this,this.
VS];this.B7.FT=[this,this.Tz];this.B7.Kj(this.E4);this.B6.DO=[this,this.VQ];this.
B6.FT=[this,this.Ty];this.B6.Kj(this.E3);this.HI.Fl(B.s$(E.Gl));this.HH.Fl(B.s$(
E.Gl));},_Done:function(){this.__proto__=B.ul.Ev;this.E6._Done();this.E4._Done();
this.E3._Done();this.B9._Done();this.B7._Done();this.B6._Done();this.HI._Done();
this.HH._Done();this.CZ._Done();this.C0._Done();this.C1._Done();this.C2._Done();
this.C3._Done();this.GW._Done();B.ul.Ev._Done.call(this);},_ReInit:function(){B.
ul.Ev._ReInit.call(this);this.E6._ReInit();this.E4._ReInit();this.E3._ReInit();this.
B9._ReInit();this.B7._ReInit();this.B6._ReInit();this.HI._ReInit();this.HH._ReInit(
);this.CZ._ReInit();this.C0._ReInit();this.C1._ReInit();this.C2._ReInit();this.C3.
_ReInit();this.GW._ReInit();},_Mark:function(D){var A;B.ul.Ev._Mark.call(this,D);
if((A=this.In)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.E6)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.E4)._cycle!=D)A._Mark(A._cycle=D);if((A=this.E3
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B9)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.B7)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B6)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.HI)._cycle!=D)A._Mark(A._cycle=D);if((A=this.HH)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.CZ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C0)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.C1)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C2
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C3)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.GW)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::TimePicker"};
E.GS={Timer:null,Ah:null,CW:null,G7:null,G9:null,G_:null,Date:null,CU:null,CV:null
,B1:null,B2:null,B3:null,B4:null,BU:null,BV:null,D6:null,D5:null,Ih:null,CY:function(
aArg){B.lq([this,this.RB],this);},VY:function(Af){var A;if(!this.Hu(false))return;
this.G.E1(B._NewObject(E.MZ,0),B._GetAutoObject(B.uk.Ko),B._GetAutoObject(B.uk.Kp
),null,null,B._GetAutoObject(B.uk.Ko),null,null,null,null,false);(E.K0.isPrototypeOf(
A=this.G)?A:null).Az.ShiftLeft();},VW:function(Af){if(this.Hu(false))this.G.E1(B.
_NewObject(E.MU,0),B._GetAutoObject(B.uk.G5),B._GetAutoObject(B.uk.Gr),null,null
,B._GetAutoObject(B.uk.G5),null,null,null,null,false);},VX:function(Af){if(this.
Hu(false))this.G.E1(B._NewObject(E.MY,0),B._GetAutoObject(B.uk.Gr),B._GetAutoObject(
B.uk.G5),null,null,B._GetAutoObject(B.uk.Gr),null,null,null,null,false);},Sd:function(
Af){var DT=this.Ah.QJ();this.B1.DN((DT.Lh/10)|0);this.B2.DN(DT.Lh%10);this.B3.DN((
DT.Ll/10)|0);this.B4.DN(DT.Ll%10);this.BU.DN((DT.LB/10)|0);this.BV.DN(DT.LB%10);
this.Date.Cu((((DT.VE(DT.Tq(),true)+Pa)+DT.VF(DT.Lm,false))+Pb)+B.tD(DT.K5,2,10)
);},VC:function(Af){var FL=700;this.B1.DM(FL);this.B2.DM(FL);this.B3.DM(FL);this.
B4.DM(FL);this.BU.DM(FL);this.BV.DM(FL);this.Sd(this);},RB:function(Af){var FL=2000;
this.B1.DM(FL);this.B2.DM(FL);this.B3.DM(FL);this.B4.DM(FL);this.BU.DM(FL);this.
BV.DM(FL);this.Sd(this);this.Timer.Is=[this,this.VC];},_Init:function(aArg){B.Core.
J._Init.call(this,aArg);B.Core.Timer._Init.call(this.Timer={M:this},0);B.Core.Ah.
_Init.call(this.Ah={M:this},0);B.Graphics.CG._Init.call(this.CW={M:this},0);B.Core.
G4._Init.call(this.G7={M:this},0);B.Core.G4._Init.call(this.G9={M:this},0);B.Core.
G4._Init.call(this.G_={M:this},0);B.uj.Text._Init.call(this.Date={M:this},0);B.uj.
Cf._Init.call(this.CU={M:this},0);B.uj.Cf._Init.call(this.CV={M:this},0);E.Cp._Init.
call(this.B1={M:this},0);E.Cp._Init.call(this.B2={M:this},0);E.Cp._Init.call(this.
B3={M:this},0);E.Cp._Init.call(this.B4={M:this},0);E.Cp._Init.call(this.BU={M:this
},0);E.Cp._Init.call(this.BV={M:this},0);B.uj.FQ._Init.call(this.D6={M:this},0);
B.uj.FQ._Init.call(this.D5={M:this},0);B.uj.FQ._Init.call(this.Ih={M:this},0);this.
__proto__=E.GS;this.P(HN);this.Timer.Ip(2000);this.Timer.BG(true);this.CW.QU(20);
this.CW.CB(360.000000);this.CW.OM(6.100000);this.CW.Ir(2);this.G7.G1(Um);this.G7.
G0(Un);this.G7.Ct(Q$);this.G7.Cs(Uo);this.G9.G1(Ra);this.G9.G0(Uq);this.G9.Ct(Ur
);this.G9.Cs(Q$);this.G_.G1(Pc);this.G_.G0(Ra);this.G_.Ct(Us);this.G_.Cs(Ut);this.
Date.Dc(0x3);this.Date.P(Uu);this.Date.Cu(Uv);this.CU.Dc(0x3);this.CU.P(Uw);this.
CU.Aa(0xDDFFFFFF);this.CU.Eu(M5);this.CV.Dc(0x3);this.CV.P(Ux);this.CV.Aa(0xDDFFFFFF
);this.CV.Eu(M5);this.B1.Dc(0x3);this.B1.P(Uy);this.B1.FZ(6.100000);this.B1.FV(60
);this.B1.FW(false);this.B2.Dc(0x3);this.B2.P(Uz);this.B2.FV(60);this.B2.FZ(6.100000
);this.B2.FW(false);this.B3.Dc(0x3);this.B3.P(UA);this.B3.FV(60);this.B3.FZ(6.100000
);this.B3.FW(false);this.B4.Dc(0x3);this.B4.P(UB);this.B4.FV(60);this.B4.FZ(6.100000
);this.B4.FW(false);this.BU.Dc(0x3);this.BU.P(UC);this.BU.FV(50);this.BU.Aa(0xDDFFFFFF
);this.BU.FZ(2.300000);this.BU.FW(false);this.BV.Dc(0x3);this.BV.P(UD);this.BV.FV(
50);this.BV.Aa(0xDDFFFFFF);this.BV.FZ(2.300000);this.BV.FW(false);this.D6.P(UE);
this.D6.Fm(1);this.D5.P(Rb);this.D5.Fm(0);this.Ih.P(UF);this.Ih.Fm(2);this.Z(this.
G7,0);this.Z(this.G9,0);this.Z(this.G_,0);this.Z(this.Date,0);this.Z(this.CU,0);
this.Z(this.CV,0);this.Z(this.B1,0);this.Z(this.B2,0);this.Z(this.B3,0);this.Z(this.
B4,0);this.Z(this.BU,0);this.Z(this.BV,0);this.Z(this.D6,0);this.Z(this.D5,0);this.
Z(this.Ih,0);this.Timer.Is=[this,this.RB];this.G7.CH=[this,this.VW];this.G9.CH=[
this,this.VY];this.G_.CH=[this,this.VX];this.Date.Fl(B.s$(E.Gl));this.CU.EL(this.
CW);this.CV.EL(this.CW);this.D6.Dq(B.s$(B.ui.Ik));this.D5.Dq(B.s$(B.ui.Ik));this.
Ih.Dq(B.s$(B.ui.Ik));this.CY(aArg);},_Done:function(){this.__proto__=B.Core.J;this.
Timer._Done();this.Ah._Done();this.CW._Done();this.G7._Done();this.G9._Done();this.
G_._Done();this.Date._Done();this.CU._Done();this.CV._Done();this.B1._Done();this.
B2._Done();this.B3._Done();this.B4._Done();this.BU._Done();this.BV._Done();this.
D6._Done();this.D5._Done();this.Ih._Done();B.Core.J._Done.call(this);},_ReInit:function(
){B.Core.J._ReInit.call(this);this.Timer._ReInit();this.Ah._ReInit();this.CW._ReInit(
);this.G7._ReInit();this.G9._ReInit();this.G_._ReInit();this.Date._ReInit();this.
CU._ReInit();this.CV._ReInit();this.B1._ReInit();this.B2._ReInit();this.B3._ReInit(
);this.B4._ReInit();this.BU._ReInit();this.BV._ReInit();this.D6._ReInit();this.D5.
_ReInit();this.Ih._ReInit();},_Mark:function(D){var A;B.Core.J._Mark.call(this,D
);if((A=this.Timer)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ah)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.CW)._cycle!=D)A._Mark(A._cycle=D);if((A=this.G7)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.G9)._cycle!=D)A._Mark(A._cycle=D);if((A=this.G_
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Date)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.CU)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CV)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.B1)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B2)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.B3)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B4)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.BU)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BV
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.D6)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.D5)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ih)._cycle!=D)A._Mark(A._cycle=
D);},_className:"BezierClock::ClockScreen"};E.K0={HB:null,Az:null,IZ:null,KY:0,CY:
function(aArg){this.Q2(B._NewObject(E.GS,0),null,null,null,null,null,null,null,null
,false);B.lq([this,this.RO],this);},RO:function(Af){this.Az.GY(B._GetAutoObject(
E.Device).FO);},Sk:function(Af){switch(this.KY){case 2:this.E1(B._NewObject(E.MY
,0),B._GetAutoObject(B.uk.Gr),B._GetAutoObject(B.uk.G5),null,null,B._GetAutoObject(
B.uk.Gr),null,null,null,null,false);break;case 3:if(B._GetAutoObject(E.Device).FO===
2)B._GetAutoObject(E.Device).GY(1);else B._GetAutoObject(E.Device).GY(2);break;case
4:this.E1(B._NewObject(E.GS,0),B._GetAutoObject(B.uk.G5),null,null,null,null,null
,null,null,null,false);break;case 6:{this.E1(B._NewObject(E.MZ,0),B._GetAutoObject(
B.uk.Ko),B._GetAutoObject(B.uk.Kp),null,null,B._GetAutoObject(B.uk.Ko),null,null
,null,null,false);this.Az.ShiftLeft();}break;case 7:{this.E1(B._NewObject(E.GS,0
),B._GetAutoObject(B.uk.Kp),null,null,null,null,null,null,null,null,false);this.
Az.ShiftRight();}break;case 8:this.E1(B._NewObject(E.MU,0),B._GetAutoObject(B.uk.
G5),B._GetAutoObject(B.uk.Gr),null,null,B._GetAutoObject(B.uk.G5),null,null,null
,null,false);break;case 9:this.E1(B._NewObject(E.GS,0),B._GetAutoObject(B.uk.Gr)
,null,null,null,null,null,null,null,null,false);break;default:;}this.KY=this.KY+
1;if(this.KY>9)this.KY=0;},_Init:function(aArg){B.Core.J._Init.call(this,aArg);B.
Core.HB._Init.call(this.HB={M:this},0);E.Az._Init.call(this.Az={M:this},0);B.Core.
Timer._Init.call(this.IZ={M:this},0);this.__proto__=E.K0;var A;this.P(HN);this.Az.
P(HN);this.IZ.Lu(2000);this.IZ.Ip(0);this.Z(this.Az,0);this.HB.OH=[this,this.RO];
this.HB.I8([A=B._GetAutoObject(E.Device),A.I6,A.GY]);this.IZ.Is=[this,this.Sk];this.
CY(aArg);},_Done:function(){this.__proto__=B.Core.J;this.HB._Done();this.Az._Done(
);this.IZ._Done();B.Core.J._Done.call(this);},_ReInit:function(){B.Core.J._ReInit.
call(this);this.HB._ReInit();this.Az._ReInit();this.IZ._ReInit();},_Mark:function(
D){var A;B.Core.J._Mark.call(this,D);if((A=this.HB)._cycle!=D)A._Mark(A._cycle=D
);if((A=this.Az)._cycle!=D)A._Mark(A._cycle=D);if((A=this.IZ)._cycle!=D)A._Mark(
A._cycle=D);},_className:"BezierClock::BezierClock"};E.MU={HD:null,HC:null,Ew:null
,Er:null,G$:null,D6:null,EM:null,Ah:null,DR:function(B$){B.Core.J.DR.call(this,B$
);var DT=this.Ah.QJ();this.Er.MO(DT.O1);this.Er.MM(DT.Lm);this.Er.I7(DT.K5);this.
Ew.MK(DT.Lh);this.Ew.ML(DT.Ll);this.Ew.MN(DT.LB);},VO:function(Af){if(!this.Hu(false
))return;this.Ah.MO(this.Er.MJ());this.Ah.MM(this.Er.MI());this.Ah.I7(this.Er.MH(
));this.Ah.MK(this.Ew.Ts());this.Ah.ML(this.Ew.Tt());this.Ah.MN(this.Ew.Tw());B.
_GetAutoObject(E.Device).Ua(this.Ah.QK());this.G.E1(B._NewObject(E.GS,0),B._GetAutoObject(
B.uk.Gr),null,null,null,null,null,null,null,null,false);},JJ:function(Af){if(this.
Hu(false))this.G.E1(B._NewObject(E.GS,0),B._GetAutoObject(B.uk.Gr),null,null,null
,null,null,null,null,null,false);},_Init:function(aArg){B.Core.J._Init.call(this
,aArg);B.um.MR._Init.call(this.HD={M:this},0);B.um.MR._Init.call(this.HC={M:this
},0);E.Ew._Init.call(this.Ew={M:this},0);E.Er._Init.call(this.Er={M:this},0);B.Core.
G4._Init.call(this.G$={M:this},0);B.uj.FQ._Init.call(this.D6={M:this},0);B.um.EM.
_Init.call(this.EM={M:this},0);B.Core.Ah._Init.call(this.Ah={M:this},0);this.__proto__=
E.MU;this.P(HN);this.HD.P(UG);this.HD.QS(UH);this.HC.P(UI);this.HC.QS(UJ);this.Ew.
P(UK);this.Er.P(UL);this.G$.G1(Pc);this.G$.G0(UM);this.G$.Ct(UN);this.G$.Cs(UO);
this.D6.P(UP);this.D6.Fm(1);this.EM.TR(0xFFC1C1C1);this.EM.TU(0xFFD1D1D1);this.EM.
TT(0x73BEBEBE);this.EM.TS(0xFFFFFFFF);this.Z(this.HD,0);this.Z(this.HC,0);this.Z(
this.Ew,0);this.Z(this.Er,0);this.Z(this.G$,0);this.Z(this.D6,0);this.HD.Im=[this
,this.VO];this.HD.Ke(this.EM);this.HC.Im=[this,this.JJ];this.HC.Ke(this.EM);this.
G$.CH=[this,this.JJ];this.D6.Dq(B.s$(B.ui.Ik));this.EM.TW(B.s$(E.Od));},_Done:function(
){this.__proto__=B.Core.J;this.HD._Done();this.HC._Done();this.Ew._Done();this.Er.
_Done();this.G$._Done();this.D6._Done();this.EM._Done();this.Ah._Done();B.Core.J.
_Done.call(this);},_ReInit:function(){B.Core.J._ReInit.call(this);this.HD._ReInit(
);this.HC._ReInit();this.Ew._ReInit();this.Er._ReInit();this.G$._ReInit();this.D6.
_ReInit();this.EM._ReInit();this.Ah._ReInit();},_Mark:function(D){var A;B.Core.J.
_Mark.call(this,D);if((A=this.HD)._cycle!=D)A._Mark(A._cycle=D);if((A=this.HC)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.Ew)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
Er)._cycle!=D)A._Mark(A._cycle=D);if((A=this.G$)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.D6)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EM)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Ah)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::SettingsScreen"
};E.Er={In:null,E7:null,E5:null,E2:null,BX:null,B8:null,BF:null,Cr:null,CZ:null,
C0:null,C1:null,C2:null,C3:null,I0:0xFFFFFFFF,I1:0x44FFFFFF,Fh:0xFFC1C1C1,Fg:0xFFE1E1E1
,LR:false,Mf:false,P1:false,Ha:function(aSize){this.BF.FX((aSize[1]/3)|0);this.B8.
FX((aSize[1]/3)|0);this.BX.FX((aSize[1]/3)|0);this.BF.BY((aSize[1]/3)|0);this.B8.
BY((aSize[1]/3)|0);this.BX.BY((aSize[1]/3)|0);this.E2.FY([0,(aSize[1]/3)|0]);this.
E5.FY([0,(aSize[1]/3)|0]);this.E7.FY([0,(aSize[1]/3)|0]);},DR:function(B$){var A;
B.ul.Ev.DR.call(this,B$);var Dk=((B$&0x10)===0x10);var KM=((B$&0x20)===0x20);var
LW=(this.E2.DQ||this.E5.DQ)||this.E7.DQ;if(LW){this.Cr.Aa(this.Fh);this.CZ.Aa(this.
Fh);this.C0.Aa(this.Fh);this.C1.Aa(this.Fh);this.C2.Aa(this.Fh);this.C3.Aa(this.
Fh);}else{this.Cr.Aa(this.Fg);this.CZ.Aa(this.Fg);this.C0.Aa(this.Fg);this.C1.Aa(
this.Fg);this.C2.Aa(this.Fg);this.C3.Aa(this.Fg);}this.LR=Dk;this.Mf=KM;this.P1=
LW;},JL:function(Af){this.Bn();if((!this.E2.DQ&&!this.E5.DQ)&&!this.E7.DQ){var E$=
this.Nr(this.MI(),this.MJ());var EB=this.MH();this.BF.Hw(E$);if(EB>E$)this.I7(E$
);B.lq(this.In,this);}},JO:function(Af){this.Bn();},TC:function(Af){var A;var GG=
this.BX.EY;var Ar=(B.uj.Text.isPrototypeOf(A=this.BX.AE)?A:null);if(!Ar)return;Ar.
Cu(B.tD(GG+2000,4,10));Ar.Fl(B.s$(E.Gl));Ar.Aa(0xFFFFFFFF);Ar.EZ(0x12);Ar.BY(AJ);
Ar.P(B.tY(Ar.H,[(A=this.BX.H)[2]-A[0],this.BX.AY]));},TA:function(Af){var A;var GG=
this.B8.EY;var Ar=(B.uj.Text.isPrototypeOf(A=this.B8.AE)?A:null);if(!Ar)return;switch(
GG){case 1:Ar.Cu(B.ui.Qm);break;case 2:Ar.Cu(B.ui.Qy);break;case 3:Ar.Cu(B.ui.Qc
);break;case 4:Ar.Cu(B.ui.QA);break;case 5:Ar.Cu(B.ui.Qv);break;case 6:Ar.Cu(B.ui.
Qu);break;case 7:Ar.Cu(B.ui.Qd);break;case 8:Ar.Cu(B.ui.Q7);break;case 9:Ar.Cu(B.
ui.QD);break;case 10:Ar.Cu(B.ui.QC);break;case 11:Ar.Cu(B.ui.Qg);break;default:Ar.
Cu(B.ui.Qt);}Ar.Fl(B.s$(E.Gl));Ar.Aa(0xFFFFFFFF);Ar.EZ(0x12);Ar.BY(AJ);Ar.P(B.tY(
Ar.H,[(A=this.B8.H)[2]-A[0],this.B8.AY]));},Tx:function(Af){var A;var GG=this.BF.
EY;var Ar=(B.uj.Text.isPrototypeOf(A=this.BF.AE)?A:null);if(!Ar)return;Ar.Cu(B.tD(
GG+1,2,10));Ar.Fl(B.s$(E.Gl));Ar.Aa(0xFFFFFFFF);Ar.EZ(0x12);Ar.BY(AJ);Ar.P(B.tY(
Ar.H,[(A=this.BF.H)[2]-A[0],this.BF.AY]));},MJ:function(){return 2000+((((-this.
BX.AW/this.BX.AY)|0)+1)%this.BX.A5);},MO:function(C){if(C<2000)C=2000;if(C>2036)
C=2036;this.BX.BY(((C-2000)-1)*-this.BX.AY);var E$=this.Nr(this.MI(),C);this.BF.
Hw(E$);if(this.MH()>E$)this.I7(E$);},MI:function(){return 1+((((-this.B8.AW/this.
B8.AY)|0)+1)%12);},MM:function(C){if(C<1)C=1;if(C>12)C=12;this.B8.BY((C-2)*-this.
B8.AY);var E$=this.Nr(C,this.MJ());this.BF.Hw(E$);if(this.MH()>E$)this.I7(E$);},
MH:function(){return 1+((((-this.BF.AW/this.BF.AY)|0)+1)%this.BF.A5);},I7:function(
C){var E$=this.Nr(this.MI(),this.MJ());if(C<1)C=1;if(C>E$)C=E$;this.BF.BY((C-2)*-
this.BF.AY);},Nr:function(Gb,Pk){if(Gb===2)if(!(Pk%4)&&(!!(Pk%100)||!(Pk%400)))return 29;
else return 28;else if((((Gb===4)||(Gb===6))||(Gb===9))||(Gb===11))return 30;else
return 31;},VP:function(Af){var A;var K=this.BF.Aw;while(!!K){var Y=(B.uj.Text.isPrototypeOf(
K)?K:null);if(!!Y){Y.Aa(this.I1);}K=K.Ad;}var R3=(B.uj.Text.isPrototypeOf(A=this.
BF.J8(this.BF.J7(B.s_(this.BF.H))))?A:null);if(!!R3){R3.Aa(this.I0);}},VT:function(
Af){var A;var K=this.B8.Aw;while(!!K){var Y=(B.uj.Text.isPrototypeOf(K)?K:null);
if(!!Y){Y.Aa(this.I1);}K=K.Ad;}var R6=(B.uj.Text.isPrototypeOf(A=this.B8.J8(this.
B8.J7(B.s_(this.B8.H))))?A:null);if(!!R6){R6.Aa(this.I0);}},V1:function(Af){var A;
var K=this.BX.Aw;while(!!K){var Y=(B.uj.Text.isPrototypeOf(K)?K:null);if(!!Y){Y.
Aa(this.I1);}K=K.Ad;}var R8=(B.uj.Text.isPrototypeOf(A=this.BX.J8(this.BX.J7(B.s_(
this.BX.H))))?A:null);if(!!R8){R8.Aa(this.I0);}},_Init:function(aArg){B.ul.Ev._Init.
call(this,aArg);B.Core.Iv._Init.call(this.E7={M:this},0);B.Core.Iv._Init.call(this.
E5={M:this},0);B.Core.Iv._Init.call(this.E2={M:this},0);B.Core.HK._Init.call(this.
BX={M:this},0);B.Core.HK._Init.call(this.B8={M:this},0);B.Core.HK._Init.call(this.
BF={M:this},0);B.uj.Cr._Init.call(this.Cr={M:this},0);B.uj.Cr._Init.call(this.CZ={
M:this},0);B.uj.Cr._Init.call(this.C0={M:this},0);B.uj.Cr._Init.call(this.C1={M:
this},0);B.uj.Cr._Init.call(this.C2={M:this},0);B.uj.Cr._Init.call(this.C3={M:this
},0);this.__proto__=E.Er;this.P(UQ);this.E7.P(Rc);this.E7.FY(CK);this.E7.Kg(0.200000
);this.E5.P(Rd);this.E5.FY(CK);this.E5.Kg(0.200000);this.E2.P(Re);this.E2.FY(CK);
this.E2.Kg(0.200000);this.BX.P(Rc);this.BX.Kf(true);this.BX.BY(80);this.BX.FX(80
);this.BX.Hw(37);this.B8.P(Rd);this.B8.Kf(true);this.B8.BY(80);this.B8.FX(80);this.
B8.Hw(12);this.BF.P(Re);this.BF.Kf(true);this.BF.BY(80);this.BF.FX(80);this.BF.Hw(
31);this.Cr.Ct(UR);this.Cr.Cs(US);this.Cr.CI(3);this.CZ.Ct(UT);this.CZ.Cs(M4);this.
CZ.CI(3);this.C0.Ct(UU);this.C0.Cs(UV);this.C0.CI(3);this.C1.Ct(UW);this.C1.Cs(UX
);this.C1.CI(3);this.C2.Ct(UY);this.C2.Cs(UZ);this.C2.CI(3);this.C3.Ct(U0);this.
C3.Cs(U1);this.C3.CI(3);this.Z(this.E7,0);this.Z(this.E5,0);this.Z(this.E2,0);this.
Z(this.BX,0);this.Z(this.B8,0);this.Z(this.BF,0);this.Z(this.Cr,0);this.Z(this.CZ
,0);this.Z(this.C0,0);this.Z(this.C1,0);this.Z(this.C2,0);this.Z(this.C3,0);this.
E7.Et=[this,this.JL];this.E7.Dr=[this,this.JO];this.E5.Et=[this,this.JL];this.E5.
Dr=[this,this.JO];this.E2.Et=[this,this.JL];this.E2.Dr=[this,this.JO];this.BX.DO=[
this,this.V1];this.BX.FT=[this,this.TC];this.BX.Kj(this.E7);this.B8.DO=[this,this.
VT];this.B8.FT=[this,this.TA];this.B8.Kj(this.E5);this.BF.DO=[this,this.VP];this.
BF.FT=[this,this.Tx];this.BF.Kj(this.E2);},_Done:function(){this.__proto__=B.ul.
Ev;this.E7._Done();this.E5._Done();this.E2._Done();this.BX._Done();this.B8._Done(
);this.BF._Done();this.Cr._Done();this.CZ._Done();this.C0._Done();this.C1._Done(
);this.C2._Done();this.C3._Done();B.ul.Ev._Done.call(this);},_ReInit:function(){
B.ul.Ev._ReInit.call(this);this.E7._ReInit();this.E5._ReInit();this.E2._ReInit();
this.BX._ReInit();this.B8._ReInit();this.BF._ReInit();this.Cr._ReInit();this.CZ.
_ReInit();this.C0._ReInit();this.C1._ReInit();this.C2._ReInit();this.C3._ReInit(
);},_Mark:function(D){var A;B.ul.Ev._Mark.call(this,D);if((A=this.In)&&((A=A[0]).
_cycle!=D))A._Mark(A._cycle=D);if((A=this.E7)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.E5)._cycle!=D)A._Mark(A._cycle=D);if((A=this.E2)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.BX)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B8)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.BF)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Cr)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.CZ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C0
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C1)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.C2)._cycle!=D)A._Mark(A._cycle=D);if((A=this.C3)._cycle!=D)A._Mark(A._cycle=
D);},_className:"BezierClock::DatePicker"};E.Cp={DK:null,AK:null,CJ:null,Df:0,Bw:
B.tA(13,B.qx,null),GU:100,Qf:0xFFFFFFFF,Q9:14.000000,Ds:B.tA(13,B.qx,null),Dt:B.
tA(13,B.qx,null),Du:B.tA(13,B.qx,null),Dv:B.tA(13,B.qx,null),Dw:B.tA(13,B.qx,null
),Dx:B.tA(13,B.qx,null),Dy:B.tA(13,B.qx,null),Dz:B.tA(13,B.qx,null),DA:B.tA(13,B.
qx,null),DB:B.tA(13,B.qx,null),RK:0,Qb:700,Oe:true,No:function(AA,aClip,aOffset,
Aj,aBlend){B.Core.J.No.call(this,AA,aClip,aOffset,Aj,aBlend);if(this.Oe){var ID=
0xFFFF0000;var I=0;while(I<13){this.VA(AA,aClip,aOffset,Aj,aBlend,this.Bw.Get(I)
);if(I>0)AA.Mo(aClip,B.tx(aOffset,this.Bw.Get(I-1)),B.tx(aOffset,this.Bw.Get(I))
,ID,ID,aBlend);I=I+1;}}},Ha:function(aSize){this.CJ.P(B.tY(this.CJ.H,aSize));B.lq([
this,this.Wg],this);B.lq([this,this.Eq],this);},CY:function(aArg){B.lq([this,this.
Eq],this);},DN:function(C){var A;if(this.Df===C)return;this.RK=this.Df%10;this.Df=
C;this.DK.OY(this);},FV:function(C){if(this.GU===C)return;this.GU=C;},Aa:function(
C){if(this.Qf===C)return;this.Qf=C;this.CJ.Aa(C);},FZ:function(C){if(this.Q9===C
)return;this.Q9=C;this.CJ.CI(C);},VA:function(AA,aClip,aOffset,Aj,aBlend,Ey){var
A;var JQ=0xAA0000FF;var PD=0x886060FF;var EF;var EG;aBlend=aBlend&&((this.F&0x2)===
0x2);Aj=Aj+1;if(Aj<256){JQ=(JQ&0x00FFFFFF)|((((Aj*((JQ>>24)&0xFF))>>8)&0xFF)<<24
);PD=(PD&0x00FFFFFF)|((((Aj*((PD>>24)&0xFF))>>8)&0xFF)<<24);}EF=B.tw(B.tx(Ey,aOffset
),Rf);EG=B.tx(B.tx(Ey,aOffset),Rf);AA.Mo(aClip,EF,EG,JQ,JQ,aBlend);EF=B.tw(B.tx(
Ey,aOffset),Rg);EG=B.tx(B.tx(Ey,aOffset),Rg);AA.Mo(aClip,EF,EG,JQ,JQ,aBlend);},FW:
function(C){if(this.Oe===C)return;this.Oe=C;this.Ai(this.H);},Wg:function(Af){var
A;var size=[(A=this.H)[2]-A[0],A[3]-A[1]];var JD=(size[0]/2)|0;var KK=(size[1]/2
)|0;var Jp=(size[0]/10)|0;var Jq=(size[1]/12)|0;this.Ds.Set(0,[Jp,KK]);this.Ds.Set(
1,[Jp,(size[1]/4)|0]);this.Ds.Set(2,[(size[0]/4)|0,Jq]);this.Ds.Set(3,[JD,Jq]);this.
Ds.Set(4,[size[0]-((size[0]/4)|0),Jq]);this.Ds.Set(5,[size[0]-Jp,(size[1]/4)|0]);
this.Ds.Set(6,[size[0]-Jp,KK]);this.Ds.Set(7,[size[0]-Jp,size[1]-((size[1]/4)|0)
]);this.Ds.Set(8,[size[0]-((size[0]/4)|0),size[1]-Jq]);this.Ds.Set(9,[JD,size[1]-
Jq]);this.Ds.Set(10,[(size[0]/4)|0,size[1]-Jq]);this.Ds.Set(11,[Jp,size[1]-((size[
1]/4)|0)]);this.Ds.Set(12,[Jp,KK-1]);this.Dt.Set(0,[(size[0]/6)|0,((size[1]*3)/10
)|0]);this.Dt.Set(1,[((size[0]*16)/60)|0,((size[1]*14)/60)|0]);this.Dt.Set(2,[((
size[0]*22)/60)|0,((size[1]*10)/60)|0]);this.Dt.Set(3,[JD,Jq]);this.Dt.Set(4,[JD
,((size[1]*4)/12)|0]);this.Dt.Set(5,[JD,((size[1]*8)/12)|0]);this.Dt.Set(6,[JD,((
size[1]*11)/12)|0]);this.Dt.Set(7,[JD,((size[1]*11)/12)|0]);this.Dt.Set(8,[((size[
0]*4)/6)|0,((size[1]*11)/12)|0]);this.Dt.Set(9,[((size[0]*5)/6)|0,((size[1]*11)/
12)|0]);this.Dt.Set(10,[((size[0]*4)/6)|0,((size[1]*11)/12)|0]);this.Dt.Set(11,[((
size[0]*2)/6)|0,((size[1]*11)/12)|0]);this.Dt.Set(12,[(size[0]/6)|0,((size[1]*11
)/12)|0]);this.Du.Set(0,[(size[0]/6)|0,((size[1]*3)/10)|0]);this.Du.Set(1,[(size[
0]/6)|0,0]);this.Du.Set(2,[((size[0]*5)/6)|0,0]);this.Du.Set(3,[((size[0]*5)/6)|
0,((size[1]*3)/10)|0]);this.Du.Set(4,[((size[0]*5)/6)|0,((size[1]*7)/16)|0]);this.
Du.Set(5,[((size[0]*39)/60)|0,((size[1]*5)/9)|0]);this.Du.Set(6,[((size[0]*33)/60
)|0,((size[1]*5)/8)|0]);this.Du.Set(7,[((size[0]*22)/60)|0,((size[1]*45)/60)|0]);
this.Du.Set(8,[((size[0]*19)/60)|0,((size[1]*47)/60)|0]);this.Du.Set(9,[(size[0]
/8)|0,((size[1]*11)/12)|0]);this.Du.Set(10,[((size[0]*3)/8)|0,((size[1]*11)/12)|
0]);this.Du.Set(11,[((size[0]*5)/8)|0,((size[1]*11)/12)|0]);this.Du.Set(12,[((size[
0]*7)/8)|0,((size[1]*11)/12)|0]);this.Dv.Set(0,[(size[0]/6)|0,((size[1]*3)/10)|0
]);this.Dv.Set(1,[(size[0]/5)|0,0]);this.Dv.Set(2,[size[0]-((size[0]/7)|0),0]);this.
Dv.Set(3,[size[0]-((size[0]/6)|0),((size[1]*3)/10)|0]);this.Dv.Set(4,[size[0]-((
size[0]/5)|0),((size[1]*3)/7)|0]);this.Dv.Set(5,[((size[0]*4)/6)|0,KK]);this.Dv.
Set(6,[((size[0]*9)/20)|0,KK]);this.Dv.Set(7,[((size[0]*7)/10)|0,KK]);this.Dv.Set(
8,[size[0]-(((size[0]*4)/30)|0),size[1]-(((size[1]*3)/7)|0)]);this.Dv.Set(9,[size[
0]-(((size[0]*3)/20)|0),size[1]-(((size[1]*3)/10)|0)]);this.Dv.Set(10,[size[0]-(((
size[0]*3)/20)|0),size[1]]);this.Dv.Set(11,[(size[0]/6)|0,size[1]]);this.Dv.Set(
12,[((size[0]*3)/20)|0,size[1]-(((size[1]*3)/10)|0)]);this.Dw.Set(0,[((size[0]*4
)/6)|0,((size[1]*19)/20)|0]);this.Dw.Set(1,[((size[0]*4)/6)|0,((size[1]*17)/20)|
0]);this.Dw.Set(2,[((size[0]*4)/6)|0,((size[1]*16)/20)|0]);this.Dw.Set(3,[((size[
0]*4)/6)|0,((size[1]*10)/20)|0]);this.Dw.Set(4,[((size[0]*4)/6)|0,((size[1]*7)/20
)|0]);this.Dw.Set(5,[((size[0]*4)/6)|0,((size[1]*1)/20)|0]);this.Dw.Set(6,[((size[
0]*4)/6)|0,((size[1]*1)/20)|0]);this.Dw.Set(7,[((size[0]*23)/60)|0,((size[1]*20)
/60)|0]);this.Dw.Set(8,[((size[0]*13)/60)|0,((size[1]*30)/60)|0]);this.Dw.Set(9,[(
size[0]/16)|0,((size[1]*6)/9)|0]);this.Dw.Set(10,[((size[0]*2)/6)|0,((size[1]*6)
/9)|0]);this.Dw.Set(11,[((size[0]*4)/6)|0,((size[1]*6)/9)|0]);this.Dw.Set(12,[((
size[0]*15)/16)|0,((size[1]*6)/9)|0]);this.Dx.Set(0,[((size[0]*3)/20)|0,((size[1
]*13)/15)|0]);this.Dx.Set(1,[((size[0]*5)/10)|0,((size[1]*59)/60)|0]);this.Dx.Set(
2,[((size[0]*5)/6)|0,((size[1]*14)/16)|0]);this.Dx.Set(3,[((size[0]*51)/60)|0,((
size[1]*10)/15)|0]);this.Dx.Set(4,[((size[0]*17)/20)|0,((size[1]*14)/30)|0]);this.
Dx.Set(5,[((size[0]*6)/10)|0,((size[1]*6)/16)|0]);this.Dx.Set(6,[((size[0]*3)/13
)|0,((size[1]*7)/16)|0]);this.Dx.Set(7,[((size[0]*15)/60)|0,((size[1]*5)/16)|0]);
this.Dx.Set(8,[((size[0]*16)/60)|0,((size[1]*7)/30)|0]);this.Dx.Set(9,[((size[0]
*3)/10)|0,(size[1]/12)|0]);this.Dx.Set(10,[((size[0]*5)/10)|0,(size[1]/12)|0]);this.
Dx.Set(11,[((size[0]*7)/10)|0,(size[1]/12)|0]);this.Dx.Set(12,[((size[0]*5)/6)|0
,(size[1]/12)|0]);this.Dy.Set(0,[((size[0]*10)/60)|0,((size[1]*18)/30)|0]);this.
Dy.Set(1,[((size[0]*5)/60)|0,((size[1]*24)/30)|0]);this.Dy.Set(2,[((size[0]*9)/30
)|0,((size[1]*29)/30)|0]);this.Dy.Set(3,[((size[0]*18)/30)|0,((size[1]*55)/60)|0
]);this.Dy.Set(4,[((size[0]*54)/60)|0,((size[1]*51)/60)|0]);this.Dy.Set(5,[((size[
0]*55)/60)|0,((size[1]*35)/60)|0]);this.Dy.Set(6,[((size[0]*43)/60)|0,((size[1]*
29)/60)|0]);this.Dy.Set(7,[((size[0]*6)/11)|0,((size[1]*8)/20)|0]);this.Dy.Set(8
,[((size[0]*3)/11)|0,((size[1]*9)/20)|0]);this.Dy.Set(9,[((size[0]*5)/30)|0,((size[
1]*12)/20)|0]);this.Dy.Set(10,[((size[0]*5)/30)|0,((size[1]*10)/20)|0]);this.Dy.
Set(11,[((size[0]*26)/60)|0,((size[1]*16)/60)|0]);this.Dy.Set(12,[((size[0]*20)/
30)|0,(size[1]/15)|0]);this.Dz.Set(0,[((size[0]*17)/60)|0,((size[1]*28)/30)|0]);
this.Dz.Set(1,[((size[0]*23)/60)|0,((size[1]*24)/30)|0]);this.Dz.Set(2,[((size[0
]*29)/60)|0,((size[1]*20)/30)|0]);this.Dz.Set(3,[((size[0]*18)/30)|0,(size[1]/2)|
0]);this.Dz.Set(4,[((size[0]*40)/60)|0,((size[1]*12)/30)|0]);this.Dz.Set(5,[((size[
0]*47)/60)|0,((size[1]*7)/30)|0]);this.Dz.Set(6,[((size[0]*53)/60)|0,(size[1]/12
)|0]);this.Dz.Set(7,[((size[0]*14)/20)|0,(size[1]/12)|0]);this.Dz.Set(8,[((size[
0]*13)/20)|0,(size[1]/12)|0]);;this.Dz.Set(9,[((size[0]*6)/20)|0,(size[1]/12)|0]
);;this.Dz.Set(10,[((size[0]*5)/20)|0,(size[1]/12)|0]);this.Dz.Set(11,[((size[0]
*5)/20)|0,(size[1]/12)|0]);this.Dz.Set(12,[((size[0]*2)/20)|0,(size[1]/12)|0]);this.
DA.Set(0,[((size[0]*59)/120)|0,((size[1]*55)/60)|0]);this.DA.Set(1,[((size[0]*19
)/20)|0,((size[1]*55)/60)|0]);this.DA.Set(2,[((size[0]*19)/20)|0,((size[1]*1)/2)|
0]);this.DA.Set(3,[((size[0]*59)/120)|0,((size[1]*1)/2)|0]);this.DA.Set(4,[((size[
0]*28)/30)|0,((size[1]*1)/2)|0]);this.DA.Set(5,[((size[0]*28)/30)|0,((size[1]*5)
/60)|0]);this.DA.Set(6,[((size[0]*4)/8)|0,((size[1]*5)/60)|0]);this.DA.Set(7,[((
size[0]*2)/30)|0,((size[1]*5)/60)|0]);this.DA.Set(8,[((size[0]*2)/30)|0,((size[1
]*1)/2)|0]);this.DA.Set(9,[((size[0]*61)/120)|0,((size[1]*1)/2)|0]);this.DA.Set(
10,[((size[0]*1)/20)|0,((size[1]*30)/60)|0]);this.DA.Set(11,[((size[0]*1)/20)|0,((
size[1]*55)/60)|0]);this.DA.Set(12,[((size[0]*61)/120)|0,((size[1]*55)/60)|0]);this.
DB.Set(0,[((size[0]*22)/60)|0,((size[1]*56)/60)|0]);this.DB.Set(1,[((size[0]*38)
/60)|0,((size[1]*40)/60)|0]);this.DB.Set(2,[((size[0]*48)/60)|0,((size[1]*30)/60
)|0]);this.DB.Set(3,[((size[0]*49)/60)|0,((size[1]*25)/60)|0]);this.DB.Set(4,[((
size[0]*55)/60)|0,((size[1]*15)/60)|0]);this.DB.Set(5,[((size[0]*45)/60)|0,((size[
1]*5)/60)|0]);this.DB.Set(6,[((size[0]*30)/60)|0,((size[1]*5)/60)|0]);this.DB.Set(
7,[((size[0]*18)/60)|0,((size[1]*5)/60)|0]);this.DB.Set(8,[((size[0]*11)/60)|0,((
size[1]*12)/60)|0]);this.DB.Set(9,[((size[0]*10)/60)|0,((size[1]*18)/60)|0]);this.
DB.Set(10,[((size[0]*7)/60)|0,((size[1]*36)/60)|0]);this.DB.Set(11,[((size[0]*42
)/60)|0,((size[1]*38)/60)|0]);this.DB.Set(12,[((size[0]*49)/60)|0,((size[1]*25)/
60)|0]);},DM:function(C){if(this.Qb===C)return;this.Qb=C;this.DK.FU(C);},Eq:function(
Af){var A;var Vz=this.Df%10;var A$=this.DK.Df;var CT=1.000000-this.DK.Df;var x1=
0.000000;var x2=0.000000;var y1=0.000000;var y2=0.000000;var I=0;while(I<13){switch(
this.RK){case 0:{x1=this.Ds.Get(I)[0]*CT;y1=this.Ds.Get(I)[1]*CT;}break;case 1:{
x1=this.Dt.Get(I)[0]*CT;y1=this.Dt.Get(I)[1]*CT;}break;case 2:{x1=this.Du.Get(I)[
0]*CT;y1=this.Du.Get(I)[1]*CT;}break;case 3:{x1=this.Dv.Get(I)[0]*CT;y1=this.Dv.
Get(I)[1]*CT;}break;case 4:{x1=this.Dw.Get(I)[0]*CT;y1=this.Dw.Get(I)[1]*CT;}break;
case 5:{x1=this.Dx.Get(I)[0]*CT;y1=this.Dx.Get(I)[1]*CT;}break;case 6:{x1=this.Dy.
Get(I)[0]*CT;y1=this.Dy.Get(I)[1]*CT;}break;case 7:{x1=this.Dz.Get(I)[0]*CT;y1=this.
Dz.Get(I)[1]*CT;}break;case 8:{x1=this.DA.Get(I)[0]*CT;y1=this.DA.Get(I)[1]*CT;}
break;case 9:{x1=this.DB.Get(I)[0]*CT;y1=this.DB.Get(I)[1]*CT;}break;default:{x1=((((
A=this.H)[2]-A[0])/2)|0)*CT;y1=((((A=this.H)[3]-A[1])/2)|0)*CT;}}switch(Vz){case
0:{x2=this.Ds.Get(I)[0]*A$;y2=this.Ds.Get(I)[1]*A$;}break;case 1:{x2=this.Dt.Get(
I)[0]*A$;y2=this.Dt.Get(I)[1]*A$;}break;case 2:{x2=this.Du.Get(I)[0]*A$;y2=this.
Du.Get(I)[1]*A$;}break;case 3:{x2=this.Dv.Get(I)[0]*A$;y2=this.Dv.Get(I)[1]*A$;}
break;case 4:{x2=this.Dw.Get(I)[0]*A$;y2=this.Dw.Get(I)[1]*A$;}break;case 5:{x2=
this.Dx.Get(I)[0]*A$;y2=this.Dx.Get(I)[1]*A$;}break;case 6:{x2=this.Dy.Get(I)[0]
*A$;y2=this.Dy.Get(I)[1]*A$;}break;case 7:{x2=this.Dz.Get(I)[0]*A$;y2=this.Dz.Get(
I)[1]*A$;}break;case 8:{x2=this.DA.Get(I)[0]*A$;y2=this.DA.Get(I)[1]*A$;}break;case
9:{x2=this.DB.Get(I)[0]*A$;y2=this.DB.Get(I)[1]*A$;}break;default:;}this.Bw.Set(
I,[(x1+x2)|0,(y1+y2)|0]);I=I+1;}this.AK.BD(0,this.GU);this.AK.Ff(0,this.Bw.Get(0
)[0],this.Bw.Get(0)[1]);this.AK.Mh(0,this.Bw.Get(1)[0],this.Bw.Get(1)[1],this.Bw.
Get(2)[0],this.Bw.Get(2)[1],this.Bw.Get(3)[0],this.Bw.Get(3)[1],(this.GU/4)|0);this.
AK.Mh(0,this.Bw.Get(4)[0],this.Bw.Get(4)[1],this.Bw.Get(5)[0],this.Bw.Get(5)[1],
this.Bw.Get(6)[0],this.Bw.Get(6)[1],(this.GU/4)|0);this.AK.Mh(0,this.Bw.Get(7)[0
],this.Bw.Get(7)[1],this.Bw.Get(8)[0],this.Bw.Get(8)[1],this.Bw.Get(9)[0],this.Bw.
Get(9)[1],(this.GU/4)|0);this.AK.Mh(0,this.Bw.Get(10)[0],this.Bw.Get(10)[1],this.
Bw.Get(11)[0],this.Bw.Get(11)[1],this.Bw.Get(12)[0],this.Bw.Get(12)[1],(this.GU/
4)|0);},_Init:function(aArg){B.Core.J._Init.call(this,aArg);B.uk.DK._Init.call(this.
DK={M:this},0);B.Graphics.AK._Init.call(this.AK={M:this},0);B.uj.CJ._Init.call(this.
CJ={M:this},0);(this.Bw=[]).__proto__=E.Cp.Bw;(this.Ds=[]).__proto__=E.Cp.Ds;(this.
Dt=[]).__proto__=E.Cp.Dt;(this.Du=[]).__proto__=E.Cp.Du;(this.Dv=[]).__proto__=E.
Cp.Dv;(this.Dw=[]).__proto__=E.Cp.Dw;(this.Dx=[]).__proto__=E.Cp.Dx;(this.Dy=[]).
__proto__=E.Cp.Dy;(this.Dz=[]).__proto__=E.Cp.Dz;(this.DA=[]).__proto__=E.Cp.DA;(
this.DB=[]).__proto__=E.Cp.DB;this.__proto__=E.Cp;this.P(U2);this.DK.Lt(1);this.
DK.FU(700);this.DK.BG(true);this.CJ.CI(14.000000);this.Z(this.CJ,0);this.DK.I5=[
this,this.Eq];this.CJ.EL(this.AK);this.CY(aArg);},_Done:function(){this.__proto__=
B.Core.J;this.DK._Done();this.AK._Done();this.CJ._Done();B.Core.J._Done.call(this
);},_ReInit:function(){B.Core.J._ReInit.call(this);this.DK._ReInit();this.AK._ReInit(
);this.CJ._ReInit();},_Mark:function(D){var A;B.Core.J._Mark.call(this,D);if((A=
this.DK)._cycle!=D)A._Mark(A._cycle=D);if((A=this.AK)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.CJ)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::Digit"
};E.C$={CG:null,Cf:null,EW:null,EX:null,Ha:function(aSize){B.Core.J.Ha.call(this
,aSize);this.CG.Kh(((aSize[0]*10)/23)|0);this.CG.Ki(((aSize[1]*10)/23)|0);this.Cf.
P(B.tY(this.Cf.H,aSize));this.Cf.Eu([(aSize[0]/2)|0,this.Cf.As[1]]);this.Cf.Eu([
this.Cf.As[0],(aSize[1]/2)|0]);this.EW.P(B.tY(this.EW.H,aSize));this.EW.Eu([((aSize[
0]*10)/19)|0,this.EW.As[1]]);this.EW.Eu([this.EW.As[0],((aSize[1]*10)/19)|0]);this.
EX.P(B.tY(this.EX.H,aSize));this.EX.Eu([((aSize[0]*10)/18)|0,this.EX.As[1]]);this.
EX.Eu([this.EX.As[0],((aSize[1]*10)/21)|0]);},_Init:function(aArg){B.Core.J._Init.
call(this,aArg);B.Graphics.CG._Init.call(this.CG={M:this},0);B.uj.Cf._Init.call(
this.Cf={M:this},0);B.uj.Cf._Init.call(this.EW={M:this},0);B.uj.Cf._Init.call(this.
EX={M:this},0);this.__proto__=E.C$;this.P(U3);this.Lr(true);this.CG.CB(360.000000
);this.CG.OM(90.000000);this.CG.Ir(2);this.Cf.P(Pd);this.Cf.Aa(0x22FFFFFF);this.
EW.P(Pd);this.EW.Aa(0x22FFFFFF);this.EX.P(Pd);this.EX.Aa(0x22FFFFFF);this.Z(this.
Cf,0);this.Z(this.EW,0);this.Z(this.EX,0);this.Cf.EL(this.CG);this.EW.EL(this.CG
);this.EX.EL(this.CG);},_Done:function(){this.__proto__=B.Core.J;this.CG._Done();
this.Cf._Done();this.EW._Done();this.EX._Done();B.Core.J._Done.call(this);},_ReInit:
function(){B.Core.J._ReInit.call(this);this.CG._ReInit();this.Cf._ReInit();this.
EW._ReInit();this.EX._ReInit();},_Mark:function(D){var A;B.Core.J._Mark.call(this
,D);if((A=this.CG)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Cf)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.EW)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EX)._cycle!=
D)A._Mark(A._cycle=D);},_className:"BezierClock::Bubble"};E.Az={Gh:null,FN:null,
Fn:null,Az:null,C$:null,E0:null,FO:0,Ha:function(aSize){var A;B.Core.J.Ha.call(this
,aSize);var R0=(((A=this.H)[3]-A[1])/4)|0;this.C$.P(B.tY(this.C$.H,[R0,R0]));},DR:
function(B$){B.Core.J.DR.call(this,B$);this.C$.Hx(this.FO>0);this.Az.Dq(null);B.
tg([this,this.V_],this);},ND:function(Af){var A;var RU=(A=this.C$.H)[2]-A[0];var
RV=(A=this.C$.H)[3]-A[1];var IT=B.tx([(((A=this.H)[2]-A[0])/3)|0,(((A=this.H)[3]-
A[1])/2)|0],this.Az.AW);this.C$.P(B.t0(this.C$.H,IT[0]+(((this.Gh.Df*RU)+(this.FN.
Df*RU))|0)));this.C$.P(B.t2(this.C$.H,IT[1]+(((this.Gh.Df*RV)-(this.FN.Df*RV))|0
)));},V_:function(Af){if(this.FO===1)this.Az.Dq(B.s$(E.OP));else if(this.FO===2)
this.Az.Dq(B.s$(E.Blue));else this.Az.Dq(B.s$(E.Mn));},GY:function(C){if(C<0)C=0;
if(C>2)C=2;if(this.FO===C)return;this.FO=C;B.tG([this,this.I6,this.GY],0);this.Bn(
);},ShiftLeft:function(){var A;this.E0.BN=this.Az.AW;this.E0.BZ=[((A=this.H)[2]-
A[0])-this.Az.An.FrameSize[0],0];B.lq([A=this.E0,A.OY],this);},ShiftRight:function(
){var A;this.E0.BN=this.Az.AW;this.E0.BZ=Pe;B.lq([A=this.E0,A.OY],this);},I6:function(
){return this.FO;},_Init:function(aArg){B.Core.J._Init.call(this,aArg);B.uk.DK._Init.
call(this.Gh={M:this},0);B.uk.DK._Init.call(this.FN={M:this},0);B.uj.Fn._Init.call(
this.Fn={M:this},0);B.uj.FQ._Init.call(this.Az={M:this},0);E.C$._Init.call(this.
C$={M:this},0);B.uk.MQ._Init.call(this.E0={M:this},0);this.__proto__=E.Az;var A;
this.P(HN);this.Gh.Kr=true;this.Gh.Kk(24);this.Gh.FU(18000);this.Gh.BG(true);this.
FN.Kr=true;this.FN.Kk(24);this.FN.FU(40000);this.FN.BG(true);this.FN.BN=-1.000000;
this.Fn.Dc(0x3F);this.Fn.P(HN);this.Fn.Aa(0xFF000000);this.Az.Dc(0x3F);this.Az.P(
HN);this.Az.EZ(0x11);this.C$.Dc(0x3);this.C$.P(U4);this.E0.Kk(23);this.E0.Lt(1);
this.E0.FU(500);this.Z(this.Fn,0);this.Z(this.Az,0);this.Z(this.C$,0);this.Gh.I5=[
this,this.ND];this.FN.I5=[this,this.ND];this.Az.Dq(B.s$(E.Mn));this.E0.Av=[A=this.
Az,A.Tv,A.BY];},_Done:function(){this.__proto__=B.Core.J;this.Gh._Done();this.FN.
_Done();this.Fn._Done();this.Az._Done();this.C$._Done();this.E0._Done();B.Core.J.
_Done.call(this);},_ReInit:function(){B.Core.J._ReInit.call(this);this.Gh._ReInit(
);this.FN._ReInit();this.Fn._ReInit();this.Az._ReInit();this.C$._ReInit();this.E0.
_ReInit();},_Mark:function(D){var A;B.Core.J._Mark.call(this,D);if((A=this.Gh)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.FN)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
Fn)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Az)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.C$)._cycle!=D)A._Mark(A._cycle=D);if((A=this.E0)._cycle!=D)A._Mark(A._cycle=
D);},_className:"BezierClock::Background"};E.MZ={BE:null,CW:null,CU:null,CV:null
,B1:null,B2:null,B3:null,B4:null,BU:null,BV:null,De:null,G8:null,Ig:null,G6:0,FM:
false,DR:function(B$){B.Core.J.DR.call(this,B$);var Ca=0xFFFFFFFF;if(!this.FM&&(
this.G6>0))Ca=0x99FFFFFF;this.B1.Aa(Ca);this.B2.Aa(Ca);this.B3.Aa(Ca);this.B4.Aa(
Ca);this.BU.Aa(Ca);this.BV.Aa(Ca);this.CU.Aa(Ca);this.CV.Aa(Ca);},PN:function(Af
){var A;if(!this.Hu(false))return;this.G.E1(B._NewObject(E.GS,0),B._GetAutoObject(
B.uk.Kp),null,null,null,null,null,null,null,null,false);(E.K0.isPrototypeOf(A=this.
G)?A:null).Az.ShiftRight();},Lq:function(C){if(this.FM===C)return;this.FM=C;if(C
){this.BE.BN=this.G6;this.BE.FU(this.G6*10);this.BE.BG(this.G6>0);}else this.BE.
BG(false);this.Bn();},ON:function(C){if(this.G6===C)return;this.G6=C;var RI=(this.
G6/6000)|0;var R2=((this.G6/100)|0)%60;var Sa=this.G6;this.B1.DN((RI/10)|0);this.
B2.DN(RI%10);this.B3.DN((R2/10)|0);this.B4.DN(R2%10);this.BU.DN((Sa/10)|0);this.
BV.DN(Sa%10);B.tG([this,this.OI,this.ON],0);this.Bn();},To:function(){return this.
FM;},OI:function(){return this.G6;},_Init:function(aArg){B.Core.J._Init.call(this
,aArg);B.uk.BE._Init.call(this.BE={M:this},0);B.Graphics.CG._Init.call(this.CW={
M:this},0);B.uj.Cf._Init.call(this.CU={M:this},0);B.uj.Cf._Init.call(this.CV={M:
this},0);E.Cp._Init.call(this.B1={M:this},0);E.Cp._Init.call(this.B2={M:this},0);
E.Cp._Init.call(this.B3={M:this},0);E.Cp._Init.call(this.B4={M:this},0);E.Cp._Init.
call(this.BU={M:this},0);E.Cp._Init.call(this.BV={M:this},0);E.OZ._Init.call(this.
De={M:this},0);B.Core.G4._Init.call(this.G8={M:this},0);B.uj.FQ._Init.call(this.
Ig={M:this},0);this.__proto__=E.MZ;this.P(HN);this.BE.Lt(1);this.BE.FU(100);this.
BE.BZ=0;this.CW.QU(20);this.CW.CB(360.000000);this.CW.OM(6.100000);this.CW.Ir(2);
this.CU.P(U5);this.CU.Aa(0xDDFFFFFF);this.CU.Eu(M5);this.CV.P(U6);this.CV.Aa(0xDDFFFFFF
);this.CV.Eu(M5);this.B1.P(U7);this.B1.DN(0);this.B1.FV(60);this.B1.FZ(6.100000);
this.B1.FW(false);this.B1.DM(700);this.B2.P(U8);this.B2.DN(0);this.B2.FV(60);this.
B2.FZ(6.100000);this.B2.FW(false);this.B2.DM(700);this.B3.P(U9);this.B3.DN(0);this.
B3.FV(60);this.B3.FZ(6.100000);this.B3.FW(false);this.B3.DM(700);this.B4.P(U_);this.
B4.DN(0);this.B4.FV(60);this.B4.FZ(6.100000);this.B4.FW(false);this.B4.DM(700);this.
BU.P(U$);this.BU.DN(0);this.BU.FV(40);this.BU.FZ(2.300000);this.BU.FW(false);this.
BU.DM(10);this.BV.P(Va);this.BV.DN(0);this.BV.FV(40);this.BV.FZ(2.300000);this.BV.
FW(false);this.BV.DM(10);this.De.P(Vb);this.De.TX(600);this.De.TZ(100);this.G8.G1(
Pe);this.G8.G0(Pc);this.G8.Ct(Rh);this.G8.Cs(Vc);this.Ig.P(Vd);this.Ig.Fm(3);this.
Z(this.CU,0);this.Z(this.CV,0);this.Z(this.B1,0);this.Z(this.B2,0);this.Z(this.B3
,0);this.Z(this.B4,0);this.Z(this.BU,0);this.Z(this.BV,0);this.Z(this.De,0);this.
Z(this.G8,0);this.Z(this.Ig,0);this.BE.Av=[this,this.OI,this.ON];this.CU.EL(this.
CW);this.CV.EL(this.CW);this.De.I8([this,this.OI,this.ON]);this.De.TY([this,this.
To,this.Lq]);this.G8.CH=[this,this.PN];this.Ig.Dq(B.s$(B.ui.Ik));},_Done:function(
){this.__proto__=B.Core.J;this.BE._Done();this.CW._Done();this.CU._Done();this.CV.
_Done();this.B1._Done();this.B2._Done();this.B3._Done();this.B4._Done();this.BU.
_Done();this.BV._Done();this.De._Done();this.G8._Done();this.Ig._Done();B.Core.J.
_Done.call(this);},_ReInit:function(){B.Core.J._ReInit.call(this);this.BE._ReInit(
);this.CW._ReInit();this.CU._ReInit();this.CV._ReInit();this.B1._ReInit();this.B2.
_ReInit();this.B3._ReInit();this.B4._ReInit();this.BU._ReInit();this.BV._ReInit(
);this.De._ReInit();this.G8._ReInit();this.Ig._ReInit();},_Mark:function(D){var A;
B.Core.J._Mark.call(this,D);if((A=this.BE)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
CW)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CU)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.CV)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B1)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.B2)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B3)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.B4)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BU)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.BV)._cycle!=D)A._Mark(A._cycle=D);if((A=this.De
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.G8)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Ig)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::TimerScreen"
};E.OZ={QE:null,In:null,Av:null,Hz:null,HL:null,CQ:null,D$:null,E8:null,E9:null,
Co:null,F_:null,Dg:null,Gs:null,Gt:null,Gu:null,A8:null,Ap:null,GO:0,DL:360,D8:0
,NY:300,MP:1,FM:false,LL:false,LR:false,Mf:false,V4:false,Ha:function(aSize){var
KW=(aSize[0]/30)|0;this.Dg.P([(aSize[0]/2)|0,0,aSize[0],(aSize[1]/2)|0]);this.Dg.
Eu([0,(aSize[1]/2)|0]);this.Dg.CI(KW);this.CQ.Kh(((aSize[0]-((KW+3.000000)|0))/2
)|0);this.CQ.Ki(((aSize[1]-((KW+3.000000)|0))/2)|0);this.Gs.P([(aSize[0]/2)|0,(aSize[
1]/2)|0,aSize[0],aSize[1]]);this.Gs.Eu(Pe);this.Gs.CI(KW);this.D$.Kh(this.CQ.A6);
this.D$.Ki(this.CQ.AZ);this.Gt.P([0,(aSize[1]/2)|0,(aSize[0]/2)|0,aSize[1]]);this.
Gt.Eu([(aSize[0]/2)|0,0]);this.Gt.CI(KW);this.E8.Kh(this.CQ.A6);this.E8.Ki(this.
CQ.AZ);this.Gu.P([0,0,(aSize[0]/2)|0,(aSize[1]/2)|0]);this.Gu.Eu([(aSize[0]/2)|0
,(aSize[1]/2)|0]);this.Gu.CI(KW);this.E9.Kh(this.CQ.A6);this.E9.Ki(this.CQ.AZ);this.
F_.P(B.tY(this.F_.H,aSize));this.F_.Eu([(aSize[0]/2)|0,(aSize[1]/2)|0]);this.HL.
Kh(this.CQ.A6-(this.Dg.Width/1.800000));this.HL.Ki(this.CQ.AZ-(this.Dg.Width/1.800000
));this.A8.P([(aSize[0]/5)|0,(aSize[1]/5)|0,((aSize[0]*4)/5)|0,((aSize[1]*4)/5)|
0]);this.Ap.P(B.tY(this.Ap.H,aSize));B.lq([this,this.Sc],this);},DR:function(B$){
var A;B.ul.De.DR.call(this,B$);var Dk=((B$&0x10)===0x10);var KM=((B$&0x20)===0x20
);var RG=this.Ap.Down&&!this.LL;if(!Dk){this.Dg.Aa(0xCCCCCCCC);this.A8.Aa(0x00CCCCCC
);}else if(RG){this.Dg.Aa(0xFFFFFFFF);this.A8.Aa(0x00FFFFFF);}else if(KM){this.Dg.
Aa(0xCCFFFFFF);this.A8.Aa(0xBBFFFFFF);}else{this.Dg.Aa(0xFFFFFFFF);if(this.FM)this.
A8.Aa(0x33FFFFFF);else if(this.Io()>0)this.A8.Aa(0xFFFFFFFF);else this.A8.Aa(0x00FFFFFF
);}this.Gs.Aa(this.Dg.Kb());this.Gt.Aa(this.Dg.Kb());this.Gu.Aa(this.Dg.Kb());this.
F_.Aa(this.Dg.Kb());var LZ=0.000000;var PJ=360.000000;var IJ=LZ;if(this.DL!==this.
D8)IJ=(((this.Io()-this.D8)*(PJ-LZ))/(this.DL-this.D8))+LZ;this.CQ.I9(-90.000000
);this.D$.I9(0.000000);this.E8.I9(90.000000);this.E9.I9(180.000000);if(IJ<=90.000000
){this.CQ.CB(IJ-90.000000);this.D$.CB(0.000000);this.E8.CB(90.000000);this.E9.CB(
180.000000);}else if(IJ<=180.000000){this.CQ.CB(90.099998);this.D$.I9(-0.100000);
this.D$.CB(IJ-90.000000);this.E8.CB(90.000000);this.E9.CB(180.000000);}else if(IJ<=
270.000000){this.CQ.CB(90.099998);this.D$.I9(-0.100000);this.D$.CB(180.000000);this.
E8.CB(IJ-90.000000);this.E9.CB(180.000000);}else{this.CQ.CB(90.099998);this.D$.I9(-
0.100000);this.D$.CB(180.100006);this.E8.CB(270.100006);this.E9.CB(IJ-89.900002);
}this.LR=Dk;this.Mf=KM;this.V4=RG;},JM:function(Af){var A;if(!!this.Av)this.QN(((
A=this.Av,A[1].call(A[0]))/this.MP)|0);},VU:function(Af){var A;if(this.LL||this.
FM)return;var LZ=0;var PJ=360;var NC=this.Io();var Hl=NC;Hl=(((-this.Ap.MS*(this.
DL-this.D8))/(PJ-LZ))|0)+this.GO;if((Hl>this.DL)&&(this.DL>this.D8))this.GO=this.
GO-(Hl-this.DL);if((Hl>this.D8)&&(this.D8>this.DL))this.GO=this.GO-(Hl-this.D8);
if((Hl<this.D8)&&(this.DL>this.D8))this.GO=this.GO-(Hl-this.D8);if((Hl<this.DL)&&(
this.D8>this.DL))this.GO=this.GO-(Hl-this.DL);this.QN(Hl);if(NC===this.Io())return;
B.lq(this.In,this);if(!!this.Av){(A=this.Av,A[2].call(A[0],this.Io()*this.MP));B.
tG(this.Av,0);}},L7:function(Af){var A;this.Bn();if(this.Io()!==this.GO)B.lq(this.
QE,this);if(this.LL){if(!!this.Hz)(A=this.Hz,A[2].call(A[0],this.FM));}},L6:function(
Af){this.Bn();this.LL=B.qu(this.A8.H,this.Ap.CX);if(this.LL){if(this.FM)this.Lq(
false);else if(this.Io()>0)this.Lq(true);}this.GO=this.Io();},I8:function(C){if(
B.tn(this.Av,C))return;if(!!this.Av)B.sO([this,this.JM],this.Av,0);this.Av=C;if(
!!C)B.sB([this,this.JM],C,0);if(!!C)B.lq([this,this.JM],this);},TX:function(C){if(
this.DL===C)return;this.DL=C;this.Bn();},Io:function(){var C=this.NY;if(this.D8>
this.DL){if(C<this.DL)C=this.DL;if(C>this.D8)C=this.D8;}else{if(C<this.D8)C=this.
D8;if(C>this.DL)C=this.DL;}return C;},QN:function(C){if(this.NY===C)return;this.
NY=C;this.Bn();},TZ:function(C){if(this.MP===C)return;if(!C)C=1;this.MP=C;this.Bn(
);},Sc:function(Af){var A;if(this.FM){this.Co.MT(2);this.Co.BD(0,4);this.Co.Ff(0
,((((A=this.A8.H)[2]-A[0])*2)/10)|0,((((A=this.A8.H)[3]-A[1])*1)/10)|0);this.Co.
Id(0,((((A=this.A8.H)[2]-A[0])*4)/10)|0,((((A=this.A8.H)[3]-A[1])*1)/10)|0);this.
Co.Id(0,((((A=this.A8.H)[2]-A[0])*4)/10)|0,((((A=this.A8.H)[3]-A[1])*9)/10)|0);this.
Co.Id(0,((((A=this.A8.H)[2]-A[0])*2)/10)|0,((((A=this.A8.H)[3]-A[1])*9)/10)|0);this.
Co.EJ(0);this.Co.BD(1,4);this.Co.Ff(1,((((A=this.A8.H)[2]-A[0])*6)/10)|0,((((A=this.
A8.H)[3]-A[1])*1)/10)|0);this.Co.Id(1,((((A=this.A8.H)[2]-A[0])*8)/10)|0,((((A=this.
A8.H)[3]-A[1])*1)/10)|0);this.Co.Id(1,((((A=this.A8.H)[2]-A[0])*8)/10)|0,((((A=this.
A8.H)[3]-A[1])*9)/10)|0);this.Co.Id(1,((((A=this.A8.H)[2]-A[0])*6)/10)|0,((((A=this.
A8.H)[3]-A[1])*9)/10)|0);this.Co.EJ(1);}else{this.Co.MT(2);this.Co.BD(0,3);this.
Co.Ff(0,((((A=this.A8.H)[2]-A[0])*3)/10)|0,((((A=this.A8.H)[3]-A[1])*2)/10)|0);this.
Co.Id(0,((((A=this.A8.H)[2]-A[0])*17)/20)|0,((((A=this.A8.H)[3]-A[1])*5)/10)|0);
this.Co.Id(0,((((A=this.A8.H)[2]-A[0])*3)/10)|0,((((A=this.A8.H)[3]-A[1])*8)/10)|
0);this.Co.EJ(0);this.Co.BD(1,0);}},TY:function(C){if(B.tn(this.Hz,C))return;if(
!!this.Hz)B.sO([this,this.PS],this.Hz,0);this.Hz=C;if(!!C)B.sB([this,this.PS],C,
0);if(!!C)B.lq([this,this.PS],this);},PS:function(Af){var A;if(!!this.Hz)this.Lq((
A=this.Hz,A[1].call(A[0])));},Lq:function(C){if(this.FM===C)return;this.FM=C;B.lq([
this,this.Sc],this);this.Bn();},_Init:function(aArg){B.ul.De._Init.call(this,aArg
);B.Graphics.CG._Init.call(this.HL={M:this},0);B.Graphics.CG._Init.call(this.CQ={
M:this},0);B.Graphics.CG._Init.call(this.D$={M:this},0);B.Graphics.CG._Init.call(
this.E8={M:this},0);B.Graphics.CG._Init.call(this.E9={M:this},0);B.Graphics.AK._Init.
call(this.Co={M:this},0);B.uj.CJ._Init.call(this.F_={M:this},0);B.uj.CJ._Init.call(
this.Dg={M:this},0);B.uj.CJ._Init.call(this.Gs={M:this},0);B.uj.CJ._Init.call(this.
Gt={M:this},0);B.uj.CJ._Init.call(this.Gu={M:this},0);B.uj.CJ._Init.call(this.A8={
M:this},0);B.Core.OU._Init.call(this.Ap={M:this},0);this.__proto__=E.OZ;this.P(Iz
);this.HL.CB(360.000000);this.HL.Ir(0);this.CQ.CB(90.000000);this.CQ.Ir(0);this.
D$.CB(90.000000);this.D$.Ir(0);this.E8.CB(90.000000);this.E8.Ir(0);this.E9.CB(90.000000
);this.E9.Ir(0);this.F_.P(Iz);this.F_.CI(3.000000);this.Dg.P(Iz);this.Gs.P(Iz);this.
Gt.P(Iz);this.Gu.P(Iz);this.A8.P(Iz);this.A8.TQ(1);this.A8.CI(15.000000);this.Ap.
P(Iz);this.Z(this.F_,0);this.Z(this.Dg,0);this.Z(this.Gs,0);this.Z(this.Gt,0);this.
Z(this.Gu,0);this.Z(this.A8,0);this.Z(this.Ap,0);this.F_.EL(this.HL);this.Dg.EL(
this.CQ);this.Gs.EL(this.D$);this.Gt.EL(this.E8);this.Gu.EL(this.E9);this.A8.EL(
this.Co);this.Ap.OJ=[this,this.VU];this.Ap.Et=[this,this.L7];this.Ap.Dr=[this,this.
L6];},_Done:function(){this.__proto__=B.ul.De;this.HL._Done();this.CQ._Done();this.
D$._Done();this.E8._Done();this.E9._Done();this.Co._Done();this.F_._Done();this.
Dg._Done();this.Gs._Done();this.Gt._Done();this.Gu._Done();this.A8._Done();this.
Ap._Done();B.ul.De._Done.call(this);},_ReInit:function(){B.ul.De._ReInit.call(this
);this.HL._ReInit();this.CQ._ReInit();this.D$._ReInit();this.E8._ReInit();this.E9.
_ReInit();this.Co._ReInit();this.F_._ReInit();this.Dg._ReInit();this.Gs._ReInit(
);this.Gt._ReInit();this.Gu._ReInit();this.A8._ReInit();this.Ap._ReInit();},_Mark:
function(D){var A;B.ul.De._Mark.call(this,D);if((A=this.QE)&&((A=A[0])._cycle!=D
))A._Mark(A._cycle=D);if((A=this.In)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((
A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Hz)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.HL)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
CQ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.D$)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.E8)._cycle!=D)A._Mark(A._cycle=D);if((A=this.E9)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Co)._cycle!=D)A._Mark(A._cycle=D);if((A=this.F_)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.Dg)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Gs)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.Gt)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Gu
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.A8)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Ap)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::TimerWheel"};
E.MY={GT:null,D5:null,CP:null,Gp:null,Gq:null,Go:null,PN:function(Af){if(this.Hu(
false))this.G.E1(B._NewObject(E.GS,0),B._GetAutoObject(B.uk.G5),null,null,null,null
,null,null,null,null,false);},_Init:function(aArg){B.Core.J._Init.call(this,aArg
);B.Core.G4._Init.call(this.GT={M:this},0);B.uj.FQ._Init.call(this.D5={M:this},0
);B.um.CP._Init.call(this.CP={M:this},0);B.um.Lz._Init.call(this.Gp={M:this},0);
B.um.Lz._Init.call(this.Gq={M:this},0);B.um.Lz._Init.call(this.Go={M:this},0);this.
__proto__=E.MY;var A;this.P(HN);this.GT.G1(Ve);this.GT.G0(Vf);this.GT.Ct(Vg);this.
GT.Cs(Rh);this.D5.P(Rb);this.D5.Fm(0);this.CP.TD(0x3F);this.CP.TH(0xFF404040);this.
CP.TK(0xFF404040);this.CP.TJ(0xFF404040);this.CP.TI(0xFF404040);this.Gp.P(Vh);this.
Gp.Iq(true);this.Gp.OL(0);this.Gq.P(Vi);this.Gq.QW(1);this.Gq.OL(1);this.Go.P(Vj
);this.Go.QW(2);this.Go.OL(2);this.Z(this.GT,0);this.Z(this.D5,0);this.Z(this.Gp
,0);this.Z(this.Gq,0);this.Z(this.Go,0);this.GT.CH=[this,this.PN];this.D5.Dq(B.s$(
B.ui.Ik));this.CP.TL(B.s$(B.ui.Ix));this.CP.TN(B.s$(B.ui.Ix));this.CP.TM(B.s$(B.
ui.Ix));this.CP.TE(B.s$(B.ui.Ix));this.CP.TG(B.s$(B.ui.Ix));this.CP.TF(B.s$(B.ui.
Ix));this.Gp.I8([A=B._GetAutoObject(E.Device),A.I6,A.GY]);this.Gp.OK(B.s$(E.KZ));
this.Gp.Ke(this.CP);this.Gq.I8([A=B._GetAutoObject(E.Device),A.I6,A.GY]);this.Gq.
OK(B.s$(E.KZ));this.Gq.Ke(this.CP);this.Go.I8([A=B._GetAutoObject(E.Device),A.I6
,A.GY]);this.Go.OK(B.s$(E.KZ));this.Go.Ke(this.CP);},_Done:function(){this.__proto__=
B.Core.J;this.GT._Done();this.D5._Done();this.CP._Done();this.Gp._Done();this.Gq.
_Done();this.Go._Done();B.Core.J._Done.call(this);},_ReInit:function(){B.Core.J.
_ReInit.call(this);this.GT._ReInit();this.D5._ReInit();this.CP._ReInit();this.Gp.
_ReInit();this.Gq._ReInit();this.Go._ReInit();},_Mark:function(D){var A;B.Core.J.
_Mark.call(this,D);if((A=this.GT)._cycle!=D)A._Mark(A._cycle=D);if((A=this.D5)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.CP)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
Gp)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Gq)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Go)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::StyleScreen"
};E.Od={_class:function(){return B.ui.BW;},0:{Data:function(){return B.uo;},Cache:[
],_this:null}};E.Gl={_class:function(){return B.ui.BW;},0:{Data:function(){return B.
up;},Cache:[],_this:null}};E.Qi={FO:1,Ua:function(Ro){},GY:function(C){if(this.FO===
C)return;this.FO=C;B.tG([this,this.I6,this.GY],0);},I6:function(){return this.FO;
},_Init:function(aArg){this.__proto__=E.Qi;B.gv++;},_Done:function(){this.__proto__=
null;B.gv--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.M)&&(A._cycle
!=D))A._Mark(A._cycle=D);},M:null,_cycle:0,_observers:null,_className:"BezierClock::DeviceClass"
};E.Device={_Init:function(){E.Qi._Init.call(this,0);},_variants:function(){return this;
},_this:null};E.KZ={_class:function(){return B.ui.An;},0:{FileName:"./res/BezierClockBackgroundPreview.png"
,Format:B.ch,NoOfFrames:3,FrameSize:[120,60],FrameDelay:0,_this:null}};E.Blue={_class:
function(){return B.ui.An;},0:{FileName:"./res/BezierClockBlue.png",Format:B.ch,
NoOfFrames:1,FrameSize:[510,272],FrameDelay:0,_this:null}};E.Mn={_class:function(
){return B.ui.An;},0:{FileName:"./res/BezierClockDark.png",Format:B.ch,NoOfFrames:
1,FrameSize:[510,272],FrameDelay:0,_this:null}};E.OP={_class:function(){return B.
ui.An;},0:{FileName:"./res/BezierClockOrange.png",Format:B.ch,NoOfFrames:1,FrameSize:[
510,272],FrameDelay:0,_this:null}};
E._Init=function(){E.Ew.__proto__=B.ul.Ev;E.GS.__proto__=B.Core.J;E.K0.__proto__=
B.Core.J;E.MU.__proto__=B.Core.J;E.Er.__proto__=B.ul.Ev;E.Cp.__proto__=B.Core.J;
E.C$.__proto__=B.Core.J;E.Az.__proto__=B.Core.J;E.MZ.__proto__=B.Core.J;E.OZ.__proto__=
B.ul.De;E.MY.__proto__=B.Core.J;};E.DC=function(D){var A;if((A=E.Od[0]._this)&&(
A._cycle!=D))A._Done(E.Od[0]._this=null);if((A=E.Gl[0]._this)&&(A._cycle!=D))A._Done(
E.Gl[0]._this=null);if((A=E.Device._this)&&(A._cycle!=D))A._Done(E.Device._this=
null);if((A=E.KZ[0]._this)&&(A._cycle!=D))A._Done(E.KZ[0]._this=null);if((A=E.Blue[
0]._this)&&(A._cycle!=D))A._Done(E.Blue[0]._this=null);if((A=E.Mn[0]._this)&&(A.
_cycle!=D))A._Done(E.Mn[0]._this=null);if((A=E.OP[0]._this)&&(A._cycle!=D))A._Done(
E.OP[0]._this=null);};return E;})();

/* Embedded Wizard */