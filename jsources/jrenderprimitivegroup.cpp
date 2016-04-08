//
//  jrenderprimitivegroup.cpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 4..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#include "jrenderprimitivegroup.hpp"
#include "jnode.hpp"
jrenderprimitivegroup::jrenderprimitivegroup()
{
	for(int i=0;i<jrenderprimitivegroup_maxcap;i++)
		objs[i] = NULL;
	objCnt = 0;
}

void jrenderprimitivegroup::addObj(jnode *obj)
{
	objs[objCnt++] = obj;
}