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
class jcurvenode;
class jskeleton
{
    friend jskeleton;
	jjoint *joints;
    
	int* table;
	
	int jointnum;
    jcurvenode *curvenodes;
    matrix_float4x4* globals;
    
    int *currentkeys;
    float currentt;
    bool animating;
public:
    void clone(jskeleton& skeleton);
    
	inline int getJointCnt() const {return jointnum;}
	inline jjoint* getjointsArr() const { return joints; }
	inline int* getTableArr() const {return table;}
    inline bool isanimating() { return animating; }
    matrix_float4x4* computeglobals();
    
	void setFromFile(char* tableBytes, char* jointBytes, char* animbytes);
    void animateto(float at);
    void advance(float dt);
	jskeleton();
};
#endif /* jskeleton_hpp */
