//
//  jcore.hpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 4..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jcore_hpp
#define jcore_hpp


#include <stdio.h>

#include "jdefinitions.h"
#include "jallocator.hpp"

class jskeleton;

typedef void(*platformSpecificSetRenderState)(JRenderState state);
typedef void(*platformSpecificSetPrimitive)(JRenderPrimitive prim);
typedef void(*platformSpecificRenderIndexed)(int offset, int cnt);
typedef void(*platformSpecificGetFile)(const char* szFileName, const char* szExt, char* &file, unsigned long& size);
typedef void(*loadSkeletonsPlatformSpecific)(jskeleton* objs, const int len, int &cap);


typedef jallocator<char, 1024*1024> filepool;

class jcore
{
	
	filepool fpool;
public:
	jcore();
	void update();
	
	void render( platformSpecificSetRenderState, platformSpecificSetPrimitive, platformSpecificRenderIndexed);

	void initVideoMemoryMapper(simd::float4 *_buffPosition, simd::float4 *_buffColor, simd::float4 *_buffNormal, int *_buffIndex);
	void layout();
	

	void loadAll( platformSpecificGetFile pfunc );
	
	filepool& getFilePool() {return fpool;}
};
#endif /* jcore_hpp */
