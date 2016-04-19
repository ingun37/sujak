//
//  jfbxcustomizer.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxcustomizer_hpp
#define jfbxcustomizer_hpp

#include <vector>
#include "fbxsdk.h"
#include "jfbxvalidater.hpp"
using namespace std;

class jfbxcustomizer
{
    vector<FbxNode*> _skels;
    vector<int> uppertable;
    
    bool isskel(FbxNode* node);
    void iterskel(FbxNode* node);
    jfbxcustomizer();
protected:
    FbxNode* _rootskel;
public:
    jfbxcustomizer(FbxNode* rootskel);
    vector<int>& getUpperTable();
    vector<FbxNode*>& getSkels();
};

#endif /* jfbxcustomizer_hpp */
