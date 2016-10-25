//
//  jglShaderpool.hpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglShaderpool_hpp
#define jglShaderpool_hpp

#include "jglTableNonTransientArray.hpp"
#include "jdefinitions.hpp"

class jglShader;
class jglTableShader : public jglTableNonTransientArray<jglShader*>
{
protected:
    void init();
    virtual jglShader* shaderOf(int vfunc, int ffunc) = 0;
	virtual jglShader* makeOf(unsigned int k);
};

#endif /* jglShaderpool_hpp */
