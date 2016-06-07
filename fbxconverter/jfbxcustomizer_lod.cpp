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

typedef struct _jlvertex
{
    matrix_double4x4 Q;
    jvertex v;
    vector<jllink> links;
    
}jlvertex;

class jledge_unique
{
public:
    jledge_unique(){}
    jledge_unique(int a, int b)
    {
        i1 = a;
        i2 = b;
    }
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
                return lesser() < e.lesser() ? 1 : -1;
        else
            return greater() < e.greater() ? 1 : -1;
        
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

    bool issame(const jlpoly& p) const
    {
        if(hasit(p.i1) && hasit(p.i2) && hasit(p.i3))
            return true;
        return false;
    }
    bool hasit(int a) const
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
    
    bool hasboth(int a, int b) const
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
    
    int other(int a, int b)
    {
        if(i1 != a && i1 != b)
            return i1;
        if(i2 != a && i2 != b)
            return i2;
        if(i3 != a && i3 != b)
            return i3;
        throw "aefefefefefefefwww";
    }
    
    int checkDuplicate() const
    {
        if(i1 == i2) return i1;
        if(i2 == i3) return i2;
        if(i3 == i1) return i3;
        return -1;
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
void makeV(vecV& V, const vector<jvertex>& vertices, const listP& P, const vector<jskinjointinfo>& src)
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
    
    for(int i=0;i<src.size();i++)
        for(int j=0;j<src[i].cpinfos.size();j++)
            V[ src[i].cpinfos[j].idx ].links.push_back( jllink( src[i].jointidx, src[i].cpinfos[j].weight ));
    
    for(int i=0;i<V.size();i++)
        if(V[i].links.size() == 0)
            throw "jiqiwerhisehfisefsef";
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

void addNthV(vecV& V, const jvertex& newv, int i1, int i2)
{
    vecV::iterator v = V.insert(V.end(), jlvertex());
    v->v = newv;
    
    float total = 0;
    map<int, float> wmap;
    total += totalPerJoint(V[i1].links, wmap);
    total += totalPerJoint(V[i2].links, wmap);
    
    float test = 0;
    for( map<int, float>::iterator it = wmap.begin();it!=wmap.end();it++)
    {
        test += it->second;
        jllink l(it->first, it->second/total);
        v->links.push_back(l);
    }
    
    if(abs(test - total) > 0.00001)
        throw "bro bro bro bro ssup ssup ssup";
}

void filterUnusedVertices(vector<jskinjointinfo>& joints, vector<jvertex>& vertices, vector<int>& indices, const vector<jskinjointinfo>& skins, const vecV &V, const listP& P)
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
        
        for(int i=0 ; i<V[*it].links.size();i++)
        {
            const jllink& l = V[*it].links[i];
            jskincpinfo cp;
            cp.idx = table[*it];
            cp.weight = l.weight;
            
            jskinjointinfo* thejoint = NULL;
            for(int j=0;j<joints.size();j++)
                if(joints[j].jointidx == l.jointidx)
                {
                    if(thejoint != NULL) throw "watup ig";
                    thejoint = &joints[j];
                    break;
                }
            if(thejoint == NULL) throw "sdsdfsdfsdfsfiii";
            thejoint->cpinfos.push_back(cp);
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

void refreshE(listE& E, const vecV& V, const listP& P, const int i1, const int i2, const int n)
{
    set<jledge_unique> changedEs;
    int erasedEdgeCnt = 0;
    for(listE::iterator e = E.begin();e!=E.end();)
    {
        bool has1 = e->unique.hasit(i1);
        bool has2 = e->unique.hasit(i2);
        
        if(has1 && has2)
            throw "has has has";
        else if(has1 || has2)
        {
                
            if(has1)
                e->unique.changeto(i1, n);
            else
                e->unique.changeto(i2, n);
                
            auto result = changedEs.insert(e->unique);
            if(!result.second)
            {
                e = E.erase(e);
                erasedEdgeCnt++;
                continue;
            }
            
            e->v = leastCostNewV( V[e->unique.i1] , V[e->unique.i2]);
            matrix_double4x4 Q = matrix_add( V[e->unique.i1].Q , V[e->unique.i2].Q );
            e->cost = vector_dot(e->v.pos4(), matrix_multiply(Q, e->v.pos4()));
        }
        e++;
    }
    if(erasedEdgeCnt != 2)
        cout << "erased Edge Cnt : " << erasedEdgeCnt << endl;
    set<jledge_unique> orphans;
    
    for(set<jledge_unique>::iterator e=changedEs.begin();e!=changedEs.end();e++)
    {
        bool remove = true;
        for(listP::const_iterator p=P.begin();p!=P.end();p++)
        {
            if(p->hasboth(e->i1, e->i2))
            {
                remove = false;
                break;
            }
        }
        
        if(remove)
            orphans.insert(*e);
    }
    
    if(orphans.size() > 0)
    {
        cout << "orhans size : " << orphans.size() << endl;
        for(listE::iterator e=E.begin() ; e!= E.end();)
        {
            if(orphans.find(e->unique) != orphans.end())
            {
                cout << "orphan removed, ";
                e = E.erase(e);
                continue;
            }
            e++;
        }
    }
    
}

bool shouldDeleteP( const jlpoly& p, const vector<jlpoly> &changedPolys, const int n )
{
    int dupv = p.checkDuplicate();
    if(dupv != -1)
    {
        if(dupv != n)
            throw "nn nn nn nn nn ";
        
        return true;
    }
    
    for(vector<jlpoly>::const_iterator it = changedPolys.begin();it!=changedPolys.end();it++)
        if(it->issame(p))
        {
            cout << "found a dup poly : " << p.i1 << "," << p.i2 << "," << p.i3 << endl;
            return true;
        }
    
    return false;
}

simd::double3 getCross_23_normed(const vecV& V, const jlpoly& p)
{
    simd::double3 cross = vector_cross( V[p.i2].v.pos - V[p.i1].v.pos, V[p.i3].v.pos - V[p.i1].v.pos);
    return vector_normalize(cross);
}

simd::double3 getCross_32_normed(const vecV& V, const jlpoly& p)
{
    simd::double3 cross = vector_cross( V[p.i3].v.pos - V[p.i1].v.pos, V[p.i2].v.pos - V[p.i1].v.pos);
    return vector_normalize(cross);
}

bool checkFlipped( const jlpoly& prev, const jlpoly& curr, const vecV &V)
{
    simd::double3 before = getCross_23_normed(V, prev);
    
    simd::double3 after1 = getCross_23_normed(V, curr);
    simd::double3 after2 = getCross_32_normed(V, curr);
    
    if (vector_dot(before, after1) < vector_dot(before, after2))
        return true;
    
    return false;
}

void jfbxcustomizer_lod::lodlast()
{
    const jlod &orig = lods[lods.size()-1];

    listP P;
    makeP(P, orig.indices, orig.vertices);
    
    vecV V;
    makeV(V, orig.vertices, P, orig.joints);
    
    listE E;
    makeE(E, P, V);

    
    int cnt = 600;
    while(cnt-- != 0)
    {
        if(E.size() < 40)
            throw "toomany";
        E.sort();
    
        int n = static_cast<int>( V.size() );
        int i1 = E.front().unique.i1;
        int i2 = E.front().unique.i2;
        
        addNthV(V, E.front().v, i1, i2);
        
        E.pop_front();
        
        vector<matrix_double4x4> adjK;
        set<int> adjV;
        
        listP::iterator itp = P.begin();
        {
            vector<jlpoly> changedPolys;
            while(itp != P.end())
            {
                if(itp->hasit(i1) || itp->hasit(i2))
                {
                    jlpoly before = *itp;
                    
                    if(itp->hasit(i1))
                        itp->changeto(i1, n);

                    if(itp->hasit(i2))
                        itp->changeto(i2, n);

                    if(itp->hasit(n))
                    {
                        if(shouldDeleteP(*itp, changedPolys, n))
                        {
                            itp = P.erase(itp);
                            continue;
                        }
                        if (checkFlipped(before, *itp, V))
                        {
                            cout << "clock wise correction" << endl;
                            itp->CWtoCCWorViceVersa();
                        }
                    
                        itp->K = makeK(V[itp->i1].v.pos, V[itp->i2].v.pos, V[itp->i3].v.pos);
                        adjK.push_back(itp->K);
                        
                        changedPolys.push_back(*itp);
                    }
                    else
                        throw "wtf mansfjeisijefse";
                    
                    adjV.insert(itp->i1);
                    adjV.insert(itp->i2);
                    adjV.insert(itp->i3);
                }
                itp++;
            }
        }//changed Polys end
        if(adjK.size()+1 != adjV.size())
            cout << "a poly apoly apoly" << endl;
        
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
        //V.Q done
        
        {
            matrix_double4x4 test2 = matrix_sub(V[n].Q, test);
            
            for(int ri = 0;ri<4;ri++)
                for(int ci=0;ci<4;ci++)
                    if(abs(test2.columns[ci][ri]) > 0.0001)
                        throw "man u fucked up";
        }
        refreshE(E, V, P, i1, i2, n);
    }
    
    lods.push_back(jlod());
    jlod& nextlod =  lods[ lods.size() - 1 ];
    const jlod& prevlod = lods[lods.size()-2];
    
    filterUnusedVertices(nextlod.joints, nextlod.vertices, nextlod.indices, prevlod.joints, V, P);
}

vector<jlod>& jfbxcustomizer_lod::getlods()
{
    if(lods.size()==0)
    {
        makebaselod();
    }
    return lods;
}