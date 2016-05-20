//
//  jfbxcustomizer_lod.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 5/3/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxcustomizer_lod.hpp"
#include <set>
#include <list>
#include <iostream>
void jfbxcustomizer_lod::makebaselod()
{
    jlod lod;
    lod.indices = getindices();
    lod.vertices = getvertices();
    lod.joints = getjointinfos();
    lods.push_back(lod);
    lodlast();
}


typedef struct _jlvertex
{
    matrix_double4x4 Q;
    jvertex v;
    
}jlvertex;

class jledge_unique
{
public:
    int i1;
    int i2;
    
    int lesser() const  { return i1 < i2? i1 : i2; }
    int greater() const { return i1 < i2? i2 : i1; }
    
    int cmp(const jledge_unique& e) const
    {
        if(greater()==e.greater())
            if(lesser()==e.lesser())
                return 0;
            else
                return lesser() < e.lesser() ? -1 : 1;
            else
                return greater() < e.greater() ? -1 : 1;
        
    }
    
    bool operator < (const jledge_unique& e) const
    {
        return cmp(e)==1;
    }
};

class jledge
{
public:
    jledge_unique unique;
    jvertex v;
    double cost;
    
    bool operator < (const jledge& e) const
    {
        return cost < e.cost;
    }
};

typedef struct _jlpoly
{
    int i1, i2, i3;
    matrix_double4x4 K;
}jlpoly;



simd::double4 solveLinearSystem(matrix_double4x4 aug, simd::double4 c)
{
    simd::double4 solution;
    
    aug = matrix_transpose(aug);
    //TODO minimum
    
    return solution;
}

jvertex interpolateVertex(jvertex& v1, jvertex& v2, float weight)
{
    jvertex interpolated;
    interpolated = (v1*weight) + (v2*(1-weight));
    return interpolated;
}

jvertex leastCostNewV(jlvertex& v1, jlvertex& v2)
{
    jvertex solution;
    matrix_double4x4 M = matrix_add( v1.Q, v2.Q );
    double det = matrix_determinant(M);
    if(-0.0001 < det && det < 0.0001)
    {
        solution = interpolateVertex(v1.v, v2.v, 0.5);
    }
    else
    {
        solution = interpolateVertex(v1.v, v2.v, 0.5);
    }
    return solution;
}

matrix_double4x4 makeK(simd::double3 polyv1, simd::double3 polyv2, simd::double3 polyv3)
{
    matrix_double4x4 K;
    
    simd::double3 v1 = polyv2 - polyv1;
    simd::double3 v2 = polyv3 - polyv1;
    
    simd::double3 up = vector_cross(v1, v2);
    up = vector_normalize(up);
    
    double len = -vector_dot(up, polyv1);
    
    simd::double4 abcd = simd::double4{ up[0], up[1], up[2], len };
    
    K.columns[0] = abcd * abcd[0];
    K.columns[1] = abcd * abcd[1];
    K.columns[2] = abcd * abcd[2];
    K.columns[3] = abcd * abcd[3];
    
    return K;
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
        p.K = makeK(V[p.i1].v.pos, V[p.i2].v.pos, V[p.i3].v.pos);
        
        V[p.i1].Q = matrix_add(V[p.i1].Q, p.K);
        V[p.i2].Q = matrix_add(V[p.i2].Q, p.K);
        V[p.i3].Q = matrix_add(V[p.i3].Q, p.K);
        
        P.push_back(p);
    }
    
    set<jledge_unique> Es;
    unsigned int repeatance=0;
    for(int i=0;i<P.size();i++)
    {
        jledge_unique edges[3];
        jlpoly& p = P[i];
        
        edges[0].i1 = p.i1;
        edges[0].i2 = p.i2;
        
        edges[1].i1 = p.i2;
        edges[1].i2 = p.i3;
        
        edges[2].i1 = p.i3;
        edges[2].i2 = p.i1;
        
        for(int j=0;j<3;j++)
        {
            auto pair = Es.insert(edges[j]);
            if(!pair.second)
                repeatance++;
        }
    }
    
    cout << "edge repeat : " << repeatance << endl;
    
    list<jledge> El;
    
    for(set<jledge_unique>::iterator it = Es.begin();it!=Es.end();it++)
    {
        jledge e;
        e.unique = *it;
        jlvertex v1, v2;
        v1 = V[e.unique.i1];
        v2 = V[e.unique.i2];
        matrix_double4x4 Q = matrix_add( v1.Q, v2.Q );
        e.v = leastCostNewV(v1, v2);
        e.cost = vector_dot(e.v.pos4(), matrix_multiply(Q, e.v.pos4()));
        El.push_back(e);
    }
    
    El.sort();
    
    
}

vector<jlod>& jfbxcustomizer_lod::getlods()
{
    if(lods.size()==0)
    {
        makebaselod();
        
    }
    return lods;
}