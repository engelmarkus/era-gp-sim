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

#include "arch/common/abstract-instruction-node-factory.hpp"
#include "arch/common/architecture-formula.hpp"
#include "arch/common/architecture.hpp"
#include "arch/riscv/instruction-node-factory.hpp"
#include "core/project-module.hpp"
#include "gtest/gtest.h"

using namespace riscv;

class RiscvBaseTest : public ::testing::Test {
 public:
  void load(ArchitectureFormula::InitializerList modules =
                ArchitectureFormula::InitializerList(),
            std::size_t memorySize = 64) {
    auto formula = ArchitectureFormula("riscv", modules);
    _project = std::make_unique<ProjectModule>(formula, memorySize, "riscv");
  }

  MemoryAccess getMemoryAccess() {
    return _project->getMemoryAccess();
  }

  const Architecture getArchitecture() {
    return _project->getArchitectureAccess().getArchitecture().get();
  }

  const NodeFactoryCollection getFactories() {
    ArchitectureAccess access = _project->getArchitectureAccess();
    return access.getArchitecture().get().getNodeFactories();
  }

  std::unique_ptr<ProjectModule> _project;
};
