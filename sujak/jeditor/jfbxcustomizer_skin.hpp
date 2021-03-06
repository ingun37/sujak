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

class jskincpinfo
{
public:
    int idx;
    float weight;
    bool operator < ( const jskincpinfo& o ) const;
} ;

typedef struct _jskinjointinfo
{
    int jointidx;
    matrix_float4x4 inverse;
    matrix_float4x4 bindmesh;
    vector<jskincpinfo> cpinfos;
} jskinjointinfo;


class jfbxcustomizer_skin : public jfbxcustomizer_vertex
{
    vector<jskinjointinfo> jointinfos;
    void genjointinfos();
    FbxSkin* skin;
public:
    vector<jskinjointinfo>& getjointinfos();
    jfbxcustomizer_skin(FbxNode* rootskel, FbxMesh* _mesh) : jfbxcustomizer_vertex(rootskel, _mesh)
    {
    }
};
#endif /* jfbxcustomizer_skin_hpp */
