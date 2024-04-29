#pragma once
#include "../LibCoreHeaders.h"

#include <string>
#include "Mesh.h"
#include "Material.h"

class Model
{
public:

    libCore::Transform transform;

    // model data 
    std::vector<Ref<Mesh>> meshes;
    std::vector<Ref<Material>> materials;
    
    void Draw(const std::string& shader);
};