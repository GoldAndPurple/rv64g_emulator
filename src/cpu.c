#include "cpu.h"
#include "csr.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_op(char *s) {
#ifdef LOGGING
  fprintf(output_f, "%s\n", s);
#endif
}

void HandleOtherCSRWrite(uint8_t *image, uint16_t csrno, uint64_t value) {
  if (csrno == 0x136) {
    printf("%d", value);
    fflush(stdout);
  } else if (csrno == 0x137) {
    printf("%08x", value);
    fflush(stdout);
  } else if (csrno == 0x138) {
    uint32_t ptrstart = value - RAM_OFFSET;
    uint32_t ptrend = ptrstart;
    if (ptrstart >= RAM_SIZE)
      printf("DEBUG PASSED INVALID PTR (%08x)\n", value);
    while (ptrend < RAM_SIZE) {
      if (image[ptrend] == 0)
        break;
      ptrend++;
    }
    if (ptrend != ptrstart)
      fwrite(image + ptrstart, ptrend - ptrstart, 1, stdout);
  } else if (csrno == 0x139) {
    putchar(value);
    fflush(stdout);
  } else if (csrno == 0x13A) {
    printf("%lf", *(double*)&value);
    fflush(stdout);
  } else if (csrno == 0x666) {
    printf("FAIL\n");
    exit(0);

  } else if (csrno == 0x777) {
    printf("PASS\n");
  }
}

int64_t HandleOtherCSRRead(uint8_t *image, uint16_t csrno) {
  return 0;
}

static uint32_t trap;
static uint64_t rval = 0;

uint64_t csr_read(CPU *cpu, uint8_t *image, uint64_t csr) {
  // printf("CSR-READ: %8x\n",csr);
  uint64_t rval = HandleOtherCSRRead(image, csr);
  if (rval == 0) {
    rval = cpu->csr[csr];
  }
  return rval;
}
void csr_write(CPU *cpu, uint8_t *image, uint64_t csr, uint64_t value) {
  // printf("CSR-WRITE: %8x\n",csr);
  HandleOtherCSRWrite(image, csr, value);
  cpu->csr[csr] = value;
}

uint64_t ram_load_8(CPU *cpu, uint8_t *image, uint64_t addr) {
  return *(uint8_t *)(image + addr);
}
uint64_t ram_load_16(CPU *cpu, uint8_t *image, uint64_t addr) {
  return *(uint16_t *)(image + addr);
}
uint64_t ram_load_32(CPU *cpu, uint8_t *image, uint64_t addr) {
  return *(uint32_t *)(image + addr);
}
uint64_t ram_load_64(CPU *cpu, uint8_t *image, uint64_t addr) {
  return *(uint64_t *)(image + addr);
}

uint64_t ram_load(CPU *cpu, uint8_t *image, uint64_t addr, uint64_t size) {

  if (addr >= RAM_SIZE - 3) {
    addr += RAM_OFFSET;
    if (addr >= 0x10000000 && addr < 0x12000000) {
      if (addr == 0x1100bffc)
        return cpu->csr[TIMERH];
      else if (addr == 0x1100bff8)
        return cpu->csr[TIMERL];
    } else {
      trap = 6;
      rval = addr;
    }
  } else {
    if (addr >= RAM_SIZE) {
      printf("LOAD: %x is inaccessible\n", addr);
      trap = 6;
      return 0;
    }
    switch (size) {
    case 8:
      return ram_load_8(cpu, image, addr);
      break;
    case 16:
      return ram_load_16(cpu, image, addr);
      break;
    case 32:
      return ram_load_32(cpu, image, addr);
      break;
    case 64:
      return ram_load_64(cpu, image, addr);
      break;
    }
  }
  return 0;
}

void ram_store_8(CPU *cpu, uint8_t *image, uint64_t addr, uint64_t value) {
  *(uint8_t *)(image + addr) = value;
}
void ram_store_16(CPU *cpu, uint8_t *image, uint64_t addr, uint64_t value) {
  *(uint16_t *)(image + addr) = value;
}
void ram_store_32(CPU *cpu, uint8_t *image, uint64_t addr, uint64_t value) {
  *(uint32_t *)(image + addr) = value;
}
void ram_store_64(CPU *cpu, uint8_t *image, uint64_t addr, uint64_t value) {
  *(uint64_t *)(image + addr) = value;
}

void ram_store(
    CPU *cpu, uint8_t *image, uint64_t addr, uint64_t size, uint64_t value) {

  if (addr >= RAM_SIZE - 3) {
    addr += RAM_OFFSET;
    if (addr >= 0x10000000 && addr < 0x12000000) {
      if (addr == 0x11004004)
        cpu->csr[TIMERMATCHH] = value;
      else if (addr == 0x11004000)
        cpu->csr[TIMERMATCHL] = value;
      else if (addr == 0x11100000) // SYSCON (reboot, poweroff, etc.)
      {
        cpu->pc += 4;
        rval = value; // return SYSCON
        trap = 13;
      } else if (addr == 0x10000000) // UART 8250 / 16550 Data Buffer
      {
        printf("%c", value);
        fflush(stdout);
      }
    } else {
      trap = 8;
      rval = addr;
    }
  } else {
    if (addr >= RAM_SIZE) {
      printf("STORE: %x is inaccessible\n", addr);
      return;
    }
    switch (size) {
    case 8:
      ram_store_8(cpu, image, addr, value);
      break;
    case 16:
      ram_store_16(cpu, image, addr, value);
      break;
    case 32:
      ram_store_32(cpu, image, addr, value);
      break;
    case 64:
      ram_store_64(cpu, image, addr, value);
      break;
    }
    return;
  }
}

uint64_t rd(uint32_t inst) {
  return (inst >> 7) & 0x1f; // rd in bits 11..7
}
uint64_t rs1(uint32_t inst) {
  return (inst >> 15) & 0x1f; // rs1 in bits 19..15
}
uint64_t rs2(uint32_t inst) {
  return (inst >> 20) & 0x1f; // rs2 in bits 24..20
}
uint64_t rs3(uint32_t inst) {
  return (inst >> 27); // rs3 in bits 26..25
}

uint64_t imm_IU(uint32_t inst) { return inst >> 20; }
int64_t imm_I(uint32_t inst) {
  uint64_t imm = inst >> 20;
  imm = imm | ((imm & 0x800) ? 0xfffffffffffff000 : 0);
  return imm;
}
uint64_t imm_S(uint32_t inst) {
  return ((int64_t)(int32_t)(inst & 0xfe000000) >> 20) | ((inst >> 7) & 0x1f);
}
uint64_t imm_B(uint32_t inst) {

  return (uint64_t)((int64_t)(int32_t)(inst & 0x80000000) >> 19) |
      ((inst & 0x80) << 4)
      | ((inst >> 20) & 0x7e0)
      | ((inst >> 7) & 0x1e);
}
uint64_t imm_U(uint32_t inst) {
  return (inst & 0xfffff000);
}
int64_t imm_J(uint32_t inst) {
  return (((int64_t)(int32_t)(inst & 0x80000000)) >> 11) |
      (inst & 0xff000) // imm[19:12]
      | ((inst >> 9) & 0x800) // imm[11]
      | ((inst >> 20) & 0x7fe); // imm[10:1]
}

uint32_t shamt(uint32_t inst) {
  // shamt(shift amount) only required for immediate shift instructions
  // shamt[4:5] = imm[5:0]
  return (uint32_t)(imm_I(inst) & 0x3f);
}

uint64_t csr(uint32_t inst) {
  // csr[11:0] = inst[31:20]
  return ((inst & 0xfff00000) >> 20);
}

static uint32_t lastpc = 0;

int cpu_cycle(
    CPU *cpu,
    uint8_t *image,
    uint32_t elapsedUs,
    int count) {

  uint32_t new_timer = cpu->csr[TIME] + elapsedUs;
  cpu->csr[TIME] = new_timer;

  // Handle Timer interrupt.
  if (cpu->csr[TIME] > cpu->csr[TIMERMATCHH]) {
    cpu->csr[EXTRAFLAGS] &= ~4; // Clear WFI
    cpu->csr[MIP] |= 1 << 7;
  } else
    cpu->csr[MIP] &= ~(1 << 7);

  if (cpu->csr[EXTRAFLAGS] & 4)
    return 1;

  cpu->csr[CYCLE]++;

  trap = 0;
  // uint32_t current_cycle = cpu->csr[CYCLEL];
  uint32_t current_cycle = cpu->csr[CYCLE];
  uint32_t inst = 0;
  uint64_t rd_old = 0;

  if ((cpu->csr[MIP] & (1 << 7)) && (cpu->csr[MIE] & (1 << 7) /*mtie*/) &&
      (cpu->csr[MSTATUS] & 0x8 /*mie*/)) {
    // Timer interrupt.
    trap = 0x80000007;
    cpu->pc -= 4;
  } else // No timer interrupt?  Execute a bunch of instructions.
    for (int icount = 0; icount < count; icount++) {
      if (cpu->pc == 0) {
        return 3;
      }
      if (cpu->pc == 0x8007ebc8 /* 0x80175b60 */) {
        cpu->pc += 0;
      }
      lastpc = cpu->pc;

      current_cycle++;
      uint32_t ofs_pc = cpu->pc - RAM_OFFSET;

      if (ofs_pc >= RAM_SIZE) {
        trap = 2; // Handle access violation on instruction read.
        break;
      } else if (ofs_pc & 3) {
        trap = 1; // Handle PC-misaligned access
        break;
      } else {
        inst = ram_load(cpu, image, ofs_pc, 32);
        rd_old = cpu->regs[rd(inst)];
        rval = 0;

        int opcode = inst & 0x7f; // opcode --  [6:2] + [1:0]
        int funct3 = (inst >> 12) & 0x7; // funct3 -- [14:12]
        int funct7 = (inst >> 25) & 0x7f; // funct7 -- [31:25]
        int funct2 = funct7 & 0x3;

        cpu->regs[0] = 0;

#ifdef LOGGING
        fprintf(output_f, "%#.8lx\t\t%#.8x\t\t", cpu->pc, inst); // DEBUG
        // dump_registers(output_f, cpu, image);
#endif

        switch (opcode) {

        case 0b0110111:
          e_LUI(cpu, inst);
          break;
        case 0b0010111:
          e_AUIPC(cpu, inst);
          break;

        case 0b1101111:
          e_JAL(cpu, inst);
          break;
        case 0b1100111:
          e_JALR(cpu, inst);
          break;

        /* B-type -- conditional branches */
        case 0b1100011:
          switch (funct3) {
          case 0b000:
            e_BEQ(cpu, inst);
            break;
          case 0b001:
            e_BNE(cpu, inst);
            break;
          case 0b100:
            e_BLT(cpu, inst);
            break;
          case 0b101:
            e_BGE(cpu, inst);
            break;
          case 0b110:
            e_BLTU(cpu, inst);
            break;
          case 0b111:
            e_BGEU(cpu, inst);
            break;
          default:
            trap = 3;
          }
          break;

        /* Load int */
        case 0b0000011: {
          switch (funct3) {
          case 0b000:
            e_LB(cpu, image, inst);
            break;
          case 0b001:
            e_LH(cpu, image, inst);
            break;
          case 0b010:
            e_LW(cpu, image, inst);
            break;
          case 0b011:
            e_LD(cpu, image, inst);
            break;
          case 0b100:
            e_LBU(cpu, image, inst);
            break;
          case 0b101:
            e_LHU(cpu, image, inst);
            break;
          case 0b110:
            e_LWU(cpu, image, inst);
            break;
          default:
            trap = 3;
          }
        } break;

        /* Store int */
        case 0b0100011: {
          switch (funct3) {
          case 0b000:
            e_SB(cpu, image, inst);
            break;
          case 0b001:
            e_SH(cpu, image, inst);
            break;
          case 0b010:
            e_SW(cpu, image, inst);
            break;
          case 0b011:
            e_SD(cpu, image, inst);
            break;
          default:
            trap = 3;
          }
        } break;

        /* Load float */
        case 0b0000111:
          switch (funct3) {
          case 0b010:
            e_FLW(cpu, image, inst);
            break;
          case 0b011:
            e_FLD(cpu, image, inst);
            break;
          }
          break;

        /* Store float */
        case 0b0100111:
          switch (funct3) {
          case 0b010:
            e_FSW(cpu, image, inst);
            break;
          case 0b011:
            e_FSD(cpu, image, inst);
            break;
          }
          break;

        /* I-type -- register-immediate integer computation */
        case 0b0010011:
          switch (funct3) {
          case 0b000:
            e_ADDI(cpu, inst);
            break;
          case 0b001:
            e_SLLI(cpu, inst);
            break;
          case 0b010:
            e_SLTI(cpu, inst);
            break;
          case 0b011:
            e_SLTIU(cpu, inst);
            break;
          case 0b100:
            e_XORI(cpu, inst);
            break;
          case 0b101:
            if (inst & 0x40000000) {
              e_SRAI(cpu, inst);
            } else {
              e_SRLI(cpu, inst);
            }
            break;
          case 0b110:
            e_ORI(cpu, inst);
            break;
          case 0b111:
            e_ANDI(cpu, inst);
            break;
          default:
            trap = 3;
          }
          break;

        /* R-type -- register-register integer computation */
        case 0b0110011:
          switch (funct7) {
          /* rv32m */
          case 0b0000001:
            switch (funct3) {
            case 0b000:
              e_MUL(cpu, inst);
              break;
            case 0b001:
              e_MULH(cpu, inst);
              break;
            case 0b010:
              e_MULHSU(cpu, inst);
              break;
            case 0b011:
              e_MULHU(cpu, inst);
              break;
            case 0b100:
              e_DIV(cpu, inst);
              break;
            case 0b101:
              e_DIVU(cpu, inst);
              break;
            case 0b110:
              e_REM(cpu, inst);
              break;
            case 0b111:
              e_REMU(cpu, inst);
              break;
            default:
              trap = 3;
            }
            break;
          /* rv32i */
          case 0b0000000:
          case 0b0100000:
            switch (funct3) {
            case 0b000:
              switch (funct7) {
              case 0b0000000:
                e_ADD(cpu, inst);
                break;
              case 0b0100000:
                e_SUB(cpu, inst);
                break;
              default:
                trap = 3;
              }
              break;
            case 0b001:
              e_SLL(cpu, inst);
              break;
            case 0b010:
              e_SLT(cpu, inst);
              break;
            case 0b011:
              e_SLTU(cpu, inst);
              break;
            case 0b100:
              e_XOR(cpu, inst);
              break;
            case 0b101:
              switch (funct7) {
              case 0b0000000:
                e_SRL(cpu, inst);
                break;
              case 0b0100000:
                e_SRA(cpu, inst);
                break;
              default:
                trap = 3;
              }
              break;
            case 0b110:
              e_OR(cpu, inst);
              break;
            case 0b111:
              e_AND(cpu, inst);
              break;
            default:
              trap = 3;
            }
            break;
          }
          break;

        /* FENCE -- device I/O and memory access */
        case 0b0001111:
          e_FENCE(cpu, inst);
          break;

        /* rv64i; I-type  */
        case 0b0011011:
          switch (funct3) {
          case 0b000:
            e_ADDIW(cpu, inst);
            break;
          case 0b001:
            e_SLLIW(cpu, inst);
            break;
          case 0b101:
            switch (funct7) {
            case 0b0000000:
              e_SRLIW(cpu, inst);
              break;
            case 0b0100000:
              e_SRLIW(cpu, inst);
              break;
            }
            break;
          }
          break;

        /* rv64i; R-type (64im) */
        case 0b0111011:
          switch (funct3) {
          case 0b000:
            switch (funct7) {
            case 0b0000000:
              e_ADDW(cpu, inst);
              break;
            case 0b0100000:
              e_SUBW(cpu, inst);
              break;
            case 0b0000001:
              e_MULW(cpu, inst);
              break;
            }
            break;
          case 0b100:
            e_DIVW(cpu, inst);
            break;
          case 0b001:
            e_SLLW(cpu, inst);
            break;
          case 0b101:
            switch (funct7) {
            case 0b0000000:
              e_SRLW(cpu, inst);
              break;
            case 0b0100000:
              e_SRAW(cpu, inst);
              break;
            case 0b0000001:
              e_DIVUW(cpu, inst);
              break;
            }
            break;
          case 0b110:
            e_REMW(cpu, inst);
            break;
          case 0b111:
            e_REMUW(cpu, inst);
            break;
          default:
            trap = 3;
          }
          break;

        /* Zicsr  */
        case 0b1110011:
          switch (funct3) {
          case 0b000:;
            {
              // e_ECALLBREAK(cpu, inst);
              uint64_t csrno = csr(inst);
              if (csrno == 0x105) // WFI (Wait for interrupts)
              {
                cpu->csr[MSTATUS] |= 8;
                cpu->csr[EXTRAFLAGS] |= 4;
                cpu->pc += 4;
                return 1;
              } else if (((csrno & 0xff) == 0x02)) // MRET
              {
                uint64_t startmstatus = cpu->csr[MSTATUS];
                uint64_t startextraflags = cpu->csr[EXTRAFLAGS];
                cpu->csr[MSTATUS] = ((startmstatus & 0x80) >> 4) |
                    ((startextraflags & 3) << 11) | 0x80;
                cpu->csr[EXTRAFLAGS] =
                    (startextraflags & ~3) | ((startmstatus >> 11) & 3);
                cpu->pc = cpu->csr[MEPC] - 4;
              } else {
                switch (csrno) {
                case 0:
                  trap = (cpu->csr[EXTRAFLAGS] & 3) ? (11 + 1) : (8 + 1);
                  return 3;
                  break; // ECALL; 8 = "Environment call from U-mode"; 11 =
                         // "Environment call from M-mode"
                case 1:
                  trap = (3 + 1);
                  break; // EBREAK 3 = "Breakpoint"
                default:
                  trap = (2 + 1);
                  break; // Illegal opcode.
                }
              }
            }
            break;
          case 0b001:;
            e_CSRRW(cpu, image, inst);
            break;
          case 0b010:
            e_CSRRS(cpu, image, inst);
            break;
          case 0b011:
            e_CSRRC(cpu, image, inst);
            break;
          case 0b101:
            e_CSRRWI(cpu, image, inst);
            break;
          case 0b110:
            e_CSRRSI(cpu, image, inst);
            break;
          case 0b111:
            e_CSRRCI(cpu, image, inst);
            break;
          default:
            trap = 3;
          }
          break;

        /* rv32a rv64a */
        case 0b0101111:
          switch (funct3) {
          case 0b010:
            switch (funct7 >> 2) {
            case 0b00010:
              e_LR_W(cpu, image, inst);
              break;
            case 0b00011:
              e_SC_W(cpu, image, inst);
              break;
            case 0b00001:
              e_AMOSWAP_W(cpu, image, inst);
              break;
            case 0b00000:
              e_AMOADD_W(cpu, image, inst);
              break;
            case 0b00100:
              e_AMOXOR_W(cpu, image, inst);
              break;
            case 0b01100:
              e_AMOAND_W(cpu, image, inst);
              break;
            case 0b01000:
              e_AMOOR_W(cpu, image, inst);
              break;
            case 0b10000:
              e_AMOMIN_W(cpu, image, inst);
              break;
            case 0b10100:
              e_AMOMAX_W(cpu, image, inst);
              break;
            case 0b11000:
              e_AMOMINU_W(cpu, image, inst);
              break;
            case 0b11100:
              e_AMOMAXU_W(cpu, image, inst);
              break;
            default:
              trap = 3;
            }
            break;

          case 0b011:
            switch (funct7 >> 2) {
            case 0b00010:
              e_LR_D(cpu, image, inst);
              break;
            case 0b00011:
              e_SC_D(cpu, image, inst);
              break;
            case 0b00001:
              e_AMOSWAP_D(cpu, image, inst);
              break;
            case 0b00000:
              e_AMOADD_D(cpu, image, inst);
              break;
            case 0b00100:
              e_AMOXOR_D(cpu, image, inst);
              break;
            case 0b01100:
              e_AMOAND_D(cpu, image, inst);
              break;
            case 0b01000:
              e_AMOOR_D(cpu, image, inst);
              break;
            case 0b10000:
              e_AMOMIN_D(cpu, image, inst);
              break;
            case 0b10100:
              e_AMOMAX_D(cpu, image, inst);
              break;
            case 0b11000:
              e_AMOMINU_D(cpu, image, inst);
              break;
            case 0b11100:
              e_AMOMAXU_D(cpu, image, inst);
              break;
            default:
              trap = 3;
            }
            break;

          default:
            trap = 3;
            break;
          }
          break;

        /* float/double computation instructions */
        case 0b1000011:
          switch (funct2) {
          case 0b00:
            e_FMADD_S(cpu, inst);
            break;
          case 0b01:
            e_FMADD_D(cpu, inst);
            break;
          }
          break;

        case 0b1000111:
          switch (funct2) {
          case 0b00:
            e_FMSUB_S(cpu, inst);
            break;
          case 0b01:
            e_FMSUB_D(cpu, inst);
            break;
          }
          break;

        case 0b1001011:
          switch (funct2) {
          case 0b00:
            e_FNMSUB_S(cpu, inst);
            break;
          case 0b01:
            e_FNMSUB_D(cpu, inst);
            break;
          }
          break;

        case 0b01001111:
          switch (funct2) {
          case 0b00:
            e_FNMADD_S(cpu, inst);
            break;
          case 0b01:
            e_FNMADD_D(cpu, inst);
            break;
          }
          break;

          /* rv32f */
        case 0b1010011:
          switch (funct7) {
          case 0b0000000:
            e_FADD_S(cpu, inst);
            break;
          case 0b0000100:
            e_FSUB_S(cpu, inst);
            break;
          case 0b0001000:
            e_FMUL_S(cpu, inst);
            break;
          case 0b0001100:
            e_FDIV_S(cpu, inst);
            break;
          case 0b0101100:
            e_FSQRT_S(cpu, inst);
            break;

          case 0b0010000:
            switch (funct3) {
            case 0b000:
              e_FSGNJ_S(cpu, inst);
              break;
            case 0b001:
              e_FSGNJN_S(cpu, inst);
              break;
            case 0b010:
              e_FSGNJX_S(cpu, inst);
              break;
            }
            break;

          case 0b0010100:
            switch (funct3) {
            case 0b000:
              e_FMIN_S(cpu, inst);
              break;
            case 0b001:
              e_FMAX_S(cpu, inst);
              break;
            }
            break;

          case 0b1100000:
            switch (rs2(inst)) {
            case 0b00000:
              e_FCVT_W_S(cpu, inst);
              break;
            case 0b00001:
              e_FCVT_WU_S(cpu, inst);
              break;
            case 0b00010:
              e_FCVT_L_S(cpu, inst);
              break;
            case 0b00011:
              e_FCVT_LU_S(cpu, inst);
              break;
            }
            break;

          case 0b1110000:
            switch (funct3) {
            case 0b000:
              e_FMV_X_W(cpu, inst);
              break;
            case 0b001:
              e_FCLASS_S(cpu, inst);
              break;
            }
            break;

          case 0b1010000:
            switch (funct3) {
            case 0b010:
              e_FEQ_S(cpu, inst);
              break;
            case 0b001:
              e_FLT_S(cpu, inst);
              break;
            case 0b000:
              e_FLE_S(cpu, inst);
              break;
            }
            break;

          case 0b1101000:
            switch (rs2(inst)) {
            case 0b00000:
              e_FCVT_S_W(cpu, inst);
              break;
            case 0b00001:
              e_FCVT_S_WU(cpu, inst);
              break;
            case 0b00010:
              e_FCVT_S_L(cpu, inst);
              break;
            case 0b00011:
              e_FCVT_S_LU(cpu, inst);
              break;
            }
            break;
          case 0b1111000:
            e_FMV_W_X(cpu, inst);
            break;

          /* rv32d */
          case 0b0000001:
            e_FADD_D(cpu, inst);
            break;
          case 0b0000101:
            e_FSUB_D(cpu, inst);
            break;
          case 0b0001001:
            e_FMUL_D(cpu, inst);
            break;
          case 0b0001101:
            e_FDIV_D(cpu, inst);
            break;
          case 0b0101101:
            e_FSQRT_D(cpu, inst);
            break;

          case 0b0010001:
            switch (funct3) {
            case 0b000:
              e_FSGNJ_D(cpu, inst);
              break;
            case 0b001:
              e_FSGNJN_D(cpu, inst);
              break;
            case 0b010:
              e_FSGNJX_D(cpu, inst);
              break;
            }
            break;

          case 0b0010101:
            switch (funct3) {
            case 0b000:
              e_FMIN_D(cpu, inst);
              break;
            case 0b001:
              e_FMAX_D(cpu, inst);
              break;
            }
            break;

          case 0b0100000:
            e_FCVT_S_D(cpu, inst);
            break;
          case 0b0100001:
            e_FCVT_D_S(cpu, inst);
            break;

          case 0b1010001:
            switch (funct3) {
            case 0b010:
              e_FEQ_D(cpu, inst);
              break;
            case 0b001:
              e_FLT_D(cpu, inst);
              break;
            case 0b000:
              e_FLE_D(cpu, inst);
              break;
            }
            break;

          case 0b1110001:
            switch (funct3) {
            case 0b000:
              e_FMV_X_D(cpu, inst);
              break;
            case 0b001:
              e_FCLASS_D(cpu, inst);
              break;
            }
            break;

          case 0b1100001:
            switch (rs2(inst)) {
            case 0b00000:
              e_FCVT_W_D(cpu, inst);
              break;
            case 0b00001:
              e_FCVT_WU_D(cpu, inst);
              break;
            case 0b00010:
              e_FCVT_L_D(cpu, inst);
              break;
            case 0b00011:
              e_FCVT_LU_D(cpu, inst);
              break;
            }
            break;

          case 0b1101001:
            switch (rs2(inst)) {
            case 0b00000:
              e_FCVT_D_W(cpu, inst);
              break;
            case 0b00001:
              e_FCVT_D_WU(cpu, inst);
              break;
            case 0b00010:
              e_FCVT_D_L(cpu, inst);
              break;
            case 0b00011:
              e_FCVT_D_LU(cpu, inst);
              break;
            }
            break;
          case 0b1111001:
            e_FMV_D_X(cpu, inst);
            break;

          default:
            trap = 3;
            break;
          }
          break;

        case 0:
          return 3;

        default:
          trap = 3;
        }

        if (trap) {
          cpu->regs[rd(inst)] = rd_old;
          break;
        }
      }

      cpu->pc += 4;
    }
  // Handle traps and interrupts.
  if (trap) {
    if (trap == 3) {
      fprintf(stdout, "\nInvalid instruction\n");
      return 3;
    } else if (trap == 13) {
      return rval;
    }

    if (trap &
        0x80000000) // If prefixed with 1 in MSB, it's an interrupt, not a trap.
    {
      cpu->csr[MCAUSE] = trap;
      cpu->csr[MTVAL] = 0;
      cpu->pc += 4;
    } else {
      cpu->csr[MCAUSE] = trap - 1;
      cpu->csr[MTVAL] = (trap > 5 && trap <= 8) ? rval : cpu->pc;
    }

    cpu->csr[MEPC] = cpu->pc;
    cpu->csr[MSTATUS] =
        ((cpu->csr[MSTATUS] & 0x08) << 4) | ((cpu->csr[EXTRAFLAGS] & 3) << 11);
    cpu->pc = cpu->csr[MTVEC] - 4;

    cpu->csr[EXTRAFLAGS] |= 3;
    trap = 0;
    cpu->pc += 4;
  }

  cpu->csr[CYCLE] = current_cycle;
  cpu->pc = cpu->pc;

  return 0;
}

void dump_registers(FILE *out, CPU *cpu, uint8_t *ram_image) {
  char *abi[] = {
      // Application Binary Interface registers
      "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0", "s1", "a0",
      "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3", "s4", "s5",
      "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6",
  };

  fprintf(
      out,
      "\nPC:  %#.8lx\t\t[0x%08x] \n",
      cpu->pc,
      *((uint32_t *)(&((uint8_t *)ram_image)[cpu->pc - RAM_OFFSET])));
  for (int i = 0; i < 8; i++) {
    fprintf(out, "%4s: %#-13.2lx\t\t", abi[i], cpu->regs[i]);
    fprintf(out, "%2s: %#-13.2lx\t\t", abi[i + 8], cpu->regs[i + 8]);
    fprintf(out, "%2s: %#-13.2lx\t\t", abi[i + 16], cpu->regs[i + 16]);
    fprintf(out, "%3s: %#-13.2lx\t\t", abi[i + 24], cpu->regs[i + 24]);
    fprintf(out, "\n");
  }
}
