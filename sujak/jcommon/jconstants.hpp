#ifndef jconstants_h
#define jconstants_h
#include "jdefinitions.hpp"
#include <vector>
namespace sujak
{
    JDataType jconstant_attribute_datatype(JVertexAttribute a)
    {
        switch(a)
        {
            case JVertexAttribute_color: return JDataType_f4;
            case JVertexAttribute_normal: return JDataType_f4;
            case JVertexAttribute_position: return JDataType_f4;
            case JVertexAttribute_uv: return JDataType_f2;
            default: throw "unknown data type";
        }
    }
	
	JDataType jconstant_attribute_datatype(JInstanceAttribute a)
	{
		switch(a)
		{
			case JInstanceAttribute_trans: return JDataType_f44;
			default: throw "unknowns  isudin instance attt attt";
		}
	}
	
    const JDataType jconstant_index_datatype = JDataType_uint32;
    
    
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
	
	JBuffer jconstant_bufferidx_of(JInstanceAttribute a)
	{
		switch(a)
		{
			case JInstanceAttribute_trans: return JBuffer_instance;
			default: throw "unknonw jsidfji instance type";
		}
	}
	
    const JBuffer jconstant_bufferidx_uniform = JBuffer_uniform;
    
    void jconstant_attributes_of_flag(const int f, size_t& attnum, JVertexAttribute* attribs)
    {
		int cnt = 0;
        for(int i=0;i<JVertexAttribute_number;i++)
        {
            if(f & (1 << i))
            {
				attribs[cnt++] = (JVertexAttribute)i;
            }
        }
        
        attnum = cnt;
    }
	
	void jconstant_attributes_of_flag(const int f, size_t& attnum, JInstanceAttribute* attribs)
	{
		int cnt = 0;
		for(int i=0;i<JInstanceAttribute_number;i++)
		{
			if(f & (1 << i))
			{
				attribs[cnt++] = (JInstanceAttribute)i;
			}
		}
		
		attnum = cnt;
	}
    typedef struct _jtype_pipeline
    {
        JShaderVert v;
        JShaderFrag f;
        JPixelFormat pf_color;
        JPixelFormat pf_depth;
        JPixelFormat pf_stencil;
        int vertexFlag;
		int instanceFlag;
    }jtype_pipeline;
	
	const int jconstant_attribute_group_instance_trans = (1 << JAttributeGroupInstance_trans);
	int jconstant_attribute_group( JAttributeGroupInstance ag)
	{
		switch(ag)
		{
			case JAttributeGroupInstance_trans: return jconstant_attribute_group_instance_trans;
			default: throw "923980234902903tr0293t9";
		}
	}
		
	const int jconstant_attribute_group_vertex_position_color = (1 << JAttributeGroupVertex_position_color);
	int jconstant_attribute_group( JAttributeGroupVertex ag)
	{
		switch(ag)
		{
			case JAttributeGroupVertex_position_color: return jconstant_attribute_group_vertex_position_color;
			default: throw "9w23902309r293r2903r902390r23rgsgcvcvbc";
		}
	}
	
    const jtype_pipeline jconstant_program_color = {
		.v = JShaderVert_color,
		.f = JShaderFrag_color,
		.pf_color = JPixelFormat_rgba8,
		.pf_depth=JPixelFormat_F32,
		.pf_stencil = JPixelFormat_8,
		.vertexFlag = jconstant_attribute_group_vertex_position_color,
		.instanceFlag = jconstant_attribute_group_instance_trans,
	};
		
	const jtype_pipeline& jconstant_programs(JPipeline p)
	{
		switch(p)
		{
			case JPipeline_color: return jconstant_program_color;
			default: throw "ijeif no p";
		}
	}
	
	const JPixelFormat jconstant_framebuffer_pixelformat = JPixelFormat_rgba8;
	const JPixelFormat jconstant_depth_pixelformat = JPixelFormat_F32;
	const JPixelFormat jconstant_stencil_pixelformat = JPixelFormat_8;
}

#endif