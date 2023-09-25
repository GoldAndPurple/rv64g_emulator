#include "cpu.h"

#include <stdint.h>

void e_FCVT_L_S(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (int64_t)(float)cpu->fregs[rs1(inst)];

  print_op("FCVT_L_S");

}
void e_FCVT_LU_S(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (uint64_t)(float)cpu->fregs[rs1(inst)];

  print_op("FCVT_LU_S");
    
}
void e_FCVT_S_L(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)(int64_t)cpu->regs[rs1(inst)];

  print_op("FCVT_S_L");
    
}
void e_FCVT_S_LU(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)(uint64_t)cpu->regs[rs1(inst)];

  print_op("FCVT_S_LU");
    
}