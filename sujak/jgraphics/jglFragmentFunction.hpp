//
//  jglFragmentFunction.hpp
//  sujak
//
//  Created by Ingun Jon on 30/10/2016.
//
//

#ifndef jglFragmentFunction_hpp
#define jglFragmentFunction_hpp
#include "jdefinitions.hpp"
class jglFragmentFunction
{
	sujak::JPixelFormat outputFormat;
	
protected:
	jglFragmentFunction(){};
	void init(sujak::JPixelFormat outputFormat)
	{
		this->outputFormat = outputFormat;
	}
};


#endif /* jglFragmentFunction_hpp */
