#ifndef jconstants_h
#define jconstants_h

#include "jdefinitions.hpp"

namespace sujak
{
	
    extern jconstantvertexatt jconstant_vertex_attributes[ JVertexAttribute_number ];
    extern jconstantbuffer jconstant_buffers[ JBuffer_number ];
    extern jconstantrenderstatecolor jconstant_states_color[ JRenderStateColor_number ];
    extern jconstantrenderstatedepthstencil jconstant_states_depthstencil[ JRenderStateDepthStencil_number ];
    extern jconstantrendercontext jconstant_contexts[ JRenderContext_number ];
	extern JDataTypeIndex jconstant_index_type;
	extern JPixelFormat jconstant_pixelformat_color;
	extern JPixelFormat jconstant_pixelformat_depth;
	extern JPixelFormat jconstant_pixelformat_stencil;
}
#endif /* jconstants_h */
