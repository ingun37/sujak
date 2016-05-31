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
#include <algorithm>
#include <iterator>
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
    
    bool hasit(int i)
    {
        if(i1 == i)
            return true;
        if(i2 == i)
            return true;
        return false;
    }
    
    void changeto(int a, int b)
    {
        if(i1 == a)
            i1 = b;
        else if(i2 == a)
            i2 = b;
        else
        {
            cout << "(changeto 2)" <<endl;
            exit(1);
        }
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

class jlpoly
{
public:
    int i1, i2, i3;
    matrix_double4x4 K;
    
    bool hasit(int a)
    {
        if(i1 == a || i2 == a || i3 == a)
            return true;
        return false;
    }
    
    bool hasboth(int a, int b)
    {
        if(hasit(a) && hasit(b))
            return true;
        return false;
    }
    
    void changeto(const int a, const int b)
    {
        if(i1 == a)
            i1 = b;
        else if(i2 == a)
            i2 = b;
        else if(i3 == a)
            i3 = b;
        else
        {
            cout << "(changeto) " << a << " " << b << " " << i1 << " " << i2 << " " << i3 << endl;
            exit(1);
        }
    }
};



simd::double4 solveLinearSystem(matrix_double4x4 aug, simd::double4 c)
{
    simd::double4 solution;
    
    aug = matrix_transpose(aug);
    //TODO minimum
    
    return solution;
}

jvertex interpolateVertex(const jvertex& v1, const jvertex& v2, float weight)
{
    jvertex interpolated;
    interpolated = (v1*weight) + (v2*(1-weight));
    return interpolated;
}

jvertex leastCostNewV(const jlvertex& v1, const jlvertex& v2)
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
    const jlod &orig = lods[lods.size()-1];
    
    const simd::double4 empty4 = simd::double4{0,0,0,0};
    const matrix_double4x4 mEmpty{empty4, empty4, empty4, empty4};
    vector<jlvertex> V;
    for(int i=0;i<orig.vertices.size();i++)
    {
        jlvertex v;
        v.Q = mEmpty;
        v.v = orig.vertices[i];
        V.push_back(v);
    }
    
    if(orig.indices.size()%3 != 0)
    {
        puts("indices mod 3 not 0");
        exit(1);
    }

    list<jlpoly> P;
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
    set<jledge_unique> Edup;
    
    for(list<jlpoly>::iterator it = P.begin() ; it != P.end() ; it++)
    {
        jledge_unique edges[3];
        jlpoly& p = *it;
        
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
            {
                Edup.insert(edges[j]);
            }
        }
    }
    
    set<jledge_unique> Eedge;
    
    set_difference(Es.begin(), Es.end(), Edup.begin(), Edup.end(), inserter(Eedge, Eedge.begin()));
    
    set<int> Ve;
    
    for(set<jledge_unique>::iterator it = Eedge.begin();it!=Eedge.end();it++)
    {
        Ve.insert(it->i1);
        Ve.insert(it->i2);
    }
    
    cout << "edge vertices : " << Ve.size() << endl;
    cout << "Es before : " << Es.size() << endl;
    
    {
        set<jledge_unique>::iterator it = Es.begin();
        while(it != Es.end())
        {
            if( Ve.find(it->i1) != Ve.end() || Ve.find(it->i2) != Ve.end())
            {
                it = Es.erase(it);
                continue;
            }
            it++;
        }
    }
    
    cout << "Es after : " << Es.size() << endl;
    
    list<jledge> El;
    
    for(set<jledge_unique>::iterator it = Es.begin();it!=Es.end();it++)
    {
        jledge e;
        e.unique = *it;
        
        const jlvertex &v1 = V[e.unique.i1];
        const jlvertex &v2 = V[e.unique.i2];
        matrix_double4x4 Q = matrix_add( v1.Q, v2.Q );
        e.v = leastCostNewV(v1, v2);
        e.cost = vector_dot(e.v.pos4(), matrix_multiply(Q, e.v.pos4()));
        El.push_back(e);
    }
    
    while(1)
    {
        El.sort();
        
        int n = static_cast<int>( V.size() );
        V.push_back(jlvertex());
        vector<jlvertex>::iterator itv = V.end()-1;
        
        itv->v = El.front().v;
        int i1 = El.front().unique.i1;
        int i2 = El.front().unique.i2;
        
        El.pop_front();
        
        list<jlpoly>::iterator itp = P.begin();
        
        vector<matrix_double4x4> adjK;
        set<int> adjV;
        while(itp != P.end())
        {
            if( itp->hasboth(i1, i2))
            {
                itp = P.erase(itp);
                continue;
            }
            else if(itp->hasit(i1))
            {
                itp->changeto(i1, n);
            }
            else if(itp->hasit(i2))
            {
                itp->changeto(i2, n);
            }
            
            if(itp->hasit(n))
            {
                itp->K = makeK(V[itp->i1].v.pos, V[itp->i2].v.pos, V[itp->i3].v.pos);
                adjK.push_back(itp->K);
                adjV.insert(itp->i1);
                adjV.insert(itp->i2);
                adjV.insert(itp->i3);
            }
            itp++;
        }
        //P done
        matrix_double4x4 test = mEmpty;
        
        for(vector<matrix_double4x4>::iterator it = adjK.begin() ; it!= adjK.end();it++)
            test = matrix_add(test, *it);
        
        for(set<int>::iterator ita = adjV.begin();ita!=adjV.end();ita++)
        {
            V[*ita].Q = mEmpty;
            for(itp = P.begin();itp!=P.end();itp++)
            {
                if(itp->hasit(*ita))
                    V[*ita].Q = matrix_add(V[*ita].Q, itp->K);
            }
        }
        
        {//test area
            matrix_double4x4 test1 = V[n].Q;
            matrix_sub(test1, test);
            
        }
        //V done
        for(set<int>::iterator ita = adjV.begin();ita!=adjV.end();ita++)
        {
            for(list<jledge>::iterator ite = El.begin();ite!=El.end();ite++)
            {
                if(ite->unique.hasit(i1) && ite->unique.hasit(i2))
                {
                    cout << "what?" << endl;
                    exit(1);
                }
                if(ite->unique.hasit(i1))
                    ite->unique.changeto(i1, n);
                else if(ite->unique.hasit(i2))
                    ite->unique.changeto(i2, n);
                //ite's unique done
                if(ite->unique.hasit(*ita))
                {
                    ite->v = leastCostNewV( V[ite->unique.i1] , V[ite->unique.i2]);
                    matrix_double4x4 Q = matrix_add( V[ite->unique.i1].Q , V[ite->unique.i2].Q );
                    ite->cost = vector_dot(ite->v.pos4(), matrix_multiply(Q, ite->v.pos4()));
                }
                //ite's cost done
            }
        }
    }
}

vector<jlod>& jfbxcustomizer_lod::getlods()
{
    /*
    matrix_double4x4 K = makeK(simd::double3{10,0,0}, simd::double3{-93,0,1}, simd::double3{1,0,100} );
    simd::double4 tmp = simd::double4{0,2,0};
    double len = vector_dot(tmp, matrix_multiply(K, tmp));
    cout << "len : " << len << endl;
    exit(1);
     */
    if(lods.size()==0)
    {
        makebaselod();
        
    }
    return lods;
}