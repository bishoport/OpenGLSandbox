#pragma once
#include "../LibCoreHeaders.h"

#include <string>
#include "Mesh.h"
#include "Material.h"
#include "skeletal/AnimData.h"

namespace libCore
{
    class Model
    {
    public:

        libCore::Transform transform;

        // model data 
        std::vector<Ref<Mesh>> meshes;
        std::vector<Ref<Material>> materials;

        //Ref<Mesh> unifiedMesh;

        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }

        //void SetupUnifyMeshes();

        void Draw(const std::string& shader);

        // SKELETAL DATA
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        int m_BoneCounter = 0;

    };
}
