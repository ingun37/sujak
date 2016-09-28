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

#include "jconstants.hpp"
#include "jvideomemoryaligninfo.h"

namespace sujak
{
    class jrenderobject
    {
        bool dataset;
        
        void* attributeDatas[JVertexAttribute_number];
        
        unsigned int vertexCnt;
        void* indices;
        unsigned int indexCnt;
        
    public:
		
        //todo nocopy
        inline int getIndexCnt() const {return indexCnt;}
        inline int getVertexCnt() const {return vertexCnt;}
        
        //inline simd::float4 getPositionAt(int i) const {return ((simd::float4*)attributeDatas[JVertexAttribute_position])[i];}
		
		inline const void* getDataForCopy(JVertexAttribute att){ return attributeDatas[att]; }
		inline const void* const * getData2DForCopy(){ return attributeDatas; }
		inline const void* getIndexDataForCopy() { return indices; }
        void setData(void* p, void* n, void* c, void* u, unsigned int vc, void* i, unsigned int ic);
        
        jrenderobject();
        void clone(jrenderobject& robj);
    };
}
#endif /* jrenderobject_hpp */
