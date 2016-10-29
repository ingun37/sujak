//
//  jglTableVertexFunction.hpp
//  sujak
//
//  Created by Ingun Jon on 29/10/2016.
//
//

#ifndef jglTableVertexFunction_hpp
#define jglTableVertexFunction_hpp
#include "jglTableNonTransientArray.hpp"
class jglVertexFunction;
class jglTableVertexFunction : public jglTableNonTransientArray<jglVertexFunction*>
{
	void init();
};

#endif /* jglTableVertexFunction_hpp */
