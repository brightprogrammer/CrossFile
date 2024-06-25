/**
 * @file Vm.h
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EPRESS OR
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


#ifndef ANVIE_SOURCE_CROSSFILE_XFT_H
#define ANVIE_SOURCE_CROSSFILE_XFT_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* proper renaming to make sure this is comptible with public opaque declaration */
typedef struct XftVm XftVm;
typedef XftVm        Vm;

/* this is never exposed publicly so we can direcly typedef it */
typedef struct TypeLoaderStack TypeLoaderStack;

/* number of registers in crossfile type vm */
#define VM_REG_COUNT 8

/**
 * @b The CrossFile Type VM.
 * */
struct XftVm {
    Uint64 regs[VM_REG_COUNT];

    Uint64 pc;                              /**< Program counter. */

    TypeLoaderStack* loader_stacks;         /**< @b A vector of stacks for type loaders */
    Size             loader_stack_count;    /**< @b Number of stacks. */
    Size             loader_stack_capacity; /**< @b Capacity of stacks vector */
};

PRIVATE Vm* vm_init (Vm* vm);
PRIVATE Vm* vm_deinit (Vm* vm);

#endif // ANVIE_SOURCE_CROSSFILE_XFT_H
