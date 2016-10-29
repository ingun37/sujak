//
//  jglVertexFunction.hpp
//  sujak
//
//  Created by Ingun Jon on 27/10/2016.
//
//

#ifndef jglVertexFunction_hpp
#define jglVertexFunction_hpp
#include "jdefinitions.hpp"
class jglVertexFunction
{
    sujak::JAttributeGroupVertex requiredAttributesVertex;
    sujak::JAttributeGroupInstance requiredAttributesInstance;
    
protected:
    jglVertexFunction(){};
    void init(sujak::JAttributeGroupInstance requiredAttributesInstance, sujak::JAttributeGroupVertex requiredAttributesVertex)
    {
        this->requiredAttributesVertex = requiredAttributesVertex;
        this->requiredAttributesInstance = requiredAttributesInstance;
    }
};

#endif /* jglVertexFunction_hpp */
