#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>
#include <string.h>
#include "magma_v2.h"
#include "cblas.h"

#include "utils.h"

#define  min(a,b)   (((a)<(b))?(a):(b))
#define  max(a,b)   (((a)<(b))?(b):(a))

/*
 * All required resources
 */
#define MAX_GPUS 256 // large enough number

typedef struct resources_t {
    magma_int_t m;
    magma_int_t n;
    magma_int_t nb;
    magma_int_t ndevs;
    magma_device_t devices[MAX_GPUS];
    magma_queue_t queues[MAX_GPUS];
    magmaDoubleComplex_ptr dlA[MAX_GPUS];
    magmaDoubleComplex* hA;
    magmaDoubleComplex* hHR;
    magmaDoubleComplex* hQ;
    magmaDoubleComplex* hTau;
} *RESOURCES;

/*
 * Alloc & init for magma_zgeqrf2_mgpu
 */
RESOURCES zgeqrf2_mgpu_init(magma_int_t m, magma_int_t n) {
    assert(m > 0 && n > 0);
    auto res = new resources_t;

    res->m = m;
    res->n = n;
    res->nb = magma_get_zgeqrf_nb(m, n);
    magma_getdevices(res->devices, MAX_GPUS, &res->ndevs);

    for (magma_int_t i = 0; i < res->ndevs; ++i) {
        magma_queue_create(res->devices[i], res->queues + i);
        magma_setdevice(res->devices[i]);
        magma_int_t rv = magma_zmalloc(res->dlA + i, m * (size_t)res->nb);
        assert(rv == MAGMA_SUCCESS);
    }
    magma_int_t rv = magma_zmalloc_cpu(&res->hA, m * (size_t)n);
    assert(rv == MAGMA_SUCCESS);
    rv = magma_zmalloc_cpu(&res->hHR, m * (size_t)n);
    assert(rv == MAGMA_SUCCESS);
    rv = magma_zmalloc_cpu(&res->hQ, m * (size_t)n);
    assert(rv == MAGMA_SUCCESS);
    rv = magma_zmalloc_cpu(&res->hTau, (size_t)min(m, n));
    assert(rv == MAGMA_SUCCESS);

    zrandfill_matrix(res->hA, m, n);
    magma_zsetmatrix_1D_col_bcyclic(res->ndevs, m, n, res->nb, res->hA, m, res->dlA, m, res->queues);
    qsync_all(res->queues, res->ndevs);

    return res;
}

void zgeqrf2_mgpu_free_gpu(RESOURCES res) {
    for (magma_int_t i = 0; i < res->ndevs; ++i) {
        if (res->dlA[i] != nullptr) {
            magma_free(res->dlA[i]); res->dlA[i] = nullptr;
            magma_queue_destroy(res->queues[i]); res->queues[i] = nullptr;
        }
    }
}

void zgeqrf2_mgpu_free(RESOURCES res) {
    magma_free_cpu(res->hA); res->hA = nullptr;
    magma_free_cpu(res->hHR); res->hHR = nullptr;
    magma_free_cpu(res->hQ); res->hQ = nullptr;
    magma_free_cpu(res->hTau); res->hTau = nullptr;

    zgeqrf2_mgpu_free_gpu(res);

    delete res;
}

/*
 * Run magma_zgeqrf2_mgpu, return elapsed walltime, put result to res->hHR,
 */
double zgeqrf2_mgpu_wtime(RESOURCES res) {
    magma_int_t info = 0;

    double t0 = magma_wtime();
    magma_int_t rv = magma_zgeqrf2_mgpu(res->ndevs, res->m, res->n, res->dlA, res->m, res->hTau, &info);
    double t1 = magma_wtime();

    assert(rv == MAGMA_SUCCESS);
    assert(info == 0);

    magma_zgetmatrix_1D_col_bcyclic(res->ndevs, res->m, res->n, res->nb, res->dlA, res->m, res->hHR, res->m, res->queues);
    qsync_all(res->queues, res->ndevs);

    return t1 - t0;
}

void zgeqrf2_mgpu_test(magma_int_t m, magma_int_t n) {
    assert(m == n);

    RESOURCES res = zgeqrf2_mgpu_init(m, n);
    double time = zgeqrf2_mgpu_wtime(res);
    zgeqrf2_mgpu_free_gpu(res);

    auto errs = zresidual_squareQR(res->hA, res->hHR, res->hTau, res->n);

    zgeqrf2_mgpu_free(res); res = nullptr;
    printf("%lf,%d,%d,%lf,%lf\n", time, m, n, errs.first, errs.second);
}

int main(int argc, char* argv[]) {
    assert(argc == 2);

    char* endptr = nullptr;
    long n = strtol(argv[1], &endptr,0);
    assert(isspace(*endptr) || *endptr == '\0');

    magma_init();
    zgeqrf2_mgpu_test((magma_int_t)n, (magma_int_t)n);
    magma_finalize();

    return 0;
}
