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
typedef struct _jcurvenode jcurvenode;
class jskeleton
{
	jjoint *joints;
    
	int* table;
	
	int jointnum;
    jcurvenode *curvenodes;
public:
	inline int getJointCnt() const {return jointnum;}
	inline jjoint* getjointsArr() const { return joints; }
	inline int* getTableArr() const {return table;}
	matrix_float4x4 transOfJointAt(int i);
	
	void setFromFile(char* tableBytes, char* jointBytes, char* animbytes);
    void animate(float t) const;
	jskeleton();
};
#endif /* jskeleton_hpp */
