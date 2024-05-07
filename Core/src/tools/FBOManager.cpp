#include "FBOManager.h"
#include <iostream>

namespace libCore {

    FBOManager::FBOManager()
        : framebuffer(0), quadVAO(0), quadVBO(0), fboWidth(0), fboHeight(0) {
    }

    FBOManager::~FBOManager() {
        cleanup();
    }

    void FBOManager::init(int width, int height) {
        fboWidth = width;
        fboHeight = height;
        setupFBO();
        setupAttachments();
        setupQuad();
        checkFBOStatus();
    }

    void FBOManager::bindFBO() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }

    void FBOManager::unbindFBO() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBOManager::bindTexture(const std::string& attachmentType,int textureSlot) {
        glActiveTexture(GL_TEXTURE0 + textureSlot); // You can choose other texture units if needed.
        glBindTexture(GL_TEXTURE_2D, attachments[attachmentType]);
    }

    void FBOManager::drawFBO(const std::string& attachmentType) {
        //bindTexture(attachmentType);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    GLuint FBOManager::getTexture(const std::string& attachmentType) {
        return attachments[attachmentType];
    }

    void FBOManager::setupFBO() {
        glGenFramebuffers(1, &framebuffer);
        bindFBO();
    }

    void FBOManager::setupAttachments() {

        // Color attachment
        GLuint colorAttachment;
        glGenTextures(1, &colorAttachment);
        glBindTexture(GL_TEXTURE_2D, colorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);
        attachments["color"] = colorAttachment;

        // Depth attachment
        GLuint depthAttachment;
        glGenTextures(1, &depthAttachment);
        glBindTexture(GL_TEXTURE_2D, depthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, fboWidth, fboHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

        attachments["depth"] = depthAttachment;

        unbindFBO();
    }


    void FBOManager::setupQuad() {
        float quadVertices[] = {
            // positions  // texCoords
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);
    }

    void FBOManager::cleanup() {
        for (auto& attachment : attachments) {
            glDeleteTextures(1, &attachment.second);
        }
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }

    void FBOManager::checkFBOStatus() {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

}