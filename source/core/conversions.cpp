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

#include "core/conversions.hpp"

namespace {
constexpr uint8_t testEQ[8]{
    0xFF, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F,
};
}

MemoryValue permute(const MemoryValue& memoryValue,
                    const std::size_t byteSize,
                    const std::vector<std::size_t> permutation) {
  assert::that(byteSize > 0);
  assert::that(memoryValue.getSize() % byteSize ==
         0);// The value has to be made out of whole bytes
  const std::size_t byteAmount{memoryValue.getSize() / byteSize};
  assert::that(permutation.size() >=
         byteAmount);// We need at least byteAmount values to permute

  // permute the bytes
  std::vector<std::uint8_t> permuted{};
  std::uint8_t mask{testEQ[byteSize % 8]};
  for (std::size_t i = 0; i < byteAmount; ++i) {
    for (std::size_t j = 0; j < byteSize - 1; j += 8) {
      if (j + 8 < byteSize) {
        permuted.push_back(
            memoryValue.getByteAt(permutation[i] * byteSize + j));
      } else {
        permuted.push_back(
            memoryValue.getByteAt(permutation[i] * byteSize + j) & mask);
      }
    }
  }

  // pull together
  if (byteSize % 8 == 0) {
    // this is so much easier
    return MemoryValue{permuted, permuted.size() * 8};
  }

  // Size of A Virtual Byte in byte
  const std::size_t sizeOfByte{(byteSize + 7) / 8};
  // The raw result vector
  std::vector<std::uint8_t> raw{};
  // The number of bits in the buffer
  std::size_t bitsInBuffer{0};
  // the Buffer
  std::uint8_t bufferByte{0};

  // for each byte
  for (std::size_t i = 0; i < byteAmount; ++i) {
    for (std::size_t j = 0; j < sizeOfByte; ++j) {
      // take byte from permuted vector and put into buffer
      bufferByte |= permuted[i * sizeOfByte + j] << bitsInBuffer;
      // Add size of this byte to bitsInBuffer
      if (j + 1 < sizeOfByte) {
        // inner byte => full byte
        bitsInBuffer += 8;
      } else {
        // last byte in virtual byte
        bitsInBuffer += byteSize % 8;
      }
      // Buffer Filled
      if (bitsInBuffer >= 8) {
        // push onto result vector
        raw.push_back(bufferByte);
        // the part of the current byte that didn'tfit into the Buffer become
        // the new Buffer
        bufferByte = permuted[i * sizeOfByte + j] >>
                     (16 - bitsInBuffer);//(8 - (bIB % 8))
        bitsInBuffer -= 8;
      }
    }
  }
  // push final Byte
  raw.push_back(bufferByte);
  return MemoryValue(raw, memoryValue.getSize());
}

MemoryValue permute(const MemoryValue& memoryValue, const std::size_t byteSize, const std::function<std::size_t(std::size_t)> permutation) {
  assert::that(byteSize > 0);
  assert::that(memoryValue.getSize() % byteSize == 0);// The value has to be made out of whole bytes
  std::vector<std::size_t> permutationVector{};
  std::size_t byteAmount{memoryValue.getSize() / byteSize};
  for (std::size_t i = 0; i < byteAmount; ++i) {
    permutationVector.push_back(permutation(i));
  }
  return permute(memoryValue, byteSize, permutationVector);
}

bool unSignumA(const MemoryValue& memoryValue) {
  return false;
}

bool signumA(const MemoryValue& memoryValue) {
  return memoryValue.get(memoryValue.getSize() - 1);
}

MemoryValue nonsignedB(const MemoryValue& memoryValue) {
  return memoryValue;
}

MemoryValue signBitB(const MemoryValue& memoryValue) {
  assert::that(memoryValue.getSize() > 1);
  return memoryValue.subSet(0, memoryValue.getSize() - 1);
}

MemoryValue onesComplementB(const MemoryValue& memoryValue) {
  assert::that(memoryValue.getSize() > 1);
  std::vector<std::uint8_t> raw{memoryValue.internal()};
  // cut last byte if necessary
  if (memoryValue.getSize() % 8 == 1) {
    raw.pop_back();
  }
  // invert all bits
  for (std::size_t i = 0; i < raw.size(); ++i) {
    raw[i] ^= 0xFF;
  }
  return MemoryValue{raw, memoryValue.getSize() - 1};
}

MemoryValue twosComplementB(const MemoryValue& memoryValue) {
  // invert
  MemoryValue result{onesComplementB(memoryValue)};
  // add one
  for (std::size_t i = 0; i < result.getSize() - 1; ++i) {
    if (!result.flip(i)) return result;
  }
  return result;
}

MemoryValue nonsignedC(const std::vector<std::uint8_t>& value,
                       std::size_t size,
                       bool sign) {
  assert::that(!sign);// don't convert signed values via nonsigned
  return MemoryValue{value, size};
}

MemoryValue
signBitC(const std::vector<std::uint8_t>& value, std::size_t size, bool sign) {
  MemoryValue result{value, size};
  // set the sign bit
  result.set(size - 1, sign);
  return result;
}

MemoryValue onesComplementC(const std::vector<std::uint8_t>& value,
                            std::size_t size,
                            bool sign) {
  std::vector<std::uint8_t> raw{value};
  // invert all bits
  for (std::size_t i = 0; i < raw.size(); ++i) {
    raw[i] ^= 0xFF;
  }
  MemoryValue result{std::move(raw), size};
  // set the sign bit
  result.set(size - 1, sign);
  return result;
}

MemoryValue twosComplementC(const std::vector<std::uint8_t>& value,
                            std::size_t size,
                            bool sign) {
  // invert
  MemoryValue result{onesComplementC(value, size, sign)};
  // subtract one
  for (std::size_t i = 0; i < result.getSize() - 1; ++i) {
    if (result.flip(i)) return result;
  }
  return result;
}

const signFunction unSignum0{ unSignumA };
const signFunction Signum0{ signumA };

const toIntegralFunction nonsigned1{ nonsignedB };
const toIntegralFunction signBit1{ signBitB };
const toIntegralFunction onesComplement1{ onesComplementB };
const toIntegralFunction twosComplement1{ twosComplementB };

const toMemoryValueFunction nonsigned2{ nonsignedC };
const toMemoryValueFunction signBit2{ signBitC };
const toMemoryValueFunction onesComplement2{ onesComplementC };
const toMemoryValueFunction twosComplement2{ twosComplementC };

const Conversion nonsigned     { unSignum0,      nonsigned1,     nonsigned2 };
const Conversion signBit       {   Signum0,        signBit1,       signBit2 };
const Conversion onesComplement{   Signum0, onesComplement1,onesComplement2 };
const Conversion twosComplement{   Signum0, twosComplement1,twosComplement2 };

Conversion switchConversion(SignedRepresentation representation) {
  switch (representation) {
  case SignedRepresentation::UNSIGNED:
    return nonsigned;
  case SignedRepresentation::SIGN_BIT:
    return signBit;
  case SignedRepresentation::ONES_COMPLEMENT:
    return onesComplement;
  case SignedRepresentation::TWOS_COMPLEMENT:
    return twosComplement;
  case SignedRepresentation::SMART:
    return nonsigned;
  default:
    assert::that(false);
  }
  return nonsigned;
}

MemoryValue permute(const MemoryValue& memoryValue, Endianness byteOrder, std::size_t byteSize) {
  switch (byteOrder) {
  case Endianness::LITTLE:
    return memoryValue;
  case Endianness::BIG:
    return permute(memoryValue, byteSize, bigEndian{ memoryValue.getSize() / byteSize });
  default:
    assert::that(false);
  }
  return MemoryValue{};
}
