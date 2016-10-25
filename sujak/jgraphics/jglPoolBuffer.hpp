//
//  jglPoolBuffer.hpp
//  sujak
//
//  Created by Ingun Jon on 25/10/2016.
//
//

#ifndef jglPoolBuffer_hpp
#define jglPoolBuffer_hpp
#include "jdefinitions.hpp"

class jglBuffer;
class jglPoolBuffer
{
public:
    static jglPoolBuffer* pool;
    virtual jglBuffer* aInitedBuffer(unsigned int reserveSize, sujak::JMemoryOption option) = 0;
};

#endif /* jglPoolBuffer_hpp */
