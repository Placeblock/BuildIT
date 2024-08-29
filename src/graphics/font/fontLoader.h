//
// Created by felix on 8/11/24.
//

#ifndef BUILDIT_FONTLOADER_H
#define BUILDIT_FONTLOADER_H


#include <GL/glew.h>

#include "fontDataLoader.h"

struct Font {
    GLuint texture;
    FontData data;
};

class FontLoader {
public:
    explicit FontLoader(FontData data);
    void load();
    Font font;
};


#endif //BUILDIT_FONTLOADER_H
