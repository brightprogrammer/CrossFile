/**
 * @file XftVmStack.c
 * @date Mon, 10th June 2024
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

/* libc */
#include <memory.h>

/* local includes */
#include "Stack.h"

/**
 * @b Initialize type loader stack for a type loader to being execution.
 *
 * @param[out] stack @c XftVmStack object to be initialized.
 *
 * @return @c stack on success.
 * @return @c Null otherwise.
 * */
PUBLIC XftVmStack* xft_vm_stack_init (XftVmStack* stack) {
    RETURN_VALUE_IF (!stack, Null, ERR_INVALID_ARGUMENTS);

    /* if stack is not de-initialized and memory is already allocated then
     * we'll reuse it by resetting complete memory region */

    if (stack->stack_data && stack->stack_capacity) {
        memset (stack->stack_data, 0, stack->stack_capacity);
    } else {
        stack->stack_data = ALLOCATE (Uint8, 64);
        RETURN_VALUE_IF (!stack->stack_data, Null, ERR_OUT_OF_MEMORY);
        stack->stack_capacity = 64;
    }

    stack->stack_size = 0;

    return stack;
}

/**
 * @b De-initialize type loader stack.
 *
 * @param[out] stack @c XftVmStack object to be de-initialized.
 *
 * @return @c stack on success.
 * @return @c Null otherwise.
 * */
PUBLIC XftVmStack* xft_vm_stack_deinit (XftVmStack* stack) {
    RETURN_VALUE_IF (!stack, Null, ERR_INVALID_ARGUMENTS);

    if (stack->stack_data) {
        memset (stack->stack_data, 0, stack->stack_capacity);
        FREE (stack->stack_data);
    }

    /* invalidate all fields */
    memset (stack, 0, sizeof (XftVmStack));

    return stack;
}

/**
 * @b Check whether stack needs resizing and resize accordingly.
 *
 * If current capacity of stack is greater than given `new_size` of stack,
 * then it's not resized and stack object is returned without changing anything.
 * In the other case, the stack is resized to new_size.
 *
 * This method only resizes the stack upwards, i.e. to increase it's size.
 *
 * @param[in] stack
 * @param[in] new_size New size of stack (if resized) 
 *
 * @return @c stack on success.
 * @return @c Null otherwise.
 * */
PUBLIC XftVmStack* xft_vm_stack_resize_up (XftVmStack* stack, Size new_size) {
    RETURN_VALUE_IF (!stack || !new_size, Null, ERR_INVALID_ARGUMENTS);

    if (stack->stack_capacity < new_size) {
        Uint8* tmpstack_data = REALLOCATE (stack->stack_data, Uint8, new_size);
        RETURN_VALUE_IF (!tmpstack_data, Null, ERR_OUT_OF_MEMORY);
        stack->stack_data     = tmpstack_data;
        stack->stack_capacity = new_size;
    }

    return stack;
}

#define TLSTACK_CHECK_AND_RESIZE(stack)                                                            \
    if (stack->stack_size + 8 >= stack->stack_capacity) {                                          \
        xft_vm_stack_resize_up (stack, stack->stack_size ? stack->stack_size * 2 : 32);            \
    }

#define TLSTACK_PUSH_HELPER(n, stack, val)                                                         \
    RETURN_VALUE_IF (!stack, Null, ERR_INVALID_ARGUMENTS);                                         \
                                                                                                   \
    /* if stack needs resizing then resize */                                                      \
    TLSTACK_CHECK_AND_RESIZE (stack);                                                              \
                                                                                                   \
    /* push to stack based on size */                                                              \
    ((Uint##n*)(stack->stack_data + stack->stack_size))[0]  = val;                                 \
    stack->stack_size                                      += n >> 3;                              \
                                                                                                   \
    return stack

#define TLSTACK_PUSHFN_GEN(n)                                                                      \
    PUBLIC XftVmStack* xft_vm_stack_push_t##n (XftVmStack* stack, Uint##n val) {                   \
        TLSTACK_PUSH_HELPER (n, stack, val);                                                       \
    }

TLSTACK_PUSHFN_GEN (8);
TLSTACK_PUSHFN_GEN (16);
TLSTACK_PUSHFN_GEN (32);
TLSTACK_PUSHFN_GEN (64);

#undef TLSTACK_PUSH_HELPER
#undef TLSTACK_PUSHFN_GEN

#define TLSTACK_POP_HELPER(n, stack, val)                                                          \
    RETURN_VALUE_IF (!stack || !val, Null, ERR_INVALID_ARGUMENTS);                                 \
    RETURN_VALUE_IF (stack->stack_size < (n >> 3), Null, "XFT VM XftVmStack Underflow Error\n");   \
                                                                                                   \
    /* pop from stack based on size */                                                             \
    stack->stack_size -= n >> 3;                                                                   \
    *val               = ((Uint##n*)(stack->stack_data + stack->stack_size))[0];                   \
                                                                                                   \
    return stack

#define TLSTACK_POPFN_GEN(n)                                                                       \
    PUBLIC XftVmStack* xft_vm_stack_pop_t##n (XftVmStack* stack, Uint##n* val) {                   \
        TLSTACK_POP_HELPER (n, stack, val);                                                        \
    }

TLSTACK_POPFN_GEN (8);
TLSTACK_POPFN_GEN (16);
TLSTACK_POPFN_GEN (32);
TLSTACK_POPFN_GEN (64);

#undef TLSTACK_POP_HELPER
#undef TLSTACK_POPFN_GEN
#undef TLSTACK_CHECK_AND_RESIZE
