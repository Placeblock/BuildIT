//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_FRAMEBUFFERRENDERABLE_H
#define BUILDIT_FRAMEBUFFERRENDERABLE_H


#include <GL/glew.h>
#include "graphics/types.h"

class FrameBufferRenderable {
public:
    explicit FrameBufferRenderable(intVec2 size);
    void updateFrameBufferSize(intVec2 newSize);
    void useFrameBuffer();
    GLuint texture = 0;
private:
    GLuint framebuffer;
protected:
    intVec2 size;
};


#endif //BUILDIT_FRAMEBUFFERRENDERABLE_H
