#pragma once

#include "../LibCoreHeaders.h"

namespace libCore {

    class FBO {
    public:

    FBO(): framebuffer(0), quadVAO(0), quadVBO(0), fboWidth(0), fboHeight(0) {}

    ~FBO() {
        cleanup();
    }

    void init(int width, int height) {
        fboWidth = width;
        fboHeight = height;
        setupFBO();
        setupAttachments();
        setupQuad();
        checkFBOStatus();
    }

    void bindFBO() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }

    void unbindFBO() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bindTexture(const std::string& attachmentType,int textureSlot) {
        glActiveTexture(GL_TEXTURE0 + textureSlot); // You can choose other texture units if needed.
        glBindTexture(GL_TEXTURE_2D, attachments[attachmentType]);
    }

    void drawFBO(const std::string& attachmentType) {
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    GLuint getTexture(const std::string& attachmentType) {
        return attachments[attachmentType];
    }

    void setupFBO() {
        glGenFramebuffers(1, &framebuffer);
        bindFBO();
    }

    void setupAttachments() {

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

    void resize(int newWidth, int newHeight) {
        fboWidth = newWidth;
        fboHeight = newHeight;

        bindFBO();
        // Resize color attachment
        glBindTexture(GL_TEXTURE_2D, attachments["color"]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        // Resize depth attachment
        glBindTexture(GL_TEXTURE_2D, attachments["depth"]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, fboWidth, fboHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        unbindFBO();
    }

    void cleanup() {
        for (auto& attachment : attachments) {
            glDeleteTextures(1, &attachment.second);
        }
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }

    void checkFBOStatus() {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }




    private:
        GLuint framebuffer;
        std::map<std::string, GLuint> attachments;
        int fboWidth, fboHeight;


        GLuint quadVAO, quadVBO;
        void setupQuad() {
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
    };
}