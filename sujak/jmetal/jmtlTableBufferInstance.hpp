//
//  jmtlBufferInstancetable.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlBufferInstancetable_hpp
#define jmtlBufferInstancetable_hpp
#import <Metal/Metal.h>
#include "jglTableNonTransientMap.hpp"
class jmtlBufferInstance;
class jmtlTableBufferInstance : public jglTableNonTransientMap<jmtlBufferInstance*>
{
	id<MTLDevice> device;
public:
	void init(id<MTLDevice> device);
	virtual jmtlBufferInstance* makeOf(int k);
};

#endif /* jmtlBufferInstancetable_hpp */
