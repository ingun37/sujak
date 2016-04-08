//
//  JJoint.h
//  MetalVertexStreaming
//
//  Created by Build on 11/26/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef JJoint_h
#define JJoint_h
#include <simd/simd.h>
#include "jrotation.hpp"
#include "jtranslation.hpp"
class jjoint
{
public:
	jtranslation pos;
	jrotation rot;
	
	inline matrix_float4x4 getTransMat() const
	{
		return matrix_multiply(pos.getMat(), rot.toMat());
	}
	inline matrix_float4x4 getOnlyRotation() const
	{
		return rot.toMat();
	}
};

#endif /* JJoint_h */
