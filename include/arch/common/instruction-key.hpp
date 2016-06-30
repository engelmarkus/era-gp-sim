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

#ifndef ERAGPSIM_ARCH_COMMON_INSTRUCTION_KEY_HPP
#define ERAGPSIM_ARCH_COMMON_INSTRUCTION_KEY_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "common/container-adapter.hpp"
#include "common/utility.hpp"

/**
 * Class to identify an instruction in an ISA.
 *
 * It is naive to think that an instruction is
 * identified uniquely through its opcode alone. In RISC-V, the opcode is a
 * quite general *instruction group* identifier. For example, all load
 * operations and all store operations have the same opcode. Next to the opcode,
 * almost all instructions have *function bits*, which further specialize an
 * instruction. For example, for loads, the instructions for load operations of
 * different sizes (`LH`, `LD`) have the same opcode, but different function
 * bits (in this case referred to as the `width`). These bits would tell the
 * decoder the width and sign of the data to load (and analogously, store, for
 * `S[BHWD][U]`). It is thus necessary to identify instructions through a tuple
 * of keys. In practice, this means a hashtable, which provides most flexibilty.
 */
class InstructionKey
    : public ContainerAdapter<std::unordered_map<std::string, std::size_t>> {
 public:
  using super = ContainerAdapter<std::unordered_map<std::string, std::size_t>>;
  using super::_container;
  using super::InitializerList;
  using super::begin;
  using super::cbegin;
  using super::end;
  using super::cend;
  using super::clear;
  using super::size;
  using super::isEmpty;
  using Key             = std::string;
  using Value           = std::size_t;
  using KeyCollection   = std::vector<Key>;
  using ValueCollection = std::vector<Value>;


  /**
   * Constructs an InstructionKey from a range of key/value pairs.
   *
   * @tparam Range A range-like type.
   *
   * @param range The range of key/value pairs.
   */
  template <typename Range>
  explicit InstructionKey(const Range& range) : super(range) {
  }

  /**
   * Constructs an InstructionKey from a list of key/value pairs.
   *
   * @param list An initializer list of key/value pairs.
   */
  explicit InstructionKey(InitializerList list);

  /**
   * Adds a new key/valeu pair to the InstructionKey.
   *
   * @param key The key of this pair.
   * @param value The value of this pair.
   *
   * @return The current `InstructionKey` object.
   */
  InstructionKey& add(const Key& key, const Value& value);

  /**
   * Adds a range of new key/value pairs to the InstructionKey.
   *
   * @tparam A range-like type.
   *
   * @param range The range of key/value pairs to add.
   *
   * @return The current `InstructionKey` object.
   */
  template <typename Range>
  InstructionKey& add(const Range& range) {
    for (auto& pair : range) {
      add(pair.first, pair.second);
    }

    return *this;
  }

  /**
   * Adds a list of key/value pairs to the InstructionKey.
   *
   * @param list The list of key/value pairs to add.
   *
   * @return The current `InstructionKey` object.
   */
  InstructionKey& add(InitializerList list);

  /**
   * Returns a value for the given key.
   *
   * @param key The key to look up the value for.
   */
  const Value& get(const Key& key) const noexcept;

  /**
   * Returns a value for the given key.
   *
   * @param key The key to look up the value for.
   *
   * @see get()
   */
  const Value& operator[](const Key& key) const;

  /**
   * Returns whether or not the InstructionKey has the given key.
   *
   * @param key The key to look for.
   */
  bool hasKey(const Key& key) const noexcept;

  /**
   * Returns the keys of the InstructionKey.
   */
  KeyCollection getKeys() const noexcept;

  /**
   * Returns the values of the InstructionKey.
   */
  ValueCollection getValues() const noexcept;
};

#endif /* ERAGPSIM_ARCH_COMMON_INSTRUCTION_KEY_HPP */
