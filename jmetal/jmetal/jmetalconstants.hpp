//
//  jmetalconstants.h
//  jmetal
//
//  Created by ingun on 9/19/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmetalconstants_h
#define jmetalconstants_h

#import <Metal/Metal.h>
#import <simd/simd.h>

enum jmetalvertexattribute
{
    jmetalvertexattribute_position,
    jmetalvertexattribute_normal,
    jmetalvertexattribute_color,
    jmetalvertexattribute_uv,
};

#define jmetalconstantvertextype_position   (simd::float4)
#define jmetalconstantvertextypesize_position sizeof jmetalconstantvertextype_position
#define jmetalconstantvertexmetaltype       MTLVertexFormatFloat4

#define jmetalconstantvertextype_normal     (simd::float4)
#define jmetalconstantvertextypesize_normal sizeof jmetalconstantvertextype_normal
#define jmetalconstantvertexmetaltype       MTLVertexFormatFloat4

#define jmetalconstantvertextype_color      (simd::float4)
#define jmetalconstantvertextypesize_color  sizeof jmetalconstantvertextype_color
#define jmetalconstantvertexmetaltype       MTLVertexFormatFloat4

#define jmetalconstantvertextype_uv         (simd::float2)
#define jmetalconstantvertextypesize_uv     sizeof jmetalconstantvertextype_uv
#define jmetalconstantvertexmetaltype       MTLVertexFormatFloat2

#define jmetalconstantindextype             (unsigned int)
#define jmetalconstantindextypesize         sizeof jmetalconstantindextype
#define jmetalconstantindexmetaltype        MTLIndexTypeUInt32

#endif /* jmetalconstants_h */
