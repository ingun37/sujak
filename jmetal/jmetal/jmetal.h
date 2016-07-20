//
//  jmetal.h
//  jmetal
//
//  Created by ingun on 7/20/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "jgl.h"
class jmetal : sujak::jgl
{
    id <MTLRenderCommandEncoder> renderEncoder;
    id <MTLBuffer> indexBuffer;
    MTLPrimitiveType primitiveTypeCurrent;
public:
    jmetal();
    virtual void preRender();
    virtual void setRenderState(sujak::JRenderState state);
    virtual void setPrimitive(sujak::JRenderPrimitive prim);
    virtual void renderIndexed(unsigned long offset, unsigned long cnt);
    
    Class layerClass();
};