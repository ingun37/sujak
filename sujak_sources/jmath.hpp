
#ifndef jmath_hpp
#define jmath_hpp

#include <stdio.h>

#include <simd/simd.h>

namespace jmath
{
	matrix_float4x4 GetTranslationMatrix( const float dx, const float dy, const float dz );
	matrix_float4x4 GetViewMatrix( const simd::float3& eye, const simd::float3& up, const simd::float3& at );
	matrix_float4x4 GetProjectionMatrixOrthogonal( const float r, const float t, const float n, const float f );
	matrix_float4x4 GetProjectionMatrixPerspective( const float r, const float t, const float n, const float f );
	matrix_float4x4 GetRotateMatrix(float x, float y, float z, float radian);
}
#endif /* jmath_hpp */
