#include "GBufferManager.h"
#include <iostream>

namespace libCore {

    GBufferManager::GBufferManager() : gBuffer(0), bufferWidth(0), bufferHeight(0) {}

    GBufferManager::~GBufferManager() {
        cleanup();
    }

    void GBufferManager::init(int width, int height) {
        bufferWidth = width;
        bufferHeight = height;
        setupGBuffer();
        checkGBufferStatus();
    }

    void GBufferManager::bindGBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    }

    void GBufferManager::unbindGBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GBufferManager::bindTexture(const std::string& attachmentType) {
        glActiveTexture(GL_TEXTURE0); // You can choose other texture units if needed.
        glBindTexture(GL_TEXTURE_2D, attachments[attachmentType]);
    }

    GLuint GBufferManager::getTexture(const std::string& attachmentType) {
        return attachments[attachmentType];
    }

    void GBufferManager::setupGBuffer() {
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        // Position attachment
        GLuint positionTexture;
        glGenTextures(1, &positionTexture);
        glBindTexture(GL_TEXTURE_2D, positionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexture, 0);

        // Normal attachment
        GLuint normalTexture;
        glGenTextures(1, &normalTexture);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, bufferWidth, bufferHeight, 0, GL_RGB, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);

        // Albedo attachment
        GLuint albedoTexture;
        glGenTextures(1, &albedoTexture);
        glBindTexture(GL_TEXTURE_2D, albedoTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferWidth, bufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTexture, 0);

        // Attachments list
        GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        // Depth attachment
        GLuint depthTexture;
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        this->attachments["position"] = positionTexture;
        this->attachments["normal"] = normalTexture;
        this->attachments["albedo"] = albedoTexture;
        this->attachments["depth"] = depthTexture;

        unbindGBuffer(); // Unbind the GBuffer after setup is complete
    }

    void GBufferManager::cleanup() {
        // Cleanup all the textures and framebuffer
        for (auto& attachment : attachments) {
            glDeleteTextures(1, &attachment.second);
        }
        glDeleteFramebuffers(1, &gBuffer);
    }

    void GBufferManager::checkGBufferStatus() {
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::GBufferManager::Framebuffer is not complete!" << std::endl;
        unbindGBuffer();
    }

} // namespace libCore

