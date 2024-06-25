/**
 * @file Vm.c
 * @date Sun, 9th June 2024
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
 * @brief @c XfStructDesc can be used to describe structured data streams and how to read them.
 * It also provides a few methods to pretty print these structs. Basic types are already defined
 * and can be used directly but complex types needs to be defined by the user.
 *
 * @c XfStruct can recursively define a structured format. Recursive here means a struct containing
 * a struct that contains another struct and so on as many times as needed. Each struct contains a set
 * of field descriptors and the field descriptor contains information such as where to write the data,
 * how to read the data (endianness), at what offset to read the data, whether data is an array of an
 * element type or it's a single element. Dynamic arrays are also possible.
 * */

/* libc */
#include <memory.h>

/* local includes */
#include "InsnBuilders.h"
#include "TypeLoaderStack.h"
#include "Vm.h"

/**
 * @b Initialize VM for execution.
 *
 * If `init` is called multiple times on a vm without calling `deinit`, then
 * init will make an attempt to reuse the already allocated memory. It won't
 * free any memory and reallocate only if required.
 *
 * @param[out] vm Vm object to be initialized for execution.
 *
 * @return @c vm on success.
 * @return @c Null otherwise.
 * */
PRIVATE Vm* vm_init (Vm* vm) {
    RETURN_VALUE_IF (!vm, Null, ERR_INVALID_ARGUMENTS);

    /* if init is being called on vm again with previous stack not deinitialized
     * then we can re-use the already allocated stacks. */

    if (!vm->loader_stacks || !vm->loader_stack_capacity) {
        vm->loader_stacks = ALLOCATE (TypeLoaderStack, 32);
        RETURN_VALUE_IF (!vm->loader_stack_capacity, Null, ERR_OUT_OF_MEMORY);
        vm->loader_stack_capacity = 32;
    }

    vm->loader_stack_count = 0;

    return vm;
}

/**
 * @b De-initialize given @c Vm object.
 *
 * @param[out] vm @c Vm object to be de-initialized.
 *
 * @return @c vm on success.
 * @return @c Null otherwise.
 * */
PRIVATE Vm* vm_deinit (Vm* vm) {
    RETURN_VALUE_IF (!vm, Null, ERR_INVALID_ARGUMENTS);

    if (vm->loader_stacks) {
        for (Size s = 0; s < vm->loader_stack_count; s++) {
            type_loader_stack_deinit (vm->loader_stacks + s);
        }

        FREE (vm->loader_stacks);
    }

    memset (vm, 0, sizeof (Vm));

    return vm;
}
