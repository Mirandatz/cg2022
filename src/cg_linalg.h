#pragma once

#include <math.h>

const float PI = 3.14159265358979323846;

typedef union CG_Vector3 {
    struct {
        float v1;
        float v2;
        float v3;
    };
    float as_array[3];
} CG_Vector3;

typedef union CG_Matrix3 {
    struct {
        float m11, m12, m13;
        float m21, m22, m23;
        float m31, m32, m33;
    };
    float as_array[3 * 3];
} CG_Matrix3;

void cg_print_v3(CG_Vector3 vec) {
    printf("v1=%f v2=%f v3=%f\n", vec.v1, vec.v2, vec.v3);
}

void cg_print_m3(CG_Matrix3 mat) {
    printf("m11=%f m12=%f m13=%f\n", mat.m11, mat.m12, mat.m13);
    printf("m21=%f m22=%f m23=%f\n", mat.m21, mat.m22, mat.m23);
    printf("m31=%f m32=%f m33=%f\n", mat.m31, mat.m32, mat.m33);
}

float cg_m3_get(CG_Matrix3 mat, int row, int column) {
    assert(0 <= row && row < 3);
    assert(0 <= column && column < 3);

    return mat.as_array[(row * 3) + column];
}

CG_Vector3 cg_mul_m3v3(CG_Matrix3 mat, CG_Vector3 vec) {
    CG_Vector3 res = {0};

    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            res.as_array[row] += cg_m3_get(mat, row, column) * vec.as_array[column];
        }
    }

    return res;
}

CG_Matrix3 cg_mul_m3m3(CG_Matrix3 left, CG_Matrix3 right) {
    CG_Matrix3 res;

    // todo: rewrite com for

    res.m11 = left.m11 * right.m11 + left.m12 * right.m21 + left.m13 * right.m31;
    res.m12 = left.m11 * right.m12 + left.m12 * right.m22 + left.m13 * right.m32;
    res.m13 = left.m11 * right.m13 + left.m12 * right.m23 + left.m13 * right.m33;

    res.m21 = left.m21 * right.m11 + left.m22 * right.m21 + left.m23 * right.m31;
    res.m22 = left.m21 * right.m12 + left.m22 * right.m22 + left.m23 * right.m32;
    res.m23 = left.m21 * right.m13 + left.m22 * right.m23 + left.m23 * right.m33;

    res.m31 = left.m31 * right.m11 + left.m32 * right.m21 + left.m33 * right.m31;
    res.m32 = left.m31 * right.m12 + left.m32 * right.m22 + left.m33 * right.m32;
    res.m33 = left.m31 * right.m13 + left.m32 * right.m23 + left.m33 * right.m33;

    return res;
}

CG_Matrix3 cg_make_m3_identity() {
    CG_Matrix3 identity = {0};
    identity.m11 = 1;
    identity.m22 = 1;
    identity.m33 = 1;
    return identity;
}

CG_Matrix3 cg_make_2d_translation(float delta_x, float delta_y) {
    CG_Matrix3 trans = cg_make_m3_identity();
    trans.m13 = delta_x;
    trans.m23 = delta_y;
    return trans;
}

CG_Matrix3 cg_make_ccw_rotation(float degrees) {
    float radians = degrees * PI / 180;

    CG_Matrix3 rotation = {
        .m11 = cos(radians),
        .m12 = -1 * sin(radians),
        .m13 = 0.0,

        .m21 = sin(radians),
        .m22 = cos(radians),
        .m23 = 0.0,

        .m31 = 0.0,
        .m32 = 0.0,
        .m33 = 1.0,
    };

    return rotation;
}

CG_Matrix3 cg_make_2d_scaling(float x_factor, float y_factor) {
    CG_Matrix3 scaling = cg_make_m3_identity();
    scaling.m11 = x_factor;
    scaling.m22 = y_factor;
    return scaling;
}

CG_Matrix3 cg_making_2d_shearing(float x_factor, float y_factor) {
    CG_Matrix3 shearing = cg_make_m3_identity();
    shearing.m12 = x_factor;
    shearing.m21 = y_factor;
    return shearing;
}
