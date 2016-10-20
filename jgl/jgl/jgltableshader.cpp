//
//  jglshaderpool.cpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jgltableshader.hpp"
using namespace sujak;
void jglTableShader::init()
{
	jglTableNonTransient<jglShader*>::initOrdered(JPipeline_number);
}
