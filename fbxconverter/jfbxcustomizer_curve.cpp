//
//  jfbxcustomizer_curve.cpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#include "jfbxcustomizer_curve.hpp"



template <typename T>
void copyVectorTo(vector<T>& vec, T* p)
{
    memcpy(p, &vec[0], sizeof(T) * vec.size());
}

void makejcurveFromFbxcurve(jcurve& dst, FbxAnimCurve* src)
{
    dst.cnt = src->KeyGetCount();
    
    vector<JCURVEINTERPOLATION> myinterpolations;
    vector<float> mytimes;
    vector<float> myvalues;
    
    for(int j=0;j<dst.cnt;j++)
    {
        mytimes.push_back(src->KeyGetTime(j).GetSecondDouble());
        myvalues.push_back(src->KeyGetValue(j));
    }
    
    vector<simd::float2> mytangents_l;
    vector<simd::float2> mytangents_r;
    
    for(int j=0;j<dst.cnt;j++)
    {
        simd::float2 tin = {0,0};
        simd::float2 tout = {0,0};
        
        FbxAnimCurveKey key = src->KeyGet(j);
        
        if(key.GetInterpolation() == FbxAnimCurveDef::EInterpolationType::eInterpolationCubic)
        {
            myinterpolations.push_back(JCURVEINTERPOLATION_CUBIC);
            
            float tension = key.GetDataFloat(FbxAnimCurveDef::EDataIndex::eTCBTension);
            float continuity = key.GetDataFloat(FbxAnimCurveDef::EDataIndex::eTCBContinuity);
            float bias = key.GetDataFloat(FbxAnimCurveDef::EDataIndex::eTCBBias);
            
            auto relpos = [&](int idx)
            {
                int relidx = j + idx;
                if (0 > relidx || relidx >= src->KeyGetCount())
                {
                    return (simd::float2){0,0};
                }
                return (simd::float2){static_cast<float>(mytimes[relidx]), myvalues[relidx]};
            };
            
            simd::float2 prevp = relpos(-1);
            simd::float2 currp = relpos(0);
            simd::float2 nextp = relpos(1);
            
            float tcb1;
            float tcb2;
            
            if(j==0)
            {
                tout = nextp - currp;
            }
            else if(j==dst.cnt-1)
            {
                tin = currp - prevp;
            }
            else
            {
                tcb1 = (1-tension)*(1+continuity)*(1-bias)/2;
                tcb2 = (1-tension)*(1-continuity)*(1+bias)/2;
                
                tin = (tcb1*(nextp-currp))+(tcb2*(currp-prevp));
                
                tcb1 = (1-tension)*(1-continuity)*(1-bias)/2;
                tcb2 = (1-tension)*(1+continuity)*(1+bias)/2;
                
                tout = (tcb1*(nextp-currp))+(tcb2*(currp-prevp));
            }
            
            if(tin[0] < 0 || tout[0] < 0)
            {
                puts("negative x in tout and tin\n");
                exit(1);
            }
        }
        else if(key.GetInterpolation() == FbxAnimCurveDef::eInterpolationLinear)
        {
            myinterpolations.push_back(JCURVEINTERPOLATION_LINEAR);
        }
        else if(key.GetInterpolation() == FbxAnimCurveDef::eInterpolationConstant)
        {
            myinterpolations.push_back(JCURVEINTERPOLATION_CONSTANT);
        }
        mytangents_l.push_back(tin);
        mytangents_r.push_back(tout);
    }
    
    if( dst.cnt != myinterpolations.size() ||
       myinterpolations.size() != mytimes.size() ||
       mytimes.size() != myvalues.size() ||
       myvalues.size() != mytangents_l.size() ||
       mytangents_l.size() != mytangents_r.size() )
    {
        printf("len err\n");
        exit(1);
    }
    
    dst.interpolations = new JCURVEINTERPOLATION[dst.cnt];
    copyVectorTo(myinterpolations, dst.interpolations);
    
    dst.times = new float[dst.cnt];
    copyVectorTo(mytimes, dst.times);
    
    dst.values = new float[dst.cnt];
    copyVectorTo(myvalues, dst.values);
    
    dst.tangents_l = new simd::float2[dst.cnt];
    copyVectorTo(mytangents_l, dst.tangents_l);
    
    dst.tangents_r = new simd::float2[dst.cnt];
    copyVectorTo(mytangents_r, dst.tangents_r);
}

void jfbxcustomizer_curve::iterEachProperty()
{
    vector<FbxNode*> &skels = getSkels();
    
    for(int i=0;i<skels.size();i++)
    {
        FbxAnimCurve *c1=NULL, *c2=NULL, *c3=NULL;
        FbxNode* skel = skels[i];
        FbxAnimCurveNode* curvenode = skel->LclRotation.GetCurveNode();
        if(curvenode)
        {
            c1 = curvenode->GetCurve(0);
            c2 = curvenode->GetCurve(1);
            c3 = curvenode->GetCurve(2);
        }
        
        iteritemcurves(c1, c2, c3);
    }

}
void jfbxcustomizer_curve::iteritemcurves(FbxAnimCurve *c1, FbxAnimCurve *c2, FbxAnimCurve *c3)
{
    jcurve *j1 = new jcurve(), *j2 = new jcurve(), *j3 = new jcurve();
    makejcurveFromFbxcurve(*j1, c1);
    makejcurveFromFbxcurve(*j2, c2);
    makejcurveFromFbxcurve(*j3, c3);
 
    jcurvenode* curvenode = new jcurvenode();
    curvenode->curves[0] = j1;
    curvenode->curves[1] = j2;
    curvenode->curves[2] = j3;
    
    curvenodes.push_back(curvenode);
}

vector<jcurvenode*>& jfbxcustomizer_curve::getCurvenodes()
{
    if(curvenodes.size()==0)
        iterEachProperty();
    return curvenodes;
}
