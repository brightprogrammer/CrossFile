# Structure Assembly (StAsm)

This note was created for a quick brainstorming session on a DSL for generating binary
file format parsers. You describe the structure shapes and compile it to generate a header
and source file to create an API for reading/writing/pretty-printing the binary file contents.

This file hints of how the end-product might look like. The syntax might completely change,
this is why it's a note (or an idea).

## Problems Faced

types of data :

- records (arrays where size of each entry varies)
   - array of polymorphic structures whose definition varies
      on preloaded data (eg: a version_no or type_info, etc...)
- arrays (all elements are of same size, size is known already)
- vectors (size is known while loading the file,
   - either through a field in the already loaded data or
   - through some computation performed on already loaded data)
   - this must be able to read both polymorphic types and fixed
     structures.

integrity constraints :

- some fields might have some constraints that need to be
  satisified while loading data (eg: checksum)

mappings :

- one-one mapping between fields
- one-many mapping?
- maany-one?
- many-many?

## Code Start

```
/**
 * The complete file can be referenced by using @FILE variable.
 * Any global variable starts with an @ and is always in caps.
 *
 * Shapes can reference themselves as well using @SELF variable.
 * This is only defined in the scope of that shape. This is explicitly
 * useful when trying to refer to a field inside struct rather than
 * a local alias or variable with same name.
 * */
@FILE.byte_order = 0x1337c0de;

/* Terminate all statements with an execlamation mark */

/**
 * There are three types of keywords for defining shapes :
 * - polymorph : To define structures that change their shape based on a value.
 * - shared : Analogous to C unions.
 * - fixed : Analogous to C structures with fixed shape.
 *
 * The most important difference among them all is that only fixed shapes can be read.
 * If a polymorph is to be read, it must be inside a struct or in the single global load
 * statement.
 * */


/**
 * There are two types of containers in StAsm.
 * - array : number of entries in the container is known before reading the file.
 * - vector : number of entries in the container is not known before reading and
 *           is computed by providing a compute method in the shape definition or
 *           by providing an "of size N" statement at the end of vector field.
 * */

/**
 * A quick example I can remember of a polymorph is some structure in java class file.
 * The spec states that there are tables throughout the file and tables are not the same as arrays.
 * An array always has all elements of same size, but in a table, elements may be of varying sizes.
 * */
polymorph XfJvmRecord {
    /* there can be only one variable on which morph can be done. */
    morph on Uint32 version;

    /* by default referring to fields in same structs don't require @SELF */

    /**
     * For morphing, one can inline the shapes or can implement them somewhere else.
     * Here I'm assuming these are either morph, or shared or a fixed type.
     * */
    morph to XfJvmClassFileInfo as class_file_info if version is 0xcafebabe;
    morph to XfJvmMethodInfo    as method_info     if version is 0xbaadf00d;
    morph to XfJvmModuleInfo    as module_info     if version is 0xabcdabcd;

    /* if version is none of those values above, then parser will throw an error messages */

    /* these assertions are reduntant because version is a morph variable. */
    assert version is not 0;
    assert version is less than UINT32_MAX;
};


/* shared types can be defined separately */
shared OffsetOrSize {
    /* field inside shared type are separated by "or" or can be written like C unions */
    Uint32 offset;
    Size size;
};

/* size and shape of structure is fixed */
fixed XfOtfTableRecord {
    /* shared types can be inlined as well! but only with the use of "or" */
    Uint32           tag_value or
    array of Char as tag_chars of size 4;

    Uint32   checksum;
    Offset32 offset;
    Size32   size;

    assert size is not 0;
    assert tag.tag_value is not 0
}

fixed XfOtfTableDir {
    Uint32 sfnt_version;
    Uint16 num_records;
    Uint16 search_range;
    Uint16 entry_selector;
    Uint16 range_shift;

    /* instead of providing a compute, we can specify size for automatic loading */
    vector of XfOtfTableRecord as records of size num_records;

    assert sfnt_version is either 0x00010000 or 0x4f54544f;
    assert num_records is greater than 0;
};

/**
 * bitmaps and enums take a base type specifier to deduce their size.
 * */

bitmap of Uint16 as XfOtfHeadFlags {
    FLAG1 = 0;
    FLAG2 = 1;
    FLAG3 = 2;
    FLAG4 = 3;

    /* some extra data to be used to pprint this bitmap or enum */
    FLAG1.str = "Something cool about flag1";
    FLAG3.str = "Something better about flag3";
};

enum of Uint16 as XfOtfLanguageId {
    ENGLISH = 0;

    ENGLISH.str = "English (en)"
};

fixed XfOtfHead {
    Uint16 major_version;
    Uint16 minor_version;
    Uint32 font_revision;
    Uint32 checksum_adjustment;
    Uint32 magic_number;

    assert magic_number is OTF_MAGIC_NUMBER;
};

fixed XfOtfFile {
    XfOtfTableDir table_dir;

    XfOtfHead head;
    XfOtfCmap cmap;
    .
    . (many more fields like this continue)
    .

    compute each record in table_dir.records {
        /* we could've read the table_dir here as well for a temporary variable */
        /* read table_dir from @FILE.data_begin */
        switch on record.tag_chars {
            case 'head' : {read head from (@FILE.data_begin + record.offset) of size record.length};
            case 'cmap' : {read head from (@FILE.data_begin + record.offset) of size record.length};
            .
            . (many more cases continue like this)
            .
        }
    }
};

/* There must be only one statement in the global scope.
 * This will define which structure actually represents the shape of file.
 * */
read XfOtfFile;
```

## Grammar

"{}" means repeat any number of times.
"()" means repeat atleast once
"[]" means optional.

Grammar is work in progress. It's in EBNF format.

```ebnf
# any type of whitespace
S = {" " | "\t" | "\n" | "\b" | "\f"};

# accessed field like table_dir.records.count
# this is more generalized form of a general field name
accessed_field = id, {".", id};

# rename an accessed field or any variable
# this rename rule only directly applies to inline rename
rename = accessed_field, [S, "as", S, id];

# machine for mathematical expressions
digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
number = digit, {digit};
expr_in_brackets = "(", S, expr, S, ")";
expr_term = number | accessed_field | expr_in_brackets;
expr_factor = expr_term, {S, "*" | "/", S, expr_term};
arith_expr = expr_factor, {S, "+" | "-", S, expr_factor};
bool_binexpr = arith_expr, {S, "or" | "and" | "xor", "left shift", "right shift", S, arith_expr};
bool_unexpr = "not", S, bool_binexpr;
expr = arith_expr | bool_binexpr | bool_unexpr;

base_type = "Uint8" | "Uint16" | "Uint32" | "Uint64"
          | "Int8"  | "Int16"  | "Int32"  | "Int64"
          | "CString";
type = base_type | id;

# different type of comparisions
cmp_eq  = expr, S,    "is", [S, "equal to"],        S, expr;
cmp_le  = expr, S,    "is less or equal to",        S, expr;
cmp_lt  = expr, S,    "is less than",               S, expr;
cmp_ge  = expr, S,    "is greater or equal to",     S, expr;
cmp_gt  = expr, S,    "is greater than",            S, expr;
cmp_neq = expr, S,    "is not", [S, "equal to"],    S, expr;
cmp_nle = expr, S,    "is not less or equal to",    S, expr;
cmp_nlt = expr, S,    "is not less than",           S, expr;
cmp_nge = expr, S,    "is not greater or equal to", S, expr;
cmp_ngt = expr, S,    "is not greater than",        S, expr;

cmp    = cmp_eq  | cmp_le  | cmp_lt  | cmp_ge  | cmp_gt
       | cmp_neq | cmp_nle | cmp_nlt | cmp_nge | cmp_ngt;

# code block is a sequence of instructions in the brackets
code_block = "{", S, (insn | switch_block | for_each_block | if_then_else_block, S), "}";
case_block = "case", S, expr, S, ":" code_block;
switch_block = "switch on", S, expr, S, "{", S, (case_block, S), "}";
for_each_block = "for each", S, id, S, "in", S, rename, S, code_block;
if_then_else_block = "if", S, cmp, S, "then", S, code_block, [S, "else", code_block];

# different types of instructions
insn_break_cont = "break" | "continue";
insn_assert = "assert", S, cmp;
insn_using = "using", S, rename;
insn_read = "read", S, id, S, "from", S, expr, [S, "of size", S, expr];
insn_assign = accessed_field, S, "=", S, expr;
insn = insn_break_cont | insn_assert | insn_using | insn_read | insn_assign, S, ";";

arglist = "(", S, {rename, S, [","]}, S, ")"
compute_single = "compute", S, id, S, arglist, S, compute_block;

compute_each = "compute", S, for_each;

fixed = "fixed", S, id, S, "{",
            (S, type, S, id, S, ";", S),
        "}", S, ";";
```
