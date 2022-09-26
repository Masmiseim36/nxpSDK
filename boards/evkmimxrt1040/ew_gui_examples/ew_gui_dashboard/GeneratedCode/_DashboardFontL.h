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

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DEFINE_FONT_RES( DashboardFontL, 9, 2, 0, 16, 0x0001, 0x0060 )
  EW_GLYPH( 0x0001, 0, -7, 4, 7, 4, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 2, 0x00000061 ),                /* ' ' */
  EW_GLYPH( 0x0021, 0, -7, 2, 7, 2, 0x00000061 ),               /* '!' */
  EW_GLYPH( 0x0022, 0, -7, 3, 2, 3, 0x00000092 ),               /* '"' */
  EW_GLYPH( 0x0023, 0, -7, 5, 7, 5, 0x000000AD ),               /* '#' */
  EW_GLYPH( 0x0024, 0, -8, 5, 9, 5, 0x00000158 ),               /* '$' */
  EW_GLYPH( 0x0025, 0, -7, 7, 7, 6, 0x00000228 ),               /* '%' */
  EW_GLYPH( 0x0026, 0, -7, 6, 7, 6, 0x000002F1 ),               /* '&' */
  EW_GLYPH( 0x0027, 0, -7, 1, 3, 1, 0x000003A5 ),               /* ''' */
  EW_GLYPH( 0x0028, 0, -7, 3, 9, 3, 0x000003B5 ),               /* '(' */
  EW_GLYPH( 0x0029, 0, -7, 3, 9, 3, 0x00000410 ),               /* ')' */
  EW_GLYPH( 0x002A, 0, -7, 4, 5, 4, 0x00000479 ),               /* '*' */
  EW_GLYPH( 0x002B, 0, -6, 5, 5, 5, 0x000004D5 ),               /* '+' */
  EW_GLYPH( 0x002C, 0, -1, 2, 3, 2, 0x00000523 ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -3, 3, 1, 3, 0x00000542 ),               /* '-' */
  EW_GLYPH( 0x002E, 0, -1, 3, 1, 3, 0x00000551 ),               /* '.' */
  EW_GLYPH( 0x002F, -1, -7, 5, 8, 3, 0x0000055B ),              /* '/' */
  EW_GLYPH( 0x0030, 0, -7, 5, 7, 5, 0x000005C4 ),               /* '0' */
  EW_GLYPH( 0x0031, 0, -7, 4, 7, 5, 0x00000637 ),               /* '1' */
  EW_GLYPH( 0x0032, 0, -7, 5, 7, 5, 0x00000679 ),               /* '2' */
  EW_GLYPH( 0x0033, 0, -7, 5, 7, 5, 0x0000071A ),               /* '3' */
  EW_GLYPH( 0x0034, 0, -7, 5, 7, 5, 0x000007B4 ),               /* '4' */
  EW_GLYPH( 0x0035, 0, -7, 5, 7, 5, 0x00000824 ),               /* '5' */
  EW_GLYPH( 0x0036, 0, -7, 5, 7, 5, 0x000008C1 ),               /* '6' */
  EW_GLYPH( 0x0037, 0, -7, 5, 7, 5, 0x00000957 ),               /* '7' */
  EW_GLYPH( 0x0038, 0, -7, 5, 7, 5, 0x000009DB ),               /* '8' */
  EW_GLYPH( 0x0039, 0, -7, 5, 7, 5, 0x00000A6A ),               /* '9' */
  EW_GLYPH( 0x003A, 0, -5, 3, 5, 2, 0x00000AFA ),               /* ':' */
  EW_GLYPH( 0x003B, 0, -5, 3, 7, 2, 0x00000B1E ),               /* ';' */
  EW_GLYPH( 0x003C, 0, -5, 4, 4, 4, 0x00000B5A ),               /* '<' */
  EW_GLYPH( 0x003D, 0, -4, 5, 3, 5, 0x00000B9C ),               /* '=' */
  EW_GLYPH( 0x003E, 0, -5, 5, 4, 4, 0x00000BEA ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -7, 4, 7, 4, 0x00000C33 ),               /* '?' */
  EW_GLYPH( 0x0040, 0, -6, 8, 8, 8, 0x00000CAB ),               /* '@' */
  EW_GLYPH( 0x0041, 0, -7, 6, 7, 6, 0x00000DD7 ),               /* 'A' */
  EW_GLYPH( 0x0042, 0, -7, 6, 7, 6, 0x00000E82 ),               /* 'B' */
  EW_GLYPH( 0x0043, 0, -7, 6, 7, 6, 0x00000F10 ),               /* 'C' */
  EW_GLYPH( 0x0044, 0, -7, 6, 7, 6, 0x00000FBC ),               /* 'D' */
  EW_GLYPH( 0x0045, 0, -7, 5, 7, 5, 0x00001041 ),               /* 'E' */
  EW_GLYPH( 0x0046, 0, -7, 5, 7, 5, 0x000010BD ),               /* 'F' */
  EW_GLYPH( 0x0047, 0, -7, 6, 7, 6, 0x00001128 ),               /* 'G' */
  EW_GLYPH( 0x0048, 0, -7, 6, 7, 6, 0x000011DA ),               /* 'H' */
  EW_GLYPH( 0x0049, 0, -7, 2, 7, 3, 0x00001235 ),               /* 'I' */
  EW_GLYPH( 0x004A, 0, -7, 5, 7, 5, 0x0000124C ),               /* 'J' */
  EW_GLYPH( 0x004B, 0, -7, 6, 7, 5, 0x000012A1 ),               /* 'K' */
  EW_GLYPH( 0x004C, 0, -7, 5, 7, 5, 0x00001324 ),               /* 'L' */
  EW_GLYPH( 0x004D, 0, -7, 8, 7, 8, 0x00001363 ),               /* 'M' */
  EW_GLYPH( 0x004E, 0, -7, 6, 7, 6, 0x0000141C ),               /* 'N' */
  EW_GLYPH( 0x004F, 0, -7, 6, 7, 6, 0x00001497 ),               /* 'O' */
  EW_GLYPH( 0x0050, 0, -7, 6, 7, 6, 0x00001548 ),               /* 'P' */
  EW_GLYPH( 0x0051, 0, -7, 6, 8, 6, 0x000015BB ),               /* 'Q' */
  EW_GLYPH( 0x0052, 0, -7, 6, 7, 6, 0x00001689 ),               /* 'R' */
  EW_GLYPH( 0x0053, 0, -7, 6, 7, 5, 0x00001709 ),               /* 'S' */
  EW_GLYPH( 0x0054, 0, -7, 6, 7, 5, 0x000017BF ),               /* 'T' */
  EW_GLYPH( 0x0055, 0, -7, 6, 7, 6, 0x0000181F ),               /* 'U' */
  EW_GLYPH( 0x0056, 0, -7, 6, 7, 6, 0x00001889 ),               /* 'V' */
  EW_GLYPH( 0x0057, 0, -7, 8, 7, 8, 0x00001934 ),               /* 'W' */
  EW_GLYPH( 0x0058, 0, -7, 6, 7, 5, 0x00001A29 ),               /* 'X' */
  EW_GLYPH( 0x0059, 0, -7, 6, 7, 5, 0x00001AD8 ),               /* 'Y' */
  EW_GLYPH( 0x005A, 0, -7, 6, 7, 5, 0x00001B5C ),               /* 'Z' */
  EW_GLYPH( 0x005B, 0, -7, 3, 8, 2, 0x00001BFA ),               /* '[' */
  EW_GLYPH( 0x005C, 0, -7, 4, 8, 4, 0x00001C2F ),               /* '\' */
  EW_GLYPH( 0x005D, 0, -7, 2, 8, 2, 0x00001CAB ),               /* ']' */
  EW_GLYPH( 0x005E, 0, -7, 4, 3, 4, 0x00001CCD ),               /* '^' */
  EW_GLYPH( 0x005F, 0, 0, 4, 1, 4, 0x00001D05 ),                /* '_' */
  EW_GLYPH( 0x0060, 0, -8, 3, 2, 3, 0x00001D19 ),               /* '`' */
  EW_GLYPH( 0x0061, 0, -5, 5, 5, 5, 0x00001D2F ),               /* 'a' */
  EW_GLYPH( 0x0062, 0, -7, 5, 7, 5, 0x00001D9F ),               /* 'b' */
  EW_GLYPH( 0x0063, 0, -5, 5, 5, 4, 0x00001E14 ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -7, 5, 7, 5, 0x00001E84 ),               /* 'd' */
  EW_GLYPH( 0x0065, 0, -5, 5, 5, 5, 0x00001EFB ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -7, 4, 7, 3, 0x00001F79 ),               /* 'f' */
  EW_GLYPH( 0x0067, 0, -5, 5, 7, 5, 0x00001FC4 ),               /* 'g' */
  EW_GLYPH( 0x0068, 0, -7, 5, 7, 5, 0x00002050 ),               /* 'h' */
  EW_GLYPH( 0x0069, 0, -7, 2, 7, 2, 0x000020A7 ),               /* 'i' */
  EW_GLYPH( 0x006A, -1, -7, 3, 9, 2, 0x000020CB ),              /* 'j' */
  EW_GLYPH( 0x006B, 0, -7, 5, 7, 5, 0x00002118 ),               /* 'k' */
  EW_GLYPH( 0x006C, 0, -7, 2, 7, 2, 0x00002189 ),               /* 'l' */
  EW_GLYPH( 0x006D, 0, -5, 8, 5, 7, 0x000021A1 ),               /* 'm' */
  EW_GLYPH( 0x006E, 0, -5, 5, 5, 5, 0x00002205 ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -5, 5, 5, 5, 0x0000224B ),               /* 'o' */
  EW_GLYPH( 0x0070, 0, -5, 5, 7, 5, 0x000022C0 ),               /* 'p' */
  EW_GLYPH( 0x0071, 0, -5, 5, 7, 5, 0x0000233C ),               /* 'q' */
  EW_GLYPH( 0x0072, 0, -5, 3, 5, 3, 0x000023BE ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -5, 5, 5, 4, 0x000023E7 ),               /* 's' */
  EW_GLYPH( 0x0074, 0, -6, 3, 6, 3, 0x00002469 ),               /* 't' */
  EW_GLYPH( 0x0075, 0, -5, 5, 5, 5, 0x000024A1 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -5, 5, 5, 4, 0x000024E6 ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -5, 7, 5, 6, 0x00002559 ),               /* 'w' */
  EW_GLYPH( 0x0078, 0, -5, 5, 5, 4, 0x000025F1 ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -5, 5, 7, 4, 0x00002664 ),               /* 'y' */
  EW_GLYPH( 0x007A, 0, -5, 5, 5, 4, 0x000026FB ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -7, 3, 9, 3, 0x00002771 ),               /* '{' */
  EW_GLYPH( 0x007C, 0, -7, 2, 8, 2, 0x000027D4 ),               /* '|' */
  EW_GLYPH( 0x007D, 0, -7, 3, 9, 3, 0x000027EC ),               /* '}' */
  EW_GLYPH( 0x007E, 0, -4, 6, 2, 6, 0x0000284F ),               /* '~' */

  EW_FONT_PIXEL( DashboardFontL, 0x0000288C )                   /* ratio 90.85 % */
    0xE7DAEF7D, 0xC0031AD4, 0xA73EC31A, 0x8DC217DE, 0xEBBEFDD5, 0xABBE4795, 0xFAE673CC,
    0xF9AFFCF5, 0xFF9FFBAF, 0xD7EBFDFA, 0xFC4C88BF, 0x731DABF0, 0x118EF5DE, 0xFBCCF7EF,
    0xD6B77E65, 0xAFBAC6F7, 0xADF8BEBB, 0x27FFF19D, 0x0E6AF9EA, 0x7FCF7CF5, 0xD7FFFF88,
    0x419EE390, 0xCC9767FF, 0xF3ACBB35, 0x21AF7A23, 0x398317F2, 0xE6B58D4F, 0x37AEF3BD,
    0x18DCFA9E, 0xBE4B1FF3, 0x95186FDE, 0xC34A8E1C, 0xFA6F7EF0, 0x2C86B18E, 0x8E4738E7,
    0xD43BEAB1, 0x7EFFFBD2, 0x6A78BF35, 0xFC4BB78D, 0xF71DF8A0, 0xA5F71DFD, 0xF93DEF58,
    0xC0F37BAC, 0xB505720E, 0x41CC2FF1, 0xF51BFE15, 0xA67F77E0, 0xCF4DE6BB, 0xE7A6E005,
    0xF54D774E, 0xF19771AD, 0xE200009B, 0x71D17F77, 0x88FDFCD7, 0xE8CB198C, 0xE1EFF913,
    0xE5FDEF17, 0x9EF8BFBE, 0x84BF66BB, 0xDCF4C6E7, 0xFB97BEB9, 0xBCCBD73E, 0x8B7E3443,
    0xF8EE3F9A, 0x1C7BBF3B, 0x15DEF790, 0xE39AEF79, 0x59DFA8EC, 0xFE6392AF, 0x7AF358E5,
    0xA793FD8D, 0x3FEF1E7F, 0x9FCFFCCF, 0x3D96F3B4, 0xDFDE3AE6, 0xBBF7F7BD, 0x4CF936B7,
    0x61E60ABE, 0x88FBE28E, 0x5CC5FDDF, 0xCC40CD77, 0xBCD731B8, 0xCFDC5D73, 0x7EEFC5F5,
    0xB1078DDA, 0xF19679BF, 0xF998713F, 0xE65F7DF8, 0x47900791, 0xEB00791E, 0x307C8F75,
    0x2FBFEB7F, 0xFBDFFAC3, 0x73D77BDC, 0xBDCF5DEF, 0x3077AD77, 0xCC03DDCF, 0xE798F773,
    0xF7EBFD3C, 0xD9A9673C, 0x8B7C3BCE, 0xF73DE16F, 0xBDCEFC57, 0xFCE4F9DD, 0xFFF7F54E,
    0x44F3FBBF, 0x3A8631AC, 0xF77FF9E7, 0xE5F9F33B, 0x3C8BFDE9, 0x7E079CF7, 0xAC99511B,
    0xFFFF87AA, 0xDFBD44FE, 0x3EF759C8, 0xFFF06FCF, 0x8BCF7B8D, 0x5861677B, 0xDC18FFDC,
    0xE0C20F9B, 0x39D69ADE, 0x3EFBBC6D, 0xDC64F7F3, 0x3206640A, 0xAB7193F7, 0xF99F7DDE,
    0x7907BBDC, 0x2A607FBF, 0x31230918, 0x3FDFBC95, 0x5FEF7B9E, 0xF207FBDE, 0xFBDE47DE,
    0xFFEF7900, 0xE4FEF7B9, 0xEF203FBD, 0x0FBDE47D, 0xAF39D600, 0x67ADF77A, 0x0577193F,
    0x6E0BFFF7, 0xD4B0CFD9, 0x3DFACE7B, 0x000FD67F, 0xDE4DBDE4, 0xB9E000DB, 0x02FF8000,
    0xC56F8000, 0xABBBE718, 0xE16795C6, 0x0F8E8B3C, 0x27330233, 0x1AC0EDFA, 0x0001FCFD,
    0xFBDE4000, 0xEFA2EE7B, 0x78279B10, 0x9831189E, 0xB8957EB5, 0xCF07E7FF, 0xF07A8063,
    0xB92DF9FE, 0x3D40DF20, 0x2273BC2F, 0xEB074257, 0xBDDEA59C, 0xB32B3ADF, 0x319181FE,
    0xB32B7918, 0xF7BBCFFE, 0xE7BDCF5B, 0x826B3DC3, 0xB9359EE0, 0xB0000BE7, 0x779E59CE,
    0xCAEEB7EF, 0x64607FAC, 0xCADE460C, 0xEEF3FFAC, 0xCE75967F, 0xE7BDC67A, 0x80F5BDC3,
    0xDC559EE0, 0xA8159C07, 0x5A73AD31, 0x62CEFFF6, 0xFAFCADC6, 0xBFFCC4BF, 0x4B3FCF5D,
    0xCD4FBBD6, 0x3EF7BDFF, 0x7DDF9BFF, 0x80000000, 0x00BFE7F5, 0xC8C60000, 0xDEE6A31C,
    0xDF1F7EB3, 0x7FD45FEF, 0xA2B118C5, 0x7BD846AA, 0x1EAA78FF, 0xBDF48C72, 0xFF7F4FDE,
    0x5FE32CF4, 0x6788E44F, 0xB3CF588C, 0xBDBE31E2, 0x8C730CBF, 0x53DB1631, 0x6F2FFAFE,
    0x557663BD, 0x3334FDBC, 0x31CDE0C6, 0xEAE7ECC4, 0xBF3C7A7C, 0x15BA9C4F, 0xFDF2AB33,
    0xE1E9DE5E, 0xF0001F19, 0xBC6F7BDE, 0xE60CD6F7, 0x41BF44D3, 0x8BCC62E7, 0xAC77BECF,
    0x0007A8FB, 0xF8BD9EA0, 0xCA4678FD, 0xF1FBF0F1, 0xE1E3948C, 0x03F7BBF7, 0xC9F7C7C0,
    0xBEFBF339, 0xD77BDEEF, 0xF7DC8CB6, 0xA9F77EB3, 0xAEF15DF9, 0x8E7FA9CB, 0xCF3003F5,
    0x3F59C53F, 0x3B2D3FCB, 0xBBE7F339, 0xCFBAE73B, 0x72BBD313, 0x5BACF9EA, 0xF902DF85,
    0x9D6FD33F, 0xF8CB7F90, 0xF079BE5B, 0x75CC5F9D, 0x9F79BDE2, 0xBFFEF371, 0xFC7DCEB9,
    0xB1BA8C6E, 0x007F6DFD, 0xDFFCFB90, 0x6E533ADF, 0x9D6FC6E4, 0xB271F469, 0x0FEF5ACF,
    0xD3FFFCC0, 0x0583C7FC, 0xF7EEEE80, 0xEB5EB005, 0xC000FEF5, 0xEBFEB7C8, 0x8F38C007,
    0xC8C7DE75, 0xB597E8C1, 0x001FD7A2, 0x7D3EFF9E, 0x71AEC1FE, 0x00024614, 0x3FFFED60,
    0x5963C7F9, 0xFDDF8800, 0x3BD71BC5, 0x7F18F6FE, 0xBD71BC7B, 0x727F9F6B, 0xD3B2CFE6,
    0xF59C53B2, 0x904ECF33, 0x6FD7B3FF, 0xC96FF21D, 0x4EB7E5BF, 0xC099FFC8, 0x18E23BFB,
    0x5FDDF880, 0xA9E3BCD4, 0xFE79F275, 0xDF67CD67, 0xFDB5C739, 0xB9901FB6, 0x02DF9FDF,
    0xF33CB0E0, 0xFAFCFF4F, 0x1BF3C4F7, 0x33C31DE3, 0xFA7C5F8B, 0xDADF1B8D, 0x9E39A94F,
    0x151DE771, 0xF506BAA3, 0x97FA53C4, 0x9DCFBC77, 0x627E5623, 0xA9D7578D, 0xFAFCFEF1,
    0x8DF9E27B, 0x3D431BF1, 0x2B1F9F0F, 0x78FDF187, 0xBDE3BDEF, 0xFE730CF5, 0x17E1BF64,
    0xEBD43BBF, 0xDF0664FD, 0x8113DF13, 0x1637EF4C, 0xEF96F000, 0xE7888995, 0x3223E78B,
    0xEFFCAF7D, 0xF77EAB7C, 0x0000053E

  EW_FONT_KERNING_CODES( DashboardFontL )
  EW_FONT_KERNING_VALUES( DashboardFontL )
EW_END_OF_FONT_RES( DashboardFontL )

/* Embedded Wizard */
