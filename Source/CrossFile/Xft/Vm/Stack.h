/**
 * @file XftVmStack.h
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

#ifndef ANVIE_SOURCE_CROSSFILE_XFT_VM_STACK_H
#define ANVIE_SOURCE_CROSSFILE_XFT_VM_STACK_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/**
 * @b XftVmStack is allocated and maintained by the VM.
 * Each method/function get's a separate stack.
 * */
typedef struct XftVmStack {
    Uint8* stack_data;
    Size   stack_size;
    Size   stack_capacity;
} XftVmStack;

PUBLIC XftVmStack* xft_vm_stack_init (XftVmStack* stack);
PUBLIC XftVmStack* xft_vm_stack_deinit (XftVmStack* stack);
PUBLIC XftVmStack* xft_vm_stack_check_up (XftVmStack* stack, Size new_size);
PUBLIC XftVmStack* xft_vm_stack_push_t8 (XftVmStack* stack, Uint8 val);
PUBLIC XftVmStack* xft_vm_stack_push_t16 (XftVmStack* stack, Uint16 val);
PUBLIC XftVmStack* xft_vm_stack_push_t32 (XftVmStack* stack, Uint32 val);
PUBLIC XftVmStack* xft_vm_stack_push_t64 (XftVmStack* stack, Uint64 val);
PUBLIC XftVmStack* xft_vm_stack_pop_t8 (XftVmStack* stack, Uint8* val);
PUBLIC XftVmStack* xft_vm_stack_pop_t16 (XftVmStack* stack, Uint16* val);
PUBLIC XftVmStack* xft_vm_stack_pop_t32 (XftVmStack* stack, Uint32* val);
PUBLIC XftVmStack* xft_vm_stack_pop_t64 (XftVmStack* stack, Uint64* val);

#endif // ANVIE_SOURCE_CROSSFILE_XFT_VM_STACK_H
