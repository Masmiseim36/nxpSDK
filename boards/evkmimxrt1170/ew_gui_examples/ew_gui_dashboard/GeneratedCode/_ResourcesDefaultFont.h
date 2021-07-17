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

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DEFINE_FONT_RES( ResourcesDefaultFont, 9, 3, 0, 2, 0x0001, 0x0060 )
  EW_GLYPH( 0x0001, 1, -6, 5, 6, 7, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 3, 0x0000001C ),                /* ' ' */
  EW_GLYPH( 0x0021, 1, -7, 1, 7, 3, 0x0000001C ),               /* '!' */
  EW_GLYPH( 0x0022, 0, -7, 3, 2, 3, 0x00000025 ),               /* '"' */
  EW_GLYPH( 0x0023, 0, -7, 4, 7, 5, 0x0000002C ),               /* '#' */
  EW_GLYPH( 0x0024, 0, -7, 5, 8, 5, 0x0000004C ),               /* '$' */
  EW_GLYPH( 0x0025, 1, -7, 6, 7, 8, 0x00000081 ),               /* '%' */
  EW_GLYPH( 0x0026, 0, -7, 6, 7, 6, 0x000000B2 ),               /* '&' */
  EW_GLYPH( 0x0027, 0, -7, 1, 2, 2, 0x000000E1 ),               /* ''' */
  EW_GLYPH( 0x0028, 0, -7, 3, 9, 3, 0x000000E5 ),               /* '(' */
  EW_GLYPH( 0x0029, 0, -7, 3, 9, 3, 0x000000FB ),               /* ')' */
  EW_GLYPH( 0x002A, 0, -7, 3, 3, 4, 0x00000117 ),               /* '*' */
  EW_GLYPH( 0x002B, 0, -5, 5, 5, 5, 0x00000125 ),               /* '+' */
  EW_GLYPH( 0x002C, 1, -1, 1, 2, 3, 0x0000013D ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -3, 2, 1, 3, 0x00000141 ),               /* '-' */
  EW_GLYPH( 0x002E, 1, -1, 1, 1, 3, 0x00000146 ),               /* '.' */
  EW_GLYPH( 0x002F, 0, -7, 3, 7, 3, 0x0000014A ),               /* '/' */
  EW_GLYPH( 0x0030, 0, -7, 4, 7, 5, 0x0000015F ),               /* '0' */
  EW_GLYPH( 0x0031, 1, -7, 2, 7, 5, 0x00000172 ),               /* '1' */
  EW_GLYPH( 0x0032, 0, -7, 4, 7, 5, 0x0000017D ),               /* '2' */
  EW_GLYPH( 0x0033, 0, -7, 4, 7, 5, 0x0000019D ),               /* '3' */
  EW_GLYPH( 0x0034, 0, -7, 5, 7, 5, 0x000001BE ),               /* '4' */
  EW_GLYPH( 0x0035, 0, -7, 4, 7, 5, 0x000001E3 ),               /* '5' */
  EW_GLYPH( 0x0036, 0, -7, 4, 7, 5, 0x00000205 ),               /* '6' */
  EW_GLYPH( 0x0037, 0, -7, 4, 7, 5, 0x00000220 ),               /* '7' */
  EW_GLYPH( 0x0038, 0, -7, 4, 7, 5, 0x00000237 ),               /* '8' */
  EW_GLYPH( 0x0039, 0, -7, 4, 7, 5, 0x00000252 ),               /* '9' */
  EW_GLYPH( 0x003A, 1, -5, 1, 5, 3, 0x0000026D ),               /* ':' */
  EW_GLYPH( 0x003B, 1, -5, 1, 6, 3, 0x00000276 ),               /* ';' */
  EW_GLYPH( 0x003C, 1, -6, 3, 5, 5, 0x0000027F ),               /* '<' */
  EW_GLYPH( 0x003D, 0, -5, 4, 3, 5, 0x00000292 ),               /* '=' */
  EW_GLYPH( 0x003E, 1, -6, 3, 5, 5, 0x000002A1 ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -7, 5, 7, 5, 0x000002B9 ),               /* '?' */
  EW_GLYPH( 0x0040, 1, -7, 8, 9, 9, 0x000002DB ),               /* '@' */
  EW_GLYPH( 0x0041, 0, -7, 7, 7, 6, 0x0000032C ),               /* 'A' */
  EW_GLYPH( 0x0042, 1, -7, 4, 7, 6, 0x0000035D ),               /* 'B' */
  EW_GLYPH( 0x0043, 1, -7, 5, 7, 7, 0x00000378 ),               /* 'C' */
  EW_GLYPH( 0x0044, 1, -7, 5, 7, 7, 0x00000398 ),               /* 'D' */
  EW_GLYPH( 0x0045, 1, -7, 5, 7, 6, 0x000003B4 ),               /* 'E' */
  EW_GLYPH( 0x0046, 1, -7, 4, 7, 6, 0x000003DD ),               /* 'F' */
  EW_GLYPH( 0x0047, 1, -7, 5, 7, 7, 0x000003F4 ),               /* 'G' */
  EW_GLYPH( 0x0048, 1, -7, 5, 7, 7, 0x0000041B ),               /* 'H' */
  EW_GLYPH( 0x0049, 1, -7, 1, 7, 3, 0x00000432 ),               /* 'I' */
  EW_GLYPH( 0x004A, 0, -7, 4, 7, 5, 0x00000437 ),               /* 'J' */
  EW_GLYPH( 0x004B, 1, -7, 5, 7, 6, 0x00000448 ),               /* 'K' */
  EW_GLYPH( 0x004C, 1, -7, 4, 7, 5, 0x00000471 ),               /* 'L' */
  EW_GLYPH( 0x004D, 0, -7, 7, 7, 7, 0x0000047F ),               /* 'M' */
  EW_GLYPH( 0x004E, 1, -7, 5, 7, 7, 0x000004A4 ),               /* 'N' */
  EW_GLYPH( 0x004F, 1, -7, 5, 7, 7, 0x000004C0 ),               /* 'O' */
  EW_GLYPH( 0x0050, 1, -7, 4, 7, 6, 0x000004DD ),               /* 'P' */
  EW_GLYPH( 0x0051, 1, -7, 5, 7, 7, 0x000004F4 ),               /* 'Q' */
  EW_GLYPH( 0x0052, 1, -7, 6, 7, 7, 0x00000512 ),               /* 'R' */
  EW_GLYPH( 0x0053, 1, -7, 4, 7, 6, 0x00000537 ),               /* 'S' */
  EW_GLYPH( 0x0054, 0, -7, 5, 7, 5, 0x00000558 ),               /* 'T' */
  EW_GLYPH( 0x0055, 1, -7, 5, 7, 7, 0x0000056D ),               /* 'U' */
  EW_GLYPH( 0x0056, 0, -7, 7, 7, 6, 0x00000584 ),               /* 'V' */
  EW_GLYPH( 0x0057, 0, -7, 9, 7, 9, 0x000005AD ),               /* 'W' */
  EW_GLYPH( 0x0058, 0, -7, 5, 7, 5, 0x000005DF ),               /* 'X' */
  EW_GLYPH( 0x0059, 1, -7, 5, 7, 7, 0x00000603 ),               /* 'Y' */
  EW_GLYPH( 0x005A, 0, -7, 6, 7, 6, 0x0000061B ),               /* 'Z' */
  EW_GLYPH( 0x005B, 1, -7, 2, 9, 3, 0x0000064E ),               /* '[' */
  EW_GLYPH( 0x005C, 0, -7, 3, 7, 3, 0x0000065C ),               /* '\' */
  EW_GLYPH( 0x005D, 0, -7, 2, 9, 3, 0x0000066D ),               /* ']' */
  EW_GLYPH( 0x005E, 0, -7, 3, 3, 3, 0x00000679 ),               /* '^' */
  EW_GLYPH( 0x005F, 0, 1, 5, 1, 5, 0x00000685 ),                /* '_' */
  EW_GLYPH( 0x0060, 0, -7, 2, 2, 3, 0x0000068E ),               /* '`' */
  EW_GLYPH( 0x0061, 0, -5, 4, 5, 5, 0x00000695 ),               /* 'a' */
  EW_GLYPH( 0x0062, 0, -7, 4, 7, 5, 0x000006AE ),               /* 'b' */
  EW_GLYPH( 0x0063, 0, -5, 4, 5, 5, 0x000006C4 ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -7, 4, 7, 5, 0x000006DB ),               /* 'd' */
  EW_GLYPH( 0x0065, 0, -5, 4, 5, 5, 0x000006F1 ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -7, 3, 7, 4, 0x0000070A ),               /* 'f' */
  EW_GLYPH( 0x0067, 0, -5, 4, 7, 5, 0x0000071D ),               /* 'g' */
  EW_GLYPH( 0x0068, 0, -7, 4, 7, 5, 0x00000738 ),               /* 'h' */
  EW_GLYPH( 0x0069, 0, -7, 1, 7, 2, 0x0000074A ),               /* 'i' */
  EW_GLYPH( 0x006A, -1, -7, 2, 9, 2, 0x00000750 ),              /* 'j' */
  EW_GLYPH( 0x006B, 0, -7, 4, 7, 5, 0x00000761 ),               /* 'k' */
  EW_GLYPH( 0x006C, 0, -7, 1, 7, 2, 0x0000077C ),               /* 'l' */
  EW_GLYPH( 0x006D, 0, -5, 7, 5, 8, 0x00000781 ),               /* 'm' */
  EW_GLYPH( 0x006E, 0, -5, 4, 5, 5, 0x0000079C ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -5, 4, 5, 5, 0x000007A9 ),               /* 'o' */
  EW_GLYPH( 0x0070, 0, -5, 4, 7, 5, 0x000007BA ),               /* 'p' */
  EW_GLYPH( 0x0071, 0, -5, 4, 7, 5, 0x000007D1 ),               /* 'q' */
  EW_GLYPH( 0x0072, 0, -5, 3, 5, 3, 0x000007E8 ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -5, 4, 5, 5, 0x000007F4 ),               /* 's' */
  EW_GLYPH( 0x0074, -1, -7, 3, 7, 3, 0x0000080D ),              /* 't' */
  EW_GLYPH( 0x0075, 0, -5, 4, 5, 5, 0x00000821 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -5, 5, 5, 6, 0x0000082F ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -5, 5, 5, 6, 0x00000845 ),               /* 'w' */
  EW_GLYPH( 0x0078, 0, -5, 4, 5, 5, 0x0000085B ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -5, 5, 7, 6, 0x0000086D ),               /* 'y' */
  EW_GLYPH( 0x007A, 0, -5, 3, 5, 4, 0x0000088B ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -7, 3, 9, 3, 0x000008A0 ),               /* '{' */
  EW_GLYPH( 0x007C, 1, -7, 1, 8, 3, 0x000008BA ),               /* '|' */
  EW_GLYPH( 0x007D, 0, -7, 3, 9, 3, 0x000008BF ),               /* '}' */
  EW_GLYPH( 0x007E, 0, -4, 4, 2, 5, 0x000008D7 ),               /* '~' */

  EW_FONT_PIXEL( ResourcesDefaultFont, 0x000008E3 )             /* ratio 96.28 % */
    0x11D81FF7, 0x9E79F1F3, 0xD8E673E7, 0x7CF637CF, 0xDB56BFF2, 0x77EC7A71, 0x3B9E56B0,
    0x7CE62EA2, 0x7B8F60DF, 0x27F3ECA2, 0x87475446, 0x60CCE179, 0x7FE6D8DE, 0x6E3BD8DE,
    0x6F31BF77, 0x1139FB3A, 0xB873DE77, 0x79936D77, 0xCBCEEBDD, 0x88C4775A, 0x7BDE73BA,
    0xCE3FF7DF, 0xD8B74E59, 0xD9DFACE3, 0xF79EED21, 0x7B0EDB9E, 0x63BB38EE, 0xE73A731D,
    0x7759C28C, 0xFF75E80F, 0xEBD13DC3, 0xCE72731E, 0x8AF6DDA8, 0x86841D60, 0x8C631138,
    0x9C0331D9, 0xC27B08A8, 0x93630F12, 0xEBD80CE7, 0xCE70F34E, 0x5EDCFB80, 0x79934D7A,
    0x77B89D7B, 0xEC02200D, 0x98776515, 0xD6222583, 0x8DFE8136, 0xF63AC358, 0xB82C358C,
    0xE631D7F7, 0x190FC676, 0xBE21619B, 0xAEFFC6E1, 0x879866B9, 0x6DF76F33, 0x9DE6B173,
    0xE1FAD6E7, 0x61BAD62C, 0x325F2C1E, 0x1DCCEF42, 0xB00DF8EE, 0xAEE5E60E, 0x62CE99C3,
    0xEE706B4D, 0x23F3D73A, 0x8358AC5E, 0x78DC09E3, 0xE0D6372F, 0x9AEFB8E8, 0x8542FD33,
    0xDF88731F, 0x00000004

  EW_FONT_KERNING_CODES( ResourcesDefaultFont )
    0x00000039, 0x00200041, 0x002C0046, 0x002C0050, 0x002C0054, 0x002C0056, 0x002C0057,
    0x002C0059, 0x002C0072, 0x002C0076, 0x002C0077, 0x002C0079, 0x002D0054, 0x002D0056,
    0x002D0059, 0x002E0046, 0x002E0050, 0x002E0054, 0x002E0056, 0x002E0057, 0x002E0059,
    0x002E0072, 0x002E0076, 0x002E0077, 0x002E0079, 0x00310031, 0x003A0054, 0x003A0059,
    0x00410020, 0x00410046, 0x00410050, 0x00410054, 0x00410056, 0x00410059, 0x00540041,
    0x0054004C, 0x00560041, 0x0056004C, 0x0057004C, 0x00590041, 0x0059004C, 0x00610054,
    0x00610056, 0x00610059, 0x00630054, 0x00650054, 0x00650056, 0x00650059, 0x006F0054,
    0x006F0056, 0x006F0059, 0x00700059, 0x00710059, 0x00730054, 0x00750059, 0x00760059,
    0x00770054, 0x00790054,

  EW_FONT_KERNING_VALUES( ResourcesDefaultFont )
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
EW_END_OF_FONT_RES( ResourcesDefaultFont )

/* Embedded Wizard */
