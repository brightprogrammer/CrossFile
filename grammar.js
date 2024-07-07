// File: xf_grammar.js
// Author: Siddharth Mishra(admin@brightprogrammer.in)
// Copyright: Copyright(c) 2024 All Rights Reserved.
// This file is a part of CrossFile.

// Comments are not allowed just anywhere, are allowed to be present only at selected
// places and are optional but recommended. They act as documentation for the formal
// description.
// They'll be used to add more detail in pprint methods. Comments must only be
// written associated with a field.

module.exports = grammar({
    name: 'xfile',

    // Ignore extras
    extras: _ => [
        /\s|\\\r?\n/
    ],

    rules: {
        source_file: $ => repeat($._decl),

        _decl: $ => choice(
            $.enum_decl,
            $.struct_decl,
            $.file_decl,
            $.typedef_decl
        ),

        enum_decl: $ => seq(
            field('doc', optional($.comment)),
            'enum',
            field('name', $.id),
            ':',
            field('type', $.type),
            '{',
            field('body', repeat($.enum_member)),
            '}'
        ),

        enum_member: $ => seq(
            field('name', seq($.id, repeat(seq(',', $.id)))),
            '=',
            field('value', choice($.num, $.range)),
            field('doc', optional($.comment))
        ),

        struct_decl: $ => seq(
            field('doc', optional($.comment)),
            'struct',
            field('name', $.id),
            '{',
            field('body', repeat($.struct_member)),
            field('assert', optional($.assert)),
            field('implicit', optional($.implicit)),
            '}'
        ),

        struct_member: $ => seq(
            field('type', $.type),
            field('name', $.id),
            field('array_size', optional(seq('[', $.expr, ']'))),
            field('addr',
                optional(
                    seq('(', optional('$'), choice('+', '-'), $.expr, ')')
                )
            ),
            field('doc', optional($.comment)),
        ),

        file_decl: $ => seq(
            field('doc', optional($.comment)),
            'file',
            $.id,
            '{',
            field('body', repeat($.struct_member)),
            '}'
        ),

        // dependent types
        typedef_decl: $ => seq(
            field('doc', optional($.comment)),
            'typedef',
            field('name', $.id),
            field('typeargs', $.typeargs),
            '{',
            repeat(seq($.id, ':', $.id)),
            '}'
        ),

        typeargs: $ => seq(
            '<', $.id, repeat(seq(',', $.id)), '>',
        ),

        basic_type: _ => choice(
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
            'CStr'
        ),

        ptr_type: $ => seq($.type, '*'),

        custom_type: $ => seq(
            field('name', $.id),
            field('typeargs', optional($.typeargs))
        ),

        type: $ => choice(
            $.basic_type,
            $.custom_type,
            $.ptr_type
        ),

        assert: $ => seq(
            '#assert', '{',
            repeat($.bool_expr),
            '}'
        ),

        implicit: $ => seq(
            '#implicit', '{',
            repeat($.struct_member),
            '}'
        ),

        expr: $ => choice(
            $.num,
            $.arr,
            $.id,
            $.arith_expr,
            $.bitwise_expr,
            $.bool_expr,
            $.parenthesized_expr
        ),

        arith_expr: $ => choice(
            prec.left(1, seq($.expr, '+', $.expr)),
            prec.left(1, seq($.expr, '-', $.expr)),
            prec.left(2, seq($.expr, '*', $.expr)),
            prec.left(2, seq($.expr, '/', $.expr)),
            prec.left(2, seq($.expr, '%', $.expr))
        ),

        bitwise_expr: $ => choice(
            prec.left(3, seq($.expr, '&', $.expr)),
            prec.left(3, seq($.expr, '|', $.expr)),
            prec.left(3, seq($.expr, '^', $.expr)),
            prec.left(3, seq($.expr, '<<', $.expr)),
            prec.left(3, seq($.expr, '>>', $.expr)),
            prec.right(3, seq('~', $.expr))
        ),

        bool_expr: $ => choice(
            prec.left(4, seq($.expr, '&&', $.expr)),
            prec.left(4, seq($.expr, '||', $.expr)),
            prec.left(5, seq($.expr, '==', $.expr)),
            prec.left(5, seq($.expr, '!=', $.expr)),
            prec.left(6, seq($.expr, '<', $.expr)),
            prec.left(6, seq($.expr, '>', $.expr)),
            prec.left(6, seq($.expr, '<=', $.expr)),
            prec.left(6, seq($.expr, '>=', $.expr)),
            prec.right(6, seq('!', $.expr))
        ),

        parenthesized_expr: $ => seq(
            '(',
            $.expr,
            ')'
        ),

        arr: $ => choice(
            seq('[', repeat($.char), ']'),
            seq('[', repeat($.num), ']'),
            seq('[', repeat($.str), ']'),
        ),

        range: $ => seq(field('range_begin', $.num), '..', field('range_end', $.num)),
        id: _ => /[a-zA-Z_][a-zA-Z0-9_]*(?:\.[a-zA-Z_][a-zA-Z0-9_]*)*/,

        _char: _ => /./,
        char: $ => $._char,

        _num: _ => choice(/\d+/, /0x[0-9a-fA-F]+/),
        num: $ => $._num,

        _str: _ => choice(/"([^"]*)"/, /'([^']*)'/),
        str: $ => $._str,

        comment: _ => token(seq('/*', /[^*]*\*+([^/*][^*]*\*+)*/, '/')),
    }
})
