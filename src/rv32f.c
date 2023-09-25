#include "cpu.h"

#include <stdint.h>
#include <math.h>

void e_FLW(CPU *cpu, uint8_t* ram_image, uint32_t inst){
  int64_t imm = imm_I(inst);
  uint64_t addr = cpu->regs[rs1(inst)] + (int64_t)imm;
  uint64_t tmp = (ram_load(cpu,ram_image, addr, 32));
  cpu->fregs[rd(inst)] = *(float *)&tmp;

  print_op("FLW");
}
void e_FSW(CPU *cpu, uint8_t* ram_image, uint32_t inst){
  uint64_t imm = imm_S(inst);
  uint64_t addr = *(int64_t *)&cpu->regs[rs1(inst)] + (int64_t)imm;
  uint32_t tmp = *(uint32_t *)&cpu->fregs[rs2(inst)];
  ram_store(cpu,ram_image, addr, 32, tmp);

  print_op("FSW");
}
void e_FMADD_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)cpu->fregs[rs1(inst)] * (float)cpu->fregs[rs2(inst)] + (float)cpu->fregs[rs3(inst)];
  print_op("FMADD_S");
}
void e_FMSUB_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)cpu->fregs[rs1(inst)] * (float)cpu->fregs[rs2(inst)] - (float)cpu->fregs[rs3(inst)];

  print_op("FMSUB_S");
}
void e_FNMSUB_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = -1*((float)cpu->fregs[rs1(inst)] * (float)cpu->fregs[rs2(inst)]) + (float)cpu->fregs[rs3(inst)];

  print_op("FNMSUB_S");
}
void e_FNMADD_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = -1*((float)cpu->fregs[rs1(inst)] * (float)cpu->fregs[rs2(inst)]) - (float)cpu->fregs[rs3(inst)];

  print_op("FNMADD_S");
}
void e_FADD_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)cpu->fregs[rs1(inst)] + (float)cpu->fregs[rs2(inst)];

  print_op("FADD_S");
}
void e_FSUB_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)cpu->fregs[rs1(inst)] - (float)cpu->fregs[rs2(inst)];

  print_op("FSUB_S");
}
void e_FMUL_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)cpu->fregs[rs1(inst)] * (float)cpu->fregs[rs2(inst)];

  print_op("FMUL_S");
}
void e_FDIV_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)cpu->fregs[rs1(inst)] / (float)cpu->fregs[rs2(inst)];

  print_op("FDIV_S");
}
void e_FSQRT_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = sqrtf((float)cpu->fregs[rs1(inst)]);

  print_op("FSQRT_S");
}
void e_FSGNJ_S(CPU *cpu, uint32_t inst){
  float r1 = cpu->fregs[rs1(inst)];
  float r2 = cpu->fregs[rs2(inst)];
  uint32_t tmp = ((*(uint32_t *)&r2) & 0x80000000) | (*(uint32_t *)&r1 & 0x7FFFFFFF);
  cpu->fregs[rd(inst)] = *(float*)&tmp;

  print_op("FSGNJ_S");
}
void e_FSGNJN_S(CPU *cpu, uint32_t inst){
  float r1 = cpu->fregs[rs1(inst)];
  float r2 = cpu->fregs[rs2(inst)];
  uint32_t tmp = ((~(*(uint32_t *)&r2)) & 0x80000000) | (*(uint32_t *)&r1 & 0x7FFFFFFF);
  cpu->fregs[rd(inst)] = *(float*)&tmp;

  print_op("FSGNJN_S");
}
void e_FSGNJX_S(CPU *cpu, uint32_t inst){
  float r1 = cpu->fregs[rs1(inst)];
  float r2 = cpu->fregs[rs2(inst)];
  uint32_t tmp = (((*(uint32_t *)&r2) & 0x80000000) ^ (((*(uint32_t *)&r1) & 0x80000000))) | (*(uint32_t *)&r1 & 0x7FFFFFFF);
  cpu->fregs[rd(inst)] = *(float*)&tmp;

  print_op("FSGNJX_S");
}
void e_FMIN_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] < cpu->fregs[rs2(inst)]) ? (float)cpu->fregs[rs1(inst)] : (float)cpu->fregs[rs2(inst)];
  print_op("FMIN_S");
}
void e_FMAX_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] > cpu->fregs[rs2(inst)]) ? (float)cpu->fregs[rs1(inst)] : (float)cpu->fregs[rs2(inst)];

  print_op("FMAX_S");
}

void e_FCVT_W_S(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (int32_t)(float)cpu->fregs[rs1(inst)];

  print_op("FCVT_W_S");
}
void e_FCVT_WU_S(CPU *cpu, uint32_t inst){
  cpu->regs[rd(inst)] = (uint32_t)(float)cpu->fregs[rs1(inst)];

  print_op("FCVT_WU_S");
}
void e_FMV_X_W(CPU *cpu, uint32_t inst){
  float r1 = cpu->fregs[rs1(inst)];
  cpu->regs[rd(inst)] = *(uint32_t*)&r1;

  print_op("FMV_X_W");
}
void e_FEQ_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] == cpu->fregs[rs2(inst)]) ? 1 : 0;

  print_op("FEQ_S");
}
void e_FLT_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] < cpu->fregs[rs2(inst)]) ? 1 : 0;

  print_op("FLT_S");
}
void e_FLE_S(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (cpu->fregs[rs1(inst)] <= cpu->fregs[rs2(inst)]) ? 1 : 0;

  print_op("FLE_S");
}
void e_FCLASS_S(CPU *cpu, uint32_t inst){
  float f = cpu->fregs[rs1(inst)];
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

  print_op("FCLASS_S");
}
void e_FCVT_S_W(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)(int32_t)cpu->regs[rs1(inst)];

  print_op("FCVT_S_W");
}
void e_FCVT_S_WU(CPU *cpu, uint32_t inst){
  cpu->fregs[rd(inst)] = (float)(uint32_t)cpu->regs[rs1(inst)];

  print_op("FCVT_S_WU");
}
void e_FMV_W_X(CPU *cpu, uint32_t inst){
  uint32_t r1 = cpu->regs[rs1(inst)];
  cpu->fregs[rd(inst)] = *(float*)&r1;

  print_op("FMV_W_X");
}