//
//  jglattributebuffer.hpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglattributebuffer_hpp
#define jglattributebuffer_hpp

#define JGLATTRIBUTEBUFFER_MAXATT_CNT 32

class jglbuffer;

typedef struct _jglAttributeBufferAttributeInfo
{
	unsigned short unitsize;
	jglbuffer* buffer;
} jglAttributeBufferAttributeInfo;


class jglAttributeBuffer
{
	int flag;
	unsigned int cnt;
	jglAttributeBufferAttributeInfo attributes[JGLATTRIBUTEBUFFER_MAXATT_CNT];
protected:
	virtual void initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum ) = 0;
	jglAttributeBuffer(int flag, unsigned int cnt);
};

#endif /* jglattributebuffer_hpp */
