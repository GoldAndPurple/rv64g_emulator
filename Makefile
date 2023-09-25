
CC = gcc

APP_NAME = main

INCLUDE_DIRS = -I include

LIB_SRC_FILES = $(shell find src/ -name '*.c')

SRC_FILES += $(LIB_SRC_FILES)

MAKE_CMD = $(CC) $(APP_NAME).c $(SRC_FILES) $(INCLUDE_DIRS) -O2 -Wno-implicit-function-declaration -lm -g -o $(APP_NAME)

all:
	$(MAKE_CMD)

clean:
	rm -f $(APP_NAME)

RISC-TOOL = /opt/riscv/bin/riscv64-unknown-elf-

C = -Os -nostdlib -fno-stack-protector -fdata-sections -ffunction-sections -static-libgcc -static -mcmodel=medany
LD = -T link.lds -Wl,--gc-sections,--print-gc-sections

test: test.c
	$(RISC-TOOL)gcc $(C) $(LD) -march=rv64imafd -mabi=lp64d test.c -o test.elf
	$(RISC-TOOL)objcopy test.elf -O binary test.bin
	$(RISC-TOOL)readelf -Wl -a test.elf > elfheader
	$(RISC-TOOL)objdump -d test.elf > elfinstr

riscv-tests: 
	find ./tests/ -name "rv64*.hex" | xargs -t -n 1 ./main