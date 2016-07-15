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
    extern void getReducedEchelonMatrix34(float m[3][4], simd::float4 v1, simd::float4 v2, simd::float4 v3);
    extern matrix_double4x4 makePlaneDistanceK(simd::double3 p1, simd::double3 p2, simd::double3 p3);
}


#endif /* jlinear_hpp */
