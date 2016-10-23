#include "jmath.hpp"



void printMatrix(const matrix_float4x4& m)
{
	for(int i=0;i<4;i++)
	{
		printf("%.2f %.2f %.2f %.2f\n", m.columns[i][0], m.columns[i][1], m.columns[i][2], m.columns[i][3]);
	}
}

matrix_float4x4 sujak::jmath::GetRotateMatrix(float x, float y, float z, float radian)
{
	float c = cosf(radian);
	float s = sinf(radian);
	
	matrix_float4x4 rmat;
	
	rmat.columns[0] = {c+(1-c)*x*x,		(1-c)*x*y + s*z,	(1-c)*x*z - s*y,0};
	rmat.columns[1] = {(1-c)*x*y - s*z,	c+(1-c)*y*y,		(1-c)*y*z + s*x,0};
	rmat.columns[2] = {(1-c)*x*z + s*y,	(1-c)*y*z - s*x,	c+(1-c)*z*z,	0};
	rmat.columns[3] = {0,				0,					0,				1};
	return rmat;
}

matrix_float4x4 sujak::jmath::GetTranslationMatrix( const float dx, const float dy, const float dz )
{
	matrix_float4x4 rmat = matrix_identity_float4x4;
	
	rmat.columns[3]= {dx, dy, dz, 1};
	
	return rmat;
}

matrix_float4x4 sujak::jmath::GetViewMatrix( const simd::float3& eye, const simd::float3& up, const simd::float3& at )
{
	simd::float3 f, r, u;
	f = simd::normalize(at-eye);
	r = simd::normalize( simd::cross(f, up) );
	u = simd::cross(r, f);
	
	//TODO exception for like crossing same vectors kinda things
	
	matrix_float4x4 dotMat;
	dotMat.columns[0] = {r[0], u[0], -f[0], 0};
	dotMat.columns[1] = {r[1], u[1], -f[1], 0};
	dotMat.columns[2] = {r[2], u[2], -f[2], 0};
	dotMat.columns[3] = { 0, 0, 0, 1};

	matrix_float4x4 movMat = jmath::GetTranslationMatrix( -eye[0], -eye[1], -eye[2] );
	//return matrix_identity_float4x4;
	return matrix_multiply(dotMat, movMat);
}

matrix_float4x4 sujak::jmath::GetProjectionMatrixPerspective( const float r, const float t, const float n, const float f )
{
	matrix_float4x4 rmat;
	
	rmat.columns[0] = {n/r, 0, 0, 0};
	rmat.columns[1] = {0, n/t, 0, 0};
	rmat.columns[2] = {0, 0, f/(n-f), -1};
	rmat.columns[3] = {0, 0, -(n*f)/(f-n), 0};
	
	return rmat;
}

matrix_float4x4 sujak::jmath::GetProjectionMatrixOrthogonal(const float r, const float t, const float n, const float f)
{
	matrix_float4x4 rmat;
	rmat.columns[0] = {1/r, 0, 0, 0};
	rmat.columns[1] = {0, 1/t, 0, 0};
	rmat.columns[2] = {0, 0, (-1)/(f-n), 0};
	rmat.columns[3] = {0, 0, (-n)/(f-n), 1};
	
	return rmat;
}
