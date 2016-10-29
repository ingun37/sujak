//
//  jglTableFragmentFunction.cpp
//  sujak
//
//  Created by Ingun Jon on 30/10/2016.
//
//
#include "jdefinitions.hpp"
#include "jglTableFragmentFunction.hpp"

void jglTableFragmentFunction::init()
{
	jglTableNonTransientArray<jglFragmentFunction*>::init(sujak::JFragmentFunction_number);
}
