#ifndef JJoint_h
#define JJoint_h
#include <simd/simd.h>
#include "jrotation.hpp"
#include "jtranslation.hpp"
namespace sujak
{
    class jjoint
    {
    public:
        jtranslation pos;
        jrotation rot;
        JROTATION_ORDER order;
        inline matrix_float4x4 getTransMat()
        {
            return matrix_multiply(pos.getMat(), rot.toMat(order));
        }
        inline matrix_float4x4 getOnlyRotation()
        {
            return rot.toMat(order);
        }
    };
}
#endif /* JJoint_h */
