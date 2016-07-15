//
//  jvideomemorymapper.hpp
//  MetalVertexStreaming
//
//  Created by Build on 12/24/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jvideomemorymapper_hpp
#define jvideomemorymapper_hpp

#include <stdio.h>
#include <simd/simd.h>
#include "jconstants.h"
#include "jvideomemoryaligninfo.h"

class jrenderobject;
class jvideomemorymapper
{
	int offsetVertex;
	int offsetIndex;
	
	bool inited;
    void* buffers[JVertexAttribute_number];

	int* buffIndex;
public:
	jvideomemorymapper()
	{
		offsetVertex = 0;
		offsetIndex = 0;
		inited = false;
		
        buffIndex = NULL;
	}
	
    void init(void* _buffers[], int* _buffIndex)
	{
        for(int i=0;i<JVertexAttribute_number;i++)
        {
            if(_buffers[i] == NULL)
                throw "video mem err";
            buffers[i] = _buffers[i];
        }
        
		buffIndex = _buffIndex;
		inited = true;
	}
	
	void mapToVideoMemory(jrenderobject *obj);

	simd::float4* getPositionMemoryOf(const jrenderobject &obj);
};

#endif /* jvideomemorymapper_hpp */
