#include "jrotation.hpp"

jrotation::jrotation()
{
    xyz = {0,0,0};
}

jrotation::jrotation(simd::float3 _xyz)
{
    xyz = _xyz;
}