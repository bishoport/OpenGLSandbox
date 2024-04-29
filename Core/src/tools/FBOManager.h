#pragma once

#include "../LibCoreHeaders.h"

namespace libCore {
    class FBOManager {
    public:
        FBOManager();
        ~FBOManager();

        void init(int width, int height);
        void bindFBO();
        void unbindFBO();
        void bindTexture(const std::string& attachmentType, int textureSlot);
        void drawFBO(const std::string& attachmentType);
        GLuint getTexture(const std::string& attachmentType);

    private:
        GLuint framebuffer;
        std::map<std::string, GLuint> attachments;
        GLuint quadVAO, quadVBO;
        int fboWidth, fboHeight;

        void setupFBO();
        void setupAttachments();
        void setupQuad();
        void cleanup();
        void checkFBOStatus();
    };
}



//#pragma once
//
//#include "../LibCoreHeaders.h"
//
//namespace libCore
//{
//    class FBOManager {
//    public:
//        void init();
//        void createFBO(int width, int height);
//        void bindFBO();
//        void unbindFBO();
//        void drawFBO();
//        GLuint getFBOTexture();
//
//        GLuint quadVAO;
//    
//        GLuint framebuffer;
//        GLuint textureColorbuffer;
//        GLuint rbo;
//
//        GLuint quadVBO;
//        void setupQuad();
//
//        float quadVertices[24] = {
//            // positions   // texCoords
//            -1.0f,  1.0f,  0.0f, 1.0f,
//            -1.0f, -1.0f,  0.0f, 0.0f,
//             1.0f, -1.0f,  1.0f, 0.0f,
//
//            -1.0f,  1.0f,  0.0f, 1.0f,
//             1.0f, -1.0f,  1.0f, 0.0f,
//             1.0f,  1.0f,  1.0f, 1.0f
//        };
//    };
//}
//
