//
//  jmetalbufferalign.h
//  jmetal
//
//  Created by Ingun Jon on 10/4/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct _jmetalbufferalign
{
    unsigned int voffset;
    unsigned int ioffset;
    unsigned int vcnt;
    unsigned int icnt;
    _jmetalbufferalign(unsigned int voffset, unsigned int ioffset, unsigned int vcnt, unsigned int icnt)
    {
        this->voffset = voffset;
        this->ioffset = ioffset;
        this->vcnt = vcnt;
        this->icnt = icnt;
    }
}jmetalbufferalign;
