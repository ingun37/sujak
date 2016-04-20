//
//  jvideomemorymapper.cpp
//  MetalVertexStreaming
//
//  Created by Build on 12/24/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "jvideomemorymapper.hpp"
#include "jrenderobject.hpp"
#include "jvideomemoryaligninfo.h"

#define ipoolmax 30000
int ipool[ipoolmax];
void jvideomemorymapper::mapToVideoMemory(jrenderobject *obj, jvideomemoryaligninfo& aligninfo)
{
	if(!inited )
	{
		puts("videomemorymapper not inited");
		exit(1);
	}
	
	memcpy(buffPosition + offsetVertex, obj->positions, sizeof(simd::float4) * obj->vertexCnt);
	memcpy(buffNormal + offsetVertex, obj->normals, sizeof(simd::float4) * obj->vertexCnt);
	memcpy(buffColor + offsetVertex, obj->colors, sizeof(simd::float4) * obj->vertexCnt);
    memcpy(buffuv + offsetVertex, obj->uvs, sizeof(simd::float2) * obj->vertexCnt);
	
	for(int i=0;i<obj->indexCnt;i++)
	{
		ipool[i] = obj->indices[i] + offsetVertex;
	}
	
	memcpy(buffIndex + offsetIndex, ipool, sizeof(int) * obj->indexCnt);
	
	aligninfo.vBufferOffset = offsetVertex;
	aligninfo.iBufferOffset = offsetIndex;
	offsetVertex += obj->vertexCnt;
	offsetIndex += obj->indexCnt;
	obj->layedout = true;
}

simd::float4* jvideomemorymapper::getPositionMemoryOf(const jrenderobject &obj)
{
	return &buffPosition[obj.getVcBufferOffset()];
}
