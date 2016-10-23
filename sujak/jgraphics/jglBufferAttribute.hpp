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

typedef struct _jglAttributeData
{
	int attribute;
	const void* src;
} jglAttributeData;


class jglBufferAttribute
{
	int flag;
	int attributecnt;
	jglAttributeBufferAttributeInfo attributes[JGLATTRIBUTEBUFFER_MAXATT_CNT];
	int cnt;
protected:
	virtual void initAttributeInfo( jglAttributeBufferAttributeInfo& info, int exponentNum ) = 0;
	jglBufferAttribute();
    void init(int flag);

public:
	void append(unsigned int &offsetCnt, unsigned int unitcnt, int attribute1, const void* src1,
						 int attribute2=0, const void* src2=0,
						 int attribute3=0, const void* src3=0);
	
	void append(unsigned int &offsetCnt, unsigned int unitcnt, int datacnt, const jglAttributeData datas[]);
};


#endif /* jglattributebuffer_hpp */
