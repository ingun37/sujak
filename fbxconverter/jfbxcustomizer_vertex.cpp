//
//  jfbxcustomizer_vertex.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxcustomizer_vertex.hpp"
#include <math.h>

jvertex jvertex::operator+(const jvertex &v) const
{
    jvertex r;
    r.pos = pos + v.pos;
    r.nor = vector_normalize( nor + v.nor );
    r.uv = uv + v.uv;
    return r;
}

jvertex jvertex::operator*(float f) const
{
    jvertex v;
    v.pos = pos * f;
    v.nor = vector_normalize( nor );
    v.uv = uv * f;
    return v;
}

void jfbxcustomizer_vertex::genVerticesAndIndices()
{
    printf("gen vert and indices... cp point : %d\n",mesh->GetControlPointsCount());
    vector<vector<jvertex>> extended;
    printf("uv element cnt : %d\n", mesh->GetElementUVCount());
    
    FbxStringList uvsetnamelist;
    mesh->GetUVSetNames(uvsetnamelist);
    
    if(uvsetnamelist.GetCount() != 1)
    {
        puts("uv name list is not 1");
        exit(1);
    }
    
    char* uvsetname = uvsetnamelist.GetStringAt(0);
    
    for(int pi = 0 ; pi < mesh->GetPolygonCount() ; pi++)
    {
        vector<jvertex> newpolygonvertices;
        for(int vi = 0 ; vi < mesh->GetPolygonSize(pi);vi++)
        {
            int cpi = mesh->GetPolygonVertex(pi, vi);
            FbxVector4 position = mesh->GetControlPointAt(cpi);
            FbxVector4 normal;
            bool success = mesh->GetPolygonVertexNormal(pi, vi, normal);
            
            if(!success)
            {
                puts("normal err");
                exit(1);
            }
            
            FbxVector2 uv;
            bool unmaped;
            success = mesh->GetPolygonVertexUV(pi, vi, uvsetname, uv, unmaped);
            if(unmaped || !success)
            {
                puts("uv err");
                exit(1);
            }
            
            jvertex newvertex;
            newvertex.nor[0] = normal[0];
            newvertex.nor[1] = normal[1];
            newvertex.nor[2] = normal[2];
            
            newvertex.pos[0] = position[0];
            newvertex.pos[1] = position[1];
            newvertex.pos[2] = position[2];
            
            newvertex.uv[0] = uv[0];
            newvertex.uv[1] = uv[1];
            
            newpolygonvertices.push_back(newvertex);
        }
        extended.push_back(newpolygonvertices);
    }
    
    vector<vector<int>> nontriangle_indices;
    for(int pi = 0 ; pi < extended.size() ; pi++)
    {
        vector<int> nontriangle_polygon_indices;
        for(int vi = 0 ; vi < extended[pi].size();vi++)
        {
            jvertex& current = extended[pi][vi];
            
            int isame = 0;
            for(isame = 0; isame<vertices.size();isame++)
            {
                jvertex& cmp = vertices[isame];
                if(current.issimilarto(cmp))
                    break;
            }
            
            if(isame == vertices.size())
                vertices.push_back(current);
            
            nontriangle_polygon_indices.push_back(isame);
        }
        nontriangle_indices.push_back(nontriangle_polygon_indices);
    }
    
    for(int pi = 0;pi<nontriangle_indices.size();pi++)
    {
        if(nontriangle_indices[pi].size() == 3)
        {
            indices.push_back( nontriangle_indices[pi][0] );
            indices.push_back( nontriangle_indices[pi][1] );
            indices.push_back( nontriangle_indices[pi][2] );
        }
        else if(nontriangle_indices[pi].size() > 3)
        {
            for(int vi = 0;vi<nontriangle_indices[pi].size()-2;vi++)
            {
                indices.push_back( nontriangle_indices[pi][0] );
                indices.push_back( nontriangle_indices[pi][vi+1] );
                indices.push_back( nontriangle_indices[pi][vi+2] );
            }
        }
        else
        {
            puts("polygon sized 2 ? ");
            exit(1);
        }
    }
    
    printf("cp after : %ld\n", vertices.size());
}

vector<jvertex>& jfbxcustomizer_vertex::getvertices()
{
    if(vertices.size()==0)
    {
        if(indices.size() != 0)
        {
            puts("indices vertices sync err");
            exit(1);
        }
        genVerticesAndIndices();
    }
    return vertices;
}

vector<int>& jfbxcustomizer_vertex::getindices()
{
    if(indices.size()==0)
    {
        if(vertices.size() != 0)
        {
            puts("vertices indices sync err");
            exit(1);
        }
        genVerticesAndIndices();
    }
    return indices;
}