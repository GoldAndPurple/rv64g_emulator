#include "cpu.h"

#include <stdint.h>

// CSR instructions
void e_CSRRW(CPU *cpu,  uint8_t *image,uint32_t inst) {
  uint64_t temp = csr_read(cpu, image, csr(inst));
  csr_write(cpu, image, csr(inst), cpu->regs[rs1(inst)]);
  if (rd(inst) != 0){
    cpu->regs[rd(inst)] = temp;
  }

  print_op("csrrw");
}
void e_CSRRS(CPU *cpu,  uint8_t *image,uint32_t inst) {
  uint64_t temp = csr_read(cpu, image, csr(inst));
  csr_write(cpu, image, csr(inst), temp | cpu->regs[rs1(inst)]);
  cpu->regs[rd(inst)] = temp;
  print_op("csrrs");
}
void e_CSRRC(CPU *cpu,  uint8_t *image,uint32_t inst) {
  uint64_t temp = csr_read(cpu, image, csr(inst));
  csr_write(cpu, image, csr(inst), temp & ~(cpu->regs[rs1(inst)]));
  cpu->regs[rd(inst)] = temp;
  print_op("csrrc");
}
void e_CSRRWI(CPU *cpu,  uint8_t *image,uint32_t inst) {
  uint64_t temp = csr_read(cpu, image, csr(inst));
  csr_write(cpu, image, csr(inst), rs1(inst));
  cpu->regs[rd(inst)] = temp;
  print_op("csrrwi");
}
void e_CSRRSI(CPU *cpu,  uint8_t *image,uint32_t inst) {
  uint64_t temp = csr_read(cpu, image, csr(inst));
  csr_write(cpu, image, csr(inst), temp | rs1(inst));
  cpu->regs[rd(inst)] = temp;
  print_op("csrrsi");
}
void e_CSRRCI(CPU *cpu,  uint8_t *image,uint32_t inst) {
  uint64_t temp = csr_read(cpu, image, csr(inst));
  csr_write(cpu, image, csr(inst), temp & ~rs1(inst));
  cpu->regs[rd(inst)] = temp;
  print_op("csrrci");
}