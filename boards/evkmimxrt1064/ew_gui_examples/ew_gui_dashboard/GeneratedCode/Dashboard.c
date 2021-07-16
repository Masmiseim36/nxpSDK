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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#include "ewlocale.h"
#include "_CoreGroup.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_DashboardAnimationClass.h"
#include "_DashboardBattery.h"
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
#include "_DashboardLeftBottomBar.h"
#include "_DashboardLeftTopBar.h"
#include "_DashboardRightValueBar.h"
#include "_DashboardStartScreen.h"
#include "_EffectsFadeInOutTransition.h"
#include "_EffectsFloatEffect.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsTransition.h"
#include "_GraphicsCanvas.h"
#include "_GraphicsPath.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
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
  0x00000872, /* ratio 39.04 % */
  0xB8106F00, 0x0009A452, 0x00720010, 0x05C00190, 0x62001B00, 0xB1318000, 0x001644A2,
  0x986C2C68, 0x8A8D4011, 0x444A2914, 0x43A2A378, 0x7E4B1486, 0xA3000914, 0x210E92C6,
  0x11F86432, 0x388E4BE1, 0xD8B4AE2A, 0x8746E3D2, 0xDCD61525, 0x46A1D1F8, 0x7E7B348A,
  0x21F09984, 0x01079500, 0x7109E8CC, 0x538943AA, 0x8A6D6A15, 0x9CC220D0, 0x04A29D56,
  0xC527B529, 0x22113E93, 0x63932A89, 0x913B8949, 0xD128D546, 0x21894E61, 0x3844322B,
  0x8A42A1D6, 0xA5462371, 0x544DB088, 0xFA195386, 0x6738CA7D, 0x042D8C4E, 0xF0E8854F,
  0x0AC15D23, 0x1A898E11, 0x8D46F299, 0x88022F53, 0xCB2C967C, 0x4B23089C, 0x2291DC54,
  0x310A8FC4, 0x5509BE96, 0x39C16A63, 0xA9CC768D, 0xBAC623F1, 0xDC7E3BAD, 0x43A9DA3B,
  0x4273978A, 0x2151CD94, 0x510A8843, 0x5C089717, 0x8F8A5B61, 0x98A5967F, 0xA660B398,
  0xEF28016F, 0x5E2392E8, 0x178B23B2, 0xB5F1FC65, 0x9B6EDFD3, 0x5C49CC42, 0x538760B5,
  0xB8D42A35, 0xC171E3BC, 0x9DE4CD1A, 0x34959E44, 0x79FD5399, 0x9654D6D6, 0x54DEB779,
  0x10C49539, 0x4441AC46, 0x4D534291, 0x9579DB75, 0x60A11539, 0xD107AD18, 0x67491F79,
  0x313B7ADF, 0xB94F5FA4, 0x7A550535, 0x58B46991, 0x14F58D86, 0x6DBD81D6, 0x31429B74,
  0x4F9E2661, 0xE1753118, 0xE74E1A46, 0x02234F1F, 0x70701829, 0x779D0475, 0x5D659632,
  0xE945A259, 0x423F911A, 0x298B5B74, 0x461258FE, 0x2188E62D, 0xA86D2563, 0x9525431D,
  0xF0469104, 0x96204585, 0x561779D4, 0x94951D9A, 0xC925945D, 0x91916635, 0x4490C531,
  0x14D97E74, 0x38FE4B50, 0x31A599A4, 0x3D509E34, 0x56E51449, 0x66553941, 0xA95D6171,
  0xF9B14490, 0x4D78D684, 0x7AD91A4E, 0x9EF70A6A, 0xD6F1A560, 0x6D4E6C14, 0xC17211C9,
  0x8F66FA65, 0x5437BA49, 0x79A1AC64, 0xE221596A, 0xA858D824, 0x8FE566DE, 0x2717653B,
  0x34762AA1, 0x05EB6DE6, 0x8D7458C8, 0x87E3D632, 0x5F8999E1, 0xBA650C47, 0x35DD94DE,
  0x70AD5B79, 0x8BAC3A8E, 0x5699FD8C, 0x24FA1571, 0x368068AE, 0x80B154D7, 0x46DB3779,
  0x918A3139, 0x847D2C67, 0x1D1CBBA9, 0xF0AA14E5, 0x609DC6B1, 0xAAB60D4B, 0xC7C3014F,
  0xEE8C56D4, 0x4E499B64, 0xAE553962, 0x6914418B, 0x07A5794E, 0x35628E1C, 0xEC639C0B,
  0xC26E5B8D, 0xE84BA64A, 0x17C95890, 0x35DBC5D5, 0x3FA6B0F5, 0x54625B8A, 0xAAC53695,
  0x94F9F7AB, 0xAD08BA63, 0x0C631CF7, 0x6AD4CB36, 0x5ACA4E85, 0x8929F3BA, 0x0A166322,
  0xE17C5366, 0x5E5C07C1, 0x1516811A, 0x89C58C78, 0xB65871AE, 0x314218C7, 0x4714AC57,
  0x11DAED88, 0x9BE99E42, 0x6E1CA0A4, 0x69915EE4, 0x6FB4558F, 0x9C3C0F00, 0x6595994F,
  0x97455917, 0xD587E847, 0xA32EAD4E, 0x63C462CC, 0x75DEB659, 0xE70176B2, 0x1DC95FD8,
  0xA4D0A786, 0xE3F80C6C, 0xE8AF73DD, 0xB5534734, 0x398B666B, 0xB7F70DCD, 0x1F6B8F99,
  0xF4FAE6E7, 0xB6D3E5EF, 0xB87D579B, 0xC6B9EAE9, 0xC004EB98, 0x1003B4EC, 0x00000010,
  0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault1[] =
{
  0x0000075A, /* ratio 39.11 % */
  0xB80D5700, 0x000DA452, 0x00700010, 0x05C001C8, 0x64001B80, 0xB1318000, 0x2C000C62,
  0x3000D800, 0x8B8021A3, 0x5C8A191D, 0x8B401078, 0xD21D2888, 0x06B128A8, 0x5C009346,
  0xB108BC3A, 0x25264965, 0xC874AE57, 0x8A4BA2F0, 0x74662F14, 0x4421D129, 0x4A112E86,
  0x23F31A2C, 0x5719974A, 0x71B87442, 0x27845369, 0x1A74CE3B, 0xA6CAA9D5, 0x6C962D40,
  0xC7A7B252, 0x3249218E, 0xA29198DC, 0x91089B43, 0x16E88494, 0x2B8ECAC0, 0x98444247,
  0xB55123D7, 0x4D2A1524, 0x5DAAB1E8, 0x4E110CC0, 0xE474E87D, 0x0C00C148, 0xB4686422,
  0x0CBEC429, 0x78BC6625, 0x864A3291, 0xC46E570A, 0x43A154EA, 0x567F1B85, 0x24F2FA0C,
  0x9315B3E7, 0x9A68BCC2, 0x80CC46F0, 0xCAF6BA33, 0xD96E22B1, 0x6EA36BBB, 0x45E9DBAA,
  0x02291D87, 0x3871ED2F, 0x93BCD9D4, 0xA630055F, 0x15B7C6E4, 0x58EE1EF1, 0xED63265C,
  0xAEBE4B6B, 0xEF6E91DC, 0x2A9917E2, 0xB130A947, 0xA54DEBD3, 0x9E8C6582, 0xB4DEE9F0,
  0xEE6DF309, 0xEE7CA294, 0x1107C130, 0x67E9ED44, 0x110C611F, 0x4E6B5DB8, 0x431624ED,
  0x50847514, 0x44491E7A, 0xB90A4291, 0x1249E004, 0x6814D71D, 0x1BD84928, 0xB831C672,
  0xA1474B98, 0xDE421174, 0x6DD4D685, 0x51E4FA0C, 0x38A1105F, 0x6DF35F97, 0x12715104,
  0x6F51A569, 0xA137E51D, 0xA6A9E647, 0x81B54C19, 0xAC4A22F4, 0x4B90E60D, 0x1E77F90A,
  0x84ADF074, 0x3A1061E1, 0x9F65A4E5, 0x449307D1, 0x91D4320E, 0xE4565A8E, 0x9E475850,
  0x0A475B54, 0x6918C795, 0x92E8B975, 0x07D50C4B, 0x651B4B9F, 0x33871D36, 0x42A328FE,
  0x55F61E3C, 0xB641628D, 0x99EF551B, 0x369651E8, 0x839305EE, 0x52884561, 0xC869DE7D,
  0x9D25905C, 0x104518B4, 0x97E3B8C9, 0x1E163129, 0xE4C1A3A1, 0x814EA723, 0x307628A9,
  0x7C1CB9EA, 0xD0C4490A, 0xE836694A, 0x291B45D1, 0xA461E9D4, 0x7E18D78D, 0x975A815C,
  0x94E92361, 0x6DBDA0A9, 0x0C822634, 0x4A27E956, 0x19EB1EB2, 0xC7D65D5A, 0x82207C94,
  0xB1B22BD8, 0x5B1568FE, 0xE0962DD0, 0xEB1E8D61, 0x4AD45F5D, 0xE749E9F4, 0x491A2492,
  0xD2E87ADB, 0x1AE6A845, 0xA58799A3, 0xBB7B9BD6, 0x4518CB89, 0x9D09D566, 0x44566D83,
  0x6ECE6926, 0x0CBCDAC4, 0x5FECB83A, 0xE3E53AFC, 0x5851D2B2, 0x394F925C, 0xC39F28E5,
  0x82ABB4A2, 0x19D879F5, 0xBB1E1D61, 0xE2304498, 0xA77163FA, 0xB1DBE77E, 0xACCC1117,
  0xD4C32BB2, 0x93199D2E, 0x48655566, 0xA6B356AD, 0x6BBAA11E, 0xE7AEFEC4, 0xD2887D66,
  0x2A72D244, 0x4512499F, 0x15FA0630, 0x8CEA3099, 0x0AC67CAC, 0x8CD4EA05, 0xD7C008C1,
  0x00040760, 0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault2[] =
{
  0x0000090E, /* ratio 38.48 % */
  0xB8001500, 0x80086452, 0x00E00030, 0x0D2003A0, 0xDC003780, 0x60789800, 0x00279160,
  0xA8C00088, 0x02002263, 0xAC78DB16, 0x80069048, 0x00600019, 0x80D1E100, 0x440035C5,
  0xC058F1A2, 0x35000C62, 0xC824823C, 0x824BA272, 0x0104004D, 0x45A5A580, 0x1C004883,
  0x0084E8F1, 0x013895D0, 0x15217498, 0x56164DC0, 0x7000E401, 0x9CD50003, 0xB4B2932C,
  0x552C8001, 0x6EAA3893, 0x6BB50B5D, 0xD951AFDA, 0x32CB4D6E, 0x379BCB28, 0x7ACD7865,
  0xBD5E8015, 0x15FAB553, 0x18E596CC, 0x06B12AA8, 0xECB6E956, 0xB22DA849, 0x139B2552,
  0x58ADCDE5, 0xDAED52BB, 0x9663EF15, 0xD4A30391, 0x49A5564B, 0x9E5537AC, 0x6B728AAD,
  0x2783D057, 0xF69C1D73, 0x5DC76C74, 0xEC86D6F5, 0xCA4A2A98, 0x1E8B6D8F, 0x8033FA5C,
  0x9AF7A9C8, 0xB1DAD934, 0x1DA3BDF0, 0x7B0944AB, 0x64B4ECF1, 0xAAC4A2F5, 0xD9E6B612,
  0x076AB39D, 0x5EF91580, 0xC30596E0, 0xB99ABE4E, 0xCB5DB260, 0x3C4DF557, 0x7AC8F1A5,
  0xB006E765, 0xF3FE36DB, 0x5556E9B3, 0x37D5D075, 0x99F263DD, 0x037A18F5, 0x63DF371E,
  0xD287D9A4, 0x57B20E76, 0x21AE7B95, 0xD6839E46, 0x75164635, 0x59563992, 0xE6616956,
  0xF19C77E0, 0xC34B1835, 0x85207845, 0xE176A9E6, 0x94B21963, 0xCDB46E1F, 0xF08DDF28,
  0x58D268E5, 0xDCB83205, 0xE815A186, 0x0E1671A2, 0xF28F5CD9, 0x719D28B5, 0x18E7F21D,
  0x37761B60, 0x79B366DB, 0x006A2546, 0x5419788E, 0x5EF6B249, 0xF9198A55, 0x063C8824,
  0x584E6528, 0x5A5FB9A5, 0x5D183D60, 0xF829B671, 0xFD28545B, 0x6481A728, 0x92A32566,
  0x8006BD82, 0x35D9501E, 0xB15B5526, 0x58735839, 0x980764B2, 0x9507E98F, 0x977E5258,
  0xC57A4A93, 0x2A5518D9, 0x592714E5, 0x55D93D6F, 0x456D399C, 0xB1644DD5, 0x395406C4,
  0x5E5EE5C1, 0x5377C951, 0xF69A52AB, 0xE6B4A098, 0x8A60EAF4, 0x5A6B479A, 0x5744A2B6,
  0x063EAE54, 0xFD59AF6B, 0xC37B9F6A, 0xB1E62AE6, 0x17BB2644, 0xBAE262B0, 0xF2C6AE97,
  0xD3ACED1A, 0xB358FB2E, 0xADAB3ED7, 0x5AD6C4AC, 0xEEE1AB2D, 0xE5B6E99A, 0xB59EEB52,
  0x2EB7A6C5, 0x2B06CAB9, 0xEECE4B25, 0x54B4A30A, 0xBA2B7BB9, 0x6EC78AF1, 0x1B8262BD,
  0x72FB4DF0, 0x28B7AFDB, 0x13150A8D, 0xA92BAE90, 0xB87D58C3, 0xDF0E7857, 0x7F552D95,
  0x86DD97D9, 0x15B77D26, 0x49C702C2, 0x595D82B2, 0x7B84314C, 0xC6569725, 0x23749959,
  0x2A812A4A, 0x32769725, 0x55C99B3C, 0x9567077D, 0x9334B2E1, 0x163237C6, 0x13465EF1,
  0x4A6CF48D, 0x0034CA73, 0xAA167A86, 0x95A68A49, 0x2694B0E6, 0x827AB287, 0x4B324AD1,
  0x7234B310, 0x2AC69881, 0x41AB4A18, 0xCACBD2CB, 0x7261BCED, 0x3544DD81, 0xEDA16954,
  0xCA5CDB9C, 0x9A8A6DDD, 0x5EB0B60A, 0xD8EC4A5C, 0x15752C4D, 0xAE075E98, 0xAB4DF7AC,
  0x9036E5CD, 0xB6C4E6A1, 0x277EDCA2, 0xEE53E3EC, 0x5CB42195, 0x5EB76576, 0xEF8B6B29,
  0xFA798F95, 0x66A694B6, 0x8CE8DC05, 0xCAF976CF, 0xE9DB9788, 0xA49A71BF, 0x0EF3AC20,
  0x55A23B16, 0x7DA2CBAD, 0xEE3CD570, 0x2949BDA2, 0xE52C546F, 0xEB89ADD9, 0x585FB8DE,
  0xE4FEED50, 0xCAAA66A7, 0x7164B29B, 0x1ED97769, 0x00101479, 0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault3[] =
{
  0x000007E2, /* ratio 34.49 % */
  0xB80DDF00, 0x000DA452, 0x00640010, 0x062001A8, 0x6C001700, 0x8300042C, 0x2C004362,
  0xF1A46A12, 0x8D0E4010, 0x041E431F, 0xC5646630, 0x56211C8C, 0x470001BC, 0x46330044,
  0x5108B8B6, 0x0E98C322, 0x0964663F, 0x874B2591, 0x9C22630E, 0x46A61299, 0x66391589,
  0x22115874, 0x3F318E47, 0xB1AA1CB2, 0x0A960026, 0xFAAD2A5D, 0x89CB6B12, 0x8C422554,
  0x4328D369, 0x2A631D94, 0x2A946A44, 0x8D4F8ED3, 0xA008C54A, 0x1597DAA8, 0xDA200285,
  0x8957E571, 0xA456330A, 0x61AAF3CA, 0xCE331AA0, 0xE6F56874, 0x2D6E8FC5, 0xD72BAD3A,
  0x63CA62B0, 0x3BD45A8F, 0xAF55A153, 0xEE76E95A, 0xD8341128, 0xAE8D98BD, 0x65FA2CDD,
  0x3931B2E6, 0x39ABB552, 0x93B7EB6F, 0x8C347AA1, 0xA2EA24F0, 0xD44A5D7E, 0xE3A5F8CB,
  0xFB0BAD9C, 0x247418BC, 0x2FB6D5ED, 0x7BACA6E3, 0x5DCF422E, 0x8CBE8635, 0xCA76B4BD,
  0x851AC508, 0x73F2F94D, 0x362F6DBC, 0xA5995EA5, 0xD3B5B4F9, 0xDC2C3703, 0xCA91E0A8,
  0xB98C5F0F, 0x9C44F494, 0x5D3421BB, 0x9659515C, 0xE96C5DDA, 0x59619DD5, 0x82D1944D,
  0x6076C9A6, 0x47895B7C, 0x4DE07FD6, 0xEA795D08, 0x815945C1, 0xC7A00153, 0xA48C4D48,
  0x650C4390, 0x0B57D676, 0x1A1084BA, 0x95662123, 0xF4D20244, 0xB0008D12, 0x104C5231,
  0x8AD0A4B9, 0x1774B1D3, 0xC8F10843, 0xF61D83A2, 0x4669DAA4, 0x906488FA, 0x5469124B,
  0x29011A8F, 0x8E436199, 0x0E4BE294, 0x44E4F601, 0x12E4BA54, 0x296E524B, 0xD2559751,
  0x62476285, 0x96A6457E, 0xA3F9A1FE, 0xD57E609A, 0x99089BE6, 0x56979179, 0x98E6D96E,
  0xE729E9A7, 0x69DA7E97, 0xBE719958, 0x857F6734, 0xA26819DE, 0xA8D9D27C, 0x6131A79B,
  0x412E6EE2, 0xD89A53A4, 0x8C62F579, 0xD90429A9, 0x3A894545, 0xB2798E92, 0x467F52F9,
  0x4210A456, 0x5318D625, 0x07DDDE61, 0xD6309491, 0xBA5F9D88, 0x45DBC5F6, 0x50EAFEBB,
  0x5AA221B0, 0x2D1C4327, 0x49A1517A, 0xB1E78519, 0x26BB2994, 0xFACECD95, 0x12D1ADE8,
  0x87B56AAA, 0xB26D89C2, 0x61FB52DA, 0x9B7EA9B7, 0x8EDBA326, 0xC896ECFB, 0xBA6CBB9E,
  0x51DBADBA, 0xE9D6D2BB, 0x422EB712, 0xF09F6E0B, 0x44EDEB3A, 0xAFDA4AFB, 0xEC02E5B5,
  0xE15FB4EE, 0xE1BD2FFB, 0xC2EEAC26, 0xAE2BF2D9, 0x1BB2F96E, 0xFB13C0B1, 0x12C3305B,
  0xC4314C3B, 0x319BA301, 0x6C1EFAC1, 0x7F1F45AF, 0xB947D54A, 0xCB13369D, 0xB2F949EC,
  0xBCC91C87, 0x5A52CB5B, 0x4A816971, 0xCFF3E002, 0x00000101, 0x00000000
};

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault4[] =
{
  0x00000032, /* ratio 104.00 % */
  0xB8000B00, 0x800AA452, 0x1E120024, 0x09A45818, 0xC8003780, 0x10932800, 0x6B8602C0,
  0x0001B400, 0x899C8002, 0x1800C200, 0xC54D0003, 0x00010193, 0x00000000
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
static const XColor _Const0008 = { 0x50, 0x50, 0x50, 0xFF };
static const XColor _Const0009 = { 0x0B, 0x78, 0xAD, 0xFF };
static const XColor _Const000A = { 0x00, 0xF6, 0xFF, 0xFF };
static const XColor _Const000B = { 0x12, 0x14, 0x3D, 0xFF };
static const XColor _Const000C = { 0x00, 0xBB, 0xFF, 0xFF };
static const XColor _Const000D = { 0x4D, 0x4D, 0x4D, 0xFF };
static const XColor _Const000E = { 0x4C, 0x3D, 0xFD, 0xFF };
static const XRect _Const000F = {{ 0, 0 }, { 480, 272 }};
static const XRect _Const0010 = {{ 0, 0 }, { 240, 272 }};
static const XRect _Const0011 = {{ 240, 0 }, { 480, 272 }};
static const XRect _Const0012 = {{ 357, 228 }, { 453, 301 }};
static const XRect _Const0013 = {{ 28, 228 }, { 123, 301 }};
static const XRect _Const0014 = {{ 0, 112 }, { 101, 153 }};
static const XRect _Const0015 = {{ 379, 112 }, { 480, 153 }};
static const XRect _Const0016 = {{ 1, 164 }, { 103, 218 }};
static const XRect _Const0017 = {{ 378, 164 }, { 480, 218 }};
static const XRect _Const0018 = {{ 89, 0 }, { 392, 272 }};
static const XRect _Const0019 = {{ 222, 232 }, { 259, 245 }};
static const XStringRes _Const001A = { _StringsDefault0, 0x0002 };
static const XStringRes _Const001B = { _StringsDefault1, 0x0002 };
static const XRect _Const001C = {{ 147, 43 }, { 334, 230 }};
static const XStringRes _Const001D = { _StringsDefault2, 0x0002 };
static const XRect _Const001E = {{ 0, 230 }, { 160, 270 }};
static const XRect _Const001F = {{ 0, 0 }, { 70, 70 }};
static const XStringRes _Const0020 = { _StringsDefault2, 0x000C };
static const XStringRes _Const0021 = { _StringsDefault2, 0x0013 };
static const XRect _Const0022 = {{ 22, 53 }, { 60, 68 }};
static const XStringRes _Const0023 = { _StringsDefault2, 0x0017 };
static const XRect _Const0024 = {{ 13, 53 }, { 39, 68 }};
static const XRect _Const0025 = {{ 0, 50 }, { 70, 53 }};
static const XRect _Const0026 = {{ 0, 28 }, { 70, 44 }};
static const XStringRes _Const0027 = { _StringsDefault2, 0x001E };
static const XRect _Const0028 = {{ 0, 0 }, { 70, 30 }};
static const XStringRes _Const0029 = { _StringsDefault2, 0x0024 };
static const XStringRes _Const002A = { _StringsDefault2, 0x0029 };
static const XRect _Const002B = {{ 357, 0 }, { 480, 231 }};
static const XRect _Const002C = {{ 119, 20 }, { 359, 232 }};
static const XRect _Const002D = {{ 0, 0 }, { 123, 231 }};
static const XRect _Const002E = {{ 130, 223 }, { 151, 239 }};
static const XRect _Const002F = {{ 328, 224 }, { 349, 239 }};
static const XRect _Const0030 = {{ 325, 100 }, { 397, 262 }};
static const XRect _Const0031 = {{ 104, 100 }, { 155, 200 }};
static const XRect _Const0032 = {{ 81, 207 }, { 121, 262 }};
static const XRect _Const0033 = {{ 320, 90 }, { 340, 100 }};
static const XStringRes _Const0034 = { _StringsDefault2, 0x002D };
static const XRect _Const0035 = {{ 138, 90 }, { 158, 100 }};
static const XStringRes _Const0036 = { _StringsDefault2, 0x0033 };
static const XPoint _Const0037 = { -123, 0 };
static const XPoint _Const0038 = { 0, 0 };
static const XPoint _Const0039 = { 480, 0 };
static const XPoint _Const003A = { 357, 0 };
static const XStringRes _Const003B = { _StringsDefault2, 0x0039 };
static const XStringRes _Const003C = { _StringsDefault3, 0x0002 };
static const XRect _Const003D = {{ 0, 0 }, { 72, 162 }};
static const XRect _Const003E = {{ 0, 91 }, { 72, 160 }};
static const XRect _Const003F = {{ 0, 1 }, { 72, 91 }};
static const XRect _Const0040 = {{ 0, 0 }, { 37, 13 }};
static const XRect _Const0041 = {{ 3, 2 }, { 8, 11 }};
static const XRect _Const0042 = {{ 9, 2 }, { 14, 11 }};
static const XRect _Const0043 = {{ 15, 2 }, { 20, 11 }};
static const XRect _Const0044 = {{ 21, 2 }, { 26, 11 }};
static const XRect _Const0045 = {{ 27, 2 }, { 32, 11 }};
static const XRect _Const0046 = {{ 0, 0 }, { 303, 272 }};
static const XRect _Const0047 = {{ -1, 0 }, { 302, 245 }};
static const XRect _Const0048 = {{ 80, 62 }, { 222, 204 }};
static const XRect _Const0049 = {{ 77, 61 }, { 225, 209 }};
static const XRect _Const004A = {{ 64, 48 }, { 238, 222 }};
static const XRect _Const004B = {{ 79, 63 }, { 223, 207 }};
static const XRect _Const004C = {{ 29, 15 }, { 274, 260 }};
static const XRect _Const004D = {{ 80, 64 }, { 222, 206 }};
static const XRect _Const004E = {{ 28, 46 }, { 48, 61 }};
static const XStringRes _Const004F = { _StringsDefault4, 0x0002 };
static const XRect _Const0050 = {{ 425, 47 }, { 463, 62 }};
static const XStringRes _Const0051 = { _StringsDefault4, 0x0007 };
static const XRect _Const0052 = {{ 409, 3 }, { 479, 53 }};
static const XRect _Const0053 = {{ 5, 5 }, { 73, 55 }};
static const XRect _Const0054 = {{ 205, 102 }, { 275, 172 }};
static const XPoint _Const0055 = { 204, 172 };
static const XPoint _Const0056 = { 274, 172 };
static const XPoint _Const0057 = { 274, 102 };
static const XPoint _Const0058 = { 204, 102 };
static const XRect _Const0059 = {{ 28, 111 }, { 70, 153 }};
static const XRect _Const005A = {{ 47, 165 }, { 89, 207 }};
static const XRect _Const005B = {{ 54, 228 }, { 96, 270 }};
static const XRect _Const005C = {{ 373, 113 }, { 415, 155 }};
static const XRect _Const005D = {{ 395, 165 }, { 437, 207 }};
static const XRect _Const005E = {{ 418, 214 }, { 460, 256 }};
static const XRect _Const005F = {{ 186, 244 }, { 320, 264 }};
static const XStringRes _Const0060 = { _StringsDefault4, 0x000E };
static const XPoint _Const0061 = { 24, 215 };
static const XPoint _Const0062 = { 54, 228 };
static const XPoint _Const0063 = { 47, 165 };
static const XPoint _Const0064 = { 31, 167 };
static const XPoint _Const0065 = { 66, 115 };
static const XPoint _Const0066 = { 28, 111 };
static const XPoint _Const0067 = { 418, 214 };
static const XPoint _Const0068 = { 384, 228 };
static const XPoint _Const0069 = { 395, 165 };
static const XPoint _Const006A = { 407, 167 };
static const XPoint _Const006B = { 373, 113 };
static const XPoint _Const006C = { 410, 111 };
static const XRect _Const006D = {{ 0, 0 }, { 245, 245 }};
static const XRect _Const006E = {{ 0, 0 }, { 142, 142 }};
static const XPoint _Const006F = { 144, 289 };
static const XPoint _Const0070 = { 157, 289 };
static const XPoint _Const0071 = { 157, 231 };
static const XPoint _Const0072 = { 144, 231 };
static const XPoint _Const0073 = { 2, 72 };
static const XColor _Const0074 = { 0xFF, 0x00, 0x00, 0xFF };
static const XColor _Const0075 = { 0x00, 0x00, 0x00, 0x00 };
static const XRect _Const0076 = {{ 0, 0 }, { 51, 100 }};
static const XRect _Const0077 = {{ 0, 70 }, { 51, 98 }};
static const XRect _Const0078 = {{ 0, 1 }, { 51, 91 }};
static const XRect _Const0079 = {{ 0, 0 }, { 40, 55 }};
static const XRect _Const007A = {{ 1, 34 }, { 40, 53 }};
static const XRect _Const007B = {{ 0, 1 }, { 36, 36 }};

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
  _this->VertBarSelectedGlowV2 = _Const0006;
  _this->GaugeSelectionV2 = _Const0007;
  _this->BatteryFrame = _Const0008;
  _this->GaugeCircle0 = _Const0003;
  _this->GaugeCircle2 = _Const0009;
  _this->GaugeCircle3 = _Const000A;
  _this->GaugeCircle4 = _Const000B;
  _this->GaugeCircle5 = _Const000C;
  _this->VertBarUnselectedV2 = _Const000D;
  _this->VertBarRecoveryV2 = _Const000E;
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
  ViewsImage__Init( &_this->RightBottomGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftBottomGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftTopGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->RightTopGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftMiddleGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->RightMiddleGlow, &_this->_XObject, 0 );
  DashboardGauge__Init( &_this->SpeedGauge, &_this->_XObject, 0 );
  DashboardBattery__Init( &_this->Battery, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDashboardV1 );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const000F );
  CoreRectView__OnSetBounds( &_this->LeftStrokePath, _Const0010 );
  ViewsStrokePath_OnSetColor( &_this->LeftStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV1 );
  ViewsStrokePath_OnSetMiterLimit( &_this->LeftStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->LeftStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->LeftStrokePath, GraphicsPathCapRound );
  ViewsStrokePath_OnSetWidth( &_this->LeftStrokePath, 2.000000f );
  CoreRectView__OnSetBounds( &_this->RightStrokePath, _Const0011 );
  ViewsStrokePath_OnSetColor( &_this->RightStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->BatteryGaugeSelectionV1 );
  ViewsStrokePath_OnSetMiterLimit( &_this->RightStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->RightStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->RightStrokePath, GraphicsPathCapRound );
  ViewsStrokePath_OnSetWidth( &_this->RightStrokePath, 2.000000f );
  CoreRectView__OnSetBounds( &_this->RightBottomGlow, _Const0012 );
  ViewsImage_OnSetColor( &_this->RightBottomGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->LeftBottomGlow, _Const0013 );
  ViewsImage_OnSetColor( &_this->LeftBottomGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->LeftTopGlow, _Const0014 );
  ViewsImage_OnSetColor( &_this->LeftTopGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->RightTopGlow, _Const0015 );
  ViewsImage_OnSetColor( &_this->RightTopGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->LeftMiddleGlow, _Const0016 );
  ViewsImage_OnSetColor( &_this->LeftMiddleGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->RightMiddleGlow, _Const0017 );
  ViewsImage_OnSetColor( &_this->RightMiddleGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeGlowV1 );
  CoreRectView__OnSetBounds( &_this->SpeedGauge, _Const0018 );
  CoreRectView__OnSetBounds( &_this->Battery, _Const0019 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightBottomGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftBottomGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftTopGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightTopGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftMiddleGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightMiddleGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SpeedGauge ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Battery ), 0 );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftPath );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightPath );
  ViewsImage_OnSetBitmap( &_this->RightBottomGlow, EwLoadResource( &DashboardRightBottomGlowV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->LeftBottomGlow, EwLoadResource( &DashboardLeftBottomGlowV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->LeftTopGlow, EwLoadResource( &DashboardTopGlowV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->RightTopGlow, EwLoadResource( &DashboardTopGlowV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->LeftMiddleGlow, EwLoadResource( &DashboardLeftMiddleGlowV1, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->RightMiddleGlow, EwLoadResource( &DashboardRightMiddleGlowV1, 
  ResourcesBitmap ));
  DashboardGauge_OnSetOutlet( &_this->SpeedGauge, EwNewRef( EwGetAutoObject( &DashboardDevice, 
  DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ));
  DashboardBattery_OnSetOutlet( &_this->Battery, EwNewRef( EwGetAutoObject( &DashboardDevice, 
  DashboardDeviceClass ), DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery ));

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
  ViewsImage__ReInit( &_this->RightBottomGlow );
  ViewsImage__ReInit( &_this->LeftBottomGlow );
  ViewsImage__ReInit( &_this->LeftTopGlow );
  ViewsImage__ReInit( &_this->RightTopGlow );
  ViewsImage__ReInit( &_this->LeftMiddleGlow );
  ViewsImage__ReInit( &_this->RightMiddleGlow );
  DashboardGauge__ReInit( &_this->SpeedGauge );
  DashboardBattery__ReInit( &_this->Battery );
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
  ViewsImage__Done( &_this->RightBottomGlow );
  ViewsImage__Done( &_this->LeftBottomGlow );
  ViewsImage__Done( &_this->LeftTopGlow );
  ViewsImage__Done( &_this->RightTopGlow );
  ViewsImage__Done( &_this->LeftMiddleGlow );
  ViewsImage__Done( &_this->RightMiddleGlow );
  DashboardGauge__Done( &_this->SpeedGauge );
  DashboardBattery__Done( &_this->Battery );

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
  XInt32 maxNoOfSubPaths;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  DashboardGauge_OnSetOpacityLabels( &_this->SpeedGauge, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.400000f, 
  0.700000f, _this->CurrentProgress ));
  DashboardGauge_OnSetOpacityGauge( &_this->SpeedGauge, (XUInt8)DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.500000f, 
  0.900000f, _this->CurrentProgress ));
  CoreGroup__OnSetOpacity( &_this->Battery, DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.200000f, 
  0.800000f, _this->CurrentProgress ));
  maxNoOfSubPaths = GraphicsPath_GetMaxNoOfSubPaths( &_this->leftPath );
  GraphicsPath_SetMaxNoOfSubPaths( &_this->leftGrowingPath, maxNoOfSubPaths );
  DashboardDashboardV1_animateSubPath( _this, &_this->leftPath, 2, &_this->leftGrowingPath, 
  1, 0.150000f, 0.900000f );
  DashboardDashboardV1_animateSubPath( _this, &_this->leftPath, 1, &_this->leftGrowingPath, 
  1, 0.850000f, 0.900000f );
  DashboardDashboardV1_animateSubPath( _this, &_this->leftPath, 0, &_this->leftGrowingPath, 
  1, 0.850000f, 0.900000f );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftGrowingPath );
  maxNoOfSubPaths = GraphicsPath_GetMaxNoOfSubPaths( &_this->rightPath );
  GraphicsPath_SetMaxNoOfSubPaths( &_this->rightGrowingPath, maxNoOfSubPaths );
  DashboardDashboardV1_animateSubPath( _this, &_this->rightPath, 2, &_this->rightGrowingPath, 
  0, 0.150000f, 0.900000f );
  DashboardDashboardV1_animateSubPath( _this, &_this->rightPath, 1, &_this->rightGrowingPath, 
  0, 0.850000f, 0.900000f );
  DashboardDashboardV1_animateSubPath( _this, &_this->rightPath, 0, &_this->rightGrowingPath, 
  0, 0.850000f, 0.900000f );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightGrowingPath );
  ViewsImage_OnSetOpacity( &_this->LeftBottomGlow, DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.850000f, 
  1.000000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->LeftMiddleGlow, _this->LeftBottomGlow.Opacity );
  ViewsImage_OnSetOpacity( &_this->LeftTopGlow, _this->LeftBottomGlow.Opacity );
  ViewsImage_OnSetOpacity( &_this->RightBottomGlow, _this->LeftBottomGlow.Opacity );
  ViewsImage_OnSetOpacity( &_this->RightMiddleGlow, _this->LeftBottomGlow.Opacity );
  ViewsImage_OnSetOpacity( &_this->RightTopGlow, _this->LeftBottomGlow.Opacity );
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

  svg = EwLoadString( &_Const001A );
  GraphicsPath_CreateFromSVGString( &_this->leftPath, svg, 1.000000f, 1.000000f, 
  10.000000f, 153.000000f, 0.000000f );
  svg = EwLoadString( &_Const001B );
  GraphicsPath_CreateFromSVGString( &_this->rightPath, svg, 1.000000f, 1.000000f, 
  140.000000f, 153.000000f, 0.000000f );
}

/* Used to animate a specific sub path of the svg path. */
void DashboardDashboardV1_animateSubPath( DashboardDashboardV1 _this, GraphicsPath 
  aPath, XInt32 aPathNo, GraphicsPath aGrowingPath, XBool aReverse, XFloat aStartValue, 
  XFloat aEndValue )
{
  XInt32 noOfEdges = GraphicsPath_GetNoOfEdges( aPath, aPathNo );
  XInt32 progress = 0;

  GraphicsPath_InitSubPath( aGrowingPath, aPathNo, noOfEdges );

  if ( aReverse )
  {
    progress = DashboardAnimationClass_linearValueProgress( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), 0, noOfEdges, aStartValue, aEndValue, _this->CurrentProgress );
    GraphicsPath_AddCopy( aGrowingPath, aPathNo, aPath, aPathNo, 0, progress );
  }
  else
  {
    progress = noOfEdges - DashboardAnimationClass_linearValueProgress( EwGetAutoObject( 
    &DashboardAnimation, DashboardAnimationClass ), 0, noOfEdges, aStartValue, aEndValue, 
    _this->CurrentProgress );
    GraphicsPath_AddCopy( aGrowingPath, aPathNo, aPath, aPathNo, progress, noOfEdges 
    - progress );
  }
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
  CoreRectView__OnSetBounds( _this, _Const000F );
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->glowEffect, EffectsTimingFastIn_EaseOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->glowEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->glowEffect, 750 );
  CoreRectView__OnSetBounds( &_this->StartEngineGloom, _this->Super2.Bounds );
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
  ViewsText__Init( &_this->DescValue, &_this->_XObject, 0 );
  ViewsText__Init( &_this->DescText, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LineImage, &_this->_XObject, 0 );
  ViewsText__Init( &_this->UnitText, &_this->_XObject, 0 );
  ViewsText__Init( &_this->ValueText, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGaugeDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const001F );
  _this->Desc = EwShareString( EwLoadString( &_Const0020 ));
  _this->DescUnit = EwShareString( EwLoadString( &_Const0021 ));
  CoreRectView__OnSetBounds( &_this->DescValue, _Const0022 );
  ViewsText_OnSetAlignment( &_this->DescValue, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->DescValue, EwConcatString( EwLoadString( &_Const0023 ), 
  _this->DescUnit ));
  ViewsText_OnSetColor( &_this->DescValue, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->DescText, _Const0024 );
  ViewsText_OnSetAlignment( &_this->DescText, ViewsTextAlignmentAlignHorzLeft | 
  ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->DescText, _this->Desc );
  ViewsText_OnSetColor( &_this->DescText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->LineImage, _Const0025 );
  CoreRectView__OnSetBounds( &_this->UnitText, _Const0026 );
  ViewsText_OnSetString( &_this->UnitText, EwLoadString( &_Const0027 ));
  ViewsText_OnSetColor( &_this->UnitText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->ValueText, _Const0028 );
  ViewsText_OnSetString( &_this->ValueText, EwLoadString( &_Const0029 ));
  ViewsText_OnSetColor( &_this->ValueText, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DescValue ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DescText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LineImage ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->UnitText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ValueText ), 0 );
  ViewsText_OnSetFont( &_this->DescValue, EwLoadResource( &DashboardFontL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->DescText, EwLoadResource( &DashboardFontL, ResourcesFont ));
  ViewsImage_OnSetBitmap( &_this->LineImage, EwLoadResource( &DashboardHorizontalLine, 
  ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->UnitText, EwLoadResource( &DashboardFontXL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->ValueText, EwLoadResource( &DashboardFontXXL, ResourcesFont ));
}

/* Re-Initializer for the class 'Dashboard::GaugeDisplay' */
void DashboardGaugeDisplay__ReInit( DashboardGaugeDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsText__ReInit( &_this->DescValue );
  ViewsText__ReInit( &_this->DescText );
  ViewsImage__ReInit( &_this->LineImage );
  ViewsText__ReInit( &_this->UnitText );
  ViewsText__ReInit( &_this->ValueText );
}

/* Finalizer method for the class 'Dashboard::GaugeDisplay' */
void DashboardGaugeDisplay__Done( DashboardGaugeDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsText__Done( &_this->DescValue );
  ViewsText__Done( &_this->DescText );
  ViewsImage__Done( &_this->LineImage );
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
  {
    ViewsText_OnSetString( &_this->DescText, _this->Desc );
    ViewsText_OnSetString( &_this->DescValue, EwConcatString( EwConcatString( EwNewStringInt( 
    EwOnGetInt32( _this->OutletDesc ), 0, 10 ), EwLoadString( &_Const002A )), _this->DescUnit ));
  }
}

/* Variants derived from the class : 'Dashboard::GaugeDisplay' */
EW_DEFINE_CLASS_VARIANTS( DashboardGaugeDisplay )
EW_END_OF_CLASS_VARIANTS( DashboardGaugeDisplay )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeDisplay' */
EW_DEFINE_CLASS( DashboardGaugeDisplay, CoreGroup, Outlet, Outlet, Outlet, DescValue, 
                 Desc, _None, "Dashboard::GaugeDisplay" )
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

/* 'C' function for method : 'Dashboard::DeviceClass.OnSetRecuperation()' */
void DashboardDeviceClass_OnSetRecuperation( DashboardDeviceClass _this, XInt32 
  value )
{
  if ( _this->Recuperation == value )
    return;

  _this->Recuperation = value;
  EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetRecuperation, 
    DashboardDeviceClass_OnSetRecuperation ), 0 );
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

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of its setting or state value. */
void DashboardDeviceClass_UpdateRecuperation( DashboardDeviceClass _this, XInt32 
  aNewValue )
{
  if ( aNewValue != _this->Recuperation )
  {
    _this->Recuperation = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, DashboardDeviceClass_OnGetRecuperation, 
      DashboardDeviceClass_OnSetRecuperation ), 0 );
  }
}

/* Wrapper function for the non virtual method : 'Dashboard::DeviceClass.UpdateRecuperation()' */
void DashboardDeviceClass__UpdateRecuperation( void* _this, XInt32 aNewValue )
{
  DashboardDeviceClass_UpdateRecuperation((DashboardDeviceClass)_this, aNewValue );
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

/* Default onget method for the property 'Recuperation' */
XInt32 DashboardDeviceClass_OnGetRecuperation( DashboardDeviceClass _this )
{
  return _this->Recuperation;
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
  ViewsImage__Init( &_this->RightGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->CenterGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->LeftGlow, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->PWR, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->KW, &_this->_XObject, 0 );
  DashboardRightValueBar__Init( &_this->RightValueBar, &_this->_XObject, 0 );
  DashboardLeftTopBar__Init( &_this->LeftValueBarTop, &_this->_XObject, 0 );
  DashboardLeftBottomBar__Init( &_this->LeftValueBarBottom, &_this->_XObject, 0 );
  ViewsText__Init( &_this->MAX, &_this->_XObject, 0 );
  ViewsText__Init( &_this->CHG, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDashboardV2 );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const000F );
  CoreRectView__OnSetBounds( &_this->RightStrokePath, _Const0011 );
  ViewsStrokePath_OnSetColor( &_this->RightStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeSelectionV2 );
  ViewsStrokePath_OnSetMiterLimit( &_this->RightStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->RightStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->RightStrokePath, GraphicsPathCapRound );
  ViewsStrokePath_OnSetWidth( &_this->RightStrokePath, 2.000000f );
  CoreRectView__OnSetBounds( &_this->LeftStrokePath, _Const0010 );
  ViewsStrokePath_OnSetColor( &_this->LeftStrokePath, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->GaugeSelectionV2 );
  ViewsStrokePath_OnSetMiterLimit( &_this->LeftStrokePath, 10.000000f );
  ViewsStrokePath_OnSetJoinPoints( &_this->LeftStrokePath, GraphicsPathJoinMiter );
  ViewsStrokePath_OnSetStartCap( &_this->LeftStrokePath, GraphicsPathCapRound );
  ViewsStrokePath_OnSetWidth( &_this->LeftStrokePath, 2.000000f );
  CoreRectView__OnSetBounds( &_this->RightGlow, _Const002B );
  ViewsImage_OnSetColor( &_this->RightGlow, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->VertBarSelectedGlowV2 );
  CoreRectView__OnSetBounds( &_this->CenterGlow, _Const002C );
  ViewsImage_OnSetColor( &_this->CenterGlow, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->VertBarSelectedGlowV2 );
  CoreRectView__OnSetBounds( &_this->LeftGlow, _Const002D );
  ViewsImage_OnSetColor( &_this->LeftGlow, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->VertBarSelectedGlowV2 );
  CoreRectView__OnSetBounds( &_this->PWR, _Const002E );
  CoreRectView__OnSetBounds( &_this->KW, _Const002F );
  CoreRectView__OnSetBounds( &_this->RightValueBar, _Const0030 );
  CoreRectView__OnSetBounds( &_this->LeftValueBarTop, _Const0031 );
  CoreRectView__OnSetBounds( &_this->LeftValueBarBottom, _Const0032 );
  CoreRectView__OnSetBounds( &_this->MAX, _Const0033 );
  ViewsText_OnSetString( &_this->MAX, EwLoadString( &_Const0034 ));
  ViewsText_OnSetColor( &_this->MAX, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreRectView__OnSetBounds( &_this->CHG, _Const0035 );
  ViewsText_OnSetString( &_this->CHG, EwLoadString( &_Const0036 ));
  ViewsText_OnSetColor( &_this->CHG, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->Text );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftStrokePath ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CenterGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftGlow ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->PWR ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->KW ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RightValueBar ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftValueBarTop ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->LeftValueBarBottom ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->MAX ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->CHG ), 0 );
  ViewsStrokePath_OnSetPath( &_this->RightStrokePath, &_this->rightPath );
  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftPath );
  ViewsImage_OnSetBitmap( &_this->RightGlow, EwLoadResource( &DashboardRightGlowV2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->CenterGlow, EwLoadResource( &DashboardCenterGlowV2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->LeftGlow, EwLoadResource( &DashboardLeftGlowV2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->PWR, EwLoadResource( &DashboardPWRIcon, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->KW, EwLoadResource( &DashboardKWIcon, ResourcesBitmap ));
  DashboardRightValueBar_OnSetOutlet( &_this->RightValueBar, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ));
  DashboardRightValueBar_OnSetOutlet((DashboardRightValueBar)&_this->LeftValueBarTop, 
  EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetEnergy, 
  DashboardDeviceClass_OnSetEnergy ));
  DashboardRightValueBar_OnSetOutlet((DashboardRightValueBar)&_this->LeftValueBarBottom, 
  EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetRecuperation, 
  DashboardDeviceClass_OnSetRecuperation ));
  ViewsText_OnSetFont( &_this->MAX, EwLoadResource( &DashboardFontS, ResourcesFont ));
  ViewsText_OnSetFont( &_this->CHG, EwLoadResource( &DashboardFontS, ResourcesFont ));

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
  ViewsImage__ReInit( &_this->RightGlow );
  ViewsImage__ReInit( &_this->CenterGlow );
  ViewsImage__ReInit( &_this->LeftGlow );
  ViewsImage__ReInit( &_this->PWR );
  ViewsImage__ReInit( &_this->KW );
  DashboardRightValueBar__ReInit( &_this->RightValueBar );
  DashboardLeftTopBar__ReInit( &_this->LeftValueBarTop );
  DashboardLeftBottomBar__ReInit( &_this->LeftValueBarBottom );
  ViewsText__ReInit( &_this->MAX );
  ViewsText__ReInit( &_this->CHG );
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
  ViewsImage__Done( &_this->RightGlow );
  ViewsImage__Done( &_this->CenterGlow );
  ViewsImage__Done( &_this->LeftGlow );
  ViewsImage__Done( &_this->PWR );
  ViewsImage__Done( &_this->KW );
  DashboardRightValueBar__Done( &_this->RightValueBar );
  DashboardLeftTopBar__Done( &_this->LeftValueBarTop );
  DashboardLeftBottomBar__Done( &_this->LeftValueBarBottom );
  ViewsText__Done( &_this->MAX );
  ViewsText__Done( &_this->CHG );

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
  XInt32 i;
  XInt32 noOfEdges;
  XInt32 progress;
  XInt32 maxNoOfSubPaths;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  CoreRectView__OnSetBounds( &_this->LeftGlow, EwSetRectOrigin( _this->LeftGlow.Super1.Bounds, 
  DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), _Const0037, _Const0038, 0.300000f, 0.700000f, _this->CurrentProgress )));
  CoreRectView__OnSetBounds( &_this->RightGlow, EwSetRectOrigin( _this->RightGlow.Super1.Bounds, 
  DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
  DashboardAnimationClass ), _Const0039, _Const003A, 0.300000f, 0.700000f, _this->CurrentProgress )));
  ViewsImage_OnSetOpacity( &_this->LeftGlow, DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 80, 0.000000f, 
  0.400000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->RightGlow, _this->LeftGlow.Opacity );
  ViewsImage_OnSetOpacity( &_this->PWR, DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.900000f, 
  1.000000f, _this->CurrentProgress ));
  ViewsImage_OnSetOpacity( &_this->KW, _this->PWR.Opacity );
  CoreGroup__OnSetOpacity( &_this->LeftValueBarBottom, DashboardAnimationClass_linearValueProgress( 
  EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.500000f, 
  0.800000f, _this->CurrentProgress ));
  CoreGroup__OnSetOpacity( &_this->LeftValueBarTop, _this->LeftValueBarBottom.Super4.Opacity );
  CoreGroup__OnSetOpacity( &_this->RightValueBar, _this->LeftValueBarBottom.Super4.Opacity );
  ViewsText_OnSetOpacity( &_this->MAX, _this->LeftValueBarBottom.Super4.Opacity );
  ViewsText_OnSetOpacity( &_this->CHG, _this->LeftValueBarBottom.Super4.Opacity );

  if ( _this->CurrentProgress <= 0.700000f )
    ViewsImage_OnSetOpacity( &_this->CenterGlow, (XUInt8)DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, _this->centerGlowOpacity, 
    0.500000f, 0.700000f, _this->CurrentProgress ));

  maxNoOfSubPaths = GraphicsPath_GetMaxNoOfSubPaths( &_this->leftPath );
  GraphicsPath_SetMaxNoOfSubPaths( &_this->leftGrowingPath, maxNoOfSubPaths );

  for ( i = 0; i < maxNoOfSubPaths; i = i + 1 )
  {
    noOfEdges = GraphicsPath_GetNoOfEdges( &_this->leftPath, i );
    progress = noOfEdges - DashboardAnimationClass_linearValueProgress( EwGetAutoObject( 
    &DashboardAnimation, DashboardAnimationClass ), 0, noOfEdges, 0.850000f, 1.000000f, 
    _this->CurrentProgress );
    GraphicsPath_InitSubPath( &_this->leftGrowingPath, i, noOfEdges );
    GraphicsPath_AddCopy( &_this->leftGrowingPath, i, &_this->leftPath, i, progress, 
    noOfEdges - progress );
  }

  ViewsStrokePath_OnSetPath( &_this->LeftStrokePath, &_this->leftGrowingPath );
  maxNoOfSubPaths = GraphicsPath_GetMaxNoOfSubPaths( &_this->rightPath );
  GraphicsPath_SetMaxNoOfSubPaths( &_this->rightGrowingPath, maxNoOfSubPaths );

  for ( i = 0; i < maxNoOfSubPaths; i = i + 1 )
  {
    noOfEdges = GraphicsPath_GetNoOfEdges( &_this->rightPath, i );
    progress = DashboardAnimationClass_linearValueProgress( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), 0, noOfEdges, 0.850000f, 1.000000f, _this->CurrentProgress );
    GraphicsPath_InitSubPath( &_this->rightGrowingPath, i, noOfEdges );
    GraphicsPath_AddCopy( &_this->rightGrowingPath, i, &_this->rightPath, i, 0, 
    progress );
  }

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
  EwAttachRefObserver( EwNewSlot( _this, DashboardDashboardV2_onSpeedChanged ), 
    EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, 
    DashboardDeviceClass_OnSetSpeed ), 0 );
  EwPostSignal( EwNewSlot( _this, DashboardDashboardV2_onSpeedChanged ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::DashboardV2.createPath()' */
void DashboardDashboardV2_createPath( DashboardDashboardV2 _this, XObject sender )
{
  XString svg;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  svg = EwLoadString( &_Const003B );
  GraphicsPath_CreateFromSVGString( &_this->leftPath, svg, 1.000000f, 1.000000f, 
  44.000000f, 6.000000f, 0.000000f );
  svg = EwLoadString( &_Const003C );
  GraphicsPath_CreateFromSVGString( &_this->rightPath, svg, 1.000000f, 1.000000f, 
  -138.000000f, 6.000000f, 0.000000f );
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

/* 'C' function for method : 'Dashboard::DashboardV2.onSpeedChanged()' */
void DashboardDashboardV2_onSpeedChanged( DashboardDashboardV2 _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->CurrentProgress > 0.700000f )
  {
    ViewsImage_OnSetOpacity( &_this->CenterGlow, DashboardAnimationClass_linearValueProgress( 
    EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 100, 255, 0.000000f, 
    100.000000f, (XFloat)EwGetAutoObject( &DashboardDevice, DashboardDeviceClass )->Speed ));
    _this->centerGlowOpacity = _this->CenterGlow.Opacity;
  }
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

/* Initializer for the class 'Dashboard::RightValueBar' */
void DashboardRightValueBar__Init( DashboardRightValueBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  TemplatesVerticalValueBar__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardRightValueBar );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->BottomBricks, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->TopBricks, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardRightValueBar );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const003D );
  _this->MaxValue = 100;
  _this->CurrentValue = 20;
  CoreRectView__OnSetBounds( &_this->BottomBricks, _Const003E );
  ViewsImage_OnSetColor( &_this->BottomBricks, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->VertBarSelectedGlowV2 );
  ViewsImage_OnSetAlignment( &_this->BottomBricks, ViewsImageAlignmentAlignHorzRight 
  | ViewsImageAlignmentAlignVertBottom );
  CoreRectView__OnSetBounds( &_this->TopBricks, _Const003F );
  ViewsImage_OnSetColor( &_this->TopBricks, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->VertBarUnselectedV2 );
  ViewsImage_OnSetAlignment( &_this->TopBricks, ViewsImageAlignmentAlignHorzLeft 
  | ViewsImageAlignmentAlignVertTop );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->BottomBricks ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TopBricks ), 0 );
  ViewsImage_OnSetBitmap( &_this->BottomBricks, EwLoadResource( &DashboardRightBricksV2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->TopBricks, EwLoadResource( &DashboardRightBricksV2, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::RightValueBar' */
void DashboardRightValueBar__ReInit( DashboardRightValueBar _this )
{
  /* At first re-initialize the super class ... */
  TemplatesVerticalValueBar__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->BottomBricks );
  ViewsImage__ReInit( &_this->TopBricks );
}

/* Finalizer method for the class 'Dashboard::RightValueBar' */
void DashboardRightValueBar__Done( DashboardRightValueBar _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesVerticalValueBar );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->BottomBricks );
  ViewsImage__Done( &_this->TopBricks );

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
void DashboardRightValueBar_UpdateViewState( DashboardRightValueBar _this, XSet 
  aState )
{
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 newPos;
  XInt32 brickHeightWithSpace;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  minPos = EwGetRectH( _this->Super3.Bounds );
  maxPos = 0;
  newPos = minPos;
  brickHeightWithSpace = 9;

  if ( _this->MaxValue != _this->MinValue )
    newPos = ((( DashboardRightValueBar_OnGetCurrentValue( _this ) - _this->MinValue ) 
    * ( maxPos - minPos )) / ( _this->MaxValue - _this->MinValue )) + minPos;

  CoreRectView__OnSetBounds( &_this->TopBricks, EwSetRectY2( _this->TopBricks.Super1.Bounds, 
  newPos - ( newPos % brickHeightWithSpace )));
  CoreRectView__OnSetBounds( &_this->BottomBricks, EwSetRectY1( _this->BottomBricks.Super1.Bounds, 
  _this->TopBricks.Super1.Bounds.Point2.Y ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardRightValueBar_onOutlet( DashboardRightValueBar _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    DashboardRightValueBar_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'Dashboard::RightValueBar.OnSetOutlet()' */
void DashboardRightValueBar_OnSetOutlet( DashboardRightValueBar _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, DashboardRightValueBar_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, DashboardRightValueBar_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, DashboardRightValueBar_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::RightValueBar.OnGetCurrentValue()' */
XInt32 DashboardRightValueBar_OnGetCurrentValue( DashboardRightValueBar _this )
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

/* 'C' function for method : 'Dashboard::RightValueBar.OnSetCurrentValue()' */
void DashboardRightValueBar_OnSetCurrentValue( DashboardRightValueBar _this, XInt32 
  value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* Variants derived from the class : 'Dashboard::RightValueBar' */
EW_DEFINE_CLASS_VARIANTS( DashboardRightValueBar )
EW_END_OF_CLASS_VARIANTS( DashboardRightValueBar )

/* Virtual Method Table (VMT) for the class : 'Dashboard::RightValueBar' */
EW_DEFINE_CLASS( DashboardRightValueBar, TemplatesVerticalValueBar, Outlet, Outlet, 
                 Outlet, BottomBricks, MaxValue, MaxValue, "Dashboard::RightValueBar" )
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
  DashboardRightValueBar_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardRightValueBar )

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
  ViewsRectangle__Init( &_this->Energy1, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Energy2, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Energy3, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Energy4, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Energy5, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardBattery );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0040 );
  _this->MaxValue = 100;
  _this->CurrentValue = 100;
  CoreView_OnSetLayout((CoreView)&_this->Image, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->Image, _this->Super3.Bounds );
  ViewsImage_OnSetColor( &_this->Image, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->BatteryFrame );
  ViewsImage_OnSetAlignment( &_this->Image, ViewsImageAlignmentAlignHorzCenter | 
  ViewsImageAlignmentAlignVertCenter | ViewsImageAlignmentStretchToFill );
  CoreRectView__OnSetBounds( &_this->Energy1, _Const0041 );
  CoreRectView__OnSetBounds( &_this->Energy2, _Const0042 );
  CoreRectView__OnSetBounds( &_this->Energy3, _Const0043 );
  CoreRectView__OnSetBounds( &_this->Energy4, _Const0044 );
  CoreRectView__OnSetBounds( &_this->Energy5, _Const0045 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Image ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Energy1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Energy2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Energy3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Energy4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Energy5 ), 0 );
  ViewsImage_OnSetBitmap( &_this->Image, EwLoadResource( &DashboardBatteryFrame, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::Battery' */
void DashboardBattery__ReInit( DashboardBattery _this )
{
  /* At first re-initialize the super class ... */
  TemplatesHorizontalValueBar__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Image );
  ViewsRectangle__ReInit( &_this->Energy1 );
  ViewsRectangle__ReInit( &_this->Energy2 );
  ViewsRectangle__ReInit( &_this->Energy3 );
  ViewsRectangle__ReInit( &_this->Energy4 );
  ViewsRectangle__ReInit( &_this->Energy5 );
}

/* Finalizer method for the class 'Dashboard::Battery' */
void DashboardBattery__Done( DashboardBattery _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesHorizontalValueBar );

  /* Finalize all embedded objects */
  ViewsImage__Done( &_this->Image );
  ViewsRectangle__Done( &_this->Energy1 );
  ViewsRectangle__Done( &_this->Energy2 );
  ViewsRectangle__Done( &_this->Energy3 );
  ViewsRectangle__Done( &_this->Energy4 );
  ViewsRectangle__Done( &_this->Energy5 );

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
  CoreGroup_UpdateViewState((CoreGroup)_this, aState );

  if ( DashboardBattery_OnGetCurrentValue( _this ) >= 20 )
    ViewsRectangle_OnSetColor( &_this->Energy1, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryGaugeSelectionV1 );
  else
    ViewsRectangle_OnSetColor( &_this->Energy1, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryFrame );

  if ( DashboardBattery_OnGetCurrentValue( _this ) >= 40 )
    ViewsRectangle_OnSetColor( &_this->Energy2, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryGaugeSelectionV1 );
  else
    ViewsRectangle_OnSetColor( &_this->Energy2, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryFrame );

  if ( DashboardBattery_OnGetCurrentValue( _this ) >= 60 )
    ViewsRectangle_OnSetColor( &_this->Energy3, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryGaugeSelectionV1 );
  else
    ViewsRectangle_OnSetColor( &_this->Energy3, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryFrame );

  if ( DashboardBattery_OnGetCurrentValue( _this ) >= 80 )
    ViewsRectangle_OnSetColor( &_this->Energy4, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryGaugeSelectionV1 );
  else
    ViewsRectangle_OnSetColor( &_this->Energy4, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryFrame );

  if ( DashboardBattery_OnGetCurrentValue( _this ) >= 100 )
    ViewsRectangle_OnSetColor( &_this->Energy5, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryGaugeSelectionV1 );
  else
    ViewsRectangle_OnSetColor( &_this->Energy5, EwGetAutoObject( &DashboardColors, 
    DashboardColorsClass )->BatteryFrame );
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

/* Include a file containing the bitmap resource : 'Dashboard::GaugeNeedle' */
#include "_DashboardGaugeNeedle.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::GaugeNeedle' */
EW_RES_WITHOUT_VARIANTS( DashboardGaugeNeedle )

/* Initializer for the class 'Dashboard::Gauge' */
void DashboardGauge__Init( DashboardGauge _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardGauge );

  /* ... then construct all embedded objects */
  ViewsImage__Init( &_this->Circle5, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle4, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle3, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle2, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Circle0, &_this->_XObject, 0 );
  DashboardGaugeLabels__Init( &_this->GaugeLabels, &_this->_XObject, 0 );
  DashboardGaugeIndicator__Init( &_this->GaugeIndicator, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGauge );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0046 );
  CoreRectView__OnSetBounds( &_this->Circle5, _Const0047 );
  ViewsImage_OnSetColor( &_this->Circle5, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle5 );
  CoreRectView__OnSetBounds( &_this->Circle4, _Const0048 );
  ViewsImage_OnSetColor( &_this->Circle4, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle4 );
  CoreRectView__OnSetBounds( &_this->Circle3, _Const0049 );
  ViewsImage_OnSetColor( &_this->Circle3, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle3 );
  CoreRectView__OnSetBounds( &_this->Circle2, _Const004A );
  ViewsImage_OnSetColor( &_this->Circle2, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle2 );
  CoreRectView__OnSetBounds( &_this->Circle0, _Const004B );
  ViewsImage_OnSetColor( &_this->Circle0, EwGetAutoObject( &DashboardColors, DashboardColorsClass )->GaugeCircle0 );
  CoreRectView__OnSetBounds( &_this->GaugeLabels, _Const004C );
  CoreRectView__OnSetBounds( &_this->GaugeIndicator, _Const004D );
  DashboardGaugeIndicator_OnSetCurrentValue( &_this->GaugeIndicator, 1 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle5 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle4 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle3 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Circle0 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->GaugeLabels ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->GaugeIndicator ), 0 );
  ViewsImage_OnSetBitmap( &_this->Circle5, EwLoadResource( &DashboardCircle5, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle4, EwLoadResource( &DashboardCircle4, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle3, EwLoadResource( &DashboardCircle3, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle2, EwLoadResource( &DashboardCircle2, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Circle0, EwLoadResource( &DashboardCircle0, ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::Gauge' */
void DashboardGauge__ReInit( DashboardGauge _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsImage__ReInit( &_this->Circle5 );
  ViewsImage__ReInit( &_this->Circle4 );
  ViewsImage__ReInit( &_this->Circle3 );
  ViewsImage__ReInit( &_this->Circle2 );
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
  ViewsImage__Done( &_this->Circle5 );
  ViewsImage__Done( &_this->Circle4 );
  ViewsImage__Done( &_this->Circle3 );
  ViewsImage__Done( &_this->Circle2 );
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
  ViewsImage_OnSetOpacity( &_this->Circle2, value );
  ViewsImage_OnSetOpacity( &_this->Circle3, value );
  ViewsImage_OnSetOpacity( &_this->Circle4, value );
  ViewsImage_OnSetOpacity( &_this->Circle5, value );
}

/* Variants derived from the class : 'Dashboard::Gauge' */
EW_DEFINE_CLASS_VARIANTS( DashboardGauge )
EW_END_OF_CLASS_VARIANTS( DashboardGauge )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Gauge' */
EW_DEFINE_CLASS( DashboardGauge, CoreGroup, Outlet, Outlet, Outlet, Circle5, OpacityGauge, 
                 OpacityGauge, "Dashboard::Gauge" )
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
  EffectsFloatEffect__Init( &_this->shutdownVariantEffect, &_this->_XObject, 0 );
  EffectsFloatEffect__Init( &_this->gaugeDisplayEffect, &_this->_XObject, 0 );
  EffectsFloatEffect__Init( &_this->shutdownGaugeDisplayEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->gaugeCheckEffect, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->ledOffEffect, &_this->_XObject, 0 );
  ViewsRectangle__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsText__Init( &_this->UI, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Mode, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->ShutdownButton, &_this->_XObject, 0 );
  WidgetSetPushButton__Init( &_this->TransitionButton, &_this->_XObject, 0 );
  DashboardDashboardV2__Init( &_this->DashboardV2, &_this->_XObject, 0 );
  DashboardDashboardV1__Init( &_this->DashboardV1, &_this->_XObject, 0 );
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
  CoreRectView__OnSetBounds( _this, _Const000F );
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
  EffectsEffect_OnSetTiming((EffectsEffect)&_this->shutdownVariantEffect, EffectsTimingEaseIn_FastOut );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->shutdownVariantEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->shutdownVariantEffect, 
  3000 );
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
  CoreRectView__OnSetBounds( &_this->Background, _this->Super2.Bounds );
  ViewsRectangle_OnSetColor( &_this->Background, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Background );
  CoreRectView__OnSetBounds( &_this->UI, _Const004E );
  ViewsText_OnSetString( &_this->UI, EwLoadString( &_Const004F ));
  CoreRectView__OnSetBounds( &_this->Mode, _Const0050 );
  ViewsText_OnSetString( &_this->Mode, EwLoadString( &_Const0051 ));
  CoreRectView__OnSetBounds( &_this->ShutdownButton, _Const0052 );
  WidgetSetPushButton_OnSetLabel( &_this->ShutdownButton, 0 );
  CoreRectView__OnSetBounds( &_this->TransitionButton, _Const0053 );
  WidgetSetPushButton_OnSetLabel( &_this->TransitionButton, 0 );
  CoreRectView__OnSetBounds( &_this->DashboardV2, _this->Super2.Bounds );
  CoreRectView__OnSetBounds( &_this->DashboardV1, _this->Super2.Bounds );
  CoreRectView__OnSetBounds( &_this->SpeedDisplay, _Const0054 );
  CoreQuadView__OnSetPoint4( &_this->SpeedWarpGroup, _Const0055 );
  CoreQuadView__OnSetPoint3( &_this->SpeedWarpGroup, _Const0056 );
  CoreQuadView__OnSetPoint2( &_this->SpeedWarpGroup, _Const0057 );
  CoreQuadView__OnSetPoint1( &_this->SpeedWarpGroup, _Const0058 );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->SpeedWarpGroup, EwGetRectCenter( 
  EwGetRectORect( _this->SpeedDisplay.Super2.Bounds )));
  CoreRectView__OnSetBounds( &_this->LedButton, _Const0059 );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton, 0 );
  WidgetSetToggleButton_OnSetIconOnFrame( &_this->LedButton, 6 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton, 7 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton1, _Const005A );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton1, 0 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton1, 0 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton1, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton2, _Const005B );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton2, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton2, 2 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton2, 2 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton2, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton3, _Const005C );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton3, 0 );
  WidgetSetToggleButton_OnSetIconOnFrame( &_this->LedButton3, 4 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton3, 5 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton3, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton4, _Const005D );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton4, 0 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton4, 1 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton4, 0 );
  CoreRectView__OnSetBounds( &_this->LedButton5, _Const005E );
  CoreGroup_OnSetEnabled((CoreGroup)&_this->LedButton5, 0 );
  WidgetSetToggleButton_OnSetIconOffFrame( &_this->LedButton5, 3 );
  WidgetSetToggleButton_OnSetIconFrame( &_this->LedButton5, 3 );
  WidgetSetToggleButton_OnSetLabel( &_this->LedButton5, 0 );
  CoreRectView__OnSetBounds( &_this->RangeDisplay, _Const005F );
  WidgetSetValueDisplay_OnSetUnit( &_this->RangeDisplay, EwLoadString( &_Const0060 ));
  WidgetSetValueDisplay_OnSetNoOfDigits( &_this->RangeDisplay, 1 );
  WidgetSetValueDisplay_OnSetCurrentFactor( &_this->RangeDisplay, 4.000000f );
  CoreTimer_OnSetPeriod( &_this->autoDemoModeTimer, 20000 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->UI ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Mode ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ShutdownButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TransitionButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DashboardV2 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DashboardV1 ), 0 );
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
  _this->shutdownVariantEffect.Outlet = EwNewRef( _this, DashboardDashboard_OnGetprogress, 
  DashboardDashboard_OnSetprogress );
  _this->gaugeDisplayEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onGaugeDisplayFinished );
  _this->gaugeDisplayEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onGaugeDisplayEffect );
  _this->shutdownGaugeDisplayEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onShutdownGaugeDisplayFinished );
  _this->shutdownGaugeDisplayEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onShutdownGaugeDisplayEffect );
  _this->gaugeCheckEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onGaugeCheckEffectFinished );
  _this->gaugeCheckEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onGaugeCheckEffect );
  _this->ledOffEffect.Super1.OnFinished = EwNewSlot( _this, DashboardDashboard_onLedOffFinished );
  _this->ledOffEffect.Super1.OnAnimate = EwNewSlot( _this, DashboardDashboard_onLedOffEffect );
  ViewsText_OnSetFont( &_this->UI, EwLoadResource( &DashboardFontLightS, ResourcesFont ));
  ViewsText_OnSetFont( &_this->Mode, EwLoadResource( &DashboardFontLightS, ResourcesFont ));
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
  DashboardGaugeDisplay_OnSetOutlet( &_this->SpeedDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed ));
  DashboardGaugeDisplay_OnSetOutletDesc( &_this->SpeedDisplay, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetODO, DashboardDeviceClass_OnSetODO ));
  ViewsWarpGroup_OnSetGroup( &_this->SpeedWarpGroup, ((CoreGroup)&_this->SpeedDisplay ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed0, DashboardDeviceClass_OnSetLed0 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton, EwGetAutoObject( &DashboardLedButtonOrangeConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton1, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed1, DashboardDeviceClass_OnSetLed1 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton1, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton1, EwGetAutoObject( &DashboardLedButtonOrangeConfig, 
  WidgetSetToggleButtonConfig ));
  WidgetSetToggleButton_OnSetOutlet( &_this->LedButton2, EwNewRef( EwGetAutoObject( 
  &DashboardDevice, DashboardDeviceClass ), DashboardDeviceClass_OnGetLed2, DashboardDeviceClass_OnSetLed2 ));
  WidgetSetToggleButton_OnSetIcon( &_this->LedButton2, EwLoadResource( &DashboardIcons, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetAppearance( &_this->LedButton2, EwGetAutoObject( &DashboardLedButtonOrangeConfig, 
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
  EffectsFloatEffect__ReInit( &_this->shutdownVariantEffect );
  EffectsFloatEffect__ReInit( &_this->gaugeDisplayEffect );
  EffectsFloatEffect__ReInit( &_this->shutdownGaugeDisplayEffect );
  EffectsInt32Effect__ReInit( &_this->gaugeCheckEffect );
  EffectsInt32Effect__ReInit( &_this->ledOffEffect );
  ViewsRectangle__ReInit( &_this->Background );
  ViewsText__ReInit( &_this->UI );
  ViewsText__ReInit( &_this->Mode );
  WidgetSetPushButton__ReInit( &_this->ShutdownButton );
  WidgetSetPushButton__ReInit( &_this->TransitionButton );
  DashboardDashboardV2__ReInit( &_this->DashboardV2 );
  DashboardDashboardV1__ReInit( &_this->DashboardV1 );
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
  EffectsFloatEffect__Done( &_this->shutdownVariantEffect );
  EffectsFloatEffect__Done( &_this->gaugeDisplayEffect );
  EffectsFloatEffect__Done( &_this->shutdownGaugeDisplayEffect );
  EffectsInt32Effect__Done( &_this->gaugeCheckEffect );
  EffectsInt32Effect__Done( &_this->ledOffEffect );
  ViewsRectangle__Done( &_this->Background );
  ViewsText__Done( &_this->UI );
  ViewsText__Done( &_this->Mode );
  WidgetSetPushButton__Done( &_this->ShutdownButton );
  WidgetSetPushButton__Done( &_this->TransitionButton );
  DashboardDashboardV2__Done( &_this->DashboardV2 );
  DashboardDashboardV1__Done( &_this->DashboardV1 );
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
    CoreGroup__OnSetOpacity( &_this->LedButton1, _this->LedButton2.Super1.Opacity );
    CoreGroup__OnSetOpacity( &_this->LedButton, _this->LedButton2.Super1.Opacity );
    CoreGroup__OnSetOpacity( &_this->LedButton5, _this->LedButton2.Super1.Opacity );
    CoreGroup__OnSetOpacity( &_this->LedButton4, _this->LedButton2.Super1.Opacity );
    CoreGroup__OnSetOpacity( &_this->LedButton3, _this->LedButton2.Super1.Opacity );
    CoreGroup__OnSetOpacity( &_this->RangeDisplay, _this->LedButton2.Super1.Opacity );
  }
  else
  {
    CoreRectView__OnSetBounds( &_this->LedButton2, EwSetRectOrigin( _this->LedButton2.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0061, _Const0062, 0.400000f, 0.600000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton1, EwSetRectOrigin( _this->LedButton1.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0063, _Const0064, 0.400000f, 0.600000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton, EwSetRectOrigin( _this->LedButton.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0065, _Const0066, 0.400000f, 0.600000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton5, EwSetRectOrigin( _this->LedButton5.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0067, _Const0068, 0.400000f, 0.600000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton4, EwSetRectOrigin( _this->LedButton4.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const0069, _Const006A, 0.400000f, 0.600000f, _this->progress )));
    CoreRectView__OnSetBounds( &_this->LedButton3, EwSetRectOrigin( _this->LedButton3.Super2.Bounds, 
    DashboardAnimationClass_linearPointProgess( EwGetAutoObject( &DashboardAnimation, 
    DashboardAnimationClass ), _Const006B, _Const006C, 0.400000f, 0.600000f, _this->progress )));

    if ( !CoreGroup_OnGetVisible((CoreGroup)&_this->DashboardV2 ))
      CoreGroup__OnSetOpacity( &_this->RangeDisplay, DashboardAnimationClass_linearValueProgress( 
      EwGetAutoObject( &DashboardAnimation, DashboardAnimationClass ), 0, 255, 0.200000f, 
      0.500000f, _this->progress ));
  }
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
  ViewsText_OnSetVisible( &_this->UI, 0 );
  ViewsText_OnSetVisible( &_this->Mode, 0 );
  CoreGroup__OnSetOpacity( &_this->SpeedDisplay, 0 );
  ViewsWarpGroup_OnSetOpacity( &_this->SpeedWarpGroup, 0 );
  CoreGroup__OnSetOpacity( &_this->RangeDisplay, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton1, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton2, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton3, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton4, 0 );
  CoreGroup__OnSetOpacity( &_this->LedButton5, 0 );
  CoreGroup__OnSetBuffered( &_this->SpeedDisplay, 1 );
  EwIdleSignal( EwNewSlot( &_this->gaugeDisplayEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  EwIdleSignal( EwNewSlot( &_this->v1Effect, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::Dashboard.onV1Effect()' */
void DashboardDashboard_onV1Effect( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV2, 1 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->TransitionButton, 1 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->ShutdownButton, 1 );
  ViewsText_OnSetVisible( &_this->UI, 1 );
  ViewsText_OnSetVisible( &_this->Mode, 1 );
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
  startOpacity = 5.000000f;
  scale = startOpacity / 255.000000f;

  if ( _this->gaugeDisplayEffect.Value >= startOpacity )
    scale = _this->gaugeDisplayEffect.Value / 255.000000f;

  ViewsWarpView__RotateAndScale( &_this->SpeedWarpGroup, EwGetRectCenter( _this->SpeedDisplay.Super2.Bounds ), 
  0.000000f, scale, scale );
}

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeDisplayFinished()' */
void DashboardDashboard_onGaugeDisplayFinished( DashboardDashboard _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup__OnSetOpacity( &_this->SpeedDisplay, 255 );
  CoreGroup__OnSetBuffered( &_this->SpeedDisplay, 0 );
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

/* 'C' function for method : 'Dashboard::Dashboard.onLedOffFinished()' */
void DashboardDashboard_onLedOffFinished( DashboardDashboard _this, XObject sender )
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

/* 'C' function for method : 'Dashboard::Dashboard.onShutdown()' */
void DashboardDashboard_onShutdown( DashboardDashboard _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_OnSetEnabled((CoreGroup)&_this->TransitionButton, 0 );
  _this->shutdown = 1;
  DashboardDeviceClass_OnSetDemoMode( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  _this->shutdownVariantEffect.Value1 = _this->progress;
  _this->shutdownVariantEffect.Value2 = 1.000000f - _this->transitionEffect.Value2;
  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV1, (XBool)( _this->progress 
  == 1.000000f ));
  CoreGroup_OnSetVisible((CoreGroup)&_this->DashboardV2, (XBool)!CoreGroup_OnGetVisible((CoreGroup)&_this->DashboardV1 ));
  CoreGroup_OnSetVisible((CoreGroup)&_this->TransitionButton, 0 );
  CoreGroup_OnSetVisible((CoreGroup)&_this->ShutdownButton, 0 );
  ViewsText_OnSetVisible( &_this->UI, 0 );
  ViewsText_OnSetVisible( &_this->Mode, 0 );
  CoreGroup__OnSetBuffered( &_this->SpeedDisplay, 1 );
  CoreGroup__OnSetOpacity( &_this->SpeedDisplay, 0 );
  EwSignal( EwNewSlot( &_this->shutdownVariantEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
  EwSignal( EwNewSlot( &_this->shutdownGaugeDisplayEffect, EffectsEffect_StartEffect ), 
    ((XObject)_this ));
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
  endOpacity = 5.000000f;
  scale = endOpacity / 255.000000f;

  if ( _this->shutdownGaugeDisplayEffect.Value >= endOpacity )
    scale = _this->shutdownGaugeDisplayEffect.Value / 255.000000f;

  ViewsWarpView__RotateAndScale( &_this->SpeedWarpGroup, EwGetRectCenter( _this->SpeedDisplay.Super2.Bounds ), 
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
  WidgetSetValueDisplayConfig_OnSetValueFont( _this, EwLoadResource( &DashboardFontLightS, 
  ResourcesFont ));
  WidgetSetValueDisplayConfig_OnSetUnitColorNegative( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetUnitColorPositive( _this, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->Text );
  WidgetSetValueDisplayConfig_OnSetUnitFont( _this, EwLoadResource( &DashboardFontLightS, 
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

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardGaugeLabels );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const006D );
  _this->increment = 10;
  _this->radius = 106;
  _this->CurrentValue = 10;
  _this->startAngle = 230.000000f;
  _this->endAngle = 490.000000f;

  /* Call the user defined constructor */
  DashboardGaugeLabels_Init( _this, aArg );
}

/* Re-Initializer for the class 'Dashboard::GaugeLabels' */
void DashboardGaugeLabels__ReInit( DashboardGaugeLabels _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Dashboard::GaugeLabels' */
void DashboardGaugeLabels__Done( DashboardGaugeLabels _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

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

  deltaAngle = ( _this->endAngle - _this->startAngle ) / 12.000000f;
  i = 0;

  while ( i < 13 )
  {
    DashboardGaugeLabels_adjustLabel( _this, i, _this->startAngle + ((XFloat)i * 
    deltaAngle ), _this->radius, ( i + 1 ) * _this->increment );
    i = i + 1;
  }
}

/* 'C' function for method : 'Dashboard::GaugeLabels.adjustLabel()' */
void DashboardGaugeLabels_adjustLabel( DashboardGaugeLabels _this, XInt32 aIndex, 
  XFloat aAngle, XInt32 aDistance, XInt32 aNumber )
{
  /* List of temporarily used variables */
  void* _tmpO0;
  XInt32 offset = 0;
  XPoint labelCenter = EwNewPoint( EwGetRectW( _this->labels[ EwCheckIndex( aIndex, 
    13 )]->Super1.Bounds ) / 2, EwGetRectH( _this->labels[ EwCheckIndex( aIndex, 
    13 )]->Super1.Bounds ) / 2 );

  ViewsText_OnSetString( _this->labels[ EwCheckIndex( aIndex, 13 )], EwNewStringInt( 
  aNumber, 0, 10 ));

  if ( aAngle > 360.000000f )
    aAngle = aAngle - 360.000000f;

  if (( aNumber / 100 ) >= 1 )
    offset = -2;

  ( _tmpO0 = _this->labels[ EwCheckIndex( aIndex, 13 )], CoreRectView__OnSetBounds( 
  _tmpO0, EwSetRectOrigin(((ViewsText)_tmpO0)->Super1.Bounds, EwMovePointPos( EwMovePointNeg( 
  EwGetRectCenter( EwGetRectORect( _this->Super2.Bounds )), labelCenter ), EwNewPoint((XInt32)((XFloat)( 
  aDistance + offset ) * EwMathCos( aAngle - 90.000000f )), (XInt32)((XFloat)( aDistance 
  + offset ) * EwMathSin( aAngle - 90.000000f )))))));
}

/* 'C' function for method : 'Dashboard::GaugeLabels.OnSetCurrentValue()' */
void DashboardGaugeLabels_OnSetCurrentValue( DashboardGaugeLabels _this, XInt32 
  value )
{
  XInt32 index;

  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  index = (XInt32)EwMathRound((XFloat)_this->CurrentValue / (XFloat)_this->increment ) 
  - 1;

  if (( index < 13 ) && ( index != _this->selected ))
  {
    XInt32 i;

    for ( i = 0; i < 13; i = i + 1 )
      ViewsText_OnSetColor( _this->labels[ EwCheckIndex( i, 13 )], EwGetAutoObject( 
      &DashboardColors, DashboardColorsClass )->Text );

    if ( index >= 0 )
      ViewsText_OnSetColor( _this->labels[ EwCheckIndex( index, 13 )], EwGetAutoObject( 
      &DashboardColors, DashboardColorsClass )->BatteryGaugeSelectionV1 );

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

/* 'C' function for method : 'Dashboard::GaugeLabels.setup()' */
void DashboardGaugeLabels_setup( DashboardGaugeLabels _this, XObject sender )
{
  XInt32 i;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  i = 0;

  while ( i < 13 )
  {
    ViewsText txt = EwNewObject( ViewsText, 0 );
    ViewsText_OnSetAutoSize( txt, 1 );
    _this->labels[ EwCheckIndex( i, 13 )] = txt;
    ViewsText_OnSetFont( txt, EwLoadResource( &DashboardFontLightM, ResourcesFont ));

    if ( _this->labels[ EwCheckIndex( i, 13 )]->Super2.Owner == 0 )
      CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->labels[ EwCheckIndex( i, 
      13 )]), 0 );

    i = i + 1;
  }
}

/* Variants derived from the class : 'Dashboard::GaugeLabels' */
EW_DEFINE_CLASS_VARIANTS( DashboardGaugeLabels )
EW_END_OF_CLASS_VARIANTS( DashboardGaugeLabels )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeLabels' */
EW_DEFINE_CLASS( DashboardGaugeLabels, CoreGroup, labels, Outlet, Outlet, increment, 
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
  CoreRectView__OnSetBounds( _this, _Const006E );
  _this->StartAngle = 205.000000f;
  _this->Scaling = 1.000000f;
  _this->MaxValue = 130;
  _this->CurrentValue = 1;
  _this->EndAngle = 490.000000f;
  CoreQuadView__OnSetPoint4( &_this->Needle, _Const006F );
  CoreQuadView__OnSetPoint3( &_this->Needle, _Const0070 );
  CoreQuadView__OnSetPoint2( &_this->Needle, _Const0071 );
  CoreQuadView__OnSetPoint1( &_this->Needle, _Const0072 );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->Needle, _Const0073 );
  ViewsWarpImage_OnSetColor( &_this->Needle, _Const0074 );
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

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardGaugeIndicator_Init( DashboardGaugeIndicator _this, XHandle aArg )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( aArg );

  DashboardGaugeIndicator_drawNeedle( _this );
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
    _Const0075, _Const0075, _Const0075, _Const0075, 0 );
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
  EffectsInt32Effect__Init( &_this->RecuperationEffect, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardDemoMode );

  /* ... and initialize objects, variables, properties, etc. */
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->DemoModeTimer, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->SpeedEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->PowerEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->BatteryEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->ODOEffect, 1 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->RecuperationEffect, 1 );
  _this->DemoModeTimer.Super1.OnFinished = EwNewSlot( _this, DashboardDemoMode_onDemoModeCycleFinished );
  _this->SpeedEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetSpeed, DashboardDeviceClass_OnSetSpeed );
  _this->PowerEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetEnergy, DashboardDeviceClass_OnSetEnergy );
  _this->BatteryEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetBattery, DashboardDeviceClass_OnSetBattery );
  _this->ODOEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  DashboardDeviceClass_OnGetODO, DashboardDeviceClass_OnSetODO );
  _this->RecuperationEffect.Outlet = EwNewRef( EwGetAutoObject( &DashboardDevice, 
  DashboardDeviceClass ), DashboardDeviceClass_OnGetRecuperation, DashboardDeviceClass_OnSetRecuperation );

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
  EffectsInt32Effect__ReInit( &_this->RecuperationEffect );
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
  EffectsInt32Effect__Done( &_this->RecuperationEffect );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
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
    100, 2000 );
    DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 0, 78, EffectsTimingExp_Out, 
    100, 1500 );
    DashboardDemoMode_StartEffect( _this, &_this->RecuperationEffect, 60, 0, EffectsTimingExp_Out, 
    0, 100 );
    DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 67, 65, EffectsTimingExp_Out, 
    100, 2200 );
    DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 567, 569, EffectsTimingExp_Out, 
    100, 2200 );
  }
  else
    if ( _this->DemoStep == 1 )
    {
      EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 2200 );
      DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 100, 77, EffectsTimingExp_Out, 
      0, 2200 );
      DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 78, 18, EffectsTimingExp_Out, 
      0, 1900 );
      DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 65, 65, EffectsTimingExp_Out, 
      0, 2200 );
      DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 569, 570, EffectsTimingExp_Out, 
      0, 2200 );
    }
    else
      if ( _this->DemoStep == 2 )
      {
        EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 1100 );
        DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 77, 37, EffectsTimingExp_Out, 
        0, 1100 );
        DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 18, 0, EffectsTimingExp_Out, 
        0, 400 );
        DashboardDemoMode_StartEffect( _this, &_this->RecuperationEffect, 0, 40, 
        EffectsTimingExp_Out, 400, 300 );
        DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 65, 65, EffectsTimingExp_Out, 
        0, 1100 );
        DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 270, 270, EffectsTimingExp_Out, 
        0, 1100 );
      }
      else
        if ( _this->DemoStep == 3 )
        {
          EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 
          2000 );
          DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 37, 55, EffectsTimingExp_Out, 
          0, 1700 );
          DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 0, 15, EffectsTimingExp_Out, 
          300, 1500 );
          DashboardDemoMode_StartEffect( _this, &_this->RecuperationEffect, 40, 
          0, EffectsTimingExp_Out, 0, 300 );
          DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 65, 64, EffectsTimingExp_Out, 
          0, 1700 );
          DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 270, 271, EffectsTimingExp_Out, 
          0, 1700 );
        }
        else
          if ( _this->DemoStep == 4 )
          {
            EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 
            2600 );
            DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 55, 100, 
            EffectsTimingExp_Out, 0, 2600 );
            DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 15, 67, EffectsTimingExp_Out, 
            0, 1800 );
            DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 64, 62, 
            EffectsTimingExp_Out, 0, 2600 );
            DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 271, 271, EffectsTimingExp_Out, 
            0, 2600 );
          }
          else
            if ( _this->DemoStep == 5 )
            {
              EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->DemoModeTimer, 
              4500 );
              DashboardDemoMode_StartEffect( _this, &_this->SpeedEffect, 100, 0, 
              EffectsTimingExp_Out, 0, 3500 );
              DashboardDemoMode_StartEffect( _this, &_this->PowerEffect, 67, 0, 
              EffectsTimingExp_Out, 0, 1300 );
              DashboardDemoMode_StartEffect( _this, &_this->RecuperationEffect, 
              0, 60, EffectsTimingExp_Out, 1300, 300 );
              DashboardDemoMode_StartEffect( _this, &_this->BatteryEffect, 62, 62, 
              EffectsTimingExp_Out, 0, 2600 );
              DashboardDemoMode_StartEffect( _this, &_this->ODOEffect, 271, 272, 
              EffectsTimingExp_Out, 0, 2600 );
            }

  _this->DemoStep = ( _this->DemoStep + 1 ) % 6;
  DashboardDeviceClass_OnSetODO( EwGetAutoObject( &DashboardDevice, DashboardDeviceClass ), 
  0 );
  EwPostSignal( EwNewSlot( &_this->DemoModeTimer, EffectsEffect_StartEffect ), ((XObject)_this ));
}

/* 'C' function for method : 'Dashboard::DemoMode.StartEffect()' */
void DashboardDemoMode_StartEffect( DashboardDemoMode _this, EffectsInt32Effect 
  aEffect, XInt32 aValue1, XInt32 aValue2, XEnum aTiming, XInt32 aInitialDelay, 
  XInt32 aCycleDuration )
{
  aEffect->Value1 = aValue1;
  aEffect->Value2 = aValue2;
  EffectsEffect_OnSetTiming((EffectsEffect)aEffect, aTiming );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)aEffect, aCycleDuration );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)aEffect, aInitialDelay );
  EwSignal( EwNewSlot( aEffect, EffectsEffect_StartEffect ), ((XObject)_this ));
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

/* Include a file containing the bitmap resource : 'Dashboard::LeftBottomGlowV1' */
#include "_DashboardLeftBottomGlowV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftBottomGlowV1' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftBottomGlowV1 )

/* Include a file containing the bitmap resource : 'Dashboard::RightBottomGlowV1' */
#include "_DashboardRightBottomGlowV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightBottomGlowV1' */
EW_RES_WITHOUT_VARIANTS( DashboardRightBottomGlowV1 )

/* Include a file containing the bitmap resource : 'Dashboard::LeftMiddleGlowV1' */
#include "_DashboardLeftMiddleGlowV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftMiddleGlowV1' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftMiddleGlowV1 )

/* Include a file containing the bitmap resource : 'Dashboard::RightMiddleGlowV1' */
#include "_DashboardRightMiddleGlowV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightMiddleGlowV1' */
EW_RES_WITHOUT_VARIANTS( DashboardRightMiddleGlowV1 )

/* Include a file containing the bitmap resource : 'Dashboard::TopGlowV1' */
#include "_DashboardTopGlowV1.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::TopGlowV1' */
EW_RES_WITHOUT_VARIANTS( DashboardTopGlowV1 )

/* Include a file containing the bitmap resource : 'Dashboard::CenterGlowV2' */
#include "_DashboardCenterGlowV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::CenterGlowV2' */
EW_RES_WITHOUT_VARIANTS( DashboardCenterGlowV2 )

/* Include a file containing the bitmap resource : 'Dashboard::KWIcon' */
#include "_DashboardKWIcon.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::KWIcon' */
EW_RES_WITHOUT_VARIANTS( DashboardKWIcon )

/* Include a file containing the bitmap resource : 'Dashboard::PWRIcon' */
#include "_DashboardPWRIcon.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::PWRIcon' */
EW_RES_WITHOUT_VARIANTS( DashboardPWRIcon )

/* Include a file containing the bitmap resource : 'Dashboard::LeftBricksV2' */
#include "_DashboardLeftBricksV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::LeftBricksV2' */
EW_RES_WITHOUT_VARIANTS( DashboardLeftBricksV2 )

/* Include a file containing the bitmap resource : 'Dashboard::RightBricksV2' */
#include "_DashboardRightBricksV2.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::RightBricksV2' */
EW_RES_WITHOUT_VARIANTS( DashboardRightBricksV2 )

/* Initializer for the class 'Dashboard::LeftTopBar' */
void DashboardLeftTopBar__Init( DashboardLeftTopBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  DashboardRightValueBar__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardLeftTopBar );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardLeftTopBar );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0076 );
  CoreRectView__OnSetBounds( &_this->Super1.BottomBricks, _Const0077 );
  ViewsImage_OnSetAlignment( &_this->Super1.BottomBricks, ViewsImageAlignmentAlignHorzLeft 
  | ViewsImageAlignmentAlignVertBottom );
  CoreRectView__OnSetBounds( &_this->Super1.TopBricks, _Const0078 );
  ViewsImage_OnSetAlignment( &_this->Super1.TopBricks, ViewsImageAlignmentAlignHorzRight 
  | ViewsImageAlignmentAlignVertTop );
  ViewsImage_OnSetBitmap( &_this->Super1.BottomBricks, EwLoadResource( &DashboardLeftBricksV2, 
  ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Super1.TopBricks, EwLoadResource( &DashboardLeftBricksV2, 
  ResourcesBitmap ));
}

/* Re-Initializer for the class 'Dashboard::LeftTopBar' */
void DashboardLeftTopBar__ReInit( DashboardLeftTopBar _this )
{
  /* At first re-initialize the super class ... */
  DashboardRightValueBar__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Dashboard::LeftTopBar' */
void DashboardLeftTopBar__Done( DashboardLeftTopBar _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( DashboardRightValueBar );

  /* Don't forget to deinitialize the super class ... */
  DashboardRightValueBar__Done( &_this->_Super );
}

/* Variants derived from the class : 'Dashboard::LeftTopBar' */
EW_DEFINE_CLASS_VARIANTS( DashboardLeftTopBar )
EW_END_OF_CLASS_VARIANTS( DashboardLeftTopBar )

/* Virtual Method Table (VMT) for the class : 'Dashboard::LeftTopBar' */
EW_DEFINE_CLASS( DashboardLeftTopBar, DashboardRightValueBar, _None, _None, _None, 
                 _None, _None, _None, "Dashboard::LeftTopBar" )
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
  DashboardRightValueBar_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardLeftTopBar )

/* Initializer for the class 'Dashboard::LeftBottomBar' */
void DashboardLeftBottomBar__Init( DashboardLeftBottomBar _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  DashboardLeftTopBar__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( DashboardLeftBottomBar );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( DashboardLeftBottomBar );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0079 );
  CoreRectView__OnSetBounds( &_this->Super2.BottomBricks, _Const007A );
  ViewsImage_OnSetColor( &_this->Super2.BottomBricks, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->VertBarUnselectedV2 );
  CoreRectView__OnSetBounds( &_this->Super2.TopBricks, _Const007B );
  ViewsImage_OnSetColor( &_this->Super2.TopBricks, EwGetAutoObject( &DashboardColors, 
  DashboardColorsClass )->VertBarRecoveryV2 );
}

/* Re-Initializer for the class 'Dashboard::LeftBottomBar' */
void DashboardLeftBottomBar__ReInit( DashboardLeftBottomBar _this )
{
  /* At first re-initialize the super class ... */
  DashboardLeftTopBar__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Dashboard::LeftBottomBar' */
void DashboardLeftBottomBar__Done( DashboardLeftBottomBar _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( DashboardLeftTopBar );

  /* Don't forget to deinitialize the super class ... */
  DashboardLeftTopBar__Done( &_this->_Super );
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
void DashboardLeftBottomBar_UpdateViewState( DashboardLeftBottomBar _this, XSet 
  aState )
{
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 newPos;
  XInt32 brickHeightWithSpace;

  DashboardRightValueBar_UpdateViewState((DashboardRightValueBar)_this, aState );
  minPos = 0;
  maxPos = EwGetRectH( _this->Super5.Bounds );
  newPos = minPos;
  brickHeightWithSpace = 9;

  if ( _this->Super2.MaxValue != _this->Super2.MinValue )
    newPos = ((( DashboardRightValueBar_OnGetCurrentValue((DashboardRightValueBar)_this ) 
    - _this->Super2.MinValue ) * ( maxPos - minPos )) / ( _this->Super2.MaxValue 
    - _this->Super2.MinValue )) + minPos;

  CoreRectView__OnSetBounds( &_this->Super2.TopBricks, EwSetRectY2( _this->Super2.TopBricks.Super1.Bounds, 
  newPos - ( newPos % brickHeightWithSpace )));
  CoreRectView__OnSetBounds( &_this->Super2.BottomBricks, EwSetRectY1( _this->Super2.BottomBricks.Super1.Bounds, 
  _this->Super2.TopBricks.Super1.Bounds.Point2.Y ));
}

/* Variants derived from the class : 'Dashboard::LeftBottomBar' */
EW_DEFINE_CLASS_VARIANTS( DashboardLeftBottomBar )
EW_END_OF_CLASS_VARIANTS( DashboardLeftBottomBar )

/* Virtual Method Table (VMT) for the class : 'Dashboard::LeftBottomBar' */
EW_DEFINE_CLASS( DashboardLeftBottomBar, DashboardLeftTopBar, _None, _None, _None, 
                 _None, _None, _None, "Dashboard::LeftBottomBar" )
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
  DashboardLeftBottomBar_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( DashboardLeftBottomBar )

/* Include a file containing the bitmap resource : 'Dashboard::Logo' */
#include "_DashboardLogo.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::Logo' */
EW_RES_WITHOUT_VARIANTS( DashboardLogo )

/* Include a file containing the bitmap resource : 'Dashboard::HorizontalLine' */
#include "_DashboardHorizontalLine.h"

/* Table with links to derived variants of the bitmap resource : 'Dashboard::HorizontalLine' */
EW_RES_WITHOUT_VARIANTS( DashboardHorizontalLine )

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

/* Include a file containing the font resource : 'Dashboard::FontLightM' */
#include "_DashboardFontLightM.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontLightM' */
EW_RES_WITHOUT_VARIANTS( DashboardFontLightM )

/* Include a file containing the font resource : 'Dashboard::FontS' */
#include "_DashboardFontS.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontS' */
EW_RES_WITHOUT_VARIANTS( DashboardFontS )

/* Include a file containing the font resource : 'Dashboard::FontLightS' */
#include "_DashboardFontLightS.h"

/* Table with links to derived variants of the font resource : 'Dashboard::FontLightS' */
EW_RES_WITHOUT_VARIANTS( DashboardFontLightS )

/* Embedded Wizard */
