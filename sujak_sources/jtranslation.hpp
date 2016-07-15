//
//  jtranslation.hpp
//  MetalVertexStreaming
//
//  Created by Build on 12/9/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jtranslation_hpp
#define jtranslation_hpp

#include <stdio.h>
#include <simd/simd.h>
class jtranslation
{
public:
	simd::float4 pos;

    jtranslation();
    jtranslation(float x, float y, float z);
    jtranslation(simd::float4 _pos);
    
    inline void sety(float v)
    {
        pos[1] = v;
    }
    
    inline void setx(float v)
    {
        pos[0] = v;
    }

	inline void setPos(simd::float4 v)
	{
		pos = v;
	}
	inline void setPos(float x, float y, float z)
	{
		pos = {x, y, z, 1};
	}
	inline simd::float4 getPos() const
	{
		return pos;
	}
	inline matrix_float4x4 getMat() const
	{
		matrix_float4x4 mat;
		
		mat.columns[0] = {1, 0, 0, 0};
		mat.columns[1] = {0, 1, 0, 0};
		mat.columns[2] = {0, 0, 1, 0};
		mat.columns[3] = {pos[0], pos[1], pos[2], 1};
		
		return mat;
	}
};
#endif /* jtranslation_hpp */
