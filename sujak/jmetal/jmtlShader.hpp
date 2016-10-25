//
//  jmtlShader.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlShader_hpp
#define jmtlShader_hpp
#import <Metal/Metal.h>
#include "jglShader.hpp"
class jmtlShader : public jglShader
{
	id<MTLRenderPipelineState> pipelinestate;
protected:
	virtual void init(sujak::JShader p) {}
public:
	
	void init(sujak::JShader p, id<MTLDevice> device, id<MTLLibrary> library);
	jmtlShader();
};

#endif /* jmtlShader_hpp */
