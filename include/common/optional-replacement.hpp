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
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ERAGPSIM_COMMON_OPTIONAL_REPLACEMENT_HPP_
#define ERAGPSIM_COMMON_OPTIONAL_REPLACEMENT_HPP_

template <typename T>
class Optional {
 public:
  Optional<T>() : _hasData{false} {
  }
  Optional<T>(const Optional<T>&) = default;
  Optional<T>(Optional<T>&&)      = default;
  Optional<T>& operator=(const Optional<T>&) = default;
  Optional<T>& operator=(Optional<T>&&) = default;
  ~Optional<T>()                        = default;
  Optional<T>(const T& data) : _data{data}, _hasData{true} {
  }
  Optional<T>(T&& data) : _data{std::move<T>(data)}, _hasData{true} {
  }
  operator bool() const {
    return _hasData;
  }
  const T& operator*() const {
    return _data;
  }
  Optional<T> operator=(const T& value) {
    _data = value;
    return *this;
  }

  bool operator==(const Optional<T>& rhs) const {
    if (bool(*this) != bool(rhs)) return false;
    if (bool(*this) == false) return true;
    return **this == *rhs;
  }
  bool operator==(const T& value) const {
    return bool(*this) ? **this == value : false;
  }
  bool operator!=(const Optional<T>& rhs) const {
    if (bool(*this) != bool(rhs)) return true;
    if (bool(*this) == false) return false;
    return **this != *rhs;
  }


 private:
  T _data;
  bool _hasData;
};

#endif// ERAGPSIM_COMMON_OPTIONAL_REPLACEMENT_HPP_
