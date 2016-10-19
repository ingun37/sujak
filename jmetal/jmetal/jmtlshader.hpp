//
//  jmtlshader.hpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jmtlshader_hpp
#define jmtlshader_hpp
#import <Metal/Metal.h>
#include "jglshader.hpp"
class jmtlShader : public jglShader
{
	id<MTLRenderPipelineState> pipelinestate;
protected:
	
public:
	void init(sujak::JPipeline p, id<MTLDevice> device, id<MTLLibrary> library);
	jmtlShader();
};

#endif /* jmtlshader_hpp */
