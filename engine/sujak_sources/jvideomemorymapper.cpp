#include <stdlib.h>
#include <string.h>
#include "jvideomemorymapper.hpp"
#include "jrenderobject.hpp"
#include "jvideomemoryaligninfo.h"

#define ipoolmax 30000
int ipool[ipoolmax];
void jvideomemorymapper::mapToVideoMemory(jrenderobject *obj)
{
	if(!inited )
	{
		puts("videomemorymapper not inited");
		exit(1);
	}
	
    for(int i=0;i<JVertexAttribute_number;i++)
    {
        int leapterm = 0;
        switch (jvertexattribtypes[i])
        {
            case JVertexType_f2:
                leapterm = sizeof(simd::float2);
                break;
            case JVertexType_f4:
                leapterm = sizeof(simd::float4);
            default:
                break;
        }
        memcpy((char*)buffers[i] + (leapterm*offsetVertex), obj->attributeDatas[i], leapterm * obj->vertexCnt);
    }
    
	for(int i=0;i<obj->indexCnt;i++)
	{
		ipool[i] = obj->indices[i] + offsetVertex;
	}
	
	memcpy(buffIndex + offsetIndex, ipool, sizeof(int) * obj->indexCnt);
	
	obj->aligninfo.vBufferOffset = offsetVertex;
	obj->aligninfo.iBufferOffset = offsetIndex;
	offsetVertex += obj->vertexCnt;
	offsetIndex += obj->indexCnt;
	obj->layedout = true;
}

simd::float4* jvideomemorymapper::getPositionMemoryOf(const jrenderobject &obj)
{
    return &(((simd::float4*)buffers[JVertexAttribute_position])[obj.getVcBufferOffset()]);
}
