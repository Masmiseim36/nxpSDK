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
* Version  : 9.30
* Date     : 14.02.2020  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

/* This is a font resource. */
EW_DEFINE_FONT_RES( CutterSmallDigitFont, 25, 6, 0, 16, 0x0001, 0x000E )
  EW_GLYPH( 0x0001, 1, -18, 10, 18, 12, 0x00000000 ),
  EW_GLYPH( 0x002C, 0, -3, 5, 8, 6, 0x000001F6 ),               /* ',' */
  EW_GLYPH( 0x002E, 1, -3, 5, 3, 7, 0x00000273 ),               /* '.' */
  EW_GLYPH( 0x0030, 1, -18, 13, 18, 15, 0x000002B0 ),           /* '0' */
  EW_GLYPH( 0x0031, 2, -18, 8, 18, 15, 0x00000504 ),            /* '1' */
  EW_GLYPH( 0x0032, 1, -18, 13, 18, 15, 0x00000600 ),           /* '2' */
  EW_GLYPH( 0x0033, 1, -18, 13, 18, 15, 0x00000892 ),           /* '3' */
  EW_GLYPH( 0x0034, 0, -18, 15, 18, 15, 0x00000B58 ),           /* '4' */
  EW_GLYPH( 0x0035, 1, -18, 13, 18, 15, 0x00000D61 ),           /* '5' */
  EW_GLYPH( 0x0036, 1, -18, 13, 18, 15, 0x00000FE3 ),           /* '6' */
  EW_GLYPH( 0x0037, 0, -18, 14, 18, 15, 0x0000126D ),           /* '7' */
  EW_GLYPH( 0x0038, 1, -18, 13, 18, 15, 0x00001479 ),           /* '8' */
  EW_GLYPH( 0x0039, 1, -18, 13, 18, 15, 0x00001736 ),           /* '9' */
  EW_GLYPH( 0x003A, 1, -14, 5, 14, 7, 0x000019D9 ),             /* ':' */

  EW_FONT_PIXEL( CutterSmallDigitFont, 0x00001A8B )             /* ratio 65.31 % */
    0xEF7BDEEB, 0xEE7977BD, 0xE2DEF7BD, 0xD4FF303E, 0x1760FB86, 0xC39E1FCF, 0xDACFF02D,
    0x099DFCC0, 0x10000484, 0x677F3012, 0x836B2702, 0xD82DCFE7, 0x6D47F3C5, 0x81F70FB8,
    0xF7B9E7F9, 0x718B7BDE, 0x2C200BEF, 0xB158BF8E, 0xB54F18CC, 0x7D88FDFC, 0x593C353F,
    0x7E7893C3, 0x03DF9EE7, 0xD442AFBE, 0x803D61F7, 0xFCF1F267, 0x19F8BFEF, 0xBF94BF29,
    0x8161E390, 0x20FE09E2, 0x11700B89, 0x02000000, 0x5C172E00, 0x78920904, 0x233C5028,
    0x2FE52FC5, 0xFEA3F0E4, 0x98FC5FF7, 0xDE273013, 0xF7D442AF, 0x61FBC602, 0xFB233DEE,
    0x057703BC, 0xD9899E20, 0xADF8DBF7, 0x00000000, 0x00000000, 0x00000000, 0xE7B9DFA8,
    0xABFBC137, 0xF0FEF510, 0x72BD404E, 0x62EEE7D2, 0x8C2B98F9, 0x1FC1C88F, 0xEFBF7122,
    0xEB321881, 0xF0DE0279, 0x2E4B3C01, 0x05C4FD40, 0x4FF117D4, 0xFF917C60, 0xE47F8C01,
    0xC7F8C01D, 0x89E409D8, 0x379007FA, 0xDEF7B9D5, 0x024FFF7B, 0xF9E00000, 0x1EFFFB9D,
    0xE42AFBF0, 0x15F0FDF3, 0xE878DAA0, 0xD30FFF7B, 0x8FC8B98F, 0x07E73388, 0x3BEF9888,
    0x704C7E30, 0x69E5DDEF, 0x4003EA00, 0x7BDC13FD, 0xC00F23F7, 0xEF18D2F8, 0x326097EF,
    0x9301EAB3, 0xF24EF3F8, 0x21CCF14B, 0x0FFF7BDE, 0xE007FC33, 0x85776269, 0x001F7E7C,
    0x007FBDDF, 0x2F0001EA, 0x8004E800, 0x3A001531, 0x966005C6, 0x9B4400BB, 0x445F8003,
    0x0E8F5003, 0x01355E00, 0x60068CE8, 0xBDEFB14C, 0x5637BC9B, 0x00058000, 0xBDEF7B8C,
    0xDEF26EF7, 0x00000000, 0xEF7BDDF8, 0xC1DEF7BD, 0x80048002, 0xF7BDEE30, 0x13243BDE,
    0x26009040, 0x7FFB9DF1, 0x427CC913, 0x00383DCE, 0xBD56F871, 0xA7269EE7, 0x6219BFD1,
    0xCD80362D, 0xC8024634, 0xCCD82FBD, 0xD60DF94C, 0x7D9E6B64, 0x9E9C876F, 0xDAC5E406,
    0x7FE8C533, 0x73BFD902, 0xABFF7803, 0x0027B002, 0xCEFE60DF, 0xF623D40D, 0x3D0E015B,
    0xFBD44A00, 0x464BE7BF, 0x78A67B18, 0x80649976, 0x7BED40F8, 0xBF18989A, 0x6364D621,
    0x00333F81, 0xD80D42A0, 0xAC19C54C, 0xB90BF5C5, 0x23C66DEF, 0x3F8C01FE, 0x8C435DC6,
    0xBDEE21DE, 0xEF7BDEF7, 0x0007F7BD, 0xF71D8000, 0xBDEF7BDE, 0xA0058877, 0x8B400986,
    0x2B0D4017, 0x007E5C00, 0x40127946, 0xA51802F3, 0x02E68006, 0x80146518, 0x4A3005E6,
    0x7E5E003E, 0x1311F200, 0x3002F340, 0xDF9EE77F, 0x42B7BF04, 0x5BC3FBD4, 0x3C597A80,
    0x25879EE7, 0x47C42F82, 0x3C440004, 0x1E3E217C, 0xEE73C37E, 0x711F8C79, 0x183FE412,
    0xE4EE4137, 0x99EE79A4, 0xBD527AF2, 0x4F92DE21, 0xB126C1B2, 0xEA831FC1, 0xD461EB10,
    0x9A7B9D60, 0x8C00CE51, 0x42B7EE43, 0xE613BBD4, 0x077FDCEF, 0x7215BDF8, 0x80AF81DD,
    0xB0F187F8, 0x5A29DDCE, 0x871874F1, 0x3185CFE5, 0x58B000C4, 0xF88B05C9, 0x7E30ACF3,
    0xCFA0CF03, 0x037085DD, 0x3B8D3244, 0x443BCE53, 0xFEFFEF12, 0x003C27DF, 0xB18072F1,
    0xEE07AA6E, 0x03F21DDD, 0x310076A0, 0x6205FED6, 0x4F0D4FDF, 0xEC64F0D6, 0x000001FB,
    0xFBEC4000, 0x1AC9E1A9, 0x0000049E

  EW_FONT_KERNING_CODES( CutterSmallDigitFont )
  EW_FONT_KERNING_VALUES( CutterSmallDigitFont )
EW_END_OF_FONT_RES( CutterSmallDigitFont )

/* Embedded Wizard */
