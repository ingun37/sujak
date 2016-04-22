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
#include <stdlib.h>
#include <simd/simd.h>
#include <math.h>
enum JROTATION_ORDER
{
    XYZ, XZY, YXZ, YZX, ZXY, ZYX
};
class jrotation
{
public:
	simd::float3 xyz;

	jrotation()
	{
        xyz = {0,0,0};
	}
	jrotation(simd::float3 _xyz)
	{
		xyz = _xyz;
	}
    
    inline void degree(float _x, float _y, float _z)
    {
        const float convert = 3.141592 / 180;
        xyz = {_x*convert, _y*convert, _z*convert};
    }

    inline void genMatX(float rad, matrix_float4x4 &mat)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        mat.columns[0] = {1,0,0,0};
        mat.columns[1] = {0,c,s,0};
        mat.columns[2] = {0,-s,c,0};
        mat.columns[3] = {0,0,0,1};
        
        
    }
    
    inline void genMatY(float rad, matrix_float4x4 &mat)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        mat.columns[0] = {c,0,-s, 0};
        mat.columns[1] = {0,1,0, 0};
        mat.columns[2] = {s,0,c,0};
        mat.columns[3] = {0,0,0,1};
    }
    
    inline void genMatZ(float rad, matrix_float4x4 &mat)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        mat.columns[0] = {c,s,0,0};
        mat.columns[1] = {-s,c,0,0};
        mat.columns[2] = {0,0,1,0};
        mat.columns[3] = {0,0,0,1};
    }
    
    inline void onlyx(float rad)
    {
        xyz = {rad,0,0};
    }
    
    inline void onlyy(float rad)
    {
        xyz = { 0,    rad,   0   };
    }
    
    inline void onlyz(float rad)
    {
        xyz ={0,0,rad};
    }
    /*
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
     */
#define jmulrotmat(A,B,C) matrix_multiply(A, matrix_multiply(B, C))
    inline matrix_float4x4 toMat(JROTATION_ORDER order)
    {
        matrix_float4x4 X, Y, Z;
        genMatX(xyz[0], X);
        genMatY(xyz[1], Y);
        genMatZ(xyz[2], Z);
        
        switch(order)
        {
            case XYZ: return jmulrotmat(Z,Y,X); break;
            case XZY: return jmulrotmat(Y,Z,X); break;
            case YXZ: return jmulrotmat(Z,X,Y); break;
            case YZX: return jmulrotmat(X,Z,Y); break;
            case ZXY: return jmulrotmat(Y,X,Z); break;
            case ZYX: return jmulrotmat(X,Y,Z); break;
            default:
                puts("wtfsdfadsfs");
                exit(1);
                return matrix_identity_float4x4;
                break;
        }
    }
#undef jmulrotmat
    /*
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
	}*/
};

#endif /* jrotation_hpp */
