//
//  jglobjvertexdata.cpp
//  jgl
//
//  Created by Ingun Jon on 17/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include <iostream>
#include "jglobjvertexdatahandle.hpp"

jglObjVertexDataHandle::jglObjVertexDataHandle()
{
    for(int i=0;i<sujak::JVertexAttribute_number;i++)
        datas[i] = NULL;
}
