//
//  jnode.hpp
//  MetalVertexStreaming
//
//  Created by Build on 12/7/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jnode_hpp
#define jnode_hpp

#include <stdio.h>
#include <simd/simd.h>
#include "jrotation.hpp"
#include "jtranslation.hpp"
namespace sujak
{
    class jrenderobject;
    class jskeleton;
    class jskinner;
    class jnode
    {
        friend jnode;
        jrenderobject* renderobj;
        jskeleton* skeleton;
        jskinner* skinner;
        
        jrotation lrot;
        jtranslation ltran;
        matrix_float4x4 local;
        
        bool localsPrecomputed;
        matrix_float4x4 *precomputeClusterDependencies;// Inverse * Bind
        simd::float4 **precomputeLinkDependencies; //Position * weight
        
    public:
        bool shouldRemap;//todo : bitfield
        
        jnode()
        {
            renderobj = NULL;
            skeleton = NULL;
            skinner = NULL;
            shouldRemap = false;
            local = matrix_identity_float4x4;
        }
        
        inline void setData( jrenderobject* r, jskeleton* s, jskinner* k )
        {
            renderobj = r;
            skeleton = s;
            skinner = k;
        }
        
        inline void testtrans(float mx, float my, float ry)
        {
            ltran.setx(mx);
            ltran.sety(my);
            lrot.onlyy(ry);
            local = matrix_multiply(ltran.getMat(),lrot.toMat(XYZ));
        }
        
        inline jrenderobject* getRenderObject() const {return renderobj;}
        inline jskeleton* getSkeleton() {return skeleton;}
        inline jskinner* getSkinner() const {return skinner;}
        
        void clone(jnode& node);
        void computeAndStoreSkinnedPositionTo(simd::float4* dest);
        
        void precomputeLocals();
    };
}
#endif /* jnode_hpp */
