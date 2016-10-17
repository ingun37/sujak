//
//  jglattributebuffer.hpp
//  jgl
//
//  Created by ingun on 15/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglattributebuffer_hpp
#define jglattributebuffer_hpp

#define JGLATTRIBUTEBUFFER_MAXATT_CNT 16

class jglObjVertexDataHandle
{
public:
    void* datas[JGLATTRIBUTEBUFFER_MAXATT_CNT];
    jglObjVertexDataHandle();
};

class jglBuffer;

typedef struct _jglAttributeBufferAttributeInfo
{
	unsigned short unitsize;
	jglBuffer* buffer;
    _jglAttributeBufferAttributeInfo()
    {
        unitsize = 0;
        buffer = 0;
    }
} jglAttributeBufferAttributeInfo;


class jglAttributeBuffer
{
	int flag;
	jglAttributeBufferAttributeInfo attributes[JGLATTRIBUTEBUFFER_MAXATT_CNT];
protected:
	virtual void initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum ) = 0;
	jglAttributeBuffer();
    void init(int flag);

    
public:
        jglObjVertexDataHandle getHandleAndAdvance(unsigned int vcnt);
};


#endif /* jglattributebuffer_hpp */
