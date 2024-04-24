#pragma once

#include "../LibCoreHeaders.h"

namespace libCore 
{
    class PrimitivesHelper {

    public:
        static Ref<Mesh> CreateQuad();
        static Ref<Mesh> CreatePlane();
        static Ref<Mesh> CreateCube();
        static Ref<Mesh> CreateSphere(float radius, unsigned int sectorCount, unsigned int stackCount);
    };
}