//
//  jmtlinstancebuffertable.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlinstancebuffertable_hpp
#define jmtlinstancebuffertable_hpp
#import <Metal/Metal.h>
#include "jgltablenontransient.hpp"
class jmtlInstanceBuffer;
class jmtlTableInstanceBuffer : public jglTableNonTransient<jmtlInstanceBuffer*>
{
	id<MTLDevice> device;
public:
	void init(id<MTLDevice> device);
	virtual jmtlInstanceBuffer* makeOf(int k);
};

#endif /* jmtlinstancebuffertable_hpp */
