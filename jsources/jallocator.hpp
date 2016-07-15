//
//  jallocator.hpp
//  MetalVertexStreaming
//
//  Created by Build on 11/27/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#ifndef jallocator_hpp
#define jallocator_hpp

#include <stdio.h>
#include <stdlib.h>
template <typename T, unsigned long I>
class jallocator
{
	static T objs[I];
	static unsigned long cnt;
public:
	
	static T* getAvailable(unsigned long len);
	static T* getStart();
	static T* getAt(unsigned long i);
	static unsigned long getCnt();
};


template <typename T, unsigned long I>
unsigned long jallocator<T,I>::cnt = 0;

template <typename T, unsigned long I>
T jallocator<T,I>::objs[];

template<typename T, unsigned long I>
T* jallocator<T, I>::getAvailable(unsigned long len)
{
	T* obj = &objs[cnt];
	cnt += len;
	if(cnt > I)
	{
		puts("jallocator err");
		exit(1);
	}
	return obj;
}

template<typename T, unsigned long I>
T* jallocator<T,I>::getStart()
{
	return objs;
}

template<typename T, unsigned long I>
T* jallocator<T,I>::getAt(unsigned long i)
{
	return &objs[i];
}

template <typename T, unsigned long I>
unsigned long jallocator<T,I>::getCnt()
{
	return cnt;
}

#endif /* jallocator_hpp */