//
//  jconstants.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/20/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jconstants.h"

JVertexType jvertexattribtypes[JVertexAttribute_number] = {JVertexType_f4, JVertexType_f4, JVertexType_f4, JVertexType_f2};
JBufferIndex jvertexbufferindices[JVertexAttribute_number] = { JBufferIndex_vertex_position, JBufferIndex_vertex_normal, JBufferIndex_vertex_color, JBufferIndex_vertex_uv };
#define kvertexcnt 2000
#define kMaxBufferBytesPerFrame (kvertexcnt * 4 * 4 * 35)
unsigned long jvertexbuffersizes[JVertexAttribute_number] = {kMaxBufferBytesPerFrame, kMaxBufferBytesPerFrame, kMaxBufferBytesPerFrame, kMaxBufferBytesPerFrame};
#define kindexcnt 8000
unsigned long jindexbuffersize = kindexcnt * 4 * 35;