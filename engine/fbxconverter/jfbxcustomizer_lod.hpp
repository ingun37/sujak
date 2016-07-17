//
//  jfbxcustomizer_lod.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 5/3/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxcustomizer_lod_hpp
#define jfbxcustomizer_lod_hpp

#include <stdio.h>
#include <vector>
#include "jfbxcustomizer_skin.hpp"
using namespace std;
typedef struct _jlod
{
    vector<int> indices;
    vector<jvertex> vertices;
    vector<jskinjointinfo> joints;
} jlod;

class jfbxcustomizer_lod : public jfbxcustomizer_skin
{
    vector<jlod> lods;
    void makebaselod();
    void lodlast();
public:
    vector<jlod>& getlods();
    jfbxcustomizer_lod(FbxNode* rootskel, FbxMesh* _mesh) : jfbxcustomizer_skin(rootskel, _mesh)
    {
    }
};

#endif /* jfbxcustomizer_lod_hpp */
