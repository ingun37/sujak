//
//  jglTableFragmentFunction.hpp
//  sujak
//
//  Created by Ingun Jon on 30/10/2016.
//
//

#ifndef jglTableFragmentFunction_hpp
#define jglTableFragmentFunction_hpp

#include <stdio.h>

#include "jglTableNonTransientArray.hpp"
class jglFragmentFunction;
class jglTableFragmentFunction : public jglTableNonTransientArray<jglFragmentFunction*>
{
	void init();
};

#endif /* jglTableFragmentFunction_hpp */
