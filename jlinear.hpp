//
//  jlinear.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 7/11/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jlinear_hpp
#define jlinear_hpp
#include <simd/simd.h>




namespace jlinear
{
    extern void jlineartest(float a1, float b1, float c1, float d1,
                            float a2, float b2, float c2, float d2,
                            float a3, float b3, float c3, float d3 );
    extern simd::float4 makePlaneContaining3Points(simd::float3 p1, simd::float3 p2, simd::float3 p3);
}


#endif /* jlinear_hpp */
