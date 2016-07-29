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

#ifndef ERAGPSIM_PARSER_EXPRESSION_TOKENIZER_HPP_
#define ERAGPSIM_PARSER_EXPRESSION_TOKENIZER_HPP_

#include "compile-state.hpp"

enum class TokenType {
    NUMBER,
    OPERATOR,
    UNARY_OPERATOR,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    FUNCTION_NAME,
    LEFT_TERNARY,
    RIGHT_TERNARY,
    COMMA,
	INVALID
};

using Token = std::pair<TokenType, std::string>;

class ExpressionTokenizer {
public:
    ExpressionTokenizer() = default;
    std::vector<Token> tokenize(const std::string& input, CompileState& state);
};

#endif /* ERAGPSIM_PARSER_EXPRESSION_TOKENIZER_HPP_ */