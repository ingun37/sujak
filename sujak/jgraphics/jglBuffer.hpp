//
//  jglBuffer.h
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglBuffer_h
#define jglBuffer_h

class jglBuffer
{
	unsigned int pointer;
	unsigned int reservedSize;
protected:
    virtual void* handle() = 0;
    void init(unsigned int reserve);
    jglBuffer();
public:
    void* handleAtPointer();
	unsigned int writeAndAdvance(const void* src, unsigned int len);//return offset in bytes
	void reset();
	
};

#endif /* jglBuffer_h */
