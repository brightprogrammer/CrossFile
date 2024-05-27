#include <Anvie/CrossFile/Otf/Tables/Common.h>

static const CString platform_id_to_str_map[] =
    {"VARIOUS", "MAC", "ISO", "WIN", "CUSTOM", "UNKNOWN"};

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

CString xf_otf_platform_encoding_get_platform_str (XfOtfPlatformEncoding platform_encoding) {
    return platform_encoding.platform_id < XF_OTF_PLATFORM_ID_MAX ?
               platform_id_to_str_map[platform_encoding.platform_id] :
               "UNKNOWN";
}

CString xf_otf_platform_encoding_get_encoding_str (XfOtfPlatformEncoding platform_encoding) {
    if (platform_encoding.platform_id <= XF_OTF_PLATFORM_ID_MAX) {
        switch (platform_encoding.platform_id) {
            case XF_OTF_PLATFORM_ID_VARIOUS : {
                if (platform_encoding.encoding_id.various <= XF_OTF_VARIOUS_ENCODING_ID_MAX) {
                    return enc_various_to_str_map[platform_encoding.encoding_id.various];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_ID_MAC : {
                if (platform_encoding.encoding_id.mac <= XF_OTF_MAC_ENCODING_ID_MAX) {
                    return enc_mac_to_str_map[platform_encoding.encoding_id.mac];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_ID_ISO : {
                if (platform_encoding.encoding_id.iso <= XF_OTF_ISO_ENCODING_ID_MAX) {
                    return enc_iso_to_str_map[platform_encoding.encoding_id.iso];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_ID_WIN : {
                if (platform_encoding.encoding_id.win <= XF_OTF_WIN_ENCODING_ID_MAX) {
                    return enc_win_to_str_map[platform_encoding.encoding_id.win];
                } else {
                    return "UNKNOWN";
                }
            }
            case XF_OTF_PLATFORM_ID_CUSTOM :
                return "CUSTOM";
            default :
                return "UNKNOWN";
        }
    } else {
        return "UNKNOWN";
    }
}
