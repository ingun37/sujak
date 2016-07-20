//
//  jgl.h
//  sujak
//
//  Created by ingun on 7/20/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jgl_h
#define jgl_h
#include "jconstants.h"
namespace sujak
{
    class jgl
    {
    public:
        virtual void preRender()=0;
        virtual void setRenderState(JRenderState state)=0;
        virtual void setPrimitive(JRenderPrimitive prim)=0;
        virtual void renderIndexed(unsigned long offset, unsigned long cnt)=0;
    };
}
#endif /* jgl_h */
