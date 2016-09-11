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
* along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#ifndef ERAGPSIM_ARCH_RISCV_INSTRUCTION_NODE_FACTORY_HPP
#define ERAGPSIM_ARCH_RISCV_INSTRUCTION_NODE_FACTORY_HPP

#include <cstdint>
#include <string>
#include <unordered_map>

#include "arch/common/abstract-instruction-node-factory.hpp"
#include "arch/common/instruction-information.hpp"
#include "arch/common/instruction-set.hpp"
#include "arch/riscv/integer-instructions.hpp"
#include "arch/riscv/load-store-instructions.hpp"

class Architecture;

namespace riscv {

/**
 * \brief The InstructionNodeFactory class is a
 * concrete Implementation of AbstractInstructionNodeFactory for the RISC-V
 * architecture.
 * \see AbstractInstructionNodeFactory
 */
class InstructionNodeFactory : public AbstractInstructionNodeFactory {
 public:
  using Node = std::unique_ptr<AbstractSyntaxTreeNode>;

  /**
   * \brief InstructionNodeFactory
   * Creates a Instruction Node Factory for RISC-V architecture
   */
  InstructionNodeFactory(const InstructionSet &instructions,
                         const Architecture &architecture);

  /**
   * \brief createInstructionNode
   * Creates and returns a RISC-V Instruction Node for a valid input mnemonic,
   * or
   * nullptr if the mnemonic cannot be mapped to a implemented RISC-V
   * instruction
   * \param mnemonic
   * \return std::uniqe_ptr pointing to the newly created instruction node, or
   * nullptr if the mnemonic cannot be mapped to a implemented RISC-V
   * instruction
   */
  Node createInstructionNode(const std::string &mnemonic) const override;

 private:
  /*! Internal integer type to represent 32bit for arithmetic operations. */
  using RV32_integral_t = uint32_t;
  /*! Internal integer type to represent 64bit for arithmetic operations. */
  using RV64_integral_t = uint64_t;

  using Factory = std::function<std::unique_ptr<AbstractSyntaxTreeNode>(
      const InstructionInformation &)>;

  /**
   * An internal utility class to map instruction names to their factory
   * functions.
   *
   * The purpose of this class is to reduce the boilerplate for adding nodes  *
   * to the factory mapping, as well as for accessing factories to create new
   * nodes.
   */
  class FactoryMap {
   public:
    /**
     * Adds an integer instruction to the factory map.
     *
     * This method makes it easy to create both a register and immediate version
     * for a given instruction. If this feature is activated, an instruction
     * with the 'i' suffix will be created as well.
     *
     * \tparam The instruction class to associate the instruction (name) with.
     * \param name The name of the instruction.
     * \param hasImmediateVersion If true, also create an immediate version of
     *        the instruction.
     */
    template <template <typename...> class InstructionType, typename SizeType>
    void
    add(const std::string &instructionName, bool hasImmediateVersion = true) {
      using Operands = typename InstructionType<SizeType>::Operands;

      // clang-format off
      _map.emplace(instructionName, [](const auto &information) {
        return std::make_unique<InstructionType<SizeType>>(
          information, Operands::REGISTERS);
      });

      if (hasImmediateVersion) {
        _map.emplace(instructionName + 'i', [](const auto &information) {
          return std::make_unique<InstructionType<SizeType>>(
              information, Operands::IMMEDIATES);
        });
        // clang-format on
      }
    }

    template <typename InstructionType, typename... Args>
    void add(const std::string &instructionName, Args &&... args) {
      // Cannot preserve value category (simply) unfortunately
      _map.emplace(instructionName, [args...](const auto &information) {
        return std::make_unique<InstructionType>(information, args...);
      });
    }

    /**
     * Returns the factory for a given instruction name.
     *
     * Error (existence) checking is performed inside this function.
     *
     * \param instructionName The name of the instruction to retrieve.
     *
     * \return The factory fucntion for the given instruction.
     */
    const Factory &get(const std::string &instructionName) const;

    /** \copydoc get() */
    const Factory &operator[](const std::string &instructionName) const;

    /**
     * Creates an instruction node for the given instruction name.
     *
     * Calling this function is the same as invoking the result of `get()`.
     *
     * \param instructionName The name of the instruction to retrieve.
     * \param information The information object for the instruction.
     *
     * \return An abstract syntax tree node for the instruction.
     */
    Node create(const std::string &instructionName,
                const InstructionInformation &information) const;

    /** \copydoc create() */
    Node operator()(const std::string &instructionName,
                    const InstructionInformation &information) const;

   private:
    /** The actual underlying string to factory mapping. */
    std::unordered_map<std::string, Factory> _map;
  };

  /**
   * \brief Sets up non-integer instructions.
   */
  void _setupOtherInstructions();

  /**
   * \brief Sets load instructions.
   */
  void _setupLoadInstructions();

  /**
   * \brief Sets store instructions.
   */
  void _setupStoreInstructions();

  /**
   * Sets up the integer instructions.
   *
   * \tparam SizeType The word size of the architecture.
   */
  template <typename SizeType>
  void _setupIntegerInstructions() {
    _factories.add<AddInstructionNode, SizeType>("add");
    _factories.add<SubInstructionNode, SizeType>("sub", false);
    _factories.add<AndInstructionNode, SizeType>("and");
    _factories.add<OrInstructionNode, SizeType>("or");
    _factories.add<XorInstructionNode, SizeType>("xor");
    _factories.add<ShiftLeftLogicalInstructionNode, SizeType>("sll");
    _factories.add<ShiftRightLogicalInstructionNode, SizeType>("srl");
    _factories.add<ShiftRightArithmeticInstructionNode, SizeType>("sra");
  }

  /**
   * Table, that maps the instruction identifier (e.g. the mnemonic "add" for
   * Addition) to a factory function that creates the special instruction node
   * (e.g.
   * AddInstructionNode)
   */
  FactoryMap _factories;

  /*!
   * \brief _instructionSet
   * Description of all instructions that can be created by this factory
   */
  InstructionSet _instructionSet;
};
}

#endif /* ERAGPSIM_ARCH_RISCV_INSTRUCTION_NODE_FACTORY_HPP */
