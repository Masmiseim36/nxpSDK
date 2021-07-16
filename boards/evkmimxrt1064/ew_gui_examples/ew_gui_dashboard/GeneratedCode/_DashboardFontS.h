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
EW_DEFINE_FONT_RES( DashboardFontS, 9, 3, 0, 16, 0x0001, 0x0060 )
  EW_GLYPH( 0x0001, 0, -6, 4, 6, 4, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 2, 0x00000048 ),                /* ' ' */
  EW_GLYPH( 0x0021, 0, -6, 2, 6, 2, 0x00000048 ),               /* '!' */
  EW_GLYPH( 0x0022, 0, -6, 3, 3, 3, 0x0000007C ),               /* '"' */
  EW_GLYPH( 0x0023, 0, -6, 5, 6, 5, 0x000000AA ),               /* '#' */
  EW_GLYPH( 0x0024, 0, -7, 4, 8, 4, 0x0000012F ),               /* '$' */
  EW_GLYPH( 0x0025, 0, -6, 7, 7, 6, 0x000001C1 ),               /* '%' */
  EW_GLYPH( 0x0026, 0, -6, 6, 7, 5, 0x0000028B ),               /* '&' */
  EW_GLYPH( 0x0027, 0, -6, 2, 3, 2, 0x0000033C ),               /* ''' */
  EW_GLYPH( 0x0028, 0, -7, 3, 9, 3, 0x00000357 ),               /* '(' */
  EW_GLYPH( 0x0029, 0, -7, 3, 9, 3, 0x000003B9 ),               /* ')' */
  EW_GLYPH( 0x002A, 0, -6, 3, 4, 3, 0x00000417 ),               /* '*' */
  EW_GLYPH( 0x002B, 0, -5, 4, 5, 4, 0x0000044B ),               /* '+' */
  EW_GLYPH( 0x002C, 0, -2, 2, 4, 2, 0x00000497 ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -3, 3, 2, 3, 0x000004BE ),               /* '-' */
  EW_GLYPH( 0x002E, 0, -2, 2, 2, 2, 0x000004D0 ),               /* '.' */
  EW_GLYPH( 0x002F, 0, -6, 4, 7, 4, 0x000004E3 ),               /* '/' */
  EW_GLYPH( 0x0030, 0, -6, 5, 7, 5, 0x00000539 ),               /* '0' */
  EW_GLYPH( 0x0031, 0, -6, 3, 6, 3, 0x000005C1 ),               /* '1' */
  EW_GLYPH( 0x0032, 0, -6, 4, 6, 4, 0x000005F7 ),               /* '2' */
  EW_GLYPH( 0x0033, 0, -6, 4, 7, 4, 0x00000660 ),               /* '3' */
  EW_GLYPH( 0x0034, 0, -6, 5, 6, 5, 0x000006E0 ),               /* '4' */
  EW_GLYPH( 0x0035, 0, -6, 4, 7, 4, 0x00000753 ),               /* '5' */
  EW_GLYPH( 0x0036, 0, -6, 4, 7, 4, 0x000007D9 ),               /* '6' */
  EW_GLYPH( 0x0037, 0, -6, 4, 6, 4, 0x00000861 ),               /* '7' */
  EW_GLYPH( 0x0038, 0, -6, 5, 7, 4, 0x000008C0 ),               /* '8' */
  EW_GLYPH( 0x0039, 0, -6, 4, 7, 4, 0x00000950 ),               /* '9' */
  EW_GLYPH( 0x003A, 0, -4, 2, 4, 2, 0x000009D7 ),               /* ':' */
  EW_GLYPH( 0x003B, 0, -4, 2, 6, 2, 0x00000A00 ),               /* ';' */
  EW_GLYPH( 0x003C, 0, -5, 4, 5, 4, 0x00000A42 ),               /* '<' */
  EW_GLYPH( 0x003D, 0, -4, 4, 3, 4, 0x00000A96 ),               /* '=' */
  EW_GLYPH( 0x003E, 0, -5, 4, 5, 4, 0x00000ADB ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -6, 4, 6, 4, 0x00000B2C ),               /* '?' */
  EW_GLYPH( 0x0040, 0, -5, 5, 6, 5, 0x00000B90 ),               /* '@' */
  EW_GLYPH( 0x0041, 0, -6, 5, 6, 5, 0x00000C16 ),               /* 'A' */
  EW_GLYPH( 0x0042, 0, -6, 5, 7, 5, 0x00000C8F ),               /* 'B' */
  EW_GLYPH( 0x0043, 0, -6, 4, 7, 4, 0x00000D1C ),               /* 'C' */
  EW_GLYPH( 0x0044, 0, -6, 5, 7, 5, 0x00000D8F ),               /* 'D' */
  EW_GLYPH( 0x0045, 0, -6, 4, 6, 4, 0x00000E10 ),               /* 'E' */
  EW_GLYPH( 0x0046, 0, -6, 4, 6, 4, 0x00000E7E ),               /* 'F' */
  EW_GLYPH( 0x0047, 0, -6, 5, 7, 5, 0x00000EDB ),               /* 'G' */
  EW_GLYPH( 0x0048, 0, -6, 5, 6, 5, 0x00000F64 ),               /* 'H' */
  EW_GLYPH( 0x0049, 0, -6, 2, 6, 2, 0x00000FC6 ),               /* 'I' */
  EW_GLYPH( 0x004A, 0, -6, 3, 7, 3, 0x00000FE2 ),               /* 'J' */
  EW_GLYPH( 0x004B, 0, -6, 5, 6, 5, 0x0000101D ),               /* 'K' */
  EW_GLYPH( 0x004C, 0, -6, 4, 6, 4, 0x00001089 ),               /* 'L' */
  EW_GLYPH( 0x004D, 0, -6, 7, 6, 7, 0x000010C3 ),               /* 'M' */
  EW_GLYPH( 0x004E, 0, -6, 5, 6, 5, 0x0000115A ),               /* 'N' */
  EW_GLYPH( 0x004F, 0, -6, 5, 7, 5, 0x000011C1 ),               /* 'O' */
  EW_GLYPH( 0x0050, 0, -6, 5, 6, 4, 0x00001260 ),               /* 'P' */
  EW_GLYPH( 0x0051, 0, -6, 5, 8, 5, 0x000012D2 ),               /* 'Q' */
  EW_GLYPH( 0x0052, 0, -6, 5, 6, 5, 0x0000137D ),               /* 'R' */
  EW_GLYPH( 0x0053, 0, -6, 4, 7, 4, 0x000013F1 ),               /* 'S' */
  EW_GLYPH( 0x0054, 0, -6, 5, 6, 4, 0x00001477 ),               /* 'T' */
  EW_GLYPH( 0x0055, 0, -6, 5, 7, 5, 0x000014C6 ),               /* 'U' */
  EW_GLYPH( 0x0056, 0, -6, 5, 6, 5, 0x0000153C ),               /* 'V' */
  EW_GLYPH( 0x0057, 0, -6, 7, 6, 7, 0x000015AA ),               /* 'W' */
  EW_GLYPH( 0x0058, 0, -6, 5, 6, 5, 0x0000166C ),               /* 'X' */
  EW_GLYPH( 0x0059, 0, -6, 5, 6, 4, 0x000016F1 ),               /* 'Y' */
  EW_GLYPH( 0x005A, 0, -6, 4, 6, 4, 0x00001761 ),               /* 'Z' */
  EW_GLYPH( 0x005B, 0, -7, 3, 9, 2, 0x000017D8 ),               /* '[' */
  EW_GLYPH( 0x005C, 0, -6, 4, 7, 4, 0x00001820 ),               /* '\' */
  EW_GLYPH( 0x005D, 0, -7, 2, 9, 2, 0x00001870 ),               /* ']' */
  EW_GLYPH( 0x005E, 0, -5, 4, 2, 3, 0x000018A6 ),               /* '^' */
  EW_GLYPH( 0x005F, 0, 0, 4, 1, 3, 0x000018CA ),                /* '_' */
  EW_GLYPH( 0x0060, 0, -6, 3, 2, 3, 0x000018E3 ),               /* '`' */
  EW_GLYPH( 0x0061, 0, -5, 4, 6, 4, 0x000018F9 ),               /* 'a' */
  EW_GLYPH( 0x0062, 0, -6, 4, 7, 4, 0x0000195E ),               /* 'b' */
  EW_GLYPH( 0x0063, 0, -5, 4, 6, 4, 0x000019C7 ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -6, 4, 7, 4, 0x00001A21 ),               /* 'd' */
  EW_GLYPH( 0x0065, 0, -5, 4, 6, 4, 0x00001A81 ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -6, 4, 6, 3, 0x00001AE6 ),               /* 'f' */
  EW_GLYPH( 0x0067, 0, -5, 5, 7, 4, 0x00001B2C ),               /* 'g' */
  EW_GLYPH( 0x0068, 0, -6, 4, 6, 4, 0x00001BB0 ),               /* 'h' */
  EW_GLYPH( 0x0069, 0, -6, 2, 6, 2, 0x00001BFB ),               /* 'i' */
  EW_GLYPH( 0x006A, 0, -6, 2, 8, 2, 0x00001C1F ),               /* 'j' */
  EW_GLYPH( 0x006B, 0, -6, 4, 6, 4, 0x00001C58 ),               /* 'k' */
  EW_GLYPH( 0x006C, 0, -6, 3, 7, 2, 0x00001CB0 ),               /* 'l' */
  EW_GLYPH( 0x006D, 0, -5, 6, 5, 6, 0x00001CEE ),               /* 'm' */
  EW_GLYPH( 0x006E, 0, -5, 4, 5, 4, 0x00001D4C ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -5, 4, 6, 4, 0x00001D87 ),               /* 'o' */
  EW_GLYPH( 0x0070, 0, -4, 4, 6, 4, 0x00001DEF ),               /* 'p' */
  EW_GLYPH( 0x0071, 0, -5, 4, 7, 4, 0x00001E58 ),               /* 'q' */
  EW_GLYPH( 0x0072, 0, -5, 3, 5, 3, 0x00001EBC ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -5, 4, 6, 4, 0x00001EE9 ),               /* 's' */
  EW_GLYPH( 0x0074, 0, -5, 3, 6, 3, 0x00001F57 ),               /* 't' */
  EW_GLYPH( 0x0075, 0, -4, 4, 5, 4, 0x00001FA3 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -4, 4, 4, 4, 0x00001FEF ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -4, 6, 4, 6, 0x0000203B ),               /* 'w' */
  EW_GLYPH( 0x0078, 0, -4, 4, 4, 4, 0x000020AC ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -4, 4, 6, 4, 0x000020F8 ),               /* 'y' */
  EW_GLYPH( 0x007A, 0, -4, 4, 4, 4, 0x00002161 ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -7, 3, 9, 2, 0x000021B5 ),               /* '{' */
  EW_GLYPH( 0x007C, 0, -7, 2, 9, 2, 0x0000220D ),               /* '|' */
  EW_GLYPH( 0x007D, 0, -7, 3, 9, 2, 0x00002235 ),               /* '}' */
  EW_GLYPH( 0x007E, 0, -3, 4, 2, 4, 0x00002290 ),               /* '~' */

  EW_FONT_PIXEL( DashboardFontS, 0x000022BE )                   /* ratio 97.63 % */
    0x9E233CF9, 0xE7AC0007, 0x4D8CF599, 0x57EB559E, 0xBF9DFF15, 0xE151C877, 0x6F8E4E79,
    0xF1388AAF, 0xB3CB18CC, 0xF6524EC6, 0xCC62FCFB, 0x7CDE3BC8, 0x7ABAF8DF, 0x79DFD5BF,
    0xEF143BC7, 0xDF00E6F7, 0xDF6FDE13, 0x6F78E5F3, 0x1ED43B3D, 0x63D52FCF, 0x8B7E88EF,
    0xD249BE39, 0x7F9AAC4B, 0xD79ADFFF, 0x73B3FF7D, 0x59DEFFDE, 0xC20BF1FE, 0xF791CC6F,
    0xDFEE492F, 0x8237E91C, 0xCC2FACB9, 0x67CC2087, 0xC317317D, 0x231AADF3, 0x2CC112BE,
    0x7DF3DBEF, 0xF4967A7B, 0xC67CF67B, 0xBF191278, 0xF6261E22, 0x4FF9FE63, 0xAC4C66CF,
    0xFEFB9877, 0x86053DDD, 0xFCF7D2B8, 0xBF7FE467, 0xAFDF8D72, 0x7A8007DE, 0x7AECED3B,
    0x8D733DFC, 0xDEE6F1BE, 0x9DAE76F5, 0xC55298CF, 0xFDFBFEB0, 0x9DF77EF7, 0x18F5E06A,
    0x49EDF1D3, 0xFD19F7EC, 0x6B18CFBB, 0x86193A8D, 0x3959F67C, 0xEFFEF9F7, 0xACA7DBFF,
    0x5BFB99F7, 0x7729BE37, 0xFBF15DC7, 0x7EFF8D6E, 0xDF8AB59F, 0x46479678, 0x3F7E1E7A,
    0xF9996FD1, 0xE62924FF, 0x575FFE5E, 0x988FFEDC, 0x9DD16F67, 0x5DCEDC6F, 0xB98DF773,
    0xEFF739DF, 0x8C9E7DCC, 0x57EAFD5F, 0x3DFABF19, 0x67CF67BF, 0xDF1BF980, 0xFE3D1BFF,
    0xF3C7BD99, 0xDF7D6BBE, 0x83F39DF3, 0x3DEB0DF5, 0xA7BFBCCF, 0x37BF34F6, 0x3999CDEA,
    0xED2FEF1F, 0xFDF94DC5, 0xBDD93F76, 0xB8413DCE, 0xCF7DFF75, 0xB1A667FC, 0x9E2DFE82,
    0xCA31AB77, 0xBD46315E, 0xF6F1A9EB, 0x02FEC6A3, 0x7878E611, 0x5FD61CD6, 0x61FFB3AE,
    0x277E6BBE, 0x884D6FEE, 0xE4F757C3, 0xEFD19CFF, 0x92EEC6A3, 0x0C800D4F, 0x3AD73FEB,
    0xD6B11FFB, 0xCE533B9C, 0x7E3DF3C7, 0x7FDF656F, 0x9DCCF5AC, 0x7E63E621, 0x302DE78F,
    0x7BBF36DF, 0xE07CDFDD, 0x5BF0711D, 0xECE71967, 0xFA8D4A31, 0x927C97FB, 0x3D678B95,
    0x03FAA8C0, 0x0097E198, 0x63BE31BE, 0x7FF51994, 0x12FA30BE, 0x587EFC92, 0x7F514635,
    0xFBAC6000, 0xA9CC35CB, 0x2BDF3F7E, 0xED70AAAA, 0x1953D43F, 0x8C13DE27, 0xAC7AAB29,
    0x8FF17ABF, 0x3D44EE3F, 0xBF9A77AC, 0xEFEE7BFD, 0x1C443FA9, 0xBFEB7CE3, 0x9FFB3C9E,
    0x951F7DE3, 0x19F297FB, 0x5863FD61, 0xEF580DFF, 0xF7FB7F34, 0x7F53DFDC, 0xF9C63888,
    0x793D7FD6, 0x6B9C9BD6, 0x72A3EFBC, 0x41AC52FF, 0x7ED695DD, 0xDFB2A310, 0xE43B17E7,
    0x8DE71EAC, 0xBDDEF53E, 0xB9BE73DE, 0x8FFF0AD6, 0x000AE779, 0x546A18C0, 0x8E0017F9,
    0xB3FEA7CC, 0x31FFBFF2, 0xB4BD8D43, 0xA7A8E57D, 0x537B191E, 0x2B993CB3, 0xBF55B9F3,
    0xA73FFDAA, 0x639939CE, 0xCE6B1C8F, 0x4E66739A, 0x4F0CF991, 0x59F3A37A, 0xFEB987BF,
    0xF7AF3AC6, 0x9866AB1B, 0xC5FCDE71, 0x589F7CFB, 0x009F7273, 0xF2B9AD6A, 0xAF551FFF,
    0x5CCF9BDF, 0xC27FFE4E, 0x12FC67FC, 0x5E63FC00, 0x1F9E2C0F, 0xB07987BE, 0x7311998B,
    0x88A804CF, 0xFCC7EC7F, 0xBF7EFDFF, 0x60958F5C, 0x5FACCFFE, 0xF8D237DD, 0xEF7FFEA5,
    0xFD81EC53, 0x20BBED7F, 0xFFFB33BE, 0x1FF3A86F, 0x6E03FFA7, 0xBB39EBFF, 0x799E26C8,
    0xDC17DDAE, 0xFDFDCF6E, 0x9D7BFEA1, 0xBC72DBE7, 0xB3A9FAA7, 0xCC67EF97, 0xE7EB674F,
    0xDEE60005, 0xD955FDDB, 0xF9CAF3FF, 0xF7EFDFB5, 0xB14F5C38, 0xA7E77E07, 0x980774F6,
    0x80AB33CF, 0x1E7AD4F9, 0x4F9EC4E0, 0xEDBC07B1, 0xC227B4F7, 0xD94F7EF9, 0x8EFB8003,
    0xFEF00779, 0x8CFCEFCE, 0x1AE9EC5F, 0x77EF0003, 0x774F727E, 0x7FFFA800, 0x08F9DF9D,
    0xE79E77F7, 0xF7F7EFFF, 0x10BBED7F, 0xFBF1E77E, 0x500AFF7F, 0xBF3BFDFF, 0x35F7613B,
    0x87F225EF, 0x7F1BFBF7, 0xFFFFA007, 0x77D6F5CA, 0x8E675CEF, 0xF19FFDF5, 0xF1EDE3D7,
    0xE79BC622, 0x03F8D77D, 0x75AF79CB, 0x635DFF7F, 0x9EFD76FF, 0xF8D695DA, 0xDEE5FBD5,
    0xB19EB199, 0xCAAEEF23, 0xE7CEF55B, 0x03DBDFEB, 0xBBDBDFBF, 0xFAEDFEC6, 0xCD2BB53D,
    0x59E4F9E1, 0xEF59FFDE, 0x13DF339B, 0x1853B8EF, 0x60C5663B, 0x3C43776F, 0x16622D51,
    0x782B3C00, 0xC825BF3F, 0x540D6318, 0xF7F33F18, 0x06FBF58E

  EW_FONT_KERNING_CODES( DashboardFontS )
  EW_FONT_KERNING_VALUES( DashboardFontS )
EW_END_OF_FONT_RES( DashboardFontS )

/* Embedded Wizard */
