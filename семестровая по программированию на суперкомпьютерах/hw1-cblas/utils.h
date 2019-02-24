#include <cstdlib>
#include "magma_v2.h"

#define ZFILL_MIN_ELEM_VAL -5.
#define ZFILL_MAX_ELEM_VAL 5.

void qsync_all(magma_queue_t* queues, magma_int_t nq);

double zl1(magmaDoubleComplex* arr, size_t nelems);
double zl2(magmaDoubleComplex* arr, size_t nelems);

void zsetI(magmaDoubleComplex* hA, magma_int_t n);
void zrandfill_matrix(magmaDoubleComplex* A, magma_int_t m, magma_int_t n);

void zrestore_squareQ(magmaDoubleComplex* hQ, magmaDoubleComplex* hHR, magmaDoubleComplex* hTau, magma_int_t n);
std::pair<double, double> zresidual_squareQR(magmaDoubleComplex* hA, magmaDoubleComplex* hHR, magmaDoubleComplex* hTau, magma_int_t n);