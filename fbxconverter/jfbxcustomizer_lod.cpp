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
#include <string>
#include <map>
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
    
    void CWtoCCWorViceVersa()
    {
        int tmp = i2;
        i2 = i3;
        i3 = tmp;
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

class jllink
{
public:
    int jointidx;
    float weight;
    jllink(int a, float b)
    {
        jointidx = a;
        weight = b;
    }
};
class jlcluster
{
public:
    vector<jllink> links;
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
    {//TODO : leastcost
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

const simd::double4 empty4 = simd::double4{0,0,0,0};
const matrix_double4x4 mEmpty{empty4, empty4, empty4, empty4};


typedef list<jlpoly> listP;
void makeP(listP& P, const vector<int>& indices, const vector<jvertex>& vertices)
{
    for(int i=0;i<indices.size();i+=3)
    {
        jlpoly p;
        p.i1 = indices[i+0];
        p.i2 = indices[i+1];
        p.i3 = indices[i+2];
        p.K = makeK(vertices[p.i1].pos, vertices[p.i2].pos, vertices[p.i3].pos);
        
        P.push_back(p);
    }
}
typedef vector<jlvertex> vecV;
void makeV(vecV& V, const vector<jvertex>& vertices, const listP& P)
{
    if(V.size() != 0)
    {
        cout << "gimme empty V" << endl;
        exit(1);
    }
    for(int i=0;i<vertices.size();i++)
    {
        jlvertex v;
        v.Q = mEmpty;
        v.v = vertices[i];
        V.push_back(v);
    }
    if(V.size()%3 != 0)
    {
        cout << "V size must be multi of 3. " << V.size() << endl;
    }
    
    for(listP::const_iterator it = P.begin() ; it!= P.end() ; it++)
    {
        V[it->i1].Q = matrix_add(V[it->i1].Q, it->K);
        V[it->i2].Q = matrix_add(V[it->i2].Q, it->K);
        V[it->i3].Q = matrix_add(V[it->i3].Q, it->K);
    }
}
typedef list<jledge> listE;
void makeE(listE &E, const listP &P, const vecV& V)
{
    set<jledge_unique> Es;
    set<jledge_unique> Edup;
    
    for(listP::const_iterator p = P.begin() ; p != P.end() ; p++)
    {
        jledge_unique edges[3];
        
        edges[0].i1 = p->i1;
        edges[0].i2 = p->i2;
        
        edges[1].i1 = p->i2;
        edges[1].i2 = p->i3;
        
        edges[2].i1 = p->i3;
        edges[2].i2 = p->i1;
        
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
    
    for(set<jledge_unique>::iterator e = Eedge.begin();e!=Eedge.end();e++)
    {
        Ve.insert(e->i1);
        Ve.insert(e->i2);
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
    
    for(set<jledge_unique>::iterator u = Es.begin();u!=Es.end();u++)
    {
        jledge e;
        e.unique = *u;
        
        const jlvertex &v1 = V[e.unique.i1];
        const jlvertex &v2 = V[e.unique.i2];
        matrix_double4x4 Q = matrix_add( v1.Q, v2.Q );
        e.v = leastCostNewV(v1, v2);
        e.cost = vector_dot(e.v.pos4(), matrix_multiply(Q, e.v.pos4()));
        E.push_back(e);
    }
    
}

typedef vector<jlcluster> vecJ;
void makeJ(vecJ& J, const vector<jskinjointinfo>& src, const size_t vlen )
{
    if(J.size() != 0)
        throw "Johnson size";
    for(int i=0;i<vlen;i++)
        J.push_back( jlcluster() );
    for(int i=0;i<src.size();i++)
        for(int j=0;j<src[i].cpinfos.size();j++)
            J[ src[i].cpinfos[j].idx ].links.push_back( jllink( i, src[i].cpinfos[j].weight ));
    
    for(int i=0;i<J.size();i++)
        if(J[i].links.size() == 0)
            throw "jiqiwerhisehfisefsef";
}

float totalPerJoint( const vector<jllink>& links, map<int, float> &wmap )
{
    float total = 0;
    for(int i=0;i<links.size();i++)
    {
        int ji = links[i].jointidx;
        float w = links[i].weight;
        
        if(wmap.find(ji) == wmap.end())
            wmap[ji] = 0;
        
        wmap[ji] = wmap[ji] + w;
        
        total += w;
    }
    return total;
}

void addNthTojlcluster(vecJ& J, int v1, int v2 )
{
    float total = 0;
    map<int, float> wmap;
    total += totalPerJoint(J[v1].links, wmap);
    total += totalPerJoint(J[v2].links, wmap);
    
    J.push_back(jlcluster());
    
    jlcluster &nth = J[ J.size() - 1 ];
    
    float test = 0;
    for( map<int, float>::iterator it = wmap.begin();it!=wmap.end();it++)
    {
        test += it->second;
        jllink l(it->first, it->second/total);
        nth.links.push_back(l);
    }
    
    if(abs(test - total) > 0.00001)
        throw "bro bro bro bro ssup ssup ssup";
}

void filterUnusedVertices(vector<jskinjointinfo>& joints, vector<jvertex>& vertices, vector<int>& indices, const vector<jskinjointinfo>& skins, const vecJ &J, const vecV &V, const listP& P)
{
    set<int> usage;
    vector<int> table;
    
    if(vertices.size() != 0)
        throw "ssup bro watup";
    
    for(int i=0;i<V.size();i++)
    {
        table.push_back(-1);
    }
    
    for(listP::const_iterator p = P.begin();p!=P.end();p++)
    {
        usage.insert(p->i1);
        usage.insert(p->i2);
        usage.insert(p->i3);
        
        if(p->i1 >= V.size() || p->i2 >= V.size() || p->i3 >= V.size())
            throw "chill man";
    }
    
    for(int i=0;i<skins.size();i++)
    {
        joints.push_back(jskinjointinfo());
        
        joints[i].bindmesh = skins[i].bindmesh;
        joints[i].inverse = skins[i].inverse;
        joints[i].jointidx = skins[i].jointidx;
    }
    
    int cnt=0;
    int testidx=-1;
    for(set<int>::iterator it = usage.begin();it!=usage.end();it++)
    {
        if(cnt==1820)
        {
            testidx = *it;
            cout << endl <<"testidx original : " << testidx << endl;
        }
        table[*it] = cnt++;
        vertices.push_back( V[*it].v );
        if(vertices.size() != cnt)
            throw "na bro";
        
        for(int i=0;i<J[*it].links.size();i++)
        {
            const jllink& l = J[*it].links[i];
            jskincpinfo cp;
            cp.idx = table[*it];
            cp.weight = l.weight;
            joints[ l.jointidx ].cpinfos.push_back(cp);
        }
    }
    
    if(table[ V.size()-1 ] == -1)
        throw "impossible!! iashefi";
    
    for(listP::const_iterator it = P.begin();it!=P.end();it++)
    {
        if(table[it->i1] == -1 || table[it->i2] == -1 || table[it->i3] == -1)
            throw "watup man u broke ass nigga";
        indices.push_back( table[it->i1] );
        indices.push_back( table[it->i2] );
        indices.push_back( table[it->i3] );
    }
    
    
    
    //test
    if(joints.size() != skins.size())
        throw "sqqqqsqqq";
    for(int i=0;i<joints.size();i++)
        cout << "joint " << i << " cp diff : " << skins[i].cpinfos.size() - joints[i].cpinfos.size() << endl;
    


    vector<float> testvec;
    for(int i=0;i<vertices.size();i++)
        testvec.push_back(0);
    for(int i=0;i<joints.size();i++)
        for(int j=0;j<joints[i].cpinfos.size();j++)
            testvec[ joints[i].cpinfos[j].idx ] += joints[i].cpinfos[j].weight;
    
    for(int i=0;i<vertices.size();i++)
        if(abs(testvec[i] - 1) > 0.001)
            throw "no good no good nog oood no gooood";
    
    
    //end
}



void jfbxcustomizer_lod::lodlast()
{
    const jlod &orig = lods[lods.size()-1];

    listP P;
    makeP(P, orig.indices, orig.vertices);
    
    vecV V;
    makeV(V, orig.vertices, P);
    
    listE E;
    makeE(E, P, V);
    
    vecJ J;
    makeJ(J, orig.joints, V.size());
    
    int cnt = 3;
    while(cnt-- != 0)
    {
        E.sort();
        
        int n = static_cast<int>( V.size() );
        V.push_back(jlvertex());
        vector<jlvertex>::iterator itv = V.end()-1;
        
        itv->v = E.front().v;
        int i1 = E.front().unique.i1;
        int i2 = E.front().unique.i2;
        
        E.pop_front();
        
        addNthTojlcluster(J, i1, i2);
        
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
            
            simd::double3 before = vector_cross( V[itp->i2].v.pos - V[itp->i1].v.pos, V[itp->i3].v.pos - V[itp->i1].v.pos);
            
            if(itp->hasit(i1))
            {
                itp->changeto(i1, n);
            }
            else if(itp->hasit(i2))
            {
                itp->changeto(i2, n);
            }
            
            simd::double3 after1 = vector_cross( V[itp->i2].v.pos - V[itp->i1].v.pos, V[itp->i3].v.pos - V[itp->i1].v.pos);
            simd::double3 after2 = vector_cross( V[itp->i3].v.pos - V[itp->i1].v.pos, V[itp->i2].v.pos - V[itp->i1].v.pos);
            if (vector_dot(before, after1) < vector_dot(before, after2))
            {
                cout << "clock wise correction" << endl;
                itp->CWtoCCWorViceVersa();
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
        
        cout << adjK.size() << ":" << adjV.size() << ", ";
        
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
            matrix_double4x4 test2 = matrix_sub(test1, test);
            
            for(int ri = 0;ri<4;ri++)
                for(int ci=0;ci<4;ci++)
                    if(abs(test2.columns[ci][ri]) > 0.0001)
                        throw "man u fucked up. getur headoutofurass";
        }
        //V done
        for(set<int>::iterator ita = adjV.begin();ita!=adjV.end();ita++)
        {
            for(list<jledge>::iterator ite = E.begin();ite!=E.end();ite++)
            {
                if(ite->unique.hasit(i1) && ite->unique.hasit(i2))
                    throw "u mad bro";
                
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
    
    lods.push_back(jlod());
    jlod& nextlod =  lods[ lods.size() - 1 ];
    const jlod& prevlod = lods[lods.size()-2];
    
    filterUnusedVertices(nextlod.joints, nextlod.vertices, nextlod.indices, prevlod.joints, J, V, P);
    
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