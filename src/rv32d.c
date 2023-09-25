#include "cpu.h"

#include <stdint.h>
#include <math.h>

void e_FLD(CPU *cpu, uint8_t* ram_image, uint32_t inst){
  int64_t imm = imm_I(inst);
  uint64_t addr = cpu->regs[rs1(inst)] + (int64_t)imm;
  uint64_t tmp = (ram_load(cpu, ram_image, addr, 64));
  cpu->fregs[rd(inst)] = *(double *)&tmp;

  print_op("FLD");
}
void e_FSD(CPU *cpu, uint8_t* ram_image, uint32_t inst){
  uint64_t imm = imm_S(inst);
  uint64_t addr = *(int64_t *)&cpu->regs[rs1(inst)] + (int64_t)imm;
  uint64_t tmp = *(uint64_t *)&cpu->fregs[rs2(inst)];
  ram_store(cpu, ram_image, addr, 64, tmp);

  print_op("FSD");
}
void e_FMADD_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = cpu->fregs[rs1(inst)] * cpu->fregs[rs2(inst)] + cpu->fregs[rs3(inst)];
  print_op("FMADD_D");
}
void e_FMSUB_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = cpu->fregs[rs1(inst)] * cpu->fregs[rs2(inst)] - cpu->fregs[rs3(inst)];

  print_op("FMSUB_D");
}
void e_FNMSUB_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = -1*(cpu->fregs[rs1(inst)] * cpu->fregs[rs2(inst)]) + cpu->fregs[rs3(inst)];

  print_op("FNMSUB_D");
}
void e_FNMADD_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = -1*(cpu->fregs[rs1(inst)] * cpu->fregs[rs2(inst)]) - cpu->fregs[rs3(inst)];

  print_op("FNMADD_D");
}
void e_FADD_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = cpu->fregs[rs1(inst)] + cpu->fregs[rs2(inst)];

  print_op("FADD_D");
}
void e_FSUB_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = cpu->fregs[rs1(inst)] - cpu->fregs[rs2(inst)];

  print_op("FSUB_D");
}
void e_FMUL_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = cpu->fregs[rs1(inst)] * cpu->fregs[rs2(inst)];

  print_op("FMUL_D");
}
void e_FDIV_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = cpu->fregs[rs1(inst)] / cpu->fregs[rs2(inst)];

  print_op("FDIV_D");
}
void e_FSQRT_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = sqrt(cpu->fregs[rs1(inst)]);

  print_op("FSQRT_D");
}
void e_FSGNJ_D(CPU *cpu, uint32_t inst){
  uint64_t tmp = ((*(uint64_t *)&cpu->fregs[rs2(inst)]) & 0x8000000000000000) | (*(uint64_t *)&cpu->fregs[rs1(inst)] & 0x7FFFFFFFFFFFFFFF);
  cpu->fregs[rd(inst)] = *(double*)&tmp;

  print_op("FSGNJ_D");
}
void e_FSGNJN_D(CPU *cpu, uint32_t inst){
  uint64_t tmp = ((~(*(uint64_t *)&cpu->fregs[rs2(inst)])) & 0x8000000000000000) | (*(uint64_t *)&cpu->fregs[rs1(inst)] & 0x7FFFFFFFFFFFFFFF);
  cpu->fregs[rd(inst)] = *(double*)&tmp;

  print_op("FSGNJN_D");
}
void e_FSGNJX_D(CPU *cpu, uint32_t inst){
  uint64_t tmp = (((*(uint64_t *)&cpu->fregs[rs2(inst)]) & 0x8000000000000000) ^ (((*(uint64_t *)&cpu->fregs[rs1(inst)]) & 0x8000000000000000))) | (*(uint64_t *)&cpu->fregs[rs1(inst)] & 0x7FFFFFFFFFFFFFFF);
  cpu->fregs[rd(inst)] = *(double*)&tmp;

  print_op("FSGNJX_D");
}
void e_FMIN_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] < cpu->fregs[rs2(inst)]) ? cpu->fregs[rs1(inst)] : cpu->fregs[rs2(inst)];
  print_op("FMIN_D");
}
void e_FMAX_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] > cpu->fregs[rs2(inst)]) ? cpu->fregs[rs1(inst)] : cpu->fregs[rs2(inst)];

  print_op("FMAX_D");
}

void e_FCVT_S_D(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (float)cpu->fregs[rs1(inst)];

  print_op("FCVT_S_D");
}
void e_FCVT_D_S(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (double)cpu->fregs[rs1(inst)];

  print_op("FCVT_D_S");
}
void e_FEQ_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] == cpu->fregs[rs2(inst)]) ? 1 : 0;

  print_op("FEQ_D");
}
void e_FLT_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] < cpu->fregs[rs2(inst)]) ? 1 : 0;

  print_op("FLT_D");
}
void e_FLE_D(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] <= cpu->fregs[rs2(inst)]) ? 1 : 0;

  print_op("FLE_D");
}
void e_FCLASS_D(CPU *cpu, uint32_t inst){
  double f = cpu->fregs[rs1(inst)];
  int cclass = fpclassify(f);
  int rvclass;
  switch (cclass)
   {
      case FP_NAN:
      rvclass = 9;
      break;

      case FP_INFINITE:
      rvclass = (f > __FLT_MAX__) ? 7 : 0;
      break;

      case FP_ZERO:
      rvclass = (f == 0.0f) ? 4 : 3;
      break;

      case FP_SUBNORMAL:
      rvclass = (f > 0.0f) ? 5 : 2;
      break;

      case FP_NORMAL:
      rvclass = (f > 0.0f) ? 6 : 1;
      break;

      default: break;
   }
  cpu->regs[rd(inst)] = (1 << rvclass);

  print_op("FCLASS_D");
}
void e_FCVT_W_D(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (int32_t)cpu->fregs[rs1(inst)];

  print_op("FCVT_W_D");
}
void e_FCVT_WU_D(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (uint32_t)cpu->fregs[rs1(inst)];

  print_op("FCVT_WU_D");
}
void e_FCVT_D_W(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (double)(int32_t)cpu->regs[rs1(inst)];

  print_op("FCVT_D_W");
}
void e_FCVT_D_WU(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (double)(uint32_t)cpu->regs[rs1(inst)];

  print_op("FCVT_D_WU");
}