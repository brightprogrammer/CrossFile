/**
 * @file Common.h
 * @date Mon, 27th May 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2024 Siddharth Mishra
 * @copyright Copyright 2024 Anvie Labs
 *
 * Copyright 2024 Siddharth Mishra, Anvie Labs
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * */

/*
 * This file exists here to define common types, enums, defines, etc... that are shared
 * by all tables and is defined in the spec.
 * */

#ifndef ANVIE_CROSSFILE_OTF_TABLES_COMMON_H
#define ANVIE_CROSSFILE_OTF_TABLES_COMMON_H

#include <Anvie/Types.h>

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#platform-ids
 * */
typedef enum XfOtfPlatform : Uint16 {
    XF_OTF_PLATFORM_MIN = 0,

    XF_OTF_PLATFORM_VARIOUS = 0,
    XF_OTF_PLATFORM_MAC     = 1,
    XF_OTF_PLATFORM_ISO     = 2,
    XF_OTF_PLATFORM_WIN     = 3,
    XF_OTF_PLATFORM_CUSTOM  = 4,

    XF_OTF_PLATFORM_MAX = 4
} XfOtfPlatform;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#unicode-platform-platform-id--0 
 * 
 * - Use of encoding IDs 0, 1 or 2 is deprecated.
 * - Encoding ID 3 should be used in conjunction with 'cmap' subtable formats 4 or 6.
 * - Encoding ID 4 should be used in conjunction with subtable formats 10 or 12.
 * 
 * - Unicode Variation Sequences supported by the font should be specified in the 'cmap'
 *   table using a format 14 subtable.
 *
 * - A format 14 subtable must only be used under platform ID 0 and encoding ID 5; 
 * - Encoding ID 5 should only be used with a format 14 subtable.
 * 
 * - Encoding ID 6 should only be used in conjunction with 'cmap' subtable format 13;
 * - Subtable format 13 should only be used under platform ID 0 and encoding ID 6.
 * */
typedef enum XfOtfVariousEncoding : Uint16 {
    XF_OTF_VARIOUS_ENCODING_MIN = 0,

    XF_OTF_VARIOUS_ENCODING_UNICODE_10                 = 0,
    XF_OTF_VARIOUS_ENCODING_UNICODE_11                 = 1,
    XF_OTF_VARIOUS_ENCODING_ISO_IEC_10646              = 2,
    XF_OTF_VARIOUS_ENCODING_UNICODE_20_BMP_ONLY        = 3,
    XF_OTF_VARIOUS_ENCODING_UNICODE_20_FULL_REPERTOIRE = 4,
    XF_OTF_VARIOUS_ENCODING_UNICODE_VARIATION_SEQ      = 5,
    XF_OTF_VARIOUS_ENCODING_UNICODE_FULL_REPERTOIRE    = 6,

    XF_OTF_VARIOUS_ENCODING_MAX = 6
} XfOtfVariousEncoding;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/name#macintosh-encoding-ids-script-manager-codes
 * */
typedef enum XfOtfMacEncoding : Uint16 {
    XF_OTF_MAC_ENCODING_MIN = 0,

    XF_OTF_MAC_ENCODING_ROMAN               = 0,
    XF_OTF_MAC_ENCODING_JAPANESE            = 1,
    XF_OTF_MAC_ENCODING_CHINESE_TRADITIONAL = 2,
    XF_OTF_MAC_ENCODING_KOREAN              = 3,
    XF_OTF_MAC_ENCODING_ARABIC              = 4,
    XF_OTF_MAC_ENCODING_HEBREW              = 5,
    XF_OTF_MAC_ENCODING_GREEK               = 6,
    XF_OTF_MAC_ENCODING_RUSSIAN             = 7,
    XF_OTF_MAC_ENCODING_RSYMBOL             = 8,
    XF_OTF_MAC_ENCODING_DEVANAGARI          = 9,
    XF_OTF_MAC_ENCODING_GURMUKHI            = 10,
    XF_OTF_MAC_ENCODING_GUJARATI            = 11,
    XF_OTF_MAC_ENCODING_ODIA                = 12,
    XF_OTF_MAC_ENCODING_BANGLA              = 13,
    XF_OTF_MAC_ENCODING_TAMIL               = 14,
    XF_OTF_MAC_ENCODING_TELUGU              = 15,
    XF_OTF_MAC_ENCODING_KANNADA             = 16,
    XF_OTF_MAC_ENCODING_MALAYALAM           = 17,
    XF_OTF_MAC_ENCODING_SINHALESE           = 18,
    XF_OTF_MAC_ENCODING_BURMESE             = 19,
    XF_OTF_MAC_ENCODING_KHMER               = 20,
    XF_OTF_MAC_ENCODING_THAI                = 21,
    XF_OTF_MAC_ENCODING_LAOTIAN             = 22,
    XF_OTF_MAC_ENCODING_GEORGIAN            = 23,
    XF_OTF_MAC_ENCODING_ARMENIAN            = 24,
    XF_OTF_MAC_ENCODING_CHINESE_SIMPLIFIED  = 25,
    XF_OTF_MAC_ENCODING_TIBETAN             = 26,
    XF_OTF_MAC_ENCODING_MONGOLIAN           = 27,
    XF_OTF_MAC_ENCODING_GEEZ                = 28,
    XF_OTF_MAC_ENCODING_SLAVIC              = 29,
    XF_OTF_MAC_ENCODING_VIETNAMESE          = 30,
    XF_OTF_MAC_ENCODING_SINDHI              = 31,
    XF_OTF_MAC_ENCODING_UNINTERPRETED       = 32,

    XF_OTF_MAC_ENCODING_MAX = 32
} XfOtfMacEncoding;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#iso-platform-platform-id--2
 * */
typedef enum XfOtfIsoEncoding : Uint16 {
    XF_OTF_ISO_ENCODING_MIN = 0,

    XF_OTF_ISO_ENCODING_7_BIT_ASCII = 0,
    XF_OTF_ISO_ENCODING_ISO_10646   = 1,
    XF_OTF_ISO_ENCODING_ISO_8859_1  = 2,

    XF_OTF_ISO_ENCODING_MAX = 2,
} XfOtfIsoEncoding;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#windows-platform-platform-id--3
 * */
typedef enum XfOtfWinEncoding : Uint16 {
    XF_OTF_WIN_ENCODING_MIN = 0,

    XF_OTF_WIN_ENCODING_SYMBOL                  = 0,
    XF_OTF_WIN_ENCODING_UNICODE_BMP             = 1,
    XF_OTF_WIN_ENCODING_SHIFTJIS                = 2,
    XF_OTF_WIN_ENCODING_PRC                     = 3,
    XF_OTF_WIN_ENCODING_BIG5                    = 4,
    XF_OTF_WIN_ENCODING_WANSUNG                 = 5,
    XF_OTF_WIN_ENCODING_JOHAB                   = 6,
    XF_OTF_WIN_ENCODING_RESERVED_7              = 7,
    XF_OTF_WIN_ENCODING_RESERVED_8              = 8,
    XF_OTF_WIN_ENCODING_RESERVED_9              = 9,
    XF_OTF_WIN_ENCODING_UNICODE_FULL_REPERTOIRE = 10,

    XF_OTF_WIN_ENCODING_MAX = 10,
} XfOtfWinEncoding;

#define XF_OTF_CUSTOM_ENCODING_MIN 0
#define XF_OTF_CUSTOM_ENCODING_MAX 255

typedef struct XfOtfPlatformEncoding {
    XfOtfPlatform platform;
    union {
        XfOtfVariousEncoding various;
        XfOtfMacEncoding     mac;
        XfOtfIsoEncoding     iso;
        XfOtfWinEncoding     win;
        Uint16               custom;
    } encoding;
} XfOtfPlatformEncoding;

#define XF_OTF_PLATFORM_ENCODING_DATA_SIZE (sizeof (XfOtfPlatform) + sizeof (Uint16))

CString xf_otf_platform_encoding_get_platform_str (XfOtfPlatformEncoding platform_encoding);
CString xf_otf_platform_encoding_get_encoding_str (XfOtfPlatformEncoding platform_encoding);

typedef enum XfOtfMacLanguage : Uint16 {
    XF_OTF_MAC_LANGUAGE_ENGLISH              = 0,
    XF_OTF_MAC_LANGUAGE_FRENCH               = 1,
    XF_OTF_MAC_LANGUAGE_GERMAN               = 2,
    XF_OTF_MAC_LANGUAGE_ITALIAN              = 3,
    XF_OTF_MAC_LANGUAGE_DUTCH                = 4,
    XF_OTF_MAC_LANGUAGE_SWEDISH              = 5,
    XF_OTF_MAC_LANGUAGE_SPANISH              = 6,
    XF_OTF_MAC_LANGUAGE_DANISH               = 7,
    XF_OTF_MAC_LANGUAGE_PORTUGUESE           = 8,
    XF_OTF_MAC_LANGUAGE_NORWEGIAN            = 9,
    XF_OTF_MAC_LANGUAGE_HEBREW               = 10,
    XF_OTF_MAC_LANGUAGE_JAPANESE             = 11,
    XF_OTF_MAC_LANGUAGE_ARABIC               = 12,
    XF_OTF_MAC_LANGUAGE_FINNISH              = 13,
    XF_OTF_MAC_LANGUAGE_GREEK                = 14,
    XF_OTF_MAC_LANGUAGE_ICELANDIC            = 15,
    XF_OTF_MAC_LANGUAGE_MALTESE              = 16,
    XF_OTF_MAC_LANGUAGE_TURKISH              = 17,
    XF_OTF_MAC_LANGUAGE_CROATIAN             = 18,
    XF_OTF_MAC_LANGUAGE_CHINESE_TRADITIONAL  = 19,
    XF_OTF_MAC_LANGUAGE_URDU                 = 20,
    XF_OTF_MAC_LANGUAGE_HINDI                = 21,
    XF_OTF_MAC_LANGUAGE_THAI                 = 22,
    XF_OTF_MAC_LANGUAGE_KOREAN               = 23,
    XF_OTF_MAC_LANGUAGE_LITHUANIAN           = 24,
    XF_OTF_MAC_LANGUAGE_POLISH               = 25,
    XF_OTF_MAC_LANGUAGE_HUNGARIAN            = 26,
    XF_OTF_MAC_LANGUAGE_ESTONIAN             = 27,
    XF_OTF_MAC_LANGUAGE_LATVIAN              = 28,
    XF_OTF_MAC_LANGUAGE_SAMI                 = 29,
    XF_OTF_MAC_LANGUAGE_FAROESE              = 30,
    XF_OTF_MAC_LANGUAGE_FARSI_PERSIAN        = 31,
    XF_OTF_MAC_LANGUAGE_RUSSIAN              = 32,
    XF_OTF_MAC_LANGUAGE_CHINESE_SIMPLIFIED   = 33,
    XF_OTF_MAC_LANGUAGE_FLEMISH              = 34,
    XF_OTF_MAC_LANGUAGE_IRISH                = 35,
    XF_OTF_MAC_LANGUAGE_ALBANIAN             = 36,
    XF_OTF_MAC_LANGUAGE_ROMANIAN             = 37,
    XF_OTF_MAC_LANGUAGE_CZECH                = 38,
    XF_OTF_MAC_LANGUAGE_SLOVAK               = 39,
    XF_OTF_MAC_LANGUAGE_SLOVENIAN            = 40,
    XF_OTF_MAC_LANGUAGE_YIDDISH              = 41,
    XF_OTF_MAC_LANGUAGE_SERBIAN              = 42,
    XF_OTF_MAC_LANGUAGE_MACEDONIAN           = 43,
    XF_OTF_MAC_LANGUAGE_BULGARIAN            = 44,
    XF_OTF_MAC_LANGUAGE_UKRAINIAN            = 45,
    XF_OTF_MAC_LANGUAGE_BYELORUSSIAN         = 46,
    XF_OTF_MAC_LANGUAGE_UZBEK                = 47,
    XF_OTF_MAC_LANGUAGE_KAZAKH               = 48,
    XF_OTF_MAC_LANGUAGE_AZERBAIJANI_CYRILLIC = 49,
    XF_OTF_MAC_LANGUAGE_AZERBAIJANI_ARABIC   = 50,
    XF_OTF_MAC_LANGUAGE_ARMENIAN             = 51,
    XF_OTF_MAC_LANGUAGE_GEORGIAN             = 52,
    XF_OTF_MAC_LANGUAGE_ROMANIAN_MOLDOVA     = 53,
    XF_OTF_MAC_LANGUAGE_KIRGHIZ              = 54,
    XF_OTF_MAC_LANGUAGE_TAJIKI               = 55,
    XF_OTF_MAC_LANGUAGE_TURKMEN              = 56,
    XF_OTF_MAC_LANGUAGE_MONGOLIAN_SCRIPT     = 57,
    XF_OTF_MAC_LANGUAGE_MONGOLIAN_CYRILLIC   = 58,
    XF_OTF_MAC_LANGUAGE_PASHTO               = 59,
    XF_OTF_MAC_LANGUAGE_KURDISH              = 60,
    XF_OTF_MAC_LANGUAGE_KASHMIRI             = 61,
    XF_OTF_MAC_LANGUAGE_SINDHI               = 62,
    XF_OTF_MAC_LANGUAGE_TIBETAN              = 63,
    XF_OTF_MAC_LANGUAGE_NEPALI               = 64,
    XF_OTF_MAC_LANGUAGE_SANSKRIT             = 65,
    XF_OTF_MAC_LANGUAGE_MARATHI              = 66,
    XF_OTF_MAC_LANGUAGE_BANGLA               = 67,
    XF_OTF_MAC_LANGUAGE_ASSAMESE             = 68,
    XF_OTF_MAC_LANGUAGE_GUJARATI             = 69,
    XF_OTF_MAC_LANGUAGE_PUNJABI              = 70,
    XF_OTF_MAC_LANGUAGE_ODIA                 = 71,
    XF_OTF_MAC_LANGUAGE_MALAYALAM            = 72,
    XF_OTF_MAC_LANGUAGE_KANNADA              = 73,
    XF_OTF_MAC_LANGUAGE_TAMIL                = 74,
    XF_OTF_MAC_LANGUAGE_TELUGU               = 75,
    XF_OTF_MAC_LANGUAGE_SINHALESE            = 76,
    XF_OTF_MAC_LANGUAGE_BURMESE              = 77,
    XF_OTF_MAC_LANGUAGE_KHMER                = 78,
    XF_OTF_MAC_LANGUAGE_LAO                  = 79,
    XF_OTF_MAC_LANGUAGE_VIETNAMESE           = 80,
    XF_OTF_MAC_LANGUAGE_INDONESIAN           = 81,
    XF_OTF_MAC_LANGUAGE_TAGALOG              = 82,
    XF_OTF_MAC_LANGUAGE_MALAY_ROMAN          = 83,
    XF_OTF_MAC_LANGUAGE_MALAY_ARABIC         = 84,
    XF_OTF_MAC_LANGUAGE_AMHARIC              = 85,
    XF_OTF_MAC_LANGUAGE_TIGRINYA             = 86,
    XF_OTF_MAC_LANGUAGE_GALLA                = 87,
    XF_OTF_MAC_LANGUAGE_SOMALI               = 88,
    XF_OTF_MAC_LANGUAGE_SWAHILI              = 89,
    XF_OTF_MAC_LANGUAGE_KINYARWANDA_RUANDA   = 90,
    XF_OTF_MAC_LANGUAGE_RUNDI                = 91,
    XF_OTF_MAC_LANGUAGE_NYANJA_CHEWA         = 92,
    XF_OTF_MAC_LANGUAGE_MALAGASY             = 93,
    XF_OTF_MAC_LANGUAGE_ESPERANTO            = 94,
    XF_OTF_MAC_LANGUAGE_WELSH                = 128,
    XF_OTF_MAC_LANGUAGE_BASQUE               = 129,
    XF_OTF_MAC_LANGUAGE_CATALAN              = 130,
    XF_OTF_MAC_LANGUAGE_LATIN                = 131,
    XF_OTF_MAC_LANGUAGE_QUECHUA              = 132,
    XF_OTF_MAC_LANGUAGE_GUARANI              = 133,
    XF_OTF_MAC_LANGUAGE_AYMARA               = 134,
    XF_OTF_MAC_LANGUAGE_TATAR                = 135,
    XF_OTF_MAC_LANGUAGE_UIGHUR               = 136,
    XF_OTF_MAC_LANGUAGE_DZONGKHA             = 137,
    XF_OTF_MAC_LANGUAGE_JAVANESE_ROMAN       = 138,
    XF_OTF_MAC_LANGUAGE_SUNDANESE_ROMAN      = 139,
    XF_OTF_MAC_LANGUAGE_GALICIAN             = 140,
    XF_OTF_MAC_LANGUAGE_AFRIKAANS            = 141,
    XF_OTF_MAC_LANGUAGE_BRETON               = 142,
    XF_OTF_MAC_LANGUAGE_INUKTITUT            = 143,
    XF_OTF_MAC_LANGUAGE_SCOTTISH_GAELIC      = 144,
    XF_OTF_MAC_LANGUAGE_MANX                 = 145,
    XF_OTF_MAC_LANGUAGE_IRISH_DOT            = 146,
    XF_OTF_MAC_LANGUAGE_TONGAN               = 147,
    XF_OTF_MAC_LANGUAGE_GREEK_POLYTONIC      = 148,
    XF_OTF_MAC_LANGUAGE_GREENLANDIC          = 149,
    XF_OTF_MAC_LANGUAGE_AZERBAIJANI_ROMAN    = 150,
    XF_OTF_MAC_LANGUAGE_MAX                  = 150
} XfOtfMacLanguage;

typedef enum XfOtfWinLanguage : Uint16 {
    XF_OTF_WIN_LANGUAGE_AFRIKAANS                  = 0x0436,
    XF_OTF_WIN_LANGUAGE_ALBANIAN                   = 0x041C,
    XF_OTF_WIN_LANGUAGE_ALSATIAN                   = 0x0484,
    XF_OTF_WIN_LANGUAGE_AMHARIC                    = 0x045E,
    XF_OTF_WIN_LANGUAGE_ARABIC_ALGERIA             = 0x1401,
    XF_OTF_WIN_LANGUAGE_ARABIC_BAHRAIN             = 0x3C01,
    XF_OTF_WIN_LANGUAGE_ARABIC_EGYPT               = 0x0C01,
    XF_OTF_WIN_LANGUAGE_ARABIC_IRAQ                = 0x0801,
    XF_OTF_WIN_LANGUAGE_ARABIC_JORDAN              = 0x2C01,
    XF_OTF_WIN_LANGUAGE_ARABIC_KUWAIT              = 0x3401,
    XF_OTF_WIN_LANGUAGE_ARABIC_LEBANON             = 0x3001,
    XF_OTF_WIN_LANGUAGE_ARABIC_LIBYA               = 0x1001,
    XF_OTF_WIN_LANGUAGE_ARABIC_MOROCCO             = 0x1801,
    XF_OTF_WIN_LANGUAGE_ARABIC_OMAN                = 0x2001,
    XF_OTF_WIN_LANGUAGE_ARABIC_QATAR               = 0x4001,
    XF_OTF_WIN_LANGUAGE_ARABIC_SAUDI_ARABIA        = 0x0401,
    XF_OTF_WIN_LANGUAGE_ARABIC_SYRIA               = 0x2801,
    XF_OTF_WIN_LANGUAGE_ARABIC_TUNISIA             = 0x1C01,
    XF_OTF_WIN_LANGUAGE_ARABIC_UAE                 = 0x3801,
    XF_OTF_WIN_LANGUAGE_ARABIC_YEMEN               = 0x2401,
    XF_OTF_WIN_LANGUAGE_ARMENIAN                   = 0x042B,
    XF_OTF_WIN_LANGUAGE_ASSAMESE                   = 0x044D,
    XF_OTF_WIN_LANGUAGE_AZERBAIJANI_CYRILLIC       = 0x082C,
    XF_OTF_WIN_LANGUAGE_AZERBAIJANI_LATIN          = 0x042C,
    XF_OTF_WIN_LANGUAGE_BASHKIR                    = 0x046D,
    XF_OTF_WIN_LANGUAGE_BASQUE                     = 0x042D,
    XF_OTF_WIN_LANGUAGE_BELARUSIAN                 = 0x0423,
    XF_OTF_WIN_LANGUAGE_BANGLA_BANGLADESH          = 0x0845,
    XF_OTF_WIN_LANGUAGE_BANGLA_INDIA               = 0x0445,
    XF_OTF_WIN_LANGUAGE_BOSNIAN_CYRILLIC           = 0x201A,
    XF_OTF_WIN_LANGUAGE_BOSNIAN_LATIN              = 0x141A,
    XF_OTF_WIN_LANGUAGE_BRETON                     = 0x047E,
    XF_OTF_WIN_LANGUAGE_BULGARIAN                  = 0x0402,
    XF_OTF_WIN_LANGUAGE_CATALAN                    = 0x0403,
    XF_OTF_WIN_LANGUAGE_CHINESE_HONG_KONG          = 0x0C04,
    XF_OTF_WIN_LANGUAGE_CHINESE_MACAO              = 0x1404,
    XF_OTF_WIN_LANGUAGE_CHINESE_PRCHINA            = 0x0804,
    XF_OTF_WIN_LANGUAGE_CHINESE_SINGAPORE          = 0x1004,
    XF_OTF_WIN_LANGUAGE_CHINESE_TAIWAN             = 0x0404,
    XF_OTF_WIN_LANGUAGE_CORSICAN                   = 0x0483,
    XF_OTF_WIN_LANGUAGE_CROATIAN                   = 0x041A,
    XF_OTF_WIN_LANGUAGE_CROATIAN_LATIN             = 0x101A,
    XF_OTF_WIN_LANGUAGE_CZECH                      = 0x0405,
    XF_OTF_WIN_LANGUAGE_DANISH                     = 0x0406,
    XF_OTF_WIN_LANGUAGE_DARI                       = 0x048C,
    XF_OTF_WIN_LANGUAGE_DIVEHI                     = 0x0465,
    XF_OTF_WIN_LANGUAGE_DUTCH_BELGIUM              = 0x0813,
    XF_OTF_WIN_LANGUAGE_DUTCH_NETHERLANDS          = 0x0413,
    XF_OTF_WIN_LANGUAGE_ENGLISH_AUSTRALIA          = 0x0C09,
    XF_OTF_WIN_LANGUAGE_ENGLISH_BELIZE             = 0x2809,
    XF_OTF_WIN_LANGUAGE_ENGLISH_CANADA             = 0x1009,
    XF_OTF_WIN_LANGUAGE_ENGLISH_CARIBBEAN          = 0x2409,
    XF_OTF_WIN_LANGUAGE_ENGLISH_INDIA              = 0x4009,
    XF_OTF_WIN_LANGUAGE_ENGLISH_IRELAND            = 0x1809,
    XF_OTF_WIN_LANGUAGE_ENGLISH_JAMAICA            = 0x2009,
    XF_OTF_WIN_LANGUAGE_ENGLISH_MALAYSIA           = 0x4409,
    XF_OTF_WIN_LANGUAGE_ENGLISH_NEW_ZEALAND        = 0x1409,
    XF_OTF_WIN_LANGUAGE_ENGLISH_PHILIPPINES        = 0x3409,
    XF_OTF_WIN_LANGUAGE_ENGLISH_SINGAPORE          = 0x4809,
    XF_OTF_WIN_LANGUAGE_ENGLISH_SOUTH_AFRICA       = 0x1C09,
    XF_OTF_WIN_LANGUAGE_ENGLISH_TRINIDAD_TOBAGO    = 0x2C09,
    XF_OTF_WIN_LANGUAGE_ENGLISH_UK                 = 0x0809,
    XF_OTF_WIN_LANGUAGE_ENGLISH_US                 = 0x0409,
    XF_OTF_WIN_LANGUAGE_ENGLISH_ZIMBABWE           = 0x3009,
    XF_OTF_WIN_LANGUAGE_ESTONIAN                   = 0x0425,
    XF_OTF_WIN_LANGUAGE_FAROESE                    = 0x0438,
    XF_OTF_WIN_LANGUAGE_FILIPINO                   = 0x0464,
    XF_OTF_WIN_LANGUAGE_FINNISH                    = 0x040B,
    XF_OTF_WIN_LANGUAGE_FRENCH_BELGIUM             = 0x080C,
    XF_OTF_WIN_LANGUAGE_FRENCH_CANADA              = 0x0C0C,
    XF_OTF_WIN_LANGUAGE_FRENCH_FRANCE              = 0x040C,
    XF_OTF_WIN_LANGUAGE_FRENCH_LUXEMBOURG          = 0x140C,
    XF_OTF_WIN_LANGUAGE_FRENCH_MONACO              = 0x180C,
    XF_OTF_WIN_LANGUAGE_FRENCH_SWITZERLAND         = 0x100C,
    XF_OTF_WIN_LANGUAGE_FRISIAN                    = 0x0462,
    XF_OTF_WIN_LANGUAGE_GALICIAN                   = 0x0456,
    XF_OTF_WIN_LANGUAGE_GEORGIAN                   = 0x0437,
    XF_OTF_WIN_LANGUAGE_GERMAN_AUSTRIA             = 0x0C07,
    XF_OTF_WIN_LANGUAGE_GERMAN_GERMANY             = 0x0407,
    XF_OTF_WIN_LANGUAGE_GERMAN_LIECHTENSTEIN       = 0x1407,
    XF_OTF_WIN_LANGUAGE_GERMAN_LUXEMBOURG          = 0x1007,
    XF_OTF_WIN_LANGUAGE_GERMAN_SWITZERLAND         = 0x0807,
    XF_OTF_WIN_LANGUAGE_GREEK                      = 0x0408,
    XF_OTF_WIN_LANGUAGE_GREENLANDIC                = 0x046F,
    XF_OTF_WIN_LANGUAGE_GUJARATI                   = 0x0447,
    XF_OTF_WIN_LANGUAGE_HAUSA_LATIN                = 0x0468,
    XF_OTF_WIN_LANGUAGE_HEBREW                     = 0x040D,
    XF_OTF_WIN_LANGUAGE_HINDI                      = 0x0439,
    XF_OTF_WIN_LANGUAGE_HUNGARIAN                  = 0x040E,
    XF_OTF_WIN_LANGUAGE_ICELANDIC                  = 0x040F,
    XF_OTF_WIN_LANGUAGE_IGBO                       = 0x0470,
    XF_OTF_WIN_LANGUAGE_INDONESIAN                 = 0x0421,
    XF_OTF_WIN_LANGUAGE_INUKTITUT                  = 0x045D,
    XF_OTF_WIN_LANGUAGE_INUKTITUT_LATIN            = 0x085D,
    XF_OTF_WIN_LANGUAGE_IRISH                      = 0x083C,
    XF_OTF_WIN_LANGUAGE_ISIXHOSA                   = 0x0434,
    XF_OTF_WIN_LANGUAGE_ISIZULU                    = 0x0435,
    XF_OTF_WIN_LANGUAGE_ITALIAN_ITALY              = 0x0410,
    XF_OTF_WIN_LANGUAGE_ITALIAN_SWITZERLAND        = 0x0810,
    XF_OTF_WIN_LANGUAGE_JAPANESE                   = 0x0411,
    XF_OTF_WIN_LANGUAGE_KANNADA                    = 0x044B,
    XF_OTF_WIN_LANGUAGE_KAZAKH                     = 0x043F,
    XF_OTF_WIN_LANGUAGE_KHMER                      = 0x0453,
    XF_OTF_WIN_LANGUAGE_KICHE                      = 0x0486,
    XF_OTF_WIN_LANGUAGE_KINYARWANDA                = 0x0487,
    XF_OTF_WIN_LANGUAGE_KISWAHILI                  = 0x0441,
    XF_OTF_WIN_LANGUAGE_KONKANI                    = 0x0457,
    XF_OTF_WIN_LANGUAGE_KOREAN                     = 0x0412,
    XF_OTF_WIN_LANGUAGE_KYRGYZ                     = 0x0440,
    XF_OTF_WIN_LANGUAGE_LAO                        = 0x0454,
    XF_OTF_WIN_LANGUAGE_LATVIAN                    = 0x0426,
    XF_OTF_WIN_LANGUAGE_LITHUANIAN                 = 0x0427,
    XF_OTF_WIN_LANGUAGE_LOWER_SORBIAN              = 0x082E,
    XF_OTF_WIN_LANGUAGE_LUXEMBOURGISH              = 0x046E,
    XF_OTF_WIN_LANGUAGE_MACEDONIAN                 = 0x042F,
    XF_OTF_WIN_LANGUAGE_MALAY_BRUNEI               = 0x083E,
    XF_OTF_WIN_LANGUAGE_MALAY_MALAYSIA             = 0x043E,
    XF_OTF_WIN_LANGUAGE_MALAYALAM                  = 0x044C,
    XF_OTF_WIN_LANGUAGE_MALTESE                    = 0x043A,
    XF_OTF_WIN_LANGUAGE_MAORI                      = 0x0481,
    XF_OTF_WIN_LANGUAGE_MAPUDUNGUN                 = 0x047A,
    XF_OTF_WIN_LANGUAGE_MARATHI                    = 0x044E,
    XF_OTF_WIN_LANGUAGE_MOHAWK                     = 0x047C,
    XF_OTF_WIN_LANGUAGE_MONGOLIAN_CYRILLIC         = 0x0450,
    XF_OTF_WIN_LANGUAGE_MONGOLIAN_TRADITIONAL      = 0x0850,
    XF_OTF_WIN_LANGUAGE_NEPALI                     = 0x0461,
    XF_OTF_WIN_LANGUAGE_NORWEGIAN_BOKMAL           = 0x0414,
    XF_OTF_WIN_LANGUAGE_NORWEGIAN_NYNORSK          = 0x0814,
    XF_OTF_WIN_LANGUAGE_OCCITAN                    = 0x0482,
    XF_OTF_WIN_LANGUAGE_ODIA                       = 0x0448,
    XF_OTF_WIN_LANGUAGE_PASHTO                     = 0x0463,
    XF_OTF_WIN_LANGUAGE_POLISH                     = 0x0415,
    XF_OTF_WIN_LANGUAGE_PORTUGUESE_BRAZIL          = 0x0416,
    XF_OTF_WIN_LANGUAGE_PORTUGUESE_PORTUGAL        = 0x0816,
    XF_OTF_WIN_LANGUAGE_PUNJABI                    = 0x0446,
    XF_OTF_WIN_LANGUAGE_QUECHUA_BOLIVIA            = 0x046B,
    XF_OTF_WIN_LANGUAGE_QUECHUA_ECUADOR            = 0x086B,
    XF_OTF_WIN_LANGUAGE_QUECHUA_PERU               = 0x0C6B,
    XF_OTF_WIN_LANGUAGE_ROMANIAN                   = 0x0418,
    XF_OTF_WIN_LANGUAGE_ROMANSH                    = 0x0417,
    XF_OTF_WIN_LANGUAGE_RUSSIAN                    = 0x0419,
    XF_OTF_WIN_LANGUAGE_SAKHA                      = 0x0485,
    XF_OTF_WIN_LANGUAGE_SAMI_INARI                 = 0x243B,
    XF_OTF_WIN_LANGUAGE_SAMI_LULE_NORWAY           = 0x103B,
    XF_OTF_WIN_LANGUAGE_SAMI_LULE_SWEDEN           = 0x143B,
    XF_OTF_WIN_LANGUAGE_SAMI_NORTHERN_FINLAND      = 0x0C3B,
    XF_OTF_WIN_LANGUAGE_SAMI_NORTHERN_NORWAY       = 0x043B,
    XF_OTF_WIN_LANGUAGE_SAMI_NORTHERN_SWEDEN       = 0x083B,
    XF_OTF_WIN_LANGUAGE_SAMI_SKOLT                 = 0x203B,
    XF_OTF_WIN_LANGUAGE_SAMI_SOUTHERN_NORWAY       = 0x183B,
    XF_OTF_WIN_LANGUAGE_SAMI_SOUTHERN_SWEDEN       = 0x1C3B,
    XF_OTF_WIN_LANGUAGE_SANSKRIT                   = 0x044F,
    XF_OTF_WIN_LANGUAGE_SERBIAN_CYRILLIC_BOSNIA    = 0x1C1A,
    XF_OTF_WIN_LANGUAGE_SERBIAN_CYRILLIC_SERBIA    = 0x0C1A,
    XF_OTF_WIN_LANGUAGE_SERBIAN_LATIN_BOSNIA       = 0x181A,
    XF_OTF_WIN_LANGUAGE_SERBIAN_LATIN_SERBIA       = 0x081A,
    XF_OTF_WIN_LANGUAGE_SESOTHO_SA_LEBOA           = 0x046C,
    XF_OTF_WIN_LANGUAGE_SETSWANA                   = 0x0432,
    XF_OTF_WIN_LANGUAGE_SINHALA                    = 0x045B,
    XF_OTF_WIN_LANGUAGE_SLOVAK                     = 0x041B,
    XF_OTF_WIN_LANGUAGE_SLOVENIAN                  = 0x0424,
    XF_OTF_WIN_LANGUAGE_SPANISH_ARGENTINA          = 0x2C0A,
    XF_OTF_WIN_LANGUAGE_SPANISH_BOLIVIA            = 0x400A,
    XF_OTF_WIN_LANGUAGE_SPANISH_CHILE              = 0x340A,
    XF_OTF_WIN_LANGUAGE_SPANISH_COLOMBIA           = 0x240A,
    XF_OTF_WIN_LANGUAGE_SPANISH_COSTA_RICA         = 0x140A,
    XF_OTF_WIN_LANGUAGE_SPANISH_DOMINICAN_REPUBLIC = 0x1C0A,
    XF_OTF_WIN_LANGUAGE_SPANISH_ECUADOR            = 0x300A,
    XF_OTF_WIN_LANGUAGE_SPANISH_EL_SALVADOR        = 0x440A,
    XF_OTF_WIN_LANGUAGE_SPANISH_GUATEMALA          = 0x100A,
    XF_OTF_WIN_LANGUAGE_SPANISH_HONDURAS           = 0x480A,
    XF_OTF_WIN_LANGUAGE_SPANISH_MEXICO             = 0x080A,
    XF_OTF_WIN_LANGUAGE_SPANISH_NICARAGUA          = 0x4C0A,
    XF_OTF_WIN_LANGUAGE_SPANISH_PANAMA             = 0x180A,
    XF_OTF_WIN_LANGUAGE_SPANISH_PARAGUAY           = 0x3C0A,
    XF_OTF_WIN_LANGUAGE_SPANISH_PERU               = 0x280A,
    XF_OTF_WIN_LANGUAGE_SPANISH_PUERTO_RICO        = 0x500A,
    XF_OTF_WIN_LANGUAGE_SPANISH_MODERN_SORT        = 0x0C0A,
    XF_OTF_WIN_LANGUAGE_SPANISH_TRADITIONAL_SORT   = 0x040A,
    XF_OTF_WIN_LANGUAGE_SPANISH_US                 = 0x540A,
    XF_OTF_WIN_LANGUAGE_SPANISH_URUGUAY            = 0x380A,
    XF_OTF_WIN_LANGUAGE_SPANISH_VENEZUELA          = 0x200A,
    XF_OTF_WIN_LANGUAGE_SWEDISH_FINLAND            = 0x081D,
    XF_OTF_WIN_LANGUAGE_SWEDISH_SWEDEN             = 0x041D,
    XF_OTF_WIN_LANGUAGE_SYRIAC                     = 0x045A,
    XF_OTF_WIN_LANGUAGE_TAJIK_CYRILLIC             = 0x0428,
    XF_OTF_WIN_LANGUAGE_TAMAZIGHT_LATIN            = 0x085F,
    XF_OTF_WIN_LANGUAGE_TAMIL                      = 0x0449,
    XF_OTF_WIN_LANGUAGE_TATAR                      = 0x0444,
    XF_OTF_WIN_LANGUAGE_TELUGU                     = 0x044A,
    XF_OTF_WIN_LANGUAGE_THAI                       = 0x041E,
    XF_OTF_WIN_LANGUAGE_TIBETAN                    = 0x0451,
    XF_OTF_WIN_LANGUAGE_TURKISH                    = 0x041F,
    XF_OTF_WIN_LANGUAGE_TURKMEN                    = 0x0442,
    XF_OTF_WIN_LANGUAGE_UIGHUR                     = 0x0480,
    XF_OTF_WIN_LANGUAGE_UKRAINIAN                  = 0x0422,
    XF_OTF_WIN_LANGUAGE_UPPER_SORBIAN              = 0x042E,
    XF_OTF_WIN_LANGUAGE_URDU                       = 0x0420,
    XF_OTF_WIN_LANGUAGE_UZBEK_CYRILLIC             = 0x0843,
    XF_OTF_WIN_LANGUAGE_UZBEK_LATIN                = 0x0443,
    XF_OTF_WIN_LANGUAGE_VIETNAMESE                 = 0x042A,
    XF_OTF_WIN_LANGUAGE_WELSH                      = 0x0452,
    XF_OTF_WIN_LANGUAGE_WOLOF                      = 0x0488,
    XF_OTF_WIN_LANGUAGE_YI                         = 0x0478,
    XF_OTF_WIN_LANGUAGE_YORUBA                     = 0x046A
} XfOtfWinLanguage;

typedef struct XfOtfLanguage {
    XfOtfPlatform platform; /* custom added field, not in binary. */
    union {
        XfOtfMacLanguage mac;
        XfOtfWinLanguage win;
        Uint16           custom;
    } language;
} XfOtfLanguage;

#define XF_OTF_LANGUAGE_DATA_SIZE (sizeof (Uint16))

CString xf_otf_language_to_str (XfOtfLanguage lang);

#endif // ANVIE_CROSSFILE_OTF_TABLES_COMMON_H
