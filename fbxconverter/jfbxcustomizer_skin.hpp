//
//  jfbxcustomizer_skin.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxcustomizer_skin_hpp
#define jfbxcustomizer_skin_hpp

#include <stdio.h>
#include <vector>
#include <simd/simd.h>
#include "jfbxcustomizer_vertex.hpp"

using namespace std;
typedef struct _jskinjointinfo
{
    int jointidx;
    int cpcnt;
    matrix_float4x4 inverse;
} jskinjointinfo;

typedef struct _jskincpinfo
{
    int idx;
    float weight;
} jskincpinfo;
class jfbxcustomizer_skin : jfbxcustomizer_vertex
{
    vector<jskinjointinfo> jointinfos;
    vector<jskincpinfo> cpinfos;
public:
    jfbxcustomizer_skin(FbxNode* rootskel, FbxMesh* _mesh) : jfbxcustomizer_skin(rootskel, _mesh)
    {
    }
};
#endif /* jfbxcustomizer_skin_hpp */
