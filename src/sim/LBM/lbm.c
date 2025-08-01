#include "lbm.h"

double f[NX][NY][NZ][NDIR];
double f_temp[NX][NY][NZ][NDIR];

const double tau = 1.5;
const double omega = 1.0 / tau;

const int cx[NDIR] = {  0, 1,-1, 0, 0, 0, 0, 1,-1, 1,-1, 1,-1, 0, 0, 0, 0, 1,-1 };
const int cy[NDIR] = {  0, 0, 0, 1,-1, 0, 0, 1,-1,-1, 1, 0, 0, 1,-1, 1,-1, 0, 0 };
const int cz[NDIR] = {  0, 0, 0, 0, 0, 1,-1, 0, 0, 1,-1, 1,-1, 1,-1,-1, 1,-1, 1 };

const double w[NDIR] = {
  1.0 / 3.0,
  1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0, 1.0 / 18.0,
  1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0,
  1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0,
  1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0, 1.0 / 36.0
};

int solid[NX][NY][NZ];

int inside(int x, int y, int z) {
    return x >= 0 && x < NX && y >= 0 && y < NY && z >= 0 && z < NZ;
}

void lbm_init() {
    for (int x = 0; x < NX; x++) {
        for (int y = 0; y < NY; y++) {
            for (int z = 0; z < NZ; z++) {
                solid[x][y][z] = 0;

                double rho = 1.0;
                double ux = 0.1; // TEST init velocity
                double uy = 0.0;
                double uz = 0.0;

                for (int i = 0; i < NDIR; i++) {
                    double cu = 3.0 * (cx[i] * ux + cy[i] * uy + cz[i] * uz);
                    f[x][y][z][i] = w[i] * rho * (1.0 + cu + 0.5 * cu * cu - 1.5 * (ux * ux + uy * uy + uz * uz));
                }

                if (x == 0 || x == NX - 1 || y == 0 || y == NY - 1 || z == 0 || z == NZ - 1) {
                    solid[x][y][z] = 1;
                }
            }
        }
    }
}

void compute_macros(int x, int y, int z, double* rho, double* ux, double* uy, double* uz) {
    *rho = 0.0;
    *ux = 0.0;
    *uy = 0.0;
    *uz = 0.0;

    for (int i = 0; i < NDIR; i++) {
        *rho += f[x][y][z][i];
        *ux += f[x][y][z][i] * cx[i];
        *uy += f[x][y][z][i] * cy[i];
        *uz += f[x][y][z][i] * cz[i];
    }

    *ux /= *rho;
    *uy /= *rho;
    *uz /= *rho;
}

void lbm_step(double f_loc[NX][NY][NZ][NDIR]) {
    for (int x = 0; x < NX; x++) {
        for (int y = 0; y < NY; y++) {
            for (int z = 0; z < NZ; z++) {
                if (solid[x][y][z]) continue;

                double rho, ux, uy, uz;
                compute_macros(x, y, z, &rho, &ux, &uy, &uz);
                printf("rho: %f, ux: %f \n", rho, ux);
                for (int i = 0; i < NDIR; i++) {
                    double cu = 3.0 * (cx[i] * ux + cy[i] * uy + cz[i] * uz);
                    double feq = w[i] * rho * (1.0 + cu + 0.5 * cu * cu - 1.5 * (ux * ux + uy * uy + uz * uz));
                    f_temp[x][y][z][i] = f[x][y][z][i] * (1.0 - omega) + omega * feq;
                }
            }
        }
    }

    for (int x = 0; x < NX; x++) {
        for (int y = 0; y < NY; y++) {
            for (int z = 0; z < NZ; z++) {
                for (int i = 0; i < NDIR; i++) {
                    int x1 = x - cx[i];
                    int y1 = y - cy[i];
                    int z1 = z - cz[i];

                    if (inside(x1, y1, z1) && !solid[x][y][z]) {
                        if (!solid[x1][y1][z1]) {
                            f[x][y][z][i] = f_temp[x1][y1][z1][i];
                        } else {
                            int opp = (i < 7) ? i ^ 1 : i ^ 2;
                            f[x][y][z][i] = f_temp[x][y][z][opp];
                        }
                    }
                }
            }
        }
    }
    f_loc = f;
}
