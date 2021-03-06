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

#ifndef ERAGPSIM_PARSER_INDEPENDENT_MACRO_END_DIRECTIVE_HPP
#define ERAGPSIM_PARSER_INDEPENDENT_MACRO_END_DIRECTIVE_HPP

#include "parser/independent/intermediate-directive.hpp"

/**
 * A direction which indicates the end of a macro in the code.
 */
class MacroEndDirective : public IntermediateDirective {
 public:
  /**
   * Instantiates a new MacroEndDirective with the given arguments.
   *
   * \param positionInterval The line interval the operation occupies.
   * \param labels The vector of labels assigned to the operation.
   * \param name The name of the operation.
   * \param arguments Arguments of the directive. Should be none.
   */
  MacroEndDirective(const CodePositionInterval& positionInterval,
                    const PositionedStringVector& labels,
                    const PositionedString& name,
                    const PositionedStringVector& arguments = {});

  /**
   * Specifies if the this operation should be processed. In this case:
   * never!
   *
   * \return Always false.
   */
  virtual bool shouldInsert() const;

  /**
   * Specifies the new target for operations after this command.
   *
   * \return Switch back to the main target.
   */
  virtual TargetSelector newTarget() const;

  /**
   * Finalizes a macro end directive.
   */
  virtual ~MacroEndDirective() = default;
};

#endif /* ERAGPSIM_PARSER_INDEPENDENT_MACRO_END_DIRECTIVE_HPP */
