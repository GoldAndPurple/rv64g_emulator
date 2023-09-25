#include "cpu.h"

#include <stdint.h>

void e_LUI(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = imm_U(inst);
  print_op("lui");
}

void e_AUIPC(CPU *cpu, uint32_t inst) {
  uint64_t imm = (imm_U(inst));
  cpu->regs[rd(inst)] = (cpu->pc + *(int64_t*)&imm);
  print_op("auipc");
}

void e_JAL(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_J(inst);
  cpu->regs[rd(inst)] = cpu->pc + 4;
  cpu->pc = cpu->pc + imm - 4;
  print_op("jal");
}
void e_JALR(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  uint64_t temp = cpu->pc + 4;
  cpu->pc = (((int64_t)cpu->regs[rs1(inst)] + (int64_t)imm) & ~1) - 4;
  cpu->regs[rd(inst)] = temp;
  print_op("jalr");
}

void e_BEQ(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_B(inst);
  if ((int64_t)(int32_t)cpu->regs[rs1(inst)] == (int64_t)(int32_t)cpu->regs[rs2(inst)])
    cpu->pc = cpu->pc + imm - 4;
  print_op("beq");
}
void e_BNE(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_B(inst);
  if ((int64_t)(int32_t)cpu->regs[rs1(inst)] != (int64_t)(int32_t)cpu->regs[rs2(inst)])
    cpu->pc = (cpu->pc + imm - 4);
  print_op("bne");
}
void e_BLT(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_B(inst);
  if ((int64_t)(int32_t)cpu->regs[rs1(inst)] < (int64_t)(int32_t)cpu->regs[rs2(inst)]) {
    cpu->pc = cpu->pc + imm - 4;
  }
  print_op("blt");
}
void e_BGE(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_B(inst);
  if ((int64_t)(int32_t)cpu->regs[rs1(inst)] >= (int64_t)(int32_t)cpu->regs[rs2(inst)])
    cpu->pc = cpu->pc + imm - 4;
  print_op("bge");
}
void e_BLTU(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_B(inst);
  if (cpu->regs[rs1(inst)] < cpu->regs[rs2(inst)])
    cpu->pc = cpu->pc + imm - 4;
  print_op("bltu");
}
void e_BGEU(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_B(inst);
  if (cpu->regs[rs1(inst)] >= cpu->regs[rs2(inst)])
    cpu->pc = cpu->pc + imm - 4;
  print_op("jal");
}
void e_LB(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t imm = imm_I(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  cpu->regs[rd(inst)] =
      (int64_t)(int8_t)ram_load(cpu, ram_image, addr - RAM_OFFSET, 8);
  print_op("lb");
}
void e_LH(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t imm = imm_I(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  cpu->regs[rd(inst)] =
      (int64_t)(int16_t)ram_load(cpu, ram_image, addr - RAM_OFFSET, 16);
  print_op("lh");
}
void e_LW(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t imm = imm_I(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  cpu->regs[rd(inst)] = ram_load(cpu, ram_image, addr - RAM_OFFSET, 32);
  print_op("lw");
}
void e_LBU(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t imm = imm_I(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  cpu->regs[rd(inst)] = ram_load(cpu, ram_image, addr - RAM_OFFSET, 8);
  print_op("lbu");
}
void e_LHU(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  int64_t imm = imm_I(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  cpu->regs[rd(inst)] = ram_load(cpu, ram_image, addr - RAM_OFFSET, 16);
  print_op("lhu");
}
void e_SB(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t imm = imm_S(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  ram_store(cpu, ram_image, addr - RAM_OFFSET, 8, cpu->regs[rs2(inst)]);
  print_op("sb");
}
void e_SH(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t imm = imm_S(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  ram_store(cpu, ram_image, addr - RAM_OFFSET, 16, cpu->regs[rs2(inst)]);
  print_op("sh");
}
void e_SW(CPU *cpu, uint8_t *ram_image, uint32_t inst) {
  uint64_t imm = imm_S(inst);
  uint64_t addr = (cpu->regs[rs1(inst)] + (int64_t)imm);
  ram_store(cpu, ram_image, addr - RAM_OFFSET, 32, cpu->regs[rs2(inst)]);
  print_op("sw");
}

void e_ADDI(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] + (int64_t)imm);
  print_op("addi");
}

void e_SLTI(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] < (int64_t)imm) ? 1 : 0;
  print_op("slti");
}

void e_SLTIU(CPU *cpu, uint32_t inst) {
  uint64_t imm = imm_IU(inst);
  cpu->regs[rd(inst)] = (cpu->regs[rs1(inst)] < imm) ? 1 : 0;
  print_op("sltiu");
}

void e_XORI(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] ^ imm;
  print_op("xori");
}

void e_SRAI(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = (int32_t)cpu->regs[rs1(inst)] >> imm;
  print_op("srai");
}

void e_ORI(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] | imm;
  print_op("ori");
}

void e_ANDI(CPU *cpu, uint32_t inst) {
  int64_t imm = imm_I(inst);
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] & imm;
  print_op("andi");
}

void e_ADD(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (uint64_t)((int64_t)cpu->regs[rs1(inst)] + (int64_t)cpu->regs[rs2(inst)]);
  print_op("add");
}

void e_SUB(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (uint64_t)((int64_t)cpu->regs[rs1(inst)] - (int64_t)cpu->regs[rs2(inst)]);
  print_op("sub");
}

void e_SLL(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] << (int64_t)cpu->regs[rs2(inst)];
  print_op("sll");
}

void e_SLT(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (int64_t)cpu->regs[rs1(inst)] < (int64_t)cpu->regs[rs2(inst)];
  print_op("slt");
}

void e_SLTU(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] < cpu->regs[rs2(inst)];
  print_op("slti");
}

void e_XOR(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] ^ cpu->regs[rs2(inst)];
  print_op("xor");
}

void e_SRL(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] >> (int64_t)cpu->regs[rs2(inst)];
  print_op("srl");
}

void e_SRA(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] =
      (int32_t)cpu->regs[rs1(inst)] >> (int64_t)cpu->regs[rs2(inst)];
  print_op("sra");
}

void e_OR(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] | cpu->regs[rs2(inst)];
  print_op("or");
}

void e_AND(CPU *cpu, uint32_t inst) {
  cpu->regs[rd(inst)] = cpu->regs[rs1(inst)] & cpu->regs[rs2(inst)];
  print_op("and");
}

void e_FENCE(CPU *cpu, uint32_t inst) { print_op("fence"); }

void e_ECALL(CPU *cpu, uint32_t inst) {}
void e_EBREAK(CPU *cpu, uint32_t inst) {}

void e_ECALLBREAK(CPU *cpu, uint32_t inst) {
  if (imm_IU(inst) == 0x0)
    e_ECALL(cpu, inst);
  if (imm_IU(inst) == 0x1)
    e_EBREAK(cpu, inst);
  print_op("ecallbreak");
}