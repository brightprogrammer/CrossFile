#include <Anvie/Common.h>
#include <Anvie/Types.h>

typedef enum XftVmInsnType : Uint8 {
    XFT_VM_INSN_TYPE_UNKNOWN = 0,

    XFT_VM_INSN_TYPE_SET_REG, /* setr reg, imm */

    /* read data of different sizes form data stream to given memory offset */
    XFT_VM_INSN_TYPE_READ_T8,  /* rd8 mem_off */
    XFT_VM_INSN_TYPE_READ_T16, /* rd16 mem_off */
    XFT_VM_INSN_TYPE_READ_T32, /* rd32 mem_off */
    XFT_VM_INSN_TYPE_READ_T64, /* rd64 mem_off */

    /* push data onto stack from given register */
    XFT_VM_INSN_TYPE_PUSH_R8,  /* push8 reg */
    XFT_VM_INSN_TYPE_PUSH_R16, /* push16 reg */
    XFT_VM_INSN_TYPE_PUSH_R32, /* push32 reg */
    XFT_VM_INSN_TYPE_PUSH_R64, /* push64 reg */

    /* push data onto stack from given memory offset */
    XFT_VM_INSN_TYPE_PUSH_M8,  /* push8 mem_off */
    XFT_VM_INSN_TYPE_PUSH_M16, /* push16 mem_off */
    XFT_VM_INSN_TYPE_PUSH_M32, /* push32 mem_off */
    XFT_VM_INSN_TYPE_PUSH_M64, /* push64 mem_off */

    /* pop data from stack to given register */
    XFT_VM_INSN_TYPE_POP_R8,  /* pop8 mem_off */
    XFT_VM_INSN_TYPE_POP_R16, /* pop16 mem_off */
    XFT_VM_INSN_TYPE_POP_R32, /* pop32 mem_off */
    XFT_VM_INSN_TYPE_POP_R64, /* pop64 mem_off */

    /* pop data from stack to given memory offset */
    XFT_VM_INSN_TYPE_POP_M8,  /* pop8 mem_off */
    XFT_VM_INSN_TYPE_POP_M16, /* pop16 mem_off */
    XFT_VM_INSN_TYPE_POP_M32, /* pop32 mem_off */
    XFT_VM_INSN_TYPE_POP_M64, /* pop64 mem_off */

    /* read an array of data of different sizes form data stream */
    XFT_VM_INSN_TYPE_READ_A8,  /* ra8 mem_off, read_elem_count */
    XFT_VM_INSN_TYPE_READ_A16, /* ra16 mem_off, read_elem_count */
    XFT_VM_INSN_TYPE_READ_A32, /* ra32 mem_off, read_elem_count */
    XFT_VM_INSN_TYPE_READ_A64, /* ra64 mem_off, read_elem_count */

    /* push arrayt of data onto stack from given memory offset */
    XFT_VM_INSN_TYPE_PUSH_A8,  /* pusha8 mem_off, push_elem_count */
    XFT_VM_INSN_TYPE_PUSH_A16, /* pusha16 mem_off, push_elem_count */
    XFT_VM_INSN_TYPE_PUSH_A32, /* pusha32 mem_off, push_elem_count */
    XFT_VM_INSN_TYPE_PUSH_A64, /* pusha64 mem_off, push_elem_count */

    /* pop array of data from stack to given memory offset */
    XFT_VM_INSN_TYPE_POP_A8,  /* popa8 mem_off, pop_elem_count */
    XFT_VM_INSN_TYPE_POP_A16, /* popa16 mem_off, pop_elem_count */
    XFT_VM_INSN_TYPE_POP_A32, /* popa32 mem_off, pop_elem_count */
    XFT_VM_INSN_TYPE_POP_A64, /* popa64 mem_off, pop_elem_count */

    /* seek instructions inside the file stream */
    XFT_VM_INSN_TYPE_SEEK_FWD, /* seekf num_bytes */
    XFT_VM_INSN_TYPE_SEEK_BAK, /* seekb num_bytes */

    /* operations on registers */
    /* arithmetic operators */
    XFT_VM_INSN_TYPE_ADD,  /* add rres, r1, r2 */
    XFT_VM_INSN_TYPE_SUB,  /* sub rres, r1, r2 */
    XFT_VM_INSN_TYPE_MUL,  /* mul rres, r1, r2 */
    XFT_VM_INSN_TYPE_DIV,  /* div rres, r1, r2 */
    XFT_VM_INSN_TYPE_MOD,  /* mod rres, r1, r2 */
    XFT_VM_INSN_TYPE_POW,  /* mod rres, r1, r2 */
    XFT_VM_INSN_TYPE_SQRT, /* mod rres, r1 */
    XFT_VM_INSN_TYPE_ABS,  /* mod rres, r1 */
    /* logical operators */
    XFT_VM_INSN_TYPE_AND,  /* and rres, r1, r2 */
    XFT_VM_INSN_TYPE_OR,   /* or rres, r1, r2 */
    XFT_VM_INSN_TYPE_XOR,  /* xor rres, r1, r2 */
    XFT_VM_INSN_TYPE_NAND, /* nand rres, r1, r2 */
    XFT_VM_INSN_TYPE_NOR,  /* nor rres, r1, r2 */
    XFT_VM_INSN_TYPE_XNOR, /* xnor rres, r1, r2 */
    XFT_VM_INSN_TYPE_NOT,  /* not rres, r1 */
    /* bitwise operators */
    XFT_VM_INSN_TYPE_LSHIFT, /* lshift rres, r1, r2 */
    XFT_VM_INSN_TYPE_RSHIFT, /* rshift rres, r1, r2 */
    XFT_VM_INSN_TYPE_ROL,    /* rshift rres, r1, r2 */
    XFT_VM_INSN_TYPE_ROR,    /* rshift rres, r1, r2 */
    /* comparision operators */
    XFT_VM_INSN_TYPE_CMPEQ, /* cmpeq rres, r1, r2 */
    XFT_VM_INSN_TYPE_CMPLE, /* cmple rres, r1, r2 */
    XFT_VM_INSN_TYPE_CMPLT, /* cmplt rres, r1, r2 */
    XFT_VM_INSN_TYPE_CMPGE, /* cmpge rres, r1, r2 */
    XFT_VM_INSN_TYPE_CMPGT, /* cmpgt rres, r1, r2 */

    /* control flow instructions */
    /* jumps happen by selecting the block from an array of blocks that the VM has */
    XFT_VM_INSN_TYPE_JA,               /* ja reg, sel : jump if above */
    XFT_VM_INSN_TYPE_JB,               /* jb reg, sel : jump if below */
    XFT_VM_INSN_TYPE_JZ,               /* jz reg, sel : jump if zero */
    XFT_VM_INSN_TYPE_JO,               /* jo reg, sel : jump if overflow */
    XFT_VM_INSN_TYPE_JC,               /* jc reg, sel : jump if carry */

    XFT_VM_INSN_TYPE_CALL_TYPE_LOADER, /* typeload typesel: Call loader method */

    /* printing and debugging infos */
    XFT_VM_INSN_TYPE_PINFO,        /* pinfo, str : Just some informative message */
    XFT_VM_INSN_TYPE_PDBG,         /* pdbg, str : When generated code thinks it might be wrong */
    XFT_VM_INSN_TYPE_PERR,         /* perr, str : When generated code thinks it is wrong */
    XFT_VM_INSN_TYPE_EXIT_SUCCESS, /* exit_success */
    XFT_VM_INSN_TYPE_EXIT_FAILURE, /* exit_failure*/

    XFT_VM_INSN_TYPE_MAX
} XftVmInsnType;

typedef struct XftVmInsn {
    XftVmInsnType insn_type;

    union {
        struct {
            Uint8  reg;
            Uint64 imm;
        } set_reg;

        struct {
            Uint8  reg;
            Uint64 mem_off;
        } read;

        struct {
            Uint8 reg;
        } push_reg, pop_reg;

        struct {
            Uint64 mem_off;
        } push_mem, pop_mem;

        struct {
            Uint64 mem_off;
            Size   elem_count;
        } read_arr, push_arr, pop_arr;

        struct {
            Size num_bytes;
        } seek_fwd, seek_bak;

        struct {
            Uint8 rres; /**< @b Register where result of op will be stored. */
            Uint8 r1;   /**< @b First operand. */
            Uint8 r2;   /**< @b Second operand. */
        } add, sub, mul, div, mod, pow, and, or, xor, nand, nor, xnor, lshift, rshift, rol, ror,
            cmpeq, cmple, cmplt, cmpge, cmpgt;

        struct {
            Uint8 rres; /**< @b Register where result of op will be stored */
            Uint8 r1;   /**< @b The one and only operand */
        } sqrt, abs, not ;

        struct {
            Uint8 reg;       /**< @b Register which contains the result of comparision. */
            Size  block_sel; /**< @b Index of block to jump to. */
        } ja, jb, jz, jo, jc;

        struct {
            Size type_load_sel; /**< @b Index of type loader to be called. */
        } call_type_loader;
    } insn;
} XftVmInsn;

/**
 * @b InsnBlock represents a sequence of structions such that any two
 * instructions in the block does not have a jump. They are analogous
 * to basic blocks.
 *
 * A VM may have many `InsnBlocks` and all blocks are stored and maintained by the
 * VM instance in a contiguous array. If the VM code wants to make a jump, they provide
 * the condition for jump and a block selector. The block selector is an index into
 * the array where all `InsnBlock` are stored.
 *
 * This selection based mechanism is created to avoid computing address offsets,
 * and make the basic blocks as usable as possible.
 * */
typedef struct XftVmInsnBlock {
    XftVmInsn* insns;
    Size       insn_count;
    Size       insn_capacity;
} XftVmInsnBlock;

/**
 * @b Type loaders are like methods that get called when VM wants to load
 * another type.
 *
 * Each type laoder stores reference to all the type loader that it can
 * call in future. This is just for quick finding of type loader, because a file
 * may have many types and finding the loader in all those arrays "may" be slow.
 * */
typedef struct XftVmTypeLoader XftVmTypeLoader;
struct XftVmTypeLoader {
    CString type_name;               /**< @b Name of type (same as struct name/typedef) */
    CString type_doc;                /**< @b Description of given type */
    Size    alloc_size;              /**< @b Allocation size for a single element of this struct */

    XftVmInsnBlock insn_blocks;      /**< @b All the basic blocks in this type loader */
    Size           insn_block_count; /**< @b Number of basic blocks in this loader. */
    Size           insn_block_capacity; /**< @b Capacity of insn_blocks array */

    XftVmTypeLoader**
         type_loader_refs;         /**< @b References to all type loaders this loader can call. */
    Size type_loader_ref_count;    /**< @b Number of references in the references array */
    Size type_loader_ref_capacity; /**< @b Capacity of references array */
};

/**
 * @b One needs to create a file loader to load the whole file.
 * 
 * A file loader contains information about all the types and type laoders,
 * that are required to load the file.
 * */
typedef struct XftVmFileLoader {
    Size main_type_loader_index;

    XftVmTypeLoader* type_loaders;         /**< @b Vector of all type loaders. */
    Size             type_loader_count;    /**< @b Number of type loaders */
    Size             type_loader_capacity; /**< @b Maximum capacity of type loader vector */
} XftVmFileLoader;

/**
 * @b Stack is allocated and maintained by the VM.
 * Each method/function get's a separate stack.
 * */
typedef struct XftVmTypeLoaderStack {
    Uint8* stack_data;
    Size   stack_size;
    Size   stack_capacity;
} XftVmTypeLoadStack;

#define XFT_VM_REG_NUM 8

/**
 * @b The CrossFile Type VM.
 * */
typedef struct XftVm {
    Uint64 regs[XFT_VM_REG_NUM];

    XftVmTypeLoadStack* stacks;         /**< @b A vector of stacks for type loaders */
    Size                stack_count;    /**< @b Number of stacks. */
    Size                stack_capacity; /**< @b Capacity of stacks vector */
} XftVm;

XftVm* xft_vm_init (XftVm* vm);
XftVm* xft_vm_deinit (XftVm* vm);
Uint8* xft_vm_exec_file_loader (XftVm* vm, XftVmFileLoader* file_loader);
Uint8* xft_vm_exec_type_loader (XftVm* vm, XftVmTypeLoader* type_loader);

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

Int32 main (int argc, char** argv) {
    XfOtfFile otf;
    xf_otf_file_open (&otf, argv[1]);

    return 0;
}
