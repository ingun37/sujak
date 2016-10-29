//
//  jglTypedBuffer.hpp
//  sujak
//
//  Created by Ingun Jon on 29/10/2016.
//
//

#ifndef jglTypedBuffer_hpp
#define jglTypedBuffer_hpp


class jglBuffer;

class jglTypedBuffer
{
    jglBuffer* buffer;
    unsigned int typeSize;
    unsigned int reservedCnt;
public:
    
    void init(unsigned int typeSize, unsigned int reservedCnt);
};


#endif /* jglTypedBuffer_hpp */
