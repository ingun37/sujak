//
//  jmtlShadertable.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlShadertable_hpp
#define jmtlShadertable_hpp
#import <Metal/Metal.h>
#include "jglTableShader.hpp"

class jmtlTableShader : public jglTableShader
{
	id<MTLDevice> device;
	id<MTLLibrary> library;
protected:
	virtual jglShader* makeOf(int key);
public:
	void init(id<MTLDevice> device);
};

#endif /* jmtlShadertable_hpp */
