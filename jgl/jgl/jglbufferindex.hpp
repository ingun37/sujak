//
//  jglbufferindex.hpp
//  jgl
//
//  Created by ingun on 20/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglbufferindex_hpp
#define jglbufferindex_hpp

class jglBuffer;
class jglBufferIndex
{
	jglBuffer* buffer;
	unsigned int cnt;
protected:
	virtual unsigned int unitSize() = 0;
	virtual jglBuffer* makeIndexBuffer(unsigned int reserveCnt) = 0;
	void init(unsigned int reserveCnt);
	jglBufferIndex();
public:
	void append(unsigned int cnt, void* src, unsigned int &offsetCnt);
};

#endif /* jglbufferindex_hpp */
