//
//  jmtlshadertable.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlshadertable_hpp
#define jmtlshadertable_hpp
#import <Metal/Metal.h>
#include "jglshadertable.hpp"

class jmtlShaderTable : public jglShaderTable
{
	id<MTLDevice> device;
	id<MTLLibrary> library;
protected:
	virtual jglShader* makeOf(int key);
public:
	void init(id<MTLDevice> device);
};

#endif /* jmtlshadertable_hpp */
