//
//  jvector.h
//  MetalVertexStreaming
//
//  Created by ingun on 4/21/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#ifndef jvector_h
#define jvector_h

#include <vector>
#include <iostream>
template <typename T>
class jvector : public std::vector
{
public:
    size_t size_type()
    {
        return sizeof(T);
    }
};
#endif /* jvector_h */
