//
//  jglshaderpool.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglshaderpool_hpp
#define jglshaderpool_hpp

#include "jgltablenontransient.hpp"
#include "jdefinitions.hpp"

class jglShader;
class jglTableShader : public jglTableNonTransient<jglShader*>
{
protected:
	virtual void init() = 0;
};

#endif /* jglshaderpool_hpp */
