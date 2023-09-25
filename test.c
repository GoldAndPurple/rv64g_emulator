int global_empty;
char rostring[] = " is equal to ";

int fact(int n) {
    if (n <= 1)
        return 1;
    else
        return n * fact(n-1);
}

static void string_print( const char * s )
{
	asm volatile( "csrrw x0, 0x138, %0\n" : : "r" (s));
}
static void int_print( int i )
{
	asm volatile( "csrrw x0, 0x136, %0\n" : : "r" (i));
}
static void double_print( double i )
{
	asm volatile( "csrrw x0, 0x13A, %0\n" : : "r" (i));
}

static inline int get_cycle() {
	int ccount;
	asm volatile(".option norvc\ncsrr %0, 0xC00":"=r" (ccount));
	return ccount;
}
static inline int get_time() {
	int t;
	asm volatile(".option norvc\ncsrr %0, 0xC01":"=r" (t));
	return t;
}

void _start(){
    asm volatile(
        "la	sp, _sstack\n"
        "jal	ra, main\n"
        "addi	ra,x0,0\n"
    );
    return;
}

int main() {
    string_print("The factorial of ");
    global_empty = 4;
    int_print(global_empty);
    string_print(rostring);
    int result = fact(global_empty);
    int_print(result);
    string_print("\n");

	int cyclecount_initial = get_cycle();
	int timer_initial = get_time();

	volatile int i;
	for( i = 0; i < 1000000; i++ )
	{
		asm volatile( "nop" );
	}
	int cyclecount = get_cycle() - cyclecount_initial;
	int timer = get_time() - timer_initial;

	string_print( "Processor speed = ");
	double_print( (double)cyclecount / (double)timer );
	string_print( " (MegaCycles / second)\n");

    return 0;
}
