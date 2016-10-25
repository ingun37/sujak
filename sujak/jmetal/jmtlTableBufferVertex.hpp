//
//  jmtlTableBufferVertex.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtltablebuffervertex_hpp
#define jmtltablebuffervertex_hpp

#import <Metal/Metal.h>
#include "jglTableNonTransientMap.hpp"

class jmtlBufferVertex;
class jglTableBufferVertex : jglTableNonTransientMap<jmtlBufferVertex*>
{
	id<MTLDevice> device;
	virtual jmtlBufferVertex* makeOf(int k);
public:
	void init(id<MTLDevice> device);
};

#endif /* jmtltablebuffervertex_hpp */
