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


#ifndef ANVIE_SOURCE_CROSSFILE_XFT_H
#define ANVIE_SOURCE_CROSSFILE_XFT_H

#include <Anvie/CrossFile/Xft.h>
#include <Anvie/Types.h>

typedef enum InsnType : Uint8 {
    INSN_TYPE_UNKNOWN = 0,

    INSN_TYPE_SET_REG, /* setr reg, imm */

    /* read data of different sizes fromd data stream to given register */
    INSN_TYPE_READ_R8,  /* rd8 reg */
    INSN_TYPE_READ_R16, /* rd16 reg */
    INSN_TYPE_READ_R32, /* rd32 reg */
    INSN_TYPE_READ_R64, /* rd64 reg */

    /* read data of different sizes form data stream to given memory offset */
    INSN_TYPE_READ_M8,  /* rd8 mem_off */
    INSN_TYPE_READ_M16, /* rd16 mem_off */
    INSN_TYPE_READ_M32, /* rd32 mem_off */
    INSN_TYPE_READ_M64, /* rd64 mem_off */

    /* push data onto stack from given register */
    INSN_TYPE_PUSH_R8,  /* push8 reg */
    INSN_TYPE_PUSH_R16, /* push16 reg */
    INSN_TYPE_PUSH_R32, /* push32 reg */
    INSN_TYPE_PUSH_R64, /* push64 reg */

    /* push data onto stack from given memory offset */
    INSN_TYPE_PUSH_M8,  /* push8 mem_off */
    INSN_TYPE_PUSH_M16, /* push16 mem_off */
    INSN_TYPE_PUSH_M32, /* push32 mem_off */
    INSN_TYPE_PUSH_M64, /* push64 mem_off */

    /* pop data from stack to given register */
    INSN_TYPE_POP_R8,  /* pop8 mem_off */
    INSN_TYPE_POP_R16, /* pop16 mem_off */
    INSN_TYPE_POP_R32, /* pop32 mem_off */
    INSN_TYPE_POP_R64, /* pop64 mem_off */

    /* pop data from stack to given memory offset */
    INSN_TYPE_POP_M8,  /* pop8 mem_off */
    INSN_TYPE_POP_M16, /* pop16 mem_off */
    INSN_TYPE_POP_M32, /* pop32 mem_off */
    INSN_TYPE_POP_M64, /* pop64 mem_off */

    /* read an array of data of different sizes form data stream */
    INSN_TYPE_READ_A8,  /* ra8 mem_off, read_elem_count */
    INSN_TYPE_READ_A16, /* ra16 mem_off, read_elem_count */
    INSN_TYPE_READ_A32, /* ra32 mem_off, read_elem_count */
    INSN_TYPE_READ_A64, /* ra64 mem_off, read_elem_count */

    /* push arrayt of data onto stack from given memory offset */
    INSN_TYPE_PUSH_A8,  /* pusha8 mem_off, push_elem_count */
    INSN_TYPE_PUSH_A16, /* pusha16 mem_off, push_elem_count */
    INSN_TYPE_PUSH_A32, /* pusha32 mem_off, push_elem_count */
    INSN_TYPE_PUSH_A64, /* pusha64 mem_off, push_elem_count */

    /* pop array of data from stack to given memory offset */
    INSN_TYPE_POP_A8,  /* popa8 mem_off, pop_elem_count */
    INSN_TYPE_POP_A16, /* popa16 mem_off, pop_elem_count */
    INSN_TYPE_POP_A32, /* popa32 mem_off, pop_elem_count */
    INSN_TYPE_POP_A64, /* popa64 mem_off, pop_elem_count */

    /* seek instructions inside the file stream */
    INSN_TYPE_SEEK_FWD, /* seekf num_bytes */
    INSN_TYPE_SEEK_BAK, /* seekb num_bytes */

    /* operations on registers */
    /* arithmetic operators */
    INSN_TYPE_ADD,  /* add rres, r1, r2 */
    INSN_TYPE_SUB,  /* sub rres, r1, r2 */
    INSN_TYPE_MUL,  /* mul rres, r1, r2 */
    INSN_TYPE_DIV,  /* div rres, r1, r2 */
    INSN_TYPE_MOD,  /* mod rres, r1, r2 */
    INSN_TYPE_POW,  /* mod rres, r1, r2 */
    INSN_TYPE_SQRT, /* mod rres, r1 */
    INSN_TYPE_ABS,  /* mod rres, r1 */
    /* logical operators */
    INSN_TYPE_AND,  /* and rres, r1, r2 */
    INSN_TYPE_OR,   /* or rres, r1, r2 */
    INSN_TYPE_XOR,  /* xor rres, r1, r2 */
    INSN_TYPE_NAND, /* nand rres, r1, r2 */
    INSN_TYPE_NOR,  /* nor rres, r1, r2 */
    INSN_TYPE_XNOR, /* xnor rres, r1, r2 */
    INSN_TYPE_NOT,  /* not rres, r1 */
    /* bitwise operators */
    INSN_TYPE_LSHIFT, /* lshift rres, r1, r2 */
    INSN_TYPE_RSHIFT, /* rshift rres, r1, r2 */
    INSN_TYPE_ROL,    /* rshift rres, r1, r2 */
    INSN_TYPE_ROR,    /* rshift rres, r1, r2 */
    /* comparision operators */
    INSN_TYPE_CMPEQ, /* cmpeq rres, r1, r2 */
    INSN_TYPE_CMPLE, /* cmple rres, r1, r2 */
    INSN_TYPE_CMPLT, /* cmplt rres, r1, r2 */
    INSN_TYPE_CMPGE, /* cmpge rres, r1, r2 */
    INSN_TYPE_CMPGT, /* cmpgt rres, r1, r2 */

    /* control flow instructions */
    /* jumps happen by selecting the block from an array of blocks that the VM has */
    INSN_TYPE_JA,               /* ja reg, sel : jump if above */
    INSN_TYPE_JB,               /* jb reg, sel : jump if below */
    INSN_TYPE_JZ,               /* jz reg, sel : jump if zero */
    INSN_TYPE_JO,               /* jo reg, sel : jump if overflow */
    INSN_TYPE_JC,               /* jc reg, sel : jump if carry */

    INSN_TYPE_CALL_TYPE_LOADER, /* typeload typesel: Call loader method */

    /* printing and debugging infos */
    INSN_TYPE_PINFO,        /* pinfo, str : Just some informative message */
    INSN_TYPE_PDBG,         /* pdbg, str : When generated code thinks it might be wrong */
    INSN_TYPE_PERR,         /* perr, str : When generated code thinks it is wrong */
    INSN_TYPE_EXIT_SUCCESS, /* exit_success */
    INSN_TYPE_EXIT_FAILURE, /* exit_failure*/

    INSN_TYPE_MAX
} InsnType;

typedef struct Insn {
    InsnType insn_type;

    union {
        struct {
            Uint8  reg;
            Uint64 imm;
        } set_reg;

        struct {
            Uint8 reg;
        } read_reg, push_reg, pop_reg;

        struct {
            Uint64 mem_off;
        } read_mem, push_mem, pop_mem;

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
            cmpeq, cmple, cmplt, cmpge, cmpgt, binop;

        struct {
            Uint8 rres; /**< @b Register where result of op will be stored */
            Uint8 r1;   /**< @b The one and only operand */
        } sqrt, abs, not, unop;

        struct {
            Uint8 reg;       /**< @b Register which contains the result of comparision. */
            Size  block_sel; /**< @b Index of block to jump to. */
        } ja, jb, jz, jo, jc, jmp;

        struct {
            Size type_load_sel; /**< @b Index of type loader to be called. */
        } call_type_loader;
    } insn;
} Insn;


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
typedef struct InsnBlock {
    Insn* insns;
    Size  insn_count;
    Size  insn_capacity;
} InsnBlock;

static inline InsnBlock* insn_block_init (InsnBlock* iblock);
static inline InsnBlock* insn_block_deinit (InsnBlock* iblock);
static inline InsnBlock* insn_block_add_insn (InsnBlock* iblock, Insn* insn);

/**
 * @b Type loaders are like methods that get called when VM wants to load
 * another type.
 *
 * Each type laoder stores reference to all the type loader that it can
 * call in future. This is just for quick finding of type loader, because a file
 * may have many types and finding the loader in all those arrays "may" be slow.
 * */
typedef XftTypeLoader TypeLoader;
struct XftTypeLoader {
    CString type_name;             /**< @b Name of type (same as struct name/typedef) */
    CString type_doc;              /**< @b Description of given type */
    Size    alloc_size;            /**< @b Allocation size for a single element of this struct */

    InsnBlock insn_blocks;         /**< @b All the basic blocks in this type loader */
    Size      insn_block_count;    /**< @b Number of basic blocks in this loader. */
    Size      insn_block_capacity; /**< @b Capacity of insn_blocks array */

    TypeLoader** type_loader_refs; /**< @b References to all type loaders this loader can call. */
    Size         type_loader_ref_count;    /**< @b Number of references in the references array */
    Size         type_loader_ref_capacity; /**< @b Capacity of references array */
};

/**
 * @b One needs to create a file loader to load the whole file.
 * 
 * A file loader contains information about all the types and type laoders,
 * that are required to load the file.
 * */
typedef XftFileLoader FileLoader;
struct XftFileLoader {
    Size main_type_loader_index;

    TypeLoader* type_loaders;         /**< @b Vector of all type loaders. */
    Size        type_loader_count;    /**< @b Number of type loaders */
    Size        type_loader_capacity; /**< @b Maximum capacity of type loader vector */
};

static inline FileLoader* file_loader_init (FileLoader* floader);
static inline FileLoader* file_loader_deinit (FileLoader* floader);
static inline TypeLoader* file_loader_create_new_type_loader (FileLoader* floader);

/**
 * @b Stack is allocated and maintained by the VM.
 * Each method/function get's a separate stack.
 * */
typedef struct TypeLoaderStack {
    Uint8* stack_data;
    Size   stack_size;
    Size   stack_capacity;
} TypeLoaderStack;

static inline TypeLoaderStack* type_loader_stack_init (TypeLoaderStack* stack);
static inline TypeLoaderStack* type_loader_stack_deinit (TypeLoaderStack* stack);
static inline TypeLoaderStack* type_loader_stack_push_t8 (TypeLoaderStack* stack, Uint8 val);
static inline TypeLoaderStack* type_loader_stack_push_t16 (TypeLoaderStack* stack, Uint16 val);
static inline TypeLoaderStack* type_loader_stack_push_t32 (TypeLoaderStack* stack, Uint32 val);
static inline TypeLoaderStack* type_loader_stack_push_t64 (TypeLoaderStack* stack, Uint64 val);
static inline TypeLoaderStack* type_loader_stack_pop_t8 (TypeLoaderStack* stack, Uint8* val);
static inline TypeLoaderStack* type_loader_stack_pop_t16 (TypeLoaderStack* stack, Uint16* val);
static inline TypeLoaderStack* type_loader_stack_pop_t32 (TypeLoaderStack* stack, Uint32* val);
static inline TypeLoaderStack* type_loader_stack_pop_t64 (TypeLoaderStack* stack, Uint64* val);

#endif // ANVIE_SOURCE_CROSSFILE_XFT_H
