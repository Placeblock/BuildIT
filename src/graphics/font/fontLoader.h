//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_FONTLOADER_H
#define BUILDIT_FONTLOADER_H


#include <GL/glew.h>

#include <utility>
#include "fontDataLoader.h"

class FontLoader {
public:
    explicit FontLoader(FontData data);
    void load();
    std::vector<GLuint> textures;
private:
    const FontData data;
};


#endif //BUILDIT_FONTLOADER_H
