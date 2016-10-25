//
//  jglTypedBuffer.hpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//

#ifndef jglTypedBuffer_hpp
#define jglTypedBuffer_hpp

class jglBuffer;
class jglTypedBuffer
{
	unsigned int typeSize;
	jglBuffer* buffer;
	unsigned int unitCnt;
protected:
	void init(unsigned int typeSize, jglBuffer* buffer);
	void append(const void* src, unsigned int unitCnt);
	jglTypedBuffer();
};

#endif /* jglTypedBuffer_hpp */
