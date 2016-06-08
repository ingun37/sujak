//
//  jrenderprimitivegroup.hpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 4..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jrenderprimitivegroup_hpp
#define jrenderprimitivegroup_hpp

#include <stdio.h>

#define jrenderprimitivegroup_maxcap 80

class jnode;
class jrenderprimitivegroup
{
public:
	jnode* objs[jrenderprimitivegroup_maxcap];
	int objCnt;
    unsigned long min;
    unsigned long total;
	void addObj(jnode* obj);
    void calculateMinMax();
	jrenderprimitivegroup();
};
#endif /* jrenderprimitivegroup_hpp */
