#include "slave.h"
#include <simd.h>

#define COL(x) (x & 0x07)           //x是指从核id号
#define ROW(x) ((x & 0x38) >> 3)
#define REG_PUTR(var, dst) asm volatile ("putr %0,%1"::"r"(var),"r"(dst))
#define REG_PUTC(var, dst) asm volatile ("putc %0,%1"::"r"(var),"r"(dst))
#define REG_GETR(var) asm volatile ("getr %0":"=r"(var))
#define REG_GETC(var) asm volatile ("getc %0":"=r"(var))

typedef struct
{
	double *A;
	double *B;
	int row;
	int col;

} Param;

void func_reg(Param *host) {

	double *A = host->A;
	double *B = host->B;
	int row = host->row;
	int col = host->col;
	int i;

	volatile unsigned long get_reply, put_reply;
	volatile int id =  athread_get_id(-1);
	int row_id, col_id;

	double A_slave[4];
	double A_slave_tmp[4];

	doublev4 A_v4_input;
	doublev4 A_v4_output;

	row_id = ROW(id);
	col_id = COL(id);

    get_reply = 0;
    athread_get(PE_MODE, A + id * 4, A_slave,  4 * sizeof(double), &get_reply, 0, 0, 0);
    while (get_reply != 1);

	if (row_id % 2 == 0) {
		simd_load(A_v4_input, &A_slave[0]);
		REG_PUTC(A_v4_input, row_id + 1);
	}
	else {
		REG_GETC(A_v4_output);
		simd_store(A_v4_output, &A_slave_tmp[0]);
	}

	athread_syn(ARRAY_SCOPE, 0xffff);

	if (row_id % 2 != 0) {

		put_reply = 0;
		athread_put(PE_MODE, A_slave_tmp, A + id * 4, 4 * sizeof(double), &put_reply, 0, 0);
		while (put_reply != 1);

	}




}

