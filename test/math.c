#define KS_CORE_IMPL
#define KS_MATH_IMPL

#include <ks/core.h>
#include <ks/math.h>

#include <unity.h>
#include <math.h>
#include <stdlib.h>

#define TEST_EPS 1e-4f
#define TEST_DEPS 1e-6

void setUp(void) {
}
void tearDown(void) {
}

/* ========================================================================= */
/* UTILITIES                                                                 */
/* ========================================================================= */

void test_util_log2ld(void) {
    TEST_ASSERT_EQUAL_UINT64(0, log2ld(1));
    TEST_ASSERT_EQUAL_UINT64(1, log2ld(2));
    TEST_ASSERT_EQUAL_UINT64(3, log2ld(8));
    TEST_ASSERT_EQUAL_UINT64(10, log2ld(1024));
}

void test_util_bit_reverse(void) {
    // Reversing bits of 1 in a 3-bit field (m=8 -> log2(8)=3) -> 1 is 001, reversed is 100 (4)
    TEST_ASSERT_EQUAL_UINT64(4, bit_reverse(1, 8));
}

/* ========================================================================= */
/* MACRO-GENERATED VECTOR FUNCTIONS (Tested on ks_vec4)                      */
/* ========================================================================= */

void test_vec4_initialization(void) {
    ks_vec4 v1;
    ks_vec4_zeroinit(&v1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, v1.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, v1.w);

    ks_vec4 v2 = ks_vec4_zeronew();
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, v2.y);

    ks_vec4 v3 = KS_VEC4(1.0f, 2.0f, 3.0f, 4.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, v3.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, v3.w);

    ks_vec4 v4 = KS_VEC4(5.0f, 6.0f, 7.0f, 8.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 7.0f, v4.z);

    ks_vec4_fillinit(&v4, 9.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 9.0f, v4.y);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 9.0f, v4.w);
}

void test_vec4_arithmetic(void) {
    ks_vec4 v1 = KS_VEC4(1.0f, 2.0f, 3.0f, 4.0f);
    ks_vec4 v2 = KS_VEC4(2.0f, 3.0f, 4.0f, 5.0f);
    ks_vec4 out;

    ks_vec4_add(&out, &v1, &v2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 3.0f, out.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 9.0f, out.w);

    ks_vec4_addi(&v1, &v2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 3.0f, v1.x);

    ks_vec4_sub(&out, &v1, &v2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, out.x);

    ks_vec4_subi(&v1, &v2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, v1.x);

    ks_vec4_neg(&out, &v1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, out.x);

    ks_vec4_negi(&v1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, v1.x);

    ks_vec4_smul(&out, &v2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, out.x);

    ks_vec4_smuli(&v2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, v2.x);

    ks_vec4_sdiv(&out, &v2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, out.x);

    ks_vec4_sdivi(&v2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, v2.x);
}

void test_vec4_geometry(void) {
    ks_vec4 v1 = KS_VEC4(1.0f, 2.0f, 2.0f, 4.0f);
    ks_vec4 v2 = KS_VEC4(2.0f, 0.0f, 0.0f, 0.0f);

    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, ks_vec4_dot(&v1, &v2));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 25.0f, ks_vec4_length_sq(&v1));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 5.0f, ks_vec4_length(&v1));

    ks_vec4 out;
    ks_vec4_norm(&out, &v1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.2f, out.x);

    ks_vec4_normi(&v1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.2f, v1.x);
    TEST_ASSERT_TRUE(ks_vec4_isunitary(&v1));

    ks_vec4 v3 = KS_VEC4(0.0f, 1.0f, 0.0f, 0.0f);
    TEST_ASSERT_TRUE(ks_vec4_iscanonic(&v3));

    ks_vec4 p1 = KS_VEC4(0.0f, 0.0f, 0.0f, 0.0f);
    ks_vec4 p2 = KS_VEC4(0.0f, 3.0f, 4.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 25.0f, ks_vec4_dist_sq(&p1, &p2));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 5.0f, ks_vec4_dist(&p1, &p2));

    ks_vec4 lerped;
    ks_vec4_lerp(&lerped, &p1, &p2, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.5f, lerped.y);

    ks_vec4_lerpi(&p1, &p2, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.5f, p1.y);
}

void test_vec4_access_and_comparison(void) {
    ks_vec4 v1 = KS_VEC4(1.0f, 2.0f, 3.0f, 4.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 3.0f, ks_vec4_get(&v1, 2));

    ks_vec4_seti(&v1, 2, 9.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 9.0f, v1.z);

    float* ptr = ks_vec4_ptr(&v1);
    ptr[0] = 5.0f;
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 5.0f, v1.x);

    ks_vec4 v2 = KS_VEC4(5.0f, 2.0f, 9.0f, 4.0f);
    TEST_ASSERT_TRUE(ks_vec4_equals(&v1, &v2));
}

void test_vec3_cross_product(void) {
    ks_vec3 i = KS_VEC3(1.0f, 0.0f, 0.0f);
    ks_vec3 j = KS_VEC3(0.0f, 1.0f, 0.0f);
    ks_vec3 out;

    ks_vec3_cross(&out, &i, &j);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, out.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, out.y);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, out.z);

    ks_vec3_crossi(&i, &j);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, i.z);
}

void test_mat_transposition(void) {
    ks_mat3 m = ks_mat3_idnew();
    ks_mat3_set(&m, 1, 0, 5.0f);  // Set Row 0, Col 1

    ks_mat3 m_trans;
    ks_mat3_transp(&m_trans, &m);

    // After transpose, it should be at Row 1, Col 0
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 5.0f, ks_mat3_get(&m_trans, 0, 1));

    // In-place
    ks_mat3_transpi(&m);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 5.0f, ks_mat3_get(&m, 0, 1));
}

void test_mat4_initialization_and_access(void) {
    ks_mat4 m1;
    ks_mat4_zeroinit(&m1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, m1.data[15]);

    ks_mat4 m2 = ks_mat4_zeronew();
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, m2.data[0]);

    ks_mat4_idinit(&m1);
    TEST_ASSERT_TRUE(ks_mat4_isidentity(&m1));

    ks_mat4 m3 = ks_mat4_idnew();
    TEST_ASSERT_TRUE(ks_mat4_isidentity(&m3));

    ks_mat4_fillinit(&m2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, ks_mat4_get(&m2, 3, 3));

    ks_mat4_set(&m2, 1, 2, 5.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 5.0f, ks_mat4_get(&m2, 1, 2));

    ks_vec4 v = KS_VEC4(1.0f, 2.0f, 3.0f, 4.0f);
    ks_mat4_setc(&m2, 0, &v);
    ks_vec4 out_c = ks_mat4_getc(&m2, 0);
    TEST_ASSERT_TRUE(ks_vec4_equals(&v, &out_c));

    ks_mat4_setr(&m2, 0, &v);
    ks_vec4 out_r = ks_mat4_getr(&m2, 0);
    TEST_ASSERT_TRUE(ks_vec4_equals(&v, &out_r));

    float* ptr = ks_mat4_ptr(&m2);
    ptr[0] = 9.0f;
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 9.0f, m2.data[0]);
}

void test_mat4_arithmetic_and_shapes(void) {
    ks_mat4 id = ks_mat4_idnew();
    ks_mat4 out;

    ks_mat4_add(&out, &id, &id);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, ks_mat4_get(&out, 0, 0));

    ks_mat4_addi(&out, &id);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 3.0f, ks_mat4_get(&out, 0, 0));

    ks_mat4_sub(&out, &out, &id);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, ks_mat4_get(&out, 0, 0));

    ks_mat4_subi(&out, &id);
    TEST_ASSERT_TRUE(ks_mat4_equals(&out, &id));

    ks_mat4_neg(&out, &id);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, ks_mat4_get(&out, 0, 0));

    ks_mat4_negi(&out);
    TEST_ASSERT_TRUE(ks_mat4_equals(&out, &id));

    ks_mat4_smul(&out, &id, 4.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, ks_mat4_get(&out, 0, 0));

    ks_mat4_smuli(&out, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, ks_mat4_get(&out, 0, 0));

    ks_mat4_sdiv(&out, &out, 2.0f);
    TEST_ASSERT_TRUE(ks_mat4_equals(&out, &id));

    ks_mat4_sdivi(&out, 1.0f);
    TEST_ASSERT_TRUE(ks_mat4_equals(&out, &id));

    TEST_ASSERT_TRUE(ks_mat4_issymmetric(&id));
    TEST_ASSERT_TRUE(ks_mat4_isdiagonal(&id));
    TEST_ASSERT_TRUE(ks_mat4_isuppertri(&id));
    TEST_ASSERT_TRUE(ks_mat4_islowertri(&id));

    ks_mat4 m2 = ks_mat4_idnew();
    ks_mat4_set(&m2, 0, 1, 5.0f);  // Not upper triangular, not diagonal
    TEST_ASSERT_FALSE(ks_mat4_isuppertri(&m2));
    TEST_ASSERT_FALSE(ks_mat4_isdiagonal(&m2));
}

void test_mat4_multiplication_and_transforms(void) {
    ks_mat4 identity = ks_mat4_idnew();
    ks_mat4 translation = ks_mat4_idnew();
    ks_mat4 rotation = ks_mat4_idnew();
    ks_mat4 scale = ks_mat4_idnew();

    ks_vec3 pos = KS_VEC3(10.0f, 0.0f, 0.0f);
    ks_vec3 scale_vec = KS_VEC3(2.0f, 2.0f, 2.0f);

    ks_mat4_transl(&translation, &pos);
    ks_mat4_scale(&scale, &scale_vec);

    // Rotate 90 degrees around Z
    ks_vec3 z_axis = KS_VEC3(0.0f, 0.0f, 1.0f);
    ks_mat4_rot(&rotation, &z_axis, KS_PI_2);

    ks_mat4 model = ks_mat4_model(&translation, &rotation, &scale);

    ks_vec4 v = KS_VEC4(1.0f, 1.0f, 1.0f, 1.0f);
    ks_vec4 vout;
    ks_mat4_mulv(&vout, &model, &v);

    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 8.0f, vout.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, vout.y);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, vout.z);
}

void test_mat4_projection(void) {
    ks_mat4 persp;
    ks_mat4_perspective(&persp, KS_PI_2, 16.0f / 9.0f, 0.1f, 100.0f);

    ks_vec4 pt = KS_VEC4(0.0f, 0.0f, -50.0f, 1.0f);  // Point in front of camera
    ks_vec4 pt_proj;

    ks_mat4_mulv(&pt_proj, &persp, &pt);
    ks_vec4_sdivi(&pt_proj, pt_proj.w);  // Perspective divide

    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, pt_proj.x);      // Centered X
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, pt_proj.y);      // Centered Y
    TEST_ASSERT_TRUE(pt_proj.z > -1.0f && pt_proj.z < 1.0f);  // Inside NDC depth
}

/* ========================================================================= */
/* EXPLICIT MATRIX FUNCTIONS                                                 */
/* ========================================================================= */

void test_mat2_explicit(void) {
    ks_mat2 m = ks_mat2_idnew();
    ks_mat2_set(&m, 0, 0, 2.0f);
    ks_mat2_set(&m, 1, 1, 3.0f);

    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 6.0f, ks_mat2_det(&m));

    ks_mat2 inv;
    ks_mat2_inv(&inv, &m);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.5f, ks_mat2_get(&inv, 0, 0));

    ks_mat2_invi(&m);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f / 3.0f, ks_mat2_get(&m, 1, 1));

    ks_mat2 rot;
    ks_mat2_mkrot(&rot, KS_PI_2);  // Explicitly create the rotation matrix instead of inplace
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, ks_mat2_get(&rot, 1, 0));  // -sin(90)

    ks_mat2 scl = ks_mat2_idnew();
    ks_vec2 sv = KS_VEC2(2.0f, 4.0f);
    ks_mat2_scale(&scl, &sv);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, ks_mat2_get(&scl, 0, 0));
}

void test_mat3_explicit(void) {
    ks_mat3 m = ks_mat3_idnew();
    ks_vec2 v2 = KS_VEC2(10.0f, 20.0f);
    ks_mat3_transl(&m, &v2);

    ks_vec3 p_pos = KS_VEC3(1.0f, 1.0f, 1.0f);  // Represent as 2D position in homogeneous coordinates
    ks_vec3 p_out;
    ks_mat3_mulv(&p_out, &m, &p_pos);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 11.0f, p_out.x);

    ks_vec3 p_dir = KS_VEC3(1.0f, 1.0f, 0.0f);  // Represent as 2D direction in homogeneous coordinates
    ks_mat3_mulv(&p_out, &m, &p_dir);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, p_out.x);  // Ignored translation

    ks_vec3 s3 = KS_VEC3(10.0f, 20.0f, 1.0f);
    ks_mat3_scale(&m, &s3);
    ks_mat3_rotz(&m, KS_PI);  // Use Z-axis rotation to act on the XY plane

    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 200.0f, ks_mat3_det(&m));  // Det changed via scale and rot
    ks_mat3_invi(&m);                                             // Should not crash

    ks_mat3 ortho;
    ks_mat3_ortho(&ortho, 0.0f, 800.0f, 600.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f / 800.0f, ks_mat3_get(&ortho, 0, 0));

    ks_mat3 look;
    ks_vec2 p2d = KS_VEC2(1.0f, 1.0f);
    ks_mat3_lookat(&look, &p2d, &v2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat3_det(&look));  // Rotation matrix
}

void test_mat4_explicit(void) {
    ks_mat4 m = ks_mat4_idnew();
    ks_vec3 v3 = KS_VEC3(10.0f, 20.0f, 30.0f);
    ks_mat4_transl(&m, &v3);

    ks_vec4 p_pos = KS_VEC4(1.0f, 1.0f, 1.0f, 1.0f);
    ks_vec4 p_out;
    ks_mat4_mulv(&p_out, &m, &p_pos);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 11.0f, p_out.x);

    ks_vec4 p_dir = KS_VEC4(1.0f, 1.0f, 1.0f, 0.0f);
    ks_mat4_mulv(&p_out, &m, &p_dir);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, p_out.x);  // Ignored translation

    ks_mat4 persp;
    ks_mat4_perspective(&persp, KS_PI_2, 16.0f / 9.0f, 0.1f, 100.0f);
    ks_vec4 pt = KS_VEC4(0.0f, 0.0f, -50.0f, 1.0f);
    ks_vec4 pt_proj;

    // Manual projection pipeline substituting removed `ks_mat4_project_vec3`
    ks_mat4_mulv(&pt_proj, &persp, &pt);
    ks_vec4_sdivi(&pt_proj, pt_proj.w);                   // Perspective divide
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, pt_proj.x);  // Center of screen

    ks_mat4 ortho;
    ks_mat4_ortho(&ortho, 0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f / 800.0f, ks_mat4_get(&ortho, 0, 0));

    ks_mat4 look;
    ks_vec3 center = KS_VEC3(0.0f, 0.0f, 0.0f);
    ks_vec3 up = KS_VEC3(0.0f, 1.0f, 0.0f);
    ks_mat4_lookat(&look, &v3, &center, &up);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat4_det(&look));

    ks_mat4 rot = ks_mat4_idnew();
    ks_mat4_rot(&rot, &up, KS_PI);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, ks_mat4_get(&rot, 0, 0));

    ks_mat4 scl = ks_mat4_idnew();
    ks_mat4_scale(&scl, &v3);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 10.0f, ks_mat4_get(&scl, 0, 0));

    float det = ks_mat4_det(&scl);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 6000.0f, det);  // 10 * 20 * 30

    ks_mat4_invi(&scl);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f / 10.0f, ks_mat4_get(&scl, 0, 0));
}

/* ========================================================================= */
/* EULER ANGLES                                                              */
/* ========================================================================= */

void test_euler_basics(void) {
    ks_euler e1;
    ks_euler_zeroinit(&e1, KS_EULER_YXZ);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, e1.x);

    ks_euler e2 = ks_euler_zeronew(KS_EULER_XYZ);
    TEST_ASSERT_EQUAL_INT(KS_EULER_XYZ, e2.order);

    ks_euler e3;
    ks_euler_init(&e3, 1.0f, 2.0f, 3.0f, KS_EULER_ZXY);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, e3.yaw);

    ks_euler e4 = ks_euler_new(0.0f, KS_PI_2, 0.0f, KS_EULER_YXZ);

    ks_mat4 m;
    ks_euler_to_mat4(&m, &e4);                                        // 90 degree yaw
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat4_get(&m, 2, 0));  // sin(90) in col 2, row 0

    ks_quat q = ks_euler_to_quat(&e4);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, cosf(KS_PI_4), q.w);
}

/* ========================================================================= */
/* QUATERNIONS                                                               */
/* ========================================================================= */

void test_quat_initialization(void) {
    ks_quat q1;
    ks_quat_zeroinit(&q1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, q1.w);

    ks_quat q2 = ks_quat_zeronew();
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, q2.x);

    ks_quat_idinit(&q1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, q1.w);

    ks_quat q3 = ks_quat_idnew();
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, q3.w);

    ks_quat q4 = KS_QUAT(1.0f, 2.0f, 3.0f, 4.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, q4.z);

    ks_quat q5 = KS_QUAT(5.0f, 6.0f, 7.0f, 8.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 8.0f, q5.z);
}

void test_quat_arithmetic(void) {
    ks_quat q1 = KS_QUAT(1.0f, 2.0f, 3.0f, 4.0f);
    ks_quat q2 = KS_QUAT(2.0f, 3.0f, 4.0f, 5.0f);
    ks_quat out;

    ks_quat_add(&out, &q1, &q2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 3.0f, out.w);

    ks_quat_addi(&q1, &q2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 3.0f, q1.w);

    ks_quat_sub(&out, &q1, &q2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, out.w);

    ks_quat_subi(&q1, &q2);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, q1.w);

    ks_quat_smul(&out, &q2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, out.w);

    ks_quat_smuli(&q2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 4.0f, q2.w);

    ks_quat_sdiv(&out, &q2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, out.w);

    ks_quat_sdivi(&q2, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, q2.w);
}

void test_quat_operations(void) {
    ks_quat id = ks_quat_idnew();
    ks_vec3 axis = KS_VEC3(0.0f, 1.0f, 0.0f);
    ks_quat qy = ks_quat_from_axis_angle(&axis, KS_PI_2);

    ks_quat out;
    ks_quat_mul(&out, &id, &qy);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, qy.y, out.y);

    ks_quat_muli(&id, &qy);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, qy.y, id.y);

    ks_vec3 v = KS_VEC3(1.0f, 0.0f, 0.0f);
    ks_vec3 v_out;
    ks_quat_mulv(&v_out, &qy, &v);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, v_out.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, v_out.y);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, v_out.z);  // 90 deg Y rotation

    ks_quat_mulvi(&qy, &v);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, v.z);

    ks_quat conj;
    ks_quat_conj(&conj, &qy);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -qy.y, conj.y);

    ks_quat_conji(&qy);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, conj.y, qy.y);

    ks_quat q3 = KS_QUAT(1.0f, 2.0f, 3.0f, 4.0f);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 30.0f, ks_quat_dot(&q3, &q3));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 30.0f, ks_quat_length_sq(&q3));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, sqrtf(30.0f), ks_quat_length(&q3));

    ks_quat_norm(&out, &q3);
    TEST_ASSERT_TRUE(ks_quat_isunitary(&out));

    ks_quat_normi(&q3);
    TEST_ASSERT_TRUE(ks_quat_isunitary(&q3));

    ks_quat_inv(&out, &q3);
    ks_quat ident;
    ks_quat_mul(&ident, &q3, &out);

    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ident.w);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, ident.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, ident.y);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, ident.z);

    ks_quat_invi(&q3);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, out.y, q3.y);

    ks_quat_idinit(&id);

    ks_mat4 m;
    ks_quat_to_mat4(&m, &id);

    // Check matrix output directly with tolerance
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat4_get(&m, 0, 0));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat4_get(&m, 1, 1));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat4_get(&m, 2, 2));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.0f, ks_mat4_get(&m, 3, 3));
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, ks_mat4_get(&m, 1, 0));

    ks_euler e = ks_euler_new(0.0f, KS_PI_2, 0.0f, KS_EULER_XYZ);
    out = ks_quat_from_euler(&e);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, cosf(KS_PI_4), out.w);
}

void test_euler_quat_matrix_equivalence(void) {
    // We test a complex multi-axis rotation
    ks_euler e = ks_euler_new(KS_PI_4, KS_PI_2, KS_PI_4, KS_EULER_YXZ);

    // 1. Direct Euler to Matrix
    ks_mat4 m_euler;
    ks_euler_to_mat4(&m_euler, &e);

    // 2. Euler to Quat, then Quat to Matrix
    ks_quat q = ks_euler_to_quat(&e);

    ks_mat4 m_quat;
    ks_quat_to_mat4(&m_quat, &q);

    // If Euler->Quat uses the correct order, both matrices will be identical
    TEST_ASSERT_TRUE_MESSAGE(ks_mat4_equals(&m_euler, &m_quat), "Euler->Quat conversion ignores order!");
}

void test_quat_axis_angle(void) {
    ks_vec3 axis = KS_VEC3(0.0f, 1.0f, 0.0f);  // Y-axis
    ks_quat q = ks_quat_from_axis_angle(&axis, KS_PI_2);

    ks_vec3 v = KS_VEC3(1.0f, 0.0f, 0.0f);
    ks_vec3 v_out;
    ks_quat_mulv(&v_out, &q, &v);

    // Rotating (1,0,0) by 90deg on Y results in (0,0,-1)
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, v_out.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, -1.0f, v_out.z);
}

/* ========================================================================= */
/* CALCULUS & NUMERICAL INTEGRATION                                          */
/* ========================================================================= */

double cb_deriv_1d(double x, void* args) {
    (void)args;
    return x * x * x;
}  // x^3
double cb_deriv_2d(double x, double y, void* args) {
    (void)args;
    return x * x + y * y * y;
}
double cb_integ_nd(const double* vars, int32_t dims, void* args) {
    (void)dims;
    (void)args;
    return vars[0] * vars[0];
}

void test_calculus_derivatives(void) {
    // d/dx (x^3) = 3x^2. At x=2 -> 12
    double d1 = ks_deriv_1d(cb_deriv_1d, 2.0, 1e-4, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 12.0, d1);

    // Partial x: 2x. At x=3 -> 6
    double dx = ks_deriv_2d_x(cb_deriv_2d, 3.0, 2.0, 1e-4, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 6.0, dx);

    // Partial y: 3y^2. At y=2 -> 12
    double dy = ks_deriv_2d_y(cb_deriv_2d, 3.0, 2.0, 1e-4, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 12.0, dy);
}

void test_calculus_integrals(void) {
    // Integral x^3 from 0 to 2 -> (2^4)/4 = 4
    double i1 = ks_integ_1d(cb_deriv_1d, 0.0, 2.0, 100, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 4.0, i1);

    // Integral of x^2 + y^3. x:[0,1] y:[0,1].
    // int_0^1 (x^2 + y^3) dx = 1/3 + y^3.
    // int_0^1 (1/3 + y^3) dy = 1/3 + 1/4 = 7/12 ~ 0.583333
    double i2 = ks_integ_2d(cb_deriv_2d, 0.0, 1.0, 100, 0.0, 1.0, 100, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 7.0 / 12.0, i2);

    // Monte Carlo integration. Tolerance must be wide due to random sampling variance.
    double bounds_min[1] = {0.0};
    double bounds_max[1] = {1.0};
    double ind = ks_integ_nd(cb_integ_nd, 1, bounds_min, bounds_max, 100000, NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.05, 1.0 / 3.0, ind);
}

/* ========================================================================= */
/* ODE SOLVERS                                                               */
/* ========================================================================= */

ks_vec2 cb_ode_2d(double t, ks_vec2 state, void* args) {
    (void)t;
    (void)args;
    return KS_VEC2(state.y, -state.x);
}
ks_vec3 cb_ode_3d(double t, ks_vec3 state, void* args) {
    (void)t;
    (void)args;
    return KS_VEC3(state.y, -state.x, 0.0f);
}

void test_ode_solvers(void) {
    double val = 0.0;
    ks_integ_euler(&val, 5.0, 0.1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 0.5, val);

    ks_vec2 p2 = KS_VEC2(0.0f, 0.0f);
    ks_vec2 v2 = KS_VEC2(1.0f, 1.0f);
    ks_vec2 a2 = KS_VEC2(2.0f, 2.0f);
    ks_integ_semi_euler_2d(&p2, &v2, a2, 0.1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.2f, v2.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.12f, p2.x);

    ks_vec3 p3 = KS_VEC3(0.0f, 0.0f, 0.0f);
    ks_vec3 v3 = KS_VEC3(1.0f, 1.0f, 1.0f);
    ks_vec3 a3 = KS_VEC3(2.0f, 2.0f, 2.0f);
    ks_integ_semi_euler_3d(&p3, &v3, a3, 0.1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 1.2f, v3.z);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.12f, p3.z);

    ks_vec2 state2 = KS_VEC2(1.0f, 0.0f);
    ks_vec2 out2 = ks_integ_rk4_2d(cb_ode_2d, 0.0, state2, 0.1, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.995f, out2.x);  // Approx cos(0.1)

    ks_vec3 state3 = KS_VEC3(1.0f, 0.0f, 0.0f);
    ks_vec3 out3 = ks_integ_rk4_3d(cb_ode_3d, 0.0, state3, 0.1, NULL);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.995f, out3.x);
}

/* ========================================================================= */
/* FIELDS                                                                    */
/* ========================================================================= */

void cb_field_sample(ks_field_ctx* ctx, void* out) {
    double* val = (double*)out;
    *val = ctx->x * 2.0;  // simple x gradient
}

void test_fields(void) {
    ks_field f = ks_field_create(4, 4, 1, sizeof(double), 1.0);
    TEST_ASSERT_NOT_NULL(f.data);

    ks_field_sample(&f, NULL, 0.0, cb_field_sample);

    double* d = (double*)f.data;
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 6.0, d[_ks_f3_idx_internal(3, 0, 0, 4, 4)]);

    ks_vec2 g = ks_sf2_grad(&f, 1, 1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 2.0f, g.x);  // df/dx = 2.0
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, g.y);  // df/dy = 0.0

    double lap = ks_sf2_lap(&f, 1, 1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 0.0, lap);  // linear gradient has 0 laplacian

    ks_field_destroy(&f);
    TEST_ASSERT_NULL(f.data);

    ks_field fvec = ks_field_create(4, 4, 4, sizeof(ks_vec3), 1.0);
    ks_vec3* vd = (ks_vec3*)fvec.data;

    // Create a simple divergence field v = (x, y, z)
    for (int z = 0; z < 4; ++z) {
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                vd[_ks_f3_idx_internal(x, y, z, 4, 4)] = KS_VEC3((float)x, (float)y, (float)z);
            }
        }
    }

    double div = ks_vf3_div(&fvec, 1, 1, 1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 3.0, div);  // dx/dx + dy/dy + dz/dz = 3.0

    ks_vec3 curl = ks_vf3_curl(&fvec, 1, 1, 1);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, curl.x);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, curl.y);
    TEST_ASSERT_FLOAT_WITHIN(TEST_EPS, 0.0f, curl.z);

    ks_field_destroy(&fvec);
}

/* ========================================================================= */
/* FFT                                                                       */
/* ========================================================================= */

void test_fft(void) {
    size_t n = 4;
    double complex samples[4] = {1.0 + 0.0 * I, 1.0 + 0.0 * I, 1.0 + 0.0 * I, 1.0 + 0.0 * I};

    int ret = fft(n, samples);
    TEST_ASSERT_EQUAL_INT(0, ret);

    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 4.0, creal(samples[0]));
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 0.0, cimag(samples[0]));

    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 0.0, creal(samples[1]));
    TEST_ASSERT_FLOAT_WITHIN(TEST_DEPS, 0.0, cimag(samples[1]));

    // Fail on non-power of two
    ret = fft(3, samples);
    TEST_ASSERT_EQUAL_INT(1, ret);
}

/* ========================================================================= */
/* MAIN RUNNER                                                               */
/* ========================================================================= */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_util_log2ld);
    RUN_TEST(test_util_bit_reverse);

    RUN_TEST(test_vec4_initialization);
    RUN_TEST(test_vec4_arithmetic);
    RUN_TEST(test_vec4_geometry);
    RUN_TEST(test_vec4_access_and_comparison);
    RUN_TEST(test_vec3_cross_product);

    RUN_TEST(test_mat_transposition);
    RUN_TEST(test_mat4_initialization_and_access);
    RUN_TEST(test_mat4_arithmetic_and_shapes);
    RUN_TEST(test_mat4_multiplication_and_transforms);
    RUN_TEST(test_mat4_projection);

    RUN_TEST(test_mat2_explicit);
    RUN_TEST(test_mat3_explicit);
    RUN_TEST(test_mat4_explicit);

    RUN_TEST(test_euler_basics);

    RUN_TEST(test_quat_initialization);
    RUN_TEST(test_quat_arithmetic);
    RUN_TEST(test_quat_operations);
    RUN_TEST(test_euler_quat_matrix_equivalence);
    RUN_TEST(test_quat_axis_angle);

    RUN_TEST(test_calculus_derivatives);
    RUN_TEST(test_calculus_integrals);

    RUN_TEST(test_ode_solvers);

    RUN_TEST(test_fields);
    RUN_TEST(test_fft);

    return UNITY_END();
}