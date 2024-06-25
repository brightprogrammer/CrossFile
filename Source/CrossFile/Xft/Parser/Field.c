/**
 * @file Field.c
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

/* local includes */
#include "Field.h"

/* libc */
#include <memory.h>

PRIVATE ExprOpnd* expr_opnd_deinit (ExprOpnd* opnd);

/**
 * @b Create a new arithmetic expression object.
 *
 * @param left Left operand (Transferred Ownership).
 * @param op operation.
 * @param right Right operand (Transferred Ownership).
 *
 * @return @c TO_ArithExpr* on success.
 * @return @c Null otherwise.
 * */
TO_ArithExpr* arith_expr_create (TO_ExprOpnd* left, ArithExprOp op, TO_ExprOpnd* right) {
    RETURN_VALUE_IF (!left || !op || !right, Null, ERR_INVALID_ARGUMENTS);

    ArithExpr* expr = NEW (ArithExpr);
    RETURN_VALUE_IF (!expr, Null, ERR_OUT_OF_MEMORY);

    memcpy (&expr->left_opnd, left, sizeof (ExprOpnd));
    expr->op = op;
    memcpy (&expr->right_opnd, right, sizeof (ExprOpnd));

    return expr;
}

/**
 * @b Take ownership of given arith expr object and destroy.
 *
 * @param expr
 * */
void arith_expr_destroy (TO_ArithExpr* expr) {
    RETURN_IF (!expr, ERR_INVALID_ARGUMENTS);

    expr_opnd_deinit (&expr->left_opnd);
    expr_opnd_deinit (&expr->right_opnd);
    expr->op = ARITH_EXPR_OP_INVALID;

    FREE (expr);
}

/**
 * @b Create a new conditional expression object.
 *
 * @param left Left operand (Transferred Ownership).
 * @param op operation.
 * @param right Right operand (Transferred Ownership).
 *
 * @return @c TO_CondExpr* on success.
 * @return @c Null otherwise.
 * */
TO_CondExpr* cond_expr_create (TO_ExprOpnd* left, CondExprOp op, TO_ExprOpnd* right) {
    RETURN_VALUE_IF (!left || !op || !right, Null, ERR_INVALID_ARGUMENTS);

    CondExpr* expr = NEW (CondExpr);
    RETURN_VALUE_IF (!expr, Null, ERR_OUT_OF_MEMORY);

    memcpy (&expr->left_opnd, left, sizeof (ExprOpnd));
    expr->op = op;
    memcpy (&expr->right_opnd, right, sizeof (ExprOpnd));

    return expr;
}

/**
 * @b Take ownership of given cond expr object and destroy.
 *
 * @param expr
 * */
void cond_expr_destroy (TO_CondExpr* expr) {
    RETURN_IF (!expr, ERR_INVALID_ARGUMENTS);

    expr_opnd_deinit (&expr->left_opnd);
    expr_opnd_deinit (&expr->right_opnd);
    expr->op = COND_EXPR_OP_INVALID;

    FREE (expr);
}

/**
 * @b Append given node to linked list of fields.
 *
 * The given next item is copied as is without any cloning of field members
 * or the object itself. This means when object is provided, the ownsership
 * is transferred to this list.
 *
 * Destroying the given object or any of it's members after append is UB.
 *
 * @param head Head of linked list.
 * @param next Next time to be inserted.
 *
 * @return Pointer to new inserted item on success.
 * @return Null otherwise.
 * */
Field* field_list_append (Field* head, TO_Field* next) {
    RETURN_VALUE_IF (!next, Null, ERR_INVALID_ARGUMENTS);

    /* create head if not present, or reset memory */
    if (!head) {
        head = NEW (Field);
        RETURN_VALUE_IF (!head, Null, ERR_OUT_OF_MEMORY);
    }

    /* find last element */
    Field* tail = head;
    while (tail->next) {
        tail = tail->next;
    }

    /* insert after last element */
    tail->next = NEW (Field);
    memcpy (tail->next, next, sizeof (Field));

    return head;
}

void field_list_pprint (Field* head) {
    // TODO: implement recursive method to automaticallty indent
    // We'll call that pprint_internal
}

/**
 * @b Destroy the complete linked list.
 *
 * @param head Head of linked list.
 * */
void field_list_destroy (TO_Field* head) {
    if (!head) {
        return;
    }

    if (head->existence_cond) {
        cond_expr_destroy (head->existence_cond);
    }

    if (head->doc_str) {
        FREE (head->doc_str);
    }

    if (head->type_name) {
        FREE (head->type_name);
    }

    if (head->size.vec_size) {
        switch (head->field_annotation) {
            case FIELD_ANNOTATION_ARRAY : {
                arith_expr_destroy (head->size.vec_size);
                break;
            }

            case FIELD_ANNOTATION_VECTOR : {
                break;
            }

            default : {
                PRINT_ERR (
                    "Invalid Field Annotation for Field = '%s'\n",
                    head->field_name ? head->field_name : "<INVALID>"
                );
                break;
            }
        }
    }

    if (head->field_name) {
        FREE (head->field_name);
    }

    if (head->next) {
        field_list_destroy (head->next);
    }

    memset (head, 0, sizeof (Field));
    FREE (head);
}

/**
 * @b Destroy contents of given @c ExprOpnd object.
 *
 * @param opnd ExprOpnd object to be de-initialized.
 *
 * @return @c opnd on success.
 * @return @c Null otherwise.
 * */
PRIVATE ExprOpnd* expr_opnd_deinit (ExprOpnd* opnd) {
    RETURN_VALUE_IF (!opnd, Null, ERR_INVALID_ARGUMENTS);

    switch (opnd->opnd_type) {
        case EXPR_OPND_TYPE_ARITH_EXPR : {
            if (opnd->arith_expr) {
                arith_expr_destroy (opnd->arith_expr);
            }
            break;
        }

        case EXPR_OPND_TYPE_COND_EXPR : {
            if (opnd->cond_expr) {
                cond_expr_destroy (opnd->cond_expr);
            }
            break;
        }

        default :
            break;
    }

    memset (opnd, 0, sizeof (ExprOpnd));
    return opnd;
}
