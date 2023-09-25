#include "cpu.h"

#include <stdint.h>

void e_LR_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  cpu->regs[rd(inst)] = tmp;
  
  print_op("lr.w");
}
void e_SC_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, cpu->regs[rs2(inst)]);
  cpu->regs[rd(inst)] = 0;
  
  print_op("sc.w");
}
void e_AMOSWAP_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = (uint32_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amoswap.w");
}
void e_AMOADD_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = tmp + (uint32_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amoadd.w");
}
void e_AMOXOR_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = tmp ^ (uint32_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amoxor.w");
}
void e_AMOAND_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = tmp & (uint32_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amoand.w");
}
void e_AMOOR_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = tmp | (uint32_t)cpu->regs[rs2(inst)];
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amoor.w");
}
void e_AMOMIN_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res =
      ((int32_t)cpu->regs[rs2(inst)] < tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amomin.w");
}
void e_AMOMAX_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res =
      ((int32_t)cpu->regs[rs2(inst)] > tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amomax.w");
}
void e_AMOMINU_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = (cpu->regs[rs2(inst)] < tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amominu.w");
}
void e_AMOMAXU_W(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint32_t tmp = ram_load(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32);
  uint32_t res = (cpu->regs[rs2(inst)] > tmp) ? cpu->regs[rs2(inst)] : tmp;
  cpu->regs[rd(inst)] = tmp;
  ram_store(cpu, ram_image, cpu->regs[rs1(inst)] - RAM_OFFSET, 32, res);
  print_op("amomaxu.w");
}
