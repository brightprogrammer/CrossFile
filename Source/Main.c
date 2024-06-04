#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* crossfile */

// XF Type Definition of a struct
#define XFT_STRUCT(struct_name, struct_def)                                                        \
    typedef struct struct_name struct_def struct_name;                                             \
    CString                               XF_STRUCT_DEF_STR_##struct_name = "struct " #struct_def

#define XFT_STRUCT(struct_name, struct_def)                                                        \
    typedef struct struct_name struct_def struct_name;                                             \
    CString                               XF_STRUCT_DEF_STR_##struct_name = "struct " #struct_def

// XF Type Definition to be used inside struct definitons
#define XFT_VECTOR(vec_type, vec_name, vec_length) vec_type* vec_name

// XF Type Definition to be used inside struct definitons
#define XFT_ARRAY(arr_type, arr_name, arr_length) arr_type arr_name[arr_length]

// XF Type Condition to be used inside struct definitons to switch
// fields on/off for reading depending on whether condition is true or false
#define XFT_IF(cond)
#define XFT_DOC(doc_str)

XFT_STRUCT (XfOtfTableRecord, {
    Uint32 table_tag;
    Uint32 checksum;
    Uint32 offset;

    XFT_DOC ("length of table record field")
    Uint32 length;
});

XFT_STRUCT (XfOtfTableDir, {
    Uint32 sfnt_version;
    Uint16 num_tables;
    Uint16 search_range;
    Uint16 entry_selector;
    Uint16 range_shift;
    XFT_VECTOR (XfOtfTableRecord, table_records, num_tables);
});

XFT_STRUCT (XfOtfOs2, {
    Uint16 version;
    Int16  x_avg_char_width;
    Uint16 weight_class;
    Uint16 width_class;
    Uint16 type;
    Int16  y_subscript_x_size;
    Int16  y_subscript_y_size;
    Int16  y_subscript_x_offset;
    Int16  y_subscript_y_offset;
    Int16  y_superscript_x_size;
    Int16  y_superscript_y_size;
    Int16  y_superscript_x_offset;
    Int16  y_superscript_y_offset;
    Int16  y_strikeout_size;
    Int16  y_strikeout_position;
    Int16  family_class;
    Uint8  panose[10];
    Uint32 unicode_range[4];
    Char   vend_id[4];
    Uint16 selection;
    Uint16 first_char_index;
    Uint16 last_char_index;
    Int16  typo_ascender;
    Int16  typo_descender;
    Int16  typo_line_gap;
    Uint16 win_ascent;
    Uint16 win_descent;

    /* v1 */
    XFT_IF (version >= 1) Uint32 code_page_range[2];

    /* v2, v3, v4*/
    XFT_IF (version >= 2) Int16  x_height;
    XFT_IF (version >= 2) Int16  cap_height;
    XFT_IF (version >= 2) Uint16 default_char;
    XFT_IF (version >= 2) Uint16 break_char;
    XFT_IF (version >= 2) Uint16 max_context;

    /* v5 */
    XFT_IF (version == 5) Uint16 lower_optical_point_size;
    XFT_IF (version == 5) Uint16 upper_optical_point_size;
})

Int32 main (int argc, char** argv) {}
