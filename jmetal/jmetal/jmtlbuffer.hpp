//
//  jmetalbuffer.h
//  jmetal
//
//  Created by ingun on 9/6/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmetalbuffer_h
#define jmetalbuffer_h

#import <Metal/Metal.h>
#include "jglbuffer.hpp"

class jmtlBuffer : public jglBuffer
{
    id<MTLBuffer> buffer;
protected:
	virtual void* handle();
public:
    void init(unsigned int reserve, id<MTLDevice> device, MTLResourceOptions option);
};

#endif /* jmetalbuffer_h */
