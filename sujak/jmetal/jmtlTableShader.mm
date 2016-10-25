//
//  jmtlShadertable.cpp
//  jmetal
//
//  Created by ingun on 19/10/2016.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#include "jmtlTableShader.hpp"
#include "jmtlShader.hpp"
#include "jallocator.hpp"

using namespace sujak;
void jmtlTableShader::init(id<MTLDevice> device)
{
	jglTableShader::init();
	this->device = device;
	library = [device newDefaultLibrary];
	if(library == nil)
	{
		NSString *path = [[NSBundle mainBundle] pathForResource:@"jmetalshader" ofType:@"metallib"];
		
		library = [device newLibraryWithFile:path error:nil];
		if(library == nil)
		{
			[NSException raise:@"library is no" format:@""];
		}
	}
	jglTableShader::init();
}

typedef jallocator<jmtlShader, 4, jmtlTableShader> shaderpool;
jglShader* jmtlTableShader::makeOf(int key)
{
	jmtlShader* shader = shaderpool::getAvailable(1);
	shader->init((JShader)key, device, library);
	return shader;
}
