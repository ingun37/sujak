//
//  jfbxvalidater.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxvalidater_hpp
#define jfbxvalidater_hpp

#include <stdio.h>
#include <vector>
#include "fbxsdk.h"
using namespace std;
class jfbxvalidater
{
    
public:
    static void uppertable(const vector<int>& arr);
    static void keycnts(vector<vector<unsigned char>>& arr, int skelcnt);
};
#endif /* jfbxvalidater_hpp */
