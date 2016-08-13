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

#ifndef ERAGPSIM_ARCH_COMMON_VALIDATION_RESULT_HPP_
#define ERAGPSIM_ARCH_COMMON_VALIDATION_RESULT_HPP_

#include <vector>

/**
 * A simple class that represents the result of the validate operation on the
 * abstract sytax tree.
 *
 * A validation result consists of a type and parameters. Parameters can be
 * added using the add() functions and retrieved using the get() functions.
 * For every type of parameter, a new index is used to identify the
 * parameters of that type. The index of a parameter is incremented for every
 * add() call.
 */
class ValidationResult {
 public:
  /**
   * Type of the validation result.
   */
  enum struct Type {
    /**
     * Validation succeeded. No parameters.
     */
    SUCCESS,

    /**
     * Invalid amount of arguments. Parameters:
     *
     * Ints:
     * * expected
     * * actual
     */
    INVALID_ARGUMENT_COUNT,

    /**
     * The syntax tree contains an argument, that is not supported at
     * that position. No parameters.
     */
    INVALID_ARGUMENT_TYPE,

    /**
     * An immediate value can't be represented by a specific amount of bits.
     * Parameters:
     *
     * Ints:
     * * Maximum amount of bits
     *
     * Bools:
     * * Whether the immediate value is signed
     */
    INVALID_IMMEDIATE_SIZE
  };

  /**
   * Creates a new ValidationResult without any parameters.
   * \type The type of the result
   */
  ValidationResult(Type type);

  /**
   * Returns true if the type of this validation result is SUCCESS.
   */
  bool isSuccess() const;

  /**
   * Returns the type of the type.
   * \return The type passed in the constructor
   */
  Type getType() const;

  /**
   * Adds an int parameter.
   * \parameter The parameter
   * \return A reference to this object
   */
  ValidationResult& addInt(int parameter) noexcept;

  /**
   * Adds a bool parameter.
   * \parameter The parameter
   * \return A reference to this object
   */
  ValidationResult& addBool(bool parameter) noexcept;

  /**
   * Gets an int parameter at a given index.
   * \param index The index
   * \return The value of the parameter. 0 if index is out of bounds.
   */
  int getInt(int index) const noexcept;

  /**
   * Gets a bool paremter at a given index.
   * \param index The index
   * \return The value of the parameter. false if index is out of bounds.
   */
  bool getBool(int index) const noexcept;

 private:
  Type _type;

  std::vector<int> _ints;
  std::vector<bool> _bools;
};

#endif /* ERAGPSIM_ARCH_COMMON_VALIDATION_RESULT_HPP_ */
