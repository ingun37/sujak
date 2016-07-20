//
//  jcore.hpp
//  MetalVertexStreaming
//
//  Created by Build on 2015. 11. 4..
//  Copyright © 2015년 Apple Inc. All rights reserved.
//

#ifndef jcore_hpp
#define jcore_hpp


#include <stdio.h>
#include <string.h>
#include "jgl.hpp"
#include "jos.h"
#include "jallocator.hpp"
namespace sujak
{
    class jobjinfo_static
    {
    public:
        char name[16];
        simd::float3 pos;
        jobjinfo_static(char* _name, float x, float y, float z)
        {
            strcpy(name, _name);
            pos = simd::float3{x, y, z};
        }
    };
    class jskeleton;
    
    
    
    typedef jallocator<char, 1024*1024, class jcore> filepool;
    
    class jcore
    {
        jgl* gl;
        jos* os;
        filepool fpool;
        bool loadstaticdone;
        char* loadfile(const char *name, const char *ext);
    public:
        jcore();
        void init(void *vbuffers[], int *ibuffer, jos* _os, jgl* _gl);
        void loadstatic(jobjinfo_static objinfo);
        void doneloadstatic();
        void update();
        void render();
    };
}


#endif /* jcore_hpp */
