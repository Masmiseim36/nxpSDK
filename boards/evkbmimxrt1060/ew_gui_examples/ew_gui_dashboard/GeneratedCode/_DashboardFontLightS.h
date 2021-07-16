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
EW_DEFINE_FONT_RES( DashboardFontLightS, 14, 4, 0, 16, 0x0001, 0x0060 )
  EW_GLYPH( 0x0001, 1, -9, 6, 9, 8, 0x00000000 ),
  EW_GLYPH( 0x0020, 0, 0, 0, 0, 3, 0x00000084 ),                /* ' ' */
  EW_GLYPH( 0x0021, 1, -9, 2, 9, 3, 0x00000084 ),               /* '!' */
  EW_GLYPH( 0x0022, 0, -9, 4, 4, 4, 0x000000B5 ),               /* '"' */
  EW_GLYPH( 0x0023, 0, -9, 8, 9, 8, 0x000000EC ),               /* '#' */
  EW_GLYPH( 0x0024, 0, -10, 7, 12, 7, 0x000001FF ),             /* '$' */
  EW_GLYPH( 0x0025, 0, -9, 11, 10, 11, 0x00000325 ),            /* '%' */
  EW_GLYPH( 0x0026, 0, -9, 9, 10, 9, 0x000004A2 ),              /* '&' */
  EW_GLYPH( 0x0027, 0, -9, 2, 4, 3, 0x000005EF ),               /* ''' */
  EW_GLYPH( 0x0028, 0, -10, 4, 12, 4, 0x0000060C ),             /* '(' */
  EW_GLYPH( 0x0029, 0, -10, 4, 12, 4, 0x00000693 ),             /* ')' */
  EW_GLYPH( 0x002A, 0, -9, 5, 5, 6, 0x00000724 ),               /* '*' */
  EW_GLYPH( 0x002B, 0, -7, 6, 6, 7, 0x00000770 ),               /* '+' */
  EW_GLYPH( 0x002C, 0, -2, 2, 4, 3, 0x000007DE ),               /* ',' */
  EW_GLYPH( 0x002D, 0, -4, 5, 2, 6, 0x000007FC ),               /* '-' */
  EW_GLYPH( 0x002E, 0, -2, 2, 2, 3, 0x00000818 ),               /* '.' */
  EW_GLYPH( 0x002F, 0, -9, 6, 10, 6, 0x00000826 ),              /* '/' */
  EW_GLYPH( 0x0030, 0, -9, 7, 10, 7, 0x000008B4 ),              /* '0' */
  EW_GLYPH( 0x0031, 0, -9, 4, 9, 5, 0x000009A9 ),               /* '1' */
  EW_GLYPH( 0x0032, 0, -9, 7, 9, 7, 0x000009F5 ),               /* '2' */
  EW_GLYPH( 0x0033, 0, -9, 6, 10, 7, 0x00000ACC ),              /* '3' */
  EW_GLYPH( 0x0034, 0, -9, 7, 9, 8, 0x00000BC6 ),               /* '4' */
  EW_GLYPH( 0x0035, 0, -9, 6, 10, 7, 0x00000C8D ),              /* '5' */
  EW_GLYPH( 0x0036, 0, -9, 7, 10, 7, 0x00000D81 ),              /* '6' */
  EW_GLYPH( 0x0037, 0, -9, 6, 9, 6, 0x00000E94 ),               /* '7' */
  EW_GLYPH( 0x0038, 0, -9, 7, 10, 7, 0x00000F4E ),              /* '8' */
  EW_GLYPH( 0x0039, 0, -9, 6, 10, 7, 0x0000105B ),              /* '9' */
  EW_GLYPH( 0x003A, 0, -6, 2, 6, 3, 0x00001158 ),               /* ':' */
  EW_GLYPH( 0x003B, 0, -6, 2, 8, 3, 0x00001180 ),               /* ';' */
  EW_GLYPH( 0x003C, 0, -7, 6, 6, 6, 0x000011B8 ),               /* '<' */
  EW_GLYPH( 0x003D, 0, -6, 7, 5, 7, 0x00001244 ),               /* '=' */
  EW_GLYPH( 0x003E, 0, -7, 6, 6, 6, 0x000012DE ),               /* '>' */
  EW_GLYPH( 0x003F, 0, -9, 6, 9, 6, 0x00001371 ),               /* '?' */
  EW_GLYPH( 0x0040, 0, -7, 8, 8, 9, 0x0000141C ),               /* '@' */
  EW_GLYPH( 0x0041, 0, -9, 8, 9, 8, 0x00001543 ),               /* 'A' */
  EW_GLYPH( 0x0042, 1, -9, 6, 10, 8, 0x00001634 ),              /* 'B' */
  EW_GLYPH( 0x0043, 0, -9, 7, 10, 7, 0x00001735 ),              /* 'C' */
  EW_GLYPH( 0x0044, 1, -9, 7, 10, 8, 0x00001812 ),              /* 'D' */
  EW_GLYPH( 0x0045, 1, -9, 6, 9, 7, 0x000018F0 ),               /* 'E' */
  EW_GLYPH( 0x0046, 1, -9, 5, 9, 7, 0x000019B7 ),               /* 'F' */
  EW_GLYPH( 0x0047, 0, -9, 7, 10, 8, 0x00001A56 ),              /* 'G' */
  EW_GLYPH( 0x0048, 1, -9, 6, 9, 8, 0x00001B44 ),               /* 'H' */
  EW_GLYPH( 0x0049, 1, -9, 2, 9, 3, 0x00001BB8 ),               /* 'I' */
  EW_GLYPH( 0x004A, 0, -9, 4, 10, 4, 0x00001BD5 ),              /* 'J' */
  EW_GLYPH( 0x004B, 1, -9, 6, 9, 7, 0x00001C27 ),               /* 'K' */
  EW_GLYPH( 0x004C, 1, -9, 5, 9, 6, 0x00001CD1 ),               /* 'L' */
  EW_GLYPH( 0x004D, 0, -9, 10, 9, 11, 0x00001D2A ),             /* 'M' */
  EW_GLYPH( 0x004E, 1, -9, 7, 9, 9, 0x00001E46 ),               /* 'N' */
  EW_GLYPH( 0x004F, 0, -9, 8, 10, 8, 0x00001EF7 ),              /* 'O' */
  EW_GLYPH( 0x0050, 1, -9, 6, 9, 7, 0x00002000 ),               /* 'P' */
  EW_GLYPH( 0x0051, 0, -9, 8, 12, 8, 0x000020BA ),              /* 'Q' */
  EW_GLYPH( 0x0052, 1, -9, 6, 9, 8, 0x000021F1 ),               /* 'R' */
  EW_GLYPH( 0x0053, 0, -9, 7, 10, 7, 0x000022C8 ),              /* 'S' */
  EW_GLYPH( 0x0054, 0, -9, 7, 9, 7, 0x000023D5 ),               /* 'T' */
  EW_GLYPH( 0x0055, 1, -9, 6, 10, 8, 0x0000246C ),              /* 'U' */
  EW_GLYPH( 0x0056, 0, -9, 8, 9, 7, 0x00002502 ),               /* 'V' */
  EW_GLYPH( 0x0057, 0, -9, 11, 9, 12, 0x000025DB ),             /* 'W' */
  EW_GLYPH( 0x0058, 0, -9, 7, 9, 7, 0x00002755 ),               /* 'X' */
  EW_GLYPH( 0x0059, 0, -9, 7, 9, 7, 0x00002840 ),               /* 'Y' */
  EW_GLYPH( 0x005A, 0, -9, 7, 9, 7, 0x000028EC ),               /* 'Z' */
  EW_GLYPH( 0x005B, 1, -10, 3, 12, 4, 0x000029DE ),             /* '[' */
  EW_GLYPH( 0x005C, 0, -9, 6, 10, 6, 0x00002A32 ),              /* '\' */
  EW_GLYPH( 0x005D, 0, -10, 3, 12, 4, 0x00002AC4 ),             /* ']' */
  EW_GLYPH( 0x005E, 0, -8, 5, 4, 5, 0x00002B1B ),               /* '^' */
  EW_GLYPH( 0x005F, 0, 0, 6, 1, 5, 0x00002B63 ),                /* '_' */
  EW_GLYPH( 0x0060, 0, -9, 4, 3, 4, 0x00002B8A ),               /* '`' */
  EW_GLYPH( 0x0061, 0, -6, 6, 7, 7, 0x00002BB1 ),               /* 'a' */
  EW_GLYPH( 0x0062, 0, -9, 7, 10, 7, 0x00002C80 ),              /* 'b' */
  EW_GLYPH( 0x0063, 0, -6, 6, 7, 6, 0x00002D51 ),               /* 'c' */
  EW_GLYPH( 0x0064, 0, -9, 6, 10, 7, 0x00002DFB ),              /* 'd' */
  EW_GLYPH( 0x0065, 0, -6, 6, 7, 7, 0x00002EB8 ),               /* 'e' */
  EW_GLYPH( 0x0066, 0, -9, 5, 9, 5, 0x00002F82 ),               /* 'f' */
  EW_GLYPH( 0x0067, 0, -6, 7, 9, 7, 0x00003003 ),               /* 'g' */
  EW_GLYPH( 0x0068, 0, -9, 7, 9, 7, 0x00003115 ),               /* 'h' */
  EW_GLYPH( 0x0069, 0, -9, 2, 9, 3, 0x00003197 ),               /* 'i' */
  EW_GLYPH( 0x006A, 0, -9, 2, 12, 3, 0x000031C7 ),              /* 'j' */
  EW_GLYPH( 0x006B, 0, -9, 6, 9, 6, 0x0000320B ),               /* 'k' */
  EW_GLYPH( 0x006C, 1, -9, 3, 10, 4, 0x00003293 ),              /* 'l' */
  EW_GLYPH( 0x006D, 0, -6, 10, 6, 11, 0x000032D8 ),             /* 'm' */
  EW_GLYPH( 0x006E, 0, -6, 7, 6, 7, 0x00003378 ),               /* 'n' */
  EW_GLYPH( 0x006F, 0, -6, 7, 7, 7, 0x000033DE ),               /* 'o' */
  EW_GLYPH( 0x0070, 0, -6, 7, 9, 7, 0x0000349C ),               /* 'p' */
  EW_GLYPH( 0x0071, 0, -6, 6, 9, 7, 0x00003560 ),               /* 'q' */
  EW_GLYPH( 0x0072, 0, -6, 5, 6, 5, 0x0000361E ),               /* 'r' */
  EW_GLYPH( 0x0073, 0, -6, 6, 7, 6, 0x0000366B ),               /* 's' */
  EW_GLYPH( 0x0074, 0, -8, 5, 9, 5, 0x00003748 ),               /* 't' */
  EW_GLYPH( 0x0075, 0, -6, 6, 7, 7, 0x000037D4 ),               /* 'u' */
  EW_GLYPH( 0x0076, 0, -6, 7, 6, 6, 0x00003845 ),               /* 'v' */
  EW_GLYPH( 0x0077, 0, -6, 10, 6, 10, 0x000038E1 ),             /* 'w' */
  EW_GLYPH( 0x0078, 0, -6, 7, 6, 6, 0x000039D3 ),               /* 'x' */
  EW_GLYPH( 0x0079, 0, -6, 7, 9, 6, 0x00003A69 ),               /* 'y' */
  EW_GLYPH( 0x007A, 0, -6, 6, 6, 6, 0x00003B34 ),               /* 'z' */
  EW_GLYPH( 0x007B, 0, -10, 4, 12, 4, 0x00003BD8 ),             /* '{' */
  EW_GLYPH( 0x007C, 1, -10, 1, 12, 3, 0x00003C53 ),             /* '|' */
  EW_GLYPH( 0x007D, 0, -10, 4, 12, 4, 0x00003C66 ),             /* '}' */
  EW_GLYPH( 0x007E, 0, -5, 6, 3, 6, 0x00003CDD ),               /* '~' */

  EW_FONT_PIXEL( DashboardFontLightS, 0x00003D24 )              /* ratio 83.21 % */
    0xE2318C63, 0x8C775987, 0x00000051, 0xEB5AFF80, 0xC200DE55, 0x4643A11B, 0xE20AAFC4,
    0x31248557, 0xE787E33E, 0xF6FE9798, 0xAF47EEFB, 0x4F7F3BDF, 0xA563958C, 0x945FA9FC,
    0x6AFD6BB1, 0x1FCF7F91, 0xEF3C408C, 0x67FEE21A, 0xDFE9F23C, 0x63174C9A, 0xEEF89A9D,
    0xBFB11BDA, 0xBFE2020C, 0xFE22B3BF, 0xAAC5F7DD, 0x220678C0, 0xE93B3D18, 0x96719E60,
    0x1DE401F5, 0xECEF3798, 0xE31F7DEF, 0xBE73EF3C, 0xCAD43DEF, 0xD032915B, 0xECF030CD,
    0x782DBDF0, 0xFDEFC61E, 0xC0F3CF50, 0x750999D7, 0x84895AD6, 0xDFBF3E8B, 0xBF4AFC6F,
    0xAF8DF7DF, 0x10889E32, 0x3AC83C0C, 0x5EC67FBF, 0xD7FDFBBE, 0x3F192FFE, 0xA3E287E2,
    0x631E796F, 0xC1881332, 0x74958933, 0xC81EDE33, 0x10EFC52F, 0x119BD8B3, 0xD4B9C0C0,
    0xC4F17EB2, 0xC44FC242, 0xC4EA2B32, 0x88082AC3, 0xDF7EA00F, 0xF7EB7BF3, 0x8917BF3D,
    0xE396378A, 0x6A049E79, 0x14F01834, 0x0FE7859C, 0xF81E6267, 0xF607583F, 0x78C0BA83,
    0xF9E6EB0A, 0xB7D7FEF1, 0x99E08E46, 0x902D2131, 0x88619181, 0xDBF3DE5A, 0xF7FFAA6F,
    0xFF31185F, 0x24BFD667, 0x78C00000, 0xCCBF229E, 0x56BBE57E, 0xC603D827, 0x09EF22F3,
    0xF7C0F4E2, 0xEF7A9E15, 0x679EA1BD, 0x67799188, 0x0BDEB5AC, 0xF7EFD479, 0x5F7DF54D,
    0xAC14F113, 0xFF6FFFA8, 0xFFDFFEBA, 0x3F980614, 0x4F34FBF0, 0x9FDE0E65, 0xEA0999E0,
    0xF7CF3FFF, 0x3CFFFF9C, 0x6318C01F, 0xDF7DF384, 0x45BEFBC3, 0xBFFFFCC0, 0x6CFFDBF0,
    0xD65425F8, 0x5FBFB7FF, 0x3FFFBFFD, 0x5833CF19, 0x9EF17337, 0xF18C4AB5, 0x2FC7213C,
    0xEB7C66F7, 0x62330995, 0xF77A4F04, 0xFAC33FEF, 0x6316FF7F, 0xF7CF918C, 0xBEF5DD7D,
    0x3D445C6F, 0x8FE705A8, 0xC16B0A78, 0x3CF50FFF, 0xBFF277C1, 0x67AB7D43, 0xDFEF401A,
    0xAFEE862F, 0xA31508E7, 0xD629031E, 0xF2E7FDF9, 0xE0EF6F9E, 0xF3BD8A79, 0xFAEF58F6,
    0xCE44999E, 0xFEE63487, 0x5FFEF8FD, 0x7CB31107, 0xBEBDFDBF, 0x6A1FFFEF, 0xD1A8D8F4,
    0xA8D8F46A, 0x00396378, 0xBF3D3BC6, 0xDD3E6FDF, 0xFDFB9E9D, 0xC6FDF21E, 0x44222223,
    0xBC477777, 0x11BEFBEF, 0x6FBEFBEF, 0x07777744, 0xF107BF10, 0xCC15BF1B, 0xBC67FFDF,
    0xF7C5FBBF, 0xAD723DFB, 0xDE332255, 0x7CCF3C67, 0xFE0F5112, 0x98553C9D, 0xE15F001F,
    0xFFFFFFD9, 0xFF9DF8FF, 0x918E7FFF, 0x215F7BEB, 0x117DEFBE, 0x862BE6A3, 0xF5DF18FF,
    0xF1E2B32C, 0xF5FBEFDD, 0xF4F048C4, 0x2257A701, 0xAA0FD8E6, 0xB19C3F3C, 0xDCF14C9E,
    0x3D547ACC, 0x9CFEF3CF, 0xCF59118B, 0x3FDF84F3, 0x577877FF, 0xDF0882CF, 0x7F5397EF,
    0x79194C6F, 0xFBE190FF, 0xEFDCEFED, 0x7C8BFFFB, 0x7FDD629E, 0x5E9EF0EE, 0x0406391D,
    0x72018800, 0xFEF4E03C, 0xFFF8CBFD, 0xF3D57FBF, 0x9FEFC93C, 0x57793BFF, 0x8243069F,
    0x12300011, 0xFFFD8666, 0xF7BBAEFF, 0x18C60FFE, 0xEFBF7EC3, 0xEFBDF18B, 0xEFDF801B,
    0x7EFDF8B7, 0xFFCC000B, 0x630FFFFF, 0xEFDFB18C, 0x7DEFC6FB, 0xFDFBE05F, 0x8F7DFD1E,
    0xF900008C, 0xF7584F3C, 0xD3DE999F, 0x20348D6B, 0x8C41AA06, 0xF1472018, 0xFB7EFDE7,
    0xEBDBE752, 0x20EC6037, 0xFFD80036, 0xFFFF66FF, 0xC300001B, 0x8180000E, 0x0000001D,
    0xDCC4FFF3, 0x1DCC819F, 0x97EFC2E2, 0xFD7C98CC, 0xF189ADE1, 0x1EF112FB, 0xEC06B9A8,
    0x80000000, 0xFFFB9818, 0x3901C9FF, 0x9EE26738, 0x633FBF12, 0xDA935056, 0xBF1EA318,
    0x4EF610C7, 0x56788279, 0x1FE9E015, 0x43BBF037, 0xD8C818C4, 0xE795F927, 0x180ABE07,
    0x03D60BDF, 0x6FCC0B7C, 0xC60F3C40, 0xF3AC0CF3, 0x7BC59FFF, 0x8C6BEFBA, 0x00018E41,
    0xE4460310, 0x7A467878, 0xA97EEF7B, 0x2FFFFFFF, 0xF84F3CF5, 0x877FF3FD, 0xB82CF577,
    0xF52FE464, 0xFBE3D6F7, 0x30001AFD, 0x9D60679E, 0xDE2CFFFF, 0x695F7DD3, 0x00C46390,
    0x88C06200, 0x48CF0F1C, 0x2FDDEF6F, 0xBFEF7FF5, 0x427FEE20, 0x79EA5BED, 0xE7FBF49E,
    0xEAEF0F7F, 0x7ACB83F9, 0xF273F7E6, 0xFAEF0CFF, 0x705A80CD, 0x9E79F2FF, 0xF33FF621,
    0x35AF4F51, 0x3CC605CD, 0xEF77C4DF, 0x1BE788DE, 0xEFFFC801, 0xFFE46677, 0xC72BCEFB,
    0xF3E46318, 0x9EFBFB7D, 0x77DEBEF5, 0x00000001, 0x6C603000, 0x000000D8, 0xAAC55082,
    0x3D7FBDEE, 0x7FFFFFFE, 0x4DF0C064, 0x43F7EB8C, 0xEFC11195, 0x83112D47, 0xD42ADFFF,
    0xB550FFB4, 0xC857F60A, 0x6A8C48C0, 0xEE32FB9E, 0x277FF0F3, 0xB50633CF, 0x18D7DA66,
    0x1EB2C732, 0xFF1E33CF, 0xF7B8631D, 0x3CF3D619, 0x32DDCF3C, 0x3D6460C6, 0x122FEFAC,
    0xEFC5C631, 0x398DA7C9, 0x4BF44FBD, 0x60F5FF7C, 0xE63C917F, 0x66DBC7EF, 0xDD07398D,
    0x8D591023, 0x6A955CFF, 0x374FDF95, 0xF81AE6B3, 0x018AC375, 0x8C631000, 0x7DF7E431,
    0xFBC63FEF, 0xEA0663BE, 0xE17BC469, 0x5CD40DF7, 0x8D40F788, 0x47FFFFFB, 0x8CBF388E,
    0x00000077, 0x8824F7C4, 0xEF099186, 0xE848C60F, 0xE848E41F, 0x7E031C86, 0xF7F22315,
    0x00006F1F, 0x24DF3C00, 0xFF14EF51, 0xD6BE645F, 0xBF7EFDFC, 0x4EE1665F, 0xFDEC1BF9,
    0xFF7B0FFF, 0xBEF9AF7F, 0xAFFFA8FF, 0xAE3222FF, 0x15DFBF5A, 0x7F37EFFF, 0x101F101E,
    0x4CFB98C0, 0x19BBD8C6, 0x0C011C8F, 0xF7E29188, 0xDFD8AFDB, 0xFFAC5FF7, 0xDDF237FF,
    0x4F58DFFF, 0xE40F5800, 0xE57EFFFA, 0x02BF7FFD, 0xD602F105, 0xF7E4FF7F, 0x4939AFDF,
    0x723C9CC0, 0xBE9EDFED, 0xD67FAFFF, 0x7E47BFFF, 0x22BDDFFB, 0xEFBE20C7, 0x3DBEF2F6,
    0x7EFFF9F2, 0x6FBFFF4B, 0xF2B4EFD1, 0x5C8EE72B, 0xAD63FFAB, 0x00000FFE, 0xFEDFEEF0,
    0xD7FB918D, 0x41A56231, 0x1FDDFECE, 0x988776EB, 0xDCCFEDF9, 0xC62BDDEB, 0xEA2BD6B7,
    0x03CDEE64, 0xF10003E2, 0x912FFEFC, 0x160CFFBF, 0x4F000000, 0x4BF8E2FC, 0xE39F8F00,
    0x53C4012F, 0x7C40F3EB, 0x7857C000, 0x8D97C4B3, 0x7BE4499E, 0xF87E9EA0, 0xFEA00000,
    0xF13F6FCF, 0xD679DFCD, 0xDF8F0FFE, 0x1F7F63D7, 0x01259AD3, 0xF1000000, 0x97FF7E7B,
    0x067FDFCA, 0x8000000B, 0x92EFFFF9, 0x33F7FFBD, 0x0446309F, 0x223126B0, 0xCDDBFFEB,
    0x14FFBFBB, 0xF167DCDF, 0xA60DF7D8, 0x9E001278, 0x7FDFBF10, 0xFB7DF8A5, 0x03E20885,
    0xDFBFFFE6, 0xBFFFEDFC, 0x98040CE6, 0xFB98C793, 0xFFF7E7F7, 0x6F100235, 0x9E5ECF7C,
    0x00CC5FF1, 0xFFFD8800, 0xFF3D46FF, 0x5ACC6FFF, 0xFFF9D8C5, 0x5FBCF10F, 0xFBF7EFE5,
    0xF7DF7F2A, 0x3D819E1F, 0x63FFAB59, 0x000FFEAD, 0x0FFDD818, 0x3F11DDCF, 0x026267E2,
    0xBD889400, 0x5EFFEC33, 0xFDB127BB, 0x98C7ACFE, 0x3DF1FFE9, 0x37F306B1, 0x4DF9887F,
    0xB1F7E4D6, 0x98CBF8E6, 0xF5A6AAFC, 0xAB488AAF, 0x7AC7FD76, 0xB519FBB8, 0x967A8FF7,
    0x8FA86B58, 0xFA2795A7, 0x2FDFD7E5, 0xFF5F0110, 0x4546617E, 0xDB127A7F, 0x8C7ACFEF,
    0xEF8FBE99, 0x7BF98349, 0x4EF5887E, 0xBA7815B8, 0xFF513FC1, 0xEB3FFFFF, 0xC451BFFF,
    0x0EFFC9B9, 0x79E0EF7E, 0x8C9FFFFA, 0xBF61DEB0, 0x32563009, 0xC4009119, 0x0BA8F4E2,
    0x3EF92440, 0xCC00D9C6, 0x21DA810C, 0x457E7800, 0xFAFBEF80, 0xDFBF7DF1, 0x00000003

  EW_FONT_KERNING_CODES( DashboardFontLightS )
  EW_FONT_KERNING_VALUES( DashboardFontLightS )
EW_END_OF_FONT_RES( DashboardFontLightS )

/* Embedded Wizard */
