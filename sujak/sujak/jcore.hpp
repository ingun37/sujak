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
#include "jconstants.h"
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
    
    typedef void(*platformSpecificSetRenderState)(JRenderState state);
    typedef void(*platformSpecificSetPrimitive)(JRenderPrimitive prim);
    typedef void(*platformSpecificRenderIndexed)(unsigned long offset, unsigned long cnt);
    typedef void(*platformSpecificGetFile)(const char* szFileName, const char* szExt, char* &file, unsigned long& size);
    typedef void(*platformSpecificGetObjInfo)(const char* jobjname, char* nameskel, char* namemesh, size_t namebufflen);
    typedef void(*loadSkeletonsPlatformSpecific)(jskeleton* objs, const int len, int &cap);
    
    
    typedef jallocator<char, 1024*1024> filepool;
    
    class jcore
    {
        
        filepool fpool;
        bool loadstaticdone;
        platformSpecificGetFile pfgetfile;
        platformSpecificGetObjInfo pfgetobjinfo;
        char* loadfile(const char *name, const char *ext);
    public:
        jcore();
        void init(void *vbuffers[], int *ibuffer, platformSpecificGetFile pGetFile, platformSpecificGetObjInfo pGetObjInfo);
        void loadstatic(jobjinfo_static objinfo);
        void doneloadstatic();
        void update();
        void render( platformSpecificSetRenderState, platformSpecificSetPrimitive, platformSpecificRenderIndexed);
    };
}


#endif /* jcore_hpp */
