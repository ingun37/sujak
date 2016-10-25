#include <simd/simd.h>
#import <UIKit/UIKit.h>



#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

#import "jdefinitions.hpp"
#import "jmetalrendertransients.hpp"


class jmetal
{
	id<MTLCommandQueue> commandqueue;
	id<MTLLibrary> library;
	jmetalrendertransients rendertransients;
	
	public:
	id<MTLDevice> device;
	
	void draw(void **vdatas, int vtype, unsigned int vcnt, void *idata, unsigned int icnt, id<MTLTexture> ctarget, sujak::JShader p, id<MTLDrawable> drawableToPresent);
	void draw(void** vdatas, int vtype, unsigned int vcnt, void* idata, unsigned int icnt,
			  id<MTLTexture> ctarget, sujak::JShader p, id<MTLDrawable> drawableToPresent,
			  id<MTLTexture> dtarget,
			  id<MTLTexture> starget
			  );
	void updateUniform(sujak::JUniform uni);
	void render();
	void init(CAMetalLayer* metallayer, CGSize drawableSize, sujak::JUniform initUniform);
	
	
	
};
