#pragma once

#include <glad/glad.h>
#include <map>
#include <string>

namespace libCore {
    class GBufferManager {
    public:
        GBufferManager();
        ~GBufferManager();

        void init(int width, int height);
        void bindGBuffer();
        void unbindGBuffer();
        void bindTexture(const std::string& attachmentType);
        GLuint getTexture(const std::string& attachmentType);

    private:
        GLuint gBuffer;
        std::map<std::string, GLuint> attachments;
        int bufferWidth, bufferHeight;

        void setupGBuffer();
        void cleanup();
        void checkGBufferStatus();
    };
}
