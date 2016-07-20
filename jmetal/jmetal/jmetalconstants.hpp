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
#include "jconstants.hpp"

extern size_t jmetalconstanttypesize[sujak::JDataTypeVertex_number];
extern MTLVertexFormat jmetalconstantmetaltype[sujak::JDataTypeVertex_number];
extern const char* jmetalconstantshaderentryvert[sujak::JShaderVert_number];
extern const char* jmetalconstantshaderentryfrag[sujak::JShaderFrag_number];
extern MTLIndexType jmetalconstantindextype[sujak::JDataTypeIndex_number];
extern MTLPixelFormat jmetalconstantpixeltype[sujak::JPixelFormat_number];
#endif /* jmetalconstants_h */
