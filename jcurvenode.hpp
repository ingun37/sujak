//
//  jcurvenode.hpp
//  MetalVertexStreaming
//
//  Created by ingun on 4/12/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jcurvenode_hpp
#define jcurvenode_hpp

#include <stdio.h>

class jcurve;

enum JCURVENODE_PROPERTY{JCURVENODE_PROPERTY_ROTATION, JCURVENODE_PROPERTY_TRANSLATION};
class jcurvenode
{
    jcurve *curves[2][3];
public:
    inline jcurve **getcurveofproperty(JCURVENODE_PROPERTY property)
    {
        return curves[property];
    }
};

#endif /* jcurvenode_hpp */
