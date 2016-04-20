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
enum JROTATION_ORDER
{
    XYZ, XZY, YXZ, YZX, ZXY, ZYX
};
class jrotation
{
public:
	simd::float4 xyzw;

	jrotation()
	{
        xyzw = {0,0,0,1};
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
    
    inline void euler_degree(float _x, float _y, float _z, JROTATION_ORDER order)
    {
        const float convert = 3.141592 / 180;
        euler(_x * convert, _y * convert, _z * convert, order);
    }

    inline void genMatX(float rad, matrix_float3x3 &mat)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        mat.columns[0] = {c,s,0};
        mat.columns[1] = {-s,c,0};
        mat.columns[2] = {0,0,1};
    }
    
    inline void genMatY(float rad, matrix_float3x3 &mat)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        mat.columns[0] = {c,0,-s};
        mat.columns[1] = {0,1,0};
        mat.columns[2] = {s,0,c};
    }
    
    inline void genMatZ(float rad, matrix_float3x3 &mat)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        mat.columns[0] = {1,0,0};
        mat.columns[1] = {0,c,s};
        mat.columns[2] = {0,-s,c};
    }
    
    inline void onlyx(float rad)
    {
        //TODO : faster
        float h = rad/2;
        float cosh = cosf(h);
        float sinh = sinf(h);
        
        xyzw =
        {
            sinh,
            0,
            0,
            cosh
        };
    }
    
    inline void onlyy(float rad)
    {
        //TODO : faster
        float h = rad/2;
        float cosh = cosf(h);
        float sinh = sinf(h);
        
        xyzw =
        {
            0,
            sinh,
            0,
            cosh
        };
    }
    
    inline void onlyz(float rad)
    {
        //TODO : faster
        float h = rad/2;
        float cosh = cosf(h);
        float sinh = sinf(h);
        
        xyzw =
        {
            0,
            0,
            sinh,
            cosh
        };
    }
    
	inline void euler(float _x, float _y, float _z, JROTATION_ORDER order)
	{
        jrotation rx, ry, rz;
        rx.onlyx(_x);
        ry.onlyy(_y);
        rz.onlyz(_z);
        
        switch(order)
        {
            case JROTATION_ORDER::XYZ:
                *this = (rz * (ry * rx));
                break;
            case JROTATION_ORDER::XZY:
                *this = (ry * (rz * rx));
                break;
            case JROTATION_ORDER::YXZ:
                *this = (rz * (rx * ry));
                break;
            case JROTATION_ORDER::YZX:
                *this = (rx * (rz * ry));
                break;
            case JROTATION_ORDER::ZXY:
                *this = (ry * (rx * rz));
                break;
            case JROTATION_ORDER::ZYX:
                *this = (rx * (ry * rz));
                break;
            default:
                puts("unexplected rotaiton order.");
                break;
        }
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
    
    inline simd::float3 toEuler() const
    {
        return (simd::float3)
        {
            static_cast<float>(atan2(  2*(xyzw[3]*xyzw[0] + xyzw[1]*xyzw[2]),  1-2*(xyzw[0]*xyzw[0] + xyzw[1]*xyzw[1]) )),
            static_cast<float>(asin(   2*(xyzw[3]*xyzw[1] - xyzw[2]*xyzw[0])                   )),
            static_cast<float>(atan2(  2*(xyzw[3]*xyzw[2] + xyzw[0]*xyzw[1]),  1-2*(xyzw[1]*xyzw[1] + xyzw[2]*xyzw[2]) ))
        };
    }
};

#endif /* jrotation_hpp */
