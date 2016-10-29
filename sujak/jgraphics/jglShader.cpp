//
//  jglShader.cpp
//  jgl
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jglShader.hpp"
using namespace sujak;

void jglShader::init(sujak::JShader p)
{
	switch(p)
	{
		case sujak::JShader_color:
			vert = JVertexFunction_color;
			frag = JFragmentFunction_color;
			pixelformat_depth = JPixelFormat_F32;
			pixelformat_stencil = JPixelFormat_8;
			break;
		default:
			throw "902iokl234wesdf";
	}
}
