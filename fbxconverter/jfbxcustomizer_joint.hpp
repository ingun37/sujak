//
//  jfbxcustomizer_joint.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxcustomizer_joint_hpp
#define jfbxcustomizer_joint_hpp

#include <stdio.h>
#include "jfbxcustomizer.hpp"
#include "jjoint.h"
#include <vector>
using namespace std;
class jfbxcustomizer_joint : jfbxcustomizer
{
    vector<jjoint> joints;
public:
    jfbxcustomizer_joint(FbxNode* rootskel);
    vector<jjoint>& getJoints();
};

#endif /* jfbxcustomizer_joint_hpp */
