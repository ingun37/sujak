

#import "jmetalrendertransients.hpp"


jmetalrenderingwork* jmetalrendertransients::getormake( id<MTLTexture> color, id<MTLTexture> depth, id<MTLTexture> stencil, id<MTLDrawable> drawable)
{
	jmetalrenderingwork* work = NULL;
	for(vec_jmetalrenderingwork::iterator iwork = renderingworks.begin() ; iwork!=renderingworks.end();iwork++)
	{
		if(iwork->colortarget == color && iwork->depthtarget == depth && iwork->stenciltarget == stencil && iwork->drawable == drawable)
		{
			work = &(*iwork);
			break;
		}
	}
	if(work == NULL)
	{
		renderingworks.push_back(jmetalrenderingwork(color, depth, stencil, drawable));
		work = &(renderingworks[ renderingworks.size() - 1 ]);
	}
	return work;
}
void jmetalrendertransients::clear()
{
	renderingworks.clear();
}

