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
#include <math.h>
matrix_float4x4 getjtransform(FbxVector4 m, FbxVector4 r, FbxVector4 s)
{
    if(abs(s[0]-1) > 0.0001 ||
       abs(s[1]-1) > 0.0001 ||
       abs(s[2]-1) > 0.0001 )
    {
        puts("not ready for s");
        exit(1);
    }
    jtranslation jt;
    jt.setPos(m[0], m[1], m[2]);
    jrotation jr;
    jr.euler_degree(r[0], r[1], r[2], JROTATION_ORDER::XYZ);
    
    return matrix_multiply(jt.getMat(), jr.toMat());
}
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
        
        matrix_float4x4 bindinverse = matrix_invert( getjtransform(bindt, bindr, binds) );
        
        FbxAMatrix meshtrans;
        cluster->GetTransformMatrix(meshtrans);
        
        bindt = meshtrans.GetT();
        bindr = meshtrans.GetR();
        binds = meshtrans.GetS();
        
        matrix_float4x4 bindmesh = getjtransform(bindt, bindr, binds);
        
        vector<jvertex>& vertices = getvertices();
        vector<jskincpinfo> newcpinfos;
        
        for(int icpi=0;icpi<cluster->GetControlPointIndicesCount();icpi++)
        {
            int cpi = cluster->GetControlPointIndices()[icpi];
            FbxVector4 originalcp = mesh->GetControlPointAt(cpi);
            
            vector<int> newcpis;
            for(int j=0;j<vertices.size();j++)
            {
                if(issimilarvector(originalcp.mData, vertices[j].pos, 3))
                {
                    newcpis.push_back(j);
                }
            }
            if(newcpis.size() == 0)
            {
                puts("can't find new cpi");
                exit(1);
            }
            
            for(int inew=0;inew<newcpis.size();inew++)
            {
                jskincpinfo newcpinfo;
                newcpinfo.idx = newcpis[inew];
                newcpinfo.weight = cluster->GetControlPointWeights()[icpi];
                newcpinfos.push_back(newcpinfo);
            }
        }
        
        jinfo.inverse = bindinverse;
        jinfo.bindmesh = bindmesh;
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