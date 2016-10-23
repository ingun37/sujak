//
//  jfbxvalidater.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxvalidater.hpp"

#define jassert(A...) printf(A);\
exit(1);


void jfbxvalidater::uppertable(const vector<int> &arr)
{
    if(arr[0] != -1)
    {
        jassert("root skel's parent is not -1")
    }
    for(int i=0;i<arr.size();i++)
    {
        if(arr[i] >= i)
        {
            jassert("skel arr wrong");
        }
    }
}

void jfbxvalidater::keycnts( vector<vector<unsigned char>> &arr, int skelcnt)
{
    if(arr.size()!=skelcnt)
    {
        jassert("123123123");
    }
}