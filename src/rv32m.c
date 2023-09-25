#include "cpu.h"

#include <stdint.h>
// rv32m
void e_MUL(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      ((int64_t)cpu->regs[rs1(inst)] * (int64_t)cpu->regs[rs2(inst)]);
  print_op("mul");
}
void e_MULH(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      ((int64_t)cpu->regs[rs1(inst)] * (int64_t)cpu->regs[rs2(inst)]) >> 32;
  print_op("mulh");
}
void e_MULHSU(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      ((int64_t)cpu->regs[rs1(inst)] * (uint64_t)cpu->regs[rs2(inst)]) >> 32;
  print_op("mulhsu");
}
void e_MULHU(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      ((uint64_t)cpu->regs[rs1(inst)] * (uint64_t)cpu->regs[rs2(inst)]) >> 32;
  print_op("mulhu");
}
void e_DIV(CPU *cpu, uint32_t inst) {
  if (((int64_t)cpu->regs[rs1(inst)] == INT64_MIN) &&
          ((int64_t)cpu->regs[rs2(inst)] == -1) ||
      (cpu->regs[rs2(inst)] == 0)) {
    cpu->regs[rd(inst)] = -1;
  } else {
    cpu->regs[rd(inst)] =
        (int64_t)cpu->regs[rs1(inst)] / (int64_t)cpu->regs[rs2(inst)];
  }
  print_op("div");
}
void e_DIVU(CPU *cpu, uint32_t inst) {
  if (cpu->regs[rs2(inst)] == 0) {
    cpu->regs[rd(inst)] = 0xffffffffffffffff;
  } else {
    cpu->regs[rd(inst)] =
        (uint64_t)cpu->regs[rs1(inst)] / (uint64_t)cpu->regs[rs2(inst)];
  }
  print_op("divu");
}
void e_REM(CPU *cpu, uint32_t inst) {
  if (((int64_t)cpu->regs[rs1(inst)] == INT32_MIN) &&
          ((int64_t)cpu->regs[rs2(inst)] == -1) ||
      (cpu->regs[rs2(inst)] == 0)) {
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)];
  } else {
    cpu->regs[rd(inst)] = (uint64_t)(
        (int64_t)cpu->regs[rs1(inst)] % (int64_t)cpu->regs[rs2(inst)]);
  }
  print_op("rem");
}
void e_REMU(CPU *cpu, uint32_t inst) {
  /* if (((int64_t)cpu->regs[rs1(inst)] == INT64_MIN) &&
          ((int64_t)cpu->regs[rs2(inst)] == -1) ||
      (cpu->regs[rs2(inst)] == 0)) {
    cpu->regs[rd(inst)] = cpu->regs[rs1(inst)];
  } else  */
  if (cpu->regs[rs2(inst)] == 0) {
    cpu->regs[rd(inst)] = 0xffffffffffffffff;
  } else {
    cpu->regs[rd(inst)] =
        (uint64_t)cpu->regs[rs1(inst)] % (uint64_t)cpu->regs[rs2(inst)];
  }
  print_op("remu");
}