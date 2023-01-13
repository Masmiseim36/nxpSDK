/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software 
* components. It is provided as an example software which is intended to be 
* modified and extended according to particular requirements.
* 
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability 
* and non-infringement of any third party IPR or other rights which may result 
* from the use or the inability to use the software.
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* Version  : 10.0
* Date     : 17.02.2021  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT_VGLite.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_CoreGroup.h"
#include "_CoreTime.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_DashboardAnimationClass.h"
#include "_DashboardBattery.h"
#include "_DashboardClock.h"
#include "_DashboardColorsClass.h"
#include "_DashboardDashboard.h"
#include "_DashboardDashboardV1.h"
#include "_DashboardDashboardV2.h"
#include "_DashboardDemoMode.h"
#include "_DashboardDeviceClass.h"
#include "_DashboardGauge.h"
#include "_DashboardGaugeDisplay.h"
#include "_DashboardGaugeIndicator.h"
#include "_DashboardGaugeLabels.h"
#include "_DashboardNavigation.h"
#include "_DashboardNavigationHints.h"
#include "_DashboardStartScreen.h"
#include "_DashboardVertValueBar.h"
#include "_EffectsFadeInOutTransition.h"
#include "_EffectsFloatEffect.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsPointEffect.h"
#include "_EffectsTransition.h"
#include "_GraphicsArcPath.h"
#include "_GraphicsCanvas.h"
#include "_GraphicsPath.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsFillPath.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsLine.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_ViewsWallpaper.h"
#include "_ViewsWarpGroup.h"
#include "_ViewsWarpImage.h"
#include "_WidgetSetPushButton.h"
#include "_WidgetSetPushButtonConfig.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetToggleButtonConfig.h"
#include "_WidgetSetValueDisplay.h"
#include "_WidgetSetValueDisplayConfig.h"
#include "Core.h"
#include "Dashboard.h"
#include "Effects.h"
#include "Graphics.h"
#include "Views.h"
#include "WidgetSet.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x0000133A, /* ratio 34.70 % */
  0xB8253700, 0x0009A452, 0x00720010, 0x05800188, 0x010F1B00, 0x62E000CC, 0x0C8445A1,
  0x48AC5861, 0x86474003, 0x68D40032, 0x4863B118, 0xE234008E, 0xE4437000, 0x190C8B41,
  0x32B95C56, 0x108748E4, 0x9964CA43, 0x8C48E792, 0x949E4200, 0xCB689418, 0xEE35119B,
  0xA731A8D4, 0x8713A04B, 0x748884B6, 0x158D4AE4, 0x69FC6A27, 0x8AC4E3B1, 0xAD169B21,
  0x456152EA, 0x1E271A88, 0xE470C97D, 0x493E92D7, 0xB2B005BA, 0x368D42A2, 0xA8ADC2D9,
  0x875C6E92, 0x2DD61923, 0xDEAC74AA, 0xCE2B1D96, 0x6151D875, 0xD92FC54B, 0x53CAC4BC,
  0x4516C3A1, 0x89798E11, 0x8F566219, 0x2E5E5D46, 0x57A797CA, 0x833108BF, 0x25B2E8A4,
  0x13A3A2E2, 0x50C85513, 0x16D4C2A1, 0x0CB44307, 0x856B33A0, 0xC44EFD1F, 0xE1A619ED,
  0x8E87A396, 0x23B7EC6C, 0x3315D5EE, 0x71D8D682, 0x13CFC8F4, 0xF8B6736B, 0x89C46138,
  0xC5FB7BD5, 0x442192EE, 0x8EFDD7DC, 0x29113854, 0x894CB7E6, 0x3E2F3C86, 0x11AB6B26,
  0x0DF493D7, 0xA26BA8B6, 0x1D14F1CF, 0x86F1624F, 0x39115999, 0xE976D6A5, 0x4691D7E1,
  0x12851206, 0xB8057078, 0xED5B6194, 0x2D4F6027, 0x8053C701, 0x9B9765C5, 0x64F5F568,
  0x72145157, 0x217A5985, 0x875104AD, 0xD6F68DA0, 0xB6D9A74A, 0x6928749D, 0xA3431A77,
  0x4F134775, 0x62570D0C, 0xE8B9137A, 0x95C84AD7, 0x347D9438, 0x6C9E2666, 0xDA08BD0A,
  0xC6C51048, 0xB622485A, 0x53821DB8, 0x5452D779, 0x92E6EA1A, 0x28052D46, 0xE10A44DB,
  0x4168D627, 0x4B5A095E, 0x9B26E179, 0xA6996853, 0x51FA6CA1, 0x086E1A05, 0x9BD65992,
  0x20B8221A, 0x484A375E, 0xAE7F9154, 0x9F6826C4, 0x7D1DE841, 0x9234BC43, 0xC6153D8F,
  0xA5C19250, 0x9260A847, 0x68270A1A, 0xD2492110, 0xD89DDE83, 0x81CE7796, 0x6B936086,
  0x9CD3D631, 0x19B5712E, 0xD4F2336E, 0xAE49A712, 0x1E9D66E8, 0x48DDD6E2, 0xD1345A54,
  0x26528B44, 0x954F6EE5, 0x2B5E6178, 0x7168BA9D, 0xA0081258, 0x84756F78, 0x5944A86C,
  0x1C525F77, 0x97DC576E, 0xAC47493D, 0x1682BBB2, 0xFDAC9F12, 0xAA786847, 0x9B5D286D,
  0x1ACB5265, 0x5512E175, 0xADE978A5, 0x0CA5E0B4, 0x7E92B6B2, 0xD3BA2E36, 0x882D3C55,
  0x751D60DB, 0x258C6FF4, 0x7EA36A52, 0x647729B8, 0xE7161AAC, 0x624E50A8, 0x9594E506,
  0x50E8B445, 0x1F45C15A, 0xE589DD66, 0x8A8BA727, 0x91BC6229, 0x4993B502, 0xB1E7CD3C,
  0x5A81E37C, 0x52A56A29, 0x8A446D6C, 0xA1143CD2, 0x1DD57288, 0x4A652E7B, 0x29E98892,
  0xA94413CC, 0x44116886, 0x23C515E6, 0x189D7D51, 0x6EA96818, 0x73A1123C, 0xC83216F1,
  0xA7148D16, 0x2A8A4DA1, 0xA5216F1D, 0x165DEDB9, 0x9D9A4B9D, 0x924CF2B7, 0xC9198396,
  0x9D404ADC, 0x848DA8E8, 0xCFE5A43A, 0xDB75FA63, 0xAD294446, 0xC7734D5D, 0x467E2035,
  0x4B594422, 0x14E4F776, 0x260F3252, 0x52A9B633, 0x3B447946, 0xBC772DA5, 0x5CBE279A,
  0x164D9AB9, 0x227FBA66, 0x096B38EA, 0xDB12B687, 0x9DDBB6F7, 0x4DE57DD1, 0x053DBBD4,
  0x275E5E2C, 0x571F8476, 0x11091994, 0x368D7D98, 0xE27E5CD8, 0x57A9F6FD, 0xBB6E3B83,
  0x910D11C5, 0xEAD50853, 0x3D3DD53B, 0x5180D86F, 0x4EFCA516, 0x732616C8, 0x39FF9684,
  0x416F4BFD, 0xD9F7B61E, 0xB834E896, 0xAD04226D, 0xDCEF9E4B, 0x9D216633, 0x116D1169,
  0x47543421, 0xFDCE0B71, 0x3AA1E436, 0x45488DB9, 0x16E8D2D1, 0xED4F9E95, 0x3908218C,
  0xE55EA1DE, 0x4A18E49D, 0xDC1BAD44, 0xAF52745B, 0x5B68F5EE, 0xFC989BAF, 0x53A30750,
  0x1B5DAD4D, 0x809F9FE3, 0x70F52C62, 0x1B18806B, 0x573CE210, 0x8E48DFBD, 0x8EEC0363,
  0xB6E35676, 0x5E8A9CC1, 0x6C912B28, 0xEF3F95F8, 0x162A4500, 0x2CB44B11, 0x60851948,
  0xC108746C, 0x42C2B713, 0x25C4FE22, 0xD042A93E, 0x2C07CE69, 0x3FA9B50D, 0xBC5512D8,
  0xBEAB0218, 0xDA7B4A38, 0x507B4C81, 0x0C9721F4, 0x19448028, 0x37CEFCE0, 0xE8E0FA11,
  0xDC893C3B, 0xB044DCC7, 0x22318E22, 0xDF5F519F, 0x4218344A, 0x9D2D2BD1, 0x0A6BC8AA,
  0x296D2C13, 0xB3EB3A72, 0x42D6E123, 0xCC08D3AA, 0x8FC1624D, 0x1A5538DB, 0x8D0A922A,
  0x4F5D7480, 0x3C4736DC, 0xDC8DCEC6, 0xF51C4AE9, 0x6985C482, 0x9C495727, 0xF47EA017,
  0x45CD9CBD, 0x5459A16C, 0xC672FD19, 0x702B7C97, 0x6412B219, 0xAE13D80C, 0xC260B17C,
  0xEA788D82, 0x71A8C146, 0x7A278A85, 0x85DAC322, 0xE828E944, 0x0EF43A25, 0x16F4350F,
  0x8FEC71D8, 0x68227452, 0x0021B84E, 0x2F6DAD16, 0x3088E189, 0x623BA116, 0xDBB5582B,
  0xF2147E10, 0x09B91923, 0x0A036A77, 0x93C73102, 0x738C58D1, 0x1B208478, 0x5546433F,
  0xD88D4E9D, 0x86420E63, 0x5A443366, 0xE4702510, 0x99433194, 0xD309B521, 0x96694E25,
  0x44570677, 0x66736A82, 0xFD4BCC56, 0x374116E4, 0x84A409E5, 0xFF3A1F0B, 0x562E1C58,
  0x118E6DF1, 0x899EC75C, 0x9464ED4A, 0x58E2FE9B, 0x908910C8, 0x38B1B196, 0x484E624D,
  0xD044D21C, 0x6284282C, 0x20C71FF1, 0x91E19F13, 0x1D4A7424, 0xBA9F181D, 0x8541D2AD,
  0x15126D35, 0xD4A23452, 0x510EB67F, 0x3D21B544, 0x4446D753, 0x5363859D, 0x2B388A1B,
  0x70E82D39, 0x25434FCB, 0x2747389C, 0x4A3274D4, 0x8F0A7AA6, 0x9FF3FE6C, 0x942FA1AE,
  0xCF560D1A, 0x7E42E610, 0xFC277249, 0xD2785FE3, 0xB9982DD7, 0x81285A2D, 0x87CACCDA,
  0x9409EEAC, 0xA2E99042, 0x4C721D64, 0x4607E2C9, 0x87A5932E, 0x149AC300, 0x1C79547D,
  0x289A247C, 0x99C259C6, 0xAAA79713, 0x62E0C0E1, 0x460ED6A4, 0xD2759E44, 0x9733571D,
  0x921501A9, 0x9723FB44, 0xD6DAB7B0, 0x57688FEA, 0x1E968FA3, 0xF99D9A79, 0xDBBF6DED,
  0x5672FF68, 0xCBDD9098, 0xA229DE87, 0xBB328146, 0xB5B3A1C2, 0xD28A2B25, 0xAC551A9C,
  0x45AC147E, 0x46CC0E29, 0xACEED027, 0xD24953E5, 0xAAE8CDE2, 0xCC2CD917, 0xA349484F,
  0x4E7BB2BD, 0x27AE76A5, 0xAADC4BC2, 0xA6DABBBC, 0x1F35B695, 0x76652D6D, 0xD954B576,
  0x247D44E7, 0x15CD6A51, 0x4EA29611, 0x0871796D, 0xCE826698, 0x0D00027C, 0xD30EED76,
  0x1154A2E2, 0x26A68A03, 0x59886D7E, 0x00B5E233, 0x0E30C5E0, 0x00000008, 0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault1[] =
{
  0x000011FE, /* ratio 35.17 % */
  0xB821FB00, 0x0009A452, 0x00600010, 0x06200160, 0x010F1A80, 0x034000B8, 0x00838E40,
  0x01946090, 0x8EC486E0, 0x84421111, 0x42221138, 0x7C66001C, 0xA1B268E4, 0xC708874B,
  0x1138AC60, 0x0C8843A5, 0xA8FCBC5B, 0x874B6512, 0x5456270A, 0xC5E1D1E9, 0x7238009C,
  0x22B2A89C, 0x111E8AC4, 0xD1F1B43A, 0x288AC5E3, 0xB9552239, 0x854A0014, 0xA4B6A917,
  0x47A234CA, 0x029115AA, 0xAD1158FD, 0x5352944B, 0xD0A88DB2, 0x1CA44AA3, 0x9B945283,
  0x8E422417, 0xF5F6270E, 0x002B5108, 0xD62F1C9D, 0xA5D46B94, 0x271E844A, 0x15400612,
  0x72C55225, 0x39445E2F, 0x8FD4E314, 0x75DA3D0E, 0x4AEBD6A9, 0xFEF794C6, 0x21D1388D,
  0x17568BCA, 0x5A9A4E1E, 0x54956D28, 0xACC462B9, 0xAC4721D0, 0x6CA275B2, 0x66A5D16D,
  0xA78771DA, 0xEEF17CFE, 0xF7559BEC, 0xD1BD7456, 0x9FBDE4B4, 0x5DA6230D, 0x857034D2,
  0xA682133F, 0x5238B6DD, 0x53C5DEB2, 0xEF5D5965, 0x1D54E2D9, 0x161AEE96, 0xA8A8E9BE,
  0xEF16E717, 0xB2F5275C, 0x571675D0, 0xB735E260, 0xC9AC7697, 0x154814E5, 0x575AB471,
  0x52A441FB, 0x744DC57F, 0xA5364291, 0x4D421156, 0x5ED2347D, 0x5134A9A5, 0x5441117C,
  0x592E6797, 0x1E6A56F8, 0x89DD156D, 0x1D58361D, 0x3439B44D, 0x95B5705D, 0xC5581F68,
  0x86639871, 0x9B34E20B, 0x9899C072, 0x0D086A59, 0xFF4A9181, 0x48162481, 0xD7B8A596,
  0x88191166, 0x525284DF, 0x426D1455, 0x8C52D5EA, 0x90C8651F, 0x74C91897, 0xA2247452,
  0x7142A514, 0x715D8645, 0x57A6C9A2, 0xF4799D45, 0x91D57A9A, 0xD2615C54, 0x932046C9,
  0xDC75ADE2, 0x56E9DD99, 0xA6619C99, 0x705F5FB6, 0x55DE84B9, 0x50C44681, 0x1441D057,
  0x718F5C9A, 0x2E6F5A79, 0x62D728B5, 0x90A4AA0B, 0xC72A9942, 0xFAA48911, 0x175511F9,
  0x8F6A74BA, 0x1204F28E, 0x24322444, 0xF12E9422, 0xBFA65289, 0x4454AAC9, 0x5EC7A1CE,
  0x0A1D7B62, 0xB29047D9, 0x73A4DB4A, 0xA363796D, 0xEAF75DD0, 0xA9E90C67, 0xA93E471B,
  0x9FAC9B76, 0x7290C9F1, 0x50A5CA12, 0xDB1D1745, 0x092AAB5D, 0x91602B79, 0x8D1717D6,
  0x10C751C7, 0x045E717A, 0x51B44C1D, 0xCEA01A6A, 0x69A235AD, 0xA8D78AB4, 0x84398D9B,
  0xE2C8ADA5, 0x7BB420B9, 0x7B11E711, 0xAB975969, 0x97B6CF78, 0x1D2AAFF0, 0x6B746AE5,
  0x421727AD, 0x1A694EF4, 0x18D10C55, 0x857D42B0, 0xBB6A6EB7, 0x6610C6A6, 0x64F5519D,
  0x2B859878, 0xAE369463, 0x1F81702A, 0x442AF8E7, 0xE7A61917, 0x84F739A5, 0xC51C5762,
  0xFE432AF6, 0xC8D446A6, 0x29C936DA, 0x9D31B14A, 0x215FD3E3, 0x1768E097, 0x746D5B85,
  0xF01A7115, 0xC4469B80, 0x4E04D59E, 0x995EF616, 0x64D6567E, 0x1CAD5A90, 0x87F314CA,
  0x2920D0D1, 0xA7BCECA4, 0x55172CC9, 0xAE35E971, 0x7C6E20D3, 0xE18DB65D, 0xE0BFF397,
  0x63586481, 0x172D9A85, 0x2957267E, 0x1B219457, 0xA6A138D9, 0xAAA96DD1, 0xB0668EB7,
  0x68BDBED6, 0xEACCE8DA, 0x1BA6B415, 0xE35C5A7E, 0x704431FB, 0x083BA571, 0xBDA36D92,
  0xABE57567, 0xCEB818DF, 0xEF9BCB2D, 0x56D15978, 0x417DEBAE, 0x11EDDF2A, 0x4626A9DD,
  0xE91DE9E8, 0x9DFED1F2, 0x8AE07E1E, 0x1C6CB00B, 0x9D280ADD, 0x950576B0, 0x9EC99378,
  0x22A97970, 0x4F893954, 0xE5BD1AA9, 0x39F69D63, 0x4B71E4B5, 0x9A59923B, 0xA6619F56,
  0x6AF7EE5A, 0x9E13B22A, 0xBF1E86AC, 0xA047C87D, 0xF0B3137A, 0x20D33FB6, 0xF1282F44,
  0xE5BEE0DB, 0x29C5ED21, 0x91E1652E, 0xBB355496, 0x7145EE08, 0x4820D2B2, 0xE6BE7816,
  0xE63C4485, 0x0EAA489B, 0xE89D28D5, 0x221A5B05, 0x34684B8D, 0xCB69962A, 0x8E028985,
  0x18B2FA61, 0x2110792A, 0x890EEC38, 0xE2A3F6AA, 0xFE5325F4, 0xEA951591, 0xE1D0B563,
  0xE44074A9, 0xA6EA5D0E, 0x6DB21E31, 0xCE7B8063, 0x1726C478, 0x222B49BE, 0xF8A28BA3,
  0x26C57458, 0xBA448ACA, 0xA10D2289, 0x61AB37E6, 0xB1934EE6, 0xD5FF125D, 0x06C9EE98,
  0x18F2E13D, 0x2B7274BE, 0x6AC8AD72, 0x9F5C71F4, 0xAA27A8A2, 0x6E50EF48, 0xC86C0D3C,
  0xE4F1C628, 0xE45E79EC, 0xA75AE48B, 0x8F8D1A13, 0x4AEAAA51, 0x42D520B8, 0x71AF49E6,
  0x7ED26A9E, 0xEEC8D52E, 0x918FCF3C, 0xE4AFD1E4, 0x15B5A098, 0x9452942F, 0x4958AF5E,
  0xECB4BDA4, 0x53BBCC71, 0xC7C560D7, 0x86AE0C8F, 0xCB53D242, 0x9D1F84B6, 0x765447A0,
  0xBA4ED923, 0x56242373, 0x31C549C8, 0xC766D590, 0xB145A7A9, 0xA5D9A98F, 0xB274F532,
  0x7671459D, 0xD59A79C0, 0x1E4D0754, 0x0421D145, 0xDCE15CF2, 0xA48F2DB6, 0xA2B7366F,
  0x98BADE5A, 0x88F157A0, 0xE5DBCC92, 0x34D634AF, 0x9CF0D8DC, 0xF8D76452, 0x643D5622,
  0x958B293E, 0x84FD5BEC, 0xBD23AF04, 0xD9CA8A27, 0x8796BB12, 0x88752D99, 0xCB2BA395,
  0xC336E2D3, 0x7F134610, 0x7B17FD09, 0x0AB2565A, 0xB380E7D6, 0x046D1981, 0x4CDE1CE8,
  0xA500916C, 0xC644C651, 0x507DE44F, 0xD84C6A53, 0x2347E906, 0x78EE963B, 0x47DC1449,
  0xC1414381, 0x008AE830, 0x56F6D6B5, 0xA6474807, 0x827A9587, 0x95D45BA1, 0xB27523C3,
  0xA79B0F69, 0x9C04DC73, 0x5631EDE7, 0x0C9F2200, 0x0EBAB69B, 0xDC2744B5, 0x110D97DC,
  0xB90E9B66, 0x727FA005, 0xA730E0C9, 0x04EDA033, 0x5653B3A0, 0x198364BA, 0x98B1D9BF,
  0xF3D6CA09, 0x1AAB74A2, 0x49230A1A, 0x8A671024, 0x1DAFD8B3, 0x4B5A580E, 0x43EA9BA2,
  0x07294F1D, 0x68439F4B, 0x1B49D08E, 0x998B31BA, 0xE1A1BD8C, 0xD9460414, 0x1E83C8D0,
  0x95401059, 0x456DC2C3, 0x456A7952, 0xF2301B08, 0xAED9C2C2, 0x5B3BE069, 0xBA574D67,
  0x40E9CCF0, 0x75EDDA92, 0xDF9EC538, 0x424646D9, 0x373BA2B2, 0x20B9DBE6, 0x19E27B97,
  0xF451732E, 0x8EA6E7DB, 0xAB9538B7, 0x5D753573, 0xBA96F6E9, 0x72ADD5DB, 0x005C5523,
  0x626EE130, 0xF6DF2D05, 0x00871C43, 0x0DF0BDE0, 0x00000008, 0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault2[] =
{
  0x000004DE, /* ratio 43.02 % */
  0xB8001500, 0x80086452, 0x00E00030, 0x0D2003A0, 0xDC003780, 0x60789800, 0x072B9160,
  0x68A46A17, 0x20800851, 0x9C84A800, 0xC580A008, 0x72500012, 0x40020824, 0x70001ACB,
  0x70493490, 0x1699C0CD, 0x41800069, 0x18C58112, 0x00EA7400, 0xCF676330, 0x00900093,
  0x49B28A18, 0xDB0400CF, 0x02CA34B8, 0x00175000, 0x8A9D106E, 0x19516A66, 0x6D80015E,
  0x56AA6639, 0xF2AB30AC, 0x06D4096C, 0xAA00B400, 0x36DAC582, 0x2D98002B, 0x9AC4EAC1,
  0xAD56E823, 0x05BEEF69, 0x4B6DD6E0, 0x9EFB2D83, 0x70132ADD, 0xB146AB5C, 0x160994CA,
  0x309E642F, 0x5C7582B1, 0xBB666815, 0x860F2F6C, 0xD02C1819, 0x02A64C96, 0xAAB3AA2D,
  0x2D7EBDCE, 0xB8397D97, 0x45CFD568, 0x1BA51743, 0xCEE0AC53, 0x75032191, 0x5632179C,
  0x06EDABB7, 0x721C7E4E, 0x7558994B, 0xF95A0516, 0x7DBF6EF1, 0xAC5E87AB, 0xDFF52813,
  0xF61B9B82, 0x4F2FBB0D, 0x0F6FD5B7, 0xEA75CC2E, 0x0998C1E6, 0x1EFCB6FB, 0x82F47C6B,
  0x3FBE2F8B, 0xFB5DBDDF, 0xDF47E182, 0x24C1467F, 0x05FB7FA0, 0xF07E9FD8, 0x6F9DA829,
  0xDFD81A09, 0x55402744, 0x152D5484, 0x7C5B97D5, 0x5B9555D9, 0x163569D7, 0x08315D00,
  0xD60F84A0, 0x297CA057, 0x8B5FC812, 0x1F98B211, 0x38A20D84, 0xDE2F8AA3, 0x3A83A2E8,
  0x592078C6, 0xD8F68D66, 0xE871905A, 0xB5554C97, 0xCA745BE0, 0x519E57F9, 0xA4A7818D,
  0x8659EF91, 0x193A7C98, 0x826718E9, 0x4E96E75D, 0x1305B996, 0xA782206C, 0x8D827413,
  0x7D002628, 0x5F9B6875, 0x623581D7, 0xA5B92D6A, 0xB9F255D5, 0x8374D555, 0x9857D4E9,
  0xE1A75DBD, 0x69B54063, 0x4D875697, 0xEB64D6E5, 0x501E09B1, 0xA884E9B0, 0xA90A1B7C,
  0xAE869715, 0xF156A0F9, 0x9713C801, 0xD2D5D646, 0x59315B9D, 0xE5739714, 0xBEA8E4D9,
  0x781DDAA5, 0x2AC5C13C, 0x05F68658, 0xB6425A94, 0x2356E254, 0xAFD2457E, 0x00000101,
  0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault3[] =
{
  0x00000592, /* ratio 39.83 % */
  0xB801D300, 0x0009A452, 0x24600019, 0xC002E004, 0x0034000D, 0x031000B0, 0x0C838D40,
  0x08A44A2D, 0x85436391, 0x442A3636, 0xC605A003, 0xE6346C8A, 0xC71218C8, 0x11178AC6,
  0xA638BC36, 0x1C19C363, 0x38AC5627, 0x97436231, 0x74222711, 0x4665F339, 0xAE711384,
  0x21F0F86C, 0xA1368713, 0x73189CAE, 0x1188C361, 0xC9C49A6B, 0x8F4EA314, 0x849A571D,
  0x4BA71199, 0x9A2B51A1, 0xE8B21844, 0xA144B445, 0x1548553E, 0x4895C840, 0x890DC400,
  0x8CCEE530, 0xC462652E, 0xE02F53D9, 0x7E151892, 0xE6B2B004, 0x1F1586DF, 0x7769F49A,
  0x2EC14223, 0xDA858E71, 0x8C4F2110, 0x5C5E356D, 0x53EBB86B, 0x362B0A84, 0xE394195C,
  0x1F08C14A, 0xB1AC490E, 0x2E8E59E8, 0xBDAEA33F, 0x90C72DF0, 0xA612355C, 0x6B6A18AD,
  0x6A2F82B2, 0xDF1CC001, 0xE3B482C0, 0x31AE3C3E, 0x4B88D1B9, 0x389C8617, 0x95C23112,
  0x6F662F29, 0xD16AB469, 0x7E71949A, 0x67F85BFE, 0x7D19B847, 0x400AF6AB, 0x00A6C669,
  0xCFBF4E9E, 0xE2D56378, 0x55A00158, 0xC521FC7D, 0x21D87CD1, 0x864DD646, 0x5A9945A1,
  0x11A7C5A0, 0x542DA67C, 0xE4005DD7, 0x005F1A15, 0x605605F8, 0x95781147, 0xD6412646,
  0x5DD944DF, 0x53899174, 0x4B971475, 0x50A715A4, 0x14660763, 0x6D946219, 0x04661187,
  0x6750F66E, 0x96E84539, 0x45C9A577, 0x19AC765B, 0xB2885AF5, 0x46A2249D, 0x9B9455B7,
  0xA7051C65, 0x05957C90, 0xBB70D177, 0x471EF749, 0x1CB469E6, 0x673CE673, 0x7E1C7050,
  0xC06FA615, 0x7762D885, 0x5187E59E, 0x2721F48E, 0xD5E980E1, 0x8764D1C9, 0x849436B5,
  0x5499019B, 0xC5395D7F, 0x3E1277D1, 0x236ED754, 0x43E4F566, 0x56163E06, 0x38356579,
  0x86548DA7, 0x5E9522A6, 0x9E50B69E, 0x21845E3A, 0x45ED2649, 0xCD9B4A60, 0x9A902207,
  0x589FC8B9, 0x63462DB4, 0x164A98AA, 0xF1A44855, 0x408DEAD8, 0x8496C842, 0x29D461A3,
  0xDA462346, 0xCD506C1F, 0x468EE816, 0x5422594D, 0xD459B55F, 0xB9EA6870, 0x32630026,
  0x00406DA7, 0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault4[] =
{
  0x000002D2, /* ratio 55.40 % */
  0xB801ED00, 0x8009A452, 0x005C0019, 0x064001B0, 0x62001600, 0x30D1A800, 0x0B87C4E1,
  0x98745A0F, 0x000C2303, 0x64762A68, 0x42A1D1A2, 0x4A210E86, 0xE2F268A4, 0x002D0018,
  0x3088B0D0, 0x319BC465, 0x21C42A67, 0x9E43A313, 0x94C6290A, 0xC226D0A8, 0x006F0A8E,
  0x22524A50, 0x9741A542, 0xE0096462, 0x3399C2A3, 0x5A4C225F, 0xABCDE551, 0xA57A3A00,
  0x4F2110A8, 0xDE750E88, 0x2A319B44, 0xA540B645, 0xB1287492, 0x10864225, 0x18EC3229,
  0x93C32632, 0xBCC0D91A, 0xCF001449, 0xC6110E96, 0xA2D42B04, 0x91318EC4, 0x137B245E,
  0x4AB8CF72, 0x5AB43B2D, 0x97802C35, 0x54DEBF43, 0xC322D39A, 0xF25B0E98, 0x237A3C54,
  0x277F8ACE, 0x9169FE0F, 0x36B04569, 0xA8AEDF1F, 0xC2E0C874, 0xE44A31A9, 0x54B17129,
  0x5EB70CAF, 0x39510844, 0x5B17E14A, 0xA002D452, 0x058183BA, 0xE001D401, 0xCF9A4006,
  0xA8800120, 0x001610E8, 0x6011D5DF, 0x0CA00297, 0x43E03080, 0x5D804F56, 0x00208008,
  0xCF17E4A8, 0xEC01BABF, 0x4700006D, 0xF761E978, 0x00440004, 0xE378E070, 0x76005755,
  0x6010006B, 0x28401875, 0x706F7F21, 0xDB4BDFC7, 0x878001C9, 0x074775EE, 0x07B4737F,
  0x7E2388C0, 0xD519DDD7, 0x80004001, 0x9D80362D, 0x18B1DF87, 0x8DE388D2, 0xD586A1D8,
  0x609D8031, 0x227199EE, 0xC87E3A7F, 0x600A0023, 0x1E866427, 0x919EF1DE, 0x6528B464,
  0x00000040, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XColor _Const0000 = { 0x00, 0x00, 0x00, 0xFF };
static const XColor _Const0001 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XColor _Const0002 = { 0x54, 0x54, 0x54, 0x7E };
static const XColor _Const0003 = { 0x01, 0xB8, 0xF6, 0xFF };
static const XColor _Const0004 = { 0x00, 0xDC, 0xE3, 0xFF };
static const XColor _Const0005 = { 0xFF, 0xB5, 0x3E, 0xFF };
static const XColor _Const0006 = { 0x00, 0xFF, 0xA9, 0xFF };
static const XColor _Const0007 = { 0x4E, 0xB5, 0x99, 0xFF };
static const XColor _Const0008 = { 0x33, 0xAA, 0x71, 0xFF };
static const XColor _Const0009 = { 0x50, 0x50, 0x50, 0xFF };
static const XColor _Const000A = { 0x10, 0x99, 0x5E, 0xFF };
static const XColor _Const000B = { 0xFF, 0xFF, 0xFF, 0xC8 };
static const XColor _Const000C = { 0x00, 0x26, 0x36, 0xFF };
static const XColor _Const000D = { 0x0B, 0x78, 0xAD, 0xFF };
static const XColor _Const000E = { 0x00, 0xF6, 0xFF, 0xFF };
static const XColor _Const000F = { 0x12, 0x14, 0x3D, 0xFF };
static const XColor _Const0010 = { 0x03, 0x1D, 0x29, 0xFF };
static const XRect _Const0011 = {{ 0, 0 }, { 1280, 720 }};
static const XRect _Const0012 = {{ 0, 0 }, { 640, 720 }};
static const XRect _Const0013 = {{ 640, 0 }, { 1280, 720 }};
static const XRect _Const0014 = {{ 461, -5 }, { 818, 193 }};
static const XRect _Const0015 = {{ 1064, 501 }, { 1244, 836 }};
static const XRect _Const0016 = {{ 36, 501 }, { 216, 836 }};
static const XRect _Const0017 = {{ 62, 57 }, { 532, 527 }};
static const XRect _Const0018 = {{ 748, 57 }, { 1218, 527 }};
static const XStringRes _Const0019 = { _StringsDefault0, 0x0002 };
static const XStringRes _Const001A = { _StringsDefault1, 0x0002 };
static const XRect _Const001B = {{ 134, 86 }, { 1158, 667 }};
static const XRect _Const001C = {{ 489, 211 }, { 792, 514 }};
static const XStringRes _Const001D = { _StringsDefault2, 0x0002 };
static const XRect _Const001E = {{ 49, 589 }, { 329, 690 }};
static const XRect _Const001F = {{ 0, 0 }, { 150, 150 }};
static const XStringRes _Const0020 = { _StringsDefault2, 0x000C };
static const XStringRes _Const0021 = { _StringsDefault2, 0x0013 };
static const XStringRes _Const0022 = { _StringsDefault2, 0x001A };
static const XRect _Const0023 = {{ 0, 122 }, { 150, 147 }};
static const XStringRes _Const0024 = { _StringsDefault2, 0x001F };
static const XPoint _Const0025 = { 143, 116 };
static const XPoint _Const0026 = { 6, 116 };
static const XRect _Const0027 = {{ 8, 60 }, { 140, 110 }};
static const XStringRes _Const0028 = { _StringsDefault2, 0x0026 };
static const XRect _Const0029 = {{ 11, -11 }, { 138, 67 }};
static const XStringRes _Const002A = { _StringsDefault2, 0x002B };
static const XRect _Const002B = {{ 0, 0 }, { 90, 40 }};
static const XStringRes _Const002C = { _StringsDefault2, 0x0030 };
static const XStringRes _Const002D = { _StringsDefault2, 0x0038 };
static const XRect _Const002E = {{ 9, 16 }, { 279, 705 }};
static const XRect _Const002F = {{ 1003, 16 }, { 1273, 705 }};
static const XRect _Const0030 = {{ 795, 46 }, { 1198, 675 }};
static const XRect _Const0031 = {{ 875, 67 }, { 1104, 503 }};
static const XRect _Const0032 = {{ 177, 67 }, { 405, 503 }};
static const XRect _Const0033 = {{ 7, 46 }, { 410, 675 }};
static const XPoint _Const0034 = { -403, 46 };
static const XPoint _Const0035 = { 84, 46 };
static const XPoint _Const0036 = { 1280, 46 };
static const XPoint _Const0037 = { 795, 46 };
static const XPoint _Const0038 = { -310, 46 };
static const XPoint _Const0039 = { 177, 67 };
static const XPoint _Const003A = { 1360, 46 };
static const XPoint _Const003B = { 875, 67 };
static const XStringRes _Const003C = { _StringsDefault2, 0x0040 };
static const XStringRes _Const003D = { _StringsDefault2, 0x0148 };
static const XRect _Const003E = {{ 0, 0 }, { 403, 629 }};
static const XRect _Const003F = {{ 0, 344 }, { 240, 628 }};
static const XPoint _Const0040 = { 0, -20 };
static const XRect _Const0041 = {{ 185, 310 }, { 425, 313 }};
static const XStringRes _Const0042 = { _StringsDefault3, 0x0002 };
static const XStringRes _Const0043 = { _StringsDefault3, 0x00EB };
static const XStringRes _Const0044 = { _StringsDefault3, 0x01E3 };
static const XStringRes _Const0045 = { _StringsDefault4, 0x0002 };
static const XRect _Const0046 = {{ 0, 0 }, { 100, 35 }};
static const XRect _Const0047 = {{ 4, 4 }, { 51, 31 }};
static const XRect _Const0048 = {{ 0, 0 }, { 828, 451 }};
static const XPoint _Const0049 = { 450, 450 };
static const XPoint _Const004A = { 449, 413 };
static const XPoint _Const004B = { 486, 404 };
static const XPoint _Const004C = { 475, 358 };
static const XPoint _Const004D = { 416, 240 };
static const XPoint _Const004E = { 362, 155 };
static const XPoint _Const004F = { 371, 137 };
static const XPoint _Const0050 = { 390, 132 };
static const XPoint _Const0051 = { 437, 29 };
static const XPoint _Const0052 = { 450, 38 };
static const XPoint _Const0053 = { 450, 0 };
static const XRect _Const0054 = {{ 406, 230 }, { 426, 250 }};
static const XRect _Const0055 = {{ 0, 0 }, { 470, 470 }};
static const XPoint _Const0056 = { 235, 235 };
static const XRect _Const0057 = {{ 89, 136 }, { 381, 380 }};
static const XRect _Const0058 = {{ 128, 129 }, { 342, 340 }};
static const XRect _Const0059 = {{ 82, 82 }, { 387, 387 }};
static const XRect _Const005A = {{ 56, 56 }, { 413, 413 }};
static const XRect _Const005B = {{ 0, 0 }, { 470, 420 }};
static const XRect _Const005C = {{ 86, 86 }, { 384, 384 }};
static const XRect _Const005D = {{ 90, 90 }, { 380, 380 }};
static const XRect _Const005E = {{ 226, 269 }, { 1054, 720 }};
static const XRect _Const005F = {{ 1168, 2 }, { 1278, 83 }};
static const XRect _Const0060 = {{ 17, 1 }, { 127, 82 }};
static const XRect _Const0061 = {{ 559, 107 }, { 724, 267 }};
static const XRect _Const0062 = {{ 596, 59 }, { 686, 99 }};
static const XRect _Const0063 = {{ 537, 19 }, { 747, 64 }};
static const XStringRes _Const0064 = { _StringsDefault4, 0x00F8 };
static const XRect _Const0065 = {{ 558, 603 }, { 723, 631 }};
static const XStringRes _Const0066 = { _StringsDefault4, 0x0106 };
static const XRect _Const0067 = {{ 590, 636 }, { 689, 671 }};
static const XRect _Const0068 = {{ 909, 218 }, { 1059, 368 }};
static const XStringRes _Const0069 = { _StringsDefault4, 0x010E };
static const XPoint _Const006A = { 909, 368 };
static const XPoint _Const006B = { 1059, 368 };
static const XPoint _Const006C = { 1059, 218 };
static const XPoint _Const006D = { 909, 218 };
static const XRect _Const006E = {{ 227, 218 }, { 377, 368 }};
static const XStringRes _Const006F = { _StringsDefault4, 0x0117 };
static const XStringRes _Const0070 = { _StringsDefault4, 0x011D };
static const XStringRes _Const0071 = { _StringsDefault4, 0x0125 };
static const XPoint _Const0072 = { 227, 368 };
static const XPoint _Const0073 = { 377, 368 };
static const XPoint _Const0074 = { 377, 218 };
static const XPoint _Const0075 = { 227, 218 };
static const XRect _Const0076 = {{ 60, 400 }, { 102, 442 }};
static const XRect _Const0077 = {{ 130, 490 }, { 172, 532 }};
static const XRect _Const0078 = {{ 170, 560 }, { 212, 602 }};
static const XRect _Const0079 = {{ 1090, 450 }, { 1132, 492 }};
static const XRect _Const007A = {{ 1010, 520 }, { 1052, 562 }};
static const XRect _Const007B = {{ 1050, 590 }, { 1092, 632 }};
static const XRect _Const007C = {{ 499, 677 }, { 819, 707 }};
static const XStringRes _Const007D = { _StringsDefault4, 0x012A };
static const XPoint _Const007E = { 94, 585 };
static const XPoint _Const007F = { 73, 657 };
static const XPoint _Const0080 = { 65, 520 };
static const XPoint _Const0081 = { 102, 592 };
static const XPoint _Const0082 = { 44, 460 };
static const XPoint _Const0083 = { 130, 532 };
static const XPoint _Const0084 = { 1158, 584 };
static const XPoint _Const0085 = { 1171, 655 };
static const XPoint _Const0086 = { 1179, 518 };
static const XPoint _Const0087 = { 1136, 592 };
static const XPoint _Const0088 = { 1198, 454 };
static const XPoint _Const0089 = { 1105, 530 };
static const XRect _Const008A = {{ 0, 0 }, { 165, 160 }};
static const XRect _Const008B = {{ 19, 96 }, { 184, 132 }};
static const XStringRes _Const008C = { _StringsDefault4, 0x0142 };
static const XRect _Const008D = {{ 0, 124 }, { 165, 157 }};
static const XStringRes _Const008E = { _StringsDefault4, 0x0146 };
static const XRect _Const008F = {{ 0, 0 }, { 165, 80 }};
static const XStringRes _Const0090 = { _StringsDefault4, 0x0153 };
static const XStringRes _Const0091 = { _StringsDefault4, 0x015F };
static const XRect _Const0092 = {{ 0, 0 }, { 469, 469 }};
static const XRect _Const0093 = {{ 0, 0 }, { 290, 290 }};
static const XPoint _Const0094 = { 144, 289 };
static const XPoint _Const0095 = { 157, 289 };
static const XPoint _Const0096 = { 157, 231 };
static const XPoint _Const0097 = { 144, 231 };
static const XPoint _Const0098 = { 6, 146 };
static const XColor _Const0099 = { 0xFF, 0x00, 0x00, 0xFF };
static const XColor _Const009A = { 0x00, 0x00, 0x00, 0x00 };

#ifndef EW_DONT_CHECK_INDEX
  /* This function is used to check the indices when accessing an array.
     If you don't want this verification add the define EW_DONT_CHECK_INDEX
     to your Makefile or project settings. */
  static int EwCheckIndex( int aIndex, int aRange, const char* aFile, int aLine )
  {
    if (( aIndex < 0 ) || ( aIndex >= aRange ))
    {
      EwPrint( "[FATAL ERROR in %s:%d] Array index %d out of bounds %d",
                aFile, aLine, aIndex, aRange );
      EwPanic();
    }
    return aIndex;
  }

  #define EwCheckIndex( aIndex, aRange ) \
    EwCheckIndex( aIndex, aRange, __FILE__, __LINE__ )
#else
  #define EwCheckIndex( aIndex, aRange ) aIndex
#endif

/* Initializer for the class 'Dashboard::ColorsClass' */
void DashboardColorsClass__Init( DashboardColorsClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardColorsClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardColorsClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Background = _Const0000;
  _this->Text = _Const0001;
  _this->TextLedsOff = _Const0002;
  _this->GaugeGlowV1 = _Const0003;
  _this->BatteryGaugeSelectionV1 = _Const0004;
  _this->LedsOnOrange = _Const0005;
  _this->GaugeGlowV2 = _Const0006;
  _this->BatteryGaugeSelectionV2 = _Const0007;
  _this->WingV2 = _Const0008;
  _this->BatteryFrame = _Const0009;
  _this->LedsOnGreen = _Const000A;
  _this->GaugeLabelLinesV1 = _Const000B;
  _this->GaugeCircle0 = _Const0003;
  _this->GaugeCircle1 = _Const000C;
  _this->GaugeCircle2 = _Const000D;
  _this->GaugeCircle3 = _Const000E;
  _this->GaugeCircle4 = _Const000F;
  _this->GaugeCircle5 = _Const0010;
}

/* Re-Initializer for the class 'Dashboard::ColorsClass' */
void DashboardColorsClass__ReInit( DashboardColorsClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Dashboard::ColorsClass' */
void DashboardColorsClass__Done( DashboardColorsClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* Variants derived from the class : 'Dashboard::ColorsClass' */
EW_DEFINE_CLASS_VARIANTS( DashboardColorsClass )
EW_END_OF_CLASS_VARIANTS( DashboardColorsClass )

/* Virtual Method Table (VMT) for the class : 'Dashboard::ColorsClass' */
EW_DEFINE_CLASS( DashboardColorsClass, XObject, _None, _None, _None, _None, _None, 
                 _None, "Dashboard::ColorsClass" )
EW_END_OF_CLASS( DashboardColorsClass )

/* User defined auto object: 'Dashboard::Colors' */
EW_DEFINE_AUTOOBJECT( DashboardColors, DashboardColorsClass )

/* Initializer for the auto object 'Dashboard::Colors' */
void DashboardColors__Init( DashboardColorsClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Dashboard::Colors' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardColors )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardColors )

/* Initializer for the class 'Dashboard::DashboardV1' */
void DashboardDashboardV1__Init( DashboardDashboardV1 _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardDashboardV1 );

  /* ... then construct all embedded objects */
  GraphicsPath__Init( &_this->leftGrowingPath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->rightPath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->rightGrowingPath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->leftPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->LeftStrokePath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->RightStrokePath, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->TopGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->RightBottomGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftBottomGlow, &_this->_XObject, 0 );
  DashboardGauge__Init( &_this->SpeedGauge, &_this->_XObject, 0 );
  DashboardGauge__Init( &_this->PowerGauge, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDashboardV1 );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0011 );
  CoreRectView__OnSetBounds( &_this->LeftStrokePath, _Const0012 );
  ViewsStrokePath_OnSetColor( &_this->LeftStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV1 );
  ViewsStrokePath_OnSetMiterLimit( &_this->LeftStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->LeftStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->LeftStrokePath, GraphicsPathCapRound );
  CoreRectView__OnSetBounds( &_this->RightStrokePath, _Const0013 );
  ViewsStrokePath_OnSetColor( &_this->RightStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV1 );
  ViewsStrokePath_OnSetMiterLimit( &_this->RightStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->RightStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->RightStrokePath, GraphicsPathCapRound );
  CoreRectView__OnSetBounds( &_this->TopGlow, _Const0014 );
  ViewsImage_OnSetColor( &_this->TopGlow, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->RightBottomGlow, _Const0015 );
  ViewsImage_OnSetColor( &_this->RightBottomGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->LeftBottomGlow, _Const0016 );
  ViewsImage_OnSetColor( &_this->LeftBottomGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->SpeedGauge, _Const0017 );
  DashboardGauge_OnSetClockwise( &_this->SpeedGauge, 1 );
  CoreRectView__OnSetBounds( &_this->PowerGauge, _Const0018 );
  DashboardGauge_OnSetClockwise( &_this->PowerGauge, 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TopGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightBottomGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftBottomGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedGauge ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PowerGauge ), 0 );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftPath );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightPath );
  ViewsImage_OnSetBitmap( &_this->TopGlow, EwLoadResource( &DashboardTopGloomV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->RightBottomGlow, EwLoadResource( &DashboardRightBottomGloomV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->LeftBottomGlow, EwLoadResource( &DashboardLeftBottomGloomV1, 
  ResourcesBitmap ));
  DashboardGauge_OnSetOutlet( &_this->SpeedGauge, EwNewRef( EwGetAutoObject( &DashboardDevice, 
  DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ));
  DashboardGauge_OnSetOutlet( &_this->PowerGauge, EwNewRef( EwGetAutoObject( &DashboardDevice, 
  DashboardDeviceClass ), DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy ));

  /* Call the user defined constructor */
  DashboardDashboardV1_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::DashboardV1' */
void DashboardDashboardV1__ReInit( DashboardDashboardV1 _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  GraphicsPath__ReInit( &_this->leftGrowingPath );
  GraphicsPath__ReInit( &_this->rightPath );
  GraphicsPath__ReInit( &_this->rightGrowingPath );
  GraphicsPath__ReInit( &_this->leftPath );
  ViewsStrokePath__ReInit( &_this->LeftStrokePath );
  ViewsStrokePath__ReInit( &_this->RightStrokePath );
  ViewsImage__ReInit( &_this->TopGlow );
  ViewsImage__ReInit( &_this->RightBottomGlow );
  ViewsImage__ReInit( &_this->LeftBottomGlow );
  DashboardGauge__ReInit( &_this->SpeedGauge );
  DashboardGauge__ReInit( &_this->PowerGauge );
}

/* Finalizer method for the class 'Dashboard::DashboardV1' */
void DashboardDashboardV1__Done( DashboardDashboardV1 _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  GraphicsPath__Done( &_this->leftGrowingPath );
  GraphicsPath__Done( &_this->rightPath );
  GraphicsPath__Done( &_this->rightGrowingPath );
  GraphicsPath__Done( &_this->leftPath );
  ViewsStrokePath__Done( &_this->LeftStrokePath );
  ViewsStrokePath__Done( &_this->RightStrokePath );
  ViewsImage__Done( &_this->TopGlow );
  ViewsImage__Done( &_this->RightBottomGlow );
  ViewsImage__Done( &_this->LeftBottomGlow );
  DashboardGauge__Done( &_this->SpeedGauge );
  DashboardGauge__Done( &_this->PowerGauge );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void DashboardDashboardV1_UpdateViewState( DashboardDashboardV1 _this, XSet aState )
{
  XInt32 noOfEdges;
  XInt32 progress;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  DashboardGauge_OnSetOpacityLabels( &_this->PowerGauge, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.400000f, 
  0.700000f, _this->CurrentProgress ));
  DashboardGauge_OnSetOpacityLabels( &_this->SpeedGauge, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.400000f, 
  0.700000f, _this->CurrentProgress ));
  DashboardGauge_OnSetOpacityGauge( &_this->PowerGauge, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.500000f, 
  0.900000f, _this->CurrentProgress ));
  DashboardGauge_OnSetOpacityGauge( &_this->SpeedGauge, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.500000f, 
  0.900000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->TopGlow, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.900000f, 
  1.000000f, _this->CurrentProgress ));
  noOfEdges = GraphicsPath_GetNoOfEdges( &_this->leftPath, 0 );
  progress = noOfEdges - DashboardAnimationClass_linearValueProgress( EwGetAutoObject( 
  &DashboardAnimation, DashboardAnimationClass ), 0, noOfEdges, 0.150000f, 0.950000f, 
  _this->CurrentProgress );
  GraphicsPath_InitSubPath( &_this->leftGrowingPath, 0, noOfEdges );
  GraphicsPath_AddCopy( &_this->leftGrowingPath, 0, &_this->leftPath, 0, progress, 
  noOfEdges - progress );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftGrowingPath );
  noOfEdges = GraphicsPath_GetNoOfEdges( &_this->rightPath, 0 );
  progress = DashboardAnimationClass_linearValueProgress( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), 0, noOfEdges, 0.150000f, 0.950000f, _this->CurrentProgress );
  GraphicsPath_InitSubPath( &_this->rightGrowingPath, 0, noOfEdges );
  GraphicsPath_AddCopy( &_this->rightGrowingPath, 0, &_this->rightPath, 0, 0, progress );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightGrowingPath );
  ViewsImage_OnSetOpacity( &_this->LeftBottomGlow, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.900000f, 
  1.000000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->RightBottomGlow, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.900000f, 
  1.000000f, _this->CurrentProgress ));
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardDashboardV1_Init( DashboardDashboardV1 _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwSignal( EwNewSlot( _this, DashboardDashboardV1_createPath ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::DashboardV1.OnSetOutlet()' */
void DashboardDashboardV1_OnSetOutlet( DashboardDashboardV1 _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardDashboardV1_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardDashboardV1_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardDashboardV1_onOutlet ), ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardDashboardV1_onOutlet( DashboardDashboardV1 _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardDashboardV1_OnSetCurrentProgress( _this, EwOnGetFloat( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::DashboardV1.OnSetCurrentProgress()' */
void DashboardDashboardV1_OnSetCurrentProgress( DashboardDashboardV1 _this, XFloat 
  value )
{
  if ( _this->CurrentProgress == value )
    return;

  _this->CurrentProgress = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Dashboard::DashboardV1.createPath()' */
void DashboardDashboardV1_createPath( DashboardDashboardV1 _this, XObject sender )
{
  XString svg;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  svg = EwLoadString( &_Const0019 );
  GraphicsPath_CreateFromSVGString( &_this->leftPath, svg, 1.000000f, 1.000000f, 
  41.500000f, 38.000000f, 0.000000f );
  svg = EwLoadString( &_Const001A );
  GraphicsPath_CreateFromSVGString( &_this->rightPath, svg, 1.000000f, 1.000000f, 
  0.000000f, 38.000000f, 0.000000f );
}

/* Variants derived from the class : 'Dashboard::DashboardV1' */
EW_DEFINE_CLASS_VARIANTS( DashboardDashboardV1 )
EW_END_OF_CLASS_VARIANTS( DashboardDashboardV1 )

/* Virtual Method Table (VMT) for the class : 'Dashboard::DashboardV1' */
EW_DEFINE_CLASS( DashboardDashboardV1, CoreGroup, Outlet, Outlet, Outlet, leftGrowingPath, 
                 CurrentProgress, CurrentProgress, "Dashboard::DashboardV1" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  DashboardDashboardV1_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardDashboardV1 )

/* Include a file containing the bitmap resource : 'Dashboard::StartEngine' */
#include "_DashboardStartEngine.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::StartEngine' */
EW_RES_WITHOUT_VARIANTS( DashboardStartEngine )

/* Initializer for the class 'Dashboard::StartScreen' */
void DashboardStartScreen__Init( DashboardStartScreen _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardStartScreen );

  /* ... then construct all embedded objects */
  EffectsInt32Effect__Init( &_this->glowEffect, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->StartEngineGloom, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->StartEngineButton, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardStartScreen );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0011 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->glowEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->glowEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->glowEffect, 750 );
  CoreRectView__OnSetBounds( &_this->StartEngineGloom, _Const001B );
  ViewsImage_OnSetOpacity( &_this->StartEngineGloom, 0 );
  CoreRectView__OnSetBounds( &_this->StartEngineButton, _Const001C );
  WidgetSetPushButton_OnSetIconFrameActive( &_this->StartEngineButton, 1 );
  WidgetSetPushButton_OnSetIconFrameFocused( &_this->StartEngineButton, 0 );
  WidgetSetPushButton_OnSetIconFrameDisabled( &_this->StartEngineButton, 0 );
  WidgetSetPushButton_OnSetIconFrameDefault( &_this->StartEngineButton, 0 );
  WidgetSetPushButton_OnSetLabel( &_this->StartEngineButton, EwLoadString( &_Const001D ));
  CoreRectView__OnSetBounds( &_this->Image, _Const001E );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StartEngineGloom ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StartEngineButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  _this->glowEffect.Super1.OnFinished = EwNewSlot( _this, DashboardStartScreen_onGlowEffectFinished );
  _this->glowEffect.Outlet = EwNewRef( &_this->StartEngineGloom, ViewsImage_OnGetOpacity, 
  ViewsImage_OnSetOpacity );
  ViewsImage_OnSetBitmap( &_this->StartEngineGloom, EwLoadResource( &DashboardStartEngineGloom, 
  ResourcesBitmap ));
  _this->StartEngineButton.OnActivate = EwNewSlot( _this, DashboardStartScreen_onStart );
  WidgetSetPushButton_OnSetIcon( &_this->StartEngineButton, EwLoadResource( &DashboardStartEngineStates, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->StartEngineButton, EwGetAutoObject( 
  &DashboardStartEngineButtonConfig, WidgetSetPushButtonConfig ));
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &DashboardLogo, ResourcesBitmap ));

  /* Call the user defined constructor */
  DashboardStartScreen_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::StartScreen' */
void DashboardStartScreen__ReInit( DashboardStartScreen _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsInt32Effect__ReInit( &_this->glowEffect );
  ViewsImage__ReInit( &_this->StartEngineGloom );
  WidgetSetPushButton__ReInit( &_this->StartEngineButton );
  ViewsImage__ReInit( &_this->Image );
}

/* Finalizer method for the class 'Dashboard::StartScreen' */
void DashboardStartScreen__Done( DashboardStartScreen _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsInt32Effect__Done( &_this->glowEffect );
  ViewsImage__Done( &_this->StartEngineGloom );
  WidgetSetPushButton__Done( &_this->StartEngineButton );
  ViewsImage__Done( &_this->Image );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardStartScreen_Init( DashboardStartScreen _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwAttachRefObserver( EwNewSlot( _this, DashboardStartScreen_onAutoDemoModeChanged ), 
    EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetAutoDemoMode, 
    DashboardDeviceClass_OnSetAutoDemoMode ), 0 );
  EwPostSignal( EwNewSlot( _this, DashboardStartScreen_onAutoDemoModeChanged ), 
    ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::StartScreen.onStart()' */
void DashboardStartScreen_onStart( DashboardStartScreen _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  DashboardDeviceClass_OnSetSpeed( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetEnergy( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetBattery( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  67 );
  DashboardDeviceClass_OnSetODO( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  267 );
  DashboardDeviceClass_OnSetLed0( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetLed1( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetLed2( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetLed3( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetLed4( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  DashboardDeviceClass_OnSetLed5( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  EwSignal( EwNewSlot( &_this->glowEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::StartScreen.onGlowEffectFinished()' */
void DashboardStartScreen_onGlowEffectFinished( DashboardStartScreen _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( CoreGroup__IsDialog( _this, 0 ))
    CoreGroup_SwitchToDialog( _this->Super3.Owner, ((CoreGroup)EwNewObject( DashboardDashboard, 
    0 )), ((EffectsTransition)EwGetAutoObject( &DashboardFadeInOutBottom, EffectsFadeInOutTransition )), 
    0, 0, 0, 0, 0, 0, EwNullSlot, EwNullSlot, 0 );
}

/* 'C' function for method : 'Dashboard::StartScreen.onAutoDemoModeChanged()' */
void DashboardStartScreen_onAutoDemoModeChanged( DashboardStartScreen _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass )->AutoDemoMode )
    EwSignal( EwNewSlot( _this, DashboardStartScreen_onStart ), ((XObject)_this ));
}

/* Variants derived from the class : 'Dashboard::StartScreen' */
EW_DEFINE_CLASS_VARIANTS( DashboardStartScreen )
EW_END_OF_CLASS_VARIANTS( DashboardStartScreen )

/* Virtual Method Table (VMT) for the class : 'Dashboard::StartScreen' */
EW_DEFINE_CLASS( DashboardStartScreen, CoreGroup, glowEffect, glowEffect, glowEffect, 
                 glowEffect, _None, _None, "Dashboard::StartScreen" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardStartScreen )

/* Initializer for the class 'Dashboard::GaugeDisplay' */
void DashboardGaugeDisplay__Init( DashboardGaugeDisplay _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardGaugeDisplay );

  /* ... then construct all embedded objects */
  ViewsText__Init( &_this->DescText, &_this->_XObject, 0 );
  ViewsLine__Init( &_this->Line, &_this->_XObject, 0 );
  ViewsText__Init( &_this->UnitText, &_this->_XObject, 0 );
  ViewsText__Init( &_this->ValueText, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGaugeDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const001F );
  _this->Unit = EwShareString( EwLoadString( &_Const0020 ));
  _this->Desc = EwShareString( EwLoadString( &_Const0021 ));
  _this->DescUnit = EwShareString( EwLoadString( &_Const0022 ));
  CoreRectView__OnSetBounds( &_this->DescText, _Const0023 );
  ViewsText_OnSetAlignment( &_this->DescText, ViewsTextAlignmentAlignHorzCenter 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->DescText, EwConcatString( EwConcatString( _this->Desc, 
  EwLoadString( &_Const0024 )), _this->DescUnit ));
  ViewsText_OnSetColor( &_this->DescText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreLineView_OnSetPoint2((CoreLineView)&_this->Line, _Const0025 );
  CoreLineView_OnSetPoint1((CoreLineView)&_this->Line, _Const0026 );
  ViewsLine_OnSetWidth( &_this->Line, 4 );
  ViewsLine_OnSetColor( &_this->Line, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->UnitText, _Const0027 );
  ViewsText_OnSetString( &_this->UnitText, EwLoadString( &_Const0028 ));
  ViewsText_OnSetColor( &_this->UnitText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->ValueText, _Const0029 );
  ViewsText_OnSetString( &_this->ValueText, EwLoadString( &_Const002A ));
  ViewsText_OnSetColor( &_this->ValueText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DescText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Line ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->UnitText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ValueText ), 0 );
  ViewsText_OnSetFont( &_this->DescText, EwLoadResource( &DashboardFontLightM, ResourcesFont ));
  ViewsText_OnSetFont( &_this->UnitText, EwLoadResource( &DashboardFontXL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->ValueText, EwLoadResource( &DashboardFontXXL, ResourcesFont ));
}

/* Re-Initializer for the class 'Dashboard::GaugeDisplay' */
void DashboardGaugeDisplay__ReInit( DashboardGaugeDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->DescText );
  ViewsLine__ReInit( &_this->Line );
  ViewsText__ReInit( &_this->UnitText );
  ViewsText__ReInit( &_this->ValueText );
}

/* Finalizer method for the class 'Dashboard::GaugeDisplay' */
void DashboardGaugeDisplay__Done( DashboardGaugeDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->DescText );
  ViewsLine__Done( &_this->Line );
  ViewsText__Done( &_this->UnitText );
  ViewsText__Done( &_this->ValueText );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetOutlet()' */
void DashboardGaugeDisplay_OnSetOutlet( DashboardGaugeDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardGaugeDisplay_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardGaugeDisplay_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardGaugeDisplay_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.onOutlet()' */
void DashboardGaugeDisplay_onOutlet( DashboardGaugeDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    ViewsText_OnSetString( &_this->ValueText, EwNewStringInt( EwOnGetInt32( _this->Outlet ), 
    0, 10 ));
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetUnit()' */
void DashboardGaugeDisplay_OnSetUnit( DashboardGaugeDisplay _this, XString value )
{
  if ( !EwCompString( _this->Unit, value ))
    return;

  _this->Unit = EwShareString( value );
  ViewsText_OnSetString( &_this->UnitText, value );
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetDesc()' */
void DashboardGaugeDisplay_OnSetDesc( DashboardGaugeDisplay _this, XString value )
{
  if ( !EwCompString( _this->Desc, value ))
    return;

  _this->Desc = EwShareString( value );
  EwPostSignal( EwNewSlot( _this, DashboardGaugeDisplay_onOutletDesc ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetOutletDesc()' */
void DashboardGaugeDisplay_OnSetOutletDesc( DashboardGaugeDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->OutletDesc, value ))
    return;

  if ( _this->OutletDesc.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardGaugeDisplay_onOutletDesc ), 
      _this->OutletDesc, 0 );

  _this->OutletDesc = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardGaugeDisplay_onOutletDesc ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardGaugeDisplay_onOutletDesc ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.onOutletDesc()' */
void DashboardGaugeDisplay_onOutletDesc( DashboardGaugeDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletDesc.Object != 0 )
    ViewsText_OnSetString( &_this->DescText, EwConcatString( EwConcatString( _this->Desc, 
    EwNewStringInt( EwOnGetInt32( _this->OutletDesc ), 0, 10 )), _this->DescUnit ));
}

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetDescUnit()' */
void DashboardGaugeDisplay_OnSetDescUnit( DashboardGaugeDisplay _this, XString value )
{
  if ( !EwCompString( _this->DescUnit, value ))
    return;

  _this->DescUnit = EwShareString( value );
  EwPostSignal( EwNewSlot( _this, DashboardGaugeDisplay_onOutletDesc ), ((XObject)_this ));
}

/* Variants derived from the class : 'Dashboard::GaugeDisplay' */
EW_DEFINE_CLASS_VARIANTS( DashboardGaugeDisplay )
EW_END_OF_CLASS_VARIANTS( DashboardGaugeDisplay )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeDisplay' */
EW_DEFINE_CLASS( DashboardGaugeDisplay, CoreGroup, Outlet, Outlet, Outlet, DescText, 
                 Unit, _None, "Dashboard::GaugeDisplay" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardGaugeDisplay )

/* Include a file containing the bitmap resource : 'Dashboard::StartEngineGloom' */
#include "_DashboardStartEngineGloom.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::StartEngineGloom' */
EW_RES_WITHOUT_VARIANTS( DashboardStartEngineGloom )

/* Include a file containing the bitmap resource : 'Dashboard::LeftBottomGloomV1' */
#include "_DashboardLeftBottomGloomV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftBottomGloomV1' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftBottomGloomV1 )

/* Include a file containing the bitmap resource : 'Dashboard::RightBottomGloomV1' */
#include "_DashboardRightBottomGloomV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightBottomGloomV1' */
EW_RES_WITHOUT_VARIANTS( DashboardRightBottomGloomV1 )

/* Include a file containing the bitmap resource : 'Dashboard::TopGloomV1' */
#include "_DashboardTopGloomV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::TopGloomV1' */
EW_RES_WITHOUT_VARIANTS( DashboardTopGloomV1 )

/* Initializer for the class 'Dashboard::DeviceClass' */
void DashboardDeviceClass__Init( DashboardDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardDeviceClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDeviceClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ODO = 267;
  _this->Battery = 67;
}

/* Re-Initializer for the class 'Dashboard::DeviceClass' */
void DashboardDeviceClass__ReInit( DashboardDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Dashboard::DeviceClass' */
void DashboardDeviceClass__Done( DashboardDeviceClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetSpeed()' */
void DashboardDeviceClass_OnSetSpeed( DashboardDeviceClass _this, XInt32 value )
{
  if ( _this->Speed == value )
    return;

  _this->Speed = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetEnergy()' */
void DashboardDeviceClass_OnSetEnergy( DashboardDeviceClass _this, XInt32 value )
{
  if ( _this->Energy == value )
    return;

  _this->Energy = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetODO()' */
void DashboardDeviceClass_OnSetODO( DashboardDeviceClass _this, XInt32 value )
{
  if ( _this->ODO == value )
    return;

  _this->ODO = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetODO, DashboardDeviceClass_OnSetODO ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetBattery()' */
void DashboardDeviceClass_OnSetBattery( DashboardDeviceClass _this, XInt32 value )
{
  if ( _this->Battery == value )
    return;

  _this->Battery = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed0()' */
void DashboardDeviceClass_OnSetLed0( DashboardDeviceClass _this, XBool value )
{
  if ( _this->Led0 == value )
    return;

  _this->Led0 = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed0, DashboardDeviceClass_OnSetLed0 ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed1()' */
void DashboardDeviceClass_OnSetLed1( DashboardDeviceClass _this, XBool value )
{
  if ( _this->Led1 == value )
    return;

  _this->Led1 = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed1, DashboardDeviceClass_OnSetLed1 ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed2()' */
void DashboardDeviceClass_OnSetLed2( DashboardDeviceClass _this, XBool value )
{
  if ( _this->Led2 == value )
    return;

  _this->Led2 = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed2, DashboardDeviceClass_OnSetLed2 ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed3()' */
void DashboardDeviceClass_OnSetLed3( DashboardDeviceClass _this, XBool value )
{
  if ( _this->Led3 == value )
    return;

  _this->Led3 = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed3, DashboardDeviceClass_OnSetLed3 ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed4()' */
void DashboardDeviceClass_OnSetLed4( DashboardDeviceClass _this, XBool value )
{
  if ( _this->Led4 == value )
    return;

  _this->Led4 = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed4, DashboardDeviceClass_OnSetLed4 ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetLed5()' */
void DashboardDeviceClass_OnSetLed5( DashboardDeviceClass _this, XBool value )
{
  if ( _this->Led5 == value )
    return;

  _this->Led5 = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed5, DashboardDeviceClass_OnSetLed5 ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetDemoMode()' */
void DashboardDeviceClass_OnSetDemoMode( DashboardDeviceClass _this, XBool value )
{
  if ( _this->DemoMode == value )
    return;

  _this->DemoMode = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetDemoMode, DashboardDeviceClass_OnSetDemoMode ), 
    0 );
}

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetAutoDemoMode()' */
void DashboardDeviceClass_OnSetAutoDemoMode( DashboardDeviceClass _this, XBool value )
{
  if ( _this->AutoDemoMode == value )
    return;

  _this->AutoDemoMode = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetAutoDemoMode, 
    DashboardDeviceClass_OnSetAutoDemoMode ), 0 );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateSpeed( DashboardDeviceClass _this, XInt32 aNewValue )
{
  if ( aNewValue != _this->Speed )
  {
    _this->Speed = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateSpeed()' */
void DashboardDeviceClass__UpdateSpeed( void* _this, XInt32 aNewValue )
{
  DashboardDeviceClass_UpdateSpeed((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateODO( DashboardDeviceClass _this, XInt32 aNewValue )
{
  if ( aNewValue != _this->ODO )
  {
    _this->ODO = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetODO, DashboardDeviceClass_OnSetODO ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateODO()' */
void DashboardDeviceClass__UpdateODO( void* _this, XInt32 aNewValue )
{
  DashboardDeviceClass_UpdateODO((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateEnergy( DashboardDeviceClass _this, XInt32 aNewValue )
{
  if ( aNewValue != _this->Energy )
  {
    _this->Energy = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateEnergy()' */
void DashboardDeviceClass__UpdateEnergy( void* _this, XInt32 aNewValue )
{
  DashboardDeviceClass_UpdateEnergy((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateBattery( DashboardDeviceClass _this, XInt32 aNewValue )
{
  if ( aNewValue != _this->Battery )
  {
    _this->Battery = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateBattery()' */
void DashboardDeviceClass__UpdateBattery( void* _this, XInt32 aNewValue )
{
  DashboardDeviceClass_UpdateBattery((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed0( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->Led0 )
  {
    _this->Led0 = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed0, DashboardDeviceClass_OnSetLed0 ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed0()' */
void DashboardDeviceClass__UpdateLed0( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateLed0((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed1( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->Led1 )
  {
    _this->Led1 = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed1, DashboardDeviceClass_OnSetLed1 ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed1()' */
void DashboardDeviceClass__UpdateLed1( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateLed1((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed2( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->Led2 )
  {
    _this->Led2 = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed2, DashboardDeviceClass_OnSetLed2 ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed2()' */
void DashboardDeviceClass__UpdateLed2( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateLed2((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed3( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->Led3 )
  {
    _this->Led3 = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed3, DashboardDeviceClass_OnSetLed3 ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed3()' */
void DashboardDeviceClass__UpdateLed3( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateLed3((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed4( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->Led4 )
  {
    _this->Led4 = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed4, DashboardDeviceClass_OnSetLed4 ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed4()' */
void DashboardDeviceClass__UpdateLed4( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateLed4((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateLed5( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->Led5 )
  {
    _this->Led5 = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetLed5, DashboardDeviceClass_OnSetLed5 ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateLed5()' */
void DashboardDeviceClass__UpdateLed5( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateLed5((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateDemoMode( DashboardDeviceClass _this, XBool aNewValue )
{
  if ( aNewValue != _this->DemoMode )
  {
    _this->DemoMode = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetDemoMode, DashboardDeviceClass_OnSetDemoMode ), 
      0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateDemoMode()' */
void DashboardDeviceClass__UpdateDemoMode( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateDemoMode((DashboardDeviceClass)_this, aNewValue );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateAutoDemoMode( DashboardDeviceClass _this, XBool 
  aNewValue )
{
  if ( aNewValue != _this->AutoDemoMode )
  {
    _this->AutoDemoMode = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetAutoDemoMode, 
      DashboardDeviceClass_OnSetAutoDemoMode ), 0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateAutoDemoMode()' */
void DashboardDeviceClass__UpdateAutoDemoMode( void* _this, XBool aNewValue )
{
  DashboardDeviceClass_UpdateAutoDemoMode((DashboardDeviceClass)_this, aNewValue );
}

/* Default onget method for the property 'Speed' */
XInt32 DashboardDeviceClass_OnGetSpeed( DashboardDeviceClass _this )
{
  return _this->Speed;
}

/* Default onget method for the property 'Energy' */
XInt32 DashboardDeviceClass_OnGetEnergy( DashboardDeviceClass _this )
{
  return _this->Energy;
}

/* Default onget method for the property 'ODO' */
XInt32 DashboardDeviceClass_OnGetODO( DashboardDeviceClass _this )
{
  return _this->ODO;
}

/* Default onget method for the property 'Battery' */
XInt32 DashboardDeviceClass_OnGetBattery( DashboardDeviceClass _this )
{
  return _this->Battery;
}

/* Default onget method for the property 'Led0' */
XBool DashboardDeviceClass_OnGetLed0( DashboardDeviceClass _this )
{
  return _this->Led0;
}

/* Default onget method for the property 'Led1' */
XBool DashboardDeviceClass_OnGetLed1( DashboardDeviceClass _this )
{
  return _this->Led1;
}

/* Default onget method for the property 'Led2' */
XBool DashboardDeviceClass_OnGetLed2( DashboardDeviceClass _this )
{
  return _this->Led2;
}

/* Default onget method for the property 'Led3' */
XBool DashboardDeviceClass_OnGetLed3( DashboardDeviceClass _this )
{
  return _this->Led3;
}

/* Default onget method for the property 'Led4' */
XBool DashboardDeviceClass_OnGetLed4( DashboardDeviceClass _this )
{
  return _this->Led4;
}

/* Default onget method for the property 'Led5' */
XBool DashboardDeviceClass_OnGetLed5( DashboardDeviceClass _this )
{
  return _this->Led5;
}

/* Default onget method for the property 'DemoMode' */
XBool DashboardDeviceClass_OnGetDemoMode( DashboardDeviceClass _this )
{
  return _this->DemoMode;
}

/* Default onget method for the property 'AutoDemoMode' */
XBool DashboardDeviceClass_OnGetAutoDemoMode( DashboardDeviceClass _this )
{
  return _this->AutoDemoMode;
}

/* Variants derived from the class : 'Dashboard::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( DashboardDeviceClass )
EW_END_OF_CLASS_VARIANTS( DashboardDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Dashboard::DeviceClass' */
EW_DEFINE_CLASS( DashboardDeviceClass, XObject, _None, _None, _None, _None, _None, 
                 _None, "Dashboard::DeviceClass" )
EW_END_OF_CLASS( DashboardDeviceClass )

/* User defined auto object: 'Dashboard::Device' */
EW_DEFINE_AUTOOBJECT( DashboardDevice, DashboardDeviceClass )

/* Initializer for the auto object 'Dashboard::Device' */
void DashboardDevice__Init( DashboardDeviceClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Dashboard::Device' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardDevice )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardDevice )

/* Initializer for the class 'Dashboard::Clock' */
void DashboardClock__Init( DashboardClock _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardClock );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->clockTimer, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Text, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardClock );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const002B );
  CoreTimer_OnSetPeriod( &_this->clockTimer, 30000 );
  CoreTimer_OnSetBegin( &_this->clockTimer, 1 );
  CoreTimer_OnSetEnabled( &_this->clockTimer, 1 );
  CoreRectView__OnSetBounds( &_this->Text, _Const002B );
  ViewsText_OnSetString( &_this->Text, EwLoadString( &_Const002C ));
  ViewsText_OnSetColor( &_this->Text, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Text ), 0 );
  _this->clockTimer.OnTrigger = EwNewSlot( _this, DashboardClock_onClockUpdate );
  ViewsText_OnSetFont( &_this->Text, EwLoadResource( &DashboardFontLightL, ResourcesFont ));
}

/* Re-Initializer for the class 'Dashboard::Clock' */
void DashboardClock__ReInit( DashboardClock _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->clockTimer );
  ViewsText__ReInit( &_this->Text );
}

/* Finalizer method for the class 'Dashboard::Clock' */
void DashboardClock__Done( DashboardClock _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->clockTimer );
  ViewsText__Done( &_this->Text );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::Clock.onClockUpdate()' */
void DashboardClock_onClockUpdate( DashboardClock _this, XObject sender )
{
  CoreTime currentTime;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  currentTime = CoreTime_OnGetCurrentTime( EwNewObject( CoreTime, 0 ));
  ViewsText_OnSetString( &_this->Text, CoreTime_Format( currentTime, EwLoadString( 
  &_Const002D )));
}

/* Variants derived from the class : 'Dashboard::Clock' */
EW_DEFINE_CLASS_VARIANTS( DashboardClock )
EW_END_OF_CLASS_VARIANTS( DashboardClock )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Clock' */
EW_DEFINE_CLASS( DashboardClock, CoreGroup, clockTimer, clockTimer, clockTimer, 
                 clockTimer, _None, _None, "Dashboard::Clock" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardClock )

/* Include a file containing the bitmap resource : 'Dashboard::Icons' */
#include "_DashboardIcons.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Icons' */
EW_RES_WITHOUT_VARIANTS( DashboardIcons )

/* Initializer for the class 'Dashboard::DashboardV2' */
void DashboardDashboardV2__Init( DashboardDashboardV2 _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardDashboardV2 );

  /* ... then construct all embedded objects */
  GraphicsPath__Init( &_this->rightPath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->leftPath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->leftGrowingPath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->rightGrowingPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->RightStrokePath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->LeftStrokePath, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftHousing, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->RightHousing, &_this->_XObject, 0 );
  DashboardVertValueBar__Init( &_this->EnergyBar, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->RightGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftGlow, &_this->_XObject, 0 );
  DashboardVertValueBar__Init( &_this->SpeedBar, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDashboardV2 );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0011 );
  CoreRectView__OnSetBounds( &_this->RightStrokePath, _Const0013 );
  ViewsStrokePath_OnSetColor( &_this->RightStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV2 );
  ViewsStrokePath_OnSetMiterLimit( &_this->RightStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->RightStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->RightStrokePath, GraphicsPathCapRound );
  CoreRectView__OnSetBounds( &_this->LeftStrokePath, _Const0012 );
  ViewsStrokePath_OnSetColor( &_this->LeftStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV2 );
  ViewsStrokePath_OnSetMiterLimit( &_this->LeftStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->LeftStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->LeftStrokePath, GraphicsPathCapRound );
  CoreRectView__OnSetBounds( &_this->LeftHousing, _Const002E );
  CoreRectView__OnSetBounds( &_this->RightHousing, _Const002F );
  CoreRectView__OnSetBounds( &_this->EnergyBar, _Const0030 );
  CoreRectView__OnSetBounds( &_this->RightGlow, _Const0031 );
  ViewsImage_OnSetColor( &_this->RightGlow, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeGlowV2 );
  CoreRectView__OnSetBounds( &_this->LeftGlow, _Const0032 );
  ViewsImage_OnSetColor( &_this->LeftGlow, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeGlowV2 );
  CoreRectView__OnSetBounds( &_this->SpeedBar, _Const0033 );
  DashboardVertValueBar_OnSetRightWing( &_this->SpeedBar, 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftHousing ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightHousing ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->EnergyBar ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedBar ), 0 );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightPath );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftPath );
  ViewsImage_OnSetBitmap( &_this->LeftHousing, EwLoadResource( &DashboardLeftWing2V2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->RightHousing, EwLoadResource( &DashboardRightWing2V2, 
  ResourcesBitmap ));
  DashboardVertValueBar_OnSetOutlet( &_this->EnergyBar, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy ));
  ViewsImage_OnSetBitmap( &_this->RightGlow, EwLoadResource( &DashboardRightGlowV2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->LeftGlow, EwLoadResource( &DashboardLeftGlowV2, 
  ResourcesBitmap ));
  DashboardVertValueBar_OnSetOutlet( &_this->SpeedBar, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ));

  /* Call the user defined constructor */
  DashboardDashboardV2_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::DashboardV2' */
void DashboardDashboardV2__ReInit( DashboardDashboardV2 _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  GraphicsPath__ReInit( &_this->rightPath );
  GraphicsPath__ReInit( &_this->leftPath );
  GraphicsPath__ReInit( &_this->leftGrowingPath );
  GraphicsPath__ReInit( &_this->rightGrowingPath );
  ViewsStrokePath__ReInit( &_this->RightStrokePath );
  ViewsStrokePath__ReInit( &_this->LeftStrokePath );
  ViewsImage__ReInit( &_this->LeftHousing );
  ViewsImage__ReInit( &_this->RightHousing );
  DashboardVertValueBar__ReInit( &_this->EnergyBar );
  ViewsImage__ReInit( &_this->RightGlow );
  ViewsImage__ReInit( &_this->LeftGlow );
  DashboardVertValueBar__ReInit( &_this->SpeedBar );
}

/* Finalizer method for the class 'Dashboard::DashboardV2' */
void DashboardDashboardV2__Done( DashboardDashboardV2 _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  GraphicsPath__Done( &_this->rightPath );
  GraphicsPath__Done( &_this->leftPath );
  GraphicsPath__Done( &_this->leftGrowingPath );
  GraphicsPath__Done( &_this->rightGrowingPath );
  ViewsStrokePath__Done( &_this->RightStrokePath );
  ViewsStrokePath__Done( &_this->LeftStrokePath );
  ViewsImage__Done( &_this->LeftHousing );
  ViewsImage__Done( &_this->RightHousing );
  DashboardVertValueBar__Done( &_this->EnergyBar );
  ViewsImage__Done( &_this->RightGlow );
  ViewsImage__Done( &_this->LeftGlow );
  DashboardVertValueBar__Done( &_this->SpeedBar );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void DashboardDashboardV2_UpdateViewState( DashboardDashboardV2 _this, XSet aState )
{
  XInt32 noOfEdges;
  XInt32 progress;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  ViewsImage_OnSetOpacity( &_this->LeftHousing, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.400000f, 
  0.600000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->RightHousing, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.400000f, 
  0.600000f, _this->CurrentProgress ));
  CoreRectView__OnSetBounds( &_this->SpeedBar, EwSetRectOrigin( _this->SpeedBar.Super3.Bounds, 
  DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), _Const0034, _Const0035, 0.300000f, 0.700000f, _this->CurrentProgress )));
  CoreRectView__OnSetBounds( &_this->EnergyBar, EwSetRectOrigin( _this->EnergyBar.Super3.Bounds, 
  DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), _Const0036, _Const0037, 0.300000f, 0.700000f, _this->CurrentProgress )));
  CoreRectView__OnSetBounds( &_this->LeftGlow, EwSetRectOrigin( _this->LeftGlow.Super1.Bounds, 
  DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), _Const0038, _Const0039, 0.300000f, 0.700000f, _this->CurrentProgress )));
  CoreRectView__OnSetBounds( &_this->RightGlow, EwSetRectOrigin( _this->RightGlow.Super1.Bounds, 
  DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), _Const003A, _Const003B, 0.300000f, 0.700000f, _this->CurrentProgress )));
  ViewsImage_OnSetOpacity( &_this->LeftGlow, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.600000f, 
  0.800000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->RightGlow, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.600000f, 
  0.800000f, _this->CurrentProgress ));
  noOfEdges = GraphicsPath_GetNoOfEdges( &_this->leftPath, 0 );
  progress = noOfEdges - DashboardAnimationClass_linearValueProgress( EwGetAutoObject( 
  &DashboardAnimation, DashboardAnimationClass ), 0, noOfEdges, 0.850000f, 1.000000f, 
  _this->CurrentProgress );
  GraphicsPath_InitSubPath( &_this->leftGrowingPath, 0, noOfEdges );
  GraphicsPath_AddCopy( &_this->leftGrowingPath, 0, &_this->leftPath, 0, progress, 
  noOfEdges - progress );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftGrowingPath );
  noOfEdges = GraphicsPath_GetNoOfEdges( &_this->rightPath, 0 );
  progress = DashboardAnimationClass_linearValueProgress( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), 0, noOfEdges, 0.850000f, 1.000000f, _this->CurrentProgress );
  GraphicsPath_InitSubPath( &_this->rightGrowingPath, 0, noOfEdges );
  GraphicsPath_AddCopy( &_this->rightGrowingPath, 0, &_this->rightPath, 0, 0, progress );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightGrowingPath );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardDashboardV2_Init( DashboardDashboardV2 _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwSignal( EwNewSlot( _this, DashboardDashboardV2_createPath ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::DashboardV2.createPath()' */
void DashboardDashboardV2_createPath( DashboardDashboardV2 _this, XObject sender )
{
  XString svg;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  svg = EwLoadString( &_Const003C );
  GraphicsPath_CreateFromSVGString( &_this->leftPath, svg, 1.000000f, 1.000000f, 
  243.500000f, 129.600006f, 0.000000f );
  svg = EwLoadString( &_Const003D );
  GraphicsPath_CreateFromSVGString( &_this->rightPath, svg, 1.000000f, 1.000000f, 
  -396.500000f, 129.600006f, 0.000000f );
}

/* 'C' function for method : 'Dashboard::DashboardV2.OnSetOutlet()' */
void DashboardDashboardV2_OnSetOutlet( DashboardDashboardV2 _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardDashboardV2_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardDashboardV2_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardDashboardV2_onOutlet ), ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardDashboardV2_onOutlet( DashboardDashboardV2 _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardDashboardV2_OnSetCurrentProgress( _this, EwOnGetFloat( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::DashboardV2.OnSetCurrentProgress()' */
void DashboardDashboardV2_OnSetCurrentProgress( DashboardDashboardV2 _this, XFloat 
  value )
{
  if ( _this->CurrentProgress == value )
    return;

  _this->CurrentProgress = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* Variants derived from the class : 'Dashboard::DashboardV2' */
EW_DEFINE_CLASS_VARIANTS( DashboardDashboardV2 )
EW_END_OF_CLASS_VARIANTS( DashboardDashboardV2 )

/* Virtual Method Table (VMT) for the class : 'Dashboard::DashboardV2' */
EW_DEFINE_CLASS( DashboardDashboardV2, CoreGroup, Outlet, Outlet, Outlet, rightPath, 
                 CurrentProgress, CurrentProgress, "Dashboard::DashboardV2" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  DashboardDashboardV2_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardDashboardV2 )

/* Include a file containing the bitmap resource : 'Dashboard::LeftWing2V2' */
#include "_DashboardLeftWing2V2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftWing2V2' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftWing2V2 )

/* Include a file containing the bitmap resource : 'Dashboard::LeftWingV2' */
#include "_DashboardLeftWingV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftWingV2' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftWingV2 )

/* Include a file containing the bitmap resource : 'Dashboard::RightWing2V2' */
#include "_DashboardRightWing2V2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightWing2V2' */
EW_RES_WITHOUT_VARIANTS( DashboardRightWing2V2 )

/* Include a file containing the bitmap resource : 'Dashboard::RightWingV2' */
#include "_DashboardRightWingV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightWingV2' */
EW_RES_WITHOUT_VARIANTS( DashboardRightWingV2 )

/* Initializer for the class 'Dashboard::VertValueBar' */
void DashboardVertValueBar__Init( DashboardVertValueBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  TemplatesVerticalValueBar__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardVertValueBar );

  /* ... then construct all embedded objects */
  GraphicsPath__Init( &_this->verticalValuePath, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->needlePath, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Wing, &_this->_XObject, 0 );
  ViewsFillPath__Init( &_this->ValueStrokePath, &_this->_XObject, 0 );
  ViewsFillPath__Init( &_this->NeedleStrokePath, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardVertValueBar );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const003E );
  _this->MaxValue = 100;
  _this->CurrentValue = 50;
  _this->RightWing = 1;
  CoreRectView__OnSetBounds( &_this->Wing, _Const003E );
  ViewsImage_OnSetColor( &_this->Wing, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->WingV2 );
  CoreRectView__OnSetBounds( &_this->ValueStrokePath, _Const003F );
  ViewsFillPath_OnSetColor( &_this->ValueStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV2 );
  ViewsFillPath_OnSetOffset( &_this->ValueStrokePath, _Const0040 );
  CoreRectView__OnSetBounds( &_this->NeedleStrokePath, _Const0041 );
  ViewsFillPath_OnSetColor( &_this->NeedleStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV2 );
  ViewsFillPath_OnSetOffset( &_this->NeedleStrokePath, _Const0040 );
  _this->increment = 10;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Wing ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ValueStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NeedleStrokePath ), 0 );
  ViewsImage_OnSetBitmap( &_this->Wing, EwLoadResource( &DashboardRightWingV2, ResourcesBitmap ));
  ViewsFillPath_OnSetPath( &_this->ValueStrokePath, &_this->verticalValuePath );
  ViewsFillPath_OnSetPath( &_this->NeedleStrokePath, &_this->needlePath );

  /* Call the user defined constructor */
  DashboardVertValueBar_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::VertValueBar' */
void DashboardVertValueBar__ReInit( DashboardVertValueBar _this )
{
  /* At first re-initialize the super class ... */
  TemplatesVerticalValueBar__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  GraphicsPath__ReInit( &_this->verticalValuePath );
  GraphicsPath__ReInit( &_this->needlePath );
  ViewsImage__ReInit( &_this->Wing );
  ViewsFillPath__ReInit( &_this->ValueStrokePath );
  ViewsFillPath__ReInit( &_this->NeedleStrokePath );
}

/* Finalizer method for the class 'Dashboard::VertValueBar' */
void DashboardVertValueBar__Done( DashboardVertValueBar _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesVerticalValueBar );

  /* Finalize all embedded objects */
  GraphicsPath__Done( &_this->verticalValuePath );
  GraphicsPath__Done( &_this->needlePath );
  ViewsImage__Done( &_this->Wing );
  ViewsFillPath__Done( &_this->ValueStrokePath );
  ViewsFillPath__Done( &_this->NeedleStrokePath );

  /* Don't forget to deinitialize the super class ... */
  TemplatesVerticalValueBar__Done( &_this->_Super );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void DashboardVertValueBar_UpdateViewState( DashboardVertValueBar _this, XSet aState )
{
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 oldPos;
  XInt32 newPos;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  minPos = 59;
  maxPos = EwGetRectH( _this->Super3.Bounds );
  oldPos = _this->ValueStrokePath.Super1.Bounds.Point1.Y;
  newPos = minPos;

  if ( _this->MaxValue != _this->MinValue )
    newPos = ((( -DashboardVertValueBar_OnGetCurrentValue( _this ) + _this->MaxValue ) 
    * ( maxPos - minPos )) / ( _this->MaxValue - _this->MinValue )) + minPos;

  if ( newPos != oldPos )
  {
    CoreRectView__OnSetBounds( &_this->ValueStrokePath, EwSetRectY1( _this->ValueStrokePath.Super1.Bounds, 
    newPos ));
    ViewsFillPath_OnSetOffset( &_this->ValueStrokePath, EwSetPointY( _this->ValueStrokePath.Offset, 
    -newPos + minPos ));

    if ( newPos == minPos )
      newPos = minPos + 3;

    CoreRectView__OnSetBounds( &_this->NeedleStrokePath, EwSetRectY1( _this->NeedleStrokePath.Super1.Bounds, 
    newPos - 3 ));
    CoreRectView__OnSetBounds( &_this->NeedleStrokePath, EwSetRectY2( _this->NeedleStrokePath.Super1.Bounds, 
    newPos ));
    ViewsFillPath_OnSetOffset( &_this->NeedleStrokePath, EwSetPointY( _this->NeedleStrokePath.Offset, 
    ( -newPos + 3 ) + minPos ));
  }
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardVertValueBar_Init( DashboardVertValueBar _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwSignal( EwNewSlot( _this, DashboardVertValueBar_setupLabels ), ((XObject)_this ));
  EwPostSignal( EwNewSlot( _this, DashboardVertValueBar_setup ), ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardVertValueBar_onOutlet( DashboardVertValueBar _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardVertValueBar_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::VertValueBar.OnSetOutlet()' */
void DashboardVertValueBar_OnSetOutlet( DashboardVertValueBar _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardVertValueBar_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardVertValueBar_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardVertValueBar_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::VertValueBar.OnGetCurrentValue()' */
XInt32 DashboardVertValueBar_OnGetCurrentValue( DashboardVertValueBar _this )
{
  XInt32 value = _this->CurrentValue;

  if ( _this->MinValue > _this->MaxValue )
  {
    if ( value < _this->MaxValue )
      value = _this->MaxValue;

    if ( value > _this->MinValue )
      value = _this->MinValue;
  }
  else
  {
    if ( value < _this->MinValue )
      value = _this->MinValue;

    if ( value > _this->MaxValue )
      value = _this->MaxValue;
  }

  return value;
}

/* 'C' function for method : 'Dashboard::VertValueBar.OnSetCurrentValue()' */
void DashboardVertValueBar_OnSetCurrentValue( DashboardVertValueBar _this, XInt32 
  value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Dashboard::VertValueBar.setup()' */
void DashboardVertValueBar_setup( DashboardVertValueBar _this, XObject sender )
{
  XString valuePathString;
  XString needlePathString;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->RightWing )
  {
    valuePathString = EwLoadString( &_Const0042 );
    needlePathString = EwLoadString( &_Const0043 );
    CoreRectView__OnSetBounds( &_this->ValueStrokePath, EwSetRectX( _this->ValueStrokePath.Super1.Bounds, 
    198 ));
    CoreRectView__OnSetBounds( &_this->NeedleStrokePath, EwSetRectX( _this->NeedleStrokePath.Super1.Bounds, 
    _this->ValueStrokePath.Super1.Bounds.Point1.X - 8 ));
    ViewsImage_OnSetBitmap( &_this->Wing, EwLoadResource( &DashboardRightWingV2, 
    ResourcesBitmap ));
  }
  else
  {
    valuePathString = EwLoadString( &_Const0044 );
    needlePathString = EwLoadString( &_Const0045 );
    CoreRectView__OnSetBounds( &_this->ValueStrokePath, EwSetRectX( _this->ValueStrokePath.Super1.Bounds, 
    0 ));
    CoreRectView__OnSetBounds( &_this->NeedleStrokePath, EwSetRectX( _this->NeedleStrokePath.Super1.Bounds, 
    _this->ValueStrokePath.Super1.Bounds.Point1.X ));
    ViewsImage_OnSetBitmap( &_this->Wing, EwLoadResource( &DashboardLeftWingV2, 
    ResourcesBitmap ));
  }

  GraphicsPath_CreateFromSVGString( &_this->verticalValuePath, valuePathString, 
  1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f );
  GraphicsPath_CreateFromSVGString( &_this->needlePath, needlePathString, 1.000000f, 
  1.000000f, 0.000000f, 0.000000f, 0.000000f );
  EwPostSignal( EwNewSlot( _this, DashboardVertValueBar_adjustLabels ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::VertValueBar.OnSetRightWing()' */
void DashboardVertValueBar_OnSetRightWing( DashboardVertValueBar _this, XBool value )
{
  if ( _this->RightWing == value )
    return;

  _this->RightWing = value;
  EwPostSignal( EwNewSlot( _this, DashboardVertValueBar_setup ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::VertValueBar.setupLabels()' */
void DashboardVertValueBar_setupLabels( DashboardVertValueBar _this, XObject sender )
{
  XInt32 i;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  i = 0;

  while ( i < 10 )
  {
    ViewsText txt = EwNewObject( ViewsText, 0 );

    if (( i % 2 ) == 0 )
      ViewsText_OnSetFont( txt, EwLoadResource( &DashboardFontLightM, ResourcesFont ));
    else
      ViewsText_OnSetFont( txt, EwLoadResource( &DashboardFontL, ResourcesFont ));

    ViewsText_OnSetColor( txt, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
    ViewsText_OnSetAutoSize( txt, 1 );
    _this->label[ EwCheckIndex( i, 10 )] = txt;
    i = i + 1;
  }
}

/* 'C' function for method : 'Dashboard::VertValueBar.adjustLabels()' */
void DashboardVertValueBar_adjustLabels( DashboardVertValueBar _this, XObject sender )
{
  /* List of temporarily used variables */
  void* _tmpO0;
  XInt32 xoffset;
  XInt32 yoffset;
  XInt32 direction;
  XInt32 i;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  xoffset = 115;
  yoffset = 56;
  direction = -1;

  if ( _this->RightWing )
  {
    xoffset = 290;
    direction = 1;
  }

  i = 0;

  while ( i < 10 )
  {
    ViewsText_OnSetString( _this->label[ EwCheckIndex( i, 10 )], EwNewStringInt(( 
    10 - i ) * _this->increment, 0, 10 ));

    if ( i < 4 )
      ( _tmpO0 = _this->label[ EwCheckIndex( i, 10 )], CoreRectView__OnSetBounds( 
      _tmpO0, EwSetRectOrigin(((ViewsText)_tmpO0)->Super1.Bounds, EwNewPoint( xoffset 
      + (( i * 18 ) * direction ), yoffset + ( i * 57 )))));
    else
      ( _tmpO0 = _this->label[ EwCheckIndex( i, 10 )], CoreRectView__OnSetBounds( 
      _tmpO0, EwSetRectOrigin(((ViewsText)_tmpO0)->Super1.Bounds, EwNewPoint(( xoffset 
      + ( 108 * direction )) + (( i * 17 ) * -direction ), yoffset + ( i * 57 )))));

    if ( _this->label[ EwCheckIndex( i, 10 )]->Super2.Owner == 0 )
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->label[ EwCheckIndex( i, 
      10 )]), 0 );

    i = i + 1;
  }
}

/* Variants derived from the class : 'Dashboard::VertValueBar' */
EW_DEFINE_CLASS_VARIANTS( DashboardVertValueBar )
EW_END_OF_CLASS_VARIANTS( DashboardVertValueBar )

/* Virtual Method Table (VMT) for the class : 'Dashboard::VertValueBar' */
EW_DEFINE_CLASS( DashboardVertValueBar, TemplatesVerticalValueBar, label, Outlet, 
                 Outlet, verticalValuePath, MaxValue, MaxValue, "Dashboard::VertValueBar" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  DashboardVertValueBar_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardVertValueBar )

/* Include a file containing the bitmap resource : 'Dashboard::LeftGlowV2' */
#include "_DashboardLeftGlowV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftGlowV2' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftGlowV2 )

/* Include a file containing the bitmap resource : 'Dashboard::RightGlowV2' */
#include "_DashboardRightGlowV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightGlowV2' */
EW_RES_WITHOUT_VARIANTS( DashboardRightGlowV2 )

/* Initializer for the class 'Dashboard::Battery' */
void DashboardBattery__Init( DashboardBattery _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  TemplatesHorizontalValueBar__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardBattery );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Image, &_this->_XObject, 0 );
  ViewsFrame__Init( &_this->Track, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardBattery );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0046 );
  _this->MaxValue = 100;
  _this->CurrentValue = 100;
  CoreView_OnSetLayout((CoreView)&_this->Image, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Image, _Const0046 );
  ViewsImage_OnSetColor( &_this->Image, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryFrame );
  ViewsImage_OnSetAlignment( &_this->Image, ViewsImageAlignmentAlignHorzCenter | 
  ViewsImageAlignmentAlignVertCenter | ViewsImageAlignmentStretchToFill );
  CoreRectView__OnSetBounds( &_this->Track, _Const0047 );
  ViewsFrame_OnSetColor( &_this->Track, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1 );
  _this->TrackColor = EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Track ), 0 );
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &DashboardBatteryFrame, 
  ResourcesBitmap ));
  ViewsFrame_OnSetBitmap( &_this->Track, EwLoadResource( &DashboardBatteryEnergy, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::Battery' */
void DashboardBattery__ReInit( DashboardBattery _this )
{
  /* At first re-initialize the super class ... */
  TemplatesHorizontalValueBar__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Image );
  ViewsFrame__ReInit( &_this->Track );
}

/* Finalizer method for the class 'Dashboard::Battery' */
void DashboardBattery__Done( DashboardBattery _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesHorizontalValueBar );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Image );
  ViewsFrame__Done( &_this->Track );

  /* Don't forget to deinitialize the super class ... */
  TemplatesHorizontalValueBar__Done( &_this->_Super );
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void DashboardBattery_UpdateViewState( DashboardBattery _this, XSet aState )
{
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 oldPos;
  XInt32 newPos;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  minPos = _this->Image.Super1.Bounds.Point1.X + 4;
  maxPos = _this->Image.Super1.Bounds.Point2.X - 10;
  oldPos = _this->Track.Super1.Bounds.Point2.X;
  newPos = minPos;

  if ( _this->MaxValue != _this->MinValue )
    newPos = ((( DashboardBattery_OnGetCurrentValue( _this ) - _this->MinValue ) 
    * ( maxPos - minPos )) / ( _this->MaxValue - _this->MinValue )) + minPos;

  if ( newPos != oldPos )
  {
    CoreRectView__OnSetBounds( &_this->Track, EwSetRectX2( _this->Track.Super1.Bounds, 
    newPos ));
  }
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardBattery_onOutlet( DashboardBattery _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardBattery_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::Battery.OnSetOutlet()' */
void DashboardBattery_OnSetOutlet( DashboardBattery _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardBattery_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardBattery_onOutlet ), value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardBattery_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Battery.OnGetCurrentValue()' */
XInt32 DashboardBattery_OnGetCurrentValue( DashboardBattery _this )
{
  XInt32 value = _this->CurrentValue;

  if ( _this->MinValue > _this->MaxValue )
  {
    if ( value < _this->MaxValue )
      value = _this->MaxValue;

    if ( value > _this->MinValue )
      value = _this->MinValue;
  }
  else
  {
    if ( value < _this->MinValue )
      value = _this->MinValue;

    if ( value > _this->MaxValue )
      value = _this->MaxValue;
  }

  return value;
}

/* 'C' function for method : 'Dashboard::Battery.OnSetCurrentValue()' */
void DashboardBattery_OnSetCurrentValue( DashboardBattery _this, XInt32 value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Dashboard::Battery.OnSetTrackColor()' */
void DashboardBattery_OnSetTrackColor( DashboardBattery _this, XColor value )
{
  if ( !EwCompColor( _this->TrackColor, value ))
    return;

  _this->TrackColor = value;
  ViewsFrame_OnSetColor( &_this->Track, value );
}

/* Variants derived from the class : 'Dashboard::Battery' */
EW_DEFINE_CLASS_VARIANTS( DashboardBattery )
EW_END_OF_CLASS_VARIANTS( DashboardBattery )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Battery' */
EW_DEFINE_CLASS( DashboardBattery, TemplatesHorizontalValueBar, Outlet, Outlet, 
                 Outlet, Image, MaxValue, MaxValue, "Dashboard::Battery" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  DashboardBattery_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardBattery )

/* Include a file containing the bitmap resource : 'Dashboard::BatteryFrame' */
#include "_DashboardBatteryFrame.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::BatteryFrame' */
EW_RES_WITHOUT_VARIANTS( DashboardBatteryFrame )

/* Include a file containing the bitmap resource : 'Dashboard::BatteryEnergy' */
#include "_DashboardBatteryEnergy.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::BatteryEnergy' */
EW_RES_WITHOUT_VARIANTS( DashboardBatteryEnergy )

/* Include a file containing the bitmap resource : 'Dashboard::GaugeNeedle' */
#include "_DashboardGaugeNeedle.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::GaugeNeedle' */
EW_RES_WITHOUT_VARIANTS( DashboardGaugeNeedle )

/* Initializer for the class 'Dashboard::Navigation' */
void DashboardNavigation__Init( DashboardNavigation _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardNavigation );

  /* ... then construct all embedded objects */
  EffectsPointEffect__Init( &_this->moveEffect, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->hintTimer, &_this->_XObject, 0 );
  ViewsWallpaper__Init( &_this->Map, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Fading, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Position, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardNavigation );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0048 );
  _this->currentWaypoint = 4;
  EffectsEffect_OnSetTimingCustom2((EffectsEffect)&_this->moveEffect, 0.500000f );
  EffectsEffect_OnSetTimingCustom1((EffectsEffect)&_this->moveEffect, -0.500000f );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->moveEffect, EffectsTimingCustom );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->moveEffect, 1 );
  _this->waypoints[ 0 ] = _Const0049;
  _this->waypoints[ 1 ] = _Const004A;
  _this->waypoints[ 2 ] = _Const004B;
  _this->waypoints[ 3 ] = _Const004C;
  _this->waypoints[ 4 ] = _Const004D;
  _this->waypoints[ 5 ] = _Const004E;
  _this->waypoints[ 6 ] = _Const004F;
  _this->waypoints[ 7 ] = _Const0050;
  _this->waypoints[ 8 ] = _Const0051;
  _this->waypoints[ 9 ] = _Const0052;
  _this->waypoints[ 10 ] = _Const0053;
  _this->PositionColor = EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1;
  _this->speedFactor = 40.000000f;
  CoreRectView__OnSetBounds( &_this->Map, _this->Super2.Bounds );
  CoreRectView__OnSetBounds( &_this->Fading, _Const0048 );
  ViewsImage_OnSetColor( &_this->Fading, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Background );
  CoreRectView__OnSetBounds( &_this->Position, _Const0054 );
  ViewsImage_OnSetColor( &_this->Position, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Map ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Fading ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Position ), 0 );
  _this->moveEffect.Super1.OnFinished = EwNewSlot( _this, DashboardNavigation_onMoveEffectFinished );
  _this->moveEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardNavigation_onMoveEffect );
  ViewsWallpaper_OnSetBitmap( &_this->Map, EwLoadResource( &DashboardMap, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Fading, EwLoadResource( &DashboardMapFading, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Position, EwLoadResource( &DashboardPosition, 
  ResourcesBitmap ));

  /* Call the user defined constructor */
  DashboardNavigation_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::Navigation' */
void DashboardNavigation__ReInit( DashboardNavigation _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsPointEffect__ReInit( &_this->moveEffect );
  CoreTimer__ReInit( &_this->hintTimer );
  ViewsWallpaper__ReInit( &_this->Map );
  ViewsImage__ReInit( &_this->Fading );
  ViewsImage__ReInit( &_this->Position );
}

/* Finalizer method for the class 'Dashboard::Navigation' */
void DashboardNavigation__Done( DashboardNavigation _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsPointEffect__Done( &_this->moveEffect );
  CoreTimer__Done( &_this->hintTimer );
  ViewsWallpaper__Done( &_this->Map );
  ViewsImage__Done( &_this->Fading );
  ViewsImage__Done( &_this->Position );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardNavigation_Init( DashboardNavigation _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwAttachRefObserver( EwNewSlot( _this, DashboardNavigation_onDemoModeChanged ), 
    EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetDemoMode, 
    DashboardDeviceClass_OnSetDemoMode ), 0 );
  EwPostSignal( EwNewSlot( _this, DashboardNavigation_onDemoModeChanged ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Navigation.OnSetOpacity()' */
void DashboardNavigation_OnSetOpacity( DashboardNavigation _this, XInt32 value )
{
  CoreGroup_OnSetOpacity((CoreGroup)_this, value );
  ViewsWallpaper_OnSetAlphaBlended( &_this->Map, (XBool)( value != 255 ));
}

/* 'C' function for method : 'Dashboard::Navigation.startNavigationCycle()' */
void DashboardNavigation_startNavigationCycle( DashboardNavigation _this, XObject 
  sender )
{
  XPoint nextWp;
  XFloat distance;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwSignal( EwNewSlot( &_this->moveEffect, EffectsEffect_StopEffect ), ((XObject)_this ));
  nextWp = EwMovePointNeg( _this->waypoints[ EwCheckIndex( _this->currentWaypoint 
  + 1, 11 )], EwGetRectCenter( _this->Position.Super1.Bounds ));
  _this->moveEffect.Value1 = EwMovePointNeg( _this->waypoints[ EwCheckIndex( _this->currentWaypoint, 
  11 )], EwGetRectCenter( _this->Position.Super1.Bounds ));
  _this->moveEffect.Value2 = nextWp;
  distance = DashboardNavigation_calculateDistance( _this, _this->waypoints[ EwCheckIndex( 
  _this->currentWaypoint, 11 )], _this->waypoints[ EwCheckIndex( _this->currentWaypoint 
  + 1, 11 )]);
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->moveEffect, (XInt32)( 
  distance * _this->speedFactor ));

  if ( _this->Hints != 0 )
  {
    XPoint nextNextWp;
    XFloat length;
    XInt32 direction;
    distance = DashboardNavigation_calculateDistance( _this, _this->waypoints[ EwCheckIndex( 
    _this->currentWaypoint + 1, 11 )], _this->waypoints[ EwCheckIndex(( _this->currentWaypoint 
    + 2 ) % 11, 11 )]);
    nextNextWp = EwMovePointNeg( _this->waypoints[ EwCheckIndex(( _this->currentWaypoint 
    + 2 ) % 11, 11 )], EwGetRectCenter( _this->Position.Super1.Bounds ));
    length = (XFloat)( nextNextWp.X - nextWp.X );
    direction = (XInt32)EwMathArcSin( length / distance );
    DashboardNavigationHints_OnSetDirection( _this->Hints, direction );
  }

  EwSignal( EwNewSlot( &_this->moveEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Navigation.onMoveEffectFinished()' */
void DashboardNavigation_onMoveEffectFinished( DashboardNavigation _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwSignal( EwNewSlot( _this, DashboardNavigation_onMoveEffect ), ((XObject)_this ));
  _this->currentWaypoint = ( _this->currentWaypoint + 1 ) % 10;
  EwPostSignal( EwNewSlot( _this, DashboardNavigation_startNavigationCycle ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Navigation.calculateDistance()' */
XFloat DashboardNavigation_calculateDistance( DashboardNavigation _this, XPoint 
  aPoint1, XPoint aPoint2 )
{
  XInt32 a;
  XInt32 b;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  a = aPoint2.X - aPoint1.X;
  b = aPoint2.Y - aPoint1.Y;
  return EwMathSqrt((XFloat)(( a * a ) + ( b * b )));
}

/* 'C' function for method : 'Dashboard::Navigation.onMoveEffect()' */
void DashboardNavigation_onMoveEffect( DashboardNavigation _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsWallpaper_OnSetScrollOffset( &_this->Map, EwNegPoint( _this->moveEffect.Value ));

  if ( _this->Hints != 0 )
  {
    XUInt32 currentTime = CoreTimer_GetCurrentTime( &_this->hintTimer );

    if ( _this->lastHintUpdate == 0 )
      _this->lastHintUpdate = currentTime;

    if (( currentTime - _this->lastHintUpdate ) > 150 )
    {
      DashboardNavigationHints_OnSetDistance( _this->Hints, (XInt32)DashboardNavigation_calculateDistance( 
      _this, EwMovePointPos( _this->moveEffect.Value, EwGetRectCenter( _this->Position.Super1.Bounds )), 
      _this->waypoints[ EwCheckIndex( _this->currentWaypoint + 1, 11 )]));
      _this->lastHintUpdate = currentTime;
    }
  }
}

/* 'C' function for method : 'Dashboard::Navigation.OnSetPositionColor()' */
void DashboardNavigation_OnSetPositionColor( DashboardNavigation _this, XColor value )
{
  if ( !EwCompColor( _this->PositionColor, value ))
    return;

  _this->PositionColor = value;
  ViewsImage_OnSetColor( &_this->Position, value );
}

/* 'C' function for method : 'Dashboard::Navigation.onDemoModeChanged()' */
void DashboardNavigation_onDemoModeChanged( DashboardNavigation _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass )->DemoMode )
    EwSignal( EwNewSlot( _this, DashboardNavigation_startNavigationCycle ), ((XObject)_this ));
  else
    EwSignal( EwNewSlot( &_this->moveEffect, EffectsEffect_StopEffect ), ((XObject)_this ));
}

/* Variants derived from the class : 'Dashboard::Navigation' */
EW_DEFINE_CLASS_VARIANTS( DashboardNavigation )
EW_END_OF_CLASS_VARIANTS( DashboardNavigation )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Navigation' */
EW_DEFINE_CLASS( DashboardNavigation, CoreGroup, Hints, moveEffect, moveEffect, 
                 moveEffect, currentWaypoint, currentWaypoint, "Dashboard::Navigation" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  DashboardNavigation_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardNavigation )

/* Initializer for the class 'Dashboard::Gauge' */
void DashboardGauge__Init( DashboardGauge _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardGauge );

  /* ... then construct all embedded objects */
  GraphicsArcPath__Init( &_this->background, &_this->_XObject, 0 );
  ViewsFillPath__Init( &_this->Bg, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle5, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle4, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle3, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle2, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle1, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle0, &_this->_XObject, 0 );
  DashboardGaugeLabels__Init( &_this->GaugeLabels, &_this->_XObject, 0 );
  DashboardGaugeIndicator__Init( &_this->GaugeIndicator, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGauge );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0055 );
  _this->Clockwise = 1;
  GraphicsArcPath_OnSetRadius( &_this->background, 235.000000f );
  GraphicsArcPath_OnSetStyle( &_this->background, GraphicsArcStyleArc );
  CoreRectView__OnSetBounds( &_this->Bg, _this->Super2.Bounds );
  ViewsFillPath_OnSetColor( &_this->Bg, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Background );
  ViewsFillPath_OnSetOffset( &_this->Bg, _Const0056 );
  CoreRectView__OnSetBounds( &_this->Circle5, _Const0057 );
  ViewsImage_OnSetColor( &_this->Circle5, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle5 );
  CoreRectView__OnSetBounds( &_this->Circle4, _Const0058 );
  ViewsImage_OnSetColor( &_this->Circle4, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle4 );
  CoreRectView__OnSetBounds( &_this->Circle3, _Const0059 );
  ViewsImage_OnSetColor( &_this->Circle3, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle3 );
  CoreRectView__OnSetBounds( &_this->Circle2, _Const005A );
  ViewsImage_OnSetColor( &_this->Circle2, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle2 );
  CoreRectView__OnSetBounds( &_this->Circle1, _Const005B );
  ViewsImage_OnSetColor( &_this->Circle1, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle1 );
  CoreRectView__OnSetBounds( &_this->Circle0, _Const005C );
  ViewsImage_OnSetColor( &_this->Circle0, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle0 );
  CoreRectView__OnSetBounds( &_this->GaugeLabels, _Const0055 );
  CoreRectView__OnSetBounds( &_this->GaugeIndicator, _Const005D );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Bg ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle0 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->GaugeLabels ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->GaugeIndicator ), 0 );
  ViewsFillPath_OnSetPath( &_this->Bg, ((GraphicsPath)&_this->background ));
  ViewsImage_OnSetBitmap( &_this->Circle5, EwLoadResource( &DashboardCircle5, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle4, EwLoadResource( &DashboardCircle4, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle3, EwLoadResource( &DashboardCircle3, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle2, EwLoadResource( &DashboardCircle2, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle1, EwLoadResource( &DashboardCircle1, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle0, EwLoadResource( &DashboardCircle0, ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::Gauge' */
void DashboardGauge__ReInit( DashboardGauge _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  GraphicsArcPath__ReInit( &_this->background );
  ViewsFillPath__ReInit( &_this->Bg );
  ViewsImage__ReInit( &_this->Circle5 );
  ViewsImage__ReInit( &_this->Circle4 );
  ViewsImage__ReInit( &_this->Circle3 );
  ViewsImage__ReInit( &_this->Circle2 );
  ViewsImage__ReInit( &_this->Circle1 );
  ViewsImage__ReInit( &_this->Circle0 );
  DashboardGaugeLabels__ReInit( &_this->GaugeLabels );
  DashboardGaugeIndicator__ReInit( &_this->GaugeIndicator );
}

/* Finalizer method for the class 'Dashboard::Gauge' */
void DashboardGauge__Done( DashboardGauge _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  GraphicsArcPath__Done( &_this->background );
  ViewsFillPath__Done( &_this->Bg );
  ViewsImage__Done( &_this->Circle5 );
  ViewsImage__Done( &_this->Circle4 );
  ViewsImage__Done( &_this->Circle3 );
  ViewsImage__Done( &_this->Circle2 );
  ViewsImage__Done( &_this->Circle1 );
  ViewsImage__Done( &_this->Circle0 );
  DashboardGaugeLabels__Done( &_this->GaugeLabels );
  DashboardGaugeIndicator__Done( &_this->GaugeIndicator );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::Gauge.OnSetOutlet()' */
void DashboardGauge_OnSetOutlet( DashboardGauge _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  _this->Outlet = value;
  DashboardGaugeLabels_OnSetOutlet( &_this->GaugeLabels, value );
  DashboardGaugeIndicator_OnSetOutlet( &_this->GaugeIndicator, value );
}

/* 'C' function for method : 'Dashboard::Gauge.OnSetClockwise()' */
void DashboardGauge_OnSetClockwise( DashboardGauge _this, XBool value )
{
  if ( _this->Clockwise == value )
    return;

  _this->Clockwise = value;
  DashboardGaugeLabels_OnSetClockwise( &_this->GaugeLabels, _this->Clockwise );

  if ( _this->Clockwise )
  {
    DashboardGaugeIndicator_OnSetStartAngle( &_this->GaugeIndicator, 180.000000f );
    DashboardGaugeIndicator_OnSetEndAngle( &_this->GaugeIndicator, 405.000000f );
  }
  else
  {
    DashboardGaugeIndicator_OnSetStartAngle( &_this->GaugeIndicator, 540.000000f );
    DashboardGaugeIndicator_OnSetEndAngle( &_this->GaugeIndicator, 315.000000f );
  }
}

/* 'C' function for method : 'Dashboard::Gauge.OnSetOpacityLabels()' */
void DashboardGauge_OnSetOpacityLabels( DashboardGauge _this, XUInt8 value )
{
  _this->OpacityLabels = value;
  CoreGroup__OnSetOpacity( &_this->GaugeLabels, value );
}

/* 'C' function for method : 'Dashboard::Gauge.OnSetOpacityGauge()' */
void DashboardGauge_OnSetOpacityGauge( DashboardGauge _this, XUInt8 value )
{
  _this->OpacityGauge = value;
  CoreGroup__OnSetOpacity( &_this->GaugeIndicator, value );
  ViewsImage_OnSetOpacity( &_this->Circle0, value );
  ViewsImage_OnSetOpacity( &_this->Circle1, value );
  ViewsImage_OnSetOpacity( &_this->Circle2, value );
  ViewsImage_OnSetOpacity( &_this->Circle3, value );
  ViewsImage_OnSetOpacity( &_this->Circle4, value );
  ViewsImage_OnSetOpacity( &_this->Circle5, value );
  ViewsFillPath_OnSetColor( &_this->Bg, EwNewColor( EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Background.Red, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Background.Green, 
  EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Background.Blue, value ));
}

/* Variants derived from the class : 'Dashboard::Gauge' */
EW_DEFINE_CLASS_VARIANTS( DashboardGauge )
EW_END_OF_CLASS_VARIANTS( DashboardGauge )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Gauge' */
EW_DEFINE_CLASS( DashboardGauge, CoreGroup, Outlet, Outlet, Outlet, background, 
                 OpacityGauge, OpacityGauge, "Dashboard::Gauge" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardGauge )

/* Include a file containing the bitmap resource : 'Dashboard::Circle0' */
#include "_DashboardCircle0.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Circle0' */
EW_RES_WITHOUT_VARIANTS( DashboardCircle0 )

/* Include a file containing the bitmap resource : 'Dashboard::Circle1' */
#include "_DashboardCircle1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Circle1' */
EW_RES_WITHOUT_VARIANTS( DashboardCircle1 )

/* Include a file containing the bitmap resource : 'Dashboard::Circle2' */
#include "_DashboardCircle2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Circle2' */
EW_RES_WITHOUT_VARIANTS( DashboardCircle2 )

/* Include a file containing the bitmap resource : 'Dashboard::Circle3' */
#include "_DashboardCircle3.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Circle3' */
EW_RES_WITHOUT_VARIANTS( DashboardCircle3 )

/* Include a file containing the bitmap resource : 'Dashboard::Circle4' */
#include "_DashboardCircle4.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Circle4' */
EW_RES_WITHOUT_VARIANTS( DashboardCircle4 )

/* Include a file containing the bitmap resource : 'Dashboard::Circle5' */
#include "_DashboardCircle5.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Circle5' */
EW_RES_WITHOUT_VARIANTS( DashboardCircle5 )

/* Initializer for the class 'Dashboard::Dashboard' */
void DashboardDashboard__Init( DashboardDashboard _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardDashboard );

  /* ... then construct all embedded objects */
  EffectsFloatEffect__Init( &_this->v1Effect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->ledOnEffect, &_this->_XObject, 0 );
  EffectsFloatEffect__Init( &_this->transitionEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->navigationEffect, &_this->_XObject, 0 );
  EffectsFloatEffect__Init( &_this->shutdownVariantEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->shutdownNavigationEffect, &_this->_XObject, 0 );
  EffectsFloatEffect__Init( &_this->gaugeDisplayEffect, &_this->_XObject, 0 );
  EffectsFloatEffect__Init( &_this->shutdownGaugeDisplayEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->gaugeCheckEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->ledOffEffect, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  DashboardNavigation__Init( &_this->Navigation, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->ShutdownButton, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->TransitionButton, &_this->_XObject, 0 );
  DashboardDashboardV2__Init( &_this->DashboardV2, &_this->_XObject, 0 );
  DashboardDashboardV1__Init( &_this->DashboardV1, &_this->_XObject, 0 );
  DashboardNavigationHints__Init( &_this->NavigationHints, &_this->_XObject, 0 );
  DashboardClock__Init( &_this->Clock, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Munich, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Reach, &_this->_XObject, 0 );
  DashboardBattery__Init( &_this->Battery, &_this->_XObject, 0 );
  DashboardGaugeDisplay__Init( &_this->EnergyDisplay, &_this->_XObject, 0 );
  ViewsWarpGroup__Init( &_this->EnergyWarpGroup, &_this->_XObject, 0 );
  DashboardGaugeDisplay__Init( &_this->SpeedDisplay, &_this->_XObject, 0 );
  ViewsWarpGroup__Init( &_this->SpeedWarpGroup, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->LedButton, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->LedButton1, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->LedButton2, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->LedButton3, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->LedButton4, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->LedButton5, &_this->_XObject, 0 );
  WidgetSetValueDisplay__Init( &_this->RangeDisplay, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->autoDemoModeTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDashboard );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0011 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->v1Effect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->v1Effect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->v1Effect, 3000 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->ledOnEffect, EffectsTimingLinear );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->ledOnEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->ledOnEffect, 1500 );
  _this->ledOnEffect.Value2 = 3;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->transitionEffect, EffectsTimingEaseIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->transitionEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->transitionEffect, 3000 );
  _this->progressReverse = 100.000000f;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->navigationEffect, EffectsTimingExp_In );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->navigationEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->navigationEffect, 2000 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->shutdownVariantEffect, EffectsTimingEaseIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->shutdownVariantEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->shutdownVariantEffect, 
  3000 );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->shutdownNavigationEffect, EffectsTimingExp_In );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->shutdownNavigationEffect, 
  1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->shutdownNavigationEffect, 
  1500 );
  _this->shutdownNavigationEffect.Value2 = 0;
  _this->shutdownNavigationEffect.Value1 = 255;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->gaugeDisplayEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->gaugeDisplayEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->gaugeDisplayEffect, 750 );
  _this->gaugeDisplayEffect.Value2 = 255.000000f;
  _this->gaugeDisplayEffect.Value1 = 0.000000f;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->shutdownGaugeDisplayEffect, EffectsTimingEaseIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->shutdownGaugeDisplayEffect, 
  1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->shutdownGaugeDisplayEffect, 
  750 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->shutdownGaugeDisplayEffect, 
  2250 );
  _this->shutdownGaugeDisplayEffect.Value2 = 0.000000f;
  _this->shutdownGaugeDisplayEffect.Value1 = 255.000000f;
  _this->gaugeCheckEffect.Super1.Symmetric = 1;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->gaugeCheckEffect, EffectsTimingEaseIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->gaugeCheckEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->gaugeCheckEffect, 1500 );
  _this->gaugeCheckEffect.Value2 = 100;
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->ledOffEffect, EffectsTimingLinear );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->ledOffEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->ledOffEffect, 1500 );
  _this->ledOffEffect.Value2 = 0;
  _this->ledOffEffect.Value1 = 3;
  CoreRectView__OnSetBounds( &_this->Background, _Const0011 );
  ViewsRectangle_OnSetColor( &_this->Background, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Background );
  CoreRectView__OnSetBounds( &_this->Navigation, _Const005E );
  CoreGroup__OnSetOpacity( &_this->Navigation, 0 );
  CoreRectView__OnSetBounds( &_this->ShutdownButton, _Const005F );
  WidgetSetPushButton_OnSetLabel( &_this->ShutdownButton, 0 );
  CoreRectView__OnSetBounds( &_this->TransitionButton, _Const0060 );
  WidgetSetPushButton_OnSetLabel( &_this->TransitionButton, 0 );
  CoreRectView__OnSetBounds( &_this->DashboardV2, _Const0011 );
  CoreRectView__OnSetBounds( &_this->DashboardV1, _Const0011 );
  CoreRectView__OnSetBounds( &_this->NavigationHints, _Const0061 );
  CoreGroup__OnSetOpacity( &_this->NavigationHints, 0 );
  CoreRectView__OnSetBounds( &_this->Clock, _Const0062 );
  CoreRectView__OnSetBounds( &_this->Munich, _Const0063 );
  ViewsText_OnSetString( &_this->Munich, EwLoadString( &_Const0064 ));
  ViewsText_OnSetColor( &_this->Munich, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->Reach, _Const0065 );
  ViewsText_OnSetString( &_this->Reach, EwLoadString( &_Const0066 ));
  ViewsText_OnSetColor( &_this->Reach, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->Battery, _Const0067 );
  DashboardBattery_OnSetTrackColor( &_this->Battery, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV2 );
  CoreRectView__OnSetBounds( &_this->EnergyDisplay, _Const0068 );
  DashboardGaugeDisplay_OnSetUnit( &_this->EnergyDisplay, EwLoadString( &_Const0028 ));
  DashboardGaugeDisplay_OnSetDesc( &_this->EnergyDisplay, EwLoadString( &_Const0069 ));
  DashboardGaugeDisplay_OnSetDescUnit( &_this->EnergyDisplay, EwLoadString( &_Const0022 ));
  CoreQuadView__OnSetPoint4( &_this->EnergyWarpGroup, _Const006A );
  CoreQuadView__OnSetPoint3( &_this->EnergyWarpGroup, _Const006B );
  CoreQuadView__OnSetPoint2( &_this->EnergyWarpGroup, _Const006C );
  CoreQuadView__OnSetPoint1( &_this->EnergyWarpGroup, _Const006D );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->EnergyWarpGroup, EwGetRectCenter( 
  EwGetRectORect( _this->EnergyDisplay.Super2.Bounds )));
  CoreRectView__OnSetBounds( &_this->SpeedDisplay, _Const006E );
  DashboardGaugeDisplay_OnSetUnit( &_this->SpeedDisplay, EwLoadString( &_Const006F ));
  DashboardGaugeDisplay_OnSetDesc( &_this->SpeedDisplay, EwLoadString( &_Const0070 ));
  DashboardGaugeDisplay_OnSetDescUnit( &_this->SpeedDisplay, EwLoadString( &_Const0071 ));
  CoreQuadView__OnSetPoint4( &_this->SpeedWarpGroup, _Const0072 );
  CoreQuadView__OnSetPoint3( &_this->SpeedWarpGroup, _Const0073 );
  CoreQuadView__OnSetPoint2( &_this->SpeedWarpGroup, _Const0074 );
  CoreQuadView__OnSetPoint1( &_this->SpeedWarpGroup, _Const0075 );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->SpeedWarpGroup, EwGetRectCenter( 
  EwGetRectORect( _this->SpeedDisplay.Super2.Bounds )));
  CoreRectView__OnSetBounds( &_this->LedButton, _Const0076 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton, 3 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton, 3 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton1, _Const0077 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton1, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton1, 4 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton1, 4 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton1, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton2, _Const0078 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton2, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton2, 5 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton2, 5 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton2, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton3, _Const0079 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton3, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton3, 0 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton3, 0 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton3, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton4, _Const007A );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton4, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton4, 1 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton4, 1 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton4, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton5, _Const007B );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton5, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton5, 2 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton5, 2 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton5, 0 );
  CoreRectView__OnSetBounds( &_this->RangeDisplay, _Const007C );
  WidgetSetValueDisplay_OnSetUnit( &_this->RangeDisplay, EwLoadString( &_Const007D ));
  WidgetSetValueDisplay_OnSetNoOfDigits( &_this->RangeDisplay, 1 );
  WidgetSetValueDisplay_OnSetCurrentFactor( &_this->RangeDisplay, 4.000000f );
  CoreTimer_OnSetPeriod( &_this->autoDemoModeTimer, 20000 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Navigation ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ShutdownButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TransitionButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DashboardV2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DashboardV1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NavigationHints ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Clock ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Munich ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Reach ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Battery ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->EnergyDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->EnergyWarpGroup ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedWarpGroup ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LedButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LedButton1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LedButton2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LedButton3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LedButton4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LedButton5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RangeDisplay ), 0 );
  _this->v1Effect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onV1Effect );
  _this->v1Effect.Outlet = EwNewRef( _this, DashboardDashboard_OnGetprogress, DashboardDashboard_OnSetprogress );
  _this->ledOnEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onLedOnEffectFinished );
  _this->ledOnEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onLedOnEffect );
  _this->transitionEffect.Outlet = EwNewRef( _this, DashboardDashboard_OnGetprogress, 
  DashboardDashboard_OnSetprogress );
  _this->navigationEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onNavigationFinished );
  _this->navigationEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onNavigationEffect );
  _this->shutdownVariantEffect.Outlet = EwNewRef( _this, DashboardDashboard_OnGetprogress, 
  DashboardDashboard_OnSetprogress );
  _this->shutdownNavigationEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onShutdownNavigationFinished );
  _this->shutdownNavigationEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onShutdownNavigationEffect );
  _this->gaugeDisplayEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onGaugeDisplayFinished );
  _this->gaugeDisplayEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onGaugeDisplayEffect );
  _this->shutdownGaugeDisplayEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onShutdownGaugeDisplayFinished );
  _this->shutdownGaugeDisplayEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onShutdownGaugeDisplayEffect );
  _this->gaugeCheckEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onGaugeCheckEffectFinished );
  _this->gaugeCheckEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onGaugeCheckEffect );
  _this->ledOffEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onLedOffEffect );
  _this->Navigation.Hints = &_this->NavigationHints;
  _this->ShutdownButton.OnActivate = EwNewSlot( _this, DashboardDashboard_onShutdown );
  WidgetSetPushButton_OnSetIcon( &_this->ShutdownButton, EwLoadResource( &DashboardOffIcon, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->ShutdownButton, EwGetAutoObject( 
  &DashboardDashboardButtonConfig, WidgetSetPushButtonConfig ));
  _this->TransitionButton.OnActivate = EwNewSlot( _this, DashboardDashboard_onStartTransition );
  WidgetSetPushButton_OnSetIcon( &_this->TransitionButton, EwLoadResource( &DashboardSwitchIcon, 
  ResourcesBitmap ));
  WidgetSetPushButton_OnSetAppearance( &_this->TransitionButton, EwGetAutoObject( 
  &DashboardDashboardButtonConfig, WidgetSetPushButtonConfig ));
  DashboardDashboardV2_OnSetOutlet( &_this->DashboardV2, EwNewRef( _this, DashboardDashboard_OnGetprogressReverse, 
  DashboardDashboard_OnSetprogressReverse ));
  DashboardDashboardV1_OnSetOutlet( &_this->DashboardV1, EwNewRef( _this, DashboardDashboard_OnGetprogress, 
  DashboardDashboard_OnSetprogress ));
  ViewsText_OnSetFont( &_this->Munich, EwLoadResource( &DashboardFontLightXL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Reach, EwLoadResource( &DashboardFontLightM, ResourcesFont ));
  DashboardBattery_OnSetOutlet( &_this->Battery, EwNewRef( EwGetAutoObject( &DashboardDevice, 
  DashboardDeviceClass ), DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery ));
  DashboardGaugeDisplay_OnSetOutlet( &_this->EnergyDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy ));
  DashboardGaugeDisplay_OnSetOutletDesc( &_this->EnergyDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery ));
  ViewsWarpGroup_OnSetGroup( &_this->EnergyWarpGroup, ((CoreGroup)&_this->EnergyDisplay ));
  DashboardGaugeDisplay_OnSetOutlet( &_this->SpeedDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ));
  DashboardGaugeDisplay_OnSetOutletDesc( &_this->SpeedDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetODO, DashboardDeviceClass_OnSetODO ));
  ViewsWarpGroup_OnSetGroup( &_this->SpeedWarpGroup, ((CoreGroup)&_this->SpeedDisplay ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed0, DashboardDeviceClass_OnSetLed0 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton, EwGetAutoObject( &DashboardLedButtonGreenConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton1, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed1, DashboardDeviceClass_OnSetLed1 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton1, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton1, EwGetAutoObject( &DashboardLedButtonGreenConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton2, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed2, DashboardDeviceClass_OnSetLed2 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton2, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton2, EwGetAutoObject( &DashboardLedButtonGreenConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton3, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed3, DashboardDeviceClass_OnSetLed3 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton3, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton3, EwGetAutoObject( &DashboardLedButtonOrangeConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton4, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed4, DashboardDeviceClass_OnSetLed4 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton4, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton4, EwGetAutoObject( &DashboardLedButtonOrangeConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton5, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed5, DashboardDeviceClass_OnSetLed5 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton5, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton5, EwGetAutoObject( &DashboardLedButtonOrangeConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetValueDisplay_OnSetOutlet( &_this->RangeDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery ));
  WidgetSetValueDisplay_OnSetAppearance( &_this->RangeDisplay, EwGetAutoObject( 
  &DashboardRangeConfig, WidgetSetValueDisplayConfig ));
  _this->autoDemoModeTimer.OnTrigger = EwNewSlot( _this, DashboardDashboard_onStartTransition );

  /* Call the user defined constructor */
  DashboardDashboard_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::Dashboard' */
void DashboardDashboard__ReInit( DashboardDashboard _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsFloatEffect__ReInit( &_this->v1Effect );
  EffectsInt32Effect__ReInit( &_this->ledOnEffect );
  EffectsFloatEffect__ReInit( &_this->transitionEffect );
  EffectsInt32Effect__ReInit( &_this->navigationEffect );
  EffectsFloatEffect__ReInit( &_this->shutdownVariantEffect );
  EffectsInt32Effect__ReInit( &_this->shutdownNavigationEffect );
  EffectsFloatEffect__ReInit( &_this->gaugeDisplayEffect );
  EffectsFloatEffect__ReInit( &_this->shutdownGaugeDisplayEffect );
  EffectsInt32Effect__ReInit( &_this->gaugeCheckEffect );
  EffectsInt32Effect__ReInit( &_this->ledOffEffect );
  ViewsRectangle__ReInit( &_this->Background );
  DashboardNavigation__ReInit( &_this->Navigation );
  WidgetSetPushButton__ReInit( &_this->ShutdownButton );
  WidgetSetPushButton__ReInit( &_this->TransitionButton );
  DashboardDashboardV2__ReInit( &_this->DashboardV2 );
  DashboardDashboardV1__ReInit( &_this->DashboardV1 );
  DashboardNavigationHints__ReInit( &_this->NavigationHints );
  DashboardClock__ReInit( &_this->Clock );
  ViewsText__ReInit( &_this->Munich );
  ViewsText__ReInit( &_this->Reach );
  DashboardBattery__ReInit( &_this->Battery );
  DashboardGaugeDisplay__ReInit( &_this->EnergyDisplay );
  ViewsWarpGroup__ReInit( &_this->EnergyWarpGroup );
  DashboardGaugeDisplay__ReInit( &_this->SpeedDisplay );
  ViewsWarpGroup__ReInit( &_this->SpeedWarpGroup );
  WidgetSetToggleButton__ReInit( &_this->LedButton );
  WidgetSetToggleButton__ReInit( &_this->LedButton1 );
  WidgetSetToggleButton__ReInit( &_this->LedButton2 );
  WidgetSetToggleButton__ReInit( &_this->LedButton3 );
  WidgetSetToggleButton__ReInit( &_this->LedButton4 );
  WidgetSetToggleButton__ReInit( &_this->LedButton5 );
  WidgetSetValueDisplay__ReInit( &_this->RangeDisplay );
  CoreTimer__ReInit( &_this->autoDemoModeTimer );
}

/* Finalizer method for the class 'Dashboard::Dashboard' */
void DashboardDashboard__Done( DashboardDashboard _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  EffectsFloatEffect__Done( &_this->v1Effect );
  EffectsInt32Effect__Done( &_this->ledOnEffect );
  EffectsFloatEffect__Done( &_this->transitionEffect );
  EffectsInt32Effect__Done( &_this->navigationEffect );
  EffectsFloatEffect__Done( &_this->shutdownVariantEffect );
  EffectsInt32Effect__Done( &_this->shutdownNavigationEffect );
  EffectsFloatEffect__Done( &_this->gaugeDisplayEffect );
  EffectsFloatEffect__Done( &_this->shutdownGaugeDisplayEffect );
  EffectsInt32Effect__Done( &_this->gaugeCheckEffect );
  EffectsInt32Effect__Done( &_this->ledOffEffect );
  ViewsRectangle__Done( &_this->Background );
  DashboardNavigation__Done( &_this->Navigation );
  WidgetSetPushButton__Done( &_this->ShutdownButton );
  WidgetSetPushButton__Done( &_this->TransitionButton );
  DashboardDashboardV2__Done( &_this->DashboardV2 );
  DashboardDashboardV1__Done( &_this->DashboardV1 );
  DashboardNavigationHints__Done( &_this->NavigationHints );
  DashboardClock__Done( &_this->Clock );
  ViewsText__Done( &_this->Munich );
  ViewsText__Done( &_this->Reach );
  DashboardBattery__Done( &_this->Battery );
  DashboardGaugeDisplay__Done( &_this->EnergyDisplay );
  ViewsWarpGroup__Done( &_this->EnergyWarpGroup );
  DashboardGaugeDisplay__Done( &_this->SpeedDisplay );
  ViewsWarpGroup__Done( &_this->SpeedWarpGroup );
  WidgetSetToggleButton__Done( &_this->LedButton );
  WidgetSetToggleButton__Done( &_this->LedButton1 );
  WidgetSetToggleButton__Done( &_this->LedButton2 );
  WidgetSetToggleButton__Done( &_this->LedButton3 );
  WidgetSetToggleButton__Done( &_this->LedButton4 );
  WidgetSetToggleButton__Done( &_this->LedButton5 );
  WidgetSetValueDisplay__Done( &_this->RangeDisplay );
  CoreTimer__Done( &_this->autoDemoModeTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardDashboard_Init( DashboardDashboard _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwAttachRefObserver( EwNewSlot( _this, DashboardDashboard_onAutoDemoModeChanged ), 
    EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetAutoDemoMode, 
    DashboardDeviceClass_OnSetAutoDemoMode ), 0 );
  EwPostSignal( EwNewSlot( _this, DashboardDashboard_onAutoDemoModeChanged ), ((XObject)_this ));
  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV1, 1 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV2, 0 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->TransitionButton, 0 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->ShutdownButton, 0 );
  _this->shutdown = 0;
  CoreGroup_OnSetVisible((CoreGroup)&_this->TransitionButton, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->ShutdownButton, 0 );
  CoreGroup__OnSetOpacity( &_this->EnergyDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->SpeedDisplay, 0 );
  ViewsWarpGroup_OnSetOpacity( &_this->SpeedWarpGroup, 0 );
  ViewsWarpGroup_OnSetOpacity( &_this->EnergyWarpGroup, 0 );
  CoreGroup__OnSetOpacity( &_this->Clock, 0 );
  ViewsText_OnSetOpacity( &_this->Munich, 0 );
  CoreGroup__OnSetOpacity( &_this->Battery, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->Battery, 0 );
  CoreGroup__OnSetOpacity( &_this->RangeDisplay, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->RangeDisplay, 0 );
  ViewsText_OnSetOpacity( &_this->Reach, 0 );
  ViewsText_OnSetVisible( &_this->Reach, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton1, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton2, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton3, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton4, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton5, 0 );
  CoreGroup__OnSetBuffered( &_this->SpeedDisplay, 1 );
  CoreGroup__OnSetBuffered( &_this->EnergyDisplay, 1 );
  EwIdleSignal( EwNewSlot( &_this->gaugeDisplayEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  EwIdleSignal( EwNewSlot( &_this->v1Effect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void DashboardDashboard_UpdateViewState( DashboardDashboard _this, XSet aState )
{
  CoreGroup_UpdateViewState((CoreGroup)_this, aState );

  if ( _this->shutdown )
  {
    XFloat offset = 1.000000f - _this->progress;

    if ( CoreGroup_OnGetVisible((CoreGroup)&_this->DashboardV1 ))
      offset = _this->progress;

    CoreGroup__OnSetOpacity( &_this->LedButton2, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.700000f, 
    1.000000f, offset ));
    CoreGroup__OnSetOpacity( &_this->LedButton1, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.700000f, 
    1.000000f, offset ));
    CoreGroup__OnSetOpacity( &_this->LedButton, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.700000f, 
    1.000000f, offset ));
    CoreGroup__OnSetOpacity( &_this->LedButton5, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.700000f, 
    1.000000f, offset ));
    CoreGroup__OnSetOpacity( &_this->LedButton4, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.700000f, 
    1.000000f, offset ));
    CoreGroup__OnSetOpacity( &_this->LedButton3, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.700000f, 
    1.000000f, offset ));
  }
  else
  {
    CoreRectView__OnSetBounds( &_this->LedButton2, EwSetRectOrigin( _this->LedButton2.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const007E, _Const007F, 0.800000f, 1.000000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton1, EwSetRectOrigin( _this->LedButton1.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0080, _Const0081, 0.800000f, 1.000000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton, EwSetRectOrigin( _this->LedButton.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0082, _Const0083, 0.800000f, 1.000000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton5, EwSetRectOrigin( _this->LedButton5.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0084, _Const0085, 0.800000f, 1.000000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton4, EwSetRectOrigin( _this->LedButton4.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0086, _Const0087, 0.800000f, 1.000000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton3, EwSetRectOrigin( _this->LedButton3.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0088, _Const0089, 0.800000f, 1.000000f, _this->progress )));
    DashboardNavigation_OnSetPositionColor( &_this->Navigation, DashboardAnimationClass_linearColorProgess( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), EwGetAutoObject( 
    &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV2, EwGetAutoObject( 
    &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1, 0.800000f, 
    1.000000f, _this->progress ));

    if ( CoreGroup_OnGetVisible((CoreGroup)&_this->DashboardV2 ))
    {
      if ( !CoreGroup_OnGetVisible((CoreGroup)&_this->Battery ))
        CoreGroup_OnSetVisible((CoreGroup)&_this->Battery, 1 );

      CoreGroup__OnSetOpacity( &_this->Battery, DashboardAnimationClass_linearValueProgress( 
      EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 255, 0, 0.200000f, 
      0.500000f, _this->progress ));

      if ( !CoreGroup_OnGetVisible((CoreGroup)&_this->RangeDisplay ))
        CoreGroup_OnSetVisible((CoreGroup)&_this->RangeDisplay, 1 );

      CoreGroup__OnSetOpacity( &_this->RangeDisplay, DashboardAnimationClass_linearValueProgress( 
      EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 255, 0, 0.200000f, 
      0.500000f, _this->progress ));

      if ( !ViewsText_OnGetVisible( &_this->Reach ))
        ViewsText_OnSetVisible( &_this->Reach, 1 );

      ViewsText_OnSetOpacity( &_this->Reach, DashboardAnimationClass_linearValueProgress( 
      EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 255, 0, 0.200000f, 
      0.500000f, _this->progress ));
    }
    else
    {
      CoreGroup_OnSetVisible((CoreGroup)&_this->Battery, 0 );
      CoreGroup_OnSetVisible((CoreGroup)&_this->RangeDisplay, 0 );
      ViewsText_OnSetVisible( &_this->Reach, 0 );
    }
  }
}

/* 'C' function for method : 'Dashboard::Dashboard.onV1Effect()' */
void DashboardDashboard_onV1Effect( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV2, 1 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->TransitionButton, 1 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->ShutdownButton, 1 );
  CoreGroup__OnSetOpacity( &_this->Clock, 255 );
  ViewsText_OnSetOpacity( &_this->Munich, 255 );
  CoreGroup__OnSetOpacity( &_this->LedButton, 255 );
  CoreGroup__OnSetOpacity( &_this->LedButton1, 255 );
  CoreGroup__OnSetOpacity( &_this->LedButton2, 255 );
  CoreGroup__OnSetOpacity( &_this->LedButton3, 255 );
  CoreGroup__OnSetOpacity( &_this->LedButton4, 255 );
  CoreGroup__OnSetOpacity( &_this->LedButton5, 255 );
  EwSignal( EwNewSlot( &_this->ledOnEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeDisplayEffect()' */
void DashboardDashboard_onGaugeDisplayEffect( DashboardDashboard _this, XObject 
  sender )
{
  XFloat startOpacity;
  XFloat scale;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsWarpGroup_OnSetOpacity( &_this->SpeedWarpGroup, (XUInt8)_this->gaugeDisplayEffect.Value );
  ViewsWarpGroup_OnSetOpacity( &_this->EnergyWarpGroup, (XUInt8)_this->gaugeDisplayEffect.Value );
  startOpacity = 5.000000f;
  scale = startOpacity / 255.000000f;

  if ( _this->gaugeDisplayEffect.Value >= startOpacity )
    scale = _this->gaugeDisplayEffect.Value / 255.000000f;

  ViewsWarpView__RotateAndScale( &_this->SpeedWarpGroup, EwGetRectCenter( _this->SpeedDisplay.Super2.Bounds ), 
  0.000000f, scale, scale );
  ViewsWarpView__RotateAndScale( &_this->EnergyWarpGroup, EwGetRectCenter( _this->EnergyDisplay.Super2.Bounds ), 
  0.000000f, scale, scale );
}

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeDisplayFinished()' */
void DashboardDashboard_onGaugeDisplayFinished( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->EnergyDisplay, 255 );
  CoreGroup__OnSetOpacity( &_this->SpeedDisplay, 255 );
  CoreGroup__OnSetBuffered( &_this->SpeedDisplay, 0 );
  CoreGroup__OnSetBuffered( &_this->EnergyDisplay, 0 );
}

/* 'C' function for method : 'Dashboard::Dashboard.onLedOnEffect()' */
void DashboardDashboard_onLedOnEffect( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  DashboardDeviceClass_OnSetLed2( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOnEffect.Value > 0 ));
  DashboardDeviceClass_OnSetLed5( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOnEffect.Value > 0 ));
  DashboardDeviceClass_OnSetLed1( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOnEffect.Value > 1 ));
  DashboardDeviceClass_OnSetLed4( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOnEffect.Value > 1 ));
  DashboardDeviceClass_OnSetLed0( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOnEffect.Value > 2 ));
  DashboardDeviceClass_OnSetLed3( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOnEffect.Value > 2 ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onLedOnEffectFinished()' */
void DashboardDashboard_onLedOnEffectFinished( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwSignal( EwNewSlot( &_this->gaugeCheckEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onStartTransition()' */
void DashboardDashboard_onStartTransition( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->transitionEffect.Value1 = _this->progress;
  _this->transitionEffect.Value2 = 1.000000f - _this->transitionEffect.Value2;
  EwSignal( EwNewSlot( &_this->transitionEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onNavigationFinished()' */
void DashboardDashboard_onNavigationFinished( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_OnSetEnabled((CoreGroup)&_this->TransitionButton, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->ShutdownButton, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton1, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton2, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton3, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton4, 1 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton5, 1 );
  DashboardDeviceClass_OnSetDemoMode( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  1 );
}

/* 'C' function for method : 'Dashboard::Dashboard.OnSetprogress()' */
void DashboardDashboard_OnSetprogress( DashboardDashboard _this, XFloat value )
{
  if ( _this->progress == value )
    return;

  _this->progress = value;
  DashboardDashboard_OnSetprogressReverse( _this, 1.000000f - value );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwNotifyRefObservers( EwNewRef( _this, DashboardDashboard_OnGetprogress, DashboardDashboard_OnSetprogress ), 
    0 );
}

/* 'C' function for method : 'Dashboard::Dashboard.OnSetprogressReverse()' */
void DashboardDashboard_OnSetprogressReverse( DashboardDashboard _this, XFloat value )
{
  if ( _this->progressReverse == value )
    return;

  _this->progressReverse = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDashboard_OnGetprogressReverse, 
    DashboardDashboard_OnSetprogressReverse ), 0 );
}

/* 'C' function for method : 'Dashboard::Dashboard.onNavigationEffect()' */
void DashboardDashboard_onNavigationEffect( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->Navigation, _this->navigationEffect.Value );
  CoreGroup__OnSetOpacity( &_this->NavigationHints, _this->navigationEffect.Value );
}

/* 'C' function for method : 'Dashboard::Dashboard.onShutdown()' */
void DashboardDashboard_onShutdown( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_OnSetEnabled((CoreGroup)&_this->TransitionButton, 0 );
  _this->shutdown = 1;
  DashboardDeviceClass_OnSetDemoMode( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  EwSignal( EwNewSlot( &_this->shutdownNavigationEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownNavigationFinished()' */
void DashboardDashboard_onShutdownNavigationFinished( DashboardDashboard _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->shutdownVariantEffect.Value1 = _this->progress;
  _this->shutdownVariantEffect.Value2 = 1.000000f - _this->transitionEffect.Value2;
  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV1, (XBool)( _this->progress 
  == 1.000000f ));
  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV2, (XBool)!CoreGroup_OnGetVisible((CoreGroup)&_this->DashboardV1 ));
  CoreGroup_OnSetVisible((CoreGroup)&_this->TransitionButton, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->ShutdownButton, 0 );
  CoreGroup__OnSetOpacity( &_this->Clock, 0 );
  ViewsText_OnSetOpacity( &_this->Munich, 0 );
  CoreGroup__OnSetOpacity( &_this->Battery, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->Battery, 0 );
  CoreGroup__OnSetOpacity( &_this->RangeDisplay, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->RangeDisplay, 0 );
  ViewsText_OnSetOpacity( &_this->Reach, 0 );
  ViewsText_OnSetVisible( &_this->Reach, 0 );
  CoreGroup__OnSetBuffered( &_this->SpeedDisplay, 1 );
  CoreGroup__OnSetBuffered( &_this->EnergyDisplay, 1 );
  CoreGroup__OnSetOpacity( &_this->EnergyDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->SpeedDisplay, 0 );
  EwSignal( EwNewSlot( &_this->shutdownVariantEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  EwSignal( EwNewSlot( &_this->shutdownGaugeDisplayEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownNavigationEffect()' */
void DashboardDashboard_onShutdownNavigationEffect( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->Navigation, _this->shutdownNavigationEffect.Value );
  CoreGroup__OnSetOpacity( &_this->NavigationHints, _this->shutdownNavigationEffect.Value );
}

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownGaugeDisplayEffect()' */
void DashboardDashboard_onShutdownGaugeDisplayEffect( DashboardDashboard _this, 
  XObject sender )
{
  XFloat endOpacity;
  XFloat scale;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsWarpGroup_OnSetOpacity( &_this->SpeedWarpGroup, (XUInt8)_this->shutdownGaugeDisplayEffect.Value );
  ViewsWarpGroup_OnSetOpacity( &_this->EnergyWarpGroup, (XUInt8)_this->shutdownGaugeDisplayEffect.Value );
  endOpacity = 5.000000f;
  scale = endOpacity / 255.000000f;

  if ( _this->shutdownGaugeDisplayEffect.Value >= endOpacity )
    scale = _this->shutdownGaugeDisplayEffect.Value / 255.000000f;

  ViewsWarpView__RotateAndScale( &_this->SpeedWarpGroup, EwGetRectCenter( _this->SpeedDisplay.Super2.Bounds ), 
  0.000000f, scale, scale );
  ViewsWarpView__RotateAndScale( &_this->EnergyWarpGroup, EwGetRectCenter( _this->EnergyDisplay.Super2.Bounds ), 
  0.000000f, scale, scale );
}

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownGaugeDisplayFinished()' */
void DashboardDashboard_onShutdownGaugeDisplayFinished( DashboardDashboard _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( CoreGroup__IsDialog( _this, 0 ))
    CoreGroup_SwitchToDialog( _this->Super3.Owner, ((CoreGroup)EwNewObject( DashboardStartScreen, 
    0 )), ((EffectsTransition)EwGetAutoObject( &DashboardFadeInOutCentered, EffectsFadeInOutTransition )), 
    ((EffectsTransition)EwGetAutoObject( &EffectsFadeInOutCentered, EffectsFadeInOutTransition )), 
    0, 0, ((EffectsTransition)EwGetAutoObject( &EffectsFadeInOutCentered, EffectsFadeInOutTransition )), 
    0, 0, EwNullSlot, EwNullSlot, 0 );
}

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeCheckEffect()' */
void DashboardDashboard_onGaugeCheckEffect( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  DashboardDeviceClass_OnSetSpeed( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  _this->gaugeCheckEffect.Value );
  DashboardDeviceClass_OnSetEnergy( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  _this->gaugeCheckEffect.Value );
}

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeCheckEffectFinished()' */
void DashboardDashboard_onGaugeCheckEffectFinished( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwSignal( EwNewSlot( &_this->navigationEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
  EwSignal( EwNewSlot( &_this->ledOffEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onLedOffEffect()' */
void DashboardDashboard_onLedOffEffect( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  DashboardDeviceClass_OnSetLed2( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOffEffect.Value > 0 ));
  DashboardDeviceClass_OnSetLed5( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOffEffect.Value > 0 ));
  DashboardDeviceClass_OnSetLed1( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOffEffect.Value > 1 ));
  DashboardDeviceClass_OnSetLed4( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOffEffect.Value > 1 ));
  DashboardDeviceClass_OnSetLed0( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOffEffect.Value > 2 ));
  DashboardDeviceClass_OnSetLed3( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  (XBool)( _this->ledOffEffect.Value > 2 ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onAutoDemoModeChanged()' */
void DashboardDashboard_onAutoDemoModeChanged( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass )->AutoDemoMode )
    EwSignal( EwNewSlot( &_this->autoDemoModeTimer, CoreTimer_StartTimer ), ((XObject)_this ));
  else
    EwSignal( EwNewSlot( &_this->autoDemoModeTimer, CoreTimer_StopTimer ), ((XObject)_this ));
}

/* Default onget method for the property 'progress' */
XFloat DashboardDashboard_OnGetprogress( DashboardDashboard _this )
{
  return _this->progress;
}

/* Default onget method for the property 'progressReverse' */
XFloat DashboardDashboard_OnGetprogressReverse( DashboardDashboard _this )
{
  return _this->progressReverse;
}

/* Variants derived from the class : 'Dashboard::Dashboard' */
EW_DEFINE_CLASS_VARIANTS( DashboardDashboard )
EW_END_OF_CLASS_VARIANTS( DashboardDashboard )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Dashboard' */
EW_DEFINE_CLASS( DashboardDashboard, CoreGroup, v1Effect, v1Effect, v1Effect, v1Effect, 
                 progress, progress, "Dashboard::Dashboard" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  DashboardDashboard_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardDashboard )

/* Include a file containing the bitmap resource : 'Dashboard::Map' */
#include "_DashboardMap.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Map' */
EW_RES_WITHOUT_VARIANTS( DashboardMap )

/* Include a file containing the bitmap resource : 'Dashboard::MapFading' */
#include "_DashboardMapFading.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::MapFading' */
EW_RES_WITHOUT_VARIANTS( DashboardMapFading )

/* Include a file containing the bitmap resource : 'Dashboard::Position' */
#include "_DashboardPosition.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Position' */
EW_RES_WITHOUT_VARIANTS( DashboardPosition )

/* Initializer for the class 'Dashboard::NavigationHints' */
void DashboardNavigationHints__Init( DashboardNavigationHints _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardNavigationHints );

  /* ... then construct all embedded objects */
  WidgetSetValueDisplay__Init( &_this->DistanceDisplay, &_this->_XObject, 0 );
  ViewsText__Init( &_this->DescText, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->DirectionImage, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardNavigationHints );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const008A );
  CoreRectView__OnSetBounds( &_this->DistanceDisplay, _Const008B );
  WidgetSetValueDisplay_OnSetUnit( &_this->DistanceDisplay, EwLoadString( &_Const008C ));
  WidgetSetValueDisplay_OnSetNoOfDigits( &_this->DistanceDisplay, 1 );
  CoreRectView__OnSetBounds( &_this->DescText, _Const008D );
  ViewsText_OnSetString( &_this->DescText, EwLoadString( &_Const008E ));
  ViewsText_OnSetColor( &_this->DescText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  ViewsText_OnSetVisible( &_this->DescText, 0 );
  CoreRectView__OnSetBounds( &_this->DirectionImage, _Const008F );
  ViewsImage_OnSetColor( &_this->DirectionImage, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DistanceDisplay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DescText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DirectionImage ), 0 );
  WidgetSetValueDisplay_OnSetAppearance( &_this->DistanceDisplay, EwGetAutoObject( 
  &DashboardNavigationHintConfig, WidgetSetValueDisplayConfig ));
  ViewsText_OnSetFont( &_this->DescText, EwLoadResource( &DashboardFontLightM, ResourcesFont ));
  ViewsImage_OnSetBitmap( &_this->DirectionImage, EwLoadResource( &DashboardArrows, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::NavigationHints' */
void DashboardNavigationHints__ReInit( DashboardNavigationHints _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  WidgetSetValueDisplay__ReInit( &_this->DistanceDisplay );
  ViewsText__ReInit( &_this->DescText );
  ViewsImage__ReInit( &_this->DirectionImage );
}

/* Finalizer method for the class 'Dashboard::NavigationHints' */
void DashboardNavigationHints__Done( DashboardNavigationHints _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  WidgetSetValueDisplay__Done( &_this->DistanceDisplay );
  ViewsText__Done( &_this->DescText );
  ViewsImage__Done( &_this->DirectionImage );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::NavigationHints.OnSetDistance()' */
void DashboardNavigationHints_OnSetDistance( DashboardNavigationHints _this, XInt32 
  value )
{
  if ( _this->Distance == value )
    return;

  _this->Distance = value;
  WidgetSetValueDisplay_OnSetCurrentValue( &_this->DistanceDisplay, value );
}

/* 'C' function for method : 'Dashboard::NavigationHints.OnSetDirection()' */
void DashboardNavigationHints_OnSetDirection( DashboardNavigationHints _this, XInt32 
  value )
{
  if ( _this->Direction == value )
    return;

  _this->Direction = value;

  if ( _this->Direction < 0 )
  {
    ViewsImage_OnSetFrameNumber( &_this->DirectionImage, 0 );
    ViewsText_OnSetString( &_this->DescText, EwLoadString( &_Const0090 ));
  }
  else
    if ( _this->Direction == 0 )
    {
      ViewsImage_OnSetFrameNumber( &_this->DirectionImage, 1 );
      ViewsText_OnSetString( &_this->DescText, EwLoadString( &_Const0091 ));
    }
    else
    {
      ViewsImage_OnSetFrameNumber( &_this->DirectionImage, 2 );
      ViewsText_OnSetString( &_this->DescText, EwLoadString( &_Const008E ));
    }
}

/* Variants derived from the class : 'Dashboard::NavigationHints' */
EW_DEFINE_CLASS_VARIANTS( DashboardNavigationHints )
EW_END_OF_CLASS_VARIANTS( DashboardNavigationHints )

/* Virtual Method Table (VMT) for the class : 'Dashboard::NavigationHints' */
EW_DEFINE_CLASS( DashboardNavigationHints, CoreGroup, DistanceDisplay, DistanceDisplay, 
                 DistanceDisplay, DistanceDisplay, Distance, Distance, "Dashboard::NavigationHints" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardNavigationHints )

/* User defined auto object: 'Dashboard::NavigationHintConfig' */
EW_DEFINE_AUTOOBJECT( DashboardNavigationHintConfig, WidgetSetValueDisplayConfig )

/* Initializer for the auto object 'Dashboard::NavigationHintConfig' */
void DashboardNavigationHintConfig__Init( WidgetSetValueDisplayConfig _this )
{
  WidgetSetValueDisplayConfig_OnSetValueMarginRight( _this, 3 );
  WidgetSetValueDisplayConfig_OnSetValueColorNegative( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetValueColorPositive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetValueAlignment( _this, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetValueDisplayConfig_OnSetValueFont( _this, EwLoadResource( &DashboardFontLightL, 
  ResourcesFont ));
  WidgetSetValueDisplayConfig_OnSetUnitColorNegative( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetUnitColorPositive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetUnitFont( _this, EwLoadResource( &DashboardFontLightL, 
  ResourcesFont ));
  WidgetSetValueDisplayConfig_OnSetFormatDecimalSign( _this, 0 );
  WidgetSetValueDisplayConfig_OnSetLayout( _this, WidgetSetValueDisplayLayoutAlignValueLeftToUnit );
}

/* Table with links to derived variants of the auto object : 'Dashboard::NavigationHintConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardNavigationHintConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardNavigationHintConfig )

/* Include a file containing the bitmap resource : 'Dashboard::Arrows' */
#include "_DashboardArrows.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Arrows' */
EW_RES_WITHOUT_VARIANTS( DashboardArrows )

/* Initializer for the class 'Dashboard::AnimationClass' */
void DashboardAnimationClass__Init( DashboardAnimationClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardAnimationClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardAnimationClass );
}

/* Re-Initializer for the class 'Dashboard::AnimationClass' */
void DashboardAnimationClass__ReInit( DashboardAnimationClass _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Dashboard::AnimationClass' */
void DashboardAnimationClass__Done( DashboardAnimationClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::AnimationClass.linearValueProgress()' */
XInt32 DashboardAnimationClass_linearValueProgress( DashboardAnimationClass _this, 
  XInt32 aValue1, XInt32 aValue2, XFloat aStart, XFloat aEnd, XFloat aProgress )
{
  XInt32 opacity;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( _this );

  if (( aProgress >= aStart ) && ( aProgress <= aEnd ))
  {
    opacity = aValue1 + (XInt32)((( aProgress - aStart ) * (XFloat)( aValue2 - aValue1 )) 
    * ( 1.000000f / ( aEnd - aStart )));
  }
  else
    if ( aProgress < aStart )
    {
      opacity = aValue1;
    }
    else
    {
      opacity = aValue2;
    }

  return opacity;
}

/* 'C' function for method : 'Dashboard::AnimationClass.linearPointProgess()' */
XPoint DashboardAnimationClass_linearPointProgess( DashboardAnimationClass _this, 
  XPoint aValue1, XPoint aValue2, XFloat aStart, XFloat aEnd, XFloat aProgress )
{
  XInt32 x = DashboardAnimationClass_linearValueProgress( _this, aValue1.X, aValue2.X, 
    aStart, aEnd, aProgress );
  XInt32 y = DashboardAnimationClass_linearValueProgress( _this, aValue1.Y, aValue2.Y, 
    aStart, aEnd, aProgress );

  return EwNewPoint( x, y );
}

/* 'C' function for method : 'Dashboard::AnimationClass.linearColorProgess()' */
XColor DashboardAnimationClass_linearColorProgess( DashboardAnimationClass _this, 
  XColor aValue1, XColor aValue2, XFloat aStart, XFloat aEnd, XFloat aProgress )
{
  XUInt8 r = (XUInt8)DashboardAnimationClass_linearValueProgress( _this, aValue1.Red, 
    aValue2.Red, aStart, aEnd, aProgress );
  XUInt8 g = (XUInt8)DashboardAnimationClass_linearValueProgress( _this, aValue1.Green, 
    aValue2.Green, aStart, aEnd, aProgress );
  XUInt8 b = (XUInt8)DashboardAnimationClass_linearValueProgress( _this, aValue1.Blue, 
    aValue2.Blue, aStart, aEnd, aProgress );

  return EwNewColor( r, g, b, 255 );
}

/* Variants derived from the class : 'Dashboard::AnimationClass' */
EW_DEFINE_CLASS_VARIANTS( DashboardAnimationClass )
EW_END_OF_CLASS_VARIANTS( DashboardAnimationClass )

/* Virtual Method Table (VMT) for the class : 'Dashboard::AnimationClass' */
EW_DEFINE_CLASS( DashboardAnimationClass, XObject, _None, _None, _None, _None, _None, 
                 _None, "Dashboard::AnimationClass" )
EW_END_OF_CLASS( DashboardAnimationClass )

/* User defined auto object: 'Dashboard::Animation' */
EW_DEFINE_AUTOOBJECT( DashboardAnimation, DashboardAnimationClass )

/* Initializer for the auto object 'Dashboard::Animation' */
void DashboardAnimation__Init( DashboardAnimationClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Dashboard::Animation' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardAnimation )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardAnimation )

/* User defined auto object: 'Dashboard::LedButtonOrangeConfig' */
EW_DEFINE_AUTOOBJECT( DashboardLedButtonOrangeConfig, WidgetSetToggleButtonConfig )

/* Initializer for the auto object 'Dashboard::LedButtonOrangeConfig' */
void DashboardLedButtonOrangeConfig__Init( WidgetSetToggleButtonConfig _this )
{
  WidgetSetToggleButtonConfig_OnSetIconOnTintActive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnOrange );
  WidgetSetToggleButtonConfig_OnSetIconOnTintFocused( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnOrange );
  WidgetSetToggleButtonConfig_OnSetIconOnTintDisabled( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnOrange );
  WidgetSetToggleButtonConfig_OnSetIconOnTintDefault( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnOrange );
  WidgetSetToggleButtonConfig_OnSetIconOffTintActive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
  WidgetSetToggleButtonConfig_OnSetIconOffTintFocused( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
  WidgetSetToggleButtonConfig_OnSetIconOffTintDisabled( _this, EwGetAutoObject( 
  &DashboardColors, DashboardColorsClass )->TextLedsOff );
  WidgetSetToggleButtonConfig_OnSetIconOffTintDefault( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
}

/* Table with links to derived variants of the auto object : 'Dashboard::LedButtonOrangeConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardLedButtonOrangeConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardLedButtonOrangeConfig )

/* User defined auto object: 'Dashboard::RangeConfig' */
EW_DEFINE_AUTOOBJECT( DashboardRangeConfig, WidgetSetValueDisplayConfig )

/* Initializer for the auto object 'Dashboard::RangeConfig' */
void DashboardRangeConfig__Init( WidgetSetValueDisplayConfig _this )
{
  WidgetSetValueDisplayConfig_OnSetValueMarginRight( _this, 3 );
  WidgetSetValueDisplayConfig_OnSetValueColorNegative( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetValueColorPositive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetValueAlignment( _this, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetValueDisplayConfig_OnSetValueFont( _this, EwLoadResource( &DashboardFontLightM, 
  ResourcesFont ));
  WidgetSetValueDisplayConfig_OnSetUnitColorNegative( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetUnitColorPositive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetUnitFont( _this, EwLoadResource( &DashboardFontLightM, 
  ResourcesFont ));
  WidgetSetValueDisplayConfig_OnSetFormatDecimalSign( _this, 0 );
  WidgetSetValueDisplayConfig_OnSetLayout( _this, WidgetSetValueDisplayLayoutAlignValueLeftToUnit );
}

/* Table with links to derived variants of the auto object : 'Dashboard::RangeConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardRangeConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardRangeConfig )

/* Initializer for the class 'Dashboard::GaugeLabels' */
void DashboardGaugeLabels__Init( DashboardGaugeLabels _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardGaugeLabels );

  /* ... then construct all embedded objects */
  GraphicsPath__Init( &_this->linePath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->StrokePath, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGaugeLabels );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0055 );
  _this->increment = 10;
  _this->radius = 217;
  _this->CurrentValue = 10;
  _this->Clockwise = 1;
  _this->startAngle = 180.000000f;
  _this->endAngle = 405.000000f;
  CoreRectView__OnSetBounds( &_this->StrokePath, _Const0092 );
  ViewsStrokePath_OnSetColor( &_this->StrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeLabelLinesV1 );
  ViewsStrokePath_OnSetWidth( &_this->StrokePath, 1.000000f );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->StrokePath ), 0 );
  ViewsStrokePath_OnSetPath( &_this->StrokePath, &_this->linePath );

  /* Call the user defined constructor */
  DashboardGaugeLabels_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::GaugeLabels' */
void DashboardGaugeLabels__ReInit( DashboardGaugeLabels _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  GraphicsPath__ReInit( &_this->linePath );
  ViewsStrokePath__ReInit( &_this->StrokePath );
}

/* Finalizer method for the class 'Dashboard::GaugeLabels' */
void DashboardGaugeLabels__Done( DashboardGaugeLabels _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  GraphicsPath__Done( &_this->linePath );
  ViewsStrokePath__Done( &_this->StrokePath );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardGaugeLabels_Init( DashboardGaugeLabels _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwSignal( EwNewSlot( _this, DashboardGaugeLabels_setup ), ((XObject)_this ));
  EwPostSignal( EwNewSlot( _this, DashboardGaugeLabels_refresh ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::GaugeLabels.refresh()' */
void DashboardGaugeLabels_refresh( DashboardGaugeLabels _this, XObject sender )
{
  XFloat deltaAngle;
  XInt32 i;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  deltaAngle = ( _this->endAngle - _this->startAngle ) / 10.000000f;
  i = 0;

  while ( i < 11 )
  {
    if ( _this->Clockwise )
      DashboardGaugeLabels_adjustLabel( _this, i, _this->startAngle + ((XFloat)i 
      * deltaAngle ), _this->radius, i * _this->increment );
    else
      DashboardGaugeLabels_adjustLabel( _this, i, _this->startAngle + ((XFloat)i 
      * deltaAngle ), _this->radius, (( 11 - i ) - 1 ) * _this->increment );

    i = i + 1;
  }
}

/* 'C' function for method : 'Dashboard::GaugeLabels.adjustLabel()' */
void DashboardGaugeLabels_adjustLabel( DashboardGaugeLabels _this, XInt32 aIndex, 
  XFloat aAngle, XInt32 aDistance, XInt32 aNumber )
{
  /* List of temporarily used variables */
  void* _tmpO0;
  XInt32 dist = aDistance;
  XInt32 offset = 0;
  XPoint labelCenter = EwNewPoint( EwGetRectW( _this->labels[ EwCheckIndex( aIndex, 
    11 )]->Super1.Bounds ) / 2, EwGetRectH( _this->labels[ EwCheckIndex( aIndex, 
    11 )]->Super1.Bounds ) / 2 );
  XInt32 p1Offset;
  XInt32 p2Offset;
  XPoint p1;
  XPoint p2;

  ViewsText_OnSetString( _this->labels[ EwCheckIndex( aIndex, 11 )], EwNewStringInt( 
  aNumber, 0, 10 ));

  if ( aAngle > 360.000000f )
    aAngle = aAngle - 360.000000f;

  if (( aNumber % 20 ) == 0 )
    aDistance = aDistance - 10;

  if (( aNumber / 100 ) >= 1 )
    offset = 5;

  ( _tmpO0 = _this->labels[ EwCheckIndex( aIndex, 11 )], CoreRectView__OnSetBounds( 
  _tmpO0, EwSetRectOrigin(((ViewsText)_tmpO0)->Super1.Bounds, EwMovePointPos( EwMovePointNeg( 
  EwGetRectCenter( EwGetRectORect( _this->Super2.Bounds )), labelCenter ), EwNewPoint((XInt32)((XFloat)( 
  aDistance + offset ) * EwMathCos( aAngle - 90.000000f )), (XInt32)((XFloat)( aDistance 
  + offset ) * EwMathSin( aAngle - 90.000000f )))))));
  p1Offset = 20;
  p2Offset = 38;
  p1 = EwMovePointPos( EwGetRectCenter( EwGetRectORect( _this->Super2.Bounds )), 
  EwNewPoint((XInt32)((XFloat)( aDistance - p1Offset ) * EwMathCos( aAngle - 90.000000f )), 
  (XInt32)((XFloat)( aDistance - p1Offset ) * EwMathSin( aAngle - 90.000000f ))));
  p2 = EwMovePointPos( EwGetRectCenter( EwGetRectORect( _this->Super2.Bounds )), 
  EwNewPoint((XInt32)((XFloat)( dist - p2Offset ) * EwMathCos( aAngle - 90.000000f )), 
  (XInt32)((XFloat)( dist - p2Offset ) * EwMathSin( aAngle - 90.000000f ))));
  GraphicsPath_InitSubPath( &_this->linePath, aIndex, 2 );
  GraphicsPath_Begin( &_this->linePath, aIndex, (XFloat)p1.X, (XFloat)p1.Y );
  GraphicsPath_AddLine( &_this->linePath, aIndex, (XFloat)p2.X, (XFloat)p2.Y );
}

/* 'C' function for method : 'Dashboard::GaugeLabels.OnSetCurrentValue()' */
void DashboardGaugeLabels_OnSetCurrentValue( DashboardGaugeLabels _this, XInt32 
  value )
{
  XInt32 index;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  index = (XInt32)EwMathRound((XFloat)_this->CurrentValue / (XFloat)_this->increment );

  if (( index < 11 ) && ( index != _this->selected ))
  {
    XInt32 i;

    for ( i = 0; i < 11; i = i + 1 )
      ViewsText_OnSetColor( _this->labels[ EwCheckIndex( i, 11 )], EwGetAutoObject( 
      &DashboardColors, DashboardColorsClass )->Text );

    if ( index >= 0 )
    {
      if ( _this->Clockwise )
        ViewsText_OnSetColor( _this->labels[ EwCheckIndex( index, 11 )], EwGetAutoObject( 
        &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1 );
      else
        ViewsText_OnSetColor( _this->labels[ EwCheckIndex(( 11 - index ) - 1, 11 )], 
        EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1 );
    }

    _this->selected = index;
  }
}

/* 'C' function for method : 'Dashboard::GaugeLabels.OnSetOutlet()' */
void DashboardGaugeLabels_OnSetOutlet( DashboardGaugeLabels _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardGaugeLabels_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardGaugeLabels_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardGaugeLabels_onOutlet ), ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardGaugeLabels_onOutlet( DashboardGaugeLabels _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardGaugeLabels_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::GaugeLabels.OnSetClockwise()' */
void DashboardGaugeLabels_OnSetClockwise( DashboardGaugeLabels _this, XBool value )
{
  XInt32 i;

  if ( _this->Clockwise == value )
    return;

  _this->Clockwise = value;
  i = 0;

  while ( i < 11 )
  {
    if ( _this->Clockwise )
    {
      if (( i % 2 ) == 0 )
        ViewsText_OnSetFont( _this->labels[ EwCheckIndex( i, 11 )], EwLoadResource( 
        &DashboardFontL, ResourcesFont ));
      else
        ViewsText_OnSetFont( _this->labels[ EwCheckIndex( i, 11 )], EwLoadResource( 
        &DashboardFontLightM, ResourcesFont ));
    }
    else
    {
      if (( i % 2 ) == 1 )
        ViewsText_OnSetFont( _this->labels[ EwCheckIndex( i, 11 )], EwLoadResource( 
        &DashboardFontLightM, ResourcesFont ));
      else
        ViewsText_OnSetFont( _this->labels[ EwCheckIndex( i, 11 )], EwLoadResource( 
        &DashboardFontL, ResourcesFont ));
    }

    i = i + 1;
  }

  if ( _this->Clockwise )
  {
    _this->startAngle = 180.000000f;
    _this->endAngle = 405.000000f;
  }
  else
  {
    _this->startAngle = 315.000000f;
    _this->endAngle = 540.000000f;
  }

  EwPostSignal( EwNewSlot( _this, DashboardGaugeLabels_refresh ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::GaugeLabels.setup()' */
void DashboardGaugeLabels_setup( DashboardGaugeLabels _this, XObject sender )
{
  XInt32 i;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  i = 0;

  while ( i < 11 )
  {
    ViewsText txt = EwNewObject( ViewsText, 0 );

    if (( i % 2 ) == 0 )
      ViewsText_OnSetFont( txt, EwLoadResource( &DashboardFontL, ResourcesFont ));
    else
      ViewsText_OnSetFont( txt, EwLoadResource( &DashboardFontLightM, ResourcesFont ));

    ViewsText_OnSetAutoSize( txt, 1 );
    _this->labels[ EwCheckIndex( i, 11 )] = txt;

    if ( _this->labels[ EwCheckIndex( i, 11 )]->Super2.Owner == 0 )
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->labels[ EwCheckIndex( i, 
      11 )]), 0 );

    i = i + 1;
  }

  GraphicsPath_SetMaxNoOfSubPaths( &_this->linePath, 11 );
}

/* Variants derived from the class : 'Dashboard::GaugeLabels' */
EW_DEFINE_CLASS_VARIANTS( DashboardGaugeLabels )
EW_END_OF_CLASS_VARIANTS( DashboardGaugeLabels )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeLabels' */
EW_DEFINE_CLASS( DashboardGaugeLabels, CoreGroup, labels, Outlet, Outlet, linePath, 
                 increment, increment, "Dashboard::GaugeLabels" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardGaugeLabels )

/* Initializer for the class 'Dashboard::GaugeIndicator' */
void DashboardGaugeIndicator__Init( DashboardGaugeIndicator _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardGaugeIndicator );

  /* ... then construct all embedded objects */
  ViewsWarpImage__Init( &_this->Needle, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGaugeIndicator );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0093 );
  _this->StartAngle = 180.000000f;
  _this->Scaling = 1.000000f;
  _this->MaxValue = 100;
  _this->CurrentValue = 1;
  _this->EndAngle = 405.000000f;
  CoreQuadView__OnSetPoint4( &_this->Needle, _Const0094 );
  CoreQuadView__OnSetPoint3( &_this->Needle, _Const0095 );
  CoreQuadView__OnSetPoint2( &_this->Needle, _Const0096 );
  CoreQuadView__OnSetPoint1( &_this->Needle, _Const0097 );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->Needle, _Const0098 );
  ViewsWarpImage_OnSetColor( &_this->Needle, _Const0099 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Needle ), 0 );
  _this->SegmentImage = EwLoadResource( &DashboardGaugeGlowSegment, ResourcesBitmap );
  ViewsWarpImage_OnSetBitmap( &_this->Needle, EwLoadResource( &DashboardGaugeNeedle, 
  ResourcesBitmap ));

  /* Call the user defined constructor */
  DashboardGaugeIndicator_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::GaugeIndicator' */
void DashboardGaugeIndicator__ReInit( DashboardGaugeIndicator _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsWarpImage__ReInit( &_this->Needle );
}

/* Finalizer method for the class 'Dashboard::GaugeIndicator' */
void DashboardGaugeIndicator__Done( DashboardGaugeIndicator _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsWarpImage__Done( &_this->Needle );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void DashboardGaugeIndicator_Draw( DashboardGaugeIndicator _this, GraphicsCanvas 
  aCanvas, XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
{
  XFloat startAngle;
  XFloat deltaAngle;
  XInt32 dx;
  XInt32 dy;

  if ( _this->StartAngle < _this->EndAngle )
  {
    startAngle = _this->StartAngle;
    deltaAngle = _this->newRotationAngle - startAngle;
  }
  else
  {
    startAngle = _this->newRotationAngle;
    deltaAngle = _this->StartAngle - _this->newRotationAngle;
  }

  dx = (( _this->Super2.Bounds.Point2.X + _this->Super2.Bounds.Point1.X ) / 2 ) 
  + aOffset.X;
  dy = (( _this->Super2.Bounds.Point2.Y + _this->Super2.Bounds.Point1.Y ) / 2 ) 
  + aOffset.Y;
  DashboardGaugeIndicator_drawPie( _this, aCanvas, aClip, dx, dy, startAngle, deltaAngle, 
  _this->Scaling, _this->Displacement, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeGlowV1, 
  aOpacity, aBlend );
  CoreGroup_Draw((CoreGroup)_this, aCanvas, aClip, aOffset, aOpacity, aBlend );
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardGaugeIndicator_Init( DashboardGaugeIndicator _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  DashboardGaugeIndicator_drawNeedle( _this );
}

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetStartAngle()' */
void DashboardGaugeIndicator_OnSetStartAngle( DashboardGaugeIndicator _this, XFloat 
  value )
{
  if ( value == _this->StartAngle )
    return;

  _this->StartAngle = value;
  CoreGroup__InvalidateArea( _this, EwGetRectORect( _this->Super2.Bounds ));
}

/* 'C' function for method : 'Dashboard::GaugeIndicator.drawPie()' */
void DashboardGaugeIndicator_drawPie( DashboardGaugeIndicator _this, GraphicsCanvas 
  aCanvas, XRect aClip, XInt32 aX, XInt32 aY, XFloat aStartAngle, XFloat aDeltaAngle, 
  XFloat aScaling, XFloat aDisplacement, XColor aColor, XInt32 aOpacity, XBool aBlend )
{
  XFloat sinA;
  XFloat cosA;
  XFloat p1x;
  XFloat p1y;
  XFloat p2x;
  XFloat p2y;
  XFloat p3x;
  XFloat p3y;
  XFloat p4x;
  XFloat p4y;
  XRect clip;
  XColor c = aColor;
  XInt32 opacity = ((( aOpacity + 1 ) * _this->Super1.Opacity ) >> 8 ) + 1;
  XInt32 w;
  XInt32 h;
  XFloat quadAngle;

  aBlend = (XBool)( aBlend && (( _this->Super3.viewState & CoreViewStateAlphaBlended ) 
  == CoreViewStateAlphaBlended ));

  if ( opacity < 256 )
    c.Alpha = (XUInt8)(( c.Alpha * opacity ) >> 8 );

  if ( _this->SegmentImage == 0 )
    return;

  w = _this->SegmentImage->FrameSize.X;
  h = _this->SegmentImage->FrameSize.Y;
  quadAngle = 0.000000f;

  if ( aDeltaAngle >= 360.000000f )
  {
    aStartAngle = 0.000000f;
    aDeltaAngle = 360.000000f;
  }

  if ( aDisplacement > 0.000000f )
  {
    XFloat displaceAngle = aStartAngle + ( aDeltaAngle / 2.000000f );
    sinA = EwMathSin( displaceAngle );
    cosA = EwMathCos( displaceAngle );
    aX = aX + (XInt32)( aDisplacement * sinA );
    aY = aY - (XInt32)( aDisplacement * cosA );
  }

  while ( aStartAngle >= 90.000000f )
  {
    aStartAngle = aStartAngle - 90.000000f;
    quadAngle = quadAngle + 90.000000f;
  }

  if (( _this->tempImage == 0 ) || EwCompPoint( _this->tempImage->Super1.FrameSize, 
      _this->SegmentImage->FrameSize ))
  {
    _this->tempImage = EwNewObject( GraphicsCanvas, 0 );
    ResourcesBitmap__OnSetFrameSize( _this->tempImage, _this->SegmentImage->FrameSize );
  }

  if (( aStartAngle > 0.000000f ) && (( aStartAngle + aDeltaAngle ) < 90.000000f ))
  {
    sinA = EwMathSin( aDeltaAngle );
    cosA = EwMathCos( aDeltaAngle );
    p1x = (((XFloat)-w ) * cosA ) + ((XFloat)h * sinA );
    p1y = (((XFloat)-h ) * cosA ) - ((XFloat)w * sinA );
    p2x = (XFloat)h * sinA;
    p2y = ((XFloat)-h ) * cosA;
    p3x = 0.000000f;
    p3y = 0.000000f;
    p4x = ((XFloat)-w ) * cosA;
    p4y = ((XFloat)-w ) * sinA;
    GraphicsCanvas_FillRectangle( _this->tempImage, EwNewRect( 0, 0, w, h ), EwNewRect( 
    0, 0, _this->tempImage->Super1.FrameSize.X, _this->tempImage->Super1.FrameSize.Y ), 
    _Const009A, _Const009A, _Const009A, _Const009A, 0 );
    GraphicsCanvas_WarpBitmap( _this->tempImage, EwNewRect( 0, 0, w, h ), _this->SegmentImage, 
    0, p1x, p1y + (XFloat)h, 1.000000f, p2x, p2y + (XFloat)h, 1.000000f, p3x, p3y 
    + (XFloat)h, 1.000000f, p4x, p4y + (XFloat)h, 1.000000f, EwNewRect( 0, 0, w, 
    h ), c, c, c, c, 1, 1 );
    sinA = EwMathSin( aStartAngle + quadAngle ) * aScaling;
    cosA = EwMathCos( aStartAngle + quadAngle ) * aScaling;
    p1x = (XFloat)h * sinA;
    p1y = ((XFloat)-h ) * cosA;
    p2x = ((XFloat)w * cosA ) + ((XFloat)h * sinA );
    p2y = (((XFloat)-h ) * cosA ) + ((XFloat)w * sinA );
    p3x = (XFloat)w * cosA;
    p3y = (XFloat)w * sinA;
    p4x = 0.000000f;
    p4y = 0.000000f;
    GraphicsCanvas_WarpBitmap( aCanvas, aClip, ((ResourcesBitmap)_this->tempImage ), 
    0, p1x + (XFloat)aX, p1y + (XFloat)aY, 1.000000f, p2x + (XFloat)aX, p2y + (XFloat)aY, 
    1.000000f, p3x + (XFloat)aX, p3y + (XFloat)aY, 1.000000f, p4x + (XFloat)aX, 
    p4y + (XFloat)aY, 1.000000f, EwNewRect( 0, 0, w, h ), _Const0001, _Const0001, 
    _Const0001, _Const0001, aBlend, 1 );
  }
  else
  {
    if ( aStartAngle > 0.000000f )
    {
      sinA = EwMathSin(( 90.000000f + quadAngle ) + aStartAngle ) * aScaling;
      cosA = EwMathCos(( 90.000000f + quadAngle ) + aStartAngle ) * aScaling;
      p1x = (((XFloat)-w ) * cosA ) + ((XFloat)h * sinA );
      p1y = (((XFloat)-h ) * cosA ) - ((XFloat)w * sinA );
      p2x = (XFloat)h * sinA;
      p2y = ((XFloat)-h ) * cosA;
      p3x = 0.000000f;
      p3y = 0.000000f;
      p4x = ((XFloat)-w ) * cosA;
      p4y = ((XFloat)-w ) * sinA;

      if ( quadAngle >= 360.000000f )
        quadAngle = quadAngle - 360.000000f;

      if ( quadAngle == 0.000000f )
        clip = EwNewRect( aX, -10000, 10000, aY );
      else
        if ( quadAngle == 90.000000f )
          clip = EwNewRect( aX, aY, 10000, 10000 );
        else
          if ( quadAngle == 180.000000f )
            clip = EwNewRect( -10000, aY, aX, 10000 );
          else
            clip = EwNewRect( -10000, -10000, aX, aY );

      GraphicsCanvas_WarpBitmap( aCanvas, EwIntersectRect( aClip, clip ), _this->SegmentImage, 
      0, p1x + (XFloat)aX, p1y + (XFloat)aY, 1.000000f, p2x + (XFloat)aX, p2y + 
      (XFloat)aY, 1.000000f, p3x + (XFloat)aX, p3y + (XFloat)aY, 1.000000f, p4x 
      + (XFloat)aX, p4y + (XFloat)aY, 1.000000f, EwNewRect( 0, 0, w, h ), c, c, 
      c, c, aBlend, 1 );
      quadAngle = quadAngle + 90.000000f;

      if ( quadAngle >= 360.000000f )
        quadAngle = quadAngle - 360.000000f;

      aDeltaAngle = aDeltaAngle - ( 90.000000f - aStartAngle );
    }

    while ( aDeltaAngle >= 90.000000f )
    {
      sinA = EwMathSin( 90.000000f + quadAngle ) * aScaling;
      cosA = EwMathCos( 90.000000f + quadAngle ) * aScaling;
      p1x = (((XFloat)-w ) * cosA ) + ((XFloat)h * sinA );
      p1y = (((XFloat)-h ) * cosA ) - ((XFloat)w * sinA );
      p2x = (XFloat)h * sinA;
      p2y = ((XFloat)-h ) * cosA;
      p3x = 0.000000f;
      p3y = 0.000000f;
      p4x = ((XFloat)-w ) * cosA;
      p4y = ((XFloat)-w ) * sinA;

      if ( quadAngle >= 360.000000f )
        quadAngle = quadAngle - 360.000000f;

      if ( quadAngle == 0.000000f )
        clip = EwNewRect( aX, -10000, 10000, aY );
      else
        if ( quadAngle == 90.000000f )
          clip = EwNewRect( aX, aY, 10000, 10000 );
        else
          if ( quadAngle == 180.000000f )
            clip = EwNewRect( -10000, aY, aX, 10000 );
          else
            clip = EwNewRect( -10000, -10000, aX, aY );

      GraphicsCanvas_WarpBitmap( aCanvas, EwIntersectRect( aClip, clip ), _this->SegmentImage, 
      0, p1x + (XFloat)aX, p1y + (XFloat)aY, 1.000000f, p2x + (XFloat)aX, p2y + 
      (XFloat)aY, 1.000000f, p3x + (XFloat)aX, p3y + (XFloat)aY, 1.000000f, p4x 
      + (XFloat)aX, p4y + (XFloat)aY, 1.000000f, EwNewRect( 0, 0, w, h ), c, c, 
      c, c, aBlend, 1 );
      quadAngle = quadAngle + 90.000000f;

      if ( quadAngle >= 360.000000f )
        quadAngle = quadAngle - 360.000000f;

      aDeltaAngle = aDeltaAngle - 90.000000f;
    }

    sinA = EwMathSin( aDeltaAngle + quadAngle ) * aScaling;
    cosA = EwMathCos( aDeltaAngle + quadAngle ) * aScaling;
    p1x = (((XFloat)-w ) * cosA ) + ((XFloat)h * sinA );
    p1y = (((XFloat)-h ) * cosA ) - ((XFloat)w * sinA );
    p2x = (XFloat)h * sinA;
    p2y = ((XFloat)-h ) * cosA;
    p3x = 0.000000f;
    p3y = 0.000000f;
    p4x = ((XFloat)-w ) * cosA;
    p4y = ((XFloat)-w ) * sinA;

    if ( quadAngle == 0.000000f )
      clip = EwNewRect( aX, -10000, 10000, aY );
    else
      if ( quadAngle == 90.000000f )
        clip = EwNewRect( aX, aY, 10000, 10000 );
      else
        if ( quadAngle == 180.000000f )
          clip = EwNewRect( -10000, aY, aX, 10000 );
        else
          clip = EwNewRect( -10000, -10000, aX, aY );

    GraphicsCanvas_WarpBitmap( aCanvas, EwIntersectRect( aClip, clip ), _this->SegmentImage, 
    0, p1x + (XFloat)aX, p1y + (XFloat)aY, 1.000000f, p2x + (XFloat)aX, p2y + (XFloat)aY, 
    1.000000f, p3x + (XFloat)aX, p3y + (XFloat)aY, 1.000000f, p4x + (XFloat)aX, 
    p4y + (XFloat)aY, 1.000000f, EwNewRect( 0, 0, w, h ), c, c, c, c, aBlend, 1 );
  }
}

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetOutlet()' */
void DashboardGaugeIndicator_OnSetOutlet( DashboardGaugeIndicator _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardGaugeIndicator_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardGaugeIndicator_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardGaugeIndicator_onOutlet ), ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardGaugeIndicator_onOutlet( DashboardGaugeIndicator _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardGaugeIndicator_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetCurrentValue()' */
void DashboardGaugeIndicator_OnSetCurrentValue( DashboardGaugeIndicator _this, XInt32 
  value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  DashboardGaugeIndicator_drawNeedle( _this );
  CoreGroup__InvalidateArea( _this, EwGetRectORect( _this->Super2.Bounds ));
}

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetEndAngle()' */
void DashboardGaugeIndicator_OnSetEndAngle( DashboardGaugeIndicator _this, XFloat 
  value )
{
  if ( value == _this->EndAngle )
    return;

  _this->EndAngle = value;
  CoreGroup__InvalidateArea( _this, EwGetRectORect( _this->Super2.Bounds ));
}

/* 'C' function for method : 'Dashboard::GaugeIndicator.drawNeedle()' */
void DashboardGaugeIndicator_drawNeedle( DashboardGaugeIndicator _this )
{
  XPoint dstPos = EwNewPoint( EwGetRectW( _this->Super2.Bounds ) / 2, EwGetRectH( 
    _this->Super2.Bounds ) / 2 );

  if ( _this->MaxValue != _this->MinValue )
    _this->newRotationAngle = (((XFloat)( _this->CurrentValue - _this->MinValue ) 
    * ( _this->EndAngle - _this->StartAngle )) / (XFloat)( _this->MaxValue - _this->MinValue )) 
    + _this->StartAngle;

  ViewsWarpView__RotateAndScale( &_this->Needle, dstPos, 360.000000f - _this->newRotationAngle, 
  1.000000f, 1.000000f );
}

/* Variants derived from the class : 'Dashboard::GaugeIndicator' */
EW_DEFINE_CLASS_VARIANTS( DashboardGaugeIndicator )
EW_END_OF_CLASS_VARIANTS( DashboardGaugeIndicator )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeIndicator' */
EW_DEFINE_CLASS( DashboardGaugeIndicator, CoreGroup, SegmentImage, Outlet, Outlet, 
                 Needle, StartAngle, StartAngle, "Dashboard::GaugeIndicator" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  DashboardGaugeIndicator_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardGaugeIndicator )

/* Include a file containing the bitmap resource : 'Dashboard::GaugeGlowSegment' */
#include "_DashboardGaugeGlowSegment.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::GaugeGlowSegment' */
EW_RES_WITHOUT_VARIANTS( DashboardGaugeGlowSegment )

/* User defined auto object: 'Dashboard::LedButtonGreenConfig' */
EW_DEFINE_AUTOOBJECT( DashboardLedButtonGreenConfig, WidgetSetToggleButtonConfig )

/* Initializer for the auto object 'Dashboard::LedButtonGreenConfig' */
void DashboardLedButtonGreenConfig__Init( WidgetSetToggleButtonConfig _this )
{
  WidgetSetToggleButtonConfig_OnSetIconOnTintActive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnGreen );
  WidgetSetToggleButtonConfig_OnSetIconOnTintFocused( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnGreen );
  WidgetSetToggleButtonConfig_OnSetIconOnTintDisabled( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnGreen );
  WidgetSetToggleButtonConfig_OnSetIconOnTintDefault( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->LedsOnGreen );
  WidgetSetToggleButtonConfig_OnSetIconOffTintActive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
  WidgetSetToggleButtonConfig_OnSetIconOffTintFocused( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
  WidgetSetToggleButtonConfig_OnSetIconOffTintDisabled( _this, EwGetAutoObject( 
  &DashboardColors, DashboardColorsClass )->TextLedsOff );
  WidgetSetToggleButtonConfig_OnSetIconOffTintDefault( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
}

/* Table with links to derived variants of the auto object : 'Dashboard::LedButtonGreenConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardLedButtonGreenConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardLedButtonGreenConfig )

/* User defined auto object: 'Dashboard::StartEngineButtonConfig' */
EW_DEFINE_AUTOOBJECT( DashboardStartEngineButtonConfig, WidgetSetPushButtonConfig )

/* Initializer for the auto object 'Dashboard::StartEngineButtonConfig' */
void DashboardStartEngineButtonConfig__Init( WidgetSetPushButtonConfig _this )
{
  WidgetSetPushButtonConfig_OnSetFaceActive( _this, EwLoadResource( &DashboardStartEngine, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceFocused( _this, EwLoadResource( &DashboardStartEngine, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceDisabled( _this, EwLoadResource( &DashboardStartEngine, 
  ResourcesBitmap ));
  WidgetSetPushButtonConfig_OnSetFaceDefault( _this, EwLoadResource( &DashboardStartEngine, 
  ResourcesBitmap ));
}

/* Table with links to derived variants of the auto object : 'Dashboard::StartEngineButtonConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardStartEngineButtonConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardStartEngineButtonConfig )

/* Include a file containing the bitmap resource : 'Dashboard::StartEngineStates' */
#include "_DashboardStartEngineStates.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::StartEngineStates' */
EW_RES_WITHOUT_VARIANTS( DashboardStartEngineStates )

/* Include a file containing the bitmap resource : 'Dashboard::SwitchIcon' */
#include "_DashboardSwitchIcon.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::SwitchIcon' */
EW_RES_WITHOUT_VARIANTS( DashboardSwitchIcon )

/* Include a file containing the bitmap resource : 'Dashboard::OffIcon' */
#include "_DashboardOffIcon.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::OffIcon' */
EW_RES_WITHOUT_VARIANTS( DashboardOffIcon )

/* User defined auto object: 'Dashboard::DashboardButtonConfig' */
EW_DEFINE_AUTOOBJECT( DashboardDashboardButtonConfig, WidgetSetPushButtonConfig )

/* Initializer for the auto object 'Dashboard::DashboardButtonConfig' */
void DashboardDashboardButtonConfig__Init( WidgetSetPushButtonConfig _this )
{
  WidgetSetPushButtonConfig_OnSetIconTintActive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
  WidgetSetPushButtonConfig_OnSetIconTintDisabled( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->TextLedsOff );
  WidgetSetPushButtonConfig_OnSetIconTintDefault( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
}

/* Table with links to derived variants of the auto object : 'Dashboard::DashboardButtonConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardDashboardButtonConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardDashboardButtonConfig )

/* Include a file containing the bitmap resource : 'Dashboard::Logo' */
#include "_DashboardLogo.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Logo' */
EW_RES_WITHOUT_VARIANTS( DashboardLogo )

/* Initializer for the class 'Dashboard::DemoMode' */
void DashboardDemoMode__Init( DashboardDemoMode _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardDemoMode );

  /* ... then construct all embedded objects */
  EffectsInt32Effect__Init( &_this->DemoModeTimer, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->SpeedEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->PowerEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->BatteryEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->ODOEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDemoMode );

  /* ... and initialize objects, variables, properties, etc. */
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->DemoModeTimer, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->SpeedEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->PowerEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->BatteryEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->ODOEffect, 1 );
  _this->DemoModeTimer.Super1.OnFinished = EwNewSlot( _this, DashboardDemoMode_onDemoModeCycleFinished );
  _this->SpeedEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed );
  _this->PowerEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy );
  _this->BatteryEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery );
  _this->ODOEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetODO, DashboardDeviceClass_OnSetODO );

  /* Call the user defined constructor */
  DashboardDemoMode_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::DemoMode' */
void DashboardDemoMode__ReInit( DashboardDemoMode _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  EffectsInt32Effect__ReInit( &_this->DemoModeTimer );
  EffectsInt32Effect__ReInit( &_this->SpeedEffect );
  EffectsInt32Effect__ReInit( &_this->PowerEffect );
  EffectsInt32Effect__ReInit( &_this->BatteryEffect );
  EffectsInt32Effect__ReInit( &_this->ODOEffect );
}

/* Finalizer method for the class 'Dashboard::DemoMode' */
void DashboardDemoMode__Done( DashboardDemoMode _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Finalize all embedded objects */
  EffectsInt32Effect__Done( &_this->DemoModeTimer );
  EffectsInt32Effect__Done( &_this->SpeedEffect );
  EffectsInt32Effect__Done( &_this->PowerEffect );
  EffectsInt32Effect__Done( &_this->BatteryEffect );
  EffectsInt32Effect__Done( &_this->ODOEffect );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'Dashboard::DemoMode.onDemoModeCycleFinished()' */
void DashboardDemoMode_onDemoModeCycleFinished( DashboardDemoMode _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->DemoStep == 0 )
  {
    EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 2200 );
    DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 0, 100, EffectsTimingExp_Out, 
    2000 );
    DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 0, 78, EffectsTimingExp_Out, 
    1500 );
    DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 67, 65, EffectsTimingExp_Out, 
    2200 );
    DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 567, 569, EffectsTimingExp_Out, 
    2200 );
  }
  else
    if ( _this->DemoStep == 1 )
    {
      EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 2200 );
      DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 100, 77, EffectsTimingExp_Out, 
      2200 );
      DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 78, 18, EffectsTimingExp_Out, 
      1900 );
      DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 65, 65, EffectsTimingExp_Out, 
      2200 );
      DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 569, 570, EffectsTimingExp_Out, 
      2200 );
    }
    else
      if ( _this->DemoStep == 2 )
      {
        EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 1100 );
        DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 77, 37, EffectsTimingExp_Out, 
        1100 );
        DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 18, 10, EffectsTimingExp_Out, 
        800 );
        DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 65, 65, EffectsTimingExp_Out, 
        1100 );
        DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 270, 270, EffectsTimingExp_Out, 
        1100 );
      }
      else
        if ( _this->DemoStep == 3 )
        {
          EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 
          2000 );
          DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 37, 55, EffectsTimingExp_Out, 
          1700 );
          DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 10, 15, EffectsTimingExp_Out, 
          1500 );
          DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 65, 64, EffectsTimingExp_Out, 
          1700 );
          DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 270, 271, EffectsTimingExp_Out, 
          1700 );
        }
        else
          if ( _this->DemoStep == 4 )
          {
            EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 
            2600 );
            DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 55, 100, 
            EffectsTimingExp_Out, 2600 );
            DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 15, 67, EffectsTimingExp_Out, 
            1800 );
            DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 64, 62, 
            EffectsTimingExp_Out, 2600 );
            DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 271, 271, EffectsTimingExp_Out, 
            2600 );
          }
          else
            if ( _this->DemoStep == 5 )
            {
              EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 
              4500 );
              DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 100, 0, 
              EffectsTimingExp_Out, 3500 );
              DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 67, 0, 
              EffectsTimingExp_Out, 1300 );
              DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 62, 62, 
              EffectsTimingExp_Out, 2600 );
              DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 271, 272, 
              EffectsTimingExp_Out, 2600 );
            }

  _this->DemoStep = ( _this->DemoStep + 1 ) % 6;
  DashboardDeviceClass_OnSetODO( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  EwPostSignal( EwNewSlot( &_this->DemoModeTimer, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::DemoMode.StartEffect()' */
void DashboardDemoMode_StartEffect( DashboardDemoMode _this, EffectsInt32Effect 
  aEffect, XInt32 aValue1, XInt32 aValue2, XEnum aTiming, XInt32 aCycleDuration )
{
  aEffect->Value1 = aValue1;
  aEffect->Value2 = aValue2;
  EffectsEffect_OnSetTiming((EffectsEffect)aEffect, aTiming );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)aEffect, aCycleDuration );
  EwSignal( EwNewSlot( aEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardDemoMode_Init( DashboardDemoMode _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  EwAttachRefObserver( EwNewSlot( _this, DashboardDemoMode_onDemoModeChanged ), 
    EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetDemoMode, 
    DashboardDeviceClass_OnSetDemoMode ), 0 );
  EwPostSignal( EwNewSlot( _this, DashboardDemoMode_onDemoModeChanged ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::DemoMode.onDemoModeChanged()' */
void DashboardDemoMode_onDemoModeChanged( DashboardDemoMode _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass )->DemoMode )
    EwSignal( EwNewSlot( &_this->DemoModeTimer, EffectsEffect_StartEffect ), ((XObject)_this ));
  else
    EwSignal( EwNewSlot( &_this->DemoModeTimer, EffectsEffect_StopEffect ), ((XObject)_this ));
}

/* Variants derived from the class : 'Dashboard::DemoMode' */
EW_DEFINE_CLASS_VARIANTS( DashboardDemoMode )
EW_END_OF_CLASS_VARIANTS( DashboardDemoMode )

/* Virtual Method Table (VMT) for the class : 'Dashboard::DemoMode' */
EW_DEFINE_CLASS( DashboardDemoMode, XObject, DemoModeTimer, DemoModeTimer, DemoModeTimer, 
                 DemoModeTimer, DemoStep, DemoStep, "Dashboard::DemoMode" )
EW_END_OF_CLASS( DashboardDemoMode )

/* User defined auto object: 'Dashboard::FadeInOutBottom' */
EW_DEFINE_AUTOOBJECT( DashboardFadeInOutBottom, EffectsFadeInOutTransition )

/* Initializer for the auto object 'Dashboard::FadeInOutBottom' */
void DashboardFadeInOutBottom__Init( EffectsFadeInOutTransition _this )
{
  _this->Alignment = EffectsDialogAlignmentAlignHorzCenter | EffectsDialogAlignmentAlignVertBottom;
}

/* Table with links to derived variants of the auto object : 'Dashboard::FadeInOutBottom' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardFadeInOutBottom )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardFadeInOutBottom )

/* User defined auto object: 'Dashboard::FadeInOutCentered' */
EW_DEFINE_AUTOOBJECT( DashboardFadeInOutCentered, EffectsFadeInOutTransition )

/* Initializer for the auto object 'Dashboard::FadeInOutCentered' */
void DashboardFadeInOutCentered__Init( EffectsFadeInOutTransition _this )
{
  _this->Alignment = EffectsDialogAlignmentAlignHorzCenter | EffectsDialogAlignmentAlignVertCenter;
}

/* Table with links to derived variants of the auto object : 'Dashboard::FadeInOutCentered' */
EW_DEFINE_AUTOOBJECT_VARIANTS( DashboardFadeInOutCentered )
EW_END_OF_AUTOOBJECT_VARIANTS( DashboardFadeInOutCentered )

/* Include a file containing the font resource : 'Dashboard::FontL' */
#include "_DashboardFontL.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontL' */
EW_RES_WITHOUT_VARIANTS( DashboardFontL )

/* Include a file containing the font resource : 'Dashboard::FontXXL' */
#include "_DashboardFontXXL.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontXXL' */
EW_RES_WITHOUT_VARIANTS( DashboardFontXXL )

/* Include a file containing the font resource : 'Dashboard::FontXL' */
#include "_DashboardFontXL.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontXL' */
EW_RES_WITHOUT_VARIANTS( DashboardFontXL )

/* Include a file containing the font resource : 'Dashboard::FontLightL' */
#include "_DashboardFontLightL.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontLightL' */
EW_RES_WITHOUT_VARIANTS( DashboardFontLightL )

/* Include a file containing the font resource : 'Dashboard::FontLightXL' */
#include "_DashboardFontLightXL.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontLightXL' */
EW_RES_WITHOUT_VARIANTS( DashboardFontLightXL )

/* Include a file containing the font resource : 'Dashboard::FontLightM' */
#include "_DashboardFontLightM.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontLightM' */
EW_RES_WITHOUT_VARIANTS( DashboardFontLightM )

/* Embedded Wizard */
