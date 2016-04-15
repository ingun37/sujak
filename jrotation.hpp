//
//  jrotation.hpp
//  MetalVertexStreaming
//
//  Created by Build on 11/26/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jrotation_hpp
#define jrotation_hpp

#include <stdio.h>
#include <simd/simd.h>
#include <math.h>
class jrotation
{
public:
	simd::float4 xyzw;

	jrotation()
	{
	}
	jrotation(simd::float4 _xyzw)
	{
		xyzw = _xyzw;
	}
	jrotation(simd::float4 axis, float radian)
	{
		axis = (simd::float4){1,1,1,0} * axis;
		xyzw = simd::normalize(axis) * sinf(radian/2);
		xyzw = (simd::float4){0,0,0,cosf(radian/2)} + xyzw;
	}
	inline jrotation operator*(const jrotation& r )
	{
		//TODO : faster
		return jrotation( (simd::float4)
		{
			xyzw[3]*r.xyzw[0] + xyzw[0]*r.xyzw[3] + xyzw[1]*r.xyzw[2] - xyzw[2]*r.xyzw[1],
			xyzw[3]*r.xyzw[1] - xyzw[0]*r.xyzw[2] + xyzw[1]*r.xyzw[3] + xyzw[2]*r.xyzw[0],
			xyzw[3]*r.xyzw[2] + xyzw[0]*r.xyzw[1] - xyzw[1]*r.xyzw[0] + xyzw[2]*r.xyzw[3],
			xyzw[3]*r.xyzw[3] - xyzw[0]*r.xyzw[0] - xyzw[1]*r.xyzw[1] - xyzw[2]*r.xyzw[2],
		});
	}
    
    inline void euler_degree(float _x, float _y, float _z)
    {
        const float convert = 3.141592 / 180;
        euler(_x * convert, _y * convert, _z * convert);
    }
    
	inline void euler(float _x, float _y, float _z)
	{
		//TODO : faster
		float hx = _x/2;
		float hy = _y/2;
		float hz = _z/2;
		float coshx = cosf(hx);
		float coshy = cosf(hy);
		float coshz = cosf(hz);
		float sinhx = sinf(hx);
		float sinhy = sinf(hy);
		float sinhz = sinf(hz);

		xyzw =
		{
			sinhx*coshy*coshz - coshx*sinhy*sinhz,
			coshx*sinhy*coshz + sinhx*coshy*sinhz,
			coshx*coshy*sinhz - sinhx*sinhy*coshz,
			coshx*coshy*coshz + sinhx*sinhy*sinhz
		};
	}
#define DBL(x) ((x)*(x))
#define EQ1(a,b) (1-2*DBL(xyzw[a])-2*DBL(xyzw[b]))
#define EQM(a,b,c,d) (2*(xyzw[a]*xyzw[b] - xyzw[c]*xyzw[d]))
#define EQP(a,b,c,d) (2*(xyzw[a]*xyzw[b] + xyzw[c]*xyzw[d]))
	inline matrix_float4x4 toMat() const
	{
		matrix_float4x4 mat;

		mat.columns[0] = {EQ1(1, 2), EQP(0, 1, 2, 3), EQM(0, 2, 1, 3), 0};
		mat.columns[1] = {EQM(0, 1, 2, 3), EQ1(0, 2), EQP(1, 2, 0, 3), 0};
		mat.columns[2] = {EQP(0, 2, 1, 3), EQM(1, 2, 0, 3), EQ1(0, 1), 0};
		mat.columns[3] = {0, 0, 0, 1};

		return mat;
	}
};

#endif /* jrotation_hpp */
