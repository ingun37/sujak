//
//  jwritehandler.h
//  sujak
//
//  Created by ingun on 18/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jwritehandler_h
#define jwritehandler_h
#include <iostream>
namespace sujak
{
	class jWriteHandler
	{
		void* dst;
		unsigned int pointer;
		unsigned int limit;
	public:
		jWriteHandler& operator=(const jWriteHandler& r)
		{
			throw "no copy no copy";
		}
		void init(void* dst, unsigned int limit)
		{
			this->dst = dst;
			this->limit = limit;
			pointer = 0;
		}
		void write(const void* src, unsigned int len)
		{
			if(pointer + len <= pointer || pointer + len <= len)
				throw "write exceeddeed";
			if(pointer + len > limit)
				throw "write exceed";
			memcpy((char*)dst + pointer, src, len);
			pointer += len;
		}
	};
}
#endif /* jwritehandler_h */
