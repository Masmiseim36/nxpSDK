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

/* Font resource : 'Oximeter::FontM' */
EW_DEFINE_FONT_RES( OximeterFontM, 11, 3, 0, 16, 0x0001, 0x0060 )
  EW_GLYPH( 0x0001, 0, -8, 6, 8, 6, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 2, 0x000000BC ),                /* ' ' */
  EW_GLYPH( 0x0021, 0, -8, 2, 8, 3, 0x000000BC ),               /* '!' */
  EW_GLYPH( 0x0022, 0, -8, 3, 3, 3, 0x000000F1 ),               /* '"' */
  EW_GLYPH( 0x0023, 0, -8, 6, 8, 6, 0x0000011D ),               /* '#' */
  EW_GLYPH( 0x0024, 0, -9, 5, 10, 5, 0x000001E5 ),              /* '$' */
  EW_GLYPH( 0x0025, 0, -8, 7, 8, 8, 0x000002B0 ),               /* '%' */
  EW_GLYPH( 0x0026, 0, -8, 6, 8, 6, 0x00000381 ),               /* '&' */
  EW_GLYPH( 0x0027, 0, -8, 2, 3, 2, 0x00000439 ),               /* ''' */
  EW_GLYPH( 0x0028, 0, -10, 3, 12, 3, 0x00000455 ),             /* '(' */
  EW_GLYPH( 0x0029, -1, -10, 4, 12, 3, 0x000004CE ),            /* ')' */
  EW_GLYPH( 0x002A, 0, -8, 3, 3, 3, 0x0000054D ),               /* '*' */
  EW_GLYPH( 0x002B, 0, -5, 5, 5, 5, 0x00000574 ),               /* '+' */
  EW_GLYPH( 0x002C, 0, -2, 2, 3, 2, 0x000005B7 ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -4, 3, 1, 3, 0x000005D2 ),               /* '-' */
  EW_GLYPH( 0x002E, 0, -1, 2, 1, 2, 0x000005E2 ),               /* '.' */
  EW_GLYPH( 0x002F, -1, -8, 5, 9, 4, 0x000005EC ),              /* '/' */
  EW_GLYPH( 0x0030, 0, -8, 5, 8, 5, 0x00000659 ),               /* '0' */
  EW_GLYPH( 0x0031, 0, -8, 5, 8, 5, 0x000006F3 ),               /* '1' */
  EW_GLYPH( 0x0032, 0, -8, 5, 8, 5, 0x00000753 ),               /* '2' */
  EW_GLYPH( 0x0033, 0, -8, 5, 8, 5, 0x000007ED ),               /* '3' */
  EW_GLYPH( 0x0034, 0, -8, 5, 8, 5, 0x00000894 ),               /* '4' */
  EW_GLYPH( 0x0035, 0, -8, 5, 8, 5, 0x00000910 ),               /* '5' */
  EW_GLYPH( 0x0036, 0, -8, 5, 8, 5, 0x000009AE ),               /* '6' */
  EW_GLYPH( 0x0037, 0, -8, 5, 8, 5, 0x00000A4E ),               /* '7' */
  EW_GLYPH( 0x0038, 0, -8, 5, 8, 5, 0x00000ADC ),               /* '8' */
  EW_GLYPH( 0x0039, 0, -8, 5, 8, 5, 0x00000B7E ),               /* '9' */
  EW_GLYPH( 0x003A, 0, -6, 2, 6, 2, 0x00000C1D ),               /* ':' */
  EW_GLYPH( 0x003B, 0, -6, 2, 7, 2, 0x00000C41 ),               /* ';' */
  EW_GLYPH( 0x003C, 0, -5, 5, 5, 5, 0x00000C76 ),               /* '<' */
  EW_GLYPH( 0x003D, 0, -4, 5, 3, 5, 0x00000CDA ),               /* '=' */
  EW_GLYPH( 0x003E, 0, -5, 5, 5, 5, 0x00000D22 ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -8, 5, 8, 4, 0x00000D8A ),               /* '?' */
  EW_GLYPH( 0x0040, 0, -8, 8, 10, 9, 0x00000E0A ),              /* '@' */
  EW_GLYPH( 0x0041, 0, -8, 6, 8, 6, 0x00000F51 ),               /* 'A' */
  EW_GLYPH( 0x0042, 0, -8, 6, 8, 6, 0x00000FF5 ),               /* 'B' */
  EW_GLYPH( 0x0043, 0, -8, 5, 8, 5, 0x0000109F ),               /* 'C' */
  EW_GLYPH( 0x0044, 0, -8, 6, 8, 6, 0x0000112D ),               /* 'D' */
  EW_GLYPH( 0x0045, 0, -8, 5, 8, 5, 0x000011BD ),               /* 'E' */
  EW_GLYPH( 0x0046, 0, -8, 5, 8, 5, 0x00001234 ),               /* 'F' */
  EW_GLYPH( 0x0047, 0, -8, 6, 8, 6, 0x00001297 ),               /* 'G' */
  EW_GLYPH( 0x0048, 0, -8, 6, 8, 7, 0x00001344 ),               /* 'H' */
  EW_GLYPH( 0x0049, 1, -8, 1, 8, 3, 0x000013A9 ),               /* 'I' */
  EW_GLYPH( 0x004A, 0, -8, 3, 8, 4, 0x000013BB ),               /* 'J' */
  EW_GLYPH( 0x004B, 0, -8, 6, 8, 6, 0x000013ED ),               /* 'K' */
  EW_GLYPH( 0x004C, 0, -8, 5, 8, 4, 0x00001473 ),               /* 'L' */
  EW_GLYPH( 0x004D, 0, -8, 7, 8, 8, 0x000014B5 ),               /* 'M' */
  EW_GLYPH( 0x004E, 0, -8, 6, 8, 6, 0x0000154E ),               /* 'N' */
  EW_GLYPH( 0x004F, 0, -8, 6, 8, 7, 0x000015DE ),               /* 'O' */
  EW_GLYPH( 0x0050, 0, -8, 5, 8, 5, 0x0000168F ),               /* 'P' */
  EW_GLYPH( 0x0051, 0, -8, 7, 10, 7, 0x00001708 ),              /* 'Q' */
  EW_GLYPH( 0x0052, 0, -8, 6, 8, 6, 0x000017F7 ),               /* 'R' */
  EW_GLYPH( 0x0053, 0, -8, 5, 8, 5, 0x0000188D ),               /* 'S' */
  EW_GLYPH( 0x0054, 0, -8, 5, 8, 5, 0x00001931 ),               /* 'T' */
  EW_GLYPH( 0x0055, 0, -8, 6, 8, 6, 0x00001988 ),               /* 'U' */
  EW_GLYPH( 0x0056, 0, -8, 6, 8, 6, 0x000019F4 ),               /* 'V' */
  EW_GLYPH( 0x0057, 0, -8, 9, 8, 8, 0x00001A96 ),               /* 'W' */
  EW_GLYPH( 0x0058, 0, -8, 6, 8, 6, 0x00001BB1 ),               /* 'X' */
  EW_GLYPH( 0x0059, 0, -8, 6, 8, 5, 0x00001C6E ),               /* 'Y' */
  EW_GLYPH( 0x005A, 0, -8, 6, 8, 6, 0x00001CFA ),               /* 'Z' */
  EW_GLYPH( 0x005B, 0, -9, 3, 11, 3, 0x00001DA8 ),              /* '[' */
  EW_GLYPH( 0x005C, 0, -8, 4, 8, 3, 0x00001DE6 ),               /* '\' */
  EW_GLYPH( 0x005D, 0, -9, 2, 11, 3, 0x00001E3B ),              /* ']' */
  EW_GLYPH( 0x005E, 0, -8, 4, 4, 4, 0x00001E6C ),               /* '^' */
  EW_GLYPH( 0x005F, -1, 1, 6, 1, 5, 0x00001EAE ),               /* '_' */
  EW_GLYPH( 0x0060, 0, -9, 3, 2, 3, 0x00001ECB ),               /* '`' */
  EW_GLYPH( 0x0061, 0, -6, 5, 6, 5, 0x00001EE1 ),               /* 'a' */
  EW_GLYPH( 0x0062, 0, -9, 5, 9, 5, 0x00001F64 ),               /* 'b' */
  EW_GLYPH( 0x0063, 0, -6, 5, 6, 4, 0x00001FED ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -9, 5, 9, 5, 0x0000205D ),               /* 'd' */
  EW_GLYPH( 0x0065, 0, -6, 5, 6, 5, 0x000020D0 ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -9, 4, 9, 3, 0x00002156 ),               /* 'f' */
  EW_GLYPH( 0x0067, 0, -6, 5, 8, 5, 0x000021A0 ),               /* 'g' */
  EW_GLYPH( 0x0068, 0, -9, 5, 9, 5, 0x0000223A ),               /* 'h' */
  EW_GLYPH( 0x0069, 0, -8, 2, 8, 3, 0x000022A3 ),               /* 'i' */
  EW_GLYPH( 0x006A, -1, -8, 3, 10, 3, 0x000022CE ),             /* 'j' */
  EW_GLYPH( 0x006B, 1, -9, 4, 9, 5, 0x00002311 ),               /* 'k' */
  EW_GLYPH( 0x006C, 0, -9, 3, 9, 3, 0x00002374 ),               /* 'l' */
  EW_GLYPH( 0x006D, 0, -6, 8, 6, 8, 0x000023A4 ),               /* 'm' */
  EW_GLYPH( 0x006E, 0, -6, 5, 6, 5, 0x0000242B ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -6, 5, 6, 5, 0x0000247F ),               /* 'o' */
  EW_GLYPH( 0x0070, 0, -6, 5, 8, 5, 0x000024FD ),               /* 'p' */
  EW_GLYPH( 0x0071, 0, -6, 5, 8, 5, 0x0000258F ),               /* 'q' */
  EW_GLYPH( 0x0072, 0, -6, 4, 6, 4, 0x00002608 ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -6, 4, 6, 4, 0x0000264E ),               /* 's' */
  EW_GLYPH( 0x0074, 0, -8, 3, 8, 3, 0x000026C0 ),               /* 't' */
  EW_GLYPH( 0x0075, 0, -6, 5, 6, 5, 0x00002701 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -6, 5, 6, 5, 0x0000274A ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -6, 7, 6, 7, 0x000027C0 ),               /* 'w' */
  EW_GLYPH( 0x0078, 0, -6, 5, 6, 4, 0x00002876 ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -6, 5, 8, 4, 0x000028F5 ),               /* 'y' */
  EW_GLYPH( 0x007A, 0, -6, 5, 6, 4, 0x00002983 ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -9, 3, 11, 3, 0x000029F9 ),              /* '{' */
  EW_GLYPH( 0x007C, 0, -9, 2, 11, 2, 0x00002A52 ),              /* '|' */
  EW_GLYPH( 0x007D, 0, -9, 3, 11, 3, 0x00002A76 ),              /* '}' */
  EW_GLYPH( 0x007E, 0, -3, 5, 1, 5, 0x00002AD0 ),               /* '~' */

  EW_FONT_PIXEL( OximeterFontM, 0x00002AE9 )                    /* ratio 82.69 % */
    0x7DEF7BFF, 0x65DEEFE7, 0xBA3F98EF, 0xE7740063, 0x3F3C77B0, 0x12EF77F3, 0x22584307,
    0xEF57B1BE, 0x5B98AFFC, 0x3992F97C, 0x5FAF678F, 0x6B7F4F3F, 0xF08CEE3F, 0xBEFF3DAD,
    0xC565C771, 0x7FEE8982, 0x39DDE33E, 0x1D9EB086, 0xFB076FE2, 0x9C8C2399, 0xF9E7CDFB,
    0xDFF95FFF, 0x1BFD55E2, 0xA1FF03F3, 0xF92F37FA, 0x269C77DF, 0x7E0EFBFF, 0x3BDC4213,
    0xCD609F62, 0x59E13C13, 0x6BAFDE72, 0xCEBBB73E, 0xAE76CE1F, 0xFA7CC799, 0x528B91B9,
    0xD6195DC3, 0xA443065F, 0x31831AE7, 0x37819FBC, 0x58552F38, 0x19032441, 0x798B14AC,
    0xBF52C539, 0x0FC1DEB9, 0xEAEEB7D0, 0x1C81775B, 0x77BE7755, 0xF07A85AD, 0xB9E08C2F,
    0xF5010FD8, 0xBC242DC7, 0x66F235F7, 0x9F7FDFD5, 0x219C0321, 0x7BFEFEA3, 0xBE7ACDE4,
    0x906FD613, 0x58000001, 0xFB1ACF37, 0xCDEE22FE, 0xE20DC399, 0x39E6E93F, 0xD7459C43,
    0xE7FEABBD, 0x2313ECF3, 0xF3EA5622, 0xC1E3BE84, 0x3E4E11CD, 0x1E43E4F7, 0x7418C634,
    0xF7C1FF33, 0xDEFBDCDE, 0xEF7108DC, 0x91DEEA1D, 0x8D79CF01, 0xEE24C77B, 0xDACE70C9,
    0x43B52627, 0xC7EE1D9D, 0xFE2FCF33, 0xED29C9DF, 0xA77DAE73, 0xEF7BE63B, 0xD77BDF7D,
    0x0F9933C5, 0x0D631FBE, 0xE26A3FBE, 0x6E233FBB, 0x6FC3483C, 0x51AF433C, 0x6D9FBC67,
    0xA737999F, 0x6E3CFEEF, 0x27FEBCAE, 0xFCF334B6, 0x9399DFAE, 0x653BF2EF, 0xB580B5AD,
    0x720B5AD6, 0xD11773D4, 0xD9EFFAD3, 0xBFDE977B, 0xF67B7D62, 0xBDF577BD, 0x77BDF577,
    0x7B9585AD, 0xE5BFFCF2, 0xF2B5BEF9, 0xA673CD37, 0x2323BBBA, 0xB952FD05, 0xC4DE37F8,
    0xF9DF901E, 0xB39CE22F, 0x7FE7D4CF, 0x844777BE, 0xD7678CB3, 0x59D622DF, 0x18F99D60,
    0x4BF3F6B1, 0x675CCDF7, 0xDDF876FE, 0x4FF84FF3, 0xBF51FC64, 0xB06B5D89, 0xEF232EC6,
    0x3BDF55BB, 0xEE2AA2ED, 0x3BDE4D3D, 0xBC846303, 0xF7BC8FFF, 0xF099E037, 0x29E7BC84,
    0xDF1DBFF5, 0x4AA7DF7B, 0x0390018C, 0x7BE32555, 0x3DEE2FEF, 0x0F5BDE4D, 0x24F0DBE2,
    0xBE224F00, 0x27ADEF2D, 0xDE4BBDEE, 0x77BC80BB, 0x00077BC8, 0xF715DEF2, 0x1DEF21DE,
    0xDE4BDE40, 0xF501000B, 0xBDF0BFBF, 0x295546F7, 0x7DF60063, 0x9EA9ED32, 0xADEF7C42,
    0x13F19B19, 0x377BC800, 0x0006EF79, 0xE0804E02, 0xF3200010, 0xFF363BB7, 0x464BFA14,
    0x0203DC2D, 0xBCF181EE, 0x0D8AAE84, 0x00000001, 0x1C67DEF2, 0x0BE2CDCC, 0xE0F3CFCF,
    0xF987EF5B, 0x20D384AB, 0x6138C401, 0x8C25E27C, 0x17BEB027, 0x3BE133D6, 0x86F01EB0,
    0xE22F9FFA, 0xD574673B, 0x11072BD0, 0x3E4418C0, 0x77C57435, 0xEF71E8CE, 0x1B7DD4BF,
    0xBAA6A04F, 0x07FEEADD, 0xBE7FEA00, 0xA339DF10, 0x2BF0CF3D, 0xC804C923, 0x598B5081,
    0x7EFFF7E2, 0x44E6BCCD, 0xB89F7D7E, 0xEDE45FF7, 0x45580A86, 0x7791FEDE, 0x843EF07E,
    0xFE7BC398, 0x39D9E4F9, 0x116AB086, 0x7F30763B, 0x739D8473, 0xF77E7C9F, 0xBAEFDFDE,
    0x0000003F, 0xF3C7E304, 0x00000002, 0xD91BA4F1, 0x15FBC4F7, 0xAB07FD5F, 0x4C69CB6A,
    0x0EE359B5, 0xBF515576, 0x37CD8C89, 0x1F238FCC, 0xAFB4C967, 0x9E3E6331, 0xB8CF1AC7,
    0x8FFCC622, 0xAF6AB17F, 0xDFFE4CFF, 0x8C617393, 0xC7E60D61, 0x1EB95CDF, 0x597BEFBE,
    0xC0D209F3, 0xAC86B7EF, 0x9FBF6DE7, 0xC53BE667, 0x7E67EFDF, 0x7937D572, 0x3E1827DF,
    0x3C080030, 0xCF1BDEF7, 0xE813DFBD, 0xF8927AC6, 0x78C3FD1A, 0xDEE37E26, 0xFA3EE31B,
    0xD0000001, 0x7E088BCF, 0xC191B18B, 0xF862EA2E, 0x0000B7F6, 0xF917A5BE, 0x3E7FFF97,
    0x7BDE554F, 0x3792CEEF, 0xC45FDF62, 0xFF7CB9B9, 0x8B763519, 0xCE7ABF8C, 0x60003F10,
    0xFFC1FFFD, 0x80352D7B, 0xFD61BF19, 0xFE7BD79F, 0x31D9CF51, 0x3F18027E, 0x12739EA9,
    0x3BE0016B, 0x6349C55B, 0x7F6D403B, 0xB3DE0E23, 0x9BF38C63, 0xD6BEDCF5, 0x664A3DBD,
    0xF786E73D, 0x39648750, 0x04000396, 0x18F77BBE, 0xF2305E77, 0x84FDC18E, 0xCFA798EE,
    0xC55FBEAC, 0x77E00086, 0x3FBDF186, 0x10003223, 0xB63B1D88, 0x62D88400, 0x80004D8B,
    0x02367EC1, 0x9C8D3BC0, 0x6F16B36E, 0x3F15AC8B, 0xD6200000, 0x5FEFDF9B, 0xEFAC67E7,
    0x19856739, 0x00016FF7, 0x77EFC800, 0x7DEFAC72, 0x000048CC, 0xC8C5F9EF, 0xBF6D6633,
    0xBF6D443F, 0x378CF231, 0x8C7FFF7F, 0x8CD7BFB9, 0x8352C01A, 0xFF5AF7FF, 0xE9DF007F,
    0xDAA4E235, 0x8ED8C0FE, 0x677C938A, 0xCFFBF903, 0x0469ECF1, 0x9FF8C100, 0xED66EFAC,
    0xA47AFC61, 0x3DF9D57F, 0x763D5E1E, 0xFE6006E7, 0x012D67C7, 0xC37F2C00, 0x97E57D3F,
    0x3B59987B, 0x67F59C97, 0x0CFD8BFD, 0xE9E3CCBF, 0xFFFCA6B1, 0xCCD639FF, 0x6FFFFEB5,
    0x3B186644, 0x4F5FC188, 0xF37F78FF, 0x4D7E7889, 0x5AFCF908, 0x2BE2B3BF, 0x6623BCBF,
    0xB11EAFD6, 0x515766EE, 0xB3C6DF85, 0xEB1EF758, 0x35F88BBE, 0x99E30BFD, 0xECBDC6FC,
    0xD3202D65, 0x993F79FB, 0x058D6B00, 0xAFD64000, 0xE2C006FD, 0x0596FCB7, 0xDCE30DF8,
    0x00000033

  EW_FONT_KERNING_CODES( OximeterFontM )
  EW_FONT_KERNING_VALUES( OximeterFontM )
EW_END_OF_FONT_RES( OximeterFontM )

/* Embedded Wizard */
