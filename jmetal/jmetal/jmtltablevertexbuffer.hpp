//
//  jmtltablevertexbuffer.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtltablevertexbuffer_hpp
#define jmtltablevertexbuffer_hpp

#import <Metal/Metal.h>
#include "jglnontransienttable.hpp"

class jmtlVertexBuffer;
class jglTableVertexBuffer : jglNonTransientTable<jmtlVertexBuffer*>
{
	id<MTLDevice> device;
	virtual jmtlVertexBuffer* makeOf(int k);
public:
	void init(id<MTLDevice> device);
};

#endif /* jmtltablevertexbuffer_hpp */
