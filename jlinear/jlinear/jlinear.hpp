/*
 *  jlinear.hpp
 *  jlinear
 *
 *  Created by Ingun Jon on 7/17/16.
 *  Copyright © 2016 Ingun Jon. All rights reserved.
 *
 */

#ifndef jlinear_
#define jlinear_

/* The classes below are exported */
#pragma GCC visibility push(default)
#include <simd/simd.h>

namespace jlinear
{
    void getReducedEchelonMatrix34(float m[3][4], simd::float4 v1, simd::float4 v2, simd::float4 v3);
    matrix_double4x4 makePlaneDistanceK(simd::double3 p1, simd::double3 p2, simd::double3 p3);
	matrix_float4x4 GetTranslationMatrix( const float dx, const float dy, const float dz );
	matrix_float4x4 GetViewMatrix( const simd::float3& eye, const simd::float3& up, const simd::float3& at );
	matrix_float4x4 GetProjectionMatrixOrthogonal( const float r, const float t, const float n, const float f );
	matrix_float4x4 GetProjectionMatrixPerspective( const float r, const float t, const float n, const float f );
	matrix_float4x4 GetRotateMatrix(float x, float y, float z, float radian);
}

#pragma GCC visibility pop
#endif
