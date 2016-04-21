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
    if(src == NULL)
        dst.cnt = 0;
    else
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
        FbxAnimCurve *crx=NULL, *cry=NULL, *crz=NULL, *ctx=NULL, *cty=NULL, *ctz=NULL;
        FbxNode* skel = skels[i];
        FbxAnimCurveNode* curvenode = skel->LclRotation.GetCurveNode();
        if(curvenode)
        {
            crx = curvenode->GetCurve(0);
            cry = curvenode->GetCurve(1);
            crz = curvenode->GetCurve(2);
        }
        
        curvenode = skel->LclTranslation.GetCurveNode();
        if(curvenode)
        {
            ctx = curvenode->GetCurve(0);
            cty = curvenode->GetCurve(1);
            ctz = curvenode->GetCurve(2);
        }
        
        iteritemcurves( crx, cry, crz, ctx, cty, ctz);
    }

}
void jfbxcustomizer_curve::iteritemcurves(FbxAnimCurve* crx,FbxAnimCurve* cry,FbxAnimCurve* crz, FbxAnimCurve* ctx,FbxAnimCurve* cty,FbxAnimCurve* ctz)
{
    jcurve *jrx = new jcurve(), *jry = new jcurve(), *jrz = new jcurve(), *jtx = new jcurve(), *jty = new jcurve(), *jtz = new jcurve();
    makejcurveFromFbxcurve(*jrx, crx);
    makejcurveFromFbxcurve(*jry, cry);
    makejcurveFromFbxcurve(*jrz, crz);
    makejcurveFromFbxcurve(*jtx, ctx);
    makejcurveFromFbxcurve(*jty, cty);
    makejcurveFromFbxcurve(*jtz, ctz);
 
    jcurvenode* curvenode = new jcurvenode();
    
    curvenode->getcurveofproperty(JCURVENODE_PROPERTY_ROTATION)[0] = jrx;
    curvenode->getcurveofproperty(JCURVENODE_PROPERTY_ROTATION)[1] = jry;
    curvenode->getcurveofproperty(JCURVENODE_PROPERTY_ROTATION)[2] = jrz;
    
    curvenode->getcurveofproperty(JCURVENODE_PROPERTY_TRANSLATION)[0] = jtx;
    curvenode->getcurveofproperty(JCURVENODE_PROPERTY_TRANSLATION)[1] = jty;
    curvenode->getcurveofproperty(JCURVENODE_PROPERTY_TRANSLATION)[2] = jtz;
    
    curvenodes.push_back(curvenode);
}

vector<jcurvenode*>& jfbxcustomizer_curve::getCurvenodes()
{
    if(curvenodes.size()==0)
        iterEachProperty();
    return curvenodes;
}
