// File: xf_grammar.js
// Author: Siddharth Mishra (admin@brightprogrammer.in)
// Copyright: Copyright (c) 2024 All Rights Reserved.
// This file is a part of CrossFile.

// Comments are not allowed just anywhere, are allowed to be present only at selected
// places and are optional but recommended. They act as documentation for the formal
// description.
// They'll be used to add more detail in pprint methods. Comments must only be
// written associated with a field.

module.exports = grammar({
    name: 'xfile'

    rules: {
        source_file: $ => repeat ($._decl)

        _decl: $ => choice (
            $.enum_decl,
            $.struct_decl,
            $.file_decl,
            $.typedef_decl
        ),

        enum_decl: $ => seq (
            field ('doc', optional ($.comment)), 
            'enum',
            field ('name', $.id),
            ':',
            field ('type', $.type),
            '{',
            field ('body', repeat ($.enum_member)),
            '}'
        ),

        enum_member: $ => seq (
            field ('name', repeat($.id, optional(','))),
            '=',
            field ('value', choice($.number, $.string)),
            field ('doc', optional ($.comment)), 
        ),
        
        struct_decl: $ => seq (
            field ('doc', optional ($.comment)), 
            'struct',
            field ('name', $.id),
            '{', 
            field ('body', repeat ($.struct_member))
            '}',
            repeat (
                choice (
                    field ('assert', optional($.assert)),
                    field ('implicit', optional($.implicit))
                )
            )
        ),

        struct_member: $ => seq (
            field ('type', $.type),
            field ('name', $.id),
            field ('array_size', optional (seq ('[', $.expr, ']'))
            field ('addr', optional (
                seq ('(', optional ('$'), choice ('+', '-'), $.expr, ')'))
            ),
            field ('doc', optional ($.comment)), 
        ),

        file_decl: $ => seq (
            optional ($.comment),
            'file',
            $.id,
            '{',
            field ('body', repeat ($.struct_decl)),
            '}'
        ),

        // dependent types
        typedef_decl: $ => seq (
            'typedef',
            field ('name', $.id),
            field ('typeargs', $.typeargs),
            '{',
            repeat (seq ($.id, ':', $.id)
            '}'
        ),

        typeargs: $ => seq (
            '<', $.id, repeat (optional (','), $.id), '>',
        ),

        basic_type: $ => choice (
            'Bool',
            'Char',
            'Uint8',
            'Uint16',
            'Uint32',
            'Uint64',
            'Int8',
            'Int16',
            'Int32',
            'Int64',
            'Size',
            'CString'
        ),

        ptr_type: $ => seq ($.type, '*'),

        custom_type: $ => seq (
            field ('name', $.id),
            field ('typeargs', optional ($.typeargs))
        ),

        type: $ => choice (
            $.basic_type,
            $.custom_type,
            $.ptr_type
        ),

        assert : $ => seq (
            '@assert', '{',
            repeat ($.bool_expr),
            '}'
        ),
        
        implicit : $ => seq (
            '@assert', '{',
            repeat ($.struct_member),
            '}'
        ),

        expr: $ => choice(
            $.arith_expr,
            $.bitwise_expr,
            $.bool_expr,
            $.parenthesized_expr,
            $.id,
            $.number
        ),

        _binop_expr: ($, op) => seq(field('lopnd', $.expr), field('op', op), field('ropnd', $.expr)),
        _unop_expr: ($, op) => seq(field('op', op), field('opnd', $.expr)),

        arith_expr: $ => prec.left(10, choice(
            $._binop_expr($, '+'),
            $._binop_expr($, '-'),
            $._binop_expr($, '*'),
            $._binop_expr($, '/'),
            $._binop_expr($, '%')
        )),

        bitwise_expr: $ => prec.left(9, choice(
            $._binop_expr($, '&'),
            $._binop_expr($, '|'),
            $._binop_expr($, '^'),
            $._binop_expr($, '<<'),
            $._binop_expr($, '>>'),
            $._unop_expr($, '~')
        )),

        bool_expr: $ => prec.left(8, choice(
            $._unop_expr($, '!'),
            $._binop_expr($, '&&'),
            $._binop_expr($, '||'),
            $._binop_expr($, '=='),
            $._binop_expr($, '!='),
            $._binop_expr($, '<'),
            $._binop_expr($, '<='),
            $._binop_expr($, '>'),
            $._binop_expr($, '>=')
        )),

        parenthesized_expr: $ => seq(
          '(',
          $.expr,
          ')'
        ),

        precedences: $ => [
            { left: [ '&&', '||' ] },   // Logical operators
            { left: [ '==', '!=', '<', '<=', '>', '>=' ] },  // Comparison operators
            { left: [ '+', '-' ] },     // Addition and subtraction
            { left: [ '*', '/', '%' ] },// Multiplication, division, modulus
            { left: [ '&', '|', '^', '<<', '>>', '~' ] },  // Bitwise operators
            { left: [ '!' ] }           // Unary NOT
        ],

        // Ignore extras
        extras: $ => [
            /\s/,         // Matches whitespace
        ],
        
        id: $ => /[a-zA-Z_][a-zA-Z0-9_]*/,
        number: $ => /\d+/,
        string: $ => choice(/"([^"]*)"/, /'([^']*)'/),
        comment: $ => token(seq('/*', /[^*]*\*+([^/*][^*]*\*+)*/, '/')),
    }
})
