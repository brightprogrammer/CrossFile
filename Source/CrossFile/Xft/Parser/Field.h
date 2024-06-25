/**
 * @file Field.h
 * @date Tue, 25th June 2024
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
 *
 * This file defines structures to contain abstract syntax tree of Xft structure description language.
 * */

#ifndef ANVIE_CROSSFILE_SOURCE_XFT_FIELD_H
#define ANVIE_CROSSFILE_SOURCE_XFT_FIELD_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* When TO_ArithExpr is used insted of ArithExpr, this means there's a transfership of ownsership
 * invovled. Same goes for any other type. TO_CondExpr means "Transfer Ownership of CondEpxr", and so on...
 *
 * This ownership transfer only applies to members of passed object and not the object itself.
 * FREE can be called upon this object but not it's "destroy" method, because destroy will trigger
 * destruction of members of object as well, which is/are owned by some other object/function now. 
 *
 * If the object is returned then the whole object is owned and now neither FREE or "destroy" must
 * be called!
 * */

typedef struct ExprOpnd  ExprOpnd, TO_ExprOpnd;
typedef struct ArithExpr ArithExpr, TO_ArithExpr;
typedef struct CondExpr  CondExpr, TO_CondExpr;
typedef struct Field     Field, TO_Field;

/**
 * Conditional or Arithmetic operand type.
 * */
typedef enum ExprOpndType {
    EXPR_OPND_TYPE_INVALID = 0,
    EXPR_OPND_TYPE_INT,        /**< @b Signed integer. */
    EXPR_OPND_TYPE_UINT,       /**< @b Unsigned integer. */
    EXPR_OPND_TYPE_FLOAT,      /**< @b Floating point number. */
    EXPR_OPND_TYPE_ARITH_EXPR, /**< @b Arithmetic expression */
    EXPR_OPND_TYPE_COND_EXPR,  /**< @b Conditional expression */
    EXPR_OPND_TYPE_MAX
} ExprOpndType;

/**
 * @b Arithmetic expression operand
 * */
struct ExprOpnd {
    ExprOpndType opnd_type;
    union {
        Uint64     unsigned_val;
        Int64      signed_val;
        Float64    float_val;
        ArithExpr* arith_expr;
        CondExpr*  cond_expr;
    };
};

/**
 * @b Arithmetic Expression Types.
 * */
typedef enum ArithExprOp {
    ARITH_EXPR_OP_INVALID = 0,
    ARITH_EXPR_OP_ADD,    /**< @b Integer addition */
    ARITH_EXPR_OP_SUB,    /**< @b Integer subtraction*/
    ARITH_EXPR_OP_MUL,    /**< @b Integer multiplication */
    ARITH_EXPR_OP_DIV,    /**< @b Integer division */
    ARITH_EXPR_OP_FADD,   /**< @b Float addition */
    ARITH_EXPR_OP_FSUB,   /**< @b Float subtraction*/
    ARITH_EXPR_OP_FMUL,   /**< @b Float multiplication */
    ARITH_EXPR_OP_FDIV,   /**< @b Float division */
    ARITH_EXPR_OP_AND,    /**< @b Bitwise AND operation */
    ARITH_EXPR_OP_OR,     /**< @b Bitwise OR operation */
    ARITH_EXPR_OP_XOR,    /**< @b Bitwise XOR operation */
    ARITH_EXPR_OP_INV,    /**< @b Bitwise 2's complement */
    ARITH_EXPR_OP_LSHIFT, /**< @b Left shift operation */
    ARITH_EXPR_OP_RSHIFT, /**< @b Right shift operation */
    ARITH_EXPR_OP_MAX
} ArithExprOp;

/**
 * @b AST to represent language of arithmetic expressions.
 * */
struct ArithExpr {
    ExprOpnd    left_opnd;
    ArithExprOp op;
    ExprOpnd    right_opnd;
};

TO_ArithExpr* arith_expr_create (TO_ExprOpnd* left, ArithExprOp op, TO_ExprOpnd* right);
void          arith_expr_destroy (TO_ArithExpr* expr);

/**
 * @b Conditional expression operation.
 * */
typedef enum CondExprOp {
    COND_EXPR_OP_INVALID = 0,
    COND_EXPR_OP_EQ,  /**< @b Equality check. */
    COND_EXPR_OP_LE,  /**< @b Less than or equal to check. */
    COND_EXPR_OP_LT,  /**< @b Less than check. */
    COND_EXPR_OP_GE,  /**< @b Greater than or equal to check. */
    COND_EXPR_OP_GT,  /**< @b Greater than check. */
    COND_EXPR_OP_AND, /**< @b Logical AND operation. */
    COND_EXPR_OP_OR,  /**< @b Logical OR operation. */
    COND_EXPR_OP_MAX
} CondExprOp;

/**
 * @b AST to represent language to conditional expressions.
 *
 * If result of expression is positive then it's considered
 * to be true, otherfalse false.
 * */
struct CondExpr {
    ExprOpnd   left_opnd;
    CondExprOp op;
    ExprOpnd   right_opnd;
};

TO_CondExpr* cond_expr_create (TO_ExprOpnd* left, CondExprOp op, TO_ExprOpnd* right);
void         cond_expr_destroy (TO_CondExpr* expr);

/**
 * @b Type of field.
 * */
typedef enum FieldType {
    FIELD_TYPE_INVALID = 0,
    FIELD_TYPE_UINT8,
    FIELD_TYPE_UINT16,
    FIELD_TYPE_UINT32,
    FIELD_TYPE_UINT64,
    FIELD_TYPE_INT8,
    FIELD_TYPE_INT16,
    FIELD_TYPE_INT32,
    FIELD_TYPE_INT64,
    FIELD_TYPE_CHAR,
    FIELD_TYPE_CSTRING,
    FIELD_TYPE_STRUCT,
    FIELD_TYPE_UNION,
    FIELD_TYPE_MAX
} FieldType;

/**
 * @b Annotation information about field.
 * */
typedef enum FieldAnnotation {
    FIELD_ANNOTATION_NONE = 0,
    FIELD_ANNOTATION_VECTOR, /**< @b Annotated field is a vector of it's type. */
    FIELD_ANNOTATION_ARRAY,  /**< @b Annotated field is an array of it's type. */
    FIELD_ANNOTATION_MAX
} FieldAnnotation;

/**
 * @b Represents a field in a union or a struct.
 * Fields form a single linked list.
 * */
struct Field {
    FieldType field_type;

    /**
     * @b Type member field name.
     * Every field must have it's own unique name.
     * */
    CString field_name;

    /**
     * @b Condition on which this member exists. Null if always exists.
     * If this expression evaluates to true then this field exists.
     * */
    CondExpr* existence_cond;
    CString   doc_str; /**< @b If documentation is provied for this member. */

    FieldAnnotation field_annotation;
    union {
        ArithExpr* vec_size; /**< @b Expression to compute length of vector. */
        Size       arr_size; /**< @b Used if array. */
    } size;

    /**
     * @b Contains struct or union type name if this is a struct/union field.
     * The corresponding type loader will be found from TypeKnowledge and executed.
     * */
    CString type_name;

    Field* next; /**< Next in the list of fields. */
};

TO_Field* field_list_append (Field* head, TO_Field* next);
void      field_list_pprint (Field* head);
void      field_list_destroy (Field* head);

#endif
