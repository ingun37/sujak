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
namespace sujak
{
    template <typename T, unsigned long I>
    class jallocator
    {
        T objs[I];
        unsigned long cnt;
    public:
        jallocator()
		{
			cnt = 0;
		}
        T* getAvailable(unsigned long len);
        T* getStart();
        T* getAt(unsigned long i);
        unsigned long getCnt();
    };
    
    template<typename T, unsigned long I>
    T* jallocator<T, I>::getAvailable(unsigned long len)
    {
		if(cnt + len >= I)
			throw "jallocaatatttoror iriii i i i";

        T* obj = &objs[cnt];
        cnt += len;
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
}
#endif /* jallocator_hpp */
