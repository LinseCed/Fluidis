#ifndef LBM_H
#define LBM_H

#define NX 32
#define NY 32
#define NZ 32
#define NDIR 19
#define STEPS 100

#include <stdio.h>

void lbm_init();
void lbm_step(double f_loc[NX][NY][NZ][NDIR]);

#endif
