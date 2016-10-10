/*
 *  jlinear.cpp
 *  jlinear
 *
 *  Created by Ingun Jon on 7/17/16.
 *  Copyright © 2016 Ingun Jon. All rights reserved.
 *
 */

#include <iomanip>
#include "jlinear.hpp"
#include "jlinearsystem.h"

using namespace std;
using namespace simd;
using namespace jlinear;


void jlinear::getReducedEchelonMatrix34(float m[3][4], float4 v1, float4 v2, float4 v3)
{
    jlinearsystem<3, 4> system;
    
    system.setrow(0, v1[0], v1[1], v1[2], v1[3]);
    system.setrow(1, v2[0], v2[1], v2[2], v2[3]);
    system.setrow(2, v3[0], v3[1], v3[2], v3[3]);
    
    system.reducedEchelonize();
    for(int i=0;i<3;i++)
        for(int j=0;j<4;j++)
            m[i][j] = system.getAt(i, j);
}

matrix_double4x4 jlinear::makePlaneDistanceK(double3 p1, double3 p2, double3 p3)
{
    jlinearsystem<3, 5> system;
    
    system.setrow(0, p1[0], p1[1], p1[2], 1, 0);
    system.setrow(1, p2[0], p2[1], p2[2], 1, 0);
    system.setrow(2, p3[0], p3[1], p3[2], 1, 0);
    
    system.reducedEchelonize();
    //ax+by+cz+d = 0
    double fvcs[4];
    
    
    for(int i=0;i<4;i++)
    {
        if(system.isElementFreeVariable(i))
        {
            fvcs[i] = 1;
            continue;
        }
        
        if(system.getNumberFreeVariableDependencyOfElement(i)>1)
            throw "impossible freevarialbe dependency number";
        
        fvcs[i] = -system.getSumOfFreeVariableCoefficientOfElement(i);
    }
    
    double3 fvcs3{fvcs[0], fvcs[1], fvcs[2]};
    double mag = dot(fvcs3, fvcs3);
    
    double sqrfv;
    
    if(abs(mag) < 0.0001)
        throw "magnitude a,b,c cant be 0";
    
    sqrfv = ((double)1)/mag;
    
    double4 fvcs4{fvcs[0], fvcs[1], fvcs[2], fvcs[3]};
    double4 zero4{0,0,0,0};
    matrix_double4x4 m1 = matrix_from_columns(fvcs4, zero4, zero4, zero4);
    matrix_double4x4 m2 = matrix_from_rows(fvcs4, zero4, zero4, zero4);
    
    matrix_double4x4 K = (m1 * m2) * sqrfv;
    return K;
}


void printMatrix(const matrix_float4x4& m)
{
	for(int i=0;i<4;i++)
	{
		printf("%.2f %.2f %.2f %.2f\n", m.columns[i][0], m.columns[i][1], m.columns[i][2], m.columns[i][3]);
	}
}

matrix_float4x4 jlinear::GetRotateMatrix(float x, float y, float z, float radian)
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

matrix_float4x4 jlinear::GetTranslationMatrix( const float dx, const float dy, const float dz )
{
	matrix_float4x4 rmat = matrix_identity_float4x4;
	
	rmat.columns[3]= {dx, dy, dz, 1};
	
	return rmat;
}

matrix_float4x4 jlinear::GetViewMatrix( const simd::float3& eye, const simd::float3& up, const simd::float3& at )
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
	
	matrix_float4x4 movMat = jlinear::GetTranslationMatrix( -eye[0], -eye[1], -eye[2] );
	//return matrix_identity_float4x4;
	return matrix_multiply(dotMat, movMat);
}

matrix_float4x4 jlinear::GetProjectionMatrixPerspective( const float r, const float t, const float n, const float f )
{
	matrix_float4x4 rmat;
	
	rmat.columns[0] = {n/r, 0, 0, 0};
	rmat.columns[1] = {0, n/t, 0, 0};
	rmat.columns[2] = {0, 0, f/(n-f), -1};
	rmat.columns[3] = {0, 0, -(n*f)/(f-n), 0};
	
	return rmat;
}

matrix_float4x4 jlinear::GetProjectionMatrixOrthogonal(const float r, const float t, const float n, const float f)
{
	matrix_float4x4 rmat;
	rmat.columns[0] = {1/r, 0, 0, 0};
	rmat.columns[1] = {0, 1/t, 0, 0};
	rmat.columns[2] = {0, 0, (-1)/(f-n), 0};
	rmat.columns[3] = {0, 0, (-n)/(f-n), 1};
	
	return rmat;
}
