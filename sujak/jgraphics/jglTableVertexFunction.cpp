//
//  jglTableVertexFunction.cpp
//  sujak
//
//  Created by Ingun Jon on 29/10/2016.
//
//
#include "jdefinitions.hpp"
#include "jglTableVertexFunction.hpp"

void jglTableVertexFunction::init()
{
	jglTableNonTransientArray<jglVertexFunction*>::init(sujak::JVertexFunction_number);
}
