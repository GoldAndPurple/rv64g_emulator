#include "cpu.h"

#include <stdint.h>


void e_LWU(CPU *cpu, uint8_t* ram_image, uint32_t inst) {
  // load unsigned 4 byte to rd from address in rs1
  int64_t imm = imm_I(inst);
  uint64_t addr = cpu->regs[rs1(inst)] + (int64_t)imm;
  cpu->regs[rd(inst)] = ram_load(cpu, ram_image, addr - RAM_OFFSET, 32);
  print_op("lwu");
}
void e_LD(CPU *cpu, uint8_t* ram_image, uint32_t inst) {
  // load 8 byte to rd from address in rs1
  int64_t imm = imm_I(inst);
  uint64_t addr = cpu->regs[rs1(inst)] + (int64_t)imm;
  cpu->regs[rd(inst)] = (int64_t)ram_load(cpu, ram_image, addr - RAM_OFFSET, 64);
  print_op("ld");
}
void e_SD(CPU *cpu, uint8_t* ram_image, uint32_t inst) {
  int64_t imm = imm_S(inst);
  uint64_t addr = cpu->regs[rs1(inst)] + (int64_t)imm;
  ram_store(cpu, ram_image, addr - RAM_OFFSET, 64, cpu->regs[rs2(inst)]);
  print_op("sd");
}
void e_SLLI(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] << shamt(inst);
  print_op("slli");
}
void e_SRLI(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_IU(inst) & 0x3f;
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] >> imm;
  print_op("srli");
}

void e_ADDIW(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = (int64_t)(int32_t)(cpu->regs[rs1(inst)] + imm);
  print_op("addiw");
}

void e_SLLIW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = (int64_t)(int32_t)(cpu->regs[rs1(inst)] << (inst& 0x1f));
  print_op("slliw");
}
void e_SRLIW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = (int64_t)(int32_t)(cpu->regs[rs1(inst)] >> (inst& 0x1f));
  print_op("srliw");
}
void e_SRAIW(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] >> (imm& 0x1f));
  print_op("sraiw");
}
void e_ADDW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (int64_t)(int32_t)(cpu->regs[rs1(inst)] + (int64_t)cpu->regs[rs2(inst)]);
  print_op("addw");
}
void e_SUBW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (int64_t)(int32_t)(cpu->regs[rs1(inst)] - (int64_t)cpu->regs[rs2(inst)]);
  print_op("subw");
}
void e_SLLW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (int64_t)(int32_t)(cpu->regs[rs1(inst)] << (cpu->regs[rs2(inst)]& 0x1f));
  print_op("sllw");
}
void e_SRLW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (int64_t)(int32_t)(cpu->regs[rs1(inst)] >> (cpu->regs[rs2(inst)]& 0x1f));
  print_op("srlw");
}
void e_SRAW(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = (int64_t)(int32_t)(
      cpu->regs[rs1(inst)] >> (uint64_t)(int64_t)(int32_t)(cpu->regs[rs2(inst)]& 0x1f));
  print_op("sraw");
}