/* C++ Assembler Interpreter
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef ERAGPSIM_ARCH_COMMON_IMMEDIATE_NODE_HPP
#define ERAGPSIM_ARCH_COMMON_IMMEDIATE_NODE_HPP

#include <memory>
#include <string>
#include <vector>

#include "arch/common/abstract-syntax-tree-node.hpp"
#include "core/memory-value.hpp"

/** A node that contains a concrete memory value. */
class ImmediateNode : public AbstractSyntaxTreeNode {
 public:
  /**
   * Constructs a new node that contains a concrete value.
   *
   * \param value The value of this node.
   */
  ImmediateNode(MemoryValue value)
  : AbstractSyntaxTreeNode(Type::IMMEDIATE), _value(value) {
  }

  /**
   * \return The concrete value
   */
  virtual MemoryValue
  getValue(DummyMemoryAccess& memory_access) const override {
    return _value;
  }

  /**
   * \return true, if there are no children.
   */
  virtual bool validate() const override {
    // Immediate values can't have any children
    return AbstractSyntaxTreeNode::_children.size() == 0;
  }

  /**
   * \return An empty MemoryValue, because the instruction has to be
   * assembled in the instruction node.
   */
  virtual MemoryValue assemble() const override {
    return MemoryValue();
  }

  /**
   * Returns always the same string: "imm".
   *
   * \return The string "imm"
   */

  virtual const std::string& getIdentifier() const override {
    return "Imm";
  }

 private:
  MemoryValue _value;
};


#endif /* ERAGPSIM_ARCH_COMMON_IMMEDIATE_NODE_HPP */
