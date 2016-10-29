//
//  jglShader.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglShader_hpp
#define jglShader_hpp
#include "jdefinitions.hpp"
class jglShader
{
	sujak::JPixelFormat pixelformat_depth;
    sujak::JPixelFormat pixelformat_stencil;
protected:
	virtual void init(sujak::JShader p) = 0;
};

#endif /* jglShader_hpp */
