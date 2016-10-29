//
//  jglFunctionVertex.hpp
//  sujak
//
//  Created by Ingun Jon on 27/10/2016.
//
//

#ifndef jglFunctionVertex_hpp
#define jglFunctionVertex_hpp
#include "jdefinitions.hpp"
class jglFunctionVertex
{
    sujak::JAttributeGroupInstance requiredAttributesInstance;
    sujak::JAttributeGroupVertex requiredAttributesVertex;
protected:
    void init(sujak::JAttributeGroupInstance requiredAttributesInstance, sujak::JAttributeGroupVertex requiredAttributesVertex);
};

#endif /* jglFunctionVertex_hpp */
