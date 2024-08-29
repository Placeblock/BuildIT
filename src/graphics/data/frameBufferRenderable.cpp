//
// Created by felix on 8/10/24.
//

#include "frameBufferRenderable.h"
#include "glm/vec2.hpp"

FrameBufferRenderable::FrameBufferRenderable(intVec2 size) : size(size) {
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glGenTextures(1, &this->frameTexture);
    glBindTexture(GL_TEXTURE_2D, this->frameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->frameTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferRenderable::updateFrameBufferSize(intVec2 newSize) {
    glBindTexture(GL_TEXTURE_2D, this->frameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newSize.x, newSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    this->size = newSize;
}

void FrameBufferRenderable::useFrameBuffer() {
    glViewport(0,0,this->size.x, this->size.y);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
}
