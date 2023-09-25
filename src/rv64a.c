#include "cpu.h"

#include <stdint.h>

void e_LR_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 64);
  cpu->regs[rd(inst)] = tmp;
  
  print_op("lr.d");
  }
void e_SC_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 64, cpu->regs[rs2(inst)]);
  cpu->regs[rd(inst)] = 0;
  
  print_op("sc.d");}
void e_AMOSWAP_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = (uint64_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amoswap.d");
}
void e_AMOADD_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = tmp + (uint64_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amoadd.d");
}
void e_AMOXOR_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = tmp ^ (uint64_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amoxor.d");
}
void e_AMOAND_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = tmp & (uint64_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amoand.d");
}
void e_AMOOR_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = tmp | (uint64_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amoor.d");
}
void e_AMOMIN_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res =
      ((int64_t)cpu->regs[rs2(inst)] < tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amomin.d");
}
void e_AMOMAX_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res =
      ((int64_t)cpu->regs[rs2(inst)] > tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amomax.d");
}
void e_AMOMINU_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = (cpu->regs[rs2(inst)] < tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amominu.d");
}
void e_AMOMAXU_D(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)], 64);
  uint64_t res = (cpu->regs[rs2(inst)] > tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)], 64, res);
  print_op("amomaxu.d");
}