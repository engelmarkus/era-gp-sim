/*
* C++ Assembler Interpreter
* Copyright (C) 2016 Chair of Computer Architecture
* at Technical University of Munich
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ERAGPSIM_PARSER_EXPRESSION_COMPILER_CLIKE_HPP
#define ERAGPSIM_PARSER_EXPRESSION_COMPILER_CLIKE_HPP

#include "expression-compiler.hpp"
#include "string-parser.hpp"

/**
 * \brief Provides some basic expression compilers.
 */
namespace CLikeExpressionCompilers
{
    //Temporary parsing method, copied from the string parser, sightly modified.
    template <typename T>
    static T simpleNumberParse(const std::string& inputString,
                                size_t start,
                                int base) {
        T value = 0;
        for (auto i = inputString.begin() + start;
            i != inputString.end();
            ++i) {
        // We increment the base, then we decode the character.
        value *= base;
        auto chr = *i;
        if (chr >= '0' && chr <= '9') {
            value += (T)(chr - '0');
        } else if (chr >= 'A' && chr <= 'Z') {
            value += (T)(chr - 'A') + 10;
        } else if (chr >= 'a' && chr <= 'z') {
            value += (T)(chr - 'a') + 10;
        }
        }
        return value;
    }

    /**
     * \brief A default expression compiler, oriented at the C language.
     */
    template<typename IntType>
        ExpressionCompiler<IntType> CLikeCompiler =  ExpressionCompiler<IntType>(ExpressionCompilerDefinition<IntType>
    {
        ExpressionParserDefinition<IntType>
        {
            std::vector<ExpressionBinaryOperator<IntType>>
            {
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "||",
                    120,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f || s ? IntType(1) : IntType(0);
                        return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "&&",
                    110,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f && s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "|",
                    100,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f | s;
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "^",
                    90,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f ^ s;
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "&",
                    80,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f & s;
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "==",
                    70,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f == s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                    "!=",
                    70,
                    [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f != s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                    ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        "<",
                        60,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f < s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        "<=",
                        60,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f <= s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        ">",
                        60,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f > s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        ">=",
                        60,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f >= s ? IntType(1) : IntType(0);
    return true;
                    }
                },
                    ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        "<<",
                        50,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f << s;
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        ">>",
                        50,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f >> s;
    return true;
                    }
                },
                    ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        "+",
                        40,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f + s;
    return true;
                    }
                },
                ExpressionBinaryOperator<IntType>
                {
                    ExpressionOperatorAssociativity::LEFT,
                        "-",
                        40,
                        [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                    {
                        out = f - s;
    return true;
                    }
                },
                        ExpressionBinaryOperator<IntType>
                    {
                        ExpressionOperatorAssociativity::LEFT,
                            "*",
                            30,
                            [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                        {
                            out = f * s;
    return true;
                        }
                    },
                    ExpressionBinaryOperator<IntType>
                    {
                        ExpressionOperatorAssociativity::LEFT,
                            "/",
                            30,
                            [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                        {
                            out = f / s;
    return true;
                        }
                    },
                    ExpressionBinaryOperator<IntType>
                        {
                            ExpressionOperatorAssociativity::LEFT,
                                "%",
                                30,
                                [](const IntType& f, const IntType& s, IntType& out, CompileState& state) -> bool
                            {
                                out = f % s;
    return true;
                            }
                        }
            },
            std::vector<ExpressionUnaryOperator<IntType>>
            {
                ExpressionUnaryOperator<IntType>
                {
                    "+",
                    [](const IntType& v, IntType& out, CompileState& state) -> bool
                    {
                        out = +v;
    return true;
                    }
                },
                    ExpressionUnaryOperator<IntType>
                    {
                        "-",
                            [](const IntType& v, IntType& out, CompileState& state) -> bool
                        {
                            out = -v;
    return true;
                        }
                    },
                        ExpressionUnaryOperator<IntType>
                        {
                            "!",
                                [](const IntType& v, IntType& out, CompileState& state) -> bool
                            {
                                out = !v ? IntType(1) : IntType(0);
    return true;
                            }
                        },
                            ExpressionUnaryOperator<IntType>
                            {
                                "~",
                                    [](const IntType& v, IntType& out, CompileState& state) -> bool
                                {
                                    out = ~v;
    return true;
                                }
                            }
            },
            std::vector<ExpressionLiteralDecoder<IntType>>
            {
                ExpressionLiteralDecoder<IntType>
                {
                    "0x[0-9a-fA-F]+",
                        [](const std::string& number, IntType& output, CompileState& state) -> bool
                    {
                        output = simpleNumberParse<IntType>(number, 2, 16);
                        return true;
                    }
                },
                    ExpressionLiteralDecoder<IntType>
                    {
                        "0b[01]+",
                            [](const std::string& number, IntType& output, CompileState& state) -> bool
                        {
                            output = simpleNumberParse<IntType>(number, 2, 2);
                            return true;
                        }
                    },
                        ExpressionLiteralDecoder<IntType>
                        {
                            "[0-9]+",
                                [](const std::string& number, IntType& output, CompileState& state) -> bool
                            {
                                output = simpleNumberParse<IntType>(number, 0, 10);
                                return true;
                            }
                        },
                            ExpressionLiteralDecoder<IntType>
                            {
                                "'.*?'",
                                    [](const std::string& number, IntType& output, CompileState& state) -> bool
                                {
                                    std::vector<uint32_t> intermediate;
                                    if (!StringParser::parseCharacter(number, intermediate, state))
                                    {
                                        return false;
                                    }
                                    output = (IntType)intermediate[0];
                                    return true;
                                }
                            },
            }
        },
        ExpressionHelpRegexes
        {
            "\\(",
            "\\)"
        }
    });

    /**
     * \brief A default compiler for 8-bit wide signed integers.
     */
    ExpressionCompiler<int8_t> CLikeCompilerI8 = CLikeCompiler<int8_t>;

    /**
     * \brief A default compiler for 8-bit wide unsigned integers.
     */
    ExpressionCompiler<uint8_t> CLikeCompilerU8 = CLikeCompiler<uint8_t>;

    /**
     * \brief A default compiler for 16-bit wide signed integers.
     */
    ExpressionCompiler<int16_t> CLikeCompilerI16 = CLikeCompiler<int16_t>;

    /**
     * \brief A default compiler for 16-bit wide unsigned integers.
     */
    ExpressionCompiler<uint16_t> CLikeCompilerU16 = CLikeCompiler<uint16_t>;

    /**
     * \brief A default compiler for 32-bit wide signed integers.
     */
    ExpressionCompiler<int32_t> CLikeCompilerI32 = CLikeCompiler<int32_t>;

    /**
     * \brief A default compiler for 32-bit wide unsigned integers.
     */
    ExpressionCompiler<uint32_t> CLikeCompilerU32 = CLikeCompiler<uint32_t>;

    /**
     * \brief A default compiler for 64-bit wide signed integers.
     */
    ExpressionCompiler<int64_t> CLikeCompilerI64 = CLikeCompiler<int64_t>;

    /**
     * \brief A default compiler for 64-bit wide unsigned integers.
     */
    ExpressionCompiler<uint64_t> CLikeCompilerU64 = CLikeCompiler<uint64_t>;
}

#endif /* ERAGPSIM_PARSER_EXPRESSION_COMPILER_CLIKE_HPP */