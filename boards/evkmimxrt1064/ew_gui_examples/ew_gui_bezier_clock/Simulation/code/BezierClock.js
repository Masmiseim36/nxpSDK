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
);if(EmWiApp.BezierClock)throw new Error("The unit file 'BezierClock.js' included twice!"
);EmWiApp.BezierClock=(function(){var B=EmWiApp;var E={};
var AJ=[0,-3];var Be=[0,0,172,150];var Db=[119,0,162,150];var CD=[0,80];var Fk=[65
,0,107,150];var Ip=[10,0,53,150];var I9=[102,57,122,89];var HE=":";var Iq=[47,57
,67,89];var Lm=[53,99];var MN=[10,99];var OJ=[52,51];var OK=[9,51];var OL=[107,51
];var OM=[65,51];var ON=[107,99];var OO=[65,99];var OP=[162,99];var OQ=[119,99];
var OR=[162,51];var OS=[119,51];var OT=", ";var OU=" ";var Gq=[0,0,480,272];var OV=[
60,215];var OW=[419,215];var QS=[419,271];var T3=[60,271];var QT=[419,0];var T4=[
479,0];var T5=[479,271];var OX=[60,0];var T6=[419,56];var T7=[60,56];var T8=[0,172
,480,226];var T9="Text";var T_=[188,148,218,178];var MO=[15,15];var T$=[188,112,
218,142];var Ua=[47,53,119,176];var Ub=[119,53,191,176];var Uc=[215,53,287,176];
var Ud=[287,53,359,176];var Ue=[361,118,393,172];var Uf=[399,118,431,172];var Ug=[
0,0,480,20];var QU=[0,250,480,272];var Uh=[460,0,480,272];var Ui=[348,200,468,250
];var Uj="Confirm";var Uk=[12,200,132,250];var Ul="Cancel";var Um=[289,39,461,189
];var Un=[19,39,279,189];var Uo=[420,0];var Uq=[420,36];var Ur=[60,36];var Us=[0
,0,480,18];var Ut=[0,0,260,150];var QV=[160,0,250,150];var QW=[79,0,149,150];var
QX=[10,0,67,150];var Uu=[67,51];var Uv=[10,51];var Uw=[67,99];var Ux=[149,99];var
Uy=[79,99];var Uz=[149,51];var UA=[79,51];var UB=[249,51];var UC=[160,51];var UD=[
249,99];var UE=[160,99];var QY=[10,0];var QZ=[0,10];var UF=[0,0,180,250];var UG=[
0,0,160,160];var OY=[0,0,190,160];var OZ=[0,0];var UH=[190,270,310,390];var UI=[
225,144,255,174];var UJ=[225,116,255,146];var UK=[133,99,177,169];var UL=[182,99
,226,169];var UM=[254,99,298,169];var UN=[302,99,346,169];var UO=[215,172,238,206
];var UP=[240,172,265,206];var UQ=[112,8,368,264];var Q0=[60,270];var UR=[0,270];
var US=[0,0,20,270];var Ir=[0,0,200,200];var UT=[60,224];var UU=[420,224];var UV=[
420,270];var UW=[20,90,150,160];var UX=[175,90,305,160];var UY=[330,90,460,160];
E.D5={Id:null,EZ:null,EX:null,EW:null,B5:null,B3:null,B2:null,Hy:null,Hx:null,CV:
null,CW:null,CX:null,CY:null,CZ:null,GN:null,IS:0xFFFFFFFF,IT:0x44FFFFFF,E$:0xFFC1C1C1
,E_:0xFFE1E1E1,Lz:false,LZ:false,PK:false,G4:function(aSize){this.B2.FR((aSize[1
]/3)|0);this.B3.FR((aSize[1]/3)|0);this.B5.FR((aSize[1]/3)|0);this.B2.BT((aSize[
1]/3)|0);this.B3.BT((aSize[1]/3)|0);this.B5.BT((aSize[1]/3)|0);this.EW.FS([0,(aSize[
1]/3)|0]);this.EX.FS([0,(aSize[1]/3)|0]);this.EZ.FS([0,(aSize[1]/3)|0]);},DM:function(
B7){var A;B.th.ES.DM.call(this,B7);var De=((B7&0x10)===0x10);var Kv=((B7&0x20)===
0x20);var LF=(this.EW.DL||this.EX.DL)||this.EZ.DL;if(LF){this.GN.Ab(this.E$);this.
CV.Ab(this.E$);this.CW.Ab(this.E$);this.CX.Ab(this.E$);this.CY.Ab(this.E$);this.
CZ.Ab(this.E$);}else{this.GN.Ab(this.E_);this.CV.Ab(this.E_);this.CW.Ab(this.E_);
this.CX.Ab(this.E_);this.CY.Ab(this.E_);this.CZ.Ab(this.E_);}this.Lz=De;this.LZ=
Kv;this.PK=LF;},JB:function(Ae){this.Bl();if((!this.EW.DL&&!this.EX.DL)&&!this.EZ.
DL)B.kD(this.Id,this);},JE:function(Ae){this.Bl();},Ti:function(Ae){var A;var GC=
this.B5.EQ;var Ar=(B.tf.Text.isPrototypeOf(A=this.B5.AD)?A:null);if(!Ar)return;Ar.
Cn(B.su(GC,2,10));Ar.Fd(B.r0(E.Gh));Ar.Ab(0xFFFFFFFF);Ar.ER(0x12);Ar.BT(AJ);Ar.Q(
B.sT(Ar.H,[(A=this.B5.H)[2]-A[0],this.B5.AW]));},Tg:function(Ae){var A;var GC=this.
B3.EQ;var Ar=(B.tf.Text.isPrototypeOf(A=this.B3.AD)?A:null);if(!Ar)return;Ar.Cn(
B.su(GC,2,10));Ar.Fd(B.r0(E.Gh));Ar.Ab(0xFFFFFFFF);Ar.ER(0x12);Ar.BT(AJ);Ar.Q(B.
sT(Ar.H,[(A=this.B3.H)[2]-A[0],this.B3.AW]));},Tf:function(Ae){var A;var GC=this.
B2.EQ;var Ar=(B.tf.Text.isPrototypeOf(A=this.B2.AD)?A:null);if(!Ar)return;Ar.Cn(
B.su(GC,2,10));Ar.Fd(B.r0(E.Gh));Ar.Ab(0xFFFFFFFF);Ar.ER(0x12);Ar.BT(AJ);Ar.Q(B.
sT(Ar.H,[(A=this.B2.H)[2]-A[0],this.B2.AW]));},Td:function(){return(((-this.B5.AU
/this.B5.AW)|0)+1)%60;},Mx:function(C){if(C<0)C=0;if(C>59)C=59;this.B5.BT((C-1)*-
this.B5.AW);},Ta:function(){return(((-this.B3.AU/this.B3.AW)|0)+1)%60;},Mv:function(
C){if(C<0)C=0;if(C>59)C=59;this.B3.BT((C-1)*-this.B3.AW);},S$:function(){return(((-
this.B2.AU/this.B2.AW)|0)+1)%24;},Mu:function(C){if(C<0)C=0;if(C>23)C=23;this.B2.
BT((C-1)*-this.B2.AW);},Vr:function(Ae){var A;var L=this.B2.Am;while(!!L){var Z=(
B.tf.Text.isPrototypeOf(L)?L:null);if(!!Z){Z.Ab(this.IT);}L=L.U;}var RL=(B.tf.Text.
isPrototypeOf(A=this.B2.JY(this.B2.JX(B.rZ(this.B2.H))))?A:null);if(!!RL){RL.Ab(
this.IS);}},Vt:function(Ae){var A;var L=this.B3.Am;while(!!L){var Z=(B.tf.Text.isPrototypeOf(
L)?L:null);if(!!Z){Z.Ab(this.IT);}L=L.U;}var RM=(B.tf.Text.isPrototypeOf(A=this.
B3.JY(this.B3.JX(B.rZ(this.B3.H))))?A:null);if(!!RM){RM.Ab(this.IS);}},Vw:function(
Ae){var A;var L=this.B5.Am;while(!!L){var Z=(B.tf.Text.isPrototypeOf(L)?L:null);
if(!!Z){Z.Ab(this.IT);}L=L.U;}var RO=(B.tf.Text.isPrototypeOf(A=this.B5.JY(this.
B5.JX(B.rZ(this.B5.H))))?A:null);if(!!RO){RO.Ab(this.IS);}},_Init:function(aArg){
B.th.ES._Init.call(this,aArg);B.Core.Im._Init.call(this.EZ={N:this},0);B.Core.Im.
_Init.call(this.EX={N:this},0);B.Core.Im._Init.call(this.EW={N:this},0);B.Core.HB.
_Init.call(this.B5={N:this},0);B.Core.HB._Init.call(this.B3={N:this},0);B.Core.HB.
_Init.call(this.B2={N:this},0);B.tf.Text._Init.call(this.Hy={N:this},0);B.tf.Text.
_Init.call(this.Hx={N:this},0);B.tf.Ck._Init.call(this.CV={N:this},0);B.tf.Ck._Init.
call(this.CW={N:this},0);B.tf.Ck._Init.call(this.CX={N:this},0);B.tf.Ck._Init.call(
this.CY={N:this},0);B.tf.Ck._Init.call(this.CZ={N:this},0);B.tf.Ck._Init.call(this.
GN={N:this},0);this.__proto__=E.D5;this.Q(Be);this.EZ.Q(Db);this.EZ.FS(CD);this.
EZ.J5(0.200000);this.EX.Q(Fk);this.EX.FS(CD);this.EX.J5(0.200000);this.EW.Q(Ip);
this.EW.FS(CD);this.EW.J5(0.200000);this.B5.Q(Db);this.B5.J4(true);this.B5.BT(80
);this.B5.FR(80);this.B5.Hp(60);this.B3.Q(Fk);this.B3.J4(true);this.B3.BT(80);this.
B3.FR(80);this.B3.Hp(60);this.B2.Q(Ip);this.B2.J4(true);this.B2.BT(80);this.B2.FR(
80);this.B2.Hp(24);this.Hy.Q(I9);this.Hy.Cn(HE);this.Hy.Ab(0xFFFFFFFF);this.Hx.Q(
Iq);this.Hx.Cn(HE);this.Hx.Ab(0xFFFFFFFF);this.CV.Cm(Lm);this.CV.Cl(MN);this.CV.
CB(3);this.CW.Cm(OJ);this.CW.Cl(OK);this.CW.CB(3);this.CX.Cm(OL);this.CX.Cl(OM);
this.CX.CB(3);this.CY.Cm(ON);this.CY.Cl(OO);this.CY.CB(3);this.CZ.Cm(OP);this.CZ.
Cl(OQ);this.CZ.CB(3);this.GN.Cm(OR);this.GN.Cl(OS);this.GN.CB(3);this.Aa(this.EZ
,0);this.Aa(this.EX,0);this.Aa(this.EW,0);this.Aa(this.B5,0);this.Aa(this.B3,0);
this.Aa(this.B2,0);this.Aa(this.Hy,0);this.Aa(this.Hx,0);this.Aa(this.CV,0);this.
Aa(this.CW,0);this.Aa(this.CX,0);this.Aa(this.CY,0);this.Aa(this.CZ,0);this.Aa(this.
GN,0);this.EZ.Em=[this,this.JB];this.EZ.Dm=[this,this.JE];this.EX.Em=[this,this.
JB];this.EX.Dm=[this,this.JE];this.EW.Em=[this,this.JB];this.EW.Dm=[this,this.JE
];this.B5.FU=[this,this.Vw];this.B5.FN=[this,this.Ti];this.B5.J8(this.EZ);this.B3.
FU=[this,this.Vt];this.B3.FN=[this,this.Tg];this.B3.J8(this.EX);this.B2.FU=[this
,this.Vr];this.B2.FN=[this,this.Tf];this.B2.J8(this.EW);this.Hy.Fd(B.r0(E.Gh));this.
Hx.Fd(B.r0(E.Gh));},_Done:function(){this.__proto__=E.D5;this.EZ._Done();this.EX.
_Done();this.EW._Done();this.B5._Done();this.B3._Done();this.B2._Done();this.Hy.
_Done();this.Hx._Done();this.CV._Done();this.CW._Done();this.CX._Done();this.CY.
_Done();this.CZ._Done();this.GN._Done();B.th.ES._Done.call(this);},_ReInit:function(
){B.th.ES._ReInit.call(this);this.EZ._ReInit();this.EX._ReInit();this.EW._ReInit(
);this.B5._ReInit();this.B3._ReInit();this.B2._ReInit();this.Hy._ReInit();this.Hx.
_ReInit();this.CV._ReInit();this.CW._ReInit();this.CX._ReInit();this.CY._ReInit(
);this.CZ._ReInit();this.GN._ReInit();},_Mark:function(D){var A;B.th.ES._Mark.call(
this,D);if((A=this.Id)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.EZ).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.EX)._cycle!=D)A._Mark(A._cycle=D);if((A=
this.EW)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B5)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.B3)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B2)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.Hy)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Hx)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.CV)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CW
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CX)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.CY)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CZ)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.GN)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::TimePicker"
};E.Gc={Timer:null,Af:null,CR:null,GZ:null,G1:null,G2:null,Date:null,CP:null,CQ:
null,BW:null,BX:null,BY:null,BZ:null,BP:null,BQ:null,D0:null,DZ:null,H9:null,CU:
function(aArg){B.kD([this,this.Ri],this);},Vz:function(Ae){var A;if(!this.Hn(false
))return;this.G.EU(B._NewObject(E.Lk,0),B._GetAutoObject(B.tg.Ka),B._GetAutoObject(
B.tg.Kb),null,null,B._GetAutoObject(B.tg.Ka),null,null,null,null,false);(E.JP.isPrototypeOf(
A=this.G)?A:null).Ay.ShiftLeft();},Vx:function(Ae){if(this.Hn(false))this.G.EU(B.
_NewObject(E.Li,0),B._GetAutoObject(B.tg.GX),B._GetAutoObject(B.tg.Gm),null,null
,B._GetAutoObject(B.tg.GX),null,null,null,null,false);},Vy:function(Ae){if(this.
Hn(false))this.G.EU(B._NewObject(E.Lj,0),B._GetAutoObject(B.tg.Gm),B._GetAutoObject(
B.tg.GX),null,null,B._GetAutoObject(B.tg.Gm),null,null,null,null,false);},RW:function(
Ae){var DO=this.Af.Qs();this.BW.DJ((DO.KZ/10)|0);this.BX.DJ(DO.KZ%10);this.BY.DJ((
DO.K3/10)|0);this.BZ.DJ(DO.K3%10);this.BP.DJ((DO.Lh/10)|0);this.BQ.DJ(DO.Lh%10);
this.Date.Cn((((DO.Vh(DO.S9(),true)+OT)+DO.Vi(DO.K4,false))+OU)+B.su(DO.KL,2,10)
);},Vf:function(Ae){var FE=700;this.BW.DI(FE);this.BX.DI(FE);this.BY.DI(FE);this.
BZ.DI(FE);this.BP.DI(FE);this.BQ.DI(FE);this.RW(this);},Ri:function(Ae){var FE=2000;
this.BW.DI(FE);this.BX.DI(FE);this.BY.DI(FE);this.BZ.DI(FE);this.BP.DI(FE);this.
BQ.DI(FE);this.RW(this);this.Timer.Ii=[this,this.Vf];},_Init:function(aArg){B.Core.
K._Init.call(this,aArg);B.Core.Timer._Init.call(this.Timer={N:this},0);B.Core.Af.
_Init.call(this.Af={N:this},0);B.Graphics.Cz._Init.call(this.CR={N:this},0);B.Core.
GW._Init.call(this.GZ={N:this},0);B.Core.GW._Init.call(this.G1={N:this},0);B.Core.
GW._Init.call(this.G2={N:this},0);B.tf.Text._Init.call(this.Date={N:this},0);B.tf.
B1._Init.call(this.CP={N:this},0);B.tf.B1._Init.call(this.CQ={N:this},0);E.Ca._Init.
call(this.BW={N:this},0);E.Ca._Init.call(this.BX={N:this},0);E.Ca._Init.call(this.
BY={N:this},0);E.Ca._Init.call(this.BZ={N:this},0);E.Ca._Init.call(this.BP={N:this
},0);E.Ca._Init.call(this.BQ={N:this},0);B.tf.FK._Init.call(this.D0={N:this},0);
B.tf.FK._Init.call(this.DZ={N:this},0);B.tf.FK._Init.call(this.H9={N:this},0);this.
__proto__=E.Gc;this.Q(Gq);this.Timer.If(2000);this.Timer.Bs(true);this.CR.QD(20);
this.CR.Ct(360.000000);this.CR.Ot(6.100000);this.CR.Ih(2);this.GZ.GS(OV);this.GZ.
GR(OW);this.GZ.Cm(QS);this.GZ.Cl(T3);this.G1.GS(QT);this.G1.GR(T4);this.G1.Cm(T5
);this.G1.Cl(QS);this.G2.GS(OX);this.G2.GR(QT);this.G2.Cm(T6);this.G2.Cl(T7);this.
Date.C9(0x3);this.Date.Q(T8);this.Date.Cn(T9);this.CP.C9(0x3);this.CP.Q(T_);this.
CP.Ab(0xDDFFFFFF);this.CP.En(MO);this.CQ.C9(0x3);this.CQ.Q(T$);this.CQ.Ab(0xDDFFFFFF
);this.CQ.En(MO);this.BW.C9(0x3);this.BW.Q(Ua);this.BW.FT(6.100000);this.BW.FP(60
);this.BW.FQ(false);this.BX.C9(0x3);this.BX.Q(Ub);this.BX.FP(60);this.BX.FT(6.100000
);this.BX.FQ(false);this.BY.C9(0x3);this.BY.Q(Uc);this.BY.FP(60);this.BY.FT(6.100000
);this.BY.FQ(false);this.BZ.C9(0x3);this.BZ.Q(Ud);this.BZ.FP(60);this.BZ.FT(6.100000
);this.BZ.FQ(false);this.BP.C9(0x3);this.BP.Q(Ue);this.BP.FP(50);this.BP.Ab(0xDDFFFFFF
);this.BP.FT(2.300000);this.BP.FQ(false);this.BQ.C9(0x3);this.BQ.Q(Uf);this.BQ.FP(
50);this.BQ.Ab(0xDDFFFFFF);this.BQ.FT(2.300000);this.BQ.FQ(false);this.D0.Q(Ug);
this.D0.Fe(1);this.DZ.Q(QU);this.DZ.Fe(0);this.H9.Q(Uh);this.H9.Fe(2);this.Aa(this.
GZ,0);this.Aa(this.G1,0);this.Aa(this.G2,0);this.Aa(this.Date,0);this.Aa(this.CP
,0);this.Aa(this.CQ,0);this.Aa(this.BW,0);this.Aa(this.BX,0);this.Aa(this.BY,0);
this.Aa(this.BZ,0);this.Aa(this.BP,0);this.Aa(this.BQ,0);this.Aa(this.D0,0);this.
Aa(this.DZ,0);this.Aa(this.H9,0);this.Timer.Ii=[this,this.Ri];this.GZ.CA=[this,this.
Vx];this.G1.CA=[this,this.Vz];this.G2.CA=[this,this.Vy];this.Date.Fd(B.r0(E.Gh));
this.CP.EB(this.CR);this.CQ.EB(this.CR);this.D0.Dl(B.r0(B.te.Ib));this.DZ.Dl(B.r0(
B.te.Ib));this.H9.Dl(B.r0(B.te.Ib));this.CU(aArg);},_Done:function(){this.__proto__=
E.Gc;this.Timer._Done();this.Af._Done();this.CR._Done();this.GZ._Done();this.G1.
_Done();this.G2._Done();this.Date._Done();this.CP._Done();this.CQ._Done();this.BW.
_Done();this.BX._Done();this.BY._Done();this.BZ._Done();this.BP._Done();this.BQ.
_Done();this.D0._Done();this.DZ._Done();this.H9._Done();B.Core.K._Done.call(this
);},_ReInit:function(){B.Core.K._ReInit.call(this);this.Timer._ReInit();this.Af.
_ReInit();this.CR._ReInit();this.GZ._ReInit();this.G1._ReInit();this.G2._ReInit(
);this.Date._ReInit();this.CP._ReInit();this.CQ._ReInit();this.BW._ReInit();this.
BX._ReInit();this.BY._ReInit();this.BZ._ReInit();this.BP._ReInit();this.BQ._ReInit(
);this.D0._ReInit();this.DZ._ReInit();this.H9._ReInit();},_Mark:function(D){var A;
B.Core.K._Mark.call(this,D);if((A=this.Timer)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Af)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CR)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.GZ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.G1)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.G2)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Date)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.CP)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
CQ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BW)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.BX)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BY)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.BZ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BP)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.BQ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.D0)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.DZ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.H9
)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::ClockScreen"};E.JP={GT:
null,Ay:null,IR:null,KG:0,CU:function(aArg){this.QL(B._NewObject(E.Gc,0),null,null
,null,null,null,null,null,null,false);B.kD([this,this.Rv],this);},Rv:function(Ae
){this.Ay.GP(B._GetAutoObject(E.Device).FH);},R3:function(Ae){switch(this.KG){case
2:this.EU(B._NewObject(E.Lj,0),B._GetAutoObject(B.tg.Gm),B._GetAutoObject(B.tg.GX
),null,null,B._GetAutoObject(B.tg.Gm),null,null,null,null,false);break;case 3:if(
B._GetAutoObject(E.Device).FH===2)B._GetAutoObject(E.Device).GP(1);else B._GetAutoObject(
E.Device).GP(2);break;case 4:this.EU(B._NewObject(E.Gc,0),B._GetAutoObject(B.tg.
GX),null,null,null,null,null,null,null,null,false);break;case 6:{this.EU(B._NewObject(
E.Lk,0),B._GetAutoObject(B.tg.Ka),B._GetAutoObject(B.tg.Kb),null,null,B._GetAutoObject(
B.tg.Ka),null,null,null,null,false);this.Ay.ShiftLeft();}break;case 7:{this.EU(B.
_NewObject(E.Gc,0),B._GetAutoObject(B.tg.Kb),null,null,null,null,null,null,null,
null,false);this.Ay.ShiftRight();}break;case 8:this.EU(B._NewObject(E.Li,0),B._GetAutoObject(
B.tg.GX),B._GetAutoObject(B.tg.Gm),null,null,B._GetAutoObject(B.tg.GX),null,null
,null,null,false);break;case 9:this.EU(B._NewObject(E.Gc,0),B._GetAutoObject(B.tg.
Gm),null,null,null,null,null,null,null,null,false);break;default:;}this.KG=this.
KG+1;if(this.KG>9)this.KG=0;},_Init:function(aArg){B.Core.K._Init.call(this,aArg
);B.Core.GT._Init.call(this.GT={N:this},0);E.Ay._Init.call(this.Ay={N:this},0);B.
Core.Timer._Init.call(this.IR={N:this},0);this.__proto__=E.JP;var A;this.Q(Gq);this.
Ay.Q(Gq);this.IR.La(2000);this.IR.If(0);this.Aa(this.Ay,0);this.GT.Oo=[this,this.
Rv];this.GT.I1([A=B._GetAutoObject(E.Device),A.IZ,A.GP]);this.IR.Ii=[this,this.R3
];this.CU(aArg);},_Done:function(){this.__proto__=E.JP;this.GT._Done();this.Ay._Done(
);this.IR._Done();B.Core.K._Done.call(this);},_ReInit:function(){B.Core.K._ReInit.
call(this);this.GT._ReInit();this.Ay._ReInit();this.IR._ReInit();},_Mark:function(
D){var A;B.Core.K._Mark.call(this,D);if((A=this.GT)._cycle!=D)A._Mark(A._cycle=D
);if((A=this.Ay)._cycle!=D)A._Mark(A._cycle=D);if((A=this.IR)._cycle!=D)A._Mark(
A._cycle=D);},_className:"BezierClock::BezierClock"};E.Li={Hv:null,Hu:null,D5:null
,DW:null,G3:null,D0:null,EC:null,Af:null,DM:function(B7){B.Core.K.DM.call(this,B7
);var DO=this.Af.Qs();this.DW.My(DO.OI);this.DW.Mw(DO.K4);this.DW.I0(DO.KL);this.
D5.Mu(DO.KZ);this.D5.Mv(DO.K3);this.D5.Mx(DO.Lh);},Vp:function(Ae){if(!this.Hn(false
))return;this.Af.My(this.DW.Mt());this.Af.Mw(this.DW.Ms());this.Af.I0(this.DW.Mr(
));this.Af.Mu(this.D5.S$());this.Af.Mv(this.D5.Ta());this.Af.Mx(this.D5.Td());B.
_GetAutoObject(E.Device).TS(this.Af.Qt());this.G.EU(B._NewObject(E.Gc,0),B._GetAutoObject(
B.tg.Gm),null,null,null,null,null,null,null,null,false);},Jz:function(Ae){if(this.
Hn(false))this.G.EU(B._NewObject(E.Gc,0),B._GetAutoObject(B.tg.Gm),null,null,null
,null,null,null,null,null,false);},_Init:function(aArg){B.Core.K._Init.call(this
,aArg);B.ti.Le._Init.call(this.Hv={N:this},0);B.ti.Le._Init.call(this.Hu={N:this
},0);E.D5._Init.call(this.D5={N:this},0);E.DW._Init.call(this.DW={N:this},0);B.Core.
GW._Init.call(this.G3={N:this},0);B.tf.FK._Init.call(this.D0={N:this},0);B.ti.EC.
_Init.call(this.EC={N:this},0);B.Core.Af._Init.call(this.Af={N:this},0);this.__proto__=
E.Li;this.Q(Gq);this.Hv.Q(Ui);this.Hv.QB(Uj);this.Hu.Q(Uk);this.Hu.QB(Ul);this.D5.
Q(Um);this.DW.Q(Un);this.G3.GS(OX);this.G3.GR(Uo);this.G3.Cm(Uq);this.G3.Cl(Ur);
this.D0.Q(Us);this.D0.Fe(1);this.EC.Ty(0xFFC1C1C1);this.EC.TB(0xFFD1D1D1);this.EC.
TA(0x73BEBEBE);this.EC.Tz(0xFFFFFFFF);this.Aa(this.Hv,0);this.Aa(this.Hu,0);this.
Aa(this.D5,0);this.Aa(this.DW,0);this.Aa(this.G3,0);this.Aa(this.D0,0);this.Hv.Ic=[
this,this.Vp];this.Hv.J3(this.EC);this.Hu.Ic=[this,this.Jz];this.Hu.J3(this.EC);
this.G3.CA=[this,this.Jz];this.D0.Dl(B.r0(B.te.Ib));this.EC.TC(B.r0(E.NV));},_Done:
function(){this.__proto__=E.Li;this.Hv._Done();this.Hu._Done();this.D5._Done();this.
DW._Done();this.G3._Done();this.D0._Done();this.EC._Done();this.Af._Done();B.Core.
K._Done.call(this);},_ReInit:function(){B.Core.K._ReInit.call(this);this.Hv._ReInit(
);this.Hu._ReInit();this.D5._ReInit();this.DW._ReInit();this.G3._ReInit();this.D0.
_ReInit();this.EC._ReInit();this.Af._ReInit();},_Mark:function(D){var A;B.Core.K.
_Mark.call(this,D);if((A=this.Hv)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Hu)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.D5)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
DW)._cycle!=D)A._Mark(A._cycle=D);if((A=this.G3)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.D0)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EC)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Af)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::SettingsScreen"
};E.DW={Id:null,E0:null,EY:null,EV:null,BS:null,B4:null,BC:null,Ck:null,CV:null,
CW:null,CX:null,CY:null,CZ:null,IS:0xFFFFFFFF,IT:0x44FFFFFF,E$:0xFFC1C1C1,E_:0xFFE1E1E1
,Lz:false,LZ:false,PK:false,G4:function(aSize){this.BC.FR((aSize[1]/3)|0);this.B4.
FR((aSize[1]/3)|0);this.BS.FR((aSize[1]/3)|0);this.BC.BT((aSize[1]/3)|0);this.B4.
BT((aSize[1]/3)|0);this.BS.BT((aSize[1]/3)|0);this.EV.FS([0,(aSize[1]/3)|0]);this.
EY.FS([0,(aSize[1]/3)|0]);this.E0.FS([0,(aSize[1]/3)|0]);},DM:function(B7){var A;
B.th.ES.DM.call(this,B7);var De=((B7&0x10)===0x10);var Kv=((B7&0x20)===0x20);var
LF=(this.EV.DL||this.EY.DL)||this.E0.DL;if(LF){this.Ck.Ab(this.E$);this.CV.Ab(this.
E$);this.CW.Ab(this.E$);this.CX.Ab(this.E$);this.CY.Ab(this.E$);this.CZ.Ab(this.
E$);}else{this.Ck.Ab(this.E_);this.CV.Ab(this.E_);this.CW.Ab(this.E_);this.CX.Ab(
this.E_);this.CY.Ab(this.E_);this.CZ.Ab(this.E_);}this.Lz=De;this.LZ=Kv;this.PK=
LF;},JB:function(Ae){this.Bl();if((!this.EV.DL&&!this.EY.DL)&&!this.E0.DL){var E4=
this.M$(this.Ms(),this.Mt());var Es=this.Mr();this.BC.Hp(E4);if(Es>E4)this.I0(E4
);B.kD(this.Id,this);}},JE:function(Ae){this.Bl();},Tj:function(Ae){var A;var GC=
this.BS.EQ;var Ar=(B.tf.Text.isPrototypeOf(A=this.BS.AD)?A:null);if(!Ar)return;Ar.
Cn(B.su(GC+2000,4,10));Ar.Fd(B.r0(E.Gh));Ar.Ab(0xFFFFFFFF);Ar.ER(0x12);Ar.BT(AJ);
Ar.Q(B.sT(Ar.H,[(A=this.BS.H)[2]-A[0],this.BS.AW]));},Th:function(Ae){var A;var GC=
this.B4.EQ;var Ar=(B.tf.Text.isPrototypeOf(A=this.B4.AD)?A:null);if(!Ar)return;switch(
GC){case 1:Ar.Cn(B.te.P6);break;case 2:Ar.Cn(B.te.Qg);break;case 3:Ar.Cn(B.te.PX
);break;case 4:Ar.Cn(B.te.Qi);break;case 5:Ar.Cn(B.te.Qd);break;case 6:Ar.Cn(B.te.
Qc);break;case 7:Ar.Cn(B.te.PY);break;case 8:Ar.Cn(B.te.QP);break;case 9:Ar.Cn(B.
te.Qm);break;case 10:Ar.Cn(B.te.Ql);break;case 11:Ar.Cn(B.te.P1);break;default:Ar.
Cn(B.te.Qb);}Ar.Fd(B.r0(E.Gh));Ar.Ab(0xFFFFFFFF);Ar.ER(0x12);Ar.BT(AJ);Ar.Q(B.sT(
Ar.H,[(A=this.B4.H)[2]-A[0],this.B4.AW]));},Te:function(Ae){var A;var GC=this.BC.
EQ;var Ar=(B.tf.Text.isPrototypeOf(A=this.BC.AD)?A:null);if(!Ar)return;Ar.Cn(B.su(
GC+1,2,10));Ar.Fd(B.r0(E.Gh));Ar.Ab(0xFFFFFFFF);Ar.ER(0x12);Ar.BT(AJ);Ar.Q(B.sT(
Ar.H,[(A=this.BC.H)[2]-A[0],this.BC.AW]));},Mt:function(){return 2000+((((-this.
BS.AU/this.BS.AW)|0)+1)%this.BS.A4);},My:function(C){if(C<2000)C=2000;if(C>2036)
C=2036;this.BS.BT(((C-2000)-1)*-this.BS.AW);var E4=this.M$(this.Ms(),C);this.BC.
Hp(E4);if(this.Mr()>E4)this.I0(E4);},Ms:function(){return 1+((((-this.B4.AU/this.
B4.AW)|0)+1)%12);},Mw:function(C){if(C<1)C=1;if(C>12)C=12;this.B4.BT((C-2)*-this.
B4.AW);var E4=this.M$(C,this.Mt());this.BC.Hp(E4);if(this.Mr()>E4)this.I0(E4);},
Mr:function(){return 1+((((-this.BC.AU/this.BC.AW)|0)+1)%this.BC.A4);},I0:function(
C){var E4=this.M$(this.Ms(),this.Mt());if(C<1)C=1;if(C>E4)C=E4;this.BC.BT((C-2)*-
this.BC.AW);},M$:function(FZ,O5){if(FZ===2)if(!(O5%4)&&(!!(O5%100)||!(O5%400)))return 29;
else return 28;else if((((FZ===4)||(FZ===6))||(FZ===9))||(FZ===11))return 30;else
return 31;},Vq:function(Ae){var A;var L=this.BC.Am;while(!!L){var Z=(B.tf.Text.isPrototypeOf(
L)?L:null);if(!!Z){Z.Ab(this.IT);}L=L.U;}var RK=(B.tf.Text.isPrototypeOf(A=this.
BC.JY(this.BC.JX(B.rZ(this.BC.H))))?A:null);if(!!RK){RK.Ab(this.IS);}},Vu:function(
Ae){var A;var L=this.B4.Am;while(!!L){var Z=(B.tf.Text.isPrototypeOf(L)?L:null);
if(!!Z){Z.Ab(this.IT);}L=L.U;}var RN=(B.tf.Text.isPrototypeOf(A=this.B4.JY(this.
B4.JX(B.rZ(this.B4.H))))?A:null);if(!!RN){RN.Ab(this.IS);}},VC:function(Ae){var A;
var L=this.BS.Am;while(!!L){var Z=(B.tf.Text.isPrototypeOf(L)?L:null);if(!!Z){Z.
Ab(this.IT);}L=L.U;}var RP=(B.tf.Text.isPrototypeOf(A=this.BS.JY(this.BS.JX(B.rZ(
this.BS.H))))?A:null);if(!!RP){RP.Ab(this.IS);}},_Init:function(aArg){B.th.ES._Init.
call(this,aArg);B.Core.Im._Init.call(this.E0={N:this},0);B.Core.Im._Init.call(this.
EY={N:this},0);B.Core.Im._Init.call(this.EV={N:this},0);B.Core.HB._Init.call(this.
BS={N:this},0);B.Core.HB._Init.call(this.B4={N:this},0);B.Core.HB._Init.call(this.
BC={N:this},0);B.tf.Ck._Init.call(this.Ck={N:this},0);B.tf.Ck._Init.call(this.CV={
N:this},0);B.tf.Ck._Init.call(this.CW={N:this},0);B.tf.Ck._Init.call(this.CX={N:
this},0);B.tf.Ck._Init.call(this.CY={N:this},0);B.tf.Ck._Init.call(this.CZ={N:this
},0);this.__proto__=E.DW;this.Q(Ut);this.E0.Q(QV);this.E0.FS(CD);this.E0.J5(0.200000
);this.EY.Q(QW);this.EY.FS(CD);this.EY.J5(0.200000);this.EV.Q(QX);this.EV.FS(CD);
this.EV.J5(0.200000);this.BS.Q(QV);this.BS.J4(true);this.BS.BT(80);this.BS.FR(80
);this.BS.Hp(37);this.B4.Q(QW);this.B4.J4(true);this.B4.BT(80);this.B4.FR(80);this.
B4.Hp(12);this.BC.Q(QX);this.BC.J4(true);this.BC.BT(80);this.BC.FR(80);this.BC.Hp(
31);this.Ck.Cm(Uu);this.Ck.Cl(Uv);this.Ck.CB(3);this.CV.Cm(Uw);this.CV.Cl(MN);this.
CV.CB(3);this.CW.Cm(Ux);this.CW.Cl(Uy);this.CW.CB(3);this.CX.Cm(Uz);this.CX.Cl(UA
);this.CX.CB(3);this.CY.Cm(UB);this.CY.Cl(UC);this.CY.CB(3);this.CZ.Cm(UD);this.
CZ.Cl(UE);this.CZ.CB(3);this.Aa(this.E0,0);this.Aa(this.EY,0);this.Aa(this.EV,0);
this.Aa(this.BS,0);this.Aa(this.B4,0);this.Aa(this.BC,0);this.Aa(this.Ck,0);this.
Aa(this.CV,0);this.Aa(this.CW,0);this.Aa(this.CX,0);this.Aa(this.CY,0);this.Aa(this.
CZ,0);this.E0.Em=[this,this.JB];this.E0.Dm=[this,this.JE];this.EY.Em=[this,this.
JB];this.EY.Dm=[this,this.JE];this.EV.Em=[this,this.JB];this.EV.Dm=[this,this.JE
];this.BS.FU=[this,this.VC];this.BS.FN=[this,this.Tj];this.BS.J8(this.E0);this.B4.
FU=[this,this.Vu];this.B4.FN=[this,this.Th];this.B4.J8(this.EY);this.BC.FU=[this
,this.Vq];this.BC.FN=[this,this.Te];this.BC.J8(this.EV);},_Done:function(){this.
__proto__=E.DW;this.E0._Done();this.EY._Done();this.EV._Done();this.BS._Done();this.
B4._Done();this.BC._Done();this.Ck._Done();this.CV._Done();this.CW._Done();this.
CX._Done();this.CY._Done();this.CZ._Done();B.th.ES._Done.call(this);},_ReInit:function(
){B.th.ES._ReInit.call(this);this.E0._ReInit();this.EY._ReInit();this.EV._ReInit(
);this.BS._ReInit();this.B4._ReInit();this.BC._ReInit();this.Ck._ReInit();this.CV.
_ReInit();this.CW._ReInit();this.CX._ReInit();this.CY._ReInit();this.CZ._ReInit(
);},_Mark:function(D){var A;B.th.ES._Mark.call(this,D);if((A=this.Id)&&((A=A[0]).
_cycle!=D))A._Mark(A._cycle=D);if((A=this.E0)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.EY)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EV)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.BS)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B4)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.BC)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ck)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.CV)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CW
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CX)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.CY)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CZ)._cycle!=D)A._Mark(A._cycle=
D);},_className:"BezierClock::DatePicker"};E.Ca={DG:null,AH:null,Cu:null,C$:0,Bt:
B.sr(13,B.pm,null),GM:100,P0:0xFFFFFFFF,QR:14.000000,Dn:B.sr(13,B.pm,null),Do:B.
sr(13,B.pm,null),Dp:B.sr(13,B.pm,null),Dq:B.sr(13,B.pm,null),Dr:B.sr(13,B.pm,null
),Ds:B.sr(13,B.pm,null),Dt:B.sr(13,B.pm,null),Du:B.sr(13,B.pm,null),Dv:B.sr(13,B.
pm,null),Dw:B.sr(13,B.pm,null),Rr:0,PW:700,NW:true,M6:function(AA,aClip,aOffset,
Ah,aBlend){B.Core.K.M6.call(this,AA,aClip,aOffset,Ah,aBlend);if(this.NW){var Ix=
0xFFFF0000;var I=0;while(I<13){this.Vd(AA,aClip,aOffset,Ah,aBlend,this.Bt.Get(I)
);if(I>0)AA.L_(aClip,B.so(aOffset,this.Bt.Get(I-1)),B.so(aOffset,this.Bt.Get(I))
,Ix,Ix,aBlend);I=I+1;}}},G4:function(aSize){this.Cu.Q(B.sT(this.Cu.H,aSize));B.kD([
this,this.VT],this);B.kD([this,this.Ei],this);},CU:function(aArg){B.kD([this,this.
Ei],this);},DJ:function(C){var A;if(this.C$===C)return;this.Rr=this.C$%10;this.C$=
C;this.DG.OF(this);},FP:function(C){if(this.GM===C)return;this.GM=C;},Ab:function(
C){if(this.P0===C)return;this.P0=C;this.Cu.Ab(C);},FT:function(C){if(this.QR===C
)return;this.QR=C;this.Cu.CB(C);},Vd:function(AA,aClip,aOffset,Ah,aBlend,Ep){var
A;var JG=0xAA0000FF;var Pm=0x886060FF;var Ew;var Ex;aBlend=aBlend&&((this.F&0x2)===
0x2);Ah=Ah+1;if(Ah<256){JG=(JG&0x00FFFFFF)|((((Ah*((JG>>24)&0xFF))>>8)&0xFF)<<24
);Pm=(Pm&0x00FFFFFF)|((((Ah*((Pm>>24)&0xFF))>>8)&0xFF)<<24);}Ew=B.sn(B.so(Ep,aOffset
),QY);Ex=B.so(B.so(Ep,aOffset),QY);AA.L_(aClip,Ew,Ex,JG,JG,aBlend);Ew=B.sn(B.so(
Ep,aOffset),QZ);Ex=B.so(B.so(Ep,aOffset),QZ);AA.L_(aClip,Ew,Ex,JG,JG,aBlend);},FQ:
function(C){if(this.NW===C)return;this.NW=C;this.Ag(this.H);},VT:function(Ae){var
A;var size=[(A=this.H)[2]-A[0],A[3]-A[1]];var Jt=(size[0]/2)|0;var Kt=(size[1]/2
)|0;var Je=(size[0]/10)|0;var Jf=(size[1]/12)|0;this.Dn.Set(0,[Je,Kt]);this.Dn.Set(
1,[Je,(size[1]/4)|0]);this.Dn.Set(2,[(size[0]/4)|0,Jf]);this.Dn.Set(3,[Jt,Jf]);this.
Dn.Set(4,[size[0]-((size[0]/4)|0),Jf]);this.Dn.Set(5,[size[0]-Je,(size[1]/4)|0]);
this.Dn.Set(6,[size[0]-Je,Kt]);this.Dn.Set(7,[size[0]-Je,size[1]-((size[1]/4)|0)
]);this.Dn.Set(8,[size[0]-((size[0]/4)|0),size[1]-Jf]);this.Dn.Set(9,[Jt,size[1]-
Jf]);this.Dn.Set(10,[(size[0]/4)|0,size[1]-Jf]);this.Dn.Set(11,[Je,size[1]-((size[
1]/4)|0)]);this.Dn.Set(12,[Je,Kt-1]);this.Do.Set(0,[(size[0]/6)|0,((size[1]*3)/10
)|0]);this.Do.Set(1,[((size[0]*16)/60)|0,((size[1]*14)/60)|0]);this.Do.Set(2,[((
size[0]*22)/60)|0,((size[1]*10)/60)|0]);this.Do.Set(3,[Jt,Jf]);this.Do.Set(4,[Jt
,((size[1]*4)/12)|0]);this.Do.Set(5,[Jt,((size[1]*8)/12)|0]);this.Do.Set(6,[Jt,((
size[1]*11)/12)|0]);this.Do.Set(7,[Jt,((size[1]*11)/12)|0]);this.Do.Set(8,[((size[
0]*4)/6)|0,((size[1]*11)/12)|0]);this.Do.Set(9,[((size[0]*5)/6)|0,((size[1]*11)/
12)|0]);this.Do.Set(10,[((size[0]*4)/6)|0,((size[1]*11)/12)|0]);this.Do.Set(11,[((
size[0]*2)/6)|0,((size[1]*11)/12)|0]);this.Do.Set(12,[(size[0]/6)|0,((size[1]*11
)/12)|0]);this.Dp.Set(0,[(size[0]/6)|0,((size[1]*3)/10)|0]);this.Dp.Set(1,[(size[
0]/6)|0,0]);this.Dp.Set(2,[((size[0]*5)/6)|0,0]);this.Dp.Set(3,[((size[0]*5)/6)|
0,((size[1]*3)/10)|0]);this.Dp.Set(4,[((size[0]*5)/6)|0,((size[1]*7)/16)|0]);this.
Dp.Set(5,[((size[0]*39)/60)|0,((size[1]*5)/9)|0]);this.Dp.Set(6,[((size[0]*33)/60
)|0,((size[1]*5)/8)|0]);this.Dp.Set(7,[((size[0]*22)/60)|0,((size[1]*45)/60)|0]);
this.Dp.Set(8,[((size[0]*19)/60)|0,((size[1]*47)/60)|0]);this.Dp.Set(9,[(size[0]
/8)|0,((size[1]*11)/12)|0]);this.Dp.Set(10,[((size[0]*3)/8)|0,((size[1]*11)/12)|
0]);this.Dp.Set(11,[((size[0]*5)/8)|0,((size[1]*11)/12)|0]);this.Dp.Set(12,[((size[
0]*7)/8)|0,((size[1]*11)/12)|0]);this.Dq.Set(0,[(size[0]/6)|0,((size[1]*3)/10)|0
]);this.Dq.Set(1,[(size[0]/5)|0,0]);this.Dq.Set(2,[size[0]-((size[0]/7)|0),0]);this.
Dq.Set(3,[size[0]-((size[0]/6)|0),((size[1]*3)/10)|0]);this.Dq.Set(4,[size[0]-((
size[0]/5)|0),((size[1]*3)/7)|0]);this.Dq.Set(5,[((size[0]*4)/6)|0,Kt]);this.Dq.
Set(6,[((size[0]*9)/20)|0,Kt]);this.Dq.Set(7,[((size[0]*7)/10)|0,Kt]);this.Dq.Set(
8,[size[0]-(((size[0]*4)/30)|0),size[1]-(((size[1]*3)/7)|0)]);this.Dq.Set(9,[size[
0]-(((size[0]*3)/20)|0),size[1]-(((size[1]*3)/10)|0)]);this.Dq.Set(10,[size[0]-(((
size[0]*3)/20)|0),size[1]]);this.Dq.Set(11,[(size[0]/6)|0,size[1]]);this.Dq.Set(
12,[((size[0]*3)/20)|0,size[1]-(((size[1]*3)/10)|0)]);this.Dr.Set(0,[((size[0]*4
)/6)|0,((size[1]*19)/20)|0]);this.Dr.Set(1,[((size[0]*4)/6)|0,((size[1]*17)/20)|
0]);this.Dr.Set(2,[((size[0]*4)/6)|0,((size[1]*16)/20)|0]);this.Dr.Set(3,[((size[
0]*4)/6)|0,((size[1]*10)/20)|0]);this.Dr.Set(4,[((size[0]*4)/6)|0,((size[1]*7)/20
)|0]);this.Dr.Set(5,[((size[0]*4)/6)|0,((size[1]*1)/20)|0]);this.Dr.Set(6,[((size[
0]*4)/6)|0,((size[1]*1)/20)|0]);this.Dr.Set(7,[((size[0]*23)/60)|0,((size[1]*20)
/60)|0]);this.Dr.Set(8,[((size[0]*13)/60)|0,((size[1]*30)/60)|0]);this.Dr.Set(9,[(
size[0]/16)|0,((size[1]*6)/9)|0]);this.Dr.Set(10,[((size[0]*2)/6)|0,((size[1]*6)
/9)|0]);this.Dr.Set(11,[((size[0]*4)/6)|0,((size[1]*6)/9)|0]);this.Dr.Set(12,[((
size[0]*15)/16)|0,((size[1]*6)/9)|0]);this.Ds.Set(0,[((size[0]*3)/20)|0,((size[1
]*13)/15)|0]);this.Ds.Set(1,[((size[0]*5)/10)|0,((size[1]*59)/60)|0]);this.Ds.Set(
2,[((size[0]*5)/6)|0,((size[1]*14)/16)|0]);this.Ds.Set(3,[((size[0]*51)/60)|0,((
size[1]*10)/15)|0]);this.Ds.Set(4,[((size[0]*17)/20)|0,((size[1]*14)/30)|0]);this.
Ds.Set(5,[((size[0]*6)/10)|0,((size[1]*6)/16)|0]);this.Ds.Set(6,[((size[0]*3)/13
)|0,((size[1]*7)/16)|0]);this.Ds.Set(7,[((size[0]*15)/60)|0,((size[1]*5)/16)|0]);
this.Ds.Set(8,[((size[0]*16)/60)|0,((size[1]*7)/30)|0]);this.Ds.Set(9,[((size[0]
*3)/10)|0,(size[1]/12)|0]);this.Ds.Set(10,[((size[0]*5)/10)|0,(size[1]/12)|0]);this.
Ds.Set(11,[((size[0]*7)/10)|0,(size[1]/12)|0]);this.Ds.Set(12,[((size[0]*5)/6)|0
,(size[1]/12)|0]);this.Dt.Set(0,[((size[0]*10)/60)|0,((size[1]*18)/30)|0]);this.
Dt.Set(1,[((size[0]*5)/60)|0,((size[1]*24)/30)|0]);this.Dt.Set(2,[((size[0]*9)/30
)|0,((size[1]*29)/30)|0]);this.Dt.Set(3,[((size[0]*18)/30)|0,((size[1]*55)/60)|0
]);this.Dt.Set(4,[((size[0]*54)/60)|0,((size[1]*51)/60)|0]);this.Dt.Set(5,[((size[
0]*55)/60)|0,((size[1]*35)/60)|0]);this.Dt.Set(6,[((size[0]*43)/60)|0,((size[1]*
29)/60)|0]);this.Dt.Set(7,[((size[0]*6)/11)|0,((size[1]*8)/20)|0]);this.Dt.Set(8
,[((size[0]*3)/11)|0,((size[1]*9)/20)|0]);this.Dt.Set(9,[((size[0]*5)/30)|0,((size[
1]*12)/20)|0]);this.Dt.Set(10,[((size[0]*5)/30)|0,((size[1]*10)/20)|0]);this.Dt.
Set(11,[((size[0]*26)/60)|0,((size[1]*16)/60)|0]);this.Dt.Set(12,[((size[0]*20)/
30)|0,(size[1]/15)|0]);this.Du.Set(0,[((size[0]*17)/60)|0,((size[1]*28)/30)|0]);
this.Du.Set(1,[((size[0]*23)/60)|0,((size[1]*24)/30)|0]);this.Du.Set(2,[((size[0
]*29)/60)|0,((size[1]*20)/30)|0]);this.Du.Set(3,[((size[0]*18)/30)|0,(size[1]/2)|
0]);this.Du.Set(4,[((size[0]*40)/60)|0,((size[1]*12)/30)|0]);this.Du.Set(5,[((size[
0]*47)/60)|0,((size[1]*7)/30)|0]);this.Du.Set(6,[((size[0]*53)/60)|0,(size[1]/12
)|0]);this.Du.Set(7,[((size[0]*14)/20)|0,(size[1]/12)|0]);this.Du.Set(8,[((size[
0]*13)/20)|0,(size[1]/12)|0]);;this.Du.Set(9,[((size[0]*6)/20)|0,(size[1]/12)|0]
);;this.Du.Set(10,[((size[0]*5)/20)|0,(size[1]/12)|0]);this.Du.Set(11,[((size[0]
*5)/20)|0,(size[1]/12)|0]);this.Du.Set(12,[((size[0]*2)/20)|0,(size[1]/12)|0]);this.
Dv.Set(0,[((size[0]*59)/120)|0,((size[1]*55)/60)|0]);this.Dv.Set(1,[((size[0]*19
)/20)|0,((size[1]*55)/60)|0]);this.Dv.Set(2,[((size[0]*19)/20)|0,((size[1]*1)/2)|
0]);this.Dv.Set(3,[((size[0]*59)/120)|0,((size[1]*1)/2)|0]);this.Dv.Set(4,[((size[
0]*28)/30)|0,((size[1]*1)/2)|0]);this.Dv.Set(5,[((size[0]*28)/30)|0,((size[1]*5)
/60)|0]);this.Dv.Set(6,[((size[0]*4)/8)|0,((size[1]*5)/60)|0]);this.Dv.Set(7,[((
size[0]*2)/30)|0,((size[1]*5)/60)|0]);this.Dv.Set(8,[((size[0]*2)/30)|0,((size[1
]*1)/2)|0]);this.Dv.Set(9,[((size[0]*61)/120)|0,((size[1]*1)/2)|0]);this.Dv.Set(
10,[((size[0]*1)/20)|0,((size[1]*30)/60)|0]);this.Dv.Set(11,[((size[0]*1)/20)|0,((
size[1]*55)/60)|0]);this.Dv.Set(12,[((size[0]*61)/120)|0,((size[1]*55)/60)|0]);this.
Dw.Set(0,[((size[0]*22)/60)|0,((size[1]*56)/60)|0]);this.Dw.Set(1,[((size[0]*38)
/60)|0,((size[1]*40)/60)|0]);this.Dw.Set(2,[((size[0]*48)/60)|0,((size[1]*30)/60
)|0]);this.Dw.Set(3,[((size[0]*49)/60)|0,((size[1]*25)/60)|0]);this.Dw.Set(4,[((
size[0]*55)/60)|0,((size[1]*15)/60)|0]);this.Dw.Set(5,[((size[0]*45)/60)|0,((size[
1]*5)/60)|0]);this.Dw.Set(6,[((size[0]*30)/60)|0,((size[1]*5)/60)|0]);this.Dw.Set(
7,[((size[0]*18)/60)|0,((size[1]*5)/60)|0]);this.Dw.Set(8,[((size[0]*11)/60)|0,((
size[1]*12)/60)|0]);this.Dw.Set(9,[((size[0]*10)/60)|0,((size[1]*18)/60)|0]);this.
Dw.Set(10,[((size[0]*7)/60)|0,((size[1]*36)/60)|0]);this.Dw.Set(11,[((size[0]*42
)/60)|0,((size[1]*38)/60)|0]);this.Dw.Set(12,[((size[0]*49)/60)|0,((size[1]*25)/
60)|0]);},DI:function(C){if(this.PW===C)return;this.PW=C;this.DG.FO(C);},Ei:function(
Ae){var A;var Vc=this.C$%10;var A9=this.DG.C$;var CN=1.000000-this.DG.C$;var x1=
0.000000;var x2=0.000000;var y1=0.000000;var y2=0.000000;var I=0;while(I<13){switch(
this.Rr){case 0:{x1=this.Dn.Get(I)[0]*CN;y1=this.Dn.Get(I)[1]*CN;}break;case 1:{
x1=this.Do.Get(I)[0]*CN;y1=this.Do.Get(I)[1]*CN;}break;case 2:{x1=this.Dp.Get(I)[
0]*CN;y1=this.Dp.Get(I)[1]*CN;}break;case 3:{x1=this.Dq.Get(I)[0]*CN;y1=this.Dq.
Get(I)[1]*CN;}break;case 4:{x1=this.Dr.Get(I)[0]*CN;y1=this.Dr.Get(I)[1]*CN;}break;
case 5:{x1=this.Ds.Get(I)[0]*CN;y1=this.Ds.Get(I)[1]*CN;}break;case 6:{x1=this.Dt.
Get(I)[0]*CN;y1=this.Dt.Get(I)[1]*CN;}break;case 7:{x1=this.Du.Get(I)[0]*CN;y1=this.
Du.Get(I)[1]*CN;}break;case 8:{x1=this.Dv.Get(I)[0]*CN;y1=this.Dv.Get(I)[1]*CN;}
break;case 9:{x1=this.Dw.Get(I)[0]*CN;y1=this.Dw.Get(I)[1]*CN;}break;default:{x1=((((
A=this.H)[2]-A[0])/2)|0)*CN;y1=((((A=this.H)[3]-A[1])/2)|0)*CN;}}switch(Vc){case
0:{x2=this.Dn.Get(I)[0]*A9;y2=this.Dn.Get(I)[1]*A9;}break;case 1:{x2=this.Do.Get(
I)[0]*A9;y2=this.Do.Get(I)[1]*A9;}break;case 2:{x2=this.Dp.Get(I)[0]*A9;y2=this.
Dp.Get(I)[1]*A9;}break;case 3:{x2=this.Dq.Get(I)[0]*A9;y2=this.Dq.Get(I)[1]*A9;}
break;case 4:{x2=this.Dr.Get(I)[0]*A9;y2=this.Dr.Get(I)[1]*A9;}break;case 5:{x2=
this.Ds.Get(I)[0]*A9;y2=this.Ds.Get(I)[1]*A9;}break;case 6:{x2=this.Dt.Get(I)[0]
*A9;y2=this.Dt.Get(I)[1]*A9;}break;case 7:{x2=this.Du.Get(I)[0]*A9;y2=this.Du.Get(
I)[1]*A9;}break;case 8:{x2=this.Dv.Get(I)[0]*A9;y2=this.Dv.Get(I)[1]*A9;}break;case
9:{x2=this.Dw.Get(I)[0]*A9;y2=this.Dw.Get(I)[1]*A9;}break;default:;}this.Bt.Set(
I,[(x1+x2)|0,(y1+y2)|0]);I=I+1;}this.AH.BA(0,this.GM);this.AH.E9(0,this.Bt.Get(0
)[0],this.Bt.Get(0)[1]);this.AH.L1(0,this.Bt.Get(1)[0],this.Bt.Get(1)[1],this.Bt.
Get(2)[0],this.Bt.Get(2)[1],this.Bt.Get(3)[0],this.Bt.Get(3)[1],(this.GM/4)|0);this.
AH.L1(0,this.Bt.Get(4)[0],this.Bt.Get(4)[1],this.Bt.Get(5)[0],this.Bt.Get(5)[1],
this.Bt.Get(6)[0],this.Bt.Get(6)[1],(this.GM/4)|0);this.AH.L1(0,this.Bt.Get(7)[0
],this.Bt.Get(7)[1],this.Bt.Get(8)[0],this.Bt.Get(8)[1],this.Bt.Get(9)[0],this.Bt.
Get(9)[1],(this.GM/4)|0);this.AH.L1(0,this.Bt.Get(10)[0],this.Bt.Get(10)[1],this.
Bt.Get(11)[0],this.Bt.Get(11)[1],this.Bt.Get(12)[0],this.Bt.Get(12)[1],(this.GM/
4)|0);},_Init:function(aArg){B.Core.K._Init.call(this,aArg);B.tg.DG._Init.call(this.
DG={N:this},0);B.Graphics.AH._Init.call(this.AH={N:this},0);B.tf.Cu._Init.call(this.
Cu={N:this},0);(this.Bt=[]).__proto__=E.Ca.Bt;(this.Dn=[]).__proto__=E.Ca.Dn;(this.
Do=[]).__proto__=E.Ca.Do;(this.Dp=[]).__proto__=E.Ca.Dp;(this.Dq=[]).__proto__=E.
Ca.Dq;(this.Dr=[]).__proto__=E.Ca.Dr;(this.Ds=[]).__proto__=E.Ca.Ds;(this.Dt=[]).
__proto__=E.Ca.Dt;(this.Du=[]).__proto__=E.Ca.Du;(this.Dv=[]).__proto__=E.Ca.Dv;(
this.Dw=[]).__proto__=E.Ca.Dw;this.__proto__=E.Ca;this.Q(UF);this.DG.K$(1);this.
DG.FO(700);this.DG.Bs(true);this.Cu.CB(14.000000);this.Aa(this.Cu,0);this.DG.IY=[
this,this.Ei];this.Cu.EB(this.AH);this.CU(aArg);},_Done:function(){this.__proto__=
E.Ca;this.DG._Done();this.AH._Done();this.Cu._Done();B.Core.K._Done.call(this);}
,_ReInit:function(){B.Core.K._ReInit.call(this);this.DG._ReInit();this.AH._ReInit(
);this.Cu._ReInit();},_Mark:function(D){var A;B.Core.K._Mark.call(this,D);if((A=
this.DG)._cycle!=D)A._Mark(A._cycle=D);if((A=this.AH)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Cu)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::Digit"
};E.CO={Cz:null,B1:null,EN:null,EO:null,G4:function(aSize){B.Core.K.G4.call(this
,aSize);this.Cz.J6(((aSize[0]*10)/23)|0);this.Cz.J7(((aSize[1]*10)/23)|0);this.B1.
Q(B.sT(this.B1.H,aSize));this.B1.En([(aSize[0]/2)|0,this.B1.As[1]]);this.B1.En([
this.B1.As[0],(aSize[1]/2)|0]);this.EN.Q(B.sT(this.EN.H,aSize));this.EN.En([((aSize[
0]*10)/19)|0,this.EN.As[1]]);this.EN.En([this.EN.As[0],((aSize[1]*10)/19)|0]);this.
EO.Q(B.sT(this.EO.H,aSize));this.EO.En([((aSize[0]*10)/18)|0,this.EO.As[1]]);this.
EO.En([this.EO.As[0],((aSize[1]*10)/21)|0]);},_Init:function(aArg){B.Core.K._Init.
call(this,aArg);B.Graphics.Cz._Init.call(this.Cz={N:this},0);B.tf.B1._Init.call(
this.B1={N:this},0);B.tf.B1._Init.call(this.EN={N:this},0);B.tf.B1._Init.call(this.
EO={N:this},0);this.__proto__=E.CO;this.Q(UG);this.K9(true);this.Cz.Ct(360.000000
);this.Cz.Ot(90.000000);this.Cz.Ih(2);this.B1.Q(OY);this.B1.Ab(0x22FFFFFF);this.
EN.Q(OY);this.EN.Ab(0x22FFFFFF);this.EO.Q(OY);this.EO.Ab(0x22FFFFFF);this.Aa(this.
B1,0);this.Aa(this.EN,0);this.Aa(this.EO,0);this.B1.EB(this.Cz);this.EN.EB(this.
Cz);this.EO.EB(this.Cz);},_Done:function(){this.__proto__=E.CO;this.Cz._Done();this.
B1._Done();this.EN._Done();this.EO._Done();B.Core.K._Done.call(this);},_ReInit:function(
){B.Core.K._ReInit.call(this);this.Cz._ReInit();this.B1._ReInit();this.EN._ReInit(
);this.EO._ReInit();},_Mark:function(D){var A;B.Core.K._Mark.call(this,D);if((A=
this.Cz)._cycle!=D)A._Mark(A._cycle=D);if((A=this.B1)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.EN)._cycle!=D)A._Mark(A._cycle=D);if((A=this.EO)._cycle!=D)A._Mark(
A._cycle=D);},_className:"BezierClock::Bubble"};E.Ay={Gb:null,FG:null,Ff:null,Ay:
null,CO:null,ET:null,FH:0,G4:function(aSize){var A;B.Core.K.G4.call(this,aSize);
var RH=(((A=this.H)[3]-A[1])/4)|0;this.CO.Q(B.sT(this.CO.H,[RH,RH]));},DM:function(
B7){B.Core.K.DM.call(this,B7);this.CO.Hq(this.FH>0);this.Ay.Dl(null);B.r6([this,
this.VL],this);},Nl:function(Ae){var A;var RB=(A=this.CO.H)[2]-A[0];var RC=(A=this.
CO.H)[3]-A[1];var IM=B.so([(((A=this.H)[2]-A[0])/3)|0,(((A=this.H)[3]-A[1])/2)|0
],this.Ay.AU);this.CO.Q(B.sV(this.CO.H,IM[0]+(((this.Gb.C$*RB)+(this.FG.C$*RB))|
0)));this.CO.Q(B.sX(this.CO.H,IM[1]+(((this.Gb.C$*RC)-(this.FG.C$*RC))|0)));},VL:
function(Ae){if(this.FH===1)this.Ay.Dl(B.r0(E.Ow));else if(this.FH===2)this.Ay.Dl(
B.r0(E.Blue));else this.Ay.Dl(B.r0(E.L8));},GP:function(C){if(C<0)C=0;if(C>2)C=2;
if(this.FH===C)return;this.FH=C;B.sx([this,this.IZ,this.GP],0);this.Bl();},ShiftLeft:
function(){var A;this.ET.BJ=this.Ay.AU;this.ET.BU=[((A=this.H)[2]-A[0])-this.Ay.
An.FrameSize[0],0];B.kD([A=this.ET,A.OF],this);},ShiftRight:function(){var A;this.
ET.BJ=this.Ay.AU;this.ET.BU=OZ;B.kD([A=this.ET,A.OF],this);},IZ:function(){return this.
FH;},_Init:function(aArg){B.Core.K._Init.call(this,aArg);B.tg.DG._Init.call(this.
Gb={N:this},0);B.tg.DG._Init.call(this.FG={N:this},0);B.tf.Ff._Init.call(this.Ff={
N:this},0);B.tf.FK._Init.call(this.Ay={N:this},0);E.CO._Init.call(this.CO={N:this
},0);B.tg.MA._Init.call(this.ET={N:this},0);this.__proto__=E.Ay;var A;this.Q(Gq);
this.Gb.Kd=true;this.Gb.J9(24);this.Gb.FO(18000);this.Gb.Bs(true);this.FG.Kd=true;
this.FG.J9(24);this.FG.FO(40000);this.FG.Bs(true);this.FG.BJ=-1.000000;this.Ff.C9(
0x3F);this.Ff.Q(Gq);this.Ff.Ab(0xFF000000);this.Ay.C9(0x3F);this.Ay.Q(Gq);this.Ay.
ER(0x11);this.CO.C9(0x3);this.CO.Q(UH);this.ET.J9(23);this.ET.K$(1);this.ET.FO(500
);this.Aa(this.Ff,0);this.Aa(this.Ay,0);this.Aa(this.CO,0);this.Gb.IY=[this,this.
Nl];this.FG.IY=[this,this.Nl];this.Ay.Dl(B.r0(E.L8));this.ET.Av=[A=this.Ay,A.Tc,
A.BT];},_Done:function(){this.__proto__=E.Ay;this.Gb._Done();this.FG._Done();this.
Ff._Done();this.Ay._Done();this.CO._Done();this.ET._Done();B.Core.K._Done.call(this
);},_ReInit:function(){B.Core.K._ReInit.call(this);this.Gb._ReInit();this.FG._ReInit(
);this.Ff._ReInit();this.Ay._ReInit();this.CO._ReInit();this.ET._ReInit();},_Mark:
function(D){var A;B.Core.K._Mark.call(this,D);if((A=this.Gb)._cycle!=D)A._Mark(A.
_cycle=D);if((A=this.FG)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Ff)._cycle!=D)
A._Mark(A._cycle=D);if((A=this.Ay)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CO).
_cycle!=D)A._Mark(A._cycle=D);if((A=this.ET)._cycle!=D)A._Mark(A._cycle=D);},_className:
"BezierClock::Background"};E.Lk={BB:null,CR:null,CP:null,CQ:null,BW:null,BX:null
,BY:null,BZ:null,BP:null,BQ:null,Dx:null,G0:null,H8:null,GY:0,FF:false,DM:function(
B7){B.Core.K.DM.call(this,B7);var B8=0xFFFFFFFF;if(!this.FF&&(this.GY>0))B8=0x99FFFFFF;
this.BW.Ab(B8);this.BX.Ab(B8);this.BY.Ab(B8);this.BZ.Ab(B8);this.BP.Ab(B8);this.
BQ.Ab(B8);this.CP.Ab(B8);this.CQ.Ab(B8);},Pv:function(Ae){var A;if(!this.Hn(false
))return;this.G.EU(B._NewObject(E.Gc,0),B._GetAutoObject(B.tg.Kb),null,null,null
,null,null,null,null,null,false);(E.JP.isPrototypeOf(A=this.G)?A:null).Ay.ShiftRight(
);},K8:function(C){if(this.FF===C)return;this.FF=C;if(C){this.BB.BJ=this.GY;this.
BB.FO(this.GY*10);this.BB.Bs(this.GY>0);}else this.BB.Bs(false);this.Bl();},Ou:function(
C){if(this.GY===C)return;this.GY=C;var Rp=(this.GY/6000)|0;var RJ=((this.GY/100)|
0)%60;var RT=this.GY;this.BW.DJ((Rp/10)|0);this.BX.DJ(Rp%10);this.BY.DJ((RJ/10)|
0);this.BZ.DJ(RJ%10);this.BP.DJ((RT/10)|0);this.BQ.DJ(RT%10);B.sx([this,this.Op,
this.Ou],0);this.Bl();},S7:function(){return this.FF;},Op:function(){return this.
GY;},_Init:function(aArg){B.Core.K._Init.call(this,aArg);B.tg.BB._Init.call(this.
BB={N:this},0);B.Graphics.Cz._Init.call(this.CR={N:this},0);B.tf.B1._Init.call(this.
CP={N:this},0);B.tf.B1._Init.call(this.CQ={N:this},0);E.Ca._Init.call(this.BW={N:
this},0);E.Ca._Init.call(this.BX={N:this},0);E.Ca._Init.call(this.BY={N:this},0);
E.Ca._Init.call(this.BZ={N:this},0);E.Ca._Init.call(this.BP={N:this},0);E.Ca._Init.
call(this.BQ={N:this},0);E.MJ._Init.call(this.Dx={N:this},0);B.Core.GW._Init.call(
this.G0={N:this},0);B.tf.FK._Init.call(this.H8={N:this},0);this.__proto__=E.Lk;this.
Q(Gq);this.BB.K$(1);this.BB.FO(100);this.BB.BU=0;this.CR.QD(20);this.CR.Ct(360.000000
);this.CR.Ot(6.100000);this.CR.Ih(2);this.CP.Q(UI);this.CP.Ab(0xDDFFFFFF);this.CP.
En(MO);this.CQ.Q(UJ);this.CQ.Ab(0xDDFFFFFF);this.CQ.En(MO);this.BW.Q(UK);this.BW.
DJ(0);this.BW.FP(60);this.BW.FT(6.100000);this.BW.FQ(false);this.BW.DI(700);this.
BX.Q(UL);this.BX.DJ(0);this.BX.FP(60);this.BX.FT(6.100000);this.BX.FQ(false);this.
BX.DI(700);this.BY.Q(UM);this.BY.DJ(0);this.BY.FP(60);this.BY.FT(6.100000);this.
BY.FQ(false);this.BY.DI(700);this.BZ.Q(UN);this.BZ.DJ(0);this.BZ.FP(60);this.BZ.
FT(6.100000);this.BZ.FQ(false);this.BZ.DI(700);this.BP.Q(UO);this.BP.DJ(0);this.
BP.FP(40);this.BP.FT(2.300000);this.BP.FQ(false);this.BP.DI(10);this.BQ.Q(UP);this.
BQ.DJ(0);this.BQ.FP(40);this.BQ.FT(2.300000);this.BQ.FQ(false);this.BQ.DI(10);this.
Dx.Q(UQ);this.Dx.TD(600);this.Dx.TF(100);this.G0.GS(OZ);this.G0.GR(OX);this.G0.Cm(
Q0);this.G0.Cl(UR);this.H8.Q(US);this.H8.Fe(3);this.Aa(this.CP,0);this.Aa(this.CQ
,0);this.Aa(this.BW,0);this.Aa(this.BX,0);this.Aa(this.BY,0);this.Aa(this.BZ,0);
this.Aa(this.BP,0);this.Aa(this.BQ,0);this.Aa(this.Dx,0);this.Aa(this.G0,0);this.
Aa(this.H8,0);this.BB.Av=[this,this.Op,this.Ou];this.CP.EB(this.CR);this.CQ.EB(this.
CR);this.Dx.I1([this,this.Op,this.Ou]);this.Dx.TE([this,this.S7,this.K8]);this.G0.
CA=[this,this.Pv];this.H8.Dl(B.r0(B.te.Ib));},_Done:function(){this.__proto__=E.
Lk;this.BB._Done();this.CR._Done();this.CP._Done();this.CQ._Done();this.BW._Done(
);this.BX._Done();this.BY._Done();this.BZ._Done();this.BP._Done();this.BQ._Done(
);this.Dx._Done();this.G0._Done();this.H8._Done();B.Core.K._Done.call(this);},_ReInit:
function(){B.Core.K._ReInit.call(this);this.BB._ReInit();this.CR._ReInit();this.
CP._ReInit();this.CQ._ReInit();this.BW._ReInit();this.BX._ReInit();this.BY._ReInit(
);this.BZ._ReInit();this.BP._ReInit();this.BQ._ReInit();this.Dx._ReInit();this.G0.
_ReInit();this.H8._ReInit();},_Mark:function(D){var A;B.Core.K._Mark.call(this,D
);if((A=this.BB)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CR)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.CP)._cycle!=D)A._Mark(A._cycle=D);if((A=this.CQ)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.BW)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BX
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BY)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.BZ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.BP)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.BQ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Dx)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.G0)._cycle!=D)A._Mark(A._cycle=D);if((A=this.H8)._cycle!=
D)A._Mark(A._cycle=D);},_className:"BezierClock::TimerScreen"};E.MJ={Qn:null,Id:
null,Av:null,Hs:null,HC:null,CK:null,D6:null,E1:null,E2:null,Ci:null,FW:null,Da:
null,Gn:null,Go:null,Gp:null,A7:null,Ap:null,GJ:0,DH:360,D2:0,NF:300,Mz:1,FF:false
,Lu:false,Lz:false,LZ:false,VF:false,G4:function(aSize){var KE=(aSize[0]/30)|0;this.
Da.Q([(aSize[0]/2)|0,0,aSize[0],(aSize[1]/2)|0]);this.Da.En([0,(aSize[1]/2)|0]);
this.Da.CB(KE);this.CK.J6(((aSize[0]-((KE+3.000000)|0))/2)|0);this.CK.J7(((aSize[
1]-((KE+3.000000)|0))/2)|0);this.Gn.Q([(aSize[0]/2)|0,(aSize[1]/2)|0,aSize[0],aSize[
1]]);this.Gn.En(OZ);this.Gn.CB(KE);this.D6.J6(this.CK.A5);this.D6.J7(this.CK.AX);
this.Go.Q([0,(aSize[1]/2)|0,(aSize[0]/2)|0,aSize[1]]);this.Go.En([(aSize[0]/2)|0
,0]);this.Go.CB(KE);this.E1.J6(this.CK.A5);this.E1.J7(this.CK.AX);this.Gp.Q([0,0
,(aSize[0]/2)|0,(aSize[1]/2)|0]);this.Gp.En([(aSize[0]/2)|0,(aSize[1]/2)|0]);this.
Gp.CB(KE);this.E2.J6(this.CK.A5);this.E2.J7(this.CK.AX);this.FW.Q(B.sT(this.FW.H
,aSize));this.FW.En([(aSize[0]/2)|0,(aSize[1]/2)|0]);this.HC.J6(this.CK.A5-(this.
Da.Width/1.800000));this.HC.J7(this.CK.AX-(this.Da.Width/1.800000));this.A7.Q([(
aSize[0]/5)|0,(aSize[1]/5)|0,((aSize[0]*4)/5)|0,((aSize[1]*4)/5)|0]);this.Ap.Q(B.
sT(this.Ap.H,aSize));B.kD([this,this.RV],this);},DM:function(B7){var A;B.th.Dx.DM.
call(this,B7);var De=((B7&0x10)===0x10);var Kv=((B7&0x20)===0x20);var Rn=this.Ap.
Down&&!this.Lu;if(!De){this.Da.Ab(0xCCCCCCCC);this.A7.Ab(0x00CCCCCC);}else if(Rn
){this.Da.Ab(0xFFFFFFFF);this.A7.Ab(0x00FFFFFF);}else if(Kv){this.Da.Ab(0xCCFFFFFF
);this.A7.Ab(0xBBFFFFFF);}else{this.Da.Ab(0xFFFFFFFF);if(this.FF)this.A7.Ab(0x33FFFFFF
);else if(this.Ie()>0)this.A7.Ab(0xFFFFFFFF);else this.A7.Ab(0x00FFFFFF);}this.Gn.
Ab(this.Da.J0());this.Go.Ab(this.Da.J0());this.Gp.Ab(this.Da.J0());this.FW.Ab(this.
Da.J0());var LI=0.000000;var Pr=360.000000;var IC=LI;if(this.DH!==this.D2)IC=(((
this.Ie()-this.D2)*(Pr-LI))/(this.DH-this.D2))+LI;this.CK.I2(-90.000000);this.D6.
I2(0.000000);this.E1.I2(90.000000);this.E2.I2(180.000000);if(IC<=90.000000){this.
CK.Ct(IC-90.000000);this.D6.Ct(0.000000);this.E1.Ct(90.000000);this.E2.Ct(180.000000
);}else if(IC<=180.000000){this.CK.Ct(90.099998);this.D6.I2(-0.100000);this.D6.Ct(
IC-90.000000);this.E1.Ct(90.000000);this.E2.Ct(180.000000);}else if(IC<=270.000000
){this.CK.Ct(90.099998);this.D6.I2(-0.100000);this.D6.Ct(180.000000);this.E1.Ct(
IC-90.000000);this.E2.Ct(180.000000);}else{this.CK.Ct(90.099998);this.D6.I2(-0.100000
);this.D6.Ct(180.100006);this.E1.Ct(270.100006);this.E2.Ct(IC-89.900002);}this.Lz=
De;this.LZ=Kv;this.VF=Rn;},JC:function(Ae){var A;if(!!this.Av)this.Qw(((A=this.Av
,A[1].call(A[0]))/this.Mz)|0);},Vv:function(Ae){var A;if(this.Lu||this.FF)return;
var LI=0;var Pr=360;var Nk=this.Ie();var He=Nk;He=(((-this.Ap.MB*(this.DH-this.D2
))/(Pr-LI))|0)+this.GJ;if((He>this.DH)&&(this.DH>this.D2))this.GJ=this.GJ-(He-this.
DH);if((He>this.D2)&&(this.D2>this.DH))this.GJ=this.GJ-(He-this.D2);if((He<this.
D2)&&(this.DH>this.D2))this.GJ=this.GJ-(He-this.D2);if((He<this.DH)&&(this.D2>this.
DH))this.GJ=this.GJ-(He-this.DH);this.Qw(He);if(Nk===this.Ie())return;B.kD(this.
Id,this);if(!!this.Av){(A=this.Av,A[2].call(A[0],this.Ie()*this.Mz));B.sx(this.Av
,0);}},LQ:function(Ae){var A;this.Bl();if(this.Ie()!==this.GJ)B.kD(this.Qn,this);
if(this.Lu){if(!!this.Hs)(A=this.Hs,A[2].call(A[0],this.FF));}},LP:function(Ae){
this.Bl();this.Lu=B.se(this.A7.H,this.Ap.Di);if(this.Lu){if(this.FF)this.K8(false
);else if(this.Ie()>0)this.K8(true);}this.GJ=this.Ie();},I1:function(C){if(B.sc(
this.Av,C))return;if(!!this.Av)B.rC([this,this.JC],this.Av,0);this.Av=C;if(!!C)B.
rn([this,this.JC],C,0);if(!!C)B.kD([this,this.JC],this);},TD:function(C){if(this.
DH===C)return;this.DH=C;this.Bl();},Ie:function(){var C=this.NF;if(this.D2>this.
DH){if(C<this.DH)C=this.DH;if(C>this.D2)C=this.D2;}else{if(C<this.D2)C=this.D2;if(
C>this.DH)C=this.DH;}return C;},Qw:function(C){if(this.NF===C)return;this.NF=C;this.
Bl();},TF:function(C){if(this.Mz===C)return;if(!C)C=1;this.Mz=C;this.Bl();},RV:function(
Ae){var A;if(this.FF){this.Ci.ME(2);this.Ci.BA(0,4);this.Ci.E9(0,((((A=this.A7.H
)[2]-A[0])*2)/10)|0,((((A=this.A7.H)[3]-A[1])*1)/10)|0);this.Ci.H5(0,((((A=this.
A7.H)[2]-A[0])*4)/10)|0,((((A=this.A7.H)[3]-A[1])*1)/10)|0);this.Ci.H5(0,((((A=this.
A7.H)[2]-A[0])*4)/10)|0,((((A=this.A7.H)[3]-A[1])*9)/10)|0);this.Ci.H5(0,((((A=this.
A7.H)[2]-A[0])*2)/10)|0,((((A=this.A7.H)[3]-A[1])*9)/10)|0);this.Ci.EA(0);this.Ci.
BA(1,4);this.Ci.E9(1,((((A=this.A7.H)[2]-A[0])*6)/10)|0,((((A=this.A7.H)[3]-A[1]
)*1)/10)|0);this.Ci.H5(1,((((A=this.A7.H)[2]-A[0])*8)/10)|0,((((A=this.A7.H)[3]-
A[1])*1)/10)|0);this.Ci.H5(1,((((A=this.A7.H)[2]-A[0])*8)/10)|0,((((A=this.A7.H)[
3]-A[1])*9)/10)|0);this.Ci.H5(1,((((A=this.A7.H)[2]-A[0])*6)/10)|0,((((A=this.A7.
H)[3]-A[1])*9)/10)|0);this.Ci.EA(1);}else{this.Ci.ME(2);this.Ci.BA(0,3);this.Ci.
E9(0,((((A=this.A7.H)[2]-A[0])*3)/10)|0,((((A=this.A7.H)[3]-A[1])*2)/10)|0);this.
Ci.H5(0,((((A=this.A7.H)[2]-A[0])*17)/20)|0,((((A=this.A7.H)[3]-A[1])*5)/10)|0);
this.Ci.H5(0,((((A=this.A7.H)[2]-A[0])*3)/10)|0,((((A=this.A7.H)[3]-A[1])*8)/10)|
0);this.Ci.EA(0);this.Ci.BA(1,0);}},TE:function(C){if(B.sc(this.Hs,C))return;if(
!!this.Hs)B.rC([this,this.PA],this.Hs,0);this.Hs=C;if(!!C)B.rn([this,this.PA],C,
0);if(!!C)B.kD([this,this.PA],this);},PA:function(Ae){var A;if(!!this.Hs)this.K8((
A=this.Hs,A[1].call(A[0])));},K8:function(C){if(this.FF===C)return;this.FF=C;B.kD([
this,this.RV],this);this.Bl();},_Init:function(aArg){B.th.Dx._Init.call(this,aArg
);B.Graphics.Cz._Init.call(this.HC={N:this},0);B.Graphics.Cz._Init.call(this.CK={
N:this},0);B.Graphics.Cz._Init.call(this.D6={N:this},0);B.Graphics.Cz._Init.call(
this.E1={N:this},0);B.Graphics.Cz._Init.call(this.E2={N:this},0);B.Graphics.AH._Init.
call(this.Ci={N:this},0);B.tf.Cu._Init.call(this.FW={N:this},0);B.tf.Cu._Init.call(
this.Da={N:this},0);B.tf.Cu._Init.call(this.Gn={N:this},0);B.tf.Cu._Init.call(this.
Go={N:this},0);B.tf.Cu._Init.call(this.Gp={N:this},0);B.tf.Cu._Init.call(this.A7={
N:this},0);B.Core.OB._Init.call(this.Ap={N:this},0);this.__proto__=E.MJ;this.Q(Ir
);this.HC.Ct(360.000000);this.HC.Ih(0);this.CK.Ct(90.000000);this.CK.Ih(0);this.
D6.Ct(90.000000);this.D6.Ih(0);this.E1.Ct(90.000000);this.E1.Ih(0);this.E2.Ct(90.000000
);this.E2.Ih(0);this.FW.Q(Ir);this.FW.CB(3.000000);this.Da.Q(Ir);this.Gn.Q(Ir);this.
Go.Q(Ir);this.Gp.Q(Ir);this.A7.Q(Ir);this.A7.Tx(1);this.A7.CB(15.000000);this.Ap.
Q(Ir);this.Aa(this.FW,0);this.Aa(this.Da,0);this.Aa(this.Gn,0);this.Aa(this.Go,0
);this.Aa(this.Gp,0);this.Aa(this.A7,0);this.Aa(this.Ap,0);this.FW.EB(this.HC);this.
Da.EB(this.CK);this.Gn.EB(this.D6);this.Go.EB(this.E1);this.Gp.EB(this.E2);this.
A7.EB(this.Ci);this.Ap.Oq=[this,this.Vv];this.Ap.Em=[this,this.LQ];this.Ap.Dm=[this
,this.LP];},_Done:function(){this.__proto__=E.MJ;this.HC._Done();this.CK._Done();
this.D6._Done();this.E1._Done();this.E2._Done();this.Ci._Done();this.FW._Done();
this.Da._Done();this.Gn._Done();this.Go._Done();this.Gp._Done();this.A7._Done();
this.Ap._Done();B.th.Dx._Done.call(this);},_ReInit:function(){B.th.Dx._ReInit.call(
this);this.HC._ReInit();this.CK._ReInit();this.D6._ReInit();this.E1._ReInit();this.
E2._ReInit();this.Ci._ReInit();this.FW._ReInit();this.Da._ReInit();this.Gn._ReInit(
);this.Go._ReInit();this.Gp._ReInit();this.A7._ReInit();this.Ap._ReInit();},_Mark:
function(D){var A;B.th.Dx._Mark.call(this,D);if((A=this.Qn)&&((A=A[0])._cycle!=D
))A._Mark(A._cycle=D);if((A=this.Id)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((
A=this.Av)&&((A=A[0])._cycle!=D))A._Mark(A._cycle=D);if((A=this.Hs)&&((A=A[0])._cycle
!=D))A._Mark(A._cycle=D);if((A=this.HC)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
CK)._cycle!=D)A._Mark(A._cycle=D);if((A=this.D6)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.E1)._cycle!=D)A._Mark(A._cycle=D);if((A=this.E2)._cycle!=D)A._Mark(A._cycle=
D);if((A=this.Ci)._cycle!=D)A._Mark(A._cycle=D);if((A=this.FW)._cycle!=D)A._Mark(
A._cycle=D);if((A=this.Da)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Gn)._cycle!=
D)A._Mark(A._cycle=D);if((A=this.Go)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Gp
)._cycle!=D)A._Mark(A._cycle=D);if((A=this.A7)._cycle!=D)A._Mark(A._cycle=D);if((
A=this.Ap)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::TimerWheel"};
E.Lj={GL:null,DZ:null,CJ:null,Gj:null,Gk:null,Gi:null,Pv:function(Ae){if(this.Hn(
false))this.G.EU(B._NewObject(E.Gc,0),B._GetAutoObject(B.tg.GX),null,null,null,null
,null,null,null,null,false);},_Init:function(aArg){B.Core.K._Init.call(this,aArg
);B.Core.GW._Init.call(this.GL={N:this},0);B.tf.FK._Init.call(this.DZ={N:this},0
);B.ti.CJ._Init.call(this.CJ={N:this},0);B.ti.J_._Init.call(this.Gj={N:this},0);
B.ti.J_._Init.call(this.Gk={N:this},0);B.ti.J_._Init.call(this.Gi={N:this},0);this.
__proto__=E.Lj;var A;this.Q(Gq);this.GL.GS(UT);this.GL.GR(UU);this.GL.Cm(UV);this.
GL.Cl(Q0);this.DZ.Q(QU);this.DZ.Fe(0);this.CJ.Tk(0x3F);this.CJ.To(0xFF404040);this.
CJ.Tr(0xFF404040);this.CJ.Tq(0xFF404040);this.CJ.Tp(0xFF404040);this.Gj.Q(UW);this.
Gj.Ig(true);this.Gj.Os(0);this.Gk.Q(UX);this.Gk.QF(1);this.Gk.Os(1);this.Gi.Q(UY
);this.Gi.QF(2);this.Gi.Os(2);this.Aa(this.GL,0);this.Aa(this.DZ,0);this.Aa(this.
Gj,0);this.Aa(this.Gk,0);this.Aa(this.Gi,0);this.GL.CA=[this,this.Pv];this.DZ.Dl(
B.r0(B.te.Ib));this.CJ.Ts(B.r0(B.te.In));this.CJ.Tu(B.r0(B.te.In));this.CJ.Tt(B.
r0(B.te.In));this.CJ.Tl(B.r0(B.te.In));this.CJ.Tn(B.r0(B.te.In));this.CJ.Tm(B.r0(
B.te.In));this.Gj.I1([A=B._GetAutoObject(E.Device),A.IZ,A.GP]);this.Gj.Or(B.r0(E.
KH));this.Gj.J3(this.CJ);this.Gk.I1([A=B._GetAutoObject(E.Device),A.IZ,A.GP]);this.
Gk.Or(B.r0(E.KH));this.Gk.J3(this.CJ);this.Gi.I1([A=B._GetAutoObject(E.Device),A.
IZ,A.GP]);this.Gi.Or(B.r0(E.KH));this.Gi.J3(this.CJ);},_Done:function(){this.__proto__=
E.Lj;this.GL._Done();this.DZ._Done();this.CJ._Done();this.Gj._Done();this.Gk._Done(
);this.Gi._Done();B.Core.K._Done.call(this);},_ReInit:function(){B.Core.K._ReInit.
call(this);this.GL._ReInit();this.DZ._ReInit();this.CJ._ReInit();this.Gj._ReInit(
);this.Gk._ReInit();this.Gi._ReInit();},_Mark:function(D){var A;B.Core.K._Mark.call(
this,D);if((A=this.GL)._cycle!=D)A._Mark(A._cycle=D);if((A=this.DZ)._cycle!=D)A.
_Mark(A._cycle=D);if((A=this.CJ)._cycle!=D)A._Mark(A._cycle=D);if((A=this.Gj)._cycle
!=D)A._Mark(A._cycle=D);if((A=this.Gk)._cycle!=D)A._Mark(A._cycle=D);if((A=this.
Gi)._cycle!=D)A._Mark(A._cycle=D);},_className:"BezierClock::StyleScreen"};E.NV={
_class:function(){return B.te.BR;},0:{Data:function(){return B.tk;},Cache:[],_this:
null}};E.Gh={_class:function(){return B.te.BR;},0:{Data:function(){return B.tl;}
,Cache:[],_this:null}};E.NG={FH:1,TS:function(Q7){},GP:function(C){if(this.FH===
C)return;this.FH=C;B.sx([this,this.IZ,this.GP],0);},IZ:function(){return this.FH;
},_Init:function(aArg){this.__proto__=E.NG;B.gb++;},_Done:function(){this.__proto__=
E.NG;B.gb--;},_ReInit:function(){},_Mark:function(D){var A;if((A=this.N)&&(A._cycle
!=D))A._Mark(A._cycle=D);},N:null,_cycle:0,_observers:null,_className:"BezierClock::DeviceClass"
};E.Device={_Init:function(){E.NG._Init.call(this,0);},_variants:function(){return this;
},_this:null};E.KH={_class:function(){return B.te.An;},0:{FileName:"./res/BezierClockBackgroundPreview.png"
,Format:B.b9,NoOfFrames:3,FrameSize:[120,60],FrameDelay:0,_this:null}};E.Blue={_class:
function(){return B.te.An;},0:{FileName:"./res/BezierClockBlue.png",Format:B.b9,
NoOfFrames:1,FrameSize:[510,272],FrameDelay:0,_this:null}};E.L8={_class:function(
){return B.te.An;},0:{FileName:"./res/BezierClockDark.png",Format:B.b9,NoOfFrames:
1,FrameSize:[510,272],FrameDelay:0,_this:null}};E.Ow={_class:function(){return B.
te.An;},0:{FileName:"./res/BezierClockOrange.png",Format:B.b9,NoOfFrames:1,FrameSize:[
510,272],FrameDelay:0,_this:null}};
E._Init=function(){E.D5.__proto__=B.th.ES;E.Gc.__proto__=B.Core.K;E.JP.__proto__=
B.Core.K;E.Li.__proto__=B.Core.K;E.DW.__proto__=B.th.ES;E.Ca.__proto__=B.Core.K;
E.CO.__proto__=B.Core.K;E.Ay.__proto__=B.Core.K;E.Lk.__proto__=B.Core.K;E.MJ.__proto__=
B.th.Dx;E.Lj.__proto__=B.Core.K;};E.Dy=function(D){var A;if((A=E.NV[0]._this)&&(
A._cycle!=D))A._Done(E.NV[0]._this=null);if((A=E.Gh[0]._this)&&(A._cycle!=D))A._Done(
E.Gh[0]._this=null);if((A=E.Device._this)&&(A._cycle!=D))A._Done(E.Device._this=
null);if((A=E.KH[0]._this)&&(A._cycle!=D))A._Done(E.KH[0]._this=null);if((A=E.Blue[
0]._this)&&(A._cycle!=D))A._Done(E.Blue[0]._this=null);if((A=E.L8[0]._this)&&(A.
_cycle!=D))A._Done(E.L8[0]._this=null);if((A=E.Ow[0]._this)&&(A._cycle!=D))A._Done(
E.Ow[0]._this=null);};return E;})();

/* Embedded Wizard */