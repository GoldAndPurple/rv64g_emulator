#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>

#define RAM_SIZE 1024*1024*8
#define RAM_OFFSET 0x80000000

// #define LOGGING

typedef struct CPU {
    uint64_t regs[32];
    uint32_t pc;
    uint64_t csr[4096];
    double fregs[32];
} CPU;

extern FILE* output_f;

uint64_t csr_read(CPU* cpu, uint8_t *image, uint64_t csr);
void csr_write(CPU* cpu, uint8_t *image, uint64_t csr, uint64_t value);
uint64_t ram_load(CPU* cpu, uint8_t* ram_image, uint64_t addr, uint64_t size);
void ram_store(CPU* cpu, uint8_t* ram_image, uint64_t addr, uint64_t size, uint64_t value);

uint64_t rd(uint32_t inst);
uint64_t rs1(uint32_t inst);
uint64_t rs2(uint32_t inst);
uint64_t rs3(uint32_t inst);
int64_t imm_I(uint32_t inst);
uint64_t imm_IU(uint32_t inst);
uint64_t imm_S(uint32_t inst);
uint64_t imm_B(uint32_t inst);
uint64_t imm_U(uint32_t inst);
int64_t imm_J(uint32_t inst);
uint32_t shamt(uint32_t inst);
uint64_t csr(uint32_t inst);

void dump_registers(FILE *out, CPU *cpu, uint8_t* ram_image);

uint64_t HandleControlStore( uint32_t addy, uint64_t val );
uint64_t HandleControlLoad( uint32_t addy );
void HandleOtherCSRWrite( uint8_t * image, uint16_t csrno, uint64_t value );
int64_t HandleOtherCSRRead( uint8_t * image, uint16_t csrno );

#endif
