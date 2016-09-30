#ifndef jconstants_h
#define jconstants_h
#include "jdefinitions.hpp"
#include <vector>
namespace sujak
{
    JDataTypeVertex jconstant_vertex_attribute_datatype(JVertexAttribute a)
    {
        switch(a)
        {
            case JVertexAttribute_color: return JDataTypeVertex_f4;
            case JVertexAttribute_normal: return JDataTypeVertex_f4;
            case JVertexAttribute_position: return JDataTypeVertex_f4;
            case JVertexAttribute_uv: return JDataTypeVertex_f2;
            default:
                throw "unknown data type";
        }
    }
    const JDataTypeIndex jconstant_index_datatype = JDataTypeIndex_32;
    
    
    JBuffer jconstant_bufferidx_of(JVertexAttribute a)
    {
        switch(a)
        {
            case JVertexAttribute_color: return JBuffer_vertex_color;
            case JVertexAttribute_normal: return JBuffer_vertex_normal;
            case JVertexAttribute_position: return JBuffer_vertex_position;
            case JVertexAttribute_uv: return JBuffer_vertex_uv;
            default: throw "unknown att att att buff";
        }
    }
    
    const JBuffer jconstant_bufferidx_uniform = JBuffer_uniform;
    
    void jconstant_vertextype_info(const int t, size_t& attnum, JVertexAttribute* attribs)
    {
        std::vector<JVertexAttribute> v;
        for(int i=0;i<JVertexAttribute_number;i++)
        {
            if(t & (1 << i))
            {
                v.push_back((JVertexAttribute)i);
            }
        }
        
        attnum = v.size();
        
        for(int i=0;i<attnum;i++)
            attribs[i] = v[i];
    }
    typedef struct _jtype_pipeline
    {
        JShaderVert v;
        JShaderFrag f;
        JPixelFormat pf_color;
        JPixelFormat pf_depth;
        JPixelFormat pf_stencil;
        int attribs;
        
    }jtype_pipeline;
    
    jtype_pipeline jconstant_programs[JPipeline_number] = {{.v = JShaderVert_color, .f = JShaderFrag_color, .pf_color = JPixelFormat_rgba8, .pf_depth=JPixelFormat_F32, .pf_stencil = JPixelFormat_8, .attribs = (1 << JVertexAttribute_position)|(1 << JVertexAttribute_color)},};
	const JPixelFormat jconstant_framebuffer_pixelformat = JPixelFormat_rgba8;
	const JPixelFormat jconstant_depth_pixelformat = JPixelFormat_F32;
	const JPixelFormat jconstant_stencil_pixelformat = JPixelFormat_8;
}

#endif
