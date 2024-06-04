#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* libc includes */
#include <errno.h>
#include <string.h>

typedef struct Expr Expr;

typedef struct AccessedField {
    CString *field_names;
    Size     count;
} AccessedField;

typedef enum ExprOpndType {
    EXPR_OPND_TYPE_UNKOWN = 0,
    EXPR_OPND_TYPE_ACCESSED_FIELD,
    EXPR_OPND_TYPE_NUMBER,
    EXPR_OPND_TYPE_EXPR,
} ExprOpndType;

typedef struct ExprOpnd {
    ExprOpndType opnd_type;
    union {
        AccessedField *accessed_field;
        Int64          number;
        Expr          *expr;
    };
} ExprOpnd;

typedef enum ExprOp {
    EXPR_OP_UNKNOWN = 0,

    EXPR_OP_ARITH_MIN,
    EXPR_OP_ARITH_ADD = EXPR_OP_ARITH_MIN,
    EXPR_OP_ARITH_SUB,
    EXPR_OP_ARITH_MUL,
    EXPR_OP_ARITH_DIV,
    EXPR_OP_ARITH_MAX = EXPR_OP_ARITH_DIV,

    EXPR_OP_BOOL_BINOP_MIN,
    EXPR_OP_BOOL_OR = EXPR_OP_BOOL_BINOP_MIN,
    EXPR_OP_BOOL_AND,
    EXPR_OP_BOOL_XOR,
    EXPR_OP_BOOL_LSHIFT,
    EXPR_OP_BOOL_RSHIFT,
    EXPR_OP_BOOL_BINOP_MAX = EXPR_OP_BOOL_RSHIFT,

    EXPR_OP_BOOL_UNOP_MIN,
    EXPR_OP_BOOL_NOT      = EXPR_OP_BOOL_UNOP_MIN,
    EXPR_OP_BOOL_UNOP_MAX = EXPR_OP_BOOL_NOT,
} ExprOp;

struct Expr {
    ExprOp epxr_op;
    union {
        struct {
            ExprOpnd lhs;
            ExprOpnd rhs;
        } add, sub, min, div, or, and, xor, lshift, rshift;
    };
};

typedef enum CmpType {
    CMP_TYPE_UNKNOWN = 0,
    CMP_TYPE_EQ,
    CMP_TYPE_LE,
    CMP_TYPE_LT,
    CMP_TYPE_GE,
    CMP_TYPE_GT,
} CmpType;

typedef struct Cmp {
    CmpType cmp_type;
    Expr   *lhs;
    Expr   *rhs;
} Cmp;

typedef enum InsnType {
    INSN_TYPE_UNKNOWN = 0,
    INSN_TYPE_BREAK,
    INSN_TYPE_CONT,
    INSN_TYPE_ASSERT,
    INSN_TYPE_USING,
    INSN_TYPE_READ,
    INSN_TYPE_ASSIGN,
} InsnType;

typedef struct Insn {
    InsnType insn_type;

    union {
        struct {
            Cmp cmp;
        } assert;

        struct {
            AccessedField var;
            Expr          expr;
        } assign;

        struct {
            AccessedField var;
            Expr          addr;
            Expr          read_size;
            Bool          has_read_size; /* size is optional */
        } read;

        struct {
            AccessedField field;
            CString       alias; /* optional alias if provided */
        } using;
    };
} Insn;

typedef struct CodeBlock CodeBlock;

typedef struct Case {
    Expr       case_expr;
    CodeBlock *code_block;
} Case;

typedef struct SwitchCaseBlock {
    Case *cases;
    Size  case_count;
    Size  case_capacity;
} SwitchCaseBlock;

typedef struct ForEachBlock {
    CString       iter_var_name;
    AccessedField accessed_field;
    CodeBlock    *code_block;
} ForEachBlock;

typedef struct IfThenElseBlock {
    Expr       if_expr;
    CodeBlock *then_block;
    CodeBlock *else_block;
} IfThenElseBlock;

typedef enum CodeBlockType {
    CODE_BLOCK_TYPE_UNKOWN,
    CODE_BLOCK_TYPE_INSN,
    CODE_BLOCK_TYPE_SWITCH,
    CODE_BLOCK_TYPE_CASE,
    CODE_BLOCK_TYPE_FOR_EACH,
    CODE_BLOCK_TYPE_IF_THEN_ELSE,
} CodeBlockType;

struct CodeBlock {
    CodeBlockType code_block_type;

    union {
        Insn            insn;
        SwitchCaseBlock switch_case;
        ForEachBlock    for_each_;
        IfThenElseBlock if_then_else_;
    };
};

typedef struct Arg {
    CString       arg_type;
    AccessedField arg_field;
    CString       arg_alias;
} Arg;

typedef struct ArgList {
    Arg *args;
    Size arg_count;
    Size arg_capacity;
} ArgList;

typedef enum ComputeType {
    COMPUTE_TYPE_UNKNOWN,
    COMPUTE_TYPE_SINGLE,
    COMPUTE_TYPE_FOR_EACH,
} ComputeType;

typedef struct Compute {
    ComputeType compute_type;

    union {
        struct {
            CodeBlock block;
        } for_each;

        struct {
            ArgList   arg_list;
            CodeBlock block;
        } single;
    };
} Compute;

typedef struct VarDecl {
    Bool    is_read; /**< Is this field read or computed? */
    CString type_name;
    CString var_name;
} VarDecl;

typedef enum StructType {
    STRUCT_TYPE_UNKNOWN = 0,
    STRUCT_TYPE_FIXED,
    STRUCT_TYPE_SHARED,
    STRUCT_TYPE_POLYMORPH,
} StructType;

typedef struct FixedStruct {
    CString id_str;

    VarDecl *var_decls;
    Size     var_decl_count;
    Size     var_decl_capacity;

    Insn *asserts;
    Size  assert_count;
    Size  assert_capacity;
} FixedStruct;

int main (int argc, char **argv) {
    RETURN_VALUE_IF (
        !argc || !argv || argc != 2 || !argv[1],
        EXIT_FAILURE,
        "USAGE : %s <binfiledesc.stasm>\n",
        argv ? argv[0] : "stasmc"
    );

    FILE   *file      = Null;
    Size    file_size = 0;
    CString file_data = Null;

    /* read complete file at once */
    {
        RETURN_VALUE_IF (!(file = fopen (argv[1], "rb")), EXIT_FAILURE, ERR_FILE_OPEN_FAILED);

        /* get file size */
        fseek (file, 0, SEEK_END);
        file_size = ftell (file);
        fseek (file, 0, SEEK_SET);
        GOTO_HANDLER_IF (!file_size, INIT_FAILED, "File size is zero. Cannot read file\n");

        /* create space to read file */
        GOTO_HANDLER_IF (
            !(file_data = ALLOCATE (Char, file_size + 1)),
            INIT_FAILED,
            ERR_OUT_OF_MEMORY
        );

        /* read complete file */
        GOTO_HANDLER_IF (
            (fread ((void *)file_data, 1, file_size, file) != file_size),
            INIT_FAILED,
            "Failed to read complete file : %s",
            strerror (errno)
        );

        // null terminate file_data
        ((Char *)file_data)[file_size] = 0;
    }

    printf ("%s\n", file_data);

    fclose (file);
    FREE (file_data);
    return EXIT_SUCCESS;

INIT_FAILED:
    if (file) {
        fclose (file);
        file = Null;
    }

    if (file_data) {
        FREE (file_data);
        file_data = Null;
    }

    return EXIT_FAILURE;
}
