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
using namespace std;
typedef struct _jvertex
{
    double pos[3];
    double nor[3];
} jvertex;
class jfbxcustomizer_vertex : public jfbxcustomizer
{
    void genVerticesAndIndices();
    
    vector<jvertex> vertices;
    
    
    vector<int> indices;
    
protected:
    
    FbxMesh* mesh;
    bool issimilarvector(double* v1, double* v2);
    bool issimilarvertex(jvertex v1, jvertex v2);
public:
    vector<jvertex>& getvertices();
    vector<int>& getindices();
    jfbxcustomizer_vertex(FbxNode* rootskel, FbxMesh* _mesh) : jfbxcustomizer(rootskel)
    {
        mesh = _mesh;
    }
};
#endif /* jfbxcustomizer_vertex_hpp */
