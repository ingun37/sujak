//
//  jglShaderpool.cpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#include "jconstants.hpp"
#include "jglTableShader.hpp"
#include "jglShader.hpp"
using namespace sujak;

void jglTableShader::init()
{
    jglTableNonTransientArray<jglShader*>::init(JShader_number);
}
jglShader* jglTableShader::makeOf(unsigned int k)
{
    jinfo_shader p = jconstant_shaders((JShader) k);
    return shaderOf(p.v, p.f);
}
