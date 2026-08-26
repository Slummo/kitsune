#define KS_MEM_IMPL

#include <ks/math.h>

static double cos_cb(double x, KS_UNUSED void* args) {
    return cos(x);
}

static double gaussian(double x, KS_UNUSED void* args) {
    return exp(-(x * x));
}

static double cylinder_top_cb(double x, double y, KS_UNUSED void* args) {
    double dist_sq = x * x + y * y;
    return (dist_sq <= 1.0f) ? 1.0f : 0.0f;
}

static double const_1(KS_UNUSED const double* vars, KS_UNUSED int32_t dims, KS_UNUSED void* args) {
    return 1.0;
}

static double sphere_5d(const double* vars, int32_t dims, KS_UNUSED void* args) {
    double sum_sq = 0.0;

    for (int32_t i = 0; i < dims; ++i) {
        sum_sq += vars[i] * vars[i];
    }

    return (sum_sq <= 1.0f) ? 1.0f : 0.0f;
}

int main(void) {
    double res;

    res = ks_deriv_1d(cos_cb, KS_PI_2, 1e-6, NULL);
    ks_print("d/dx cos(pi/2) = -sin(pi/2) = %.1lf (expected %.1lf)", res, -1.0);

    res = ks_integ_1d(gaussian, -100.0, 100.0, 1 << 10, NULL);
    ks_print("integral [-inf, +inf] gaussian = %.11lf (expected %.11lf)", res, KS_SQRTPI);

    res = ks_integ_2d(cylinder_top_cb, -1.0f, 1.0f, 500, -1.0f, 1.0f, 500, NULL);
    ks_print("2D Circle Area = %lf (expected %lf)", res, KS_PI);

    double mins1[] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double maxs1[] = {2.0, 2.0, 2.0, 2.0, 2.0};
    res = ks_integ_nd(const_1, 5, mins1, maxs1, 1 << 20, NULL);
    ks_print("5D Cube Volume %lf (expected 32.0)", res);

    double mins2[] = {-1, -1, -1, -1, -1};
    double maxs2[] = {1, 1, 1, 1, 1};
    res = ks_integ_nd(sphere_5d, 5, mins2, maxs2, 1 << 20, NULL);
    ks_print("5D Sphere Volume = %lf (expected ~5.2637)", res);

    return EXIT_SUCCESS;
}