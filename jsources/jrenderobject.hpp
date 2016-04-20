//
//  jrenderobject.hpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 3..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jrenderobject_hpp
#define jrenderobject_hpp

#include <stdio.h>
#include <simd/simd.H>

#include "jdefinitions.h"
#include "jvideomemoryaligninfo.h"

class jrenderobject
{
	friend class jvideomemorymapper;

	//TODO : bitfield
	bool layedout;
	bool dataset;
	
    void* attributeDatas[JVertexAttribute_number];
    
	int vertexCnt;
	int* indices;
	int indexCnt;

public:

	jvideomemoryaligninfo aligninfo;
	//todo nocopy
	inline int getIndexCnt() const {return indexCnt;}
	inline int getVertexCnt() const {return vertexCnt;}
	inline int getVcBufferOffset() const {return aligninfo.vBufferOffset;}
	inline int getIBufferOffset() const {return aligninfo.iBufferOffset;}
    inline simd::float4 getPositionAt(int i) const {return ((simd::float4*)attributeDatas[JVertexAttribute_position])[i];}

    void setData(simd::float4* p, simd::float4* n, simd::float4* c, simd::float2* u, int vc, int* i, int ic);
	
	jrenderobject();
	
};

#endif /* jrenderobject_hpp */
