#include "arch/riscv/load-store-instructions.hpp"

<<<<<<< HEAD
bool LoadInstructionNode::validate() {
  if (!requireChildren(NodeType::REGISTER, 0, 2) ||
      !requireChildren(NodeType::IMMEDIATE, 2, 1)) {
=======
using namespace riscv;

bool LoadInstructionNode::validate() {
  if (!requireChildren(Type::REGISTER, 0, 2) ||
      !requireChildren(Type::IMMEDIATE, 2, 1)) {
>>>>>>> master
    return false;
  }

  // TODO Switch-case through the types and validate
  // the size of the offset respectively

  return true;
}

bool StoreInstructionNode::validate() {
<<<<<<< HEAD
  if (!requireChildren(NodeType::REGISTER, 0, 2) ||
      !requireChildren(NodeType::IMMEDIATE, 2, 1)) {
=======
  if (!requireChildren(Type::REGISTER, 0, 2) ||
      !requireChildren(Type::IMMEDIATE, 2, 1)) {
>>>>>>> master
    return false;
  }

  // TODO Switch-case through the types and validate
  // the size of the offset respectively

  return true;
}

MemoryValue LoadInstructionNode::getValue(DummyMemoryAccess &memory_access) {
<<<<<<< HEAD
    std::string dest = _children.at(0)->getIdentifier();
    MemoryValue base = _children.at(1)->getValue(memory_access);
    MemoryValue offset = _children.at(2)->getValue(memory_access);

    // TODO Replace this with actual implementation
    // TODO Ensure the correct amount of bytes are loaded from memory
}

MemoryValue StoreInstructionNode::getValue(DummyMemoryAccess &memory_access) {
    MemoryValue base = _children.at(0)->getValue(memory_access);
    MemoryValue src = _children.at(1)->getValue(memory_access);
    MemoryValue offset = _children.at(2)->getValue(memory_access);

    // TODO Replace this with actual implementation
    // memory_access.putMemory(base + offset, src);
    // TODO Ensure the correct amount of bytes are written into memory
=======
  std::string dest   = _children.at(0)->getIdentifier();
  MemoryValue base   = _children.at(1)->getValue(memory_access);
  MemoryValue offset = _children.at(2)->getValue(memory_access);

  // TODO Replace this with actual implementation
  // TODO Ensure the correct amount of bytes are loaded from memory
  return {};
}

MemoryValue StoreInstructionNode::getValue(DummyMemoryAccess &memory_access) {
  MemoryValue base   = _children.at(0)->getValue(memory_access);
  MemoryValue src    = _children.at(1)->getValue(memory_access);
  MemoryValue offset = _children.at(2)->getValue(memory_access);

  // TODO Replace this with actual implementation
  // memory_access.putMemory(base + offset, src);
  // TODO Ensure the correct amount of bytes are written into memory
  return {};
>>>>>>> master
}
