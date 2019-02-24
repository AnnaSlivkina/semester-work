#include "utils.h"
#include <cstring>
#include <cblas.h>

/*
 * Sync with all given magma queries
 */
void qsync_all(magma_queue_t* queues, magma_int_t nq) {
    for (magma_int_t i = 0; i < nq; ++i) {
        magma_queue_sync(queues[i]);
    }
}

/*
 * Compute l1 vector norm
 */
double zl1(magmaDoubleComplex* arr, size_t nelems) {
    long double l1 = 0;
    for (magmaDoubleComplex* p = arr; p <  arr + nelems; ++p) {
        l1 += abs(p->x) + abs(p->y);
    }
    return l1;
}

/*
 * Compute l2 vector norm
 */
double zl2(magmaDoubleComplex* arr, size_t nelems) {
    long double l2 = 0;
    for (magmaDoubleComplex* p = arr; p < arr + nelems; ++p) {
        l2 = p->x * p->x + p->y * p->y;
    }
    return sqrt(l2);
}

/*
 * Set Square Matrix to I
 */
void zsetI(magmaDoubleComplex* hA, magma_int_t n) {
    for (size_t j = 0; j < (size_t)n; ++j) {
        for (size_t i = 0; i < (size_t)n; ++i) {
            hA[i + j * n] = {i == j? 1.: 0., 0.};
        }
    }
}

/*
 *  Fill magmaDoubleCompex matrix of size m x n with random values of
 *  Re and Im parts in range [MIN_ELEM_VAL, MAX_ELEM_VAL]
 */
void zrandfill_matrix(magmaDoubleComplex* A, magma_int_t m, magma_int_t n) {
    assert(n > 0);
    double range = ZFILL_MAX_ELEM_VAL - ZFILL_MIN_ELEM_VAL;
    for (size_t i = 0; i < (size_t)m; ++i) {
        for (size_t j = 0; j < (size_t)n; ++j) {
            A[i + j * m].x = (range * rand()) / RAND_MAX - range / 2;
            A[i + j * m].y = (range * rand()) / RAND_MAX - range / 2;
        }
    }
}

/*
 * Restore Q from square HR matrix
 */
void zrestore_squareQ(magmaDoubleComplex* hQ, magmaDoubleComplex* hHR, magmaDoubleComplex* hTau, magma_int_t n) {

    magmaDoubleComplex* hV = nullptr;
    magmaDoubleComplex* hVT = nullptr;
    magmaDoubleComplex* hH = nullptr;
    magmaDoubleComplex* hT = nullptr;
    magma_int_t rv = MAGMA_SUCCESS;

    rv = magma_zmalloc_cpu(&hV, (size_t)n);
    assert(rv == MAGMA_SUCCESS);
    rv = magma_zmalloc_cpu(&hVT, (size_t)n);
    assert(rv == MAGMA_SUCCESS);
    rv = magma_zmalloc_cpu(&hH, n * (size_t)n);
    assert(rv == MAGMA_SUCCESS);
    rv = magma_zmalloc_cpu(&hT, n * (size_t)n);
    assert(rv == MAGMA_SUCCESS);

    zsetI(hQ, n);

    magmaDoubleComplex one = {1, 0};
    magmaDoubleComplex zero = {0, 0};
    for (size_t i = 0; i < (size_t)n; ++i) {
        for (size_t j = 0; j < (size_t)n; ++j) {
            if (j < i) {
                hVT[j] = hV[j] = zero;
            } else if (j == i) {
                hVT[j] = hV[j] = one;
            } else {
                hVT[j] = hV[j] = hHR[j + i * n];
                hVT[j].y = - hHR[j + i * n].y;
            }
        }

        zsetI(hH, n);

        magmaDoubleComplex neg_tau = {-hTau[i].x, -hTau[i].y};

        cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
                    n, n, 1,
                    &neg_tau, hV, n, hVT, 1,
                    &one, hH, n);

        memcpy(hT, hQ, sizeof(magmaDoubleComplex) * n * n);

        cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
                    n, n, n,
                    &one, hT, n, hH, n,
                    &zero, hQ, n);
    }

    magma_free_cpu(hV); hV = nullptr;
    magma_free_cpu(hVT); hVT = nullptr;
    magma_free_cpu(hH); hH = nullptr;
    magma_free_cpu(hT); hT = nullptr;
}

/*
 * Replace hA with error of hA QR-decomposition, given hHR, hTau
 */
std::pair<double, double> zresidual_squareQR(magmaDoubleComplex* hA, magmaDoubleComplex* hHR, magmaDoubleComplex* hTau, magma_int_t n) {
    magmaDoubleComplex one = {1, 0};
    magmaDoubleComplex neg_one = {-1, 0};

    double l1_norm = zl1(hA, n * (size_t)n);
    double l2_norm = zl2(hA, n * (size_t)n);

    magmaDoubleComplex* hQ = nullptr;
    magma_int_t rv = magma_zmalloc_cpu(&hQ, n * (size_t)n);
    assert(rv == MAGMA_SUCCESS);

    zrestore_squareQ(hQ, hHR, hTau, n);

    for (size_t j = 0; j < (size_t)n; ++j) {
        for (size_t i = j + 1; i < (size_t)n; ++i) {
            hHR[i + j * n] = {0, 0};
        }
    }

    cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
                n, n, n,
                &one, hQ, n, hHR, n,
                &neg_one, hA, n);

    magma_free_cpu(hQ); hQ = nullptr;

    return {zl1(hA, n * (size_t)n) / l1_norm, zl1(hA, n * (size_t)n) / l2_norm};
}