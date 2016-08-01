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

#ifndef ERAGPSIM_ARCH_COMMON_AST_HPP_
#define ERAGPSIM_ARCH_COMMON_AST_HPP_

#include <memory>
#include <string>
#include <vector>

/*
 * A dummy memory access. It will be replaced by
 * a proper implementation soon.
 */
struct DummyMemoryAccess {};

/*
 * A dummy memory value. It will be replaced by
 * a proper implementation soon.
 */
struct MemoryValue {};

/**
 * The different node types of the Syntax Tree.
 * Note, that the last two types are not needed for RISC V.
 */
enum struct NodeType {
  INSTRUCTION,
  IMMEDIATE,
  REGISTER,
  MEMORY_ACCESS,
  ARITHMETIC
};

/** The base class for nodes in the abstract syntax tree */
class AbstractSyntaxTreeNode {
public:
  using NodePtr = std::unique_ptr<AbstractSyntaxTreeNode>;

  /**
   * Executes this node and it's children recursively.
   *
   * \param memory_access Access to memory and registers
   * \return An memory value, that represents the the result of the execution.
   * The meaning differs between different node types.
   */
  virtual MemoryValue getValue(DummyMemoryAccess &memory_access) = 0;

  /**
   * Validates the structure of this syntax tree. This should be called
   * before every execution.
   *
   * \return Whether this syntax tree is valid for execution.
   */
  virtual bool validate() = 0;

  /**
   * Assembles this syntax tree into its binary representation. So, this
   * simulates the architecture specific assemblage.
   *
   * \return The bit representation of this syntax tree.
   */
  virtual MemoryValue assemble() = 0;

  /**
   * Returns the identifier of this node. The identifier is formatted as a
   * string and depends on the node type.
   *
   * \return The identifier of this node.
   */
  virtual std::string getIdentifier() = 0;

  /**
   * Getter for the type of this node.
   * \return The type of this node.
   */
  NodeType getType() {
    return _node_type;
  }

  /**
   * Adds a child to this node. Note, that the node will be added after
   * the last node, that has been added.
   *
   * \param node The node to be added.
   */
  void addChild(NodePtr node) { _children.push_back(std::move(node)); }

 protected:
  /**
   * Constructs a new node. The constructor is supposed to be called in
   * the subclasses.
   *
   * \param node_type The type of this node.
   */
  AbstractSyntaxTreeNode(NodeType node_type) : _node_type(node_type) {
  }

  std::vector<NodePtr> _children;

 private:
  NodeType _node_type;
};

/** A node that contains a concrete memory value. */
class ImmediateNode : public AbstractSyntaxTreeNode {
public:
  /**
   * Constructs a new node that contains a concrete value.
   *
   * \param value The value of this node.
   */
  ImmediateNode(MemoryValue value)
      : AbstractSyntaxTreeNode(NodeType::IMMEDIATE), _value(value) {}

  /**
   * \return The concrete value
   */
  virtual MemoryValue getValue(DummyMemoryAccess &memory_access) override {
    return _value;
  }

  /**
   * \return true, if there are no children.
   */
  virtual bool validate() override {
    // Immediate values can't have any children
    return AbstractSyntaxTreeNode::_children.size() == 0;
  }

  /**
   * \return An empty MemoryValue, because the instruction has to be
   * assembled in the instruction node.
   */
  virtual MemoryValue assemble() override { return MemoryValue{}; }

  /**
   * Returns always the same string: "imm".
   *
   * \return The string "imm"
   */
  virtual std::string getIdentifier() { return "Imm"; }

private:
  MemoryValue _value;
};

/** A node that represents a register. */
class RegisterNode : public AbstractSyntaxTreeNode {
public:
  /**
   * Constructs a new node that represents a register.
   *
   * \param value The identifier for the register.
   */
  RegisterNode(std::string identifier)
      : AbstractSyntaxTreeNode(NodeType::REGISTER), _identifier(identifier) {}

  /**
   * \return The content of the register, represented by this node.
   */
  virtual MemoryValue getValue(DummyMemoryAccess &memory_access) override {
    // TODO Return the actual content of the register using the proper
    // memory access
    return MemoryValue{};
  }

  /**
   * \return true, if there are no children.
   */
  virtual bool validate() override {
    // Immediate values can't have any children
    return AbstractSyntaxTreeNode::_children.size() == 0;
  }

  /**
   * \return An empty MemoryValue, because the instruction has to be
   * assembled in the instruction node.
   */
  virtual MemoryValue assemble() override { return MemoryValue{}; }

  /**
   * \return The identifier of the register.
   */
  virtual std::string getIdentifier() { return _identifier; }

private:
  std::string _identifier;
};

#endif // ERAGPSIM_ARCH_COMMON_AST_HPP_