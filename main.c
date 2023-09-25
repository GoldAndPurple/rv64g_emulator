#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "cpu.h"
#include "csr.h"

static void MiniSleep()
{
	usleep(500);
}

static uint64_t GetTimeMicroseconds()
{
	struct timeval tv;
	gettimeofday( &tv, 0 );
	return tv.tv_usec + ((uint64_t)(tv.tv_sec)) * 1000000LL;
}

void read_file(CPU *cpu, uint8_t* ram_image, char *filename) {
  FILE *file;
  uint8_t *buffer;
  unsigned long fileLen;

  file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Unable to open file %s", filename);
  }

  fseek(file, 0, SEEK_END);
  fileLen = ftell(file);
  fseek(file, 0, SEEK_SET);

  fread((ram_image/*  + offset */), fileLen, 1, file);
  fclose(file);
}

struct CPU* core;
uint8_t* ram_image;
FILE* output_f = 0;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: exec <filename>\n");
    exit(1);
  }

  struct CPU cpu;
  core=&cpu;

  core->pc = RAM_OFFSET;
  ram_image = malloc( RAM_SIZE );
  memset( ram_image, 0, RAM_SIZE );

  read_file(core, ram_image, argv[1]);

#ifdef LOGGING
	output_f = fopen ("log.txt", "w");
#endif 

	long long instct = -1;
	int single_step = 0;
	uint64_t i;
	uint64_t lastTime = GetTimeMicroseconds();
  uint32_t elapsedUs;
	int instrs_per_flip = single_step?1:1024;
	for( i = 0; i < instct+1 || instct < 0; i += instrs_per_flip )
	{
		uint64_t* this_ccount = &core->csr[CYCLE];
		uint32_t elapsedUs = 0;
    elapsedUs = GetTimeMicroseconds() - lastTime;
		lastTime += elapsedUs;

		int ret1 = cpu_cycle( core, ram_image, elapsedUs, instrs_per_flip );
		if( single_step ){
			dump_registers(stdout, core, ram_image);
			getchar();
		}
		switch( ret1 )
		{
			case 0: break;
			case 1: MiniSleep(); *this_ccount += instrs_per_flip; break;
			case 3: instct = 0; break;
			default: printf( "Unknown failure\n%08x\n", ret1); break;
		}
	}
  return 0;
}
