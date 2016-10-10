//
//  testview.m
//  jmetal
//
//  Created by ingun on 10/5/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "testview.hpp"
#import <QuartzCore/CAMetalLayer.h>
#import "jmetal.hpp"
#import "jconstants.hpp"
#import "../../jlinear/jlinear/jlinear.hpp"
using namespace sujak;
jmetal gl;

simd::float4 pos[] = {
	{-20, 20, -20, 1},
	{ 20, 20, -29, 1},
	{ 20, -20, -29, 1},
	{-20, -28, -30, 1},
};

simd::float4 col[] = {
	{ 1, 0, 0, 1},
	{ 1, 0, 0, 1},
	{ 1, 0, 0, 1},
	{ 1, 0, 0, 1},
};

unsigned int idxs[] = {
	0, 1, 3,
	3, 1, 2,
};
int vtype = (1 << JVertexAttribute_position) | (1 << JVertexAttribute_color);

@implementation testview
{
	__weak CAMetalLayer *metallayer;
}
-(void)initcommon
{
	metallayer = (CAMetalLayer*)self.layer;
	if(metallayer == nil)
		[NSException raise:@"cametallayer is null" format:@""];
	
	CGSize drawableSize = self.bounds.size;
	UIScreen* screen = self.window.screen ?: [UIScreen mainScreen];
	drawableSize.width *= screen.nativeScale;
	drawableSize.height *= screen.nativeScale;
	
	JUniform uni;
	uni.ortho = jlinear::GetProjectionMatrixOrthogonal(drawableSize.width/2, drawableSize.height/2, 1, 100);
	uni.view = jlinear::GetViewMatrix({0,0,0}, {0,1,0}, {0,0,-1});
	gl.init(metallayer, drawableSize, uni);
	
}
-(instancetype)initWithCoder:(NSCoder *)aDecoder
{
	
	self = [super initWithCoder:aDecoder];
	[self initcommon];
	return self;
}
-(instancetype)initWithFrame:(CGRect)frame
{
	
	self =  [super initWithFrame:frame];
	[self initcommon];
	return self;
}
-(instancetype)init
{
	self = [super init];
	[self initcommon];
	return self;
}
+(Class)layerClass
{
	return [CAMetalLayer class];
}
-(void)display
{
	gl.render();
}
-(void)update
{
	void* vdatas[2] ={pos, col};
	id<CAMetalDrawable> drawable = [metallayer nextDrawable];
	if(drawable == nil)
		return;
	NSUInteger h = [[drawable texture] height];
	NSUInteger w = [[drawable texture] width];
	id<MTLTexture> tt = drawable.texture;
	h = tt.height;
	w = tt.width;
	gl.draw(vdatas, vtype, 4, idxs, 6, [drawable texture], sujak::JPipeline_color, drawable);
	
}

@end
