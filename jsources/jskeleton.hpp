//
//  jskeleton.hpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 3..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jskeleton_hpp
#define jskeleton_hpp

#include <simd/simd.h>
#include <stdio.h>
#include "jjoint.h"

class jskeleton
{
	jjoint *joints;
	int* table;
	matrix_float4x4 *inverseTable;
	int jointnum;
public:
	inline int getJointCnt() const {return jointnum;}
	inline jjoint* getjointsArr() const { return joints; }
	inline int* getTableArr() const {return table;}
	matrix_float4x4 transOfJointAt(int i);
	matrix_float4x4 inverseOfJointAt(int i);
	void setFromFile(char* tableBytes, char* jointBytes);
	jskeleton();
};
#endif /* jskeleton_hpp */
