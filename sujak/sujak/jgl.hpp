//
//  jgl.h
//  sujak
//
//  Created by ingun on 7/20/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jgl_h
#define jgl_h
#include "jconstants.hpp"
#include "jrenderobject.hpp"
namespace sujak
{
    class jgl
    {
    public:
        virtual void init()=0;
		virtual void loadObject(JRenderContext state, jrenderobject* obj) = 0;
    };
}
#endif /* jgl_h */
