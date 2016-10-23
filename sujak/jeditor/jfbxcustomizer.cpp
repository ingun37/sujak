//
//  jfbxcustomizer.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include <stack>
#include "jfbxcustomizer.hpp"



bool jfbxcustomizer::isskel(FbxNode *node)
{
    if(node->GetNodeAttribute() == NULL)
        return false;
    if(node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
        return true;
    return false;
}

void jfbxcustomizer::iterskel(FbxNode *node)
{
    if(!isskel(node))
        return;
    _skels.push_back(node);
    for(int i=0;i<node->GetChildCount();i++)
        iterskel(node->GetChild(i));
}
vector<FbxNode*>& jfbxcustomizer::getSkels()
{
    if(_skels.size() == 0)
    {
        puts("start itering skel");
        iterskel(_rootskel);
    }

    return _skels;
}
vector<int>& jfbxcustomizer::getUpperTable()
{
    if(uppertable.size() > 0)
        return uppertable;
    
    vector<FbxNode*>& skels = getSkels();
    
    for(int i=0;i<skels.size();i++)
    {
        FbxNode* skel = skels[i];
        int upperidx = -1;
        
        for(int j=0;j<i;j++)
            if(skel->GetParent() == skels[j])
                upperidx = j;
        
        uppertable.push_back(upperidx);
    }
    jfbxvalidater::uppertable(uppertable);
    return uppertable;
}