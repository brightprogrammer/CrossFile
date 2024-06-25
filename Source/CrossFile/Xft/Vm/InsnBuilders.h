/**
 * @file InsnBuilders.h
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
 *
 * @bried Defines some macros for east initialization of instructions
 * on the stack.
 * */


#ifndef ANVIE_SOURCE_CROSSFILE_INSN_BUILDERS_H
#define ANVIE_SOURCE_CROSSFILE_INSN_BUILDERS_H

#define SET_REG(reg, imm) ((Insn) {.insn_type = INSN_TYPE_SET_REG, .insn = {.set_reg = {reg, imm}}})




#define READ_REG(b, reg) ((Insn) {.insn_type = INSN_TYPE_READ_R##b, .insn = {.read_reg = {reg}}})
#define READ_REG8(reg)   READ_REG (8, reg)
#define READ_REG16(reg)  READ_REG (16, reg)
#define READ_REG32(reg)  READ_REG (32, reg)
#define READ_REG64(reg)  READ_REG (64, reg)

#define PUSH_REG(b, reg) ((Insn) {.insn_type = INSN_TYPE_PUSH_R##b, .insn = {.push_reg = {reg}}})
#define PUSH_REG8(reg)   PUSH_REG (8, reg)
#define PUSH_REG16(reg)  PUSH_REG (16, reg)
#define PUSH_REG32(reg)  PUSH_REG (32, reg)
#define PUSH_REG64(reg)  PUSH_REG (64, reg)

#define POP_REG(b, reg) ((Insn) {.insn_type = INSN_TYPE_POP_R##b, .insn = {.pop_reg = {reg}}})
#define POP_REG8(reg)   POP_REG (8, reg)
#define POP_REG16(reg)  POP_REG (16, reg)
#define POP_REG32(reg)  POP_REG (32, reg)
#define POP_REG64(reg)  POP_REG (64, reg)




#define READ_MEM(b, mem) ((Insn) {.insn_type = INSN_TYPE_READ_M##b, .insn = {.read_mem = {mem}}})
#define READ_MEM8(mem)   READ_MEM (8, mem)
#define READ_MEM16(mem)  READ_MEM (16, mem)
#define READ_MEM32(mem)  READ_MEM (32, mem)
#define READ_MEM64(mem)  READ_MEM (64, mem)

#define PUSH_MEM(b, mem) ((Insn) {.insn_type = INSN_TYPE_PUSH_M##b, .insn = {.push_mem = {mem}}})
#define PUSH_MEM8(mem)   PUSH_MEM (8, mem)
#define PUSH_MEM16(mem)  PUSH_MEM (16, mem)
#define PUSH_MEM32(mem)  PUSH_MEM (32, mem)
#define PUSH_MEM64(mem)  PUSH_MEM (64, mem)

#define POP_MEM(b, mem) ((Insn) {.insn_type = INSN_TYPE_POP_M##b, .insn = {.pop_mem = {mem}}})
#define POP_MEM8(mem)   POP_MEM (8, mem)
#define POP_MEM16(mem)  POP_MEM (16, mem)
#define POP_MEM32(mem)  POP_MEM (32, mem)
#define POP_MEM64(mem)  POP_MEM (64, mem)




#define READ_ARR(b, mem_off, elem_count)                                                           \
    ((Insn) {.insn_type = INSN_TYPE_READ_A##b, .insn = {.read_arr = {mem_off, elem_count}}})
#define READ_ARR8(mem_off, elem_count)  READ_ARR (8, mem_off, elem_count)
#define READ_ARR16(mem_off, elem_count) READ_ARR (16, mem_off, elem_count)
#define READ_ARR32(mem_off, elem_count) READ_ARR (32, mem_off, elem_count)
#define READ_ARR64(mem_off, elem_count) READ_ARR (64, mem_off, elem_count)

#define PUSH_ARR(b, mem_off, elem_count)                                                           \
    ((Insn) {.insn_type = INSN_TYPE_PUSH_A##b, .insn = {.push_arr = {mem_off, elem_count}}})
#define PUSH_ARR8(mem_off, elem_count)  PUSH_ARR (8, mem_off, elem_count)
#define PUSH_ARR16(mem_off, elem_count) PUSH_ARR (16, mem_off, elem_count)
#define PUSH_ARR32(mem_off, elem_count) PUSH_ARR (32, mem_off, elem_count)
#define PUSH_ARR64(mem_off, elem_count) PUSH_ARR (64, mem_off, elem_count)

#define POP_ARR(b, mem_off, elem_count)                                                            \
    ((Insn) {.insn_type = INSN_TYPE_POP_A##b, .insn = {.pop_arr = {mem_off, elem_count}}})
#define POP_ARR8(mem_off, elem_count)  POP_ARR (8, mem_off, elem_count)
#define POP_ARR16(mem_off, elem_count) POP_ARR (16, mem_off, elem_count)
#define POP_ARR32(mem_off, elem_count) POP_ARR (32, mem_off, elem_count)
#define POP_ARR64(mem_off, elem_count) POP_ARR (64, mem_off, elem_count)




#define SEEK(dir, numb) ((Insn) {.insn_type = INSN_TYPE_SEEK_##dir, .insn = {.seek = {numb}}})
#define SEEK_FWD(numb)  SEEK (FWD, numb)
#define SEEK_BAK(numb)  SEEK (BAK, numb)




#define BINOP(type, rres, r1, r2) ((Insn) {.insn_type = type, .insn = {.binop = {rres, r1, r2}}})
#define ADD(rres, r1, r2)         BINOP (INSN_TYPE_ADD, rres, r1, r2)
#define SUB(rres, r1, r2)         BINOP (INSN_TYPE_SUB, rres, r1, r2)
#define MUL(rres, r1, r2)         BINOP (INSN_TYPE_MUL, rres, r1, r2)
#define DIV(rres, r1, r2)         BINOP (INSN_TYPE_DIV, rres, r1, r2)
#define MOD(rres, r1, r2)         BINOP (INSN_TYPE_MOD, rres, r1, r2)
#define POW(rres, r1, r2)         BINOP (INSN_TYPE_POW, rres, r1, r2)
#define AND(rres, r1, r2)         BINOP (INSN_TYPE_AND, rres, r1, r2)
#define OR(rres, r1, r2)          BINOP (INSN_TYPE_OR, rres, r1, r2)
#define XOR(rres, r1, r2)         BINOP (INSN_TYPE_XOR, rres, r1, r2)
#define NAND(rres, r1, r2)        BINOP (INSN_TYPE_NAND, rres, r1, r2)
#define NOR(rres, r1, r2)         BINOP (INSN_TYPE_NOR, rres, r1, r2)
#define XNOR(rres, r1, r2)        BINOP (INSN_TYPE_XOR, rres, r1, r2)
#define LSHIFT(rres, r1, r2)      BINOP (INSN_TYPE_LSHIFT, rres, r1, r2)
#define RSHIFT(rres, r1, r2)      BINOP (INSN_TYPE_RSHIFT, rres, r1, r2)
#define ROL(rres, r1, r2)         BINOP (INSN_TYPE_ROL, rres, r1, r2)
#define ROR(rres, r1, r2)         BINOP (INSN_TYPE_ROR, rres, r1, r2)
#define CMPEQ(rres, r1, r2)       BINOP (INSN_TYPE_CMPEQ, rres, r1, r2)
#define CMPLE(rres, r1, r2)       BINOP (INSN_TYPE_CMPLE, rres, r1, r2)
#define CMPLT(rres, r1, r2)       BINOP (INSN_TYPE_CMPLT, rres, r1, r2)
#define CMPGE(rres, r1, r2)       BINOP (INSN_TYPE_CMPGE, rres, r1, r2)
#define CMPGT(rres, r1, r2)       BINOP (INSN_TYPE_CMPGT, rres, r1, r2)




#define UNOP(type, rres, r1) ((Insn) {.insn_type = type, .insn = {.unop = {rres, r1}}})
#define SQRT(rres, r1)       UNOP (INSN_TYPE_SQRT, rres, r1)
#define ABS(rres, r1)        UNOP (INSN_TYPE_ABS, rres, r1)
#define NOT(rres, r1)        UNOP (INSN_TYPE_NOT, rres, r1)




#define JMP(cond, reg, sel) ((Insn) {.insn_type = INSN_TYPE_J##cond, .insn = {.jmp = {reg, sel}}})
#define JA(reg, sel)        JMP (A, reg, sel)
#define JB(reg, sel)        JMP (B, reg, sel)
#define JZ(reg, sel)        JMP (Z, reg, sel)
#define JO(reg, sel)        JMP (O, reg, sel)
#define JC(reg, sel)        JMP (C, reg, sel)




#define CALL(sel)                                                                                  \
    ((Insn) {.insn_type = INSN_TYPE_CALL_TYPE_LOADER, .insn = {.call_type_loader = {sel}}})

#endif // ANVIE_SOURCE_CROSSFILE_INSN_BUILDERS_H
