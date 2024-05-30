/**
 * @file Common.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/Otf/Tables/Common.h>

static const CString platform_to_str_map[] = {"VARIOUS", "MAC", "ISO", "WIN", "CUSTOM", "UNKNOWN"};

static const CString enc_various_to_str_map[] = {
    "Unicode 1.0",
    "Unicode 1.1",
    "ISO/IEC 10646",
    "Unicode 2.0+ (BMP only)",
    "Unicode 2.0+ (Full repertoire)",
    "Unicode Variation Sequences",
    "Unicode full repertoire",
    "UNKNOWN"
};
static const CString enc_iso_to_str_map[] =
    {"7-bit ASCII", "ISO 10646\0", "ISO 8859-1\0", "UNKNOWN"};
static const CString enc_win_to_str_map[] = {
    "Symbol",
    "Unicode BMP",
    "ShiftJIS",
    "PRC",
    "Big5",
    "Wansung",
    "Johab",
    "Reserved",
    "Reserved",
    "Reserved",
    "Unicode full repertoire",
    "UNKNOWN"
};
static CString enc_mac_to_str_map[] = {
    "Roman",
    "Japanese",
    "Chinese (Traditional)",
    "Korean",
    "Arabic",
    "Hebrew",
    "Greek",
    "Russian",
    "RSymbol",
    "Devanagari",
    "Gurmukhi",
    "Gujarati",
    "Odia",
    "Bangla",
    "Tamil",
    "Telugu",
    "Kannada",
    "Malayalam",
    "Sinhalese",
    "Burmese",
    "Khmer",
    "Thai",
    "Laotian",
    "Georgian",
    "Armenian",
    "Chinese (Simplified)",
    "Tibetan",
    "Mongolian",
    "Geez",
    "Slavic",
    "Vietnamese",
    "Sindhi",
    "Uninterpreted",
    "UNKNOWN"
};

static inline CString win_lang_to_full_str (XfOtfWinLanguage lang);
static inline CString mac_lang_to_full_str (XfOtfMacLanguage lang);

CString xf_otf_platform_encoding_get_platform_str (XfOtfPlatformEncoding platform_encoding) {
    return platform_encoding.platform < XF_OTF_PLATFORM_MAX ?
               platform_to_str_map[platform_encoding.platform] :
               "UNKNOWN";
}

CString xf_otf_platform_encoding_get_encoding_str (XfOtfPlatformEncoding platform_encoding) {
    if (platform_encoding.platform <= XF_OTF_PLATFORM_MAX) {
        switch (platform_encoding.platform) {
            case XF_OTF_PLATFORM_VARIOUS : {
                if (platform_encoding.encoding.various <= XF_OTF_VARIOUS_ENCODING_MAX) {
                    return enc_various_to_str_map[platform_encoding.encoding.various];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_MAC : {
                if (platform_encoding.encoding.mac <= XF_OTF_MAC_ENCODING_MAX) {
                    return enc_mac_to_str_map[platform_encoding.encoding.mac];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_ISO : {
                if (platform_encoding.encoding.iso <= XF_OTF_ISO_ENCODING_MAX) {
                    return enc_iso_to_str_map[platform_encoding.encoding.iso];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_WIN : {
                if (platform_encoding.encoding.win <= XF_OTF_WIN_ENCODING_MAX) {
                    return enc_win_to_str_map[platform_encoding.encoding.win];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_CUSTOM :
                return "CUSTOM";
            default :
                return "UNKNOWN";
        }
    } else {
        return "UNKNOWN";
    }
}

CString xf_otf_language_to_str (XfOtfLanguage lang_id) {
    switch (lang_id.platform) {
        case XF_OTF_PLATFORM_MAC :
            return mac_lang_to_full_str (lang_id.language.mac);
        case XF_OTF_PLATFORM_WIN :
            return win_lang_to_full_str (lang_id.language.win);
        default :
            return "Unknown";
    }
}

static inline CString win_lang_to_full_str (XfOtfWinLanguage lang) {
    switch (lang) {
        case 0x041C :
            return "Albanian";
        case 0x0484 :
            return "Alsatian";
        case 0x045E :
            return "Amharic";
        case 0x1401 :
            return "Arabic (Algeria)";
        case 0x3C01 :
            return "Arabic (Bahrain)";
        case 0x0C01 :
            return "Arabic (Egypt)";
        case 0x0801 :
            return "Arabic (Iraq)";
        case 0x2C01 :
            return "Arabic (Jordan)";
        case 0x3401 :
            return "Arabic (Kuwait)";
        case 0x3001 :
            return "Arabic (Lebanon)";
        case 0x1001 :
            return "Arabic (Libya)";
        case 0x1801 :
            return "Arabic (Morocco)";
        case 0x2001 :
            return "Arabic (Oman)";
        case 0x4001 :
            return "Arabic (Qatar)";
        case 0x0401 :
            return "Arabic (Saudi Arabia)";
        case 0x2801 :
            return "Arabic (Syria)";
        case 0x1C01 :
            return "Arabic (Tunisia)";
        case 0x3801 :
            return "Arabic (U.A.E.)";
        case 0x2401 :
            return "Arabic (Yemen)";
        case 0x042B :
            return "Armenian";
        case 0x044D :
            return "Assamese";
        case 0x082C :
            return "Azerbaijani (Cyrillic)";
        case 0x042C :
            return "Azerbaijani (Latin)";
        case 0x046D :
            return "Bashkir";
        case 0x042D :
            return "Basque";
        case 0x0423 :
            return "Belarusian";
        case 0x0845 :
            return "Bangla (Bangladesh)";
        case 0x0445 :
            return "Bangla (India)";
        case 0x201A :
            return "Bosnian (Cyrillic)";
        case 0x141A :
            return "Bosnian (Latin)";
        case 0x047E :
            return "Breton";
        case 0x0402 :
            return "Bulgarian";
        case 0x0403 :
            return "Catalan";
        case 0x0C04 :
            return "Chinese (Hong Kong SAR)";
        case 0x1404 :
            return "Chinese (Macao SAR)";
        case 0x0804 :
            return "Chinese (People’s Republic of China)";
        case 0x1004 :
            return "Chinese (Singapore)";
        case 0x0404 :
            return "Chinese (Taiwan)";
        case 0x0483 :
            return "Corsican";
        case 0x041A :
            return "Croatian";
        case 0x101A :
            return "Croatian (Latin)";
        case 0x0405 :
            return "Czech";
        case 0x0406 :
            return "Danish";
        case 0x048C :
            return "Dari";
        case 0x0465 :
            return "Divehi";
        case 0x0813 :
            return "Dutch (Belgium)";
        case 0x0413 :
            return "Dutch (Netherlands)";
        case 0x0C09 :
            return "English (Australia)";
        case 0x2809 :
            return "English (Belize)";
        case 0x1009 :
            return "English (Canada)";
        case 0x2409 :
            return "English (Caribbean)";
        case 0x4009 :
            return "English (India)";
        case 0x1809 :
            return "English (Ireland)";
        case 0x2009 :
            return "English (Jamaica)";
        case 0x4409 :
            return "English (Malaysia)";
        case 0x1409 :
            return "English (New Zealand)";
        case 0x3409 :
            return "English (Republic of the Philippines)";
        case 0x4809 :
            return "English (Singapore)";
        case 0x1C09 :
            return "English (South Africa)";
        case 0x2C09 :
            return "English (Trinidad and Tobago)";
        case 0x0809 :
            return "English (United Kingdom)";
        case 0x0409 :
            return "English (United States)";
        case 0x3009 :
            return "English (Zimbabwe)";
        case 0x0425 :
            return "Estonian";
        case 0x0438 :
            return "Faroese";
        case 0x0464 :
            return "Filipino";
        case 0x040B :
            return "Finnish";
        case 0x080C :
            return "French (Belgium)";
        case 0x0C0C :
            return "French (Canada)";
        case 0x040C :
            return "French (France)";
        case 0x140C :
            return "French (Luxembourg)";
        case 0x180C :
            return "French (Principality of Monaco)";
        case 0x100C :
            return "French (Switzerland)";
        case 0x0462 :
            return "Frisian";
        case 0x0456 :
            return "Galician";
        case 0x0437 :
            return "Georgian";
        case 0x0C07 :
            return "German (Austria)";
        case 0x0407 :
            return "German (Germany)";
        case 0x1407 :
            return "German (Liechtenstein)";
        case 0x1007 :
            return "German (Luxembourg)";
        case 0x0807 :
            return "German (Switzerland)";
        case 0x0408 :
            return "Greek";
        case 0x046F :
            return "Greenlandic";
        case 0x0447 :
            return "Gujarati";
        case 0x0468 :
            return "Hausa (Latin)";
        case 0x040D :
            return "Hebrew";
        case 0x0439 :
            return "Hindi";
        case 0x040E :
            return "Hungarian";
        case 0x040F :
            return "Icelandic";
        case 0x0470 :
            return "Igbo";
        case 0x0421 :
            return "Indonesian";
        case 0x045D :
            return "Inuktitut";
        case 0x085D :
            return "Inuktitut (Latin)";
        case 0x083C :
            return "Irish";
        case 0x0434 :
            return "isiXhosa";
        case 0x0435 :
            return "isiZulu";
        case 0x0410 :
            return "Italian (Italy)";
        case 0x0810 :
            return "Italian (Switzerland)";
        case 0x0411 :
            return "Japanese";
        case 0x044B :
            return "Kannada";
        case 0x043F :
            return "Kazakh";
        case 0x0453 :
            return "Khmer";
        case 0x0486 :
            return "K’iche";
        case 0x0487 :
            return "Kinyarwanda";
        case 0x0441 :
            return "Kiswahili";
        case 0x0457 :
            return "Konkani";
        case 0x0412 :
            return "Korean";
        case 0x0440 :
            return "Kyrgyz";
        case 0x0454 :
            return "Lao";
        case 0x0426 :
            return "Latvian";
        case 0x0427 :
            return "Lithuanian";
        case 0x082E :
            return "Lower Sorbian";
        case 0x046E :
            return "Luxembourgish";
        case 0x042F :
            return "Macedonian";
        case 0x083E :
            return "Malay (Brunei)";
        case 0x043E :
            return "Malay (Malaysia)";
        case 0x044C :
            return "Malayalam";
        case 0x043A :
            return "Maltese";
        case 0x0481 :
            return "Maori";
        case 0x047A :
            return "Mapudungun";
        case 0x044E :
            return "Marathi";
        case 0x047C :
            return "Mohawk";
        case 0x0450 :
            return "Mongolian (Cyrillic)";
        case 0x0850 :
            return "Mongolian (Traditional)";
        case 0x0461 :
            return "Nepali";
        case 0x0414 :
            return "Norwegian (Bokmal)";
        case 0x0814 :
            return "Norwegian (Nynorsk)";
        case 0x0482 :
            return "Occitan";
        case 0x0448 :
            return "Odia";
        case 0x0463 :
            return "Pashto";
        case 0x0415 :
            return "Polish";
        case 0x0416 :
            return "Portuguese (Brazil)";
        case 0x0816 :
            return "Portuguese (Portugal)";
        case 0x0446 :
            return "Punjabi";
        case 0x046B :
            return "Quechua (Bolivia)";
        case 0x086B :
            return "Quechua (Ecuador)";
        case 0x0C6B :
            return "Quechua (Peru)";
        case 0x0418 :
            return "Romanian";
        case 0x0417 :
            return "Romansh";
        case 0x0419 :
            return "Russian";
        case 0x0485 :
            return "Sakha";
        case 0x243B :
            return "Sami (Inari)";
        case 0x103B :
            return "Sami (Lule) (Norway)";
        case 0x143B :
            return "Sami (Lule) (Sweden)";
        case 0x0C3B :
            return "Sami (Northern) (Finland)";
        case 0x043B :
            return "Sami (Northern) (Norway)";
        case 0x083B :
            return "Sami (Northern) (Sweden)";
        case 0x203B :
            return "Sami (Skolt)";
        case 0x183B :
            return "Sami (Southern) (Norway)";
        case 0x1C3B :
            return "Sami (Southern) (Sweden)";
        case 0x044F :
            return "Sanskrit";
        case 0x1C1A :
            return "Serbian (Cyrillic) (Bosnia and Herzegovina)";
        case 0x0C1A :
            return "Serbian (Cyrillic) (Serbia)";
        case 0x181A :
            return "Serbian (Latin) (Bosnia and Herzegovina)";
        case 0x081A :
            return "Serbian (Latin) (Serbia)";
        case 0x046C :
            return "Sesotho sa Leboa";
        case 0x0432 :
            return "Setswana";
        case 0x045B :
            return "Sinhala";
        case 0x041B :
            return "Slovak";
        case 0x0424 :
            return "Slovenian";
        case 0x2C0A :
            return "Spanish (Argentina)";
        case 0x400A :
            return "Spanish (Bolivia)";
        case 0x340A :
            return "Spanish (Chile)";
        case 0x240A :
            return "Spanish (Colombia)";
        case 0x140A :
            return "Spanish (Costa Rica)";
        case 0x1C0A :
            return "Spanish (Dominican Republic)";
        case 0x300A :
            return "Spanish (Ecuador)";
        case 0x440A :
            return "Spanish (El Salvador)";
        case 0x100A :
            return "Spanish (Guatemala)";
        case 0x480A :
            return "Spanish (Honduras)";
        case 0x080A :
            return "Spanish (Mexico)";
        case 0x4C0A :
            return "Spanish (Nicaragua)";
        case 0x180A :
            return "Spanish (Panama)";
        case 0x3C0A :
            return "Spanish (Paraguay)";
        case 0x280A :
            return "Spanish (Peru)";
        case 0x500A :
            return "Spanish (Puerto Rico)";
        case 0x0C0A :
            return "Spanish (Spain) (Modern Sort)";
        case 0x040A :
            return "Spanish (Spain) (Traditional Sort)";
        case 0x540A :
            return "Spanish (United States)";
        case 0x380A :
            return "Spanish (Uruguay)";
        case 0x200A :
            return "Spanish (Venezuela)";
        case 0x081D :
            return "Swedish (Finland)";
        case 0x041D :
            return "Swedish (Sweden)";
        case 0x045A :
            return "Syriac";
        case 0x0428 :
            return "Tajik (Cyrillic)";
        case 0x085F :
            return "Tamazight (Latin)";
        case 0x0449 :
            return "Tamil";
        case 0x0444 :
            return "Tatar";
        case 0x044A :
            return "Telugu";
        case 0x041E :
            return "Thai";
        case 0x0451 :
            return "Tibetan (PRC)";
        case 0x041F :
            return "Turkish";
        case 0x0442 :
            return "Turkmen";
        case 0x0480 :
            return "Uighur (PRC)";
        case 0x0422 :
            return "Ukrainian";
        case 0x042E :
            return "Upper Sorbian";
        case 0x0420 :
            return "Urdu (Islamic Republic of Pakistan)";
        case 0x0843 :
            return "Uzbek (Cyrillic)";
        case 0x0443 :
            return "Uzbek (Latin)";
        case 0x042A :
            return "Vietnamese";
        case 0x0452 :
            return "Welsh";
        case 0x0488 :
            return "Wolof";
        case 0x0478 :
            return "Yi (PRC)";
        case 0x046A :
            return "Yoruba";
        default :
            return "Unknown";
    }
}

static inline CString mac_lang_to_full_str (XfOtfMacLanguage lang_id) {
    RETURN_VALUE_IF (lang_id > XF_OTF_MAC_LANGUAGE_MAX, "Unknown", "Invalid language id\n");

    static const CString full_language_names[] = {
        "English",
        "French",
        "German",
        "Italian",
        "Dutch",
        "Swedish",
        "Spanish",
        "Danish",
        "Portuguese",
        "Norwegian",
        "Hebrew",
        "Japanese",
        "Arabic",
        "Finnish",
        "Greek",
        "Icelandic",
        "Maltese",
        "Turkish",
        "Croatian",
        "Chinese (Traditional)",
        "Urdu",
        "Hindi",
        "Thai",
        "Korean",
        "Lithuanian",
        "Polish",
        "Hungarian",
        "Estonian",
        "Latvian",
        "Sami",
        "Faroese",
        "Farsi/Persian",
        "Russian",
        "Chinese (Simplified)",
        "Flemish",
        "Irish",
        "Albanian",
        "Romanian",
        "Czech",
        "Slovak",
        "Slovenian",
        "Yiddish",
        "Serbian",
        "Macedonian",
        "Bulgarian",
        "Ukrainian",
        "Byelorussian",
        "Uzbek",
        "Kazakh",
        "Azerbaijani (Cyrillic script)",
        "Azerbaijani (Arabic script)",
        "Armenian",
        "Georgian",
        "Romanian (Moldova)",
        "Kirghiz",
        "Tajiki",
        "Turkmen",
        "Mongolian (Mongolian script)",
        "Mongolian (Cyrillic script)",
        "Pashto",
        "Kurdish",
        "Kashmiri",
        "Sindhi",
        "Tibetan",
        "Nepali",
        "Sanskrit",
        "Marathi",
        "Bangla",
        "Assamese",
        "Gujarati",
        "Punjabi",
        "Odia",
        "Malayalam",
        "Kannada",
        "Tamil",
        "Telugu",
        "Sinhalese",
        "Burmese",
        "Khmer",
        "Lao",
        "Vietnamese",
        "Indonesian",
        "Tagalog",
        "Malay (Roman script)",
        "Malay (Arabic script)",
        "Amharic",
        "Tigrinya",
        "Galla",
        "Somali",
        "Swahili",
        "Kinyarwanda/Ruanda",
        "Rundi",
        "Nyanja/Chewa",
        "Malagasy",
        "Esperanto",
        "Welsh",
        "Basque",
        "Catalan",
        "Latin",
        "Quechua",
        "Guarani",
        "Aymara",
        "Tatar",
        "Uighur",
        "Dzongkha",
        "Javanese (Roman script)",
        "Sundanese (Roman script)",
        "Galician",
        "Afrikaans",
        "Breton",
        "Inuktitut",
        "Scottish Gaelic",
        "Manx",
        "Irish (with dot above)",
        "Tongan",
        "Greek (polytonic)",
        "Greenlandic",
        "Azerbaijani (Roman script)"
    };

    return full_language_names[lang_id];
}
