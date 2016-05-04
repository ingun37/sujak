//
//  jfbxcustomizer_lod.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 5/3/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxcustomizer_lod.hpp"
#include <set>
void jfbxcustomizer_lod::makebaselod()
{
    jlod lod;
    lod.indices = getindices();
    lod.vertices = getvertices();
    lod.joints = getjointinfos();
    lods.push_back(lod);
}

class jledge
{
public:
    int i1;
    int i2;
    simd::double3 v;
    float cost;
    int lesser() const  { return i1 < i2? i1 : i2; }
    int greater() const { return i1 < i2? i2 : i1; }
    int cmp(const jledge& e) const
    {
        if(greater()==e.greater())
            if(lesser()==e.lesser())
                return 0;
            else
                return lesser() < e.lesser() ? -1 : 1;
        else
            return greater() < e.greater() ? -1 : 1;
        
    }
    bool operator < (const jledge& e) const
    {
        return cmp(e)==1;
    }
};

typedef struct _jlpoly
{
    int i1, i2, i3;
    matrix_double4x4 K;
}jlpoly;

typedef struct _jlvertex
{
    matrix_double4x4 Q;
    jvertex v;
}jlvertex;

jvertex interpolateVertex(jvertex& v1, jvertex& v2, float weight)
{
    jvertex interpolated;
    
    
    
    
    return interpolated;
}
jlvertex leastCostNewV(jlvertex& v1, jlvertex& v2)
{
    jlvertex solution;
    matrix_double4x4 M = matrix_add( v1.Q, v2.Q );
    double det = matrix_determinant(M);
    if(-0.0001 < det && det < 0.0001)
    {
        
    }
    
    return solution;
}

void jfbxcustomizer_lod::lodlast()
{
    jlod orig = lods[lods.size()-1];
    
    const simd::double4 empty4 = simd::double4{0,0,0,0};
    
    vector<jlvertex> V;
    for(int i=0;i<orig.vertices.size();i++)
    {
        jlvertex v;
        v.Q.columns[0] = empty4;
        v.Q.columns[1] = empty4;
        v.Q.columns[2] = empty4;
        v.Q.columns[3] = empty4;
        v.v = orig.vertices[i];
        V.push_back(v);
    }
    
    if(orig.indices.size()%3 != 0)
    {
        puts("indices mod 3 not 0");
        exit(1);
    }

    vector<jlpoly> P;
    for(int i=0;i<orig.indices.size();i+=3)
    {
        jlpoly p;
        p.i1 = orig.indices[i+0];
        p.i2 = orig.indices[i+1];
        p.i3 = orig.indices[i+2];
        simd::double3 p1 = V[p.i1].v.pos;
        simd::double3 p2 = V[p.i2].v.pos;
        simd::double3 p3 = V[p.i3].v.pos;
        
        simd::double3 v1 = p2-p1;
        simd::double3 v2 = p3-p1;
        
        simd::double3 up = vector_cross(v1, v2);
        
        up = simd::normalize(up);
        
        double len = simd::dot(up, p1);
        
        simd::double4 plane = simd::double4{ up[0], up[1], up[2], len };
        
        p.K.columns[0] = plane * plane[0];
        p.K.columns[1] = plane * plane[1];
        p.K.columns[2] = plane * plane[2];
        p.K.columns[3] = plane * plane[3];
        
        V[p.i1].Q = matrix_add(V[p.i1].Q, p.K);
        V[p.i2].Q = matrix_add(V[p.i2].Q, p.K);
        V[p.i3].Q = matrix_add(V[p.i3].Q, p.K);
    }
    
    set<jledge> E;
    for(int i=0;i<P.size();i++)
    {
        jledge edges[3];
        jlpoly& p = P[i];
        
        edges[0].i1 = p.i1;
        edges[0].i2 = p.i2;
        
        edges[1].i1 = p.i2;
        edges[1].i2 = p.i3;
        
        edges[2].i1 = p.i3;
        edges[2].i2 = p.i1;
        for(int j=0;j<3;j++)
        {
            auto jpair = E.insert(edges[j]);
            if(!jpair.second)
                continue;
            set<jledge>::iterator e = jpair.first;
            matrix_double4x4 q = matrix_add( V[e->i1].Q, V[e->i2].Q );
            q.columns[0][3] = q.columns[1][3] = q.columns[2][3] = 0;
            q.columns[3][3] = 1;
        }
    }
}

vector<jlod>& jfbxcustomizer_lod::getlods()
{
    if(lods.size()==0)
    {
        makebaselod();
        
    }
    return lods;
}