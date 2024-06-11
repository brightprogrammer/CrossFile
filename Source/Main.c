#include <Anvie/Common.h>
#include <Anvie/CrossFile/Xft.h>
#include <Anvie/Types.h>

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
});

typedef struct ArithExpr {
} ArithExpr;

typedef struct CondExpr {
} CondExpr;

typedef enum TypeMemberType {
    TYPE_MEMBER_TYPE_INVALID = 0,
    TYPE_MEMBER_TYPE_UINT8,
    TYPE_MEMBER_TYPE_UINT16,
    TYPE_MEMBER_TYPE_UINT32,
    TYPE_MEMBER_TYPE_UINT64,
    TYPE_MEMBER_TYPE_INT8,
    TYPE_MEMBER_TYPE_INT16,
    TYPE_MEMBER_TYPE_INT32,
    TYPE_MEMBER_TYPE_INT64,
    TYPE_MEMBER_TYPE_SIZE,
    TYPE_MEMBER_TYPE_CHAR,
    TYPE_MEMBER_TYPE_CSTRING,
    TYPE_MEMBER_TYPE_CUSTOM,
    TYPE_MEMBER_TYPE_MAX
} TypeMemberType;

typedef enum TypeMemberTypeAnnotation {
    TYPE_MEMBER_TYPE_ANNOTATION_NONE = 0,
    TYPE_MEMBER_TYPE_ANNOTATION_VECTOR,
    TYPE_MEMBER_TYPE_ANNOTATION_ARRAY,
    TYPE_MEMBER_TYPE_ANNOTATION_MAX
} TypeMemberTypeAnnotation;

typedef struct TypeMember {
    CondExpr* if_cond;       /**< @b Condition on which this member is read. Null if always read. */
    CString   doc_str;       /**< @b If documentation is provied for this member. */

    union {
        ArithExpr* dyn_size; /**< @b Used if vector. */
        Size       const_size; /**< @b Used if array. */
    } length;

    TypeMemberType           type;
    TypeMemberTypeAnnotation type_annotation;

    /**
     * @b Valid only if member is of CUSTOM type. This means that this type was
     * created by either typedef-ing a struct or a union.
     *
     * This name must match exactly one type name of registered types otherwise,
     * the VM during execution will raise a call failure related error.
     * */
    CString type_name;

    CString name; /**< @b Type member field name. */
} TypeMember;

typedef struct Type {
    Bool    is_struct;
    CString type_name;

    TypeMember* members;
    Size        member_count;
    Size        member_capacity;
} Type;

/**
 * @b Skip whitespace starting from given position.
 *
 * @param begin Begin iterator. 
 * @param end End iterator.
 *
 * @return Iterator to character after when first non
 *         whitespace character is found on success.
 * @return @c Null otherwise.
 * */
PRIVATE Char* skip_whitespace (Char* begin, Char* end) {
    RETURN_VALUE_IF (!begin || !end || begin > end, Null, ERR_INVALID_ARGUMENTS);

    /* this means we've completed parsing or there's some error */
    if (begin == end) {
        return end;
    }

    Char* iter = begin;
    while (iter < begin) {
        Char c = *iter;
        if (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\b') {
            break;
        }
    }

    return iter;
}

PRIVATE Char* parse_union (Char* begin, Char* end) {
    RETURN_VALUE_IF (!begin || !end || begin > end, Null, ERR_INVALID_ARGUMENTS);

    if (iter <)

        return iter;
}

int main() {}
