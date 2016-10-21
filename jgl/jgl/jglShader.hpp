//
//  jglShader.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglShader_hpp
#define jglShader_hpp
#include "jconstants.hpp"
class jglShader
{
	sujak::JPipeline p;
protected:
	virtual void init(sujak::JPipeline p) = 0;
};

#endif /* jglShader_hpp */
