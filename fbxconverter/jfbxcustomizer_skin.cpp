//
//  jfbxcustomizer_skin.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxcustomizer_skin.hpp"
#include <iostream>
#include "jjoint.h"

void jfbxcustomizer_skin::genjointinfos()
{
    for(int i=0;i<skin->GetClusterCount();i++)
    {
        jskinjointinfo jinfo;
        
        FbxCluster* cluster = skin->GetCluster(i);
        int joint=-1;
        
        for (int j=0;j<getSkels().size();j++)
        {
            if( getSkels()[j] == cluster->GetLink() )
            {
                joint = j;
                break;
            }
        }
        
        if(joint == -1)
        {
            cout << "link not found " << endl;
            exit(1);
        }
        
        FbxAMatrix linktrans;
        cluster->GetTransformLinkMatrix(linktrans);
        
        FbxVector4 bindt = linktrans.GetT();
        FbxVector4 bindr = linktrans.GetR();
        FbxVector4 binds = linktrans.GetS();
        if (binds[0] > 1.0001f || binds[1] > 1.0001f || binds[2] > 1.0001f ||
            binds[0] < 0.9999f || binds[1] < 0.9999f || binds[2] < 0.9999f)
        {
            puts("not ready for scale");
            exit(1);
        }
        
        jtranslation jt;
        jt.setPos(bindt[0], bindt[1], bindt[2]);
        jrotation jr;
        
        jr.euler(bindr[0], bindr[1], bindr[2], JROTATION_ORDER::XYZ);
        //TODO : scale
        
        matrix_float4x4 bindx = matrix_multiply(jt.getMat(), jr.toMat());
        bindx = matrix_invert(bindx);
        
        vector<jvertex>& vertices = getvertices();
        
        vector<jskincpinfo> newcpinfos;
        
        for(int i=0;i<cluster->GetControlPointIndicesCount();i++)
        {
            int cpi = cluster->GetControlPointIndices()[i];
            FbxVector4 originalcp = mesh->GetControlPointAt(cpi);
            
            int newcpi = -1;
            for(int j=0;j<vertices.size();j++)
            {
                if(issimilarvector(originalcp.mData, vertices[j].pos))
                {
                    newcpi = j;
                    break;
                }
            }
            if(newcpi == -1)
            {
                puts("can't find new cpi");
                exit(1);
            }
            
            jskincpinfo newcpinfo;
            newcpinfo.idx = newcpi;
            newcpinfo.weight = cluster->GetControlPointWeights()[i];
            newcpinfos.push_back(newcpinfo);
        }
        
        jinfo.inverse = bindx;
        jinfo.jointidx = joint;
        jinfo.cpinfos = newcpinfos;
        
        jointinfos.push_back(jinfo);
    }
    
    float *weightvalidate = new float[getvertices().size()];
    
    memset(weightvalidate, 0, sizeof(float) * getvertices().size());
    
    for(int i=0;i<jointinfos.size();i++)
    {
        jskinjointinfo jinfo = jointinfos[i];
        for(int j=0;j<jinfo.cpinfos.size();j++)
        {
            weightvalidate[ jinfo.cpinfos[j].idx ] += jinfo.cpinfos[j].weight;
        }
    }
    
    for(int i=0;i<getvertices().size();i++)
    {
        if(abs(weightvalidate[i] - 1) > 0.0006)
        {
            puts("cp weight not pro");
            exit(1);
        }
    }
}
vector<jskinjointinfo>& jfbxcustomizer_skin::getjointinfos()
{
    if(jointinfos.size() == 0)
    {
        if(mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) > 1)
        {
            cout << "mesh has " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformer." << endl;
            exit(1);
        }
        else if(mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) ==0)
        {
            cout << "mesh has " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformer. skipping." << endl;
            exit(1);
        }
        
        cout << "mesh has " << mesh->GetDeformerCount() << " deformers, " << mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) << " skin deformers." << endl;
        
        
        skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::EDeformerType::eSkin);
        if(skin == NULL)
        {
            cout << "no skin" << endl;
            exit(1);
        }
        
        genjointinfos();
    }
    return jointinfos;
}