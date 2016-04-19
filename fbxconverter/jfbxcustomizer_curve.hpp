//
//  jfbxcustomizer_curve.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/19/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jfbxcustomizer_curve_hpp
#define jfbxcustomizer_curve_hpp

#include <stdio.h>
#include <vector>
#include "fbxsdk.h"
#include "jfbxcustomizer_joint.hpp"
#include "jcurve.hpp"
#include "jcurvenode.hpp"
using namespace std;


class jfbxcustomizer_curve : public jfbxcustomizer_joint
{
    void iterEachProperty();
    void iteritemcurves(FbxAnimCurve* c1,FbxAnimCurve* c2,FbxAnimCurve* c3);
    
    vector<jcurvenode*> curvenodes;
    
public:
    jfbxcustomizer_curve(FbxNode* rootskel) : jfbxcustomizer_joint(rootskel){};
    vector<jcurvenode*>& getCurvenodes();
};
#endif /* jfbxcustomizer_curve_hpp */
