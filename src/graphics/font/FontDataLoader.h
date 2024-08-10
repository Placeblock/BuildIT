//
// Created by felix on 8/10/24.
//

#ifndef BUILDIT_FONTDATALOADER_H
#define BUILDIT_FONTDATALOADER_H


#include <string>
#include <utility>
#include <vector>
#include <map>
#include "glm/vec2.hpp"

struct Char {
    uint id; // Unicode-ID
    glm::vec2 pos; // Position in bitmap
    glm::vec2 size; // Glyph size
    glm::vec2 offset; // Glyph offset
    int advance; // Advance in x-direction
    uint page;
};

struct Kerning {
    uint firstID;
    uint secondID;
    int amount;
};

struct Page {
    uint id;
    std::string fileName;
};

class FontDataLoader {
public:
    explicit FontDataLoader(std::string filePath) : filePath(std::move(filePath)) {};
    void load();
    std::string name;
    int size = 0;
    bool bold = false, italic = false;
    glm::vec2 bitmapSize;
    uint pageCount;
    uint base = 0;
    uint lineHeight = 0;
    std::map<uint, Char> chars;
    std::vector<Kerning> kernings;
    std::vector<Page> pages;
private:
    std::string filePath;
    static std::pair<std::string, std::string> readPair(std::istringstream* lineStream);
};


#endif //BUILDIT_FONTDATALOADER_H
