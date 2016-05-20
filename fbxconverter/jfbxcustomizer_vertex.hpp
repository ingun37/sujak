//
//  jfbxcustomizer_vertex.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxcustomizer_vertex_hpp
#define jfbxcustomizer_vertex_hpp

#include <stdio.h>
#include <vector>
#include "fbxsdk.h"
#include "jfbxcustomizer.hpp"
#include <simd/simd.h>
using namespace std;
class jvertex
{
public:
    simd::double3 pos;
    simd::double3 nor;
    simd::double2 uv;
    jvertex operator + (const jvertex& v);
    jvertex operator * (float f);
    
    simd::double4 pos4()
    {
        simd::double4 v = (simd::double4){pos[0], pos[1], pos[2], 1};
        return v;
    }
    bool issimilarto(const jvertex& v)
    {
        if( vector_length(pos - v.pos) > 0.0001)
            return false;
        if( vector_length(uv - v.uv) > 0.0001)
            return false;
        if( vector_length(nor - v.nor) > 0.0001)
            return false;
        return true;
    }
};
class jfbxcustomizer_vertex : public jfbxcustomizer
{
    void genVerticesAndIndices();
    
    vector<jvertex> vertices;
    
    
    vector<int> indices;
    
protected:
    
    FbxMesh* mesh;
    
    
    
public:
    vector<jvertex>& getvertices();
    vector<int>& getindices();
    jfbxcustomizer_vertex(FbxNode* rootskel, FbxMesh* _mesh) : jfbxcustomizer(rootskel)
    {
        mesh = _mesh;
    }
};
#endif /* jfbxcustomizer_vertex_hpp */
