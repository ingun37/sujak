//
//  jglobjvertexdata.hpp
//  jgl
//
//  Created by Ingun Jon on 17/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jglobjvertexdatahandle_hpp
#define jglobjvertexdatahandle_hpp

#include "jdefinitions.hpp"

class jglObjVertexDataHandle
{
public:
    unsigned int offset;
    unsigned int len;
    void* datas[sujak::JVertexAttribute_number];
    jglObjVertexDataHandle();
};
#endif /* jglobjvertexdata_hpp */
