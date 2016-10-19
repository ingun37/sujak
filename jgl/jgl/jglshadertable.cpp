//
//  jglshaderpool.cpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jglshadertable.hpp"
using namespace sujak;
void jglShaderTable::init()
{
	jglNonTransientTable<jglShader*>::initOrdered(JPipeline_number);
}
