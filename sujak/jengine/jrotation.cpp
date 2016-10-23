#include "jrotation.hpp"

sujak::jrotation::jrotation()
{
    xyz = {0,0,0};
}

sujak::jrotation::jrotation(simd::float3 _xyz)
{
    xyz = _xyz;
}