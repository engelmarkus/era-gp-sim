########################################
# SOURCES
########################################

set(ARCH_RISCV_SOURCES
	control-flow-instructions.cpp
	instruction-node.cpp
	integer-instructions.cpp
	load-store-instructions.cpp
	mul-div-instructions.cpp
	riscv-instruction-node-factory.cpp
	riscv-register-access-node-factory.cpp
	riscv-immediate-node-factory.cpp
)

########################################
# TARGET
########################################

add_library(era-sim-arch-riscv STATIC ${ARCH_RISCV_SOURCES})

target_link_libraries(era-sim-arch-riscv era-sim-arch-common)
