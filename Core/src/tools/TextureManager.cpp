#include "TextureManager.h"

namespace libCore
{
    Ref<Texture> TextureManager::LoadTexture(const char* image, const char* texType, GLuint slot)
    {
        auto texture = CreateRef<Texture>(image, "diffuse", 0);
        return texture;
    }
}
