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

typedef struct _jcurvenode
{
    jcurve *curves_r[3];
    jcurve *curves_t[3];
} jcurvenode;

#endif /* jcurvenode_hpp */
