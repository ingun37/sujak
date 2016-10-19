//
//  jglshaderpool.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglshaderpool_hpp
#define jglshaderpool_hpp

#include "jglnontransienttable.hpp"
#include "jdefinitions.hpp"

class jglShader;
class jglShaderTable : public jglNonTransientTable<jglShader*>
{
protected:
	virtual jglShader* shaderOf(sujak::JPipeline p) = 0;
	void init();
};

#endif /* jglshaderpool_hpp */
