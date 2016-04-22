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
    JROTATION_ORDER order;
	inline matrix_float4x4 getTransMat()
	{
		return matrix_multiply(pos.getMat(), rot.toMat(order));
	}
	inline matrix_float4x4 getOnlyRotation()
	{
		return rot.toMat(order);
	}
};

#endif /* JJoint_h */
