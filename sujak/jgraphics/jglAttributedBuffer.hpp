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

typedef struct _jglAttributedBufferAttributeInfo
{
	unsigned short unitsize;
	jglBuffer* buffer;
	
    _jglAttributedBufferAttributeInfo()
    {
        unitsize = 0;
        buffer = 0;
    }
} jglAttributedBufferAttributeInfo;

typedef struct _jglAttributedBufferData
{
	int attribute;
	const void* src;
} jglAttributedBufferData;


class jglAttributedBuffer
{
	int flag;
	int attributecnt;
	jglAttributedBufferAttributeInfo attributes[JGLATTRIBUTEBUFFER_MAXATT_CNT];
	int cnt;
protected:
	virtual void initAttributeInfo( jglAttributedBufferAttributeInfo& info, int exponentNum ) = 0;
	jglAttributedBuffer();
    void init(int flag);

public:
	void append(unsigned int &offsetCnt, unsigned int unitcnt, int attribute1, const void* src1,
						 int attribute2=0, const void* src2=0,
						 int attribute3=0, const void* src3=0);
	
	void append(unsigned int &offsetCnt, unsigned int unitcnt, int datacnt, const jglAttributedBufferData datas[]);
};


#endif /* jglattributebuffer_hpp */
