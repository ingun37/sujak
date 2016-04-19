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
protected:
    bool isskel(FbxNode* node);
    void iterskel(FbxNode* node);

    vector<int> uppertable;
    vector<FbxNode*> _skels;
    vector<FbxNode*>& getSkels();
    FbxNode* _rootskel;
    jfbxcustomizer(){};
public:
    jfbxcustomizer(FbxNode* rootskel) : _rootskel(rootskel){};
    vector<int>& getUpperTable();
    
};

#endif /* jfbxcustomizer_hpp */
