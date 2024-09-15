//
// Created by felix on 8/4/24.
//

#include "instancedMeshRenderer.h"
#include "graphics/util.h"

BufferLayout getInstancedLayout() {
    return BufferLayout{BufferLayoutElement{GL_FLOAT, 2, false, 1}};
}