//
//  jglTableTypedBuffer.hpp
//  sujak
//
//  Created by ingun on 25/10/2016.
//
//

#ifndef jglTableTypedBuffer_hpp
#define jglTableTypedBuffer_hpp

#include "jglTableNonTransientArray.hpp"
class jglTypedBuffer;
class jglTableTypedBuffer : jglTableNonTransientArray<jglTypedBuffer*>
{
protected:
	void init();
	virtual jglTypedBuffer* makeOf(unsigned int k);
    
    virtual unsigned int typeSizeOf() = 0;
	
public:
	
};

#endif /* jglTableTypedBuffer_hpp */
