#include "cpu.h"

#include <stdint.h>

void e_FCVT_L_D(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = (int64_t)cpu->fregs[rs1(inst)];

  print_op("FCVT_L_D");
}
void e_FCVT_LU_D(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = (uint64_t)cpu->fregs[rs1(inst)];

  print_op("FCVT_LU_D");
}
void e_FMV_X_D(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = *(uint64_t*)&cpu->fregs[rs1(inst)];

  print_op("FMV_X_D");
}
void e_FCVT_D_L(CPU *cpu, uint32_t inst) {
  cpu->fregs[rd(inst)] = (double)(int64_t)cpu->regs[rs1(inst)];

  print_op("FCVT_D_L");
}
void e_FCVT_D_LU(CPU *cpu, uint32_t inst) {
  cpu->fregs[rd(inst)] = (double)(uint64_t)cpu->regs[rs1(inst)];

  print_op("FCVT_D_LU");
}
void e_FMV_D_X(CPU *cpu, uint32_t inst) {
  cpu->fregs[rd(inst)] = *(double*)(uint64_t*)&cpu->regs[rs1(inst)];

  print_op("FMV_D_X");
}