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
class jglTexture;
class jglPool
{
public:
    static jglPool* pool;
    virtual jglBuffer* aInitedBuffer(unsigned int reserveSize, sujak::JMemoryOption option) = 0;
	virtual jglTexture* aInitedTexture(unsigned int width, unsigned int height, sujak::JPixelFormat f) = 0;
};

#endif /* jglPoolBuffer_hpp */
