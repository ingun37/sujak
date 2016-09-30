//
//  jmetalrenderobj.h
//  jmetal
//
//  Created by Ingun Jon on 10/4/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <vector>
#import <Foundation/Foundation.h>
#import "jmetalbufferalign.hpp"

typedef std::vector<jmetalbufferalign> vec_jmetalbufferalign;
typedef struct _jmetalrenderobj
{
    int vertextype;
    vec_jmetalbufferalign aligns;
    _jmetalrenderobj(int vertextype)
    {
        this->vertextype = vertextype;
    }
}jmetalrenderobj;
