#include "arch/riscv/instruction-node.hpp"
#include <cassert>

using namespace riscv;

const std::string& InstructionNode::getIdentifier() const {
  return _instructionInformation.getMnemonic();
}

bool InstructionNode::requireChildren(Type type,
                                      size_t startIndex,
                                      size_t amount) const {
  if (_children.size() - startIndex < amount) return false;

  for (size_t i = startIndex; i < startIndex + amount; i++) {
    if (_children.at(i)->getType() != type) {
      return false;
    }
  }
  return true;
}

size_t InstructionNode::getChildrenCount() const{
    return _children.size();
}

bool InstructionNode::validateAllChildren() {
    for(auto &child : _children) {
        if(!child->validate()) {
            return false;
        }
    }
    return true;
}

AbstractSyntaxTreeNode& InstructionNode::getChild(size_t index) {
    assert(index > 0 && index < _children.size());
    return *(_children.at(index));
}
