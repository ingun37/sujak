//
//  jglShaderpool.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglShaderpool_hpp
#define jglShaderpool_hpp

#include "jglTableNonTransient.hpp"
#include "jdefinitions.hpp"

class jglShader;
class jglTableShader : public jglTableNonTransient<jglShader*>
{
protected:
	virtual void init() = 0;
};

#endif /* jglShaderpool_hpp */
