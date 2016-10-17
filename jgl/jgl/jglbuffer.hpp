//
//  jglbuffer.h
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglbuffer_h
#define jglbuffer_h

class jglBuffer
{
	unsigned int len;
protected:
    virtual void* handle() = 0;
    void init(unsigned int reserve);
    jglBuffer();
public:
    void* advancedHandle();
	void advance(unsigned int len);
	void reset();
	
};

#endif /* jglbuffer_h */
