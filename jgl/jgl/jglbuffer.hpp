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
public:
	jglBuffer();
	void append(void* data, unsigned int len);
	void reset();
	virtual void* handle() = 0;
};

#endif /* jglbuffer_h */
