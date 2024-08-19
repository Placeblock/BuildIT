//
// Created by felix on 8/19/24.
//

#ifndef BUILDIT_FONT_H
#define BUILDIT_FONT_H


#include "graphics/font/fontLoader.h"
#include "graphics/font/fontMetrics.h"

namespace GLOBALS {
    extern Font font;
    extern FontMetrics metrics;
}

void load() {
    FontDataLoader fontDataLoader{"resources/font/data.fnt"};
    fontDataLoader.load();
    FontLoader fontLoader{fontDataLoader.fontData};
    fontLoader.load();
    GLOBALS::font = fontLoader.font;
    GLOBALS::metrics = FontMetrics{fontDataLoader.fontData};
}


#endif //BUILDIT_FONT_H
