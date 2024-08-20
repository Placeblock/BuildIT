//
// Created by felix on 8/10/24.
//

#include <fstream>
#include <sstream>
#include "fontDataLoader.h"

void FontDataLoader::load() {
    std::ifstream infile(this->filePath);

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream lineStream(line);
        std::string action;
        lineStream >> action;
        if (action == "info") {
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                if (pair.first == "face") {
                    this->fontData.name = pair.second.substr(1, pair.second.length()-2);
                    continue;
                }
                converter << pair.second;
                if (pair.first == "size") {
                    converter >> this->fontData.size;
                } else if (pair.first == "bold") {
                    converter >> this->fontData.bold;
                } else if (pair.first == "italic") {
                    converter >> this->fontData.italic;
                }
            }
        } else if (action == "common") {
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                converter << pair.second;
                if (pair.first == "lineHeight") {
                    converter >> this->fontData.lineHeight;
                } else if (pair.first == "base") {
                    converter >> this->fontData.base;
                } else if (pair.first == "scaleW") {
                    converter >> this->fontData.bitmapSize.x;
                } else if (pair.first == "scaleH") {
                    converter >> this->fontData.bitmapSize.y;
                }
            }
        } else if (action == "char") {
            Char newChar{};
            while (!lineStream.eof()) {
                std::stringstream converter;
                std::pair<std::string, std::string> pair = FontDataLoader::readPair(&lineStream);
                converter << pair.second;
                if (pair.first == "id") {
                    converter >> newChar.id;
                } else if (pair.first == "x") {
                    converter >> newChar.pos.x;
                } else if (pair.first == "y") {
                    converter >> newChar.pos.y;
                } else if (pair.first == "width") {
                    converter >> newChar.size.x;
                } else if (pair.first == "height") {
                    converter >> newChar.size.y;
                } else if (pair.first == "xoffset") {
                    converter >> newChar.offset.x;
                } else if (pair.first == "yoffset") {
                    converter >> newChar.offset.y;
                } else if (pair.first == "xadvance") {
                    converter >> newChar.advance;
                }
            }
            this->fontData.chars[newChar.id] = newChar;
        }
    }
}

std::pair<std::string, std::string> FontDataLoader::readPair(std::istringstream* lineStream) {
    std::string pairString;
    *lineStream >> pairString;
    std::size_t i = pairString.find('=');
    std::string key = pairString.substr(0, i);
    std::string value = pairString.substr(i+1);
    return {key, value};
}
