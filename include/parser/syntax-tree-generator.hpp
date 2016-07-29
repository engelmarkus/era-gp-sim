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

#ifndef ERAGPSIM_PARSER_SYNTAX_TREE_GENERATOR_HPP_
#define ERAGPSIM_PARSER_SYNTAX_TREE_GENERATOR_HPP_

#include <memory>
#include <vector>
#include "arch/common/abstract-node-factory-collection.hpp"
#include "compile-state.hpp"

class SyntaxTreeGenerator {
public:
    SyntaxTreeGenerator(AbstractNodeFactoryCollection&& nodeFactories) : _nodeFactories(std::move(nodeFactories))
    {}
    std::unique_ptr<AbstractSyntaxTreeNode> transformOperand(const std::string& operand, CompileState& state) const;
    std::unique_ptr<AbstractSyntaxTreeNode> transformCommand(const std::string& command_name, std::vector<std::unique_ptr<AbstractSyntaxTreeNode>>& sources, std::vector<std::unique_ptr<AbstractSyntaxTreeNode>>& targets, CompileState& state) const;
private:
    AbstractNodeFactoryCollection _nodeFactories;
};

#endif /* ERAGPSIM_PARSER_SYNTAX_TREE_GENERATOR_HPP_ */