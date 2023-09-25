#include "cpu.h"

#include <stdint.h>
void e_MULW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = (int64_t)(
      (int32_t)cpu->regs[rs1(inst)] * (int32_t)cpu->regs[rs2(inst)]);
  print_op("mulw");
}
void e_DIVW(CPU *cpu, uint32_t inst) {
  if (((int64_t)cpu->regs[rs1(inst)] == INT64_MIN) &&
          ((int64_t)cpu->regs[rs2(inst)] == -1) ||
      (cpu->regs[rs2(inst)] == 0)) {
    cpu->regs[rd(inst)] = -1;
  } else {
  cpu->regs[rd(inst)] =
      (int64_t)(int32_t)(cpu->regs[rs1(inst)] / (int64_t)cpu->regs[rs2(inst)]);}
  print_op("divw");
}
void e_DIVUW(CPU *cpu, uint32_t inst) {
  if (((int64_t)cpu->regs[rs1(inst)] == INT64_MIN) &&
          ((int64_t)cpu->regs[rs2(inst)] == -1) ||
      (cpu->regs[rs2(inst)] == 0)) {
    cpu->regs[rd(inst)] = 0xffffffffffffffff;
  } else {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] / (int64_t)cpu->regs[rs2(inst)];}
  print_op("divuw");
}
void e_REMW(CPU *cpu, uint32_t inst) {
  if (((int64_t)cpu->regs[rs1(inst)] == INT64_MIN) &&
          ((int64_t)cpu->regs[rs2(inst)] == -1) ||
      (cpu->regs[rs2(inst)] == 0)) {
    cpu->regs[rd(inst)] = -1;
  } else {
  cpu->regs[rd(inst)] =
      (int64_t)(int32_t)(cpu->regs[rs1(inst)] % (int64_t)cpu->regs[rs2(inst)]);}
  print_op("remw");
}
void e_REMUW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] % (int64_t)cpu->regs[rs2(inst)];
  print_op("remuw");
}