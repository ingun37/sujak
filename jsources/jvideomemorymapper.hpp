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

#include "jvideomemoryaligninfo.h"

class jrenderobject;
class jvideomemorymapper
{
	int offsetVertex;
	int offsetIndex;
	
	bool inited;
	simd::float4* buffPosition;
	simd::float4* buffColor;
	simd::float4* buffNormal;
	int* buffIndex;
public:
	jvideomemorymapper()
	{
		offsetVertex = 0;
		offsetIndex = 0;
		inited = false;
		buffPosition = buffColor = buffNormal = NULL;
	}
	
	void init(simd::float4* _buffPosition, simd::float4* _buffColor, simd::float4* _buffNormal, int* _buffIndex)
	{
		buffPosition = _buffPosition;
		buffColor = _buffColor;
		buffNormal = _buffNormal;
		buffIndex = _buffIndex;
		inited = true;
	}
	
	void mapToVideoMemory(jrenderobject *obj, jvideomemoryaligninfo& info);

	simd::float4* getPositionMemoryOf(const jrenderobject &obj);
};

#endif /* jvideomemorymapper_hpp */
