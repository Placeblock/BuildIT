//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_FONTDATALOADER_H
#define BUILDIT_FONTDATALOADER_H


#include <string>
#include <utility>
#include <map>
#include <iosfwd>
#include "graphics/types.h"

struct Char {
    unsigned int id; // Unicode-ID
    intVec2 pos; // Position in bitmap
    intVec2 size; // Glyph size
    intVec2 offset; // Glyph offset
    int advance; // Advance in x-direction
};

struct FontData {
    std::string name;
    int size = 0;
    bool bold = false, italic = false;
    intVec2 bitmapSize;
    unsigned int base = 0;
    unsigned int lineHeight = 0;
    std::map<uint, Char> chars;
};

class FontDataLoader {
public:
    explicit FontDataLoader(std::string filePath) : filePath(std::move(filePath)) {};
    void load();
    FontData fontData;
private:
    std::string filePath;
    static std::pair<std::string, std::string> readPair(std::istringstream* lineStream);
};


#endif //BUILDIT_FONTDATALOADER_H
